// Probe.h: interface for the CProbe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROBE_H__20D1ABA6_5D0C_4807_BDDA_155C350B5A83__INCLUDED_)
#define AFX_PROBE_H__20D1ABA6_5D0C_4807_BDDA_155C350B5A83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FFT.h"

class	CProbeDataSheet
{
public:
	int	m_nInterfaceDelay;
	int	m_nInterfaceWidth;
	float GetLowerFrequency(CString &Buff);
	float m_fBandWidth;
	float m_fUpperFrequency;
	float m_fLowerFrequency;
	float m_fCentralFrequency;
	float m_fPeakFrequency;
	float m_fPulseDurationNs;
	void CalculateStats(float fAtAttenuation);
	float GetRatioBWCF(CString &Buff);
	float GetUpperFrequency(CString &Buff);
	float GetBandWidth(CString &Buff);
	float GetCentralFrequency(CString &Buff);
	float GetPeakFrequency(CString &Buff);
	float GetPulseDuration(CString &Buff);
	CFFT m_FFT;
	void FastFourierTransform();
	void Refresh();
	bool Retrieve(CUSLFile* pFile);
	int	Save(CUSLFile *pFile);
	void Zero();
	bool m_bModified;
	bool m_bLocked;
	TimeSlotData	m_TS;
	float			m_fDelay;
	float			m_fSamplePeriod;
	int				m_nAcqLength;
	int				m_nFFTLength;
	int				m_nNumberFields;
	int				m_osTime;
	float			m_fFFTMinFrequency;
	float			m_fFFTMaxFrequency;
	int				m_nAmpFrequency[8192];
	char			m_cArray[8192];
	CStringArray	m_strValues;
	CStringArray	m_Fields;
	CProbeDataSheet();
	virtual ~CProbeDataSheet();
};

class	CProbeScanData {
public:
	CPoint	m_ptEllipseTop;
	CPoint	m_ptEllipseBottom;
	CPoint	m_ptEllipseLeft;
	CPoint	m_ptEllipseRight;
	CPoint	m_ptCentre;
	float	FindBScanFocalZone(int nDropValue);
	float	FindCScanSpotSize(int nDropValue);
	int		FindPeak();
	int		m_nPeakValue;
	int		m_nSamplesLine;
	int		m_nNumberLines;
	float	m_fFastScanSize;
	float	m_fSlowScanSize;
	float	m_fFastIncrement;
	float	m_fSlowIncrement;
	float	m_fFocalLength;
	float	m_fDiameterSpot;
	float	m_fWidthSpot;
	float	m_fHeightSpot;
	float	m_fSymmetrySpot;
	CCoord	m_Cp[2];
	CCoord	m_CpCentre;
	CRect	m_rrArc;
	unsigned char	*m_cAmpBuffer;
	void Zero();
	bool Retrieve(CUSLFile* pFile);
	int Save(CUSLFile* pFile);
	void AllocateMemory();
	CProbeScanData();
	virtual ~CProbeScanData();

	void operator =(CProbeScanData &CProbeScan);

};

class CProbeBeamProfile
{
public:
	//BScan
	float FindFocalZoneOfAllBScans(float fAtAttenuation);
	int FindPeakOfAllBScans();
	int CalculateStatsFromBScans(float fAtAttenuation);
	void AddBScan(char *pArray, int nSamplesLine, int nNumberLines, float fFastSize, float fSlowSize, float fFastInc, float fSlowInc,CCoord &CpHead,CCoord &CpStart,CCoord &CpFinish);

	//Cscan
	float FindSpotSizeOfAllCScans(float fAtAttenuation);
	int FindPeakOfAllCScans();
	int CalculateStatsFromCScans(float fAtAttenuation);
	void AddCScan(char *pArray, int nSamplesLine, int nNumberLines, float fFastSize, float fSlowSize, float fFastInc, float fSlowInc,CCoord &CpHead,CCoord &CpStart,CCoord &CpFinish);

	void AllocateMemory();
	bool Retrieve(CUSLFile* pFile);
	int	Save(CUSLFile* pFile);
	void Zero();
	void DeleteAllBScans();
	void DeleteAllCScans();
	bool m_bModified;
	bool m_bLocked;
	float			m_fSpotSizeFromBScans;
	float			m_fSpotSizeFromCScans;
	int				m_nPeakFromBScans;
	int				m_nPeakFromCScans;
	int				m_nBScansL;
	CProbeScanData	*m_pBScans;
	int				m_nCScansL;
	CProbeScanData	*m_pCScans;
	int				m_osTime;
	TimeSlotData	m_TS;
	float			m_fNearDistance;
	float			m_fFarDistance;
	int				m_nNumberFields;
	CStringArray	m_strValues;
	CStringArray	m_Fields;
	CProbeBeamProfile();
	virtual ~CProbeBeamProfile();
};

class CProbe  
{
public:
	void InitializeFields();
	void AddBeamProfileScan(int nBscan_CScan,int nWhich,char* pArray,int nSamplesLine,int nNumberLines,float fFastSize,float fSlowSize,float fFastInc,float fSlowInc,CCoord &CpCentre);
	int	m_nNumberBeamProfiles;
	CProbeBeamProfile* m_pProbeBeamProfile[256];
	bool DeleteProbeDataSheet(int nWhich);
	int	m_nNumberDataSheets;
	CProbeDataSheet* m_pProbeDataSheet[256];
	CCoord m_Cp[2];
	bool m_bModified;
	bool CheckNewProfileAllowed();
	void Zero();
	CString m_FileName;
	int				m_nNumberFields;
	CStringArray	m_strValues;
	CStringArray	m_Fields;
	bool Retrieve(CUSLFile* pFile);
	int Save(CUSLFile *pFile);
	CProbe();
	virtual ~CProbe();

};

#endif // !defined(AFX_PROBE_H__20D1ABA6_5D0C_4807_BDDA_155C350B5A83__INCLUDED_)
