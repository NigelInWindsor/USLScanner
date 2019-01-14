// Pmac.h: interface for the CPmac class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PMAC_H__B408279D_59A8_4AB3_A03E_9BA2F62CC47E__INCLUDED_)
#define AFX_PMAC_H__B408279D_59A8_4AB3_A03E_9BA2F62CC47E__INCLUDED_

#include "Coord.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define NO_PMAC_DEVICE			0xFFFFFFFF

#include "PmacPcomm32.h"
#include "PmacPcommServer.h"
#include "PmacUSL.h"
#include "Servo.h"

#define PMAC1					1
#define PMAC2					2
#define PMAC_Ultralite			3

#define Turbo_PMAC1				6
#define Turbo_PMAC2				9

#define Turbo_PMAC_Ultralite	7
#define	Turbo_PMAC2_Ultralite	8

#define	PMAC_MOVE_FLAG_1			0x60
#define	PMAC_MOVE_FLAG_2			0x61

#define	PMAC_NUMBER_COORDS			0x62
#define	PMAC_MANIPULATOR_MODE		0x63
#define	PMAC_LOOP_COUNT_1			0x64
#define	PMAC_CURRENT_PROGRAM		0x65
#define	PMAC_JOYSTICK_TIP_MODE		0x66
#define	PMAC_COORDINATE_STATUS		0x67
#define	PMAC_LOOP_COUNT_2			0x68

#define	PMAC_TA						0x69
#define	PMAC_TM						0x6A
#define	PMAC_TS						0x6B

#define	PMAC_FEEDFORWARD			0x6E

#define	PMAC_DPRAM_DATA_START	0x70
#define	PMAC_TENTH_AXIS			0x5B
#define PMAC_SAFETY_UNIT_INPUTS	0x5C
#define	PMAC_GENERAL_OUTPUTS	0x5D
#define	PMAC_GENERAL_INPUTS		0x5E
#define	PMAC_GENERAL_INPUTS_EOR	0x5F
#define PMAC_PUMP_ENCODERS		0x3BC
#define	PMAC_DAC				0x3C0
#define	PMAC_FEEDRATE			0x3D0

#define	FIXTURE_SELECTED		0x01
#define	SCANNER_SELECTED		0x01
#define	TIP_SELECTED			0x02
#define	DEADMAN_SWITCH			0x04
#define	FAST_JOYSTICK			0x08

#define	COMMS32			0
#define	COMMSSERVER		1
#define	PMACCOMMSUSL	2
#define PMACISAUSL		3

#define	RED_LIGHT		0x80
#define	AMBER_LIGHT		0x40
#define	GREEN_LIGHT		0x20
#define	BLUE_LIGHT		0x10


class CPmac  
{
public:
	CCoord m_CpSurfacePump;
	void ChangePumpSpeedDueToHeight();
	void ClearGeoBrickFaults();
	void SetLimitSwitchesToDefault();
	void VerticalBoreScannerMoveProbes(float fPos);
	void VerticalBoreScannerMoveTurntable(int nPos);
	int m_nCoordinateStatus;
	int m_nProgramNumber[2];
	int m_nProgramStatus;
	bool m_bTakeShortestTurntableRoute;
	bool m_bRetrieveVirtualAxes;
	bool UpdateSafetyUnitInputs(int nValue);
	void JogMove(AxisData* pAxis,float fDistance,int nMode);
	void UpdateVirtualAxes();
	void SetPLCInterlocks();
	int m_nTunningDistance;
	void DownloadLimitSwitchOffsets();
	void DownloadSoftwareOverTravelLimits();
	void DisableSoftwareOverTravelLimits();
	void OriginRTheta(int nWhichAxis);
	void LiftInitialize();
	bool LiftCommandUp();
	bool LiftCommandDown();
	bool LiftCommandStop();
	bool IsLiftUp();
	bool IsLiftDown();
	int WhereIsLift();
	void AbortRotateThetaJoystick();
	inline int MinMax(int *pnV, int nMin, int nMax);
	void UpdateTurntableCartesianScanningAxes(CCoord Cp);
	void StartTurntableCartesianScanningAxes(CCoord Cp);
	void GetTurntableTruePos(int nPos);
	void StartXNumberOfRevs(float fNumber,float fSpeed);
	void GetTurntablePos(COORDDATA *pCp);
	bool IsRotateRunning();
	bool m_bLS_Disabled_Reenable_On_Exit[32];
	void ReEnableAllLimitSwitches();
	void EnableLimitSwitch(AxisData* pAxis);
	void DisableLimitSwitch(AxisData* pAxis);
	void EnableLights(int nFlag);
	void SetGeneralInputXor();
	void WriteGeneralOuput(int nValue);
	int GetGeneralOuput();
	bool				CheckPumps();
	CCoord				m_CpCollisionPoint;
	bool				m_bDriveAwayRequired[2];
	int					m_nDPRAMError;
	int					m_nPmacType;
	CString				m_strPmacType;
	bool				StartDriver();
	bool				m_bDriverOpen;
	int					m_nWhichDLL;
	CPmacPcomm32		m_Comms32;
	CPmacPcommServer	m_CommsServer;
	CPmacUSL			m_CommsUSL;
	CServo				m_ServoUser;

	//Poly variables
	float	m_fStopTime;
	float	m_fMaxStopTime;


	void RelativeMove(AxisData* pAxis,int nRelMove);
	void RelativeMove(AxisData* pAxis,float fRelMove);

	bool m_bWriteReentrant;
	int	m_nMachinePosition[1024];
	int	m_nPosition[1024];
	int	m_nStatus[1024];
	int	m_nFollowingError[1024];
	int	m_nVelocity[1024];
	int	m_nDAC[1024];
	int	m_nPumpsVanes[4];

	int	m_nDPRAMWriteAddrs[1024];
	int	m_nDPRAMWriteValue[1024];
	int	m_nDPRAMWritePtr;
	int	m_nDPRAMWriteCurrent;

	int	m_nGeneralOutput;
	int	m_nGeneralInput;
	int	m_nLastGeneralInput;

	int	m_nJoyDev[1024];
	int	m_nDesirePos[1024];
	bool m_bJoyDev;
	bool m_bDesirePos;

	void MoveToHighGain();
	void JogTo(int nAxis,int nPos);
	int GetAxisEncoderPos(int nAxis);
	void StopContinuousRotate(int nAxis);
	int	m_nLastPos[32];
	void StartContinuousRotate(int nAxis,float fRotateSpeed, int nCW_CCW = 0);
	int GetNumberAxes();
	void SetFeedRate(float fPercentageRate);
	void AirStop(int nMask);
	void AirStart(int nMask);
	bool ReportAirStatus(int nMask);
	void SetScanSpeed(float fSpeed);
	void SetScanSpeedSingleBridgeDualSided(float fReqSpeed);
	void ResetAllMaxSpeeds();
	int m_nTunningAxis;
	int m_nTunningPos;
	void TuningMove();
	int m_nPumpPurgeStartTime[32];
	void PurgePump(int nWhich,bool bStart=FALSE);
	int m_nScanSide;
	void HomeAxisWithoutMoving(int nWhichAxis);
	void DownloadPIDForManipulators();
	int m_nWritePtr;
	CCoord m_CpJoystickSurface;
	CCoord m_CpJoystickHead;
	CCoord m_CpSurface;
	CCoord m_CpHead;
	int nIrqCount;
	void WriteDpramFifo(int nOffset, int nValue,int nAction);
	void WriteDpram(int nOffset,int nValue, bool bDoMemCheck = false);
	int ReadDpram(int nOffset);
	void DpramCheckWrite(int nOffset,int nLength,PVOID pMem);
	int DpramSetMem(int nOffset,int nLength,PVOID pMem, bool bDoMemCheck = TRUE);
	int DpramGetMem(int nOffset,int nLength,PVOID pMem);
	bool DownLoad(CString FilePath);
	int	m_nThreadAction;
	int	m_nSubThreadAction;
	int	m_nWhichJoystickAxis;
	int	m_nJoystickLoopCount;
	BOOL m_bThreadRun;
	BOOL m_bInterrupt;
	bool m_bLookAheadFlag;

	void RequestAllAccelerationsAndVelocities();
	void SetupLookAhead();
	void DisableLookAhead();
	int DownLoadPolyMoveCoordinatesSingleBridgeDualSided(CPolyCoord *ToolPath, int nDir);
	int DownLoadNextPolyMoveCoordinateSingleBridgeDualSided(int nNextCoordinate,CPolyCoord *ToolPath, int nDir);
	int DownLoadPolyMoveCoordinates(CPolyCoord *ToolPath, int nDir);
	int DownLoadNextPolyMoveCoordinate(int nNextCoordinate,CPolyCoord *ToolPath, int nDir);
	void StartPolyMoveWithLookAhead(int nn);
	int GetLoopCounter();
	BOOL IsPolyMotionFinished();

	CCriticalSection	m_CriticalSection;


	void DoAStep(int nAxis, int nDistance, int nTime);
	void ReportPumpEncoderValues(int *pArray);
	void SetPumpSpeed(int nWhichAxis);
	bool ReportPumpStatus(int nWhichAxis);
	void PumpStopStart(int nWhich,bool bStopStart);
	void PumpStart(int nWhich);
	void PumpStop(int nWhich);
	void Retrieve(CString FilePath);
	void Save(CUSLFile* pFile);
	void StartJoystick();
	void RotateThetaJoystick(int nNotSafe);
	void PLCJoystickServiceRoutine12Axes(int nNotSafe);
	void PLCJoystickServiceRoutine10Axes(int nNotSafe);
	void PLCJoystickServiceRoutingSBT(int nNotSafe);
	void PLCJoystickServiceRoutine(int nNotSafe);
	bool DoesAxisNeedHoming(int nWhichAxis);
	bool WaitForFreeCoordinateSystems();
	BOOL IsPhysicalAxisInPosition(int nWhich);
	BOOL IsAxisInPosition(int nWhich);
	BOOL IsMotionFinished();
	BOOL IsMoveRFinished();
	int CalcMoveTime(struct AxisData *pAxis,int	nTotalS);
	void EnableSingleBridgeTTRollers(int nAction);
	void EnableVerticalBoreScanner(int nAction);
	void Enable12Axes(int nAction);
	void Enable10AxesVersion2(int nAction);
	void Enable6Axes(int nAction);
	void Enable7Axes(int nAction);
	void Enable10Axes(int nAction);
	void EnableAxis(int nAxisStruct,int nPhysicalAxisOffset, char cCoordAxis);
	void EnableAxis(int nAxisStruct);
	void EnableAxes(int nAction);
	bool MoveJustR(bool bAction);
	bool MoveTo(bool bAction);
	void UpdateHardwareDesirePos();
	bool OriginAll(int nAction);
	bool OriginFindAxis(int nWhichAxis);
	bool SuspendMotors();
	int GetBrakeStatus(int nWhich);
	int	GetInputPort();
	void ZBrakesEnable(int bOffOn,int nWhich);
	void DisableInterrupt();
	void EnableInterrupt();
	void SetVariable(CString VariableName, int nOffset, int nValue);
	void SetVariable(CString VariableName, int nOffset, float fValue);
	void SetVariable(CString VariableName, int nOffset, CString Value);
	int RequestMVariableDefinitions(CString VariableName, int nStartOffset,int nFinishOffset, CString &RxBuff);
	int RequestMultipleVariables(CString VariableName, int nStartOffset,int nFinishOffset, char *Result);
	int RequestMultipleVariables(CString VariableName, int nStartOffset,int nFinishOffset, CString &RxBuff);
	BOOL RequestVariable(CString VariableName, int nOffset, CString *pRxBuff);
	BOOL RequestSlaveVariable(CString VariableName, int nNode, int nOffset, CString *pRxBuff);
	int RequestResponce(CString TxBuff, CString *pRxBuff);
	BOOL GetStr(CString &RxBuff);
	int SendStr(CString TxBuff,bool bWaitForResponce=FALSE);
	bool SuspendDriver();
	void KillAllMotors();
	bool IsMachineZero();

	CPmac();
	virtual ~CPmac();


	FILETIME m_n64LastTime;

};

#endif // !defined(AFX_PMAC_H__B408279D_59A8_4AB3_A03E_9BA2F62CC47E__INCLUDED_)
