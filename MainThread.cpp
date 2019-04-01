// MainThread.cpp: implementation of the CMainThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "MainThread.h"
#include "MainFrm.h"
#include "PromptDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainThread::CMainThread()
{
	m_nThreadAction = 0;
	m_nSubThreadAction = 0;
	m_nDangerPlaneThreadAction = 0;
	m_nSafeThreadAction = 0;
	m_nThreadCounter = 0;
	m_bHoldingAbort = FALSE;
	m_bEnableJoystick = TRUE;
	m_bMoveTurnTable=false;
	m_bMoveToStartAtBeginningOfScan = TRUE;
	m_CpEccentricCentre.Zero();
	m_bScanLineUpdateByMotorRevCounter=false;
	m_bCollectPmacTrajectory = false;
	m_bCollectTrajectory = false;
	m_nLadderExecuteSide = 2;
	m_nBladeSide = 0;
	m_bNormDisableDAC = false;
	m_nPeakOppositeStatus = 0;
	m_nNormalizeStatus = 0;

	m_HRTimer.StartTimer();
	m_nExtPtr = 0;
	ZeroMemory(m_ExternalEvents, sizeof m_ExternalEvents);

	m_hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);  //Set Semaphore count to 1
}

CMainThread::~CMainThread()
{
	m_HRTimer.StopTimer();
}

void CMainThread::StartThread()
{
	theApp.StartThread(L"Main thread", &MainThread, this, 50, THREAD_PRIORITY_NORMAL);
}

void CMainThread::SuspendThread()
{
	theApp.SuspendThread(this);
}




UINT MainThread(LPVOID pParam)
{
	CMainThread* pParent = (CMainThread *)pParam;

	if (WaitForSingleObject(pParent->m_hSemaphore, 10) == WAIT_OBJECT_0) {	//wait for Semaphore count == 1 then decrement it

		if(theApp.m_Si10User.IsConnected() == TRUE)
			theApp.m_Si10User.CheckHoldingBrake();

		pParent->ServoThread();

		ReleaseSemaphore(pParent->m_hSemaphore, 1, NULL);	//Increment the semaphore count
	}

	return THREAD_CONTINUE;
}

void CMainThread::ServoThread()
{
	m_nThreadCounter++;

	if(m_nThreadAction>0)
		int gg=99;

	switch(m_nThreadAction) {
	case 0:
		//This can be set to zero in MainFrm::OnTimer();
		m_nSubThreadAction=0;
		break;
	case WAIT_FOR_MOVE_FINISHED:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_bScanning=FALSE;
			if(m_bEnableJoystick==TRUE) {
				m_nThreadAction=JOYSTICK_START;
				m_nSubThreadAction=0;
			} else {
				m_nThreadAction=0;
				m_nSubThreadAction=0;
			}
		};
		m_nLastThreadAction=m_nThreadAction;
		break;
	case JOYSTICK_START:
		if(IsSafetyCompromised(m_nThreadAction) == 0) Joystick();
		m_nLastThreadAction=m_nThreadAction;
		break;
	case MOVETO_STRAITE_LINE:
		if(IsSafetyCompromised(m_nThreadAction) == 0) MovetoStraightLine();
		m_nLastThreadAction=m_nThreadAction;
		break;
	case MOVE_JUST_R:
		MoveJustR();
		m_nLastThreadAction=m_nThreadAction;
		break;
	case MOVETO_CONTOUR_START:
		if(IsSafetyCompromised(m_nThreadAction) == 0) MovetoContourStart();
		m_nLastThreadAction=m_nThreadAction;
		break;
	case MOVETO_VIA_DANGER_PLANE:
		if(IsSafetyCompromised(m_nThreadAction) == 0) MovetoViaDangerPlane();
		m_nLastThreadAction=m_nThreadAction;
		break;
	case MOVETO_SURFACE:
		if(IsSafetyCompromised(m_nThreadAction) == 0) MovetoSurface();
		m_nLastThreadAction=m_nThreadAction;
		break;
	case MOVE_ROUND_BALL:
		if(IsSafetyCompromised(m_nThreadAction) == 0) MoveRoundBall();
		m_nLastThreadAction=m_nThreadAction;
		break;
	case POLY_MOVE:
		if(IsSafetyCompromised(m_nThreadAction) == 0) PolyMoveto();
		m_nLastThreadAction=m_nThreadAction;
		break;
	case REPETETIVE_POLY_MOVE:
		if(IsSafetyCompromised(m_nThreadAction) == 0) RepetetivePolyMoveto();
		m_nLastThreadAction=m_nThreadAction;
		break;
	case TEST_ALL_LINES:
		if(IsSafetyCompromised(m_nThreadAction) == 0) TestAllLines();
		m_nLastThreadAction=m_nThreadAction;
		break;
	case DUAL_POLY_MOVE:
		if(IsSafetyCompromised(m_nThreadAction) == 0) DualPolyMoveto();
		m_nLastThreadAction=m_nThreadAction;
		break;
	case ORIGIN_ALL:
	case ORIGIN_ALL_LEFT:
	case ORIGIN_ALL_RIGHT:
	case ORIGIN_X_LEFT:
	case ORIGIN_Y_LEFT:
	case ORIGIN_Z_LEFT:
	case ORIGIN_XT_LEFT:
	case ORIGIN_YT_LEFT:
	case ORIGIN_X_RIGHT:
	case ORIGIN_Y_RIGHT:
	case ORIGIN_Z_RIGHT:
	case ORIGIN_XT_RIGHT:
	case ORIGIN_YT_RIGHT:
	case ORIGIN_X_BRIDGE:
	case ORIGIN_R_LEFT:
		if(IsSafetyCompromised(m_nThreadAction) == 0) Origin();
		break;
	case ORIGIN_XYZ_LEFT:
		if(IsSafetyCompromised(m_nThreadAction) == 0) OriginXYZLeft();
		break;
	case ORIGIN_XYZ_RIGHT:
		if(IsSafetyCompromised(m_nThreadAction) == 0) OriginXYZRight();
		break;
	case ORIGIN_SEQUENTIAL:
		if(IsSafetyCompromised(m_nThreadAction) == 0) OriginSequential();
		break;
	case ORIGIN_XTYT_RIGHT:
		if(IsSafetyCompromised(m_nThreadAction) == 0) OriginXtYtRight();
		break;
	case ORIGIN_ALL_FOREPLANE:
		if(IsSafetyCompromised(m_nThreadAction) == 0) OriginAllForeplane();
		break;
	case ORIGIN_X_MOVING:
	case ORIGIN_Y_MOVING:
	case ORIGIN_Z_MOVING:
	case ORIGIN_X_FIXED:
	case ORIGIN_Y_FIXED:
	case ORIGIN_Z_FIXED:
		if(IsSafetyCompromised(m_nThreadAction) == 0) OriginSingleAxisHoldingFixture();
		break;
	case ORIGIN_X_Y_Z_R_THETA_LEFT:
		if(IsSafetyCompromised(m_nThreadAction) == 0) OriginXYZRThetaLeft();
		break;
	case MOVETO_SAFE:
		if(IsSafetyCompromised(m_nThreadAction) == 0) MovetoSafe();
		break;
	case SINGLE_AUTO_ALIGN:
		if(IsSafetyCompromised(m_nThreadAction) == 0) SingleAutoAlign();
		break;
	case MEASURE_WATER_VELOCITY:
		if(IsSafetyCompromised(m_nThreadAction) == 0) MeasureWaterVelocity();
		break;
	case SINGLE_TIP_AUTO_ALIGN:
		if(IsSafetyCompromised(m_nThreadAction) == 0) SingleTipAutoAlign();
		break;
	case NORMALIZE:
		if(IsSafetyCompromised(m_nThreadAction) == 0) Normalize();
		break;
	case CONTOUR_SCAN:
		if(IsSafetyCompromised(m_nThreadAction) == 0) ContourScan();
		break;
	case ORIGIN_HOLDING_FIXTURE:
		if(IsSafetyCompromised(m_nThreadAction) == 0) OriginHoldingFixture();
		break;
	case ORIGIN_HOLDING_FIXTURE_FIXED:
		if(IsSafetyCompromised(m_nThreadAction) == 0) OriginHoldingFixtureFixed();
		break;
	case ORIGIN_HOLDING_FIXTURE_MOVEING:
		if(IsSafetyCompromised(m_nThreadAction) == 0) OriginHoldingFixtureMoveing();
		break;
	case MOVETO_HOLDING_FIXTURE:
		if(IsSafetyCompromised(m_nThreadAction) == 0) MovetoHoldingFixture();
		break;
	case MOVETO_SAFELY:
		if(IsSafetyCompromised(m_nThreadAction) == 0) MovetoSafely();
		break;
	case FLAT_SCAN:
		if(IsSafetyCompromised(m_nThreadAction) == 0) FlatScan();
		break;
	case BLADE_SCAN:
		if(IsSafetyCompromised(m_nThreadAction) == 0) BladeScan();
		break;
	case FIND_ZERO:
		if(IsSafetyCompromised(m_nThreadAction) == 0) FindZero();
		break;
	case PEAK_OPPOSITE:
		if(IsSafetyCompromised(m_nThreadAction) == 0) PeakOpposite();
		break;
	case TUNING_MOVE:
		if(IsSafetyCompromised(m_nThreadAction) == 0) TuningMove();
		break;
	case TIPS_REMOVE_AIR:
		if(IsSafetyCompromised(m_nThreadAction) == 0) TipsRemoveAir();
		break;
	case REPAIR_AXIS:
		if(IsSafetyCompromised(m_nThreadAction) == 0) RepairAxis();
		break;
	case TURNTABLE_SCAN:
		if(IsSafetyCompromised(m_nThreadAction) == 0) TurnTableScan();
		break;
	case CONTINUOUS_TURNTABLE_SCAN:
		if(IsSafetyCompromised(m_nThreadAction) == 0) ContinuousTurntableScan();
		break;
	case SEGMENT_TURNTABLE_SCAN:
		if(IsSafetyCompromised(m_nThreadAction) == 0) SegmentTurntableScan();
		break;
	case MOVETO_HIGH_GAIN:
		if(IsSafetyCompromised(m_nThreadAction) == 0) MoveToHighGain();
		break;
	case MOVETO_PARK:
		switch(theApp.m_nJobNumber) {
			case FOREPLANE:
				if(IsSafetyCompromised(m_nThreadAction) == 0) MoveToParkForeplane();
				break;
			default:
				if(IsSafetyCompromised(m_nThreadAction) == 0) MoveToPark();
				break;
		}
		break;
	case PROBE_CROSS_SECTION:
		ProbeCrossSection();
		break;
	case PROBE_AXIAL_PLOT:
		ProbeAxialPlot();
		break;
	case ECCENTRIC_TEST:
		EccentricTest();
		break;
	case MOVETO_TURNTABLE_SAFE:
		MovetoTurntableSafe();
		break;
	case TRACK_ALONG_LINE:
		TrackAlongLine();
		break;
	case LASER_TAUGHT_LINE_SCAN:
		LaserTaughtLineScan();
		break;
	case EXECUTE_LADDER:
		if(IsSafetyCompromised(m_nThreadAction) == 0) ExecuteLadder();
		break;
	case BLADE_TO_DEGREES:
		if(IsSafetyCompromised(m_nThreadAction) == 0) BladeToDegrees();
		break;
	case BLADE_GO_HOME:
		if(IsSafetyCompromised(m_nThreadAction) == 0) BladeGoHome();
		break;
	case ROBOT_CONTOUR_SCAN:
		if(IsSafetyCompromised(m_nThreadAction) == 0) RobotContourScan();
		break;
	case MULTI_PROBE_SCAN:	if(IsSafetyCompromised(m_nThreadAction) == 0) 	MultiProbeScan();
		break;
	case THICKNESS_CALIBRATION_TWO_PT:
		ThicknessCalibrationTwoPts();
		break;
	case THICKNESS_CALIBRATION_All_PTS:
		break;
	case CHECK_THICKNESS_GAUGE: CheckThicknessGauge();
		break;
	case DO_MARKER_PEN: DoMarkerPen();
		break;
	case ROBOT_ROLLER_PROBE_SCAN: RobotRollerProbeScan();
		break;
	case MOVE_ROBOT_WITH_BACKOFF: MoveRobotWithBackOff();
		break;
	case PRE_WET_ROLLER_PROBE: PreWetRollerProbe();
		break;
	case ORIGIN_ALL_ROBO_9_PLUS_9: 
		if(IsSafetyCompromised(m_nThreadAction) == 0) OriginRobo9Plus9();
		break;
	case JOG_TO_POSITION: 
		if(IsSafetyCompromised(m_nThreadAction) == 0) JogToPosition();
		break;
	case DEPOSIT_TOOL_0:	if(IsSafetyCompromised(m_nThreadAction) == 0) DepositTool(0);
		break;
	case DEPOSIT_TOOL_1:	if(IsSafetyCompromised(m_nThreadAction) == 0) DepositTool(1);
		break;
	case RETRIEVE_TOOL_0:	if(IsSafetyCompromised(m_nThreadAction) == 0) RetrieveTool(0);
		break;
	case RETRIEVE_TOOL_1:	if(IsSafetyCompromised(m_nThreadAction) == 0) RetrieveTool(1);
		break;
	case LEADSCREW_COMPENSATION:	if(IsSafetyCompromised(m_nThreadAction) == 0) LeadscrewCompensation();
		break;
	case INTELLIGENT_GOTO:	IntelligentGoto();
		break;
	}
}

void CMainThread::MovetoStraightLine()
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		MovetoStraightLineCartesian();
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TANK_5AXIS:
	case TANK_6AXIS:
	case TWIN_TOWER_KINEMATIC:
		MovetoStraightLineKinematic();
		break;
	case TRACKED_PROBE:
		break;
	}
}

void CMainThread::MovetoStraightLineCartesian()
{
	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_Motors.SuspendMotors();
		if( (theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable!=TRUE) || (theApp.m_nJobNumber == GKN ) ){ //Don't want to move rotate on straight line move for GKN
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		} else {
			theApp.m_Motors.EnableAxes(BOTH_MOVETO_WITH_R);
		}
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		m_nSubThreadAction=10;
		Sleep(100);
		break;
	case 10:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 20;
		};
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_bCollectTrajectory=false;
			m_nThreadAction=0;
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			m_nSubThreadAction=0;
		}
		break;
	}


}


void CMainThread::MoveJustR()
{

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.EnableAxes(MOVE_JUST_R);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveJustR(DONT_WAIT);
		m_nSubThreadAction=10;
	case 10:
		if(theApp.m_Motors.IsMoveRFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			m_nSubThreadAction=0;
		}
		break;
	}


}


void CMainThread::PolyMoveto()
{
	switch(m_nSubThreadAction) {
	case 0:
		m_bAbortFlag = false;
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.EnableAxes(BOTH_MOVETO_WITH_R);
		m_nLoop=0;
		m_nSubThreadAction=10;
		break;
	case 10:
		m_ToolPath.GetCoord(m_nLoop,&m_CpMoveTo);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		Sleep(400);
		m_nSubThreadAction = 20;
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nLoop++;
			if((m_bAbortFlag == false) && (m_nLoop<m_ToolPath.GetSize())) {
				m_nSubThreadAction = 10;
			} else {
				m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
				m_nSubThreadAction = 0;
			}
		}
		break;
	}
}

void CMainThread::RepetetivePolyMoveto()
{
	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_ServoUser.SuspendJoystick(5);
		theApp.m_ServoUser.DownLoadPolyMoveCoordinates(&m_ToolPath, m_nDir=0);
		if(theApp.m_nMotorType==PMAC_TURBO) theApp.m_ServoUser.SetupLookAhead();
//		theApp.m_ServoUser.CollectPositions();
		theApp.m_ServoUser.StartPolyMoveWithLookAhead(m_ToolPath.m_nCoordL);
		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
//			theApp.m_ServoUser.StopCollectingPositions();

			m_nDir ^= 1;
			theApp.m_ServoUser.DownLoadPolyMoveCoordinates(&m_ToolPath, m_nDir);
			if(theApp.m_nMotorType==PMAC_TURBO) theApp.m_ServoUser.SetupLookAhead();
//			theApp.m_ServoUser.CollectPositions();
			theApp.m_ServoUser.StartPolyMoveWithLookAhead(m_ToolPath.m_nCoordL);
		}
		break;
	}

}

void CMainThread::DualPolyMoveto()
{
	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_ServoUser.SuspendJoystick(4);
		theApp.m_ServoUser.DownLoadPolyMoveCoordinates(&m_ToolPath, 0);
//		if(theApp.m_nMotorType==PMAC_TURBO) theApp.m_ServoUser.SetupDualLookAhead();
//		theApp.m_ServoUser.CollectPositions();
//		theApp.m_ServoUser.StartDualPolyMoveWithLookAhead(m_ToolPath.m_nCoordL);
		m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		break;
	}

}

void CMainThread::Joystick()
{
	switch(m_nSubThreadAction) {
	case 0:
		if(IsSafetyCompromised(m_nThreadAction) == 0) {

			if(theApp.m_PmacUser.m_bDriverOpen == TRUE) {
				theApp.m_PmacUser.StartJoystick();
			};
			if(theApp.m_Si10User.IsConnected()==TRUE) {
				theApp.m_Si10User.EnableJoystick();
			} else {
				theApp.m_Si6User.EnableJoystick();
			}
			theApp.m_FBCtrl.EnableJoystick();

			m_nSubThreadAction=10;
		}
		break;
	case 10:
		switch(theApp.m_nMotorType) {
		default: theApp.m_PmacUser.PLCJoystickServiceRoutine(IsSafetyCompromised(m_nThreadAction));
			break;
		case SI6:
			break;
		case SI10:
			break;
		case FIELD_BUS:	theApp.m_FBCtrl.Joystick();
			break;
		}
		break;
	}
}

void CMainThread::Origin()
{
	switch(m_nSubThreadAction) {
	case 0:
		switch(m_nThreadAction) {
		case ORIGIN_ALL_LEFT:
		case ORIGIN_ALL_RIGHT:
		case ORIGIN_ALL:theApp.m_Motors.OriginAll(m_nThreadAction);
			theApp.ActivityLog(L"Origin all");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_X_BRIDGE: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nXBridge);
			theApp.ActivityLog(L"Origin bridge");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_X_LEFT: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nXLeft);
			theApp.ActivityLog(L"Origin X left");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_Y_LEFT: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nYLeft);
			theApp.ActivityLog(L"Origin Y left");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_Z_LEFT: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nZLeft);
			theApp.ActivityLog(L"Origin Z left");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_XT_LEFT: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nXtLeft);
			theApp.ActivityLog(L"Origin Xt left");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_YT_LEFT: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nYtLeft);
			theApp.ActivityLog(L"Origin Yt left");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_R_LEFT: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nRLeft);
			theApp.ActivityLog(L"Origin R left");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_X_RIGHT: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nXRight);
			theApp.ActivityLog(L"Origin X right");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_Y_RIGHT: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nYRight);
			theApp.ActivityLog(L"Origin Y right");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_Z_RIGHT: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nZRight);
			theApp.ActivityLog(L"Origin Z right");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_XT_RIGHT: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nXtRight);
			theApp.ActivityLog(L"Origin Xt right");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_YT_RIGHT: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nYtRight);
			theApp.ActivityLog(L"Origin Yt right");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_X_MOVING: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nXStep);
			theApp.ActivityLog(L"Origin X step");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_Y_MOVING: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nYStep);
			theApp.ActivityLog(L"Origin Y step");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_Z_MOVING: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nZStep);
			theApp.ActivityLog(L"Origin Z step");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_X_FIXED: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nRStep);
			theApp.ActivityLog(L"Origin X step");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_Y_FIXED: theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nXtStep);
			theApp.ActivityLog(L"Origin Y step");
			m_nSubThreadAction=999;
			break;
		case ORIGIN_Z_FIXED: theApp.m_Si6User.EnableAxis(theApp.m_Tank.nZStep,HOMETO);
			theApp.ActivityLog(L"Origin Z step");
			m_nSubThreadAction=999;
			break;
		};
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			
			switch(m_nThreadAction) {
				case ORIGIN_ALL_LEFT:
					theApp.m_PmacUser.HomeAxisWithoutMoving(21);
					theApp.m_PmacUser.HomeAxisWithoutMoving(22);
					theApp.m_PmacUser.HomeAxisWithoutMoving(23);
					theApp.m_PmacUser.HomeAxisWithoutMoving(24);
					theApp.m_PmacUser.HomeAxisWithoutMoving(25);
				case ORIGIN_ALL_RIGHT:
					theApp.m_PmacUser.HomeAxisWithoutMoving(26);
					theApp.m_PmacUser.HomeAxisWithoutMoving(27);
					theApp.m_PmacUser.HomeAxisWithoutMoving(28);
					theApp.m_PmacUser.HomeAxisWithoutMoving(29);
					theApp.m_PmacUser.HomeAxisWithoutMoving(30);
				case ORIGIN_ALL:
					theApp.m_PmacUser.HomeAxisWithoutMoving(21);
					theApp.m_PmacUser.HomeAxisWithoutMoving(22);
					theApp.m_PmacUser.HomeAxisWithoutMoving(23);
					theApp.m_PmacUser.HomeAxisWithoutMoving(24);
					theApp.m_PmacUser.HomeAxisWithoutMoving(25);
					theApp.m_PmacUser.HomeAxisWithoutMoving(26);
					theApp.m_PmacUser.HomeAxisWithoutMoving(27);
					theApp.m_PmacUser.HomeAxisWithoutMoving(28);
					theApp.m_PmacUser.HomeAxisWithoutMoving(29);
					theApp.m_PmacUser.HomeAxisWithoutMoving(30);
					break;
				case ORIGIN_X_LEFT:	theApp.m_PmacUser.HomeAxisWithoutMoving(21);
					break;
				case ORIGIN_Y_LEFT:	theApp.m_PmacUser.HomeAxisWithoutMoving(22);
					break;
				case ORIGIN_Z_LEFT:	theApp.m_PmacUser.HomeAxisWithoutMoving(23);
					break;
				case ORIGIN_XT_LEFT:	theApp.m_PmacUser.HomeAxisWithoutMoving(24);
					break;
				case ORIGIN_YT_LEFT:	theApp.m_PmacUser.HomeAxisWithoutMoving(25);
					break;
				case ORIGIN_X_RIGHT:	theApp.m_PmacUser.HomeAxisWithoutMoving(26);
					break;
				case ORIGIN_Y_RIGHT:	theApp.m_PmacUser.HomeAxisWithoutMoving(27);
					break;
				case ORIGIN_Z_RIGHT:	theApp.m_PmacUser.HomeAxisWithoutMoving(28);
					break;
				case ORIGIN_XT_RIGHT:	theApp.m_PmacUser.HomeAxisWithoutMoving(29);
					break;
				case ORIGIN_YT_RIGHT:	theApp.m_PmacUser.HomeAxisWithoutMoving(30);
					break;
			}
			
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			m_nSubThreadAction=0;
			theApp.ActivityLog(L"Completed");
		}
		break;
	}
}

void CMainThread::StopCurrentThreadAction()
{
	int nLoop = 200;
	m_nThreadAction = 0;
	while((m_nSubThreadAction != 0) && (nLoop>0)) {
		Sleep(1);
		nLoop--;
	};
	m_nSafeThreadAction = 0;
	m_nDangerPlaneThreadAction = 0;
	m_nPeakOppositeStatus = 0;
	m_nNormalizeStatus = 0;

}

void CMainThread::MovetoSafe()
{
	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_ServoUser.SuspendJoystick(4);
		theApp.m_ServoUser.GetHeadPos(&m_CpMoveTo);
		switch(theApp.m_Tank.nScannerDescription) {
		case DUAL_SIDED_7_AXES:
		case DUAL_SIDED_SQUIRTER:
		case SINGLE_BRIDGE_DUAL_SIDED:
		case DUAL_SIDED_SQUIRTER_2:
			if(theApp.m_LastSettings.nManipulatorType[0]==VERTICAL_WRIST)
				return; //Placed her for terma when using vertical extension with immersion roll in tank to stop accidentally driving into side.
			m_CpMoveTo.Side0.fY=(float)theApp.m_Axes[Y0AXIS].nOffset * theApp.m_Axes[Y0AXIS].fStepSize * -1.0f;
			m_CpMoveTo.Side1.fY=(float)theApp.m_Axes[Y1AXIS].nOffset * theApp.m_Axes[Y1AXIS].fStepSize * -1.0f;
			m_CpMoveTo.Side0.fXt=0.0;
			m_CpMoveTo.Side1.fXt=0.0;
			m_CpMoveTo.Side0.fYt=0.0;
			m_CpMoveTo.Side1.fYt=0.0;
			break;
		case TRACKED_PROBE:
			break;
		default:
			m_CpMoveTo.Side0.fZ=(float)theApp.m_Axes[Z0AXIS].nOffset * theApp.m_Axes[Z0AXIS].fStepSize * -1.0f;
			m_CpMoveTo.Side1.fZ=(float)theApp.m_Axes[Z1AXIS].nOffset * theApp.m_Axes[Z1AXIS].fStepSize * -1.0f;
			m_CpMoveTo.Side0.fXt=0.0;
			m_CpMoveTo.Side1.fXt=0.0;
			m_CpMoveTo.Side0.fYt=0.0;
			m_CpMoveTo.Side1.fYt=0.0;
			break;
		}
		theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
		theApp.m_ServoUser.MoveTo(DONT_WAIT);
		m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		break;
	}

}

void CMainThread::MovetoHome()
{
	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_ServoUser.SuspendJoystick(4);
		theApp.m_ServoUser.GetHeadPos(&m_CpMoveTo);
		m_CpMoveTo.Side0.fY=(float)theApp.m_Axes[Y0AXIS].nOffset * theApp.m_Axes[Y0AXIS].fStepSize * -1.0f;
		m_CpMoveTo.Side1.fY=(float)theApp.m_Axes[Y1AXIS].nOffset * theApp.m_Axes[Y1AXIS].fStepSize * -1.0f;
		m_CpMoveTo.Side0.fXt=0.0;
		m_CpMoveTo.Side1.fXt=0.0;
		m_CpMoveTo.Side0.fYt=0.0;
		m_CpMoveTo.Side1.fYt=0.0;

		theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
		theApp.m_ServoUser.MoveTo(DONT_WAIT);
		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.GetHeadPos(&m_CpMoveTo);
			m_CpMoveTo.Side0.fX=(float)theApp.m_Axes[X0AXIS].nOffset * theApp.m_Axes[X0AXIS].fStepSize * -1.0f;
			m_CpMoveTo.Side1.fX=(float)theApp.m_Axes[X1AXIS].nOffset * theApp.m_Axes[X1AXIS].fStepSize * -1.0f;
			m_CpMoveTo.Side0.fY=(float)theApp.m_Axes[Y0AXIS].nOffset * theApp.m_Axes[Y0AXIS].fStepSize * -1.0f;
			m_CpMoveTo.Side1.fY=(float)theApp.m_Axes[Y1AXIS].nOffset * theApp.m_Axes[Y1AXIS].fStepSize * -1.0f;
			m_CpMoveTo.Side0.fZ=(float)theApp.m_Axes[Z0AXIS].nOffset * theApp.m_Axes[Z0AXIS].fStepSize * -1.0f;
			m_CpMoveTo.Side1.fZ=(float)theApp.m_Axes[Z1AXIS].nOffset * theApp.m_Axes[Z1AXIS].fStepSize * -1.0f;
			m_CpMoveTo.Side0.fXt=0.0;
			m_CpMoveTo.Side1.fXt=0.0;
			m_CpMoveTo.Side0.fYt=0.0;
			m_CpMoveTo.Side1.fYt=0.0;

			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);

			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}

}


void CMainThread::TestAllLines()
{


}

void CMainThread::OriginAllForeplane()
{
	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nZLeft);
		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nXLeft);
			m_nSubThreadAction=20;
		}
		break;
	case 20:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_Motors.OriginFindAxis(theApp.m_Tank.nYLeft);
			m_nSubThreadAction=30;
		}
		break;
	case 30:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}

}


void CMainThread::OriginXYZLeft()
{
/*
	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_ServoUser.OriginFindAxis(Y0AXIS);
		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(Z0AXIS);
			m_nSubThreadAction=20;
		}
		break;
	case 20:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(X0AXIS);
			m_nSubThreadAction=30;
		}
		break;
	case 30:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}
*/
}

void CMainThread::OriginXYZRight()
{
/*
	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_ServoUser.OriginFindAxis(Y1AXIS);
		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(Z1AXIS);
			m_nSubThreadAction=20;
		}
		break;
	case 20:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(X1AXIS);
			m_nSubThreadAction=30;
		}
		break;
	case 30:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}
*/
}


void CMainThread::OriginXtYtRight()
{
/*
	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_ServoUser.OriginFindAxis(XT1AXIS);
		m_nSubThreadAction=10;
		Sleep(20);
		break;
	case 10:
		if(theApp.m_ServoUser.IsOriginFinished(XT1AXIS) == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(YT1AXIS);
			m_nSubThreadAction=100;
			Sleep(20);
		}
		break;
	case 100:
		if(theApp.m_ServoUser.IsOriginFinished(YT1AXIS) == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			Sleep(20);
		}
		break;
	}
*/
}


void CMainThread::OriginSequential()
{
/*
	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_ServoUser.OriginFindAxis(Y0AXIS);
		m_nSubThreadAction=10;
		Sleep(20);
		break;
	case 10:
		if(theApp.m_ServoUser.IsOriginFinished(Y0AXIS) == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(Z0AXIS);
			m_nSubThreadAction=20;
			Sleep(20);
		}
		break;
	case 20:
		if(theApp.m_ServoUser.IsOriginFinished(Z0AXIS) == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(X0AXIS);
			m_nSubThreadAction=30;
			Sleep(20);
		}
		break;
	case 30:
		if(theApp.m_ServoUser.IsOriginFinished(X0AXIS) == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(Y1AXIS);
			Sleep(20);
		}
		break;
	case 40:
		if(theApp.m_ServoUser.IsOriginFinished(Y1AXIS) == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(Z1AXIS);
			m_nSubThreadAction=50;
			Sleep(20);
		}
		break;
	case 50:
		if(theApp.m_ServoUser.IsOriginFinished(Z1AXIS) == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(X1AXIS);
			m_nSubThreadAction=60;
			Sleep(20);
		}
		break;
	case 60:
		if(theApp.m_ServoUser.IsOriginFinished(X1AXIS) == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(XT0AXIS);
			m_nSubThreadAction=70;
			Sleep(20);
		}
		break;
	case 70:
		if(theApp.m_ServoUser.IsOriginFinished(XT0AXIS) == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(YT0AXIS);
			m_nSubThreadAction=80;
			Sleep(20);
		}
		break;
	case 80:
		if(theApp.m_ServoUser.IsOriginFinished(YT0AXIS) == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(XT1AXIS);
			m_nSubThreadAction=90;
			Sleep(20);
		}
		break;
	case 90:
		if(theApp.m_ServoUser.IsOriginFinished(XT1AXIS) == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(YT1AXIS);
			m_nSubThreadAction=100;
			Sleep(20);
		}
		break;
	case 100:
		if(theApp.m_ServoUser.IsOriginFinished(YT1AXIS) == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			Sleep(20);
		}
		break;
	}
*/
}

#define	AUTO_SIZE	12
void CMainThread::SingleAutoAlign()
{
	static int	nStart,nFinish;
	int	nNumberSamples = 200;
	int	nMaxPos = 0,nAxisPos;
	float fPos;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	static int nLoop;
	int	nStartTimeNs,nStopTimeNs;
	bool bPulseEcho;
	int	nTransmitter;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_ServoUser.SuspendJoystick(4);

		m_CpMoveTo.Side1.fZ-=(AUTO_SIZE/2);
		theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
		theApp.m_ServoUser.MoveTo(DONT_WAIT);
		nLoop=0;
		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			nStart=(int)(m_CpMoveTo.Side1.fZ / theApp.m_Axes[Z1AXIS].fStepSize);
			nFinish= nStart + (int)(AUTO_SIZE / theApp.m_Axes[Z1AXIS].fStepSize);
//			theApp.m_ServoUser.ClearArray(0);
//			theApp.m_ServoUser.CollectData(Z1AXIS,nStart,nFinish,nNumberSamples,1,0,TRUE,1);
			m_CpMoveTo.Side1.fZ+=AUTO_SIZE;
			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
			m_nSubThreadAction=20;
		}
		break;
	case 20:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.StopCollectingData();
//			if(pFrame->m_pRhomboidCompenstionDlg) {
//				pFrame->m_pRhomboidCompenstionDlg->TransferData(Z1AXIS,nNumberSamples);
//			}
//			pFrame->SendMessage(UI_REFRESH_RHOMBOID_VIEW);
	//		theApp.m_ServoUser.FindPeakAmplitude(nNumberSamples,&nMaxPos);
			nAxisPos = MulDiv(nMaxPos,nFinish-nStart,nNumberSamples) + nStart;
			fPos = (float)nAxisPos * theApp.m_Axes[Z1AXIS].fStepSize;

			m_CpMoveTo.Side1.fX-=(AUTO_SIZE/2);
			m_CpMoveTo.Side1.fZ = fPos;

			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
			m_nSubThreadAction=30;
		}
		break;
	case 30:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			nStart=(int)(m_CpMoveTo.Side1.fX / theApp.m_Axes[X1AXIS].fStepSize);
			nFinish= nStart + (int)(AUTO_SIZE / theApp.m_Axes[X1AXIS].fStepSize);
//			theApp.m_ServoUser.ClearArray(0);
//			theApp.m_ServoUser.CollectData(X1AXIS,nStart,nFinish,nNumberSamples,1,0,TRUE,1);
			m_CpMoveTo.Side1.fX+=AUTO_SIZE;
			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
			m_nSubThreadAction=40;
		}
		break;
	case 40:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.StopCollectingData();
//			if(pFrame->m_pRhomboidCompenstionDlg) {
//				pFrame->m_pRhomboidCompenstionDlg->TransferData(X1AXIS,nNumberSamples);
//			}
//			pFrame->SendMessage(UI_REFRESH_RHOMBOID_VIEW);
//			theApp.m_ServoUser.FindPeakAmplitude(nNumberSamples,&nMaxPos);
			nAxisPos = MulDiv(nMaxPos,nFinish-nStart,nNumberSamples) + nStart;
			fPos = (float)nAxisPos * theApp.m_Axes[X1AXIS].fStepSize;

			m_CpMoveTo.Side1.fX = fPos;

			if(nLoop==0) {
				m_CpMoveTo.Side1.fZ-=(AUTO_SIZE/2);
				m_nSubThreadAction=10;
				nLoop++;
			} else {
				m_nSubThreadAction=50;
			}
			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
		}
		break;
	case 50:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_UtUser.Dsp200GetStartStopTimeNs(&nStartTimeNs,&nStopTimeNs,&bPulseEcho,&nTransmitter);
			float fPath= ((float)nStartTimeNs*(float)theApp.m_LastSettings.nWaterVelocity) / 1e6f;
			float dWaterPathError= fPath - (theApp.m_Kinematics.m_fDesiredWaterPath[PORTSIDE] + theApp.m_Kinematics.m_fDesiredWaterPath[STARBOARD]);
			if(fabs(dWaterPathError)<=10.0) {
				m_CpMoveTo.Side1.fY -= (float)dWaterPathError;
				theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
				theApp.m_ServoUser.MoveTo(DONT_WAIT);
				m_nSubThreadAction=60;
			} else {
				m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			}
		}
		break;
	case 60:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}



}

void CMainThread::MeasureWaterVelocity()
{
	CCoord CpHead,CpSurface;
	static	CCoord	CpStart,CpFinish;
	float fX,fY,fZ,fDistance;
	int nSide = theApp.m_LastSettings.nWaterVelocitySide;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.GetHeadPos(&CpStart);

		theApp.m_Motors.GetSurfacePos(&CpSurface);
		CpSurface.Side[nSide].fX += (theApp.m_LastSettings.fWaterVelocityDistance * CpSurface.Side[nSide].fI);
		CpSurface.Side[nSide].fY += (theApp.m_LastSettings.fWaterVelocityDistance * CpSurface.Side[nSide].fJ);
		CpSurface.Side[nSide].fZ += (theApp.m_LastSettings.fWaterVelocityDistance * CpSurface.Side[nSide].fK);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);

		m_nStartTimeNs=theApp.m_UtUser.GetGateWaterTimeNs(theApp.m_LastSettings.nWaterVelocityTS);

		m_CpMoveTo = CpHead;

		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		Sleep(1000);
		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			Sleep(1000);
			theApp.m_Motors.GetHeadPos(&CpFinish);

			fX = CpStart.Side[nSide].fX - CpFinish.Side[nSide].fX;
			fY = CpStart.Side[nSide].fY - CpFinish.Side[nSide].fY;
			fZ = CpStart.Side[nSide].fZ - CpFinish.Side[nSide].fZ;

			fDistance =  (float)sqrt((double)(fX*fX + fY*fY + fZ*fZ));
			if(theApp.m_UtUser.m_TS[theApp.m_LastSettings.nWaterVelocityTS].Pr30.nMuxTx == theApp.m_UtUser.m_TS[theApp.m_LastSettings.nWaterVelocityTS].Pr30.nMuxRx) {
				fDistance *= 2.0f;
			}

			m_nStopTimeNs=theApp.m_UtUser.GetGateWaterTimeNs(theApp.m_LastSettings.nWaterVelocityTS);
			theApp.m_LastSettings.nWaterVelocity = (int)((fDistance / (float)(m_nStopTimeNs-m_nStartTimeNs)) * 1e6);
			theApp.m_LastSettings.nWaterVelocity = abs(theApp.m_LastSettings.nWaterVelocity);

			theApp.m_Motors.DesirePosEqualsCoord(CpStart);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nSubThreadAction=20;
		}
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;

			CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
			if(pFrame->m_pMotionToolsSheet) {
				pFrame->m_pMotionToolsSheet->UpdateAllPages();
			}

		}
		break;
	}

}

void CMainThread::SingleTipAutoAlign()
{
	static int	nStart,nFinish;
	int	nNumberSamples = 200;
	int	nMaxPos = 0,nAxisPos;
	float fPos;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	static int nLoop;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_ServoUser.SuspendJoystick(4);

		m_CpMoveTo.Side1.fXt-=5.0;
		theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
		theApp.m_ServoUser.MoveTo(DONT_WAIT);
		nLoop=0;
		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			nStart=(int)(m_CpMoveTo.Side1.fXt / theApp.m_Axes[XT1AXIS].fStepSize);
			nFinish= nStart + (int)(10.0 / theApp.m_Axes[XT1AXIS].fStepSize);
//			theApp.m_ServoUser.ClearArray(0);
//			theApp.m_ServoUser.CollectData(XT1AXIS,nStart,nFinish,nNumberSamples,1,0,TRUE,1);
			m_CpMoveTo.Side1.fXt+=10.0;
			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
			m_nSubThreadAction=20;
		}
		break;
	case 20:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.StopCollectingData();
//			if(pFrame->m_pTipCompensationDlg) {
//				pFrame->m_pTipCompensationDlg->TransferData(XT1AXIS,nNumberSamples);
//			}
//			pFrame->SendMessage(UI_REFRESH_TIP_VIEW);
//			theApp.m_ServoUser.FindPeakAmplitude(nNumberSamples,&nMaxPos);
			nAxisPos = MulDiv(nMaxPos,nFinish-nStart,nNumberSamples) + nStart;
			fPos = (float)nAxisPos * theApp.m_Axes[XT1AXIS].fStepSize;

			m_CpMoveTo.Side1.fXt = fPos;
			m_CpMoveTo.Side1.fYt-=5.0;
			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
			m_nSubThreadAction=30;
		}
		break;
	case 30:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			nStart=(int)(m_CpMoveTo.Side1.fYt / theApp.m_Axes[YT1AXIS].fStepSize);
			nFinish= nStart + (int)(10.0 / theApp.m_Axes[YT1AXIS].fStepSize);
//			theApp.m_ServoUser.ClearArray(0);
//			theApp.m_ServoUser.CollectData(YT1AXIS,nStart,nFinish,nNumberSamples,1,0,TRUE,1);
			m_CpMoveTo.Side1.fYt+=10.0;
			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
			m_nSubThreadAction=40;
		}
		break;
	case 40:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.StopCollectingData();
//			if(pFrame->m_pTipCompensationDlg) {
//				pFrame->m_pTipCompensationDlg->TransferData(YT1AXIS,nNumberSamples);
//			}
//			pFrame->SendMessage(UI_REFRESH_TIP_VIEW);
//			theApp.m_ServoUser.FindPeakAmplitude(nNumberSamples,&nMaxPos);
			nAxisPos = MulDiv(nMaxPos,nFinish-nStart,nNumberSamples) + nStart;
			fPos = (float)nAxisPos * theApp.m_Axes[YT1AXIS].fStepSize;

			m_CpMoveTo.Side1.fYt = fPos;
			if(nLoop==0) {
				m_CpMoveTo.Side1.fXt-=5.0;
				m_nSubThreadAction=10;
				nLoop++;
			} else {
				m_nSubThreadAction=50;
			}

			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
		}
		break;
	case 50:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}
}


void CMainThread::OriginHoldingFixture()
{
	if(CheckForHoldingAbort()==TRUE) return;

	if(theApp.m_Si10User.IsConnected() == TRUE) {
		switch(m_nSubThreadAction) {
		case 0:
			theApp.m_Motors.EnableHomingFixtureHomeFind(-1);
			m_nSubThreadAction=10;
			break;
		case 10:
			if(theApp.m_Motors.TestHoldingFixtureAllAxisRunning() == FALSE) {
				m_nSubThreadAction=0;
				m_nThreadAction=JOYSTICK_START;
			}
			break;
		}
	} else {
		switch(m_nSubThreadAction) {
		case 0:
			theApp.m_Motors.EnableHomingFixtureHomeFind(-1);
			m_nSubThreadAction=10;
			break;
		case 10:
			if(theApp.m_Motors.TestHoldingFixtureAllAxisRunning() == FALSE) {
				m_CpHolding.fXMoving = (float)theApp.m_Axes[theApp.m_Tank.nXStep].nHomeFoundAt * theApp.m_Axes[theApp.m_Tank.nXStep].fStepSize;
				m_CpHolding.fYMoving = (float)theApp.m_Axes[theApp.m_Tank.nYStep].nHomeFoundAt * theApp.m_Axes[theApp.m_Tank.nYStep].fStepSize;
				m_CpHolding.fZMoving = (float)theApp.m_Axes[theApp.m_Tank.nZStep].nHomeFoundAt * theApp.m_Axes[theApp.m_Tank.nZStep].fStepSize;
				m_CpHolding.fYFixed = (float)theApp.m_Axes[theApp.m_Tank.nXtStep].nHomeFoundAt * theApp.m_Axes[theApp.m_Tank.nXtStep].fStepSize;
				m_CpHolding.fZFixed = (float)theApp.m_Axes[theApp.m_Tank.nYtStep].nHomeFoundAt * theApp.m_Axes[theApp.m_Tank.nYtStep].fStepSize;
				theApp.m_Motors.MoveHoldindFixtureToCoord(m_CpHolding);
				m_nSubThreadAction=20;
			}
			break;
		case 20:
			if(theApp.m_Motors.TestHoldingFixtureAllAxisRunning() == FALSE) {
				theApp.m_Motors.HoldingFixtureZeroPosition(theApp.m_Tank.nXStep);
				theApp.m_Motors.HoldingFixtureZeroPosition(theApp.m_Tank.nYStep);
				theApp.m_Motors.HoldingFixtureZeroPosition(theApp.m_Tank.nZStep);
				theApp.m_Motors.HoldingFixtureZeroPosition(theApp.m_Tank.nXtStep);
				theApp.m_Motors.HoldingFixtureZeroPosition(theApp.m_Tank.nYtStep);
				m_CpHolding.fXMoving = (float)theApp.m_Axes[theApp.m_Tank.nXStep].nHomePos * theApp.m_Axes[theApp.m_Tank.nXStep].fStepSize;
				m_CpHolding.fYMoving = (float)theApp.m_Axes[theApp.m_Tank.nYStep].nHomePos * theApp.m_Axes[theApp.m_Tank.nYStep].fStepSize;
				m_CpHolding.fZMoving = (float)theApp.m_Axes[theApp.m_Tank.nZStep].nHomePos * theApp.m_Axes[theApp.m_Tank.nZStep].fStepSize;
				m_CpHolding.fYFixed = (float)theApp.m_Axes[theApp.m_Tank.nXtStep].nHomePos * theApp.m_Axes[theApp.m_Tank.nXtStep].fStepSize;
				m_CpHolding.fZFixed = (float)theApp.m_Axes[theApp.m_Tank.nYtStep].nHomePos * theApp.m_Axes[theApp.m_Tank.nYtStep].fStepSize;
				theApp.m_Motors.MoveHoldindFixtureToCoord(m_CpHolding);
				m_nSubThreadAction=30;
			}
			break;
		case 30:
			if(theApp.m_Motors.TestHoldingFixtureAllAxisRunning() == FALSE) {
				theApp.m_Motors.HoldingFixtureZeroPosition(theApp.m_Tank.nXStep);
				theApp.m_Motors.HoldingFixtureZeroPosition(theApp.m_Tank.nYStep);
				theApp.m_Motors.HoldingFixtureZeroPosition(theApp.m_Tank.nZStep);
				theApp.m_Motors.HoldingFixtureZeroPosition(theApp.m_Tank.nXtStep);
				theApp.m_Motors.HoldingFixtureZeroPosition(theApp.m_Tank.nYtStep);
				m_nSubThreadAction=0;
				m_nThreadAction=JOYSTICK_START;
			}
			break;
		}
	}
}

void CMainThread::OriginSingleAxisHoldingFixture()
{
	if(theApp.m_Si10User.IsConnected() == TRUE) {
		OriginSingleAxisHoldingFixtureSi10();
	} else {
		OriginSingleAxisHoldingFixtureSi6();
	}

}


void CMainThread::OriginSingleAxisHoldingFixtureSi10()
{
	int nAxis = -1;

	switch(m_nThreadAction) {
	case ORIGIN_X_MOVING: nAxis = theApp.m_Tank.nXStep;
		break;
	case ORIGIN_Y_MOVING: nAxis = theApp.m_Tank.nYStep;
		break;
	case ORIGIN_Z_MOVING: nAxis = theApp.m_Tank.nZStep;
		break;
	case ORIGIN_X_FIXED: nAxis = theApp.m_Tank.nRStep;
		break;
	case ORIGIN_Y_FIXED: nAxis = theApp.m_Tank.nXtStep;
		break;
	case ORIGIN_Z_FIXED: nAxis = theApp.m_Tank.nYtStep;
		break;
	}
	if(nAxis==-1) return;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_Motors.EnableHomingFixtureHomeFind(nAxis);
		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_Motors.TestHoldingFixtureAllAxisRunning() == FALSE) {
//			theApp.m_Motors.ZeroHoldingFixturePosition(nAxis);
			m_nSubThreadAction=0;
			m_nThreadAction=JOYSTICK_START;
		}
		break;
	}


}

void CMainThread::OriginSingleAxisHoldingFixtureSi6()
{
	int nAxis=-1;

	if(CheckForHoldingAbort()==TRUE) return;

	switch(m_nThreadAction) {
	case ORIGIN_X_MOVING: nAxis = theApp.m_Tank.nXStep;
		break;
	case ORIGIN_Y_MOVING: nAxis = theApp.m_Tank.nYStep;
		break;
	case ORIGIN_Z_MOVING: nAxis = theApp.m_Tank.nZStep;
		break;
	case ORIGIN_X_FIXED: nAxis = theApp.m_Tank.nRStep;
		break;
	case ORIGIN_Y_FIXED: nAxis = theApp.m_Tank.nXtStep;
		break;
	case ORIGIN_Z_FIXED: nAxis = theApp.m_Tank.nYtStep;
		break;
	}
	if(nAxis==-1) return;


	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_Si6User.EnableHomeFind(nAxis);
		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_Motors.TestHoldingFixtureAllAxisRunning() == FALSE) {
			theApp.m_Motors.GetHoldingCoord(&m_CpHolding);
			switch(m_nThreadAction) {
			case ORIGIN_X_MOVING: m_CpHolding.fXMoving = (float)theApp.m_Axes[theApp.m_Tank.nXStep].nHomeFoundAt * theApp.m_Axes[theApp.m_Tank.nXStep].fStepSize;
				break;
			case ORIGIN_Y_MOVING: m_CpHolding.fYMoving = (float)theApp.m_Axes[theApp.m_Tank.nYStep].nHomeFoundAt * theApp.m_Axes[theApp.m_Tank.nYStep].fStepSize;
				break;
			case ORIGIN_Z_MOVING: m_CpHolding.fZMoving = (float)theApp.m_Axes[theApp.m_Tank.nZStep].nHomeFoundAt * theApp.m_Axes[theApp.m_Tank.nZStep].fStepSize;
				break;
			case ORIGIN_X_FIXED: m_CpHolding.fXFixed = (float)theApp.m_Axes[theApp.m_Tank.nRStep].nHomeFoundAt * theApp.m_Axes[theApp.m_Tank.nRStep].fStepSize;
				break;
			case ORIGIN_Y_FIXED: m_CpHolding.fYFixed = (float)theApp.m_Axes[theApp.m_Tank.nXtStep].nHomeFoundAt * theApp.m_Axes[theApp.m_Tank.nXtStep].fStepSize;
				break;
			case ORIGIN_Z_FIXED: m_CpHolding.fZFixed = (float)theApp.m_Axes[theApp.m_Tank.nYtStep].nHomeFoundAt * theApp.m_Axes[theApp.m_Tank.nYtStep].fStepSize;
				break;
			}
			theApp.m_Si6User.MoveToCoord(m_CpHolding);
			m_nSubThreadAction=20;
		}
		break;
	case 20:
		if(theApp.m_Motors.TestHoldingFixtureAllAxisRunning() == FALSE) {
			theApp.m_Motors.ZeroHoldingFixturePosition(nAxis);
			theApp.m_Motors.GetHoldingCoord(&m_CpHolding);
			switch(m_nThreadAction) {
			case ORIGIN_X_MOVING: m_CpHolding.fXMoving = (float)theApp.m_Axes[theApp.m_Tank.nXStep].nHomePos * theApp.m_Axes[theApp.m_Tank.nXStep].fStepSize;
				break;
			case ORIGIN_Y_MOVING: m_CpHolding.fYMoving = (float)theApp.m_Axes[theApp.m_Tank.nYStep].nHomePos * theApp.m_Axes[theApp.m_Tank.nYStep].fStepSize;
				break;
			case ORIGIN_Z_MOVING: m_CpHolding.fZMoving = (float)theApp.m_Axes[theApp.m_Tank.nZStep].nHomePos * theApp.m_Axes[theApp.m_Tank.nZStep].fStepSize;
				break;
			case ORIGIN_X_FIXED: m_CpHolding.fXFixed = (float)theApp.m_Axes[theApp.m_Tank.nRStep].nHomePos * theApp.m_Axes[theApp.m_Tank.nRStep].fStepSize;
				break;
			case ORIGIN_Y_FIXED: m_CpHolding.fYFixed = (float)theApp.m_Axes[theApp.m_Tank.nXtStep].nHomePos * theApp.m_Axes[theApp.m_Tank.nXtStep].fStepSize;
				break;
			case ORIGIN_Z_FIXED: m_CpHolding.fZFixed = (float)theApp.m_Axes[theApp.m_Tank.nYtStep].nHomePos * theApp.m_Axes[theApp.m_Tank.nYtStep].fStepSize;
				break;
			}
			theApp.m_Si6User.MoveToCoord(m_CpHolding);
			m_nSubThreadAction=30;
		}
		break;
	case 30:
		if(theApp.m_Motors.TestHoldingFixtureAllAxisRunning() == FALSE) {
			theApp.m_Motors.ZeroHoldingFixturePosition(nAxis);
			m_nSubThreadAction=0;
			m_nThreadAction=JOYSTICK_START;
		}
		break;
	}
}


void CMainThread::OriginHoldingFixtureFixed()
{
/*
	if(CheckForHoldingAbort()==TRUE) return;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_Si6User.EnableHomeFindFixed();
		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_Si6User.TestAllAxisRunning() == FALSE) {
			m_CpHold.fXp = (float)theApp.m_AxesHolding[XMOVEABLE].nPos*theApp.m_AxesHolding[XMOVEABLE].fStepSize;
			m_CpHold.fYFixedp = (float)theApp.m_AxesHolding[YFIXED].nHomeFoundAt*theApp.m_AxesHolding[YFIXED].fStepSize;
			m_CpHold.fZFixedp = (float)theApp.m_AxesHolding[ZFIXED].nHomeFoundAt*theApp.m_AxesHolding[ZFIXED].fStepSize;
			m_CpHold.fYMoveablep = (float)theApp.m_AxesHolding[YMOVEABLE].nPos*theApp.m_AxesHolding[YMOVEABLE].fStepSize;
			m_CpHold.fZMoveablep = (float)theApp.m_AxesHolding[ZMOVEABLE].nPos*theApp.m_AxesHolding[ZMOVEABLE].fStepSize;
			theApp.m_Si6User.MoveToCoord(m_CpHold);
			m_nSubThreadAction=20;
		}
		break;
	case 20:
		if(theApp.m_Si6User.TestAllAxisRunning() == FALSE) {
			theApp.m_Si6User.ZeroPosition(YFIXED);
			theApp.m_Si6User.ZeroPosition(ZFIXED);
			m_CpHold.fXp = (float)theApp.m_AxesHolding[XMOVEABLE].nPos*theApp.m_AxesHolding[XMOVEABLE].fStepSize;
			m_CpHold.fYFixedp = (float)theApp.m_AxesHolding[YFIXED].nHomePos*theApp.m_AxesHolding[YFIXED].fStepSize;
			m_CpHold.fZFixedp = (float)theApp.m_AxesHolding[ZFIXED].nHomePos*theApp.m_AxesHolding[ZFIXED].fStepSize;
			m_CpHold.fYMoveablep = (float)theApp.m_AxesHolding[YMOVEABLE].nPos*theApp.m_AxesHolding[YMOVEABLE].fStepSize;
			m_CpHold.fZMoveablep = (float)theApp.m_AxesHolding[ZMOVEABLE].nPos*theApp.m_AxesHolding[ZMOVEABLE].fStepSize;
			theApp.m_Si6User.MoveToCoord(m_CpHold);
			m_nSubThreadAction=30;
		}
		break;
	case 30:
		if(theApp.m_Si6User.TestAllAxisRunning() == FALSE) {
			theApp.m_Si6User.ZeroPosition(YFIXED);
			theApp.m_Si6User.ZeroPosition(ZFIXED);
			m_nSubThreadAction=0;
			m_nThreadAction=JOYSTICK_START;
		}
		break;
	}
*/
}

void CMainThread::OriginHoldingFixtureMoveing()
{
/*
	if(CheckForHoldingAbort()==TRUE) return;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_Si6User.EnableHomeFindMoveing();
		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_Si6User.TestAllAxisRunning() == FALSE) {
			m_CpHold.fXp = (float)theApp.m_AxesHolding[XMOVEABLE].nHomeFoundAt*theApp.m_AxesHolding[XMOVEABLE].fStepSize;
			m_CpHold.fYFixedp = (float)theApp.m_AxesHolding[YFIXED].nPos*theApp.m_AxesHolding[YFIXED].fStepSize;
			m_CpHold.fZFixedp = (float)theApp.m_AxesHolding[ZFIXED].nPos*theApp.m_AxesHolding[ZFIXED].fStepSize;
			m_CpHold.fYMoveablep = (float)theApp.m_AxesHolding[YMOVEABLE].nHomeFoundAt*theApp.m_AxesHolding[YMOVEABLE].fStepSize;
			m_CpHold.fZMoveablep = (float)theApp.m_AxesHolding[ZMOVEABLE].nHomeFoundAt*theApp.m_AxesHolding[ZMOVEABLE].fStepSize;
			theApp.m_Si6User.MoveToCoord(m_CpHold);
			m_nSubThreadAction=20;
		}
		break;
	case 20:
		if(theApp.m_Si6User.TestAllAxisRunning() == FALSE) {
			theApp.m_Si6User.ZeroPosition(XMOVEABLE);
			theApp.m_Si6User.ZeroPosition(YMOVEABLE);
			theApp.m_Si6User.ZeroPosition(ZMOVEABLE);
			m_CpHold.fXp = (float)theApp.m_AxesHolding[XMOVEABLE].nHomePos*theApp.m_AxesHolding[XMOVEABLE].fStepSize;
			m_CpHold.fYFixedp = (float)theApp.m_AxesHolding[YFIXED].nPos*theApp.m_AxesHolding[YFIXED].fStepSize;
			m_CpHold.fZFixedp = (float)theApp.m_AxesHolding[ZFIXED].nPos*theApp.m_AxesHolding[ZFIXED].fStepSize;
			m_CpHold.fYMoveablep = (float)theApp.m_AxesHolding[YMOVEABLE].nHomePos*theApp.m_AxesHolding[YMOVEABLE].fStepSize;
			m_CpHold.fZMoveablep = (float)theApp.m_AxesHolding[ZMOVEABLE].nHomePos*theApp.m_AxesHolding[ZMOVEABLE].fStepSize;
			theApp.m_Si6User.MoveToCoord(m_CpHold);
			m_nSubThreadAction=30;
		}
		break;
	case 30:
		if(theApp.m_Si6User.TestAllAxisRunning() == FALSE) {
			theApp.m_Si6User.ZeroPosition(XMOVEABLE);
			theApp.m_Si6User.ZeroPosition(YMOVEABLE);
			theApp.m_Si6User.ZeroPosition(ZMOVEABLE);
			m_nSubThreadAction=0;
			m_nThreadAction=JOYSTICK_START;
		}
		break;
	}
*/
}

void CMainThread::MovetoHoldingFixture()
{
	CString Buff,Warning;

	if(CheckForHoldingAbort()==TRUE) return;

	switch(m_nSubThreadAction) {
	case 0:
		Buff.LoadString(IDS_Is_It_Safe_To_Carry);
		Warning.LoadString(IDS_Warning_Fixture_Movement);
		if(MessageBox(NULL,Buff,Warning,MB_ICONWARNING|MB_YESNO)==IDYES) {
			theApp.m_Motors.MoveHoldindFixtureToCoord(m_CpHolding);
			m_nSubThreadAction=10;
			Sleep(100);
		} else {
			m_nSubThreadAction=0;
			m_nThreadAction=JOYSTICK_START;
		}
		break;
	case 10:
		if(theApp.m_Motors.TestHoldingFixtureAllAxisRunning() == FALSE) {
			m_nSubThreadAction=0;
			m_nThreadAction=JOYSTICK_START;
		}
		break;
	}
}

void CMainThread::MovetoSafely()
{
	CCoord	Cp;
/*
	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_ServoUser.GetHeadPos(&Cp);

		m_CpPoly[0]=Cp;
		m_CpPoly[1]=m_CpMoveTo;
		m_CpPoly[2]=m_CpMoveTo;
		if(m_nMoveSide & 1) {
			m_CpPoly[0].Side0.fY=PROFILE->m_CpDanger[UPPER_LEFT].Side0.fY;
			m_CpPoly[1].Side0.fY=PROFILE->m_CpDanger[UPPER_LEFT].Side0.fY;
		}
		if(m_nMoveSide & 2) {
			m_CpPoly[0].Side1.fY=PROFILE->m_CpDanger[UPPER_LEFT].Side1.fY;
			m_CpPoly[1].Side1.fY=PROFILE->m_CpDanger[UPPER_LEFT].Side1.fY;
		}



		theApp.m_ServoUser.SuspendJoystick(4);
		theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpPoly[0]);
		theApp.m_ServoUser.MoveTo(DONT_WAIT);
		Sleep(250);
		m_nSubThreadAction = 10;
		break;
	case 10:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpPoly[1]);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
			Sleep(250);
			m_nSubThreadAction = 20;
		}
		break;
	case 20:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpPoly[2]);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
			Sleep(250);
			m_nSubThreadAction = 30;
		}
		break;
	case 30:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}
	*/
}

void CMainThread::FlatScan()
{
/*	static int nDir;
	static int nLine;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CoordStruct	CpStartOfLine,CpSurface,CpHead;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_ServoUser.SuspendJoystick(4);
		theApp.m_ServoUser.GetHeadPos(&m_CpMoveTo);
		m_CpMoveTo.Side0.fY=(float)theApp.m_Axes[Y0AXIS].nOffset * theApp.m_Axes[Y0AXIS].fStepSize * -1.0f;
		m_CpMoveTo.Side1.fY=(float)theApp.m_Axes[Y1AXIS].nOffset * theApp.m_Axes[Y1AXIS].fStepSize * -1.0f;
		m_CpMoveTo.Side0.fXt=0.0;
		m_CpMoveTo.Side1.fXt=0.0;
		m_CpMoveTo.Side0.fYt=0.0;
		m_CpMoveTo.Side1.fYt=0.0;

		theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
		theApp.m_ServoUser.MoveTo(DONT_WAIT);
		Sleep(100);
		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			CpSurface=PROFILE->m_CpTestPiece[0];
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			theApp.m_ServoUser.GetHeadPos(&CpHead);
			switch(PROFILE->m_nScanSide) {
			case 0:
				m_CpMoveTo.Side1.fX=CpHead.Side1.fX;
				m_CpMoveTo.Side1.fY=CpHead.Side1.fY;
				m_CpMoveTo.Side1.fZ=CpHead.Side1.fZ;
				m_CpMoveTo.Side1.fXt=CpHead.Side1.fXt;
				m_CpMoveTo.Side1.fYt=CpHead.Side1.fYt;
				break;
			case 1:
				m_CpMoveTo.Side0.fX=CpHead.Side0.fX;
				m_CpMoveTo.Side0.fY=CpHead.Side0.fY;
				m_CpMoveTo.Side0.fZ=CpHead.Side0.fZ;
				m_CpMoveTo.Side0.fXt=CpHead.Side0.fXt;
				m_CpMoveTo.Side0.fYt=CpHead.Side0.fYt;
				break;
			case 2:
				break;
			}

			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
			Sleep(100);
			m_nSubThreadAction=20;
		}
	case 20:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.SuspendJoystick(5);

			PROFILE->CalculateToolTipForTestPiece(nLine=0,pFrame->m_pScanView3DDlg->m_Data.m_nTPNumberLines-1);
			m_pPolyCp = PROFILE->m_pCpToolTip;
			m_nPolyL = PROFILE->m_nToolTipL;


			theApp.m_ServoUser.DownLoadPolyMoveCoordinates(m_nPolyL, m_pPolyCp, nDir=0);
			if(theApp.m_LastSettings.bWithLookAhead==TRUE) theApp.m_ServoUser.SetupLookAhead();
			theApp.m_ServoUser.StartCollectingComplexData(X0AXIS);
			theApp.m_ServoUser.StartPolyMoveWithLookAhead(m_nPolyL);
			m_nSubThreadAction=30;
			Sleep(100);
			m_bAbortFlag = FALSE;
		};
		break;
	case 30:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.StopCollectingComplexData();

			pFrame->m_pScanView3DDlg->m_Data.GetFlatScanLineFromDriver(nLine);
			pFrame->m_pScanView3DDlg->m_Data.BuildTestPieceCscan(pFrame->m_pScanView3DDlg->m_nImageNumber,pFrame->m_pScanView3DDlg->m_nImageType);
			pFrame->m_pScanView3DDlg->ForceRefreshOfCscanArray();
			pFrame->m_pScanView3DDlg->Invalidate(FALSE);

			nDir ^= 1;
			nLine++;
			if((nLine<(pFrame->m_pScanView3DDlg->m_Data.m_nTPNumberLines-1)) && (m_bAbortFlag==FALSE)) {
				PROFILE->CalculateToolTipForTestPiece(nLine,pFrame->m_pScanView3DDlg->m_Data.m_nTPNumberLines-1);
				theApp.m_Thread.m_pPolyCp = PROFILE->m_pCpToolTip;
				theApp.m_Thread.m_nPolyL = PROFILE->m_nToolTipL;
				if(nDir == 0) {
					CpStartOfLine = PROFILE->m_pCpToolTip[0];
				} else {
					CpStartOfLine = PROFILE->m_pCpToolTip[PROFILE->m_nToolTipL-1];
				}

				theApp.m_ServoUser.DownLoadPolyMoveCoordinates(1, &CpStartOfLine, 0);
				if(theApp.m_LastSettings.bWithLookAhead==TRUE) theApp.m_ServoUser.SetupLookAhead();
				theApp.m_ServoUser.StartPolyMoveWithLookAhead(1);
				m_nSubThreadAction=40;
			} else {
				m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
			}
			Sleep(100);
		}
		break;
	case 40:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {

			theApp.m_ServoUser.DownLoadPolyMoveCoordinates(m_nPolyL, m_pPolyCp, nDir);
			if(theApp.m_LastSettings.bWithLookAhead==TRUE) theApp.m_ServoUser.SetupLookAhead();
			theApp.m_ServoUser.StartCollectingComplexData(X0AXIS);

			theApp.m_ServoUser.StartPolyMoveWithLookAhead(m_nPolyL);
			m_nSubThreadAction=30;
		}
		break;
	}
	*/
}

void CMainThread::FindZero()
{
/*	static int	nStart,nFinish;
	int	nNumberSamples = 200;
	int	nMaxPos,nAxisPos;
	float fPos;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	static nLoop;
	int	nStartTimeNs,nStopTimeNs;
	bool bPulseEcho;
	int	nTransmitter;
	int	nXAxis,nZAxis,nSide;
	float fMovement = 1.5;

	if(m_nMoveSide == PORTSIDE) {
		nSide = PORTSIDE;
		nXAxis = X0AXIS;
		nZAxis = Z0AXIS;
	} else {
		nSide = STARBOARD;
		nXAxis = X1AXIS;
		nZAxis = Z1AXIS;
	}

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_ServoUser.SuspendJoystick(4);

		theApp.m_ServoUser.GetHeadPos(&m_CpMoveTo);
		m_CpMoveTo.Side[nSide].fZ-=fMovement;
		theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
		theApp.m_ServoUser.MoveTo(DONT_WAIT);
		nLoop=0;
		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {

			nStart=(int)(m_CpMoveTo.Side[nSide].fZ / theApp.m_Axes[nZAxis].fStepSize);
			nFinish= nStart + (int)((fMovement*2.0f) / theApp.m_Axes[nZAxis].fStepSize);
			theApp.m_ServoUser.ClearArray(0);
			theApp.m_ServoUser.CollectData(nZAxis,nStart,nFinish,nNumberSamples,1,0,TRUE,1);
			m_CpMoveTo.Side[nSide].fZ+=(fMovement*2.0f);
			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
			m_nSubThreadAction=20;
		}
		break;
	case 20:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.StopCollectingData();
			if(pFrame->m_pRhomboidCompenstionDlg) {
				pFrame->m_pRhomboidCompenstionDlg->TransferData(nZAxis,nNumberSamples);
			}
//			pFrame->SendMessage(UI_REFRESH_RHOMBOID_VIEW);
			theApp.m_ServoUser.FindPeakAmplitude(nNumberSamples,&nMaxPos);
			nAxisPos = MulDiv(nMaxPos,nFinish-nStart,nNumberSamples) + nStart;
			fPos = (float)nAxisPos * theApp.m_Axes[nZAxis].fStepSize;

			m_CpMoveTo.Side[nSide].fX-=fMovement;
			m_CpMoveTo.Side[nSide].fZ = fPos;

			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
			m_nSubThreadAction=30;
		}
		break;
	case 30:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			nStart=(int)(m_CpMoveTo.Side[nSide].fX / theApp.m_Axes[nXAxis].fStepSize);
			nFinish= nStart + (int)((fMovement*2.0f) / theApp.m_Axes[nXAxis].fStepSize);
			theApp.m_ServoUser.ClearArray(0);
			theApp.m_ServoUser.CollectData(nXAxis,nStart,nFinish,nNumberSamples,1,0,TRUE,1);
			m_CpMoveTo.Side[nSide].fX+=(fMovement*2.0f);
			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
			m_nSubThreadAction=40;
		}
		break;
	case 40:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.StopCollectingData();
			if(pFrame->m_pRhomboidCompenstionDlg) {
				pFrame->m_pRhomboidCompenstionDlg->TransferData(nXAxis,nNumberSamples);
			}
	//		pFrame->SendMessage(UI_REFRESH_RHOMBOID_VIEW);
			theApp.m_ServoUser.FindPeakAmplitude(nNumberSamples,&nMaxPos);
			nAxisPos = MulDiv(nMaxPos,nFinish-nStart,nNumberSamples) + nStart;
			fPos = (float)nAxisPos * theApp.m_Axes[nXAxis].fStepSize;

			m_CpMoveTo.Side[nSide].fX = fPos;

			if(nLoop==0) {
				m_CpMoveTo.Side[nSide].fZ-=fMovement;
				m_nSubThreadAction=10;
				nLoop++;
			} else {
				m_nSubThreadAction=50;
			}
			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
		}
		break;
	case 50:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_UtUser.Dsp200GetStartStopTimeNs(&nStartTimeNs,&nStopTimeNs,&bPulseEcho,&nTransmitter);
			double dPath= ((double)nStartTimeNs*(double)theApp.m_LastSettings.nWaterVelocity) / 1e6;
			if(bPulseEcho==TRUE) dPath /= 2.0;
			double dWaterPathError= dPath - theApp.m_Kinematics.m_fDesiredWaterPath[nSide];
			if(fabs(dWaterPathError)<=10.0) {
				m_CpMoveTo.Side[nSide].fY -= (float)dWaterPathError;
				theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
				theApp.m_ServoUser.MoveTo(DONT_WAIT);
				m_nSubThreadAction=60;
			} else {
				m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			}
		}
		break;
	case 60:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}
*/

}


void CMainThread::ImmersionContourScan()
{
/*	static int nDir;
	static int nLine;
	static int nLastCoordinate;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CoordStruct	CpStartOfLine,Cp;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_LastSettings.nBrainOffOn = 0;
		theApp.m_bScanning=TRUE;
		theApp.m_ServoUser.SuspendJoystick(4);

		theApp.m_ServoUser.GetHeadPos(&m_CpMoveTo);
		switch(PROFILE->m_nScanSide) {
		case 0:
			break;
		case 1:
			m_CpMoveTo.Side1.fZ=PROFILE->m_CpDanger[0].Side1.fZ;
			break;
		case 2:
			break;
		}
		m_CpMoveTo.Side0.fXt=0.0;
		m_CpMoveTo.Side1.fXt=0.0;
		m_CpMoveTo.Side0.fYt=0.0;
		m_CpMoveTo.Side1.fYt=0.0;

		theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
		theApp.m_ServoUser.MoveTo(DONT_WAIT);

		m_nSubThreadAction=2;
		break;
	case 2:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			CpStartOfLine=PROFILE->m_PolyLine[m_nScanStartLine].m_Coord[0];

			theApp.m_Kinematics.TipsFromNormals(&CpStartOfLine,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&CpStartOfLine,&Cp,STARBOARD,TRUE,TRUE);

			m_CpMoveTo.Side1.fX=Cp.Side1.fX;
			m_CpMoveTo.Side1.fY=Cp.Side1.fY;
			m_CpMoveTo.Side1.fXt=Cp.Side1.fXt;
			m_CpMoveTo.Side1.fYt=Cp.Side1.fYt;

			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);

			m_nSubThreadAction=4;
		}
		break;
	case 4:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			CpStartOfLine=PROFILE->m_PolyLine[m_nScanStartLine].m_Coord[0];

			theApp.m_Kinematics.TipsFromNormals(&CpStartOfLine,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&CpStartOfLine,&Cp,STARBOARD,TRUE,TRUE);

			m_CpMoveTo.Side1.fX=Cp.Side1.fX;
			m_CpMoveTo.Side1.fY=Cp.Side1.fY;
			m_CpMoveTo.Side1.fZ=Cp.Side1.fZ;
			m_CpMoveTo.Side1.fXt=Cp.Side1.fXt;
			m_CpMoveTo.Side1.fYt=Cp.Side1.fYt;

			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpMoveTo);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);

			m_nSubThreadAction=10;
		}
		break;
	case 10:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.SuspendJoystick(6);

			PROFILE->CalculateToolTipFromLine(nLine=m_nScanStartLine);
			m_pPolyCp = PROFILE->m_pCpToolTip;
			m_nPolyL = PROFILE->m_nToolTipL;


			nLastCoordinate=theApp.m_ServoUser.DownLoadPolyMoveCoordinates(m_nPolyL, m_pPolyCp, nDir=0);
			if(theApp.m_LastSettings.bWithLookAhead==TRUE) theApp.m_ServoUser.SetupLookAhead();
			theApp.m_ServoUser.StartCollectingComplexData(X1AXIS);
			theApp.m_ServoUser.StartPolyMoveWithLookAhead(m_nPolyL);
			m_nSubThreadAction=20;
			m_bAbortFlag = FALSE;
		};
		break;
	case 20:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.StopCollectingComplexData();

			pFrame->m_pScanView3DDlg->m_Data.GetScanLineFromDriver(nLine-m_nScanStartLine,STARBOARD);
			PROFILE->GetScanLineFromDriver();

			if(pFrame->m_pProfileGraphDlg!=NULL) {
				pFrame->m_pProfileGraphDlg->m_nLine=nLine;
				pFrame->m_pProfileGraphDlg->Invalidate(FALSE);
			}

			nDir ^= 1;
			nLine+=PROFILE->m_nLineIncrement;
			if((nLine<PROFILE->m_nScanFinishLine) && (m_bAbortFlag==FALSE)) {
				PROFILE->CalculateToolTipFromLine(nLine);
				theApp.m_Thread.m_pPolyCp = PROFILE->m_pCpToolTip;
				theApp.m_Thread.m_nPolyL = PROFILE->m_nToolTipL;
				if(nDir == 0) {
					CpStartOfLine = PROFILE->m_pCpToolTip[0];
				} else {
					CpStartOfLine = PROFILE->m_pCpToolTip[PROFILE->m_nToolTipL-1];
				}

				theApp.m_ServoUser.DownLoadPolyMoveCoordinates(1, &CpStartOfLine, 0);
				if(theApp.m_LastSettings.bWithLookAhead==TRUE) theApp.m_ServoUser.SetupLookAhead();
				theApp.m_ServoUser.StartPolyMoveWithLookAhead(1);
				m_nSubThreadAction=30;
			} else {
				m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
			}

			pFrame->m_pScanView3DDlg->m_Data.BuildCscan(pFrame->m_pScanView3DDlg->m_nImageNumber,pFrame->m_pScanView3DDlg->m_nImageType);
			pFrame->m_pScanView3DDlg->ForceRefreshOfCscanArray();
			pFrame->m_pScanView3DDlg->Invalidate(FALSE);
		} else {
			nLastCoordinate=theApp.m_ServoUser.DownLoadNextPolyMoveCoordinate(nLastCoordinate,m_nPolyL, m_pPolyCp, nDir);
		}
		break;
	case 30:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {

			nLastCoordinate=theApp.m_ServoUser.DownLoadPolyMoveCoordinates(m_nPolyL, m_pPolyCp, nDir);
			if(theApp.m_LastSettings.bWithLookAhead==TRUE) theApp.m_ServoUser.SetupLookAhead();
			theApp.m_ServoUser.StartCollectingComplexData(X1AXIS);

			theApp.m_ServoUser.StartPolyMoveWithLookAhead(m_nPolyL);
			m_nSubThreadAction=20;
		}
		break;
	}
*/
}

void CMainThread::ImmersionMovetoSafely()
{
/*	CoordStruct	Cp;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_ServoUser.GetHeadPos(&Cp);

		m_CpPoly[0]=Cp;
		m_CpPoly[1]=m_CpMoveTo;
		m_CpPoly[2]=m_CpMoveTo;
		if(m_nMoveSide & 1) {
			m_CpPoly[0].Side0.fZ=PROFILE->m_CpDanger[UPPER_LEFT].Side0.fY;
			m_CpPoly[1].Side0.fZ=PROFILE->m_CpDanger[UPPER_LEFT].Side0.fY;
		}
		if(m_nMoveSide & 2) {
			m_CpPoly[0].Side1.fZ=PROFILE->m_CpDanger[UPPER_LEFT].Side1.fZ;
			m_CpPoly[1].Side1.fZ=PROFILE->m_CpDanger[UPPER_LEFT].Side1.fZ;
		}



		theApp.m_ServoUser.SuspendJoystick(4);
		theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpPoly[0]);
		theApp.m_ServoUser.MoveTo(DONT_WAIT);
		Sleep(250);
		m_nSubThreadAction = 10;
		break;
	case 10:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpPoly[1]);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
			Sleep(250);
			m_nSubThreadAction = 20;
		}
		break;
	case 20:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			theApp.m_ServoUser.DesirePosEqualsCoord(&m_CpPoly[2]);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
			Sleep(250);
			m_nSubThreadAction = 30;
		}
		break;
	case 30:
		if(theApp.m_ServoUser.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}
*/
}

void CMainThread::ImmersionOriginAllRight()
{

/*	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_ServoUser.OriginFindAxis(Z1AXIS);
		m_nSubThreadAction=10;
		Sleep(20);
		break;
	case 10:
		if(theApp.m_ServoUser.IsOriginFinished(Z1AXIS) == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(Y1AXIS);
			m_nSubThreadAction=20;
			Sleep(20);
		}
		break;
	case 20:
		if(theApp.m_ServoUser.IsOriginFinished(Y1AXIS) == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(X1AXIS);
			m_nSubThreadAction=30;
			Sleep(20);
		}
		break;
	case 30:
		if(theApp.m_ServoUser.IsOriginFinished(X1AXIS) == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(XT1AXIS);
			m_nSubThreadAction=40;
			Sleep(20);
		}
		break;
	case 40:
		if(theApp.m_ServoUser.IsOriginFinished(XT1AXIS) == TRUE) {
			theApp.m_ServoUser.OriginFindAxis(YT1AXIS);
			m_nSubThreadAction=100;
			Sleep(20);
		}
		break;
	case 100:
		if(theApp.m_ServoUser.IsOriginFinished(YT1AXIS) == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			Sleep(20);
		}
		break;
	}
*/
}





bool CMainThread::HasNormalizeStarted()
{
	if(m_nNormalizeStatus > 0) return TRUE;
	return FALSE;

}

bool CMainThread::HasNormalizeFinished()
{
	if(m_nNormalizeStatus == 999) return TRUE;
	return FALSE;

}

bool CMainThread::HasWaterPathAdjustFinished()
{
	if(m_nThreadAction!=WATERPATH_ADJUST) return TRUE;
	return FALSE;

}

bool CMainThread::HasPeakOppositeFinished()
{
	if(m_nPeakOppositeStatus == 999) return TRUE;
	return FALSE;

}

bool CMainThread::HasPeakOppositeStarted()
{
	if(m_nPeakOppositeStatus >0) return true;
	return false;

}

bool CMainThread::HasProbeCrossSectionFinished()
{
	if(m_nThreadAction!=PROBE_CROSS_SECTION) return TRUE;
	return FALSE;

}

bool CMainThread::HasAnyTypeOfMoveToFinished()
{
	if(m_nThreadAction==MOVETO_SAFELY) return FALSE;
	if(m_nThreadAction==MOVETO_STRAITE_LINE) return FALSE;
	if((m_nThreadAction==MOVETO_VIA_DANGER_PLANE) || (m_nThreadAction==MOVETO_SURFACE)) return FALSE;
	return true;
}

bool CMainThread::HasMovetoSafelyFinished()
{
	if(m_nThreadAction==MOVETO_SAFELY) return FALSE;
	return TRUE;

}

bool CMainThread::HasMovetoStraightLineFinished()
{
	if(m_nThreadAction==MOVETO_STRAITE_LINE) return FALSE;
	return TRUE;

}

bool CMainThread::HasMovetoViaDangerPlaneFinished()
{
	if((m_nThreadAction==MOVETO_VIA_DANGER_PLANE) || (m_nThreadAction==MOVETO_SURFACE)) return FALSE;
	return TRUE;

}

bool CMainThread::CheckForHoldingAbort()
{
/*
	if(m_bHoldingAbort==TRUE) {
		theApp.m_Si6User.StopAllMotorsDead();
		m_nSubThreadAction=0;
		m_nThreadAction=JOYSTICK_START;
		m_bHoldingAbort=FALSE;
		return TRUE;
	}
	m_bHoldingAbort=FALSE;
	*/
	return FALSE;
}




void CMainThread::ContourScanRAxis(int nScanLineNumber)
{
	CCoord	Cp;
	CString Buff;

	Cp = PROFILE->m_ScanLine[nScanLineNumber].m_pCp[0];
	theApp.m_Axes[theApp.m_Tank.nRLeft].nDesirePos=(int)(Cp.Side0.fR/theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize); // TODO need to control Revcounter better + (theApp.m_Motors.m_nRevCounter*theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize*360);
	Buff.Format(_T("#%dj=%d"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis,theApp.m_Axes[theApp.m_Tank.nRLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nRLeft].nOffset);
	theApp.m_PmacUser.SendStr(Buff);
}

void CMainThread::CollectLaserTaughtLineData()
{

}

void CMainThread::CollectNormalizeData()
{
	CCoord CpHead,CpSurface;
	int nSample;
	D3DXVECTOR4 V,W;

	switch(theApp.m_Tank.nScannerDescription) {
	default:	theApp.m_Motors.GetHeadPos(&CpHead);
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TANK_5AXIS:
	case TANK_2AXIS:
	case TWIN_TOWER_KINEMATIC:
	case TANK_6AXIS:
		theApp.m_Motors.GetSurfacePos(&CpHead);
		break;
	case TRACKED_PROBE:
		theApp.m_pTracker->GetSurfacePos(&CpHead);
		break;
	}

	if((m_nNormAction == NORMALIZE) && 
		(
		(theApp.m_LastSettings.nManipulatorType[m_nNormSide] == ROTATE_DIF_THETA) ||
		(theApp.m_LastSettings.nManipulatorType[m_nNormSide] == ROTATE_THETA_VERTICAL) ||
		(theApp.m_LastSettings.nManipulatorType[m_nNormSide] == ROTATE_THETA_HORIZONTAL)
		)){
		theApp.m_Motors.GetSurfacePos(&CpSurface);
		W = CpSurface.Side[PORTSIDE].norm - m_ToolPath.m_pCp[0].Side[PORTSIDE].norm;
		V = m_ToolPath.m_pCp[m_ToolPath.m_nCoordL-1].Side[PORTSIDE].norm - m_ToolPath.m_pCp[0].Side[PORTSIDE].norm;
		float fW = D3DXVec4Length(&W);
		float fV = D3DXVec4Length(&V);
		nSample = (int)((fW * (float)(m_CpNormal.m_nCoordL-1)) / fV);
		CpHead = CpSurface;
	} else {
		float fXMin = m_ToolPath.m_pCp[0].fPos[m_ToolPath.m_nPrimaryAxis];
		float fXMax = m_ToolPath.m_pCp[m_ToolPath.m_nCoordL-1].fPos[m_ToolPath.m_nPrimaryAxis];
		nSample = (int)((CpHead.fPos[m_ToolPath.m_nPrimaryAxis]-fXMin) * (float)(m_CpNormal.m_nCoordL-1) / (fXMax - fXMin));
	}


	if((nSample>=0) && (nSample<m_CpNormal.m_nCoordL)) {
		switch(m_nNormAction) {
		case NORMALIZE:
			CpHead.nAmp = abs(theApp.m_UtUser.m_TS[theApp.m_LastSettings.nNormalizeTimeSlot[theApp.m_LastSettings.nNormSide]].Gate.nAmplitude[theApp.m_LastSettings.nNormalizeGate]);
			break;
		case PEAK_OPPOSITE:
			CpHead.nAmp = abs(theApp.m_UtUser.m_TS[theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide]].Gate.nAmplitude[theApp.m_LastSettings.nPeakGate]);
			break;
		}
		m_CpNormal.m_pCp[nSample] = CpHead;
	}
}



void CMainThread::Normalize()
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_LastSettings.nManipulatorType[m_nNormSide]) {
		default: NormalizePitchYaw();
			break;
		case ROTATE_DIF_THETA:
			NormalizeRotateThetaDiff();
			break;
		case ROTATE_THETA_HORIZONTAL:
		case ROTATE_THETA_VERTICAL:
			NormalizeRotateTheta();
			break;
		}
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		NormalizeRobot();
		break;
	case RAILWAY_AXLE:
	case TANK_5AXIS:
	case TANK_2AXIS:
	case DUAL_ROBOT:
	case TWIN_TOWER_KINEMATIC:
	case TANK_6AXIS:
		NormalizePPMAC();
		break;
	case TRACKED_PROBE:
		break;
	}

}

void CMainThread::NormalizeRotateThetaDiff()
{
	int nSide,nNotSide,ii;
	float fAngle;
	CCoord CpCurrent,CpHead,CpSurface;
	D3DXMATRIXA16 matRot;
	D3DXVECTOR3 vec;

	nSide=theApp.m_PmacUser.m_nScanSide=m_nNormSide;
	nSide==PORTSIDE ? nNotSide = STARBOARD : nNotSide = PORTSIDE;


	switch(m_nSubThreadAction) {
	case 0:
		m_nNormalizeStatus = 1;
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.GetHeadPos(&m_CpHead);
		theApp.m_Motors.GetSurfacePos(&m_CpSurface);
		CpCurrent = m_CpSurface;

		theApp.m_Kinematics.D3DXRotateMatrixFromNormal(m_matNormal, m_CpSurface.Side[nSide].norm);

		if(theApp.m_LastSettings.nNormalizeXtip != 1) {
			m_nSubThreadAction = 100;
			return;
		}

		vec = D3DXVECTOR3(0.0f,0.0f,1.0f);
		D3DXMatrixRotationY(&matRot,-theApp.m_LastSettings.fNormalizeAngle * DEG_TO_RAD);
		D3DXVec3TransformCoord(&vec,&vec,&matRot);
		D3DXVec3TransformCoord(&vec,&vec,&m_matNormal);
		CopyMemory(m_CpSurface.Side[nSide].norm, vec,sizeof D3DXVECTOR3);

		theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&CpHead,PORTSIDE,TRUE,TRUE,&CpCurrent);
//		theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&CpHead,STARBOARD);
		m_CpMoveTo = CpHead;
		m_CpMoveTo.Side0.fR = m_CpSurface.Side0.fR;

		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		m_nSubThreadAction = 10;
		Sleep(500);
		break;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetSurfacePos(&CpSurface);
			CpCurrent = CpSurface;
			m_ToolPath.Zero();

			for(ii=0;ii<40;ii++) {
				fAngle = ((theApp.m_LastSettings.fNormalizeAngle * 2.0f * (float)ii) / 39.0f) - theApp.m_LastSettings.fNormalizeAngle;
				vec = D3DXVECTOR3(0.0f,0.0f,1.0f);
				D3DXMatrixRotationY(&matRot,fAngle * DEG_TO_RAD);
				D3DXVec3TransformCoord(&vec,&vec,&matRot);
				D3DXVec3TransformCoord(&vec,&vec,&m_matNormal);
				CopyMemory(m_CpSurface.Side[nSide].norm, vec,sizeof D3DXVECTOR3);

				theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&CpHead,PORTSIDE,TRUE,TRUE,&CpCurrent);
//				theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&CpHead,STARBOARD);
				m_ToolPath.Add(CpHead);
			}
			theApp.m_Motors.SuspendMotors();
			if(nSide==PORTSIDE) {
				m_ToolPath.SetPrimaryAxis(3);
				theApp.m_Motors.EnableAxes(LEFT_POLY_MOVE);
			} else {
				m_ToolPath.SetPrimaryAxis(13);
				theApp.m_Motors.EnableAxes(RIGHT_POLY_MOVE);
			}
			theApp.m_PmacUser.DownLoadPolyMoveCoordinates(&m_ToolPath, 0);
			theApp.m_PmacUser.SetupLookAhead();
			if(theApp.m_bCollectNormalizeData != FALSE) {
				Sleep(1);
			}
			m_CpNormal.AllocateCoords(100);
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[39];

			theApp.m_bCollectNormalizeData=TRUE;
			theApp.m_PmacUser.StartPolyMoveWithLookAhead(m_ToolPath.m_nCoordL);
			m_nSubThreadAction = 20;
			Sleep(500);
		};
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_bCollectNormalizeData=FALSE;
			m_CpNormal.m_nPrimaryAxis=m_ToolPath.m_nPrimaryAxis;
			m_CpNormal.FindPeakAmplitude(&CpHead,theApp.m_LastSettings.nPeakMode[0]);

			theApp.m_Kinematics.NormalsFromTips(&CpHead,nSide);						//Done like this to ensure always looks at the same point
			m_CpSurface.Side[nSide].norm = CpHead.Side[nSide].norm;
			theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&m_CpMoveTo,nSide,TRUE,TRUE);

			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);

			NormalizeViewInvalidate(HORIZONTAL, m_CpMoveTo);

			m_nSubThreadAction = 100;
			Sleep(500);
		}
		break;

	case 100:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetHeadPos(&m_CpHead);
			theApp.m_Motors.GetSurfacePos(&m_CpSurface);
			CpCurrent = m_CpSurface;

			theApp.m_Kinematics.D3DXRotateMatrixFromNormal(m_matNormal, m_CpSurface.Side[nSide].norm);

			if(theApp.m_LastSettings.nNormalizeYtip != 1) {
				m_nSubThreadAction = 999;
				return;
			}

			vec = D3DXVECTOR3(0.0f,0.0f,1.0f);
			D3DXMatrixRotationX(&matRot,-theApp.m_LastSettings.fNormalizeAngle * DEG_TO_RAD);
			D3DXVec3TransformCoord(&vec,&vec,&matRot);
			D3DXVec3TransformCoord(&vec,&vec,&m_matNormal);
			CopyMemory(m_CpSurface.Side[nSide].norm, vec,sizeof D3DXVECTOR3);

			theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&CpHead,PORTSIDE,TRUE,TRUE,&CpCurrent);
	//		theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&CpHead,STARBOARD);
			m_CpMoveTo = CpHead;
			m_CpMoveTo.Side0.fR = m_CpSurface.Side0.fR;

			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nSubThreadAction = 110;
			Sleep(500);
		}
		break;
	case 110:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetSurfacePos(&CpSurface);
			CpCurrent = CpSurface;
			m_ToolPath.Zero();

			for(ii=0;ii<40;ii++) {
				fAngle = ((theApp.m_LastSettings.fNormalizeAngle * 2.0f * (float)ii) / 39.0f) - theApp.m_LastSettings.fNormalizeAngle;
				vec = D3DXVECTOR3(0.0f,0.0f,1.0f);
				D3DXMatrixRotationX(&matRot,fAngle * DEG_TO_RAD);
				D3DXVec3TransformCoord(&vec,&vec,&matRot);
				D3DXVec3TransformCoord(&vec,&vec,&m_matNormal);
				CopyMemory(m_CpSurface.Side[nSide].norm, vec,sizeof D3DXVECTOR3);

				theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&CpHead,PORTSIDE,TRUE,TRUE,&CpCurrent);
//				theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&CpHead,STARBOARD);
				m_ToolPath.Add(CpHead);
			}
			theApp.m_Motors.SuspendMotors();
			if(nSide==PORTSIDE) {
				m_ToolPath.SetPrimaryAxis(4);
				theApp.m_Motors.EnableAxes(LEFT_POLY_MOVE);
			} else {
				m_ToolPath.SetPrimaryAxis(14);
				theApp.m_Motors.EnableAxes(RIGHT_POLY_MOVE);
			}
			theApp.m_PmacUser.DownLoadPolyMoveCoordinates(&m_ToolPath, 0);
			theApp.m_PmacUser.SetupLookAhead();
			if(theApp.m_bCollectNormalizeData != FALSE) {
				Sleep(1);
			}
			m_CpNormal.AllocateCoords(100);
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[39];

			theApp.m_bCollectNormalizeData=TRUE;
			theApp.m_PmacUser.StartPolyMoveWithLookAhead(m_ToolPath.m_nCoordL);
			m_nSubThreadAction = 120;
			Sleep(500);
		};
		break;
	case 120:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_bCollectNormalizeData=FALSE;
			m_CpNormal.m_nPrimaryAxis=m_ToolPath.m_nPrimaryAxis;
			m_CpNormal.FindPeakAmplitude(&CpHead,theApp.m_LastSettings.nPeakMode[0]);

			theApp.m_Kinematics.NormalsFromTips(&CpHead,nSide);						//Done like this to ensure always looks at the same point
			m_CpSurface.Side[nSide].norm = CpHead.Side[nSide].norm;
			theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&m_CpMoveTo,nSide,TRUE,TRUE);

			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);

			NormalizeViewInvalidate(VERTICAL, m_CpMoveTo);

			m_nSubThreadAction = 999;
		}
		break;

	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nNormalizeStatus = 999;
			theApp.m_bCollectNormalizeData=FALSE;
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}


}

void CMainThread::NormalizeRotateTheta()
{
	int nSide,nNotSide,ii;
	float fAngle;
	CCoord CpCurrent,CpHead,CpSurface;
	D3DXMATRIXA16 matRot;
	D3DXVECTOR3 vec;

	nSide=theApp.m_PmacUser.m_nScanSide=m_nNormSide;
	nSide==PORTSIDE ? nNotSide = STARBOARD : nNotSide = PORTSIDE;


	switch(m_nSubThreadAction) {
	case 0:
		m_nNormalizeStatus = 1;
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.GetHeadPos(&m_CpHead);
		theApp.m_Motors.GetSurfacePos(&m_CpSurface);
		CpCurrent = m_CpSurface;


		if(theApp.m_LastSettings.nNormalizeXtip != 1) {
			m_nSubThreadAction = 100;
			return;
		}

		switch(theApp.m_LastSettings.nManipulatorType[m_nNormSide]) {
		case ROTATE_DIF_THETA:
		case ROTATE_THETA_VERTICAL:
			vec = D3DXVECTOR3(0.0f,0.0f,1.0f);
			theApp.m_Kinematics.D3DXRotateMatrixFromKNormal(m_matNormal, m_CpSurface.Side[nSide].norm);
			D3DXMatrixRotationY(&matRot,-theApp.m_LastSettings.fNormalizeAngle * DEG_TO_RAD);
			break;
		case ROTATE_THETA_HORIZONTAL:
			vec = D3DXVECTOR3(0.0f,-1.0f,0.0f);
			theApp.m_Kinematics.D3DXRotateMatrixFromJNormal(m_matNormal, m_CpSurface.Side[nSide].norm);
			D3DXMatrixRotationX(&matRot,-theApp.m_LastSettings.fNormalizeAngle * DEG_TO_RAD);
			break;
		}
		D3DXVec3TransformCoord(&vec,&vec,&matRot);
		D3DXVec3TransformCoord(&vec,&vec,&m_matNormal);
		CopyMemory(m_CpSurface.Side[nSide].norm, vec,sizeof D3DXVECTOR3);

		theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&CpHead,PORTSIDE,TRUE,TRUE,&CpCurrent);
//		theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&CpHead,STARBOARD,TRUE,TRUE,&CpCurrent);
		m_CpMoveTo = CpHead;
		m_CpMoveTo.Side0.fR = m_CpSurface.Side0.fR;

		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		m_nSubThreadAction = 10;
		Sleep(500);
		break;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetSurfacePos(&CpSurface);
			CpCurrent = CpSurface;
			m_ToolPath.Zero();

			for(ii=0;ii<40;ii++) {
				fAngle = ((theApp.m_LastSettings.fNormalizeAngle * 2.0f * (float)ii) / 39.0f) - theApp.m_LastSettings.fNormalizeAngle;
				switch(theApp.m_LastSettings.nManipulatorType[m_nNormSide]) {
				case ROTATE_DIF_THETA:
				case ROTATE_THETA_VERTICAL:
					vec = D3DXVECTOR3(0.0f,0.0f,1.0f);
					D3DXMatrixRotationY(&matRot,fAngle * DEG_TO_RAD);
					break;
				case ROTATE_THETA_HORIZONTAL:
					vec = D3DXVECTOR3(0.0f,-1.0f,0.0f);
					D3DXMatrixRotationX(&matRot,fAngle * DEG_TO_RAD);
					break;
				}
				D3DXVec3TransformCoord(&vec,&vec,&matRot);
				D3DXVec3TransformCoord(&vec,&vec,&m_matNormal);
				CopyMemory(m_CpSurface.Side[nSide].norm, vec,sizeof D3DXVECTOR3);

				theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&CpHead,PORTSIDE,TRUE,TRUE,&CpCurrent);
				theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&CpHead,STARBOARD,TRUE,TRUE,&CpCurrent);
				m_ToolPath.Add(CpHead);
			}
			m_bNewToolPathIfAnyoneWantsIt = true;

			theApp.m_Motors.SuspendMotors();
			if(nSide==PORTSIDE) {
				m_ToolPath.SetPrimaryAxis(6);					//sometimes 6
				theApp.m_Motors.EnableAxes(LEFT_POLY_MOVE);
			} else {
				m_ToolPath.SetPrimaryAxis(16);					//sometimes 16
				theApp.m_Motors.EnableAxes(RIGHT_POLY_MOVE);
			}
			theApp.m_PmacUser.DownLoadPolyMoveCoordinates(&m_ToolPath, 0);
			theApp.m_Motors.SetupLookAhead();
			if(theApp.m_bCollectNormalizeData != FALSE) {
				Sleep(1);
			}
			m_CpNormal.AllocateCoords(100);
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[39];

			theApp.m_bCollectNormalizeData=TRUE;
			theApp.m_PmacUser.StartPolyMoveWithLookAhead(m_ToolPath.m_nCoordL);
			m_nSubThreadAction = 20;
			Sleep(500);
		};
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_bCollectNormalizeData=FALSE;
			m_CpNormal.m_nPrimaryAxis=m_ToolPath.m_nPrimaryAxis;
			m_CpNormal.FindPeakAmplitude(&CpHead,theApp.m_LastSettings.nPeakMode[0]);

			theApp.m_Kinematics.NormalsFromTips(&CpSurface,nSide);						//Done like this to ensure always looks at the same point
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);


			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);

			NormalizeViewInvalidate(HORIZONTAL, m_CpMoveTo);

			m_nSubThreadAction = 100;
			Sleep(500);
		}
		break;

	case 100:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetHeadPos(&m_CpHead);
			theApp.m_Motors.GetSurfacePos(&m_CpSurface);
			CpCurrent = m_CpSurface;


			if(theApp.m_LastSettings.nNormalizeYtip != 1) {
				m_nSubThreadAction = 999;
				return;
			}

			switch(theApp.m_LastSettings.nManipulatorType[m_nNormSide]) {
			case ROTATE_DIF_THETA:
			case ROTATE_THETA_VERTICAL:
				vec = D3DXVECTOR3(0.0f,0.0f,1.0f);
				theApp.m_Kinematics.D3DXRotateMatrixFromKNormal(m_matNormal, m_CpSurface.Side[nSide].norm);
				D3DXMatrixRotationX(&matRot,-theApp.m_LastSettings.fNormalizeAngle * DEG_TO_RAD);
				break;
			case ROTATE_THETA_HORIZONTAL:
				vec = D3DXVECTOR3(0.0f,-1.0f,0.0f);
				theApp.m_Kinematics.D3DXRotateMatrixFromJNormal(m_matNormal, m_CpSurface.Side[nSide].norm);
				D3DXMatrixRotationZ(&matRot,-theApp.m_LastSettings.fNormalizeAngle * DEG_TO_RAD);
				break;
			}
			D3DXVec3TransformCoord(&vec,&vec,&matRot);
			D3DXVec3TransformCoord(&vec,&vec,&m_matNormal);
			CopyMemory(m_CpSurface.Side[nSide].norm, vec,sizeof D3DXVECTOR3);

			theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
			m_CpMoveTo = CpHead;
			m_CpMoveTo.Side0.fR = m_CpSurface.Side0.fR;

			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nSubThreadAction = 110;
			Sleep(500);
		}
		break;
	case 110:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetSurfacePos(&CpSurface);
			CpCurrent = CpSurface;
			m_ToolPath.Zero();

			for(ii=0;ii<40;ii++) {
				fAngle = ((theApp.m_LastSettings.fNormalizeAngle * 2.0f * (float)ii) / 39.0f) - theApp.m_LastSettings.fNormalizeAngle;
				switch(theApp.m_LastSettings.nManipulatorType[m_nNormSide]) {
				case ROTATE_DIF_THETA:
				case ROTATE_THETA_VERTICAL:
					vec = D3DXVECTOR3(0.0f,0.0f,1.0f);
					D3DXMatrixRotationX(&matRot,fAngle * DEG_TO_RAD);
					break;
				case ROTATE_THETA_HORIZONTAL:
					vec = D3DXVECTOR3(0.0f,-1.0f,0.0f);
					D3DXMatrixRotationZ(&matRot,fAngle * DEG_TO_RAD);
					break;
				}
				D3DXVec3TransformCoord(&vec,&vec,&matRot);
				D3DXVec3TransformCoord(&vec,&vec,&m_matNormal);
				CopyMemory(m_CpSurface.Side[nSide].norm, vec,sizeof D3DXVECTOR3);

				theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&CpHead,PORTSIDE,TRUE,TRUE,&CpCurrent);
				theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&CpHead,STARBOARD,TRUE,TRUE,&CpCurrent);
				m_ToolPath.Add(CpHead);
			}
			m_bNewToolPathIfAnyoneWantsIt = true;

			theApp.m_Motors.SuspendMotors();
			if(nSide==PORTSIDE) {
				m_ToolPath.SetPrimaryAxis(8);
				theApp.m_Motors.EnableAxes(LEFT_POLY_MOVE);
			} else {
				m_ToolPath.SetPrimaryAxis(18);
				theApp.m_Motors.EnableAxes(RIGHT_POLY_MOVE);
			}
			theApp.m_PmacUser.DownLoadPolyMoveCoordinates(&m_ToolPath, 0);
			theApp.m_Motors.SetupLookAhead();
			if(theApp.m_bCollectNormalizeData != FALSE) {
				Sleep(1);
			}
			m_CpNormal.AllocateCoords(100);
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[39];

			theApp.m_bCollectNormalizeData=TRUE;
			theApp.m_PmacUser.StartPolyMoveWithLookAhead(m_ToolPath.m_nCoordL);
			m_nSubThreadAction = 120;
			Sleep(500);
		};
		break;
	case 120:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_bCollectNormalizeData=FALSE;
			m_CpNormal.m_nPrimaryAxis=m_ToolPath.m_nPrimaryAxis;
			m_CpNormal.FindPeakAmplitude(&CpHead,theApp.m_LastSettings.nPeakMode[0]);

			theApp.m_Kinematics.NormalsFromTips(&CpSurface,nSide);						//Done like this to ensure always looks at the same point
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);

			NormalizeViewInvalidate(VERTICAL, m_CpMoveTo);

			m_nSubThreadAction = 999;
		}
		break;

	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nNormalizeStatus = 999;
			theApp.m_bCollectNormalizeData=FALSE;
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}


}


void CMainThread::NormalizeRobot()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	int nSide,nNotSide,ii;
	CCoord Cp;
	static CCoord CpStart,CpPeek;
	float fAngle;
	D3DXVECTOR3 vPos,vNorm;
	D3DXQUATERNION quatPos;
	D3DXMATRIXA16 matWorld,matRot;

	nSide=theApp.m_PmacUser.m_nScanSide=m_nNormSide;
	nSide==PORTSIDE ? nNotSide = STARBOARD : nNotSide = PORTSIDE;

	switch(m_nSubThreadAction) {
	case 0:
		m_nNormalizeStatus = 1;
		theApp.m_Motors.GetSurfacePos(&Cp);
		CpStart = Cp;

		if(theApp.m_LastSettings.nNormalizeXtip != 1) {
			m_nSubThreadAction = 10;
			return;
		}

		theApp.m_Ethercat.InvalidateRobotJoints();
		theApp.m_Robot[nSide].GetToolTipPos(&vPos, &quatPos);
		D3DXMatrixRotationQuaternion(&matWorld,&quatPos);

		m_ToolPath.Zero();
		for(ii=0;ii<2;ii++) {
			vNorm = D3DXVECTOR3(0.0f,0.0f,1.0f);
			fAngle = ((theApp.m_LastSettings.fNormalizeAngle * 2.0f * (float)ii) / 1.0f) - theApp.m_LastSettings.fNormalizeAngle;
			D3DXMatrixRotationX(&matRot,fAngle * DEG_TO_RAD);
			D3DXVec3TransformCoord(&vNorm,&vNorm,&matRot);
			D3DXVec3TransformCoord(&vNorm,&vNorm,&matWorld);
			CopyMemory(Cp.Side[nSide].norm, vNorm,sizeof D3DXVECTOR3);
			if (nSide == PORTSIDE) theApp.InvertNormScannerSpecific(&Cp, PORTSIDE);
//			if (nSide == STARBOARD) theApp.InvertNormScannerSpecific(&Cp, STARBOARD);

			m_ToolPath.Add(Cp);
		}
		if(nSide==PORTSIDE) {
			m_ToolPath.SetPrimaryAxis(7);
		} else {
			m_ToolPath.SetPrimaryAxis(17);
		}
		m_CpNormal.AllocateCoords(100);
		m_CpNormal.m_nPrimaryAxis=m_ToolPath.m_nPrimaryAxis;
		m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
		m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[1];

		theApp.m_bCollectNormalizeData=TRUE;
		theApp.m_FBCtrl.DoPolyLine(&m_ToolPath,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);
		m_nSubThreadAction+=10;
		break;
	case 10:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction+=10;
		}
		break;
	case 20:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {
			theApp.m_bCollectNormalizeData=FALSE;
			m_nNormPlotDirection = HORIZONTAL;
			m_CpNormal.FindPeakAmplitude(&CpPeek,theApp.m_LastSettings.nPeakMode[0]);
			CpStart.SetSide(CpPeek,nSide);
			m_ToolPath.Zero();
			m_ToolPath.Add(CpStart);
			theApp.m_FBCtrl.DoPolyLine(&m_ToolPath,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);
			NormalizeViewInvalidate(m_nNormPlotDirection, m_CpMoveTo);
			m_nSubThreadAction+=10;
		}
		break;
	case 30:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction+=10;
		}
		break;

	case 40:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {
			theApp.m_Motors.GetSurfacePos(&Cp);
			CpStart = Cp;

			if(theApp.m_LastSettings.nNormalizeYtip != 1) {
				m_nSubThreadAction = 999;
				return;
			}

			theApp.m_Ethercat.InvalidateRobotJoints();
			theApp.m_Robot[nSide].GetToolTipPos(&vPos, &quatPos);
			D3DXMatrixRotationQuaternion(&matWorld,&quatPos);

			m_ToolPath.Zero();
			for(ii=0;ii<2;ii++) {
				vNorm = D3DXVECTOR3(0.0f,0.0f,1.0f);
				fAngle = ((theApp.m_LastSettings.fNormalizeAngle * 2.0f * (float)ii) / 1.0f) - theApp.m_LastSettings.fNormalizeAngle;
				D3DXMatrixRotationY(&matRot,fAngle * DEG_TO_RAD);
				D3DXVec3TransformCoord(&vNorm,&vNorm,&matRot);
				D3DXVec3TransformCoord(&vNorm,&vNorm,&matWorld);
				CopyMemory(Cp.Side[nSide].norm, vNorm,sizeof D3DXVECTOR3);
				if (nSide == PORTSIDE) theApp.InvertNormScannerSpecific(&Cp, PORTSIDE);
//				if (nSide == STARBOARD) theApp.InvertNormScannerSpecific(&Cp, STARBOARD);

				m_ToolPath.Add(Cp);
			}
			if(nSide==PORTSIDE) {
				m_ToolPath.SetPrimaryAxis(8);
			} else {
				m_ToolPath.SetPrimaryAxis(18);
			}
			m_CpNormal.AllocateCoords(100);
			m_CpNormal.m_nPrimaryAxis=m_ToolPath.m_nPrimaryAxis;
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[1];

			theApp.m_bCollectNormalizeData=TRUE;
			theApp.m_FBCtrl.DoPolyLine(&m_ToolPath,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);
			m_nSubThreadAction+=10;
		}
		break;
	case 50:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction+=10;
		}
		break;

	case 60:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {
			theApp.m_bCollectNormalizeData=FALSE;
			m_nNormPlotDirection = VERTICAL;
			m_CpNormal.FindPeakAmplitude(&CpPeek,theApp.m_LastSettings.nPeakMode[0]);
			CpStart.SetSide(CpPeek,nSide);
			m_ToolPath.Zero();
			m_ToolPath.Add(CpStart);
			theApp.m_FBCtrl.DoPolyLine(&m_ToolPath,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);
			NormalizeViewInvalidate(m_nNormPlotDirection, m_CpMoveTo);
			m_nSubThreadAction += 10;
		}
		break;
	case 70:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction=999;
		}
		break;

	case 999:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {
			m_nNormalizeStatus = 999;
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
		}
		break;
	}

}


void CMainThread::NormalizePitchYaw()
{
	static CCoord	Cp,CpTemp;
	int	ii;
	static int nStart,nFinish;
	int	nNumberSamples = 200;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	int nSide,nNotSide;
	static int nDacEnable;

	nSide=theApp.m_PmacUser.m_nScanSide=m_nNormSide;
	nSide==PORTSIDE ? nNotSide = STARBOARD : nNotSide = PORTSIDE;

	switch(m_nSubThreadAction) {
	case 0:
		m_nNormalizeStatus = 1;

		nDacEnable = theApp.m_UtUser.m_TS[0].Pr30.nDacEnable;
		if(m_bNormDisableDAC == true) {
			theApp.m_UtUser.m_TS[0].Pr30.nDacEnable = 0;
			theApp.m_UtUser.Pr30DacEnable(&theApp.m_UtUser.m_TS[0].Pr30);
		}

		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.GetHeadPos(&m_CpHeadNorm);
		theApp.m_Kinematics.SurfaceFromHead(&m_CpHeadNorm,&m_CpSurfaceNorm,PORTSIDE,TRUE,TRUE, CURRENT_FL);
		theApp.m_Kinematics.SurfaceFromHead(&m_CpHeadNorm,&m_CpSurfaceNorm,STARBOARD,TRUE,TRUE, CURRENT_FL);
		theApp.m_Kinematics.NormalsFromTips(&m_CpSurfaceNorm,PORTSIDE);
		theApp.m_Kinematics.NormalsFromTips(&m_CpSurfaceNorm,STARBOARD);

		if(theApp.m_LastSettings.nNormalizeXtip != 1) {
			m_nSubThreadAction = 50;
			return;
		}

		m_CpSurfaceNorm.Side[nSide].fXt = m_CpHeadNorm.Side[nSide].fXt - theApp.m_LastSettings.fNormalizeAngle;
		theApp.m_Kinematics.NormalsFromTips(&m_CpSurfaceNorm,PORTSIDE);
		theApp.m_Kinematics.NormalsFromTips(&m_CpSurfaceNorm,STARBOARD);

		theApp.m_Kinematics.HeadFromSurface(&m_CpSurfaceNorm,&CpTemp,PORTSIDE,TRUE,TRUE, NULL, CURRENT_FL);
		theApp.m_Kinematics.HeadFromSurface(&m_CpSurfaceNorm,&CpTemp,STARBOARD,TRUE,TRUE, NULL, CURRENT_FL);
		CpTemp.Side[nNotSide] = m_CpHeadNorm.Side[nNotSide];
		m_CpMoveTo = CpTemp;
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		m_nSubThreadAction = 5;
		Sleep(500);
		break;
	case 5:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 10;
		};
		break;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			CpTemp = m_CpHeadNorm;
			m_ToolPath.Zero();

			for(ii=0;ii<40;ii++) {
				Cp = m_CpSurfaceNorm;
				Cp.Side[nSide].fXt = m_CpHeadNorm.Side[nSide].fXt - (float)theApp.m_LastSettings.fNormalizeAngle + ((((float)theApp.m_LastSettings.fNormalizeAngle*2.0f) * (float)ii) / 39.0f);
				theApp.m_Kinematics.NormalsFromTips(&Cp,nSide);
				theApp.m_Kinematics.HeadFromSurface(&Cp,&CpTemp,nSide,TRUE,TRUE, NULL, CURRENT_FL);
				m_ToolPath.Add(CpTemp);
			}



			theApp.m_Motors.SuspendMotors();
			if(nSide==PORTSIDE) {
				m_ToolPath.SetPrimaryAxis(3);
				theApp.m_Motors.EnableAxes(LEFT_POLY_MOVE);
			} else {
				m_ToolPath.SetPrimaryAxis(13);
				switch(theApp.m_Tank.nScannerDescription) {
				default: theApp.m_Motors.EnableAxes(RIGHT_POLY_MOVE);
					break;
				case FB_11_AXIS:
				case FB_TWIN_TOWER:
					theApp.m_Motors.EnableAxes(BOTH_POLY_MOVE);
					break;
				case TRACKED_PROBE:
					break;
				}

			}
			theApp.m_Motors.DownLoadPolyMoveCoordinates(&m_ToolPath, 0);
			theApp.m_Motors.SetupLookAhead();
			if(theApp.m_bCollectNormalizeData != FALSE) {
				Sleep(1);
			}
			m_CpNormal.AllocateCoords(100);
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[39];

		//	Sleep(100);
			theApp.m_PmacUser.m_nScanSide = nSide;
			theApp.m_bCollectNormalizeData=TRUE;
			theApp.m_Motors.StartPolyMoveWithLookAhead(m_ToolPath.m_nCoordL);
			m_nSubThreadAction = 15;
			Sleep(500);
		}
		break;
	case 15:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 20;
		};
		break;

	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nNormalizeStatus=20;
			theApp.m_Motors.SuspendMotors();
			theApp.m_bCollectNormalizeData=FALSE;
			if(nSide==PORTSIDE) {
				m_CpNormal.m_nPrimaryAxis=3;
			} else {
				m_CpNormal.m_nPrimaryAxis=13;
			}
			m_CpNormal.FindPeakAmplitude(&CpTemp,theApp.m_LastSettings.nPeakMode[0]);

			if(theApp.m_LastSettings.nManipulatorType[0]!=INDEX_FINGER) {  //Wrist

				if(fabsf( m_CpHeadNorm.Side[nSide].fXt - CpTemp.Side[nSide].fXt ) <= theApp.m_LastSettings.fNormalizeAngle) {
					m_CpSurfaceNorm.Side[nSide].fXt = CpTemp.Side[nSide].fXt;
				};
				m_CpSurfaceNorm.Side[nSide].fYt = m_CpHeadNorm.Side[nSide].fYt;
				if(theApp.m_LastSettings.nNormalizeYtip == 1) {
					m_CpSurfaceNorm.Side[nSide].fYt -= theApp.m_LastSettings.fNormalizeAngle;
				}
				theApp.m_Kinematics.NormalsFromTips(&m_CpSurfaceNorm,PORTSIDE);
				theApp.m_Kinematics.NormalsFromTips(&m_CpSurfaceNorm,STARBOARD);
				theApp.m_Kinematics.HeadFromSurface(&m_CpSurfaceNorm,&CpTemp,PORTSIDE,TRUE,TRUE, NULL, CURRENT_FL);
				theApp.m_Kinematics.HeadFromSurface(&m_CpSurfaceNorm,&CpTemp,STARBOARD,TRUE,TRUE, NULL, CURRENT_FL);
				m_CpMoveTo.Side[nSide] = CpTemp.Side[nSide];
			} else {
				m_CpMoveTo.Side[nSide] = CpTemp.Side[nSide];
			}

			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);

			NormalizeViewInvalidate(HORIZONTAL, m_CpMoveTo);

			if(theApp.m_LastSettings.nManipulatorType[0]==INDEX_FINGER) {  //Finger
				m_nSubThreadAction = 998;
			} else {
				Sleep(500);
				m_nSubThreadAction = 40;
			}
		}
		break;
	case 40:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 50;
		};
		break;

	case 50:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			if(theApp.m_LastSettings.nNormalizeYtip != 1) {
				m_nSubThreadAction = 999;
				return;
			}
			CpTemp = m_CpHeadNorm;
			m_ToolPath.Zero();

			for(ii=0;ii<40;ii++) {
				Cp = m_CpSurfaceNorm;
				Cp.Side[nSide].fYt = m_CpHeadNorm.Side[nSide].fYt - (float)theApp.m_LastSettings.fNormalizeAngle + ((((float)theApp.m_LastSettings.fNormalizeAngle*2.0f) * (float)ii) / 39.0f);
				theApp.m_Kinematics.NormalsFromTips(&Cp,nSide);
				theApp.m_Kinematics.HeadFromSurface(&Cp,&CpTemp,nSide,TRUE,TRUE, NULL, CURRENT_FL);
				m_ToolPath.Add(CpTemp);
			}



			theApp.m_Motors.SuspendMotors();
			if(nSide==PORTSIDE) {
				m_ToolPath.SetPrimaryAxis(4);
				theApp.m_Motors.EnableAxes(LEFT_POLY_MOVE);
			} else {
				m_ToolPath.SetPrimaryAxis(14);
				switch(theApp.m_Tank.nScannerDescription) {
				default: theApp.m_Motors.EnableAxes(RIGHT_POLY_MOVE);
					break;
				case FB_TWIN_TOWER:
				case FB_11_AXIS: theApp.m_Motors.EnableAxes(BOTH_POLY_MOVE);
					break;
				case TRACKED_PROBE:
					break;
				}
			}
			theApp.m_Motors.DownLoadPolyMoveCoordinates(&m_ToolPath, 0);
			theApp.m_Motors.SetupLookAhead();
			if(theApp.m_bCollectNormalizeData != FALSE) {
				Sleep(1);
			}
			m_CpNormal.AllocateCoords(100);
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[39];

		//	Sleep(100);

			theApp.m_PmacUser.m_nScanSide = nSide;
			theApp.m_Motors.CollectNormalizeData(true,m_ToolPath.m_nPrimaryAxis);
			theApp.m_Motors.StartPolyMoveWithLookAhead(m_ToolPath.m_nCoordL);
			m_nSubThreadAction = 55;
			Sleep(500);
		}
		break;
	case 55:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 60;
		};
		break;
	case 60:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nNormalizeStatus = 60;
			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.CollectNormalizeData(false);
			if(nSide==PORTSIDE) {
				m_CpNormal.m_nPrimaryAxis=4;
			} else {
				m_CpNormal.m_nPrimaryAxis=14;
			}
			m_CpNormal.FindPeakAmplitude(&CpTemp,theApp.m_LastSettings.nPeakMode[0]);
			if(fabsf( m_CpHeadNorm.Side[nSide].fXt - CpTemp.Side[nSide].fXt ) <= theApp.m_LastSettings.fNormalizeAngle) {
				m_CpSurfaceNorm.Side[nSide].fXt = CpTemp.Side[nSide].fXt;
			}
			if(fabsf( m_CpHeadNorm.Side[nSide].fYt - CpTemp.Side[nSide].fYt ) <= theApp.m_LastSettings.fNormalizeAngle) {
				m_CpSurfaceNorm.Side[nSide].fYt = CpTemp.Side[nSide].fYt;
			}
			theApp.m_Kinematics.NormalsFromTips(&m_CpSurfaceNorm,PORTSIDE);
			theApp.m_Kinematics.NormalsFromTips(&m_CpSurfaceNorm,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&m_CpSurfaceNorm,&CpTemp,PORTSIDE,TRUE,TRUE, NULL, CURRENT_FL);
			theApp.m_Kinematics.HeadFromSurface(&m_CpSurfaceNorm,&CpTemp,STARBOARD,TRUE,TRUE, NULL, CURRENT_FL);
			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
			m_CpMoveTo.Side[nSide] = CpTemp.Side[nSide];
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);

			NormalizeViewInvalidate(VERTICAL, m_CpMoveTo);

			m_nSubThreadAction = 998;
		}
		break;
	case 998:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 999;
		};
		break;

	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.CollectNormalizeData(false);

			if(m_bNormDisableDAC == true) {
				theApp.m_UtUser.m_TS[0].Pr30.nDacEnable = nDacEnable;
				theApp.m_UtUser.Pr30DacEnable(&theApp.m_UtUser.m_TS[0].Pr30);
			}

			m_nNormalizeStatus = 999;
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}


}



void CMainThread::PeakOpposite()
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_LastSettings.nPeakOrthogonalMode) {
		case 0:	//PeakOppositeXyThenZy();
			PeakOppositeZyThenXy();
			break;
		case 1: RectilinearScan();
			break;
		}
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		PeakOppositeRobot();
		break;
	case TWIN_TOWER_KINEMATIC:
	case TANK_6AXIS:
		PeakOppositeKinematic();
		break;
	case TRACKED_PROBE:
		break;
	}


}

void CMainThread::PeakOppositeRobot()
{
	CCoord	Cp,CpSurface;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	int nNotSide,nSide;
	int	nYAxis,nZAxis,nXAxis;
	float fDistance = theApp.m_LastSettings.fPeakDistance;
	static CCoord CpStart,CpPeek;
	CPolyCoord Line;

	float fWidth = -1.0f;
	float fHeight = -1.0f;
	fWidth = theApp.m_LastSettings.fPeakDistance;
	fHeight = theApp.m_LastSettings.fPeakDistance;

	D3DXMATRIXA16 matWorld,matNormal,matTranslation;
	D3DXVECTOR3 vecP,vPos;
	D3DXQUATERNION quatPos;

	nSide=theApp.m_PmacUser.m_nScanSide=m_nNormSide;
	nSide==PORTSIDE ? nNotSide = STARBOARD : nNotSide = PORTSIDE;

	nXAxis = theApp.m_Tank.nXRight;
	nYAxis = theApp.m_Tank.nYRight;
	nZAxis = theApp.m_Tank.nZRight;


	switch(m_nSubThreadAction) {
	case 0:
		m_nPeakOppositeStatus = 1;
		theApp.m_Motors.GetSurfacePos(&Cp);
		CpStart = Cp;

		theApp.m_Ethercat.InvalidateRobotJoints();
		theApp.m_Robot[nSide].GetToolTipPos(&vPos, &quatPos);
		D3DXMatrixRotationQuaternion(&matNormal,&quatPos);
		D3DXMatrixTranslation(&matTranslation,Cp.Side[nSide].fX,Cp.Side[nSide].fY,Cp.Side[nSide].fZ);
		D3DXMatrixMultiply(&matWorld,&matNormal,&matTranslation);

		m_CpRect[0] = Cp;
		vecP.x = 0.0f;
		vecP.y = fHeight / -2.0f;
		vecP.z = 0.0f;
		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
		m_CpRect[0].Side[nSide].fX = vecP.x;
		m_CpRect[0].Side[nSide].fY = vecP.y;
		m_CpRect[0].Side[nSide].fZ = vecP.z;

		m_CpRect[1] = Cp;
		vecP.x = 0.0f;
		vecP.y = fHeight / 2.0f;
		vecP.z = 0.0f;
		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
		m_CpRect[1].Side[nSide].fX = vecP.x;
		m_CpRect[1].Side[nSide].fY = vecP.y;
		m_CpRect[1].Side[nSide].fZ = vecP.z;

		m_ToolPath.Zero();
		m_ToolPath.Add(m_CpRect[0]);
		m_ToolPath.Add(m_CpRect[1]);

		m_CpNormal.AllocateCoords(100);
		m_CpNormal.ClearMemory();
		if(nSide==STARBOARD) {
			if(fabs(m_CpRect[1].Side1.fZ - m_CpRect[0].Side1.fZ) > fabs(m_CpRect[1].Side1.fY - m_CpRect[0].Side1.fY)) {
				m_ToolPath.SetPrimaryAxis(12);	//zRight
				m_CpNormal.SetPrimaryAxis(12);
			} else {
				m_ToolPath.SetPrimaryAxis(11);	//yRight
				m_CpNormal.SetPrimaryAxis(11);
			}
		} else {
			if(fabs(m_CpRect[1].Side0.fZ - m_CpRect[0].Side0.fZ) > fabs(m_CpRect[1].Side0.fY - m_CpRect[0].Side0.fY)) {
				m_ToolPath.SetPrimaryAxis(2);	//zLeft
				m_CpNormal.SetPrimaryAxis(2);
			} else {
				m_ToolPath.SetPrimaryAxis(1);	//yLeft
				m_CpNormal.SetPrimaryAxis(1);
			}
		}
		m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
		m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[1];
		theApp.m_bCollectNormalizeData=TRUE;
		theApp.m_FBCtrl.DoPolyLine(&m_ToolPath,theApp.m_LastSettings.fPeekOrthogonalSpeed,theApp.m_LastSettings.fPeekOrthogonalSpeed);

		m_nSubThreadAction = 5;
		break;
	case 5:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 10;
		}
		break;
	case 10:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {
			theApp.m_bCollectNormalizeData=FALSE;
			m_CpNormal.FindPeakAmplitude(&CpPeek,theApp.m_LastSettings.nPeakMode[0]);

			NormalizeViewInvalidate(m_nNormPlotDirection = HORIZONTAL, m_CpMoveTo);

			CpStart.SetSide(CpPeek,nSide);

			Cp = CpStart;

			theApp.m_Ethercat.InvalidateRobotJoints();
			theApp.m_Robot[nSide].GetToolTipPos(&vPos, &quatPos);
			D3DXMatrixRotationQuaternion(&matNormal,&quatPos);
			D3DXMatrixTranslation(&matTranslation,Cp.Side[nSide].fX,Cp.Side[nSide].fY,Cp.Side[nSide].fZ);
			D3DXMatrixMultiply(&matWorld,&matNormal,&matTranslation);

			m_CpRect[0] = Cp;
			vecP.x = fHeight / -2.0f;
			vecP.y = 0.0f;
			vecP.z = 0.0f;
			D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
			m_CpRect[0].Side[nSide].fX = vecP.x;
			m_CpRect[0].Side[nSide].fY = vecP.y;
			m_CpRect[0].Side[nSide].fZ = vecP.z;

			m_CpRect[1] = Cp;
			vecP.x = fHeight / 2.0f;
			vecP.y = 0.0f;
			vecP.z = 0.0f;
			D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
			m_CpRect[1].Side[nSide].fX = vecP.x;
			m_CpRect[1].Side[nSide].fY = vecP.y;
			m_CpRect[1].Side[nSide].fZ = vecP.z;

			m_ToolPath.Zero();
			m_ToolPath.Add(m_CpRect[0]);
			m_ToolPath.Add(m_CpRect[1]);

			m_CpNormal.AllocateCoords(100);
			m_CpNormal.ClearMemory();
			if(nSide==STARBOARD) {
				if(fabs(m_CpRect[1].Side1.fZ - m_CpRect[0].Side1.fZ) > fabs(m_CpRect[1].Side1.fY - m_CpRect[0].Side1.fY)) {
					m_ToolPath.SetPrimaryAxis(12);	//zRight
					m_CpNormal.SetPrimaryAxis(12);
				} else {
					m_ToolPath.SetPrimaryAxis(11);	//yRight
					m_CpNormal.SetPrimaryAxis(11);
				}
			} else {
				if(fabs(m_CpRect[1].Side0.fZ - m_CpRect[0].Side0.fZ) > fabs(m_CpRect[1].Side0.fY - m_CpRect[0].Side0.fY)) {
					m_ToolPath.SetPrimaryAxis(2);	//zLeft
					m_CpNormal.SetPrimaryAxis(2);
				} else {
					m_ToolPath.SetPrimaryAxis(1);	//yLeft
					m_CpNormal.SetPrimaryAxis(1);
				}
			}
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[1];
			theApp.m_bCollectNormalizeData=TRUE;
			theApp.m_FBCtrl.DoPolyLine(&m_ToolPath,theApp.m_LastSettings.fPeekOrthogonalSpeed,theApp.m_LastSettings.fPeekOrthogonalSpeed);

			m_nSubThreadAction = 25;
		}
		break;
	case 25:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 30;
		}
		break;
	case 30:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {
			theApp.m_bCollectNormalizeData=FALSE;
			m_CpNormal.FindPeakAmplitude(&CpPeek,theApp.m_LastSettings.nPeakMode[0]);

			NormalizeViewInvalidate(m_nNormPlotDirection = VERTICAL, m_CpMoveTo);

			CpStart.SetSide(CpPeek,nSide);

			Line.Zero();
			Line.Add(CpStart);
			theApp.m_FBCtrl.DoPolyLine(&Line,theApp.m_LastSettings.fPeekOrthogonalSpeed,theApp.m_LastSettings.fPeekOrthogonalSpeed);

			m_nSubThreadAction = 995;
		}
		break;
	case 995:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 999;
		}
		break;
	case 999:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {
			m_nPeakOppositeStatus = 999;
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
		}
		break;
	}


}

void CMainThread::PeakOppositeZyThenXy()
{
	CCoord	Cp,CpSurface,CpHead,CpTemp;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	int nNotSide,nSide;
	int	nYAxis,nZAxis,nXAxis;
	float fDistance = theApp.m_LastSettings.fPeakDistance;
	static CCoord CpStart;

	float fWidth = -1.0f;
	float fHeight = -1.0f;
	fWidth = theApp.m_LastSettings.fPeakDistance;
	fHeight = theApp.m_LastSettings.fPeakDistance;

	D3DXMATRIXA16 matWorld,matNormal,matTranslation;
	D3DXVECTOR3 vecP;

	nSide=theApp.m_PmacUser.m_nScanSide=m_nNormSide;
	nSide==PORTSIDE ? nNotSide = STARBOARD : nNotSide = PORTSIDE;

	nXAxis = theApp.m_Tank.nXRight;
	nYAxis = theApp.m_Tank.nYRight;
	nZAxis = theApp.m_Tank.nZRight;


	switch(m_nSubThreadAction) {
	case 0:
		m_nPeakOppositeStatus = 1;
		theApp.m_Motors.GetHeadPos(&CpHead);
		CpStart=CpHead;

		CpTemp = CpHead;

		theApp.m_Kinematics.NormalsFromTips(&CpHead,PORTSIDE);
		theApp.m_Kinematics.NormalsFromTips(&CpHead,STARBOARD);

		D3DXMatrixTranslation(&matTranslation,CpHead.Side[nSide].fX,CpHead.Side[nSide].fZ,CpHead.Side[nSide].fY);
		D3DXMatrixRotationYawPitchRoll(&matNormal,CpHead.Side[nSide].fI * PIf / -2.0f,CpHead.Side[nSide].fK * PIf / 2.0f,0.0f);
		D3DXMatrixMultiply(&matWorld,&matNormal,&matTranslation);

		m_CpRect[0] = CpHead;
		vecP.x = 0.0f;
		vecP.y = fHeight / -2.0f;
		vecP.z = 0.0f;
		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
		m_CpRect[0].Side[nSide].fX = vecP.x;
		m_CpRect[0].Side[nSide].fY = vecP.z;
		m_CpRect[0].Side[nSide].fZ = vecP.y;

		m_CpRect[1] = CpHead;
		vecP.x = 0.0f;
		vecP.y = fHeight / 2.0f;
		vecP.z = 0.0f;
		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
		m_CpRect[1].Side[nSide].fX = vecP.x;
		m_CpRect[1].Side[nSide].fY = vecP.z;
		m_CpRect[1].Side[nSide].fZ = vecP.y;


		m_CpMoveTo = m_CpRect[0];
		theApp.m_Motors.GetTurntablePos(&m_CpMoveTo);
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo,PEAK_OPTOMISE);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		m_nSubThreadAction = 5;
		Sleep(500);
		break;
	case 5:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 10;
		}
		break;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetHeadPos(&CpHead);
			m_ToolPath.Zero();
			m_ToolPath.Add(CpHead);

			m_CpMoveTo = m_CpRect[1];
			m_ToolPath.Add(m_CpMoveTo);

			m_CpNormal.AllocateCoords(100);
			m_CpNormal.ClearMemory();
			if(nSide==STARBOARD) {
				if(fabs(m_CpRect[1].Side1.fZ - m_CpRect[0].Side1.fZ) > fabs(m_CpRect[1].Side1.fY - m_CpRect[0].Side1.fY)) {
					m_ToolPath.SetPrimaryAxis(12);	//zRight
					m_CpNormal.SetPrimaryAxis(12);
				} else {
					m_ToolPath.SetPrimaryAxis(11);	//yRight
					m_CpNormal.SetPrimaryAxis(11);
				}
			} else {
				if(fabs(m_CpRect[1].Side0.fZ - m_CpRect[0].Side0.fZ) > fabs(m_CpRect[1].Side0.fY - m_CpRect[0].Side0.fY)) {
					m_ToolPath.SetPrimaryAxis(2);	//zLeft
					m_CpNormal.SetPrimaryAxis(2);
				} else {
					m_ToolPath.SetPrimaryAxis(1);	//yLeft
					m_CpNormal.SetPrimaryAxis(1);
				}
			}
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[1];
			m_CpNormal.m_pCp[0].nAmp=m_CpNormal.m_pCp[99].nAmp=0;
			theApp.m_bCollectNormalizeData=TRUE;

			m_CpMoveTo.SetSide(CpStart,nNotSide);
			m_CpMoveTo.Side[nSide].fXt = CpStart.Side[nSide].fXt;
			m_CpMoveTo.Side[nSide].fYt = CpStart.Side[nSide].fYt;
			theApp.m_Motors.GetTurntablePos(&m_CpMoveTo);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo,PEAK_OPTOMISE);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(500);
			m_nSubThreadAction = 15;
		}
		break;
	case 15:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 20;
		}
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_bCollectNormalizeData=FALSE;
			m_CpNormal.FindPeakAmplitude(&CpHead,theApp.m_LastSettings.nPeakMode[0]);

			NormalizeViewInvalidate(VERTICAL, CpHead);
/*
			fXtAngle=CpStart.Side[nSide].fXt*DEG_TO_RAD;
			fCosXtip = (float)cos(fXtAngle);
			fSinXtip = (float)sin(fXtAngle);
			fXxtip = fCosXtip * fDistance;
			fYxtip = fSinXtip * fDistance;

			CpHead.Side[nSide].fX -= fXxtip;
			CpHead.Side[nSide].fY += fYxtip;

			m_CpMoveTo=CpHead;
			m_CpMoveTo.SetSide(CpStart,nNotSide);
			m_CpMoveTo.Side[nSide].fXt = CpStart.Side[nSide].fXt;
			m_CpMoveTo.Side[nSide].fYt = CpStart.Side[nSide].fYt;
*/
			theApp.m_Kinematics.NormalsFromTips(&CpHead,PORTSIDE);
			theApp.m_Kinematics.NormalsFromTips(&CpHead,STARBOARD);

		D3DXMatrixTranslation(&matTranslation,CpHead.Side[nSide].fX,CpHead.Side[nSide].fZ,CpHead.Side[nSide].fY);
		D3DXMatrixRotationYawPitchRoll(&matNormal,CpHead.Side[nSide].fI * PIf / -2.0f,CpHead.Side[nSide].fK * PIf / 2.0f,0.0f);
		D3DXMatrixMultiply(&matWorld,&matNormal,&matTranslation);

			m_CpRect[2] = CpHead;
			vecP.x = fWidth / -2.0f;
			vecP.y = 0.0f;
			vecP.z = 0.0f;
			D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
			m_CpRect[2].Side[nSide].fX = vecP.x;
			m_CpRect[2].Side[nSide].fY = vecP.z;
			m_CpRect[2].Side[nSide].fZ = vecP.y;

			m_CpRect[3] = CpHead;
			vecP.x = fWidth / 2.0f;
			vecP.y = 0.0f;
			vecP.z = 0.0f;
			D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
			m_CpRect[3].Side[nSide].fX = vecP.x;
			m_CpRect[3].Side[nSide].fY = vecP.z;
			m_CpRect[3].Side[nSide].fZ = vecP.y;

			m_CpMoveTo = m_CpRect[2];
			theApp.m_Motors.GetTurntablePos(&m_CpMoveTo);
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo,PEAK_OPTOMISE);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nSubThreadAction = 25;
			Sleep(500);
		}
		break;
	case 25:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 30;
		}
		break;
	case 30:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetHeadPos(&CpHead);
			m_ToolPath.Zero();
			m_ToolPath.Add(CpHead);

//			m_CpMoveTo.Side[nSide].fY = CpHead.Side[nSide].fY -  (fYxtip*2.0f);
//			m_CpMoveTo.Side[nSide].fX = CpHead.Side[nSide].fX +  (fXxtip*2.0f);

			m_CpMoveTo = m_CpRect[3];
			m_ToolPath.Add(m_CpMoveTo);

			m_CpNormal.AllocateCoords(100);
			m_CpNormal.ClearMemory();
			if(nSide==STARBOARD) {
				if(fabs(m_CpRect[3].Side1.fY - m_CpRect[2].Side1.fY) > fabs(m_CpRect[3].Side1.fX - m_CpRect[2].Side1.fX)) {
					m_ToolPath.SetPrimaryAxis(11);	//yRight
					m_CpNormal.SetPrimaryAxis(11);
				} else {
					m_ToolPath.SetPrimaryAxis(10);	//xRight
					m_CpNormal.SetPrimaryAxis(10);
				}
			} else {
				if(fabs(m_CpRect[3].Side0.fZ - m_CpRect[2].Side0.fZ) > fabs(m_CpRect[3].Side0.fX - m_CpRect[2].Side0.fX)) {
					m_ToolPath.SetPrimaryAxis(2);	//ZLeft
					m_CpNormal.SetPrimaryAxis(2);
				} else {
					m_ToolPath.SetPrimaryAxis(0);	//xLeft
					m_CpNormal.SetPrimaryAxis(0);
				}
			}
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[1];
			m_CpNormal.m_pCp[0].nAmp=m_CpNormal.m_pCp[99].nAmp=0;
			theApp.m_bCollectNormalizeData=TRUE;

			m_CpMoveTo.SetSide(CpStart,nNotSide);
			m_CpMoveTo.Side[nSide].fXt = CpStart.Side[nSide].fXt;
			m_CpMoveTo.Side[nSide].fYt = CpStart.Side[nSide].fYt;
			theApp.m_Motors.GetTurntablePos(&m_CpMoveTo);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo,PEAK_OPTOMISE);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
			m_nSubThreadAction = 35;
			Sleep(500);
		}
		break;
	case 35:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 40;
		}
		break;
	case 40:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_bCollectNormalizeData=FALSE;
			m_CpNormal.FindPeakAmplitude(&m_CpMoveTo,theApp.m_LastSettings.nPeakMode[0]);

			NormalizeViewInvalidate(HORIZONTAL, m_CpMoveTo);

			m_CpMoveTo.SetSide(CpStart,nNotSide);
			m_CpMoveTo.Side[nSide].fXt = CpStart.Side[nSide].fXt;
			m_CpMoveTo.Side[nSide].fYt = CpStart.Side[nSide].fYt;
			theApp.m_Motors.GetTurntablePos(&m_CpMoveTo);
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo,PEAK_OPTOMISE);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nSubThreadAction = 998;
			Sleep(500);
		}
		break;
	case 998:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 999;
		}
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_bCollectNormalizeData=FALSE;
			m_nPeakOppositeStatus = 999;
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}


}

void CMainThread::PeakOppositeXyThenZy()
{
	CCoord	Cp,CpSurface,CpHead,CpTemp;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	int nNotSide,nSide;
	int	nYAxis,nZAxis,nXAxis;
	float fDistance = theApp.m_LastSettings.fPeakDistance;
	static CCoord CpStart;

	float fWidth = -1.0f;
	float fHeight = -1.0f;
	fWidth = theApp.m_LastSettings.fPeakDistance;
	fHeight = theApp.m_LastSettings.fPeakDistance;

	nSide=theApp.m_PmacUser.m_nScanSide=m_nNormSide;
	nSide==PORTSIDE ? nNotSide = STARBOARD : nNotSide = PORTSIDE;

	nXAxis = theApp.m_Tank.nXRight;
	nYAxis = theApp.m_Tank.nYRight;
	nZAxis = theApp.m_Tank.nZRight;


	D3DXMATRIXA16 matWorld,matNormal,matTranslation;
	D3DXVECTOR3 vecP;

	switch(m_nSubThreadAction) {
	case 0:
		m_nPeakOppositeStatus = 1;
		theApp.m_Motors.GetHeadPos(&CpHead);
		theApp.m_Kinematics.NormalsFromTips(&CpHead,PORTSIDE);
		theApp.m_Kinematics.NormalsFromTips(&CpHead,STARBOARD);
		CpStart=CpHead;

		theApp.m_Kinematics.D3DXRotateMatrixFromKNormal(matNormal, CpHead.Side[nSide].norm);
		D3DXMatrixTranslation(&matTranslation,CpHead.Side[nSide].fX,CpHead.Side[nSide].fY,CpHead.Side[nSide].fZ);
		D3DXMatrixMultiply(&matWorld,&matNormal,&matTranslation);

		m_CpRect[0] = CpHead;
		vecP.x = fHeight / -2.0f;
		vecP.y = 0.0f;
		vecP.z = 0.0f;
		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
		m_CpRect[0].Side[nSide].fX = vecP.x;
		m_CpRect[0].Side[nSide].fY = vecP.y;
		m_CpRect[0].Side[nSide].fZ = vecP.z;

		m_CpRect[1] = CpHead;
		vecP.x = fHeight / 2.0f;
		vecP.y = 0.0f;
		vecP.z = 0.0f;
		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
		m_CpRect[1].Side[nSide].fX = vecP.x;
		m_CpRect[1].Side[nSide].fY = vecP.y;
		m_CpRect[1].Side[nSide].fZ = vecP.z;

		m_CpMoveTo = m_CpRect[0];
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo,PEAK_OPTOMISE);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		m_nSubThreadAction = 10;
//		Sleep(500);
		break;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetHeadPos(&CpHead);
			m_ToolPath.Zero();
			m_ToolPath.Add(CpHead);

			m_CpMoveTo = m_CpRect[1];
			m_ToolPath.Add(m_CpMoveTo);

			m_CpNormal.AllocateCoords(100);
			m_CpNormal.ClearMemory();
			if(nSide==STARBOARD) {
				if(fabs(m_CpRect[1].Side1.fY - m_CpRect[0].Side1.fY) > fabs(m_CpRect[1].Side1.fX - m_CpRect[0].Side1.fX)) {
					m_ToolPath.SetPrimaryAxis(11);	//zRight
					m_CpNormal.SetPrimaryAxis(11);
				} else {
					m_ToolPath.SetPrimaryAxis(10);	//yRight
					m_CpNormal.SetPrimaryAxis(10);
				}
			} else {
				if(fabs(m_CpRect[1].Side0.fY - m_CpRect[0].Side0.fY) > fabs(m_CpRect[1].Side0.fX - m_CpRect[0].Side0.fX)) {
					m_ToolPath.SetPrimaryAxis(1);
					m_CpNormal.SetPrimaryAxis(1);
				} else {
					m_ToolPath.SetPrimaryAxis(0);
					m_CpNormal.SetPrimaryAxis(0);
				}
			}
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[1];
			m_CpNormal.m_pCp[0].nAmp=m_CpNormal.m_pCp[99].nAmp=0;
			theApp.m_bCollectNormalizeData=TRUE;

			m_CpMoveTo.SetSide(CpStart,nNotSide);
			m_CpMoveTo.Side[nSide].fXt = CpStart.Side[nSide].fXt;
			m_CpMoveTo.Side[nSide].fYt = CpStart.Side[nSide].fYt;
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo,PEAK_OPTOMISE);
			theApp.m_Motors.MoveTo(DONT_WAIT);
//			Sleep(2500);
			m_nSubThreadAction = 20;
		}
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_bCollectNormalizeData=FALSE;
			m_CpNormal.FindPeakAmplitude(&CpHead,theApp.m_LastSettings.nPeakMode[0]);

			NormalizeViewInvalidate(HORIZONTAL, CpHead);

			theApp.m_Kinematics.NormalsFromTips(&CpHead,PORTSIDE);
			theApp.m_Kinematics.NormalsFromTips(&CpHead,STARBOARD);

			theApp.m_Kinematics.D3DXRotateMatrixFromKNormal(matNormal, CpHead.Side[nSide].norm);
			D3DXMatrixTranslation(&matTranslation,CpHead.Side[nSide].fX,CpHead.Side[nSide].fY,CpHead.Side[nSide].fZ);
			D3DXMatrixMultiply(&matWorld,&matNormal,&matTranslation);

			m_CpRect[2] = CpHead;
			vecP.x = 0.0f;
			vecP.y = fHeight / -2.0f;
			vecP.z = 0.0f;
			D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
			m_CpRect[2].Side[nSide].fX = vecP.x;
			m_CpRect[2].Side[nSide].fY = vecP.y;
			m_CpRect[2].Side[nSide].fZ = vecP.z;

			m_CpRect[3] = CpHead;
			vecP.x = 0.0f;
			vecP.y = fHeight / 2.0f;
			vecP.z = 0.0f;
			D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
			m_CpRect[3].Side[nSide].fX = vecP.x;
			m_CpRect[3].Side[nSide].fY = vecP.y;
			m_CpRect[3].Side[nSide].fZ = vecP.z;

			m_CpMoveTo = m_CpRect[2];
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo,PEAK_OPTOMISE);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nSubThreadAction = 30;
//			Sleep(500);
		}
		break;
	case 30:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetHeadPos(&CpHead);
			m_ToolPath.Zero();
			m_ToolPath.Add(CpHead);

			m_CpMoveTo = m_CpRect[3];
			m_ToolPath.Add(m_CpMoveTo);

			m_CpNormal.AllocateCoords(100);
			m_CpNormal.ClearMemory();
			if(nSide==STARBOARD) {
				if(fabs(m_CpRect[3].Side1.fZ - m_CpRect[2].Side1.fZ) > fabs(m_CpRect[3].Side1.fY - m_CpRect[2].Side1.fY)) {
					m_ToolPath.SetPrimaryAxis(12);
					m_CpNormal.SetPrimaryAxis(12);
				} else {
					m_ToolPath.SetPrimaryAxis(11);
					m_CpNormal.SetPrimaryAxis(11);
				}
			} else {
				if(fabs(m_CpRect[3].Side0.fZ - m_CpRect[2].Side0.fZ) > fabs(m_CpRect[3].Side0.fY - m_CpRect[2].Side0.fY)) {
					m_ToolPath.SetPrimaryAxis(2);
					m_CpNormal.SetPrimaryAxis(2);
				} else {
					m_ToolPath.SetPrimaryAxis(1);
					m_CpNormal.SetPrimaryAxis(1);
				}
			}
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[1];
			m_CpNormal.m_pCp[0].nAmp=m_CpNormal.m_pCp[99].nAmp=0;
			theApp.m_bCollectNormalizeData=TRUE;

			m_CpMoveTo.SetSide(CpStart,nNotSide);
			m_CpMoveTo.Side[nSide].fXt = CpStart.Side[nSide].fXt;
			m_CpMoveTo.Side[nSide].fYt = CpStart.Side[nSide].fYt;
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo,PEAK_OPTOMISE);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			theApp.m_ServoUser.MoveTo(DONT_WAIT);
			m_nSubThreadAction = 40;
//			Sleep(500);
		}
		break;
	case 40:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_bCollectNormalizeData=FALSE;
			m_CpNormal.FindPeakAmplitude(&m_CpMoveTo,theApp.m_LastSettings.nPeakMode[0]);

			NormalizeViewInvalidate(HORIZONTAL, m_CpMoveTo);

			m_CpMoveTo.SetSide(CpStart,nNotSide);
			m_CpMoveTo.Side[nSide].fXt = CpStart.Side[nSide].fXt;
			m_CpMoveTo.Side[nSide].fYt = CpStart.Side[nSide].fYt;
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo,PEAK_OPTOMISE);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nSubThreadAction = 999;
//			Sleep(500);
		}
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nPeakOppositeStatus = 999;
			theApp.m_bCollectNormalizeData=FALSE;
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}
}



void CMainThread::TuningMove()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.EnableAxes(TUNNING);
		m_nTuningPtr=0;
		if(m_CpTunning.m_pCp == NULL) {
			m_CpTunning.AllocateCoords(60000);
		}
		theApp.m_PmacUser.TuningMove();
		m_nSubThreadAction=10;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_bCollectTunning=FALSE;
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			m_nSubThreadAction=0;
			pFrame->m_pPmacSheet->Invalidate(FALSE);
		}
		break;
	}

}

void CMainThread::CollectTunning()
{
	CCoord Cp;

	Cp.fTunningPos=(float)theApp.m_Axes[theApp.m_PmacUser.m_nTunningAxis].nPos * theApp.m_Axes[theApp.m_PmacUser.m_nTunningAxis].fStepSize;
	Cp.fTunningFollowingError=(float)theApp.m_Axes[theApp.m_PmacUser.m_nTunningAxis].nFollowingError  * theApp.m_Axes[theApp.m_PmacUser.m_nTunningAxis].fStepSize;
	Cp.nTunningFollowingError=theApp.m_Axes[theApp.m_PmacUser.m_nTunningAxis].nFollowingError;
	Cp.fTunningDAC=(float)theApp.m_Axes[theApp.m_PmacUser.m_nTunningAxis].nDAC * 10.0f / 20480.0f;
	Cp.nTunningDAC=theApp.m_Axes[theApp.m_PmacUser.m_nTunningAxis].nDAC;
	Cp.nVelocity=theApp.m_Axes[theApp.m_PmacUser.m_nTunningAxis].nCurrentSpeed;
	Cp.fVelocity=((float)theApp.m_Axes[theApp.m_PmacUser.m_nTunningAxis].nCurrentSpeed*theApp.m_Axes[theApp.m_PmacUser.m_nTunningAxis].fStepSize)/1.360f;

	m_CpTunning.m_pCp[m_nTuningPtr] = Cp;
	if(m_nTuningPtr<(m_CpTunning.m_nCoordL-1)) m_nTuningPtr++;

}

void CMainThread::CollectTrajectory()
{
	CCoord Cp;

	return;
//	theApp.m_Motors.GetHeadPos(&Cp);
	for(int nAxis = 0;nAxis<16;nAxis++) {
		Cp.fPos[nAxis] = (float)theApp.m_Axes[nAxis].nCurrentSpeed * theApp.m_Axes[nAxis].fStepSize;
//		Cp.fPos[nAxis] = (float)theApp.m_Axes[nAxis].nDAC;
	}
	m_CpTrajectory.ModifyCoord(m_CpTrajectory.m_nNextPtr++,Cp);
}

void CMainThread::MoveToHighGain()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CString Buff;
	int	nPos;

	switch(m_nSubThreadAction) {
	case 0:
		nPos = theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_PmacUser.m_nTunningAxis].nPhysicalAxis - 1 ].ActualToMachine(theApp.m_PmacUser.m_nTunningPos,theApp.m_PmacUser.m_nPosition[theApp.m_PmacUser.m_nTunningAxis-1]);
		theApp.m_PmacUser.JogTo(theApp.m_PmacUser.m_nTunningAxis, nPos);
		Sleep(500);
		m_nSubThreadAction=10;
	case 10:
		if(theApp.m_PmacUser.IsPhysicalAxisInPosition(theApp.m_PmacUser.m_nTunningAxis) == TRUE) {
//			theApp.m_PmacUser.RequestVariable("I", 100 * theApp.m_PmacUser.m_nTunningAxis  + 30, &Buff);
//			nGain = _ttoi(Buff);
//			theApp.m_PmacUser.SetVariable("I", 100 * theApp.m_PmacUser.m_nTunningAxis + 30, nGain * theApp.m_LastSettings.nInPosGainMultiplier);
//			Sleep(1000);
//			theApp.m_PmacUser.SetVariable("I", 100 * theApp.m_PmacUser.m_nTunningAxis + 30, nGain);
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			m_nSubThreadAction=0;
		}
		break;
	}

}


void CMainThread::TipsRemoveAir()
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		TipsRemoveAirCartesian();
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TWIN_TOWER_KINEMATIC:
		TipsRemoveAirRobot();
		break;
	case TRACKED_PROBE:
		break;
	}
}

void CMainThread::TipsRemoveAirCartesian()
{
	CTime CurrentTime = CTime::GetCurrentTime();
	static int osStartTime;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
		m_CpMoveTo.Side0.fYt=theApp.m_LastSettings.fRemoveAirAngle[0];
		m_CpMoveTo.Side1.fYt=theApp.m_LastSettings.fRemoveAirAngle[1];
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		m_nSubThreadAction=5;
		break;
	case 5:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction=10;
		};
		break;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			osStartTime=(int)CurrentTime.GetTime();
			m_nSubThreadAction=20;
		}
		break;
	case 20:
		if((CurrentTime.GetTime()-osStartTime) > theApp.m_LastSettings.nRemoveAirTime) {
			m_CpMoveTo.Side0.fYt=0.0;
			m_CpMoveTo.Side1.fYt=0.0;

			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);

			m_nSubThreadAction=30;
		}
		break;
	case 30:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction=999;
		};
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nSubThreadAction=0;
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;

	}
}


void CMainThread::RepairAxis()
{
	static	int nCurrentPos;
	static int nFollowingError;
	CString Buff;

	switch(m_nSubThreadAction) {
	case 0:
		nCurrentPos = theApp.m_Axes[m_nRepairAxis].nPos;
		nFollowingError = theApp.m_Axes[m_nRepairAxis].nFollowingError / 3072;

		Buff.Format(L"#%dk",theApp.m_Axes[m_nRepairAxis].nPhysicalAxis);
		theApp.m_PmacUser.SendStr(Buff);
		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			Buff.Format(L"j/");
			theApp.m_PmacUser.SendStr(Buff);
			m_nSubThreadAction = 20;
		}
		break;
	case 20:
		if(nFollowingError >= 0) {
			Buff.Format(L"j:%d",(int)(-1.0f / theApp.m_Axes[m_nRepairAxis].fStepSize));
		} else {
			Buff.Format(L"j:%d",(int)(1.0f / theApp.m_Axes[m_nRepairAxis].fStepSize));
		}
		theApp.m_PmacUser.SendStr(Buff);
		m_nSubThreadAction = 30;
		break;
	case 30:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			Buff.Format(L"j=%d",nCurrentPos - nFollowingError);
			theApp.m_PmacUser.SendStr(Buff);
			m_nSubThreadAction = 999;
		};
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nSubThreadAction = 0;
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}

}

/////////////////////////////////////////////////////
// m_CpMoveTo is a surface coordinate
/////////////////////////////////////////////////////
void CMainThread::MovetoStartWithoutDangerPlane()
{
	switch(theApp.m_Tank.nScannerDescription) {
	default: MovetoPosWithoutDangerPlanePreFieldBus();
		break;
	case SPX_ROBOT: 
	case TANK_5AXIS:
	case TANK_2AXIS:
		MovetoPosWithoutDangerPlaneFieldBus();
		break;
	case TRACKED_PROBE:
		break;
	}
}

void CMainThread::MovetoPosWithoutDangerPlaneFieldBus()
{
	CPolyCoord Line;

	switch(m_nDangerPlaneThreadAction) {
	case 0:
		Line.Add(m_CpMoveTo);
		theApp.m_FBCtrl.DoPolyLine(&Line,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);
		m_nDangerPlaneThreadAction += 10;
		break;
	case 10:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nDangerPlaneThreadAction = 999;
		}
		break;
	case 999:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {
			m_nDangerPlaneThreadAction = 0;
			if(m_nThreadAction == MOVETO_VIA_DANGER_PLANE) {
				m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
			} else {
				m_nSubThreadAction += 10;						//Add 10 because calling routine was a subthread anyway
			}
		}
		break;
	}

}

void CMainThread::MovetoPosWithoutDangerPlanePreFieldBus()
{
	static CCoord CpSurface,CpHead,Cp,CpDanger,CpDestination;

	int	nSide=0;
	int	nNotSide=1;


	switch(m_nDangerPlaneThreadAction) {
	case 0:

		CpDestination = m_CpMoveTo;
		theApp.m_Motors.SuspendMotors();
		theApp.m_WS.CalculateDangerZone();

		theApp.m_Motors.GetSurfacePos(&CpSurface);
		switch(theApp.m_PmacUser.m_nScanSide) {
		case 0:
			CpDestination.Side1 = CpSurface.Side1;
			break;
		case 1:
			CpDestination.Side0 = CpSurface.Side0;
			break;
		}

		theApp.m_Motors.GetHeadPos(&Cp);

		CpSurface = CpDestination;

		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);

		switch(theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide) {
		case 0:
			CpHead.Side[nNotSide] = Cp.Side[nNotSide];
			break;
		case 1:
			CpHead.Side[nSide] = Cp.Side[nSide];
			break;
		case 2:
			break;
		}
		m_CpMoveTo = CpHead;
		if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable!=TRUE) {
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		}else{
			theApp.m_Motors.EnableAxes(BOTH_MOVETO_WITH_R);
		}
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		m_nDangerPlaneThreadAction = 998;
		m_nScanLine = m_nScanStartLine;
		break;
	case 998:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nDangerPlaneThreadAction=999;
		}
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			if(m_nThreadAction == MOVETO_VIA_DANGER_PLANE) {
				m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
			} else {
				m_nSubThreadAction += 10;						//Add 10 because calling routine was a subthread anyway
			}
		}
		break;
	}

}

/////////////////////////////////////////////////////
// m_CpMoveTo is a surface coordinate
/////////////////////////////////////////////////////

void CMainThread::MovetoViaDangerPlane()
{
	switch(PROFILE->m_nComponentDescription) {
	default:
		if(theApp.m_nJobNumber==GKN){
			MovetoViaDangerPlaneGKN();
			return;
		}

		switch(theApp.m_Tank.nScannerDescription) {
		case DUAL_SIDED_7_AXES:
		case DUAL_SIDED_SQUIRTER:
		case SINGLE_BRIDGE_DUAL_SIDED:
		case DUAL_SIDED_SQUIRTER_2:
		case FB_11_AXIS:
		case FB_TWIN_TOWER:
			MovetoViaDangerPlaneDualSuirter();
			break;
		case SINGLE_BRIDGE_TT_ROLLERS:
		case SINGLE_BRIDGE_TANK_WITH_R:
		case SINGLE_BRIDGE_TANK:
		case DOUBLE_BRIDGE_TANK:
		case TANK_5AXIS:
		case TANK_6AXIS:
		case TANK_2AXIS:
			MovetoViaDangerPlaneBridgeTank();
			break;
		case SINGLE_ROBOT:
		case DUAL_ROBOT:
		case DUAL_ROBOT_9_PLUS_9:
		case SPX_ROBOT:
		case TWIN_TOWER_KINEMATIC:
			MovetoViaDangerPlaneKinematic();
			break;
		case TRACKED_PROBE:
			break;
		}
		break;

	case CLOSED_ROTARY:
	case OPEN_ROTARY:
	case CIRCULAR:

		switch(theApp.m_Tank.nScannerDescription) {
		default:	MovetoViaTurntableDangerPlane();
			break;
		case VERTICAL_BORE_SCANNER:
			MovetoStraightLineCartesian();
			break;
		case SINGLE_ROBOT:
		case DUAL_ROBOT:
		case DUAL_ROBOT_9_PLUS_9:
		case SPX_ROBOT:
		case TWIN_TOWER_KINEMATIC:
			MovetoViaDangerPlaneKinematic();
			break;
		case TRACKED_PROBE:
			break;
		}
		break;
	}
}

void CMainThread::MovetoViaDangerPlaneBridgeTank()
{
	switch(m_nDangerPlaneThreadAction) {
	case 0:
		m_CpDestination = m_CpMoveTo;
		theApp.m_Motors.SuspendMotors();
		PROFILE->CalculateDangerZone();

		theApp.m_Motors.GetSurfacePos(&m_CpSurface);

		if(m_CpDestination.Side0.fK >= 0.0f) {
			m_CpSurface.Side0.fZ = PROFILE->m_CpDanger[DANGER_MAXS].Side0.fZ; //PROFILE->m_CpDanger[DANGER_MINS].Side0.fZ;
		} else {
			m_CpSurface.Side0.fZ = PROFILE->m_CpDanger[DANGER_MINS].Side0.fZ; //PROFILE->m_CpDanger[DANGER_MAXS].Side0.fZ;
		}

		theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE, NULL, CURRENT_FL);
		if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable!=TRUE) {
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		}else{
			theApp.m_Motors.EnableAxes(BOTH_MOVETO_WITH_R);
		}
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		m_nDangerPlaneThreadAction=10;
		break;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetHeadPos(&m_CpHead);
			theApp.m_Kinematics.HeadFromSurface(&m_CpDestination,&m_CpMoveTo,PORTSIDE,TRUE,TRUE, NULL, CURRENT_FL);
			m_CpMoveTo.Side0.fZ = m_CpHead.Side0.fZ;
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nDangerPlaneThreadAction=20;
		}
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {

			theApp.m_Kinematics.HeadFromSurface(&m_CpDestination,&m_CpMoveTo,PORTSIDE,TRUE,TRUE, NULL, CURRENT_FL);

			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nDangerPlaneThreadAction = 999;
			m_nScanLine = m_nScanStartLine;
		}
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			if(m_nThreadAction == MOVETO_VIA_DANGER_PLANE) {
				m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
			} else {
				m_nSubThreadAction += 10;						//Add 10 because calling routine was a subthread anyway
			}
		}
		break;
	}
}

void CMainThread::MovetoViaDangerPlaneDualSuirter()
{
	static CCoord CpSurface,CpHead,Cp,CpDanger,CpDestination;
	CCoord *pCpMin = &PROFILE->m_CpDanger[DANGER_MINS];
	CCoord *pCpMax = &PROFILE->m_CpDanger[DANGER_MAXS];

	int	nSide=0;
	int	nNotSide=1;

	switch(m_nDangerPlaneThreadAction) {
	case 0:

		CpDestination = m_CpMoveTo;
		theApp.m_Motors.SuspendMotors();
		theApp.m_WS.CalculateDangerZone();

		theApp.m_Motors.GetSurfacePos(&CpSurface);
		switch(theApp.m_PmacUser.m_nScanSide) {
		case 0:
			CpDestination.Side1 = CpSurface.Side1;
			break;
		case 1:
			CpDestination.Side0 = CpSurface.Side0;
			break;
		}
/*
		if((pCpMin->Side[nSide].fI != -1.0f) || (pCpMin->Side[nNotSide].fI != -1.0f) || (theApp.m_PmacUser.m_nScanSide != 2)) {
			pCpMin->Side[nSide].fI = -1.0f;
			pCpMin->Side[nNotSide].fI = -1.0f;
			theApp.m_PmacUser.m_nScanSide = 2;
		}
*/
		theApp.m_Motors.GetHeadPos(&m_CpMoveTo);

		switch(theApp.m_PmacUser.m_nScanSide) {
		case 0:
			theApp.m_Kinematics.HeadFromSurface(pCpMin,&Cp,PORTSIDE,TRUE,TRUE);
			if(pCpMin->Side[nSide].fI) m_CpMoveTo.Side[nSide].fX = Cp.Side[nSide].fX;
			if(pCpMin->Side[nSide].fJ) m_CpMoveTo.Side[nSide].fY = Cp.Side[nSide].fY;
			if(pCpMin->Side[nSide].fK) m_CpMoveTo.Side[nSide].fZ = Cp.Side[nSide].fZ;
			break;
		case 1:
			theApp.m_Kinematics.HeadFromSurface(pCpMax,&Cp,STARBOARD,TRUE,TRUE);
			if(pCpMin->Side[nNotSide].fI) m_CpMoveTo.Side[nNotSide].fX = Cp.Side[nNotSide].fX;
			if(pCpMin->Side[nNotSide].fJ) m_CpMoveTo.Side[nNotSide].fY = Cp.Side[nNotSide].fY;
			if(pCpMin->Side[nNotSide].fK) m_CpMoveTo.Side[nNotSide].fZ = Cp.Side[nNotSide].fZ;
			break;
		case 2:
			theApp.m_Kinematics.HeadFromSurface(pCpMin,&Cp,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(pCpMax,&Cp,STARBOARD,TRUE,TRUE);
			if(pCpMin->Side[nSide].fI) m_CpMoveTo.Side[nSide].fX = Cp.Side[nSide].fX;
			if(pCpMin->Side[nSide].fJ) m_CpMoveTo.Side[nSide].fY = Cp.Side[nSide].fY;
			if(pCpMin->Side[nSide].fK) m_CpMoveTo.Side[nSide].fZ = Cp.Side[nSide].fZ;
			if(pCpMin->Side[nNotSide].fI) m_CpMoveTo.Side[nNotSide].fX = Cp.Side[nNotSide].fX;
			if(pCpMin->Side[nNotSide].fJ) m_CpMoveTo.Side[nNotSide].fY = Cp.Side[nNotSide].fY;
			if(pCpMin->Side[nNotSide].fK) m_CpMoveTo.Side[nNotSide].fZ = Cp.Side[nNotSide].fZ;
			break;
		}
		m_CpHead = m_CpMoveTo;
	case 2:
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpHead);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		Sleep(100);

		m_nDangerPlaneThreadAction=5;
		break;
	case 5:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nDangerPlaneThreadAction=10;
		}
		break;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			if(theApp.m_Motors.IsItAtDesirePos() != true) {
				m_nDangerPlaneThreadAction = 2;
				break;
			};

			theApp.m_Motors.GetHeadPos(&CpHead);

			CpSurface = CpDestination;

			switch(theApp.m_PmacUser.m_nScanSide) {
			case 0:
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
				m_CpMoveTo.Side1 = CpHead.Side1;
				if(pCpMin->Side[nSide].fI) m_CpMoveTo.Side[nSide].fX = CpHead.Side[nSide].fX;
				if(pCpMin->Side[nSide].fJ) m_CpMoveTo.Side[nSide].fY = CpHead.Side[nSide].fY;
				if(pCpMin->Side[nSide].fK) m_CpMoveTo.Side[nSide].fZ = CpHead.Side[nSide].fZ;
				break;
			case 1:
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);
				if(pCpMin->Side[nNotSide].fI) m_CpMoveTo.Side[nNotSide].fX = CpHead.Side[nNotSide].fX;
				if(pCpMin->Side[nNotSide].fJ) m_CpMoveTo.Side[nNotSide].fY = CpHead.Side[nNotSide].fY;
				if(pCpMin->Side[nNotSide].fK) m_CpMoveTo.Side[nNotSide].fZ = CpHead.Side[nNotSide].fZ;
				break;
			case 2:
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);
				if(pCpMin->Side[nSide].fI) m_CpMoveTo.Side[nSide].fX = CpHead.Side[nSide].fX;
				if(pCpMin->Side[nSide].fJ) m_CpMoveTo.Side[nSide].fY = CpHead.Side[nSide].fY;
				if(pCpMin->Side[nSide].fK) m_CpMoveTo.Side[nSide].fZ = CpHead.Side[nSide].fZ;
				if(pCpMin->Side[nNotSide].fI) m_CpMoveTo.Side[nNotSide].fX = CpHead.Side[nNotSide].fX;
				if(pCpMin->Side[nNotSide].fJ) m_CpMoveTo.Side[nNotSide].fY = CpHead.Side[nNotSide].fY;
				if(pCpMin->Side[nNotSide].fK) m_CpMoveTo.Side[nNotSide].fZ = CpHead.Side[nNotSide].fZ;
				break;
			}
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(300);

			m_nDangerPlaneThreadAction=15;
		}
		break;
	case 15:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nDangerPlaneThreadAction=20;
		}
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetHeadPos(&Cp);

			CpSurface = CpDestination;

			theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);

			switch(theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide) {
			case 0:
				CpHead.Side[nNotSide] = Cp.Side[nNotSide];
				break;
			case 1:
				CpHead.Side[nSide] = Cp.Side[nSide];
				break;
			case 2:
				break;
			}
			theApp.m_Thread.m_CpMoveTo = CpHead;
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nDangerPlaneThreadAction = 998;
			m_nScanLine = m_nScanStartLine;
			Sleep(300);
		}
		break;
	case 998:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nDangerPlaneThreadAction=999;
		}
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			if(m_nThreadAction == MOVETO_VIA_DANGER_PLANE) {
				m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
			} else {
				m_nSubThreadAction += 10;						//Add 10 because calling routine was a subthread anyway
			}
		}
		break;
	}
}

void CMainThread::MovetoViaDangerPlaneGKN()
{
	static CCoord CpSurface,CpHead,Cp,CpDanger,CpDestination;

	switch(m_nDangerPlaneThreadAction) {
	case 0: //Moves tips and y to 0
		CpDestination = m_CpMoveTo;
		theApp.m_Motors.SuspendMotors();
		PROFILE->CalculateDangerZone();

		theApp.m_Motors.GetHeadPos(&m_CpMoveTo);

		CpDanger = PROFILE->m_CpDanger[DANGER_MINS];
		switch(PROFILE->m_nSlowAxis) {
		case 2:
			theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE); //Need to move Y's to Zero and left side Ztip to -90 to keep clear of rotating blade
			m_CpMoveTo.Side0.fY = (float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
			m_CpMoveTo.Side0.fYt = -90.0f;
			m_CpMoveTo.Side1.fY = (float)theApp.m_Axes[theApp.m_Tank.nYRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize * -1.0f;
			m_CpMoveTo.Side1.fXt = 0.0f;
			m_CpMoveTo.Side1.fYt = 0.0f;
			break;
		case 1:
			//We should not be here as GKN works in XZ
			switch(theApp.m_PmacUser.m_nScanSide) {
			case 0:
				theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
				m_CpMoveTo.Side0.fZ = Cp.Side0.fZ;
				m_CpMoveTo.Side1.fXt=0.0f;
				m_CpMoveTo.Side1.fYt=0.0f;
				break;
			case 1:
				theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
				m_CpMoveTo.Side1.fZ = Cp.Side1.fZ;
				m_CpMoveTo.Side0.fXt=0.0f;
				m_CpMoveTo.Side0.fYt=0.0f;
				break;
			case 2:
				theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
				theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
				m_CpMoveTo.Side0.fZ = Cp.Side0.fZ;
				m_CpMoveTo.Side1.fZ = Cp.Side1.fZ;
				break;
			}
			break;
		}
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		Sleep(300);

		m_nDangerPlaneThreadAction=5;
		break;
	case 5: //Move Left Z up
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {

			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);

			CpSurface = CpDestination;
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);

			m_CpMoveTo.Side0.fZ = (float)theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize * -1.0f;;

			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(800);

			m_nDangerPlaneThreadAction=8;

		}
		break;
	case 8: //Move rotate
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {

			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);

			CpSurface = CpDestination;
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);

			m_CpMoveTo.Side0.fR = CpHead.Side0.fR;

			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(300);

			m_nDangerPlaneThreadAction=10;
		}
		break;
	case 10: //move all axes apart from y to destination
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {

			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);

			CpSurface = CpDestination;
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);

			CpDanger = PROFILE->m_CpDanger[DANGER_MINS];
			switch(PROFILE->m_nSlowAxis) {
			case 2: //Need to move opposite side to avoid Xright hitting stop
				switch(theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide) {
				case 0:
					m_CpMoveTo.Side0 = CpHead.Side0;
					m_CpMoveTo.Side1.fX = CpHead.Side0.fX;
					theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
					m_CpMoveTo.Side0.fY = (float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
					break;
				case 1:
					m_CpMoveTo.Side1 = CpHead.Side1;
					m_CpMoveTo.Side0.fX = CpHead.Side1.fX;
					theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
					m_CpMoveTo.Side1.fY = (float)theApp.m_Axes[theApp.m_Tank.nYRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize * -1.0f;
					break;
				case 2:
					m_CpMoveTo = CpHead;
					theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
					theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
					m_CpMoveTo.Side0.fY = (float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
					m_CpMoveTo.Side1.fY = (float)theApp.m_Axes[theApp.m_Tank.nYRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize * -1.0f;
					break;
				}
				break;
			case 1:
				//We should not be here as GKN works in XZ
				switch(theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide) {
				case 0:
					m_CpMoveTo.Side0 = CpHead.Side0;
					theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
					m_CpMoveTo.Side0.fZ = Cp.Side0.fZ;
					break;
				case 1:
					m_CpMoveTo.Side1 = CpHead.Side1;
					theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
					m_CpMoveTo.Side1.fZ = Cp.Side1.fZ;
					break;
				case 2:
					m_CpMoveTo = CpHead;
					theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
					theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
					m_CpMoveTo.Side0.fZ = Cp.Side0.fZ;
					m_CpMoveTo.Side1.fZ = Cp.Side1.fZ;
					break;
				}
				break;
			}
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(300);

			m_nDangerPlaneThreadAction=20;
		}
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) { //Moves all axes to destination, keeps other side at current position
			theApp.m_Motors.GetHeadPos(&Cp);

			CpSurface = CpDestination;

			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);

			switch(theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide) {
			case 0:
				CpHead.Side1 = Cp.Side1;
				break;
			case 1:
				CpHead.Side0 = Cp.Side0;
				break;
			case 2:
				break;
			}
			theApp.m_Thread.m_CpMoveTo = CpHead;
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nDangerPlaneThreadAction = 999;
			m_nScanLine = m_nScanStartLine;
			Sleep(300);
		}
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}
}


void CMainThread::MovetoSurface()
{
	static CCoord CpSurface,CpHead,Cp,CpDanger,CpDestination;

	switch(m_nDangerPlaneThreadAction) {
	case 0:
		CpDestination = m_CpMoveTo;
		theApp.m_Motors.SuspendMotors();

		theApp.m_Motors.GetHeadPos(&Cp);

		CpSurface = CpDestination;

		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);

		switch(theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide) {
		case 0:
			CpHead.Side1 = Cp.Side1;
			break;
		case 1:
			CpHead.Side0 = Cp.Side0;
			break;
		case 2:
			break;
		}
		theApp.m_Thread.m_CpMoveTo = CpHead;
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		m_nDangerPlaneThreadAction = 999;
		Sleep(300);
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}
}


//return 0 if safe to continue
//return 1 if stop immediately

int CMainThread::IsSafetyCompromised(int nAction /*Thread or Timer Action*/)
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	int nMask = 1;
	int nFlag = 0;
	int nCheckState;
	int ii;
	static int nOldCheckState = -1;
	static int nOldInPort = -1;
	AndonEventType AndonFlag = MachineOkay;
	CString AndonMessage;
	AndonMessage.Empty();

	switch(nAction) {
	default:							nCheckState = SAFETY_STOP;
		break;
	case WAIT_FOR_MOVE_FINISHED:
	case JOYSTICK_START:
										nCheckState = SAFETY_JOYSTICK;
		break;
	case MOVETO_STRAITE_LINE:
										nCheckState = SAFETY_MOVETO_STRAITE_LINE;
		break;
	case POLY_MOVE:
	case REPETETIVE_POLY_MOVE:
	case TEST_ALL_LINES:
	case DUAL_POLY_MOVE:
	case MOVETO_SAFE:
	case SINGLE_AUTO_ALIGN:
	case MULTIPLE_AUTO_ALIGN:
	case MEASURE_WATER_VELOCITY:
	case SINGLE_TIP_AUTO_ALIGN:
	case MULTIPLE_TIP_AUTO_ALIGN:
	case NORMALIZE:
	case CONTOUR_SCAN:
	case MULTI_PROBE_SCAN:
	case TURNTABLE_SCAN:
	case CONTINUOUS_TURNTABLE_SCAN:
	case SEGMENT_TURNTABLE_SCAN:
	case MOVETO_HIGH_GAIN:
	case MOVETO_HOLDING_FIXTURE:
	case FLAT_SCAN:
	case MOVETO_SAFELY:
	case FIND_ZERO:
	case PEAK_OPPOSITE:
	case WATERPATH_ADJUST:
	case OPTIMIZE_BRAIN_COORDINATE:
	case OPTIMIZE_BRAIN_LINE:
	case OPTIMIZE_BRAIN_ALL_LINES:
	case MOVETO_ORIGIN:
	case MOVE_ROUND_BALL:
	case TUNING_MOVE:
	case TIPS_REMOVE_AIR:
	case REPAIR_AXIS:
	case MOVETO_CONTOUR_START:
	case MOVETO_VIA_DANGER_PLANE:
	case MOVETO_VIEW_VIA_DANGER_PLANE:
	case MOVETO_SURFACE:
	case RELEARN_LINE:
	case RELEARN_BRAIN:
	case OPTOMISE_COORDINATE:
	case MOVETO_PARK:
	case LASER_TAUGHT_LINE_SCAN:
	case EXECUTE_LADDER:
	case BLADE_TO_DEGREES:
	case BLADE_GO_HOME:
	case BLADE_SCAN:
	case WORKSHEET_SCAN:
	case ROBOT_CONTOUR_SCAN:
	case ORIGIN_ALL_ROBO_9_PLUS_9:
	case JOG_TO_POSITION:
	case DEPOSIT_TOOL_0:
	case DEPOSIT_TOOL_1:
	case RETRIEVE_TOOL_0:
	case RETRIEVE_TOOL_1:
	case LEADSCREW_COMPENSATION:

										nCheckState = SAFETY_POLY_MOVE;
		break;
	case ORIGIN_X_LEFT:
	case ORIGIN_Y_LEFT:
	case ORIGIN_Z_LEFT:
	case ORIGIN_XT_LEFT:
	case ORIGIN_YT_LEFT:
	case ORIGIN_X_RIGHT:
	case ORIGIN_Y_RIGHT:
	case ORIGIN_Z_RIGHT:
	case ORIGIN_XT_RIGHT:
	case ORIGIN_YT_RIGHT:
	case ORIGIN_ALL:
	case ORIGIN_ALL_LEFT:
	case ORIGIN_ALL_RIGHT:
	case ORIGIN_XYZ_LEFT:
	case ORIGIN_XYZ_RIGHT:
	case ORIGIN_XTYT_RIGHT:
	case ORIGIN_RIGHT_ALL:
	case ORIGIN_SEQUENTIAL:
	case ORIGIN_X_BRIDGE:
	case ORIGIN_R_LEFT:
	case ORIGIN_X_MOVING:
	case ORIGIN_Y_MOVING:
	case ORIGIN_Z_MOVING:
	case ORIGIN_X_FIXED:
	case ORIGIN_Y_FIXED:
	case ORIGIN_Z_FIXED:
	case ORIGIN_HOLDING_FIXTURE:
	case ORIGIN_HOLDING_FIXTURE_FIXED:
	case ORIGIN_HOLDING_FIXTURE_MOVEING:
	case ORIGIN_ALL_FOREPLANE:
	case ORIGIN_X_Y_Z_R_THETA_LEFT:
											nCheckState = SAFETY_ORIGIN;
		break;
	}


	/* Check the bit table with I/O readings and see if safe*/
	m_nInPort = theApp.m_Motors.GetInputPort();

	if(m_nInPort & theApp.m_Tank.nEmergencyBit) {
		Abort();
		nFlag = 1;
		AndonFlag = MachineFault;
		AndonMessage = L"Emergency switch pressed in Safety compromised";
		goto function_finish;
	}

	if(((m_nInPort - nOldInPort) & theApp.m_Tank.nAutomaticBit) && (nCheckState == SAFETY_ORIGIN || nCheckState == SAFETY_POLY_MOVE || nCheckState == SAFETY_MOVETO_STRAITE_LINE) ) {

		theApp.m_Motors.SuspendMotors();
		Abort();
		MessageBox(NULL, L"You interferred with an auto move so the system has stopped", L"Error",MB_ICONERROR);

		nFlag = 1;
		AndonFlag = MachineFault;
		AndonMessage = L"You interferred with an auto move so the system has stopped";
		goto function_finish;
	};


	switch(nCheckState) {
	default: nFlag = 1;
		m_nThreadAction = 0;
		m_nSubThreadAction = 0;
		UnsafePrompt();
		AndonFlag = MachineWarning;
		AndonMessage = L"Unsafe move in default";
		goto function_finish;
		break;
	case SAFETY_JOYSTICK:
		break;
	case SAFETY_MOVETO_STRAITE_LINE:
	case SAFETY_POLY_MOVE:
	case SAFETY_ORIGIN:
		AndonFlag = CycleRunning;
		AndonMessage = L"Cycle running";
		nMask = 1;
		nFlag = 0;
		for(ii=0;ii<16;ii++,nMask<<=1) {
			switch(theApp.m_nSafetyLogic[nCheckState][ii]) {
			case 0: //Dont care
				break;
			case 1: // False;
				if(m_nInPort & nMask) nFlag = 1;
				break;
			case 2://True
				if(!(m_nInPort & nMask)) nFlag = 1;
				break;
			}
			if(nFlag) {
				if(m_nSubThreadAction>0) {
					m_bAbortFlag = TRUE;
					theApp.m_bCollectBladeData=FALSE;
					theApp.m_bCollectComplexData=FALSE;
					theApp.m_bCollectNormalizeData=FALSE;
					theApp.m_bCollectTunning=FALSE;
					theApp.m_bLaserCollectTaughtLines=FALSE;
					theApp.m_Motors.SuspendMotors();
					theApp.m_PmacUser.PumpStop(theApp.m_Tank.nLeftPump);
					theApp.m_PmacUser.PumpStop(theApp.m_Tank.nRightPump);
				}

				pFrame->m_nTimerAction=0;
				pFrame->m_nSubTimerAction=0;
				m_nThreadAction=0;
				m_nSubThreadAction = 0;
				UnsafePrompt();
				AndonFlag = MachineWarning;
				AndonMessage = L"Unsafe move in Auto mode";
				goto function_finish;
			}
		}

		break;
	}

	nOldCheckState = nCheckState;

function_finish:
	nOldInPort = m_nInPort;
	theApp.AndonEvent(AndonFlag, AndonMessage);

	return nFlag;
}

void CMainThread::Abort()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	m_bAbortFlag = TRUE;
	theApp.m_bCollectBladeData=FALSE;
	theApp.m_bCollectComplexData=FALSE;
	theApp.m_bCollectNormalizeData=FALSE;
	theApp.m_bCollectTunning=FALSE;
	theApp.m_bLaserCollectTaughtLines=FALSE;
	m_nThreadAction=0;
	m_nSubThreadAction = 0;
	pFrame->m_nTimerAction=0;
	pFrame->m_nSubTimerAction=0;
}

void CMainThread::UnsafePrompt()
{
	CString Buff;

	Buff += " This is unsafe. The instruction has been cancelled";
	CPromptDlg* pDlg = new CPromptDlg(NULL,Buff);
	pDlg->Create(CPromptDlg::IDD, NULL);
	Sleep(2000);
	pDlg->SendMessage(WM_CLOSE);
	delete pDlg;

}



void CMainThread::RectilinearScan()
{
	CCoord	Cp,CpSurface,CpHead,CpTemp,Cp0,Cp1;
	int nNotSide,nSide;
	int nLine,nColoumn,nn,nAxis,nSample;
	float fMaxDeviation,fTemp;
	int nPrimaryAxis,nSamplesLine,nNumberLines;
	CString Buff;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	float fWidth = -1.0f;
	float fHeight = -1.0f;
	if(m_pNormalizePage) {
		fWidth = theApp.m_LastSettings.fPeakDistance;
		fHeight = theApp.m_LastSettings.fPeakDistance;
		nSamplesLine = 100;
		nNumberLines = 30;
	}
	if(m_pProbeBeamSectionsPage) {
		fWidth = theApp.m_LastSettings.fProbeFastScanSize;
		fHeight = theApp.m_LastSettings.fProbeSlowScanSize;
		nSamplesLine = (int)(theApp.m_LastSettings.fProbeFastScanSize / theApp.m_LastSettings.fProbeFastIncrement);
		nNumberLines = (int)(theApp.m_LastSettings.fProbeSlowScanSize / theApp.m_LastSettings.fProbeSlowIncrement);
	}
	if((fWidth <= 0.0f) || (fHeight <= 0.0f))
		return;

	D3DXMATRIXA16 matWorld,matNormal,matTranslation;
	D3DXVECTOR3 vecP;

	nSide=theApp.m_PmacUser.m_nScanSide=m_nNormSide;
	nSide==PORTSIDE ? nNotSide = STARBOARD : nNotSide = PORTSIDE;

	switch(m_nSubThreadAction) {
	case 0:
		m_bAbortFlag = FALSE;
		m_bPause = FALSE;
		theApp.m_Motors.GetSurfacePos(&CpSurface);


		m_ToolPath.Zero();
		m_ToolPath.Add(CpHead);
		m_ToolPath.Add(CpHead);

		theApp.m_Kinematics.D3DXRotateMatrixFromKNormal(matNormal, CpSurface.Side[nSide].norm);
		D3DXMatrixTranslation(&matTranslation,CpSurface.Side[nSide].fX,CpSurface.Side[nSide].fY,CpSurface.Side[nSide].fZ);
		D3DXMatrixMultiply(&matWorld,&matNormal,&matTranslation);

		for(nn=3,nLine=0;nLine<2;nLine++) {
			for(nColoumn=0;nColoumn<2;nColoumn++,nn--) {
				m_CpRect[nn] = CpSurface;
				vecP.x = (fWidth / -2) + ((float)nColoumn * fWidth);
				vecP.y = (fHeight / -2) + ((float)nLine * fHeight);
				vecP.z = 0.0f;
				D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
				m_CpRect[nn].Side[nSide].fX = vecP.x;
				m_CpRect[nn].Side[nSide].fY = vecP.y;
				m_CpRect[nn].Side[nSide].fZ = vecP.z;
			}
		}

		m_bPause = FALSE;
		theApp.m_bScanning=TRUE;
		theApp.m_Motors.SuspendMotors();
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE, NULL, 0);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE, NULL, 0);
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		Sleep(300);
		m_nSubThreadAction=10;

		break;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nScanLine = 0;
			m_nNumberScanLines=nNumberLines;
			m_nSubThreadAction=20;
		}
		break;
	case 20:
		if((m_nScanLine < m_nNumberScanLines) && (m_bAbortFlag == FALSE)) {
			fMaxDeviation = 0.0f;
			nPrimaryAxis = 0;
			for(nAxis=0;nAxis<20;nAxis++) {
				Cp0.fPos[nAxis] = ((m_CpRect[2].fPos[nAxis] - m_CpRect[0].fPos[nAxis]) * (float)m_nScanLine / (float)(m_nNumberScanLines - 1)) + m_CpRect[0].fPos[nAxis];
				Cp1.fPos[nAxis] = ((m_CpRect[3].fPos[nAxis] - m_CpRect[1].fPos[nAxis]) * (float)m_nScanLine / (float)(m_nNumberScanLines - 1)) + m_CpRect[1].fPos[nAxis];
				fTemp = (float)fabs(Cp1.fPos[nAxis] - Cp0.fPos[nAxis]);
				if(fMaxDeviation < fTemp) {
					fMaxDeviation = fTemp;
					nPrimaryAxis = nAxis;
				}
			}
			theApp.m_Kinematics.HeadFromSurface(&Cp0,&CpHead,PORTSIDE,TRUE,TRUE, NULL, 0);
			theApp.m_Kinematics.HeadFromSurface(&Cp0,&CpHead,STARBOARD,TRUE,TRUE, NULL, 0);
			m_ToolPath.ModifyCoord(0,CpHead);
			theApp.m_Kinematics.HeadFromSurface(&Cp1,&CpHead,PORTSIDE,TRUE,TRUE, NULL, 0);
			theApp.m_Kinematics.HeadFromSurface(&Cp1,&CpHead,STARBOARD,TRUE,TRUE, NULL, 0);
			m_ToolPath.ModifyCoord(1,CpHead);
			m_CpNormal.AllocateCoords(nSamplesLine);
			m_CpNormal.ClearMemory();
			m_ToolPath.SetPrimaryAxis(nPrimaryAxis);
			m_CpNormal.SetPrimaryAxis(nPrimaryAxis);
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[nSamplesLine-1] = m_ToolPath.m_pCp[1];
			m_CpNormal.m_pCp[0].nAmp=m_CpNormal.m_pCp[nSamplesLine-1].nAmp=0;

			theApp.m_bCollectNormalizeData=TRUE;

			m_CpMoveTo = m_ToolPath.m_pCp[(m_nScanLine+1) & 1];
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(300);
			m_nSubThreadAction = 30;
		} else {
			if(m_pNormalizePage) {
				m_pNormalizePage->FindRectilinearCentre(&nSample,&nLine);
			}
			if(m_pProbeBeamSectionsPage) {
				m_pProbeBeamSectionsPage->FindRectilinearCentre(&nSample,&nLine);
			}
			for(nAxis=0;nAxis<20;nAxis++) {
				Cp0.fPos[nAxis] = ((m_CpRect[2].fPos[nAxis] - m_CpRect[0].fPos[nAxis]) * (float)nLine / (float)(m_nNumberScanLines - 1)) + m_CpRect[0].fPos[nAxis];
				Cp1.fPos[nAxis] = ((m_CpRect[3].fPos[nAxis] - m_CpRect[1].fPos[nAxis]) * (float)nLine / (float)(m_nNumberScanLines - 1)) + m_CpRect[1].fPos[nAxis];
			}
			for(nAxis=0;nAxis<20;nAxis++) {
				CpSurface.fPos[nAxis] = ((Cp1.fPos[nAxis] - Cp0.fPos[nAxis]) * (float)nSample / (float)(m_CpNormal.m_nCoordL - 1)) + Cp0.fPos[nAxis];
			}
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE, NULL, 0);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE, NULL, 0);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(300);
			m_nSubThreadAction = 40;
		}
		break;
	case 30:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_bCollectNormalizeData=FALSE;
			if(m_pNormalizePage) {
				m_pNormalizePage->TransferRectilinearData(&m_CpNormal,m_nNumberScanLines,m_nScanLine);
				m_pNormalizePage->Invalidate(FALSE);
			}
			if(m_pProbeBeamSectionsPage) {
				m_pProbeBeamSectionsPage->TransferRectilinearData(&m_CpNormal,m_nNumberScanLines,m_nScanLine);
				m_pProbeBeamSectionsPage->Invalidate(FALSE);
			}
			m_nScanLine++;
			m_nSubThreadAction = 20;
		}
		break;
	case 40:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nSubThreadAction = 0;
			m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
			if(m_pProbeBeamSectionsPage) {
				m_pProbeBeamSectionsPage->TransferScanToBeamProfile(); // done hear to ensure the probe is in the centre
			}
		}
		break;
	}

}




void CMainThread::CollectEccentricData()
{
	CCoord Cp;

	float fPath=theApp.m_UtUser.GetGateWaterPath(theApp.m_LastSettings.nEccentricTimeSlot);
	theApp.m_Motors.GetSurfacePos(&Cp);

	int nPtr = (int)((Cp.Side0.fR * (float)m_CpEccentric.m_nCoordL) / 360.0f);
	m_CpEccentric.m_pCp[nPtr] = Cp;
	m_CpEccentric.m_pCp[nPtr].fWp[0] = fPath;
}



void CMainThread::ProbeAxialPlot()
{
	CCoord	Cp,CpSurface,CpHead,CpTemp,Cp0,Cp1;
	int nNotSide,nSide;
	int nAxis,nDelay;
	float fMaxDeviation,fTemp;
	int nPrimaryAxis,nSamplesLine,nNumberLines;
	CString Buff;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	float fWidth = -1.0f;
	float fHeight = -1.0f;

	nSide=theApp.m_PmacUser.m_nScanSide=m_nNormSide;
	nSide==PORTSIDE ? nNotSide = STARBOARD : nNotSide = PORTSIDE;

	fWidth = theApp.m_LastSettings.fProbeFastScanSize;
	fHeight = (float)fabs(m_pProbe->m_Cp[1].Side[nSide].fY - m_pProbe->m_Cp[0].Side[nSide].fY);

	nSamplesLine = (int)(theApp.m_LastSettings.fProbeFastScanSize / theApp.m_LastSettings.fProbeFastIncrement);
	nNumberLines = (int)(fHeight / theApp.m_LastSettings.fProbeSlowIncrement);


	switch(m_nSubThreadAction) {
	case 0:
		m_bAbortFlag = FALSE;
		m_bPause = FALSE;
		theApp.m_Motors.GetHeadPos(&CpHead);
		m_ToolPath.Zero();
		m_ToolPath.Add(CpHead);
		m_ToolPath.Add(CpHead);


		//bottom two coordinates
		m_CpRect[0] = m_pProbe->m_Cp[0];
		m_CpRect[1] = m_pProbe->m_Cp[0];
		m_CpRect[0].Side[nSide].fX -= (fWidth / 2.0f);
		m_CpRect[1].Side[nSide].fX += (fWidth / 2.0f);
		//top two coordinates
		m_CpRect[2] = m_pProbe->m_Cp[1];
		m_CpRect[3] = m_pProbe->m_Cp[1];
		m_CpRect[2].Side[nSide].fX -= (fWidth / 2.0f);
		m_CpRect[3].Side[nSide].fX += (fWidth / 2.0f);


		m_bPause = FALSE;
		theApp.m_bScanning=TRUE;
		theApp.m_Motors.SuspendMotors();
		m_CpMoveTo = m_CpRect[0];
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		Sleep(300);
		m_nSubThreadAction=10;

		break;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nScanLine = 0;
			m_nNumberScanLines=nNumberLines;
			m_nSubThreadAction=20;
		}
		break;
	case 20:
		if((m_nScanLine < m_nNumberScanLines) && (m_bAbortFlag == FALSE)) {
			fMaxDeviation = 0.0f;
			nPrimaryAxis = 0;
			for(nAxis=0;nAxis<20;nAxis++) {
				Cp0.fPos[nAxis] = ((m_CpRect[2].fPos[nAxis] - m_CpRect[0].fPos[nAxis]) * (float)m_nScanLine / (float)(m_nNumberScanLines - 1)) + m_CpRect[0].fPos[nAxis];
				Cp1.fPos[nAxis] = ((m_CpRect[3].fPos[nAxis] - m_CpRect[1].fPos[nAxis]) * (float)m_nScanLine / (float)(m_nNumberScanLines - 1)) + m_CpRect[1].fPos[nAxis];
				fTemp = (float)fabs(Cp1.fPos[nAxis] - Cp0.fPos[nAxis]);
				if(fMaxDeviation < fTemp) {
					fMaxDeviation = fTemp;
					nPrimaryAxis = nAxis;
				}
			}

			if(theApp.m_LastSettings.bProbeTrackgate == TRUE) {
				nDelay = (int)((float)(m_pProbe->m_Cp[1].nDelay[0] - m_pProbe->m_Cp[0].nDelay[0]) * (float)m_nScanLine / (float)(m_nNumberScanLines - 1));
				nDelay += m_pProbe->m_Cp[0].nDelay[0];
				theApp.m_UtUser.m_TS[theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide]].Gate.nNsDelay[0] = nDelay;
				theApp.m_UtUser.Dsp200CalculateHardware(theApp.m_LastSettings.nPeakTimeSlot[theApp.m_LastSettings.nPeakSide],0);
			}


			m_ToolPath.ModifyCoord(0,Cp0);
			m_ToolPath.ModifyCoord(1,Cp1);
			m_CpNormal.AllocateCoords(nSamplesLine);
			m_CpNormal.ClearMemory();
			m_ToolPath.SetPrimaryAxis(nPrimaryAxis);
			m_CpNormal.SetPrimaryAxis(nPrimaryAxis);
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[nSamplesLine-1] = m_ToolPath.m_pCp[1];
			m_CpNormal.m_pCp[0].nAmp=m_CpNormal.m_pCp[nSamplesLine-1].nAmp=0;

			theApp.m_bCollectNormalizeData=TRUE;
			theApp.m_Si10User.m_Si10.bCollectData = TRUE;

			m_CpMoveTo = m_ToolPath.m_pCp[(m_nScanLine+1) & 1];
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(300);
			m_nSubThreadAction = 30;
		} else {
			Sleep(300);
			m_nSubThreadAction = 40;
		}
		break;
	case 30:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_bCollectNormalizeData=FALSE;
			theApp.m_Si10User.m_Si10.bCollectData = FALSE;
			if(m_pProbeAxialPlotPage) {
				m_pProbeAxialPlotPage->TransferRectilinearData(&m_CpNormal,m_nNumberScanLines,m_nScanLine);
				m_pProbeAxialPlotPage->Invalidate(FALSE);
			}
			m_nScanLine++;
			m_nSubThreadAction = 20;
		}
		break;
	case 40:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nSubThreadAction = 0;
			m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
			if(m_pProbeAxialPlotPage) {
				m_pProbeAxialPlotPage->TransferScanToBeamProfile();
			}
		}
		break;
	}

}





void CMainThread::MoveToPark()
{
	static CCoord CpSurface,CpHead,Cp,CpDanger,CpDestination;
	CString Buff;

	switch(m_nSubThreadAction) {
	case 0: //Moves tips and y to 0
		theApp.m_Motors.GetHeadPos(&m_CpMoveTo);

		m_CpMoveTo.Side0.fY = (float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
	//	m_CpMoveTo.Side0.fXt = 0.0f;
	//	m_CpMoveTo.Side0.fYt = 0.0f;
		m_CpMoveTo.Side1.fY = (float)theApp.m_Axes[theApp.m_Tank.nYRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize * -1.0f;
	//	m_CpMoveTo.Side1.fXt = 0.0f;
	//	m_CpMoveTo.Side1.fYt = 0.0f;

		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		Sleep(800);

		m_nSubThreadAction=10;
		break;
	case 10: //Move Left Z up
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {

			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);

			m_CpMoveTo.Side0.fZ = (float)theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize * -1.0f;;
			m_CpMoveTo.Side1.fZ = (float)theApp.m_Axes[theApp.m_Tank.nZRight].nOffset * theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize * -1.0f;;

			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(800);

			m_nSubThreadAction=20;

		}
		break;
	case 20: //Move rotate
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {

			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);

			m_CpMoveTo.Side0.fXt = 0.0f;
			m_CpMoveTo.Side0.fYt = 0.0f;
			m_CpMoveTo.Side1.fXt = 0.0f;
			m_CpMoveTo.Side1.fYt = 0.0f;

			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(800);

			m_nSubThreadAction=30;
		}
		break;
	case 30: //move all axes apart from y to destination
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {

			Buff.Format(_T("#%dj+"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis);
			theApp.m_PmacUser.SendStr(Buff);
			Buff.Format(_T("#%dj+"),theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis);
			theApp.m_PmacUser.SendStr(Buff);
			Buff.Format(_T("#%dj-"),theApp.m_Axes[theApp.m_Tank.nXBridge].nPhysicalAxis);
			theApp.m_PmacUser.SendStr(Buff);

			m_nSubThreadAction=999;
		}
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}

}

void CMainThread::MoveToParkForeplane()
{
	static CCoord CpSurface,CpHead,Cp,CpDanger,CpDestination;
	CString Buff;

	switch(m_nSubThreadAction) {
	case 0: //move Z to 0
		theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
		m_CpMoveTo.Side0.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize * -1.0f;
	
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		Sleep(500);

		m_nSubThreadAction=10;
		break;
	case 10: //tips to surface 0
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {

			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
			m_CpMoveTo.Side0.fXt=0.0f;
			m_CpMoveTo.Side0.fYt=0.0f;
		
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(500);
	
			m_nSubThreadAction=20;
		}
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
		
			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
			m_CpMoveTo.Side0.fX=(float)theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize * -1.0f;
			m_CpMoveTo.Side0.fY=(float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
		
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(500);
	
			m_nSubThreadAction=30;
		}
		break;
		case 30: //move all axes apart from y to destination
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nSubThreadAction=999;
		}
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}//switch


}


void CMainThread::EccentricTest()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	int	nn;
	float fWp;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_Motors.CollectEccentricData(false);;	//this is here in case the previous one stalled out and nothing was reset;
		theApp.m_Motors.GetSurfacePos(&m_CpSurface);
		m_CpEccentric.Zero();
		m_CpEccentric.AllocateCoords(360);
		m_CpEccentricCentre.Zero();
		for(nn=0;nn<360;nn++) {
			m_CpEccentric.m_pCp[nn].fWp[0] = theApp.m_Kinematics.m_fDesiredWaterPath[0];
		}
		theApp.m_Motors.CollectEccentricData(true);

		theApp.m_Motors.StartXNumberOfRevs(2.0f,PROFILE->m_fScanSpeed);
		m_nSubThreadAction=999;
		break;
	case 999:
		if(pFrame->m_pProfileSheet) {
			pFrame->m_pProfileSheet->m_p3DEditTurnTablePage->Invalidate(FALSE);
		}
		if(theApp.m_Motors.IsAxisInPosition(theApp.m_Tank.nRLeft) == TRUE) {
			theApp.m_Motors.CollectEccentricData(false);
			m_CpEccentricCentre.Zero();
			m_CpEccentricCentre.fWp[0] = 10000.0f;
			fWp=0.0f;
			for(nn=0;nn<360;nn++) {
				fWp+=m_CpEccentric.m_pCp[nn].fWp[0];
				if(m_CpEccentric.m_pCp[nn].fWp[0] < m_CpEccentricCentre.fWp[0]) {
					m_CpEccentricCentre = m_CpEccentric.m_pCp[nn];
				}
			}
			fWp /= (float)360.0f;
			m_CpEccentricCentre.fWp[0] = fWp - m_CpEccentricCentre.fWp[0];	//This is the closes point

			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}

}



void CMainThread::MovetoTurntableSafe()
{
	switch(m_nSafeThreadAction) {
	case 0:
		theApp.m_Motors.GetSurfacePos(&m_CpSurface);
		m_CpMoveTo = m_CpSurface;
		PROFILE->CalculateTurntableDangerZone();

		if(m_CpSurface.Side0.fZ < theApp.m_Tank.CpSafe.Side0.fZ) {	//Inside component. Need to move up
			m_CpMoveTo.Side0.fY = 0.0f;
			m_CpMoveTo.Side1.fY = 0.0f;
			m_CpMoveTo.Side0.fX = PROFILE->m_CpDanger[0].Side0.fX;
			m_CpMoveTo.Side1.fX = PROFILE->m_CpDanger[0].Side1.fX;
			m_nSafeThreadAction = 100;
		} else {
			m_CpMoveTo = theApp.m_Tank.CpSafe;
			m_nSafeThreadAction = 999;
		}
		theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

		m_CpMoveTo.Side0.fR = m_CpSurface.Side0.fR;
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		break;
	case 100:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetSurfacePos(&m_CpSurface);
			m_CpMoveTo = m_CpSurface;
			m_CpMoveTo.Side0.fZ = PROFILE->m_CpDanger[0].Side0.fZ;
			m_CpMoveTo.Side1.fZ = PROFILE->m_CpDanger[0].Side1.fZ;
			m_CpMoveTo.Side0.fXt = 0.0f;
			m_CpMoveTo.Side1.fXt = 0.0f;
			m_CpMoveTo.Side0.fYt = 0.0f;
			m_CpMoveTo.Side1.fYt = 0.0f;
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			m_CpMoveTo.Side0.fR = m_CpSurface.Side0.fR;
			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nSafeThreadAction = 110;
		}
		break;
	case 110:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Kinematics.HeadFromSurface(&theApp.m_Tank.CpSafe,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&theApp.m_Tank.CpSafe,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			m_CpMoveTo.Side0.fR = m_CpSurface.Side0.fR;
			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nSafeThreadAction = 999;
		}
		break;

	case 200:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetSurfacePos(&m_CpSurface);
			m_CpMoveTo = m_CpSurface;
			PROFILE->CalculateTurntableDangerZone();
			m_CpMoveTo.Side0.fX = PROFILE->m_CpDanger[0].Side0.fX;
			m_CpMoveTo.Side1.fX = PROFILE->m_CpDanger[0].Side1.fX;
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			m_CpMoveTo.Side0.fR = m_CpSurface.Side0.fR;
			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nSafeThreadAction = 210;
		}

		break;
	case 210:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetSurfacePos(&m_CpSurface);
			m_CpMoveTo = m_CpSurface;
			m_CpMoveTo.Side0.fZ = PROFILE->m_CpDanger[0].Side0.fZ;
			m_CpMoveTo.Side1.fZ = PROFILE->m_CpDanger[0].Side1.fZ;
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			m_CpMoveTo.Side0.fR = m_CpSurface.Side0.fR;
			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nSafeThreadAction = 999;
		}

		break;

	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			m_nSafeThreadAction=0;
			m_nSubThreadAction = 0;
		}

		break;
	}
}



void CMainThread::ContinuousTurntableScan()
{
	static int nLastCoordinate;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	static CCoord	CpHead,CpSurface,Cp, CpDanger;
	CCoord Cp0,Cp1;
	int nAxis;
	float fDeltaX;

	switch(m_nSubThreadAction) {
	case 0:
		m_bAbortFlag = FALSE;
		m_bPause = FALSE;
		theApp.m_bScanning=TRUE;
		theApp.m_Motors.SuspendMotors();
		m_nRevCounter =  m_nScanLine = m_nScanStartLine;
		theApp.m_Motors.SetRevCount(m_nRevCounter);
		m_bScanLineUpdateByMotorRevCounter=true;

		theApp.m_Motors.GetHeadPos(&CpHead);

		CpSurface = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];
		CpSurface.Side0.fR = CpHead.Side0.fR;
		theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
		theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

		switch(theApp.m_PmacUser.m_nScanSide) {
		case 0:	m_CpMoveTo.CopySide(STARBOARD,CpHead);
			break;
		case 1:	m_CpMoveTo.CopySide(PORTSIDE,CpHead);
			break;
		}
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		m_nSubThreadAction=10;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.StartTurntableCartesianScanningAxes(m_CpMoveTo);
			theApp.m_Motors.StartContinuousRotate(theApp.m_Tank.nRLeft,m_fCurrentSpeed = PROFILE->m_fScanSpeed);
			theApp.m_Motors.CollectComplexData(TRUE);
			m_nSubThreadAction=20;
		};
		break;
	case 20:
		if(m_nRevCounter - theApp.m_Motors.GetRevCount()) {

			pFrame->SendMessage(INVALIDATE_VIEW);
			if(m_fCurrentSpeed - PROFILE->m_fScanSpeed) theApp.m_Motors.StartContinuousRotate(theApp.m_Tank.nRLeft,m_fCurrentSpeed = PROFILE->m_fScanSpeed);

		}
		m_nScanLine = m_nRevCounter = theApp.m_Motors.GetRevCount();

		if(m_nScanLine<PROFILE->m_nScanFinishLine) {
			theApp.m_Motors.GetHeadPos(&CpHead);

			Cp0 = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];
			Cp1 = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine+1];

			for(nAxis=0;nAxis<20;nAxis++) {
				CpSurface.fPos[nAxis] = (Cp1.fPos[nAxis] - Cp0.fPos[nAxis]) * CpHead.Side0.fR / 360.0f + Cp0.fPos[nAxis];
			}
			CpSurface.Side0.fR = CpHead.Side0.fR;

			if(theApp.m_LastSettings.nUseEccentricCompensation != 0) {
				fDeltaX = cosf((CpHead.Side0.fR-m_CpEccentricCentre.Side0.fR)  * DEG_TO_RAD) * m_CpEccentricCentre.fWp[0];
				CpSurface.Side0.fX -= fDeltaX;
				CpSurface.Side1.fX -= fDeltaX;
			}

			theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			switch(theApp.m_PmacUser.m_nScanSide) {
			case 0:	m_CpMoveTo.CopySide(STARBOARD,CpHead);
				break;
			case 1:	m_CpMoveTo.CopySide(PORTSIDE,CpHead);
				break;
			}
			theApp.m_Motors.UpdateTurntableCartesianScanningAxes(m_CpMoveTo);

		} else {
			m_bAbortFlag = TRUE;
		}

		if(m_bAbortFlag == TRUE) {
			theApp.m_Motors.CollectComplexData(FALSE);
			theApp.m_Motors.StopContinuousRotate(theApp.m_Tank.nRLeft);
			theApp.m_Motors.SuspendMotors();
			m_nSubThreadAction=999;
		}
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			m_nSafeThreadAction=0;
			m_nSubThreadAction = 0;
			theApp.m_bScanning=FALSE;
		}
		break;
	}


}

void CMainThread::SegmentTurntableScan()
{
	static int nLastCoordinate;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	static CCoord	CpHead,CpSurface,Cp, CpDanger;
	CCoord Cp0,Cp1;

	switch(m_nSubThreadAction) {
	case 0:
		m_bAbortFlag = FALSE;
		m_bPause = FALSE;
		theApp.m_bScanning=TRUE;
		theApp.m_Motors.SuspendMotors();

		theApp.m_Motors.GetHeadPos(&CpHead);

		CpSurface = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine=m_nScanStartLine];
		CpSurface.Side0.fR = PROFILE->m_CpRect[0].Side0.fR;
		theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
		theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

		switch(theApp.m_PmacUser.m_nScanSide) {
		case 0:	m_CpMoveTo.CopySide(STARBOARD,CpHead);
			break;
		case 1:	m_CpMoveTo.CopySide(PORTSIDE,CpHead);
			break;
		}
		theApp.m_Motors.EnableAxes(BOTH_MOVETO_WITH_R);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		m_nDir = 0;
		m_nSubThreadAction=10;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			CpSurface = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];

			if(m_nDir & 1) {
				CpSurface.Side0.fR = PROFILE->m_CpRect[0].Side0.fR;
			} else {
				CpSurface.Side0.fR = PROFILE->m_CpRect[1].Side0.fR;
			}

			theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			switch(theApp.m_PmacUser.m_nScanSide) {
			case 0:	m_CpMoveTo.CopySide(STARBOARD,CpHead);
				break;
			case 1:	m_CpMoveTo.CopySide(PORTSIDE,CpHead);
				break;
			}
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);

			theApp.m_Motors.CollectComplexData(TRUE);
			m_nSubThreadAction=20;
		};
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.CollectComplexData(FALSE);

			theApp.m_pScanData->SaveVolumeDuringScan(m_nScanLine-m_nScanStartLine);

			if(PROFILE->m_nLineIncrement<=0) PROFILE->m_nLineIncrement = 1;

			m_nDir ^= 1;
			m_nScanLine+=PROFILE->m_nLineIncrement;
			if((m_nScanLine<PROFILE->m_nScanFinishLine) && (m_bAbortFlag==FALSE)) {
				m_nSubThreadAction = 10;
			} else {
				theApp.m_Motors.CollectComplexData(FALSE);
				m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
			}

			pFrame->SendMessage(INVALIDATE_VIEW);
			break;
		}
		break;
	}

}

void CMainThread::TurnTableScan()
{
	switch(theApp.m_Tank.nScannerDescription) {
	default: TurnTableScanPreFieldBus();
		break;
	case TANK_6AXIS:
	case TANK_5AXIS:
	case SPX_ROBOT: TurnTableScanKinematic();
		break;
	case TWIN_TOWER_KINEMATIC:
	case TRACKED_PROBE:
		break;
	}
}


void CMainThread::TurnTableScanPreFieldBus()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CCoord Cp0,Cp1,CpHead,CpSurface;
	int nAxis,nNextScanLine,nTS;
	float fDeltaX;
	CString Buff;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.StartLogging();
		theApp.m_Motors.CollectComplexData(FALSE);
		m_nLastGainValue = 0;
		m_bAbortFlag = FALSE;
		m_bPause = FALSE;
		theApp.m_Motors.SuspendMotors();
		m_nRevCounter = m_nScanLine = m_nScanStartLine;
		theApp.m_Motors.SetRevCount(m_nRevCounter);

		theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide;
		m_nMoveSide = 3;									//Check to see if both sides need moving NEEDS Doing. from mexicalli

		if(m_bMoveToStartAtBeginningOfScan!=TRUE) {
			m_nSubThreadAction = 100;
			return;
		}
		m_CpMoveTo = PROFILE->m_ScanLine[0].m_pCp[m_nScanStartLine];
		switch(theApp.m_nUseTurntableOrRollers) {
		case 0:	m_nSubThreadAction = 10;
			break;
		case 1: m_nSubThreadAction = 20;
			break;
		}
		m_nScanFinishLine = theApp.m_pScanData->m_nScanFinishLine;

		break;
	case 10:
		CpSurface = PROFILE->m_ScanLine[0].m_pCp[m_nScanStartLine];
		CpSurface.Side0.fR = 2.0f;
		m_CpMoveTo = CpSurface;
		if(theApp.m_LastSettings.nScanMask & _SCAN_START_VIA_DANGER_PLANE) {
			MovetoViaTurntableDangerPlane();
		} else {
			MovetoStartWithoutDangerPlane();
		}
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nSubThreadAction = 100;
		}
		break;

	case 100:
		m_bMoveTurnTable = true;
		switch(PROFILE->m_ScanLine[0].m_pCp[m_nScanLine].nType) {
		default:
		case CONTINUOUS_TURNTABLE:
			m_nRevCounter = m_nScanLine;
			theApp.m_Motors.SetRevCount(m_nRevCounter);
			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.EnableAxes(BOTH_MOVETO_WITHOUT_R);
			theApp.m_Motors.StartTurntableCartesianScanningAxes(m_CpMoveTo);
			switch(theApp.m_nUseTurntableOrRollers) {
			default:
				theApp.m_Motors.StartContinuousRotate(theApp.m_Tank.nRLeft,m_fCurrentSpeed = PROFILE->m_fScanSpeed);
				break;
			case 1:
				theApp.m_Motors.StartContinuousRotate(theApp.m_Tank.nRRight,m_fCurrentSpeed = PROFILE->m_fScanSpeed);
				break;
			}

			theApp.m_Motors.CollectComplexData(TRUE);
			m_bScanLineUpdateByMotorRevCounter=true;
			m_nSubThreadAction = 200;
			break;
		case SEGMENT_TURNTABLE:
			m_nRevCounter = m_nScanLine;
			theApp.m_Motors.SetRevCount(m_nRevCounter);

			m_CpSurface = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];

			theApp.m_Kinematics.TipsFromNormals(&m_CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&m_CpSurface,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			theApp.m_Motors.EnableAxes(BOTH_MOVETO_WITH_R);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nDir = 0;
			m_bScanLineUpdateByMotorRevCounter=false;
			theApp.m_Motors.CollectComplexData(TRUE);
			m_nSubThreadAction = 300;
			break;
		case JUMP_TURNTABLE:
			theApp.m_Motors.CollectComplexData(FALSE);
			theApp.m_Motors.SuspendMotors();
			for(m_nScanLine;m_nScanLine < PROFILE->m_ScanLine[0].m_nCoordL;m_nScanLine++) {
				if(PROFILE->m_ScanLine[0].m_pCp[m_nScanLine].nType != JUMP_TURNTABLE) {
					theApp.m_Motors.GetSurfacePos(&m_CpSurface);
					m_CpMoveTo = m_CpSurface;
					m_CpMoveTo.Side0.fX = PROFILE->m_CpDanger[0].Side0.fX;
					m_CpMoveTo.Side1.fX = PROFILE->m_CpDanger[0].Side1.fX;
					theApp.m_Kinematics.TipsFromNormals(&m_CpMoveTo,PORTSIDE);
					theApp.m_Kinematics.TipsFromNormals(&m_CpMoveTo,STARBOARD);
					theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
					theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

					theApp.m_Motors.EnableAxes(BOTH_MOVETO);
					theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
					theApp.m_Motors.MoveTo(DONT_WAIT);
					m_nSubThreadAction = 400;
					return;
					break;
				}
			}
			m_nSubThreadAction = 999;
			break;
		}
		break;

	case 200:	//Continuous Turntable scan
		if(m_nRevCounter - theApp.m_Motors.GetRevCount()) {

			theApp.m_pScanData->FillForDPCError(m_nScanLine-PROFILE->m_nScanStartLine);
			theApp.m_pScanData->SaveVolumeDuringScan(m_nScanLine-PROFILE->m_nScanStartLine);

			pFrame->SendMessage(INVALIDATE_VIEW);
			if(m_fCurrentSpeed - PROFILE->m_fScanSpeed) {
				switch(theApp.m_nUseTurntableOrRollers) {
				default:
					theApp.m_Motors.StartContinuousRotate(theApp.m_Tank.nRLeft,m_fCurrentSpeed = PROFILE->m_fScanSpeed);
					break;
				case 1:
					theApp.m_Motors.StartContinuousRotate(theApp.m_Tank.nRRight,m_fCurrentSpeed = PROFILE->m_fScanSpeed);
					break;
				}
			}
		//	m_nScanLine++; //this is updated in m_Data.CollectComplexData();
			m_nRevCounter = theApp.m_Motors.GetRevCount();
			if(m_nScanLine >= m_nScanFinishLine) {
				theApp.m_Motors.CollectComplexData(FALSE);
				theApp.m_Motors.StopContinuousRotate(theApp.m_Tank.nRLeft);
				theApp.m_Motors.SuspendMotors();
				m_nSubThreadAction=999;
				return;
			}
			if(PROFILE->m_ScanLine[0].m_pCp[m_nScanLine].nType == CONTINUOUS_TURNTABLE) {
				m_nSubThreadAction = 200;
			} else {
				m_nSubThreadAction = 100;
				return;
			}
		}
		if(m_bAbortFlag == true) {
			theApp.m_Motors.CollectComplexData(FALSE);
			theApp.m_Motors.StopContinuousRotate(theApp.m_Tank.nRLeft);
			theApp.m_Motors.SuspendMotors();
			m_nSubThreadAction=999;
			return;
		}

		theApp.m_Motors.GetHeadPos(&CpHead);

		
		if((CpHead.Side0.fR > 0.1f) && (CpHead.Side0.fR < 350.0f)) {
			Cp0 = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];

			nNextScanLine = m_nScanLine+1;
			if(nNextScanLine >= PROFILE->m_ScanLine[0].m_nCoordL) nNextScanLine = PROFILE->m_ScanLine[0].m_nCoordL - 1;
			
			Cp1 = PROFILE->m_ScanLine[0].m_pCp[nNextScanLine];

			theApp.m_UtUser.TurntableGainChange(Cp0);

			if(theApp.m_nUseTurntableOrRollers == 0) {
				for(nAxis=0;nAxis<20;nAxis++) {
					m_CpSurface.fPos[nAxis] = (((Cp1.fPos[nAxis] - Cp0.fPos[nAxis]) * CpHead.Side0.fR) / 360.0f) + Cp0.fPos[nAxis];
				}
			} else {
				for(nAxis=0;nAxis<20;nAxis++) {
					m_CpSurface.fPos[nAxis] = (((Cp1.fPos[nAxis] - Cp0.fPos[nAxis]) * CpHead.Side1.fR) / 360.0f) + Cp0.fPos[nAxis];
				}
			}
			m_CpSurface.Side0.fR = CpHead.Side0.fR;
			m_CpSurface.Side1.fR = CpHead.Side1.fR;


			if(theApp.m_LastSettings.nUseEccentricCompensation != 0) {
				fDeltaX = cosf((CpHead.Side0.fR-m_CpEccentricCentre.Side0.fR)  * DEG_TO_RAD) * m_CpEccentricCentre.fWp[0];
				m_CpSurface.Side0.fX -= fDeltaX;
				m_CpSurface.Side1.fX -= fDeltaX;
			}

			theApp.m_Kinematics.TipsFromNormals(&m_CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&m_CpSurface,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			Buff.Format(L"%d %d %d     %d    %.01f %.01f %.01f %.01f %.01f %.01f\r\n",m_nScanLine,nNextScanLine,theApp.m_Axes[10].nRevCounter,theApp.m_PmacUser.m_nMachinePosition[10],
				m_CpMoveTo.Side0.fX,m_CpMoveTo.Side0.fY,m_CpMoveTo.Side0.fZ,m_CpMoveTo.Side0.fXt,m_CpMoveTo.Side0.fY,m_CpMoveTo.Side0.fR);
			theApp.AddToLogging(&Buff);


			theApp.m_Motors.UpdateTurntableCartesianScanningAxes(m_CpMoveTo);
		}
		break;


	case 300:	//Segment Turntable scan
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_CpSurface = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];
			m_CpMoveTo = m_CpSurface;

			theApp.m_UtUser.TurntableGainChange(m_CpSurface);
			if(m_nDir & 1) {
				if(theApp.m_nUseTurntableOrRollers == 0) {
					m_CpMoveTo.Side0.fR = m_CpSurface.fAngle[0];
				} else {
					m_CpMoveTo.Side1.fR = m_CpSurface.fAngle[0];
				}
			} else {
				if(theApp.m_nUseTurntableOrRollers == 0) {
					m_CpMoveTo.Side0.fR = m_CpSurface.fAngle[1];
				} else {
					m_CpMoveTo.Side1.fR = m_CpSurface.fAngle[1];
				}
			}

			theApp.m_Kinematics.TipsFromNormals(&m_CpMoveTo,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&m_CpMoveTo,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);

			m_nSubThreadAction=310;
		}
		break;
	case 310:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			pFrame->SendMessage(INVALIDATE_VIEW);
			m_nScanLine++;
			if((m_nScanLine >= PROFILE->m_nScanFinishLine) || (m_bAbortFlag == true)) {
				theApp.m_Motors.CollectComplexData(FALSE);
				m_nSubThreadAction=999;
				return;
			}
			m_nDir ^= 1;

			if(PROFILE->m_ScanLine[0].m_pCp[m_nScanLine].nType == SEGMENT_TURNTABLE) {
				m_nSubThreadAction = 300;
			} else {
				m_nSubThreadAction = 100;
			}
		}
		break;

	case 400:	//Jump Turntable
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_CpMoveTo = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];
			m_CpMoveTo.Side0.fX = PROFILE->m_CpDanger[0].Side0.fX;
			m_CpMoveTo.Side1.fX = PROFILE->m_CpDanger[0].Side1.fX;

			theApp.m_Kinematics.TipsFromNormals(&m_CpMoveTo,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&m_CpMoveTo,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);

			m_nSubThreadAction=410;
		}
		break;
	case 410:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_CpMoveTo = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];

			theApp.m_Kinematics.TipsFromNormals(&m_CpMoveTo,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&m_CpMoveTo,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);

			m_nSubThreadAction=420;
		}
		break;
	case 420:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nSubThreadAction=100;
		}
		break;


	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			m_nSafeThreadAction=0;
			m_nSubThreadAction = 0;
			theApp.m_bScanning=FALSE;
			for(nTS=0;nTS<3;nTS++) {
				m_CpSurface.nGain[0]=0;
				m_CpSurface.nTS=nTS;
				theApp.m_UtUser.TurntableGainChange(m_CpSurface);
			}
			theApp.StopLogging();
		}

		break;

	}
}

void CMainThread::MovetoViaTurntableDangerPlane()
{
	static CCoord CpSurface,CpHead,Cp,CpDanger,CpDestination;
	D3DXVECTOR3 vecP;

	switch(m_nDangerPlaneThreadAction) {
	case 0:
		CpDestination = m_CpMoveTo;
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.GetSurfacePos(&m_CpSurface);
		PROFILE->CalculateDangerZone();


		vecP = m_CpSurface.Side1.pt;
		D3DXVec3Normalize((D3DXVECTOR3*)&vecP, (D3DXVECTOR3*)&vecP);
		vecP *= PROFILE->m_CpDanger[0].fRadius[STARBOARD];

		if(m_nMoveSide & 1) {
			if(PROFILE->m_vecToDangerPlane[0].x) m_CpSurface.Side0.fX = PROFILE->m_CpDanger[0].Side0.fX;
			if(PROFILE->m_vecToDangerPlane[0].y) m_CpSurface.Side0.fY = PROFILE->m_CpDanger[0].Side0.fY;
			if(PROFILE->m_vecToDangerPlane[0].z) m_CpSurface.Side0.fZ = PROFILE->m_CpDanger[0].Side0.fZ;
		}
		if(m_nMoveSide & 2) {
			m_CpSurface.Side1.pt.x = vecP.x;
			m_CpSurface.Side1.pt.y = vecP.y;
		}

		theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

		m_bMoveTurnTable = false;
		if(m_bMoveTurnTable==false) {
			theApp.m_Motors.GetTurntablePos(&m_CpMoveTo);
		}

		theApp.m_Axes[theApp.m_Tank.nRLeft].nDesireRevCount = theApp.m_Axes[theApp.m_Tank.nRLeft].nRevCounter;

		m_CpHead = m_CpMoveTo;
	case 2:
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpHead);
		theApp.m_Motors.MoveTo(DONT_WAIT);
			theApp.m_FBCtrl.JogJustR(CpDestination.Side0.fR, PROFILE->m_fScanSpeed);
		m_nDangerPlaneThreadAction = 90;
		Sleep(300);
		break;
	case 90:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nDangerPlaneThreadAction = 100;
		};
		break;
	case 100:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			if(theApp.m_Motors.IsItAtDesirePos() != true) {
				m_nDangerPlaneThreadAction = 2;
				break;
			};


			theApp.m_Motors.GetSurfacePos(&m_CpSurface);

			if(m_nMoveSide & 1) {
				if(PROFILE->m_vecToDangerPlane[0].x == 0.0f) m_CpSurface.Side0.fY = CpDestination.Side0.fY;
				if(PROFILE->m_vecToDangerPlane[0].y == 0.0f) m_CpSurface.Side0.fX = CpDestination.Side0.fX;
				if(PROFILE->m_vecToDangerPlane[0].z == 0.0f) m_CpSurface.Side0.fZ = CpDestination.Side0.fZ;
			}
			if(m_nMoveSide & 2) {
				vecP = CpDestination.Side1.pt;
				D3DXVec3Normalize((D3DXVECTOR3*)&vecP, (D3DXVECTOR3*)&vecP);
				vecP *= PROFILE->m_CpDanger[0].fRadius[STARBOARD];
				m_CpSurface.Side1.pt.x = vecP.x;
				m_CpSurface.Side1.pt.y = vecP.y;
				m_CpSurface.Side1.fZ = CpDestination.Side1.fZ;
			}

			theApp.m_Kinematics.TipsFromNormals(&m_CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&m_CpSurface,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			theApp.m_Motors.SuspendMotors();
			if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable!=TRUE) {
				theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			} else {
				m_CpMoveTo.Side0.fR = CpDestination.Side0.fR;
				theApp.m_Motors.EnableAxes(BOTH_MOVETO_WITH_R);
			}
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			theApp.m_FBCtrl.JogJustR(CpDestination.Side0.fR, PROFILE->m_fScanSpeed);
			Sleep(300);
			m_nDangerPlaneThreadAction = 105;
		}
		break;
	case 105:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nDangerPlaneThreadAction = 110;
		};
		break;
	case 110:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetSurfacePos(&m_CpSurface);
			if(m_nMoveSide & 1) {
				m_CpSurface.Side0 = CpDestination.Side0;
			}
			if(m_nMoveSide & 2) {
				m_CpSurface.Side1 = CpDestination.Side1;
			}
			theApp.m_Kinematics.TipsFromNormals(&m_CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&m_CpSurface,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);

			theApp.m_FBCtrl.JogJustR(CpDestination.Side0.fR, PROFILE->m_fScanSpeed);

			Sleep(300);
			m_nDangerPlaneThreadAction = 997;
		}
		break;
	case 997:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nDangerPlaneThreadAction = 998;
		};
		break;
	case 998:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nDangerPlaneThreadAction = 999;
		}
		break;
	case 999:
		if(theApp.m_Motors.IsMoveRFinished() == TRUE) {
			if(m_nThreadAction == MOVETO_VIA_DANGER_PLANE) {
				m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
			} else {
				m_nSubThreadAction += 10;						//Add 10 because calling routine was a subthread anyway
			}
		}

		break;
	}

}


void CMainThread::InvalidateToolPath()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if(pFrame->m_pScanView) {
		if(pFrame->m_pScanView->m_pToolPathPage) {
			pFrame->m_pScanView->m_pToolPathPage->FetchToolPath();
		}
	}
}

void CMainThread::OriginXYZRThetaLeft()
{
	switch(m_nSubThreadAction) {
	case 0:
		/*theApp.ActivityLog(L"Origin XYZRTheta");
		theApp.m_Motors.SuspendMotors();
		theApp.m_PmacUser.OriginRTheta(theApp.m_Tank.nXtLeft);
		*/m_nSubThreadAction = 10;
		break;
	case 10:
	//	if(theApp.m_Motors.IsMotionFinished() == TRUE) {
	//		theApp.m_PmacUser.OriginRTheta(theApp.m_Tank.nYtLeft);
			m_nSubThreadAction = 20;
	//	}
		break;
	case 20:
	//	if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_PmacUser.OriginAll(ORIGIN_XYZ_LEFT);
			m_nSubThreadAction = 999;
	//	}
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			m_nSubThreadAction=0;
			theApp.ActivityLog(L"Completed");
		}
		break;

	}
}

void CMainThread::ProbeCrossSection()
{
	CCoord	Cp,CpSurface,CpHead,CpTemp,Cp0,Cp1;
	int nNotSide,nSide;
	int nLine,nColoumn,nn,nAxis,nSample;
	float fMaxDeviation,fTemp;
	int nPrimaryAxis,nSamplesLine,nNumberLines;
	CString Buff;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	float fWidth = -1.0f;
	float fHeight = -1.0f;
	if(m_pNormalizePage) {
		fWidth = theApp.m_LastSettings.fPeakDistance;
		fHeight = theApp.m_LastSettings.fPeakDistance;
		nSamplesLine = 100;
		nNumberLines = 30;
	}
	if(m_pProbeBeamSectionsPage) {
		fWidth = theApp.m_LastSettings.fProbeFastScanSize;
		fHeight = theApp.m_LastSettings.fProbeSlowScanSize;
		nSamplesLine = (int)(theApp.m_LastSettings.fProbeFastScanSize / theApp.m_LastSettings.fProbeFastIncrement);
		nNumberLines = (int)(theApp.m_LastSettings.fProbeSlowScanSize / theApp.m_LastSettings.fProbeSlowIncrement);
	}
	if((fWidth <= 0.0f) || (fHeight <= 0.0f))
		return;



	D3DXMATRIXA16 matWorld,matNormal,matTranslation;
	D3DXVECTOR3 vecP;

	nSide=theApp.m_PmacUser.m_nScanSide=m_nNormSide;
	nSide==PORTSIDE ? nNotSide = STARBOARD : nNotSide = PORTSIDE;

	switch(m_nSubThreadAction) {
	case 0:
		m_bAbortFlag = FALSE;
		m_bPause = FALSE;
		theApp.m_Motors.GetHeadPos(&CpHead);
		CpHead.Side0.fXt = 0.0f;
		CpHead.Side0.fYt = 0.0f;
		CpHead.Side0.fI=0.0f;
		CpHead.Side0.fJ=-1.0f;
		CpHead.Side0.fK=0.0f;
		m_ToolPath.Zero();
		m_ToolPath.Add(CpHead);
		m_ToolPath.Add(CpHead);

		D3DXMatrixTranslation(&matTranslation,CpHead.Side[nSide].fX,CpHead.Side[nSide].fZ,CpHead.Side[nSide].fY);
		D3DXMatrixRotationYawPitchRoll(&matNormal,CpHead.Side[nSide].fI * PIf / -2.0f,CpHead.Side[nSide].fK * PIf / 2.0f,0.0f);
		D3DXMatrixMultiply(&matWorld,&matNormal,&matTranslation);

		for(nn=0,nLine=0;nLine<2;nLine++) {
			for(nColoumn=0;nColoumn<2;nColoumn++,nn++) {
				m_CpRect[nn] = CpHead;
				vecP.x = (fWidth / -2) + ((float)nColoumn * fWidth);
				vecP.y = (fHeight / -2) + ((float)nLine * fHeight);
				vecP.z = 0.0f;
				D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
				m_CpRect[nn].Side[nSide].fX = vecP.x;
				m_CpRect[nn].Side[nSide].fY = vecP.z;
				m_CpRect[nn].Side[nSide].fZ = vecP.y;
			}
		}

		m_bPause = FALSE;
		theApp.m_bScanning=TRUE;
		theApp.m_Motors.SuspendMotors();
		m_CpMoveTo = m_CpRect[0];
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		Sleep(300);
		m_nSubThreadAction=10;

		break;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nScanLine = 0;
			m_nNumberScanLines=nNumberLines;
			m_nSubThreadAction=20;
		}
		break;
	case 20:
		if((m_nScanLine < m_nNumberScanLines) && (m_bAbortFlag == FALSE)) {
			fMaxDeviation = 0.0f;
			nPrimaryAxis = 0;
			for(nAxis=0;nAxis<20;nAxis++) {
				Cp0.fPos[nAxis] = ((m_CpRect[2].fPos[nAxis] - m_CpRect[0].fPos[nAxis]) * (float)m_nScanLine / (float)(m_nNumberScanLines - 1)) + m_CpRect[0].fPos[nAxis];
				Cp1.fPos[nAxis] = ((m_CpRect[3].fPos[nAxis] - m_CpRect[1].fPos[nAxis]) * (float)m_nScanLine / (float)(m_nNumberScanLines - 1)) + m_CpRect[1].fPos[nAxis];
				fTemp = (float)fabs(Cp1.fPos[nAxis] - Cp0.fPos[nAxis]);
				if(fMaxDeviation < fTemp) {
					fMaxDeviation = fTemp;
					nPrimaryAxis = nAxis;
				}
			}
			m_ToolPath.ModifyCoord(0,Cp0);
			m_ToolPath.ModifyCoord(1,Cp1);
			m_CpNormal.AllocateCoords(nSamplesLine);
			m_CpNormal.ClearMemory();
			m_ToolPath.SetPrimaryAxis(nPrimaryAxis);
			m_CpNormal.SetPrimaryAxis(nPrimaryAxis);
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[nSamplesLine-1] = m_ToolPath.m_pCp[1];
			m_CpNormal.m_pCp[0].nAmp=m_CpNormal.m_pCp[nSamplesLine-1].nAmp=0;

			theApp.m_bCollectNormalizeData=TRUE;
			theApp.m_Si10User.m_Si10.bCollectData = TRUE;

			m_CpMoveTo = m_ToolPath.m_pCp[(m_nScanLine+1) & 1];
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(300);
			m_nSubThreadAction = 30;
		} else {
			if(m_pNormalizePage) {
				m_pNormalizePage->FindRectilinearCentre(&nSample,&nLine);
			}
			if(m_pProbeBeamSectionsPage) {
				m_pProbeBeamSectionsPage->FindRectilinearCentre(&nSample,&nLine);
			}
			for(nAxis=0;nAxis<20;nAxis++) {
				Cp0.fPos[nAxis] = ((m_CpRect[2].fPos[nAxis] - m_CpRect[0].fPos[nAxis]) * (float)nLine / (float)(m_nNumberScanLines - 1)) + m_CpRect[0].fPos[nAxis];
				Cp1.fPos[nAxis] = ((m_CpRect[3].fPos[nAxis] - m_CpRect[1].fPos[nAxis]) * (float)nLine / (float)(m_nNumberScanLines - 1)) + m_CpRect[1].fPos[nAxis];
			}
			for(nAxis=0;nAxis<20;nAxis++) {
				m_CpMoveTo.fPos[nAxis] = ((Cp1.fPos[nAxis] - Cp0.fPos[nAxis]) * (float)nSample / (float)(m_CpNormal.m_nCoordL - 1)) + Cp0.fPos[nAxis];
			}
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(300);
			m_nSubThreadAction = 40;
		}
		break;
	case 30:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_bCollectNormalizeData=FALSE;
			theApp.m_Si10User.m_Si10.bCollectData = FALSE;
			if(m_pNormalizePage) {
				m_pNormalizePage->TransferRectilinearData(&m_CpNormal,m_nNumberScanLines,m_nScanLine);
				m_pNormalizePage->Invalidate(FALSE);
			}
			if(m_pProbeBeamSectionsPage) {
				m_pProbeBeamSectionsPage->TransferRectilinearData(&m_CpNormal,m_nNumberScanLines,m_nScanLine);
				m_pProbeBeamSectionsPage->Invalidate(FALSE);
			}
			m_nScanLine++;
			m_nSubThreadAction = 20;
		}
		break;
	case 40:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nSubThreadAction = 0;
			m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
			if(m_pProbeBeamSectionsPage) {
				m_pProbeBeamSectionsPage->TransferScanToBeamProfile(); // done hear to ensure the probe is in the centre
			}
		}
		break;
	}

}

void CMainThread::TrackAlongLine()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CCoord Cp,CpSurface;

	switch(m_nSubThreadAction) {
	case 0:
		m_bAbortFlag = FALSE;
		m_bPause = FALSE;
		m_nSubThreadAction = 10;
		break;
	case 10:
		m_CpNormal.AllocateCoords(100);

		switch(theApp.m_nEditLineType) {
		case TAUGHTLINES:
			if(PROFILE->m_nTaughtLineL<=0) {
				m_nSubThreadAction=999;
				return;
			}
			PROFILE->m_TaughtLine[theApp.m_nLine].GetCoord(0,&m_CpNormal.m_pCp[0]);
			PROFILE->m_TaughtLine[theApp.m_nLine].GetLastCoord(&m_CpNormal.m_pCp[99]);

			CpSurface = PROFILE->m_TaughtLine[theApp.m_nLine].m_pCp[0];
			m_CpMoveTo = CpSurface;
			PROFILE->SetToolPathFromLine(&PROFILE->m_TaughtLine[theApp.m_nLine]);
			break;
		case SCANLINES:
			if(PROFILE->m_nScanLineL<=0) {
				m_nSubThreadAction=999;
				return;
			}
			PROFILE->m_ScanLine[theApp.m_nLine].GetCoord(0,&m_CpNormal.m_pCp[0]);
			PROFILE->m_ScanLine[theApp.m_nLine].GetLastCoord(&m_CpNormal.m_pCp[99]);

			CpSurface = PROFILE->m_ScanLine[theApp.m_nLine].m_pCp[0];
			m_CpMoveTo = CpSurface;
			PROFILE->SetToolPathFromLine(&PROFILE->m_ScanLine[theApp.m_nLine]);
			break;
		default: m_nSubThreadAction = 999;
			return;
			break;
		}

		PROFILE->AvoidExclusionZones(&m_CpMoveTo);
		if(theApp.m_LastSettings.nScanMask & _SCAN_START_VIA_DANGER_PLANE) {
			MovetoViaDangerPlane();
		} else {
			MovetoStartWithoutDangerPlane();
		}
		break;
	case 20:
		theApp.m_Motors.SuspendMotors();

		switch(theApp.m_PmacUser.m_nScanSide = 0) {
		case 0:	theApp.m_Motors.EnableAxes(LEFT_POLY_MOVE);
			break;
		case 1:	theApp.m_Motors.EnableAxes(RIGHT_POLY_MOVE);
			break;
		case 2:	theApp.m_Motors.EnableAxes(BOTH_POLY_MOVE);
			break;
		}
		theApp.m_Motors.SetScanSpeed(PROFILE->m_fScanSpeed * theApp.m_LastSettings.fFeedRate / 100.0f);
		theApp.m_Motors.DownLoadPolyMoveCoordinates(&PROFILE->m_ToolPath, 0);
		theApp.m_Motors.SetupLookAhead();

		PROFILE->m_ToolPath.m_nPrimaryAxis = 0;
		theApp.m_bCollectWaterPath=TRUE;

		theApp.m_Motors.StartPolyMoveWithLookAhead(m_ToolPath.m_nCoordL);
		m_nSubThreadAction=999;

		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_bCollectWaterPath=FALSE;

			theApp.m_Motors.SuspendMotors();

			m_CpNormal.DecimateUsingZPos(theApp.m_LastSettings.fWpVariation);

			switch(theApp.m_nEditLineType) {
			case TAUGHTLINES:
				PROFILE->m_TaughtLine[theApp.m_nLine] = &m_CpNormal;
				break;
			case SCANLINES:
				PROFILE->m_ScanLine[theApp.m_nLine] = &m_CpNormal;
				break;
			}
			pFrame->SendMessage(UI_UPDATE_PROFILE_DLG,NULL,NULL);

			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}

}

void CMainThread::CollectWaterPathData()
{
	CCoord CpSurface,CpHead;
	int nSample;
	int nOldMode = theApp.m_LastSettings.nPositionDisplayMode;
	static int nLastSample;

	theApp.m_LastSettings.nPositionDisplayMode = 3;
	theApp.m_Motors.GetSurfacePos(&CpSurface);
	theApp.m_LastSettings.nPositionDisplayMode = nOldMode;

	theApp.m_Motors.GetHeadPos(&CpHead);

	float fXMin = PROFILE->m_ToolPath.m_pCp[0].fPos[PROFILE->m_ToolPath.m_nPrimaryAxis];
	float fXMax = PROFILE->m_ToolPath.m_pCp[PROFILE->m_ToolPath.m_nCoordL-1].fPos[PROFILE->m_ToolPath.m_nPrimaryAxis];
	nSample = (int)((CpHead.fPos[PROFILE->m_ToolPath.m_nPrimaryAxis]-fXMin) * (float)(m_CpNormal.m_nCoordL-1) / (fXMax - fXMin));

	if(nSample<0) nSample = 0;
	if(nSample>=m_CpNormal.m_nCoordL) nSample = m_CpNormal.m_nCoordL-1;
	if(nLastSample != nSample) {											//to protect coordinate of nSample 0
		m_CpNormal.m_pCp[nSample] = CpSurface;
	}
	if(nSample == (m_CpNormal.m_nCoordL - 2) ) {
		m_CpNormal.m_pCp[m_CpNormal.m_nCoordL-1] = CpSurface;
	}
	nLastSample = nSample;

}

void CMainThread::LaserTaughtLineScan()
{

}

/////////////////////////////////////////////////////////////
//External Events;
////////////////////////////////////////////////////////////
void CMainThread::PrfEvent()
{
	if(theApp.m_LastSettings.nExtDataCollectionStrobeSrc == 0) {
		unsigned int nPtr = m_nExtPtr++;
		nPtr%=EXT_SIZE;

		m_ExternalEvents[nPtr].dHRTime = m_HRTimer.GetTime();
		m_ExternalEvents[nPtr].nStatus = INTERNAL_PRF;
	}

}



void CMainThread::ContourScanOld()
{
	static int nLastCoordinate;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	static CCoord	CpHead,CpSurface,Cp, CpDanger;
	CCoord StartOfLine, FinishOfLine;



	switch(m_nSubThreadAction) {
	case 0: //move out of danger zone
		m_bAbortFlag = FALSE;
		m_bPause = FALSE;
		theApp.m_bScanning=TRUE;
		theApp.m_Motors.SuspendMotors();
		m_bScanLineUpdateByMotorRevCounter=false;
		PROFILE->CalculateDangerZone();

		theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
//		m_bMoveToStartAtBeginningOfScan=false;
		if(m_bMoveToStartAtBeginningOfScan!=TRUE) {
			m_nSubThreadAction = 30;
			return;
		}
		m_nSubThreadAction = 10;

		break;
	case 10:
		CpSurface = PROFILE->m_ScanLine[m_nScanStartLine].m_pCp[0];
		m_CpMoveTo = CpSurface;
		PROFILE->AvoidExclusionZones(&m_CpMoveTo);
		if(theApp.m_LastSettings.nScanMask & _SCAN_START_VIA_DANGER_PLANE) {
			MovetoViaDangerPlane();
		} else {
			MovetoStartWithoutDangerPlane();
		}

		//m_nSubThreadAction is incremented by ten at the end of the MovetoViaDangerplane
		break;
	case 20: //move to start
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetHeadPos(&Cp);

			CpSurface = PROFILE->m_ScanLine[m_nScanStartLine].m_pCp[0];
			PROFILE->AvoidExclusionZones(&CpSurface);

			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);

			switch(theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide) {
			case 0:
				CpHead.Side1.fX=Cp.Side1.fX;
				CpHead.Side1.fY=Cp.Side1.fY;
				CpHead.Side1.fZ=Cp.Side1.fZ;
				CpHead.Side1.fXt=Cp.Side1.fXt;
				CpHead.Side1.fYt=Cp.Side1.fYt;
				break;
			case 1:
				CpHead.Side0.fX=Cp.Side0.fX;
				CpHead.Side0.fY=Cp.Side0.fY;
				CpHead.Side0.fZ=Cp.Side0.fZ;
				CpHead.Side0.fXt=Cp.Side0.fXt;
				CpHead.Side0.fYt=Cp.Side0.fYt;
				break;
			case 2:
				break;
			}


			if((theApp.m_nJobNumber==GKN)||(theApp.m_nJobNumber==DOWTY_BSCAN)||(PROFILE->IsRUsed()))
				ContourScanRAxis(m_nScanStartLine);

			theApp.m_Thread.m_CpMoveTo = CpHead;
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
//			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nSubThreadAction=30;
			m_nScanLine = m_nScanStartLine;
		}
		break;
	case 30:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) { //Do first line
			theApp.m_Motors.SuspendMotors();

			PROFILE->CalculateToolPath(m_nScanLine=m_nScanStartLine,0,m_nDir=0);
			InvalidateToolPath();

			switch(theApp.m_PmacUser.m_nScanSide) {
			case 0:	theApp.m_Motors.EnableAxes(LEFT_POLY_MOVE);
				break;
			case 1:	theApp.m_Motors.EnableAxes(RIGHT_POLY_MOVE);
				break;
			case 2:	theApp.m_Motors.EnableAxes(BOTH_POLY_MOVE);
				break;
			}
			theApp.m_Motors.SetScanSpeed(PROFILE->m_fScanSpeed * theApp.m_LastSettings.fFeedRate / 100.0f);
			nLastCoordinate=theApp.m_Motors.DownLoadPolyMoveCoordinates(&PROFILE->m_ToolPath, m_nDir);
			theApp.m_Motors.SetupLookAhead();

			theApp.m_Motors.CollectComplexData(TRUE);

			theApp.m_Motors.StartPolyMoveWithLookAhead(PROFILE->m_ToolPath.m_nCoordL);
			m_nSubThreadAction=40;
		};

		break;
	case 40: //Do 2nd line onwards
		if(theApp.m_Motors.IsPolyMotionFinished() == TRUE) {

			theApp.m_Motors.CollectComplexData(FALSE);

//			theApp.m_pScanData->ShrinkAndShiftLine(m_nScanLine-m_nScanStartLine,PORTSIDE);
//			theApp.m_pScanData->FillForDPCError(m_nScanLine-PROFILE->m_nScanStartLine);

			switch(PROFILE->m_nLineIncrementMode) {
			case FIXED_INC:
				m_nLineIncrement = PROFILE->m_nLineIncrement;
				break;
			case VARIABLE_INC:
				m_nLineIncrement = PROFILE->m_ScanLine[m_nScanLine].m_nLineIncrement;
				theApp.m_pScanData->FillForLineIncrement(m_nScanLine-PROFILE->m_nScanStartLine,m_nLineIncrement);
				break;
			}
			if(m_nLineIncrement<=0) m_nLineIncrement = 1;
				theApp.m_pScanData->SaveVolumeDuringScan((m_nScanLine-PROFILE->m_nScanStartLine)/m_nLineIncrement);

			m_nDir ^= 1;
			if(theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30] != 2) {
				m_nScanLine+=(m_nLineIncrement * theApp.m_pScanData->m_nNumberProbesPerImage);
			} else {
				m_nScanLine+=theApp.m_pScanData->m_nNumberProbesPerImage;
			}
			if((m_nScanLine<PROFILE->m_nScanFinishLine) && (m_bAbortFlag==FALSE)) {
				if((theApp.m_nJobNumber==GKN)||(theApp.m_nJobNumber==DOWTY_BSCAN)||(PROFILE->IsRUsed()))
					ContourScanRAxis(m_nScanLine);
				
				PROFILE->CalculateToolPath(m_nScanLine,0,m_nDir);
				InvalidateToolPath();
				theApp.m_Motors.SetScanSpeed(PROFILE->m_fScanSpeed * theApp.m_LastSettings.fFeedRate / 100.0f);

				nLastCoordinate=theApp.m_Motors.DownLoadPolyMoveCoordinates(&PROFILE->m_ToolPath, m_nDir);

				theApp.m_Motors.CollectComplexData(TRUE);
				theApp.m_Motors.StartPolyMoveWithLookAhead(PROFILE->m_ToolPath.m_nCoordL);
				Sleep(100);
			} else {
				//if have filename save?
				m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
				theApp.ActivityLog(L"Scan finished naturaly");
			}

			pFrame->SendMessage(INVALIDATE_VIEW);

		} else {
			nLastCoordinate=theApp.m_Motors.DownLoadNextPolyMoveCoordinate(nLastCoordinate,&PROFILE->m_ToolPath, m_nDir);
			nLastCoordinate=theApp.m_Motors.DownLoadNextPolyMoveCoordinate(nLastCoordinate,&PROFILE->m_ToolPath, m_nDir);

		}
		break;
	}

}

void CMainThread::MovetoContourStart()
{
	CCoord CpSurface;

	if(theApp.m_nJobNumber==GKN){
		m_CpMoveTo = PROFILE->m_ScanLine[PROFILE->m_nScanStartLine].m_pCp[0];
		m_nSubThreadAction = 0;
		theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
		return;
	}

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_Motors.SuspendMotors();
		theApp.m_WS.CalculateDangerZone();
		if(theApp.m_LastSettings.nReverseScanLines == 0) {
			CpSurface = PROFILE->m_ScanLine[PROFILE->m_nScanStartLine].m_pCp[0];
		} else {
			CpSurface = PROFILE->m_ScanLine[PROFILE->m_nScanFinishLine].m_pCp[0];
		}
		if(theApp.m_LastSettings.bUseBrainCompensation == 1) 
			PROFILE->CalculateBrainCoord(PROFILE->m_nFastAxis,PROFILE->m_nSlowAxis,CpSurface);

		PROFILE->AvoidExclusionZones(&CpSurface);
		m_CpMoveTo = CpSurface;
		m_nSubThreadAction = 10;
		break;
	case 10:
		if(theApp.m_LastSettings.nScanMask & _SCAN_START_VIA_DANGER_PLANE) {
			MovetoViaDangerPlane();
		} else {
			MovetoStartWithoutDangerPlane();
		}

		//m_nSubThreadAction is incremented by ten at the end of the MovetoViaDangerplane
		break;
	case 20:
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}
}

void CMainThread::ContourScan()
{
	static int nLastCoordinate;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	static CCoord	CpHead,CpSurface,Cp, CpDanger;
	CCoord StartOfLine, FinishOfLine;
	static float fOldScanSpeed;

	if(theApp.m_nJobNumber == GKN) {
		ContourScanGKN();
		return;
	}

	switch(m_nSubThreadAction) {
	case 0: //move out of danger zone
		m_bAbortFlag = FALSE;
		m_bPause = FALSE;
		theApp.m_bScanning=TRUE;
		fOldScanSpeed = 0.0f;
		m_bScanLineUpdateByMotorRevCounter=false;
		theApp.m_WS.CalculateDangerZone();
		theApp.m_pScanData->GenerateSamplePositionVertices();
		theApp.m_Motors.RequestAllAccelerationsAndVelocities();
		theApp.m_pScanData->AllocateVolumeBuffersAndOffsets();

		m_nScanLine=m_nScanStartLine;

		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
		m_nDir=0;
		m_nDangerPlaneThreadAction = 0;
		m_nSubThreadAction+=10;
		Sleep(300);

		break;
	case 10:
		CpSurface = PROFILE->m_ScanLine[m_nScanStartLine].m_pCp[0];
		m_CpMoveTo = CpSurface;
		PROFILE->AvoidExclusionZones(&m_CpMoveTo);
		if(theApp.m_LastSettings.nScanMask & _SCAN_START_VIA_DANGER_PLANE) {
			MovetoViaDangerPlane();
		} else {
			MovetoStartWithoutDangerPlane();
		}

		//m_nSubThreadAction is incremented by ten at the end of the MovetoViaDangerplane
		break;
	case 20: //move to start
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetHeadPos(&Cp);

			PROFILE->CalculateToolPath(m_nScanLine,0,m_nDir);
			InvalidateToolPath();
			if(PROFILE->m_ToolPath.GetSize() > 40) {
				MessageBox(NULL,L"Too many way points\nScan aborted",L"Error",MB_ICONERROR);
				theApp.m_bScanning = false;
				theApp.m_Motors.ResetAllMaxSpeeds();
				m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
				theApp.ActivityLog(L"Scan finished due to too many waypoints");
				return;
			}

			if(m_nDir==0) {
				PROFILE->m_ToolPath.GetCoord(0,&CpHead);
				CpSurface = PROFILE->m_ScanLine[m_nScanLine].m_pCp[0];
			} else {
				PROFILE->m_ToolPath.GetLastCoord(&CpHead);
				CpSurface = PROFILE->m_ScanLine[m_nScanLine].m_pCp[ PROFILE->m_ScanLine[m_nScanLine].m_nCoordL-1 ];
			}

			switch(theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide) {
			case 0:
				CpHead.Side1.fX=Cp.Side1.fX;
				CpHead.Side1.fY=Cp.Side1.fY;
				CpHead.Side1.fZ=Cp.Side1.fZ;
				CpHead.Side1.fXt=Cp.Side1.fXt;
				CpHead.Side1.fYt=Cp.Side1.fYt;
				break;
			case 1:
				CpHead.Side0.fX=Cp.Side0.fX;
				CpHead.Side0.fY=Cp.Side0.fY;
				CpHead.Side0.fZ=Cp.Side0.fZ;
				CpHead.Side0.fXt=Cp.Side0.fXt;
				CpHead.Side0.fYt=Cp.Side0.fYt;
				break;
			case 2:
				break;
			}

			if((theApp.m_nJobNumber==GKN)||(theApp.m_nJobNumber==DOWTY_BSCAN)||(PROFILE->IsRUsed()))
				ContourScanRAxis(m_nScanLine);

			if(CpHead.nType != 1) {
				theApp.m_Thread.m_CpMoveTo = CpHead;
				theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
				m_nSubThreadAction+=20;
			} else {
				theApp.m_Thread.m_CpMoveTo = CpSurface;
				m_nScanStartLine=m_nScanLine;
				m_nSubThreadAction+=10;
			}

		}
		break;
	case 30:
		MovetoViaDangerPlane();
		break;
	case 40:
//		if(theApp.m_Motors.IsMotionStarted() == TRUE) { //Do first line
			m_nSubThreadAction+=10;
			m_nDangerPlaneThreadAction = 0;
//		}
		break;
	case 50:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) { //Do first line
			theApp.m_Motors.SuspendMotors();

			switch(theApp.m_PmacUser.m_nScanSide) {
			case 0:	theApp.m_Motors.EnableAxes(LEFT_POLY_MOVE);
				break;
			case 1:	theApp.m_Motors.EnableAxes(RIGHT_POLY_MOVE);
				break;
			case 2:	theApp.m_Motors.EnableAxes(BOTH_POLY_MOVE);
				break;
			}
			theApp.m_LastSettings.fFeedRate = 100.0f;
			theApp.m_Motors.SetScanSpeed(PROFILE->m_fScanSpeed * theApp.m_LastSettings.fFeedRate / 100.0f);
			nLastCoordinate=theApp.m_Motors.DownLoadPolyMoveCoordinates(&PROFILE->m_ToolPath, m_nDir);
			theApp.m_Motors.SetupLookAhead();

			theApp.m_Motors.CollectComplexData(TRUE);

			theApp.m_Motors.StartPolyMoveWithLookAhead(PROFILE->m_ToolPath.m_nCoordL);
			m_nSubThreadAction+=10;
			Sleep(500);
		};

		break;
	case 60:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) { //Do first line
			m_nSubThreadAction+=10;
			Sleep(500);
		}
		break;
	case 70: //Do 2nd line onwards
		if(theApp.m_Motors.IsPolyMotionFinished() == TRUE) {

			theApp.m_Motors.CollectComplexData(FALSE);

//			theApp.m_pScanData->ShrinkAndShiftLine(m_nScanLine-m_nScanStartLine,PORTSIDE);
			theApp.m_pScanData->FillForDPCError(m_nScanLine-PROFILE->m_nScanStartLine);

			switch(PROFILE->m_nLineIncrementMode) {
			case FIXED_INC:
				m_nLineIncrement = PROFILE->m_nLineIncrement;
				break;
			case VARIABLE_INC:
				m_nLineIncrement = PROFILE->m_ScanLine[m_nScanLine].m_nLineIncrement;
				theApp.m_pScanData->FillForLineIncrement(m_nScanLine-PROFILE->m_nScanStartLine,m_nLineIncrement);
				break;
			}
			if(m_nLineIncrement<=0) m_nLineIncrement = 1;
				theApp.m_pScanData->SaveVolumeDuringScan((m_nScanLine-PROFILE->m_nScanStartLine)/m_nLineIncrement);
			if((theApp.m_pScanData->m_FileName.IsEmpty() != TRUE) && (theApp.m_LastSettings.nSaveScanEveryLine!=0)) {
				theApp.m_pScanData->SaveDuringScan();
			};


			m_nDir ^= 1;
			if(theApp.m_LastSettings.nReverseScanLines == 0) {
				if(theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
					m_nScanLine+=theApp.m_pScanData->m_nNumberProbesPerImage;
				} else {
					m_nScanLine+=(m_nLineIncrement * theApp.m_pScanData->m_nNumberProbesPerImage);
				}
				if((m_nScanLine<PROFILE->m_nScanFinishLine) && (m_bAbortFlag==FALSE)) {
					if((theApp.m_nJobNumber==GKN)||(theApp.m_nJobNumber==DOWTY_BSCAN)||(PROFILE->IsRUsed()))
						ContourScanRAxis(m_nScanLine);
					
					m_nSubThreadAction = 20;
				} else {
					//if have filename save?
					theApp.m_bScanning = false;
					theApp.m_Motors.ResetAllMaxSpeeds();
					m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
					theApp.ActivityLog(L"Scan finished naturaly");
				}
			} else {
				if(theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
					m_nScanLine-=theApp.m_pScanData->m_nNumberProbesPerImage;
				} else {
					m_nScanLine-=(m_nLineIncrement * theApp.m_pScanData->m_nNumberProbesPerImage);
				}
				if((m_nScanLine>=PROFILE->m_nScanStartLine) && (m_bAbortFlag==FALSE)) {
					if((theApp.m_nJobNumber==GKN)||(theApp.m_nJobNumber==DOWTY_BSCAN)||(PROFILE->IsRUsed()))
						ContourScanRAxis(m_nScanLine);
					
					m_nSubThreadAction = 20;
				} else {
					//if have filename save?
					theApp.m_Motors.ResetAllMaxSpeeds();
					m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
					theApp.ActivityLog(L"Scan finished naturaly");
				}
			}

			pFrame->SendMessage(INVALIDATE_VIEW);

		} else {
			nLastCoordinate=theApp.m_Motors.DownLoadNextPolyMoveCoordinate(nLastCoordinate,&PROFILE->m_ToolPath, m_nDir);
			nLastCoordinate=theApp.m_Motors.DownLoadNextPolyMoveCoordinate(nLastCoordinate,&PROFILE->m_ToolPath, m_nDir);

		}
		break;
	}

}

void CMainThread::MoveRoundBall()
{
	CCoord	CpSurface,CpHead;
	int	ii,nSide;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	float fR;

	theApp.m_PmacUser.m_nScanSide=m_nNormSide;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_Motors.GetHeadPos(&CpHead);
		fR = CpHead.Side0.fR;

		m_ToolPath.Zero();

		for(ii=0;ii<20;ii++) {
			theApp.m_Motors.GetHeadPos(&CpHead);
			CpSurface = theApp.m_CpBall;
			if(m_nNormSide<2) {
				CpSurface.Side[m_nNormSide].fXt = CpHead.Side[m_nNormSide].fXt + ((m_fXtMove-CpHead.Side[m_nNormSide].fXt) * (float)ii) / 19.0f;
				CpSurface.Side[m_nNormSide].fYt = CpHead.Side[m_nNormSide].fYt + ((m_fYtMove-CpHead.Side[m_nNormSide].fYt) * (float)ii) / 19.0f;
				theApp.m_Kinematics.NormalsFromTips(&CpSurface,m_nNormSide);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,m_nNormSide,TRUE,TRUE);
			} else {
				for(nSide=0;nSide<2;nSide++) {
					CpSurface.Side[nSide].fXt = CpHead.Side[nSide].fXt + ((m_fXtMove-CpHead.Side[nSide].fXt) * (float)ii) / 19.0f;
					CpSurface.Side[nSide].fYt = CpHead.Side[nSide].fYt + ((m_fYtMove-CpHead.Side[nSide].fYt) * (float)ii) / 19.0f;
					theApp.m_Kinematics.NormalsFromTips(&CpSurface,nSide);
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,nSide,TRUE,TRUE);
				}
			}
			CpHead.Side0.fR = fR;

			m_ToolPath.Add(CpHead);
		}
		theApp.m_Motors.SuspendMotors();
		switch(m_nNormSide) {
		case PORTSIDE:	theApp.m_Motors.EnableAxes(LEFT_POLY_MOVE);
			break;
		case STARBOARD:	theApp.m_Motors.EnableAxes(RIGHT_POLY_MOVE);
			break;
		case 2: theApp.m_Motors.EnableAxes(BOTH_POLY_MOVE);
			break;
		}

		theApp.m_Motors.DownLoadPolyMoveCoordinates(&m_ToolPath, 0);
		theApp.m_Motors.SetupLookAhead();
		theApp.m_Motors.StartPolyMoveWithLookAhead(m_ToolPath.m_nCoordL);
	
		m_nSubThreadAction = 999;
		Sleep(500);
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}
}
void CMainThread::ExecuteLadder()
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		ExecuteRectilinearLadder();
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TWIN_TOWER_KINEMATIC:
	case TANK_6AXIS:
	case TANK_5AXIS:
		ExecuteRobotLadder();
		break;
	case TRACKED_PROBE:
		break;
	}
}


void CMainThread::ExecuteRectilinearLadder()
{
	static CCoord CpSurface,CpHead,Cp,CpLadder;
	int nSide,ii;

	switch(m_nSubThreadAction) {
	case 0:
		m_nIndex = 0;
		if(theApp.m_pLadder == NULL)	{
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			break;
		}
		theApp.m_Motors.SuspendMotors();
		m_nSubThreadAction = 10;
		break;

	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetHeadPos(&CpHead);
			if(m_nIndex < theApp.m_pLadder[m_nLadder].m_nCoordL) {
				theApp.m_pLadder[m_nLadder].GetCoord(m_nIndex, &CpLadder);

				for(nSide=0;nSide<2;nSide++) {
					if(((nSide==0)&&(m_nLadderExecuteSide&1)) || ((nSide==1)&&(m_nLadderExecuteSide&2))) {
						switch(	CpLadder.nMoveMode[nSide] ) {
						case LADDER_NO_ACTION:
							break;
						case LADDER_ABSOLUTE:
							for(ii=0;ii<5;ii++) {
								if(CpLadder.Side[nSide].fPos[ii] != 0.0f) CpHead.Side[nSide].fPos[ii] = CpLadder.Side[nSide].fPos[ii];
							}
							break;
						case LADDER_RELATIVE:
							for(ii=0;ii<5;ii++) {
								if(CpLadder.Side[nSide].fPos[ii] != 0.0f) CpHead.Side[nSide].fPos[ii] += CpLadder.Side[nSide].fPos[ii];
							}
							break;
						}
					}
				};

				m_CpHead = CpHead;
	case 2:
				theApp.m_Motors.SuspendMotors();
				theApp.m_Motors.EnableAxes(BOTH_MOVETO);
				theApp.m_Motors.DesirePosEqualsCoord(m_CpHead);
				theApp.m_Motors.MoveTo(DONT_WAIT);
			
				m_nSubThreadAction = 20;
			} else {
				m_nSubThreadAction = 999;
			}
		}
		break;
	case 20:
		if(theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 30;
		}
		break;

	case 30:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			if(theApp.m_Motors.IsItAtDesirePos() != true) {
				m_nSubThreadAction = 2;
				break;
			};
			m_nIndex++;
			m_nSubThreadAction = 10;
		}
		break;

	case 999:
		if(theApp.m_Motors.IsMoveRFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}

		break;
	}


}

void CMainThread::ExecuteRobotLadder()
{
	CPolyCoord Line;
	int nIndex,nSide,ii;
	CCoord Cp,CpSurface,CpCurrent;

	switch(m_nSubThreadAction) {
	case 0:
		m_nIndex = 0;
		if(theApp.m_pLadder == NULL)	{
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			break;
		}
	
		theApp.m_Motors.GetSurfacePos(&CpCurrent);
		CpSurface = CpCurrent;

		for(nIndex=0; nIndex < theApp.m_pLadder[m_nLadder].m_nCoordL; nIndex++) {
			theApp.m_pLadder[m_nLadder].GetCoord(nIndex, &Cp);

			for(nSide=0;nSide<2;nSide++) {
				switch(	Cp.nMoveMode[nSide] ) {
				case LADDER_NO_ACTION:
					break;
				case LADDER_ABSOLUTE:
					if(((nSide==0)&&(m_nLadderExecuteSide&1)) || ((nSide==1)&&(m_nLadderExecuteSide&2))) {
						for(ii=0;ii<3;ii++) {
							if(Cp.Side[nSide].fPos[ii] != 0.0f) CpSurface.Side[nSide].fPos[ii] = Cp.Side[nSide].fPos[ii];
						}
						CpSurface.Side[nSide].norm = Cp.Side[nSide].norm;
					} else {
						CpSurface.Side[nSide] = CpCurrent.Side[nSide];
					}
					break;
				case LADDER_RELATIVE:
					if(((nSide==0)&&(m_nLadderExecuteSide&1)) || ((nSide==1)&&(m_nLadderExecuteSide&2))) {
						for(ii=0;ii<3;ii++) {
							if(Cp.Side[nSide].fPos[ii] != 0.0f) CpSurface.Side[nSide].fPos[ii] = Cp.Side[nSide].fPos[ii] + CpCurrent.Side[nSide].fPos[ii];
						}
						CpSurface.Side[nSide].norm = Cp.Side[nSide].norm;
					} else {
						CpSurface.Side[nSide] = CpCurrent.Side[nSide];
					}
					break;
				}
			}
			Line.Add(CpSurface);
		};

		theApp.m_FBCtrl.DoPolyLine(&Line,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);
		m_nSubThreadAction += 10;
		break;
	case 10:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction = 999;
		}
		break;
	case 999:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
//			m_nThreadAction=0;			//Finished as a stand alone subthread
		}
		break;
	}


}


void CMainThread::RotaryScan()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CCoord Cp0,Cp1,CpHead,CpSurface;
	int nTS;

	switch(m_nSubThreadAction) {
	case 0:
	//	PROFILE->GenerateTurntableWayPoints();
		theApp.m_Motors.CollectComplexData(FALSE);
		m_nLastGainValue = 0;
		m_bAbortFlag = FALSE;
		m_bPause = FALSE;
		theApp.m_Motors.SuspendMotors();
		m_nRevCounter = m_nScanLine = m_nScanStartLine;
		theApp.m_Motors.SetRevCount(m_nRevCounter);

		theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide;

		if(m_bMoveToStartAtBeginningOfScan!=TRUE) {
			m_nSubThreadAction = 100;
			return;
		}
		m_CpMoveTo = PROFILE->m_WayPoints[m_nScanLine].m_pCp[0];
		m_nSubThreadAction = 20;

		break;
	case 10:
		MovetoViaTurntableDangerPlane();	//m_nSubThreadAction automatically incremented by 10 at the ednd of the routine
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nSubThreadAction = 100;
		}
		break;

	case 100:
		m_bMoveTurnTable = true;
		PROFILE->m_ScanLine[m_nScanLine].m_pCp[0].nType = CONTINUOUS_TURNTABLE;
		switch(PROFILE->m_ScanLine[m_nScanLine].m_pCp[0].nType) {
		default:
		case CONTINUOUS_TURNTABLE:
			m_nRevCounter = m_nScanLine;
			theApp.m_Motors.SetRevCount(m_nRevCounter);
			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.StartTurntableCartesianScanningAxes(m_CpMoveTo);
			theApp.m_Motors.StartContinuousRotate(theApp.m_Tank.nRLeft,m_fCurrentSpeed = PROFILE->m_fScanSpeed);
			theApp.m_Motors.CollectComplexData(TRUE);
			m_bScanLineUpdateByMotorRevCounter=true;
			m_nSubThreadAction = 200;
			break;
		case SEGMENT_TURNTABLE:
			m_nRevCounter = m_nScanLine;
			theApp.m_Motors.SetRevCount(m_nRevCounter);

			m_CpSurface = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];

			theApp.m_Kinematics.TipsFromNormals(&m_CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&m_CpSurface,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&m_CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			theApp.m_Motors.EnableAxes(BOTH_MOVETO_WITH_R);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nDir = 0;
			m_bScanLineUpdateByMotorRevCounter=false;
			theApp.m_Motors.CollectComplexData(TRUE);
			m_nSubThreadAction = 300;
			break;
		case JUMP_TURNTABLE:
			theApp.m_Motors.CollectComplexData(FALSE);
			theApp.m_Motors.SuspendMotors();
			for(m_nScanLine;m_nScanLine < PROFILE->m_ScanLine[0].m_nCoordL;m_nScanLine++) {
				if(PROFILE->m_ScanLine[0].m_pCp[m_nScanLine].nType != JUMP_TURNTABLE) {
					theApp.m_Motors.GetSurfacePos(&m_CpSurface);
					m_CpMoveTo = m_CpSurface;
					m_CpMoveTo.Side0.fX = PROFILE->m_CpDanger[0].Side0.fX;
					m_CpMoveTo.Side1.fX = PROFILE->m_CpDanger[0].Side1.fX;
					theApp.m_Kinematics.TipsFromNormals(&m_CpMoveTo,PORTSIDE);
					theApp.m_Kinematics.TipsFromNormals(&m_CpMoveTo,STARBOARD);
					theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
					theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

					theApp.m_Motors.EnableAxes(BOTH_MOVETO);
					theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
					theApp.m_Motors.MoveTo(DONT_WAIT);
					m_nSubThreadAction = 400;
					return;
					break;
				}
			}
			m_nSubThreadAction = 999;
			break;
		}
		break;

	case 200:	//Continuous Turntable scan
		if(m_nRevCounter - theApp.m_Motors.GetRevCount()) {

//			theApp.m_pScanData->SaveVolumeDuringScan(m_nScanLine-PROFILE->m_nScanStartLine);

			pFrame->SendMessage(INVALIDATE_VIEW);
			if(m_fCurrentSpeed - PROFILE->m_fScanSpeed)
				theApp.m_Motors.StartContinuousRotate(theApp.m_Tank.nRLeft,m_fCurrentSpeed = PROFILE->m_fScanSpeed);

		//	m_nScanLine++; //this is updated in m_Data.CollectComplexData();
			m_nRevCounter = theApp.m_Motors.GetRevCount();
			if(m_nScanLine >= PROFILE->m_nScanFinishLine) {
				theApp.m_Motors.CollectComplexData(FALSE);
				theApp.m_Motors.StopContinuousRotate(theApp.m_Tank.nRLeft);
				theApp.m_Motors.SuspendMotors();
				m_nSubThreadAction=999;
				return;
			}
			if(PROFILE->m_ScanLine[m_nScanLine].m_pCp[0].nType == CONTINUOUS_TURNTABLE) {
				m_nSubThreadAction = 200;
			} else {
				m_nSubThreadAction = 100;
				return;
			}
		}
		if(m_bAbortFlag == true) {
			theApp.m_Motors.CollectComplexData(FALSE);
			theApp.m_Motors.StopContinuousRotate(theApp.m_Tank.nRLeft);
			theApp.m_Motors.SuspendMotors();
			m_nSubThreadAction=999;
			return;
		}

		theApp.m_Motors.GetHeadPos(&CpHead);
		PROFILE->m_WayPoints[m_nScanLine].CalculateCoord(CpHead.Side0.fR,CpSurface);

		theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
		theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

		theApp.m_Motors.UpdateTurntableCartesianScanningAxes(m_CpMoveTo);


		break;


	case 300:	//Segment Turntable scan
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_CpSurface = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];
			m_CpMoveTo = m_CpSurface;

			theApp.m_UtUser.TurntableGainChange(m_CpSurface);
			if(m_nDir & 1) {
				m_CpMoveTo.Side0.fR;
			} else {
				m_CpMoveTo.Side0.fR = m_CpSurface.Side1.fR;
			}

			theApp.m_Kinematics.TipsFromNormals(&m_CpMoveTo,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&m_CpMoveTo,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);

			m_nSubThreadAction=310;
		}
		break;
	case 310:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			pFrame->SendMessage(INVALIDATE_VIEW);
			m_nScanLine++;
			if((m_nScanLine >= PROFILE->m_nScanFinishLine) || (m_bAbortFlag == true)) {
				theApp.m_Motors.CollectComplexData(FALSE);
				m_nSubThreadAction=999;
				return;
			}
			m_nDir ^= 1;

			if(PROFILE->m_ScanLine[0].m_pCp[m_nScanLine].nType == SEGMENT_TURNTABLE) {
				m_nSubThreadAction = 300;
			} else {
				m_nSubThreadAction = 100;
			}
		}
		break;

	case 400:	//Jump Turntable
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_CpMoveTo = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];
			m_CpMoveTo.Side0.fX = PROFILE->m_CpDanger[0].Side0.fX;
			m_CpMoveTo.Side1.fX = PROFILE->m_CpDanger[0].Side1.fX;

			theApp.m_Kinematics.TipsFromNormals(&m_CpMoveTo,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&m_CpMoveTo,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);

			m_nSubThreadAction=410;
		}
		break;
	case 410:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_CpMoveTo = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];

			theApp.m_Kinematics.TipsFromNormals(&m_CpMoveTo,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&m_CpMoveTo,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&m_CpMoveTo,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);

			m_nSubThreadAction=420;
		}
		break;
	case 420:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nSubThreadAction=100;
		}
		break;


	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			m_nSafeThreadAction=0;
			m_nSubThreadAction = 0;
			theApp.m_bScanning=FALSE;
			for(nTS=0;nTS<3;nTS++) {
				m_CpSurface.nGain[0]=0;
				m_CpSurface.nTS=nTS;
				theApp.m_UtUser.TurntableGainChange(m_CpSurface);
			}

		}

		break;

	}
}

void CMainThread::MovetoViaDangerPlaneKinematic()
{
	CCoord Cp;
	CCoord *pCpMin = &PROFILE->m_CpDanger[DANGER_MINS];
	CCoord *pCpMax = &PROFILE->m_CpDanger[DANGER_MAXS];
	CPolyCoord Line;

	int	nSide=0;
	int	nNotSide=1;

	switch(m_nDangerPlaneThreadAction) {
	case 0:

		PROFILE->CalculateDangerZone();

		theApp.m_Motors.GetSurfacePos(&Cp);

		switch(theApp.m_PmacUser.m_nScanSide) {
		case 0:
			if(pCpMin->Side[nSide].fI) Cp.Side[nSide].fX = pCpMin->Side[nSide].fX;
			if(pCpMin->Side[nSide].fJ) Cp.Side[nSide].fY = pCpMin->Side[nSide].fY;
			if(pCpMin->Side[nSide].fK) Cp.Side[nSide].fZ = pCpMin->Side[nSide].fZ;
			break;
		case 1:
			if(pCpMin->Side[nNotSide].fI) Cp.Side[nNotSide].fX = pCpMax->Side[nNotSide].fX;
			if(pCpMin->Side[nNotSide].fJ) Cp.Side[nNotSide].fY = pCpMax->Side[nNotSide].fY;
			if(pCpMin->Side[nNotSide].fK) Cp.Side[nNotSide].fZ = pCpMax->Side[nNotSide].fZ;
			break;
		case 2:
			if(pCpMin->Side[nSide].fI) Cp.Side[nSide].fX = pCpMin->Side[nSide].fX;
			if(pCpMin->Side[nSide].fJ) Cp.Side[nSide].fY = pCpMin->Side[nSide].fY;
			if(pCpMin->Side[nSide].fK) Cp.Side[nSide].fZ = pCpMin->Side[nSide].fZ;
			if(pCpMin->Side[nNotSide].fI) Cp.Side[nNotSide].fX = pCpMax->Side[nNotSide].fX;
			if(pCpMin->Side[nNotSide].fJ) Cp.Side[nNotSide].fY = pCpMax->Side[nNotSide].fY;
			if(pCpMin->Side[nNotSide].fK) Cp.Side[nNotSide].fZ = pCpMax->Side[nNotSide].fZ;
			break;
		}
		Line.Add(Cp);

		Cp = m_CpMoveTo;
		switch(theApp.m_PmacUser.m_nScanSide) {
		case 0:
			if(pCpMin->Side[nSide].fI) Cp.Side[nSide].fX = pCpMin->Side[nSide].fX;
			if(pCpMin->Side[nSide].fJ) Cp.Side[nSide].fY = pCpMin->Side[nSide].fY;
			if(pCpMin->Side[nSide].fK) Cp.Side[nSide].fZ = pCpMin->Side[nSide].fZ;
			break;
		case 1:
			if(pCpMin->Side[nNotSide].fI) Cp.Side[nNotSide].fX = pCpMax->Side[nNotSide].fX;
			if(pCpMin->Side[nNotSide].fJ) Cp.Side[nNotSide].fY = pCpMax->Side[nNotSide].fY;
			if(pCpMin->Side[nNotSide].fK) Cp.Side[nNotSide].fZ = pCpMax->Side[nNotSide].fZ;
			break;
		case 2:
			if(pCpMin->Side[nSide].fI) Cp.Side[nSide].fX = pCpMin->Side[nSide].fX;
			if(pCpMin->Side[nSide].fJ) Cp.Side[nSide].fY = pCpMin->Side[nSide].fY;
			if(pCpMin->Side[nSide].fK) Cp.Side[nSide].fZ = pCpMin->Side[nSide].fZ;
			if(pCpMin->Side[nNotSide].fI) Cp.Side[nNotSide].fX = pCpMax->Side[nNotSide].fX;
			if(pCpMin->Side[nNotSide].fJ) Cp.Side[nNotSide].fY = pCpMax->Side[nNotSide].fY;
			if(pCpMin->Side[nNotSide].fK) Cp.Side[nNotSide].fZ = pCpMax->Side[nNotSide].fZ;
			break;
		}
		Line.Add(Cp);
		Line.Add(m_CpMoveTo);
		theApp.m_FBCtrl.DoPolyLine(&Line,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);

		m_nDangerPlaneThreadAction += 10;
		break;
	case 10:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nDangerPlaneThreadAction = 999;
		}
		break;
	case 999:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {
			m_nDangerPlaneThreadAction = 0;
			if(m_nThreadAction == MOVETO_VIA_DANGER_PLANE) {
				m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
			} else {
				m_nSubThreadAction += 10;						//Add 10 because calling routine was a subthread anyway
			}
		}
		break;
	}

}



void CMainThread::MovetoStraightLineKinematic()
{
	CPolyCoord Line;
	static int nCheckedCount;

	switch(m_nSubThreadAction) {
	case 0:
		Line.Add(m_CpMoveTo);
		theApp.m_FBCtrl.DoPolyLine(&Line,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);
		m_nSubThreadAction += 10;
		nCheckedCount = 0;
		break;
	case 10:
		if(theApp.m_FBCtrl.IsMotionStarted() == true || nCheckedCount > 20) {
			m_nSubThreadAction = 999;
			nCheckedCount = 0;
		}
		break;
	case 999:
		if(theApp.m_FBCtrl.IsMotionFinished() == true || nCheckedCount>20) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
//			m_nThreadAction=0;			//Finished as a stand alone subthread
		}
		break;
	}
	nCheckedCount++;

}



void CMainThread::IntelligentGoto()
{
	CCoord Cp;
	CCoord *pCpMin = &PROFILE->m_CpDanger[DANGER_MINS];
	CCoord *pCpMax = &PROFILE->m_CpDanger[DANGER_MAXS];
	CPolyCoord Line;
	static int nCheckedCount;

	switch(m_nDangerPlaneThreadAction) {
	case 0:

		PROFILE->CalculateDangerZone();

		theApp.m_Motors.GetSurfacePos(&Cp);

		if(m_nDangerPlaneMask & 0x03) {

			if(m_nMoveSide & 1 && m_nDangerPlaneMask & 1) {
				if(PROFILE->m_vecToDangerPlane[0].x) Cp.Side[0].fX = pCpMin->Side[0].fX;
				if(PROFILE->m_vecToDangerPlane[0].y) Cp.Side[0].fY = pCpMin->Side[0].fY;
				if(PROFILE->m_vecToDangerPlane[0].z) Cp.Side[0].fZ = pCpMin->Side[0].fZ;
			}
			if(m_nMoveSide & 2 && m_nDangerPlaneMask & 2) {
				switch(PROFILE->m_nComponentDescription) {
				default:
					if(PROFILE->m_vecToDangerPlane[1].x) Cp.Side[1].fX = pCpMax->Side[1].fX;
					if(PROFILE->m_vecToDangerPlane[1].y) Cp.Side[1].fY = pCpMax->Side[1].fY;
					if(PROFILE->m_vecToDangerPlane[1].z) Cp.Side[1].fZ = pCpMax->Side[1].fZ;
					break;
				case CLOSED_ROTARY:
				case OPEN_ROTARY:
				case CIRCULAR:
					Cp.Side[1].fX -= (Cp.Side[1].fI * theApp.m_LastSettings.fDangerMargin[0]);
					Cp.Side[1].fY -= (Cp.Side[1].fJ * theApp.m_LastSettings.fDangerMargin[0]);
					Cp.Side[1].fZ -= (Cp.Side[1].fK * theApp.m_LastSettings.fDangerMargin[0]);
					break;
				}
			}
			Line.Add(Cp);

			if(m_nMoveSide & 1) Cp.Side0 = m_CpSurface.Side0;
			if(m_nMoveSide & 2) Cp.Side1 = m_CpSurface.Side1;
			if(m_nMoveSide & 1 && m_nDangerPlaneMask & 1) {
				if(PROFILE->m_vecToDangerPlane[0].x) Cp.Side[0].fX = pCpMin->Side[0].fX;
				if(PROFILE->m_vecToDangerPlane[0].y) Cp.Side[0].fY = pCpMin->Side[0].fY;
				if(PROFILE->m_vecToDangerPlane[0].z) Cp.Side[0].fZ = pCpMin->Side[0].fZ;
			}
			if(m_nMoveSide & 2 && m_nDangerPlaneMask & 2) {
				switch(PROFILE->m_nComponentDescription) {
				default:
					if(PROFILE->m_vecToDangerPlane[1].x) Cp.Side[1].fX = pCpMax->Side[1].fX;
					if(PROFILE->m_vecToDangerPlane[1].y) Cp.Side[1].fY = pCpMax->Side[1].fY;
					if(PROFILE->m_vecToDangerPlane[1].z) Cp.Side[1].fZ = pCpMax->Side[1].fZ;
					break;
				case CLOSED_ROTARY:
				case OPEN_ROTARY:
				case CIRCULAR:
					Cp.Side[1].fX -= (Cp.Side[1].fI * theApp.m_LastSettings.fDangerMargin[0]);
					Cp.Side[1].fY -= (Cp.Side[1].fJ * theApp.m_LastSettings.fDangerMargin[0]);
					Cp.Side[1].fZ -= (Cp.Side[1].fK * theApp.m_LastSettings.fDangerMargin[0]);
					break;
				}
			}
			Line.Add(Cp);
		}

		if(m_nMoveSide & 1) Cp.Side0 = m_CpSurface.Side0;
		if(m_nMoveSide & 2) Cp.Side1 = m_CpSurface.Side1;
		Line.Add(Cp);

		theApp.m_FBCtrl.DoPolyLine(&Line,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);
		nCheckedCount = 0;

		m_nDangerPlaneThreadAction += 10;
		break;
	case 10:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nDangerPlaneThreadAction = 999;
			nCheckedCount = 0;
		}
		break;
	case 999:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {
			m_nDangerPlaneThreadAction = 0;
			if(m_nThreadAction == INTELLIGENT_GOTO) {
				m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
			} else {
				m_nSubThreadAction += 10;						//Add 10 because calling routine was a subthread anyway
			}
		}
		break;
	}
	nCheckedCount++;


}

bool CMainThread::HasIntelligentGotoFinished()
{
	if(m_nThreadAction==INTELLIGENT_GOTO) return FALSE;
	return TRUE;
}


void CMainThread::BladeToDegrees()
{
	switch(m_nDangerPlaneThreadAction) {
	case 0:
		m_CpDestination = m_CpMoveTo;

		theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
		m_CpMoveTo.Side0.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize * -1.0f;
	
		theApp.m_Motors.EnableAxes(LEFT_MOVETO_WITH_R);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		Sleep(500);

		m_nDangerPlaneThreadAction=10;
		break;
	case 10: //tips to surface 0
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {

			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
			m_CpMoveTo.Side0.fX=(float)theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize * -1.0f;
			m_CpMoveTo.Side0.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize * -1.0f;
			m_CpMoveTo.Side0.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize * -1.0f;
		
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(500);
	
			m_nDangerPlaneThreadAction=20;
		}
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
			m_CpMoveTo.Side0.fR=m_CpDestination.Side0.fR;
		
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(500);
	
			m_nDangerPlaneThreadAction=30;
		}
		break;
	case 30:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nDangerPlaneThreadAction = 999;
		}
		break;
	case 999:
		if(theApp.m_Motors.IsMoveRFinished() == TRUE) {
			if(m_nThreadAction == BLADE_TO_DEGREES) {
				m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
			} else {
				m_nSubThreadAction += 10;						//Add 10 because calling routine was a subthread anyway
			}
		}

		break;
	}
}

void CMainThread::BladeGoHome()
{
	switch(m_nDangerPlaneThreadAction) {
	case 0:
		m_CpDestination = m_CpMoveTo;

		theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
		m_CpMoveTo.Side0.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize * -1.0f;
	
		theApp.m_Motors.EnableAxes(LEFT_MOVETO_WITH_R);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		Sleep(500);

		m_nDangerPlaneThreadAction=10;
		break;
	case 10: //tips to surface 0
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {

			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
			m_CpMoveTo.Side0.fX=(float)theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize * -1.0f;
			m_CpMoveTo.Side0.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize * -1.0f;
			m_CpMoveTo.Side0.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize * -1.0f;
		
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(500);
	
			m_nDangerPlaneThreadAction=20;
		}
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
			m_CpMoveTo.Side0.fY=(float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
		
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(500);
	
			m_nDangerPlaneThreadAction=30;
		}
		break;
	case 30:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nDangerPlaneThreadAction = 999;
		}
		break;
	case 999:
		if(theApp.m_Motors.IsMoveRFinished() == TRUE) {
			if(m_nThreadAction == BLADE_TO_DEGREES) {
				m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
			} else {
				m_nSubThreadAction += 10;						//Add 10 because calling routine was a subthread anyway
			}
		}

		break;
	}

}

void CMainThread::RobotContourScan()
{
	int	ii;
	CPolyCoord Line;
	CCoord CpSurface;
	int nPtr;
	int nMax = PROFILE->m_nWayPointNumber;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	switch(m_nSubThreadAction) {
	case 0:
		switch(PROFILE->m_nScanSide) {
		case 0:	m_nMoveSide = 1;
			break;
		case 1:	m_nMoveSide = 2;
			break;
		case 2:	m_nMoveSide = 3;
			break;
		}
		m_nDangerPlaneMask = theApp.m_LastSettings.nDangerPlaneMoveMask;
		m_nDangerPlaneThreadAction = 0;
		m_bAbortFlag = FALSE;
		m_bPause = FALSE;
		theApp.m_bScanning=TRUE;
		m_bScanLineUpdateByMotorRevCounter=false;
		PROFILE->CalculateDangerZone();
		theApp.m_pScanData->GenerateSamplePositionVertices();

		m_nScanLine=m_nScanStartLine;

		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.GetHeadPos(&m_CpMoveTo);

		theApp.m_FBCtrl.m_nScanLine=-1;
		m_nDir=0;
		m_nSubThreadAction = 10;

		break;
	case 10:
		m_CpSurface = PROFILE->m_ScanLine[m_nScanStartLine].m_pCp[0];
		PROFILE->AvoidExclusionZones(&m_CpSurface);
		if(theApp.m_LastSettings.nScanMask & _SCAN_START_VIA_DANGER_PLANE) {
			IntelligentGoto();
		} else {
			m_nSubThreadAction+=10;
		}

		break;
	case 20: //move to start
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {
			if(PROFILE->m_nWayPointNumber<2) PROFILE->m_nWayPointNumber = 40; //default to 40
			if(PROFILE->m_nWayPointNumber>200) PROFILE->m_nWayPointNumber = 200;

			Line.Zero();
			switch(m_nDir) {
			case 0:
				for(ii=0;ii<PROFILE->m_nWayPointNumber;ii++) {
					nPtr = MulDiv(ii,PROFILE->m_ScanLine[m_nScanLine].GetSize()-1,PROFILE->m_nWayPointNumber-1);
					PROFILE->m_ScanLine[m_nScanLine].GetCoord(nPtr,&CpSurface);
					if((theApp.m_LastSettings.bUseBrainCompensation == TRUE) && (PROFILE->m_nBrainLineL>0)){
						PROFILE->CalculateBrainCoord(PROFILE->m_nFastAxis,PROFILE->m_nSlowAxis,CpSurface);
					}
					Line.Add(CpSurface);
				}
				break;
			case 1:
				for(ii=PROFILE->m_nWayPointNumber-1;ii>=0;ii--) {
					nPtr = MulDiv(ii,PROFILE->m_ScanLine[m_nScanLine].GetSize()-1,PROFILE->m_nWayPointNumber-1);
					PROFILE->m_ScanLine[m_nScanLine].GetCoord(nPtr,&CpSurface);
					if((theApp.m_LastSettings.bUseBrainCompensation == TRUE) && (PROFILE->m_nBrainLineL>0)){
						PROFILE->CalculateBrainCoord(PROFILE->m_nFastAxis,PROFILE->m_nSlowAxis,CpSurface);
					}
					Line.Add(CpSurface);
				}
				break;
			}
			theApp.m_Motors.CollectComplexData(TRUE);
			theApp.m_FBCtrl.DoPolyLine(&Line,PROFILE->m_fScanSpeed,10.0f);
			m_nSubThreadAction=30;
		}
		break;
	case 30:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction = 40;
		};
		break;

	case 40:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {

			theApp.m_Motors.CollectComplexData(FALSE);
			theApp.m_pScanData->FillForDPCError(m_nScanLine-PROFILE->m_nScanStartLine);
			theApp.m_pScanData->DeGlitch(m_nScanLine - PROFILE->m_nScanStartLine);

			if((theApp.m_pScanData->m_FileName.IsEmpty() != TRUE) && (theApp.m_LastSettings.nSaveScanEveryLine!=0)) {
				theApp.m_pScanData->SaveDuringScan();
			};
			if(m_nLineIncrement<=0) m_nLineIncrement = 1;
			theApp.m_pScanData->SaveVolumeDuringScan((m_nScanLine-PROFILE->m_nScanStartLine)/m_nLineIncrement);

			m_nDir ^= 1;
			if(theApp.m_LastSettings.nReverseScanLines == 0) {
				if(theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
					m_nLineIncrement=1;
					m_nScanLine+=(m_nLineIncrement * theApp.m_pScanData->m_nNumberProbesPerImage);
				} else {
					m_nScanLine+=theApp.m_pScanData->m_nNumberProbesPerImage;
				}
				if((m_nScanLine<(PROFILE->m_nScanFinishLine-m_nLineIncrement)) && (m_bAbortFlag==FALSE)) {

					m_nSubThreadAction = 20;
				} else {
					m_nSubThreadAction = 0;
					m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
					m_nThreadAction = 0;
					theApp.ActivityLog(L"Scan finished naturaly");
				}
			} else {
				if(theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
					m_nLineIncrement=1;
					m_nScanLine-=(m_nLineIncrement * theApp.m_pScanData->m_nNumberProbesPerImage);
				} else {
					m_nScanLine-=theApp.m_pScanData->m_nNumberProbesPerImage;
				}
				if((m_nScanLine>=(PROFILE->m_nScanStartLine)) && (m_bAbortFlag==FALSE)) {

					m_nSubThreadAction = 20;
				} else {
					m_nSubThreadAction = 0;
					m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
					m_nThreadAction = 0;
					theApp.ActivityLog(L"Scan finished naturaly");
				}
			}
			pFrame->SendMessage(INVALIDATE_VIEW);

		}
		break;
	}

}

void CMainThread::MultiProbeScan()
{
	static int nLastCoordinate;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	static CCoord	CpHead,CpSurface,Cp, CpDanger;
	CCoord StartOfLine, FinishOfLine;
	static float fOldScanSpeed;
	int nLineSeparation,nProbe,nPtr,nLinesRemaining,nLine;



	switch(m_nSubThreadAction) {
	case 0: //move out of danger zone
		m_bAbortFlag = FALSE;
		m_bPause = FALSE;
		theApp.m_bScanning=TRUE;
		fOldScanSpeed = 0.0f;
		m_bScanLineUpdateByMotorRevCounter=false;
		PROFILE->CalculateDangerZone();
		theApp.m_pScanData->GenerateSamplePositionVertices();
		theApp.m_PmacUser.RequestAllAccelerationsAndVelocities();
		theApp.m_pScanData->AllocateVolumeBuffersAndOffsets();

		m_nScanLine=m_nScanStartLine;

		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
		m_bMoveToStartAtBeginningOfScan=true;
		if(m_bMoveToStartAtBeginningOfScan!=TRUE) {
			m_nSubThreadAction = 20;
			return;
		}
		m_nDir=0;
		m_nSubThreadAction = 10;

		break;
	case 10:
		CpSurface = PROFILE->m_ScanLine[m_nScanStartLine].m_pCp[0];
		CpSurface.Side0.fY -= ((float)PROFILE->m_nFirstProbe * theApp.m_LastSettings.fProbeSeparation);
		m_CpMoveTo = CpSurface;
		PROFILE->AvoidExclusionZones(&m_CpMoveTo);
		if(theApp.m_LastSettings.nScanMask & _SCAN_START_VIA_DANGER_PLANE) {
			MovetoViaDangerPlane();
		} else {
			MovetoStartWithoutDangerPlane();
		}

		//m_nSubThreadAction is incremented by ten at the end of the MovetoViaDangerplane
		break;
	case 20: //move to start
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetHeadPos(&Cp);

			PROFILE->CalculateToolPath(m_nScanLine,0,m_nDir);
			InvalidateToolPath();

			if(m_nDir==0) {
				PROFILE->m_ToolPath.GetCoord(0,&CpHead);
			} else {
				PROFILE->m_ToolPath.GetLastCoord(&CpHead);
			}

			switch(theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide) {
			case 0:
				CpHead.Side1.fX=Cp.Side1.fX;
				CpHead.Side1.fY=Cp.Side1.fY;
				CpHead.Side1.fZ=Cp.Side1.fZ;
				CpHead.Side1.fXt=Cp.Side1.fXt;
				CpHead.Side1.fYt=Cp.Side1.fYt;
				break;
			case 1:
				CpHead.Side0.fX=Cp.Side0.fX;
				CpHead.Side0.fY=Cp.Side0.fY;
				CpHead.Side0.fZ=Cp.Side0.fZ;
				CpHead.Side0.fXt=Cp.Side0.fXt;
				CpHead.Side0.fYt=Cp.Side0.fYt;
				break;
			case 2:
				break;
			}


			if((theApp.m_nJobNumber==GKN)||(theApp.m_nJobNumber==DOWTY_BSCAN)||(PROFILE->IsRUsed()))
				ContourScanRAxis(m_nScanLine);

			theApp.m_Motors.SuspendMotors();
			theApp.m_Thread.m_CpMoveTo = CpHead;
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nSubThreadAction=30;
		}
		break;
	case 30:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) { //Do first line
			theApp.m_Motors.SuspendMotors();

			switch(theApp.m_PmacUser.m_nScanSide) {
			case 0:	theApp.m_Motors.EnableAxes(LEFT_POLY_MOVE);
				break;
			case 1:	theApp.m_Motors.EnableAxes(RIGHT_POLY_MOVE);
				break;
			case 2:	theApp.m_Motors.EnableAxes(BOTH_POLY_MOVE);
				break;
			}
			theApp.m_Motors.SetScanSpeed(PROFILE->m_fScanSpeed * theApp.m_LastSettings.fFeedRate / 100.0f);
			nLastCoordinate=theApp.m_Motors.DownLoadPolyMoveCoordinates(&PROFILE->m_ToolPath, m_nDir);
			theApp.m_Motors.SetupLookAhead();

			theApp.m_Motors.CollectComplexData(TRUE);

			theApp.m_Motors.StartPolyMoveWithLookAhead(PROFILE->m_ToolPath.m_nCoordL);
			m_nSubThreadAction=40;
		};

		break;
	case 40: //Do 2nd line onwards
		if(theApp.m_Motors.IsPolyMotionFinished() == TRUE) {

			nLineSeparation = (int)(theApp.m_LastSettings.fProbeSeparation / PROFILE->m_fSlowIncrement);

			theApp.m_Motors.CollectComplexData(FALSE);

			for(nProbe=0;nProbe<theApp.m_pScanData->m_nNumberProbesPerImage;nProbe++) {
				nPtr = m_nScanLine + nProbe * nLineSeparation;
				if(nPtr<theApp.m_pScanData->m_nNumberLines) {
					theApp.m_pScanData->FillForDPCError(nPtr);
					theApp.m_pScanData->m_nLineScannedStatus[nPtr] = 1;
				}
			}

			m_nLineIncrement = PROFILE->m_nLineIncrement;
			if(m_nLineIncrement<=0) m_nLineIncrement = 1;

			theApp.m_pScanData->SaveVolumeDuringScan((m_nScanLine-PROFILE->m_nScanStartLine)/m_nLineIncrement);

			m_nDir ^= 1;
			if(theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30] != 2) {
				//Normal multiplexer

				m_nScanLine+=m_nLineIncrement;

				if(theApp.m_pScanData->m_nLineScannedStatus[m_nScanLine] == 1) {

					//Find next empty scan line
					for(nLine=0;nLine<theApp.m_pScanData->m_nNumberLines;nLine++) {
						if(theApp.m_pScanData->m_nLineScannedStatus[nLine] == 0) {
							m_nScanLine = nLine;

							if((m_nScanLine + ((theApp.m_pScanData->m_nNumberProbesPerImage-1) * nLineSeparation)) >= theApp.m_pScanData->m_nNumberLines) {

								nLinesRemaining = theApp.m_pScanData->m_nNumberLines - m_nScanLine;
								nProbe = (nLinesRemaining / nLineSeparation);

								m_nScanLine = nLine - (((theApp.m_pScanData->m_nNumberProbesPerImage-1) - nProbe) * nLineSeparation);
							}
							break;
						}
					}

				}
			} else {
				//Linear scanning array
				m_nScanLine+=theApp.m_pScanData->m_nNumberProbesPerImage;
			}

			if((theApp.m_pScanData->HaveAllLinesBeenScanned() == false) && (m_bAbortFlag==FALSE) && (m_nScanLine<(PROFILE->m_nScanFinishLine-m_nLineIncrement))) {
				m_nSubThreadAction = 20;
			} else {
				theApp.m_PmacUser.ResetAllMaxSpeeds();
				m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
				theApp.ActivityLog(L"Scan finished naturaly");
			}

			pFrame->SendMessage(INVALIDATE_VIEW);

		} else {
			nLastCoordinate=theApp.m_Motors.DownLoadNextPolyMoveCoordinate(nLastCoordinate,&PROFILE->m_ToolPath, m_nDir);
			nLastCoordinate=theApp.m_Motors.DownLoadNextPolyMoveCoordinate(nLastCoordinate,&PROFILE->m_ToolPath, m_nDir);

		}
		break;
	}

}

void CMainThread::ThicknessCalibrationTwoPts()
{
	CCoord CpSurface,CpHead;
	int	nTimeNs,nT1,nT0;
	float fThickness0,fThickness1;
	float fConstant,fVelocity;
	CMainFrame*  pFrame;
	

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_ThickCal.GetCoord(theApp.m_LastSettings.nThicknessTestPt0,&CpSurface);
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);

		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		Sleep(100);
		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			Sleep(300);
			nTimeNs = theApp.m_UtUser.GetGateMaterialTimeNs(0,1);
			theApp.m_ThickCal.m_pCp[theApp.m_LastSettings.nThicknessTestPt0].nTimens = nTimeNs;
			m_nSubThreadAction = 20;
		};

	case 20: //tips to surface 0
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_ThickCal.GetCoord(theApp.m_LastSettings.nThicknessTestPt1,&CpSurface);
			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);

			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(100);
			m_nSubThreadAction=30;

		}
		break;
	case 30:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			Sleep(300);

			nT0 = theApp.m_ThickCal.m_pCp[theApp.m_LastSettings.nThicknessTestPt0].nTimens;
			fThickness0 = theApp.m_ThickCal.m_pCp[theApp.m_LastSettings.nThicknessTestPt0].fNomThickness;

			nTimeNs = theApp.m_UtUser.GetGateMaterialTimeNs(0,1);
			nT1 = theApp.m_ThickCal.m_pCp[theApp.m_LastSettings.nThicknessTestPt1].nTimens = nTimeNs;
			fThickness1 = theApp.m_ThickCal.m_pCp[theApp.m_LastSettings.nThicknessTestPt1].fNomThickness;

			fConstant = (-fThickness0 * (float)(nT1 - nT0) / (fThickness1 - fThickness0) ) + (float)nT0;
			theApp.m_LastSettings.nMaterialConstant = (int)(fConstant * -1.0f);

			fVelocity = (fThickness1 - fThickness0) / (float)(nT1 - nT0) * 2e6f;
			theApp.m_LastSettings.nMaterialVelocity = (int)fVelocity;

			pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
			pFrame->m_nOldMeasurementUnits = -1;

			m_nSubThreadAction=999;
		}
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nSubThreadAction = 0;
			m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
			theApp.ActivityLog(L"2 pt calibration finished");
		}
		break;
	}

}

void CMainThread::CheckThicknessGauge()
{
	CCoord CpSurface,Cp;
	int	nPath;
	CMainFrame*  pFrame;
	CTime CurrentTime;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		m_nIndex = 0;
		m_nSubThreadAction=10;
		break;
	case 10:
		theApp.m_ThickCal.GetCoord(m_nIndex,&CpSurface);

		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&m_CpMoveTo,STARBOARD,TRUE,TRUE);

		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		Sleep(300);
		m_nSubThreadAction = 20;
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {

			Sleep(500);

			nPath = theApp.m_UtUser.GetGateMaterialTimeNs(0,1);
			switch(theApp.m_LastSettings.nMaterialVelocityMode) {
			case 0: theApp.m_ThickCal.m_pCp[m_nIndex].fActThickness = theApp.TimeToThickness(nPath,NULL) / 2.0f;
				break;
			case 1: theApp.m_ThickCal.m_pCp[m_nIndex].nTimens = nPath;
				theApp.ComputeVelocityTable();
				theApp.m_ThickCal.m_pCp[m_nIndex].fActThickness = theApp.TimeToThickness(nPath, NULL) / 2.0f;
				break;
			}

			m_nIndex++;
			if(m_nIndex < theApp.m_ThickCal.GetSize() ) {
				m_nSubThreadAction = 10;
			} else {
				CurrentTime = CTime::GetCurrentTime();
				theApp.m_LastSettings.osThicknessCalibrationDate=(int)CurrentTime.GetTime();


				theApp.m_LastSettings.bThicknessCalibrationStatus=true;
				for(m_nIndex=0;m_nIndex<theApp.m_ThickCal.GetSize();m_nIndex++) {
					theApp.m_ThickCal.GetCoord(m_nIndex,&Cp);
					if((Cp.fActThickness<Cp.fMinThickness) || (Cp.fActThickness>Cp.fMaxThickness)) {
						theApp.m_LastSettings.bThicknessCalibrationStatus = false;
					}
				}

				pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
				pFrame->m_nOldMeasurementUnits = -1;

				m_nSubThreadAction = 0;
				m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
				theApp.ActivityLog(L"Thicknes gauge check finished");
			};

		};
		break;
	}


}




void CMainThread::TipsRemoveAirRobot()
{
	CPolyCoord Line;
	CCoord Cp;
	D3DXVECTOR3 vPos,vNorm;
	D3DXQUATERNION quatPos;
	D3DXMATRIXA16 matWorld,matRot;
	int nSide = PORTSIDE;
	CTime CurrentTime = CTime::GetCurrentTime();
	static int osStartTime;
	float fPresentYAngle;


	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_Motors.GetSurfacePos(&Cp);
		m_CpSurface = Cp;

		switch (theApp.m_Tank.nScannerDescription) {
		case SINGLE_ROBOT:
		case DUAL_ROBOT:
		case DUAL_ROBOT_9_PLUS_9:
		case SPX_ROBOT:
			for (nSide = 0; nSide < 2; nSide++) {

				fPresentYAngle = -atan2f(m_CpSurface.Side[nSide].fI, m_CpSurface.Side[nSide].fK) * RAD_TO_DEG;

				vNorm = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

				theApp.m_Ethercat.InvalidateRobotJoints();
				theApp.m_Robot[nSide].GetToolTipPos(&vPos, &quatPos);
				D3DXMatrixRotationQuaternion(&matWorld, &quatPos);

				D3DXMatrixRotationY(&matRot, -(theApp.m_LastSettings.fRemoveAirAngle[nSide] + (90.0f - fPresentYAngle)) * DEG_TO_RAD);
				D3DXVec3TransformCoord(&vNorm, &vNorm, &matWorld);
				D3DXVec3TransformCoord(&vNorm, &vNorm, &matRot);

				CopyMemory(Cp.Side[nSide].norm, vNorm, sizeof D3DXVECTOR3);
			};
			theApp.InvertNormScannerSpecific(&Cp, PORTSIDE);
	//		theApp.InvertNormScannerSpecific(&Cp, STARBOARD);

			break;
		case TWIN_TOWER_KINEMATIC:
			fPresentYAngle = -atan2f(m_CpSurface.Side[nSide].fI, m_CpSurface.Side[nSide].fK) * RAD_TO_DEG;

			Cp = m_CpSurface;
			Cp.Side0.norm = D3DXVECTOR4(0.0f, -0.707f, -0.707f, 0.0f);
			Cp.Side1.norm = D3DXVECTOR4(0.0f, 0.707f, -0.707f, 0.0f);
			break;
		};


		Line.Zero();
		Line.Add(Cp);
		theApp.m_FBCtrl.DoPolyLine(&Line,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);
		m_nSubThreadAction += 10;
		break;
	case 10:
		if(theApp.m_FBCtrl.IsMotionStarted() == TRUE) {
			m_nSubThreadAction+=10;
		}
		break;
	case 20:
		if(theApp.m_FBCtrl.IsMotionFinished() == TRUE) {
			osStartTime=(int)CurrentTime.GetTime();
			m_nSubThreadAction+=10;
		}
		break;
	case 30:
		if((CurrentTime.GetTime()-osStartTime) > theApp.m_LastSettings.nRemoveAirTime) {
			Line.Zero();
			Line.Add(m_CpSurface);
			theApp.m_FBCtrl.DoPolyLine(&Line,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);
			m_nSubThreadAction += 10;
		}
		break;
	case 40:
		if(theApp.m_FBCtrl.IsMotionStarted() == TRUE) {
			m_nSubThreadAction=999;
		}
		break;

	case 999:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
//			m_nThreadAction=0;			//Finished as a stand alone subthread
		}
		break;
	}

}

void CMainThread::DoMarkerPen()
{
	static CCoord CpSurface,CpHead,Cp,CpDanger,CpDestination;
	CString Buff;

	switch(m_nSubThreadAction) {
	case 0: 
		theApp.ActivityLog(L"Marker pen Started");
		theApp.m_Motors.GetHeadPos(&CpHead);

		m_CpMoveTo = CpHead;
		m_CpMoveTo.Side0.pt += theApp.m_Tank.vecMarkerPen[0];

		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		Sleep(100);

		m_nSubThreadAction=10;
		break;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.AirStart(theApp.m_Tank.nRightAirBit, 1);
			Sleep(theApp.m_Tank.nMarkerPenOnTime[0]);
			theApp.m_Motors.AirStop(theApp.m_Tank.nRightAirBit, 1);
			
			m_CpMoveTo = CpHead;

			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(100);

			m_nSubThreadAction=999;
		}
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nSubThreadAction = 0;
			m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
			theApp.ActivityLog(L"Marker pen finished");
		}
		break;
	}

}



bool CMainThread::IsJoystickRunning()
{
	if(m_nThreadAction == JOYSTICK_START) return true;
	return false;
}

void CMainThread::BladeScan()
{
	static int nLastCoordinate;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	static CCoord	CpHead,CpSurface,Cp, CpDanger;
	CPolyCoord StartOfLine;


	switch(m_nSubThreadAction) {
	case 0:

		switch(m_nBladeSide) {
		case OUTSIDE:	PROFILE->m_vecBladeOffset[OUTSIDE] = D3DXVECTOR3(0.0f, theApp.m_LastSettings.fBladeYOffset[OUTSIDE], 0.0f);
			break;
		case INSIDE:	PROFILE->m_vecBladeOffset[INSIDE] = D3DXVECTOR3(0.0f, theApp.m_LastSettings.fBladeYOffset[INSIDE], 0.0f);
			break;
		}

		m_nSubThreadAction = 10;
		break;

	case 10:

		for(m_nScanLine=m_nScanStartLine;m_nScanLine<=PROFILE->m_nScanFinishLine;m_nScanLine++) {
			if(PROFILE->m_LineBlade[m_nBladeSide][m_nScanLine].m_nUseToScan == 1) {
				goto start_line_found;
			}
		}
		m_nSubThreadAction = 999;
		return;

start_line_found:		
		
		//move out of danger zone
		m_bAbortFlag = FALSE;
		m_bPause = FALSE;
		theApp.m_bScanning=TRUE;
		theApp.m_Motors.SuspendMotors();
		PROFILE->CalculateDangerZone();

		PROFILE->CalculateToolPath( m_nScanLine);

		if(PROFILE->m_ToolPath.m_pCp[0].Side0.fX < PROFILE->m_ToolPath.m_pCp[PROFILE->m_ToolPath.m_nCoordL-1].Side0.fX) {
			m_nDir = 0;
			PROFILE->m_LineBlade[m_nBladeSide][m_nScanLine].GetFirstCoord(&CpSurface);
		} else {
			m_nDir = 1;
			PROFILE->m_LineBlade[m_nBladeSide][m_nScanLine].GetLastCoord(&CpSurface);
		}
		m_CpStart = CpSurface;
		m_nSubThreadAction=20;
		break;
	case 20:
		MoveToBladeStartPos();	//m_nSubThreadAction is incremented by ten at the end of the MovetoViaDangerplane
		break;
	case 30:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) { //Do first line
			theApp.m_Motors.SuspendMotors();

			switch(theApp.m_PmacUser.m_nScanSide) {
			case 0:	theApp.m_Motors.EnableAxes(LEFT_POLY_MOVE);
				break;
			case 1:	theApp.m_Motors.EnableAxes(RIGHT_POLY_MOVE);
				break;
			case 2:	theApp.m_Motors.EnableAxes(BOTH_POLY_MOVE);
				break;
			}
			theApp.m_Motors.SetScanSpeed(PROFILE->m_fScanSpeed * theApp.m_LastSettings.fFeedRate / 100.0f);
			nLastCoordinate=theApp.m_Motors.DownLoadPolyMoveCoordinates(&PROFILE->m_ToolPath, m_nDir);
			theApp.m_Motors.SetupLookAhead();

			pFrame->m_pScanViewBladeThicknessDlg->m_Data.ClearLinePriorToScan(m_nBladeSide,m_nScanLine);
			theApp.m_Motors.CollectBladeData(TRUE);

			theApp.m_Motors.StartPolyMoveWithLookAhead(PROFILE->m_ToolPath.m_nCoordL);
			m_nSubThreadAction=40;
		};

		break;
	case 40:
		if(theApp.m_Motors.IsPolyMotionFinished() == TRUE) {
			theApp.m_Motors.CollectBladeData(FALSE);

			theApp.m_nBladeLine = m_nScanLine;
			pFrame->m_pScanViewBladeThicknessDlg->m_nScanType = m_nBladeSide;
			pFrame->m_pScanViewBladeThicknessDlg->UpdateAllControls();
			pFrame->m_pScanViewBladeThicknessDlg->SetProfileMinMaxThickness();
			pFrame->m_pScanViewBladeThicknessDlg->Invalidate(FALSE);

			m_nDir ^= 1;
			m_nScanLine++;
			for(m_nScanLine;m_nScanLine<=PROFILE->m_nScanFinishLine;m_nScanLine++) {
				if((PROFILE->m_LineBlade[m_nBladeSide][m_nScanLine].m_nUseToScan & 1) && (m_bAbortFlag==FALSE)) {
					PROFILE->CalculateToolPath(m_nScanLine);


					if(m_nDir == 0) {
						PROFILE->m_LineBlade[m_nBladeSide][m_nScanLine].GetFirstCoord(&CpSurface);
					} else {
						PROFILE->m_LineBlade[m_nBladeSide][m_nScanLine].GetLastCoord(&CpSurface);
					}

					theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
					theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);

					m_CpMoveTo = CpHead;

					theApp.m_Motors.SuspendMotors();
					theApp.m_Motors.EnableAxes(BOTH_MOVETO);
					theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
					theApp.m_Motors.MoveTo(DONT_WAIT);
					m_nSubThreadAction = 50;
					return;
				}
			}
			m_nSubThreadAction = 999;
			m_nThreadAction = WAIT_FOR_MOVE_FINISHED;

		} else {
			nLastCoordinate=theApp.m_Motors.DownLoadNextPolyMoveCoordinate(nLastCoordinate,&PROFILE->m_ToolPath, m_nDir);
		}
		break;
	case 50:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) { 
			theApp.m_Motors.SuspendMotors();
			switch(theApp.m_PmacUser.m_nScanSide) {
			case 0:	theApp.m_Motors.EnableAxes(LEFT_POLY_MOVE);
				break;
			case 1:	theApp.m_Motors.EnableAxes(RIGHT_POLY_MOVE);
				break;
			case 2:	theApp.m_Motors.EnableAxes(BOTH_POLY_MOVE);
				break;
			}
			theApp.m_Motors.SetScanSpeed(PROFILE->m_fScanSpeed * theApp.m_LastSettings.fFeedRate / 100.0f);
			nLastCoordinate=theApp.m_Motors.DownLoadPolyMoveCoordinates(&PROFILE->m_ToolPath, m_nDir);
			theApp.m_Motors.SetupLookAhead();

			pFrame->m_pScanViewBladeThicknessDlg->m_Data.ClearLinePriorToScan(m_nBladeSide,m_nScanLine);
			theApp.m_Motors.CollectBladeData(TRUE);

			theApp.m_Motors.StartPolyMoveWithLookAhead(PROFILE->m_ToolPath.m_nCoordL);
			m_nSubThreadAction = 40;
		}
	
		break;	
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}
}


void CMainThread::MoveToBladeStartPos()
{
	static int nPtr=0;
	int nSide=0;
	CCoord CpSurface,CpHead;
	static CCoord CpDestination;

	switch(m_nDangerPlaneThreadAction) {
	case 0:
		theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
		m_CpMoveTo.Side0.fZ = (float)theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize * -1.0f;
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
		theApp.m_Motors.MoveTo(DONT_WAIT);
		m_nDangerPlaneThreadAction = 10;
		break;
	case 10:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_CpMoveTo.Side0.fX = (float)theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize * -1.0f;
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nDangerPlaneThreadAction = 20;
		}
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_CpMoveTo.Side0.fR = m_CpStart.Side0.fR;
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nDangerPlaneThreadAction = 30;
		}
		break;
	case 30:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);

			theApp.m_Kinematics.TipsFromNormals(&m_CpStart,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&m_CpStart,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&m_CpStart,&CpHead,PORTSIDE,TRUE,TRUE);

			m_CpMoveTo.Side0.fY = CpHead.Side0.fY;
			m_CpMoveTo.Side0.fXt = CpHead.Side0.fXt;
			m_CpMoveTo.Side0.fYt = CpHead.Side0.fYt;

			CpDestination = CpHead;

			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nDangerPlaneThreadAction=40;
		}
		break;
	case 40:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_CpMoveTo.Side0.fX = CpDestination.Side0.fX;
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nDangerPlaneThreadAction = 50;
		}
		break;
	case 50:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_CpMoveTo = CpDestination;
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nDangerPlaneThreadAction = 999;
		}
		break;
	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			if(m_nThreadAction == MOVETO_BLADE_START_POS) {
				m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
			} else {
				m_nSubThreadAction += 10;						//Add 10 because calling routine was a subthread anyway
			}
		}
		break;
	}

}

void CMainThread::RobotRollerProbeScan()
{
	CPolyCoord Line;
	CCoord CpSurface;
	int nMax = 70;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	switch(m_nSubThreadAction) {
	case 0: //move out of danger zone
		m_bAbortFlag = FALSE;
		m_bPause = FALSE;
		theApp.m_bScanning=TRUE;
		m_bScanLineUpdateByMotorRevCounter=false;
		theApp.m_pScanData->GenerateSamplePositionVertices();

		m_nScanLine=m_nScanStartLine;

		theApp.m_FBCtrl.KillAllMotionPrograms();

		theApp.m_FBCtrl.m_nScanLine=-1;
		m_nDir=0;
		m_nSubThreadAction = 10;

		break;
	case 10:
		CpSurface = PROFILE->m_ScanLine[m_nScanStartLine].m_pCp[0];
		m_CpMoveTo = CpSurface;
//		MoveRobotWithBackOff();
		MoveRobotToStartOfLine(m_nScanStartLine);

		break;

	case 20: //move to start
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {

			PROFILE->GenerateRaftWayPoints(&Line, m_nScanLine,theApp.m_pScanData->m_nNumberProbesPerImage);
			if(m_nDir) {
				Line.ReverseAllCoords();
			}

			theApp.m_Motors.CollectComplexData(TRUE);
			theApp.m_FBCtrl.DoPolyLine(&Line,PROFILE->m_fScanSpeed,PROFILE->m_fScanSpeed);
//			Sleep(1200);
			m_nSubThreadAction=30;
		}
		break;
	case 30:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction = 40;
		};
		break;

	case 40:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {

			theApp.m_Motors.CollectComplexData(FALSE);
//			theApp.m_pScanData->FillForDPCError(m_nScanLine-PROFILE->m_nScanStartLine);
			theApp.m_pScanData->SaveVolumeDuringScan(m_nScanLine-PROFILE->m_nScanStartLine);

			switch(PROFILE->m_nBiUniDirectionalScan) {
			default:
				m_nDir ^= 1;
				break;
			case 1:
				break;
			};
			m_nScanLine+=theApp.m_pScanData->m_nNumberProbesPerImage;
			if((m_nScanLine<PROFILE->m_nScanFinishLine) && (m_bAbortFlag==FALSE)) {
				if(theApp.m_LastSettings.nScanMask & _ROLLERPROBE_BACKOFF) {
					switch(m_nDir) {
					case 0:	PROFILE->m_ScanLine[m_nScanLine].GetFirstCoord(&m_CpMoveTo);
						break;
					case 1:	PROFILE->m_ScanLine[m_nScanLine].GetLastCoord(&m_CpMoveTo);
						break;
					}
					switch(PROFILE->m_nBiUniDirectionalScan) {
					default:	MoveRobotWithBackOff();
						break;
					case 1:	MoveRobotToStartOfLine(m_nScanLine);
						break;
					};
					m_nSubThreadAction = 10;
				} else {
					m_nSubThreadAction = 20;
				}
			} else {
				m_nSubThreadAction = 0;
				m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
				m_nThreadAction = 0;
				theApp.ActivityLog(L"Scan finished naturaly");
			}
			pFrame->SendMessage(INVALIDATE_VIEW);

		}
		break;
	}

}

void CMainThread::MoveRobotToStartOfLine(int nScanLine)
{
	CCoord CpSurface,Cp;
	CPolyCoord Line;

	int	nSide=0;
	int	nNotSide=1;

	switch(m_nDangerPlaneThreadAction) {
	case 0:
		/*
		theApp.m_Motors.GetSurfacePos(&CpSurface);
		CpSurface.Side0.fX += (theApp.m_LastSettings.fDangerMargin[0] * CpSurface.Side0.fI);
		CpSurface.Side0.fY += (theApp.m_LastSettings.fDangerMargin[0] * CpSurface.Side0.fJ);
		CpSurface.Side0.fZ += (theApp.m_LastSettings.fDangerMargin[0] * CpSurface.Side0.fK);
		Line.Add(CpSurface);

		PROFILE->m_ScanLine[nScanLine].GetCoord(PROFILE->m_ScanLine[nScanLine].GetSize()/2, &CpSurface);
		CpSurface.Side0.fX += (theApp.m_LastSettings.fDangerMargin[0] * CpSurface.Side0.fI);
		CpSurface.Side0.fY += (theApp.m_LastSettings.fDangerMargin[0] * CpSurface.Side0.fJ);
		CpSurface.Side0.fZ += (theApp.m_LastSettings.fDangerMargin[0] * CpSurface.Side0.fK);
		Line.Add(CpSurface);

		PROFILE->m_ScanLine[nScanLine].GetFirstCoord(&CpSurface);
		CpSurface.Side0.fX += (theApp.m_LastSettings.fDangerMargin[0] * CpSurface.Side0.fI);
		CpSurface.Side0.fY += (theApp.m_LastSettings.fDangerMargin[0] * CpSurface.Side0.fJ);
		CpSurface.Side0.fZ += (theApp.m_LastSettings.fDangerMargin[0] * CpSurface.Side0.fK);
		Line.Add(CpSurface);
		*/

		PROFILE->m_ScanLine[nScanLine].GetFirstCoord(&CpSurface);
		Line.Add(CpSurface);
		theApp.m_FBCtrl.DoPolyLine(&Line,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);

		m_nDangerPlaneThreadAction = 10;
		break;
	case 10:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nDangerPlaneThreadAction = 999;
		};
		break;

	case 999:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {
			m_nDangerPlaneThreadAction = 0;
			if(m_nThreadAction == MOVE_ROBOT_WITH_BACKOFF) {
				m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
			} else {
				m_nSubThreadAction += 10;						//Add 10 because calling routine was a subthread anyway
			}
		}
		break;
	}

}

void CMainThread::MoveRobotWithBackOff()
{
	CCoord CpSurface,Cp;
	CPolyCoord Line;

	int	nSide=0;
	int	nNotSide=1;

	switch(m_nDangerPlaneThreadAction) {
	case 0:

		theApp.m_Motors.GetSurfacePos(&CpSurface);
		CpSurface.Side0.fX += (theApp.m_LastSettings.fDangerMargin[0] * CpSurface.Side0.fI);
		CpSurface.Side0.fY += (theApp.m_LastSettings.fDangerMargin[0] * CpSurface.Side0.fJ);
		CpSurface.Side0.fZ += (theApp.m_LastSettings.fDangerMargin[0] * CpSurface.Side0.fK);
		Line.Add(CpSurface);

		Cp = m_CpMoveTo;
		Cp.Side0.fX += (theApp.m_LastSettings.fDangerMargin[0] * m_CpMoveTo.Side0.fI);
		Cp.Side0.fY += (theApp.m_LastSettings.fDangerMargin[0] * m_CpMoveTo.Side0.fJ);
		Cp.Side0.fZ += (theApp.m_LastSettings.fDangerMargin[0] * m_CpMoveTo.Side0.fK);

		Line.Add(Cp);
		Line.Add(m_CpMoveTo);
		theApp.m_FBCtrl.DoPolyLine(&Line,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);

		m_nDangerPlaneThreadAction = 10;
		break;
	case 10:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nDangerPlaneThreadAction = 999;
		}
		break;
	case 999:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {
			m_nDangerPlaneThreadAction = 0;
			if(m_nThreadAction == MOVE_ROBOT_WITH_BACKOFF) {
				m_nThreadAction=WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
			} else {
				m_nSubThreadAction += 10;						//Add 10 because calling routine was a subthread anyway
			}
		}
		break;
	}

}

void CMainThread::PreWetRollerProbe()
{
	CCoord CpSurface,Cp,CpStart;
	CPolyCoord Line;
	int nLine,nSample;
	float	fLength;

	int	nSide=0;
	int	nNotSide=1;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_FBCtrl.RRWaterSpray(1);

		theApp.m_Motors.GetSurfacePos(&CpSurface);
		CpSurface.Side0.fX += (theApp.m_LastSettings.fDangerMargin[0] * CpSurface.Side0.fI);
		CpSurface.Side0.fY += (theApp.m_LastSettings.fDangerMargin[0] * CpSurface.Side0.fJ);
		CpSurface.Side0.fZ += (theApp.m_LastSettings.fDangerMargin[0] * CpSurface.Side0.fK);
		Line.Add(CpSurface);

		nLine = PROFILE->m_nScanLineL / 2;
		PROFILE->m_ScanLine[nLine].GetFirstCoord(&CpStart);
		for(nSample = 0;nSample < PROFILE->m_ScanLine[nLine].GetSize();nSample++) {
			PROFILE->m_ScanLine[nLine].GetCoord(nSample,&Cp);
			Line.Add(Cp);
			fLength = Cp.Length(CpStart,0);
			if(fLength > 250.0f) break;
		}

		theApp.m_FBCtrl.DoPolyLine(&Line,theApp.m_LastSettings.fMoveSpeed,theApp.m_LastSettings.fMoveSpeed);

		m_nSubThreadAction = 999;
		break;
	case 999:
		if(theApp.m_FBCtrl.IsMotionFinished() == true) {
			m_nSubThreadAction = 0;
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}

}


void CMainThread::OriginRobo9Plus9()
{
	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_FBCtrl.KillAllMotionPrograms();
		if(m_nRoboAxisMask & 0x0001) theApp.m_FBCtrl.SendStr(L"enable plc 11");
		if(m_nRoboAxisMask & 0x0002) theApp.m_FBCtrl.SendStr(L"enable plc 12");
		if(m_nRoboAxisMask & 0x0004) theApp.m_FBCtrl.SendStr(L"enable plc 13");
		if(m_nRoboAxisMask & 0x0400) theApp.m_FBCtrl.SendStr(L"enable plc 14");
		if(m_nRoboAxisMask & 0x0800) theApp.m_FBCtrl.SendStr(L"enable plc 15");
		if(m_nRoboAxisMask & 0x1000) theApp.m_FBCtrl.SendStr(L"enable plc 16");
		if(m_nRoboAxisMask & 0x2000) theApp.m_FBCtrl.SendStr(L"enable plc 19");	//Turntable
		Sleep(500);
		m_nSubThreadAction = 10;
		break;
	case 10:
		if(theApp.m_FBCtrl.IsAxesInPos(m_nRoboAxisMask) == true) {
			m_nSubThreadAction = 0;
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}

}


void CMainThread::JogToPosition()
{
	CString Buff;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.m_FBCtrl.SendStr(L"&1qa");
		theApp.m_FBCtrl.SendStr(L"&2qa");
		theApp.m_FBCtrl.SendStr(L"&3qa");
		theApp.m_FBCtrl.SendStr(L"undefine all");
		if(m_nJogWhatToPosition & _JOG_TOWER) {
			if(m_nJogRobotSide & 1) {
				Buff.Format(L"#2j=%d #3j=%d",
					(int)(m_CpMoveTo.Side0.fPos[7] / theApp.m_Axes[1].fStepSize),
					(int)(m_CpMoveTo.Side0.fPos[8] / theApp.m_Axes[2].fStepSize));
				theApp.m_FBCtrl.SendStr(Buff);
				Sleep(10);
			};
			if(m_nJogRobotSide & 2) {
				Buff.Format(L"#13j=%d",
					(int)(m_CpMoveTo.Side1.fPos[8] / theApp.m_Axes[12].fStepSize));
				theApp.m_FBCtrl.SendStr(Buff);
				Sleep(10);
			}
		}

		if(m_nJogWhatToPosition & _JOG_ROBOT) {
			if(m_nJogRobotSide & 1) {
				Buff.Format(L"#4j=%d #5j=%d #6j=%d #7j=%d #8j=%d #9j=%d",
					(int)(m_CpMoveTo.Side0.fPos[0] / theApp.m_Robot[0].m_fStepSize[0]), (int)(m_CpMoveTo.Side0.fPos[1] / theApp.m_Robot[0].m_fStepSize[1]),
					(int)(m_CpMoveTo.Side0.fPos[2] / theApp.m_Robot[0].m_fStepSize[2]), (int)(m_CpMoveTo.Side0.fPos[3] / theApp.m_Robot[0].m_fStepSize[3]),
					(int)(m_CpMoveTo.Side0.fPos[4] / theApp.m_Robot[0].m_fStepSize[4]), (int)(m_CpMoveTo.Side0.fPos[5] / theApp.m_Robot[0].m_fStepSize[5]));
				theApp.m_FBCtrl.SendStr(Buff);
				Sleep(10);
			}
			if(m_nJogRobotSide & 2) {
				Buff.Format(L"#14j=%d #15j=%d #16j=%d #17j=%d #18j=%d #19j=%d",
					(int)(m_CpMoveTo.Side1.fPos[0] / theApp.m_Robot[1].m_fStepSize[0]), (int)(m_CpMoveTo.Side1.fPos[1] / theApp.m_Robot[1].m_fStepSize[1]),
					(int)(m_CpMoveTo.Side1.fPos[2] / theApp.m_Robot[1].m_fStepSize[2]), (int)(m_CpMoveTo.Side1.fPos[3] / theApp.m_Robot[1].m_fStepSize[3]),
					(int)(m_CpMoveTo.Side1.fPos[4] / theApp.m_Robot[1].m_fStepSize[4]), (int)(m_CpMoveTo.Side1.fPos[5] / theApp.m_Robot[1].m_fStepSize[5]));
				theApp.m_FBCtrl.SendStr(Buff);
				Sleep(10);
			}
		}

		Sleep(500);
		m_nSubThreadAction = 10;
		break;
	case 10:
		if(theApp.m_FBCtrl.IsJogMoveFinished(m_nJogRobotSide, m_nJogWhatToPosition) == true) {
			m_nSubThreadAction = 0;
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}
}


void CMainThread::ContourScanGKN()
{
	static int nLastCoordinate;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	static CCoord	CpHead,CpSurface,Cp, CpDanger;
	CPolyCoord StartOfLine;


	switch(m_nSubThreadAction) {
	case 0: //move out of danger zone
		switch(theApp.m_nJobNumber){
		default:
			m_bAbortFlag = FALSE;
			m_bPause = FALSE;
			theApp.m_bScanning=TRUE;
			theApp.m_Motors.SuspendMotors();
			PROFILE->CalculateDangerZone();

			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
			if(m_bMoveToStartAtBeginningOfScan!=TRUE) {
				m_nSubThreadAction = 30;
				return;
			}

			CpDanger = PROFILE->m_CpDanger[PORTSIDE];
			switch(theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide) {
			case 0:
				theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
				m_CpMoveTo.Side0.fY = Cp.Side0.fY;
				m_CpMoveTo.Side1.fXt=0.0f;
				m_CpMoveTo.Side1.fYt=0.0f;
				break;
			case 1:
				theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
				m_CpMoveTo.Side1.fY = Cp.Side1.fY;
				m_CpMoveTo.Side0.fXt=0.0f;
				m_CpMoveTo.Side0.fYt=0.0f;
				break;
			case 2:
				theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
				theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
				m_CpMoveTo.Side0.fY = Cp.Side0.fY;
				m_CpMoveTo.Side1.fY = Cp.Side1.fY;
				break;
			}
			theApp.m_Motors.EnableAxes(BOTH_MOVETO);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(300);

			m_nSubThreadAction=10;
			break;
		case GKN:
			m_bAbortFlag = FALSE;
			m_bPause = FALSE;
			theApp.m_bScanning=TRUE;
			theApp.m_Motors.SuspendMotors();
			PROFILE->CalculateDangerZone();

			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);
			if(m_bMoveToStartAtBeginningOfScan!=TRUE) {
				m_nSubThreadAction = 30;
				return;
			}

			CpDanger = PROFILE->m_CpDanger[PORTSIDE];
			theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE); //Need to move Y's to Zero and left side Ztip to -90 to keep clear of rotating blade
			m_CpMoveTo.Side0.fY = (float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
			m_CpMoveTo.Side0.fYt = -90.0f;
			m_CpMoveTo.Side1.fY = (float)theApp.m_Axes[theApp.m_Tank.nYRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize * -1.0f;
			m_CpMoveTo.Side1.fXt = 0.0f;
			m_CpMoveTo.Side1.fYt = 0.0f;
			theApp.m_Motors.EnableAxes(BOTH_MOVETO_WITH_R);
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(800);

			m_nSubThreadAction=5;
			break;
			}
		break;
	case 5: //GKN FUNCTION Move Left Z up
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {

			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);

			m_CpMoveTo.Side0.fZ = (float)theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize * -1.0f;;

			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(800);

			m_nSubThreadAction=8;

		}
		break;
	case 8: //GKN FUNCTION Move rotate
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {

			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);

			CpSurface = PROFILE->m_ScanLine[m_nScanStartLine].m_pCp[0];
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);

			m_CpMoveTo.Side0.fR = CpHead.Side0.fR;

			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(300);

			m_nSubThreadAction=9;
		}
		break;
	case 9: //GKN FUNCTION MOVE X
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {

			theApp.m_Motors.GetHeadPos(&m_CpMoveTo);

			CpSurface = PROFILE->m_ScanLine[m_nScanStartLine].m_pCp[0];
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);

			CpDanger = PROFILE->m_CpDanger[PORTSIDE];
			switch(theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide) {
			case 0:
			//	m_CpMoveTo.Side0 = CpHead.Side0;
				m_CpMoveTo.Side0.fX = CpHead.Side0.fX;
				m_CpMoveTo.Side1.fX = CpHead.Side0.fX;
				theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
				m_CpMoveTo.Side0.fY = (float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
				break;
			case 1:
			//	m_CpMoveTo.Side1 = CpHead.Side1;
				m_CpMoveTo.Side1.fX = CpHead.Side1.fX;
				m_CpMoveTo.Side0.fX = CpHead.Side1.fX;
				theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
				m_CpMoveTo.Side1.fY = (float)theApp.m_Axes[theApp.m_Tank.nYRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize * -1.0f;
				break;
			case 2:
			//	m_CpMoveTo = CpHead;
				m_CpMoveTo.Side0.fX = CpHead.Side0.fX;
				m_CpMoveTo.Side1.fX = CpHead.Side1.fX;
				theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
				theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
				m_CpMoveTo.Side0.fY = (float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
				m_CpMoveTo.Side1.fY = (float)theApp.m_Axes[theApp.m_Tank.nYRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize * -1.0f;
				break;
			}
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			Sleep(300);

			m_nSubThreadAction=10;
		}
		break;
	case 10: //moveto start except Y keep in out of danger zone
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {

			switch(theApp.m_nJobNumber){
			default:
				theApp.m_Motors.GetHeadPos(&m_CpMoveTo);

				CpSurface = PROFILE->m_ScanLine[m_nScanStartLine].m_pCp[0];
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);

				CpDanger = PROFILE->m_CpDanger[PORTSIDE];
				switch(theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide) {
				case 0:
					m_CpMoveTo.Side0 = CpHead.Side0;
					theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
					m_CpMoveTo.Side0.fY = Cp.Side0.fY;
					break;
				case 1:
					m_CpMoveTo.Side1 = CpHead.Side1;
					theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
					m_CpMoveTo.Side1.fY = Cp.Side1.fY;
					break;
				case 2:
					m_CpMoveTo = CpHead;
					theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
					theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
					m_CpMoveTo.Side0.fY = Cp.Side0.fY;
					m_CpMoveTo.Side1.fY = Cp.Side1.fY;
					break;
				}
				theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
				theApp.m_Motors.MoveTo(DONT_WAIT);
				Sleep(300);

				m_nSubThreadAction=20;
				break;
			case GKN:
				theApp.m_Motors.GetHeadPos(&m_CpMoveTo);

				CpSurface = PROFILE->m_ScanLine[m_nScanStartLine].m_pCp[0];
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);

				CpDanger = PROFILE->m_CpDanger[PORTSIDE];
				switch(theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide) {
				case 0:
					m_CpMoveTo.Side0 = CpHead.Side0;
					m_CpMoveTo.Side1.fX = CpHead.Side0.fX;
					theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
					m_CpMoveTo.Side0.fY = (float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
					break;
				case 1:
					m_CpMoveTo.Side1 = CpHead.Side1;
					m_CpMoveTo.Side0.fX = CpHead.Side1.fX;
					theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
					m_CpMoveTo.Side1.fY = (float)theApp.m_Axes[theApp.m_Tank.nYRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize * -1.0f;
					break;
				case 2:
					m_CpMoveTo = CpHead;
					theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
					theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
					m_CpMoveTo.Side0.fY = (float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
					m_CpMoveTo.Side1.fY = (float)theApp.m_Axes[theApp.m_Tank.nYRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize * -1.0f;
					break;
				}
				theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
				theApp.m_Motors.MoveTo(DONT_WAIT);
				Sleep(300);

				m_nSubThreadAction=20;
			break;
			}
		}
		break;
	case 20: //move to start
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.GetHeadPos(&Cp);

			CpSurface = PROFILE->m_ScanLine[m_nScanStartLine].m_pCp[0];

			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);

			switch(theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide) {
			case 0:
				CpHead.Side1.fX=Cp.Side1.fX;
				CpHead.Side1.fY=Cp.Side1.fY;
				CpHead.Side1.fZ=Cp.Side1.fZ;
				CpHead.Side1.fXt=Cp.Side1.fXt;
				CpHead.Side1.fYt=Cp.Side1.fYt;
				break;
			case 1:
				CpHead.Side0.fX=Cp.Side0.fX;
				CpHead.Side0.fY=Cp.Side0.fY;
				CpHead.Side0.fZ=Cp.Side0.fZ;
				CpHead.Side0.fXt=Cp.Side0.fXt;
				CpHead.Side0.fYt=Cp.Side0.fYt;
				break;
			case 2:
				break;
			}


			if(theApp.m_nJobNumber==GKN)
				ContourScanRAxis(m_nScanStartLine);

			theApp.m_Thread.m_CpMoveTo = CpHead;
			theApp.m_Motors.DesirePosEqualsCoord(m_CpMoveTo);
			theApp.m_Motors.MoveTo(DONT_WAIT);
			m_nSubThreadAction=30;
			m_nScanLine = m_nScanStartLine;
		}
		break;
	case 30:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) { //Do first line
			theApp.m_Motors.SuspendMotors();

			PROFILE->CalculateToolPath(m_nScanLine=m_nScanStartLine);
			switch(theApp.m_PmacUser.m_nScanSide) {
			case 0:	theApp.m_Motors.EnableAxes(LEFT_POLY_MOVE);
				break;
			case 1:	theApp.m_Motors.EnableAxes(RIGHT_POLY_MOVE);
				break;
			case 2:	theApp.m_Motors.EnableAxes(BOTH_POLY_MOVE);
				break;
			}
			if(theApp.m_LastSettings.fFeedRate <= 0.0f) theApp.m_LastSettings.fFeedRate = 100.0f;
			theApp.m_Motors.SetScanSpeed(PROFILE->m_fScanSpeed * theApp.m_LastSettings.fFeedRate / 100.0f);
			nLastCoordinate=theApp.m_Motors.DownLoadPolyMoveCoordinates(&PROFILE->m_ToolPath, m_nDir=0);
			theApp.m_PmacUser.SetupLookAhead();
			theApp.m_Motors.CollectComplexData(TRUE);
			theApp.m_Motors.StartPolyMoveWithLookAhead(PROFILE->m_ToolPath.m_nCoordL);
			m_nSubThreadAction=40;
		};

		break;
	case 40: //Do 2nd line onwards
		if(theApp.m_Motors.IsPolyMotionFinished() == TRUE) {
			theApp.m_Motors.CollectComplexData(FALSE);

//			theApp.m_pScanData->ShrinkAndShiftLine(m_nScanLine-m_nScanStartLine,PORTSIDE);

			if(PROFILE->m_nLineIncrement<=0) PROFILE->m_nLineIncrement = 1;
			theApp.m_pScanData->SaveVolumeDuringScan((m_nScanLine-PROFILE->m_nScanStartLine)/PROFILE->m_nLineIncrement);

			m_nDir ^= 1;
			m_nScanLine+=PROFILE->m_nLineIncrement;
			if((m_nScanLine<PROFILE->m_nScanFinishLine) && (m_bAbortFlag==FALSE)) {
				if(theApp.m_nJobNumber==GKN)
					ContourScanRAxis(m_nScanLine);
				PROFILE->CalculateToolPath(m_nScanLine);
				theApp.m_Motors.SetScanSpeed(PROFILE->m_fScanSpeed * theApp.m_LastSettings.fFeedRate / 100.0f);
				nLastCoordinate=theApp.m_Motors.DownLoadPolyMoveCoordinates(&PROFILE->m_ToolPath, m_nDir);
				theApp.m_Motors.CollectComplexData(TRUE);
				theApp.m_Motors.StartPolyMoveWithLookAhead(PROFILE->m_ToolPath.m_nCoordL);
			} else {
				//if have filename save?

				m_nThreadAction = WAIT_FOR_MOVE_FINISHED;
			}

			pFrame->SendMessage(INVALIDATE_VIEW);

		} else {
			nLastCoordinate=theApp.m_Motors.DownLoadNextPolyMoveCoordinate(nLastCoordinate,&PROFILE->m_ToolPath, m_nDir);
		}
		break;
	}

}

bool CMainThread::IsScanning()
{
	if(theApp.m_bScanning == true) return true;

	switch(m_nThreadAction) {
	case CONTOUR_SCAN:
	case TURNTABLE_SCAN:
	case CONTINUOUS_TURNTABLE_SCAN:
	case SEGMENT_TURNTABLE_SCAN:
	case ROBOT_CONTOUR_SCAN:
	case MULTI_PROBE_SCAN:
	case ROBOT_ROLLER_PROBE_SCAN:
		return true;
		break;
	}

	return false;


}


void CMainThread::RetrieveTool(int nSide)
{
	CCoord CpSurface, CpMachine;
	static CCoord CpPark;

	if(theApp.m_FBCtrl.IsDoorClosed(nSide) == false) {
		theApp.m_Motors.SuspendMotors();
		theApp.m_StatusMessage.LoadString(IDS_Cabinet_door_not_shut);
		return;
	}


	switch(m_nSubThreadAction) {
	case 0: 
		theApp.m_StatusMessage.LoadString(IDS_Retrieving_tool);
		theApp.m_LastSettings.fFeedRate = 100.0f;
		theApp.m_Motors.SuspendMotors();
		theApp.m_ToolChangers.getParkPos(m_nToolIndexToRetrieveDeposit[nSide], &CpPark);
		theApp.m_Kinematics.TipsFromNormals(&CpPark, nSide);
		theApp.m_Motors.GetMachinePos(&CpMachine);
		CpMachine.Side[nSide].fX = CpPark.Side[nSide].fX + theApp.m_Tank.vToolRegion[nSide].x;
		CpMachine.Side[nSide].fY = CpPark.Side[nSide].fY;
		CpMachine.Side[nSide].fXt = CpPark.Side[nSide].fXt;
		CpMachine.Side[nSide].fYt = CpPark.Side[nSide].fYt;
		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(CpMachine);
		theApp.m_Motors.MoveTo(DONT_WAIT,_MACHINE);

		theApp.m_FBCtrl.UnlockTool(nSide);
		m_nSubThreadAction += 10;
		break;
	case 10:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction += 10;
		}
		break;
	case 20:
		if(theApp.m_FBCtrl.IsMotionFinished() == true && theApp.m_FBCtrl.IsItAtDesirePos() == true) {
			theApp.m_Motors.GetMachinePos(&CpMachine);
			CpMachine.Side[nSide].fZ = CpPark.Side[nSide].fZ;
			theApp.m_Motors.DesirePosEqualsCoord(CpMachine);
			theApp.m_Motors.MoveTo(DONT_WAIT,_MACHINE);
			theApp.m_LastSettings.fFeedRate = 100.0f;
			m_nSubThreadAction += 10;
		}
		break;
	case 30:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction += 10;
		}
		break;
	case 40:
		if(theApp.m_FBCtrl.IsMotionFinished() == true && theApp.m_FBCtrl.IsItAtDesirePos() == true && theApp.m_FBCtrl.IsToolLocked(nSide) == false) {
			theApp.m_Motors.GetMachinePos(&CpMachine);
			CpMachine.Side[nSide] = CpPark.Side[nSide];
			theApp.m_LastSettings.fFeedRate = 25.0f;
			theApp.m_Motors.DesirePosEqualsCoord(CpMachine);
			theApp.m_Motors.MoveTo(DONT_WAIT,_MACHINE);
			theApp.m_LastSettings.fFeedRate = 100.0f;
			m_nSubThreadAction += 10;
		}
		break;
	case 50:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction += 10;
		}
		break;
	case 60:
		if(theApp.m_FBCtrl.IsMotionFinished() == true && theApp.m_FBCtrl.IsItAtDesirePos() == true) {
			theApp.m_FBCtrl.LockTool(nSide);
			m_nSubThreadAction += 10;
			theApp.m_StatusMessage.LoadString(IDS_Waiting_to_lock_clamp);
			theApp.m_rgbStatusMessageBk = RGB(255,0,0);
		}
		break;
	case 70:
		if(theApp.m_FBCtrl.IsToolLocked(nSide) == true) {
			theApp.m_StatusMessage.LoadString(IDS_Clamp_locked);
			theApp.m_rgbStatusMessageBk = GetSysColor(COLOR_MENU);
			theApp.m_Motors.EnableCrashDetector(true, nSide);
			theApp.m_ToolChangers.InvalidateKinematicWaterPath(nSide);
			theApp.m_Motors.GetMachinePos(&CpMachine);
			CpMachine.Side[nSide].fZ += theApp.m_Tank.vToolLatch[nSide].z;
			theApp.m_LastSettings.fFeedRate = 25.0f;
			theApp.m_Motors.DesirePosEqualsCoord(CpMachine);
			theApp.m_Motors.MoveTo(DONT_WAIT,_MACHINE);
			theApp.m_LastSettings.fFeedRate = 100.0f;
			m_nSubThreadAction += 10;
		}
		break;
	case 80:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction += 10;
		}
		break;
	case 90:
		if(theApp.m_FBCtrl.IsMotionFinished() == true && theApp.m_FBCtrl.IsItAtDesirePos() == true) {
			theApp.m_Motors.GetMachinePos(&CpMachine);
			CpMachine.Side[nSide].fX += theApp.m_ToolChangers.getLength(m_nToolIndexToRetrieveDeposit[nSide]);
			theApp.m_LastSettings.fFeedRate = 25.0f;
			theApp.m_Motors.DesirePosEqualsCoord(CpMachine);
			theApp.m_Motors.MoveTo(DONT_WAIT,_MACHINE);
			theApp.m_LastSettings.fFeedRate = 100.0f;
			m_nSubThreadAction += 10;
		}
		break;
	case 100:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction = 999;
		}
		break;

	case 999:
		if(theApp.m_FBCtrl.IsMotionFinished() == true && theApp.m_FBCtrl.IsItAtDesirePos() == true) {
			theApp.m_StatusMessage.LoadString(IDS_Sequence_finished_succesfully);
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}

}



void CMainThread::DepositTool(int nSide)
{
	CCoord CpSurface, CpMachine;
	static CCoord CpPark;

	if(theApp.m_FBCtrl.IsDoorClosed(nSide) == false) {
		theApp.m_Motors.SuspendMotors();
		theApp.m_StatusMessage.LoadString(IDS_Cabinet_door_not_shut);
	}

	switch(m_nSubThreadAction) {
	case 0: 
		theApp.m_StatusMessage.LoadString(IDS_Depositing_tool);
		theApp.m_FBCtrl.PumpStop(theApp.m_Tank.nLeftPump, nSide);
		theApp.m_Motors.EnableCrashDetector(false, nSide);


		theApp.m_LastSettings.fFeedRate = 100.0f;
		theApp.m_Motors.SuspendMotors();
		theApp.m_ToolChangers.getParkPos(m_nToolIndexToRetrieveDeposit[nSide], &CpPark);
		theApp.m_Kinematics.TipsFromNormals(&CpPark, nSide);
		theApp.m_Motors.GetMachinePos(&CpMachine);
		CpMachine.Side[nSide] = CpPark.Side[nSide];
		CpMachine.Side[nSide].fX += theApp.m_ToolChangers.getLength(m_nToolIndexToRetrieveDeposit[nSide]);
		CpMachine.Side[nSide].fZ += theApp.m_Tank.vToolLatch[nSide].z;

		theApp.m_Motors.EnableAxes(BOTH_MOVETO);
		theApp.m_Motors.DesirePosEqualsCoord(CpMachine);
		theApp.m_Motors.MoveTo(DONT_WAIT,_MACHINE);
		m_nSubThreadAction += 10;
		break;
	case 10:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction += 10;
		}
		break;
	case 20:
		if(theApp.m_FBCtrl.IsMotionFinished() == true && theApp.m_FBCtrl.IsItAtDesirePos() == true) {
			theApp.m_Motors.GetMachinePos(&CpMachine);
			CpMachine.Side[nSide] = CpPark.Side[nSide];
			CpMachine.Side[nSide].fZ += theApp.m_Tank.vToolLatch[nSide].z;
			theApp.m_LastSettings.fFeedRate = 25.0f;
			theApp.m_Motors.DesirePosEqualsCoord(CpMachine);
			theApp.m_Motors.MoveTo(DONT_WAIT,_MACHINE);
			theApp.m_LastSettings.fFeedRate = 100.0f;
			m_nSubThreadAction += 10;
		}
		break;
	case 30:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction += 10;
		}
		break;
	case 40:
		if(theApp.m_FBCtrl.IsMotionFinished() == true && theApp.m_FBCtrl.IsItAtDesirePos() == true) {
			theApp.m_Motors.GetMachinePos(&CpMachine);
			CpMachine.Side[nSide] = CpPark.Side[nSide];
			theApp.m_LastSettings.fFeedRate = 25.0f;
			theApp.m_Motors.DesirePosEqualsCoord(CpMachine);
			theApp.m_Motors.MoveTo(DONT_WAIT,_MACHINE);
			theApp.m_LastSettings.fFeedRate = 100.0f;
			m_nSubThreadAction += 10;
		}
		break;
	case 50:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction += 10;
		}
		break;
	case 60:
		if(theApp.m_FBCtrl.IsMotionFinished() == true && theApp.m_FBCtrl.IsItAtDesirePos() == true) {
			theApp.m_FBCtrl.UnlockTool(nSide);
			theApp.m_StatusMessage.LoadString(IDS_Trying_to_unlock_clamp);
			m_nSubThreadAction += 10;
		}
		break;
	case 70:
		if(theApp.m_FBCtrl.IsToolLocked(nSide) == false) {
			theApp.m_StatusMessage.LoadString(IDS_Clamp_unlocked);
			theApp.m_Motors.GetMachinePos(&CpMachine);
			CpMachine.Side[nSide].fX += theApp.m_Tank.vToolRegion[nSide].x;
			theApp.m_LastSettings.fFeedRate = 25.0f;
			theApp.m_Motors.DesirePosEqualsCoord(CpMachine);
			theApp.m_Motors.MoveTo(DONT_WAIT,_MACHINE);
			theApp.m_LastSettings.fFeedRate = 100.0f;
			m_nSubThreadAction += 10;
		}
		break;
	case 80:
		if(theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction = 999;
		}
		break;
	case 999:
		if(theApp.m_FBCtrl.IsMotionFinished() == true && theApp.m_FBCtrl.IsItAtDesirePos() == true) {
			theApp.m_StatusMessage.LoadString(IDS_Sequence_finished_succesfully);
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}

}


void CMainThread::TurnTableScanKinematic()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CCoord Cp0,Cp1;
	int nNextScanLine,nTS;
	float fDeltaX;
	CString Buff;
	CPolyCoord Line;
	static float fRDesirePos;

	switch(m_nSubThreadAction) {
	case 0:
		theApp.StartLogging();

		theApp.m_pScanData->GenerateRotarySamplePositionVertices();

		theApp.m_Motors.CollectComplexData(FALSE);
		m_nLastGainValue = 0;
		m_bAbortFlag = FALSE;
		m_bPause = FALSE;
		theApp.m_Motors.SuspendMotors();
		m_nRevCounter = m_nScanLine = m_nScanStartLine;
		theApp.m_Motors.SetRevCount(m_nRevCounter);

		switch(theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide) {
		case 0: m_nMoveSide = 1;
			break;
		case 1: m_nMoveSide = 2;
			break;
		case 2: m_nMoveSide = 3;
			break;
		}

		if(m_bMoveToStartAtBeginningOfScan!=TRUE) {
			m_nSubThreadAction = 100;
			return;
		}
		if(theApp.m_LastSettings.nReverseScanLines == 0) {
			nNextScanLine = m_nScanStartLine+1;
			if(nNextScanLine >= PROFILE->m_ScanLine[0].m_nCoordL) nNextScanLine = PROFILE->m_ScanLine[0].m_nCoordL - 1;
		} else {
			nNextScanLine = m_nScanStartLine-1;
			if(nNextScanLine < 0) nNextScanLine = 0;
		}

		Cp0 = PROFILE->m_ScanLine[0].m_pCp[m_nScanStartLine];
		Cp1 = PROFILE->m_ScanLine[0].m_pCp[nNextScanLine];

		theApp.m_Motors.GetSurfacePos(&m_CpSurface);
		m_CpSurface.Side0.fR = 0.0f;
		InterpolatePosForAngle(&m_CpSurface, &Cp0, &Cp1);

		m_nDangerPlaneMask = theApp.m_LastSettings.nDangerPlaneMoveMask;
		m_nDangerPlaneThreadAction = 0;

		switch(theApp.m_nUseTurntableOrRollers) {
		case 0:	m_nSubThreadAction = 10;
			break;
		case 1: m_nSubThreadAction = 20;
			break;
		}
		m_nScanFinishLine = theApp.m_pScanData->m_nScanFinishLine;

		break;
	case 10:
		IntelligentGoto();
		break;
	case 20:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nSubThreadAction = 100;
		}
		break;

	case 100:
		m_bMoveTurnTable = true;
		switch(PROFILE->m_ScanLine[0].m_pCp[m_nScanLine].nType) {
		default:
		case CONTINUOUS_TURNTABLE:
			m_nRevCounter = m_nScanLine;
			theApp.m_Motors.SetRevCount(m_nRevCounter);
			theApp.m_Motors.GetSurfacePos(&m_CpSurface);
			theApp.m_Motors.StartTurntableCartesianScanningAxes(m_CpSurface);
			switch(theApp.m_nUseTurntableOrRollers) {
			default:
				theApp.m_Motors.StartContinuousRotate(theApp.m_Tank.nRLeft,m_fCurrentSpeed = PROFILE->m_fScanSpeed);
				break;
			case 1:
				theApp.m_Motors.StartContinuousRotate(theApp.m_Tank.nRRight,m_fCurrentSpeed = PROFILE->m_fScanSpeed);
				break;
			}

			theApp.m_Motors.CollectComplexData(TRUE);
			m_bScanLineUpdateByMotorRevCounter=true;
			m_nSubThreadAction = 200;
			break;
		case SEGMENT_TURNTABLE:
			m_nSubThreadAction = 300;
			break;
		case JUMP_TURNTABLE:
			m_nSubThreadAction = 400;
			break;
		}
		break;

	case 200:	//Continuous Turntable scan
		if(m_nRevCounter - theApp.m_Motors.GetRevCount()) {

			theApp.m_pScanData->FillForDPCError(m_nScanLine-PROFILE->m_nScanStartLine);
			theApp.m_pScanData->SaveVolumeDuringScan(m_nScanLine-PROFILE->m_nScanStartLine);

			pFrame->SendMessage(INVALIDATE_VIEW);
			if(m_fCurrentSpeed - PROFILE->m_fScanSpeed) {
				switch(theApp.m_nUseTurntableOrRollers) {
				default:
					theApp.m_Motors.StartContinuousRotate(theApp.m_Tank.nRLeft,m_fCurrentSpeed = PROFILE->m_fScanSpeed);
					break;
				case 1:
					theApp.m_Motors.StartContinuousRotate(theApp.m_Tank.nRRight,m_fCurrentSpeed = PROFILE->m_fScanSpeed);
					break;
				}
			}
		//	m_nScanLine++; //this is updated in m_Data.CollectComplexData();
			m_nRevCounter = theApp.m_Motors.GetRevCount();
			if(m_nScanLine >= m_nScanFinishLine) {
				theApp.m_Motors.CollectComplexData(FALSE);
				theApp.m_Motors.StopContinuousRotate(theApp.m_Tank.nRLeft);
				theApp.m_Motors.SuspendMotors();
				m_nSubThreadAction=999;
				return;
			}
			if(PROFILE->m_ScanLine[0].m_pCp[m_nScanLine].nType == CONTINUOUS_TURNTABLE) {
				m_nSubThreadAction = 200;
			} else {
				m_nSubThreadAction = 100;
				return;
			}
		}
		if(m_bAbortFlag == true) {
			theApp.m_Motors.CollectComplexData(FALSE);
			theApp.m_Motors.StopContinuousRotate(theApp.m_Tank.nRLeft);
			theApp.m_Motors.SuspendMotors();
			m_nSubThreadAction=999;
			return;
		}

		theApp.m_Motors.GetSurfacePos(&m_CpSurface);
		
		if((m_CpSurface.Side0.fR > 0.1f) && (m_CpSurface.Side0.fR < 350.0f)) {

			if(theApp.m_LastSettings.nReverseScanLines == 0) {
				nNextScanLine = m_nScanLine+1;
				if(nNextScanLine >= PROFILE->m_ScanLine[0].m_nCoordL) nNextScanLine = PROFILE->m_ScanLine[0].m_nCoordL - 1;
			} else {
				nNextScanLine = m_nScanLine-1;
				if(nNextScanLine < 0) nNextScanLine = 0;
			}

			Cp0 = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];
			Cp1 = PROFILE->m_ScanLine[0].m_pCp[nNextScanLine];

//			theApp.m_UtUser.TurntableGainChange(Cp0);

			InterpolatePosForAngle(&m_CpSurface, &Cp0, &Cp1);

			if(theApp.m_LastSettings.nUseEccentricCompensation != 0) {
				fDeltaX = cosf((m_CpSurface.Side0.fR-m_CpEccentricCentre.Side0.fR)  * DEG_TO_RAD) * m_CpEccentricCentre.fWp[0];
				m_CpSurface.Side0.fX -= fDeltaX;
				m_CpSurface.Side1.fX -= fDeltaX;
			}
			if (PROFILE->m_bUseAdditionalAngleDuringScanning[0]) {
				PROFILE->ApplyAdditionalAngle(&m_CpSurface);
			}

			Buff.Format(L"%d %d %d     %d    %.01f %.01f %.01f %.01f %.01f %.01f\r\n",m_nScanLine,nNextScanLine,theApp.m_Axes[10].nRevCounter,theApp.m_PmacUser.m_nMachinePosition[10],
				m_CpSurface.Side0.fX,m_CpSurface.Side0.fY,m_CpSurface.Side0.fZ,m_CpSurface.Side0.fXt,m_CpSurface.Side0.fY,m_CpSurface.Side0.fR);
			theApp.AddToLogging(&Buff);


			theApp.m_Motors.UpdateTurntableCartesianScanningAxes(m_CpSurface);
		}
		break;


	case 300:	//Segment Turntable scan
		m_nRevCounter = m_nScanLine;
		theApp.m_Motors.SetRevCount(m_nRevCounter);

		m_CpSurface = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];
		theApp.m_FBCtrl.JogJustR(fRDesirePos = m_CpSurface.fAngle[0], PROFILE->m_fScanSpeed);
		Line.Zero();
		Line.Add(m_CpSurface);
		theApp.m_FBCtrl.DoPolyLine(&Line,PROFILE->m_fScanSpeed,10.0f);
		m_nDir = 0;
		m_nSubThreadAction += 10;
		break;
	case 310:
		if(theApp.m_Motors.IsMotionStarted() == true) {
			m_nSubThreadAction+=10;
		}
		break;
	case 320:
		if(theApp.m_Motors.IsMotionStarted() == true && theApp.m_FBCtrl.IsRAxisInPosition(fRDesirePos) == true) {
			m_bScanLineUpdateByMotorRevCounter=false;
			theApp.m_Motors.CollectComplexData(TRUE);

			m_CpSurface = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];

//			theApp.m_User.TurntableGainChange(m_CpSurface);
			if(m_nDir & 1) {
				fRDesirePos = m_CpSurface.fAngle[0];
			} else {
				fRDesirePos = m_CpSurface.fAngle[1];
			}

			m_CpSurface = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];
			theApp.m_FBCtrl.JogJustR(fRDesirePos = m_CpSurface.fAngle[0], PROFILE->m_fScanSpeed);
			Line.Zero();
			Line.Add(m_CpSurface);
			theApp.m_FBCtrl.DoPolyLine(&Line,PROFILE->m_fScanSpeed,10.0f);

			m_nSubThreadAction+=10;
		}
		break;
	case 330:
		if(theApp.m_Motors.IsMotionStarted() == true) {
			m_nSubThreadAction+=10;
		}
		break;
	case 340:
		if(theApp.m_Motors.IsMotionStarted() == true && theApp.m_FBCtrl.IsRAxisInPosition(fRDesirePos) == true) {
			pFrame->SendMessage(INVALIDATE_VIEW);
			m_nScanLine++;
			if((m_nScanLine >= PROFILE->m_nScanFinishLine) || (m_bAbortFlag == true)) {
				theApp.m_Motors.CollectComplexData(FALSE);
				m_nSubThreadAction=999;
				return;
			}
			m_nDir ^= 1;

			if(PROFILE->m_ScanLine[0].m_pCp[m_nScanLine].nType == SEGMENT_TURNTABLE) {
				m_nSubThreadAction = 320;
			} else {
				m_nSubThreadAction = 100;
			}
		}
		break;

	case 400:	//Jump Turntable
		theApp.m_Motors.CollectComplexData(FALSE);
		Line.Zero();
		theApp.m_Motors.GetSurfacePos(&m_CpSurface);
		m_CpMoveTo = m_CpSurface;
		m_CpSurface.Side0.fX = PROFILE->m_CpDanger[0].Side0.fX;
		m_CpSurface.Side1.fX = PROFILE->m_CpDanger[0].Side1.fX;
		Line.Add(m_CpSurface);
		for(m_nScanLine;m_nScanLine < PROFILE->m_ScanLine[0].m_nCoordL;m_nScanLine++) {
			if(PROFILE->m_ScanLine[0].m_pCp[m_nScanLine].nType != JUMP_TURNTABLE) {
				m_CpSurface = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];
				m_CpSurface.Side0.fX = PROFILE->m_CpDanger[0].Side0.fX;
				m_CpSurface.Side1.fX = PROFILE->m_CpDanger[0].Side1.fX;
				Line.Add(m_CpSurface);
				m_CpSurface = PROFILE->m_ScanLine[0].m_pCp[m_nScanLine];
				Line.Add(m_CpSurface);
				theApp.m_FBCtrl.DoPolyLine(&Line,PROFILE->m_fScanSpeed,10.0f);
				m_nSubThreadAction+=10;
				break;
			}
		}
		m_nSubThreadAction = 999;
		break;
	case 410:
		if(theApp.m_Motors.IsMotionStarted() == true) {
			m_nSubThreadAction+=10;
		}
		break;
	case 420:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nSubThreadAction=100;
		}
		break;


	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
			m_nSafeThreadAction=0;
			m_nSubThreadAction = 0;
			theApp.m_bScanning=FALSE;
			for(nTS=0;nTS<3;nTS++) {
				m_CpSurface.nGain[0]=0;
				m_CpSurface.nTS=nTS;
//				theApp.m_UtUser.TurntableGainChange(m_CpSurface);
			}
			theApp.StopLogging();
		}

		break;

	}
}


//Angle must alread be in pCpSurface->Side0.fR
//This is for spiral turntable scans
void CMainThread::InterpolatePosForAngle(CCoord *pCpSurface, CCoord *pCp0, CCoord *pCp1)
{

	float fCoefficient = pCpSurface->Side0.fR / 360.0f;
	for(int nSide=0;nSide<2;nSide++) {
		pCpSurface->Side[nSide].fX = (pCp1->Side[nSide].fX - pCp0->Side[nSide].fX) * fCoefficient + pCp0->Side[nSide].fX;
		pCpSurface->Side[nSide].fY = (pCp1->Side[nSide].fY - pCp0->Side[nSide].fY) * fCoefficient + pCp0->Side[nSide].fY;
		pCpSurface->Side[nSide].fZ = (pCp1->Side[nSide].fZ - pCp0->Side[nSide].fZ) * fCoefficient + pCp0->Side[nSide].fZ;
		pCpSurface->Side[nSide].fI = (pCp1->Side[nSide].fI - pCp0->Side[nSide].fI) * fCoefficient + pCp0->Side[nSide].fI;
		pCpSurface->Side[nSide].fJ = (pCp1->Side[nSide].fJ - pCp0->Side[nSide].fJ) * fCoefficient + pCp0->Side[nSide].fJ;
		pCpSurface->Side[nSide].fK = (pCp1->Side[nSide].fK - pCp0->Side[nSide].fK) * fCoefficient + pCp0->Side[nSide].fK;
	}
}


void CMainThread::LeadscrewCompensation()
{
	CCoord CpSurface, CpMachine;
	static CCoord CpPark;
	static int nLoop;
	float fPos, fLaserPos;
	static int nDesirePos;
	D3DXVECTOR3 vPos;
	int nAxis = m_nLeadscrewCompensationAxis;
	CMainFrame*  pFrame;

	switch(m_nSubThreadAction) {
	case 0: 
		theApp.m_StatusMessage.LoadString(IDS_Leadscrew_compensation);
		theApp.m_Motors.SuspendMotors();
		nLoop = 0;
		m_nSubThreadAction+=10;
		break;
	case 10:
		fPos = theApp.m_Kinematics.m_LeadScrew[  theApp.m_Axes[nAxis].nPhysicalAxis-1  ].m_pPos[nLoop].fMachine;
		nDesirePos = (int)(fPos / theApp.m_Axes[nAxis].fStepSize);
		theApp.m_Motors.AbsoluteMove(&theApp.m_Axes[nAxis], nDesirePos);
		m_nSubThreadAction+=10;
		break;
	case 20:
		if(theApp.m_Motors.IsAxisInMachinePosition(m_nLeadscrewCompensationAxis, nDesirePos) == true) {
			Sleep(1000);

			theApp.m_FaroWorkerThread.GetRawXYZCoordinate(&vPos);
			theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vPos, &vPos);
			switch(theApp.m_LastSettings.nLaserAxis) {
			case 0: fLaserPos = vPos.x;
				break;
			case 1: fLaserPos = vPos.y;
				break;
			case 2: fLaserPos = vPos.z;
				break;
			}

			theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[nAxis].nPhysicalAxis-1 ].m_pPos[nLoop].fActualPos = fLaserPos;
			theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[nAxis].nPhysicalAxis-1 ].m_pPos[nLoop].fActualNeg = fLaserPos;
			theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[nAxis].nPhysicalAxis-1 ].m_pPos[nLoop].nActualPos = (int)(fLaserPos / theApp.m_Axes[nAxis].fStepSize);
			theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[nAxis].nPhysicalAxis-1 ].m_pPos[nLoop].nActualNeg = (int)(fLaserPos / theApp.m_Axes[nAxis].fStepSize);
			
			pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
			pFrame->SendMessage(UI_UPDATE_PMAC_SHEET);

			nLoop++;
			if(nLoop < theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[nAxis].nPhysicalAxis-1 ].GetSize()) {
				m_nSubThreadAction = 10;
			} else {
				m_nSubThreadAction = 999;
			}
		};
		break;
	case 999:
		if(theApp.m_FBCtrl.IsMotionFinished() == true && theApp.m_FBCtrl.IsItAtDesirePos() == true) {
			theApp.m_StatusMessage.LoadString(IDS_Leadscrew_compensation_finished_succesfully);
			m_nThreadAction=WAIT_FOR_MOVE_FINISHED;
		}
		break;
	}
}

void CMainThread::NormalizePPMAC()
{
	CMainFrame*  pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	int nSide, nNotSide, ii;
	CCoord Cp, CpNorm[2];
	static CCoord CpStart, CpPeek;
	float fAngle;
	D3DXVECTOR3 vPos, vNorm;
	D3DXQUATERNION quatPos;
	D3DXMATRIXA16 matWorld, matRot;
	CPolyCoord Line;
#define NUMBER_ARC_POINTS	5

	nSide = theApp.m_PmacUser.m_nScanSide = m_nNormSide;
	nSide == PORTSIDE ? nNotSide = STARBOARD : nNotSide = PORTSIDE;

	switch (m_nSubThreadAction) {
	case 0:
		m_nNormalizeStatus = 1;
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.EnableAxes(BOTH_POLY_MOVE);
		theApp.m_Motors.GetSurfacePos(&Cp);
		CpStart = Cp;

		if (theApp.m_LastSettings.nNormalizeXtip != 1) {
			m_nSubThreadAction = 60;
			return;
		}

		theApp.m_Kinematics.D3DXRotateMatrixFromNormal(matWorld, CpStart.Side[nSide].norm);

		m_ToolPath.Zero();
		for (ii = 0; ii <= NUMBER_ARC_POINTS; ii++) {
			vNorm = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			fAngle = ((theApp.m_LastSettings.fNormalizeAngle * 2.0f * (float)ii) / (float)NUMBER_ARC_POINTS) - theApp.m_LastSettings.fNormalizeAngle;
			D3DXMatrixRotationX(&matRot, fAngle * DEG_TO_RAD);
			D3DXVec3TransformCoord(&vNorm, &vNorm, &matRot);
			D3DXVec3TransformCoord(&vNorm, &vNorm, &matWorld);
			CopyMemory(Cp.Side[nSide].norm, vNorm, sizeof D3DXVECTOR3);

			m_ToolPath.Add(Cp);
			if (ii == 0) CpNorm[0] = Cp;
			if (ii == NUMBER_ARC_POINTS) CpNorm[1] = Cp;
		}
		if (fabsf(CpNorm[1].Side[nSide].fI - CpNorm[0].Side[nSide].fI) > fabsf(CpNorm[1].Side[nSide].fJ - CpNorm[0].Side[nSide].fJ)) {
			if (fabsf(CpNorm[1].Side[nSide].fI - CpNorm[0].Side[nSide].fI) > fabsf(CpNorm[1].Side[nSide].fK - CpNorm[0].Side[nSide].fK)) {
				m_ToolPath.SetPrimaryAxis(6);
			}
			else {
				m_ToolPath.SetPrimaryAxis(8);
			}
		}
		else {
			if (fabsf(CpNorm[1].Side[nSide].fJ - CpNorm[0].Side[nSide].fJ) > fabsf(CpNorm[1].Side[nSide].fK - CpNorm[0].Side[nSide].fK)) {
				m_ToolPath.SetPrimaryAxis(7);
			}
			else {
				m_ToolPath.SetPrimaryAxis(8);
			}
		}
		if (nSide == STARBOARD) {
			m_ToolPath.SetPrimaryAxis(m_ToolPath.m_nPrimaryAxis + 10);
		}
		m_CpNormal.AllocateCoords(100);
		m_CpNormal.ClearMemory();
		m_CpNormal.m_nPrimaryAxis = m_ToolPath.m_nPrimaryAxis;
		m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
		m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[NUMBER_ARC_POINTS];


		Line.Zero();
		Line.Add(CpNorm[0]);
		theApp.m_FBCtrl.DoPolyLine(&Line, theApp.m_LastSettings.fSpeedNormalize[0], theApp.m_LastSettings.fSpeedNormalize[0]);
		m_nSubThreadAction += 10;
		break;
	case 10:
		if (theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction += 10;
		}
		break;
	case 20:
		if (theApp.m_FBCtrl.IsMotionFinished() == true) {
			theApp.m_bCollectNormalizeData = TRUE;
			theApp.m_FBCtrl.DoPolyLine(&m_ToolPath, theApp.m_LastSettings.fSpeedNormalize[0], theApp.m_LastSettings.fSpeedNormalize[0]);
			m_nSubThreadAction += 10;
		}
		break;
	case 30:
		if (theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction += 10;
		}
		break;
	case 40:
		if (theApp.m_FBCtrl.IsMotionFinished() == true) {
			theApp.m_bCollectNormalizeData = FALSE;
			m_nNormPlotDirection = HORIZONTAL;
			m_CpNormal.FindPeakAmplitude(&CpPeek, theApp.m_LastSettings.nPeakMode[0]);

			CpStart.Side[nSide].norm = CpPeek.Side[nSide].norm;

			Line.Zero();
			Line.Add(CpStart);
			theApp.m_FBCtrl.DoPolyLine(&Line, theApp.m_LastSettings.fSpeedNormalize[0], theApp.m_LastSettings.fSpeedNormalize[0]);
			NormalizeViewInvalidate(m_nNormPlotDirection, CpStart);
			m_nSubThreadAction += 10;
		}
		break;
	case 50:
		if (theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction += 10;
		}
		break;

	case 60:
		if (theApp.m_FBCtrl.IsMotionFinished() == true) {
			theApp.m_Motors.GetSurfacePos(&Cp);
			CpStart = Cp;

			if (theApp.m_LastSettings.nNormalizeYtip != 1) {
				m_nSubThreadAction = 999;
				return;
			}

			theApp.m_Kinematics.D3DXRotateMatrixFromNormal(matWorld, CpStart.Side[nSide].norm);

			m_ToolPath.Zero();
			for (ii = 0; ii <= NUMBER_ARC_POINTS; ii++) {
				vNorm = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
				fAngle = ((theApp.m_LastSettings.fNormalizeAngle * 2.0f * (float)ii) / (float)NUMBER_ARC_POINTS) - theApp.m_LastSettings.fNormalizeAngle;
				D3DXMatrixRotationY(&matRot, fAngle * DEG_TO_RAD);
				D3DXVec3TransformCoord(&vNorm, &vNorm, &matRot);
				D3DXVec3TransformCoord(&vNorm, &vNorm, &matWorld);
				CopyMemory(Cp.Side[nSide].norm, vNorm, sizeof D3DXVECTOR3);

				m_ToolPath.Add(Cp);
				if (ii == 0) CpNorm[0] = Cp;
				if (ii == NUMBER_ARC_POINTS) CpNorm[1] = Cp;
			}
			if (fabsf(CpNorm[1].Side[nSide].fI - CpNorm[0].Side[nSide].fI) > fabsf(CpNorm[1].Side[nSide].fJ - CpNorm[0].Side[nSide].fJ)) {
				if (fabsf(CpNorm[1].Side[nSide].fI - CpNorm[0].Side[nSide].fI) > fabsf(CpNorm[1].Side[nSide].fK - CpNorm[0].Side[nSide].fK)) {
					m_ToolPath.SetPrimaryAxis(6);
				}
				else {
					m_ToolPath.SetPrimaryAxis(8);
				}
			}
			else {
				if (fabsf(CpNorm[1].Side[nSide].fJ - CpNorm[0].Side[nSide].fJ) > fabsf(CpNorm[1].Side[nSide].fK - CpNorm[0].Side[nSide].fK)) {
					m_ToolPath.SetPrimaryAxis(7);
				}
				else {
					m_ToolPath.SetPrimaryAxis(8);
				}
			}
			if (nSide == STARBOARD) {
				m_ToolPath.SetPrimaryAxis(m_ToolPath.m_nPrimaryAxis + 10);
			}
			m_CpNormal.AllocateCoords(100);
			m_CpNormal.ClearMemory();
			m_CpNormal.m_nPrimaryAxis = m_ToolPath.m_nPrimaryAxis;
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[NUMBER_ARC_POINTS];

			Line.Zero();
			Line.Add(CpNorm[0]);
			theApp.m_FBCtrl.DoPolyLine(&Line, theApp.m_LastSettings.fSpeedNormalize[0], theApp.m_LastSettings.fSpeedNormalize[0]);
			m_nSubThreadAction += 10;
		}
		break;
	case 70:
		if (theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction += 10;
		}
		break;

	case 80:
		if (theApp.m_FBCtrl.IsMotionFinished() == true) {
			theApp.m_bCollectNormalizeData = TRUE;
			theApp.m_FBCtrl.DoPolyLine(&m_ToolPath, theApp.m_LastSettings.fSpeedNormalize[0], theApp.m_LastSettings.fSpeedNormalize[0]);
			m_nSubThreadAction += 10;
		}
		break;
	case 90:
		if (theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction += 10;
		}
		break;

	case 100:
		if (theApp.m_FBCtrl.IsMotionFinished() == true) {
			theApp.m_bCollectNormalizeData = FALSE;
			m_nNormPlotDirection = VERTICAL;
			m_CpNormal.FindPeakAmplitude(&CpPeek, theApp.m_LastSettings.nPeakMode[0]);

			CpStart.Side[nSide].norm = CpPeek.Side[nSide].norm;

			Line.Zero();
			Line.Add(CpStart);
			theApp.m_FBCtrl.DoPolyLine(&Line, theApp.m_LastSettings.fSpeedNormalize[0], theApp.m_LastSettings.fSpeedNormalize[0]);
			NormalizeViewInvalidate(m_nNormPlotDirection, CpStart);
			m_nSubThreadAction += 10;
		}
		break;
	case 110:
		if (theApp.m_FBCtrl.IsMotionStarted() == true) {
			m_nSubThreadAction = 999;
		}
		break;

	case 999:
		if (theApp.m_FBCtrl.IsMotionFinished() == true) {
			m_nNormalizeStatus = 999;
			m_nThreadAction = WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
		}
		break;
	}

}


void CMainThread::NormalizeViewInvalidate(int nDirection, CCoord Cp)
{
	FRAME;

	if (pFrame->m_pMotionToolsSheet) {
		if (pFrame->m_pMotionToolsSheet->m_pNormalizePage) {
			if (pFrame->m_pMotionToolsSheet->m_pNormalizePage->GetSafeHwnd()) {
				pFrame->m_pMotionToolsSheet->m_pNormalizePage->TransferData(nDirection, &m_CpNormal, Cp);
				pFrame->m_pMotionToolsSheet->m_pNormalizePage->Invalidate(FALSE);
			}
		}
	}

	OptimizePaneMessage Msg;

	Msg.nDirection = nDirection;
	Msg.pLine = &m_CpNormal;
	Msg.pCp = &Cp;
	pFrame->SendMessage(UI_INVALIDATE_OPTIMIZE_PANE, (WPARAM)&Msg, NULL);
}

void CMainThread::PeakOppositeKinematic()
{
	CCoord	Cp, CpSurface;
	CMainFrame*  pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	int nNotSide, nSide;
	int	nYAxis, nZAxis, nXAxis;
	float fDistance = theApp.m_LastSettings.fPeakDistance;
	static CCoord CpStart, CpPeek;
	CPolyCoord Line;

	float fWidth = -1.0f;
	float fHeight = -1.0f;
	fWidth = theApp.m_LastSettings.fPeakDistance;
	fHeight = theApp.m_LastSettings.fPeakDistance;

	D3DXMATRIXA16 matWorld, matNormal, matTranslation;
	D3DXVECTOR3 vecP, vPos;
	D3DXQUATERNION quatPos;

	nSide = theApp.m_PmacUser.m_nScanSide = m_nNormSide;
	nSide == PORTSIDE ? nNotSide = STARBOARD : nNotSide = PORTSIDE;

	nXAxis = theApp.m_Tank.nXRight;
	nYAxis = theApp.m_Tank.nYRight;
	nZAxis = theApp.m_Tank.nZRight;


	switch (m_nSubThreadAction) {
	case 0:
		m_nPeakOppositeStatus = 1;
		theApp.m_Motors.GetSurfacePos(&Cp);
		CpStart = Cp;

		theApp.m_Kinematics.D3DXRotateMatrixFromNormal(matNormal, CpStart.Side[nSide].norm);
		D3DXMatrixTranslation(&matTranslation, Cp.Side[nSide].fX, Cp.Side[nSide].fY, Cp.Side[nSide].fZ);
		D3DXMatrixMultiply(&matWorld, &matNormal, &matTranslation);

		m_CpRect[0] = Cp;
		vecP.x = 0.0f;
		vecP.y = fHeight / -2.0f;
		vecP.z = 0.0f;
		D3DXVec3TransformCoord(&vecP, &vecP, &matWorld);
		m_CpRect[0].Side[nSide].fX = vecP.x;
		m_CpRect[0].Side[nSide].fY = vecP.y;
		m_CpRect[0].Side[nSide].fZ = vecP.z;

		m_CpRect[1] = Cp;
		vecP.x = 0.0f;
		vecP.y = fHeight / 2.0f;
		vecP.z = 0.0f;
		D3DXVec3TransformCoord(&vecP, &vecP, &matWorld);
		m_CpRect[1].Side[nSide].fX = vecP.x;
		m_CpRect[1].Side[nSide].fY = vecP.y;
		m_CpRect[1].Side[nSide].fZ = vecP.z;

		m_ToolPath.Zero();
		m_ToolPath.Add(m_CpRect[0]);
		m_ToolPath.Add(m_CpRect[1]);

		m_CpNormal.AllocateCoords(100);
		m_CpNormal.ClearMemory();
		if (nSide == STARBOARD) {
			if (fabs(m_CpRect[1].Side1.fZ - m_CpRect[0].Side1.fZ) > fabs(m_CpRect[1].Side1.fY - m_CpRect[0].Side1.fY)) {
				m_ToolPath.SetPrimaryAxis(12);	//zRight
				m_CpNormal.SetPrimaryAxis(12);
			}
			else {
				m_ToolPath.SetPrimaryAxis(11);	//yRight
				m_CpNormal.SetPrimaryAxis(11);
			}
		}
		else {
			if (fabs(m_CpRect[1].Side0.fZ - m_CpRect[0].Side0.fZ) > fabs(m_CpRect[1].Side0.fY - m_CpRect[0].Side0.fY)) {
				m_ToolPath.SetPrimaryAxis(2);	//zLeft
				m_CpNormal.SetPrimaryAxis(2);
			}
			else {
				m_ToolPath.SetPrimaryAxis(1);	//yLeft
				m_CpNormal.SetPrimaryAxis(1);
			}
		}
		m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
		m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[1];
		theApp.m_bCollectNormalizeData = TRUE;
		theApp.m_FBCtrl.DoPolyLine(&m_ToolPath, theApp.m_LastSettings.fPeekOrthogonalSpeed, theApp.m_LastSettings.fPeekOrthogonalSpeed);

		m_nSubThreadAction = 5;
		break;
	case 5:
		if (theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 10;
		}
		break;
	case 10:
		if (theApp.m_FBCtrl.IsMotionFinished() == true) {
			theApp.m_bCollectNormalizeData = FALSE;
			m_CpNormal.FindPeakAmplitude(&CpPeek, theApp.m_LastSettings.nPeakMode[0]);

			NormalizeViewInvalidate(m_nNormPlotDirection = VERTICAL, m_CpMoveTo);

			CpStart.SetSide(CpPeek, nSide);

			Cp = CpStart;

			theApp.m_Kinematics.D3DXRotateMatrixFromNormal(matNormal, CpStart.Side[nSide].norm);
			D3DXMatrixTranslation(&matTranslation, Cp.Side[nSide].fX, Cp.Side[nSide].fY, Cp.Side[nSide].fZ);
			D3DXMatrixMultiply(&matWorld, &matNormal, &matTranslation);

			m_CpRect[0] = Cp;
			vecP.x = fHeight / -2.0f;
			vecP.y = 0.0f;
			vecP.z = 0.0f;
			D3DXVec3TransformCoord(&vecP, &vecP, &matWorld);
			m_CpRect[0].Side[nSide].fX = vecP.x;
			m_CpRect[0].Side[nSide].fY = vecP.y;
			m_CpRect[0].Side[nSide].fZ = vecP.z;

			m_CpRect[1] = Cp;
			vecP.x = fHeight / 2.0f;
			vecP.y = 0.0f;
			vecP.z = 0.0f;
			D3DXVec3TransformCoord(&vecP, &vecP, &matWorld);
			m_CpRect[1].Side[nSide].fX = vecP.x;
			m_CpRect[1].Side[nSide].fY = vecP.y;
			m_CpRect[1].Side[nSide].fZ = vecP.z;

			m_ToolPath.Zero();
			m_ToolPath.Add(m_CpRect[0]);
			m_ToolPath.Add(m_CpRect[1]);

			m_CpNormal.AllocateCoords(100);
			m_CpNormal.ClearMemory();
			if (nSide == STARBOARD) {
				if (fabs(m_CpRect[1].Side1.fX - m_CpRect[0].Side1.fX) > fabs(m_CpRect[1].Side1.fY - m_CpRect[0].Side1.fY)) {
					m_ToolPath.SetPrimaryAxis(10);	//xRight
					m_CpNormal.SetPrimaryAxis(10);
				}
				else {
					m_ToolPath.SetPrimaryAxis(11);	//yRight
					m_CpNormal.SetPrimaryAxis(11);
				}
			}
			else {
				if (fabs(m_CpRect[1].Side0.fX - m_CpRect[0].Side0.fX) > fabs(m_CpRect[1].Side0.fY - m_CpRect[0].Side0.fY)) {
					m_ToolPath.SetPrimaryAxis(0);	//xLeft
					m_CpNormal.SetPrimaryAxis(0);
				}
				else {
					m_ToolPath.SetPrimaryAxis(1);	//yLeft
					m_CpNormal.SetPrimaryAxis(1);
				}
			}
			m_CpNormal.m_pCp[0] = m_ToolPath.m_pCp[0];
			m_CpNormal.m_pCp[99] = m_ToolPath.m_pCp[1];
			theApp.m_bCollectNormalizeData = TRUE;
			theApp.m_FBCtrl.DoPolyLine(&m_ToolPath, theApp.m_LastSettings.fPeekOrthogonalSpeed, theApp.m_LastSettings.fPeekOrthogonalSpeed);

			m_nSubThreadAction = 25;
		}
		break;
	case 25:
		if (theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 30;
		}
		break;
	case 30:
		if (theApp.m_FBCtrl.IsMotionFinished() == true) {
			theApp.m_bCollectNormalizeData = FALSE;
			m_CpNormal.FindPeakAmplitude(&CpPeek, theApp.m_LastSettings.nPeakMode[0]);

			NormalizeViewInvalidate(m_nNormPlotDirection = HORIZONTAL, m_CpMoveTo);

			CpStart.SetSide(CpPeek, nSide);

			Line.Zero();
			Line.Add(CpStart);
			theApp.m_FBCtrl.DoPolyLine(&Line, theApp.m_LastSettings.fPeekOrthogonalSpeed, theApp.m_LastSettings.fPeekOrthogonalSpeed);

			m_nSubThreadAction = 995;
		}
		break;
	case 995:
		if (theApp.m_Motors.IsMotionStarted() == TRUE) {
			m_nSubThreadAction = 999;
		}
		break;
	case 999:
		if (theApp.m_FBCtrl.IsMotionFinished() == true) {
			m_nPeakOppositeStatus = 999;
			m_nThreadAction = WAIT_FOR_MOVE_FINISHED;			//Finished as a stand alone subthread
		}
		break;
	}


}
