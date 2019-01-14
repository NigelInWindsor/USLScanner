// ProbeToolsSheet.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ProbeToolsSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProbeToolsSheet

IMPLEMENT_DYNAMIC(CProbeToolsSheet, CPropertySheet)

inline void CProbeToolsSheet::Construct(CWnd** pDlg,CProbe* pProbe)
{
	

	m_pDlg = pDlg;
	m_pProbe = pProbe;
	
	m_ProbeBeamSectionsPage = NULL;
	m_pProbeAxialPlotPage = NULL;

	m_ProbeBeamSectionsPage = new CProbeBeamSectionsPage(m_pProbe);
	AddPage(m_ProbeBeamSectionsPage);

	m_pProbeAxialPlotPage = new CProbeAxialPlotPage(m_pProbe);
	AddPage(m_pProbeAxialPlotPage);

}

CProbeToolsSheet::CProbeToolsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg,CProbe* pProbe)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg,pProbe);
}

CProbeToolsSheet::CProbeToolsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg,CProbe* pProbe)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg,pProbe);
}

CProbeToolsSheet::~CProbeToolsSheet()
{
	SAFE_DELETE(m_ProbeBeamSectionsPage);
	SAFE_DELETE(m_pProbeAxialPlotPage);

}


BEGIN_MESSAGE_MAP(CProbeToolsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CProbeToolsSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProbeToolsSheet message handlers

void CProbeToolsSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::PostNcDestroy();
	delete this;
}
void CProbeToolsSheet::OnClose() 
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_PROBE_TOOLS_SHEET]=FALSE;
	
	CPropertySheet::OnClose();
}

void CProbeToolsSheet::OnDestroy() 
{

	


	//Store Window Position
	if (IDD_PROBE_TOOLS_SHEET< MAXIMUMWINDOWSIDNUMBER)
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_PROBE_TOOLS_SHEET]);

	CPropertySheet::OnDestroy();
	
	*m_pDlg	= NULL;	
}


BOOL CProbeToolsSheet::OnInitDialog() 
{
	
	BOOL bResult = CPropertySheet::OnInitDialog();
	

	if (IDD_PROBE_TOOLS_SHEET< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_PROBE_TOOLS_SHEET];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE); //SWP_NOSIZE);
	}


	return bResult;
}

void CProbeToolsSheet::UpdateAllPages()
{

	m_ProbeBeamSectionsPage->UpdateAllControls();
	m_pProbeAxialPlotPage->UpdateAllControls();


}