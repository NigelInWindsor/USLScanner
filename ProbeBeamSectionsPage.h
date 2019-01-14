#if !defined(AFX_PROBEBEAMSECTIONSPAGE_H__2B8BF9B0_83FE_4D2E_9C28_7B69F4DC242F__INCLUDED_)
#define AFX_PROBEBEAMSECTIONSPAGE_H__2B8BF9B0_83FE_4D2E_9C28_7B69F4DC242F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProbeBeamSectionsPage.h : header file
//
#include "Probe.h"
/////////////////////////////////////////////////////////////////////////////
// CProbeBeamSectionsPage dialog

class CProbeBeamSectionsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CProbeBeamSectionsPage)

// Construction
public:
	int FindAppropriateBeamProfile();
	int	m_nSectionNumber;
	int m_nSubTimerAction;
	int m_nTimerAction;
	void MultipleBeamProfileSections();
	void TransferScanToBeamProfile();
	char* m_pRectilinearArray;
	int m_nImageOrientation;
	CRect m_RectilinearRect;

	void FindRectilinearCentre(int *pnSample, int *pnLine);
	void TransferRectilinearData(CPolyCoord *pLine,int nNumberLines,int nLine);
	void RenderScan(CPaintDC* pDC);
	
	void UpdateAllControls();
	CProbe* m_pProbe;
	CProbeBeamSectionsPage(CProbe* pProbe = NULL);
	~CProbeBeamSectionsPage();

// Dialog Data
	//{{AFX_DATA(CProbeBeamSectionsPage)
	enum { IDD = IDD_PROBE_BEAM_SECTIONS_PAGE };
	CButton	m_checkTrackGate;
	CSpinButtonCtrl	m_spinNumberSections;
	CEdit	m_editStartPos;
	CEdit	m_editSlowScanSize;
	CEdit	m_editNumberSections;
	CEdit	m_editIncrement;
	CEdit	m_editFinishPos;
	CEdit	m_editFastScanSize;
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CProbeBeamSectionsPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CProbeBeamSectionsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditFastSize();
	afx_msg void OnChangeEditFinishPos();
	afx_msg void OnChangeEditIncrement();
	afx_msg void OnChangeEditNumberSections();
	afx_msg void OnChangeEditSlowSize();
	afx_msg void OnChangeEditStartPos();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonEnterStartPos();
	afx_msg void OnButtonEnterFinishPos();
	afx_msg void OnButtonStart();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCheckTrackGate();
	afx_msg void OnButtonGoto0();
	afx_msg void OnButtonGoto1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROBEBEAMSECTIONSPAGE_H__2B8BF9B0_83FE_4D2E_9C28_7B69F4DC242F__INCLUDED_)
