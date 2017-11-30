#pragma once

#include <fstream>

#include "ServiceBase.h"

#define MAX_BUFFER  4096
#define LOCAL_FILE_LOGGER

// ���������, ���������� ��� ������ �� ������������ ��������
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
	void ZipDir(char* zipName, LPCTSTR szSourceDir, LPCTSTR szTargetDir, LPCTSTR szSourceMask);

private:

	struct Pattern {
		TCHAR mask[MAX_PATH];
	};

	HANDLE m_hStoppedEvent;
#ifdef LOCAL_FILE_LOGGER
	FILE* logFile = nullptr;
#endif
	// ��� ini-�����
	TCHAR szIniFileName[MAX_PATH];
	// ��� ������������� (��������) ����������
	TCHAR m_szSrcPath[MAX_PATH];
	// ��� ����������, � ������� ����������� ��������������
	TCHAR m_szDstPath[MAX_PATH];
	// ��� ����� ������
	CHAR m_szZipFile[MAX_PATH];

	DIRECTORY_INFO DirInfo; // ��������� �������� ����������
	HANDLE  hCompPort = NULL; // ���������� "����� ����������"
	DWORD dwNotifyFilter = // ������ ����������� �������� ����������
		FILE_NOTIFY_CHANGE_FILE_NAME
		| FILE_NOTIFY_CHANGE_DIR_NAME
		| FILE_NOTIFY_CHANGE_SIZE
		| FILE_NOTIFY_CHANGE_LAST_WRITE
		| FILE_NOTIFY_CHANGE_CREATION
		;

	// ���������
	static constexpr PCHAR s_pComment = "Empty comment";
	// ������ ��� ����������� ������
	size_t buff_size = 1024 * 1024;
	size_t data_size = 1024 * 1024;
	char* file_buff = nullptr;
	// ��������� ����� (������ ��������� ���������� - ����� ������)
	char szMainArchiveFullFileName[MAX_PATH] = "";
	char szInnerArchiveFile[MAX_PATH];
};
