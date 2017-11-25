
// ClientDlg.h : header file
//

#pragma once

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
	HICON m_hIcon;
	CString m_csHost;
	CString m_csPort;
	CString m_csObject;
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
};
