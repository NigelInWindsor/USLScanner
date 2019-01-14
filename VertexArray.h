// VertexArray.h: interface for the CUSLVertexArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERTEXARRAY_H__D74F6C25_00F3_4C91_A591_E695D719C17A__INCLUDED_)
#define AFX_VERTEXARRAY_H__D74F6C25_00F3_4C91_A591_E695D719C17A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CVertexArray  
{
public:
	void DeleteFirstSamples(int nNumber);
	void DeleteLastSamples(int nNumber);
	void Insert(CVertexArray* pVertex, int nInsertIndex);
	void Append(CVertexArray* pVertex);
	void GetCoord(int nIndex,CCoord *pCp,int nSide);
	void GetVertex(float fFractionOfLength,CUSLVertex *pVertex, int *pnIndex);
	CString GetPt(int nIndex, bool bVerbose);
	float FindNearestVertex(CUSLVertex vertex, int *pnSegment,int nDir);
	float FindNearestSegment(CUSLVertex vertex, int *pnSegment);
	float FindNearestSegment(D3DXVECTOR3 vec, int *pnSegment);
	float FindNearestSegment(CCoord Cp,int nSide,int *pnSegment);
	BOOL SetPt(int nIndex, D3DXVECTOR3 *pPt);
	void TransformLine(D3DXMATRIXA16 matWorld);
	void GetVectorLengths(D3DXVECTOR3 *pVec);
	void CalculateAngle(float *pfAngle);
	void CalculateLength(float *pfLength);
	bool CalculateCircumCenter(float *fRadius, D3DXVECTOR3 *pVec = NULL, float *fAngle0 = NULL, float *fAngle1 = NULL, D3DXVECTOR3 *pNorm = NULL);
	void GetNormalToTwoVectors(D3DXVECTOR3 *pvNorm);
	float GetRotateZAngle();
	BOOL GetVertex(int nIndex,CUSLVertex *pPt);
	BOOL GetPt(int nIndex,D3DXVECTOR3 *pPt);
	int GetSize();
	UINT64 GetSpaceTakenInFile();
	float DistanceToSegment(CCoord Cp, int nSide, int nSegment);
	void SetCoord(int nIndex,CCoord *Cp, int nSide);
	void ModifyCoord(int nIndex,CCoord Cp);
	void 	operator = (CPolyCoord* pCp);
	void	operator = (CVertexArray* pVertex);
	void Delete(int nStartIndex, int nFinishIndex);
	void Add(D3DXVECTOR3 *pVec);
	void SetSize(int nNewSize, int nGrowBy = -1);
	void RemoveAll();
	void Retrieve(CUSLFile* pFile);
	ULONGLONG Save(CUSLFile* pFile);
	void Zero();
	void GetVertices(CStringArray &StringArray);
	void GetVertices(CPolyCoord &Coordinates, int nNumberPerLine=-1);

	CUSLVertex	*m_pVp;
	int		m_nCoordL;
	int		m_nFirstSample;
	int		m_nMaxAccessed;
	int		m_nAllocatedSize;

	VertexArrayType		m_Type;

	CVertexArray();
	virtual ~CVertexArray();

	void AllocateCoords(int nCoordsToAllocate);
	void AllocateCoordMemory(int nCoordsToAllocate);
	void ReallocateCoords(int nNewL);
	void DeleteAllCoords();

};

#endif // !defined(AFX_VERTEXARRAY_H__D74F6C25_00F3_4C91_A591_E695D719C17A__INCLUDED_)
