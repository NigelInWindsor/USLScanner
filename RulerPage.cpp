// RulerPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "RulerPage.h"
#include "ViewPage.h"
#include "ViewSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRulerPage property page

IMPLEMENT_DYNCREATE(CRulerPage, CPropertyPage)

CRulerPage::CRulerPage(CWnd* pParent, int nParentType) : CPropertyPage(CRulerPage::IDD)
{
	//{{AFX_DATA_INIT(CRulerPage)
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	
	m_bScanUsesRotateAxis=false;
	m_nParentType = nParentType;
}

CRulerPage::~CRulerPage()
{
}

void CRulerPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRulerPage)
	DDX_Control(pDX, IDC_COMBO_FLAG_MODE, m_comboFlagMode);
	DDX_Control(pDX, IDC_COMBO_UNITS, m_comboUnits);
	DDX_Control(pDX, IDC_EDIT_SCALE, m_editScale);
	DDX_Control(pDX, IDC_STATIC_WIDTH_3, m_staticWidth3);
	DDX_Control(pDX, IDC_STATIC_STYLE_2, m_staticStyleEllipse);
	DDX_Control(pDX, IDC_STATIC_STYLE_1, m_staticStyleBox);
	DDX_Control(pDX, IDC_STATIC_STYLE_0, m_staticStyleLine);
	DDX_Control(pDX, IDC_LIST_RULER_TEXT, m_listRulerText);
	DDX_Control(pDX, IDC_STATIC_WIDTH_2, m_staticWidth2);
	DDX_Control(pDX, IDC_STATIC_WIDTH_1, m_staticWidth1);
	DDX_Control(pDX, IDC_STATIC_WIDTH_0, m_staticWidth0);
	DDX_Control(pDX, IDC_STATIC_COLOR, m_staticColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRulerPage, CPropertyPage)
	//{{AFX_MSG_MAP(CRulerPage)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_LBN_SELCHANGE(IDC_LIST_RULER_TEXT, OnSelchangeListRulerText)
	ON_EN_CHANGE(IDC_EDIT_SCALE, OnChangeEditScale)
	ON_CBN_SELCHANGE(IDC_COMBO_UNITS, OnSelchangeComboUnits)
	ON_CBN_SELCHANGE(IDC_COMBO_FLAG_MODE, OnSelchangeComboFlagMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRulerPage message handlers

BOOL CRulerPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CViewSheet* pSheet = (CViewSheet*)m_pParent;
	CString Buff;

	if(m_nParentType == 0) {
		CViewSheet* pSheet = (CViewSheet*)m_pParent;
		if(pSheet->m_Data.m_nFastAxis == 5)
			m_bScanUsesRotateAxis=true;
	} else {
		CVolumeViewDlg* pDlg = (CVolumeViewDlg*)m_pParent;
		if(pDlg->m_pData->m_nFastAxis == 5)
			m_bScanUsesRotateAxis=true;
	}


	//Change this to list control and map options to ruler mode
	if(m_bScanUsesRotateAxis) {
		//Add to String table
		m_listRulerText.AddString(_T("Circumferential Angle"));
		m_listRulerText.AddString(_T("Circumferential Distance"));
		m_listRulerText.AddString(_T("Perpendicular Distance"));
		m_listRulerText.AddString(_T("Surface Distance"));
	}else{
		Buff.LoadString(IDS_Ruler_Length_XXX); m_listRulerText.AddString(Buff);
		Buff.LoadString(IDS_Vector_Length_XXX); m_listRulerText.AddString(Buff);
		Buff.LoadString(IDS_Surface_Length_XXX); m_listRulerText.AddString(Buff);
		Buff.LoadString(IDS_Width_Height_XXX); m_listRulerText.AddString(Buff);
		Buff.LoadString(IDS_Diameter_XXX); m_listRulerText.AddString(Buff);
	}


	Buff.LoadString(IDS_None);		m_comboFlagMode.AddString(Buff);
	Buff.LoadString(IDS_Start);		m_comboFlagMode.AddString(Buff);
	Buff.LoadString(IDS_Finish);	m_comboFlagMode.AddString(Buff);
	Buff.LoadString(IDS_Mid_way);	m_comboFlagMode.AddString(Buff);

	m_comboUnits.AddString(_T("mm"));
	m_comboUnits.AddString(_T("\x022"));
	
	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CRulerPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CRulerPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	if(m_bScanUsesRotateAxis) {
		m_listRulerText.SetCurSel(theApp.m_LastSettings.nRulerTextMode-5);
	}else{
		m_listRulerText.SetCurSel(theApp.m_LastSettings.nRulerTextMode);
	}

	Buff.Format(_T("%.01f%s"),theApp.m_LastSettings.fRulerScale,theApp.m_Units);
	m_editScale.SetWindowText(Buff);

	m_comboUnits.SetCurSel(theApp.m_LastSettings.nMeasurementUnits);
	m_comboFlagMode.SetCurSel(theApp.m_LastSettings.nRulerFlagMode);

}

void CRulerPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rr;
	CPen* pOldPen;
	int	ii,nX;
	CFont Font;
	CFont* pOldFont;
	CString Buff;

	m_staticWidth0.GetWindowRect(&rr);
	ScreenToClient(&rr);
	dc.Rectangle(rr);
	CPen pen1(PS_SOLID,1,theApp.m_LastSettings.rgbRulerLine);
	pOldPen=dc.SelectObject(&pen1);
	dc.MoveTo(rr.left+4,rr.CenterPoint().y);
	dc.LineTo(rr.right-4,rr.CenterPoint().y);

	m_staticWidth1.GetWindowRect(&rr);
	ScreenToClient(&rr);
	dc.SelectObject(pOldPen);
	dc.Rectangle(rr);
	CPen pen2(PS_SOLID,2,theApp.m_LastSettings.rgbRulerLine);
	dc.SelectObject(&pen2);
	dc.MoveTo(rr.left+4,rr.CenterPoint().y);
	dc.LineTo(rr.right-4,rr.CenterPoint().y);

	m_staticWidth2.GetWindowRect(&rr);
	ScreenToClient(&rr);
	dc.SelectObject(pOldPen);
	dc.Rectangle(rr);
	CPen pen3(PS_SOLID,3,theApp.m_LastSettings.rgbRulerLine);
	dc.SelectObject(&pen3);
	dc.MoveTo(rr.left+4,rr.CenterPoint().y);
	dc.LineTo(rr.right-4,rr.CenterPoint().y);

	m_staticWidth3.GetWindowRect(&rr);
	ScreenToClient(&rr);
	dc.SelectObject(pOldPen);
	dc.Rectangle(rr);
	CBrush brush(GetSysColor(COLOR_INFOBK));
	CBrush* pOldBrush = dc.SelectObject(&brush);
	rr.DeflateRect(6,3);
	dc.Rectangle(&rr);
	dc.SelectObject(pOldBrush);
	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	Font.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
	pOldFont = dc.SelectObject(&Font);
	dc.SetBkMode(TRANSPARENT);

	for(ii=1;ii<5;ii++) {
		nX=MulDiv(ii,rr.Width()-1,5)+rr.left;
		dc.MoveTo(nX,rr.top);
		dc.LineTo(nX,rr.top+3);
		Buff.Format(_T("%d"),ii);
		dc.TextOut(nX,rr.top,Buff);
	}
	dc.SelectObject(pOldFont);


	m_staticColor.GetWindowRect(&rr);
	ScreenToClient(&rr);
	dc.FillRect(&rr,&CBrush(theApp.m_LastSettings.rgbRulerLine));

	m_staticStyleLine.GetWindowRect(&rr);
	ScreenToClient(&rr);
	dc.SelectObject(pOldPen);
	dc.Rectangle(rr);
	dc.SelectObject(&pen1);
	rr.DeflateRect(3,3);
	dc.MoveTo(rr.TopLeft());
	dc.LineTo(rr.BottomRight());

	m_staticStyleBox.GetWindowRect(&rr);
	ScreenToClient(&rr);
	dc.SelectObject(pOldPen);
	dc.Rectangle(rr);
	dc.SelectObject(&pen1);
	rr.DeflateRect(3,3);
	dc.Rectangle(rr);

	m_staticStyleEllipse.GetWindowRect(&rr);
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


void CRulerPage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CMFCColorDialog dlg;
	CRect rr;

	m_staticWidth0.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) 	theApp.m_LastSettings.nRulerWidth = 1;

	m_staticWidth1.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) 	theApp.m_LastSettings.nRulerWidth = 2;
	m_staticWidth2.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) 	theApp.m_LastSettings.nRulerWidth = 3;
	m_staticWidth3.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) 	theApp.m_LastSettings.nRulerWidth = 4;

	m_staticColor.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbRulerLine=dlg.GetColor();
			Invalidate(FALSE);
		};
	}

	m_staticStyleLine.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) 	theApp.m_LastSettings.nRulerStyle = RULER_LINE_STYLE;
	m_staticStyleBox.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) 	theApp.m_LastSettings.nRulerStyle = RULER_RECTANGLE_STYLE;
	m_staticStyleEllipse.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) 	theApp.m_LastSettings.nRulerStyle = RULER_ELLIPSE_STYLE;

	CPropertyPage::OnLButtonDown(nFlags, point);
}

void CRulerPage::OnSelchangeListRulerText() 
{
	if(m_bScanUsesRotateAxis) {
		theApp.m_LastSettings.nRulerTextMode = m_listRulerText.GetCurSel()+5;
	}else{
		theApp.m_LastSettings.nRulerTextMode = m_listRulerText.GetCurSel();
	}
}



void CRulerPage::OnChangeEditScale() 
{
	CString Buff;

	m_editScale.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_LastSettings.fRulerScale);
}

void CRulerPage::OnSelchangeComboUnits() 
{
	theApp.m_LastSettings.nMeasurementUnits = m_comboUnits.GetCurSel();
	UpdateAllControls();
}


void CRulerPage::OnSelchangeComboFlagMode() 
{
	theApp.m_LastSettings.nRulerFlagMode = m_comboFlagMode.GetCurSel();
}
