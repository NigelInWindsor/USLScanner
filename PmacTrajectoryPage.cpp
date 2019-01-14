// PmacTrajectoryPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "PmacTrajectoryPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPmacTrajectoryPage property page

IMPLEMENT_DYNCREATE(CPmacTrajectoryPage, CResizablePage)

CPmacTrajectoryPage::CPmacTrajectoryPage() : CResizablePage(CPmacTrajectoryPage::IDD)
{
	//{{AFX_DATA_INIT(CPmacTrajectoryPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nSide = PORTSIDE;
	m_bGotoIfHighlighted = true;
	m_bLbDown = false;
	m_fBorderPos = 0.5f;
	m_pToolPath = NULL;

	m_nRenderType = 0;
	m_nHeadSurface = 0;
	m_nTableOrder = 0;
}

CPmacTrajectoryPage::~CPmacTrajectoryPage()
{
}

void CPmacTrajectoryPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPmacTrajectoryPage)
	DDX_Control(pDX, IDC_LIST_COORDS, m_listCoord);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPmacTrajectoryPage, CResizablePage)
	//{{AFX_MSG_MAP(CPmacTrajectoryPage)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_COORDS, OnGetdispinfoListCoords)
	ON_NOTIFY(NM_CLICK, IDC_LIST_COORDS, OnClickListCoords)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_COORDS, OnRclickListCoords)
	ON_WM_TIMER()
	ON_COMMAND(ID_LIST_GOTO, OnListGoto)
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_GOTO_IF_HIGHLIGHTED, OnButtonGotoIfHighlighted)
	ON_COMMAND(ID_BUTTON_LAST_TOOLPATH, OnButtonLastToolpath)
	ON_COMMAND(ID_BUTTON_NEXT_TOOLPATH, OnButtonNextToolpath)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_BUTTON_VECTOR_PLOT, OnButtonVectorPlot)
	ON_COMMAND(ID_BUTTON_SURFACE_WAYPOINT_LINE, OnButtonSurfaceWaypointLine)
	ON_COMMAND(ID_BUTTON_HEAD_WAYPOINT_LINE, OnButtonHeadWaypointLine)
	ON_COMMAND(ID_BUTTON_LEFT_COORDINATES, OnButtonLeftCoordinates)
	ON_COMMAND(ID_BUTTON_RIGHT_COORDINATES, OnButtonRightCoordinates)
	ON_COMMAND(ID_BUTTON_REVERSE_TABLE_ORDER, OnButtonReverseTableOrder)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPmacTrajectoryPage message handlers

BOOL CPmacTrajectoryPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	
	GetClientRect(m_rrClient);

	
	CreateColumns();
	UpdateAllControls();
	
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPmacTrajectoryPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CPmacTrajectoryPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	FillList();
	m_listCoord.Invalidate(false);
	SetToolBarCheckedState();
}



void CPmacTrajectoryPage::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	CRect rrView,rrList,rrReBar;
	
	if(GetSafeHwnd()) {
		GetClientRect(&m_rrClient);

		if (m_staticView.GetSafeHwnd() && m_listCoord.GetSafeHwnd()) {
			m_staticView.GetWindowRect(&rrView);
			ScreenToClient(&rrView);
			m_listCoord.GetWindowRect(&rrList);
			ScreenToClient(&rrList);

			m_wndRebar.GetWindowRect(&rrReBar);
			ScreenToClient(&rrReBar);
			m_rrClient.top = rrReBar.bottom + 4;

			rrView = m_rrClient;
			rrView.bottom = (int)((float)m_rrClient.Height() * m_fBorderPos) - 4;
			m_staticView.SetWindowPos(NULL, rrView.left, rrView.top, rrView.Width(), rrView.Height(), NULL);

			rrList = m_rrClient;
			rrList.top = (int)((float)m_rrClient.Height() * m_fBorderPos) + 4;
			m_listCoord.SetWindowPos(NULL, rrList.left, rrList.top, rrList.Width(), rrList.Height(), NULL);

			m_rrBorder = rrList;
			m_rrBorder.top = rrView.bottom;
			m_rrBorder.bottom = rrList.top;

			Invalidate(false);
		}
	}
	
}

BOOL CPmacTrajectoryPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastPmacTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	SetTimer(1,200,NULL);

	theApp.m_Thread.m_bCollectPmacTrajectory = true;
	return CResizablePage::OnSetActive();
}

BOOL CPmacTrajectoryPage::OnKillActive() 
{
	theApp.m_Thread.m_bCollectPmacTrajectory = false;
	KillTimer(1);
	
	return CResizablePage::OnKillActive();
}


void CPmacTrajectoryPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rr;
	CDC	dcMem;
	CBitmap bmp;
	CRect rrGraph;
	CString Buff;

	LOGFONT lf;
	memset((void*)&lf,0,sizeof(lf));
	lf.lfHeight = 12;
	CFont Font;
	CFont* pOldFont;
	Font.CreateFontIndirect(&lf);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	CRect rrArea = rr;
	rrArea.OffsetRect(-rr.left,-rr.top);

	COLORREF	*pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];

	bmp.CreateCompatibleBitmap(&dc,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),pArray);
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	pOldFont = dcMem.SelectObject(&Font);
	dcMem.SetBkMode(TRANSPARENT);

	dcMem.FillRect(&rrArea,&CBrush(RGB(255,255,255)));

	Buff = "8888.8";
	CSize size = dcMem.GetTextExtent(Buff);

	switch(m_nRenderType) {
	case 0: RenderVectorPlot((CPaintDC*)&dcMem,rr);
		 RenderTipPlot((CPaintDC*)&dcMem,rr);
		break;
	case 1:
		for(int nAxis = 0;nAxis<10;nAxis++) {
			rrGraph.left = rrArea.left + size.cx;
			rrGraph.right = rrArea.right - size.cx;
			rrGraph.top = rrArea.top + MulDiv(nAxis,rrArea.Height(),9) + size.cy;
			rrGraph.bottom = rrArea.top + MulDiv(nAxis+1,rrArea.Height(),9) - size.cy;

			DrawGraticule((CPaintDC*)&dcMem,rrGraph,nAxis);
			DrawTrajectory((CPaintDC*)&dcMem,rrGraph,nAxis);

		}
		break;
	}

	dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);

	dcMem.SelectObject(pOldFont);
	dcMem.SelectObject(pOldBitmap);
	SAFE_DELETE( pArray );
}

void CPmacTrajectoryPage::DrawGraticule(CPaintDC* pDC,CRect rr,int nAxis)
{
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	CPen penGrey(PS_SOLID,1,RGB(200,200,200));
	CPen* pOldPen = pDC->SelectObject(&penBlack);

	pDC->MoveTo(rr.TopLeft());
	pDC->LineTo(rr.left,rr.bottom);
	pDC->MoveTo(rr.left,rr.CenterPoint().y);
	pDC->LineTo(rr.right,rr.CenterPoint().y);

	pDC->SelectObject(pOldPen);

}

void CPmacTrajectoryPage::DrawTrajectory(CPaintDC* pDC,CRect rr,int nAxis)
{
	if(InitializeToolPathPtr() == false) return;

	float fMin,fMax,fHeight;;
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	CPen penGrey(PS_SOLID,1,RGB(200,200,200));
	CPen* pOldPen = pDC->SelectObject(&penBlack);
	CPoint *pt = NULL;


	int nMax = m_pToolPath->m_nMaxAccessed;
	if(nMax > 0) {
		m_pToolPath->FindLimits(nAxis,&fMin,&fMax);
		fHeight = fMax - fMin;
		if(fHeight<1.0f) fHeight = 1.0f;

		pt = new CPoint[nMax + 1];
		for(int nn=0;nn<=nMax;nn++) {

			pt[nn].x = rr.left + MulDiv(nn,rr.Width(),nMax);
			pt[nn].y = rr.bottom - (int)((m_pToolPath->m_pCp[nn].fPos[nAxis] - fMin) * (float)rr.Height() / fHeight);

		}
		pDC->Polyline(pt,nMax);
	}
	SAFE_DELETE( pt );
	pDC->SelectObject(pOldPen);
}

void CPmacTrajectoryPage::CreateColumns()
{
	CString ColumnName[20] = { L"#",L"X", L"Y", L"Z", L"I",L"J",L"K",L"Xt",L"Yt"};
	int ColumnWidth[20] = { 0,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60};

	int nColumnCount = m_listCoord.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listCoord.DeleteColumn(0);
	}

	for (int ii=0;ii<9;ii++) m_listCoord.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	m_listCoord.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CPmacTrajectoryPage::FillList()
{
	if(InitializeToolPathPtr() == false) return;
	CString Buff;
	int ii;

	int nCount = m_listCoord.GetItemCount();
	if(nCount<m_pToolPath->m_nCoordL) {
		for(ii=nCount;ii<m_pToolPath->m_nCoordL;ii++) {
			Buff.Format(_T("%d"),ii+1);
			m_listCoord.InsertItem(ii, Buff.GetBuffer(255), ii);
		};
	} else {
		for(ii=nCount-1;ii>=m_pToolPath->m_nCoordL;ii--) {
			m_listCoord.DeleteItem(ii);
		}
	}
	m_listCoord.EnsureVisible(theApp.m_nIndex,FALSE);
	for(ii=0;ii<m_pToolPath->m_nCoordL;ii++)	m_listCoord.SetItemState(ii , 0,LVIS_SELECTED);
	m_listCoord.SetItemState(theApp.m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listCoord.SetFocus();

}

void CPmacTrajectoryPage::OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	CCoord* pCp;

	if(InitializeToolPathPtr() == false) return;

	if(m_pToolPath->m_nCoordL<=0) {
		str[0]=0;
		pDispInfo->item.pszText=str;
		return;
	}

	if(m_nTableOrder == 0) {
		pCp = (CCoord*)&m_pToolPath->m_pCp[pDispInfo->item.iItem];
	} else {
		pCp = (CCoord*)&m_pToolPath->m_pCp[m_pToolPath->m_nCoordL - pDispInfo->item.iItem - 1];
	}

	switch(pDispInfo->item.iSubItem) {
	case 0:	swprintf_s(str,100,_T("%d"),pDispInfo->item.iItem+1);
		break;
	case 1:	swprintf_s(str, 100, _T("%.01f"),pCp->Side[m_nSide].fX);
		break;
	case 2:	swprintf_s(str, 100, _T("%.01f"),pCp->Side[m_nSide].fY);
		break;
	case 3:	swprintf_s(str, 100, _T("%.01f"),pCp->Side[m_nSide].fZ);
		break;
	case 4:	swprintf_s(str, 100, _T("%.02f"),pCp->Side[m_nSide].fI);
		break;
	case 5:	swprintf_s(str, 100, _T("%.02f"),pCp->Side[m_nSide].fJ);
		break;
	case 6:	swprintf_s(str, 100, _T("%.02f"),pCp->Side[m_nSide].fK);
		break;
	case 7:	swprintf_s(str, 100, _T("%.02f"),pCp->Side[m_nSide].fXt);
		break;
	case 8:	swprintf_s(str, 100, _T("%.02f"),pCp->Side[m_nSide].fYt);
		break;
	}
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CPmacTrajectoryPage::OnClickListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	m_nIndex=pDispInfo->item.mask;
	if(m_bGotoIfHighlighted == true) {
		Goto(m_nIndex);
	}
	
	*pResult = 0;
}

void CPmacTrajectoryPage::OnRclickListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu *pPopup;
	CPoint	Point;

	POSITION pos = m_listCoord.GetFirstSelectedItemPosition();
	m_nIndex=m_listCoord.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if(menu->LoadMenu(IDR_RB_TRAJECTORY_LIST_MENU)) {
		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);
	}
	delete menu;
	
	*pResult = 0;
}


void CPmacTrajectoryPage::OnTimer(UINT nIDEvent) 
{
	CCoord Cp;
/*
	if(theApp.m_Thread.m_bNewToolPathIfAnyoneWantsIt == true) {

		m_ToolPath += &theApp.m_Thread.m_ToolPath;
		theApp.m_Thread.m_bNewToolPathIfAnyoneWantsIt = false;
		UpdateAllControls();
		Invalidate(false);
	}

	theApp.m_Motors.GetSurfacePos(&Cp);
	if(Cp != m_CpSurface) {
		m_CpSurface = Cp;
		Invalidate(false);
	}
*/

	CResizablePage::OnTimer(nIDEvent);
}

void CPmacTrajectoryPage::OnListGoto() 
{

	Goto(m_nIndex);

}

int CPmacTrajectoryPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CResizablePage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_TRAJECTORY_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()+210,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndToolBar);
	
	
	return 0;
}

BOOL CPmacTrajectoryPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

	switch(nID) {
	case ID_BUTTON_GOTO_IF_HIGHLIGHTED: strTipText.LoadString(IDS_Goto_If_Highlighted);
	   break;
	case ID_BUTTON_LAST_TOOLPATH: strTipText.LoadString(IDS_Last_Toolpath);
		break;
	case ID_BUTTON_NEXT_TOOLPATH: strTipText.LoadString(IDS_Next_Toolpath);
		break;
	case ID_BUTTON_VECTOR_PLOT: strTipText.LoadString(IDS_Polar_Plot);
		break;
	case ID_BUTTON_HEAD_WAYPOINT_LINE: strTipText.LoadString(IDS_Head_Way_Points);
		break;
	case ID_BUTTON_SURFACE_WAYPOINT_LINE: strTipText.LoadString(IDS_Surface_Way_Points);
		break;
	case ID_BUTTON_CLEAR_ALL: strTipText.LoadString(IDS_Clear_All);
		break;
	case ID_BUTTON_REVERSE_TABLE_ORDER:  strTipText.LoadString(IDS_Reverse_Table_Order);
		break;
	case ID_BUTTON_LEFT_COORDINATES:  strTipText.LoadString(IDS_Left);
		break;
	case ID_BUTTON_RIGHT_COORDINATES:  strTipText.LoadString(IDS_Right);
		break;
	}
//      strTipText.Format(_T("Control ID = %d", nID);

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}

void CPmacTrajectoryPage::Goto(int nIndex)
{
	CCoord CpHead,Cp;

	theApp.m_Motors.GetHeadPos(&CpHead);
	if(m_pToolPath->m_nCoordL<=0) return;
	if(m_nIndex<0) return;
	if(nIndex<m_pToolPath->m_nCoordL) {
		m_pToolPath->GetCoord(nIndex,&Cp);
		switch(m_nSide) {
		case PORTSIDE: Cp.CopySide(STARBOARD,CpHead);
			break;
		case STARBOARD: Cp.CopySide(STARBOARD,CpHead);
			break;
		}
	};
	theApp.m_Motors.GotoHeadCoord(Cp);

}



void CPmacTrajectoryPage::OnButtonGotoIfHighlighted() 
{
	// TODO: Add your command handler code here
	
}

void CPmacTrajectoryPage::OnButtonLastToolpath() 
{
	m_pToolPath = &theApp.m_Thread.m_ToolPath;
	theApp.m_Thread.m_bNewToolPathIfAnyoneWantsIt = false;
	UpdateAllControls();
	Invalidate(false);
	
}

void CPmacTrajectoryPage::OnButtonNextToolpath() 
{
	// TODO: Add your command handler code here
	
}

BOOL CPmacTrajectoryPage::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint	point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	if(m_rrBorder.PtInRect(point)) {
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		return TRUE;
	}
	
	return CResizablePage::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CPmacTrajectoryPage::PreTranslateMessage(MSG* pMsg) 
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	CRect rr = m_rrClient;
	CRect rrView,rrList;
	CString Buff;

	switch(pMsg->message) {
	case 0x100:
		switch(pMsg->wParam) {
		case CURSOR_DOWN:
			m_nIndex++;
			if(m_nIndex>=m_pToolPath->m_nCoordL) m_nIndex=m_pToolPath->m_nCoordL-1;
			if(m_nIndex<0) m_nIndex=0;
			if(m_bGotoIfHighlighted == true) {
				Goto(m_nIndex);
			}
			break;
		case CURSOR_UP:
			m_nIndex--;
			if(m_nIndex<0) m_nIndex=0;
			if(m_bGotoIfHighlighted == true) {
				Goto(m_nIndex);
			}
			break;
		}
		break;
	case 0x200: // Mouse move
		switch(pMsg->wParam) {
		case 1:
			if((m_bLbDown == true) && (point != m_ptLbDown)) {
				m_fBorderPos = (float)point.y * 1.0f / (float)rr.Height();

				rrView = m_rrClient;
				rrView.bottom = (int)((float)m_rrClient.Height() * m_fBorderPos) - 4;
				m_staticView.SetWindowPos( NULL , rrView.left, rrView.top, rrView.Width(), rrView.Height(), NULL);

				rrList = m_rrClient;
				rrList.top =(int)((float)m_rrClient.Height() * m_fBorderPos) + 4;
				m_listCoord.SetWindowPos( NULL , rrList.left, rrList.top, rrList.Width(), rrList.Height(), NULL);

				m_rrBorder = rrList;
				m_rrBorder.top = rrView.bottom;
				m_rrBorder.bottom = rrList.top;

				m_listCoord.UpdateWindow();
				m_staticView.UpdateWindow();
				Invalidate(true);

				m_ptLbDown = point;
				return TRUE;
			}
			break;
		};
		break;
	case 0x201://LBUTTONDOWN
		if(m_rrBorder.PtInRect(point)) {
			m_bLbDown = true;
			m_ptLbDown = point;
			return TRUE;
		}
		break;

	case 0x202:	//LBUTTONUP
		m_bLbDown = false;
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CPmacTrajectoryPage::OnButtonVectorPlot() 
{

	m_nRenderType = 0;
	Invalidate(false);
	SetToolBarCheckedState();
}

void CPmacTrajectoryPage::SetToolBarCheckedState()
{
	int	nIndex;

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_VECTOR_PLOT))>=0)
		m_nRenderType==0 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_HEAD_WAYPOINT_LINE))>=0)
		m_nHeadSurface==0 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_SURFACE_WAYPOINT_LINE))>=0)
		m_nHeadSurface==1 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_REVERSE_TABLE_ORDER))>=0)
		m_nTableOrder==1 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_LEFT_COORDINATES))>=0)
		m_nSide==0 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_RIGHT_COORDINATES))>=0)
		m_nSide==1 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
}

void CPmacTrajectoryPage::RenderVectorPlot(CPaintDC *pDC, CRect rrView)
{
	if(InitializeToolPathPtr() == false) return;
	CRect rr;
	int	nRadius,ii,nX,nY,nAxisL;
	float fRadius;
	CCoord Cp;
	CString Buff = L"-1.00";
	CSize size = pDC->GetTextExtent(Buff);
	CPen penGrey(PS_SOLID,1,RGB(200,200,200));
	CPen* pOldPen = pDC->SelectObject(&penGrey);

	if((rrView.Width()/2) > rrView.Height()) {
		nRadius = rrView.Height();
	} else {
		nRadius = rrView.Width() / 2;
	}
	nRadius /= 2;
	nAxisL = nRadius;
	nRadius -= (size.cy*2);
	fRadius = (float)nRadius;

	rr.left = rrView.Width()/4 - nRadius;
	rr.right = rrView.Width()/4 + nRadius;
	rr.top = rrView.CenterPoint().y - nRadius - rrView.top;
	rr.bottom = rrView.CenterPoint().y + nRadius - rrView.top;

	pDC->Ellipse(rr);
	pDC->MoveTo(rr.CenterPoint().x - nAxisL,rr.CenterPoint().y);
	pDC->LineTo(rr.CenterPoint().x + nAxisL,rr.CenterPoint().y);
	pDC->MoveTo(rr.CenterPoint().x ,rr.CenterPoint().y - nAxisL);
	pDC->LineTo(rr.CenterPoint().x ,rr.CenterPoint().y + nAxisL);

	Buff = "i:1.0";
	pDC->TextOut(rr.CenterPoint().x+nRadius,rr.CenterPoint().y - size.cy, Buff);
	Buff = "i:-1.0";
	pDC->TextOut(rr.CenterPoint().x-nRadius-size.cx,rr.CenterPoint().y - size.cy, Buff);
	Buff = "k:1.0";
	pDC->TextOut(rr.CenterPoint().x+2,rr.CenterPoint().y - nRadius - size.cy, Buff);
	Buff = "k:-1.0";
	pDC->TextOut(rr.CenterPoint().x+2,rr.CenterPoint().y + nRadius, Buff);


	pDC->SelectObject(pOldPen);
	for(ii=0;ii<m_pToolPath->m_nCoordL;ii++) {
		m_pToolPath->GetCoord(ii,&Cp);
		nX = rr.CenterPoint().x + (int)(Cp.Side[m_nSide].fI * fRadius);
		nY = rr.CenterPoint().y - (int)(Cp.Side[m_nSide].fK * fRadius);

		if(ii==0) {
			pDC->MoveTo(nX,nY);
		} else {
			pDC->LineTo(nX,nY);
		};
	}
	nX = rr.CenterPoint().x + (int)(m_CpSurface.Side[m_nSide].fI * fRadius);
	nY = rr.CenterPoint().y - (int)(m_CpSurface.Side[m_nSide].fK * fRadius);

	pDC->MoveTo(nX-2,nY-2);
	pDC->LineTo(nX+3,nY+3);
	pDC->MoveTo(nX-2,nY+2);
	pDC->LineTo(nX+3,nY-3);
		

}

void CPmacTrajectoryPage::RenderTipPlot(CPaintDC *pDC, CRect rrView)
{
	CRect rr,rrEllipse;
	int	nRadius,ii,nX,nY,nAxisL;
	float fRadius;
	CCoord Cp;
	CString Buff = L"-1.00";
	CSize size = pDC->GetTextExtent(Buff);
	CPen penGrey(PS_SOLID,1,RGB(200,200,200));
	CPen* pOldPen = pDC->SelectObject(&penGrey);

	if((rrView.Width()/2) > rrView.Height()) {
		nRadius = rrView.Height();
	} else {
		nRadius = rrView.Width() / 2;
	}
	nRadius /= 2;
	nAxisL = nRadius;
	nRadius -= (size.cy*2);
	fRadius = (float)nRadius;

	rr.left = (rrView.Width()*3)/4 - nRadius;
	rr.right = (rrView.Width()*3)/4 + nRadius;
	rr.top = rrView.CenterPoint().y - nRadius - rrView.top;
	rr.bottom = rrView.CenterPoint().y + nRadius - rrView.top;


	pDC->MoveTo(rr.CenterPoint().x - nAxisL,rr.CenterPoint().y);
	pDC->LineTo(rr.CenterPoint().x + nAxisL,rr.CenterPoint().y);
	pDC->MoveTo(rr.CenterPoint().x ,rr.CenterPoint().y - nAxisL);
	pDC->LineTo(rr.CenterPoint().x ,rr.CenterPoint().y + nAxisL);

	for(float fXt=90.0f;fXt<=360.0f;fXt+=90.0f) {
		nX = (int)(fXt * fRadius / 360.0f);
		pDC->MoveTo(rr.CenterPoint().x - nX,rr.CenterPoint().y-nRadius);
		pDC->LineTo(rr.CenterPoint().x - nX,rr.CenterPoint().y+nRadius);
		pDC->MoveTo(rr.CenterPoint().x + nX,rr.CenterPoint().y-nRadius);
		pDC->LineTo(rr.CenterPoint().x + nX,rr.CenterPoint().y+nRadius);
	}

	Buff = "Xt:+360";
	pDC->TextOut(rr.CenterPoint().x+nRadius+2,rr.CenterPoint().y - size.cy, Buff);
	Buff = "Xt:-360";
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.CenterPoint().x-nRadius-size.cx-2,rr.CenterPoint().y - size.cy, Buff);
	Buff = "Yt:+120";
	pDC->TextOut(rr.CenterPoint().x+2,rr.CenterPoint().y-nRadius-size.cy, Buff);
	Buff = "Yt:-120";
	pDC->TextOut(rr.CenterPoint().x+2,rr.CenterPoint().y+nRadius, Buff);

	pDC->SelectObject(pOldPen);

	if(m_pToolPath ) {
		for(ii=0;ii<m_pToolPath->m_nCoordL;ii++) {
			m_pToolPath->GetCoord(ii,&Cp);
			nX = rr.CenterPoint().x + (int)(Cp.Side[m_nSide].fXt * fRadius / 360.0f);
			nY = rr.CenterPoint().y - (int)(Cp.Side[m_nSide].fYt * fRadius / 120.0f);

			if(ii==0) {
				pDC->MoveTo(nX,nY);
			} else {
				pDC->LineTo(nX,nY);
			};
		}
	}

	nX = rr.CenterPoint().x + (int)(m_CpSurface.Side[m_nSide].fXt * fRadius / 360.0f);
	nY = rr.CenterPoint().y - (int)(m_CpSurface.Side[m_nSide].fYt * fRadius / 120.0f);

	pDC->MoveTo(nX-2,nY-2);
	pDC->LineTo(nX+3,nY+3);
	pDC->MoveTo(nX-2,nY+2);
	pDC->LineTo(nX+3,nY-3);
}



void CPmacTrajectoryPage::RefreshFromWayPoints()
{
	if(InitializeToolPathPtr()) {
		UpdateAllControls();
		Invalidate(false);
	}
}



void CPmacTrajectoryPage::OnButtonHeadWaypointLine() 
{
	m_nHeadSurface = 0;

	if(InitializeToolPathPtr()) {
		UpdateAllControls();
		Invalidate(false);
	}
	
}

void CPmacTrajectoryPage::OnButtonSurfaceWaypointLine() 
{
	m_nHeadSurface = 1;

	if(InitializeToolPathPtr()) {
		UpdateAllControls();
		Invalidate(false);
	}
	
}

bool CPmacTrajectoryPage::InitializeToolPathPtr()
{
	if(PROFILE->m_WayPoints) {
		if(theApp.m_nLine < PROFILE->m_nWayPointsL) {
			m_pToolPath = &PROFILE->m_WayPoints[theApp.m_nLine];
			return true;
		}
	}
	return false;
}

void CPmacTrajectoryPage::OnButtonLeftCoordinates() 
{
	m_nSide = 0;
	UpdateAllControls();
	Invalidate(false);
	
}

void CPmacTrajectoryPage::OnButtonRightCoordinates() 
{
	m_nSide = 1;
	UpdateAllControls();
	Invalidate(false);
	
}

void CPmacTrajectoryPage::OnButtonReverseTableOrder() 
{

	m_nTableOrder ^= 1;
	UpdateAllControls();
	Invalidate(false);

}
