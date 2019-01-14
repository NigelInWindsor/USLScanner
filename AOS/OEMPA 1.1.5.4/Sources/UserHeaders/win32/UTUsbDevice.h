
#pragma once

#if !defined(_COM_DECL_)
#ifdef _UTSOCK_EXPORT_
	#define _COM_DECL_ __declspec(dllexport)
#else _UTSOCK_EXPORT_
	#define _COM_DECL_ __declspec(dllimport)
#endif _UTSOCK_EXPORT_
#endif _COM_DECL_

#define _USB3_
#if defined(_USB3_)
#define _USB3_DECL_
#endif


class _COM_DECL_ CUTUsb3Device
{
public:
	int GetStreamSizeEx() const;
	DWORD GetVersion() const;
	bool IsConnected() const;
	void Reset();
	void EnableEmulator(double dTimeSlot);
	bool GetSerialNumber(int iSize,char *cSerial);
	bool GetSerialNumber(int iSize,wchar_t *wcSerial);
	bool GetFlashSerialNumber(int iSize,char *cSerial);
	bool GetFlashSerialNumber(int iSize,wchar_t *wcSerial);

};

