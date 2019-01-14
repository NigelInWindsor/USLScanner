#if !defined(AFX_PR20DLG_H__53121CB6_2C1C_11D5_847E_70FF54C1840C__INCLUDED_)
#define AFX_PR20DLG_H__53121CB6_2C1C_11D5_847E_70FF54C1840C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Pr20Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPr20Dlg dialog

class CPr20Dlg : public CPropertyPage
{
// Construction
public:
	int m_nPulserType;
	CPr20Dlg(UINT nCap = NULL,CWnd* pParent = NULL,CWnd** DlgPtr=NULL,int nPulserType=20);
	~CPr20Dlg();

// Dialog Data
	//{{AFX_DATA(CPr20Dlg)
	enum { IDD = IDD_PR20 };
	CComboBox	m_comboRectifier;
	CSpinButtonCtrl	m_PulseVoltageSpin;
	CEdit	m_PulseVoltageEdit;
	CSpinButtonCtrl	m_PrfSpin;
	CEdit	m_PrfEdit;
	CEdit	m_AttenuationEdit;
	CComboBox	m_FilterCombo;
	CSliderCtrl	m_AttenuationSlider;
	CSpinButtonCtrl	m_SpinAttenuation;
	//}}AFX_DATA

	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPr20Dlg)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	struct	PR2050Data* m_pPr;
	struct	UtGlobalData* m_pGlobal;
	CUSLut* m_pCards;
	BOOL	m_bComboListSet;
	CWnd** m_DlgPtr;

	// Generated message map functions
	//{{AFX_MSG(CPr20Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeFiltersCombo();
	afx_msg void OnChangeAttenuation();
	afx_msg void OnClose();
	afx_msg void OnChangePrfEdit();
	afx_msg void OnChangePulseVoltageEdit();
	afx_msg void OnSelchangeRectifierCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PR20DLG_H__53121CB6_2C1C_11D5_847E_70FF54C1840C__INCLUDED_)
