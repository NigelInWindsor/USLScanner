#pragma once



class CPM40Packets
{
public:
	CPM40Packets();
	~CPM40Packets();

	INT32 stressPacketSend(PCFG_PARAMS pcfgParams);
	INT32 stressPacketReceive(PCFG_PARAMS	pcfgParams);
	INT32 stressPacketReceives(PCFG_PARAMS	pcfgParams);
	INT32 stressPacketLoop(PCFG_PARAMS		pcfgParams);
	INT32 stressPacketWrite(PCFG_PARAMS	pcfgParams);
	INT32 stressPacketRead(PCFG_PARAMS	pcfgParams);
	INT32 stressPacketWriteRead(PCFG_PARAMS pcfgParams);
	VOID SinglePacketReadWrite(PCFG_PARAMS pcfgParams, UCHAR DMAEngineOffset, BOOLEAN	Direction);
	VOID SinglePacketRecvs(PCFG_PARAMS pcfgParams, UINT8 DMAEngineOffset);


};



extern VOID PacketWRLoopbackThreadProc(PTHREAD_PARAMS	pparams);
//extern VOID PM40PacketReadWriteThreadProc(PTHREAD_PARAMS params)

