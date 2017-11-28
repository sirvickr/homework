
// ClientDlg.h : header file
//

#pragma once
#include "afxwin.h"

class CTcpClient;

// CClientDlg dialog
class CClientDlg : public CDialogEx
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CClientDlg();

// Dialog Data
	enum { IDD = IDD_CLIENT_DIALOG };

	void SetHost(const CString &value);
	CString GetHost();

	void SetPort(const CString &value);
	CString GetPort();

	void SetObject(const CString &value);
	CString GetObject();

	void SetDisk(const CString &value);
	CString GetDisk();

	void SetUser(const CString &value);
	CString GetUser();

	void SetReplyText(const CString &value);
	CString GetReplyText();

	void SetAccRead(BOOL value);
	BOOL GetAccRead();

	void SetAccWrite(BOOL value);
	BOOL GetAccWrite();

	void SetAccExec(BOOL value);
	BOOL GetAccExec();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	static constexpr LPTSTR IniFile = _T("config.ini");
	TCHAR szIniFileName[MAX_PATH];
	HICON m_hIcon;
	CString m_csHost;
	CString m_csPort;
	CString m_csObject;
	CString m_csDisk;
	CString m_csUser;
	CString m_csReplyText;

	BOOL m_bAccRead;
	BOOL m_bAccWrite;
	BOOL m_bAccExec;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCmdconnect();

private:
	CTcpClient* client;
	CListBox m_lstHosts;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeListHosts();
};
