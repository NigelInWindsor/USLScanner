// IGES.h: interface for the CIGES class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGES_H__099185EF_5528_48F7_8A6B_36162423A08A__INCLUDED_)
#define AFX_IGES_H__099185EF_5528_48F7_8A6B_36162423A08A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "USLFile.h"


class CIGES  
{
public:
	void GenerateTransforms();
	bool ConicArcLimits(EntityStruct *pEntity,D3DXVECTOR3* pvecMin,D3DXVECTOR3* pvecMax);
	bool CircleArcLimits(EntityStruct *pEntity,D3DXVECTOR3* pvecMin,D3DXVECTOR3* pvecMax);
	bool LineLimits(EntityStruct *pEntity,D3DXVECTOR3* pvecMin,D3DXVECTOR3* pvecMax);
	bool PointLimits(EntityStruct *pEntity,D3DXVECTOR3* pvecMin,D3DXVECTOR3* pvecMax);
	bool Format(EntityStruct *pEntity,CString *pBuff);
	void GenerateGlobalFields();

	void FillEntityWithData(int nEntity);
	void AddParameterDataToEntity(int nEntityPtr,char *Line,CUSLFile* pFile,int nLineLength);
	void AddIGESEntity(char *Line0,char *Line1);
	void Zero();
	bool Retrieve(CUSLFile *pFile);

	EntityStruct* m_pEntity;
	int	m_nEntityL;
	char m_cParameterDelimiter;
	char m_cRecordDelimiter;
	float m_fMul;
	float m_fMaxSize;
	
	CString m_EntityType[500];
	CString	m_PathName;
	CString m_FileName;
	CStringArray m_strStart;
	CStringArray m_strGlobal;
	CStringArray m_strDirectory;
	CStringArray m_strEntity;
	CStringArray m_strTerminate;

	CStringArray m_strGlobalFields;

	CIGES();
	virtual ~CIGES();

};

#endif // !defined(AFX_IGES_H__099185EF_5528_48F7_8A6B_36162423A08A__INCLUDED_)
