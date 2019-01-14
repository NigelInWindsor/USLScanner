// MachineDescriptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "MachineDescriptionDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMachineDescriptionDlg dialog
IMPLEMENT_DYNCREATE(CMachineDescriptionDlg, CPropertyPage)


CMachineDescriptionDlg::CMachineDescriptionDlg() : CPropertyPage(CMachineDescriptionDlg::IDD)
{
	//{{AFX_DATA_INIT(CMachineDescriptionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMachineDescriptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMachineDescriptionDlg)
	DDX_Control(pDX, IDC_COMBO_3D_MACHINE_DRAWING, m_combo3DMachineDrawing);
	DDX_Control(pDX, IDC_CHECK_DISPLAY_TT, m_checkDisplayIn3D);
	DDX_Control(pDX, IDC_EDIT_Z_TT, m_editTTZOffset);
	DDX_Control(pDX, IDC_EDIT_Y_TT, m_editTTYOffset);
	DDX_Control(pDX, IDC_EDIT_X_TT, m_editTTXOffset);
	DDX_Control(pDX, IDC_EDIT_TT_DIAMETER, m_editTTDiameter);
	DDX_Control(pDX, IDC_COMBO_CUSTOMER, m_comboCustomer);
	DDX_Control(pDX, IDC_EDIT_Z_OFSET, m_editZOffset);
	DDX_Control(pDX, IDC_EDIT_Z_LENGTH, m_editZLength);
	DDX_Control(pDX, IDC_EDIT_Y_OFFSET, m_editYOffset);
	DDX_Control(pDX, IDC_EDIT_Y_LENGTH, m_editYLength);
	DDX_Control(pDX, IDC_EDIT_X_OFFSET, m_editXOffset);
	DDX_Control(pDX, IDC_EDIT_X_LENGTH, m_editXLength);
	DDX_Control(pDX, IDC_COMBO_STYLE, m_comboStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMachineDescriptionDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CMachineDescriptionDlg)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, OnSelchangeComboStyle)
	ON_EN_CHANGE(IDC_EDIT_X_LENGTH, OnChangeEditXLength)
	ON_EN_CHANGE(IDC_EDIT_Y_LENGTH, OnChangeEditYLength)
	ON_EN_CHANGE(IDC_EDIT_Z_LENGTH, OnChangeEditZLength)
	ON_EN_CHANGE(IDC_EDIT_X_OFFSET, OnChangeEditXOffset)
	ON_EN_CHANGE(IDC_EDIT_Y_OFFSET, OnChangeEditYOffset)
	ON_EN_CHANGE(IDC_EDIT_Z_OFSET, OnChangeEditZOfset)
	ON_CBN_SELCHANGE(IDC_COMBO_CUSTOMER, OnSelchangeComboCustomer)
	ON_EN_CHANGE(IDC_EDIT_X_TT, OnChangeEditXTt)
	ON_EN_CHANGE(IDC_EDIT_Y_TT, OnChangeEditYTt)
	ON_EN_CHANGE(IDC_EDIT_Z_TT, OnChangeEditZTt)
	ON_EN_CHANGE(IDC_EDIT_TT_DIAMETER, OnChangeEditTtDiameter)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY_TT, OnCheckDisplayTt)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_3D_MACHINE_DRAWING, OnSelchangeCombo3dMachineDrawing)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMachineDescriptionDlg message handlers


BOOL CMachineDescriptionDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	Buff.Format(_T("%.0f mm"),theApp.m_Tank.fXLength);
	m_editXLength.SetWindowText(Buff);
	Buff.Format(_T("%.0f mm"),theApp.m_Tank.fYLength);
	m_editYLength.SetWindowText(Buff);
	Buff.Format(_T("%.0f mm"),theApp.m_Tank.fZLength);
	m_editZLength.SetWindowText(Buff);

	Buff.Format(_T("%.0f mm"),theApp.m_Tank.fXSurfaceZero);
	m_editXOffset.SetWindowText(Buff);
	Buff.Format(_T("%.0f mm"),theApp.m_Tank.fYSurfaceZero);
	m_editYOffset.SetWindowText(Buff);
	Buff.Format(_T("%.0f mm"),theApp.m_Tank.fZSurfaceZero);
	m_editZOffset.SetWindowText(Buff);

	Buff.Format(_T("%.0f mm"),theApp.m_Tank.fTTXOffset);
	m_editTTXOffset.SetWindowText(Buff);
	Buff.Format(_T("%.0f mm"),theApp.m_Tank.fTTYOffset);
	m_editTTYOffset.SetWindowText(Buff);
	Buff.Format(_T("%.0f mm"),theApp.m_Tank.fTTZOffset);
	m_editTTZOffset.SetWindowText(Buff);
	Buff.Format(_T("%.0f mm"),theApp.m_Tank.fTTDiameter);
	m_editTTDiameter.SetWindowText(Buff);

	Buff.LoadString(IDS_Dual_Sided_Squirter);			m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_Single_Bridge_Tank);			m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_Double_Bridge_Tank);			m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_Single_Bridge_Double_Sided);	m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_Dual_Bridge_Double_Sided);		m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_Dual_Sided_Squirter_Version_2);	m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_Dual_Squirter_less_9_axes);		m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_Single_Robot);					m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_Dual_Robot);					m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_Single_Bridge_Tank_With_Rot);	m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_Vertical_Bore_scanner);			m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_XYZ_Xt_Yt_R_Roller);			m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_9_plus_9_dual_robot);			m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_PPMAC_11_Axis);					m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_Twin_Tower);					m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_SPX_Robot);						m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_Railway_Wheels_Axle);			m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_Tracked_Probe);					m_comboStyle.AddString(Buff);
	Buff.LoadString(IDS_Tank_5_Axis);					m_comboStyle.AddString(Buff);
	Buff.Format(L"QuickStep Flat bed");					m_comboStyle.AddString(Buff);
	Buff.Format(L"Tank 6 axis kinematic");				m_comboStyle.AddString(Buff);
	Buff.Format(L"Twin tower kinematic");				m_comboStyle.AddString(Buff);
	m_comboStyle.SetCurSel(theApp.m_Tank.nScannerDescription);

	Buff.LoadString(IDS_General);						m_comboCustomer.AddString(Buff);
	Buff.LoadString(IDS_Fokker);						m_comboCustomer.AddString(Buff);
	Buff.LoadString(IDS_Harbin);						m_comboCustomer.AddString(Buff);
	Buff.LoadString(IDS_NLR);							m_comboCustomer.AddString(Buff);
	Buff.LoadString(IDS_GKN);							m_comboCustomer.AddString(Buff);
	Buff.LoadString(IDS_ForePlane_Scanner);				m_comboCustomer.AddString(Buff);
	Buff.LoadString(IDS_ICAM);							m_comboCustomer.AddString(Buff);
	Buff.LoadString(IDS_RR_FlatPack);					m_comboCustomer.AddString(Buff);
	Buff.LoadString(IDS_GOODRICH_MACHINE_4);			m_comboCustomer.AddString(Buff);
	Buff.LoadString(IDS_BAE_FOREPLANE_2);				m_comboCustomer.AddString(Buff);
	Buff.LoadString(IDS_GKN_ALABAMA_2);					m_comboCustomer.AddString(Buff);
	Buff.LoadString(IDS_DOWTY_BSCAN);					m_comboCustomer.AddString(Buff);
	Buff.LoadString(IDS_BAE_SUBMARINES);				m_comboCustomer.AddString(Buff);
	Buff.LoadString(IDS_TWI);							m_comboCustomer.AddString(Buff);
	Buff.LoadString(IDS_Hengshen);						m_comboCustomer.AddString(Buff);
	m_checkDisplayIn3D.SetCheck(theApp.m_LastSettings.nDisplayTurnTableIn3D);

	Buff.LoadString(IDS_Standard_10_Axis);		m_combo3DMachineDrawing.AddString(Buff);
	Buff.LoadString(IDS_Tank_zero_near_side);	m_combo3DMachineDrawing.AddString(Buff);
	Buff.LoadString(IDS_Tank_zero_far_side);	m_combo3DMachineDrawing.AddString(Buff);
	m_combo3DMachineDrawing.SetCurSel(theApp.m_LastSettings.n3DMachineDrawingStyle);


	switch(theApp.m_nJobNumber) {
	default:	m_comboCustomer.SetCurSel(0);
		break;
	case FOKKER:				m_comboCustomer.SetCurSel(1);
		break;
	case HARBIN:				m_comboCustomer.SetCurSel(2);
		break;
	case NLR:					m_comboCustomer.SetCurSel(3);
		break;
	case GKN:					m_comboCustomer.SetCurSel(4);
		break;
	case FOREPLANE:				m_comboCustomer.SetCurSel(5);
		break;
	case ICAM:					m_comboCustomer.SetCurSel(6);
		break;
	case RRFLATPACK:			m_comboCustomer.SetCurSel(7);
		break;
	case GOODRICH_MACHINE_4:	m_comboCustomer.SetCurSel(8);
		break;
	case BAE_FOREPLANE_2:		m_comboCustomer.SetCurSel(9);
		break;
	case GKN_ALABAMA_2:			m_comboCustomer.SetCurSel(10);
		break;
	case DOWTY_BSCAN:			m_comboCustomer.SetCurSel(11);
		break;
	case BAE_SUB:				m_comboCustomer.SetCurSel(12);
		break;
	case TWI:					m_comboCustomer.SetCurSel(13);
		break;
	case HENGSHEN:				m_comboCustomer.SetCurSel(14);
		break;
	};
	
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CMachineDescriptionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}


void CMachineDescriptionDlg::OnSelchangeComboStyle() 
{
	theApp.m_Tank.nScannerDescription = (ScannerDescription)m_comboStyle.GetCurSel();
}

void CMachineDescriptionDlg::OnChangeEditXLength() 
{
	CString Buff;
	float fTemp = theApp.m_Tank.fXLength;

	m_editXLength.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Tank.fXLength);
	if(fTemp - theApp.m_Tank.fXLength) Invalidate3DImages();
}

void CMachineDescriptionDlg::OnChangeEditYLength() 
{
	CString Buff;
	float fTemp = theApp.m_Tank.fYLength;

	m_editYLength.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Tank.fYLength);
	if(fTemp - theApp.m_Tank.fYLength) Invalidate3DImages();
}

void CMachineDescriptionDlg::OnChangeEditZLength() 
{
	CString Buff;
	float fTemp = theApp.m_Tank.fZLength;

	m_editZLength.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Tank.fZLength);
	if(fTemp - theApp.m_Tank.fZLength) Invalidate3DImages();
}

void CMachineDescriptionDlg::OnChangeEditXOffset() 
{
	CString Buff;
	float fTemp = theApp.m_Tank.fXSurfaceZero;

	m_editXOffset.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Tank.fXSurfaceZero);
	if(fTemp - theApp.m_Tank.fXSurfaceZero) Invalidate3DImages();
}

void CMachineDescriptionDlg::OnChangeEditYOffset() 
{
	CString Buff;
	float fTemp = theApp.m_Tank.fYSurfaceZero;

	m_editYOffset.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Tank.fYSurfaceZero);
	if(fTemp - theApp.m_Tank.fYSurfaceZero)	Invalidate3DImages();
}

void CMachineDescriptionDlg::OnChangeEditZOfset() 
{
	CString Buff;
	float fTemp = theApp.m_Tank.fZSurfaceZero;

	m_editZOffset.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Tank.fZSurfaceZero);
	if(fTemp - theApp.m_Tank.fZSurfaceZero)	Invalidate3DImages();
}

void CMachineDescriptionDlg::OnSelchangeComboCustomer() 
{
	switch(m_comboCustomer.GetCurSel()) {
	case 0:	theApp.m_nJobNumber=0;
		break;
	case 1:	theApp.m_nJobNumber=FOKKER;
		break;
	case 2:	theApp.m_nJobNumber=HARBIN;
		break;
	case 3:	theApp.m_nJobNumber=NLR;
		break;
	case 4:	theApp.m_nJobNumber=GKN;
		break;
	case 5:	theApp.m_nJobNumber=FOREPLANE;
		break;
	case 6:	theApp.m_nJobNumber=ICAM;
		break;
	case 7:	theApp.m_nJobNumber=RRFLATPACK;
		break;
	case 8:	theApp.m_nJobNumber=GOODRICH_MACHINE_4;
		break;
	case 9: theApp.m_nJobNumber=BAE_FOREPLANE_2;
		break;
	case 10: theApp.m_nJobNumber=GKN_ALABAMA_2;
		break;
	case 11: theApp.m_nJobNumber=DOWTY_BSCAN;
		break;
	case 12: theApp.m_nJobNumber=BAE_SUB;
		break;
	case 13: theApp.m_nJobNumber=TWI;
		break;
	case 14: theApp.m_nJobNumber=HENGSHEN;
		break;
	};
}

void CMachineDescriptionDlg::OnChangeEditXTt() 
{
	CString Buff;
	float fTemp = theApp.m_Tank.fTTXOffset;

	m_editTTXOffset.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Tank.fTTXOffset);
	if(fTemp - theApp.m_Tank.fTTXOffset)	Invalidate3DImages();
}

void CMachineDescriptionDlg::OnChangeEditYTt() 
{
	CString Buff;
	float fTemp = theApp.m_Tank.fTTYOffset;

	m_editTTYOffset.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Tank.fTTYOffset);
	if(fTemp - theApp.m_Tank.fTTYOffset)	Invalidate3DImages();
}

void CMachineDescriptionDlg::OnChangeEditZTt() 
{
	CString Buff;
	float fTemp = theApp.m_Tank.fTTZOffset;

	m_editTTZOffset.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Tank.fTTZOffset);
	if(fTemp - theApp.m_Tank.fTTZOffset)	Invalidate3DImages();
}

void CMachineDescriptionDlg::OnChangeEditTtDiameter() 
{
	CString Buff;
	float fTemp = theApp.m_Tank.fTTDiameter;

	m_editTTDiameter.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Tank.fTTDiameter);
	if(fTemp - theApp.m_Tank.fTTDiameter)	Invalidate3DImages();
}

void CMachineDescriptionDlg::OnCheckDisplayTt() 
{
	theApp.m_LastSettings.nDisplayTurnTableIn3D = m_checkDisplayIn3D.GetCheck();
	Invalidate3DImages();

}

void CMachineDescriptionDlg::Invalidate3DImages()
{
	CMainFrame* pFrame = (CMainFrame*)theApp.m_pMainWnd;

	pFrame->Invalidate3DImages();
}

void CMachineDescriptionDlg::OnSelchangeCombo3dMachineDrawing() 
{
	theApp.m_LastSettings.n3DMachineDrawingStyle = (MachineDrawingStyle)m_combo3DMachineDrawing.GetCurSel();
	Invalidate3DImages();
}
