// PolyCoord.h: interface for the CPolyCoord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYCOORD_H__89B48DF3_95C7_4495_B63F_57E545CA9731__INCLUDED_)
#define AFX_POLYCOORD_H__89B48DF3_95C7_4495_B63F_57E545CA9731__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	LINE_STR_LENGTH	256

#define LINEAR_LINE		0
#define	SPLINE_LINE		1
#define	POLYNOMIAL_LINE	2
#define	QUADRATIC_LINE	3

#define	EOL_EXTRAPOLATION		0
#define	EOL_NO_EXTRAPOLATION	1

enum LineStyle {
	Linear = 0,
	Spline,
	Polynomial,
	Quadratic,
};

enum LineSort {
	along_axis_nearest = -2,
	along_axis_rotation,
};

enum EOL {
	extrapolate = 0,
	noextrapolate,
};


class CPolyCoord  
{
public:
	void SetExtrapolation(int nEol);
	float NearestPointDistance(XMVECTOR * vPoint, int nSide, XMVECTOR * vNearestPoint, XMVECTOR * vNearestNormal, float fScale);
	float FindNearestSegment(XMVECTOR * vStart, int nSide, int * pnSegment);
	float DistanceToSegment(XMVECTOR * vStart, int nSide, int nSegment);
	void SortNearestAdjacent();
	void MoveToFirstCoordinate(int nIndex);
	void CircumcenterBetweenTwoPoints(float fRadius, int nStartIndex, int nExtraPoints);
	void setEqual(CPolyCoord &SrcLine, int nDir);
	char* GetCharString();
	void DecimateEitherSideOfZero(float fStepSize);
	void setAllMaximumThickness(float *pfArray,int nNumberSamples,int nFirstIndex, int nDir);
	void setAllNominalThickness(float *pfArray,int nNumberSamples,int nFirstIndex, int nDir);
	void setAllMinimumThickness(float *pfArray,int nNumberSamples,int nFirstIndex, int nDir);
	bool m_bTLNeeded;
	bool GetNominalActualThickness(int nPos, float *fNomThickness, float *fActualThickness);
	void AlignLeadingEdge();
	void SetAllTypes(int nFlag);
	int CPolyCoord::FindNearestCoord(CCoord Cp);
	int FindNearestCoord(float fXPos,int nGuess);
	void SetAllfPos(CCoord *Cp,int nWhichF);
	bool GetActualThickness(float fXPos, CString *pBuff, float *fActThickness);
	bool GetNominalThickness(float fXPos, CString *pBuff, float *fNomThickness, float *fMinThickness = NULL, float *fMaxThickness = NULL, int *nType = NULL, bool *bExtrapolated = false, bool bTrafficLightCheckDuringSaving = false);

	void SetMinMaxThicknesses(float fMin, float fMax,int nStartIndex,int nFinishIndex);
	void SetMinMaxThicknesses(float fMin, float fMax);
	bool DeleteTrafficLight(float fXPos);
	int MoveTrafficLight(int nSample,float fXPos);
	int FindNearestTrafficLight(float fXpos);
	int AddTrafficLight(float fXPos);
	bool FormatBladeString(int nSample, char *str, D3DXMATRIXA16 &Matrix, CCoord CpTran,bool bTrafficLightCheckDuringSaving);

	bool GetVertex(int nIndex, int nSide, CUSLVertex *pVertex);
	void BladeEdgeClip();
	void DeleteAnyCoordXtYtGreater(float fAngle);
	void AddAllRPos(int nSide, float fAngle);
	void SetAllRPos(int nSide, float fAngle);
	void DeleteCoordsWith(int nWhichSign, char cNorm);
	void SetSizeToMaxAccessed();
	void RotateAllNormalsToIEqualsZero();
	void SetString(CString Buff);
	CString GetString();
	void ShiftAllCoordsAlongVector(float fDistance);
	void DeleteAnyCoordWithYgreaterThan(float fY,float fZMin, float fZMax);
	void FindMinimumYCoord(CCoord &Cp);
	void SetNormalsTo(int nWhichNormalComponent, float fValue);
	void Add(int nWhich_fPos,float fDelta);
	bool IsChangeInPos(int nIndex, int nAxis, float fDelta);
	bool IsChangeInDir(int nIndex, int nAxis);
	void ConvertLeftHandedToRightHanded();
	bool DoesPlaneCutSegment(float fPos, int nSegment, int nPlane);
	void Crop(int nLessThanGreaterThan, float fPos, int nAxis);
	void ProjectToYPlane(int nEnd,float fYequals);
	void SetAllCoords(CCoord Cp, int nSide);
	void SetAllCoordsButNotR(CCoord Cp, int nSide);
	float HowFarAlongLineIsCoordinate(CCoord Cp,int nAxis);
	void ComputePolarValues(D3DXVECTOR3 vecZero);
	void ComputeBothTipsFromNormals(int nPtr);
	void StripOutZeroCoords();
	void Extrapolate(int nEnd,float fLength,int nStyle);
	void SetStyle(int nStyle);
	int GetSize();
	void SetAllNorms(int nSide,D3DXVECTOR4 norm);
	void DecimateUsingZPos(float fZVariation);
	float DistanceToSegment(CCoord Cp, int nSide, int nSegment);
	float FindNearestSegment(CCoord Cp, int nSide, int *pnSegment);
	void GetFirstCoord(COORDDATA *pCp);
	int GetLastCoord(COORDDATA *pCp);
	int	GetCenterCoord(COORDDATA *pCp);
	bool AddIfNotAlreadyHere(CCoord Cp);
	void ChangeSignOfAnElement(char cWhich);
	void SetAllWp(int nSide,float fWp);
	void GetElectronicValues(CCoord* pCp,int nAxis);
	void GenerateSineWave(int nMode,CCoord *pCp,float fPitch,float fMaxAmp,float fPhase,float fSeg,float fCycles);
	void Straighten(int nIndex0,int nIndex1);
	void CalculateXtip(int nIndex);
	void CalculateCoord(CCoord Cp0,float fIncrement,CCoord *pCp1);
	float GetSplineLength(int nNumberOfSegs);
	void ZeroAllDeltas(int nAction);
	bool DoesXPlainCutLine(float fX,CCoord *pCp0,CCoord* pCp1,CCoord &Cp);
	void DeleteCoordInArea( CCoord *pCp0, CCoord *pCp1,int nIgnoreAxis);
	bool AddEntity(EntityStruct* pEntity,int nEntityNumber);
	void TransformLine(D3DXMATRIXA16 & matWorld);
	void SetAllModifiy(bool bFlag);
	void InsertCoord(int nIndex,CCoord Cp,int nAfterOrAt=0);
	void InsertCoord(CCoord Cp);
	void InsertCoord(int nIndex, COORDDATA Cp, int nAfterOrAt);
	bool Bisect(int nIndex);
	bool GetCoordComeWhatMay(int nIndex,COORDDATA *pCp);
	bool GetCoord(int nIndex,COORDDATA *pCp);
	float m_fExtrapolate[2];
	void ReallocateCoords(int nCoordL);
	void RemoveAll();
	void DeleteAllCoords();
	void ClearMemory();
	void FindPeakAmplitude(CCoord* pCp,int nMode = 0);
	float GetLength();
	void SetPrimaryAxis(int nAxis);
	int	m_nLine;
	int m_nPrimaryAxis;
	float m_fLinearLength;
	void FindSegment(float fPos,int &kLo,int &kHi);
	bool CalculateCoord(float fPos,CCoord &Cp);
	bool QuadraticInterpolation(float fPos,CCoord &Cp);
	bool LagrangeInterpolationRolling(float fPos,CCoord &Cp);
	bool LagrangeInterpolation(float fPos,CCoord &Cp);
	bool PolynomialInterpolation(float fPos,CCoord &Cp);
	bool Splint(float x,int nAxis,float *y,int *nType);
	void Spline(int nAxis);
	void PreProcessLine();
	void SortVariable(int nWhichVariable,int nSortDir = 0);
	void Sort(int nAxis,int nSortDir = 0);
	void InvertAllNormals();
	void InvertINormal();
	void InvertJNormal();
	void InvertKNormal();
	void Decimate(int nMaxPts);
	void DeleteCoords(int nStartIndex,int nFinishIndex);
	void ReverseAllCoords();
	void FindLimits(int nAxis, float *fMin,float *fMax);
	void FindLimits(COORDDATA *pCp0, COORDDATA *pCp1, bool bInitializeFirst);
	COORDDATA* GetPtrCoord(int nIndex);
	COORDDATA * ElementAt(int nIndex);
	int GetCoordType(int nIndex);
	void AllocateCoords(int nCoordL);
	void AllocateCoordMemory(int nCoordsToAllocate);
	ULONGLONG Save(CUSLFile* pFile);
	void Retrieve(CUSLFile * pFile, bool bOldCoordType = false);
	void SetColour(COLORREF rgb);
	COLORREF GetColour();

	void operator = (CPolyCoord* pCp);
	void operator += (CPolyCoord* pCp);
	void operator + (COORDDATA Cp);
	void operator + (COORDDATA *Cp);
	void Add(CCoord Cp);
	void Add(COORDDATA Cp);
	void AddVertex(CUSLVertex vPt);
	void DeleteCoord(int nIndex);
	void DeleteCoord(COORDDATA Cp,int nSide);
	void ModifyCoord(int nIndex,CCoord Cp);
	void ModifyBothNorms(int nIndex,D3DXVECTOR4 vec);

	COORDDATA	*m_pCp;
	int		m_nAllocatedSize;
	int		m_nCoordL;
	int		m_nStyle;
	int		m_nEol[2];
	int		m_nClosedEnds;
	int		m_nSpare[7];
	bool	m_bModified;
	bool	m_bImportOldCoordType;
	int		m_nKnotsL;
	int		m_nCurveOrder;
	int		m_nEntityNumber;
	int		m_nLineIncrement;
	int		m_nMaxPtr;
	int		m_nPlys;
	int		m_nNextPtr;
	int		m_nMaxAccessed;

	COLORREF m_rgb;

	int		m_nP1Curve;
	int		m_nP2Curve;
	int		m_nP3Curve;
	int		m_nP4Curve;
	float	m_fV0;
	float	m_fV1;
	float	m_fXNorm;
	float	m_fYNorm;
	float	m_fZNorm;
	float	m_fFrequency;

	float *m_fLength[2];
	float *m_fLengthPosDiff[30];
	float *m_pfKnots;

	D3DXMATRIXA16 m_Matrix;
	WCHAR	m_wStr[LINE_STR_LENGTH];

	//Not saved or retrieved
	int		m_nOffOn;
	int		m_nUseToScan;
	int		m_nUseToPrint;

	void Zero();
	CPolyCoord(CCoord Cp);
	CPolyCoord();
	virtual ~CPolyCoord();

};

int ComparePolyCoordData(const void *Cp0,const void *Cp1);
int CompareVariableCoordData(const void *Cp0,const void *Cp1);

#endif // !defined(AFX_POLYCOORD_H__89B48DF3_95C7_4495_B63F_57E545CA9731__INCLUDED_)
