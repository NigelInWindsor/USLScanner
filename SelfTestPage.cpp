// Pr15SelfTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "SelfTestPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelfTestPage dialog
IMPLEMENT_DYNCREATE(CSelfTestPage, CPropertyPage)

CSelfTestPage::CSelfTestPage() : CPropertyPage(CSelfTestPage::IDD)
{
	APP App = (APP) AfxGetApp();
	//{{AFX_DATA_INIT(CSelfTestPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bDialogCreated=FALSE;
	m_pPr15=&App->m_UtUser.m_TS[App->m_UtUser.m_Global.nTimeSlot].Pr15;
	m_pPl15=&App->m_UtUser.m_TS[App->m_UtUser.m_Global.nTimeSlot].Pl15;
	m_pPr30=&App->m_UtUser.m_TS[App->m_UtUser.m_Global.nTimeSlot].Pr30;
	m_pResults=NULL;
	m_nNumberSamples=0;
}


void CSelfTestPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelfTestPage)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_EDIT_NOM_ATTENUATION, m_editNomAttenuation);
	DDX_Control(pDX, IDC_EDIT_MAIN_GAIN, m_editMainGain);
	DDX_Control(pDX, IDC_EDIT_STEP_FREQUENCY, m_editStepFrequency);
	DDX_Control(pDX, IDC_EDIT_MIN_FREQUENCY, m_editMinFrequency);
	DDX_Control(pDX, IDC_EDIT_MAX_FREQUENCY, m_editMaxFrequency);
	DDX_Control(pDX, IDC_SPIN_FREQUENCY, m_spinFrequency);
	DDX_Control(pDX, IDC_SPIN_AMPLITUDE, m_spinAmplitude);
	DDX_Control(pDX, IDC_EDIT_FREQUENCY, m_editFrequency);
	DDX_Control(pDX, IDC_EDIT_AMPLITUDE, m_editAmplitude);
	DDX_Control(pDX, IDC_COMBO_OFF_ON, m_comboOffOn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelfTestPage, CDialog)
	//{{AFX_MSG_MAP(CSelfTestPage)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_AMPLITUDE, OnChangeEditAmplitude)
	ON_EN_CHANGE(IDC_EDIT_FREQUENCY, OnChangeEditFrequency)
	ON_CBN_SELCHANGE(IDC_COMBO_OFF_ON, OnSelchangeComboOffOn)
	ON_BN_CLICKED(IDC_BUTTON_IMPLEMENT, OnButtonImplement)
	ON_EN_CHANGE(IDC_EDIT_MAIN_GAIN, OnChangeEditMainGain)
	ON_EN_CHANGE(IDC_EDIT_MAX_FREQUENCY, OnChangeEditMaxFrequency)
	ON_EN_CHANGE(IDC_EDIT_MIN_FREQUENCY, OnChangeEditMinFrequency)
	ON_EN_CHANGE(IDC_EDIT_NOM_ATTENUATION, OnChangeEditNomAttenuation)
	ON_EN_CHANGE(IDC_EDIT_STEP_FREQUENCY, OnChangeEditStepFrequency)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelfTestPage message handlers


BOOL CSelfTestPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString	Buff;
	APP App = (APP) AfxGetApp();


	Buff.Format("%.0f KHz",(double)App->m_LastSettings.nSelfTestFrequency*5.0);
	m_editFrequency.SetWindowText(Buff);
	m_spinFrequency.SetRange(80,4095);
	m_spinFrequency.SetPos(App->m_LastSettings.nSelfTestFrequency);


	Buff.Format("%d dB",App->m_LastSettings.nSelfTestAmplitude);
	m_editAmplitude.SetWindowText(Buff);
	m_spinAmplitude.SetRange(0,93);
	m_spinAmplitude.SetPos(App->m_LastSettings.nSelfTestAmplitude);

	m_comboOffOn.AddString("Off");
	m_comboOffOn.AddString("On");
	m_comboOffOn.SetCurSel((int)App->m_LastSettings.nSelfTestOffOn);


	Buff.Format("%.01f MHz",App->m_LastSettings.fSelfTestMinFrequency);
	m_editMinFrequency.SetWindowText(Buff);
	Buff.Format("%.01f MHz",App->m_LastSettings.fSelfTestMaxFrequency);
	m_editMaxFrequency.SetWindowText(Buff);
	Buff.Format("%.01f MHz",App->m_LastSettings.fSelfTestStepFrequency);
	m_editStepFrequency.SetWindowText(Buff);
	Buff.Format("%d dB",App->m_LastSettings.nSelfTestNomAttenuation);
	m_editNomAttenuation.SetWindowText(Buff);
	Buff.Format("%.01f dB",App->m_LastSettings.fSelfTestMainGain);
	m_editMainGain.SetWindowText(Buff);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelfTestPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	APP App = (APP) AfxGetApp();
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
//	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_FREQUENCY:
		App->m_LastSettings.nSelfTestFrequency=(pSpin->GetPos()&0xffff);
		Buff.Format("%.0f KHz",(double)App->m_LastSettings.nSelfTestFrequency*5.0);
		m_editFrequency.SetWindowText(Buff);
		App->m_UtUser.Pr15SelfTestFrequency(m_pPr15);
		App->m_UtUser.Pr30SelfTestFrequency(m_pPr30);
		break;
	case IDC_SPIN_AMPLITUDE:
		App->m_LastSettings.nSelfTestAmplitude=(pSpin->GetPos()&0xffff);
		Buff.Format("%d dB",App->m_LastSettings.nSelfTestAmplitude);
		m_editAmplitude.SetWindowText(Buff);
		App->m_UtUser.Pr15SelfTestAmplitude(m_pPr15);
		App->m_UtUser.Pr30SelfTestAmplitude(m_pPr30);
		break;
	}
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSelfTestPage::OnChangeEditAmplitude() 
{
	APP App = (APP) AfxGetApp();
	int	nTemp;
	CString Buff;

	m_editAmplitude.GetWindowText(Buff);
	nTemp=atoi(Buff);
	if(nTemp-App->m_LastSettings.nSelfTestAmplitude) {
		App->m_LastSettings.nSelfTestAmplitude=nTemp;
		App->m_UtUser.Pr15SelfTestAmplitude(m_pPr15);
		App->m_UtUser.Pr30SelfTestAmplitude(m_pPr30);
		m_spinAmplitude.SetPos(App->m_LastSettings.nSelfTestAmplitude);
	};
}

void CSelfTestPage::OnChangeEditFrequency() 
{
	APP App = (APP) AfxGetApp();
	int nTemp;
	CString Buff;

	m_editFrequency.GetWindowText(Buff);
	nTemp=atoi(Buff)/5;
	if(nTemp-App->m_LastSettings.nSelfTestFrequency) {
		if(nTemp<80) nTemp=80;
		if(nTemp>4095) nTemp=4095;
		App->m_LastSettings.nSelfTestFrequency=nTemp;
		App->m_UtUser.Pr15SelfTestFrequency(m_pPr15);
		App->m_UtUser.Pr30SelfTestFrequency(m_pPr30);
		m_spinFrequency.SetPos(App->m_LastSettings.nSelfTestFrequency);
	};
}

void CSelfTestPage::OnSelchangeComboOffOn() 
{
	APP App =(APP) AfxGetApp();

	App->m_LastSettings.nSelfTestOffOn=m_comboOffOn.GetCurSel();
	App->m_UtUser.Pr15SelfTestOnOff(m_pPr15);
	App->m_UtUser.Pr30SelfTestOnOff(m_pPr30);
	
}


void CSelfTestPage::OnChangeEditMainGain() 
{
	APP App =(APP) AfxGetApp();
	CString	Buff;

	m_editMainGain.GetWindowText(Buff);
	App->m_LastSettings.fSelfTestMainGain=(float)atof(Buff);
	
}

void CSelfTestPage::OnChangeEditMaxFrequency() 
{
	APP App =(APP) AfxGetApp();
	CString	Buff;

	m_editMaxFrequency.GetWindowText(Buff);
	App->m_LastSettings.fSelfTestMaxFrequency=(float)atof(Buff);
	
}

void CSelfTestPage::OnChangeEditMinFrequency() 
{
	APP App =(APP) AfxGetApp();
	CString	Buff;

	m_editMinFrequency.GetWindowText(Buff);
	App->m_LastSettings.fSelfTestMinFrequency=(float)atof(Buff);
	
}

void CSelfTestPage::OnChangeEditNomAttenuation() 
{
	APP App =(APP) AfxGetApp();
	CString	Buff;

	m_editNomAttenuation.GetWindowText(Buff);
	App->m_LastSettings.nSelfTestNomAttenuation=atoi(Buff);
	
}

void CSelfTestPage::OnChangeEditStepFrequency() 
{
	APP App =(APP) AfxGetApp();
	CString	Buff;

	m_editStepFrequency.GetWindowText(Buff);
	App->m_LastSettings.fSelfTestStepFrequency=(float)atof(Buff);
	
}


void CSelfTestPage::OnButtonImplement() 
{
	APP App =(APP) AfxGetApp();
	Pr15Data OldPr15;
	Pr30Data OldPr30;
	float	fFreq;
	int	ii,gg;

	OldPr15 = *m_pPr15;
	OldPr30 = *m_pPr30;

	m_pPr15->tvg_enable=0;
	m_pPr15->nMainGain=(int)(App->m_LastSettings.fSelfTestMainGain*2.0) + 56;
	m_pPr15->nSelfTestAmplitude=App->m_LastSettings.nSelfTestNomAttenuation;
	m_pPr15->bSelfTest=TRUE;

	m_pPr30->nDacEnable=0;
	m_pPr30->fLinGain = App->m_LastSettings.fSelfTestMainGain;
	m_pPr30->nSelfTestAmplitude=App->m_LastSettings.nSelfTestNomAttenuation;
	m_pPr30->nSelfTestOffOn=TRUE;

	App->m_UtUser.Pr15SelfTestAmplitude(m_pPr15);
	App->m_UtUser.Pr15Gain(m_pPr15);
	App->m_UtUser.Pr15TvgCommands(m_pPr15);
	App->m_UtUser.Pr30SelfTestAmplitude(m_pPr30);
	App->m_UtUser.Pr30LinFloatGain(m_pPr30);
	App->m_UtUser.Pr30DacEnable(m_pPr30);
	Sleep(100);

	m_nNumberSamples=(int)((App->m_LastSettings.fSelfTestMaxFrequency-App->m_LastSettings.fSelfTestMinFrequency)/App->m_LastSettings.fSelfTestStepFrequency);
	m_nNumberSamples++;
	if(m_pResults!=NULL) delete m_pResults;
	m_pResults = new ResultsData[m_nNumberSamples];

	for(gg=0;gg<2;gg++) {
		if(gg==1) {
			m_pPr15->nMainGain-=12;
			App->m_UtUser.Pr15Gain(m_pPr15);
			m_pPr30->fLinGain-=12;
			App->m_UtUser.Pr30LinFloatGain(m_pPr30);
		}

		for(fFreq=App->m_LastSettings.fSelfTestMinFrequency,ii=0;fFreq<=App->m_LastSettings.fSelfTestMaxFrequency;fFreq+=App->m_LastSettings.fSelfTestStepFrequency,ii++) {
			App->m_LastSettings.nSelfTestFrequency=(int)(fFreq / 0.005);
//			App->m_UtUser.Pr15SelfTestFrequency(m_pPr15);
			App->m_UtUser.Pr30SelfTestFrequency(m_pPr30);
			Sleep(1000);
			m_pResults[ii].dFrequency=fFreq;
			m_pResults[ii].nAmplitude[gg]=App->m_UtUser.m_TS[App->m_UtUser.m_Global.nTimeSlot].Gate.nAmplitude[0];
			Invalidate(TRUE);
		};
	};


	*m_pPr15 = OldPr15;
	App->m_UtUser.Pr15SelfTestOnOff(m_pPr15);
	App->m_UtUser.Pr15SelfTestAmplitude(m_pPr15);
	App->m_UtUser.Pr15Gain(m_pPr15);
	App->m_UtUser.Pr15TvgCommands(m_pPr15);
	App->m_UtUser.Pr15SelfTestFrequency(m_pPr15);

	*m_pPr30 = OldPr30;
	App->m_UtUser.Pr30SelfTestOnOff(m_pPr30);
	App->m_UtUser.Pr30SelfTestAmplitude(m_pPr30);
	App->m_UtUser.Pr30LinFloatGain(m_pPr30);
	App->m_UtUser.Pr30DacEnable(m_pPr30);
	App->m_UtUser.Pr30SelfTestFrequency(m_pPr30);

	Invalidate(TRUE);


}

void CSelfTestPage::OnPaint() 
{
	APP App = (APP) AfxGetApp();
	CPaintDC dc(this); // device context for painting

	if(m_nNumberSamples<=0) return;
	int	ii,nPnX,nPnY,gg;
	CRect rr;
	double	dFreq=App->m_LastSettings.fSelfTestMaxFrequency-App->m_LastSettings.fSelfTestMinFrequency;
	CString	Buff;
	CSize ll;

	LOGFONT lf;
	memset( (void*)&lf, 0, sizeof(lf));
	lf.lfHeight = 10;
	_tcscpy(lf.lfFaceName, "Times");

	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = dc.SelectObject(&Font);
	dc.SetBkMode(TRANSPARENT);
	
	m_staticView.GetWindowRect(rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2,2,2);

	CBrush Brush(RGB(255,255,255));
	CBrush * pOldBrush = dc.SelectObject(&Brush);
	dc.FillRect(rr,&Brush);
	dc.SelectObject(pOldBrush);
	CPen Pen(PS_SOLID  ,1,RGB(0,0,0));
	CPen* pOldPen = dc.SelectObject(&Pen);

	for(gg=0;gg<2;gg++) {
		for(ii=0;ii<m_nNumberSamples;ii++) {
			nPnX=(int)(((m_pResults[ii].dFrequency-App->m_LastSettings.fSelfTestMinFrequency)*(double)rr.Width()) / dFreq)+rr.left;
			nPnY=rr.bottom-MulDiv(m_pResults[ii].nAmplitude[gg],rr.Height(),127);
			if(ii==0) {
				dc.MoveTo(nPnX,nPnY);
			} else {
				dc.LineTo(nPnX,nPnY);
			}
		}
	};

	for(ii=0;ii<m_nNumberSamples;ii++) {
		nPnX=(int)(((m_pResults[ii].dFrequency-App->m_LastSettings.fSelfTestMinFrequency)*(double)rr.Width()) / dFreq)+rr.left;
		dc.MoveTo(nPnX,rr.bottom);
		dc.LineTo(nPnX,rr.bottom+4);
		Buff.Format("%.01f",m_pResults[ii].dFrequency);
		ll=dc.GetTextExtent(Buff);
		dc.TextOut(nPnX-ll.cx/2,rr.bottom+2,Buff);
	}
	dc.SelectObject(pOldFont);
	dc.SelectObject(pOldPen);

}
