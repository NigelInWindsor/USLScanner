// TurnTablePage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "TurnTablePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTurnTablePage property page

IMPLEMENT_DYNCREATE(CTurnTablePage, CPropertyPage)

CTurnTablePage::CTurnTablePage() : CPropertyPage(CTurnTablePage::IDD)
{
	//{{AFX_DATA_INIT(CTurnTablePage)
	//}}AFX_DATA_INIT
	
	m_Circumcentre.SetSize(3);
	m_CpSurface0.Zero();
	m_CpSurface1.Zero();
}

CTurnTablePage::~CTurnTablePage()
{
}

void CTurnTablePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTurnTablePage)
	DDX_Control(pDX, IDC_EDIT_STORE_1, m_editStore1);
	DDX_Control(pDX, IDC_EDIT_STORE_0, m_editStore0);
	DDX_Control(pDX, IDC_EDIT_CIRCUMCENTER, m_editCircumcentre);
	DDX_Control(pDX, IDC_EDIT_CC2, m_editCC2);
	DDX_Control(pDX, IDC_EDIT_CC1, m_editCC1);
	DDX_Control(pDX, IDC_EDIT_CC0, m_editCC0);
	DDX_Control(pDX, IDC_BUTTON_CC_2, m_buttonCC2);
	DDX_Control(pDX, IDC_BUTTON_CC_1, m_buttonCC1);
	DDX_Control(pDX, IDC_BUTTON_CC_0, m_buttonCC0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTurnTablePage, CPropertyPage)
	//{{AFX_MSG_MAP(CTurnTablePage)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT_CC0, OnChangeEditCc0)
	ON_EN_CHANGE(IDC_EDIT_CC1, OnChangeEditCc1)
	ON_EN_CHANGE(IDC_EDIT_CC2, OnChangeEditCc2)
	ON_BN_CLICKED(IDC_BUTTON_CC_0, OnButtonCc0)
	ON_BN_CLICKED(IDC_BUTTON_CC_1, OnButtonCc1)
	ON_BN_CLICKED(IDC_BUTTON_CC_2, OnButtonCc2)
	ON_BN_CLICKED(IDC_BUTTON_STORE, OnButtonStore)
	ON_BN_CLICKED(IDC_BUTTON_STORE_0, OnButtonStore0)
	ON_BN_CLICKED(IDC_BUTTON_STORE_1, OnButtonStore1)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_0, OnButtonGoto0)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_1, OnButtonGoto1)
	ON_BN_CLICKED(IDC_BUTTON_GOTO_CENTER, OnButtonGotoCenter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTurnTablePage message handlers

BOOL CTurnTablePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CTurnTablePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CTurnTablePage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	m_editCC0.SetWindowText( m_Circumcentre.GetPt(0,true) );
	m_editCC1.SetWindowText( m_Circumcentre.GetPt(1,true) );
	m_editCC2.SetWindowText( m_Circumcentre.GetPt(2,true) );

	Buff.Format(_T("X:%.1f Y:%.1f Z:%.1f"),m_CpSurface0.Side0.fX,m_CpSurface0.Side0.fY,m_CpSurface0.Side0.fZ);
	m_editStore0.SetWindowText(Buff);
	Buff.Format(_T("X:%.1f Y:%.1f Z:%.1f"),m_CpSurface1.Side0.fX,m_CpSurface1.Side0.fY,m_CpSurface1.Side0.fZ);
	m_editStore1.SetWindowText(Buff);

	UpdateCircumcentre();
}

void CTurnTablePage::OnChangeEditCc0() 
{
	CString Buff;
	CUSLVertex vert;
	
	m_editCC0.GetWindowText(Buff);
	ExtractVector(Buff,&vert.pt);
	m_Circumcentre.SetPt(0,&vert.pt);
	UpdateCircumcentre();
}

void CTurnTablePage::OnChangeEditCc1() 
{
	CString Buff;
	CUSLVertex vert;
	
	m_editCC1.GetWindowText(Buff);
	ExtractVector(Buff,&vert.pt);
	m_Circumcentre.SetPt(1,&vert.pt);
	UpdateCircumcentre();

}

void CTurnTablePage::OnChangeEditCc2() 
{
	CString Buff;
	CUSLVertex vert;
	
	m_editCC2.GetWindowText(Buff);
	ExtractVector(Buff,&vert.pt);
	m_Circumcentre.SetPt(2,&vert.pt);
	UpdateCircumcentre();

}



void CTurnTablePage::UpdateCircumcentre()
{
	float fRadius;
	D3DXVECTOR3 vec;
	CString Buff;

	m_Circumcentre.CalculateCircumCenter(&fRadius, &vec);
	Buff.Format(L"X:%.01f Y:%.01f Z:%.01f",vec.x,vec.y,vec.z);
	m_editCircumcentre.SetWindowText(Buff);

}

BOOL CTurnTablePage::OnSetActive() 
{
	theApp.m_LastSettings.nLastCompensationToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	UpdateAllControls();

	return CPropertyPage::OnSetActive();
}

void CTurnTablePage::OnButtonCc0() 
{
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	m_Circumcentre.SetPt(0,&CpSurface.Side0.pt);
	UpdateAllControls();
	UpdateCircumcentre();
	
}

void CTurnTablePage::OnButtonCc1() 
{
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	m_Circumcentre.SetPt(1,&CpSurface.Side0.pt);
	UpdateAllControls();
	UpdateCircumcentre();
}

void CTurnTablePage::OnButtonCc2() 
{
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	m_Circumcentre.SetPt(2,&CpSurface.Side0.pt);
	UpdateAllControls();
	UpdateCircumcentre();
}


void CTurnTablePage::OnButtonStore() 
{
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	theApp.m_Tank.vTTCentre = CpSurface.Side0.pt;
	UpdateAllControls();
}

void CTurnTablePage::OnButtonStore0() 
{
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&m_CpSurface0);
	UpdateAllControls();
	
}

void CTurnTablePage::OnButtonStore1() 
{
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&m_CpSurface1);
	UpdateAllControls();
}

void CTurnTablePage::OnButtonGoto0() 
{
	CCoord CpCurrent,Cp,CpHead;

	theApp.m_Thread.StopCurrentThreadAction();

	theApp.m_Thread.m_ToolPath.Zero();
	theApp.m_Kinematics.HeadFromSurface(&m_CpSurface0,&CpHead,PORTSIDE,TRUE,TRUE,NULL,CURRENT_FL);
	theApp.m_Thread.m_ToolPath.Add(CpHead);

	theApp.m_Thread.m_nThreadAction=POLY_MOVE;
	
}

void CTurnTablePage::OnButtonGoto1() 
{
	CCoord CpHead;

	theApp.m_Thread.StopCurrentThreadAction();

	theApp.m_Thread.m_ToolPath.Zero();
	theApp.m_Kinematics.HeadFromSurface(&m_CpSurface1,&CpHead,PORTSIDE,TRUE,TRUE,NULL,CURRENT_FL);
	theApp.m_Thread.m_ToolPath.Add(CpHead);

	theApp.m_Thread.m_nThreadAction=POLY_MOVE;
}

void CTurnTablePage::OnButtonGotoCenter() 
{
	CCoord Cp;

	for (int ii = 0; ii < 30; ii++) {
		Cp.fPos[ii] = (m_CpSurface0.fPos[ii] + m_CpSurface1.fPos[ii]) / 2.0f;
	}

	theApp.m_Motors.GotoSurfaceCoord(&Cp, 1);
}
