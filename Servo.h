// Servo.h: interface for the CServo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVO_H__41D56D79_8753_4D20_A7A6_902B0D01DA09__INCLUDED_)
#define AFX_SERVO_H__41D56D79_8753_4D20_A7A6_902B0D01DA09__INCLUDED_

#include "StdAfx.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Include\Intr.h"

extern	UINT ServoThread (LPVOID lpvoid);


#define	BELL					7
#define	ACK						6
#define	LF						10
#define	CR						13
#define	ADDRS0					0
#define	ADDRS1					0x10


class CServo  
{
public:
	int m_nWritePtr;
	int	GetLoopCounter();
	int	 GetProgramRunning();
	BOOL IsAxisInPosition(int nWhich);
	BOOL IsMotionFinished();
	BOOL IsPolyMotionFinished();
	bool IsOriginFinished(int nAxis);
	void AimCompositesOriginSourceProgram();
	bool GetStallStatus();
	void SetRevCounter(int nCount);
	int	GetRevCounter();
	void ResetAllMaxSpeeds();
	void GetRollerPos(COORDDATA *pCp);
	void ZeroAxis(int nAxis);
	void StartContinuousRotate(int nAxis,float dRotateSpeed);
	void GetTurntablePos(COORDDATA *pCp);
	void TWIJoystickServiceRoutine();
	int GetBrakeStatus();
	void ZBrakesOn();
	void ZBrakesEnable(int bOffOn);
	int	GetESwitchStatus();
	bool m_bDataMemoryShared;
	char m_LastStrRx[1024];
	char m_LastStrTx[1024];
	int RequestMultipleVariable(char *VariableName, int nStartOffset,int nFinishOffset, char *Result);
	int RequestMultipleVariables(CString VariableName, int nStartOffset,int nFinishOffset, CString &Result);
	int RequestMVariableDefinitions(CString VariableName, int nStartOffset,int nFinishOffset, CString &Result);
	int RequestMVariableDefinitions(char *VariableName, int nStartOffset,int nFinishOffset, char *Result);
	bool IsHomeFindFinishedOnAxis(int nAxis);
	void SimpleBackwardForwardProgram();
	bool HomeFoundOnR();
	bool HomeFoundOnXYZXtYt();
	bool AreMotorsPowerd();
	bool IsThisSide(int nSide);
	bool IsMovementAllowed();
	bool DoesAxisNeedHoming(int nWhichAxis);
	void TurntableTo(int nSide);
	bool IsSystemParked();
	int m_nPlcAddrs;
	bool LiftCommandDown();
	bool LiftCommandUp();
	void DisableLookAhead();
	void SetupLookAhead();
	void FindMeanAmplitude(int nSamples, int *nPos);
	void FindPeakAmplitude(int nSamples, int *nPos);
	void PLCJoystickServiceRoutine();
	void GoniometricJoystickServiceRoutine();
	int GetCurrentProgram();
	bool IsHomeRequired();
	void SuspendLimitSwitches();
	void RestoreLimitSwitches();
	void RecoverFromLimitSwitches();
	bool ReportPumpStatus(int nWhich);
	void SetPumpSpeed(int nWhichAxis);
	float GetPumpSpeed(int nWhich);
	void PumpStop(int nWhich);
	void PumpStartAndSpeed(int nWhich,float dSpeed);
	void PumpStopStart(int nWhich,bool bStopStart);
	void PumpStart(int nWhich);
	void StopAllMotorsDead();
	void CoordEqualsCurrentPos(COORDDATA *pCp);
	void GetHeadPos(COORDDATA *pCp);
	void DesirePosEqualsCoord(COORDDATA *pCp);
	void UpdatePmacDesirePos();
	void OriginFindAxis(int nWhich);
	void HomeAllAxesWithoutMoving();
	void HomeAxisWithoutMoving(int nWhichAxis);
	void MoveToZero();
	void StartJoystick();
	void CalculateJoystickPositions();
	void StopServoThread();
	void StartServoThread();
	void Reset();
	int CalcMoveTime(struct AxisData *pAxis,int	nTotalS);
	bool WaitForFreeCoordinateSystems();
	bool SuspendJoystick(int nCoordinateMode);
	void StartScan();
	void MoveTo(BOOL bAction);
	int RequestVariable(char *VariableName,int nOffset,char *Result);
	int RequestVariable(CString VariableName, int nOffset, CString &Result);
	void SetVariable(char *VariableName,int nOffset,int nValue);
	void SetVariable(char *VariableName,int nOffset,float fValue);
	void SetVariable(char *VariableName,int nOffset,double dValue);
	void SetVariable(char *VariableName,int nOffset,char *pStr);
	void WriteDpram(int nOffset,int nValue);
	int	DpramSetMem(int nOffset,int nLength,PVOID pMem);
	int DpramGetMem(int nOffset,int nLength,PVOID pMem);
	LONG ReadDpram(int nOffset);
	LONG GetIrqCount();
	BOOL ShareTimeSlotStruct();
	BOOL FreeTimeSlotStruct();
	BOOL ShareUserServoStruct();
	BOOL FreeUserServoStruct();
	BOOL ShareDriverStruct();
	BOOL FreeDriverStruct();
	bool CollectData(int nAxis, int nStart, int nFinish, int nSamples, int nNumberAmpGates,int nNumberTimeGates=0,bool bContinuousRotate=FALSE);
	bool StopCollectingData();
	BOOL AllocateAndShareDataMemory();
	BOOL FreeAndDeallocateDataMemory();
	void DisableInterrupt();
	void EnableInterrupt();
	void JoystickPlc();
	void JoystickProgram();
	void OriginAllS455Program();
	void OriginAllProgram();
	void OriginX0Program();
	void OriginY0Program();
	void OriginZ0Program();
	void OriginXt0Program();
	void OriginYt0Program();
	void OriginR0Program();
	void MovetoProgram();
	void ScanProgram();
	void StartDualPolyMoveWithLookAhead(int nn);
	void StartPolyMoveWithLookAhead(int nn);
	void StartPolyMove(int nn);
	void StartDualPolyMove(int nn);
	int DownLoadNextPolyMoveCoordinate(int nNextCoordinate,CPolyCoord *ToolPath, int nDir);
	int DownLoadPolyMoveCoordinates(CPolyCoord *ToolPath, int nDir);

	void PolyMoveProgram();
	void GeneralIVariables();
	void GeneralMVariables();
	void StatusPlcProgram();
	void PositionPlcProgram();
	bool DownloadProgram(CString FilePath);
	void DownLoadProgram(char *String,int nm,int nn);
	BOOL CommsInitialize();
	void CommsClear();
	int CommsHit();
	int CommsGetch();
	void CommsPutch(char vv);
	int CommsGetStr(char *string);
	int CommsGetStr(CString &str);
	int CommsStr(char *string);
	int CommsStr(CString str, bool bWaitForResponce);
	void CommsStrNoCr(char *string);
	bool StartDriver();
	bool StartDriver(CString &strPmacType,int *nPmacType);
	void SuspendDriver();
	CServo();
	virtual ~CServo();
	BOOL GetHandleToDevice(int OS);
	WORD Inp(int nOffset);
	WORD Inpw(int nOffset);
	void Outp(int nOffset, int nValue);
	void GetVersion(CString* Version);
	void GetVersion(char *Version,int Length);

	void CStringToChar(CString str, char* chr);
	void CharToCString(char *chr, CString &str);

	HANDLE	m_hServo;
	int m_nJoystickMargin;
	int m_nJoystickPercent;
	bool m_bWorking[2];
	int	m_nCardNumber;
	int m_nAxisOffset;
	int m_nTimeout;
	int m_nMaxTimeoutUsed;
	CWinThread	*m_pThread;
	bool m_bThreadRun;
	int	m_nThreadAction;
	bool m_bAllMotorsHomed;
	bool m_bLookAheadFlag;
	COORDDATA	CpJoystickHead;
	COORDDATA m_SurfaceCp;
	COORDDATA m_HeadCp;

	char m_LastCommsStr[50];
	char m_UnSolicitedData[100];
	int	m_nUnSolicitedPtr;

	struct	SERVOData	m_Pmac;
	unsigned char	Array[1000000];
	int		m_nPmacAddrs;			//0 or 1
	
	bool		m_bThreadEnabled;
	bool		m_bThreadFinished;
	bool				m_bIntCallbackThreadFinished;
	bool				m_bIntCallbackThreadEnabled;
	HANDLE				m_hEvent;
	int					m_nEventsCount;

};


#endif // !defined(AFX_SERVO_H__41D56D79_8753_4D20_A7A6_902B0D01DA09__INCLUDED_)
