// HRTimer.h: interface for the CHRTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HRTIMER_H__672F9F49_CD41_42F7_9C4D_D64E9E0CDB90__INCLUDED_)
#define AFX_HRTIMER_H__672F9F49_CD41_42F7_9C4D_D64E9E0CDB90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHRTimer  
{
public:
	double GetTime();
	double dPeriod;
	double dFrequency;
	LARGE_INTEGER stop;
	LARGE_INTEGER start;
	double StopTimer();
	void StartTimer();
	double GetFrequency();
	CHRTimer();
	virtual ~CHRTimer();

};

#endif // !defined(AFX_HRTIMER_H__672F9F49_CD41_42F7_9C4D_D64E9E0CDB90__INCLUDED_)
