// HistogramOptionsPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "HistogramOptionsPage.h"
#include "ViewSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistogramOptionsPage property page

IMPLEMENT_DYNCREATE(CHistogramOptionsPage, CPropertyPage)

CHistogramOptionsPage::CHistogramOptionsPage(CWnd* pParent) : CPropertyPage(CHistogramOptionsPage::IDD)
{
	//{{AFX_DATA_INIT(CHistogramOptionsPage)
	//}}AFX_DATA_INIT
	m_pParent = pParent;
}

CHistogramOptionsPage::~CHistogramOptionsPage()
{
}

void CHistogramOptionsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistogramOptionsPage)
	DDX_Control(pDX, IDC_EDIT_MAX_DROP_OUT, m_editMaxDropOut);
	DDX_Control(pDX, IDC_STATIC_PAINT_DROP_OUT, m_staticPaintDropOut);
	DDX_Control(pDX, IDC_EDIT_ASPECT_RATIO, m_editAspectRatio);
	DDX_Control(pDX, IDC_EDIT_AREA, m_editArea);
	DDX_Control(pDX, IDC_EDIT_HIST_SIZE, m_editHistogramSize);
	DDX_Control(pDX, IDC_CHECK_FIXEDSIZE, m_checkEnterSize);
	DDX_Control(pDX, IDC_STATIC_STYLE_2, m_staticStyleCircle);
	DDX_Control(pDX, IDC_STATIC_STYLE_1, m_staticStyleBox);
	DDX_Control(pDX, IDC_SPIN_FONT_SIZE, m_spinFontSize);
	DDX_Control(pDX, IDC_EDIT_FONT_SIZE, m_editFontSize);
	DDX_Control(pDX, IDC_CHECK_PAINT_1, m_checkPaint1);
	DDX_Control(pDX, IDC_CHECK_PAINT, m_checkPaint0);
	DDX_Control(pDX, IDC_STATIC_PAINT_1, m_staticPaint1);
	DDX_Control(pDX, IDC_STATIC_PAINT_0, m_staticPaint0);
	DDX_Control(pDX, IDC_CHECK_MAX_WIDTH, m_checkMaxWidth);
	DDX_Control(pDX, IDC_CHECK_MAX_HEIGHT, m_checkMaxHeight);
	DDX_Control(pDX, IDC_STATIC_1, m_staticView1);
	DDX_Control(pDX, IDC_STATIC_0, m_staticView0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistogramOptionsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CHistogramOptionsPage)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_LBN_SELCHANGE(IDC_LIST_AREA_OPTION, OnSelchangeListAreaOption)
	ON_BN_CLICKED(IDC_CHECK_MAX_WIDTH, OnCheckMaxWidth)
	ON_BN_CLICKED(IDC_CHECK_MAX_HEIGHT, OnCheckMaxHeight)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_CHECK_PAINT, OnCheckPaint0)
	ON_BN_CLICKED(IDC_CHECK_PAINT_1, OnCheckPaint1)
	ON_EN_CHANGE(IDC_EDIT_FONT_SIZE, OnChangeEditFontSize)
	ON_BN_CLICKED(IDC_CHECK_FIXEDSIZE, OnCheckFixedsize)
	ON_EN_CHANGE(IDC_EDIT_HIST_SIZE, OnChangeEditHistSize)
	ON_EN_CHANGE(IDC_EDIT_ASPECT_RATIO, OnChangeEditAspectRatio)
	ON_EN_CHANGE(IDC_EDIT_AREA, OnChangeEditArea)
	ON_EN_CHANGE(IDC_EDIT_MAX_DROP_OUT, OnChangeEditMaxDropOut)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistogramOptionsPage message handlers

BOOL CHistogramOptionsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	m_spinFontSize.SetRange(8,24);

	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CHistogramOptionsPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CHistogramOptionsPage::UpdateAllControls()
{
	CString Buff;
	
	m_checkMaxWidth.SetCheck(theApp.m_LastSettings.bHistogramMaxWidth&1);
	m_checkMaxHeight.SetCheck(theApp.m_LastSettings.bHistogramMaxHeight&1);

	m_checkPaint0.SetCheck(theApp.m_LastSettings.bHistogramPaintView[0]  & 1 );
	m_checkPaint1.SetCheck(theApp.m_LastSettings.bHistogramPaintView[1]  & 1 );

	m_checkEnterSize.SetCheck(theApp.m_LastSettings.bHistogramEnterSize);


	Buff.Format(_T("%d"),theApp.m_LastSettings.lfHistogram.lfHeight);
	m_editFontSize.SetWindowText(Buff);
	m_spinFontSize.SetPos(theApp.m_LastSettings.lfHistogram.lfHeight);


	Buff.Format(_T("%.01f mm"),theApp.m_LastSettings.fHistogramCircleSize);
	m_editHistogramSize.SetWindowText(Buff);


	Buff.Format(L"%.01f : 1",theApp.m_LastSettings.fDropOutAspectRatio);
	m_editAspectRatio.SetWindowText(Buff);
	Buff.Format(L"%.01fmm",theApp.m_LastSettings.fDropOutArea);
	m_editArea.SetWindowText(Buff);
	Buff.Format(L"%.01f%%",theApp.m_LastSettings.fDropOutMaxPercentage);
	m_editMaxDropOut.SetWindowText(Buff);

}


void CHistogramOptionsPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	int	ii,yy,xx,nSum;
	CRect rr;
	CPen* pOldPen;
	CPen pen1(PS_SOLID,1,theApp.m_LastSettings.rgbRulerLine);
	
	for(ii=0;ii<2;ii++) {
		switch(ii) {
		case 0:	m_staticView0.GetWindowRect(&rr);
			break;
		case 1:	m_staticView1.GetWindowRect(&rr);
			break;
		}
		ScreenToClient(&rr);
		dc.Rectangle(&rr);
		rr.DeflateRect(6,6);
		dc.MoveTo(rr.TopLeft());
		dc.LineTo(rr.left,rr.bottom);
		dc.LineTo(rr.BottomRight());

		switch(ii) {
		case 0:
			dc.MoveTo(rr.TopLeft());
			for(yy=rr.top;yy<rr.bottom;yy++) {
				xx=MulDiv(rand(),rr.Width()-1,RAND_MAX)+rr.left;
				dc.LineTo(xx,yy);
			}
			break;
		case 1:
			dc.MoveTo(rr.TopLeft());
			nSum=rr.left;
			for(yy=rr.top;yy<rr.bottom;yy++) {
				if(nSum<rr.right) nSum+=MulDiv(rand()/10,rr.Width()-1,RAND_MAX);
				if(nSum>rr.right) nSum=rr.right;
				dc.LineTo(nSum,yy);
			}
			break;
		}
	}
	BlockFill(&dc,&m_staticPaint0,theApp.m_LastSettings.rgbHistogramColor[0]);
	BlockFill(&dc,&m_staticPaint1,theApp.m_LastSettings.rgbHistogramColor[1]);
	BlockFill(&dc,&m_staticPaintDropOut,theApp.m_LastSettings.rgbDropOut);

	pOldPen=dc.SelectObject(&pen1);
	
	m_staticStyleBox.GetWindowRect(&rr);
	ScreenToClient(&rr);
	dc.SelectObject(pOldPen);
	dc.Rectangle(rr);
	dc.SelectObject(&pen1);
	rr.DeflateRect(3,3);
	dc.Rectangle(rr);

	m_staticStyleCircle.GetWindowRect(&rr);
	ScreenToClient(&rr);
	dc.SelectObject(pOldPen);
	dc.Rectangle(rr);
	dc.SelectObject(&pen1);
	rr.DeflateRect(3,3);
	rr.top = rr.CenterPoint().y - rr.Width()/2;
	rr.bottom = rr.CenterPoint().y + rr.Width()/2-1;
	dc.Ellipse(&rr);

	dc.SelectObject(pOldPen);
}

void CHistogramOptionsPage::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);
}


void CHistogramOptionsPage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CMFCColorDialog dlg;
	
	m_staticView0.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) theApp.m_LastSettings.nHistogramAbsoluteCummulative=0;

	m_staticView1.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) theApp.m_LastSettings.nHistogramAbsoluteCummulative=1;

	m_staticPaint0.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbHistogramColor[0]=dlg.GetColor();
			Invalidate(FALSE);
		};
	}
	m_staticPaint1.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbHistogramColor[1]=dlg.GetColor();
			Invalidate(FALSE);
		};
	}
	m_staticPaintDropOut.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbDropOut=dlg.GetColor();
			Invalidate(FALSE);
		};
	}

	m_staticStyleBox.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) 	theApp.m_LastSettings.nHistogramStyle = HISTOGRAM_RECTANGLE_STYLE;
	m_staticStyleCircle.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) 	theApp.m_LastSettings.nHistogramStyle = HISTOGRAM_CIRCLE_STYLE;

	
	CPropertyPage::OnLButtonDown(nFlags, point);
}


void CHistogramOptionsPage::OnSelchangeListAreaOption() 
{
	theApp.m_LastSettings.nHistogramAreamode = m_listAreaOption.GetCurSel();
}

void CHistogramOptionsPage::OnCheckMaxWidth() 
{
	theApp.m_LastSettings.bHistogramMaxWidth = m_checkMaxWidth.GetCheck() & 1;
}

void CHistogramOptionsPage::OnCheckMaxHeight() 
{
	theApp.m_LastSettings.bHistogramMaxHeight = m_checkMaxHeight.GetCheck() & 1;
}




void CHistogramOptionsPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CString	Buff;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	int	nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_DB_CHANGE:
		nTemp = theApp.m_LastSettings.nHistogramAmpDeltaThreshold;
		theApp.m_LastSettings.nHistogramAmpDeltaThreshold=(pSpin->GetPos()&0xffff)-100;
		if(nTemp - theApp.m_LastSettings.nHistogramAmpDeltaThreshold) {
			Buff.Format(_T("%ddB"),theApp.m_LastSettings.nHistogramAmpDeltaThreshold);
			m_editDBChange.SetWindowText(Buff);
		}
		break;
	case IDC_SPIN_FONT_SIZE:
		nTemp = theApp.m_LastSettings.lfHistogram.lfHeight;
		theApp.m_LastSettings.lfHistogram.lfHeight=pSpin->GetPos()&0xffff;
		Buff.Format(_T("%d"),theApp.m_LastSettings.lfHistogram.lfHeight);
		m_editFontSize.SetWindowText(Buff);
		break;

	}

	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CHistogramOptionsPage::OnCheckPaint0() 
{
	theApp.m_LastSettings.bHistogramPaintView[0] = m_checkPaint0.GetCheck() & 1;

	CViewSheet* pSheet = (CViewSheet*)m_pParent;
	pSheet->InvalidateChild();

}




void CHistogramOptionsPage::OnCheckPaint1() 
{
	theApp.m_LastSettings.bHistogramPaintView[1] = m_checkPaint1.GetCheck() & 1;

	CViewSheet* pSheet = (CViewSheet*)m_pParent;
	pSheet->InvalidateChild();
}

void CHistogramOptionsPage::OnChangeEditFontSize() 
{
	CString Buff;
	int ntemp = theApp.m_LastSettings.lfHistogram.lfHeight;

	m_editFontSize.GetWindowText(Buff);
	theApp.m_LastSettings.lfHistogram.lfHeight = _ttoi(Buff);
	if(ntemp - theApp.m_LastSettings.lfHistogram.lfHeight) {
		m_spinFontSize.SetPos(theApp.m_LastSettings.lfHistogram.lfHeight);
	}
}

void CHistogramOptionsPage::OnCheckFixedsize() 
{
	theApp.m_LastSettings.bHistogramEnterSize = m_checkEnterSize.GetCheck() & 1;

	CViewSheet* pSheet = (CViewSheet*)m_pParent;
	pSheet->InvalidateChild();
}

void CHistogramOptionsPage::OnChangeEditHistSize() 
{
	CString Buff;

	m_editHistogramSize.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fHistogramCircleSize);
}

void CHistogramOptionsPage::OnChangeEditAspectRatio() 
{
	CString Buff;
	float fTemp;

	m_editAspectRatio.GetWindowText(Buff);

	swscanf_s(Buff,L"%f",&fTemp);
	theApp.m_LastSettings.fDropOutAspectRatio = fTemp;	
}

void CHistogramOptionsPage::OnChangeEditArea() 
{
	CString Buff;
	float fTemp;

	m_editArea.GetWindowText(Buff);

	swscanf_s(Buff,L"%f",&fTemp);

	switch(theApp.m_LastSettings.nMeasurementUnits) {
	default:	theApp.m_LastSettings.fDropOutArea = fTemp;
		break;
	case 1:	theApp.m_LastSettings.fDropOutArea = fTemp * theApp.m_fUnits;
		break;
	}
}

void CHistogramOptionsPage::OnChangeEditMaxDropOut() 
{
	CString Buff;

	m_editMaxDropOut.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fDropOutMaxPercentage);
}
