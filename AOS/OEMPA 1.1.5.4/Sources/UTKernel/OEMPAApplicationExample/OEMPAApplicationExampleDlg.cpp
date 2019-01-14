
// OEMPAApplicationExampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OEMPAApplicationExample.h"
#include "OEMPAApplicationExampleDlg.h"
#include "DlgHwLink.h"
//#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


const wchar_t g_pDefault[]=L"abcdefghijklmnopqrstuvwxyz";
enumKeepAlive GetDefaultKeepAlive()
{
	enumKeepAlive eKeepAlive;
	wchar_t wcKeepAlive[20];

	eKeepAlive = eKeepAliveHardwareAndComputer;
	GetPrivateProfileString(L"UTKernel\\DeviceOEMPA",L"DefaultKeepAlive",g_pDefault,wcKeepAlive,20,UTKernel_GetConfigurationFile());
	if(!wcscmp(wcKeepAlive,g_pDefault))
	{
		WritePrivateProfileString(L"UTKernel\\DeviceOEMPA",L"DefaultKeepAlive",L"1",UTKernel_GetConfigurationFile());
	}else if(wcslen(wcKeepAlive)==1)
	{
		switch(wcKeepAlive[0])
		{
		case L'0':	eKeepAlive = eKeepAliveOff;break;
		case L'1':	eKeepAlive = eKeepAliveHardwareAndComputer;break;
		case L'2':	eKeepAlive = eKeepAliveHardwareOnly;break;
		case L'3':	eKeepAlive = eKeepAliveComputerOnly;break;
		}
	}
	return eKeepAlive;
}
void SetDefaultKeepAlive(enumKeepAlive eKeepAlive)
{
	wchar_t wcKeepAlive[20];

	switch(eKeepAlive)
	{
	case eKeepAliveOff:					wcscpy_s(wcKeepAlive,20,L"0");break;
	case eKeepAliveHardwareAndComputer:	wcscpy_s(wcKeepAlive,20,L"1");break;
	case eKeepAliveHardwareOnly:		wcscpy_s(wcKeepAlive,20,L"2");break;
	case eKeepAliveComputerOnly:		wcscpy_s(wcKeepAlive,20,L"3");break;
	}
	WritePrivateProfileString(L"UTKernel\\DeviceOEMPA",L"DefaultKeepAlive",wcKeepAlive,UTKernel_GetConfigurationFile());
}


// COEMPAApplicationExampleDlg dialog



COEMPAApplicationExampleDlg::COEMPAApplicationExampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COEMPAApplicationExampleDlg::IDD, pParent)
	, m_dEditReadGain(0)
	, m_iEditReadCycleCount(0)
	, m_dEditWriteGain(0)
	, m_dEditThroughputCscan(0)
	, m_dEditThroughputAscan(0)
	, m_dEditThroughputFrame(0)
	, m_bCheckPulserAutoEnable(FALSE)
	, m_dEditFileGain(0)
	, m_iEditFileCycleCount(0)
	, m_bCheckConnect(FALSE)
	, m_dEditEncoderAscan(0)
	, m_dEditEncoderCscan(0)
	, m_bCheckPulserEnable(FALSE)
	, m_dwEditResolutionX(0)
	, m_dEditAcquisitionStepX(0)
	, m_iComboSynchronisation(0)
	, m_dwEditCommunicationErrorCount(0)
	, m_strEditFilePath(_T(""))
	, m_dwEditLostCountAscan(0)
	, m_dwEditLostCountCscan(0)
	, m_dwEditPort(5001)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	InitializeCriticalSection(&m_CriticSection);
	m_dwTimerID = 0;
#ifndef _EVALUATION_
	m_cIP[0] = 192;
	m_cIP[1] = 168;
	m_cIP[2] = 1;
	m_cIP[3] = 11;
#else _EVALUATION_
	m_cIP[0] = 127;
	m_cIP[1] = 0;
	m_cIP[2] = 0;
	m_cIP[3] = 1;
#endif _EVALUATION_
	m_ullDataSizeAscan = 0;
	m_ullDataSizeCscan = 0;
	m_dwFrameId = 0;
	m_iFileCycleCount = -1;
	m_dFileGain = -1.0;
	m_strFilePath = L"";
	m_dEncoderAscan = 0.0;
	m_dEncoderCscan = 0.0;
	m_bEncoderAscan = false;
	m_bEncoderCscan = false;
	m_ullDisplayDataSizeAscan = -1;
	m_ullDisplayDataSizeCscan = -1;
	m_dwDisplayFrameId = -1;
	m_dwDisplayTickCount = -1;
	m_pEditBkBrush = new CBrush(RGB(255, 0, 0));
}

COEMPAApplicationExampleDlg::~COEMPAApplicationExampleDlg()
{
	DeleteCriticalSection(&m_CriticSection);
	if(m_pEditBkBrush)
		delete m_pEditBkBrush;
	m_pEditBkBrush = NULL;
}

void COEMPAApplicationExampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPAddress);
	DDX_Control(pDX, IDC_BUTTON_FILE2HW, m_ButtonFile2Hw);
	DDX_Control(pDX, IDC_CHECK_CONNECT, m_CheckConnect);
	DDX_Text(pDX, IDC_EDIT_RD_GAIN, m_dEditReadGain);
	DDX_Text(pDX, IDC_EDIT_RD_CYCLE_COUNT, m_iEditReadCycleCount);
	DDX_Control(pDX, IDC_BUTTON_RD_CYCLE_COUNT, m_ButtonReadCycleCount);
	DDX_Control(pDX, IDC_BUTTON_RD_GAIN, m_ButtonReadGain);
	DDX_Text(pDX, IDC_EDIT_WR_GAIN, m_dEditWriteGain);
	DDX_Control(pDX, IDC_BUTTON_WR_GAIN, m_ButtonWriteGain);
	DDX_Text(pDX, IDC_EDIT_THROUGHPUT_CSCAN, m_dEditThroughputCscan);
	DDX_Text(pDX, IDC_EDIT_THROUGHPUT_ASCAN, m_dEditThroughputAscan);
	DDX_Text(pDX, IDC_EDIT_THROUGHPUT_FRAME, m_dEditThroughputFrame);
	DDX_Control(pDX, IDC_CHECK_PULSER_AUTO_ENABLE, m_CheckPulserAutoEnable);
	DDX_Check(pDX, IDC_CHECK_PULSER_AUTO_ENABLE, m_bCheckPulserAutoEnable);
	DDX_Text(pDX, IDC_EDIT_FILE_GAIN, m_dEditFileGain);
	DDX_Text(pDX, IDC_EDIT_FILE_CYCLE_COUNT, m_iEditFileCycleCount);
	DDX_Check(pDX, IDC_CHECK_CONNECT, m_bCheckConnect);
	DDX_Control(pDX, IDC_BUTTON_HW2FILE, m_ButtonHw2File);
	DDX_Control(pDX, IDC_EDIT_WR_GAIN, m_EditWriteGain);
	DDX_Text(pDX, IDC_EDIT_ENCODER_ASCAN, m_dEditEncoderAscan);
	DDX_Text(pDX, IDC_EDIT_ENCODER_CSCAN, m_dEditEncoderCscan);
	DDX_Control(pDX, IDC_CHECK_ENABLE, m_CheckPulserEnable);
	DDX_Check(pDX, IDC_CHECK_ENABLE, m_bCheckPulserEnable);
	DDX_Control(pDX, IDC_EDIT_WR_RESOLUTION_X, m_EditResolutionX);
	DDX_Text(pDX, IDC_EDIT_WR_RESOLUTION_X, m_dwEditResolutionX);
	DDX_Control(pDX, IDC_EDIT_WR_STEP_X, m_EditAcquisitionStepX);
	DDX_Control(pDX, IDC_BUTTON_WR_RESOLUTION, m_ButtonWriteResolution);
	DDX_Control(pDX, IDC_BUTTON_WR_STEP, m_ButtonWriteAcquisitionStep);
	DDX_Text(pDX, IDC_EDIT_WR_STEP_X, m_dEditAcquisitionStepX);
	DDX_Control(pDX, IDC_COMBO_TRIGGER, m_ComboSynchronisation);
	DDX_CBIndex(pDX, IDC_COMBO_TRIGGER, m_iComboSynchronisation);
	DDX_Control(pDX, IDC_BUTTON_WR_ENCODER_RESET, m_ButtonResetEncoder);
	DDX_Text(pDX, IDC_EDIT_ERROR_COUNT, m_dwEditCommunicationErrorCount);
	DDX_Control(pDX, IDC_EDIT_ERROR_COUNT, m_EditCommunicationErrorCount);
	DDX_Text(pDX, IDC_EDIT_FILE_PATH, m_strEditFilePath);
	DDX_Text(pDX, IDC_EDIT_ASCAN_LOST_COUNT, m_dwEditLostCountAscan);
	DDX_Text(pDX, IDC_EDIT_CSCAN_LOST_COUNT, m_dwEditLostCountCscan);
	DDX_Control(pDX, IDC_EDIT_PORT, m_EditPort);
	DDX_Text(pDX, IDC_EDIT_PORT, m_dwEditPort);
	DDX_Control(pDX, IDC_BUTTON_HW_LINK, m_ButtonHwLink);
	DDX_Control(pDX, IDC_EDIT_ASCAN_LOST_COUNT, m_EditDataLostAscan);
	DDX_Control(pDX, IDC_EDIT_CSCAN_LOST_COUNT, m_EditDataLostCscan);
}

BEGIN_MESSAGE_MAP(COEMPAApplicationExampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_CONNECT, &COEMPAApplicationExampleDlg::OnBnClickedCheckConnect)
	ON_BN_CLICKED(IDC_BUTTON_RD_GAIN, &COEMPAApplicationExampleDlg::OnBnClickedButtonRdGain)
	ON_BN_CLICKED(IDC_BUTTON_RD_CYCLE_COUNT, &COEMPAApplicationExampleDlg::OnBnClickedButtonRdCycleCount)
	ON_BN_CLICKED(IDC_BUTTON_WR_GAIN, &COEMPAApplicationExampleDlg::OnBnClickedButtonWrGain)
	ON_BN_CLICKED(IDC_BUTTON_FILE2HW, &COEMPAApplicationExampleDlg::OnBnClickedButtonFile2hw)
	ON_BN_CLICKED(IDC_CHECK_PULSER_AUTO_ENABLE, &COEMPAApplicationExampleDlg::OnBnClickedCheckPulserAutoEnable)
	ON_BN_CLICKED(IDC_BUTTON_HW2FILE, &COEMPAApplicationExampleDlg::OnBnClickedButtonHw2file)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_ENABLE, &COEMPAApplicationExampleDlg::OnBnClickedCheckEnable)
	ON_BN_CLICKED(IDC_BUTTON_WR_RESOLUTION, &COEMPAApplicationExampleDlg::OnBnClickedButtonWrResolution)
	ON_BN_CLICKED(IDC_BUTTON_WR_STEP, &COEMPAApplicationExampleDlg::OnBnClickedButtonWrStep)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER, &COEMPAApplicationExampleDlg::OnCbnSelchangeComboSynchronisation)
	ON_BN_CLICKED(IDC_BUTTON_WR_ENCODER_RESET, &COEMPAApplicationExampleDlg::OnBnClickedButtonWrEncoderReset)
	ON_BN_CLICKED(IDOK, &COEMPAApplicationExampleDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &COEMPAApplicationExampleDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_HW_LINK, &COEMPAApplicationExampleDlg::OnBnClickedButtonHwLink)
END_MESSAGE_MAP()


// COEMPAApplicationExampleDlg message handlers

BOOL COEMPAApplicationExampleDlg::OnInitDialog()
{
	CString string;

	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_HWDeviceOEMPA.SetAcquisitionParameter(this);
	m_HWDeviceOEMPA.SetAcquisitionAscan_0x00010103(COEMPAApplicationExampleDlg::CallbackAcquisitionAscan);
	m_HWDeviceOEMPA.SetAcquisitionCscan_0x00010X02(COEMPAApplicationExampleDlg::CallbackAcquisitionCscan);
	OEMPA_SetCallbackCustomizedDriverAPI(COEMPAApplicationExampleDlg::CallbackCustomizedAPI);
	m_IPAddress.SetAddress(m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
	if(m_HWDeviceOEMPA.GetSWDevice()->GetUnlockEnableShot())
		m_bCheckPulserAutoEnable = TRUE;
	UpdateDialog();

	SetTimer(true);
	if(IsWow64())
		string.Format(L"OEMPAApplicationExample (x86, PID %d)",GetCurrentProcessId());
	else
		string.Format(L"OEMPAApplicationExample (PID %d)",GetCurrentProcessId());
	SetWindowText(string);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COEMPAApplicationExampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

HBRUSH COEMPAApplicationExampleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if(&m_EditDataLostAscan==pWnd)
	{
		if(m_dwEditLostCountAscan>0)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkColor(RGB(255, 0, 0));
			return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
		}
	}
	if(&m_EditDataLostCscan==pWnd)
	{
		if(m_dwEditLostCountCscan>0)
		{
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkColor(RGB(255, 0, 0));
			return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
		}
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void COEMPAApplicationExampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COEMPAApplicationExampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool COEMPAApplicationExampleDlg::SetTimer(bool bEnable)
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

void COEMPAApplicationExampleDlg::UpdateDialog()
{
	BOOL bEncoder=TRUE;

	m_IPAddress.EnableWindow(!m_bCheckConnect);
	m_EditPort.EnableWindow(!m_bCheckConnect);
	m_ButtonFile2Hw.EnableWindow(m_bCheckConnect);
	m_ButtonHw2File.EnableWindow(m_bCheckConnect);
	m_ButtonReadCycleCount.EnableWindow(m_bCheckConnect);
	m_ButtonReadGain.EnableWindow(m_bCheckConnect);
	m_ButtonWriteGain.EnableWindow(m_bCheckConnect);
	m_CheckPulserEnable.EnableWindow(m_bCheckConnect);
	m_EditWriteGain.EnableWindow(m_bCheckConnect);
	if(!m_bCheckConnect)
	{
		m_EditResolutionX.EnableWindow(FALSE);
		m_EditAcquisitionStepX.EnableWindow(FALSE);
		m_ButtonWriteResolution.EnableWindow(FALSE);
		m_ButtonWriteAcquisitionStep.EnableWindow(FALSE);
		m_dEditAcquisitionStepX = -1.0;
		m_dEditReadGain = -1.0;
		m_iEditReadCycleCount = -1;
		m_dEditWriteGain = -1.0;
		m_dEditThroughputCscan = -1.0;
		m_dEditThroughputAscan = -1.0;
		m_dEditThroughputFrame = -1.0;
		m_dEditFileGain = -1.0;
		m_iEditFileCycleCount = -1;
		m_strEditFilePath = L"";
		bEncoder = FALSE;
		m_dwEditResolutionX = 0;
		m_dEditAcquisitionStepX = -1.0;
	}else{
		if(!m_HWDeviceOEMPA.GetSWDeviceOEMPA()->IsIOBoardEnabled())
			bEncoder = FALSE;//no board IO, it is impossible to use encoder.
		if(!m_dwEditResolutionX)
			m_dwEditResolutionX = 1;
		if(m_dEditAcquisitionStepX<0.0)
			m_dEditAcquisitionStepX = 1.0;
	}
	m_ButtonResetEncoder.EnableWindow(bEncoder);
	m_EditResolutionX.EnableWindow(bEncoder);
	m_EditAcquisitionStepX.EnableWindow(bEncoder);
	m_ButtonWriteResolution.EnableWindow(bEncoder);
	m_ButtonWriteAcquisitionStep.EnableWindow(bEncoder);
	m_ComboSynchronisation.EnableWindow(bEncoder);
	if(!bEncoder)
		m_iComboSynchronisation = 0;//Internal
	UpdateData(FALSE);
}

void COEMPAApplicationExampleDlg::OnBnClickedCheckConnect()
{
	wchar_t pValue[40];

	UpdateData();
	if(m_bCheckConnect)
	{
		m_IPAddress.GetAddress(m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
		swprintf(pValue,40,L"%d.%d.%d.%d",m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
		m_dwEditCommunicationErrorCount = 0;//reset error count
		m_dwEditLostCountAscan = 0;//reset error count
		if(!m_HWDeviceOEMPA.GetSWDeviceOEMPA()->SetIPAddress(pValue) ||
			!m_HWDeviceOEMPA.GetSWDeviceOEMPA()->SetPort((unsigned short)m_dwEditPort) ||
			!m_HWDeviceOEMPA.GetSWDevice()->SetConnectionState(eConnected,true))
			m_bCheckConnect = FALSE;
		else
			m_dEditWriteGain = 0.0;
	}else{
		if(!m_HWDeviceOEMPA.GetSWDevice()->SetConnectionState(eDisconnected,true))
			m_bCheckConnect = TRUE;
		else{
			EnterCriticalSection(&m_CriticSection);
			m_ullDataSizeAscan = 0;
			m_ullDataSizeCscan = 0;
			m_ullDisplayDataSizeAscan = -1;
			m_ullDisplayDataSizeCscan = -1;
			m_dwDisplayFrameId = -1;
			m_dwDisplayTickCount = -1;
			m_bEncoderAscan = false;
			m_bEncoderCscan = false;
			LeaveCriticalSection(&m_CriticSection);
		}
	}
	UpdateDialog();
}

void COEMPAApplicationExampleDlg::OnBnClickedButtonHwLink()
{
	CDlgHwLink dlg(&m_HWDeviceOEMPA,this);

	dlg.DoModal();
}

void COEMPAApplicationExampleDlg::OnBnClickedButtonRdGain()
{
	bool bRet=true;
	
	UpdateData();
	m_dEditReadGain = -1.0;
	if(m_HWDeviceOEMPA.LockDevice())
	{
		if(!m_HWDeviceOEMPA.GetGainDigital(0,&m_dEditReadGain))
			bRet = false;
		if(!m_HWDeviceOEMPA.UnlockDevice())
			bRet = false;
	}else
		bRet = false;
	UpdateDialog();
	if(!bRet)
		AfxMessageBox(L"Communication ERROR!");
}

void COEMPAApplicationExampleDlg::OnBnClickedButtonRdCycleCount()
{
	bool bRet=true;
	//WORD wTemp=-1;
	
	UpdateData();
	m_iEditReadCycleCount = -1;
	if(m_HWDeviceOEMPA.LockDevice())
	{
		if(!m_HWDeviceOEMPA.GetCycleCount(&m_iEditReadCycleCount))
			bRet = false;
		//if(!m_HWDeviceOEMPA.GetTemperatureSensor(1,1,&wTemp))
		//	bRet = false;
		if(!m_HWDeviceOEMPA.UnlockDevice())
			bRet = false;
	}else
		bRet = false;
	UpdateDialog();
	if(!bRet)
		AfxMessageBox(L"Communication ERROR!");
	else{
	}
}

void COEMPAApplicationExampleDlg::OnBnClickedButtonWrGain()
{
	bool bRet=true;
	
	UpdateData();
	if(m_HWDeviceOEMPA.LockDevice())
	{
		if(!m_HWDeviceOEMPA.SetGainDigital(0,m_dEditWriteGain))
			bRet = false;
		if(!m_HWDeviceOEMPA.UnlockDevice())
			bRet = false;
	}else
		bRet = false;
	UpdateDialog();
	if(!bRet)
		AfxMessageBox(L"Communication ERROR!");
}

void COEMPAApplicationExampleDlg::OnBnClickedCheckEnable()
{
	bool bRet=true;
	enumAcquisitionState eAcqState;
	
	UpdateData();
	if(m_bCheckPulserEnable)
		eAcqState = eAcqOn;
	else
		eAcqState = eAcqOff;
	if(m_HWDeviceOEMPA.LockDevice())
	{
		if(!m_HWDeviceOEMPA.UnlockDevice(eAcqState))
			bRet = false;
	}else
		bRet = false;
	UpdateDialog();
	if(!bRet)
		AfxMessageBox(L"Communication ERROR!");
}

void COEMPAApplicationExampleDlg::OnBnClickedButtonFile2hw()
{
	CString string,strFileFullPathName=L"*.txt";
	strFileFullPathName = L"*.txt";
	CFileDialog filedialog(TRUE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	bool bRet;

	if(filedialog.DoModal()!=IDOK)
		return;
	CWaitCursor wait;
	string = filedialog.GetPathName();
	UpdateData();
	bRet = OEMPA_ReadFileWriteHW(&m_HWDeviceOEMPA,string);
	UpdateDialog();
}

void COEMPAApplicationExampleDlg::OnBnClickedCheckPulserAutoEnable()
{
	UpdateData();
	m_HWDeviceOEMPA.GetSWDevice()->SetUnlockEnableShot(m_bCheckPulserAutoEnable?true:false);
	UpdateDialog();
}

void COEMPAApplicationExampleDlg::OnBnClickedButtonHw2file()
{
	CString string,strFileFullPathName=L"*.txt";
	strFileFullPathName = L"*.txt";
	CFileDialog filedialog(FALSE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	bool bRet;

	if(filedialog.DoModal()!=IDOK)
		return;
	string = filedialog.GetPathName();
	UpdateData();
	bRet = OEMPA_ReadHWWriteFile(&m_HWDeviceOEMPA,g_iOEMPACycleCountMax,g_iOEMPAApertureElementCountMax,g_iOEMPAFocalCountMax,g_iOEMPADACCountMax,string.GetBuffer());
	UpdateDialog();
}

void COEMPAApplicationExampleDlg::OnCbnSelchangeComboSynchronisation()
{
	int iComboSynchronisation=m_iComboSynchronisation;
	enumOEMPATrigger eTrig;
	enumOEMPARequestIO eReqIO;
	bool bRet=true;
	
	UpdateData();
	switch(m_iComboSynchronisation)
	{
	case 2:
		eTrig = eOEMPAInternal;
		eReqIO = eOEMPAOnDigitalInputAndCycle;
		break;
	case 1:
		eTrig = eOEMPAEncoder;
		eReqIO = eOEMPAOnDigitalInputAndCycle;
		break;
	case 0:
		eTrig = eOEMPAInternal;
		eReqIO = eOEMPAOnDigitalInputOnly;
		break;
	}
	if(m_HWDeviceOEMPA.LockDevice())
	{
		if(!m_HWDeviceOEMPA.SetTriggerMode(eTrig))//trigger
			bRet = false;
		if(!m_HWDeviceOEMPA.SetRequestIO(eReqIO))//stream IO are also required to set the trigger properly.
			bRet = false;
		if(!m_HWDeviceOEMPA.SetEncoderWire1(0,eDigitalInput01))//it is required to define wire 1 of the encoder
			bRet = false;
		if(!m_HWDeviceOEMPA.SetEncoderWire2(0,eDigitalInput02))//it is required to define wire 2 of the encoder
			bRet = false;
		if(!m_HWDeviceOEMPA.UnlockDevice())
			bRet = false;
	}else
		bRet = false;
	if(!iComboSynchronisation && m_iComboSynchronisation)
	{
		OnBnClickedButtonWrResolution();
		OnBnClickedButtonWrStep();
	}
	UpdateDialog();
	if(!bRet)
		AfxMessageBox(L"Communication ERROR!");
}

void COEMPAApplicationExampleDlg::OnBnClickedButtonWrResolution()
{
	bool bRet=true;
	
	UpdateData();
	//Lock is not required with SW device.
	if(!m_HWDeviceOEMPA.GetSWDevice()->GetSWEncoder(0)->lSetResolution(m_dwEditResolutionX))
		bRet = false;
	UpdateDialog();
	if(!bRet)
		AfxMessageBox(L"Communication ERROR!");
}

void COEMPAApplicationExampleDlg::OnBnClickedButtonWrStep()
{
	bool bRet=true;
	double dAcqStep;
	
	UpdateData();
	if(m_HWDeviceOEMPA.LockDevice())
	{
		dAcqStep = m_dEditAcquisitionStepX/1000.0;//unit meter
		if(!m_HWDeviceOEMPA.SetTriggerEncoderStep(dAcqStep))
			bRet = false;
		m_dEditAcquisitionStepX = dAcqStep*1000.0;//unit millimeter
		if(!m_HWDeviceOEMPA.UnlockDevice())
			bRet = false;
	}else
		bRet = false;
	UpdateDialog();
	if(!bRet)
		AfxMessageBox(L"Communication ERROR!");
}

void COEMPAApplicationExampleDlg::OnBnClickedButtonWrEncoderReset()
{
	bool bRet=true;
	
	UpdateData();
	if(m_HWDeviceOEMPA.LockDevice())
	{
		if(!m_HWDeviceOEMPA.ResetEncoder(0))
			bRet = false;
		if(!m_HWDeviceOEMPA.SetTimeStamp(true,true))//STOP_NEXT_STANDBY
			bRet = false;
		if(!m_HWDeviceOEMPA.UnlockDevice())
			bRet = false;
	}else
		bRet = false;
	UpdateDialog();
	if(!bRet)
		AfxMessageBox(L"Communication ERROR!");
}

structCorrectionOEMPA* WINAPI COEMPAApplicationExampleDlg::CallbackCustomizedAPI(void *pAcquisitionParameter,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception)
{
	COEMPAApplicationExampleDlg *pDlg;
	CHWDeviceOEMPA *pOEMPA=(CHWDeviceOEMPA*)pAcquisitionParameter;

	pDlg = (COEMPAApplicationExampleDlg*)pOEMPA->GetAcquisitionParameter();
	if(!pDlg)
		return NULL;
	switch(eStep)
	{
	case eWriteHW_Enter:
		EnterCriticalSection(&pDlg->m_CriticSection);
		pDlg->m_strFilePath = pFileName;
		pDlg->m_iFileCycleCount = pRoot->iCycleCount;
		if(pCycle)//pCycle could be NULL.
			pDlg->m_dFileGain = pCycle->dGainDigital;
		LeaveCriticalSection(&pDlg->m_CriticSection);
		break;
	}
	return NULL;//we dont need correction in the current example.
}

UINT WINAPI COEMPAApplicationExampleDlg::CallbackAcquisitionAscan(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat)
{
	COEMPAApplicationExampleDlg *pDlg;

	pDlg = (COEMPAApplicationExampleDlg*)pAcquisitionParameter;
	if(!pDlg)
		return 0;
	EnterCriticalSection(&pDlg->m_CriticSection);
	pDlg->m_dwFrameId = pStreamHeader->frameId;
	pDlg->m_ullDataSizeAscan += pAscanHeader->dataSize*pAscanHeader->dataCount;
	pDlg->m_bEncoderAscan = true;
	pDlg->m_dEncoderAscan = acqInfo.dEncoder[0];
	LeaveCriticalSection(&pDlg->m_CriticSection);
	return 0;
}

UINT WINAPI COEMPAApplicationExampleDlg::CallbackAcquisitionCscan(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp, const structCscanAmpTof_0x0202 *pBufferAmpTof)
{
	COEMPAApplicationExampleDlg *pDlg;
	int iDataSize=0;

	pDlg = (COEMPAApplicationExampleDlg*)pAcquisitionParameter;
	if(!pDlg)
		return 0;
	EnterCriticalSection(&pDlg->m_CriticSection);
	pDlg->m_dwFrameId = pStreamHeader->frameId;
	iDataSize = pCscanHeader->count*sizeof(DWORD);
	if(pBufferAmp && !pBufferAmpTof)
		pDlg->m_ullDataSizeCscan += iDataSize;
	if(!pBufferAmp && pBufferAmpTof)
		pDlg->m_ullDataSizeCscan += iDataSize*2;
	pDlg->m_bEncoderCscan = true;
	pDlg->m_dEncoderCscan = acqInfo.dEncoder[0];
	LeaveCriticalSection(&pDlg->m_CriticSection);
	return 0;
}

void COEMPAApplicationExampleDlg::OnTimer(UINT_PTR nIDEvent)
{
	DWORD dwTickCount=GetTickCount();
	DWORD dwCommunicationErrorCount;
	DWORD dwLostCountAscan;
	DWORD dwLostCountCscan;

	ASSERT(nIDEvent == g_kTimerID);
	EnterCriticalSection(&m_CriticSection);
	UpdateData();
	if(!m_strFilePath.IsEmpty())
	{
		m_strEditFilePath = m_strFilePath;
		m_strFilePath = L"";
	}
	if(m_iFileCycleCount>=0)
	{
		m_iEditFileCycleCount = m_iFileCycleCount;
		m_iFileCycleCount = -1;
	}
	if(m_dFileGain>=0.0)
	{
		m_dEditFileGain = m_dFileGain;
		m_dFileGain = -1.0;
	}
	if(m_bEncoderAscan)
	{
		m_dEditEncoderAscan = m_dEncoderAscan*1000.0;
		m_bEncoderAscan = false;
	}
	if(m_bEncoderCscan)
	{
		m_dEditEncoderCscan = m_dEncoderCscan*1000.0;
		m_bEncoderCscan = false;
	}
	if(m_dwDisplayTickCount==-1)
	{
		m_dEditThroughputAscan = 0.0;
		m_dEditThroughputCscan = 0.0;
		m_dEditThroughputFrame = 0.0;
	}else{
		m_dEditThroughputAscan = (double)(m_ullDataSizeAscan-m_ullDisplayDataSizeAscan);
		m_dEditThroughputAscan *= 1000.0/(dwTickCount-m_dwDisplayTickCount);
		m_dEditThroughputAscan /= 1024*1024;
		m_dEditThroughputCscan = (double)(m_ullDataSizeCscan-m_ullDisplayDataSizeCscan);
		m_dEditThroughputCscan *= 1000.0/(dwTickCount-m_dwDisplayTickCount);
		m_dEditThroughputCscan /= 1024;
		m_dEditThroughputFrame = (double)(m_dwFrameId-m_dwDisplayFrameId);
		m_dEditThroughputFrame *= 1000.0/(dwTickCount-m_dwDisplayTickCount);
	}
	m_ullDisplayDataSizeAscan = m_ullDataSizeAscan;
	m_ullDisplayDataSizeCscan = m_ullDataSizeCscan;
	m_dwDisplayFrameId = m_dwFrameId;
	m_dwDisplayTickCount = dwTickCount;
	LeaveCriticalSection(&m_CriticSection);
	if(!m_HWDeviceOEMPA.GetSWDevice()->IsConnected())
		m_bCheckConnect = FALSE;
	if(!m_HWDeviceOEMPA.GetSWDevice()->IsPulserEnabled())
		m_bCheckPulserEnable = FALSE;
	else
		m_bCheckPulserEnable = TRUE;
	dwCommunicationErrorCount = m_HWDeviceOEMPA.GetSWDevice()->GetStreamError();
	if(m_dwEditCommunicationErrorCount!=dwCommunicationErrorCount)
		m_dwEditCommunicationErrorCount = dwCommunicationErrorCount;
	dwLostCountAscan = m_HWDeviceOEMPA.GetSWDevice()->GetLostCountAscan();
	if(m_dwEditLostCountAscan!=dwLostCountAscan)
		m_dwEditLostCountAscan = dwLostCountAscan;
	dwLostCountCscan = m_HWDeviceOEMPA.GetSWDevice()->GetLostCountCscan();
	if(m_dwEditLostCountCscan!=dwLostCountCscan)
		m_dwEditLostCountCscan = dwLostCountCscan;
	if(m_HWDeviceOEMPA.GetSWDevice()->GetUnlockEnableShot())
		m_bCheckPulserAutoEnable = TRUE;
	else
		m_bCheckPulserAutoEnable = FALSE;
	UpdateDialog();
	CDialog::OnTimer(nIDEvent);
}

void COEMPAApplicationExampleDlg::OnBnClickedOk()
{
	//CDialog::OnOK();
}

void COEMPAApplicationExampleDlg::OnBnClickedCancel()
{
	CDialog::OnCancel();
}


