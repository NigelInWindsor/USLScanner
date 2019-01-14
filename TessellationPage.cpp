// TessellationPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "TessellationPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTessellationPage property page

IMPLEMENT_DYNCREATE(CTessellationPage, CPropertyPage)

CTessellationPage::CTessellationPage(CPropertySheet* pToolSheet, CPropertyPage* pViewPage) : CPropertyPage(CTessellationPage::IDD)
{
	//{{AFX_DATA_INIT(CTessellationPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSheet = pToolSheet;
	m_pPage = pViewPage;
	
	m_nOrder = 0;
	m_bCtrlButton = false;
	m_bShiftButton = false;
	m_nIndex = 0;

}

CTessellationPage::~CTessellationPage()
{
}

void CTessellationPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTessellationPage)
	DDX_Control(pDX, IDC_LIST_EDGES, m_listEdges);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTessellationPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTessellationPage)
	ON_BN_CLICKED(IDC_BUTTON_AUTOMATIC, OnButtonAutomatic)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_EDGES, OnGetdispinfoListEdges)
	ON_NOTIFY(NM_CLICK, IDC_LIST_EDGES, OnClickListEdges)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_EDGES, OnRclickListEdges)
	ON_COMMAND(ID_TESSELLATION_DELETE, OnTessellationDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTessellationPage message handlers

void CTessellationPage::OnButtonAutomatic() 
{
	

	PROFILE->CalculateTessellationEdges();
	RenderParent();
	FillList();
}

void CTessellationPage::RenderParent()
{
	m_pPage->Invalidate(false);
}

BOOL CTessellationPage::OnSetActive() 
{
	

	theApp.m_LastSettings.nLast3DDrawingToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

BOOL CTessellationPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	
	CreateColumns();
	FillList();
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTessellationPage::UpdateAllControls()
{

}

void CTessellationPage::CreateColumns()
{
	
	int ColumnWidth[6] = { 26,50,50,50,50,20};
	CString ColumnName[6];

	ColumnName[0]= "#";
	ColumnName[1]= "Node 1";
	ColumnName[2]= "Node 2";

	int nColumnCount = m_listEdges.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listEdges.DeleteColumn(0);
	}
	for (int ii=0;ii<3;ii++) {
		m_listEdges.InsertColumn(ii,ColumnName[ii], LVCFMT_RIGHT,ColumnWidth[ii]);
	}
	m_listEdges.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CTessellationPage::FillList()
{
	
	CString Buff;

	m_listEdges.DeleteAllItems();
	for (int ii=0;ii<PROFILE->m_Edge.GetSize();ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listEdges.InsertItem(ii, Buff.GetBuffer(255), ii);
		m_listEdges.SetItemState(ii , 0,LVIS_SELECTED);
	};

	m_listEdges.SetFocus();
	m_listEdges.EnsureVisible(m_nIndex,false);
	m_listEdges.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);

}

void CTessellationPage::OnGetdispinfoListEdges(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	int nn = pDispInfo->item.iItem;
	D3DXVECTOR3 vPt;

	if(PROFILE->m_Edge.GetSize()<=0) return;

	switch(pDispInfo->item.iSubItem) {
	case 0: swprintf_s(str,L"%d",nn+1);
		break;
	case 1:	swprintf_s(str,L"%d",PROFILE->m_Edge.m_pEdge[nn].nNode[0]);
		break;
	case 2:	swprintf_s(str,L"%d",PROFILE->m_Edge.m_pEdge[nn].nNode[1]);
		break;
	}
	
	pDispInfo->item.pszText=str;

	*pResult = 0;
}

void CTessellationPage::OnClickListEdges(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	int nIndex=pDispInfo->item.mask;
	if(nIndex<0) {
		ZeroMemory(theApp.m_nListOrderArray,sizeof theApp.m_nListOrderArray);
		m_nOrder = 0;
	} else {

		if(m_bShiftButton == false && m_bCtrlButton == false) {
			ZeroMemory(theApp.m_nListOrderArray,sizeof theApp.m_nListOrderArray);
			m_nOrder = 0;
			theApp.m_nListOrderArray[PROFILE->m_Edge.m_pEdge[nIndex].nNode[0]] = ++m_nOrder;
			theApp.m_nListOrderArray[PROFILE->m_Edge.m_pEdge[nIndex].nNode[1]] = ++m_nOrder;
		}

		if(m_bCtrlButton == true) {
			theApp.m_nListOrderArray[PROFILE->m_Edge.m_pEdge[nIndex].nNode[0]] = ++m_nOrder;
			theApp.m_nListOrderArray[PROFILE->m_Edge.m_pEdge[nIndex].nNode[1]] = ++m_nOrder;
		}

		if(m_bShiftButton == true) {

			POSITION pos = m_listEdges.GetFirstSelectedItemPosition();
			while(pos) {
				nIndex=m_listEdges.GetNextSelectedItem(pos);
				if(theApp.m_nListOrderArray[PROFILE->m_Edge.m_pEdge[nIndex].nNode[0]] == 0) {
					theApp.m_nListOrderArray[PROFILE->m_Edge.m_pEdge[nIndex].nNode[0]] = ++m_nOrder;
					theApp.m_nListOrderArray[PROFILE->m_Edge.m_pEdge[nIndex].nNode[1]] = ++m_nOrder;
				}
			}

		}
	}

	m_listEdges.Invalidate();
	RenderParent();
	
	*pResult = 0;
}


BOOL CTessellationPage::PreTranslateMessage(MSG* pMsg) 
{
	
	POSITION pos;

	switch(pMsg->message) {
	case WM_KEYUP:
		ZeroMemory(theApp.m_nListOrderArray,sizeof theApp.m_nListOrderArray);
		m_nOrder = 0;
		pos = m_listEdges.GetFirstSelectedItemPosition();
		while(pos) {
			m_nIndex=m_listEdges.GetNextSelectedItem(pos);
			theApp.m_nListOrderArray[PROFILE->m_Edge.m_pEdge[m_nIndex].nNode[0]] = ++m_nOrder;
			theApp.m_nListOrderArray[PROFILE->m_Edge.m_pEdge[m_nIndex].nNode[1]] = ++m_nOrder;
		}
		RenderParent();
		m_bCtrlButton = false;
		m_bShiftButton = false;
		break;
	case WM_KEYDOWN:
		switch(pMsg->wParam) {
		case 16: m_bShiftButton=true;
			break;
		case 17: m_bCtrlButton=true;
			break;
		case 46:	//delete
			pos = m_listEdges.GetFirstSelectedItemPosition();
			while(pos) {
				m_nIndex=m_listEdges.GetNextSelectedItem(pos);
				PROFILE->m_Edge.Delete(m_nIndex);
				m_listEdges.DeleteItem(m_nIndex);
				m_listEdges.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
			}
			RenderParent();
			break;
		}
		break;
	}
		
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CTessellationPage::OnRclickListEdges(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;

	POSITION pos = m_listEdges.GetFirstSelectedItemPosition();
	m_nIndex=m_listEdges.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_TESSELLATION_MENU)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
	
	
	*pResult = 0;
}

void CTessellationPage::OnTessellationDelete() 
{
	

	POSITION pos = m_listEdges.GetFirstSelectedItemPosition();
	while(pos) {
		m_nIndex=m_listEdges.GetNextSelectedItem(pos);
		PROFILE->m_Edge.Delete(m_nIndex);
	}

	FillList();
	RenderParent();

}
