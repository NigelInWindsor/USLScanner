// EdgeArray.h: interface for the CEdgeArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDGEARRAY_H__17A714FA_14A0_48AB_A467_D3FF30B0A453__INCLUDED_)
#define AFX_EDGEARRAY_H__17A714FA_14A0_48AB_A467_D3FF30B0A453__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEdgeArray  
{
public:
	int Save(CFile *pFile);
	void Retrieve(CFile *pFile);
	void Sort(int nWhichNode);
	bool DoEdgesIntersect(int nIndex0, int nIndex1);
	void RemoveCrossingEdges();
	bool DoesEdgeExist(EDGE *pEdge);
	void operator = (CEdgeArray* pEdge);
	void Delete(int nIndex);
	void Add(EDGE *pEdge);
	bool AddIfShortest(EDGE *pEdge,int nStartSearchIndex =0);
	void GetPt(int nIndex, int nNode, D3DXVECTOR3 *pPt);
	int GetSize();
	void Zero();
	int	 m_nCoordL;
	EDGE* m_pEdge;
	CEdgeArray();
	virtual ~CEdgeArray();

};

int CompareEdges(const void *Cp0,const void *Cp1);

#endif // !defined(AFX_EDGEARRAY_H__17A714FA_14A0_48AB_A467_D3FF30B0A453__INCLUDED_)
