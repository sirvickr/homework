#pragma once

#include <fstream>

#include "ServiceBase.h"

#define MAX_BUFFER  4096
//#define LOCAL_FILE_LOGGER

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

	CMainService(PWSTR pszServiceName,
		BOOL fCanStop = TRUE,
		BOOL fCanShutdown = TRUE,
		BOOL fCanPauseContinue = FALSE);
	virtual ~CMainService(void);

protected:

	virtual void OnStart(DWORD dwArgc, PWSTR *pszArgv);
	virtual void OnStop();

	void ServiceWorkerThread(void);

	void BackupDir(PWSTR szSourceDir, PWSTR szTargetDir, PWSTR szSourceMask);

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

	DIRECTORY_INFO DirInfo; // ��������� �������� ����������
	HANDLE  hCompPort = NULL; // ���������� "����� ����������"
	DWORD dwNotifyFilter = // ������ ����������� �������� ����������
		FILE_NOTIFY_CHANGE_FILE_NAME
		| FILE_NOTIFY_CHANGE_DIR_NAME
		| FILE_NOTIFY_CHANGE_SIZE
		| FILE_NOTIFY_CHANGE_LAST_WRITE
		| FILE_NOTIFY_CHANGE_CREATION
		;
};
