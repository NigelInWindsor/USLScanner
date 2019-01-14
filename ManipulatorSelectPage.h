#if !defined(AFX_MANIPULATORSELECTPAGE_H__52AFC636_992D_44F2_8E76_0FA18609C9C1__INCLUDED_)
#define AFX_MANIPULATORSELECTPAGE_H__52AFC636_992D_44F2_8E76_0FA18609C9C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManipulatorSelectPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CManipulatorSelectPage dialog

class CManipulatorSelectPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CManipulatorSelectPage)

// Construction
public:
	void UpdateAllControls();
	CManipulatorSelectPage();
	~CManipulatorSelectPage();

// Dialog Data
	//{{AFX_DATA(CManipulatorSelectPage)
	enum { IDD = IDD_MANIPULATOR_SELECT_PAGE };
	CEdit	m_editPrompt;
	CComboBox	m_comboSelectManipulator;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CManipulatorSelectPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CManipulatorSelectPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboSelectManipulator();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANIPULATORSELECTPAGE_H__52AFC636_992D_44F2_8E76_0FA18609C9C1__INCLUDED_)
