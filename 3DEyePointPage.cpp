// 3DEyePointPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "3DEyePointPage.h"
#include "3DViewEditDlg.h"
#include "3DViewPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DEyePointPage property page

IMPLEMENT_DYNCREATE(C3DEyePointPage, CPropertyPage)

C3DEyePointPage::C3DEyePointPage(CPropertySheet* pToolSheet, CPropertyPage* pViewPage)
 : CPropertyPage(C3DEyePointPage::IDD)
{
	//{{AFX_DATA_INIT(C3DEyePointPage)
	//}}AFX_DATA_INIT
	m_pViewPage = pViewPage;
	m_pToolSheet = pToolSheet;
}

C3DEyePointPage::~C3DEyePointPage()
{
}

void C3DEyePointPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3DEyePointPage)
	DDX_Control(pDX, IDC_COMBO_COORDINATE_FRAME, m_comboCoordinateFrame);
	DDX_Control(pDX, IDC_COMBO_UP_VECTOR, m_comboUpVector);
	DDX_Control(pDX, IDC_CHECK_TURNTABLE, m_checkTurnTable);
	DDX_Control(pDX, IDC_COMBO_RESOLUTION, m_comboResolution);
	DDX_Control(pDX, IDC_COMBO_CSCAN_OVERLAYMODE, m_comboCScanOverlayMode);
	DDX_Control(pDX, IDC_CHECK_FILL_SCAN_LINES, m_checkFillScanLines);
	DDX_Control(pDX, IDC_EDIT_WALL_PICTURE, m_editWallPicture);
	DDX_Control(pDX, IDC_EDIT_ROWS, m_editRows);
	DDX_Control(pDX, IDC_EDIT_COLUMNS, m_editColumns);
	DDX_Control(pDX, IDC_EDIT_VIEW_PT, m_editViewPt);
	DDX_Control(pDX, IDC_EDIT_EYE_PT, m_editEyePt);
	DDX_Control(pDX, IDC_CHECK_WIRE_FRAME, m_checkWireFrame);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3DEyePointPage, CPropertyPage)
	//{{AFX_MSG_MAP(C3DEyePointPage)
	ON_BN_CLICKED(IDC_CHECK_WIRE_FRAME, OnCheckWireFrame)
	ON_EN_CHANGE(IDC_EDIT_ROWS, OnChangeEditRows)
	ON_EN_CHANGE(IDC_EDIT_COLUMNS, OnChangeEditColumns)
	ON_EN_CHANGE(IDC_EDIT_WALL_PICTURE, OnChangeEditWallPicture)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_CHECK_FILL_SCAN_LINES, OnCheckFillScanLines)
	ON_CBN_SELCHANGE(IDC_COMBO_RESOLUTION, OnSelchangeComboResolution)
	ON_CBN_SELCHANGE(IDC_COMBO_CSCAN_OVERLAYMODE, OnSelchangeComboCscanOverlaymode)
	ON_BN_CLICKED(IDC_CHECK_TURNTABLE, OnCheckTurntable)
	ON_CBN_SELCHANGE(IDC_COMBO_UP_VECTOR, OnSelchangeComboUpVector)
	ON_CBN_SELCHANGE(IDC_COMBO_COORDINATE_FRAME, OnSelchangeComboCoordinateFrame)
	ON_EN_CHANGE(IDC_EDIT_EYE_PT, OnChangeEditEyePt)
	ON_EN_CHANGE(IDC_EDIT_VIEW_PT, OnChangeEditViewPt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DEyePointPage message handlers

BOOL C3DEyePointPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	Buff.LoadString(IDS_Pixels);	m_comboCScanOverlayMode.AddString(Buff);
	Buff.LoadString(IDS_Mesh);		m_comboCScanOverlayMode.AddString(Buff);

	Buff = "100%%";					m_comboResolution.AddString(Buff);
	Buff = "1/2";					m_comboResolution.AddString(Buff);
	Buff = "1/3";					m_comboResolution.AddString(Buff);
	Buff = "1/4";					m_comboResolution.AddString(Buff);
	Buff = "1/5";					m_comboResolution.AddString(Buff);

	Buff.LoadString(IDS_Scanner);	m_comboCoordinateFrame.AddString(Buff);
	Buff.LoadString(IDS_CAD);		m_comboCoordinateFrame.AddString(Buff);

	m_comboUpVector.AddString(L"0.0, 0.0, 1.0");
	m_comboUpVector.AddString(L"0.0, 0.0, -1.0");

	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3DEyePointPage::UpdateAllControls()
{
	if(GetSafeHwnd() == NULL) return;
	CString Buff;

	Buff.Format(_T("X:%.0f Y:%.0f Z:%.0f"),theApp.m_LastSettings.vecEyePt.x,theApp.m_LastSettings.vecEyePt.y,theApp.m_LastSettings.vecEyePt.z);
	m_editEyePt.SetWindowText(Buff);

	Buff.Format(_T("X:%.0f Y:%.0f Z:%.0f"),theApp.m_LastSettings.vecLookAtPt.x,theApp.m_LastSettings.vecLookAtPt.y,theApp.m_LastSettings.vecLookAtPt.z);
	m_editViewPt.SetWindowText(Buff);

	m_checkWireFrame.SetCheck(theApp.m_LastSettings.bWireframe);

	Buff.Format(_T("%d"),theApp.m_LastSettings.nTesselateColumns);
	m_editColumns.SetWindowText(Buff);
	Buff.Format(_T("%d"),theApp.m_LastSettings.nTesselateRows);
	m_editRows.SetWindowText(Buff);

	m_editWallPicture.SetWindowText(theApp.m_WallImagePathName);

	m_checkTurnTable.SetCheck(theApp.m_LastSettings.nDisplayTurnTableIn3D);
	m_checkFillScanLines.SetCheck(theApp.m_LastSettings.n3DFillInScanLines);


	m_comboCScanOverlayMode.SetCurSel(theApp.m_LastSettings.nCScanOverlayMode);
	m_comboResolution.SetCurSel(theApp.m_LastSettings.nCScanOverlayResolution);

	m_comboUpVector.SetCurSel(theApp.m_LastSettings.n3DUpVector);

	m_comboCoordinateFrame.SetCurSel(theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame);

	SetAccessPrivelages();

}

void C3DEyePointPage::OnCheckWireFrame() 
{
	theApp.m_LastSettings.bWireframe = m_checkWireFrame.GetCheck() & 0x01;
}




void C3DEyePointPage::SetAccessPrivelages()
{
	if(theApp.m_LastSettings.nCScanOverlayMode == 0) {
		m_comboResolution.EnableWindow(true);
		m_editRows.EnableWindow(false);
		m_editColumns.EnableWindow(false);
	} else {
		m_comboResolution.EnableWindow(false);
		m_editRows.EnableWindow(true);
		m_editColumns.EnableWindow(true);
	}
}

void C3DEyePointPage::OnChangeEditRows() 
{
	CString Buff;

	m_editRows.GetWindowText(Buff);
	theApp.m_LastSettings.nTesselateRows = _ttoi(Buff);
}

void C3DEyePointPage::OnChangeEditColumns() 
{
	CString Buff;

	m_editColumns.GetWindowText(Buff);
	theApp.m_LastSettings.nTesselateColumns = _ttoi(Buff);
}

void C3DEyePointPage::OnChangeEditWallPicture() 
{
	m_editWallPicture.GetWindowText(theApp.m_WallImagePathName);
}

void C3DEyePointPage::OnButtonBrowse() 
{
	CFileDialog FileDlg(TRUE,_T(".*"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("All Files (*.*)|*.*|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_WallImagePathName;
	if(FileDlg.DoModal()==IDOK) {
		theApp.m_WallImagePathName=FileDlg.GetPathName();
		m_editWallPicture.SetWindowText(theApp.m_WallImagePathName);
	}
	
}


void C3DEyePointPage::OnCheckFillScanLines() 
{
	theApp.m_LastSettings.n3DFillInScanLines = m_checkFillScanLines.GetCheck();
	RenderParent(3);
}

void C3DEyePointPage::OnSelchangeComboResolution() 
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	
	theApp.m_LastSettings.nCScanOverlayResolution = m_comboResolution.GetCurSel();

	pFrame->SendMessage(UI_UPDATE_3D_VIEW_DLG, 1, 0);
	
}

void C3DEyePointPage::OnSelchangeComboCscanOverlaymode() 
{
	theApp.m_LastSettings.nCScanOverlayMode = m_comboCScanOverlayMode.GetCurSel();
	UpdateAllControls();
}

void C3DEyePointPage::OnCheckTurntable() 
{
	theApp.m_LastSettings.nDisplayTurnTableIn3D = m_checkTurnTable.GetCheck();
}

void C3DEyePointPage::RenderParent(int nWhich)
{
//	C3DViewPage* pPage = (C3DViewPage*)m_pViewPage;

//	if(nWhich & 1)	pPage->InitializeEyeAndLookAtPt();
//	if(nWhich & 2)	pPage->InitializeComponent();
//	pPage->Invalidate(false);
}

void C3DEyePointPage::OnSelchangeComboUpVector() 
{
	theApp.m_LastSettings.n3DUpVector = m_comboUpVector.GetCurSel();
}

void C3DEyePointPage::OnSelchangeComboCoordinateFrame() 
{
	theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame = m_comboCoordinateFrame.GetCurSel();
}

void C3DEyePointPage::OnChangeEditEyePt() 
{
	CString Buff;
	D3DXVECTOR3 vOldEyePt = theApp.m_LastSettings.vecEyePt;

	m_editEyePt.GetWindowText(Buff);

	ExtractVector(Buff, &theApp.m_LastSettings.vecEyePt);
	if(theApp.m_LastSettings.vecEyePt != vOldEyePt) {
		RenderParent(1);
	}

}

void C3DEyePointPage::OnChangeEditViewPt() 
{
	CString Buff;
	D3DXVECTOR3 vOldPt = theApp.m_LastSettings.vecLookAtPt;

	m_editViewPt.GetWindowText(Buff);
	ExtractVector(Buff, &theApp.m_LastSettings.vecLookAtPt);
	if(theApp.m_LastSettings.vecLookAtPt != vOldPt) {
		RenderParent(1);
	}
}

BOOL C3DEyePointPage::OnSetActive() 
{
	theApp.m_LastSettings.nLast3DDrawingToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}
