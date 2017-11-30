#include "MainService.h"
#include "ThreadPool.h"
#include "Strsafe.h"
#include "miniz.h"
#include <assert.h>
#include <tchar.h>

using namespace std;

#ifdef LOCAL_FILE_LOGGER
#define Log(stream) if(logFile) { stream; fflush(logFile); }
#else
#define Log(stream) 
#endif

static LPCTSTR lpcszIniFileName = _T("lab3.ini");

CMainService::CMainService(LPCTSTR pszServiceName,
	BOOL fCanStop,
	BOOL fCanShutdown,
	BOOL fCanPauseContinue)
	: CServiceBase(pszServiceName, fCanStop, fCanShutdown, fCanPauseContinue)
{
	file_buff = (char*)malloc(buff_size);
	if (!file_buff) {
		WriteEventLogEntry(_T("Main: file_buff malloc failed"), EVENTLOG_ERROR_TYPE);
	}
#ifdef LOCAL_FILE_LOGGER
	logFile = _wfopen(_T("E:\\Laba\\lab3.log"), _T("wt"));
#endif
	// ��������� ��������������� ������������ ����� � 
	// ��������� ��� ini-�����, � ����� �������� �� ��������� 
	// ��� m_szSrcPath � m_szDstPath
	DWORD dwLen = GetModuleFileName(NULL, szIniFileName, MAX_PATH);
	if (dwLen)
	{
		Log(_ftprintf(logFile, _T("module path = %s\n"), szIniFileName));
		for (int i = int(dwLen - 1); i >= 0; i--)
		{
			if (szIniFileName[i] == _T('\\'))
			{
				szIniFileName[i + 1] = _T('\0');
				_tcscpy(m_szSrcPath, szIniFileName);
				_tcscat(m_szSrcPath, _T("source"));
				Log(_ftprintf(logFile, _T("m_szSrcPath = %s\n"), m_szSrcPath));
				_tcscpy(m_szDstPath, szIniFileName);
				_tcscat(m_szDstPath, _T("target"));
				Log(_ftprintf(logFile, _T("m_szDstPath = %s\n"), m_szDstPath));
				_tcscat(szIniFileName, lpcszIniFileName);
				Log(_ftprintf(logFile, _T("szIniFileName = %s\n"), szIniFileName));
				break;
			}
		}
	}
	// �������� ������ �������������, ����� ������� 
	// ����� ���������� ��������� �������� ������
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
	if (file_buff)
	{
		free(file_buff);
	}
}

void CMainService::OnStart(DWORD dwArgc, LPTSTR *lpszArgv)
{
	// ������ � ������ ����������
	WriteEventLogEntry(_T("BackupService: OnStart"), EVENTLOG_INFORMATION_TYPE);

	GetPrivateProfileString(_T("common"), _T("SourceDir"), _T("c:\\temp1"), m_szSrcPath, MAX_PATH, szIniFileName);
	GetPrivateProfileString(_T("common"), _T("TargetDir"), _T("c:\\temp2"), m_szDstPath, MAX_PATH, szIniFileName);
	
	TCHAR buffer[MAX_PATH];
	GetPrivateProfileString(_T("common"), _T("ZipFile"), _T("backup"), buffer, MAX_PATH, szIniFileName);
	_tcscat(buffer, _T(".zip"));
	WideCharToMultiByte(CP_ACP, 0, buffer, -1, m_szZipFile, _tcslen(buffer) + 1, NULL, NULL);

	Log(_ftprintf(logFile, _T("m_szSrcPath = %s\n"), m_szSrcPath));
	Log(_ftprintf(logFile, _T("m_szDstPath = %s\n"), m_szDstPath));

	// ��� ������� ��������� �� ���������� ����������
	if (!CreateDirectory(m_szSrcPath, NULL)) // �� ������, ���� ����� �� ����������
	{
#if 0
		BackupDir(m_szSrcPath, m_szDstPath, _T("*"));
#else
		szMainArchiveFullFileName[0] = '\0';

		if (_tcslen(m_szDstPath)) {
			CreateDirectory(m_szDstPath, NULL);
			int path_length = wcslen(m_szDstPath);
			if (path_length) {
				WideCharToMultiByte(CP_ACP, 0, m_szDstPath, -1, szMainArchiveFullFileName, path_length + 1, NULL, NULL);
				for (int i = 0; i < path_length; ++i) {
					if (szMainArchiveFullFileName[path_length - 1] == '\\') {
						szMainArchiveFullFileName[path_length - 1] = '/';
					}
				}
				if (szMainArchiveFullFileName[path_length - 1] != '/') {
					strcat(szMainArchiveFullFileName, "/");
				}
			}
		}

		strcat(szMainArchiveFullFileName, m_szZipFile);

		remove(szMainArchiveFullFileName);
		
		ZipDir(szMainArchiveFullFileName, m_szSrcPath, _T(""), _T("*"));
#endif
	}
	
	memset(&DirInfo, 0x00, sizeof(DIRECTORY_INFO));

	// ������� �������� ���������� (�������� ����������)
	Log(_ftprintf(logFile, _T("CreateFile (%s)\n"), m_szSrcPath));
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
		WriteEventLogEntry(_T("BackupService: Failed to open source directory"), EVENTLOG_ERROR_TYPE);
	}

	lstrcpy(DirInfo.lpszDirName, m_szSrcPath);
	// ������� "���� ����������"
	hCompPort = CreateIoCompletionPort(DirInfo.hDir, hCompPort, (DWORD)&DirInfo, 0);

	// ���������� ����������� ��������
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
		WriteEventLogEntry(_T("OnStart: ReadDirectoryChangesW failed"), EVENTLOG_ERROR_TYPE);
	}

	// ������ ��������� �������� ������
	CThreadPool::QueueUserWorkItem(&CMainService::ServiceWorkerThread, this);
}

// �������� ������� ����� ������
void CMainService::ServiceWorkerThread(void)
{
	Log(_ftprintf(logFile, _T("CServiceBase::ServiceWorkerThread enter\n")));

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
		// �������� �� ������� ��������� �������� �� ��������� ����� ����������
		// (����������� �����)
		GetQueuedCompletionStatus((HANDLE)hCompPort,
			&numBytes,
			(LPDWORD)&di,///PULONG_PTR (x64) // ���� ��������� �� �������� ����� � CreateIoCompletionPort()
			&lpOverlapped,
			INFINITE);

		if (di)
		{

			Log(_ftprintf(logFile, _T("Notify (cbOffset %u)...\n"), cbOffset));
			fni = (PFILE_NOTIFY_INFORMATION)di->lpBuffer;

			do
			{
				cbOffset = fni->NextEntryOffset;

				lstrcpyn(FileName, fni->FileName, fni->FileNameLength / sizeof(TCHAR) + 1);
				FileName[fni->FileNameLength / sizeof(TCHAR) + 1] = '\0';
				_stprintf(szSourceName, _T("%s\\%s"), m_szSrcPath, FileName);
				_stprintf(szTargetName, _T("%s\\%s"), m_szDstPath, FileName);

				HANDLE hFind = FindFirstFile(szSourceName, &ffd);
				if (INVALID_HANDLE_VALUE == hFind && fni->Action != FILE_ACTION_RENAMED_OLD_NAME && fni->Action != FILE_ACTION_REMOVED)
				{
					Log(_ftprintf(logFile, _T("(FindFirstFile failed:%08X)"), ffd.dwFileAttributes));
				}
				Log(_ftprintf(logFile, _T("[%u.%u]: "), cbOffset, fni->Action));

				BOOL retc = FALSE;
				switch (fni->Action)
				{
				case FILE_ACTION_ADDED:
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						Log(_ftprintf(logFile, _T("folder added: %s\n"), szSourceName));
						if (!CreateDirectory(szTargetName, NULL))
						{
							Log(_ftprintf(logFile, _T("CreateDirectory(%s) failed with error %u\n"), szTargetName, GetLastError()));
							WriteEventLogEntry(_T("Main: FolderAdded: CreateDirectory failed"), EVENTLOG_ERROR_TYPE);
						}
					}
					else
					{
						Log(_ftprintf(logFile, _T("file added: %s\n"), szSourceName));
					}
					break;
				case FILE_ACTION_REMOVED: // �� ��������� ���������� ������ �� �������
#if 0 
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						Log(_ftprintf(logFile, _T("folder deleted: %s\n"), szSourceName));
					}
					else
					{
						Log(_ftprintf(logFile, _T("file deleted: %s\n"), szSourceName));
						if (!DeleteFile(szTargetName))
						{
							Log(_ftprintf(logFile, _T("DeleteFile(%s) failed with error %u\n"), szTargetName, GetLastError()));
						}
					}
#endif
					break;
				case FILE_ACTION_MODIFIED:
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						Log(_ftprintf(logFile, _T("folder modified: %s\n"), szSourceName));
					}
					else
					{
						Log(_ftprintf(logFile, _T("file modified: %s\n"), szSourceName));
#if 1
						// ������� �������� ����
						/*FILE* pInfile = _tfopen(szSource, _T("rb"));
						if (!pInfile) {
							WriteEventLogEntry(_T("Main: ZipDir: Failed opening input file"), EVENTLOG_ERROR_TYPE);
							break;
						}
						// ��������� ������ �������� �����
						fseek(pInfile, 0, SEEK_END);
						long file_loc = ftell(pInfile);
						fseek(pInfile, 0, SEEK_SET);

						if ((file_loc < 0) || (file_loc > INT_MAX)) {
							WriteEventLogEntry(_T("Main: ZipDir: File is too large to be processed by this example"), EVENTLOG_ERROR_TYPE);
							fclose(pInfile); pInfile = nullptr;
							break;
						}
						// ������������ ������� ��� �������� ����� ������ ������
						if (szTargetDir && _tcslen(szTargetDir))
							_stprintf(szTarget, _T("%s/%s"), szTargetDir, lpszTestFile);
						else
							_tcscpy(szTarget, lpszTestFile);*/
#else
						if (!CopyFile(szSourceName, szTargetName, FALSE))
						{
							Log(_ftprintf(logFile, _T("\nCopyFile(%s, %s) failed with error %u\n"), szSourceName, szTargetName, GetLastError()));
							WriteEventLogEntry(_T("Main: FileModofied: CopyFile failed"), EVENTLOG_ERROR_TYPE);
						}
#endif
///						fclose(pInfile); pInfile = nullptr;
					}
					break;
				case FILE_ACTION_RENAMED_OLD_NAME:
					_tcscpy(szOldName, szTargetName);
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						Log(_ftprintf(logFile, _T("folder name changed - old name: %s\n"), szSourceName));
					}
					else
					{
						Log(_ftprintf(logFile, _T("file name changed - old name: %s\n"), szSourceName));
					}
					break;
				case FILE_ACTION_RENAMED_NEW_NAME:
					if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						Log(_ftprintf(logFile, _T("folder name changed - new name: %s\n"), szSourceName));
					}
					else
					{
						Log(_ftprintf(logFile, _T("file name changed - new name: %s\n"), szSourceName));
					}
					if (!MoveFile(szOldName, szTargetName))
					{
						Log(_ftprintf(logFile, _T("MoveFile(%s, %s) failed with error %u\n"), szOldName, szSourceName, GetLastError()));
						WriteEventLogEntry(_T("Main: NewName: MoveFile failed"), EVENTLOG_ERROR_TYPE);
					}
					break;
				default:
					Log(_ftprintf(logFile, _T("unknown event: %s\n"), szSourceName));
					break;
				}
#ifdef LOCAL_FILE_LOGGER
				fflush(logFile);
#endif

				FindClose(hFind);

				fni = (PFILE_NOTIFY_INFORMATION)((LPBYTE)fni + cbOffset);

			} while (cbOffset);

			// ���������� ������� ��������
			ReadDirectoryChangesW(di->hDir, di->lpBuffer,
				MAX_BUFFER,
				TRUE,
				dwNotifyFilter,
				&di->dwBufLength,
				&di->Overlapped,
				NULL);
		}
	
	} while (di); // ����� ������ PostQueuedCompletionStatus() � OnStop() ���� ��������� ����� �������

	// ��������� �� ���������
	SetEvent(m_hStoppedEvent);
	Log(_ftprintf(logFile, _T("CServiceBase::ServiceWorkerThread leave\n")));
}

//
// ����������, ����� SCM �������� ���������� ������� Stop
void CMainService::OnStop()
{
	// ������ � ������ ����������
	WriteEventLogEntry(_T("BackupService: OnStop"), EVENTLOG_INFORMATION_TYPE);
	Log(_ftprintf(logFile, _T("Shutdown the main thread\n")));
	// �������������� ����� GetQueuedCompletionStatus() � �������� ������
	PostQueuedCompletionStatus(hCompPort, 0, 0, NULL);
	// ��������� ��� ����������
	if (WaitForSingleObject(m_hStoppedEvent, INFINITE) != WAIT_OBJECT_0)
	{
		throw GetLastError();
	}
	// ���������� ���������� �������
	CloseHandle(DirInfo.hDir);
	CloseHandle(hCompPort);
	Log(_ftprintf(logFile, _T("Done\n")));
}

// �������������� ����������� ����������� ���������� szSourceDir � szTargetDir
void CMainService::BackupDir(LPCTSTR szSourceDir, LPCTSTR szTargetDir, LPCTSTR szSourceMask)
{
	//Log(_ftprintf(logFile, _T("BackupDir \"%s\" (%s) \"%s\"\n"), szSourceDir, szSourceMask, szTargetDir));
	TCHAR szSource[MAX_PATH];
	TCHAR szTarget[MAX_PATH];
	WIN32_FIND_DATA ffd;
	//LARGE_INTEGER filesize;

	CreateDirectory(szTargetDir, NULL);

	TCHAR szMask[MAX_PATH];
	_stprintf(szMask, _T("%s\\%s"), szSourceDir, szSourceMask);
	HANDLE hFind = FindFirstFile(szMask, &ffd);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		WriteEventLogEntry(_T("Main: BackupDir: FindFirstFile failed"), EVENTLOG_ERROR_TYPE);
		return;
	}
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (_tcscmp(_T("."), ffd.cFileName) && _tcscmp(_T(".."), ffd.cFileName))
			{
				_stprintf(szTarget, _T("%s\\%s"), szTargetDir, ffd.cFileName);
				_stprintf(szSource, _T("%s\\"), szSourceDir);
				_tcscat(szSource, ffd.cFileName);
				BackupDir(szSource, szTarget, szSourceMask);
			}
		}
		else
		{
			//filesize.LowPart = ffd.nFileSizeLow;
			//filesize.HighPart = ffd.nFileSizeHigh;
			_stprintf(szSource, _T("%s\\%s"), szSourceDir, ffd.cFileName);
			_stprintf(szTarget, _T("%s\\%s"), szTargetDir, ffd.cFileName);
			CopyFile(szSource, szTarget, FALSE);
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);
}

// ������������� ����������� ���������� szSourceDir � szTargetDir
void CMainService::ZipDir(char* zipName, LPCTSTR szSourceDir, LPCTSTR szTargetDir, LPCTSTR szSourceMask)
{
	//Log(_ftprintf(logFile, _T("BackupDir \"%s\" (%s) \"%s\"\n"), szSourceDir, szSourceMask, szTargetDir));
	TCHAR szSource[MAX_PATH];
	TCHAR szTarget[MAX_PATH];
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;

	mz_bool status;

	//CreateDirectory(szTargetDir, NULL);

	TCHAR szMask[MAX_PATH];
	_stprintf(szMask, _T("%s/%s"), szSourceDir, szSourceMask);
	HANDLE hFind = FindFirstFile(szMask, &ffd);
	if (INVALID_HANDLE_VALUE == hFind) {
		WriteEventLogEntry(_T("Main: BackupDir: FindFirstFile failed"), EVENTLOG_ERROR_TYPE);
		return;
	}
	do {
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (_tcscmp(_T("."), ffd.cFileName) && _tcscmp(_T(".."), ffd.cFileName)) {
				if (szTargetDir && _tcslen(szTargetDir))
					_stprintf(szTarget, _T("%s/%s"), szTargetDir, ffd.cFileName);
				else
					_stprintf(szTarget, _T("%s/"), ffd.cFileName);
				_stprintf(szSource, _T("%s/"), szSourceDir);
				_tcscat(szSource, ffd.cFileName);

				// Add a directory entry
				WideCharToMultiByte(CP_ACP, 0, szTarget, -1, szInnerArchiveFile, _tcslen(szTarget) + 1, NULL, NULL);
				if (!status) {
					WriteEventLogEntry(_T("Main: ZipDir: mz_zip_add_mem_to_archive_file_in_place(dir) failed"), EVENTLOG_ERROR_TYPE);
				}
				ZipDir(zipName, szSource, szTarget, szSourceMask);
			}
		}
		else {
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			_stprintf(szSource, _T("%s/%s"), szSourceDir, ffd.cFileName);
			// ������� �������� ����
			FILE* pInfile = _tfopen(szSource, _T("rb"));
			if (!pInfile) {
				WriteEventLogEntry(_T("Main: ZipDir: Failed opening input file"), EVENTLOG_ERROR_TYPE);
				continue;
			}
			// ������������ ������� ��� �������� ����� ������ ������
			if (szTargetDir && _tcslen(szTargetDir))
				_stprintf(szTarget, _T("%s/%s"), szTargetDir, ffd.cFileName);
			else
				_tcscpy(szTarget, ffd.cFileName);
			// ��������� ���� � �����
			data_size = static_cast<size_t>(filesize.QuadPart);
			if (buff_size < data_size) {
				buff_size = data_size;
				if (file_buff)
					free(file_buff);
				file_buff = (char*)malloc(buff_size);
			}
			if (file_buff) {
				WideCharToMultiByte(CP_ACP, 0, szTarget, -1, szInnerArchiveFile, _tcslen(szTarget) + 1, NULL, NULL);
				size_t n = fread(file_buff, 1, data_size, pInfile);
				fclose(pInfile); pInfile = nullptr;
				status = mz_zip_add_mem_to_archive_file_in_place(zipName, szInnerArchiveFile, file_buff, data_size, s_pComment, (uint16_t)strlen(s_pComment), MZ_BEST_COMPRESSION);
				if (!status) {
					WriteEventLogEntry(_T("Main: ZipDir: mz_zip_add_mem_to_archive_file_in_place(file) failed"), EVENTLOG_ERROR_TYPE);
				}
			}
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);
}
