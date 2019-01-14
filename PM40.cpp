#include "stdafx.h"
#include "PM40.h"
#include "PM40Packets.h"
#include "PM40Utils.h"
#include "USLScanner.h"

CPM40Packets g_PM40Packets;
CPM40Utils g_PM40Utils;

extern VOID PM40PacketReadWrite(PTHREAD_PARAMS params);
extern UINT CPM40PendForInterrupt(LPVOID pParam);



CPM40::CPM40()
{
	m_pcfgParams = new CFG_PARAMS;
	ZeroMemory(m_pcfgParams, sizeof CFG_PARAMS);

	//! Setup the defaults
	m_pcfgParams->cardNum = DEFAULT_CARD_NUMBER;
	m_pcfgParams->bPacketMode = DEFAULT_PACKET_MODE;
	m_pcfgParams->BufferSize = DEFAULT_BUFFER_SIZE;
	m_pcfgParams->MaxPacketSize = DEFAULT_MAX_PACKET_SIZE;
	m_pcfgParams->DMAEngineOffset = DEFAULT_DMA_ENGINE_OFFSET;
	m_pcfgParams->Iterations = 1;									 //(UINT32)DEFAULT_ITERATIONS;
	m_pcfgParams->Seconds = (UINT32)DEFAULT_SECONDS;
	m_pcfgParams->NumberThreads = DEFAULT_NUMBER_THREAD;
	m_pcfgParams->MaxDMAAddress = DEFAULT_ADDR_PACKET_DMA_MAX;
	m_pcfgParams->bVerbose = DEFAULT_VERBOSE;
	m_pcfgParams->CardAddress = 0;
	m_pcfgParams->Index = -1;
	m_pcfgParams->Type = -1;
	m_pcfgParams->bDebug = DEFAULT_DEBUG;
	m_pcfgParams->bQuiet = DEFAULT_QUIET;
	m_pcfgParams->DataPatternType = PATTERN_INCREMENTING;
	m_pcfgParams->DataSeedSize = 1;
	m_pcfgParams->DataSeedPattern = DEFAULT_SEED_PATTERN;
	m_pcfgParams->UserInfoSeedSize = 1;
	m_pcfgParams->UserInfoSeedPattern = DEFAULT_SEED_PATTERN;
	//	m_pcfgParams->CmdArgc = argc;
	//	m_pcfgParams->CmdArgv = &(argv[1]);
	m_pcfgParams->bStopOnError = DEFAULT_STOP_ON_ERROR;
	m_pcfgParams->iPass = 0;
	m_pcfgParams->bRandomize = DEFAULT_RANDOMIZE;
	m_pcfgParams->bPatternSpecified = FALSE;
	m_pcfgParams->StatsMultiplier = 1;
	m_pcfgParams->bSecondRefresh = FALSE;
	m_pcfgParams->bBlockingAPIs = TRUE;
	m_pcfgParams->UserIntTimeout = -1;
	m_pcfgParams->bAppendData = FALSE;
	m_pcfgParams->bRunning = TRUE;

	m_WaitParams.hThreadHandle = 0;

	if (!InitializeCriticalSectionAndSpinCount(&m_pcfgParams->CriticalSection, 0x00000400))
	{
		fprintf(stderr, "InitializeCriticalSectionAndSpinCount Failed, returned %d", GetLastError());
	}

	m_nMaxMailboxWait = 0;
	m_nMailboxFailed = 0;


}


CPM40::~CPM40()
{
	CloseDevice();
	if (m_pcfgParams) {
		delete m_pcfgParams;
	}
}

bool CPM40::FindDevice()
{
	int nStatus;

	if ((nStatus = ConnectToBoard(m_pcfgParams->cardNum, &m_DmaInfo)) == STATUS_SUCCESSFUL)
	{
		//! Make sure the DLL is compatible.
		if ((m_DmaInfo.DLLMajorVersion == VER_MAJOR_NUM) &&
			(m_DmaInfo.DLLMinorVersion == VER_MINOR_NUM))
		{
			//! Set the DMA Registers BAR Number.
			m_pcfgParams->DMA_BAR = m_DmaInfo.DMARegistersBAR;

			//! Indicate we found a board.
			m_bFoundPCIBoard = TRUE;
			//! Get the DMA Capabilities from the card.
			GetDMAEngineCap(m_pcfgParams->cardNum, m_pcfgParams->DMAEngineOffset, &m_DMACap);
			switch (m_DMACap.DmaCapabilities & DMA_CAP_STATS_SCALING_FACTOR_MASK)
			{
			case DMA_CAP_STATS_SCALING_FACTOR_8:
				m_pcfgParams->StatsMultiplier = 8;
				break;
			case DMA_CAP_STATS_SCALING_FACTOR_4:
				m_pcfgParams->StatsMultiplier = 4;
				break;
			case DMA_CAP_STATS_SCALING_FACTOR_2:
				m_pcfgParams->StatsMultiplier = 2;
				break;
			default:
				break;
			}

			GetBoardCfg(0, &m_BoardConfig);
			EnableMailbox();
		}
		else
		{
			m_ErrorMsg.Format(L"Error: The Installed DmaDriverDLL is not at the same revision level\nThe DLL is Version = %d.%d.%d.%d\nThe rquired version = %d.%d",
				m_DmaInfo.DLLMajorVersion, m_DmaInfo.DLLMinorVersion,
				m_DmaInfo.DLLSubMinorVersion, m_DmaInfo.DLLBuildNumberVersion,
				VER_MAJOR_NUM, VER_MINOR_NUM);
			MessageBox(NULL, m_ErrorMsg, L"Error", MB_ICONERROR);
		}
	}
	else {
		switch (nStatus) {
		case STATUS_INCOMPLETE:	m_ErrorMsg.Format(L"Incomplete message to connect to board");
			break;
		default: m_ErrorMsg.Format(L"Unrecognised error message #%d", nStatus);
			break;
		}
	}
	return nStatus;
}

BOOL CPM40::IsConnected()
{
	return m_bFoundPCIBoard;
}


void CPM40::CloseDevice()
{
	DeleteCriticalSection(&m_pcfgParams->CriticalSection);

	if (IsConnected() == TRUE) {

	}

}

CString &CPM40::getDllVersion()
{
	static CString strVersion;
	WCHAR *Array = new WCHAR[100];

	GetDLLVersion(Array, 100);
	strVersion.Format(L"%s", Array);

	delete Array;
	return strVersion;
}

CString &CPM40::getDriverVersion()
{
	static CString strVersion;
	WCHAR *Array = new WCHAR[100];
	ZeroMemory(Array, 100);

//	GetDriverVersion(Array, 100);
	strVersion.Format(L"%s", Array);

	delete Array;
	return strVersion;
}

CString &CPM40::getAllInterruptCounts()
{
	static CString strVersion;
	WCHAR *Array = new WCHAR[100];

//	GetAllInterruptCounts(0, Array, 100);
	strVersion.Format(L"%s", Array);

	delete Array;
	return strVersion;
}

void CPM40::StartADCCommsToBoard()
{
	if (m_bFoundPCIBoard != true) return;
	m_pcfgParams->Type = TYPE_ENHANCED_PACKET;

	if (m_pcfgParams->Type == TYPE_PACKET)
	{
		if (m_DmaInfo.PacketRecvEngineCount || m_DmaInfo.PacketSendEngineCount)
		{
			//! Setup User Interrupt support.
			StartWaitThreads(m_pcfgParams, &m_WaitParams);
		}
		else
		{
			m_ErrorMsg.Format(L"Error: No Packet DMA Engines found.  command not applicable!\n");
			m_pcfgParams->iPass = ERROR_BAD_PARAMETER;
		}
	}
	else if (m_pcfgParams->Type == TYPE_ENHANCED_PACKET)
	{
		if (m_DmaInfo.AddressablePacketMode)
		{
			if (m_DmaInfo.PacketRecvEngineCount || m_DmaInfo.PacketSendEngineCount)
			{
				//! Setup User Interrupt support.
				StartWaitThreads(m_pcfgParams, &m_WaitParams);
			}
			else
			{
				m_ErrorMsg.Format(L"Error: No Read or Write Packet DMA Engines found,  command not applicable!\n");
				m_pcfgParams->iPass = ERROR_BAD_PARAMETER;
			}
		}
		else
		{
			m_ErrorMsg.Format(L"Error: No Addressable Packet Mode DMA Engines found,  command not applicable!\n");
			m_pcfgParams->iPass = ERROR_BAD_PARAMETER;
		}
	}
}

void CPM40::StartWaitThreads(PCFG_PARAMS     pcfgParams, PWAIT_THREAD_PARAMS pWaitParams)
{
//	RegisterInterruptCallback();
//	return;

	pWaitParams->pcfgParams = pcfgParams;

	pWaitParams->hThreadHandle = CreateThread(
		(LPSECURITY_ATTRIBUTES)NULL,                                    //! No security.
		(DWORD)0,                                                       //! Same stack size.
		(LPTHREAD_START_ROUTINE)WaitThreadProc,                         //! Thread procedure.
		(LPVOID)pWaitParams,                                            //! Pointer the config parameters.
		(DWORD)0,                                                       //! Start immediately.
		(LPDWORD)NULL);                                                 //! Thread ID.


	if (pWaitParams->hThreadHandle == NULL)
	{
		fprintf(stderr, "CreateThread Failed, returned %d", GetLastError());
	}
}


void CPM40::FinishWaitThreads()
{
	if (m_WaitParams.hThreadHandle != 0)
	{
		int i = 5;
		UserIRQCancel(m_pcfgParams->cardNum);
		CloseHandle(m_WaitParams.hThreadHandle);
		while (m_WaitParams.hThreadHandle != 0)
		{
			Sleep(1);
			if (!i--)
			{
				break;
			}
		}
	}

}


CString& CPM40::getLastErrorMessage()
{
	return m_ErrorMsg;
}

bool CPM40::IsCardFound()
{
	return m_bFoundPCIBoard;
}


int CPM40::ReadBar0(UINT64 n64CardAddrs)
{
	int nArray[32];
	int	nBoard = 0;
	int nRdWr = 1;	//Read;
	int nBarNum = 0;
	UINT8* pcArray = (UINT8*)nArray;
	UINT64 n64MemOffset = 0;
	UINT64 n64Length = 4;
	STAT_STRUCT Status;

	if (IsCardFound() == true) {

		ZeroMemory((void*)&Status, sizeof Status);

		int nStatus = DoMem(nBoard, nRdWr, nBarNum, pcArray, n64MemOffset, n64CardAddrs, n64Length, &Status);

		if (nStatus) {
			m_ErrorMsg.Format(L"Failed to Read from BAR %d\nAddrs %x Valuse %x", nBarNum, n64CardAddrs, nArray[0]);
		}

	}
	return nArray[0];
}

bool CPM40::WriteBar0(UINT64 n64CardAddrs, int nValue)
{
	int	nBoard = 0;
	int nRdWr = 0;	//Write;
	int nBarNum = 0;
	UINT64 n64MemOffset = 0;
	UINT64 n64Length = 4;
	STAT_STRUCT Status;

	if (IsCardFound() == true) {

		ZeroMemory((void*)&Status, sizeof Status);

		int nStatus = DoMem(nBoard, nRdWr, nBarNum, (UINT8*)&nValue, n64MemOffset, n64CardAddrs, n64Length, &Status);

		if (nStatus) {
			m_ErrorMsg.Format(L"Failed to write to BAR %d\nAddrs %x Value %x", nBarNum, n64CardAddrs, nValue);
			return false;
		}
		return true;
	}
	return false;

}

int CPM40::getDMACap()
{
	return m_DMACap.DmaCapabilities;
}

bool CPM40::IsDMAEnginePresent()
{
	if (m_DMACap.DmaCapabilities & 1) {
		return true;
	}
	else {
		return false;
	}
}

CString *CPM40::getDMADirection()
{
	static CString Buff;

	if (m_DMACap.DmaCapabilities & 1) {
		switch ((m_DMACap.DmaCapabilities >> 1) & 0x003) {
		case 0: Buff = L"Write to card";
			break;
		case 1: Buff = L"Read from card";
			break;
		case 2: Buff = L"Reserved";
			break;
		case 3: Buff = L"Write and Read";
			break;
		}
	}
	else {
		Buff = L"N/A";
	}

	return &Buff;
}

CString *CPM40::getDMAEngineType()
{
	static CString Buff;

	if (m_DMACap.DmaCapabilities & 1) {
		switch ((m_DMACap.DmaCapabilities >> 4) & 0x003) {
		case 0: Buff = L"Block DMA";
			break;
		case 1: Buff = L"FIFO Packet DMA";
			break;
		case 2: Buff = L"Addressable Packet DMA";
			break;
		case 3: Buff = L"FIFO and Addressable Packet DMA";
			break;
		}
	}
	else {
		Buff = L"N/A";
	}

	return &Buff;
}

CString *CPM40::getDMAEngineNumber()
{
	static CString Buff;

	if (m_DMACap.DmaCapabilities & 1) {
		Buff.Format(L"%d", (m_DMACap.DmaCapabilities >> 8) & 0x0ff);
	}
	else {
		Buff = L"N/A";
	}
	return &Buff;

}

CString *CPM40::getDMAMemorySize()
{
	static CString Buff;

	if (m_DMACap.DmaCapabilities & 1) {
		Buff.Format(L"%x h", (m_DMACap.DmaCapabilities >> 16) & 0x0ff);
	}
	else {
		Buff = L"N/A";
	}
	return &Buff;

}

void CPM40::ReadDMA()
{
	INT32	status = 0;

	m_pcfgParams->Type = TYPE_ENHANCED_PACKET;
	if (m_DmaInfo.AddressablePacketMode)
	{
		if (m_DmaInfo.PacketRecvEngineCount || m_DmaInfo.PacketSendEngineCount)
		{
//			StartWaitThreads(m_pcfgParams, &m_WaitParams);
			status = g_PM40Packets.stressPacketRead(m_pcfgParams);
//			FinishWaitThreads();
		}
		else
		{
			m_ErrorMsg.Format(L"Error: No Read or Write Packet DMA Engines found\n");
			m_pcfgParams->iPass = ERROR_BAD_PARAMETER;
		}
	}
	else
	{
		m_ErrorMsg.Format(L"Error: No Addressable Packet Mode DMA Engines found\n");
		m_pcfgParams->iPass = ERROR_BAD_PARAMETER;
	}

}

#define I2CVIAPIE			0x01
#define INTMAILBOXFAILED	0x02
#define INTMAILBOXEMPTIED	0x04

void CPM40::EnableMailbox()
{
	int nMask = I2CVIAPIE;
	WriteBar0(0x8824, nMask);	//0x8024

	nMask = ReadBar0(0x8824);	//0x8024
}

int CPM40::MailboxTx(int nValue, int *pnCheckSum)
{
	int timeout = 0;

	if (m_nMailboxFailed > 0) {
		MessageBox(NULL, _T("Mailbox has failed"), _T("Error"), NULL);
		m_nMailboxFailed = -1;
		return FALSE;
	}

	while (ReadBar0(0x8824) & 0x04) {
		timeout++;
		if (timeout>32000) {
			m_nMailboxFailed++;
			return FALSE;
		};
	};
	if (m_nMaxMailboxWait < timeout) m_nMaxMailboxWait = timeout;
	WriteBar0(0x8820, nValue);				//0x8020

	if (nValue & 0x80) {
		*pnCheckSum = nValue;
	}
	else {
		*pnCheckSum += nValue;
	}
	if (nValue == 0xff) m_nTotalCommandsSent++;

	return TRUE;
}


void CPM40::ConfigureADC()
{
	theApp.m_LastSettings.nAdcCardType = USL_PR40;
	theApp.m_UtUser.Pr30Command4(&theApp.m_UtUser.m_TS[0].Pr30);	//SetTimeSlot

	StartADCCommsToBoard();
}

void CPM40::ConfigureWidthDelay(TimeSlotData *pTS, int nBoardNumber)
{

	for (int nDivisor = 1; nDivisor < 30; nDivisor++) {
		float fSamplePeriod = 1.0f / (250.0e-3f / (float)nDivisor);
		int nNumberSamples = (int)((float)pTS->Adc.nWidth / fSamplePeriod);
		
		if (nNumberSamples < 8192) {
			pTS->Adc.fSamplePeriod = fSamplePeriod;
			pTS->Adc.nAcqLength = max(nNumberSamples, 1);
			pTS->Adc.nClkDivisor = nDivisor;
			theApp.m_UtUser.Pr30Command4(&pTS->Pr30);	//SetTimeSlot
			theApp.m_UtUser.Pr30Command4Ah(pTS);
			theApp.m_UtUser.Pr30Command4Bh(pTS);
			theApp.m_UtUser.Pr30Command4Dh(pTS);
			break;
		}
	}
}

void CPM40::ArmADC()
{
	WriteBar0(0x801C, 0xA5);
	m_dArmedAt = theApp.getHighResTime();
}

#define	ADC_DATA_AVAILABLE		0x0001
#define TRIGGER_OCCURED			0x0002
#define ACQ_ENDED_ARRAY_LENGTH	0x0004
#define ACQ_ENDED_OVERFLOW		0x0008
#define ACQ_ENDED_PRF_TERMINATE	0x0010

bool CPM40::IsConversionComplete(int *nTS)
{
	CString Buff;
	int nFlag = ReadBar0(0x8000);

	if ((nFlag & (ADC_DATA_AVAILABLE | TRIGGER_OCCURED)) == (ADC_DATA_AVAILABLE | TRIGGER_OCCURED)) {
		m_nConversionCount++;
		m_dElapsedTime = theApp.m_PM40User.getElapsedSinceLastArmed();
		if (nTS) {
			*nTS = ((nFlag >> 8) & 0x0ff) - 1;
			if (*nTS == -1) {
				Buff.Format(L"%04x", nFlag);
			}
		}
		return true;
	}
	return false;
}

#define PM30_MAILBOX_EMPTY	0x010000
#define SPC_MAILBOX_IRQ		0x020000
#define ADC1_IRQ			0x040000
#define ADC2_IRQ			0x080000

bool CPM40::IsMyIrq(int nMask)
{
	int nReg = ReadBar0(0x8880);	//0x8080
	if (nReg & nMask) {
		return true;
	}
	return false;
}

void CPM40::ClearMyIrq(int nMask)
{
	WriteBar0(0x8880, nMask);	//0x8080
}

int CPM40::getConversionCount()
{
	return m_nConversionCount;
}

double CPM40::getElapsedTime()
{
	return m_dElapsedTime;
}

double CPM40::getElapsedSinceLastArmed()
{
	return theApp.getHighResTime() - m_dArmedAt;
}

int CPM40::IncrementTimedOutCount()
{
	return ++m_nTimedOutCount;
}
int CPM40::getTimedOutCount()
{
	return m_nTimedOutCount;
}


void WaitThreadProc(PWAIT_THREAD_PARAMS pWaitParams)
{
	PCFG_PARAMS         pcfgParams = pWaitParams->pcfgParams;
	UINT32              status;
	THREAD_PARAMS		ThreadParams;
	int nSlot;

	for (nSlot = 0; nSlot < 256; nSlot++) theApp.m_UtUser.m_TS[nSlot].nIrqCount = 0;

	ZeroMemory(&ThreadParams, sizeof ThreadParams);
	ThreadParams.pcfgParams = pcfgParams;
	ThreadParams.Direction = DMA_CAP_CARD_TO_SYSTEM;
	ThreadParams.cardNum = 0;
	ThreadParams.DMAEngineOffset,
	ThreadParams.UserInfo = 0;
	ThreadParams.Iterations = 1;
	ThreadParams.MaxPacketSize = 16384;

	g_PM40Utils.BufferAllocAndSet(pcfgParams, &ThreadParams);

	status = SetupPacketMode(
		pcfgParams->cardNum,
		pcfgParams->DMAEngineOffset,
		(PUINT8)NULL,
		(PUINT32)NULL,
		NULL,
		PACKET_MODE_ADDRESSABLE,
		ThreadParams.MaxPacketSize);
	if (status != STATUS_SUCCESSFUL)
	{
		return;
	}

	theApp.m_PM40User.ClearMyIrq(ADC1_IRQ | ADC2_IRQ);
	theApp.m_PM40User.ArmADC();

	int nPollingIrq = 0;

	while (pcfgParams->bRunning)
	{
		switch (nPollingIrq) {
		case 0:
			if (theApp.m_PM40User.IsConversionComplete(&nSlot)) {
				theApp.m_PM40User.ReadTrace(&ThreadParams, nSlot);
				theApp.m_PM40User.ArmADC();
			}
			if (theApp.m_PM40User.getElapsedSinceLastArmed() > 0.1) {
				theApp.m_PM40User.IncrementTimedOutCount();
				theApp.m_PM40User.ArmADC();
			}
			break;
		case 1:
			switch (status = UserIRQWait(pcfgParams->cardNum, 20)) {
			case 0:
				if (theApp.m_PM40User.IsMyIrq(ADC1_IRQ | ADC2_IRQ) && theApp.m_PM40User.IsConversionComplete(&nSlot)) {
					theApp.m_PM40User.ReadTrace(&ThreadParams, nSlot);
					theApp.m_PM40User.ClearMyIrq(ADC1_IRQ | ADC2_IRQ);
					theApp.m_PM40User.ArmADC();
				}
				break;
			case ERROR_SEM_TIMEOUT:
				theApp.m_PM40User.IncrementTimedOutCount();
				theApp.m_PM40User.ClearMyIrq(ADC1_IRQ | ADC2_IRQ);
				theApp.m_PM40User.ArmADC();
				break;
			default:
				break;
			}
			break;
		}
	}

	delete ThreadParams.Buffer;

	pWaitParams->hThreadHandle = 0;
}


VOID PM40PacketReadWrite(PTHREAD_PARAMS params)
{
	UINT64		CardOffset;
	UINT32		TransferSize;
	UINT32		Mode = 0;
	UINT32		status;

	CardOffset = params->CardOffset;
	TransferSize = params->MaxPacketSize;

	if (params->bFIFO)
	{
		Mode = READ_WRITE_MODE_FLAG_FIFO;
	}


	if (params->Direction == DMA_CAP_SYSTEM_TO_CARD)
	{

		status = PacketWriteEx(
			params->cardNum,
			params->DMAEngineOffset,
			params->UserInfo,
			CardOffset,
			Mode,
			(PUINT8)params->Buffer,
			TransferSize);

		if (status != 0)
		{
			return;
		}
	}
	else
	{
		params->UserInfo = 0;
		status = PacketReadEx(
			params->cardNum,
			params->DMAEngineOffset,
			&params->UserInfo,
			CardOffset,
			Mode,
			(PUINT8)params->Buffer,
			&TransferSize);

		if (status != 0)
		{
			return;
		}
		params->PacketCount++;
	}
	params->Running = FALSE;
}

VOID CPM40::ReadTrace(PTHREAD_PARAMS params, int nSlot)
{
	UINT64		CardOffset;
	UINT32		TransferSize;
	UINT32		Mode = 0;
	UINT32		status;
	bool		bLogFlag;

	float fDelayns = (float)theApp.m_UtUser.m_TS[nSlot].Adc.nDelay + theApp.m_Scope.m_fMainBangConstant * 1e9f;
	TransferSize = theApp.m_UtUser.m_TS[nSlot].Adc.nAcqLength * sizeof(__int16);
	CardOffset = (int)(fDelayns / theApp.m_UtUser.m_TS[nSlot].Adc.fSamplePeriod) * sizeof(__int16);
	params->UserInfo = 0;

#define MAX_PKT_SIZE 4096

	for(UINT32 nAddrsOffset = 0; nAddrsOffset<TransferSize;nAddrsOffset += MAX_PKT_SIZE) {
		UINT32 nPktSize = TransferSize - nAddrsOffset;
		if (nPktSize > MAX_PKT_SIZE) nPktSize = MAX_PKT_SIZE;

		status = PacketReadEx(
			params->cardNum,
			params->DMAEngineOffset,
			&params->UserInfo,
			CardOffset + nAddrsOffset,
			Mode,
			(PUINT8)params->Buffer + nAddrsOffset,
			&nPktSize);

		if (status != 0)
		{
			return;
		}
	}
	if (theApp.m_Scope.m_nPlayRecording[nSlot] == 0) {
		theApp.m_UtUser.m_TS[nSlot].Pr30.nOutputSelect == 2 ? bLogFlag = true : bLogFlag = false;
		theApp.m_Scope.FillScopeArrayShort(nSlot, (__int16*)params->Buffer, theApp.m_UtUser.m_TS[nSlot].Adc.nAcqLength, bLogFlag, theApp.m_UtUser.m_TS[nSlot].Adc.nAverages);
	}
	theApp.m_UtUser.m_TS[nSlot].nIrqCount++;

	theApp.m_UtUser.ProcessTrace(theApp.m_Scope.m_RFTrace[nSlot], nSlot);
}


BOOL CPM40::RegisterInterruptCallback()
{
	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	RegisterDeviceInterruptCallback(0, &m_hEvent);

	m_bIntCallbackThreadEnabled = true;
	m_pThread = AfxBeginThread(&CPM40PendForInterrupt, this, THREAD_PRIORITY_TIME_CRITICAL, 0, NULL);

	return true;
}


void CPM40::DeRegisterInterruptCallback()
{

	if (m_bIntCallbackThreadEnabled) {
		TRACE0("Wait for CPM40 Thread to finish\n");
		m_bIntCallbackThreadEnabled = false;
		WaitForSingleObject(m_pThread, INFINITE);
		TRACE0("CPM40 Object finished\n");
	}

}

UINT CPM40PendForInterrupt(LPVOID pParam)
{
	CPM40* pParent = (CPM40 *)pParam;

	theApp.m_PM40User.ClearMyIrq(ADC1_IRQ | ADC2_IRQ);
	theApp.m_PM40User.ArmADC();
//	UserIRQWait(0, 20);

	pParent->m_bIntCallbackThreadFinished = FALSE;
	while (pParent->m_bIntCallbackThreadEnabled == TRUE) {
		if (WaitForSingleObject(pParent->m_hEvent, 100) == WAIT_OBJECT_0) {
			pParent->m_nEventsCount++;

			theApp.m_PM40User.ClearMyIrq(ADC1_IRQ | ADC2_IRQ);
			theApp.m_PM40User.ArmADC();

			ResetEvent(pParent->m_hEvent);
		}
	}
	pParent->m_bIntCallbackThreadFinished = TRUE;
	TRACE0("SI10x64 Interrupt callback finished\n");

	return 0;
}
