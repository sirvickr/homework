#include "MainService.h"
#include "ThreadPool.h"
#include "Strsafe.h"

using namespace std;

#ifdef LOCAL_FILE_LOGGER
#define Log(stream) if(logFile) { stream; fflush(logFile); }
#else
#define Log(stream) 
#endif

static const PWSTR lpcszIniFileName = TEXT("lab3.ini");

CMainService::CMainService(PWSTR pszServiceName,
	BOOL fCanStop,
	BOOL fCanShutdown,
	BOOL fCanPauseContinue)
	: CServiceBase(pszServiceName, fCanStop, fCanShutdown, fCanPauseContinue)
{
#ifdef LOCAL_FILE_LOGGER
	logFile = _wfopen(TEXT("E:\\Laba\\lab3.log"), TEXT("wt"));
#endif
	// определим местонахождение исполняемого файла и 
	// установим имя ini-файла, а также значения по умолчанию 
	// для m_szSrcPath и m_szDstPath
	DWORD dwLen = GetModuleFileName(NULL, szIniFileName, MAX_PATH);
	if (dwLen)
	{
		Log(fwprintf(logFile, TEXT("module path = %s\n"), szIniFileName));
		for (int i = int(dwLen - 1); i >= 0; i--)
		{
			if (szIniFileName[i] == TEXT('\\'))
			{
				szIniFileName[i + 1] = TEXT('\0');
				wcscpy(m_szSrcPath, szIniFileName);
				wcscat(m_szSrcPath, TEXT("source"));
				Log(fwprintf(logFile, TEXT("m_szSrcPath = %s\n"), m_szSrcPath));
				wcscpy(m_szDstPath, szIniFileName);
				wcscat(m_szDstPath, TEXT("target"));
				Log(fwprintf(logFile, TEXT("m_szDstPath = %s\n"), m_szDstPath));
				wcscat(szIniFileName, lpcszIniFileName);
				Log(fwprintf(logFile, TEXT("szIniFileName = %s\n"), szIniFileName));
				break;
			}
		}
	}
	// создадим объект синхронизации, через который 
	// будем дожидаться остановки главного потока
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
#ifdef LOCAL_FILE_LOGGER
	if (logFile)
	{
		fclose(logFile);
		logFile = nullptr;
	}
#endif
}

void CMainService::OnStart(DWORD dwArgc, LPWSTR *lpszArgv)
{
	// Запись в Журнал приложений
	WriteEventLogEntry(TEXT("BackupService: OnStart"), EVENTLOG_INFORMATION_TYPE);

	GetPrivateProfileString(TEXT("common"), TEXT("SourceDir"), TEXT("failed"), m_szSrcPath, MAX_PATH, szIniFileName);
	GetPrivateProfileString(TEXT("common"), TEXT("TargetDir"), TEXT("failed"), m_szDstPath, MAX_PATH, szIniFileName);
	Log(fwprintf(logFile, TEXT("m_szSrcPath = %s\n"), m_szSrcPath));
	Log(fwprintf(logFile, TEXT("m_szDstPath = %s\n"), m_szDstPath));

	// При запуске сохраняем всё содержимое директории
	if (!CreateDirectory(m_szSrcPath, NULL)) // на случай, если такая не существует
	{
		BackupDir(m_szSrcPath, m_szDstPath, TEXT("*"));
	}

	memset(&DirInfo, 0x00, sizeof(DIRECTORY_INFO));

	// Открыть исходную директорию (получить дескриптор)
	Log(fwprintf(logFile, TEXT("CreateFile (%s)\n"), m_szSrcPath));
	DirInfo.hDir = CreateFile(m_szSrcPath,
		FILE_LIST_DIRECTORY,
		FILE_SHARE_READ 
		| FILE_SHARE_WRITE
		| FILE_SHARE_DELETE
		,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS |
		FILE_FLAG_OVERLAPPED,
		NULL);
	if (DirInfo.hDir == INVALID_HANDLE_VALUE)
	{
		WriteEventLogEntry(TEXT("BackupService: Failed to open source directory"), EVENTLOG_ERROR_TYPE);
	}

	lstrcpy(DirInfo.lpszDirName, m_szSrcPath);
	// Открыть "порт завершения"
	hCompPort = CreateIoCompletionPort(DirInfo.hDir, hCompPort, (DWORD)&DirInfo, 0);

	// Заупустить асинхронное слежение
	BOOL retc = ReadDirectoryChangesW(DirInfo.hDir,  // HANDLE TO DIRECTORY
		DirInfo.lpBuffer,                // Formatted buffer into which read results are returned.  This is a 
		MAX_BUFFER,                         // Length of previous parameter, in bytes
		TRUE,                               // Monitor sub trees?
		dwNotifyFilter,                     // What we are watching for
		&DirInfo.dwBufLength,            // Number of bytes returned into second parameter
		&DirInfo.Overlapped,             // OVERLAPPED structure that supplies data to be used during an asynchronous operation.  If this is NULL, ReadDirectoryChangesW does not return immediately.
		NULL);                              // Completion routine
	if (!retc)
	{
		WriteEventLogEntry(TEXT("OnStart: ReadDirectoryChangesW failed"), EVENTLOG_ERROR_TYPE);
	}

	// Запуск основного рабочего потока
	CThreadPool::QueueUserWorkItem(&CMainService::ServiceWorkerThread, this);
}

// Основной рабочий поток службы
void CMainService::ServiceWorkerThread(void)
{
	Log(fwprintf(logFile, TEXT("CServiceBase::ServiceWorkerThread enter\n")));

	DWORD numBytes = 0;
	DWORD cbOffset = 0;
	DIRECTORY_INFO* di = NULL;
	LPOVERLAPPED lpOverlapped = NULL;
	PFILE_NOTIFY_INFORMATION fni = NULL;
	TCHAR FileName[MAX_PATH];
	TCHAR szSourceName[MAX_PATH];
	TCHAR szTargetName[MAX_PATH];
	TCHAR szOldName[MAX_PATH];

	WIN32_FIND_DATA ffd;

	do
	{
		// Получить из очереди очередное соощение об изменении нашей директории
		// (блокирующий вызов)
		GetQueuedCompletionStatus((HANDLE)hCompPort,
			&numBytes,
			(LPDWORD)&di,///PULONG_PTR (x64) // этот указатель мы передали ранее в CreateIoCompletionPort()
			&lpOverlapped,
			INFINITE);

		if (di)
		{

			Log(fwprintf(logFile, TEXT("Notify (cbOffset %u)...\n"), cbOffset));
			fni = (PFILE_NOTIFY_INFORMATION)di->lpBuffer;

			do
			{
				cbOffset = fni->NextEntryOffset;

				lstrcpyn(FileName, fni->FileName, fni->FileNameLength / sizeof(WCHAR) + 1);
				FileName[fni->FileNameLength / sizeof(WCHAR) + 1] = '\0';
				swprintf(szSourceName, TEXT("%s\\%s"), m_szSrcPath, FileName);
				swprintf(szTargetName, TEXT("%s\\%s"), m_szDstPath, FileName);

				HANDLE hFind = FindFirstFile(szSourceName, &ffd);
				if (INVALID_HANDLE_VALUE == hFind && fni->Action != FILE_ACTION_RENAMED_OLD_NAME && fni->Action != FILE_ACTION_REMOVED)
				{
					Log(fwprintf(logFile, TEXT("(FindFirstFile failed:%08X)"), ffd.dwFileAttributes));
				}
				Log(fwprintf(logFile, TEXT("[%u.%u]: "), cbOffset, fni->Action));

				BOOL retc = FALSE;
				switch (fni->Action)
				{
				case FILE_ACTION_ADDED:
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						Log(fwprintf(logFile, TEXT("folder added: %s\n"), szSourceName));
						if (!CreateDirectory(szTargetName, NULL))
						{
							Log(fwprintf(logFile, TEXT("CreateDirectory(%s) failed with error %u\n"), szTargetName, GetLastError()));
							WriteEventLogEntry(TEXT("Main: FolderAdded: CreateDirectory failed"), EVENTLOG_ERROR_TYPE);
						}
					}
					else
					{
						Log(fwprintf(logFile, TEXT("file added: %s\n"), szSourceName));
					}
					break;
				case FILE_ACTION_REMOVED: // из резервной директории ничего не удаляем
#if 0 
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						Log(fwprintf(logFile, TEXT("folder deleted: %s\n"), szSourceName));
					}
					else
					{
						Log(fwprintf(logFile, TEXT("file deleted: %s\n"), szSourceName));
						if (!DeleteFile(szTargetName))
						{
							Log(fwprintf(logFile, TEXT("DeleteFile(%s) failed with error %u\n"), szTargetName, GetLastError()));
						}
					}
#endif
					break;
				case FILE_ACTION_MODIFIED:
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						Log(fwprintf(logFile, TEXT("folder modified: %s\n"), szSourceName));
					}
					else
					{
						Log(fwprintf(logFile, TEXT("file modified: %s\n"), szSourceName));
						if (!CopyFile(szSourceName, szTargetName, FALSE))
						{
							Log(fwprintf(logFile, TEXT("\nCopyFile(%s, %s) failed with error %u\n"), szSourceName, szTargetName, GetLastError()));
							WriteEventLogEntry(TEXT("Main: FileModofied: CopyFile failed"), EVENTLOG_ERROR_TYPE);
						}
					}
					break;
				case FILE_ACTION_RENAMED_OLD_NAME:
					wcscpy(szOldName, szTargetName);
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						Log(fwprintf(logFile, TEXT("folder name changed - old name: %s\n"), szSourceName));
					}
					else
					{
						Log(fwprintf(logFile, TEXT("file name changed - old name: %s\n"), szSourceName));
					}
					break;
				case FILE_ACTION_RENAMED_NEW_NAME:
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						Log(fwprintf(logFile, TEXT("folder name changed - new name: %s\n"), szSourceName));
					}
					else
					{
						Log(fwprintf(logFile, TEXT("file name changed - new name: %s\n"), szSourceName));
					}
					if (!MoveFile(szOldName, szTargetName))
					{
						Log(fwprintf(logFile, TEXT("MoveFile(%s, %s) failed with error %u\n"), szOldName, szSourceName, GetLastError()));
						WriteEventLogEntry(TEXT("Main: NewName: MoveFile failed"), EVENTLOG_ERROR_TYPE);
					}
					break;
				default:
					Log(fwprintf(logFile, TEXT("unknown event: %s\n"), szSourceName));
					break;
				}
#ifdef LOCAL_FILE_LOGGER
				fflush(logFile);
#endif

				FindClose(hFind);

				fni = (PFILE_NOTIFY_INFORMATION)((LPBYTE)fni + cbOffset);

			} while (cbOffset);

			// Перезапуск команды слежения
			ReadDirectoryChangesW(di->hDir, di->lpBuffer,
				MAX_BUFFER,
				TRUE,
				dwNotifyFilter,
				&di->dwBufLength,
				&di->Overlapped,
				NULL);
		}
	
	} while (di); // после вызова PostQueuedCompletionStatus() в OnStop() этот указатель будет нулевым

	// Уведомить об остановке
	SetEvent(m_hStoppedEvent);
	Log(fwprintf(logFile, TEXT("CServiceBase::ServiceWorkerThread leave\n")));
}

//
// Вызывается, когда SCM посылает приложению команду Stop
void CMainService::OnStop()
{
	// Запись в Журнал приложений
	WriteEventLogEntry(TEXT("BackupService: OnStop"), EVENTLOG_INFORMATION_TYPE);
	Log(fwprintf(logFile, TEXT("Shutdown the main thread\n")));
	// Разблокировать вызов GetQueuedCompletionStatus() в основном потоке
	PostQueuedCompletionStatus(hCompPort, 0, 0, NULL);
	// Дождаться его завершения
	if (WaitForSingleObject(m_hStoppedEvent, INFINITE) != WAIT_OBJECT_0)
	{
		throw GetLastError();
	}
	// Освободить выделенные ресурсы
	CloseHandle(DirInfo.hDir);
	CloseHandle(hCompPort);
	Log(fwprintf(logFile, TEXT("Done\n")));
}

// рекурсированое копирование содержимого директории szSourceDir в szTargetDir
void CMainService::BackupDir(PWSTR szSourceDir, PWSTR szTargetDir, PWSTR szSourceMask)
{
	//Log(fwprintf(logFile, TEXT("BackupDir \"%s\" (%s) \"%s\"\n"), szSourceDir, szSourceMask, szTargetDir));
	TCHAR szSource[MAX_PATH];
	TCHAR szTarget[MAX_PATH];
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;

	CreateDirectory(szTargetDir, NULL);

	TCHAR szMask[MAX_PATH];
	wsprintf(szMask, TEXT("%s\\%s"), szSourceDir, szSourceMask);
	HANDLE hFind = FindFirstFile(szMask, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		WriteEventLogEntry(TEXT("Main: BackupDir: FindFirstFile failed"), EVENTLOG_ERROR_TYPE);
		return;
	}
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (wcscmp(TEXT("."), ffd.cFileName) && wcscmp(TEXT(".."), ffd.cFileName))
			{
				wsprintf(szTarget, TEXT("%s\\%s"), szTargetDir, ffd.cFileName);
				wsprintf(szSource, TEXT("%s\\"), szSourceDir);
				wcscat(szSource, ffd.cFileName);
				BackupDir(szSource, szTarget, szSourceMask);
			}
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			wsprintf(szSource, TEXT("%s\\%s"), szSourceDir, ffd.cFileName);
			wsprintf(szTarget, TEXT("%s\\%s"), szTargetDir, ffd.cFileName);
			CopyFile(szSource, szTarget, FALSE);
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);
}
