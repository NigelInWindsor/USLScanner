// LoopTimer.cpp: implementation of the CLoopTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "LoopTimer.h"
#include "Mmsystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoopTimer::CLoopTimer()
{
	reset();
}

CLoopTimer::~CLoopTimer()
{

}

void CLoopTimer::reset(){
	CSingleLock lock = CSingleLock(&m_CritSection);
	lock.Lock();
	m_t1 = timeGetTime();
	m_n = 1;
	m_delta = -1;
	m_ave = 0;
	m_var = 0;
	m_max = 0;
	m_min = 1e10;
	m_sum = 0;
	m_sum2 = 0;
	lock.Unlock();
}


void CLoopTimer::time(){
		CSingleLock lock = CSingleLock(&m_CritSection);
		lock.Lock();
		m_t2 = timeGetTime();
		m_delta = (float)(m_t2-m_t1);
		m_sum+=m_delta;
		m_sum2+=m_delta*m_delta;
		m_var=(m_sum2-(m_sum*m_sum)/m_n)/(m_n-1);
		m_ave += (m_delta-m_ave)/m_n;
		if(m_delta>m_max) m_max=m_delta;
		if(m_delta<m_max) m_min=m_delta;
		m_n++;
		m_t1 = m_t2;
		lock.Unlock();
}

void CLoopTimer::time1(){
	CSingleLock lock = CSingleLock(&m_CritSection);
	lock.Lock();
	m_t1 = timeGetTime();
	lock.Unlock();
}


float CLoopTimer::getAveErr(){
	CSingleLock lock = CSingleLock(&m_CritSection);
	lock.Lock();
	if(m_var <=0) {
		return 0;
	} else {
		return sqrt(m_var);
	}
	lock.Unlock();
}