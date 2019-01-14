#if !defined(AFX_PROBEDATASHEETPAGE_H__AF2ADD69_D9F8_4D7A_BD05_5065B004611E__INCLUDED_)
#define AFX_PROBEDATASHEETPAGE_H__AF2ADD69_D9F8_4D7A_BD05_5065B004611E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProbeDataSheetPage.h : header file
//
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CProbeDataSheetPage dialog

class CProbeDataSheetPage : public CResizablePage
{
	DECLARE_DYNCREATE(CProbeDataSheetPage)

// Construction
public:
	void SetAccessPrivelage();
	void CreateColumnList();
	void FillList();
	void RenderBode(CPaintDC *pDC, int nWhich);
	void RenderScope(CPaintDC* pDC,int nWhich);
	
	void UpdateAllControls();

	CProbeDataSheet*	m_pDataSheet;
	CProbe*				m_pProbe;

	CProbeDataSheetPage(CProbe* pProbe = NULL, CProbeDataSheet* pDataSheet = NULL);
	~CProbeDataSheetPage();

// Dialog Data
	//{{AFX_DATA(CProbeDataSheetPage)
	enum { IDD = IDD_PROBE_DATA_SHEET_PAGE };
	CEdit	m_editTransducerDescription;
	CSpinButtonCtrl	m_spinAtAttenuation;
	CEdit	m_editAtAttenuation;
	CEdit	m_editCentreFrequency;
	CEdit	m_editRectifierDecay;
	CEdit	m_editRectifierMode;
	CEdit	m_editLogPostFilter;
	CEdit	m_editCutOffPoint;
	CEdit	m_editLogGain;
	CEdit	m_editLinGain;
	CButton	m_buttonGroup;
	CStatic	m_text2;
	CStatic	m_text1;
	CStatic	m_text0;
	CListCtrl	m_listDac;
	CString	m_strAmplifier;
	CString	m_strDacBlanking;
	CString	m_strDacMode;
	CString	m_strDacTriggerThreshold;
	CString	m_strDacTrigger;
	CString	m_strFilterRange;
	CString	m_strFilterType;
	CString	m_strPreampDamping;
	CString	m_strPreampGain;
	CString	m_strPreampMode;
	CString	m_strPRF;
	CString	m_strPulseVoltage;
	CString	m_strPulseWidth;
	CString	m_strReceiver;
	CString	m_strTransmitter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CProbeDataSheetPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CProbeDataSheetPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit0();
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit2();
	afx_msg void OnChangeEdit3();
	afx_msg void OnChangeEdit4();
	afx_msg void OnPaint();
	afx_msg void OnButtonLock();
	afx_msg void OnRadioPulseDuration();
	afx_msg void OnChangeEditDbDrop();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCheckPulseDuration();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROBEDATASHEETPAGE_H__AF2ADD69_D9F8_4D7A_BD05_5065B004611E__INCLUDED_)
