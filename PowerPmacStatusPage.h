#include "afxwin.h"
#include "afxcmn.h"
#if !defined(AFX_POWERPMACSTATUSPAGE_H__04440A65_F600_4015_A836_940E06D3DD49__INCLUDED_)
#define AFX_POWERPMACSTATUSPAGE_H__04440A65_F600_4015_A836_940E06D3DD49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PowerPmacStatusPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPowerPmacStatusPage dialog

class CPowerPmacStatusPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPowerPmacStatusPage)

// Construction
public:
	HBRUSH m_hBr;
	float  m_fTSMove[3];
	float  m_fTAMove[3];
	float  m_fTMMove[3];
	__int64	m_n64CoordStatus[3];
	CPowerPmacStatusPage();
	~CPowerPmacStatusPage();

// Dialog Data
	//{{AFX_DATA(CPowerPmacStatusPage)
	enum { IDD = IDD_POWER_PMAC_STATUS };
	CEdit	m_editTS;
	CEdit	m_editTM;
	CEdit	m_editTA;
	CButton	m_checkProgActive3;
	CButton	m_checkProgActive2;
	CButton	m_checkProgActive1;
	CButton	m_checkTimersEnabled3;
	CButton	m_checkTimersEnabled2;
	CButton	m_checkTimersEnabled1;
	CButton	m_checkMoveTimer3;
	CButton	m_checkMoveTimer2;
	CButton	m_checkMoveTimer1;
	CButton	m_checkLookAhead3;
	CButton	m_checkLookAhead2;
	CButton	m_checkLookAhead1;
	CButton	m_checkInPos3;
	CButton	m_checkInPos2;
	CButton	m_checkInPos1;
	CButton	m_checkDesVelZero3;
	CButton	m_checkDesVelZero2;
	CButton	m_checkDesVelZero1;
	CButton	m_checkAmpEnabled3;
	CButton	m_checkAmpEnabled2;
	CButton	m_checkAmpEnabled1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPowerPmacStatusPage)
	public:
	void UpdateAllControls();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPowerPmacStatusPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CEdit m_editMoveSpeed;
	CEdit m_editNormalizationSpeed;
	CEdit m_editOrthogonalSpeed;
	CSpinButtonCtrl m_spinMoveSpeed;
	CSpinButtonCtrl m_spinNormalizationSpeed;
	CSpinButtonCtrl m_spinOrthogonalSpeed;
	afx_msg void OnEnChangeEditSpeedMove();
	afx_msg void OnEnChangeEditSpeedNormalization();
	afx_msg void OnEnChangeEditSpeedOrthogonal();
	afx_msg void OnDeltaposSpinSpeedMove(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinSpeedNormalization(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinSpeedOrthogonal(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_editAxisEnableMask1;
	CEdit m_editAxisEnableMask2;
	afx_msg void OnEnChangeEditAxisEnableMask1();
	afx_msg void OnEnChangeEditAxisEnableMask2();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POWERPMACSTATUSPAGE_H__04440A65_F600_4015_A836_940E06D3DD49__INCLUDED_)
