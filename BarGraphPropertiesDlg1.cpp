// BarGraphPropertiesDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "BarGraphPropertiesDlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBarGraphPropertiesDlg dialog


CBarGraphPropertiesDlg::CBarGraphPropertiesDlg(CWnd* pParent,CWnd** pDlg, FeatureStruct* pFeature,CData* pData)
	: CDialog(CBarGraphPropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBarGraphPropertiesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_pData = pData;
	m_pFeature = pFeature;
}


void CBarGraphPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarGraphPropertiesDlg)
	DDX_Control(pDX, IDC_COMBO_GATE, m_comboGate);
	DDX_Control(pDX, IDC_STATIC_BORDER_COLOR, m_staticBorderColor);
	DDX_Control(pDX, IDC_SPIN_BORDER_WIDTH, m_spinBorderWidth);
	DDX_Control(pDX, IDC_EDIT_BORDER_WIDTH, m_editBorderWidth);
	DDX_Control(pDX, IDC_COMBO_BORDER, m_comboBorder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBarGraphPropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CBarGraphPropertiesDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_BORDER_WIDTH, OnChangeEditBorderWidth)
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_BORDER, OnSelchangeComboBorder)
	ON_CBN_SELCHANGE(IDC_COMBO_GATE, OnSelchangeComboGate)
	ON_BN_CLICKED(IDC_BUTTON_GRATICULE_FONT, OnButtonGraticuleFont)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarGraphPropertiesDlg message handlers

void CBarGraphPropertiesDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CBarGraphPropertiesDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CBarGraphPropertiesDlg::IDD]=FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CBarGraphPropertiesDlg::OnDestroy() 
{
	if (CBarGraphPropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CBarGraphPropertiesDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
}

BOOL CBarGraphPropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString	Buff;

	if (CBarGraphPropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CBarGraphPropertiesDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
			theApp.ConfineToScreen(&rect);
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
		}
	}

	for(int nGate = 1;nGate<9;nGate++) {
		Buff.Format(_T("%d"),nGate);
		m_comboGate.AddString(Buff);
	}
	m_comboGate.AddString(_T("Thickness"));
	
	m_comboBorder.AddString(_T("None"));
	m_comboBorder.AddString(_T("Raised"));
	m_comboBorder.AddString(_T("Sunken"));

	m_spinBorderWidth.SetRange(1,100);
/*
	m_comboCaptionPosition.AddString("Don't Display");
	m_comboCaptionPosition.AddString("Top Left");
	m_comboCaptionPosition.AddString("Top Center");
	m_comboCaptionPosition.AddString("Top Right");
	m_comboCaptionPosition.AddString("Bottom Left");
	m_comboCaptionPosition.AddString("Bottom Center");
	m_comboCaptionPosition.AddString("Bottom Right");
*/
	
	UpdateAllControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBarGraphPropertiesDlg::UpdateAllControls()
{
	CString Buff;

	Buff.Format(_T("%d"),m_pFeature->nBorderWidth);
	m_editBorderWidth.SetWindowText(Buff);
	m_spinBorderWidth.SetPos(m_pFeature->nBorderWidth);
	m_comboGate.SetCurSel(m_pFeature->nImageNumber);
	m_comboBorder.SetCurSel(m_pFeature->nBorderType);

//	m_checkLeftBorder.SetCheck(m_pFeature->bLeftGraticule&1);
//	m_checkRightBorder.SetCheck(m_pFeature->bRightGraticule&1);
//	m_checkTopBorder.SetCheck(m_pFeature->bTopGraticule&1);
//	m_checkBottomBorder.SetCheck(m_pFeature->bBottomGraticule&1);

//	m_comboCaptionPosition.SetCurSel(m_pFeature->nCaptionPosition);

	Buff.Format(_T("%s"),*m_pFeature->pCaption);
//	m_editCaption.SetWindowText(Buff);


}

void CBarGraphPropertiesDlg::OnChangeEditBorderWidth() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CBarGraphPropertiesDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CString	Buff;
	int	nTemp;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_BORDER_WIDTH:
		nTemp=m_pFeature->nBorderWidth;
		m_pFeature->nBorderWidth=pSpin->GetPos()&0xffff;
		if(nTemp-m_pFeature->nBorderWidth) {
			Buff.Format(_T("%d"),m_pFeature->nBorderWidth);
			m_editBorderWidth.SetWindowText(Buff);
			m_pParent->Invalidate(FALSE);
		}
		break;
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CBarGraphPropertiesDlg::OnSelchangeComboBorder() 
{
	m_pFeature->nBorderType=m_comboBorder.GetCurSel();
	m_pParent->Invalidate(FALSE);
	
}

void CBarGraphPropertiesDlg::OnSelchangeComboGate() 
{
	m_pFeature->nImageNumber=m_comboGate.GetCurSel();
	m_pParent->Invalidate(FALSE);
	
}

void CBarGraphPropertiesDlg::OnButtonGraticuleFont() 
{
	CClientDC dc(this);
	CString Buff;

	CFontDialog dlg(&m_pFeature->lf);
	dlg.m_cf.rgbColors = m_pFeature->rgbText;
	if(dlg.DoModal() == IDOK) {
		dlg.GetCurrentFont(&m_pFeature->lf);
		dlg.GetCurrentFont(&theApp.m_LastSettings.lfReport);

		m_pFeature->rgbText = dlg.GetColor();

		m_pParent->Invalidate(FALSE);
	}
	
}

void CBarGraphPropertiesDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CMFCColorDialog dlg;

	m_staticBorderColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			m_pFeature->rgbBorderColor=dlg.GetColor();
			Invalidate(FALSE);
			m_pParent->Invalidate(FALSE);
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}


void CBarGraphPropertiesDlg::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);

}

void CBarGraphPropertiesDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	BlockFill(&dc,&m_staticBorderColor,m_pFeature->rgbBorderColor);
}
