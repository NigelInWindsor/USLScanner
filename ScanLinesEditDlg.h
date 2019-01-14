#if !defined(AFX_SCANLINESEDITDLG_H__AED44865_3E16_4566_B148_EEA30B8E8CF5__INCLUDED_)
#define AFX_SCANLINESEDITDLG_H__AED44865_3E16_4566_B148_EEA30B8E8CF5__INCLUDED_

#include "FlattenDlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScanLinesEditDlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScanLinesEditDlg dialog

class CScanLinesEditDlg : public CDialog
{
// Construction
public:
	CFlattenDlg *m_pFlattenDlg;
	void UpdatePmacTrajectory();
	void UpdateParent(int nAction);
	void UpdateAllControls();
	CWnd* m_pParent;
	CWnd** m_pDlg;
	
	int		m_nLeft;
	int		m_nTop;
	CScanLinesEditDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,int nLeft=0,int nTop=0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScanLinesEditDlg)
	enum { IDD = IDD_EDITSCANLINES_DIALOG };
	CStatic	m_staticColor;
	CButton	m_checkCrosses;
	CButton	m_checkLines;
	CButton	m_checkOn;
	CSpinButtonCtrl	m_spinLineNumber;
	CEdit	m_editLineNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScanLinesEditDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScanLinesEditDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditLineNumber();
	afx_msg void OnButtonFlatten();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANLINESEDITDLG_H__AED44865_3E16_4566_B148_EEA30B8E8CF5__INCLUDED_)
