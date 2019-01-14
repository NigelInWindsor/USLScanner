#if !defined(AFX_INCLUSIONRULESPAGE_H__303C3EDC_3D5E_4A39_98BF_A0567D1FF455__INCLUDED_)
#define AFX_INCLUSIONRULESPAGE_H__303C3EDC_3D5E_4A39_98BF_A0567D1FF455__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InclusionRulesPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInclusionRulesPage dialog

class CInclusionRulesPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CInclusionRulesPage)

// Construction
public:
	void SetAccessPrivelages();
	
	int	m_nGlobalLocal;
	void UpdateAllControls();
	CInclusions* m_pInclusions;
	CInclusionRulesPage(CInclusions* pInclusions = NULL, int nGlobalLocal = 0);
	~CInclusionRulesPage();

// Dialog Data
	//{{AFX_DATA(CInclusionRulesPage)
	enum { IDD = IDD_INCLUSION_RULES_PAGE };
	CEdit	m_editRefStandard;
	CEdit	m_editTestSensitivity;
	CEdit	m_editTestGain;
	CEdit	m_editTestDate;
	CEdit	m_editRefSensitivity;
	CEdit	m_editRefDate;
	CEdit	m_editRefGain;
	CEdit	m_editDensity;
	CEdit	m_editShearWaveVelocity;
	CEdit	m_editMaterialVelocity;
	CEdit	m_editFBHSize;
	CEdit	m_editFocalDepth;
	CEdit	m_editFocalLength;
	CEdit	m_editProbeID;
	CEdit	m_editWeightC;
	CEdit	m_editWeightB;
	CEdit	m_editWeightA;
	CEdit	m_editGreaterC;
	CEdit	m_editGreaterB;
	CEdit	m_editGreaterA;
	CSpinButtonCtrl	m_spinYSamples;
	CSpinButtonCtrl	m_spinXSamples;
	CSpinButtonCtrl	m_spinThreshold;
	CEdit	m_editThreshold;
	CEdit	m_editYSamples;
	CEdit	m_editXSamples;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CInclusionRulesPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CInclusionRulesPage)
	afx_msg void OnChangeEditXSamples();
	afx_msg void OnChangeEditYSamples();
	afx_msg void OnChangeEditThreshold();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditGreaterA();
	afx_msg void OnChangeEditGreaterB();
	afx_msg void OnChangeEditGreaterC();
	afx_msg void OnChangeEditWeightA();
	afx_msg void OnChangeEditWeightB();
	afx_msg void OnChangeEditWeightC();
	afx_msg void OnChangeEditFbhSize();
	afx_msg void OnChangeEditFocalDepth();
	afx_msg void OnChangeEditFocalLength();
	afx_msg void OnChangeEditProbeId();
	afx_msg void OnChangeEditDensity();
	afx_msg void OnChangeEditMaterialVelocity();
	afx_msg void OnChangeEditShearwaveVelocity();
	afx_msg void OnChangeEditRefGain();
	afx_msg void OnChangeEditRefStandard();
	afx_msg void OnButtonRefResult();
	afx_msg void OnButtonTestResult();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INCLUSIONRULESPAGE_H__303C3EDC_3D5E_4A39_98BF_A0567D1FF455__INCLUDED_)
