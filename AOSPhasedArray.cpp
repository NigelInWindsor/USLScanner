#include "stdafx.h"
#include "AOSPhasedArray.h"
#include "USLScanner.h"
#include "CPing.h"


structCorrectionOEMPA g_Correction;//useful for corrected view.

CHWDeviceOEMPA *m_pHWDeviceOEMPA = NULL;
CUTUsb3Device *g_pUsb3Device = NULL;

unsigned char ucArray[8192];

CRITICAL_SECTION m_CriticSectionCounters;
CRITICAL_SECTION m_ChannelCriticalSection;
int m_iChannelProbe;
int m_iChannelScan;
int m_iChannelCount;
static int m_iChannelProbeCount;
static int m_iChannelScanCycle[g_iOEMMCProbeCountMax];
static int m_iFMCElementStart;
static int m_iFMCElementStop;
static int m_iFMCElementStep;
static int m_iFMCElementCount;
static bool m_bFullMatrixCapture;
static bool m_bMultiHWChannel;
static BYTE m_abyMultiChannelDecimation[g_iOEMPACycleCountMax];
static float m_afMultiChannelStart[g_iOEMPACycleCountMax];
static float m_fMultiChannelPeriod;
static clock_t c_f;
static clock_t c_i;
static int m_nFrameCount;
static int m_nFrameRate;
static int m_nLastFrameCount;
static int m_nLastPRFCount;
static int m_nPRFCount;
static int m_nPRFRate;
static unsigned int m_nDataRate = 0;
static unsigned int m_nLastTotalBytesRead = 0;
static unsigned int m_nTotalBytesRead = 0;


CAOSPhasedArray::CAOSPhasedArray()
{

	m_bConnected = false;
	m_bFullMatrixCapture = false;
	m_bMultiHWChannel = false;
	m_fMultiChannelPeriod = 10.0e-9f;
	m_iFMCElementStart = 0;
	m_iFMCElementStop = 0;
	m_iFMCElementStep = 1;
	m_iFMCElementCount = 1;
	m_iChannelProbeCount = 0;
	m_iChannelCount = 0;
	m_iChannelProbe = -1,
	m_iChannelScan = -1;

	m_fTxFocalLength = 50.0f;
	m_fElementPitch = 0.5f;
	m_nScanMode = 0;
	ZeroMemory(m_fRxFocalLength, sizeof m_fRxFocalLength);

	m_nFrameCount = 0;

	ZeroMemory(m_iChannelScanCycle, sizeof m_iChannelScanCycle);

	InitializeCriticalSection(&m_ChannelCriticalSection);
	InitializeCriticalSection(&m_CriticSectionCounters);
	ResetCounters();
	memset(&g_Correction, 0, sizeof(g_Correction));
	OEMPA_New(g_Correction);
	g_bInitGain = true;
	memset(m_abyMultiChannelDecimation, 0, sizeof(m_abyMultiChannelDecimation));
	memset(m_afMultiChannelStart, 0, sizeof(m_afMultiChannelStart));

}


CAOSPhasedArray::~CAOSPhasedArray()
{
	Enable(false);
	Disconnect();
}

void CAOSPhasedArray::ResetCounters()
{
	
	EnterCriticalSection(&m_CriticSectionCounters);
	m_dwDigitalInputsAscan = 0;
	m_dwDigitalInputsCscan = 0;
	LeaveCriticalSection(&m_CriticSectionCounters);
	
}

bool CAOSPhasedArray::Connect(int nIPAddress)
{
	wchar_t pValue[40];
	DWORD dwProcessId;
	CString Buff;
	sockaddr_in Server;
	CString strAddrs;

	if (m_bConnected == false) {

		if (nIPAddress == 0) {
			ZeroMemory(&Server, sizeof Server);
			Server.sin_addr.S_un.S_un_b.s_b4 = 192;
			Server.sin_addr.S_un.S_un_b.s_b3 = 168;
			Server.sin_addr.S_un.S_un_b.s_b2 = 1;
			Server.sin_addr.S_un.S_un_b.s_b1 = 11;
		}
		else {
			Server.sin_addr.s_addr = nIPAddress;
		}
		swprintf(pValue, 40, L"%d.%d.%d.%d", Server.sin_addr.S_un.S_un_b.s_b4, Server.sin_addr.S_un.S_un_b.s_b3, Server.sin_addr.S_un.S_un_b.s_b2, Server.sin_addr.S_un.S_un_b.s_b1);
		unsigned short usValue = 5001;

		CPing Ping;
		strAddrs.Format(L"%s", pValue);
		if (Ping.Ping(strAddrs, usValue, &m_Messages) != 0) {
			Buff.Format(L"Failed to Ping AOS Box %s", strAddrs);
			m_Messages.Add(Buff);
			return false;
		}


		Buff = AfxGetAppName();
		CHWDeviceOEMPA::RegisterMultiProcess(Buff);
		DriverRegistration();


		if (CHWDeviceOEMPA::IsMultiProcessConnected(pValue, dwProcessId)) {
			CHWDeviceOEMPA::DisconnectMultiProcess(pValue, dwProcessId);
		}

		if (m_pHWDeviceOEMPA) {
			m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->SetIPAddress(pValue);
			m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->SetPort(usValue);

			m_strEditLostCount = L"";
			if (!m_pHWDeviceOEMPA->GetSWDevice()->SetConnectionState(eConnected, true)) {
				AfxMessageBox(L"Error : Can't connect to AOS Phased array");
				return false;
			}

			ProcessConnection();
		}
	}
	return m_bConnected;
}

bool CAOSPhasedArray::Disconnect()
{
	TRACE0("AOS Disconnect\n");

	if (m_pHWDeviceOEMPA) {
		if(m_pHWDeviceOEMPA->LockDevice()) {
			if (m_pHWDeviceOEMPA->GetSWDevice()->IsPulserEnabled()) {
				m_pHWDeviceOEMPA->GetSWDevice()->EnablePulser(false);
			}
	
			m_pHWDeviceOEMPA->UnlockDevice();
		}
		
		if (!m_pHWDeviceOEMPA->GetSWDevice()->SetConnectionState(eDisconnected, true)) {
			AfxMessageBox(L"Error : Can't disconnect from AOS Phased array");
			return false;
		}
		if (!m_pHWDeviceOEMPA->UnregisterMultiProcess()) {
			AfxMessageBox(L"Error : failed to unregister multi process");
			return false;
		}

		SAFE_DELETE( m_pHWDeviceOEMPA);
		m_bConnected = false;
	}
	UTKernel_Close();
	
	TRACE0("AOS Disconnected\n");
	return true;
}

bool CAOSPhasedArray::isConnected()
{
	return m_bConnected;
}


void CAOSPhasedArray::ProcessConnection()
{
	bool bRet = true;

	m_iIOMaxTemperature = 0;
	m_bFullMatrixCapture = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsFullMatrixCapture();
	if (m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsTFMProcessed())
		m_bFullMatrixCapture = false;
	m_bMultiHWChannel = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsMultiHWChannelEnabled();

	m_nCycleCount = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetCycleCount();

	m_bConnected = true;
}

void CAOSPhasedArray::getStatus()
{
//	WORD wTemperature, wFWId;
	CString string, strFile, strType;
	int iBoardCount, iSensorCountMax;
	WORD wFWId;
//	int iSensorCountMax, iMasterDeviceId, iDeviceCount, , iSensorCount, iBoardCount2;

	if (!m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetTemperatureCount(iBoardCount, iSensorCountMax))
		return;
/*
	if (!m_pHWDeviceOEMPA->LockDevice(eAcqOff))
	{
		AfxMessageBox(L"Communication error!");
		return;
	}

	if (!(bStatus = m_pHWDeviceOEMPA->GetFWId(&wFWId))) wFWId = 0;
	if (m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->IsOEMMCEnabled())
		strType = L"OEMMC";
	else
		strType = L"OEMPA";

	if (!m_pHWDeviceOEMPA->UnLockDevice(eAcqOff))
	{
		AfxMessageBox(L"Communication error!");
		return;
	}
*/
	wFWId = (WORD)m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetFirmwareId();
	m_nMaxElementCount = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetElementCountMax();
	m_nMaxApertureSize = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetApertureCountMax();
	m_nMaxCycleCount = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetCycleCountMax();
	m_bMultiplexer = m_pHWDeviceOEMPA->IsMultiplexer();

}

void CAOSPhasedArray::getMaxNumberElements(CString *pBuff)
{
	pBuff->Format(L"Number elements: %d", m_nMaxElementCount);
}

void CAOSPhasedArray::getMaxApertureSize(CString *pBuff)
{
	pBuff->Format(L"Aperture size: %d", m_nMaxApertureSize);
}

void CAOSPhasedArray::getMaxCycleCount(CString *pBuff)
{
	pBuff->Format(L"Maximum focal laws: %d", m_nMaxCycleCount);
}

bool CAOSPhasedArray::isMultiplexer(CString *pBuff)
{
	if (pBuff != NULL) {
		if (m_bMultiplexer == true) {
			pBuff->Format(L"Multiplexer: true");
		}
		else {
			pBuff->Format(L"Multiplexer: true");
		}
	}
	return m_bMultiplexer;
}

bool CAOSPhasedArray::Enable(bool bStart)
{
	if (isConnected() == false) return false;
	bool bRet;
	CHWDeviceOEMPA *pSlave = (CHWDeviceOEMPA*)m_pHWDeviceOEMPA->GetMatched256DeviceOEMPA();

	if (!g_pUsb3Device)
		g_pUsb3Device = m_pHWDeviceOEMPA->GetSWDeviceOEMPA()->GetUSB3Device();
	if (g_pUsb3Device && (g_pUsb3Device->GetVersion() == 0xCAFE))
	{
		if (m_bCheckShotEnable)
			g_pUsb3Device->EnableEmulator(1.0);
		else
			g_pUsb3Device->EnableEmulator(0.0);
		return true;
	}

	g_bFirstShotEnable = true;
	m_bUnlockEnableShot = bStart;
	m_bCheckShotEnable = bStart;

	if (m_bUnlockEnableShot)
	{
		if (m_bCheckShotEnable)
			m_pHWDeviceOEMPA->GetSWDevice()->SetUnlockEnableShot(true);
		else
			m_pHWDeviceOEMPA->GetSWDevice()->SetUnlockEnableShot(false);
		if (pSlave)
		{
			if (m_bCheckShotEnable)
				pSlave->GetSWDevice()->SetUnlockEnableShot(true);
			else
				pSlave->GetSWDevice()->SetUnlockEnableShot(false);
		}
	}
	if (m_bCheckShotEnable)
		bRet = HWEnablePulser(true);
	else
		bRet = HWEnablePulser(false);
	if (!bRet) {
		if (bStart == true)			AfxMessageBox(L"Failed to start AOS pulser!");
		if (bStart == false)		AfxMessageBox(L"Failed to stop AOS pulser!");

	}

	return bRet;
}


int CAOSPhasedArray::getFrameCount()
{
	return m_nFrameCount;
}

int CAOSPhasedArray::getFrameRate()
{
	return m_nFrameRate;
}

int CAOSPhasedArray::getPRFRate()
{
	return m_nPRFRate;
}

unsigned int CAOSPhasedArray::getDataRate()
{
	return m_nDataRate;
}

bool CAOSPhasedArray::setAllDacVariables(PVOID pParent)
{
	bool bRet = true;
	bool bEnableDac = false;
	if (isConnected() == false)	return false;
	double dTime[64];
	float fGain[64];
	ZeroMemory(dTime, sizeof dTime);
	ZeroMemory(fGain, sizeof fGain);
	int nDacCount = 0;
	CPhasedArrayProbe* pProbe = (CPhasedArrayProbe*)pParent;

	switch (pProbe->getDacMode()) {
	case 0: bEnableDac = false;
		break;
	case 1: bEnableDac = true;
		nDacCount = pProbe->getDACCount(0);
		break;
	case 2: bEnableDac = true;
		nDacCount = pProbe->getDACCount(0);
		break;
	}

	for (nDacCount = 0; nDacCount < 64; nDacCount++) {
		if (nDacCount != 0 && pProbe->m_fDacDelay[0][nDacCount] == 0.0f) break;
		dTime[nDacCount] = (double)(pProbe->m_fDacDelay[0][nDacCount] * 1e-6f);
		fGain[nDacCount] = pProbe->m_fDacGain[0][nDacCount];
	}

	if (m_pHWDeviceOEMPA->LockDevice(eAcqOff))
		{

		for (int nCycle = 0; nCycle < m_nCycleCount && bRet == true; nCycle++) {
			if (!m_pHWDeviceOEMPA->EnableDAC(nCycle, bEnableDac)) bRet = false;
			if (!m_pHWDeviceOEMPA->SetDACGain(true, nCycle, nDacCount, dTime, fGain)) bRet = false;
		}


		if (!m_pHWDeviceOEMPA->UnlockDevice(eAcqOn)) {
			bRet = false;
			AfxMessageBox(L"Failed to unlock");
			m_Messages.Add(L"Failed to Unlock: setAllDacVariables");
		}
	}

	if (pProbe->getDacMode() == 2) {
		setInterfaceGate(pParent, true);
	}
	else {
		setInterfaceGate(pParent, false);
	}

	return bRet;
}

bool CAOSPhasedArray::setInterfaceGate(PVOID pParent, bool bEnable)
{
	bool bRet = true;
	if (isConnected() == false)	return false;
	CPhasedArrayProbe* pProbe = (CPhasedArrayProbe*)pParent;

	int nGate = 0;
	double dThresholdPercent = (double)theApp.m_UtUser.m_TS[0].Gate.nThreshold[0];
	enumGateModeAmp eGateModeAmp = eAmpMaximum;
	enumGateModeTof eGateModeTof = eTofThresholdCross;
	enumRectification eGateRectification = eSigned;
	double dGateStart = (double)theApp.m_UtUser.m_TS[0].Gate.nNsDelay[0] * 1.0e-9;
	double dGateStop = (double)(theApp.m_UtUser.m_TS[0].Gate.nNsWidth[0] + theApp.m_UtUser.m_TS[0].Gate.nNsDelay[0]) * 1.0e-9;
	int iTrackingCycleIndex = 0;
	int iTrackingGateIndex = 0;

	if (m_pHWDeviceOEMPA->LockDevice(eAcqOff))
	{
		for (int nCycle = 0; nCycle < m_nCycleCount && bRet == true; nCycle++) {
			if (!m_pHWDeviceOEMPA->SetGateModeThreshold(nCycle, nGate, bEnable, dThresholdPercent, eGateModeAmp, eGateModeTof, eGateRectification)) bRet = false;
			if (!m_pHWDeviceOEMPA->SetGateStart(nCycle, nGate, dGateStart)) bRet = false;
			if (!m_pHWDeviceOEMPA->SetGateStop(nCycle, nGate, dGateStop)) bRet = false;
			iTrackingCycleIndex = nCycle;
			if (!m_pHWDeviceOEMPA->SetTrackingDac(nCycle, bEnable, iTrackingCycleIndex, iTrackingGateIndex)) bRet = false;
		}

		if (!m_pHWDeviceOEMPA->UnlockDevice(eAcqOn)) {
			bRet = false;
			AfxMessageBox(L"Failed to unlock");
			m_Messages.Add(L"Failed to Unlock: setInterfaceGate");
		}
	}

	return bRet;
}

bool CAOSPhasedArray::setAnalogueGain(float fGain)
{
	bool bRet = true;
	if (isConnected() == false)	return false;

	if (m_pHWDeviceOEMPA->LockDevice(eAcqOff))
	{
		for (int nCycle = 0; nCycle < m_nCycleCount; nCycle++) {
			if (!m_pHWDeviceOEMPA->SetGainAnalog(nCycle, fGain))
				bRet = false;
		}
		if (!m_pHWDeviceOEMPA->UnlockDevice(eAcqOn)) {
			bRet = false;
			AfxMessageBox( L"Failed to unlock");
		}
	}

	return bRet;
}

bool CAOSPhasedArray::setDigitalGain(float fGain)
{
	bool bRet = true;
	if (isConnected() == false)	return false;
	double dGain = (double)fGain;

	if (m_pHWDeviceOEMPA->LockDevice(eAcqOff))
	{
		for (int nCycle = 0; nCycle < m_nCycleCount; nCycle++) {
			if (!m_pHWDeviceOEMPA->SetGainDigital(nCycle, dGain))
				bRet = false;
		}
		if (!m_pHWDeviceOEMPA->UnlockDevice(eAcqOn)) {
			bRet = false;
			AfxMessageBox( L"Failed to unlock");
		}
	}

	return bRet;
}

bool CAOSPhasedArray::setPulseWidth(PVOID pParent, float fPulseWidth)
{
	bool bRet = true;
	if (isConnected() == false)	return false;

	CPhasedArrayProbe* pProbe = (CPhasedArrayProbe*)pParent;

	if (m_pHWDeviceOEMPA->LockDevice(eAcqOff))
	{
		for (int nCycle = 0; nCycle < m_nCycleCount; nCycle++) {

			for (int nEl = 0; nEl < MAX_APERTURE_COUNT; nEl++) pProbe->m_FLTx[nCycle].m_fPulseWidth[nEl] = fPulseWidth;
			if (!m_pHWDeviceOEMPA->SetEmissionWidths(nCycle, pProbe->m_FLTx[nCycle].m_dwAperture, pProbe->m_FLTx[nCycle].m_fPulseWidth)) bRet = false;

		}
		if (!m_pHWDeviceOEMPA->UnlockDevice(eAcqOn)) {
			bRet = false;
			AfxMessageBox( L"Failed to unlock");
		}
	}

	return bRet;
}



bool CAOSPhasedArray::DriverRegistration()
{
	bool bError = false;
	__int64 iBufferByteSize = (__int64)2 * (__int64)8 * (__int64)1024 * (__int64)1024;
#ifdef _WIN64
	CAcquisitionFifo *pFifoAscan, *pFifoCscan, *pFifoIO;
#endif _WIN64

	if (!m_pHWDeviceOEMPA)
	{
		OEMPA_SetCallbackCustomizedDriverAPI(CAOSPhasedArray::CallbackCustomizedAPI);
		m_pHWDeviceOEMPA = new CHWDeviceOEMPA();
		if (!m_pHWDeviceOEMPA)
			bError = true;
		if (!bError && !m_pHWDeviceOEMPA->SetAcquisitionParameter(NULL /*g_pView*/))
			bError = true;
		if (!bError)
		{
#ifdef _WIN64
			pFifoAscan = m_pHWDeviceOEMPA->GetAcquisitionFifo(eFifoAscan);
			if (m_bFifoAscanEnable && !pFifoAscan->Alloc(1024, iBufferByteSize))//function to enable the fifo for ascan.
				bError = true;
			pFifoCscan = m_pHWDeviceOEMPA->GetAcquisitionFifo(eFifoCscan);
			if (m_bFifoCscanEnable && !pFifoCscan->Alloc(1024, 1024 * 1024))//function to enable the fifo for cscan.
				bError = true;
			pFifoIO = m_pHWDeviceOEMPA->GetAcquisitionFifo(eFifoIO);
#endif _WIN64
			if (!m_bFifoAscanEnable || (m_bFifoAscanEnable && m_bFifoAscanDriverThread))
			{
				if (!m_pHWDeviceOEMPA->SetAcquisitionAscan_0x00010103(CAOSPhasedArray::ProcessAcquisitionAscan_0x00010103))
					bError = true;
			}
			if (!m_bFifoCscanEnable || (m_bFifoCscanEnable && m_bFifoCscanDriverThread))
			{
				if (!bError && !m_pHWDeviceOEMPA->SetAcquisitionCscan_0x00010X02(ProcessAcquisitionCscan_0x00010X02))
					bError = true;
			}
			if (!m_bFifoIOEnable || (m_bFifoIOEnable && m_bFifoIODriverThread))
			{
				if (!bError && !m_pHWDeviceOEMPA->SetAcquisitionIO_1x00010101(ProcessAcquisitionIO_1x00010101, true))
					bError = true;
			}
			if ((m_bFifoAscanEnable && !m_bFifoAscanDriverThread) ||
				(m_bFifoCscanEnable && !m_bFifoCscanDriverThread) ||
				(m_bFifoIOEnable && !m_bFifoIODriverThread))
			{//explicit thread to manage the fifoAscan. In case of the integrated thread to manage the fifo (in the driver) no explicit thread is required.
//NG				m_hThread = CreateThread(NULL, TRUE, ThreadFifo, this, 0, NULL);
			}
		}
		if (m_pHWDeviceOEMPA)
		{
			m_bUnlockEnableShot = m_pHWDeviceOEMPA->GetSWDevice()->GetUnlockEnableShot();
			if (m_bUnlockEnableShot)
				m_pHWDeviceOEMPA->GetSWDevice()->SetUnlockDefaultEnablePulser(false);
		}
	}
	if (bError)
		AfxMessageBox(L"ERROR to register the driver instance !!!");
	return !bError;
}

DWORD CAOSPhasedArray::CallbackThreadToolbox(HWND hWnd, CRunToolbox * pRunToolbox)
{
	return 0;
}

UINT WINAPI CAOSPhasedArray::ProcessAcquisitionAscan_0x00010103(void * pAcquisitionParameter, structAcqInfoEx & acqInfo, const CStream_0x0001 * pStreamHeader, const CSubStreamAscan_0x0103 * pAscanHeader, const void * pBufferMax, const void * pBufferMin, const void * pBufferSat)
{
	unsigned char *pDest;

	EnterCriticalSection(&m_CriticSectionCounters);

	m_nPRFCount++;
	if (pAscanHeader->cycle == 0) {
		m_nFrameCount++;
		c_f = clock();
		double dElapsedTime = (double)(c_f - c_i) / CLOCKS_PER_SEC;

		if (dElapsedTime > 0.5) {
			int nConversions = m_nFrameCount - m_nLastFrameCount;
			c_i = c_f;
			m_nLastFrameCount = m_nFrameCount;
			m_nFrameRate = (int)((double)nConversions / dElapsedTime);

			nConversions = m_nPRFCount - m_nLastPRFCount;
			m_nLastPRFCount = m_nPRFCount;
			m_nPRFRate = (int)((double)nConversions / dElapsedTime);
			m_nDataRate = (int)((double)(m_nTotalBytesRead - m_nLastTotalBytesRead) / dElapsedTime);
			m_nLastTotalBytesRead = m_nTotalBytesRead;
		}
	}
	if (pAscanHeader->FWAcqIdChannelProbe >= 0 && pAscanHeader->FWAcqIdChannelProbe < 128 && pAscanHeader->dataCount>0 && pAscanHeader->dataCount<=8192) {
		memcpy(ucArray, pBufferMax, pAscanHeader->dataCount);
		m_nTotalBytesRead += pAscanHeader->dataCount;
		int nSlot = pAscanHeader->FWAcqIdChannelProbe;

		if (nSlot >= 0 && nSlot < 256 && pAscanHeader->dataCount>0 && pAscanHeader->dataCount < 8192) {

			pDest = (unsigned char*)theApp.m_Scope.m_RFTrace[nSlot];
			if (theApp.m_UtUser.m_TS[0].Adc.nRfType == RFTYPELINEAR) {
				for (unsigned int ii = 0; ii < pAscanHeader->dataCount; ii++, pDest++)
					*pDest = ucArray[ii];
			}
			else {
				for (unsigned int ii = 0; ii < pAscanHeader->dataCount; ii++, pDest++)
					*pDest = ucArray[ii] >>= 1;
			}
			theApp.m_LSA.StoreAScan(nSlot, ucArray, pAscanHeader->dataCount);
			theApp.m_UtUser.m_TS[nSlot].Adc.nAcqLength = pAscanHeader->dataCount;
			theApp.m_UtUser.ProcessTrace(theApp.m_Scope.m_RFTrace[nSlot], nSlot);
		}
		else {
			TRACE0("Faulty AOS data packet\n");
		}

//		memcpy(theApp.m_Scope.m_RFTrace[pAscanHeader->FWAcqIdChannelProbe], pBufferMax, pAscanHeader->dataCount);
//		theApp.m_LSA.StoreAScan(pAscanHeader->FWAcqIdChannelProbe, (unsigned char*)pBufferMax, pAscanHeader->dataCount);
	}
	LeaveCriticalSection(&m_CriticSectionCounters);

	return 0;
}

UINT CAOSPhasedArray::ProcessAcquisitionCscan_0x00010X02(void * pAcquisitionParameter, structAcqInfoEx & acqInfo, const CStream_0x0001 * pStreamHeader, const CSubStreamCscan_0x0X02 * pCscanHeader, const structCscanAmp_0x0102 * pBufferAmp, const structCscanAmpTof_0x0202 * pBufferAmpTof)
{
	return 0;
}

UINT CAOSPhasedArray::ProcessAcquisitionIO_1x00010101(void * pAcquisitionParameter, structAcqInfoEx & acqInfo, const CStream_0x0001 * pStreamHeader, const CSubStreamIO_0x0101 * pIOHeader)
{
	return 0;
}

structCorrectionOEMPA* WINAPI CAOSPhasedArray::CallbackCustomizedAPI(void *pAcquisitionParameter, const wchar_t *pFileName, enumStepCustomizedAPI eStep, structRoot *pRoot, structCycle *pCycle, CFocalLaw *pEmission, CFocalLaw *pReception)
{
	return &g_Correction;
}

UINT CAOSPhasedArray::sProcessAcquisitionStream(void * pAcquisitionParameter, void * pData, int iSize)
{
	return 0;
}

UINT CAOSPhasedArray::ProcessAcquisitionStream(void * pData, int iSize)
{
	return 0;
}

DWORD WINAPI CAOSPhasedArray::ThreadFifo(LPVOID pParam)
{
#ifdef _WIN64
	structAcqInfoEx acqInfo, *pAcqInfo;
	CStream_0x0001 *pStreamHeader;
	CSubStreamAscan_0x0103 *pAscanHeader;
	const CSubStreamIO_0x0101 *pIOHeader;
	const CSubStreamCscan_0x0X02* pCscanHeader;
	const structCscanAmp_0x0102* pBufferAmp;
	const structCscanAmpTof_0x0202* pBufferAmpTof;
	void *pBufferMax = NULL;
	void *pBufferMin = NULL;
	void *pBufferSat = NULL;
	int iFifoCount;
	CAcquisitionFifo *pFifoAscan;
	CAcquisitionFifo *pFifoCscan;
	CAcquisitionFifo *pFifoIO;

	if (!m_pDlgHW->m_pHWDeviceOEMPA)
		return 0;
	while (!g_bExit)
	{
		if (!m_pDlgHW->m_pHWDeviceOEMPA)
			break;
		pFifoAscan = m_pDlgHW->m_pHWDeviceOEMPA->GetAcquisitionFifo(eFifoAscan);
		pFifoCscan = m_pDlgHW->m_pHWDeviceOEMPA->GetAcquisitionFifo(eFifoCscan);
		pFifoIO = m_pDlgHW->m_pHWDeviceOEMPA->GetAcquisitionFifo(eFifoIO);
		iFifoCount = pFifoAscan->GetCount();
		if (!m_pDlgHW->m_bFifoAscanDriverThread && iFifoCount)
		{
			while (pFifoAscan->OutAscan(-1, true, pAcqInfo, pStreamHeader, pAscanHeader, pBufferMax, pBufferMin, pBufferSat))
			{
				if (pAcqInfo)
					memcpy(&acqInfo, pAcqInfo, sizeof(acqInfo));
				if (pStreamHeader && pAscanHeader)
					ProcessAcquisitionAscan_0x00010103(g_pView, acqInfo, pStreamHeader, pAscanHeader, pBufferMax, pBufferMin, pBufferSat);
				pFifoAscan->RemoveItem(-1);
			}
		}
		iFifoCount = pFifoCscan->GetCount();
		if (!m_pDlgHW->m_bFifoCscanDriverThread && iFifoCount)
		{
			while (pFifoCscan->OutCscan(-1, true, pAcqInfo, pStreamHeader, pCscanHeader, pBufferAmp, pBufferAmpTof))
			{
				if (pAcqInfo)
					memcpy(&acqInfo, pAcqInfo, sizeof(acqInfo));
				if (pStreamHeader && pCscanHeader)
					ProcessAcquisitionCscan_0x00010X02(g_pView, acqInfo, pStreamHeader, pCscanHeader, pBufferAmp, pBufferAmpTof);
				pFifoCscan->RemoveItem(-1);
			}
		}
		iFifoCount = pFifoIO->GetCount();
		if (m_pDlgHW->m_bFifoIODriverThread && iFifoCount)
		{
			while (pFifoIO->OutIO(-1, true, pAcqInfo, pStreamHeader, pIOHeader))
			{
				if (pAcqInfo)
					memcpy(&acqInfo, pAcqInfo, sizeof(acqInfo));
				if (pStreamHeader && pIOHeader)
					ProcessAcquisitionIO_1x00010101(g_pView, acqInfo, pStreamHeader, pIOHeader);
				pFifoIO->RemoveItem(-1);
			}
		}
		Sleep(10);
	}
#endif _WIN64
	return 0;
}

bool CAOSPhasedArray::HWEnablePulser256(bool bEnable)
{
	enumAcquisitionState eAcqState;
	CHWDeviceOEMPA *pSlave = (CHWDeviceOEMPA*)m_pHWDeviceOEMPA->GetMatched256DeviceOEMPA();

	if (!pSlave)
		return false;
	if (!g_bFirstShotEnable)
		return true;
	if (bEnable)
		eAcqState = eAcqOn;
	else
		eAcqState = eAcqOff;
	if (!m_pHWDeviceOEMPA->LockDevice(eAcqOff))
		return false;
	if (!pSlave->LockDevice(eAcqOff))
		return false;
	if (!pSlave->UnlockDevice(eAcqState))
		return false;
	if (!m_pHWDeviceOEMPA->UnlockDevice(eAcqOn)) {
		AfxMessageBox( L"Failed to unlock");
		return false;
	}
	return true;
}

bool CAOSPhasedArray::HWEnablePulser(bool bEnable)
{
	enumAcquisitionState eAcqState;
	CHWDeviceOEMPA *pSlave = (CHWDeviceOEMPA*)m_pHWDeviceOEMPA->GetMatched256DeviceOEMPA();

	if (pSlave)
		return HWEnablePulser256(bEnable);
	if (!g_bFirstShotEnable)
		return true;
	if (bEnable)
		eAcqState = eAcqOn;
	else
		eAcqState = eAcqOff;
	if (!m_pHWDeviceOEMPA->LockDevice(eAcqOff))
		return false;
	if (!m_pHWDeviceOEMPA->UnlockDevice(eAcqOn)) {
		AfxMessageBox( L"Failed to unlock");
		return false;
	}
	return true;
}

bool CAOSPhasedArray::Initialize()
{
	bool bRet = true;
	int nCycleCount = 32;
	enumOEMPATrigger eTrig = eOEMPAInternal;

	if (isConnected() == false)	return false;

	if (m_pHWDeviceOEMPA->LockDevice(eAcqOff))
	{

		if (!m_pHWDeviceOEMPA->EnableMultiHWChannel(false))
			bRet = false;
		if (!m_pHWDeviceOEMPA->EnableCscanTof(false))
			bRet = false;
		if (!m_pHWDeviceOEMPA->SetAscanBitSize(e8Bits))
			bRet = false;
		if (!m_pHWDeviceOEMPA->SetExternalTriggerSequence(eDigitalInputOff))
			bRet = false;
		if (!m_pHWDeviceOEMPA->SetExternalTriggerCycle(eDigitalInputOff))
			bRet = false;
		if (!m_pHWDeviceOEMPA->SetTriggerMode(eTrig))
			bRet = false;
		if (!m_pHWDeviceOEMPA->SetAscanRequest(eAscanAll))
			bRet = false;

		if (!m_pHWDeviceOEMPA->UnlockDevice(eAcqOn)) {
			bRet = false;
			AfxMessageBox( L"Failed to unlock");
		}
	}

//	Enable(true);

	return bRet;
}




ULONGLONG CAOSPhasedArray::Save(CUSLFile* pFile)
{

	ULONGLONG ullStartOffset = pFile->GetPosition();
	pFile->Write(m_wFileIdentifier, sizeof m_wFileIdentifier);
	pFile->Write(&theApp.m_fVersion, sizeof theApp.m_fVersion);

	pFile->Seek(ullStartOffset + 0x100, CFile::begin);
	pFile->Write(&m_dRange, sizeof m_dRange);
	pFile->Write(&m_dDelay, sizeof m_dDelay);
	pFile->Write(&m_fTxFocalLength, sizeof m_fTxFocalLength);
	pFile->Write(&m_fElementPitch, sizeof m_fElementPitch);
	pFile->Write(&m_nScanMode, sizeof m_nScanMode);
	pFile->Write(m_fRxFocalLength, sizeof m_fRxFocalLength);
	pFile->Write(&m_nAScanLength, sizeof m_nAScanLength);


	return ullStartOffset;
}


bool CAOSPhasedArray::Retrieve(CUSLFile* pFile)
{
	WCHAR	wFileIdentifier[60];
	float fVersion;

	ULONGLONG ullStartOffset = pFile->GetPosition();
	pFile->Read(wFileIdentifier, sizeof wFileIdentifier);
	pFile->Read(&fVersion, sizeof fVersion);

	if ((wcscmp(wFileIdentifier, m_wFileIdentifier) != 0) || (fVersion > theApp.m_fVersion)) return false;

	pFile->Seek(ullStartOffset + 0x100, CFile::begin);
	pFile->Read(&m_dRange, sizeof m_dRange);
	pFile->Read(&m_dDelay, sizeof m_dDelay);
	pFile->Read(&m_fTxFocalLength, sizeof m_fTxFocalLength);
	pFile->Read(&m_fElementPitch, sizeof m_fElementPitch);
	pFile->Read(&m_nScanMode, sizeof m_nScanMode);
	pFile->Read(m_fRxFocalLength, sizeof m_fRxFocalLength);
	pFile->Read(&m_nAScanLength, sizeof m_nAScanLength);

	return true;
}

bool CAOSPhasedArray::SetFilter()
{
	CHWDeviceOEMPA *pSlave = (CHWDeviceOEMPA*)m_pHWDeviceOEMPA->GetMatched256DeviceOEMPA();

	return true;
}

bool CAOSPhasedArray::setPRF(PVOID pParent)
{
	double dTimeSlotTime = 1.0f / (float)theApp.m_UtUser.m_Global.nPrf;
	bool bRet = true;
	CPhasedArrayProbe* pProbe = (CPhasedArrayProbe*)pParent;

	if (isConnected() == false)	return false;

	if (m_pHWDeviceOEMPA->LockDevice(eAcqOff))
	{
		for (int nFL = 0; nFL < pProbe->getNumberFocalLaws(); nFL++) {

			if (!m_pHWDeviceOEMPA->SetTimeSlot(nFL, dTimeSlotTime))	bRet = false;
		}
		if (!m_pHWDeviceOEMPA->UnlockDevice(eAcqOn)) {
			bRet = false;
			AfxMessageBox( L"Failed to unlock");
		}
	}
	return bRet;
}


bool CAOSPhasedArray::setTxFocalLaws(PVOID pParent)
{
	bool bRet = true;
	CPhasedArrayProbe* pProbe = (CPhasedArrayProbe*)pParent;

	if (isConnected() == false)	return false;

	if (m_pHWDeviceOEMPA->LockDevice(eAcqOff))
	{
		for (int nFL = 0; nFL < pProbe->getNumberFocalLaws(); nFL++) {

			if (!m_pHWDeviceOEMPA->SetEmissionDelays(nFL, pProbe->m_FLTx[nFL].m_dwAperture, pProbe->m_FLTx[nFL].m_fTxDelay))
				bRet = false;
		}
		if (!m_pHWDeviceOEMPA->UnlockDevice(eAcqOn)) {
			bRet = false;
			AfxMessageBox( L"Failed to unlock");
		}
	}
	return bRet;
}

bool CAOSPhasedArray::setRxFocalLaws(PVOID pParent)
{
	bool bRet = true;
	CPhasedArrayProbe* pProbe = (CPhasedArrayProbe*)pParent;
	bool bDDFEnable;

	if (isConnected() == false)	return false;

	if (m_pHWDeviceOEMPA->LockDevice(eAcqOff))
	{
		for (int nFL = 0; nFL < pProbe->getNumberFocalLaws(); nFL++) {

			pProbe->m_FLRx[nFL].m_nFocalTOFCount > 0 ? bDDFEnable = true : bDDFEnable = false;
			if (!m_pHWDeviceOEMPA->SetReceptionFocusing(nFL, pProbe->m_FLRx[nFL].m_nFocalTOFCount, pProbe->m_FLRx[nFL].m_dFocalTOFDelay))	bRet = false;
			if (!m_pHWDeviceOEMPA->SetReceptionDelays(nFL, MAX_APERTURE_COUNT, pProbe->m_FLRx[nFL].m_nFocalTOFCount, pProbe->m_FLRx[nFL].m_dwAperture, &pProbe->m_FLRx[nFL].m_fRxDelay[0][0]))	bRet = false;
			if (!m_pHWDeviceOEMPA->EnableDDF(nFL, bDDFEnable))	bRet = false;
		}
		if (!m_pHWDeviceOEMPA->UnlockDevice(eAcqOn)) {
			bRet = false;
			AfxMessageBox( L"Failed to unlock");
		}
	}
	return bRet;
}

bool CAOSPhasedArray::setWidthDelay(PVOID pParent)
{
	bool bRet = true;
	CPhasedArrayProbe* pProbe = (CPhasedArrayProbe*)pParent;
	enumCompressionType eComp = eCompression;
	double dRdDelay, dRdRange, dRdSamplingFrequency;
	long lPointCount = MinMax(&m_nAScanLength, 512, 8192);
	long lPointFactor;

	if (isConnected() == false)	return false;

	if (m_pHWDeviceOEMPA->LockDevice(eAcqOff))
	{
		theApp.m_UtUser.m_Global.nTimeSlot = 0;

		int nDelay = theApp.m_UtUser.m_TS[0].Adc.nDelay;
		int nWidth = theApp.m_UtUser.m_TS[0].Adc.nWidth;
		for (int nFL = 0; nFL < pProbe->getNumberFocalLaws(); nFL++) {
			theApp.m_UtUser.m_TS[nFL].Adc.nDelay = nDelay;
			theApp.m_UtUser.m_TS[nFL].Adc.nWidth = nWidth;


			pProbe->m_FLRx[nFL].m_dAscanRange = (double)nWidth * 1e-9;
			lPointFactor = (nWidth / 10) / lPointCount;
			if (!m_pHWDeviceOEMPA->SetAscanRangeWithCount(nFL, pProbe->m_FLRx[nFL].m_dAscanRange, eComp, lPointCount, lPointFactor))	bRet = false;

			pProbe->m_FLRx[nFL].m_dAscanDelay = (double)nDelay * 1e-9;
			if (!m_pHWDeviceOEMPA->SetAscanStart(nFL, pProbe->m_FLRx[nFL].m_dAscanDelay))												bRet = false;

		}

		if (!m_pHWDeviceOEMPA->GetSamplingFrequency(eComp, lPointFactor, dRdSamplingFrequency))			bRet = false;

		if (!m_pHWDeviceOEMPA->GetAscanStart(0, &dRdDelay))												bRet = false;
		if (!m_pHWDeviceOEMPA->GetAscanRange(0, &dRdRange, &eComp, &lPointCount, &lPointFactor))		bRet = false;

		if (!m_pHWDeviceOEMPA->UnlockDevice(eAcqOn)) {
			bRet = false;
			AfxMessageBox( L"Failed to unlock");
		}
		for (int nFL = 0; nFL < pProbe->getNumberFocalLaws(); nFL++) {
			theApp.m_UtUser.m_TS[nFL].Adc.fSamplePeriod = (float)(1.0 / dRdSamplingFrequency) / 1e-9f;
		}
	}
	return bRet;
}

bool CAOSPhasedArray::setRectify(PVOID pParent, int nFL)
{
	bool bAlreadyLocked;
	bool bRet = true;
	enumRectification eRect;
	if (isConnected() == false)	return false;

	CPhasedArrayProbe* pProbe = (CPhasedArrayProbe*)pParent;
	eRect = (enumRectification)pProbe->m_nRectification;

	if ((bAlreadyLocked = m_pHWDeviceOEMPA->IsDeviceLocked()) == false) {
		if (!m_pHWDeviceOEMPA->LockDevice(eAcqOff)) return false;
	}

	if (nFL != -1) {
		if (!m_pHWDeviceOEMPA->SetAscanRectification(nFL, eRect))		bRet = false;
	}
	else {
		for (int nCycle = 0; nCycle < m_nCycleCount; nCycle++) {
			if (!m_pHWDeviceOEMPA->SetAscanRectification(nCycle, eRect))		bRet = false;
		}
	}

	if(bAlreadyLocked == false) {
		if (!m_pHWDeviceOEMPA->UnlockDevice(eAcqOn)) {
			bRet = false;
			AfxMessageBox( L"Failed to unlock");
		}
	}

	return bRet;
}


bool CAOSPhasedArray::setFilter(PVOID pParent, int nFL)
{
	bool bAlreadyLocked;
	bool bRet = true;
	double dMax = -1.0e6, dMin = 1.0e6;
	__int16 wArray[34];


	if (isConnected() == false)	return false;

	CPhasedArrayProbe* pProbe = (CPhasedArrayProbe*)pParent;
	ZeroMemory(wArray, sizeof wArray);
	WORD wScale = (WORD)pProbe->m_nFilterGain;

	CFIR* pFIR = &pProbe->m_FIR;
	if (pFIR->m_dAmpFilter != NULL) {
		for (int ii = 0; ii < pFIR->nNumTaps; ii++) {
			if (pFIR->m_dAmpFilter[ii] > dMax) dMax = pFIR->m_dAmpFilter[ii];
			if (pFIR->m_dAmpFilter[ii] < dMin) dMin = pFIR->m_dAmpFilter[ii];
		}
		if ((dMax - dMin) < 0.3) {
			dMax = (double)((int)(dMax*100.0) + 1) / 100.0;
			dMin = (double)((int)(dMin*100.0) - 1) / 100.0;
		}
		else {
			dMax = (double)((int)(dMax*10.0) + 1) / 10.0;
			dMin = (double)((int)(dMin*10.0) - 1) / 10.0;
		}
		if (dMax <= dMin) dMax = dMin + 0.1;

		for (int ii = 0; ii < 33; ii++) {
			wArray[ii] = (WORD)( (pFIR->m_dAmpFilter[ii] * 65535.0) / (dMax - dMin));
		}
	}


	if ((bAlreadyLocked = m_pHWDeviceOEMPA->IsDeviceLocked()) == false) {
		if (!m_pHWDeviceOEMPA->LockDevice(eAcqOff)) return false;
	}
	if (pProbe->m_eFilterType == FILTER_OFF) {
		if (nFL != -1) {
			if (!m_pHWDeviceOEMPA->SetFilterIndex(nFL, eOEMPAFilterOff))	bRet = false;
		}
		else {
			for (int nCycle = 0; nCycle < m_nCycleCount; nCycle++) {
				if (!m_pHWDeviceOEMPA->SetFilterIndex(nCycle, eOEMPAFilterOff))	bRet = false;
			}
		}

	}
	else {

		if (nFL != -1) {
			if (!m_pHWDeviceOEMPA->SetFilterIndex(nFL, eOEMPAFilter1))	bRet = false;
			if (!m_pHWDeviceOEMPA->SetFilter(eOEMPAFilter1, wScale, wArray))	bRet = false;
		}
		else {
			for (int nCycle = 0; nCycle < m_nCycleCount; nCycle++) {
				if (!m_pHWDeviceOEMPA->SetFilterIndex(nCycle, eOEMPAFilter1))	bRet = false;
				if (!m_pHWDeviceOEMPA->SetFilter(eOEMPAFilter1, wScale, wArray))	bRet = false;
			}
		}
	}

	if (bAlreadyLocked == false) {
		if (!m_pHWDeviceOEMPA->UnlockDevice(eAcqOn)) {
			bRet = false;
			AfxMessageBox( L"Failed to unlock");
		}
	}

	return bRet;
}


bool CAOSPhasedArray::setAllHardwareVariables(PVOID pParent)
{
	bool bRet = true;
	double dTemp;
	double dTimeSlotTime = 1.0f / (float)theApp.m_UtUser.m_Global.nPrf;
	WORD wID = 65535;
	DWORD adwHWAperture[4] = { 0 };
	DWORD adwHWRead[4] = { 0 };
//	bool bEnable;
//	enumGateModeTof tof;
//	enumRectification eRect;
//	int iAcqElem;
	enumAcquisitionFlush eAcquisitionFlush = eAutomatic;
	float fArray[128];
	bool bDDFEnable;
	int nFL;

	CPhasedArrayProbe* pProbe = (CPhasedArrayProbe*)pParent;

	enumCompressionType eComp = eCompression;
	long lPointCount;
	long lPointFactor;

	int iMode = 0;
	bool bGateEnable = false;
	double dGateStartWr = 0.0;
	double dGateStopWr = 0.0;
	enumGateModeAmp eGateModeAmpWr = (enumGateModeAmp)iMode;
	enumGateModeTof eGateModeTofWr = (enumGateModeTof)iMode;
	enumRectification eGateRectificationWr = (enumRectification)iMode;
	double dGateThresholdPercentWr = 0.0;

	if (isConnected() == false)	return false;

	if (m_pHWDeviceOEMPA->LockDevice(eAcqOff))
	{
		m_nCycleCount = pProbe->getNumberFocalLaws();

		if (!m_pHWDeviceOEMPA->SetCycleCount(m_nCycleCount)) bRet = false;
			
		int nFiringOrder = pProbe->m_nFiringOrder + 1;

		for (int nCycle = 0; nCycle < m_nCycleCount; ) {

			int nFLStart = nCycle / nFiringOrder;

			for (int nLoop = 0; nLoop < nFiringOrder; nLoop++, nCycle++) {

				nFL = nFLStart + (nLoop * (m_nCycleCount / nFiringOrder));

				wID = nFL;

				m_pHWDeviceOEMPA->SetAllElementEnable(false, adwHWAperture);
				for (int ii = 0; ii < pProbe->m_FLTx[nFL].m_nApertureCount; ii++) {
					if (pProbe->getReverseArray() == false) {
						m_pHWDeviceOEMPA->SetElementEnable(ii + pProbe->m_FLTx[nFL].getFirstElement(), true, adwHWAperture);
					}
					else {
						m_pHWDeviceOEMPA->SetElementEnable((pProbe->getNumberElements()-1) - (ii + pProbe->m_FLTx[nFL].getFirstElement()), true, adwHWAperture);
					}
				}
				CopyMemory(pProbe->m_FLTx[nFL].m_dwAperture, adwHWAperture, sizeof adwHWAperture);
				CopyMemory(pProbe->m_FLRx[nFL].m_dwAperture, adwHWAperture, sizeof adwHWAperture);

				//Cycle
				dTemp = (double)pProbe->m_fDigitalGain;
				if (!m_pHWDeviceOEMPA->SetGainDigital(nCycle, dTemp))																			bRet = false;
				if (!m_pHWDeviceOEMPA->SetBeamCorrection(nCycle, m_fBeamCorrectionGain))														bRet = false;

				lPointCount = MinMax(&m_nAScanLength, 512, 8192);
				pProbe->m_FLRx[nFL].m_dAscanRange = (double)theApp.m_UtUser.m_TS[ nFL ].Adc.nWidth * 1e-9;
				lPointFactor = (long)((float)theApp.m_UtUser.m_TS[nFL].Adc.nWidth / 4) / lPointCount;
				lPointFactor = 0;
				if (!m_pHWDeviceOEMPA->SetAscanRangeWithCount(nCycle, pProbe->m_FLRx[nFL].m_dAscanRange, eComp, lPointCount, lPointFactor))	bRet = false;

				pProbe->m_FLRx[nFL].m_dAscanDelay = (double)theApp.m_UtUser.m_TS[nFL].Adc.nDelay * 1e-9;
				if (!m_pHWDeviceOEMPA->SetAscanStart(nCycle, pProbe->m_FLRx[nFL].m_dAscanDelay))												bRet = false;

				if (!m_pHWDeviceOEMPA->SetTimeSlot(nCycle, dTimeSlotTime))																		bRet = false;
				if (!setRectify(pProbe, nCycle))																								bRet = false;
//				if (!m_pHWDeviceOEMPA->EnableDAC(nCycle, m_bDACEnable))																			bRet = false;
//				if (!m_pHWDeviceOEMPA->SetDACSlope(nCycle, m_nDACCount, &m_dDACTime, &m_fDACSlope))												bRet = false;
				if (!m_pHWDeviceOEMPA->EnableAscanMaximum(nCycle, true))																		bRet = false;
				if (!m_pHWDeviceOEMPA->EnableAscanMinimum(nCycle, false))																		bRet = false;
				if (!m_pHWDeviceOEMPA->EnableAscanSaturation(nCycle, false))																	bRet = false;
				if (!m_pHWDeviceOEMPA->EnableAscan(true))																						bRet = false;
				if (!m_pHWDeviceOEMPA->SetAscanAcqIdChannelProbe(nCycle, wID))																	bRet = false;
				if (!m_pHWDeviceOEMPA->SetAscanAcqIdChannelScan(nCycle, wID))																	bRet = false;
				if (!m_pHWDeviceOEMPA->SetAscanAcqIdChannelCycle(nCycle, wID))																	bRet = false;

				for (int iGateIndex = 0; iGateIndex < g_iOEMPAGateCountMax; iGateIndex++) {
					if (!m_pHWDeviceOEMPA->SetGateModeThreshold(nCycle, iGateIndex, bGateEnable, eGateModeAmpWr, eGateModeTofWr, eGateRectificationWr, dGateThresholdPercentWr)) bRet = false;
				}


				//Cyle Pulser
				if (!m_pHWDeviceOEMPA->SetEmissionWedgeDelay(nCycle, m_nCycleCount, pProbe->m_FLTx[nFL].m_dWedgeDelay))						bRet = false;
				if (!m_pHWDeviceOEMPA->SetMultiplexerEnable(nCycle, adwHWAperture))																bRet = false;
				if (!m_pHWDeviceOEMPA->SetEmissionDelays(nCycle, adwHWAperture, pProbe->m_FLTx[nFL].m_fTxDelay))								bRet = false;
				if (!m_pHWDeviceOEMPA->SetEmissionWidths(nCycle, adwHWAperture, pProbe->m_FLTx[nFL].m_fPulseWidth))							bRet = false;
				if (!m_pHWDeviceOEMPA->SetEmissionEnable(nCycle, adwHWAperture))																bRet = false;

				//Cycle Receiver
				if (!m_pHWDeviceOEMPA->SetReceptionWedgeDelay(nCycle, m_nCycleCount, pProbe->m_FLRx[nFL].m_dWedgeDelay))						bRet = false;
				if (!m_pHWDeviceOEMPA->SetReceptionEnable(nCycle, adwHWAperture))																bRet = false;
				if (!m_pHWDeviceOEMPA->EnableMultiHWChannelAcquisition(nCycle, m_nCycleCount, false))											bRet = false;

				pProbe->m_FLRx[nFL].m_nFocalTOFCount > 0 ? bDDFEnable = true : bDDFEnable = false;
				if (!m_pHWDeviceOEMPA->SetReceptionFocusing(nCycle, pProbe->m_FLRx[nFL].m_nFocalTOFCount, pProbe->m_FLRx[nFL].m_dFocalTOFDelay))	bRet = false;
				if (!m_pHWDeviceOEMPA->SetReceptionDelays(nCycle, MAX_APERTURE_COUNT, pProbe->m_FLRx[nFL].m_nFocalTOFCount, pProbe->m_FLRx[nFL].m_dwAperture, &pProbe->m_FLRx[nFL].m_fRxDelay[0][0]))	bRet = false;
				if (!m_pHWDeviceOEMPA->EnableDDF(nCycle, bDDFEnable))	bRet = false;

				ZeroMemory(fArray, sizeof fArray);
				if (!m_pHWDeviceOEMPA->SetReceptionGains(nCycle, adwHWAperture, fArray))														bRet = false;
				if (!m_pHWDeviceOEMPA->SetGainAnalog(nCycle, pProbe->m_fAnalogueGain))															bRet = false;
				if (!setFilter(pProbe, nCycle))																								bRet = false;
			}
		}
		if (!m_pHWDeviceOEMPA->UnlockDevice(eAcqOn)) {
			bRet = false;
			AfxMessageBox( L"Failed to unlock");
		}
	}

	Enable(true);

	return bRet;
}


bool CAOSPhasedArray::HWToFile()
{
	CString strFileFullPathName = L"*.txt";
	strFileFullPathName = UTKernel_GetFolderDataFiles(L"OEMPA");
	strFileFullPathName += L"*.txt";
	CFileDialog filedialog(FALSE, L"*.txt", strFileFullPathName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||", NULL, 0);
	CString string;
	CString stringSlave;
	DWORD dwAttributes;
	bool bRet = false;
	CHWDeviceOEMPA *pSlave = (CHWDeviceOEMPA*)m_pHWDeviceOEMPA->GetMatched256DeviceOEMPA();

	if (!m_pHWDeviceOEMPA)	return false;
	if (filedialog.DoModal() != IDOK)	return false;

	string = filedialog.GetFileExt();
	if ((string != L"txt") && (string != L"bin"))	return false;

	CWaitCursor wait;
	string = filedialog.GetPathName();
	dwAttributes = GetFileAttributes(string);
	if (dwAttributes != INVALID_FILE_ATTRIBUTES)
		DeleteFile(string);

	Enable(false);
	if (!pSlave)
		bRet = OEMPA_ReadHWWriteFile(m_pHWDeviceOEMPA, g_iOEMPACycleCountMax, g_iOEMPAApertureElementCountMax, g_iOEMPAFocalCountMax, g_iOEMPADACCountMax, string.GetBuffer());
	else {
		CFileDialog filedialogSlave(FALSE, L"*.txt", strFileFullPathName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Text Files (*.txt)|*.txt|Binary Files (*.bin)|*.bin|All Files (*.*)|*.*||", NULL, 0);
		//2 files are required for the 256:256.
		EditFile(string.GetBuffer(), false);
		if (filedialogSlave.DoModal() != IDOK)
			return false;
		stringSlave = filedialogSlave.GetFileExt();
		stringSlave.MakeLower();
		if ((stringSlave != L"txt") && (stringSlave != L"bin"))
			return false;
		CWaitCursor wait;
		stringSlave = filedialogSlave.GetPathName();
		dwAttributes = GetFileAttributes(stringSlave);
		if (dwAttributes != INVALID_FILE_ATTRIBUTES)
			DeleteFile(stringSlave);
		HWEnablePulser256(false);
		bRet = OEMPA_ReadHWWriteFile(m_pHWDeviceOEMPA, g_iOEMPACycleCountMax, g_iOEMPAApertureElementCountMax, g_iOEMPAFocalCountMax, g_iOEMPADACCountMax, string.GetBuffer());
		bRet = OEMPA_ReadHWWriteFile(pSlave, g_iOEMPACycleCountMax, g_iOEMPAApertureElementCountMax, g_iOEMPAFocalCountMax, g_iOEMPADACCountMax, stringSlave.GetBuffer()) && bRet;
	}

	if (bRet)
		EditFile(string.GetBuffer(), false);
	if (pSlave && bRet)
		EditFile(stringSlave.GetBuffer(), false);
	UTKernel_SetFolderDataFiles(L"OEMPA", string.GetBuffer());

	Enable(true);

	return true;

}

void CAOSPhasedArray::EditFile(wchar_t *pFile, bool bCloseWaiting)
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
	TRACE0("AOS Wait to close file\n");
	if (bCloseWaiting)
		WaitForSingleObject(execinfo.hProcess, INFINITE);
	TRACE0("AOS Wait completed\n");
}


