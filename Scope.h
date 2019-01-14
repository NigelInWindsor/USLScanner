// Scope.h: interface for the CScope class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOPE_H__D1751FED_019F_4298_A2FF_BBEE4FA4C688__INCLUDED_)
#define AFX_SCOPE_H__D1751FED_019F_4298_A2FF_BBEE4FA4C688__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	SIGNED_BINARY	0
#define	UNSIGNED_BINARY	1

#define	SINE_WAVE		0
#define	SQUARE_WAVE		1
#define TRIANGLE_WAVE	2
#define	SAWTOOTH_WAVE	3

#define MAX_ARRAY_LENGTH	8192

class CScope  
{
public:
	float	m_fSGFrequency[10];
	int		m_nSGAmplitude[10];
	int		m_nSGType[10];
	int		m_nSGLengthMode[10];
	int		m_nSGWindow[10];

	void SignalGenerator(ADC200Data* pAdc,char *pArray,int nSlot);
	void CalculateLogConversionTable();
	void AllocateAverageSpace(int nSlot, int nNumberAverages);
	void FillScopeArray(int nSlot, char * pSrc, int nAcqLength, bool bLogFlag, int nAverages);
	void FillScopeArrayShort(int nSlot, __int16 * pSrc, int nAcqLength, bool bLogFlag, int nAverages);
	int m_nPlayRecording[256];
	bool m_bTimeSlotName;
	unsigned char cLogConversionTable[256];
	__int16 wLogConversionTable[0x10000];
	__int16	m_wRFTrace[257][MAX_ARRAY_LENGTH];
	char	m_RFTrace[257][MAX_ARRAY_LENGTH];
	int*	m_RfAverage[257];
	int*	m_nSum[257];
	int		m_nAllocatedAverage[257];
	int		m_nAverageCount[257];
	float	m_fMainBangConstant;
	float	m_fMaxRange;	//seconds
	float	m_fDelay;		//seconds
	float	m_fWidth;		//seconds
	float	m_fSamplePeriod;//seconds
	float	m_fSampleRate;	//Hertz
	float	m_fDCOffset;
	int		m_nAcqLength;
	int		m_nDelay;		//ns
	int		m_nWidth;		//ns
	int		m_nTextSize;
	int		m_nDigitizer;
	int		m_nMaxAcqLength;
	int		m_nGateDelayUnits;
	int		m_nGateWidthUnits;
	int		m_nTraceDelayUnits;
	int		m_nTraceWidthUnits;
	int		m_nRfType;
	int		m_nAdcIndexFirstPoint[256];
	int		m_nCoupling;
	int		m_nLogMin;
	int		m_nLogMax;
	int		m_nEncodeing;
	int		m_nAverages;
	int		m_nTOFText[2];
	int		m_nBits;
	int		m_nRulerDisplayMask;
	float	m_fVoltageRange;
	bool	m_bStatistics;
	bool	m_bBarGraph;
	bool	m_bWaterPathText;
	bool	m_bDelayWidthText;
	bool	m_bGraticule;
	bool	m_bTraceMode;
	bool	m_bDisplayGateAmp;
	bool	m_bLeftScale;
	bool	m_bRightScale;
	bool	m_bTopScale;
	bool	m_bBottomScale;
	unsigned char	m_cDisplayDACPtsMask;
	COLORREF m_rgbTextColor;
	COLORREF m_rgbGateColor[8];
	COLORREF m_rgbGraticuleColor;
	COLORREF m_rgbTraceColor;
	COLORREF m_rgbBkColor;
	COLORREF m_rgbPosPhaseColor;
	COLORREF m_rgbNegPhaseColor;
	COLORREF m_rgbDACColor;

	void Retrieve(CString FilePath);
	int	Save(CString FilePath);
	void Retrieve(CFile* pFile);
	int Save(CFile* pFile);
	void Zero();
	CScope();
	virtual ~CScope();

};

#endif // !defined(AFX_SCOPE_H__D1751FED_019F_4298_A2FF_BBEE4FA4C688__INCLUDED_)
