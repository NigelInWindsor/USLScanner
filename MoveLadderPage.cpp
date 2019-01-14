// MoveLadderPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "MoveLadderPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CMoveLadderPage property page

IMPLEMENT_DYNCREATE(CMoveLadderPage, CPropertyPage)

CMoveLadderPage::CMoveLadderPage() : CPropertyPage(CMoveLadderPage::IDD)
{
	//{{AFX_DATA_INIT(CMoveLadderPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nIndex = 0;
	m_strAction[0].LoadString( IDS_No_action );
	m_strAction[1].LoadString( IDS_Absolute );
	m_strAction[2].LoadString( IDS_Relative );

	m_pEditSpinItem=NULL;

}

CMoveLadderPage::~CMoveLadderPage()
{
}

void CMoveLadderPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMoveLadderPage)
	DDX_Control(pDX, IDC_LIST_LADDER, m_listLadder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMoveLadderPage, CPropertyPage)
	//{{AFX_MSG_MAP(CMoveLadderPage)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LADDER, OnClickListLadder)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LADDER, OnDblclkListLadder)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_LADDER, OnGetdispinfoListLadder)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LADDER, OnRclickListLadder)
	ON_COMMAND(ID_LADDER_ADD, OnLadderAdd)
	ON_COMMAND(ID_LADDER_INSERT, OnLadderInsert)
	ON_COMMAND(ID_LADDER_DELETE, OnLadderDelete)
	ON_COMMAND(ID_LADDER_STORELEFTHEADPOSITION, OnLadderStoreleftheadposition)
	ON_COMMAND(ID_LADDER_STORERIGHTHEADPOSITION, OnLadderStorerightheadposition)
	ON_COMMAND(ID_LADDER_EXECUTETHISLINE, OnLadderExecutethisline)
	ON_COMMAND(ID_LADDER_EXECUTEALL, OnLadderExecuteall)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,ItemChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMoveLadderPage message handlers
BOOL CMoveLadderPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CreateColumns();
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMoveLadderPage::CreateColumns()
{
	CString ColumnName[12] = { L"#"};
	int ColumnWidth[12] = { 0, 100, 220, 100, 220 };


	ColumnName[1].LoadString(IDS_Left_Action);
	ColumnName[2].LoadString(IDS_Coord);
	ColumnName[3].LoadString(IDS_Right_Action);
	ColumnName[4].LoadString(IDS_Coord);

	for (int ii=0;ii<5;ii++) {
		m_listLadder.InsertColumn(ii,ColumnName[ii], LVCFMT_LEFT,ColumnWidth[ii]);
	}


}

void CMoveLadderPage::FillList()
{
	CCoord Cp;
	CString Buff;

	if(theApp.m_pLadder == NULL) return;
	if(theApp.m_nLadderL == 0) return;

	m_listLadder.DeleteAllItems();
	for(int ii=0; ii<theApp.m_pLadder[0].m_nCoordL;ii++) {
		Buff.Format(L"%d",ii+1);
		m_listLadder.InsertItem(ii, Buff.GetBuffer(255), ii);
	}
	m_listLadder.EnsureVisible(0,FALSE);
	m_listLadder.SetItemState(0 , LVIS_SELECTED,LVIS_SELECTED);
	m_listLadder.SetFocus();
	m_listLadder.SetExtendedStyle( LVS_EX_FULLROWSELECT   );


}

BOOL CMoveLadderPage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnSetActive();
}

void CMoveLadderPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	FillList();

}

void CMoveLadderPage::OnClickListLadder(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SAFE_DELETE( m_pEditSpinItem );
	
	*pResult = 0;
}

void CMoveLadderPage::OnDblclkListLadder(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;
	CString Temp;
	CCoord Cp;

	m_listLadder.GetWindowRect(CtrlRect);
	m_listLadder.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	SAFE_DELETE( m_pEditSpinItem );

	if(theApp.m_pLadder != NULL) {

		if(pDispInfo->item.mask < (UINT)theApp.m_pLadder[0].m_nCoordL) {
			m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);
			m_nIndex=pDispInfo->item.mask;

			theApp.m_pLadder[0].GetCoord(m_nIndex,&Cp);
			m_strEdit.Empty();

			switch(m_nColumn=pDispInfo->item.iItem) {
			case 0:
				break;
			case 1:
				m_pEditSpinItem->Initialize(&theApp.m_pLadder[0].m_pCp[m_nIndex].nMoveMode[PORTSIDE],m_strAction,3);
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 2:
				m_strEdit.Empty();
				if(Cp.Side0.fX) { Temp.Format(L"X=%.1f ",Cp.Side0.fX);	m_strEdit += Temp; }
				if(Cp.Side0.fY) { Temp.Format(L"Y=%.1f ",Cp.Side0.fY);	m_strEdit += Temp; }
				if(Cp.Side0.fZ) { Temp.Format(L"Z=%.1f ",Cp.Side0.fZ);	m_strEdit += Temp; }
				switch(theApp.m_Tank.nScannerDescription) {
				default:
					if(Cp.Side0.fXt) { Temp.Format(L"Xt=%.2f ",Cp.Side0.fXt);	m_strEdit += Temp; }
					if(Cp.Side0.fYt) { Temp.Format(L"Yt=%.2f ",Cp.Side0.fYt);	m_strEdit += Temp; }
					break;
				case SINGLE_ROBOT:
				case DUAL_ROBOT:
				case DUAL_ROBOT_9_PLUS_9:
				case TANK_6AXIS:
				case TANK_5AXIS:
					Temp.Format(L"i=%.04f j=%.04f k=%.04f",Cp.Side0.fI,Cp.Side0.fJ,Cp.Side0.fK);	m_strEdit += Temp;
					break;
				}
				m_pEditSpinItem->Initialize(&m_strEdit);
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 3:
				m_pEditSpinItem->Initialize(&theApp.m_pLadder[0].m_pCp[m_nIndex].nMoveMode[STARBOARD],m_strAction,3);
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 4:
				m_strEdit.Empty();
				if(Cp.Side1.fX) { Temp.Format(L"X=%.1f ",Cp.Side1.fX);	m_strEdit += Temp; }
				if(Cp.Side1.fY) { Temp.Format(L"Y=%.1f ",Cp.Side1.fY);	m_strEdit += Temp; }
				if(Cp.Side1.fZ) { Temp.Format(L"Z=%.1f ",Cp.Side1.fZ);	m_strEdit += Temp; }
				switch(theApp.m_Tank.nScannerDescription) {
				default:
					if(Cp.Side1.fXt) { Temp.Format(L"Xt=%.2f ",Cp.Side1.fXt);	m_strEdit += Temp; }
					if(Cp.Side1.fYt) { Temp.Format(L"Yt=%.2f ",Cp.Side1.fYt);	m_strEdit += Temp; }
					break;
				case SINGLE_ROBOT:
				case DUAL_ROBOT:
				case DUAL_ROBOT_9_PLUS_9:
				case TANK_6AXIS:
				case TANK_5AXIS:
					Temp.Format(L"i=%.04f j=%.04f k=%.04f",Cp.Side1.fI,Cp.Side1.fJ,Cp.Side1.fK);	m_strEdit += Temp;
					break;
				}
				m_pEditSpinItem->Initialize(&m_strEdit);
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			}
		}
	}
	*pResult = 0;
}

void CMoveLadderPage::OnGetdispinfoListLadder(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	CString Buff,Temp;
	CCoord Cp;

	str[0]=0;
	if(theApp.m_pLadder != NULL) {

		if(pDispInfo->item.iItem < theApp.m_pLadder[0].m_nCoordL) {
			theApp.m_pLadder[0].GetCoord(pDispInfo->item.iItem,&Cp);

			switch(pDispInfo->item.iSubItem) {
			case 0:
				break;
			case 1:
				swprintf_s(str,100,_T("%s"), (LPCWSTR)m_strAction[Cp.nMoveMode[PORTSIDE]]);
				break;
			case 2:
				Buff.Empty();
				if(Cp.Side0.fX) { Temp.Format(L"X=%.0f ",Cp.Side0.fX);	Buff += Temp; }
				if(Cp.Side0.fY) { Temp.Format(L"Y=%.0f ",Cp.Side0.fY);	Buff += Temp; }
				if(Cp.Side0.fZ) { Temp.Format(L"Z=%.0f ",Cp.Side0.fZ);	Buff += Temp; }
				switch(theApp.m_Tank.nScannerDescription) {
				default:
					if(Cp.Side0.fXt) { Temp.Format(L"Xt=%.1f ",Cp.Side0.fXt);	Buff += Temp; }
					if(Cp.Side0.fYt) { Temp.Format(L"Yt=%.1f ",Cp.Side0.fYt);	Buff += Temp; }
					break;
				case SINGLE_ROBOT:
				case DUAL_ROBOT:
				case DUAL_ROBOT_9_PLUS_9:
				case TANK_6AXIS:
				case TANK_5AXIS:
					Temp.Format(L"i=%.04f j=%.04f k=%.04f",Cp.Side0.fI,Cp.Side0.fJ,Cp.Side0.fK);	Buff += Temp;
					break;
				}
				swprintf_s(str,100,L"%s",(LPCWSTR)Buff);
				break;
			case 3:
				swprintf_s(str,100,_T("%s"), (LPCWSTR)m_strAction[Cp.nMoveMode[STARBOARD]]);
				break;
			case 4:
				Buff.Empty();
				if(Cp.Side1.fX) { Temp.Format(L"X=%.0f ",Cp.Side1.fX);	Buff += Temp; }
				if(Cp.Side1.fY) { Temp.Format(L"Y=%.0f ",Cp.Side1.fY);	Buff += Temp; }
				if(Cp.Side1.fZ) { Temp.Format(L"Z=%.0f ",Cp.Side1.fZ);	Buff += Temp; }
				switch(theApp.m_Tank.nScannerDescription) {
				default:
					if(Cp.Side1.fXt) { Temp.Format(L"Xt=%.1f ",Cp.Side1.fXt);	Buff += Temp; }
					if(Cp.Side1.fYt) { Temp.Format(L"Yt=%.1f ",Cp.Side1.fYt);	Buff += Temp; }
					break;
				case SINGLE_ROBOT:
				case DUAL_ROBOT:
				case DUAL_ROBOT_9_PLUS_9:
				case TANK_6AXIS:
				case TANK_5AXIS:
					Temp.Format(L"i=%.04f j=%.04f k=%.04f",Cp.Side1.fI,Cp.Side1.fJ,Cp.Side1.fK);	Buff += Temp;
					break;
				}
				swprintf_s(str,100,L"%s", (LPCWSTR)Buff);
				break;
			}
		}

	}
	pDispInfo->item.pszText=str;

	
	*pResult = 0;
}




void CMoveLadderPage::OnRclickListLadder(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu *pPopup;
	CPoint	Point;
	
	POSITION pos = m_listLadder.GetFirstSelectedItemPosition();
	m_nIndex= m_listLadder.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_LADDER_MENU)) {
		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);
		delete menu;

	}
	
	*pResult = 0;
}

void CMoveLadderPage::OnLadderAdd() 
{
	CCoord Cp;

	if(theApp.m_pLadder == NULL) {
		theApp.m_pLadder = new CPolyCoord[ theApp.m_nLadderL = 1];
	}
	Cp.Zero();
	theApp.m_pLadder[theApp.m_nLadderL - 1].Add(Cp);
	FillList();
}

void CMoveLadderPage::OnLadderInsert() 
{
	CCoord Cp;
	Cp.Zero();

	if(theApp.m_pLadder == NULL) {
		theApp.m_pLadder = new CPolyCoord[ theApp.m_nLadderL = 1];
		theApp.m_pLadder[theApp.m_nLadderL - 1].Add(Cp);
	} else {
		theApp.m_pLadder[theApp.m_nLadderL - 1].InsertCoord(m_nIndex,Cp);
	}
	FillList();
}

void CMoveLadderPage::OnLadderDelete() 
{
	if(theApp.m_pLadder==NULL) return;

	if(theApp.m_pLadder[0].m_nCoordL == 1) {
		SAFE_DELETE_ARRAY( theApp.m_pLadder );
		theApp.m_nLadderL = 0;
	} else {
		theApp.m_pLadder[0].DeleteCoords(m_nIndex,m_nIndex);
	}

	FillList();

	
}


void CMoveLadderPage::OnLadderStoreleftheadposition() 
{
	CCoord CpHead,CpSurface;
	CpHead.Zero();

	if(theApp.m_pLadder == NULL) {
		theApp.m_pLadder = new CPolyCoord[ theApp.m_nLadderL = 1];
		theApp.m_pLadder[theApp.m_nLadderL - 1].Add(CpHead);
		m_nIndex=0;
		FillList();
	}
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Motors.GetHeadPos(&CpHead);
		CopyMemory(&theApp.m_pLadder[0].m_pCp[m_nIndex].Side0,&CpHead.Side0, sizeof SUBCOORD);
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case TANK_6AXIS:
	case TANK_5AXIS:
		theApp.m_Motors.GetSurfacePos(&CpSurface);
		CopyMemory(&theApp.m_pLadder[0].m_pCp[m_nIndex].Side0,&CpSurface.Side0, sizeof SUBCOORD);
		break;
	}

	theApp.m_pLadder[0].m_pCp[m_nIndex].nMoveMode[PORTSIDE] = 1;

	Invalidate(FALSE);
}

void CMoveLadderPage::OnLadderStorerightheadposition() 
{
	CCoord CpHead,CpSurface;
	CpHead.Zero();

	if(theApp.m_pLadder == NULL) {
		theApp.m_pLadder = new CPolyCoord[ theApp.m_nLadderL = 1];
		theApp.m_pLadder[theApp.m_nLadderL - 1].Add(CpHead);
		m_nIndex=0;
		FillList();
	}
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Motors.GetHeadPos(&CpHead);
		CopyMemory(&theApp.m_pLadder[0].m_pCp[m_nIndex].Side1,&CpHead.Side1, sizeof SUBCOORD);
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case TANK_6AXIS:
	case TANK_5AXIS:
		theApp.m_Motors.GetSurfacePos(&CpSurface);
		CopyMemory(&theApp.m_pLadder[0].m_pCp[m_nIndex].Side1,&CpSurface.Side1, sizeof SUBCOORD);
		break;
	}

	theApp.m_pLadder[0].m_pCp[m_nIndex].nMoveMode[STARBOARD] = 1;

	Invalidate(FALSE);
}

HRESULT CMoveLadderPage::ItemChanged(WPARAM,LPARAM)
{
	int nStart;
	CCoord Cp;
	CString Buff = m_strEdit;

	Buff.MakeUpper();
	Buff.Replace(L"XTIP",L"XT");
	Buff.Replace(L"YTIP",L"YT");
	Buff.Remove(' ');
	
	theApp.m_pLadder[0].GetCoord(m_nIndex,&Cp);

	switch(m_nColumn) {
	case 2: ZeroMemory(&Cp.Side0,sizeof Cp.Side0);
		if((nStart=Buff.Find(L"X=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"X=%f",&Cp.Side0.fX);
		if((nStart=Buff.Find(L"Y=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Y=%f",&Cp.Side0.fY);
		if((nStart=Buff.Find(L"Z=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Z=%f",&Cp.Side0.fZ);
		if((nStart=Buff.Find(L"XT=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"XT=%f",&Cp.Side0.fXt);
		if((nStart=Buff.Find(L"YT=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"YT=%f",&Cp.Side0.fYt);
		if((nStart=Buff.Find(L"I=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"I=%f",&Cp.Side0.fI);
		if((nStart=Buff.Find(L"J=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"J=%f",&Cp.Side0.fJ);
		if((nStart=Buff.Find(L"K=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"K=%f",&Cp.Side0.fK);
		break;
	case 4: ZeroMemory(&Cp.Side1,sizeof Cp.Side1);
		if((nStart=Buff.Find(L"X=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"X=%f",&Cp.Side1.fX);
		if((nStart=Buff.Find(L"Y=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Y=%f",&Cp.Side1.fY);
		if((nStart=Buff.Find(L"Z=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"Z=%f",&Cp.Side1.fZ);
		if((nStart=Buff.Find(L"XT=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"XT=%f",&Cp.Side1.fXt);
		if((nStart=Buff.Find(L"YT=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"YT=%f",&Cp.Side1.fYt);
		if((nStart=Buff.Find(L"I=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"I=%f",&Cp.Side1.fI);
		if((nStart=Buff.Find(L"J=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"J=%f",&Cp.Side1.fJ);
		if((nStart=Buff.Find(L"K=",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"K=%f",&Cp.Side1.fK);
		break;
	}
	theApp.m_pLadder[0].ModifyCoord(m_nIndex,Cp);
	return NULL;
}

void CMoveLadderPage::OnLadderExecutethisline() 
{
	CCoord CpHead, CpLadder, CpSurface;
	int ii,nSide;

	theApp.m_pLadder[0].GetCoord(m_nIndex, &CpLadder);


	switch (theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Motors.GetHeadPos(&CpHead);
		for(nSide=0;nSide<2;nSide++) {
			switch(	CpLadder.nMoveMode[nSide] ) {
			case LADDER_NO_ACTION:
				break;
			case LADDER_ABSOLUTE:
				for(ii=0;ii<5;ii++) {
					if(CpLadder.Side[nSide].fPos[ii] != 0.0f) CpHead.Side[nSide].fPos[ii] = CpLadder.Side[nSide].fPos[ii];
				}
				break;
			case LADDER_RELATIVE:
				for(ii=0;ii<5;ii++) {
					if(CpLadder.Side[nSide].fPos[ii] != 0.0f) CpHead.Side[nSide].fPos[ii] += CpLadder.Side[nSide].fPos[ii];
				}
				break;
			}
		};
		theApp.m_Motors.GotoHeadCoord(CpHead);
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case TANK_6AXIS:
	case TANK_5AXIS:
		theApp.m_Motors.GetSurfacePos(&CpSurface);
		for (nSide = 0; nSide<2; nSide++) {
			switch (CpLadder.nMoveMode[nSide]) {
			case LADDER_NO_ACTION:
				break;
			case LADDER_ABSOLUTE:
				for (ii = 0; ii<3; ii++) {
					if (CpLadder.Side[nSide].fPos[ii] != 0.0f) CpSurface.Side[nSide].fPos[ii] = CpLadder.Side[nSide].fPos[ii];
				}
				CpSurface.Side[nSide].norm = CpLadder.Side[nSide].norm;
				break;
			case LADDER_RELATIVE:
				for (ii = 0; ii<3; ii++) {
					if (CpLadder.Side[nSide].fPos[ii] != 0.0f) CpSurface.Side[nSide].fPos[ii] += CpLadder.Side[nSide].fPos[ii];
				}
				break;
			}
		};

		theApp.m_Motors.GotoSurfaceCoord(&CpSurface, 3);
		break;
	}
}

void CMoveLadderPage::OnLadderExecuteall() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nLadder = 0;
	theApp.m_Thread.m_nLadderExecuteSide=3;
	theApp.m_Thread.m_nThreadAction=EXECUTE_LADDER;
}
