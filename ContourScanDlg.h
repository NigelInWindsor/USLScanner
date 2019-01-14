#if !defined(AFX_CONTOURSCANDLG_H__03F02DE4_76CC_4E4B_A805_3A8CBC1A5F62__INCLUDED_)
#define AFX_CONTOURSCANDLG_H__03F02DE4_76CC_4E4B_A805_3A8CBC1A5F62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContourScanDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CContourScanPage dialog
class CContourScanPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CContourScanPage)

// Construction
public:
	void UpdateInformation();
	int	m_nOldScanLine;
	void SetAccessPrivelage();
	void UpdateAllControls();
	HBRUSH m_hBr;
	CWnd* m_pParent;
	CWnd** m_pDlg;
	
	CContourScanPage();   // standard constructor
	~CContourScanPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CContourScanPage)
	enum { IDD = IDD_CONTOUR_SCAN_DIALOG };
	CComboBox	m_comboBiUinDirectional;
	CButton	m_checkReverseScanLines;
	CComboBox	m_comboLineIndexMode;
	CComboBox	m_comboLineIndex;
	CEdit	m_editScantime;
	CEdit	m_editMemory;
	CSpinButtonCtrl	m_spinScanSpeed;
	CEdit	m_editScanSpeed;
	CButton	m_checkUseBrain;
	CEdit	m_editResumeLine;
	CEdit	m_editScanLine;
	CButton	m_buttonPause;
	CComboBox	m_comboScanWhat;
	CEdit	m_editStartLine;
	CEdit	m_editFinishLine;
	CButton	m_buttonWithLookAhead;
	CEdit	m_editFilePath;
	CComboBox	m_comboScanType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContourScanPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContourScanPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonAbort();
	afx_msg void OnButtonMovetoStart();
	afx_msg void OnChangeEditFilepath();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeEditFinishLine();
	afx_msg void OnChangeEditStartLine();
	afx_msg void OnSelchangeComboScanType();
	afx_msg void OnButtonStartLine();
	afx_msg void OnButtonFinishLine();
	afx_msg void OnChangeEditResumeLine();
	afx_msg void OnCheckUseBrain();
	afx_msg void OnChangeEditScanSpeed();
	afx_msg void OnSelchangeComboLineIndexMode();
	afx_msg void OnSelchangeComboLineIndex();
	afx_msg void OnDeltaposSpinScanSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckReverseScanLines();
	afx_msg void OnSelchangeComboBiUni();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTOURSCANDLG_H__03F02DE4_76CC_4E4B_A805_3A8CBC1A5F62__INCLUDED_)
