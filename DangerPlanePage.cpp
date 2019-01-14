// DangerPlanePage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "DangerPlanePage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDangerPlanePage property page

IMPLEMENT_DYNCREATE(CDangerPlanePage, CPropertyPage)

CDangerPlanePage::CDangerPlanePage(CPropertySheet* pToolSheet, CPropertyPage* pViewPage) : CPropertyPage(CDangerPlanePage::IDD)
{
	//{{AFX_DATA_INIT(CDangerPlanePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSheet = pToolSheet;
	m_pPage = pViewPage;

	
}

CDangerPlanePage::~CDangerPlanePage()
{
}

void CDangerPlanePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDangerPlanePage)
	DDX_Control(pDX, IDC_SPIN_CLEARANCE_1, m_spinClearance1);
	DDX_Control(pDX, IDC_SPIN_CLEARANCE_0, m_spinClearance0);
	DDX_Control(pDX, IDC_EDIT_CLEARANCE_1, m_editClearance1);
	DDX_Control(pDX, IDC_EDIT_CLEARANCE_0, m_editClearance0);
	DDX_Control(pDX, IDC_STATIC_1, m_static1);
	DDX_Control(pDX, IDC_STATIC_0, m_static0);
	DDX_Control(pDX, IDC_COMBO_VEC_TO_DANGERPLANE2, m_comboVecToDangerPlane1);
	DDX_Control(pDX, IDC_COMBO_VEC_TO_DANGERPLANE, m_comboVecToDangerPlane0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDangerPlanePage, CPropertyPage)
	//{{AFX_MSG_MAP(CDangerPlanePage)
	ON_CBN_SELCHANGE(IDC_COMBO_VEC_TO_DANGERPLANE, OnSelchangeComboVecToDangerplane)
	ON_CBN_SELCHANGE(IDC_COMBO_VEC_TO_DANGERPLANE2, OnSelchangeComboVecToDangerplane2)
	ON_EN_CHANGE(IDC_EDIT_CLEARANCE_0, OnChangeEditClearance0)
	ON_EN_CHANGE(IDC_EDIT_CLEARANCE_1, OnChangeEditClearance1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CLEARANCE_0, OnDeltaposSpinClearance0)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CLEARANCE_1, OnDeltaposSpinClearance1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDangerPlanePage message handlers

BOOL CDangerPlanePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_static0.SetWindowText(theApp.m_DlgSideName[0]);
	m_static1.SetWindowText(theApp.m_DlgSideName[1]);

	m_comboVecToDangerPlane0.AddString(L"i: -1.0"); 
	m_comboVecToDangerPlane0.AddString(L"j: -1.0"); 
	m_comboVecToDangerPlane0.AddString(L"k: -1.0"); 
	m_comboVecToDangerPlane1.AddString(L"i: 1.0"); 
	m_comboVecToDangerPlane1.AddString(L"j: 1.0"); 
	m_comboVecToDangerPlane1.AddString(L"k: 1.0"); 

	m_spinClearance0.SetPos(50);
	m_spinClearance0.SetRange(0, 100);
	m_spinClearance1.SetPos(50);
	m_spinClearance1.SetRange(0, 100);

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CDangerPlanePage::UpdateAllControls()
{
	CString Buff;

	if(PROFILE->m_vecToDangerPlane[0].x != 0.0f)	m_comboVecToDangerPlane0.SetCurSel(0);
	if(PROFILE->m_vecToDangerPlane[0].y != 0.0f)	m_comboVecToDangerPlane0.SetCurSel(1);
	if(PROFILE->m_vecToDangerPlane[0].z != 0.0f)	m_comboVecToDangerPlane0.SetCurSel(2);

	if(PROFILE->m_vecToDangerPlane[1].x != 0.0f)	m_comboVecToDangerPlane1.SetCurSel(0);
	if(PROFILE->m_vecToDangerPlane[1].y != 0.0f)	m_comboVecToDangerPlane1.SetCurSel(1);
	if(PROFILE->m_vecToDangerPlane[1].z != 0.0f)	m_comboVecToDangerPlane1.SetCurSel(2);


	Buff.Format(L"%.0f mm", theApp.m_LastSettings.fDangerMargin[0]);
	m_editClearance0.SetWindowText(Buff);
	Buff.Format(L"%.0f mm", theApp.m_LastSettings.fDangerMargin[1]);
	m_editClearance1.SetWindowText(Buff);
}

BOOL CDangerPlanePage::OnSetActive() 
{
	theApp.m_LastSettings.nLast3DDrawingToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

BOOL CDangerPlanePage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}

void CDangerPlanePage::OnSelchangeComboVecToDangerplane() 
{
	ZeroMemory(&PROFILE->m_vecToDangerPlane[0], sizeof PROFILE->m_vecToDangerPlane[0]);

	switch(m_comboVecToDangerPlane0.GetCurSel()) {
	case 0:	PROFILE->m_vecToDangerPlane[0].x = -1.0f;
		break;
	case 1:	PROFILE->m_vecToDangerPlane[0].y = -1.0f;
		break;
	case 2:	PROFILE->m_vecToDangerPlane[0].z = -1.0f;
		break;
	};
	
}

void CDangerPlanePage::OnSelchangeComboVecToDangerplane2() 
{
	ZeroMemory(&PROFILE->m_vecToDangerPlane[1], sizeof PROFILE->m_vecToDangerPlane[1]);
	switch(m_comboVecToDangerPlane1.GetCurSel()) {
	case 0:	PROFILE->m_vecToDangerPlane[1].x = 1.0f;
		break;
	case 1:	PROFILE->m_vecToDangerPlane[1].y = 1.0f;
		break;
	case 2:	PROFILE->m_vecToDangerPlane[1].z = 1.0f;
		break;
	};
}

void CDangerPlanePage::OnChangeEditClearance0() 
{
	CString Buff;

	m_editClearance0.GetWindowText(Buff);
	_WTOF(Buff, theApp.m_LastSettings.fDangerMargin[0]);
	Invalidate3DView();

}

void CDangerPlanePage::OnChangeEditClearance1() 
{
	CString Buff;

	m_editClearance1.GetWindowText(Buff);
	_WTOF(Buff, theApp.m_LastSettings.fDangerMargin[1]);
	Invalidate3DView();
}

void CDangerPlanePage::OnDeltaposSpinClearance0(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	theApp.m_LastSettings.fDangerMargin[0] += (float)(pNMUpDown->iDelta * 5);
	UpdateAllControls();
	Invalidate3DView();

	*pResult = 0;
}

void CDangerPlanePage::OnDeltaposSpinClearance1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	theApp.m_LastSettings.fDangerMargin[1] += (float)(pNMUpDown->iDelta * 5);
	UpdateAllControls();

	Invalidate3DView();
	
	*pResult = 0;
}


void CDangerPlanePage::Invalidate3DView()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SendMessage(UI_UPDATE_3D_VIEW_DLG);
}