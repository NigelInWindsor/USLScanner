// LeadScrew.cpp: implementation of the CLeadScrew class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "LeadScrew.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLeadScrew::CLeadScrew()
{
	m_pPos = NULL;
	m_nCoordL = 0;
	m_fStepSize = 0.01f;
	m_bOffOn = false;
}

CLeadScrew::~CLeadScrew()
{
	SAFE_DELETE_ARRAY( m_pPos );

}

void CLeadScrew::Zero()
{
	SAFE_DELETE_ARRAY( m_pPos );
	m_nCoordL = 0;
	m_fStepSize = 0.01f;
	m_bOffOn = false;
}

bool CLeadScrew::Save(CUSLFile *pFile)
{
	pFile->Write(&m_fStepSize, sizeof m_fStepSize);
	pFile->Write(&m_bOffOn, sizeof m_bOffOn);
	pFile->Write(&m_nCoordL, sizeof m_nCoordL);
	for(int nn=0;nn<m_nCoordL;nn++) {
		pFile->Write(&m_pPos[nn], sizeof Position);
	}
	return true;
}

bool CLeadScrew::Retrieve(CUSLFile *pFile)
{
	Zero();

	pFile->Read(&m_fStepSize, sizeof m_fStepSize);
	pFile->Read(&m_bOffOn, sizeof m_bOffOn);
	pFile->Read(&m_nCoordL, sizeof m_nCoordL);
	if(m_nCoordL>0) {
		m_pPos = new Position[m_nCoordL];
		for(int nn=0;nn<m_nCoordL;nn++) {
			pFile->Read(&m_pPos[nn], sizeof Position);
		}
	}
	return true;


	return false;
}

int CLeadScrew::GetSize()
{
	return m_nCoordL;
}

int CLeadScrew::MachineToActualPos(int nMachine)
{
	if(m_bOffOn != true) return nMachine;
	if(m_nCoordL<=1) return nMachine;

	int k;
	int kLo = 0;
	int kHi = m_nCoordL-1;

	while((kHi-kLo)>1) {
		k=(kHi+kLo)/2;
		(m_pPos[k].nMachine > nMachine)?(kHi=k):(kLo=k);
	};

	int nActualPos = MulDiv(nMachine-m_pPos[kLo].nMachine,m_pPos[kHi].nActualPos-m_pPos[kLo].nActualPos,m_pPos[kHi].nMachine-m_pPos[kLo].nMachine) + m_pPos[kLo].nActualPos;

	return nActualPos;
}

int CLeadScrew::MachineToActualNeg(int nMachine)
{
	if(m_bOffOn != true) return nMachine;
	if(m_nCoordL<=1) return nMachine;

	int k;
	int kLo = 0;
	int kHi = m_nCoordL-1;

	while((kHi-kLo)>1) {
		k=(kHi+kLo)/2;
		(m_pPos[k].nMachine > nMachine)?(kHi=k):(kLo=k);
	};

	int nActualNeg = MulDiv(nMachine-m_pPos[kLo].nMachine,m_pPos[kHi].nActualNeg-m_pPos[kLo].nActualNeg,m_pPos[kHi].nMachine-m_pPos[kLo].nMachine) + m_pPos[kLo].nActualNeg;

	return nActualNeg;
}

int	CLeadScrew::ActualToMachine(int nDesirePos, int nCurrentPos)
{
	if(nDesirePos <= nCurrentPos) {
		return ActualPositiveToMachine(nDesirePos);
	} else {
		return ActualNegativeToMachine(nDesirePos);
	}
}

int CLeadScrew::ActualPositiveToMachine(int nActualPos)
{
	if(m_bOffOn != true) return nActualPos;
	if(m_nCoordL<=1) return nActualPos;

	int k;
	int kLo = 0;
	int kHi = m_nCoordL-1;

	while((kHi-kLo)>1) {
		k=(kHi+kLo)/2;
		(m_pPos[k].nActualPos > nActualPos)?(kHi=k):(kLo=k);
	};

	int nMachine = MulDiv(nActualPos-m_pPos[kLo].nActualPos,m_pPos[kHi].nMachine-m_pPos[kLo].nMachine,m_pPos[kHi].nActualPos-m_pPos[kLo].nActualPos) + m_pPos[kLo].nMachine;

	return nMachine;
}

int CLeadScrew::ActualNegativeToMachine(int nActualNeg)
{
	if(m_bOffOn != true) return nActualNeg;
	if(m_nCoordL<=1) return nActualNeg;

	int k;
	int kLo = 0;
	int kHi = m_nCoordL-1;

	while((kHi-kLo)>1) {
		k=(kHi+kLo)/2;
		(m_pPos[k].nActualNeg > nActualNeg)?(kHi=k):(kLo=k);
	};

	int nMachine = MulDiv(nActualNeg-m_pPos[kLo].nActualNeg,m_pPos[kHi].nMachine-m_pPos[kLo].nMachine,m_pPos[kHi].nActualNeg-m_pPos[kLo].nActualNeg) + m_pPos[kLo].nMachine;

	return nMachine;
}

void CLeadScrew::Sort()
{
	qsort(m_pPos,m_nCoordL,sizeof Position,ComparePositionData);
}

int ComparePositionData(const void *Cp0,const void *Cp1)
{
	Position *Cl0 = (Position *)Cp0;
	Position *Cl1 = (Position *)Cp1;

	if(Cl0->nMachine == Cl1->nMachine) return 0;
	if(Cl0->nMachine < Cl1->nMachine) return -1;
	if(Cl0->nMachine > Cl1->nMachine) return 1;

	return 0;
}

void CLeadScrew::Add(int nMachine, int nActualPos, int nActualNeg)
{

	if(m_nCoordL<=0) {
		m_pPos = new Position[m_nCoordL=1];
	} else {
		m_nCoordL++;
		m_pPos = (Position*)realloc(m_pPos,m_nCoordL * sizeof Position);
	}
	int nn=m_nCoordL-1;

	m_pPos[nn].nMachine = nMachine;
	m_pPos[nn].nActualPos = nActualPos;
	m_pPos[nn].nActualNeg = nActualNeg;
	m_pPos[nn].fMachine = (float)nMachine * m_fStepSize;
	m_pPos[nn].fActualPos = (float)nActualPos * m_fStepSize;
	m_pPos[nn].fActualNeg = (float)nActualNeg * m_fStepSize;

}

void CLeadScrew::Add(Position Pos)
{
	if(m_nCoordL<=0) {
		m_pPos = new Position[m_nCoordL=1];
	} else {
		m_nCoordL++;
		m_pPos = (Position*)realloc(m_pPos,m_nCoordL * sizeof Position);
	}
	int nn=m_nCoordL-1;

	CopyMemory(&m_pPos[nn], &Pos, sizeof Pos);
}

void CLeadScrew::Delete(int nIndex)
{
	int	nn;

	if((m_nCoordL>0) && (nIndex<m_nCoordL) && (nIndex>=0)) {
		m_nCoordL--;
		for(nn=nIndex; nn<m_nCoordL ;nn++) {
			m_pPos[nn]=m_pPos[nn+1];
		};
		if(m_nCoordL>0) {
			m_pPos = (Position *)realloc(m_pPos,m_nCoordL * sizeof Position);
		} else {
			SAFE_DELETE_ARRAY( m_pPos );
		}
	}

}

void CLeadScrew::GetPosition(int nIndex, Position *pPos)
{
	if(m_pPos==NULL) return;
	if(m_nCoordL<=0) return;
	CopyMemory(pPos,&m_pPos[nIndex],sizeof Position);

}

void CLeadScrew::ModifyPosition(int nIndex, Position *pPos)
{
	if(m_pPos==NULL) return;
	if(m_nCoordL<=0) return;
	CopyMemory(&m_pPos[nIndex],pPos,sizeof Position);

}

Position* CLeadScrew::GetPtrPosition(int nIndex)
{
	return &m_pPos[nIndex];

}



void CLeadScrew::InvalidateIntegers()
{

	for(int nn=0;nn<m_nCoordL;nn++) {
		m_pPos[nn].nMachine = (int)(m_pPos[nn].fMachine / m_fStepSize);
		m_pPos[nn].nActualPos = (int)(m_pPos[nn].fActualPos / m_fStepSize);
		m_pPos[nn].nActualNeg = (int)(m_pPos[nn].fActualNeg / m_fStepSize);
	}

}

void CLeadScrew::InvalidateFloats()
{
	for(int nn=0;nn<m_nCoordL;nn++) {
		m_pPos[nn].fMachine = (float)m_pPos[nn].nMachine * m_fStepSize;
		m_pPos[nn].fActualPos = (float)m_pPos[nn].nActualPos * m_fStepSize;
		m_pPos[nn].fActualNeg = (float)m_pPos[nn].nActualNeg * m_fStepSize;
	}

}

void CLeadScrew::CopyPositiveToNegative()
{
	for(int nn=0;nn<m_nCoordL;nn++) {
		m_pPos[nn].fActualNeg = m_pPos[nn].fActualPos;
		m_pPos[nn].nActualNeg = m_pPos[nn].nActualPos;
	}
}

void CLeadScrew::CopyNegativeToPositive()
{
	for(int nn=0;nn<m_nCoordL;nn++) {
		m_pPos[nn].fActualPos = m_pPos[nn].fActualNeg;
		m_pPos[nn].nActualPos = m_pPos[nn].nActualNeg;
	}

}

void CLeadScrew::CopyMasterToNegativePositive()
{
	for(int nn=0;nn<m_nCoordL;nn++) {
		m_pPos[nn].fActualPos = m_pPos[nn].fMachine;
		m_pPos[nn].fActualNeg = m_pPos[nn].fMachine;
		m_pPos[nn].nActualPos = m_pPos[nn].nMachine;
		m_pPos[nn].nActualNeg = m_pPos[nn].nMachine;
	}
}

void CLeadScrew::FillTable(float fMin, float fMax, float fStepsize)
{
	float fPos;
	int ii;
	float *fArray = NULL;

	SAFE_DELETE_ARRAY( m_pPos );

	int nSize = (int)((fMax - fMin) / fStepsize) + 4;
	fArray = new float[nSize];		
	
	for(fPos=0.0f, ii=0;fPos<=fMax;fPos+=fStepsize) {
		fArray[ii++] = fPos;
	}
	if(fabsf(fArray[ii-1] - fMax) > 10.0f) {
		fArray[ii++] = fMax;
	}

	for(fPos=-fStepsize;fPos>=fMin;fPos-=fStepsize) {
		fArray[ii++] = fPos;
	}
	if(fabsf(fArray[ii-1] - fMin) > 10.0f && fMin != 0.0f) {
		fArray[ii++] = fMin;
	}

	m_pPos = new Position[m_nCoordL = ii];
	for(ii=0;ii<m_nCoordL;ii++) {
		m_pPos[ii].fActualPos = m_pPos[ii].fActualNeg = m_pPos[ii].fMachine = fArray[ii];

		m_pPos[ii].nMachine = (int)(m_pPos[ii].fMachine / m_fStepSize);
		m_pPos[ii].nActualPos = (int)(m_pPos[ii].fActualPos / m_fStepSize);
		m_pPos[ii].nActualNeg = (int)(m_pPos[ii].fActualNeg / m_fStepSize);
	}
	Sort();

	delete fArray;
}