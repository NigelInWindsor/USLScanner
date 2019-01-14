// TaughtLinesEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "TaughtLinesEditDlg.h"
#include "3DViewEditDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define	FILLLIST	0x01
#define	PAINT		0x02
#define	INVALIDATE	0x04

/////////////////////////////////////////////////////////////////////////////
// CTaughtLinesEditDlg dialog


CTaughtLinesEditDlg::CTaughtLinesEditDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nLeft, int nTop)
	: CDialog(CTaughtLinesEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaughtLinesEditDlg)
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pDlg = pDlg;
	
	m_nLeft = nLeft;
	m_nTop = nTop;
	m_pTaughtTools = NULL;
	m_nIndex=0;
	m_pEditSpinItem=NULL;
	m_pFlatPackToolsDlg = NULL;
}


void CTaughtLinesEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaughtLinesEditDlg)
	DDX_Control(pDX, IDC_CHECK_PEAKRX, m_checkPeakOpposite);
	DDX_Control(pDX, IDC_CHECK_MOVE_RIGHT, m_checkMoveRight);
	DDX_Control(pDX, IDC_CHECK_WP_0, m_checkWaterPath);
	DDX_Control(pDX, IDC_CHECK_NORMALIZE, m_checkNormalize);
	DDX_Control(pDX, IDC_COMBO_OPTOMIZE_WHAT, m_comboOptomizeWhat);
	DDX_Control(pDX, IDC_LIST_LINES, m_listLines);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaughtLinesEditDlg, CDialog)
	//{{AFX_MSG_MAP(CTaughtLinesEditDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(ID_TOOLS_TAUGHTLINES, OnToolsTaughtlines)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LINES, OnClickListLines)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_LINES, OnGetdispinfoListLines)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LINES, OnDblclkListLines)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LINES, OnRclickListLines)
	ON_BN_CLICKED(IDC_BUTTON_FLAT, OnButtonFlat)
	ON_CBN_SELCHANGE(IDC_COMBO_OPTOMIZE_WHAT, OnSelchangeComboOptomizeWhat)
	ON_BN_CLICKED(IDC_CHECK_MOVE_RIGHT, OnCheckMoveRight)
	ON_BN_CLICKED(IDC_CHECK_NORMALIZE, OnCheckNormalize)
	ON_BN_CLICKED(IDC_CHECK_PEAKRX, OnCheckPeakrx)
	ON_BN_CLICKED(IDC_CHECK_WP_0, OnCheckWp0)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_IT_SO, OnButtonMakeItSo)
	ON_COMMAND(ID_TAUGHT_ALLLINEAR, OnTaughtAlllinear)
	ON_COMMAND(ID_TAUGHT_ALLSPLINE, OnTaughtAllspline)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,ItemChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaughtLinesEditDlg message handlers

void CTaughtLinesEditDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CTaughtLinesEditDlg::OnClose() 
{

	
	CDialog::OnClose();
	DestroyWindow();
}

void CTaughtLinesEditDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CTaughtLinesEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;

	SetWindowPos( NULL , m_nLeft, m_nTop, 0, 0, SWP_NOSIZE);

	CWnd* pWnd;
	if(theApp.m_nSide0Orientation == 1) {
		pWnd = (CWnd*)GetDlgItem(IDC_CHECK_WP_0);
		Buff.LoadString(IDS_Water_Right);	pWnd->SetWindowText(Buff);
		pWnd = (CWnd*)GetDlgItem(IDC_CHECK_MOVE_RIGHT);
		Buff.LoadString(IDS_Left_to_Right);	pWnd->SetWindowText(Buff);
	}

	Buff.LoadString(IDS_Current);	m_comboOptomizeWhat.AddString(Buff);
	Buff.LoadString(IDS_Line);		m_comboOptomizeWhat.AddString(Buff);
	Buff.LoadString(IDS_All_Lines);	m_comboOptomizeWhat.AddString(Buff);


	UpdateAllControls();
	CreateColumns();
	FillList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaughtLinesEditDlg::UpdateAllControls()
{
	CString Buff;

	if(theApp.m_nLine>=PROFILE->m_nTaughtLineL) {
		theApp.m_nLine=PROFILE->m_nTaughtLineL-1;
	}
	if(theApp.m_nLine<0) theApp.m_nLine=0;

	FillList();

	m_comboOptomizeWhat.SetCurSel(theApp.m_LastSettings.nOptomiseOption);
	m_checkNormalize.SetCheck(theApp.m_LastSettings.nOptomiseNormals & 1);
	m_checkWaterPath.SetCheck(theApp.m_LastSettings.nOptomiseWp & 1);
	m_checkMoveRight.SetCheck(theApp.m_LastSettings.nOptomiseMoveRightToLeft & 1);
	m_checkPeakOpposite.SetCheck(theApp.m_LastSettings.nOptomiseOpposite);

}



void CTaughtLinesEditDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CString Buff;

/*
	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_LINE_NUMBER:
		nTemp = theApp.m_nLine;
		theApp.m_nLine=pSpin->GetPos()&0xffff;
		if(nTemp - theApp.m_nLine) {
			UpdateAllControls();
			UpdateParent(INVALIDATE|FILLLIST);
		}
		break;
	}
*/	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CTaughtLinesEditDlg::UpdateParent(int nAction)
{
	C3DViewEditDlg* pParent = (C3DViewEditDlg*)m_pParent;


	if(nAction & FILLLIST) pParent->FillList();
	if(nAction & PAINT) pParent->SendMessage(WM_PAINT);
	if(nAction & INVALIDATE) pParent->Invalidate(FALSE);

}





void CTaughtLinesEditDlg::ToolTaughtLinesFromParent()
{

	OnToolsTaughtlines();

}

void CTaughtLinesEditDlg::OnToolsTaughtlines() 
{
/*
	if (m_pTaughtTools == NULL) {
		m_pTaughtTools = new CTaughtToolsDlg(this, (CWnd**)&m_pTaughtTools);
		m_pTaughtTools->Create(CTaughtToolsDlg::IDD,this);
	} else {
		m_pTaughtTools->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}		
*/
}

void CTaughtLinesEditDlg::CreateColumns()
{
	int ColumnWidth[10] = { 16,60,40,100,100,50,50,50,80,50};
	CString ColumnName[10];
	ColumnName[0]= "#";
	ColumnName[1].LoadString(IDS_Style);
	ColumnName[2].LoadString(IDS_Step);

	int nColumnCount = m_listLines.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listLines.DeleteColumn(0);
	}
	for (int ii=0;ii<3;ii++) {
		m_listLines.InsertColumn(ii,ColumnName[ii], LVCFMT_LEFT,ColumnWidth[ii]);
	}
	m_listLines.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CTaughtLinesEditDlg::FillList()
{
	int	ii;
	CString Buff;

	if(m_listLines.GetItemCount() - PROFILE->m_nTaughtLineL) {
		m_listLines.DeleteAllItems();
		for (ii=0;ii<PROFILE->m_nTaughtLineL;ii++) {
			Buff.Format(_T("%d"),ii+1);
			m_listLines.InsertItem(ii, Buff.GetBuffer(255), ii);
		};
	}

	m_listLines.EnsureVisible(m_nIndex=theApp.m_nLine,FALSE);
	for(ii=0;ii<PROFILE->m_nTaughtLineL;ii++) 
		m_listLines.SetItemState(ii , 0,LVIS_SELECTED);
	m_listLines.SetItemState(theApp.m_nLine , LVIS_SELECTED,LVIS_SELECTED);
	m_listLines.SetFocus();

}

void CTaughtLinesEditDlg::OnClickListLines(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	ClearEditSpin();
	theApp.m_nLine = m_nIndex=pDispInfo->item.mask;
	UpdateParent(INVALIDATE|FILLLIST);
	
	*pResult = 0;
}

void CTaughtLinesEditDlg::OnGetdispinfoListLines(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	CString Style[4];
	Style[0].LoadString(IDS_Linear);
	Style[1].LoadString(IDS_Spline);
	Style[2].LoadString(IDS_Polynomial);
	Style[3].LoadString(IDS_Quadratic);

	if(PROFILE->m_TaughtLine==NULL) return;

	switch(pDispInfo->item.iSubItem) {
	case 0:
		break;
	case 1:	swprintf_s(str,100,_T("%s"),(LPCWSTR)Style[PROFILE->m_TaughtLine[pDispInfo->item.iItem].m_nStyle]);
		break;
	case 2:
		if(PROFILE->m_TaughtLine[pDispInfo->item.iItem].m_nLineIncrement<=0) {
			swprintf_s(str,_T(""));
		} else {
			swprintf_s(str,_T("%.01f"),PROFILE->m_fSlowIncrement * PROFILE->m_TaughtLine[pDispInfo->item.iItem].m_nLineIncrement);
		}
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}


void CTaughtLinesEditDlg::OnDblclkListLines(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;
	CString Style[4];
	Style[0].LoadString(IDS_Linear);
	Style[1].LoadString(IDS_Spline);
	Style[2].LoadString(IDS_Polynomial);
	Style[3].LoadString(IDS_Quadratic);
	CString Inc[10];
	Inc[0] = " ";
	for(int ii=1;ii<10;ii++) {
		Inc[ii].Format(_T("%.01f"),PROFILE->m_fSlowIncrement * ii);
	}

	m_listLines.GetWindowRect(CtrlRect);
	m_listLines.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	ClearEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

	switch(pDispInfo->item.iItem) {
	case 0:
		break;
	case 1: m_pEditSpinItem->Initialize(&PROFILE->m_TaughtLine[pDispInfo->item.mask].m_nStyle,Style,4);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	case 2:
		 m_pEditSpinItem->Initialize(&PROFILE->m_TaughtLine[pDispInfo->item.mask].m_nLineIncrement,Inc,10);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	}
	
	*pResult = 0;
}

void CTaughtLinesEditDlg::ClearEditSpin()
{
	if(m_pEditSpinItem!=NULL) {
		delete m_pEditSpinItem;
		m_pEditSpinItem=NULL;
	}

}

void CTaughtLinesEditDlg::OnRclickListLines(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	SAFE_DELETE( m_pEditSpinItem );
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;


	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_TAUGHT_LIST)) {
		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);
	};
	delete menu;
	

	*pResult = 0;
}

void CTaughtLinesEditDlg::OnButtonFlat() 
{
	if (m_pFlatPackToolsDlg == NULL) {
		m_pFlatPackToolsDlg = new CFLatPackToolsDlg(this, (CWnd**)&m_pFlatPackToolsDlg);
		m_pFlatPackToolsDlg->Create(CFLatPackToolsDlg::IDD,this);
	} else {
		m_pFlatPackToolsDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}		
}


HRESULT CTaughtLinesEditDlg::ItemChanged(WPARAM, LPARAM)
{
	UpdateParent(INVALIDATE|FILLLIST);

	return NULL;
}


void CTaughtLinesEditDlg::OnSelchangeComboOptomizeWhat() 
{
	theApp.m_LastSettings.nOptomiseOption = m_comboOptomizeWhat.GetCurSel();
}

void CTaughtLinesEditDlg::OnCheckMoveRight() 
{
	theApp.m_LastSettings.nOptomiseMoveRightToLeft = m_checkMoveRight.GetCheck();
}

void CTaughtLinesEditDlg::OnCheckNormalize() 
{
	theApp.m_LastSettings.nOptomiseNormals = m_checkNormalize.GetCheck();
}

void CTaughtLinesEditDlg::OnCheckPeakrx() 
{
	theApp.m_LastSettings.nOptomiseOpposite = m_checkPeakOpposite.GetCheck();
}

void CTaughtLinesEditDlg::OnCheckWp0() 
{
	theApp.m_LastSettings.nOptomiseWp = m_checkWaterPath.GetCheck();
}

void CTaughtLinesEditDlg::OnButtonMakeItSo() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	int nLine;

	for(nLine=0;nLine<PROFILE->m_nTaughtLineL;nLine++) {
		PROFILE->m_TaughtLine[nLine].SetAllModifiy(FALSE);
	}

	switch(theApp.m_LastSettings.nOptomiseOption) {
	case 0: PROFILE->m_TaughtLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].bModified = TRUE;
		break;
	case 1: PROFILE->m_TaughtLine[theApp.m_nLine].SetAllModifiy(TRUE);
		break;
	case 2:
		for(nLine=0;nLine<PROFILE->m_nTaughtLineL;nLine++) {
			PROFILE->m_TaughtLine[nLine].SetAllModifiy(TRUE);
		}
		break;
	}

	pFrame->StopCurrentTimerAction();
	pFrame->m_nTimerAction = RELEARN_LINE;
	
}

void CTaughtLinesEditDlg::OnTaughtAlllinear() 
{
	for(int nLine=0;nLine < PROFILE->m_nTaughtLineL; nLine++) {
		PROFILE->m_TaughtLine[nLine].m_nStyle = 0;
	}
	UpdateParent(INVALIDATE|FILLLIST);
	m_listLines.Invalidate(false);
	
}

void CTaughtLinesEditDlg::OnTaughtAllspline() 
{
	for(int nLine=0;nLine < PROFILE->m_nTaughtLineL; nLine++) {
		PROFILE->m_TaughtLine[nLine].m_nStyle = 1;
	}
	UpdateParent(INVALIDATE|FILLLIST);
	m_listLines.Invalidate(false);
}
