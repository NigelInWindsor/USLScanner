// GraticulePage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "GraticulePage.h"
#include "ViewSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGraticulePage property page

IMPLEMENT_DYNCREATE(CGraticulePage, CPropertyPage)

CGraticulePage::CGraticulePage(CWnd* pViewSheet) : CPropertyPage(CGraticulePage::IDD)
{
	//{{AFX_DATA_INIT(CGraticulePage)
	//}}AFX_DATA_INIT
	m_pViewSheet = pViewSheet;
	
}

CGraticulePage::~CGraticulePage()
{
}

void CGraticulePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGraticulePage)
	DDX_Control(pDX, IDC_SPIN_SPACING, m_spinSpacing);
	DDX_Control(pDX, IDC_CHECK_SPACING_AUTO, m_checkAutoSpacing);
	DDX_Control(pDX, IDC_EDIT_SPACING, m_editSpacing);
	DDX_Control(pDX, IDC_CHECK_R_AXIS_REVERSE, m_checkRAxisGraticuleReverse);
	DDX_Control(pDX, IDC_COMBO_RAXIS_MODE, m_comboRaxisMode);
	DDX_Control(pDX, IDC_STATIC_EXAMPLE, m_staticExample);
	DDX_Control(pDX, IDC_STATIC_STYLE_COLOR, m_staticStyleColor);
	DDX_Control(pDX, IDC_STATIC_7, m_static7);
	DDX_Control(pDX, IDC_STATIC_6, m_static6);
	DDX_Control(pDX, IDC_STATIC_5, m_static5);
	DDX_Control(pDX, IDC_STATIC_4, m_static4);
	DDX_Control(pDX, IDC_STATIC_3, m_static3);
	DDX_Control(pDX, IDC_STATIC_2, m_static2);
	DDX_Control(pDX, IDC_STATIC_1, m_static1);
	DDX_Control(pDX, IDC_STATIC_0, m_static0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGraticulePage, CPropertyPage)
	//{{AFX_MSG_MAP(CGraticulePage)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	ON_CBN_SELCHANGE(IDC_COMBO_RAXIS_MODE, OnSelchangeComboRaxisMode)
	ON_BN_CLICKED(IDC_CHECK_R_AXIS_REVERSE, OnCheckRAxisReverse)
	ON_EN_CHANGE(IDC_EDIT_SPACING, OnChangeEditSpacing)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPACING, OnDeltaposSpinSpacing)
	ON_BN_CLICKED(IDC_CHECK_SPACING_AUTO, OnCheckSpacingAuto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraticulePage message handlers

void CGraticulePage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rr;
	int	xx,yy,nXp,nYp;
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	CPen penStyle(PS_SOLID,1,theApp.m_LastSettings.rgbViewGraticuleColor);
	CPen* pOldPen = dc.SelectObject(&penBlack);
	
	for(int ii=0;ii<8;ii++) {
		switch(ii) {
		case 0: m_static0.GetWindowRect(&rr);
			break;
		case 1: m_static1.GetWindowRect(&rr);
			break;
		case 2: m_static2.GetWindowRect(&rr);
			break;
		case 3: m_static3.GetWindowRect(&rr);
			break;
		case 4: m_static4.GetWindowRect(&rr);
			break;
		case 5: m_static5.GetWindowRect(&rr);
			break;
		case 6: m_static6.GetWindowRect(&rr);
			break;
		case 7: m_static7.GetWindowRect(&rr);
			break;
		}
		dc.SelectObject(&penBlack);
		ScreenToClient(&rr);
		rr.DeflateRect(1,1);
		dc.FillRect(&rr,&CBrush(RGB(255,255,255)));

		rr.DeflateRect(6,6);
		dc.Rectangle(&rr);
		switch(ii) {
		case 0:
			break;
		case 1:
			for(xx=0;xx<5;xx++) {
				nXp=MulDiv(xx,rr.Width()-1,4) + rr.left;
				dc.MoveTo(nXp,rr.bottom);
				dc.LineTo(nXp,rr.bottom+3);
			}
			for(yy=0;yy<6;yy++) {
				nYp=MulDiv(yy,rr.Height()-1,5) + rr.top;
				dc.MoveTo(rr.right,nYp);
				dc.LineTo(rr.right+3,nYp);
			}
			break;
		case 2:
			for(xx=0;xx<5;xx++) {
				nXp=MulDiv(xx,rr.Width()-1,4) + rr.left;
				dc.MoveTo(nXp,rr.bottom);
				dc.LineTo(nXp,rr.bottom+3);
			}
			for(yy=0;yy<6;yy++) {
				nYp=MulDiv(yy,rr.Height()-1,5) + rr.top;
				dc.MoveTo(rr.left,nYp);
				dc.LineTo(rr.left-3,nYp);
			}
			break;
		case 3:
			for(xx=0;xx<5;xx++) {
				nXp=MulDiv(xx,rr.Width()-1,4) + rr.left;
				dc.MoveTo(nXp,rr.bottom);
				dc.LineTo(nXp,rr.bottom+3);
			}
			for(yy=0;yy<6;yy++) {
				nYp=MulDiv(yy,rr.Height()-1,5) + rr.top;
				dc.MoveTo(rr.right,nYp);
				dc.LineTo(rr.right+3,nYp);
				dc.MoveTo(rr.left,nYp);
				dc.LineTo(rr.left-3,nYp);
			}
			break;
		case 4:
			break;
		case 5: dc.SelectObject(&penStyle);
			for(xx=1;xx<4;xx++) {
				nXp=MulDiv(xx,rr.Width()-1,4) + rr.left;
				dc.MoveTo(nXp,rr.top);
				dc.LineTo(nXp,rr.bottom);
			}
			for(yy=1;yy<5;yy++) {
				nYp=MulDiv(yy,rr.Height()-1,5) + rr.top;
				dc.MoveTo(rr.left,nYp);
				dc.LineTo(rr.right,nYp);
			}
			break;
		case 6: dc.SelectObject(&penStyle);
			for(yy=1;yy<5;yy++) {
				nYp=MulDiv(yy,rr.Height()-1,5) + rr.top;
				for(xx=1;xx<4;xx++) {
					nXp=MulDiv(xx,rr.Width()-1,4) + rr.left;
					dc.SetPixel(nXp,nYp,theApp.m_LastSettings.rgbViewGraticuleColor);
				}
			}
			break;
		case 7: dc.SelectObject(&penStyle);
			for(yy=1;yy<5;yy++) {
				nYp=MulDiv(yy,rr.Height()-1,5) + rr.top;
				for(xx=1;xx<4;xx++) {
					nXp=MulDiv(xx,rr.Width()-1,4) + rr.left;
					dc.MoveTo(nXp-2,nYp);
					dc.LineTo(nXp+3,nYp);
					dc.MoveTo(nXp,nYp-2);
					dc.LineTo(nXp,nYp+3);
				}
			}
			break;
		}

	}
	m_staticStyleColor.GetWindowRect(&rr);
	ScreenToClient(&rr);
	dc.FillRect(&rr,&CBrush(theApp.m_LastSettings.rgbViewGraticuleColor));

	dc.SelectObject(pOldPen);

	CString Buff = L"25.6";
	CFont* pFont = (CFont *) new CFont;
	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	pFont->CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
	CFont* pOldFont = dc.SelectObject(pFont);
	m_staticExample.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(3,3);
	dc.FillRect(&rr,&CBrush(RGB(255,255,255)));
	CSize size = dc.GetTextExtent(Buff);
	dc.TextOut((rr.left+rr.right)/2 - size.cx/2,(rr.top+rr.bottom)/2-size.cy/2,L"25.6");
	dc.SelectObject(pOldFont);
	delete pFont;


}

void CGraticulePage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CMFCColorDialog dlg;
	
	for(int ii=0;ii<8;ii++) {
		switch(ii) {
		case 0: m_static0.GetWindowRect(&rr);
			break;
		case 1: m_static1.GetWindowRect(&rr);
			break;
		case 2: m_static2.GetWindowRect(&rr);
			break;
		case 3: m_static3.GetWindowRect(&rr);
			break;
		case 4: m_static4.GetWindowRect(&rr);
			break;
		case 5: m_static5.GetWindowRect(&rr);
			break;
		case 6: m_static6.GetWindowRect(&rr);
			break;
		case 7: m_static7.GetWindowRect(&rr);
			break;
		}
		ScreenToClient(&rr);
		if(rr.PtInRect(point)) {
			switch(ii) {
			case 0:	theApp.m_LastSettings.nViewGraticuleStyle	&= ~(GRATICULE_BOTTOM | GRATICULE_RIGHT | GRATICULE_LEFT | GRATICULE_TOP);
				break;
			case 1:	theApp.m_LastSettings.nViewGraticuleStyle	&= ~(GRATICULE_BOTTOM | GRATICULE_RIGHT | GRATICULE_LEFT | GRATICULE_TOP);
				theApp.m_LastSettings.nViewGraticuleStyle |= (GRATICULE_BOTTOM | GRATICULE_RIGHT);
				break;
			case 2:	theApp.m_LastSettings.nViewGraticuleStyle	&= ~(GRATICULE_BOTTOM | GRATICULE_RIGHT | GRATICULE_LEFT | GRATICULE_TOP);
				theApp.m_LastSettings.nViewGraticuleStyle |= (GRATICULE_BOTTOM | GRATICULE_LEFT);
				break;
			case 3:	theApp.m_LastSettings.nViewGraticuleStyle	&= ~(GRATICULE_BOTTOM | GRATICULE_RIGHT | GRATICULE_LEFT | GRATICULE_TOP);
				theApp.m_LastSettings.nViewGraticuleStyle |= (GRATICULE_BOTTOM | GRATICULE_RIGHT | GRATICULE_LEFT);
				break;
			case 4: theApp.m_LastSettings.nViewGraticuleStyle &= ~(GRATICULE_LINES | GRATICULE_DOTS | GRATICULE_CROSSES);
				break;
			case 5: theApp.m_LastSettings.nViewGraticuleStyle &= ~(GRATICULE_LINES | GRATICULE_DOTS | GRATICULE_CROSSES);
				theApp.m_LastSettings.nViewGraticuleStyle |= GRATICULE_LINES;
				break;
			case 6: theApp.m_LastSettings.nViewGraticuleStyle &= ~(GRATICULE_LINES | GRATICULE_DOTS | GRATICULE_CROSSES);
				theApp.m_LastSettings.nViewGraticuleStyle |= GRATICULE_DOTS;
				break;
			case 7: theApp.m_LastSettings.nViewGraticuleStyle &= ~(GRATICULE_LINES | GRATICULE_DOTS | GRATICULE_CROSSES);
				theApp.m_LastSettings.nViewGraticuleStyle |= GRATICULE_CROSSES;
				break;
			}
		}
	}

	m_staticStyleColor.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point) ) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbViewGraticuleColor=dlg.GetColor();
			Invalidate(FALSE);
		};
	};
	

	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;
	pSheet->InvalidateChild();

	CPropertyPage::OnLButtonDown(nFlags, point);
}


void CGraticulePage::OnButtonFont() 
{
	CFontDialog dlg(&theApp.m_LastSettings.lfViewGraticule);
	dlg.m_cf.rgbColors = RGB(0,0,0);
	if(dlg.DoModal() == IDOK) {
		dlg.GetCurrentFont(&theApp.m_LastSettings.lfViewGraticule);
		Invalidate(FALSE);
	}
	
}

void CGraticulePage::OnSelchangeComboRaxisMode() 
{
	theApp.m_LastSettings.nRAxisGraticuleMode = m_comboRaxisMode.GetCurSel();
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;
	pSheet->InvalidateChild();
}

BOOL CGraticulePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	Buff.LoadString(IDS_Degrees);			m_comboRaxisMode.AddString(Buff);
	Buff.Format(L"MM First line");			m_comboRaxisMode.AddString(Buff);
	Buff.Format(L"MM Last line");			m_comboRaxisMode.AddString(Buff);
	
	m_comboRaxisMode.SetCurSel(theApp.m_LastSettings.nRAxisGraticuleMode);
	
	m_checkRAxisGraticuleReverse.SetCheck(theApp.m_LastSettings.nRAxisGraticuleReverse);

	m_spinSpacing.SetRange(0,10000);
	m_spinSpacing.SetPos((int)(theApp.m_LastSettings.fViewGraticuleSpacing * 10.0f));
	
	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CGraticulePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CGraticulePage::OnCheckRAxisReverse() 
{
	theApp.m_LastSettings.nRAxisGraticuleReverse = m_checkRAxisGraticuleReverse.GetCheck();
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;
	pSheet->InvalidateChild();

}

void CGraticulePage::OnChangeEditSpacing() 
{
	CString Buff;

	m_editSpacing.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fViewGraticuleSpacing);

	m_spinSpacing.SetPos((int)(theApp.m_LastSettings.fViewGraticuleSpacing * 10.0f));

	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;
	pSheet->InvalidateChild();
	
}

void CGraticulePage::OnDeltaposSpinSpacing(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	theApp.m_LastSettings.fViewGraticuleSpacing += (1.0f * (float)pNMUpDown->iDelta);
	UpdateAllControls();

	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;
	pSheet->InvalidateChild();

	*pResult = 0;
}

void CGraticulePage::OnCheckSpacingAuto() 
{
	m_checkAutoSpacing.GetCheck() ? theApp.m_LastSettings.nViewGraticuleSpacingMode = 1 : theApp.m_LastSettings.nViewGraticuleSpacingMode = 0;

	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;
	pSheet->InvalidateChild();
	
}

void CGraticulePage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	Buff.Format(L"%.01f mm",theApp.m_LastSettings.fViewGraticuleSpacing);
	m_editSpacing.SetWindowText(Buff);

	if(theApp.m_LastSettings.nViewGraticuleSpacingMode == 0) {
		m_checkAutoSpacing.SetCheck(false);
	} else {
		m_checkAutoSpacing.SetCheck(true);
	}

}

BOOL CGraticulePage::OnSetActive() 
{
	theApp.m_LastSettings.nLastViewOptionsTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}
