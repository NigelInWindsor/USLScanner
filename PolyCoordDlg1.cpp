// PolyCoordDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "PolyCoordDlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct	ItemData {
	HTREEITEM hItem;
	HTREEITEM hSide[2];
	HTREEITEM hPt[2];
	HTREEITEM hNorm[2];
};

#define	COORDS_AXIS	0
#define	AXIS_AXIS	1

/////////////////////////////////////////////////////////////////////////////
// CPolyCoordDlg dialog


CPolyCoordDlg::CPolyCoordDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg, CPolyCoord* pLine, int nLineType, int nLine)
	: CDialog(CPolyCoordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPolyCoordDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	m_pMasterLine = pLine;
	m_nIndex = 0;
	m_nSurfaceJoint = 0;

	m_NewLine.Zero();
	if(pLine) m_Line = pLine;
	m_pLine = pLine;

	m_nLineType = nLineType;
	m_nLine = nLine;
	InitializeLinePtr();

	theApp.m_LastSettings.nVertexDisplayMode = COORDS_AXIS;
}


void CPolyCoordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPolyCoordDlg)
	DDX_Control(pDX, IDC_EDIT_PROMPT, m_editPrompt);
	DDX_Control(pDX, IDC_SPIN_LINE_NUMBER, m_spinLineNumber);
	DDX_Control(pDX, IDC_EDIT_LINE_NUMBER, m_editLineNumber);
	DDX_Control(pDX, IDC_COMBO_LINE_TYPE, m_comboLineType);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_TREE, m_treeCoords);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPolyCoordDlg, CDialog)
	//{{AFX_MSG_MAP(CPolyCoordDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_RCLICK, IDC_TREE, OnRclickTree)
	ON_COMMAND(ID_COORD_EXPAND, OnCoordExpand)
	ON_COMMAND(ID_COORD_EXPANDNORM, OnCoordExpandnorm)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_COMMAND(ID_VERTEX_PERSPECTIVE_ZY, OnVertexPerspectiveZy)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_EXPORT, OnButtonExport)
	ON_COMMAND(ID_VERTEX_PERSPECTIVE, OnVertexPerspective)
	ON_COMMAND(ID_VERTEX_AXES_X, OnVertexAxesX)
	ON_COMMAND(ID_VERTEX_AXES_Y, OnVertexAxesY)
	ON_COMMAND(ID_VERTEX_AXES_Z, OnVertexAxesZ)
	ON_COMMAND(ID_VERTEX_AXES_XTIP, OnVertexAxesXtip)
	ON_COMMAND(ID_VERTEX_AXES_YTIP, OnVertexAxesYtip)
	ON_COMMAND(ID_VERTEX_AXES_XRIGHT, OnVertexAxesXright)
	ON_COMMAND(ID_VERTEX_AXES_Y_RIGHT, OnVertexAxesYRight)
	ON_COMMAND(ID_VERTEX_AXES_Z_RIGHT, OnVertexAxesZRight)
	ON_COMMAND(ID_VERTEX_AXES_XTIP_RIGHT, OnVertexAxesXtipRight)
	ON_COMMAND(ID_VERTEX_AXES_YTIP_RIGHT, OnVertexAxesYtipRight)
	ON_EN_CHANGE(IDC_EDIT_LINE_NUMBER, OnChangeEditLineNumber)
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_TYPE, OnSelchangeComboLineType)
	ON_WM_VSCROLL()
	ON_COMMAND(ID_BUTTON_JOINT, OnButtonJoint)
	ON_COMMAND(ID_BUTTON_SURFACE, OnButtonSurface)
	ON_COMMAND(ID_BUTTON_X_LEFT, OnVertexAxesX)
	ON_COMMAND(ID_BUTTON_Y_LEFT, OnVertexAxesY)
	ON_COMMAND(ID_BUTTON_Z_LEFT, OnVertexAxesZ)
	ON_COMMAND(ID_BUTTON_XTIP_LEFT, OnVertexAxesXtip)
	ON_COMMAND(ID_BUTTON_YTIP_LEFT, OnVertexAxesYtip)
	ON_COMMAND(ID_BUTTON_X_RIGHT, OnVertexAxesXright)
	ON_COMMAND(ID_BUTTON_Y_RIGHT, OnVertexAxesYRight)
	ON_COMMAND(ID_BUTTON_Z_RIGHT, OnVertexAxesZRight)
	ON_COMMAND(ID_BUTTON_XTIP_RIGHT, OnVertexAxesXtipRight)
	ON_COMMAND(ID_BUTTON_YTIP_RIGHT, OnVertexAxesYtipRight)
	ON_COMMAND(ID_BUTTON_IJK_LEFT, OnButtonIjkLeft)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPolyCoordDlg message handlers
int CPolyCoordDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_POLYCOORD_TOOLBAR)) {
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

void CPolyCoordDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CPolyCoordDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CPolyCoordDlg::IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CPolyCoordDlg::OnDestroy() 
{
	if (CPolyCoordDlg::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CPolyCoordDlg::IDD]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CPolyCoordDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();
	CString Buff;


	m_StaticPosTree.Initialize(this, m_treeCoords, TRUE, FALSE, TRUE, TRUE);
	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);

	if (CPolyCoordDlg::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CPolyCoordDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}
	m_StaticPosTree.SetNewPos();
	m_StaticPosView.SetNewPos();

	Buff.LoadString(IDS_Taught);	m_comboLineType.AddString(Buff);
	Buff.LoadString(IDS_Scan);		m_comboLineType.AddString(Buff);
	Buff.LoadString(IDS_Brane);		m_comboLineType.AddString(Buff);
	Buff.LoadString(IDS_Envelope);	m_comboLineType.AddString(Buff);

	CreateColumns();

	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPolyCoordDlg::UpdateAllControls()
{
	CString Buff;

	m_comboLineType.SetCurSel(m_nLineType);
	Buff.Format(L"%d",m_nLine+1);
	m_editLineNumber.SetWindowText(Buff);
	m_spinLineNumber.SetRange(0,m_nLineL-1);
	m_spinLineNumber.SetPos(m_nLine);

	SetToolBarCheckedState();
	FillList();
}

void CPolyCoordDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticPosTree.SetNewPos();
	m_StaticPosView.SetNewPos();

	Invalidate(false);
	
}

void CPolyCoordDlg::CreateColumns()
{

}

void CPolyCoordDlg::FillList()
{
	CString Buff;
	int	nCoordL;

	m_treeCoords.DeleteAllItems();

	if((nCoordL=m_Line.GetSize())>0) {

		ItemData *pItem = new ItemData [nCoordL];

		for(int nn=0; nn<nCoordL;nn++) {
			Buff.Format(_T("Pt %d"),nn+1);							pItem[nn].hItem=m_treeCoords.InsertItem(Buff);
			m_treeCoords.SetItemData(pItem[nn].hItem,nn+1);

			for(int nSide=0;nSide<2;nSide++) {
				Buff.Format(_T("Side %d"),nSide);					pItem[nn].hSide[nSide]=m_treeCoords.InsertItem(Buff,pItem[nn].hItem);
				Buff.Format(_T("Pt"));								pItem[nn].hPt[nSide]=m_treeCoords.InsertItem(Buff,pItem[nn].hSide[nSide]);
				Buff.Format(_T("Norm"));							pItem[nn].hNorm[nSide]=m_treeCoords.InsertItem(Buff,pItem[nn].hSide[nSide]);

				Buff.Format(_T("X:%.02f"),m_Line.m_pCp[nn].Side[nSide].fX);	m_treeCoords.InsertItem(Buff,pItem[nn].hPt[nSide]);
				Buff.Format(_T("Y:%.02f"),m_Line.m_pCp[nn].Side[nSide].fY);	m_treeCoords.InsertItem(Buff,pItem[nn].hPt[nSide]);
				Buff.Format(_T("Z:%.02f"),m_Line.m_pCp[nn].Side[nSide].fZ);	m_treeCoords.InsertItem(Buff,pItem[nn].hPt[nSide]);

				Buff.Format(_T("i:%.06f"),m_Line.m_pCp[nn].Side[nSide].fI);	m_treeCoords.InsertItem(Buff,pItem[nn].hNorm[nSide]);
				Buff.Format(_T("j:%.06f"),m_Line.m_pCp[nn].Side[nSide].fJ);	m_treeCoords.InsertItem(Buff,pItem[nn].hNorm[nSide]);
				Buff.Format(_T("k:%.06f"),m_Line.m_pCp[nn].Side[nSide].fK);	m_treeCoords.InsertItem(Buff,pItem[nn].hNorm[nSide]);
				Buff.Format(_T("W:%.06f"),m_Line.m_pCp[nn].Side[nSide].fW);	m_treeCoords.InsertItem(Buff,pItem[nn].hNorm[nSide]);

				Buff.Format(_T("Xt:%.03f"),m_Line.m_pCp[nn].Side[nSide].fXt);	m_treeCoords.InsertItem(Buff,pItem[nn].hNorm[nSide]);
				Buff.Format(_T("Yt:%.03f"),m_Line.m_pCp[nn].Side[nSide].fYt);	m_treeCoords.InsertItem(Buff,pItem[nn].hNorm[nSide]);
			}
		}

		delete pItem;
	}


	return ;
}

void CPolyCoordDlg::OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu* menu = (CMenu *) new CMenu;
	CMenu *pPopup;
	CPoint Point;
	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_COORD_MENU)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};

	delete menu;
	*pResult = 0;
	return;

}



void CPolyCoordDlg::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pNMTreeView->itemNew.lParam) {
		m_nIndex = pNMTreeView->itemNew.lParam - 1;
		Invalidate(false);
	}
	*pResult = 0;
}

void CPolyCoordDlg::OnCoordExpand() 
{
	ItemData Item;
	Item.hItem = m_treeCoords.GetSelectedItem();
	m_treeCoords.EnsureVisible(Item.hItem);

	if (Item.hItem != NULL) {
		if (m_treeCoords.ItemHasChildren(Item.hItem)) {
			Item.hSide[0] = m_treeCoords.GetChildItem(Item.hItem);
			m_treeCoords.Expand(Item.hSide[0], TVE_EXPAND);
			m_treeCoords.EnsureVisible(Item.hSide[0]);
			Item.hSide[1] = m_treeCoords.GetNextSiblingItem(Item.hSide[0]);
			m_treeCoords.Expand(Item.hSide[1], TVE_EXPAND);
			m_treeCoords.EnsureVisible(Item.hSide[1]);

			Item.hPt[0] = m_treeCoords.GetChildItem(Item.hSide[0]);
			m_treeCoords.Expand(Item.hPt[0], TVE_EXPAND);
			m_treeCoords.EnsureVisible(Item.hPt[0]);

			Item.hPt[1] = m_treeCoords.GetChildItem(Item.hSide[1]);
			m_treeCoords.Expand(Item.hPt[1], TVE_EXPAND);
			m_treeCoords.EnsureVisible(Item.hPt[1]);
		}
	}
	
}

void CPolyCoordDlg::OnCoordExpandnorm() 
{
	ItemData Item;
	Item.hItem = m_treeCoords.GetSelectedItem();
	m_treeCoords.EnsureVisible(Item.hItem);

	if (Item.hItem != NULL) {
		if (m_treeCoords.ItemHasChildren(Item.hItem)) {
			Item.hSide[0] = m_treeCoords.GetChildItem(Item.hItem);
			m_treeCoords.Expand(Item.hSide[0], TVE_EXPAND);
			m_treeCoords.EnsureVisible(Item.hSide[0]);
			Item.hSide[1] = m_treeCoords.GetNextSiblingItem(Item.hSide[0]);
			m_treeCoords.Expand(Item.hSide[1], TVE_EXPAND);
			m_treeCoords.EnsureVisible(Item.hSide[1]);

			Item.hPt[0] = m_treeCoords.GetChildItem(Item.hSide[0]);
			Item.hNorm[0] = m_treeCoords.GetNextSiblingItem(Item.hPt[0]);
			m_treeCoords.Expand(Item.hNorm[0], TVE_EXPAND);
			m_treeCoords.EnsureVisible(Item.hNorm[0]);

			Item.hPt[1] = m_treeCoords.GetChildItem(Item.hSide[1]);
			Item.hNorm[1] = m_treeCoords.GetNextSiblingItem(Item.hPt[1]);
			m_treeCoords.Expand(Item.hNorm[1], TVE_EXPAND);
			m_treeCoords.EnsureVisible(Item.hNorm[1]);
		}
	}
}


void CPolyCoordDlg::OnPaint() 
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

	m_Line.FindLimits(&m_Cp[0],&m_Cp[1],true);

	RenderPerspectiveArrows((CPaintDC*)&dcMem,rr);
	RenderGraticule((CPaintDC*)&dcMem,rr);
	RenderLine((CPaintDC*)&dcMem,rr,&m_Line);

	dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);

	dcMem.SelectObject(pOldFont);
	dcMem.SelectObject(pOldBitmap);
	SAFE_DELETE( pArray );
}


void CPolyCoordDlg::RenderPerspectiveArrows(CPaintDC *pDC, CRect rrView)
{
	CString Buff;

	CPen penRed(PS_SOLID,1,RGB(255,0,0));
	CPen penGreen(PS_SOLID,1,RGB(0,255,0));
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));

	CPen* pOldPen = pDC->SelectObject(&penBlack);

	switch(theApp.m_LastSettings.nHAxis) {
	case 0: pDC->SelectObject(&penRed);
		Buff = 'X';
		break;
	case 1: pDC->SelectObject(&penGreen);
		Buff = 'Y';
		break;
	case 2: pDC->SelectObject(&penBlue);
		Buff = 'Z';
		break;
	}
	pDC->MoveTo(10,10);
	pDC->LineTo(30,10);
	pDC->LineTo(26,6);
	pDC->MoveTo(30,10);
	pDC->LineTo(26,14);
	CSize size = pDC->GetTextExtent(Buff);
	pDC->TextOut(30,10-size.cy/2,Buff);

	switch(theApp.m_LastSettings.nVAxis) {
	case 0: pDC->SelectObject(&penRed);
		Buff = 'X';
		break;
	case 1: pDC->SelectObject(&penGreen);
		Buff = 'Y';
		break;
	case 2: pDC->SelectObject(&penBlue);
		Buff = 'Z';
		break;
	}
	pDC->MoveTo(10,10);
	pDC->LineTo(10,30);
	pDC->LineTo(6,26);
	pDC->MoveTo(10,30);
	pDC->LineTo(14,26);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(10-size.cx/2,30,Buff);

	pDC->SelectObject(pOldPen);
}


void CPolyCoordDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu* menu = (CMenu *) new CMenu;
	CPoint pt = point;
	CRect rr;

	ScreenToClient(&point);
	m_ptClient = point;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(1,1);

	if(rr.PtInRect(point)) {
		if (menu->LoadMenu(IDR_RB_VERTICES_MENU)) {
			CMenu* pPopup = menu->GetSubMenu(0);

			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
		};
	}

	delete menu;
	
}

void CPolyCoordDlg::RenderGraticule(CPaintDC *pDC, CRect rrView)
{
	CString Buff;
	int ii,xx,yy;
	CSize size;


	m_rrGrat = rrView;
	m_rrGrat.OffsetRect(-rrView.left,-rrView.top);
	m_rrGrat.DeflateRect(40,20);

	pDC->MoveTo(m_rrGrat.TopLeft());
	pDC->LineTo(m_rrGrat.left, m_rrGrat.bottom);
	pDC->LineTo(m_rrGrat.BottomRight());

	for(ii=0;ii<m_Line.GetSize();ii++) {
		xx=m_rrGrat.left + MulDiv(ii,m_rrGrat.Width(),m_Line.GetSize()-1);
		pDC->MoveTo(xx,m_rrGrat.bottom);
		pDC->LineTo(xx,m_rrGrat.bottom+4);
		Buff.Format(L"%d",ii+1);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(xx-size.cx/2,m_rrGrat.bottom+4,Buff);
	}
	
	for(ii=-90;ii<=90;ii+=10) {
		yy = m_rrGrat.CenterPoint().y - MulDiv(ii,m_rrGrat.Height()/2,90);
		Buff.Format(L"%d%s",ii,DEGREES);
		size = pDC->GetTextExtent(Buff);
		pDC->MoveTo(m_rrGrat.left-3,yy);
		pDC->LineTo(m_rrGrat.left,yy);
		pDC->TextOut(m_rrGrat.left-3-size.cx,yy-size.cy/2,Buff);
	}


}


void CPolyCoordDlg::RenderLine(CPaintDC *pDC, CRect rrView, CPolyCoord *pLine)
{
	CString Buff;
	CCoord Cp;
	CPoint pt;
	CPen penRed(PS_SOLID,1,RGB(255,0,0));
	CPen penGreen(PS_SOLID,1,RGB(0,255,0));
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	int ii,nAxis,nMask;

	CPen* pOldPen = pDC->SelectObject(&penBlack);
	switch(theApp.m_LastSettings.nVertexDisplayMode) {
	case AXIS_AXIS:
		nAxis=0;
		for(ii=0;ii<pLine->GetSize();ii++) {
			pLine->GetCoord(ii,&Cp);
			if(ii==0) {
				pDC->MoveTo(WorldToClient(Cp,&pt,ii,nAxis));
			} else {
				pDC->LineTo(WorldToClient(Cp,&pt,ii,nAxis));
			}
		}

		pDC->SelectObject(&penRed);
		pLine->GetCoord(m_nIndex,&Cp);
		WorldToClient(Cp,&pt,m_nIndex,nAxis);
		pDC->MoveTo(pt.x-3,pt.y-3);
		pDC->LineTo(pt.x+4,pt.y+4);
		pDC->MoveTo(pt.x-3,pt.y+3);
		pDC->LineTo(pt.x+4,pt.y-4);
		break;
	case COORDS_AXIS:
		nMask = 1;
		for(nAxis=0;nAxis<30;nAxis++,nMask<<=1) {

			if(theApp.m_LastSettings.nVertexAxisDisplayMask & nMask) {
				if(nAxis<10) {
					pDC->SelectObject(&penRed);
				} else {
					pDC->SelectObject(&penGreen);
				}

				for(ii=0;ii<pLine->GetSize();ii++) {
					pLine->GetCoord(ii,&Cp);
					if(ii==0) {
						pDC->MoveTo(WorldToClient(Cp,&pt,ii,nAxis));
					} else {
						pDC->LineTo(WorldToClient(Cp,&pt,ii,nAxis));
					}
				}

			}
		}

		break;
	}
	 pDC->SelectObject(pOldPen);
}

CPoint CPolyCoordDlg::WorldToClient(CCoord Cp,CPoint *pt,int nIndex,int nAxis)
{
	int nH = theApp.m_LastSettings.nHAxis;
	int nV = theApp.m_LastSettings.nVAxis;
	CCoord CpSurface;

	if(m_nSurfaceJoint == 1) {
		CpSurface = Cp;
		theApp.m_Kinematics.HeadFromSurface(&CpSurface, &Cp, PORTSIDE, TRUE,TRUE);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface, &Cp, STARBOARD, TRUE,TRUE);
	}

	switch(theApp.m_LastSettings.nVertexDisplayMode) {
	case AXIS_AXIS:
		pt->x = m_rrGrat.left +   (int)((Cp.Side0.fPos[nH] - m_Cp[0].Side0.fPos[nH]) * (float)m_rrGrat.Width() / (m_Cp[1].Side0.fPos[nH]-m_Cp[0].Side0.fPos[nH]));
		pt->y = m_rrGrat.bottom - (int)((Cp.Side0.fPos[nV] - m_Cp[0].Side0.fPos[nV]) * (float)m_rrGrat.Height() / (m_Cp[1].Side0.fPos[nV]-m_Cp[0].Side0.fPos[nV]));
		break;
	case COORDS_AXIS:
		pt->x = m_rrGrat.left + MulDiv(m_rrGrat.Width(),nIndex,m_Line.GetSize()-1);
		switch(nAxis) {
		case 0:
		case 1:
		case 2:
		case 10:
		case 11:
		case 12:
			pt->y=m_rrGrat.CenterPoint().y - (int)(Cp.fPos[nAxis] * (float)m_rrGrat.Height()/2 / 4000.0f);
			break;
		case 5:
		case 6:
		case 7:
			pt->y=m_rrGrat.CenterPoint().y - (int)(Cp.fPos[nAxis] * (float)m_rrGrat.Height()/2 / 1.0f);
			break;
		case 3:
		case 4:
		case 13:
		case 14:
			pt->y=m_rrGrat.CenterPoint().y - (int)(Cp.fPos[nAxis] * (float)m_rrGrat.Height()/2 / 90.0f);
			break;
		}

		break;
	}
	return *pt;

}

CCoord CPolyCoordDlg::ClientToWorld(CPoint pt, CCoord *Cp)
{
	int nH = theApp.m_LastSettings.nHAxis;
	int nV = theApp.m_LastSettings.nVAxis;

	Cp->Zero();

	Cp->Side0.fPos[nH] = (float)(pt.x - m_rrGrat.left) * (m_Cp[1].Side0.fPos[nH]-m_Cp[0].Side0.fPos[nH]) / (float)m_rrGrat.Width() + m_Cp[0].Side0.fPos[nH];
	Cp->Side0.fPos[nV] = (float)(m_rrGrat.bottom - pt.y) * (m_Cp[1].Side0.fPos[nV]-m_Cp[0].Side0.fPos[nV]) / (float)m_rrGrat.Height() + m_Cp[0].Side0.fPos[nV];

	return *Cp;
}

void CPolyCoordDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
//	m_bLBDown = true;
	m_ptClient = point;
	CCoord Cp;
	CRect rrView;
	int nIgnoreAxis=0;
	float fTemp;
	float fHypot=100000.0f;
	CString Buff;


	if(theApp.m_LastSettings.nHAxis==0 && theApp.m_LastSettings.nVAxis==1) nIgnoreAxis = 2;
	if(theApp.m_LastSettings.nHAxis==1 && theApp.m_LastSettings.nVAxis==0) nIgnoreAxis = 2;
	if(theApp.m_LastSettings.nHAxis==0 && theApp.m_LastSettings.nVAxis==2) nIgnoreAxis = 1;
	if(theApp.m_LastSettings.nHAxis==2 && theApp.m_LastSettings.nVAxis==0) nIgnoreAxis = 1;
	if(theApp.m_LastSettings.nHAxis==1 && theApp.m_LastSettings.nVAxis==2) nIgnoreAxis = 0;
	if(theApp.m_LastSettings.nHAxis==2 && theApp.m_LastSettings.nVAxis==1) nIgnoreAxis = 0;

	m_staticView.GetWindowRect(&rrView);
	ScreenToClient(&rrView);
	rrView.DeflateRect(1,1);

	if(rrView.PtInRect(point)) {
		point.x -= rrView.left;
		point.y -= rrView.top;
		ClientToWorld(point, &Cp);

		for(int nn=0;nn<m_Line.m_nCoordL;nn++) {
			Cp.Side0.fPos[nIgnoreAxis] = m_Line.m_pCp[nn].Side0.fPos[nIgnoreAxis];
			fTemp=Cp.Length(m_Line.m_pCp[nn]);
			if(fTemp<fHypot) {
				m_nIndex = nn;
				fHypot=fTemp;
			}
		}
		Buff.Format(L"Index %d",MulDiv(point.x,m_Line.m_nCoordL-1,rrView.Width()));
		m_editPrompt.SetWindowText(Buff);

		Invalidate( false );

	}
	
	CDialog::OnLButtonDown(nFlags, point);
}


//this is primarily a demonstrator to generate coords for the robot
void CPolyCoordDlg::OnButtonExport() 
{
	CCoord Cp;
	CString Buff,Language;
	CUSLFile File;
	CFileException e;
	CString FilePath = L"C:\\Temp\\Coords.txt";
	int nOutputMode = 1;
	D3DXMATRIXA16 matNormal;
	D3DXQUATERNION qNormal;
	int jj=0;
	int ii;

	if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
		File.DecodeFileException(&e,&Buff,FilePath);
		Language.LoadString(IDS_Error);
		MessageBox(Buff,Language,MB_ICONERROR);
	} else {
		CArchive* pArchFile;
		pArchFile = new CArchive(&File,CArchive::store);

		for(ii=0,jj=0;ii<m_Line.GetSize();ii+=5,jj+=14) {
			m_Line.GetCoord(ii,&Cp);

			Cp.InvertNorm(PORTSIDE);
			Cp.TransformCoordinate(PORTSIDE,&theApp.m_Robot[0].m_vBase,theApp.m_Robot[0].m_fBaseRotX,theApp.m_Robot[0].m_fBaseRotY,theApp.m_Robot[0].m_fBaseRotZ);
			Cp.TransformCoordinate(STARBOARD,&theApp.m_Robot[1].m_vBase,theApp.m_Robot[1].m_fBaseRotX,theApp.m_Robot[1].m_fBaseRotY,theApp.m_Robot[1].m_fBaseRotZ);

			Cp.FormatQuaternion(&Buff,0,jj);
			pArchFile->WriteString(Buff + "\r\n");
		}
		for(jj=0;ii>=0;ii-=5,jj+=14) {
			m_Line.GetCoord(ii,&Cp);

			Cp.InvertNorm(PORTSIDE);
			Cp.TransformCoordinate(PORTSIDE,&theApp.m_Robot[0].m_vBase,theApp.m_Robot[0].m_fBaseRotX,theApp.m_Robot[0].m_fBaseRotY,theApp.m_Robot[0].m_fBaseRotZ);
			Cp.TransformCoordinate(STARBOARD,&theApp.m_Robot[1].m_vBase,theApp.m_Robot[1].m_fBaseRotX,theApp.m_Robot[1].m_fBaseRotY,theApp.m_Robot[1].m_fBaseRotZ);
			Cp.FormatQuaternion(&Buff,0,jj);
			pArchFile->WriteString(Buff + "\r\n");
		}

		pArchFile->Close();
		delete pArchFile;

		File.Close();
	}



}

//theApp.m_LastSettings.nVertexAxisDisplayMask

void CPolyCoordDlg::OnVertexPerspectiveZy() 
{
	theApp.m_LastSettings.nHAxis = 2;
	theApp.m_LastSettings.nVAxis = 1;
	theApp.m_LastSettings.nVertexDisplayMode = AXIS_AXIS;
	Invalidate(false);
	
}

void CPolyCoordDlg::OnVertexPerspective() 
{
	theApp.m_LastSettings.nVertexDisplayMode = COORDS_AXIS;
	Invalidate(false);	
}

void CPolyCoordDlg::OnVertexAxesX() 
{
	theApp.m_LastSettings.nVertexAxisDisplayMask ^= 1;
	Invalidate(false);	
	SetToolBarCheckedState();
}

void CPolyCoordDlg::OnVertexAxesY() 
{
	theApp.m_LastSettings.nVertexAxisDisplayMask ^= 2;
	Invalidate(false);	
	SetToolBarCheckedState();
}

void CPolyCoordDlg::OnVertexAxesZ() 
{
	theApp.m_LastSettings.nVertexAxisDisplayMask ^= 4;
	Invalidate(false);	
	SetToolBarCheckedState();
}

void CPolyCoordDlg::OnVertexAxesXtip() 
{
	theApp.m_LastSettings.nVertexAxisDisplayMask ^= 8;
	Invalidate(false);	
	SetToolBarCheckedState();
}

void CPolyCoordDlg::OnVertexAxesYtip() 
{
	theApp.m_LastSettings.nVertexAxisDisplayMask ^= 0x10;
	Invalidate(false);	
	SetToolBarCheckedState();
}

void CPolyCoordDlg::OnButtonIjkLeft() 
{
	theApp.m_LastSettings.nVertexAxisDisplayMask ^= 0xe0;
	Invalidate(false);	
	SetToolBarCheckedState();
}

void CPolyCoordDlg::OnVertexAxesXright() 
{
	theApp.m_LastSettings.nVertexAxisDisplayMask ^= 0x400;
	Invalidate(false);	
	SetToolBarCheckedState();
}

void CPolyCoordDlg::OnVertexAxesYRight() 
{
	theApp.m_LastSettings.nVertexAxisDisplayMask ^= 0x800;
	Invalidate(false);	
	SetToolBarCheckedState();
}

void CPolyCoordDlg::OnVertexAxesZRight() 
{
	theApp.m_LastSettings.nVertexAxisDisplayMask ^= 0x1000;
	Invalidate(false);	
	SetToolBarCheckedState();
}

void CPolyCoordDlg::OnVertexAxesXtipRight() 
{
	theApp.m_LastSettings.nVertexAxisDisplayMask ^= 0x2000;
	Invalidate(false);	
	SetToolBarCheckedState();
}

void CPolyCoordDlg::OnVertexAxesYtipRight() 
{
	theApp.m_LastSettings.nVertexAxisDisplayMask ^= 0x4000;
	Invalidate(false);	
	SetToolBarCheckedState();
}


void CPolyCoordDlg::OnChangeEditLineNumber() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CPolyCoordDlg::OnSelchangeComboLineType() 
{
	m_nLineType = m_comboLineType.GetCurSel();
	InitializeLinePtr();
	UpdateAllControls();
	
}

void CPolyCoordDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	int nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_LINE_NUMBER:
		if(m_pLine) {
			nTemp = m_nLine;
			m_nLine = pSpin->GetPos()&0xffff;
			if(m_nLine<0) m_nLine = 0;
			if(nTemp - m_nLine) {
				m_Line = &m_pLine[m_nLine];
				UpdateAllControls();
				Invalidate(false);
			}
		}
		break;
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CPolyCoordDlg::InitializeLinePtr()
{
	switch(m_nLineType) {
	case TAUGHTLINES: m_pLine = PROFILE->m_TaughtLine;
		m_nLineL = PROFILE->m_nTaughtLineL;
		break;
	case SCANLINES: m_pLine = PROFILE->m_ScanLine;
		m_nLineL = PROFILE->m_nScanLineL;
		break;
	case BRAINLINES: m_pLine = PROFILE->m_BrainLine;
		m_nLineL = PROFILE->m_nBrainLineL;
		break;
	case ENVELOPELINES: m_pLine = PROFILE->m_EnvelopeLine;
		m_nLineL = PROFILE->m_nEnvelopeLineL;
		break;
	case SURFACELINES: m_pLine = PROFILE->m_SurfaceLine;
		m_nLineL = PROFILE->m_nSurfaceLineL;
		break;
	case WAYLINES: m_pLine = PROFILE->m_WayPoints;
		m_nLineL = PROFILE->m_nWayPointsL;
		break;
	}
	if(m_nLine>=m_nLineL) m_nLine = m_nLineL-1;
	if(m_nLine<0) m_nLine = 0;


	if(m_pLine) {
		m_Line = &m_pLine[m_nLine];
	} else {
		m_nLine = 0;
	}

}

void CPolyCoordDlg::OnButtonJoint() 
{
	m_nSurfaceJoint = 1;
	UpdateAllControls();
	
}

void CPolyCoordDlg::OnButtonSurface() 
{

	m_nSurfaceJoint = 0;
	UpdateAllControls();
}

void CPolyCoordDlg::SetToolBarCheckedState()
{
	int	nIndex;

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_SURFACE))>=0)
		m_nSurfaceJoint==0 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_JOINT))>=0)
		m_nSurfaceJoint==1 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_X_LEFT))>=0)
		theApp.m_LastSettings.nVertexAxisDisplayMask & 0x0001 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_Y_LEFT))>=0)
		theApp.m_LastSettings.nVertexAxisDisplayMask & 0x0002 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_Z_LEFT))>=0)
		theApp.m_LastSettings.nVertexAxisDisplayMask & 0x0004 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_XTIP_LEFT))>=0)
		theApp.m_LastSettings.nVertexAxisDisplayMask & 0x0008 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_YTIP_LEFT))>=0)
		theApp.m_LastSettings.nVertexAxisDisplayMask & 0x0010 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_IJK_LEFT))>=0)
		theApp.m_LastSettings.nVertexAxisDisplayMask & 0x00e0 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_X_RIGHT))>=0)
		theApp.m_LastSettings.nVertexAxisDisplayMask & 0x0400 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_Y_RIGHT))>=0)
		theApp.m_LastSettings.nVertexAxisDisplayMask & 0x0800 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_Z_RIGHT))>=0)
		theApp.m_LastSettings.nVertexAxisDisplayMask & 0x1000 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_XTIP_RIGHT))>=0)
		theApp.m_LastSettings.nVertexAxisDisplayMask & 0x2000 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_YTIP_RIGHT))>=0)
		theApp.m_LastSettings.nVertexAxisDisplayMask & 0x4000 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);

}


