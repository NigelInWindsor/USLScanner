// PolyMeshCoord.h: interface for the CPolyMeshCoord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYMESHCOORD_H__CC1C3BD5_99CB_48E8_A371_04A1617DB787__INCLUDED_)
#define AFX_POLYMESHCOORD_H__CC1C3BD5_99CB_48E8_A371_04A1617DB787__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPolyMeshCoord  
{
public:
	void GetRow(int nRow,CPolyCoord *pLine);
	void GetColumn(int nColumn,CPolyCoord *pLine);
	void BuildMesh(CCoord CpOffset,float fColumnLength,float fRowLength);
	bool SetAt(int nColumn, int nRow, CCoord Cp);
	void SetSize(int nColumns,int nRows);
	void Zero();
	COORDDATA	*m_pCp;
	int			m_nRows;
	int			m_nColumns;
	bool		m_bModified;
	CPolyMeshCoord();
	virtual ~CPolyMeshCoord();

};

#endif // !defined(AFX_POLYMESHCOORD_H__CC1C3BD5_99CB_48E8_A371_04A1617DB787__INCLUDED_)
