#include "stdafx.h"
#include "USLScanner.h"
#include "PM40.h"
#include "PM40Packets.h"

#include "PM40Utils.h"
extern	CPM40Utils g_PM40Utils;

CPM40Packets::CPM40Packets()
{
}


CPM40Packets::~CPM40Packets()
{
}

//---------------------------------------------------------------------------------------
//			Packet Stress Thread routines
//---------------------------------------------------------------------------------------

/*! Packet Recieve Stress Thread
*
* \brief Packet Receieve Stress Thread
* \param params
*/
VOID PM40PacketRecvThreadProc(PTHREAD_PARAMS params)
{
	UINT32			status;
	UINT32			i;
	UINT64			UserStatus;
	UINT32			BufferToken;
	PUINT8 			Buffer;
	UINT32			Length = (UINT32)-1;

	BufferToken = (UINT32)-1;	// On the first call we do not have a Rx buffer to return

	for (i = 0; i < params->Iterations; i++)
	{
		if (params->bBlockingAPIs)
		{
			status = PacketReceiveEx(
				params->cardNum,
				params->DMAEngineOffset,
				&UserStatus,
				&BufferToken,
				(PUINT8)&Buffer,
				&Length);
		}
		else
		{

			status = PacketReceiveNB(
				params->cardNum,
				params->DMAEngineOffset,
				&UserStatus,
				&BufferToken,
				(PUINT8)&Buffer,
				&Length);
		}
		// Check the status
		if (status != 0)
		{
			g_PM40Utils.DisplayErrorInfo(NULL, params, status);
			if (params->bStopOnError)
			{
				// Exit the loop and cleanup
				break;
			}
		}
		else
		{
			// Make sure the Buffer pointer and length are valid
			if ((Buffer != 0) && (Length != 0))
			{
				params->PacketCount++;
				params->FullPacketCount++;

				// Request ownership of the critical section.
				BEGIN_CRITICAL(&params->CriticalSection);
				params->TotalBytesTransferred += Length;
				// Release ownership of the critical section.
				END_CRITICAL(&params->CriticalSection);

				//printf("Data: Buffer Addr: 0x%lp, Ret Buffer = 0x%lp, Length = 0x%lLx\n", 
				//	params->Buffer, Buffer, Length);

				if (Buffer != NULL)
				{
					if (!g_PM40Utils.ValidateDataBuffer(params, Buffer, Length))
					{
						if (params->bStopOnError)
						{
							// Exit the loop and cleanup
							break;
						}
					}
					if (!g_PM40Utils.ValidateUserStatus(params, UserStatus))
					{
						if (params->bStopOnError)
						{
							// Exit the loop and cleanup
							break;
						}
					}
					if (params->bDebug)
					{
						if (params->pcfgParams->bSecondRefresh)
						{
							params->pcfgParams->bSecondRefresh = FALSE;
							g_PM40Utils.DisplayFormattedBytes(
								params->pcfgParams,
								(UINT64)Buffer,
								(PUINT8)Buffer,
								(INT32)min(256, Length));
						}
					}
				}
				else
				{
					printf("PacketRecvThreadProc: returned a NULL buffer, Length = %d\n", Length);
				}
			}
			else
			{
				// Special Statistics
				params->ZeroPacketCount++;
			}
		}
		// See if we are still supposed to run, another thread may want to shut us down.
		if (!params->Running)
		{
			break;
		}
	}
	if ((int)BufferToken != -1)
	{
		PacketReturnReceive(
			params->cardNum,
			params->DMAEngineOffset,
			&BufferToken);
	}
	params->Running = FALSE;
	Sleep(10);
}

/*! Packet Recieves Stress Thread (Multiple Packets per call)
*
* \brief
* \param params
*/
VOID PM40PacketRecvsThreadProc(PTHREAD_PARAMS params)
{
	PPACKET_RECVS_STRUCT	pPacketRecvs = NULL; // Pointer to Packet Receives struct
	UINT32					status;
	UINT32					i;
	UINT32					t;
	PUINT8 					Buffer;
	UINT32					Length = (UINT32)-1;

	pPacketRecvs = (PPACKET_RECVS_STRUCT)malloc(sizeof(PACKET_RECVS_STRUCT) +
		(NUMBER_PACKET_RECVS * sizeof(PACKET_ENTRY_STRUCT)));

	if (pPacketRecvs == NULL)
	{
		printf("Failed to allocate Packet Receives structure.\n");
		return;
	}

	// Do one Packet Recieves to clean out any existing overrun errors
	// while we where setting up this thread. Throw away the results.
	pPacketRecvs->AvailNumEntries = NUMBER_PACKET_RECVS;
	pPacketRecvs->RetNumEntries = 0;
	status = PacketReceives(
		params->cardNum,
		params->DMAEngineOffset,
		pPacketRecvs);

	for (i = 0; i < params->Iterations; i++)
	{
		pPacketRecvs->AvailNumEntries = NUMBER_PACKET_RECVS;
		pPacketRecvs->RetNumEntries = 0;
		status = PacketReceives(
			params->cardNum,
			params->DMAEngineOffset,
			pPacketRecvs);
		// Check the status
		if (status != 0)
		{
			g_PM40Utils.DisplayErrorInfo(NULL, params, status);
			if (params->bStopOnError)
			{
				// Exit the loop and cleanup
				break;
			}
		}
		else
		{
			if (pPacketRecvs->EngineStatus & (DMA_OVERRUN_ERROR | PACKET_ERROR_MALFORMED))
			{
				if (params->bDebug)
				{
					printf("Packet Receives returned DMA Engine error: 0x%x.\n", pPacketRecvs->EngineStatus);
				}
				if (params->bStopOnError)
				{
					// Exit the loop and cleanup
					params->Running = FALSE;
					return;
				}
			}
			if (pPacketRecvs->EngineStatus == DMA_OVERRUN_ERROR)
			{
				params->DMAOverrunCount++;
			}
			if (pPacketRecvs->EngineStatus == PACKET_ERROR_MALFORMED)
			{
				params->DMADescErrorCount++;
			}
			// Special Statistics
			if (pPacketRecvs->RetNumEntries == 0)
				params->ZeroPacketCount++;
			else if (pPacketRecvs->RetNumEntries == pPacketRecvs->AvailNumEntries)
				params->FullPacketCount++;
			else
				params->PartialPacketCount++;

			for (t = 0; t < pPacketRecvs->RetNumEntries; t++)
			{
				Buffer = (PUINT8)pPacketRecvs->Packets[t].Address;
				Length = pPacketRecvs->Packets[t].Length;

				// Make sure the Buffer pointer and length are valid
				if ((Buffer != 0) &&
					(Length != 0) &&
					(pPacketRecvs->Packets[t].Status == 0))
				{
					params->PacketCount++;

					// Request ownership of the critical section.
					BEGIN_CRITICAL(&params->CriticalSection);
					params->TotalBytesTransferred += Length;
					// Release ownership of the critical section.
					END_CRITICAL(&params->CriticalSection);

					if (!g_PM40Utils.ValidateDataBuffer(params, Buffer, Length))
					{
						if (params->bStopOnError)
						{
							// Exit the loop and cleanup
							params->Running = FALSE;
							return;
						}
					}
					if (params->bDebug)
					{
						if (params->pcfgParams->bSecondRefresh)
						{
							params->pcfgParams->bSecondRefresh = FALSE;
							g_PM40Utils.DisplayFormattedBytes(
								params->pcfgParams,
								(UINT64)Buffer,
								(PUINT8)Buffer,
								(INT32)min(256, Length));
						}
					}
					if (!g_PM40Utils.ValidateUserStatus(params, pPacketRecvs->Packets[t].UserStatus))
					{
						if (params->bStopOnError)
						{
							// Exit the loop and cleanup
							params->Running = FALSE;
							return;
						}
					}
				}
				else
				{
					if (!params->bQuiet)
					{
						printf("Invalid Packet recieved, Buffer=0x%p, Length=%x, Status=0x%x\n",
							Buffer, Length, pPacketRecvs->Packets[t].Status);
					}
					params->iPass = ERROR_TRANSFER_COUNT_MISMATCH;
					if (params->bStopOnError)
					{
						// Exit the loop and cleanup
						params->Running = FALSE;
						return;
					}
				}
			} // for loop
		}
		// See if we are still supposed to run, another thread may want to shut us down.
		if (!params->Running)
		{
			break;
		}
	}
	params->Running = FALSE;
	if (pPacketRecvs != NULL)
	{
		free(pPacketRecvs);
	}

	Sleep(10);
}

/*! Packet Send Stress Thread
*
* \brief
* \param params
*/
VOID PM40PacketSendThreadProc(PTHREAD_PARAMS params)
{
	UINT32			status;
	UINT32			i;

	for (i = 0; i < params->Iterations; i++)
	{
		if (params->DataPatternType == PATTERN_RANDOM)
		{
			g_PM40Utils.WriteRandomBuffer(params, params->MaxPacketSize);
		}
		if (params->bDebug)
		{
			if (params->pcfgParams->bSecondRefresh)
			{
				params->pcfgParams->bSecondRefresh = FALSE;
				g_PM40Utils.DisplayFormattedBytes(
					params->pcfgParams,
					(UINT64)params->Buffer,
					(PUINT8)params->Buffer,
					(INT32)min(256,
					params->MaxPacketSize));
			}
		}
		// Send the packet off via the DLL interface
		status = PacketSendEx(
			params->cardNum,
			params->DMAEngineOffset,
			params->UserInfo,
			0,
			(PUINT8)params->Buffer,
			params->MaxPacketSize);
		// Check the status
		if (status != 0)
		{
			g_PM40Utils.DisplayErrorInfo(NULL, params, status);
			// Break out of the loop and cleanup
			break;
		}
		else
		{
			params->PacketCount++;

			// Request ownership of the critical section.
			BEGIN_CRITICAL(&params->CriticalSection);
			params->TotalBytesTransferred += params->MaxPacketSize;
			// Release ownership of the critical section.
			END_CRITICAL(&params->CriticalSection);
		}
		// See if we are still supposed to run, another thread may want to shut us down.
		if (!params->Running)
		{
			break;
		}
	}
	params->Running = FALSE;
	Sleep(10);
}

/*! Packet Loop Stress Thread
*
* \brief
* \param pparams
*/
VOID PM40PacketLoopThreadProc(PTHREAD_PARAMS 	pparams)
{
	PTHREAD_PARAMS	ReadParams;
	PTHREAD_PARAMS	WriteParams;
	UINT64			UserControl;
	UINT64			UserStatus;
	UINT32			TransferSize;
	UINT32			BufferToken;
	UINT32			status;
	UINT32          i;
	UINT32          t;
	PUINT8 			Buffer;
	UINT32			Length = (UINT32)-1;

	ReadParams = pparams;
	pparams++;
	WriteParams = pparams;

	TransferSize = WriteParams->MaxPacketSize;

	BufferToken = (UINT32)-1;	// On the first call we do not have a Rx buffer to return

	for (i = 0; i < WriteParams->Iterations; i++)
	{
		if (WriteParams->bRandomize)
		{
			TransferSize = rand() % WriteParams->AllocSize + 1;
		}
		if (WriteParams->DataPatternType == PATTERN_RANDOM)
		{
			g_PM40Utils.WriteRandomBuffer(WriteParams, TransferSize);
		}

		UserControl = rand();
		// Put the Packet number in the Buffer data
		status = PacketSendEx(
			WriteParams->cardNum,
			WriteParams->DMAEngineOffset,
			UserControl,
			0,
			(PUINT8)WriteParams->Buffer,
			TransferSize);
		// Check the status
		if (status != 0)
		{
			g_PM40Utils.DisplayErrorInfo(NULL, WriteParams, status);
			// Break out of the loop and cleanup
			break;
		}
		WriteParams->PacketCount++;

		// Request ownership of the critical section.
		BEGIN_CRITICAL(&WriteParams->CriticalSection);
		WriteParams->TotalBytesTransferred += TransferSize;
		// Release ownership of the critical section.
		END_CRITICAL(&WriteParams->CriticalSection);

		if (ReadParams->bBlockingAPIs)
		{
			status = PacketReceiveEx(
				ReadParams->cardNum,
				ReadParams->DMAEngineOffset,
				&UserStatus,
				&BufferToken,
				(PUINT8)&Buffer,
				&Length);
		}
		else
		{
			status = PacketReceiveNB(
				ReadParams->cardNum,
				ReadParams->DMAEngineOffset,
				&UserStatus,
				&BufferToken,
				(PUINT8)&Buffer,
				&Length);

		}
		// Check the status
		if (status != 0)
		{
			g_PM40Utils.DisplayErrorInfo(NULL, ReadParams, status);
			// Exit the loop and cleanup
			break;
		}
		else
		{
			// Make sure the Buffer pointer and length are valid
			if ((Buffer != 0) && (Length != 0))
			{
				if (Length != TransferSize)
				{
					printf("Receive Size mismatch, Write Value: %d, Read Value: %d\n", TransferSize, Length);
				}
				for (t = 0; t < TransferSize; t++)
				{
					if (ReadParams->Buffer[t] != WriteParams->Buffer[t])
					{
						if (!ReadParams->bQuiet)
						{
							printf("Data mistatch at offset %d, Write Value: 0x%x, Read Value: 0x%x\n",
								t, WriteParams->Buffer[t], ReadParams->Buffer[t]);
						}
						ReadParams->iPass = ERROR_TRANSFER_DATA_MISMATCH;
						if (WriteParams->bStopOnError)
						{
							// stop here
							WriteParams->Running = FALSE;
							break;
						}
					}
				}
				if (ReadParams->bDebug)
				{
					if (ReadParams->pcfgParams->bSecondRefresh)
					{
						ReadParams->pcfgParams->bSecondRefresh = FALSE;
						g_PM40Utils.DisplayFormattedBytes(
							ReadParams->pcfgParams,
							(UINT64)ReadParams->Buffer,
							(PUINT8)ReadParams->Buffer,
							(INT32)min(256, Length));
					}
				}
				if (UserStatus != UserControl)
				{
					ReadParams->iPass = ERROR_TRANSFER_DATA_MISMATCH;
					printf("Invalid Packet UserControl/Status, Control=0x%llx, Status=0x%llx\n", UserControl, UserStatus);
					if (WriteParams->bStopOnError)
					{
						// stop here
						WriteParams->Running = FALSE;
						break;
					}
				}
				ReadParams->PacketCount++;
				// Request ownership of the critical section.
				BEGIN_CRITICAL(&ReadParams->CriticalSection);
				ReadParams->TotalBytesTransferred, Length += Length;
				// Release ownership of the critical section.
				END_CRITICAL(&ReadParams->CriticalSection);
			}
			else
			{
				if (WriteParams->bBlockingAPIs)
				{
					if (!WriteParams->bQuiet)
					{
						printf("Invalid Packet recieved, Buffer=0x%p, Length=%d\n", Buffer, Length);
					}
					ReadParams->iPass = ERROR_TRANSFER_COUNT_MISMATCH;
				}
			}
		}
		// See if we are still supposed to run, another thread may want to shut us down.
		if (!WriteParams->Running || !ReadParams->Running)
		{
			break;
		}
	}
	if ((int)BufferToken != -1)
	{
		PacketReturnReceive(ReadParams->cardNum,
			ReadParams->DMAEngineOffset,
			&BufferToken);
	}
	WriteParams->Running = FALSE;
	ReadParams->Running = FALSE;
	Sleep(10);
}

//---------------------------------------------------------------------------------------
//			FIFO Packet Mode Stress Testing Routines
//---------------------------------------------------------------------------------------

/*! StressPacketSend
*
* \brief Function to handle the "PacketSend" command.
*  Calls the DmaDriver DLL to do the work.
* \param pcfgParams
* \return 0, -2
*/
INT32 CPM40Packets::stressPacketSend(PCFG_PARAMS pcfgParams)
{
	THREAD_PARAMS	params[MAX_THREADS];

	UINT32			NumberPackets;
	UINT32			i;
	BOOLEAN			Running;

	pcfgParams->TestType = PACKET_SEND;

	NumberPackets = pcfgParams->Iterations;
	pcfgParams->BufferSize = DEFAULT_MAX_PACKET_SIZE;
	pcfgParams->MaxPacketSize = DEFAULT_MAX_PACKET_SIZE;

	// get the parameters from the command line, if any
	if (pcfgParams->CmdArgc > 1)
	{
		pcfgParams->BufferSize = (UINT64)g_PM40Utils.__strtoull(pcfgParams->CmdArgv[1]);
		if (pcfgParams->BufferSize > 0x00000000ffffffff)
		{
			printf("Max Packet Size (%lld) is too large\n", pcfgParams->BufferSize);
			return -2;
		}
		pcfgParams->MaxPacketSize = (UINT32)pcfgParams->BufferSize;
		if (pcfgParams->MaxPacketSize > PACKET_GENERATOR_MAX_PACKET_SIZE)
		{
			printf("Max Packet Size (0x%x) is too large for the Packet Checker.\n\tThe maximum size is %d\n",
				pcfgParams->MaxPacketSize, PACKET_GENERATOR_MAX_PACKET_SIZE);
			return -2;
		}
	}

	// Make sure there we do at least one transfer for each thread
	if (NumberPackets < (UINT32)pcfgParams->NumberThreads)
	{
		NumberPackets = pcfgParams->NumberThreads;
	}

	// print debug messages
	if (pcfgParams->bVerbose == TRUE)
	{
		printf("Number Packets = %d\n", (INT32)NumberPackets);
		printf("MaxPacketSize = %d\n", (INT32)pcfgParams->MaxPacketSize);
		printf("Threads = %d\n", pcfgParams->NumberThreads);
	}

	// In the event of a bad shutdown, prior to now.
	ResetDMAEngine(
		pcfgParams->cardNum,
		pcfgParams->DMAEngineOffset,
		(DMA_CAP_SYSTEM_TO_CARD | DMA_CAP_PACKET_DMA));

	// Setup the packet checker - must have something to recieve packets
	if (g_PM40Utils.ConfigPacketGenChk(pcfgParams, PACKET_GEN_ENABLE, S2C_DIRECTION, NumberPackets) != STATUS_SUCCESSFUL)
	{
		printf("Setup Packet Checker failed, Max Packet Size (0x%x) is too large.\n", pcfgParams->MaxPacketSize);
		return -2;
	}

	// Make sure we have enough packets for all the threads
	NumberPackets = NumberPackets / pcfgParams->NumberThreads;
	for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
	{
		g_PM40Utils.InitThreadParams(pcfgParams, &params[i]);
		params[i].Iterations = NumberPackets;
	}

	for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
	{
		if (g_PM40Utils.BufferAllocAndSet(pcfgParams, &params[i]))
		{
			goto ShutdownStressPacketSend;
		}
	}
	// Now we have the buffers we need start the test threads
	for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
	{
		g_PM40Utils.StartThread(&params[i], (LPTHREAD_START_ROUTINE)PM40PacketSendThreadProc);
	}

	g_PM40Utils.ClearAllStats(
		pcfgParams,
		&params[0],
		pcfgParams->NumberThreads,
		CLI_SYSTEM_TO_CARD);
	g_PM40Utils.InitSummaryStats(pcfgParams);

	// Loop forever until all threads are finished
	Running = TRUE;
	while (Running)
	{
		if (g_PM40Utils.CollectAllStats(pcfgParams, &params[0], pcfgParams->NumberThreads, CLI_SYSTEM_TO_CARD) == 0)
		{
			Running = g_PM40Utils.TestRunControl(pcfgParams, &params[0]);
		}
		else
		{
			Running = FALSE;
		}
	}
	// Display the summaries from this test run
	g_PM40Utils.DisplaySummaryInfo(pcfgParams);

ShutdownStressPacketSend:
	for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
	{
		// Free the buffer that was allocated for each thread
		g_PM40Utils.FreeMem(&params[i]);
	}

	// Shutdown the Packet Checker
	g_PM40Utils.ConfigPacketGenChk(
		pcfgParams,
		0,
		S2C_DIRECTION,
		0);

	if (pcfgParams->bVerbose == TRUE)
	{
		printf("\nStress Packet Send completed.\n");
	}
	return 0;
}

/*! StressPacketReceive
*
* \brief Function to handle the "packetRecv" command.
*  Calls the DmaDriver DLL to do the work.
* \param pcfgParams
* \return 0, -2
*/
INT32 CPM40Packets::stressPacketReceive(PCFG_PARAMS	pcfgParams)
{
	THREAD_PARAMS	params[MAX_THREADS];
	THREAD_PARAMS	RxParams;
	UINT32			NumberPackets;
	UINT32			i;
	UINT32			status;
	BOOLEAN			Running;
	UINT32			FullPacketCount = 0;
	UINT32			ZeroPacketCount = 0;

	pcfgParams->TestType = PACKET_RECV;

	// get a parameters from the command line
	NumberPackets = pcfgParams->Iterations;

	// We are here because the Application needs to provide the Receive Bufffer
	pcfgParams->MaxPacketSize = DEFAULT_MAX_PACKET_SIZE;
	pcfgParams->BufferSize = DEFAULT_BUFFER_SIZE;

	if (pcfgParams->CmdArgc > 1)
	{
		pcfgParams->MaxPacketSize = (UINT32)g_PM40Utils.__strtoul(pcfgParams->CmdArgv[1]);
	}
	if (pcfgParams->CmdArgc > 2)
	{
		pcfgParams->BufferSize = (UINT64)g_PM40Utils.__strtoull(pcfgParams->CmdArgv[2]);
	}
	if (pcfgParams->MaxPacketSize > PACKET_GENERATOR_MAX_PACKET_SIZE)
	{
		printf("Max Packet Size (0x%x) is too large for the Packet Generator.\n\tThe maximum size is %d\n",
			pcfgParams->MaxPacketSize, PACKET_GENERATOR_MAX_PACKET_SIZE);
		return -2;
	}

	g_PM40Utils.InitThreadParams(pcfgParams, &RxParams);
	// Allocate and setup the Receive Buffer
	if (g_PM40Utils.BufferAllocAndSet(pcfgParams, &RxParams))
	{
		return -1;
	}

	// In the event of a bad shutdown, prior to now.
	ResetDMAEngine(
		pcfgParams->cardNum,
		pcfgParams->DMAEngineOffset,
		(DMA_CAP_CARD_TO_SYSTEM | DMA_CAP_PACKET_DMA));

	status = SetupPacketMode(
		pcfgParams->cardNum,
		pcfgParams->DMAEngineOffset,
		(PUINT8)RxParams.Buffer,
		(PUINT32)&RxParams.AllocSize,
		&RxParams.MaxPacketSize,
		PACKET_MODE_FIFO,
		0);
	if (status != STATUS_SUCCESSFUL)
	{
		g_PM40Utils.FreeMem(&RxParams);
		g_PM40Utils.DisplayErrorInfo(pcfgParams, NULL, status);
		return -1;
	}

	// Setup the Packet Generator which will provide us a steady stream of recieve packets courtesy of the h/w.
	if (g_PM40Utils.ConfigPacketGenChk(pcfgParams, PACKET_GEN_ENABLE, C2S_DIRECTION, NumberPackets) != STATUS_SUCCESSFUL)
	{
		g_PM40Utils.FreeMem(&RxParams);
		printf("Setup Packet Generator failed, Max Packet Size (0x%x) is too large\n", pcfgParams->MaxPacketSize);
		return -2;
	}

	// Make sure there we do at least one transfer for each thread
	if (NumberPackets < (UINT32)pcfgParams->NumberThreads)
	{
		NumberPackets = pcfgParams->NumberThreads;
	}

	// print debug messages
	if (pcfgParams->bVerbose == TRUE)
	{
		printf("Number Packets = %d\n", (INT32)NumberPackets);
		printf("MaxPacketSize = %d\n", (INT32)pcfgParams->MaxPacketSize);
		printf("Buffer Size = %lld\n", pcfgParams->BufferSize);
#ifdef WIN32	// Windows version
		printf("Alloc Buffer Size = %Id\n", RxParams.AllocSize);  // %llu
#else // Linux version
		printf("Alloc Buffer Size = %zd\n", RxParams.AllocSize);
#endif	// Windows vs. Linux
		printf("Threads = %d\n", pcfgParams->NumberThreads);
	}

	NumberPackets = NumberPackets / pcfgParams->NumberThreads;
	for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
	{
		g_PM40Utils.InitThreadParams(pcfgParams, &params[i]);
		params[i].Iterations = NumberPackets;
		params[i].Buffer = RxParams.Buffer;
		params[i].AllocSize = RxParams.AllocSize;

		g_PM40Utils.StartThread(&params[i], (LPTHREAD_START_ROUTINE)PM40PacketRecvThreadProc);
	}

	g_PM40Utils.ClearAllStats(pcfgParams, &params[0], pcfgParams->NumberThreads, CLI_CARD_TO_SYSTEM);
	g_PM40Utils.InitSummaryStats(pcfgParams);

	// Loop forever until all threads are finished
	Running = TRUE;
	while (Running)
	{
		if (g_PM40Utils.CollectAllStats(pcfgParams, &params[0], pcfgParams->NumberThreads, CLI_CARD_TO_SYSTEM) == 0)
		{
			Running = g_PM40Utils.TestRunControl(pcfgParams, &params[0]);
		}
		else
		{
			Running = FALSE;
		}
	}

	// Shutdown the Packet Generator
	g_PM40Utils.ConfigPacketGenChk(
		pcfgParams,
		0,
		C2S_DIRECTION,
		0);

	// Do an orderly shutdwon by releasing the packet buffers from the driver
	ShutdownPacketMode(
		pcfgParams->cardNum,
		pcfgParams->DMAEngineOffset);
	// Then free the allocated memory

	// Display the summaries frm this test run
	g_PM40Utils.DisplaySummaryInfo(pcfgParams);

	g_PM40Utils.FreeMem(&RxParams);

	if (pcfgParams->bVerbose == TRUE)
	{
		for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
		{
			ZeroPacketCount += params[i].ZeroPacketCount;
			FullPacketCount += params[i].FullPacketCount;
		}
		printf("Packet Stats: Full %d, Empty %d.\n",
			FullPacketCount, ZeroPacketCount);
		// print debug messages
		printf("\nStress Packet Receive Complete.\n");
	}
	return 0;
}

/*! StressPacketReceives
*
* \brief Function to handle the "packetRecvs" command.
*  Calls the DmaDriver DLL to do the work.
* \param pcfgParams
*/
INT32 CPM40Packets::stressPacketReceives(PCFG_PARAMS	pcfgParams)
{
	THREAD_PARAMS		params[MAX_THREADS];
	THREAD_PARAMS		RxParams;
	UINT32				NumberPackets;
	UINT32				i;
	UINT32				status;
	UINT32				OverrunErrorCount = 0;
	UINT32				DescErrorCount = 0;
	UINT32				FullPacketCount = 0;
	UINT32				PartialPacketCount = 0;
	UINT32				ZeroPacketCount = 0;
	BOOLEAN				Running;

	pcfgParams->TestType = PACKET_RECVS;

	// get a parameters from the command line
	NumberPackets = pcfgParams->Iterations;

	// We are here because the Application needs to provide the Receive Bufffer
	pcfgParams->MaxPacketSize = DEFAULT_MAX_PACKET_SIZE;
	pcfgParams->BufferSize = DEFAULT_BUFFER_SIZE;

	if (pcfgParams->CmdArgc > 1)
	{
		pcfgParams->MaxPacketSize = (UINT32)g_PM40Utils.__strtoul(pcfgParams->CmdArgv[1]);
	}
	if (pcfgParams->CmdArgc > 2)
	{
		pcfgParams->BufferSize = (UINT64)g_PM40Utils.__strtoull(pcfgParams->CmdArgv[2]);
	}
	if (pcfgParams->MaxPacketSize > PACKET_GENERATOR_MAX_PACKET_SIZE)
	{
		printf("Max Packet Size (0x%x) is too large for the Packet Generator.\n\tThe maximum size is %d\n",
			pcfgParams->MaxPacketSize, PACKET_GENERATOR_MAX_PACKET_SIZE);
		return -2;
	}
	if (pcfgParams->bPatternSpecified)
	{
		printf("Pattern generation not supported in this command.\n");
		pcfgParams->bPatternSpecified = FALSE;
	}
	if (pcfgParams->bRandomize)
	{
		printf("Randomization not supported in this command.\n");
		pcfgParams->bRandomize = FALSE;
	}

	g_PM40Utils.InitThreadParams(pcfgParams, &RxParams);
	// Allocate and setup the Receive Buffer
	if (g_PM40Utils.BufferAllocAndSet(pcfgParams, &RxParams))
	{
		return -1;
	}

	// In the event of a bad shutdown, prior to now.
	ResetDMAEngine(pcfgParams->cardNum, pcfgParams->DMAEngineOffset, (DMA_CAP_CARD_TO_SYSTEM | DMA_CAP_PACKET_DMA));

	status = SetupPacketMode(
		pcfgParams->cardNum,
		pcfgParams->DMAEngineOffset,
		(PUINT8)RxParams.Buffer,
		(PUINT32)&RxParams.AllocSize,
		&RxParams.MaxPacketSize,
		PACKET_MODE_STREAMING,
		0);
	if (status != STATUS_SUCCESSFUL)
	{
		g_PM40Utils.FreeMem(&RxParams);
		printf("Setup Packet Mode failed: ");
		g_PM40Utils.DisplayErrorInfo(pcfgParams, NULL, status);
		return -1;
	}

	// Setup the Packet Generator which will provide us a steady stream of recieve packets courtesy of the h/w.
	if (g_PM40Utils.ConfigPacketGenChk(pcfgParams, PACKET_GEN_ENABLE, C2S_DIRECTION, NumberPackets) != STATUS_SUCCESSFUL)
	{
		g_PM40Utils.FreeMem(&RxParams);
		printf("Setup Packet Generator failed, Max Packet Size (0x%x) is too large\n", pcfgParams->MaxPacketSize);
		return -2;
	}

	// Make sure there we do at least one transfer for each thread
	if (NumberPackets < (UINT32)pcfgParams->NumberThreads)
	{
		NumberPackets = pcfgParams->NumberThreads;
	}

	// print debug messages
	if (pcfgParams->bVerbose == TRUE)
	{
		printf("Number Packets = %d\n", (INT32)NumberPackets);
		printf("MaxPacketSize = %d\n", (INT32)pcfgParams->MaxPacketSize);
		printf("Buffer Size = %lld\n", pcfgParams->BufferSize);
#ifdef WIN32	// Windows version
		printf("Alloc Buffer Size = %Id\n", RxParams.AllocSize);
#else // Linux version
		printf("Alloc Buffer Size = %zd\n", RxParams.AllocSize);
#endif	// Windows vs. Linux
		printf("Threads = %d\n", pcfgParams->NumberThreads);
	}

	NumberPackets = NumberPackets / pcfgParams->NumberThreads;
	for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
	{
		g_PM40Utils.InitThreadParams(pcfgParams, &params[i]);
		params[i].Iterations = NumberPackets;
		params[i].Buffer = RxParams.Buffer;
		params[i].AllocSize = RxParams.AllocSize;

		g_PM40Utils.StartThread(&params[i], (LPTHREAD_START_ROUTINE)PM40PacketRecvsThreadProc);
	}

	g_PM40Utils.ClearAllStats(pcfgParams, &params[0], pcfgParams->NumberThreads, CLI_CARD_TO_SYSTEM);
	g_PM40Utils.InitSummaryStats(pcfgParams);

	// Loop forever until all threads are finished
	Running = TRUE;
	while (Running)
	{
		if (g_PM40Utils.CollectAllStats(pcfgParams, &params[0], pcfgParams->NumberThreads, CLI_CARD_TO_SYSTEM) == 0)
		{
			Running = g_PM40Utils.TestRunControl(pcfgParams, &params[0]);
		}
		else
		{
			Running = FALSE;
		}
	}

	// Shutdown the Packet Generator
	g_PM40Utils.ConfigPacketGenChk(pcfgParams, 0, C2S_DIRECTION, 0);

	// Do an orderly shutdwon by releasing the packet buffers from the driver
	ShutdownPacketMode(pcfgParams->cardNum, pcfgParams->DMAEngineOffset);
	// Then free the allocated memory

	// Display the summaries frm this test run
	g_PM40Utils.DisplaySummaryInfo(pcfgParams);

	g_PM40Utils.FreeMem(&RxParams);

	if (pcfgParams->bVerbose == TRUE)
	{
		for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
		{
			OverrunErrorCount += params[i].DMAOverrunCount;
			DescErrorCount += params[i].DMADescErrorCount;
			ZeroPacketCount += params[i].ZeroPacketCount;
			PartialPacketCount += params[i].PartialPacketCount;
			FullPacketCount += params[i].FullPacketCount;
		}
		if (OverrunErrorCount > 0)
		{
			printf("Stress Packet Receives recorded 0x%x DMA Engine Overrun Errors\n", OverrunErrorCount);
		}
		if (DescErrorCount > 0)
		{
			printf("Stress Packet Receives recorded 0x%x DMA Engine Bad DMA Decriptors Errors\n", DescErrorCount);
		}
		printf("Packet Stats: Full %d, Partial %d, Empty %d\n",
			(INT32)FullPacketCount, (INT32)PartialPacketCount, (INT32)ZeroPacketCount);

		// print debug messages
		printf("\nStress Packet Receive Complete\n");
	}
	return 0;
}

/*! PacketLoopStress
*
* \brief Function to handle the "PacketLoop" command.
*  Calls the DmaDriver DLL to do the work.
* \param pcfgParams
* \return
*/
INT32 CPM40Packets::stressPacketLoop(PCFG_PARAMS		pcfgParams)
{
	THREAD_PARAMS	params[NUM_STRESS_PARAMS];
	UINT32			status;
	UINT32			NumberPackets;
	UINT32			i;
	BOOLEAN			Running;

	pcfgParams->TestType = PACKET_LOOP;

	/*! \note: For PacketLoop, the driver Hard sets the loopback to a
	* single thread. It is currently not designed to handle more than
	* one because the recieve handler is in the Loop thread routine.
	*/
	pcfgParams->NumberThreads = 1;

	// get a parameters from the command line
	pcfgParams->MaxPacketSize = DEFAULT_MAX_PACKET_SIZE;
	pcfgParams->BufferSize = DEFAULT_BUFFER_SIZE;

	if (pcfgParams->CmdArgc > 1)
	{
		pcfgParams->MaxPacketSize = (UINT32)g_PM40Utils.__strtoul(pcfgParams->CmdArgv[1]);
	}
	if (pcfgParams->CmdArgc > 2)
	{
		pcfgParams->BufferSize = (UINT64)g_PM40Utils.__strtoull(pcfgParams->CmdArgv[2]);
	}
	if (pcfgParams->BufferSize < (pcfgParams->MaxPacketSize * 10))
	{
		pcfgParams->BufferSize = pcfgParams->MaxPacketSize * 10;
	}

	g_PM40Utils.InitThreadParams(pcfgParams, &params[READ_STRESS]);
	// Allocate and setup the Receive Buffer
	if (g_PM40Utils.BufferAllocAndSet(pcfgParams, &params[READ_STRESS]))
	{
		return -1;
	}

	g_PM40Utils.InitThreadParams(pcfgParams, &params[WRITE_STRESS]);
	// Make the send packet size the Max Packet Size for allocation.
	pcfgParams->BufferSize = pcfgParams->MaxPacketSize;
	// Allocate and setup the Send Buffer
	if (g_PM40Utils.BufferAllocAndSet(pcfgParams, &params[WRITE_STRESS]))
	{
		g_PM40Utils.FreeMem(&params[READ_STRESS]);
		return -1;
	}

	// Make sure there we do at least one transfer for each thread
	NumberPackets = pcfgParams->Iterations;
	if (NumberPackets < (UINT32)pcfgParams->NumberThreads)
	{
		NumberPackets = pcfgParams->NumberThreads;
	}

	/*! \note Due to issues with improper shutdown and cleanup of the PacketGenerator/Checker,
	*  a reset is necessary to ensure a proper state for both C2S/S2C engines.
	*/
	ResetDMAEngine(pcfgParams->cardNum, pcfgParams->DMAEngineOffset, (DMA_CAP_SYSTEM_TO_CARD | DMA_CAP_PACKET_DMA));
	ResetDMAEngine(pcfgParams->cardNum, pcfgParams->DMAEngineOffset, (DMA_CAP_CARD_TO_SYSTEM | DMA_CAP_PACKET_DMA));

	// Turn on Loopback on both the S2C and C2S DMA Engines
	if (g_PM40Utils.ConfigPacketGenChk(pcfgParams, PACKET_GEN_LOOPBACK_ENABLE, S2C_DIRECTION, NumberPackets) != STATUS_SUCCESSFUL)
	{
		g_PM40Utils.FreeMem(&params[READ_STRESS]);
		g_PM40Utils.FreeMem(&params[WRITE_STRESS]);
		printf("Setup Packet Gen/Checker failed.\n");
		return -2;
	}

	status = SetupPacketMode(
		pcfgParams->cardNum,
		pcfgParams->DMAEngineOffset,
		(PUINT8)params[READ_STRESS].Buffer,
		(PUINT32)&params[READ_STRESS].AllocSize,
		&params[0].MaxPacketSize,
		PACKET_MODE_FIFO,
		0);
	if (status != STATUS_SUCCESSFUL)
	{
		g_PM40Utils.FreeMem(&params[READ_STRESS]);
		g_PM40Utils.FreeMem(&params[WRITE_STRESS]);
		g_PM40Utils.DisplayErrorInfo(pcfgParams, NULL, status);
		return -1;
	}

	// Startup the Send thread(s)
	if (g_PM40Utils.StartThread(&params[0], (LPTHREAD_START_ROUTINE)PM40PacketLoopThreadProc) != 0)
	{
		ShutdownPacketMode(
			pcfgParams->cardNum,
			pcfgParams->DMAEngineOffset);
		g_PM40Utils.FreeMem(&params[READ_STRESS]);
		g_PM40Utils.FreeMem(&params[WRITE_STRESS]);
		// print debug messages
		printf("Test failed to start thread, exiting.\n");
		return -2;
	}

	// print debug messages
	if (pcfgParams->bVerbose == TRUE)
	{
		printf("Number Packets = %d.\n", (INT32)NumberPackets);
		printf("MaxPacketSize = %d\n", (INT32)pcfgParams->MaxPacketSize);
		printf("Send Buffer Allocated Size per thread = %d.\n", (INT32)pcfgParams->MaxPacketSize);
	}

	// Read and discard first stats due to startup time.
#ifdef WIN32
	g_PM40Utils.ClearAllStats(pcfgParams, &params[0], NUM_STRESS_PARAMS,
	enum CLI_DMA_DIRECTION(CLI_SYSTEM_TO_CARD | CLI_CARD_TO_SYSTEM));
#else
	ClearAllStats(pcfgParams, &params[0], NUM_STRESS_PARAMS,
		CLI_DMA_DIRECTION(CLI_SYSTEM_TO_CARD | CLI_CARD_TO_SYSTEM));
#endif // Windows v. Linux
	g_PM40Utils.InitSummaryStats(pcfgParams);

	// Loop forever until all threads are finished
	Running = TRUE;
	while (Running)
	{
#ifdef WIN32
		if (g_PM40Utils.CollectAllStats(pcfgParams, &params[0], NUM_STRESS_PARAMS,
		enum CLI_DMA_DIRECTION(CLI_SYSTEM_TO_CARD | CLI_CARD_TO_SYSTEM)) == 0)
#else
		if (CollectAllStats(pcfgParams, &params[0], NUM_STRESS_PARAMS,
			CLI_DMA_DIRECTION(CLI_SYSTEM_TO_CARD | CLI_CARD_TO_SYSTEM)) == 0)
#endif // Windows v. Linux
		{
			Running = g_PM40Utils.TestRunControl(pcfgParams, &params[0]);
		}
		else
		{
			Running = FALSE;
		}
	}
	// Display the summaries from this test run
	g_PM40Utils.DisplaySummaryInfo(pcfgParams);

	// Turn off Loopback on both the S2C and C2S DMA Engines
	g_PM40Utils.ConfigPacketGenChk(pcfgParams, 0, S2C_DIRECTION, 0);
	g_PM40Utils.ConfigPacketGenChk(pcfgParams, 0, C2S_DIRECTION, 0);

	// Do an orderly shutdwon by releasing the packet buffers from the driver
	ShutdownPacketMode(pcfgParams->cardNum, pcfgParams->DMAEngineOffset);
	for (i = 0; i < NUM_STRESS_PARAMS; i++)
	{
		if (params[i].Buffer)
		{
			// Free the buffer that was allocated for each thread
			g_PM40Utils.FreeMem(&params[i]);
		}
	}
	if (pcfgParams->bVerbose == FALSE)
	{
		// print debug messages
		printf("Packet Loopback Stress Complete\n");
	}
	return 0;
}

//---------------------------------------------------------------------------------------
//			Addressable Packet Mode Stress Testing Routines
//---------------------------------------------------------------------------------------

/*! Packet Write Stress Thread
*
* \brief
* \param params
* \return
*/
VOID PM40PacketReadWriteThreadProc(PTHREAD_PARAMS params)
{
	UINT64		CardOffset;
	UINT32		TransferSize;
	UINT32		Mode = 0;
	UINT32		status;
	UINT64		i;

	CardOffset = params->CardOffset;
	if (params->bFIFO)
	{
		Mode = READ_WRITE_MODE_FLAG_FIFO;
	}

	for (i = 0; i < params->Iterations; i++)
	{
		TransferSize = params->MaxPacketSize;
		// Check for random size and start offset option
		if (params->bRandomize)
		{
			// If randomize and Addressable FIFO mode
			if (params->bFIFO)
			{
				// Only randomize the transfer length
				TransferSize = rand() % params->MaxPacketSize + 1;
			}
			else
			{
				CardOffset = 0;
				if (params->CardOffset)
				{
					CardOffset = rand() % params->CardOffset;
				}
				TransferSize = rand() % params->MaxPacketSize + 1;
				if ((CardOffset + TransferSize) > DEFAULT_MAX_CARD_OFFSET)
				{
					TransferSize = (UINT32)(DEFAULT_MAX_CARD_OFFSET - CardOffset);
				}
			}
		}
		if (params->Direction == DMA_CAP_SYSTEM_TO_CARD)
		{
			if (params->DataPatternType == PATTERN_RANDOM)
			{
				g_PM40Utils.WriteRandomBuffer(params, TransferSize);
			}
			// Put the Packet number in the Buffer data and the User Control
			if (!params->bUserInfoPatternSpecified)
			{
				params->UserInfo = i;
			}

			if (params->bDebug)
			{
				if (params->pcfgParams->bSecondRefresh)
				{
					params->pcfgParams->bSecondRefresh = FALSE;
					g_PM40Utils.DisplayFormattedBytes(
						params->pcfgParams,
						(UINT64)params->Buffer,
						(PUINT8)params->Buffer,
						(INT32)min(256, TransferSize));
				}
			}
			status = PacketWriteEx(
				params->cardNum,
				params->DMAEngineOffset,
				params->UserInfo,
				CardOffset,
				Mode,
				(PUINT8)params->Buffer,
				TransferSize);
			// Check the status
			if (status != 0)
			{
				g_PM40Utils.DisplayErrorInfo(NULL, params, status);
				// Break out of the loop and cleanup
				break;
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
			// Check the status
			if (status != 0)
			{
				g_PM40Utils.DisplayErrorInfo(NULL, params, status);
				// Break out of the loop and cleanup
				break;
			}
			params->PacketCount++;
			__int16* pArray = (__int16*)params->Buffer;

			for (int ii = 0; ii < 256; ii++) {
				theApp.m_Scope.m_RFTrace[0][ii] = pArray[ii]>>8;
			}

			if (params->bDebug)
			{
				if (params->pcfgParams->bSecondRefresh)
				{
					params->pcfgParams->bSecondRefresh = FALSE;
					g_PM40Utils.DisplayFormattedBytes(
						params->pcfgParams,
						(UINT64)params->Buffer,
						(PUINT8)params->Buffer,
						(INT32)min(256, TransferSize));
				}
			}
		}

		// Request ownership of the critical section.
		BEGIN_CRITICAL(&params->CriticalSection);
		params->TotalBytesTransferred += TransferSize;
		// Release ownership of the critical section.
		END_CRITICAL(&params->CriticalSection);

		// See if we are still supposed to run, another thread may want to shut us down.
		if (!params->Running)
		{
			break;
		}
	}
	params->Running = FALSE;
	Sleep(10);
}

/* Packet Write Read Loop Stress Thread
*
* \brief
* \param pparams
* \return
*/
VOID PM40PacketWRLoopbackThreadProc(PTHREAD_PARAMS	pparams)
{
	PTHREAD_PARAMS	ReadParams;
	PTHREAD_PARAMS	WriteParams;
	UINT64			i;
	UINT32			t;
	UINT64			UserControl;
	UINT64			UserStatus;
	UINT64			CardOffset;
	UINT32			Mode = 0;
	UINT32			ReadTransferSize;
	UINT32			WriteTransferSize;
	UINT32			status;

	ReadParams = pparams;
	pparams++;
	WriteParams = pparams;

	WriteTransferSize = WriteParams->MaxPacketSize;
	CardOffset = WriteParams->CardOffset;
	if (WriteParams->bFIFO)
	{
		Mode = READ_WRITE_MODE_FLAG_FIFO;
	}

	for (i = 0; i < WriteParams->Iterations; i++)
	{
		WriteTransferSize = WriteParams->MaxPacketSize;
		// Check for random size and start offset option
		if (WriteParams->bRandomize)
		{
			// If randomize and FIFO Addressable mode do not change the CardOffset
			if (WriteParams->bFIFO)
			{
				Mode = READ_WRITE_MODE_FLAG_ADDRESSED;
				if (rand() % 2)
				{
					Mode = READ_WRITE_MODE_FLAG_FIFO;
				}
				WriteTransferSize = rand() % WriteParams->MaxPacketSize + 1;
			}
			else
			{
				if (WriteParams->CardOffset)
				{
					CardOffset = rand() % WriteParams->CardOffset;
				}
				WriteTransferSize = rand() % WriteParams->MaxPacketSize + 1;
				if ((CardOffset + WriteTransferSize) > WriteParams->MaxDMAAddress)
				{
					WriteTransferSize = (UINT32)(WriteParams->MaxDMAAddress - CardOffset);
				}
			}
		}
		if (WriteParams->DataPatternType == PATTERN_RANDOM)
		{
			g_PM40Utils.WriteRandomBuffer(WriteParams, WriteTransferSize);
		}

		// Put the Packet number in the User Control
		UserControl = i;
		status = PacketWriteEx(
			WriteParams->cardNum,
			WriteParams->DMAEngineOffset,
			UserControl,
			CardOffset,
			Mode,
			(PUINT8)WriteParams->Buffer,
			WriteTransferSize);
		// Check the status
		if (status != 0)
		{
			g_PM40Utils.DisplayErrorInfo(NULL, WriteParams, status);
			// Break out of the loop and cleanup
			break;
		}
		WriteParams->PacketCount++;

		// Request ownership of the critical section.
		BEGIN_CRITICAL(&WriteParams->CriticalSection);
		WriteParams->TotalBytesTransferred += WriteTransferSize;
		// Release ownership of the critical section.
		END_CRITICAL(&WriteParams->CriticalSection);

		ReadTransferSize = WriteTransferSize;
		status = PacketReadEx(
			ReadParams->cardNum,
			ReadParams->DMAEngineOffset,
			&UserStatus,
			CardOffset,
			Mode,
			(PUINT8)ReadParams->Buffer,
			&ReadTransferSize);
		// Check the status
		if (status != 0)
		{
			g_PM40Utils.DisplayErrorInfo(NULL, ReadParams, status);
			// Break out of the loop and cleanup
			break;
		}
		else
		{
			// Make sure the length is valid
			if (ReadTransferSize != 0)
			{
				if (WriteTransferSize != ReadTransferSize)
				{
					printf("Receive Size mismatch, Write Value: 0x%x, Read Value: 0x%x\n", WriteTransferSize, ReadTransferSize);
				}
				for (t = 0; t < WriteTransferSize; t++)
				{
					if (ReadParams->Buffer[t] != WriteParams->Buffer[t])
					{
						if (!ReadParams->bQuiet)
						{
							printf("Data mistatch at offset 0x%x, Write Value: 0x%x, Read Value: 0x%x\n",
								t, WriteParams->Buffer[t], ReadParams->Buffer[t]);
						}
						ReadParams->iPass = ERROR_TRANSFER_DATA_MISMATCH;
						if (ReadParams->bStopOnError)
						{
							// stop here
							ReadParams->Running = FALSE;
							break;
						}
					}
				}
			}
			else
			{
				if (!ReadParams->bQuiet)
				{
					printf("Invalid Packet recieved, Length=0x%x\n", ReadTransferSize);
				}
				ReadParams->iPass = ERROR_TRANSFER_COUNT_MISMATCH;
			}
			ReadParams->PacketCount++;

			if (ReadParams->bDebug)
			{
				if (ReadParams->pcfgParams->bSecondRefresh)
				{
					ReadParams->pcfgParams->bSecondRefresh = FALSE;
					g_PM40Utils.DisplayFormattedBytes(
						ReadParams->pcfgParams,
						(UINT64)ReadParams->Buffer,
						(PUINT8)ReadParams->Buffer,
						(INT32)min(256, ReadTransferSize));
				}
			}

			// Request ownership of the critical section.
			BEGIN_CRITICAL(&ReadParams->CriticalSection);
			ReadParams->TotalBytesTransferred += ReadTransferSize;
			// Release ownership of the critical section.
			END_CRITICAL(&ReadParams->CriticalSection);
		}
		// See if we are still supposed to run, 
		// another thread may want to shut us down.
		if (!WriteParams->Running || !ReadParams->Running)
		{
			break;
		}
	}
	WriteParams->Running = FALSE;
	ReadParams->Running = FALSE;
	Sleep(10);
}

/* StressPacketWrite
*
* \brief Function to handle the "PacketWrite" command.
*  Calls the DmaDriver DLL to do the work.
* \param pcfgParams
* \return
*/
INT32 CPM40Packets::stressPacketWrite(PCFG_PARAMS	pcfgParams)
{
	THREAD_PARAMS		params[MAX_THREADS];
	UINT32				NumberPackets;
	UINT32				i;
	BOOLEAN				Running;

	pcfgParams->TestType = PACKET_WRITE;

	NumberPackets = pcfgParams->Iterations;
	pcfgParams->BufferSize = DEFAULT_MAX_PACKET_SIZE;
	pcfgParams->MaxPacketSize = DEFAULT_MAX_PACKET_SIZE;
	pcfgParams->CardAddress = 0;
	pcfgParams->UserInfo = 0;

	// get the parameters from the command line, if any
	if (pcfgParams->CmdArgc > 1)
	{
		pcfgParams->CardAddress = (UINT64)g_PM40Utils.__strtoull(pcfgParams->CmdArgv[1]);
	}
	else
	{
		if (pcfgParams->bRandomize)
		{
			pcfgParams->CardAddress = (UINT64)DEFAULT_MAX_CARD_OFFSET;
		}
	}
	if (pcfgParams->CmdArgc > 2)
	{
		pcfgParams->BufferSize = (UINT64)g_PM40Utils.__strtoull(pcfgParams->CmdArgv[2]);
		if (pcfgParams->BufferSize <= pcfgParams->MaxDMAAddress)
		{
			pcfgParams->MaxPacketSize = (UINT32)pcfgParams->BufferSize;
		}
		else
		{
			printf("Max Packet Size (%lld) is too large.\n", pcfgParams->BufferSize);
			return -2;
		}
	}
	if (pcfgParams->MaxPacketSize > pcfgParams->MaxDMAAddress)
	{
		if (pcfgParams->bVerbose == TRUE)
		{
			printf("Max Packet Size is larger than the DMA Memory, setting to DMA Memory size.\n");
		}
		pcfgParams->MaxPacketSize = pcfgParams->MaxDMAAddress;
		pcfgParams->BufferSize = pcfgParams->MaxDMAAddress;
	}
	if (pcfgParams->CmdArgc > 3)
	{
		pcfgParams->UserInfo = (UINT64)g_PM40Utils.__strtoull(pcfgParams->CmdArgv[3]);
		pcfgParams->bUserInfoPatternSpecified = FALSE;
	}

	// Make sure there we do at least one transfer for each thread
	if (NumberPackets < (UINT32)pcfgParams->NumberThreads)
	{
		NumberPackets = pcfgParams->NumberThreads;
	}

	// print debug messages
	if (pcfgParams->bVerbose == TRUE)
	{
		printf("Number Packets = %d\n", (INT32)NumberPackets);
		printf("MaxPacketSize = %d\n", (INT32)pcfgParams->MaxPacketSize);
		printf("CardOffset = 0x%llx\n", pcfgParams->CardAddress);
		printf("UserControl = 0x%llx\n", pcfgParams->UserInfo);
		printf("Threads = %d\n", pcfgParams->NumberThreads);
	}

	// Setup the packet checker in Loopback RAM Mode
	if (g_PM40Utils.ConfigPacketGenChk(pcfgParams, PACKET_GEN_LOOPBACK_USE_RAM, S2C_DIRECTION, NumberPackets) != STATUS_SUCCESSFUL)
	{
		printf("Setup Packet Checker failed, Max Packet Size (0x%x) is too large\n", pcfgParams->MaxPacketSize);
		return -2;
	}

	// Make sure we have enough packets for all the threads
	NumberPackets = NumberPackets / pcfgParams->NumberThreads;
	for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
	{
		g_PM40Utils.InitThreadParams(pcfgParams, &params[i]);
		params[i].Iterations = NumberPackets;
		params[i].Direction = DMA_CAP_SYSTEM_TO_CARD;
	}
	for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
	{
		if (g_PM40Utils.BufferAllocAndSet(pcfgParams, &params[i]))
		{
			goto ShutdownStressPacketWrite;
		}
	}
	// Now we have the buffers we need, start the test threads
	for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
	{
		g_PM40Utils.StartThread(&params[i], (LPTHREAD_START_ROUTINE)PM40PacketReadWriteThreadProc);
	}

	// Get the stats and discard due to start up may be bogus
	g_PM40Utils.ClearAllStats(pcfgParams, &params[0], pcfgParams->NumberThreads, CLI_SYSTEM_TO_CARD);
	g_PM40Utils.InitSummaryStats(pcfgParams);

	// Loop forever until all threads are finished
	Running = TRUE;
	while (Running)
	{
		if (g_PM40Utils.CollectAllStats(pcfgParams, &params[0], pcfgParams->NumberThreads, CLI_SYSTEM_TO_CARD) == 0)
		{
			Running = g_PM40Utils.TestRunControl(pcfgParams, &params[0]);
		}
		else
		{
			Running = FALSE;
		}
	}

	// Display the summaries from this test run
	g_PM40Utils.DisplaySummaryInfo(pcfgParams);

ShutdownStressPacketWrite:
	// Shutdown the Packet Checker
	g_PM40Utils.ConfigPacketGenChk(pcfgParams, 0, S2C_DIRECTION, 0);

	for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
	{
		// Free the buffer that was allocated for each thread
		g_PM40Utils.FreeMem(&params[i]);
	}

	if (pcfgParams->bVerbose == TRUE)
	{
		printf("\nStress Packet Write completed.\n");
	}
	return 0;
}

/*! StressPacketRead
*
* \brief Function to handle the "packetRead" command.
*  Calls the DmaDriver DLL to do the work.
* \params pcfgParams
* \return
*/
INT32 CPM40Packets::stressPacketRead(PCFG_PARAMS	pcfgParams)
{
	THREAD_PARAMS		params[MAX_THREADS];
	UINT32				NumberPackets;
	UINT32				i;
	UINT32				status;
	BOOLEAN				Running;
	CString Buff;

	pcfgParams->TestType = PACKET_READ;

	// get a parameters from the command line
	NumberPackets = pcfgParams->Iterations;

	// We are here because the Application needs to provide the Receive Bufffer
	pcfgParams->MaxPacketSize = DEFAULT_MAX_PACKET_SIZE;
	pcfgParams->BufferSize = DEFAULT_MAX_PACKET_SIZE;
	pcfgParams->CardAddress = 0;

	pcfgParams->BufferSize = DEFAULT_MAX_PACKET_SIZE;
	pcfgParams->MaxPacketSize = (UINT32)pcfgParams->BufferSize;

	if (pcfgParams->MaxPacketSize > pcfgParams->MaxDMAAddress)
	{
		if (pcfgParams->bVerbose == TRUE)
		{
			Buff.Format(L"Max Packet Size is larger than the DMA Memory, setting to DMA Memory size.");	
			//theApp.m_DMAMessages.Add(Buff);
		}
		pcfgParams->MaxPacketSize = pcfgParams->MaxDMAAddress;
		pcfgParams->BufferSize = pcfgParams->MaxDMAAddress;
	}

	// Make sure there we do at least one transfer for each thread
	if (NumberPackets < (UINT32)pcfgParams->NumberThreads)
	{
		NumberPackets = pcfgParams->NumberThreads;
	}

	// print debug messages
	if (pcfgParams->bVerbose == FALSE)
	{
		
		Buff.Format(L"Number Packets = %d", (INT32)NumberPackets);				//theApp.m_DMAMessages.Add(Buff);
		Buff.Format(L"MaxPacketSize = %d", (INT32)pcfgParams->MaxPacketSize);	//theApp.m_DMAMessages.Add(Buff);
		Buff.Format(L"Buffer Size = %lld", pcfgParams->BufferSize);				//theApp.m_DMAMessages.Add(Buff);
		Buff.Format(L"Threads = %d", pcfgParams->NumberThreads);				//theApp.m_DMAMessages.Add(Buff);
	}

	// Setup the Packet Generator in this case will provide RAM to loopback packets.
	if (g_PM40Utils.ConfigPacketGenChk(pcfgParams, PACKET_GEN_LOOPBACK_USE_RAM, C2S_DIRECTION, NumberPackets) != STATUS_SUCCESSFUL)
	{
		Buff.Format(L"Setup Packet Generator failed, Max Packet Size (0x%x) is too large.", pcfgParams->MaxPacketSize);	//theApp.m_DMAMessages.Add(Buff);
		return -2;
	}

	status = SetupPacketMode(
		pcfgParams->cardNum,
		pcfgParams->DMAEngineOffset,
		(PUINT8)NULL,
		(PUINT32)NULL,
		NULL,
		PACKET_MODE_ADDRESSABLE,
		8192);
	if (status != STATUS_SUCCESSFUL)
	{
		g_PM40Utils.DisplayErrorInfo(pcfgParams, NULL, status);
		return -1;
	}

	// Make sure we have enough packets for all the threads
	NumberPackets = NumberPackets / pcfgParams->NumberThreads;
	for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
	{
		g_PM40Utils.InitThreadParams(pcfgParams, &params[i]);
		params[i].Iterations = NumberPackets;
		params[i].Direction = DMA_CAP_CARD_TO_SYSTEM;
	}
	for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
	{
		if (g_PM40Utils.BufferAllocAndSet(pcfgParams, &params[i]))
		{
			goto ShutdownStressPacketRead;
		}
	}
	g_PM40Utils.ClearAllStats(pcfgParams, &params[0], pcfgParams->NumberThreads, CLI_CARD_TO_SYSTEM);
	g_PM40Utils.InitSummaryStats(pcfgParams);

	// Now we have the buffers we need, start the test threads
	for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
	{
		g_PM40Utils.StartThread(&params[i], (LPTHREAD_START_ROUTINE)PM40PacketReadWriteThreadProc);
	}


	// Loop forever until all threads are finished
	Running = TRUE;
	while (Running)
	{
		if (g_PM40Utils.CollectAllStats(pcfgParams, &params[0], pcfgParams->NumberThreads, CLI_CARD_TO_SYSTEM) == 0)
		{
			Running = g_PM40Utils.TestRunControl(pcfgParams, &params[0]);
		}
		else
		{
			Running = FALSE;
		}
	}
	// Display the summaries frm this test run
	g_PM40Utils.DisplaySummaryInfo(pcfgParams);

ShutdownStressPacketRead:
	// Shutdown the Packet Generator
	g_PM40Utils.ConfigPacketGenChk(pcfgParams, 0, C2S_DIRECTION, 0);

	// Do an orderly shutdwon by releasing the packet buffers from the driver
	ShutdownPacketMode(pcfgParams->cardNum, pcfgParams->DMAEngineOffset);
	// Then free the allocated memory
	for (i = 0; i < (UINT32)pcfgParams->NumberThreads; i++)
	{
		// Free the buffer that was allocated for each thread
		g_PM40Utils.FreeMem(&params[i]);
	}

	if (pcfgParams->bVerbose == TRUE)
	{
		// print debug messages
		Buff.Format(L"Stress Packet Read Complete.");	//theApp.m_DMAMessages.Add(Buff);
	}
//	theApp.m_bMessageRender = true;
	return 0;
}

/* PacketWR
*
* \brief Function to handle the "PacketWR" command.
*  Calls the DmaDriver DLL to do the work.
* \param pcfgParams
* \return
*/
INT32 CPM40Packets::stressPacketWriteRead(PCFG_PARAMS pcfgParams)
{
	THREAD_PARAMS	params[NUM_STRESS_PARAMS];
	UINT32			status;
	UINT32			NumberPackets;
	UINT32			i;
	BOOLEAN			Running;

	pcfgParams->TestType = PACKET_WR;

	// Hard set the loopback to a single thread. It is currently not 
	// designed to handle more than one. due the recieve handler in the 
	// Loop thread routine.
	pcfgParams->NumberThreads = 1;

	// get a parameters from the command line
	pcfgParams->MaxPacketSize = DEFAULT_DMA_LENGTH;
	pcfgParams->BufferSize = DEFAULT_BUFFER_SIZE;
	pcfgParams->CardAddress = 0;

	if (pcfgParams->CmdArgc > 1)
	{
		pcfgParams->CardAddress = (UINT64)g_PM40Utils.__strtoull(pcfgParams->CmdArgv[1]);
	}
	else
	{
		if (pcfgParams->bRandomize)
		{
			pcfgParams->CardAddress = (UINT64)DEFAULT_ADDR_PACKET_DMA_MAX;
		}
	}
	if (pcfgParams->CmdArgc > 2)
	{
		pcfgParams->BufferSize = (UINT64)g_PM40Utils.__strtoull(pcfgParams->CmdArgv[2]);
		if (pcfgParams->BufferSize > 0x00000000ffffffff)
		{
			printf("Max Packet Size (%lld) is too large.\n", pcfgParams->BufferSize);
			return -2;
		}
		pcfgParams->MaxPacketSize = (UINT32)pcfgParams->BufferSize;
	}
	if (pcfgParams->MaxPacketSize > pcfgParams->MaxDMAAddress)
	{
		if (pcfgParams->bVerbose == TRUE)
		{
			printf("Max Packet Size is larger than the DMA Memory, setting to DMA Memory size.\n");
		}
		pcfgParams->MaxPacketSize = pcfgParams->MaxDMAAddress;
		pcfgParams->BufferSize = pcfgParams->MaxDMAAddress;
	}
	if (!pcfgParams->bFIFO)
	{
		if (pcfgParams->CardAddress > pcfgParams->MaxDMAAddress)
		{
			if (pcfgParams->bVerbose == TRUE)
			{
				printf("Card Offset is larger than the DMA Memory, setting Card Offset to 0.\n");
			}
			pcfgParams->CardAddress = 0;
		}
	}

	g_PM40Utils.InitThreadParams(pcfgParams, &params[READ_STRESS]);
	// Allocate and setup the Receive Buffer
	if (g_PM40Utils.BufferAllocAndSet(pcfgParams, &params[READ_STRESS]))
	{
		return -1;
	}

	g_PM40Utils.InitThreadParams(pcfgParams, &params[WRITE_STRESS]);
	// Make the send packet size the Max Packet Size for allocation.
	pcfgParams->BufferSize = pcfgParams->MaxPacketSize;
	// Allocate and setup the Send Buffer
	if (g_PM40Utils.BufferAllocAndSet(pcfgParams, &params[WRITE_STRESS]))
	{
		g_PM40Utils.FreeMem(&params[READ_STRESS]);
		return -1;
	}

	// Make sure there we do at least one transfer for each thread
	NumberPackets = pcfgParams->Iterations;
	if (NumberPackets < (UINT32)pcfgParams->NumberThreads)
	{
		NumberPackets = pcfgParams->NumberThreads;
	}

	// Turn on Loopback on both the S2C and C2S DMA Engines
	if (g_PM40Utils.ConfigPacketGenChk(pcfgParams, PACKET_GEN_LOOPBACK_USE_RAM, S2C_DIRECTION, NumberPackets) != STATUS_SUCCESSFUL)
	{
		g_PM40Utils.FreeMem(&params[READ_STRESS]);
		g_PM40Utils.FreeMem(&params[WRITE_STRESS]);
		printf("Setup Packet Checker failed, Max Packet Size (0x%x) is too large.\n", pcfgParams->MaxPacketSize);
		return -2;
	}

	if (g_PM40Utils.ConfigPacketGenChk(pcfgParams, PACKET_GEN_LOOPBACK_USE_RAM, C2S_DIRECTION, NumberPackets) != STATUS_SUCCESSFUL)
	{
		g_PM40Utils.FreeMem(&params[READ_STRESS]);
		g_PM40Utils.FreeMem(&params[WRITE_STRESS]);
		printf("Setup Packet Generator failed, Max Packet Size (0x%x) is too large.\n", pcfgParams->MaxPacketSize);
		return -2;
	}

	status = SetupPacketMode(
		pcfgParams->cardNum,
		pcfgParams->DMAEngineOffset,
		(PUINT8)NULL,
		(PUINT32)NULL,
		NULL,
		PACKET_MODE_ADDRESSABLE,
		8192);
	if (status != STATUS_SUCCESSFUL)
	{
		g_PM40Utils.FreeMem(&params[READ_STRESS]);
		g_PM40Utils.FreeMem(&params[WRITE_STRESS]);
		g_PM40Utils.DisplayErrorInfo(pcfgParams, NULL, status);
		return -1;
	}

	// Startup the Send thread(s)
	if (g_PM40Utils.StartThread(&params[0], (LPTHREAD_START_ROUTINE)PM40PacketWRLoopbackThreadProc) != 0)
	{
		ShutdownPacketMode(
			pcfgParams->cardNum,
			pcfgParams->DMAEngineOffset);
		g_PM40Utils.FreeMem(&params[READ_STRESS]);
		g_PM40Utils.FreeMem(&params[WRITE_STRESS]);
		// print debug messages
		printf("Test failed to start thread, exiting.\n");
		return -2;
	}

	// print debug messages
	if (pcfgParams->bVerbose == TRUE)
	{
		printf("Number Packets = %d.\n", (INT32)NumberPackets);
		printf("MaxPacketSize = %d\n", (INT32)pcfgParams->MaxPacketSize);
		printf("Send Buffer Allocated Size per thread = 0x%x.\n", pcfgParams->MaxPacketSize);
	}

#ifdef WIN32
	g_PM40Utils.ClearAllStats(pcfgParams, &params[0], NUM_STRESS_PARAMS,
	enum CLI_DMA_DIRECTION(CLI_CARD_TO_SYSTEM | CLI_SYSTEM_TO_CARD));
#else
	ClearAllStats(pcfgParams, &params[0], NUM_STRESS_PARAMS,
		CLI_DMA_DIRECTION(CLI_CARD_TO_SYSTEM | CLI_SYSTEM_TO_CARD));
#endif // Windows v. Linux
	g_PM40Utils.InitSummaryStats(pcfgParams);

	// Loop forever until all threads are finished
	Running = TRUE;
	while (Running)
	{
#ifdef WIN32
		if (g_PM40Utils.CollectAllStats(pcfgParams, &params[0], NUM_STRESS_PARAMS,
		enum CLI_DMA_DIRECTION(CLI_SYSTEM_TO_CARD | CLI_CARD_TO_SYSTEM)) == 0)
#else
		if (CollectAllStats(pcfgParams, &params[0], NUM_STRESS_PARAMS,
			CLI_DMA_DIRECTION(CLI_SYSTEM_TO_CARD | CLI_CARD_TO_SYSTEM)) == 0)
#endif // Windows v. Linux
		{
			Running = g_PM40Utils.TestRunControl(pcfgParams, &params[0]);
		}
		else
		{
			Running = FALSE;
		}
	}

	// Display the summaries from this test run
	g_PM40Utils.DisplaySummaryInfo(pcfgParams);

	NumberPackets = 0;
	// Turn off Loopback on both the S2C and C2S DMA Engines
	g_PM40Utils.ConfigPacketGenChk(pcfgParams, 0, S2C_DIRECTION, 0);
	g_PM40Utils.ConfigPacketGenChk(pcfgParams, 0, C2S_DIRECTION, 0);

	// Do an orderly shutdwon by releasing the packet buffers from the driver
	ShutdownPacketMode(pcfgParams->cardNum, pcfgParams->DMAEngineOffset);
	for (i = 0; i < NUM_STRESS_PARAMS; i++)
	{
		if (params[i].Buffer)
		{
			// Free the buffer that was allocated for each thread
			g_PM40Utils.FreeMem(&params[i]);
		}
	}

	if (pcfgParams->bVerbose == TRUE)
	{
		// print debug messages
		printf("Packet Write/Read Stress Complete.\n");
	}
	return 0;
}

/* Single Packet Read or Write
*
* \brief
* \param pcfgParams
* \param DMAEngineOffset
* \param Direction
* \return
*/
VOID CPM40Packets::SinglePacketReadWrite(PCFG_PARAMS pcfgParams,UCHAR DMAEngineOffset,BOOLEAN	Direction)
{
	UINT32			TransferSize;
	UINT64			UserInfo = 0;
	PUINT8 			Buffer;
	UINT32			status;

	TransferSize = (UINT32)pcfgParams->BufferSize;

	// Allocate the buffer
#ifdef WIN32
	Buffer = (PUINT8)VirtualAlloc(NULL, TransferSize, MEM_COMMIT, PAGE_READWRITE | PAGE_NOCACHE);
	if (Buffer == NULL)
#else // Linux
	if (posix_memalign((void **)&Buffer, 4096, TransferSize))
#endif // Windows vs. Linux
	{
		printf("SinglePacketReadWrite buffer allocation failed.");
		return;
	}

	if (Direction == S2C_DIRECTION)
	{
		// Debug: printf("Performing Packet Send, Size = 0x%d\n", TransferSize);
		// Put the Packet number in the Buffer data and the User Control
		status = PacketSendEx(
			pcfgParams->cardNum,
			DMAEngineOffset,
			UserInfo,
			0,
			(PUINT8)Buffer,
			TransferSize);
	}
	else
	{
		// Debug: printf("Performing Packet Read, Size = 0x%d\n", TransferSize);
		status = PacketReadEx(
			pcfgParams->cardNum,
			DMAEngineOffset,
			&UserInfo,
			0,
			0,
			(PUINT8)Buffer,
			&TransferSize);
	}	// end if (Direction ==S2C_DIRECTION)

	if (Buffer != NULL)
	{
#ifdef WIN32	
		if (!VirtualFree(Buffer, 0, MEM_RELEASE))
		{
			printf("Error %d: while freeing buffer.\n", GetLastError());
		}
#else	// Linux
		free(Buffer);
#endif	// end WIN32 Conditional Compile
	}
}

/*! Single Packet Recieve Stream
*
* \brief
* \param pcfgParams
* \param DMAEngineOffset
* \return None
*/
VOID CPM40Packets::SinglePacketRecvs(PCFG_PARAMS pcfgParams,UINT8 DMAEngineOffset)
{
	PPACKET_RECVS_STRUCT	pPacketRecvs = NULL;	// Pointer to Packet Receives struct
	UINT32					status;

	pPacketRecvs = (PPACKET_RECVS_STRUCT)malloc(sizeof(PACKET_RECVS_STRUCT) +
		(NUMBER_PACKET_RECVS * sizeof(PACKET_ENTRY_STRUCT)));

	if (pPacketRecvs != NULL)
	{
		// Do one PacketReceives to clean out any existing overrun errors
		// while we where setting up this thread. Discard the results.
		pPacketRecvs->AvailNumEntries = NUMBER_PACKET_RECVS;
		pPacketRecvs->RetNumEntries = 0;

		status = PacketReceives(
			pcfgParams->cardNum,
			DMAEngineOffset,
			pPacketRecvs);
		if (status != 0)
		{
			printf("PacketReceives failed, returned %u.\n", status);
		}
		else
		{
			if (pPacketRecvs->RetNumEntries == 0)
			{
				//printf("SinglePacketRecvs returned 0 packets\n");
			}
		}
		free(pPacketRecvs);
	}
}
