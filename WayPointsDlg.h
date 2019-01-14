#if !defined(AFX_WAYPOINTSDLG_H__FC7FE15F_DA62_4081_BA09_CC62AC8E41E8__INCLUDED_)
#define AFX_WAYPOINTSDLG_H__FC7FE15F_DA62_4081_BA09_CC62AC8E41E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WayPointsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWayPointsDlg dialog

class CWayPointsDlg : public CDialog
{
// Construction
public:
	void UpdateParent(int nAction);
	void DisplayMaxWayPoints();
	void UpdateAllControls();
	
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CWayPointsDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWayPointsDlg)
	enum { IDD = IDD_WAYPOINTS_DLG };
	CProgressCtrl	m_ProgressBar;
	CButton	m_checkWithLiftOff;
	CEdit	m_editMaxWayPoints;
	CEdit	m_editNumberWayPoints;
	CEdit	m_editChangeXYZ;
	CEdit	m_editChangeIJK;
	CComboBox	m_comboWaypointsMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWayPointsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWayPointsDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboWaypointsMode();
	afx_msg void OnChangeEditChangeXyz();
	afx_msg void OnChangeEditChangeIjk();
	afx_msg void OnChangeEditNumberSegments();
	afx_msg void OnButtonRecalculate();
	afx_msg void OnCheckWithLiftOff();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAYPOINTSDLG_H__FC7FE15F_DA62_4081_BA09_CC62AC8E41E8__INCLUDED_)
