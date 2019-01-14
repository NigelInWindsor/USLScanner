// ImagePropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ImagePropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImagePropertiesDlg dialog


CImagePropertiesDlg::CImagePropertiesDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg, FeatureStruct* pFeature,CData* pData)
	: CDialog(CImagePropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLinePropertiesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pFeature = pFeature;
	m_pDlg = pDlg;
	m_pParent = pParent;
	
	m_pData = pData;
}


void CImagePropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImagePropertiesDlg)
	DDX_Control(pDX, IDC_COMBO_SCALE, m_comboScale);
	DDX_Control(pDX, IDC_COMBO_CAPTION_POSITION, m_comboCaptionPosition);
	DDX_Control(pDX, IDC_EDIT_CAPTION, m_editCaption);
	DDX_Control(pDX, IDC_CHECK_TOP, m_checkTopBorder);
	DDX_Control(pDX, IDC_CHECK_RIGHT, m_checkRightBorder);
	DDX_Control(pDX, IDC_CHECK_LEFT, m_checkLeftBorder);
	DDX_Control(pDX, IDC_CHECK_BOTTOM, m_checkBottomBorder);
	DDX_Control(pDX, IDC_STATIC_BORDER_COLOR, m_staticBorderColor);
	DDX_Control(pDX, IDC_SPIN_BORDER_WIDTH, m_spinBorderWidth);
	DDX_Control(pDX, IDC_EDIT_BORDER_WIDTH, m_editBorderWidth);
	DDX_Control(pDX, IDC_COMBO_BORDER, m_comboBorder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImagePropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CImagePropertiesDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_BORDER, OnSelchangeComboBorder)
	ON_EN_CHANGE(IDC_EDIT_BORDER_WIDTH, OnChangeEditBorderWidth)
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_GRATICULE_FONT, OnButtonGraticuleFont)
	ON_BN_CLICKED(IDC_CHECK_BOTTOM, OnCheckBottom)
	ON_BN_CLICKED(IDC_CHECK_LEFT, OnCheckLeft)
	ON_BN_CLICKED(IDC_CHECK_RIGHT, OnCheckRight)
	ON_BN_CLICKED(IDC_CHECK_TOP, OnCheckTop)
	ON_EN_CHANGE(IDC_EDIT_CAPTION, OnChangeEditCaption)
	ON_BN_CLICKED(IDC_CHECK_CAPTION_RIGHT, OnCheckCaptionRight)
	ON_CBN_SELCHANGE(IDC_COMBO_CAPTION_POSITION, OnSelchangeComboCaptionPosition)
	ON_CBN_SELCHANGE(IDC_COMBO_SCALE, OnSelchangeComboScale)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImagePropertiesDlg message handlers

void CImagePropertiesDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CImagePropertiesDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CImagePropertiesDlg::IDD]=FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CImagePropertiesDlg::OnDestroy() 
{
	if (CImagePropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CImagePropertiesDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
}

BOOL CImagePropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	Buff;

	if (CImagePropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CImagePropertiesDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}

	Buff.LoadString(IDS_None); m_comboBorder.AddString(Buff);
	Buff.LoadString(IDS_Raised); m_comboBorder.AddString(Buff);
	Buff.LoadString(IDS_Sunken); m_comboBorder.AddString(Buff);

	m_spinBorderWidth.SetRange(1,100);

	Buff.LoadString(IDS_Dont_Display); m_comboCaptionPosition.AddString(Buff);
	Buff.LoadString(IDS_Top_Left); m_comboCaptionPosition.AddString(Buff);
	Buff.LoadString(IDS_Top_Centre); m_comboCaptionPosition.AddString(Buff);
	Buff.LoadString(IDS_Top_Right); m_comboCaptionPosition.AddString(Buff);
	Buff.LoadString(IDS_Bottom_Left); m_comboCaptionPosition.AddString(Buff);
	Buff.LoadString(IDS_Bottom_Centre); m_comboCaptionPosition.AddString(Buff);
	Buff.LoadString(IDS_Bottom_Right); m_comboCaptionPosition.AddString(Buff);

	Buff.LoadString(IDS_Dont_Display); m_comboScale.AddString(Buff);
	Buff.LoadString(IDS_Top_Centre); m_comboScale.AddString(Buff);
	Buff.LoadString(IDS_Bottom_Centre); m_comboScale.AddString(Buff);
	Buff.LoadString(IDS_Top_Right); m_comboScale.AddString(Buff);
	m_comboScale.SetCurSel(m_pFeature->nScalePosition);

	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CImagePropertiesDlg::UpdateAllControls()
{
	CString Buff;

	Buff.Format((CString)"%d",m_pFeature->nBorderWidth);
	m_editBorderWidth.SetWindowText(Buff);
	m_spinBorderWidth.SetPos(m_pFeature->nBorderWidth);
	m_comboBorder.SetCurSel(m_pFeature->nBorderType);

	m_checkLeftBorder.SetCheck(m_pFeature->bLeftGraticule&1);
	m_checkRightBorder.SetCheck(m_pFeature->bRightGraticule&1);
	m_checkTopBorder.SetCheck(m_pFeature->bTopGraticule&1);
	m_checkBottomBorder.SetCheck(m_pFeature->bBottomGraticule&1);

	m_comboCaptionPosition.SetCurSel(m_pFeature->nCaptionPosition);

	Buff.Format((CString)"%s",*m_pFeature->pCaption);
	m_editCaption.SetWindowText(Buff);


}

void CImagePropertiesDlg::OnSelchangeComboBorder() 
{
	m_pFeature->nBorderType=m_comboBorder.GetCurSel();
	m_pParent->Invalidate(FALSE);
}

void CImagePropertiesDlg::OnChangeEditBorderWidth() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}


void CImagePropertiesDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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
			Buff.Format((CString)"%d",m_pFeature->nBorderWidth);
			m_editBorderWidth.SetWindowText(Buff);
			m_pParent->Invalidate(FALSE);
		}
		break;
	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CImagePropertiesDlg::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);

}


void CImagePropertiesDlg::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CImagePropertiesDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	BlockFill(&dc,&m_staticBorderColor,m_pFeature->rgbBorderColor);
}

void CImagePropertiesDlg::OnButtonGraticuleFont() 
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

void CImagePropertiesDlg::OnCheckBottom() 
{
	m_pFeature->bBottomGraticule=m_checkBottomBorder.GetCheck()&1;
	m_pParent->Invalidate(FALSE);
}

void CImagePropertiesDlg::OnCheckLeft() 
{
	m_pFeature->bLeftGraticule=m_checkLeftBorder.GetCheck()&1;	
	m_pParent->Invalidate(FALSE);
}

void CImagePropertiesDlg::OnCheckRight() 
{
	m_pFeature->bRightGraticule=m_checkRightBorder.GetCheck()&1;	
	m_pParent->Invalidate(FALSE);
}

void CImagePropertiesDlg::OnCheckTop() 
{
	m_pFeature->bTopGraticule=m_checkTopBorder.GetCheck()&1;	
	m_pParent->Invalidate(FALSE);
}

void CImagePropertiesDlg::OnChangeEditCaption() 
{
	CString Buff;

	m_editCaption.GetWindowText(Buff);
	m_pFeature->pCaption->Format((CString)"%s",Buff);
	m_pParent->Invalidate(FALSE);
	
}

void CImagePropertiesDlg::OnCheckCaptionBottom() 
{
	// TODO: Add your control notification handler code here
	
}

void CImagePropertiesDlg::OnCheckCaptionTop() 
{
	// TODO: Add your control notification handler code here
	
}

void CImagePropertiesDlg::OnCheckCaptionCenter() 
{
	// TODO: Add your control notification handler code here
	
}

void CImagePropertiesDlg::OnCheckCaptionRight() 
{
	// TODO: Add your control notification handler code here
	
}

void CImagePropertiesDlg::OnCheckDisplayFilename() 
{
	// TODO: Add your control notification handler code here
	
}

void CImagePropertiesDlg::OnSelchangeComboCaptionPosition() 
{

	m_pFeature->nCaptionPosition = m_comboCaptionPosition.GetCurSel();
	m_pParent->Invalidate(FALSE);
}

void CImagePropertiesDlg::OnSelchangeComboScale() 
{
	m_pFeature->nScalePosition = m_comboScale.GetCurSel();
	m_pParent->Invalidate(FALSE);
	
}
