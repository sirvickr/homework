
// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "TcpClient.h"
#include "afxdialogex.h"
#include <sstream>
#include <iostream>
#include <iomanip>

typedef std::basic_string<TCHAR> tstring;
typedef std::basic_ostream<TCHAR> tostream;
typedef std::basic_istream<TCHAR> tistream;
typedef std::basic_ostringstream<TCHAR> tostringstream;
typedef std::basic_istringstream<TCHAR> tistringstream;

#if defined(UNICODE) || defined(_UNICODE)
#define tcout std::wcout
#define tcerr std::wcerr
#else
#define tcout std::cout
#define tcerr std::cerr
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent), client(NULL)
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
	using namespace std;
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
		const TCHAR* objectPath = TEXT("D:\\zhuk.png");
		AgentRequest request;
		request.reqType = reqObjectOwn;
		if(request.reqType == reqObjectOwn) {
			request.pathSize = wcslen(objectPath);
		} else {
			request.pathSize = 0;
		}

		AgentReply reply;
		memset(&reply, 0x00, sizeof(reply));

		reply.reqType = request.reqType;
		
		int nRet = client->Request(reply);
	
		ostr << TEXT("Код возврата: ") << nRet << TEXT("\n") << endl;

		if(0 == nRet) {
#if 1
			switch(reply.reqType) {
			case reqOsVer:
				reply.vf.osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
				if (reply.valid) {
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
				}
				else {
					ostr << "GetVersionEx failed: " << reply.valid << endl;
				}
				break;
			case reqSysTime:
				if(reply.valid) {
					ostr << "GetSystemTime: " << reply.vf.sysTime.wYear
						<< "-" << reply.vf.sysTime.wMonth
						<< "-" << reply.vf.sysTime.wDay
						<< " (" << reply.vf.sysTime.wDayOfWeek << ")"
						<< " " << reply.vf.sysTime.wHour
						<< ":" << reply.vf.sysTime.wMinute
						<< ":" << reply.vf.sysTime.wSecond
						<< "." << reply.vf.sysTime.wMilliseconds
					<< endl;
				}
				break;
			case reqTickCount:
				if(reply.valid) {
					ostr << "GetTickCount: " << reply.vf.tickCount << endl;
				}
				break;
			case reqMemStatus:
				if(reply.valid) {
					ostr << "percent of memory in use " << reply.vf.memStatus.dwMemoryLoad << endl;
					ostr << "total KB of physical memory " << reply.vf.memStatus.ullTotalPhys << endl;
					ostr << "free  KB of physical memory " << reply.vf.memStatus.ullAvailPhys << endl;
					ostr << "total KB of paging file " << reply.vf.memStatus.ullTotalPageFile << endl;
					ostr << "free  KB of paging file " << reply.vf.memStatus.ullAvailPageFile << endl;
					ostr << "total KB of virtual memory " << reply.vf.memStatus.ullTotalVirtual << endl;
					ostr << "free  KB of virtual memory " << reply.vf.memStatus.ullAvailVirtual << endl;
					ostr << "free  KB of extended memory " << reply.vf.memStatus.ullAvailExtendedVirtual << endl;
				}
				else {
					ostr << "GlobalMemoryStatusEx failed: " << reply.valid << endl;
				}
				break;
			case reqDriveType:
				if(reply.valid) {
					ostr << "GetDriveType: " << reply.vf.driveType << endl;
				}
				break;
			case reqFreeSpace:
				if (reply.valid) {
					ostr << "total " << reply.vf.freeSpace.totalNumberOfBytes.QuadPart
						<< " free " << reply.vf.freeSpace.totalNumberOfFreeBytes.QuadPart
						<< " (" << reply.vf.freeSpace.freeBytesAvailable.QuadPart << ")" << endl;
				}
				else {
					ostr << "GlobalMemoryStatusEx failed: " << reply.valid << endl;
				}
				break;
			case reqObjectAcl:
				break;
			case reqObjectOwn:
				if (reply.valid) {
					ostr << TEXT("owner.size = ") << reply.vf.objectOwn.size << endl;
					reply.vf.objectOwn.name[reply.vf.objectOwn.size] = TEXT('\0');
					ostr << TEXT("owner.name = \"") << reply.vf.objectOwn.name << TEXT("\"") << endl;
				}
				else {
					ostr << "GlobalMemoryStatusEx failed: " << reply.valid << endl;
				}
				break;
			default:
				ostr << "Unknown request: " << reply.reqType << endl;
			}
#endif
		} else {
			ostr << "Сетевая ошибка: " << nRet << endl;
		}

		MessageBox(ostr.str().c_str(), TEXT("Результат запроса"), MB_OK);
	}
}
