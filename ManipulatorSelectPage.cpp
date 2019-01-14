// ManipulatorSelectPage.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "ManipulatorSelectPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManipulatorSelectPage property page

IMPLEMENT_DYNCREATE(CManipulatorSelectPage, CPropertyPage)

CManipulatorSelectPage::CManipulatorSelectPage() : CPropertyPage(CManipulatorSelectPage::IDD)
{
	//{{AFX_DATA_INIT(CManipulatorSelectPage)
	//}}AFX_DATA_INIT
}

CManipulatorSelectPage::~CManipulatorSelectPage()
{
}

void CManipulatorSelectPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManipulatorSelectPage)
	DDX_Control(pDX, IDC_EDIT_PROMPT, m_editPrompt);
	DDX_Control(pDX, IDC_COMBO_SELECT_MANIPULATOR, m_comboSelectManipulator);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CManipulatorSelectPage, CPropertyPage)
	//{{AFX_MSG_MAP(CManipulatorSelectPage)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECT_MANIPULATOR, OnSelchangeComboSelectManipulator)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManipulatorSelectPage message handlers

BOOL CManipulatorSelectPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_comboSelectManipulator.AddString(_T("Both Wrists"));
	m_comboSelectManipulator.AddString(_T("Wrist & Finger Left, Right Wrist"));
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CManipulatorSelectPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastMotionToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	return CPropertyPage::OnSetActive();
}

void CManipulatorSelectPage::OnSelchangeComboSelectManipulator() 
{
	switch(theApp.m_LastSettings.nManipulatorType[0]) {
	case 0: theApp.m_Axes[theApp.m_Tank.nXtLeftMaster] = theApp.m_Axes[theApp.m_Tank.nXtLeft];
		break;
	case 1: theApp.m_Axes[theApp.m_Tank.nFingerLeftMaster] = theApp.m_Axes[theApp.m_Tank.nXtLeft];
		break;
	}

	
	theApp.m_LastSettings.nManipulatorType[0] = m_comboSelectManipulator.GetCurSel();

	switch(theApp.m_LastSettings.nManipulatorType[0]) {
	case 0: theApp.m_Axes[theApp.m_Tank.nXtLeft] = theApp.m_Axes[theApp.m_Tank.nXtLeftMaster];
		break;
	case 1: theApp.m_Axes[theApp.m_Tank.nXtLeft] = theApp.m_Axes[theApp.m_Tank.nFingerLeftMaster];
		break;
	}
	theApp.m_PmacUser.DownloadPIDForManipulators();
	UpdateAllControls();
}

void CManipulatorSelectPage::UpdateAllControls()
{
	CString Buff;

	m_comboSelectManipulator.SetCurSel(theApp.m_LastSettings.nManipulatorType[0]);

	switch(theApp.m_LastSettings.nManipulatorType[0]) {
	case 0:
		Buff = " Before putting the finger manipulator on, ensure that the \r\n Xtip axis has been origined and is at Zero.\r\n\r\n";
		Buff += " Kill the power to the motors and then fit the manipulator.\r\n\r\n";
		Buff += " Then Select the manipulator type.\r\n\r\n Only then should you power the motors.\r\n";
		break;
	case 1:
		Buff = " Before removing the finger Kill the power to the motors\r\n";
		Buff += " and then remove the manipulator.\r\n\r\n";
		Buff += " Then Select the manipulator type.\r\n\r\n Only then should you power the motors.\r\n";
		break;
	}

	m_editPrompt.SetWindowText(Buff);
}
