#if !defined(AFX_SI6STATUSPAGE_H__5ECA57B5_66B2_4D28_B08B_FF5E43A201B2__INCLUDED_)
#define AFX_SI6STATUSPAGE_H__5ECA57B5_66B2_4D28_B08B_FF5E43A201B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Si6StatusPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSi6StatusPage dialog

class CSi6StatusPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSi6StatusPage)

// Construction
public:
	CSi6StatusPage();
	~CSi6StatusPage();

public:
	

// Dialog Data
	//{{AFX_DATA(CSi6StatusPage)
	enum { IDD = IDD_SI6_STATUS_PAGE };
	CEdit	m_editSwitchEOR;
	CEdit	m_DspEdit;
	int		m_nSAxis;
	int		m_nXAxis;
	int		m_nYAxis;
	int		m_nZAxis;
	int		m_nCountR;
	int		m_nCountX;
	int		m_nCountXt;
	int		m_nCountY;
	int		m_nCountYt;
	int		m_nCountZ;
	int		m_nPosR;
	int		m_nPosX;
	int		m_nPosXt;
	int		m_nPosY;
	int		m_nPosYt;
	int		m_nPosZ;
	CString	m_csProcessState;
	CString	m_csDirReg;
	int		m_nDspIrq;
	int		m_nSi6Irq;
	int		m_nDataPtr;
	CString	m_stringIrqMask;
	CString	m_stringLimitSwitches;
	int		m_nMSCJoy0;
	int		m_nMSCJoy1;
	int		m_nMSCJoy2;
	int		m_nMSCJoy3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSi6StatusPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSi6StatusPage)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeEditSwitchEor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SI6STATUSPAGE_H__5ECA57B5_66B2_4D28_B08B_FF5E43A201B2__INCLUDED_)
