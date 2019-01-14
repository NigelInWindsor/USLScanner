
#pragma once

#if !defined(_COM_DECL_)
#ifdef _UTSOCK_EXPORT_
	#define _COM_DECL_ __declspec(dllexport)
#else _UTSOCK_EXPORT_
	#define _COM_DECL_ __declspec(dllimport)
#endif _UTSOCK_EXPORT_
#endif _COM_DECL_

_COM_DECL_ bool UTCom_Ping(char *ip, int timeout);

#pragma pack(1)
class _COM_DECL_ CStream_0x0001
{
public:
	CStream_0x0001();


	unsigned start:32;
	unsigned size:32;
	unsigned frameId:32;//frame number.
	unsigned settingId:32;
	unsigned subStreamCount:16;
	unsigned version:16;
};
class _COM_DECL_ CSubStreamIO_0x0101
{
public:
	CSubStreamIO_0x0101();

	bool Check() const;

	unsigned id:8;
	unsigned version:8;
	unsigned size:16;
	unsigned timeStampLow:32;
	unsigned timeStampHigh:32;
	unsigned cycle:16;
	unsigned maxTemperature:7;//maximum temperature (unit=). Not supported by all FW.
	unsigned padding0:9;
	unsigned seqLow:32;
	unsigned seqHigh:32;
	unsigned inputs:32;//digital inputs value.
	unsigned edges:32;//edges : which digital input has been updated ?
						//if bit 16 is set to 1 then this is initialisation stream (sent on any updates from "SetRequestIO").
	unsigned encoder1:32;
	unsigned encoder2:32;
};



typedef struct structCscanAmp_0x0102{
		unsigned byAmp:8;//cscan amplitude. Could be signed value, it depends of the GateModeThreshold setting data.
		unsigned gateId:2;//gate identifier (could be: 0=first gate, 1=second gate, 2=third gate, 3=fourth gate).
		unsigned sign:1;//0 byAmp is unsigned, 1 byAmp is signed.
		unsigned AmpOverThreshold:1;//0 if byAmp is under threshold, and 1 if it is over the threshold.
		unsigned TofValidity:1;//0 (no time of flight).
		unsigned IFOldReference:1;//(IFtracking) 1 if old reference time of flight has been used to synchronize the start or the stop.
		unsigned IFNotInitialized:1;//(IFtracking) 1 if the first reference time of flight has been initialized yet (because of acoustic or because of bad setting stop>start).
		unsigned reserved:1;
		unsigned wAcqId:16;//acquisition identifier (could be used to link the unique gate/cycle cscan data with the setting). But it is not required to use it.
}structCscanAmp_0x0102;
typedef struct structCscanAmpTof_0x0202{
		unsigned byAmp:8;//cscan amplitude. Could be signed value, it depends of the GateModeThreshold setting data.
		unsigned gateId:2;//gate identifier (could be: 0=first gate, 1=second gate, 2=third gate, 3=fourth gate).
		unsigned sign:1;//0 byAmp is unsigned, 1 byAmp is signed.
		unsigned AmpOverThreshold:1;//0 if byAmp is under threshold, and 1 if it is over the threshold.
		unsigned TofValidity:1;//1 if wTof is valid.
		unsigned IFOldReference:1;//(IFtracking) 1 if old reference time of flight has been used to synchronize the start or the stop.
		unsigned IFNotInitialized:1;//(IFtracking) 1 if the first reference time of flight has been initialized yet (because of acoustic or because of bad setting stop>start).
		unsigned reserved:1;
		unsigned wAcqIdAmp:16;//acquisition identifier (could be used to link the unique gate/cycle cscan data with the setting). But it is not required to use it.
		unsigned wTof:16;//cscan time of flight for the same gate.
		unsigned wAcqIdTof:16;//time of flight acquisition identifier (could be used to link the unique gate/cycle cscan data with the setting). But it is not required to use it.
}structCscanAmpTof_0x0202;
class _COM_DECL_ CSubStreamCscan_0x0X02
{
public:
	CSubStreamCscan_0x0X02();


	bool Check() const;//check consistancy of the sub strem.

	unsigned id:8;
	unsigned version:8;	//could be 1 or 2 (it depends of the setting, parameter RequestCscan with enumeration enumOEMPARequestCscan):
						//1: all cscan gate (amplitude only), packed together in the current sub stream, are for the same cycle number.
						//	 in this case the member "cylce" could used.
						//2: all cscan gate (amplitude + time of flight) of all cycles of the sequence are packed together in the same substream.
						//	 in this case the member "cylce" has no significance.
	unsigned size:16;
	unsigned timeStampLow:32;
	unsigned timeStampHigh:32;
	unsigned cycle:16;//cycle value in case "version" member value is 1, cycle=0xffff in case "version" member value is 2 (not significant in this case).
	unsigned count:15;//count of gates (amplitude only in case of "version" is 1, or amplitude + time of flight in case of "version" is 2).
	unsigned reserved:1;
	unsigned seqLow:32;//sequence number (LOW WORD).
	unsigned seqHigh:32;//sequence number (HIGH WORD).
};
class _COM_DECL_ CSubStreamAscan_0x0103
{
public:
	CSubStreamAscan_0x0103();

	bool Check() const;

	//ONLY USEFUL MEMBERS FOR THE USER HAVE COMMENT.
	unsigned id:8;
	unsigned version:8;
	unsigned size:16;
	unsigned timeStampLow:32;
	unsigned timeStampHigh:32;
	unsigned cycle:16;//cycle number.
	unsigned dataCount:16;//data count (dataCount x dataSize = total data size).
	unsigned src:1;
	unsigned dst:1;
	unsigned type:1;
	unsigned error:1;
	unsigned dataSize:4;//size of one data (in bytes), take a look also to member "bitSize".
	unsigned align:4;
	unsigned max:1;//maximum buffer is valid.
	unsigned min:1;//minimum buffer is valid.
	unsigned sat:1;//saturation buffer is valid.
	unsigned sign:1;//sign of maximum and minimum buffer data.
	unsigned bitSize:2;//see enumBitSize
	unsigned dacIFOldReference:1;//(IFtracking of the dac) 1 if old reference time of flight has been used to synchronize the start or the stop.
	unsigned dacIFNotInitialized:1;//(IFtracking of the dac) 1 if the first reference time of flight has been initialized yet.
	unsigned IFOldReference:1;//(IFtracking of the ascan gate) 1 if old reference time of flight has been used to synchronize the start or the stop.
	unsigned IFNotInitialized:1;//(IFtracking of the ascan gate) 1 if the first reference time of flight has been initialized yet.
	unsigned padding0:10;
	unsigned FWAcqIdChannelCycle:16;//acquisition Id for saturation data, see "OEMPA_SetAscanAcqIdChannelCycle".
	unsigned FWAcqIdChannelScan:16;//acquisition Id for minimum data, see "OEMPA_SetAscanAcqIdChannelScan".
	unsigned FWAcqIdChannelProbe:16;//acquisition Id for maximum data, see "OEMPA_SetAscanAcqIdChannelProbe".
	unsigned padding1:16;
	unsigned seqLow:32;//sequence number (LOW DWORD)
	unsigned seqHigh:32;//sequence number (HIGH DWORD)
};
class _COM_DECL_ CSubStreamParameters_0x0104
{//single address
public:
	CSubStreamParameters_0x0104();

	bool Check() const;

	unsigned id:8;
	unsigned version:8;
	unsigned size:16;
	unsigned timeStampLow:32;
	unsigned timeStampHigh:32;
	unsigned comId:16;
	unsigned count:16;
	unsigned src:1;
	unsigned dst:1;
	unsigned type:1;
	unsigned error:1;
	unsigned dataSize:4;
	unsigned align:4;
	unsigned retransmit:4;
	unsigned marge:16;
};
class _COM_DECL_ CSubStreamParameters_0x0204
{//multiple address
public:
	CSubStreamParameters_0x0204();

	bool Check() const;

	unsigned id:8;
	unsigned version:8;
	unsigned size:16;
	unsigned timeStampLow:32;
	unsigned timeStampHigh:32;
	unsigned comId:16;
	unsigned count:16;
	unsigned src:1;
	unsigned dst:1;
	unsigned type:1;
	unsigned error:1;
	unsigned dataSize:4;
	unsigned align:4;
	unsigned retransmit:4;
	unsigned marge:16;
};
class _COM_DECL_ CSubStreamParameters_0x0304
{//multiple address
public:
	CSubStreamParameters_0x0304();

	bool Check() const;

	unsigned id:8;
	unsigned version:8;
	unsigned size:16;
};
class _COM_DECL_ CSubStreamPadding_0x04
{//multiple address
public:
	CSubStreamPadding_0x04(bool bRequest);

	bool Check() const;

	unsigned id:8;
	unsigned version:8;
	unsigned size:16;
};
class _COM_DECL_ CSubStreamParameters_0x10
{//single address
public:
	CSubStreamParameters_0x10();

	bool Check() const;

	unsigned id:8;
	unsigned size:24;
	unsigned timeStampLow:32;
	unsigned timeStampHigh:32;
	unsigned count:32;
	unsigned src:1;
	unsigned dst:1;
	unsigned type:1;
	unsigned error:1;
	unsigned dataSize:4;
	unsigned align:4;
	unsigned retransmit:4;
	unsigned marge:16;
};
#pragma pack()

