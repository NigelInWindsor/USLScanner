// Profile.h: interface for the CProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILE_H__59A9673A_174B_4FAB_B8D8_3AD69357F129__INCLUDED_)
#define AFX_PROFILE_H__59A9673A_174B_4FAB_B8D8_3AD69357F129__INCLUDED_

#include "Coord.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PolyCoord.h"
#include "EdgeArray.h"
#include "VertexArray.h"
#include "StdAfx.h"	// Added by ClassView
//#include <map>


#define	FIXED_INC		0
#define	VARIABLE_INC	1

struct CoordStructGeneric {
	int	nStructSize;

	double	dX0p;
	double	dX1p;
	double	dY0p;
	double	dY1p;
	double	dZ0p;
	double	dZ1p;
	double	dXt0p;
	double	dXt1p;
	double	dYt0p;
	double	dYt1p;
	double	dR0p;
	double	dR1p;
	int nLine;
	double dWp;
};

struct	BoreScanHeader {
	int nStructSize;

	float fdBTop;
	float fdBBottom;

	float fRefZTop;
	float fRefZBottom;

	float fEvalZTop;
	float fEvalZBottom;

	float fDefectArea;

	int	nSpare[50];
};

enum LineTypes {
	taught = 0,
	scan,
	brane,
	envelope,
	reference,
	forward,
	reverse,
};

enum DangerPlaneType {
	none,
	x_plane_n,
	x_plane_p,
	y_plane_n,
	y_plane_p,
	z_plane_n,
	z_plane_p,
	normal,
	OD,
	ID,
};

class CProfile
{
public:
	bool IsCorrectOneLoaded(CString *pDateProfileName);
	int GetNumberLines(int nLineType);
	CPolyCoord * GetLineElement(LineTypes nLineType, int nLine);
	CCoord & GetRefCoord(int nIndex);
	void SetRefCoord(int nIndex, CCoord Cp);
	XMFLOAT3 & getDangerPlaneGlassSize(int nSide);
	XMFLOAT3 & getOverallSize();
	bool getNeedsRendering();
	bool setNeedsRendering(bool bFlag);
	bool GenerateSingleLineBScan(int nLineNumber);
	bool LoadDesiredUtFile();
	void GenerateInterpolatedLines();
	void InsertExtraScanLines(int nNumberExtraLines);
	void setRUsed(bool bState);
	bool IsCoordinateInsideEnvelope(int nLine,int nIndex,bool *bPresentlyInside);
	float GetCosAngleDeviation(CCoord Cp0,CCoord Cp1);
	void GenerateRaftWayPoints(CPolyCoord *pLine, int nScanLine,int nNumberScanLines);
	void GuessCADNorms();
	void ImportCADRefPts(CUSLFile *pFile);
	void GenerateRRArcScanLines();
	void CalculateLiftOffPointsForWayPoints(int nLine,int nNumberLines, int nDir, CPolyCoord *pLine);
	int m_nWayPointsWithLiftOff;
	void BisectAndAddLine(int nLineType,int nLine);
	void GenerateTubeScan(int nPrimaryAxis);
	void ApplyAdditionalAngle(CCoord * pCp);
	void CopyBraneLinesToBladeBraneLines(int nSide);
	void AddBladeBranePt(int nLine);
	void DecimateBladeLines(int nWhich);
	void AddRAngleToBlade(float fAngle,int nOutsideInside);
	void RotateBlade(int nAxis,float fAngle, int nOutsideInside);
	void GenerateArcScanLines(int nCentreMode);
	void BladeEdgeClip();
	void DeleteAnythingGreater45();
	void GenerateInterpolatedScanLines();
	void RotateComponent(int nAxis, float fAngle);
	void DeleteScanCoordWith(int nWhich, char cNorm,int nStart, int nFinish);
	int m_nBraneStyle;
	void TrimEdge(CCoord &Cp0, CCoord &Cp1);
	void GenerateDNoseScanLinesWithStraightLE();
	void SetScanLineRAngle(int nStartLine, int nFinishLine, float fAngle);
	void AddScanLineRAngle(int nStartLine, int nFinishLine, float fAngle);
	void SetScanLineWaterPaths(int nStartLine,int nFinishLine,float fWaterPath);
	void RotateAllNormalsToIEqualsZero();
	void AddRRotation(float fDelta);
	void DeleteLines(int nLineType, int nStartIndex, int nFinishIndex);
	void SplitScanLinesDueToValueChange(int nAxis);
	void SplitScanLinesDueToXDirChange();
	void ConvertLeftHandedToRightHanded(int nLineType);
	void DeleteScanLinesThatAreCompletelyOneSideOfThePlane(int nLessThanGreaterThan,float fPos,int nPlane);
	void CropScanLines(int nLessThanGreaterThan,float fPos,int nAxis);
	void CalculateTessellationEdges();
	int m_nEdgeL;
	int	m_nTriangleL;
	CEdgeArray m_Edge;
	TRIANGLE* m_pTriangle;
	CVertexArray m_vCloud;
	int m_nFlattenMode;
	void GenerateTurntableWayPoints();
	void ComputePolarValues(int nWhichLines);
	void AllocateSurfaceLine(int nSamplesLine);
	void ModifySurfaceLine(CCoord Cp,int nSample,int nLine);
	void ExtrapolateScanLines(int nEnd,float fLength);
	void FindLineLengthFromTaughtLines(CCoord Cp,float *fLength);
	void CopyLocalUtToGlobal();
	void CopyGlobalUtToLocal();
	void ComponentMoveLine(D3DXMATRIXA16 & matWorld, int nLineType, int nLine);
	void ComponentMove(D3DXMATRIXA16 & matWorld);
	bool DeleteCoordFromLine(int nLineType,int nLine, int nIndex);
	bool AddCoordToLine(int nLineType,int nLine, CCoord Cp);
	void DivideScanLineInTwo(int nLine,int nIndex);
	void GenerateScanLinesScanForAsymetricDNose();
	int m_nMeasuredRefPtsLocked;
	void SetAllNormals(int nLineType, D3DXVECTOR4 norm);
	void GenerateFlatPackMesh();
	void SetToolPathFromLine(CPolyCoord *pLine);
	void FindScanLineSampleSlowVersion(CCoord Cp, int *pnSample, int *pnLine);
	void FindScanLineSample(CCoord Cp, int *pnSample, int *pnLine);
	void CalculateTurntableDangerZone();
	void AvoidExclusionZones(CCoord* pCp);
	void SwapLines(int nLine0, int nLine1,int nType);
	int FindNearestScanLine(CCoord Cp);
	void SetLineIncrement(CPolyCoord *pLine);
	void ChangeSignOfAnElement(char cWhich);
	void InvertAllNormals();
	void ModifyCoordWp(int nLineType,int nLine0,int nLine1,int nLeftDo,float fWpLeft0,float fWpLeft1,int nRightDo,float fWpRight0,float fWpRight1);
	void Untwist();

	int GetMaxWayPoints(CString *pBuff);
	void CalculateWayPoints(PVOID pVoid);
	bool m_bCalculateWayPointsThreadEnabled;
	CWinThread* 	m_pBackGroundThread;
	void SuspendCalculateWayPointsThread();

	void GetDiameters(float* fDiameters);
	HRESULT GenerateRectilinearScanFromCoordinates(int nSide);
	HRESULT GenerateRectilinearScanFromSizes(int nSide);
	HRESULT GenerateContinousRotateRectilinearScanFromCoordinates(int nSide, bool bCoordinates);
	void OrderLines(int nWhich,int nOrderAxis);
	void ModifyTaughtCoordinate(int nLine,int nIndex);
	int m_nTurntableScanMode;
	void GenerateRotoSymetricScanLineV1();
	void GenerateRotoSymetricScanLineV2();
	void ZeroAllBrainDeltas(int nAction);
	void ReverseScanLineOrder();
	void CropCoordInArea(int nLineType, CCoord *pCp0, CCoord *pCp1,int nIgnoreAxis);
	bool CalculateEnvelopeFromRect();
	void DeleteAllLines(int nLineType);
	void DeleteCoordInArea(int nLineType, CCoord *Cp0, CCoord *Cp1,int nIgnoreAxis);

	bool CheckNewProfileAllowed();
	void ModifyBrainDeltas(int nLine,int nIndex, bool bSingleSided = false);
	bool CalculateBrainCoord(int nFastAxis,int nSlowAxis,CCoord &pCp);
	void GenerateMesh();
	void GenerateHorizontalMesh();
	void GenerateVerticalMesh();
	void RotatePlaneToZ0(CCoord O,CCoord A, CCoord B, float *fAngle0, float *fAngle1, float *fAngle2);
	float VectorLength(VectorStruct Vp);
	void MountAtSurfaceD3DX();
	void MountAtSurface();
	void CrossProduct(CCoord A, CCoord B, CCoord *pP);
	float DotProduct(CCoord A, CCoord B);
	void CalculateMathematicalNormals();
	int m_nPerpendicularStyle;
	bool GetSurfaceCoordinate(CCoord &pCp,CPoint ptSample, float fPos,int nFastAxis,bool bGenerateVirtualXY = false);
	void CalculateOppositeTaughtSide(int nSide);
	void ModifyTaughtLine(int nLine,CPolyCoord* pLine);
	int GetTaughtLineCoordL(int nLine);
	bool GetTaughtCoord(int nLine,int nIndex,CCoord* pCoord);
	void FindScanSizes(float *pfFastScanSize,float *pfSlowScanSize,CCoord *CpStart,CCoord * CpFinish);
	int FindMaximumSamplesLine(int *nSamplesLine);
	int FindNumberScanLines(int *pnNumberScanLines);
	void CalculateToolPath(int nLine,int nStorrageDestination = 0,int nDir = 0);
	void CalculateToolPath(int nLine,CPolyCoord &ToolPath, int nStorrageDestination = 0,int nDir = 0);
	int m_nScanFinishLine;
	int m_nScanStartLine;
	void CalculateDangerZone();
	bool CalculateCoord(int nFastAxis,int nSlowAxis,float fFastPos, float fSlowPos, CCoord &pCp);
	bool FindScanLineLimitsFromEnvelope(int nFastAxis,int nSlowAxisAxis,float fPosp,CCoord *CpStart,CCoord *CpFinish);
	bool FindScanLineLimits(int nSlowAxisAxis,float fPosp,CCoord *CpStart,CCoord *CpFinish);

	void GenerateScanLines();
	void GenerateScanLinesScanForJNose();
	void GenerateScanLinesScanParallelToTaughtUsingFastAxis();
	void GenerateScanLinesScanParallelToTaughtUsingAxis30(float fStepSize =50.0f);
	void GenerateScanLinesScanPerpendicularlToTaught();

	void SortLine(int nLineType,int nLine,int nSortAxis);
	void SortEachLine(int nWhichLines,int nSortAxis);
	void New();
	HoldingFixtureCoord m_CpHolding;
	void GetTheProfileInTheScanner(int nMode = 0,COORDDATA *pCp = NULL);
	void DecodeGotoLine(CString Buff,COORDDATA* pCp);
	int AddLine(int nLineType,CPolyCoord* pInsertLine,int nSortAxis = 0);
	void AddLine(int nLineType);
	void DeleteLine(int nSrcLine,int nLine);
	void ReverseLines(int nSrcLine,int nStep,int nStartLine = 1);
	void FindBoundingCube(int nWhichLines,CCoord* pCpMin,CCoord* pCpMax);
	void AutomaticRectangleEnvelope(int nSrcLines,float fTop,float fLeft,float fBottom,float fRight);
	void AutomaticEnvelope(int nSrcLines,float fTop,float fLeft,float fBottom,float fRight);
	bool FindNearestCoordinate(int nWhichLine,COORDDATA Cp,int *pnLine,int *pnIndex,char cIgnoreAxis=0);
	bool FindNearestAngleCoordinate(int nWhichLine,COORDDATA Cp,int *pnLine,int *pnIndex);
	void DisplayThumbNail(CString csFilePath,CDC* pDC,CRect rr);
	void CopyAllLines(int nDest,int nSrc,bool bDeleteSrc = false);
	void CopyAllLinesToGlobal(int nSrc);
	void PasteAllLinesFromGlobal(int nDest, int nInsertAt = 0);
	bool IsRUsed();
	bool m_bNeedsRendering;
	bool m_bDiametersDervivedFromCoords;
	int	m_nBiUniDirectionalScan;
	int m_nLineIncrement;
	int	m_nLineIncrementMode;
	float m_fFastIncrement;
	float m_fSlowIncrement;
	float m_fFastScanSize;
	float m_fSlowScanSize;
	float m_fComponentDiameter[2];	//First Line, Last Line;
	DangerPlaneType	m_eDangerPlaneType[2];
	XMFLOAT3		m_vDangerPlaneNorm[2];


	void	SetScanStartTime();
	float	CalculateLineScanTime(int nLine);
	CTime	m_StartTime;
	CTime	m_FinishTime;

	bool	m_bExclusionZones[3];
	int m_nManipulatorType[2];
	D3DXVECTOR3		m_vecToDangerPlane[2];
	D3DXVECTOR4		m_vecAdditionalAngle[2];
	bool			m_bUseAdditionalAngleDuringScanning[2];
	CString m_FileName;
	CString m_strDescription;
	CString m_UltrasonicPathFileName;
	CCoord m_CpExclusion[3][2];
	CCoord m_CpCadHole[3];
	CCoord m_CpMeasuredHole[3];
	CCoord m_CpDanger[2];
	CCoord m_CpRect[2];
	CCoord m_CpTranslation[MAX_LINE_TYPES];
	CCoord m_CpMax;
	CCoord m_CpMin;
	void ReverseAllLines();
	void TransformCoordinate(COORDDATA Cp, COORDDATA *pCp);
	void CalculateMatrix();
	void AcceptTransform();

	void FindAllHoleLimits(COORDDATA *pCp0, COORDDATA *pCp1);
	void FindAllLineLimits(COORDDATA *pCp0, COORDDATA *pCp1);
	void FindLineLimits(CPolyCoord *pLine,int nNumberLines,COORDDATA *pCp0, COORDDATA *pCp1);
	void GetScanLineCoord(int nLine,int nIndex,COORDDATA *pCp);
	int GetScanLineCoordL(int nLine);

	CPolyCoord m_Approach;
	CPolyCoord m_Depart;
	CPolyCoord* m_SurfaceLine;
	CPolyCoord* m_ScanLine;
	CPolyCoord* m_BrainLine;
	CPolyCoord* m_TaughtLine;
	CPolyCoord* m_EnvelopeLine;
	CPolyCoord* m_WayPoints;
	CPolyCoord*	m_LineBlade[MAX_LINE_TYPES];
	CPolyCoord m_ToolPath;

	int m_nRotaryScanMode;
	int	m_nCollectScanLines;
	int	m_nCollectSurface;
	int	m_nNumberProbesPerImage;
	int	m_nFirstProbe;
	int	m_nHeader[20];
	int m_nOriginOffsets[30];
	int	m_nSurfaceLineL;
	int	m_nWayPointsL;
	int	m_nScanLineL;
	int	m_nBrainLineL;
	int	m_nTaughtLineL;
	int	m_nEnvelopeLineL;
	int	m_nLineBladeL[MAX_LINE_TYPES];
	int m_nScanResumeLine;
	int m_nFastAxis;
	int m_nSlowAxis;
	int m_nComputeAlgorithm;
	int	m_nRectilinearScanMode;
	int	m_nScanType;
	int	m_nCollectAxis;
	int	m_nWayPointNumber;
	int m_nWaypointMode;
	float	m_fWaypointIJKChange;
	float	m_fWaypointXYZChange;
	ComponentDescription	m_nComponentDescription;

	TimeSlotData	m_TS[256];
	UtGlobalData	m_Global;
	BoreScanHeader	m_BoreScanHeader;

	bool  m_bModified;
	float m_fTranslateX;
	float m_fTranslateY;
	float m_fTranslateZ;
	float m_fRotateX;
	float m_fRotateY;
	float m_fRotateZ;
	float m_fRotateR;
	float m_fScanSpeed;
	D3DXMATRIXA16 m_matCadToScan;
	D3DXMATRIXA16 m_matBladeLine[4];
	D3DXVECTOR3 m_vecBladeOffset[2];

	int m_nRUsed;

	void AddBraneLine(CPolyCoord *pPolyLine);
	void AddTaughtLine(CPolyCoord *pPolyLine);
	void AddScanLine(CPolyCoord *pPolyLine);
	void ImportAptSourceXifeiFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType);
	void ImportAptSourceCTRMFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType);
	void ImportAptSourceFokkerSingleRapidFormat(CString FileName);
	void ImportAptSourceFokkerDoubleRapidFormat(CString FileName);
	void ImportAptSourceBAESamlesburyFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType);
	void ImportAptSourceHafeiFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType);
	void ImportAptSourceGKNFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType);
	void ImportAptSourceTermaFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType);
	void ImportAptSourceGknJsfFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType);
	void ImportAptSourceSimpleRaster(CString FileName);
	void ImportAptSourceGoodrichFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType);
	void ImportAptSourceGoodrichMachine4(CString FileName);
	void ImportAptSourceDowtyFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType);
	bool ImportDowtyProfile(CUSLFile *pFile);
	void CopyGenericToCCoord(CCoord &CpSurface, CoordStructGeneric Cp);

	void operator = (CProfile* pProfile);

	bool Retrieve(CUSLFile* pFile);
	ULONGLONG Save(CUSLFile* pFile);
	void Zero();
	int m_nScanSide;
	CProfile();
	virtual ~CProfile();
	void ProfileDataValidationCheck();
	int FindLineNumber(int nWhichLines,int nAxis, float fPosition, float fMaxDeviation, float fMaxVariance);

	void InvertNormals(bool bTaught, bool bScan, bool bBrain, bool bCAD, bool bI, bool bJ, bool bK);


	int m_nSurfaceSample;

private:
	float m_fIden[3][3];
	float m_fRotX[3][3];
	float m_fRotY[3][3];
	float m_fRotZ[3][3];


};

#endif // !defined(AFX_PROFILE_H__59A9673A_174B_4FAB_B8D8_3AD69357F129__INCLUDED_)
