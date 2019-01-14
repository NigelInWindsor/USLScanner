#if !defined(AFX_STEPPERSHEET_H__6DED9F95_75E1_473E_9AAC_37133DF3914C__INCLUDED_)
#define AFX_STEPPERSHEET_H__6DED9F95_75E1_473E_9AAC_37133DF3914C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StepperSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStepperSheet
#include "AxisDlg.h"
#include "Si6StatusPage.h"

class CStepperSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CStepperSheet)

// Construction
public:
	CAxisDlg* m_pAxis[16];
	CSi6StatusPage* m_pSi6Page;
	
	CWnd** m_pDlg;
	void Construct(CWnd** pDlg);
	CStepperSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);
	CStepperSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStepperSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateAllPages();
	virtual ~CStepperSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStepperSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STEPPERSHEET_H__6DED9F95_75E1_473E_9AAC_37133DF3914C__INCLUDED_)
