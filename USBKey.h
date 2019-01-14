// USBKey.h: interface for the CUSBKey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USBKEY_H__40B50D79_9C63_4479_BEBF_CC591F89294C__INCLUDED_)
#define AFX_USBKEY_H__40B50D79_9C63_4479_BEBF_CC591F89294C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUSBKey  
{
public:
	CUSBKey();
	virtual ~CUSBKey();

	bool CalculateCode(int nVersion, DWORD &Code);
	bool IsValid(int nVersion);
	
	int GetNumDevices();
	bool GetDeviceSerialNumber(CString &SerialNumber);
	bool GetDeviceDescription(CString &Description);
	bool GetDeviceChipID(DWORD &ChipID);


	bool ReadEEPROM(DWORD &nReadVal);


	bool bDevicePresent;


};

#endif // !defined(AFX_USBKEY_H__40B50D79_9C63_4479_BEBF_CC591F89294C__INCLUDED_)
