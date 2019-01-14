// USBKey.cpp: implementation of the CUSBKey class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "USBKey.h"
#include "ftd2xx.h"
#include "FTChipID.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUSBKey::CUSBKey()
{
	bDevicePresent=false;
}

CUSBKey::~CUSBKey()
{
	bDevicePresent=false;

}

#define VERSION_1 0xaaaaaaaa;

bool CUSBKey::CalculateCode(int nVersion, DWORD &Code)
{
	DWORD ChipID;

	if(!GetDeviceChipID(ChipID))
		return false;


	switch(nVersion) {
	case 0:
		Code = ChipID^VERSION_1;
		break;
	}

	return true;

}

bool CUSBKey::IsValid(int nVersion)
{
	DWORD nEEPROM,nCode;

	if(!ReadEEPROM(nEEPROM))
		return false;

	if(!CalculateCode(nVersion,nCode))
		return false;

	if(nCode!=nEEPROM)
		return false;

	return true;

}

int CUSBKey::GetNumDevices()
{

	FTID_STATUS Status = FTID_SUCCESS;
	DWORD NumDevices = 0;
	Status = FTID_GetNumDevices(&NumDevices);

	if(NumDevices>0)
		bDevicePresent=true;

	return NumDevices;

}

bool CUSBKey::GetDeviceSerialNumber(CString &SerialNumber)
{
	

	if(!bDevicePresent)
		return false;

	FTID_STATUS Status = FTID_SUCCESS;
	char ch[256];
	Status = FTID_GetDeviceSerialNumber(0, ch, 256);

	if(Status==FTID_SUCCESS) {
		theApp.CharToCString(ch, SerialNumber);
		return true;
	}else{
		return false;
	}

	return false; //should never get here
}


bool CUSBKey::GetDeviceDescription(CString &Description)
{
	

	if(!bDevicePresent)
		return false;


	FTID_STATUS Status = FTID_SUCCESS;
	char ch[256];
	Status = FTID_GetDeviceDescription(0, ch, 256);

	if(Status==FTID_SUCCESS) {
		theApp.CharToCString(ch, Description);
		return true;
	}else{
		return false;
	}

	return false; //should never get here
}


bool CUSBKey::GetDeviceChipID(DWORD &ChipID)
{
	

	if(!bDevicePresent)
		return false;


	FTID_STATUS Status = FTID_SUCCESS;
	Status = FTID_GetDeviceChipID(0, &ChipID);

	if(Status==FTID_SUCCESS) {
		return true;
	}else{
		return false;
	}

	return false; //should never get here
}

bool CUSBKey::ReadEEPROM(DWORD &nReadVal)
{
	


	FT_HANDLE ftHandle; 
	FT_STATUS ftStatus = FT_Open(0, &ftHandle); 
	bool bFlag=true;

	if (ftStatus != FT_OK) { 
		return false;
	} 

	unsigned char buffer[8]; 
	DWORD BytesRead; 

	ftStatus = FT_EE_UARead(ftHandle, buffer, 8, &BytesRead); 

	if (ftStatus == FT_OK) { 
		//nReadVal = atoi((char*)buffer);
		nReadVal = strtoul((char*)buffer,NULL,16);
	} else { 
		bFlag = false;
	} 

	FT_Close(ftHandle);

	return bFlag;

}