#pragma once

#include <fstream>
#include <string>
#include <list>

#include "ServiceBase.h"

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

private:

	struct Pattern {
		TCHAR mask[MAX_PATH];
	};

	BOOL m_fStopping;
	HANDLE m_hStoppedEvent;
	std::ofstream log;
	FILE* logFile = nullptr;
	// список резервируемых файлов
	//std::list<std::string> m_sSrcPatterns;
	std::list<Pattern> m_sSrcPatterns;
	// имя директории, из которой выполняется копирование
	//std::string m_sSrcPath;
	wchar_t m_szSrcPath[MAX_PATH];
	// имя директории, в которую выполняется резервирование
	//std::string m_sDstPath;
	wchar_t m_szDstPath[MAX_PATH];
	// имя архива
	//std::string m_sDstFile;
	wchar_t m_szDstFile[MAX_PATH];
};
