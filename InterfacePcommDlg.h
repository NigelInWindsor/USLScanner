// InterfacePcommDlg.h : header file
//

#if !defined(AFX_INTERFACEPCOMMDLG_H__6E248F15_37B2_4CAB_A93D_81FD645FD025__INCLUDED_)
#define AFX_INTERFACEPCOMMDLG_H__6E248F15_37B2_4CAB_A93D_81FD645FD025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CInterfacePcommDlg dialog

class CInterfacePcommDlg : public CDialog
{
// Construction
public:
	void UpdateAllControls();
	bool m_bCtrlButton;
	CInterfacePcommDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CInterfacePcommDlg)
	enum { IDD = IDD_INTERFACEPCOMM_DIALOG };
	CEdit	m_edit73;
	CEdit	m_edit72;
	CEdit	m_edit6891;
	CEdit	m_edit6890;
	CEdit	m_edit6852;
	CEdit	m_edit6851;
	CEdit	m_edit6850;
	CEdit	m_edit82;
	CEdit	m_edit81;
	CEdit	m_edit80;
	CEdit	m_edit79;
	CEdit	m_edit78;
	CEdit	m_edit71;
	CEdit	m_edit70;
	CEdit	m_edit6841;
	CEdit	m_edit6840;
	CEdit	m_edit6802;
	CEdit	m_edit6801;
	CEdit	m_edit6800;
	CEdit	m_editResponce;
	CEdit	m_editCommand;
	//}}AFX_DATA


	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInterfacePcommDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CInterfacePcommDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDeviceOpen();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERFACEPCOMMDLG_H__6E248F15_37B2_4CAB_A93D_81FD645FD025__INCLUDED_)
