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
	// определим местонахождение исполняемого файла и 
	// установим имя ini-файла, а также значения по умолчанию 
	// для m_szSrcPath и m_szDstPath
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
	if (file_buff)
	{
		free(file_buff);
	}
}

void CMainService::OnStart(DWORD dwArgc, LPTSTR *lpszArgv)
{
	// Запись в Журнал приложений
	WriteEventLogEntry(_T("BackupService: OnStart"), EVENTLOG_INFORMATION_TYPE);

	GetPrivateProfileString(_T("common"), _T("SourceDir"), _T("failed"), m_szSrcPath, MAX_PATH, szIniFileName);
	GetPrivateProfileString(_T("common"), _T("TargetDir"), _T("failed"), m_szDstPath, MAX_PATH, szIniFileName);
	Log(_ftprintf(logFile, _T("m_szSrcPath = %s\n"), m_szSrcPath));
	Log(_ftprintf(logFile, _T("m_szDstPath = %s\n"), m_szDstPath));

	// При запуске сохраняем всё содержимое директории
	if (!CreateDirectory(m_szSrcPath, NULL)) // на случай, если такая не существует
	{
		//BackupDir(m_szSrcPath, m_szDstPath, _T("*"));
		ZipDir(m_szSrcPath, m_szDstPath, _T("*"));
	}
	
	Test(m_szDstPath);

	memset(&DirInfo, 0x00, sizeof(DIRECTORY_INFO));

	// Открыть исходную директорию (получить дескриптор)
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
		WriteEventLogEntry(_T("OnStart: ReadDirectoryChangesW failed"), EVENTLOG_ERROR_TYPE);
	}

	// Запуск основного рабочего потока
	CThreadPool::QueueUserWorkItem(&CMainService::ServiceWorkerThread, this);
}

// Основной рабочий поток службы
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
		// Получить из очереди очередное соощение об изменении нашей директории
		// (блокирующий вызов)
		GetQueuedCompletionStatus((HANDLE)hCompPort,
			&numBytes,
			(LPDWORD)&di,///PULONG_PTR (x64) // этот указатель мы передали ранее в CreateIoCompletionPort()
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
				case FILE_ACTION_REMOVED: // из резервной директории ничего не удаляем
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
						if (!CopyFile(szSourceName, szTargetName, FALSE))
						{
							Log(_ftprintf(logFile, _T("\nCopyFile(%s, %s) failed with error %u\n"), szSourceName, szTargetName, GetLastError()));
							WriteEventLogEntry(_T("Main: FileModofied: CopyFile failed"), EVENTLOG_ERROR_TYPE);
						}
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
	Log(_ftprintf(logFile, _T("CServiceBase::ServiceWorkerThread leave\n")));
}

//
// Вызывается, когда SCM посылает приложению команду Stop
void CMainService::OnStop()
{
	// Запись в Журнал приложений
	WriteEventLogEntry(_T("BackupService: OnStop"), EVENTLOG_INFORMATION_TYPE);
	Log(_ftprintf(logFile, _T("Shutdown the main thread\n")));
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
	Log(_ftprintf(logFile, _T("Done\n")));
}

// рекурсированое копирование содержимого директории szSourceDir в szTargetDir
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

static const char *s_pComment = "Empty comment";
static const char *s_Test_archive_filename = "backup.zip";

// The string to compress.
static const char *s_pTest_str =
"MISSION CONTROL I wouldn't worry too much about the computer. First of all, there is still a chance that he is right, despite your tests, and" \
"if it should happen again, we suggest eliminating this possibility by allowing the unit to remain in place and seeing whether or not it" \
"actually fails. If the computer should turn out to be wrong, the situation is still not alarming. The type of obsessional error he may be" \
"guilty of is not unknown among the latest generation of HAL 9000 computers. It has almost always revolved around a single detail, such as" \
"the one you have described, and it has never interfered with the integrity or reliability of the computer's performance in other areas." \
"No one is certain of the cause of this kind of malfunctioning. It may be over-programming, but it could also be any number of reasons. In any" \
"event, it is somewhat analogous to human neurotic behavior. Does this answer your query?  Zero-five-three-Zero, MC, transmission concluded.";

void CMainService::ZipDir(LPCTSTR szSourceDir, LPCTSTR szTargetDir, LPCTSTR szSourceMask)
{
	char archive_filename[64];
	char main_archive_filename[MAX_PATH] = "";

	if (szTargetDir) {
		CreateDirectory(szTargetDir, NULL);
		int path_length = wcslen(szTargetDir);
		if (path_length) {
			WideCharToMultiByte(CP_ACP, 0, szTargetDir, -1, main_archive_filename, path_length + 1, NULL, NULL);
			if (main_archive_filename[path_length - 1] != '\\') {
				strcat(main_archive_filename, "\\");
			}
		}
	}
	strcat(main_archive_filename, s_Test_archive_filename);

	remove(main_archive_filename);

	ZipDir(main_archive_filename, szSourceDir, _T(""), szSourceMask);
}

// архивирование содержимого директории szSourceDir в szTargetDir
void CMainService::ZipDir(char* zipName, LPCTSTR szSourceDir, LPCTSTR szTargetDir, LPCTSTR szSourceMask)
{
	//Log(_ftprintf(logFile, _T("BackupDir \"%s\" (%s) \"%s\"\n"), szSourceDir, szSourceMask, szTargetDir));
	TCHAR szSource[MAX_PATH];
	TCHAR szTarget[MAX_PATH];
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;

	mz_bool status;
	size_t uncomp_size;
	mz_zip_archive zip_archive;
	//void *p;

	//CreateDirectory(szTargetDir, NULL);

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
				/* Add a directory entry for testing
				status = mz_zip_add_mem_to_archive_file_in_place(main_archive_filename, "directory/", NULL, 0, "no comment", (uint16_t)strlen("no comment"), MZ_BEST_COMPRESSION);
				if (!status)
				{
				printf("mz_zip_add_mem_to_archive_file_in_place failed!\n");
				return EXIT_FAILURE;
				}*/
				_stprintf(szTarget, _T("%s\\%s"), szTargetDir, ffd.cFileName);
				_stprintf(szSource, _T("%s\\"), szSourceDir);
				_tcscat(szSource, ffd.cFileName);
				ZipDir(zipName, szSource, szTarget, szSourceMask);
			}
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			_stprintf(szSource, _T("%s\\%s"), szSourceDir, ffd.cFileName);
			_stprintf(szTarget, _T("%s\\%s"), szTargetDir, ffd.cFileName);
#if 1
			// прочитать файл в буфер
			data_size = static_cast<size_t>(filesize.QuadPart);
			if (buff_size < data_size) {
				buff_size = data_size;
				if (file_buff)
					free(file_buff);
				file_buff = (char*)malloc(buff_size);
			}
			if (file_buff) {
				WideCharToMultiByte(CP_ACP, 0, szTarget, -1, archive_filename, _tcslen(szTarget) + 1, NULL, NULL);
				//sprintf(archive_filename, "directory/%u.txt", i);
				strcpy(file_buff, s_pTest_str);
				status = mz_zip_add_mem_to_archive_file_in_place(zipName, archive_filename, file_buff, strlen(file_buff) + 1, s_pComment, (uint16_t)strlen(s_pComment), MZ_BEST_COMPRESSION);
				if (!status)
				{
					printf("mz_zip_add_mem_to_archive_file_in_place failed!\n");
					return;
				}
			}
#else
			CopyFile(szSource, szTarget, FALSE);
#endif
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);

}

int CMainService::Test(LPCTSTR lpszPath)
{
	int i, sort_iter;
	mz_bool status;
	size_t uncomp_size;
	mz_zip_archive zip_archive;
	void *p;
	const int N = 2;// 50;
	char data[2048];
	char archive_filename[64];
	char main_archive_filename[MAX_PATH] = "";

	if (lpszPath) {
		int path_length = wcslen(lpszPath);
		if (path_length) {
			WideCharToMultiByte(CP_ACP, 0, lpszPath, -1, main_archive_filename, path_length + 1, NULL, NULL);
			if (main_archive_filename[path_length - 1] != '\\') {
				strcat(main_archive_filename, "\\");
			}
		}
	}
	strcat(main_archive_filename, s_Test_archive_filename);

	assert((strlen(s_pTest_str) + 64) < sizeof(data));

	printf("miniz.c version: %s\n", MZ_VERSION);

	// Delete the test archive, so it doesn't keep growing as we run this test
	remove(main_archive_filename);

	// Append a bunch of text files to the test archive
	for (i = (N - 1); i >= 0; --i)
	{
		sprintf(archive_filename, "%u.txt", i);
		sprintf(data, "%u %s %u", (N - 1) - i, s_pTest_str, i);

		// Add a new file to the archive. Note this is an IN-PLACE operation, so if it fails your archive is probably hosed (its central directory may not be complete) but it should be recoverable using zip -F or -FF. So use caution with this guy.
		// A more robust way to add a file to an archive would be to read it into memory, perform the operation, then write a new archive out to a temp file and then delete/rename the files.
		// Or, write a new archive to disk to a temp file, then delete/rename the files. For this test this API is fine.
		status = mz_zip_add_mem_to_archive_file_in_place(main_archive_filename, archive_filename, data, strlen(data) + 1, s_pComment, (uint16_t)strlen(s_pComment), MZ_BEST_COMPRESSION);
		if (!status)
		{
			printf("mz_zip_add_mem_to_archive_file_in_place failed!\n");
			return EXIT_FAILURE;
		}
	}

	// Add a directory entry for testing
	status = mz_zip_add_mem_to_archive_file_in_place(main_archive_filename, "directory/", NULL, 0, "no comment", (uint16_t)strlen("no comment"), MZ_BEST_COMPRESSION);
	if (!status)
	{
		printf("mz_zip_add_mem_to_archive_file_in_place failed!\n");
		return EXIT_FAILURE;
	}

	/// Append a bunch of text files to the test archive
	for (i = (N - 1); i >= 0; --i)
	{
		sprintf(archive_filename, "directory/%u.txt", i);
		sprintf(data, "%u %s %u", (N - 1) - i, s_pTest_str, i);

		// Add a new file to the archive. Note this is an IN-PLACE operation, so if it fails your archive is probably hosed (its central directory may not be complete) but it should be recoverable using zip -F or -FF. So use caution with this guy.
		// A more robust way to add a file to an archive would be to read it into memory, perform the operation, then write a new archive out to a temp file and then delete/rename the files.
		// Or, write a new archive to disk to a temp file, then delete/rename the files. For this test this API is fine.
		status = mz_zip_add_mem_to_archive_file_in_place(main_archive_filename, archive_filename, data, strlen(data) + 1, s_pComment, (uint16_t)strlen(s_pComment), MZ_BEST_COMPRESSION);
		if (!status)
		{
			printf("mz_zip_add_mem_to_archive_file_in_place failed!\n");
			return EXIT_FAILURE;
		}
	}

	// Now try to open the archive.
	memset(&zip_archive, 0, sizeof(zip_archive));

	status = mz_zip_reader_init_file(&zip_archive, main_archive_filename, 0);
	if (!status)
	{
		printf("mz_zip_reader_init_file() failed!\n");
		return EXIT_FAILURE;
	}

	// Get and print information about each file in the archive.
	for (i = 0; i < (int)mz_zip_reader_get_num_files(&zip_archive); i++)
	{
		mz_zip_archive_file_stat file_stat;
		if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat))
		{
			printf("mz_zip_reader_file_stat() failed!\n");
			mz_zip_reader_end(&zip_archive);
			return EXIT_FAILURE;
		}

		printf("Filename: \"%s\", Comment: \"%s\", Uncompressed size: %u, Compressed size: %u, Is Dir: %u\n", file_stat.m_filename, file_stat.m_comment, (uint32_t)file_stat.m_uncomp_size, (uint32_t)file_stat.m_comp_size, mz_zip_reader_is_file_a_directory(&zip_archive, i));

		if (!strcmp(file_stat.m_filename, "directory/"))
		{
			if (!mz_zip_reader_is_file_a_directory(&zip_archive, i))
			{
				printf("mz_zip_reader_is_file_a_directory() didn't return the expected results!\n");
				mz_zip_reader_end(&zip_archive);
				return EXIT_FAILURE;
			}
		}
	}

	// Close the archive, freeing any resources it was using
	mz_zip_reader_end(&zip_archive);

	// Now verify the compressed data
	for (sort_iter = 0; sort_iter < 2; sort_iter++)
	{
		memset(&zip_archive, 0, sizeof(zip_archive));
		status = mz_zip_reader_init_file(&zip_archive, main_archive_filename, sort_iter ? MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY : 0);
		if (!status)
		{
			printf("mz_zip_reader_init_file() failed!\n");
			return EXIT_FAILURE;
		}

		for (i = 0; i < N; i++)
		{
			sprintf(archive_filename, "%u.txt", i);
			sprintf(data, "%u %s %u", (N - 1) - i, s_pTest_str, i);

			// Try to extract all the files to the heap.
			p = mz_zip_reader_extract_file_to_heap(&zip_archive, archive_filename, &uncomp_size, 0);
			if (!p)
			{
				printf("mz_zip_reader_extract_file_to_heap() failed!\n");
				mz_zip_reader_end(&zip_archive);
				return EXIT_FAILURE;
			}

			// Make sure the extraction really succeeded.
			if ((uncomp_size != (strlen(data) + 1)) || (memcmp(p, data, strlen(data))))
			{
				printf("mz_zip_reader_extract_file_to_heap() failed to extract the proper data\n");
				mz_free(p);
				mz_zip_reader_end(&zip_archive);
				return EXIT_FAILURE;
			}

			printf("Successfully extracted file \"%s\", size %u\n", archive_filename, (uint32_t)uncomp_size);
			printf("File data: \"%s\"\n", (const char *)p);

			// We're done.
			mz_free(p);
		}

		// Close the archive, freeing any resources it was using
		mz_zip_reader_end(&zip_archive);
	}

	printf("Success.\n");
	return EXIT_SUCCESS;
}
