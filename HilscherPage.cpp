// HilscherPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "HilscherPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHilscherPage property page

IMPLEMENT_DYNCREATE(CHilscherPage, CPropertyPage)

CHilscherPage::CHilscherPage() : CPropertyPage(CHilscherPage::IDD)
{
	//{{AFX_DATA_INIT(CHilscherPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}

CHilscherPage::~CHilscherPage()
{
}

void CHilscherPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHilscherPage)
	DDX_Control(pDX, IDC_EDIT_SYNC_SHIFT1, m_editSyncShift1);
	DDX_Control(pDX, IDC_SPIN_SYNC_SHIFT1, m_spinSyncShift1);
	DDX_Control(pDX, IDC_EDIT_SYNC_SHIFT2, m_editSyncShift2);
	DDX_Control(pDX, IDC_SPIN_SYNC_SHIFT2, m_spinSyncShift2);
	DDX_Control(pDX, IDC_EDIT_SYNC_SHIFT3, m_editSyncShift3);
	DDX_Control(pDX, IDC_SPIN_SYNC_SHIFT3, m_spinSyncShift3);
	DDX_Control(pDX, IDC_EDIT_CALLBACK_PERIOD, m_editCallbackPeriod);
	DDX_Control(pDX, IDC_EDIT_CALLBACK_COUNT, m_editCallbackCount);
	DDX_Control(pDX, IDC_COMBO_DISPLAY_MODE, m_comboDisplayMode);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_MODE, m_comboMode);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_buttonConnect);
}


BEGIN_MESSAGE_MAP(CHilscherPage, CPropertyPage)
	//{{AFX_MSG_MAP(CHilscherPage)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY_MODE, OnSelchangeComboDisplayMode)
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_HILSCHER_CLEARALL, OnHilscherClearall)
	ON_EN_CHANGE(IDC_EDIT_SYNC_SHIFT1, OnChangeEditSyncShift1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SYNC_SHIFT1, OnDeltaposSpinSyncShift1)
	ON_EN_CHANGE(IDC_EDIT_SYNC_SHIFT2, OnChangeEditSyncShift2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SYNC_SHIFT2, OnDeltaposSpinSyncShift2)
	ON_EN_CHANGE(IDC_EDIT_SYNC_SHIFT3, OnChangeEditSyncShift3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SYNC_SHIFT3, OnDeltaposSpinSyncShift3)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, &CHilscherPage::OnCbnSelchangeComboMode)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CHilscherPage::OnBnClickedButtonConnect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHilscherPage message handlers

BOOL CHilscherPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	Buff.LoadString(IDS_Raw);		m_comboDisplayMode.AddString(Buff);
	Buff.LoadString(IDS_Decoded);	m_comboDisplayMode.AddString(Buff);

	m_spinSyncShift1.SetRange(0, 100);
	m_spinSyncShift2.SetRange(0, 100);
	m_spinSyncShift3.SetRange(0, 100);

	m_comboMode.AddString(L"Irq SI10");
	m_comboMode.AddString(L"Event 0 ms");
	m_comboMode.AddString(L"Event 1 ms");
	m_comboMode.AddString(L"Event 2 ms");
	m_comboMode.AddString(L"Event 5 ms");
	m_comboMode.AddString(L"Polled");

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CHilscherPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CHilscherPage::UpdateAllControls()
{
	CString Buff;

	theApp.m_LastSettings.nHilscherDisplayMask & 1 ? m_comboDisplayMode.SetCurSel(1) : m_comboDisplayMode.SetCurSel(0);

	Buff.Format(L"%.01f", theApp.m_Tank.fEthercatSyncShift[1]);
	m_editSyncShift1.SetWindowText(Buff);
	Buff.Format(L"%.01f", theApp.m_Tank.fEthercatSyncShift[2]);
	m_editSyncShift2.SetWindowText(Buff);
	Buff.Format(L"%.01f", theApp.m_Tank.fEthercatSyncShift[3]);
	m_editSyncShift3.SetWindowText(Buff);

	m_comboMode.SetCurSel(theApp.m_LastSettings.nEthercatCommunicationMode);

	if (theApp.m_Ethercat.IsFieldBusConnected() == true) {
		m_buttonConnect.SetWindowTextW(L"Disconnect");
	}
	else {
		m_buttonConnect.SetWindowTextW(L"Connect");
	}
}


BOOL CHilscherPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastPmacTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	SetTimer(1,200,NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CHilscherPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void CHilscherPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(GetSafeHwnd()) {
		RenderView();
		m_editCallbackCount.SetWindowText(theApp.m_Ethercat.getCallbackCount());
		m_editCallbackPeriod.SetWindowText(theApp.m_Ethercat.getCallbackPeriod());
	}

}

void CHilscherPage::OnTimer(UINT nIDEvent) 
{
	Invalidate(false);
	
	CPropertyPage::OnTimer(nIDEvent);
}

void CHilscherPage::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	CRect rr, rrClient, rrToolBar;

	if(GetSafeHwnd()) {
		GetClientRect(&rrClient);

		if (m_staticView.GetSafeHwnd()) {
			m_staticView.GetWindowRect(&rr);
			ScreenToClient(&rr);
			int nBorder = rr.top - rrClient.top;
			rr.right = rrClient.right - nBorder;
			rr.left = rrClient.CenterPoint().x;
			rr.bottom = rrClient.bottom - nBorder;
			m_staticView.SetWindowPos(NULL, rr.left, rr.top, rr.Width(), rr.Height(), NULL);
		}
	}
	
}

void CHilscherPage::OnSelchangeComboDisplayMode() 
{
	switch(m_comboDisplayMode.GetCurSel()) {
	case 0:	theApp.m_LastSettings.nHilscherDisplayMask &= 0xfffe;
		break;
	case 1:	theApp.m_LastSettings.nHilscherDisplayMask |= 0x0001;
		break;
	}
}


void CHilscherPage::RenderView()
{
	CDC  dcMem;
	CBitmap bmp;
	CSize size;
	int		yy, nAxis;
	static	bool bFlag = FALSE;
	CString Buff, Temp;
	CRect rr, rrView;


	CPaintDC* pDC = (CPaintDC*)m_staticView.GetDC();
	m_staticView.GetWindowRect(rrView);
	rrView -= rrView.TopLeft();

	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nBytesPixel = nBitsPixel / 8;
	if (nBytesPixel<0) nBytesPixel = 1;

	char	*pArray;
	pArray = new char[(rrView.Width() + 2) * (rrView.Height() + 2) * nBytesPixel];
	FillMemory(pArray, (rrView.Width() + 2) * (rrView.Height() + 2) * nBytesPixel, 0xff);

	dcMem.CreateCompatibleDC(pDC);

	dcMem.SetTextColor(RGB(255, 255, 0));
	dcMem.SetBkMode(TRANSPARENT);
	Buff = "-100.0%";
	size = dcMem.GetTextExtent(Buff);
	
	m_nDisplayRows = rrView.Height() / size.cy;

	bmp.CreateCompatibleBitmap(pDC, rrView.Width(), rrView.Height());
	bmp.SetBitmapBits((DWORD)(rrView.Width()*rrView.Height()*nBytesPixel), pArray);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	dcMem.FillRect(rrView, &CBrush(RGB(0, 0, 255)));


	yy=0;
	if(theApp.m_LastSettings.nHilscherDisplayMask & 0x0001) {
		
		dcMem.TextOut(4, yy, theApp.m_Ethercat.getStrDataFormat());
		yy += size.cy;
		dcMem.TextOut(4, yy, theApp.m_Ethercat.getLifeCount());
		yy += size.cy;

		for(int nCoord=0;nCoord<3;nCoord++) {
			dcMem.TextOut(4, yy, theApp.m_Ethercat.getCoordStatus(nCoord));
			yy += size.cy;
		}
		switch(theApp.m_Ethercat.getDataFormat(NULL)) {
		case FB_TWIN_TOWER:
			for(nAxis=0;nAxis<13;nAxis++) {
				dcMem.TextOut(4, yy, theApp.m_Ethercat.getEncoderPos(nAxis));
				yy += size.cy;
			}

			dcMem.TextOut(4, yy, theApp.m_Ethercat.getAirKnifeStatus());
			yy += size.cy;
			dcMem.TextOut(4, yy, theApp.m_Ethercat.getPumpStatus());
			yy += size.cy;
			dcMem.TextOut(4, yy, theApp.m_Ethercat.getToolClampStatusAndID(0));
			yy += size.cy;
			dcMem.TextOut(4, yy, theApp.m_Ethercat.getToolClampStatusAndID(1));
			yy += size.cy;
			dcMem.TextOut(4, yy, theApp.m_Ethercat.getTemperature(0));
			yy += size.cy;
			dcMem.TextOut(4, yy, theApp.m_Ethercat.getTemperature(1));
			yy += size.cy;
			dcMem.TextOut(4, yy, theApp.m_Ethercat.getRackDoorStatus());
			yy += size.cy;
			break;
		case DUAL_ROBOT:
			for(nAxis=0;nAxis<14;nAxis++) {
				dcMem.TextOut(4, yy, theApp.m_Ethercat.getEncoderPos(nAxis));
				dcMem.TextOut(210, yy, theApp.m_Ethercat.getStrAxisStatus(nAxis));
				yy += size.cy;
			}
			dcMem.TextOut(4, yy, theApp.m_Ethercat.getPumpStatus());
			yy += size.cy;
			dcMem.TextOut(4, yy, theApp.m_Ethercat.getAirKnifeStatus());
			yy += size.cy;
			break;
		case DUAL_ROBOT_9_PLUS_9:
		case SPX_ROBOT:
			for(nAxis=0;nAxis<21;nAxis++) {
				dcMem.TextOut(4, yy, theApp.m_Ethercat.getEncoderPos(nAxis));
				dcMem.TextOut(210, yy, theApp.m_Ethercat.getStrAxisStatus(nAxis));
				yy += size.cy;
			}
			dcMem.TextOut(4, yy, theApp.m_Ethercat.getPumpStatus());
			yy += size.cy;
			dcMem.TextOut(4, yy, theApp.m_Ethercat.getAirKnifeStatus());
			yy += size.cy;
			dcMem.TextOut(4, yy, theApp.m_Ethercat.getEStopStatus());
			yy += size.cy;
			break;
		case RAILWAY_AXLE:
			for (nAxis = 0; nAxis<7; nAxis++) {
				dcMem.TextOut(4, yy, theApp.m_Ethercat.getEncoderPos(nAxis));
				dcMem.TextOut(210, yy, theApp.m_Ethercat.getStrAxisStatus(nAxis));
				yy += size.cy;
			}
			break;
		case TANK_5AXIS:
			for (nAxis = 0; nAxis<5; nAxis++) {
				dcMem.TextOut(4, yy, theApp.m_Ethercat.getEncoderPos(nAxis));
				dcMem.TextOut(210, yy, theApp.m_Ethercat.getStrAxisStatus(nAxis));
				yy += size.cy;
			}
			break;
		case QUICKSTEP_FLATBED:
			for (nAxis = 0; nAxis<2; nAxis++) {
				dcMem.TextOut(4, yy, theApp.m_Ethercat.getEncoderPos(nAxis));
				dcMem.TextOut(210, yy, theApp.m_Ethercat.getStrAxisStatus(nAxis));
				yy += size.cy;
			}
			break;
		case TANK_6AXIS:
			for (nAxis = 0; nAxis < 6; nAxis++) {
				dcMem.TextOut(4, yy, theApp.m_Ethercat.getEncoderPos(nAxis));
				dcMem.TextOut(210, yy, theApp.m_Ethercat.getStrAxisStatus(nAxis));
				yy += size.cy;
			}
			break;
		case TWIN_TOWER_KINEMATIC:
			for (nAxis = 0; nAxis < 10; nAxis++) {
				dcMem.TextOut(4, yy, theApp.m_Ethercat.getEncoderPos(nAxis));
				dcMem.TextOut(210, yy, theApp.m_Ethercat.getStrAxisStatus(nAxis));
				yy += size.cy;
			}
			dcMem.TextOut(4, yy, theApp.m_Ethercat.getPumpStatus());
			yy += size.cy;
			dcMem.TextOut(4, yy, theApp.m_Ethercat.getAirKnifeStatus());
			yy += size.cy;
			dcMem.TextOut(4, yy, theApp.m_Ethercat.getEStopStatus());
			yy += size.cy;
			break;
		}


	} else {

		for(int nRow=0, nPtr=0 ;nRow<25;nRow++) {
			Buff.Empty();
			for(int jj=0;jj<4;jj++) {
				Temp.Format(L"%02x ",theApp.m_Ethercat.m_RxArray[nPtr++]);
				Buff += Temp;
			}
			dcMem.TextOut(4, yy, Buff);
			yy += size.cy;
		}
	}


	pDC->BitBlt(rrView.left, rrView.top, rrView.Width(), rrView.Height(), &dcMem, 0, 0, SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	dcMem.SetBkMode(OPAQUE);

	delete pArray;

	m_staticView.ReleaseDC(pDC);

}


void CHilscherPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;
	int nMenu = IDR_RB_HILSCHER_MENU;
	CRect rr;

	GetCursorPos(&Point);

	if (menu->LoadMenu(nMenu)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);
	}
	delete menu;
	
}

void CHilscherPage::OnHilscherClearall() 
{
	
}

void CHilscherPage::OnCbnSelchangeComboMode()
{
	theApp.m_LastSettings.nEthercatCommunicationMode = m_comboMode.GetCurSel();
}

void CHilscherPage::OnChangeEditSyncShift1()
{
	CString Buff;

	m_editSyncShift1.GetWindowText(Buff);
	_WTOF(Buff, theApp.m_Tank.fEthercatSyncShift[AmplifierType::Kollmorgen]);
}

void CHilscherPage::OnDeltaposSpinSyncShift1(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	theApp.m_Tank.fEthercatSyncShift[AmplifierType::Kollmorgen] += ((float)pNMUpDown->iDelta * 0.1f);
	UpdateAllControls();

	*pResult = 0;
}

void CHilscherPage::OnChangeEditSyncShift2()
{
	CString Buff;

	m_editSyncShift2.GetWindowText(Buff);
	_WTOF(Buff, theApp.m_Tank.fEthercatSyncShift[AmplifierType::Copley]);
}

void CHilscherPage::OnDeltaposSpinSyncShift2(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	theApp.m_Tank.fEthercatSyncShift[AmplifierType::Copley] += ((float)pNMUpDown->iDelta * 0.1f);
	UpdateAllControls();

	*pResult = 0;
}

void CHilscherPage::OnChangeEditSyncShift3()
{
	CString Buff;

	m_editSyncShift3.GetWindowText(Buff);
	_WTOF(Buff, theApp.m_Tank.fEthercatSyncShift[AmplifierType::Staubli]);
}

void CHilscherPage::OnDeltaposSpinSyncShift3(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	theApp.m_Tank.fEthercatSyncShift[AmplifierType::Staubli] += ((float)pNMUpDown->iDelta * 0.1f);
	UpdateAllControls();

	*pResult = 0;
}


void CHilscherPage::OnBnClickedButtonConnect()
{
	if (theApp.m_Ethercat.IsFieldBusConnected() == false) {
		theApp.m_Ethercat.StartFieldBus();
	}
	else {
		theApp.m_Ethercat.Close();
		m_buttonConnect.SetWindowTextW(L"Connect");
	}
}
