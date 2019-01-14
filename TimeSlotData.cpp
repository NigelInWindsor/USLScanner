// TimeSlot.cpp: implementation of the CTimeSlot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "TimeSlotData.h"
#include "USLScanner.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeSlotData::CTimeSlotData(int index)
{	
	m_index = index;
	m_pTimeSlotData = ((CUSLScannerApp*)AfxGetApp())->m_UtUser.m_TS;
}

CTimeSlotData::CTimeSlotData()
{

}

CTimeSlotData::~CTimeSlotData()
{

}

int CTimeSlotData::GetIndex(){
	m_CritSection.Lock();
	int index = m_index;
	m_CritSection.Unlock();
	return m_index;
}
