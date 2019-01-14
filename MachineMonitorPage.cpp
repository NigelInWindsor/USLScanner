// MachineMonitorPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "MachineMonitorPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMachineMonitorPage property page

#define	_BLACK RGB(0,0,0)
#define	_WHITE RGB(255,255,255)
#define	_RED RGB(255,0,0)
#define	_GREEN RGB(0,255,0)
#define	_BLUE RGB(0,0,255)
#define	_YELLOW RGB(255,255,0)


IMPLEMENT_DYNCREATE(CMachineMonitorPage, CPropertyPage)

CMachineMonitorPage::CMachineMonitorPage() : CPropertyPage(CMachineMonitorPage::IDD)
{
	//{{AFX_DATA_INIT(CMachineMonitorPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

CMachineMonitorPage::~CMachineMonitorPage()
{
}

void CMachineMonitorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMachineMonitorPage)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMachineMonitorPage, CPropertyPage)
	//{{AFX_MSG_MAP(CMachineMonitorPage)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMachineMonitorPage message handlers

BOOL CMachineMonitorPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMachineMonitorPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;


}


BOOL CMachineMonitorPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastMaintenanceTab = ((CPropertySheet*)GetParent())->GetActiveIndex();

	m_nEntries = 0;
	m_nTopRow = 0;
	ZeroMemory(m_rgbText, sizeof m_rgbText);
	ZeroMemory(m_rgbBk, sizeof m_rgbBk);
	m_strArray.RemoveAll();

	CStringArray m_strArray;

	SetTimer(1,250,NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CMachineMonitorPage::OnKillActive() 
{

	KillTimer(1);
	return CPropertyPage::OnKillActive();
}

void CMachineMonitorPage::OnTimer(UINT nIDEvent) 
{
	switch(theApp.m_Tank.nScannerDescription) {
	case FB_TWIN_TOWER:	RenderHengshen();
		break;
	default:
		break;
	}
	
	CPropertyPage::OnTimer(nIDEvent);
}

void CMachineMonitorPage::FillNextEntry(CString Buff, COLORREF rgbText, COLORREF rgbBk)
{
	m_strArray.Add(Buff);
	m_nEntries = m_strArray.GetSize();
	int nn = m_nEntries - 1;
	m_rgbText[nn] = rgbText;
	m_rgbBk[nn] = rgbBk;
}

void CMachineMonitorPage::RenderHengshen()
{
	CString Buff,Label;
	int ii;
	CString strOkay, strEmpty, strTemp;

	strOkay.LoadString(IDS_Ok);
	strEmpty.LoadString(IDS_Empty);

	m_strArray.RemoveAll();

	Label.LoadString(IDS_Cabinet_temperature);	 Buff.Format(L"%s 1: %d%sC", Label, theApp.m_FBCtrl.RequestTemperature(0), DEGREES);
	FillNextEntry(Buff,_BLACK,_WHITE);
	Label.LoadString(IDS_Cabinet_temperature);	 Buff.Format(L"%s 2: %d%sC", Label, theApp.m_FBCtrl.RequestTemperature(1), DEGREES);
	FillNextEntry(Buff,_BLACK,_WHITE);
	Label.LoadString(IDS_Cabinet_temperature);	 Buff.Format(L"%s 3: %d%sC", Label, theApp.m_FBCtrl.RequestTemperature(2), DEGREES);
	FillNextEntry(Buff,_BLACK,_WHITE);

	for(ii=0;ii<6;ii++) {
		Label.LoadString(IDS_Oiler);
		switch(theApp.m_FBCtrl.RequestOiler(ii)) {
		case 0: Buff.Format(L"%s %d: %s", Label, ii + 1, strOkay);
			FillNextEntry(Buff,_BLACK,_GREEN);
			break;
		default: Buff.Format(L"%s %d: %s", Label, ii + 1, strEmpty);
			FillNextEntry(Buff,_RED,_WHITE);
			break;
		}
	}
	Label.LoadString(IDS_Water_level_in_pit);
	if(theApp.m_FBCtrl.RequestEthercatGeneralInput() & 2) {
		strTemp.LoadString(IDS_Flooded);
		Buff.Format(L"%s: %s", Label, strTemp);
		FillNextEntry(Buff,_BLACK,_RED);
	} else {
		strTemp.LoadString(IDS_Not_flooded);
		Buff.Format(L"%s: %s", Label, strTemp);
		FillNextEntry(Buff,_BLACK,_WHITE);
	}

	Label.LoadString(IDS_Water_level_in_bowser);
	if(theApp.m_FBCtrl.RequestEthercatGeneralInput() & 1) {
		strTemp.LoadString(IDS_Acceptable);
		Buff.Format(L"%s: %s", Label, strTemp);
		FillNextEntry(Buff,_BLACK,_WHITE);
	} else {
		strTemp.LoadString(IDS_Low);
		Buff.Format(L"%s: %s", Label, strTemp);
		FillNextEntry(Buff,_BLACK,_YELLOW);
	}


	Invalidate(false);

}

void CMachineMonitorPage::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	CRect rrClient,rr;

	if(GetSafeHwnd()) {
		GetClientRect(&rrClient);
		rr = rrClient;

		if(m_staticView.GetSafeHwnd())
			m_staticView.SetWindowPos(NULL,rr.left,rr.top, rr.Width(),rr.Height(), NULL);

	}

}

void CMachineMonitorPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC  dcMem;
	CBitmap bmp;
	CRect rrStatic;
	CString Buff;
	CFont Font;
	CRect rrBK;

	LOGFONT	lf = theApp.m_LastSettings.lfViewGraticule;
	lf.lfEscapement=0;
	lf.lfHeight = -20;
	Font.CreateFontIndirect(&lf);

	m_staticView.GetWindowRect(&rrStatic);
	ScreenToClient(&rrStatic);
	rrStatic.DeflateRect(1,1);
	COLORREF	*pArray = new COLORREF[(rrStatic.Width()+2) * (rrStatic.Height()+2)];

	bmp.CreateCompatibleBitmap(&dc,rrStatic.Width(),rrStatic.Height());
	bmp.SetBitmapBits((DWORD)(rrStatic.Width()*rrStatic.Height()*4),pArray);
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	
	CRect rr = rrStatic;
	rr.OffsetRect(-rrStatic.left,-rrStatic.top);
	dcMem.FillRect(&rr,&CBrush(RGB(255,255,255)));
	CFont* pOldFont = dcMem.SelectObject(&Font);

	Buff = "Hello";
	CSize m_size = dcMem.GetTextExtent(Buff);
	int nRowL = rr.Height() / m_size.cy;

	dcMem.SetBkMode(TRANSPARENT);
	for(int nRow=0;nRow<nRowL && nRow<m_nEntries;nRow++) {
		rrBK = rr;
		rrBK.top = nRow*m_size.cy;
		rrBK.bottom = rrBK.top + m_size.cy;

		dcMem.FillRect(&rrBK,&CBrush(m_rgbBk[nRow + m_nTopRow]));
		dcMem.SetTextColor(m_rgbText[nRow + m_nTopRow]);

		if((nRow + m_nTopRow) < m_nEntries) {
			dcMem.TextOut(1,nRow*m_size.cy,m_strArray.GetAt(nRow + m_nTopRow));
		}
	}

	dc.BitBlt(rrStatic.left,rrStatic.top,rrStatic.Width(),rrStatic.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldFont);
	dcMem.SelectObject(pOldBitmap);

	delete pArray;
}

