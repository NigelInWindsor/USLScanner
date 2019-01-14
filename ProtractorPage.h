#if !defined(AFX_PROTRACTORPAGE_H__B7D94F20_521C_4DED_953E_A6AB291290CE__INCLUDED_)
#define AFX_PROTRACTORPAGE_H__B7D94F20_521C_4DED_953E_A6AB291290CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProtractorPage.h : header file
//

#include "Serial.h"

/////////////////////////////////////////////////////////////////////////////
// CProtractorPage dialog

class CProtractorPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CProtractorPage)

// Construction
public:
	CString m_strAngle;
	COLORREF	*m_pArray;
	CFont* m_pFont;
	CRect m_rrClient;
	LOGFONT m_LogFont;
	
	CSerial m_Serial;
	CProtractorPage();
	~CProtractorPage();

// Dialog Data
	//{{AFX_DATA(CProtractorPage)
	enum { IDD = IDD_PROTRACTOR_PAGE };
	CComboBox	m_comboPortNumber;
	CStatic	m_staticView;
	CEdit	m_editAngle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CProtractorPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CProtractorPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeComboPortNumber();
	afx_msg void OnChangeEditAngle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROTRACTORPAGE_H__B7D94F20_521C_4DED_953E_A6AB291290CE__INCLUDED_)
