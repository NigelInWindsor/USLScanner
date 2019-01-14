// SerialEgismos.h: interface for the CSerialEgismos class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALEGISMOS_H__DA2D9916_2766_467D_8EB5_88C035B86E56__INCLUDED_)
#define AFX_SERIALEGISMOS_H__DA2D9916_2766_467D_8EB5_88C035B86E56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Serial.h"

class CSerialEgismos : public CSerial  
{
public:
	CSerialEgismos();
	virtual ~CSerialEgismos();
	void SuspendThread();
	virtual bool Initialize(CString strPortName);
	virtual bool IStringReceived();
	virtual bool GetStr(CString *pBuff);
private:
	//Threads
	typedef struct THREADSTRUCT
	{
		CSerialEgismos* _this;
	}THREADSTRUCT;
	static UINT SerialPortReadThread(LPVOID pParam);
	THREADSTRUCT *m_pParam;
	bool m_bEnd;
	virtual void ByteSuccessfullyRead(BYTE* pArray,DWORD dwBytesTransfered);

};

#endif // !defined(AFX_SERIALEGISMOS_H__DA2D9916_2766_467D_8EB5_88C035B86E56__INCLUDED_)


