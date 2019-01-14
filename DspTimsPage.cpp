// DspTimsPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "DspTimsPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDspTimsPage dialog

IMPLEMENT_DYNCREATE(CDspTimsPage, CPropertyPage)

CDspTimsPage::CDspTimsPage() : CPropertyPage(CDspTimsPage::IDD)
{
	//{{AFX_DATA_INIT(CDspTimsPage)
	//}}AFX_DATA_INIT
	m_bDialogCreated=FALSE;
}

CDspTimsPage::~CDspTimsPage()
{
}

void CDspTimsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDspTimsPage)
	DDX_Control(pDX, IDC_CHECK_COLLECT_1, m_checkCollectMetalPath);
	DDX_Control(pDX, IDC_EDIT_MATERIAL_DENSITY, m_editMaterialDensity);
	DDX_Control(pDX, IDC_STATIC_MATERIAL_PATH, m_staticMaterialPath);
	DDX_Control(pDX, IDC_STATIC_WATER_PATH, m_staticWaterPath);
	DDX_Control(pDX, IDC_SPIN_WATER_VELOCITY, m_spinWaterVelocity);
	DDX_Control(pDX, IDC_SPIN_MATERIAL_VELOCITY, m_spinMaterialVelocity);
	DDX_Control(pDX, IDC_EDITMATERIAL_VELOCITY, m_editMaterialVelocity);
	DDX_Control(pDX, IDC_EDIT_WATER_VELOCITY, m_editWaterVelocity);
	DDX_Control(pDX, IDC_COMBOSTART_SLOPE, m_comboStartSlope);
	DDX_Control(pDX, IDC_COMBO_STOP_SLOPE, m_comboStopSlope);
	DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
	DDX_Control(pDX, IDC_COMBO_DETECTION, m_comboDetection);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDspTimsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDspTimsPage)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, OnSelchangeComboMode)
	ON_CBN_SELCHANGE(IDC_COMBO_DETECTION, OnSelchangeComboDetection)
	ON_CBN_SELCHANGE(IDC_COMBOSTART_SLOPE, OnSelchangeCombostartSlope)
	ON_CBN_SELCHANGE(IDC_COMBO_STOP_SLOPE, OnSelchangeComboStopSlope)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_WATER_VELOCITY, OnChangeEditWaterVelocity)
	ON_EN_CHANGE(IDC_EDITMATERIAL_VELOCITY, OnChangeEditmaterialVelocity)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_WATER_VELOCITY, OnButtonWaterVelocity)
	ON_EN_CHANGE(IDC_EDIT_MATERIAL_DENSITY, OnChangeEditMaterialDensity)
	ON_BN_CLICKED(IDC_CHECK_COLLECT_1, OnCheckCollect1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDspTimsPage message handlers

BOOL CDspTimsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	APP App = (APP) AfxGetApp();
	CString Buff;

	m_comboMode.AddString("Off");
	m_comboMode.AddString("MB -> Interface");
	m_comboMode.AddString("MB -> Gate 1");
	m_comboMode.AddString("Interface -> Gate 1");

	m_comboDetection.AddString("Edge");
	m_comboDetection.AddString("Peak");

	m_comboStartSlope.AddString("Rising");
	m_comboStartSlope.AddString("Falling");

	m_comboStopSlope.AddString("Rising");
	m_comboStopSlope.AddString("Falling");

	m_spinWaterVelocity.SetRange(1000,2000);
	m_spinMaterialVelocity.SetRange(1000,7000);

	SetTimer(1,200,NULL);

	m_bDialogCreated=TRUE;
	UpdateAllControls();
	SetAccessPrivelage();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDspTimsPage::OnSelchangeComboMode() 
{
	APP App = (APP) AfxGetApp();
	App->m_UtUser.m_TS[App->m_UtUser.m_Global.nTimeSlot].Gate.nTimsOffOn[0]=m_comboMode.GetCurSel();
	App->m_UtUser.Dsp200CalculateTimsHardware(App->m_UtUser.m_Global.nTimeSlot,0);
}

void CDspTimsPage::OnSelchangeComboDetection() 
{
	APP App = (APP) AfxGetApp();
	App->m_UtUser.m_TS[App->m_UtUser.m_Global.nTimeSlot].Gate.nTimsDetectionMode[0]=m_comboDetection.GetCurSel();
	App->m_UtUser.Dsp200CalculateTimsHardware(App->m_UtUser.m_Global.nTimeSlot,0);
}

void CDspTimsPage::OnSelchangeCombostartSlope() 
{
	APP App = (APP) AfxGetApp();
	App->m_UtUser.m_TS[App->m_UtUser.m_Global.nTimeSlot].Gate.nTimsStartSlope[0]=m_comboStartSlope.GetCurSel();
	App->m_UtUser.Dsp200CalculateTimsHardware(App->m_UtUser.m_Global.nTimeSlot,0);
	
}

void CDspTimsPage::OnSelchangeComboStopSlope() 
{
	APP App = (APP) AfxGetApp();
	App->m_UtUser.m_TS[App->m_UtUser.m_Global.nTimeSlot].Gate.nTimsStopSlope[0]=m_comboStopSlope.GetCurSel();
	App->m_UtUser.Dsp200CalculateTimsHardware(App->m_UtUser.m_Global.nTimeSlot,0);
	
}

void CDspTimsPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	APP App = (APP) AfxGetApp();
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CString	Buff;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_WATER_VELOCITY:
		App->m_LastSettings.nWaterVelocity=(pSpin->GetPos()&0xffff);
		Buff.Format("%d m/s",App->m_LastSettings.nWaterVelocity);
		m_editWaterVelocity.SetWindowText(Buff);
		break;
	case IDC_SPIN_MATERIAL_VELOCITY:
		App->m_LastSettings.nMaterialVelocity=(pSpin->GetPos()&0xffff);
		Buff.Format("%d m/s",App->m_LastSettings.nMaterialVelocity);
		m_editMaterialVelocity.SetWindowText(Buff);
		break;
	}	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDspTimsPage::OnChangeEditWaterVelocity() 
{
	APP App = (APP) AfxGetApp();
	int	nTemp;
	CString	Buff;

	m_editWaterVelocity.GetWindowText(Buff);
	nTemp=atoi(Buff);
	if(App->m_LastSettings.nWaterVelocity-nTemp) {
		App->m_LastSettings.nWaterVelocity=nTemp;
		m_spinWaterVelocity.SetPos(nTemp);
	};

}

void CDspTimsPage::OnChangeEditmaterialVelocity() 
{
	APP App = (APP) AfxGetApp();
	int	nTemp;
	CString	Buff;

	m_editMaterialVelocity.GetWindowText(Buff);
	nTemp=atoi(Buff);
	if(App->m_LastSettings.nMaterialVelocity-nTemp) {
		App->m_LastSettings.nMaterialVelocity=nTemp;
		m_spinMaterialVelocity.SetPos(nTemp);
	};
	
}

void CDspTimsPage::OnTimer(UINT nIDEvent) 
{
	APP App =(APP) AfxGetApp();
	static int nOldStartTime=-1;
	static int nOldStopTime=-1;
	static int nOldMaterialVelocity=-1;
	static int nOldWaterVelocity=-1;
	int	nStart,nStop;
	CString	Buff;
	CRect rr;
	double dPath;

	
	App->m_UtUser.Dsp200GetStartStopTimeNs(&nStart,&nStop);

	if((nOldStopTime-nStop) || ( nOldMaterialVelocity-App->m_LastSettings.nMaterialVelocity) ||
		(nOldStartTime-nStart) || ( nOldWaterVelocity-App->m_LastSettings.nWaterVelocity)) {
		nOldStopTime=nStop;
		nOldStartTime=nStart;
		nOldMaterialVelocity=App->m_LastSettings.nMaterialVelocity;
		nOldWaterVelocity=App->m_LastSettings.nWaterVelocity;

		m_staticWaterPath.GetWindowRect(rr);
		ScreenToClient(&rr);
		rr.top+=3;
		rr.bottom-=3;
		rr.left+=3;
		rr.right-=3;
		LOGFONT lf;
		memset( (void*)&lf, 0, sizeof(lf));
		lf.lfHeight = rr.Height();
		_tcscpy(lf.lfFaceName, _T("Times"));		//Times

		CFont Font;
		Font.CreateFontIndirect(&lf);
		CDC* pDC=GetDC();
		CFont* pOldFont = pDC->SelectObject(&Font);

		CBrush Brush(RGB(255,255,255));
		CBrush* pOldBrush=pDC->SelectObject(&Brush);

		dPath= ((double)nStart*(double)App->m_LastSettings.nWaterVelocity) / 2e6;
		if(dPath>200) dPath=0.0;
		pDC->FillRect(rr,&Brush);
		Buff.Format("%.02f mm",dPath);
		pDC->TextOut(rr.left,rr.top,Buff);

		m_staticMaterialPath.GetWindowRect(rr);
		ScreenToClient(&rr);
		rr.top+=3;
		rr.bottom-=3;
		rr.left+=3;
		rr.right-=3;
		dPath= ((double)(nStop-nStart+App->m_LastSettings.nMaterialConstant)*(double)App->m_LastSettings.nMaterialVelocity) / 2e6;
		if(dPath>200) dPath=0.0;
		pDC->FillRect(rr,&Brush);
		Buff.Format("%.02f mm",dPath);
		pDC->TextOut(rr.left,rr.top,Buff);


		Buff.Format("%d m/s",App->m_LastSettings.nWaterVelocity);
		m_editWaterVelocity.SetWindowText(Buff);

		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldFont);
		ReleaseDC(pDC);
	};

	UpdateData(FALSE);	
	CPropertyPage::OnTimer(nIDEvent);
}

void CDspTimsPage::UpdateAllControls()
{
	APP App = (APP) AfxGetApp();
	CString	Buff;

	if(	m_bDialogCreated==FALSE) return;

	m_comboMode.SetCurSel(App->m_UtUser.m_TS[App->m_UtUser.m_Global.nTimeSlot].Gate.nTimsOffOn[0]);
	m_comboDetection.SetCurSel(App->m_UtUser.m_TS[App->m_UtUser.m_Global.nTimeSlot].Gate.nTimsDetectionMode[0]);
	m_comboStartSlope.SetCurSel(App->m_UtUser.m_TS[App->m_UtUser.m_Global.nTimeSlot].Gate.nTimsStartSlope[0]);
	m_comboStopSlope.SetCurSel(App->m_UtUser.m_TS[App->m_UtUser.m_Global.nTimeSlot].Gate.nTimsStopSlope[0]);

	Buff.Format("%d m/s",App->m_LastSettings.nWaterVelocity);
	m_editWaterVelocity.SetWindowText(Buff);
	Buff.Format("%d m/s",App->m_LastSettings.nMaterialVelocity);
	m_editMaterialVelocity.SetWindowText(Buff);
	m_spinWaterVelocity.SetPos(App->m_LastSettings.nWaterVelocity);
	m_spinMaterialVelocity.SetPos(App->m_LastSettings.nMaterialVelocity);

	Buff.Format("%g",App->m_LastSettings.fMaterialDensity);
	m_editMaterialDensity.SetWindowText(Buff);

	m_checkCollectMetalPath.SetCheck(App->m_UtUser.m_TS[App->m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[1]&1);


}

void CDspTimsPage::OnButtonWaterVelocity() 
{
	APP App = (APP) AfxGetApp();
	
}

void CDspTimsPage::SetAccessPrivelage()
{
	if(m_bDialogCreated!=TRUE) return;
	APP App = (APP) AfxGetApp();
	bool bFlag=FALSE;

	if(App->m_nAccessLevel>=SUPERVISOR) bFlag=TRUE;

	m_comboMode.EnableWindow(bFlag);
	m_comboDetection.EnableWindow(bFlag);
	m_comboStartSlope.EnableWindow(bFlag);
	m_comboStopSlope.EnableWindow(bFlag);
	m_editWaterVelocity.EnableWindow(bFlag);
	m_editMaterialVelocity.EnableWindow(bFlag);
	m_spinWaterVelocity.EnableWindow(bFlag);
	m_spinMaterialVelocity.EnableWindow(bFlag);

}

void CDspTimsPage::OnChangeEditMaterialDensity() 
{
	APP App = (APP) AfxGetApp();
	CString Buff;

	m_editMaterialDensity.GetWindowText(Buff);
	App->m_LastSettings.fMaterialDensity = (float)atof(Buff);
}

void CDspTimsPage::OnCheckCollect1() 
{
	APP App = (APP) AfxGetApp();

	App->m_UtUser.m_TS[App->m_UtUser.m_Global.nTimeSlot].Gate.nCollectTime[1]=m_checkCollectMetalPath.GetCheck()&1;
}

BOOL CDspTimsPage::OnSetActive() 
{

	UpdateAllControls();
	return CPropertyPage::OnSetActive();
}
