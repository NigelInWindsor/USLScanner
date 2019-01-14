// ExtTrigScan.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ExtTrigScan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtTrigScan property page

IMPLEMENT_DYNCREATE(CExtTrigScan, CPropertyPage)

CExtTrigScan::CExtTrigScan() : CPropertyPage(CExtTrigScan::IDD)
{
	//{{AFX_DATA_INIT(CExtTrigScan)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pArrayView = NULL;
}

CExtTrigScan::~CExtTrigScan()
{
	SAFE_DELETE( m_pArrayView );
}

void CExtTrigScan::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtTrigScan)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_COMBO_TRIG_SOURCE, m_comboTrigSource);
	DDX_Control(pDX, IDC_USLBARCTRL, m_scrollbarDelayWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtTrigScan, CPropertyPage)
	//{{AFX_MSG_MAP(CExtTrigScan)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIG_SOURCE, OnSelchangeComboTrigSource)
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtTrigScan message handlers

BOOL CExtTrigScan::OnInitDialog() 
{
	CString Buff;
	CPropertyPage::OnInitDialog();
	
	Buff.LoadString(IDS_PRF);			m_comboTrigSource.AddString(Buff);
	Buff.LoadString(IDS_Internal_Clk);	m_comboTrigSource.AddString(Buff);
	Buff.LoadString(IDS_Input_0);		m_comboTrigSource.AddString(Buff);
	Buff.LoadString(IDS_Input_1);		m_comboTrigSource.AddString(Buff);
	Buff.LoadString(IDS_Input_2);		m_comboTrigSource.AddString(Buff);
	Buff.LoadString(IDS_Input_3);		m_comboTrigSource.AddString(Buff);
	Buff.LoadString(IDS_Input_4);		m_comboTrigSource.AddString(Buff);
	Buff.LoadString(IDS_Input_5);		m_comboTrigSource.AddString(Buff);
	Buff.LoadString(IDS_Input_6);		m_comboTrigSource.AddString(Buff);
	Buff.LoadString(IDS_Input_7);		m_comboTrigSource.AddString(Buff);

	m_scrollbarDelayWidth.SetMax(200);	//20 seconds
	m_scrollbarDelayWidth.SetMin(0);
	m_scrollbarDelayWidth.SetHorizontal(TRUE);
	m_scrollbarDelayWidth.SetWidthHighCursor(15);
	m_scrollbarDelayWidth.SetWidthLowCursor(15);
	m_scrollbarDelayWidth.SetMinimumScreenWidth(1);
	m_scrollbarDelayWidth.SetLowPosition(0);
	m_scrollbarDelayWidth.SetHighPosition(200);

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CExtTrigScan::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

BOOL CExtTrigScan::OnSetActive() 
{
	
	UpdateAllControls();

	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	SetTimer(1,100,NULL);

	return CPropertyPage::OnSetActive();
}

void CExtTrigScan::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	m_comboTrigSource.SetCurSel( theApp.m_LastSettings.nExtDataCollectionStrobeSrc );
}

void CExtTrigScan::OnSelchangeComboTrigSource() 
{
	theApp.m_LastSettings.nExtDataCollectionStrobeSrc = m_comboTrigSource.GetCurSel();
}

void CExtTrigScan::OnTimer(UINT nIDEvent) 
{

	Invalidate(FALSE);
	CPropertyPage::OnTimer(nIDEvent);
}

void CExtTrigScan::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rrView;

	m_staticView.GetWindowRect(&rrView);
	ScreenToClient(&rrView);
	rrView.DeflateRect(1,1);

	InitializeViewVariables(&dc,&rrView);

	PresentRenderdView(&dc);
	
	// Do not call CPropertyPage::OnPaint() for painting messages
}

void CExtTrigScan::InitializeViewVariables(CPaintDC *pDC, CRect *prr)
{
	if((prr->Width() - m_rrView.Width()) || (prr->Height() - m_rrView.Height())) {
		m_rrView = prr;

		int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
		m_nBytesPixel = nBitsPixel/8;

		SAFE_DELETE( m_pArrayView );
		m_pArrayView = new char[(m_rrView.Width()+2) * (m_rrView.Height()+2) * m_nBytesPixel];

	}

}


void CExtTrigScan::PresentRenderdView(CPaintDC *pDC)
{
	CDC  dcMem;
	CBitmap bmp;
	CRect rr;
	int nX;

	bmp.CreateCompatibleBitmap(pDC,m_rrView.Width(),m_rrView.Height());
	bmp.SetBitmapBits((DWORD)(m_rrView.Width()*m_rrView.Height()*m_nBytesPixel),m_pArrayView);

	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	rr = m_rrView;
	rr.OffsetRect(-rr.left,-rr.top);
	dcMem.FillRect(rr,&CBrush(RGB(255,255,255)));

	if(theApp.m_LastSettings.dExtDisplayPeriod < 0.01) theApp.m_LastSettings.dExtDisplayPeriod = 0.01;
	double dCurrentTime = theApp.m_Thread.m_HRTimer.GetTime();

	unsigned int nPtr = (theApp.m_Thread.m_nExtPtr-1) % EXT_SIZE;
	dcMem.MoveTo(rr.right,rr.top+10);
	while((theApp.m_Thread.m_ExternalEvents[nPtr].dHRTime != 0.0) && ((dCurrentTime - theApp.m_Thread.m_ExternalEvents[nPtr].dHRTime) < theApp.m_LastSettings.dExtDisplayPeriod)) {

		nX = rr.right - (int)((double)rr.Width() * (dCurrentTime - theApp.m_Thread.m_ExternalEvents[nPtr].dHRTime) / theApp.m_LastSettings.dExtDisplayPeriod);

		if(theApp.m_Thread.m_ExternalEvents[nPtr].nStatus & INTERNAL_PRF) {
			dcMem.LineTo(nX,rr.top+10);
			dcMem.LineTo(nX,rr.top+5);
			dcMem.MoveTo(nX,rr.top+10);
		}
		nPtr--;
		nPtr %= EXT_SIZE;
	}





	pDC->BitBlt(m_rrView.left,m_rrView.top,m_rrView.Width(),m_rrView.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);


}

BEGIN_EVENTSINK_MAP(CExtTrigScan, CPropertyPage)
    //{{AFX_EVENTSINK_MAP(CExtTrigScan)
	ON_EVENT(CExtTrigScan, IDC_USLBARCTRL, 1 /* ScrollHasMoved */, OnScrollHasMovedUslbarctrl, VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CExtTrigScan::OnScrollHasMovedUslbarctrl(long LowPosition, long HighPosition, long Width) 
{
	theApp.m_LastSettings.dExtDisplayPeriod = (double)(m_scrollbarDelayWidth.GetHighPosition() - m_scrollbarDelayWidth.GetLowPosition()) / 10.0;
	if(theApp.m_LastSettings.dExtDisplayPeriod < 0.01) theApp.m_LastSettings.dExtDisplayPeriod = 0.01;
	
}
