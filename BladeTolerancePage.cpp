// BladeTolerancePage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "BladeTolerancePage.h"
#include "ViewBladeThickneessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBladeTolerancePage property page

IMPLEMENT_DYNCREATE(CBladeTolerancePage, CPropertyPage)

CBladeTolerancePage::CBladeTolerancePage(CWnd* pParent) : CPropertyPage(CBladeTolerancePage::IDD)
{
	//{{AFX_DATA_INIT(CBladeTolerancePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	
}

CBladeTolerancePage::~CBladeTolerancePage()
{
}

void CBladeTolerancePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBladeTolerancePage)
	DDX_Control(pDX, IDC_CHECK_AUTO_VERTICAL_SCALE, m_checkAutoVerticalScale);
	DDX_Control(pDX, IDC_STATIC_FONT_COLOR, m_staticFontColor);
	DDX_Control(pDX, IDC_STATIC_BK_COLOR, m_staticBkColor);
	DDX_Control(pDX, IDC_SPIN_FONT_SIZE, m_spinFontSize);
	DDX_Control(pDX, IDC_EDIT_FONT_SIZE, m_editFontSize);
	DDX_Control(pDX, IDC_STATIC_NOM, m_staticNom);
	DDX_Control(pDX, IDC_STATIC_ACT, m_staticAct);
	DDX_Control(pDX, IDC_CHECK_DISPLAY_NOM, m_checkDisplayNom);
	DDX_Control(pDX, IDC_STATIC_MIN_TOL, m_staticMin);
	DDX_Control(pDX, IDC_STATIC_MAX_TOL, m_staticMax);
	DDX_Control(pDX, IDC_SPIN_MIN_TOL, m_spinMin);
	DDX_Control(pDX, IDC_SPIN_MAX_TOL, m_spinMax);
	DDX_Control(pDX, IDC_EDIT_MIN_TOL, m_editMin);
	DDX_Control(pDX, IDC_EDIT_MAX_TOL, m_editMax);
	DDX_Control(pDX, IDC_CHECK_DISPLAY_MIN, m_checkDisplayMin);
	DDX_Control(pDX, IDC_CHECK_DISPLAY_MAX, m_checkDisplayMax);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBladeTolerancePage, CPropertyPage)
	//{{AFX_MSG_MAP(CBladeTolerancePage)
	ON_EN_CHANGE(IDC_EDIT_MAX_TOL, OnChangeEditMaxTol)
	ON_EN_CHANGE(IDC_EDIT_MIN_TOL, OnChangeEditMinTol)
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_DISPLAY_MIN, OnCheckDisplayMin)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY_MAX, OnCheckDisplayMax)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY_NOM, OnCheckDisplayNom)
	ON_EN_CHANGE(IDC_EDIT_FONT_SIZE, OnChangeEditFontSize)
	ON_BN_CLICKED(IDC_CHECK_AUTO_VERTICAL_SCALE, OnCheckAutoVerticalScale)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBladeTolerancePage message handlers

BOOL CBladeTolerancePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	

	m_spinMin.SetRange(0,32000);
	m_spinMax.SetRange(0,32000);
	m_spinFontSize.SetRange(1,30);

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBladeTolerancePage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	Buff.Format(_T("%.03f"),theApp.m_LastSettings.fBladeMaxTolerance);
	m_editMax.SetWindowText(Buff);
	Buff.Format(_T("%.03f"),theApp.m_LastSettings.fBladeMinTolerance);
	m_editMin.SetWindowText(Buff);
	Buff.Format(_T("%d"),theApp.m_LastSettings.nBladeFontSize);
	m_editFontSize.SetWindowText(Buff);

	m_spinMin.SetPos((int)(theApp.m_LastSettings.fBladeMinTolerance * 1000.0f));
	m_spinMax.SetPos((int)(theApp.m_LastSettings.fBladeMaxTolerance * 1000.0f));
	m_spinFontSize.SetPos(theApp.m_LastSettings.nBladeFontSize);

	theApp.m_LastSettings.nBladeDisplayMask & DISPLAY_MIN_TOLERANCE ? m_checkDisplayMin.SetCheck(true) : m_checkDisplayMin.SetCheck(false);
	theApp.m_LastSettings.nBladeDisplayMask & DISPLAY_MAX_TOLERANCE ? m_checkDisplayMax.SetCheck(true) : m_checkDisplayMax.SetCheck(false);
	theApp.m_LastSettings.nBladeDisplayMask & DISPLAY_NOM_THICKNESS ? m_checkDisplayNom.SetCheck(true) : m_checkDisplayNom.SetCheck(false);

	m_checkAutoVerticalScale.SetCheck(theApp.m_LastSettings.nBladeAutoVerticalScale &= 1);

}

void CBladeTolerancePage::OnChangeEditMaxTol() 
{
	CViewBladeThickneessDlg* pParent = (CViewBladeThickneessDlg*) m_pParent;
	CString Buff;
	float fTemp = theApp.m_LastSettings.fBladeMaxTolerance;

	m_editMax.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fBladeMaxTolerance);
	if(fTemp - theApp.m_LastSettings.fBladeMaxTolerance) {
		m_spinMax.SetPos((int)(theApp.m_LastSettings.fBladeMaxTolerance * 1000.0f));
		pParent->UpdateBladeTolerances();
		RefreshParent();
	}
}

void CBladeTolerancePage::OnChangeEditMinTol() 
{
	CViewBladeThickneessDlg* pParent = (CViewBladeThickneessDlg*) m_pParent;
	CString Buff;
	float fTemp = theApp.m_LastSettings.fBladeMinTolerance;

	m_editMin.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fBladeMinTolerance);
	if(fTemp - theApp.m_LastSettings.fBladeMinTolerance) {
		m_spinMin.SetPos((int)(theApp.m_LastSettings.fBladeMinTolerance * 1000.0f));
		pParent->UpdateBladeTolerances();
		RefreshParent();
	}
}

void CBladeTolerancePage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CViewBladeThickneessDlg* pParent = (CViewBladeThickneessDlg*) m_pParent;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	float fTemp;
	int nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_MIN_TOL:
		fTemp = (float)(pSpin->GetPos()&0xffff)/1000.0f;
		if(theApp.m_LastSettings.fBladeMinTolerance - fTemp) {
			theApp.m_LastSettings.fBladeMinTolerance = fTemp;
			UpdateAllControls();
			pParent->UpdateBladeTolerances();
			RefreshParent();
		}
		break;
	case IDC_SPIN_MAX_TOL:
		fTemp = (float)(pSpin->GetPos()&0xffff)/1000.0f;
		if(theApp.m_LastSettings.fBladeMaxTolerance - fTemp) {
			theApp.m_LastSettings.fBladeMaxTolerance = fTemp;
			UpdateAllControls();
			pParent->UpdateBladeTolerances();
			RefreshParent();
		}
		break;
	case IDC_SPIN_FONT_SIZE:
		nTemp = theApp.m_LastSettings.nBladeFontSize;
		theApp.m_LastSettings.nBladeFontSize = pSpin->GetPos()&0xffff;
		if(nTemp - theApp.m_LastSettings.nBladeFontSize) {
			UpdateAllControls();
			RefreshParent();
		}
		break;

	}
	
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CBladeTolerancePage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CMFCColorDialog dlg;

	m_staticMax.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbCrossSection[RGB_MAXIMUM]=dlg.GetColor();
			Invalidate(FALSE);
			RefreshParent();
		}
	}

	m_staticMin.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbCrossSection[RGB_MINIMUM]=dlg.GetColor();
			Invalidate(FALSE);
			RefreshParent();
		}
	}
	
	m_staticNom.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbCrossSection[RGB_NOMINAL]=dlg.GetColor();
			Invalidate(FALSE);
			RefreshParent();
		}
	}

	m_staticAct.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbCrossSection[RGB_TOF_TRACE]=dlg.GetColor();
			Invalidate(FALSE);
			RefreshParent();
		}
	}

	m_staticBkColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbCrossSection[RGB_BKGROUND]=dlg.GetColor();
			Invalidate(FALSE);
			RefreshParent();
		}
	}

	m_staticFontColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbCrossSection[RGB_FONT]=dlg.GetColor();
			Invalidate(FALSE);
			RefreshParent();
		}
	}

	CPropertyPage::OnLButtonDown(nFlags, point);
}

void CBladeTolerancePage::RefreshParent()
{
	CViewBladeThickneessDlg* pParent = (CViewBladeThickneessDlg*) m_pParent;

//	pParent->SendMessage(WM_PAINT);
	pParent->Invalidate(FALSE);

}

void CBladeTolerancePage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	BlockFill(&dc,&m_staticMin,theApp.m_LastSettings.rgbCrossSection[RGB_MINIMUM]);
	BlockFill(&dc,&m_staticMax,theApp.m_LastSettings.rgbCrossSection[RGB_MAXIMUM]);
	BlockFill(&dc,&m_staticNom,theApp.m_LastSettings.rgbCrossSection[RGB_NOMINAL]);
	BlockFill(&dc,&m_staticAct,theApp.m_LastSettings.rgbCrossSection[RGB_TOF_TRACE]);
	BlockFill(&dc,&m_staticBkColor,theApp.m_LastSettings.rgbCrossSection[RGB_BKGROUND]);
	BlockFill(&dc,&m_staticFontColor,theApp.m_LastSettings.rgbCrossSection[RGB_FONT]);

}

void CBladeTolerancePage::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);

}

void CBladeTolerancePage::OnCheckDisplayMin() 
{
	m_checkDisplayMin.GetCheck() == 1 ? theApp.m_LastSettings.nBladeDisplayMask |= DISPLAY_MIN_TOLERANCE : theApp.m_LastSettings.nBladeDisplayMask &= ~DISPLAY_MIN_TOLERANCE;
	RefreshParent();

}

void CBladeTolerancePage::OnCheckDisplayMax() 
{
	m_checkDisplayMax.GetCheck() == 1 ? theApp.m_LastSettings.nBladeDisplayMask |= DISPLAY_MAX_TOLERANCE : theApp.m_LastSettings.nBladeDisplayMask &= ~DISPLAY_MAX_TOLERANCE;
	RefreshParent();
}

void CBladeTolerancePage::OnCheckDisplayNom() 
{
	m_checkDisplayNom.GetCheck() == 1 ? theApp.m_LastSettings.nBladeDisplayMask |= DISPLAY_NOM_THICKNESS : theApp.m_LastSettings.nBladeDisplayMask &= ~DISPLAY_NOM_THICKNESS;
	RefreshParent();
}


void CBladeTolerancePage::OnChangeEditFontSize() 
{
	CString Buff;
	int nTemp = theApp.m_LastSettings.nBladeFontSize;

	Buff.Format(_T("%d"),theApp.m_LastSettings.nBladeFontSize);
	m_editFontSize.GetWindowText(Buff);
	theApp.m_LastSettings.nBladeFontSize = _ttoi(Buff);
	if(nTemp - theApp.m_LastSettings.nBladeFontSize) {
		m_spinFontSize.SetPos(theApp.m_LastSettings.nBladeFontSize);
		RefreshParent();
	}

	
}

BOOL CBladeTolerancePage::OnSetActive() 
{
	theApp.m_LastSettings.nLastViewOptionsTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

void CBladeTolerancePage::OnCheckAutoVerticalScale() 
{
	theApp.m_LastSettings.nBladeAutoVerticalScale = m_checkAutoVerticalScale.GetCheck();
}
