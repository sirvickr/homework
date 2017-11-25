
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

// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CClientDlg::IDD, pParent), client(NULL)
, m_bAccRead(false), m_bAccWrite(false), m_bAccExec(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	client = new CTcpClient();
}

CClientDlg::~CClientDlg()
{
	if(client) {
		delete client;
	}
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
	//DDX_Text(pDX, IDC_TEXT_REPLY, m_csReplyText);
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
	CheckRadioButton(IDC_RADIO1, IDC_RADIO8, IDC_RADIO1);

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
	//TCHAR buff[BuffSize];
	if(client) {
		/*
		reqOsVer,
		reqSysTime,
		reqTickCount,
		reqMemStatus,
		reqDriveType,
		reqFreeSpace,
		reqObjectAcl,
		reqObjectOwn,
		*/
		LPCTSTR objectPath = NULL;
		LPCTSTR userName = NULL;
		AgentRequest request;
		request.reqSize = sizeof(request.reqSize) + sizeof(request.reqType);
		request.reqType = GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO8) - IDC_RADIO1;

		switch (request.reqType) {
		case reqDriveType:
			objectPath = (LPCTSTR)GetDisk();
			break;
		case reqFreeSpace:
			objectPath = (LPCTSTR)GetDisk();
			break;
		case reqAccessRights:
			objectPath = (LPCTSTR)GetObject();
			userName = (LPCTSTR)GetUser();
			break;
		case reqObjectOwn:
			objectPath = (LPCTSTR)GetObject();
			break;
		default:
			break;
		}
		if(objectPath)
			request.reqSize += wcslen(objectPath) * sizeof(TCHAR);
		
		AgentReply reply = client->Request((LPCTSTR)GetHost(), (LPCTSTR)GetPort(), request, objectPath);
	
		if(reply.errorCode == 0) {
#if 1
			switch(reply.reqType) {
			case reqOsVer:
				ostr << reply.vf.osVer.dwMajorVersion << "." << reply.vf.osVer.dwMinorVersion << "." << reply.vf.osVer.dwBuildNumber;
				SetDlgItemText(IDC_EDIT_OSVER, ostr.str().c_str());

				SetDlgItemInt(IDC_EDIT_PLATFORM, reply.vf.osVer.dwPlatformId);

				SetDlgItemText(IDC_EDIT_SPSTR, reply.vf.osVer.szCSDVersion);

				ostr.str(TEXT(""));
				ostr << reply.vf.osVer.wServicePackMajor << "." << reply.vf.osVer.wServicePackMinor;
				SetDlgItemText(IDC_EDIT_SPNUM, ostr.str().c_str());

				ostr.str(TEXT(""));
				ostr << hex << uppercase << setfill(TEXT('0')) << setw(8) << reply.vf.osVer.wSuiteMask << dec;
				SetDlgItemText(IDC_EDIT_SITE, ostr.str().c_str());

				ostr.str(TEXT(""));
				ostr << " wProductType " << DWORD(reply.vf.osVer.wProductType);
				SetDlgItemText(IDC_EDIT_SITE, ostr.str().c_str());
				break;
			case reqSysTime:
				ostr << reply.vf.sysTime.wHour << ":" << reply.vf.sysTime.wMinute << ":" << reply.vf.sysTime.wSecond;
				SetDlgItemText(IDC_EDIT_SYSTIME, ostr.str().c_str());
				break;
			case reqTickCount:
				SetDlgItemInt(IDC_EDIT_TICKS, reply.vf.tickCount);
				break;
			case reqMemStatus:
				ostr.str(TEXT(""));
				ostr << reply.vf.memStatus.dwMemoryLoad;
				SetDlgItemText(IDC_EDIT_MEMLOAD, ostr.str().c_str());
				ostr.str(TEXT(""));
				ostr << reply.vf.memStatus.ullTotalPhys;
				SetDlgItemText(IDC_EDIT_MEMTOTALPHYS, ostr.str().c_str());
				ostr.str(TEXT(""));
				ostr << reply.vf.memStatus.ullAvailPhys;
				SetDlgItemText(IDC_EDIT_MEMAVAILPHYS, ostr.str().c_str());
				ostr.str(TEXT(""));
				ostr << reply.vf.memStatus.ullTotalPageFile;
				SetDlgItemText(IDC_EDIT_MEMTOTALPAGE, ostr.str().c_str());
				ostr.str(TEXT(""));
				ostr << reply.vf.memStatus.ullAvailPageFile;
				SetDlgItemText(IDC_EDIT_MEMAVAILPAGE, ostr.str().c_str());
				ostr.str(TEXT(""));
				ostr << reply.vf.memStatus.ullTotalVirtual;
				SetDlgItemText(IDC_EDIT_MEMTOTALVIRT, ostr.str().c_str());
				ostr.str(TEXT(""));
				ostr << reply.vf.memStatus.ullAvailVirtual;
				SetDlgItemText(IDC_EDIT_MEMAVAILVIRT, ostr.str().c_str());
				ostr.str(TEXT(""));
				ostr << reply.vf.memStatus.ullAvailExtendedVirtual;
				SetDlgItemText(IDC_EDIT_MEMAVAILVIRTEXT, ostr.str().c_str());
				break;
			case reqDriveType:
				ostr << reply.vf.driveType << " (";
				switch (reply.vf.driveType) {
				case DRIVE_UNKNOWN:
					ostr << TEXT("неизвестный тип");
					break;
				case DRIVE_NO_ROOT_DIR:
					ostr << TEXT("неверный путь: ") << objectPath;
					break;
				case DRIVE_REMOVABLE:
					ostr << TEXT("съемное устройство");
					break;
				case DRIVE_FIXED:
					ostr << TEXT("жесткий диск");
					break;
				case DRIVE_REMOTE:
					ostr << TEXT("сетевой диск");
					break;
				case DRIVE_CDROM:
					ostr << TEXT("CD-ROM");
					break;
				case DRIVE_RAMDISK:
					ostr << TEXT("виртуальный диск");
					break;
				default:
					ostr << TEXT("неизвестный код");
					break;
				}
				ostr << ")";
				SetDlgItemText(IDC_EDIT_DRIVE, ostr.str().c_str());
				break;
			case reqFreeSpace:
				ostr.str(TEXT(""));
				ostr << reply.vf.freeSpace.totalNumberOfBytes.QuadPart;
				SetDlgItemText(IDC_EDIT_TOTALBYTES, ostr.str().c_str());
				ostr.str(TEXT(""));
				ostr << reply.vf.freeSpace.totalNumberOfFreeBytes.QuadPart;
				SetDlgItemText(IDC_EDIT_TOTALFREEBYTES, ostr.str().c_str());
				ostr.str(TEXT(""));
				ostr << reply.vf.freeSpace.freeBytesAvailable.QuadPart;
				SetDlgItemText(IDC_EDIT_TOTALAVAILBYTES, ostr.str().c_str());
				break;
			case reqAccessRights:
				ostr << "Effective Allowed Access Mask: " << hex << uppercase 
					<< setfill(_T('0')) << setw(8) << reply.vf.accessMask << dec;
				if (((reply.vf.accessMask & GENERIC_ALL) == GENERIC_ALL)
					|| ((reply.vf.accessMask & FILE_ALL_ACCESS) == FILE_ALL_ACCESS))
				{
					ostr << " ( Full Control )" << endl;
					CheckDlgButton(IDC_CHECK_READ, TRUE);
					CheckDlgButton(IDC_CHECK_WRITE, TRUE);
					CheckDlgButton(IDC_CHECK_EXEC, TRUE);
				} else {
					ostr << " (";
					if (((reply.vf.accessMask & GENERIC_READ) == GENERIC_READ)
						|| ((reply.vf.accessMask & FILE_GENERIC_READ) == FILE_GENERIC_READ))
					{
						ostr << " Read";
						CheckDlgButton(IDC_CHECK_READ, TRUE);
					}
					if (((reply.vf.accessMask & GENERIC_WRITE) == GENERIC_WRITE)
						|| ((reply.vf.accessMask & FILE_GENERIC_WRITE) == FILE_GENERIC_WRITE))
					{
						ostr << " Write";
						CheckDlgButton(IDC_CHECK_WRITE, TRUE);
					}
					if (((reply.vf.accessMask & GENERIC_EXECUTE) == GENERIC_EXECUTE)
						|| ((reply.vf.accessMask & FILE_GENERIC_EXECUTE) == FILE_GENERIC_EXECUTE))
					{
						ostr << " Execute";
						CheckDlgButton(IDC_CHECK_READ, TRUE);
					}
					ostr << " )" << endl;
				}
				break;
			case reqObjectOwn:
				SetDlgItemText(IDC_EDIT_OWNER, reply.vf.objectOwn.name);
				break;
			default:
				ostr << "Unknown request: " << reply.reqType << endl;
			}
#endif
		} else {
			ostr << TEXT("Код ошибки: ") << reply.errorCode << endl;
			MessageBox(ostr.str().c_str(), TEXT("Результат запроса"), MB_OK);
		}

	}
}
