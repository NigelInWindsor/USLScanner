// 3DStiffenerPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "3DStiffenerPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DStiffenerPage property page

IMPLEMENT_DYNCREATE(C3DStiffenerPage, CPropertyPage)

C3DStiffenerPage::C3DStiffenerPage() : CPropertyPage(C3DStiffenerPage::IDD)
{
	//{{AFX_DATA_INIT(C3DStiffenerPage)
	//}}AFX_DATA_INIT
	
	m_nIndex = 0;

	m_strAction[0].LoadString(IDS_Moveto_Danger_Plane);
	m_strAction[1].LoadString(IDS_Danger_Plane_Moveto);
	m_strAction[2].LoadString(IDS_Moveto);

	m_pEditSpinItem = NULL;

}

C3DStiffenerPage::~C3DStiffenerPage()
{
}

void C3DStiffenerPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3DStiffenerPage)
	DDX_Control(pDX, IDC_LIST_DEPART, m_listDepart);
	DDX_Control(pDX, IDC_LIST_APPROACH, m_listApproach);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3DStiffenerPage, CPropertyPage)
	//{{AFX_MSG_MAP(C3DStiffenerPage)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_APPROACH, OnGetdispinfoListApproach)
	ON_NOTIFY(NM_CLICK, IDC_LIST_APPROACH, OnClickListApproach)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_APPROACH, OnDblclkListApproach)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_DEPART, OnGetdispinfoListDepart)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DEPART, OnClickListDepart)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DEPART, OnDblclkListDepart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DStiffenerPage message handlers

BOOL C3DStiffenerPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CreateColumns();
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3DStiffenerPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;


	FillApproachList();
	FillDepartList();

}

void C3DStiffenerPage::CreateColumns()
{
	int ColumnWidth[10] = { 0, 100,100,100,100};
	CString ColumnName[10] = {L"#",L"Action",L"XYZijk",L"Action",L"XYZijk"};
	int	ii;

	for (ii=0;ii<5;ii++) {
		m_listApproach.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
		m_listDepart.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	}

	m_listApproach.SetExtendedStyle( LVS_EX_FULLROWSELECT   );
	m_listDepart.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void C3DStiffenerPage::FillApproachList()
{

}

void C3DStiffenerPage::FillDepartList()
{

}


void C3DStiffenerPage::OnGetdispinfoListApproach(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void C3DStiffenerPage::OnClickListApproach(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void C3DStiffenerPage::OnDblclkListApproach(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void C3DStiffenerPage::OnGetdispinfoListDepart(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void C3DStiffenerPage::OnClickListDepart(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void C3DStiffenerPage::OnDblclkListDepart(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
