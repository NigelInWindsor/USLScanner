// IsometricPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "IsometricPage.h"
#include "3DViewEditDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIsometricPage property page

IMPLEMENT_DYNCREATE(CIsometricPage, CPropertyPage)

CIsometricPage::CIsometricPage(CWnd* pParent /*=NULL*/,CWnd* p3DViewEditDlg) : CPropertyPage(CIsometricPage::IDD)
{
	//{{AFX_DATA_INIT(CIsometricPage)
	//}}AFX_DATA_INIT

	m_p3DViewEditDlg = p3DViewEditDlg;
	m_pParent = pParent;
	

	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(pFrame->m_pViewSheet[0]!=NULL) {
		m_pData = &pFrame->m_pViewSheet[0]->m_Data;
	} else {
		m_pData = NULL;
	}
	m_nNumberImages=0;
}

CIsometricPage::~CIsometricPage()
{
}

void CIsometricPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIsometricPage)
	DDX_Control(pDX, IDC_SPIN_VERTICAL, m_spinVerticalExaggeration);
	DDX_Control(pDX, IDC_EDIT_VERTICAL, m_editVerticalExaggeration);
	DDX_Control(pDX, IDC_EDIT_DECIMATE_SAMPLES, m_editDecimateSamples);
	DDX_Control(pDX, IDC_EDIT_DECIMATE_LINES, m_editDecimateLines);
	DDX_Control(pDX, IDC_COMBO_DENSITY, m_comboDensity);
	DDX_Control(pDX, IDC_STATIC_MESH_COLOR, m_staticMeshColor);
	DDX_Control(pDX, IDC_CHECK_MESH, m_checkMesh);
	DDX_Control(pDX, IDC_COMBO_IMAGE, m_comboImage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIsometricPage, CPropertyPage)
	//{{AFX_MSG_MAP(CIsometricPage)
	ON_CBN_SELCHANGE(IDC_COMBO_IMAGE, OnSelchangeComboImage)
	ON_BN_CLICKED(IDC_CHECK_MESH, OnCheckMesh)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_CBN_SELCHANGE(IDC_COMBO_DENSITY, OnSelchangeComboDensity)
	ON_EN_CHANGE(IDC_EDIT_DECIMATE_LINES, OnChangeEditDecimateLines)
	ON_EN_CHANGE(IDC_EDIT_DECIMATE_SAMPLES, OnChangeEditDecimateSamples)
	ON_EN_CHANGE(IDC_EDIT_VERTICAL, OnChangeEditVertical)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIsometricPage message handlers

BOOL CIsometricPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff,Temp;

	m_nNumberImages=0;
	if(m_pData) {
		for(int tt=0;tt<NUMBER_BUFFERS;tt++) {
			if(m_pData->m_nTimeBuffer[tt]!=NULL) {
				Buff.LoadString(IDS_Thickness);
				Temp.Format(_T(" %d"),m_nNumberImages+1);
				m_comboImage.AddString(Buff + Temp);
				m_nNumberImages++;
			}
		}
	}
	if(m_nNumberImages==0) {
		Buff.LoadString(IDS_No_Thickness_Cscans);	m_comboImage.AddString(Buff);
		m_comboImage.SetCurSel(0);
	}
	m_comboImage.SetCurSel(0);

	for(int ii=1;ii<20;ii++) {
		Buff.Format(_T("%d"),ii);
		m_comboDensity.AddString(Buff);
	}


	Buff.Format(_T("%0.1f"),theApp.m_LastSettings.fIsometricVerticalExaggeration);
	m_editVerticalExaggeration.SetWindowText(Buff);
	m_spinVerticalExaggeration.SetRange(1,10);
	m_spinVerticalExaggeration.SetPos((int)theApp.m_LastSettings.fIsometricVerticalExaggeration);

	m_spinVerticalExaggeration.EnableWindow(FALSE);
	m_spinVerticalExaggeration.ModifyStyle(WS_VISIBLE,0); //I thinks its better without this control

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIsometricPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	m_checkMesh.SetCheck(theApp.m_LastSettings.nIsometricOverlayMesh&1);
	m_comboDensity.SetCurSel(theApp.m_LastSettings.nIsometricMeshDensity);

	Buff.Format(_T("%d"),theApp.m_LastSettings.nIsometricDecimateLines);
	m_editDecimateLines.SetWindowText(Buff);
	Buff.Format(_T("%d"),theApp.m_LastSettings.nIsometricDecimateSamples);
	m_editDecimateSamples.SetWindowText(Buff);


}

void CIsometricPage::OnSelchangeComboImage() 
{
	C3DViewEditDlg* pDlg = (C3DViewEditDlg*)m_p3DViewEditDlg;

	pDlg->m_nIsometricImageNumber = m_comboImage.GetCurSel();
	pDlg->RefreshView();
}

BOOL CIsometricPage::OnSetActive() 
{
	theApp.m_LastSettings.nLast3DDrawingToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

void CIsometricPage::OnCheckMesh() 
{
	C3DViewEditDlg* pDlg = (C3DViewEditDlg*)m_p3DViewEditDlg;

	theApp.m_LastSettings.nIsometricOverlayMesh = m_checkMesh.GetCheck() & 1;

	pDlg->RefreshView();
}

void CIsometricPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rr;

	BlockFill(&dc,&m_staticMeshColor,theApp.m_LastSettings.rgbIsometricMeshColor);
}

void CIsometricPage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CMFCColorDialog dlg;

	m_staticMeshColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if(rr.PtInRect(point)) {
		if(dlg.DoModal()==IDOK) {
			theApp.m_LastSettings.rgbIsometricMeshColor=dlg.GetColor();
			Invalidate(FALSE);
		}
	}
	
	CPropertyPage::OnLButtonDown(nFlags, point);
}

void CIsometricPage::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr,&Brush);

}

void CIsometricPage::OnSelchangeComboDensity() 
{
	C3DViewEditDlg* pDlg = (C3DViewEditDlg*)m_p3DViewEditDlg;

	theApp.m_LastSettings.nIsometricMeshDensity = m_comboDensity.GetCurSel() + 1;
	pDlg->RefreshView();
	
}

void CIsometricPage::OnChangeEditDecimateLines() 
{
	CString Buff;

	m_editDecimateLines.GetWindowText(Buff);
	theApp.m_LastSettings.nIsometricDecimateLines = _ttoi(Buff);
}

void CIsometricPage::OnChangeEditDecimateSamples() 
{
	CString Buff;

	m_editDecimateSamples.GetWindowText(Buff);
	theApp.m_LastSettings.nIsometricDecimateSamples = _ttoi(Buff);
}

void CIsometricPage::OnChangeEditVertical() 
{
	CString Buff;
	C3DViewEditDlg* pDlg = (C3DViewEditDlg*)m_p3DViewEditDlg;

	m_editVerticalExaggeration.GetWindowText(Buff);

	_WTOF(Buff,theApp.m_LastSettings.fIsometricVerticalExaggeration);
	m_spinVerticalExaggeration.SetPos((int)theApp.m_LastSettings.fIsometricVerticalExaggeration);
//	pDlg->RefreshView();

	
}

void CIsometricPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	CString	Buff;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_VERTICAL:
		theApp.m_LastSettings.fIsometricVerticalExaggeration = (float)(pSpin->GetPos()&0xffff);
		Buff.Format(_T("%0.1f"),theApp.m_LastSettings.fIsometricVerticalExaggeration);
		m_editVerticalExaggeration.SetWindowText(Buff);
		break;
	};


	CPropertyPage::OnVScroll(nSBCode, nPos, pScrollBar);

}
