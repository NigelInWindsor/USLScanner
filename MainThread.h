// MainThread.h: interface for the CMainThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINTHREAD_H__F56DEA48_C622_4C3C_8EE8_494BBE323D5E__INCLUDED_)
#define AFX_MAINTHREAD_H__F56DEA48_C622_4C3C_8EE8_494BBE323D5E__INCLUDED_

#include "StdAfx.h"	// Added by ClassView
#include "PolyCoord.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	TOPLEFT		0
#define	TOPRIGHT	1
#define	BOTTOMLEFT	2
#define	BOTTOMRIGHT	3

#include "NormalizePage.h"
#include "ProbeBeamSectionsPage.h"
#include "ProbeAxialPlotPage.h"
#include "HRTimer.h"

#define	EXT_SIZE	0x1000
#define	INTERNAL_PRF	0x100
#define	INTERNAL_CLK	0x200
#define	EXTERNAL_0		0x001
#define _JOG_ROBOT		0x001
#define _JOG_TOWER		0x002

struct	ExternalEventData {
	int	nStructSize;

	double	dHRTime;
	int		nStatus;
};

class CMainThread  
{
public:
	bool HasIntelligentGotoFinished();
	void IntelligentGoto();
	void LeadscrewCompensation();
	void DepositTool(int nSide);
	void TurnTableScanKinematic();
	void RetrieveTool(int nSide);
	void Abort();
	bool IsScanning();
	void JogToPosition();
	int	m_nJogWhatToPosition;
	int	m_nJogRobotSide;
	int	m_nRoboAxisMask;
	int m_nScanFinishLine;
	int m_nPeakOppositeStatus;
	int m_nNormalizeStatus;

	void OriginRobo9Plus9();
	void PreWetRollerProbe();
	int m_nLoop;
	void MoveRobotToStartOfLine(int nScanLine);
	void MoveRobotWithBackOff();
	void RobotRollerProbeScan();
	bool m_bNormDisableDAC;
	void MoveToBladeStartPos();
	void BladeScan();
	int m_nBladeSide;
	bool IsJoystickRunning();
	int m_nNormPlotDirection;
	void MovetoStartWithoutDangerPlane();
	void MovetoPosWithoutDangerPlanePreFieldBus();
	void MovetoPosWithoutDangerPlaneFieldBus();
	void DoMarkerPen();
	void TipsRemoveAirRobot();
	void TipsRemoveAirCartesian();
	void PeakOppositeKinematic();
	void PeakOppositeRobot();
	void CheckThicknessGauge();
	void ThicknessCalibrationTwoPts();
	void MultiProbeScan();
	void RobotContourScan();
	void BladeGoHome();
	void BladeToDegrees();
	void MovetoStraightLineKinematic();
	int m_nLadderExecuteSide;
	int	m_nRepairAxis;
	int m_nScratch;
	int m_nIndex;
	void ExecuteRobotLadder();
	void ExecuteRectilinearLadder();
	void ExecuteLadder();
	int m_nLadder;
	CHRTimer	m_HRTimer;
	void PrfEvent();
	unsigned int	m_nExtPtr;
	ExternalEventData m_ExternalEvents[EXT_SIZE];
	HANDLE m_hSemaphore;

	void InterpolatePosForAngle(CCoord *pCpSurface, CCoord *pCp0, CCoord *pCp1);
	void RepairAxis();
	void CollectWaterPathData();
	void TrackAlongLine();
	void OriginXYZRThetaLeft();
	void InvalidateToolPath();
	void NormalizeViewInvalidate(int nDirection, CCoord Cp);
	int	m_nLastGainValue;			//Used in the turntable routine;
	bool m_bScanLineUpdateByMotorRevCounter;
	void MovetoViaTurntableDangerPlane();
	void TurnTableScan();
	void TurnTableScanPreFieldBus();
	void MovetoTurntableSafe();
	void CollectEccentricData();
	void CollectTrajectory();
	void EccentricTest();
	CProbe* m_pProbe;
	CProbeAxialPlotPage* m_pProbeAxialPlotPage;
	CNormalizePage*	m_pNormalizePage;
	CProbeBeamSectionsPage*	m_pProbeBeamSectionsPage;

	int m_nStartTimeNs;
	int m_nStopTimeNs;

	void RotaryScan();
	void MovetoViaDangerPlaneKinematic();
	void ProbeCrossSection();
	void ProbeAxialPlot();
	void RectilinearScan();
	void UnsafePrompt();
	int m_nThreadCounter;
	int IsSafetyCompromised(int nAction);
	void SegmentTurntableScan();
	bool m_bMoveToStartAtBeginningOfScan;
	void MoveToHighGain();
	void ContinuousTurntableScan();
	void Tracking();
	void MovetoViewViaDangerPlane();
	void MovetoViaDangerPlane();
	void MovetoContourStart();
	void TipsRemoveAir();
	int m_nTuningPtr;
	void CollectTunning();
	void TuningMove();
	int	m_nLineIncrement;
	int m_nScanStartLine;
	void MoveRoundBall();
	bool m_bEnableJoystick;
	D3DXMATRIXA16 m_matNormal;
	int	m_nNormSide;
	int m_nNormAxis0;
	int m_nNormAxis1;
	int m_nNormAction;
	int	m_nRevCounter;
	bool m_bMoveTurnTable;
	float m_fXtMove;
	float m_fYtMove;
	void CollectNormalizeData();
	bool CheckForHoldingAbort();
	bool m_bHoldingAbort;
	bool m_bPause;
	bool CMainThread::HasAnyTypeOfMoveToFinished();
	bool HasMovetoViaDangerPlaneFinished();
	bool HasMovetoStraightLineFinished();
	bool HasMovetoSafelyFinished();
	bool HasPeakOppositeStarted();
	bool HasPeakOppositeFinished();
	bool HasProbeCrossSectionFinished();
	bool HasWaterPathAdjustFinished();
	bool HasNormalizeStarted();
	bool HasNormalizeFinished();
	void OptimizeBrainCoordinate();
	void WaterPathAdjust();
	void PeakOpposite();
	void PeakOppositeXyThenZy();
	void PeakOppositeZyThenXy();
	void OriginHoldingFixtureMoveing();
	void OriginHoldingFixtureFixed();
	void ImmersionOriginAllRight();
	void ImmersionMovetoSafely();
	void ImmersionContourScan();
	void OriginXtYtRight();
	void FindZero();
	void FlatScan();
	void MovetoSurface();
	void MovetoSafely();
	void MovetoHoldingFixture();
	void OriginAllForeplane();
	void OriginSingleAxisHoldingFixture();
	void OriginSingleAxisHoldingFixtureSi6();
	void OriginSingleAxisHoldingFixtureSi10();
	void OriginHoldingFixture();
	bool m_bAbortFlag;
	void ContourScan();
	void ContourScanGKN();
	void ContourScanOld();
	void ContourScanRAxis(int nScanLineNumber=NULL);
	CCoord m_CpPoly[40];
	void Normalize();
	void NormalizeRotateThetaDiff();
	void NormalizeRotateTheta();
	void NormalizePPMAC5Axis();
	void NormalizeRobot();
	void NormalizePPMAC();
	void NormalizePitchYaw();
	void SingleTipAutoAlign();
	void MeasureWaterVelocity();
	void OriginSequential();
	void SingleAutoAlign();
	void OriginXYZRight();
	void OriginXYZLeft();
	void TestAllLines();
	void RepetetivePolyMoveto();
	void MovetoSafe();
	void MovetoHome();
	void StopCurrentThreadAction();
	void Origin();
	void Joystick();
	int m_nLastThreadAction;
	void DualPolyMoveto();
	void PolyMoveto();
	bool m_bCollectPmacTrajectory;
	bool m_bCollectTrajectory;
	CPolyCoord m_CpTrajectory;
	CPolyCoord m_CpTunning;
	CPolyCoord m_CpNormal;
	CPolyCoord m_ToolPath;
	CPolyCoord m_CpEccentric;

	CCoord m_CpStart;
	CCoord m_CpEccentricCentre;
	CCoord m_CpMoveTo;
	CCoord m_CpHead;
	CCoord m_CpSurface;
	CCoord m_CpDestination;
	CCoord m_CpRect[4];
	CCoord m_CpHeadNorm;
	CCoord m_CpSurfaceNorm;

	HoldingFixtureCoord m_CpHolding;
	void MovetoStraightLine();
	void MovetoStraightLineCartesian();
	void MoveJustR();
	void ServoThread();

	float m_fCurrentSpeed;
	int m_nMoveSide;
	int m_nDir;
	int	m_nDoorStatus;
	int	m_nGateStatus;
	int	m_nScanLine;
	int	m_nNumberScanLines;
	int	m_nToolIndexToRetrieveDeposit[2];
	int	m_nLeadscrewCompensationAxis;
	int	m_nDangerPlaneMask;

	int	m_nSafeThreadAction;
	int m_nDangerPlaneThreadAction;
	int m_nSubThreadAction;
	int m_nThreadAction;

	bool	m_bNewToolPathIfAnyoneWantsIt;

	int	m_nSafetyLoopCounter;
	int	m_nInPort;
	int m_nDataSamples;
	void MovetoViaDangerPlaneBridgeTank();
	void MovetoViaDangerPlaneDualSuirter();
	void MovetoViaDangerPlaneGKN();
	void MoveToPark();
	void MoveToParkForeplane();
	
	void LaserTaughtLineScan();
	int	m_nLaserScanLine;
	float m_fLaserScanIncrement;
	int m_nLaserScanLinesL;
	void CollectLaserTaughtLineData();
	int m_nOldTaughtLine;


	void SuspendThread();
	void StartThread();
	CMainThread();
	virtual ~CMainThread();

};

	UINT	MainThread(LPVOID pPAram);

#endif // !defined(AFX_MAINTHREAD_H__F56DEA48_C622_4C3C_8EE8_494BBE323D5E__INCLUDED_)
