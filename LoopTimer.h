// LoopTimer.h: interface for the CLoopTimer class.
//
// A class for generting time stats in a loop.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOOPTIMER_H__4B1F4FCF_ECC0_45D5_86C9_D95AFD8B879D__INCLUDED_)
#define AFX_LOOPTIMER_H__4B1F4FCF_ECC0_45D5_86C9_D95AFD8B879D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoopTimer  
{
public:
	CLoopTimer();
	virtual ~CLoopTimer();

	void reset();
	//For measuring time in a loop.
	void time();
	//For measuring time bewteen time1 and time2.
	void time1();
	float getAve(){return m_ave;}
	float getAveErr();
	float getMax(){return m_max;}
	float getMin(){return m_min;}

private:
	long m_t1, m_t2;
	float m_n;
	float m_ave;
	float m_var;
	float m_max;
	float m_min;
	float m_sum;
	float m_sum2;
	float m_delta;

	CCriticalSection m_CritSection;
};

#endif // !defined(AFX_LOOPTIMER_H__4B1F4FCF_ECC0_45D5_86C9_D95AFD8B879D__INCLUDED_)
