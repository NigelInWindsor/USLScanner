// PAEdgeEqualizerPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "PAEdgeEqualizerPage.h"
#include "ViewSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPAEdgeEqualizerPage property page

IMPLEMENT_DYNCREATE(CPAEdgeEqualizerPage, CPropertyPage)

CPAEdgeEqualizerPage::CPAEdgeEqualizerPage(CWnd* pViewSheet) : CPropertyPage(CPAEdgeEqualizerPage::IDD)
{
	//{{AFX_DATA_INIT(CPAEdgeEqualizerPage)
	//}}AFX_DATA_INIT
	m_pViewSheet = pViewSheet;
	
}

CPAEdgeEqualizerPage::~CPAEdgeEqualizerPage()
{
}

void CPAEdgeEqualizerPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPAEdgeEqualizerPage)
	DDX_Control(pDX, IDC_SLIDER_LATERAL_ROTATION, m_sliderLatRot);
	DDX_Control(pDX, IDC_SLIDER_DIR, m_sliderDirection);
	DDX_Control(pDX, IDC_SLIDER_BEAM_3, m_sliderBeam3);
	DDX_Control(pDX, IDC_SLIDER_BEAM_2, m_sliderBeam2);
	DDX_Control(pDX, IDC_SLIDER_BEAM_1, m_sliderBeam1);
	DDX_Control(pDX, IDC_SLIDER_BEAM_0, m_sliderBeam0);
	DDX_Control(pDX, IDC_EDIT_BEAM_4, m_editBeam4);
	DDX_Control(pDX, IDC_EDIT_BEAM_3, m_editBeam3);
	DDX_Control(pDX, IDC_EDIT_BEAM_2, m_editBeam2);
	DDX_Control(pDX, IDC_EDIT_BEAM_1, m_editBeam1);
	DDX_Control(pDX, IDC_EDIT_BEAM_0, m_editBeam0);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPAEdgeEqualizerPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPAEdgeEqualizerPage)
	ON_EN_CHANGE(IDC_EDIT_BEAM_1, OnChangeEditBeam1)
	ON_EN_CHANGE(IDC_EDIT_BEAM_2, OnChangeEditBeam2)
	ON_EN_CHANGE(IDC_EDIT_BEAM_3, OnChangeEditBeam3)
	ON_EN_CHANGE(IDC_EDIT_BEAM_4, OnChangeEditBeam4)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPAEdgeEqualizerPage message handlers

BOOL CPAEdgeEqualizerPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_sliderBeam0.SetRange(0,100);
	m_sliderBeam1.SetRange(0,100);
	m_sliderBeam2.SetRange(0,100);
	m_sliderBeam3.SetRange(0,100);
	m_sliderDirection.SetRange(0,100);
	m_sliderLatRot.SetRange(0,100);

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPAEdgeEqualizerPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	Buff.Format(_T("%d"),(theApp.m_LastSettings.nPAEdgeLine[0]=0) + 1);
	m_editBeam0.SetWindowText(Buff);
	Buff.Format(_T("%d"),theApp.m_LastSettings.nPAEdgeLine[1] + 1);
	m_editBeam1.SetWindowText(Buff);
	Buff.Format(_T("%d"),theApp.m_LastSettings.nPAEdgeLine[2] + 1);
	m_editBeam2.SetWindowText(Buff);
	Buff.Format(_T("%d"),theApp.m_LastSettings.nPAEdgeLine[3] + 1);
	m_editBeam3.SetWindowText(Buff);
	Buff.Format(_T("%d"),theApp.m_LastSettings.nPAEdgeLine[4] + 1);
	m_editBeam4.SetWindowText(Buff);

	theApp.m_LastSettings.nPAEdgeOffset[0]=0;
	m_sliderBeam0.SetPos(theApp.m_LastSettings.nPAEdgeOffset[1] + 50);
	m_sliderBeam1.SetPos(theApp.m_LastSettings.nPAEdgeOffset[2] + 50);
	m_sliderBeam2.SetPos(theApp.m_LastSettings.nPAEdgeOffset[3] + 50);
	m_sliderBeam3.SetPos(theApp.m_LastSettings.nPAEdgeOffset[4] + 50);
	m_sliderDirection.SetPos(theApp.m_LastSettings.nPAEdgeDir + 50);
	m_sliderLatRot.SetPos(theApp.m_LastSettings.nPALatRot + 50);

}

void CPAEdgeEqualizerPage::OnChangeEditBeam1() 
{
	CString Buff;

	m_editBeam1.GetWindowText(Buff);
	theApp.m_LastSettings.nPAEdgeLine[1] = _ttoi(Buff) - 1;
}

void CPAEdgeEqualizerPage::OnChangeEditBeam2() 
{
	CString Buff;

	m_editBeam2.GetWindowText(Buff);
	theApp.m_LastSettings.nPAEdgeLine[2] = _ttoi(Buff) - 1;
}

void CPAEdgeEqualizerPage::OnChangeEditBeam3() 
{
	CString Buff;

	m_editBeam3.GetWindowText(Buff);
	theApp.m_LastSettings.nPAEdgeLine[3] = _ttoi(Buff) - 1;
}

void CPAEdgeEqualizerPage::OnChangeEditBeam4() 
{
	CString Buff;

	m_editBeam4.GetWindowText(Buff);
	theApp.m_LastSettings.nPAEdgeLine[4] = _ttoi(Buff) - 1;
}

void CPAEdgeEqualizerPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;

	switch(pSlider->GetDlgCtrlID()) {
	case IDC_SLIDER_BEAM_0:
		theApp.m_LastSettings.nPAEdgeOffset[1]=(pSlider->GetPos()&0xffff)-50;
		pSheet->InvalidateChild();
		break;
	case IDC_SLIDER_BEAM_1:
		theApp.m_LastSettings.nPAEdgeOffset[2]=(pSlider->GetPos()&0xffff)-50;
		pSheet->InvalidateChild();
		break;
	case IDC_SLIDER_BEAM_2:
		theApp.m_LastSettings.nPAEdgeOffset[3]=(pSlider->GetPos()&0xffff)-50;
		pSheet->InvalidateChild();
		break;
	case IDC_SLIDER_BEAM_3:
		theApp.m_LastSettings.nPAEdgeOffset[4]=(pSlider->GetPos()&0xffff)-50;
		pSheet->InvalidateChild();
		break;
	case IDC_SLIDER_DIR:
		theApp.m_LastSettings.nPAEdgeDir=(pSlider->GetPos()&0xffff)-50;
		pSheet->InvalidateChild();
		break;
	case IDC_SLIDER_LATERAL_ROTATION:
		theApp.m_LastSettings.nPALatRot=(pSlider->GetPos()&0xffff)-50;
		pSheet->InvalidateChild();
		break;
	};
	
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CPAEdgeEqualizerPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastViewOptionsTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}
