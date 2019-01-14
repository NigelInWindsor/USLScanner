// Pr15SelfTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "SelfTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelfTestDlg dialog
IMPLEMENT_DYNCREATE(CSelfTestDlg, CPropertyPage)

CSelfTestDlg::CSelfTestDlg() : CPropertyPage(CSelfTestDlg::IDD)
{
	
	//{{AFX_DATA_INIT(CSelfTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nTS = theApp.m_UtUser.m_Global.nTimeSlot;
	m_pPr15=&theApp.m_UtUser.m_TS[m_nTS].Pr15;
	m_pPl15=&theApp.m_UtUser.m_TS[m_nTS].Pl15;
	m_pPr30=&theApp.m_UtUser.m_TS[m_nTS].Pr30;
	m_nNumberSamples=theApp.m_UtUser.m_Cal[m_nTS].nArrayL;
	m_nTS = 0;

	m_nPulser = USL_PR30;
}


void CSelfTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelfTestDlg)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_EDIT_STEP_FREQUENCY, m_editStepFrequency);
	DDX_Control(pDX, IDC_EDIT_MIN_FREQUENCY, m_editMinFrequency);
	DDX_Control(pDX, IDC_EDIT_MAX_FREQUENCY, m_editMaxFrequency);
	DDX_Control(pDX, IDC_EDIT_AMPLITUDE, m_editAmplitude);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelfTestDlg, CDialog)
	//{{AFX_MSG_MAP(CSelfTestDlg)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_AMPLITUDE, OnChangeEditAmplitude)
	ON_BN_CLICKED(IDC_BUTTON_IMPLEMENT, OnButtonImplement)
	ON_EN_CHANGE(IDC_EDIT_MAX_FREQUENCY, OnChangeEditMaxFrequency)
	ON_EN_CHANGE(IDC_EDIT_MIN_FREQUENCY, OnChangeEditMinFrequency)
	ON_EN_CHANGE(IDC_EDIT_STEP_FREQUENCY, OnChangeEditStepFrequency)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelfTestDlg message handlers



BOOL CSelfTestDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	
	UpdateAllControls();

	Invalidate(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelfTestDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;
	

	Buff.Format(_T("%d"),0x3f - m_pPr30->nSelfTestAmplitude);
	m_editAmplitude.SetWindowText(Buff);

	Buff.Format(_T("%.01f MHz"),theApp.m_UtUser.m_Cal[m_nTS].fMinFrequency);
	m_editMinFrequency.SetWindowText(Buff);
	Buff.Format(_T("%.01f MHz"),theApp.m_UtUser.m_Cal[m_nTS].fMaxFrequency);
	m_editMaxFrequency.SetWindowText(Buff);
	Buff.Format(_T("%.01f MHz"),theApp.m_UtUser.m_Cal[m_nTS].fIncFrequency);
	m_editStepFrequency.SetWindowText(Buff);
}

void CSelfTestDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
//	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_FREQUENCY:
		break;
	}
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSelfTestDlg::OnChangeEditAmplitude() 
{
	
	int	nTemp;
	CString Buff;

	m_editAmplitude.GetWindowText(Buff);
	nTemp=0x3f - _ttoi(Buff);
	if(nTemp-m_pPr30->nSelfTestAmplitude) {
		m_pPr30->nSelfTestAmplitude=nTemp;
	};
}





void CSelfTestDlg::OnChangeEditMaxFrequency() 
{
	
	CString	Buff;

	m_editMaxFrequency.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_UtUser.m_Cal[m_nTS].fMaxFrequency);
	
}

void CSelfTestDlg::OnChangeEditMinFrequency() 
{
	
	CString	Buff;

	m_editMinFrequency.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_UtUser.m_Cal[m_nTS].fMinFrequency);
	
}


void CSelfTestDlg::OnChangeEditStepFrequency() 
{
	
	CString	Buff;

	m_editStepFrequency.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_UtUser.m_Cal[m_nTS].fIncFrequency);
	
}


void CSelfTestDlg::OnButtonImplement() 
{
	
	Pr15Data OldPr15;
	Pr30Data OldPr30;
	double	dFreq;
	int	ii;
	int	nLoop;

	OldPr15 = *m_pPr15;
	OldPr30 = *m_pPr30;

	switch(m_nPulser) {
	case USL_PR15:
		m_pPr15->tvg_enable=0;
		m_pPr15->bSelfTest=TRUE;
		break;
	case USL_PR30:
		m_pPr30->nDacEnable=0;
		m_pPr30->nSelfTestOffOn=TRUE;
		break;
	}
	theApp.m_LastSettings.nSelfTestOffOn=1;
	theApp.m_UtUser.Pr30DacEnable(m_pPr30);
	theApp.m_UtUser.Pr30SelfTestOnOff(m_pPr30);
	theApp.m_UtUser.Pr30SelfTestAmplitude(m_pPr30);

	theApp.m_UtUser.m_Cal[m_nTS].nArrayL=m_nNumberSamples=(int)((theApp.m_UtUser.m_Cal[m_nTS].fMaxFrequency-theApp.m_UtUser.m_Cal[m_nTS].fMinFrequency)/theApp.m_UtUser.m_Cal[m_nTS].fIncFrequency);
	SAFE_DELETE(theApp.m_UtUser.m_pnCalTrace[m_nTS]);
	theApp.m_UtUser.m_pnCalTrace[m_nTS] = new int[m_nNumberSamples];

	for(ii=0;ii<m_nNumberSamples;ii++) {
		dFreq=theApp.m_UtUser.m_Cal[m_nTS].fMinFrequency + (float)ii * theApp.m_UtUser.m_Cal[m_nTS].fIncFrequency;
		m_pPr30->nSelfTestFrequency=(int)(dFreq / 0.005);
		for(nLoop=0;nLoop<2;nLoop++) {
			theApp.m_UtUser.Pr15SelfTestFrequency(m_pPr15);
			theApp.m_UtUser.Pr30SelfTestFrequency(m_pPr30);
			Sleep(50);
			if((theApp.m_UtUser.m_pnCalTrace[m_nTS][ii]=theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nAmplitude[0]) > 5) {
				break;
			}
		}
		DisplayResultsTrace();
	};

	theApp.m_LastSettings.nSelfTestOffOn=0;

	*m_pPr15 = OldPr15;
	*m_pPr30 = OldPr30;
	theApp.m_UtUser.Pr15SelfTestOnOff(m_pPr15);
	theApp.m_UtUser.Pr15SelfTestAmplitude(m_pPr15);
	theApp.m_UtUser.Pr15TvgCommands(m_pPr15);
	theApp.m_UtUser.Pr15SelfTestFrequency(m_pPr15);

	theApp.m_UtUser.Pr30SelfTestOnOff(m_pPr30);
	theApp.m_UtUser.Pr30SelfTestAmplitude(m_pPr30);
	theApp.m_UtUser.Pr30DacEnable(m_pPr30);
	theApp.m_UtUser.Pr30SelfTestFrequency(m_pPr30);

}

void CSelfTestDlg::OnPaint() 
{

	CPaintDC dc(this); // device context for painting

	DisplayResultsTrace(&dc);
}

void CSelfTestDlg::DisplayResultsTrace(CPaintDC* pPaintDC)
{
	if(m_nNumberSamples<=0) return;
	
	int	ii,nPnX,nPnY;
	CRect rr;
	double	dFreq=theApp.m_UtUser.m_Cal[m_nTS].fMaxFrequency - theApp.m_UtUser.m_Cal[m_nTS].fMinFrequency;
	CString	Buff;
	CSize ll;

	CDC* pDC;
	
	if(pPaintDC == NULL) {
		pDC = GetDC();
	} else {
		pDC = (CDC*)pPaintDC;
	}

	LOGFONT lf;
	memset( (void*)&lf, 0, sizeof(lf));
	lf.lfHeight = 14;
	theApp.SetFontLanguage(&lf);

	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);
	
	m_staticView.GetWindowRect(rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2,2,2);

	CBrush Brush(RGB(255,255,255));
	CBrush * pOldBrush = pDC->SelectObject(&Brush);
	pDC->FillRect(rr,&Brush);
	pDC->SelectObject(pOldBrush);
	CPen Pen(PS_SOLID  ,1,RGB(0,0,0));
	CPen penBlue(PS_SOLID  ,1,RGB(0,0,255));
	CPen* pOldPen = pDC->SelectObject(&Pen);

	if(theApp.m_UtUser.m_pnCalTrace[m_nTS] != NULL) {
		for(ii=0;ii<m_nNumberSamples;ii++) {
			nPnX = MulDiv(ii,rr.Width(),m_nNumberSamples-1) + rr.left;
			nPnY=rr.bottom-MulDiv(theApp.m_UtUser.m_pnCalTrace[m_nTS][ii],rr.Height(),127);
			if(ii==0) {
				pDC->MoveTo(nPnX,nPnY);
			} else {
				pDC->LineTo(nPnX,nPnY);
			}
		};
	}
	if((theApp.m_UtUser.m_Cal[m_nTS].bDefaultTraceTrue == TRUE) && (theApp.m_UtUser.m_pnDefaultCalTrace[m_nTS] != NULL)) {
		pDC->SelectObject(&penBlue);
		for(ii=0;ii<m_nNumberSamples;ii++) {
			nPnX = MulDiv(ii,rr.Width(),m_nNumberSamples-1) + rr.left;
			nPnY=rr.bottom-MulDiv(theApp.m_UtUser.m_pnDefaultCalTrace[m_nTS][ii],rr.Height(),127);
			if(ii==0) {
				pDC->MoveTo(nPnX,nPnY);
			} else {
				pDC->LineTo(nPnX,nPnY);
			}
		};
	}

	CTime time(theApp.m_UtUser.m_Cal[m_nTS].m_osTestDateTime);

	Buff.Format(_T("%s Default Signal Amplitue:%d"),time.Format(_T("%d/%m/%Y")),theApp.m_UtUser.m_Cal[m_nTS].nDefaultSignalAmplitude);
	pDC->TextOut(rr.left,rr.top,Buff);

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);

	if(pPaintDC == NULL) {
		ReleaseDC(pDC);
	}

}


void CSelfTestDlg::OnButtonDefault() 
{
	
	CTime CurrentTime = CTime::GetCurrentTime();
	theApp.m_UtUser.m_Cal[m_nTS].m_osTestDateTime=(int)CurrentTime.GetTime();

	SAFE_DELETE(theApp.m_UtUser.m_pnDefaultCalTrace[m_nTS]);
	theApp.m_UtUser.m_pnDefaultCalTrace[m_nTS] = new int[theApp.m_UtUser.m_Cal[m_nTS].nArrayL];

	CopyMemory(theApp.m_UtUser.m_pnDefaultCalTrace[m_nTS],theApp.m_UtUser.m_pnCalTrace[m_nTS],theApp.m_UtUser.m_Cal[m_nTS].nArrayL * sizeof (int));
	theApp.m_UtUser.m_Cal[m_nTS].bDefaultTraceTrue = TRUE;
	theApp.m_UtUser.m_Cal[m_nTS].nDefaultSignalAmplitude = 0x3f - m_pPr30->nSelfTestAmplitude;
	Invalidate(FALSE);

}
