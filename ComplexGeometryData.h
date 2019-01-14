// ComplexGeometryData.h: interface for the CComplexGeometryData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPLEXGEOMETRYDATA_H__34A7338B_1B09_4610_9A38_70E3403A4027__INCLUDED_)
#define AFX_COMPLEXGEOMETRYDATA_H__34A7338B_1B09_4610_9A38_70E3403A4027__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CComplexGeometryData
{
public:
	bool Retrieve(CUSLFile* pFile,CData* pData);
	CComplexGeometryData();
	virtual ~CComplexGeometryData();

};

#endif // !defined(AFX_COMPLEXGEOMETRYDATA_H__34A7338B_1B09_4610_9A38_70E3403A4027__INCLUDED_)
