// EdgeEqualizerPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "EdgeEqualizerPage.h"
#include "ViewSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEdgeEqualizerPage property page

IMPLEMENT_DYNCREATE(CEdgeEqualizerPage, CPropertyPage)

CEdgeEqualizerPage::CEdgeEqualizerPage(CWnd* pViewSheet) : CPropertyPage(CEdgeEqualizerPage::IDD)
{
	//{{AFX_DATA_INIT(CEdgeEqualizerPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pViewSheet = pViewSheet;
	
	m_nEdge=0;
}

CEdgeEqualizerPage::~CEdgeEqualizerPage()
{
}

void CEdgeEqualizerPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEdgeEqualizerPage)
	DDX_Control(pDX, IDC_SLIDER_EDGE, m_sliderEdge);
	DDX_Control(pDX, IDC_SLIDER_EDGE_7, m_sliderEdge7);
	DDX_Control(pDX, IDC_SLIDER_EDGE_6, m_sliderEdge6);
	DDX_Control(pDX, IDC_SLIDER_EDGE_5, m_sliderEdge5);
	DDX_Control(pDX, IDC_SLIDER_EDGE_4, m_sliderEdge4);
	DDX_Control(pDX, IDC_SLIDER_EDGE_3, m_sliderEdge3);
	DDX_Control(pDX, IDC_SLIDER_EDGE_2, m_sliderEdge2);
	DDX_Control(pDX, IDC_SLIDER_EDGE_1, m_sliderEdge1);
	DDX_Control(pDX, IDC_SLIDER_EDGE_0, m_sliderEdge0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEdgeEqualizerPage, CPropertyPage)
	//{{AFX_MSG_MAP(CEdgeEqualizerPage)
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_ZERO, OnButtonZero)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdgeEqualizerPage message handlers


BOOL CEdgeEqualizerPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_sliderEdge0.SetRange(0,60);
	m_sliderEdge1.SetRange(0,60);
	m_sliderEdge2.SetRange(0,60);
	m_sliderEdge3.SetRange(0,60);
	m_sliderEdge4.SetRange(0,60);
	m_sliderEdge5.SetRange(0,60);
	m_sliderEdge6.SetRange(0,60);
	m_sliderEdge7.SetRange(0,60);

	m_sliderEdge.SetRange(0,60);


	
	UpdateAllControls();

	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CEdgeEqualizerPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void CEdgeEqualizerPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	m_sliderEdge0.SetPos(theApp.m_LastSettings.nImageEdge[0]+30);
	m_sliderEdge1.SetPos(theApp.m_LastSettings.nImageEdge[1]+30);
	m_sliderEdge2.SetPos(theApp.m_LastSettings.nImageEdge[2]+30);
	m_sliderEdge3.SetPos(theApp.m_LastSettings.nImageEdge[3]+30);
	m_sliderEdge4.SetPos(theApp.m_LastSettings.nImageEdge[4]+30);
	m_sliderEdge5.SetPos(theApp.m_LastSettings.nImageEdge[5]+30);
	m_sliderEdge6.SetPos(theApp.m_LastSettings.nImageEdge[6]+30);
	m_sliderEdge7.SetPos(theApp.m_LastSettings.nImageEdge[7]+30);

	m_sliderEdge.SetPos(m_nEdge+30);
}

void CEdgeEqualizerPage::OnButtonZero() 
{
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;
	int	ii;

	m_nEdge=0;
	for(ii=0;ii<8;ii++) {
		theApp.m_LastSettings.nImageEdge[ii]=0;
	}
	UpdateAllControls();
	pSheet->InvalidateChild();

}



void CEdgeEqualizerPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int	ii;
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SLIDER_EDGE_0:
		theApp.m_LastSettings.nImageEdge[0]=(pSlider->GetPos()&0xffff)-30;
		pSheet->InvalidateChild();
		break;
	case IDC_SLIDER_EDGE_1:
		theApp.m_LastSettings.nImageEdge[1]=(pSlider->GetPos()&0xffff)-30;
		pSheet->InvalidateChild();
		break;
	case IDC_SLIDER_EDGE_2:
		theApp.m_LastSettings.nImageEdge[2]=(pSlider->GetPos()&0xffff)-30;
		pSheet->InvalidateChild();
		break;
	case IDC_SLIDER_EDGE_3:
		theApp.m_LastSettings.nImageEdge[3]=(pSlider->GetPos()&0xffff)-30;
		pSheet->InvalidateChild();
		break;
	case IDC_SLIDER_EDGE_4:
		theApp.m_LastSettings.nImageEdge[4]=(pSlider->GetPos()&0xffff)-30;
		pSheet->InvalidateChild();
		break;
	case IDC_SLIDER_EDGE_5:
		theApp.m_LastSettings.nImageEdge[5]=(pSlider->GetPos()&0xffff)-30;
		pSheet->InvalidateChild();
		break;
	case IDC_SLIDER_EDGE_6:
		theApp.m_LastSettings.nImageEdge[6]=(pSlider->GetPos()&0xffff)-30;
		pSheet->InvalidateChild();
		break;
	case IDC_SLIDER_EDGE_7:
		theApp.m_LastSettings.nImageEdge[7]=(pSlider->GetPos()&0xffff)-30;
		pSheet->InvalidateChild();
		break;
	case IDC_SLIDER_EDGE:
		m_nEdge = (pSlider->GetPos()&0xffff)-30;
		for(ii=0;ii<8;ii++) {
			theApp.m_LastSettings.nImageEdge[ii]=(pSlider->GetPos()&0xffff)-30;
		}
		UpdateAllControls();
		pSheet->InvalidateChild();
		break;
	};
	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CEdgeEqualizerPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastViewOptionsTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}
