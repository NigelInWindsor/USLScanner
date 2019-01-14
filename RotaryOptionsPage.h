#if !defined(AFX_ROTARYOPTIONSPAGE_H__DE77749F_1F7D_4E89_8C75_FEDD225BB9D0__INCLUDED_)
#define AFX_ROTARYOPTIONSPAGE_H__DE77749F_1F7D_4E89_8C75_FEDD225BB9D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RotaryOptionsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRotaryOptionsPage dialog

class CRotaryOptionsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRotaryOptionsPage)

// Construction
public:
	void UpdateAllControls();
	bool m_bDlgCreated;
	CData* m_pData;
	CWnd* m_pViewSheet;
	CRotaryOptionsPage(CWnd* pViewSheet=NULL,CData* pData=NULL);
	~CRotaryOptionsPage();

// Dialog Data
	//{{AFX_DATA(CRotaryOptionsPage)
	enum { IDD = IDD_ROTARY_VIEW_PAGE };
	CEdit	m_editDiameter1;
	CEdit	m_editDiameter0;
	CEdit	m_editAngle1;
	CEdit	m_editAngle0;
	CComboBox	m_comboDisplayMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRotaryOptionsPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRotaryOptionsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboDisplayMode();
	afx_msg void OnChangeEditAngle0();
	afx_msg void OnChangeEditAngle1();
	afx_msg void OnChangeEditDiameter0();
	afx_msg void OnChangeEditDiameter1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROTARYOPTIONSPAGE_H__DE77749F_1F7D_4E89_8C75_FEDD225BB9D0__INCLUDED_)
