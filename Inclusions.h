// Inclusions.h: interface for the CInclusions class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INCLUSIONS_H__E572BB01_3D40_4C23_97A2_CD9C2B225A6D__INCLUDED_)
#define AFX_INCLUSIONS_H__E572BB01_3D40_4C23_97A2_CD9C2B225A6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct	DefectStruct {
	int	nStructSize;

	int	nXs;
	int	nYs;
	int	nX[2];
	int	nY[2];
	int	nAmp;
	int	nCount;
	float fXp;
	float fYp;
	float fDepth;
	float fFBH;
	float fGain;
	float fLength;
	int	nClass;
	int	nConfirmed;
	char String[50];

	int	nSpare[20];
};


class CInclusions
{

public:
	void SaveToTextFile(CUSLFile* pFile, PVOID pVoidData);
	bool BuildInclusionData(PVOID pVoidData, int nImageType,int nImageNumber,CRect rrSample);
	int GetComponentShape(PVOID pVoidData);
	CString GetComponentShapeVerbose(PVOID pVoidData);
	DefectStruct* GetPtrDefect(int nIndex);
	bool IsDefects();
	int GetDefectSize();
	CInclusions();
	virtual ~CInclusions();

	DefectStruct*	m_pDef;
	int		m_nDefL;

	CRect m_rrArea;

	float m_fTSA;
	float m_fTestMass;
	float m_fWeight[8];
	float m_fInclusionLength;
	float m_fVolume;
	float m_fTEDA;
	float m_fTotalLength;
	
	int	m_osRefDate;
	int	m_osTestDate;

	float	CalculateVolume(PVOID pVoidData);
	float	CalculateTotalEquivelantDefectArea(PVOID pVoidData);
	float	CalculateMass(PVOID pVoidData);
	float	CalculateTSA(PVOID pVoidData);
	void	SetClass(DefectStruct* pDef);
	void	AddDefect(DefectStruct NewDef);
	void	DeleteDefect(int nIndex);
	void	CalculateAllDefects();
	void	OrigionalCalculateAllDefects();

	CString	m_ProbeName;
	CString	m_RefGain;
	CString	m_TestGain;
	CString	m_RefSensitivityFBH;
	CString	m_TestSensitivityFBH;
	CString	m_RefFBH;
	CString	m_RefStandard;
	CString	m_FocalLength;
	CString	m_FocalDepth;
	CString m_Cast;
	CString m_Customer;
	CString	m_Grade;
	CString	m_OrderNumber;
	CString m_SampleRef;
	CString m_Probe;
	CString m_RefDate;
	CString m_TestDate;


	UINT64 Save(CUSLFile * pFile);
	void  Retrieve(CUSLFile* pFile);
	void operator = (CInclusions* pInclusions);
};

#endif // !defined(AFX_INCLUSIONS_H__E572BB01_3D40_4C23_97A2_CD9C2B225A6D__INCLUDED_)
