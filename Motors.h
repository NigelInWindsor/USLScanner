// Motors.h: interface for the CMotors class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOTORS_H__E1D51F32_B71E_468A_80D9_2CA9C17F6297__INCLUDED_)
#define AFX_MOTORS_H__E1D51F32_B71E_468A_80D9_2CA9C17F6297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	LEFT_MOVETO				1
#define	RIGHT_MOVETO			2
#define	BOTH_MOVETO				3
#define	ENABLE_JOYSTICK			4
#define	BOTH_POLY_MOVE			5
#define	TUNNING					6
#define	LEFT_POLY_MOVE			7
#define	RIGHT_POLY_MOVE			8
#define	BOTH_MOVETO_WITH_R		9
#define	ENABLE_JOYSTICK_R_THETA	10
#define	LEFT_MOVETO_WITH_R		11
#define	BOTH_MOVETO_WITHOUT_R	12
#define	PEAK_OPTOMISE			13

#define	_MACHINE			0
#define _HEAD				1
#define	_SURFACE			2

class CMotors  
{
public:
	void IntelligentGotoSurfaceCoord(CCoord *pCpSurface, int nSideMask, int nDangerPlaneMask);
	void EnableCrashDetector(bool bFlag, int nSide);
	void OriginFingerManipulator();
	bool IsAxisInMachinePosition(int nAxis, int nDesirePos);
	void LockTool(int nSide);
	void UnlockTool(int nSide);
	bool IsMachineZero();
	void AbsoluteMove(AxisData* pAxis,int nJogposition);
	void RelativeMove(AxisData* pAxis,int nJogDistane);
	bool IsAlive();
	void ResetAllMaxSpeeds();
	void RequestAllAccelerationsAndVelocities();
	bool IsItAtDesirePos();
	bool IsMotionStarted();
	void SetupLookAhead();
	void CollectBladeData(bool bAction);
	void PumpStop(int nAxis, int nSide);
	void PumpStart(int nAxis, int nSide);
	void SetPumpSpeed(int nAxis, int nSide);
	bool ReportPumpStatus(int nAxis, int nSide);
	void GotoHeadCoord(CCoord Cp);
	void GetTipAngles(int nSide,float *pfXt, float *pfYt);
	void StartXNumberOfRevs(float fNumber,float fSpeed);
	void TotalDistanceTravelled();
	void EnsureJoystickValuesAreCurrent();
	void ZeroHoldingFixturePosition(int nAxis);
	void EnableHomingFixtureHomeFind(int nAxis);
	void HoldingFixtureZeroPosition(int nAxis);
	bool TestHoldingFixtureAllAxisRunning();
	void MoveHoldindFixtureToCoord(HoldingFixtureCoord Cp);
	void CollectComplexData(bool bAction);
	void StartPolyMoveWithLookAhead(int nn);
	int DownLoadNextPolyMoveCoordinate(int nNextCoordinate,CPolyCoord *ToolPath, int nDir);
	int DownLoadPolyMoveCoordinates(CPolyCoord *ToolPath, int nDir);
	void SetScanSpeed(float fSpeed);
	bool IsPolyMotionFinished();
	void StopContinuousRotate(int nAxis,bool bControlledStop = true);
	void GotoSurfaceCoord(CCoord* pCpSurface,int nSide);
	bool IsMotionFinished();
	bool WaitMotionFinished(double dSeconds);
	bool IsAxisInPosition(int nAxis);
	bool IsMoveRFinished();
	void GetEncoderPos(CCoord *pCp);
	int GetRevCount();
	void SetRevCount(int nRev);
	void StartTurntableCartesianScanningAxes(CCoord Cp);
	void UpdateTurntableCartesianScanningAxes(CCoord Cp);
	void CollectEccentricData(bool bCollectData);
	void CollectNormalizeData(bool bCollectData, int nAxis=0);
	int	m_nJoyValue[5];
	int	m_nSwitchJoyValue;
	int m_nToggleJoyPos;

	int m_nRevCounter;
	
	
	void EnableAxes(int nAction);
	void MoveJustR(int nAction);
	void MoveTo(int nAction, int nFrame = _SURFACE);
	void DesirePosEqualsHeadCoord(CCoord Cp, int nAction=0);
	void GetTurntablePos(CCoord * pCp);
	void DesirePosEqualsCoord(CCoord Cp, int nAction=0);
	void SuspendMotors();
	void GetHoldingCoord(HoldingFixtureCoord* pCp);
	void ZeroAxis(int nWhich);
	void CalculateRollerStepSize();
	void OriginFindAxis(int nWhichAxis);
	void OriginAll(int nThreadAction, int nAxesMask = 0);
	void GetRollerPos(COORDDATA *pCp);
	void StartContinuousRotate(int nAxis,float fSpeed,int nDir=0);
	bool IsRotateRunning();
	void GetSurfacePos(CCoord *pCpSurface,int nFL = 0);
	void GetHeadPos(COORDDATA *pCp);
	void GetMachinePos(COORDDATA *pCp);
	void KillAllMotors();
	int	 GetInputPort();
	void ZBrakesEnable(int bOffOn, int nWhich);
	int GetBrakeStatus(int nWhich);
	void AirStart(int nMask, int nSide);
	void AirStop(int nMask, int nSide);
	bool ReportAirStatus(int nMask, int nSide);
	

	CMotors();
	virtual ~CMotors();

};

#endif // !defined(AFX_MOTORS_H__E1D51F32_B71E_468A_80D9_2CA9C17F6297__INCLUDED_)
