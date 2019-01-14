// ScanLinesPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ScanLinesPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScanLinesPage property page

IMPLEMENT_DYNCREATE(CScanLinesPage, CPropertyPage)

CScanLinesPage::CScanLinesPage() : CPropertyPage(CScanLinesPage::IDD)
{
	//{{AFX_DATA_INIT(CScanLinesPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nTimeSlotIndex = 0;
	m_nCoordIndex = 0;

	m_pEditSpinItem = NULL;
}

CScanLinesPage::~CScanLinesPage()
{
}

void CScanLinesPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScanLinesPage)
	DDX_Control(pDX, IDC_LIST_TIMESLOTS, m_listTimeSlots);
	DDX_Control(pDX, IDC_LIST_COORDS, m_listCoords);
	DDX_Control(pDX, IDC_SPIN_NUMBER_LINES, m_spinNumberLines);
	DDX_Control(pDX, IDC_EDIT_NUMBER_LINES, m_editNumberLines);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScanLinesPage, CPropertyPage)
	//{{AFX_MSG_MAP(CScanLinesPage)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_NUMBER_LINES, OnChangeEditNumberLines)
	ON_WM_VSCROLL()
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_TIMESLOTS, OnGetdispinfoListTimeslots)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_COORDS, OnGetdispinfoListCoords)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TIMESLOTS, OnClickListTimeslots)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TIMESLOTS, OnDblclkListTimeslots)
	ON_WM_KILLFOCUS()
	ON_NOTIFY(NM_CLICK, IDC_LIST_COORDS, OnClickListCoords)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COORDS, OnDblclkListCoords)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,ItemChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScanLinesPage message handlers

BOOL CScanLinesPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	

	if(theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30]==2) {
		m_editNumberLines.EnableWindow(false);
		m_spinNumberLines.EnableWindow(false);
	}	

	m_spinNumberLines.SetRange(1,256);
	
	CreateColumns();
	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CScanLinesPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}


BOOL CScanLinesPage::OnSetActive() 
{
	
	
	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

BOOL CScanLinesPage::OnKillActive() 
{
	
	
	return CPropertyPage::OnKillActive();
}

void CScanLinesPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	
	CString Buff;

	Buff.Format(_T("%d"),PROFILE->m_nNumberProbesPerImage);
	m_editNumberLines.SetWindowText(Buff);
	m_spinNumberLines.SetPos(PROFILE->m_nNumberProbesPerImage);


	FillListTimeSlots();
	FillListCoords();
}

void CScanLinesPage::OnChangeEditNumberLines() 
{
	
	CString Buff;

	m_editNumberLines.GetWindowText(Buff);

	PROFILE->m_nNumberProbesPerImage = _ttoi(Buff);
	if(PROFILE->m_nNumberProbesPerImage < 1) {
		PROFILE->m_nNumberProbesPerImage = 1;
		Buff.Format(_T("%d"),PROFILE->m_nNumberProbesPerImage);
		m_editNumberLines.SetWindowText(Buff);
	}
	if(PROFILE->m_nNumberProbesPerImage > 256) {
		PROFILE->m_nNumberProbesPerImage = 256;
		Buff.Format(_T("%d"),PROFILE->m_nNumberProbesPerImage);
		m_editNumberLines.SetWindowText(Buff);
	}

	m_spinNumberLines.SetPos(PROFILE->m_nNumberProbesPerImage);

	
}

void CScanLinesPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	int	nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_NUMBER_LINES:
		nTemp = PROFILE->m_nNumberProbesPerImage;
		PROFILE->m_nNumberProbesPerImage=(pSpin->GetPos()&0xffff);
		if(nTemp - PROFILE->m_nNumberProbesPerImage) {
			UpdateAllControls();
		}
		break;
	}
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CScanLinesPage::CreateColumns()
{
	CString ColumnName[5] = { L"#",L"TimeSlot", L"Line No."};
	int ColumnWidth[5] = { 0,60,50 };
	CRect rr;

	ColumnName[1].LoadString(IDS_TimeSlot);
	ColumnName[2].LoadString(IDS_Line_No);

	m_listTimeSlots.GetWindowRect(&rr);

	// Delete all of the columns.
	int nColumnCount = m_listTimeSlots.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listTimeSlots.DeleteColumn(0);
	}

	m_listTimeSlots.InsertColumn(0,ColumnName[0], LVCFMT_LEFT,0);
	m_listTimeSlots.InsertColumn(1,ColumnName[1], LVCFMT_LEFT,(rr.Width()*5)/8-1);
	m_listTimeSlots.InsertColumn(2,ColumnName[2], LVCFMT_LEFT,(rr.Width()*3)/8-2);

	m_listTimeSlots.SetExtendedStyle( LVS_EX_FULLROWSELECT  );


	ColumnName[1].LoadString(IDS_Line);
	ColumnName[2].Format(_T("X"));
	ColumnName[3].Format(_T("Y"));
	ColumnName[4].Format(_T("Z"));

	m_listCoords.GetWindowRect(&rr);

	// Delete all of the columns.
	nColumnCount = m_listCoords.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listCoords.DeleteColumn(0);
	}

	m_listCoords.InsertColumn(0,ColumnName[0], LVCFMT_LEFT,0);
	m_listCoords.InsertColumn(1,ColumnName[1], LVCFMT_LEFT,rr.Width()/4-1);
	m_listCoords.InsertColumn(2,ColumnName[2], LVCFMT_LEFT,rr.Width()/4-1);
	m_listCoords.InsertColumn(3,ColumnName[3], LVCFMT_LEFT,rr.Width()/4-1);
	m_listCoords.InsertColumn(4,ColumnName[4], LVCFMT_LEFT,rr.Width()/4-1);

	m_listCoords.SetExtendedStyle( LVS_EX_FULLROWSELECT  );

}

void CScanLinesPage::FillListTimeSlots()
{
	
	CString Buff;

	m_listTimeSlots.DeleteAllItems();
	for (int ii=0;ii<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listTimeSlots.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listTimeSlots.EnsureVisible(m_nTimeSlotIndex,FALSE);
	for(int ii=0;ii<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;ii++) 
		m_listTimeSlots.SetItemState(ii , 0,LVIS_SELECTED);
	m_listTimeSlots.SetItemState(m_nTimeSlotIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listTimeSlots.SetFocus();


}

void CScanLinesPage::FillListCoords()
{
	
	CString Buff;

	m_listCoords.DeleteAllItems();
	for (int ii=0;ii<PROFILE->m_nNumberProbesPerImage;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listCoords.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listCoords.EnsureVisible(m_nTimeSlotIndex,FALSE);
	for(int ii=0;ii<PROFILE->m_nNumberProbesPerImage;ii++) 
		m_listCoords.SetItemState(ii , 0,LVIS_SELECTED);
	m_listCoords.SetItemState(m_nCoordIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listCoords.SetFocus();

}

void CScanLinesPage::OnGetdispinfoListTimeslots(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];


	switch(pDispInfo->item.iSubItem) {
	case 0:
		break;
	case 1:
		swprintf_s(str,_T("%s"),theApp.m_UtUser.m_TS[pDispInfo->item.iItem].cName);
		break;
	case 2:
		swprintf_s(str,_T("%d"),theApp.m_UtUser.m_TS[pDispInfo->item.iItem].nScanLine+1);
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CScanLinesPage::OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	
	static	WCHAR str[100];


	switch(pDispInfo->item.iSubItem) {
	case 0:
		break;
	case 1:
		swprintf_s(str,_T("%d"),pDispInfo->item.iItem + 1);
		break;
	case 2:
		swprintf_s(str,_T("%.01f"),theApp.m_Kinematics.m_Probe[pDispInfo->item.iItem].fX);
		break;
	case 3:
		swprintf_s(str,_T("%.01f"),theApp.m_Kinematics.m_Probe[pDispInfo->item.iItem].fY);
		break;
	case 4:
		swprintf_s(str,_T("%.01f"),theApp.m_Kinematics.m_Probe[pDispInfo->item.iItem].fZ);
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CScanLinesPage::OnClickListTimeslots(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	ClearEditSpin();
	m_nTimeSlotIndex=pDispInfo->item.mask;
	
	*pResult = 0;
}

void CScanLinesPage::OnDblclkListTimeslots(NMHDR* pNMHDR, LRESULT* pResult) 
{

	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	if(theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30]==2) 
		return;

	m_listTimeSlots.GetWindowRect(CtrlRect);
	m_listTimeSlots.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	ClearEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

	switch(pDispInfo->item.iItem) {
	case 0:
		break;
	case 1:
		break;
	case 2:	
		m_nEditInt = theApp.m_UtUser.m_TS[m_nTimeSlotIndex=pDispInfo->item.mask].nScanLine + 1;
		m_pEditSpinItem->Initialize(&m_nEditInt,1,256,1);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		m_nItemChangeType = 1;
		break;
	}
	
	*pResult = 0;
}

void CScanLinesPage::ClearEditSpin()
{
	SAFE_DELETE( m_pEditSpinItem );

}

HRESULT CScanLinesPage::ItemChanged(WPARAM, LPARAM)
{
	

	if(m_nItemChangeType == 0) {		//Coord

	} else {						//timeslot
		theApp.m_UtUser.m_TS[m_nTimeSlotIndex].nScanLine = m_nEditInt - 1;
	}
	return NULL;
}

void CScanLinesPage::OnClickListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	ClearEditSpin();
	m_nCoordIndex=pDispInfo->item.mask;
	
	*pResult = 0;
}

void CScanLinesPage::OnDblclkListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	if(theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30]==2) 
		return;

	m_listCoords.GetWindowRect(CtrlRect);
	m_listCoords.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	ClearEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

	switch(pDispInfo->item.iItem) {
	case 0:
		break;
	case 1:
		break;
	case 2:	
		m_pEditSpinItem->Initialize(&theApp.m_Kinematics.m_Probe[m_nCoordIndex=pDispInfo->item.mask].fX,-1000.0f,1000.0f,0.1f,_T("%.01f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		m_nItemChangeType = 0;
		break;
	case 3:	
		m_pEditSpinItem->Initialize(&theApp.m_Kinematics.m_Probe[m_nCoordIndex=pDispInfo->item.mask].fY,-1000.0f,1000.0f,0.1f,_T("%.01f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		m_nItemChangeType = 0;
		break;
	case 4:	
		m_pEditSpinItem->Initialize(&theApp.m_Kinematics.m_Probe[m_nCoordIndex=pDispInfo->item.mask].fZ,-1000.0f,1000.0f,0.1f,_T("%.01f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		m_nItemChangeType = 0;
		break;
	}
	
	*pResult = 0;
}
