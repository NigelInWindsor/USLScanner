// DlgWizard.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "DlgSetupFiles.h"
#include "UTKernelLayoutFile.h"
#include "DlgHW.h"
#include "TimerProcess.h"
#include <afxrich.h>
#include <afxcview.h>
//#include "UTMFC.h"

#define WPARAM_RUN_TOOLBOX			123
#define LPARAM_RUN_TOOLBOX_OK		1234
#define LPARAM_RUN_TOOLBOX_ERROR	12345

void NotifyHWDlg(BOOL bCheckWizardEnable);
bool IsBinaryFile(const wchar_t *pFileName,int iSize,wchar_t *pDiffFileName);
void EditFile(wchar_t *pFile,bool bCloseWaiting);
void CheckCarriageReturnNotepad(const wchar_t *strFileNameIn,const wchar_t *strFileNameOut);
bool FileReadString(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *pString,int iSize,bool bDisplayMsg=true);
bool FileReadInteger(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int &iData,bool bDisplayMsg=true,int iMin=INT_MIN,int iMax=INT_MAX);
bool FileReadDouble(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,double dUnitFactor,double &dData,bool bDisplayMsg=true);
bool FileReadArrayDouble(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iCountMax,double dUnitFactor,double *pdData,int &iCountExpected);

wchar_t* GetEnumNext(wchar_t *pEnum);
bool FileReadEnum(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *pEnumList,int &iEnum,bool bDisplayErrorMsg=true);

extern wchar_t g_pDefaultString2[];
bool AddArrayDouble(double dData,int iCountMax,double *pdData,int iCountExpected,int &iIndex);
wchar_t *ReadArrayDouble(wchar_t *pValue,double &dValue);
void NotifyErrorFile(wchar_t *pSection,wchar_t *pKey);

extern wchar_t g_wcFolderData[];
extern UINT g_uiUTEventMessage;
CDlgSetupFiles *CDlgSetupFiles::m_pDlgWizard=NULL;
CUTWizardSystem *CDlgSetupFiles::m_pWizardSystem=NULL;
CUTChannels *CDlgSetupFiles::m_pChannel=NULL;
BOOL CDlgSetupFiles::m_bCheckWizardEnable=FALSE;
int CDlgSetupFiles::m_iIndexLayoutFile=-1;
bool CDlgSetupFiles::m_bConnect=false;
CRunToolbox g_RunToolbox;
int CDlgSetupFiles::m_iDeviceId=-1;

bool FileReadArrayDepth(wchar_t *pSection,wchar_t *pKey,wchar_t *pFileName,int iCountMax,double dUnitFactor,double *pdData,int &iCountExpected)
{
	wchar_t pKeyBigData[MAX_PATH];
	wchar_t line[MAX_PATH];
	int iCount;
	wchar_t *pValue;
	double dData;

	iCount = 0;
	swprintf(pKeyBigData,MAX_PATH,L"%s",pKey);
	GetPrivateProfileString(pSection,pKeyBigData,g_pDefaultString2,line,MAX_PATH,pFileName);
	if(!wcscmp(line,g_pDefaultString2))
		return false;
	iCountExpected = 1;
	pValue = ReadArrayDouble(line,dData);
	dData *= dUnitFactor;
	if(pValue)
		iCountExpected++;
	while(AddArrayDouble(dData,iCountMax,pdData,iCountExpected,iCount) && pValue)
	{
		pValue = ReadArrayDouble(pValue,dData);
		dData *= dUnitFactor;
		if(pValue)
			iCountExpected++;
	}
	if(iCountExpected>iCountMax)
	{
		NotifyErrorFile(pSection,pKey);
		return false;
	}
	for(int iIndex=0;iIndex<iCountExpected-1;iIndex++)
	{
		if(pdData[iIndex]>=pdData[iIndex+1])
		{
			NotifyErrorFile(pSection,pKey);
			return false;
		}
	}
	return true;
}

bool ChangeExtensionFile(wchar_t *pFileName,int iSizeNew,wchar_t *pFileNew,bool &bTextFile)
{
	_TCHAR drive[_MAX_DRIVE],dir[_MAX_DIR],fname[MAX_PATH],ext[_MAX_EXT];
	bool bRet=false;

	_wsplitpath_s(pFileName,drive,_MAX_DRIVE,dir,_MAX_DIR,fname,MAX_PATH,ext,_MAX_EXT);
	if(!wcscmp(ext,L".bin"))
	{
		bTextFile = false;
		bRet = true;
	}
	if(!wcscmp(ext,L".txt"))
	{
		bTextFile = true;
		bRet = true;
	}
	wcscpy_s(pFileNew,iSizeNew,drive);
	wcscat_s(pFileNew,iSizeNew,dir);
	wcscat_s(pFileNew,iSizeNew,fname);
	if(bTextFile)
		wcscat_s(pFileNew,iSizeNew,L".bin");
	else
		wcscat_s(pFileNew,iSizeNew,L".txt");
	return bRet;
}

extern wchar_t g_wcFolderExe[MAX_PATH];
typedef void (*TypeCUTMFCContainer_Save)(CWnd *pThis, wchar_t *pFileName, int iSelectionRootSize, int *aiSelectionRoot);
HMODULE g_hModuleUTKernelMFC=NULL;
TypeCUTMFCContainer_Save g_pCUTMFCContainer_Save=NULL;
bool myCUTMFCContainer_Save(CWnd *pThis, wchar_t *pFileName, int iSelectionRootSize, int *aiSelectionRoot)
{
	CString string=g_wcFolderExe;
	string += L"UTKernelMFC.dll";
	if(!g_hModuleUTKernelMFC)
		g_hModuleUTKernelMFC = LoadLibrary(string);
	if(g_hModuleUTKernelMFC && !g_pCUTMFCContainer_Save)
		g_pCUTMFCContainer_Save = (TypeCUTMFCContainer_Save)GetProcAddress(g_hModuleUTKernelMFC,"CUTMFCContainer_Save");
	if(!g_pCUTMFCContainer_Save)
		return false;
	(g_pCUTMFCContainer_Save)(pThis,pFileName,iSelectionRootSize,aiSelectionRoot);
	return true;
}

// CDlgSetupFiles dialog

IMPLEMENT_DYNAMIC(CDlgSetupFiles, CDialog)

CDlgSetupFiles::CDlgSetupFiles(int iDeviceId,CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetupFiles::IDD, pParent)
{
	m_iDeviceId = iDeviceId;
	g_uiUTEventMessage = UTKernel_RegisterWindowMessage();
	Create(IDD_DIALOG_SETUPFILES,pParent);
	ShowWindow(SW_SHOW);
}

CDlgSetupFiles::~CDlgSetupFiles()
{
	m_iIndexLayoutFile = -1;
	if(m_bCheckWizardEnable)
		WizardDelete();//explicit call: this is a quicker way to close.
}

void CDlgSetupFiles::PostNcDestroy()
{
	if(m_pDlgWizard==this)
		m_pDlgWizard = NULL;
	delete this;
}

void CDlgSetupFiles::Display(int iDeviceId)
{
	if(!m_pDlgWizard)
		m_pDlgWizard = new CDlgSetupFiles(iDeviceId);
	if(m_pDlgWizard->IsIconic())
		m_pDlgWizard->ShowWindow(SW_RESTORE);
	m_pDlgWizard->ShowWindow(SW_SHOW);
}

void CDlgSetupFiles::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SYSTEM_LOAD, m_ButtonSystemLoad);
	DDX_Control(pDX, IDC_BUTTON_SYSTEM_SAVE, m_ButtonSystemSave);
	DDX_Control(pDX, IDC_BUTTON_NOTEPAD, m_ButtonNotepad);
	DDX_Control(pDX, IDC_BUTTON_TOOLBOX, m_ButtonToolbox);
	DDX_Control(pDX, IDC_BUTTON_SCAN_LINEAR, m_ButtonLinear);
	DDX_Control(pDX, IDC_BUTTON_SCAN_SECTORIAL, m_ButtonSectorial);
	DDX_Control(pDX, IDC_BUTTON_WIZARD_UPDATE, m_ButtonWizardUpdate);
	DDX_Control(pDX, IDC_CHECK_WIZARD_ENABLE2, m_CheckWizardEnable);
	DDX_Check(pDX, IDC_CHECK_WIZARD_ENABLE, m_bCheckWizardEnable);
	DDX_Control(pDX, IDC_BUTTON_WIZARD_TO_FILE, m_ButtonWizardToFile);
	DDX_Control(pDX, IDC_BUTTON_FILE_SAMPLE, m_ButtonFileSample);
	DDX_Control(pDX, IDC_BUTTON_TXT_TO_BIN, m_ButtonTxtToBin);
	DDX_Control(pDX, IDC_BUTTON_BIN_TO_TXT, m_ButtonBinToTxt);
}


BEGIN_MESSAGE_MAP(CDlgSetupFiles, CDialog)
	ON_REGISTERED_MESSAGE(g_uiUTEventMessage, &CDlgSetupFiles::OnEventMessage)
	ON_BN_CLICKED(IDC_BUTTON_SYSTEM_LOAD, &CDlgSetupFiles::OnBnClickedButtonSystemLoad)
	ON_BN_CLICKED(IDC_BUTTON_SYSTEM_SAVE, &CDlgSetupFiles::OnBnClickedButtonSystemSave)
	ON_BN_CLICKED(IDOK, &CDlgSetupFiles::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_WIZARD_UPDATE, &CDlgSetupFiles::OnBnClickedButtonWizardUpdate)
	ON_BN_CLICKED(IDC_BUTTON_NOTEPAD, &CDlgSetupFiles::OnBnClickedButtonNotepad)
	ON_BN_CLICKED(IDC_CHECK_WIZARD_ENABLE, &CDlgSetupFiles::OnBnClickedCheckWizardEnable)
	ON_BN_CLICKED(IDC_BUTTON_TOOLBOX, &CDlgSetupFiles::OnBnClickedButtonToolbox)
	ON_BN_CLICKED(IDC_BUTTON_SCAN_LINEAR, &CDlgSetupFiles::OnBnClickedButtonScanLinear)
	ON_BN_CLICKED(IDC_BUTTON_SCAN_SECTORIAL, &CDlgSetupFiles::OnBnClickedButtonScanSectorial)
	ON_BN_CLICKED(IDC_BUTTON_WIZARD_TO_FILE, &CDlgSetupFiles::OnBnClickedButtonWizardToFile)
	ON_BN_CLICKED(IDC_BUTTON_FILE_SAMPLE, &CDlgSetupFiles::OnBnClickedButtonFileSample)
	ON_BN_CLICKED(IDC_BUTTON_TXT_TO_BIN, &CDlgSetupFiles::OnBnClickedButtonTxtToBin)
	ON_BN_CLICKED(IDC_BUTTON_BIN_TO_TXT, &CDlgSetupFiles::OnBnClickedButtonBinToTxt)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON_FILE_OPTION, &CDlgSetupFiles::OnBnClickedButtonFileOption)
END_MESSAGE_MAP()


// CDlgSetupFiles message handlers
BOOL CDlgSetupFiles::OnInitDialog(void)
{
	int iResHorz,iResVert,iData;
	WINDOWPLACEMENT wndPlacement;
	wchar_t pTitle[MAX_PATH];
	bool bRegistered,bCreateNew;
	SYSTEMTIME sysTime;

	CDialog::OnInitDialog();

	UpdateDialog_();
	if(UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),MAX_PATH,pTitle,iResHorz,iResVert,wndPlacement))
	{
		if(UTKernel_LayoutFile()->RegisterWindow(pTitle,m_iIndexLayoutFile,bRegistered,bCreateNew) && bRegistered)
		{
			if(UTKernel_LayoutFile()->ReadPositionWindow(m_iIndexLayoutFile,pTitle,sysTime,iResHorz,iResVert,wndPlacement,iData))
			if(UTKernel_LayoutFile()->CheckPlacement(wndPlacement))
				SetWindowPlacement(&wndPlacement);
		}else
			UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,pTitle,iResHorz,iResVert,wndPlacement,0);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

LRESULT CDlgSetupFiles::OnEventMessage(WPARAM wParam,LPARAM lParam)
{
	if((wParam==WPARAM_RUN_TOOLBOX) && (lParam==LPARAM_RUN_TOOLBOX_OK))
		g_RunToolbox.SignalPostFeedback();//No error
	if((wParam==WPARAM_RUN_TOOLBOX) && (lParam==LPARAM_RUN_TOOLBOX_ERROR))
		AfxMessageBox(L"Error to run the toolbox, please try again!");
	return 0;
}

DWORD WINAPI CDlgSetupFiles::CallbackThreadToolbox(HWND hWnd,CRunToolbox *pRunToolbox)
{
	int iRootId;
	CWaitCursor wait;

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
	if(m_bCheckWizardEnable)
	{
		//edit the WizardSystem and the Channel objects.
		if(m_pWizardSystem && m_pWizardSystem->GetID(iRootId))
		{
			pRunToolbox->ResetPostFeedback();
			UTKernel_ToolboxDisplay(iRootId,hWnd,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
			if(!pRunToolbox->WaitPostFeedback(5000,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_ERROR))
				return 3;
		}
		if(m_pChannel && m_pChannel->GetID(iRootId))
		{
			pRunToolbox->ResetPostFeedback();
			UTKernel_ToolboxDisplay(iRootId,hWnd,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
			if(!pRunToolbox->WaitPostFeedback(5000,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_ERROR))
				return 4;
		}
	}else{
		//edit the Preference object.
		CUTPreference *pPreference=CUTPreference::GetCurrentPreference();
		if(pPreference && pPreference->GetID(iRootId))
		{
			pRunToolbox->ResetPostFeedback();
			UTKernel_ToolboxDisplay(iRootId,hWnd,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
			if(!pRunToolbox->WaitPostFeedback(5000,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_ERROR))
				return 5;
		}
	}
	return 0;//no error
}

void CDlgSetupFiles::SetConnect(bool bConnect)
{
	m_bConnect = bConnect;
	if(m_pDlgWizard)
		m_pDlgWizard->UpdateDialog_();
}

void CDlgSetupFiles::UpdateDialog_()
{
	m_ButtonFileSample.EnableWindow(m_bConnect);
	m_ButtonTxtToBin.EnableWindow(m_bConnect);
	m_ButtonBinToTxt.EnableWindow(m_bConnect);
	m_ButtonWizardToFile.EnableWindow(m_bCheckWizardEnable && m_bConnect);
	m_ButtonSystemLoad.EnableWindow(TRUE);
	m_ButtonSystemSave.EnableWindow(m_bCheckWizardEnable);
	m_ButtonNotepad.EnableWindow(m_bCheckWizardEnable);
	m_ButtonWizardUpdate.EnableWindow(m_bCheckWizardEnable);
	m_ButtonToolbox.EnableWindow(IsWow64()?FALSE:TRUE);
	m_ButtonLinear.EnableWindow(m_bCheckWizardEnable);
	m_ButtonSectorial.EnableWindow(m_bCheckWizardEnable);
	UpdateData(FALSE);
}

bool CDlgSetupFiles::WizardDelete()
{
	bool bRet=true;

	if(!m_pWizardSystem)
		return false;
	if(!m_pChannel)
		return false;
	if(!CUTKernelFile::DeleteObject(m_pChannel,eRootTypeChannels))
		bRet = false;
	else
		m_pChannel = NULL;
	if(!CUTKernelFile::DeleteObject(m_pWizardSystem,eRootTypeWizardSystem))
		bRet = false;
	else
		m_pWizardSystem = NULL;
	m_bCheckWizardEnable = FALSE;
	return bRet;
}

bool CDlgSetupFiles::WizardNew()
{
	wchar_t pAux[MAX_PATH];
	CUTKernelRoot *ptr;
	bool bRet=true;
	int iWizardProbeIndex=0;
	bool bCreate;

	if(m_bCheckWizardEnable)
	{
		WizardDelete();
		m_bCheckWizardEnable = FALSE;
	}
	swprintf(pAux,MAX_PATH,L"OEMPA_system");
	ptr = CUTKernelRoot::SafeNewObject(eRootTypeWizardSystem,pAux,bCreate);
	m_pWizardSystem = dynamic_cast<CUTWizardSystem*>(ptr);
	if(!m_pWizardSystem)
	{
		WizardDelete();
		return false;
	}
	swprintf(pAux,g_iMaxNameLength+1,L"OEMPA_channel_%d",m_iDeviceId);
	ptr = CUTKernelRoot::SafeNewObject(eRootTypeChannels,pAux,bCreate);
	m_pChannel = dynamic_cast<CUTChannels*>(ptr);
	if(!m_pChannel)
	{
		WizardDelete();
		return false;
	}
	if(m_pChannel->LinkWizardProbe(0,m_pWizardSystem,iWizardProbeIndex))
		m_bCheckWizardEnable = TRUE;
	else
		WizardDelete();
	return bRet;
}

void CDlgSetupFiles::OnBnClickedOk()
{
	DestroyWindow();//OnOK();
}

bool CDlgSetupFiles::View(bool bEdit,CString strFileName)
{
	CString string=_T(""),strAux,strComment2;
	CString csRequest = _T("/c ");
	CString csCmdLine=_T("");
	SHELLEXECUTEINFO execinfo;
	CDWordArray adwData;

	csRequest = _T("");
	csCmdLine=_T("");
	memset(&execinfo, 0, sizeof(execinfo));
	execinfo.lpFile = _T("notepad.exe");
	execinfo.cbSize = sizeof(execinfo);
	execinfo.lpVerb = _T("open");
	execinfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	execinfo.nShow = SW_SHOWDEFAULT;
	execinfo.lpParameters = strFileName;//_T("c:\\DumpStream.txt");
	ShellExecuteEx(&execinfo);
	if(!bEdit)
		return true;
	WaitForSingleObject(execinfo.hProcess, INFINITE);
	if((AfxMessageBox(_T("Do you want to reload the edited file ?"),MB_YESNO)==IDYES))
	{
		//TerminateProcess(execinfo.hProcess,0);
		return true;
	}
	return false;
}

bool CDlgSetupFiles::RemoveFile(CString strFileName,bool bDisplayMsg)
{
	bool bRet;
	CString string;

	string.Format(L"Do you want to overwrite the following file ?\n\"%s\"",strFileName);
	try{
		CStdioFile file(strFileName,CFile::typeText | CFile::modeRead);
		file.Close();
		if(!bDisplayMsg || (bDisplayMsg && (AfxMessageBox(string,MB_YESNO)==IDYES)))
		{
			CFile::Remove(strFileName);
			bRet = true;
		}else
			bRet = false;
	}catch(CFileException *e){
		e->Delete();
		bRet = true;
	}
	return bRet;
}

void CDlgSetupFiles::OnBnClickedButtonWizardUpdate()
{
	UpdateData();
	if(!WizardUpdateScan())
		AfxMessageBox(L"Error");
	UpdateDialog_();
}

void CDlgSetupFiles::OnBnClickedButtonSystemLoad()
{
	CString strFileName;
	CString strFileFullPathName=_T("*.txt");
	_TCHAR BASED_CODE szFilter[] = _T("Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||");
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"Kernel");
	strFileFullPathName += L"*.txt";
	CFileDialog filedialog(TRUE,_T("*.txt"),strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter,this,0);

	if(filedialog.DoModal()!=IDOK)
		return;
	CWaitCursor wait;
	strFileName = filedialog.GetPathName();
	WizardLoad(strFileName);
	NotifyHWDlg(m_bCheckWizardEnable);
	UpdateDialog_();
	UTKernel_SetFolderDataFiles(L"Kernel",strFileName.GetBuffer());
}

bool CDlgSetupFiles::WizardSave(CString strFile)
{
	CString strFileName;
	CUTKernelFile file;

	UpdateData();
	strFileName = strFile;
	if(!file.AddObject(m_pWizardSystem,true))
		return false;
	if(!file.AddObject(m_pChannel,true))
		return false;
	if(file.Save(strFileName.GetBuffer()))
	{
		UTKernel_SetFolderDataFiles(L"Kernel",strFileName.GetBuffer());
		return true;
	}
	return false;
}

bool CDlgSetupFiles::WizardUpdateScan()
{
	if(!m_pChannel)
		return false;
	return (*m_pChannel).GetProbe(0)->Scan(0).UpdateScan();
	//1.1.3.2c return true;
}

bool CDlgSetupFiles::WizardLoad(CString &strFileName)
{
	int iRootWizardSystemId=-1,iRootChannelId=-1;
	bool bRet=true;
	CUTKernelFile file;
	CUTKernelRoot *ptr;
	wchar_t pRootName[MAX_PATH];
	wchar_t pChannelName[g_iMaxNameLength+1];
	enumRootType eRootID;

	UpdateData();
	if(!file.LoadPrepare(strFileName.GetBuffer(),false))
		return false;
	if(file.GetCount()!=2)
		return false;
	if(!file.GetObject(0,eRootID))
		return false;
	if((eRootID!=eRootTypeWizardSystem) && (eRootID!=eRootTypeChannels))
		return false;
	if(eRootID==eRootTypeWizardSystem)
		iRootWizardSystemId = 0;
	else
		iRootChannelId = 0;
	if(!file.GetObject(1,eRootID))
		return false;
	if(!iRootWizardSystemId && (eRootID!=eRootTypeChannels))
		return false;
	if(!iRootChannelId && (eRootID!=eRootTypeWizardSystem))
		return false;
	if(eRootID==eRootTypeWizardSystem)
		iRootWizardSystemId = 1;
	else
		iRootChannelId = 1;
	if(!file.GetObject(iRootWizardSystemId,MAX_PATH,pRootName))
		return false;
	if(wcsstr(pRootName,L"OEMPA_system")!=pRootName)
		return false;
	if(!file.GetObject(iRootChannelId,MAX_PATH,pRootName))
		return false;
	swprintf(pChannelName,g_iMaxNameLength+1,L"OEMPA_channel_%d",m_iDeviceId);
	if(wcsstr(pRootName,pChannelName)!=pRootName)
		return false;
	if(!file.SetObject(iRootWizardSystemId,true))
		return false;
	if(!file.SetObject(iRootChannelId,true))
		return false;
	if(file.LoadComplete())
	{
		ptr = CUTKernelRoot::FindRootName(L"OEMPA_system",eRootTypeWizardSystem);
		if(!m_pWizardSystem)
			m_pWizardSystem = dynamic_cast<CUTWizardSystem*>(ptr);
		else if(m_pWizardSystem != dynamic_cast<CUTWizardSystem*>(ptr))
			bRet = false;
		swprintf(pChannelName,g_iMaxNameLength+1,L"OEMPA_channel_%d",m_iDeviceId);
		ptr = CUTKernelRoot::FindRootName(pChannelName,eRootTypeChannels);
		if(!m_pChannel)
			m_pChannel = dynamic_cast<CUTChannels*>(ptr);
		else if(m_pChannel != dynamic_cast<CUTChannels*>(ptr))
			bRet = false;
		m_bCheckWizardEnable = TRUE;
	}
	UpdateDialog_();
	return bRet;
}

void CDlgSetupFiles::OnBnClickedButtonSystemSave()
{
	CString strFileName;
	CString strFileFullPathName=L"*.txt";
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"Kernel");
	strFileFullPathName += L"*.txt";
	CFileDialog filedialog(FALSE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);

	if(filedialog.DoModal()!=IDOK)
		return;
	CWaitCursor wait;
	strFileName = filedialog.GetPathName();
	UpdateData();
	WizardSave(strFileName);
	UpdateDialog_();
	UTKernel_SetFolderDataFiles(L"Kernel",strFileName.GetBuffer());
}

void CDlgSetupFiles::OnBnClickedButtonNotepad()
{
	CString strFileName;

	strFileName = g_wcFolderData;
	strFileName += L"Wizard.txt";
	DeleteFile(strFileName);
	if(!WizardSave(strFileName))
	{
		AfxMessageBox(L"Error");
		return;
	}
	if(View(true,strFileName))
		WizardLoad(strFileName);
}

void CDlgSetupFiles::OnBnClickedCheckWizardEnable()
{
	UpdateData();
	if(m_bCheckWizardEnable)
		WizardNew();
	else
		WizardDelete();
	NotifyHWDlg(m_bCheckWizardEnable);
	UpdateDialog_();
	if(m_bCheckWizardEnable)
		OnBnClickedButtonWizardUpdate();//default init
}

void CDlgSetupFiles::OnBnClickedButtonToolbox()
{
	g_RunToolbox.Run(GetSafeHwnd(),CDlgSetupFiles::CallbackThreadToolbox);
}

void CDlgSetupFiles::OnBnClickedButtonScanLinear()
{
	if(WizardFileSamples(true))
	{
		CWaitCursor wait;
		OnBnClickedButtonWizardUpdate();
	}
}

void CDlgSetupFiles::OnBnClickedButtonScanSectorial()
{
	if(WizardFileSamples(false))
	{
		CWaitCursor wait;
		OnBnClickedButtonWizardUpdate();
	}
}

void CDlgSetupFiles::WizardWriteEnum(CStdioFile &file,wchar_t *pKeyName,int iIndex,wchar_t *pEnumList,int iEnum)
{
	wchar_t *pValue;
	bool bFound=false;
	CString string;

	pValue = pEnumList;
	while(iEnum && pValue)
	{
		iEnum--;
		pValue = GetEnumNext(pValue);
	}
	if(!pValue)
		return;
	string.Format(L"%s[%d]=%s\n",pKeyName,iIndex,pValue);
	file.WriteString(string);
}

void CDlgSetupFiles::WizardWriteEnumList(CStdioFile &file,wchar_t *pEnumList)
{
	wchar_t *pValue;
	CString string;
	int iCount=0;

	string = L"#Available values: ";
	pValue = pEnumList;
	while(pValue)
	{
		if(iCount)
			string += L", \"";
		else
			string += L"\"";
		string += pValue;
		string += L"\"";
		iCount++;
		pValue = GetEnumNext(pValue);
	}
	string += L"\n";
	file.WriteString(string);
}

bool CDlgSetupFiles::WizardReadFile(
									CString &strFileName,
									bool bLinear,
									double &dSpecimenVelocity,
									int iWaveSize,
									wchar_t *wcWave,
									bool &bLongitudinalWave,
									bool &bWedgeEnable,
									double &dSpecimenRadius,
									double &dWedgeVelocity,
									double &dWedgeHeight,
									double &dWedgeAngle,
									int &iProbeElementCount,
									double &dProbePitch,
									double &dProbeFrequency,
									double &dProbeRadius,
									double &dScanDepthEmission,
									double *adScanDepthReception,
									int &iScanDepthReception,
									int &iScanElementStart,
									int &iScanElementStop,
									int &iScanElementStep,
									int &iScanElementCount,
									double &dScanAngleStart,
									double &dScanAngleStop,
									double &dScanAngleStep,
									double &dAscanStart,
									double &dAscanRange,
									double &dTimeSlot,
									int &iCscanCount,
									double *adCscanStart,
									double *adCscanRange,
									double *adCscanThreshold,
									enumRectification *aeRectification,
									enumGateModeAmp *aeModeAmp,
									enumGateModeTof *aeModeTof,
									enumDepthMode &eDepthMode
									)
{
	int iEnum;
	int iWedgeEnable;
	CString string;
	wchar_t wcDepthMode[20];

	eDepthMode = eStandardDepth;
	if(!FileReadDouble(L"Specimen",L"Velocity",strFileName.GetBuffer(),1.0,dSpecimenVelocity))
		return false;
	if(!FileReadString(L"Specimen",L"Wave",strFileName.GetBuffer(),wcWave,iWaveSize))
		return false;
	if(wcWave[0]==L'L')
		bLongitudinalWave = true;
	else if(wcWave[0]==L'T')
		bLongitudinalWave = false;
	else
		return false;
	//if(bLinear)
	//{
		if(!FileReadDouble(L"Specimen",L"Radius",strFileName.GetBuffer(),1.0,dSpecimenRadius))
			return false;
		dSpecimenRadius /= 1000.0;
	//}
	if(!FileReadInteger(L"Wedge",L"Enable",strFileName.GetBuffer(),iWedgeEnable))
		return false;
	if(iWedgeEnable)
		bWedgeEnable = true;
	else
		bWedgeEnable = false;
	if(!FileReadDouble(L"Wedge",L"Velocity",strFileName.GetBuffer(),1.0,dWedgeVelocity))
		return false;
	if(!FileReadDouble(L"Wedge",L"Height",strFileName.GetBuffer(),1.0e-3,dWedgeHeight))
		return false;
	if(!FileReadDouble(L"Wedge",L"Angle",strFileName.GetBuffer(),M_PI/180.0,dWedgeAngle))
		return false;
	if(!FileReadInteger(L"Probe",L"ElementCount",strFileName.GetBuffer(),iProbeElementCount))
		return false;
	if(!FileReadDouble(L"Probe",L"Pitch",strFileName.GetBuffer(),1.0e-3,dProbePitch))
		return false;
	if(!FileReadDouble(L"Probe",L"Frequency",strFileName.GetBuffer(),1.0e6,dProbeFrequency))
		return false;
	if(bLinear)
	{
		if(!FileReadDouble(L"Probe",L"Radius",strFileName.GetBuffer(),1.0e-3,dProbeRadius))
			return false;
		if(!FileReadDouble(L"Scan\\Linear",L"DepthEmission",strFileName.GetBuffer(),1.0e-3,dScanDepthEmission))
			return false;
		if(!FileReadArrayDepth(L"Scan\\Linear",L"DepthReception",strFileName.GetBuffer(),32,1.0e-3,adScanDepthReception,iScanDepthReception))
			return false;
		if(!FileReadInteger(L"Scan\\Linear",L"Count",strFileName.GetBuffer(),iScanElementCount))
			return false;
		if(!FileReadDouble(L"Scan\\Linear",L"Angle",strFileName.GetBuffer(),M_PI/180.0,dScanAngleStart))
			return false;
		if(!FileReadInteger(L"Scan\\Linear",L"ElementStart",strFileName.GetBuffer(),iScanElementStart))
			return false;
		iScanElementStart--;
		if(!FileReadInteger(L"Scan\\Linear",L"ElementStop",strFileName.GetBuffer(),iScanElementStop))
			return false;
		iScanElementStop--;
		if(!FileReadInteger(L"Scan\\Linear",L"ElementStep",strFileName.GetBuffer(),iScanElementStep))
			return false;
		if(!iScanElementStep)
		{
			AfxMessageBox(L"Error element step canno't be null!");
			return false;
		}
		if(!FileReadString(L"Scan\\Linear",L"DepthMode",strFileName.GetBuffer(),wcDepthMode,20,false))
			wcscpy_s(wcDepthMode,L"TrueDepth");
	}else{
		if(!FileReadDouble(L"Probe",L"Radius",strFileName.GetBuffer(),1.0e-3,dProbeRadius))
			return false;
		if(!FileReadDouble(L"Scan\\Sectorial",L"DepthEmission",strFileName.GetBuffer(),1.0e-3,dScanDepthEmission))
			return false;
		if(!FileReadArrayDepth(L"Scan\\Sectorial",L"DepthReception",strFileName.GetBuffer(),32,1.0e-3,adScanDepthReception,iScanDepthReception))
			return false;
		if(!FileReadInteger(L"Scan\\Sectorial",L"Count",strFileName.GetBuffer(),iScanElementCount))
			return false;
		if(!FileReadInteger(L"Scan\\Sectorial",L"Element",strFileName.GetBuffer(),iScanElementStart))
			return false;
		iScanElementStart--;
		if(!FileReadDouble(L"Scan\\Sectorial",L"AngleStart",strFileName.GetBuffer(),M_PI/180.0,dScanAngleStart))
			return false;
		if(!FileReadDouble(L"Scan\\Sectorial",L"AngleStop",strFileName.GetBuffer(),M_PI/180.0,dScanAngleStop))
			return false;
		if(!FileReadDouble(L"Scan\\Sectorial",L"AngleStep",strFileName.GetBuffer(),M_PI/180.0,dScanAngleStep))
			return false;
		if(!FileReadString(L"Scan\\Sectorial",L"DepthMode",strFileName.GetBuffer(),wcDepthMode,20,false))
			wcscpy_s(wcDepthMode,L"TrueDepth");
	}
	if(!wcscmp(wcDepthMode,L"TrueDepth"))
		eDepthMode = eStandardDepth;
	else
	if(!wcscmp(wcDepthMode,L"SoundPath"))
		eDepthMode = eSoundPath;
	else
	if(!wcscmp(wcDepthMode,L"TrueDepthBigBar"))
	{
		if(dSpecimenRadius>0.0)
			eDepthMode = eDepthBigBar;
	}else
		return false;
	if(!FileReadDouble(L"Ascan",L"Start",strFileName.GetBuffer(),1.0e-6,dAscanStart))
		return false;
	if(!FileReadDouble(L"Ascan",L"Range",strFileName.GetBuffer(),1.0e-6,dAscanRange))
		return false;
	if(!FileReadDouble(L"Ascan",L"TimeSlot",strFileName.GetBuffer(),1.0e-6,dTimeSlot))
		return false;
	if(!FileReadInteger(L"Cscan",L"Count",strFileName.GetBuffer(),iCscanCount))
		return false;
//	wchar_t ArrayOne[4][] = { L"Signed",L"Unsigned",L"UnsignedPositive",L"UnsignedNegative" };

	for(int iGate=0;iGate<iCscanCount;iGate++)
	{
		string.Format(L"Start[%d]",iGate);
		if(!FileReadDouble(L"Cscan",string.GetBuffer(),strFileName.GetBuffer(),1.0e-6,adCscanStart[iGate]))
			return false;
		string.Format(L"Range[%d]",iGate);
		if(!FileReadDouble(L"Cscan",string.GetBuffer(),strFileName.GetBuffer(),1.0e-6,adCscanRange[iGate]))
			return false;
		string.Format(L"Threshold[%d]",iGate);
		if(!FileReadDouble(L"Cscan",string.GetBuffer(),strFileName.GetBuffer(),1.0,adCscanThreshold[iGate]))
			return false;
		string.Format(L"Rectification[%d]",iGate);
		if(!FileReadEnum(L"Cscan",string.GetBuffer(),strFileName.GetBuffer(),L"Signed""\0""Unsigned""\0""UnsignedPositive""\0""UnsignedNegative""\0",iEnum))
			return false;
		aeRectification[iGate] = (enumRectification)iEnum;
		string.Format(L"ModeAmp[%d]",iGate);
		if(!FileReadEnum(L"Cscan",string.GetBuffer(),strFileName.GetBuffer(),L"Absolute""\0""Maximum""\0""Minimum""\0""PeakToPeak""\0",iEnum))
			return false;
		aeModeAmp[iGate] = (enumGateModeAmp)iEnum;
		string.Format(L"ModeTof[%d]",iGate);
		if(!FileReadEnum(L"Cscan",string.GetBuffer(),strFileName.GetBuffer(),L"AmplitudeDetection""\0""ThresholdCross""\0""ZeroFirstAfterThresholdCross""\0""ZeroLastBeforeThresholdCross""\0",iEnum))
			return false;
		aeModeTof[iGate] = (enumGateModeTof)iEnum;
	}
	return true;
}

bool CDlgSetupFiles::WizardWriteFile(
									CString &strFileName,
									bool bLinear,
									double &dSpecimenVelocity,
									int iWaveSize,
									wchar_t *wcWave,
									bool &bLongitudinalWave,
									bool &bWedgeEnable,
									double &dSpecimenRadius,
									double &dWedgeVelocity,
									double &dWedgeHeight,
									double &dWedgeAngle,
									int &iProbeElementCount,
									double &dProbePitch,
									double &dProbeFrequency,
									double &dProbeRadius,
									double &dScanDepthEmission,
									double *adScanDepthReception,
									int &iScanDepthReception,
									int &iScanElementStart,
									int &iScanElementStop,
									int &iScanElementStep,
									int &iScanElementCount,
									double &dScanAngleStart,
									double &dScanAngleStop,
									double &dScanAngleStep,
									double &dAscanStart,
									double &dAscanRange,
									double &dTimeSlot,
									int &iCscanCount,
									double *adCscanStart,
									double *adCscanRange,
									double *adCscanThreshold,
									enumRectification *aeRectification,
									enumGateModeAmp *aeModeAmp,
									enumGateModeTof *aeModeTof,
									enumDepthMode &eDepthMode
									)
{
	CString string;

	try{
		CStdioFile file(strFileName,CFile::typeText | CFile::modeWrite | CFile::modeCreate);
		if(bLinear)
			file.WriteString(L"# TEMPLATE - SINGLE-CHANNEL - LINEAR\n");
		else
			file.WriteString(L"# TEMPLATE - SINGLE-CHANNEL - SECTORIAL\n");
		file.WriteString(L"#Fill in following input parameters, save the file and\n");
		file.WriteString(L"#close notepad to go on!\n");
		file.WriteString(L"\n");
		file.WriteString(L"[Specimen]\n");
		file.WriteString(L"#\"Velocity\" is the velocity in the specimen (unit=meter/sec).\n");
		string.Format(L"Velocity=%.1lf m/s\n",dSpecimenVelocity);
		file.WriteString(string);
		file.WriteString(L"#\"Wave\" is the wave ('L' for Longitudinal and 'T' for Transverse).\n");
		if(bLongitudinalWave)
			string.Format(L"Wave=L\n");
		else
			string.Format(L"Wave=T\n");
		file.WriteString(string);
		//if(bLinear)
		//{
			file.WriteString(L"#\"Radius\" is 0.0 for a plane (unit=millimeter).\n");
			string.Format(L"Radius=%.1lf mm\n",dSpecimenRadius*1000.0);
			file.WriteString(string);
		//}
		file.WriteString(L"\n");
		file.WriteString(L"[Wedge]\n");
		file.WriteString(L"#\"Enable\" should be used if the wedge is used (0 to disable or 1 to enable).\n");
		if(bWedgeEnable)
			string.Format(L"Enable=1\n");
		else
			string.Format(L"Enable=0\n");
		file.WriteString(string);
		file.WriteString(L"#\"Velocity\" is the velocity in the wedge (unit=meter/sec).\n");
		string.Format(L"Velocity=%.1lf m/s\n",dWedgeVelocity);
		file.WriteString(string);
		file.WriteString(L"#\"Height\" is the probe height over the specimen for the middle of the probe (unit=milliter).\n");
		string.Format(L"Height=%.1lf mm\n",dWedgeHeight*1.0e3);
		file.WriteString(string);
		file.WriteString(L"#\"Angle\" is the probe angle over the specimen (unit=degree).\n");
		string.Format(L"Angle=%.1lf deg\n",dWedgeAngle*180.0/M_PI);
		file.WriteString(string);
		file.WriteString(L"\n");
		file.WriteString(L"[Probe]\n");
		file.WriteString(L"#\"ElementCount\" is the element count (linear phased array).\n");
		string.Format(L"ElementCount=%d\n",iProbeElementCount);
		file.WriteString(string);
		file.WriteString(L"#\"Pitch\" is the element pitch.\n");
		string.Format(L"Pitch=%.1lf mm\n",dProbePitch*1.0e3);
		file.WriteString(string);
		file.WriteString(L"#\"Frequency\" is the frequency (used to set the pulse width) unit is Megahertz.\n");
		string.Format(L"Frequency=%.2lf MHz\n",dProbeFrequency*1.0e-6);
		file.WriteString(string);
		//if(bLinear)
		//{
			file.WriteString(L"#\"Radius\" is 0.0 for a plane probe (unit=millimeter).\n");
			string.Format(L"Radius=%.1lf mm\n",dProbeRadius*1000.0);
			file.WriteString(string);
		//}
		file.WriteString(L"\n");
		if(bLinear)
			file.WriteString(L"[Scan\\Linear]\n");
		else
			file.WriteString(L"[Scan\\Sectorial]\n");
		file.WriteString(L"#\"Count\" is the aperture element count.\n");
		string.Format(L"Count=%d\n",iScanElementCount);
		file.WriteString(string);
		file.WriteString(L"#\"DepthEmission\" is the emission depth of all apertures (unit=millimeter).\n");
		string.Format(L"DepthEmission=%.1lf mm\n",dScanDepthEmission*1.0e3);
		file.WriteString(string);
		file.WriteString(L"#\"DepthReception\" multiples depths is possible for DDF (unit=millimeter).\n");
		file.WriteString(L"#\"DepthReception=60.0;80.0;100.0 mm\" <= DDF example\n");
		file.WriteString(L"DepthReception=");
		for(int iIndex=0;iIndex<iScanDepthReception;iIndex++)
		{
			if(iIndex==iScanDepthReception-1)
				string.Format(L"%.1lf",adScanDepthReception[iIndex]*1.0e3);
			else
				string.Format(L"%.1lf;",adScanDepthReception[iIndex]*1.0e3);
			file.WriteString(string);
		}
		file.WriteString(L" mm\n");
		if(bLinear)
		{
			string.Format(L" mm\n");
			file.WriteString(L"#\"Angle\" is the angle of all apertures (unit=degree).\n");
			string.Format(L"Angle=%.1lf deg\n",dScanAngleStart*180.0/M_PI);
			file.WriteString(string);
			file.WriteString(L"#\"ElementStart\" is the first element of the first aperture.\n");
			string.Format(L"ElementStart=%d\n",iScanElementStart);
			file.WriteString(string);
			file.WriteString(L"#\"ElementStop\" is the first element of the last aperture.\n");
			string.Format(L"ElementStop=%d\n",iScanElementStop);
			file.WriteString(string);
			file.WriteString(L"#\"ElementStep\" is the element step.\n");
			string.Format(L"ElementStep=%d\n",iScanElementStep);
			file.WriteString(string);
			file.WriteString(L"#\"DepthMode\" could be \"TrueDepth\", \"SoundPath\" or \"TrueDepthBigBar\".\n");
			switch(eDepthMode)
			{
			case eStandardDepth:	string.Format(L"DepthMode=TrueDepth\n");break;
			case eSoundPath:		string.Format(L"DepthMode=SoundPath\n");break;
			case eDepthBigBar:		string.Format(L"DepthMode=TrueDepthBigBar\n");break;
			default: return false;
			}
			file.WriteString(string);
		}else{
			file.WriteString(L"#\"Element\" is the first element of all apertures.\n");
			string.Format(L"Element=%d\n",iScanElementStart);
			file.WriteString(string);
			file.WriteString(L"#\"AngleStart\" is the angle of the first aperture (unit=degree).\n");
			string.Format(L"AngleStart=%.1lf deg\n",dScanAngleStart*180.0/M_PI);
			file.WriteString(string);
			file.WriteString(L"#\"AngleStop\" is the angle of the last aperture (unit=degree).\n");
			string.Format(L"AngleStop=%.1lf deg\n",dScanAngleStop*180.0/M_PI);
			file.WriteString(string);
			file.WriteString(L"#\"AngleStep\" is the angle step (unit=degree).\n");
			string.Format(L"AngleStep=%.1lf deg\n",dScanAngleStep*180.0/M_PI);
			file.WriteString(string);
			file.WriteString(L"#\"DepthMode\" could be \"TrueDepth\", \"SoundPath\" or \"TrueDepthBigBar\".\n");
			switch(eDepthMode)
			{
			case eStandardDepth:	string.Format(L"DepthMode=TrueDepth\n");break;
			case eSoundPath:		string.Format(L"DepthMode=SoundPath\n");break;
			case eDepthBigBar:		string.Format(L"DepthMode=TrueDepthBigBar\n");break;
			default: return false;
			}
			file.WriteString(string);
		}
		file.WriteString(L"\n[Ascan]\n");
		file.WriteString(L"#\"Start\" is the digitizing start time in microsecond.\n");
		string.Format(L"Start=%.3lf 탎\n",dAscanStart*1.0e6);
		file.WriteString(string);
		file.WriteString(L"#\"Range\" is the digitizing range in microsecond.\n");
		string.Format(L"Range=%.3lf 탎\n",dAscanRange*1.0e6);
		file.WriteString(string);
		file.WriteString(L"#\"TimeSlot\" is the time between two pulse shot in microsecond.\n");
		string.Format(L"TimeSlot=%.3lf 탎\n",dTimeSlot*1.0e6);
		file.WriteString(string);

		file.WriteString(L"\n[Cscan]\n");
		file.WriteString(L"#\"Count\" is the Cscan gate count (range is 0 to 4).\n");
		string.Format(L"Count=%d\n",iCscanCount);
		file.WriteString(string);
		file.WriteString(L"#\"Start[0]\" is the start time in microsecond for the first Cscan gate.\n");
		for(int iGate=0;iGate<4;iGate++)
		{
			string.Format(L"Start[%d]=%.3lf 탎\n",iGate,adCscanStart[iGate]*1.0e6);
			file.WriteString(string);
		}
		file.WriteString(L"#\"Range[0]\" is the range in microsecond for the first Cscan gate.\n");
		for(int iGate=0;iGate<4;iGate++)
		{
			string.Format(L"Range[%d]=%.3lf 탎\n",iGate,adCscanRange[iGate]*1.0e6);
			file.WriteString(string);
		}
		file.WriteString(L"#\"Threshold[0]\" is the threshold in percent for the first Cscan gate.\n");
		for(int iGate=0;iGate<4;iGate++)
		{
			string.Format(L"Threshold[%d]=%.1lf %%\n",iGate,adCscanThreshold[iGate]);
			file.WriteString(string);
		}
		file.WriteString(L"#\"Rectification[0]\" is the Rectification mode for the first Cscan gate.\n");
		WizardWriteEnumList(file,L"Signed""\0""Unsigned""\0""UnsignedPositive""\0""UnsignedNegative""\0");
		for(int iGate=0;iGate<4;iGate++)
		{
			WizardWriteEnum(file,L"Rectification",iGate,L"Signed""\0""Unsigned""\0""UnsignedPositive""\0""UnsignedNegative""\0",(int)aeRectification[iGate]);
		}
		file.WriteString(L"#\"ModeAmp[0]\" is the Amplitude mode for the first Cscan gate.\n");
		WizardWriteEnumList(file,L"Absolute""\0""Maximum""\0""Minimum""\0""PeakToPeak""\0");
		for(int iGate=0;iGate<4;iGate++)
		{
			WizardWriteEnum(file,L"ModeAmp",iGate,L"Absolute""\0""Maximum""\0""Minimum""\0""PeakToPeak""\0",(int)aeModeAmp[iGate]);
		}
		file.WriteString(L"#\"ModeTof[0]\" is the TimeOfFlight mode for the first Cscan gate.\n");
		WizardWriteEnumList(file,L"AmplitudeDetection""\0""ThresholdCross""\0""ZeroFirstAfterThresholdCross""\0""ZeroLastBeforeThresholdCross""\0");
		for(int iGate=0;iGate<4;iGate++)
		{
			WizardWriteEnum(file,L"ModeTof",iGate,L"AmplitudeDetection""\0""ThresholdCross""\0""ZeroFirstAfterThresholdCross""\0""ZeroLastBeforeThresholdCross""\0",(int)aeModeTof[iGate]);
		}

		file.Close();
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
	}
	return true;
}
bool CDlgSetupFiles::dEqual(double d1,double d2,double dPrecision)
{
	double dDelta=fabs(d1-d2);
	if(dDelta<dPrecision)
		return true;
	return false;
}
bool CDlgSetupFiles::WizardCompare(
									CString &strFileName,
									bool bLinear2,
									double &dSpecimenVelocity2,
									int iWaveSize2,
									wchar_t *wcWave2,
									bool &bLongitudinalWave2,
									bool &bWedgeEnable2,
									double &dSpecimenRadius2,
									double &dWedgeVelocity2,
									double &dWedgeHeight2,
									double &dWedgeAngle2,
									int &iProbeElementCount2,
									double &dProbePitch2,
									double &dProbeFrequency2,
									double &dProbeRadius2,
									double &dScanDepthEmission2,
									double *adScanDepthReception2,
									int &iScanDepthReception2,
									int iScanElementStart2,
									int iScanElementStop2,
									int &iScanElementStep2,
									int &iScanElementCount2,
									double &dScanAngleStart2,
									double &dScanAngleStop2,
									double &dScanAngleStep2,
									double &dAscanStart2,
									double &dAscanRange2,
									double &dTimeSlot2,
									int &iCscanCount2,
									double *adCscanStart2,
									double *adCscanRange2,
									double *adCscanThreshold2,
									enumRectification *aeRectification2,
									enumGateModeAmp *aeModeAmp2,
									enumGateModeTof *aeModeTof2,
									enumDepthMode &eDepthMode2
									)
{
	bool bLinear;
	double dSpecimenVelocity;
	wchar_t wcWave[50];
	bool bLongitudinalWave;
	bool bWedgeEnable;
	double dSpecimenRadius;
	double dWedgeVelocity;
	double dWedgeHeight;
	double dWedgeAngle;
	int iProbeElementCount;
	double dProbePitch;
	double dProbeFrequency;
	double dProbeRadius;
	double dScanDepthEmission;
	double adScanDepthReception[32];
	int iScanDepthReception;
	int iScanElementStart;
	int iScanElementStop;
	int iScanElementStep;
	int iScanElementCount;
	double dScanAngleStart;
	double dScanAngleStop;
	double dScanAngleStep;
	double dAscanStart;
	double dAscanRange;
	double dTimeSlot;
	int iCscanCount;
	double adCscanStart[4],adCscanRange[4],adCscanThreshold[4];
	enumRectification aeRectification[4];
	enumGateModeAmp aeModeAmp[4];
	enumGateModeTof aeModeTof[4];
	enumDepthMode eDepthMode;
	bool bExist;
	double dAux;

	bExist = false;
	if(FileReadDouble(L"Scan\\Linear",L"DepthEmission",strFileName.GetBuffer(),1.0e-3,dScanDepthEmission,false))
	{
		bExist = true;
		bLinear = true;
	}else
	if(FileReadDouble(L"Scan\\Sectorial",L"DepthEmission",strFileName.GetBuffer(),1.0e-3,dScanDepthEmission,false))
	{
		bExist = true;
		bLinear = false;
	}
	//1.1.5.1r start
	if(!FileReadDouble(L"Specimen",L"Radius",strFileName.GetBuffer(),1.0e-3,dAux,false))
		return false;
	if(!bExist)
		return false;
	if(!WizardReadFile(
									strFileName,
									bLinear,
									dSpecimenVelocity,
									50,
									wcWave,
									bLongitudinalWave,
									bWedgeEnable,
									dSpecimenRadius,
									dWedgeVelocity,
									dWedgeHeight,
									dWedgeAngle,
									iProbeElementCount,
									dProbePitch,
									dProbeFrequency,
									dProbeRadius,
									dScanDepthEmission,
									adScanDepthReception,
									iScanDepthReception,
									iScanElementStart,
									iScanElementStop,
									iScanElementStep,
									iScanElementCount,
									dScanAngleStart,
									dScanAngleStop,
									dScanAngleStep,
									dAscanStart,
									dAscanRange,
									dTimeSlot,
									iCscanCount,
									adCscanStart,
									adCscanRange,
									adCscanThreshold,
									aeRectification,
									aeModeAmp,
									aeModeTof,
									eDepthMode
									))
		return false;
	if(bLinear2!=bLinear)
		return false;
	if(!dEqual(dSpecimenVelocity2,dSpecimenVelocity,0.1))
		return false;
	if(bLongitudinalWave2!=bLongitudinalWave)
		return false;
	if(bWedgeEnable2!=bWedgeEnable)
		return false;
	if(!dEqual(dSpecimenRadius2,dSpecimenRadius,0.01e-3))
		return false;
	if(!dEqual(dWedgeVelocity2,dWedgeVelocity,0.1))
		return false;
	if(!dEqual(dWedgeHeight2,dWedgeHeight,0.01e-3))
		return false;
	if(!dEqual(dWedgeAngle2,dWedgeAngle,0.1*M_PI/180.0))
		return false;
	if(iProbeElementCount2!=iProbeElementCount)
		return false;
	if(!dEqual(dProbePitch2,dProbePitch,0.01e-3))
		return false;
	if(!dEqual(dProbeFrequency2,dProbeFrequency,1.0e5))
		return false;
	if(!dEqual(dProbeRadius2,dProbeRadius,0.01e-3))
		return false;
	if(!dEqual(dScanDepthEmission2,dScanDepthEmission,0.01e-3))
		return false;
	if(iScanDepthReception2!=iScanDepthReception)
		return false;
	for(int iIndex=0;iIndex<iScanDepthReception;iIndex++)
	{
		if(!dEqual(adScanDepthReception2[iIndex],adScanDepthReception[iIndex],0.01e-3))
			return false;
	}
	if(iScanElementStart2!=iScanElementStart)
		return false;
	if(bLinear && (iScanElementStop2!=iScanElementStop))
		return false;
	if(bLinear && (iScanElementStep2!=iScanElementStep))
		return false;
	if(iScanElementCount2!=iScanElementCount)
		return false;
	if(!dEqual(dScanAngleStart2,dScanAngleStart,0.1*M_PI/180.0))
		return false;
	if(!bLinear && !dEqual(dScanAngleStop2,dScanAngleStop,0.1*M_PI/180.0))
		return false;
	if(!bLinear && !dEqual(dScanAngleStep2,dScanAngleStep,0.1*M_PI/180.0))
		return false;
	if(!dEqual(dAscanStart2,dAscanStart,10.0e-9))
		return false;
	if(!dEqual(dAscanRange2,dAscanRange,10.0e-9))
		return false;
	if(!dEqual(dTimeSlot2,dTimeSlot,10.0e-9))
		return false;
	//if(iCscanCount2!=iCscanCount)
	//	return false;
	//if(adCscanStart2!=adCscanStart)
	//	return false;
	//if(adCscanRange2!=adCscanRange)
	//	return false;
	//if(adCscanThreshold2!=adCscanThreshold)
	//	return false;
	//if(aeRectification2!=aeRectification)
	//	return false;
	//if(aeModeAmp2!=aeModeAmp)
	//	return false;
	//if(aeModeTof2!=aeModeTof)
	//	return false;
	if(eDepthMode!=eDepthMode2)
		return false;
	return true;
}

bool CDlgSetupFiles::WizardFileSamples(bool bLinear,bool bRequest)
{
	CString strFileName,strFileName2,string,string2;
	wchar_t wcWave[50];
	bool bLongitudinalWave;
	int iProbeElementCount;
	double dSpecimenVelocity,dProbePitch,dProbeFrequency,dAux;
	bool bWedgeEnable;
	double dWedgeHeight,dWedgeAngle,dWedgeVelocity;
	double dScanDepthEmission,adScanDepthReception[32];
	int iScanDepthReception,iScanElementCount;
	int iScanElementStart,iScanElementStop,iScanElementStep;
	double dScanAngleStart,dScanAngleStop,dScanAngleStep;
	double dAscanStart,dAscanRange,dTimeSlot;
	double adCscanStart[4],adCscanRange[4],adCscanThreshold[4];
	enumRectification aeRectification[4];
	enumGateModeAmp aeModeAmp[4];
	enumGateModeTof aeModeTof[4];
	int iCscanCount;
	enumDepthMode eDepthMode;
	double dSpecimenRadius,dProbeRadius;
	bool bExist;
	int iCountLinear,iCountSectorial,iCountFocalLaw;
	CUTScan *pScan;

	if(!OEMPA_ReadWizard(	m_pWizardSystem,m_pChannel,
					bLongitudinalWave,dSpecimenVelocity,
					iProbeElementCount,dProbePitch,dProbeFrequency,
					bWedgeEnable,
					dWedgeHeight,dWedgeAngle,dWedgeVelocity,
					dScanDepthEmission,32,adScanDepthReception,iScanDepthReception,
					iScanElementCount,
					iScanElementStart,iScanElementStop,iScanElementStep,
					dScanAngleStart,dScanAngleStop,dScanAngleStep,
					dAscanStart,dAscanRange,dTimeSlot,
					4,adCscanStart,adCscanRange,adCscanThreshold,
					aeRectification,aeModeAmp,aeModeTof,iCscanCount,
					eDepthMode))
		return false;
	switch(m_pWizardSystem->Specimen().GetSpecimen())
	{
	case ePlane: dSpecimenRadius = 0.0;break;
	case eDisk:
	case eBar:
	case eTube: m_pWizardSystem->Specimen().GetRadius(dSpecimenRadius);break;
	}
	if(!m_pWizardSystem->Probe(0).GetRadius(dProbeRadius))
		dProbeRadius = 0.0;
	if(bLinear && !iScanElementStep)
		iScanElementStep = 1;
	iScanElementStart++;
	iScanElementStop++;
	strFileName = g_wcFolderData;
	strFileName += L"Wizard.txt";
	if(bRequest)
	{
		//Is it linear or sectorial?
		if(!m_pChannel->IsMultiChannels())
			pScan = &m_pChannel->Probe().Scan();//single channel
		else
			pScan = &m_pChannel->Probe(0).Scan(0);//multi channel
		if(!pScan)
			return false;
		iCountFocalLaw = pScan->GetReceiverCycleCount();
		if(dScanAngleStep)
			iCountSectorial = (int)lround2((dScanAngleStop-dScanAngleStart)/dScanAngleStep)+1;
		else
			iCountSectorial = -1;
		if(iScanElementStep)
			iCountLinear = (iScanElementStop-iScanElementStart)/iScanElementStep+1;
		else
			iCountLinear = -1;
		if(FileReadDouble(L"Scan\\Linear",L"DepthEmission",strFileName.GetBuffer(),1.0e-3,dScanDepthEmission,false))
		{
			bLinear = true;
			if((iCountFocalLaw>0) && (iCountFocalLaw!=iCountLinear))
				return false;
		}else
		if(FileReadDouble(L"Scan\\Sectorial",L"DepthEmission",strFileName.GetBuffer(),1.0e-3,dScanDepthEmission,false))
		{
			bLinear = false;
			if((iCountFocalLaw>0) && (iCountFocalLaw!=iCountSectorial))
				return false;
		}
	}
	bExist = false;
	if(bLinear && FileReadDouble(L"Scan\\Linear",L"DepthEmission",strFileName.GetBuffer(),1.0e-3,dScanDepthEmission,false))
		bExist = true;
	if(!bLinear && FileReadDouble(L"Scan\\Sectorial",L"DepthEmission",strFileName.GetBuffer(),1.0e-3,dScanDepthEmission,false))
		bExist = true;
	//1.1.5.1r start
	if(!FileReadDouble(L"Specimen",L"Radius",strFileName.GetBuffer(),1.0e-3,dAux,false))
		bExist = false;
	//1.1.5.1r stop
	if(bRequest)
	{
		if(WizardCompare(
										strFileName,
										bLinear,
										dSpecimenVelocity,
										50,
										wcWave,
										bLongitudinalWave,
										bWedgeEnable,
										dSpecimenRadius,
										dWedgeVelocity,
										dWedgeHeight,
										dWedgeAngle,
										iProbeElementCount,
										dProbePitch,
										dProbeFrequency,
										dProbeRadius,
										dScanDepthEmission,
										adScanDepthReception,
										iScanDepthReception,
										iScanElementStart-1,
										iScanElementStop-1,
										iScanElementStep,
										iScanElementCount,
										dScanAngleStart,
										dScanAngleStop,
										dScanAngleStep,
										dAscanStart,
										dAscanRange,
										dTimeSlot,
										iCscanCount,
										adCscanStart,
										adCscanRange,
										adCscanThreshold,
										aeRectification,
										aeModeAmp,
										aeModeTof,
										eDepthMode
										))
			return true;
		return false;
	}
    if (!bExist)
	{
		if(!WizardWriteFile(
										strFileName,
										bLinear,
										dSpecimenVelocity,
										50,
										wcWave,
										bLongitudinalWave,
										bWedgeEnable,
										dSpecimenRadius,
										dWedgeVelocity,
										dWedgeHeight,
										dWedgeAngle,
										iProbeElementCount,
										dProbePitch,
										dProbeFrequency,
										dProbeRadius,
										dScanDepthEmission,
										adScanDepthReception,
										iScanDepthReception,
										iScanElementStart,
										iScanElementStop,
										iScanElementStep,
										iScanElementCount,
										dScanAngleStart,
										dScanAngleStop,
										dScanAngleStep,
										dAscanStart,
										dAscanRange,
										dTimeSlot,
										iCscanCount,
										adCscanStart,
										adCscanRange,
										adCscanThreshold,
										aeRectification,
										aeModeAmp,
										aeModeTof,
										eDepthMode
										))
			return false;
	}
	EditFile(strFileName.GetBuffer(),true);
	CheckCarriageReturnNotepad(strFileName,strFileName);
	if(!WizardReadFile(
									strFileName,
									bLinear,
									dSpecimenVelocity,
									50,
									wcWave,
									bLongitudinalWave,
									bWedgeEnable,
									dSpecimenRadius,
									dWedgeVelocity,
									dWedgeHeight,
									dWedgeAngle,
									iProbeElementCount,
									dProbePitch,
									dProbeFrequency,
									dProbeRadius,
									dScanDepthEmission,
									adScanDepthReception,
									iScanDepthReception,
									iScanElementStart,
									iScanElementStop,
									iScanElementStep,
									iScanElementCount,
									dScanAngleStart,
									dScanAngleStop,
									dScanAngleStep,
									dAscanStart,
									dAscanRange,
									dTimeSlot,
									iCscanCount,
									adCscanStart,
									adCscanRange,
									adCscanThreshold,
									aeRectification,
									aeModeAmp,
									aeModeTof,
									eDepthMode
									))
		return false;
	if(AfxMessageBox(L"Do you want to complete ?",MB_YESNO)==IDNO)
		return false;
	CWaitCursor wait;
	if((!dSpecimenRadius && m_pWizardSystem->Specimen().SetSpecimen(ePlane)) ||
		((dSpecimenRadius>0.0) && m_pWizardSystem->Specimen().SetSpecimen(eDisk)))
	{
		AfxMessageBox(L"Error Specimen");
		return false;
	}
	if((dSpecimenRadius>0.0) && m_pWizardSystem->Specimen().SetRadius(dSpecimenRadius))
	{
		AfxMessageBox(L"Error SpecimenRadius!");
		return false;
	}
	if(m_pWizardSystem->Probe(0).SetRadius(dProbeRadius))
	{
		AfxMessageBox(L"Error ProbeRadius!");
		return false;
	}
	if(iScanElementStart<0)
	{//1.1.5.4i
		AfxMessageBox(L"Error ScanElementStart!");
		return false;
	}
	if(!OEMPA_WriteWizard(	m_pWizardSystem,m_pChannel,
						bLongitudinalWave,dSpecimenVelocity,
						iProbeElementCount,dProbePitch,dProbeFrequency,
						bWedgeEnable,
						dWedgeHeight,dWedgeAngle,dWedgeVelocity,
						dScanDepthEmission,adScanDepthReception,iScanDepthReception,
						iScanElementCount,
						bLinear,iScanElementStart,iScanElementStop,iScanElementStep,
						!bLinear,dScanAngleStart,dScanAngleStop,dScanAngleStep,
						dAscanStart,dAscanRange,dTimeSlot,
						iCscanCount,adCscanStart,adCscanRange,adCscanThreshold,
						aeRectification,aeModeAmp,aeModeTof,
						eDepthMode))
		AfxMessageBox(L"Error");
	return true;
}

void CDlgSetupFiles::UpdateDialog()
{
	if(CDlgSetupFiles::m_pDlgWizard)
		CDlgSetupFiles::m_pDlgWizard->UpdateDialog_();
}

bool CDlgSetupFiles::IsKernelObject(wchar_t *wcObjectName,enumRootType eInRootID,int &iKernelObjectId)
{
	enumRootType eRootID;
	CString string;

	if(!swscanf_s(wcObjectName,L"%d",&iKernelObjectId,sizeof(iKernelObjectId)))
	{
		eRootID = eInRootID;
		if(!CUTKernelRoot::FindRootName(wcObjectName,eRootID,iKernelObjectId))
		{
			string.Format(L"Object \"%s\" is not found in the kernel database!",wcObjectName);
			AfxMessageBox(string);
			return false;
		}
	}
	if(!CUTKernelRoot::FindRootId(iKernelObjectId,eRootID))
	{
		string.Format(L"Object %d is not found in the kernel database!",iKernelObjectId);
		AfxMessageBox(string);
		return false;
	}
	if(eRootID!=eInRootID)
	{
		string.Format(L"Object %d is a wrong object!",iKernelObjectId);
		AfxMessageBox(string);
		return false;
	}
	return true;
}

bool CDlgSetupFiles::OEMPAFileSamples(bool bWizard)
{
	CString strFileFullPathName=L"*.txt";
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"OEMPA");
	strFileFullPathName += L"*.txt";
	CFileDialog filedialog(FALSE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	CString string,strSelectionFile,strCorrectedFile,strAux;
	int iCycleCount;
	int iEmissionElementCount;
	double dEmissionWedgeDelay;
	int iReceptionElementCount,iReceptionFocalCount;
	double dReceptionWedgeDelay;
	double dAscanStart,dAscanRange,dGateStart,dGateStop,dGateThreshold;
	int iDACCount,iGateCount;
	_TCHAR drive[_MAX_DRIVE],dir[_MAX_DIR],fname[MAX_PATH],ext[_MAX_EXT];
	bool bTextFile;
	double dTimeSlot;
	double dPulseWidth;
	int iSameAllCycles;
	wchar_t wcChannel[g_iMaxNameLength+1];
	wchar_t wcSystem[g_iMaxNameLength+1];
	wchar_t pChannelName[g_iMaxNameLength+1];
	int iWizardChannelId;
	int iWizardSystemId;
	CHWDeviceOEMPA *pHWDeviceOEMPA;
	bool bDeleteAux,bMerge;

	pHWDeviceOEMPA = CDlgHW::GetKernelDriverOEMPA();
	if(!pHWDeviceOEMPA)
		return false;
	UpdateData();
	if(filedialog.DoModal()!=IDOK)
		return false;
	string = filedialog.GetFileExt();
	if((string!=L"txt") && (string!=L"bin"))
		return false;
	if(string==L"txt")
		bTextFile = true;
	else
		bTextFile = false;
	string = filedialog.GetPathName();
	_wsplitpath_s(string.GetBuffer(),drive,_MAX_DRIVE,dir,_MAX_DIR,fname,MAX_PATH,ext,_MAX_EXT);
	strSelectionFile = string;
	strCorrectedFile = string;
	if(!bWizard)
	{
		try{
			CStdioFile file(strSelectionFile,CFile::typeText | CFile::modeWrite | CFile::modeCreate);
			file.WriteString(L"# TEMPLATE - SINGLE-CHANNEL\n");
			file.WriteString(L"#Fill in following input parameters (default values are OK),save the file and\n");
			file.WriteString(L"#close notepad to go on!\n");
			file.WriteString(L"\n");
			file.WriteString(L"[OEMPACustomizeAPI\\Parameters]\n");
			file.WriteString(L"#\"CycleCount\" is the cycle count.\n");
			file.WriteString(L"CycleCount=1\n");
			file.WriteString(L"#\"TimeSlot=\" for all cycles.\n");
			file.WriteString(L"TimeSlot=2000 탎\n");
			file.WriteString(L"#\"SameAllCycles\" 1 to have same UT parameters for all cycles.\n");
			file.WriteString(L"SameAllCycles=1\n");
			file.WriteString(L"\n");
			file.WriteString(L"[OEMPACustomizeAPI\\EmissionParameters]\n");
			file.WriteString(L"#\"ElementCount\" is the element count for the emission aperture.\n");
			file.WriteString(L"ElementCount=16\n");
			file.WriteString(L"#\"WedgeDelay\" is the wedge delay (unit=microsecond) for the emission aperture.\n");
			file.WriteString(L"WedgeDelay=0.0 탎\n");
			file.WriteString(L"#\"PulseWidth=\" for all cycles.\n");
			file.WriteString(L"PulseWidth=0.1 탎\n");
			file.WriteString(L"\n");
			file.WriteString(L"[OEMPACustomizeAPI\\ReceptionParameters]\n");
			file.WriteString(L"#\"ElementCount\" is the element count for the reception aperture.\n");
			file.WriteString(L"ElementCount=16\n");
			file.WriteString(L"#\"FocalCount\" is the reception focal law count including the first focal law even in the case that the Dynamic Depth Focusing is not used.\n");
			file.WriteString(L"FocalCount=4\n");
			file.WriteString(L"#\"WedgeDelay\" is the wedge delay (unit=microsecond) for the reception aperture.\n");
			file.WriteString(L"WedgeDelay=0.0 탎\n");
			file.WriteString(L"#\"AscanStart\" is the ascan start (unit=microsecond) for all gates.\n");
			file.WriteString(L"AscanStart=1.0 탎\n");
			file.WriteString(L"#\"AscanRange\" is the ascan range (unit=microsecond) for all gates.\n");
			file.WriteString(L"AscanRange=5.0 탎\n");
			file.WriteString(L"#\"DACCount\" is the point count of the DAC.\n");
			file.WriteString(L"DACCount=0\n");
			file.WriteString(L"\n");
			file.WriteString(L"[OEMPACustomizeAPI\\GateParameters]\n");
			file.WriteString(L"#\"Count\" is the gate count.\n");
			file.WriteString(L"Count=0\n");
			file.WriteString(L"#\"Start\" is the gate start (unit=microsecond) for all gates.\n");
			file.WriteString(L"Start=2.0 탎\n");
			file.WriteString(L"#\"Stop\" is the gate stop (unit=microsecond) for all gates.\n");
			file.WriteString(L"Stop=4.0 탎\n");
			file.WriteString(L"#\"Threshold\" is the gate threshold (unit=percent) for all gates.\n");
			file.WriteString(L"Threshold=50.0 %\n");
			file.Close();
		}catch(CFileException *e){
			e->ReportError();
			e->Delete();
		}
		EditFile(strSelectionFile.GetBuffer(),true);
		CheckCarriageReturnNotepad(strSelectionFile,strSelectionFile);
		if(!FileReadInteger(L"OEMPACustomizeAPI\\Parameters",L"CycleCount",strSelectionFile.GetBuffer(),iCycleCount))
			return false;
		if(!FileReadDouble(L"OEMPACustomizeAPI\\Parameters",L"TimeSlot",strSelectionFile.GetBuffer(),1.0e-6,dTimeSlot))
			return false;
		if(!FileReadInteger(L"OEMPACustomizeAPI\\Parameters",L"SameAllCycles",strSelectionFile.GetBuffer(),iSameAllCycles))
			return false;
		if(!FileReadInteger(L"OEMPACustomizeAPI\\EmissionParameters",L"ElementCount",strSelectionFile.GetBuffer(),iEmissionElementCount))
			return false;
		if(!FileReadDouble(L"OEMPACustomizeAPI\\EmissionParameters",L"WedgeDelay",strSelectionFile.GetBuffer(),1.0e-6,dEmissionWedgeDelay))
			return false;
		if(!FileReadDouble(L"OEMPACustomizeAPI\\EmissionParameters",L"PulseWidth",strSelectionFile.GetBuffer(),1.0e-6,dPulseWidth))
			return false;
		if(!FileReadInteger(L"OEMPACustomizeAPI\\ReceptionParameters",L"ElementCount",strSelectionFile.GetBuffer(),iReceptionElementCount))
			return false;
		if(!FileReadInteger(L"OEMPACustomizeAPI\\ReceptionParameters",L"FocalCount",strSelectionFile.GetBuffer(),iReceptionFocalCount))
			return false;
		if(!FileReadDouble(L"OEMPACustomizeAPI\\ReceptionParameters",L"WedgeDelay",strSelectionFile.GetBuffer(),1.0e-6,dReceptionWedgeDelay))
			return false;
		if(!FileReadDouble(L"OEMPACustomizeAPI\\ReceptionParameters",L"AscanStart",strSelectionFile.GetBuffer(),1.0e-6,dAscanStart))
			return false;
		if(!FileReadDouble(L"OEMPACustomizeAPI\\ReceptionParameters",L"AscanRange",strSelectionFile.GetBuffer(),1.0e-6,dAscanRange))
			return false;
		if(!FileReadInteger(L"OEMPACustomizeAPI\\ReceptionParameters",L"DACCount",strSelectionFile.GetBuffer(),iDACCount))
			return false;
		if(!FileReadInteger(L"OEMPACustomizeAPI\\GateParameters",L"Count",strSelectionFile.GetBuffer(),iGateCount))
			return false;
		if(!FileReadDouble(L"OEMPACustomizeAPI\\GateParameters",L"Start",strSelectionFile.GetBuffer(),1.0e-6,dGateStart))
			return false;
		if(!FileReadDouble(L"OEMPACustomizeAPI\\GateParameters",L"Stop",strSelectionFile.GetBuffer(),1.0e-6,dGateStop))
			return false;
		if(!FileReadDouble(L"OEMPACustomizeAPI\\GateParameters",L"Threshold",strSelectionFile.GetBuffer(),1.0,dGateThreshold))
			return false;
		if(AfxMessageBox(L"Do you want to complete ?",MB_YESNO)==IDNO)
			return false;
	}else{
		try{
			CFile::Remove(strSelectionFile);
		}catch(CFileException *e){
			e->Delete();
		}
		swprintf(pChannelName,g_iMaxNameLength+1,L"OEMPA_channel_%d",m_iDeviceId);
		wcscpy_s(wcChannel,g_iMaxNameLength+1,pChannelName);
		wcscpy_s(wcSystem,g_iMaxNameLength+1,L"OEMPA_system");
	}
	CWaitCursor wait;
	if(!bWizard)
		OEMPA_ReadWizardWriteFile(pHWDeviceOEMPA,string.GetBuffer(),iCycleCount,dTimeSlot,iSameAllCycles,
			iEmissionElementCount,dEmissionWedgeDelay,(float)dPulseWidth,
			iReceptionElementCount,iReceptionFocalCount,dReceptionWedgeDelay,
			dAscanStart,dAscanRange,iDACCount,
			iGateCount,dGateStart,dGateStop,dGateThreshold);
	else{
		if(!IsKernelObject(wcChannel,eRootTypeChannels,iWizardChannelId))
			return false;
		if(!IsKernelObject(wcSystem,eRootTypeWizardSystem,iWizardSystemId))
			return false;
		OEMPA_ReadWizardWriteFile(pHWDeviceOEMPA,
			iWizardSystemId,iWizardChannelId,string.GetBuffer());
		if(WizardFileSamples(false,true))
		{
			strAux = g_wcFolderData;
			strAux += L"Wizard.txt";
			bMerge = true;
			bDeleteAux = false;
		}else{
			int aiListRootId[2]={iWizardSystemId,iWizardChannelId};
			_wsplitpath_s(string.GetBuffer(),drive,_MAX_DRIVE,dir,_MAX_DIR,fname,MAX_PATH,ext,_MAX_EXT);
			strAux = drive;
			strAux += dir;
			strAux += fname;
			strAux += L".tmp";
			bMerge = false;//myCUTMFCContainer_Save(this, strAux.GetBuffer(), 2, aiListRootId);
			bDeleteAux = true;
		}
		if(bMerge)
		try{
			CStdioFile fileIn(strAux,CFile::typeText | CFile::modeRead);
			CStdioFile fileOut(strSelectionFile,CFile::typeText | CFile::modeWrite);
			fileOut.SeekToEnd();
			fileOut.WriteString(L"\n");
			while(fileIn.ReadString(strAux))
			{
				fileOut.WriteString(L"#");
				fileOut.WriteString(strAux);
				fileOut.WriteString(L"\n");
			}
		}catch(CFileException *e){
			e->ReportError();
			e->Delete();
		}
		if(bDeleteAux)
			DeleteFile(strAux);
	}
	if(bTextFile)
		EditFile(string.GetBuffer(),false);
	UTKernel_SetFolderDataFiles(L"OEMPA",string.GetBuffer());
	return true;
}

void CDlgSetupFiles::OnBnClickedButtonWizardToFile()
{
	OEMPAFileSamples(true);
}

void CDlgSetupFiles::OnBnClickedButtonFileSample()
{
	OEMPAFileSamples(false);
}

void CDlgSetupFiles::OnBnClickedButtonTxtToBin()
{
	CString strFileFullPathName=L"*.txt";
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"OEMPA");
	strFileFullPathName += L"*.txt";
	CFileDialog filedialog(TRUE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||",this,0);
	CString string,strMsg;
	wchar_t pFileNew[MAX_PATH];
	bool bTextFile;
	CFileStatus rStatus;
	int iRet;
	CHWDeviceOEMPA *pHWDeviceOEMPA;

	pHWDeviceOEMPA = CDlgHW::GetKernelDriverOEMPA();
	if(!pHWDeviceOEMPA)
		return;
	if(filedialog.DoModal()!=IDOK)
		return;
	string = filedialog.GetFileExt();
	if(string!=L"txt")
		return;
	string = filedialog.GetPathName();
	if(!ChangeExtensionFile(string.GetBuffer(),MAX_PATH,pFileNew,bTextFile))
		return;
	if(CFile::GetStatus(pFileNew,rStatus))
	{
		strMsg.Format(L"Do you want to overwrite \"%s\" ?",pFileNew);
		iRet = AfxMessageBox(strMsg,MB_YESNOCANCEL);
		if(iRet!=IDYES)
			return;
		CFile::Remove(pFileNew);
	}
	CWaitCursor wait;
	OEMPA_ConvertFileFormat(pHWDeviceOEMPA,string.GetBuffer(),pFileNew);
	UTKernel_SetFolderDataFiles(L"OEMPA",string.GetBuffer());
}

void CDlgSetupFiles::OnBnClickedButtonBinToTxt()
{
	CString strFileFullPathName=L"*.bin";
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"OEMPA");
	strFileFullPathName += L"*.bin";
	CFileDialog filedialog(TRUE,L"*.bin",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	CString string,strMsg;
	wchar_t pFileNew[MAX_PATH];
	bool bTextFile;
	CFileStatus rStatus;
	int iRet;
	CHWDeviceOEMPA *pHWDeviceOEMPA;

	pHWDeviceOEMPA = CDlgHW::GetKernelDriverOEMPA();
	if(!pHWDeviceOEMPA)
		return;
	if(filedialog.DoModal()!=IDOK)
		return;
	string = filedialog.GetFileExt();
	if(string!=L"bin")
		return;
	string = filedialog.GetPathName();
	if(!ChangeExtensionFile(string.GetBuffer(),MAX_PATH,pFileNew,bTextFile))
		return;
	if(CFile::GetStatus(pFileNew,rStatus))
	{
		strMsg.Format(L"Do you want to overwrite \"%s\" ?",pFileNew);
		iRet = AfxMessageBox(strMsg,MB_YESNOCANCEL);
		if(iRet!=IDYES)
			return;
		CFile::Remove(pFileNew);
	}
	CWaitCursor wait;
	OEMPA_ConvertFileFormat(pHWDeviceOEMPA,string.GetBuffer(),pFileNew);
	UTKernel_SetFolderDataFiles(L"OEMPA",string.GetBuffer());
}

void CDlgSetupFiles::OnMove(int x, int y)
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;
	wchar_t pTitle[MAX_PATH];
	CDialog::OnMove(x, y);

	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),MAX_PATH,pTitle,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,pTitle,iResHorz,iResVert,wndPlacement,0);
}

void CDlgSetupFiles::OnBnClickedButtonFileOption()
{
	AfxMessageBox(L"Please run the \"Manager\" to display OEMPA file options!");
}
