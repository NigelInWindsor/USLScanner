// RMessage.cpp: implementation of the CRMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "RMessage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <wchar.h>
#include <sstream>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//Size of the data buffer (body of message)
const int CRMessage::m_iDataBufSize = RMESSAGE_DATA_SIZE_BYTES;
//Size of the wchar_t* header items.
const int CRMessage::m_iHeaderItemSize = RMESSAGE_HEADERITEM_LENGTH;
//Size of the header as a whole
const int CRMessage::m_iHeaderSize = 3*m_iHeaderItemSize + sizeof(int);
//Total size
const int CRMessage::m_iSize = m_iHeaderSize + m_iDataBufSize;

const CString CRMessage::m_intro_expected = CString("URMG");

CRMessage::CRMessage() 
{
	//Check the message size is correct
	ASSERT(m_iSize == RMESSAGE_SIZE_BYTES);
	m_iBytesPacked=0;
	m_intro=m_intro_expected;
	//Default length of data string to be sent
	m_dataSendSize=0;
	m_commSize=5;
	unsigned int intSize=7;
	m_headerSize=3*m_commSize+intSize;

}	

CRMessage::~CRMessage()
{
}



//Read the data from the socket into the CRMessage
int CRMessage::Read(CRClientSocket* pSock, HANDLE killEventHandle){
	m_CritSection.Lock();
		m_hKillEvent = killEventHandle;
		unsigned int totalBytesRead=0;
		unsigned int nBytesRead=0;

	

		unsigned int bytesRemaining = m_headerSize;
		//Read the header 
	//	while(totalBytesRead < m_headerSize){
	//		pSock->GetBuffer(&m_pBuf[totalBytesRead], &nBytesRead, bytesRemaining);
	//		if(nBytesRead <=0) return -1;
	//		totalBytesRead+=nBytesRead;
	//		bytesRemaining-=nBytesRead;
	//	}

		pSock->GetBuffer(&m_pBuf[totalBytesRead], &nBytesRead, m_iSize); //Get all the data.
		
		totalBytesRead = nBytesRead;

		//Unpack the header
		//Intro
		m_iBytesPacked=0;
		Unpack(&m_intro, m_commSize);
		//Check the header intro string
		CheckIntro();
		//Get the destination string
		Unpack(&m_dest, m_commSize);
		//Get the command string
		Unpack(&m_comm, m_commSize);
		//Get the data size string
		Unpack(&m_dataSendSize);
		//Make sure we have a large enough buffer in CRMessage
		ASSERT(m_dataSendSize <= m_iDataBufSize);
		//Get the message body (data)
		unsigned int bytesToRead = totalBytesRead + m_dataSendSize;
		bytesRemaining = m_dataSendSize;
	
		//while(totalBytesRead < bytesToRead){
	//		pSock->GetBuffer(&m_pBuf[totalBytesRead], &nBytesRead, bytesRemaining);
	//		if(nBytesRead <=0) return -1;
	//		totalBytesRead+=nBytesRead;
	//		bytesRemaining-=nBytesRead;
	//	}
		//Copy data
		Unpack(m_pDataBuf, m_dataSendSize);
	//	AfxMessageBox(toString());
		m_CritSection.Unlock();
		return totalBytesRead;
}

int CRMessage::Send(CRClientSocket* pSock){
	return pSock->SendBuffer(m_pBuf, m_iBytesPacked);
}

CString CRMessage::toString(){
	CString strDataSize;
	strDataSize.Format(L"%d", m_dataSendSize);
	CString str = m_intro + L" " + m_dest + L" " + m_comm + L" " + strDataSize;
	return str;
}

//Read a string into the char array element pointed to by pBuf.
int CRMessage::ReadString(CRClientSocket* pSock, char* pBuf){
	//Read until null character
	int nBytesRead = 0;
		do {
			char* byteRead = &pBuf[nBytesRead];
			//Exit if kill event is triggered
			if(WaitForSingleObject(m_hKillEvent, 0) == WAIT_OBJECT_0){
			ReleaseSemaphore(m_hKillEvent,1,NULL);
			break;
			}
			pSock->GetByte(byteRead);
			nBytesRead++;;
		} while(pBuf[nBytesRead-1] != '\0');
	return nBytesRead;
}

//Read a string from the buffer
//iByte = starting byte to read from in the message buffer m_pBuf
int CRMessage::ReadString(char* pDestBuf, int iByte){
	//Read until null character
	char* byteRead;
	int nBytesRead = 0;
		do {
			//Get pointer to destination buffer
			byteRead = &pDestBuf[nBytesRead];
			//Get one byte;
			(*byteRead) = m_pBuf[iByte  + nBytesRead];
			nBytesRead++;;
	//	} while(m_pBuf[nBytesRead-1] != '\0');
	} while(*byteRead != '\0');
	//CString debugString = pDestBuf;
	//OutputDebugString(debugString);
	//debugString = "\n";
	//OutputDebugString(debugString);
	//wchar_t wStr[RMESSAGE_DATA_SIZE_BYTES];
	//int result = MultiByteToWideChar(CP_UTF8, NULL, pDestBuf, nBytesRead, wStr, nBytesRead);
	//debugString = wStr;
	//OutputDebugString(debugString);
	//debugString = "\n";
	//OutputDebugString(debugString);
	return nBytesRead;
}

CRMessage::CRMessage(const CRMessage &obj){
	CRMessage();
	SetDestination(obj.GetDestination());
	SetCommand(obj.GetCommand());;
	SetData(obj.GetData(),obj.GetDataSendSize());
	m_iBytesPacked=obj.m_iBytesPacked;
}


void CRMessage::SetDestination(CString val){
	//wcscpy(m_dest,val);
	m_CritSection.Lock();
	m_dest=val;
	m_CritSection.Unlock();
}

void CRMessage::SetCommand(CString val){
//	wcscpy(m_comm, val);
	m_CritSection.Lock();
	m_comm=val;
	m_CritSection.Unlock();
}

void CRMessage::SetDataSendSize(int val){
	m_CritSection.Lock();
	m_dataSendSize=val;
	m_CritSection.Unlock();
}


void CRMessage::AppendData(char* source, int nBytes){
	m_CritSection.Lock();
	memcpy(&m_pDataBuf[GetDataSendSize()], source, nBytes);
	SetDataSendSize(GetDataSendSize()+nBytes);
	m_CritSection.Unlock();
}

void CRMessage::NullTerminate(){
	CSingleLock lock = CSingleLock(&m_CritSection);
	lock.Lock();
	if(m_pDataBuf[GetDataSendSize()-1] != 0){ //Null terminate the message data if not already done.
		m_pDataBuf[GetDataSendSize()]=(char)'\0';
		SetDataSendSize(GetDataSendSize()+1);
	}
	lock.Unlock();
}

void CRMessage::SetData(char* source, int nBytes){
	m_CritSection.Lock();
	
	SetDataSendSize(0);
	//SetDataSendSize(nBytes);

	AppendData(source, nBytes);
	//memcpy(m_pDataBuf, source, nBytes);

	m_CritSection.Unlock();
}


void CRMessage::SetData(CString sData){
	m_CritSection.Lock();
	SetDataSendSize(0);
	AppendData(sData);
	m_CritSection.Unlock();
}


void CRMessage::AppendData(CString sData){
	m_CritSection.Lock();
	//Change the wide string to UTF-8 encoding
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, sData.GetBuffer(sData.GetLength()), sData.GetLength(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, sData.GetBuffer(sData.GetLength()), sData.GetLength(), &strTo[0], size_needed, NULL,NULL);
	AppendData((char*)strTo.c_str(), size_needed);
	m_CritSection.Unlock();
}

void CRMessage::SetData(float fData){
	m_CritSection.Lock();
	SetDataSendSize(0);
	AppendData(fData);
	m_CritSection.Unlock();
}

void CRMessage::AppendData(float fData){
	m_CritSection.Lock();
	CString sData;
	sData.Format(_T("%.04f"),fData);
	AppendData(sData);
	m_CritSection.Unlock();
}


void CRMessage::SetData(int iData){
	m_CritSection.Lock();
	SetDataSendSize(0);
	AppendData(iData);
	m_CritSection.Unlock();
}

void CRMessage::AppendData(int iData){
	m_CritSection.Lock();
	CString sData;
	sData.Format(_T("%d"),iData);
	AppendData(sData);
	m_CritSection.Unlock();
}


void CRMessage::SetData(long int iData){
	m_CritSection.Lock();
	SetDataSendSize(0);
	AppendData(toString(iData));
	m_CritSection.Unlock();
}

void CRMessage::AppendData(long int iData){
	m_CritSection.Lock();
	AppendData(toString(iData));
	m_CritSection.Unlock();
}


CString CRMessage::toString(int iData){
	m_CritSection.Lock();;
	CString sData;
	sData.Format(_T("%d"),iData);
	m_CritSection.Unlock();
	return sData;
}


CString CRMessage::toString(float iData){
	CSingleLock lock = CSingleLock(&m_CritSection);
	lock.Lock();
	CString sData;
	sData.Format(_T("%f"),iData);
	lock.Unlock();
	return sData;
}

void CRMessage::SetGatesData(int nTimeSlot){
	CSingleLock lock(&m_CritSection);
	lock.Lock();
	
	ADC200Data* pAdc=&theApp.m_UtUser.m_TS[nTimeSlot].Adc;
	GatesData* pGate=&theApp.m_UtUser.m_TS[nTimeSlot].Gate;

	int nGates = sizeof(pGate->nAmplitude)/sizeof(int);

	float widthSampleNs=pAdc->fSamplePeriod ;
	int interfaceTime_ns = (int)((float)pGate->nTimeSample[0]*widthSampleNs);

	SetData(nGates);
	AppendData(L",");
	AppendData(interfaceTime_ns);
	AppendData(L",");
	AppendData(theApp.m_UtUser.GetGateWaterPath(nTimeSlot));
	AppendData(L",");
	AppendData(theApp.m_nWpPrecision);
	AppendData(L",");
	AppendData(theApp.m_LastSettings.nMeasurementUnits);
	for(int i=0; i<nGates; i++){
		AppendData(L",");
		AppendData(pGate->cName[i]);
		AppendData(L",");
		AppendData(pGate->nGateMode[i]);
		AppendData(L",");
		AppendData(pGate->nNsDelay[i]);
		AppendData(L",");
		AppendData(pGate->nNsWidth[i]);
		AppendData(L",");
		AppendData(pGate->nThreshold[i]);
		AppendData(L",");
		AppendData(pGate->nAmplitude[i]);
		AppendData(L",");
		AppendData((int)(widthSampleNs*(float)pGate->nTimeSample[i]));
		AppendData(L",");
		AppendData((int)pAdc->nAcqLength);
		AppendData(L",");
		AppendData(GetRValue(theApp.m_Scope.m_rgbGateColor[i]));
		AppendData(L",");
		AppendData(GetGValue(theApp.m_Scope.m_rgbGateColor[i]));
		AppendData(L",");
		AppendData(GetBValue(theApp.m_Scope.m_rgbGateColor[i]));
	}
	lock.Unlock();
}



char* CRMessage::GetData() const{
	static char buffer[RMESSAGE_DATA_SIZE_BYTES];
	memcpy(buffer, m_pDataBuf, m_iDataBufSize);
	return buffer;
}

void CRMessage::GetData(CString* val){
	(*val) = m_pDataBuf;
}

void CRMessage::GetData(int* val){
	//Get the CString from the buffer
	int result = atoi(m_pDataBuf);
	(*val) = result;
}

void CRMessage::Pack(){
	m_CritSection.Lock();
	m_iBytesPacked=0;
	//Null terminate the data buffer.
	NullTerminate();
	//Reset the intro for sending
	m_intro=m_intro_expected;
	Pack(m_intro);
	Pack(m_dest);
	Pack(m_comm);
	Pack(m_dataSendSize);
	Pack(m_pDataBuf, m_dataSendSize);
	m_CritSection.Unlock();
}

void CRMessage::Pack(char* source, int iBytes){
	m_CritSection.Lock();
	memcpy(m_pBuf+m_iBytesPacked, source, iBytes);
	m_iBytesPacked+=iBytes;
	m_CritSection.Unlock();
}


void CRMessage::Pack(CString source, int iBytes){
	m_CritSection.Lock();
	memcpy(m_pBuf+m_iBytesPacked, source.GetBuffer(iBytes), iBytes);
	m_iBytesPacked+=iBytes;
	m_CritSection.Unlock();
}

void CRMessage::Pack(CString source){
	m_CritSection.Lock();
	wchar_t* wbuffer = (wchar_t*)source.GetBuffer(source.GetLength());
	char buffer[RMESSAGE_HEADERITEM_LENGTH];
	size_t stBufferSize;
	errno_t errNo = wcstombs_s(&stBufferSize, buffer, wbuffer, source.GetLength() + 1);
	int bufferSize = (int)stBufferSize;
	int nBytes = bufferSize;
	memcpy(m_pBuf+m_iBytesPacked, buffer, nBytes);
	m_iBytesPacked+=nBytes;
	m_CritSection.Unlock();
}

void CRMessage::Pack(int val){
	m_CritSection.Lock();
	CString str;
	str.Format(L"%06d",val);
	Pack(str);
	m_CritSection.Unlock();
}

void CRMessage::Unpack(){
	m_CritSection.Lock();
	m_iBytesPacked=0;
	//Reset the intro 
	m_intro="";
	Unpack(&m_intro);
	CheckIntro();
	Unpack(&m_dest);
	Unpack(&m_comm);
	Unpack(&m_dataSendSize);
	if(m_dataSendSize>0){
		Unpack(m_pDataBuf, m_dataSendSize);
	}
	m_CritSection.Unlock();
}	

//Check the intro in the raw buffer
void CRMessage::CheckIntro(){
	m_CritSection.Lock();
	ASSERT(m_intro.Compare(m_intro_expected) == 0);
	m_CritSection.Unlock();
}

void CRMessage::Unpack(char* dest, int iBytes){
	m_CritSection.Lock();
	memcpy(dest, m_pBuf+m_iBytesPacked, iBytes);
	m_iBytesPacked += iBytes;
	m_CritSection.Unlock();
}

void CRMessage::Unpack(CString* dest, int iBytes){
	char buff[RMESSAGE_HEADERITEM_LENGTH];
	memcpy(buff, m_pBuf+m_iBytesPacked, iBytes);
	m_iBytesPacked += iBytes;
	CString str = CString(buff);
	(*dest) = str;
}

void CRMessage::Unpack(CString* dest){
	char buff[RMESSAGE_HEADERITEM_LENGTH];
	m_iBytesPacked += ReadString(buff,m_iBytesPacked);
	CString str = CString(buff);
	(*dest) = str;
}

void CRMessage::Unpack(int* val){
	char buff[RMESSAGE_HEADERITEM_LENGTH];
	m_iBytesPacked += ReadString(buff,m_iBytesPacked);
	(*val)=atoi(buff);
}	
	

void CRMessage::Unpack(int* val, int iBytes){
	CString str;
	//Get the CString from the buffer
	Unpack(&str, iBytes);
	//Convert the CString to long int
	*val = _wtoi(str);
//	memcpy(&val, m_pBuf + m_iBytesPacked, iBytes);
	m_iBytesPacked += iBytes;
}	
	
void CRMessage::SetRawBuffer(char* source){
	CSingleLock lock = CSingleLock(&m_CritSection);
	lock.Lock();
	memcpy(m_pBuf, source, m_iSize);
	//Unpack the raw buffer into the class data members.
	Unpack();
	lock.Unlock();
}

char* CRMessage::GetRawBuffer() {
	CSingleLock lock = CSingleLock(&m_CritSection);
	lock.Lock();
	Pack();	
//	char* buffer = new char[RMESSAGE_SIZE_BYTES];
	static char buffer[RMESSAGE_SIZE_BYTES];
	//Build the raw message buffer from the class data members.
	memcpy(buffer, m_pBuf, m_iSize);
	lock.Unlock();
	return buffer;
}

bool CRMessage::CompareCommand(const char* command){
	return (m_comm.Compare(CString(command)) == 0);
}

bool CRMessage::CompareDestination(const char* dest){
	return (m_dest.Compare(CString(dest)) == 0);
}

bool CRMessage::FindDestination(const char* dest){
	return (m_dest.Find(CString(dest)) == 0);
}
