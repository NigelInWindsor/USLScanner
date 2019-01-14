// FokkerPumpPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "FokkerPumpPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFokkerPumpPage dialog

IMPLEMENT_DYNCREATE(CFokkerPumpPage, CPropertyPage)


CFokkerPumpPage::CFokkerPumpPage() : CPropertyPage(CFokkerPumpPage::IDD)
{
	//{{AFX_DATA_INIT(CFokkerPumpPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	

	switch(theApp.m_nSide0Orientation) {
	case 0: m_nLeft = theApp.m_Tank.nLeftPump;
		m_nRight = theApp.m_Tank.nRightPump;
		break;
	case 1: m_nLeft = theApp.m_Tank.nRightPump;
		m_nRight = theApp.m_Tank.nLeftPump;
		break;
	}

}


void CFokkerPumpPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFokkerPumpPage)
	DDX_Control(pDX, IDC_COMBO_PUMP1_MODE, m_comboPumpMode1);
	DDX_Control(pDX, IDC_COMBO_PUMP0_MODE, m_comboPumpMode0);
	DDX_Control(pDX, IDC_EDIT_VERTICAL_MULTIPLIER_1, m_editVerticalMultiplier1);
	DDX_Control(pDX, IDC_EDIT_VERTICAL_MULTIPLIER_0, m_editVerticalMultiplier0);
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_staticRight);
	DDX_Control(pDX, IDC_STATIC_LEFT, m_staticLeft);
	DDX_Control(pDX, IDC_EDIT_VANE_ENCODER_1, m_editVaneEncoder1);
	DDX_Control(pDX, IDC_EDIT_VANE_ENCODER_0, m_editVaneEncoder0);
	DDX_Control(pDX, IDC_EDIT_PUMP_ENCODER_1, m_editPumpEncoder1);
	DDX_Control(pDX, IDC_EDIT_PUMP_ENCODER_0, m_editPumpEncoder0);
	DDX_Control(pDX, IDC_EDIT_SCALE_FACTOR2, m_editScaleFactor1);
	DDX_Control(pDX, IDC_EDIT_SCALE_FACTOR, m_editScaleFactor0);
	DDX_Control(pDX, IDC_EDIT_RIGHT_PHYSICAL_AXIS, m_editRightPhysicalAxis);
	DDX_Control(pDX, IDC_EDIT_LEFT_PHYSICAL_AXIS, m_editLeftPhysicalAxis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFokkerPumpPage, CPropertyPage)
	//{{AFX_MSG_MAP(CFokkerPumpPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_EN_CHANGE(IDC_EDIT_SCALE_FACTOR, OnChangeEditScaleFactor)
	ON_EN_CHANGE(IDC_EDIT_SCALE_FACTOR2, OnChangeEditScaleFactor2)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_VERTICAL_MULTIPLIER_0, OnChangeEditVerticalMultiplier0)
	ON_EN_CHANGE(IDC_EDIT_VERTICAL_MULTIPLIER_1, OnChangeEditVerticalMultiplier1)
	ON_CBN_SELCHANGE(IDC_COMBO_PUMP1_MODE, OnSelchangeComboPump1Mode)
	ON_CBN_SELCHANGE(IDC_COMBO_PUMP0_MODE, OnSelchangeComboPump0Mode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFokkerPumpPage message handlers

BOOL CFokkerPumpPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	m_staticLeft.SetWindowText(theApp.m_DlgSideName[0]);
	m_staticRight.SetWindowText(theApp.m_DlgSideName[1]);

	Buff.LoadString(IDS_Simple_jog);		m_comboPumpMode0.AddString(Buff);	m_comboPumpMode1.AddString(Buff);
	Buff.LoadString(IDS_Flow_sensor);		m_comboPumpMode0.AddString(Buff);	m_comboPumpMode1.AddString(Buff);
	Buff.LoadString(IDS_Height_multiplier);	m_comboPumpMode0.AddString(Buff);	m_comboPumpMode1.AddString(Buff);
	
	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CFokkerPumpPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CFokkerPumpPage::UpdateAllControls()
{
	CString Buff;

	Buff.Format(_T("%d"),theApp.m_Axes[m_nLeft].nPhysicalAxis);
	m_editLeftPhysicalAxis.SetWindowText(Buff);

	Buff.Format(_T("%d"),theApp.m_Axes[m_nRight].nPhysicalAxis);
	m_editRightPhysicalAxis.SetWindowText(Buff);

	switch(theApp.m_nSide0Orientation) {
	case 0:
		Buff.Format(_T("%.02f"),theApp.m_Tank.fPumpScaleFactor[0]);
		m_editScaleFactor0.SetWindowText(Buff);

		Buff.Format(_T("%.02f"),theApp.m_Tank.fPumpScaleFactor[1]);
		m_editScaleFactor1.SetWindowText(Buff);

		Buff.Format(L"%.03f",theApp.m_Tank.fPumpVerticalMultiplier[0]);
		m_editVerticalMultiplier0.SetWindowText(Buff);

		Buff.Format(L"%.03f",theApp.m_Tank.fPumpVerticalMultiplier[1]);
		m_editVerticalMultiplier1.SetWindowText(Buff);

		m_comboPumpMode0.SetCurSel(theApp.m_Tank.nPumpMode[0]);
		m_comboPumpMode1.SetCurSel(theApp.m_Tank.nPumpMode[1]);

		break;
	case 1:
		Buff.Format(_T("%.02f"),theApp.m_Tank.fPumpScaleFactor[1]);
		m_editScaleFactor0.SetWindowText(Buff);

		Buff.Format(_T("%.02f"),theApp.m_Tank.fPumpScaleFactor[0]);
		m_editScaleFactor1.SetWindowText(Buff);

		Buff.Format(L"%.03f",theApp.m_Tank.fPumpVerticalMultiplier[1]);
		m_editVerticalMultiplier0.SetWindowText(Buff);

		Buff.Format(L"%.03f",theApp.m_Tank.fPumpVerticalMultiplier[0]);
		m_editVerticalMultiplier1.SetWindowText(Buff);

		m_comboPumpMode0.SetCurSel(theApp.m_Tank.nPumpMode[1]);
		m_comboPumpMode1.SetCurSel(theApp.m_Tank.nPumpMode[0]);
		break;
	}

}

void CFokkerPumpPage::OnButtonApply() 
{
	CString Buff;

	m_editLeftPhysicalAxis.GetWindowText(Buff);
	theApp.m_Axes[m_nLeft].nPhysicalAxis = _ttoi(Buff);

	m_editRightPhysicalAxis.GetWindowText(Buff);
	theApp.m_Axes[m_nRight].nPhysicalAxis = _ttoi(Buff);
	
}

void CFokkerPumpPage::OnChangeEditScaleFactor() 
{
	CString Buff;
	
	m_editScaleFactor0.GetWindowText(Buff);
	switch(theApp.m_nSide0Orientation) {
	case 0:	_WTOF(Buff,theApp.m_Tank.fPumpScaleFactor[0]);
		break;
	case 1:	_WTOF(Buff,theApp.m_Tank.fPumpScaleFactor[1]);
		break;
	}
}

void CFokkerPumpPage::OnChangeEditScaleFactor2() 
{
	CString Buff;
	
	m_editScaleFactor1.GetWindowText(Buff);
	switch(theApp.m_nSide0Orientation) {
	case 0:	_WTOF(Buff,theApp.m_Tank.fPumpScaleFactor[1]);
		break;
	case 1:	_WTOF(Buff,theApp.m_Tank.fPumpScaleFactor[0]);
		break;
	}
}

void CFokkerPumpPage::OnTimer(UINT nIDEvent) 
{
	int	nArray[4];
	CString Buff;

	theApp.m_PmacUser.ReportPumpEncoderValues(nArray);

	switch(theApp.m_nSide0Orientation) {
	case 0:
		Buff.Format(_T("%d"),(nArray[0] - m_nOldArray[0]) * 2);
		m_editPumpEncoder0.SetWindowText(Buff);
		Buff.Format(_T("%d"),(nArray[1] - m_nOldArray[1]) * 2);
		m_editPumpEncoder1.SetWindowText(Buff);
		Buff.Format(_T("%d"),(nArray[2] - m_nOldArray[2]) * 2);
		m_editVaneEncoder0.SetWindowText(Buff);
		Buff.Format(_T("%d"),(nArray[3] - m_nOldArray[3]) * 2);
		m_editVaneEncoder1.SetWindowText(Buff);
		break;
	case 1:
		Buff.Format(_T("%d"),(nArray[1] - m_nOldArray[1]) * 2);
		m_editPumpEncoder0.SetWindowText(Buff);
		Buff.Format(_T("%d"),(nArray[0] - m_nOldArray[0]) * 2);
		m_editPumpEncoder1.SetWindowText(Buff);
		Buff.Format(_T("%d"),(nArray[3] - m_nOldArray[3]) * 2);
		m_editVaneEncoder0.SetWindowText(Buff);
		Buff.Format(_T("%d"),(nArray[2] - m_nOldArray[2]) * 2);
		m_editVaneEncoder1.SetWindowText(Buff);
		break;
	}

	CopyMemory(m_nOldArray,nArray,sizeof nArray);
	
	CPropertyPage::OnTimer(nIDEvent);
}

BOOL CFokkerPumpPage::OnSetActive() 
{
	SetTimer(1,500,NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CFokkerPumpPage::OnKillActive() 
{
	KillTimer(1);
	return CPropertyPage::OnKillActive();
}

void CFokkerPumpPage::OnChangeEditVerticalMultiplier0() 
{
	CString Buff;
	float fTemp;

	m_editVerticalMultiplier0.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	if(fTemp<-3.0f) fTemp = -3.0f;
	if(fTemp>3.0f) fTemp = 3.0f;

	switch(theApp.m_nSide0Orientation) {
	case 0:	theApp.m_Tank.fPumpVerticalMultiplier[0] = fTemp;
		break;
	case 1:	theApp.m_Tank.fPumpVerticalMultiplier[1] = fTemp;
		break;
	}
}

void CFokkerPumpPage::OnChangeEditVerticalMultiplier1() 
{
	CString Buff;
	float fTemp;

	m_editVerticalMultiplier1.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	if(fTemp<-3.0f) fTemp = 3.0f;
	if(fTemp>3.0f) fTemp = 3.0f;

	switch(theApp.m_nSide0Orientation) {
	case 0:	theApp.m_Tank.fPumpVerticalMultiplier[1] = fTemp;
		break;
	case 1:	theApp.m_Tank.fPumpVerticalMultiplier[0] = fTemp;
		break;
	}
}

void CFokkerPumpPage::OnSelchangeComboPump1Mode() 
{
	switch(theApp.m_nSide0Orientation) {
	case 0:	theApp.m_Tank.nPumpMode[1]=m_comboPumpMode1.GetCurSel();
		break;
	case 1:	theApp.m_Tank.nPumpMode[0]=m_comboPumpMode1.GetCurSel();
		break;
	}
}

void CFokkerPumpPage::OnSelchangeComboPump0Mode() 
{
	switch(theApp.m_nSide0Orientation) {
	case 0:	theApp.m_Tank.nPumpMode[0]=m_comboPumpMode0.GetCurSel();
		break;
	case 1:	theApp.m_Tank.nPumpMode[1]=m_comboPumpMode0.GetCurSel();
		break;
	}
}
