#include "stdafx.h"
#include "resource.h"
#include "MFCNormalizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMFCNormalizeDlg, CDialogEx)
BEGIN_MESSAGE_MAP(CMFCNormalizeDlg, CDialogEx)
END_MESSAGE_MAP()

CMFCNormalizeDlg::CMFCNormalizeDlg(CWnd* pParent)
	: CDialogEx(CMFCNormalizeDlg::IDD, pParent)
{
}

CMFCNormalizeDlg::~CMFCNormalizeDlg()
{
}

/**********************************************************************/
IMPLEMENT_DYNAMIC(CDialogPane, CDockablePane)
BEGIN_MESSAGE_MAP(CDialogPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

CDialogPane::CDialogPane() {}
CDialogPane::~CDialogPane() {}

int CDialogPane::OnCreate(LPCREATESTRUCT lp)
{
	if (CDockablePane::OnCreate(lp) == -1)
		return -1;
	if (!m_wndDlg.Create(CMFCNormalizeDlg::IDD, this))
		return -1;
	m_wndDlg.ShowWindow(SW_SHOWDEFAULT);
	return  0;
}
void CDialogPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	m_wndDlg.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER);
}


