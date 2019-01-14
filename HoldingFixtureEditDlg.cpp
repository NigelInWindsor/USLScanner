// HoldingFixtureEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "HoldingFixtureEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHoldingFixtureEditDlg dialog


CHoldingFixtureEditDlg::CHoldingFixtureEditDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,int nLeft,int nTop)
	: CDialog(CHoldingFixtureEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHoldingFixtureEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nLeft = nLeft;
	m_nTop = nTop;
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_pFont = NULL;


}


void CHoldingFixtureEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHoldingFixtureEditDlg)
	DDX_Control(pDX, IDC_EDITFIXED_X, m_editFixedX);
	DDX_Control(pDX, IDC_EDITMOVING_Z, m_editZMoving);
	DDX_Control(pDX, IDC_EDITMOVING_Y, m_editYMoving);
	DDX_Control(pDX, IDC_EDITMOVING_X, m_editXMoving);
	DDX_Control(pDX, IDC_EDITFIXED_Y, m_editYFixed);
	DDX_Control(pDX, IDC_EDIT_FIXED_Z, m_editZFixed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHoldingFixtureEditDlg, CDialog)
	//{{AFX_MSG_MAP(CHoldingFixtureEditDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_STORE, OnButtonStore)
	ON_BN_CLICKED(IDC_BUTTON_GOTO, OnButtonGoto)
	ON_EN_CHANGE(IDC_EDITFIXED_Y, OnChangeEditfixedY)
	ON_EN_CHANGE(IDC_EDIT_FIXED_Z, OnChangeEditFixedZ)
	ON_EN_CHANGE(IDC_EDITMOVING_X, OnChangeEditmovingX)
	ON_EN_CHANGE(IDC_EDITMOVING_Y, OnChangeEditmovingY)
	ON_EN_CHANGE(IDC_EDITMOVING_Z, OnChangeEditmovingZ)
	ON_BN_CLICKED(IDC_BUTTON_GOTOPARK, OnButtonGotopark)
	ON_EN_CHANGE(IDC_EDITFIXED_X, OnChangeEditfixedX)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHoldingFixtureEditDlg message handlers

void CHoldingFixtureEditDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CHoldingFixtureEditDlg::OnClose() 
{

	
	CDialog::OnClose();
	DestroyWindow();
}

void CHoldingFixtureEditDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CHoldingFixtureEditDlg::OnInitDialog() 
{
	CString Buff;

	CDialog::OnInitDialog();

	SetWindowPos( NULL , m_nLeft, m_nTop, 0, 0, SWP_NOSIZE);


	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHoldingFixtureEditDlg::UpdateAllControls()
{
	CString Buff;

	Buff.Format(_T("%.01fmm"),PROFILE->m_CpHolding.fXFixed);
	m_editFixedX.SetWindowText(Buff);
	Buff.Format(_T("%.01fmm"),PROFILE->m_CpHolding.fYFixed);
	m_editYFixed.SetWindowText(Buff);
	Buff.Format(_T("%.01fmm"),PROFILE->m_CpHolding.fZFixed);
	m_editZFixed.SetWindowText(Buff);

	Buff.Format(_T("%.01fmm"),PROFILE->m_CpHolding.fXMoving);
	m_editXMoving.SetWindowText(Buff);
	Buff.Format(_T("%.01fmm"),PROFILE->m_CpHolding.fYMoving);
	m_editYMoving.SetWindowText(Buff);
	Buff.Format(_T("%.01fmm"),PROFILE->m_CpHolding.fZMoving);
	m_editZMoving.SetWindowText(Buff);
	

}

void CHoldingFixtureEditDlg::OnButtonStore() 
{
	theApp.m_Motors.GetHoldingCoord(&PROFILE->m_CpHolding);
	UpdateAllControls();
}

void CHoldingFixtureEditDlg::OnButtonGoto() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpHolding = PROFILE->m_CpHolding;
	theApp.m_Thread.m_nThreadAction = MOVETO_HOLDING_FIXTURE;
}

void CHoldingFixtureEditDlg::OnChangeEditfixedX() 
{
	CString Buff;

	m_editFixedX.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_CpHolding.fXFixed);
}

void CHoldingFixtureEditDlg::OnChangeEditfixedY() 
{
	CString Buff;

	m_editYFixed.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_CpHolding.fYFixed);
	
}

void CHoldingFixtureEditDlg::OnChangeEditFixedZ() 
{
	CString Buff;

	m_editZFixed.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_CpHolding.fZFixed);
}

void CHoldingFixtureEditDlg::OnChangeEditmovingX() 
{
	CString Buff;

	m_editXMoving.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_CpHolding.fXMoving);
}

void CHoldingFixtureEditDlg::OnChangeEditmovingY() 
{
	CString Buff;

	m_editYMoving.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_CpHolding.fYMoving);
}

void CHoldingFixtureEditDlg::OnChangeEditmovingZ() 
{
	CString Buff;

	m_editZMoving.GetWindowText(Buff);
	_WTOF(Buff,PROFILE->m_CpHolding.fZMoving);
	
}

void CHoldingFixtureEditDlg::OnButtonGotopark() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	ZeroMemory(&theApp.m_Thread.m_CpHolding,sizeof theApp.m_Thread.m_CpHolding);

	theApp.m_Thread.m_CpHolding.fXMoving = theApp.m_Axes[theApp.m_Tank.nXStep].fParkPosition;
	theApp.m_Thread.m_CpHolding.fYMoving = theApp.m_Axes[theApp.m_Tank.nYStep].fParkPosition;
	theApp.m_Thread.m_CpHolding.fZMoving = theApp.m_Axes[theApp.m_Tank.nZStep].fParkPosition;
	theApp.m_Thread.m_CpHolding.fXFixed = theApp.m_Axes[theApp.m_Tank.nRStep].fParkPosition;
	theApp.m_Thread.m_CpHolding.fYFixed = theApp.m_Axes[theApp.m_Tank.nXtStep].fParkPosition;
	theApp.m_Thread.m_CpHolding.fZFixed = theApp.m_Axes[theApp.m_Tank.nYtStep].fParkPosition;
	theApp.m_Thread.m_nThreadAction = MOVETO_HOLDING_FIXTURE;

}

void CHoldingFixtureEditDlg::OnSetFont(CFont* pFont) 
{

	
	CDialog::OnSetFont(pFont);
}

