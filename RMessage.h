// RMessage.h: interface for the CRMessage class.
//
//A thread-safe message class. 
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMESSAGE_H__476D79BA_9D92_4053_A717_161A4487C50C__INCLUDED_)
#define AFX_RMESSAGE_H__476D79BA_9D92_4053_A717_161A4487C50C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RMESSAGE_SIZE_BYTES 40094
#define RMESSAGE_DATA_SIZE_BYTES 40000
//The length in characters of the CString header items.
#define RMESSAGE_HEADERITEM_LENGTH 30

#include "RSocket.h"
#include "RClientSocket.h"
#include "USLut.h"

class CRMessage  
{
public:
	static const CString m_intro_expected;
	CRMessage();
	virtual ~CRMessage();
	//Copy constructor
	CRMessage(const CRMessage &obj);

	//Read data from the socket into the message
	int Read(CRClientSocket*, HANDLE killEventHandle);
	//Send bytes from the message to the socket
	int Send(CRClientSocket*);

	void SetDestination(CString);
	void SetCommand(CString);
	void SetData(char* pData, int nBytes);
	void AppendData(char* pData, int nBytes);
	//void SetTraceData(ADC200Data* pAdc);
	void SetData(CString sData);
	void SetData(float fData);
	void SetData(int iData);
	void SetData(long int iData);
	void AppendData(CString sData);
	void AppendData(float fData);
	void AppendData(int iData);
	void AppendData(long int iData);


    void SetGatesData(int timeSlot);
	void NullTerminate();

	CString toString(int);
	CString toString(float);

	CString GetIntro() const {return m_intro;} 
	CString GetDestination() const {return m_dest;}
	CString GetCommand() const {return m_comm;}
	char* GetData() const;
	void GetData(CString* val);
	void GetData(int* val);
	int GetDataSendSize() const {return m_dataSendSize;}

	//Size of the message buffer
	static const int m_iSize;
	//Size of the data part of the buffer
	static const int m_iDataBufSize;
	//Total size of the header
	static const int m_iHeaderSize;

	//Copy the raw message data (e.g. on receipt of a raw buffer on the network).
	void SetRawBuffer(char* source);
	//Get the raw buffer for sending on the network
	char* GetRawBuffer();

	CString toString();
	void Pack();

	bool CompareCommand(const char* command);

	bool CompareDestination(const char* dest);

	bool FindDestination(const char* dest);


private:
	//Size of each of the CString type header items
	static const int m_iHeaderItemSize;
	static const int m_iIntroSize;
	//Header items
    CString m_intro;
	CString m_dest;
	CString m_comm;

	//The size of the header in bytes.
	int m_headerSize;
	//The size of a command (3 chars + null char).
	int m_commSize;
	//The number of bytes from the data buffer to send.
	int m_dataSendSize;
	//Data buffer
	char m_pDataBuf[RMESSAGE_DATA_SIZE_BYTES];
	//Raw message buffer
	char m_pBuf[RMESSAGE_SIZE_BYTES];

	//Unpack the raw buffer
	void Unpack();
	void Unpack(char* dest, int iBytes);
	void Unpack(CString* dest);
	void Unpack(CString* dest, int iBytes);
	void Unpack(int*, int iBytes);
	void Unpack(int*);
	void CheckIntro();

	//Pack the data into the raw buffer, ready for sending.

	void Pack(char* source, int iBytes);
	void Pack(CString source, int iBytes);
	void Pack(CString source);
	void Pack(int);
	void SetDataSendSize(int);

	//Counter for packing/unpacking bytes
	int m_iBytesPacked;

	//Read string from socket to buffer
	int ReadString(CRClientSocket* pSock, char* pBuf);

	//Reas string from m_pBuf to pDesfBuf
	int CRMessage::ReadString(char* pDestBuf, int iByte);

	//For synchronisation
	CCriticalSection m_CritSection;	

	//For interruption inside a thread
	HANDLE m_hKillEvent;
};

#endif // !defined(AFX_RMESSAGE_H__476D79BA_9D92_4053_A717_161A4487C50C__INCLUDED_)
