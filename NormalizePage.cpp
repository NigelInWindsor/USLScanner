// NormalizePage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "NormalizePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNormalizePage property page

IMPLEMENT_DYNCREATE(CNormalizePage, CPropertyPage)

CNormalizePage::CNormalizePage() : CPropertyPage(CNormalizePage::IDD)
{
	//{{AFX_DATA_INIT(CNormalizePage)
	//}}AFX_DATA_INIT
	m_pRectilinearArray = FALSE;
	m_RectilinearRect.SetRectEmpty();

	theApp.m_LastSettings.nNormSide &= 1;
	theApp.m_LastSettings.nPeakSide &= 1;

	m_nImageOrientation = 1;

	m_nPaintMode = 0;
}

CNormalizePage::~CNormalizePage()
{
	SAFE_DELETE(m_pRectilinearArray);
}

void CNormalizePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNormalizePage)
	DDX_Control(pDX, IDC_BUTTON_MOVETO_RIGHT, m_buttonMove1To2);
	DDX_Control(pDX, IDC_BUTTON_MOVETO_LEFT, m_buttonMove2To1);
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_staticRight);
	DDX_Control(pDX, IDC_STATIC_LEFT, m_staticLeft);
	DDX_Control(pDX, IDC_COMBO_RIGHT_WP_TS, m_comboRightWpTS);
	DDX_Control(pDX, IDC_COMBO_LEFT_WP_TS, m_comboLeftWpTS);
	DDX_Control(pDX, IDC_COMBO_PEAK_TIMESLOT, m_comboPeakTimeSlot);
	DDX_Control(pDX, IDC_COMBO_TIMESLOT, m_comboNormTimeSlot);
	DDX_Control(pDX, IDC_COMBO_NORM_SIDE, m_comboNormSide);
	DDX_Control(pDX, IDC_COMBO_PEAK_SIDE, m_comboPeakSide);
	DDX_Control(pDX, IDC_COMBO_INSPECTION_MODE, m_comboPeakOrthogonalMode);
	DDX_Control(pDX, IDC_COMBO_PEAK_MODE, m_comboPeakMode);
	DDX_Control(pDX, IDC_CHECK_YTIP, m_checkYTip);
	DDX_Control(pDX, IDC_CHECK_XTIP, m_checkXTip);
	DDX_Control(pDX, IDC_EDIT_DISTANCE, m_editDistance);
	DDX_Control(pDX, IDC_SPIN_DISTANCE, m_spinDistance);
	DDX_Control(pDX, IDC_COMBO_GATE2, m_comboGatePeak);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_COMBO_GATE, m_comboGate);
	DDX_Control(pDX, IDC_SPIN_ANGLE, m_spinAngle);
	DDX_Control(pDX, IDC_EDIT_ANGLE, m_editAngle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNormalizePage, CPropertyPage)
	//{{AFX_MSG_MAP(CNormalizePage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_NORMALIZE, OnButtonNormalize)
	ON_BN_CLICKED(IDC_BUTTON_PEAK_OPPOSITE, OnButtonPeakOpposite)
	ON_WM_VSCROLL()
	ON_EN_CHANGE(IDC_EDIT_ANGLE, OnChangeEditAngle)
	ON_CBN_SELCHANGE(IDC_COMBO_GATE, OnSelchangeComboGate)
	ON_CBN_SELCHANGE(IDC_COMBO_TIMESLOT, OnSelchangeComboTimeslot)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_DISTANCE, OnChangeEditDistance)
	ON_CBN_SELCHANGE(IDC_COMBO_GATE2, OnSelchangeComboGate2)
	ON_BN_CLICKED(IDC_BUTTON_WATERPATH_LEFT, OnButtonWaterpathLeft)
	ON_BN_CLICKED(IDC_BUTTON_WATERPATH_RIGHT, OnButtonWaterpathRight)
	ON_BN_CLICKED(IDC_BUTTON_MOVETO_LEFT, OnButtonMovetoLeft)
	ON_BN_CLICKED(IDC_BUTTON_MOVETO_RIGHT, OnButtonMovetoRight)
	ON_CBN_SELCHANGE(IDC_COMBO_PEAK_SIDE, OnSelchangeComboPeakSide)
	ON_BN_CLICKED(IDC_CHECK_XTIP, OnCheckXtip)
	ON_BN_CLICKED(IDC_CHECK_YTIP, OnCheckYtip)
	ON_CBN_SELCHANGE(IDC_COMBO_PEAK_MODE, OnSelchangeComboPeakMode)
	ON_WM_HELPINFO()
	ON_CBN_SELCHANGE(IDC_COMBO_INSPECTION_MODE, OnSelchangeComboInspectionMode)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_NORM_SIDE, OnSelchangeComboNormSide)
	ON_CBN_SELCHANGE(IDC_COMBO_PEAK_TIMESLOT, OnSelchangeComboPeakTimeslot)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DISTANCE, OnDeltaposSpinDistance)
	ON_CBN_SELCHANGE(IDC_COMBO_LEFT_WP_TS, OnSelchangeComboLeftWpTs)
	ON_CBN_SELCHANGE(IDC_COMBO_RIGHT_WP_TS, OnSelchangeComboRightWpTs)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_NORMALIZE_GOTO, OnNormalizeGoto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNormalizePage message handlers

BOOL CNormalizePage::OnInitDialog() 
{
	CString Buff;
	CPropertyPage::OnInitDialog();

	LOGFONT	lf;
	CFont* pFont = GetFont();
	pFont->GetLogFont(&lf);



	m_spinAngle.SetRange(0,300);
	m_spinDistance.SetRange(0,300);

	m_comboPeakSide.AddString(theApp.m_DlgSideName[0]);
	m_comboPeakSide.AddString(theApp.m_DlgSideName[1]);
	m_comboNormSide.AddString(theApp.m_DlgSideName[0]);
	m_comboNormSide.AddString(theApp.m_DlgSideName[1]);

	Buff.LoadString(IDS_Peak);			m_comboPeakMode.AddString(Buff);
	Buff.LoadString(IDS_Peak_6dB);		m_comboPeakMode.AddString(Buff);
	Buff.LoadString(IDS_Minimum);		m_comboPeakMode.AddString(Buff);
	Buff.LoadString(IDS_Minimum_6dB);	m_comboPeakMode.AddString(Buff);

	Buff.LoadString(IDS_Criss_Cross);		m_comboPeakOrthogonalMode.AddString(Buff);
	Buff.LoadString(IDS_Rectilinear_scan);	m_comboPeakOrthogonalMode.AddString(Buff);

//	theApp.CharToCString(theApp.m_Axes[theApp.m_Tank.nXtLeft].cName, Buff);
//	m_checkXTip.SetWindowText(_T("i"));

//	theApp.CharToCString(theApp.m_Axes[theApp.m_Tank.nYtLeft].cName, Buff);
//	m_checkYTip.SetWindowText(_T("j"));



	UpdateAllControls();
	SetTimer(1,200,NULL);

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CNormalizePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CNormalizePage::UpdateAllControls()
{
	if(GetSafeHwnd() == NULL) return;
	CString Buff;

	if (theApp.m_nSide0Orientation == 0) {
		m_staticLeft.SetWindowText(theApp.m_DlgSideName[0]);
		m_staticRight.SetWindowText(theApp.m_DlgSideName[1]);
		Buff.Format(L"%s -> %s", theApp.m_DlgSideName[0], theApp.m_DlgSideName[1]);	m_buttonMove1To2.SetWindowText(Buff);
		Buff.Format(L"%s -> %s", theApp.m_DlgSideName[1], theApp.m_DlgSideName[0]);	m_buttonMove2To1.SetWindowText(Buff);
	}
	else {
		m_staticLeft.SetWindowText(theApp.m_DlgSideName[1]);
		m_staticRight.SetWindowText(theApp.m_DlgSideName[0]);
		Buff.Format(L"%s -> %s", theApp.m_DlgSideName[1], theApp.m_DlgSideName[0]);	m_buttonMove1To2.SetWindowText(Buff);
		Buff.Format(L"%s -> %s", theApp.m_DlgSideName[0], theApp.m_DlgSideName[1]);	m_buttonMove2To1.SetWindowText(Buff);
	}

	Buff.Format(_T("%.01f"),theApp.m_LastSettings.fNormalizeAngle);
	m_editAngle.SetWindowText(Buff);
	m_spinAngle.SetPos((int)(theApp.m_LastSettings.fNormalizeAngle * 10));

	Buff.Format(_T("%.*f%s"),theApp.m_nUnitsPrecision,theApp.m_LastSettings.fPeakDistance*theApp.m_fUnits,theApp.m_Units);
	m_editDistance.SetWindowText(Buff);


	m_comboNormTimeSlot.ResetContent();
	m_comboPeakTimeSlot.ResetContent();
	m_comboLeftWpTS.ResetContent();
	m_comboRightWpTS.ResetContent();
	for(int nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
		m_comboNormTimeSlot.AddString(theApp.m_UtUser.m_TS[nTS].cName);
		m_comboPeakTimeSlot.AddString(theApp.m_UtUser.m_TS[nTS].cName);
		m_comboLeftWpTS.AddString(theApp.m_UtUser.m_TS[nTS].cName);
		m_comboRightWpTS.AddString(theApp.m_UtUser.m_TS[nTS].cName);
	}
	m_comboNormTimeSlot.SetCurSel(theApp.m_LastSettings.nNormalizeTimeSlot[theApp.m_LastSettings.nNormSide]);
	m_comboPeakTimeSlot.SetCurSel(theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide]);
	if(theApp.m_nSide0Orientation==0) {
		m_comboLeftWpTS.SetCurSel(theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
		m_comboRightWpTS.SetCurSel(theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
	} else {
		m_comboLeftWpTS.SetCurSel(theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
		m_comboRightWpTS.SetCurSel(theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
	}

	m_comboGate.ResetContent();
	m_comboGatePeak.ResetContent();
	m_comboGate.LimitText(sizeof theApp.m_UtUser.m_TS[theApp.m_LastSettings.nNormalizeTimeSlot[theApp.m_LastSettings.nNormSide]].Gate.cName[0] - 1);
	for(int gg=0;gg<8;gg++) {
		m_comboGate.AddString(theApp.m_UtUser.m_TS[theApp.m_LastSettings.nNormalizeTimeSlot[theApp.m_LastSettings.nNormSide]].Gate.cName[gg]);
		m_comboGatePeak.AddString(theApp.m_UtUser.m_TS[theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide]].Gate.cName[gg]);
	}
	m_comboGate.SetCurSel(theApp.m_LastSettings.nNormalizeGate);
	m_comboGatePeak.SetCurSel(theApp.m_LastSettings.nPeakGate);
	m_comboPeakSide.SetCurSel(theApp.m_LastSettings.nPeakSide);
	m_comboNormSide.SetCurSel(theApp.m_LastSettings.nNormSide);

	m_checkXTip.SetCheck(theApp.m_LastSettings.nNormalizeXtip & 1);
	m_checkYTip.SetCheck(theApp.m_LastSettings.nNormalizeYtip & 1);

	m_comboPeakMode.SetCurSel(theApp.m_LastSettings.nPeakMode[0] & 1);
	m_comboPeakOrthogonalMode.SetCurSel(theApp.m_LastSettings.nPeakOrthogonalMode);


	SetAccessPrivelages();

}

void CNormalizePage::SetAccessPrivelages()
{
	if (GetSafeHwnd() == NULL) return;
	bool bFlagShow,bFlagEnable;
	CWnd* pWnd;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		bFlagShow = TRUE;
		break;
	case SINGLE_BRIDGE_TT_ROLLERS:
	case SINGLE_BRIDGE_TANK_WITH_R:
	case SINGLE_BRIDGE_TANK:
		bFlagShow = FALSE;
		break;
	}

	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDD_NORMALIZE_PAGE] ? bFlagEnable = TRUE : bFlagEnable = FALSE;
	
	ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_MOVETO_RIGHT),		bFlagShow,bFlagEnable);
	ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_MOVETO_LEFT),		bFlagShow,bFlagEnable);
	ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_WATERPATH_RIGHT),	bFlagShow,bFlagEnable);
	ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_PEAK_OPPOSITE),	bFlagShow,bFlagEnable);
	ShowAndEnableWindow(pWnd = GetDlgItem(IDC_STATIC_LEFT),				bFlagShow,bFlagEnable);
	ShowAndEnableWindow(pWnd = GetDlgItem(IDC_STATIC_RIGHT),			bFlagShow,bFlagEnable);

	ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_NORMALIZE),		TRUE,bFlagEnable);
	ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_PEAK_OPPOSITE),	TRUE,bFlagEnable);
	ShowAndEnableWindow(pWnd = GetDlgItem(IDC_BUTTON_WATERPATH_LEFT),	TRUE,bFlagEnable);
	ShowAndEnableWindow((CWnd*)(&m_comboNormSide),						TRUE,bFlagEnable);
	ShowAndEnableWindow((CWnd*)(&m_comboPeakSide),						TRUE,bFlagEnable);
	ShowAndEnableWindow((CWnd*)(&m_editDistance),						TRUE,bFlagEnable);
	ShowAndEnableWindow((CWnd*)(&m_spinDistance),						TRUE,bFlagEnable);
	ShowAndEnableWindow((CWnd*)(&m_comboGatePeak),						TRUE,bFlagEnable);
	ShowAndEnableWindow((CWnd*)(&m_comboGate),							TRUE,bFlagEnable);
	ShowAndEnableWindow((CWnd*)(&m_comboPeakSide),						TRUE,bFlagEnable);
	ShowAndEnableWindow((CWnd*)(&m_comboNormTimeSlot),					TRUE,bFlagEnable);
	ShowAndEnableWindow((CWnd*)(&m_comboPeakTimeSlot),					TRUE,bFlagEnable);
	ShowAndEnableWindow((CWnd*)(&m_comboPeakMode),						TRUE,bFlagEnable);
	ShowAndEnableWindow((CWnd*)(&m_comboPeakOrthogonalMode),			TRUE,bFlagEnable);
	ShowAndEnableWindow((CWnd*)(&m_editAngle),							TRUE,bFlagEnable);
	ShowAndEnableWindow((CWnd*)(&m_comboLeftWpTS),						TRUE,bFlagEnable);
	ShowAndEnableWindow((CWnd*)(&m_comboRightWpTS),						TRUE,bFlagEnable);
}

void CNormalizePage::ShowAndEnableWindow(CWnd* pWnd,bool bShowFlag,bool bEnableFlag)
{
	pWnd->ShowWindow(bShowFlag);
	if(bShowFlag == TRUE) pWnd->EnableWindow(bEnableFlag);
}

void CNormalizePage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CString	Buff;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_ANGLE:
		theApp.m_LastSettings.fNormalizeAngle=(float)(pSpin->GetPos()&0xffff)/10.0f;
		Buff.Format(_T("%.01f"),theApp.m_LastSettings.fNormalizeAngle);
		m_editAngle.SetWindowText(Buff);
		break;
	}

	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CNormalizePage::OnChangeEditAngle() 
{
	float fTemp;
	CString	Buff;

	m_editAngle.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	if(fTemp-theApp.m_LastSettings.fNormalizeAngle) {
		theApp.m_LastSettings.fNormalizeAngle=fTemp;
		m_spinAngle.SetPos((int)(fTemp*10.0));
	};
	
}

void CNormalizePage::OnSetFont(CFont* pFont) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::OnSetFont(pFont);
}

BOOL CNormalizePage::OnSetActive() 
{
	UpdateAllControls();
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	return CPropertyPage::OnSetActive();
}

void CNormalizePage::OnSelchangeComboGate() 
{
	theApp.m_LastSettings.nNormalizeGate = m_comboGate.GetCurSel();
}

void CNormalizePage::OnSelchangeComboTimeslot() 
{
	theApp.m_LastSettings.nNormalizeTimeSlot[theApp.m_LastSettings.nNormSide] = m_comboNormTimeSlot.GetCurSel();
	UpdateAllControls();
	
}

void CNormalizePage::TransferData(int nAxis, CPolyCoord *pLine,CCoord Cp)
{
	switch(nAxis) {
	case HORIZONTAL: m_CpHorizontal.Zero();
		m_CpHorizontal = pLine;
		m_CpHorizontalCentre = Cp;
		break;
	case VERTICAL: m_CpVertical.Zero();
		m_CpVertical = pLine;
		m_CpVerticalCentre = Cp;
		break;
	}
}

void CNormalizePage::TransferRectilinearData(CPolyCoord *pLine,int nNumberLines,int nLine)
{
	if(nLine<=0)
		return;

	nLine--;

	if((nNumberLines != m_RectilinearRect.Height()) || (pLine->m_nCoordL != m_RectilinearRect.Width()) || (m_pRectilinearArray == NULL)) {
		m_RectilinearRect.SetRect(0,0,pLine->m_nCoordL,nNumberLines);
		SAFE_DELETE(m_pRectilinearArray);
		m_pRectilinearArray = new char[m_RectilinearRect.Height() * m_RectilinearRect.Width()];
	}

	int nY = nNumberLines - nLine - 1;
	if(nY<0) nY = 0;
	if(nY>=nNumberLines) nY = nNumberLines-1;

	int nOffset = nLine * m_RectilinearRect.Width();
	for(int ii=0;ii<m_RectilinearRect.Width();ii++) {
		m_pRectilinearArray[nOffset+ii] = pLine->m_pCp[ii].nAmp;

		switch(theApp.m_LastSettings.nPeakMode[0]) {
		case 0:
		case 1:
			pLine->m_pCp[ii].nAmp = 0;
			break;
		case 2:
		case 3:
			pLine->m_pCp[ii].nAmp = 127;
			break;
		}
	}

}


void CNormalizePage::OnPaint() 
{
	if (GetSafeHwnd() == NULL) return;
	CPaintDC dc(this); // device context for painting

	if(m_nPaintMode == 0) {
		RenderCrissCross(&dc);
	} else {
		switch(theApp.m_LastSettings.nPeakOrthogonalMode) {
		case 0: RenderCrissCross(&dc);
			break;
		case 1: RenderRectilinear(&dc);
			break;
		}
	}
}


void CNormalizePage::RenderCrissCross(CPaintDC* pDC)
{
	CString m_Axis[] = {L"X Left",L"Y Left",L"Z Left",L"Xt Left",L"Zt Left",L"R Left",L"I Left",L"J Left",L"K Left",
		L"X Right",L"Y Right",L"Z Right",L"Xt Right",L"Zt Right",L"R Right",L"I Right",L"J Right",L"K Right"};

	
	CRect rr;
	CBrush Brush(RGB(255,255,255));
	CPen* pOldPen;
	CPen penRed(PS_SOLID,1,RGB(255,0,0));
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CPen penGrey(PS_SOLID,1,RGB(50,50,50));
	int	pnx,pny,ii,nMin,nMax;
	float f0,f1;
	CSize size;
	CString Buff;
	CBrush brushBK(GetSysColor(COLOR_BTNFACE));

	LOGFONT lf;
	memset((void*)&lf,0,sizeof(lf));
	lf.lfHeight = 10;
	CFont Font;
	CFont* pOldFont;
	Font.CreateFontIndirect(&lf);

	Buff = "Hello";
	size = pDC->GetTextExtent(Buff);

	pOldPen = pDC->SelectObject(&penRed);
	pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);

	//??????????
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.top=rr.bottom;
	rr.bottom=rr.top+3 + size.cy;
	rr.left-=size.cx;
	rr.right+=size.cx;
	pDC->FillRect(rr,&brushBK);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2);
	pDC->FillRect(&rr,&Brush);

	CRect rect;
	CRgn rgn;
	pDC->GetClipBox(&rect);
	rgn.CreateRectRgn(rr.left, rr.top, rr.right, rr.bottom);
	pDC->SelectClipRgn(&rgn);


	//Horizontal
	if(m_CpHorizontal.m_nCoordL>0) {
		pOldPen = pDC->SelectObject(&penRed);
		for(ii=0;ii<m_CpHorizontal.m_nCoordL;ii++) {
			pnx=MulDiv(ii,rr.Width(),m_CpHorizontal.m_nCoordL-1) + rr.left;
			pny=rr.bottom - MulDiv(m_CpHorizontal.m_pCp[ii].nAmp,rr.Height(),127);
			if(ii==0) {
				pDC->MoveTo(pnx,pny);
			} else {
				pDC->LineTo(pnx,pny);
			}
		}

		f0=m_CpHorizontal.m_pCp[0].fPos[m_CpHorizontal.m_nPrimaryAxis];
		f1=m_CpHorizontal.m_pCp[m_CpHorizontal.m_nCoordL-1].fPos[m_CpHorizontal.m_nPrimaryAxis];
		nMin=(int)f0;
		nMax=(int)f1;
/*
		pDC->SelectObject(penGrey);
		Buff=m_Axis[m_CpHorizontal.m_nPrimaryAxis];
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.CenterPoint().x-size.cx/2,rr.bottom-size.cy,Buff);

		fMin = (float)nMin;
		fMax = (float)nMax;
		for(fStep=fMin;fStep<=fMax;fStep+=5.0) {
			pnx = (int)((fStep-f0) * (float)rr.Width() / (f1-f0)) + rr.left;
			pDC->MoveTo(pnx,rr.bottom+2);
			pDC->LineTo(pnx,rr.bottom+4);
			Buff.Format(_T("%.1f"),fStep);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(pnx-size.cx/2,rr.bottom+3,Buff);
		}
		*/
		pDC->SelectObject(&penRed);
		pnx = (int)((m_CpHorizontalCentre.fPos[m_CpHorizontal.m_nPrimaryAxis] - f0) * (float)rr.Width() / (f1-f0)) + rr.left;
		pnx= rr.left + MulDiv(m_CpHorizontal.m_nMaxPtr,rr.Width(),m_CpHorizontal.m_nCoordL - 1);
		pDC->MoveTo(pnx,rr.top);
		pDC->LineTo(pnx,rr.bottom);
	}

		//Vertical
	if(m_CpVertical.m_nCoordL>0) {
		pOldPen = pDC->SelectObject(&penBlue);
		for(ii=0;ii<m_CpVertical.m_nCoordL;ii++) {
			pny=rr.bottom- MulDiv(ii,rr.Height(),m_CpVertical.m_nCoordL-1);
			pnx=rr.left + MulDiv(m_CpVertical.m_pCp[ii].nAmp,rr.Width(),127);
			if(ii==0) {
				pDC->MoveTo(pnx,pny);
			} else {
				pDC->LineTo(pnx,pny);
			}
		}

		f0=m_CpVertical.m_pCp[0].fPos[m_CpVertical.m_nPrimaryAxis];
		f1=m_CpVertical.m_pCp[m_CpVertical.m_nCoordL-1].fPos[m_CpVertical.m_nPrimaryAxis];
		nMin=(int)f0;
		nMax=(int)f1;
		/*
		pDC->SelectObject(penGrey);
		Buff=m_Axis[m_CpVertical.m_nPrimaryAxis];
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(rr.CenterPoint().x-size.cx/2,rr.bottom-size.cy,Buff);

		fMin = (float)nMin;
		fMax = (float)nMax;
		for(fStep=fMin;fStep<=fMax;fStep+=5.0) {
			pnx = (int)((fStep-f0) * (float)rr.Width() / (f1-f0)) + rr.left;
			pDC->MoveTo(pnx,rr.bottom+2);
			pDC->LineTo(pnx,rr.bottom+4);
			Buff.Format(_T("%.1f"),fStep);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(pnx-size.cx/2,rr.bottom+3,Buff);
		}
		*/

		pDC->SelectObject(&penBlue);
//		pny= rr.bottom - (int)((m_CpVerticalCentre.fPos[m_CpVertical.m_nPrimaryAxis] - f0) * (float)rr.Height() / (f1-f0));
		pny= rr.bottom - MulDiv(m_CpVertical.m_nMaxPtr,rr.Height(),m_CpVertical.m_nCoordL - 1);
		pDC->MoveTo(rr.left,pny);
		pDC->LineTo(rr.right,pny);
	}
	rgn.SetRectRgn(&rect);
	pDC->SelectClipRgn(&rgn);


	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);

}

void CNormalizePage::RenderRectilinear(CPaintDC* pDC)
{
	CRect rr;
	CDC  dcMem;
	CBitmap bmp;
	int	nOffset,nPtr,nColor,ii,nX,nY,pnX,pnY;
	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int	nBytesPixel = nBitsPixel/8;

	if(m_pRectilinearArray==NULL) return;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2);
	char *pArrayView = new char[(rr.Width()+2) * (rr.Height()+2) * nBytesPixel];
	char *pDest;

	char	cColor[256][4];
	for(ii=0;ii<256;ii++) {
		cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>16)&0xff);
		cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>8)&0xff);
		cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[0][ii]>>0)&0xff);
	}

	for(nY=0;nY<rr.Height();nY++) {
		if(m_nImageOrientation == 0) {
			nOffset = MulDiv(nY,m_RectilinearRect.Height()-1,rr.Height()-1) * m_RectilinearRect.Width();
		} else {
			nOffset = MulDiv(rr.Height()-nY-1,m_RectilinearRect.Height()-1,rr.Height()-1) * m_RectilinearRect.Width();
		}
		pDest = pArrayView + (nY * rr.Width() * nBytesPixel);
		for(nX=0;nX<rr.Width();nX++) {
			nPtr = MulDiv(nX,m_RectilinearRect.Width()-1,rr.Width()-1);
			nColor = m_pRectilinearArray[nOffset + nPtr] + 128;
			*pDest++=cColor[nColor][0];
			*pDest++=cColor[nColor][1];
			*pDest++=cColor[nColor][2];
			if(nBytesPixel>3) pDest++;
		}
	}



	bmp.CreateCompatibleBitmap(pDC,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*nBytesPixel),pArrayView);

	dcMem.CreateCompatibleDC(pDC);

	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	pDC->BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	delete pArrayView;

	FindRectilinearCentre(&nX,&nY);

	CPen penRed(PS_SOLID,1,RGB(255,0,0));
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CPen *pOldPen = pDC->SelectObject(&penRed);

	pnX = MulDiv(nX,rr.Width(),m_RectilinearRect.Width()) + rr.left;
	if(m_nImageOrientation==0) {
		pnY = MulDiv(nY,rr.Height(),m_RectilinearRect.Height()) + rr.top;
	} else {
		pnY = MulDiv(m_RectilinearRect.Height()-nY-1,rr.Height(),m_RectilinearRect.Height()) + rr.top;
	}
	pDC->MoveTo(rr.left,pnY);
	pDC->LineTo(rr.right,pnY);
	
	pDC->SelectObject(&penBlue);
	pDC->MoveTo(pnX,rr.top);
	pDC->LineTo(pnX,rr.bottom);

	pDC->SelectObject(pOldPen);

}

void CNormalizePage::OnChangeEditDistance() 
{
	float fTemp;
	CString	Buff;

	m_editDistance.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;

	if(theApp.m_LastSettings.fPeakDistance-fTemp) {
		theApp.m_LastSettings.fPeakDistance=fTemp;
	};
	
}

void CNormalizePage::OnDeltaposSpinDistance(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		theApp.m_LastSettings.fPeakDistance += ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		theApp.m_LastSettings.fPeakDistance += ((float)pNMUpDown->iDelta * 0.0254f);
	}
	UpdateAllControls();
	
	*pResult = 0;
}

void CNormalizePage::OnSelchangeComboGate2() 
{
	theApp.m_LastSettings.nPeakGate = m_comboGatePeak.GetCurSel();
}


void CNormalizePage::OnButtonNormalize() 
{
	if(theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) 
		theApp.m_LastSettings.nNormalizeTimeSlot[theApp.m_LastSettings.nNormSide] = CURRENT_FL;

	m_nPaintMode = 0;
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXtLeft;
	theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYtLeft;
	theApp.m_Thread.m_nNormAction = NORMALIZE;
	theApp.m_Thread.m_nNormSide = theApp.m_LastSettings.nNormSide;
	theApp.m_Thread.m_nThreadAction=NORMALIZE;
}


void CNormalizePage::OnButtonWaterpathLeft() 
{
	CCoord CpSurface,CpHead;
	float fWp;

	theApp.m_Thread.StopCurrentThreadAction();
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Motors.GetHeadPos(&CpHead);
		if(theApp.m_nSide0Orientation == 0) {
			if(theApp.m_LastSettings.nDesiredDevice==NOT_PHASED_ARRAY) {
				theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
			} else {
				theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,CURRENT_FL);
			}
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE,CURRENT_FL);
			theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE,NULL, CURRENT_FL);
		} else {
			if(theApp.m_LastSettings.nDesiredDevice==NOT_PHASED_ARRAY) {
				theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
			} else {
				theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,CURRENT_FL);
			}
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,FALSE, CURRENT_FL);
			theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE,NULL, CURRENT_FL);
		}
		theApp.m_Thread.m_CpMoveTo = CpHead;
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
	case TWIN_TOWER_KINEMATIC:
	case TANK_6AXIS:
		theApp.m_Motors.GetSurfacePos(&CpSurface);
		if(theApp.m_nSide0Orientation==0) {
			theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE],&fWp);
			fWp -= theApp.m_Kinematics.m_fDesiredWaterPath[PORTSIDE];				//Checked okay
			CpSurface.Side[PORTSIDE].fX -= CpSurface.Side[PORTSIDE].fI * fWp;
			CpSurface.Side[PORTSIDE].fY -= CpSurface.Side[PORTSIDE].fJ * fWp;
			CpSurface.Side[PORTSIDE].fZ -= CpSurface.Side[PORTSIDE].fK * fWp;
		} else {
			theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD],&fWp);
			fWp -= theApp.m_Kinematics.m_fDesiredWaterPath[STARBOARD];			//Not Checked
			CpSurface.Side[STARBOARD].fX -= CpSurface.Side[STARBOARD].fI * fWp;
			CpSurface.Side[STARBOARD].fY -= CpSurface.Side[STARBOARD].fJ * fWp;
			CpSurface.Side[STARBOARD].fZ -= CpSurface.Side[STARBOARD].fK * fWp;
		}

		if (theApp.m_Tank.nScannerDescription == TANK_5AXIS || theApp.m_Tank.nScannerDescription == QUICKSTEP_FLATBED) {
			theApp.m_Motors.IntelligentGotoSurfaceCoord(&CpSurface, 1, 0);
			return;
		}

		theApp.m_Thread.m_CpMoveTo = CpSurface;
		break;
	}
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
}

void CNormalizePage::OnButtonWaterpathRight() 
{
	CCoord CpSurface,CpHead;
	float fWp;

	theApp.m_Thread.StopCurrentThreadAction();
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Motors.GetHeadPos(&CpHead);
		if(theApp.m_nSide0Orientation == 0) {
			if(theApp.m_LastSettings.nDesiredDevice==NOT_PHASED_ARRAY) {
				theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
			} else {
				theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,CURRENT_FL);
			}
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,FALSE, CURRENT_FL);
			theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE,NULL, CURRENT_FL);
		} else {
			if(theApp.m_LastSettings.nDesiredDevice==NOT_PHASED_ARRAY) {
				theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
			} else {
				theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,CURRENT_FL);
			}
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE, CURRENT_FL);
			theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE, NULL, CURRENT_FL);
		}
		theApp.m_Thread.m_CpMoveTo = CpHead;
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TWIN_TOWER_KINEMATIC:
	case TANK_6AXIS:
		theApp.m_Motors.GetSurfacePos(&CpSurface);
		if(theApp.m_nSide0Orientation==0) {
			theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD],&fWp);
			fWp -= theApp.m_Kinematics.m_fDesiredWaterPath[STARBOARD];					//Checked Okay
			CpSurface.Side[STARBOARD].fX -= CpSurface.Side[STARBOARD].fI * fWp;
			CpSurface.Side[STARBOARD].fY -= CpSurface.Side[STARBOARD].fJ * fWp;
			CpSurface.Side[STARBOARD].fZ -= CpSurface.Side[STARBOARD].fK * fWp;
		} else {
			theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE],&fWp);
			fWp -= theApp.m_Kinematics.m_fDesiredWaterPath[PORTSIDE];						//Not checked
			CpSurface.Side[PORTSIDE].fX -= CpSurface.Side[PORTSIDE].fI * fWp;
			CpSurface.Side[PORTSIDE].fY -= CpSurface.Side[PORTSIDE].fJ * fWp;
			CpSurface.Side[PORTSIDE].fZ -= CpSurface.Side[PORTSIDE].fK * fWp;
		}

		theApp.m_Thread.m_CpMoveTo = CpSurface;
		break;
	}
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	
}

void CNormalizePage::OnButtonMovetoLeft() 
{
	CCoord CpHead,CpSurface;

	if(theApp.m_LastSettings.nDisplayAreYouSure!=0)
		if(MessageBox(_T("Are You Sure\nYou want to move the Right to the Left"),_T("Warning"),MB_YESNO) != IDYES) return;

	theApp.m_Thread.StopCurrentThreadAction();
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Motors.GetHeadPos(&CpHead);
		if(theApp.m_nSide0Orientation==0) {
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.CalculateOppositeCoord(STARBOARD,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
			theApp.m_Kinematics.CalculateCompensationPos(&CpHead);
		} else {
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
			theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
			theApp.m_Kinematics.CalculateOppositeCoord(PORTSIDE,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.CalculateCompensationPos(&CpHead);
		}
		CpHead.Side[1].fR=0.0f;
		theApp.m_Thread.m_CpMoveTo = CpHead;
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TWIN_TOWER_KINEMATIC:
	case TANK_6AXIS:
		theApp.m_Motors.GetSurfacePos(&CpSurface);
		if(theApp.m_nSide0Orientation==0) {
			theApp.m_Kinematics.CalculateOppositeCoord(STARBOARD,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
		} else {
			theApp.m_Kinematics.CalculateOppositeCoord(PORTSIDE,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
		}

		theApp.m_Thread.m_CpMoveTo = CpSurface;
		break;
	}

	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	
}

void CNormalizePage::OnButtonMovetoRight() 
{
	CCoord CpHead,CpSurface;

	if(theApp.m_LastSettings.nDisplayAreYouSure!=0)
		if(MessageBox(_T("Are You Sure\nYou want to move the Left to the Right"),_T("Warning"),MB_YESNO) != IDYES) return;

	theApp.m_Thread.StopCurrentThreadAction();
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Motors.GetHeadPos(&CpHead);
		if(theApp.m_nSide0Orientation==0) {
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
			theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
			theApp.m_Kinematics.CalculateOppositeCoord(PORTSIDE,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
		} else {
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.CalculateOppositeCoord(STARBOARD,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
		}
		theApp.m_Thread.m_CpMoveTo = CpHead;
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TWIN_TOWER_KINEMATIC:
	case TANK_6AXIS:
		theApp.m_Motors.GetSurfacePos(&CpSurface);
		if(theApp.m_nSide0Orientation==0) {
			theApp.m_Kinematics.CalculateOppositeCoord(PORTSIDE,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
		} else {
			theApp.m_Kinematics.CalculateOppositeCoord(STARBOARD,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
		}

		theApp.m_Thread.m_CpMoveTo = CpSurface;
		break;
	}
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
}

void CNormalizePage::OnButtonPeakOpposite() 
{
	if(theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) 
		theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide] = CURRENT_FL;

	m_nPaintMode = 1;
	theApp.m_Thread.StopCurrentThreadAction();
	switch(theApp.m_LastSettings.nPeakSide) {
	case 0:
		theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXLeft;
		theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYLeft;
		theApp.m_Thread.m_nNormAction = PEAK_OPPOSITE;
		theApp.m_Thread.m_nNormSide = theApp.m_nPrimarySide;
		break;
	case 1:
		theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXRight;
		theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYRight;
		theApp.m_Thread.m_nNormAction = PEAK_OPPOSITE;
		theApp.m_Thread.m_nNormSide = theApp.m_nSecondarySide;
		break;
	}
	switch(theApp.m_LastSettings.nPeakOrthogonalMode) {
	case 0:	theApp.m_Thread.m_nThreadAction=PEAK_OPPOSITE;
		break;
	case 1:
		theApp.m_Thread.m_pNormalizePage = this;
		theApp.m_Thread.m_pProbeBeamSectionsPage = NULL;
		theApp.m_Thread.m_nThreadAction=PEAK_OPPOSITE;
		break;
	}

}

void CNormalizePage::OnSelchangeComboPeakSide() 
{
	theApp.m_LastSettings.nPeakSide = m_comboPeakSide.GetCurSel();
	UpdateAllControls();
}

void CNormalizePage::OnCheckXtip() 
{
	theApp.m_LastSettings.nNormalizeXtip = m_checkXTip.GetCheck()&1;
}

void CNormalizePage::OnCheckYtip() 
{
	theApp.m_LastSettings.nNormalizeYtip = m_checkYTip.GetCheck()&1;
}

void CNormalizePage::OnSelchangeComboPeakMode() 
{
	theApp.m_LastSettings.nPeakMode[0] = m_comboPeakMode.GetCurSel();
	Invalidate(FALSE);
}


BOOL CNormalizePage::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	theApp.m_Help.Open(_T("MotionTools_Normalize.html"),NULL);
	return CPropertyPage::OnHelpInfo(pHelpInfo);
}

void CNormalizePage::OnSelchangeComboInspectionMode() 
{
	theApp.m_LastSettings.nPeakOrthogonalMode = m_comboPeakOrthogonalMode.GetCurSel()&1;
}

void CNormalizePage::FindRectilinearCentre(int *pnSample, int *pnLine)
{
	int nX,nY,nMax,nn,nThresh,nX0,nX1,nY0,nY1,nNumberGreaterThanThresh,nSum;
	int	nMin;

	*pnSample = m_RectilinearRect.CenterPoint().x;
	*pnLine = m_RectilinearRect.CenterPoint().y;
	switch(theApp.m_LastSettings.nPeakMode[0]) {
	case 0:
	case 1:
		nMax=0;
		for(nY=0,nn=0;nY<m_RectilinearRect.Height();nY++) {
			for(nX=0;nX<m_RectilinearRect.Width();nX++,nn++) {
				if(m_pRectilinearArray[nn]>nMax) {
					*pnSample = nX;
					*pnLine = nY;
					nMax = m_pRectilinearArray[nn];
				}
			}
		}
		break;
	case 2:
	case 3:
		nMin=32000;
		nMax = 0;
		for(nY=0,nn=0;nY<m_RectilinearRect.Height();nY++) {
			for(nX=0;nX<m_RectilinearRect.Width();nX++,nn++) {
				if(m_pRectilinearArray[nn]<nMin) {
					*pnSample = nX;
					*pnLine = nY;
					nMin = m_pRectilinearArray[nn];
				}
				if(m_pRectilinearArray[nn]>nMax) {
					nMax = m_pRectilinearArray[nn];
				}
			}
		}
		break;
	};

	
	switch(theApp.m_LastSettings.nPeakMode[0]) {
	case 0:
	case 2:
		break;
	case 1:						//Maximum -6dB
		nX0=nX1=*pnSample;
		nY0=nY1=*pnLine;
		nThresh = nMax / 2;
		nSum=nNumberGreaterThanThresh=m_RectilinearRect.Width();
		for(nY=*pnLine;(nY>=0) && (nSum>0);nY--) {
			nn=nY * m_RectilinearRect.Width();
			nSum=0;
			for(nX=0;nX<m_RectilinearRect.Width();nX++,nn++) {
				if(m_pRectilinearArray[nn] >= nThresh) nSum++;
			}
			if((nSum<=nNumberGreaterThanThresh) && (nSum>0)) {
				nNumberGreaterThanThresh = nSum;
				nY0=nY;
			}
		}
		nSum=nNumberGreaterThanThresh=m_RectilinearRect.Width();
		for(nY=*pnLine;(nY<m_RectilinearRect.Height()) && (nSum>0);nY++) {
			nn=nY * m_RectilinearRect.Width();
			nSum=0;
			for(nX=0;nX<m_RectilinearRect.Width();nX++,nn++) {
				if(m_pRectilinearArray[nn] >= nThresh) nSum++;
			}
			if((nSum<=nNumberGreaterThanThresh) && (nSum>0)) {
				nNumberGreaterThanThresh = nSum;
				nY1=nY;
			}
		}
		*pnLine = (nY1 + nY0) / 2;

		nSum=nNumberGreaterThanThresh=m_RectilinearRect.Height();
		for(nX=*pnSample;(nX>=0) && (nSum>0);nX--) {
			for(nY=0,nn=nX,nSum=0;nY<m_RectilinearRect.Height();nY++,nn+=m_RectilinearRect.Width()) {
				if(m_pRectilinearArray[nn] >= nThresh) nSum++;
			}
			if((nSum<=nNumberGreaterThanThresh) && (nSum>0)) {
				nNumberGreaterThanThresh = nSum;
				nX0=nX;
			}
		}
		nSum=nNumberGreaterThanThresh=m_RectilinearRect.Height();
		for(nX=*pnSample;(nX<m_RectilinearRect.Width()) && (nSum>0);nX++) {
			for(nY=0,nn=nX,nSum=0;nY<m_RectilinearRect.Height();nY++,nn+=m_RectilinearRect.Width()) {
				if(m_pRectilinearArray[nn] >= nThresh) nSum++;
			}
			if((nSum<=nNumberGreaterThanThresh) && (nSum>0)) {
				nNumberGreaterThanThresh = nSum;
				nX1=nX;
			}
		}
		*pnSample = (nX1 + nX0) / 2;
		break;
	case 3:						//Minimu -6dB
		nX0=nX1=*pnSample;
		nY0=nY1=*pnLine;
		nThresh = (nMax - nMin) / 2;
		nSum=nNumberGreaterThanThresh=m_RectilinearRect.Width();
		for(nY=*pnLine;(nY>=0) && (nSum>0);nY--) {
			nn=nY * m_RectilinearRect.Width();
			nSum=0;
			for(nX=0;nX<m_RectilinearRect.Width();nX++,nn++) {
				if(m_pRectilinearArray[nn] <= nThresh) nSum++;
			}
			if((nSum<=nNumberGreaterThanThresh) && (nSum>0)) {
				nNumberGreaterThanThresh = nSum;
				nY0=nY;
			}
		}
		nSum=nNumberGreaterThanThresh=m_RectilinearRect.Width();
		for(nY=*pnLine;(nY<m_RectilinearRect.Height()) && (nSum>0);nY++) {
			nn=nY * m_RectilinearRect.Width();
			nSum=0;
			for(nX=0;nX<m_RectilinearRect.Width();nX++,nn++) {
				if(m_pRectilinearArray[nn] <= nThresh) nSum++;
			}
			if((nSum<=nNumberGreaterThanThresh) && (nSum>0)) {
				nNumberGreaterThanThresh = nSum;
				nY1=nY;
			}
		}
		*pnLine = (nY1 + nY0) / 2;

		nSum=nNumberGreaterThanThresh=m_RectilinearRect.Height();
		for(nX=*pnSample;(nX>=0) && (nSum>0);nX--) {
			for(nY=0,nn=nX,nSum=0;nY<m_RectilinearRect.Height();nY++,nn+=m_RectilinearRect.Width()) {
				if(m_pRectilinearArray[nn] <= nThresh) nSum++;
			}
			if((nSum<=nNumberGreaterThanThresh) && (nSum>0)) {
				nNumberGreaterThanThresh = nSum;
				nX0=nX;
			}
		}
		nSum=nNumberGreaterThanThresh=m_RectilinearRect.Height();
		for(nX=*pnSample;(nX<m_RectilinearRect.Width()) && (nSum>0);nX++) {
			for(nY=0,nn=nX,nSum=0;nY<m_RectilinearRect.Height();nY++,nn+=m_RectilinearRect.Width()) {
				if(m_pRectilinearArray[nn] <= nThresh) nSum++;
			}
			if((nSum<=nNumberGreaterThanThresh) && (nSum>0)) {
				nNumberGreaterThanThresh = nSum;
				nX1=nX;
			}
		}
		*pnSample = (nX1 + nX0) / 2;

		break;
	}
}

void CNormalizePage::OnTimer(UINT nIDEvent) 
{
	CDC  dcMem;
	CRgn rgn;
	CRect rr,rect;
	CCoord	CpSurface;
	CPoint	point(-1,-1);
	CDC* pDC = GetDC();
	CPen pen(PS_SOLID,1,RGB(255,0,0));
	CPen* pOldPen=pDC->SelectObject(&pen);
	CBitmap *pOldBitmap;
	CBitmap	bmp,bmpCrossHair;


	if((m_nPaintMode == 1) && (theApp.m_LastSettings.nPeakOrthogonalMode == 1) ) {
	
		theApp.m_Motors.GetSurfacePos(&CpSurface);

		WorldToClient(CpSurface,theApp.m_LastSettings.nPeakSide,&point);
		m_staticView.GetWindowRect(&rr);
		ScreenToClient(&rr);
		if(m_nImageOrientation==0) {
			point.x-=8;
			point.y-=8;
		} else {
			point.x-=8;
			point.y-=16;
		}

		MinMax(&point.x,-8,rr.right);
		MinMax(&point.y,-8,rr.bottom);
		int m_nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
		int nBytesPixel = m_nBitsPixel/8;

		bmp.CreateCompatibleBitmap(pDC,16,16);
		bmp.SetBitmapBits((DWORD)(256*nBytesPixel),m_colorrefCrossHair);
		dcMem.CreateCompatibleDC(pDC);

		if(m_pointCrossHair!=point){
			pDC->GetClipBox(&rect);
			rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
			pDC->SelectClipRgn(&rgn);
			if(m_bCrossVisible==TRUE) {
				pOldBitmap = dcMem.SelectObject(&bmp);
				pDC->BitBlt(m_pointCrossHair.x,m_pointCrossHair.y,16,16,&dcMem,0,0,SRCCOPY);
				dcMem.SelectObject(pOldBitmap);
				m_bCrossVisible=FALSE;
			}
			bmpCrossHair.CreateCompatibleBitmap(pDC,16,16);
			pOldBitmap = dcMem.SelectObject(&bmpCrossHair);
			dcMem.BitBlt(0,0,16,16,pDC,point.x,point.y,SRCCOPY);
			dcMem.SelectObject(pOldBitmap);
			bmpCrossHair.GetBitmapBits(256*nBytesPixel,m_colorrefCrossHair);

			pDC->MoveTo(point.x+7,point.y);
			pDC->LineTo(point.x+7,point.y+6);
			pDC->MoveTo(point.x+8,point.y);
			pDC->LineTo(point.x+8,point.y+8);
			pDC->MoveTo(point.x+9,point.y);
			pDC->LineTo(point.x+9,point.y+6);

			pDC->MoveTo(point.x+7,point.y+15);
			pDC->LineTo(point.x+7,point.y+10);
			pDC->MoveTo(point.x+8,point.y+15);
			pDC->LineTo(point.x+8,point.y+8);
			pDC->MoveTo(point.x+9,point.y+15);
			pDC->LineTo(point.x+9,point.y+10);

			pDC->MoveTo(point.x,point.y+7);
			pDC->LineTo(point.x+6,point.y+7);
			pDC->MoveTo(point.x,point.y+8);
			pDC->LineTo(point.x+8,point.y+8);
			pDC->MoveTo(point.x,point.y+9);
			pDC->LineTo(point.x+6,point.y+9);

			pDC->MoveTo(point.x+15,point.y+7);
			pDC->LineTo(point.x+10,point.y+7);
			pDC->MoveTo(point.x+15,point.y+8);
			pDC->LineTo(point.x+8,point.y+8);
			pDC->MoveTo(point.x+15,point.y+9);
			pDC->LineTo(point.x+10,point.y+9);

			m_pointCrossHair=point;
			m_bCrossVisible=TRUE;
			rgn.SetRectRgn(&rect);
			pDC->SelectClipRgn(&rgn);

		};
	}


	pDC->SelectObject(pOldPen);

	ReleaseDC(pDC);
	
	CPropertyPage::OnTimer(nIDEvent);
}

void CNormalizePage::WorldToClient(CCoord Cp, int nSide, CPoint *pt)
{
	D3DXVECTOR3 vecWidth, vecHeight;
	CRect rr;
	CCoord m_CpRect[4];
	CCoord Cp0,Cp1,CpPt;
	float fX,fY;

	CopyMemory(m_CpRect,theApp.m_Thread.m_CpRect,sizeof m_CpRect);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	int nLine,nSample;
	float fMinLength = 1000000.0f;
	float fLength;
	int nL=0;
	int nS=0;
	//This bit is very neieve it works but there must be a better way;

	for(nLine=0,fY=0.0f;nLine<100;nLine++, fY+=0.01f) {
		for(nSample=0,fX=0.0f;nSample<100;nSample++, fX += 0.01f) {

			Cp0.Side0.fX = (fX * (m_CpRect[1].Side0.fX - m_CpRect[0].Side0.fX)) + m_CpRect[0].Side0.fX;
			Cp0.Side0.fY = (fX * (m_CpRect[1].Side0.fY - m_CpRect[0].Side0.fY)) + m_CpRect[0].Side0.fY;
			Cp0.Side0.fZ = (fX * (m_CpRect[1].Side0.fZ - m_CpRect[0].Side0.fZ)) + m_CpRect[0].Side0.fZ;

			Cp1.Side0.fX = (fX * (m_CpRect[3].Side0.fX - m_CpRect[2].Side0.fX)) + m_CpRect[2].Side0.fX;
			Cp1.Side0.fY = (fX * (m_CpRect[3].Side0.fY - m_CpRect[2].Side0.fY)) + m_CpRect[2].Side0.fY;
			Cp1.Side0.fZ = (fX * (m_CpRect[3].Side0.fZ - m_CpRect[2].Side0.fZ)) + m_CpRect[2].Side0.fZ;

			CpPt.Side0.fX = (fY * (Cp1.Side0.fX - Cp0.Side0.fX)) + Cp0.Side0.fX;
			CpPt.Side0.fY = (fY * (Cp1.Side0.fY - Cp0.Side0.fY)) + Cp0.Side0.fY;
			CpPt.Side0.fZ = (fY * (Cp1.Side0.fZ - Cp0.Side0.fZ)) + Cp0.Side0.fZ;

			fLength = CCoord::Length(CpPt,Cp);

			if(fLength < fMinLength) {
				fMinLength = fLength;
				nL = nLine;
				nS = nSample;
			}
		}
	}

	pt->x = MulDiv(nS,rr.Width(),99) + rr.left;
	pt->y = rr.bottom - MulDiv(nL,rr.Height(),99);

}

int CNormalizePage::MinMax(int *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}

long CNormalizePage::MinMax(long *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}

void CNormalizePage::OnSelchangeComboNormSide() 
{
	theApp.m_LastSettings.nNormSide = m_comboNormSide.GetCurSel();
	UpdateAllControls();
	
}

void CNormalizePage::OnSelchangeComboPeakTimeslot() 
{
	theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide] = m_comboPeakTimeSlot.GetCurSel();
}


void CNormalizePage::OnSelchangeComboLeftWpTs() 
{
	if(theApp.m_nSide0Orientation==0) {
		theApp.m_LastSettings.nWaterPathTS[PORTSIDE] = m_comboLeftWpTS.GetCurSel();
	} else {
		theApp.m_LastSettings.nWaterPathTS[STARBOARD] = m_comboLeftWpTS.GetCurSel();
	}
	
}

void CNormalizePage::OnSelchangeComboRightWpTs() 
{
	if(theApp.m_nSide0Orientation==0) {
		theApp.m_LastSettings.nWaterPathTS[STARBOARD] = m_comboRightWpTS.GetCurSel();
	} else {
		theApp.m_LastSettings.nWaterPathTS[PORTSIDE] = m_comboRightWpTS.GetCurSel();
	}
}

void CNormalizePage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CRect rr;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup = NULL;

	m_ptClient = point;
	ScreenToClient(&m_ptClient);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(1,1);
	
	if(theApp.m_LastSettings.nPeakOrthogonalMode == 1) {

		if(rr.PtInRect(m_ptClient)) {

			menu->LoadMenu(IDR_RB_NORMALIZE_MENU);
			CMenu* pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);

		}

	}

	delete menu;
}

void CNormalizePage::OnNormalizeGoto() 
{
	CRect rr;
	CCoord CpHead,CpSurface,Cp0,Cp1;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(1,1);

	float fX = (float)(m_ptClient.x - rr.left) / (float)rr.Width();
	float fY = (float)(rr.bottom - m_ptClient.y) / (float)rr.Width();

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.GetSurfacePos(&CpSurface);

	if(theApp.m_LastSettings.nPeakSide == PORTSIDE) {
		Cp0.Side0.fX = (fX * (theApp.m_Thread.m_CpRect[1].Side0.fX - theApp.m_Thread.m_CpRect[0].Side0.fX)) + theApp.m_Thread.m_CpRect[0].Side0.fX;
		Cp0.Side0.fY = (fX * (theApp.m_Thread.m_CpRect[1].Side0.fY - theApp.m_Thread.m_CpRect[0].Side0.fY)) + theApp.m_Thread.m_CpRect[0].Side0.fY;
		Cp0.Side0.fZ = (fX * (theApp.m_Thread.m_CpRect[1].Side0.fZ - theApp.m_Thread.m_CpRect[0].Side0.fZ)) + theApp.m_Thread.m_CpRect[0].Side0.fZ;

		Cp1.Side0.fX = (fX * (theApp.m_Thread.m_CpRect[3].Side0.fX - theApp.m_Thread.m_CpRect[2].Side0.fX)) + theApp.m_Thread.m_CpRect[2].Side0.fX;
		Cp1.Side0.fY = (fX * (theApp.m_Thread.m_CpRect[3].Side0.fY - theApp.m_Thread.m_CpRect[2].Side0.fY)) + theApp.m_Thread.m_CpRect[2].Side0.fY;
		Cp1.Side0.fZ = (fX * (theApp.m_Thread.m_CpRect[3].Side0.fZ - theApp.m_Thread.m_CpRect[2].Side0.fZ)) + theApp.m_Thread.m_CpRect[2].Side0.fZ;

		CpSurface.Side0.fX = (fY * (Cp1.Side0.fX - Cp0.Side0.fX)) + Cp0.Side0.fX;
		CpSurface.Side0.fY = (fY * (Cp1.Side0.fY - Cp0.Side0.fY)) + Cp0.Side0.fY;
		CpSurface.Side0.fZ = (fY * (Cp1.Side0.fZ - Cp0.Side0.fZ)) + Cp0.Side0.fZ;
	} else {
		MessageBox(_T("Not implemented yet"),_T("Message"),MB_OK);
	}
	theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
	theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);

	theApp.m_Thread.m_CpMoveTo = CpHead;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;

}
