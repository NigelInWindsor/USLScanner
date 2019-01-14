#pragma once

class CPM40Utils
{
public:
	CPM40Utils();
	~CPM40Utils();

public:
	VOID InitThreadParams(PCFG_PARAMS pcfgParams, PTHREAD_PARAMS params);
	INT32 StartThread(PTHREAD_PARAMS          params, LPTHREAD_START_ROUTINE  ThreadProc);
	BOOLEAN TestRunControl(PCFG_PARAMS     pcfgParams, PTHREAD_PARAMS  params);
	VOID ShutdownTestThreads(PCFG_PARAMS     pcfgParams, PTHREAD_PARAMS  params);
	VOID ClearAllStats(PCFG_PARAMS pcfgParams, PTHREAD_PARAMS params, INT32 numberThreads, enum CLI_DMA_DIRECTION  direction);
	UINT32 CollectAllStats(PCFG_PARAMS pcfgParams, PTHREAD_PARAMS params, INT32 numberThreads, enum CLI_DMA_DIRECTION  direction);
	UINT32 DisplayPassInfo(PCFG_PARAMS     pcfgParams, PPASS_STATS     pPassStats);
	VOID InitSummaryStats(PCFG_PARAMS     pcfgParams);
	VOID DisplaySummaryInfo(PCFG_PARAMS pcfgParams);
	VOID DisplayFormattedBytes(PCFG_PARAMS pcfgParams, UINT64  Offset, PUINT8 pBuffer, INT32 BytesToDisplay);
	VOID DisplayErrorInfo(PCFG_PARAMS pcfgParams, PTHREAD_PARAMS params, UINT32 status);
	UINT32 __strtoul(const char *    nptr);
	UINT64 __strtoull(const char * nptr);
	UINT64 strtoull_l(const char * __restrict nptr, char **      __restrict endptr, INT32 base);
	UINT32 _hexttoul(const char *    value);
	void reverse_string(unsigned char *	str, UINT64			strLength);
	INT32 atohx(PCHAR           dst, const char *    src);
	INT32 GetCPUUtil();
	INT32 SetupOSSpecifics(PCFG_PARAMS pcfgParams);
	INT32 ShutdownOSSpecifics(PCFG_PARAMS pcfgParams);
	INT32 ConfigPacketGenChk(PCFG_PARAMS pcfgParams, INT32 Mode, BOOLEAN Direction, UINT32 NumberPackets);
	INT32 SetupPacketGenerator(PCFG_PARAMS pcfgParams, UINT8 DMAEngineOffset, BOOLEAN Direction, PPACKET_GENCHK_STRUCT pPacketGenChk);
	INT32 BufferAllocAndSet(PCFG_PARAMS pcfgParams, PTHREAD_PARAMS  params);
	BOOLEAN ValidateDataBuffer(PTHREAD_PARAMS  params, PUINT8 Buffer, UINT64 Length);
	VOID WriteRandomBuffer(PTHREAD_PARAMS  params, UINT32 Length);
	BOOLEAN ValidateUserStatus(PTHREAD_PARAMS params, UINT64 UserStatus);
	UINT32 GetNextPattern(INT32 Type, INT32 Size, UINT32 Seed);
	VOID FreeMem(PTHREAD_PARAMS  params);

};

