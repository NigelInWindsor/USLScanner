// VertexArray.cpp: implementation of the CVertexArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "VertexArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVertexArray::CVertexArray()
{
	m_nMaxAccessed = 0;
	m_nAllocatedSize = 0;
	m_nCoordL=0;
	m_pVp=NULL;

	m_nFirstSample = 0;
}

CVertexArray::~CVertexArray()
{
	m_nAllocatedSize = m_nCoordL = 0;
	SAFE_DELETE_ARRAY(m_pVp);
}

void CVertexArray::AllocateCoords(int nCoordsToAllocate)
{
	AllocateCoordMemory(nCoordsToAllocate);
	m_nCoordL = nCoordsToAllocate;
}

void CVertexArray::AllocateCoordMemory(int nCoordsToAllocate)
{
	SAFE_DELETE_ARRAY(m_pVp);
	if (nCoordsToAllocate>0) m_pVp = new CUSLVertex[m_nAllocatedSize = nCoordsToAllocate];
	m_nMaxAccessed = 0;
	m_nAllocatedSize = nCoordsToAllocate;
}

void CVertexArray::ReallocateCoords(int nNewL)
{
	CUSLVertex *pCpTemp;

	if (nNewL == 0) {
		DeleteAllCoords();
	}
	else {
		if (m_pVp == NULL) {
			AllocateCoordMemory(nNewL);
		}
		else {
			int oldAllocatedSize = m_nAllocatedSize;
			pCpTemp = new CUSLVertex[oldAllocatedSize];
			CopyMemory(pCpTemp, m_pVp, oldAllocatedSize * sizeof CUSLVertex);
			if (m_pVp) delete[] m_pVp;

			int lengthToCopy = oldAllocatedSize;
			if (nNewL < oldAllocatedSize) { //Reducing the length.
				lengthToCopy = nNewL;
				m_nCoordL = nNewL;
			}
			m_pVp = new CUSLVertex[m_nCoordL = m_nAllocatedSize = nNewL];
			CopyMemory(m_pVp, pCpTemp, lengthToCopy * sizeof CUSLVertex);
			delete[] pCpTemp;
		}
	}
}

void CVertexArray::DeleteAllCoords()
{
	SAFE_DELETE_ARRAY(m_pVp);
	m_nAllocatedSize = m_nCoordL = 0;
}

void CVertexArray::Zero()
{
	m_nAllocatedSize = m_nCoordL = 0;
	SAFE_DELETE_ARRAY(m_pVp);
}


ULONGLONG CVertexArray::Save(CUSLFile *pFile)
{
	int nStructSize = sizeof CUSLVertex;
	ULONGLONG n64CoordOffset,n64Eof;
	ULONGLONG n64FileOffset = pFile->GetPosition();

	pFile->Write(&n64CoordOffset,sizeof n64CoordOffset);
	pFile->Write(&m_nCoordL,sizeof m_nCoordL);
	pFile->Write(&nStructSize,sizeof nStructSize);
	pFile->Write(&m_nFirstSample,sizeof m_nFirstSample);
	pFile->Write(&m_Type,sizeof m_Type);

	n64CoordOffset=pFile->GetPosition();
	int nArraySize = m_nCoordL * sizeof(CUSLVertex);
	pFile->Write(m_pVp, nArraySize);

	n64Eof=pFile->GetPosition();

	pFile->Seek(n64FileOffset,CFile::begin);
	pFile->Write(&n64CoordOffset,sizeof n64CoordOffset);

	pFile->Seek(n64Eof,CFile::begin);

	return n64FileOffset;
}

void CVertexArray::Retrieve(CUSLFile *pFile)
{
	int nStructSize;
	UINT64 n64CoordOffset;

	RemoveAll();
	pFile->Read(&n64CoordOffset,sizeof n64CoordOffset);
	pFile->Read(&m_nCoordL, sizeof m_nCoordL);
	pFile->Read(&nStructSize, sizeof nStructSize);
	pFile->Read(&m_nFirstSample,sizeof m_nFirstSample);
	pFile->Read(&m_Type,sizeof m_Type);
	
	if(n64CoordOffset >= 0  && n64CoordOffset< pFile->GetLength() && m_nCoordL > 0 && m_nCoordL<20000) {
		pFile->Seek(n64CoordOffset,CFile::begin);
		SetSize(m_nCoordL);

		if (pFile->m_fVersion <= 5.00f) {
			for (int ii = 0; ii < m_nCoordL; ii++)
				m_pVp[ii].Retrieve(pFile);
		}
		else {
			int nArraySize = m_nCoordL * sizeof(CUSLVertex);
			pFile->Read(m_pVp, nArraySize);
		}
	}
	else {
		m_nCoordL = 0;
		m_nFirstSample = 0;
		m_Type = (VertexArrayType)0;
	}
}

void CVertexArray::GetVertices(CStringArray &StringArray)
{
	CString Buff;
	StringArray.RemoveAll();

	for(int ii=0; ii<m_nCoordL; ii++) {
		Buff.Format(_T("%0.03f \t%0.03f \t%0.03f\t %0.5f \t%0.5f \t%0.5f"),m_pVp[ii].fX,m_pVp[ii].fY,m_pVp[ii].fZ,m_pVp[ii].fI,m_pVp[ii].fJ,m_pVp[ii].fK);
		StringArray.Add(Buff);
	}

}

void CVertexArray::GetVertices(CPolyCoord &Coordinates, int nNumberPerLine)
{
	CString Buff;
	int ii,jj;
	CCoord Cp;

	if(nNumberPerLine==-1) nNumberPerLine=m_nCoordL;

	Coordinates.Zero();

	if(m_nCoordL>0) {
		for(ii=0,jj=0; ii<m_nCoordL; ii+=(m_nCoordL/nNumberPerLine),jj++) {
			Cp.Side0.fX = Cp.Side1.fX = m_pVp[ii].fX;
			Cp.Side0.fY = Cp.Side1.fY = m_pVp[ii].fY;
			Cp.Side0.fZ = Cp.Side1.fZ = m_pVp[ii].fZ;
			Cp.Side0.fI = Cp.Side1.fI = m_pVp[ii].fI;
			Cp.Side0.fJ = Cp.Side1.fJ = m_pVp[ii].fJ;
			Cp.Side0.fK = Cp.Side1.fK = m_pVp[ii].fK;
			Coordinates.Add(Cp);
		}

		//Always capture last point
		if(ii>(m_nCoordL-1)) {
			ii=m_nCoordL-1;
			Cp.Side0.fX = Cp.Side1.fX = m_pVp[ii].fX;
			Cp.Side0.fY = Cp.Side1.fY = m_pVp[ii].fY;
			Cp.Side0.fZ = Cp.Side1.fZ = m_pVp[ii].fZ;
			Cp.Side0.fI = Cp.Side1.fI = m_pVp[ii].fI;
			Cp.Side0.fJ = Cp.Side1.fJ = m_pVp[ii].fJ;
			Cp.Side0.fK = Cp.Side1.fK = m_pVp[ii].fK;
			Coordinates.Add(Cp);
		}

	}

}


void CVertexArray::RemoveAll()
{
	SAFE_DELETE_ARRAY(m_pVp);
	m_nAllocatedSize = m_nCoordL = 0;
	m_nFirstSample=0;
}

void CVertexArray::Add(D3DXVECTOR3 *pVec)
{

	if(m_nCoordL==0) {
		SetSize(1);
	} else {
		SetSize(0,1);
	}
	m_pVp[m_nCoordL-1].pt = *pVec;
	m_pVp[m_nCoordL-1].norm = D3DXVECTOR4(0.0f,0.0f,1.0f,0.0f);

}

void CVertexArray::Delete(int nStartIndex, int nFinishIndex)
{
	int	nn,nLength;
	nLength = nFinishIndex-nStartIndex+1;
	for(nn=nStartIndex;nn<m_nCoordL-nLength;nn++ ) {
		m_pVp[nn]=m_pVp[nn+nLength];
	}
	m_nCoordL -= nLength;
	if(m_nCoordL>0) {
		ReallocateCoords(m_nCoordL);
	} else {
		m_nAllocatedSize = m_nCoordL = 0;
		SAFE_DELETE_ARRAY(m_pVp);
	}

}

void CVertexArray::SetSize(int nNewSize, int nGrowBy)
{
	if(m_pVp==NULL) {
		AllocateCoords(nNewSize);
	} else {
		int nOldSize = m_nCoordL;
		if(nGrowBy>-1) {
			ReallocateCoords(m_nCoordL + nGrowBy);
		}
		else {
			if (nNewSize > 0) {
				ReallocateCoords(nNewSize);
			}
			else {
				RemoveAll();
				return;
			}
		}
	}
}

void CVertexArray::operator = (CPolyCoord* pCp)
{
	m_nCoordL = pCp->m_nCoordL;

	if(m_pVp==NULL) {
		AllocateCoords(m_nCoordL);
	} else {
		ReallocateCoords(m_nCoordL);
	};

	for(int ii=0;ii<m_nCoordL;ii++) {
		m_pVp[ii].pt = pCp->m_pCp[ii].Side0.pt;
		m_pVp[ii].norm = pCp->m_pCp[ii].Side0.norm;
	}
}

void CVertexArray::operator = (CVertexArray* pVertex)
{
	m_nCoordL = pVertex->m_nCoordL;
	m_nFirstSample = pVertex->m_nFirstSample;
	m_Type = pVertex->m_Type;

	if(m_pVp==NULL) {
		AllocateCoords(m_nCoordL);
	} else {
		ReallocateCoords(m_nCoordL);
	};

	for(int ii=0;ii<m_nCoordL;ii++) {
		m_pVp[ii] = pVertex->m_pVp[ii];
	}
}

void CVertexArray::Append(CVertexArray* pVertex)
{
	if(pVertex == NULL) return;
	int nOldCoordL = m_nCoordL;
	m_nCoordL += pVertex->m_nCoordL;

	if(m_pVp==NULL) {
		AllocateCoords(m_nCoordL);
	} else {
		ReallocateCoords(m_nCoordL);
	};

	CopyMemory(&m_pVp[nOldCoordL], &pVertex->m_pVp[0], (sizeof CUSLVertex) * pVertex->m_nCoordL);
}

void CVertexArray::Insert(CVertexArray* pVertex, int nInsertIndex)
{
	int ii,nStart;
	int nOldCoordL = m_nCoordL;
	if(nInsertIndex<0) return;

	if(nInsertIndex<m_nCoordL) {
		m_nCoordL += pVertex->m_nCoordL;
	} else {
		m_nCoordL = nInsertIndex + pVertex->m_nCoordL;
	}

	if(m_pVp==NULL) {
		AllocateCoords(m_nCoordL);
	} else {
		ReallocateCoords(m_nCoordL);
	};

	CUSLVertex *pVp = new CUSLVertex[m_nCoordL];
	for(ii=0;(ii<nInsertIndex) && (ii<nOldCoordL);ii++) {
		pVp[ii] = m_pVp[ii];
	}
	nStart = ii;

	for(ii=0;ii<pVertex->m_nCoordL;ii++) {
		pVp[nStart + ii] = pVertex->m_pVp[ii];
	}
	nStart = nStart+ii;

	for(ii=nInsertIndex;ii<nOldCoordL;ii++) {
		pVp[nStart + ii] = m_pVp[ii];
	}
	for(ii=0;ii<m_nCoordL;ii++) {
		m_pVp[ii] = pVp[ii];
	}
	SAFE_DELETE_ARRAY( pVp );

}

void CVertexArray::ModifyCoord(int nIndex, CCoord Cp)
{
	if(nIndex>=0 && nIndex <m_nCoordL) {
		m_pVp[nIndex].pt = Cp.Side0.pt;
		m_pVp[nIndex].norm = Cp.Side0.norm;
	}

}

void CVertexArray::SetCoord(int nIndex, CCoord *Cp, int nSide)
{
	if(nIndex>=0 && nIndex <m_nCoordL) {
		m_pVp[nIndex].pt = Cp->Side[nSide].pt;
		m_pVp[nIndex].norm = Cp->Side[nSide].norm;
	}

}

void CVertexArray::GetCoord(int nIndex, CCoord *pCp, int nSide)
{
	if(nIndex>=0 && nIndex <m_nCoordL) {
		pCp->Side[nSide].pt = m_pVp[nIndex].pt;
		pCp->Side[nSide].norm.x = m_pVp[nIndex].norm.x;
		pCp->Side[nSide].norm.y = m_pVp[nIndex].norm.y;
		pCp->Side[nSide].norm.z = m_pVp[nIndex].norm.z;
	}

}

BOOL CVertexArray::GetPt(int nIndex,D3DXVECTOR3 *pPt)
{
	if(nIndex>=0 && nIndex <m_nCoordL) {
		*pPt = m_pVp[nIndex].pt;
		return TRUE;
	}
	return FALSE;
}

BOOL CVertexArray::SetPt(int nIndex, D3DXVECTOR3 *pPt)
{
	if(nIndex>=0 && nIndex <m_nCoordL) {
		m_pVp[nIndex].pt = *pPt;
		return TRUE;
	}
	return FALSE;

}

BOOL CVertexArray::GetVertex(int nIndex, CUSLVertex *pVt)
{
	if(nIndex>=0 && nIndex <m_nCoordL) {
		*pVt = m_pVp[nIndex];
		return TRUE;
	}
	return FALSE;
}

float CVertexArray::FindNearestSegment(D3DXVECTOR3 vec, int *pnSegment)
{
	CCoord Cp;

	Cp.Side0.pt = vec;
	return	FindNearestSegment(Cp,0,pnSegment);
}

float CVertexArray::FindNearestSegment(CUSLVertex USLVERTEX, int *pnSegment)
{
	CCoord Cp;

	Cp.Side0.pt = USLVERTEX.pt;
	return	FindNearestSegment(Cp,0,pnSegment);
}

float CVertexArray::FindNearestSegment(CCoord Cp, int nSide, int *pnSegment)
{
	
	float fLengthHi,fLengthLo,fLength;
	int k,kLo,kHi;

	fLength = 1000000000.0f;
	*pnSegment=0;
	kHi=m_nCoordL-1;
	kLo=0;
	fLengthHi = DistanceToSegment(Cp,nSide,kHi);
	fLengthLo = DistanceToSegment(Cp,nSide,kLo);
	while((kHi-kLo)>1) {
		k=(kHi+kLo)/2;
		if(fLengthHi > fLengthLo) {
			if((kHi-k) > 2) {
				kHi = (kHi+k)/2;
			} else {
				kHi=k;
			}
			*pnSegment=kHi;
			fLengthHi = DistanceToSegment(Cp,nSide,kHi);
		} else {
			if((k-kLo) > 2) {
				kLo = (k+kLo)/2;
			} else {
				kLo=k;
			}
			*pnSegment=kLo;
			fLengthLo = DistanceToSegment(Cp,nSide,kLo);
		}
	}
	if(fLength > fLengthHi) fLength = fLengthHi;
	if(fLength > fLengthLo) fLength = fLengthLo;
	return fLength;

}

float CVertexArray::FindNearestVertex(CUSLVertex vertex, int *pnSegment,int nDir) 
{
	float fLengthHi,fLengthLo,fLength,fTemp;
	int k,kLo,kHi;
	D3DXVECTOR3 W;

	fLength = 1000000000.0f;
	*pnSegment=0;
	kHi=m_nCoordL-1;
	kLo=0;


	if(nDir == 0) { 
		for(k=0;k<m_nCoordL;k++) {
			fTemp = D3DXVec3Length(&(vertex.pt - m_pVp[k].pt));
			if(fTemp < fLength) {
				fLength = fTemp;
				*pnSegment = k;
			}
		}
	} else {
		for(k=m_nCoordL-1;k>0;k--) {
			fTemp = D3DXVec3Length(&(vertex.pt - m_pVp[k].pt));
			if(fTemp < fLength) {
				fLength = fTemp;
				*pnSegment = k;
			}
		}
	}
	return fLength;



	while((kHi-kLo)>4) {
		k=(kHi+kLo)/2;
		fLength = D3DXVec3Length(&(vertex.pt - m_pVp[k].pt));

		if(fLengthHi > fLengthLo) {
			if(fLength > fLengthLo) {
				kHi=k;
			} else {
				kHi = (kHi+k)/2;
			}
			fLengthHi = D3DXVec3Length(&(vertex.pt - m_pVp[kHi].pt));
		} else {
			if(fLength > fLengthHi) {
				kLo=k;
			} else {
				kLo = (kLo+k)/2;
			}
			fLengthLo = D3DXVec3Length(&(vertex.pt - m_pVp[kLo].pt));
		}
	}
	*pnSegment = k;
	if(fLength > fLengthHi) fLength = fLengthHi;
	if(fLength > fLengthLo) fLength = fLengthLo;
	return fLength;
}

float CVertexArray::DistanceToSegment(CCoord Cp, int nSide, int nSegment)
{
	D3DXVECTOR3 V,W;
	int ii=nSegment;
	int jj=ii+1;

	if(nSegment>=(m_nCoordL-1)) {
		W = Cp.Side[nSide].pt - m_pVp[ii].pt;
		return D3DXVec3Length(&W);

	} else {

		V = m_pVp[jj].pt - m_pVp[ii].pt;
		W = Cp.Side[nSide].pt - m_pVp[ii].pt;

		float fC1 = D3DXVec3Dot(&W, &V);
		if(fC1<=0) {
			return D3DXVec3Length(&W);
		}

		float fC2 = D3DXVec3Dot(&V, &V);
		if(fC2<=fC1) {
			W = Cp.Side[nSide].pt - m_pVp[jj].pt;
			return D3DXVec3Length(&W);
		}

		float fB = fC1 / fC2;
		V = V * fB;
		D3DXVECTOR3 Pb = m_pVp[ii].pt + V;
		W = Cp.Side[nSide].pt - Pb;
		return D3DXVec3Length(&W);
	}

	return -1.0f;

}

UINT64 CVertexArray::GetSpaceTakenInFile()
{
	UINT64 n64Size = 0;
	UINT64 n64CoordOffset = 0;

	n64Size += (UINT64)(sizeof n64CoordOffset);
	n64Size += (UINT64)(sizeof m_nCoordL);
	n64Size += (UINT64)(sizeof CUSLVertex);
	n64Size += (UINT64)(sizeof m_nFirstSample);
	n64Size += (UINT64)((sizeof CUSLVertex) * m_nCoordL);

	return n64Size;
}

int CVertexArray::GetSize()
{
	return m_nCoordL;

}

float CVertexArray::GetRotateZAngle()
{

	return atan2f(m_pVp[1].pt.y-m_pVp[0].pt.y, m_pVp[1].pt.x-m_pVp[0].pt.x);
}

void CVertexArray::GetNormalToTwoVectors(D3DXVECTOR3 *pvNorm)
{
	D3DXVECTOR3 vOA,vOB,vNorm;

	vOA = m_pVp[0].pt - m_pVp[1].pt;
	vOB = m_pVp[2].pt - m_pVp[1].pt;

	D3DXVec3Normalize( &vOA, &vOA );
	D3DXVec3Normalize( &vOB, &vOB );
	D3DXVec3Cross(&vNorm, &vOB, &vOA);
	D3DXVec3Normalize( pvNorm, &vNorm );

}

bool CVertexArray::CalculateCircumCenter(float *fRadius, D3DXVECTOR3 *pVec, float *fAngle0, float *fAngle1, D3DXVECTOR3 *pNorm)
{
	float fNumerator,fDenominator,fA,fB,fC;
	D3DXVECTOR3 vAB, vBC, vAC,vNorm, vOpposite,vACentre,vAO,vBO,vCO;

	if(m_nCoordL < 3) return false;

	//CircumRadius

	vAB = m_pVp[1].pt - m_pVp[0].pt;
	vBC = m_pVp[2].pt - m_pVp[1].pt;
	vAC = m_pVp[2].pt - m_pVp[0].pt;

	fA = D3DXVec3Length(&vAB);
	fB = D3DXVec3Length(&vBC);
	fC = D3DXVec3Length(&vAC);

	fNumerator = fA * fB * fC;
	fDenominator = sqrtf( (fA+fB+fC) * (fB+fC-fA) * (fC+fA-fB) * (fA+fB-fC) );
	*fRadius = fNumerator / fDenominator;


	//Circum center
	D3DXVec3Cross(&vNorm, &vAB, &vAC);
	D3DXVec3Normalize( &vNorm, &vNorm );

	if(pNorm) {
		*pNorm = vNorm;
		if(pNorm->z <0.0f) {
			pNorm->x *= -1.0f;
			pNorm->y *= -1.0f;
			pNorm->z *= -1.0f;
		}
	}

	D3DXVec3Cross(&vOpposite, &vNorm, &vAB);
	D3DXVec3Normalize( &vOpposite, &vOpposite );

	float fHalfA = fA / 2.0f;
	float fLengthOpposite = sqrtf((*fRadius * *fRadius) - (fHalfA * fHalfA));

	vACentre = (m_pVp[1].pt + m_pVp[0].pt) / 2.0f;
	*pVec = (vOpposite * fLengthOpposite) + vACentre;

	if(fAngle0 != NULL) {
		vAO = m_pVp[0].pt - *pVec;
		vBO = m_pVp[1].pt - *pVec;
		D3DXVec3Normalize( &vAO, &vAO );
		D3DXVec3Normalize( &vBO, &vBO );
		*fAngle0 = acosf(D3DXVec3Dot(&vAO, &vBO));
	}
	if(fAngle0 != NULL) {
		vCO = m_pVp[2].pt - *pVec;
		vBO = m_pVp[1].pt - *pVec;
		D3DXVec3Normalize( &vCO, &vCO );
		D3DXVec3Normalize( &vBO, &vBO );
		*fAngle1 = acosf(D3DXVec3Dot(&vCO, &vBO));

	}
	return true;
}


void CVertexArray::CalculateLength(float *pfLength)
{
	D3DXVECTOR3 vLine;

	*pfLength = 0.0f;
	if(m_nCoordL>=2) {
		for(int ii=0;ii<(m_nCoordL-1);ii++) {
			vLine = m_pVp[ii+1].pt-m_pVp[ii].pt;
			*pfLength += D3DXVec3Length(&vLine);
		}
	}
}

void CVertexArray::CalculateAngle(float *pfAngle)
{
	D3DXVECTOR3 vAO,vBO;

	*pfAngle = 0.0f;
	if(m_nCoordL < 3) return;

	vAO = m_pVp[0].pt - m_pVp[1].pt;
	vBO = m_pVp[2].pt - m_pVp[1].pt;
	D3DXVec3Normalize( &vAO, &vAO );
	D3DXVec3Normalize( &vBO, &vBO );
	*pfAngle = acosf(D3DXVec3Dot(&vAO, &vBO));

}

void CVertexArray::GetVectorLengths(D3DXVECTOR3 *pVec)
{
	if(m_nCoordL>=2) {
		*pVec = m_pVp[1].pt-m_pVp[0].pt;
	} else {
		*pVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

void CVertexArray::TransformLine(D3DXMATRIXA16 matWorld)
{
	
	D3DXMATRIXA16 matRotate = matWorld;
	matRotate._41 = 0.0f;
	matRotate._42 = 0.0f;
	matRotate._43 = 0.0f;
	matRotate._44 = 1.0f;

	for(int ii=0;ii<m_nCoordL;ii++) {
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_pVp[ii].pt,		(D3DXVECTOR3*)&m_pVp[ii].pt,	&matWorld);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_pVp[ii].norm,	(D3DXVECTOR3*)&m_pVp[ii].norm,	&matRotate);
	}

}



CString CVertexArray::GetPt(int nIndex, bool bVerbose)
{
	CString Buff;

	Buff.Empty();
	if(nIndex>=0 && nIndex <m_nCoordL) {
		switch(bVerbose) {
		case false: Buff.Format(L"%.01f %.01f %.01f",m_pVp[nIndex].pt.x,m_pVp[nIndex].pt.y,m_pVp[nIndex].pt.z);
			break;
		case true: Buff.Format(L"X:%.01f Y:%.01f Z:%.01f",m_pVp[nIndex].pt.x,m_pVp[nIndex].pt.y,m_pVp[nIndex].pt.z);
			break;
		}
	}
	return Buff;
}

void CVertexArray::GetVertex(float fFractionOfLength, CUSLVertex *pVertex, int *pnIndex)
{
	int nIndex;

	*pnIndex = nIndex = (int)(fFractionOfLength * (float)(GetSize()-1));
	MinMax(&nIndex,0,GetSize()-1);

	CopyMemory(pVertex, &m_pVp[nIndex], sizeof CUSLVertex);

}


void CVertexArray::DeleteLastSamples(int nNumber)
{
	m_nCoordL -= nNumber;
	if(m_nCoordL>0) {
		ReallocateCoords(m_nCoordL);
	} else {
		m_nAllocatedSize = m_nCoordL = 0;
		SAFE_DELETE_ARRAY(m_pVp);
	}

}

void CVertexArray::DeleteFirstSamples(int nNumber)
{

	m_nCoordL -= nNumber;
	if(m_nCoordL>0) {
		CopyMemory(&m_pVp[0],&m_pVp[nNumber],m_nCoordL * sizeof CUSLVertex);
		ReallocateCoords(m_nCoordL);
	} else {
		m_nAllocatedSize = m_nCoordL = 0;
		SAFE_DELETE_ARRAY(m_pVp);
	}
}
