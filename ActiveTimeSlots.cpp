// ActiveTimeSlots.cpp: implementation of the CActiveTimeSlots class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "ActiveTimeSlots.h"
#include "TimeSlotDataPr20.h"
#include "TimeSlotDataPr30.h"
#include "TimeSlotDataPr50.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CActiveTimeSlots::CActiveTimeSlots()
{
	m_iter = m_vector.begin();
}

CActiveTimeSlots::~CActiveTimeSlots()
{
 m_CritSection.Lock();
 Clear();
 m_CritSection.Unlock();
}

void CActiveTimeSlots::addPr20(){
	m_CritSection.Lock();
	CTimeSlotDataPr20* data = new CTimeSlotDataPr20(GetSize());
	addTimeslotData(data);
	m_CritSection.Unlock();
}

void CActiveTimeSlots::addPr30(){
	m_CritSection.Lock();
	CTimeSlotDataPr30* data = new CTimeSlotDataPr30(GetSize());
	addTimeslotData(data);
	m_CritSection.Unlock();
}

void CActiveTimeSlots::addPr50(){
	m_CritSection.Lock();
	CTimeSlotDataPr50* data = new CTimeSlotDataPr50(GetSize());
	addTimeslotData(data);
	m_CritSection.Unlock();
}

void CActiveTimeSlots::addTimeslotData(CTimeSlotData* data){
	m_CritSection.Lock();
	m_vector.push_back(data);
	m_CritSection.Unlock();
}

int CActiveTimeSlots::GetSize(){
m_CritSection.Lock();
int size = m_vector.size();
m_CritSection.Unlock();
return size;
}

CTimeSlotData* CActiveTimeSlots::RemoveLast(){
	m_CritSection.Lock();
	CTimeSlotData* pData = NULL;
	if(!m_vector.empty()){
		pData=m_vector.back();
		m_vector.pop_back();
	}
	m_CritSection.Unlock();
	return pData;
}

CTimeSlotData* CActiveTimeSlots::Get(int i){
	m_CritSection.Lock();
	CTimeSlotData* pDat = NULL;
	if(i<(int)m_vector.size()){
		pDat=m_vector.at(i);
	}
	m_CritSection.Unlock();
	return pDat;
}


CTimeSlotData* CActiveTimeSlots::GetFirst(){
	m_CritSection.Lock();
	CTimeSlotData* pDat = NULL;
	m_iter = m_vector.begin();
	if(!m_vector.empty()){
		pDat = *m_iter;
	}
	m_CritSection.Unlock();
	return pDat;
}

CTimeSlotData* CActiveTimeSlots::GetNext(){
	m_CritSection.Lock();
	CTimeSlotData* pDat = NULL;
	if(!m_vector.empty()){
		m_iter++;
		if(m_iter != m_vector.end()){
			pDat = *m_iter;
		} 
	}
	m_CritSection.Unlock();
	return pDat;
}

void CActiveTimeSlots::Clear(){
	m_CritSection.Lock();
	CTimeSlotData* pDat;
	do{
		pDat = RemoveLast();
		if(pDat) delete pDat;
	} while(pDat);
	m_CritSection.Unlock();
}