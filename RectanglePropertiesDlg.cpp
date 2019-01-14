// RectanglePropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "RectanglePropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRectanglePropertiesDlg dialog


CRectanglePropertiesDlg::CRectanglePropertiesDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg, FeatureStruct* pFeature)
	: CDialog(CRectanglePropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRectanglePropertiesDlg)
	//}}AFX_DATA_INIT
	m_pDlg = pDlg;
	m_pParent = pParent;
	
	m_pFeature = pFeature;
}


void CRectanglePropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRectanglePropertiesDlg)
	DDX_Control(pDX, IDC_COMBO_BORDER, m_comboBorder);
	DDX_Control(pDX, IDC_STATIC_LINE_COLOR, m_staticColor);
	DDX_Control(pDX, IDC_SPIN_LINE_WIDTH, m_spinLineWidth);
	DDX_Control(pDX, IDC_EDIT_LINE_WIDTH, m_editLineWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRectanglePropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CRectanglePropertiesDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_BORDER, OnSelchangeComboBorder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRectanglePropertiesDlg message handlers

void CRectanglePropertiesDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CRectanglePropertiesDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CRectanglePropertiesDlg::IDD]=FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CRectanglePropertiesDlg::OnDestroy() 
{
	if (CRectanglePropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CRectanglePropertiesDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CRectanglePropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString	Buff;

	if (CRectanglePropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CRectanglePropertiesDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}

	m_spinLineWidth.SetRange(1,100);

	Buff.LoadString(IDS_Solid); m_comboBorder.AddString(Buff);
	Buff.LoadString(IDS_Raised); m_comboBorder.AddString(Buff);
	Buff.LoadString(IDS_Sunken); m_comboBorder.AddString(Buff);
	Buff.LoadString(IDS_Filled); m_comboBorder.AddString(Buff);

	UpdateAllControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRectanglePropertiesDlg::UpdateAllControls()
{
	CString Buff;

	Buff.Format((CString)"%d",m_pFeature->nLineWidth);
	m_editLineWidth.SetWindowText(Buff);
	m_spinLineWidth.SetPos(m_pFeature->nLineWidth);

	m_comboBorder.SetCurSel(m_pFeature->nBorderType);

}

void CRectanglePropertiesDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CString	Buff;
	int	nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_LINE_WIDTH:
		nTemp=m_pFeature->nLineWidth;
		m_pFeature->nLineWidth=m_pFeature->nBorderWidth=pSpin->GetPos()&0xffff;
		if(nTemp-m_pFeature->nLineWidth) {
			Buff.Format((CString)"%d",m_pFeature->nLineWidth);
			m_editLineWidth.SetWindowText(Buff);
			m_pParent->Invalidate(FALSE);
		}
		break;
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CRectanglePropertiesDlg::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);

}

void CRectanglePropertiesDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CMFCColorDialog dlg;

	m_staticColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			m_pFeature->rgbBorderColor=m_pFeature->rgbLine=dlg.GetColor();
			Invalidate(FALSE);
			m_pParent->Invalidate(FALSE);
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CRectanglePropertiesDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	BlockFill(&dc,&m_staticColor,m_pFeature->rgbLine);
}


void CRectanglePropertiesDlg::OnSelchangeComboBorder() 
{

	m_pFeature->nBorderType = m_comboBorder.GetCurSel();
	m_pParent->Invalidate(FALSE);

}
