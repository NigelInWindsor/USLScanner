#if !defined(AFX_WAYLINESEDITDLG_H__40F1AA16_96F7_4668_83D9_3B48B1E90CA2__INCLUDED_)
#define AFX_WAYLINESEDITDLG_H__40F1AA16_96F7_4668_83D9_3B48B1E90CA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WayLinesEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWayLinesEditDlg dialog

class CWayLinesEditDlg : public CDialog
{
// Construction
public:
	HRESULT UICalculateWayPointsFinished(WPARAM wParam, LPARAM lParam);
	void DisplayMaxWayPoints();
	void SetAccessPrivelages();
	void UpdatePmacTrajectory();
	void UpdateParent(int nAction);
	void UpdateAllControls();
	
	int m_nLeft;
	int m_nTop;
	CWnd* m_pParent;
	CWnd** m_pDlg;
	CWayLinesEditDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg = NULL,int nLeft =0, int nTop = 0);

// Dialog Data
	//{{AFX_DATA(CWayLinesEditDlg)
	enum { IDD = IDD_EDITWAYLINES_DIALOG };
	CButton	m_buttonCalculate;
	CProgressCtrl	m_progressBar;
	CEdit	m_editMaxWayPoints;
	CComboBox	m_comboMode;
	CEdit	m_editSegmentCount;
	CEdit	m_editDevVector;
	CEdit	m_editDevNorm;
	CSpinButtonCtrl	m_spinLineNumber;
	CEdit	m_editLineNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWayLinesEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWayLinesEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditLineNumber();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnDeltaposSpinLineNumber(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboWaypointsMode();
	afx_msg void OnChangeEditDevNorm();
	afx_msg void OnChangeEditDevVector();
	afx_msg void OnChangeEditSegmentCount();
	afx_msg void OnButtonRecalculate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAYLINESEDITDLG_H__40F1AA16_96F7_4668_83D9_3B48B1E90CA2__INCLUDED_)
