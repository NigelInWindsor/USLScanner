// CADTableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "CADTableDlg.h"
#include "ViewPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCADTableDlg dialog


CCADTableDlg::CCADTableDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,CData* pData)
	: CDialog(CCADTableDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCADTableDlg)
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_pData = pData;
	
	m_pEditSpinItem = NULL;

	m_CADElement.Zero();

}


void CCADTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCADTableDlg)
	DDX_Control(pDX, IDC_CHECK_LINE_TEXT, m_checkLineText);
	DDX_Control(pDX, IDC_CHECK_FINISH_ARROW, m_checkFinishArrow);
	DDX_Control(pDX, IDC_CHECK_START_ARROW, m_checkStartArrow);
	DDX_Control(pDX, IDC_SPIN_LINE_WIDTH, m_spinLineWidth);
	DDX_Control(pDX, IDC_EDIT_LINE_WIDTH, m_editLineWidth);
	DDX_Control(pDX, IDC_COMBO_DEFECT_MODE, m_comboDefectMode);
	DDX_Control(pDX, IDC_EDIT_FONT_SIZE, m_editFontSize);
	DDX_Control(pDX, IDC_STATIC_LINE_COLOR, m_staticLineColor);
	DDX_Control(pDX, IDC_SPIN_FONT_SIZE, m_spinFontSize);
	DDX_Control(pDX, IDC_BUTTON_FONT, m_buttonFont);
	DDX_Control(pDX, IDC_LIST_TABLE, m_listTable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCADTableDlg, CDialog)
	//{{AFX_MSG_MAP(CCADTableDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TABLE, OnGetdispinfoListTable)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TABLE, OnClickListTable)
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_CAD_LENGTH, OnButtonCadLength)
	ON_COMMAND(ID_BUTTON_CAD_WIDTH, OnButtonCadWidth)
	ON_COMMAND(ID_BUTTON_CAD_HEIGHT, OnButtonCadHeight)
	ON_COMMAND(ID_BUTTON_CAD_AREA, OnButtonCadArea)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	ON_EN_CHANGE(IDC_EDIT_FONT_SIZE, OnChangeEditFontSize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FONT_SIZE, OnDeltaposSpinFontSize)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_TABLE, OnRclickListTable)
	ON_COMMAND(ID_CAD_COPY, OnCadCopy)
	ON_COMMAND(ID_CAD_PASTE, OnCadPaste)
	ON_COMMAND(ID_CAD_DELETE, OnCadDelete)
	ON_COMMAND(ID_BUTTON_CAD_REF_LEVEL, OnButtonCadRefLevel)
	ON_COMMAND(ID_BUTTON_CAD_DEFECT_LEVEL, OnButtonCadDefectLevel)
	ON_COMMAND(ID_BUTTON_CAD_DEFECT_AREA, OnButtonCadDefectArea)
	ON_COMMAND(ID_CAD_COPYTOCLIPBOARD, OnCadCopytoclipboard)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TABLE, OnDblclkListTable)
	ON_COMMAND(ID_BUTTON_VERBOSE_TABLE, OnButtonVerboseTable)
	ON_COMMAND(ID_BUTTON_COLUMN_TABLE, OnButtonColumnTable)
	ON_COMMAND(ID_BUTTON_CAD_DEFECT_HEIGHT, OnButtonCadDefectHeight)
	ON_COMMAND(ID_BUTTON_CAD_DEFECT_WIDTH, OnButtonCadDefectWidth)
	ON_CBN_SELCHANGE(IDC_COMBO_DEFECT_MODE, OnSelchangeComboDefectMode)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LINE_WIDTH, OnDeltaposSpinLineWidth)
	ON_EN_CHANGE(IDC_EDIT_LINE_WIDTH, OnChangeEditLineWidth)
	ON_WM_CHARTOITEM()
	ON_BN_CLICKED(IDC_CHECK_START_ARROW, OnCheckStartArrow)
	ON_BN_CLICKED(IDC_CHECK_FINISH_ARROW, OnCheckFinishArrow)
	ON_BN_CLICKED(IDC_CHECK_LINE_TEXT, OnCheckLineText)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_MESSAGE(UI_ITEMCHANGED,TableChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCADTableDlg message handlers
int CCADTableDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndPropertiesToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndPropertiesToolBar.LoadToolBar(IDR_CAD_PROPERTIES_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create properties ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()+210,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndPropertiesToolBar);
	
	return 0;
}

void CCADTableDlg::PostNcDestroy() 
{

	CDialog::PostNcDestroy();
	delete this;
}

void CCADTableDlg::OnClose() 
{
	InvalidateParent();
	theApp.m_LastSettings.bWindowOpenOnLastExit[CCADTableDlg::IDD]=FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CCADTableDlg::OnDestroy() 
{
	if (CCADTableDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CCADTableDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CCADTableDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	Buff;

	m_StaticPosTable.Initialize(this, m_listTable, TRUE, TRUE, TRUE, TRUE);

	if (CCADTableDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CCADTableDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
		}
	}
	m_StaticPosTable.SetNewPos();
	
	m_spinFontSize.SetRange(0,72);
	m_spinLineWidth.SetRange(1,20);

	Buff.LoadString(IDS_Transparent);	m_comboDefectMode.AddString(Buff);
	Buff.LoadString(IDS_Paint);			m_comboDefectMode.AddString(Buff);
	Buff.LoadString(IDS_Rectangle);		m_comboDefectMode.AddString(Buff);

	

	CreateColumns();
	FillList();
	SetToolBarCheckedState();

	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCADTableDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_StaticPosTable.SetNewPos();
	
}
void CCADTableDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString Buff;
	int nFlags;


	if(m_pData->IsCADElement() == false) {

		Buff.Format(L"%s",theApp.m_LastSettings.lfCAD.lfFaceName);
		m_buttonFont.SetWindowText(Buff);

		Buff.Format(L"%d",pFrame->PointSizeFromHeight(theApp.m_LastSettings.lfCAD.lfHeight));
		m_editFontSize.SetWindowText(Buff);

		m_spinFontSize.SetPos(pFrame->PointSizeFromHeight(theApp.m_LastSettings.lfCAD.lfHeight));

		m_comboDefectMode.SetCurSel(theApp.m_LastSettings.nCADDefectPaintMode);

		Buff.Format(L"%d",theApp.m_LastSettings.nCADLineWidth);
		m_editLineWidth.SetWindowText(Buff);

		m_spinLineWidth.SetPos(theApp.m_LastSettings.nCADLineWidth);

		m_checkStartArrow.SetCheck(theApp.m_LastSettings.nCADFlags & CAD_START_ARROW);
		m_checkFinishArrow.SetCheck(theApp.m_LastSettings.nCADFlags & CAD_FINISH_ARROW);
		m_checkLineText.SetCheck(theApp.m_LastSettings.nCADFlags & CAD_LINE_TEXT);


	} else {

		Buff.Format(L"%s",m_pData->m_pCADElement[m_pData->GetCADElement()].m_lf.lfFaceName);
		m_buttonFont.SetWindowText(Buff);

		Buff.Format(L"%d",pFrame->PointSizeFromHeight(m_pData->m_pCADElement[m_pData->GetCADElement()].m_lf.lfHeight));
		m_editFontSize.SetWindowText(Buff);

		m_spinFontSize.SetPos(pFrame->PointSizeFromHeight(m_pData->m_pCADElement[m_pData->GetCADElement()].m_lf.lfHeight));

		m_comboDefectMode.SetCurSel(m_pData->m_pCADElement[m_pData->GetCADElement()].m_nCADDefectPaintMode);

		Buff.Format(L"%d",m_pData->m_pCADElement[m_pData->GetCADElement()].m_nLineWidth);
		m_editLineWidth.SetWindowText(Buff);

		m_spinLineWidth.SetPos(m_pData->m_pCADElement[m_pData->GetCADElement()].m_nLineWidth);

		m_pData->GetCADElementFlags(&nFlags);
		m_checkStartArrow.SetCheck(nFlags & CAD_START_ARROW);
		m_checkFinishArrow.SetCheck(nFlags & CAD_FINISH_ARROW);
		m_checkLineText.SetCheck(nFlags & CAD_LINE_TEXT);
	}

	SetToolBarCheckedState();

}

void CCADTableDlg::CreateColumns()
{
	int ColumnWidth0[20] = { 0,50,470 };
	int ColumnWidth1[20] = { 0,70,70,70,70,70,70,70,70,70,70,70 };
	CString ColumnName[20];
	int ii;

	ColumnName[0]= "#";
	ColumnName[1].LoadString(IDS_Title);

	int nColumnCount = m_listTable.GetHeaderCtrl()->GetItemCount();
	for (ii=0;ii < nColumnCount;ii++) {
		m_listTable.DeleteColumn(0);
	}

	switch(	theApp.m_LastSettings.nCADTableDisplayMode ) {
	case 0:
		ColumnName[2].LoadString(IDS_Description);
		for (ii=0;ii<3;ii++) m_listTable.InsertColumn(ii,ColumnName[ii], LVCFMT_LEFT,ColumnWidth0[ii]);
		break;
	case 1:
		ColumnName[2].LoadString(IDS_Length);
		ColumnName[3].LoadString(IDS_Width);
		ColumnName[4].LoadString(IDS_Height);
		ColumnName[5].LoadString(IDS_Area);
		ColumnName[6].LoadString(IDS_Ref_Level);
		ColumnName[7].LoadString(IDS_Defect_Level);
		ColumnName[8].LoadString(IDS_Defect_Area);
		ColumnName[9].LoadString(IDS_Defect_Width);
		ColumnName[10].LoadString(IDS_Defect_Height);
		for (ii=0;ii<11;ii++) m_listTable.InsertColumn(ii,ColumnName[ii], LVCFMT_LEFT,ColumnWidth1[ii]);
		break;
	}
	m_listTable.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CCADTableDlg::FillList()
{
	CString Buff;

	if(m_listTable.GetItemCount() != m_pData->m_nCADElementL) {
		m_listTable.DeleteAllItems();
		for (int ii=0;ii<m_pData->m_nCADElementL;ii++) {
			Buff.Format(_T("%d"),ii+1);
			m_listTable.InsertItem(ii, Buff.GetBuffer(255), ii);
		};
	}


//	m_listTable.EnsureVisible(m_nIndex,FALSE);
//	for(ii=0;ii<m_pData->m_nCADElementL;ii++) 
//		m_listTable.SetItemState(ii , 0,LVIS_SELECTED);
//	m_listTable.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
//	m_listTable.SetFocus();

}

void CCADTableDlg::OnGetdispinfoListTable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	CString Buff;
	int nn;

	if(m_pData->m_nCADElementL<=0) return;
	nn = pDispInfo->item.iItem;
	str[0]=0;

	switch(	theApp.m_LastSettings.nCADTableDisplayMode ) {
	case 0:
		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:
			swprintf_s(str,_T("%s"),m_pData->m_pCADElement[nn].m_wText);
			break;
		case 2:m_pData->GetCADVerboseStats(nn,&Buff);
			Buff.Replace(L"\t",L" ");
			swprintf_s(str,100,_T("%s"),(LPCWSTR)Buff);
			break;
		}
		break;
	case 1:
		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:
			swprintf_s(str,_T("%s"),m_pData->m_pCADElement[nn].m_wText);
			break;
		case 2:
			if(m_pData->m_pCADElement[nn].m_nFlags & CAD_LENGTH) {
				m_pData->GetCADElementLength(nn,&Buff);
				swprintf_s(str,100,_T("%s"), (LPCWSTR)Buff);
			}
			break;
		case 3:
			if(m_pData->m_pCADElement[nn].m_nFlags & CAD_WIDTH) {
				m_pData->GetCADElementWidth(nn,&Buff);
				swprintf_s(str,100,_T("%s"), (LPCWSTR)Buff);
			}
			break;
		case 4:
			if(m_pData->m_pCADElement[nn].m_nFlags & CAD_HEIGHT) {
				m_pData->GetCADElementHeight(nn,&Buff);
				swprintf_s(str,100,_T("%s"), (LPCWSTR)Buff);
			}
			break;
		case 5:
			if(m_pData->m_pCADElement[nn].m_nFlags & CAD_TOTAL_AREA) {
				m_pData->GetCADElementArea(nn,&Buff);
				swprintf_s(str,100,_T("%s"), (LPCWSTR)Buff);
			}
			break;
		case 6:
			if(m_pData->m_pCADElement[nn].m_nFlags & CAD_REF_LEVEL) {
				m_pData->GetHistogramRefLevel(&m_pData->m_pCADElement[nn].m_Hist,&Buff,false);
				swprintf_s(str,100,_T("%s"), (LPCWSTR)Buff);
			}
			break;
		case 7:
			if(m_pData->m_pCADElement[nn].m_nFlags & CAD_DEFECT_LEVEL) {
				m_pData->GetHistogramDefectLevel(&m_pData->m_pCADElement[nn].m_Hist,&Buff,false);
				swprintf_s(str,100,_T("%s"), (LPCWSTR)Buff);
			}
			break;
		case 8:
			if(m_pData->m_pCADElement[nn].m_nFlags & CAD_DEFECT_AREA) {
				m_pData->GetHistogramDefectArea(&m_pData->m_pCADElement[nn].m_Hist,&Buff,false);
				swprintf_s(str,100,_T("%s"), (LPCWSTR)Buff);
			}
			break;
		case 9:
			if(m_pData->m_pCADElement[nn].m_nFlags & CAD_DEFECT_WIDTH) {
				m_pData->GetHistogramDefectWidth(&m_pData->m_pCADElement[nn].m_Hist,&Buff,false);
				swprintf_s(str,100,_T("%s"), (LPCWSTR)Buff);
			}
			break;
		case 10:
			if(m_pData->m_pCADElement[nn].m_nFlags & CAD_DEFECT_HEIGHT) {
				m_pData->GetHistogramDefectHeight(&m_pData->m_pCADElement[nn].m_Hist,&Buff,false);
				swprintf_s(str,100,_T("%s"), (LPCWSTR)Buff);
			}
			break;
		}
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CCADTableDlg::OnClickListTable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if(m_pData->m_nCADElementL>0) {
		m_pData->m_nCADPtr=pDispInfo->item.mask;
		if(m_pData->m_nCADPtr<0) m_pData->m_nCADPtr=0;
		if(m_pData->m_nCADPtr>=m_pData->m_nCADElementL) m_pData->m_nCADPtr=m_pData->m_nCADElementL-1;
		InvalidateParent();
		UpdateAllControls();
		Invalidate(false);
	}

	
	*pResult = 0;
}



BOOL CCADTableDlg::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
   LRESULT *pResult)
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
   case ID_BUTTON_CAD_LENGTH: strTipText.LoadString(IDS_Length_Circumference);
	   break;
   case ID_BUTTON_CAD_WIDTH: strTipText.LoadString(IDS_Width);
	   break;
   case ID_BUTTON_CAD_HEIGHT: strTipText.LoadString(IDS_Height);
	   break;
   case ID_BUTTON_CAD_AREA: strTipText.LoadString(IDS_Area);
	   break;
   case ID_BUTTON_CAD_REF_LEVEL: strTipText.LoadString(IDS_Ref_Level);
	   break;
   case ID_BUTTON_CAD_DEFECT_LEVEL: strTipText.LoadString(IDS_Defect_Level);
	   break;
   case ID_BUTTON_CAD_DEFECT_AREA: strTipText.LoadString(IDS_Defect_Area);
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


void CCADTableDlg::SetToolBarCheckedState()
{
	int nFlags;
	int	nIndex;

	if(m_pData->GetCADElementFlags(&nFlags) ) {
		theApp.m_LastSettings.nCADFlags = nFlags;
	} else {
		nFlags = theApp.m_LastSettings.nCADFlags;
	}

	if((nIndex=m_wndPropertiesToolBar.CommandToIndex(ID_BUTTON_CAD_LENGTH))>=0)
		nFlags & CAD_LENGTH ? m_wndPropertiesToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPropertiesToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPropertiesToolBar.CommandToIndex(ID_BUTTON_CAD_HEIGHT))>=0)
		nFlags & CAD_HEIGHT ? m_wndPropertiesToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPropertiesToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPropertiesToolBar.CommandToIndex(ID_BUTTON_CAD_WIDTH))>=0)
		nFlags & CAD_WIDTH ? m_wndPropertiesToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPropertiesToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPropertiesToolBar.CommandToIndex(ID_BUTTON_CAD_AREA))>=0)
		nFlags & CAD_TOTAL_AREA ? m_wndPropertiesToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPropertiesToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPropertiesToolBar.CommandToIndex(ID_BUTTON_CAD_REF_LEVEL))>=0)
		nFlags & CAD_REF_LEVEL ? m_wndPropertiesToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPropertiesToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPropertiesToolBar.CommandToIndex(ID_BUTTON_CAD_DEFECT_LEVEL))>=0)
		nFlags & CAD_DEFECT_LEVEL ? m_wndPropertiesToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPropertiesToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPropertiesToolBar.CommandToIndex(ID_BUTTON_CAD_DEFECT_AREA))>=0)
		nFlags & CAD_DEFECT_AREA ? m_wndPropertiesToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPropertiesToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPropertiesToolBar.CommandToIndex(ID_BUTTON_CAD_DEFECT_WIDTH))>=0)
		nFlags & CAD_DEFECT_WIDTH ? m_wndPropertiesToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPropertiesToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPropertiesToolBar.CommandToIndex(ID_BUTTON_CAD_DEFECT_HEIGHT))>=0)
		nFlags & CAD_DEFECT_HEIGHT ? m_wndPropertiesToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPropertiesToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndPropertiesToolBar.CommandToIndex(ID_BUTTON_VERBOSE_TABLE))>=0)
		theApp.m_LastSettings.nCADTableDisplayMode == 0 ? m_wndPropertiesToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPropertiesToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndPropertiesToolBar.CommandToIndex(ID_BUTTON_COLUMN_TABLE))>=0)
		theApp.m_LastSettings.nCADTableDisplayMode == 1 ? m_wndPropertiesToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndPropertiesToolBar.SetButtonStyle(nIndex,0);


}

void CCADTableDlg::OnButtonCadLength() 
{

	m_pData->ToggleCADElementFlag(CAD_LENGTH);
	SetToolBarCheckedState();
	m_listTable.Invalidate(false);
	
}

void CCADTableDlg::OnButtonCadWidth() 
{
	m_pData->ToggleCADElementFlag(CAD_WIDTH);
	SetToolBarCheckedState();
	m_listTable.Invalidate(false);
}

void CCADTableDlg::OnButtonCadHeight() 
{
	m_pData->ToggleCADElementFlag(CAD_HEIGHT);
	SetToolBarCheckedState();
	m_listTable.Invalidate(false);
}

void CCADTableDlg::OnButtonCadHistogram() 
{
	m_pData->ToggleCADElementFlag(CAD_HISTOGRAM);
	SetToolBarCheckedState();
	m_listTable.Invalidate(false);
}

void CCADTableDlg::OnButtonCadRefLevel() 
{
	m_pData->ToggleCADElementFlag(CAD_REF_LEVEL);
	SetToolBarCheckedState();
	m_listTable.Invalidate(false);
}

void CCADTableDlg::OnButtonCadDefectLevel() 
{
	m_pData->ToggleCADElementFlag(CAD_DEFECT_LEVEL);
	SetToolBarCheckedState();
	m_listTable.Invalidate(false);
}

void CCADTableDlg::OnButtonCadDefectHeight() 
{
	m_pData->ToggleCADElementFlag(CAD_DEFECT_HEIGHT);
	SetToolBarCheckedState();
	m_listTable.Invalidate(false);
}

void CCADTableDlg::OnButtonCadDefectWidth() 
{
	m_pData->ToggleCADElementFlag(CAD_DEFECT_WIDTH);
	SetToolBarCheckedState();
	m_listTable.Invalidate(false);
}

void CCADTableDlg::OnButtonCadDefectArea() 
{
	m_pData->ToggleCADElementFlag(CAD_DEFECT_AREA);
	SetToolBarCheckedState();
	m_listTable.Invalidate(false);
}

void CCADTableDlg::OnButtonCadArea() 
{
	m_pData->ToggleCADElementFlag(CAD_TOTAL_AREA);
	SetToolBarCheckedState();
	m_listTable.Invalidate(false);
}

void CCADTableDlg::OnButtonFont() 
{
	BOOL bFlag;
	LOGFONT lf = theApp.m_LastSettings.lfCAD;

	if(bFlag=m_pData->IsCADElement()) {
		lf=m_pData->m_pCADElement[m_pData->GetCADElement()].m_lf;
	}

	CFontDialog dlg(&lf);
	if(bFlag == false) {
		dlg.m_cf.rgbColors = theApp.m_LastSettings.rgbCADText;
	} else {
		dlg.m_cf.rgbColors = m_pData->m_pCADElement[m_pData->GetCADElement()].m_rgbText;
	}
	if(dlg.DoModal() == IDOK) {
		dlg.GetCurrentFont(&theApp.m_LastSettings.lfCAD);
		theApp.m_LastSettings.rgbCADText = dlg.m_cf.rgbColors;

		if(bFlag) {
			dlg.GetCurrentFont(&m_pData->m_pCADElement[m_pData->GetCADElement()].m_lf);
			m_pData->m_pCADElement[m_pData->GetCADElement()].m_rgbText = dlg.m_cf.rgbColors;
			InvalidateParent();
		}

		UpdateAllControls();
	}
	
}

void CCADTableDlg::OnChangeEditFontSize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CCADTableDlg::OnDeltaposSpinFontSize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	theApp.m_LastSettings.lfCAD.lfHeight = pFrame->lfHeightFromPointSize(pNMUpDown->iPos);
	if(m_pData->IsCADElement() ) {
		m_pData->m_pCADElement[m_pData->m_nCADPtr].m_lf.lfHeight = theApp.m_LastSettings.lfCAD.lfHeight;
		InvalidateParent();
	}

	UpdateAllControls();
	*pResult = 0;
}

void CCADTableDlg::OnDeltaposSpinLineWidth(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	theApp.m_LastSettings.nCADLineWidth = pNMUpDown->iPos;
	if(m_pData->IsCADElement() ) {
		m_pData->m_pCADElement[m_pData->m_nCADPtr].m_nLineWidth = theApp.m_LastSettings.nCADLineWidth;
		InvalidateParent();
	}

	UpdateAllControls();
	*pResult = 0;
}

void CCADTableDlg::OnChangeEditLineWidth() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CCADTableDlg::OnSelchangeComboDefectMode() 
{
	theApp.m_LastSettings.nCADDefectPaintMode = m_comboDefectMode.GetCurSel();

	if(m_pData->IsCADElement() ) {
		m_pData->m_pCADElement[m_pData->m_nCADPtr].m_nCADDefectPaintMode = theApp.m_LastSettings.nCADDefectPaintMode;
		InvalidateParent();
	}
}

void CCADTableDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CMFCColorDialog dlg;

	m_staticLineColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbCADColor=dlg.GetColor();
			if(m_pData->IsCADElement() ) {
				m_pData->m_pCADElement[m_pData->m_nCADPtr].m_rgbColor = dlg.GetColor();
				InvalidateParent();
			}
			Invalidate(FALSE);
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}


void CCADTableDlg::InvalidateParent()
{
	CViewSheet* pSheet = (CViewSheet*)m_pParent;
	CViewPage* pPage = (CViewPage*)pSheet->GetActivePage();

	pSheet->m_nToolOption = 0;
	pSheet->SetToolBarCheckedState();
	pPage->m_nToolOption = TOOL_RECTANGLE;
	if(m_pData->m_pCADElement) {
		switch(m_pData->m_pCADElement[m_pData->m_nCADPtr].m_Type) {
		default:
			pSheet->m_nToolOption = pPage->m_nToolOption = TOOL_RECTANGLE;
			break;
		case CAD_PARALLELOGRAM:
			pSheet->m_nToolOption = pPage->m_nToolOption = TOOL_PARALLELOGRAM;
			break;
		case CAD_CENTRELINE:
			pSheet->m_nToolOption = pPage->m_nToolOption = TOOL_CENTRELINE;
			break;
		}
	};

	pPage->CADElementSizeArrows();
	pPage->Invalidate(false);

}

void CCADTableDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rr;
	
	m_staticLineColor.GetWindowRect(&rr);
	ScreenToClient(rr);

	if(m_pData->IsCADElement()) {
		dc.FillRect(rr,&CBrush(m_pData->m_pCADElement[m_pData->m_nCADPtr].m_rgbColor));
	} else {
		dc.FillRect(rr,&CBrush(theApp.m_LastSettings.rgbCADColor));
	};
}

void CCADTableDlg::OnRclickListTable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

//	m_nDacIndex=pDispInfo->item.mask;

	CMenu* menu = (CMenu *) new CMenu;
	CPoint	Point;

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_CAD_TABLE_MENU)) {
		CMenu* pPopup = menu->GetSubMenu(0);
		ASSERT(pPopup != NULL);
				
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;

	*pResult = 0;
}

void CCADTableDlg::OnCadCopy() 
{
	if(m_pData->IsCADElement()) {
		m_CADElement = &m_pData->m_pCADElement[m_pData->m_nCADPtr];
	}
}

void CCADTableDlg::OnCadPaste() 
{
	m_pData->AddCADElement(&m_CADElement);
	FillList();
	UpdateAllControls();
	Invalidate(false);

	InvalidateParent();
	
}

void CCADTableDlg::OnCadDelete() 
{
	m_pData->DeleteCADElement(m_pData->m_nCADPtr);
	FillList();
	UpdateAllControls();
	Invalidate(false);

	InvalidateParent();
}



void CCADTableDlg::OnCadCopytoclipboard() 
{
	HGLOBAL hClipboardData;
	WCHAR * pchData;
	CString Buff,Temp;
	int ii,nn;
	CString ColumnName[12];

	Buff.Empty();

	if(m_pData->IsCADElement()) {
		switch(	theApp.m_LastSettings.nCADTableDisplayMode ) {
		case 0:

			for(ii=0;ii<m_pData->m_nCADElementL;ii++) {

				Temp.Format(L"%s",m_pData->m_pCADElement[ii].m_wText);
				Buff += Temp;
				Buff += "\t";
				m_pData->GetCADVerboseStats(ii,&Temp);
				Buff += Temp;
				Buff += "\r\n";
			}
			break;

		case 1:

			ColumnName[0].LoadString(IDS_Title);
			ColumnName[1].LoadString(IDS_Length);
			ColumnName[2].LoadString(IDS_Width);
			ColumnName[3].LoadString(IDS_Height);
			ColumnName[4].LoadString(IDS_Area);
			ColumnName[5].LoadString(IDS_Ref_Level);
			ColumnName[6].LoadString(IDS_Defect_Level);
			ColumnName[7].LoadString(IDS_Defect_Area);
			ColumnName[8].LoadString(IDS_Defect_Width);
			ColumnName[9].LoadString(IDS_Defect_Height);
			for(ii=0;ii<10;ii++) {
				Buff += ColumnName[ii];
				Buff += "\t";
			}
			Buff += "\r\n";

			for(nn=0;nn<m_pData->m_nCADElementL;nn++) {
				Temp.Format(_T("%s"),m_pData->m_pCADElement[nn].m_wText);
				Buff += (Temp + L"\t");

				if(m_pData->m_pCADElement[nn].m_nFlags & CAD_LENGTH) {
					m_pData->GetCADElementLength(nn,&Temp);
					Buff += Temp;
				}
				Buff += L"\t";
				if(m_pData->m_pCADElement[nn].m_nFlags & CAD_WIDTH) {
					m_pData->GetCADElementWidth(nn,&Temp);
					Buff += Temp;
				}
				Buff += "\t";
				if(m_pData->m_pCADElement[nn].m_nFlags & CAD_HEIGHT) {
					m_pData->GetCADElementHeight(nn,&Temp);
					Buff += Temp;
				}
				Buff += "\t";
				if(m_pData->m_pCADElement[nn].m_nFlags & CAD_TOTAL_AREA) {
					m_pData->GetCADElementArea(nn,&Temp);
					Buff += Temp;
				}
				Buff += "\t";
				if(m_pData->m_pCADElement[nn].m_nFlags & CAD_REF_LEVEL) {
					m_pData->GetHistogramRefLevel(&m_pData->m_pCADElement[nn].m_Hist,&Temp,false);
					Buff += Temp;
				}
				Buff += "\t";
				if(m_pData->m_pCADElement[nn].m_nFlags & CAD_DEFECT_LEVEL) {
					m_pData->GetHistogramDefectLevel(&m_pData->m_pCADElement[nn].m_Hist,&Temp,false);
					Buff += Temp;
				}
				Buff += "\t";
				if(m_pData->m_pCADElement[nn].m_nFlags & CAD_DEFECT_AREA) {
					m_pData->GetHistogramDefectArea(&m_pData->m_pCADElement[nn].m_Hist,&Temp,false);
					Buff += Temp;
				}
				Buff += "\t";
				if(m_pData->m_pCADElement[nn].m_nFlags & CAD_DEFECT_WIDTH) {
					m_pData->GetHistogramDefectWidth(&m_pData->m_pCADElement[nn].m_Hist,&Temp,false);
					Buff += Temp;
				}
				Buff += "\t";
				if(m_pData->m_pCADElement[nn].m_nFlags & CAD_DEFECT_HEIGHT) {
					m_pData->GetHistogramDefectHeight(&m_pData->m_pCADElement[nn].m_Hist,&Temp,false);
					Buff += Temp;
				}
				Buff += "\t";
				Buff += "\r\n";
			}
			break;
		}

		if ( OpenClipboard() )
		{
			EmptyClipboard();
			hClipboardData = GlobalAlloc(GMEM_DDESHARE, (Buff.GetLength()+1) * sizeof WCHAR);
			pchData = (WCHAR*)GlobalLock(hClipboardData);
			for(ii=0;ii<Buff.GetLength();ii++) {
				pchData[ii] = Buff.GetAt(ii);
			}
			GlobalUnlock(hClipboardData);
			SetClipboardData(CF_UNICODETEXT,hClipboardData);
			CloseClipboard();
		}
	}	
}

void CCADTableDlg::OnDblclkListTable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	SAFE_DELETE( m_pEditSpinItem );

	if(m_pData->IsCADElement() == false) return;
	int nn=pDispInfo->item.mask;

	if((nn>=0) && (nn < m_pData->m_nCADElementL)) {
		m_pData->m_nCADPtr = nn;

		m_listTable.GetWindowRect(CtrlRect);
		m_listTable.GetSubItemRect(nn,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
		Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
		Rect.InflateRect(0,1,0,2);

		m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

		switch(pDispInfo->item.iItem) {
		case 0:
			break;
		case 1:	m_pEditSpinItem->Initialize(m_pData->m_pCADElement[nn].m_wText);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		}
	}	
	*pResult = 0;
}

HRESULT CCADTableDlg::TableChanged(WPARAM,LPARAM)
{
	InvalidateParent();
	return NULL;
}

void CCADTableDlg::OnButtonVerboseTable() 
{
	theApp.m_LastSettings.nCADTableDisplayMode = 0;

	CreateColumns();
	FillList();
	SetToolBarCheckedState();

	UpdateAllControls();
}

void CCADTableDlg::OnButtonColumnTable() 
{
	theApp.m_LastSettings.nCADTableDisplayMode = 1;

	CreateColumns();
	FillList();
	SetToolBarCheckedState();

	UpdateAllControls();
}





void CCADTableDlg::OnCheckStartArrow() 
{
	theApp.m_LastSettings.nCADFlags & CAD_START_ARROW ? theApp.m_LastSettings.nCADFlags &= ~CAD_START_ARROW : theApp.m_LastSettings.nCADFlags |= CAD_START_ARROW;
	m_pData->SetCADElementFlags(theApp.m_LastSettings.nCADFlags);
	InvalidateParent();
}

void CCADTableDlg::OnCheckFinishArrow() 
{
	theApp.m_LastSettings.nCADFlags & CAD_FINISH_ARROW ? theApp.m_LastSettings.nCADFlags &= ~CAD_FINISH_ARROW : theApp.m_LastSettings.nCADFlags |= CAD_FINISH_ARROW;
	m_pData->SetCADElementFlags(theApp.m_LastSettings.nCADFlags);
	InvalidateParent();
}

void CCADTableDlg::OnCheckLineText() 
{
	theApp.m_LastSettings.nCADFlags & CAD_LINE_TEXT ? theApp.m_LastSettings.nCADFlags &= ~CAD_LINE_TEXT : theApp.m_LastSettings.nCADFlags |= CAD_LINE_TEXT;
	m_pData->SetCADElementFlags(theApp.m_LastSettings.nCADFlags);
	InvalidateParent();
}
