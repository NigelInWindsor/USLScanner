// HRTimer.cpp: implementation of the CHRTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "HRTimer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHRTimer::CHRTimer()
{

	dFrequency = 
	dPeriod = 1.0 / GetFrequency();
}

CHRTimer::~CHRTimer()
{

}

double CHRTimer::GetFrequency()
{
	LARGE_INTEGER proc_freq;
	if(!::QueryPerformanceFrequency(&proc_freq)) {
	}

	return (double)proc_freq.QuadPart;

}

void CHRTimer::StartTimer()
{
	int oldmask = ::SetThreadAffinityMask(::GetCurrentThread(),0);
	::QueryPerformanceCounter(&start);
	::SetThreadAffinityMask(::GetCurrentThread(),oldmask);
}

double CHRTimer::StopTimer()
{
	int oldmask = ::SetThreadAffinityMask(::GetCurrentThread(),0);
	::QueryPerformanceCounter(&stop);
	::SetThreadAffinityMask(::GetCurrentThread(),oldmask);

	return ((stop.QuadPart - start.QuadPart) * dPeriod);

}

double CHRTimer::GetTime()
{
	return StopTimer();
}
