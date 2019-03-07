#include "stdafx.h"
#include "USLScanner.h"
#include "CPhasedArraySheet.h"


IMPLEMENT_DYNAMIC(CPhasedArraySheet, CResizableSheet)

CPhasedArraySheet::CPhasedArraySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CResizableSheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPhasedArraySheet::CPhasedArraySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage, CWnd** pDlg)
	: CResizableSheet(pszCaption, pParentWnd, iSelectPage)
{
	m_pDlg = pDlg;

	m_pPhasedArrayFocalLawsPage = NULL;
	m_pPhasedArrayProbePage = NULL;
	m_pAOSPhasedArrayUTPage = NULL;

	m_pPhasedArrayFocalLawsPage = new CPhasedArrayFocalLawsDlg();
	AddPage(m_pPhasedArrayFocalLawsPage);

	m_pPhasedArrayProbePage = new CPhasedArrayProbeDlg();
	AddPage(m_pPhasedArrayProbePage);

	m_pAOSPhasedArrayUTPage = new CAOSPhasedArrayUTPage();
	AddPage(m_pAOSPhasedArrayUTPage);

}



CPhasedArraySheet::~CPhasedArraySheet()
{
	SAFE_DELETE(m_pPhasedArrayFocalLawsPage);
	SAFE_DELETE(m_pPhasedArrayProbePage);
	SAFE_DELETE(m_pAOSPhasedArrayUTPage);
}


void CPhasedArraySheet::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CResizableSheet::PostNcDestroy();
	delete this;

}
BEGIN_MESSAGE_MAP(CPhasedArraySheet, CResizableSheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void CPhasedArraySheet::OnClose()
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CPhasedArrayProbeDlg::IDD % MAXIMUMWINDOWSIDNUMBER] = FALSE;

	CResizableSheet::OnClose();
}


void CPhasedArraySheet::OnDestroy()
{
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[CPhasedArrayProbeDlg::IDD % MAXIMUMWINDOWSIDNUMBER]);

	CResizableSheet::OnDestroy();

	*m_pDlg = NULL;
}


BOOL CPhasedArraySheet::OnInitDialog()
{
	BOOL bResult = CResizableSheet::OnInitDialog();

	CRect rc;
	GetWindowRect(&rc);
	SetMinTrackSize(CSize(GetMinWidth(), rc.Height()));
	EnableSaveRestore(_T("PhasedArraySheet"), TRUE, TRUE);

	return bResult;
}


void CPhasedArraySheet::UpdateAllPages()
{
	if(m_pPhasedArrayFocalLawsPage) m_pPhasedArrayFocalLawsPage->UpdateAllControls();
	if(m_pPhasedArrayProbePage) m_pPhasedArrayProbePage->UpdateAllControls();
	if(m_pAOSPhasedArrayUTPage) m_pAOSPhasedArrayUTPage->UpdateAllControls();

}

void CPhasedArraySheet::UpdateActivePage()
{
	CPropertyPage *pPage = GetActivePage();
	if (m_pPhasedArrayFocalLawsPage && pPage == m_pPhasedArrayFocalLawsPage) m_pPhasedArrayFocalLawsPage->UpdateAllControls();
	if (m_pPhasedArrayProbePage && pPage == m_pPhasedArrayProbePage) m_pPhasedArrayProbePage->UpdateAllControls();
	if (m_pAOSPhasedArrayUTPage && pPage == m_pAOSPhasedArrayUTPage) m_pAOSPhasedArrayUTPage->UpdateAllControls();
}