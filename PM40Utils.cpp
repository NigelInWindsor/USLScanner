#include "stdafx.h"
#include "PM40.h"
#include "PM40Utils.h"
#include "PM40Packets.h"

extern	CPM40Packets g_PM40Packets;


// -------------------------------------------------------------------------
// Global variables
// -------------------------------------------------------------------------
UINT64          g_idleTime;
UINT64          g_kernelTime;
UINT64          g_userTime;

#ifndef WIN32   // Linux Version
static FILE *   ghProcStat;
UINT64          g_niceTime;
UINT64          g_iowaitTime;
PCFG_PARAMS     gpcfgParams;
#endif          // Linux version


#ifndef WIN32   // Linux Version
static void     SignalHandler(int sig);
#endif          // Linux version
/*
INT32 SetupPacketGenerator(
	PCFG_PARAMS             pcfgParams,
	UINT8                   DMAEngineOffset,
	BOOLEAN                 Direction,
	PPACKET_GENCHK_STRUCT   pPacketGenChk);
*/

#if DBG
UINT32
GetPacketCheckerErrors(
PCFG_PARAMS             pcfgParams,
UINT8                   DMAEngineOffset);
#endif // DBG
/*
UINT64
strtoull_l(
const char * __restrict nptr,
char ** __restrict      endptr,
INT32                   base
);

UINT32
GetNextPattern(
INT32                   Type,
INT32                   Size,
UINT32                  Seed
);
*/

/*! InitThreadParams
*
* \brief Initializes the common thread parameter structure
* \param pcfgParams
* \param params
* \return none
*/

CPM40Utils::CPM40Utils()
{
}


CPM40Utils::~CPM40Utils()
{
}


VOID CPM40Utils::InitThreadParams(PCFG_PARAMS pcfgParams, PTHREAD_PARAMS params)
{
	params->pcfgParams = pcfgParams;
	params->Buffer = NULL;
	params->hThreadHandle = 0;
	params->Direction = 0;

	params->PacketCount = 0;
	params->Running = TRUE;
	params->iPass = 0;

	//! Copy the main configuration information to the thread param struct
	//! to avoid any cross thread issues
	params->Iterations = pcfgParams->Iterations;
	params->MaxPacketSize = pcfgParams->MaxPacketSize;
	params->CardOffset = pcfgParams->CardAddress;
	params->MaxDMAAddress = pcfgParams->MaxDMAAddress;

	params->cardNum = pcfgParams->cardNum;
	params->DMAEngineOffset = pcfgParams->DMAEngineOffset;

	params->DataSeedPattern = pcfgParams->DataSeedPattern;
	params->DataPatternType = pcfgParams->DataPatternType;
	params->DataSeedSize = pcfgParams->DataSeedSize;
	params->UserInfo = pcfgParams->UserInfo;
	params->UserInfoPatternType = pcfgParams->UserInfoPatternType;
	params->UserInfoSeedPattern = pcfgParams->UserInfoSeedPattern;
	params->UserInfoSeedSize = pcfgParams->UserInfoSeedSize;
	params->bUserInfoPatternSpecified = pcfgParams->bUserInfoPatternSpecified;

	params->bDebug = pcfgParams->bDebug;
	params->bQuiet = pcfgParams->bQuiet;
	params->bRandomize = pcfgParams->bRandomize;
	params->bStopOnError = pcfgParams->bStopOnError;
	params->bPatternSpecified = pcfgParams->bPatternSpecified;
	params->bFIFO = pcfgParams->bFIFO;
	params->bBlockingAPIs = pcfgParams->bBlockingAPIs;
	params->bAppendData = pcfgParams->bAppendData;
	params->DMAOverrunCount = 0;
	params->DMADescErrorCount = 0;
	params->ZeroPacketCount = 0;
	params->FullPacketCount = 0;
	params->PartialPacketCount = 0;

	params->CriticalSection = pcfgParams->CriticalSection;
}

/*! StartThread
*
* \brief Start a thread of execution
* \param params
* \param ThreadProc
* \return 0, -2, -3
*/
INT32 CPM40Utils::StartThread(PTHREAD_PARAMS          params,LPTHREAD_START_ROUTINE  ThreadProc)
{
#ifdef WIN32
	//! Create a thread.
	params->hThreadHandle = CreateThread((LPSECURITY_ATTRIBUTES)NULL,   //! No security.
		(DWORD)0,                                   //! Same stack size.
		ThreadProc,                                 //! Thread procedure.
		(LPVOID)params,                             //! Pointer the config parameters.
		(DWORD)0,                                   //! Start immediately.
		NULL);                                      //! Thread ID.

	if (params->hThreadHandle == NULL)
	{
		fprintf(stderr, "CreateThread Failed, returned %d", GetLastError());
		return -2;
	}

	//! Boost the priority of this thread so we don't drop packets.
#if 0  //! Thread Priority Boost (Windows)
	if (!SetThreadPriority(params->hThreadHandle, THREAD_PRIORITY_ABOVE_NORMAL)) //THREAD_PRIORITY_HIGHEST))
	{
		fprintf(stderr, "SetThreadPriority failed");
		return -3;
#endif //! Thread Priority Boost (Windows)

#else // Linux
	//! Create a thread
	if (pthread_create(
		&params->hThreadHandle,
		NULL,
		ThreadProc,
		(PVOID)params))
	{
		fprintf(stderr, "CreateThread failed");
		return -2;
	}
#endif // Windows | Linux

	return 0;
	}

/*! TestRunControl
*
* \brief Controls the test and if necessary shuts down the test threads
* \param pcfgParams
* \param params
* \return Running
*/
BOOLEAN CPM40Utils::TestRunControl(PCFG_PARAMS     pcfgParams,PTHREAD_PARAMS  params)
{
	PTHREAD_PARAMS  pparams = params;
	UINT32          i;
	BOOLEAN         Running = FALSE;

	//! Notify threads that the one second interval has occurrred.
	pcfgParams->bSecondRefresh = TRUE;


	if ((pcfgParams->iPass != 0) && (pcfgParams->bStopOnError))
	{
		pcfgParams->Seconds = 0;
	}
	//! Count down the seconds and if zero shut the test down.
	if (!--pcfgParams->Seconds)
	{
		//! Shutdown all threads.
		pparams = params;
		for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
		{
			pparams->Running = FALSE;
			pparams++;
		}
		//! Allow time for the threads to shutdown.
		Sleep(1000);
	}
	else
	{
		pparams = params;
		for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
		{
			if (pparams->Running)
			{
				Running = TRUE;
				pparams->PacketCount = 0;
			}
			pparams++;
		}
	}
	//! If the driver is shutting down, collect the result codes from each thread
	//! and copy them to the main test thread.
	//! Stop on the first error code.
	if (!Running)
	{
		if (pcfgParams->iPass == 0)
		{
			pparams = params;
			for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
			{
				if (pparams->iPass != 0)
				{
					pcfgParams->iPass = pparams->iPass;
					break;
				}
				pparams++;
			}
		}
	}
	pcfgParams->bRunning = Running;
	return Running;
}

/*! ShutdownTestThreads
*
* \brief Stops all running test threads
* \param pcfgParams
* \param params
* \return none
*/
VOID CPM40Utils::ShutdownTestThreads(PCFG_PARAMS     pcfgParams,PTHREAD_PARAMS  params)
{
	PTHREAD_PARAMS  pparams = params;
	UINT32          i;

	// Shutdown all threads
	pparams = params;
	for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
	{
		pparams->Running = FALSE;
		pparams++;
	}
	// Allow time for the threads to shutdown
	Sleep(1000);
	return;
}

/*! ClearAllStats
*
* \brief Clears all statistics from the hardware and test threads
* \param pcfgParams
* \param params
* \param numberThreads
* \param direction
* \return none
*/
VOID CPM40Utils::ClearAllStats(PCFG_PARAMS pcfgParams,PTHREAD_PARAMS params,INT32  numberThreads,enum CLI_DMA_DIRECTION  direction)
{
	PTHREAD_PARAMS          pparams = params;
	DMA_STAT_STRUCT         dmaStatus;
	INT32                   i;

	// Get the stats and discard due to start up may be bogus
	Sleep(1000);
	if (direction & CLI_CARD_TO_SYSTEM)
	{
		GetDmaPerf(
			pcfgParams->cardNum,
			pcfgParams->DMAEngineOffset,
			DMA_CAP_PACKET_DMA | DMA_CAP_CARD_TO_SYSTEM,
			&dmaStatus);
	}
	if (direction & CLI_SYSTEM_TO_CARD)
	{
		GetDmaPerf(
			pcfgParams->cardNum,
			pcfgParams->DMAEngineOffset,
			DMA_CAP_PACKET_DMA | DMA_CAP_SYSTEM_TO_CARD,
			&dmaStatus);
	}

	BEGIN_CRITICAL(&pcfgParams->CriticalSection);
	for (i = 0; i < numberThreads; i++)
	{
		pparams->TotalBytesTransferred = 0;
		pparams++;
	}
	// Release ownership of the critical section.
	END_CRITICAL(&pcfgParams->CriticalSection);
	return;
}

/*! CollectAllStats
*
* \brief  Collects all statistics from the hardware and test threads
* \param pcfgParams
* \param params
* \param numberThreads
* \param direction
*/
UINT32 CPM40Utils::CollectAllStats(PCFG_PARAMS pcfgParams,PTHREAD_PARAMS params,INT32 numberThreads,enum CLI_DMA_DIRECTION  direction)
{
	PTHREAD_PARAMS          pparams = params;
	PASS_STATS              PassStats;
	DMA_STAT_STRUCT         dmaStatus;
	UINT32                  status;
	INT32                   i;
	CString Buff;

	// Wait 1 second
	Sleep(1000);

	memset(
		&dmaStatus,
		0,
		sizeof(DMA_STAT_STRUCT));
	memset(
		&PassStats,
		0,
		sizeof(PASS_STATS));
	status = 0;

	// Get Performance numbers from driver
	// Get the performance stats
	if (direction & CLI_CARD_TO_SYSTEM)
	{
		status = GetDmaPerf(
			pcfgParams->cardNum,
			pcfgParams->DMAEngineOffset,
			DMA_CAP_PACKET_DMA | DMA_CAP_CARD_TO_SYSTEM,
			&dmaStatus);

		// Get the data from the Perf struct and strip off the low indicator bits
		PassStats.DriverTime = dmaStatus.DriverTime;
		PassStats.HardwareTime = dmaStatus.HardwareTime;
#ifdef HARDWARE_BYTE_COUNT_STATS
		PassStats.BytesTransfered = dmaStatus.CompletedByteCount;
#endif // HARDWARE_BYTE_COUNT_STATS
		PassStats.IntsPerSecond = dmaStatus.IntsPerSecond;
		PassStats.DPCsPerSecond = dmaStatus.DPCsPerSecond;
	}
	if (direction & CLI_SYSTEM_TO_CARD)
	{
		status |= GetDmaPerf(
			pcfgParams->cardNum,
			pcfgParams->DMAEngineOffset,
			DMA_CAP_PACKET_DMA | DMA_CAP_SYSTEM_TO_CARD,
			&dmaStatus);

		// Get the data from the Perf struct and strip off the low indicator bits
		PassStats.DriverTime += dmaStatus.DriverTime;
#ifdef HARDWARE_BYTE_COUNT_STATS
		PassStats.BytesTransfered = dmaStatus.CompletedByteCount;
#endif // HARDWARE_BYTE_COUNT_STATS
		PassStats.IntsPerSecond += dmaStatus.IntsPerSecond;
		PassStats.DPCsPerSecond += dmaStatus.DPCsPerSecond;
	}

	if (status == STATUS_SUCCESSFUL)
	{
#ifndef HARDWARE_BYTE_COUNT_STATS
		PassStats.BytesTransfered = 0;
		BEGIN_CRITICAL(&pcfgParams->CriticalSection);
		for (i = 0; i < numberThreads; i++)
		{
			// Gather all the stats from each thread
			PassStats.BytesTransfered += pparams->TotalBytesTransferred;
			pparams->TotalBytesTransferred = 0;
			pparams++;
		}
		// Release ownership of the critical section.
		END_CRITICAL(&pcfgParams->CriticalSection);
#endif  // NOT HARDWARE_BYTE_COUNT_STATS
		// Display the results of this pass
		status = DisplayPassInfo(pcfgParams, &PassStats);
	}
	else
	{
		Buff.Format(L"Error occurred while getting DMA Performance Info\n");
		status = (UINT32)ERROR_DLL_CALL_FAILED;
	}
	return status;
}

/*! DisplayPassInfo
*
* \brief Keeps track of each pass info and optionally display it
* \param pcfgParams
* \param pPassStats
* \return 0, ERROR_NO_DATA_TRANSFER
*/
UINT32 CPM40Utils::DisplayPassInfo(PCFG_PARAMS     pcfgParams,PPASS_STATS     pPassStats)
{
	float           MBTransferred = 0;
	CString Buff;

#ifdef HARDWARE_BYTE_COUNT_STATS
	// Apply the multiplier (if any).
	pPassStats->BytesTransfered = pPassStats->BytesTransfered * pcfgParams->StatsMultiplier;
#endif // HARDWARE_BYTE_COUNT_STATS
	// Convert the result to megabytes.
	if (pPassStats->BytesTransfered != 0)
	{
		MBTransferred = (float)(pPassStats->BytesTransfered / 1000000.0);
	}

	// Keep track of the totals for the final summary
	pcfgParams->SummaryStats.Samples++;
	pcfgParams->SummaryStats.DriverTimeTotal += pPassStats->DriverTime;
	pcfgParams->SummaryStats.HardwareTimeTotal += pPassStats->HardwareTime;
	pcfgParams->SummaryStats.BytesTransferedTotal += pPassStats->BytesTransfered;
	pcfgParams->SummaryStats.IntsPerSecondTotal += pPassStats->IntsPerSecond;
	pcfgParams->SummaryStats.DPCsPerSecondTotal += pPassStats->DPCsPerSecond;
	pPassStats->CPUUtil = GetCPUUtil();
	pcfgParams->SummaryStats.CPUUtilTotal += pPassStats->CPUUtil;

	if (!pcfgParams->bQuiet)
	{
#ifdef WIN32    // Windows version
		Buff.Format(L"Pass %3lld: MegaBytes/Sec: %6.3f, ", pcfgParams->SummaryStats.Samples, MBTransferred);
		Buff.Format(L"\tCPU Util: %d\n", pPassStats->CPUUtil);
		if (pcfgParams->bVerbose == TRUE)
		{
			Buff.Format(L"DMA active time: %lld ns, ", pPassStats->DriverTime);
			Buff.Format(L"\tDMA wait time: %lld ns\n", pPassStats->HardwareTime);
			Buff.Format(L"Ints/Sec: %lld, ", pPassStats->IntsPerSecond);
			Buff.Format(L"\tDPCs/Sec: %lld\n", pPassStats->DPCsPerSecond);
		}
#else // Linux version
		Buff.Format(L"Pass %qd: MegaBytes/Sec: %6.3f, ", pcfgParams->SummaryStats.Samples, MBTransferred);
		Buff.Format(L"\tCPU Util: %d\n", pPassStats->CPUUtil);
		if (pcfgParams->bVerbose == TRUE)
		{
			Buff.Format(L"DMA active time: %qd ns, ", pPassStats->DriverTime);
			Buff.Format(L"\tDMA wait time: %qd ns\n", pPassStats->HardwareTime);
			Buff.Format(L"Ints/Sec: %qd, ", pPassStats->IntsPerSecond);
			Buff.Format(L"\tTskls/Sec: %qd\n", pPassStats->DPCsPerSecond);
		}
#endif  // Windows vs. Linux
	}
	if (pPassStats->BytesTransfered == 0)
	{
		pcfgParams->iPass = ERROR_NO_DATA_TRANSFER;
		return (UINT32)ERROR_NO_DATA_TRANSFER;
	}
	return 0;
}

/*! InitSummaryStats
*
* \brief Prepares the summary stats and CPU Utilization
* \param pcfgParams
* \return none
*/
VOID CPM40Utils::InitSummaryStats(PCFG_PARAMS     pcfgParams)
{
	pcfgParams->SummaryStats.Samples = 0;
	pcfgParams->SummaryStats.DriverTimeTotal = 0;
	pcfgParams->SummaryStats.BytesTransferedTotal = 0;
	pcfgParams->SummaryStats.IntsPerSecondTotal = 0;
	pcfgParams->SummaryStats.DPCsPerSecondTotal = 0;
	pcfgParams->SummaryStats.CPUUtilTotal = 0;
	GetCPUUtil();
}

/*! DisplaySummaryInfo
*
* \brief Display the summary from the test passes
* \param pcfgParams
*/
VOID CPM40Utils::DisplaySummaryInfo(PCFG_PARAMS pcfgParams)
{
	float   MBTransferred = 0;
	CString Buff;

	if (!pcfgParams->bQuiet)
	{
		// Convert the result to megabytes.
		if (pcfgParams->SummaryStats.BytesTransferedTotal != 0)
		{
			MBTransferred = (float)((pcfgParams->SummaryStats.BytesTransferedTotal / pcfgParams->SummaryStats.Samples) / 1000000.0);
		}
		if (pcfgParams->SummaryStats.Samples)
		{
#ifdef WIN32    // Windows version
			Buff.Format(L"Total bytes transferred %lld", pcfgParams->SummaryStats.BytesTransferedTotal); //theApp.m_DMAMessages.Add(Buff);
			Buff.Format(L"Passes: %lld: Avg MegaBytes/Sec: %6.3f, ", pcfgParams->SummaryStats.Samples, MBTransferred); //theApp.m_DMAMessages.Add(Buff);
			Buff.Format(L"\tAvg CPU Util: %lld",
				pcfgParams->SummaryStats.CPUUtilTotal / pcfgParams->SummaryStats.Samples); //theApp.m_DMAMessages.Add(Buff);
			if (pcfgParams->bVerbose == TRUE)
			{
				Buff.Format(L"Avg DMA active time: %lld ns, ",
					pcfgParams->SummaryStats.DriverTimeTotal / pcfgParams->SummaryStats.Samples); //theApp.m_DMAMessages.Add(Buff);
				Buff.Format(L"\tAvg DMA wait time: %lld ns",
					pcfgParams->SummaryStats.HardwareTimeTotal / pcfgParams->SummaryStats.Samples); //theApp.m_DMAMessages.Add(Buff);
				Buff.Format(L"Avg Ints/Sec: %lld, ",
					pcfgParams->SummaryStats.IntsPerSecondTotal / pcfgParams->SummaryStats.Samples); //theApp.m_DMAMessages.Add(Buff);
				Buff.Format(L"\tAvg DPCs/Sec: %lld",
					pcfgParams->SummaryStats.DPCsPerSecondTotal / pcfgParams->SummaryStats.Samples); //theApp.m_DMAMessages.Add(Buff);
			}
#else // Linux version
			Buff.Format(L"Passes: %qd: Avg MegaBytes/Sec: %6.3f, ", pcfgParams->SummaryStats.Samples, MBTransferred);
			Buff.Format(L"\tAvg CPU Util: %qd\n",
				pcfgParams->SummaryStats.CPUUtilTotal / pcfgParams->SummaryStats.Samples);
			if (pcfgParams->bVerbose == true)
			{
				Buff.Format(L"Avg DMA active time: %qd ns, ",
					pcfgParams->SummaryStats.DriverTimeTotal / pcfgParams->SummaryStats.Samples);
				Buff.Format(L"\tAvg DMA wait time: %qd ns\n",
					pcfgParams->SummaryStats.HardwareTimeTotal / pcfgParams->SummaryStats.Samples);
				Buff.Format(L"Avg Ints/Sec: %qd, ",
					pcfgParams->SummaryStats.IntsPerSecondTotal / pcfgParams->SummaryStats.Samples);
				Buff.Format(L"\tAvg Tskls/Sec: %qd\n",
					pcfgParams->SummaryStats.DPCsPerSecondTotal / pcfgParams->SummaryStats.Samples);
			}
#endif  // Windows vs. Linux
		}
	}
}

/*! DisplayFormattedBytes
*
* \brief Display a formatted buffer dump.
* \param pcfgParams
* \param Offset
* \param pBuffer
* \param BytesToDisplay
* \return none
*/
VOID CPM40Utils::DisplayFormattedBytes(PCFG_PARAMS pcfgParams,UINT64  Offset,PUINT8 pBuffer,INT32 BytesToDisplay)
{
	INT32               i;
	CString Buff;

	if (!pcfgParams->bQuiet)
	{
		// print out the buffer
		for (i = 0; i < BytesToDisplay; i++)
		{
			if ((i & 0x0f) == 0)
			{
				Buff.Format(L"0x%04llx: 0x", i + Offset);
			}
			Buff.Format(L"%02x ", pBuffer[i]);
			if (i % 16 == 15)
			{
				Buff.Format(L"\n");
			}
		}
		Buff.Format(L"\n");
	}
}

/*! DisplayErrorInfo
*
* \brief Display the Error information
* \param pcfgParams
* \param params
* \param status
* \return none
*/
VOID CPM40Utils::DisplayErrorInfo(PCFG_PARAMS pcfgParams,PTHREAD_PARAMS params,UINT32 status)
{
	BOOLEAN             quietMode = false;
	CString Buff;

	if (pcfgParams != NULL)
	{
		quietMode = pcfgParams->bQuiet;
		pcfgParams->iPass = ERROR_DLL_CALL_FAILED;
	}
	else if (params != NULL)
	{
		quietMode = params->bQuiet;
		params->iPass = ERROR_DLL_CALL_FAILED;
	}
	if (!quietMode)
	{
#ifdef WIN32    // Windows version
		Buff.Format(L"\nError returned from Driver/DLL. Return code = 0x%x:\n\t", status);
		switch (status)
		{
		case (UINT32)ERROR_IO_DEVICE:           // STATUS_DRIVER_INTERNAL_ERROR
		{
			Buff.Format(L"Internal driver / OS fault.");
			break;
		}
		case (UINT32)ERROR_ADAP_HDW_ERR:        // STATUS_ADAPTER_HARDWARE_ERROR
		{
			Buff.Format(L"DMA Transaction Error.");
			break;
		}
		case (UINT32)ERROR_INVALID_FUNCTION:    // STATUS_INVALID_DEVICE_REQUEST
		{
			Buff.Format(L"Incompatible DMA Engine specified or transfer size is too large.");
			break;
		}
		case (UINT32)ERROR_BAD_COMMAND:         // STATUS_INVALID_DEVICE_STATE
		{
			Buff.Format(L"No Buffer present or No Packet Receive buffer.");
			break;
		}
		case (UINT32)ERROR_INVALID_PARAMETER:   // STATUS_INVALID_PARAMETER
		{
			Buff.Format(L"Invalid parameter, parameter out of range or bad token.");
			break;
		}
		case (UINT32)ERROR_NO_SYSTEM_RESOURCES: // STATUS_INSUFFICIENT_RESOURCES
		{
			Buff.Format(L"The transfer size requested is too large.");
			break;
		}
		case (UINT32)ERROR_FILE_NOT_FOUND:      // STATUS_NO_SUCH_DEVICE
		{
			Buff.Format(L"Invalid device or incorrect DMA Engine specified.");
			break;
		}
		case ERROR_NOT_ENOUGH_MEMORY:           // STATUS_NO_MEMORY
		{
			Buff.Format(L"Unable to allocate enough DMA Descriptors.");
			break;
		}
		case ERROR_NOT_SUPPORTED:               // STATUS_NOT_SUPPORTED
		{
			Buff.Format(L"Requested function is not supported.");
			break;
		}
		case ERROR_SEM_TIMEOUT:                 // STATUS_IO_TIMEOUT
		{
			Buff.Format(L"The request timed out.");
			break;
		}
		case (UINT32)ERROR_BUSY:                // STATUS_DEVICE_BUSY
		{
			Buff.Format(L"The devices DMA Engine is busy.");
			break;
		}
		default:
			Buff.Format(L"Unknown error code");
		}
#else // Linux version
		Buff.Format(L"\nError returned from Driver. errno = %d:\n\t", status);
		switch (status)
		{
		case ENODEV:
		{
			Buff.Format(L"Non-existent DMA Engine specified.");
			break;
		}
		case EFAULT:
		{
			Buff.Format(L"Internal driver / OS fault.");
			break;
		}
		case EIO:
		{
			Buff.Format(L"DMA Transaction Error.");
			break;
		}
		case ENOSYS:
		{
			Buff.Format(L"Incompatible DMA Engine specified.");
			break;
		}
		case ENOBUFS:
		{
			Buff.Format(L"No Buffer present or No Packet Receive buffer.");
			break;
		}
		case EINVAL:
		{
			Buff.Format(L"Invalid parameter, parameter out of range or Bad Receive Token.");
			break;
		}
		case ENOMEM:
		{
			Buff.Format(L"Failed to allocate enough memory for the transaction.");
			break;
		}
		case EMSGSIZE:
		{
			Buff.Format(L"The transfer size requested is too large.");
			break;
		}
		case STATUS_INVALID_MODE:
		{
			Buff.Format(L"Invalid parameter, incorrect DMA Engine specified.");
			break;
		}
		default:
			Buff.Format(L"Unknown error code.");
		}
#endif  // Windows vs. Linux
		Buff.Format(L"\n");
	}
}

/*! __strtoul
*
* \brief Convert a string to a UINT32
* \param nptr
* \return result
*/
UINT32 CPM40Utils::__strtoul(const char *    nptr)
{
	UINT32          result = 0;

	result = strtoul(nptr, NULL, 10);
	if (!result)
	{
		result = strtoul(nptr, NULL, 16);
	}
	return (result);
}

/*! __strtoull
*
* \brief Convert a string to a unsigned long long (64 bit)
* \param nptr
* \return result
*/
UINT64 CPM40Utils::__strtoull(const char * nptr)
{
	UINT64  result = 0;
	result = strtoull_l(nptr, NULL, 10);
	if (!result)
	{
		result = strtoull_l(nptr, NULL, 16);
	}
	return (result);
}

/*! strtoull_l
*
* \brief Internal function - Convert a string to an unsigned long long integer.
* \note Assumes that the upper and lower case
*  alphabets and digits are each contiguous.
* \param nptr
* \param endptr
* \param base
* \return acc
*/
UINT64 CPM40Utils::strtoull_l(const char * __restrict nptr,char **      __restrict endptr,INT32 base)
{
	const char *            s;
	UINT64                  acc;
	INT8                    c;
	UINT64                  cutoff;
	INT32                   any;
	INT32                   cutlim;

	s = nptr;
	do {
		c = *s++;
	} while (isspace((UCHAR)c));
	if (c == '-')
	{
		c = *s++;
	}
	else
	{
		if (c == '+')
		{
			c = *s++;
		}
	}
	if ((base == 0 || base == 16) &&
		c == '0' && (*s == 'x' || *s == 'X'))
	{
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
	{
		base = c == '0' ? 8 : 10;
	}
	acc = any = 0;
	if (base < 2 || base > 36)
	{
		goto noconv;
	}

	cutoff = MY_ULONGLONG_MAX / base;
	cutlim = MY_ULONGLONG_MAX % base;
	for (;; c = *s++)
	{
		if (c >= '0' && c <= '9')
		{
			c -= '0';
		}
		else if (c >= 'A' && c <= 'Z')
		{
			c -= 'A' - 10;
		}
		else if (c >= 'a' && c <= 'z')
		{
			c -= 'a' - 10;
		}
		else
		{
			break;
		}
		if (c >= base)
		{
			break;
		}
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
		{
			any = -1;
		}
		else
		{
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0)
	{
		acc = MY_ULONGLONG_MAX;
	}
	else if (!any)
	{
	noconv:
		acc = 0;
	}   //else if (neg)
	//acc = -acc;
	if (endptr != NULL)
	{
		*endptr = (PCHAR)(any ? s - 1 : nptr);
	}
	return (acc);
}

/*! __hextoul
*
* \brief Converts a hex value to UINT32
* \param value
* \return finalNum
*/
UINT32 CPM40Utils::_hexttoul(const char *    value)
{
	UINT32          finalNum = 0;
	const char *    nextChar = value;
	INT8            upperChar;
	INT32           length = 0;

	while ((*nextChar != 0) && (length < 8))
	{
		upperChar = (INT8)toupper(*nextChar);
		if ((upperChar >= '0') && (upperChar <= '9'))
		{
			// add next char
			finalNum <<= 4;
			finalNum += (upperChar - '0');
		}
		else if ((upperChar >= 'A') && (upperChar <= 'F'))
		{
			// add next char
			finalNum <<= 4;
			finalNum += (upperChar - 'A') + 10;
		}
		else if (upperChar == 'X')
		{
			// chop off beginning 0x
			finalNum = 0;
			length = 0;
		}

		// next char
		nextChar++;
		length++;
	}
	return finalNum;
}

void CPM40Utils::reverse_string(unsigned char *	str,UINT64			strLength)
{
	// skip null
	if (str == 0)
	{
		return;
	}

	// get range
	unsigned char *start = str;
	unsigned char *end = start + strLength - 1; // -1 for \0
	unsigned char temp;

	// reverse
	while (end > start)
	{
		// swap
		temp = *start;
		*start = *end;
		*end = temp;

		// move
		++start;
		--end;
	}
}

/*! atohx
*
* \brief Convert an ASCII hex string to a binary string
* \param dst
* \param src
* \return Length
*/
INT32 CPM40Utils::atohx(PCHAR           dst,const char *    src)
{
	PCHAR           dstStart = dst;
	INT32           Length = 0;
	INT32           hexlsb,
		hexmsb;
	INT32           lsb = 0,
		msb = 0;

	hexmsb = tolower(*src);
	hexlsb = tolower(*(src + 1));

	// strip of the 0x if present
	if (hexmsb == '0')
	{
		if (hexlsb == 'x')
		{
			src += 2;
		}
	}

	for (lsb, msb; *src; src += 2)
	{
		msb = tolower(*src);
		lsb = tolower(*(src + 1));
		msb -= isdigit(msb) ? 0x30 : 0x57;
		lsb -= isdigit(lsb) ? 0x30 : 0x57;
		// Make sure we had a hex value converted
		if ((msb < 0x0 || msb > 0xf) || (lsb < 0x0 || lsb > 0xf))
		{
			// If not a valid hex value, exit out
			*dstStart = 0;
			return 0;
		}
		*dst++ = (char)(lsb | (msb << 4));
		Length++;
	}
	*dst = 0;
	return Length;
}

/*! GetCPUUtil
*
*  \brief retieves the CPU Utilization since the last time this routine was called
*  \note It is necessary to call once to establish a baseline before collecting stats.
*  \return cpu - CPU Utilization since last call to this routine
*/
INT32 CPM40Utils::GetCPUUtil()
{
	INT32       cpu = 0;

	UINT64      idleTime;
	UINT64      kernelTime;
	UINT64      userTime;
	UINT64      idl;
	UINT64      usr;
	UINT64      sys;
#ifndef WIN32 // Linux version
	UINT64      niceTime;
	UINT64      iowaitTime;
	UINT64      nice;
	UINT64      iowait;
	UINT64      totalTime;
	INT32       ret;
	INT8        line[256];
#else
	UINT64      ker;
#endif  // Windows conditional compile.

#ifdef WIN32
	GetSystemTimes((LPFILETIME)&idleTime, (LPFILETIME)&kernelTime, (LPFILETIME)&userTime);

	usr = userTime - g_userTime;
	ker = kernelTime - g_kernelTime;
	idl = idleTime - g_idleTime;
	sys = (usr + ker);

	if (sys > 0)
	{
		cpu = INT32((sys - idl) * 100 / sys); // System Idle take 100 % of CPU.
	}

	g_idleTime = idleTime;
	g_kernelTime = kernelTime;
	g_userTime = userTime;

#else // Linux

	/*!
	In a Linux environment, it is recommeneded that %idle/usage is used first, using /proc/stat.
	If that does not work, disable /proc/stat for the future by closing the file and use the next-best method.
	*/
	if (ghProcStat != NULL)
	{
		//! Get the first line with the data.
		rewind(ghProcStat);
		fflush(ghProcStat);
		if (NULL == fgets(line, 256, ghProcStat))
		{
			Buff.Format(L"Failed to read CPU Status, disabling CPU Stats\n");
			fclose(ghProcStat);
			ghProcStat = NULL;     // Don't try again.
		}
		else
		{
			iowaitTime = 0;
			ret = sscanf(line, "%*s %qu %qu %qu %qu %qu",
				&userTime,
				&kernelTime, &niceTime, &idleTime, &iowaitTime);
			if (ret < 4)
			{
				Buff.Format(L"Failed to parse valid CPU Stats, disabiling CPU Stats\n");
				fclose(ghProcStat);
				ghProcStat = NULL; // Don't try again.
			}
			else
			{
				//! Store the current usage.
				usr = userTime - g_userTime;
				sys = kernelTime - g_kernelTime;
				nice = niceTime - g_niceTime;
				idl = idleTime - g_idleTime;
				iowait = iowaitTime - g_iowaitTime;
				//! Calculate the CPU Percentage Usage.
				sys = usr + sys + nice;
				totalTime = sys + idl + iowait;
				if ((totalTime > 0) && (sys > 0))
				{
					cpu = (INT32)(100L * sys / totalTime);
				}
				//! Store the values for the next calculation.
				g_userTime = userTime;
				g_kernelTime = kernelTime;
				g_niceTime = niceTime;
				g_idleTime = idleTime;
				g_iowaitTime = iowaitTime;
			}
		}
	}

#endif // Windows vs. Linux
	return cpu;
}

/*! SetupOSSpecifics
*
* \brief Sets up any OS Specific functions, i.e Open /proc/stat in Linux
* \param pcfgParams
* \return 0, 1, -4
*/
INT32 CPM40Utils::SetupOSSpecifics(PCFG_PARAMS pcfgParams)
{
#ifndef WIN32  // Linux Version
	// Create mutex attribute variable.
	pthread_mutexattr_t mAttr;

	/*! \note
	* The following method is called in order to initialize the status calls
	* routines.  After that it is safe to call each of the status calls separately
	* @return GNUNET_OK on success and GNUNET_SYSERR on error (or calls errexit).
	*/
	ghProcStat = fopen("/proc/stat", "r");
	if (NULL == ghProcStat)
	{
		Buff.Format(L"Failed to open /proc/stat for CPU Utilization, disabiling CPU Stats\n");
		return 1;
	}

	// Cache the pointer to the configuration struct just for the signal handler.
	gpcfgParams = pcfgParams;

	// Handle the signals, I want to try and shutdown as gracefully as possible.
	signal(SIGTERM, SignalHandler);
	signal(SIGABRT, SignalHandler);
	signal(SIGQUIT, SignalHandler);
	signal(SIGINT, SignalHandler);
	signal(SIGHUP, SignalHandler);
	signal(SIGTSTP, SignalHandler);
	signal(SIGSTOP, SignalHandler);

	pthread_mutexattr_init(&mAttr);
	// Setup recursive mutex for mutex attribute.
	pthread_mutexattr_settype(&mAttr, PTHREAD_MUTEX_NORMAL);

	// Use the mutex attribute to create the mutex.
	pthread_mutex_init(&pcfgParams->CriticalSection, &mAttr);

	// Mutex attribute can be destroy after initializing the mutex variable
	pthread_mutexattr_destroy(&mAttr);
#else
	if (!InitializeCriticalSectionAndSpinCount(&pcfgParams->CriticalSection, 0x00000400))
	{
		fprintf(stderr, "InitializeCriticalSectionAndSpinCount Failed, returned %d", GetLastError());
		return -4;
	}
#endif  // Linux version
	return 0;
}

/*! ShutdownOSSpecifics
*
* \brief Shutsdown any OS Specific functions, i.e Close /proc/stat in Linux
* \param pcfgParams
* \return
*/
INT32 CPM40Utils::ShutdownOSSpecifics(PCFG_PARAMS pcfgParams)
{
#ifndef WIN32  // Linux Version
	pthread_mutex_destroy(&pcfgParams->CriticalSection);
	if (ghProcStat != NULL)
	{
		fclose(ghProcStat);
		ghProcStat = NULL;
	}
#else
	// Release resources used by the critical section object.
	DeleteCriticalSection(&pcfgParams->CriticalSection);
#endif  // Linux version
	return 0;
}

/*! ConfigPacketGenChk
*
* \brief Configures the hardware reference designs Packet Generator / Checker
* \param pcfgParams
* \param Mode
* \param Direction
* \param NumberPackets
* \return
*/
INT32 CPM40Utils::ConfigPacketGenChk(PCFG_PARAMS pcfgParams,INT32 Mode,BOOLEAN Direction,UINT32 NumberPackets)
{
	PACKET_GENCHK_STRUCT    PacketGenChk;
	UINT64                  UserInfoSeedHigh;
	UINT32                  UserInfoSeedNext;
	CString Buff;

	memset(&PacketGenChk, 0, sizeof(PACKET_GENCHK_STRUCT));
	PacketGenChk.Control = Mode;

	if (Mode == 0)
	{

#if 0 // DBG
		if (Direction == S2C_DIRECTION)
		{
			GetPacketCheckerErrors(pcfgParams, (UINT8)pcfgParams->DMAEngineOffset);
		}
#endif // DBG

		// Now disable the Gen/Chk.
		PacketGenChk.Control = Mode;
	}
	else if (Mode == PACKET_GEN_LOOPBACK_ENABLE)
	{
		PacketGenChk.NumPackets = 0; // Run forever
		if (SetupPacketGenerator(
			pcfgParams,
			(UINT8)pcfgParams->DMAEngineOffset,
			!Direction,
			&PacketGenChk) != STATUS_SUCCESSFUL)
		{
			Buff.Format(L"Config Packet Checker failed.\n");
			return -2;
		}
	}
	else if (Mode == PACKET_GEN_LOOPBACK_USE_RAM)   // aka Packet Source/Destination Select
	{
		pcfgParams->bPatternSpecified = FALSE;
	}
	else if (Mode == PACKET_GEN_ENABLE)
	{
		PacketGenChk.Control |= 0x00010000;         // Set FASTEST packet rate control

		//PacketGenChk.Control |= 0xFF010000;       // Set SLOWEST packet rate control

		PacketGenChk.NumPackets = NumberPackets;
		if ((int)NumberPackets == DEFAULT_ITERATIONS)
		{
			PacketGenChk.NumPackets = 0;            // Run forever
		}

		if (pcfgParams->bPatternSpecified)
		{
			PacketGenChk.DataSeed = (UINT32)pcfgParams->DataSeedPattern;

			if (pcfgParams->DataPatternType == PATTERN_INCREMENTING)
			{
				if (pcfgParams->DataSeedSize == 1)
				{
					PacketGenChk.Control |= PACKET_GEN_DATA_PATTERN_INC_BYTE;
					pcfgParams->DataSeedPattern = pcfgParams->DataSeedPattern & 0xFF;
				}
				else
				{
					PacketGenChk.Control |= PACKET_GEN_DATA_PATTERN_INC_DWORD;
				}
			}
			else if (pcfgParams->DataPatternType == PATTERN_RANDOM)
			{
				PacketGenChk.Control |= PACKET_GEN_DATA_PATTERN_LFSR;
			}
		}

		if (pcfgParams->bUserInfoPatternSpecified)
		{
			// When testing User Status we must run only 1 thread since the status
			// continues across packet boundaries. We have to have the packets in order.
			pcfgParams->NumberThreads = 1;

			// Setup the User Status / Control portion of the Generator/Checker.
			pcfgParams->UserInfoSeedPattern &= 0xFFFFFFFF;
			if (pcfgParams->UserInfoPatternType == PATTERN_INCREMENTING)
			{
				if (pcfgParams->UserInfoSeedSize == 1)
				{
					PacketGenChk.Control |= PACKET_GEN_USER_PATTERN_INC_BYTE;
					pcfgParams->UserInfoSeedPattern = pcfgParams->UserInfoSeedPattern & 0xFF;
				}
				else
				{
					PacketGenChk.Control |= PACKET_GEN_USER_PATTERN_INC_DWORD;
				}
				PacketGenChk.Control |= PACKET_GEN_CONT_USER_PATTERN;
			}
			else if (pcfgParams->UserInfoPatternType == PATTERN_RANDOM)
			{
				PacketGenChk.Control |= PACKET_GEN_USER_PATTERN_LFSR | PACKET_GEN_CONT_USER_PATTERN;
			}

			UserInfoSeedNext = pcfgParams->UserInfoSeedPattern & 0xFFFFFFFF;
			UserInfoSeedHigh = GetNextPattern(pcfgParams->UserInfoPatternType, pcfgParams->UserInfoSeedSize, UserInfoSeedNext);
			pcfgParams->UserInfoSeedPattern |= ((UserInfoSeedHigh & 0xFFFFFFFF) << 32);

			PacketGenChk.UserCtrlStatSeed = (UINT32)pcfgParams->UserInfoSeedPattern & 0xFFFFFFFF;
			// Buff.Format(L"Initial User Info Seed: 0x%llx\n", pcfgParams->UserInfoSeedPattern);
		}

		// Setup the Packet Lengths for the Generator/Checker
		if (pcfgParams->bRandomize)
		{
			// Set to use all 4 size table entries
			PacketGenChk.Control |= PACKET_GEN_TABLE_ENTRIES_MASK;
			PacketGenChk.PacketLength[0] = rand() % pcfgParams->MaxPacketSize + 1;
			PacketGenChk.PacketLength[1] = rand() % pcfgParams->MaxPacketSize + 1;
			PacketGenChk.PacketLength[2] = rand() % pcfgParams->MaxPacketSize + 1;
			PacketGenChk.PacketLength[3] = rand() % pcfgParams->MaxPacketSize + 1;
			if (pcfgParams->bVerbose)
			{
				Buff.Format(L"Random Packet sizes set to %d, %d, %d, %d\n",
					PacketGenChk.PacketLength[0],
					PacketGenChk.PacketLength[1],
					PacketGenChk.PacketLength[2],
					PacketGenChk.PacketLength[3]
					);
			}
		}
		else
		{
			PacketGenChk.PacketLength[0] = pcfgParams->MaxPacketSize;
			PacketGenChk.PacketLength[1] = pcfgParams->MaxPacketSize;
			PacketGenChk.PacketLength[2] = pcfgParams->MaxPacketSize;
			PacketGenChk.PacketLength[3] = pcfgParams->MaxPacketSize;
		}
	}
	//! Setup the packet generator/checker
	if (SetupPacketGenerator(pcfgParams, (UINT8)pcfgParams->DMAEngineOffset,
		Direction, &PacketGenChk) != STATUS_SUCCESSFUL)
	{
		Buff.Format(L"Setup Packet Generator/Checker failed, Max Packet Size (%x) is too large\n", pcfgParams->MaxPacketSize);
		return -2;
	}
	return STATUS_SUCCESSFUL;
}

/*! SetupPacketGenerator
*
* \brief Setup the traffic Generator so we get DMA transfers
* \param pcfgParams
* \param DMAEngineOffset
* \param Direction
* \param pPacketGenChk
* \return
*/
INT32 CPM40Utils::SetupPacketGenerator(PCFG_PARAMS pcfgParams,UINT8 DMAEngineOffset,BOOLEAN Direction,PPACKET_GENCHK_STRUCT pPacketGenChk)
{
	GENERATOR_CHECKER_CONTROL   GenChkCtrl;
	GENERATOR_CHECKER_PARAMS    GenChkParams;
	DMA_ENGINE_STRUCT           DMAEngInfo;
	STAT_STRUCT                 StatusInfo;
	UINT64                      CardOffset = 0;
	UINT8                       DMAEngine;
	UINT32                      TypeDirection;
	INT32                       i;
	INT32                       Status = 0;
	CString Buff;

	if (Direction == S2C_DIRECTION)
	{
		DMAEngine = DMAEngineOffset;
		TypeDirection = (DMA_CAP_SYSTEM_TO_CARD | DMA_CAP_PACKET_DMA);
	}
	else
	{
		DMAEngine = DMAEngineOffset + 32;
		TypeDirection = (DMA_CAP_CARD_TO_SYSTEM | DMA_CAP_PACKET_DMA);
	}

	if (DMAEngine < MAX_NUM_DMA_ENGINES)
	{
		// The DMA Control Status Register is the second Dword in the DMA Engine struct.
		CardOffset = (DMAEngine * 0x100);
		Status = DoMem(
			pcfgParams->cardNum,
			READ_FROM_CARD,
			pcfgParams->DMA_BAR,
			(PUINT8)&DMAEngInfo,
			0,
			CardOffset,
			sizeof(DMAEngInfo),
			&StatusInfo);
		// Check the status
		if (Status != 0)
		{
			DisplayErrorInfo(
				pcfgParams,
				NULL,
				Status);
			return Status;
		}
	}
	else
	{
		Buff.Format(L"getDMAStat: Invalid DMA Engine specified (%d), Must be less than 64!\n", DMAEngine);
		return Status;
	}

	GenChkCtrl.Control = pPacketGenChk->Control;
	GenChkParams.DataSeed = pPacketGenChk->DataSeed;
	GenChkParams.NumPackets = pPacketGenChk->NumPackets;
	GenChkParams.UserCtrlStatSeed = pPacketGenChk->UserCtrlStatSeed;

	for (i = 0; i < 4; i++)
	{
		GenChkParams.PacketLength[i] = pPacketGenChk->PacketLength[i];
	}

	CardOffset = PACKET_GENERATOR_ENGINE_OFFSET + sizeof(GENERATOR_CHECKER_CONTROL) + (DMAEngine * PACKET_GENERATOR_BLOCK_SIZE);

	Status = (INT32)DoMem(
		pcfgParams->cardNum,
		WRITE_TO_CARD,
		pcfgParams->DMA_BAR,
		(PUINT8)&GenChkParams,
		0,
		CardOffset,
		sizeof(GENERATOR_CHECKER_PARAMS),
		&StatusInfo);

	if (Status == 0)
	{
		if (StatusInfo.CompletedByteCount == sizeof(GENERATOR_CHECKER_PARAMS))
		{
			CardOffset = PACKET_GENERATOR_ENGINE_OFFSET + (DMAEngine * PACKET_GENERATOR_BLOCK_SIZE);

			Status = (INT32)DoMem(
				pcfgParams->cardNum,
				WRITE_TO_CARD,
				pcfgParams->DMA_BAR,
				(PUINT8)&GenChkCtrl,
				0,
				CardOffset,
				sizeof(GENERATOR_CHECKER_CONTROL),
				&StatusInfo);

			// Check to see if we are shutting down the Packet Generaotr/Checker
			if (GenChkCtrl.Control == 0)
			{
				// Set the rety count
				i = 10;
				// And make sure it shuts down
				do {
					Status = (int)DoMem(
						pcfgParams->cardNum,
						READ_FROM_CARD,
						pcfgParams->DMA_BAR,
						(PUCHAR)&GenChkCtrl,
						0,
						CardOffset,
						sizeof(GENERATOR_CHECKER_CONTROL),
						&StatusInfo);
					if (!i--)
					{
						Buff.Format(L"Packet Generator/Checker failed to stop!\n");
						break;
					}
					if (GenChkCtrl.Control & PACKET_GEN_ENABLE)
					{
						if ((pcfgParams->TestType != PACKET_RECV) &&
							(pcfgParams->TestType != PACKET_RECVS))
						{
							g_PM40Packets.SinglePacketReadWrite(
								pcfgParams,
								DMAEngineOffset,
								Direction);
							Sleep(1);
						}
						else
						{
							g_PM40Packets.SinglePacketRecvs(pcfgParams, DMAEngineOffset);
							Sleep(1);
						}
						ResetDMAEngine(
							pcfgParams->cardNum,
							DMAEngineOffset,
							TypeDirection);
						Sleep(1);
					}
				} while (GenChkCtrl.Control & PACKET_GEN_ENABLE);
			}
		}
		else
		{
			i = sizeof(GENERATOR_CHECKER_PARAMS);
			Buff.Format(L"DoMem transfer size error, Completed %llu, expected %i\n",
				StatusInfo.CompletedByteCount, i);
			Status = ERROR_TRANSFER_DATA_MISMATCH;
		}
	}
	else
	{
		Buff.Format(L"DoMem failed to write parameters!\n, status = %d", Status);
	}
	return Status;
}

#if 0 //DBG  Commented out
/*! GetPacketCheckerErrors
*
* \briefReturn the Packet Checker errors (if any)
* \param pcfgParams
* \param DMAEngineOffset
* \return CheckerError
*/
UINT32
GetPacketCheckerErrors(
PCFG_PARAMS             pcfgParams,
UINT8                   DMAEngineOffset)
{
	STAT_STRUCT             StatusInfo;
	UINT64                  CardOffset = 0;
	UINT32                  CheckerError = 0;
	UINT32                  CheckerClear = CHECKER_CLEAR_ERROR_CMD;
	INT32                   Status = 0;

	if (DMAEngineOffset < MAX_NUM_DMA_ENGINES)
	{
		CardOffset = PACKET_GENERATOR_ENGINE_OFFSET + ((DMAEngineOffset * PACKET_GENERATOR_BLOCK_SIZE) + CHECKER_ERROR_OFFSET);

		Status = (int)DoMem(
			pcfgParams->cardNum,
			READ_FROM_CARD,
			pcfgParams->DMA_BAR,
			(BYTE *)&CheckerError,
			0,
			CardOffset,
			sizeof(CheckerError),
			&StatusInfo);

		if (Status == 0)
		{
			Status = (int)DoMem(
				pcfgParams->cardNum,
				WRITE_TO_CARD,
				pcfgParams->DMA_BAR,
				(BYTE *)&CheckerClear,
				0,
				CardOffset,
				sizeof(CheckerClear),
				&StatusInfo);

			if ((pcfgParams->bUserInfoPatternSpecified) || (pcfgParams->bPatternSpecified))
			{
				if (CheckerError != 0)
				{
					Buff.Format(L"Packet Checker recorded the following errors:\n");
					if (CheckerError &  CHECKER_ERROR_SOP)
						Buff.Format(L"\tSOP Error detected!\n");
					if (CheckerError &  CHECKER_ERROR_EOP)
						Buff.Format(L"\tEOP Error detected!\n");
					if (CheckerError &  CHECKER_ERROR_CPL)
						Buff.Format(L"\tDMA Error detected!\n");
					if (pcfgParams->bPatternSpecified)
					{
						if (CheckerError &  CHECKER_ERROR_DATA)
							Buff.Format(L"\tPacket data Error detected!\n");
						if (CheckerError &  CHECKER_ERROR_BAD_DATA)
							Buff.Format(L"\tUnexpected Data Error detected!\n");
					}
					if (pcfgParams->bUserInfoPatternSpecified)
					{
						if (CheckerError &  CHECKER_ERROR_USR_CTL)
							Buff.Format(L"\tUser Control Error detected!\n");
					}
					Buff.Format(L"\n\tError Count: %d\n", ((CheckerError & CHECKER_ERROR_CNTR_MASK) >> 8));
				}
			}
		}
	}
	return CheckerError;
}
#endif // DBG

/*! BufferAllocAndSet
*
* \brief Allocates a buffer based on the contents of cfgParams BufferSize and
*  initializes the buffer based on DataPatternType and SeedPattern
* \param pcfgParams
* \param params
* \return 0, -1
*/
INT32 CPM40Utils::BufferAllocAndSet(PCFG_PARAMS pcfgParams,PTHREAD_PARAMS  params)
{
	UINT32          t;
	UINT32          seed = 0;
	UINT8           seqNum = 0;
	PUINT32         ulBuffer;
	UINT32          InitLen;
	UINT32          InitValue;
	CString Buff;

	// Allocate the buffer in multiples of 1024 bytes (minimum of 1024)
	params->TransferLength = pcfgParams->BufferSize;
	params->Buffer = NULL;

	params->AllocSize = SIZE_T((pcfgParams->BufferSize + 1023) & -1024);
	if ((params->AllocSize == 0) || (params->AllocSize < 8))
	{
#ifdef WIN32
		Buff.Format(L"Invalid Buffer Allocation Size = %Id!\n", params->AllocSize);
#else // Linux
		Buff.Format(L"Invalid Buffer Allocation Size = %zd!\n", params->AllocSize);
#endif // Windows vs. Linux
		pcfgParams->iPass = ERROR_MEMORY_ALLOC_FAILED;
		return -1;
	}
	// Allocate the buffer
#ifdef WIN32
	params->Buffer = (PUINT8)VirtualAlloc(NULL, params->AllocSize, MEM_COMMIT, PAGE_READWRITE | PAGE_NOCACHE);
	if (params->Buffer == NULL)
#else // Linux
	if (posix_memalign((void **)&params->Buffer, 4096, params->AllocSize))
#endif // Windows vs. Linux
	{
#ifdef WIN32
		Buff.Format(L"Buffer malloc Failed! Size = %Id\n", params->AllocSize);
#else // Linux
		Buff.Format(L"Buffer malloc Failed! Size = %zd\n", params->AllocSize);
#endif // Windows vs. Linux
		params->Buffer = NULL;
		pcfgParams->iPass = ERROR_MEMORY_ALLOC_FAILED;
		return -1;
	}
	if (params->AllocSize != pcfgParams->BufferSize)
	{
		if (pcfgParams->bDebug)
		{
#ifdef WIN32
			Buff.Format(L"Buffer size optimized to %Id for better performance from the requested size of %lld.\n",
				params->AllocSize, pcfgParams->BufferSize);
#else // Linux
			Buff.Format(L"Buffer size optimized to %zd for better performance from the requested size of %lld.\n",
				params->AllocSize, pcfgParams->BufferSize);
#endif // Windows vs. Linux
			FreeMem(params);
			pcfgParams->iPass = ERROR_MEMORY_ALLOC_FAILED;
			return -1;
		}
	}
	memset(params->Buffer, 0xA5, params->AllocSize);

	if (params->AllocSize > 0x7FFFFFFF)
	{
		// Turn off pattern setup and checking if using larger the 2GB buffers.
		pcfgParams->bPatternSpecified = FALSE;
	}

	if (pcfgParams->bPatternSpecified)
	{
		ulBuffer = (PUINT32)params->Buffer;
		InitLen = (UINT32)params->AllocSize >> 2;

		switch (pcfgParams->DataPatternType)
		{
		case PATTERN_INCREMENTING:
		case PATTERN_SPECIFIED:
		{
			seed = pcfgParams->DataSeedPattern;
			for (t = 0; t < InitLen; t++)
			{
				InitValue = seed;
				if (pcfgParams->DataSeedSize == 1)
				{
					InitValue = ((seqNum & 0xFF) << 24) | ((seqNum & 0xFF) << 16) | ((seqNum & 0xFF) << 8) | (seed & 0xFF);
				}
				ulBuffer[t] = InitValue;
				if (pcfgParams->DataPatternType == PATTERN_INCREMENTING)
				{
					if (pcfgParams->DataSeedSize == 1)
					{
						seed += 4;
						seqNum += 4;
					}
					else
					{
						seed++;
					}
				}
			}
		}
		break;

		case PATTERN_RANDOM:
		{
			srand(pcfgParams->DataSeedPattern);
			for (t = 0; t < InitLen; t++)
			{
				ulBuffer[t] = rand();
			}
		}
		break;

		default:
			memset(params->Buffer, 0xAA, params->AllocSize);
		}
	}
	return 0;
}

/*! ValidateDataBuffer
*
* \brief Compares the buffer to the data pattern specified
* \param params
* \param Buffer
* \param Length
*/
BOOLEAN CPM40Utils::ValidateDataBuffer(PTHREAD_PARAMS  params,PUINT8 Buffer,UINT64 Length)
{
	BOOLEAN         result = true;
	UINT64          BufferOffset;
	UINT32          LeftToDo = 0;
	PUINT32         ulBuffer;
	UINT32          seed;
	UINT32          CmpLen;
	UINT32          t;
	CString Buff;

	if (params->bPatternSpecified)
	{
		if (Buffer == NULL)
		{
			Buff.Format(L"ValidateDataBuffer: Buffer = NULL\n");
			return FALSE;
		}
		if (Length == 0)
		{
			Buff.Format(L"ValidateDataBuffer: Length = 0\n");
			return FALSE;
		}

		ulBuffer = (PUINT32)Buffer;
		BufferOffset = Buffer - params->Buffer;
		// Check for an out of range buffer
		if (BufferOffset > params->AllocSize)
		{
			Buff.Format(L"ValidateDataBuffer: Buffer Addr: 0x%p, Ret Buffer = 0x%p, BufferOffset = 0x%llx, Length = 0x%llx\n",
				params->Buffer, Buffer, BufferOffset, Length);
			return FALSE;
		}
		CmpLen = (UINT32)(Length >> 2);

		if ((BufferOffset + Length) > params->AllocSize)
		{
			CmpLen = (UINT32)((params->AllocSize - BufferOffset) >> 2);
			LeftToDo = (UINT32)((Length - (params->AllocSize - BufferOffset)) >> 2);
		}

		seed = params->DataSeedPattern;
		while (CmpLen)
		{
			for (t = 0; t < CmpLen; t++)
			{
				if (ulBuffer[t] != (UINT32)seed)
				{
					if (!params->bQuiet)
					{
						Buff.Format(L"Data mistatch at offset %x (out of %lld)! Expected Value: 0x%08x, Buffer Value: 0x%08x\n",
							t * 4, Length, (UINT32)(seed & 0xFFFFFFFF), (ulBuffer[t] & 0xFFFFFFFF));
					}
					result = false;
					params->iPass = ERROR_TRANSFER_DATA_MISMATCH;
					if (params->bStopOnError)
					{
						// stop here
						break;
					}
				}
				seed = GetNextPattern(
					params->DataPatternType,
					params->DataSeedSize,
					seed);
			}
			/*! \note In order to command for a continious pattern
			*  the driver should save the seed value thus:
			*  params->DataSeedPattern = seed;
			*/
			CmpLen = LeftToDo;
			LeftToDo = 0;
			ulBuffer = (PUINT32)params->Buffer;
		}
	}
	return result;
}

/*! WriteRandomBuffer
*
* \brief Writes random data to the buffer pointed to in the params struct.
* \param params
* \param Length
* \return none
*/
VOID CPM40Utils::WriteRandomBuffer(PTHREAD_PARAMS  params,UINT32 Length)
{
	UINT32          t;
	PUINT32         ulBuffer = (PUINT32)params->Buffer;
	UINT32          InitLen = 0;
	PUINT8          ucBuffer;

	InitLen = Length >> 2;
	srand(params->DataSeedPattern);
	for (t = 0; t < InitLen; t++)
	{
		*ulBuffer = (UINT32)rand();
		ulBuffer++;
	}
	ucBuffer = (PUINT8)ulBuffer;
	InitLen = Length & 0x3;
	if (InitLen > 0)
	{
		for (t = 0; t < InitLen; t++)
		{
			*ucBuffer = (UINT8)(rand() & 0xFF);
		}
	}
}

/*! ValidateUserStatus
*
* \brief Compares the UserStatus to the expected pattern specified
* \param params
* \param UserStatus
* \return result
*/
BOOLEAN CPM40Utils::ValidateUserStatus(PTHREAD_PARAMS params,UINT64 UserStatus)
{
	UINT64              Seed = 0;
	UINT32              SeedHigh;
	UINT32              SeedLow;
	BOOLEAN             result = TRUE;
	CString Buff;

	if (params->bUserInfoPatternSpecified)
	{
		Seed = params->UserInfoSeedPattern;
		if (UserStatus != Seed)
		{
			if (!params->bQuiet)
			{
				Buff.Format(L"User Status mistatch! Expected Value: 0x%llx, Status Value: 0x%0llx\n",
					(UINT64)Seed, UserStatus);
			}
			result = FALSE;
			params->iPass = ERROR_TRANSFER_DATA_MISMATCH;
		}
		SeedHigh = (Seed >> 32);
		SeedLow = GetNextPattern(params->UserInfoPatternType, params->UserInfoSeedSize, SeedHigh);
		SeedHigh = GetNextPattern(params->UserInfoPatternType, params->UserInfoSeedSize, SeedLow);
		params->UserInfoSeedPattern = SeedHigh;
		params->UserInfoSeedPattern = (params->UserInfoSeedPattern << 32) | SeedLow;
	}
	return result;
}

/*! GetNextpattern
*
* \brief Calculates the next 32 bit pattern based on the Type and Size
* \param Type
* \param Size
*/
UINT32 CPM40Utils::GetNextPattern(INT32 Type,INT32 Size,UINT32 Seed)
{
	UINT32              NewPattern = PATTERN_INCREMENTING;
	UINT8               ucSeqNum = 0;
	UINT8               ucSeed = 0;
	BOOLEAN             y1, y2, y3, y4, y5;

	if ((Type == PATTERN_INCREMENTING) ||
		(Type == PATTERN_SPECIFIED))
	{
		NewPattern = Seed;
		if (Type == PATTERN_INCREMENTING)
		{
			if (Size == 1)
			{
				ucSeed = (UINT8)Seed & 0xFF;
				ucSeqNum = (UINT8)(Seed >> 8) & 0xFF;
				ucSeed += 4;
				ucSeqNum += 4;
				NewPattern = ((ucSeqNum & 0xFF) << 24) | ((ucSeqNum & 0xFF) << 16) | ((ucSeqNum & 0xFF) << 8) | (ucSeed & 0xFF);
			}
			else
			{
				NewPattern++;
			}
		}
	}
	else if (Type == PATTERN_RANDOM)
	{
		y1 = (Seed & 0x80000000) ? 1 : 0;
		y2 = (Seed & 0x00200000) ? 1 : 0;
		y3 = (Seed & 0x00000400) ? 1 : 0;
		y4 = (Seed & 0x00000002) ? 1 : 0;
		y5 = (Seed & 0x00000001) ? 1 : 0;
		NewPattern = Seed << 1 | ((~(y1 ^ y2 ^ y3 ^ y4 ^ y5)) & 0x01);
		//Buff.Format(L"pattern: 0x%x\n", NewPattern);
	}
	return NewPattern;
}

/*! FreeMem
*
* \brief Frees memory previous allocated by BufferAllocAndSet
* \param params
* \return none
*/
VOID CPM40Utils::FreeMem(PTHREAD_PARAMS  params)
{
	CString Buff;

	if (params->Buffer != NULL)
	{
#ifdef WIN32
		if (!VirtualFree(params->Buffer, 0, MEM_RELEASE))
		{
			Buff.Format(L"Error %d: while freeing buffer!\n", GetLastError());
		}
#else // Linux
		free(params->Buffer);
#endif  // Windows vs. Linux
		params->Buffer = NULL;
	}
}

#ifndef WIN32  // Linux Version
/*! SignalHandler
*
* \brief Handles any Control-C, or other abrupt termination requests.
* \param sig
*/
static void
SignalHandler(INT32 sig)
{
	gpcfgParams->Seconds = 1;
	fprintf(stderr, "\nUser requested application stop, shutting down DmaDriverCli.\n");
}

/*! SignalIOHandler
*
* \brief Handles SIGIO sginals
* \param sig
*/
//static void
//SignalIOHandler(INT32 sig)
//{
//    Buff.Format(L"SIGIO: User Interrupt Signal Handler.\n");
//    //Buff.Format(L"sig: 0x%x \n", sig);
//}
#endif // Linux Version
