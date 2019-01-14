// DeviceResoucesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "DeviceResoucesDlg.h"
#include "PermissionDlg.h"
#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeviceResoucesDlg dialog


CDeviceResoucesDlg::CDeviceResoucesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeviceResoucesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeviceResoucesDlg)
	m_stringPmacVersion = _T("");
	m_stringSi6Version = _T("");
	m_stringUltrasonicsVersion = _T("");
	m_Status = _T("");
	//}}AFX_DATA_INIT
}


void CDeviceResoucesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeviceResoucesDlg)
	DDX_Control(pDX, IDC_COMBO_SI6_PRESENT, m_comboSi6Present);
	DDX_Control(pDX, IDC_COMBO_PMAC_PRESENT, m_comboPmacPresent);
	DDX_Control(pDX, IDC_BUTTON_LOOKUP, m_buttonLookUp);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_buttonBrowse);
	DDX_Control(pDX, IDC_BUTTON_INSTALL, m_buttonInstall);
	DDX_Control(pDX, IDC_EDIT_LASTSETTINGS, m_editLastSettings);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_buttonStop);
	DDX_Control(pDX, IDC_BUTTON_START, m_buttonStart);
	DDX_Control(pDX, IDC_COMBO_HARDWARE, m_comboHardware);
	DDX_Control(pDX, IDC_EDIT_DRIVER_NAME, m_editDriverName);
	DDX_Control(pDX, IDC_EDIT_PMAC_MEM_RANGE, m_editPmacMemRange);
	DDX_Control(pDX, IDC_EDIT_PMAC_MEM_ADDRS, m_editPmacMemAddrs);
	DDX_Control(pDX, IDC_EDIT_PMAC_IRQ, m_editPmacIRQ);
	DDX_Control(pDX, IDC_EDIT_PMAC_IO_RANGE, m_editPmacIORange);
	DDX_Control(pDX, IDC_EDIT_PMAC_IO_ADDRS, m_editPmacIOAddrs);
	DDX_Control(pDX, IDC_EDIT_UT_MEM_RANGE, m_editUtMemRange);
	DDX_Control(pDX, IDC_EDIT_UT_MEM_ADDRS, m_editUtMemAddrs);
	DDX_Control(pDX, IDC_EDIT_UT_IRQ, m_editUtIRQ);
	DDX_Control(pDX, IDC_EDIT_UT_IO_RANGE, m_editUtIORange);
	DDX_Control(pDX, IDC_EDIT_UT_IO_ADDRS, m_editUtIOAddrs);
	DDX_Control(pDX, IDC_EDIT_SI6_MEM_RANGE, m_editSi6MemRange);
	DDX_Control(pDX, IDC_EDIT_SI6_MEM_ADDRS, m_editSi6MemAddrs);
	DDX_Control(pDX, IDC_EDIT_SI6_IRQ, m_editSi6IRQ);
	DDX_Control(pDX, IDC_EDIT_SI6_IO_RANGE, m_editSi6IORange);
	DDX_Control(pDX, IDC_EDIT_SI6_IO_ADDRS, m_editSi6IOAddrs);
	DDX_Text(pDX, IDC_EDIT_PMAC_VERSION, m_stringPmacVersion);
	DDX_Text(pDX, IDC_EDIT_SI6_VERSION, m_stringSi6Version);
	DDX_Text(pDX, IDC_EDIT_ULTRASONICS_VERSION, m_stringUltrasonicsVersion);
	DDX_Text(pDX, IDC_EDIT_STATUS, m_Status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeviceResoucesDlg, CDialog)
	//{{AFX_MSG_MAP(CDeviceResoucesDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOOKUP, OnButtonLookup)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_CBN_SELCHANGE(IDC_COMBO_HARDWARE, OnSelchangeComboHardware)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_INSTALL, OnButtonInstall)
	ON_CBN_SELCHANGE(IDC_COMBO_PMAC_PRESENT, OnSelchangeComboPmacPresent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeviceResoucesDlg message handlers

BOOL CDeviceResoucesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString	Buff;

	Buff.Format(_T("%03x h"),theApp.m_Si6Res.nIOAddrs);
	m_editSi6IOAddrs.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_Si6Res.nIORange);
	m_editSi6IORange.SetWindowText(Buff);
	Buff.Format(_T("%05x h"),theApp.m_Si6Res.nMemAddrs);
	m_editSi6MemAddrs.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_Si6Res.nMemRange);
	m_editSi6MemRange.SetWindowText(Buff);
	Buff.Format(_T("%d "),theApp.m_Si6Res.nIrq);
	m_editSi6IRQ.SetWindowText(Buff);

	Buff.Format(_T("%03x h"),theApp.m_UtRes.nIOAddrs);
	m_editUtIOAddrs.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_UtRes.nIORange);
	m_editUtIORange.SetWindowText(Buff);
	Buff.Format(_T("%05x h"),theApp.m_UtRes.nMemAddrs);
	m_editUtMemAddrs.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_UtRes.nMemRange);
	m_editUtMemRange.SetWindowText(Buff);
	Buff.Format(_T("%d "),theApp.m_UtRes.nIrq);
	m_editUtIRQ.SetWindowText(Buff);
	
	Buff.Format(_T("%03x h"),theApp.m_PmacRes.nIOAddrs);
	m_editPmacIOAddrs.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_PmacRes.nIORange);
	m_editPmacIORange.SetWindowText(Buff);
	Buff.Format(_T("%05x h"),theApp.m_PmacRes.nMemAddrs);
	m_editPmacMemAddrs.SetWindowText(Buff);
	Buff.Format(_T("%02x h"),theApp.m_PmacRes.nMemRange);
	m_editPmacMemRange.SetWindowText(Buff);
	Buff.Format(_T("%d "),theApp.m_PmacRes.nIrq);
	m_editPmacIRQ.SetWindowText(Buff);
	

	theApp.m_Si6User.GetVersion(&m_stringSi6Version);
	theApp.m_UtUser.GetVersion(&m_stringUltrasonicsVersion);
	theApp.m_ServoUser.GetVersion(&m_stringPmacVersion);

	m_comboHardware.AddString(_T("Not Present"));
	m_comboHardware.AddString(_T("Present"));
	m_comboHardware.SetCurSel(theApp.m_bUtHardware);

	m_comboPmacPresent.AddString(_T("Not Present"));
	m_comboPmacPresent.AddString(_T("Present"));
	m_comboPmacPresent.SetCurSel(theApp.m_bServoHardware);

	m_comboSi6Present.AddString(_T("Not Present"));
	m_comboSi6Present.AddString(_T("Present"));
	m_comboSi6Present.SetCurSel(theApp.m_bSi6Hardware);

	m_editLastSettings.SetWindowText(theApp.m_LastSettingsPath);

	if(theApp.m_bUtHardware==TRUE) {
		m_buttonStart.EnableWindow(FALSE);
		m_buttonStop.EnableWindow(FALSE);
		m_buttonInstall.EnableWindow(FALSE);
	}
	SetAccessPrivelage();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDeviceResoucesDlg::OnOK()
{
	CString	Buff;

	m_editSi6IOAddrs.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Si6Res.nIOAddrs);
	m_editSi6IORange.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Si6Res.nIORange);
	m_editSi6MemAddrs.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Si6Res.nMemAddrs);
	m_editSi6MemRange.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_Si6Res.nMemRange);
	m_editSi6IRQ.GetWindowText(Buff);
	swscanf_s(Buff,_T("%d"),&theApp.m_Si6Res.nIrq);

	m_editUtIOAddrs.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_UtRes.nIOAddrs);
	m_editUtIORange.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_UtRes.nIORange);
	m_editUtMemAddrs.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_UtRes.nMemAddrs);
	m_editUtMemRange.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_UtRes.nMemRange);
	m_editUtIRQ.GetWindowText(Buff);
	swscanf_s(Buff,_T("%d"),&theApp.m_UtRes.nIrq);

	m_editPmacIOAddrs.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_PmacRes.nIOAddrs);
	m_editPmacIORange.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_PmacRes.nIORange);
	m_editPmacMemAddrs.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_PmacRes.nMemAddrs);
	m_editPmacMemRange.GetWindowText(Buff);
	swscanf_s(Buff,_T("%x"),&theApp.m_PmacRes.nMemRange);
	m_editPmacIRQ.GetWindowText(Buff);
	swscanf_s(Buff,_T("%d"),&theApp.m_PmacRes.nIrq);

	m_editLastSettings.GetWindowText(theApp.m_LastSettingsPath);

	theApp.m_bUtHardware=m_comboHardware.GetCurSel() & 0x01;
	theApp.m_bServoHardware=m_comboPmacPresent.GetCurSel() & 0x01;
	theApp.m_bSi6Hardware=m_comboSi6Present.GetCurSel() & 0x01;

	CPermissionDlg dlg;
	if((dlg.DoModal() == IDOK) && (dlg.m_bAcceptable == TRUE)) {
		theApp.SaveCardEntries();
		theApp.SaveRegistryVariables();
	}

	CDialog::OnOK();

}


void CDeviceResoucesDlg::OnButtonLookup() 
{
	CString	Driver;
	CRegistry Reg;

	m_editDriverName.GetWindowText(Driver);
	Reg.LookupDriver(Driver,&m_Status,this);		
}

void CDeviceResoucesDlg::OnButtonStart() 
{
	CString	Driver;
	CRegistry Reg;

	m_editDriverName.GetWindowText(Driver);
	Reg.StartDriver(Driver,&m_Status,this);
}

void CDeviceResoucesDlg::OnButtonStop() 
{
	CString	Driver;
	CRegistry Reg;

	m_editDriverName.GetWindowText(Driver);
	Reg.StopDriver(Driver,&m_Status,this);	
}

void CDeviceResoucesDlg::OnSelchangeComboHardware() 
{
}
void CDeviceResoucesDlg::OnSelchangeComboPmacPresent() 
{

}


void CDeviceResoucesDlg::OnButtonBrowse() 
{
	CFile File;
	CString Filename;
	WCHAR str[_MAX_PATH];
	WCHAR *ptr;

	CFileDialog FileDlg(TRUE,_T(".set"),_T("LastSettings.set"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Set (*.set)|*.set|All Files (*.*)|*.*||"),NULL);
	FileDlg.DoModal();
	wcscpy_s(str,_MAX_PATH,FileDlg.GetPathName());
	if((ptr=wcsstr(str,_T("\\LastSettings")))!=NULL) *ptr=0;
	theApp.m_LastSettingsPath.Format(_T("%s"),str);
		
	m_editLastSettings.SetWindowText(theApp.m_LastSettingsPath);	
}

void CDeviceResoucesDlg::OnButtonInstall() 
{
	CFileDialog FileDlg(TRUE,_T(".sys"),_T("System Files"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Sys (*.sys)|*.sys|All Files (*.*)|*.*||"),NULL);
	
	if(FileDlg.DoModal()==IDOK) {
		CRegistry Reg( theApp.m_ProgramName );
		CString FilePath = FileDlg.GetPathName();
		CString DriverName;
		m_editDriverName.GetWindowText(DriverName);

		Reg.CopyDriverToWindows(FilePath,DriverName,&m_Status,this);
		Reg.CreateHardwareKey(DriverName);
	}
}

void CDeviceResoucesDlg::SetAccessPrivelage()
{
	if(!(theApp.m_nLogonLevelMask & MANUFACTURER)) {
		m_buttonInstall.EnableWindow(FALSE);
		m_editLastSettings.EnableWindow(FALSE);
		m_buttonStop.EnableWindow(FALSE);
		m_buttonStart.EnableWindow(FALSE);
		m_comboHardware.EnableWindow(FALSE);
		m_editDriverName.EnableWindow(FALSE);
		m_editPmacMemRange.EnableWindow(FALSE);
		m_editPmacMemAddrs.EnableWindow(FALSE);
		m_editPmacIRQ.EnableWindow(FALSE);
		m_editPmacIORange.EnableWindow(FALSE);
		m_editPmacIOAddrs.EnableWindow(FALSE);
		m_editUtMemRange.EnableWindow(FALSE);
		m_editUtMemAddrs.EnableWindow(FALSE);
		m_editUtIRQ.EnableWindow(FALSE);
		m_editUtIORange.EnableWindow(FALSE);
		m_editUtIOAddrs.EnableWindow(FALSE);
		m_editSi6MemRange.EnableWindow(FALSE);
		m_editSi6MemAddrs.EnableWindow(FALSE);
		m_editSi6IRQ.EnableWindow(FALSE);
		m_editSi6IORange.EnableWindow(FALSE);
		m_editSi6IOAddrs.EnableWindow(FALSE);
		m_buttonLookUp.EnableWindow(FALSE);
		m_buttonBrowse.EnableWindow(FALSE);
//		m_stringPmacVersion.EnableWindow(FALSE);
//		m_stringSi6Version.EnableWindow(FALSE);
//		m_stringUltrasonicsVersion.EnableWindow(FALSE);
//		m_Status.EnableWindow(FALSE);
	}

}


void CDeviceResoucesDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
