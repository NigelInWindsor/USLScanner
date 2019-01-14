// TimeSlotDataPr20.cpp: implementation of the CTimeSlotDataPr20 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "TimeSlotDataPr20.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeSlotDataPr20::CTimeSlotDataPr20(int index):CTimeSlotData(index)
{

}


CTimeSlotDataPr20::CTimeSlotDataPr20()
{

}

CTimeSlotDataPr20::~CTimeSlotDataPr20()
{

}

int CTimeSlotDataPr20::GetGain(){
	return 0; //Struct PR2050Data does not have a gain variable. See sldriver\sys\uslioctl.h
}

void CTimeSlotDataPr20::SetGain(int val){
	//do nothing
}
