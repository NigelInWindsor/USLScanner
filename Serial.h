// Serial.h: interface for the CSerial class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIAL_H__09A89D56_2AD6_4F8B_9CD2_CF8EA33AFFE7__INCLUDED_)
#define AFX_SERIAL_H__09A89D56_2AD6_4F8B_9CD2_CF8EA33AFFE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSerial  
{
public:
	bool SendStr(CString *pBuff);
	virtual bool GetStr(CString *pBuff);
	bool Hit();
	virtual bool IStringReceived();
	void WriteByte(char Byte);
	void WriteBytes(char* bytes, int length);
	void WriteBytes(unsigned char* bytes, int length);
	void WriteStr(char *string);
	char	*m_RxBuffer;
	int		m_nRxPtr;
	int		m_nLastReadPtr;
	HANDLE	hPort;
	HANDLE	hReadThread;
	CWinThread	*m_pThread;
	bool m_bThreadFinished;
	bool m_bThreadEnabled;
	bool m_bCR;
	bool m_bLF;
	WCHAR m_PortName[50];
	int m_nComPort;

	virtual void ByteSuccessfullyRead(BYTE* pArray,DWORD dwBytesTransfered);
	virtual bool Initialize(CString strPortName);
	bool Close();
	bool Start(int nComPort);
	bool Start(CString sComPort);
	CSerial();
	virtual ~CSerial();

	void SetBaudRate(int val){m_iBaudRate = val;}
	void SetByteSize(int val){m_iByteSize = val;}

protected:
	int m_iBaudRate;
	int m_iByteSize; 

};

#endif // !defined(AFX_SERIAL_H__09A89D56_2AD6_4F8B_9CD2_CF8EA33AFFE7__INCLUDED_)
