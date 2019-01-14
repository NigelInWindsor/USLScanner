// DlgMaximumDetection.cpp : implementation file
//

#include "stdafx.h"
#include "OEMPATool.h"
#include "DlgMaximumDetection.h"
#include "DlgHW.h"
#include "ChildView.h"
#include "limits.h"

extern CChildView *g_pView;

CRITICAL_SECTION CDlgMaximumDetection::m_CriticSection;
BOOL CDlgMaximumDetection::m_bCheckEnable=FALSE;
bool CDlgMaximumDetection::m_bNewSetup=false;
bool CDlgMaximumDetection::m_bUpdate=false;
double CDlgMaximumDetection::m_dEditAxisStart=0.0;
double CDlgMaximumDetection::m_dEditAxisStop=0.0;
double CDlgMaximumDetection::m_dEditAxisOffset=0.0;
double CDlgMaximumDetection::m_dEditGateStart=0.0;
double CDlgMaximumDetection::m_dEditGateStop=0.0;
double CDlgMaximumDetection::m_dDigitizingPeriod=10.0e-9;
double CDlgMaximumDetection::m_dAxisStart[g_iOEMPACycleCountMax];
double CDlgMaximumDetection::m_dAxisRange[g_iOEMPACycleCountMax];
int CDlgMaximumDetection::m_iCycleCount=0;
int CDlgMaximumDetection::m_iEditCycle=0;
int CDlgMaximumDetection::m_iStart=0;
int CDlgMaximumDetection::m_iRange=0;
double CDlgMaximumDetection::m_dMaxTof=0.0;
double CDlgMaximumDetection::m_dMaxAmp=0.0;
CDlgMaximumDetection *CDlgMaximumDetection::m_pDlgHW=NULL;

// CDlgMaximumDetection dialog

IMPLEMENT_DYNAMIC(CDlgMaximumDetection, CDialog)

CDlgMaximumDetection::CDlgMaximumDetection(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMaximumDetection::IDD, pParent)
{
	InitializeCriticalSection(&m_CriticSection);
	Create(CDlgMaximumDetection::IDD,pParent);
	ShowWindow(SW_SHOW);
}

CDlgMaximumDetection::~CDlgMaximumDetection()
{
}

void CDlgMaximumDetection::PostNcDestroy()
{
	if(m_pDlgHW==this)
		m_pDlgHW = NULL;
	delete this;
}

void CDlgMaximumDetection::Display()
{
	if(!m_pDlgHW)
		m_pDlgHW = new CDlgMaximumDetection;
	if(!m_pDlgHW)
		return;
	if(m_pDlgHW->IsIconic())
		m_pDlgHW->ShowWindow(SW_RESTORE);
	m_pDlgHW->ShowWindow(SW_SHOW);
}

void CDlgMaximumDetection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ENABLE, m_bCheckEnable);
	DDX_Control(pDX, IDC_EDIT_AXIS_START, m_EditAxisStart);
	DDX_Control(pDX, IDC_EDIT_AXIS_OFFSET, m_EditAxisOffset);
	DDX_Control(pDX, IDC_EDIT_AXIS_STOP, m_EditAxisStop);
	DDX_Control(pDX, IDC_EDIT_GATE_START, m_EditGateStart);
	DDX_Control(pDX, IDC_EDIT_GATE_STOP, m_EditGateStop);
	DDX_Control(pDX, IDC_EDIT_OUT, m_EditGateResult);
	DDX_Text(pDX, IDC_EDIT_AXIS_START, m_dEditAxisStart);
	DDX_Text(pDX, IDC_EDIT_AXIS_STOP, m_dEditAxisStop);
	DDX_Text(pDX, IDC_EDIT_AXIS_OFFSET, m_dEditAxisOffset);
	DDX_Text(pDX, IDC_EDIT_GATE_START, m_dEditGateStart);
	DDX_Text(pDX, IDC_EDIT_GATE_STOP, m_dEditGateStop);
	DDX_Control(pDX, IDC_EDIT_CYCLE, m_EditCycle);
	DDX_Text(pDX, IDC_EDIT_CYCLE, m_iEditCycle);
}


BEGIN_MESSAGE_MAP(CDlgMaximumDetection, CDialog)
	ON_BN_CLICKED(IDC_CHECK_ENABLE, &CDlgMaximumDetection::OnBnClickedCheckEnable)
	ON_BN_CLICKED(IDCANCEL, &CDlgMaximumDetection::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgMaximumDetection message handlers

BOOL CDlgMaximumDetection::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData();
	EnterCriticalSection(&m_CriticSection);
	for(int iCycle=0;iCycle<g_iOEMPACycleCountMax;iCycle++)
	{
		m_dAxisStart[iCycle] = 0.0;
		m_dAxisRange[iCycle] = 0.0;
	}
	LeaveCriticalSection(&m_CriticSection);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

int iround(double x);

void CDlgMaximumDetection::UpdateDialog()
{
	if(!m_pDlgHW)
		return;
	m_pDlgHW->m_EditAxisStart.EnableWindow(!m_bCheckEnable);
	m_pDlgHW->m_EditAxisOffset.EnableWindow(!m_bCheckEnable);
	m_pDlgHW->m_EditAxisStop.EnableWindow(!m_bCheckEnable);
	m_pDlgHW->m_EditGateStart.EnableWindow(!m_bCheckEnable);
	m_pDlgHW->m_EditGateStop.EnableWindow(!m_bCheckEnable);
	m_pDlgHW->m_EditGateResult.ShowWindow(m_bCheckEnable?SW_SHOW:SW_HIDE);
	if(m_bCheckEnable)
	{
		if(m_dEditGateStart>=m_dEditGateStop)
			m_dEditGateStop = m_dEditAxisStart;
		if(m_dEditGateStart<m_dEditAxisStart)
			m_dEditGateStart = m_dEditAxisStart;
		if(m_dEditGateStart>m_dEditAxisStop)
			m_dEditGateStart = m_dEditAxisStop;
		if(m_dEditGateStop<m_dEditAxisStart)
			m_dEditGateStop = m_dEditAxisStart;
		if(m_dEditGateStop>m_dEditAxisStop)
			m_dEditGateStop = m_dEditAxisStop;
		m_iStart = iround((m_dEditGateStart*1e-6-m_dAxisStart[m_iEditCycle])/m_dDigitizingPeriod);
		m_iRange = iround((m_dEditGateStop-m_dEditGateStart)*1e-6/m_dDigitizingPeriod);
	}
	m_pDlgHW->UpdateData(FALSE);
}

void CDlgMaximumDetection::OnTimer()
{
	if(!m_bCheckEnable)
	{
		if(m_bNewSetup)
		{
			m_bNewSetup = false;
			m_dEditAxisStart = m_dAxisStart[m_iEditCycle]*1e6;
			m_dEditAxisStop = m_dEditAxisStart + m_dAxisRange[m_iEditCycle]*1e6;
			m_bUpdate = true;
		}
		if(m_bUpdate)
		{
			m_bUpdate = false;
			UpdateDialog();
		}
		return;
	}
	CString string;
	string.Format(L"Max: %.3lf us\t%.2lf %%",m_dMaxTof*1e6,m_dMaxAmp);
	m_pDlgHW->m_EditGateResult.SetWindowText(string);
}

structCorrectionOEMPA* WINAPI CDlgMaximumDetection::CallbackCustomizedAPI(CHWDeviceOEMPA *pOEMPA,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception)
{
	int iCycle=0;
	double dStart,dRange,dStop;

	if(!m_pDlgHW)
		return NULL;
	if(eStep==eWriteHW_Leave)
	{
		if(pCycle)
		{
			EnterCriticalSection(&m_CriticSection);
			m_bNewSetup = true;
			m_dDigitizingPeriod = pOEMPA->GetSWDeviceOEMPA()->dGetClockPeriod();
			m_iCycleCount = pRoot->iCycleCount;
			if(m_iCycleCount>2048)
				m_iCycleCount = 2048;
			for(int iCycle=0;iCycle<m_iCycleCount;iCycle++)
			{
				dStart = m_dAxisStart[iCycle] = pCycle[iCycle].dStart;
				dRange = m_dAxisRange[iCycle] = pCycle[iCycle].dRange;
				dStop = m_dAxisStart[iCycle]+dRange;
			}
			LeaveCriticalSection(&m_CriticSection);
		}
	}
	return NULL;
}

UINT WINAPI CDlgMaximumDetection::ProcessAcquisitionAscan_0x00010103(int iAscanCycle,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat)
{
	bool bAscanSign=true;
	WORD wDataCount;
	enumBitSize eBitSize;
	BYTE *pbyAcquisitionDataSrc;
	char *pcAcquisitionDataSrc;
	WORD *pwAcquisitionDataSrc;
	short *psAcquisitionDataSrc;
	double dScale;
	LARGE_INTEGER llSequence;

	EnterCriticalSection(&m_CriticSection);
	llSequence.LowPart = pAscanHeader->seqLow;
	llSequence.HighPart = pAscanHeader->seqHigh;
	eBitSize = (enumBitSize)pAscanHeader->bitSize;
	wDataCount = pAscanHeader->dataCount;
	if(pAscanHeader->sign)
		bAscanSign = true;
	else
		bAscanSign = false;
	if(eBitSize==eLog8Bits)
		bAscanSign = false;
	if(!m_bCheckEnable || (iAscanCycle!=m_iEditCycle) || !m_pDlgHW)
	{
		LeaveCriticalSection(&m_CriticSection);
		return 0;
	}
	if(!m_pDlgHW->m_iRange)
	{
		LeaveCriticalSection(&m_CriticSection);
		return 0;
	}
	m_pDlgHW->m_dMaxAmp = 0.0;
	m_pDlgHW->m_dMaxTof = 0.0;
	switch(eBitSize)
	{
	case eLog8Bits:
	case e8Bits:
		if(bAscanSign)
		{
			dScale = (double)SCHAR_MAX;
			pcAcquisitionDataSrc = (char*)pBufferMax;
			for(int i=m_iStart;i<(int)wDataCount;i++)
			{
				if(i>=m_iStart+m_iRange)
					break;
				if(m_pDlgHW->m_dMaxAmp<pcAcquisitionDataSrc[i])
				{
					m_pDlgHW->m_dMaxAmp = pcAcquisitionDataSrc[i];
					m_pDlgHW->m_dMaxTof = m_dAxisStart[m_iEditCycle]+m_dEditAxisOffset*1e-6+i*m_pDlgHW->m_dDigitizingPeriod;
				}
			}
		}else{
			dScale = (double)UCHAR_MAX;
			pbyAcquisitionDataSrc = (BYTE*)pBufferMax;
			for(int i=m_iStart;i<(int)wDataCount;i++)
			{
				if(i>=m_iStart+m_iRange)
					break;
				if(m_pDlgHW->m_dMaxAmp<pbyAcquisitionDataSrc[i])
				{
					m_pDlgHW->m_dMaxAmp = pbyAcquisitionDataSrc[i];
					m_pDlgHW->m_dMaxTof = m_dAxisStart[m_iEditCycle]+m_dEditAxisOffset*1e-6+i*m_pDlgHW->m_dDigitizingPeriod;
				}
			}
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
			for(int i=m_iStart;i<(int)wDataCount;i++)
			{
				if(i>=m_iStart+m_iRange)
					break;
				if(m_pDlgHW->m_dMaxAmp<psAcquisitionDataSrc[i])
				{
					m_pDlgHW->m_dMaxAmp = psAcquisitionDataSrc[i];
					m_pDlgHW->m_dMaxTof = m_dAxisStart[m_iEditCycle]+m_dEditAxisOffset*1e-6+i*m_pDlgHW->m_dDigitizingPeriod;
				}
			}
		}else{
			if(eBitSize==e12Bits)
				dScale = (double)0x0fff;
			else
				dScale = (double)USHRT_MAX;
			pwAcquisitionDataSrc = (WORD*)pBufferMax;
			for(int i=m_iStart;i<(int)wDataCount;i++)
			{
				if(i>=m_iStart+m_iRange)
					break;
				if(m_pDlgHW->m_dMaxAmp<pwAcquisitionDataSrc[i])
				{
					m_pDlgHW->m_dMaxAmp = pwAcquisitionDataSrc[i];
					m_pDlgHW->m_dMaxTof = m_dAxisStart[m_iEditCycle]+m_dEditAxisOffset*1e-6+i*m_pDlgHW->m_dDigitizingPeriod;
				}
			}
		}
		break;
	}
	m_pDlgHW->m_dMaxAmp *= 100.0;
	m_pDlgHW->m_dMaxAmp /= dScale;
	LeaveCriticalSection(&m_CriticSection);
	return 0;
}

void CDlgMaximumDetection::OnBnClickedCheckEnable()
{
	UpdateData();
	UpdateDialog();
}

void CDlgMaximumDetection::OnBnClickedCancel()
{
	CDialog::OnCancel();
}
