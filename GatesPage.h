#include "afxwin.h"
#if !defined(AFX_GATESPAGE_H__C686E1DF_8CDC_4176_B0DF_A29EE153BF1C__INCLUDED_)
#define AFX_GATESPAGE_H__C686E1DF_8CDC_4176_B0DF_A29EE153BF1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GatesPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGatesPage dialog

class CGatesPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CGatesPage)

// Construction
public:
	void CopyGates();
	void SetGateTimeSlot(int nGate, int nTimeSlot);
	void SetAccessPrivelages();
	int m_nTOF;
	void OnCheckTof() ;
	void OnCheckPhase() ;
	void OnCheckGate0() ;
	void OnCheckGate1() ;
	void OnCheckGate2() ;
	void OnCheckGate3() ;
	void OnCheckGate4() ;
	void OnCheckGate5() ;
	void OnCheckGate6() ;
	void OnCheckGate7() ;
	void BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor);
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	BOOL OnSetActive();
	CString	m_Units[6];
	int m_nAccel[6];
	int m_nTimeSlot;
	int m_nGate;
	
	void UpdateAllControls();
	void EnableDisablePrePostTrigger();
	CGatesPage();
	~CGatesPage();

// Dialog Data
	//{{AFX_DATA(CGatesPage)
	enum { IDD = IDD_GATES_PAGE };
	HBRUSH m_hBr;
	CStatic	m_staticPosPhaseColor;
	CStatic	m_staticNegPhaseColor;
	CComboBox	m_comboCollectPhase;
	CComboBox	m_comboDefaultTOF;
	CEdit	m_editPeakWidth;
	CSpinButtonCtrl	m_spinPeakWidth;
	CButton	m_checkCollectToolPath;
	CButton	m_checkCollectScanLines;
	CComboBox	m_comboCollectSide;
	CButton	m_checkActualPathXYZIJKW;
	CButton	m_checkToolPathIJKW;
	CButton	m_checkCollectToolPathXYZ;
	CSpinButtonCtrl	m_spinFarDeadZone;
	CEdit	m_editFarDeadZone;
	CSpinButtonCtrl	m_spinPostTrigger;
	CSpinButtonCtrl	m_spinPreTrigger;
	CEdit	m_editPreTrigger;
	CEdit	m_editPostTrigger;
	CComboBox	m_comboCollectAScan;
	CButton	m_checkTof0;
	CButton	m_checkTof1;
	CButton	m_checkTof2;
	CButton	m_checkTof3;
	CButton	m_checkTof4;
	CButton	m_checkTof5;
	CButton	m_checkTof6;
	CButton	m_checkTof7;
	CComboBox	m_comboTOFStopGate;
	CComboBox	m_comboTOFStartGate;
	CComboBox	m_comboTOFNumber;
	CButton	m_checkTimeSlot;
	CButton	m_checkTOF;
	CButton	m_checkPhase;
	CButton	m_checkGate7;
	CButton	m_checkGate6;
	CButton	m_checkGate5;
	CButton	m_checkGate4;
	CButton	m_checkGate3;
	CButton	m_checkGate2;
	CButton	m_checkGate1;
	CButton	m_checkGate0;
	CStatic	m_staticGateColor7;
	CStatic	m_staticGateColor5;
	CStatic	m_staticGateColor6;
	CStatic	m_staticGateColor4;
	CStatic	m_staticGateColor3;
	CStatic	m_staticGateColor2;
	CStatic	m_staticGateColor1;
	CStatic	m_staticGateColor0;
	CComboBox	m_comboTOFMode;
	CStatic	m_staticGateColor;
	CEdit	m_editTimeSlotName;
	CComboBox	m_comboTimeSlot;
	CComboBox	m_comboWidthUnits;
	CComboBox	m_comboDelayUnits;
	CSpinButtonCtrl	m_spinThreshold;
	CEdit	m_editThreshold;
	CSpinButtonCtrl	m_spinGateWidth;
	CSpinButtonCtrl	m_spinGateDelay;
	CStatic	m_staticDelay;
	CStatic	m_staticWidth;
	CStatic	m_staticThreshold;
	CEdit	m_editGateWidth;
	CEdit	m_editGateDelay;
	CComboBox	m_comboGateNumber;
	CComboBox	m_comboGateMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGatesPage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGatesPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeComboWhichTimeslot();
	afx_msg void OnSelchangeComboGateNumber();
	afx_msg void OnSelchangeComboGateMode();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeComboTofMode();
	afx_msg void OnChangeEditGateDelay();
	afx_msg void OnChangeEditGateWidth();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSelchangeComboTofStopGate();
	afx_msg void OnSelchangeComboTofStartGate();
	afx_msg void OnSelchangeComboTof();
	afx_msg void OnCheckTof0();
	afx_msg void OnCheckTof1();
	afx_msg void OnCheckTof2();
	afx_msg void OnCheckTof3();
	afx_msg void OnCheckTof4();
	afx_msg void OnCheckTof5();
	afx_msg void OnCheckTof6();
	afx_msg void OnCheckTof7();
	afx_msg void OnSelchangeComboCollectAScan();
	afx_msg void OnChangeEditPreTrigger();
	afx_msg void OnChangeEditPostTrigger();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnChangeEditFarDelay();
	afx_msg void OnCheckScanLines();
	afx_msg void OnCheckToolPath();
	afx_msg void OnChangeEditPeakWidth();
	afx_msg void OnSelchangeComboDefaultTof();
	afx_msg void OnSelchangeComboCollectPhase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnCbnSelchangeComboNumberTimeslots();
	CComboBox m_comboNumberTimeSlots;
	CComboBox m_comboAverages;
	afx_msg void OnCbnSelchangeComboAverages();
	afx_msg void OnDeltaposSpinDelay(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GATESPAGE_H__C686E1DF_8CDC_4176_B0DF_A29EE153BF1C__INCLUDED_)
