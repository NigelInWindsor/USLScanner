// ColoursEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ColoursEditDlg.h"
#include "3DViewEditDlg.h"
#include "3DViewPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColoursEditDlg dialog
IMPLEMENT_DYNCREATE(CColoursEditDlg, CPropertyPage)

CColoursEditDlg::CColoursEditDlg(CPropertySheet* pToolSheet, CPropertyPage* pViewPage) 
	: CPropertyPage(CColoursEditDlg::IDD)
{
	//{{AFX_DATA_INIT(CColoursEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pViewPage = pViewPage;
	m_pToolSheet = pToolSheet;
}


void CColoursEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColoursEditDlg)
	DDX_Control(pDX, IDC_STATIC_COLOR_OUTSIDE, m_staticOutsideColor);
	DDX_Control(pDX, IDC_STATIC_COLOR_INSIDE, m_staticInsideColor);
	DDX_Control(pDX, IDC_STATIC_COLOR_IMAGE_VERTICES, m_staticVerticesColor);
	DDX_Control(pDX, IDC_STATIC_COLOR_ROOM_WALLS, m_staticRoomWallColor);
	DDX_Control(pDX, IDC_STATIC_COLOR_TAUGHT, m_staticTaughtColor);
	DDX_Control(pDX, IDC_STATIC_COLOR_SCAN, m_staticScanColor);
	DDX_Control(pDX, IDC_STATIC_COLOR_ENVELOPE, m_staticEnvelopeColor);
	DDX_Control(pDX, IDC_STATIC_COLOR_BRAIN, m_staticBrainColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColoursEditDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CColoursEditDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColoursEditDlg message handlers

BOOL CColoursEditDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColoursEditDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rr;

	m_staticTaughtColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	dc.FillRect(rr,&CBrush(theApp.m_LastSettings.rgbDisplayTaughtColor));

	m_staticScanColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	dc.FillRect(rr,&CBrush(theApp.m_LastSettings.rgbDisplayScanColor));

	m_staticBrainColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	dc.FillRect(rr,&CBrush(theApp.m_LastSettings.rgbDisplayBrainColor));

	m_staticEnvelopeColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	dc.FillRect(rr,&CBrush(theApp.m_LastSettings.rgbDisplayEnvelopeColor));

	m_staticRoomWallColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	dc.FillRect(rr,&CBrush(theApp.m_LastSettings.rgbRoomWallColor));

	m_staticVerticesColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	dc.FillRect(rr,&CBrush(theApp.m_LastSettings.rgbVerticesColor));

	m_staticOutsideColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	dc.FillRect(rr,&CBrush(theApp.m_LastSettings.rgbBladeColor[OUTSIDE]));

	m_staticInsideColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	dc.FillRect(rr,&CBrush(theApp.m_LastSettings.rgbBladeColor[INSIDE]));
}

void CColoursEditDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CColorDialog dlg;

	m_staticTaughtColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbDisplayTaughtColor=dlg.GetColor();
			Invalidate(FALSE);
		}
	}
	m_staticScanColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbDisplayScanColor=dlg.GetColor();
			Invalidate(FALSE);
		}
	}
	m_staticBrainColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbDisplayBrainColor=dlg.GetColor();
			Invalidate(FALSE);
		}
	}
	m_staticEnvelopeColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbDisplayEnvelopeColor=dlg.GetColor();
			Invalidate(FALSE);
		}
	}
	m_staticRoomWallColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbRoomWallColor=dlg.GetColor();
			Invalidate(FALSE);
			RenderParent();
		}
	}
	m_staticVerticesColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbVerticesColor=dlg.GetColor();
			Invalidate(FALSE);
			RenderParent();
		}
	}
	m_staticOutsideColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbBladeColor[OUTSIDE]=dlg.GetColor();
			Invalidate(FALSE);
			RenderParent();
		}
	}
	m_staticInsideColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbBladeColor[INSIDE]=dlg.GetColor();
			Invalidate(FALSE);
			RenderParent();
		}
	}
	
	CPropertyPage::OnLButtonDown(nFlags, point);
}

BOOL CColoursEditDlg::OnSetActive() 
{
	theApp.m_LastSettings.nLast3DDrawingToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

void CColoursEditDlg::RenderParent()
{
	C3DViewPage *pDlg = (C3DViewPage*)m_pViewPage;

	pDlg->InitializeRoom();
	pDlg->Invalidate(false);
}
