// Ethercat.h: interface for the CEthercat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ETHERCAT_H__B2F92F1A_0185_4CF7_8737_DC1A20C398D5__INCLUDED_)
#define AFX_ETHERCAT_H__B2F92F1A_0185_4CF7_8737_DC1A20C398D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\Hilscher\Include\cifxuser.h"
#include ".\Hilscher\Include\cifxErrors.h"
#include ".\Hilscher\Include\rcx_public.h"
#include ".\Hilscher\Include\rx_Results.h"


#define CARD_CONNECTED		0x01
#define	CHANNEL_OPEN		0x02
#define	HOST_READY			0x04
#define	ETHERCAT_BUS_ON		0x08

#pragma pack(push, 1)

struct	EthercatIO {
	union {
		struct {
			unsigned char cMachineDescription;
			unsigned char cCntrl[3];
		};
		int nCntrl;
	};
	unsigned char cArray[130];
	double	dHRTime;
};


struct Global32AxisIO {
	unsigned char cDataFormat;
	unsigned char cLife;
	unsigned __int16 cCoordStatus[3];
	int nEncoderPos[32];
	unsigned char cAxisStatus[32];
};


struct TwinRobotIO {
	unsigned char cDataFormat;
	unsigned char cLife;
	unsigned char cGeneralStatus;
	unsigned char cCoordStatus[3];
	int nEncoderPos[14];
	unsigned char cAxisStatus[14];
};


struct HengshenIO {
	unsigned char cDataFormat;
	unsigned char cLife;
	unsigned __int16 cCoordStatus[3];
	int nEncoderPos[13];
	unsigned char cAxisStatus[13];
	unsigned char cPumpStatus;
	unsigned char cAirKnifeStatus;
	unsigned char cToolIDClampStatus[2];
	char cTemperature[2];
	unsigned char cRackDoors;
	unsigned char cCrashDetect;
	unsigned char cGeneralInput;
};

struct SPXRobotIO {
	unsigned char cDataFormat;
	unsigned char cLife;
	unsigned char cGeneralStatus;
	unsigned char cCoordStatus[3];
	int nEncoderPos[18];
	unsigned char cAxisStatus[18];
};

struct RailwayAxleIO {
	unsigned char cDataFormat;
	unsigned char cLife;
	unsigned __int16 cCoordStatus[3];
	int nEncoderPos[7];
	unsigned char cAxisStatus[7];
};

struct Tank5AxisIO {
	unsigned char cDataFormat;
	unsigned char cLife;
	unsigned __int16 cCoordStatus[3];
	int nEncoderPos[5];
	unsigned char cAxisStatus[5];
	unsigned char cPumpStatus;
	unsigned char cAirKnifeStatus;
	unsigned char cEStopStatus;
};

struct Tank6AxisIO {
	unsigned char cDataFormat;
	unsigned char cLife;
	unsigned __int16 cCoordStatus[3];
	int nEncoderPos[6];
	unsigned char cAxisStatus[6];
};

struct Tank10AxisIO {
	unsigned char cDataFormat;
	unsigned char cLife;
	unsigned __int16 cCoordStatus[3];
	int nEncoderPos[10];
	unsigned char cAxisStatus[10];
	unsigned char cPumpStatus;
	unsigned char cAirKnifeStatus;
	unsigned char cEStopStatus;
};

#pragma pack(pop)

class CEthercat  
{
public:
	int EncoderPos(int nAxis);

	CString &getRackDoorStatus();
	CString &getTemperature(int nWhich);
	CString &getToolClampStatusAndID(int nSide);
	CString &getPumpStatus();
	CString &getAirKnifeStatus();
	CString &getEStopStatus();
	CString &getEncoderPos(int nAxis);
	CString & getStrAxisStatus(int nAxis);
	CString &getCoordStatus(int nCoord);
	CString & getStrDataFormat();
	int getAxisStatus(int nAxis);
	int getDataFormat(int * nDataFormat = NULL);
	CString &getLifeCount();
	CString &getCallbackCount();
	CString &getCallbackPeriod();

	static CString strDataFormat;
	int getMotorStatus(int nMotor);

	int ConvertAxisStatusToPmacAxisStatus(int cStatus);
	int CheckForRollOver(int nAxis, int nPos);

	unsigned char m_abBuffer[1000];
	unsigned char* m_pabDPMMemoryIN;
	uint32_t m_ulAreaStartOffsetIN;
	uint32_t m_ulAreaSizeIN;
	unsigned char* m_pabDPMMemoryOUT;
	uint32_t m_ulAreaStartOffsetOUT;
	uint32_t m_ulAreaSizeOUT;
	PLC_MEMORY_INFORMATION m_tMemoryOUT;
	PLC_MEMORY_INFORMATION m_tMemoryIN;
	void RegisterPLCPointers();
	void DeRegisterPLCPointers();

	bool IsRotateRunning();
	void TestPlc();
	int	 GetInputPort();
	bool IsAlive();
	bool ReportCoordStatus(int nCoord);
	bool ReportWaterSpray();
	void InvalidateRobotJoints();
	void Transfer32AxisIOCoordStatus();
	bool ReportPumpStatus(int nAxis, int nSide);
	void Si10CallBack();
//	HengshenIO	m_HengshenIO;
	Global32AxisIO	m_32AxisIO[2];
	EthercatIO	m_EthercatIO[2];
	CHRTimer	m_HRTimer;
	CCriticalSection	m_CriticalSection;

	unsigned char m_RxArray[sizeof EthercatIO];
	unsigned char m_TxArray[sizeof EthercatIO];

	int	m_nOldPos[32];

	void RegisterInterruptCallback();
	void DeRegisterInterruptCallback();
	HANDLE	m_hEvent;
	CWinThread* m_pThread;
	bool m_bThreadEnabled;
	bool m_bThreadFinished;


	int m_nCallBackCount;
	int	m_nOldCallBackCount;
	int SwitchToSyncmode();
	void DeRegisterNotificationCallback();
	void RegisterNotificationCallback();
	bool IOWrite(void *pArray,int nLength);
	bool IORead(void *pArray,int nLength);
	bool IORead(int nLength);
	void EnumBoard();
	uint32_t	m_ulState;
	int	m_nBusStatus;
	HANDLE m_hDriver;
	HANDLE m_hChannel;
	bool m_bConnected;
	bool m_bFieldBusConnected;
	bool IsConnected();
	bool IsFieldBusConnected();
	void Close(CEdit *pEdit = NULL);
	void Initialize(CEdit *pEdit = NULL);
	void StartFieldBus();
	CEthercat();
	virtual ~CEthercat();

};

#endif // !defined(AFX_ETHERCAT_H__B2F92F1A_0185_4CF7_8737_DC1A20C398D5__INCLUDED_)
