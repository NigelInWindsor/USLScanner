// TimeSlotDataPr50.cpp: implementation of the CTimeSlotDataPr50 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "TimeSlotDataPr50.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeSlotDataPr50::CTimeSlotDataPr50(int index):CTimeSlotData(index)
{

}


CTimeSlotDataPr50::CTimeSlotDataPr50()
{

}

CTimeSlotDataPr50::~CTimeSlotDataPr50()
{

}

int CTimeSlotDataPr50::GetGain(){
	return 0; //Pr50 does not have this data in its struct: see PR2050Data in usldriver\sys\uslioctl.h
}

void CTimeSlotDataPr50::SetGain(int val){
	// do nothing
}