#if !defined(AFX_PROBEAXIALPLOTPAGE_H__17ED0FD2_CE8C_4D57_9E86_E0A27236F4AE__INCLUDED_)
#define AFX_PROBEAXIALPLOTPAGE_H__17ED0FD2_CE8C_4D57_9E86_E0A27236F4AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProbeAxialPlotPage.h : header file
//
#include "Probe.h"

/////////////////////////////////////////////////////////////////////////////
// CProbeAxialPlotPage dialog

class CProbeAxialPlotPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CProbeAxialPlotPage)

// Construction
public:
	char* m_pRectilinearArray[2];
	int m_nImageOrientation;
	CRect m_RectilinearRect;

	void TransferScanToBeamProfile();
	void RenderScan(CPaintDC *pDC);
	void TransferRectilinearData(CPolyCoord *pLine,int nNumberLines,int nLine);
	void UpdateAllControls();
	void Start();
	
	CProbe* m_pProbe;
	CProbeAxialPlotPage(CProbe* pProbe = NULL);
	~CProbeAxialPlotPage();

// Dialog Data
	//{{AFX_DATA(CProbeAxialPlotPage)
	enum { IDD = IDD_PROBE_AXIAL_PLOT_PAGE };
	CComboBox	m_comboScanAxes2;
	CComboBox	m_comboScanAxes1;
	CStatic	m_staticView2;
	CEdit	m_editFinishPos;
	CEdit	m_editStartPos;
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CProbeAxialPlotPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CProbeAxialPlotPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonStart();
	afx_msg void OnPaint();
	afx_msg void OnChangeEditStartPos();
	afx_msg void OnChangeEditFinishPos();
	afx_msg void OnButtonEnterStartPos();
	afx_msg void OnButtonEnterFinishPos();
	afx_msg void OnButtonGoto0();
	afx_msg void OnButtonGoto1();
	afx_msg void OnSelchangeComboPlot1Scanaxes();
	afx_msg void OnSelchangeComboPlot2Scanaxes();
	afx_msg void OnButtonStart2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROBEAXIALPLOTPAGE_H__17ED0FD2_CE8C_4D57_9E86_E0A27236F4AE__INCLUDED_)
