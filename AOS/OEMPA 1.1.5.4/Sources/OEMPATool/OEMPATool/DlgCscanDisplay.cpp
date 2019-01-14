// DlgCscanDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "OEMPATool.h"
#include "DlgCscanDisplay.h"
#include "DlgHW.h"
#include "ChildView.h"
#include "limits.h"
#include "UTKernelLayoutFile.h"

extern CChildView *g_pView;

BOOL CDlgCscanDisplay::m_bCheckDisplay=FALSE;
int CDlgCscanDisplay::m_iRadioHw=0;
int CDlgCscanDisplay::m_iComboGate=0;
double CDlgCscanDisplay::m_dDigitizingPeriod=10.0e-9;
double CDlgCscanDisplay::m_dAscanStart[g_iOEMPACycleCountMax];
double CDlgCscanDisplay::m_dAscanRange[g_iOEMPACycleCountMax];
int CDlgCscanDisplay::m_iCycleCount=0;
CDlgCscanDisplay *CDlgCscanDisplay::m_pDlg=NULL;
bool CDlgCscanDisplay::m_bEnableCscan=false;
bool CDlgCscanDisplay::m_bEnableAscan=false;
bool CDlgCscanDisplay::m_bEnableCscanTof=false;
int CDlgCscanDisplay::m_iIndexLayoutFile=-1;

//setting to compute software cscan.
int CDlgCscanDisplay::m_iEditPointCount=200;
int CDlgCscanDisplay::m_aiPointCount[g_iOEMPACycleCountMax];
bool CDlgCscanDisplay::m_abGateEnableHW[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
bool CDlgCscanDisplay::m_abGateEnableSW[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
bool CDlgCscanDisplay::m_abGateSign[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
enumGateModeAmp CDlgCscanDisplay::m_aeGateModeAmp[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
double CDlgCscanDisplay::m_adGateStart[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
double CDlgCscanDisplay::m_adGateStop[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
int CDlgCscanDisplay::m_aiGateStart[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
int CDlgCscanDisplay::m_aiGateStop[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
int CDlgCscanDisplay::m_aiAcquisitionSWAmpCount[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
BYTE *CDlgCscanDisplay::m_apAcquisitionSWAmp[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
ULONGLONG CDlgCscanDisplay::m_ullUpdateSWCount=0;
LONGLONG CDlgCscanDisplay::m_llAcquisitionSWAmpSequenceIndex0=-1;
int CDlgCscanDisplay::m_aiAcquisitionSWAmpLastIndex[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
int CDlgCscanDisplay::m_aiAcquisitionHWAmpCount[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
BYTE *CDlgCscanDisplay::m_apAcquisitionHWAmp[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
ULONGLONG CDlgCscanDisplay::m_ullUpdateHWCount=0;
LONGLONG CDlgCscanDisplay::m_llAcquisitionHWAmpSequenceIndex0=-1;
int CDlgCscanDisplay::m_aiAcquisitionHWAmpLastIndex[g_iOEMPAGateCountMax][g_iOEMPACycleCountMax];
CRITICAL_SECTION CDlgCscanDisplay::m_CriticSection;
bool CDlgCscanDisplay::m_bEncoderEnable=false;
double CDlgCscanDisplay::m_dEncoderStep=0.0;
bool CDlgCscanDisplay::m_bCallbackCalled=false;

// CDlgCscanDisplay dialog

IMPLEMENT_DYNAMIC(CDlgCscanDisplay, CDialog)

CDlgCscanDisplay::CDlgCscanDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCscanDisplay::IDD, pParent)
	, m_bCheckEnableCscan(FALSE)
{
	m_iIndexLayoutFile = -1;
	InitializeCriticalSection(&m_CriticSection);
	Create(CDlgCscanDisplay::IDD,pParent);
	ShowWindow(SW_SHOW);
}

CDlgCscanDisplay::~CDlgCscanDisplay()
{
	DeleteCriticalSection(&m_CriticSection);
}

void CDlgCscanDisplay::PostNcDestroy()
{
	if(m_pDlg==this)
		m_pDlg = NULL;
	delete this;
}

void CDlgCscanDisplay::Display()
{
	if(!m_pDlg)
		m_pDlg = new CDlgCscanDisplay;
	if(!m_pDlg)
		return;
	if(m_pDlg->IsIconic())
		m_pDlg->ShowWindow(SW_RESTORE);
	m_pDlg->ShowWindow(SW_SHOW);
}

bool CDlgCscanDisplay::IsDisplayed()
{
	if(!m_pDlg)
		return false;
	return true;
}

void CDlgCscanDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_DISPLAY, m_bCheckDisplay);
	DDX_Radio(pDX, IDC_RADIO_HW, m_iRadioHw);
	DDX_CBIndex(pDX, IDC_COMBO_GATE, m_iComboGate);
	DDX_Text(pDX, IDC_EDIT_POINT_COUNT, m_iEditPointCount);
	DDX_Check(pDX, IDC_CHECK_ENABLE_CSCAN, m_bCheckEnableCscan);
	DDX_Control(pDX, IDC_CHECK_ENABLE_CSCAN, m_CheckEnableCscan);
	DDX_Control(pDX, IDC_CHECK_DISPLAY, m_CheckDisplay);
	DDX_Control(pDX, IDC_RADIO_HW, m_RadioHW);
	DDX_Control(pDX, IDC_RADIO_SW, m_RadioSW);
	DDX_Control(pDX, IDC_EDIT_POINT_COUNT, m_EditPointCount);
	DDX_Control(pDX, IDC_COMBO_GATE, m_ComboGate);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_ButtonReset);
}


BEGIN_MESSAGE_MAP(CDlgCscanDisplay, CDialog)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDCANCEL, &CDlgCscanDisplay::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY, &CDlgCscanDisplay::OnBnClickedCheckDisplay)
	ON_CBN_SELCHANGE(IDC_COMBO_GATE, &CDlgCscanDisplay::OnCbnSelchangeComboGate)
	ON_EN_KILLFOCUS(IDC_EDIT_POINT_COUNT, &CDlgCscanDisplay::OnEnKillfocusEditPointCount)
	ON_BN_CLICKED(IDC_RADIO_HW, &CDlgCscanDisplay::OnBnClickedRadioHw)
	ON_BN_CLICKED(IDC_RADIO_SW, &CDlgCscanDisplay::OnBnClickedRadioSw)
	ON_BN_CLICKED(IDC_CHECK_ENABLE_CSCAN, &CDlgCscanDisplay::OnBnClickedCheckEnableCscan)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgCscanDisplay::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CDlgCscanDisplay message handlers

BOOL CDlgCscanDisplay::OnInitDialog()
{
	CString string;
	int iResHorz,iResVert,iData;
	WINDOWPLACEMENT wndPlacement;
	wchar_t pTitle[MAX_PATH];
	bool bRegistered,bCreateNew;
	SYSTEMTIME sysTime;

	CDialog::OnInitDialog();
	UpdateData();
	EnterCriticalSection(&m_CriticSection);
	m_bEnableAscan = false;
	m_bEnableCscanTof = false;
	m_ullUpdateSWCount = 0;
	m_ullUpdateHWCount = 0;
	m_llAcquisitionSWAmpSequenceIndex0 = -1;
	m_llAcquisitionHWAmpSequenceIndex0 = -1;
	for(int iCycle=0;iCycle<g_iOEMPACycleCountMax;iCycle++)
	{
		m_dAscanStart[iCycle] = 0.0;
		m_dAscanRange[iCycle] = 0.0;
		m_aiPointCount[iCycle] = 0;
		for(int iGate=0;iGate<g_iOEMPAGateCountMax;iGate++)
		{
			m_aiAcquisitionSWAmpCount[iGate][iCycle] = 0;
			m_apAcquisitionSWAmp[iGate][iCycle] = NULL;
			m_aiAcquisitionSWAmpLastIndex[iGate][iCycle] = -1;
			m_aiAcquisitionHWAmpCount[iGate][iCycle] = 0;
			m_apAcquisitionHWAmp[iGate][iCycle] = NULL;
			m_aiAcquisitionHWAmpLastIndex[iGate][iCycle] = -1;
			m_abGateEnableHW[iGate][iCycle] = false;
			m_abGateEnableSW[iGate][iCycle] = false;
			m_abGateSign[iGate][iCycle] = false;
			m_aeGateModeAmp[iGate][iCycle] = eAmpMaximum;
			m_adGateStart[iGate][iCycle] = 0.0;
			m_adGateStop[iGate][iCycle] = 0.0;
			m_aiGateStart[iGate][iCycle] = 0;
			m_aiGateStop[iGate][iCycle] = 0;
		}
	}
	LeaveCriticalSection(&m_CriticSection);
	UpdateDialog();

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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

int iround(double x)
{
	int nx = (int)x;
	if(x>0)
	{
		if((x-nx)>0.5)
			return nx+1;
		else
			return nx;
	}else{
		if((x-nx)<-0.5)
			return nx-1;
		else
			return nx;
	}
}

void CDlgCscanDisplay::_UpdateDialog()
{
	if(m_pDlg)
		m_pDlg->UpdateDialog();
}
void CDlgCscanDisplay::UpdateDialog(bool bForce)
{
	BOOL bEnableCscan=m_bEnableCscan?TRUE:FALSE;

	bEnableCscan = bEnableCscan && m_bCheckEnableCscan;
	m_EditPointCount.EnableWindow(!m_bCheckEnableCscan);
	m_CheckDisplay.EnableWindow(bEnableCscan);
	m_RadioSW.EnableWindow(bEnableCscan && m_bEnableAscan && m_bCheckDisplay);
	m_RadioHW.EnableWindow(bEnableCscan && m_bCheckDisplay);
	if(!m_bEnableAscan && (m_iRadioHw!=0))
	{
		m_iRadioHw = 0;
		UpdateData(FALSE);
	}
	m_ComboGate.EnableWindow(bEnableCscan && m_bCheckDisplay);
	m_ButtonReset.EnableWindow(bEnableCscan && m_bCheckDisplay);
	if(bForce)
		UpdateData(FALSE);
}

void CDlgCscanDisplay::OnTimer()
{
	if(m_pDlg)
	if((m_bEnableCscan && !m_bCheckDisplay) || (!m_bEnableCscan && m_bCheckDisplay))
		m_pDlg->UpdateDialog(false);
}

structCorrectionOEMPA* WINAPI CDlgCscanDisplay::CallbackCustomizedAPI(CHWDeviceOEMPA *pOEMPA,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception)
{
	int iCycle=0;
	double dAux,dAscanStart,dAscanRange,dAscanStop;

	if(!m_pDlg)
	{
		if(eStep==eWriteHW_Leave)
			m_bCallbackCalled = false;
		return NULL;
	}else{
		if(eStep==eWriteHW_Leave)
			m_bCallbackCalled = true;
	}
	if(eStep==eWriteHW_Leave)
	{
		m_bEnableCscan = false;
		if(pCycle)
		{
			EnterCriticalSection(&m_CriticSection);
			m_bEnableAscan = pRoot->bAscanEnable;
			m_bEnableCscanTof = pRoot->bEnableCscanTof;
			m_dDigitizingPeriod = pOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod();
			m_iCycleCount = pRoot->iCycleCount;
			if(m_iCycleCount>2048)
				m_iCycleCount = 2048;
			m_llAcquisitionSWAmpSequenceIndex0 = -1;
			for(int iCycle=0;iCycle<g_iOEMPACycleCountMax;iCycle++)
			{
				for(int iGate=0;iGate<g_iOEMPAGateCountMax;iGate++)
				{
					if(m_aiAcquisitionSWAmpCount[iGate][iCycle] && m_apAcquisitionSWAmp[iGate][iCycle])
						delete m_apAcquisitionSWAmp[iGate][iCycle];
					m_aiAcquisitionSWAmpCount[iGate][iCycle] = 0;
					m_apAcquisitionSWAmp[iGate][iCycle] = NULL;
					m_aiAcquisitionSWAmpLastIndex[iGate][iCycle] = -1;
				}
			}
			m_llAcquisitionHWAmpSequenceIndex0 = -1;
			for(int iCycle=0;iCycle<g_iOEMPACycleCountMax;iCycle++)
			{
				for(int iGate=0;iGate<g_iOEMPAGateCountMax;iGate++)
				{
					if(m_aiAcquisitionHWAmpCount[iGate][iCycle] && m_apAcquisitionHWAmp[iGate][iCycle])
						delete m_apAcquisitionHWAmp[iGate][iCycle];
					m_aiAcquisitionHWAmpCount[iGate][iCycle] = 0;
					m_apAcquisitionHWAmp[iGate][iCycle] = NULL;
					m_aiAcquisitionHWAmpLastIndex[iGate][iCycle] = -1;
				}
			}
			for(int iCycle=0;iCycle<m_iCycleCount;iCycle++)
			{
				dAscanStart = m_dAscanStart[iCycle] = pCycle[iCycle].dStart;
				dAscanRange = m_dAscanRange[iCycle] = pCycle[iCycle].dRange;
				dAscanStop = m_dAscanStart[iCycle]+dAscanRange;
				m_aiPointCount[iCycle] = (int)pCycle[iCycle].lPointCount;
				for(int iGate=0;iGate<g_iOEMPAGateCountMax;iGate++)
				{
					m_abGateEnableHW[iGate][iCycle] = pCycle[iCycle].gate[iGate].bEnable;
					m_abGateEnableSW[iGate][iCycle] = false;
					if(!pCycle[iCycle].gate[iGate].bEnable)
						continue;
					dAux = pCycle[iCycle].gate[iGate].dStart;
					if((dAux<dAscanStart) || (dAscanStop<dAux))
						continue;
					m_adGateStart[iGate][iCycle] = dAux;
					dAux = pCycle[iCycle].gate[iGate].dStop;
					if((dAux<dAscanStart) || (dAscanStop<dAux))
						continue;
					m_adGateStop[iGate][iCycle] = dAux;
					dAux = m_adGateStart[iGate][iCycle]-dAscanStart;
					m_aiGateStart[iGate][iCycle] = (int)(m_aiPointCount[iCycle]*dAux/dAscanRange);
					dAux = m_adGateStop[iGate][iCycle]-dAscanStart;
					m_aiGateStop[iGate][iCycle] = (int)(m_aiPointCount[iCycle]*dAux/dAscanRange);
					m_abGateSign[iGate][iCycle] = (pCycle[iCycle].gate[iGate].eRectification==eSigned)?true:false;
					m_aeGateModeAmp[iGate][iCycle] = pCycle[iCycle].gate[iGate].eModeAmp;
					m_abGateEnableSW[iGate][iCycle] = true;
					m_bEnableCscan = true;
				}
			}
			if((pOEMPA->GetSWDeviceOEMPA()->GetTriggerMode()==eOEMPAEncoder) &&
				pOEMPA->GetSWDeviceOEMPA()->GetTriggerEncoderStep(m_dEncoderStep))
				m_bEncoderEnable = true;
			else
				m_bEncoderEnable = false;
			LeaveCriticalSection(&m_CriticSection);
		}
	}
	return NULL;
}

UINT WINAPI CDlgCscanDisplay::ProcessAcquisitionAscan_0x00010103(int iAscanCycle,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat)
{
	bool bAscanSign=true;
	WORD wDataCount;
	enumBitSize eBitSize;
	BYTE *pbyAcquisitionDataDst;
	BYTE *pbyAcquisitionDataSrc;
	char *pcAcquisitionDataSrc;
	WORD *pwAcquisitionDataSrc;
	short *psAcquisitionDataSrc;
	double dScale;
	LARGE_INTEGER llSequence;
	int iIndex,iPointCount;
	char cMax,cMin,cData;
	BYTE byMax,byData;
	
	if(!m_pDlg || !m_pDlg->m_bCheckEnableCscan)
		return 0;
	EnterCriticalSection(&m_CriticSection);
	llSequence.LowPart = pAscanHeader->seqLow;
	llSequence.HighPart = pAscanHeader->seqHigh;
	if(m_llAcquisitionSWAmpSequenceIndex0==-1)
		m_llAcquisitionSWAmpSequenceIndex0 = llSequence.QuadPart;
	eBitSize = (enumBitSize)pAscanHeader->bitSize;
	wDataCount = pAscanHeader->dataCount;
	if(pAscanHeader->sign)
		bAscanSign = true;
	else
		bAscanSign = false;
	if(eBitSize==eLog8Bits)
		bAscanSign = false;
	iIndex = (int)(llSequence.QuadPart-m_llAcquisitionSWAmpSequenceIndex0);
	if(m_bEncoderEnable)
		iIndex = (int)lround2(acqInfo.dEncoder[0] / m_dEncoderStep);
	if(m_pDlg)
	{
		for(int iGate=0;iGate<g_iOEMPAGateCountMax;iGate++)
		{
			if(!m_abGateEnableSW[iGate][iAscanCycle])
				continue;
			iPointCount = m_iEditPointCount;//m_aiPointCount[iAscanCycle];
			if(m_aiAcquisitionSWAmpCount[iGate][iAscanCycle]!=iPointCount)
			{
				if(m_apAcquisitionSWAmp[iGate][iAscanCycle])
					delete m_apAcquisitionSWAmp[iGate][iAscanCycle];
				m_apAcquisitionSWAmp[iGate][iAscanCycle] = NULL;
				m_aiAcquisitionSWAmpCount[iGate][iAscanCycle] = iPointCount;
				if(iPointCount)
				{
					m_apAcquisitionSWAmp[iGate][iAscanCycle] = new BYTE[iPointCount];
					if(m_apAcquisitionSWAmp[iGate][iAscanCycle])
						memset(m_apAcquisitionSWAmp[iGate][iAscanCycle],0,sizeof(BYTE)*iPointCount);
				}
				m_aiAcquisitionSWAmpLastIndex[iGate][iAscanCycle] = -1;
				if(m_bEncoderEnable && ((iIndex<0) || (iIndex>=m_aiAcquisitionSWAmpCount[iGate][iAscanCycle])))
					continue;
			}else
			if((iIndex<0) || (iIndex>=m_aiAcquisitionSWAmpCount[iGate][iAscanCycle]))
			{
				if(!m_bEncoderEnable)
				{
					m_llAcquisitionSWAmpSequenceIndex0 = llSequence.QuadPart;
					iIndex = 0;
					memset(m_apAcquisitionSWAmp[iGate][iAscanCycle],0,sizeof(BYTE)*m_aiAcquisitionSWAmpCount[iGate][iAscanCycle]);
					m_aiAcquisitionSWAmpLastIndex[iGate][iAscanCycle] = -1;
				}else{
					LeaveCriticalSection(&m_CriticSection);
					return 0;
				}
			}
			pbyAcquisitionDataDst = m_apAcquisitionSWAmp[iGate][iAscanCycle];
			if(!pbyAcquisitionDataDst)
				continue;
			cMax = 0;
			cMin = SCHAR_MAX;
			byMax = 0;
			switch(eBitSize)
			{
			case eLog8Bits:
			case e8Bits:
				if(bAscanSign)
				{
					pcAcquisitionDataSrc = (char*)pBufferMax;
					for(int i=m_aiGateStart[iGate][iAscanCycle];i<(int)m_aiGateStop[iGate][iAscanCycle];i++)
					{
						if(cMax<pcAcquisitionDataSrc[i])
						{
							cMax = pcAcquisitionDataSrc[i];
						}
						if(cMin>pcAcquisitionDataSrc[i])
						{
							cMin = pcAcquisitionDataSrc[i];
						}
					}
					switch(m_aeGateModeAmp[iGate][iAscanCycle])
					{
					case eAmpAbsolute:
						if((cMin<0) && (cMax<-cMin))
							cMax = cMin;
						if(m_abGateSign[iGate][iAscanCycle])
							pbyAcquisitionDataDst[iIndex] = (BYTE)cMax;
						else
							pbyAcquisitionDataDst[iIndex] = (BYTE)(cMax*2);
						break;
					case eAmpMaximum:
						if(m_abGateSign[iGate][iAscanCycle])
							pbyAcquisitionDataDst[iIndex] = (BYTE)cMax;
						else
							pbyAcquisitionDataDst[iIndex] = (BYTE)(cMax*2);
						break;
					case eAmpMinimum:
						cMax = cMin;
						if(m_abGateSign[iGate][iAscanCycle])
							pbyAcquisitionDataDst[iIndex] = (BYTE)cMax;
						else
							pbyAcquisitionDataDst[iIndex] = (BYTE)(cMax*2);
						break;
					case eAmpPeakToPeak:
						if(m_abGateSign[iGate][iAscanCycle])
							pbyAcquisitionDataDst[iIndex] = (BYTE)(cMax-cMin);
						else
							pbyAcquisitionDataDst[iIndex] = (BYTE)((cMax-cMin)*2);
						break;
					}
					m_aiAcquisitionSWAmpLastIndex[iGate][iAscanCycle] = iIndex;
				}else{
					pbyAcquisitionDataSrc = (BYTE*)pBufferMax;
					for(int i=m_aiGateStart[iGate][iAscanCycle];i<(int)m_aiGateStop[iGate][iAscanCycle];i++)
					{
						if(byMax<pbyAcquisitionDataSrc[i])
						{
							byMax = pbyAcquisitionDataSrc[i];
						}
					}
					if(m_abGateSign[iGate][iAscanCycle])
						pbyAcquisitionDataDst[iIndex] = byMax/2;
					else
						pbyAcquisitionDataDst[iIndex] = byMax;
					m_aiAcquisitionSWAmpLastIndex[iGate][iAscanCycle] = iIndex;
				}
				break;
			case e12Bits:
			case e16Bits:
				if(bAscanSign)
				{
					if(eBitSize==e12Bits)
						dScale = (double)0x07ff;
					else
						dScale = (double)SHRT_MAX;
					psAcquisitionDataSrc = (short*)pBufferMax;
					for(int i=m_aiGateStart[iGate][iAscanCycle];i<(int)m_aiGateStop[iGate][iAscanCycle];i++)
					{
						cData = (char)(psAcquisitionDataSrc[i]/dScale);
						if(cMax<cData)
						{
							cMax = cData;
						}
						if(cMin>cData)
						{
							cMin = cData;
						}
					}
					if(m_abGateSign[iGate][iAscanCycle])
						pbyAcquisitionDataDst[iIndex] = (BYTE)cMax;
					else
						pbyAcquisitionDataDst[iIndex] = (BYTE)cMax*2;
					switch(m_aeGateModeAmp[iGate][iAscanCycle])
					{
					case eAmpAbsolute:
						if((cMin<0) && (cMax<-cMin))
							cMax = -cMin;
						if(m_abGateSign[iGate][iAscanCycle])
							pbyAcquisitionDataDst[iIndex] = (BYTE)cMax;
						else
							pbyAcquisitionDataDst[iIndex] = (BYTE)(cMax*2);
						break;
					case eAmpMaximum:
						if(m_abGateSign[iGate][iAscanCycle])
							pbyAcquisitionDataDst[iIndex] = (BYTE)cMax;
						else
							pbyAcquisitionDataDst[iIndex] = (BYTE)(cMax*2);
						break;
					case eAmpMinimum:
						cMax = cMin;
						if(m_abGateSign[iGate][iAscanCycle])
							pbyAcquisitionDataDst[iIndex] = (BYTE)cMax;
						else
							pbyAcquisitionDataDst[iIndex] = (BYTE)(cMax*2);
						break;
					case eAmpPeakToPeak:
						if(m_abGateSign[iGate][iAscanCycle])
							pbyAcquisitionDataDst[iIndex] = (BYTE)(cMax-cMin);
						else
							pbyAcquisitionDataDst[iIndex] = (BYTE)((cMax-cMin)*2);
						break;
					}
					m_aiAcquisitionSWAmpLastIndex[iGate][iAscanCycle] = iIndex;
				}else{
					if(eBitSize==e12Bits)
						dScale = (double)0x0fff;
					else
						dScale = (double)USHRT_MAX;
					pwAcquisitionDataSrc = (WORD*)pBufferMax;
					for(int i=m_aiGateStart[iGate][iAscanCycle];i<(int)m_aiGateStop[iGate][iAscanCycle];i++)
					{
						byData = (BYTE)(pwAcquisitionDataSrc[i]/dScale);
						if(byMax<byData)
						{
							byMax = byData;
						}
					}
					if(m_abGateSign[iGate][iAscanCycle])
						pbyAcquisitionDataDst[iIndex] = byMax/2;
					else
						pbyAcquisitionDataDst[iIndex] = byMax;
					m_aiAcquisitionSWAmpLastIndex[iGate][iAscanCycle] = iIndex;
				}
				break;
			}
		}
		m_ullUpdateSWCount++;
	}
	LeaveCriticalSection(&m_CriticSection);
	return 0;
}

typedef union unionAmp{
	char charData;
	BYTE byteData;
}unionAmp;
UINT WINAPI CDlgCscanDisplay::ProcessAcquisitionCscan_0x00010X02(int iCscanCycle,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp, const structCscanAmpTof_0x0202 *pBufferAmpTof)
{
	unsigned int uiCycle,uiGateId,uiData;
	unionAmp *pAmp;
	BYTE *pbyAcquisitionDataDst;
	LARGE_INTEGER llSequence;
	int iIndexCscan,iPointCount;
	bool bProcess=true;

	if(!m_pDlg || !m_pDlg->m_bCheckEnableCscan)
		return 0;
	EnterCriticalSection(&m_CriticSection);
	llSequence.LowPart = pCscanHeader->seqLow;
	llSequence.HighPart = pCscanHeader->seqHigh;
	if(m_llAcquisitionHWAmpSequenceIndex0==-1)
		m_llAcquisitionHWAmpSequenceIndex0 = llSequence.QuadPart;
	iIndexCscan = (int)(llSequence.QuadPart-m_llAcquisitionHWAmpSequenceIndex0);
	if(m_bEncoderEnable)
		iIndexCscan = (int)lround2(acqInfo.dEncoder[0] / m_dEncoderStep);
	for(int iGate=0;iGate<g_iOEMPAGateCountMax;iGate++)
	{
		if(!m_abGateEnableHW[iGate][iCscanCycle])
			continue;
		iPointCount = m_iEditPointCount;//m_aiPointCount[iCscanCycle];
		if(m_aiAcquisitionHWAmpCount[iGate][iCscanCycle]!=iPointCount)
		{
			if(m_apAcquisitionHWAmp[iGate][iCscanCycle])
				delete m_apAcquisitionHWAmp[iGate][iCscanCycle];
			m_apAcquisitionHWAmp[iGate][iCscanCycle] = NULL;
			m_aiAcquisitionHWAmpCount[iGate][iCscanCycle] = iPointCount;
			if(iPointCount)
			{
				m_apAcquisitionHWAmp[iGate][iCscanCycle] = new BYTE[iPointCount];
				memset(m_apAcquisitionHWAmp[iGate][iCscanCycle],0,sizeof(BYTE)*m_aiAcquisitionHWAmpCount[iGate][iCscanCycle]);
			}
			m_aiAcquisitionHWAmpLastIndex[iGate][iCscanCycle] = -1;
			if(m_bEncoderEnable && ((iIndexCscan<0) || (iIndexCscan>=m_aiAcquisitionHWAmpCount[iGate][iCscanCycle])))
			{
				bProcess = false;
				continue;
			}
		}else
		if((iIndexCscan<0) || (iIndexCscan>=m_aiAcquisitionHWAmpCount[iGate][iCscanCycle]))
		{
			if(!m_bEncoderEnable)
			{
				m_llAcquisitionHWAmpSequenceIndex0 = llSequence.QuadPart;
				iIndexCscan = 0;
				memset(m_apAcquisitionHWAmp[iGate][iCscanCycle],0,sizeof(BYTE)*m_aiAcquisitionHWAmpCount[iGate][iCscanCycle]);
				m_aiAcquisitionHWAmpLastIndex[iGate][iCscanCycle] = -1;
			}else{
				LeaveCriticalSection(&m_CriticSection);
				return 0;
			}
		}
	}
	if(bProcess && pCscanHeader->Check() && pBufferAmp)
	{
		uiCycle = pCscanHeader->cycle;
		if(uiCycle<g_iOEMPACycleCountMax)
		for(int iIndex=0;iIndex<(int)pCscanHeader->count;iIndex++)
		{
			uiGateId = pBufferAmp[iIndex].gateId;
			if(uiGateId>=g_iOEMPAGateCountMax)
				continue;
			if(!m_abGateEnableHW[uiGateId][iCscanCycle])
				continue;
			pbyAcquisitionDataDst = m_apAcquisitionHWAmp[uiGateId][iCscanCycle];
			if(!pbyAcquisitionDataDst)
				continue;
			uiData = pBufferAmp[iIndex].byAmp;
			pAmp = (unionAmp*)&uiData;
			if(pBufferAmp[iIndex].sign)
				pbyAcquisitionDataDst[iIndexCscan] = pAmp->charData;
			else
				pbyAcquisitionDataDst[iIndexCscan] = pAmp->byteData;
			//m_bAcquisitionCscanAmp[uiGateId][uiCycle] = true;
		}
	}
	if(bProcess && pCscanHeader->Check() && pBufferAmpTof)
	{
		uiCycle = pCscanHeader->cycle;
		if(uiCycle<g_iOEMPACycleCountMax)
		for(int iIndex=0;iIndex<(int)pCscanHeader->count;iIndex++)
		{
			uiGateId = pBufferAmpTof[iIndex].gateId;
			if(uiGateId>=g_iOEMPAGateCountMax)
				continue;
			if(!m_abGateEnableHW[uiGateId][iCscanCycle])
				continue;
			pbyAcquisitionDataDst = m_apAcquisitionHWAmp[uiGateId][iCscanCycle];
			if(!pbyAcquisitionDataDst)
				continue;
			uiData = pBufferAmpTof[iIndex].byAmp;
			pAmp = (unionAmp*)&uiData;
			if(pBufferAmpTof[iIndex].sign)
				pbyAcquisitionDataDst[iIndexCscan] = pAmp->charData;
			else
				pbyAcquisitionDataDst[iIndexCscan] = pAmp->byteData;
			//m_sAcquisitionCscanTof[uiGateId][uiCycle] = pBufferAmpTof[iIndex].wTof;
		}
	}
	m_ullUpdateHWCount++;
	LeaveCriticalSection(&m_CriticSection);
	return 0;
}

void CDlgCscanDisplay::OnMove(int x, int y)
{
	int iResHorz,iResVert;
	WINDOWPLACEMENT wndPlacement;
	wchar_t pTitle[MAX_PATH];
	CDialog::OnMove(x, y);

	if((m_iIndexLayoutFile>=0) && UTKernel_LayoutFile()->GetOperatingSystemWindow(GetSafeHwnd(),MAX_PATH,pTitle,iResHorz,iResVert,wndPlacement))
		UTKernel_LayoutFile()->WritePositionWindow(m_iIndexLayoutFile,m_strLayoutTitle,iResHorz,iResVert,wndPlacement,0);
}

void CDlgCscanDisplay::OnBnClickedButtonReset()
{
	EnterCriticalSection(&m_CriticSection);
	for(int iCscanCycle=0;iCscanCycle<m_iCycleCount;iCscanCycle++)
	{
		for(int iGate=0;iGate<g_iOEMPAGateCountMax;iGate++)
		{
			if(m_abGateEnableSW[iGate][iCscanCycle] && m_aiAcquisitionSWAmpCount[iGate][iCscanCycle])
				memset(m_apAcquisitionSWAmp[iGate][iCscanCycle],0,sizeof(BYTE)*m_aiAcquisitionSWAmpCount[iGate][iCscanCycle]);
			if(m_abGateEnableHW[iGate][iCscanCycle] && m_aiAcquisitionHWAmpCount[iGate][iCscanCycle])
				memset(m_apAcquisitionHWAmp[iGate][iCscanCycle],0,sizeof(BYTE)*m_aiAcquisitionHWAmpCount[iGate][iCscanCycle]);
		}
	}
	LeaveCriticalSection(&m_CriticSection);
	if(g_pView)
		g_pView->UpdateImage(CDlgHW::m_bCheckDisplayImage?true:false);
}

void CDlgCscanDisplay::OnBnClickedCheckEnableCscan()
{
	UpdateData();
	if(!m_bCallbackCalled)
	{
		m_bCheckEnableCscan = FALSE;
		AfxMessageBox(L"Error: here is the way to enable Cscan:\r\n1) open this dialog.\r\n2) load an OEMPA file with HW gates.\r\n3) enable the Cscan checkbox (you have just clicked on it).");
	}
	UpdateDialog();
	if(g_pView && m_bCallbackCalled)
		g_pView->UpdateImage(CDlgHW::m_bCheckDisplayImage?true:false);
}

void CDlgCscanDisplay::OnBnClickedCheckDisplay()
{
	UpdateData();
	UpdateDialog();
	if(g_pView)
		g_pView->UpdateImage(CDlgHW::m_bCheckDisplayImage?true:false);
}

void CDlgCscanDisplay::OnBnClickedRadioHw()
{
	UpdateData();
	UpdateDialog();
	if(g_pView)
		g_pView->UpdateImage(CDlgHW::m_bCheckDisplayImage?true:false);
}

void CDlgCscanDisplay::OnBnClickedRadioSw()
{
	UpdateData();
	UpdateDialog();
	if(g_pView)
		g_pView->UpdateImage(CDlgHW::m_bCheckDisplayImage?true:false);
}

void CDlgCscanDisplay::OnCbnSelchangeComboGate()
{
	UpdateData();
	UpdateDialog();
	if(g_pView)
		g_pView->UpdateImage(CDlgHW::m_bCheckDisplayImage?true:false);
}

void CDlgCscanDisplay::OnEnKillfocusEditPointCount()
{
	UpdateData();
	UpdateDialog();
	if(g_pView)
		g_pView->UpdateImage(CDlgHW::m_bCheckDisplayImage?true:false);
}

void CDlgCscanDisplay::OnBnClickedCancel()
{
	CDialog::OnCancel();
}
