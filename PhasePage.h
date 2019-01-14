#if !defined(AFX_PHASEPAGE_H__41511436_F82D_4001_B8D7_F2FFE904FAEA__INCLUDED_)
#define AFX_PHASEPAGE_H__41511436_F82D_4001_B8D7_F2FFE904FAEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhasePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPhasePage dialog

class CPhasePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPhasePage)

// Construction
public:
	HBRUSH m_hBr;
	void	UpdateAllControls();
	
	CWnd* m_pViewSheet;
	CData* m_pData;
	CPhasePage(CWnd* pViewSheet = NULL, CData* pData = NULL);
	~CPhasePage();

// Dialog Data
	//{{AFX_DATA(CPhasePage)
	enum { IDD = IDD_PHASE_PAGE };
	CComboBox	m_comboNegativeSign;
	CComboBox	m_comboPositiveSign;
	CComboBox	m_comboOverlayWhich;
	CButton	m_checkPositive;
	CButton	m_checkNegative;
	CEdit	m_editNegativeAmp;
	CSpinButtonCtrl	m_spinPositiveAmp;
	CSpinButtonCtrl	m_spinNegativeAmp;
	CEdit	m_editPositiveAmp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPhasePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPhasePage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnChangeEditPositiveAmplitude();
	afx_msg void OnChangeEditNegativeAmplitude();
	afx_msg void OnDeltaposSpinNegativeAmplitude(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinPositiveAmplitude(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckPositive();
	afx_msg void OnCheckNegative();
	afx_msg void OnSelchangeComboOverlayWhich();
	afx_msg void OnSelchangeComboNegativeSign();
	afx_msg void OnSelchangeComboPositiveSign();
	afx_msg void OnButtonDisplay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHASEPAGE_H__41511436_F82D_4001_B8D7_F2FFE904FAEA__INCLUDED_)
