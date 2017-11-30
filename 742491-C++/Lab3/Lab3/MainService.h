#pragma once

#include <fstream>

#include "ServiceBase.h"

#define MAX_BUFFER  4096
//#define LOCAL_FILE_LOGGER

// структура, содержащая все данные по наблюдаемому каталогу
struct DIRECTORY_INFO {
	HANDLE      hDir;
	TCHAR       lpszDirName[MAX_PATH];
	CHAR        lpBuffer[MAX_BUFFER];
	DWORD       dwBufLength;
	OVERLAPPED  Overlapped;
};

class CMainService : public CServiceBase
{
public:

	CMainService(LPCTSTR pszServiceName,
		BOOL fCanStop = TRUE,
		BOOL fCanShutdown = TRUE,
		BOOL fCanPauseContinue = FALSE);
	virtual ~CMainService(void);

protected:

	virtual void OnStart(DWORD dwArgc, LPTSTR *pszArgv);
	virtual void OnStop();

	void ServiceWorkerThread(void);

	void BackupDir(LPCTSTR szSourceDir, LPCTSTR szTargetDir, LPCTSTR szSourceMask);
	void ZipDir(LPCTSTR szSourceDir, LPCTSTR szTargetDir, LPCTSTR szSourceMask);
	void NormalizePath(PCHAR pszPath, size_t length = 0);
	PCHAR GetFileBuffer(size_t size);
	BOOL BackupFile(LPCTSTR lpszSourceName, LPCTSTR lpszTargetName);
private:

	struct Pattern {
		TCHAR mask[MAX_PATH];
	};

	HANDLE m_hStoppedEvent;
#ifdef LOCAL_FILE_LOGGER
	FILE* logFile = nullptr;
#endif
	// имя ini-файла
	TCHAR szIniFileName[MAX_PATH];
	// имя резервируемой (исходной) директории
	TCHAR m_szSrcPath[MAX_PATH];
	// имя директории, в которую выполняется резервирование
	TCHAR m_szDstPath[MAX_PATH];
	// имя файла архива
	CHAR m_szZipFile[MAX_PATH];

	DIRECTORY_INFO DirInfo; // параметры исходной директории
	HANDLE  hCompPort = NULL; // дескриптор "порта завершения"
	DWORD dwNotifyFilter = // список модификаций исходной директории
		FILE_NOTIFY_CHANGE_FILE_NAME
		| FILE_NOTIFY_CHANGE_DIR_NAME
		| FILE_NOTIFY_CHANGE_SIZE
		| FILE_NOTIFY_CHANGE_LAST_WRITE
		| FILE_NOTIFY_CHANGE_CREATION
		;

	// изменение файла сигнализируется дважды, обрабатываем однократно
	bool processModified = false;
	// архиватор
	static constexpr PCHAR s_pComment = "Empty comment";
	// память для содержимого файлов
	size_t buff_size = 1024 * 1024;
	size_t data_size = 1024 * 1024;
	PCHAR file_buff = nullptr;
	// разгрузка стека (вместо локальных переменных - члены класса)
	char szMainArchiveFullFileName[MAX_PATH] = "";
	char szInnerArchiveFile[MAX_PATH];
};
