// CustomizeSheet.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "CustomizeSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomizeSheet

IMPLEMENT_DYNAMIC(CCustomizeSheet, CPropertySheet)

inline void CCustomizeSheet::Construct(CWnd** pDlg)
{
	m_pDlg = pDlg;
	
	m_pMachineDescriptionDlg = NULL;

	m_pMachineDescriptionDlg = new CMachineDescriptionDlg();
	AddPage(m_pMachineDescriptionDlg);

	SetActivePage(theApp.m_LastSettings.nLastCustomizeTab);

}

CCustomizeSheet::CCustomizeSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CCustomizeSheet::CCustomizeSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** pDlg)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Construct(pDlg);
}

CCustomizeSheet::~CCustomizeSheet()
{
	if(m_pMachineDescriptionDlg) RemovePage(m_pMachineDescriptionDlg);
	SAFE_DELETE(m_pMachineDescriptionDlg);
}


BEGIN_MESSAGE_MAP(CCustomizeSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CCustomizeSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomizeSheet message handlers

void CCustomizeSheet::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertySheet::PostNcDestroy();
	delete this;
}
void CCustomizeSheet::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_CUSTOMIZE_SHEET % MAXIMUMWINDOWSIDNUMBER]=FALSE;
	
	CPropertySheet::OnClose();
}

void CCustomizeSheet::OnDestroy() 
{
	//Store Window Position
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_CUSTOMIZE_SHEET % MAXIMUMWINDOWSIDNUMBER]);

	CPropertySheet::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CCustomizeSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	

	CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_CUSTOMIZE_SHEET % MAXIMUMWINDOWSIDNUMBER];
	if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
		theApp.ConfineToScreen(&rect);
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}


	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);

	return bResult;

}
