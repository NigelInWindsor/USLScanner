// TurntablePage1.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "TurntablePage1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTurntablePage property page

IMPLEMENT_DYNCREATE(CTurntablePage, CPropertyPage)

CTurntablePage::CTurntablePage() : CPropertyPage(CTurntablePage::IDD)
{
	//{{AFX_DATA_INIT(CTurntablePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bDlgCreated = FALSE;
	m_CircumCenterElement.SetSize(3);

}

CTurntablePage::~CTurntablePage()
{
}

void CTurntablePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTurntablePage)
	DDX_Control(pDX, IDC_EDIT_CIRCUMCENTER, m_editCircumcenter);
	DDX_Control(pDX, IDC_BUTTON_CC_2, m_buttonCC2);
	DDX_Control(pDX, IDC_BUTTON_CC_1, m_buttonCC1);
	DDX_Control(pDX, IDC_BUTTON_CC_0, m_buttonCC0);
	DDX_Control(pDX, IDC_EDIT_COORDINATE, m_editCoordinate);
	DDX_Control(pDX, IDC_EDIT_SAFE_POS_LEFT, m_editLeft);
	DDX_Control(pDX, IDC_EDIT_SAFE_POS_RIGHT, m_editRight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTurntablePage, CPropertyPage)
	//{{AFX_MSG_MAP(CTurntablePage)
	ON_BN_CLICKED(IDC_BUTTON_GOTO, OnButtonGoto)
	ON_BN_CLICKED(IDC_BUTTON_STORE, OnButtonStore)
	ON_EN_CHANGE(IDC_EDIT_COORDINATE, OnChangeEditCoordinate)
	ON_BN_CLICKED(IDC_BUTTON_STORE_SAFE_POS, OnButtonStoreSafePos)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_SAFE, OnButtonGotoSafe)
	ON_BN_CLICKED(IDC_BUTTON_CC_0, OnButtonCc0)
	ON_BN_CLICKED(IDC_BUTTON_CC_1, OnButtonCc1)
	ON_BN_CLICKED(IDC_BUTTON_CC_2, OnButtonCc2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTurntablePage message handlers

BOOL CTurntablePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	APP App = (APP) AfxGetApp();
	CString Buff;

	m_bDlgCreated = TRUE;
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTurntablePage::UpdateAllControls()
{
	if(m_bDlgCreated != TRUE) return;
	APP App = (APP) AfxGetApp();
	CString Buff;
	CString Unit[] = {"mm", "\x022", "\x0b0"};
	D3DXVECTOR3 vec;
	float fRadius;

	Buff.Format(_T("X:%.1f Y:%.1f Z:%.1f"),App->m_Tank.fXTurnTableCentre,App->m_Tank.fYTurnTableCentre,App->m_Tank.fZTurnTableCentre);
	m_editCoordinate.SetWindowText(Buff);

	Buff.Format(_T("X:%.*f %s\r\nY:%.*f %s\r\nZ:%.*f %s\r\nXtip:%.*f %s\r\nZTip:%.*f %s\r\n"),
		App->m_nUnitsPrecision, App->m_Tank.CpSafe.Side0.fX * App->m_fUnits, App->m_Units,
		App->m_nUnitsPrecision, App->m_Tank.CpSafe.Side0.fY * App->m_fUnits, App->m_Units,
		App->m_nUnitsPrecision, App->m_Tank.CpSafe.Side0.fZ * App->m_fUnits, App->m_Units,
		App->m_nUnitsPrecision, App->m_Tank.CpSafe.Side0.fXt,Unit[App->m_Axes[App->m_Tank.nXtRight].nUnitSelected],
		App->m_nUnitsPrecision, App->m_Tank.CpSafe.Side0.fYt,Unit[App->m_Axes[App->m_Tank.nYtRight].nUnitSelected]);
	m_editLeft.SetWindowText(Buff);

	Buff.Format(_T("X:%.*f %s\r\nY:%.*f %s\r\nZ:%.*f %s\r\nXtip:%.*f %s\r\nZTip:%.*f %s\r\n"),
		App->m_nUnitsPrecision, App->m_Tank.CpSafe.Side1.fX * App->m_fUnits, App->m_Units,
		App->m_nUnitsPrecision, App->m_Tank.CpSafe.Side1.fY * App->m_fUnits, App->m_Units,
		App->m_nUnitsPrecision, App->m_Tank.CpSafe.Side1.fZ * App->m_fUnits, App->m_Units,
		App->m_nUnitsPrecision, App->m_Tank.CpSafe.Side1.fXt,Unit[App->m_Axes[App->m_Tank.nXtRight].nUnitSelected],
		App->m_nUnitsPrecision, App->m_Tank.CpSafe.Side1.fYt,Unit[App->m_Axes[App->m_Tank.nYtRight].nUnitSelected]);
	m_editRight.SetWindowText(Buff);

	m_CircumCenterElement.GetPt(0,&vec);
	Buff.Format(L"X:%.01f Y:%.01f Z:%.01f",vec.x,vec.y,vec.z);
	m_buttonCC0.SetWindowText(Buff);

	m_CircumCenterElement.GetPt(1,&vec);
	Buff.Format(L"X:%.01f Y:%.01f Z:%.01f",vec.x,vec.y,vec.z);
	m_buttonCC1.SetWindowText(Buff);

	m_CircumCenterElement.GetPt(2,&vec);
	Buff.Format(L"X:%.01f Y:%.01f Z:%.01f",vec.x,vec.y,vec.z);
	m_buttonCC2.SetWindowText(Buff);

	m_CircumCenterElement.CalculateCircumCenter(&fRadius, &vec);
	Buff.Format(L"X:%.01f Y:%.01f Z:%.01f",vec.x,vec.y,vec.z);
	m_editCircumcenter.SetWindowText(Buff);
}

void CTurntablePage::OnButtonGoto() 
{
	APP App = (APP) AfxGetApp();
	CCoord CpSurface;
	CpSurface.Zero();

	CpSurface.Side0.fX=App->m_Tank.fXTurnTableCentre;
	CpSurface.Side0.fY=App->m_Tank.fYTurnTableCentre;
	CpSurface.Side0.fZ=App->m_Tank.fZTurnTableCentre;
	CpSurface.Side1.fX=App->m_Tank.fXTurnTableCentre;
	CpSurface.Side1.fY=App->m_Tank.fYTurnTableCentre;
	CpSurface.Side1.fZ=App->m_Tank.fZTurnTableCentre;
	App->m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
	App->m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
	App->m_Motors.GotoSurfaceCoord(&CpSurface,App->m_nPrimarySide);
	
}

void CTurntablePage::OnButtonStore() 
{
	APP App = (APP) AfxGetApp();
	CCoord CpSurface;

	App->m_Motors.GetSurfacePos(&CpSurface);
	App->m_Tank.fXTurnTableCentre = CpSurface.Side0.fX;
	App->m_Tank.fYTurnTableCentre = CpSurface.Side0.fY;
	App->m_Tank.fZTurnTableCentre = CpSurface.Side0.fZ;

	UpdateAllControls();
	App->SaveTank();
}


void CTurntablePage::OnChangeEditCoordinate() 
{
	APP App = (APP) AfxGetApp();
	CString Buff;

	m_editCoordinate.GetWindowText(Buff);
	swscanf(Buff,_T("X:%f Y:%f Z:%f"),&App->m_Tank.fXTurnTableCentre,
				&App->m_Tank.fYTurnTableCentre,&App->m_Tank.fZTurnTableCentre);

}

void CTurntablePage::OnButtonStoreSafePos() 
{
	APP App = (APP) AfxGetApp();

	App->m_Motors.GetSurfacePos(&App->m_Tank.CpSafe);

	UpdateAllControls();
	App->SaveTank();
	
}

void CTurntablePage::OnButtonGotoSafe() 
{
	APP App = (APP) AfxGetApp();

	App->m_Motors.GotoSurfaceCoord(&App->m_Tank.CpSafe,-1);
}

void CTurntablePage::OnButtonCc0() 
{	
	APP App = (APP) AfxGetApp();
	CCoord Cp;

	App->m_Motors.GetSurfacePos(&Cp);
	m_CircumCenterElement.ModifyCoord(0,Cp);	
	UpdateAllControls();
}

void CTurntablePage::OnButtonCc1() 
{
	APP App = (APP) AfxGetApp();
	CCoord Cp;

	App->m_Motors.GetSurfacePos(&Cp);
	m_CircumCenterElement.ModifyCoord(1,Cp);	
	UpdateAllControls();
}

void CTurntablePage::OnButtonCc2() 
{
	APP App = (APP) AfxGetApp();
	CCoord Cp;

	App->m_Motors.GetSurfacePos(&Cp);
	m_CircumCenterElement.ModifyCoord(2,Cp);	
	UpdateAllControls();
}

BOOL CTurntablePage::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnSetActive();
}
