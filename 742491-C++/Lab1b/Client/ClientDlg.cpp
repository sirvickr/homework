
// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "TcpClient.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

char* CmdLineToArgv(char * CmdLine, int & argc, char **& argv);

// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CClientDlg::IDD, pParent)
#if 0
, client(NULL)
#endif
, m_bAccRead(false), m_bAccWrite(false), m_bAccExec(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
#if 0
	client = new CTcpClient();
#endif
}

CClientDlg::~CClientDlg()
{
#if 0
	if(client) {
		delete client;
	}
#endif
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HOST, m_csHost);
	DDX_Text(pDX, IDC_EDIT_PORT, m_csPort);
	DDX_Text(pDX, IDC_EDIT_OBJECT, m_csObject);
	DDX_Text(pDX, IDC_EDIT_DISK, m_csDisk);
	DDX_Text(pDX, IDC_EDIT_OWNER, m_csUser);
	DDX_Check(pDX, IDC_CHECK_READ, m_bAccRead);
	DDX_Check(pDX, IDC_CHECK_WRITE, m_bAccWrite);
	DDX_Check(pDX, IDC_CHECK_EXEC, m_bAccExec);
	DDX_Control(pDX, IDC_LIST_HOSTS, m_lstHosts);
}

void CClientDlg::SetHost(const CString &value)
{
	m_csHost = value;
}

CString CClientDlg::GetHost()
{
	return m_csHost;
}

void CClientDlg::SetPort(const CString &value)
{
	m_csPort = value;
}

CString CClientDlg::GetPort()
{
	return m_csPort;
}

void CClientDlg::SetObject(const CString &value)
{
	m_csObject = value;
}

CString CClientDlg::GetObject()
{
	return m_csObject;
}

void CClientDlg::SetDisk(const CString &value)
{
	m_csDisk = value;
}

CString CClientDlg::GetDisk()
{
	return m_csDisk;
}

void CClientDlg::SetUser(const CString &value)
{
	m_csUser = value;
}

CString CClientDlg::GetUser()
{
	return m_csUser;
}

void CClientDlg::SetReplyText(const CString &value)
{
	m_csReplyText = value;
}

CString CClientDlg::GetReplyText()
{
	return m_csReplyText;
}

void CClientDlg::SetAccRead(BOOL value) 
{
	m_bAccRead = value;
}

BOOL CClientDlg::GetAccRead()
{
	return m_bAccRead;
}

void CClientDlg::SetAccWrite(BOOL value)
{
	m_bAccWrite = value;
}

BOOL CClientDlg::GetAccWrite()
{
	return m_bAccWrite;
}

void CClientDlg::SetAccExec(BOOL value)
{
	m_bAccExec = value;
}

BOOL CClientDlg::GetAccExec()
{
	return m_bAccExec;
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(CmdConnect, &CClientDlg::OnBnClickedCmdconnect)
	ON_BN_CLICKED(IDOK, &CClientDlg::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST_HOSTS, &CClientDlg::OnLbnSelchangeListHosts)
END_MESSAGE_MAP()

// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// ��������� ��������������� ������������ ����� � 
	// ��������� ��� ini-�����, � ����� �������� �� ��������� 
	// ��� m_szSrcPath � m_szDstPath
	TCHAR szHosts[MAX_PATH] = _T("");
	const TCHAR seps[] = _T(" ;");
	
	TCHAR szHostName[MAX_PATH] = _T("");
	TCHAR szPortNumber[10] = _T("");
	TCHAR szObjectPath[MAX_PATH] = _T("");
	TCHAR szDiscLetter[10] = _T("");
	TCHAR szUserName[100] = _T("");
	szIniFileName[0] = _T('\0');
	DWORD dwLen = GetModuleFileName(NULL, szIniFileName, MAX_PATH);
	if (dwLen) {
		for (int i = int(dwLen - 1); i >= 0; i--) {
			if (szIniFileName[i] == _T('\\')) {
				szIniFileName[i + 1] = _T('\0');
				_tcscat_s<MAX_PATH>(szIniFileName, IniFile);

				GetPrivateProfileString(_T("common"), _T("host"), _T("localhost"), szHostName, MAX_PATH, szIniFileName);
				GetPrivateProfileString(_T("common"), _T("hosts"), _T("localhost"), szHosts, MAX_PATH, szIniFileName);
				tcout << _T("Splitting ") << szHosts << _T(" into tokens") << endl;
				TCHAR *next_token = NULL;
				TCHAR *token = _tcstok_s(szHosts, seps, &next_token);
				while (token) {
					tcout << _T(" host: \"") << token << _T("\"") << endl;
					m_lstHosts.AddString(token);
					token = _tcstok_s(NULL, seps, &next_token);
				}
				int index = m_lstHosts.FindStringExact(0, szHostName);
				if (LB_ERR == index) {
					if (m_lstHosts.GetCount() == 0) {
						m_lstHosts.AddString(szHostName);
						index = 0;
					}
				}
				m_lstHosts.SetCurSel(index);

				GetPrivateProfileString(_T("common"), _T("port"), _T("5001"), szPortNumber, 10, szIniFileName);
				GetPrivateProfileString(_T("common"), _T("object"), _T("C:\\Windows\\explorer.exe"), szObjectPath, MAX_PATH, szIniFileName);
				GetPrivateProfileString(_T("common"), _T("disk"), _T("C:"), szDiscLetter, 10, szIniFileName);
				GetPrivateProfileString(_T("common"), _T("user"), _T("UserName"), szUserName, 100, szIniFileName);

				break;
			}
		}
	}

	SetDlgItemText(IDC_EDIT_HOST, szHostName);
	SetDlgItemText(IDC_EDIT_PORT, szPortNumber);
	SetDlgItemText(IDC_EDIT_OBJECT, szObjectPath);
	SetDlgItemText(IDC_EDIT_DISK, szDiscLetter);
	SetDlgItemText(IDC_EDIT_OWNER, szUserName);

	CheckRadioButton(IDC_RADIO1, IDC_RADIO8, IDC_RADIO1);

	InitOrb();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientDlg::OnBnClickedCmdconnect()
{
	UpdateData();
	tostringstream ostr;
	const size_t BuffSize = 512;
#if 1
	Agent::InfoProvider_var infoProvider = GetInfoProvider((LPCTSTR)GetHost(), (LPCTSTR)GetPort());
	if (CORBA::is_nil(infoProvider.in())) {
		MessageBox(_T("�� ������� ������������ � ���������� �������"), _T("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	int reqType = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO8) - IDC_RADIO1;
	switch (reqType) {
	case reqOsVer:
		try {
			Agent::OsVersionInfoEx* result = infoProvider->GetOsVersionInfo();

			ostr << result->dwMajorVersion << "." << result->dwMinorVersion << "." << result->dwBuildNumber;
			SetDlgItemText(IDC_EDIT_OSVER, ostr.str().c_str());

			SetDlgItemInt(IDC_EDIT_PLATFORM, result->dwPlatformId);

			SetDlgItemText(IDC_EDIT_SPSTR, result->szCSDVersion);

			ostr.str(TEXT(""));
			ostr << result->wServicePackMajor << "." << result->wServicePackMinor;
			SetDlgItemText(IDC_EDIT_SPNUM, ostr.str().c_str());

			ostr.str(TEXT(""));
			ostr << hex << uppercase << setfill(TEXT('0')) << setw(8) << result->wSuiteMask << dec;
			SetDlgItemText(IDC_EDIT_SITE, ostr.str().c_str());

			ostr.str(TEXT(""));
			ostr << DWORD(result->wProductType);
			SetDlgItemText(IDC_EDIT_PRODUCT, ostr.str().c_str());
		} catch (...) {
			MessageBox(_T("�� ������� ��������� ������ ��"), _T("������"), MB_OK | MB_ICONERROR);
		}
		break;

	case reqSysTime:
		try {
			Agent::SystemTime sysTime = infoProvider->GetSysTime();
			ostr << setfill(_T('0')) << setw(2) << sysTime.wHour
				<< _T(':') << setw(2) << sysTime.wMinute
				<< _T(':') << setw(2) << sysTime.wSecond;
			SetDlgItemText(IDC_EDIT_SYSTIME, ostr.str().c_str());
		} catch (...) {
			MessageBox(_T("�� ������� ��������� ������� �����"), _T("������"), MB_OK | MB_ICONERROR);
		}
		break;

	case reqTickCount:
		try {
			SetDlgItemInt(IDC_EDIT_TICKS, infoProvider->GetTickCount());
		} catch (...) {
			MessageBox(_T("�� ������� ��������� ���������� ������ � ������� ��"), _T("������"), MB_OK | MB_ICONERROR);
		}
		break;

	case reqMemStatus:
		try {
			Agent::MemoryStatusEx memStatus = infoProvider->GetMemoryStatus();
			ostr.str(TEXT(""));
			ostr << memStatus.dwMemoryLoad;
			SetDlgItemText(IDC_EDIT_MEMLOAD, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << memStatus.ullTotalPhys;
			SetDlgItemText(IDC_EDIT_MEMTOTALPHYS, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << memStatus.ullAvailPhys;
			SetDlgItemText(IDC_EDIT_MEMAVAILPHYS, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << memStatus.ullTotalPageFile;
			SetDlgItemText(IDC_EDIT_MEMTOTALPAGE, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << memStatus.ullAvailPageFile;
			SetDlgItemText(IDC_EDIT_MEMAVAILPAGE, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << memStatus.ullTotalVirtual;
			SetDlgItemText(IDC_EDIT_MEMTOTALVIRT, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << memStatus.ullAvailVirtual;
			SetDlgItemText(IDC_EDIT_MEMAVAILVIRT, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << memStatus.ullAvailExtendedVirtual;
			SetDlgItemText(IDC_EDIT_MEMAVAILVIRTEXT, ostr.str().c_str());
		} catch (...) {
			MessageBox(_T("�� ������� ��������� ���������� ������ � ������� ��"), _T("������"), MB_OK | MB_ICONERROR);
		}
		break;

	case reqDriveType:
		try {
			CORBA::ULong driveType = infoProvider->GetDriveTypeInfo((LPCTSTR)GetDisk());
			ostr << driveType << " (";
			switch (driveType) {
			case DRIVE_UNKNOWN:
				ostr << TEXT("����������� ���");
				break;
			case DRIVE_NO_ROOT_DIR:
				ostr << TEXT("�������� ����");
				break;
			case DRIVE_REMOVABLE:
				ostr << TEXT("������� ����������");
				break;
			case DRIVE_FIXED:
				ostr << TEXT("������� ����");
				break;
			case DRIVE_REMOTE:
				ostr << TEXT("������� ����");
				break;
			case DRIVE_CDROM:
				ostr << TEXT("CD-ROM");
				break;
			case DRIVE_RAMDISK:
				ostr << TEXT("����������� ����");
				break;
			default:
				ostr << TEXT("����������� ���");
				break;
			}
			ostr << ")";
			SetDlgItemText(IDC_EDIT_DRIVE, ostr.str().c_str());
		}
		catch (...) {
			MessageBox(_T("�� ������� ��������� ��� �������"), _T("������"), MB_OK | MB_ICONERROR);
		}

		break;

	case reqFreeSpace:
		try {
			Agent::FreeSpaceReply freeSpace  = infoProvider->GetFreeSpaceInfo((LPCTSTR)GetDisk());
			ostr.str(TEXT(""));
			ostr << freeSpace.totalNumberOfBytes;
			SetDlgItemText(IDC_EDIT_TOTALBYTES, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << freeSpace.totalNumberOfFreeBytes;
			SetDlgItemText(IDC_EDIT_TOTALFREEBYTES, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << freeSpace.freeBytesAvailable;
			SetDlgItemText(IDC_EDIT_TOTALAVAILBYTES, ostr.str().c_str());
		}
		catch (...) {
			MessageBox(_T("�� ������� ��������� ������ � ��������� �������� ������������"), _T("������"), MB_OK | MB_ICONERROR);
		}

		break;

	case reqAccessRights:
		try {
			CORBA::ULong accessMask = infoProvider->GetAccessMask((LPCTSTR)GetObject(), (LPCTSTR)GetUser());
			CheckDlgButton(IDC_CHECK_READ, FALSE);
			CheckDlgButton(IDC_CHECK_WRITE, FALSE);
			CheckDlgButton(IDC_CHECK_EXEC, FALSE);
			if (((accessMask & GENERIC_ALL) == GENERIC_ALL) || ((accessMask & FILE_ALL_ACCESS) == FILE_ALL_ACCESS)) {
				ostr << " ( Full Control )" << endl;
				CheckDlgButton(IDC_CHECK_READ, TRUE);
				CheckDlgButton(IDC_CHECK_WRITE, TRUE);
				CheckDlgButton(IDC_CHECK_EXEC, TRUE);
			}
			else {
				ostr << " (";
				if (((accessMask & GENERIC_READ) == GENERIC_READ)
					|| ((accessMask & FILE_GENERIC_READ) == FILE_GENERIC_READ))
				{
					ostr << " Read";
					CheckDlgButton(IDC_CHECK_READ, TRUE);
				}
				if (((accessMask & GENERIC_WRITE) == GENERIC_WRITE)
					|| ((accessMask & FILE_GENERIC_WRITE) == FILE_GENERIC_WRITE))
				{
					ostr << " Write";
					CheckDlgButton(IDC_CHECK_WRITE, TRUE);
				}
				if (((accessMask & GENERIC_EXECUTE) == GENERIC_EXECUTE)
					|| ((accessMask & FILE_GENERIC_EXECUTE) == FILE_GENERIC_EXECUTE))
				{
					ostr << " Execute";
					CheckDlgButton(IDC_CHECK_EXEC, TRUE);
				}
				ostr << " )" << endl;
			}
		}
		catch (...) {
			MessageBox(_T("�� ������� ��������� ����� �������"), _T("������"), MB_OK | MB_ICONERROR);
		}
		break;

	case reqObjectOwn:
		try {
			SetDlgItemText(IDC_EDIT_OWNER, infoProvider->GetObjectOwnerInfo((LPCTSTR)GetObject()));
		}
		catch (...) {
			MessageBox(_T("�� ������� ��������� ��������� �������"), _T("������"), MB_OK | MB_ICONERROR);
		}
		break;

	}
#else
	if(client) {

		AgentRequest request;
		request.reqType = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO8) - IDC_RADIO1;
		request.headSize = sizeof(request.reqType) + sizeof(request.headSize) + sizeof(request.pathSize) + sizeof(request.userSize);

		OSVERSIONINFOEX osVer;
		SYSTEMTIME sysTime;
		DWORD tickCount;
		MEMORYSTATUSEX memStatus;
		UINT driveType;
		FreeSpaceReply freeSpace;
		ACCESS_MASK accessMask;
		TCHAR szObjectOwner[MAX_PATH];

		int resultCode = -1;

		switch (request.reqType) {
		case reqOsVer:
			resultCode = client->GetOsVer(osVer, (LPCTSTR)GetHost(), (LPCTSTR)GetPort());
			if (resultCode != ERROR_SUCCESS) {
				break;
			}

			ostr << osVer.dwMajorVersion << "." << osVer.dwMinorVersion << "." << osVer.dwBuildNumber;
			SetDlgItemText(IDC_EDIT_OSVER, ostr.str().c_str());

			SetDlgItemInt(IDC_EDIT_PLATFORM, osVer.dwPlatformId);

			SetDlgItemText(IDC_EDIT_SPSTR, osVer.szCSDVersion);

			ostr.str(TEXT(""));
			ostr << osVer.wServicePackMajor << "." << osVer.wServicePackMinor;
			SetDlgItemText(IDC_EDIT_SPNUM, ostr.str().c_str());

			ostr.str(TEXT(""));
			ostr << hex << uppercase << setfill(TEXT('0')) << setw(8) << osVer.wSuiteMask << dec;
			SetDlgItemText(IDC_EDIT_SITE, ostr.str().c_str());

			ostr.str(TEXT(""));
			ostr << DWORD(osVer.wProductType);
			SetDlgItemText(IDC_EDIT_PRODUCT, ostr.str().c_str());

			break;

		case reqSysTime:
			resultCode = client->GetSysTime(sysTime, (LPCTSTR)GetHost(), (LPCTSTR)GetPort());
			if (resultCode != ERROR_SUCCESS) {
				break;
			}

			ostr << setfill(_T('0')) << setw(2) << sysTime.wHour 
				<< _T(':') << setw(2) << sysTime.wMinute 
				<< _T(':') << setw(2) << sysTime.wSecond;
			SetDlgItemText(IDC_EDIT_SYSTIME, ostr.str().c_str());

			break;

		case reqTickCount:
			resultCode = client->GetTickCount(tickCount, (LPCTSTR)GetHost(), (LPCTSTR)GetPort());
			if (resultCode != ERROR_SUCCESS) {
				break;
			}

			SetDlgItemInt(IDC_EDIT_TICKS, tickCount);

			break;

		case reqMemStatus:
			resultCode = client->GetMemStatus(memStatus, (LPCTSTR)GetHost(), (LPCTSTR)GetPort());
			if (resultCode != ERROR_SUCCESS) {
				break;
			}

			ostr.str(TEXT(""));
			ostr << memStatus.dwMemoryLoad;
			SetDlgItemText(IDC_EDIT_MEMLOAD, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << memStatus.ullTotalPhys;
			SetDlgItemText(IDC_EDIT_MEMTOTALPHYS, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << memStatus.ullAvailPhys;
			SetDlgItemText(IDC_EDIT_MEMAVAILPHYS, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << memStatus.ullTotalPageFile;
			SetDlgItemText(IDC_EDIT_MEMTOTALPAGE, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << memStatus.ullAvailPageFile;
			SetDlgItemText(IDC_EDIT_MEMAVAILPAGE, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << memStatus.ullTotalVirtual;
			SetDlgItemText(IDC_EDIT_MEMTOTALVIRT, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << memStatus.ullAvailVirtual;
			SetDlgItemText(IDC_EDIT_MEMAVAILVIRT, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << memStatus.ullAvailExtendedVirtual;
			SetDlgItemText(IDC_EDIT_MEMAVAILVIRTEXT, ostr.str().c_str());

			break;

		case reqDriveType:
			resultCode = client->GetDriveType(driveType, (LPCTSTR)GetHost(), (LPCTSTR)GetPort(), (LPCTSTR)GetDisk());
			if (resultCode != ERROR_SUCCESS) {
				break;
			}

			ostr << driveType << " (";
			switch (driveType) {
			case DRIVE_UNKNOWN:
				ostr << TEXT("����������� ���");
				break;
			case DRIVE_NO_ROOT_DIR:
				ostr << TEXT("�������� ����");
				break;
			case DRIVE_REMOVABLE:
				ostr << TEXT("������� ����������");
				break;
			case DRIVE_FIXED:
				ostr << TEXT("������� ����");
				break;
			case DRIVE_REMOTE:
				ostr << TEXT("������� ����");
				break;
			case DRIVE_CDROM:
				ostr << TEXT("CD-ROM");
				break;
			case DRIVE_RAMDISK:
				ostr << TEXT("����������� ����");
				break;
			default:
				ostr << TEXT("����������� ���");
				break;
			}
			ostr << ")";
			SetDlgItemText(IDC_EDIT_DRIVE, ostr.str().c_str());

			break;

		case reqFreeSpace:
			resultCode = client->GetDiskFreeSpace(freeSpace, (LPCTSTR)GetHost(), (LPCTSTR)GetPort(), (LPCTSTR)GetDisk());
			if (resultCode != ERROR_SUCCESS) {
				break;
			}

			ostr.str(TEXT(""));
			ostr << freeSpace.totalNumberOfBytes.QuadPart;
			SetDlgItemText(IDC_EDIT_TOTALBYTES, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << freeSpace.totalNumberOfFreeBytes.QuadPart;
			SetDlgItemText(IDC_EDIT_TOTALFREEBYTES, ostr.str().c_str());
			ostr.str(TEXT(""));
			ostr << freeSpace.freeBytesAvailable.QuadPart;
			SetDlgItemText(IDC_EDIT_TOTALAVAILBYTES, ostr.str().c_str());

			break;

		case reqAccessRights:
			resultCode = client->GetAccessRights(accessMask, (LPCTSTR)GetHost(), (LPCTSTR)GetPort(), (LPCTSTR)GetObject(), (LPCTSTR)GetUser());
			if (resultCode != ERROR_SUCCESS) {
				break;
			}

			CheckDlgButton(IDC_CHECK_READ, FALSE);
			CheckDlgButton(IDC_CHECK_WRITE, FALSE);
			CheckDlgButton(IDC_CHECK_EXEC, FALSE);
			if (((accessMask & GENERIC_ALL) == GENERIC_ALL) || ((accessMask & FILE_ALL_ACCESS) == FILE_ALL_ACCESS)) {
				ostr << " ( Full Control )" << endl;
				CheckDlgButton(IDC_CHECK_READ, TRUE);
				CheckDlgButton(IDC_CHECK_WRITE, TRUE);
				CheckDlgButton(IDC_CHECK_EXEC, TRUE);
			}
			else {
				ostr << " (";
				if (((accessMask & GENERIC_READ) == GENERIC_READ)
					|| ((accessMask & FILE_GENERIC_READ) == FILE_GENERIC_READ))
				{
					ostr << " Read";
					CheckDlgButton(IDC_CHECK_READ, TRUE);
				}
				if (((accessMask & GENERIC_WRITE) == GENERIC_WRITE)
					|| ((accessMask & FILE_GENERIC_WRITE) == FILE_GENERIC_WRITE))
				{
					ostr << " Write";
					CheckDlgButton(IDC_CHECK_WRITE, TRUE);
				}
				if (((accessMask & GENERIC_EXECUTE) == GENERIC_EXECUTE)
					|| ((accessMask & FILE_GENERIC_EXECUTE) == FILE_GENERIC_EXECUTE))
				{
					ostr << " Execute";
					CheckDlgButton(IDC_CHECK_EXEC, TRUE);
				}
				ostr << " )" << endl;
			}

			break;

		case reqObjectOwn:
			resultCode = client->GetObjectOwner(szObjectOwner, MAX_PATH, (LPCTSTR)GetHost(), (LPCTSTR)GetPort(), (LPCTSTR)GetObject());
			if (resultCode != ERROR_SUCCESS) {
				break;
			}

			SetDlgItemText(IDC_EDIT_OWNER, szObjectOwner);

			break;

		}

		if (resultCode != ERROR_SUCCESS) {
			ostr << TEXT("��� ������: ") << resultCode << endl;
			MessageBox(ostr.str().c_str(), TEXT("��������� �������"), MB_OK);
		}
	}
#endif
}

void CClientDlg::OnBnClickedOk()
{
	CString csValue;
	GetDlgItemText(IDC_EDIT_HOST, csValue);
	WritePrivateProfileString(_T("common"), _T("host"), (LPCTSTR)csValue, szIniFileName);
	GetDlgItemText(IDC_EDIT_PORT, csValue);
	WritePrivateProfileString(_T("common"), _T("port"), (LPCTSTR)csValue, szIniFileName);
	GetDlgItemText(IDC_EDIT_OBJECT, csValue);
	WritePrivateProfileString(_T("common"), _T("object"), (LPCTSTR)csValue, szIniFileName);
	GetDlgItemText(IDC_EDIT_DISK, csValue);
	WritePrivateProfileString(_T("common"), _T("disk"), (LPCTSTR)csValue, szIniFileName);
	GetDlgItemText(IDC_EDIT_OWNER, csValue);
	WritePrivateProfileString(_T("common"), _T("user"), (LPCTSTR)csValue, szIniFileName);

	CDialogEx::OnOK();
}

void CClientDlg::OnLbnSelchangeListHosts()
{
	CString csHost;
	int index = m_lstHosts.GetCurSel();
	if (LB_ERR == index) {
		return;
	}
	m_lstHosts.GetText(index, csHost);
	SetDlgItemText(IDC_EDIT_HOST, (LPCTSTR)csHost);
}

char* CmdLineToArgv(char * CmdLine, int & argc, char **& argv)
{
	argc = 0;
	argv = NULL;
	char * result = 0;
	if (CmdLine) {
		result = new char[strlen(CmdLine) + 1];
		strcpy(result, CmdLine);
		if (result) {
			const char * seps = " ";
			char * token = strtok(CmdLine, seps);
			while (token) {
				argv = (PCHAR*)realloc(argv, ++argc * sizeof(PCHAR));
				argv[argc - 1] = token; // new char[strlen(token) + 1]
				token = strtok(0, seps);
			}
		}
	}
	return result;
}

void CClientDlg::InitOrb()
{
	int argc;
	char** argv;
	CmdLineToArgv(GetCommandLine(), argc, argv);
	orb = CORBA::ORB_init(argc, argv);
#if 0
	infoProvider = GetInfoProvider("localhost");
	if (CORBA::is_nil(infoProvider.in())) {
		cerr << "InfoProvider: zero ptr" << endl;
	}
	else {
		cout << "resolved InfoProvider object" << endl;
	}
#endif
}

Agent::InfoProvider_var CClientDlg::GetInfoProvider(const std::string& host, const std::string& port) {
	cout << "resolving naming service" << endl;
	CosNaming::NamingContext_var nc = CosNaming::NamingContext::_nil();
	try {
#if 1
		string ior = "corbaloc:iiop:1.1@" + host + ":" + port + "/NameService";
		//LPCTSTR ior = _T("corbaloc:iiop:1.1@localhost:9999/NameService");
		CORBA::Object_var obj = orb->string_to_object(ior.c_str());
		nc = CosNaming::NamingContext::_narrow(obj.in());
#else
		ACE_Time_Value timeout(0, 1000000);
		CORBA::Object_var obj = orb->resolve_initial_references("NameService", &timeout);
		nc = CosNaming::NamingContext::_narrow(obj.in());
#endif
	}
	catch (CORBA::Exception &) {
		cerr << "could not resolve existing NameService (exeption)" << endl;
		return Agent::InfoProvider::_nil();
	}
	if (CORBA::is_nil(nc.in())) {
		cerr << "NameService: zero ptr" << endl;
		return Agent::InfoProvider::_nil();
	}
	cout << "resolved existing NameService" << endl;

	CosNaming::Name name(1);
	name.length(1);
	name[0].id = CORBA::string_dup("SysInfoAgent");
	CORBA::Object_var obj = nc->resolve(name);

	Agent::InfoProvider_var result = Agent::InfoProvider::_narrow(obj.in());
	return result;
}
