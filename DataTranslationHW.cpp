// DataTranslationHW.cpp: implementation of the CDataTranslationHW class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "DataTranslationHW.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
BOOL __stdcall AddBoardList( unsigned short * lpszName, unsigned short * lpszEntry, LPARAM lParam );
//////////////////////////////////////////////////////////////////////



CDataTranslationHW::CDataTranslationHW()
{
	m_hDrvr = NULL;
	m_hDass = NULL;
	m_hBuffer = NULL;
	m_bBoardEnabled = false;
	ZeroMemory(m_name, sizeof m_name);
	//this needs sorting for 2017
//	olDaEnumBoards(AddBoardList, NULL);
}

CDataTranslationHW::~CDataTranslationHW()
{
	StopADCConversion();
	if(m_hDass) olDaReleaseDASS(m_hDass);
	if(m_hDrvr) olDaTerminate(m_hDrvr);
	m_bBoardEnabled = false;
}

bool CDataTranslationHW::Start()
{
	char cStr[50] = "DT321(01)";
	unsigned int nNumberElements;


	if(strlen(m_name) > 0) {

		m_status = olDaInitialize((PTSTR)cStr, &m_hDrvr);
		if(m_hDrvr == NULL) return false;
		olDaGetDASS(m_hDrvr,OLSS_AD,0, &m_hDass);
		if(m_hDass == NULL) return false;
		olDaGetDevCaps(m_hDrvr,OLDC_ADELEMENTS,&nNumberElements);
		m_bBoardEnabled = true;

		return true;
	}

	return false;
}

void CDataTranslationHW::Close()
{

}

void CDataTranslationHW::CheckError(ECODE status)
{

}

BOOL __stdcall AddBoardList( unsigned short * lpszName, unsigned short * lpszEntry, LPARAM lParam )
{
    sprintf_s( theApp.m_DataTranslationHW.m_name,40, "%ws", (LPCWSTR)lpszName );
    return TRUE;
}

bool CDataTranslationHW::IsPresent()
{
	return m_bBoardEnabled;
}

void CDataTranslationHW::SetADCSingleValueMode()
{
	if(IsPresent()) {
		m_status = olDaSetDataFlow(m_hDass,OL_DF_SINGLEVALUE);
		m_status = olDaConfig(m_hDass);
	}
}

void CDataTranslationHW::StartADCContinuousMode()
{
	int nChannel = 0;
	double dGain[4] = {1.0, 2.0, 4.0, 8.0};
	double dFrequency = 2000.0;
	unsigned int nDMA = 0;
	unsigned int nSampleSize;
	int	ii;
	unsigned int nSize = ARRAY_SIZE;
	unsigned int nNumberChannels=0;

	if(IsPresent()) {

		m_status = olDaSetDataFlow(m_hDass,OL_DF_CONTINUOUS);
		m_status = olDaSetWrapMode(m_hDass, OL_WRP_MULTIPLE);
		m_status = olDaSetClockFrequency(m_hDass, dFrequency);
		m_status = olDaSetDmaUsage(m_hDass,nDMA);
		for(ii=0;ii<4;ii++) {
			if(theApp.m_UtUser.m_Global.Ext.bCollect[ii]) nNumberChannels++;
		};
		m_status = olDaSetChannelListSize(m_hDass,nNumberChannels);

		for(ii=0, nNumberChannels = 0 ;ii<4;ii++) {
			if(theApp.m_UtUser.m_Global.Ext.bCollect[ii]) {

				m_status = olDaSetChannelListEntry(m_hDass,nNumberChannels,ii);
				m_status = olDaSetGainListEntry(m_hDass, nNumberChannels,dGain[theApp.m_UtUser.m_Global.Ext.cGain[ii] & 3]);
				nNumberChannels++;
			}
		}
		nSize = nNumberChannels;

		m_status = olDaConfig(m_hDass);

		m_status = olDaGetResolution(m_hDass,&nSampleSize);
	    if (nSampleSize > 16)
			nSampleSize=4; //4 bytes...// e.g. 24 bits = 4 btyes
		else
			nSampleSize=2;             // e.g. 16 or 12 bits = 2 bytes

//		for(ii=0;ii<NUM_ADCS;ii++) {
			m_status = olDmCallocBuffer(0,0,nSize,nSampleSize,&m_hBuffer);
			m_status = olDaPutBuffer(m_hDass, m_hBuffer);
//		}
		m_status = olDaStart(m_hDass);
	}
}

int	CDataTranslationHW::GetLastContinuousValue(int *pnValues,int nLength)
{
	int nValue,ii;
	unsigned long nSamples;
	WORD *pArray;

	if(IsPresent() && m_hBuffer) {
		m_status = olDmGetBufferPtr( m_hBuffer,(LPVOID*)&pArray);
		m_status = olDmGetValidSamples( m_hBuffer, &nSamples );
		nValue = pArray[nSamples-1];
		for(ii=0;ii<4;ii++) {
			pnValues[ii] = pArray[nSamples-1 - ii];
		}
		m_status = olDaPutBuffer(m_hDass, m_hBuffer);
	}
	return nValue;
}

void CDataTranslationHW::StopADCConversion()
{
	int	ii;

	if(IsPresent()) {
		m_status = olDaAbort(m_hDass);

		if(m_hBuffer) {
			m_status = olDaFlushBuffers(m_hDass);
   
			for (ii=0;ii<NUM_ADCS;ii++) {
				m_status = olDaGetBuffer(m_hDass, &m_hBuffer);
				m_status = olDmFreeBuffer(m_hBuffer);
			}
			m_hBuffer = NULL;
		}
	}

}

int CDataTranslationHW::GetADCValue(int nChannel, int nGain)
{
	long nValue = 0;
	double dGain[4] = {1.0, 2.0, 4.0, 8.0};

	if(IsPresent()) {
		double dHRTime0 = m_HRTimer.GetTime();
		m_status = olDaGetSingleValue(m_hDass, &nValue, nChannel, dGain[nGain & 3]);
		double dHRTime1 = m_HRTimer.GetTime();

		double dPeriod = dHRTime1 - dHRTime0;
	}

	return nValue;
}

//This is called by CPMACUSL.InterruptSR
void CDataTranslationHW::ReadAllADC()
{
	if(IsPresent() == false) return;

	for(int gg=0;gg<4;gg++) {
		m_nExtADCArray[gg] = GetADCValue(gg, theApp.m_UtUser.m_Global.Ext.cGain[gg] );
	}

}
