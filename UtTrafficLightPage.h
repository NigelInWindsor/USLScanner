#if !defined(AFX_UTTRAFFICLIGHTPAGE_H__490CFCEC_8E3B_4AA1_9A47_4A8491DC9E2D__INCLUDED_)
#define AFX_UTTRAFFICLIGHTPAGE_H__490CFCEC_8E3B_4AA1_9A47_4A8491DC9E2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UtTrafficLightPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUtTrafficLightPage dialog

class CUtTrafficLightPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CUtTrafficLightPage)

// Construction
public:
	void	RenderAmplitude();
	void	BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor);
	void	UpdateAllControls();
	
	CRect	m_rrClient;
	LOGFONT	m_LogFont;
	CFont*	m_pFont;
	CString m_strAmplitude;
	COLORREF	*m_pArray;
	CUtTrafficLightPage();
	~CUtTrafficLightPage();

// Dialog Data
	//{{AFX_DATA(CUtTrafficLightPage)
	enum { IDD = IDD_UT_TRAFFIC_LIGHT_PAGE };
	CComboBox	m_comboGate;
	CComboBox	m_comboTimeSlot;
	CStatic	m_staticAmplitude;
	CStatic	m_staticLessThan;
	CStatic	m_staticGreater;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CUtTrafficLightPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CUtTrafficLightPage)
	afx_msg void OnSelchangeComboGate();
	afx_msg void OnSelchangeComboTimeslot();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UTTRAFFICLIGHTPAGE_H__490CFCEC_8E3B_4AA1_9A47_4A8491DC9E2D__INCLUDED_)
