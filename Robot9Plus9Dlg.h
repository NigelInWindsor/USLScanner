#if !defined(AFX_ROBOT9PLUS9DLG_H__673E2E6C_CA26_42A8_B9AF_D7BBEDCCD91C__INCLUDED_)
#define AFX_ROBOT9PLUS9DLG_H__673E2E6C_CA26_42A8_B9AF_D7BBEDCCD91C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Robot9Plus9Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRobot9Plus9Dlg dialog

class CRobot9Plus9Dlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CRobot9Plus9Dlg)

// Construction
public:
	
	void UpdateAllControls();
	CRobot9Plus9Dlg();
	~CRobot9Plus9Dlg();

// Dialog Data
	//{{AFX_DATA(CRobot9Plus9Dlg)
	enum { IDD = IDD_ROBOT_9_PLUS_9_DLG };
	CEdit	m_editPosZRight;
	CEdit	m_editPosZLeft;
	CEdit	m_editPosYRight;
	CEdit	m_editPosYLeft;
	CEdit	m_editPosXRight;
	CEdit	m_editPosXLeft;
	CButton	m_checkIncludeZAxis;
	CButton	m_checkIncludeYAxis;
	CButton	m_checkIncludeXAxis;
	CEdit	m_editStepSize13;
	CEdit	m_editStepSize12;
	CEdit	m_editStepSize11;
	CEdit	m_editStepSize3;
	CEdit	m_editStepSize2;
	CEdit	m_editStepSize1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRobot9Plus9Dlg)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRobot9Plus9Dlg)
	afx_msg void OnChangeEditStepsize1();
	afx_msg void OnChangeEditStepsize2();
	afx_msg void OnChangeEditStepsize3();
	afx_msg void OnChangeEditStepsize11();
	afx_msg void OnChangeEditStepsize12();
	afx_msg void OnChangeEditStepsize13();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonHome1();
	afx_msg void OnButtonHome2();
	afx_msg void OnButtonHome3();
	afx_msg void OnButtonHome11();
	afx_msg void OnButtonHome12();
	afx_msg void OnButtonHome13();
	afx_msg void OnButtonAll();
	afx_msg void OnButtonAllLeft();
	afx_msg void OnButtonAllRight();
	afx_msg void OnCheckIncludeXAxis();
	afx_msg void OnCheckIncludeYAxis();
	afx_msg void OnCheckIncludeZAxis();
	afx_msg void OnButtonZeroLeft();
	afx_msg void OnButtonZeroRight();
	afx_msg void OnButtonHomeFailed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOT9PLUS9DLG_H__673E2E6C_CA26_42A8_B9AF_D7BBEDCCD91C__INCLUDED_)
