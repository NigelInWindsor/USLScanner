#if !defined(AFX_ORIGINFOREPLANEPAGE_H__501D7C57_E9A8_44FD_8D62_388B4532AFEA__INCLUDED_)
#define AFX_ORIGINFOREPLANEPAGE_H__501D7C57_E9A8_44FD_8D62_388B4532AFEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OriginForeplanePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COriginForeplanePage dialog

class COriginForeplanePage : public CPropertyPage
{
	DECLARE_DYNCREATE(COriginForeplanePage)

// Construction
public:
	void SetRegistry();
	
	void UpdateAllControls();
	COriginForeplanePage();
	~COriginForeplanePage();

// Dialog Data
	//{{AFX_DATA(COriginForeplanePage)
	enum { IDD = IDD_ORIGIN_FOREPLANE_PAGE };
	CComboBox	m_comboYTipAxisCombo;
	CComboBox	m_comboXTipAxisOrder;
	CComboBox	m_comboZAxisOrder;
	CComboBox	m_comboYAxisOrder;
	CComboBox	m_comboXAxisOrder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COriginForeplanePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COriginForeplanePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAll();
	afx_msg void OnSelchangeComboXAxis();
	afx_msg void OnSelchangeComboYAxis();
	afx_msg void OnSelchangeComboZAxis();
	afx_msg void OnSelchangeComboXtipAxis();
	afx_msg void OnSelchangeComboYtipAxis();
	afx_msg void OnButtonOriginX();
	afx_msg void OnButtonOriginY();
	afx_msg void OnButtonOriginZ();
	afx_msg void OnButtonOriginXtip();
	afx_msg void OnButtonOriginYtip();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORIGINFOREPLANEPAGE_H__501D7C57_E9A8_44FD_8D62_388B4532AFEA__INCLUDED_)
