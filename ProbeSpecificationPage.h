#if !defined(AFX_PROBESPECIFICATIONPAGE_H__63FE6219_61CA_441D_A9AB_D1801E3418BD__INCLUDED_)
#define AFX_PROBESPECIFICATIONPAGE_H__63FE6219_61CA_441D_A9AB_D1801E3418BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProbeSpecificationPage.h : header file
//
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CProbeSpecificationPage dialog

class CProbeSpecificationPage : public CResizablePage
{
	DECLARE_DYNCREATE(CProbeSpecificationPage)

// Construction
public:
	
	void UpdateAllControls();
	CProbe* m_pProbe;
	CProbeSpecificationPage(CProbe* pProbe = NULL);
	~CProbeSpecificationPage();

// Dialog Data
	//{{AFX_DATA(CProbeSpecificationPage)
	enum { IDD = IDD_PROBE_SPECIFICATION_PAGE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CProbeSpecificationPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CProbeSpecificationPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit2();
	afx_msg void OnChangeEdit3();
	afx_msg void OnChangeEdit4();
	afx_msg void OnChangeEdit5();
	afx_msg void OnChangeEdit6();
	afx_msg void OnChangeEdit7();
	afx_msg void OnChangeEdit8();
	afx_msg void OnChangeEdit9();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROBESPECIFICATIONPAGE_H__63FE6219_61CA_441D_A9AB_D1801E3418BD__INCLUDED_)
