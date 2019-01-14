// PolyCoord.cpp: implementation of the CPolyCoord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "PolyCoord.h"
#include "CoordV3.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPolyCoord::CPolyCoord()
{
	m_nCoordL=0;
	m_nKnotsL=0;
	m_nCurveOrder=0;
	m_bModified=FALSE;
	m_nStyle=0;
	m_nPrimaryAxis=0;
	m_fLinearLength=0.0;
	m_nLineIncrement = 0;
	m_nNextPtr = 0;
	m_nMaxAccessed = 0;
	m_nLine = 0;
	m_rgb = 0;
	m_nUseToPrint = 0;
	m_nAllocatedSize=0;

	m_bImportOldCoordType=false;
	ZeroMemory(m_fExtrapolate,sizeof m_fExtrapolate);

	ZeroMemory(m_wStr, sizeof m_wStr);
	D3DXMatrixIdentity(&m_Matrix);

	m_pCp=NULL;
	m_fLength[0] = NULL;
	m_fLength[1] = NULL;
	m_pfKnots = NULL;
	for(int nAxis =0;nAxis<30;nAxis++) {
		m_fLengthPosDiff[nAxis] = NULL;
	}


}

CPolyCoord::CPolyCoord(CCoord Cp)
{
	m_nCoordL=1;
	m_pCp = new COORDDATA[m_nCoordL];
	m_pCp[0]=Cp;
	m_bModified=TRUE;
	m_nStyle=0;
	m_rgb = 0;
}

CPolyCoord::~CPolyCoord()
{
	int nAxis;

	SAFE_DELETE_ARRAY( m_pCp );
	SAFE_DELETE( m_fLength[0] );
	SAFE_DELETE( m_fLength[1] );
	SAFE_DELETE( m_pfKnots );
	for(nAxis = 0;nAxis<30;nAxis++) {
		SAFE_DELETE( m_fLengthPosDiff[ nAxis ] );
	}
}

void CPolyCoord::Zero()
{
	SAFE_DELETE_ARRAY( m_pCp );
	m_nCoordL=0;
	m_nStyle=0;
	m_nPrimaryAxis=0;
	m_fLinearLength=0.0f;
	m_nLineIncrement=0;
	m_nLine = 0;
	m_rgb = 0;
	m_nUseToPrint = 0;

	ZeroMemory(m_fExtrapolate,sizeof m_fExtrapolate);
	ZeroMemory(m_wStr, sizeof m_wStr);
	D3DXMatrixIdentity(&m_Matrix);

	for(int nSide=0;nSide<2;nSide++) {
		if(m_fLength[nSide]) delete m_fLength[nSide];
		m_fLength[nSide]=NULL;
	}
	for(int nAxis=0;nAxis<30;nAxis++) {
		if(m_fLengthPosDiff[nAxis]) delete m_fLengthPosDiff[nAxis];
		m_fLengthPosDiff[nAxis]=NULL;
	};

	if(m_pfKnots) delete m_pfKnots;
	m_pfKnots=NULL;
	m_nKnotsL=0;
	m_nCurveOrder=0;

	m_nNextPtr = 0;
	m_nMaxAccessed = 0;
	m_nAllocatedSize = 0;

}

void CPolyCoord::RemoveAll()
{
	Zero();
}

void CPolyCoord::AllocateCoords(int nCoordsToAllocate)
{
	AllocateCoordMemory(nCoordsToAllocate);
	m_nCoordL = nCoordsToAllocate;
}

void CPolyCoord::AllocateCoordMemory(int nCoordsToAllocate)
{
	SAFE_DELETE_ARRAY(m_pCp);
	if (nCoordsToAllocate>0) m_pCp = new COORDDATA[m_nAllocatedSize = nCoordsToAllocate];
	m_nMaxAccessed = 0;
	m_nAllocatedSize = nCoordsToAllocate;
}

void CPolyCoord::ReallocateCoords(int nNewL)
{
	COORDDATA *pCpTemp;

	if (nNewL == 0) {
		DeleteAllCoords();
	}
	else {
		if (m_pCp == NULL) {
			AllocateCoordMemory(nNewL);
		}
		else {
			int oldAllocatedSize = m_nAllocatedSize;
			pCpTemp = new COORDDATA[oldAllocatedSize];
			CopyMemory(pCpTemp, m_pCp, oldAllocatedSize * sizeof COORDDATA);
			if (m_pCp) delete[] m_pCp;

			int lengthToCopy = oldAllocatedSize;
			if (nNewL < oldAllocatedSize) { //Reducing the length.
				lengthToCopy = nNewL;
				m_nCoordL = nNewL;
			}
			m_pCp = new COORDDATA[m_nAllocatedSize = nNewL];
			CopyMemory(m_pCp, pCpTemp, lengthToCopy * sizeof COORDDATA);
			delete[] pCpTemp;

		}
	}
}

void CPolyCoord::DeleteAllCoords()
{

	SAFE_DELETE_ARRAY( m_pCp );
	m_nAllocatedSize = m_nCoordL = 0;
}

void CPolyCoord::ClearMemory()
{
	if(m_pCp) {
		ZeroMemory(m_pCp,m_nCoordL * sizeof COORDDATA);
	}
	if(m_pfKnots) {
		ZeroMemory(m_pfKnots,m_nKnotsL * sizeof (int));
	}
}

void CPolyCoord::SetStyle(int nStyle)
{

	m_nStyle = nStyle;
	m_bModified = true;
}

void CPolyCoord::SetColour(COLORREF rgb)
{
	m_rgb = rgb;
	m_bModified = true;
}

COLORREF CPolyCoord::GetColour()
{
	return m_rgb;
}

void CPolyCoord::operator + (COORDDATA Cp)
{
	int nn=m_nCoordL;
	m_nCoordL++;

	if(m_nCoordL==1) {
		AllocateCoordMemory(m_nCoordL + 20);
	} else {
		if(m_nCoordL > m_nAllocatedSize) {
			ReallocateCoords(m_nCoordL + 20);
		}
	};
	m_pCp[nn] = Cp;
	m_bModified = TRUE;
}

void CPolyCoord::operator + (COORDDATA *Cp)
{
	int nn=m_nCoordL;
	m_nCoordL++;

	if(m_nCoordL==1) {
		AllocateCoordMemory(m_nCoordL + 20);
	} else {
		if(m_nCoordL > m_nAllocatedSize) {
			ReallocateCoords(m_nCoordL + 20);
		}
	};
	m_pCp[nn] = *Cp;
	m_bModified = TRUE;
}

void CPolyCoord::Add(CCoord Cp)
{
	int nn = m_nCoordL;
	m_nCoordL++;

	if (m_nCoordL == 1) {
		AllocateCoordMemory(m_nCoordL + 20);
	}
	else {
		if (m_nCoordL > m_nAllocatedSize) {
			ReallocateCoords(m_nCoordL + 20);
		}
	};
	m_pCp[nn] = Cp;
	m_bModified = TRUE;
}

void CPolyCoord::Add(COORDDATA Cp)
{
	int nn = m_nCoordL;
	m_nCoordL++;

	if (m_nCoordL == 1) {
		AllocateCoordMemory(m_nCoordL + 20);
	}
	else {
		if (m_nCoordL > m_nAllocatedSize) {
			ReallocateCoords(m_nCoordL + 20);
		}
	};
	m_pCp[nn] = Cp;
	m_bModified = TRUE;
}

void CPolyCoord::AddVertex(CUSLVertex vPt)
{
	int nn=m_nCoordL;
	m_nCoordL++;

	if(m_nCoordL==1) {
		AllocateCoordMemory(m_nCoordL + 20);
	} else {
		if(m_nCoordL > m_nAllocatedSize) {
			ReallocateCoords(m_nCoordL + 20);
		}
	};

	m_pCp[nn].Side0.fX = m_pCp[nn].Side1.fX = vPt.fX;
	m_pCp[nn].Side0.fY = m_pCp[nn].Side1.fY = vPt.fY;
	m_pCp[nn].Side0.fZ = m_pCp[nn].Side1.fZ = vPt.fZ;
	m_pCp[nn].Side0.fI = m_pCp[nn].Side1.fI = vPt.fI;
	m_pCp[nn].Side0.fJ = m_pCp[nn].Side1.fJ = vPt.fJ;
	m_pCp[nn].Side0.fK = m_pCp[nn].Side1.fK = vPt.fK;
	
	m_bModified = TRUE;
}

bool CPolyCoord::AddIfNotAlreadyHere(CCoord Cp)
{
	int nAxis;

	for(int nn=0;nn<m_nCoordL;nn++) {
		for(nAxis=0;nAxis<20;nAxis++) {
			if(m_pCp[nn].fPos[nAxis] != Cp.fPos[nAxis]) {
				break;
			}
		}
		if(nAxis == 20) return false;
	}
	Add(Cp);
	return true;
}


void CPolyCoord::InsertCoord(CCoord Cp)
{
	int nSegment;

	FindNearestSegment(Cp,0,&nSegment);
	InsertCoord(nSegment,Cp,0);
}

void CPolyCoord::InsertCoord(int nIndex, COORDDATA Cp, int nAfterOrAt)
{
	CCoord Cp1;
	Cp1 = Cp;

	InsertCoord(nIndex, Cp1, nAfterOrAt);
}

void CPolyCoord::InsertCoord(int nIndex, CCoord Cp,int nAfterOrAt)
{
	m_nCoordL++;

	if(m_nCoordL==1) {
		AllocateCoordMemory(m_nCoordL + 20);
		m_pCp[nIndex] = Cp;
	} else {
		if(m_nCoordL>m_nAllocatedSize) {
			ReallocateCoords(m_nCoordL + 20);
		}
		if(nAfterOrAt == 0) {
			for(int nn=m_nCoordL-1;nn>nIndex;nn--) {
				m_pCp[nn]=m_pCp[nn-1];
			}
			m_pCp[nIndex+1] = Cp;
		} else {
			for(int nn=m_nCoordL-1;nn>nIndex;nn--) {
				m_pCp[nn]=m_pCp[nn-1];
			}
			m_pCp[nIndex] = Cp;
		}
	};
	m_bModified = TRUE;

}

void CPolyCoord::ModifyCoord(int nIndex,CCoord Cp)
{
	if(nIndex<m_nCoordL) {
		m_pCp[nIndex] = Cp;
		m_bModified = TRUE;
		if(nIndex>m_nMaxAccessed) m_nMaxAccessed=nIndex;
	}
}

void CPolyCoord::ModifyBothNorms(int nIndex,D3DXVECTOR4 vec)
{
	if(nIndex<0) return;
	if(nIndex>=m_nCoordL) return;

	m_pCp[nIndex].Side0.norm = vec;
	m_pCp[nIndex].Side1.norm = vec;
	m_bModified = TRUE;
	if(nIndex>m_nMaxAccessed) m_nMaxAccessed=nIndex;

}

void CPolyCoord::operator = (CPolyCoord* pCp)
{
	

	if (pCp->m_nCoordL>0) {
		if (m_pCp == NULL) {
			AllocateCoordMemory(pCp->m_nCoordL + 20);
		}
		else {
			if (pCp->m_nCoordL > m_nAllocatedSize) {
				ReallocateCoords(pCp->m_nCoordL + 20);
			}
		};
		m_nCoordL = pCp->m_nCoordL;
		CopyMemory(m_pCp, pCp->m_pCp, m_nCoordL * sizeof COORDDATA);
	}

	if(pCp->m_nKnotsL>0) {
		if(m_pfKnots==NULL) {
			m_pfKnots = new float[m_nKnotsL=pCp->m_nKnotsL];
		} else {
			m_pfKnots = (float *)realloc(m_pfKnots,(m_nKnotsL = pCp->m_nKnotsL) * sizeof (float));
		}
		CopyMemory(m_pfKnots,pCp->m_pfKnots,pCp->m_nKnotsL * sizeof (float));
	}

	m_nPrimaryAxis = pCp->m_nPrimaryAxis;
	m_nStyle = pCp->m_nStyle;
	m_nCurveOrder = pCp->m_nCurveOrder;
	m_nLineIncrement = pCp->m_nLineIncrement;
	m_nMaxPtr = pCp->m_nMaxPtr;
	m_rgb = pCp->m_rgb;
	CopyMemory(m_wStr, pCp->m_wStr, sizeof m_wStr);
	CopyMemory(m_Matrix,pCp->m_Matrix,sizeof m_Matrix);

	m_bModified = TRUE;

}

void CPolyCoord::operator += (CPolyCoord* pCp)
{
	int nOldLength = m_nCoordL;
	if(nOldLength<0) nOldLength = 0;
	m_nCoordL += pCp->m_nCoordL;

	int nOldKnotsL = m_nKnotsL;
	if(nOldKnotsL<0) nOldKnotsL = 0;

	if(m_pCp==NULL) {
		AllocateCoordMemory(m_nCoordL + 20);
	} else {
		if(m_nCoordL>m_nAllocatedSize) {
			ReallocateCoords(m_nCoordL + 20);
		}
	};
	CopyMemory(&m_pCp[nOldLength],pCp->m_pCp,pCp->m_nCoordL * sizeof COORDDATA);

	if(pCp->m_nKnotsL>0) {
		if(m_pfKnots==NULL) {
			m_pfKnots = new float[m_nKnotsL=pCp->m_nKnotsL];
		} else {
			m_pfKnots = (float *)realloc(m_pfKnots,(m_nKnotsL += pCp->m_nKnotsL) * sizeof (float));
		}
		CopyMemory(&m_pfKnots[nOldKnotsL],pCp->m_pfKnots,pCp->m_nKnotsL * sizeof (float));
	}

	m_nPrimaryAxis = pCp->m_nPrimaryAxis;
	m_nStyle = pCp->m_nStyle;
	m_nCurveOrder = pCp->m_nCurveOrder;
	m_nLineIncrement = pCp->m_nLineIncrement;
	m_nMaxPtr = pCp->m_nMaxPtr;
	m_rgb = pCp->m_rgb;
	CopyMemory(m_wStr, pCp->m_wStr, sizeof m_wStr);
	m_bModified = TRUE;

}



void CPolyCoord::DeleteCoord(int nIndex)
{
	int	nn;

	if((m_nCoordL>0) && (nIndex<m_nCoordL) && (nIndex>=0)) {
		m_nCoordL--;
		for(nn=nIndex; nn<m_nCoordL ;nn++) {
			m_pCp[nn]=m_pCp[nn+1];
		};
		if(m_nCoordL>0) {
			ReallocateCoords(m_nCoordL);
		} else {
			DeleteAllCoords();
		}
		m_bModified = TRUE;
	}
}



void CPolyCoord::DeleteCoord(COORDDATA Cp,int nSide)
{
	int	nn;

	for(nn=0;nn<m_nCoordL;nn++) {
		if(m_pCp[nn].Side[nSide].fX==Cp.Side[nSide].fX && m_pCp[nn].Side[nSide].fY==Cp.Side[nSide].fY && m_pCp[nn].Side[nSide].fZ==Cp.Side[nSide].fZ) {
			DeleteCoord(nn);
			m_bModified = TRUE;
			return;
		}
	}

}

void CPolyCoord::DeleteCoords(int nStartIndex, int nFinishIndex)
{
	int	nn,nLength;
	nLength = nFinishIndex-nStartIndex+1;
	for(nn=nStartIndex;nn<m_nCoordL-nLength;nn++ ) {
		m_pCp[nn]=m_pCp[nn+nLength];
	}
	m_nCoordL -= nLength;
	if(m_nCoordL>0) {
		ReallocateCoords(m_nCoordL);
	} else {
		DeleteAllCoords();
	}
	m_bModified = TRUE;

}

void CPolyCoord::Decimate(int nMaxPts)
{
	CPolyCoord Line;
	int	nn,nSrc;

	if(m_nCoordL>nMaxPts) {
		for(nn=0;nn<nMaxPts;nn++) {
			nSrc = MulDiv(nn,m_nCoordL-1,nMaxPts-1);
			Line + (&m_pCp[nSrc]);
		}
		Zero();

		AllocateCoords(nMaxPts);
		for(nn=0;nn<nMaxPts;nn++) {
			m_pCp[nn]=Line.m_pCp[nn];
		}
		m_bModified = TRUE;
	}

}




ULONGLONG CPolyCoord::Save(CUSLFile *pFile)
{
	int nStructSize = sizeof COORDDATA;
	ULONGLONG n64CoordOffset,n64Eof;
	ULONGLONG n64FileOffset = pFile->GetPosition();

	pFile->Write(&n64CoordOffset,sizeof n64CoordOffset);
	pFile->Write(&m_nCoordL,sizeof m_nCoordL);
	pFile->Write(&nStructSize,sizeof nStructSize);
	pFile->Write(&m_nStyle,sizeof m_nStyle);
	pFile->Write(&m_nEol,sizeof m_nEol);
	pFile->Write(&m_fExtrapolate,sizeof m_fExtrapolate);
	pFile->Write(&m_nLineIncrement,sizeof m_nLineIncrement);
	pFile->Write(&m_rgb,sizeof m_rgb);
	pFile->Write(m_wStr, sizeof m_wStr);
	pFile->Write(m_Matrix, sizeof m_Matrix);

	n64CoordOffset=pFile->GetPosition();
	if(m_pCp) pFile->Write(m_pCp,(sizeof COORDDATA) * m_nCoordL);
	n64Eof=pFile->GetPosition();

	pFile->Seek(n64FileOffset, CFile::begin);
	pFile->Write(&n64CoordOffset,sizeof n64CoordOffset);

	pFile->Seek(n64Eof,CFile::begin);

	return n64FileOffset;
}

void CPolyCoord::Retrieve(CUSLFile *pFile, bool bOldCoordType)
{
	int nStructSize;
	ULONGLONG n64CoordOffset;
	int nCoordOffset;

	Zero();
	if (bOldCoordType == false) {
		pFile->Read(&n64CoordOffset, sizeof n64CoordOffset);
		pFile->Read(&m_nCoordL, sizeof m_nCoordL);
		pFile->Read(&nStructSize, sizeof nStructSize);
		pFile->Read(&m_nStyle, sizeof m_nStyle);
		pFile->Read(&m_nEol, sizeof m_nEol);
		pFile->Read(&m_fExtrapolate, sizeof m_fExtrapolate);
		pFile->Read(&m_nLineIncrement, sizeof m_nLineIncrement);
		pFile->Read(&m_rgb, sizeof m_rgb);
		pFile->Read(m_wStr, sizeof m_wStr);
		pFile->Read(m_Matrix, sizeof m_Matrix);

		pFile->Seek(n64CoordOffset, CFile::begin);
		AllocateCoords(m_nCoordL);

		if (m_nCoordL) pFile->Read((void*)m_pCp, (sizeof COORDDATA) * m_nCoordL);
	}
	else {
		pFile->Read(&nCoordOffset, sizeof nCoordOffset);
		pFile->Read(&m_nCoordL, sizeof m_nCoordL);
		pFile->Read(&nStructSize, sizeof nStructSize);
		pFile->Read(&m_nStyle, sizeof m_nStyle);
		pFile->Read(&m_nEol, sizeof m_nEol);
		pFile->Read(&m_fExtrapolate, sizeof m_fExtrapolate);
		pFile->Read(&m_nLineIncrement, sizeof m_nLineIncrement);
		pFile->Read(&m_rgb, sizeof m_rgb);
		pFile->Read(m_wStr, sizeof m_wStr);
		pFile->Read(m_Matrix, sizeof m_Matrix);

		pFile->Seek(nCoordOffset, CFile::begin);
		AllocateCoords(m_nCoordL);

		if (nStructSize == (sizeof COORDDATA)) {
			if (m_nCoordL) pFile->Read((void*)m_pCp, (sizeof COORDDATA) * m_nCoordL);
			for (int ii = 0; ii < m_nCoordL; ii++) m_pCp[ii].InvertNorm(1);
		}
		else {
			for (int nn = 0; nn < m_nCoordL; nn++) {
				void *pCoord = &m_pCp[nn];
				pFile->Read(pCoord, nStructSize);
			}
		}
	}

	m_wStr[LINE_STR_LENGTH-1] = 0;
}


COORDDATA* CPolyCoord::GetPtrCoord(int nIndex)
{
	return &m_pCp[nIndex];
}

COORDDATA*	CPolyCoord::ElementAt(int nIndex)
{
	if (nIndex >= 0 && nIndex < m_nCoordL) {
		return &m_pCp[nIndex];
	}

	return NULL;
}

int CPolyCoord::GetCoordType(int nIndex)
{
	return m_pCp[nIndex].nType;
}


bool CPolyCoord::GetCoord(int nIndex,COORDDATA *pCp)
{
	if(m_pCp==NULL) return false;
	if(m_nCoordL<=0) return false;
	if(nIndex>=m_nCoordL) return false;
	if(nIndex<0) return false;
	CopyMemory(pCp,&m_pCp[nIndex],sizeof COORDDATA);
	return true;
}

void CPolyCoord::GetFirstCoord(COORDDATA *pCp)
{
	if(m_pCp==NULL) return;
	if(m_nCoordL<=0) return;
	CopyMemory(pCp,&m_pCp[0],sizeof COORDDATA);

}

int CPolyCoord::GetLastCoord(COORDDATA *pCp)
{
	if(m_pCp==NULL) return -1;
	if(m_nCoordL<=0) return -1;
	CopyMemory(pCp,&m_pCp[m_nCoordL-1],sizeof COORDDATA);

	return m_nCoordL-1;
}

int CPolyCoord::GetCenterCoord(COORDDATA *pCp)
{
	if(m_pCp==NULL) return -1;
	if(m_nCoordL<=0) return -1;
	int nn = m_nCoordL / 2;

	CopyMemory(pCp,&m_pCp[nn],sizeof COORDDATA);

	return nn;
}

bool CPolyCoord::GetCoordComeWhatMay(int nIndex,COORDDATA *pCp)
{
	if(nIndex<0) nIndex=0;
	if(nIndex>=m_nCoordL) nIndex = m_nCoordL-1;
	CopyMemory(pCp,&m_pCp[nIndex],sizeof COORDDATA);
	return true;

}

bool CPolyCoord::GetVertex(int nIndex, int nSide, CUSLVertex *pVertex)
{
	if(m_pCp==NULL) return false;
	if(m_nCoordL<=0) return false;
	if(nIndex>=m_nCoordL) return false;
	if(nIndex<0) return false;

	pVertex->pt = m_pCp[nIndex].Side[nSide].pt;
	pVertex->norm = m_pCp[nIndex].Side[nSide].norm;

	return true;

}


void CPolyCoord::FindLimits(COORDDATA *pCp0, COORDDATA *pCp1, bool bInitializeFirst)
{
	int nn=0;

	if(m_nCoordL>0) {
		if(bInitializeFirst) {
			*pCp0 = m_pCp[nn];
			*pCp1 = m_pCp[nn];
			nn++;
		}
		for(nn;nn<m_nCoordL;nn++) {
			if(pCp0->Side[PORTSIDE].fX>m_pCp[nn].Side[PORTSIDE].fX) pCp0->Side[PORTSIDE].fX = m_pCp[nn].Side[PORTSIDE].fX;
			if(pCp1->Side[PORTSIDE].fX<m_pCp[nn].Side[PORTSIDE].fX) pCp1->Side[PORTSIDE].fX = m_pCp[nn].Side[PORTSIDE].fX;

			if(pCp0->Side[PORTSIDE].fY>m_pCp[nn].Side[PORTSIDE].fY) pCp0->Side[PORTSIDE].fY = m_pCp[nn].Side[PORTSIDE].fY;
			if(pCp1->Side[PORTSIDE].fY<m_pCp[nn].Side[PORTSIDE].fY) pCp1->Side[PORTSIDE].fY = m_pCp[nn].Side[PORTSIDE].fY;

			if(pCp0->Side[PORTSIDE].fZ>m_pCp[nn].Side[PORTSIDE].fZ) pCp0->Side[PORTSIDE].fZ = m_pCp[nn].Side[PORTSIDE].fZ;
			if(pCp1->Side[PORTSIDE].fZ<m_pCp[nn].Side[PORTSIDE].fZ) pCp1->Side[PORTSIDE].fZ = m_pCp[nn].Side[PORTSIDE].fZ;

			if(pCp0->Side[PORTSIDE].fI>m_pCp[nn].Side[PORTSIDE].fI) pCp0->Side[PORTSIDE].fI = m_pCp[nn].Side[PORTSIDE].fI;
			if(pCp1->Side[PORTSIDE].fI<m_pCp[nn].Side[PORTSIDE].fI) pCp1->Side[PORTSIDE].fI = m_pCp[nn].Side[PORTSIDE].fI;

			if(pCp0->Side[PORTSIDE].fJ>m_pCp[nn].Side[PORTSIDE].fJ) pCp0->Side[PORTSIDE].fJ = m_pCp[nn].Side[PORTSIDE].fJ;
			if(pCp1->Side[PORTSIDE].fJ<m_pCp[nn].Side[PORTSIDE].fJ) pCp1->Side[PORTSIDE].fJ = m_pCp[nn].Side[PORTSIDE].fJ;
		
			if(pCp0->Side[PORTSIDE].fK>m_pCp[nn].Side[PORTSIDE].fK) pCp0->Side[PORTSIDE].fK = m_pCp[nn].Side[PORTSIDE].fK;
			if(pCp1->Side[PORTSIDE].fK<m_pCp[nn].Side[PORTSIDE].fK) pCp1->Side[PORTSIDE].fK = m_pCp[nn].Side[PORTSIDE].fK;

		}
	} else {
		pCp0->Side[PORTSIDE].fX=0.0;
		pCp1->Side[PORTSIDE].fX=100.0;

		pCp0->Side[PORTSIDE].fY=0.0;
		pCp1->Side[PORTSIDE].fY=100.0;

		pCp0->Side[PORTSIDE].fZ=0.0;
		pCp1->Side[PORTSIDE].fZ=100.0;

		pCp0->Side[PORTSIDE].fI=-1.0;
		pCp1->Side[PORTSIDE].fI=1.0;

		pCp0->Side[PORTSIDE].fJ=-1.0;
		pCp1->Side[PORTSIDE].fJ=1.0;

		pCp0->Side[PORTSIDE].fK=-1.0;
		pCp1->Side[PORTSIDE].fK=1.0;
	}

}

void CPolyCoord::FindLimits(int nAxis, float *fMin,float *fMax)
{

	if(m_nCoordL>0) {
		*fMin = m_pCp[0].fPos[nAxis];
		*fMax = m_pCp[0].fPos[nAxis];
		for(int nn=1;nn<m_nMaxAccessed;nn++) {
			if(*fMin>m_pCp[nn].fPos[nAxis]) *fMin=m_pCp[nn].fPos[nAxis];
			if(*fMax<m_pCp[nn].fPos[nAxis]) *fMax=m_pCp[nn].fPos[nAxis];
		}
	} else {
		*fMin = 0.0f;
		*fMax = 1.0f;
	}

}


void CPolyCoord::ReverseAllCoords()
{
	COORDDATA *pCp;

	pCp = new COORDDATA[m_nCoordL];
	for(int ii=0;ii<m_nCoordL;ii++) {
		pCp[ii]=m_pCp[m_nCoordL-1-ii];
	}
	for(int ii=0;ii<m_nCoordL;ii++) {
		m_pCp[ii]=pCp[ii];
	}
	delete [] pCp;

}


void CPolyCoord::InvertAllNormals()
{
	for(int ii=0;ii<m_nCoordL;ii++) {
		m_pCp[ii].Side0.fI *= -1;
		m_pCp[ii].Side0.fJ *= -1;
		m_pCp[ii].Side0.fK *= -1;
		m_pCp[ii].Side1.fI *= -1;
		m_pCp[ii].Side1.fJ *= -1;
		m_pCp[ii].Side1.fK *= -1;
	}
}


void CPolyCoord::InvertINormal()
{
	for(int ii=0;ii<m_nCoordL;ii++) {
		m_pCp[ii].Side0.fI *= -1;
		m_pCp[ii].Side1.fI *= -1;
	}

}

void CPolyCoord::InvertJNormal()
{
	for(int ii=0;ii<m_nCoordL;ii++) {
		m_pCp[ii].Side0.fJ *= -1;
		m_pCp[ii].Side1.fJ *= -1;
	}

}

void CPolyCoord::InvertKNormal()
{
	for(int ii=0;ii<m_nCoordL;ii++) {
		m_pCp[ii].Side0.fK *= -1;
		m_pCp[ii].Side1.fK *= -1;
	}

}



void CPolyCoord::Sort(int nAxis,int nSortDir)
{
	theApp.m_nSortAxis = nAxis;
	theApp.m_nSortDir = nSortDir;

	qsort(m_pCp,m_nCoordL,sizeof COORDDATA,ComparePolyCoordData);
}

int ComparePolyCoordData(const void *Cp0,const void *Cp1)
{
	COORDDATA *Cl0 = (COORDDATA*)Cp0;
	COORDDATA *Cl1 = (COORDDATA*)Cp1;

	if(theApp.m_nSortDir == 0) {
		if(Cl0->fPos[theApp.m_nSortAxis] == Cl1->fPos[theApp.m_nSortAxis]) 
			return 0;
		if(Cl0->fPos[theApp.m_nSortAxis] < Cl1->fPos[theApp.m_nSortAxis]) 
			return -1;
		if(Cl0->fPos[theApp.m_nSortAxis] > Cl1->fPos[theApp.m_nSortAxis]) 
			return 1;
	} else {
		if(Cl0->fPos[theApp.m_nSortAxis] == Cl1->fPos[theApp.m_nSortAxis]) 
			return 0;
		if(Cl0->fPos[theApp.m_nSortAxis] > Cl1->fPos[theApp.m_nSortAxis]) 
			return -1;
		if(Cl0->fPos[theApp.m_nSortAxis] < Cl1->fPos[theApp.m_nSortAxis]) 
			return 1;
	}

	return 0;

}

void CPolyCoord::SortVariable(int nWhichVariable,int nSortDir)
{
	theApp.m_nSortAxis = nWhichVariable;
	theApp.m_nSortDir = nSortDir;

	qsort(m_pCp,m_nCoordL,sizeof COORDDATA,CompareVariableCoordData);
}

int CompareVariableCoordData(const void *Cp0,const void *Cp1)
{
	COORDDATA *Cl0 = (COORDDATA*)Cp0;
	COORDDATA *Cl1 = (COORDDATA*)Cp1;

	switch(theApp.m_nSortAxis) {
	case 0:
		if(theApp.m_nSortDir == 0) {
			if(Cl0->fNomThickness == Cl1->fNomThickness) 
				return 0;
			if(Cl0->fNomThickness < Cl1->fNomThickness) 
				return -1;
			if(Cl0->fNomThickness > Cl1->fNomThickness) 
				return 1;
		} else {
			if(Cl0->fNomThickness == Cl1->fNomThickness) 
				return 0;
			if(Cl0->fNomThickness > Cl1->fNomThickness) 
				return -1;
			if(Cl0->fNomThickness < Cl1->fNomThickness) 
				return 1;
		}
		break;
	}

	return 0;

}

void CPolyCoord::PreProcessLine()
{
	float fX,fY,fZ;
	int nSide,ii,nAxis;

	if(m_nCoordL<=0) return;

	if(m_bModified!=FALSE) {

		if(m_fLength[0]) delete m_fLength[0];
		if(m_fLength[1]) delete m_fLength[1];
		m_fLength[0]=new float[m_nCoordL];
		m_fLength[1]=new float[m_nCoordL];
		if(m_nStyle==1) {
			for(nAxis=0;nAxis<30;nAxis++) {
				if(m_fLengthPosDiff[nAxis]) delete m_fLengthPosDiff[nAxis];
				m_fLengthPosDiff[nAxis]=new float[m_nCoordL];
			}
		}
		
		m_fLength[0][0]=0;
		m_fLength[1][0]=0;
		if(m_nPrimaryAxis==30) {
			for(ii=1;ii<m_nCoordL;ii++) {
				for(nSide=0;nSide<2;nSide++) {
					fX=m_pCp[ii].Side[nSide].fX - m_pCp[ii-1].Side[nSide].fX;
					fY=m_pCp[ii].Side[nSide].fY - m_pCp[ii-1].Side[nSide].fY;
					fZ=m_pCp[ii].Side[nSide].fZ - m_pCp[ii-1].Side[nSide].fZ;
					m_fLength[nSide][ii]=(float)sqrt((double)((fX*fX)+(fY*fY)+(fZ*fZ))) + m_fLength[nSide][ii-1];
				}
			}
			m_fLinearLength = m_fLength[0][ii-1];
		} else {
			for(ii=0;ii<m_nCoordL;ii++) {
				m_fLength[0][ii] = m_pCp[ii].fPos[m_nPrimaryAxis];
			}
			m_fLinearLength = m_fLength[0][ii-1] - m_fLength[0][0];
		}
		m_fLinearLength = fabs(m_fLinearLength);

		if(m_nStyle==1) {
			for(nAxis=0;nAxis<30;nAxis++) {
				Spline(nAxis);
			}
		}
	}
	m_bModified = FALSE;
}


void CPolyCoord::Spline(int nAxis)
{
	int i,k;
	float *u;
	float	sig,p,qn,un;
	float	yp1=2e30f;
	float  ypn=2e30f;
	float *y2a = m_fLengthPosDiff[nAxis];
	float *x = m_fLength[0];

	u = new float[m_nCoordL+2];
	if(yp1>=1e30) {
		y2a[0]=0;	//y2a[1]=0;
		u[0]=0;		//u[1]=0;
	} else {
		y2a[0]=-0.5f;	//y2a[1]=-0.5f;
		u[0]=(3.0f/(x[1]-x[0]))*((m_pCp[1].fPos[nAxis]-m_pCp[0].fPos[nAxis])/(x[1]-x[0])-yp1);	//u[1]=(3.0f/(x[2]-x[1]))*((y[2]-y[1])/(x[2]-x[1])-yp1);
	};
	for(i=1;i<(m_nCoordL-1);i++) {
		sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);
		p=sig*y2a[i-1]+2;
		y2a[i]=(sig-1.0f)/p;
		u[i]=(6.0f*((m_pCp[i+1].fPos[nAxis]-m_pCp[i].fPos[nAxis])/(x[i+1]-x[i])-(m_pCp[i].fPos[nAxis]-m_pCp[i-1].fPos[nAxis])
				/(x[i]-x[i-1]))/(x[i+1]-x[i-1])-sig*u[i-1])/p;
	};
	if(ypn>=1e30f) {
		qn=0.0f;
		un=0.0f;
	}else {
		qn=0.5f;
		un=(3.0f/(x[i]-x[i-1]))*(ypn-(m_pCp[i].fPos[nAxis]-m_pCp[i-1].fPos[nAxis])/(x[i]-x[i-1]));
	};
	if(i<m_nCoordL) y2a[i]=(un-qn*u[i-1])/(qn*y2a[i-1]+1.0f);
	for(k=m_nCoordL-2;k>=0;k--) {
		y2a[k]=y2a[k]*y2a[k+1]+u[k];
	};
	delete u;
}

bool CPolyCoord::Splint(float x,int nAxis,float *y,int *nType)
{
	int	klo,khi,k;
	float	h,a,b,dtemp;
	float	ac,bc,hs;
	float *xa = m_fLength[0];
	float *y2a = m_fLengthPosDiff[nAxis];

	klo=0;
	khi=m_nCoordL-1;


	if(m_nPrimaryAxis==30) {
		while((khi-klo)>1) {
			k=(khi+klo)/2;
			(m_fLength[0][k]>x)?(khi=k):(klo=k);
		};
		h=m_fLength[0][khi]-m_fLength[0][klo];
		if(h==0.0) return FALSE;
		a=(m_fLength[0][khi]-x)/h;
		b=(x-m_fLength[0][klo])/h;
	} else {
		while((khi-klo)>1) {
			k=(khi+klo)/2;
			(m_pCp[k].fPos[m_nPrimaryAxis]>x)?(khi=k):(klo=k);
		};
		h=m_pCp[khi].fPos[m_nPrimaryAxis]-m_pCp[klo].fPos[m_nPrimaryAxis];
		if(h==0.0) return FALSE;
		a=(m_pCp[khi].fPos[m_nPrimaryAxis]-x)/h;
		b=(x-m_pCp[klo].fPos[m_nPrimaryAxis])/h;
	}

	ac=(a*a*a)-a;
	bc=(b*b*b)-b;
	hs=h*h;
	dtemp=a*m_pCp[klo].fPos[nAxis] + b*m_pCp[khi].fPos[nAxis] + (ac*y2a[klo] + bc*y2a[khi])*hs/6.0f;
	*y=dtemp;
	*nType = m_pCp[klo].nType;
	return FALSE;
}


bool CPolyCoord::CalculateCoord(float fPos,CCoord &Cp)
{
	int	kHi,kLo,k,nAxis,gg;
	bool bFlag = TRUE;
	float fEleckLo,fEleckHi;

	PreProcessLine();

	kHi=m_nCoordL-1;
	kLo=0;
	if(m_nPrimaryAxis<30) {
		while((kHi-kLo)>1) {
			k=(kHi+kLo)/2;
			if(m_pCp[kHi].fPos[m_nPrimaryAxis] > m_pCp[kLo].fPos[m_nPrimaryAxis]) { 
				(m_pCp[k].fPos[m_nPrimaryAxis]>fPos)?(kHi=k):(kLo=k);
			} else {
				(m_pCp[k].fPos[m_nPrimaryAxis]<fPos)?(kHi=k):(kLo=k);
			}
		};
	} else {
		while((kHi-kLo)>1) {
			k=(kHi+kLo)/2;
			if(m_fLength[0][kHi] > m_fLength[0][kLo]) { 
				(m_fLength[0][k]>fPos)?(kHi=k):(kLo=k);
			} else {
				(m_fLength[0][k]<fPos)?(kHi=k):(kLo=k);
			}
		};
	}

	switch(m_nStyle) {
	case 0:
		if(m_nPrimaryAxis<30) {
			if((fPos<m_pCp[0].fPos[m_nPrimaryAxis]) && (m_nEol[0]==EOL_NO_EXTRAPOLATION)) bFlag=FALSE;
			if((fPos>m_pCp[m_nCoordL-1].fPos[m_nPrimaryAxis]) && (m_nEol[1]==EOL_NO_EXTRAPOLATION)) bFlag=FALSE;

			for(nAxis=0;nAxis<30;nAxis++) {
				Cp.fPos[nAxis]=((fPos-m_pCp[kLo].fPos[m_nPrimaryAxis]) * (m_pCp[kHi].fPos[nAxis]-m_pCp[kLo].fPos[nAxis])) / (m_pCp[kHi].fPos[m_nPrimaryAxis]-m_pCp[kLo].fPos[m_nPrimaryAxis]) + m_pCp[kLo].fPos[nAxis];
			}
			for(gg=0;gg<14;gg++) {
				fEleckLo = (float)m_pCp[kLo].nElec[gg];
				fEleckHi = (float)m_pCp[kHi].nElec[gg];
				Cp.nElec[gg]=(int)(((fPos-m_pCp[kLo].fPos[m_nPrimaryAxis]) * (fEleckHi-fEleckLo)) / (m_pCp[kHi].fPos[m_nPrimaryAxis]-m_pCp[kLo].fPos[m_nPrimaryAxis]) + fEleckLo);
			}
			for(gg=0;gg<2;gg++) {
				Cp.fWp[gg]=((fPos-m_pCp[kLo].fPos[m_nPrimaryAxis]) * (m_pCp[kHi].fWp[gg]-m_pCp[kLo].fWp[gg])) / (m_pCp[kHi].fPos[m_nPrimaryAxis]-m_pCp[kLo].fPos[m_nPrimaryAxis]) + m_pCp[kLo].fWp[gg];
				Cp.fRadius[gg]=((fPos-m_pCp[kLo].fPos[m_nPrimaryAxis]) * (m_pCp[kHi].fRadius[gg]-m_pCp[kLo].fRadius[gg])) / (m_pCp[kHi].fPos[m_nPrimaryAxis]-m_pCp[kLo].fPos[m_nPrimaryAxis]) + m_pCp[kLo].fRadius[gg];
				Cp.fAngle[gg]=((fPos-m_pCp[kLo].fPos[m_nPrimaryAxis]) * (m_pCp[kHi].fAngle[gg]-m_pCp[kLo].fAngle[gg])) / (m_pCp[kHi].fPos[m_nPrimaryAxis]-m_pCp[kLo].fPos[m_nPrimaryAxis]) + m_pCp[kLo].fAngle[gg];
			}
			Cp.fThickness=((fPos-m_pCp[kLo].fPos[m_nPrimaryAxis]) * (m_pCp[kHi].fThickness-m_pCp[kLo].fThickness)) / (m_pCp[kHi].fPos[m_nPrimaryAxis]-m_pCp[kLo].fPos[m_nPrimaryAxis]) + m_pCp[kLo].fThickness;
			Cp.nType = m_pCp[kLo].nType;

		} else {

			if((fPos<m_fLength[0][0]) && (m_nEol[0]==EOL_NO_EXTRAPOLATION)) bFlag=FALSE;
			if((fPos>m_fLength[0][m_nCoordL-1]) && (m_nEol[1]==EOL_NO_EXTRAPOLATION)) bFlag=FALSE;

			for(nAxis=0;nAxis<30;nAxis++) {
				Cp.fPos[nAxis]=((fPos-m_fLength[0][kLo]) * (m_pCp[kHi].fPos[nAxis]-m_pCp[kLo].fPos[nAxis])) / (m_fLength[0][kHi]-m_fLength[0][kLo]) + m_pCp[kLo].fPos[nAxis];
			}
			for(gg=0;gg<14;gg++) {
				fEleckLo = (float)m_pCp[kLo].nElec[gg];
				fEleckHi = (float)m_pCp[kHi].nElec[gg];
				Cp.nDelay[gg]=(int)(((fPos-m_fLength[0][kLo]) * (fEleckHi-fEleckLo)) / (m_fLength[0][kHi]-m_fLength[0][kLo]) + fEleckLo);
			}
			for(gg=0;gg<2;gg++) {
				Cp.fWp[gg]=((fPos-m_fLength[0][kLo]) * (m_pCp[kHi].fWp[gg]-m_pCp[kLo].fWp[gg])) / (m_fLength[0][kHi]-m_fLength[0][kLo]) + m_pCp[kLo].fWp[gg];
				Cp.fRadius[gg]=((fPos-m_fLength[0][kLo]) * (m_pCp[kHi].fRadius[gg]-m_pCp[kLo].fRadius[gg])) / (m_fLength[0][kHi]-m_fLength[0][kLo]) + m_pCp[kLo].fRadius[gg];
				Cp.fAngle[gg]=((fPos-m_fLength[0][kLo]) * (m_pCp[kHi].fAngle[gg]-m_pCp[kLo].fAngle[gg])) / (m_fLength[0][kHi]-m_fLength[0][kLo]) + m_pCp[kLo].fAngle[gg];
			}
			Cp.fThickness=((fPos-m_fLength[0][kLo]) * (m_pCp[kHi].fThickness-m_pCp[kLo].fThickness)) / (m_fLength[0][kHi]-m_fLength[0][kLo]) + m_pCp[kLo].fThickness;
			Cp.nGain[0] = m_pCp[kLo].nGain[0];
			Cp.nTS = m_pCp[kLo].nTS;
			Cp.nType = m_pCp[kLo].nType;

		}
		break;
	case 1:
		for(nAxis=0;nAxis<30;nAxis++) {
			Splint(fPos,nAxis,&Cp.fPos[nAxis],&Cp.nType);
		}
		for(gg=0;gg<6;gg++) {
			fEleckLo = (float)m_pCp[kLo].nElec[gg];
			fEleckHi = (float)m_pCp[kHi].nElec[gg];
			Cp.nDelay[gg]=(int)(((fPos-m_fLength[0][kLo]) * (fEleckHi-fEleckLo)) / (m_fLength[0][kHi]-m_fLength[0][kLo]) + fEleckLo);
		}
		for(gg=0;gg<2;gg++) {
			Cp.fWp[gg]=((fPos-m_fLength[0][kLo]) * (m_pCp[kHi].fWp[gg]-m_pCp[kLo].fWp[gg])) / (m_fLength[0][kHi]-m_fLength[0][kLo]) + m_pCp[kLo].fWp[gg];
			Cp.fRadius[gg]=((fPos-m_fLength[0][kLo]) * (m_pCp[kHi].fRadius[gg]-m_pCp[kLo].fRadius[gg])) / (m_fLength[0][kHi]-m_fLength[0][kLo]) + m_pCp[kLo].fRadius[gg];
			Cp.fAngle[gg]=((fPos-m_fLength[0][kLo]) * (m_pCp[kHi].fAngle[gg]-m_pCp[kLo].fAngle[gg])) / (m_fLength[0][kHi]-m_fLength[0][kLo]) + m_pCp[kLo].fAngle[gg];
		}
		break;
	case 2: //PolynomialInterpolation(fPos,Cp);
		LagrangeInterpolationRolling(fPos,Cp);
		break;
	case 3: QuadraticInterpolation(fPos,Cp);
		break;
	}

	D3DXVec3Normalize((D3DXVECTOR3*)&Cp.Side0.norm,(D3DXVECTOR3*)&Cp.Side0.norm);
	D3DXVec3Normalize((D3DXVECTOR3*)&Cp.Side1.norm,(D3DXVECTOR3*)&Cp.Side1.norm);
	Cp.LimitIJK();

	return bFlag;


}

void CPolyCoord::FindSegment(float fPos,int &kLo,int &kHi)
{
	int k;

	kHi=m_nCoordL-1;
	kLo=0;
	if(m_nPrimaryAxis<30) {
		while((kHi-kLo)>1) {
			k=(kHi+kLo)/2;
			if(m_pCp[kHi].fPos[m_nPrimaryAxis] > m_pCp[kLo].fPos[m_nPrimaryAxis]) { 
				(m_pCp[k].fPos[m_nPrimaryAxis]>fPos)?(kHi=k):(kLo=k);
			} else {
				(m_pCp[k].fPos[m_nPrimaryAxis]<fPos)?(kHi=k):(kLo=k);
			}
		};
	} else {
		while((kHi-kLo)>1) {
			k=(kHi+kLo)/2;
			if(m_fLength[0][kHi] > m_fLength[0][kLo]) { 
				(m_fLength[0][k]>fPos)?(kHi=k):(kLo=k);
			} else {
				(m_fLength[0][k]<fPos)?(kHi=k):(kLo=k);
			}
		};
	}
}

bool CPolyCoord::LagrangeInterpolation(float fPos,CCoord &Cp)
{
	int	i,k,nAxis;

    if (m_nCoordL == 0) return 0;
 

    float* fLagrange = new float[m_nCoordL];
    for (i = 0; i < m_nCoordL; i++)
    {
        fLagrange[i] = 1.0f;
 
        for (k = 0; k < m_nCoordL; k++)
        {
            if (i != k)
            {
                fLagrange[i] *= ((fPos - m_pCp[k].fPos[m_nPrimaryAxis])) / (m_pCp[i].fPos[m_nPrimaryAxis] - m_pCp[k].fPos[m_nPrimaryAxis]);
            }
        }
    }
 
	Cp.fPos[m_nPrimaryAxis] = fPos;
	for(nAxis=0;nAxis<30;nAxis++) {
		if(nAxis - m_nPrimaryAxis) {
			float y = 0;
			for (i = 0; i < m_nCoordL; i++)
			{
				y += fLagrange[i] * m_pCp[i].fPos[nAxis];
			}
			Cp.fPos[nAxis] = y;
		}
	}
	delete fLagrange;
    return true;
}

bool CPolyCoord::LagrangeInterpolationRolling(float fPos,CCoord &Cp)
{
	int	i,k,nAxis,kLo,kHi;

    if (m_nCoordL == 0) return 0;
 
	FindSegment(fPos,kLo,kHi);
//	kLo-=1;
	if(kLo<0) kLo=0;
	kHi=kLo+4;
	if(kHi>m_nCoordL) kHi = m_nCoordL;
	int nCount = kHi-kLo;

    float* fLagrange = new float[m_nCoordL];
    for (i = 0; i < nCount; i++)
    {
        fLagrange[i] = 1.0f;
 
        for (k = kLo; k < kHi; k++)
        {
            if (i != (k-kLo))
            {
                fLagrange[i] *= ((fPos - m_pCp[k].fPos[m_nPrimaryAxis])) / (m_pCp[i+kLo].fPos[m_nPrimaryAxis] - m_pCp[k].fPos[m_nPrimaryAxis]);
            }
        }
    }
 
	Cp.fPos[m_nPrimaryAxis] = fPos;
	for(nAxis=0;nAxis<30;nAxis++) {
		if(nAxis - m_nPrimaryAxis) {
			float y = 0;
			for (i = 0; i < nCount; i++)
			{
				y += fLagrange[i] * m_pCp[i+kLo].fPos[nAxis];
			}
			Cp.fPos[nAxis] = y;
		}
	}
	delete fLagrange;
    return true;
}


struct InterpolationData {
	float	d;
	float	c;
};


bool CPolyCoord::PolynomialInterpolation(float fPos,CCoord &Cp)
{
	int	i,m,ns;
	float	dif,dift;
	float	w,dy,yr,fDeltafPos,ho,hp,den;
	int	nSide = 0;
	int nAxis;

	if(m_nCoordL<2) return false;
	Cp.fPos[m_nPrimaryAxis] = fPos;

	InterpolationData* inter = new InterpolationData[m_nCoordL+2];

	
	for(nAxis=0;nAxis<30;nAxis++) {

		if(nAxis - m_nPrimaryAxis) {

			ns = 1;
			dif = fabs(fPos - m_pCp[0].fPos[m_nPrimaryAxis]);
			for(i=1;i<=m_nCoordL;i++) {
				dift = fabs(fPos - m_pCp[i-1].fPos[m_nPrimaryAxis]);
				if(dift < dif) {
					ns = i;
					dif = dift;
				};
				inter[i].d = inter[i].c = m_pCp[i-1].fPos[nAxis];
			};
			yr=m_pCp[ns-1].fPos[nAxis];
			ns--;
			fDeltafPos=fPos;

			for(m=1;m<=(m_nCoordL-1);m++) {
				for(i=1;i<=(m_nCoordL-m);i++) {
					ho = m_pCp[i-1].fPos[m_nPrimaryAxis] - fDeltafPos;
					hp = m_pCp[i-1+m].fPos[m_nPrimaryAxis] - fDeltafPos;
					w = inter[i+1].c - inter[i].d;
					den = ho - hp;
					if(den == 0.0f) {
						delete inter;
						return(FALSE);
					};
					inter[i].d = (hp * w)/den;
					inter[i].c = (ho * w)/den;
				};
				if((2*ns) < (m_nCoordL-m)) {
					dy = inter[ns+1].c;
				} else {
					dy = inter[ns].d;
					ns = ns - 1;
				};
				yr = yr + dy;
			};
			Cp.fPos[nAxis] = yr;
		}

	}
	delete inter;

	return TRUE;
}

bool CPolyCoord::QuadraticInterpolation(float fPos,CCoord &Cp)
{
	int	i,m,ns,nAxis;
	float	dif,dift;
	float	w,dy,yr,dxx,ho,hp,den;

	if(m_nCoordL<=0) return false;

	InterpolationData *inter = new InterpolationData[m_nCoordL+2];

	Cp.fPos[m_nPrimaryAxis] = fPos;

	for(nAxis=0;nAxis<30;nAxis++) {
		if(nAxis - m_nPrimaryAxis) {
			ns = 1;
			dif = fabs(fPos - m_pCp[0].fPos[m_nPrimaryAxis]);
			for(i=1;i<=m_nCoordL;i++) {
				dift = fabs(fPos - m_pCp[i-1].fPos[m_nPrimaryAxis]);
				if(dift < dif) {
					ns = i;
					dif = dift;
				};
				inter[i].d = inter[i].c = m_pCp[i-1].fPos[nAxis];
			};
			yr=m_pCp[ns-1].fPos[nAxis];
			ns--;
			dxx=fPos;
			for(m=1;m<=(m_nCoordL-1);m++) {
				for(i=1;i<=(m_nCoordL-m);i++) {
					ho = m_pCp[i-1].fPos[m_nPrimaryAxis] - dxx;
					hp = m_pCp[i-1+m].fPos[m_nPrimaryAxis] - dxx;
					w = inter[i+1].c - inter[i].d;
					den = ho - hp;
					if(den == 0.0f) {
						delete inter;
						return(FALSE);
					};
					inter[i].d = (hp * w)/den;
					inter[i].c = (ho * w)/den;
				};
				if((2*ns) < (m_nCoordL-m)) {
					dy = inter[ns+1].c;
				} else {
					dy = inter[ns].d;
					ns = ns - 1;
				};
				yr = yr + dy;
			};
			Cp.fPos[nAxis] = yr;
		}
	}
	delete inter;

	return TRUE;
}

/*
bool CPolyCoord::PolynomialInterpolation(int nAxis,float fPos,float *fResult)
{
	int	i,m,ns;
	float	dif,dift;
	InterpolationData	*inter;
	float	w,dy,yr,fDeltafPos,ho,hp,den;
	int	nSide = 0;

	inter = new InterpolationData[m_nCoordL+2];
	float *x = new float[m_nCoordL+2];
	float *y = new float[m_nCoordL+2];

	for(i=0;i<m_nCoordL;i++) {
		x[i+1]=m_pCp[i].Side[nSide].fX;
		y[i+1]=m_pCp[i].Side[nSide].fZ;
	};
	

	ns = 1;
	dif = fabs(fPos - m_pCp[0].Side[nSide].fX);
	for(i=1;i<=m_nCoordL;i++) {
		dift = fabs(fPos - m_pCp[i-1].Side[nSide].fX);
		if(dift < dif) {
			ns = i;
			dif = dift;
		};
		inter[i].d = inter[i].c = y[i];
	};
	yr=y[ns];
	ns--;
//	fDeltafPos=xx;
	for(m=1;m<=(m_nCoordL-1);m++) {
		for(i=1;i<=(m_nCoordL-m);i++) {
			ho = x[i] - fDeltafPos;
			hp = x[i+m] - fDeltafPos;
			w = inter[i+1].c - inter[i].d;
			den = ho - hp;
			if(den == 0l) {
				delete inter;
				delete x;
				delete y;
				return(FALSE);
			};
			inter[i].d = (hp * w)/den;
			inter[i].c = (ho * w)/den;
		};
		if((2*ns) < (m_nCoordL-m)) {
			dy = inter[ns+1].c;
		} else {
			dy = inter[ns].d;
			ns = ns - 1;
		};
		yr = yr + dy;
	};
	*yy = yr;
	delete inter;
	delete x;
	delete y;

	return TRUE;
}

bool CPolyCoord::QuadraticInterpolation(int nSide,double xx,double *yy)
{
	int	i,m,ns;
	float	dif,dift;
	InterpolationData	*inter;
	float	w,dy,yr,dxx,ho,hp,den;

	inter = new InterpolationData[m_nCoordL+2];
	float *x = new float[m_nCoordL+2];
	float *y = new float[m_nCoordL+2];

	for(i=0;i<m_nCoordL;i++) {
		x[i+1]=m_pCp[i].dXp[nSide];
		y[i+1]=m_pCp[i].dZp[nSide];
	};
	ns = 1;
	dif = fabs(xx - m_pCp[0].dXp[nSide]);
	for(i=1;i<=m_nCoordL;i++) {
		dift = fabs(xx - m_pCp[i-1].dXp[nSide]);
		if(dift < dif) {
			ns = i;
			dif = dift;
		};
		inter[i].d = inter[i].c = y[i];
	};
	yr=y[ns];
	ns--;
	dxx=xx;
	for(m=1;m<=(m_nCoordL-1);m++) {
		for(i=1;i<=(m_nCoordL-m);i++) {
			ho = x[i] - dxx;
			hp = x[i+m] - dxx;
			w = inter[i+1].c - inter[i].d;
			den = ho - hp;
			if(den == 0l) {
				delete inter;
				delete x;
				delete y;
				return(FALSE);
			};
			inter[i].d = (hp * w)/den;
			inter[i].c = (ho * w)/den;
		};
		if((2*ns) < (m_nCoordL-m)) {
			dy = inter[ns+1].c;
		} else {
			dy = inter[ns].d;
			ns = ns - 1;
		};
		yr = yr + dy;
	};
	*yy = yr;
	delete inter;
	delete x;
	delete y;

	return TRUE;
}
*/


void CPolyCoord::SetPrimaryAxis(int nAxis)
{

	if(m_nPrimaryAxis - nAxis) {
		m_nPrimaryAxis = nAxis;
		m_bModified = TRUE;
	}

}

float CPolyCoord::GetLength()
{
	if(((m_nCoordL>1) && (m_fLinearLength<=0.0f)) || (m_bModified==TRUE)) {
		m_bModified = TRUE;
		PreProcessLine();
	}
	return m_fLinearLength;
}

void CPolyCoord::FindPeakAmplitude(CCoord *pCp,int nMode)
{
	int	nMax=0;
	int	nMin=10000;
	int nThreshold;
	int nMinPtr0;
	int nMinPtr1;

	m_nMaxPtr = 0;

	for(int ii=0;ii<m_nCoordL;ii++) {
		if(m_pCp[ii].nAmp>nMax) {
			nMax = m_pCp[ii].nAmp;
			m_nMaxPtr = ii;
		}
		if(m_pCp[ii].nAmp>=0) {
			if(m_pCp[ii].nAmp<nMin) nMin = m_pCp[ii].nAmp;
		}
	}
	if(nMode == 1) {
		nThreshold = (nMax+nMin)/2;
		nMinPtr0=0;
		nMinPtr1=m_nCoordL-1;
		for(int ii=m_nMaxPtr;ii>=0;ii--) {
			if(m_pCp[ii].nAmp<nThreshold) {
				nMinPtr0=ii;
				break;
			}
		}
		for(int ii=m_nMaxPtr;ii<m_nCoordL;ii++) {
			if(m_pCp[ii].nAmp<nThreshold) {
				nMinPtr1=ii;
				break;
			}
		}
		m_nMaxPtr = (nMinPtr0 + nMinPtr1) / 2;
	}
	CopyMemory(&(pCp->nStructSize),&m_pCp[m_nMaxPtr].nStructSize,sizeof COORDDATA);
}


bool CPolyCoord::Bisect(int nIndex)
{

	CCoord Cp;

	if(nIndex<(m_nCoordL-1)) {

		for(int nAxis=0;nAxis<30;nAxis++) {
			Cp.fPos[nAxis] = (m_pCp[nIndex].fPos[nAxis] + m_pCp[nIndex+1].fPos[nAxis]) / 2.0f;
		}
		D3DXVec3Normalize(&(D3DXVECTOR3)Cp.Side0.norm,&(D3DXVECTOR3)Cp.Side0.norm);
		D3DXVec3Normalize(&(D3DXVECTOR3)Cp.Side1.norm,&(D3DXVECTOR3)Cp.Side1.norm);

		InsertCoord(nIndex,Cp,0);

		return TRUE;
	}
	return FALSE;

}


void CPolyCoord::SetAllModifiy(bool bFlag)
{
	for(int ii=0;ii<m_nCoordL;ii++) {
		m_pCp[ii].bModified = bFlag;
	}

}





void CPolyCoord::DeleteCoordInArea(CCoord *pCp0, CCoord *pCp1, int nIgnoreAxis)
{

	float fX0 = pCp0->Side0.fX;
	float fX1 = pCp1->Side0.fX;
	float fY0 = pCp0->Side0.fY;
	float fY1 = pCp1->Side0.fY;
	float fZ0 = pCp0->Side0.fZ;
	float fZ1 = pCp1->Side0.fZ;
	float fTemp;

	if(fX0>fX1) {
		fTemp = fX0;
		fX0 = fX1;
		fX1 = fTemp;
	}
	if(fY0>fY1) {
		fTemp = fY0;
		fY0 = fY1;
		fY1 = fTemp;
	}
	if(fZ0>fZ1) {
		fTemp = fZ0;
		fZ0 = fZ1;
		fZ1 = fTemp;
	}
	for(int nn=0;nn<m_nCoordL;nn++) {
		switch(nIgnoreAxis) {
		case 'X':
			if ((fY0 < m_pCp[nn].Side0.fY) && (m_pCp[nn].Side0.fY < fY1) &&
				(fZ0 < m_pCp[nn].Side0.fZ) && (m_pCp[nn].Side0.fZ < fZ1)) {
				DeleteCoord(nn);
				nn--;
			}
			break;
		case 'Y':
			if((fX0 < m_pCp[nn].Side0.fX) && (m_pCp[nn].Side0.fX < fX1) && 
			   (fZ0 < m_pCp[nn].Side0.fZ) && (m_pCp[nn].Side0.fZ < fZ1)) {
				DeleteCoord(nn);
				nn--;
			}
			break;
		case 'Z':
			if ((fX0 < m_pCp[nn].Side0.fX) && (m_pCp[nn].Side0.fX < fX1) &&
				(fY0 < m_pCp[nn].Side0.fY) && (m_pCp[nn].Side0.fY < fY1)) {
				DeleteCoord(nn);
				nn--;
			}
			break;
		}
	}
}

bool CPolyCoord::DoesXPlainCutLine(float fX, CCoord *pCp0, CCoord *pCp1, CCoord &Cp)
{

	bool bFlag = FALSE;

	if(m_nCoordL<=0)
		return bFlag;

	if(m_pCp[0].Side[0].fX < m_pCp[m_nCoordL-1].Side[0].fX) {
		if((fX>=m_pCp[0].Side[0].fX) && (fX<=m_pCp[m_nCoordL-1].Side[0].fX)) bFlag = TRUE;
	} else {
		if((fX<=m_pCp[0].Side[0].fX) && (fX>=m_pCp[m_nCoordL-1].Side[0].fX)) bFlag = TRUE;
	}
	if(bFlag == FALSE) return bFlag;

	CalculateCoord(fX,Cp);

	if(Cp.Side[0].fZ>=pCp0->Side[0].fZ) return FALSE;
	if(Cp.Side[0].fZ<=pCp1->Side[0].fZ) return FALSE;

	return TRUE;

}




void CPolyCoord::ZeroAllDeltas(int nAction)
{
	int nn;

	switch(nAction) {
	case 0:
		for(nn=0;nn<m_nCoordL;nn++) {
			m_pCp[nn].ZeroSide(2);
		}
		break;
	case 1:
		for(nn=0;nn<m_nCoordL;nn++) {
			m_pCp[nn].Delta.fX0=0.0f;
			m_pCp[nn].Delta.fY0=0.0f;
			m_pCp[nn].Delta.fZ0=0.0f;
		}
		break;
	case 2:
		for(nn=0;nn<m_nCoordL;nn++) {
			m_pCp[nn].Delta.fX1=0.0f;
			m_pCp[nn].Delta.fY1=0.0f;
			m_pCp[nn].Delta.fZ1=0.0f;
		}
		break;
	case 3:
		for(nn=0;nn<m_nCoordL;nn++) {
			m_pCp[nn].Delta.fI=0.0f;
			m_pCp[nn].Delta.fJ=0.0f;
			m_pCp[nn].Delta.fK=0.0f;
		}
		break;
	}

}

float CPolyCoord::GetSplineLength(int nNumberOfSegs)
{
	int nn;
	float fLength=0.0f;
	CCoord Cp0,Cp1;
	float fPos;

	PreProcessLine();

	Cp0 = m_pCp[0];
	for(nn=1;nn<nNumberOfSegs;nn++) {
		fPos = ((m_pCp[m_nCoordL-1].fPos[m_nPrimaryAxis] - m_pCp[0].fPos[m_nPrimaryAxis]) * (float)nn / (float)(nNumberOfSegs-1)) + m_pCp[0].fPos[m_nPrimaryAxis];
		CalculateCoord(fPos,Cp1);
		fLength += Cp0.Length(Cp1,0);
		Cp0=Cp1;
	}
	return fLength;
}

void CPolyCoord::CalculateCoord(CCoord Cp0, float fIncrement, CCoord *pCp1)
{
	float fPos,fLength,fInc,fError;
	CCoord Cp;
	bool bFlag=TRUE;
	int	nn=0;

	fInc = fIncrement;
	fError = fabs(fIncrement / 100.0f);

	while(bFlag) {
		nn++;
		fPos = Cp0.fPos[m_nPrimaryAxis] + fInc;
		CalculateCoord(fPos,Cp);
		fLength = Cp0.Length(Cp,0);

		if(fIncrement >= 0.0f) {
			if(fabs(fIncrement-fLength) > fError) {
				if(fIncrement<fLength) {
					fInc += (fIncrement-fLength);
					bFlag=TRUE;
				} else {
					fInc += (fIncrement-fLength);
					bFlag=TRUE;
				}
			} else {
				bFlag = FALSE;
			}
		} else {
			if(fabs(fabs(fIncrement)-fLength) > fError) {
				if(fabs(fIncrement)<fLength) {
					fInc += (fLength - fabs(fIncrement));
					bFlag=TRUE;
				} else {
					fInc -= (fabs(fIncrement)-fLength);
					bFlag=TRUE;
				}
			} else {
				bFlag = FALSE;
			}
		}
		if(nn>20) {
			ASSERT(FALSE);
		}
	}
//	Cp.UnityVector();
	Cp.LimitIJK();
	CopyMemory(pCp1,&Cp,sizeof Cp);

}

void CPolyCoord::CalculateXtip(int nIndex)
{
	float fX = m_pCp[nIndex+1].Side0.fX - m_pCp[nIndex-1].Side0.fX;
	float fY = m_pCp[nIndex-1].Side0.fY - m_pCp[nIndex+1].Side0.fY;

	float fXAngle = atan(fY/fX) * RAD_TO_DEG;

	m_pCp[nIndex].Side0.fXt = m_pCp[nIndex].Side1.fXt = fXAngle;

	theApp.m_Kinematics.NormalsFromTips((CCoord*)&m_pCp[nIndex],PORTSIDE);
	theApp.m_Kinematics.NormalsFromTips((CCoord*)&m_pCp[nIndex],STARBOARD);
}

void CPolyCoord::Straighten(int nIndex0, int nIndex1)
{
	float fX[2],fY[2],fZ[2];
	int nSide,nn;

	if((nIndex1-nIndex0) < 1) return;

	for(nSide=0;nSide<2;nSide++) {
		fX[nSide] = m_pCp[nIndex1].Side[nSide].fX - m_pCp[nIndex0].Side[nSide].fX;
		fY[nSide] = m_pCp[nIndex1].Side[nSide].fY - m_pCp[nIndex0].Side[nSide].fY;
		fZ[nSide] = m_pCp[nIndex1].Side[nSide].fZ - m_pCp[nIndex0].Side[nSide].fZ;
	}

	for(nSide=0;nSide<2;nSide++) {
		for(nn=nIndex0;nn<=nIndex1;nn++) {
			m_pCp[nn].Side[nSide].fY = (fY[nSide] * (m_pCp[nn].Side[nSide].fX - m_pCp[nIndex0].Side[nSide].fX) / fX[nSide]) + m_pCp[nIndex0].Side[nSide].fY;
			m_pCp[nn].Side[nSide].fZ = (fZ[nSide] * (m_pCp[nn].Side[nSide].fX - m_pCp[nIndex0].Side[nSide].fX) / fX[nSide]) + m_pCp[nIndex0].Side[nSide].fZ;
		}
	}
}

void CPolyCoord::GenerateSineWave(int nMode,CCoord *pCp, float fPitch, float fMaxAmp, float fPhase, float fSeg,float fCycles)
{
	float fLength;
	float fOmega,fAmp,fPos,fTangent,fAmp1;
	CCoord Cp;

	fOmega = (1000.0f / fPitch) * 2.0f * PIf;
	fPhase = fPhase * 2.0f * PIf / 360.0f;

	switch(nMode) {
	case 0:	fLength = pCp[0].Length(pCp[1]);
		break;
	case 1: fLength = fPitch * fCycles;
		break;
	}

	Cp = pCp[0];

	for(fPos=0.0;fPos<=fLength;fPos+=fSeg) {

		fAmp = fMaxAmp * sinf(fOmega * fPos / 1000.0f + fPhase);
		fAmp1 = fMaxAmp * sinf(fOmega * (fPos+0.1f) / 1000.0f + fPhase);

//		fTangent = 90.0f * cos(fOmega * fPos / 1000.0 + fPhase) * (fMaxAmp / fPitch) * 4.0f;
//		fI = 1.0f * cos(fOmega * fPos / 1000.0 + fPhase) * (fMaxAmp / fPitch) * 4.0f;

		fTangent = atanf((fAmp-fAmp1)/0.1f) * 180.0f / PIf;

		Cp.Side0.fX = Cp.Side1.fX = pCp[0].Side0.fX + fPos;
		Cp.Side0.fY = Cp.Side1.fY = pCp[0].Side0.fY + fAmp;
		Cp.Side0.fZ = Cp.Side1.fZ = pCp[0].Side0.fZ;

		Cp.Side0.fXt = Cp.Side1.fXt = fTangent;
		Cp.Side0.fYt = Cp.Side1.fYt = 0.0f;
		theApp.m_Kinematics.NormalsFromTips(&Cp,0);
		theApp.m_Kinematics.NormalsFromTips(&Cp,1);

		Add(Cp);
	}

}

void CPolyCoord::GetElectronicValues(CCoord *pCp, int nAxis)
{
	int k,kLo,kHi,gg;
	float fEleckLo,fEleckHi;


	kHi=m_nCoordL-1;
	kLo=0;

	while((kHi-kLo)>1) {
		k=(kHi+kLo)/2;
		if(m_pCp[kHi].fPos[nAxis] > m_pCp[kLo].fPos[nAxis]) { 
			(m_pCp[k].fPos[nAxis] > pCp->fPos[nAxis])?(kHi=k):(kLo=k);
		} else {
			(m_pCp[k].fPos[nAxis] < pCp->fPos[nAxis])?(kHi=k):(kLo=k);
		}
	};
	for(gg=0;gg<14;gg++) {
		fEleckLo = (float)m_pCp[kLo].nElec[gg];
		fEleckHi = (float)m_pCp[kHi].nElec[gg];
		pCp->nElec[gg]=(int)(((pCp->fPos[nAxis]-m_pCp[kLo].fPos[nAxis]) * (fEleckHi-fEleckLo)) / (m_pCp[kHi].fPos[nAxis]-m_pCp[kLo].fPos[nAxis]) + fEleckLo);
	}

}

void CPolyCoord::SetAllWp(int nSide, float fWp)
{
	for(int ii=0;ii<m_nCoordL;ii++) {
		m_pCp[ii].fWp[nSide] = fWp;
	}

}

void CPolyCoord::ChangeSignOfAnElement(char cWhich)
{
	for(int ii=0;ii<m_nCoordL;ii++) {
		switch(cWhich) {
		case 'i':
			m_pCp[ii].Side0.fI *= -1;
			m_pCp[ii].Side1.fI *= -1;
			break;
		case 'j':
			m_pCp[ii].Side0.fJ *= -1;
			m_pCp[ii].Side1.fJ *= -1;
			break;
		case 'k':
			m_pCp[ii].Side0.fK *= -1;
			m_pCp[ii].Side1.fK *= -1;
		}
	}
}



float CPolyCoord::FindNearestSegment(CCoord Cp, int nSide, int *pnSegment)
{
	
	float fLengthHi,fLengthLo,fLength;
	int k,kLo,kHi;

	fLength = 1000000000.0f;

	for(k=0;k<m_nCoordL;k++) {
		fLengthLo = DistanceToSegment(Cp,nSide,k);
		if(fLength > fLengthLo) {
			fLength = fLengthLo;
			*pnSegment = k;
		}
	}
	return fLength;




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
		if(fLength > fLengthHi) fLength = fLengthHi;
		if(fLength > fLengthLo) fLength = fLengthLo;
	}
	if(fLength > fLengthHi) fLength = fLengthHi;
	if(fLength > fLengthLo) fLength = fLengthLo;
	return fLength;

}

float CPolyCoord::DistanceToSegment(CCoord Cp, int nSide, int nSegment)
{
	D3DXVECTOR3 V,W;
	int ii=nSegment;
	int jj=ii+1;

	if(nSegment>=(m_nCoordL-1)) {
		W = D3DXVECTOR3(       Cp.Side[nSide].fX - m_pCp[ii].Side[nSide].fX,        Cp.Side[nSide].fY - m_pCp[ii].Side[nSide].fY,        Cp.Side[nSide].fZ - m_pCp[ii].Side[nSide].fZ);
		return D3DXVec3Length(&W);

	} else {

		V = D3DXVECTOR3(m_pCp[jj].Side[nSide].fX - m_pCp[ii].Side[nSide].fX, m_pCp[jj].Side[nSide].fY - m_pCp[ii].Side[nSide].fY, m_pCp[jj].Side[nSide].fZ - m_pCp[ii].Side[nSide].fZ);
		W = D3DXVECTOR3(       Cp.Side[nSide].fX - m_pCp[ii].Side[nSide].fX,        Cp.Side[nSide].fY - m_pCp[ii].Side[nSide].fY,        Cp.Side[nSide].fZ - m_pCp[ii].Side[nSide].fZ);

		float fC1 = D3DXVec3Dot(&W, &V);
		if(fC1<=0) {
			return D3DXVec3Length(&W);
		}

		float fC2 = D3DXVec3Dot(&V, &V);
		if(fC2<=fC1) {
			W = D3DXVECTOR3(   Cp.Side[nSide].fX - m_pCp[jj].Side[nSide].fX,        Cp.Side[nSide].fY - m_pCp[jj].Side[nSide].fY,        Cp.Side[nSide].fZ - m_pCp[jj].Side[nSide].fZ);
			return D3DXVec3Length(&W);
		}

		float fB = fC1 / fC2;
		V = V * fB;
		D3DXVECTOR3 Pb(m_pCp[ii].Side[nSide].fX + V.x,m_pCp[ii].Side[nSide].fY + V.y,m_pCp[ii].Side[nSide].fZ + V.z);
		W = D3DXVECTOR3(Cp.Side[nSide].fX-Pb.x, Cp.Side[nSide].fY-Pb.y, Cp.Side[nSide].fZ-Pb.z);
		return D3DXVec3Length(&W);
	}

	return -1.0f;

}

void CPolyCoord::DecimateUsingZPos(float fZVariation)
{
	float fLength;
	int ii;

	int nS = 0;
	for(ii=0;ii<m_nCoordL;ii++) {
		fLength = fabsf(m_pCp[ii].Side0.fZ-m_pCp[nS].Side0.fZ);
		if(fLength >= fZVariation) {
			DeleteCoords(nS+1,ii-1);
			ii=nS+1;
			nS++;
		}
	}
	if(ii==m_nCoordL) {
		if(((m_nCoordL-2) - (nS+1)) > 0) {
			DeleteCoords(nS+1,m_nCoordL-2);
		}
	}
			

}

void CPolyCoord::SetAllNorms(int nSide, D3DXVECTOR4 norm)
{

	for(int ii=0;ii<m_nCoordL;ii++) {
		m_pCp[ii].Side[nSide].norm = norm;
	}
}

int CPolyCoord::GetSize()
{

	return m_nCoordL;

}
bool CPolyCoord::AddEntity(EntityStruct *pEntity,int nEntityNumber)
{
	float K,M,N,A;
	CCoord Cp;
	int nn,nPtr;
	float *pfWeight;
	Cp.Zero();

	m_nEntityNumber = nEntityNumber;
	switch(pEntity->nEntityType) {
	default: return FALSE;
		break;
	case 110:	//Line;
		for(nn=0,nPtr=0;nn<2;nn++,nPtr+=2) {
			Cp.Side0.fX=pEntity->fData[nPtr];
			Cp.Side0.fY=pEntity->fData[nPtr+1];
			Cp.Side0.fZ=pEntity->fData[nPtr+2];
			Add(Cp);
		}
		return TRUE;
		break;
	case 126:	//NURB Curve
		K = (float)((int)pEntity->fData[0]);
		M = (float)((int)pEntity->fData[1]);				//0				1
		m_nP1Curve = (int)pEntity->fData[2];	//Non			Planar
		m_nP2Curve = (int)pEntity->fData[3];	//Open curve	Closed curve
		m_nP3Curve = (int)pEntity->fData[4];	//rational		Polynomial
		m_nP4Curve = (int)pEntity->fData[5];	//non			Periodic
		N = 1 + K - M;
		A = N + 2 * M;
		m_pfKnots = new float[(int)A+1];
		m_nKnotsL = (int)A+1;
		for(nn=0,nPtr=6;nn<=A;nn++,nPtr++) {
			m_pfKnots[nn]=pEntity->fData[nPtr];
		}
		pfWeight = new float[(int)K+1];
		for(nn=0;nn<=K;nn++,nPtr++) {
			pfWeight[nn]=pEntity->fData[nPtr];
		}
		for(nn=0;nn<=K;nn++,nPtr+=3) {
			Cp.Side0.fX=pEntity->fData[nPtr];
			Cp.Side0.fY=pEntity->fData[nPtr+1];
			Cp.Side0.fZ=pEntity->fData[nPtr+2];
			Cp.fWeight = pfWeight[nn];
			Add(Cp);
		}
		m_fV0=pEntity->fData[nPtr++];
		m_fV1=pEntity->fData[nPtr++];
		m_fXNorm=pEntity->fData[nPtr++];
		m_fYNorm=pEntity->fData[nPtr++];
		m_fZNorm=pEntity->fData[nPtr++];

		m_nCurveOrder = pEntity->nFormNumber;	//0:rational 1:Line 2:Circular 3:Elliptical 4:Parabolic 5:Hyperbolic

		delete pfWeight;
		return TRUE;
		break;
	}

	return FALSE;
}

void CPolyCoord::TransformLine(D3DXMATRIXA16 & matWorld)
{
	D3DXMATRIXA16 matRotate = matWorld;
	matRotate._41 = 0.0f;
	matRotate._42 = 0.0f;
	matRotate._43 = 0.0f;
	matRotate._44 = 1.0f;

	for(int ii=0;ii<m_nCoordL;ii++) {
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_pCp[ii].Side0.pt,	(D3DXVECTOR3*)&m_pCp[ii].Side0.pt,	&matWorld);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_pCp[ii].Side0.norm,	(D3DXVECTOR3*)&m_pCp[ii].Side0.norm,&matRotate);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_pCp[ii].Side1.pt,	(D3DXVECTOR3*)&m_pCp[ii].Side1.pt,	&matWorld);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_pCp[ii].Side1.norm,	(D3DXVECTOR3*)&m_pCp[ii].Side1.norm,&matRotate);
		theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_pCp[ii],PORTSIDE);
		theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_pCp[ii],STARBOARD);
	}

	m_Matrix *= matWorld;
	m_Matrix *= matRotate;

}


void CPolyCoord::Extrapolate(int nEnd, float fLength,int nStyle)
{
	D3DXVECTOR3 vec;
//	COORDDATA Cp;
	CCoord Cp;
	int kLo,kHi,nOldStyle,nOldPrimaryAxis;


	switch(nStyle) {
	case 0:
		if(m_nCoordL<2) return;

		if(nEnd == 0) {
			kLo = 0;
			kHi = 1;
			Cp=m_pCp[kLo];
			fLength*=-1.0f;
		} else {
			kLo=m_nCoordL-2;
			kHi=m_nCoordL-1;
			Cp=m_pCp[kHi];
		}

		vec = m_pCp[kHi].Side0.pt - m_pCp[kLo].Side0.pt;
		D3DXVec3Normalize(&vec,&vec);
		vec *= fLength;
		Cp.Side0.pt += vec;
		Cp.Side1.pt += vec;
		break;
	case 1:
		if(m_nCoordL<3) return;

		nOldStyle = m_nStyle;
		nOldPrimaryAxis = m_nPrimaryAxis;
		m_nStyle = 1;
		m_nPrimaryAxis = 30;
		m_bModified = TRUE;

		if(nEnd == 0) {
			fLength *= -1.0f;
		} else {
			fLength += GetLength();
		};
		CalculateCoord(fLength,Cp);

		m_nPrimaryAxis = nOldPrimaryAxis;
		m_nStyle = nOldStyle;
		break;
	}

	if(nEnd == 1) {
		Add(Cp);
	} else {
		InsertCoord(0,Cp,1);
	}
}

void CPolyCoord::StripOutZeroCoords()
{
	int mm;

	for(int nn=m_nCoordL-1;nn>=0;nn--) {
		if(m_pCp[nn].IsZero() == true) {
			for(mm=nn-1; mm>=0 && m_pCp[mm].IsZero(); mm--) {};
			if(mm<0) mm = 0;
			DeleteCoords(mm, nn);
			nn=m_nCoordL;
		}
	}
}

void CPolyCoord::ComputeBothTipsFromNormals(int nPtr)
{
	theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_pCp[nPtr],PORTSIDE);
	theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_pCp[nPtr],STARBOARD);
}

void CPolyCoord::ComputePolarValues(D3DXVECTOR3 vecZero)
{

	for(int ii=0;ii<m_nCoordL;ii++) {

		m_pCp[ii].CalculatePolarAboutZ(vecZero,PORTSIDE);
		m_pCp[ii].CalculatePolarAboutZ(vecZero,STARBOARD);

	}

}

float CPolyCoord::HowFarAlongLineIsCoordinate(CCoord Cp, int nAxis)
{

	float fTotal = 0.0f;
	float fVertexLength;
	D3DXVECTOR3 W;

	for(int ii = 0;ii<(m_nCoordL-1);ii++) {

		W = m_pCp[ii+1].Side0.pt - m_pCp[ii].Side0.pt;
		fVertexLength = D3DXVec3Length(&W);

		if((Cp.fPos[nAxis] >= m_pCp[ii].fPos[nAxis]) && (Cp.fPos[nAxis] < m_pCp[ii+1].fPos[nAxis])) {
			fTotal += (fVertexLength * (Cp.fPos[nAxis] - m_pCp[ii].fPos[nAxis]) / (m_pCp[ii+1].fPos[nAxis] - m_pCp[ii].fPos[nAxis]));
			return fTotal;
		} else {
			fTotal += fVertexLength;
		}
	}
	return fTotal;
}

void CPolyCoord::SetAllCoords(CCoord Cp, int nSide)
{
	int ii;

	switch(nSide) {
	case 0:
	case 1:
		for(ii=0;ii<m_nCoordL;ii++) {
			m_pCp[ii].Side[nSide] = Cp.Side[nSide];
		}
		break;
	default:
		for(ii=0;ii<m_nCoordL;ii++) {
			m_pCp[ii] = Cp;
		}
		break;
	}

}

void CPolyCoord::SetAllCoordsButNotR(CCoord Cp, int nSide)
{
	int ii;
	float fR;

	switch(nSide) {
	case 0:
	case 1:
		for(ii=0;ii<m_nCoordL;ii++) {
			fR=m_pCp[ii].Side[nSide].fR;
			m_pCp[ii].Side[nSide] = Cp.Side[nSide];
			m_pCp[ii].Side[nSide].fR=fR;
		}
		break;
	default:
		for(ii=0;ii<m_nCoordL;ii++) {
			m_pCp[ii] = Cp;
		}
		break;
	}

}

void CPolyCoord::ProjectToYPlane(int nEnd, float fYequals)
{
	D3DXVECTOR3 vec;
	COORDDATA Cp;
	if(m_nCoordL<2) return;
	float fAngle,fHypot;

	if(nEnd == 0) {
		vec = m_pCp[0].Side0.pt - m_pCp[1].Side0.pt;
		Cp=m_pCp[0];
	} else {
		vec = m_pCp[m_nCoordL-1].Side0.pt - m_pCp[m_nCoordL-2].Side0.pt;
		Cp=m_pCp[m_nCoordL-1];
	}

	float fAdjacent = fYequals - Cp.Side0.fY;
	D3DXVec3Normalize(&vec,&vec);

	if(((fAdjacent>0.0f) && (vec.y > 0.0f)) || ((fAdjacent<0.0f) && (vec.y < 0.0f))) {
		fAdjacent = fabsf( fAdjacent );
		Cp.Side0.fY = Cp.Side1.fY = fYequals;
		fAngle = atan2f(vec.x,vec.y);
		fHypot = fAdjacent / cosf(fAngle);
		Cp.Side0.fX = Cp.Side0.fX + fHypot * sinf(fAngle);
		Cp.Side1.fX = Cp.Side1.fX + fHypot * sinf(fAngle);
		fAngle = atan2f(vec.z,vec.y);
		fHypot = fAdjacent / cosf(fAngle);
		Cp.Side0.fZ = Cp.Side0.fZ + fHypot * sinf(fAngle);
		Cp.Side1.fZ = Cp.Side1.fZ + fHypot * sinf(fAngle);

		if(nEnd == 1) {
			Add(Cp);
		} else {
			InsertCoord(0,Cp,1);
		}
	}
}

void CPolyCoord::Crop(int nLessThanGreaterThan, float fPos, int nPlane)
{
	int nn,nAxis;
	CCoord Cp;

	switch(nLessThanGreaterThan) {
	case 0:
		for(nn=0;nn<(m_nCoordL-2); nn++) {
			if(DoesPlaneCutSegment(fPos, nn, nPlane) == true) {
				Cp = m_pCp[nn+1];
				for(nAxis=0;nAxis<30;nAxis++) {
					Cp.fPos[nAxis]=((fPos-m_pCp[nn+1].fPos[nPlane]) * (m_pCp[nn].fPos[nAxis]-m_pCp[nn+1].fPos[nAxis])) / (m_pCp[nn].fPos[nPlane]-m_pCp[nn+1].fPos[nPlane]) + m_pCp[nn+1].fPos[nAxis];
				}
				D3DXVec3Normalize((D3DXVECTOR3*)&Cp.Side0.norm,(D3DXVECTOR3*)&Cp.Side0.norm);
				D3DXVec3Normalize((D3DXVECTOR3*)&Cp.Side1.norm,(D3DXVECTOR3*)&Cp.Side1.norm);
				DeleteCoords(0,nn-1);
				if(m_nCoordL>0) m_pCp[0] = Cp;
				break;
			}
		}
		break;
	case 1:
		for(nn=m_nCoordL-1;nn>1; nn--) {
			if(DoesPlaneCutSegment(fPos, nn-1, nPlane) == true) {
				Cp = m_pCp[nn-1];
				for(nAxis=0;nAxis<30;nAxis++) {
					Cp.fPos[nAxis]=((fPos-m_pCp[nn-1].fPos[nPlane]) * (m_pCp[nn].fPos[nAxis]-m_pCp[nn-1].fPos[nAxis])) / (m_pCp[nn].fPos[nPlane]-m_pCp[nn-1].fPos[nPlane]) + m_pCp[nn-1].fPos[nAxis];
				}
				D3DXVec3Normalize((D3DXVECTOR3*)&Cp.Side0.norm,(D3DXVECTOR3*)&Cp.Side0.norm);
				D3DXVec3Normalize((D3DXVECTOR3*)&Cp.Side1.norm,(D3DXVECTOR3*)&Cp.Side1.norm);
				DeleteCoords(nn+1,m_nCoordL-1);
				m_pCp[m_nCoordL-1] = Cp;
				break;
			}
		}
		break;
	}

}

bool CPolyCoord::DoesPlaneCutSegment(float fPos, int nSegment, int nPlane)
{

	if(m_nCoordL<=0)
		return false;

	if(m_pCp[nSegment].Side[0].fPos[nPlane] < m_pCp[nSegment+1].Side[0].fPos[nPlane]) {
		if((fPos>=m_pCp[nSegment].Side[0].fPos[nPlane]) && (fPos<=m_pCp[nSegment+1].Side[0].fPos[nPlane])) return true;
	} else {
		if((fPos<=m_pCp[nSegment].Side[0].fPos[nPlane]) && (fPos>=m_pCp[nSegment+1].Side[0].fPos[nPlane])) return true;
	}
	return false;

}


void CPolyCoord::ConvertLeftHandedToRightHanded()
{
	D3DXMATRIXA16 matMirror;

	D3DXMatrixIdentity(&matMirror);
	matMirror._11 = -1.0f;

	for(int ii=0;ii<m_nCoordL;ii++) {
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_pCp[ii].Side0.pt,	(D3DXVECTOR3*)&m_pCp[ii].Side0.pt,	&matMirror);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_pCp[ii].Side0.norm,	(D3DXVECTOR3*)&m_pCp[ii].Side0.norm,&matMirror);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_pCp[ii].Side1.pt,	(D3DXVECTOR3*)&m_pCp[ii].Side1.pt,	&matMirror);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_pCp[ii].Side1.norm,	(D3DXVECTOR3*)&m_pCp[ii].Side1.norm,&matMirror);
		theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_pCp[ii],PORTSIDE);
		theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_pCp[ii],STARBOARD);
	}


}

bool CPolyCoord::IsChangeInDir(int nIndex, int nAxis)
{

	if((m_pCp[nIndex-1].fPos[nAxis] <= m_pCp[nIndex].fPos[nAxis]) && (m_pCp[nIndex+2].fPos[nAxis] <= m_pCp[nIndex+1].fPos[nAxis])) return true;
	if((m_pCp[nIndex-1].fPos[nAxis] >= m_pCp[nIndex].fPos[nAxis]) && (m_pCp[nIndex+2].fPos[nAxis] >= m_pCp[nIndex+1].fPos[nAxis])) return true;
	return false;
}

bool CPolyCoord::IsChangeInPos(int nIndex, int nAxis, float fDelta)
{
	if(fabs(m_pCp[nIndex].fPos[nAxis] - m_pCp[nIndex + 1].fPos[nAxis]) >= fDelta) return true;
	return false;

}

void CPolyCoord::Add(int nWhich_fPos, float fDelta)
{
	for(int ii=0;ii<m_nCoordL;ii++) {
		m_pCp[ii].fPos[nWhich_fPos] += fDelta;
	}

}

void CPolyCoord::SetNormalsTo(int nWhichNormalComponent, float fValue)
{
	for(int ii=0;ii<m_nCoordL;ii++) {
		switch(nWhichNormalComponent) {
		case 0:
			m_pCp[ii].Side0.fI = fValue;
			m_pCp[ii].Side1.fI = fValue;
			break;
		case 1:
			m_pCp[ii].Side0.fJ = fValue;
			m_pCp[ii].Side1.fJ = fValue;
			break;
		case 2:
			m_pCp[ii].Side0.fK = fValue;
			m_pCp[ii].Side1.fK = fValue;
			break;
		}
		D3DXVec3Normalize((D3DXVECTOR3*)&m_pCp[ii].Side0.norm,(D3DXVECTOR3*)&m_pCp[ii].Side0.norm);
		D3DXVec3Normalize((D3DXVECTOR3*)&m_pCp[ii].Side1.norm,(D3DXVECTOR3*)&m_pCp[ii].Side1.norm);
	}

}

void CPolyCoord::FindMinimumYCoord(CCoord &Cp)
{
	float fYMin=1e9f;
	float fPos;
	CCoord CpLocal;

	for(fPos=0.0f;fPos<GetLength();fPos+=0.1f) {
		CalculateCoord(fPos,CpLocal);
		if(CpLocal.Side0.fY < fYMin) {
			fYMin = CpLocal.Side0.fY;
			Cp = CpLocal;
			Cp.fLength = fPos;
		}
	}
}

void CPolyCoord::DeleteAnyCoordWithYgreaterThan(float fY,float fZMin, float fZMax)
{

	for(int ii=0;ii<m_nCoordL;ii++) {
		if((m_pCp[ii].Side0.fY > fY) && (m_pCp[ii].Side0.fZ > fZMin) && (m_pCp[ii].Side0.fZ < fZMax)){
			DeleteCoord(ii);
			ii--;
		}
	}

}

void CPolyCoord::ShiftAllCoordsAlongVector(float fDistance)
{
	D3DXVECTOR3 vec;
	for(int nn=0;nn<m_nCoordL;nn++) {
		vec = (D3DXVECTOR3)m_pCp[nn].Side0.norm;
		m_pCp[nn].Side0.pt += (vec * fDistance);

		vec = (D3DXVECTOR3)m_pCp[nn].Side1.norm;
		m_pCp[nn].Side1.pt += (vec * fDistance);
	};
}

CString CPolyCoord::GetString()
{
	CString Buff;

	Buff.Format(L"%s",m_wStr);
	return Buff;
}

void CPolyCoord::SetString(CString Buff)
{
	if(Buff.GetLength() < LINE_STR_LENGTH) {
		swprintf_s(m_wStr,256,L"%s",(LPCWSTR)Buff);
	}

}

char* CPolyCoord::GetCharString()
{
	int ii;
	static char str[LINE_STR_LENGTH];
	
	for(ii=0;ii<m_wStr[ii] != 0;ii++) {
		str[ii] = (char)m_wStr[ii];
	}
	str[ii] = 0;

	return str;
}


void CPolyCoord::RotateAllNormalsToIEqualsZero()
{
	D3DXMATRIXA16 matRotate;
	float fAngle;

	for(int nn=0;nn<m_nCoordL;nn++) {
		fAngle = atan2f(m_pCp[nn].Side0.fI,-m_pCp[nn].Side0.fJ);
		D3DXMatrixRotationZ(&matRotate,-fAngle);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_pCp[nn].Side0.norm,	(D3DXVECTOR3*)&m_pCp[nn].Side0.norm,&matRotate);
		D3DXVec3Normalize((D3DXVECTOR3*)&m_pCp[nn].Side0.norm,	(D3DXVECTOR3*)&m_pCp[nn].Side0.norm);

		fAngle = atan2f(m_pCp[nn].Side1.fI,-m_pCp[nn].Side1.fJ);
		D3DXMatrixRotationZ(&matRotate,-fAngle);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_pCp[nn].Side1.norm,	(D3DXVECTOR3*)&m_pCp[nn].Side1.norm,&matRotate);

		theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_pCp[nn],PORTSIDE);
		theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_pCp[nn],STARBOARD);
	}
}

void CPolyCoord::SetSizeToMaxAccessed()
{
	DeleteCoords(m_nMaxAccessed+1,m_nCoordL);
}

void CPolyCoord::DeleteCoordsWith(int nWhichSign, char cNorm)
{

	int nn,ii,nStart,nFinish;

	switch(cNorm) {
	case 'i': nn = 6;
		break;
	case 'j': nn = 7;
		break;
	case 'k': nn = 8;
		break;
	}

	for(ii=0;ii<m_nCoordL;ii++) {
		if(((nWhichSign == POSITIVE) && (m_pCp[ii].fPos[nn] >= 0.0f)) || ((nWhichSign == NEGATIVE) && (m_pCp[ii].fPos[nn] <= 0.0f))) {
			nStart = ii;
			for( ;ii<m_nCoordL;ii++) {
				if(((nWhichSign == POSITIVE) && (m_pCp[ii].fPos[nn] >= 0.0f)) || ((nWhichSign == NEGATIVE) && (m_pCp[ii].fPos[nn] <= 0.0f))) {
					nFinish = ii;
				} else {
					break;
				}
			}
			DeleteCoords(nStart,nFinish);
			ii=-1;
		}
	}
}

void CPolyCoord::SetAllRPos(int nSide, float fAngle)
{
	for(int ii=0;ii<m_nCoordL;ii++) {
		m_pCp[ii].Side[nSide].fR = fAngle;
	}

}

void CPolyCoord::AddAllRPos(int nSide, float fAngle)
{
	for(int ii=0;ii<m_nCoordL;ii++) {
		m_pCp[ii].Side[nSide].fR += fAngle;
	}

}

void CPolyCoord::DeleteAnyCoordXtYtGreater(float fAngle)
{
	int ii,nStart,nFinish;

	for(ii=0;ii<m_nCoordL;ii++) {
		if((fabs(m_pCp[ii].Side0.fXt) >= fAngle) || (fabs(m_pCp[ii].Side0.fYt) >= fAngle)) {
			nStart = ii;
			for( ;ii<m_nCoordL;ii++) {
				if((fabs(m_pCp[ii].Side0.fXt) >= fAngle) || (fabs(m_pCp[ii].Side0.fYt) >= fAngle)) {
					nFinish = ii;
				} else {
					break;
				}
			}
			DeleteCoords(nStart,nFinish);
			ii=-1;
		}
	}


}

void CPolyCoord::BladeEdgeClip()
{
	
	int nPtr,ii,nStart,nFinish;
	COORDDATA Cp;

	if(m_pCp==0) return;
	if(m_nCoordL<3) return;

	//Find left edge
	nPtr = m_nCoordL / 3;
	Cp = m_pCp[nPtr];
	/*
	for(ii=0;ii<nPtr;ii++) {
		if(Signf(m_pCp[ii].Side0.fI) != Signf(Cp.Side0.fI)) {
			nStart = ii;
			for( ;ii<nPtr;ii++) {
				if(Signf(m_pCp[ii].Side0.fI) != Signf(Cp.Side0.fI)) {
					nFinish = ii;
				} else {
					break;
				}
			}
			DeleteCoords(nStart,nFinish);
			ii=-1;
		}
	}
*/
	//find right edge
	nPtr = (m_nCoordL * 2) / 3;
	Cp = m_pCp[nPtr];
	/*
	for(ii=nPtr;ii<m_nCoordL;ii++) {
		if(Signf(m_pCp[ii].Side0.fI) != Signf(Cp.Side0.fI)) {
			nStart = ii;
			for( ;ii<m_nCoordL;ii++) {
				if(Signf(m_pCp[ii].Side0.fI) != Signf(Cp.Side0.fI)) {
					nFinish = ii;
				} else {
					break;
				}
			}
			DeleteCoords(nStart,nFinish);
			ii=nPtr-1;
		}
	}
*/

	//delete left 10 mm then extrapolate 20mm
	Cp = m_pCp[0];
	nStart=0;
	for(ii=0;ii<m_nCoordL;ii++) {
		if(fabsf(m_pCp[ii].Side0.fX - Cp.Side0.fX) < 10.0f) {
			nFinish=ii;
		}
	}
	DeleteCoords(nStart,nFinish);

	if(m_pCp==0) return;
	if(m_nCoordL<3) return;

	//delete right 10 mm then extrapolate 20mm
	Cp = m_pCp[m_nCoordL-1];
	nFinish=m_nCoordL-1;
	for(ii=m_nCoordL-1;ii>=0;ii--) {
		if(fabsf(m_pCp[ii].Side0.fX - Cp.Side0.fX) < 10.0f) {
			nStart=ii;
		}
	}
	DeleteCoords(nStart,nFinish);


}

bool CPolyCoord::GetNominalActualThickness(int nPos, float *fNomThickness, float *fActualThickness)
{

	if(m_pCp == NULL) {
		return 0;
	}
	if(nPos<0) nPos = 0;
	if(nPos >= m_nCoordL) nPos = m_nCoordL-1;

	if(fNomThickness) *fNomThickness = m_pCp[nPos].fNomThickness;
	if(fActualThickness) *fActualThickness = m_pCp[nPos].fActThickness;

	return true;

}

bool CPolyCoord::GetNominalThickness(float fXPos, CString *pBuff, float *fNomThickness, float *fMinThickness, float *fMaxThickness,int *nType,bool *bExtrapolated,bool bTrafficLightCheckDuringSaving)
{
	int nKLo = 0;
	int nKHi = m_nCoordL - 1;
	int nK = (nKHi+nKLo) /2;
	float fThickness;

	if(bExtrapolated) *bExtrapolated = false;
	if(m_pCp == NULL) {
		return 0;
	}
	if(m_pCp[nKHi].Side0.fX > m_pCp[nKLo].Side0.fX) {
		if(bExtrapolated) if(fXPos > m_pCp[nKHi].Side0.fX) *bExtrapolated = true;
		if(bExtrapolated) if(fXPos < m_pCp[nKLo].Side0.fX) *bExtrapolated = true;
		while((nKHi - nKLo) > 1) {
			if(fXPos <=  m_pCp[nK].Side0.fX) {
				nKHi = nK;
			} else {
				nKLo = nK;
			}
			nK = (nKHi + nKLo) / 2;
		}
	} else {
		if(bExtrapolated) if(fXPos < m_pCp[nKHi].Side0.fX) *bExtrapolated = true;
		if(bExtrapolated) if(fXPos > m_pCp[nKLo].Side0.fX) *bExtrapolated = true;
		while((nKHi - nKLo) > 1) {
			if(fXPos >=  m_pCp[nK].Side0.fX) {
				nKHi = nK;
			} else {
				nKLo = nK;
			}
			nK = (nKHi + nKLo) / 2;
		}
	}

	fThickness = (fXPos -  m_pCp[nKLo].Side0.fX) * ( m_pCp[nKHi].fNomThickness -  m_pCp[nKLo].fNomThickness) 
									/ ( m_pCp[nKHi].Side0.fX - m_pCp[nKLo].Side0.fX);
	fThickness +=  m_pCp[nKLo].fNomThickness;
	if(nType != NULL) {
		*nType = m_pCp[nKLo].nType;
		if((m_bTLNeeded == true) && (bTrafficLightCheckDuringSaving == true)) *nType = TRAFFIC_LIGHT;
	}
	m_bTLNeeded=false;
	if(((m_pCp[nKLo].nType & TRAFFIC_LIGHT)==0) && (m_pCp[nKHi].nType & TRAFFIC_LIGHT)) {
		m_bTLNeeded = true;
	}


	if(fNomThickness) *fNomThickness = fThickness;
	if(fMinThickness) {
		fThickness = (fXPos -  m_pCp[nKLo].Side0.fX) * ( m_pCp[nKHi].fMinThickness -  m_pCp[nKLo].fMinThickness) 
										/ ( m_pCp[nKHi].Side0.fX - m_pCp[nKLo].Side0.fX);
		fThickness +=  m_pCp[nKLo].fMinThickness;
		*fMinThickness = fThickness;
	}
	if(fMaxThickness) {
		fThickness = (fXPos -  m_pCp[nKLo].Side0.fX) * ( m_pCp[nKHi].fMaxThickness -  m_pCp[nKLo].fMaxThickness) 
										/ ( m_pCp[nKHi].Side0.fX - m_pCp[nKLo].Side0.fX);
		fThickness +=  m_pCp[nKLo].fMaxThickness;
		*fMaxThickness = fThickness;
	}

	if(pBuff) pBuff->Format(_T("Nom Thickness: %.03f mm"),fThickness);

	return true;

}

bool CPolyCoord::GetActualThickness(float fXPos, CString *pBuff, float *fActThickness)
{
	int nKLo = 0;
	int nKHi = m_nCoordL - 1;
	int nK = (nKHi+nKLo) /2;

	if(m_pCp == NULL) {
		return 0;
	}
	if(m_pCp[nKHi].Side0.fX > m_pCp[nKLo].Side0.fX) {
		while((nKHi - nKLo) > 1) {
			if(fXPos <=  m_pCp[nK].Side0.fX) {
				nKHi = nK;
			} else {
				nKLo = nK;
			}
			nK = (nKHi + nKLo) / 2;
		}
	} else {
		while((nKHi - nKLo) > 1) {
			if(fXPos >=  m_pCp[nK].Side0.fX) {
				nKHi = nK;
			} else {
				nKLo = nK;
			}
			nK = (nKHi + nKLo) / 2;
		}
	}

	float fThickness = (fXPos -  m_pCp[nKLo].Side0.fX) * ( m_pCp[nKHi].fActThickness -  m_pCp[nKLo].fActThickness) 
									/ ( m_pCp[nKHi].Side0.fX - m_pCp[nKLo].Side0.fX);
	fThickness +=  m_pCp[nKLo].fActThickness;

	if(fActThickness) *fActThickness = fThickness;
	if(pBuff) pBuff->Format(_T("Act Thickness: %.03f mm"),fThickness);

	return true;
}

//return true if extrapolated coordinate
//this is here so that this coordinate doesn't get saved to file
bool CPolyCoord::FormatBladeString(int nSample, char *str, D3DXMATRIXA16 &Matrix,CCoord CpTran,bool bTrafficLightCheckDuringSaving)
{
	CCoord Cp;
	char temp[160];
	float fNomThickness,fMinThickness,fMaxThickness;
	int nType;
	bool bExtrapolated = false;

	Cp = m_pCp[nSample];

	float fX;
	fX = Cp.Side0.fX - m_Matrix.m[3][0];
	GetNominalThickness(fX,NULL,&fNomThickness,&fMinThickness,&fMaxThickness,&nType,&bExtrapolated,bTrafficLightCheckDuringSaving);
	Cp.Side0.fJ *= -1.0f;
	Cp.Side1.fJ *= -1.0f;
	Cp.Side[2].fJ *= -1.0f;
	Cp.TransformCoord(Matrix);

	bool bStyle = 1;
	if(bStyle == 0) {
		sprintf_s(str,100,"%.03f %.03f %.03f %.04f %.04f %.04f %.03f %.03f",
			Cp.Side[2].fX+CpTran.Side0.fX,Cp.Side[2].fY+CpTran.Side0.fY,Cp.Side[2].fZ+CpTran.Side0.fZ,
			Cp.Side[2].fI,Cp.Side[2].fJ,Cp.Side[2].fK,
			Cp.fActThickness,fNomThickness);

		if(nType & TRAFFIC_LIGHT) {
			strcat_s(str,100," Y ");
		} else {
			strcat_s(str, 100," N ");
		}
		sprintf_s(temp,"%.03f %.03f",fMinThickness,fMaxThickness);
		strcat_s(str, 100,temp);

		if(Cp.fActThickness > fMaxThickness) {
			strcat_s(str, 100," >");
		} else {
			if(Cp.fActThickness < fMinThickness) {
				strcat_s(str, 100," <");
			} else {
				strcat_s(str, 100," =");
			}
		}
	} else {	//csv
		sprintf_s(str,100,"%.03f,%.03f,%.03f,%.04f,%.04f,%.04f,%.03f,%.03f,",
			Cp.Side[2].fX+CpTran.Side0.fX,Cp.Side[2].fY+CpTran.Side0.fY,Cp.Side[2].fZ+CpTran.Side0.fZ,
			Cp.Side[2].fI,Cp.Side[2].fJ,Cp.Side[2].fK,
			Cp.fActThickness,fNomThickness);

		if(nType & TRAFFIC_LIGHT) {
			strcat_s(str, 100,"Y,");
		} else {
			strcat_s(str, 100,"N,");
		}
		sprintf_s(temp,"%.03f,%.03f,",fMinThickness,fMaxThickness);
		strcat_s(str, 100,temp);

		if(Cp.fActThickness > fMaxThickness) {
			strcat_s(str, 100,">");
		} else {
			if(Cp.fActThickness < fMinThickness) {
				strcat_s(str, 100,"<");
			} else {
				strcat_s(str, 100,"=");
			}
		}
		sprintf_s(temp,",%d",Cp.nAmp);
		strcat_s(str, 100, temp);
	}
	strcat_s(str, 100,"\r\n");

	return bExtrapolated;

}

int CPolyCoord::AddTrafficLight(float fXPos)
{
	int nKLo = 0;
	int nKHi = m_nCoordL - 1;
	int nK = (nKHi + nKLo) / 2;

	while((nKHi - nKLo) > 1) {
		if(fXPos <=  m_pCp[nK].Side0.fX) {
			nKHi = nK;
		} else {
			nKLo = nK;
		}
		nK = (nKHi + nKLo) / 2;
	}

	if(fabs(fXPos -  m_pCp[nKLo].Side0.fX) <= fabs(fXPos -  m_pCp[nKHi].Side0.fX)) {
		nK=nKLo;
	} else {
		nK=nKHi;
	}
	m_pCp[nK].nType |= TRAFFIC_LIGHT;

	return nK;
}

int CPolyCoord::FindNearestTrafficLight(float fXpos)
{
	float fMinDistance = 100000.0;
	int	nNearest=0;
	float fTemp;

	for(int ii=0;ii<m_nCoordL;ii++) {
		if(m_pCp[ii].nType & TRAFFIC_LIGHT) {
			fTemp = fabs(fXpos - m_pCp[ii].Side0.fX);
			if(fTemp < fMinDistance) {
				fMinDistance = fTemp;
				nNearest=ii;
			}
		}
	}

	return nNearest;
}

int CPolyCoord::MoveTrafficLight(int nSample, float fXPos)
{
	int nKLo = 0;
	int nKHi = m_nCoordL - 1;
	int nK = (nKHi + nKLo) / 2;

	if(m_pCp[nKHi].Side0.fX > m_pCp[nKLo].Side0.fX) {
		while((nKHi - nKLo) > 1) {
			if(fXPos <=  m_pCp[nK].Side0.fX) {
				nKHi = nK;
			} else {
				nKLo = nK;
			}
			nK = (nKHi + nKLo) / 2;
		}
	} else {
		while((nKHi - nKLo) > 1) {
			if(fXPos >=  m_pCp[nK].Side0.fX) {
				nKHi = nK;
			} else {
				nKLo = nK;
			}
			nK = (nKHi + nKLo) / 2;
		}
	}
	if(fabs(fXPos -  m_pCp[nKLo].Side0.fX) <= fabs(fXPos -  m_pCp[nKHi].Side0.fX)) {
		nK=nKLo;
	} else {
		nK=nKHi;
	}

	if(m_pCp[nK].nType & TRAFFIC_LIGHT) return nSample;
	m_pCp[nK].nType |= TRAFFIC_LIGHT;
	m_pCp[nSample].nType &= ~TRAFFIC_LIGHT;
	return nK;
}

bool CPolyCoord::DeleteTrafficLight(float fXPos)
{
	int nSample = FindNearestTrafficLight(fXPos);
	if(fabs(fXPos -  m_pCp[nSample].Side0.fX) > 5.0f) {
		MessageBox(NULL,_T("Ther is no traffic light within 5.0mm of the mouse.\r\nNo traffic light will be deleted"),_T("Error"),NULL);
		return FALSE;
	}
	m_pCp[nSample].nType &= ~TRAFFIC_LIGHT;
	return true;

}

void CPolyCoord::SetMinMaxThicknesses(float fMin, float fMax)
{

	for(int ii=0;ii<m_nCoordL;ii++) {
		m_pCp[ii].fMinThickness = m_pCp[ii].fNomThickness - fMin;
		m_pCp[ii].fMaxThickness = m_pCp[ii].fNomThickness + fMax;
	}

}

void CPolyCoord::SetMinMaxThicknesses(float fMin, float fMax,int nStartIndex,int nFinishIndex)
{

	for(int ii=nStartIndex;ii<m_nCoordL && ii<=nFinishIndex;ii++) {
		m_pCp[ii].fMinThickness = m_pCp[ii].fNomThickness - fMin;
		m_pCp[ii].fMaxThickness = m_pCp[ii].fNomThickness + fMax;
	}

}

void CPolyCoord::SetAllfPos(CCoord *pCp, int nWhichPos)
{
	for(int ii=0;ii<m_nCoordL;ii++) {
		m_pCp[ii].fPos[nWhichPos] = pCp->fPos[nWhichPos];
	}

}

//NOT WORKING
int CPolyCoord::FindNearestCoord(CCoord Cp)
{
	int nKLo = 0;
	int nKHi = m_nCoordL - 1;
	int nK;
	float fKHiLength,fKLoLength;
	D3DXVECTOR3 pt;

	if(m_pCp == NULL) {
		return 0;
	}

	pt = m_pCp[nKHi].Side0.pt - Cp.Side0.pt;
	fKHiLength = D3DXVec3Length(&pt);
	pt = m_pCp[nKLo].Side0.pt - Cp.Side0.pt;
	fKLoLength = D3DXVec3Length(&pt);

	while((nKHi - nKLo) > 1) {

		if(fKHiLength > fKLoLength ) {
			nKHi = nK;
			pt = m_pCp[nKHi].Side0.pt - Cp.Side0.pt;
			fKHiLength = D3DXVec3Length(&pt);
		} else {
			nKLo = nK;
			pt = m_pCp[nKLo].Side0.pt - Cp.Side0.pt;
			fKLoLength = D3DXVec3Length(&pt);
		}
		nK = (nKHi + nKLo) / 2;
	}
	if(fKLoLength < fKHiLength) {
		return nKLo;
	} else {
		return nKHi;
	}
}

int CPolyCoord::FindNearestCoord(float fXPos, int nGuess)
{
	int nKLo = 0;
	int nKHi = m_nCoordL - 1;
	int nK = nGuess;

	if(m_pCp == NULL) {
		return 0;
	}
	if(m_pCp[nKHi].Side0.fX > m_pCp[nKLo].Side0.fX) {
		while((nKHi - nKLo) > 1) {
			if(fXPos <=  m_pCp[nK].Side0.fX) {
				nKHi = nK;
			} else {
				nKLo = nK;
			}
			nK = (nKHi + nKLo) / 2;
		}
	} else {
		while((nKHi - nKLo) > 1) {
			if(fXPos >=  m_pCp[nK].Side0.fX) {
				nKHi = nK;
			} else {
				nKLo = nK;
			}
			nK = (nKHi + nKLo) / 2;
		}
	}
	if(fabs(fXPos -  m_pCp[nKLo].Side0.fX) <= fabs(fXPos -  m_pCp[nKHi].Side0.fX)) {
		nK=nKLo;
	} else {
		nK=nKHi;
	}
	return nK;

}


void CPolyCoord::SetAllTypes(int nFlag)
{

	for(int ii=0;ii<m_nCoordL;ii++) {
		m_pCp[ii].nType = nFlag;
	}

}

void CPolyCoord::AlignLeadingEdge()
{
	if(m_pCp == NULL) return;
	if(m_nCoordL<=0) return;
	float fDrop = 100.0f - theApp.m_LastSettings.fLePercentageDrop;
	float fNomXPoint=0.0;
	float fActXPoint=0.0;
	float fNomThickness = 0.0f;
	int nn,nPtr;
	int	nNomLE= m_nCoordL/2;

	//Find LE nominal value;
	if(theApp.m_Thread.m_nBladeSide == 0) {
		for(nn=0;nn<m_nCoordL;nn++) {
			if(m_pCp[nn].fNomThickness) {
				fNomThickness = m_pCp[nn].fNomThickness;
				nNomLE = nn;
				fNomXPoint = m_pCp[nn].Side0.fX;
				break;
			}
		}
	} else {
		for(nn=m_nCoordL - 1;nn>=0;nn--) {
			if(m_pCp[nn].fNomThickness) {
				fNomThickness = m_pCp[nn].fNomThickness;
				nNomLE = nn;
				fNomXPoint = m_pCp[nn].Side0.fX;
				break;
			}
		}
	};
	

	switch(theApp.m_LastSettings.nLEAlgorithm) {
	case 0:	//Nearest value which is 2 * LE Nominal thickness
		fNomThickness *= 2.0f;
		if(m_pCp[nNomLE].fActThickness < fNomThickness) {		//Look for rising edge
			for(nn=0;nn<(m_nCoordL/2);nn++) {
				nPtr = nNomLE + nn;
				MinMax(&nPtr,1,m_nCoordL-2);
				if((m_pCp[nPtr].fActThickness > fNomThickness)  && (m_pCp[nPtr+1].fActThickness > fNomThickness)) {
					fActXPoint = m_pCp[nPtr].Side0.fX;
					break;
				}
				nPtr = nNomLE - nn;
				MinMax(&nPtr,1,m_nCoordL-2);
				if((m_pCp[nPtr].fActThickness > fNomThickness)  && (m_pCp[nPtr-1].fActThickness > fNomThickness)) {
					fActXPoint = m_pCp[nPtr].Side0.fX;
					break;
				}
			}
		} else {	//Look for falling edge
			for(nn=0;nn<(m_nCoordL/2);nn++) {
				nPtr = nNomLE + nn;
				MinMax(&nPtr,1,m_nCoordL-2);
				if((m_pCp[nPtr].fActThickness < fNomThickness)  && (m_pCp[nPtr+1].fActThickness < fNomThickness)) {
					fActXPoint = m_pCp[nPtr].Side0.fX;
					break;
				}
				nPtr = nNomLE - nn;
				MinMax(&nPtr,1,m_nCoordL-2);
				if((m_pCp[nPtr].fActThickness < fNomThickness)  && (m_pCp[nPtr-1].fActThickness < fNomThickness)) {
					fActXPoint = m_pCp[nPtr].Side0.fX;
					break;
				}
			}
		};
		break;

	}

	
	
	
	
	m_Matrix.m[3][0] = fActXPoint - fNomXPoint;
	if(fabs(m_Matrix.m[3][0]) > 50.0f) m_Matrix.m[3][0] = 0.0f;


}

void CPolyCoord::setAllMinimumThickness(float *pfArray, int nNumberSamples, int nFirstIndex,int nDir)
{
	int nPtr,nDest;

	if(m_pCp == NULL) return;

	for(nDest=0;nDest<m_nCoordL;nDest++) {
		m_pCp[nDest].fMinThickness = 0.0f;
	}
	switch(nDir) {
	case 0:
		for(nPtr=0, nDest=nFirstIndex ;(nPtr<nNumberSamples) && (nDest<m_nCoordL);nPtr++,nDest++) {
			if((nDest>=0) && (nDest<m_nCoordL))
				m_pCp[nDest].fMinThickness = pfArray[nPtr];
		}
		break;
	case 1:
		for(nPtr=0, nDest=nFirstIndex ;(nPtr<nNumberSamples) && (nDest>=0);nPtr++,nDest--) {
			if((nDest>=0) && (nDest<m_nCoordL))
				m_pCp[nDest].fMinThickness = pfArray[nPtr];
		}
		break;
	}
}

void CPolyCoord::setAllNominalThickness(float *pfArray, int nNumberSamples, int nFirstIndex, int nDir)
{
	int nPtr,nDest;

	if(m_pCp == NULL) return;

	for(nDest=0;nDest<m_nCoordL;nDest++) {
		m_pCp[nDest].fNomThickness = 0.0f;
	}

	switch(nDir) {
	case 0:
		for(nPtr=0, nDest=nFirstIndex ;(nPtr<nNumberSamples) && (nDest<m_nCoordL);nPtr++,nDest++) {
			if((nDest>=0) && (nDest<m_nCoordL))
				m_pCp[nDest].fNomThickness = pfArray[nPtr];
		}
		break;
	case 1:
		for(nPtr=0, nDest=nFirstIndex ;(nPtr<nNumberSamples) && (nDest>=0);nPtr++,nDest--) {
			if((nDest>=0) && (nDest<m_nCoordL))
				m_pCp[nDest].fNomThickness = pfArray[nPtr];
		}
		break;
	}
}

void CPolyCoord::setAllMaximumThickness(float *pfArray, int nNumberSamples, int nFirstIndex, int nDir)
{
	int nPtr,nDest;

	if(m_pCp == NULL) return;

	for(nDest=0;nDest<m_nCoordL;nDest++) {
		m_pCp[nDest].fMaxThickness = 0.0f;
	}

	switch(nDir) {
	case 0:
		for(nPtr=0, nDest=nFirstIndex ;(nPtr<nNumberSamples) && (nDest<m_nCoordL);nPtr++,nDest++) {
			if((nDest>=0) && (nDest<m_nCoordL))
				m_pCp[nDest].fMaxThickness = pfArray[nPtr];
		}
		break;
	case 1:
		for(nPtr=0, nDest=nFirstIndex ;(nPtr<nNumberSamples) && (nDest>=0);nPtr++,nDest--) {
			if((nDest>=0) && (nDest<m_nCoordL))
				m_pCp[nDest].fMaxThickness = pfArray[nPtr];
		}
		break;
	}
}

void CPolyCoord::DecimateEitherSideOfZero(float fStepSize)
{
	float fXMin = m_pCp[0].Side0.fX;
	float fXMax = m_pCp[m_nCoordL-1].Side0.fX;
	int nn,nDest;
	float fX;
	CCoord Cp0,Cp1;

	CPolyCoord *pLine = new CPolyCoord;

	pLine->AllocateCoords( (int)((fXMax - fXMin) / fStepSize) * 2);

	fX = 0.0f;
	CalculateCoord(fX,Cp0);
	pLine->ModifyCoord(0,Cp0);
	Cp1 = Cp0;

	for( nDest=1;Cp1.Side0.fX < fXMax;nDest++) {
		CalculateCoord(Cp0, fStepSize,&Cp1);
		pLine->ModifyCoord(nDest,Cp1);
		Cp0 = Cp1;
	}


	fX = 0.0f;
	CalculateCoord(fX,Cp0);
	for( ; Cp1.Side0.fX > fXMin;nDest++) {
		CalculateCoord(Cp0,-fStepSize,&Cp1);
		pLine->ModifyCoord(nDest,Cp1);
		Cp0 = Cp1;
	}

	AllocateCoords(nDest);
	for(nn=0;nn<m_nCoordL; nn++) {
		m_pCp[nn] = pLine->m_pCp[nn];
	}

	delete pLine;

	Sort(0,0);
}



void CPolyCoord::setEqual(CPolyCoord &SrcLine, int nDir)
{
	int	ii,jj;

	Zero();

	m_nCoordL = SrcLine.m_nCoordL;

	if(m_nCoordL>0) {
		if(m_pCp==NULL) {
			m_pCp = new COORDDATA[m_nAllocatedSize = m_nCoordL];
		} else {
			ReallocateCoords(m_nCoordL);
		};
		if(nDir == 0) {
			CopyMemory(m_pCp,SrcLine.m_pCp,m_nCoordL * sizeof COORDDATA);
		} else {
			for(ii=0,jj=m_nCoordL-1;ii<m_nCoordL;ii++,jj--) {
				CopyMemory(&m_pCp[ii], &SrcLine.m_pCp[jj], sizeof COORDDATA);
			}
		}
	}

	if(SrcLine.m_nKnotsL>0) {
		if(m_pfKnots==NULL) {
			m_pfKnots = new float[m_nKnotsL=SrcLine.m_nKnotsL];
		} else {
			m_pfKnots = (float *)realloc(m_pfKnots,(m_nKnotsL = SrcLine.m_nKnotsL) * sizeof (float));
		}
		CopyMemory(m_pfKnots,SrcLine.m_pfKnots,SrcLine.m_nKnotsL * sizeof (float));
	}

	m_nPrimaryAxis = SrcLine.m_nPrimaryAxis;
	m_nStyle = SrcLine.m_nStyle;
	m_nCurveOrder = SrcLine.m_nCurveOrder;
	m_nLineIncrement = SrcLine.m_nLineIncrement;
	m_nMaxPtr = SrcLine.m_nMaxPtr;
	m_rgb = SrcLine.m_rgb;
	CopyMemory(m_wStr, SrcLine.m_wStr, sizeof m_wStr);
	CopyMemory(m_Matrix,SrcLine.m_Matrix,sizeof m_Matrix);

	m_bModified = TRUE;

}

void CPolyCoord::CircumcenterBetweenTwoPoints(float fRadius, int nStartIndex, int nExtraPoints)
{

}

void CPolyCoord::MoveToFirstCoordinate(int nIndex)
{

	COORDDATA Cp;

	Cp = m_pCp[nIndex];
	for(int ii=nIndex;ii>0;ii--) {
		m_pCp[ii] = m_pCp[ii-1];
	}
	m_pCp[0]=Cp;

}

void CPolyCoord::SortNearestAdjacent()
{
	int	nNearest;
	float fLength,fShortest;
	CCoord Cp;

	for(int ii=0;ii<(m_nCoordL-1);ii++) {
		fShortest = 100000.0f;
		nNearest = 0;
		for(int jj=ii+1;jj<m_nCoordL;jj++) {
			fLength = D3DXVec3Length(&(m_pCp[ii].Side0.pt - m_pCp[jj].Side0.pt));
			if(fLength < fShortest) {
				fShortest = fLength;
				nNearest = jj;
			}
		}
		Cp = m_pCp[ii+1];
		m_pCp[ii+1] = m_pCp[nNearest];
		m_pCp[nNearest] = Cp;
	};

}

void CPolyCoord::SetExtrapolation(int nEol)
{
	m_nEol[0] = m_nEol[1] = nEol;
}

//fScale is to allow for an overlap when calculating triangles so as to avoid bleed through of the back ground
float CPolyCoord::NearestPointDistance(XMVECTOR *vPoint, int nSide, XMVECTOR *vNearestPoint, XMVECTOR *vNearestNormal, float fScale)
{
	int nSegment;

	FindNearestSegment(vPoint, nSide, &nSegment);

	XMVECTOR V, W;
	int ii = nSegment;
	int jj = ii + 1;

	if (nSegment >= (m_nCoordL - 1)) {
		W = XMVectorSubtract(XMLoadFloat3((XMFLOAT3*)&m_pCp[ii].Side[nSide].pt), *vPoint);
		*vNearestPoint = XMLoadFloat3((XMFLOAT3*)&m_pCp[ii].Side[nSide].pt);
		if (vNearestNormal) *vNearestNormal = XMLoadFloat4((XMFLOAT4*)&m_pCp[ii].Side[nSide].norm);
		return XMVector3Length(W).m128_f32[0];
	}
	else {

		V = XMLoadFloat3((XMFLOAT3*)&m_pCp[jj].Side[nSide].pt) - XMLoadFloat3((XMFLOAT3*)&m_pCp[ii].Side[nSide].pt);
		W = XMVectorSubtract(*vPoint, XMLoadFloat3((XMFLOAT3*)&m_pCp[ii].Side[nSide].pt));

		float fC1 = XMVector3Dot(W, V).m128_f32[0];
		if (fC1 <= 0) {
			*vNearestPoint = XMVectorSubtract(*vPoint, W * fScale);
			if (vNearestNormal) *vNearestNormal = XMLoadFloat4((XMFLOAT4*)&m_pCp[ii].Side[nSide].norm);
			return XMVector3Length(W).m128_f32[0];
		}

		float fC2 = XMVector3Dot(V, V).m128_f32[0];
		if (fC2 <= fC1) {
			W = XMVectorSubtract(XMLoadFloat3((XMFLOAT3*)&m_pCp[jj].Side[nSide].pt), *vPoint);
			*vNearestPoint = XMVectorAdd(W * fScale, *vPoint);
			if (vNearestNormal) *vNearestNormal = XMLoadFloat4((XMFLOAT4*)&m_pCp[jj].Side[nSide].norm);
			return XMVector3Length(W).m128_f32[0];
		}

		float fB = fC1 / fC2;
		V = V * fB;
		XMVECTOR Pb = XMLoadFloat3((XMFLOAT3*)&m_pCp[ii].Side[nSide].pt) + V;
		W = XMVectorSubtract(Pb, *vPoint);
		*vNearestPoint = XMVectorAdd(W * fScale, *vPoint);
		if (vNearestNormal) {
			*vNearestNormal = XMVectorSubtract(XMLoadFloat4((XMFLOAT4*)&m_pCp[jj].Side[nSide].norm), XMLoadFloat4((XMFLOAT4*)&m_pCp[ii].Side[nSide].norm)) * fB;
			*vNearestNormal = XMVectorAdd(*vNearestNormal, XMLoadFloat4((XMFLOAT4*)&m_pCp[ii].Side[nSide].norm));
		}
		return XMVector3Length(W).m128_f32[0];
	}
}

float CPolyCoord::FindNearestSegment(XMVECTOR *vStart, int nSide, int *pnSegment)
{
	float fLengthHi, fLengthLo, fLength;
	int k, kLo, kHi;

	fLength = 1000000000.0f;
	*pnSegment = 0;
	for (k = 0; k<m_nCoordL; k++) {
		fLengthLo = DistanceToSegment(vStart, nSide, k);
		if (fLength > fLengthLo) {
			fLength = fLengthLo;
			*pnSegment = k;
		}
	}
	return fLength;


	fLength = 1000000000.0f;
	*pnSegment = 0;
	kHi = m_nCoordL - 1;
	kLo = 0;
	fLengthHi = DistanceToSegment(vStart, nSide, kHi);
	fLengthLo = DistanceToSegment(vStart, nSide, kLo);
	while ((kHi - kLo)>1) {
		k = (kHi + kLo) / 2;
		if (fLengthHi > fLengthLo) {
			if ((kHi - k) > 2) {
				kHi = (kHi + k) / 2;
			}
			else {
				kHi = k;
			}
			*pnSegment = kHi;
			fLengthHi = DistanceToSegment(vStart, nSide, kHi);
		}
		else {
			if ((k - kLo) > 2) {
				kLo = (k + kLo) / 2;
			}
			else {
				kLo = k;
			}
			*pnSegment = kLo;
			fLengthLo = DistanceToSegment(vStart, nSide, kLo);
		}
	}
	if (fLength > fLengthHi) fLength = fLengthHi;
	if (fLength > fLengthLo) fLength = fLengthLo;
	return fLength;

}

float CPolyCoord::DistanceToSegment(XMVECTOR *vStart, int nSide, int nSegment)
{
	XMVECTOR V, W;
	int ii = nSegment;
	int jj = ii + 1;

	if (nSegment >= (m_nCoordL - 1)) {
		W = XMVectorSubtract(*vStart, XMLoadFloat3((XMFLOAT3*)&m_pCp[ii].Side[nSide].pt));
		return XMVector3Length(W).m128_f32[0];

	}
	else {

		V = XMLoadFloat3((XMFLOAT3*)&m_pCp[jj].Side[nSide].pt) - XMLoadFloat3((XMFLOAT3*)&m_pCp[ii].Side[nSide].pt);
		W = XMVectorSubtract(*vStart, XMLoadFloat3((XMFLOAT3*)&m_pCp[ii].Side[nSide].pt));

		float fC1 = XMVector3Dot(W, V).m128_f32[0];
		if (fC1 <= 0) {
			return XMVector3Length(W).m128_f32[0];
		}

		float fC2 = XMVector3Dot(V, V).m128_f32[0];
		if (fC2 <= fC1) {
			W = XMVectorSubtract(*vStart, XMLoadFloat3((XMFLOAT3*)&m_pCp[jj].Side[nSide].pt));
			return XMVector3Length(W).m128_f32[0];
		}

		float fB = fC1 / fC2;
		V = V * fB;
		XMVECTOR Pb = XMLoadFloat3((XMFLOAT3*)&m_pCp[ii].Side[nSide].pt) + V;
		W = XMVectorSubtract(*vStart, Pb);
		return XMVector3Length(W).m128_f32[0];
	}

	return -1.0f;

}
