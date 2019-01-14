#if !defined(AFX_TRANSLATIONDLG_H__B36DCB0B_FD24_4D81_8425_7671D25B27B7__INCLUDED_)
#define AFX_TRANSLATIONDLG_H__B36DCB0B_FD24_4D81_8425_7671D25B27B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TranslationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTranslationDlg dialog

class CTranslationDlg : public CDialog
{
// Construction
public:
	void UpdateParent(int nAction);
	void RefreshLocationHoleDlg();
	void UpdateAllControls();
	void UpdateAllEditControls();
	void UpdateAllSpinControls();
	APP m_pApp;
	CWnd* m_pParent;
	CWnd** m_pDlg;
	
	int		m_nLeft;
	int		m_nTop;
	CTranslationDlg(CWnd* pParent = NULL,CWnd** pDlg=NULL,int nLeft=0,int nTop=0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTranslationDlg)
	enum { IDD = IDD_TRANSFORM_DIALOG };
	CSpinButtonCtrl	m_spinRRotation;
	CEdit	m_editRRotation;
	CComboBox	m_comboOrientation;
	CComboBox	m_comboSecondaryAxis;
	CComboBox	m_comboPrimaryAxis;
	CComboBox	m_comboPerspective;
	CComboBox	m_comboDisplayLines;
	CSpinButtonCtrl	m_spinZTranslation;
	CSpinButtonCtrl	m_spinZRotation;
	CSpinButtonCtrl	m_spinYTranslation;
	CSpinButtonCtrl	m_spinYRotation;
	CSpinButtonCtrl	m_spinXTranslation;
	CSpinButtonCtrl	m_spinXRotation;
	CEdit	m_editZTranslation;
	CEdit	m_editZRotation;
	CEdit	m_editYTranslation;
	CEdit	m_editYRotation;
	CEdit	m_editXTranslation;
	CEdit	m_editXRotation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTranslationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTranslationDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditXRotation();
	afx_msg void OnChangeEditYRotation();
	afx_msg void OnChangeEditZRotation();
	afx_msg void OnChangeEditXTranslation();
	afx_msg void OnChangeEditYTranslation();
	afx_msg void OnChangeEditZTranslation();
	afx_msg void OnButtonEngage();
	afx_msg void OnSelchangeComboDisplayLines();
	afx_msg void OnButtonReverseLines();
	afx_msg void OnSelchangeComboPerspective();
	afx_msg void OnCheckInvert();
	afx_msg void OnSelchangeComboPrimaryAxis();
	afx_msg void OnSelchangeComboSecondaryAxis();
	afx_msg void OnSelchangeComboOrientation();
	afx_msg void OnDeltaposSpinXTranslation(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinYTranslation(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinZTranslation(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditRRotation();
	afx_msg void OnButtonZero();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSLATIONDLG_H__B36DCB0B_FD24_4D81_8425_7671D25B27B7__INCLUDED_)
