// FieldBusController.h: interface for the CFieldBusController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDBUSCONTROLLER_H__FF19C828_D2E8_4734_AC89_538922490AC5__INCLUDED_)
#define AFX_FIELDBUSCONTROLLER_H__FF19C828_D2E8_4734_AC89_538922490AC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Ethernet.h"
#include "ClientSocket.h"
#include "Coord.h"	// Added by ClassView

#define _PP_DESVELZERO		0x0000400000000000
#define _PP_INPOS			0x0000080000000000
#define _PP_SEGSTOPREQ		0x0000000000010000
#define _PP_SEGSTOPREQ		0x0000000000010000
#define _PP_SEGMOVE			0x0000000000300000
#define	_PP_LASTSEGMOVE		0x0000000000040000
#define	_PP_CONTMOTION		0x0000000004000000
#define	_PP_MOTIONFINISHED	0x0000700000000000
#define	_PP_LOOKAHEAD		0x0000000000004000

#define	_PP_MOVETIMER		0x0001000000000000
#define	_PP_TIMERSENABLED	0x0000010000000000
#define	_PP_AMP_ENABLED		0x0000100000000000


#define	MOVETO_INCLUDE		0
#define JOYSTICK_INCLUDE	1

#define MAX_NUMBER_STRINGS 10000
#define	RX_STR		0x000
#define	TX_STR		0x001
#define	ERROR_STR	0x002

#define	_TERMINAL		0
#define	_BACKGROUND		1


extern	long FieldBusTerminalCallback(CString *pBuff);
extern	long FieldBusBackGroundCallback(CString *pBuff);

struct PPMACMotor {
	int	nJogSpeed;
};

struct PPMACSys {
	float fServoPeriod;
	int		nServoCount;
};

struct PPMACEcat {
	int nDCClockDiff;
};

struct PPMACCoord {
	unsigned __int64	n64Status;
};

class CFieldBusController  
{
public:
	void DownloadAxisParameters(int nAxis);
	void DownloadAllAxisParameters();
	void DownloadPPMACKinematics(int nSide);
	bool WaitAxesEnabled(int nCoordMask);
	bool IsMotorsEnabled(int nCoordMask);
	void EnableCrashDetector(bool bFlag, int nSide);
	void OriginFingerManipulator();
	struct	PPMACEcat	m_ECAT;
	struct	PPMACSys	m_Sys;
	struct	PPMACMotor	m_Motor[32];
	struct	PPMACCoord	m_Coord[8];

	int	m_nGeneralEthercatInput;
	int m_nCrashDetectStatus[2];
	int m_nDoorStatus[2];
	int m_nPumpStatus[2];
	int m_nAirKnifeStatus[2];
	int m_nEStopStatus[2];
	int	m_nTemperature[10];
	int	m_nOilerStatus[10];
	int m_nToolID[2];
	int m_nToolHolderStatus[2];
	CStringArray m_strWatchArray;
	float m_fWatchArray[100];
	int m_nWatchRxStr;
	int	m_nBkRxStr;
	static const int m_nMaxNumberAxes = 32;

	bool IsAxisInMachinePosition(int nAxis, int nPos);
	bool WaitForAmplifiersEnabled(int nMask,int nMaxWaitTime_ms);
	void AndonEvent(int nCode);
	bool IsRAxisInPosition(float fPos);
	bool IsRotateRunning();
	bool IsDoorClosed(int nSide);
	bool IsToolLocked(int nSide);
	void UnlockTool(int nSide);
	void LockTool(int nSide);
	int	RequestEthercatGeneralInput();
	int RequestToolHolderStatus(int nWhich);
	int RequestToolID(int nWhich);
	int	RequestTemperature(int nWhich);
	int	RequestOiler(int nWhich);
	int DecodeAllStrings(CString *pBuff);
	void SendStr(CString Buff, int nWhichSocket = _TERMINAL);
	void SendAxesIncluded(int nWhichSet, bool bForce);
	void SendRobotWeightings(int nMode, bool bForce);
	void DisasterRecovery();
	void JogJustR(float fAngle, float fRotateSpeed);
	void AbsoluteMove(AxisData* pAxis,int nJogposition);
	void RelativeMove(AxisData *pAxis,int JogDistance);
	bool IsJogMoveFinished(int nSide, int nWhat);
	bool IsAxesInPos(int nAxisMask);
	void StartContinuousRotate(int nAxis,float fRotateSpeed, int nCW_CCW = 0);
	void StopContinuousRotate(int nAxis, bool bControlledStop = true);
	void UpdateTurntableCartesianScanningAxes(CCoord Cp);
	void StartTurntableCartesianScanningAxes(CCoord Cp);
	void ResetAllMaxSpeeds();
	void RequestAllAccelerationsAndVelocities();
	void SetScanSpeed(float fSpeed);
	bool IsItAtDesirePos();
	bool ReportAirStatus(int nMask, int nSide);
	void AirStart(int nMask, int nSide);
	void AirStop(int nMask, int nSide);
	void ChangePumpSpeedDueToHeight();
	void StartPolyMoveWithLookAhead(int nn);
	void SetupLookAhead();
	int DownLoadPolyMoveCoordinates(CPolyCoord *ToolPath, int nDir);
	int CalcMoveTime(struct AxisData *pAxis,int	nTotalS);
	bool MoveTo(int nAction, int nFrame = _SURFACE);
	void DisableLookAhead();
	bool WaitForFreeCoordinateSystems();
	void EnableAxis(int nAxisStruct,int nPhysicalAxisOffset, CString strCoordAxis);
	void EnableAxis(int nAxisStruct);
	void EnableAxes(int nAction);
	void Origin(int nWhichAxis);
	void ResetKarlMorganOperationMode();
	int	m_nGeneralOutput;
	void EnableLights(int nFlag);
	void RRWaterSpray(int noffOn);
	void ToggleRRWaterSpray();
	void SuspendEthercat();
	void StartEthercat();
	void KillAllMotionPrograms();
	void KillMotors();
	int m_nScanLine;
	void GetPeekCoordinate(CCoord *pCp);
	void SuspendMotors();
	void EnableMotors();
	bool IsPeekCoordinateFound();
	CCoord m_CpPeek;
	bool m_bPeekCoordinateFound;
	void RequestCoordinates();
	CUSLFile m_FileDebug;
	CVertexArray m_vDataGather;
	CString Gather;
	int	m_nIndex;
	int	m_nItem;
	int	m_nIndexL;
	int m_nOldFrameToolJoyMode;
	float m_fLastJogCommand[5];
	int m_nJoyZeroValue[5];
	void Joystick(int nNotSafe = 0);
	void JoystickWithKinematics(int nNotSafe = 0);
	void JoystickWithoutKinematics(int nNotSafe = 0);
	void JoystickTwinTower(int nNotSafe = 0);
	void EnableJoystick();
	void SendFrameToolJoyMode(bool bForce);
	bool m_bTxNextMsg;
	int	m_nLastMsgCounter;
	bool IsMotionFinished();
	bool IsMotionStarted();
	void DownloadEndEffectorWithWaterPath(int nRobot, bool bImmediately);
	void PumpStop(int nAxis, int nSide);
	void HomeAxisWithoutMoving(int nPhysicalAxis, int nOffset = 0);
	void PumpStart(int nAxis, int nSide);
	bool ReportPumpStatus(int nAxis, int nSide);
	void SetPumpSpeed(int nAxis, int nSide);
	void DownloadRobotJoints(int nRobot);
	void DownloadRobotKinematics(int nRobot);
	void DoPolyLine(CPolyCoord *pLine, float fMoveSpeed, float fInitialSpeed);
	int bJoystickMode;
	bool bJoystickEnabled;
	float m_fTAMove[3];
	float m_fTDMove[3];
	float m_fTMMove[3];
	float m_fTSMove[3];
	int	m_nProgActive[3];
	__int64 m_n64CoordStatus[3];
	int	m_nPlcLifeCount;
	int	m_nProgramEnable;
	int m_nProgramFinished;
	int	m_nCoordPtr;
	int	m_nNumberCoords;

	CClientSocket m_TerminalSocket;
	CClientSocket m_BackGroundSocket;
	CStringArray m_RxArray;
	unsigned char m_ArrayFlag[MAX_NUMBER_STRINGS];
	CString m_LastSendStr;
	int	getRxArraySize();
	CString getRxArrayAt(int nIndex);
	int GetRxFlagAt(int nIndex);
	void AddRxStr(CString * pBuff);


	bool SocketConnect();
	void Zero();
	bool Retrieve(CFile* pFile);
	int Save(CFile *pFile);

	__int64 m_n64Status[14];
	WCHAR	m_wPassword[200];
	WCHAR	m_wUser[200];
	int	m_nProtocol;
	int	m_nPort;
	int m_nIPAddrs;
	bool m_bEnableAtStartUp;
	int	m_nDCRefBand;
	int	m_nDCRefPlus;
	int	m_nDCRefMinus;
	int	m_nPostProcessing;
	int	m_nProcessingSide;
	int	m_nGeneralInput;
	float	fLastJogSpeed[20];
	void WriteJogSpeed(int nAxis,float fSpeed);
	int	nLastDesirePos[20];
	void WriteDesirePos(int nAxis,int nDesirePos,int nForce = 0);
	CCoord m_CpJoystickHead;
	CCoord m_CpHead;
	CCoord m_CpSurface;
	CCoord m_CpSurfacePump;

	void SuspendStartUpThread();
	void StartStartUpThread();
	void SuspendCommsThread();
	void DownloadPolyLine(CPolyCoord * pLine, int nFirstCoordinate);
	void StartCommsThread();
	int	m_nThreadAction;

	bool m_bStartUpThreadEnabled;
	bool m_bStartUpThreadFinished;
	CWinThread* m_pStartUpThread;

	bool m_bCommsThreadEnabled;
	bool m_bCommsThreadFinished;
	CWinThread* m_pCommsThread;

	void Initialize();
	bool IsConnected(int nWhichSocket);

	void SetHomePosToEncoderPos(int nAxis);
	void SetHomePos(int nAxis, int nHomePos);
	void SetHomePos(int nAxis);
	void SetHomePosAll();

	CFieldBusController();
	virtual ~CFieldBusController();

};

UINT	FieldBusThread(LPVOID pPAram);

#endif // !defined(AFX_FIELDBUSCONTROLLER_H__FF19C828_D2E8_4734_AC89_538922490AC5__INCLUDED_)
