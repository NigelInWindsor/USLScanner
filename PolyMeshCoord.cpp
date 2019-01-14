// PolyMeshCoord.cpp: implementation of the CPolyMeshCoord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "PolyMeshCoord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPolyMeshCoord::CPolyMeshCoord()
{
	m_pCp = NULL;
	m_nRows = 0;
	m_nColumns = 0;
	m_bModified = false;

}

CPolyMeshCoord::~CPolyMeshCoord()
{
	SAFE_DELETE_ARRAY( m_pCp );

}

void CPolyMeshCoord::Zero()
{
	SAFE_DELETE_ARRAY( m_pCp );
	m_nRows = 0;
	m_nColumns = 0;
	m_bModified = false;

}

void CPolyMeshCoord::SetSize(int nColumns, int nRows)
{
	Zero();

	m_nRows = nRows;
	m_nColumns = nColumns;

	m_pCp = new COORDDATA[nRows * nColumns];
	ZeroMemory(m_pCp, nRows * nColumns * sizeof(COORDDATA));
}



bool CPolyMeshCoord::SetAt(int nColumn, int nRow, CCoord Cp)
{
	int nn;

	if((nColumn>=0) && (nColumn<m_nColumns) && (nRow>=0) && (nRow<m_nRows)) {
		nn = nRow * m_nColumns + nColumn;
		m_pCp[nn] = Cp;
		m_bModified = TRUE;

		return true;
	}
	return false;

}

void CPolyMeshCoord::BuildMesh(CCoord CpOffset, float fColumnLength, float fRowLength)
{
	CCoord Cp;
	int	nRow,nColumn;

	Cp = CpOffset;
	for(nRow=0;nRow<m_nRows;nRow++) {
		for(nColumn=0;nColumn<m_nColumns;nColumn++) {
			Cp.Side0.fX=((float)nColumn * fRowLength / (float)(m_nColumns-1)) + CpOffset.Side0.fX;
			Cp.Side0.fY=((float)nRow * fColumnLength / (float)(m_nRows-1)) + CpOffset.Side0.fY;

			SetAt(nColumn,nRow,Cp);
		}
	}
}

void CPolyMeshCoord::GetRow(int nRow, CPolyCoord *pLine)
{
	CCoord Cp;
	int nn;

	pLine->AllocateCoords(m_nColumns);
	for(int nColumn=0;nColumn<m_nColumns;nColumn++) {
		nn = nRow * m_nColumns + nColumn;
		Cp = m_pCp[nn];
		pLine->ModifyCoord(nColumn,Cp);
	}
}

void CPolyMeshCoord::GetColumn(int nColumn, CPolyCoord *pLine)
{
	CCoord Cp;
	int nn;

	pLine->AllocateCoords(m_nColumns);
	for(int nRow=0;nRow<m_nRows;nRow++) {
		nn = nRow * m_nColumns + nColumn;
		Cp = m_pCp[nn];
		pLine->ModifyCoord(nRow,Cp);
	}
}
