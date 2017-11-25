
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
	DDX_Text(pDX, IDC_EDIT_USER, m_csUser);
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
		request.reqType = reqAccessRights;

		switch (request.reqType) {
		case reqDriveType:
			objectPath = (LPCTSTR)GetObject();
			break;
		case reqFreeSpace:
			objectPath = (LPCTSTR)GetObject();
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
	
		ostr << TEXT("Код возврата: ") << reply.errorCode << TEXT("\n") << endl;

		if(reply.errorCode == 0) {
#if 1
			switch(reply.reqType) {
			case reqOsVer:
				reply.vf.osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
				reply.vf.osVer.szCSDVersion[63] = TEXT('\0');
				ostr << "OS version " << reply.vf.osVer.dwMajorVersion
					<< "." << reply.vf.osVer.dwMinorVersion
					<< "." << reply.vf.osVer.dwBuildNumber
					<< endl;
				ostr << " dwPlatformId " << reply.vf.osVer.dwPlatformId << endl;
				ostr << TEXT(" sp \"") << reply.vf.osVer.szCSDVersion << TEXT("\"") << endl;
				ostr << " ServicePack " << reply.vf.osVer.wServicePackMajor 
					<< "." << reply.vf.osVer.wServicePackMinor << endl;
				ostr << " wSuiteMask " << hex << uppercase << setfill(TEXT('0')) 
					<< setw(8) << reply.vf.osVer.wSuiteMask << dec << endl;
				ostr << " wProductType " << DWORD(reply.vf.osVer.wProductType) << endl;
				break;
			case reqSysTime:
				ostr << "GetSystemTime: " << reply.vf.sysTime.wYear
					<< "-" << reply.vf.sysTime.wMonth
					<< "-" << reply.vf.sysTime.wDay
					<< " (" << reply.vf.sysTime.wDayOfWeek << ")"
					<< " " << reply.vf.sysTime.wHour
					<< ":" << reply.vf.sysTime.wMinute
					<< ":" << reply.vf.sysTime.wSecond
					<< "." << reply.vf.sysTime.wMilliseconds
				<< endl;
			case reqTickCount:
				ostr << "GetTickCount: " << reply.vf.tickCount << endl;
				break;
			case reqMemStatus:
				ostr << "percent of memory in use " << reply.vf.memStatus.dwMemoryLoad << endl;
				ostr << "total KB of physical memory " << reply.vf.memStatus.ullTotalPhys << endl;
				ostr << "free  KB of physical memory " << reply.vf.memStatus.ullAvailPhys << endl;
				ostr << "total KB of paging file " << reply.vf.memStatus.ullTotalPageFile << endl;
				ostr << "free  KB of paging file " << reply.vf.memStatus.ullAvailPageFile << endl;
				ostr << "total KB of virtual memory " << reply.vf.memStatus.ullTotalVirtual << endl;
				ostr << "free  KB of virtual memory " << reply.vf.memStatus.ullAvailVirtual << endl;
				ostr << "free  KB of extended memory " << reply.vf.memStatus.ullAvailExtendedVirtual << endl;
				break;
			case reqDriveType:
				ostr << "GetDriveType: " << reply.vf.driveType << endl;
				SetDlgItemText(IDC_EDIT_HOST, _itow(reply.vf.driveType)));
				break;
			case reqFreeSpace:
				ostr << "total " << reply.vf.freeSpace.totalNumberOfBytes.QuadPart
					<< " free " << reply.vf.freeSpace.totalNumberOfFreeBytes.QuadPart
					<< " (" << reply.vf.freeSpace.freeBytesAvailable.QuadPart << ")" << endl;
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
				ostr << TEXT("owner.size = ") << reply.vf.objectOwn.size << endl;
				reply.vf.objectOwn.name[reply.vf.objectOwn.size] = TEXT('\0');
				ostr << TEXT("owner.name = \"") << reply.vf.objectOwn.name << TEXT("\"") << endl;
				break;
			default:
				ostr << "Unknown request: " << reply.reqType << endl;
			}
#endif
		} else {
			ostr << "Код ошибки: " << reply.errorCode << endl;
		}

		MessageBox(ostr.str().c_str(), TEXT("Результат запроса"), MB_OK);
	}
}
