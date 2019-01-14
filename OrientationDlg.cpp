// OrientationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "OrientationDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrientationDlg dialog
IMPLEMENT_DYNCREATE(COrientationDlg, CPropertyPage)


COrientationDlg::COrientationDlg(): CPropertyPage(COrientationDlg::IDD)
{
	//{{AFX_DATA_INIT(COrientationDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COrientationDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrientationDlg)
	DDX_Control(pDX, IDC_EDIT_RIGHT_NAME, m_editRightName);
	DDX_Control(pDX, IDC_EDIT_LEFT_NAME, m_editLeftName);
	DDX_Control(pDX, IDC_COMBO_UNITS, m_comboUnits);
	DDX_Control(pDX, IDC_COMBO_SIDE0_ORIENTATION, m_comboOrientation);
	DDX_Control(pDX, IDC_COMBO_PRIMARY_SIDE, m_comboPrimarySide);
	DDX_Control(pDX, IDC_COMBO_LANGUAGE, m_comboLanguage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrientationDlg, CPropertyPage)
	//{{AFX_MSG_MAP(COrientationDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_LANGUAGE, OnSelchangeComboLanguage)
	ON_CBN_SELCHANGE(IDC_COMBO_PRIMARY_SIDE, OnSelchangeComboPrimarySide)
	ON_CBN_SELCHANGE(IDC_COMBO_SIDE0_ORIENTATION, OnSelchangeComboSide0Orientation)
	ON_CBN_SELCHANGE(IDC_COMBO_UNITS, OnSelchangeComboUnits)
	ON_EN_CHANGE(IDC_EDIT_RIGHT_NAME, OnChangeEditRightName)
	ON_EN_CHANGE(IDC_EDIT_LEFT_NAME, OnChangeEditLeftName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrientationDlg message handlers

BOOL COrientationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString Buff;
	
	m_editLeftName.SetWindowText(theApp.m_DlgSideName[0]);
	m_editRightName.SetWindowText(theApp.m_DlgSideName[1]);

	Buff.LoadString(IDS_Left); m_comboOrientation.AddString(Buff);
	Buff.LoadString(IDS_Right); m_comboOrientation.AddString(Buff);
	m_comboOrientation.SetCurSel(theApp.m_nSide0Orientation);

	Buff.LoadString(IDS_Left); m_comboPrimarySide.AddString(Buff);
	Buff.LoadString(IDS_Right); m_comboPrimarySide.AddString(Buff);
	m_comboPrimarySide.SetCurSel(theApp.m_nPrimarySide);

	Buff.LoadString(IDS_English); m_comboLanguage.AddString(Buff);
	Buff.LoadString(IDS_Russian); m_comboLanguage.AddString(Buff);
	Buff.LoadString(IDS_Chinese); m_comboLanguage.AddString(Buff);
	Buff.LoadString(IDS_German); m_comboLanguage.AddString(Buff);
	Buff.LoadString(IDS_Spanish); m_comboLanguage.AddString(Buff);
	Buff.LoadString(IDS_Japanese); m_comboLanguage.AddString(Buff);
	Buff.LoadString(IDS_Taiwanese); m_comboLanguage.AddString(Buff);
	m_comboLanguage.SetCurSel(theApp.m_nLanguage);

	m_comboUnits.AddString(_T("mm"));
	m_comboUnits.AddString(_T("\x022"));
	m_comboUnits.SetCurSel(theApp.m_LastSettings.nMeasurementUnits);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COrientationDlg::SetRegistry() 
{
	theApp.SaveRegistryVariables();
}

void COrientationDlg::OnSelchangeComboLanguage() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	theApp.m_nLanguage = m_comboLanguage.GetCurSel();
	SetRegistry();
	theApp.SetLanguage();
	pFrame->SendMessage(UI_LANGUAGE_CHANGED);
}

void COrientationDlg::OnSelchangeComboPrimarySide() 
{
	theApp.m_nPrimarySide = m_comboPrimarySide.GetCurSel();
	SetRegistry();
}

void COrientationDlg::OnSelchangeComboSide0Orientation() 
{
	theApp.m_nSide0Orientation = m_comboOrientation.GetCurSel();
	SetRegistry();
}

void COrientationDlg::OnSelchangeComboUnits() 
{
	theApp.m_LastSettings.nMeasurementUnits = m_comboUnits.GetCurSel();
}

void COrientationDlg::OnChangeEditLeftName() 
{
	m_editLeftName.GetWindowText(theApp.m_DlgSideName[0]);
}

void COrientationDlg::OnChangeEditRightName() 
{
	m_editRightName.GetWindowText(theApp.m_DlgSideName[1]);
}

