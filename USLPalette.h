// USLPalette.h: interface for the CUSLPalette class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USLPALETTE_H__49142296_668B_4B06_B00D_CDEE046D682C__INCLUDED_)
#define AFX_USLPALETTE_H__49142296_668B_4B06_B00D_CDEE046D682C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	MAXENTRIES 200

#define	RETRIEVE_AMPLITUDE	1
#define	RETRIEVE_THICKNESS	2
#define RETRIEVE_PHASE		4
#define	RETRIEVE_POROSITY	8

struct	EntryStruct	{
	bool		bSet;
	COLORREF	rgbValue;
	float		fValue;
};

struct	PorosityData {
	int nStructSize;

	float fPorosity;
	float fAttenuation;
	float fMeandBAttenuation;
	float fMeanLinearAttenuation;
	int	nPlys;

	int	nSpare[10];
};

class CUSLPalette  
{
public:
	void PorosityDeleteAll();
	CString m_PorosityRefTarget;
	CString	m_PorosityStandardID;
	float m_fPorosityRefAttenuation;
	float m_fPorositySquirterDiameter;
	float m_fPorosityProbeDiameter;
	float m_fPorosityFrequency;
	void PorosityGetMinMaxPlies(int *nMinPlies,int *nMaxPlies);
	void GetMaxAttenuationPlys(int m_nIndex,float *fMaxAttenuation,int *nPlys);
	void PorositySort();
	void PorosityDeleteItem(int nIndex);
	void ProcessPorosityData();
	void GetMaxAttenuationPorosity(int nIndex,float *fAttenuation,float *fPorosity);
	void PorosityMeanChanged(int nIndex);
	int AddEmptyPorosityEntry();
	int	m_nPorosityL;
	int	m_nPorosityCurveL;
	CPolyCoord *m_pPorosityCurve;
	CPolyCoord m_PorosityThresholdCurve;
	PorosityData* m_pPorosity;
	CString m_FileName;
	void SamPaletteExtraction(int *Array);
	void MinMax(int *pnV,int nMin,int nMax);
	int	m_nStepMode[8];
	int m_nGraphType[8];
	int	m_nAmplitudeEntriesl[8];
	int m_nThicknessEntriesl[2];
	bool CopyAmpltudePaletteTo(int nSrc,int nDest);
	int m_nWhichAmp;
	int	m_nWhichThick;
	int	m_nBrightness[8];
	void GreyScale();
	double m_ddBZeroLevel[8];
	double m_ddBTopLevel[8];
	double m_ddBRange[8];
	double m_ddBStepSize[8];
	int m_nAmplitudeScaleType[8];
	int m_nMaxAmplitudeNumber[8];
	int m_nMaxThicknessNumber[2];
	int m_nMinThicknessNumber[2];
	COLORREF* m_rgb16[8];
	COLORREF* m_rgbAmplitude[8];
	COLORREF* m_rgbThickness[2];
	COLORREF m_rgbBWPassed;
	COLORREF m_rgbBWFailed;
	COLORREF m_rgbPosPhase;
	COLORREF m_rgbNegPhase;
	void CalculateLogPalette();
	void CalculateExtPalette();
	void CalculateAmplitudePalette();
	void CalculateThicknessPalette();
	void CalculatePalette();
	double m_dMaxThickness[2];
	double m_dMinThickness[2];
	double m_dThicknessRange[2];
	double m_dStepSize[2];
	int m_nModeThickness[2];
	int m_nModeAmplitude[8];
//	int	m_nAmplitudeStep;
	void Zero();
	struct	EntryStruct	m_Amplitude[8][MAXENTRIES];
	struct	EntryStruct	m_Thickness[2][MAXENTRIES];
	void Save(CString FilePath);
	void Retrieve(CString FilePath);
	void Save(CUSLFile* pFile);
	void Retrieve(CUSLFile* pFile,int nWhich = 0xffff);
	CUSLPalette();
	virtual ~CUSLPalette();
	void Destroy();

};

int ComparePorosityData(const void *Cp0,const void *Cp1);

#endif // !defined(AFX_USLPALETTE_H__49142296_668B_4B06_B00D_CDEE046D682C__INCLUDED_)
