#if !defined(AFX_ROBOTJOINTSPAGE_H__486A3209_D31C_4CB7_88DF_D7E9BDF55E59__INCLUDED_)
#define AFX_ROBOTJOINTSPAGE_H__486A3209_D31C_4CB7_88DF_D7E9BDF55E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RobotJointsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRobotJointsPage dialog

class CRobotJointsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRobotJointsPage)

// Construction
public:
	int	m_nRobot;
	int	m_nStartAxis;

	
	void UpdateAllControls();
	CRobotJointsPage();
	~CRobotJointsPage();

// Dialog Data
	//{{AFX_DATA(CRobotJointsPage)
	enum { IDD = IDD_ROBOT_JOINTS_PAGE };
	CEdit	m_editMinRange1;
	CEdit	m_editMinRange2;
	CEdit	m_editMinRange3;
	CEdit	m_editMinRange4;
	CEdit	m_editMinRange5;
	CEdit	m_editMinRange6;
	CEdit	m_editMaxVelocity1;
	CEdit	m_editMaxVelocity2;
	CEdit	m_editMaxVelocity3;
	CEdit	m_editMaxVelocity4;
	CEdit	m_editMaxVelocity5;
	CEdit	m_editMaxVelocity6;
	CEdit	m_editMaxRange1;
	CEdit	m_editMaxRange2;
	CEdit	m_editMaxRange3;
	CEdit	m_editMaxRange4;
	CEdit	m_editMaxRange5;
	CEdit	m_editMaxRange6;
	CEdit	m_editAccel1;
	CEdit	m_editAccel2;
	CEdit	m_editAccel3;
	CEdit	m_editAccel4;
	CEdit	m_editAccel5;
	CEdit	m_editAccel6;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRobotJointsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRobotJointsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnAcceptButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTJOINTSPAGE_H__486A3209_D31C_4CB7_88DF_D7E9BDF55E59__INCLUDED_)
