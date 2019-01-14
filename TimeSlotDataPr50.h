// TimeSlotDataPr50.h: interface for the CTimeSlotDataPr50 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMESLOTDATAPR50_H__9BDADB81_E577_4BC7_96CA_1591FE8C10F9__INCLUDED_)
#define AFX_TIMESLOTDATAPR50_H__9BDADB81_E577_4BC7_96CA_1591FE8C10F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TimeSlotData.h"

class CTimeSlotDataPr50 : public CTimeSlotData  
{
private:
	CTimeSlotDataPr50();
public:
	CTimeSlotDataPr50(int index);
	virtual ~CTimeSlotDataPr50();
	virtual int GetGain();
	virtual void SetGain(int val);
};

#endif // !defined(AFX_TIMESLOTDATAPR50_H__9BDADB81_E577_4BC7_96CA_1591FE8C10F9__INCLUDED_)
