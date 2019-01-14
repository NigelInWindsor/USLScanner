// UtTrafficLightPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "UtTrafficLightPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	_GREATER	0
#define	_BELOW	1

/////////////////////////////////////////////////////////////////////////////
// CUtTrafficLightPage property page

IMPLEMENT_DYNCREATE(CUtTrafficLightPage, CPropertyPage)

CUtTrafficLightPage::CUtTrafficLightPage() : CPropertyPage(CUtTrafficLightPage::IDD)
{
	//{{AFX_DATA_INIT(CUtTrafficLightPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pFont = NULL;
	m_pArray = NULL;
	m_strAmplitude = L"0.00";

}

CUtTrafficLightPage::~CUtTrafficLightPage()
{
	SAFE_DELETE(m_pFont);
	SAFE_DELETE(m_pArray);
}

void CUtTrafficLightPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUtTrafficLightPage)
	DDX_Control(pDX, IDC_COMBO_GATE, m_comboGate);
	DDX_Control(pDX, IDC_COMBO_TIMESLOT, m_comboTimeSlot);
	DDX_Control(pDX, IDC_STATIC_AMPLITUDE, m_staticAmplitude);
	DDX_Control(pDX, IDC_STATIC_LESS_THAN, m_staticLessThan);
	DDX_Control(pDX, IDC_STATIC_GREATER, m_staticGreater);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUtTrafficLightPage, CPropertyPage)
	//{{AFX_MSG_MAP(CUtTrafficLightPage)
	ON_CBN_SELCHANGE(IDC_COMBO_GATE, OnSelchangeComboGate)
	ON_CBN_SELCHANGE(IDC_COMBO_TIMESLOT, OnSelchangeComboTimeslot)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUtTrafficLightPage message handlers
void CUtTrafficLightPage::OnDestroy() 
{

	CPropertyPage::OnDestroy();
}

BOOL CUtTrafficLightPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CRect rr;
	LOGFONT	lf;
	
	UpdateAllControls();

	CFont* pFont = GetFont();
	pFont->GetLogFont(&m_LogFont);

	m_staticAmplitude.GetWindowRect(rr);
	lf = m_LogFont;
	lf.lfHeight = (int)((float)rr.Height() / -1.50f);
	theApp.SetFontLanguage(&lf);
	SAFE_DELETE( m_pFont );
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&lf);

	Invalidate(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUtTrafficLightPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	int nTimeSlot = MinMax(&theApp.m_LastSettings.nAlarmTimeSlot,0,7);
	int nGate = MinMax(&theApp.m_LastSettings.nAlarmGate,0,7);
	GatesData* pGate = &theApp.m_UtUser.m_TS[nTimeSlot].Gate;

	m_comboTimeSlot.ResetContent();
	if(theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30]==2) {
		m_comboTimeSlot.InsertString(0,_T("LSA"));
		m_comboTimeSlot.SetCurSel(0);
		m_comboTimeSlot.EnableWindow(false);
	}else{
		for(int nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
			m_comboTimeSlot.AddString(theApp.m_UtUser.m_TS[nTS].cName);
		}
		m_comboTimeSlot.SetCurSel(theApp.m_UtUser.m_Global.nTimeSlot);
	}	


	m_comboGate.ResetContent();
	m_comboGate.LimitText(sizeof theApp.m_UtUser.m_TS[nTimeSlot].Gate.cName[0] - 1);
	for(int gg=0;gg<8;gg++) {
		m_comboGate.AddString(theApp.m_UtUser.m_TS[nTimeSlot].Gate.cName[gg]);
	}
	m_comboGate.SetCurSel(nGate);

}

void CUtTrafficLightPage::OnSelchangeComboGate() 
{
	theApp.m_LastSettings.nAlarmGate = m_comboGate.GetCurSel();
}

void CUtTrafficLightPage::OnSelchangeComboTimeslot() 
{
	int nTemp = theApp.m_LastSettings.nAlarmTimeSlot;

	theApp.m_LastSettings.nAlarmTimeSlot = m_comboTimeSlot.GetCurSel();
	if(nTemp - theApp.m_LastSettings.nAlarmTimeSlot) {
		UpdateAllControls();
	}
	
}

void CUtTrafficLightPage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CMFCColorDialog dlg;
	CRect rr;

	m_staticGreater.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbAlarm[_GREATER]=dlg.GetColor();
			Invalidate(FALSE);
		}
	}
	m_staticLessThan.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbAlarm[_BELOW]=dlg.GetColor();
			Invalidate(FALSE);
		}
	}
	
	CPropertyPage::OnLButtonDown(nFlags, point);
}


BOOL CUtTrafficLightPage::OnSetActive() 
{
	UpdateAllControls();
	SetTimer(1,100,NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CUtTrafficLightPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}


void CUtTrafficLightPage::OnTimer(UINT nIDEvent) 
{
	RenderAmplitude();
	
	CPropertyPage::OnTimer(nIDEvent);
}

void CUtTrafficLightPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rr;

	BlockFill(&dc,&m_staticGreater,theApp.m_LastSettings.rgbAlarm[_GREATER]);
	BlockFill(&dc,&m_staticLessThan,theApp.m_LastSettings.rgbAlarm[_BELOW]);

}

void CUtTrafficLightPage::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);

}

void CUtTrafficLightPage::RenderAmplitude()
{
	if(GetSafeHwnd() == NULL) return;

	CDC* pDC = GetDC();
	CRect		rr,rect,rrStore;
	CDC			dcMem;
	CBitmap		bmp;
	CSize size;
	CString Buff;

	int nTimeSlot = MinMax(&theApp.m_LastSettings.nAlarmTimeSlot,0,7);
	GatesData* pGate = &theApp.m_UtUser.m_TS[nTimeSlot].Gate;
	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[nTimeSlot].Adc;
	DSP200Data* pDsp = &theApp.m_UtUser.m_TS[nTimeSlot].Dsp;
	
	m_staticAmplitude.GetWindowRect(&rr);
	ScreenToClient(&rr);
//	rr.DeflateRect(1,1);
	if(m_pArray==NULL) {
		SAFE_DELETE( m_pArray );
		m_pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
		memset(m_pArray,0xff,rr.Width()*sizeof(COLORREF)*rr.Height());
	}

	rect.left=0;
	rect.top=0;
	rect.right = rr.Width();
	rect.bottom = rr.Height();

	bmp.CreateCompatibleBitmap(pDC,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),m_pArray);
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	CFont* pOldFont = dcMem.SelectObject(m_pFont);
	dcMem.SetTextColor(theApp.m_LastSettings.rgbFaroText);
	dcMem.SetBkMode(TRANSPARENT);



	int nGate = MinMax(&theApp.m_LastSettings.nAlarmGate,0,7);
	int nAmplitude = pGate->nAmplitude[nGate];
	int nAboveOrBelow = _BELOW;

	switch(pAdc->nRfType) {
	case 0:
	case 1:
		m_strAmplitude.Format(L"%d%%",MulDiv(nAmplitude,100,128));
		if(MulDiv(nAmplitude,100,128) >= pGate->nThreshold[nGate]) nAboveOrBelow = _GREATER;
		break;
	case 2:
		m_strAmplitude.Format(L"%ddB",-theApp.m_LastSettings.nLogRange-MulDiv(nAmplitude,-theApp.m_LastSettings.nLogRange,127));
		if(MulDiv(nAmplitude,100,128) >= pGate->nThreshold[nGate]) nAboveOrBelow = _GREATER;
		break;
	}




	dcMem.FillRect(&rect,&CBrush(theApp.m_LastSettings.rgbAlarm[nAboveOrBelow]));


	size = dcMem.GetTextExtent(m_strAmplitude);
	dcMem.TextOut(rect.CenterPoint().x - size.cx/2, rect.CenterPoint().y - size.cy/2, m_strAmplitude);

	pDC->BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldFont);


	ReleaseDC(pDC);
}

void CUtTrafficLightPage::OnSize(UINT nType, int cx, int cy) 
{
	if (GetSafeHwnd() == NULL) return;
	CRect rr;
	CPropertyPage::OnSize(nType, cx, cy);

	m_staticAmplitude.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.right = cx-12;
	rr.bottom = cy-12;
	m_staticAmplitude.SetWindowPos(NULL,rr.left,rr.top,rr.Width(),rr.Height(),NULL);

}
