// IntervalTimer.cpp: implementation of the CIntervalTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "IntervalTimer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIntervalTimer::CIntervalTimer()
{
	m_CStats = new CStats();
	Reset();
}

CIntervalTimer::~CIntervalTimer()
{
	delete m_CStats;
}

void CIntervalTimer::Reset(){
	m_t0=-1;
	m_T=-1;
	m_nMeasCount=0;
	m_CStats->Reset();
}

void CIntervalTimer::Measure(){
	DWORD timenow = GetTickCount();
	if(m_t0>0){
		m_T = timenow - m_t0;
		m_t0 = timenow;
		if(m_nMeasCount>1){
			DoStats();
		}else {
			m_nMeasCount++;
		}
	} else {
		m_T=0;
		m_t0 = timenow;
		m_nMeasCount++;
	}
}

void CIntervalTimer::DoStats(){
	m_CStats->Add((float)m_T);
}

CStats* CIntervalTimer::Stats(){return m_CStats;}