// TimeSlotData.h
// A wrapper for the time slot data.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMESLOTDATA_H__0AC46C5B_258D_4BF2_BEA2_40625B1ED9C8__INCLUDED_)
#define AFX_TIMESLOTDATA_H__0AC46C5B_258D_4BF2_BEA2_40625B1ED9C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTimeSlotData  
{
private:
	int m_index;

protected:
	CCriticalSection m_CritSection;
	TimeSlotData* m_pTimeSlotData;
	CTimeSlotData();
public:
	CTimeSlotData(int index);
	virtual ~CTimeSlotData();
	virtual int GetGain() = 0;
	virtual void SetGain(int val) = 0;
	int GetIndex();
};

#endif // !defined(AFX_TIMESLOTDATA_H__0AC46C5B_258D_4BF2_BEA2_40625B1ED9C8__INCLUDED_)
