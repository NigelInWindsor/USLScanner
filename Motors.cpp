// Motors.cpp: implementation of the CMotors class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "Motors.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMotors::CMotors()
{

}

CMotors::~CMotors()
{

}

void CMotors::GetHeadPos(COORDDATA *pCp)
{
	float fXBridge;

	theApp.m_Ethercat.InvalidateRobotJoints();

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		pCp->Side0.fX=(float)theApp.m_Axes[theApp.m_Tank.nXLeft].nPos*theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize;
		pCp->Side0.fY=(float)theApp.m_Axes[theApp.m_Tank.nYLeft].nPos*theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize;
		pCp->Side0.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZLeft].nPos*theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize;
		pCp->Side0.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos*theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize;
		pCp->Side0.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos*theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize;
		pCp->Side0.fR=(float)theApp.m_Axes[theApp.m_Tank.nRLeft].nPos*theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize;

		pCp->Side1.fX=(float)theApp.m_Axes[theApp.m_Tank.nXRight].nPos*theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize;
		pCp->Side1.fY=(float)theApp.m_Axes[theApp.m_Tank.nYRight].nPos*theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize;
		pCp->Side1.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZRight].nPos*theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize;
		pCp->Side1.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtRight].nPos*theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize;
		pCp->Side1.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtRight].nPos*theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize;
		pCp->Side1.fR=(float)theApp.m_Axes[theApp.m_Tank.nRRight].nPos*theApp.m_Axes[theApp.m_Tank.nRRight].fStepSize;
		break;
	case SINGLE_BRIDGE_DUAL_SIDED:
	case DUAL_BRIDGE_DUAL_SIDED:
		fXBridge = (float)theApp.m_Axes[theApp.m_Tank.nXBridge].nPos*theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize;
		pCp->Side0.fX=(float)theApp.m_Axes[theApp.m_Tank.nXLeft].nPos*theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize + fXBridge;
		pCp->Side0.fY=(float)theApp.m_Axes[theApp.m_Tank.nYLeft].nPos*theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize;
		pCp->Side0.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZLeft].nPos*theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize;
		pCp->Side0.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos*theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize;
		pCp->Side0.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos*theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize;
		pCp->Side0.fR=(float)theApp.m_Axes[theApp.m_Tank.nRLeft].nPos*theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize;

		pCp->Side1.fX=(float)theApp.m_Axes[theApp.m_Tank.nXRight].nPos*theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize + fXBridge;
		pCp->Side1.fY=(float)theApp.m_Axes[theApp.m_Tank.nYRight].nPos*theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize;
		pCp->Side1.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZRight].nPos*theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize;
		pCp->Side1.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtRight].nPos*theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize;
		pCp->Side1.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtRight].nPos*theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize;
		pCp->Side1.fR=(float)theApp.m_Axes[theApp.m_Tank.nRRight].nPos*theApp.m_Axes[theApp.m_Tank.nRRight].fStepSize;
		break;
	case FB_TWIN_TOWER:
		theApp.m_Tank.nFingerLeft = 10;
		switch(theApp.m_LastSettings.nManipulatorType[0]) {
		case HORIZONTAL_WRIST:
			pCp->Side0.fX=(float)theApp.m_Axes[theApp.m_Tank.nXLeft].nPos*theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize;
			pCp->Side0.fY=(float)theApp.m_Axes[theApp.m_Tank.nYLeft].nPos*theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize;
			pCp->Side0.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZLeft].nPos*theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize;
			pCp->Side0.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos*theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize;
			pCp->Side0.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos*theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize;
			pCp->Side0.fR=(float)theApp.m_Axes[theApp.m_Tank.nRLeft].nPos*theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize;
			break;
		case TWIN_TOWER_FINGER:
			pCp->Side0.fX=(float)theApp.m_Axes[theApp.m_Tank.nXLeft].nPos*theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize;
			pCp->Side0.fY=(float)theApp.m_Axes[theApp.m_Tank.nYLeft].nPos*theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize;
			pCp->Side0.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZLeft].nPos*theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize;
			pCp->Side0.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos*theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize;
			pCp->Side0.fYt=(float)theApp.m_Axes[theApp.m_Tank.nFingerLeft].nPos*theApp.m_Axes[theApp.m_Tank.nFingerLeft].fStepSize;
			pCp->Side0.fR=(float)theApp.m_Axes[theApp.m_Tank.nRLeft].nPos*theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize;
			break;
		}
		pCp->Side1.fX=(float)theApp.m_Axes[theApp.m_Tank.nXRight].nPos*theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize;
		pCp->Side1.fY=(float)theApp.m_Axes[theApp.m_Tank.nYRight].nPos*theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize;
		pCp->Side1.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZRight].nPos*theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize;
		pCp->Side1.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtRight].nPos*theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize;
		pCp->Side1.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtRight].nPos*theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize;
		pCp->Side1.fR=(float)theApp.m_Axes[theApp.m_Tank.nRRight].nPos*theApp.m_Axes[theApp.m_Tank.nRRight].fStepSize;
		break;
	}

	if(theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable == 0) pCp->Side0.fXt = 0.0f;
	if(theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable == 0) pCp->Side0.fYt = 0.0f;
	if(theApp.m_Axes[theApp.m_Tank.nXtRight].bAvailable == 0) pCp->Side1.fXt = 0.0f;
	if(theApp.m_Axes[theApp.m_Tank.nYtRight].bAvailable == 0) pCp->Side1.fYt = 0.0f;
}

void CMotors::GetTipAngles(int nSide, float *pfXt, float *pfYt)
{
	switch(nSide) {
	case PORTSIDE:
		*pfXt = (float)theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos*theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize;
		*pfYt = (float)theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos*theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize;
		break;
	case STARBOARD:
		*pfXt = (float)theApp.m_Axes[theApp.m_Tank.nXtRight].nPos*theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize;
		*pfYt = (float)theApp.m_Axes[theApp.m_Tank.nYtRight].nPos*theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize;
		break;
	}
}

void CMotors::GetMachinePos(COORDDATA *pCp)
{
	theApp.m_Ethercat.InvalidateRobotJoints();

	pCp->Side0.fX=(float)(theApp.m_Axes[theApp.m_Tank.nXLeft].nPos+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset)*theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize;
	pCp->Side0.fY=(float)(theApp.m_Axes[theApp.m_Tank.nYLeft].nPos+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset)*theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize;
	pCp->Side0.fZ=(float)(theApp.m_Axes[theApp.m_Tank.nZLeft].nPos+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset)*theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize;
	pCp->Side0.fXt=(float)(theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset)*theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize;
	pCp->Side0.fYt=(float)(theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset)*theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize;
	pCp->Side0.fR=(float)(theApp.m_Axes[theApp.m_Tank.nRLeft].nPos+theApp.m_Axes[theApp.m_Tank.nRLeft].nOffset)*theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize;

	pCp->Side1.fX=(float)(theApp.m_Axes[theApp.m_Tank.nXRight].nPos+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset)*theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize;
	pCp->Side1.fY=(float)(theApp.m_Axes[theApp.m_Tank.nYRight].nPos+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset)*theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize;
	pCp->Side1.fZ=(float)(theApp.m_Axes[theApp.m_Tank.nZRight].nPos+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset)*theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize;
	pCp->Side1.fXt=(float)(theApp.m_Axes[theApp.m_Tank.nXtRight].nPos+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset)*theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize;
	pCp->Side1.fYt=(float)(theApp.m_Axes[theApp.m_Tank.nYtRight].nPos+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset)*theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize;
	pCp->Side1.fR=0.0f;
}

void CMotors::GetEncoderPos(CCoord *pCp)
{
	theApp.m_Ethercat.InvalidateRobotJoints();

	pCp->Side0.fX=(float)(theApp.m_Axes[theApp.m_Tank.nXLeft].nPos+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset)*theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize;
	pCp->Side0.fY=(float)(theApp.m_Axes[theApp.m_Tank.nYLeft].nPos+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset)*theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize;
	pCp->Side0.fZ=(float)(theApp.m_Axes[theApp.m_Tank.nZLeft].nPos+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset)*theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize;
	pCp->Side0.fXt=(float)(theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset)*theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize;
	pCp->Side0.fYt=(float)(theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset)*theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize;
	pCp->Side0.fR=(float)(theApp.m_Axes[theApp.m_Tank.nRLeft].nPos+theApp.m_Axes[theApp.m_Tank.nRLeft].nOffset)*theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize;

	pCp->Side1.fX=(float)(theApp.m_Axes[theApp.m_Tank.nXRight].nPos+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset)*theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize;
	pCp->Side1.fY=(float)(theApp.m_Axes[theApp.m_Tank.nYRight].nPos+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset)*theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize;
	pCp->Side1.fZ=(float)(theApp.m_Axes[theApp.m_Tank.nZRight].nPos+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset)*theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize;
	pCp->Side1.fXt=(float)(theApp.m_Axes[theApp.m_Tank.nXtRight].nPos+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset)*theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize;
	pCp->Side1.fYt=(float)(theApp.m_Axes[theApp.m_Tank.nYtRight].nPos+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset)*theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize;

}


void CMotors::GetSurfacePos(CCoord *pCpSurface,int nFL)
{
	D3DXMATRIXA16 matBase;
	CCoord CpHead;

	CpHead=*pCpSurface;
	GetHeadPos(&CpHead);

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_LastSettings.nPositionDisplayMode) {
		default:
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,pCpSurface,PORTSIDE,TRUE,TRUE, nFL);
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,pCpSurface,STARBOARD,TRUE,TRUE, nFL);
			break;
		case 3:
			theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
			theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,pCpSurface,PORTSIDE,TRUE,FALSE, nFL);
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,pCpSurface,STARBOARD,TRUE,FALSE, nFL);
			break;
		}
		theApp.m_Kinematics.NormalsFromTips(pCpSurface,PORTSIDE);
		theApp.m_Kinematics.NormalsFromTips(pCpSurface,STARBOARD);
		pCpSurface->Side[0].pt += theApp.m_Tank.vFrameZero[0];
		pCpSurface->Side[1].pt += theApp.m_Tank.vFrameZero[1];
		switch(theApp.m_LastSettings.nRotateSurfaceCoordinates) {
		case 0:
			break;
		case 1:
			theApp.m_Kinematics.CalculateRandP(pCpSurface,PORTSIDE,0.0f);
			theApp.m_Kinematics.CalculateRandP(pCpSurface,STARBOARD,0.0f);
			theApp.m_Kinematics.TipsFromNormals(pCpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(pCpSurface,STARBOARD);
			break;
		case 2:
			theApp.m_Kinematics.CalculateRandP(pCpSurface,PORTSIDE,180.0f);
			theApp.m_Kinematics.CalculateRandP(pCpSurface,STARBOARD,180.0f);
			theApp.m_Kinematics.TipsFromNormals(pCpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(pCpSurface,STARBOARD);
			break;
		};
		return;
		break;
	case TWIN_TOWER_KINEMATIC:
		theApp.m_Kinematics.SurfaceFromHead(&CpHead, pCpSurface, PORTSIDE, TRUE, TRUE, nFL);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead, pCpSurface, STARBOARD, TRUE, TRUE, nFL);
		theApp.m_Kinematics.NormalsFromTips(pCpSurface, PORTSIDE);
		theApp.m_Kinematics.NormalsFromTips(pCpSurface, STARBOARD);
		pCpSurface->RotateCoordinate(PORTSIDE, 0.0f, 0.0f, 0.0f);
		pCpSurface->RotateCoordinate(STARBOARD, 0.0f, 0.0f, 180.0f);
		pCpSurface->InvertNorm(PORTSIDE);
		pCpSurface->InvertNorm(STARBOARD);
		pCpSurface->Side[0].pt += theApp.m_Tank.vFrameZero[0];
		pCpSurface->Side[1].pt += theApp.m_Tank.vFrameZero[1];
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
		theApp.m_Robot[0].GetToolPos(pCpSurface,PORTSIDE);
		theApp.m_Robot[1].GetToolPos(pCpSurface,STARBOARD);
		pCpSurface->RotateCoordinate(PORTSIDE, 0.0f, 0.0f, 0.0f);
		pCpSurface->RotateCoordinate(STARBOARD, 0.0f, 0.0f, 180.0f);
		pCpSurface->InvertNorm(PORTSIDE);
		pCpSurface->InvertNorm(STARBOARD);
		pCpSurface->Side[0].pt += theApp.m_Tank.vFrameZero[0];
		pCpSurface->Side[1].pt += theApp.m_Tank.vFrameZero[1];
		break;
	case DUAL_ROBOT_9_PLUS_9:
		theApp.m_Robot[0].GetToolPos(pCpSurface,PORTSIDE);
		theApp.m_Robot[1].GetToolPos(pCpSurface,STARBOARD);
		pCpSurface->Side[0].fX += ((float)theApp.m_Axes[0].nPos * theApp.m_Axes[0].fStepSize);
		pCpSurface->Side[0].fY += ((float)theApp.m_Axes[1].nPos * theApp.m_Axes[1].fStepSize);
		pCpSurface->Side[0].fZ += ((float)theApp.m_Axes[2].nPos * theApp.m_Axes[2].fStepSize);
		pCpSurface->Side[1].fX += ((float)theApp.m_Axes[10].nPos * theApp.m_Axes[10].fStepSize);
		pCpSurface->Side[1].fY -= ((float)theApp.m_Axes[11].nPos * theApp.m_Axes[11].fStepSize);
		pCpSurface->Side[1].fZ += ((float)theApp.m_Axes[12].nPos * theApp.m_Axes[12].fStepSize);
		pCpSurface->InvertNorm(PORTSIDE);
		pCpSurface->InvertNorm(STARBOARD);
		pCpSurface->Side[0].pt += theApp.m_Tank.vFrameZero[0];
		pCpSurface->Side[1].pt += theApp.m_Tank.vFrameZero[1];
		break;
	case SPX_ROBOT:
		theApp.m_Robot[0].GetToolPos(pCpSurface,PORTSIDE);
		theApp.m_Robot[1].GetToolPos(pCpSurface,STARBOARD);
		pCpSurface->Side[0].fX = pCpSurface->Side[0].fX + ((float)theApp.m_Axes[1].nPos * theApp.m_Axes[1].fStepSize);
		pCpSurface->Side[0].fZ = pCpSurface->Side[0].fZ + ((float)theApp.m_Axes[2].nPos * theApp.m_Axes[2].fStepSize);
		pCpSurface->Side[1].fZ = ((float)theApp.m_Axes[12].nPos * theApp.m_Axes[12].fStepSize) + pCpSurface->Side[1].fZ;
		theApp.InvertNormScannerSpecific(pCpSurface,PORTSIDE);
		pCpSurface->Side[0].fR = CpHead.Side[0].fR;
		break;
	case TRACKED_PROBE:
		if (theApp.m_pTracker) {
			theApp.m_pTracker->GetSurfacePos(pCpSurface);
		}
	}
	if(theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
		pCpSurface->Side0.pt += theApp.m_PhasedArray[0].m_FL[CURRENT_FL].vecPt;
	}

}



void CMotors::OriginFindAxis(int nWhichAxis)
{
	if(theApp.m_Axes[nWhichAxis].nMotorType == SERVO) {
		theApp.m_PmacUser.OriginFindAxis(nWhichAxis);
	} else {
		switch(theApp.m_nMotorType) {
		case SI6:
			theApp.m_Si6User.EnableAxis(nWhichAxis,HOMETO);
			break;
		case SI10:
			theApp.m_Si10User.EnableAxis(nWhichAxis,HOMETO);
			break;
		}
	}

}

void CMotors::OriginAll(int nThreadAction, int nAxesMask)
{
	int nAxis;

	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:	theApp.m_PmacUser.OriginAll(nThreadAction);
		break;
	case SI6:
		break;
	case SI10:
		for(nAxis=0;nAxis<10;nAxis++) {
			if((theApp.m_Axes[nAxis].nMotorType == STEPPER) && (theApp.m_Axes[nAxis].bAvailable == TRUE)) {
				theApp.m_Si10User.EnableAxis(nAxis,HOMETO);
			}
		}
		break;
	case FIELD_BUS:
		theApp.m_FBCtrl.Origin(nAxesMask);
		break;
	}


}


void CMotors::CalculateRollerStepSize()
{
//	theApp.m_Axes[X1AXIS].fStepSize = ( theApp.m_ScanData.m_fComponentDiameter[0] / theApp.m_LastSettings.fRollerDiameter) / (float)theApp.m_LastSettings.nRollerStepsPerRev;
	theApp.m_Axes[X1AXIS].fStepSize = ( theApp.m_LastSettings.fRollerDiameter / theApp.m_ScanData.m_fComponentDiameter[0]) * (float)theApp.m_LastSettings.fRollerStepSize;
}

void CMotors::ZeroAxis(int nWhich)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: theApp.m_ServoUser.SuspendJoystick(1);
		theApp.m_Axes[nWhich].nOffset=0;
		theApp.m_PmacUser.HomeAxisWithoutMoving(theApp.m_Axes[nWhich].nPhysicalAxis);
		Sleep(100);
		theApp.m_Axes[nWhich].nOffset=theApp.m_Axes[nWhich].nPos;
		break;
	case SI6:
		theApp.m_Si6User.ZeroPosition(nWhich);
		break;
	case SI10:
		theApp.m_Si10User.ZeroPosition(nWhich);
		break;
	case FIELD_BUS:
		theApp.m_Axes[nWhich].nOffset=0;
		theApp.m_FBCtrl.HomeAxisWithoutMoving(theApp.m_Axes[nWhich].nPhysicalAxis,0);
		Sleep(100);
		theApp.m_Axes[nWhich].nOffset=theApp.m_Axes[nWhich].nPos;
		break;
	}
}

void CMotors::GetHoldingCoord(HoldingFixtureCoord *pCp)
{
	pCp->fXMoving = (float)theApp.m_Axes[theApp.m_Tank.nXStep].nPos * theApp.m_Axes[theApp.m_Tank.nXStep].fStepSize;
	pCp->fYMoving = (float)theApp.m_Axes[theApp.m_Tank.nYStep].nPos * theApp.m_Axes[theApp.m_Tank.nYStep].fStepSize;
	pCp->fZMoving = (float)theApp.m_Axes[theApp.m_Tank.nZStep].nPos * theApp.m_Axes[theApp.m_Tank.nZStep].fStepSize;
	pCp->fYFixed = (float)theApp.m_Axes[theApp.m_Tank.nXtStep].nPos * theApp.m_Axes[theApp.m_Tank.nXtStep].fStepSize;
	pCp->fZFixed = (float)theApp.m_Axes[theApp.m_Tank.nYtStep].nPos * theApp.m_Axes[theApp.m_Tank.nYtStep].fStepSize;

	pCp->fXFixed = (float)theApp.m_Axes[theApp.m_Tank.nRStep].nPos * theApp.m_Axes[theApp.m_Tank.nRStep].fStepSize;
}

void CMotors::SuspendMotors()
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:	theApp.m_PmacUser.SuspendMotors();
		break;
	case SI6:
		break;
	case SI10: theApp.m_Si10User.StopAllMotorsDead();
		break;
	case FIELD_BUS: theApp.m_FBCtrl.KillAllMotionPrograms();
		break;
	}


}

void CMotors::EnableAxes(int nAction)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:	theApp.m_PmacUser.EnableAxes(nAction);
		break;
	case SI6:
		break;
	case SI10: theApp.m_Si10User.EnableAxes(nAction);
		break;
	case FIELD_BUS: theApp.m_FBCtrl.EnableAxes(nAction);
		break;
	}

}

void CMotors::DesirePosEqualsCoord(CCoord Cp, int nAction)
{
	float fXBridge;

	//Todo in here need a switch that decide if we are doing a tuning move not to incorporate the Xleft and Bridge together

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
		theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
		theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
		theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nDesirePos=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
		theApp.m_Axes[theApp.m_Tank.nRLeft].nDesirePos=(int)(Cp.Side0.fR/theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize);

		theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
		theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
		theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
		theApp.m_Axes[theApp.m_Tank.nXtRight].nDesirePos=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
		theApp.m_Axes[theApp.m_Tank.nYtRight].nDesirePos=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
		theApp.m_Axes[theApp.m_Tank.nRRight].nDesirePos=(int)(Cp.Side1.fR/theApp.m_Axes[theApp.m_Tank.nRRight].fStepSize);
		break;
	case FB_TWIN_TOWER:
		theApp.m_Tank.nFingerLeft = 10;
		switch(theApp.m_LastSettings.nManipulatorType[0]) {
		case HORIZONTAL_WRIST:
			theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nDesirePos=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
			break;
		case TWIN_TOWER_FINGER:
			theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nDesirePos=0;
			theApp.m_Axes[theApp.m_Tank.nFingerLeft].nDesirePos=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nFingerLeft].fStepSize);
			break;
		}
		theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
		theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
		theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
		theApp.m_Axes[theApp.m_Tank.nXtRight].nDesirePos=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
		theApp.m_Axes[theApp.m_Tank.nYtRight].nDesirePos=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
		break;
	case SINGLE_BRIDGE_DUAL_SIDED:
		if(theApp.m_LastSettings.bEnableSide[0] == theApp.m_LastSettings.bEnableSide[1] == 1) {
			
			switch(nAction){
			default:
				if((theApp.m_LastSettings.nManipulatorType[0] == INDEX_FINGER) || (theApp.m_LastSettings.nManipulatorType[1] == INDEX_FINGER)) {
					fXBridge = ((Cp.Side0.fX + Cp.Side1.fX) / 2.0f) + (theApp.m_Kinematics.m_fXOffset[PORTSIDE] / 3.0f);
				} else {
					fXBridge = ((Cp.Side0.fX + Cp.Side1.fX) / 2.0f) + theApp.m_Kinematics.m_fXOffset[PORTSIDE];
				}
				theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)(fXBridge/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)((Cp.Side0.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos=(int)((Cp.Side1.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
				break;
			case TUNNING:
				if(theApp.m_PmacUser.m_nTunningAxis==0) {
					theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize); //theApp.m_Axes[theApp.m_Tank.nXBridge].nPos; //TODO
				}else{ 
					theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
				}
				theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
				break;
			case PEAK_OPTOMISE:
				fXBridge=(float)theApp.m_Axes[theApp.m_Tank.nXBridge].nPos*theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize;
				theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)(fXBridge/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)((Cp.Side0.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos=(int)((Cp.Side1.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
				break;
			}

			
			
			theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nDesirePos=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nRLeft].nDesirePos=(int)(Cp.Side0.fR/theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize);

			
			theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nXtRight].nDesirePos=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nYtRight].nDesirePos=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
			theApp.m_Axes[theApp.m_Tank.nRRight].nDesirePos=(int)(Cp.Side1.fR/theApp.m_Axes[theApp.m_Tank.nRRight].fStepSize);
		} else {
			if(theApp.m_LastSettings.bEnableSide[0] == 1) {
				theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)((Cp.Side0.fX+theApp.m_Kinematics.m_fXOffset[PORTSIDE])/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)(-theApp.m_Kinematics.m_fXOffset[PORTSIDE]/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nYtLeft].nDesirePos=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nRLeft].nDesirePos=(int)(Cp.Side0.fR/theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize);

				theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos=theApp.m_Axes[theApp.m_Tank.nXRight].nPos;
				theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos=theApp.m_Axes[theApp.m_Tank.nYRight].nPos;
				theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos=theApp.m_Axes[theApp.m_Tank.nZRight].nPos;
				theApp.m_Axes[theApp.m_Tank.nXtRight].nDesirePos=theApp.m_Axes[theApp.m_Tank.nXtRight].nPos;
				theApp.m_Axes[theApp.m_Tank.nYtRight].nDesirePos=theApp.m_Axes[theApp.m_Tank.nYtRight].nPos;
				theApp.m_Axes[theApp.m_Tank.nRRight].nDesirePos=theApp.m_Axes[theApp.m_Tank.nRRight].nPos;
			};
			if(theApp.m_LastSettings.bEnableSide[1] == 1) {
				theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)((Cp.Side1.fX+theApp.m_Kinematics.m_fXOffset[STARBOARD])/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);

				theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=theApp.m_Axes[theApp.m_Tank.nXLeft].nPos;
				theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos=theApp.m_Axes[theApp.m_Tank.nYLeft].nPos;
				theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos=theApp.m_Axes[theApp.m_Tank.nZLeft].nPos;
				theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos=theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos;
				theApp.m_Axes[theApp.m_Tank.nYtLeft].nDesirePos=theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos;
				theApp.m_Axes[theApp.m_Tank.nRLeft].nDesirePos=theApp.m_Axes[theApp.m_Tank.nRLeft].nPos;

				theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos=(int)(-theApp.m_Kinematics.m_fXOffset[STARBOARD]/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nXtRight].nDesirePos=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nYtRight].nDesirePos=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nRRight].nDesirePos=(int)(Cp.Side1.fR/theApp.m_Axes[theApp.m_Tank.nRRight].fStepSize);
			};
		}
		break;
	}
}



void CMotors::DesirePosEqualsHeadCoord(CCoord Cp, int nAction)
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize) - theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset;
		theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize) - theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset;
		theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize) - theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset;
		theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize) - theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset;
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nDesirePos=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize) - theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset;
		theApp.m_Axes[theApp.m_Tank.nRLeft].nDesirePos=(int)(Cp.Side0.fR/theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize) - theApp.m_Axes[theApp.m_Tank.nRLeft].nOffset;

		theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize) - theApp.m_Axes[theApp.m_Tank.nXRight].nOffset;
		theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize) - theApp.m_Axes[theApp.m_Tank.nYRight].nOffset;
		theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize) - theApp.m_Axes[theApp.m_Tank.nZRight].nOffset;
		theApp.m_Axes[theApp.m_Tank.nXtRight].nDesirePos=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize) - theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset;
		theApp.m_Axes[theApp.m_Tank.nYtRight].nDesirePos=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize) - theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset;
		theApp.m_Axes[theApp.m_Tank.nRRight].nDesirePos=(int)(Cp.Side1.fR/theApp.m_Axes[theApp.m_Tank.nRRight].fStepSize) - theApp.m_Axes[theApp.m_Tank.nRRight].nOffset;
		break;
	case 999: // to shut compiler up
		break;
	}
}


void CMotors::GetTurntablePos(CCoord *pCp)
{
	switch(theApp.m_nMotorType) {
	case SI6: theApp.m_Si6User.GetTurntableCoord(pCp);
		break;
	case SERVO_TURBO:
	case SERVO:
	case FIELD_BUS:	pCp->Side0.fR = (float)theApp.m_Axes[theApp.m_Tank.nRLeft].nPos * theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize;
		break;
	}
}


void CMotors::StartContinuousRotate(int nAxis,float fSpeed,int nDir)
{
	switch(theApp.m_nMotorType) {
	case SI6:// theApp.m_Si6User.StartContinuousRotate(nSamplesLine);
		break;
	case SI10:theApp.m_Si10User.StartContinuousRotate(nAxis,fSpeed,nDir);
		break;
	case SERVO_TURBO:
	case SERVO: theApp.m_PmacUser.StartContinuousRotate(nAxis,fSpeed,nDir);
		break;
	case FIELD_BUS: theApp.m_FBCtrl.StartContinuousRotate(nAxis,fSpeed,nDir);
		break;
	}

}

int CMotors::GetRevCount()
{
	switch(theApp.m_nMotorType) {
	case SI6:// theApp.m_Si6User.StartContinuousRotate(nSamplesLine);
		break;
	case SI10:return theApp.m_Si10User.GetRevCounter(theApp.m_Tank.nRLeft);
		break;
	case SERVO_TURBO:
	case SERVO: return theApp.m_Motors.m_nRevCounter;
		break;
	case FIELD_BUS: return theApp.m_Motors.m_nRevCounter;
		break;
	}
	return 0;
}

void CMotors::SetRevCount(int nRev)
{
	switch(theApp.m_nMotorType) {
	case SI6:// theApp.m_Si6User.StartContinuousRotate(nSamplesLine);
		break;
	case SI10: theApp.m_Si10User.SetRevCounter(theApp.m_Tank.nRLeft,nRev);
		break;
	case SERVO_TURBO:
	case SERVO: theApp.m_Motors.m_nRevCounter = nRev;
		break;
	case FIELD_BUS: theApp.m_Motors.m_nRevCounter = nRev;
		break;
	}

}


void CMotors::StartXNumberOfRevs(float fNumber,float fSpeed)
{
	switch(theApp.m_nMotorType) {
	case SI6:
		break;
	case SI10: theApp.m_Si10User.StartXNumberOfRevs(fNumber,fSpeed);
		break;
	case SERVO_TURBO:
	case SERVO: theApp.m_PmacUser.StartXNumberOfRevs(fNumber,fSpeed);
		break;
	}

}


void CMotors::GetRollerPos(COORDDATA *pCp)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:	theApp.m_ServoUser.GetRollerPos(pCp);
		break;
	case SI6: theApp.m_Si6User.GetRollerPos(pCp);
		break;
	case SI10:
		break;
	}

}


bool CMotors::IsMotionFinished()
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: return theApp.m_PmacUser.IsMotionFinished();
		break;
	case SI6:
		break;
	case SI10: return theApp.m_Si10User.IsMotionFinished();
		break;
	case FIELD_BUS:	return theApp.m_FBCtrl.IsMotionFinished();
		break;
	}
	return TRUE;
}

bool CMotors::WaitMotionFinished(double dSeconds)
{
	double dFinishTime = theApp.getHighResTime() + dSeconds;

	while (IsMotionFinished() != true && theApp.getHighResTime() < dFinishTime) {
		theApp.m_Ethercat.InvalidateRobotJoints();
	};

	return IsMotionFinished();
}

bool CMotors::IsAxisInPosition(int nAxis)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: return theApp.m_PmacUser.IsAxisInPosition(nAxis);
		break;
	case SI6:
		break;
	case SI10: return theApp.m_Si10User.IsAxisInPosition(nAxis);
		break;
	case FIELD_BUS: return theApp.m_FBCtrl.IsItAtDesirePos();
		break;
	}
	return TRUE;
}

bool CMotors::IsAxisInMachinePosition(int nAxis, int nDesirePos)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:
		break;
	case SI6:
		break;
	case SI10:
		break;
	case FIELD_BUS: return theApp.m_FBCtrl.IsAxisInMachinePosition(nAxis, nDesirePos);
		break;
	}
	return TRUE;
}

void CMotors::MoveJustR(int nAction)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:	theApp.m_PmacUser.MoveJustR(nAction);
		break;
	case SI6:
		break;
	case SI10: theApp.m_Si10User.MoveJustR(nAction);
		break;
	}

}

bool CMotors::IsMoveRFinished()
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: return theApp.m_PmacUser.IsMoveRFinished();
		break;
	case SI6:
		break;
	case SI10:
		break;
	}
	return TRUE;
}

void CMotors::MoveTo(int nAction, int nFrame)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:	theApp.m_PmacUser.MoveTo(nAction);
		break;
	case SI6:
		break;
	case SI10: theApp.m_Si10User.MoveTo(nAction);
		break;
	case FIELD_BUS:	theApp.m_FBCtrl.MoveTo(nAction, nFrame);
		break;
	}

}

void CMotors::GotoHeadCoord(CCoord Cp)
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;

}


bool CMotors::IsRotateRunning()
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: return theApp.m_PmacUser.IsRotateRunning();
		break;
	case SI6:
		break;
	case SI10: return theApp.m_Si10User.IsRotateRunning();
		break;
	case FIELD_BUS:	return theApp.m_FBCtrl.IsRotateRunning();
		break;
	}
	
	return false;

}

void CMotors::StopContinuousRotate(int nAxis, bool bControlledStop)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: theApp.m_PmacUser.StopContinuousRotate(nAxis);
		break;
	case SI6: 
		break;
	case SI10: theApp.m_Si10User.StopContinuousRotate(nAxis);
		break;
	case FIELD_BUS:	theApp.m_FBCtrl.StopContinuousRotate(nAxis, bControlledStop);
		break;
	}

}

bool CMotors::IsPolyMotionFinished()
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: return theApp.m_PmacUser.IsPolyMotionFinished();
		break;
	case SI6:
		break;
	case SI10: return theApp.m_Si10User.IsMotionFinished();
		break;
	case FIELD_BUS:	return theApp.m_FBCtrl.IsMotionFinished();
		break;
	}

	return TRUE;
}

void CMotors::SetScanSpeed(float fSpeed)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: theApp.m_PmacUser.SetScanSpeed(fSpeed);
		break;
	case SI6:
		break;
	case SI10: theApp.m_Si10User.SetScanSpeed(fSpeed);
		break;
	case FIELD_BUS: theApp.m_FBCtrl.SetScanSpeed(fSpeed);
		break;
	}

}

int CMotors::DownLoadPolyMoveCoordinates(CPolyCoord *ToolPath, int nDir)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: return theApp.m_PmacUser.DownLoadPolyMoveCoordinates(ToolPath,nDir);
		break;
	case SI6:
		break;
	case SI10: return theApp.m_Si10User.DownLoadPolyMoveCoordinates(ToolPath,nDir);
		break;
	case FIELD_BUS: return theApp.m_FBCtrl.DownLoadPolyMoveCoordinates(ToolPath,nDir);
		break;
	}
	return 0;

}

void CMotors::SetupLookAhead()
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: theApp.m_PmacUser.SetupLookAhead();
		break;
	case SI6:
		break;
	case SI10:
		break;
	case FIELD_BUS: theApp.m_FBCtrl.SetupLookAhead();
		break;
	}

}

int CMotors::DownLoadNextPolyMoveCoordinate(int nNextCoordinate, CPolyCoord *ToolPath, int nDir)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: return theApp.m_PmacUser.DownLoadNextPolyMoveCoordinate(nNextCoordinate, ToolPath, nDir);
		break;
	case SI6:	return nNextCoordinate;
		break;
	case SI10:	return nNextCoordinate;
		break;
	}
	return nNextCoordinate;

}

void CMotors::StartPolyMoveWithLookAhead(int nn)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: theApp.m_PmacUser.StartPolyMoveWithLookAhead(nn);
		break;
	case SI6:
		break;
	case SI10: theApp.m_Si10User.MoveTo(DONT_WAIT);
		break;
	case FIELD_BUS: theApp.m_FBCtrl.StartPolyMoveWithLookAhead(nn);
		break;
	}

}

void CMotors::CollectComplexData(bool bAction)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: 
		break;
	case SI6:
		break;
	case SI10:
		theApp.m_Si10User.m_Si10.nWhichAxis = PROFILE->m_nFastAxis;
		theApp.m_Si10User.m_Si10.nWhichAxis = 2;
		theApp.m_Si10User.m_Si10.bCollectData = bAction;
		break;
	}
	theApp.m_bCollectComplexData = bAction;
}

void CMotors::MoveHoldindFixtureToCoord(HoldingFixtureCoord Cp)
{
	if(theApp.m_Si10User.IsConnected() == TRUE) {
		theApp.m_Si10User.MoveToCoord(Cp);
	} else {
		theApp.m_Si6User.MoveToCoord(Cp);
	}
}

bool CMotors::TestHoldingFixtureAllAxisRunning()
{
	if(theApp.m_Si10User.IsConnected() == TRUE) {
		return theApp.m_Si10User.TestHoldingFixtureAllAxisRunning();
	} else {
		return theApp.m_Si6User.TestAllAxisRunning();
	}
}

void CMotors::HoldingFixtureZeroPosition(int nAxis)
{
	if(theApp.m_Si10User.IsConnected() == TRUE) {
		theApp.m_Si10User.ZeroPosition(nAxis);
	} else {
		theApp.m_Si6User.ZeroPosition(nAxis);
	}
}

void CMotors::EnableHomingFixtureHomeFind(int nAxis)
{
	if(theApp.m_Si10User.IsConnected() == TRUE) {
		theApp.m_Si10User.EnableHomeFind(nAxis);
	} else {
		theApp.m_Si6User.EnableHomeFind(nAxis);
	}
}

void CMotors::ZeroHoldingFixturePosition(int nAxis)
{
	if(theApp.m_Si10User.IsConnected() == TRUE) {
		theApp.m_Si10User.ZeroPosition(nAxis);
	} else {
		theApp.m_Si6User.ZeroPosition(nAxis);
	}
}


void CMotors::EnsureJoystickValuesAreCurrent()
{
	int nTemp;

	switch (theApp.m_Tank.nJoystickSource) {
	default:
		break;
	case WIRED_SI10:
		for (int nIndex = 0; nIndex < 4; nIndex++) {
			nTemp = MulDiv(theApp.m_Si10User.ReadJoystick(nIndex) - 127, 156 - 127, 127) + 127;
			m_nJoyValue[nIndex] = MinMax(&nTemp, 0, 255);
		}
		m_nJoyValue[4] = 128;
		m_nSwitchJoyValue = m_nJoyValue[3];
		break;
	case WIRED_MSC:
		m_nJoyValue[0] = theApp.m_UtUser.MscAdcRead(0);
		m_nJoyValue[1] = theApp.m_UtUser.MscAdcRead(1);
		m_nJoyValue[2] = theApp.m_UtUser.MscAdcRead(2);
		m_nJoyValue[3] = theApp.m_UtUser.MscAdcRead(3);
		m_nJoyValue[4] = 128;
		m_nSwitchJoyValue &= theApp.m_Tank.nDeadManBit0 ^ 0xff;
		m_nSwitchJoyValue &= theApp.m_Tank.nDeadManBit1 ^ 0xff; //bypass deadman
		m_nSwitchJoyValue &= theApp.m_Tank.nMainManipBit ^ 0xff;
		m_nToggleJoyPos = 1;
		break;
	}

}

void CMotors::TotalDistanceTravelled()
{
	for(int nAxis = 0;nAxis<32;nAxis++) {
		if(theApp.m_Axes[nAxis].nPos - theApp.m_Axes[nAxis].nLastPosForTotalTravel) {
			theApp.m_Axes[nAxis].fTotalDistance += fabs((theApp.m_Axes[nAxis].nPos - theApp.m_Axes[nAxis].nLastPosForTotalTravel) * theApp.m_Axes[nAxis].fStepSize);
			theApp.m_Axes[nAxis].nLastPosForTotalTravel = theApp.m_Axes[nAxis].nPos;
		}
	}

}



//Kills motors designed to kill motors when amps turned off
void CMotors::KillAllMotors()
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: 
		theApp.m_PmacUser.KillAllMotors();
		break;
	}

}


void CMotors::StartTurntableCartesianScanningAxes(CCoord Cp)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: 
		theApp.m_PmacUser.StartTurntableCartesianScanningAxes(Cp);
		break;
	case SI6:
		break;
	case SI10:
		theApp.m_Si10User.MoveTurntableCartesianScanningAxes(Cp);
		break;
	case FIELD_BUS: 
		theApp.m_FBCtrl.StartTurntableCartesianScanningAxes(Cp);
		break;
	}

}

void CMotors::UpdateTurntableCartesianScanningAxes(CCoord Cp)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: 
		theApp.m_PmacUser.UpdateTurntableCartesianScanningAxes(Cp);
		break;
	case SI6:
		break;
	case SI10:
		theApp.m_Si10User.UpdateTurntableCartesianScanningAxes(Cp);
		break;
	case FIELD_BUS: 
		theApp.m_FBCtrl.UpdateTurntableCartesianScanningAxes(Cp);
		break;
	}


}

void CMotors::CollectEccentricData(bool bCollectData)
{
	theApp.m_bCollectEccentricData=bCollectData;

	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: 
		break;
	case SI6:
		break;
	case SI10:
		theApp.m_Si10User.m_Si10.nWhichAxis = theApp.m_Tank.nRLeft;
		theApp.m_Si10User.m_Si10.bCollectData = bCollectData;
		break;
	}
}

void CMotors::CollectNormalizeData(bool bCollectData, int nAxis)
{
	theApp.m_bCollectNormalizeData=bCollectData;

	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: 
		break;
	case SI6:
		break;
	case SI10:
		theApp.m_Si10User.m_Si10.nWhichAxis = nAxis;
		theApp.m_Si10User.m_Si10.bCollectData = bCollectData;
		break;
	}
}

int CMotors::GetInputPort()
{
	switch(theApp.m_LastSettings.nIOController) {
	default:
	case SERVO_TURBO:
	case SERVO:
		return theApp.m_PmacUser.GetInputPort();
		break;
	case SI10:
		return theApp.m_Si10User.GetInputPort();
		break;
	case SAFETY_UNIT:
		return theApp.m_SafetySerial.m_GeneralInputs;//GetInputPort();
		break;
	case FIELD_BUS:
		return	theApp.m_Ethercat.GetInputPort() ^ 0x80;
		break;
	}

	return 0; //Should never get here
}


void CMotors::ZBrakesEnable(int bOffOn, int nWhich)
{
	switch(theApp.m_LastSettings.nIOController) {
	default:
	case SERVO_TURBO:
	case SERVO:
		theApp.m_PmacUser.ZBrakesEnable(bOffOn, nWhich);
		break;
	case SAFETY_UNIT:
		theApp.m_SafetySerial.ZBrakesEnable(bOffOn, nWhich);
		break;
	}//switch

	return;

}

/*
void CMotors::SetGeneralOutputs(int nn)
{
	switch(theApp.m_LastSettings.nIOController) {
	default:
	case SERVO_TURBO:
	case SERVO:
		theApp.m_PmacUser.SetGeneralOutputs(nn);
		break;
	case SAFETY_UNIT:
		theApp.m_SafetySerial.SetOutputPort(nn);
		break;
	}//switch

	return;
}


int CMotors::GetGeneralOutputs()
{
	switch(theApp.m_LastSettings.nIOController) {
	default:
	case SERVO_TURBO:
	case SERVO:
		return theApp.m_PmacUser.GetGeneralOutputs();
		break;
	case SAFETY_UNIT:
		return theApp.m_SafetySerial.GetOutputPort();
		break;
	}//switch

	return 0; //shouldn't get here

}
*/

bool CMotors::ReportAirStatus(int nMask,int nSide)
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_LastSettings.nIOController) {
		default:
		case SERVO_TURBO:
		case SERVO:
			return theApp.m_PmacUser.ReportAirStatus(nMask);
			break;
		case SAFETY_UNIT:
			return theApp.m_SafetySerial.ReportAirStatus(nMask);
			break;
		case FIELD_BUS:
			return theApp.m_FBCtrl.ReportAirStatus(nMask ,nSide);
			break;
		}
		break;
	case FB_TWIN_TOWER:
	case DUAL_ROBOT_9_PLUS_9:
		return theApp.m_FBCtrl.ReportAirStatus(nMask ,nSide);
	};

	return 0; //shouldn't get here

}

void CMotors::AirStart(int nMask, int nSide)
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_LastSettings.nIOController) {
		default:
		case SERVO_TURBO:
		case SERVO:
			theApp.m_PmacUser.AirStart(nMask);
			break;
		case SAFETY_UNIT:
			theApp.m_SafetySerial.AirStart(nMask);
			break;
		case FIELD_BUS:
			theApp.m_FBCtrl.AirStart(nMask, nSide);
			break;
		}//switch
		break;
	case FB_TWIN_TOWER:
	case DUAL_ROBOT_9_PLUS_9:
		theApp.m_FBCtrl.AirStart(nMask, nSide);
		break;
	}

}

void CMotors::AirStop(int nMask, int nSide)
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_LastSettings.nIOController) {
		default:
		case SERVO_TURBO:
		case SERVO:
			theApp.m_PmacUser.AirStop(nMask);
			break;
		case SAFETY_UNIT:
			theApp.m_SafetySerial.AirStop(nMask);
			break;
		case FIELD_BUS:
			theApp.m_FBCtrl.AirStop(nMask, nSide);
			break;
		}//switch
		break;
	case FB_TWIN_TOWER:
	case DUAL_ROBOT_9_PLUS_9:
		theApp.m_FBCtrl.AirStop(nMask, nSide);
		break;
	}


}

int CMotors::GetBrakeStatus(int nWhich)
{
	switch(theApp.m_LastSettings.nIOController) {
	default:
	case SERVO_TURBO:
	case SERVO:
		return theApp.m_PmacUser.GetBrakeStatus(nWhich);
		break;
	case SAFETY_UNIT:
		return theApp.m_SafetySerial.GetBrakeStatus(nWhich);
		break;
	}//switch

	return 0; //shouldn't get here



}

bool CMotors::ReportPumpStatus(int nAxis, int nSide)
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		return theApp.m_PmacUser.ReportPumpStatus(nAxis);
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case FB_11_AXIS:
	case FB_TWIN_TOWER:
	case SPX_ROBOT:
	case TWIN_TOWER_KINEMATIC:
		return theApp.m_Ethercat.ReportPumpStatus(nAxis, nSide);
		break;
	}

}

void CMotors::SetPumpSpeed(int nAxis, int nSide)
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_PmacUser.SetPumpSpeed(nAxis);
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case FB_11_AXIS:
	case FB_TWIN_TOWER:
	case SPX_ROBOT:
	case TWIN_TOWER_KINEMATIC:
		theApp.m_FBCtrl.SetPumpSpeed(nAxis, nSide);
		break;
	}

}

void CMotors::PumpStart(int nAxis, int nSide)
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_PmacUser.PumpStart(nAxis);
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case FB_11_AXIS:
	case FB_TWIN_TOWER:
	case SPX_ROBOT:
	case TWIN_TOWER_KINEMATIC:
		theApp.m_FBCtrl.PumpStart(nAxis, nSide);
		break;
	}

}

void CMotors::PumpStop(int nAxis, int nSide)
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_PmacUser.PumpStop(nAxis);
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case FB_11_AXIS:
	case FB_TWIN_TOWER:
	case SPX_ROBOT:
	case TWIN_TOWER_KINEMATIC:
		theApp.m_FBCtrl.PumpStop(nAxis, nSide);
		break;
	}

}

void CMotors::CollectBladeData(bool bAction)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: 
		break;
	case SI6:
		break;
	case SI10:
		theApp.m_Si10User.m_Si10.nWhichAxis = PROFILE->m_nFastAxis;
		theApp.m_Si10User.m_Si10.bCollectData = bAction;
		break;
	}
	theApp.m_bCollectBladeData = bAction;

}

void CMotors::GotoSurfaceCoord(CCoord *pCpSurface, int nSide)
{
	CCoord	Cp,CpSurface,CpHead;

	CpSurface = *pCpSurface;

	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:	if(theApp.m_ServoUser.AreMotorsPowerd()==FALSE) return;
		break;
	case SI6:
		break;
	}

	theApp.m_Thread.StopCurrentThreadAction();

	switch(theApp.m_Tank.nScannerDescription) {
	default:

		GetHeadPos(&CpHead);

		theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
		theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,STARBOARD,TRUE,TRUE);

		switch(nSide) {
		case 0://Keep Right side where it is
			Cp.CopySide(STARBOARD,CpHead);
			Cp.Side1.fR = CpSurface.Side1.fR;
			break;
		case 1://Keep left side where it is
			Cp.CopySide(PORTSIDE,CpHead);
			break;
		}
		theApp.m_Thread.m_CpMoveTo = Cp;
		theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case RAILWAY_AXLE:
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
	case TANK_6AXIS:
		theApp.m_Thread.m_CpMoveTo = CpSurface;
		if(theApp.m_LastSettings.nGotoWithStepOff) {
			theApp.m_Thread.m_nMoveSide = nSide;
			theApp.m_Thread.m_nDangerPlaneThreadAction = 0;
			theApp.m_Thread.m_nThreadAction = MOVE_ROBOT_WITH_BACKOFF;
		} else {
			theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
		};

		break;
	}
}

void CMotors::IntelligentGotoSurfaceCoord(CCoord *pCpSurface, int nSideMask, int nDangerPlaneMask)
{
	int nSide[4] = { 0, 0, 1, 2 };
	theApp.m_Thread.StopCurrentThreadAction();

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		GotoSurfaceCoord(pCpSurface, nSide[nSideMask]);
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TANK_5AXIS:
	case QUICKSTEP_FLATBED:
	case TWIN_TOWER_KINEMATIC:
	case TANK_6AXIS:
		theApp.m_Thread.m_CpSurface = *pCpSurface;
		theApp.m_Thread.m_nMoveSide = nSideMask;
		theApp.m_Thread.m_nDangerPlaneMask = nDangerPlaneMask;
		theApp.m_Thread.m_nDangerPlaneThreadAction = 0;
		theApp.m_Thread.m_nThreadAction = INTELLIGENT_GOTO;
		break;
	}
}


bool CMotors::IsMotionStarted()
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:  return true;
		break;
	case SI6: return true;
		break;
	case SI10: return true;
		break;
	case FIELD_BUS: return theApp.m_FBCtrl.IsMotionStarted();
		break;
	}
	return false;
}

bool CMotors::IsItAtDesirePos()
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:  return true;
		break;
	case SI6: return true;
		break;
	case SI10: return true;
		break;
	case FIELD_BUS: return theApp.m_FBCtrl.IsItAtDesirePos();
		break;
	}
	return true;
}

void CMotors::RequestAllAccelerationsAndVelocities()
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:  theApp.m_PmacUser.RequestAllAccelerationsAndVelocities();
		break;
	case SI6:
		break;
	case SI10:
		break;
	case FIELD_BUS: theApp.m_FBCtrl.RequestAllAccelerationsAndVelocities();
		break;
	}

}

void CMotors::ResetAllMaxSpeeds()
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:  theApp.m_PmacUser.ResetAllMaxSpeeds();
		break;
	case SI6:
		break;
	case SI10:
		break;
	case FIELD_BUS: theApp.m_FBCtrl.ResetAllMaxSpeeds();
		break;
	}

}

bool CMotors::IsAlive()
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:  return true;
		break;
	case SI6:
		break;
	case SI10:
		break;
	case FIELD_BUS: return theApp.m_Ethercat.IsAlive();
		break;
	}

	return true;

}

void CMotors::RelativeMove(AxisData *pAxis, int nJogDistance)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: theApp.m_PmacUser.RelativeMove(pAxis, nJogDistance);
		break;
	case SI6:
		break;
	case SI10:
		break;
	case FIELD_BUS: theApp.m_FBCtrl.RelativeMove(pAxis, nJogDistance);
		break;
	}

}

void CMotors::AbsoluteMove(AxisData *pAxis, int nJogPosition)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:
		break;
	case SI6:
		break;
	case SI10:
		break;
	case FIELD_BUS: theApp.m_FBCtrl.AbsoluteMove(pAxis, nJogPosition);
		break;
	}

}


bool CMotors::IsMachineZero()
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: return theApp.m_PmacUser.IsMachineZero();
		break;
	case SI6:
		break;
	case SI10:
		break;
	case FIELD_BUS:
		break;
	}
	return FALSE;
}

void CMotors::LockTool(int nSide)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:
		break;
	case SI6:
		break;
	case SI10:
		break;
	case FIELD_BUS: theApp.m_FBCtrl.LockTool(nSide);
		break;
	}

}

void CMotors::UnlockTool(int nSide)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:
		break;
	case SI6:
		break;
	case SI10:
		break;
	case FIELD_BUS: theApp.m_FBCtrl.UnlockTool(nSide);
		break;
	}

}

void CMotors::OriginFingerManipulator()
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:
		break;
	case SI6:
		break;
	case SI10:
		break;
	case FIELD_BUS: theApp.m_FBCtrl.OriginFingerManipulator();
		break;
	}
}

void CMotors::EnableCrashDetector(bool bFlag, int nSide)
{
	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO:
		break;
	case SI6:
		break;
	case SI10:
		break;
	case FIELD_BUS: theApp.m_FBCtrl.EnableCrashDetector(bFlag, nSide);
		break;
	}
}

