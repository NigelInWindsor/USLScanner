#if !defined(AFX_TURNTABLEPAGE1_H__24E076DC_CC51_486E_B885_FEA8955A9983__INCLUDED_)
#define AFX_TURNTABLEPAGE1_H__24E076DC_CC51_486E_B885_FEA8955A9983__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TurntablePage1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTurntablePage dialog

class CTurntablePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTurntablePage)

// Construction
public:
	CVertexArray m_CircumCenterElement;
	bool m_bDlgCreated;
	void UpdateAllControls();
	CTurntablePage();
	~CTurntablePage();

// Dialog Data
	//{{AFX_DATA(CTurntablePage)
	enum { IDD = IDD_TURNTABLE_PAGE };
	CEdit	m_editCoordinate;
	CEdit	m_editLeft;
	CEdit	m_editRight;
	CEdit	m_editCircumcenter;
	CButton	m_buttonCC0;
	CButton	m_buttonCC1;
	CButton	m_buttonCC2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTurntablePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTurntablePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonGoto();
	afx_msg void OnButtonStore();
	afx_msg void OnChangeEditCoordinate();
	afx_msg void OnButtonStoreSafePos();
	afx_msg void OnButtonGotoSafe();
	afx_msg void OnButtonCc0();
	afx_msg void OnButtonCc1();
	afx_msg void OnButtonCc2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TURNTABLEPAGE1_H__24E076DC_CC51_486E_B885_FEA8955A9983__INCLUDED_)
