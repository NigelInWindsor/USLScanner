#if !defined(AFX_PR15SELFTESTDLG_H__9E70C913_45AA_471F_A1BB_55EB56405D4F__INCLUDED_)
#define AFX_PR15SELFTESTDLG_H__9E70C913_45AA_471F_A1BB_55EB56405D4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pr15SelfTestDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelfTestDlg dialog

struct	ResultsData	{
	int	nStructSize;
	double	dFrequency;
	int	nAmplitude[2];
};
class CSelfTestPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSelfTestPage)

// Construction
public:
	int	m_nNumberSamples;
	ResultsData* m_pResults;
	Pr30Data* m_pPr30;
	Pr15Data* m_pPr15;
	Pl15Data* m_pPl15;
	double m_dAmplitude;
	double m_dFrequency;
	bool m_bDialogCreated;
	CSelfTestPage();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelfTestDlg)
	enum { IDD = IDD_SELF_CALIBRATION_DIALOG };
	CStatic	m_staticView;
	CEdit	m_editNomAttenuation;
	CEdit	m_editMainGain;
	CEdit	m_editStepFrequency;
	CEdit	m_editMinFrequency;
	CEdit	m_editMaxFrequency;
	CSpinButtonCtrl	m_spinFrequency;
	CSpinButtonCtrl	m_spinAmplitude;
	CEdit	m_editFrequency;
	CEdit	m_editAmplitude;
	CComboBox	m_comboOffOn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelfTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelfTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditAmplitude();
	afx_msg void OnChangeEditFrequency();
	afx_msg void OnSelchangeComboOffOn();
	afx_msg void OnButtonImplement();
	afx_msg void OnChangeEditMainGain();
	afx_msg void OnChangeEditMaxFrequency();
	afx_msg void OnChangeEditMinFrequency();
	afx_msg void OnChangeEditNomAttenuation();
	afx_msg void OnChangeEditStepFrequency();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PR15SELFTESTDLG_H__9E70C913_45AA_471F_A1BB_55EB56405D4F__INCLUDED_)
