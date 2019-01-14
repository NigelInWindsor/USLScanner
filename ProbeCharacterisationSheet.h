#if !defined(AFX_PROBECHARACTERISATIONSHEET_H__E9440B25_0A16_4092_9ED2_DC9B436C6F2A__INCLUDED_)
#define AFX_PROBECHARACTERISATIONSHEET_H__E9440B25_0A16_4092_9ED2_DC9B436C6F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProbeCharacterisationSheet.h : header file
//
#include "ResizableSheet.h"
#include "ProbeSpecificationPage.h"
#include "ProbeDataSheetPage.h"
#include "ProbeBeamProfilePage.h"
#include "ProbeToolsSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CProbeCharacterisationSheet

class CProbeCharacterisationSheet : public CResizableSheet
{
	DECLARE_DYNAMIC(CProbeCharacterisationSheet)

// Construction
public:

	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	CProbeBeamProfilePage* m_pProbeBeamProfilePage[256];
	CProbeDataSheetPage* m_pProbeDataSheetPage[256];
	CProbeSpecificationPage* m_pProbeSpecificationPage;
	CProbeToolsSheet* m_pProbeToolsSheet;
	CPropertyPage* m_pCurrentPage;

	CProbe m_Probe;

	CWnd** m_pDlg;
	
	void Construct(CWnd** pDlg);
	CToolBar    m_wndToolsToolBar;
	CReBar		m_wndRebar;
	CMenu* m_pMenu;
	CProbeCharacterisationSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);
	CProbeCharacterisationSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProbeCharacterisationSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetWindowTitle();
	void UpdateAllPages();
	void SetTabNames();
	void RemoveAllPages(bool bIncludingSpecification);
	void AddAllPages(bool bIncludingSpecification);
	void SetToolBarCheckedState();
	virtual ~CProbeCharacterisationSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CProbeCharacterisationSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveas();
	afx_msg void OnDatasheetsAdd();
	afx_msg void OnBeamprofilesAdd();
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonLock();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonTools();
	afx_msg void OnButtonExportClipboard();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROBECHARACTERISATIONSHEET_H__E9440B25_0A16_4092_9ED2_DC9B436C6F2A__INCLUDED_)
