// EdgeArray.cpp: implementation of the CEdgeArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "EdgeArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEdgeArray::CEdgeArray()
{

	m_nCoordL = 0;
	m_pEdge = NULL;
}

CEdgeArray::~CEdgeArray()
{

	SAFE_DELETE( m_pEdge );
}

void CEdgeArray::Zero()
{

	SAFE_DELETE( m_pEdge );
	m_nCoordL=0;
}

int CEdgeArray::GetSize()
{

	return m_nCoordL;
}

void CEdgeArray::GetPt(int nIndex, int nNode, D3DXVECTOR3 *pPt)
{
	if(m_pEdge == NULL) return;
	if(nIndex>=0 && nIndex<m_nCoordL) {
		*pPt = m_pEdge[nIndex].v[nNode].pt;
	}

}

void CEdgeArray::Add(EDGE *pEdge)
{
	if(m_pEdge == NULL) {
		m_pEdge = new EDGE[m_nCoordL=1];
	} else {
		m_nCoordL++;
		m_pEdge = (EDGE *)realloc(m_pEdge,m_nCoordL * sizeof EDGE);
	}

	CopyMemory(&m_pEdge[m_nCoordL-1], pEdge, sizeof EDGE);
}

void CEdgeArray::Delete(int nIndex)
{
	int	nn;

	if((m_nCoordL>0) && (nIndex<m_nCoordL) && (nIndex>=0)) {
		m_nCoordL--;
		for(nn=nIndex; nn<m_nCoordL ;nn++) {
			m_pEdge[nn]=m_pEdge[nn+1];
		};
		if(m_nCoordL>0) {
			m_pEdge = (EDGE *)realloc(m_pEdge,m_nCoordL * sizeof EDGE);
		} else {
			SAFE_DELETE( m_pEdge ); 
		}
	}
}

bool CEdgeArray::AddIfShortest(EDGE *pEdge,int nStartSearchIndex)
{
	bool bNodeExists = false;

	if(m_pEdge == NULL) {
		Add(pEdge);
		return true;
	}


	for(int ii=nStartSearchIndex;ii<m_nCoordL;ii++) {
		//return if both nodes are the same
		if(((m_pEdge[ii].nNode[0]==pEdge->nNode[0]) && (m_pEdge[ii].nNode[1]==pEdge->nNode[1])) ||
			((m_pEdge[ii].nNode[0]==pEdge->nNode[1]) && (m_pEdge[ii].nNode[1]==pEdge->nNode[0])))
			return false;

		if(m_pEdge[ii].nNode[0]==pEdge->nNode[0]) {
			bNodeExists = true;
			//if first node is the same and new edge is shortest,replace
			if(m_pEdge[ii].fLength > pEdge->fLength) {
				CopyMemory(&m_pEdge[ii], pEdge, sizeof EDGE);
				return true;
			}
		}
	}
	if(bNodeExists == false) {
		Add(pEdge);
		return true;
	}
	return false;
}

void CEdgeArray::operator = (CEdgeArray* pEdge)
{

}




bool CEdgeArray::DoesEdgeExist(EDGE *pEdge)
{
	int ii;
	if(pEdge->nNode[0] == pEdge->nNode[1]) return true;
	for(ii=0;ii<m_nCoordL;ii++) {
		if(((m_pEdge[ii].nNode[0]==pEdge->nNode[0]) && (m_pEdge[ii].nNode[1]==pEdge->nNode[1])) ||
			((m_pEdge[ii].nNode[0]==pEdge->nNode[1]) && (m_pEdge[ii].nNode[1]==pEdge->nNode[0]))) return true;
	}
	return false;
}

void CEdgeArray::RemoveCrossingEdges()
{
	int	ii,jj;

	for(ii=0;ii<m_nCoordL;ii++) {
		for(jj=0;jj<m_nCoordL;jj++) {

			if((m_pEdge[ii].nNode[0] - m_pEdge[jj].nNode[0]) && (m_pEdge[ii].nNode[1] - m_pEdge[jj].nNode[1]) &&
				(m_pEdge[ii].nNode[1] - m_pEdge[jj].nNode[0]) && (m_pEdge[ii].nNode[0] - m_pEdge[jj].nNode[1])) {

				if(DoEdgesIntersect(ii,jj) ) {

					if(m_pEdge[ii].fLength < m_pEdge[jj].fLength) {
						Delete(jj);
						jj=40000;
					} else {
						Delete(ii);
						ii=-1;
					}
				}
			}
		}

	}

}

/*
   s10_x = p1_x - p0_x;
    s10_y = p1_y - p0_y;
    s02_x = p0_x - p2_x;
    s02_y = p0_y - p2_y;
	s_numer = s10_x * s02_y - s10_y * s02_x;
    if (s_numer < 0)        return 0; // No collision     
	s32_x = p3_x - p2_x;
    s32_y = p3_y - p2_y;
    t_numer = s32_x * s02_y - s32_y * s02_x;
    if (t_numer < 0)        return 0; // No collision     
	denom = s10_x * s32_y - s32_x * s10_y;
    if (s_numer > denom || t_numer > denom)        return 0; // No collision 
    // Collision detected    
	t = t_numer / denom;
    if (i_x != NULL)        *i_x = p0_x + (t * s10_x);
    if (i_y != NULL)        *i_y = p0_y + (t * s10_y);
*/
bool CEdgeArray::DoEdgesIntersect(int nIndex0, int nIndex1)
{
	float s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom, t;

    s10_x = m_pEdge[nIndex0].v[1].fX - m_pEdge[nIndex0].v[0].fX;
    s10_y = m_pEdge[nIndex0].v[1].fZ - m_pEdge[nIndex0].v[0].fZ;
    s02_x = m_pEdge[nIndex0].v[0].fX - m_pEdge[nIndex1].v[0].fX;
    s02_y = m_pEdge[nIndex0].v[0].fZ - m_pEdge[nIndex1].v[0].fZ;
	s_numer = s10_x * s02_y - s10_y * s02_x;
    if (s_numer < 0)        return false; // No collision     
	s32_x = m_pEdge[nIndex1].v[1].fX - m_pEdge[nIndex1].v[0].fX;
    s32_y = m_pEdge[nIndex1].v[1].fZ - m_pEdge[nIndex1].v[0].fZ;
    t_numer = s32_x * s02_y - s32_y * s02_x;
    if (t_numer < 0)        return false; // No collision     
	denom = s10_x * s32_y - s32_x * s10_y;
    if (s_numer > denom || t_numer > denom)        return false; // No collision 
    // Collision detected    
	t = t_numer / denom;
 //   if (i_x != NULL)        *i_x = m_pEdge[nIndex0].v[0].fX + (t * s10_x);
 //   if (i_y != NULL)        *i_y = m_pEdge[nIndex0].v[0].fY + (t * s10_y);
	return true;
}

void CEdgeArray::Sort(int nWhichNode)
{
	qsort(m_pEdge,m_nCoordL,sizeof EDGE,CompareEdges);
}

int CompareEdges(const void *Cp0,const void *Cp1)
{
	PEDGE Cl0 = (PEDGE)Cp0;
	PEDGE Cl1 = (PEDGE)Cp1;

	int nWhichNode = 0;

	int nn = Cl0->nNode[0] - Cl1->nNode[0];
	if(nn==0) {
		return Cl0->nNode[1] - Cl1->nNode[1];
	} else {
		return nn;
	}

}

int CEdgeArray::Save(CFile *pFile)
{
	int nStructSize = sizeof EDGE;
	int	nCoordOffset,nEof;
	int nFileOffset = (int)pFile->GetPosition();

	pFile->Write(&nCoordOffset,sizeof nCoordOffset);
	pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);
	pFile->Write(&m_nCoordL,sizeof m_nCoordL);
	pFile->Write(&nStructSize,sizeof nStructSize);

	nCoordOffset=(int)pFile->GetPosition();
	pFile->Write(m_pEdge,(sizeof EDGE) * m_nCoordL);
	nEof=(int)pFile->GetPosition();

	pFile->Seek(nFileOffset,CFile::begin);
	pFile->Write(&nCoordOffset,sizeof nCoordOffset);

	pFile->Seek(nEof,CFile::begin);

	return nFileOffset;
}

void CEdgeArray::Retrieve(CFile *pFile)
{
	float fVersion;
	int nStructSize,nCoordOffset;

	Zero();
	pFile->Read(&nCoordOffset,sizeof nCoordOffset);
	pFile->Read(&fVersion,sizeof fVersion);
	pFile->Read(&m_nCoordL, sizeof m_nCoordL);
	pFile->Read(&nStructSize, sizeof nStructSize);
	
	pFile->Seek(nCoordOffset,CFile::begin);
	if(m_nCoordL) {
		m_pEdge = new EDGE[m_nCoordL];
		pFile->Read((void*)m_pEdge,(sizeof EDGE) * m_nCoordL);
	}
}

