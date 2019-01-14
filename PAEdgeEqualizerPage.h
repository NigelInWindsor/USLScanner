#if !defined(AFX_PAEDGEEQUALIZERPAGE_H__8F2D065F_607E_4C6A_ABC7_AC4C9BEA4F50__INCLUDED_)
#define AFX_PAEDGEEQUALIZERPAGE_H__8F2D065F_607E_4C6A_ABC7_AC4C9BEA4F50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PAEdgeEqualizerPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPAEdgeEqualizerPage dialog

class CPAEdgeEqualizerPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPAEdgeEqualizerPage)

// Construction
public:
	CWnd* m_pViewSheet;
	
	void UpdateAllControls();
	CPAEdgeEqualizerPage(CWnd* pViewSheet = NULL);
	~CPAEdgeEqualizerPage();

// Dialog Data
	//{{AFX_DATA(CPAEdgeEqualizerPage)
	enum { IDD = IDD_PA_EDGE_EQUALIZER };
	CSliderCtrl	m_sliderLatRot;
	CSliderCtrl	m_sliderDirection;
	CSliderCtrl	m_sliderBeam3;
	CSliderCtrl	m_sliderBeam2;
	CSliderCtrl	m_sliderBeam1;
	CSliderCtrl	m_sliderBeam0;
	CEdit	m_editBeam4;
	CEdit	m_editBeam3;
	CEdit	m_editBeam2;
	CEdit	m_editBeam1;
	CEdit	m_editBeam0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPAEdgeEqualizerPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPAEdgeEqualizerPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditBeam1();
	afx_msg void OnChangeEditBeam2();
	afx_msg void OnChangeEditBeam3();
	afx_msg void OnChangeEditBeam4();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAEDGEEQUALIZERPAGE_H__8F2D065F_607E_4C6A_ABC7_AC4C9BEA4F50__INCLUDED_)
