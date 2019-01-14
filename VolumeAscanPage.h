#if !defined(AFX_VOLUMEASCANPAGE_H__15F05CD2_2C44_4D6D_9CB6_27C5B217A82D__INCLUDED_)
#define AFX_VOLUMEASCANPAGE_H__15F05CD2_2C44_4D6D_9CB6_27C5B217A82D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VolumeAscanPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVolumeAscanPage dialog

class CVolumeAscanPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CVolumeAscanPage)

// Construction
public:
	HBRUSH m_hBr;
	void InvalidateParent();
	BOOL Dsp200CalculateHardware(int nTimeSlot,int nWhich);
	CString m_Units[6];
	void UpdateAllControls();
	int	m_nGate;
	
	GatesData* m_pGate;
	CData* m_pData;
	CWnd* m_pParent;
	CVolumeAscanPage(CWnd* pParent = NULL,GatesData* pGate = NULL,CData* pData= NULL);
	~CVolumeAscanPage();

// Dialog Data
	//{{AFX_DATA(CVolumeAscanPage)
	enum { IDD = IDD_VOLUME_ASCAN_PAGE };
	CComboBox	m_comboCrossHairValue;
	CComboBox	m_comboCScanGate;
	CSpinButtonCtrl	m_spinFarDeadZone;
	CEdit	m_editFarDeadZone;
	CComboBox	m_comboLockSurface;
	CSpinButtonCtrl	m_spinGateWidth;
	CSpinButtonCtrl	m_spinGateThreshold;
	CSpinButtonCtrl	m_spinGateDelay;
	CEdit	m_editGateWidth;
	CEdit	m_editGateThreshold;
	CEdit	m_editGateDelay;
	CComboBox	m_comboTOFMode;
	CComboBox	m_comboGateNumber;
	CComboBox	m_comboRfType;
	CComboBox	m_comboGateMode;
	CComboBox	m_comboGraticule;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CVolumeAscanPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CVolumeAscanPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeComboGateMode();
	afx_msg void OnSelchangeComboGraticule();
	afx_msg void OnSelchangeComboRftype();
	afx_msg void OnChangeEditGateDelay();
	afx_msg void OnChangeEditGateThreshold();
	afx_msg void OnChangeEditGateWidth();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeComboGateNumber();
	afx_msg void OnSelchangeComboTofMode();
	afx_msg void OnSelchangeComboLockSurface();
	afx_msg void OnChangeEditFarDelay();
	afx_msg void OnSelchangeComboGateCscanGate();
	afx_msg void OnDeltaposSpinDelay(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinWidth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboCrosshairvalue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOLUMEASCANPAGE_H__15F05CD2_2C44_4D6D_9CB6_27C5B217A82D__INCLUDED_)
