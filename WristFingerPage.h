#if !defined(AFX_WRISTFINGERPAGE_H__30D5F395_65CC_4CEC_A88A_23F6BCFB09C2__INCLUDED_)
#define AFX_WRISTFINGERPAGE_H__30D5F395_65CC_4CEC_A88A_23F6BCFB09C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WristFingerPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWristFingerPage dialog

class CWristFingerPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CWristFingerPage)

// Construction
public:
	int	m_nLeft;
	int m_nRight;
	void UpdateAllControls();
	CWristFingerPage();
	~CWristFingerPage();

// Dialog Data
	//{{AFX_DATA(CWristFingerPage)
	enum { IDD = IDD_WRIST_FINGER_PAGE };
	CEdit	m_editYOffsetRight;
	CEdit	m_editYOffsetLeft;
	CEdit	m_editRightXOffset;
	CEdit	m_editRightArmLength;
	CEdit	m_editMirrorNozzleRight;
	CEdit	m_editInternalWaterPathRight;
	CEdit	m_editMirrorNozzleLeft;
	CEdit	m_editInternalWaterPathLeft;
	CEdit	m_editLeftArmLength;
	CEdit	m_editLeftXOffset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWristFingerPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWristFingerPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditLeftArmLength();
	afx_msg void OnChangeEditXLeftOffset();
	afx_msg void OnChangeEditIntWpLeft();
	afx_msg void OnChangeEditMirrorNozzleLeft();
	afx_msg void OnChangeEditXRightOffset();
	afx_msg void OnChangeEditRightArmLength();
	afx_msg void OnChangeEditMirrorNozzleRight();
	afx_msg void OnChangeEditIntWpRight();
	afx_msg void OnChangeEditYOffsetLeft();
	afx_msg void OnChangeEditYOffsetRight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WRISTFINGERPAGE_H__30D5F395_65CC_4CEC_A88A_23F6BCFB09C2__INCLUDED_)
