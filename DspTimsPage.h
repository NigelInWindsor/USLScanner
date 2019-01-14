#if !defined(AFX_DSPTIMSPAGE_H__8DB21200_AC1A_41AC_A7F3_D784D021AA08__INCLUDED_)
#define AFX_DSPTIMSPAGE_H__8DB21200_AC1A_41AC_A7F3_D784D021AA08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DspTimsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDspTimsPage dialog

class CDspTimsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDspTimsPage)

// Construction
public:
	void SetAccessPrivelage();
	void UpdateAllControls();
	bool m_bDialogCreated;
	CDspTimsPage();   // standard constructor
	~CDspTimsPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDspTimsPage)
	enum { IDD = IDD_DSP_TIMS_DIALOG };
	CButton	m_checkCollectMetalPath;
	CEdit	m_editMaterialDensity;
	CStatic	m_staticMaterialPath;
	CStatic	m_staticWaterPath;
	CSpinButtonCtrl	m_spinWaterVelocity;
	CSpinButtonCtrl	m_spinMaterialVelocity;
	CEdit	m_editMaterialVelocity;
	CEdit	m_editWaterVelocity;
	CComboBox	m_comboStartSlope;
	CComboBox	m_comboStopSlope;
	CComboBox	m_comboMode;
	CComboBox	m_comboDetection;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDspTimsPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDspTimsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboMode();
	afx_msg void OnSelchangeComboDetection();
	afx_msg void OnSelchangeCombostartSlope();
	afx_msg void OnSelchangeComboStopSlope();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditWaterVelocity();
	afx_msg void OnChangeEditmaterialVelocity();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonWaterVelocity();
	afx_msg void OnChangeEditMaterialDensity();
	afx_msg void OnCheckCollect1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSPTIMSPAGE_H__8DB21200_AC1A_41AC_A7F3_D784D021AA08__INCLUDED_)
