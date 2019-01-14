// USBtoI2C.h: interface for the CUSBtoI2C class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USBTOI2C_H__4F8448A9_5C8D_4E15_8D9A_3E1F7BB1EC55__INCLUDED_)
#define AFX_USBTOI2C_H__4F8448A9_5C8D_4E15_8D9A_3E1F7BB1EC55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUSBtoI2C  
{
public:
	CUSBtoI2C();
	virtual ~CUSBtoI2C();

	int FindDevice();
	void StartDriver();
	int MailboxTx(BYTE nValue);
	int ReadTS0();
	void Reset();

	bool m_bDevicePresent;


private:
	void SetupCommPort( LPTSTR ComPort);

	bool m_bMailboxFailed;
	BYTE Msg[64];
	int nMsgL;
	HANDLE m_hCom;
	HANDLE m_hLPT;

};

#endif // !defined(AFX_USBTOI2C_H__4F8448A9_5C8D_4E15_8D9A_3E1F7BB1EC55__INCLUDED_)
