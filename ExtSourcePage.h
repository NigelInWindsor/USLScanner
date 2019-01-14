#if !defined(AFX_EXTSOURCEPAGE_H__215667BD_D0D6_433C_BD36_1E06E748F810__INCLUDED_)
#define AFX_EXTSOURCEPAGE_H__215667BD_D0D6_433C_BD36_1E06E748F810__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtSourcePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExtSourcePage dialog

class CExtSourcePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CExtSourcePage)

// Construction
public:
	void UpdateDataTranslationHardWare();
	void UpdateAllControls();
	
	CExtSourcePage();
	~CExtSourcePage();

// Dialog Data
	//{{AFX_DATA(CExtSourcePage)
	enum { IDD = IDD_EXT_SOURCE_PAGE };
	CEdit	m_editADC6;
	CEdit	m_editADC5;
	CComboBox	m_comboOutputMode;
	CEdit	m_editADC4;
	CEdit	m_editADC3;
	CEdit	m_editADC2;
	CEdit	m_editADC1;
	CComboBox	m_comboInputRange4;
	CComboBox	m_comboInputRange3;
	CComboBox	m_comboInputRange2;
	CComboBox	m_comboInputRange1;
	CButton	m_checkCollectADC6;
	CButton	m_checkCollectADC5;
	CButton	m_checkCollectADC4;
	CButton	m_checkCollectADC3;
	CButton	m_checkCollectADC2;
	CButton	m_checkCollectADC1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CExtSourcePage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CExtSourcePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboAdc1();
	afx_msg void OnSelchangeComboOutputMode();
	afx_msg void OnSelchangeComboAdc2();
	afx_msg void OnSelchangeComboAdc3();
	afx_msg void OnSelchangeComboAdc4();
	afx_msg void OnCheckCollectAdc1();
	afx_msg void OnCheckCollectAdc2();
	afx_msg void OnCheckCollectAdc3();
	afx_msg void OnCheckCollectAdc4();
	afx_msg void OnCheckCollectAdc5();
	afx_msg void OnCheckCollectAdc6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTSOURCEPAGE_H__215667BD_D0D6_433C_BD36_1E06E748F810__INCLUDED_)
