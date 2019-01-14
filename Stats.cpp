// Stats.cpp: implementation of the CStats class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "Stats.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStats::CStats()
{
	Reset();
}

CStats::~CStats()
{

}

void CStats::Reset(){
	m_fMean=0;
	m_fVal=0;
	m_fVariance=0;
	m_fMin=-1;
	m_fMax=-1;
	m_fM2=0;
	m_fCount = 0;
}

void CStats::Add(float val){
	m_fLast = m_fVal;
	m_fVal = val;
	if (m_fMin <0) {
		m_fMin = val;
		m_fMax = val;
	}
	else {
		if (val<m_fMin) m_fMin = val;
		if (val>m_fMax) m_fMax = val;
	}
	m_fCount ++;
	float prevMean = m_fMean;
	float diff = val-prevMean;

	m_fMean += diff / m_fCount;
	m_fM2 += (val-m_fMean)*diff;
	m_fVariance = m_fM2 / m_fCount;
}

float CStats::Mean(){return m_fMean;}