#if !defined(AFX_PROBETOOLSSHEET_H__83BCB3F1_90EA_410F_863B_E4BF5E0169CC__INCLUDED_)
#define AFX_PROBETOOLSSHEET_H__83BCB3F1_90EA_410F_863B_E4BF5E0169CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProbeToolsSheet.h : header file
//
#include "ProbeBeamSectionsPage.h"
#include "ProbeAxialPlotPage.h"

/////////////////////////////////////////////////////////////////////////////
// CProbeToolsSheet

class CProbeToolsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CProbeToolsSheet)

// Construction
public:
	CProbeAxialPlotPage* m_pProbeAxialPlotPage;
	CProbeBeamSectionsPage* m_ProbeBeamSectionsPage;
	CWnd** m_pDlg;
	
	CProbe* m_pProbe;
	void Construct(CWnd** pDlg,CProbe* pProbe);
	CProbeToolsSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL,CProbe* pProbe = NULL);
	CProbeToolsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** pDlg = NULL,CProbe* pProbe = NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProbeToolsSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateAllPages();
	virtual ~CProbeToolsSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CProbeToolsSheet)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROBETOOLSSHEET_H__83BCB3F1_90EA_410F_863B_E4BF5E0169CC__INCLUDED_)
