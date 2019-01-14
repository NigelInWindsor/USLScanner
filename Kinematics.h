// Kinematics.h: interface for the CKinematics class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KINEMATICS_H__C969F327_DECB_4E8E_9B3C_954BF25EDABC__INCLUDED_)
#define AFX_KINEMATICS_H__C969F327_DECB_4E8E_9B3C_954BF25EDABC__INCLUDED_

#include "PolyCoord.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	XPTS	60
#define	YPTS	50
#define	ZPTS	50

#define	XTPTS	60
#define	YTPTS	60

#define	HORIZONTAL_WRIST			0
#define	VERTICAL_WRIST				1
#define	VERTICAL_WRIST_90			2
#define	INDEX_FINGER				3
#define	FOREPLANE_VERTICAL_WRIST	4	
#define	INDEX_FINGER_V2				5
#define	GOODRICH_4					6
#define	ROTATE_DIF_THETA			7
#define	FOREPLANE_VERTICAL_WRIST_1	8
#define	DOWTY_B_SCAN				9
#define	ROTATE_THETA_HORIZONTAL		10
#define	ROTATE_THETA_VERTICAL		11
#define	ARCADIA_2					12
#define	SWIVEL_Z_OFFSET_GIMBAL		13
#define SWIVEL_GIMBAL				14
#define TWI_MANIP					15
#define ROTATE_SWIVEL_GIMBAL		16
#define TWIN_TOWER_FINGER			17
#define VERTICAL_WRIST_PPMAC		18

struct	RhomboidStruct {
	int	nStructSize;

	float	dXp;
	float	dYp;
	float	dZp;
	float	dDeltaX;
	float	dDeltaY;
	float	dDeltaZ;
	int		nXp;
	int		nYp;
	int		nZp;
	int		nDeltaX;
	int		nDeltaY;
	int		nDeltaZ;
	int		nAction;
	int		nAmplitude;
};

struct	TipStruct {
	int	nStructSize;

	float	dXtp;
	float	dYtp;
	float	dDeltaXt;
	float	dDeltaYt;
	int		nXtp;
	int		nYtp;
	int		nDeltaXt;
	int		nDeltaYt;
	int		nAction;
	int		nAmplitude;
};

struct	ProbeStruct {
	int nStructSize;

	float	fX;
	float	fY;
	float	fZ;
	int		nX;
	int		nY;
	int		nZ;
	int		nSampleOffset;
	int		nLineOffset;

	int		nSpare[10];
};

class CKinematics  
{
public:
	void InvalidateToolchangerKinematics();
	void ZeroFrame(int nSide);
	void D3DXRotateMatrixFromNormal(D3DXMATRIXA16 &mat, D3DXVECTOR4 vec);
	bool RetrieveManipulator(CFile *pFile, int nSide);
	bool SaveManipulator(CFile *pFile, int nSide);
	float GetDesiredWaterPath(int nSide);
	void RThetaShortestRoutes(CPolyCoord *pLine,int nDir,CCoord *pCpCurrent);
	void CalculateRollerKinematics(float fComponentDiameter, bool bSetStepsPerRev, bool bTakeShortestTurntableRoute);
	void CalculateRandP(CCoord *pCp,int nSide,float fRefAngle);
	float m_fIden[3][3];
	float m_fRotX[3][3];
	float m_fRotY[3][3];
	float m_fRotZ[3][3];
	float m_fTranslateX;
	float m_fTranslateY;
	float m_fTranslateZ;
	float m_fRotateX;
	float m_fRotateY;
	float m_fRotateZ;
	float VectorLength(VectorStruct Vp);
	void RotatePlaneToZ0(CCoord O,CCoord A, CCoord B, float *fAngle0, float *fAngle1, float *fAngle2);
	void TransformCoordinate(COORDDATA Cp, COORDDATA *pCp);
	void CalculateMatrix();
	void Convert(CCoord CpThreeCoordsTo[], CCoord CpThreeCoordsFrom[], CCoord *CpTo, CCoord *CpFrom);
	void SurfaceFromLaser(CCoord *CpLaser, CCoord *CpSurface);
	void LaserFromSurface(CCoord *CpSurface, CCoord *CpLaser);

	void MoveInToCorrectQuadrant(CPolyCoord* pLine,int nSide);
	void LeadScrewCompensate(int nAxis, float fCurrentPos, float fDesirePos, int *nPos);
	void D3DXRotateMatrixFromJNormal(D3DXMATRIXA16 &mat, D3DXVECTOR4 vec);
	void D3DXRotateMatrixFromKNormal(D3DXMATRIXA16 &mat, D3DXVECTOR4 vec);
	void RThetaShortestRoute(CCoord* pCpDest,CCoord* pCpSrc,int nSide);
	void CalculateProbeSampleOffsets(CData *pData);
	void InvalidateDesiredWaterPath(CCoord *pCp);
	void GetComponentThickness(CCoord *pCp);
	void GetDesiredWaterPath(CCoord *pCp,int nSide);
	void CalculateCompensationPos(CCoord* pCp);
	void PreProcessAxesCompensation();
	CLeadScrew m_LeadScrew[16];
	CString ManipulatorFileName;
	CString ManipulatorFilePath;
	bool RetrieveManipulator(CString FilePath);
	bool RetrieveManipulator(CUSLFile *pFile);
	void SaveManipulator(CString FilePath);
	void SaveManipulator(CUSLFile *pFile);
	bool SaveCSV(CString FilePath);
	void Zero();
	int m_nAmp;
	void SetAllActions();
	bool m_bLearnMode;
	void MarkTestPieceAction();
	void MarkScanVolumeAction();
	void CurrentCubeMarkAction(CCoord	*pCpHead);
	void CopyPlaneToPlane(int nYSrc,int nYDest);
	void ClearDeltas(int nX,int nY,int nZ);
	void CalculateBallCenterFromSurface(CCoord CpSurface,CCoord* pCpCenter);
	bool CheckValidWaterPath(int nSide, int nTimeSlot,float *fWp = NULL);
	void CalculateBallCenterFromHole(CCoord CpHole,CCoord* pCpBall);
	void CalculateStarboardSurfacePos(CCoord* pCp);
	void SetComponentThickness(float dComponentThickness);
	void ModifyTipDeltas(int nX, int nY, CCoord CpHead);
	void CalculateRealTipCoord(CCoord* pCp);
	bool InitializeTip();

	ProbeStruct	m_Probe[256];
	TipStruct	m_Tip[XTPTS][YTPTS];
	float	m_dXTipMinMax;
	float	m_dYTipMinMax;
	float	m_dXTipStep;
	float	m_dYTipStep;
	int		m_nXTipMinMax;
	int		m_nYTipMinMax;
	int		m_nXTipStep;
	int		m_nYTipStep;
	int		m_nXTipPts;
	int		m_nYTipPts;

	void Compensate(CCoord* pCp);
	int MulDiv(int nV0,int nV1,int nV2);
	void CalculateRealCoord(CCoord* pCpHead);
	void OldCalculateRealCoord(CCoord* pCpHead);
	void ModifyDeltas(int nX,int nY,int nZ,CCoord CpSurface);
	void ClearAllActions();
	bool CalculateIntegerVariables();
	bool InitializeRhomboid();
	bool m_bDesiredWaterPathChanged;
	float	m_dXStepSize;
	float	m_dYStepSize;
	float	m_dZStepSize;
	float	m_dXMin;
	float	m_dYMin;
	float	m_dZMin;
	float	m_dXMax;
	float	m_dYMax;
	float	m_dZMax;
	int	m_nXStepSize;
	int	m_nYStepSize;
	int	m_nZStepSize;
	int	m_nXMin;
	int	m_nYMin;
	int	m_nZMin;
	int	m_nXMax;
	int	m_nYMax;
	int	m_nZMax;
	int	m_nXpts;
	int	m_nYpts;
	int	m_nZpts;
	int	nKinematicOffsetLoadMask;
	int nLimitSwitchOffsetLoadMask;
	int	m_nAxesIncludedJoystick;
	int	m_nAxesIncludedMoves;
	RhomboidStruct	m_Rhomboid[XPTS][YPTS][ZPTS];
	D3DXMATRIXA16	m_matNormal,m_matX,m_matY,m_matZ;


	void CalculateCoordAlongNorm(CCoord CpSurface,CCoord* pCp,int nSide,float fDistance);
	void CalculateOverShootPoint(CCoord CpSurface,CCoord* pCp,int nSide);
	void CalculateApproachPoint(CCoord CpSurface,CCoord* pCp,int nSide);
	void CalculateOppositeCoord(int nDestSide,CCoord *pCp,float fThickness=0.0f);
	void NormalsFromTips(CCoord *pCp,int nSide);
	void TipsFromNormals(CCoord *pCp,int nSide,float fRotateThetaRevNumber = 0.0f);
	float m_fRobotWeightingX[2];
	float m_fRobotWeightingY[2];
	float m_fRobotWeightingZ[2];
	float m_dCosXtip[2];
	float m_dSinXtip[2];
	float m_dCosYtip[2];
	float m_dSinYtip[2];
	float m_dYtip[2];
	float m_dXtip[2];
	float m_dXError;
	float m_dYError;
	float m_dZError;
	float m_dXtError;
	float m_dYtError;
	int m_nXError;
	int m_nYError;
	int m_nZError;
	int m_nXtError;
	int m_nYtError;
	int	m_nXtipRotateAxis;
	int	m_nYtipRotateAxis;
	int	m_nInvertXtip;
	int	m_nInvertYtip;
	RhomboidStruct* m_pFrontLowerLeft;
	bool m_bRhomboidCompensation;
	bool m_bTipCompensation;

	void HeadFromSurface(CCoord *CpSurface, CCoord *CpHead, int nSide, bool bCalculateSinCos = TRUE, bool bUseDesiredWp = TRUE, CCoord *CpCurrent = NULL, int nFl = 0);
	void SurfaceFromHead(CCoord *CpHead, CCoord *CpSurface, int nSide, bool bCalculateSinCos = TRUE, bool bUseDesiredWp = TRUE, int nFl = 0);
	bool Retrieve(CString Name);
	void Save(CString Name);
	bool Retrieve(CUSLFile* pFile);
	float getToolLength(int nSide);
	void Save(CUSLFile* pFile);

	float m_fXOffset[8];
	float m_fYOffset[8];
	float m_fZOffset[8];
	float m_fProbeLength[8];
	float m_fDesiredWaterPath[8];
	float m_fActualWaterPath[8];
	float m_fWaterPath[8];
	float m_fArmLength[8];
	float m_fProbeMirror[8];

	float m_fComponentThickness;

	D3DXVECTOR3	m_vLimb0[8];
	D3DXVECTOR3	m_vLimb1[8];

	CKinematics();
	virtual ~CKinematics();

};

#endif // !defined(AFX_KINEMATICS_H__C969F327_DECB_4E8E_9B3C_954BF25EDABC__INCLUDED_)
