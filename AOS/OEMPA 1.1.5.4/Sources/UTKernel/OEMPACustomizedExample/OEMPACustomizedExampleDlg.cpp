
// OEMPACustomizedExampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OEMPACustomizedExample.h"
#include "OEMPACustomizedExampleDlg.h"

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


bool StringWriteArrayInteger(CString &string,int *piData,int iCount,wchar_t *pUnit)
{
	wchar_t wcAux[MAX_PATH];
	wchar_t wcLine[MAX_PATH];
	int iLength;

	//single line
	iLength = 0;
	wcLine[0] = 0;
	for(int iIndex=0;iIndex<iCount;iIndex++)
	{
		if(iIndex==iCount-1)
		{
			if(!pUnit || (pUnit && !wcslen(pUnit)))
				swprintf(wcAux,MAX_PATH,L"%d",piData[iIndex]);
			else
				swprintf(wcAux,MAX_PATH,L"%d %s",piData[iIndex],pUnit);
		}else
			swprintf(wcAux,MAX_PATH,L"%d;",piData[iIndex]);
		iLength += (int)wcslen(wcAux);
		if(iLength>=MAX_PATH)
		{
			wcLine[MAX_PATH-2] = L'X';
			wcLine[MAX_PATH-1] = 0;
			string = wcLine;
			return false;
		}
		wcscat_s(wcLine,MAX_PATH,wcAux);
	}
	string = wcLine;
	return true;
}

bool StringWriteArrayFloat(CString &string,float fUnitFactor,float *pfData,int iCount,wchar_t *pUnit)
{
	wchar_t wcAux[MAX_PATH];
	wchar_t wcLine[MAX_PATH];
	int iLength;

	//single line
	iLength = 0;
	wcLine[0] = 0;
	for(int iIndex=0;iIndex<iCount;iIndex++)
	{
		if(iIndex==iCount-1)
		{
			if(!pUnit || (pUnit && !wcslen(pUnit)))
				swprintf(wcAux,MAX_PATH,L"%.1f",pfData[iIndex]*fUnitFactor);
			else
				swprintf(wcAux,MAX_PATH,L"%.1f %s",pfData[iIndex]*fUnitFactor,pUnit);
		}else
			swprintf(wcAux,MAX_PATH,L"%.1f;",pfData[iIndex]*fUnitFactor);
		iLength += (int)wcslen(wcAux);
		if(iLength>=MAX_PATH)
		{
			wcLine[MAX_PATH-2] = L'X';
			wcLine[MAX_PATH-1] = 0;
			string = wcLine;
			return false;
		}
		wcscat_s(wcLine,MAX_PATH,wcAux);
	}
	string = wcLine;
	return true;
}

wchar_t *myReadArrayLongLong(bool bDecimal,wchar_t *pValue,long long &llValue)
{
	//wchar_t separator=L';';
	int iIndex;
	bool bFound=false;
	wchar_t *pStop=pValue;
	wchar_t c;
	long long powerFactor = 1;
	long long tmp;
	bool bSign=false;
	
	pStop = wcsstr(pValue,L"0x");
	if(!pStop)
	{
		if(!bDecimal)
			return NULL;
		pStop = pValue;
		if(*pStop==L'-')
		{
			bSign = true;
			pStop++;
		}
	}else{
		bDecimal = false;
		pStop += 2;
	}
	iIndex = 0;
	llValue = 0;
	while(1)
	{
		c = *pStop;//pValue[i];
		if(!c)
			break;
		if ( (c >= L'0') && (c <= L'9'))
		{
			tmp = c - L'0';
			pStop++;
		}else if ( bDecimal && (c >= L'A') && (c <= L'F'))
		{
			tmp = c - L'A' + 10;
			pStop++;
		}else if ( bDecimal && (c >= L'a') && (c <= L'f'))
		{
			tmp = c - L'a' + 10;
			pStop++;
		}else{
			if(c==L';')
				break;
			return NULL;
		}
		if(!bDecimal)
			llValue = llValue<<4;
		else
			llValue *= 10;
		llValue += tmp;
	}
	if(bSign)
		llValue = -llValue;
	return pStop;
}

wchar_t *myReadArrayFloat(wchar_t *pValue,float &fValue)
{
	double dValue;
	wchar_t *pStop;

	dValue = _wtof(pValue);
	fValue = (float)dValue;
	pStop = wcsstr(pValue,L";");
	if(pStop)
		pStop++;
	return pStop;
}

bool myAddArrayInteger(int iData,int iCountMax,int *piData,int iCountExpected,int &iIndex)
{
	if(iIndex>=iCountMax)
		return false;
	if(iCountExpected && (iIndex>=iCountExpected))
		return false;
	piData[iIndex++] = iData;
	return true;
}

bool myAddArrayFloat(float fData,int iCountMax,float *pfData,int iCountExpected,int &iIndex)
{
	if(iIndex>=iCountMax)
		return false;
	if(iCountExpected && (iIndex>=iCountExpected))
		return false;
	pfData[iIndex++] = fData;
	return true;
}

bool FileReadArrayInteger(CString &string,int iCountMax,int *piData,int &iCountExpected)
{
	wchar_t line[MAX_PATH];
	int iData,iCount;
	wchar_t *pValue;
	long long llValue;

	iCount = 0;
	wcscpy_s(line,MAX_PATH,string);//GetPrivateProfileString(pSection,pKeyBigData,g_pDefaultString2,line,MAX_PATH,pFileName);
	pValue = myReadArrayLongLong(true,line,llValue);
	iData = (int)llValue;
	if(*pValue)
		pValue++;
	while(myAddArrayInteger(iData,iCountMax,piData,iCountExpected,iCount) && *pValue)
	{
		pValue = myReadArrayLongLong(true,pValue,llValue);
		if(*pValue)
			pValue++;
		iData = (int)llValue;
	}
	iCountExpected = iCount;
	for(int iElement=0;iElement<iCount;iElement++)
	{
		for(int iElement2=0;iElement2<iCount;iElement2++)
		{
			if((iElement!=iElement2) && (piData[iElement]==piData[iElement2]))
				return false;//error
		}
	}
	return true;
}

bool FileReadArrayFloat(CString &string,int iCountMax,float fUnitFactor,float *pfData,int &iCountExpected)
{
	wchar_t line[MAX_PATH];
	int iCount;
	wchar_t *pValue;
	float fData;

	iCount = 0;
	wcscpy_s(line,MAX_PATH,string);
	pValue = myReadArrayFloat(line,fData);
	fData *= fUnitFactor;
	while(myAddArrayFloat(fData,iCountMax,pfData,iCountExpected,iCount) && pValue)
	{
		pValue = myReadArrayFloat(pValue,fData);
		fData *= fUnitFactor;
	}
	iCountExpected = iCount;
	return true;
}

// COEMPACustomizedExampleDlg dialog




COEMPACustomizedExampleDlg::COEMPACustomizedExampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COEMPACustomizedExampleDlg::IDD, pParent)
	, m_bCheckConnect(FALSE)
	, m_iRadioReception(FALSE)
	, m_strStaticWizard1(_T(""))
	, m_strStaticWizard2(_T(""))
	, m_strStaticDisplay(_T(""))
	, m_strStaticAperture(_T(""))
	, m_strStaticApodisation(_T(""))
	, m_strEditAperture(_T(""))
	, m_strEditApodisation(_T(""))
	, m_strEditFile(_T(""))
	, m_fEditBeamCorrection(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strStaticBeamCorrection = _T("");
	m_eWizardStep = eWizardDisconnected;
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
	m_hThread = 0;
	m_dwThreadId = 0;
	m_eLoadStep = eReadFileWriteHW_Enter;
	InitializeCriticalSection(&m_CriticalSection);
	m_pRoot = NULL;
	m_pCycle = NULL;
	m_pEmission = NULL;
	m_pReception = NULL;
	m_pHWDeviceOEMPA = NULL;
}

void COEMPACustomizedExampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IPAddress);
	DDX_Control(pDX, IDC_CHECK_CONNECT, m_CheckConnect);
	DDX_Check(pDX, IDC_CHECK_CONNECT, m_bCheckConnect);
	DDX_Control(pDX, IDC_BUTTON_WIZARD_1, m_ButtonWizardPrevious);
	DDX_Control(pDX, IDC_BUTTON_WIZARD_2, m_ButtonWizardNext);
	DDX_Control(pDX, IDC_RADIO_RECEPTION, m_RadioReception);
	DDX_Control(pDX, IDC_RADIO_EMISSION, m_RadioEmission);
	DDX_Radio(pDX, IDC_RADIO_RECEPTION, m_iRadioReception);
	DDX_Text(pDX, IDC_STATIC_WIZARD_1, m_strStaticWizard1);
	DDX_Text(pDX, IDC_STATIC_WIZARD_2, m_strStaticWizard2);
	DDX_Text(pDX, IDC_STATIC_COMMENT, m_strStaticDisplay);
	DDX_Text(pDX, IDC_STATIC_APERTURE, m_strStaticAperture);
	DDX_Text(pDX, IDC_STATIC_APODISATION, m_strStaticApodisation);
	DDX_Control(pDX, IDC_EDIT_APERTURE, m_EditAperture);
	DDX_Control(pDX, IDC_EDIT_APODISATION, m_EditApodisation);
	DDX_Control(pDX, IDC_EDIT_BEAM_CORRECTION, m_EditBeamCorrection);
	DDX_Text(pDX, IDC_STATIC_BEAM_CORRECTION, m_strStaticBeamCorrection);
	DDX_Text(pDX, IDC_EDIT_APERTURE, m_strEditAperture);
	DDX_Text(pDX, IDC_EDIT_APODISATION, m_strEditApodisation);
	DDX_Control(pDX, IDC_EDIT_FILE, m_EditFile);
	DDX_Text(pDX, IDC_EDIT_FILE, m_strEditFile);
	DDX_Control(pDX, IDC_STATIC_DATA, m_StaticData);
	DDX_Text(pDX, IDC_EDIT_BEAM_CORRECTION, m_fEditBeamCorrection);
}

BEGIN_MESSAGE_MAP(COEMPACustomizedExampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_CONNECT, &COEMPACustomizedExampleDlg::OnClickedCheckConnect)
	ON_BN_CLICKED(IDC_BUTTON_WIZARD_1, &COEMPACustomizedExampleDlg::OnClickedButtonWizardPrevious)
	ON_BN_CLICKED(IDC_BUTTON_WIZARD_2, &COEMPACustomizedExampleDlg::OnClickedButtonWizardNext)
	ON_COMMAND(IDC_RADIO_EMISSION, &COEMPACustomizedExampleDlg::OnRadioEmission)
	ON_BN_CLICKED(IDC_RADIO_RECEPTION, &COEMPACustomizedExampleDlg::OnClickedRadioReception)
	ON_BN_CLICKED(IDCANCEL, &COEMPACustomizedExampleDlg::OnBnClickedCancel)
	ON_EN_KILLFOCUS(IDC_EDIT_APERTURE, &COEMPACustomizedExampleDlg::OnEnKillfocusEditAperture)
	ON_EN_KILLFOCUS(IDC_EDIT_APODISATION, &COEMPACustomizedExampleDlg::OnEnKillfocusEditApodisation)
	ON_EN_KILLFOCUS(IDC_EDIT_BEAM_CORRECTION, &COEMPACustomizedExampleDlg::OnEnKillfocusEditBeamCorrection)
	ON_BN_CLICKED(IDOK, &COEMPACustomizedExampleDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// COEMPACustomizedExampleDlg message handlers

BOOL COEMPACustomizedExampleDlg::OnInitDialog()
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
	UpdateDialog();
	m_pHWDeviceOEMPA = &m_HWDeviceOEMPA;//new CHWDeviceOEMPA();
	m_pHWDeviceOEMPA->SetAcquisitionParameter(this);//registration of the user data
	OEMPA_SetCallbackCustomizedDriverAPI(COEMPACustomizedExampleDlg::CallbackCustomizedAPI);
	m_IPAddress.SetAddress(m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
	SetTimer(true);

	if(IsWow64())
		string.Format(L"OEMPACustomizedExample (x86, PID %d)",GetCurrentProcessId());
	else
		string.Format(L"OEMPACustomizedExample (PID %d)",GetCurrentProcessId());
	SetWindowText(string);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COEMPACustomizedExampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void COEMPACustomizedExampleDlg::OnPaint()
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
HCURSOR COEMPACustomizedExampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool COEMPACustomizedExampleDlg::SetTimer(bool bEnable)
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

void COEMPACustomizedExampleDlg::OnTimer(UINT_PTR nIDEvent)
{
	enumStepCustomizedAPI eLoadStep;
	enumWizardStep eWizardStep;
	bool bRet;

	CDialog::OnTimer(nIDEvent);
	EnterCriticalSection(&m_CriticalSection);
	eLoadStep = m_eLoadStep;
	LeaveCriticalSection(&m_CriticalSection);
	ASSERT(nIDEvent == g_kTimerID);
	if(m_bCheckConnect && !m_pHWDeviceOEMPA->GetSWDevice()->IsConnected())
	{
		m_bCheckConnect = FALSE;
		EnterCriticalSection(&m_CriticalSection);
		m_eWizardStep = eWizardDisconnected;
		LeaveCriticalSection(&m_CriticalSection);
		UpdateDialog();
		return;
	}
	EnterCriticalSection(&m_CriticalSection);
	eWizardStep = m_eWizardStep;
	LeaveCriticalSection(&m_CriticalSection);
	switch(eWizardStep)
	{
	case eWizardDisconnected:
		break;
	case eWizardConnected:
	case eWizardInput:
	case eWizardOutput:
	case eWizardFinish:
	case eWizardLoad:
		if(eLoadStep==eReadFileWriteHW_Leave)
		{
			SetTimer(false);
			EnterCriticalSection(&m_CriticalSection);
			m_eWizardStep = eWizardConnected;
			bRet = m_bReadFileWriteHW;
			m_eLoadStep = eReadFileWriteHW_Enter;
			LeaveCriticalSection(&m_CriticalSection);
			UpdateDialog();
			SetTimer(true);
		}
		break;
	}
}

void COEMPACustomizedExampleDlg::UpdateDialog()
{
	enumWizardStep eWizardStep;

	EnterCriticalSection(&m_CriticalSection);
	eWizardStep = m_eWizardStep;
	LeaveCriticalSection(&m_CriticalSection);
	m_EditFile.EnableWindow(FALSE);
	switch(eWizardStep)
	{
	case eWizardDisconnected:
		m_IPAddress.EnableWindow(TRUE);
		m_ButtonWizardPrevious.ShowWindow(SW_HIDE);
		m_ButtonWizardNext.ShowWindow(SW_HIDE);
		m_strStaticWizard1 = L"";
		m_strStaticWizard2 = L"";
		m_strStaticDisplay = L"";
		m_EditFile.ShowWindow(SW_HIDE);
		m_StaticData.ShowWindow(SW_HIDE);
		m_RadioEmission.ShowWindow(SW_HIDE);
		m_RadioReception.ShowWindow(SW_HIDE);
		m_EditAperture.ShowWindow(SW_HIDE);
		m_EditApodisation.ShowWindow(SW_HIDE);
		m_EditBeamCorrection.ShowWindow(SW_HIDE);
		break;
	case eWizardConnected:
		m_IPAddress.EnableWindow(FALSE);
		m_ButtonWizardPrevious.ShowWindow(SW_HIDE);
		m_ButtonWizardNext.ShowWindow(SW_SHOW);
		m_ButtonWizardNext.SetWindowText(L">>");
		m_strStaticWizard1 = L"";
		m_strStaticWizard2 = L"\">>\" to load an OEMPA file in the hardware device.";
		m_strStaticDisplay = L"The file is not loaded immediately, the process is completed after a few steps...";
		m_EditFile.ShowWindow(SW_HIDE);
		m_StaticData.ShowWindow(SW_HIDE);
		m_RadioEmission.ShowWindow(SW_HIDE);
		m_RadioReception.ShowWindow(SW_HIDE);
		m_EditAperture.ShowWindow(SW_HIDE);
		m_EditApodisation.ShowWindow(SW_HIDE);
		m_EditBeamCorrection.ShowWindow(SW_HIDE);
		m_iRadioReception = 0;
		break;
	case eWizardInput:
		m_IPAddress.EnableWindow(FALSE);
		m_ButtonWizardPrevious.ShowWindow(SW_SHOW);
		m_ButtonWizardPrevious.SetWindowText(L"<<");
		m_ButtonWizardNext.ShowWindow(SW_SHOW);
		m_ButtonWizardNext.SetWindowText(L">>");
		m_EditFile.ShowWindow(SW_SHOW);
		m_StaticData.ShowWindow(SW_SHOW);
		m_RadioEmission.ShowWindow(SW_SHOW);
		m_RadioReception.ShowWindow(SW_SHOW);
		m_EditAperture.ShowWindow(SW_SHOW);
		m_EditApodisation.ShowWindow(SW_SHOW);
		m_EditBeamCorrection.ShowWindow(!m_iRadioReception?SW_SHOW:SW_HIDE);
		m_strStaticWizard1 = L"\"<<\" to load another OEMPA file in the hardware device.";
		m_strStaticWizard2 = L"\">>\" to edit the data that will be loaded in the hardware device.";
		m_strStaticDisplay = L"Data in the file:";
		m_EditAperture.EnableWindow(FALSE);
		m_EditApodisation.EnableWindow(FALSE);
		m_EditBeamCorrection.EnableWindow(FALSE);
		break;
	case eWizardOutput:
		m_IPAddress.EnableWindow(FALSE);
		m_ButtonWizardPrevious.ShowWindow(SW_SHOW);
		m_ButtonWizardPrevious.SetWindowText(L"<<");
		m_ButtonWizardNext.ShowWindow(SW_SHOW);
		m_ButtonWizardNext.SetWindowText(L">>");
		m_strStaticWizard1 = L"\"<<\" to display the data in the file.";
		m_strStaticWizard2 = L"\">>\" to save the data in a new OEMPA file.";
		m_strStaticDisplay = L"Edit the data that will be loaded in the hardware device:";
		m_EditFile.ShowWindow(SW_SHOW);
		m_StaticData.ShowWindow(SW_SHOW);
		m_RadioEmission.ShowWindow(SW_SHOW);
		m_RadioReception.ShowWindow(SW_SHOW);
		m_EditAperture.ShowWindow(SW_SHOW);
		m_EditApodisation.ShowWindow(SW_SHOW);
		m_EditBeamCorrection.ShowWindow(!m_iRadioReception?SW_SHOW:SW_HIDE);
		m_EditAperture.EnableWindow(TRUE);
		m_EditApodisation.EnableWindow(TRUE);
		m_EditBeamCorrection.EnableWindow(TRUE);
		break;
	case eWizardFinish:
	case eWizardLoad:
		m_IPAddress.EnableWindow(FALSE);
		m_ButtonWizardPrevious.ShowWindow(SW_SHOW);
		m_ButtonWizardPrevious.SetWindowText(L"<<");
		m_ButtonWizardNext.ShowWindow(SW_SHOW);
		m_ButtonWizardNext.SetWindowText(L"Finish");
		m_strStaticWizard1 = L"\"<<\" to edit the data.";
		m_strStaticWizard2 = L"\"Finish\" to load the data in the hardware device.";
		m_strStaticDisplay = L"Data that will be loaded in the hardware device:";
		m_EditFile.ShowWindow(SW_SHOW);
		m_StaticData.ShowWindow(SW_SHOW);
		m_RadioEmission.ShowWindow(SW_SHOW);
		m_RadioReception.ShowWindow(SW_SHOW);
		m_EditAperture.ShowWindow(SW_SHOW);
		m_EditApodisation.ShowWindow(SW_SHOW);
		m_EditBeamCorrection.ShowWindow(!m_iRadioReception?SW_SHOW:SW_HIDE);
		m_EditAperture.EnableWindow(FALSE);
		m_EditApodisation.EnableWindow(FALSE);
		m_EditBeamCorrection.EnableWindow(FALSE);
		break;
	}
	UpdateEdit();
	UpdateData(FALSE);
}

void COEMPACustomizedExampleDlg::OnBnClickedOk()
{
	UpdateData();
	UpdateDataFormEdit();
	UpdateDialog();
}

void COEMPACustomizedExampleDlg::OnBnClickedCancel()
{
	CDialog::OnCancel();
}

void COEMPACustomizedExampleDlg::OnClickedCheckConnect()
{
	wchar_t pValue[40];
	CWaitCursor wait;

	UpdateData();
	if(m_bCheckConnect)
	{
		m_IPAddress.GetAddress(m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
		swprintf(pValue,40,L"%d.%d.%d.%d",m_cIP[0],m_cIP[1],m_cIP[2],m_cIP[3]);
		if(!m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->SetIPAddress(pValue) ||
			!m_pHWDeviceOEMPA->GetSWDevice()->SetConnectionState(eConnected,true))
			m_bCheckConnect = FALSE;
		else{
			EnterCriticalSection(&m_CriticalSection);
			m_eWizardStep = eWizardConnected;
			LeaveCriticalSection(&m_CriticalSection);
		}
	}else{
		if(!m_pHWDeviceOEMPA->GetSWDevice()->SetConnectionState(eDisconnected,true))
			m_bCheckConnect = TRUE;
		else{
			EnterCriticalSection(&m_CriticalSection);
			m_eWizardStep = eWizardDisconnected;
			LeaveCriticalSection(&m_CriticalSection);
		}
	}
	UpdateDialog();
}

void COEMPACustomizedExampleDlg::OnClickedButtonWizardPrevious()
{
	enumWizardStep eWizardStep;

	UpdateData();
	EnterCriticalSection(&m_CriticalSection);
	eWizardStep = m_eWizardStep;
	LeaveCriticalSection(&m_CriticalSection);
	switch(eWizardStep)
	{
	case eWizardDisconnected:	break;//error
	case eWizardConnected:		break;//error
	case eWizardInput:			eWizardStep = eWizardConnected;break;
	case eWizardOutput:			eWizardStep = eWizardInput;break;
	case eWizardFinish:			eWizardStep = eWizardOutput;break;
	case eWizardLoad:			eWizardStep = eWizardFinish;break;
	}
	EnterCriticalSection(&m_CriticalSection);
	m_eWizardStep = eWizardStep;
	LeaveCriticalSection(&m_CriticalSection);
	UpdateDialog();
}

void COEMPACustomizedExampleDlg::OnClickedButtonWizardNext()
{
	enumWizardStep eWizardStep;
	enumStepCustomizedAPI eLoadStep;
	bool bRet;

	UpdateData();
	EnterCriticalSection(&m_CriticalSection);
	eWizardStep = m_eWizardStep;
	LeaveCriticalSection(&m_CriticalSection);
	switch(eWizardStep)
	{
	case eWizardDisconnected:	break;//error
	case eWizardConnected:		if(Load())
								{
									eWizardStep = eWizardInput;
								}else{
									eWizardStep = eWizardConnected;
									TerminateThread(m_hThread,0);
									m_hThread = 0;
								}
								break;
	case eWizardInput:			eWizardStep = eWizardOutput;break;
	case eWizardOutput:			Save();
								eWizardStep = eWizardFinish;
								break;
	case eWizardFinish:			EnterCriticalSection(&m_CriticalSection);
								m_eWizardStep = eWizardLoad;
								eLoadStep = m_eLoadStep;
								LeaveCriticalSection(&m_CriticalSection);
								while(eLoadStep!=eReadFileWriteHW_Leave)
								{
									EnterCriticalSection(&m_CriticalSection);
									eLoadStep = m_eLoadStep;
									bRet = m_bReadFileWriteHW;
									LeaveCriticalSection(&m_CriticalSection);
									Sleep(1000);
									if(eLoadStep==eReadFileWriteHW_Leave)
										break;
								}
								m_hThread = 0;
								eWizardStep = eWizardConnected;
								LeaveCriticalSection(&m_CriticalSection);
								if(bRet)
									AfxMessageBox(L"Success: The data has been loaded!");
								else
									AfxMessageBox(L"Error: The data has not been loaded!");
								break;
	case eWizardLoad:			break;
	}
	EnterCriticalSection(&m_CriticalSection);
	m_eWizardStep = eWizardStep;
	LeaveCriticalSection(&m_CriticalSection);
	UpdateDialog();
}

bool COEMPACustomizedExampleDlg::Load()
{
	CString string,strFileFullPathName=L"*.txt";
	strFileFullPathName = L"*.txt";
	CFileDialog filedialog(TRUE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	enumStepCustomizedAPI eLoadStep;

	if (m_hThread)
	{
		TerminateThread(m_hThread,0);
		m_hThread = 0;
	}
	if(filedialog.DoModal()!=IDOK)
		return false;
	CWaitCursor wait;
	m_strEditFile = filedialog.GetPathName();
	UpdateData(FALSE);
	m_eLoadStep = eReadFileWriteHW_Enter;
	eLoadStep = eReadFileWriteHW_Enter;
	m_eWizardStep = eWizardInput;
	if (!m_hThread)
		m_hThread = CreateThread(NULL,0,ThreadLoad,this,0/*CREATE_SUSPENDED*/,&m_dwThreadId);
	if (!m_hThread)
		return false;
	while(eLoadStep==eReadFileWriteHW_Enter)
	{
		EnterCriticalSection(&m_CriticalSection);
		eLoadStep = m_eLoadStep;
		LeaveCriticalSection(&m_CriticalSection);
		if(eLoadStep==eWriteHW_Enter)
		{
			OnClickedRadioReception();
			return true;
		}
		Sleep(1000);
	}
	return false;
}

bool COEMPACustomizedExampleDlg::Save()
{
	CString strOutputFile,strFileFullPathName=L"*.txt";
	strFileFullPathName = L"*.txt";
	CFileDialog filedialog(FALSE,L"*.txt",strFileFullPathName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||",this,0);
	enumStepCustomizedAPI eLoadStep;
	bool bRet;

	EnterCriticalSection(&m_CriticalSection);
	eLoadStep = m_eLoadStep;
	if(eLoadStep != eWriteHW_Enter)
	{
		LeaveCriticalSection(&m_CriticalSection);
		return false;
	}
	if(filedialog.DoModal()!=IDOK)
	{
		LeaveCriticalSection(&m_CriticalSection);
		return false;
	}
	strOutputFile = filedialog.GetPathName();
	memcpy(m_pEmission,&m_EmissionOut,sizeof(m_EmissionOut));
	memcpy(m_pReception,&m_ReceptionOut,sizeof(m_ReceptionOut));
	m_pCycle->fBeamCorrection = m_fBeamCorrectionOut;
	DeleteFile(strOutputFile);
	bRet = OEMPA_WriteFileText(strOutputFile,m_pRoot,m_pCycle,m_pEmission,m_pReception);
	memcpy(m_pEmission,&m_EmissionIn,sizeof(m_EmissionIn));
	memcpy(m_pReception,&m_ReceptionIn,sizeof(m_ReceptionIn));
	m_pCycle->fBeamCorrection = m_fBeamCorrectionIn;
	LeaveCriticalSection(&m_CriticalSection);
	return bRet;
}

void COEMPACustomizedExampleDlg::UpdateElementList(CFocalLaw *pFocalLaw,int aiElement[g_iOEMPAApertureElementCountMax])
{
	DWORD dwElement;
	DWORD dwCurrent;

	int iIndex = 0;
	for(int iAp=0;iAp<g_iOEMPAApertureDWordCount;iAp++)
	{
		dwElement = pFocalLaw->adwElement[iAp];
		dwCurrent = 1;
		for(int iElem=0;iElem<32;iElem++)
		{
			if(dwElement & dwCurrent)
				aiElement[iIndex++] = iElem+32*iAp;
			dwCurrent = dwCurrent<<1;
		}
	}
}

void COEMPACustomizedExampleDlg::UpdateElementList(int aiElement[g_iOEMPAApertureElementCountMax],CFocalLaw *pFocalLaw)
{
	m_pHWDeviceOEMPA->SetAllElementEnable(false,pFocalLaw->adwElement);
	for(int iAp=0;iAp<pFocalLaw->iElementCount;iAp++)
	{
		m_pHWDeviceOEMPA->SetElementEnable(aiElement[iAp],true,pFocalLaw->adwElement);
	}
}

void COEMPACustomizedExampleDlg::UpdateEdit()
{
	CFocalLaw *pFocalLaw;
	int aiElement[g_iOEMPAApertureElementCountMax] = {0};
	enumWizardStep eWizardStep;

	EnterCriticalSection(&m_CriticalSection);
	eWizardStep = m_eWizardStep;
	LeaveCriticalSection(&m_CriticalSection);
	switch(eWizardStep)
	{
	case eWizardDisconnected:
	case eWizardConnected:		
		m_strStaticAperture = L"";
		m_strStaticApodisation = L"";
		m_strStaticBeamCorrection = L"";
		m_strEditAperture = L"";
		m_strEditApodisation = L"";
		return;//error
	case eWizardInput:			break;
	case eWizardOutput:			break;
	case eWizardFinish:			break;
	case eWizardLoad:			break;
	}
	if(!m_iRadioReception)
	{
		m_strStaticAperture = L"Element List";
		m_strStaticApodisation = L"Apodisation (dB)";
		m_strStaticBeamCorrection = L"Beam Correction (dB)";
		if(eWizardStep==eWizardInput)
		{
			pFocalLaw = &m_ReceptionIn;
			m_fEditBeamCorrection = m_fBeamCorrectionIn;
		}else{
			pFocalLaw = &m_ReceptionOut;
			m_fEditBeamCorrection = m_fBeamCorrectionOut;
		}
		UpdateElementList(pFocalLaw,aiElement);
		StringWriteArrayInteger(m_strEditAperture,aiElement,pFocalLaw->iElementCount,NULL);
		StringWriteArrayFloat(m_strEditApodisation,1.0f,pFocalLaw->afPrm,pFocalLaw->iElementCount,NULL);
	}else{
		m_strStaticAperture = L"Element List";
		m_strStaticApodisation = L"Pulse Width (us)";
		m_strStaticBeamCorrection = L"";
		if(eWizardStep==eWizardInput)
			pFocalLaw = &m_EmissionIn;
		else
			pFocalLaw = &m_EmissionOut;
		UpdateElementList(pFocalLaw,aiElement);
		StringWriteArrayInteger(m_strEditAperture,aiElement,pFocalLaw->iElementCount,NULL);
		StringWriteArrayFloat(m_strEditApodisation,1.0e6f,pFocalLaw->afPrm,pFocalLaw->iElementCount,NULL);
	}
}

void COEMPACustomizedExampleDlg::UpdateDataFormEdit()
{
	CFocalLaw *pFocalLaw;
	int aiElement[g_iOEMPAApertureElementCountMax] = {0};
	float afPrm[g_iOEMPAApertureElementCountMax];
	enumWizardStep eWizardStep;
	int iCountExpected;

	EnterCriticalSection(&m_CriticalSection);
	eWizardStep = m_eWizardStep;
	LeaveCriticalSection(&m_CriticalSection);
	switch(eWizardStep)
	{
	case eWizardDisconnected:
	case eWizardConnected:		
		return;//error
	case eWizardInput:			break;
	case eWizardOutput:			break;
	case eWizardFinish:			break;
	case eWizardLoad:			break;
	}
	if(!m_iRadioReception)
	{
		if(eWizardStep==eWizardInput)
		{
			pFocalLaw = &m_ReceptionIn;
			m_fBeamCorrectionIn = m_fEditBeamCorrection;
		}else{
			pFocalLaw = &m_ReceptionOut;
			m_fBeamCorrectionOut = m_fEditBeamCorrection;
		}
		iCountExpected = pFocalLaw->iElementCount;
		if(FileReadArrayInteger(m_strEditAperture,g_iOEMPAApertureElementCountMax,aiElement,iCountExpected))
			UpdateElementList(aiElement,pFocalLaw);
		else{
			if(iCountExpected != pFocalLaw->iElementCount)
				AfxMessageBox(L"Error: element count cannot be modified");
			else
				AfxMessageBox(L"Error: bad value");
		}
		iCountExpected = pFocalLaw->iElementCount;
		if(FileReadArrayFloat(m_strEditApodisation,pFocalLaw->iElementCount,1.0f,afPrm,iCountExpected))
			memcpy(pFocalLaw->afPrm,afPrm,sizeof(afPrm));
		else{
			if(iCountExpected != pFocalLaw->iElementCount)
				AfxMessageBox(L"Error: element count cannot be modified");
			else
				AfxMessageBox(L"Error: bad value");
		}
	}else{
		if(eWizardStep==eWizardInput)
			pFocalLaw = &m_EmissionIn;
		else
			pFocalLaw = &m_EmissionOut;
		iCountExpected = pFocalLaw->iElementCount;
		if(FileReadArrayInteger(m_strEditAperture,g_iOEMPAApertureElementCountMax,aiElement,iCountExpected))
			UpdateElementList(aiElement,pFocalLaw);
		else{
			if(iCountExpected != pFocalLaw->iElementCount)
				AfxMessageBox(L"Error: element count cannot be modified");
			else
				AfxMessageBox(L"Error: bad value");
		}
		iCountExpected = pFocalLaw->iElementCount;
		if(FileReadArrayFloat(m_strEditApodisation,pFocalLaw->iElementCount,1.0e-6f,afPrm,iCountExpected))
			memcpy(pFocalLaw->afPrm,afPrm,sizeof(afPrm));
		else{
			if(iCountExpected != pFocalLaw->iElementCount)
				AfxMessageBox(L"Error: element count cannot be modified");
			else
				AfxMessageBox(L"Error: bad value");
		}
	}
}

void COEMPACustomizedExampleDlg::OnRadioEmission()
{
	UpdateData();
	UpdateDialog();
}

void COEMPACustomizedExampleDlg::OnClickedRadioReception()
{
	UpdateData();
	UpdateDialog();
}

void COEMPACustomizedExampleDlg::OnEnKillfocusEditAperture()
{
	UpdateData();
	UpdateDataFormEdit();
	UpdateDialog();
}

void COEMPACustomizedExampleDlg::OnEnKillfocusEditApodisation()
{
	UpdateData();
	UpdateDataFormEdit();
	UpdateDialog();
}

void COEMPACustomizedExampleDlg::OnEnKillfocusEditBeamCorrection()
{
	UpdateData();
	UpdateDataFormEdit();
	UpdateDialog();
}

structCorrectionOEMPA* WINAPI COEMPACustomizedExampleDlg::CallbackCustomizedAPI(void *pAcquisitionParameter,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception)
{
	COEMPACustomizedExampleDlg *pDlg;
	enumWizardStep eWizardStep;
	CHWDeviceOEMPA *pOEMPA=(CHWDeviceOEMPA*)pAcquisitionParameter;

	pDlg = (COEMPACustomizedExampleDlg*)pOEMPA->GetAcquisitionParameter();
	if(!pDlg)
		return NULL;
	EnterCriticalSection(&pDlg->m_CriticalSection);
	eWizardStep = pDlg->m_eWizardStep;
	LeaveCriticalSection(&pDlg->m_CriticalSection);
	if(eWizardStep==eWizardDisconnected)
		return NULL;//not initialized yet.
	switch(eStep)
	{
	case eWriteHW_Enter:
		if(!pRoot->iCycleCount)
			eWizardStep = eWizardConnected;//abort
		else{
			pDlg->m_pRoot = pRoot;
			pDlg->m_pCycle = pCycle;
			pDlg->m_pEmission = pEmission;
			pDlg->m_pReception = pReception;
			pDlg->m_EmissionIn = *pEmission;
			//memcpy(&pDlg->m_EmissionIn,pEmission,sizeof(CFocalLaw));
			//memcpy(&pDlg->m_EmissionOut,pEmission,sizeof(CFocalLaw));
			//memcpy(&pDlg->m_ReceptionIn,pReception,sizeof(CFocalLaw));
			//memcpy(&pDlg->m_ReceptionOut,pReception,sizeof(CFocalLaw));
			pDlg->m_EmissionOut = *pEmission;
			pDlg->m_ReceptionIn = *pReception;
			pDlg->m_ReceptionOut = *pReception;
			pDlg->m_fBeamCorrectionIn = pCycle->fBeamCorrection;
			pDlg->m_fBeamCorrectionOut = pCycle->fBeamCorrection;
			EnterCriticalSection(&pDlg->m_CriticalSection);
			pDlg->m_eLoadStep = eStep;
			LeaveCriticalSection(&pDlg->m_CriticalSection);
			//wait user notification to go on
			while(1)
			{
				EnterCriticalSection(&pDlg->m_CriticalSection);
				pDlg->m_eLoadStep = eStep;
				eWizardStep = pDlg->m_eWizardStep;
				LeaveCriticalSection(&pDlg->m_CriticalSection);
				if(eWizardStep == eWizardLoad)
					break;//finish
				if(eWizardStep == eWizardDisconnected)
					break;//abort
				if(eWizardStep == eWizardConnected)
					break;//abort
				Sleep(100);
			}
		}
		break;
	}
	EnterCriticalSection(&pDlg->m_CriticalSection);
	pDlg->m_eLoadStep = eStep;
	LeaveCriticalSection(&pDlg->m_CriticalSection);
	return NULL;//we dont need correction in the current example.
}

DWORD WINAPI COEMPACustomizedExampleDlg::ThreadLoad(LPVOID pParam)
{
	COEMPACustomizedExampleDlg *pDlg;
	bool bRet;

	pDlg = (COEMPACustomizedExampleDlg*)pParam;
	if(!pDlg)
		return 0;
	bRet = OEMPA_ReadFileWriteHW(pDlg->m_pHWDeviceOEMPA,pDlg->m_strEditFile);
	EnterCriticalSection(&pDlg->m_CriticalSection);
	pDlg->m_bReadFileWriteHW = bRet;
	pDlg->m_eLoadStep = eReadFileWriteHW_Leave;
	pDlg->m_hThread = 0;
	LeaveCriticalSection(&pDlg->m_CriticalSection);
	return 0;
}
