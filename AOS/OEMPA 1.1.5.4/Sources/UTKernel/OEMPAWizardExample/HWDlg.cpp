// HWDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OEMPAWizardExample.h"
#include "OEMPAWizardExampleView.h"
#include "HWDlg.h"
#include "MainFrm.h"
#include "OEMPAWizardExampleDoc.h"
#include "afxdialogex.h"
#include "TimerProcess.h"

#define WPARAM_RUN_TOOLBOX			123
#define LPARAM_RUN_TOOLBOX_OK		1234
#define LPARAM_RUN_TOOLBOX_ERROR	12345

void EditFile(const wchar_t *pFile,bool bCloseWaiting);
bool WizardDelete();
bool WizardNew();
bool DeviceOEMPADelete();
bool DeviceOEMPANew(bool bHW);
bool OEMPA_ReadWizardWriteFile(const wchar_t *pFileName);
bool IsWizardScanUpToDate();
bool WizardUpdateScan(int *piErrorChannelProbe,int *piErrorChannelScan);
bool WizardToHw();
bool ReadWizardWriteTemplate(bool bOverwrite);
bool ReadTemplateWriteWizard();

extern CHWDeviceOEMPA *g_pHWDeviceOEMPA;
extern CUTWizardSystem *g_pWizardSystem;
extern CUTChannels *g_pChannel;

// CHWDlg dialog
CRunToolbox g_RunToolbox;

static UINT g_uiUTEventMessage=0;
BOOL CHWDlg::m_bCheckConnect=FALSE;
DWORD CHWDlg::m_dwTimerID=0;
#ifndef _EVALUATION_
BYTE CHWDlg::m_cIP[4]={192,168,1,11};
#else _EVALUATION_
BYTE CHWDlg::m_cIP[4]={127,0,0,1};
#endif _EVALUATION_
int CHWDlg::m_iRadioSingleChannel=0;
bool CHWDlg::m_bIsModified=false;
bool CHWDlg::m_bIsChannelUpToDate=false;

IMPLEMENT_DYNAMIC(CHWDlg, CDialog)

CHWDlg::CHWDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHWDlg::IDD, pParent)
{
	g_uiUTEventMessage = UTKernel_RegisterWindowMessage();
	Create(IDD_DIALOG_HW,pParent);
	ShowWindow(SW_SHOW);
}

CHWDlg::~CHWDlg()
{
	WizardDelete();//explicit call: this is a quicker way to close.
}

void CHWDlg::PostNcDestroy()
{
	delete this;
	COEMPAWizardExampleView::g_pDlg = NULL;
}

void CHWDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPAddress);
	DDX_Control(pDX, IDC_CHECK_CONNECT, m_CheckConnect);
	DDX_Check(pDX, IDC_CHECK_CONNECT, m_bCheckConnect);
	DDX_Control(pDX, IDC_BUTTON_OEMPA_FILE_TO_HW, m_ButtonOEMPAFileToHw);
	DDX_Control(pDX, IDC_BUTTON_HW_TO_OEMPA_FILE, m_ButtonHwToOEMPAFile);
	DDX_Control(pDX, IDC_BUTTON_TEMPLATE_TO_UPDATE_SCAN, m_ButtonTemplateToUpdateScan);
	DDX_Control(pDX, IDC_BUTTON_TEMPLATE_TO_WIZARD, m_ButtonTemplateToWizard);
	DDX_Control(pDX, IDC_BUTTON_WIZARD_TO_TEMPLATE, m_ButtonWizardToTemplate);
	DDX_Control(pDX, IDC_BUTTON_KERNEL_FILE_TO_TEMPLATE, m_ButtonKernelFileToWizard);
	DDX_Control(pDX, IDC_BUTTON_WIZARD_TO_KERNEL_FILE, m_ButtonWizardToKernelFile);
	DDX_Radio(pDX, IDC_RADIO_SINGLE_CHANNEL, m_iRadioSingleChannel);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_ButtonEdit);
	DDX_Control(pDX, IDC_BUTTON_UPDATE_SCAN, m_ButtonUpdateScans);
	DDX_Control(pDX, IDC_BUTTON_WIZARD_TO_HW, m_ButtonWizardToHw);
	DDX_Control(pDX, IDC_BUTTON_WIZARD_TO_OEMPA_FILE, m_ButtonWizardToOempaFile);
	DDX_Control(pDX, IDC_BUTTON_WIZARD_TO_KERNEL_FILE_FULL, m_ButtonWizardToKernelFileFull);
	DDX_Control(pDX, IDC_BUTTON_HW_TO_WIZARD, m_ButtonHwToWizard);
	DDX_Control(pDX, IDC_BUTTON_OEMPA_FILE_TO_WIZARD, m_ButtonOempaFileToWizard);
}


BEGIN_MESSAGE_MAP(CHWDlg, CDialog)
	ON_WM_TIMER()
	ON_REGISTERED_MESSAGE(g_uiUTEventMessage, &CHWDlg::OnEventMessage)
	ON_BN_CLICKED(IDC_CHECK_CONNECT, &CHWDlg::OnBnClickedCheckConnect)
	ON_BN_CLICKED(IDC_BUTTON_WIZARD_TO_HW, &CHWDlg::OnBnClickedButtonWizardToHw)
	ON_BN_CLICKED(IDC_BUTTON_OEMPA_FILE_TO_HW, &CHWDlg::OnBnClickedButtonOEMPAFileToHw)
	ON_BN_CLICKED(IDC_BUTTON_HW_TO_OEMPA_FILE, &CHWDlg::OnBnClickedButtonHwToOEMPAFile)
	ON_BN_CLICKED(IDOK, &CHWDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_TEMPLATE_TO_UPDATE_SCAN, &CHWDlg::OnBnClickedButtonTemplateToUpdateScan)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CHWDlg::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_WIZARD_TO_KERNEL_FILE, &CHWDlg::OnBnClickedButtonWizardToKernelFile)
	ON_BN_CLICKED(IDC_BUTTON_KERNEL_FILE_TO_TEMPLATE, &CHWDlg::OnBnClickedButtonKernelFileToWizard)
	ON_BN_CLICKED(IDC_BUTTON_TEMPLATE_TO_WIZARD, &CHWDlg::OnBnClickedButtonTemplateToWizard)
	ON_BN_CLICKED(IDC_BUTTON_WIZARD_TO_TEMPLATE, &CHWDlg::OnBnClickedButtonWizardToTemplate)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_SCAN, &CHWDlg::OnBnClickedButtonUpdateScan)
	ON_BN_CLICKED(IDC_RADIO_SINGLE_CHANNEL, &CHWDlg::OnBnClickedRadioSingleChannel)
	ON_BN_CLICKED(IDC_RADIO_MULTI_CHANNEL, &CHWDlg::OnBnClickedRadioMultiChannel)
	ON_BN_CLICKED(IDC_BUTTON_TEMPLATE_VIEW, &CHWDlg::OnBnClickedButtonTemplateView)
	ON_BN_CLICKED(IDC_BUTTON_WIZARD_TO_OEMPA_FILE, &CHWDlg::OnBnClickedButtonWizardToOempaFile)
	ON_BN_CLICKED(IDC_BUTTON_WIZARD_TO_KERNEL_FILE_FULL, &CHWDlg::OnBnClickedButtonWizardToKernelFileFull)
	ON_BN_CLICKED(IDC_BUTTON_OEMPA_FILE_OPTIONS, &CHWDlg::OnBnClickedButtonOempaFileOptions)
END_MESSAGE_MAP()


// CHWDlg message handlers
BOOL CHWDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(CUTChannels::IsDefaultMultiChannels())
	{
		m_iRadioSingleChannel = 1;
	}else{
		m_iRadioSingleChannel = 0;
	}
	UpdateDialog();
	m_IPAddress.SetAddress(m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
	SetTimer(true);
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

bool CHWDlg::SetTimer(bool bEnable)
{
	if(bEnable && !m_dwTimerID)
	{
		m_dwTimerID = (DWORD)CWnd::SetTimer(g_kTimerID, g_kTimerDelay, NULL);
		if(m_dwTimerID)
			return true;
		return false;
	}
	if(!bEnable && m_dwTimerID)
	{
		KillTimer(m_dwTimerID);
		m_dwTimerID = 0;
		return true;
	}
	return false;
}

void CHWDlg::UpdateDialog()
{
	bool bConnected;

	bConnected=(m_bCheckConnect?true:false);

	m_ButtonWizardToHw.ShowWindow(SW_SHOW);
	m_CheckConnect.EnableWindow(TRUE);
	m_IPAddress.EnableWindow(!m_bCheckConnect);
	m_ButtonTemplateToUpdateScan.EnableWindow(TRUE);
	m_ButtonOEMPAFileToHw.EnableWindow(m_bCheckConnect);
	m_ButtonHwToOEMPAFile.EnableWindow(m_bCheckConnect);

	if(m_iRadioSingleChannel)
	{
		if(m_bIsChannelUpToDate)
			m_ButtonUpdateScans.SetWindowText(L"Scans Updated");
		else
			m_ButtonUpdateScans.SetWindowText(L"Update Scans");
	}else{
		if(m_bIsChannelUpToDate)
			m_ButtonUpdateScans.SetWindowText(L"Scan Updated");
		else
			m_ButtonUpdateScans.SetWindowText(L"Update Scan");
	}
	m_ButtonTemplateToWizard.EnableWindow(TRUE);
	m_ButtonWizardToTemplate.EnableWindow(TRUE);
	m_ButtonKernelFileToWizard.EnableWindow(TRUE);
	m_ButtonWizardToKernelFile.EnableWindow(TRUE);
	m_ButtonWizardToKernelFileFull.EnableWindow(TRUE);
	m_ButtonWizardToHw.EnableWindow(m_bCheckConnect);
	m_ButtonHwToWizard.EnableWindow(m_bCheckConnect);
	m_ButtonWizardToOempaFile.EnableWindow(TRUE);
	m_ButtonEdit.EnableWindow(TRUE);
	m_ButtonUpdateScans.EnableWindow(TRUE);

	UpdateData(FALSE);
}

void CHWDlg::OnTimer(UINT_PTR nIDEvent)
{
	bool bUpdate=false;
	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
	COEMPAWizardExampleView *pView;
	COEMPAWizardExampleDoc *pDoc;
	bool bIsChannelUpToDate=m_bIsChannelUpToDate;

	ASSERT(nIDEvent == g_kTimerID);
	if(pMainFrame)
	{
		pView = pMainFrame->myGetActiveView();
		pDoc = pView->GetDocument();
		if(pDoc->IsModified())
			m_bIsModified = true;
		else
			m_bIsModified = false;
	}
	m_bIsChannelUpToDate = IsWizardScanUpToDate();
	if(m_bIsChannelUpToDate!=bIsChannelUpToDate)
		bUpdate = true;
	if(g_pHWDeviceOEMPA)
	{
		if(m_bCheckConnect && !g_pHWDeviceOEMPA->GetSWDevice()->IsConnected())
		{
			bUpdate = true;
			m_bCheckConnect = FALSE;
		}
	}
	if(bUpdate)
		UpdateDialog();
	CDialog::OnTimer(nIDEvent);
}

LRESULT CHWDlg::OnEventMessage(WPARAM wParam,LPARAM lParam)
{
	if((wParam==WPARAM_RUN_TOOLBOX) && (lParam==LPARAM_RUN_TOOLBOX_OK))
		g_RunToolbox.SignalPostFeedback();//No error
	if((wParam==WPARAM_RUN_TOOLBOX) && (lParam==LPARAM_RUN_TOOLBOX_ERROR))
		AfxMessageBox(L"Error to run the toolbox, please try again!");
	return 0;
}

DWORD WINAPI CHWDlg::CallbackThreadToolboxEdit(HWND hWnd,CRunToolbox *pRunToolbox)
{
	int iRootId;

	if(!UTKernel_IsToolboxRunning())
	{
		pRunToolbox->ResetPostFeedback();
		UTKernel_ToolboxRun(false,hWnd,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
		if(!pRunToolbox->WaitPostFeedback(5000,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_ERROR))
			return 1;
		pRunToolbox->ResetPostFeedback();
		UTKernel_ToolboxDisplayCloseAll(hWnd,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
		if(!pRunToolbox->WaitPostFeedback(5000,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_ERROR))
			return 2;
	}
	if(UTKernel_IsToolboxRunning())
	{
		if(g_pWizardSystem && g_pWizardSystem->GetID(iRootId))
		{
			pRunToolbox->ResetPostFeedback();
			UTKernel_ToolboxDisplay(iRootId,hWnd,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
			if(!pRunToolbox->WaitPostFeedback(5000,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_ERROR))
				return 3;
		}
		if(g_pChannel && g_pChannel->GetID(iRootId))
		{
			pRunToolbox->ResetPostFeedback();
			UTKernel_ToolboxDisplay(iRootId,hWnd,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
			if(!pRunToolbox->WaitPostFeedback(5000,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_ERROR))
				return 4;
		}
	}
	return 0;//no error
}

void CHWDlg::OnBnClickedButtonEdit()
{
	g_RunToolbox.Run(GetSafeHwnd(),CHWDlg::CallbackThreadToolboxEdit);
}

void CHWDlg::OnBnClickedCheckConnect()
{
	BOOL bCheckConnect=m_bCheckConnect;
	CWaitCursor wait;
	bool bError=false;
	wchar_t pValue[40];

	UpdateData();
	if(!bCheckConnect && m_bCheckConnect)
	{
		if(DeviceOEMPADelete() && DeviceOEMPANew(true))
		{
			m_IPAddress.GetAddress(m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
			swprintf(pValue,40,L"%d.%d.%d.%d",m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
			if(	!g_pHWDeviceOEMPA->GetSWDeviceOEMPA()->SetIPAddress(pValue) ||
				!g_pHWDeviceOEMPA->GetSWDevice()->SetConnectionState(eConnected,true))
				bError = true;
		}else
			bError = true;
	}
	if(bCheckConnect && !m_bCheckConnect)
	{
		g_pHWDeviceOEMPA->GetSWDevice()->SetConnectionState(eDisconnected,true);
		if(!DeviceOEMPADelete() || !DeviceOEMPANew(false))
			bError = true;
	}
	UpdateDialog();
	if(bError)
		AfxMessageBox(L"Error!");
}

void CHWDlg::OnBnClickedButtonWizardToHw()
{
	bool bError;

	UpdateData();
	bError = !WizardToHw();
	UpdateDialog();
	if(bError)
		AfxMessageBox(L"Error!");
}

void CHWDlg::OnBnClickedButtonOEMPAFileToHw()
{
	CString strFileFullPathName=L"*.txt";
	CFileDialog filedialog(TRUE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	CString string;

	UpdateData();
	if(filedialog.DoModal()!=IDOK)
		return;
	string = filedialog.GetFileExt();
	if((string!=L"txt") && (string!=L"bin"))
		return;
	CWaitCursor wait;
	string = filedialog.GetPathName();
	if(!OEMPA_ReadFileWriteHW(g_pHWDeviceOEMPA,string.GetBuffer()))
		AfxMessageBox(L"Error to update HW!");
	UpdateDialog();
}

void CHWDlg::OnBnClickedButtonHwToOEMPAFile()
{
	CString strFileFullPathName=L"*.txt";
	CFileDialog filedialog(FALSE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	CString string;
	DWORD dwAttributes;

	UpdateData();
	if(filedialog.DoModal()!=IDOK)
		return;
	string = filedialog.GetFileExt();
	if((string!=L"txt") && (string!=L"bin"))
		return;
	CWaitCursor wait;
	string = filedialog.GetPathName();
	dwAttributes = GetFileAttributes(string);
	if(dwAttributes!=INVALID_FILE_ATTRIBUTES)
		DeleteFile(string);
	OEMPA_ReadHWWriteFile(g_pHWDeviceOEMPA,g_iOEMPACycleCountMax,g_iOEMPAApertureElementCountMax,g_iOEMPAFocalCountMax,g_iOEMPADACCountMax,string.GetBuffer());
	EditFile(string.GetBuffer(),false);
	UpdateDialog();
}

void CHWDlg::OnBnClickedOk()
{
	CDialog::OnOK();
}

//void CHWDlg::OnBnClickedButtonTemplateToOEMPAFile()
//{
//	CString strFileFullPathName=L"*.txt";
//	CFileDialog filedialog(FALSE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||",this,0);
//	CString string;
//	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
//
//	if(filedialog.DoModal()!=IDOK)
//		return;
//	string = filedialog.GetPathName();
//	try{
//		CFile::Remove(string);
//	}catch(CFileException *e){
//		e->Delete();
//	}
//	if(CUTChannels::IsDefaultMultiChannels())
//		pMainFrame->FileSaveAs(string);
//	else
//		TemplateToOEMPAFile(string);
//}
//
bool CHWDlg::TemplateToWizard()
{
	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
	COEMPAWizardExampleView *pView;
	COEMPAWizardExampleDoc *pDoc;
	bool bRet=true;

	if(pMainFrame)
	{
		pView = pMainFrame->myGetActiveView();
		pDoc = pView->GetDocument();
		pMainFrame->FileSave(pDoc->GetPathName());//first: save the template file
		if(!ReadTemplateWriteWizard())
			bRet = false;
	}else
		bRet = false;
	return bRet;
}

void CHWDlg::OnBnClickedButtonTemplateToUpdateScan()
{
	bool bRet=false;
	int iErrorChannelProbe,iErrorChannelScan;
	CString string;

	if(TemplateToWizard())
	{
		bRet = WizardUpdateScan(&iErrorChannelProbe,&iErrorChannelScan);
		string.Format(L"Error to update probe index %d scan index %d!",iErrorChannelProbe,iErrorChannelScan);
		if(!bRet)
			AfxMessageBox(string);
	}else
		AfxMessageBox(L"Error from template to wizard!");
}

void CHWDlg::OnBnClickedButtonTemplateToWizard()
{
	if(!TemplateToWizard())
		AfxMessageBox(L"Error template to Wizard!");
}

void CHWDlg::OnBnClickedButtonWizardToOempaFile()
{
	CString strFileFullPathName=L"*.txt";
	CFileDialog filedialog(FALSE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||",this,0);
	CString string;
	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
	bool bRet=false;

	if(filedialog.DoModal()!=IDOK)
		return;
	string = filedialog.GetPathName();
	try{
		CFile::Remove(string);
	}catch(CFileException *e){
		e->Delete();
	}
	CWaitCursor wait;
	if(pMainFrame)
	{
		bRet = ReadTemplateWriteWizard();
		pMainFrame->FileSaveAs(string);
	}
	if(!bRet)
		AfxMessageBox(L"Error Wizard to OEMPA file!");
}

void CHWDlg::OnBnClickedRadioSingleChannel()
{
	OnBnClickedRadioChannel();
}

void CHWDlg::OnBnClickedRadioMultiChannel()
{
	OnBnClickedRadioChannel();
}

void CHWDlg::OnBnClickedRadioChannel()
{
	int iRadioSingleChannel=m_iRadioSingleChannel;
	bool bNotSure=false;
	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
	COEMPAWizardExampleView *pView=NULL;
	CWaitCursor wait;

	UpdateData();
	if(iRadioSingleChannel==m_iRadioSingleChannel)
		return;
	if(iRadioSingleChannel && !m_iRadioSingleChannel)
	{
		if(AfxMessageBox(L"Are you sure ?\r\n(You can get compatibility problem if you want to return back to multi-channel)",MB_YESNO)==IDNO)
			bNotSure = true;
	}else{
		if(AfxMessageBox(L"Are you sure ?\r\n(You can get compatibility problem if you want to return back to single-channel)",MB_YESNO)==IDNO)
			bNotSure = true;
	}
	if(bNotSure)
	{
		m_iRadioSingleChannel = iRadioSingleChannel;
		UpdateData(FALSE);
		return;
	}
	WizardDelete();
	CUTChannels::SetDefaultMultiChannels(m_iRadioSingleChannel?true:false);
	WizardNew();
	if(pMainFrame)
		pView = (COEMPAWizardExampleView*)pMainFrame->GetActiveView();
	if(pView)
	{
		ReadTemplateWriteWizard();
		pView->EditTemplate();
	}
	UpdateDialog();
}

void CHWDlg::OnBnClickedButtonWizardToKernelFile()
{
	CUTKernelFile file;
	CString strFileFullPathName=L"*.txt";
	CFileDialog filedialog(FALSE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||",this,0);
	CString string;
	DWORD dwAttributes;
	bool bSavedResults;

	UpdateData();
	if(filedialog.DoModal()!=IDOK)
		return;
	string = filedialog.GetFileExt();
	if(string!=L"txt")
		return;
	CWaitCursor wait;
	string = filedialog.GetPathName();
	bSavedResults = g_pChannel->IsSavedResults();
	g_pChannel->SaveResults(false);//save only the main part (setting part)
	dwAttributes = GetFileAttributes(string);
	if(dwAttributes!=INVALID_FILE_ATTRIBUTES)
		DeleteFile(string);
	//Before calling member function "Save", you have to browse the kernel and add any object you want to save
	if(!file.AddObject(g_pChannel,true))//want to save the channel
		return;
	if(!file.AddObject(g_pWizardSystem,true))//want to save the wizard
		return;
	if(!file.Save(string))
		AfxMessageBox(L"Error to save the file");
	else{
		//EditFile(string.GetBuffer(),false);
	}
	g_pChannel->SaveResults(bSavedResults);
	UpdateDialog();
}

void CHWDlg::OnBnClickedButtonWizardToKernelFileFull()
{
	CUTKernelFile file;
	CString strFileFullPathName=L"*.txt";
	CFileDialog filedialog(FALSE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	CString string;
	DWORD dwAttributes;
	bool bBinary=false;
	bool bSavedResults;

	UpdateData();
	if(filedialog.DoModal()!=IDOK)
		return;
	string = filedialog.GetFileExt();
	if((string!=L"txt") && (string!=L"bin"))
		return;
	if(string==L"bin")
		bBinary = true;
	CWaitCursor wait;
	string = filedialog.GetPathName();
	bSavedResults = g_pChannel->IsSavedResults();
	g_pChannel->SaveResults(true);//save all data
	dwAttributes = GetFileAttributes(string);
	if(dwAttributes!=INVALID_FILE_ATTRIBUTES)
		DeleteFile(string);
	//Before calling member function "Save", you have to browse the kernel and add any object you want to save
	if(!file.AddObject(g_pChannel,true))//want to save the channel
		return;
	if(!file.AddObject(g_pWizardSystem,true))//want to save the wizard
		return;
	if(!file.Save(string))
		AfxMessageBox(L"Error to save the file");
	else{
		//EditFile(string.GetBuffer(),false);
	}
	g_pChannel->SaveResults(bSavedResults);
	UpdateDialog();
}

void CHWDlg::OnBnClickedButtonKernelFileToWizard()
{
	CUTKernelFile file;
	CString strFileFullPathName=L"*.txt";
	CFileDialog filedialog(TRUE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	CString string;
	wchar_t pRootName[MAX_PATH];
	wchar_t pChannelName[g_iMaxNameLength+1];

	UpdateData();
	if(filedialog.DoModal()!=IDOK)
		return;
	string = filedialog.GetFileExt();
	if((string!=L"txt") && (string!=L"bin"))
		return;
	CWaitCursor wait;
	string = filedialog.GetPathName();
	if(!file.LoadPrepare(string,false))
		return;
	//select which items to load
	for(int iItem=0;iItem<file.GetCount();iItem++)
	{
		if(!file.GetObject(iItem,MAX_PATH,pRootName))
			return;
		if(!wcscmp(pRootName,L"OEMPA_system") && !file.SetObject(iItem,true))
			return;
		swprintf(pChannelName,g_iMaxNameLength+1,L"OEMPA_channel");
		if(!wcscmp(pRootName,pChannelName) && !file.SetObject(iItem,true))
			return;
	}
	if(file.GetSelectedCount()==2)
	{
		if(!file.LoadComplete())
			AfxMessageBox(L"Error: bad file format!");
		else
			OnBnClickedButtonWizardToTemplate();
	}else
		AfxMessageBox(L"Error: bad file format!");
}

void CHWDlg::OnBnClickedButtonWizardToTemplate()
{
	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
	COEMPAWizardExampleView *pView=NULL;
	COEMPAWizardExampleDoc *pDoc;

	if(pMainFrame)
		pView = (COEMPAWizardExampleView*)pMainFrame->GetActiveView();
	if(pView)
	{
		ReadWizardWriteTemplate(true);
		pView->EditTemplate();
		pDoc = pView->GetDocument();
		pMainFrame->FileSave(pDoc->GetPathName());//save the template file
	}
}

void CHWDlg::OnBnClickedButtonUpdateScan()
{
	CWaitCursor wait;
	int iErrorChannelProbe,iErrorChannelScan;
	CString string;
	CMainFrame *pMainFrame=(CMainFrame*)AfxGetMainWnd();
	COEMPAWizardExampleView *pView;
	COEMPAWizardExampleDoc *pDoc;
	bool bRet=false;

	if(pMainFrame)
	{
		pView = pMainFrame->myGetActiveView();
		pDoc = pView->GetDocument();
		bRet = pMainFrame->FileSave(pDoc->GetPathName());//first: save the template file
	}
	if(!bRet)
		AfxMessageBox(L"Error to save the template!");
	else if(!WizardUpdateScan(&iErrorChannelProbe,&iErrorChannelScan))
	{
		string.Format(L"Error to update probe index %d scan index %d!",iErrorChannelProbe,iErrorChannelScan);
		AfxMessageBox(string);
	}
}

void CHWDlg::OnBnClickedButtonTemplateView()
{
	AfxMessageBox(L"Use the main windows to edit the template!");
}

void CHWDlg::OnBnClickedButtonOempaFileOptions()
{
	AfxMessageBox(L"Please run the \"Manager\" to display OEMPA file options!");
}
