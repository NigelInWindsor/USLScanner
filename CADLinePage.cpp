// CADLinePage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "CADLinePage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ViewSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CCADLinePage property page

IMPLEMENT_DYNCREATE(CCADLinePage, CPropertyPage)

CCADLinePage::CCADLinePage(CWnd* pViewSheet) : CPropertyPage(CCADLinePage::IDD)
{
	//{{AFX_DATA_INIT(CCADLinePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pViewSheet = (CViewSheet*)pViewSheet;
}

CCADLinePage::~CCADLinePage()
{
}

void CCADLinePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCADLinePage)
	DDX_Control(pDX, IDC_CHECK_LENGTH, m_checkLength);
	DDX_Control(pDX, IDC_STATIC_FONT_SIZE, m_staticFontSize);
	DDX_Control(pDX, IDC_STATIC_FONT_NAME, m_staticFontName);
	DDX_Control(pDX, IDC_STATIC_COLOR, m_staticColor);
	DDX_Control(pDX, IDC_SPIN_THICKNESS, m_spinWidth);
	DDX_Control(pDX, IDC_EDIT_THICKNESS, m_editWidth);
	DDX_Control(pDX, IDC_CHECK_SHOW, m_checkShowLine);
	DDX_Control(pDX, IDC_CHECK_NODE1_ARROW_OUTSIDE, m_checkNode1ArrowOutside);
	DDX_Control(pDX, IDC_CHECK_NODE0_ARROW_OUTSIDE, m_checkNode0ArrowOutside);
	DDX_Control(pDX, IDC_CHECK_NODE1_ARROW, m_checkNode1ArrowInside);
	DDX_Control(pDX, IDC_CHECK_NODE0_ARROW, m_checkNode0ArrowInside);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCADLinePage, CPropertyPage)
	//{{AFX_MSG_MAP(CCADLinePage)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_CHECK_NODE0_ARROW, OnCheckNode0Arrow)
	ON_BN_CLICKED(IDC_CHECK_NODE1_ARROW, OnCheckNode1Arrow)
	ON_BN_CLICKED(IDC_CHECK_NODE0_ARROW_OUTSIDE, OnCheckNode0ArrowOutside)
	ON_BN_CLICKED(IDC_CHECK_NODE1_ARROW_OUTSIDE, OnCheckNode1ArrowOutside)
	ON_BN_CLICKED(IDC_CHECK_SHOW, OnCheckShow)
	ON_EN_CHANGE(IDC_EDIT_THICKNESS, OnChangeEditThickness)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_LENGTH, OnCheckLength)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCADLinePage message handlers

BOOL CCADLinePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	

	m_spinWidth.SetRange(1,20);

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCADLinePage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	CString Buff;

	m_checkNode1ArrowOutside.SetCheck(theApp.m_LastSettings.nCADFlags & CAD_NODE1_ARROW_O);
	m_checkNode0ArrowOutside.SetCheck(theApp.m_LastSettings.nCADFlags & CAD_NODE0_ARROW_O);
	m_checkNode1ArrowInside.SetCheck(theApp.m_LastSettings.nCADFlags & CAD_NODE1_ARROW_I);
	m_checkNode0ArrowInside.SetCheck(theApp.m_LastSettings.nCADFlags & CAD_NODE0_ARROW_I);
	m_checkShowLine.SetCheck(theApp.m_LastSettings.nCADFlags & CAD_SHOW_LINE);
	m_checkLength.SetCheck(theApp.m_LastSettings.nCADFlags & CAD_SHOW_LENGTH);

	Buff.Format(_T("%d"),theApp.m_LastSettings.nCADLineWidth);
	m_editWidth.SetWindowText(Buff);

	m_spinWidth.SetPos(theApp.m_LastSettings.nCADLineWidth);

	BlockFill();
}

void CCADLinePage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	int	nTemp = theApp.m_LastSettings.nCADLineWidth;
	
	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_THICKNESS:
		theApp.m_LastSettings.nCADLineWidth=pSpin->GetPos()&0xffff;
		if(nTemp - theApp.m_LastSettings.nCADLineWidth) {
			m_spinWidth.SetPos(theApp.m_LastSettings.nCADLineWidth);
			UpdateAllControls();
			Invalidate();
		}
		break;
	}

	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CCADLinePage::OnCheckNode0Arrow() 
{
	m_checkNode0ArrowInside.GetCheck() ? theApp.m_LastSettings.nCADFlags |= CAD_NODE0_ARROW_I : theApp.m_LastSettings.nCADFlags &= ~CAD_NODE0_ARROW_I;
	Invalidate();
}

void CCADLinePage::OnCheckNode1Arrow() 
{
	m_checkNode1ArrowInside.GetCheck() ? theApp.m_LastSettings.nCADFlags |= CAD_NODE1_ARROW_I : theApp.m_LastSettings.nCADFlags &= ~CAD_NODE1_ARROW_I;
	Invalidate();
}

void CCADLinePage::OnCheckNode0ArrowOutside() 
{
	m_checkNode0ArrowOutside.GetCheck() ? theApp.m_LastSettings.nCADFlags |= CAD_NODE0_ARROW_O : theApp.m_LastSettings.nCADFlags &= ~CAD_NODE0_ARROW_O;
	Invalidate();
}

void CCADLinePage::OnCheckNode1ArrowOutside() 
{
	m_checkNode1ArrowOutside.GetCheck() ? theApp.m_LastSettings.nCADFlags |= CAD_NODE1_ARROW_O : theApp.m_LastSettings.nCADFlags &= ~CAD_NODE1_ARROW_O;
	Invalidate();
}

void CCADLinePage::OnCheckShow() 
{
	m_checkShowLine.GetCheck() ? theApp.m_LastSettings.nCADFlags |= CAD_SHOW_LINE : theApp.m_LastSettings.nCADFlags &= ~CAD_SHOW_LINE;
	Invalidate();
}

void CCADLinePage::OnCheckLength() 
{
	m_checkLength.GetCheck() ? theApp.m_LastSettings.nCADFlags |= CAD_SHOW_LENGTH : theApp.m_LastSettings.nCADFlags &= ~CAD_SHOW_LENGTH;
	Invalidate();
}

void CCADLinePage::OnChangeEditThickness() 
{
	CString Buff;
	int	nTemp = theApp.m_LastSettings.nCADLineWidth;

	m_editWidth.GetWindowText(Buff);
	theApp.m_LastSettings.nCADLineWidth = _ttoi(Buff);
	if(theApp.m_LastSettings.nCADLineWidth<1) theApp.m_LastSettings.nCADLineWidth = 1;
	if(theApp.m_LastSettings.nCADLineWidth>20) theApp.m_LastSettings.nCADLineWidth = 20;

	if(nTemp - theApp.m_LastSettings.nCADLineWidth) {
		m_spinWidth.SetPos(theApp.m_LastSettings.nCADLineWidth);
		Invalidate();
	}
}

void CCADLinePage::Invalidate()
{
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;

	if(pSheet->m_Data.m_nCADElementL>0) {
		pSheet->m_Data.m_pCADElement[pSheet->m_Data.m_nCADPtr].SetToLastSettings();
		pSheet->InvalidateChild();
	}
}

void CCADLinePage::OnButtonFont() 
{
	CString Buff;

	CFontDialog dlg(&theApp.m_LastSettings.lfCAD);
	dlg.m_cf.rgbColors = theApp.m_LastSettings.rgbCADText;
	if(dlg.DoModal() == IDOK) {
		dlg.GetCurrentFont(&theApp.m_LastSettings.lfCAD);
		theApp.m_LastSettings.rgbCADText = dlg.GetColor();
		Invalidate();
	}
}

void CCADLinePage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CMFCColorDialog dlg;

	m_staticColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbCADColor=dlg.GetColor();
			BlockFill();
			Invalidate();
		}
	}
	
	CPropertyPage::OnLButtonDown(nFlags, point);
}



void CCADLinePage::BlockFill()
{
	CRect rr;
	CDC* pDC = GetDC();

	m_staticColor.GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(theApp.m_LastSettings.rgbCADColor);

	pDC->FillRect(rr,&Brush);

	ReleaseDC(pDC);
}

void CCADLinePage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	

	BlockFill();
	// Do not call CPropertyPage::OnPaint() for painting messages
}


