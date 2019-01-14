// LinePropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "LinePropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLinePropertiesDlg dialog


CLinePropertiesDlg::CLinePropertiesDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg, FeatureStruct* pFeature)
	: CDialog(CLinePropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLinePropertiesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pFeature = pFeature;
	m_pDlg = pDlg;
	m_pParent = pParent;
	

}


void CLinePropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLinePropertiesDlg)
	DDX_Control(pDX, IDC_COMBO_LINE_STYLE, m_comboLineStyle);
	DDX_Control(pDX, IDC_STATIC_LINE_COLOR, m_staticColor);
	DDX_Control(pDX, IDC_SPIN_LINE_WIDTH, m_spinLineWidth);
	DDX_Control(pDX, IDC_EDIT_LINE_WIDTH, m_editLineWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLinePropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CLinePropertiesDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_STYLE, OnSelchangeComboLineStyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLinePropertiesDlg message handlers

void CLinePropertiesDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CLinePropertiesDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CLinePropertiesDlg::IDD]=FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CLinePropertiesDlg::OnDestroy() 
{
	if (CLinePropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CLinePropertiesDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
}

BOOL CLinePropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	Buff;

	if (CLinePropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CLinePropertiesDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}

	m_spinLineWidth.SetRange(1,100);
	Buff.LoadString(IDS_Solid);	m_comboLineStyle.AddString(Buff);
	Buff.LoadString(IDS_Dot); m_comboLineStyle.AddString(Buff);
	Buff.LoadString(IDS_Dashed); m_comboLineStyle.AddString(Buff);
	Buff.LoadString(IDS_Dash_Dot); m_comboLineStyle.AddString(Buff);
	Buff.LoadString(IDS_Dash_Dot_Dot); m_comboLineStyle.AddString(Buff);
	Buff.LoadString(IDS_Frame); m_comboLineStyle.AddString(Buff);


	UpdateAllControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLinePropertiesDlg::UpdateAllControls()
{
	CString Buff;

	Buff.Format((CString)"%d",m_pFeature->nLineWidth);
	m_editLineWidth.SetWindowText(Buff);
	m_spinLineWidth.SetPos(m_pFeature->nLineWidth);

	switch(m_pFeature->nLineStyle) {
	case PS_SOLID: m_comboLineStyle.SetCurSel(0);
		break;
	case PS_DOT: m_comboLineStyle.SetCurSel(1);
		break;
	case PS_DASH: m_comboLineStyle.SetCurSel(2);
		break;
	case PS_DASHDOT: m_comboLineStyle.SetCurSel(3);
		break;
	case PS_DASHDOTDOT: m_comboLineStyle.SetCurSel(4);
		break;
	case PS_INSIDEFRAME: m_comboLineStyle.SetCurSel(5);
		break;
	}

}

void CLinePropertiesDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CString	Buff;
	int	nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_LINE_WIDTH:
		nTemp=m_pFeature->nLineWidth;
		m_pFeature->nLineWidth=pSpin->GetPos()&0xffff;
		if(nTemp-m_pFeature->nLineWidth) {
			Buff.Format((CString)"%d",m_pFeature->nLineWidth);
			m_editLineWidth.SetWindowText(Buff);
			m_pParent->Invalidate(FALSE);
		}
		break;
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CLinePropertiesDlg::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);

}

void CLinePropertiesDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CMFCColorDialog dlg;

	m_staticColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			m_pFeature->rgbLine=dlg.GetColor();
			Invalidate(FALSE);
			m_pParent->Invalidate(FALSE);
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CLinePropertiesDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	BlockFill(&dc,&m_staticColor,m_pFeature->rgbLine);
}

void CLinePropertiesDlg::OnSelchangeComboLineStyle() 
{
	switch(m_comboLineStyle.GetCurSel()) {
	case 0: m_pFeature->nLineStyle = PS_SOLID;
		break;
	case 1: m_pFeature->nLineStyle = PS_DOT;
		break;
	case 2: m_pFeature->nLineStyle = PS_DASH;
		break;
	case 3: m_pFeature->nLineStyle = PS_DASHDOT;
		break;
	case 4: m_pFeature->nLineStyle = PS_DASHDOTDOT;
		break;
	case 5: m_pFeature->nLineStyle = PS_INSIDEFRAME;
		break;
	}
	m_pParent->Invalidate(FALSE);
}
