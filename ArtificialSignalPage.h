#if !defined(AFX_ARTIFICIALSIGNALPAGE_H__7A45EE30_4CF8_4816_9E08_9BEA6E30439B__INCLUDED_)
#define AFX_ARTIFICIALSIGNALPAGE_H__7A45EE30_4CF8_4816_9E08_9BEA6E30439B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ArtificialSignalPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CArtificialSignalPage dialog

class CArtificialSignalPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CArtificialSignalPage)

// Construction
public:
	
	void UpdateAllControls();
	CArtificialSignalPage();
	~CArtificialSignalPage();

// Dialog Data
	//{{AFX_DATA(CArtificialSignalPage)
	enum { IDD = IDD_ARTIFICIAL_SIGNAL_PAGE };
	CComboBox	m_comboWindow;
	CSpinButtonCtrl	m_spinAmplitude;
	CEdit	m_editAmplitude;
	CSpinButtonCtrl	m_spinFrequency;
	CEdit	m_editFrequency;
	CComboBox	m_comboShape;
	CComboBox	m_comboLength;
	CButton	m_checkUse;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CArtificialSignalPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CArtificialSignalPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboLength();
	afx_msg void OnSelchangeComboShape();
	afx_msg void OnCheckUseSignalGenerator();
	afx_msg void OnChangeEditFrequency();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditAmplitude();
	afx_msg void OnSelchangeCombowindow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARTIFICIALSIGNALPAGE_H__7A45EE30_4CF8_4816_9E08_9BEA6E30439B__INCLUDED_)
