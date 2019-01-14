// FeatureEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "FeatureEditDlg.h"
#include "ReportPrintDlg1.h"
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeatureEditDlg dialog


CFeatureEditDlg::CFeatureEditDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg, FeatureStruct* pFeature,int* pnFeatureL)
	: CDialog(CFeatureEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeatureEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	m_pParent = pParent;
	
	m_nIndex = 0;
	m_pEditSpinItem = NULL;
	m_pFeature = pFeature;
	m_pnFeatureL = pnFeatureL;
}


void CFeatureEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeatureEditDlg)
	DDX_Control(pDX, IDC_LIST_FEATURE, m_listFeature);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeatureEditDlg, CDialog)
	//{{AFX_MSG_MAP(CFeatureEditDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_FEATURE, OnClickListFeature)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FEATURE, OnDblclkListFeature)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_FEATURE, OnGetdispinfoListFeature)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FEATURE, OnRclickListFeature)
	ON_COMMAND(ID_FEATURE_DELETE, OnFeatureDelete)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,TableChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeatureEditDlg message handlers

void CFeatureEditDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CFeatureEditDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CFeatureEditDlg::IDD]=FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CFeatureEditDlg::OnDestroy() 
{
	if (CFeatureEditDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CFeatureEditDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CFeatureEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	Buff;

	m_StaticPosList.Initialize(this, m_listFeature, TRUE, TRUE, TRUE, TRUE);

	if (CFeatureEditDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CFeatureEditDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}

	m_StaticPosList.SetNewPos();

	InitializeArrays();

	CreateColumns();
	FillList();

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFeatureEditDlg::UpdateAllControls()
{
	if(GetSafeHwnd() == NULL) return;
}

void CFeatureEditDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if(GetSafeHwnd() == NULL) return;
	m_StaticPosList.SetNewPos();	
}




void CFeatureEditDlg::CreateColumns()
{
	CString ColumnName[20] = { L"#"};
	int ColumnWidth[20] = { 20,80,80,50,50,50,50,70,70,70,70,70,70,70,70,70,70,70 };
	CRect rr;

	ColumnName[1].LoadString(IDS_Feature_Type);
	ColumnName[2].LoadString(IDS_Feature_Variable);
	ColumnName[3].LoadString(IDS_Feature_Top);
	ColumnName[4].LoadString(IDS_Feature_Left);
	ColumnName[5].LoadString(IDS_Feature_Bottom);
	ColumnName[6].LoadString(IDS_Feature_Right);
	ColumnName[7].LoadString(IDS_Feature_Caption);
	ColumnName[8].LoadString(IDS_Feature_Font_Size);
	ColumnName[9].LoadString(IDS_Feature_Color);
	ColumnName[10].LoadString(IDS_Feature_Width);
	ColumnName[11].LoadString(IDS_Feature_Page);


	m_listFeature.GetWindowRect(&rr);
	rr.right-=22;

	// Delete all of the columns.
	int nColumnCount = m_listFeature.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listFeature.DeleteColumn(0);
	}

	for(int nn=0;nn<12;nn++) {
		switch(nn) {
		default: m_listFeature.InsertColumn(nn,ColumnName[nn], LVCFMT_RIGHT,ColumnWidth[nn]);
			break;
		case 2:
		case 7: m_listFeature.InsertColumn(nn,ColumnName[nn], LVCFMT_LEFT,ColumnWidth[nn]);
			break;
		}
	}

	m_listFeature.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CFeatureEditDlg::FillList()
{
	CString	Buff;

	m_listFeature.DeleteAllItems();
	for (int ii=0;ii<*m_pnFeatureL;ii++) {
		Buff.Format((CString)"%d",ii+1);
		m_listFeature.InsertItem(ii, Buff.GetBuffer(255), ii);
	};
	m_listFeature.EnsureVisible(m_nIndex,FALSE);
	m_listFeature.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listFeature.SetFocus();


}


void CFeatureEditDlg::OnClickListFeature(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CloseEditSpin();
	
	*pResult = 0;
}

void CFeatureEditDlg::OnDblclkListFeature(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	m_listFeature.GetWindowRect(CtrlRect);
	m_listFeature.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	CloseEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**)&m_pEditSpinItem,&Rect);

	m_nIndex = pDispInfo->item.mask;
	CReportPrintDlg* pDlg = (CReportPrintDlg*)m_pParent;
	pDlg->RectToMM(m_pFeature[pDispInfo->item.mask].rr,&m_fTop,&m_fLeft,&m_fBottom,&m_fRight);

	switch(pDispInfo->item.iItem) {
	case 3:	m_pEditSpinItem->Initialize(&m_fTop,0.0f,1400.0f,0.1f,_T("%.01f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	case 4:	m_pEditSpinItem->Initialize(&m_fLeft,0.0f,1400.0f,0.1f,_T("%.01f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	case 5:	m_pEditSpinItem->Initialize(&m_fBottom,0.0f,1400.0f,0.1f,_T("%.01f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	case 6:	m_pEditSpinItem->Initialize(&m_fRight,0.0f,1400.0f,0.1f,_T("%.01f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	case 7:	m_pEditSpinItem->Initialize(m_pFeature[pDispInfo->item.mask].pCaption);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	case 8:	m_pEditSpinItem->Initialize((int*)(&m_pFeature[pDispInfo->item.mask].lf.lfHeight),-50,50,1);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	case 11:	m_pEditSpinItem->Initialize((int*)(&m_pFeature[pDispInfo->item.mask].nPageNumber),0,20,1);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	}
	*pResult = 0;
}

void CFeatureEditDlg::OnRclickListFeature(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CloseEditSpin();
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	m_nIndex=pDispInfo->item.mask;
	CMenu* menu = (CMenu *) new CMenu;
	CPoint	Point;

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_FEATURE_MENU)) {
		CMenu* pPopup = menu->GetSubMenu(0);
		ASSERT(pPopup != NULL);
				
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;

	*pResult = 0;
}


void CFeatureEditDlg::CloseEditSpin()
{
	CReportPrintDlg* pDlg = (CReportPrintDlg*)m_pParent;

	if(m_pEditSpinItem!=NULL) {
		m_pEditSpinItem->SendMessage(WM_CLOSE);
		m_pEditSpinItem=NULL;
		pDlg->Invalidate(FALSE);
	}


}

void CFeatureEditDlg::OnGetdispinfoListFeature(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	float fTop,fLeft,fBottom,fRight;
	CReportPrintDlg* pDlg = (CReportPrintDlg*)m_pParent;

	pDlg->RectToMM(m_pFeature[pDispInfo->item.iItem].rr,&fTop,&fLeft,&fBottom,&fRight);

	str[0]=0;
	switch(pDispInfo->item.iSubItem) {
	case 0:	swprintf_s(str,100,_T("%d"),pDispInfo->item.iItem+1);
		break;
	case 1:	swprintf_s(str,100,_T("%s"),(LPCWSTR)m_Type[m_pFeature[pDispInfo->item.iItem].nType]);
		break;
	case 2:
		if(m_pFeature[pDispInfo->item.iItem].nType==REPORT_DATA_VALUE) {
			swprintf_s(str,100,_T("%s"),(LPCWSTR)m_DataValueType[m_pFeature[pDispInfo->item.iItem].nDataValueType]);
		}
		break;
	case 3:	swprintf_s(str,_T("%.01f"),fTop );
		break;
	case 4:	swprintf_s(str,_T("%.01f"),fLeft );
		break;
	case 5:
		if((m_pFeature[pDispInfo->item.iItem].nType!=REPORT_DATA_VALUE) && (m_pFeature[pDispInfo->item.iItem].nType!=REPORT_TEXT)) {
			swprintf_s(str,_T("%.01f"),fBottom );
		}
		break;
	case 6:
		if((m_pFeature[pDispInfo->item.iItem].nType!=REPORT_DATA_VALUE) && (m_pFeature[pDispInfo->item.iItem].nType!=REPORT_TEXT)) {
			swprintf_s(str,_T("%.01f"),fRight );
		}
		break;
	case 7:
		if((m_pFeature[pDispInfo->item.iItem].nType==REPORT_TEXT) || (m_pFeature[pDispInfo->item.iItem].nType==REPORT_DATA_VALUE)) {
			swprintf_s(str,_T("%s"),m_pFeature[pDispInfo->item.iItem].pCaption->GetBuffer(100));
		}
		break;
	case 8:	swprintf_s(str,_T("%d"),m_pFeature[pDispInfo->item.iItem].lf.lfHeight);
		break;
	case 9:	swprintf_s(str,_T("%x"),m_pFeature[pDispInfo->item.iItem].rgbLine);
		break;
	case 11:	swprintf_s(str,_T("%x"),m_pFeature[pDispInfo->item.iItem].nPageNumber + 1);
		break;
	}
	pDispInfo->item.pszText=str;	
	*pResult = 0;
}

void CFeatureEditDlg::InitializeArrays()
{

	m_Type[0].Empty();
	m_Type[REPORT_TEXT].LoadString(IDS_Report_Text);
	m_Type[REPORT_IMAGE].LoadString(IDS_Report_Image);
	m_Type[REPORT_RECTANGLE].LoadString(IDS_Report_Rectangle);
	m_Type[REPORT_LINE].LoadString(IDS_Report_Line);
	m_Type[REPORT_DATE].LoadString(IDS_Report_Date);
	m_Type[REPORT_LIST].LoadString(IDS_Report_List);
	m_Type[REPORT_BARGRAPH].LoadString(IDS_Report_BarGraph);
	m_Type[REPORT_DATA_VALUE].LoadString(IDS_Report_Data_Value);
	m_Type[REPORT_TABLE].LoadString(IDS_Table);

	for(int nn=0;nn<300;nn++) {
		switch(nn) {
		case REPORT_CUSTOMER_DETAILS: m_DataValueType[nn] = _T("Customer");
			break;
		case REPORT_CONTRACT_DETAILS: m_DataValueType[nn] = _T("Contract");
			break;
		case	REPORT_SPECIFICATION_DETAILS: m_DataValueType[nn] = _T("Specification");
			break;
		case	REPORT_ITEM_DETAILS: m_DataValueType[nn] = _T("Item");
			break;
		case	REPORT_TECHNIQUE_DETAILS: m_DataValueType[nn] = _T("Technique");
			break;
		case	REPORT_LOT_DETAILS: m_DataValueType[nn] = _T("Lot");
			break;
		case	REPORT_ALLOY_DETAILS: m_DataValueType[nn] = _T("Alloy");
			break;
		case REPORT_HEAT_NO_DETAILS: m_DataValueType[nn] = _T("Heat");
			break;
		case REPORT_ITEM_CODE_DETAILS: m_DataValueType[nn] = _T("Item Code");
			break;
		case REPORT_NUMBER_ITEMS_IN_LOT_DETAILS: m_DataValueType[nn] = _T("Number of Items");
			break;
		case REPORT_SIZE_DETAILS:m_DataValueType[nn] = _T("Size x X Y");
			break;
		case PROBE_SERIAL_0:
		case PROBE_SERIAL_1:
		case PROBE_SERIAL_2:
		case PROBE_SERIAL_3:
		case PROBE_SERIAL_4:
		case PROBE_SERIAL_5:
		case PROBE_SERIAL_6:
		case PROBE_SERIAL_7:
			m_DataValueType[nn].Format(_T("Serial No %d"),nn-PROBE_SERIAL_0+1);
			break;
		case PROBE_FREQ_0:
		case PROBE_FREQ_1:
		case PROBE_FREQ_2:
		case PROBE_FREQ_3:
		case PROBE_FREQ_4:
		case PROBE_FREQ_5:
		case PROBE_FREQ_6:
		case PROBE_FREQ_7:
			m_DataValueType[nn].Format(_T("Frequency %d"),nn-PROBE_FREQ_0+1);
			break;
		case PROBE_LENGTH_0:
		case PROBE_LENGTH_1:
		case PROBE_LENGTH_2:
		case PROBE_LENGTH_3:
		case PROBE_LENGTH_4:
		case PROBE_LENGTH_5:
		case PROBE_LENGTH_6:
		case PROBE_LENGTH_7:
			m_DataValueType[nn].Format(_T("Focal Length %d"),nn-PROBE_LENGTH_0+1);
			break;
		case PROBE_DIA_0:
		case PROBE_DIA_1:
		case PROBE_DIA_2:
		case PROBE_DIA_3:
		case PROBE_DIA_4:
		case PROBE_DIA_5:
		case PROBE_DIA_6:
		case PROBE_DIA_7:
			m_DataValueType[nn].Format(_T("Diameter %d"),nn-PROBE_DIA_0+1);
			break;
		case PROBE_SPOT_0:
		case PROBE_SPOT_1:
		case PROBE_SPOT_2:
		case PROBE_SPOT_3:
		case PROBE_SPOT_4:
		case PROBE_SPOT_5:
		case PROBE_SPOT_6:
		case PROBE_SPOT_7:
			m_DataValueType[nn].Format(_T("Focal Size %d"),nn-PROBE_SPOT_0+1);
			break;
		case REPORT_DEPTH_0:
		case REPORT_DEPTH_1:
		case REPORT_DEPTH_2:
		case REPORT_DEPTH_3:
		case REPORT_DEPTH_4:
		case REPORT_DEPTH_5:
		case REPORT_DEPTH_6:
		case REPORT_DEPTH_7:
			m_DataValueType[nn].Format(_T("Hole Depth %d"),nn-REPORT_DEPTH_0+1);
			break;
		case REPORT_AMP_AT_DEPTH_0:
		case REPORT_AMP_AT_DEPTH_0+1:
		case REPORT_AMP_AT_DEPTH_0+2:
		case REPORT_AMP_AT_DEPTH_0+3:
		case REPORT_AMP_AT_DEPTH_0+4:
		case REPORT_AMP_AT_DEPTH_0+5:
		case REPORT_AMP_AT_DEPTH_0+6:
		case REPORT_AMP_AT_DEPTH_0+7:
			m_DataValueType[nn].Format(_T("Amp at Depth 1 %d"),nn-REPORT_AMP_AT_DEPTH_0+1);
			break;
		case REPORT_AMP_AT_DEPTH_1:
		case REPORT_AMP_AT_DEPTH_1+1:
		case REPORT_AMP_AT_DEPTH_1+2:
		case REPORT_AMP_AT_DEPTH_1+3:
		case REPORT_AMP_AT_DEPTH_1+4:
		case REPORT_AMP_AT_DEPTH_1+5:
		case REPORT_AMP_AT_DEPTH_1+6:
		case REPORT_AMP_AT_DEPTH_1+7:
			m_DataValueType[nn].Format(_T("Amp at Depth 2 %d"),nn-REPORT_AMP_AT_DEPTH_1+1);
			break;
		case REPORT_AMP_AT_DEPTH_2:
		case REPORT_AMP_AT_DEPTH_2+1:
		case REPORT_AMP_AT_DEPTH_2+2:
		case REPORT_AMP_AT_DEPTH_2+3:
		case REPORT_AMP_AT_DEPTH_2+4:
		case REPORT_AMP_AT_DEPTH_2+5:
		case REPORT_AMP_AT_DEPTH_2+6:
		case REPORT_AMP_AT_DEPTH_2+7:
			m_DataValueType[nn].Format(_T("Amp at Depth 3 %d"),nn-REPORT_AMP_AT_DEPTH_2+1);
			break;
		case REPORT_NOMINAL_GAIN:
		case REPORT_NOMINAL_GAIN+1:
		case REPORT_NOMINAL_GAIN+2:
		case REPORT_NOMINAL_GAIN+3:
		case REPORT_NOMINAL_GAIN+4:
		case REPORT_NOMINAL_GAIN+5:
		case REPORT_NOMINAL_GAIN+6:
		case REPORT_NOMINAL_GAIN+7:
			m_DataValueType[nn].Format(_T("Nominal Gain %d"),nn-REPORT_NOMINAL_GAIN+1);
			break;
		case REPORT_COMPENSATION_GAIN:
		case REPORT_COMPENSATION_GAIN+1:
		case REPORT_COMPENSATION_GAIN+2:
		case REPORT_COMPENSATION_GAIN+3:
		case REPORT_COMPENSATION_GAIN+4:
		case REPORT_COMPENSATION_GAIN+5:
		case REPORT_COMPENSATION_GAIN+6:
		case REPORT_COMPENSATION_GAIN+7:
			m_DataValueType[nn].Format(_T("Compensation Gain %d"),nn-REPORT_COMPENSATION_GAIN+1);
			break;
		case REPORT_INSPECTION_GAIN:
		case REPORT_INSPECTION_GAIN+1:
		case REPORT_INSPECTION_GAIN+2:
		case REPORT_INSPECTION_GAIN+3:
		case REPORT_INSPECTION_GAIN+4:
		case REPORT_INSPECTION_GAIN+5:
		case REPORT_INSPECTION_GAIN+6:
		case REPORT_INSPECTION_GAIN+7:
			m_DataValueType[nn].Format(_T("Inspection Gain %d"),nn-REPORT_INSPECTION_GAIN+1);
			break;
		}
	}
}


void CFeatureEditDlg::SetIndex(int nIndex)
{
	for(int nn=0;nn<*m_pnFeatureL;nn++) {
		m_listFeature.SetItemState(nn , NULL,LVIS_SELECTED);
	}
	if(m_pEditSpinItem!=NULL) {
		m_pEditSpinItem->SendMessage(WM_CLOSE);
		m_pEditSpinItem=NULL;
	}
	m_nIndex = nIndex;
	m_listFeature.EnsureVisible(m_nIndex,FALSE);
	m_listFeature.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listFeature.SetFocus();
}

HRESULT CFeatureEditDlg::TableChanged(WPARAM, LPARAM)
{
	CReportPrintDlg* pDlg = (CReportPrintDlg*)m_pParent;
	pDlg->MMtoCoords(m_fTop,m_fLeft,m_fBottom,m_fRight,&m_pFeature[m_nIndex].rr);

	pDlg->Invalidate(FALSE);
	return NULL;
}


void CFeatureEditDlg::OnFeatureDelete() 
{
	
	CReportPrintDlg* pDlg = (CReportPrintDlg*)m_pParent;

	pDlg->DeleteFeature(m_nIndex);
	pDlg->Invalidate(FALSE);
	if(*m_pnFeatureL >= m_nIndex) {
		m_nIndex = *m_pnFeatureL-1;
		FillList();
	}
}
