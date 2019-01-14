#if !defined(AFX_EDGEEQUALIZERPAGE_H__2D7EED40_EE90_4E9F_BC51_52732E619982__INCLUDED_)
#define AFX_EDGEEQUALIZERPAGE_H__2D7EED40_EE90_4E9F_BC51_52732E619982__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EdgeEqualizerPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEdgeEqualizerPage dialog

class CEdgeEqualizerPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CEdgeEqualizerPage)

// Construction
public:
	HBRUSH m_hBr;
	int	m_nEdge;
	void UpdateAllControls();
	
	CWnd* m_pViewSheet;
	CEdgeEqualizerPage(CWnd* pViewSheet=NULL);
	~CEdgeEqualizerPage();

// Dialog Data
	//{{AFX_DATA(CEdgeEqualizerPage)
	enum { IDD = IDD_EDGE_EQUALIZER_PAGE };
	CSliderCtrl	m_sliderEdge;
	CSliderCtrl	m_sliderEdge7;
	CSliderCtrl	m_sliderEdge6;
	CSliderCtrl	m_sliderEdge5;
	CSliderCtrl	m_sliderEdge4;
	CSliderCtrl	m_sliderEdge3;
	CSliderCtrl	m_sliderEdge2;
	CSliderCtrl	m_sliderEdge1;
	CSliderCtrl	m_sliderEdge0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEdgeEqualizerPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEdgeEqualizerPage)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnButtonZero();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDGEEQUALIZERPAGE_H__2D7EED40_EE90_4E9F_BC51_52732E619982__INCLUDED_)
