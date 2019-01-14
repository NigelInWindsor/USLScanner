// AxesCompensationPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "AxesCompensationPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxesCompensationPage property page

IMPLEMENT_DYNCREATE(CAxesCompensationPage, CPropertyPage)

CAxesCompensationPage::CAxesCompensationPage() : CPropertyPage(CAxesCompensationPage::IDD)
{
	//{{AFX_DATA_INIT(CAxesCompensationPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nAxis = 0;
	m_nIndex = 0;
}

CAxesCompensationPage::~CAxesCompensationPage()
{
}

void CAxesCompensationPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxesCompensationPage)
	DDX_Control(pDX, IDC_EDIT_GASH, m_editGash);
	DDX_Control(pDX, IDC_CHECK_USE, m_checkUse);
	DDX_Control(pDX, IDC_EDIT_POS_XRIGHT, m_editPosXRight);
	DDX_Control(pDX, IDC_EDIT_POS_YRIGHT, m_editPosYRight);
	DDX_Control(pDX, IDC_EDIT_POS_ZRIGHT, m_editPosZRight);
	DDX_Control(pDX, IDC_LIST_COORDS, m_listCoords);
	DDX_Control(pDX, IDC_COMBO_AXIS, m_comboAxis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxesCompensationPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAxesCompensationPage)
	ON_CBN_SELCHANGE(IDC_COMBO_AXIS, OnSelchangeComboAxis)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_COORDS, OnRclickListCoords)
	ON_COMMAND(ID_COMPENSATE_ADD, OnCompensateAdd)
	ON_COMMAND(ID_COMPENSATE_DELETE, OnCompensateDelete)
	ON_COMMAND(ID_COMPENSATE_MODIFY, OnCompensateModify)
	ON_COMMAND(ID_COMPENSATE_CLEARALL, OnCompensateClearall)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_COORDS, OnGetdispinfoListCoords)
	ON_BN_CLICKED(IDC_CHECK_USE, OnCheckUse)
	ON_EN_CHANGE(IDC_EDIT_GASH, OnChangeEditGash)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxesCompensationPage message handlers
BOOL CAxesCompensationPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	switch(theApp.m_nSide0Orientation) {
	case 0: m_nLeft = PORTSIDE;
		m_nRight = STARBOARD;
		m_comboAxis.AddString(_T("X Left"));
		m_comboAxis.AddString(_T("Y Left"));
		m_comboAxis.AddString(_T("Z Left"));
		break;
	case 1: m_nLeft = STARBOARD;
		m_nRight = PORTSIDE;
		m_comboAxis.AddString(_T("X Right"));
		m_comboAxis.AddString(_T("Y Right"));
		m_comboAxis.AddString(_T("Z Right"));
		break;
	}

	CreateColumns();
	FillList();

	UpdateAllControls();

	SetTimer(1,300,NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CAxesCompensationPage::UpdateAllControls()
{
	m_comboAxis.SetCurSel(m_nAxis);
	m_checkUse.SetCheck(theApp.m_LastSettings.bUseAxesCompensation & 1);
}

void CAxesCompensationPage::OnSelchangeComboAxis() 
{
	m_nAxis = m_comboAxis.GetCurSel();

	CreateColumns();
	FillList();
}

BOOL CAxesCompensationPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastCompensationToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	return CPropertyPage::OnSetActive();
}



void CAxesCompensationPage::CreateColumns()
{
	CString ColumnName[6] = { L"#",L"Pos",L"X Error", L"Y Error", L"Z Error"};
	int ColumnWidth[6] = { 0,60,60,60,60};

	int nColumnCount = m_listCoords.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listCoords.DeleteColumn(0);
	}
	for (int ii=0;ii<5;ii++) {
		m_listCoords.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	}
	m_listCoords.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CAxesCompensationPage::FillList()
{
	CString Buff;
	CLeadScrew* pLine = &theApp.m_Kinematics.m_LeadScrew[m_nAxis];
	int nCoordL = pLine->m_nCoordL;
	int nCount = m_listCoords.GetItemCount();
	int ii;

	if(nCount<nCoordL) {
		for (int ii=nCount;ii<nCoordL;ii++) {
			Buff.Format(_T("%d"),ii+1);
			m_listCoords.InsertItem(ii, Buff.GetBuffer(255), ii);
		};
	} else {
		for(ii=nCount-1;ii>=nCoordL;ii--) {
			m_listCoords.DeleteItem(ii);
		}
	}
	m_listCoords.EnsureVisible(m_nIndex,FALSE);
	for(ii=0;ii<nCoordL;ii++) 
		m_listCoords.SetItemState(ii , 0,LVIS_SELECTED);
	m_listCoords.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listCoords.SetFocus();

}


void CAxesCompensationPage::OnTimer(UINT nIDEvent) 
{
	CString Buff;
	CString Unit[] = {L"mm", L"\x022", L"\x0b0"};
	CCoord CpHead,CpSurface,CpDesire;

	theApp.m_Motors.GetHeadPos(&CpHead);
	theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
	CpSurface.Side[m_nRight] = CpSurface.Side[m_nLeft];
	theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpDesire,STARBOARD,TRUE,TRUE);

	if(m_CpError.Side[m_nLeft].fX - (CpHead.Side[m_nRight].fX - CpDesire.Side[m_nLeft].fX)) {
		Buff.Format(_T("%.02f mm"),m_CpError.Side[m_nLeft].fX = (CpHead.Side[m_nRight].fX - CpDesire.Side[m_nRight].fX));
		m_editPosXRight.SetWindowText(Buff);
	}
	if(m_CpError.Side[m_nLeft].fY - (CpHead.Side[m_nRight].fY - CpSurface.Side[m_nLeft].fY)) {
		Buff.Format(_T("%.02f mm"),m_CpError.Side[m_nLeft].fY = (CpHead.Side[m_nRight].fY - CpDesire.Side[m_nRight].fY));
		m_editPosYRight.SetWindowText(Buff);
	}
	if(m_CpError.Side[m_nLeft].fZ - (CpHead.Side[m_nRight].fZ - CpSurface.Side[m_nLeft].fZ)) {
		Buff.Format(_T("%.02f mm"),m_CpError.Side[m_nLeft].fZ = (CpHead.Side[m_nRight].fZ - CpDesire.Side[m_nRight].fZ));
		m_editPosZRight.SetWindowText(Buff);
	}
	m_CpError.Side[m_nRight].fX = CpDesire.Side[m_nRight].fX;
	m_CpError.Side[m_nRight].fY = CpDesire.Side[m_nRight].fY;
	m_CpError.Side[m_nRight].fZ = CpDesire.Side[m_nRight].fZ;

	
	CPropertyPage::OnTimer(nIDEvent);
}

void CAxesCompensationPage::OnRclickListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;

	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	m_nIndex=m_listCoords.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_COMPENSATE_MENU)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
	
	
	*pResult = 0;
}

void CAxesCompensationPage::OnCompensateAdd() 
{
//	theApp.m_Kinematics.m_LeadScrew[m_nAxis].Add(m_CpError);
//	theApp.m_Kinematics.m_LeadScrew[m_nAxis].Sort();
//	theApp.m_Kinematics.PreProcessAxesCompensation();
	FillList();
	Invalidate(FALSE);


}

void CAxesCompensationPage::OnCompensateDelete() 
{
	int nStartIndex,nFinishIndex;
	
	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	nStartIndex=(int)pos;
	nFinishIndex=nStartIndex=m_listCoords.GetNextSelectedItem(pos);
	while(pos) {
		nFinishIndex=m_listCoords.GetNextSelectedItem(pos);
	}
//	theApp.m_Kinematics.m_LeadScrew[m_nAxis].DeleteCoords(nStartIndex,nFinishIndex);
	theApp.m_Kinematics.PreProcessAxesCompensation();
	FillList();

	Invalidate(FALSE);	
}

void CAxesCompensationPage::OnCompensateModify() 
{
	int nStartIndex;
	
	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	nStartIndex=m_listCoords.GetNextSelectedItem(pos);
//	theApp.m_Kinematics.m_LeadScrew[m_nAxis].ModifyCoord(nStartIndex,m_CpError);
	theApp.m_Kinematics.m_LeadScrew[m_nAxis].Sort();
	theApp.m_Kinematics.PreProcessAxesCompensation();
	FillList();

	Invalidate(FALSE);	
}

void CAxesCompensationPage::OnCompensateClearall() 
{
	theApp.m_Kinematics.m_LeadScrew[m_nAxis].Zero();	
	FillList();
}

void CAxesCompensationPage::OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	CLeadScrew* pLine = &theApp.m_Kinematics.m_LeadScrew[m_nAxis];


	pDispInfo->item.pszText=str;

	if(pLine->m_nCoordL<=0) {
		str[0]=0;
		return;
	}
/*
	switch(pDispInfo->item.iSubItem) {
	case 0:	swprintf(str,_T("%d"),pDispInfo->item.iItem+1);
		break;
	case 1:	swprintf(str,_T("%d"),pLine->m_pPos[pDispInfo->item.iItem].nActualPos);
		break;
	case 2:	swprintf(str,_T("%.02f"),pLine->m_pPos[pDispInfo->item.iItem].Side[m_nLeft].fX);
		break;
	case 3:	swprintf(str,_T("%.02f"),pLine->m_pPos[pDispInfo->item.iItem].Side[m_nLeft].fY);
		break;
	case 4:	swprintf(str,_T("%.02f"),pLine->m_pPos[pDispInfo->item.iItem].Side[m_nLeft].fZ);
		break;
	}
	pDispInfo->item.pszText=str;
*/
	*pResult = 0;
}

void CAxesCompensationPage::OnCheckUse() 
{
	theApp.m_LastSettings.bUseAxesCompensation = m_checkUse.GetCheck()&1;
}

void CAxesCompensationPage::OnChangeEditGash() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
