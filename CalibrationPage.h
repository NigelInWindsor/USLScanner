#if !defined(AFX_CALIBRATIONPAGE_H__CB056E8E_6352_46AD_9F04_9C5DFC808DC1__INCLUDED_)
#define AFX_CALIBRATIONPAGE_H__CB056E8E_6352_46AD_9F04_9C5DFC808DC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalibrationPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalibrationPage dialog

class CCalibrationPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCalibrationPage)

// Construction
public:
	void UpdateAllControls();
	
	CCalibrationPage();
	~CCalibrationPage();

// Dialog Data
	//{{AFX_DATA(CCalibrationPage)
	enum { IDD = IDD_CALIBRATION_DIALOG };
	CSpinButtonCtrl	m_spinThicknessReCalibration;
	CEdit	m_editThicknessReCalibrationPeriod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCalibrationPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCalibrationPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditThickRecalPeriod();
	afx_msg void OnDeltaposSpinThicknessRecalibration(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIBRATIONPAGE_H__CB056E8E_6352_46AD_9F04_9C5DFC808DC1__INCLUDED_)
