#include "MainService.h"
#include "ThreadPool.h"

using namespace std;

#define Log(stream) if(log) { stream; }
#define Log2(stream) if(logFile) { stream; fflush(logFile); }

static const PWSTR IniFileName = TEXT("E:/Logs/lab3.ini");

CMainService::CMainService(PWSTR pszServiceName,
	BOOL fCanStop,
	BOOL fCanShutdown,
	BOOL fCanPauseContinue)
	: CServiceBase(pszServiceName, fCanStop, fCanShutdown, fCanPauseContinue), log(L"E:/Logs/svc.log")
{
	m_fStopping = FALSE;
	logFile = _wfopen(TEXT("E:/Logs/lab3.log"), TEXT("wt"));

	// Create a manual-reset event that is not signaled at first to indicate 
	// the stopped signal of the service.
	m_hStoppedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_hStoppedEvent == NULL)
	{
		throw GetLastError();
	}
}

CMainService::~CMainService()
{
	if (m_hStoppedEvent)
	{
		CloseHandle(m_hStoppedEvent);
		m_hStoppedEvent = NULL;
	}
	if (logFile)
	{
		fclose(logFile);
		logFile = nullptr;
	}
}

void CMainService::OnStart(DWORD dwArgc, LPWSTR *lpszArgv)
{
	char sz[MAX_PATH];
	Log(log << "CServiceBase::OnStart()" << endl);
	GetPrivateProfileString(TEXT("common"), TEXT("SourceDir"), TEXT("failed"), m_szSrcPath, MAX_PATH, IniFileName);
	GetPrivateProfileString(TEXT("common"), TEXT("DestPath"), TEXT("failed"), m_szDstPath, MAX_PATH, IniFileName);
	GetPrivateProfileString(TEXT("common"), TEXT("DestFile"), TEXT("failed"), m_szDstFile, MAX_PATH, IniFileName);
	DWORD n1 = GetPrivateProfileStringA("common", "SourceDir", "failed", sz, MAX_PATH, "E:/Logs/lab3.ini");
	Log(log << " n = " << n1 << " param = \"" << sz << "\"" << endl);
	Log2(fwprintf(logFile, TEXT("m_szSrcPath = %s\n"), m_szSrcPath));
	Log2(fwprintf(logFile, TEXT("m_szDstPath = %s\n"), m_szDstPath));
	Log2(fwprintf(logFile, TEXT("m_szDstFile = %s\n"), m_szDstFile));

	// Log a service start message to the Application log.
	WriteEventLogEntry(L"CppWindowsService in OnStart", EVENTLOG_INFORMATION_TYPE);

	// Queue the main service function for execution in a worker thread.
	CThreadPool::QueueUserWorkItem(&CMainService::ServiceWorkerThread, this);
}

//
//   FUNCTION: CSampleService::ServiceWorkerThread(void)
//
//   PURPOSE: The method performs the main function of the service. It runs 
//   on a thread pool worker thread.
//
void CMainService::ServiceWorkerThread(void)
{
	Log(log << "CServiceBase::ServiceWorkerThread enter" << endl);
	// Periodically check if the service is stopping.
	while (!m_fStopping)
	{
		// Perform main service function here...

		::Sleep(2000);  // Simulate some lengthy operations.
	}

	// Signal the stopped event.
	SetEvent(m_hStoppedEvent);
	Log(log << "CServiceBase::ServiceWorkerThread leave" << endl);
}


//
//   FUNCTION: CSampleService::OnStop(void)
//
//   PURPOSE: The function is executed when a Stop command is sent to the 
//   service by SCM. It specifies actions to take when a service stops 
//   running. In this code sample, OnStop logs a service-stop message to the 
//   Application log, and waits for the finish of the main service function.
//
//   COMMENTS:
//   Be sure to periodically call ReportServiceStatus() with 
//   SERVICE_STOP_PENDING if the procedure is going to take long time. 
//
void CMainService::OnStop()
{
	// Log a service stop message to the Application log.
	WriteEventLogEntry(L"CppWindowsService in OnStop",
		EVENTLOG_INFORMATION_TYPE);

	// Indicate that the service is stopping and wait for the finish of the 
	// main service function (ServiceWorkerThread).
	m_fStopping = TRUE;
	if (WaitForSingleObject(m_hStoppedEvent, INFINITE) != WAIT_OBJECT_0)
	{
		throw GetLastError();
	}
}