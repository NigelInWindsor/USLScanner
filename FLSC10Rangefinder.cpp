// FLSC10Rangefinder.cpp: implementation of the CFLSC10Rangefinder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "MainFrm.h"
#include "FLSC10Rangefinder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFLSC10Rangefinder::CFLSC10Rangefinder() : CLaserRangefinder()
{
	SetBaudRate(19200);
	SetByteSize(7);
	m_sPort = "COM8"; 

}

CFLSC10Rangefinder::~CFLSC10Rangefinder()
{
}


void CFLSC10Rangefinder::Measure(){
	char bytes[] = {'s', '0', 'g', '\r', '\n'};
	WriteBytes(bytes, 5);
}

void CFLSC10Rangefinder::MeasureCont(){
	char bytes[] = {'s', '0', 'h', '\r', '\n'};
	WriteBytes(bytes, 5);
}

void CFLSC10Rangefinder::StopClear(){
	SetRunningContinuously(false);
	char bytes[] = {'s', '0', 'c', '\r', '\n'};
	WriteBytes(bytes, 5);
}


void CFLSC10Rangefinder::HandleMessage(CString meas){
	int poll_timeout_ms = 0;
	if (WaitForSingleObject(CFLSC10Page::m_comsSemaphore, poll_timeout_ms) != WAIT_OBJECT_0) {
		//Already handling a message. Ignore this one.
		return;
	}

	int sLength = meas.GetLength();
	m_sHeader = meas.Left(4);	
	bool singleMeas = (m_sHeader.Find(L"g0g+")>=0);
	bool contMeas = (m_sHeader.Find(L"g0h+")>=0);
	if( singleMeas || contMeas ){ //The string contains measurement data.
		if (contMeas) SetRunningContinuously(true);
		OnMeasurementReceived(meas);
	} else if(m_sHeader.Find(L"g0@E")>=0){
		OnErrorReceived(meas);
	} else {
		CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
		if(pFrame){
			pFrame->PostMessage(REMOTE_HANDLER_FLSC10,REMOTE_FLSC10_MISC);
		}
	}
}

void CFLSC10Rangefinder::OnMeasurementReceived(CString meas){
	int sLength = meas.GetLength();
	m_sData = meas.Right(sLength-4);
	CString formatted;
	if(!m_sData.IsEmpty()){
		_WTOF(m_sData,m_fDistance);
		m_fDistance/=10;
	}
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(pFrame){
			pFrame->PostMessage(REMOTE_HANDLER_FLSC10,REMOTE_FLSC10_MEAS_REC);
	}
}


void CFLSC10Rangefinder::OnErrorReceived(CString sErr){
	m_fDistance=-1;//Set the distance to an invalid value.
	int sLength = sErr.GetLength();
	m_sError = sErr.Right(sLength-4); //The rest of the header is data
	m_errorCode = _wtoi(m_sError);
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(pFrame){
		pFrame->PostMessage(REMOTE_HANDLER_FLSC10,REMOTE_FLSC10_ERR);
	}
}



