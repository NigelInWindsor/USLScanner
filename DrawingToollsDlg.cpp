// DrawingToollsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "DrawingToollsDlg.h"
#include "3DViewEditDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawingToollsDlg dialog
IMPLEMENT_DYNCREATE(CDrawingToollsDlg, CPropertyPage)


CDrawingToollsDlg::CDrawingToollsDlg(CWnd* pParent /*=NULL*/,CWnd* p3DViewEditDlg)
	: CPropertyPage(CDrawingToollsDlg::IDD)
{
	//{{AFX_DATA_INIT(CDrawingToollsDlg)
	//}}AFX_DATA_INIT
	
	m_p3DViewEditDlg = p3DViewEditDlg;
}

CDrawingToollsDlg::~CDrawingToollsDlg()
{
}

void CDrawingToollsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawingToollsDlg)
	DDX_Control(pDX, IDC_CHECK_MAINTAIN_ASPECT_RATIO, m_checkMaintainRefPts);
	DDX_Control(pDX, IDC_CHECK_INCLUDE_REF_PTS, m_checkIncludeRefPts);
	DDX_Control(pDX, IDC_CHECK_FIRSTLAST, m_checkFirstLast);
	DDX_Control(pDX, IDC_COMBO_WHICH_LINES, m_comboWhichLines);
	DDX_Control(pDX, IDC_CHECK_XTIP, m_checkNormals);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDrawingToollsDlg, CDialog)
	//{{AFX_MSG_MAP(CDrawingToollsDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_WHICH_LINES, OnSelchangeComboWhichLines)
	ON_BN_CLICKED(IDC_CHECK_XTIP, OnCheckXtip)
	ON_BN_CLICKED(IDC_CHECK_FIRSTLAST, OnCheckFirstlast)
	ON_BN_CLICKED(IDC_CHECK_MAINTAIN_ASPECT_RATIO, OnCheckMaintainAspectRatio)
	ON_BN_CLICKED(IDC_CHECK_INCLUDE_REF_PTS, OnCheckIncludeRefPts)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingToollsDlg message handlers

BOOL CDrawingToollsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;

	Buff.LoadString(IDS_All_Lines);	m_comboWhichLines.AddString(Buff);
	Buff.LoadString(IDS_Current_Line);	m_comboWhichLines.AddString(Buff);

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDrawingToollsDlg::OnSelchangeComboWhichLines() 
{
	theApp.m_LastSettings.nDisplayEditWhichLines = m_comboWhichLines.GetCurSel();
}

void CDrawingToollsDlg::OnCheckXtip() 
{
	theApp.m_LastSettings.nDisplayEditNormals = m_checkNormals.GetCheck() & 1;
}



void CDrawingToollsDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	m_checkNormals.SetCheck(theApp.m_LastSettings.nDisplayEditNormals&1);
	m_checkFirstLast.SetCheck(theApp.m_LastSettings.nDisplayEditFirstLastCurve&1);

	m_comboWhichLines.SetCurSel(theApp.m_LastSettings.nDisplayEditWhichLines);

	m_checkIncludeRefPts.SetCheck(theApp.m_LastSettings.bIncludeRefPtsInScaling);
	m_checkMaintainRefPts.SetCheck(theApp.m_LastSettings.bMaintainAspectRaioInScaling);
}

void CDrawingToollsDlg::OnCheckFirstlast() 
{
	theApp.m_LastSettings.nDisplayEditFirstLastCurve = m_checkFirstLast.GetCheck() & 1;
}



void CDrawingToollsDlg::Update3DEditView(int nAction)
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
//	C3DViewEditDlg* p3DView = (C3DViewEditDlg*)m_p3DViewEditDlg;


//	if(nAction & FILLLIST) p3DView->FillList();
//	if(nAction & PAINT) p3DView->SendMessage(WM_PAINT);
//	if(nAction & INVALIDATE) p3DView->Invalidate(FALSE);

	if(pFrame->m_pProfileSheet) {
//		pFrame->m_p3DViewEditDlg->FillList();
		pFrame->m_pProfileSheet->Invalidate(FALSE);
	}

}

BOOL CDrawingToollsDlg::OnSetActive() 
{
	theApp.m_LastSettings.nLast3DDrawingToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

void CDrawingToollsDlg::OnCheckMaintainAspectRatio() 
{
	theApp.m_LastSettings.bMaintainAspectRaioInScaling = m_checkMaintainRefPts.GetCheck() & 0x01;
	Update3DEditView(0);
}

void CDrawingToollsDlg::OnCheckIncludeRefPts() 
{
	theApp.m_LastSettings.bIncludeRefPtsInScaling = m_checkIncludeRefPts.GetCheck() & 0x01;
	Update3DEditView(0);
}
