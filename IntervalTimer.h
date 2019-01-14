// IntervalTimer.h: interface for the CIntervalTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERVALTIMER_H__25F06B1B_2309_4567_A0F9_4BA8498E877A__INCLUDED_)
#define AFX_INTERVALTIMER_H__25F06B1B_2309_4567_A0F9_4BA8498E877A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cmath>
#include "Stats.h"

class CIntervalTimer  
{
public:
	CIntervalTimer();
	virtual ~CIntervalTimer();
	void Reset();
	void Measure();
	CStats* Stats();
	

private:
	DWORD m_t0;
	DWORD m_T;
	int m_nMeasCount;
	void DoStats();
	void DoMinMax();
	CStats* m_CStats;
};

#endif // !defined(AFX_INTERVALTIMER_H__25F06B1B_2309_4567_A0F9_4BA8498E877A__INCLUDED_)
