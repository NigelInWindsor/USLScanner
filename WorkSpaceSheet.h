#if !defined(AFX_WORKSPACESHEET_H__15564904_773E_484D_B513_3F05581CACE4__INCLUDED_)
#define AFX_WORKSPACESHEET_H__15564904_773E_484D_B513_3F05581CACE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkSpaceSheet.h : header file
//
#include "WorkSpaceDlg.h"
#include "MultipleScansPage.h"
#include "ResizableSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceSheet

class CWorkSpaceSheet : public CResizableSheet
{
	DECLARE_DYNAMIC(CWorkSpaceSheet)

// Construction
public:
	void SetWindowText();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CToolBar   m_wndToolBar;
	CReBar     m_wndRebar;
	CMenu *m_pMenu;
	CWnd** m_pDlg;
	

	CMultipleScansPage* m_pMultipleScansPage;
	CWorkSpaceDlg* m_pWorkSpacePage;

	void Construct(CWnd** pDlg);
	CWorkSpaceSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);
	CWorkSpaceSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkSpaceSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateAllPages();
	void SetToolBarCheckedState();
	virtual ~CWorkSpaceSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWorkSpaceSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileNewworkspace();
	afx_msg void OnFileOpenworkspace();
	afx_msg void OnFileSaveworkspace();
	afx_msg void OnButtonStartScan();
	afx_msg void OnButtonPauseScan();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonEraseAllFilenames();
	afx_msg void OnButtonPartition();
	afx_msg void OnButtonSerial();
	afx_msg void OnButtonSingleScan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACESHEET_H__15564904_773E_484D_B513_3F05581CACE4__INCLUDED_)
