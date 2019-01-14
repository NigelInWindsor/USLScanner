#include "stdafx.h"
#include "USLScanner.h"
#include "USLDockablePane.h"


CUSLDockCreate::CUSLDockCreate() {};
CUSLDockCreate::~CUSLDockCreate() {};

CUSLDockCreate::CUSLDockCreate(int nId, int nIDHighBitMap, bool bCombo, bool bList, BOOL bHasGripper) {
	m_nID = nId;
	m_nIDHighBitMap = nIDHighBitMap;
	m_bCombo = bCombo;
	m_bList = bList;
	m_bHasGripper = bHasGripper;
}

// CUSLDockablePane

IMPLEMENT_DYNAMIC(CUSLDockablePane, CDockablePane)

CUSLDockablePane::CUSLDockablePane()
{
	m_nID = 0;
	m_nComboHeight = 0;
	m_nIDHighBitMap = 0;
	m_bCombo = false;
	m_bList = false;
	m_nIDDlg = 0;
	m_pwndDlg = NULL;

}

CUSLDockablePane::~CUSLDockablePane()
{
}


BEGIN_MESSAGE_MAP(CUSLDockablePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()



// CUSLDockablePane message handlers



BOOL CUSLDockablePane::Create(LPCTSTR lpszCaption, CWnd* pParentWnd, const RECT& rect, CUSLDockCreate* pCreate, DWORD dwStyle, DWORD dwTabbedStyle, DWORD dwControlBarStyle, CCreateContext* pContext)
{
	m_nID = pCreate->m_nID;

	m_nIDHighBitMap = pCreate->m_nIDHighBitMap;
	m_bCombo = pCreate->m_bCombo;
	m_bList = pCreate->m_bList;

	return CDockablePane::Create(lpszCaption, pParentWnd, rect, pCreate->m_bHasGripper, pCreate->m_nID, dwStyle);
}


int CUSLDockablePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (m_bCombo == true) {
		if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, ID_COMBO_PROPERTY))
		{
			TRACE0("Failed to create Properties Combo \n");
			return -1;      // fail to create
		}

	}

	if (m_bList) {
		if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
		{
			TRACE0("Failed to create Properties Grid \n");
			return -1;      // fail to create
		}

		InitPropList();
	}
	/*
	if (m_nIDDlg) {
	switch (m_nIDDlg) {
	case CAccessLevelsDlg::IDD:	m_pwndDlg = new CAccessLevelsDlg();
	break;
	}
	if (!m_pwndDlg->Create(m_nIDDlg, this)) {
	AfxMessageBox(_T("Failed to create Dlg"));
	return -1;
	}
	}
	*/
	if (m_nIDHighBitMap) {
		//		m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
		//		m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* Is locked */);

		m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, m_nIDHighBitMap);
		m_wndToolBar.LoadToolBar(m_nIDHighBitMap, 0, 0, TRUE /* Is locked */);
		m_wndToolBar.CleanUpLockedImages();
		m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? m_nIDHighBitMap : IDR_PROPERTIES, 0, 0, TRUE /* Locked */);

		m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
		m_wndToolBar.SetOwner(this);

		// All commands will be routed via this control , not via the parent frame:
		m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
	}

	CreateFont();
	AdjustLayout();

	return 0;
}


void CUSLDockablePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	AdjustLayout();
}

void CUSLDockablePane::AdjustLayout()
{
	int cyTlb = 0;
	int	nY;

	if (GetSafeHwnd() == NULL)
	{
		return;
	}


	CRect rectClient;
	GetClientRect(rectClient);
	nY = rectClient.top;

	if (m_wndToolBar.GetSafeHwnd()) {
		cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
	}

	if (m_wndObjectCombo.GetSafeHwnd()) {
		CRect rectCombo;
		m_wndObjectCombo.GetClientRect(&rectCombo);
		m_nComboHeight = rectCombo.Height() - 1;
		m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, nY, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
		nY = rectClient.top + m_nComboHeight;
	}
	if (m_wndToolBar.GetSafeHwnd()) {
		m_wndToolBar.SetWindowPos(NULL, rectClient.left, nY, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
		nY += cyTlb;
	}
	if (m_wndPropList.GetSafeHwnd()) {
		m_wndPropList.SetWindowPos(NULL, rectClient.left, nY, rectClient.Width(), rectClient.Height() - (m_nComboHeight + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
	}
	if (m_pwndDlg && m_pwndDlg->GetSafeHwnd()) {
		m_pwndDlg->SetWindowPos(NULL, rectClient.left, nY, rectClient.Width(), rectClient.Height() - (m_nComboHeight + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
	}
}


int CUSLDockablePane::InitPropList()
{
	m_wndPropList.RemoveAll();
	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	SetAccessibility();

	return 0;
}

void CUSLDockablePane::SetAccessibility()
{
	int nCount = 0;
	int nID = this->GetDlgCtrlID();

	bool bEnable = theApp.IsAccessible(nID);
	bEnable = true;

	if (m_wndPropList.GetSafeHwnd()) {
		if (bEnable == false) {
			m_wndPropList.SetCustomColors(RGB(220, 220, 220), ((COLORREF)-1), ((COLORREF)-1), ((COLORREF)-1), ((COLORREF)-1), ((COLORREF)-1), ((COLORREF)-1));
		}
		else {
			m_wndPropList.SetCustomColors(((COLORREF)-1), ((COLORREF)-1), ((COLORREF)-1), ((COLORREF)-1), ((COLORREF)-1), ((COLORREF)-1), ((COLORREF)-1));
		}

		nCount = m_wndPropList.GetPropertyCount();

		for (int ii = 0; ii < nCount; ii++) {
			CMFCPropertyGridProperty* pProp = m_wndPropList.GetProperty(ii);
			if (pProp) {
				pProp->Enable(bEnable);
			}
		}
		m_wndPropList.RedrawWindow();
	}
}

void CUSLDockablePane::CreateFont()
{

	LOGFONT lf;
	::DeleteObject(m_Font.Detach());

	if (theApp.m_LastSettings.bUseAlernativeToSystemFont == false) {
		afxGlobalData.fontRegular.GetLogFont(&lf);

		NONCLIENTMETRICS info;
		info.cbSize = sizeof(info);

		afxGlobalData.GetNonClientMetrics(info);

		lf.lfHeight = info.lfMenuFont.lfHeight;
		lf.lfWeight = info.lfMenuFont.lfWeight;
		lf.lfItalic = info.lfMenuFont.lfItalic;
	}
	else {
		lf = theApp.m_LastSettings.lfMenus;
	}
	lf.lfHeight = MulDiv(lf.lfHeight, theApp.nPercentageTextSize[theApp.m_LastSettings.nTextPercentageSize], 100);
	m_Font.CreateFontIndirect(&lf);


	if (m_wndPropList.GetSafeHwnd()) m_wndPropList.SetFont(&m_Font);
	if (m_wndObjectCombo.GetSafeHwnd()) m_wndObjectCombo.SetFont(&m_Font);
	if (m_wndPropList.GetSafeHwnd()) m_wndPropList.SetFont(&m_Font);
	if (m_pwndDlg->GetSafeHwnd()) m_pwndDlg->SetFont(&m_Font);
}

void CUSLDockablePane::ChangeFontSize()
{
	CreateFont();
	AdjustLayout();
}

void CUSLDockablePane::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDockablePane::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
}
