// ListPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ListPropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListPropertiesDlg dialog


CListPropertiesDlg::CListPropertiesDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg, FeatureStruct* pFeature,CData* pData)
	: CDialog(CListPropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListPropertiesDlg)
	//}}AFX_DATA_INIT
	m_pFeature = pFeature;
	m_pDlg = pDlg;
	m_pParent = pParent;
	
	m_pData = pData;
}


void CListPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListPropertiesDlg)
	DDX_Control(pDX, IDC_COMBO_LANGUAGE, m_comboLanguage);
	DDX_Control(pDX, IDC_STATIC_BORDER_COLOR, m_staticBorderColor);
	DDX_Control(pDX, IDC_STATIC_BK_COLOR, m_staticBkColor);
	DDX_Control(pDX, IDC_EDIT_CAPTION, m_editCaption);
	DDX_Control(pDX, IDC_COMBO_WHICH_LIST, m_comboListType);
	DDX_Control(pDX, IDC_SPIN_BORDER_WIDTH, m_spinBorderWidth);
	DDX_Control(pDX, IDC_EDIT_BORDER_WIDTH, m_editBorderWidth);
	DDX_Control(pDX, IDC_COMBO_BORDER, m_comboBorder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListPropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CListPropertiesDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	ON_CBN_SELCHANGE(IDC_COMBO_WHICH_LIST, OnSelchangeComboWhichList)
	ON_CBN_SELCHANGE(IDC_COMBO_BORDER, OnSelchangeComboBorder)
	ON_EN_CHANGE(IDC_EDIT_CAPTION, OnChangeEditCaption)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_LANGUAGE, OnSelchangeComboLanguage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListPropertiesDlg message handlers

void CListPropertiesDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CListPropertiesDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CListPropertiesDlg::IDD]=FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CListPropertiesDlg::OnDestroy() 
{
	if (CListPropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CListPropertiesDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CListPropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString	Buff;

	if (CListPropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CListPropertiesDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}

	
	Buff.LoadString(IDS_None); m_comboBorder.AddString(Buff);
	Buff.LoadString(IDS_Raised); m_comboBorder.AddString(Buff);
	Buff.LoadString(IDS_Sunken); m_comboBorder.AddString(Buff);

	m_spinBorderWidth.SetRange(1,100);

	m_comboListType.AddString(L"Pr30");
	m_comboListType.AddString(L"Drawfting");
	m_comboListType.AddString((CString)"Pl15");
	Buff.LoadString(IDS_Component_Stats); m_comboListType.AddString(Buff);
	Buff.LoadString(IDS_Defect_List); m_comboListType.AddString(Buff);
	Buff.LoadString(IDS_Multiple_Results); m_comboListType.AddString(Buff);

	Buff.LoadString(IDS_English);	m_comboLanguage.AddString(Buff);
	Buff.LoadString(IDS_Display);	m_comboLanguage.AddString(Buff);

	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CListPropertiesDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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

void CListPropertiesDlg::UpdateAllControls()
{
	CString Buff;

	Buff.Format((CString)"%d",m_pFeature->nBorderWidth);
	m_editBorderWidth.SetWindowText(Buff);
	m_spinBorderWidth.SetPos(m_pFeature->nBorderWidth);
	m_comboListType.SetCurSel(m_pFeature->nListType);
	m_comboBorder.SetCurSel(m_pFeature->nBorderType);

	Buff.Format((CString)"%s",*m_pFeature->pCaption);
	m_editCaption.SetWindowText(Buff);

	m_comboLanguage.SetCurSel(m_pFeature->nLanguage);


}

void CListPropertiesDlg::OnButtonFont() 
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

void CListPropertiesDlg::OnSelchangeComboWhichList() 
{
	m_pFeature->nListType=m_comboListType.GetCurSel();
	m_pParent->Invalidate(FALSE);
	
}

void CListPropertiesDlg::OnSelchangeComboBorder() 
{
	m_pFeature->nBorderType=m_comboBorder.GetCurSel();
	m_pParent->Invalidate(FALSE);
	
}

void CListPropertiesDlg::OnChangeEditCaption() 
{
	CString Buff;

	m_editCaption.GetWindowText(Buff);
	m_pFeature->pCaption->Format((CString)"%s",Buff);
	m_pParent->Invalidate(FALSE);
	
}

void CListPropertiesDlg::OnLButtonDown(UINT nFlags, CPoint point) 
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


void CListPropertiesDlg::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);

}

void CListPropertiesDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	BlockFill(&dc,&m_staticBorderColor,m_pFeature->rgbBorderColor);
}

void CListPropertiesDlg::OnSelchangeComboLanguage() 
{
	m_pFeature->nLanguage = m_comboLanguage.GetCurSel();
	m_pParent->Invalidate(FALSE);
}
