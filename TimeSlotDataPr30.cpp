// TimeSlotDataPr30.cpp: implementation of the CTimeSlotDataPr30 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "TimeSlotDataPr30.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeSlotDataPr30::CTimeSlotDataPr30(int index):CTimeSlotData(index)
{

}


CTimeSlotDataPr30::CTimeSlotDataPr30()
{

}

CTimeSlotDataPr30::~CTimeSlotDataPr30()
{

}

int CTimeSlotDataPr30::GetGain(){
	return m_pTimeSlotData[GetIndex()].Pr30.nPreampGain;
}

void CTimeSlotDataPr30::SetGain(int val){
	m_CritSection.Lock();
	
	theApp.m_UtUser.SelectWriteTimeslot();
	m_pTimeSlotData[GetIndex()].Pr30.nPreampGain = val;
	theApp.m_UtUser.Pr30PreampGain(&m_pTimeSlotData[GetIndex()].Pr30);
	m_CritSection.Unlock();
}