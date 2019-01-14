#if !defined(AFX_VERTICALBORESCANNERPAGE_H__016FE3F2_C7DB_4870_B95D_97248EDB62EB__INCLUDED_)
#define AFX_VERTICALBORESCANNERPAGE_H__016FE3F2_C7DB_4870_B95D_97248EDB62EB__INCLUDED_

#include "Coord.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VerticalBoreScannerPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVerticalBoreScannerPage dialog

class CVerticalBoreScannerPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CVerticalBoreScannerPage)

// Construction
public:
	int m_nOldThreadAction;
	int m_nOldScanLine;
	CTime m_StartTime;
	void GenerateScanLines();
	void SetToolBarCheckedState();
	CCoord m_OldCpSurface;
	
	void UpdateAllControls();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CToolBar   m_wndToolBar;
	CReBar     m_wndRebar;
	CVerticalBoreScannerPage();
	~CVerticalBoreScannerPage();

// Dialog Data
	//{{AFX_DATA(CVerticalBoreScannerPage)
	enum { IDD = IDD_VERTICAL_BORE_SCANNER_PAGE };
	CEdit	m_editTotalDefectArea;
	CEdit	m_editSentence;
	CEdit	m_editRefZone;
	CEdit	m_editEvalZone;
	CEdit	m_editDBRange;
	CEdit	m_editScanLine;
	CEdit	m_editResumeLine;
	CButton	m_buttonStart;
	CSpinButtonCtrl	m_spinDiameter;
	CEdit	m_editDiameter;
	CEdit	m_editAngle;
	CEdit	m_editFastIncrement;
	CEdit	m_editFinishHeight;
	CEdit	m_editHeight;
	CEdit	m_editSlowIncrement;
	CEdit	m_editStartHeight;
	CSpinButtonCtrl	m_spinFastIncrement;
	CSpinButtonCtrl	m_spinSlowIncrement;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CVerticalBoreScannerPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CVerticalBoreScannerPage)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChangeEditSlowIncrement();
	afx_msg void OnChangeEditFastIncrement();
	afx_msg void OnChangeEditStartHeight();
	afx_msg void OnChangeEditFinishHeight();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDeltaposSpinSlowIncrement(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinFastIncrement(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonTurntableUp();
	afx_msg void OnButtonTurntableDown();
	afx_msg void OnButtonProbesUp();
	afx_msg void OnButtonFinishHeight();
	afx_msg void OnButtonStartHeight();
	afx_msg void OnButtonProbesToStartHeight();
	afx_msg void OnButtonProbesToFinishHeight();
	afx_msg void OnChangeEditDiameter();
	afx_msg void OnDeltaposSpinDiameter(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonStart();
	afx_msg void OnChangeEditResumeLine();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveas();
	afx_msg void OnButtonZeroRotate();
	afx_msg void OnChangeEditDbRange();
	afx_msg void OnChangeEditEvalZone();
	afx_msg void OnChangeEditRefRange();
	afx_msg void OnButtonDbRange();
	afx_msg void OnButtonEvalZone();
	afx_msg void OnButtonRefZone();
	afx_msg void OnButtonSentence();
	afx_msg void OnChangeEditTotalDefectArea();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERTICALBORESCANNERPAGE_H__016FE3F2_C7DB_4870_B95D_97248EDB62EB__INCLUDED_)
