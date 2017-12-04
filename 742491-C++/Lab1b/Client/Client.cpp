
// Client.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientApp

BEGIN_MESSAGE_MAP(CClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CClientApp construction

CClientApp::CClientApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CClientApp object

CClientApp theApp;


// CClientApp initialization

BOOL CClientApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CClientDlg dlg;
	m_pMainWnd = &dlg;
#if 0
	// определим местонахождение исполняемого файла и 
	// установим имя ini-файла, а также значения по умолчанию 
	// для m_szSrcPath и m_szDstPath
	const LPTSTR IniFile = _T("config.ini");
	TCHAR szIniFileName[MAX_PATH];
	TCHAR szHostName[MAX_PATH] = _T("");
	TCHAR szPortNumber[MAX_PATH] = _T("");
	szIniFileName[0] = _T('\0');
	DWORD dwLen = GetModuleFileName(NULL, szIniFileName, MAX_PATH);
	if (dwLen) {
		for (int i = int(dwLen - 1); i >= 0; i--) {
			if (szIniFileName[i] == _T('\\')) {
				szIniFileName[i + 1] = _T('\0');
				wcscat_s<MAX_PATH>(szIniFileName, IniFile);

				GetPrivateProfileString(_T("common"), _T("host"), _T("localhost"), szHostName, MAX_PATH, szIniFileName);
				GetPrivateProfileString(_T("common"), _T("port"), _T("5001"), szPortNumber, MAX_PATH, szIniFileName);

				break;
			}
		}
	}

	dlg.SetHost(szHostName);
	dlg.SetPort(szPortNumber);
	dlg.SetObject(CString("C:\\Windows\\explorer.exe"));
	dlg.SetDisk(CString("C:"));
	//dlg.SetUser(CString("UserName"));
#endif
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

