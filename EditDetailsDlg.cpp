// EditDetailsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "EditDetailsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditDetailsDlg dialog


CEditDetailsDlg::CEditDetailsDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg)
	: CDialog(CEditDetailsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditDetailsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	
	m_pEditSpinItem = NULL;
	m_nIndex=0;
	m_pDetailsPage = NULL;

	m_Automatic[0].Empty();
	m_Automatic[1].LoadString(IDS_Long_Date);
	m_Automatic[2].LoadString(IDS_Long_Date_Time);
	m_Automatic[3].LoadString(IDS_Short_Date);
	m_Automatic[4].LoadString(IDS_Short_Date_Time);
	m_Automatic[5].LoadString(IDS_Operator);
	m_Automatic[6].LoadString(IDS_Clear);

	m_Ctrl[0].LoadString(IDS_Edit);
	m_Ctrl[1].LoadString(IDS_Button);

}


void CEditDetailsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditDetailsDlg)
	DDX_Control(pDX, IDC_LIST_DETAILS, m_listDetails);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditDetailsDlg, CDialog)
	//{{AFX_MSG_MAP(CEditDetailsDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_DETAILS, OnClickListDetails)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DETAILS, OnDblclkListDetails)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DETAILS, OnRclickListDetails)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_DETAILS, OnGetdispinfoListDetails)
	ON_COMMAND(ID_EDIT_ADD, OnEditAdd)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,ItemChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditDetailsDlg message handlers

void CEditDetailsDlg::PostNcDestroy() 
{
	if(m_pDetailsPage) m_pDetailsPage->SendMessage(WM_CLOSE);
	
	CDialog::PostNcDestroy();
	delete this;
}

void CEditDetailsDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CEditDetailsDlg::IDD] = FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CEditDetailsDlg::OnDestroy() 
{
	if (CEditDetailsDlg::IDD< MAXIMUMWINDOWSIDNUMBER)
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CEditDetailsDlg::IDD]);

	CDialog::OnDestroy();
	
	*m_pDlg	= NULL;	
	
}

BOOL CEditDetailsDlg::OnInitDialog() 
{
	BOOL bResult = CDialog::OnInitDialog();
	
	m_StaticPosList.Initialize(this, m_listDetails, TRUE, TRUE, TRUE, TRUE);

	if (CEditDetailsDlg::IDD< MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CEditDetailsDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}

	m_StaticPosList.SetNewPos();

	

	CreateColumns();
	FillList();
	UpdateAllControls();
	RefreshDetailsPage();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditDetailsDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

}

void CEditDetailsDlg::CreateColumns()
{
	int ColumnWidth[10] = { 0,172,50,50,50,50,50,50,80,50};
	CString ColumnName[10];
	ColumnName[0] = "#";
	ColumnName[1].LoadString(IDS_Description);
	ColumnName[2].LoadString(IDS_Top);
	ColumnName[3].LoadString(IDS_Left);
	ColumnName[4].LoadString(IDS_Bottom);
	ColumnName[5].LoadString(IDS_Right);
	ColumnName[6].LoadString(IDS_Bottom);
	ColumnName[7].LoadString(IDS_Right);
	ColumnName[8].LoadString(IDS_Automatic);
	ColumnName[9].LoadString(IDS_Ctrl);

	int nColumnCount = m_listDetails.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listDetails.DeleteColumn(0);
	}
	for (int ii=0;ii<10;ii++) {
		m_listDetails.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	}
	m_listDetails.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CEditDetailsDlg::FillList()
{
	CString Buff;

	m_listDetails.DeleteAllItems();
	for (int ii=0;ii<theApp.m_nDescriptionL;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listDetails.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listDetails.EnsureVisible(m_nIndex,FALSE);
	for(int ii=0;ii<theApp.m_nDescriptionL;ii++) 
		m_listDetails.SetItemState(ii , 0,LVIS_SELECTED);
	m_listDetails.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listDetails.SetFocus();

}


void CEditDetailsDlg::OnClickListDetails(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ClearEditSpin();
	
	*pResult = 0;
}

void CEditDetailsDlg::OnDblclkListDetails(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	m_listDetails.GetWindowRect(CtrlRect);
	m_listDetails.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	ClearEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

	switch(pDispInfo->item.iItem) {
	case 0:
		break;
	case 1:	m_pEditSpinItem->Initialize(theApp.m_pDescription[m_nIndex=pDispInfo->item.mask].pCaption);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	case 2:	m_pEditSpinItem->Initialize((int*)(&theApp.m_pDescription[m_nIndex=pDispInfo->item.mask].rrCtrl.top),0,1000,1);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	case 3:	m_pEditSpinItem->Initialize((int*)(&theApp.m_pDescription[m_nIndex=pDispInfo->item.mask].rrCtrl.left),0,1000,1);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	case 4:	m_pEditSpinItem->Initialize((int*)(&theApp.m_pDescription[m_nIndex=pDispInfo->item.mask].rrCtrl.bottom),0,1000,1);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	case 5:	m_pEditSpinItem->Initialize((int*)(&theApp.m_pDescription[m_nIndex=pDispInfo->item.mask].rrCtrl.right),0,1000,1);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	case 6:	m_pEditSpinItem->Initialize((int*)(&theApp.m_pDescription[m_nIndex=pDispInfo->item.mask].rrCtrl.bottom),0,1000,1);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	case 7:	m_pEditSpinItem->Initialize((int*)(&theApp.m_pDescription[m_nIndex=pDispInfo->item.mask].rrCtrl.right),0,1000,1);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	case 8: m_pEditSpinItem->Initialize((int*)&theApp.m_pDescription[m_nIndex=pDispInfo->item.mask].nCtrlType,m_Ctrl,2);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	}

	*pResult = 0;
}

void CEditDetailsDlg::OnRclickListDetails(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;

	POSITION pos = m_listDetails.GetFirstSelectedItemPosition();
	m_nIndex=m_listDetails.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_EDIT_DESCRIPTION_MENU)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
	
	
	*pResult = 0;
}

void CEditDetailsDlg::OnGetdispinfoListDetails(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];

	switch(pDispInfo->item.iSubItem) {
	case 0:
		break;
	case 1:
		str[0]=0;
		swprintf_s(str,_T("%s"),theApp.m_pDescription[pDispInfo->item.iItem].pCaption->GetBuffer());
		break;
	case 2:
		swprintf_s(str,_T("%d"),theApp.m_pDescription[pDispInfo->item.iItem].rrCtrl.top);
		break;
	case 3:
		swprintf_s(str,_T("%d"),theApp.m_pDescription[pDispInfo->item.iItem].rrCtrl.left);
		break;
	case 4:
		swprintf_s(str,_T("%d"),theApp.m_pDescription[pDispInfo->item.iItem].rrCtrl.bottom);
		break;
	case 5:
		swprintf_s(str,_T("%d"),theApp.m_pDescription[pDispInfo->item.iItem].rrCtrl.right);
		break;
	case 6:
		swprintf_s(str,_T("%d"),theApp.m_pDescription[pDispInfo->item.iItem].rrCtrl.bottom);
		break;
	case 7:
		swprintf_s(str,_T("%d"),theApp.m_pDescription[pDispInfo->item.iItem].rrCtrl.right);
		break;
	case 8:
		swprintf_s(str,_T("%s"),m_Ctrl[theApp.m_pDescription[pDispInfo->item.iItem].nCtrlType].GetBuffer());
		break;
	}
	
	pDispInfo->item.pszText=str;

	
	*pResult = 0;
}


void CEditDetailsDlg::ClearEditSpin()
{
	if(m_pEditSpinItem!=NULL) {
		delete m_pEditSpinItem;
		m_pEditSpinItem=NULL;
	}

}

void CEditDetailsDlg::OnEditAdd() 
{
	CRect rr;

	int	nn=theApp.m_nDescriptionL++;
	int	ii;
	DescriptionStruct* pDescriptionTemp;

	if(nn==0) {
		theApp.m_pDescription = (DescriptionStruct *) new DescriptionStruct[theApp.m_nDescriptionL];
	} else {
		pDescriptionTemp = (DescriptionStruct *) new DescriptionStruct[theApp.m_nDescriptionL];
		for(ii=0;ii<nn;ii++) {
			pDescriptionTemp[ii] = theApp.m_pDescription[ii];
		}
		delete theApp.m_pDescription;
		theApp.m_pDescription = (DescriptionStruct *) new DescriptionStruct[theApp.m_nDescriptionL];
		for(ii=0;ii<nn;ii++) {
			theApp.m_pDescription[ii] = pDescriptionTemp[ii];
		}
		delete pDescriptionTemp;
	}

	ZeroMemory(&theApp.m_pDescription[nn],sizeof theApp.m_pDescription[nn]);
	theApp.m_pDescription[nn].nStructSize = sizeof DescriptionStruct;
	theApp.m_pDescription[nn].rrCtrl.SetRect(0,nn*26+5,0,0);
	theApp.m_pDescription[nn].nCtrlType = (CtrlType)0;
	theApp.m_pDescription[nn].pCaption = new CString(_T("Static"));

	FillList();
	RefreshDetailsPage();
	
}

void CEditDetailsDlg::OnEditDelete() 
{
	int nIndex;
	
	POSITION pos = m_listDetails.GetFirstSelectedItemPosition();
	while(pos) {
		nIndex=m_listDetails.GetNextSelectedItem(pos);
		DeleteDescription(nIndex);
	}

	FillList();
	RefreshDetailsPage();
}

bool CEditDetailsDlg::DeleteDescription(int nIndex)
{
	DescriptionStruct* pDescriptionTemp;
	int	ii;
	CString Caption,Edit;

	pDescriptionTemp = (DescriptionStruct *) new DescriptionStruct[theApp.m_nDescriptionL-1];
	for(ii=0;ii<nIndex;ii++) {
		Caption.Format(_T("%s"),*theApp.m_pDescription[ii].pCaption);
		delete theApp.m_pDescription[ii].pCaption;
		pDescriptionTemp[ii] = theApp.m_pDescription[ii];
		pDescriptionTemp[ii].pCaption = new CString(Caption);
	}
	for(ii=nIndex;ii<(theApp.m_nDescriptionL-1);ii++) {
		Caption.Format(_T("%s"),*theApp.m_pDescription[ii+1].pCaption);
		delete theApp.m_pDescription[ii].pCaption;
		pDescriptionTemp[ii] = theApp.m_pDescription[ii+1];
		pDescriptionTemp[ii].pCaption = new CString(Caption);
	}

	delete theApp.m_pDescription[ii].pCaption;
	delete theApp.m_pDescription;
	theApp.m_pDescription = NULL;
	theApp.m_nDescriptionL--;

	if(theApp.m_nDescriptionL>0) {
		theApp.m_pDescription = (DescriptionStruct *) new DescriptionStruct[theApp.m_nDescriptionL];
		for(ii=0;ii<theApp.m_nDescriptionL;ii++) {
			theApp.m_pDescription[ii] = pDescriptionTemp[ii];
		}
	}
	delete pDescriptionTemp;

	return TRUE;
}


void CEditDetailsDlg::RefreshDetailsPage()
{
	if(m_pDetailsPage) delete m_pDetailsPage;

	m_pDetailsPage = new CDetailsPage((CWnd**)&m_pDetailsPage,&theApp.m_Data,TRUE);
	m_pDetailsPage->Create(CDetailsPage::IDD);	
}

HRESULT CEditDetailsDlg::ItemChanged(WPARAM,LPARAM)
{
	m_pDetailsPage->Invalidate(TRUE);
	return NULL;
}

void CEditDetailsDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticPosList.SetNewPos();
	
}
