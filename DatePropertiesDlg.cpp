// DatePropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "DatePropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDatePropertiesDlg dialog


CDatePropertiesDlg::CDatePropertiesDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,FeatureStruct* pFeature,CData* pData)
	: CDialog(CDatePropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDatePropertiesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_pDlg = pDlg;
	m_pFeature = pFeature;
	m_pData = pData;
	

}


void CDatePropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDatePropertiesDlg)
	DDX_Control(pDX, IDC_EDIT_CAPTION, m_editCaption);
	DDX_Control(pDX, IDC_CHECK_CAPTION_RIGHT, m_checkCaptionRight);
	DDX_Control(pDX, IDC_CHECK_CAPTION_LEFT, m_checkCaptionLeft);
	DDX_Control(pDX, IDC_COMBO_WHICH_DATE, m_comboWhichDate);
	DDX_Control(pDX, IDC_COMBO_DATE_FORMAT, m_comboDateFormat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDatePropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CDatePropertiesDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_DATE_FORMAT, OnSelchangeComboDateFormat)
	ON_CBN_SELCHANGE(IDC_COMBO_WHICH_DATE, OnSelchangeComboWhichDate)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	ON_BN_CLICKED(IDC_CHECK_CAPTION_LEFT, OnCheckCaptionLeft)
	ON_BN_CLICKED(IDC_CHECK_CAPTION_RIGHT, OnCheckCaptionRight)
	ON_EN_CHANGE(IDC_EDIT_CAPTION, OnChangeEditCaption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDatePropertiesDlg message handlers

void CDatePropertiesDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CDatePropertiesDlg::OnClose() 
{
	theApp.m_LastSettings.bWindowOpenOnLastExit[CDatePropertiesDlg::IDD]=FALSE;
	
	CDialog::OnClose();
	DestroyWindow();
}

void CDatePropertiesDlg::OnDestroy() 
{
	if (CDatePropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		GetWindowRect(theApp.m_LastSettings.RectWindowPos[CDatePropertiesDlg::IDD]);
	}
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;	
	
}

BOOL CDatePropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString	Buff;

	if (CDatePropertiesDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[CDatePropertiesDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOSIZE);
	}
	
	m_comboDateFormat.AddString((CString)"DD/MM/YYYY");
	m_comboDateFormat.AddString((CString)"MM/DD/YYYY");
	m_comboDateFormat.AddString((CString)"DD/MM/YY");
	m_comboDateFormat.AddString((CString)"MM/YY/YY");
	m_comboDateFormat.AddString((CString)"DD/MONTH/YYYY");
	m_comboDateFormat.AddString((CString)"MONTH/DD/YYYY");
	m_comboDateFormat.AddString((CString)"DD/MM/YYYY HHhMM");
	m_comboDateFormat.AddString((CString)"MM/DD/YYYY HHhMM");
	m_comboDateFormat.AddString((CString)"DD/MM/YY HHhMM");
	m_comboDateFormat.AddString((CString)"MM/YY/YY HHhMM");
	m_comboDateFormat.AddString((CString)"DD/MONTH/YYYY HHhMM");
	m_comboDateFormat.AddString((CString)"MONTH/DD/YYYY HHhMM");

	Buff.LoadString(IDS_Scan); m_comboWhichDate.AddString(Buff);
	Buff.LoadString(IDS_Todays); m_comboWhichDate.AddString(Buff);


	UpdateAllControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDatePropertiesDlg::UpdateAllControls()
{
	CString Buff;

	m_comboDateFormat.SetCurSel(m_pFeature->nDateFormat);
	m_comboWhichDate.SetCurSel(m_pFeature->nWhichDate);
	switch(m_pFeature->nCaptionPosition) {
	case 0:
		m_checkCaptionLeft.SetCheck(TRUE);
		m_checkCaptionRight.SetCheck(FALSE);
		break;
	case 1:
		m_checkCaptionLeft.SetCheck(FALSE);
		m_checkCaptionRight.SetCheck(TRUE);
		break;
	}
	Buff.Format((CString)"%s",*m_pFeature->pCaption);
	m_editCaption.SetWindowText(Buff);


}

void CDatePropertiesDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CDatePropertiesDlg::OnSelchangeComboDateFormat() 
{
	m_pFeature->nDateFormat=m_comboDateFormat.GetCurSel();
	m_pParent->Invalidate(FALSE);
	
}

void CDatePropertiesDlg::OnSelchangeComboWhichDate() 
{
	m_pFeature->nWhichDate=m_comboWhichDate.GetCurSel();
	m_pParent->Invalidate(FALSE);
}

void CDatePropertiesDlg::OnButtonFont() 
{
	CClientDC dc(this);

	CFontDialog dlg(&m_pFeature->lf);
	dlg.m_cf.rgbColors = m_pFeature->rgbText;
	if(dlg.DoModal() == IDOK) {
		dlg.GetCurrentFont(&m_pFeature->lf);
		m_pFeature->rgbText = dlg.GetColor();
		m_pParent->Invalidate(FALSE);
	}
	
}

void CDatePropertiesDlg::OnCheckCaptionLeft() 
{
	m_pFeature->nCaptionPosition=0;
	UpdateAllControls();
	m_pParent->Invalidate(FALSE);
	
}

void CDatePropertiesDlg::OnCheckCaptionRight() 
{
	m_pFeature->nCaptionPosition=1;
	UpdateAllControls();
	m_pParent->Invalidate(FALSE);
	
}

void CDatePropertiesDlg::OnChangeEditCaption() 
{
	CString Buff;

	m_editCaption.GetWindowText(Buff);
	m_pFeature->pCaption->Format((CString)"%s",Buff);
	m_pParent->Invalidate(FALSE);
	
}
