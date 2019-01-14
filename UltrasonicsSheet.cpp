// UltrasonicsSheet.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "UltrasonicsSheet.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUltrasonicsSheet

IMPLEMENT_DYNAMIC(CUltrasonicsSheet, CPropertySheet)

CUltrasonicsSheet::CUltrasonicsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CUltrasonicsSheet::CUltrasonicsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,CWnd** DlgPtr)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	
	m_DlgPtr = DlgPtr;
	m_FlagEnableClose = TRUE;

	m_pTimeSlotPr300 = NULL;
	m_pTimeSlotPr301 = NULL;
	m_pTimeSlotPr302 = NULL;
	m_pTimeSlotPr303 = NULL;
	m_pTimeSlotPr304 = NULL;
	m_pTimeSlotPr305 = NULL;
	m_pTimeSlotPr306 = NULL;
	m_pTimeSlotPr307 = NULL;
	m_pScopeOptions = NULL;
	m_pPr20 = NULL;
	m_pPr50 = NULL;
	m_pGlobalUt = NULL;
	m_pGatesPage = NULL;
	m_pSelfTestPage = NULL;
	m_pLSAPage = NULL;
	m_pExtSourcePage = NULL;
	m_pUtTrafficLightPage = NULL;

	m_pTimeSlotPr300 = new CTimeSlotPr30(IDS_PR30_0, this, 0, true);
	m_pTimeSlotPr301 = new CTimeSlotPr30(IDS_PR30_1, this, 1, true);
	m_pTimeSlotPr302 = new CTimeSlotPr30(IDS_PR30_2, this, 2, true);
	m_pTimeSlotPr303 = new CTimeSlotPr30(IDS_PR30_3, this, 3, true);
	m_pTimeSlotPr304 = new CTimeSlotPr30(IDS_PR30_4, this, 4, true);
	m_pTimeSlotPr305 = new CTimeSlotPr30(IDS_PR30_5, this, 5, true);
	m_pTimeSlotPr306 = new CTimeSlotPr30(IDS_PR30_6, this, 6, true);
	m_pTimeSlotPr307 = new CTimeSlotPr30(IDS_PR30_7, this, 7, true);
	m_pLSAPage = new CLSAPage();

	SetNumberTimeslots(NULL,NULL);

	if(theApp.m_LastSettings.nUltrasonicsTabMask & PR20_TAB) {
		m_pPr20 = new CPr20Dlg(IDS_PR20,0,(CWnd**) this,20);
		AddPage(m_pPr20);
	}
	if(theApp.m_LastSettings.nUltrasonicsTabMask & PR50_TAB) {
		m_pPr50 = new CPr20Dlg(IDS_PR50,0,(CWnd**) this,50);
		AddPage(m_pPr50);
	}

	if(theApp.m_LastSettings.nUltrasonicsTabMask & SCOPE_TAB) {
		m_pScopeOptions = new CScopeOptionsPage();
		AddPage(m_pScopeOptions);
	}
	if(theApp.m_LastSettings.nUltrasonicsTabMask & GLOBAL_TAB) {
		m_pGlobalUt = new CGlobalUtPage(this);
		AddPage(m_pGlobalUt);
	}
	if(theApp.m_LastSettings.nUltrasonicsTabMask & GATES_TAB) {
		m_pGatesPage = new CGatesPage();
		AddPage(m_pGatesPage);
	}
	if(theApp.m_LastSettings.nUltrasonicsTabMask & SELF_TEST_TAB){
		m_pSelfTestPage = new CSelfTestDlg();
		AddPage(m_pSelfTestPage);
	}
	if(theApp.m_LastSettings.nUltrasonicsTabMask & EXT_SOURCE_TAB){
		m_pExtSourcePage = new CExtSourcePage();
		AddPage(m_pExtSourcePage);
	}
	if(theApp.m_LastSettings.nUltrasonicsTabMask & TRAFFIC_LIGHTS_TAB){
		m_pUtTrafficLightPage = new CUtTrafficLightPage();
		AddPage(m_pUtTrafficLightPage);
	}


}

CUltrasonicsSheet::~CUltrasonicsSheet()
{
	SAFE_DELETE( m_pTimeSlotPr300 );
	SAFE_DELETE( m_pTimeSlotPr301 );
	SAFE_DELETE( m_pTimeSlotPr302 );
	SAFE_DELETE( m_pTimeSlotPr303 );
	SAFE_DELETE( m_pTimeSlotPr304 );
	SAFE_DELETE( m_pTimeSlotPr305 );
	SAFE_DELETE( m_pTimeSlotPr306 );
	SAFE_DELETE( m_pTimeSlotPr307 );
	SAFE_DELETE( m_pScopeOptions );
	SAFE_DELETE( m_pPr20 );
	SAFE_DELETE( m_pPr50 );
	SAFE_DELETE( m_pGlobalUt );
	SAFE_DELETE( m_pGatesPage );
	SAFE_DELETE( m_pSelfTestPage );
	SAFE_DELETE( m_pLSAPage );
	SAFE_DELETE( m_pExtSourcePage );
	SAFE_DELETE( m_pUtTrafficLightPage );

}


BEGIN_MESSAGE_MAP(CUltrasonicsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CUltrasonicsSheet)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_MESSAGE(UPDATE_TIMESLOTS,SetNumberTimeslots)
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUltrasonicsSheet message handlers
int CUltrasonicsSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	EnableStackedTabs(FALSE);
	
	return 0;
}

void CUltrasonicsSheet::PostNcDestroy() 
{

	CPropertySheet::PostNcDestroy();
	delete this;
}
void CUltrasonicsSheet::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_ULTRASONICS_SHEET % MAXIMUMWINDOWSIDNUMBER]=FALSE;
	
	CPropertySheet::OnClose();
}

void CUltrasonicsSheet::OnDestroy() 
{
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[IDD_ULTRASONICS_SHEET % MAXIMUMWINDOWSIDNUMBER]);

	CPropertySheet::OnDestroy();
	
	*m_DlgPtr	= NULL;	
}


BOOL CUltrasonicsSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	CString Buff;

	if (IDD_ULTRASONICS_SHEET< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[IDD_ULTRASONICS_SHEET];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE); //SWP_NOSIZE);
		}
	}

	SetWindowText();
	Invalidate(TRUE);
	SetIcon( AfxGetApp()->LoadIcon(IDI_ICON1), TRUE);

	SetTabNames();

	return bResult;
}

void CUltrasonicsSheet::UpdateAllPages()
{

	if( m_pScopeOptions) m_pScopeOptions->UpdateAllControls();
	if(	m_pTimeSlotPr300 && m_pTimeSlotPr300->GetSafeHwnd()) m_pTimeSlotPr300->UpdateAllControls();
	if( m_pTimeSlotPr301 && m_pTimeSlotPr301->GetSafeHwnd()) m_pTimeSlotPr301->UpdateAllControls();
	if( m_pTimeSlotPr302 && m_pTimeSlotPr302->GetSafeHwnd()) m_pTimeSlotPr302->UpdateAllControls();
	if( m_pTimeSlotPr303 && m_pTimeSlotPr303->GetSafeHwnd()) m_pTimeSlotPr303->UpdateAllControls();
	if( m_pTimeSlotPr304 && m_pTimeSlotPr304->GetSafeHwnd()) m_pTimeSlotPr304->UpdateAllControls();
	if( m_pTimeSlotPr305 && m_pTimeSlotPr305->GetSafeHwnd()) m_pTimeSlotPr305->UpdateAllControls();
	if( m_pTimeSlotPr306 && m_pTimeSlotPr306->GetSafeHwnd()) m_pTimeSlotPr306->UpdateAllControls();
	if( m_pTimeSlotPr307 && m_pTimeSlotPr307->GetSafeHwnd()) m_pTimeSlotPr307->UpdateAllControls();


}

void CUltrasonicsSheet::SetAccessPrivelage()
{
	if(GetSafeHwnd() == NULL) return;

	if (m_pTimeSlotPr300 && m_pTimeSlotPr300->GetSafeHwnd()) m_pTimeSlotPr300->SetAccessPrivelage();
	if (m_pTimeSlotPr301 && m_pTimeSlotPr301->GetSafeHwnd()) m_pTimeSlotPr301->SetAccessPrivelage();
	if (m_pTimeSlotPr302 && m_pTimeSlotPr302->GetSafeHwnd()) m_pTimeSlotPr302->SetAccessPrivelage();
	if (m_pTimeSlotPr303 && m_pTimeSlotPr303->GetSafeHwnd()) m_pTimeSlotPr303->SetAccessPrivelage();
	if (m_pTimeSlotPr304 && m_pTimeSlotPr304->GetSafeHwnd()) m_pTimeSlotPr304->SetAccessPrivelage();
	if (m_pTimeSlotPr305 && m_pTimeSlotPr305->GetSafeHwnd()) m_pTimeSlotPr305->SetAccessPrivelage();
	if (m_pTimeSlotPr306 && m_pTimeSlotPr306->GetSafeHwnd()) m_pTimeSlotPr306->SetAccessPrivelage();
	if (m_pTimeSlotPr307 && m_pTimeSlotPr307->GetSafeHwnd()) m_pTimeSlotPr307->SetAccessPrivelage();
	if( m_pScopeOptions) m_pScopeOptions->SetAccessPrivelage();
	if( m_pGatesPage) m_pGatesPage->SetAccessPrivelages();

}

void CUltrasonicsSheet::UpdatePr15Sheet()
{
//	if( m_pTimeSlotPr150) m_pTimeSlotPr150->UpdateAllControls();

}

void CUltrasonicsSheet::SetMaxTimeSlots()
{
	RemoveTimeslots();

	if(theApp.m_UtUser.m_Global.Mux.nMaxTimeslots>0) {
		AddPage(m_pTimeSlotPr300);
	}
	if(theApp.m_UtUser.m_Global.Mux.nMaxTimeslots>1) {
		AddPage(m_pTimeSlotPr301);
	}
	if(theApp.m_UtUser.m_Global.Mux.nMaxTimeslots>2) {
		AddPage(m_pTimeSlotPr302);
	}
	if(theApp.m_UtUser.m_Global.Mux.nMaxTimeslots>3) {
		AddPage(m_pTimeSlotPr303);
	}
	if(theApp.m_UtUser.m_Global.Mux.nMaxTimeslots>4) {
		AddPage(m_pTimeSlotPr304);
	}
	if(theApp.m_UtUser.m_Global.Mux.nMaxTimeslots>5) {
		AddPage(m_pTimeSlotPr305);
	}
	if(theApp.m_UtUser.m_Global.Mux.nMaxTimeslots>6) {
		AddPage(m_pTimeSlotPr306);
	}
	if(theApp.m_UtUser.m_Global.Mux.nMaxTimeslots>7) {
		AddPage(m_pTimeSlotPr307);
	}

}

void CUltrasonicsSheet::SetWindowText()
{
	SetTitle(L"Ultrasonics:"+theApp.m_UtFileTitle);
}

void CUltrasonicsSheet::UpdateDacTable()
{
	if (m_pTimeSlotPr300 && m_pTimeSlotPr300->GetSafeHwnd()) m_pTimeSlotPr300->UpdateDacTable();
	if (m_pTimeSlotPr301 && m_pTimeSlotPr301->GetSafeHwnd()) m_pTimeSlotPr301->UpdateDacTable();
	if (m_pTimeSlotPr302 && m_pTimeSlotPr302->GetSafeHwnd()) m_pTimeSlotPr302->UpdateDacTable();
	if (m_pTimeSlotPr303 && m_pTimeSlotPr303->GetSafeHwnd()) m_pTimeSlotPr303->UpdateDacTable();
	if (m_pTimeSlotPr304 && m_pTimeSlotPr304->GetSafeHwnd()) m_pTimeSlotPr304->UpdateDacTable();
	if (m_pTimeSlotPr305 && m_pTimeSlotPr305->GetSafeHwnd()) m_pTimeSlotPr305->UpdateDacTable();
	if (m_pTimeSlotPr306 && m_pTimeSlotPr306->GetSafeHwnd()) m_pTimeSlotPr306->UpdateDacTable();
	if (m_pTimeSlotPr307 && m_pTimeSlotPr307->GetSafeHwnd()) m_pTimeSlotPr307->UpdateDacTable();
	if(	m_pLSAPage && m_pLSAPage->GetSafeHwnd()) m_pLSAPage->UpdateDacTable();

}


void CUltrasonicsSheet::SetTabNames()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	TCITEM TabCtrlItem;
	CTabCtrl* tab = GetTabControl();
	int	nn;

	if(theApp.m_LastSettings.nAttachedToPr30[0] == LSA) {
		Buff.Format(_T("PM30"));
		SetTabName(0,Buff);
		return;
	};


	for(int nTS=0;nTS<8;nTS++) {
		nn=-1;
		switch(nTS) {
		case 0:	if(m_pTimeSlotPr300) nn=GetPageIndex(m_pTimeSlotPr300);
			break;
		case 1:	if(m_pTimeSlotPr301) nn=GetPageIndex(m_pTimeSlotPr301);
			break;
		case 2:	if(m_pTimeSlotPr302) nn=GetPageIndex(m_pTimeSlotPr302);
			break;
		case 3:	if(m_pTimeSlotPr303) nn=GetPageIndex(m_pTimeSlotPr303);
			break;
		case 4:	if(m_pTimeSlotPr304) nn=GetPageIndex(m_pTimeSlotPr304);
			break;
		case 5:	if(m_pTimeSlotPr305) nn=GetPageIndex(m_pTimeSlotPr305);
			break;
		case 6:	if(m_pTimeSlotPr306) nn=GetPageIndex(m_pTimeSlotPr306);
			break;
		case 7:	if(m_pTimeSlotPr307) nn=GetPageIndex(m_pTimeSlotPr307);
			break;
		}
		if(nn>=0) {
			ZeroMemory(&TabCtrlItem,sizeof TabCtrlItem);
			tab->GetItem(nn,&TabCtrlItem);
			TabCtrlItem.pszText = theApp.m_UtUser.m_TS[nTS].cName;
			TabCtrlItem.mask=TCIF_TEXT;
			tab->SetItem(nn,&TabCtrlItem);
		}
	}
}

void CUltrasonicsSheet::SetTabName(int nTabNumber, CString cName)
{
	if(GetSafeHwnd() == NULL) return;

	TCITEM TabCtrlItem;
	CTabCtrl* tab = GetTabControl();
	WCHAR wcName[60];
	int nn=-1;

	switch(nTabNumber){
	case 0:	if (m_pTimeSlotPr300 && m_pTimeSlotPr300->GetSafeHwnd()) nn = GetPageIndex(m_pTimeSlotPr300);
		break;
	case 1:	if (m_pTimeSlotPr301 && m_pTimeSlotPr301->GetSafeHwnd()) nn = GetPageIndex(m_pTimeSlotPr301);
		break;
	case 2:	if (m_pTimeSlotPr302 && m_pTimeSlotPr302->GetSafeHwnd()) nn = GetPageIndex(m_pTimeSlotPr302);
		break;
	case 3:	if (m_pTimeSlotPr303 && m_pTimeSlotPr303->GetSafeHwnd()) nn = GetPageIndex(m_pTimeSlotPr303);
		break;
	case 4:	if (m_pTimeSlotPr304 && m_pTimeSlotPr304->GetSafeHwnd()) nn = GetPageIndex(m_pTimeSlotPr304);
		break;
	case 5:	if (m_pTimeSlotPr305 && m_pTimeSlotPr305->GetSafeHwnd()) nn = GetPageIndex(m_pTimeSlotPr305);
		break;
	case 6:	if (m_pTimeSlotPr306 && m_pTimeSlotPr306->GetSafeHwnd()) nn = GetPageIndex(m_pTimeSlotPr306);
		break;
	case 7:	if (m_pTimeSlotPr307 && m_pTimeSlotPr307->GetSafeHwnd()) nn = GetPageIndex(m_pTimeSlotPr307);
		break;
	}


	if(nn>=0) {
		ZeroMemory(&TabCtrlItem,sizeof TabCtrlItem);
		tab->GetItem(nn,&TabCtrlItem);
		wcsncpy_s(wcName,cName,60);
		TabCtrlItem.pszText = wcName;
		TabCtrlItem.mask=TCIF_TEXT;
		tab->SetItem(nn,&TabCtrlItem);
	}

}


HRESULT CUltrasonicsSheet::SetNumberTimeslots(WPARAM wp, LPARAM lp)
{
	if( m_pLSAPage && GetPageIndex(m_pLSAPage) >= 0) RemovePage(m_pLSAPage);

	SetMaxTimeSlots();
	if (theApp.m_LastSettings.nUltrasonicsTabMask & PHASED_ARRAY_TAB) {
		AddPage(m_pLSAPage);
	}

	SetTabNames();

	return NULL;
}


void CUltrasonicsSheet::UpdateGatesPage()
{
	if(	m_pGatesPage ) m_pGatesPage->UpdateAllControls();

}

void CUltrasonicsSheet::UpdateLSAPage()
{
	if( m_pLSAPage && m_pLSAPage->GetSafeHwnd()) m_pLSAPage->UpdateAllControls();
}

void CUltrasonicsSheet::RedrawAllPages()
{
	if(GetSafeHwnd() == NULL) return;
	if( m_pTimeSlotPr300 && m_pTimeSlotPr300->GetSafeHwnd()) m_pTimeSlotPr300->RedrawLayout();
	if( m_pTimeSlotPr301 && m_pTimeSlotPr301->GetSafeHwnd()) m_pTimeSlotPr301->RedrawLayout();
	if( m_pTimeSlotPr302 && m_pTimeSlotPr302->GetSafeHwnd()) m_pTimeSlotPr302->RedrawLayout();
	if( m_pTimeSlotPr303 && m_pTimeSlotPr303->GetSafeHwnd()) m_pTimeSlotPr303->RedrawLayout();
	if( m_pTimeSlotPr304 && m_pTimeSlotPr304->GetSafeHwnd()) m_pTimeSlotPr304->RedrawLayout();
	if( m_pTimeSlotPr305 && m_pTimeSlotPr305->GetSafeHwnd()) m_pTimeSlotPr305->RedrawLayout();
	if( m_pTimeSlotPr306 && m_pTimeSlotPr306->GetSafeHwnd()) m_pTimeSlotPr306->RedrawLayout();
	if( m_pTimeSlotPr307 && m_pTimeSlotPr307->GetSafeHwnd()) m_pTimeSlotPr307->RedrawLayout();
//	if( m_pScopeOptions) m_pScopeOptions->RedrawLayout();
//	if( m_pGatesPage) m_pGatesPage->RedrawLayout();

}

void CUltrasonicsSheet::OnRButtonDown(UINT nFlags, CPoint point)
{
	FRAME;

	pFrame->SendMessage(UI_SHOW_PROPERTIES_PANE, NULL, Tabs);

	CPropertySheet::OnRButtonDown(nFlags, point);
}

void CUltrasonicsSheet::RemoveTimeslots()
{
	if (m_pTimeSlotPr300 && GetPageIndex(m_pTimeSlotPr300) >= 0) RemovePage(m_pTimeSlotPr300);
	if (m_pTimeSlotPr301 && GetPageIndex(m_pTimeSlotPr301) >= 0) RemovePage(m_pTimeSlotPr301);
	if (m_pTimeSlotPr302 && GetPageIndex(m_pTimeSlotPr302) >= 0) RemovePage(m_pTimeSlotPr302);
	if (m_pTimeSlotPr303 && GetPageIndex(m_pTimeSlotPr303) >= 0) RemovePage(m_pTimeSlotPr303);
	if (m_pTimeSlotPr304 && GetPageIndex(m_pTimeSlotPr304) >= 0) RemovePage(m_pTimeSlotPr304);
	if (m_pTimeSlotPr305 && GetPageIndex(m_pTimeSlotPr305) >= 0) RemovePage(m_pTimeSlotPr305);
	if (m_pTimeSlotPr306 && GetPageIndex(m_pTimeSlotPr306) >= 0) RemovePage(m_pTimeSlotPr306);
	if (m_pTimeSlotPr307 && GetPageIndex(m_pTimeSlotPr307) >= 0) RemovePage(m_pTimeSlotPr307);
}