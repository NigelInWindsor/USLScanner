// SerialFLSC10.h: interface for the CSerialFLSC10 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALFLSC10_H__646E5D6C_4277_4E7D_9732_4E646745E79A__INCLUDED_)
#define AFX_SERIALFLSC10_H__646E5D6C_4277_4E7D_9732_4E646745E79A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Serial.h"

class CSerialFLSC10 : public CSerial  
{
public:
	CSerialFLSC10();
	virtual ~CSerialFLSC10();
	void SuspendThread();
	virtual bool Initialize(CString strPortName);
	virtual bool IStringReceived();
	virtual bool GetStr(CString *pBuff);
private:
	//Threads
	typedef struct THREADSTRUCT
	{
		CSerialFLSC10* _this;
	}THREADSTRUCT;
	static UINT SerialPortReadThread(LPVOID pParam);
	THREADSTRUCT *m_pParam;
	virtual void ByteSuccessfullyRead(BYTE* pArray,DWORD dwBytesTransfered);
};

#endif // !defined(AFX_SERIALFLSC10_H__646E5D6C_4277_4E7D_9732_4E646745E79A__INCLUDED_)
