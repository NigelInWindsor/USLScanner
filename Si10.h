// Si10.h: interface for the Si10 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SI10_H__2CAFBE13_71D3_453B_A06D_341559B2C53F__INCLUDED_)
#define AFX_SI10_H__2CAFBE13_71D3_453B_A06D_341559B2C53F__INCLUDED_

#include "PolyCoord.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\include\PLXAPI.h"
#include ".\include\Reg9030.h"
#include ".\SI10Driver\Include\PlxIoctl.h"
#include <afxmt.h>

#define PCI_DEVICE_ID 0x9030; // Values for PCI9030
#define PCI_VENDOR_ID 0x10B5; //   "     "     "

//Used in LoadFPGAConfigurationData()
#define FPGACfgFileNameVerChar "A"
#define FPGACfgFileNameRevChar "1"
#define FPGARevisionByte1 'A'
#define FPGARevisionByte2 '1'
#define FPGARevisionByte3 '0'
#define FPGAVarientByte   ' '



#define SI10ApiSuccess 512

class CSi10
{
public:
	CSi10();
	virtual ~CSi10();


	//functions
public:
	void	SetGPIOIRQMode();
	void	UpdateTurntableCartesianScanningAxes(CCoord Cp);
	int		m_nOldProcessState;
	void	EnableLights(int nFlag);
	void	EnableHomeFind(int nAxis);
	void	ZeroPosition(int nWhich);
	void	EncoderZero(struct AxisData	*pAxis);
	void	MoveToCoord(HoldingFixtureCoord Cp);
	bool	FindLatestBinaryFile(WCHAR* pStr);
	bool	FindDevice();
	BOOL	IsConnected();
	bool	StartDriver();
	void	EnableAxis(int nWhich,int action,int nReqSpeed=-1);
	void	DisableAxis(int nWhich);
	bool	IsMotionFinished();
	bool	TestAllAxisRunning();
	bool	TestHoldingFixtureAllAxisRunning();
	bool	TestAxisRunning(int nWhich);
	int		ReadJoystick(int nWhich);
	U32		InpW(int nOffset);
	void	OutpW(int nOffset, U32 Value);
	int		Si10ReadTS0();
	void	Si10SetPRF(int nPrf);
	void	Reset();
	void	Si10ArmPRF();
	bool	ShareUTStruct();
	bool	FreeUTStruct();
	bool	CollectData(void* ptr=NULL, int nScanLine=0, int nStepSize=0, int nSamplePeriod=0);
	void	StopAllMotorsDead();
	void	EnableJoystick();
	void	MoveTo(int nAction);
	int		DownLoadPolyMoveCoordinates(CPolyCoord *ToolPath, int nDir);
	void	CheckHoldingBrake();
	int		GetInputPort();
	void	StartContinuousRotate(int nAxis, float Speed, int nDir=0);
	void	StartContinuousRotate(int nAxis, float fIncrement, float fStartPos, float fFinishPos);
	void	StopContinuousRotate(int nAxis);
	int		GetRevCounter(int nAxis);
	void	SetRevCounter(int nAxis, int nValue);
	bool	IsRotateRunning();
	void	StartXNumberOfRevs(float fNumber,float fSpeed);
	void	MoveJustR(int nAction);
	bool	IsAxisInPosition(int nAxis);
	void	EnableAxes(int nAction);
	void	DownLoadCoord(CCoord Cp);
	void	MoveTurntableCartesianScanningAxes(CCoord Cp);
	void	WriteGeneralOuput(int nValue);
	int		GetGeneralOuput();
	void	DeRegisterInterruptCallback();





private:
	CCriticalSection	m_CriticalSection;
	bool	LoadFPGAConfigurationData();
	void	AxisDefaults(int nWhich);
	bool	isFPGALoaded(CString* pBuff = NULL);
	bool	SI10DeviceOpen();
	bool	SI10PciBarSpaceRead(U32 addrs, U32 ByteCount,U32 *pData);
	bool	SI10PciBarSpaceWrite(U32 addrs, U32 ByteCount,U32 *pData);
	void	RegisterInterruptCallback();
	bool	ShareTimeSlotStruct();



	//variables
public:
	void	CounterStart(int number, int value);
	bool	ShareUserSi10Struct();
	int GetFieldBusRegister();
	void SetScanSpeed(float fSpeed);
	void CloseLaserDoor();
	void OpenLaserDoor();
	int ReadAbsStatus(int nWhich, CString &Buff);
	int ReadAbsEncoder(int nWhich);
	void	TestPatternOutput(int nPattern);
	void	GetFirmwareVersion(CString* pBuff);
	void	CloseDevice();
	int		ReadLimitSwitches();
	bool	GetDriverVersion(CString *pBuff);
	int					m_nGeneralInput;
	int					m_nGeneralOutput;
	int					m_nEventsCount;
	bool				m_bConnected;
	struct	SI10Data	m_Si10;
	bool				m_bForce;
	bool				m_bIntCallbackThreadFinished;
	bool				m_bIntCallbackThreadEnabled;
	HANDLE				m_hEvent;
	CWinThread*			m_pThread;
	PLX_DEVICE_OBJECT	m_Device;
	U32					m_nCountersEnabled; //usd to keep track of which counters are enabled
	char				m_Array[10000000];
	bool				m_bInternalPRF;
	bool				m_bContinuousPRF;
	bool				m_bPRFCreateInt;
	struct SI10UTData	m_Si10UTData;


};


UINT	PendForInterrupt(LPVOID pParam);


#endif // !defined(AFX_SI10_H__2CAFBE13_71D3_453B_A06D_341559B2C53F__INCLUDED_)
