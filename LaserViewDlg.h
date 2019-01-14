#if !defined(AFX_LASERVIEWDLG_H__DE2024FF_EB5F_4B76_AB8D_43688B1C3519__INCLUDED_)
#define AFX_LASERVIEWDLG_H__DE2024FF_EB5F_4B76_AB8D_43688B1C3519__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LaserViewDlg.h : header file
//
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CLaserViewDlg dialog

class CLaserViewDlg : public CDialog
{
// Construction
public:
	CLaserViewDlg(CWnd* pParent = NULL ,CWnd** pDlg = NULL,int nLeft=0, int nTop=0);   // standard constructor
	void	UpdateAllControls();

// Dialog Data
	//{{AFX_DATA(CLaserViewDlg)
	enum { IDD = IDD_LASER_VIEW_DLG };
	CSpinButtonCtrl	m_spinLineNumber;
	CEdit	m_editLineNumber;
	CComboBox	m_comboMarkerDisplay;
	CEdit	m_editMirrorDistance;
	CEdit	m_editBlockSize;
	CButton	m_checkSingleReading;
	CEdit	m_editMaxYDeviation;
	CButton	m_checkOpenLaserDoor;
	CButton	m_checkFlipDisplay;
	CButton	m_checkReverseEncoder;
	CButton	m_checkFIR;
	CEdit	m_editMinValidAmplitude;
	CEdit	m_editCallbackPercent;
	CButton	m_checkDiscard;
	CEdit	m_editJitter;
	CEdit	m_editAverageSamples;
	CEdit	m_editAverageLines;
	CEdit	m_editMotorSpeed;
	CEdit	m_editSamplesPerSecond;
	CEdit	m_editAngleRange;
	CEdit	m_editMinRange;
	CEdit	m_editMaxRange;
	CEdit	m_editOffset;
	CComboBox	m_comboView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLaserViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	CWnd*	m_pParent;
	CWnd**	m_pDlg;
	bool	m_bDlgCreated;
	int		m_nLeft;
	int		m_nTop;
	int		m_nIndex;


protected:

	// Generated message map functions
	//{{AFX_MSG(CLaserViewDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeComboView();
	afx_msg void OnChangeEditOffset();
	afx_msg void OnButtonTakeTaughtPonts();
	afx_msg void OnChangeEditMinDistance();
	afx_msg void OnChangeEditMaxDistance();
	afx_msg void OnChangeEditAngleRange();
	afx_msg void OnChangeEditSamplespersec();
	afx_msg void OnChangeEditMotorspeed();
	afx_msg void OnChangeEditAveragesamples();
	afx_msg void OnChangeEditAveragelines();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonJitter();
	afx_msg void OnChangeEditCallbackpercent();
	afx_msg void OnCheckDiscard();
	afx_msg void OnChangeEditMinAmp();
	afx_msg void OnCheckFir();
	afx_msg void OnCheckReverseLaserEncoder();
	afx_msg void OnCheckLaserFlipDisplay();
	afx_msg void OnCheckOpenLaser();
	afx_msg void OnChangeEditMaxYDeviation();
	afx_msg void OnCheckSingleReading();
	afx_msg void OnChangeEditBlockSize();
	afx_msg void OnChangeEditMirrorDistance();
	afx_msg void OnSelchangeComboMarkerDisplay();
	afx_msg void OnChangeEditLineNumber();
	afx_msg void OnDeltaposSpinLineNumber(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LASERVIEWDLG_H__DE2024FF_EB5F_4B76_AB8D_43688B1C3519__INCLUDED_)
