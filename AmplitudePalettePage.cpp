// AmplitudePalettePage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "AmplitudePalettePage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAmplitudePalettePage property page

IMPLEMENT_DYNCREATE(CAmplitudePalettePage, CResizablePage)

CAmplitudePalettePage::CAmplitudePalettePage(int nWhichAmp) : CResizablePage(CAmplitudePalettePage::IDD)
{
	//{{AFX_DATA_INIT(CAmplitudePalettePage)
	//}}AFX_DATA_INIT

	m_nWhichAmp=MinMax(&nWhichAmp, 0, 7);
	m_nSlideEntry=-1;
	m_bMoveSlider=FALSE;
	m_pFont=NULL;
}

CAmplitudePalettePage::~CAmplitudePalettePage()
{
	SAFE_DELETE( m_pFont );
}

void CAmplitudePalettePage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAmplitudePalettePage)
	DDX_Control(pDX, IDC_STATIC_THRESHOLDS, m_staticThresholds);
	DDX_Control(pDX, IDC_EDIT_TOP_THRESHOLD, m_editTopThreshold);
	DDX_Control(pDX, IDC_SPIN_TOP_THRESHOLD, m_spinTopThreshold);
	DDX_Control(pDX, IDC_COMBO_THRESHOLD_MODE, m_comboThresholdMode);
	DDX_Control(pDX, IDC_EDIT_THRESHOLD_SPAN, m_editThresholdSpan);
	DDX_Control(pDX, IDC_SPIN_THRESHOLD_SPAN, m_spinThresholdSpan);
	DDX_Control(pDX, IDC_STATIC_BOTTOM_COLOR, m_staticBottomColour);
	DDX_Control(pDX, IDC_STATIC_TOP_COLOR, m_staticTopColour);
	DDX_Control(pDX, IDC_SLIDER_THRESHOLD, m_sliderThreshold);
	DDX_Control(pDX, IDC_BUTTON_MAKE_IT_SO, m_buttonMakeItSo);
	DDX_Control(pDX, IDC_STATIC_8, m_static8);
	DDX_Control(pDX, IDC_STATIC_7, m_static7);
	DDX_Control(pDX, IDC_STATIC_6, m_static6);
	DDX_Control(pDX, IDC_STATIC_5, m_static5);
	DDX_Control(pDX, IDC_STATIC_4, m_static4);
	DDX_Control(pDX, IDC_STATIC_3, m_static3);
	DDX_Control(pDX, IDC_STATIC_2, m_static2);
	DDX_Control(pDX, IDC_STATIC_1, m_static1);
	DDX_Control(pDX, IDC_STATIC_0, m_static0);
	DDX_Control(pDX, IDC_COMBO_STEP_MODE, m_comboStepMode);
	DDX_Control(pDX, IDC_STATIC_SLIDER, m_staticSlider);
	DDX_Control(pDX, IDC_COMBO_GRAPH_TYPE, m_comboGraphType);
	DDX_Control(pDX, IDC_SPIN_NUMBER_OF_ENTRIES, m_spinNumberOfEntries);
	DDX_Control(pDX, IDC_EDIT_NUMBER_OF_ENTRIES, m_editNumberOfEntries);
	DDX_Control(pDX, IDC_COMBO_COPY_TO, m_comboCopyTo);
	DDX_Control(pDX, IDC_COMBO_WHICH_GATE, m_comboWhichGate);
	DDX_Control(pDX, IDC_SPIN_TOP_LEVEL, m_spinTopLevel);
	DDX_Control(pDX, IDC_EDIT_TOP_LEVEL, m_editTopLevel);
	DDX_Control(pDX, IDC_COMBO_SCALE_TYPE, m_comboScaleType);
	DDX_Control(pDX, IDC_STATIC_ENTRIES, m_staticEntries);
	DDX_Control(pDX, IDC_STATIC_BAR_GRAPH, m_staticBarGraph);
	DDX_Control(pDX, IDC_SPIN_STEPSIZE, m_spinStepSize);
	DDX_Control(pDX, IDC_EDIT_STEPSIZE, m_editStepSize);
	DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAmplitudePalettePage, CResizablePage)
	//{{AFX_MSG_MAP(CAmplitudePalettePage)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, OnSelchangeComboMode)
	ON_EN_CHANGE(IDC_EDIT_STEPSIZE, OnChangeEditStepsize)
	ON_CBN_SELCHANGE(IDC_COMBO_SCALE_TYPE, OnSelchangeComboScaleType)
	ON_EN_CHANGE(IDC_EDIT_TOP_LEVEL, OnChangeEditTopLevel)
	ON_CBN_SELCHANGE(IDC_COMBO_WHICH_GATE, OnSelchangeComboWhichGate)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_IT_SO, OnButtonMakeItSo)
	ON_EN_CHANGE(IDC_EDIT_NUMBER_OF_ENTRIES, OnChangeEditNumberOfEntries)
	ON_CBN_SELCHANGE(IDC_COMBO_GRAPH_TYPE, OnSelchangeComboGraphType)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_CBN_SELCHANGE(IDC_COMBO_STEP_MODE, OnSelchangeComboStepMode)
	ON_WM_HELPINFO()
	ON_EN_CHANGE(IDC_EDIT_THRESHOLD_SPAN, OnChangeEditThresholdSpan)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_THRESHOLD_SPAN, OnDeltaposSpinThresholdSpan)
	ON_CBN_SELCHANGE(IDC_COMBO_THRESHOLD_MODE, OnSelchangeComboThresholdMode)
	ON_EN_CHANGE(IDC_EDIT_TOP_THRESHOLD, OnChangeEditTopThreshold)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_TOP_THRESHOLD, OnDeltaposSpinTopThreshold)
	//}}AFX_MSG_MAP
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_STEPSIZE, &CAmplitudePalettePage::OnDeltaposSpinStepsize)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAmplitudePalettePage message handlers
BOOL CAmplitudePalettePage::OnInitDialog() 
{
	CString	Buff,Language;
	CResizablePage::OnInitDialog();

	GetClientRect(m_rrClient);

	AddAnchor(&m_comboStepMode);
	AddAnchor(&m_staticSlider);
	AddAnchor(&m_comboGraphType);
	AddAnchor(&m_spinNumberOfEntries);
	AddAnchor(&m_editNumberOfEntries);
	AddAnchor(&m_comboCopyTo);
	AddAnchor(&m_comboWhichGate);
	AddAnchor(&m_spinTopLevel);
	AddAnchor(&m_editTopLevel);
	AddAnchor(&m_comboScaleType);
	AddAnchor(&m_staticEntries);
	AddAnchor(&m_staticBarGraph);
	AddAnchor(&m_spinStepSize);
	AddAnchor(&m_editStepSize);
	AddAnchor(&m_comboMode);
	AddAnchor(&m_static0);
	AddAnchor(&m_static1);
	AddAnchor(&m_static2);
	AddAnchor(&m_static3);
	AddAnchor(&m_static4);
	AddAnchor(&m_static5);
	AddAnchor(&m_static6);
	AddAnchor(&m_static7);
	AddAnchor(&m_static8);
	AddAnchor(&m_buttonMakeItSo);
	AddAnchor(&m_staticBottomColour);
	AddAnchor(&m_staticTopColour);
	AddAnchor(&m_sliderThreshold);
	AddAnchor(&m_editThresholdSpan);
	AddAnchor(&m_spinThresholdSpan);
	AddAnchor(&m_comboThresholdMode);
	AddAnchor(&m_editTopThreshold);
	AddAnchor(&m_spinTopThreshold);
	AddAnchor(&m_staticThresholds);

	Language.LoadString(IDS_100_FSH); m_comboScaleType.AddString(Language);
	Language.LoadString(IDS_dB_FSH); m_comboScaleType.AddString(Language);
	Language.LoadString(IDS_100dB_Log); m_comboScaleType.AddString(Language);


	m_spinTopLevel.SetRange(0,100);
	m_spinStepSize.SetRange(0,1000);
	m_spinNumberOfEntries.SetRange(1,100);

	Language.LoadString(IDS_Blended); m_comboMode.AddString(Language);
	Language.LoadString(IDS_Block); m_comboMode.AddString(Language);

	m_comboWhichGate.AddString(_T("1"));
	m_comboWhichGate.AddString(_T("2"));
	m_comboWhichGate.AddString(_T("3"));
	m_comboWhichGate.AddString(_T("4"));
	m_comboWhichGate.AddString(_T("5"));
	m_comboWhichGate.AddString(_T("6"));
	m_comboWhichGate.AddString(_T("7"));
	m_comboWhichGate.AddString(_T("8"));

	Language.LoadString(IDS_All); m_comboCopyTo.AddString(Language);
	m_comboCopyTo.AddString(_T("1"));
	m_comboCopyTo.AddString(_T("2"));
	m_comboCopyTo.AddString(_T("3"));
	m_comboCopyTo.AddString(_T("4"));
	m_comboCopyTo.AddString(_T("5"));
	m_comboCopyTo.AddString(_T("6"));
	m_comboCopyTo.AddString(_T("7"));
	m_comboCopyTo.AddString(_T("8"));

	Language.LoadString(IDS_Total); m_comboGraphType.AddString(Language);
	Language.LoadString(IDS_Window); m_comboGraphType.AddString(Language);

	Language.LoadString(IDS_Step);		m_comboStepMode.AddString(Language);
	Language.LoadString(IDS_Variable);	m_comboStepMode.AddString(Language);
	Language.LoadString(IDS_Top);		m_comboStepMode.AddString(Language);

	m_sliderThreshold.SetRange(0,1000);
	m_spinThresholdSpan.SetRange(0,1000);

	Buff.LoadString(IDS_Off);		m_comboThresholdMode.AddString(Buff);
	Buff.LoadString(IDS_Overlay);	m_comboThresholdMode.AddString(Buff);
	Buff.LoadString(IDS_Squash);	m_comboThresholdMode.AddString(Buff);

	m_spinTopThreshold.SetRange(0,1000);


	CFont* pFont = GetFont();
	pFont->GetLogFont(&m_LogFont);

	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&m_LogFont);
	

	UpdateAllControls();
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAmplitudePalettePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CAmplitudePalettePage::AddAnchor(CWnd *pCtrl)
{
	CRect rr;

	pCtrl->GetWindowRect(&rr);
	ScreenToClient(&rr);

	int nID = pCtrl->GetDlgCtrlID();
	CResizablePage::AddAnchor(nID, CSize(MulDiv(rr.left,100,m_rrClient.Width()),MulDiv(rr.top,100,m_rrClient.Height())), CSize(MulDiv(rr.right,100,m_rrClient.Width()),MulDiv(rr.bottom,100,m_rrClient.Height())));	
}

void CAmplitudePalettePage::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	CRect rr,r;
	LOGFONT	lf;
	CString Buff;

	if(GetSafeHwnd() && m_comboStepMode.GetSafeHwnd()) {
		
		GetClientRect(rr);
		lf = m_LogFont;
		lf.lfHeight = MulDiv(rr.Height(),m_LogFont.lfHeight,m_rrClient.Height());
		SAFE_DELETE( m_pFont );
		m_pFont = new CFont();
		m_pFont->CreateFontIndirect(&lf);
		m_comboStepMode.SetFont(m_pFont);
		m_staticSlider.SetFont(m_pFont);
		m_comboGraphType.SetFont(m_pFont);
		m_spinNumberOfEntries.SetFont(m_pFont);
		m_editNumberOfEntries.SetFont(m_pFont);
		m_comboCopyTo.SetFont(m_pFont);
		m_comboWhichGate.SetFont(m_pFont);
		m_spinTopLevel.SetFont(m_pFont);
		m_editTopLevel.SetFont(m_pFont);
		m_comboScaleType.SetFont(m_pFont);
		m_staticEntries.SetFont(m_pFont);
		m_staticBarGraph.SetFont(m_pFont);
		m_spinStepSize.SetFont(m_pFont);
		m_editStepSize.SetFont(m_pFont);
		m_comboMode.SetFont(m_pFont);
		m_static0.SetFont(m_pFont);
		m_static1.SetFont(m_pFont);
		m_static2.SetFont(m_pFont);
		m_static3.SetFont(m_pFont);
		m_static4.SetFont(m_pFont);
		m_static5.SetFont(m_pFont);
		m_static6.SetFont(m_pFont);
		m_static7.SetFont(m_pFont);
		m_static8.SetFont(m_pFont);
		m_buttonMakeItSo.SetFont(m_pFont);
		m_editThresholdSpan.SetFont(m_pFont);
		m_spinThresholdSpan.SetFont(m_pFont);
		m_comboThresholdMode.SetFont(m_pFont);
		m_editTopThreshold.SetFont(m_pFont);
		m_spinTopThreshold.SetFont(m_pFont);
		m_staticThresholds.SetFont(m_pFont);


		Invalidate(FALSE);
		UpdateAllControls();
	}

}

void CAmplitudePalettePage::UpdateAllControls()
{
	CString	Buff;
	float fdBLog = (float)theApp.m_LastSettings.nLogRange;

	if(GetSafeHwnd()==NULL) return;

	switch(theApp.m_Palette.m_nAmplitudeScaleType[m_nWhichAmp]) {
	case 0:
		Buff.Format(_T("%.01f %%"),theApp.m_Palette.m_ddBStepSize[m_nWhichAmp]);
		m_editStepSize.SetWindowText(Buff);
		Buff.Format(_T("%.01f %%"),theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]);
		m_editTopLevel.SetWindowText(Buff);
		break;
	case 1:
		Buff.Format(_T("%.01fdB"),theApp.m_Palette.m_ddBStepSize[m_nWhichAmp]);
		m_editStepSize.SetWindowText(Buff);
		Buff.Format(_T("%.01fdB"),theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]/-2.0);
		m_editTopLevel.SetWindowText(Buff);
		break;
	case 2:
		Buff.Format(_T("%.01fdB"),theApp.m_Palette.m_ddBStepSize[m_nWhichAmp]);
		m_editStepSize.SetWindowText(Buff);
		Buff.Format(_T("%.01fdB"),(float)theApp.m_LastSettings.nLogRange-theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]);
		m_editTopLevel.SetWindowText(Buff);
		break;
	}
	m_comboScaleType.SetCurSel(theApp.m_Palette.m_nAmplitudeScaleType[m_nWhichAmp]);
	m_comboMode.SetCurSel(theApp.m_Palette.m_nModeAmplitude[m_nWhichAmp]);
	m_comboGraphType.SetCurSel(theApp.m_Palette.m_nGraphType[m_nWhichAmp]);

	m_spinTopLevel.SetPos((int)(theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]));

	m_spinStepSize.SetPos((int)(theApp.m_Palette.m_ddBStepSize[m_nWhichAmp]*10));

	Buff.Format(_T("%d"),theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp]);
	m_editNumberOfEntries.SetWindowText(Buff);
	m_spinNumberOfEntries.SetPos(theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp]);


	m_comboWhichGate.SetCurSel(m_nWhichAmp);
	m_comboCopyTo.SetCurSel(0);
	m_comboStepMode.SetCurSel(theApp.m_Palette.m_nStepMode[m_nWhichAmp]);

	Buff.Format(L"%.01fdB",theApp.m_LastSettings.fThresholdSpan);
	m_editThresholdSpan.SetWindowText(Buff);

	m_spinThresholdSpan.SetPos((int)(theApp.m_LastSettings.fThresholdSpan * 10.0f));

	m_sliderThreshold.SetPos((int)(theApp.m_LastSettings.fThresholdTop * 10.0f));

	m_comboThresholdMode.SetCurSel(theApp.m_LastSettings.nThresholdMode);

	SetTopThresholdText();

	m_spinTopThreshold.SetPos((int)(theApp.m_LastSettings.fThresholdTop * 10.0f));

	SetAccessPrivelage();

}

void CAmplitudePalettePage::OnDeltaposSpinStepsize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	theApp.m_Palette.m_ddBStepSize[m_nWhichAmp] += (double)pNMUpDown->iDelta * 0.1;
	MinMax(&theApp.m_Palette.m_ddBStepSize[m_nWhichAmp], 0.1, 20.0);
	UpdateAllControls();

	*pResult = 0;
}



void CAmplitudePalettePage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int nTemp;
	float fTemp;

	switch(pSpin->GetDlgCtrlID()) {
//	case IDC_SPIN_STEPSIZE:
//		theApp.m_Palette.m_ddBStepSize[m_nWhichAmp]=(double)(pSpin->GetPos()&0xffff)/10.0;
//		UpdateAllControls();
//		break;
	case IDC_SPIN_TOP_LEVEL:
		theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]=(double)(pSpin->GetPos()&0xffff);
		UpdateAllControls();
		break;
	case IDC_SPIN_NUMBER_OF_ENTRIES:
		nTemp = theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp];
		theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp]=pSpin->GetPos()&0xffff;
		if(nTemp - theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp]) {
			Buff.Format(_T("%d"),theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp]);
			m_editNumberOfEntries.SetWindowText(Buff);
			Invalidate(FALSE);
		}
		break;
	case IDC_SLIDER_THRESHOLD:
		fTemp = theApp.m_LastSettings.fThresholdTop;
		theApp.m_LastSettings.fThresholdTop = (float)(pSlider->GetPos()&0xffff) / 10.0f;
		if(fTemp - theApp.m_LastSettings.fThresholdTop) {
			m_spinTopThreshold.SetPos((int)(theApp.m_LastSettings.fThresholdTop * 10.0f));
			SetTopThresholdText();
		}
		break;

	};		
	theApp.m_Palette.CalculatePalette();
	Invalidate(FALSE);
	UpdateRelatedDialogs();
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CAmplitudePalettePage::OnPaint() 
{
	CRect rr;
	CPaintDC dc(this); // device context for painting
	
	RefreshEntries(&dc);

	m_staticSlider.GetWindowRect(rr);
	ScreenToClient(rr);
	RefreshSlider(&dc,rr);

	m_staticBarGraph.GetWindowRect(rr);
	ScreenToClient(rr);
	RefreshBarGraph(&dc,rr);

	BlockFill(&dc,&m_staticTopColour,theApp.m_LastSettings.rgbThresholdTop);
	BlockFill(&dc,&m_staticBottomColour,theApp.m_LastSettings.rgbThresholdBottom);

}

void CAmplitudePalettePage::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);

}

void CAmplitudePalettePage::RefreshEntries(CPaintDC *pDC)
{
	int	ii,yy,y1;
	CRect rr,rectBox;
	CBrush* pOldBrush;
	CBrush* pBrush;
	
	if(theApp.m_Palette.m_ddBStepSize[m_nWhichAmp] <= 0.1) theApp.m_Palette.m_ddBStepSize[m_nWhichAmp] = 1.0;

	int nEntries = theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp];

	m_staticEntries.GetWindowRect(rr);
	ScreenToClient(rr);

	for(ii=0;ii<theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp];ii++) {
		yy=MulDiv(ii,rr.Height(),theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp]) + rr.top;
		y1=MulDiv(ii+1,rr.Height(),theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp]) + rr.top;
		pBrush = new CBrush;
		if(theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].bSet==TRUE) {
			pBrush->CreateSolidBrush(theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].rgbValue);
		} else {
			pBrush->CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
		};
		pOldBrush=pDC->SelectObject(pBrush);

		rectBox.SetRect(rr.left,yy,rr.right,y1);
		pDC->Rectangle(rectBox);

		pDC->SelectObject(pOldBrush);
		delete pBrush;
	}
}

void CAmplitudePalettePage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CMFCColorDialog dlg;
	int	ii,pnY;
	CRect rr;

	m_staticEntries.GetWindowRect(rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		ii = MulDiv(point.y-rr.top,theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp]-1,rr.Height());
		if(dlg.DoModal()==IDOK) {
			theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].bSet=TRUE;
			theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].rgbValue=dlg.GetColor();
//			prgbCustomColor = dlg.GetSavedCustomColors();
			for(int ii = 0;ii<16;ii++) {
//				theApp.rgbCustomColor[ii] = prgbCustomColor[ii];
			}
		};
		theApp.m_Palette.CalculatePalette();
		Invalidate(FALSE);
		SendMessage(WM_PAINT);
		UpdateRelatedDialogs();
	}
	m_staticSlider.GetWindowRect(rr);
	ScreenToClient(rr);
	rr.left-=3;
	rr.right+=3;
	float fHeight  = (float)rr.Height();
	int nBottom = rr.bottom;
	float fRange = 100.0;
	if(theApp.m_Palette.m_nAmplitudeScaleType[m_nWhichAmp] == 2) fRange = (float)theApp.m_LastSettings.nLogRange;
	for(ii=0;(ii<theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp])&&(m_nSlideEntry==-1);ii++) {
		pnY = nBottom - (int)((theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].fValue*fHeight)/fRange);
		rr.top = pnY-3;
		rr.bottom = pnY+3;
		if(rr.PtInRect(point)) {
			m_nSlideEntry = ii;
			m_bMoveSlider=TRUE;
		};
	}

	m_staticTopColour.GetWindowRect(rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbThresholdTop=dlg.GetColor();
		};
		Invalidate(FALSE);
		SendMessage(WM_PAINT);
		UpdateRelatedDialogs();
	}

	m_staticBottomColour.GetWindowRect(rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbThresholdBottom=dlg.GetColor();
		};
		Invalidate(FALSE);
		SendMessage(WM_PAINT);
		UpdateRelatedDialogs();
	}


	
	
	
	

	CDialog::OnLButtonDown(nFlags, point);
}

void CAmplitudePalettePage::OnSelchangeComboMode() 
{
	theApp.m_Palette.m_nModeAmplitude[m_nWhichAmp]=m_comboMode.GetCurSel();
	theApp.m_Palette.CalculatePalette();
	Invalidate(FALSE);
	UpdateRelatedDialogs();
}

void CAmplitudePalettePage::OnChangeEditStepsize() 
{
	CString	Buff;

	m_editStepSize.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Palette.m_ddBStepSize[m_nWhichAmp]);
	if(theApp.m_Palette.m_ddBStepSize[m_nWhichAmp]<=0.0) theApp.m_Palette.m_ddBStepSize[m_nWhichAmp]=0.1;
	m_spinStepSize.SetPos((int)(theApp.m_Palette.m_ddBStepSize[m_nWhichAmp]*10));
	theApp.m_Palette.CalculatePalette();
	Invalidate(FALSE);
	UpdateRelatedDialogs();

}



void CAmplitudePalettePage::SlowRefreshBarGraph(CPaintDC *pDC,CRect rr)
{
	CRect	bar;
	CBrush* pOldBrush;
	CBrush* pBrush;
	CPen*	pOldPen;
	CPen*	pPen;
	int	yy,nColor,gg,nPny;
	static	bool bFlag=FALSE;
	CString Buff;
	double	dStep;

	bar = rr;
	rr.DeflateRect(2,2,2,2);
	bar.top+=8;
	bar.bottom-=8;
	bar.left+=4;
	bar.right=bar.left+8;

	pBrush = new CBrush;
	pBrush->CreateSolidBrush(RGB(255,255,255));
	pOldBrush=pDC->SelectObject(pBrush);

	if(bFlag==FALSE) {
		pDC->Rectangle(rr);
	//	bFlag=TRUE;
	};

	switch(theApp.m_Palette.m_nAmplitudeScaleType[m_nWhichAmp]) {
	case 0:	//Blended
		for(gg=0;gg<10;gg++) {
			for(yy=gg;yy<=bar.Height();yy+=10) {
				nColor=MulDiv(yy,theApp.m_Palette.m_nMaxAmplitudeNumber[m_nWhichAmp],bar.Height());
				pPen = new CPen;
				pPen->CreatePen(PS_SOLID,0,theApp.m_Palette.m_rgbAmplitude[m_nWhichAmp][nColor]);
				pOldPen=pDC->SelectObject(pPen);
				pDC->MoveTo(bar.left,bar.bottom-yy);
				pDC->LineTo(bar.right,bar.bottom-yy);
				pDC->SelectObject(pOldPen);
				delete pPen;
			};
		};
		break;
	case 1:
	case 2:
		for(gg=0;gg<10;gg++) {
			for(yy=gg;yy<=bar.Height();yy+=10) {
				nColor=MulDiv(yy,127,bar.Height())+128;
				pPen = new CPen;
				pPen->CreatePen(PS_SOLID,0,theApp.m_Palette.m_rgbAmplitude[m_nWhichAmp][nColor]);
				pOldPen=pDC->SelectObject(pPen);
				pDC->MoveTo(bar.left,bar.bottom-yy);
				pDC->LineTo(bar.right,bar.bottom-yy);
				pDC->SelectObject(pOldPen);
				delete pPen;
			};
		};
		break;
	}


	LOGFONT lf;
	memset( (void*)&lf, 0, sizeof(lf));
	lf.lfHeight = 10;

	CRect rrClient;
	GetClientRect(rr);
	lf.lfHeight = MulDiv(rrClient.Height(),10,m_rrClient.Height());

	theApp.SetFontLanguage(&lf);

	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);
	double dDb[6]={1.0,0.707,0.5,0.1};
	CString	str[4]={L"0",L"-3",L"-6",L"-20"};
	int	ii;

	CPen BlackPen(PS_SOLID,0,RGB(0,0,0));
	pOldPen=pDC->SelectObject(&BlackPen);
	switch(theApp.m_Palette.m_nAmplitudeScaleType[m_nWhichAmp]) {
	case 0:	//Linear
		for(ii=0;ii<4;ii++) {
			nPny=bar.Height()/2 - (int)(dDb[ii]*(double)(bar.Height()/2)) +bar.top;
			pDC->MoveTo(bar.right+2,nPny);
			pDC->LineTo(bar.right+8,nPny);
			Buff.Format(_T("%s"),str[ii]);
			pDC->TextOut(bar.right+8,nPny-5,Buff);
		};
		break;
		break;
	case 2: //Log
		int nTop = (int)((bar.Height() * theApp.m_Palette.m_ddBZeroLevel[m_nWhichAmp]) / theApp.m_LastSettings.nLogRange);
		for(dStep=0;dStep<=100.0;dStep+=10.0) {
			nPny=bar.bottom-(int)((dStep*(double)bar.Height())/(float)theApp.m_LastSettings.nLogRange);
			pDC->MoveTo(bar.right,nPny);
			pDC->LineTo(bar.right+3,nPny);
			Buff.Format(_T("%.01f"),dStep);
			pDC->TextOut(bar.right+3,nPny-5,Buff);
		};
		break;
	};
	/*
	for(dStep=0;dStep<=theApp.m_Palette.m_dMaxThickness;dStep+=theApp.m_Palette.m_dStepSize) {
		nPny=bar.bottom-(int)((dStep*(double)bar.Height())/theApp.m_Palette.m_dMaxThickness);
		pDC->MoveTo(bar.right+2,nPny);
		pDC->LineTo(bar.right+8,nPny);
		Buff.Format(_T("%.02f"),dStep);
		pDC->TextOut(bar.right+8,nPny-5,Buff);
	};
	*/
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	delete pBrush;

}



void CAmplitudePalettePage::RefreshBarGraph(CPaintDC *pDC, CRect rect,int nWhichAmp,LOGFONT *plf,int nDevice)
{
	if(nWhichAmp != -1) m_nWhichAmp = nWhichAmp;

	switch(theApp.m_Palette.m_nGraphType[m_nWhichAmp]) {
	case 0: RefreshBarGraphTotal(pDC, rect,plf,nDevice);
		break;
	case 1: RefreshBarGraphWindow(pDC, rect, nDevice);
		break;
	}
}

void CAmplitudePalettePage::RefreshBarGraphTotal(CPaintDC *pDC, CRect rect,LOGFONT *plf,int nDevice)
{
	CRect	bar;
	CBrush* pOldBrush;
	CBrush* pBrush;
	CPen*	pOldPen;
	int		xx,yy,nColor,nPny;
	double dStep;
	static	bool bFlag=FALSE;
	CString Buff;

	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nBytesPixel = nBitsPixel/8;
	if(nBytesPixel<0) nBytesPixel=1;

	LOGFONT lf;
	if(plf==NULL) {
		memset( (void*)&lf, 0, sizeof(lf));
		lf.lfHeight = 12;
		CRect rrClient;
		GetClientRect(rrClient);
		lf.lfHeight = MulDiv(rrClient.Height(),12,m_rrClient.Height());
		theApp.SetFontLanguage(&lf);
	} else {
		CopyMemory(&lf,plf,sizeof(lf));
	}

	CSize size;
	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);
	Buff = "-100.0%";
	size = pDC->GetTextExtent(Buff);

	bar = rect;
//	rect.DeflateRect(2,2,2,2);
	bar.top+=size.cy/2;
	bar.bottom-=size.cy/2;
	bar.left+=4;
	bar.right=rect.right-size.cx-3;
	if(bar.right<=bar.left) bar.right=bar.left + 10;
	if(bar.bottom<=bar.top) bar.bottom = bar.top+1;

	int nWidth = bar.Width();
	bar.right = bar.left + (((nWidth / 8) + 1) * 8);

	pBrush = new CBrush;
	pBrush->CreateSolidBrush(RGB(255,255,255));
	pOldBrush=pDC->SelectObject(pBrush);

	if(bFlag==FALSE) {
		pDC->Rectangle(rect);
	//	bFlag=TRUE;
	};

	CDC  dcMem;
	CBitmap bmp;
	char	*pArray,*pBuffer;
	pArray = new char[(bar.Width()+2) * (bar.Height()+2) * 4];
	ZeroMemory(pArray,(bar.Width()+2) * (bar.Height()+2) * 4);

	for(yy=0;yy<(bar.Height()-1);yy++) {
		pBuffer=&pArray[((bar.Height()-1-yy)*bar.Width()) * nBytesPixel];

		nColor=MulDiv(yy,127,(bar.Height()-1))+128;
		if((nDevice==PRINTER) && (theApp.m_LastSettings.bRedBlueReverse==true)) {
			for(xx=0;xx<bar.Width();xx++) {
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nWhichAmp][nColor]>>0)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nWhichAmp][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nWhichAmp][nColor]>>16)&0xff);
				if(nBytesPixel>3) pBuffer++;
			};
		} else {
			for(xx=0;xx<bar.Width();xx++) {
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nWhichAmp][nColor]>>16)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nWhichAmp][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nWhichAmp][nColor]>>0)&0xff);
				if(nBytesPixel>3) pBuffer++;
			};
		}
	};

	bmp.CreateCompatibleBitmap(pDC,bar.Width(),bar.Height());
	bmp.SetBitmapBits((DWORD)(bar.Width()*bar.Height()*nBytesPixel),pArray);
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	pDC->BitBlt(bar.left,bar.top,bar.Width(),bar.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);


	int	ii;
	int nTop;

	CPen BlackPen(PS_SOLID,0,RGB(0,0,0));
	pOldPen=pDC->SelectObject(&BlackPen);
	int dY = bar.Height()-1;
	float fdB;
	switch(theApp.m_Palette.m_nAmplitudeScaleType[m_nWhichAmp]) {
	case 0:	//100% FSH
		nTop = (int)((bar.Height() * theApp.m_Palette.m_ddBZeroLevel[m_nWhichAmp]) / 100.0);
		for(dStep=0;dStep<=100.0;dStep+=10.0) {
			nPny=bar.top+(int)((dStep*(double)dY)/100.0);
			if((int)dStep==60) nPny+=1;
			if((int)dStep==30) nPny+=1;
			pDC->MoveTo(bar.right,nPny);
			pDC->LineTo(bar.right+3,nPny);
			Buff.Format(_T("%.0f%%"),100.0-dStep);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(bar.right+3,nPny-size.cy/2,Buff);
		};
		break;
	case 1:	//dB FSH
		switch(theApp.m_Palette.m_nStepMode[m_nWhichAmp]) {
		case 0:
			for(ii=0;ii<theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp];ii++) {
				fdB = (float)((theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]/2.0) + theApp.m_Palette.m_ddBStepSize[m_nWhichAmp] * (double)ii);
				nPny = bar.bottom - (int)(pow(10,(fdB / -20.0f)) * (double)bar.Height());
				pDC->MoveTo(bar.right+2,nPny);
				pDC->LineTo(bar.right+8,nPny);
				Buff.Format(_T("%.01f"),-fdB);
				size = pDC->GetTextExtent(Buff);
				pDC->TextOut(bar.right+8,nPny-size.cy/2,Buff);
			}
			break;
		case 1:
		case 2:
			for(ii=0;ii<theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp];ii++) {

				fdB = 20.0f * (float)log10(theApp.m_Palette.m_Amplitude[m_nWhichAmp][0].fValue/theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].fValue);

				nPny=bar.Height() - (int)((theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].fValue*(float)dY)/100.0f) + bar.top;
				pDC->MoveTo(bar.right+2,nPny);
				pDC->LineTo(bar.right+8,nPny);
				Buff.Format(_T("%.01f"),fdB);
				size = pDC->GetTextExtent(Buff);
				pDC->TextOut(bar.right+8,nPny-size.cy/2,Buff);
			};
			break;
		}
		break;
	case 2: //100dB Log
		nTop = (int)((bar.Height() * theApp.m_Palette.m_ddBZeroLevel[m_nWhichAmp]) / (float)theApp.m_LastSettings.nLogRange);
		for(dStep=0;dStep<=(double)theApp.m_LastSettings.nLogRange;dStep+=10.0) {
			nPny=bar.top+(int)((dStep*(double)dY)/(double)theApp.m_LastSettings.nLogRange);
			pDC->MoveTo(bar.right,nPny);
			pDC->LineTo(bar.right+3,nPny);
			Buff.Format(_T("%.0fdB"),dStep);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(bar.right+3,nPny-size.cy/2,Buff);
		};
		break;
	};
	/*
	for(dStep=0;dStep<=theApp.m_Palette.m_dMaxThickness;dStep+=theApp.m_Palette.m_dStepSize) {
		nPny=bar.bottom-(int)((dStep*(double)bar.Height())/theApp.m_Palette.m_dMaxThickness);
		pDC->MoveTo(bar.right+2,nPny);
		pDC->LineTo(bar.right+8,nPny);
		Buff.Format(_T("%.02f"),dStep);
		pDC->TextOut(bar.right+8,nPny-5,Buff);
	};
	*/
	pDC->SetBkMode(OPAQUE);
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	delete pBrush;
	delete pArray;

}

void CAmplitudePalettePage::RefreshBarGraphWindow(CPaintDC *pDC, CRect rect,int nDevice)
{
	CRect	bar;
	CBrush* pOldBrush;
	CBrush* pBrush;
	CPen*	pOldPen;
	int		xx,yy,nColor,nPny,nn;
	double dStep;
	static	bool bFlag=FALSE;
	CString Buff;

	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nBytesPixel = nBitsPixel/8;
	if(nBytesPixel<0) nBytesPixel=1;

	bar = rect;
//	rect.DeflateRect(2,2,2,2);
	bar.top+=10;
	bar.bottom-=10;
	bar.left+=4;
	bar.right=bar.left+8;

	pBrush = new CBrush;
	pBrush->CreateSolidBrush(RGB(255,255,255));
	pOldBrush=pDC->SelectObject(pBrush);

	if(bFlag==FALSE) {
		pDC->Rectangle(rect);
	//	bFlag=TRUE;
	};

	CDC  dcMem;
	CBitmap bmp;
	char	*pArray,*pBuffer;
	pArray = new char[(bar.Width()+2) * (bar.Height()+2) * 4];
	dStep = theApp.m_Palette.m_ddBStepSize[m_nWhichAmp];
	int nEntries = theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp];

	float fRange = 100.0f;
	if(theApp.m_Palette.m_nAmplitudeScaleType[m_nWhichAmp] == 2) fRange = (float)theApp.m_LastSettings.nLogRange;
	int nTopColor = (int)(theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp] * 127.0 /fRange) + 128;
	int	nBottomColor = nTopColor-(int)(dStep*(double)(nEntries)*127.0/fRange);

	for(yy=0;yy<(bar.Height()-1);yy++) {
		pBuffer=&pArray[((bar.Height()-1-yy)*bar.Width()) * nBytesPixel];

		nColor=MulDiv(yy,nTopColor-nBottomColor,(bar.Height()-1))+nBottomColor;
		if((nDevice==PRINTER) && (theApp.m_LastSettings.bRedBlueReverse==true)) {
			for(xx=0;xx<bar.Width();xx++) {
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nWhichAmp][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nWhichAmp][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nWhichAmp][nColor]>>16)&0xff);
				if(nBytesPixel>3) pBuffer++;
			};
		} else {
			for(xx=0;xx<bar.Width();xx++) {
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nWhichAmp][nColor]>>16)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nWhichAmp][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nWhichAmp][nColor]>>0)&0xff);
				if(nBytesPixel>3) pBuffer++;
			};
		}
	};

	bmp.CreateCompatibleBitmap(pDC,bar.Width(),bar.Height());
	bmp.SetBitmapBits((DWORD)(bar.Width()*bar.Height()*nBytesPixel),pArray);
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	pDC->BitBlt(bar.left,bar.top,bar.Width(),bar.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);


	LOGFONT lf;
	memset( (void*)&lf, 0, sizeof(lf));
	lf.lfHeight = 12;
	CRect rrClient;
	GetClientRect(rrClient);
	lf.lfHeight = MulDiv(rrClient.Height(),12,m_rrClient.Height());
	theApp.SetFontLanguage(&lf);

	CSize size;
	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);
	double dDb[6]={1.0,0.707,0.5,0.316,0.251,0.1};
	CString	str[6]={L"0",L"-3",L"-6",L"-10",L"-12",L"-20"};
	int	ii;

	CPen BlackPen(PS_SOLID,0,RGB(0,0,0));
	pOldPen=pDC->SelectObject(&BlackPen);
	int dY = bar.Height()-1;
	switch(theApp.m_Palette.m_nAmplitudeScaleType[m_nWhichAmp]) {
	case 0:	//100% FSH
		for(nn=0;nn<(nEntries+1);nn++) {
			nPny=MulDiv(nn,dY,nEntries)+bar.top;
			pDC->MoveTo(bar.right,nPny);
			pDC->LineTo(bar.right+3,nPny);
			Buff.Format(_T("%.0f%%"),theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]-dStep*(double)nn);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(bar.right+3,nPny-size.cy/2,Buff);
		}
		break;
	case 1:	//dB FSH
		for(ii=0;ii<6;ii++) {
			nPny=bar.Height() - (int)(dDb[ii]*(double)dY) +bar.top;
			pDC->MoveTo(bar.right+2,nPny);
			pDC->LineTo(bar.right+8,nPny);
			Buff.Format(_T("%s"),str[ii]);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(bar.right+8,nPny-size.cy/2,Buff);
		};
		break;
		break;
	case 2: //100dB Log
		for(nn=0;nn<(nEntries+1);nn++) {
			nPny=MulDiv(nn,dY,nEntries)+bar.top;
			pDC->MoveTo(bar.right,nPny);
			pDC->LineTo(bar.right+3,nPny);
			Buff.Format(_T("%.0fdB"),theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]-theApp.m_LastSettings.nLogRange-dStep*(double)nn);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(bar.right+3,nPny-size.cy/2,Buff);
		}
		break;
	};
	/*
	for(dStep=0;dStep<=theApp.m_Palette.m_dMaxThickness;dStep+=theApp.m_Palette.m_dStepSize) {
		nPny=bar.bottom-(int)((dStep*(double)bar.Height())/theApp.m_Palette.m_dMaxThickness);
		pDC->MoveTo(bar.right+2,nPny);
		pDC->LineTo(bar.right+8,nPny);
		Buff.Format(_T("%.02f"),dStep);
		pDC->TextOut(bar.right+8,nPny-5,Buff);
	};
	*/
	pDC->SetBkMode(OPAQUE);
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

	delete pBrush;
	delete pArray;

}


void CAmplitudePalettePage::RefreshSlider(CPaintDC *pDC,CRect rr)
{
	int	ii,pnY,pnX;
	CBrush* pOldBrush;
	CBrush* pBrush;
	CRect rrBK = rr;
	rrBK.InflateRect(3,5);
	CString	Buff;
	CSize size;

	LOGFONT lf;
	memset( (void*)&lf, 0, sizeof(lf));
	lf.lfHeight = 14;
	CRect rrClient;
	GetClientRect(rrClient);
	lf.lfHeight = MulDiv(rrClient.Height(),14,m_rrClient.Height());
	theApp.SetFontLanguage(&lf);

	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);

	Buff.Format(_T("%d%%"),100);
	size=pDC->GetTextExtent(Buff);
	rrBK.right+=(size.cx+6);

	
	pBrush = new CBrush(GetSysColor(COLOR_BTNFACE));
	pDC->FillRect(rrBK,pBrush);
	delete pBrush;
	pnX=(rr.left+rr.right)/2;

	float fRange = 100.0;
	if(theApp.m_Palette.m_nAmplitudeScaleType[m_nWhichAmp] == 2) fRange = (float)theApp.m_LastSettings.nLogRange;

	for(ii=0;ii<theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp];ii++) {
		if(theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].fValue > fRange) theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].fValue = fRange;
		if(theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].fValue <   0.0f) theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].fValue =   0.0f;
		pBrush = new CBrush(theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].rgbValue);
		pOldBrush = pDC->SelectObject(pBrush);

		pnY = rr.bottom - (int)((theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].fValue*(float)rr.Height())/fRange);
		pDC->Ellipse(pnX-4,pnY-4,pnX+5,pnY+5);

		switch(theApp.m_Palette.m_nAmplitudeScaleType[m_nWhichAmp]) {
		case 0:
		case 1:
			Buff.Format(_T("%.0f%%"),theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].fValue);
			break;
		case 2:
			Buff.Format(_T("-%.0fdB"),fRange-theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].fValue);
			break;
		}
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(pnX+6,pnY-size.cy/2,Buff);

		pDC->SelectObject(pOldBrush);
		delete pBrush;
	}

	pDC->SetBkMode(OPAQUE);

	pOldFont = pDC->SelectObject(pOldFont);


}


void CAmplitudePalettePage::OnSelchangeComboScaleType() 
{
	theApp.m_Palette.m_nAmplitudeScaleType[m_nWhichAmp] = m_comboScaleType.GetCurSel();
	UpdateAllControls();
	Invalidate(FALSE);
}



void CAmplitudePalettePage::OnChangeEditTopLevel() 
{
	CString	Buff;
	double	dValue=theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp];
	float fTemp;

	m_editTopLevel.GetWindowText(Buff);
	switch(theApp.m_Palette.m_nAmplitudeScaleType[m_nWhichAmp]) {
	case 0:
		_WTOF(Buff,theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]);
		if(theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]>100.0) theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]=100.0;
		if(theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]<0.0) theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]=0.0;
		m_spinTopLevel.SetPos((int)(theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]));
		if(dValue-theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]) {
			theApp.m_Palette.CalculatePalette();
			Invalidate(FALSE);
			UpdateRelatedDialogs();
		};
		break;
	case 1:
		_WTOF(Buff,theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]);
		theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp] *= -2.0;
		if(theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]>100.0) theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]=100.0;
		if(theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]<0.0) theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]=0.0;
		m_spinTopLevel.SetPos((int)(theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]));
		if(dValue-theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]) {
			theApp.m_Palette.CalculatePalette();
			Invalidate(FALSE);
			UpdateRelatedDialogs();
		};
		break;
	case 2:
		_WTOF(Buff,fTemp);
		theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp] = theApp.m_LastSettings.nLogRange - fTemp;
		if(theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]>theApp.m_LastSettings.nLogRange) theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]=theApp.m_LastSettings.nLogRange;
		if(theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]<0.0) theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]=0.0;
		m_spinTopLevel.SetPos(100-(int)(theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]));
		if(dValue-theApp.m_Palette.m_ddBTopLevel[m_nWhichAmp]) {
			theApp.m_Palette.CalculatePalette();
			Invalidate(FALSE);
			UpdateRelatedDialogs();
		};
		break;
	}
}

void CAmplitudePalettePage::UpdateRelatedDialogs()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	ASSERT(pFrame);
	
	for(int nn=0;nn<MAX_NUMBER_VIEWS;nn++) {
		if(pFrame->m_pViewSheet[nn]!=NULL) {
			pFrame->m_pViewSheet[nn]->InvalidateChild();
		};
	};
	if(pFrame->m_pProbeCharacterisationSheet) {
		pFrame->m_pProbeCharacterisationSheet->UpdateAllPages();
	}

}


void CAmplitudePalettePage::OnSelchangeComboWhichGate() 
{
	theApp.m_Palette.m_nWhichAmp = m_nWhichAmp = m_comboWhichGate.GetCurSel();
	UpdateAllControls();
	Invalidate(TRUE);
}

void CAmplitudePalettePage::OnButtonMakeItSo() 
{
	int nAction = m_comboCopyTo.GetCurSel() - 1;
	int	ii;

	if(nAction==-1) {
		for(ii=0;ii<8;ii++) {
			theApp.m_Palette.CopyAmpltudePaletteTo(m_nWhichAmp,ii);
		}
	} else {
		theApp.m_Palette.CopyAmpltudePaletteTo(m_nWhichAmp,nAction);
	}

}

void CAmplitudePalettePage::OnChangeEditNumberOfEntries() 
{
	CString Buff;
	int nTemp=theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp];

	m_editNumberOfEntries.GetWindowText(Buff);
	theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp]=_ttoi(Buff);
	if(nTemp-theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp]) {
		m_spinNumberOfEntries.SetPos(theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp]);
	}

}

void CAmplitudePalettePage::OnSelchangeComboStepMode() 
{
	theApp.m_Palette.m_nStepMode[m_nWhichAmp]=m_comboStepMode.GetCurSel();
	theApp.m_Palette.CalculatePalette();
	Invalidate(FALSE);
	UpdateRelatedDialogs();

}

BOOL CAmplitudePalettePage::OnSetActive() 
{
	theApp.m_LastSettings.nLastPaletteTab = CAmplitudePalettePage::IDD;
	UpdateAllControls();

	return CResizablePage::OnSetActive();
}

void CAmplitudePalettePage::OnSelchangeComboGraphType() 
{
	theApp.m_Palette.m_nGraphType[m_nWhichAmp]=m_comboGraphType.GetCurSel();
	Invalidate(FALSE);
}



void CAmplitudePalettePage::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect	rr;
	float fValue[256];
	int ii;
	float fRange = 100.0;
	float fTopValue;

	if(m_bMoveSlider == TRUE) {

		if(theApp.m_Palette.m_nAmplitudeScaleType[m_nWhichAmp] == 2) fRange = (float)theApp.m_LastSettings.nLogRange;
		m_staticSlider.GetWindowRect(rr);
		ScreenToClient(rr);

		switch(theApp.m_Palette.m_nStepMode[m_nWhichAmp]) {
		default:
			if(m_nSlideEntry>=0) {
				theApp.m_Palette.m_Amplitude[m_nWhichAmp][m_nSlideEntry].fValue = ((float)(rr.bottom-point.y)*fRange)/(float)rr.Height();
				if(theApp.m_Palette.m_Amplitude[m_nWhichAmp][m_nSlideEntry].fValue >= fRange) theApp.m_Palette.m_Amplitude[m_nWhichAmp][m_nSlideEntry].fValue = fRange;
				if(theApp.m_Palette.m_Amplitude[m_nWhichAmp][m_nSlideEntry].fValue < 0.0f) theApp.m_Palette.m_Amplitude[m_nWhichAmp][m_nSlideEntry].fValue = 0.0f;
			}
			break;
		case 2:
			if(m_nSlideEntry==0) {
				for(ii=0;ii<theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp];ii++) fValue[ii] = theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].fValue;

				fTopValue = ((float)(rr.bottom-point.y)*fRange)/(float)rr.Height();
				if(fTopValue > (float)theApp.m_LastSettings.nLogRange) fTopValue = (float)theApp.m_LastSettings.nLogRange;

				for(ii=0;ii<theApp.m_Palette.m_nAmplitudeEntriesl[m_nWhichAmp];ii++) {
					switch(theApp.m_Palette.m_nAmplitudeScaleType[m_nWhichAmp]) {
					case 0:
					case 2:
						theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].fValue = fTopValue - (fValue[0] - fValue[ii]);
						break;
					case 1:
						theApp.m_Palette.m_Amplitude[m_nWhichAmp][ii].fValue = fTopValue / (fValue[0] / fValue[ii]);
						break;
					}
				}
			}
			break;
		}


		theApp.m_Palette.CalculatePalette();
		Invalidate(FALSE);
	//	SendMessage(WM_PAINT);
		UpdateRelatedDialogs();

	}

	CResizablePage::OnMouseMove(nFlags, point);
}

void CAmplitudePalettePage::OnLButtonUp(UINT nFlags, CPoint point) 
{

	m_nSlideEntry=-1;
	m_bMoveSlider=FALSE;

	CResizablePage::OnLButtonUp(nFlags, point);
}

BOOL CAmplitudePalettePage::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	theApp.m_Help.Open(_T("PaletteDlg.html"),NULL);
	
	return CResizablePage::OnHelpInfo(pHelpInfo);
}

void CAmplitudePalettePage::SetAccessPrivelage()
{
	bool bFlag = false;

	if(theApp.m_Palette.m_nStepMode[m_nWhichAmp]==0) {
		bFlag = TRUE;
	} else {
		bFlag = FALSE;
	}
	m_editStepSize.EnableWindow(bFlag);
	m_editTopLevel.EnableWindow(bFlag);
	m_spinTopLevel.EnableWindow(bFlag);
	m_spinStepSize.EnableWindow(bFlag);

	theApp.m_nLogonLevelMask & theApp.m_cAccess[IDD_PALETTE_AMPLITUDE_DIALOG] ? bFlag = true : bFlag = false;

	m_comboStepMode.EnableWindow(bFlag);
	m_staticSlider.EnableWindow(bFlag);
	m_comboGraphType.EnableWindow(bFlag);
	m_spinNumberOfEntries.EnableWindow(bFlag);
	m_editNumberOfEntries.EnableWindow(bFlag);
	m_comboCopyTo.EnableWindow(bFlag);
	m_comboWhichGate.EnableWindow(bFlag);
	m_spinTopLevel.EnableWindow(bFlag);
	m_editTopLevel.EnableWindow(bFlag);
	m_comboScaleType.EnableWindow(bFlag);
	m_staticEntries.EnableWindow(bFlag);
	m_staticBarGraph.EnableWindow(bFlag);
	m_spinStepSize.EnableWindow(bFlag);
	m_editStepSize.EnableWindow(bFlag);
	m_comboMode.EnableWindow(bFlag);



}


void CAmplitudePalettePage::OnChangeEditThresholdSpan() 
{
	CString Buff;
	float fTemp = theApp.m_LastSettings.fThresholdSpan;

	m_editThresholdSpan.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fThresholdSpan);
	if(fTemp - theApp.m_LastSettings.fThresholdSpan) {
		m_spinThresholdSpan.SetPos((int)(theApp.m_LastSettings.fThresholdSpan * 10.0f));
		theApp.m_Palette.CalculatePalette();
		Invalidate(FALSE);
		UpdateRelatedDialogs();
	}
}

void CAmplitudePalettePage::OnDeltaposSpinThresholdSpan(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	float fTemp = theApp.m_LastSettings.fThresholdSpan;
	CString Buff;

	theApp.m_LastSettings.fThresholdSpan += (float)pNMUpDown->iDelta * 0.1f;
	if(theApp.m_LastSettings.fThresholdSpan < 0.0f) theApp.m_LastSettings.fThresholdSpan = 0.0f;
	if(theApp.m_LastSettings.fThresholdSpan > 100.0f) theApp.m_LastSettings.fThresholdSpan = 100.0f;

	if(fTemp - theApp.m_LastSettings.fThresholdSpan) {

		Buff.Format(L"%.01fdB",theApp.m_LastSettings.fThresholdSpan);
		m_editThresholdSpan.SetWindowText(Buff);
	}
	
	*pResult = 0;
}


void CAmplitudePalettePage::OnSelchangeComboThresholdMode() 
{
	theApp.m_LastSettings.nThresholdMode = m_comboThresholdMode.GetCurSel();
	theApp.m_Palette.CalculatePalette();
	Invalidate(FALSE);
	UpdateRelatedDialogs();
}

void CAmplitudePalettePage::OnChangeEditTopThreshold() 
{
	CString Buff;
	float fTemp = theApp.m_LastSettings.fThresholdTop;
	float fNewValue;

	m_editTopThreshold.GetWindowText(Buff);
	_WTOF(Buff,fNewValue);

	switch(theApp.m_Palette.m_nAmplitudeScaleType[m_nWhichAmp]) {
	case 0: theApp.m_LastSettings.fThresholdTop = 100.0f - fNewValue;
		break;
	case 1:	//	dTop = -20.0 * log10((100.0-theApp.m_LastSettings.fThresholdTop) / 100.0);
		theApp.m_LastSettings.fThresholdTop = 100.0f - (float)(pow(10.0,(double)(fNewValue / -20.0f)) * 100.0);

		break;
	case 2: theApp.m_LastSettings.fThresholdTop = fNewValue;
		break;
	}

	if(fTemp - theApp.m_LastSettings.fThresholdTop) {
		m_spinTopThreshold.SetPos((int)(theApp.m_LastSettings.fThresholdTop * 10.0f));
		theApp.m_Palette.CalculatePalette();
		Invalidate(FALSE);
		UpdateRelatedDialogs();
	}
	
	
}

void CAmplitudePalettePage::OnDeltaposSpinTopThreshold(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	float fTemp = theApp.m_LastSettings.fThresholdTop;
	CString Buff;

	theApp.m_LastSettings.fThresholdTop -= (float)pNMUpDown->iDelta * 1.0f;
	MinMax(&theApp.m_LastSettings.fThresholdTop,0.0f,100.0f);

	if(fTemp - theApp.m_LastSettings.fThresholdTop) {

		SetTopThresholdText();

		m_sliderThreshold.SetPos((int)(theApp.m_LastSettings.fThresholdTop * 10.0f));
	}
	
	*pResult = 0;
}

void CAmplitudePalettePage::SetTopThresholdText()
{
	CString Buff;
	double	dTop;

	switch(theApp.m_Palette.m_nAmplitudeScaleType[m_nWhichAmp]) {
	case 0: 
		Buff.Format(L"%.0f%%",100.0f - theApp.m_LastSettings.fThresholdTop);
		break;
	case 1:
		dTop = -20.0 * log10((100.0-theApp.m_LastSettings.fThresholdTop) / 100.0);
		Buff.Format(L"%.01fdB",dTop);
		break;
	case 2:
		Buff.Format(L"%.01fdB",theApp.m_LastSettings.fThresholdTop);
		break;
	}
	m_editTopThreshold.SetWindowText(Buff);
}


