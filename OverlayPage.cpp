// OverlayPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "OverlayPage.h"
#include "ViewSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COverlayPage property page

IMPLEMENT_DYNCREATE(COverlayPage, CPropertyPage)

COverlayPage::COverlayPage(CWnd* pParent) : CPropertyPage(COverlayPage::IDD)
{
	m_pParent = pParent;
	

	m_nLineType = 0;

	CViewSheet* pSheet = (CViewSheet*)m_pParent;
	m_pData = &pSheet->m_Data;
}

COverlayPage::~COverlayPage()
{
}

void COverlayPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COverlayPage)
	DDX_Control(pDX, IDC_COMBO_FLATTEN, m_comboFlatten);
	DDX_Control(pDX, IDC_CHECK_DISPLAY_CROSSES, m_checkDisplayCrosses);
	DDX_Control(pDX, IDC_STATIC_COLOR, m_staticColor);
	DDX_Control(pDX, IDC_COMBO_LINE_MODE, m_comboLineMode);
	DDX_Control(pDX, IDC_COMBO_LINE_TYPE, m_comboLineType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COverlayPage, CPropertyPage)
	//{{AFX_MSG_MAP(COverlayPage)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY_CROSSES, OnCheckDisplayCrosses)
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_TYPE, OnSelchangeComboLineType)
	ON_CBN_SELCHANGE(IDC_COMBO_LINE_MODE, OnSelchangeComboLineMode)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_COMBO_FLATTEN, OnSelchangeComboFlatten)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COverlayPage message handlers


BOOL COverlayPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;
	
	Buff.LoadString(IDS_Envelope);	m_comboLineType.AddString(Buff);
	Buff.LoadString(IDS_Ancillary);	m_comboLineType.AddString(Buff);

	Buff.LoadString(IDS_No_Lines);				m_comboLineMode.AddString(Buff);
	Buff.LoadString(IDS_Join_Vertices);			m_comboLineMode.AddString(Buff);
	Buff.LoadString(IDS_Join_First_to_Last);	m_comboLineMode.AddString(Buff);

	Buff.LoadString(IDS_Flatten_from_left);		m_comboFlatten.AddString(Buff);
	Buff.LoadString(IDS_Flatten_from_centre);	m_comboFlatten.AddString(Buff);
	Buff.LoadString(IDS_Flatten_from_right);	m_comboFlatten.AddString(Buff);
 	Buff.LoadString(IDS_Press_flat);			m_comboFlatten.AddString(Buff);
	Buff.LoadString(IDS_Flatten_from_vertical);	m_comboFlatten.AddString(Buff);

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COverlayPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	m_comboLineType.SetCurSel(m_nLineType);

	m_comboLineMode.SetCurSel( theApp.m_LastSettings.nOverlayLineMode[m_nLineType] );
	m_checkDisplayCrosses.SetCheck( theApp.m_LastSettings.bOverlayDisplayCrosses[m_nLineType] );
	m_comboFlatten.SetCurSel(m_pData->m_nFlattenMode);

}

void COverlayPage::OnSelchangeComboLineType() 
{

	m_nLineType = m_comboLineType.GetCurSel();
	UpdateAllControls();
	Invalidate(false);
}

void COverlayPage::OnCheckDisplayCrosses() 
{
	theApp.m_LastSettings.bOverlayDisplayCrosses[m_nLineType] = m_checkDisplayCrosses.GetCheck() & 0x01;
	InvalidateView();
}


void COverlayPage::OnSelchangeComboLineMode() 
{
	theApp.m_LastSettings.nOverlayLineMode[m_nLineType] = m_comboLineMode.GetCurSel();
	InvalidateView();
}

void COverlayPage::InvalidateView()
{

	CViewSheet* pSheet = (CViewSheet*)m_pParent;
	pSheet->InvalidateChild();

}

void COverlayPage::OnPaint() 
{
	CRect rr;
	CPaintDC dc(this); // device context for painting
	
	m_staticColor.GetWindowRect(&rr);
	ScreenToClient(&rr);
	dc.FillRect(&rr,&CBrush(theApp.m_LastSettings.rgbOverlayColor[m_nLineType]));
	
	// Do not call CPropertyPage::OnPaint() for painting messages
}

void COverlayPage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CMFCColorDialog dlg;

	m_staticColor.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbOverlayColor[m_nLineType]=dlg.GetColor();
			Invalidate(FALSE);
			InvalidateView();
		};
	}
	
	CPropertyPage::OnLButtonDown(nFlags, point);
}

void COverlayPage::OnSelchangeComboFlatten() 
{
	m_pData->m_nFlattenMode = m_comboFlatten.GetCurSel();
	m_pData->ChangeFlattenMode(m_pData->m_nFlattenMode);
	InvalidateView();
}

BOOL COverlayPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastViewOptionsTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	return CPropertyPage::OnSetActive();
}
