// TimeSlotDataPr20.h: interface for the CTimeSlotDataPr20 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMESLOTDATAPR20_H__F9A8A294_01B5_4BDA_956B_15C994551036__INCLUDED_)
#define AFX_TIMESLOTDATAPR20_H__F9A8A294_01B5_4BDA_956B_15C994551036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TimeSlotData.h"

class CTimeSlotDataPr20 : public CTimeSlotData  
{
private:
	CTimeSlotDataPr20();
public:
	CTimeSlotDataPr20(int index);
	virtual ~CTimeSlotDataPr20();
	virtual int GetGain();
	virtual void SetGain(int val);

};

#endif // !defined(AFX_TIMESLOTDATAPR20_H__F9A8A294_01B5_4BDA_956B_15C994551036__INCLUDED_)
