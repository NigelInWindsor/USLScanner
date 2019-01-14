// DlgAscan.cpp : implementation file
//

#include "stdafx.h"
#include "DlgHW.h"
#include "DlgSetupFiles.h"
#include "DlgHWLink.h"
#include "DisplayImage.h"
#include "ChildView.h"
#include "UTKernelLayoutFile.h"
#include "TimerProcess.h"
#include "DlgCscanDisplay.h"
#include "DlgCalibration.h"

#define WPARAM_UPDATE_COMBO_CHANNEL	111
#define WPARAM_CSCAN_WARNING		112
#define WPARAM_RUN_TOOLBOX			123
#define LPARAM_RUN_TOOLBOX_OK		1234
#define LPARAM_RUN_TOOLBOX_ERROR	12345

//void NotifyWizardDlg(BOOL bWizardEnable);

extern CChildView *g_pView;
extern wchar_t g_wcFolderData[];
extern bool g_bColorDefault;
extern BYTE g_byColorRed[256];
extern BYTE g_byColorGreen[256];
extern BYTE g_byColorBlue[256];
extern BYTE g_byColorRedU[256];
extern BYTE g_byColorGreenU[256];
extern BYTE g_byColorBlueU[256];

extern CRunToolbox g_RunToolbox;

structCorrectionOEMPA g_Correction;//useful for corrected view.

bool CDlgHW::m_bAcquisitionWholeStream=false;//VERY SPECIAL PURPOSE in order to receive the whole FW stream instead of ascan, cscan and IO.
bool CDlgHW::m_bFifoAscanEnable=false;//can be changed to true to enable ascan fifo.
bool CDlgHW::m_bFifoCscanEnable=false;//can be changed to true to enable cscan fifo.
bool CDlgHW::m_bFifoIOEnable=false;//can be changed to true to enable cscan fifo.
bool CDlgHW::m_bFifoAscanDriverThread=true;//can be changed to false.
bool CDlgHW::m_bFifoCscanDriverThread=true;//can be changed to false.
bool CDlgHW::m_bFifoIODriverThread=true;//can be changed to false.
UINT g_uiUTEventMessage=0;
CDlgHW *CDlgHW::m_pDlgHW=NULL;
bool g_bInitGain=false;
double CDlgHW::m_adGain[g_iOEMPACycleCountMax];
double CDlgHW::m_dEditGain=0;
int CDlgHW::m_iComboCycle=0;
int CDlgHW::m_iCycleCount=0;
BOOL CDlgHW::m_bCheckShotEnable=TRUE;
BOOL CDlgHW::m_bCheckDisplayMax=FALSE;
#ifdef _EVALUATION_
BYTE CDlgHW::m_cIP[4]={127,0,0,1};
#else _EVALUATION_
BYTE CDlgHW::m_cIP[4]={192,168,1,11};
#endif _EVALUATION_
DWORD CDlgHW::m_dwIPAddress=0;
DWORD CDlgHW::m_dwEditPort=5001;
bool CDlgHW::m_bInitUsb=false;
int CDlgHW::m_iRadioEthernet=0;
BOOL CDlgHW::m_bCheckDisplayImage=FALSE;
BOOL CDlgHW::m_bCheckDisplayInvert=FALSE;
BOOL CDlgHW::m_bEnableCorrection=FALSE;
BOOL CDlgHW::m_bCheckWizardEnable=FALSE;
BOOL CDlgHW::m_bCheckFileFormatAddressData=FALSE;
int CDlgHW::m_iIndexLayoutFile=-1;
bool CDlgHW::m_bUnlockEnableShot=true;
bool CDlgHW::g_bFirstShotEnable=false;
CString CDlgHW::m_strEncoder;
DWORD CDlgHW::m_dwTimerID=0;
int CDlgHW::m_iComboChannel=0;
CRITICAL_SECTION CDlgHW::m_ChannelCriticalSection;
int CDlgHW::m_iChannelProbe=-1,CDlgHW::m_iChannelScan=-1;
int CDlgHW::m_iChannelCount=0;
int CDlgHW::m_iChannelProbeCount=0;
int CDlgHW::m_iChannelScanCount[g_iOEMMCProbeCountMax]={0,0,0,0};
int CDlgHW::m_iChannelCycleCount[g_iOEMMCProbeCountMax][g_iOEMMCScanCountMax]=
										{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
CRITICAL_SECTION CDlgHW::m_CriticSectionCounters;
CString CDlgHW::m_strEditLostCount=L"";
ULONGLONG CDlgHW::m_ullDataSizeAscan=0,CDlgHW::m_ullDisplayDataSizeAscan=0;
ULONGLONG CDlgHW::m_ullDataSizeCscan=0,CDlgHW::m_ullDisplayDataSizeCscan=0;
ULONGLONG CDlgHW::m_ullAscanCount=0;
double CDlgHW::m_dEncoderAscan[g_iDriverEncoderCountMax]={0.0,0.0};
double CDlgHW::m_dEncoderCscan[g_iDriverEncoderCountMax]={0.0,0.0};
DWORD CDlgHW::m_dwEncoderAscanTickCount=0;
DWORD CDlgHW::m_dwEncoderCscanTickCount=0;
LARGE_INTEGER CDlgHW::m_llEncoderAscanTickCount={0,0};
LARGE_INTEGER CDlgHW::m_llEncoderCscanTickCount={0,0};
DWORD CDlgHW::m_dwDisplayTickCount=0;
DWORD CDlgHW::m_dwDigitalInputsAscan=0;
DWORD CDlgHW::m_dwDigitalInputsCscan=0;
bool CDlgHW::m_bFullMatrixCapture=false;
bool CDlgHW::m_bMultiHWChannel=false;
BYTE CDlgHW::m_abyMultiChannelDecimation[g_iOEMPACycleCountMax];
float CDlgHW::m_afMultiChannelStart[g_iOEMPACycleCountMax];
float CDlgHW::m_fMultiChannelPeriod=10.0e-9f;
int CDlgHW::m_iFMCElementStart=0;
int CDlgHW::m_iFMCElementStop=0;
int CDlgHW::m_iFMCElementStep=1;
int CDlgHW::m_iFMCElementCount=1;
BOOL CDlgHW::m_bCheckFMCCalibration=FALSE;
bool g_bAdvanced=false;
int CDlgHW::m_iIOMaxTemperature=0;

CHWDeviceOEMPA *CDlgHW::m_pHWDeviceOEMPA=NULL;
HANDLE CDlgHW::m_hThread=NULL;
DWORD CDlgHW::m_dwThreadId=0;

bool FileReadInteger(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int &iData,bool bDisplayMsg=true,int iMin=INT_MIN,int iMax=INT_MAX);
bool FileReadFloat(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,float fUnitFactor,float &fData,bool bDisplayMsg=true);
bool WINAPI OEMPA_WriteHWWizard(	CHWDeviceOEMPA *pHWDeviceOEMPA,
					int iWizardSystemId,int iWizardChannelId);
void APISamples(CHWDeviceOEMPA *pHWDeviceOEMPA);

void EditFile(wchar_t *pFile,bool bCloseWaiting)
{
	SHELLEXECUTEINFO execinfo;

	memset(&execinfo, 0, sizeof(execinfo));
	execinfo.lpFile = pFile;
	execinfo.cbSize = sizeof(execinfo);
	execinfo.lpVerb = L"open";
	execinfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	execinfo.nShow = SW_SHOWDEFAULT;
	execinfo.lpParameters = 0;
	ShellExecuteEx(&execinfo);
	if(bCloseWaiting)
		WaitForSingleObject(execinfo.hProcess, INFINITE);
}

void CheckCarriageReturnNotepad(const wchar_t *strFileNameIn,const wchar_t *strFileNameOut)
{
	//Sometimes in ASCII saved text file it is possible to find too much 0x0D character.
	//it is better to remove those characters, see the following example:
	//0x03FE4ED5  6e 74 3d 31 0d 0d 36 0d 0d 0a 0d 0d 0d 0a 5b 53  nt=1..6.......[S
	//0x03FE4EE5  63 61 6e 5c 4c 69 6e 65 61 72 5d 0d 0d 0d 0d 0a  can\Linear].....
	//0x03FE4EF5  23 22 43 6f 75 6e 74 22 20 69 73 20 74 68 65 20  #"Count" is the 
	//0x03FE4F05  61 70 65 72 74 75 72 65 20 65 6c 65 6d 65 6e 74  aperture element
	//0x03FE4F15  20 63 6f 75 6e 74 2e 0d 0d 0d 0d 0a 43 6f 75 6e   count......Coun
	//0x03FE4F25  74 3d 31 0d 0d 36 0d 0d 0a 23 22 44 65 70 74 68  t=1..6...#"Depth
	//0x03FE4F35  45 6d 69 73 73 69 6f 6e 22 20 69 73 20 74 68 65  Emission" is the
	//0x03FE4F45  20 65 6d 69 73 73 69 6f 6e 20 64 65 70 74 68 20   emission depth 
	//0x03FE4F55  6f 66 20 61 6c 6c 20 61 70 65 72 74 75 72 65 73  of all apertures
	int iLength=0,iCount=0,iIndex,iIndex2,iSize;
	unsigned int uiLength=0;
	char *pData;

	try{
		CFile fileIn(strFileNameIn,CFile::typeBinary | CFile::modeRead);
		iLength = (int)fileIn.GetLength();
		iSize = iLength+1;
		pData = new char[iSize];
		if(pData)
		{
			memset(pData,0,iLength+1);
			uiLength = fileIn.Read(pData,iLength);
		}
		fileIn.Close();
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
	}
	if((!iLength || !uiLength) || (iLength!=uiLength))
	{
		if(pData)
			delete pData;
		return;
	}
	//modifications
	iLength = (int)strlen(pData);
	iIndex = 0;
	while(iIndex<iLength)
	{
		if(pData[iIndex]==0x0D)
		{
			iIndex2 = iIndex+1;
			while((iIndex2<iLength) && (pData[iIndex2]==0x0D))
				iIndex2++;
			if(iIndex+1<iIndex2)
			if(iIndex2<iLength)
			{
				//remove 0x0D
				if(pData[iIndex2]==0x0A)
					//keep only one (0x0D 0x0A)
					iIndex++;
				strcpy_s(&pData[iIndex],iSize,&pData[iIndex2]);
				iLength -= iIndex2-iIndex;
			}else{
				pData[iIndex] = 0;
				break;
			}
		}
		iIndex++;
	}
	uiLength = (int)strlen(pData);
	if(iLength!=uiLength)
	{
		if(pData)
			delete pData;
		return;//no modification
	}
	try{
		CFile fileOut(strFileNameOut,CFile::typeBinary | CFile::modeCreate | CFile::modeWrite);
		fileOut.Write(pData,iLength);
		fileOut.Close();
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
	}
	if(pData)
		delete pData;
}

int GetBitsize(__int64 iCount)
{
	double dDataByteSize;
	int iDataBitSize;

	dDataByteSize = (double)iCount;
	if(iCount==2)
	{//bitwise data
		dDataByteSize = 1;
		iDataBitSize = 1;
	}else{
		dDataByteSize = log(dDataByteSize)/log(2.0)*1.0000000000001;
		iDataBitSize = (int)dDataByteSize;
	}
	return iDataBitSize;
}

// CDlgHW dialog

IMPLEMENT_DYNAMIC(CDlgHW, CDialog)

CDlgHW::CDlgHW(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgHW::IDD, pParent)
    , m_bCheckConnect(FALSE)
    , m_dwEditError(0)
    , m_strComboUsb(_T(""))
    , m_bCheckTest(FALSE)
{
	CHWDeviceOEMPA::RegisterMultiProcess(L"OEMPATool");
	if(!g_bInitGain)
	{
		AfxInitRichEdit2();
		//CheckCarriageReturnNotepad(L"C:\\Users\\Public\\SW\\UTKernel\\doc\\Wizard3.bin",L"C:\\Users\\Public\\SW\\UTKernel\\doc\\Wizard3.bin");
		InitializeCriticalSection(&m_ChannelCriticalSection);
		InitializeCriticalSection(&m_CriticSectionCounters);
		ResetCounters();
		memset(&g_Correction,0,sizeof(g_Correction));
		OEMPA_New(g_Correction);
		g_bInitGain = true;
		for(int iCycle=0;iCycle<g_iOEMPACycleCountMax;iCycle++)
		{
			m_adGain[iCycle] = 0.0;
		}
		memset(m_abyMultiChannelDecimation,0,sizeof(m_abyMultiChannelDecimation));
		memset(m_afMultiChannelStart,0,sizeof(m_afMultiChannelStart));
	}
	m_bkColor = RGB(0xff,0xff,0xff);
	m_bkColorInit = false;
	m_bInitDialog = false;
	m_bGainKillfocusEnable = true;
	CDlgHW::m_dwIPAddress = (m_cIP[0]<<24)+(m_cIP[1]<<16)+(m_cIP[2]<<8)+(m_cIP[3]<<0);
	g_uiUTEventMessage = UTKernel_RegisterWindowMessage();
	Create(IDD_DIALOG_HW,pParent);
	ShowWindow(SW_SHOW);
	m_pEditBkBrush = new CBrush(RGB(255, 0, 0));
	SHORT nState;
	nState = GetKeyState(VK_CONTROL);
	if(nState&0x8000)
		g_bAdvanced = true;
}

CDlgHW::~CDlgHW()
{
	m_iIndexLayoutFile = -1;
	OEMPA_Free(g_Correction);
	memset(&g_Correction,0,sizeof(g_Correction));
	CHWDeviceOEMPA::UnregisterMultiProcess();
}

void CDlgHW::PostNcDestroy()
{
	if(m_pDlgHW==this)
		m_pDlgHW = NULL;
	if(m_pEditBkBrush)
		delete m_pEditBkBrush;
	m_pEditBkBrush = NULL;
	delete this;
}

void CDlgHW::Display()
{
	if(!m_pDlgHW)
		m_pDlgHW = new CDlgHW;
	if(!m_pDlgHW)
		return;
	if(m_pDlgHW->IsIconic())
		m_pDlgHW->ShowWindow(SW_RESTORE);
	m_pDlgHW->ShowWindow(SW_SHOW);
}

int CDlgHW::GetDeviceId()
{
	if(!m_pHWDeviceOEMPA)
		return -1;
	return m_pHWDeviceOEMPA->GetDeviceId();
}

bool CDlgHW::SetTimer(bool bEnable)
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

CHWDeviceOEMPA *CDlgHW::GetKernelDriverOEMPA()
{
	return m_pHWDeviceOEMPA;
}

void CDlgHW::Destroy()
{
	if(m_pHWDeviceOEMPA)
		delete m_pHWDeviceOEMPA;
	m_pHWDeviceOEMPA = NULL;
	if(m_pDlgHW)
		m_pDlgHW->DestroyWindow();
}

void CDlgHW::ResetCounters()
{
	EnterCriticalSection(&m_CriticSectionCounters);
	m_ullDataSizeAscan = 0;
	m_ullDataSizeCscan = 0;
	m_ullAscanCount = 0;
	m_ullDisplayDataSizeAscan = -1;
	m_ullDisplayDataSizeCscan = -1;
	m_llEncoderAscanTickCount.QuadPart = 0;
	m_llEncoderCscanTickCount.QuadPart = 0;
	m_dwEncoderAscanTickCount = 0;
	m_dwEncoderCscanTickCount = 0;
	m_dwDisplayTickCount = -1;
	for(int i=0;i<g_iDriverEncoderCountMax;i++)
	{
		m_dEncoderAscan[i] = 0.0;
		m_dEncoderCscan[i] = 0.0;
	}
	m_dwDigitalInputsAscan = 0;
	m_dwDigitalInputsCscan = 0;
	LeaveCriticalSection(&m_CriticSectionCounters);
}

void CDlgHW::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GAIN, CDlgHW::m_dEditGain);
	DDX_Check(pDX, IDC_CHECK_SHOT_ENABLE, CDlgHW::m_bCheckShotEnable);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPAddress);
	DDX_Text(pDX, IDC_EDIT_PORT, m_dwEditPort);
	DDV_MinMaxUInt(pDX, m_dwEditPort, 0, 65535);
	DDX_Control(pDX, IDC_EDIT_PORT, m_EditPort);
	DDX_Check(pDX, IDC_CHECK_CONNECT, m_bCheckConnect);
	DDX_Text(pDX, IDC_EDIT_ERROR, m_dwEditError);
	DDX_Control(pDX, IDC_COMBO_USB, m_ComboUsb);
	DDX_CBString(pDX, IDC_COMBO_USB, m_strComboUsb);
	DDX_Radio(pDX, IDC_RADIO_ETHERNET, m_iRadioEthernet);
	DDX_Control(pDX, IDC_RADIO_ETHERNET, m_RadioEthernet);
	DDX_Control(pDX, IDC_RADIO_USB, m_RadioUsb);
	DDX_Control(pDX, IDC_SPIN_GAIN, m_SpinGain);
	DDX_Control(pDX, IDC_EDIT_GAIN, m_EditGain);
	DDX_Control(pDX, IDC_COMBO_DISPLAY_CYCLE, m_ComboCycle);
	DDX_CBIndex(pDX, IDC_COMBO_DISPLAY_CYCLE, m_iComboCycle);
	DDX_Control(pDX, IDC_CHECK_SHOT_ENABLE, m_CheckShotEnable);
	DDX_Control(pDX, IDC_BUTTON_WRITE, m_ButtonAPISamples);
	DDX_Control(pDX, IDC_BUTTON_FILE_OEMPA_LOAD, m_ButtonFileToHW);
	DDX_Control(pDX, IDC_BUTTON_FILE_OEMPA_SAVE, m_ButtonHWToFile);
	DDX_Control(pDX, IDC_CHECK_DISPLAY_IMAGE, m_CheckDisplayImage);
	DDX_Check(pDX, IDC_CHECK_DISPLAY_IMAGE, m_bCheckDisplayImage);
	DDX_Control(pDX, IDC_CHECK_DISPLAY_INVERT, m_CheckDisplayInvert);
	DDX_Check(pDX, IDC_CHECK_DISPLAY_INVERT, m_bCheckDisplayInvert);
	DDX_Control(pDX, IDC_BUTTON_WIZARD_TO_HW, m_ButtonWizardToHW);
	DDX_Control(pDX, IDC_BUTTON_DISPLAY_COLOR, m_ButtonColor);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_ButtonReset);
	DDX_Control(pDX, IDC_BUTTON_STATUS, m_ButtonStatus);
	DDX_Control(pDX, IDC_RICHEDIT_ENCODER, m_EditEncoder);
	DDX_Control(pDX, IDC_CHECK_DISPLAY_MAX, m_CheckDisplayMax);
	DDX_Check(pDX, IDC_CHECK_DISPLAY_MAX, m_bCheckDisplayMax);
	DDX_Control(pDX, IDC_COMBO_CHANNEL, m_ComboChannel);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL, m_iComboChannel);
	DDX_Control(pDX, IDC_STATIC_CHANNEL, m_StaticChannel);
	DDX_Text(pDX, IDC_EDIT_DATA_LOST, m_strEditLostCount);
	DDX_Control(pDX, IDC_EDIT_DATA_LOST, m_EditDataLost);
	DDX_Control(pDX, IDC_BUTTON_OUTPUT, m_EditButtonOutput);
	DDX_Control(pDX, IDC_BUTTON_CONFIG, m_ButtonToolbox);
	DDX_Control(pDX, IDC_BUTTON_CALIBRATION, m_EditCalibration);
	DDX_Control(pDX, IDC_STATIC_GAIN_UNIT, m_StaticGainUnit);
	DDX_Control(pDX, IDC_STATIC_CYCLE, m_StaticCycle);
	DDX_Control(pDX, IDC_CHECK_FMC_CALIBRATION, m_CheckFMCCalibration);
	DDX_Check(pDX, IDC_CHECK_FMC_CALIBRATION, m_bCheckFMCCalibration);
	DDX_Control(pDX, IDC_EDIT_ERROR, m_EditError);
	DDX_Control(pDX, IDC_CHECK_TEST, m_CheckTest);
	DDX_Check(pDX, IDC_CHECK_TEST, m_bCheckTest);
	DDX_Control(pDX, IDC_CHECK_FILE_ADD_DATA_FORMAT, m_CheckFileFormatAddressData);
	DDX_Check(pDX, IDC_CHECK_FILE_ADD_DATA_FORMAT, m_bCheckFileFormatAddressData);
	DDX_Control(pDX, IDC_BUTTON_RESET_USB3, m_ButtonResetUSB3);
}

BEGIN_MESSAGE_MAP(CDlgHW, CDialog)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_MOVE()
	ON_WM_CTLCOLOR()
	ON_REGISTERED_MESSAGE(g_uiUTEventMessage, &CDlgHW::OnEventMessage)
	ON_BN_CLICKED(IDOK, &CDlgHW::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgHW::OnBnClickedCancel)
	ON_EN_KILLFOCUS(IDC_EDIT_GAIN, &CDlgHW::OnEnKillfocusEditGain)
	ON_BN_CLICKED(IDC_CHECK_SHOT_ENABLE, &CDlgHW::OnBnClickedCheckPulserEnable)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, &CDlgHW::OnBnClickedButtonWrite)
	ON_BN_CLICKED(IDC_CHECK_CONNECT, &CDlgHW::OnBnClickedCheckConnect)
	ON_BN_CLICKED(IDC_RADIO_ETHERNET, &CDlgHW::OnBnClickedRadioEthernet)
	ON_BN_CLICKED(IDC_RADIO_USB, &CDlgHW::OnBnClickedRadioUsb)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GAIN, &CDlgHW::OnDeltaposSpinGain)
	ON_BN_CLICKED(IDC_BUTTON_FILE_OEMPA_LOAD, &CDlgHW::OnBnClickedButtonFileOempaLoad)
	ON_BN_CLICKED(IDC_BUTTON_FILE_OEMPA_SAVE, &CDlgHW::OnBnClickedButtonFileOempaSave)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY_CYCLE, &CDlgHW::OnCbnSelchangeComboDisplayCycle)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY_IMAGE, &CDlgHW::OnBnClickedCheckDisplayImage)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY_INVERT, &CDlgHW::OnBnClickedCheckDisplayInvert)
	ON_BN_CLICKED(IDC_BUTTON_WIZARD_TO_HW, &CDlgHW::OnBnClickedButtonWizardToHw)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY_COLOR, &CDlgHW::OnBnClickedButtonDisplayColor)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgHW::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_STATUS, &CDlgHW::OnBnClickedButtonStatus)
	ON_EN_KILLFOCUS(IDC_IPADDRESS, &CDlgHW::OnEnKillfocusIPAddress)
	ON_EN_KILLFOCUS(IDC_EDIT_PORT, &CDlgHW::OnEnKillfocusEditPort)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CDlgHW::OnBnClickedButtonConfig)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY_MAX, &CDlgHW::OnBnClickedCheckDisplayMax)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL, &CDlgHW::OnCbnSelchangeComboChannel)
	ON_BN_CLICKED(IDC_BUTTON_MULTI, &CDlgHW::OnBnClickedButtonMulti)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, &CDlgHW::OnBnClickedButtonOutput)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATION, &CDlgHW::OnBnClickedButtonCalibration)
	ON_BN_CLICKED(IDC_CHECK_TEST, &CDlgHW::OnBnClickedCheckTest)
	ON_BN_CLICKED(IDC_BUTTON_FILE_OEMPA_LOAD2, &CDlgHW::OnBnClickedButtonFileOempaLoad2)
	ON_BN_CLICKED(IDC_CHECK_FILE_ADD_DATA_FORMAT, &CDlgHW::OnBnClickedCheckFileAddDataFormat)
	ON_BN_CLICKED(IDC_BUTTON_RESET_USB3, &CDlgHW::OnBnClickedButtonResetUsb3)
END_MESSAGE_MAP()


// CDlgHW message handlers

BOOL CDlgHW::OnInitDialog()
{
	CString string;
	int iResHorz,iResVert,iData;
	WINDOWPLACEMENT wndPlacement;
	wchar_t pTitle[MAX_PATH];
	bool bRegistered,bCreateNew;
	SYSTEMTIME sysTime;
	int iCount=0;

	CDialog::OnInitDialog();

	UpdateData();
	if(g_pView)
		g_pView->SetTimer(m_bCheckShotEnable);
	if(DriverRegistration())
	{
		UpdateDialog_();
		UpdateDriver();
		UpdateCycle();
		UpdateGain();
		m_SpinGain.SetRange32(0,800);
		m_SpinGain.SetBuddy(&m_EditGain);
		UpdateData(FALSE);
	}else
		PostMessage(WM_CLOSE);//DestroyWindow();

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
	m_strLayoutTitle = pTitle;
	m_bInitDialog = true;
	SetTimer(true);
	string.Format(L"Hardware (PID %d)",GetCurrentProcessId());
	SetWindowText(string);
	while(!m_bkColorInit && (iCount<10))
	{
		Sleep(10);
		iCount++;
	}
	//if(m_bkColorInit)
	//	m_EditEncoder.SetBackgroundColor(FALSE,m_bkColor);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

extern bool g_bExit;

DWORD WINAPI CDlgHW::ThreadFifo(LPVOID pParam)
{
#ifdef _WIN64
	structAcqInfoEx acqInfo,*pAcqInfo;
	CStream_0x0001 *pStreamHeader;
	CSubStreamAscan_0x0103 *pAscanHeader;
	const CSubStreamIO_0x0101 *pIOHeader;
	const CSubStreamCscan_0x0X02* pCscanHeader;
	const structCscanAmp_0x0102* pBufferAmp;
	const structCscanAmpTof_0x0202* pBufferAmpTof;
	void *pBufferMax=NULL;
	void *pBufferMin=NULL;
	void *pBufferSat=NULL;
	int iFifoCount;
	CAcquisitionFifo *pFifoAscan;
	CAcquisitionFifo *pFifoCscan;
	CAcquisitionFifo *pFifoIO;

	if(!m_pDlgHW->m_pHWDeviceOEMPA)
		return 0;
	while(!g_bExit)
	{
		if(!m_pDlgHW->m_pHWDeviceOEMPA)
			break;
		pFifoAscan = m_pDlgHW->m_pHWDeviceOEMPA->GetAcquisitionFifo(eFifoAscan);
		pFifoCscan = m_pDlgHW->m_pHWDeviceOEMPA->GetAcquisitionFifo(eFifoCscan);
		pFifoIO = m_pDlgHW->m_pHWDeviceOEMPA->GetAcquisitionFifo(eFifoIO);
		iFifoCount = pFifoAscan->GetCount();
		if(!m_pDlgHW->m_bFifoAscanDriverThread && iFifoCount)
		{
			while(pFifoAscan->OutAscan(-1,true,pAcqInfo,pStreamHeader,pAscanHeader,pBufferMax,pBufferMin,pBufferSat))
			{
				if(pAcqInfo)
					memcpy(&acqInfo,pAcqInfo,sizeof(acqInfo));
				if(pStreamHeader && pAscanHeader)
					ProcessAcquisitionAscan_0x00010103(g_pView,acqInfo,pStreamHeader,pAscanHeader,pBufferMax,pBufferMin,pBufferSat);
				pFifoAscan->RemoveItem(-1);
			}
		}
		iFifoCount = pFifoCscan->GetCount();
		if(!m_pDlgHW->m_bFifoCscanDriverThread && iFifoCount)
		{
			while(pFifoCscan->OutCscan(-1,true,pAcqInfo,pStreamHeader,pCscanHeader,pBufferAmp,pBufferAmpTof))
			{
				if(pAcqInfo)
					memcpy(&acqInfo,pAcqInfo,sizeof(acqInfo));
				if(pStreamHeader && pCscanHeader)
					ProcessAcquisitionCscan_0x00010X02(g_pView,acqInfo,pStreamHeader,pCscanHeader,pBufferAmp,pBufferAmpTof);
				pFifoCscan->RemoveItem(-1);
			}
		}
		iFifoCount = pFifoIO->GetCount();
		if(m_pDlgHW->m_bFifoIODriverThread && iFifoCount)
		{
			while(pFifoIO->OutIO(-1,true,pAcqInfo,pStreamHeader,pIOHeader))
			{
				if(pAcqInfo)
					memcpy(&acqInfo,pAcqInfo,sizeof(acqInfo));
				if(pStreamHeader && pIOHeader)
					ProcessAcquisitionIO_1x00010101(g_pView,acqInfo,pStreamHeader,pIOHeader);
				pFifoIO->RemoveItem(-1);
			}
		}
		Sleep(10);
	}
#endif _WIN64
	return 0;
}

DllImport void ResetAcqInfoEx(structAcqInfoEx &a);
typedef struct strucDebugUSB3StreamEx{
	unsigned Size2:8;
	unsigned Size1:8;
	unsigned Size0:8;
	unsigned Marge0:8;
	unsigned Size5:8;
	unsigned Size4:8;
	unsigned Size3:8;
	unsigned Marge1:8;
	unsigned StrmSent_vector:6;//quels buffers ont été envoyés et sont prêts à être écrits à nouveau.
	unsigned Marge20:2;
	unsigned StrmReady_vector:6;//quels buffers sont prêts à être envoyés sur l'USB.
	unsigned Marge21:2;
	unsigned Buff_number:3;//numéro du buffer où était stocké la stream actuelle.
	unsigned Marge22:5;
	unsigned StrmDitch_vector:6;//chaque bit indique si un buffer doit être ignoré, dans le cas où une erreur a été détectée, ou alors si un Flush a eu lieu (le MSB correspond au buffer 0, le LSB correspond au buffer 5).
	unsigned Marge23:2;
	unsigned USBLost_counter:32;
	unsigned Size_next_stream:32;
}strucDebugUSB3StreamEx;
unsigned int g_uiDebugUSB3SizeAllBufferMax=0;
unsigned int g_uiDebugUSB3SizeAllBufferCurrent=0;
unsigned int g_uiDebugUSB3SizeBufferCurrent=0;
CUTUsb3Device *g_pUsb3Device=NULL;
void GetDebugUSB3Size(strucDebugUSB3StreamEx *pDebugUSB3,unsigned int uiSizeBufferCurrent,unsigned int &uiSizeAllBuffer)
{
	//"StrmReady_vector" a 1 mais "StrmSent_vector" a 0 (avec ambiguite sur le "Buff_number").
	uiSizeAllBuffer = 0;
	if((pDebugUSB3->Buff_number==0) || ((pDebugUSB3->StrmReady_vector & 1) & !(pDebugUSB3->StrmSent_vector & 1)))
	{
		uiSizeAllBuffer += pDebugUSB3->Size0;
		if(!pDebugUSB3->Size0 && (pDebugUSB3->Buff_number==0))
			uiSizeAllBuffer += uiSizeBufferCurrent/65536;
	}
	if((pDebugUSB3->Buff_number==1) || ((pDebugUSB3->StrmReady_vector & 2) & !(pDebugUSB3->StrmSent_vector & 2)))
	{
		uiSizeAllBuffer += pDebugUSB3->Size1;
		if(!pDebugUSB3->Size1 && (pDebugUSB3->Buff_number==1))
			uiSizeAllBuffer += uiSizeBufferCurrent/65536;
	}
	if((pDebugUSB3->Buff_number==2) || ((pDebugUSB3->StrmReady_vector & 4) & !(pDebugUSB3->StrmSent_vector & 4)))
	{
		uiSizeAllBuffer += pDebugUSB3->Size2;
		if(!pDebugUSB3->Size2 && (pDebugUSB3->Buff_number==2))
			uiSizeAllBuffer += uiSizeBufferCurrent/65536;
	}
	if((pDebugUSB3->Buff_number==3) || ((pDebugUSB3->StrmReady_vector & 8) & !(pDebugUSB3->StrmSent_vector & 8)))
	{
		uiSizeAllBuffer += pDebugUSB3->Size3;
		if(!pDebugUSB3->Size3 && (pDebugUSB3->Buff_number==3))
			uiSizeAllBuffer += uiSizeBufferCurrent/65536;
	}
	if((pDebugUSB3->Buff_number==4) || ((pDebugUSB3->StrmReady_vector & 16) & !(pDebugUSB3->StrmSent_vector & 16)))
	{
		uiSizeAllBuffer += pDebugUSB3->Size4;
		if(!pDebugUSB3->Size4 && (pDebugUSB3->Buff_number==4))
			uiSizeAllBuffer += uiSizeBufferCurrent/65536;
	}
	if((pDebugUSB3->Buff_number==5) || ((pDebugUSB3->StrmReady_vector & 32) & !(pDebugUSB3->StrmSent_vector & 32)))
	{
		uiSizeAllBuffer += pDebugUSB3->Size5;
		if(!pDebugUSB3->Size5 && (pDebugUSB3->Buff_number==5))
			uiSizeAllBuffer += uiSizeBufferCurrent/65536;
	}
	uiSizeAllBuffer *= 64;//KiloByte
}
UINT WINAPI CDlgHW::sProcessAcquisitionStream(void *pAcquisitionParameter,void *pData,int iSize)
{
	if(g_pView && m_pDlgHW && m_pHWDeviceOEMPA)
		return m_pDlgHW->ProcessAcquisitionStream(pData,iSize);
	return 1;
}
UINT WINAPI CDlgHW::ProcessAcquisitionStream(void *data,int iSize)
{
	static int iStreamSizeEx=0,size;
	CStream_0x0001 *pStreamHeader;
	CSubStreamAscan_0x0103 *pAscanHeader;
	structAcqInfoEx auxInfo;
	BYTE *pbyData=(BYTE*)data,*pData;
	DWORD *pdwData;
	const void *pBufferMax;
	unsigned int uiSizeAllBuffer;

#ifdef _WIN64
	InterlockedAdd64((LONGLONG volatile *)&m_ullDataSizeAscan,(LONGLONG)iSize);
#endif _WIN64
	if(!iStreamSizeEx)
	{
		g_pUsb3Device = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetUSB3Device();
		if(g_pUsb3Device)
			iStreamSizeEx = g_pUsb3Device->GetStreamSizeEx()/sizeof(DWORD);
	}else{
		//Look for debug data.
		size = iSize;
		pData = &pbyData[size];
		pdwData = (DWORD*)pData;
		if((pdwData[-1]==0x56789ABC) && (pdwData[iStreamSizeEx-1]==0xC0DEABBA))
		{
			GetDebugUSB3Size((strucDebugUSB3StreamEx*)pdwData,(unsigned int)iSize,uiSizeAllBuffer);
			InterlockedExchange((LONG volatile *)&g_uiDebugUSB3SizeBufferCurrent,(LONG)(iSize/1024));
			InterlockedExchange((LONG volatile *)&g_uiDebugUSB3SizeAllBufferCurrent,(LONG)uiSizeAllBuffer);
			if(g_uiDebugUSB3SizeAllBufferMax<uiSizeAllBuffer)
				InterlockedExchange((LONG volatile *)&g_uiDebugUSB3SizeAllBufferMax,(LONG)uiSizeAllBuffer);
		}
		//Display the first ascan in the stream.
		ResetAcqInfoEx(auxInfo);
		pStreamHeader = (CStream_0x0001*)data;
		size = sizeof(CStream_0x0001);
		while(size<iSize)
		{
			pData = &pbyData[size];
			pdwData = (DWORD*)pData;
			pAscanHeader = (CSubStreamAscan_0x0103*)pData;
			if(pAscanHeader->Check())
			{
				m_ullAscanCount++;
				m_ullDataSizeAscan += pAscanHeader->dataSize*pAscanHeader->dataCount;
				pBufferMax = (const void *)&pbyData[sizeof(CStream_0x0001)+sizeof(CSubStreamAscan_0x0103)];
				return g_pView->ProcessAcquisitionAscan_0x00010103((int)pAscanHeader->cycle,auxInfo,pStreamHeader,pAscanHeader,pBufferMax,NULL,NULL);
			}
			size += HIWORD(*pdwData)+sizeof(DWORD);
		}
	}
	return 0;
}

UINT WINAPI CDlgHW::ProcessAcquisitionAscan_0x00010103(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat)
{
	int iCycle;
	structAcqInfoEx auxInfo;
	structAcqChannelInfo chInfo;

	if(g_bExit)
		return 0;
	if(CDlgHW::m_iIOMaxTemperature<(int)acqInfo.lMaxTemperature)
		CDlgHW::m_iIOMaxTemperature = (int)acqInfo.lMaxTemperature;
	memcpy(&auxInfo,&acqInfo,sizeof(acqInfo));
	EnterCriticalSection(&m_CriticSectionCounters);
	if(pAscanHeader)
	{
		m_ullAscanCount++;
		m_ullDataSizeAscan += pAscanHeader->dataSize*pAscanHeader->dataCount;
		m_llEncoderAscanTickCount.LowPart = pAscanHeader->timeStampLow;
		m_llEncoderAscanTickCount.HighPart = pAscanHeader->timeStampHigh;
		m_dwEncoderAscanTickCount = GetTickCount();
		for(int i=0;i<g_iDriverEncoderCountMax;i++)
		{
			m_dEncoderAscan[i] = auxInfo.dEncoder[i];
		}
		m_dwDigitalInputsAscan = auxInfo.dwDigitalInputs;
	}
	LeaveCriticalSection(&m_CriticSectionCounters);
	if(g_pView)
	{
		iCycle = pAscanHeader->cycle;
		if(m_bFullMatrixCapture)
		{
			chInfo.lChannelProbe = 0;
			chInfo.lChannelScan = pAscanHeader->cycle;
			chInfo.lChannelCycle = auxInfo.lFMCElementIndex-m_iFMCElementStart;
			auxInfo.bMultiChannel = true;
		}else
		if(acqInfo.bMultiChannel)
		{
			chInfo.lChannelProbe = (long)pAscanHeader->FWAcqIdChannelProbe;
			chInfo.lChannelScan = (long)pAscanHeader->FWAcqIdChannelScan;
			chInfo.lChannelCycle = (long)pAscanHeader->FWAcqIdChannelCycle;
		}else{
			chInfo.lChannelProbe = -1;
			chInfo.lChannelScan = -1;
			chInfo.lChannelCycle = -1;
		}
		if(CDlgHW::m_pDlgHW->ProcessMultiChannel(chInfo,auxInfo,iCycle))
		{
			return g_pView->ProcessAcquisitionAscan_0x00010103(iCycle,auxInfo,pStreamHeader,pAscanHeader,pBufferMax,pBufferMin,pBufferSat);
		}
	}
	return 0;
}

UINT WINAPI CDlgHW::ProcessAcquisitionIO_1x00010101(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamIO_0x0101 *pIOHeader)
{
	if(g_bExit)
		return 0;
	if(CDlgHW::m_iIOMaxTemperature<(int)pIOHeader->maxTemperature)
		CDlgHW::m_iIOMaxTemperature = (int)pIOHeader->maxTemperature;
	if(g_pView)
	{
		if(pIOHeader->edges)
		{
			//digital inputs has been updated! => pIOHeader->inputs
		}
		return 0;
	}
	return 0;
}

typedef union unionAmp{
	char charData;
	BYTE byteData;
}unionAmp;
UINT WINAPI CDlgHW::ProcessAcquisitionCscan_0x00010X02(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp, const structCscanAmpTof_0x0202 *pBufferAmpTof)
{
	int iCycle,iDataSize;
	UINT uiRet;

	if(g_bExit)
		return 0;
	EnterCriticalSection(&m_CriticSectionCounters);
	if(CDlgHW::m_iIOMaxTemperature<(int)acqInfo.lMaxTemperature)
		CDlgHW::m_iIOMaxTemperature = (int)acqInfo.lMaxTemperature;
	if(pCscanHeader)
	{
		iDataSize = pCscanHeader->count*sizeof(DWORD);
		if(pBufferAmp && !pBufferAmpTof)
			m_ullDataSizeCscan += iDataSize;
		if(!pBufferAmp && pBufferAmpTof)
			m_ullDataSizeCscan += iDataSize*2;
		m_llEncoderCscanTickCount.LowPart = pCscanHeader->timeStampLow;
		m_llEncoderCscanTickCount.HighPart = pCscanHeader->timeStampHigh;
		m_dwEncoderCscanTickCount = GetTickCount();
		for(int i=0;i<g_iDriverEncoderCountMax;i++)
		{
			m_dEncoderCscan[i] = acqInfo.dEncoder[i];
		}
		m_dwDigitalInputsCscan = acqInfo.dwDigitalInputs;
	}
	LeaveCriticalSection(&m_CriticSectionCounters);
	if(g_pView)
	{
		iCycle = pCscanHeader->cycle;//current cycle (single channel)
		uiRet = g_pView->ProcessAcquisitionCscan_0x00010X02(iCycle,acqInfo,pStreamHeader,pCscanHeader,pBufferAmp,pBufferAmpTof);
		return uiRet;
	}
	return 0;
}

structCorrectionOEMPA* WINAPI CDlgHW::CallbackCustomizedAPI(void *pAcquisitionParameter,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception)
{
	int iChannelProbe,iChannelScan,iChannelCycle;
	CHWDeviceOEMPA *pOEMPA=(CHWDeviceOEMPA*)pAcquisitionParameter;
	bool bCscan=false;
	float *pf;
	int iMultiChannelAcqCycle,iMultiChannelElement,iMultiChannelElementCount;
	unionDecimation *pX;

	if(eStep==eWriteHW_Enter)
	{
		m_bFullMatrixCapture = pRoot->bEnableFMC;
		if(pRoot->eEnableTFM!=eTFMOff)
			m_bFullMatrixCapture = false;
		m_bMultiHWChannel = pRoot->bEnableMultiHWChannel;
		if(m_bMultiHWChannel)
		{
			m_fMultiChannelPeriod = pOEMPA->GetSWDeviceOEMPA()->fGetClockPeriod();
			iMultiChannelAcqCycle = -1;
			for(int iCycle=0;iCycle<pRoot->iCycleCount;iCycle++)
			{
				if(pCycle[iCycle].bHWAcquisition)
					iMultiChannelAcqCycle = iCycle;
				m_abyMultiChannelDecimation[iCycle] = 0;
				m_afMultiChannelStart[iCycle] = 0.0f;
				if(iMultiChannelAcqCycle>=0)
				{
					pf = &pReception[iMultiChannelAcqCycle].afDelay[0][0];
					if(pReception[iCycle].adwElement.GetListElement(1,&iMultiChannelElement,iMultiChannelElementCount) && (iMultiChannelElementCount==1))
					{
						if(iCycle)
							m_afMultiChannelStart[iCycle] = pf[iMultiChannelElement];
						pX = (unionDecimation*)&pReception[iMultiChannelAcqCycle].hwAcqDecimation.byte[iMultiChannelElement/2];
						if(iMultiChannelElement%2)
							m_abyMultiChannelDecimation[iCycle] = (pX->data.msb+1);
						else
							m_abyMultiChannelDecimation[iCycle] = (pX->data.lsb+1);
					}
				}
			}
		}
		EnterCriticalSection(&m_ChannelCriticalSection);
		if(m_bFullMatrixCapture && pFileName && wcsstr(pFileName,L"Cfg\\DefaultSetupOEMPA.txt") && pCycle)
		{
			m_iFMCElementStart = pRoot->iFMCElementStart;
			m_iFMCElementStop = pRoot->iFMCElementStop;
			m_iFMCElementStep = pRoot->iFMCElementStep;
			m_iFMCElementCount = m_iFMCElementStop-m_iFMCElementStart+1;
			if(m_iFMCElementCount%pRoot->iFMCElementStep>0)
				m_iFMCElementCount = 1+m_iFMCElementCount/m_iFMCElementStep;
			else
				m_iFMCElementCount = m_iFMCElementCount/m_iFMCElementStep;
			for(int iCycle=0;iCycle<pRoot->iCycleCount;iCycle++)
			{
				//1.1.5.3l pCycle[iCycle].dTimeSlot *= (m_iFMCElementStop-m_iFMCElementStart+1);
				pCycle[iCycle].dTimeSlot *= m_iFMCElementCount;
			}
		}
		LeaveCriticalSection(&m_ChannelCriticalSection);
	}
	if(eStep==eWriteHW_Leave)
	{
		m_bFullMatrixCapture = pRoot->pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCapture();
		if(pRoot->eEnableTFM!=eTFMOff)
			m_bFullMatrixCapture = false;
		m_bMultiHWChannel = pRoot->pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsMultiHWChannelEnabled();
		EnterCriticalSection(&m_ChannelCriticalSection);
		if(m_bFullMatrixCapture)
		{
			m_iFMCElementStart = pRoot->iFMCElementStart;
			m_iFMCElementStop = pRoot->iFMCElementStop;
			m_iFMCElementStep = pRoot->iFMCElementStep;
			m_iFMCElementCount = m_iFMCElementStop-m_iFMCElementStart+1;
			if(m_iFMCElementCount%pRoot->iFMCElementStep>0)
				m_iFMCElementCount = 1+m_iFMCElementCount/m_iFMCElementStep;
			else
				m_iFMCElementCount = m_iFMCElementCount/m_iFMCElementStep;
		}
		m_iChannelProbeCount = 0;
		memset(m_iChannelScanCount,0,sizeof(m_iChannelScanCount));
		memset(m_iChannelCycleCount,0,sizeof(m_iChannelCycleCount));
		if(m_bFullMatrixCapture)
		{
			if(CDlgHW::m_pDlgHW)
				CDlgHW::m_pDlgHW->UpdateFMC(pRoot->iCycleCount);
		}else{
			for(int iCycle=0;iCycle<pRoot->iCycleCount;iCycle++)
			{
				if((pCycle[iCycle].wAcqIdChannelProbe!=0xffff) && (pCycle[iCycle].wAcqIdChannelScan!=0xffff))
				{
					iChannelProbe = (int)pCycle[iCycle].wAcqIdChannelProbe;
					iChannelScan = (int)pCycle[iCycle].wAcqIdChannelScan;
					iChannelCycle = (int)pCycle[iCycle].wAcqIdChannelCycle;
					if(m_iChannelProbeCount<=iChannelProbe)
						m_iChannelProbeCount = iChannelProbe+1;
					if((0<=iChannelProbe) && (iChannelProbe<g_iOEMMCProbeCountMax) && (0<=iChannelScan) && (iChannelScan<g_iOEMMCScanCountMax))
					{
						if(m_iChannelScanCount[iChannelProbe]<=iChannelScan)
							m_iChannelScanCount[iChannelProbe] = iChannelScan+1;
						if((iChannelScan<16) && (m_iChannelCycleCount[iChannelProbe][iChannelScan]<=iChannelCycle))
							m_iChannelCycleCount[iChannelProbe][iChannelScan] = iChannelCycle+1;
					}
				}
				if(pCycle[iCycle].iGateCount>0)
				{
					for(int iGateIndex=0;iGateIndex<g_iOEMPAGateCountMax;iGateIndex++)
					{
						if(pCycle[iCycle].gate[iGateIndex].bEnable)
							bCscan = true;
					}
				}
			}
			m_iChannelCount = 0;
			for(int iChannelProbe=0;iChannelProbe<m_iChannelProbeCount;iChannelProbe++)
			{
				for(int iChannelScan=0;iChannelScan<m_iChannelScanCount[iChannelProbe];iChannelScan++)
				{
					if(!m_iChannelCycleCount[iChannelProbe][iChannelScan])
						break;
					m_iChannelCount++;
				}
			}
		}
		LeaveCriticalSection(&m_ChannelCriticalSection);
		//preparethe view in order to not lost acquisition data.
		if(g_pView)
			g_pView->UpdateCycleCountData(pRoot->iCycleCount);
		::PostMessage(m_pDlgHW->GetSafeHwnd(),g_uiUTEventMessage,WPARAM_UPDATE_COMBO_CHANNEL,(LPARAM)m_iChannelCount);
	}
	CDlgCscanDisplay::CallbackCustomizedAPI(pOEMPA,pFileName,eStep,pRoot,pCycle,pEmission,pReception);
	if(bCscan && CDlgCscanDisplay::IsDisplayed())
		::PostMessage(m_pDlgHW->GetSafeHwnd(),g_uiUTEventMessage,WPARAM_CSCAN_WARNING,0);
	return &g_Correction;
}

void CDlgHW::DisplayEncoder()
{
	DWORD dwTickCount=GetTickCount();
	double dThroughputAscan,dThroughputCscan;
	double dEncoder[2];
	LARGE_INTEGER llEncoderTickCount;

	if(!m_pDlgHW)
		return;
	EnterCriticalSection(&m_CriticSectionCounters);
	if(m_dwEncoderAscanTickCount<m_dwEncoderCscanTickCount)
	{
		for(int i=0;i<g_iDriverEncoderCountMax;i++)
		{
			dEncoder[i] = m_dEncoderCscan[i];
		}
		llEncoderTickCount.QuadPart = m_llEncoderCscanTickCount.QuadPart;
	}else{
		for(int i=0;i<g_iDriverEncoderCountMax;i++)
		{
			dEncoder[i] = m_dEncoderAscan[i];
		}
		llEncoderTickCount.QuadPart = m_llEncoderAscanTickCount.QuadPart;
	}
	if(m_dwDisplayTickCount==-1)
	{
		dThroughputAscan = 0.0;
		dThroughputCscan = 0.0;
	}else{
		dThroughputAscan = (double)(m_ullDataSizeAscan-m_ullDisplayDataSizeAscan);
		dThroughputAscan *= 1000.0/(dwTickCount-m_dwDisplayTickCount);
		dThroughputAscan /= 1024*1024;
		dThroughputCscan = (double)(m_ullDataSizeCscan-m_ullDisplayDataSizeCscan);
		dThroughputCscan *= 1000.0/(dwTickCount-m_dwDisplayTickCount);
		dThroughputCscan /= 1024;
	}
	m_ullDisplayDataSizeAscan = m_ullDataSizeAscan;
	m_ullDisplayDataSizeCscan = m_ullDataSizeCscan;
	m_dwDisplayTickCount = dwTickCount;
	LeaveCriticalSection(&m_CriticSectionCounters);
	if(g_uiDebugUSB3SizeAllBufferMax || g_uiDebugUSB3SizeAllBufferCurrent || g_uiDebugUSB3SizeBufferCurrent)
	{
		if(!dThroughputAscan && !dThroughputCscan)
			m_strEncoder.Format(L"X=%.2f Y=%.2f In=0x%.2X\r\nUSB KB: %u %u %u",dEncoder[0]*1e3,dEncoder[1]*1e3,m_dwDigitalInputsAscan,g_uiDebugUSB3SizeAllBufferMax,g_uiDebugUSB3SizeAllBufferCurrent,g_uiDebugUSB3SizeBufferCurrent);
		else if(dThroughputAscan && !dThroughputCscan)
			m_strEncoder.Format(L"X=%.2f Y=%.2f In=0x%.2X\r\nA=%.2f MB/sec\r\nUSB KB: %u %u %u",dEncoder[0]*1e3,dEncoder[1]*1e3,m_dwDigitalInputsAscan,dThroughputAscan,g_uiDebugUSB3SizeAllBufferMax,g_uiDebugUSB3SizeAllBufferCurrent,g_uiDebugUSB3SizeBufferCurrent);
		m_pDlgHW->m_EditEncoder.SetWindowText(m_strEncoder);
		return;
	}
	if(!dThroughputAscan && !dThroughputCscan)
		m_strEncoder.Format(L"X=%.2f Y=%.2f In=0x%.2X",dEncoder[0]*1e3,dEncoder[1]*1e3,m_dwDigitalInputsAscan);
	else if(dThroughputAscan && !dThroughputCscan)
		m_strEncoder.Format(L"X=%.2f Y=%.2f In=0x%.2X\r\nA=%.2f MB/sec",dEncoder[0]*1e3,dEncoder[1]*1e3,m_dwDigitalInputsAscan,dThroughputAscan);
	else if(!dThroughputAscan && dThroughputCscan)
		m_strEncoder.Format(L"X=%.2f Y=%.2f In=0x%.2X\r\nC=%.2f KB/sec",dEncoder[0]*1e3,dEncoder[1]*1e3,m_dwDigitalInputsAscan,dThroughputCscan);
	else if(dThroughputAscan && dThroughputCscan)
		m_strEncoder.Format(L"X=%.2f Y=%.2f In=0x%.2X\r\nA=%.2f MB/sec C=%.2f KB/sec",dEncoder[0]*1e3,dEncoder[1]*1e3,m_dwDigitalInputsAscan,dThroughputAscan,dThroughputCscan);
	m_pDlgHW->m_EditEncoder.SetWindowText(m_strEncoder);
}

bool CDlgHW::DriverRegistration()
{
	bool bError=false;
	__int64 iBufferByteSize=(__int64)2*(__int64)8*(__int64)1024*(__int64)1024;
#ifdef _WIN64
	CAcquisitionFifo *pFifoAscan,*pFifoCscan,*pFifoIO;
#endif _WIN64

	if(!m_pHWDeviceOEMPA)
	{
		OEMPA_SetCallbackCustomizedDriverAPI(CDlgHW::CallbackCustomizedAPI);
		m_pHWDeviceOEMPA = new CHWDeviceOEMPA();
		if(!m_pHWDeviceOEMPA)
			bError = true;
		if(!bError && !m_pHWDeviceOEMPA->SetAcquisitionParameter(g_pView))
			bError = true;
		if(!bError)
		{
#ifdef _WIN64
			pFifoAscan = m_pHWDeviceOEMPA->GetAcquisitionFifo(eFifoAscan);
			if(m_bFifoAscanEnable && !pFifoAscan->Alloc(1024, iBufferByteSize))//function to enable the fifo for ascan.
				bError = true;
			pFifoCscan = m_pHWDeviceOEMPA->GetAcquisitionFifo(eFifoCscan);
			if(m_bFifoCscanEnable && !pFifoCscan->Alloc(1024, 1024*1024))//function to enable the fifo for cscan.
				bError = true;
			pFifoIO = m_pHWDeviceOEMPA->GetAcquisitionFifo(eFifoIO);
#endif _WIN64
			if(!m_bFifoAscanEnable || (m_bFifoAscanEnable && m_bFifoAscanDriverThread))
			{
				if(!m_pHWDeviceOEMPA->SetAcquisitionAscan_0x00010103(CDlgHW::ProcessAcquisitionAscan_0x00010103))
					bError = true;
			}
			if(!m_bFifoCscanEnable || (m_bFifoCscanEnable && m_bFifoCscanDriverThread))
			{
				if(!bError && !m_pHWDeviceOEMPA->SetAcquisitionCscan_0x00010X02(ProcessAcquisitionCscan_0x00010X02))
					bError = true;
			}
			if(!m_bFifoIOEnable || (m_bFifoIOEnable && m_bFifoIODriverThread))
			{
				if(!bError && !m_pHWDeviceOEMPA->SetAcquisitionIO_1x00010101(ProcessAcquisitionIO_1x00010101,true))
					bError = true;
			}
			if(	(m_bFifoAscanEnable && !m_bFifoAscanDriverThread) ||
				(m_bFifoCscanEnable && !m_bFifoCscanDriverThread) ||
				(m_bFifoIOEnable && !m_bFifoIODriverThread))
			{//explicit thread to manage the fifoAscan. In case of the integrated thread to manage the fifo (in the driver) no explicit thread is required.
				m_hThread = CreateThread(NULL,TRUE,ThreadFifo,this,0,NULL);	
			}
		}
		UpdateData(FALSE);
		if(m_pHWDeviceOEMPA)
		{
			m_bUnlockEnableShot = m_pHWDeviceOEMPA->GetSWDevice()->GetUnlockEnableShot();
			if(m_bUnlockEnableShot)
				m_pHWDeviceOEMPA->GetSWDevice()->SetUnlockDefaultEnablePulser(false);
			m_IPAddress.SetAddress(m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
		}
	}
	if(bError)
		AfxMessageBox(L"ERROR to register the driver instance !!!");
	return !bError;
}

bool GetIPAddress(const wchar_t *pIPAddress,BYTE byAddr[4])
{
	int aiAddr[4],iIPIndex;
	int iIndex;
	const wchar_t *pAux=pIPAddress;

	aiAddr[3] = _wtoi(pIPAddress);
	aiAddr[2] = 0;
	aiAddr[1] = 0;
	aiAddr[0] = 0;
	iIPIndex = 2;
	iIndex = 0;
	do{
		while(pIPAddress[iIndex] && (pIPAddress[iIndex]!=L'.'))
			iIndex++;
		if(pIPAddress[iIndex]==L'.')
		{
			pAux = &pIPAddress[iIndex+1];
			aiAddr[iIPIndex] = _wtoi(pAux);
			iIPIndex--;
		}
		if(!pIPAddress[iIndex] && (iIPIndex>=0))
			return false;
		iIndex++;
	}while(iIPIndex>=0);
	byAddr[0] = aiAddr[3];
	byAddr[1] = aiAddr[2];
	byAddr[2] = aiAddr[1];
	byAddr[3] = aiAddr[0];
	return true;
}

bool CDlgHW::UpdateDriver()
{
	wchar_t pValue[40];
	unsigned short usValue;
	bool bRet=true;
	bool bConnected=false;

	if(!m_pHWDeviceOEMPA)
		return false;
	if(m_pHWDeviceOEMPA->GetSWDevice()->IsConnected())
		bConnected = true;
	m_IPAddress.GetAddress(m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
	usValue = (unsigned short)m_dwEditPort;
	swprintf(pValue,40,L"%d.%d.%d.%d",m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
	m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->SetIPAddress(pValue);
	m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->SetPort(usValue);
	m_IPAddress.SetAddress(m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
	return bRet;
}

bool CDlgHW::UpdateCycle()
{
	CString string;
	int iComboCycle,iCycleCount;

	if(m_bFullMatrixCapture)
	{
		if(!HWReadCycleCount(iCycleCount))
			return false;
		if(!m_iComboChannel && !m_bCheckFMCCalibration)
			iCycleCount *= m_iFMCElementCount;//1.1.5.3l (m_iFMCElementStop-m_iFMCElementStart+1);
		else
			iCycleCount = m_iFMCElementCount;//(m_iFMCElementStop-m_iFMCElementStart+1);
		iComboCycle = m_iComboCycle;
		m_ComboCycle.ResetContent();
		m_ComboCycle.AddString(L"All");
		for(int iCycle=0;iCycle<iCycleCount;iCycle++)
		{
			string.Format(L"%d",iCycle);
			m_ComboCycle.AddString(string);
		}
		if((iComboCycle<0) || (m_iComboCycle>iCycleCount))
			m_iComboCycle = 0;
		UpdateData(FALSE);
		if(g_pView)
			g_pView->UpdateCycleCount(iCycleCount);
		return true;
	}
	if(!m_bCheckConnect)
	{
		m_ComboCycle.ResetContent();
		if(g_pView)
			g_pView->UpdateCycleCount(0);
		iComboCycle = -1;
		return true;
	}
	if(!HWReadCycleCount(m_iCycleCount))
	{
		AfxMessageBox(L"Communication error!");
		return false;
	}
	iComboCycle = m_iComboCycle;
	m_ComboCycle.ResetContent();
	m_ComboCycle.AddString(L"All");
	for(int iCycle=0;iCycle<m_iCycleCount;iCycle++)
	{
		string.Format(L"%d",iCycle);
		m_ComboCycle.AddString(string);
	}
	if((iComboCycle<0) || (m_iComboCycle>m_iCycleCount))
		m_iComboCycle = 0;
	if(g_pView)
		g_pView->UpdateCycleCount(m_iCycleCount);
	if(g_Correction.iCount==m_iCycleCount)
		m_bEnableCorrection = true;
	else
		m_bEnableCorrection = false;
	UpdateData(FALSE);
	return true;
}

bool CDlgHW::UpdateGain()
{
	CString string;
	int iPos;

	if(!m_bCheckConnect)
	{
		m_dEditGain = 0.0;
		m_SpinGain.SetPos32(0);
		return true;
	}
	if(!HWReadGainAll(m_adGain,m_iCycleCount))
	{
		AfxMessageBox(L"Communication error!");
		return false;
	}
	if(!m_iComboCycle)
		m_dEditGain = m_adGain[0];
	if(m_iComboCycle>0)
		m_dEditGain = m_adGain[m_iComboCycle-1];
	iPos = (int)(CDlgHW::m_dEditGain*10.0);
	m_SpinGain.SetPos32(iPos);
	UpdateData(FALSE);
	return true;
}

void CDlgHW::UpdateDialog_()
{
	bool bConnected;
	bool bIOBoard;

	if(!m_pHWDeviceOEMPA)
		return;
	bConnected = (m_pHWDeviceOEMPA->GetSWDevice()->IsConnected()?true:false);
	m_bCheckConnect = bConnected?TRUE:FALSE;
	if(!m_bCheckConnect)
	{
		m_EditPort.EnableWindow(TRUE);
		m_IPAddress.EnableWindow(TRUE);
		m_dwEditError = 0;
	}
	m_EditPort.EnableWindow(!bConnected);
	m_IPAddress.EnableWindow(!bConnected);
	m_ComboUsb.EnableWindow(!bConnected);
	if(bConnected && m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetFlashUSB3Version())
	{
		m_RadioEthernet.EnableWindow(true);
		m_RadioUsb.EnableWindow(true);
		m_CheckTest.ShowWindow(SW_SHOW);
		m_ButtonResetUSB3.ShowWindow(SW_SHOW);
		m_RadioEthernet.ShowWindow(SW_SHOW);
		m_RadioUsb.ShowWindow(SW_SHOW);
		if(m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsUSB3Enabled())
		{
			m_iRadioEthernet = 1;
			m_CheckTest.EnableWindow(TRUE);
			m_ButtonResetUSB3.EnableWindow(TRUE);
		}else{
			m_iRadioEthernet = 0;
			m_CheckTest.EnableWindow(FALSE);
			m_ButtonResetUSB3.EnableWindow(FALSE);
		}
	}else{
		m_RadioEthernet.EnableWindow(false);
		m_RadioUsb.EnableWindow(false);
		m_CheckTest.ShowWindow(SW_HIDE);
		m_ButtonResetUSB3.ShowWindow(SW_HIDE);
		m_RadioEthernet.ShowWindow(SW_HIDE);
		m_RadioUsb.ShowWindow(SW_HIDE);
	}
	m_EditPort.ShowWindow(SW_SHOW);
	m_IPAddress.ShowWindow(SW_SHOW);
	m_ComboUsb.ShowWindow(SW_HIDE);
	m_ButtonAPISamples.EnableWindow(bConnected);
	m_ButtonFileToHW.EnableWindow(bConnected);
	m_ButtonHWToFile.EnableWindow(bConnected);
	m_ButtonWizardToHW.EnableWindow(bConnected && m_bCheckWizardEnable);
	m_ButtonReset.EnableWindow(bConnected);
	m_ButtonStatus.EnableWindow(bConnected);
	m_ButtonColor.EnableWindow(m_bCheckDisplayImage);
	m_ComboCycle.EnableWindow(bConnected);
	m_EditGain.EnableWindow(bConnected);
	m_CheckShotEnable.EnableWindow(bConnected);
	m_CheckDisplayMax.EnableWindow(bConnected);
	m_EditButtonOutput.EnableWindow(bConnected);
	m_EditCalibration.EnableWindow(bConnected);
	m_CheckFileFormatAddressData.EnableWindow(bConnected);
	m_CheckFileFormatAddressData.ShowWindow(g_bAdvanced);
	if(bConnected && m_pHWDeviceOEMPA)
		bIOBoard = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsIOBoardEnabled();
	else
		bIOBoard = false;
	m_EditButtonOutput.EnableWindow(bIOBoard);
	if(!m_iChannelCount)
		m_iComboChannel = 0;
	m_ButtonToolbox.EnableWindow(IsWow64()?FALSE:TRUE);
	if(!m_bFullMatrixCapture)
	{
		if(m_bMultiHWChannel)
		{
			m_StaticChannel.SetWindowText(L"OEMMC");
		}else{
			if((m_iChannelCount>0) && CUTChannels::IsDefaultMultiChannels())
				m_StaticChannel.SetWindowText(L"Channel (Probe-Scan)");
			else
				m_StaticChannel.SetWindowText(L"Single Channel");
		}
		m_StaticCycle.SetWindowText(L"Cycle");
		m_ComboChannel.ShowWindow(m_iChannelCount>0?SW_SHOW:SW_HIDE);
	}else{
		m_StaticChannel.SetWindowText(L"Full Matrix Capture (cycle)");
		if(!m_iComboChannel)
			m_StaticCycle.SetWindowText(L"ALL");
		else
			m_StaticCycle.SetWindowText(L"Element");
		m_ComboChannel.ShowWindow(SW_SHOW);
	}
	m_EditGain.ShowWindow(m_bFullMatrixCapture?SW_HIDE:SW_SHOW);
	m_SpinGain.ShowWindow(m_bFullMatrixCapture?SW_HIDE:SW_SHOW);
	m_StaticGainUnit.ShowWindow(m_bFullMatrixCapture?SW_HIDE:SW_SHOW);
	//m_EditCalibration.ShowWindow(m_bFullMatrixCapture?SW_HIDE:SW_SHOW);
	m_CheckFMCCalibration.ShowWindow(m_bFullMatrixCapture?SW_SHOW:SW_HIDE);
	UpdateData(FALSE);
}

LRESULT CDlgHW::OnEventMessage(WPARAM wParam,LPARAM lParam)
{
	CString strUsb=m_strComboUsb;
	int iUsbDevice;

	iUsbDevice = -1;
	if((wParam==WPARAM_RUN_TOOLBOX) && (lParam==LPARAM_RUN_TOOLBOX_OK))
		g_RunToolbox.SignalPostFeedback();//No error
	if((wParam==WPARAM_RUN_TOOLBOX) && (lParam==LPARAM_RUN_TOOLBOX_ERROR))
		AfxMessageBox(L"Error to run the toolbox, please try again!");
	if(wParam==WPARAM_UPDATE_COMBO_CHANNEL)
	{
		UpdateCycle();//1.1.5.0 (required for the calibration)
		UpdateComboChannel();
		UpdateDialog_();
	}
	if(wParam==WPARAM_CSCAN_WARNING)
	{
		CDlgCscanDisplay::_UpdateDialog();
	}
	return 0;
}

DWORD WINAPI CDlgHW::CallbackThreadToolbox(HWND hWnd,CRunToolbox *pRunToolbox)
{
	CUTPreference *pPref=CUTPreference::GetCurrentPreference();
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
		if(pPref && pPref->GetID(iRootId))
		{
			pRunToolbox->ResetPostFeedback();
			UTKernel_ToolboxDisplay(iRootId,hWnd,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
			if(!pRunToolbox->WaitPostFeedback(5000,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_ERROR))
				return 3;
		}
	}
	return 0;//no error
}

void CDlgHW::ProcessConnection()
{
	m_iIOMaxTemperature = 0;
	m_bFullMatrixCapture = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCapture();
	if(m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsTFMProcessed())
		m_bFullMatrixCapture = false;
	m_bMultiHWChannel = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsMultiHWChannelEnabled();
	if(m_bFullMatrixCapture)
		HWReadFMC();
	UpdateCycle();
	UpdateGain();
	UpdateComboChannel();
}

void CDlgHW::OnBnClickedCheckConnect()
{
	BOOL bCheckConnect=m_bCheckConnect;
	CWaitCursor wait;
	wchar_t pValue[40];
	DWORD dwProcessId;

	UpdateData();
	if(!bCheckConnect && m_bCheckConnect)
	{
		m_IPAddress.GetAddress(m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
		swprintf(pValue,40,L"%d.%d.%d.%d",m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
		if(CHWDeviceOEMPA::IsMultiProcessConnected(pValue,dwProcessId))
			CHWDeviceOEMPA::DisconnectMultiProcess(pValue,dwProcessId);
		m_strEditLostCount = L"";
		if(!m_pHWDeviceOEMPA->GetSWDevice()->SetConnectionState(eConnected,true))
			AfxMessageBox(L"Error : Can't connect");
//if(m_pHWDeviceOEMPA->LockDevice())
//{
//m_pHWDeviceOEMPA->TestFWMemoryCycle(64,0x0);
//m_pHWDeviceOEMPA->UnlockDevice();
//}
		ProcessConnection();
	}
	if(bCheckConnect && !m_bCheckConnect)
		m_pHWDeviceOEMPA->GetSWDevice()->SetConnectionState(eDisconnected,true);
	UpdateDialog_();
	CDlgSetupFiles::SetConnect(m_bCheckConnect?true:false);
	OnBnClickedCheckTest();
}

bool CDlgHW::HWReadCycleCount(int &iCycleCount)
{
	int iCycleCount2;
	structTFMParameters tfmParameters;

	if(	m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCapture() &&
		m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsTFMProcessed(&tfmParameters))
	{
		iCycleCount = tfmParameters.iCycleCountOut;
		return true;
	}
	iCycleCount2 = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetCycleCount();
	if(iCycleCount2<0)
		return false;//no need to change cycle count if iCycleCount2==-1
	if(iCycleCount2>100000)
		return false;
	iCycleCount = iCycleCount2;
	return true;
}

void CDlgHW::UpdateFMC(int iCycleCount)
{
	m_iChannelProbeCount = 1;
	m_iChannelCount = iCycleCount;
	m_iChannelScanCount[0] = iCycleCount;
	for(int iChannelScan=0;(iChannelScan<iCycleCount) && (iChannelScan<g_iOEMMCScanCountMax);iChannelScan++)
	{
		m_iChannelCycleCount[0][iChannelScan] = m_iFMCElementCount;
	}
}

bool CDlgHW::HWReadFMC()
{
	bool bRet=true;
	int iCycleCount;
	bool bFullMatrixCapture = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCapture();

	if(!bFullMatrixCapture)
		return true;
	if(!m_pHWDeviceOEMPA->LockDevice(eAcqOff))
		return false;
	if(m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFMCElementStepSupported())
	{
		if(!m_pHWDeviceOEMPA->GetFMCElement(&m_iFMCElementStart,&m_iFMCElementStop,&m_iFMCElementStep))
			bRet = false;
	}else{
		m_iFMCElementStep = 1;
		if(!m_pHWDeviceOEMPA->GetFMCElementStart(&m_iFMCElementStart))
			bRet = false;
		if(!m_pHWDeviceOEMPA->GetFMCElementStop(&m_iFMCElementStop))
			bRet = false;
	}
	if(!m_pHWDeviceOEMPA->GetCycleCount(&iCycleCount))
		bRet = false;
	if(!m_pHWDeviceOEMPA->UnlockDevice())
		bRet = false;
	UpdateFMC(iCycleCount);
	return bRet;
}

bool CDlgHW::HWWriteGain256(int iCycle,double &dGain)
{
	bool bRet;
	CHWDeviceOEMPA *pSlave=(CHWDeviceOEMPA*)m_pHWDeviceOEMPA->GetMatched256DeviceOEMPA();

	if(!pSlave)
		return false;
	if(m_bFullMatrixCapture)
		return false;
	if(!m_iComboCycle)
		return false;
	if(!m_pHWDeviceOEMPA->LockDevice(eAcqOff))
		return false;
	if(!pSlave->LockDevice(eAcqOff))
		return false;
	bRet = m_pHWDeviceOEMPA->SetGainDigital(iCycle,dGain);
	if(!pSlave->UnlockDevice())
		bRet = false;
	if(!m_pHWDeviceOEMPA->UnlockDevice())
		bRet = false;
	return bRet;
}

bool CDlgHW::HWWriteGain(int iCycle,double &dGain)
{
	bool bRet;

	if(m_pHWDeviceOEMPA->GetMatched256Device())
		return HWWriteGain256(iCycle, dGain);
	if(m_bFullMatrixCapture)
		return false;
	if(!m_iComboCycle)
		return false;
	if(!m_pHWDeviceOEMPA->LockDevice(eAcqOff))
		return false;
	bRet = m_pHWDeviceOEMPA->SetGainDigital(iCycle,dGain);
	if(!m_pHWDeviceOEMPA->UnlockDevice())
		bRet = false;
	return bRet;
}

bool CDlgHW::HWWriteGainAll256(double adGain[g_iOEMPACycleCountMax])
{
	bool bRet=true;
	CHWDeviceOEMPA *pSlave=(CHWDeviceOEMPA*)m_pHWDeviceOEMPA->GetMatched256DeviceOEMPA();

	if(!pSlave)
		return false;
	if(m_bFullMatrixCapture)
		return false;
	if(m_iComboCycle)
		return false;
	if(!m_pHWDeviceOEMPA->LockDevice(eAcqOff))
		return false;
	if(!pSlave->LockDevice(eAcqOff))
		return false;
	for(int iCycle=0;iCycle<m_iCycleCount;iCycle++)
		bRet = m_pHWDeviceOEMPA->SetGainDigital(iCycle,adGain[iCycle]) && bRet;
	if(!pSlave->UnlockDevice())
		bRet = false;
	if(!m_pHWDeviceOEMPA->UnlockDevice())
		bRet = false;
	return bRet;
}

bool CDlgHW::HWWriteGainAll(double adGain[g_iOEMPACycleCountMax])
{
	bool bRet=true;

	if(m_pHWDeviceOEMPA->GetMatched256Device())
		return HWWriteGainAll256(adGain);
	if(m_bFullMatrixCapture)
		return false;
	if(m_iComboCycle)
		return false;
	if(!m_pHWDeviceOEMPA->LockDevice(eAcqOff))
		return false;
	for(int iCycle=0;iCycle<m_iCycleCount;iCycle++)
	{
		bRet = m_pHWDeviceOEMPA->SetGainDigital(iCycle,adGain[iCycle]) && bRet;
	}
	if(!m_pHWDeviceOEMPA->UnlockDevice())
		bRet = false;
	return bRet;
}

bool CDlgHW::HWReadGain(int iCycle,double &dGain)
{
	bool bRet;

	if(m_bFullMatrixCapture)
		return false;
	if(!m_pHWDeviceOEMPA->LockDevice(eAcqOff))
		return false;
	bRet = m_pHWDeviceOEMPA->GetGainDigital(iCycle,&dGain);
	if(!m_pHWDeviceOEMPA->UnlockDevice())
		bRet = false;
	return bRet;
}

bool CDlgHW::HWReadGainAll(double adGain[g_iOEMPACycleCountMax],int &iCycleCount)
{
	bool bRet=true;

	if(m_bFullMatrixCapture)
		return true;
	if(!m_pHWDeviceOEMPA->LockDevice(eAcqOff))
		return false;
	if(!m_pHWDeviceOEMPA->GetCycleCount(&iCycleCount))
		bRet = false;
	if(!m_pHWDeviceOEMPA->Flush())
		bRet = false;
	if(bRet)
	{
		for(int iCycle=0;iCycle<iCycleCount;iCycle++)
		{
			bRet = m_pHWDeviceOEMPA->GetGainDigital(iCycle,&adGain[iCycle]) && bRet;
		}
	}else{
		for(int iCycle=0;iCycle<g_iOEMPACycleCountMax;iCycle++)
		{
			adGain[iCycle] = 0.0;
		}
	}
	if(!m_pHWDeviceOEMPA->UnlockDevice())
		bRet = false;
	return bRet;
}

bool CDlgHW::HWEnablePulser256(bool bEnable)
{
	enumAcquisitionState eAcqState;
	CHWDeviceOEMPA *pSlave=(CHWDeviceOEMPA*)m_pHWDeviceOEMPA->GetMatched256DeviceOEMPA();
	
	if(!pSlave)
		return false;
	if(!g_bFirstShotEnable)
		return true;
	if(bEnable)
		eAcqState = eAcqOn;
	else
		eAcqState = eAcqOff;
	if(!m_pHWDeviceOEMPA->LockDevice())
		return false;
	if(!pSlave->LockDevice())
		return false;
	if(!pSlave->UnlockDevice(eAcqState))
		return false;
	if(!m_pHWDeviceOEMPA->UnlockDevice(eAcqState))
		return false;
	return true;
}

bool CDlgHW::HWEnablePulser(bool bEnable)
{
	enumAcquisitionState eAcqState;
	CHWDeviceOEMPA *pSlave=(CHWDeviceOEMPA*)m_pHWDeviceOEMPA->GetMatched256DeviceOEMPA();
	
	if(pSlave)
		return HWEnablePulser256(bEnable);
	if(!g_bFirstShotEnable)
		return true;
	if(bEnable)
		eAcqState = eAcqOn;
	else
		eAcqState = eAcqOff;
	if(!m_pHWDeviceOEMPA->LockDevice())
		return false;
	if(!m_pHWDeviceOEMPA->UnlockDevice(eAcqState))
		return false;
	return true;
}

bool CDlgHW::SetTriggerMode(enumOEMPATrigger eTrig)
{
	bool bRet=true;

	if(!m_pHWDeviceOEMPA->LockDevice(eAcqOff))
		return false;
	m_pHWDeviceOEMPA->SetTriggerMode(eTrig);
	if(!m_pHWDeviceOEMPA->UnlockDevice())
		bRet = false;
	return bRet;
}

void CDlgHW::OnTimer(UINT_PTR nIDEvent)
{
	bool bUpdate=false;
	DWORD dwEditError;
	int iDataLostAscan,iDataLostCscan,iDataLostEncoder,iDataLostUSB3,iDataLostFifoAscan,iDataLostFifoCscan;
	CString strDataLost,strAux;

	ASSERT(nIDEvent == g_kTimerID);
	if(m_pHWDeviceOEMPA)
	{
		if(m_bCheckConnect && !m_pHWDeviceOEMPA->GetSWDevice()->IsConnected())
		{
			bUpdate = true;
			m_bCheckConnect = FALSE;
		}
		if(!g_pUsb3Device || (g_pUsb3Device->GetVersion()!=0xCAFE))
		if(!m_pHWDeviceOEMPA->GetSWDevice()->IsPulserEnabled())
		{
			if(m_bCheckShotEnable)
			{
				bUpdate = true;
				m_bCheckShotEnable = FALSE;
			}
		}else{
			if(!m_bCheckShotEnable)
			{
				bUpdate = true;
				m_bCheckShotEnable = TRUE;
			}
		}
		if(m_bCheckConnect)
		{
			dwEditError = (DWORD)m_pHWDeviceOEMPA->GetSWDevice()->GetStreamError();
			if(m_dwEditError!=dwEditError)
			{
				bUpdate = true;
				m_dwEditError = dwEditError;
			}
			iDataLostAscan = m_pHWDeviceOEMPA->GetSWDevice()->GetLostCountAscan();
			iDataLostCscan = m_pHWDeviceOEMPA->GetSWDevice()->GetLostCountCscan();
			iDataLostEncoder = m_pHWDeviceOEMPA->GetSWDevice()->GetLostCountEncoder();
			iDataLostUSB3 = m_pHWDeviceOEMPA->GetSWDevice()->GetLostCountUSB3();
			iDataLostFifoAscan = m_pHWDeviceOEMPA->GetSWDevice()->GetLostCountFifo(eFifoAscan);
			iDataLostFifoCscan = m_pHWDeviceOEMPA->GetSWDevice()->GetLostCountFifo(eFifoCscan);
			strDataLost = L"";
			if(iDataLostAscan)
			{
			      
				strAux.Format(L"A=%d ",iDataLostAscan);
				strDataLost += strAux;
			}
			if(iDataLostCscan)
			{
				strAux.Format(L"C=%d ",iDataLostCscan);
				strDataLost += strAux;
			}
			if(iDataLostEncoder)
			{
				strAux.Format(L"E=%d ",iDataLostEncoder);
				strDataLost += strAux;
			}
			if(iDataLostUSB3)
			{
				strAux.Format(L"U=%d ",iDataLostUSB3);
				strDataLost += strAux;
			}
			if(iDataLostFifoAscan)
			{
				strAux.Format(L"FA=%d ",iDataLostFifoAscan);
				strDataLost += strAux;
			}
			if(iDataLostFifoCscan)
			{
				strAux.Format(L"FC=%d ",iDataLostFifoCscan);
				strDataLost += strAux;
			}
			if(m_strEditLostCount!=strDataLost)
			{
				bUpdate = true;
				m_strEditLostCount = strDataLost;
			}
		}
		if(bUpdate)
			UpdateDialog();
	}
	CDialog::OnTimer(nIDEvent);
	CDlgCscanDisplay::OnTimer();
}

HBRUSH CDlgHW::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hBrush;

	if(0 && (&m_EditDataLost==pWnd))
	{
		if(m_strEditLostCount.GetLength()>0)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkColor(RGB(255, 0, 0));
			return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
		}
	}
	hBrush = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor == CTLCOLOR_DLG )
	{
		LOGBRUSH logBrush;
		GetObject(hBrush, sizeof(LOGBRUSH), &logBrush);
		m_bkColor = logBrush.lbColor;
		if(!m_bkColorInit)
			m_EditEncoder.SetBackgroundColor(FALSE,m_bkColor);
		m_bkColorInit = true;
	}
	return hBrush;
}

void CDlgHW::OnBnClickedOk()
{
	//OnOK();
	CWnd *pWnd=GetFocus();
	if(pWnd==&m_EditGain)
		UpdateGain_();
}

void CDlgHW::OnClose()
{
	ShowWindow(SW_HIDE);//DestroyWindow();
}

void CDlgHW::OnBnClickedCancel()
{
	ShowWindow(SW_HIDE);
}

void CDlgHW::OnBnClickedRadioEthernet()
{
	UpdateData();
	m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->EnableUSB3(false);
	UpdateDialog_();
}

void CDlgHW::OnBnClickedRadioUsb()
{
	UpdateData();
	m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->EnableUSB3(true);
	UpdateDialog_();
}

void CDlgHW::OnDeltaposSpinGain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	*pResult = 0;
	BOOL bResult;
	int iPos=m_SpinGain.GetPos32(&bResult);
	iPos = pNMUpDown->iPos+pNMUpDown->iDelta;
	CDlgHW::m_dEditGain = iPos/10.0;
	UpdateData(FALSE);
	WriteGainHW();
}

void CDlgHW::OnEnKillfocusEditGain()
{
	int iPos;

	if(!m_bGainKillfocusEnable)
		return;
	UpdateData();
	iPos = (int)(CDlgHW::m_dEditGain*10.0);
	m_SpinGain.SetPos32(iPos);
	if(IsGainModified())
		WriteGainHW();
}

void CDlgHW::UpdateGain_()
{
	int iPos;

	UpdateData();
	iPos = (int)(CDlgHW::m_dEditGain*10.0);
	m_SpinGain.SetPos32(iPos);
	m_bGainKillfocusEnable = false;
	WriteGainHW();
	m_bGainKillfocusEnable = true;
}

bool CDlgHW::IsGainModified()
{
	if(!m_iComboCycle)
	{
		for(int iCycle=1;iCycle<m_iCycleCount;iCycle++)
		{
			if(m_adGain[iCycle] != CDlgHW::m_dEditGain)
				return true;
		}
		return false;
	}
	if(m_iComboCycle>0)
	{
		if(m_adGain[m_iComboCycle-1] == CDlgHW::m_dEditGain)
			return false;
	}
	return true;
}

void CDlgHW::WriteGainHW()
{
	if(!m_iComboCycle)
	{
		for(int iCycle=0;iCycle<m_iCycleCount;iCycle++)
			m_adGain[iCycle] = CDlgHW::m_dEditGain;
		if(!HWWriteGainAll(m_adGain))
			AfxMessageBox(L"Communication error!");
	}
	if(m_iComboCycle>0)
	{
		m_adGain[m_iComboCycle-1] = CDlgHW::m_dEditGain;
		if(!HWWriteGain(m_iComboCycle-1,m_dEditGain))
			AfxMessageBox(L"Communication error!");
	}
}

void CDlgHW::OnCbnSelchangeComboDisplayCycle()
{
	int iPos;

	UpdateData();
	if(!m_iComboCycle)
		CDlgHW::m_dEditGain = m_adGain[0];
	if(m_iComboCycle>0)
		CDlgHW::m_dEditGain = m_adGain[m_iComboCycle-1];
	iPos = (int)(CDlgHW::m_dEditGain*10.0);
	m_SpinGain.SetPos32(iPos);
	UpdateData(FALSE);
	if(g_pView)
		g_pView->UpdateCycle(m_iComboCycle-1);
	if(g_pView)
		g_pView->UpdateImage(m_bCheckDisplayImage?true:false);
}

void CDlgHW::OnBnClickedCheckPulserEnable()
{
	bool bRet;
	CHWDeviceOEMPA *pSlave=(CHWDeviceOEMPA*)m_pHWDeviceOEMPA->GetMatched256DeviceOEMPA();

	UpdateData();
	if(!g_pUsb3Device)
		g_pUsb3Device = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetUSB3Device();
	if(g_pUsb3Device && (g_pUsb3Device->GetVersion()==0xCAFE))
	{
		if(m_bCheckShotEnable)
			g_pUsb3Device->EnableEmulator(1.0);
		else
			g_pUsb3Device->EnableEmulator(0.0);
		return;
	}
	UpdateData();
	if(m_bInitDialog)
		g_bFirstShotEnable = true;
	if(m_bUnlockEnableShot)
	{
		if(m_bCheckShotEnable)
			m_pHWDeviceOEMPA->GetSWDevice()->SetUnlockEnableShot(true);
		else
			m_pHWDeviceOEMPA->GetSWDevice()->SetUnlockEnableShot(false);
		if(pSlave)
		{
			if(m_bCheckShotEnable)
				pSlave->GetSWDevice()->SetUnlockEnableShot(true);
			else
				pSlave->GetSWDevice()->SetUnlockEnableShot(false);
		}
	}
	if(m_bCheckShotEnable)
		bRet = HWEnablePulser(true);
	else
		bRet = HWEnablePulser(false);
	if(!bRet)
		AfxMessageBox(L"Communication error!");
}

bool CDlgHW::IsKernelObject(wchar_t *wcObjectName,enumRootType eInRootID,int &iKernelObjectId)
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

void CDlgHW::OnBnClickedCheckFileAddDataFormat()
{
	if(AfxMessageBox(L"Be careful very low level debug access, are you sure?",MB_OKCANCEL)==IDCANCEL)
	{
		m_bCheckFileFormatAddressData = FALSE;
		UpdateDialog_();
	}
	UpdateData();
	UpdateDialog_();
}

void CDlgHW::OnBnClickedButtonFileOempaLoad()
{
	CString strFileFullPathName=L"*.txt";
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"OEMPA");
	strFileFullPathName += L"*.txt";
	CFileDialog filedialog(TRUE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	CString string;
	bool bRet;
	CHWDeviceOEMPA *pSlave=(CHWDeviceOEMPA*)m_pHWDeviceOEMPA->GetMatched256DeviceOEMPA();

	if(!m_pHWDeviceOEMPA)
		return;
	if(filedialog.DoModal()!=IDOK)
		return;
	string = filedialog.GetFileExt();
	string.MakeLower();
	if((string!=L"txt") && (string!=L"bin"))
		return;
	CWaitCursor wait;
	string = filedialog.GetPathName();
	if(pSlave && !GetPrivateProfileInt(L"Root",L"Enable256",0,string.GetBuffer()))
		pSlave = NULL;
	if(!m_bCheckFileFormatAddressData || pSlave)
	{
		if(!pSlave)
			bRet = OEMPA_ReadFileWriteHW(m_pHWDeviceOEMPA,string.GetBuffer());
		else{
			CFileDialog filedialogSlave(TRUE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
			CString stringSlave;
			//2 files are required for the 256:256.
			if(filedialogSlave.DoModal()!=IDOK)
				return;
			stringSlave = filedialogSlave.GetFileExt();
			stringSlave.MakeLower();
			if((stringSlave!=L"txt") && (stringSlave!=L"bin"))
				return;
			stringSlave = filedialogSlave.GetPathName();
			if(!GetPrivateProfileInt(L"Root",L"Enable256",0,stringSlave.GetBuffer()))
			{
				AfxMessageBox(L"Error the slave file is not able to manage the 256:256 device!");
				return;
			}
			CWaitCursor wait;
			bRet = OEMPA_ReadFileWriteHW256(m_pHWDeviceOEMPA, pSlave, string.GetBuffer(), stringSlave.GetBuffer());
		}
	}else{
		bRet = Loadfile(string);
		ProcessConnection();
	}
	UpdateData();
	m_iComboCycle = 0;
	UpdateData(FALSE);
	UpdateCycle();
	UpdateGain();
	UpdateDialog_();
	OnCbnSelchangeComboDisplayCycle();
	UTKernel_SetFolderDataFiles(L"OEMPA",string.GetBuffer());
}

void CDlgHW::OnBnClickedButtonFileOempaSave()
{
	CString strFileFullPathName=L"*.txt";
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"OEMPA");
	strFileFullPathName += L"*.txt";
	CFileDialog filedialog(FALSE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	CString string;
	CString stringSlave;
	DWORD dwAttributes;
	bool bRet=false;
	CHWDeviceOEMPA *pSlave=(CHWDeviceOEMPA*)m_pHWDeviceOEMPA->GetMatched256DeviceOEMPA();

	if(!m_pHWDeviceOEMPA)
		return;
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
	if(!pSlave)
		bRet = OEMPA_ReadHWWriteFile(m_pHWDeviceOEMPA,g_iOEMPACycleCountMax,g_iOEMPAApertureElementCountMax,g_iOEMPAFocalCountMax,g_iOEMPADACCountMax,string.GetBuffer());
	else{
		CFileDialog filedialogSlave(FALSE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
		//2 files are required for the 256:256.
		EditFile(string.GetBuffer(),false);
		if(filedialogSlave.DoModal()!=IDOK)
			return;
		stringSlave = filedialogSlave.GetFileExt();
		stringSlave.MakeLower();
		if((stringSlave!=L"txt") && (stringSlave!=L"bin"))
			return;
		CWaitCursor wait;
		stringSlave = filedialogSlave.GetPathName();
		dwAttributes = GetFileAttributes(stringSlave);
		if(dwAttributes!=INVALID_FILE_ATTRIBUTES)
			DeleteFile(stringSlave);
		HWEnablePulser256(false);
		bRet = OEMPA_ReadHWWriteFile(m_pHWDeviceOEMPA,g_iOEMPACycleCountMax,g_iOEMPAApertureElementCountMax,g_iOEMPAFocalCountMax,g_iOEMPADACCountMax,string.GetBuffer());
		bRet = OEMPA_ReadHWWriteFile(pSlave,g_iOEMPACycleCountMax,g_iOEMPAApertureElementCountMax,g_iOEMPAFocalCountMax,g_iOEMPADACCountMax,stringSlave.GetBuffer()) && bRet;
	}
	UpdateData();
	UpdateCycle();
	UpdateGain();
	UpdateDialog_();
	if(bRet)
		EditFile(string.GetBuffer(),false);
	if(pSlave && bRet)
		EditFile(stringSlave.GetBuffer(),false);
	UTKernel_SetFolderDataFiles(L"OEMPA",string.GetBuffer());
}

void CDlgHW::OnBnClickedButtonWrite()
{
	UpdateData();
	APISamples(m_pHWDeviceOEMPA);
	UpdateCycle();
	UpdateGain();
	UpdateDialog_();
}

void CDlgHW::OnBnClickedCheckDisplayImage()
{
	UpdateData();
	if(g_pView)
		g_pView->UpdateImage(m_bCheckDisplayImage?true:false);
	UpdateDialog_();
}

void CDlgHW::OnBnClickedCheckDisplayInvert()
{
	UpdateData();
	if(g_pView)
		g_pView->UpdateInvert(m_bCheckDisplayInvert?true:false);
	UpdateData(FALSE);
}

void CDlgHW::OnBnClickedButtonWizardToHw()
{
	wchar_t wcChannel[g_iMaxNameLength+1];
	wchar_t wcSystem[g_iMaxNameLength+1];
	wchar_t pRootName[g_iMaxNameLength+1];
	int iWizardChannelId;
	int iWizardSystemId;
	bool bRet;

	if(!m_pHWDeviceOEMPA)
		return;
	swprintf(pRootName,g_iMaxNameLength+1,L"OEMPA_channel_%d",m_pHWDeviceOEMPA->GetDeviceId());
	wcscpy_s(wcChannel,g_iMaxNameLength+1,pRootName);
	wcscpy_s(wcSystem,g_iMaxNameLength+1,L"OEMPA_system");
	if(!IsKernelObject(wcChannel,eRootTypeChannels,iWizardChannelId))
		return;
	if(!IsKernelObject(wcSystem,eRootTypeWizardSystem,iWizardSystemId))
		return;
	bRet = OEMPA_WriteHWWizard(m_pHWDeviceOEMPA,iWizardSystemId,iWizardChannelId);
	UpdateData();
	m_iComboCycle = 0;
	UpdateData(FALSE);
	UpdateCycle();
	UpdateGain();
	UpdateDialog_();
	OnCbnSelchangeComboDisplayCycle();
}

void CDlgHW::OnBnClickedButtonDisplayColor()
{
	CString strFileFullPathName=L"*.col";
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"Palette");
	strFileFullPathName += L"*.col";
	CFileDialog filedialog(TRUE,L"*.col",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.col)|*.col|All Files (*.*)|*.*||",this,0);
	CString string;
	int iColorsQty;
	int aiValue[256];
	int aiRed[256];
	int aiGreen[256];
	int aiBlue[256];
	wchar_t wcSection[20];
	int iValue;
	int iIndex;
	double dDelta1,dDelta2,dColor,dValue;

	if(filedialog.DoModal()!=IDOK)
		return;
	string = filedialog.GetPathName();
	if(!FileReadInteger(L"INFOS",L"COLORSQTY",string.GetBuffer(),iColorsQty))
		return;
	for(int iColor=0;iColor<iColorsQty;iColor++)
	{
		swprintf(wcSection,20,L"COLOR%d",iColor+1);
		if(!FileReadInteger(wcSection,L"VALUE",string.GetBuffer(),aiValue[iColor]))
			return;
		if(!iColor && (aiValue[0]!=0))
		{
			AfxMessageBox(L"Error : First value is not 0!\n(key \"VALUE\" in section \"COLOR1\")");
			return;
		}
		if((iColor==iColorsQty) && (aiValue[iColor]!=100))
		{
			AfxMessageBox(L"Error : Last value is not 100!\n(key \"VALUE\" in section \"COLORlast\")");
			return;
		}
		if(iColor && (aiValue[iColor]<aiValue[iColor-1]))
		{
			AfxMessageBox(L"Error : bad value incrementation!");
			return;
		}
		if(!FileReadInteger(wcSection,L"RED",string.GetBuffer(),aiRed[iColor]))
			return;
		if(!FileReadInteger(wcSection,L"GREEN",string.GetBuffer(),aiGreen[iColor]))
			return;
		if(!FileReadInteger(wcSection,L"BLUE",string.GetBuffer(),aiBlue[iColor]))
			return;
	}
	g_bColorDefault = false;
	iIndex = 0;
	for(int iColor=0;iColor<128;iColor++)
	{
		if(iIndex+1>=iColorsQty)
			return;
		dValue = 100.0*iColor/128.0;
		iValue = (int)lround2(dValue);
next:
		if(iValue<=aiValue[iIndex+1])
		{
			dDelta1 = (double)(iValue-aiValue[iIndex]);
			dDelta2 = (double)(aiValue[iIndex+1]-iValue);
			dColor = (dDelta1*aiRed[iIndex+1]+dDelta2*aiRed[iIndex])/(double)(aiValue[iIndex+1]-aiValue[iIndex]);
			g_byColorRed[iColor+128] = (BYTE)lround2(dColor);
			dColor = (dDelta1*aiGreen[iIndex+1]+dDelta2*aiGreen[iIndex])/(double)(aiValue[iIndex+1]-aiValue[iIndex]);
			g_byColorGreen[iColor+128] = (BYTE)lround2(dColor);
			dColor = (dDelta1*aiBlue[iIndex+1]+dDelta2*aiBlue[iIndex])/(double)(aiValue[iIndex+1]-aiValue[iIndex]);
			g_byColorBlue[iColor+128] = (BYTE)lround2(dColor);
			g_byColorRed[127-iColor] = g_byColorRed[iColor+128];
			g_byColorGreen[127-iColor] = g_byColorGreen[iColor+128];
			g_byColorBlue[127-iColor] = g_byColorBlue[iColor+128];
		}else{
			iIndex++;
			goto next;
		}
	}
	iIndex = 0;
	for(int iColor=0;iColor<256;iColor++)
	{
		if(iIndex+1>=iColorsQty)
			return;
		dValue = 100.0*iColor/255.0;
		iValue = (int)lround2(dValue);
nextU:
		if(iValue<=aiValue[iIndex+1])
		{
			dDelta1 = (double)(iValue-aiValue[iIndex]);
			dDelta2 = (double)(aiValue[iIndex+1]-iValue);
			dColor = (dDelta1*aiRed[iIndex+1]+dDelta2*aiRed[iIndex])/(double)(aiValue[iIndex+1]-aiValue[iIndex]);
			g_byColorRedU[iColor] = (BYTE)lround2(dColor);
			dColor = (dDelta1*aiGreen[iIndex+1]+dDelta2*aiGreen[iIndex])/(double)(aiValue[iIndex+1]-aiValue[iIndex]);
			g_byColorGreenU[iColor] = (BYTE)lround2(dColor);
			dColor = (dDelta1*aiBlue[iIndex+1]+dDelta2*aiBlue[iIndex])/(double)(aiValue[iIndex+1]-aiValue[iIndex]);
			g_byColorBlueU[iColor] = (BYTE)lround2(dColor);
		}else{
			iIndex++;
			goto nextU;
		}
	}
	if(g_pView)
		g_pView->UpdateColor();
	UTKernel_SetFolderDataFiles(L"Palette",strFileFullPathName.GetBuffer());
}

void CDlgHW::OnBnClickedButtonReset()
{
	bool bRet=true;

	if(!m_pHWDeviceOEMPA)
		return;
	if(m_pHWDeviceOEMPA->LockDevice(eAcqOff))
	{
		if(!m_pHWDeviceOEMPA->ResetTrackingTable())
			bRet = false;
		if(!m_pHWDeviceOEMPA->ResetTimeStamp())//STOP_NEXT_STANDBY
			bRet = false;
		for(int iEncoderIndex=0;iEncoderIndex<g_iOEMPAEncoderCountMax;iEncoderIndex++)
		{
			if(!m_pHWDeviceOEMPA->ResetEncoder(iEncoderIndex))
				bRet = false;
		}
		if(!m_pHWDeviceOEMPA->UnlockDevice())
			bRet = false;
	}else{
		AfxMessageBox(L"Reset Error");
		return;
	}
	if(g_pView && m_bCheckDisplayMax)
		g_pView->ResetData();
	if(bRet)
		AfxMessageBox(L"Reset OK");
	else
		AfxMessageBox(L"Error");
}

void CDlgHW::StatusExtra(CHWDeviceOempa *pHWDeviceOempa,CString &string)
{
	CSWDeviceOEMPA::structOptionsCom com;
	CSWDeviceOEMPA::structOptionsTCP TCP;
	CSWDeviceOEMPA::structOptionsFlash flash;
	CSWDeviceOEMPA::structVersion version;
	CString aux;
	int iCount=0;
	DWORD dwAux;
	wchar_t pSerialNumber[50];

	string = L"";
	if(pHWDeviceOempa->GetSWDeviceOEMPA()->GetSerialNumber(pSerialNumber,50))
	{
		if(!string.GetLength() || string[string.GetLength()-1]!=L'\n')
			string += L"\n";
		aux.Format(L"SerialNumber=\"%s\"",pSerialNumber);
		string += aux;
	}
	if(pHWDeviceOempa->GetSWDeviceOEMPA()->IsUSB3Connected())
	{
		CUTUsb3Device *pUsb3Device=pHWDeviceOempa->GetSWDeviceOEMPA()->GetUSB3Device();
		if(pUsb3Device)
		{
			wchar_t wcFlashSerialNumber[20];
			wchar_t wcSerialNumber[20];
			pUsb3Device->GetFlashSerialNumber(20,wcFlashSerialNumber);
			pUsb3Device->GetSerialNumber(20,wcSerialNumber);
			if(!string.GetLength() || string[string.GetLength()-1]!=L'\n')
				string += L"\n";
			aux.Format(L"USB3SerialNumberHW=\"%s\"",wcSerialNumber);
			string += aux;
			if(!string.GetLength() || string[string.GetLength()-1]!=L'\n')
				string += L"\n";
			aux.Format(L"USB3SerialNumberFlash=\"%s\"",wcFlashSerialNumber);
			string += aux;
		}
	}
	if(pHWDeviceOempa->GetSWDeviceOEMPA()->GetEmbeddedVersion(version))
	{
		if(!string.GetLength() || string[string.GetLength()-1]!=L'\n')
			string += L"\n";
		aux.Format(L"EmbeddedVersion=%d.%d.%d.%d",version.uMajorMSB,version.uMajorLSB,version.uMinorMSB,version.uMinorLSB);
		string += aux;
	}
	if(pHWDeviceOempa->GetSWDeviceOEMPA()->GetOptionsCom(com))
	{
		if(!string.GetLength() || string[string.GetLength()-1]!=L'\n')
			string += L"\n";
		aux.Format(L"EthernetSpeed=%d",com.uEthernetSpeed);
		string += aux;
	}
	if(pHWDeviceOempa->GetSWDeviceOEMPA()->GetOptionsTCP(TCP))
	{
		if(!string.GetLength() || string[string.GetLength()-1]!=L'\n')
			string += L"\n";
		aux.Format(L"TCPOptions='%d.%d.%d.%d'",TCP.uOption,TCP.uMss,TCP.uWndSize,TCP.uScale);//bug: bad order...
		string += aux;
	}
	if(pHWDeviceOempa->GetSWDeviceOEMPA()->GetOptionsFlash(flash))
	{
		if(!string.GetLength() || string[string.GetLength()-1]!=L'\n')
			string += L"\n";
		aux.Format(L"Flash => manufacturer=0x%.2lX, memType=0x%.2lX, memCapacity=0x%.2lX",flash.uManufacturer,flash.uMemoryType,flash.uMemoryCapacity);
		string += aux;
	}
	iCount = pHWDeviceOempa->GetSWDeviceOEMPA()->GetPasscodeCount();
	if(iCount)
	{
		if(!string.GetLength() || string[string.GetLength()-1]!=L'\n')
			string += L"\n";
		aux.Format(L"Passcode=");
		string += aux;
		for(int iIndex=0;iIndex<iCount;iIndex++)
		{
			if(!pHWDeviceOempa->GetSWDeviceOEMPA()->GetPasscode(iIndex,dwAux))
				continue;
			aux.Format(L"%lu",dwAux);
			string += aux;
			if(iIndex<iCount-1)
				aux.Format(L":");
			else
				aux.Format(L"\n");
			string += aux;
		}
	}
	if(pHWDeviceOempa->GetSWDeviceOEMPA()->GetOptionsFlash(flash))
	{
		if(!string.GetLength() || string[string.GetLength()-1]!=L'\n')
			string += L"\n";
		aux.Format(L"TimeOffsetCorrectionSupported=%d",pHWDeviceOempa->GetSWDeviceOEMPA()->IsTimeOffsetSupported()?1:0);
		string += aux;
	}
	string += L"\n";
}

void CDlgHW::OnBnClickedButtonStatus()
{
	CString strFileFullPathName=L"*.txt";
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"Status");
	strFileFullPathName += L"*.txt";
	CFileDialog filedialog(FALSE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	CString string,strFile,strType;
	WORD wTemperature[(1+g_iOEMPABoardCountMax)*2][g_iOEMPARXBoardTemperatureSensorCountMax];//1 for the ComBoard.
	ULONGLONG ullTimeStampSW,aullTimeStampHW[8];
	bool bRet=true,bIO;
	DWORD dwDigitalInput;
	int iSensorCountMax,iMasterDeviceId,iDeviceCount,iBoardCount,iSensorCount,iBoardCount2;
	structRoot root;
	structCycle cycle;
	WORD wFWId=0,wFWId2=0,wFWId3=0,wFWId4=0;
	CString strHWOptions;
	CString strStatusExtra,strAux1,strAux2,strAux3,strAux4,strFWId,strFWUSB3;
	CString strMaxTemperature;
	CString strFMCSupported;
	int iLength;
	CHWDeviceOempa *pHWDeviceOempa;
	bool bFullMatrixCapture2,bFullMatrixCapture3,bFullMatrixCapture4;

	if(!m_pHWDeviceOEMPA)
		return;
	if(filedialog.DoModal()!=IDOK)
		return;
	strFile = filedialog.GetPathName();
	if(!m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetTemperatureCount(iBoardCount,iSensorCountMax))
		return;
	if(m_pHWDeviceOEMPA && !m_pHWDeviceOEMPA->GetMatchedDeviceOempa())
	{
		StatusExtra(m_pHWDeviceOEMPA,strStatusExtra);
	}else{
		pHWDeviceOempa = m_pHWDeviceOEMPA->GetMatched256DeviceOempa(2);
		StatusExtra(m_pHWDeviceOEMPA,strAux1);
		StatusExtra(m_pHWDeviceOEMPA->GetMatchedDeviceOempa(),strAux2);
		if(m_pHWDeviceOEMPA && !pHWDeviceOempa)
		{
			strStatusExtra = L"\n----MASTER DEVICE----";
			strStatusExtra += strAux1;
			strStatusExtra += L"----SLAVE DEVICE----";
			strStatusExtra += strAux2;
		}else{
			StatusExtra(pHWDeviceOempa,strAux3);
			StatusExtra(pHWDeviceOempa->GetMatchedDeviceOempa(),strAux4);
			strStatusExtra = L"\n----MASTER DEVICE----";
			strStatusExtra += strAux1;
			strStatusExtra += L"----SLAVE 1 DEVICE----";
			strStatusExtra += strAux2;
			strStatusExtra += L"----SLAVE 2 DEVICE----";
			strStatusExtra += strAux3;
			strStatusExtra += L"----SLAVE 3 DEVICE----";
			strStatusExtra += strAux4;
		}
	}
	if(!m_pHWDeviceOEMPA->LockDevice(eAcqOff))
	{
		AfxMessageBox(L"Communication error!");
		return;
	}
	for(int iIndexBoard=0;iIndexBoard<iBoardCount;iIndexBoard++)
	{
		if(!m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetTemperatureSensorCount(iIndexBoard,iSensorCount))
		{
			bRet = false;
			break;
		}
		for(int iIndexSensor=0;iIndexSensor<iSensorCount;iIndexSensor++)
		{
			if(!m_pHWDeviceOEMPA->GetTemperatureSensor(iIndexBoard,iIndexSensor,&wTemperature[iIndexBoard][iIndexSensor]))
				bRet = false;
		}
	}
	if(!OEMPA_ReadHWWriteFileCycle(CDlgHW::m_pHWDeviceOEMPA,0,g_iOEMPADACCountMax,&root,&cycle))
		bRet = false;
	iMasterDeviceId = m_pHWDeviceOEMPA->GetMasterDeviceId();
	iDeviceCount = 1;
	if(iMasterDeviceId<0)
		iMasterDeviceId = m_pHWDeviceOEMPA->GetDeviceId();
		iDeviceCount += m_pHWDeviceOEMPA->GetSlaveCount(iMasterDeviceId);
	if(iDeviceCount<=8)
	{
		if(!m_pHWDeviceOEMPA->GetTimeStamp(iDeviceCount,ullTimeStampSW,aullTimeStampHW))
			bRet = false;
	}else{
		for(int iDevice=0;iDevice<iDeviceCount;iDevice++)
			aullTimeStampHW[iDevice] = 0;
	}
	dwDigitalInput = 0;
	if(m_pHWDeviceOEMPA->GetDigitalInput(&dwDigitalInput))
		bIO = true;
	else
		bIO = false;
	if(!m_pHWDeviceOEMPA->GetFWId(&wFWId))//request FWId from the device (communication data exchange).
		wFWId = 0;
	if(m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsOEMMCEnabled())
		strType = L"OEMMC";
	else
		strType = L"OEMPA";
	if(!m_pHWDeviceOEMPA->UnlockDevice())
	{
		AfxMessageBox(L"Communication error!");
		return;
	}
	wFWId = (WORD)m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetFirmwareId();
	if(m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetFWUSB3Version())
		strFWUSB3.Format(L"FirmwareUSB3=0x%.4lX\n", m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetFWUSB3Version());
	else
		strFWUSB3 = L"";
	int iCycleCountMax = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetCycleCountMax();
	bool bFullMatrixCapture = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCapture();
	bool bTpacqAvailable = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsTpacquisitionAvailable();
	bool bWTSWAvailable = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsWTSWAvailable();
	bool bEncoderDecimal = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsEncoderDecimal();
	bool bMultiHWChannelSupported = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsMultiHWChannelSupported();
	bool bMultiHWChannelEnabled = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsMultiHWChannelEnabled();
	double dPulserPowerMax = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetPulserPowerMax();
	double dPulserPowerCurrent = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetPulserPowerCurrent();
	DWORD dwSWBaseAddress = m_pHWDeviceOEMPA->GetSWBaseAddress();
	if(m_iIOMaxTemperature>0)
		strMaxTemperature.Format(L"\nIOMaxTemperature=%d Celsius",m_iIOMaxTemperature);
	else
		strMaxTemperature = L"";
	if(m_pHWDeviceOEMPA->GetMatchedDeviceOempa())
	{
		wFWId2 = (WORD)m_pHWDeviceOEMPA->GetMatchedDeviceOempa()->GetSWDeviceOEMPA()->GetFirmwareId();
		if(!m_pHWDeviceOEMPA->GetMatched256DeviceOempa(2))
			strFWId.Format(L"1=0x%.4X\nFirmwareId2=0x%.4X",wFWId,wFWId2);
		else{
			wFWId3 = (WORD)m_pHWDeviceOEMPA->GetMatched256DeviceOempa(2)->GetSWDeviceOEMPA()->GetFirmwareId();
			if(!m_pHWDeviceOEMPA->GetMatched256DeviceOempa(3))
				strFWId.Format(L"1=0x%.4X\nFirmwareId2=0x%.4X\nFirmwareId3=0x%.4X",wFWId,wFWId2,wFWId3);
			else{
				wFWId4 = (WORD)m_pHWDeviceOEMPA->GetMatched256DeviceOempa(3)->GetSWDeviceOEMPA()->GetFirmwareId();
				strFWId.Format(L"1=0x%.4X\nFirmwareId2=0x%.4X\nFirmwareId3=0x%.4X\nFirmwareId4=0x%.4X",wFWId,wFWId2,wFWId3,wFWId4);
			}
		}
		if(m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCaptureReadWrite())
		{
			if(!m_pHWDeviceOEMPA->GetMatchedDeviceOempa())
				strFMCSupported.Format(L"FullMatrixCaptureSupported=1\nFullMatrixCaptureEnabled=%d\n",bFullMatrixCapture?1:0);
			else{
				bFullMatrixCapture2 = m_pHWDeviceOEMPA->GetMatchedDeviceOempa()->GetSWDeviceOEMPA()->IsFullMatrixCaptureReadWrite();
				strFMCSupported.Format(L"FullMatrixCaptureSupported1=1\nFullMatrixCaptureSupported2=%d\nFullMatrixCaptureEnabled=%d\n",bFullMatrixCapture2?1:0,bFullMatrixCapture?1:0);
			}
			if(m_pHWDeviceOEMPA->GetMatched256DeviceOempa(2))
			{
				bFullMatrixCapture2 = bFullMatrixCapture3 = bFullMatrixCapture4 = false;
				if(m_pHWDeviceOEMPA->GetMatchedDeviceOempa())
					bFullMatrixCapture2 = m_pHWDeviceOEMPA->GetMatchedDeviceOempa()->GetSWDeviceOEMPA()->IsFullMatrixCaptureReadWrite();
				if(m_pHWDeviceOEMPA->GetMatched256DeviceOempa(2))
					bFullMatrixCapture3 = m_pHWDeviceOEMPA->GetMatched256DeviceOempa(2)->GetSWDeviceOEMPA()->IsFullMatrixCaptureReadWrite();
				if(m_pHWDeviceOEMPA->GetMatched256DeviceOempa(3))
					bFullMatrixCapture4 = m_pHWDeviceOEMPA->GetMatched256DeviceOempa(3)->GetSWDeviceOEMPA()->IsFullMatrixCaptureReadWrite();
				strFMCSupported.Format(L"FullMatrixCaptureSupported1=1\nFullMatrixCaptureSupported2=%d\nFullMatrixCaptureSupported3=%d\nFullMatrixCaptureSupported4=%d\nFullMatrixCaptureEnabled=%d\n",
					bFullMatrixCapture2?1:0,bFullMatrixCapture3?1:0,bFullMatrixCapture4?1:0,bFullMatrixCapture?1:0);
			}
		}else
			strFMCSupported.Format(L"FullMatrixCapture=%d\n",bFullMatrixCapture?1:0);
	}else{
		strFWId.Format(L"=0x%.4X",wFWId);
		if(m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCaptureReadWrite())
		{
			if(!m_pHWDeviceOEMPA->GetMatchedDeviceOempa())
				strFMCSupported.Format(L"FullMatrixCaptureSupported=1\nFullMatrixCaptureEnabled=%d\n",bFullMatrixCapture?1:0);
			else{
				if(m_pHWDeviceOEMPA->GetMatchedDeviceOempa()->GetSWDeviceOEMPA()->IsFullMatrixCaptureReadWrite())
					strFMCSupported.Format(L"FullMatrixCaptureSupported1=1\nFullMatrixCaptureSupported2=1\nFullMatrixCaptureEnabled=%d\n",bFullMatrixCapture?1:0);
				else
					strFMCSupported.Format(L"FullMatrixCaptureSupported1=1\nFullMatrixCaptureSupported2=0\nFullMatrixCaptureEnabled=%d\n",bFullMatrixCapture?1:0);
			}
		}else
			strFMCSupported.Format(L"FullMatrixCapture=%d\n",bFullMatrixCapture?1:0);
	}
	strHWOptions.Format(L"SystemType=%d:%d (%s)\nIOBoard=%d\nMaxCycles=%d\n%sTPACQ SW=%d\nWT SW=%d\nEncoderDecimal=%d\nMultiChannelSupported=%d\nMultiChannelEnabled=%d\nFirmwareId%s\n%sPulserPowerMax=%.1lf W\nPulserPowerCurrent=%.2lf W%s",
			m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetApertureCountMax(),
			m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetElementCountMax(),
			strType,bIO?1:0,iCycleCountMax,
			strFMCSupported,
			bTpacqAvailable?1:0,bWTSWAvailable?1:0,bEncoderDecimal?1:0,
			bMultiHWChannelSupported?1:0,
			bMultiHWChannelEnabled?1:0,
			strFWId,strFWUSB3,
			dPulserPowerMax,dPulserPowerCurrent,strMaxTemperature
			);
	if(bRet)
	{
		try{
			CStdioFile file(strFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
			file.SeekToEnd();
			file.WriteString(L"=====SOFTWARE====\n");
			string.Format(L"DriverVersion=%s%c\n",OEMPA_GetVersion(),KIT_VERSION_LETTER);
			file.WriteString(string);
			file.WriteString(L"=====HARDWARE====\n");
			file.WriteString(strHWOptions);
			if(!strStatusExtra.IsEmpty())
				file.WriteString(strStatusExtra);
			string.Format(L"\nBoard\tSensor\tTemperature\n");
			file.WriteString(string);
			for(int iIndexBoard=0;iIndexBoard<iBoardCount;iIndexBoard++)
			{
				if(!m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetTemperatureSensorCount(iIndexBoard,iSensorCount))
				{
					bRet = false;
					break;
				}
				for(int iIndexSensor=0;iIndexSensor<iSensorCount;iIndexSensor++)
				{
					string.Format(L"%d\t%d\t%d °C\n",iIndexBoard,iIndexSensor,(int)wTemperature[iIndexBoard][iIndexSensor]);
					file.WriteString(string);
				}
			}
			if(m_pHWDeviceOEMPA->GetMatched256DeviceOempa(2))
			{
				CHWDeviceOEMPA *pHWDeviceOEMPA=m_pHWDeviceOEMPA->GetMatched256DeviceOempa(2)->GetHWDeviceOEMPA();
				if(!pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetTemperatureCount(iBoardCount2,iSensorCountMax))
					return;
				for(int iIndexBoard=0;iIndexBoard<iBoardCount2;iIndexBoard++)
				{
					if(!pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetTemperatureSensorCount(iIndexBoard,iSensorCount))
					{
						bRet = false;
						break;
					}
					for(int iIndexSensor=0;iIndexSensor<iSensorCount;iIndexSensor++)
					{
						string.Format(L"%d\t%d\t%d °C\n",iIndexBoard+iBoardCount,iIndexSensor,(int)wTemperature[iIndexBoard][iIndexSensor]);
						file.WriteString(string);
					}
				}
			}
			for(int iDevice=0;iDevice<iDeviceCount;iDevice++)
			{
				string.Format(L"\n");
				file.WriteString(string);
				string.Format(L"TimeStamp (%d) SW=0x%.16I64X HW=0x%.16I64X\n",iDevice,ullTimeStampSW,aullTimeStampHW[iDevice]);
				file.WriteString(string);
				if(ullTimeStampSW>aullTimeStampHW[iDevice])
					string.Format(L"TimeStamp Delta=+0x%.16I64X\n",ullTimeStampSW-aullTimeStampHW[iDevice]);
				else
					string.Format(L"TimeStamp Delta=-0x%.16I64X\n",aullTimeStampHW[iDevice]-ullTimeStampSW);
				file.WriteString(string);
			}
			if(bIO)
			{
				string.Format(L"DigitalInputs = 0x%.2lX\n",dwDigitalInput);
				file.WriteString(string);
			}
			if(g_pView)
				g_pView->DumpFileData(file);
			file.Close();
		}catch(CFileException *e){
			e->ReportError();
			e->Delete();
		}
		try{
			CStdioFile file(strFile, CFile::modeRead | CFile::typeText);
			iLength = (int)file.GetLength();
			file.Close();
		}catch(CFileException *e){
			iLength = 0;
			e->ReportError();
			e->Delete();
		}
		if(iLength<1024*1024)
			EditFile(strFile.GetBuffer(),false);
		UTKernel_SetFolderDataFiles(L"Status",strFile.GetBuffer());
		if(iLength>=1024*1024)
			AfxMessageBox(L"The file has been created!");
	}
}

void CDlgHW::UpdateDialog()
{
	if(CDlgHW::m_pDlgHW)
		CDlgHW::m_pDlgHW->UpdateDialog_();
}

void NotifyHWDlg(BOOL bWizardEnable)
{
	CDlgHW::m_bCheckWizardEnable = bWizardEnable;
	CDlgHW::UpdateDialog();
}

void CDlgHW::OnMove(int x, int y)
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;
	wchar_t pTitle[MAX_PATH];
	CDialog::OnMove(x, y);

	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),MAX_PATH,pTitle,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,m_strLayoutTitle,iResHorz,iResVert,wndPlacement,0);
}

void CDlgHW::OnEnKillfocusIPAddress()
{
	UpdateData();
	m_IPAddress.GetAddress(m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
	UpdateDriver();
	UpdateDialog();
}

void CDlgHW::OnEnKillfocusEditPort()
{
	UpdateData();
	UpdateDriver();
	UpdateDialog();
}

void CDlgHW::OnBnClickedButtonConfig()
{
	UpdateData();
	if(!m_pHWDeviceOEMPA)
		return;
	g_RunToolbox.Run(GetSafeHwnd(),CDlgHW::CallbackThreadToolbox);
}

void CDlgHW::OnBnClickedCheckDisplayMax()
{
	UpdateData();
}

void CDlgHW::UpdateComboChannel()
{
	int iIndex;
	DWORD dwData;
	CString string;

	m_ComboChannel.ResetContent();
	m_ComboChannel.AddString(L"All");
	for(int iChannelProbe=0;iChannelProbe<m_iChannelProbeCount;iChannelProbe++)
	{
		for(int iChannelScan=0;iChannelScan<m_iChannelScanCount[iChannelProbe];iChannelScan++)
		{
			if(!m_bFullMatrixCapture)
			{
				if(!m_iChannelCycleCount[iChannelProbe][iChannelScan])
					break;
				string.Format(L"%d-%d",iChannelProbe,iChannelScan);
				iIndex = m_ComboChannel.AddString(string);
				dwData = MAKELONG(iChannelProbe,iChannelScan);
			}else{
				string.Format(L"%d",iChannelScan);
				iIndex = m_ComboChannel.AddString(string);
				dwData = iChannelScan;
			}
			m_ComboChannel.SetItemData(iIndex,dwData);
		}
	}
	m_iComboChannel = 0;
	m_iChannelProbe = m_iChannelScan = -1;
	UpdateDialog();
}

bool CDlgHW::ProcessMultiChannel(structAcqChannelInfo &chInfo,structAcqInfoEx &acqInfo,int &iCycle)
{
	bool bRet=true;

	if(!(acqInfo.bMultiChannel?true:false) && !acqInfo.bFullMatrixCapture)
		return true;
	EnterCriticalSection(&m_ChannelCriticalSection);
	if(acqInfo.bFullMatrixCapture)
	{
		if(!m_bCheckFMCCalibration)
		{
			if(!m_iComboChannel)
				//1.1.5.3l iCycle = iCycle*(int)(m_iFMCElementStop-m_iFMCElementStart+1)+(int)acqInfo.lFMCElementIndex-m_iFMCElementStart;
				iCycle = iCycle*m_iFMCElementCount+((int)acqInfo.lFMCElementIndex-m_iFMCElementStart)/m_iFMCElementStep;
			else if(iCycle==m_iComboChannel-1)
				//1.1.5.3l iCycle = (int)acqInfo.lFMCElementIndex-m_iFMCElementStart;//cycle for the current channel.
				iCycle = ((int)acqInfo.lFMCElementIndex-m_iFMCElementStart)/m_iFMCElementStep;//cycle for the current channel.
			else
				bRet = false;
		}else{
			if(iCycle!=(int)acqInfo.lFMCElementIndex)
				bRet = false;
		}
	}else
	if((acqInfo.bMultiChannel?true:false) && (m_iChannelCount>0) && (m_iComboChannel>0))
	{
		if(((int)chInfo.lChannelProbe==m_iChannelProbe) && ((int)chInfo.lChannelScan==m_iChannelScan))
			iCycle = (int)chInfo.lChannelCycle;//cycle for the current channel.
		else
			bRet = false;
	}
	LeaveCriticalSection(&m_ChannelCriticalSection);
	return bRet;
}

void CDlgHW::OnCbnSelchangeComboChannel()
{
	int iComboChannel=m_iComboChannel;
	int iCycleCount;
	DWORD dwData;

	UpdateData();
	EnterCriticalSection(&m_ChannelCriticalSection);
	if(m_iComboChannel>m_iChannelCount)
	{
		m_iComboChannel = iComboChannel;
		LeaveCriticalSection(&m_ChannelCriticalSection);
		UpdateData(FALSE);
		return;
	}
	dwData = (DWORD)m_ComboChannel.GetItemData(m_iComboChannel);
	if(!m_bFullMatrixCapture)
	{
		m_iChannelProbe = (int)LOWORD(dwData);
		m_iChannelScan = (int)HIWORD(dwData);
		if(m_iComboChannel>0)
			iCycleCount = m_iChannelCycleCount[m_iChannelProbe][m_iChannelScan];
		else
			iCycleCount = m_iCycleCount;
		if(g_pView)
			g_pView->UpdateCycleCount(iCycleCount);
	}else{
		m_iChannelProbe = 0;
		m_iChannelScan = (int)dwData;
		if(m_iComboChannel>0)
			iCycleCount = m_iChannelCycleCount[0][0];
		else
			iCycleCount = m_iChannelCycleCount[0][0]*m_iChannelCount;
		if(g_pView)
			g_pView->UpdateCycleCount(iCycleCount);
		UpdateCycle();
		UpdateGain();
	}
	LeaveCriticalSection(&m_ChannelCriticalSection);
	UpdateDialog();
}

void CDlgHW::OnBnClickedButtonMulti()
{
	CDlgHwLink dlg(CDlgHW::m_pHWDeviceOEMPA,this);

	dlg.DoModal();
}

void CDlgHW::OnBnClickedButtonOutput()
{
	bool bIOBoard = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsIOBoardEnabled();
	CString strFileName,string;
	bool bExist=false;
	bool bRet=true;
	int iDigitalOutput[g_iOEMPAMappingDigitalOutputMax];
	wchar_t wcKey[50];
	enumOEMPAMappingDigitalOutput eMappingDigitalOutput;

	if(!bIOBoard)
		return;
	strFileName = g_wcFolderData;
	strFileName += L"DigitalOutputs.txt";
	if(FileReadInteger(L"DigitalOutputs",L"Output_0",strFileName.GetBuffer(),iDigitalOutput[0],false,0,1))
		bExist = true;
	if(!bExist)
	{
		try{
			CStdioFile file(strFileName,CFile::typeText | CFile::modeWrite | CFile::modeCreate);
			file.WriteString(L"# DIGITAL OUTPUTS\n");
			file.WriteString(L"\n");
			file.WriteString(L"[DigitalOutputs]\n");
			for(int iDigitalOutputIndex=0;iDigitalOutputIndex<g_iOEMPAMappingDigitalOutputMax;iDigitalOutputIndex++)
			{
				string.Format(L"Output_%d=0\n",iDigitalOutputIndex);
				file.WriteString(string);
			}
			file.Close();
		}catch(CFileException *e){
			e->ReportError();
			e->Delete();
		}
	}
	EditFile(strFileName.GetBuffer(),true);
	CheckCarriageReturnNotepad(strFileName,strFileName);
	for(int iDigitalOutputIndex=0;iDigitalOutputIndex<g_iOEMPAMappingDigitalOutputMax;iDigitalOutputIndex++)
	{
		string.Format(L"Output_%d",iDigitalOutputIndex);
		wcscpy_s(wcKey,50,string);
		if(!FileReadInteger(L"DigitalOutputs",wcKey,strFileName.GetBuffer(),iDigitalOutput[iDigitalOutputIndex],true,0,1))
		{
			bRet = false;
			break;
		}
	}
	if(bRet && m_pHWDeviceOEMPA->LockDevice())
	{
		for(int iDigitalOutputIndex=0;iDigitalOutputIndex<g_iOEMPAMappingDigitalOutputMax;iDigitalOutputIndex++)
		{
			if(!iDigitalOutput[iDigitalOutputIndex])
				eMappingDigitalOutput = eOEMPALow;
			else
				eMappingDigitalOutput = eOEMPAHigh;
			if(!m_pHWDeviceOEMPA->SetDigitalOutput(iDigitalOutputIndex,eMappingDigitalOutput))
			{
				bRet = false;
				break;
			}
		}
		if(!m_pHWDeviceOEMPA->UnlockDevice())
			bRet = false;
	}else
		bRet = false;
	if(!bRet)
		AfxMessageBox(L"Error!");
}

void CDlgHW::OnBnClickedButtonCalibration()
{
	CDlgCalibration dlg(m_pHWDeviceOEMPA);

	dlg.DoModal();
}

void CDlgHW::OnBnClickedCheckTest()
{
	UpdateData();
	if(m_bCheckTest)
		m_bAcquisitionWholeStream = true;
	else
		m_bAcquisitionWholeStream = false;
	InterlockedExchange((LONG volatile *)&g_uiDebugUSB3SizeBufferCurrent,(LONG)0);
	InterlockedExchange((LONG volatile *)&g_uiDebugUSB3SizeAllBufferCurrent,(LONG)0);
	InterlockedExchange((LONG volatile *)&g_uiDebugUSB3SizeAllBufferMax,(LONG)0);
	if(m_bAcquisitionWholeStream)
		m_pHWDeviceOEMPA->SetAcquisitionStream(CDlgHW::sProcessAcquisitionStream);
	else
		m_pHWDeviceOEMPA->SetAcquisitionStream(NULL);
}

void CDlgHW::OnBnClickedButtonResetUsb3()
{
	if(!g_pUsb3Device)
		g_pUsb3Device = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetUSB3Device();
	if(g_pUsb3Device)
		g_pUsb3Device->Reset();
}

void CDlgHW::OnBnClickedButtonFileOempaLoad2()
{
	CString strFileFullPathName=L"*.txt";
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"OEMPA");
	strFileFullPathName += L"*.txt";
	CFileDialog filedialog(TRUE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	CString string;
	bool bRet;

	if(!m_pHWDeviceOEMPA)
		return;
	if(filedialog.DoModal()!=IDOK)
		return;
	string = filedialog.GetFileExt();
	if((string!=L"txt") && (string!=L"bin"))
		return;
	CWaitCursor wait;
	string = filedialog.GetPathName();
	bRet = OEMPA_ReadFileWriteHW(m_pHWDeviceOEMPA,string.GetBuffer());
	if(!bRet)
	{
		//1.1.5.2g
	}
	UpdateData();
	m_iComboCycle = 0;
	UpdateData(FALSE);
	UpdateCycle();
	UpdateGain();
	UpdateDialog_();
	OnCbnSelchangeComboDisplayCycle();
	UTKernel_SetFolderDataFiles(L"OEMPA",string.GetBuffer());
}

unsigned long HexToLong2(CString &string,int &iByteSize,bool &bError,bool *pbWarning=NULL);
unsigned long HexToLong2(CString &string,int &iByteSize,bool &bError,bool *pbWarning)
{
	wchar_t cAux;
	CString strAux,strAux2;
	unsigned long ulAux,ulValue=0;
	int iAux;

	bError = false;
	if(pbWarning)
		*pbWarning = false;
	strAux = string.Left(2);
	if(strAux!=(L"0x"))
	{
		bError = true;
		return 0;
	}
	iByteSize = 0;
	for(int i=0;i<8/*2*iByteSize*/ && string.GetLength();i++)
	{
		iByteSize = (i+1)/2;
		strAux = string.Right(1);
		iAux = string.GetLength();
		strAux2 = string.Left(iAux-1);
		string = strAux2;
		cAux = strAux.GetAt(0);
		if(('0'<=cAux)&&(cAux<='9'))
		{
			ulAux = cAux-'0';
		}else
		if(('a'<=cAux)&&(cAux<='f'))
		{
			ulAux = cAux-'a'+10;
		}else
		if(('A'<=cAux)&&(cAux<='F'))
		{
			ulAux = cAux-'A'+10;
		}else{
			ulAux = 0;
			if(pbWarning)
				*pbWarning = true;
			break;//bError = true;
		}
		ulValue += ulAux<<(i*4);
	}
	switch(iByteSize)
	{
	case 1: ulValue = ulValue & 0x000000ff;break;
	case 2: ulValue = ulValue & 0x0000ffff;break;
	case 3: ulValue = ulValue & 0x00ffffff;break;
	case 4: break;
	}
	return ulValue;
}

unsigned long HexToLong(CString &string,int iByteSize,bool &bError,bool *pbWarning=NULL);
unsigned long HexToLong(CString &string,int iByteSize,bool &bError,bool *pbWarning)
{
	wchar_t cAux;
	CString strAux,strAux2;
	unsigned long ulAux,ulValue=0;
	int iAux;

	bError = false;
	if(pbWarning)
		*pbWarning = false;
	strAux = string.Left(2);
	if(strAux!=(L"0x"))
	{
		bError = true;
		return 0;
	}
	for(int i=0;i<8/*2*iByteSize*/ && string.GetLength();i++)
	{
		strAux = string.Right(1);
		iAux = string.GetLength();
		strAux2 = string.Left(iAux-1);
		string = strAux2;
		cAux = strAux.GetAt(0);
		if(('0'<=cAux)&&(cAux<='9'))
		{
			ulAux = cAux-'0';
		}else
		if(('a'<=cAux)&&(cAux<='f'))
		{
			ulAux = cAux-'a'+10;
		}else
		if(('A'<=cAux)&&(cAux<='F'))
		{
			ulAux = cAux-'A'+10;
		}else{
			ulAux = 0;
			if(pbWarning && (i>=2*iByteSize))
				*pbWarning = true;
			break;//bError = true;
		}
		ulValue += ulAux<<(i*4);
	}
	switch(iByteSize)
	{
	case 1: ulValue = ulValue & 0x000000ff;break;
	case 2: ulValue = ulValue & 0x0000ffff;break;
	case 3: ulValue = ulValue & 0x00ffffff;break;
	case 4: break;
	}
	return ulValue;
}

bool CDlgHW::Loadfile(CString strFileName)
{
	CString strMsg;
	bool bRet=true;
	CString string,strAux;
	DWORD dwAddress,dwData;
	CString myString;
	bool bError;
	bool bLockDevice=false;
	int iDataSize2,iFind,iDataCount;

	iDataCount = 0;
	try{
		CStdioFile file(strFileName,CFile::typeText | CFile::modeRead);
		file.SeekToBegin();
		while(file.ReadString(string))
		{
			if(string==L"STOP")
				break;
			if(string.GetLength()<10)
				continue;//1.1.5.2g
			myString = string.Left(10);
			dwAddress = HexToLong(myString,4,bError);
			if(bError)
				continue;
			iFind = string.Find(L"0x",2);
			if(iFind<0)
				continue;
			myString = string.Right(string.GetLength()-iFind);
			dwData = HexToLong2(myString,iDataSize2,bError);
			if(bError)
				continue;
			if(!bLockDevice)
			{
				if(!m_pHWDeviceOEMPA->LockDevice())
				{
					bRet = false;
					break;
				}
				bLockDevice = true;
			}
			//Don't use this very low level function, they is reserved for advanced user.
			if(!m_pHWDeviceOEMPA->WriteHW(dwAddress,dwData,iDataSize2))
			{
				bRet = false;
			}else
				iDataCount++;
		}
		file.Close();
	}catch(CFileException *e){
		e->ReportError();
		e->Delete();
		bRet = false;
	}
	if(bLockDevice && !m_pHWDeviceOEMPA->UnlockDevice())
		bRet = false;
	if(bRet)
	{
		string.Format(L"%d data sent.",iDataCount);
		AfxMessageBox(string);
	}
	return bRet;
}
