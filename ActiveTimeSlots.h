// ActiveTimeSlots.h: interface for the CActiveTimeSlots class.
//
// A container to the time slot data wrappers for the currently active time slots.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTIVETIMESLOTS_H__5558581A_BE1C_47D7_A9C4_C3C1EEAA989F__INCLUDED_)
#define AFX_ACTIVETIMESLOTS_H__5558581A_BE1C_47D7_A9C4_C3C1EEAA989F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "TimeSlotData.h"

class CActiveTimeSlots  
{
public:
	CActiveTimeSlots();
	virtual ~CActiveTimeSlots();
	
	//Add one of the time slot types in time slot index order.
	void addPr30();
	void addPr50();
	void addPr20();
	int GetSize();
	CTimeSlotData* RemoveLast(); //Remove and return the last element.
	void Clear(); //Remove all the wrappers 
	CTimeSlotData* Get(int i);
	CTimeSlotData* GetFirst();
	CTimeSlotData* GetNext();

private:
	std::vector<CTimeSlotData*> m_vector;
	CCriticalSection m_CritSection;
	void addTimeslotData(CTimeSlotData* data);
	std::vector<CTimeSlotData*>::iterator m_iter;
};

#endif // !defined(AFX_ACTIVETIMESLOTS_H__5558581A_BE1C_47D7_A9C4_C3C1EEAA989F__INCLUDED_)
