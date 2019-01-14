// TimeSlotDataPr30.h: interface for the CTimeSlotDataPr30 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMESLOTDATAPR30_H__BE9D56B4_AA9F_42FC_A91F_9808F67B0950__INCLUDED_)
#define AFX_TIMESLOTDATAPR30_H__BE9D56B4_AA9F_42FC_A91F_9808F67B0950__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TimeSlotData.h"

class CTimeSlotDataPr30 : public CTimeSlotData  
{
private:
	CTimeSlotDataPr30();
public:
	CTimeSlotDataPr30(int index);
	virtual ~CTimeSlotDataPr30();
	virtual int GetGain();
	virtual void SetGain(int val);
};

#endif // !defined(AFX_TIMESLOTDATAPR30_H__BE9D56B4_AA9F_42FC_A91F_9808F67B0950__INCLUDED_)
