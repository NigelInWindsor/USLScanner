#if !defined(AFX_LASERSCANDLG_H__1C5B4A7E_B154_4FDD_AE16_158A3F2CB794__INCLUDED_)
#define AFX_LASERSCANDLG_H__1C5B4A7E_B154_4FDD_AE16_158A3F2CB794__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LaserScanDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLaserScanDlg dialog

class CLaserScanDlg : public CDialog
{
// Construction
public:
	CLaserScanDlg(CWnd* pParent = NULL ,CWnd** pDlg = NULL,int nLeft=0, int nTop=0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLaserScanDlg)
	enum { IDD = IDD_LASER_SCAN_DLG };
	CButton	m_checkContinuous;
	CEdit	m_editStartPoint;
	CEdit	m_editSlowIncrement;
	CEdit	m_editFinishPoint;
	CEdit	m_editFastIncrement;
	//}}AFX_DATA

	CWnd*	m_pParent;
	CWnd**	m_pDlg;
	bool	m_bDlgCreated;
	int		m_nLeft;
	int		m_nTop;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLaserScanDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLaserScanDlg)
	afx_msg void OnChangeEditStartPoint();
	afx_msg void OnChangeEditFinishPoint();
	afx_msg void OnChangeEditFastIncrement();
	afx_msg void OnChangeEditSlowIncrement();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnButtonScan();
	afx_msg void OnButtonStoreStart();
	afx_msg void OnButtonStoreFinish();
	afx_msg void OnButtonGotoFinish();
	afx_msg void OnButtonGotoStart();
	afx_msg void OnCheckContinuousScan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LASERSCANDLG_H__1C5B4A7E_B154_4FDD_AE16_158A3F2CB794__INCLUDED_)
