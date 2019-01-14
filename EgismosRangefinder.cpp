// EgismosRangefinder.cpp: implementation of the CEgismosRangefinder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "EgismosRangefinder.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEgismosRangefinder::CEgismosRangefinder() : CLaserRangefinder()
{
	m_sPort = L"COM4";
	SetBaudRate(9600);
	SetByteSize(8);
	m_bEnd=false;
}

CEgismosRangefinder::~CEgismosRangefinder()
{
}

void CEgismosRangefinder::ReadSoftwareVersion(){
	BYTE bytes[] = {0xaa, 0x01, 0x01, 0x02, 0xA8};
	WriteBytes(bytes, 5);
}

void CEgismosRangefinder::ReadDeviceType(){
	BYTE bytes[] = {0xaa, 0x01, 0x02, 0x03, 0xa8};
	WriteBytes(bytes, 5);
}

void CEgismosRangefinder::ReadDeviceStatus(){
	BYTE bytes[] = {0xAA, 0x01, 0x08, 0x09, 0xA8};
	WriteBytes(bytes, 5);
}

void CEgismosRangefinder::ReadSlaveAddress(){
	BYTE bytes[] = {0xAA, 0x00, 0x04, 0x04, 0xA8};
	WriteBytes(bytes, 5);
}

void CEgismosRangefinder::SetSlaveAddress(){
	BYTE bytes[] = {0xAA, 0x00, 0x41, 0x02, 0x43, 0xA8};
	WriteBytes(bytes, 6);
}

void CEgismosRangefinder::LaserOn(){
	BYTE bytes[] = {0xAA, 0x01, 0x42, 0x43, 0xA8};
	WriteBytes(bytes, 5);
}

void CEgismosRangefinder::LaserOff(){
	BYTE bytes[] = {0xAA, 0x01, 0x43, 0x44, 0xA8};
	WriteBytes(bytes, 5);
}

void CEgismosRangefinder::BuzzerOn(){
	BYTE bytes[] = {0xAA, 0x01, 0x47, 0x01, 0x49, 0xA8};
	WriteBytes(bytes, 6);
}

void CEgismosRangefinder::BuzzerOff(){
	BYTE bytes[] = {0xAA, 0x01, 0x47, 0x00, 0x48, 0xA8};
	WriteBytes(bytes, 6);
}

void CEgismosRangefinder::Measure(){
	BYTE bytes[] = {0xAA, 0x01, 0x44, 0x45,0xA8};
	WriteBytes(bytes, 5);
}

void CEgismosRangefinder::MeasureCont(){
	BYTE bytes[] = {0xAA, 0x01, 0x45, 0x46, 0xA8};
	WriteBytes(bytes, 5);
}

void CEgismosRangefinder::StopClear(){
	BYTE bytes[] = {0xAA, 0x01, 0x46, 0x47, 0xA8};
	WriteBytes(bytes, 5);
}


void CEgismosRangefinder::HandleMessage(CString mess){
	int poll_timeout_ms = 0;
	if (WaitForSingleObject(CEgismosRangefinderPage::m_comsSemaphore, poll_timeout_ms) != WAIT_OBJECT_0) {
		//Already handling a message. Ignore this one.
		m_Crit.Unlock();
		return;
	}
	int sLength = mess.GetLength();
	CString header = mess.Left(3);
	if(header.Find(0x44) || header.Find(0x45)){ //The string contains measurement data.
		OnMeasurementReceived(mess);
	}
}

void CEgismosRangefinder::OnMeasurementReceived(CString mess){
	int sLength = mess.GetLength();
	CString rest = mess.Right(8);
	CString footer = rest.Right(2);
	//Get the data bytes
	m_sData = rest.Left(6);
	//Convert to float, in mm	

	CString head = mess.Left(3);

	//Check for error
	if(head.Compare(L"ERR") == 0){
		OnErrorReceived(mess);
		return;
	}

	CString formatted;
	if(!m_sData.IsEmpty()){
		_WTOF(m_sData,m_fDistance);
	}

	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->PostMessage(REMOTE_HANDLER_EGISMOSRF,REMOTE_EGISMOSRF_MEAS_REC);
}

void CEgismosRangefinder::OnErrorReceived(CString mess){
}

void CEgismosRangefinder::TestEndOfLineByte(BYTE val){
	if(val == 0xa8) {
			m_bEnd = true;
		}
}

void CEgismosRangefinder::ResetEndOfLineByte(BYTE val){
	if(val == 0xa8) {
			m_bEnd = false;
		}
}


bool CEgismosRangefinder::IStringReceived()
{
	bool result = m_bEnd;
	return result;
}