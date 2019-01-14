#if !defined(AFX_TOOLBARPAGE_H__FCD4CAB3_84C1_4DD2_9AEF_8FB899506543__INCLUDED_)
#define AFX_TOOLBARPAGE_H__FCD4CAB3_84C1_4DD2_9AEF_8FB899506543__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolbarPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolbarPage dialog

class CToolbarPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CToolbarPage)

// Construction
public:
	CToolbarPage();
	~CToolbarPage();

// Dialog Data
	//{{AFX_DATA(CToolbarPage)
	enum { IDD = IDD_TOOLBAR_PAGE };
	//}}AFX_DATA
	CCheckListBox	m_listToolBar;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CToolbarPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nID[9] = { IDR_AIR_WATER_SIDE_0_TOOLBAR, IDR_AIR_WATER_SIDE_1_TOOLBAR, IDR_DUAL_WATERPATH_THICKNESS_TOOLBAR, IDR_MOTION_TOOLBAR_DUAL_SQUIRTER , 
		IDR_ESTOP_JOY_SAFE_TOOLBAR , IDR_LOG_ON_OFF_TOOLBAR , IDR_USEFULL_TOOLBAR, IDR_DANGERPLANE_TOOLBAR, IDR_TIPS_TOOLBAR };

	// Generated message map functions
	//{{AFX_MSG(CToolbarPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListToolbars();
	afx_msg void OnCheckChangeListToolbars();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARPAGE_H__FCD4CAB3_84C1_4DD2_9AEF_8FB899506543__INCLUDED_)
