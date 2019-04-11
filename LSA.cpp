// LSA.cpp: implementation of the CLSA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "LSA.h"
#include <math.h>
#include "MainFrm.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
	Some notes on LSA

	There will be only 3 beams for n prf cycles per sweep where n=Focal Law size-1
	We will require 128 - n Focal Laws where n=Focal Law size-1

*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLSA::CLSA()
{

	m_bDevicePresent = false;
	m_bFailedNotice = false;
}

CLSA::~CLSA()
{

}

bool CLSA::FindDevice()
{
	int nData;
	bool bFlag;

	if(m_bDevicePresent)
		return true; //Already found

	if(!theApp.m_Si10User.IsConnected())
		return false;

	m_Si10User = &theApp.m_Si10User;

	/*
	U32 VersionData = m_Si10User->InpW(0x11C);

	if(VersionData==0)
		return false;

	if( (FPGA_Type = ((VersionData & 0xFF000000)>>24)) != 76) //L
		return false; //SI10 daughter board is either not set up for LSA or is not present

	PCB_Revision = (VersionData & 0x000000FF);
	FPGA_Revision = ((VersionData & 0x0000FF00)>>8);
	FPGA_Version = ((VersionData & 0x00FF0000)>>16);
	*/


	m_bDevicePresent = true;
	bFlag = CommandTransmit(0xE4,0x03);
	m_bDevicePresent = false;

	if(bFlag == false) return false;

	if(ReadMessageRegister(nData) == true) {
		m_bDevicePresent = true;
	}

	return m_bDevicePresent;
}

bool CLSA::IsConnected()
{
	return m_bDevicePresent;
}


bool CLSA::CommandTransmit(int nCode, int nData)
{

	int ii=0;
	U32 Status;

	if(m_bFailedNotice)
		return false;

	if(m_bDevicePresent==false)
		return false;

	if (m_Si10User == NULL)
		return false;

	DWORD nTXWord = nCode<<24;			//shift to highest 8 bits
	nData &= 0x00FFFFFF;				//strip Data to 24 bits
	nTXWord |= nData;					//add data


	Status = m_Si10User->InpW(0x104);

	while ((Status & 0x00000003)) {		//Command Message In transmission
		ii++;
		if (ii == 1000) {
			if (m_bFailedNotice == false) {
				m_bFailedNotice = true;
				//				MessageBox(NULL,_T("LSA Mailbox has failed"),_T("Error"),NULL);
			}
			return false;
		}
		Status = m_Si10User->InpW(0x104);
	}

	m_Si10User->OutpW(0x100, nTXWord);

	return true;
}

bool CLSA::ReadMessageRegister(int &nData)
{
	
	int ii;
	U32 Status;

	Status = m_Si10User->InpW(0x104);
	ii=0;
	while(!(Status&0x200)){ //bit 9
		ii++;
		if(ii>100)
			return false;
		Sleep(1);
		Status = m_Si10User->InpW(0x104);
	}


	nData = (m_Si10User->InpW(0x100) &0xFF);

	return true;
}


//This will tell LSA Page to update its controls
bool CLSA::UpdateAllControls()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SendMessage(UI_UPDATE_LSA_UT_PAGE);

	return true;
}

bool CLSA::MaintainUniformTSs(int nTimeSlot)
{

	if(!m_bDevicePresent)
		return false;

	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	int ii;
	
	for(ii=0;ii<256;ii++){
		memcpy(&theApp.m_UtUser.m_TS[ii].Adc,&theApp.m_UtUser.m_TS[nTimeSlot].Adc,sizeof(theApp.m_UtUser.m_TS[nTimeSlot].Adc));
		memcpy(&theApp.m_UtUser.m_TS[ii].Dsp,&theApp.m_UtUser.m_TS[nTimeSlot].Dsp,sizeof(theApp.m_UtUser.m_TS[nTimeSlot].Dsp));
	}

	theApp.m_UtUser.CopyGatesAcrossTimeslots(nTimeSlot); //Have to do gates this way because we don't want to copy the amplitudes

	//if user has scan lines page update it
	if(pFrame->m_pMotionToolsSheet) {
		if(pFrame->m_pMotionToolsSheet->m_pScanLinesPage) {
			pFrame->m_pMotionToolsSheet->m_pScanLinesPage->UpdateAllControls();
		}
	}
	return true;
}


/***
	ApplyFocalLaws

	This is the work horse function.
	We give this function all the high level information and it:
		Works out what we want
		Calculates the focal laws
		Applys to the Hardware
	
	  I expect the number of paramters to grow
***/

int CLSA::GetFocalLawNumberFromActualLawAndOutput(int nActualLaw, int nOutput)
{
	struct LSAFocalLaw LocalFL;

	GetSetFL(nActualLaw, &LocalFL,true);
	return LocalFL.nOutputFocalLawNumber[nOutput];	

}

bool CLSA::GetActualLawAndOutputFromFocalLawNumber(int nFocalLawNumber, int &nActualLaw, int &nOutput)
{
	struct LSAFocalLaw LocalFL;

	for(int ii=0;ii<nFocalLawsL;ii++) {
		GetSetFL(ii, &LocalFL,true);
		for(int jj=0;jj<4;jj++) {
			if(LocalFL.nOutputFocalLawNumber[jj]==nFocalLawNumber) {
				nActualLaw = ii;
				nOutput = jj;
				return true;				
			}
		}
	}	

	return false;
}

bool CLSA::GetActualLawAndACMFromFocalLawNumber(int nFocalLawNumber, int &nActualLaw, int &nACM)
{
	int nOffset;
	struct LSAFocalLaw LocalFL;

	for(int ii=0;ii<nFocalLawsL;ii++) {
		GetSetFL(ii, &LocalFL,true);
		for(int jj=0;jj<4;jj++) {
			if(LocalFL.nOutputFocalLawNumber[jj]==nFocalLawNumber) {
				nActualLaw = ii + 1;
				
				switch(theApp.m_UtUser.m_nPM30Count){
				case 4:	nOffset=32;
					break;
				case 2: nOffset=64;
					break;
				default: nOffset=0; //Shoudln't be needed
					break;
				}

				GetSetFL(nActualLaw, &LocalFL,true);
				nACM = (((LocalFL.nTXStartElement - 1) + (jj*nOffset)) / 32);

				return true;				
			}
		}
	}	


	return false;



}



float CLSA::GetFLGain(int nViewLaw)
{
	return GetCollectiveGain(nViewLaw);
}

float CLSA::GetCollectiveGain(int nViewLaw)
{
	int nActualLaw, nACM;
	struct LSAFocalLaw LocalFL;

	if(GetActualLawAndACMFromFocalLawNumber(nViewLaw, nActualLaw, nACM)) {
		GetSetFL(nViewLaw, &LocalFL,true);
		return LocalFL.fCollectiveGain[nACM];
	} else {
		return 0.0;
	}
}

float CLSA::GetGainTrim(int nViewLaw)
{
	int nFL,nACM;
	struct LSAFocalLaw LocalFL;

	if(GetActualLawAndACMFromFocalLawNumber(nViewLaw,nFL,nACM)){
		GetSetFL(nFL, &LocalFL,true);
		return LocalFL.fGainTrim[nACM];
	} else {
		return 0.0;
	}

}


/*  Global Commands
	These effect all focal laws
*/

bool CLSA::CmdSetHTVoltage(int nVoltage)
{
	return CommandTransmit(0x07, nVoltage);
}

bool CLSA::CmdSetHTOnOff(bool bOn)
{
	U32 nValue = 0x00000000;

	if(bOn)
		nValue = 0x1;

	return CommandTransmit(0x08, nValue);
}

bool CLSA::CmdSetWriteFL(int nWriteLaw)
{
	if(nWriteLaw==0) {
		ASSERT(0);
	}
	return CommandTransmit(0x10, nWriteLaw);
}

bool CLSA::CmdSetActiveFL(int nActiveFL)
{
	return CommandTransmit(0x11, nActiveFL);
}

bool CLSA::CmdSetTXPulseWidth( int nWidthNS)
{
	return CommandTransmit(0x18, int(nWidthNS/10));
}

bool CLSA::CmdSetPPGain(int nPPGainDb)
{
	switch(nPPGainDb) {
	case 0:
		return CommandTransmit(0x20, 0);
	case 10:
		return CommandTransmit(0x20, 1);
	case 20:
		return CommandTransmit(0x20, 2);
	case 30:
		return CommandTransmit(0x20, 3);
	default:
		return false;
	}

}

bool CLSA::CmdSetFilters(float fLowPassMHz, float fHighPassMHz)
{
	U32 nValue;

	if(fLowPassMHz==3.5)
		nValue = 0;
	else if(fLowPassMHz==5)
		nValue = 1;
	else if(fLowPassMHz==10)
		nValue = 2;
	else if(fLowPassMHz==0xFF)
		nValue = 3;
	else
		return false;
	
	if(fHighPassMHz==0)
		nValue |= 0;
	else if(fHighPassMHz==1)
		nValue |= (1<<8);
	else if(fHighPassMHz==2)
		nValue |= (2<<8);
	else if(fHighPassMHz==5)
		nValue |= (3<<8);
	else
		return false;

	return CommandTransmit(0x42, nValue);
}
			

/*	Focal Law specific Commands
*/

/*We can receive a start element of greater than 32
	but this equates to following beams, so we have to tell the hardware start element of <=32
*/

bool CLSA::CmdSetTXBeamPos(int nWriteLaw, int nStartElement, int nFinishElement)
{
	U32 nValue = 0x00000000;
	int nStart,nFinish,nACM=0;
	struct LSAFocalLaw LocalFL;

	CmdSetWriteFL(nWriteLaw);

	if(nStartElement>32) {
		nACM = (nStartElement-1)/32; //??
		nStart = (nStartElement-1) - (32*nACM) + 1;
		nFinish = (nFinishElement-1) - (32*nACM) + 1;
	} else {
		nStart = nStartElement;
		nFinish = nFinishElement;
	}

	nValue = ((nStart&0x000000FF));
	nValue |= ((nFinish&0x000000FF)<<8);
	CommandTransmit(0x19, nValue);

	if(true/*CommandTransmit(0x19, nValue)*/){
		//keep our copies of FLs up to date as our only way of checking data in system
		GetSetFL(nWriteLaw, &LocalFL,true);
		LocalFL.nTXStartElement = nStartElement; 
		LocalFL.nTXFinishElement = nFinishElement;
		GetSetFL(nWriteLaw, &LocalFL,false);
		return true;
	}else{
		return false;
	}

}

bool CLSA::CmdSetRXBeamPos(int nWriteLaw, int nStartElement, int nFinishElement)
{

	U32 nValue = 0x00000000;
	U32 nTemp = 0x00000000;
	int nStart,nFinish,nACM;
	struct LSAFocalLaw LocalFL;

	CmdSetWriteFL(nWriteLaw);

	if(nStartElement>32) {
		nACM = (nStartElement-1)/32; //??
		nStart = (nStartElement-1) - (32*nACM) + 1;
		nFinish = (nFinishElement-1) - (32*nACM) + 1;
	} else {
		nStart = nStartElement;
		nFinish = nFinishElement;
	}

	nValue = ((nStart&0x000000FF));
	nValue |= ((nFinish&0x000000FF)<<8);

	if(CommandTransmit(0x30, nValue)){
		//keep our copies of FLs up to date as our only way of checking data in system
		GetSetFL(nWriteLaw, &LocalFL,true);
		LocalFL.nRXStartElement = nStartElement; 
		LocalFL.nRXFinishElement = nFinishElement;
		GetSetFL(nWriteLaw, &LocalFL,false);
		return true;
	}else{
		return false;
	}
}

bool CLSA::CmdSetTXDelays(int nWriteLaw, int nElement, int nDelayNS)
{
	U32 nValue = 0x00000000;
	U32 ndelay = nDelayNS/10;
	int nACM,nStart;

	CmdSetWriteFL(nWriteLaw);

	if(nElement>32) {
		nACM = (nElement-1)/32; //??
		nStart = (nElement-1) - (32*nACM) + 1;
	} else {
		nStart = nElement;
	}

	nValue = ((nStart&0x000000FF));
	nValue |= ((ndelay&0x000001FF)<<8);

	if(CommandTransmit(0x1C, nValue)){
		return true;
	}

	return false;
}



bool CLSA::CmdSetFLCollectiveGain(int nWriteLaw, float fGainDb) 
{
	int nActualLaw,nACM;
	struct LSAFocalLaw LocalFL;
	
	if(GetActualLawAndACMFromFocalLawNumber(nWriteLaw,nActualLaw,nACM)==false)
		return false;

	
	CmdSetWriteFL(nActualLaw);

	GetSetFL(nWriteLaw, &LocalFL,true);
	LocalFL.fCollectiveGain[nACM] = fGainDb;
	GetSetFL(nWriteLaw, &LocalFL,false);//set

	if(fGainDb<0)
		fGainDb=0;

	if(fGainDb>50)
		fGainDb=50;

	U32 nValue = int(fGainDb/0.05);
	nValue |= (((nACM+1)&0x07)<<20);
	
	return CommandTransmit(0x41, nValue);

}

bool CLSA::CmdSetGainTrim(int nWriteLaw, float fGaindB)
{
	int nActualLaw,nACM;
	struct LSAFocalLaw LocalFL;

	if(GetActualLawAndACMFromFocalLawNumber(nWriteLaw,nActualLaw,nACM)==false)
		return false;

	CmdSetWriteFL(nActualLaw);

	if(fGaindB<0)
		fGaindB=0;

	if(fGaindB>6)
		fGaindB=6;

	U32 nValue = int(fGaindB/0.025);
	nValue |= (((nACM+1)&0x07)<<20);

	GetSetFL(nActualLaw, &LocalFL,true);
	LocalFL.fGainTrim[nACM] = fGaindB;
	GetSetFL(nActualLaw, &LocalFL,false);//set

	return CommandTransmit(0x44,nValue);

}

void CLSA::StoreAScan(int nFocalLaw, unsigned char *pArray, int nAScanLength)
{
	m_nAscanLength = nAScanLength;
	if((nFocalLaw<=128)&&(m_nAscanLength<=8192))
		memcpy(&m_AscanData[nFocalLaw*MAX_ASCAN_LENGTH],pArray,nAScanLength);

}

void CLSA::StoreAScan(int nFocalLaw, char *pArray, int nAScanLength)
{
	m_nAscanLength = nAScanLength;
	if((nFocalLaw<=128)&&(m_nAscanLength<=8192))
		memcpy(&m_AscanData[nFocalLaw*MAX_ASCAN_LENGTH],pArray,nAScanLength);

}

bool CLSA::OldCalculateDelays(int nDepthmm, int nElements, std::vector<int> *Delaysns, float fElementPitch)
{

	double dAngle;
	double dx,dy,xx,yy, dDistance;
	double  dSmallestDelay=0;
	int i,nDelay;
	std::vector<int>::iterator iter1;
	
	
	//calculate target wavlengths to focus
	/***currently assume middle element is normal to probe so calculate angle for first probe***/
	dAngle = atan( (nDepthmm / ( ( (nElements-1)*fElementPitch) / 2 ) ) );
	
	/* Need to convert distance, to angled distances and not normal distance */
	nDepthmm = (int)sqrt( (float)(nDepthmm*nDepthmm) + (float)( (((float)(nElements-1)*fElementPitch)/2.0f)*((float)(nElements-1)*fElementPitch)/2.0f));
	
	dy = nDepthmm * sin(dAngle);
	dx = nDepthmm * cos(dAngle);
	
	Delaysns->clear();
	
	//Go through each element calculating delay 1st pass should be zero.
	for(i=0; i<nElements; i++) 
	{
		//calcuate Distance for each element
		xx = dx - (i * fElementPitch);
		yy = dy; //assuming probe parallel to surface
		dDistance = sqrt( (xx*xx)+(yy*yy) );

		nDelay = (int)((((nDepthmm-dDistance)/1000)/1482)*1e9); //in ns
		if (nDelay<dSmallestDelay)
			dSmallestDelay = nDelay; //Look for -ve delay we cant have that so add the smallest of these to all
		Delaysns->push_back(nDelay);
	}

	if(dSmallestDelay<0)
	{
		iter1 = Delaysns->begin();
		while( iter1 != Delaysns->end() ) {
			*iter1 += (int)(-dSmallestDelay);
			iter1++;
		}
	}
	
	return true;
}

int CLSA::GetLowestFLStartElement()
{
	int ii,jj,nOffset,nTemp,nElement=10000;
	struct LSAFocalLaw LocalFL;

	switch(theApp.m_UtUser.m_nPM30Count) {
	case 4:
		nOffset=32;
		break;
	case 2:
		nOffset=64;
		break;
	case 1:
		nOffset=0;
		break;
	default:
		return 0;
	}


	for(ii=1;ii<=m_nActualLawsL;ii++)
	{
		GetSetFL(ii, &LocalFL,true);
		for(jj=0;jj<4;jj++){
			if(LocalFL.bOutput[jj]==true){
				nTemp = (LocalFL.nTXStartElement) + (nOffset*jj);
				if(nTemp<nElement)
					nElement=nTemp;
			}
		}
	}


	return nElement;

}

int CLSA::GetHightesFLStartElement()
{
	int ii,jj,nOffset,nTemp,nElement=-10000;
	struct LSAFocalLaw LocalFL;

	switch(theApp.m_UtUser.m_nPM30Count) {
	case 4:
		nOffset=32;
		break;
	case 2:
		nOffset=64;
		break;
	case 1:
		nOffset=1;
		break;
	default:
		return 0;
	}


	for(ii=1;ii<m_nActualLawsL;ii++)
	{
		GetSetFL(ii, &LocalFL,true);
		for(jj=0;jj<4;jj++){
			if(LocalFL.bOutput[jj]==true){
				nTemp = (LocalFL.nTXStartElement%nOffset) + (nOffset*jj);
				if(nTemp>nElement)
					nElement=nTemp;
			}
		}
	}


	return nElement;

}


float CLSA::GetCurrentFLDistanceFromElementOne(int nFL)
{
	int nActualLaw, nOutput;
	int nOffset, nStartElement;
	float fRetDistance;
	struct LSAFocalLaw LocalFL;


	if(nFL<0)
		nFL = theApp.m_LSA.m_nScopeViewLaw;

	if(!GetActualLawAndOutputFromFocalLawNumber(nFL, nActualLaw, nOutput))
		return 0.0;

	switch(theApp.m_UtUser.m_nPM30Count) {
		case 4:
			nOffset=32;
			break;
		case 2:
			nOffset=64;
			break;
		default:
			nOffset=0;
			break;
	}

	GetSetFL(nActualLaw, &LocalFL,true);
	nStartElement = LocalFL.nTXStartElement + (nOffset*nOutput);

	fRetDistance = nStartElement*theApp.m_PhasedArray[PORTSIDE].getElementPitch();
	fRetDistance += (theApp.m_PhasedArray[PORTSIDE].getNumberElements()/2)*theApp.m_PhasedArray[PORTSIDE].getElementPitch();

	return fRetDistance;

}

bool CLSA::IsOutput(int nLaw, int nOutput)
{
	bool bRetval;
	struct LSAFocalLaw LocalFL;

	GetSetFL(nLaw, &LocalFL,true);
	bRetval = LocalFL.bOutput[nOutput];

	return bRetval;
}

//Only place where we can access m_FL
//Otherwise not thread safe
//UPDATE:: This did not fix the invalid handle fault!!
bool CLSA::GetSetFL(int nFL, struct LSAFocalLaw *FL, bool bGet)
{

	m_CriticalSection.Lock();
	if(bGet){
		memcpy(FL,&m_FL[nFL],sizeof LSAFocalLaw);
	}else{
		memcpy(&m_FL[nFL],FL,sizeof LSAFocalLaw);
	}
	m_CriticalSection.Unlock();

	return true;
}

bool CLSA::GetFL(int nFL, struct LSAFocalLaw *FL)
{

	m_CriticalSection.Lock();
	memcpy(FL,&m_FL[nFL],sizeof LSAFocalLaw);
	m_CriticalSection.Unlock();

	return true;
}

bool CLSA::SetFL(int nFL, struct LSAFocalLaw *FL)
{

	m_CriticalSection.Lock();
	memcpy(&m_FL[nFL],FL,sizeof LSAFocalLaw);
	m_CriticalSection.Unlock();

	return true;
}


bool CLSA::OldApplyFocalLaws(int &nFocalLaws, int nElements, int nStartPosition, bool bFocused, int nFocusDepth, int nScanPitch, float fArrayPitchmm)
{
	int nStartElement, nFinishElement, nTemp;
	int ii,jj,element;
	int nACM,nActualNumberofFLs,nOutput;
	std::vector<int>::iterator iter1;
	std::vector<int> Delaysns;
	bool bNewFL;
	struct LSAFocalLaw LocalFLii;
	struct LSAFocalLaw LocalFLjj;

	//some validation required here
	if(theApp.m_UtUser.m_nPM30Count<=0)
		return false;

	
	if(bFocused){
		OldCalculateDelays(nFocusDepth, nElements, &Delaysns, fArrayPitchmm);
	}else{
		for(ii=0;ii<nElements;ii++){
			Delaysns.push_back(0);
		}
	}


	for(ii=0,nActualNumberofFLs=0;ii<nFocalLaws;ii++) {

		GetSetFL(ii+1, &LocalFLii,true);
		for(jj=0;jj<4;jj++){
			LocalFLii.bOutput[jj] = false; //set all beams off, will repopulate later
			LocalFLii.nOutputFocalLawNumber[jj] = -1;
		}
		GetSetFL(ii+1, &LocalFLii,false);

		nStartElement = nStartPosition+(ii*nScanPitch);
		nFinishElement = nStartElement+nElements;

		if((nStartElement>theApp.m_PhasedArray[PORTSIDE].getNumberElements()) || (nFinishElement>theApp.m_PhasedArray[PORTSIDE].getNumberElements()) ) {
			nFocalLaws = ii;
			break;
		}


		if(theApp.m_PhasedArray[PORTSIDE].getReverseArray() == true) {
			nTemp = nStartElement;
			nStartElement = theApp.m_PhasedArray[PORTSIDE].getNumberElements() - nFinishElement;
			nFinishElement = theApp.m_PhasedArray[PORTSIDE].getNumberElements() - nTemp;
		}

		//Can we add this FL as an additional output to a previous Law?
		switch(theApp.m_UtUser.m_nPM30Count) {
		case 4:
			bNewFL=true;

			if(nStartElement<=32) {
				nOutput=0;
				bNewFL=true;
			}else if((nStartElement>32)&&(nStartElement<=64)) {
				nOutput=1;
				for(jj=0;jj<nActualNumberofFLs;jj++) {
					GetSetFL(jj+1, &LocalFLjj,true);
					if((LocalFLjj.nTXStartElement==(nStartElement-32))&&(LocalFLjj.nTXFinishElement==(nFinishElement-32))) {
						LocalFLjj.bOutput[nOutput] = true;
						LocalFLjj.nOutputFocalLawNumber[nOutput] = ii+1;
						GetSetFL(jj+1, &LocalFLjj,false);
						bNewFL=false;
					}
				}
			}else if((nStartElement>64)&&(nStartElement<=96)) {
				nOutput=2;
				for(jj=0;jj<nActualNumberofFLs;jj++) {
					GetSetFL(jj+1, &LocalFLjj,true);
					if((LocalFLjj.nTXStartElement==(nStartElement-64))&&(LocalFLjj.nTXFinishElement==(nFinishElement-64))) {
						LocalFLjj.bOutput[nOutput] = true;
						LocalFLjj.nOutputFocalLawNumber[nOutput] = ii+1;
						GetSetFL(jj+1, &LocalFLjj,false);
						bNewFL=false;
					}
				}
			}else if((nStartElement>96)&&(nStartElement<=128)) {
				nOutput=3;
				for(jj=0;jj<nActualNumberofFLs;jj++) {
					GetSetFL(jj+1, &LocalFLjj,true);
					if((LocalFLjj.nTXStartElement==(nStartElement-96))&&(LocalFLjj.nTXFinishElement==(nFinishElement-96))) {
						LocalFLjj.bOutput[nOutput] = true;
						LocalFLjj.nOutputFocalLawNumber[nOutput] = ii+1;
						GetSetFL(jj+1, &LocalFLjj,false);
						bNewFL=false;
					}
				}
			}else{
				//Shouldn't get here
				bNewFL = false;
			}

			if(bNewFL) {
				CmdSetTXBeamPos(ii+1, nStartElement, nFinishElement);
				CmdSetRXBeamPos(ii+1, nStartElement, nFinishElement);
				GetSetFL(ii+1, &LocalFLii,true);
				LocalFLii.bOutput[nOutput] = true;				
				LocalFLii.nOutputFocalLawNumber[nOutput] = ii+1;
				GetSetFL(ii+1, &LocalFLii,false);
				nActualNumberofFLs++; 
			}
			break;
		case 2:
			bNewFL=true;	
			if(nStartElement<=64) {
				nOutput=0;
				bNewFL=true;
			}else if((nStartElement>64)&&(nStartElement<=128)) {
				nOutput=1;
				for(jj=0;jj<nActualNumberofFLs;jj++) {
					GetSetFL(jj+1, &LocalFLjj,true);
					if((LocalFLjj.nTXStartElement==(nStartElement-64))&&(LocalFLjj.nTXFinishElement==(nFinishElement-64))) {
						LocalFLjj.bOutput[nOutput] = true;
						LocalFLjj.nOutputFocalLawNumber[nOutput] = ii+1;
						GetSetFL(jj+1, &LocalFLjj,false);
						bNewFL=false;
					}
				}
			}else {
				//Shouldn't get here
				bNewFL = false;
			}

			if(bNewFL) {
				GetSetFL(ii+1, &LocalFLii,true);
				LocalFLii.nOutputFocalLawNumber[nOutput] = ii+1;
				LocalFLii.bOutput[nOutput] = true; 
				GetSetFL(ii+1, &LocalFLii,false);
				CmdSetTXBeamPos(ii+1, nStartElement, nFinishElement);
				CmdSetRXBeamPos(ii+1, nStartElement, nFinishElement);
				nActualNumberofFLs++; 
			}
			break;
		case 1: //Only have 1 PM30 so only one channel
			GetSetFL(ii, &LocalFLii,true);
			LocalFLii.bOutput[0] = true; //Only 1 PM30 only first output active and always active
			LocalFLii.nOutputFocalLawNumber[0] = ii;
			GetSetFL(ii, &LocalFLii,false);
			CmdSetTXBeamPos(ii, nStartElement, nFinishElement);
			CmdSetRXBeamPos(ii, nStartElement, nFinishElement);
			nActualNumberofFLs++; 
			break;
		default:
			//Error Should not get here
			break;
		}

		
	}
	
	for(ii=0;ii<nActualNumberofFLs;ii++) {

		GetSetFL(ii, &LocalFLii,true);
		CmdSetWriteFL(ii);

		//Set MUX setting
		switch(theApp.m_UtUser.m_nPM30Count) {
		case 4:	
			nACM = LocalFLii.nTXStartElement/32;
			switch(nACM) {
			case 0:
				CommandTransmit(0x43,0); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP & OP = ACM#3 on ACM#3 OP
				break;
			case 1:
				CommandTransmit(0x43,0); //4:1 Multiplexing, OP = ACM#2 on ACM#1 OP & OP = ACM#4 on ACM#3 OP
				break;
			case 2:
				CommandTransmit(0x43,0); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP & OP = ACM#3 on ACM#3 OP
				break;
			case 3:
				CommandTransmit(0x43,0); //4:1 Multiplexing, OP = ACM#2 on ACM#1 OP & OP = ACM#4 on ACM#3 OP
				break;
			default:
				return false;
				//Error Should in  no way get here
			}//ACM Switch
			break;
			break;
		case 2: 
			nACM = LocalFLii.nTXStartElement/32;
			switch(nACM) {
			case 0:
				CommandTransmit(0x43,0); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP & OP = ACM#3 on ACM#3 OP
				CommandTransmit(0x43,2); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP & OP = ACM#3 on ACM#3 OP
				break;
			case 1:
				CommandTransmit(0x43,0); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP & OP = ACM#3 on ACM#3 OP
				CommandTransmit(0x43,5); //4:1 Multiplexing, OP = ACM#2 on ACM#1 OP & OP = ACM#4 on ACM#3 OP
				break;
			case 2:
				CommandTransmit(0x43,0); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP & OP = ACM#3 on ACM#3 OP
				CommandTransmit(0x43,2); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP & OP = ACM#3 on ACM#3 OP
				break;
			case 3:
				CommandTransmit(0x43,0); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP & OP = ACM#3 on ACM#3 OP
				CommandTransmit(0x43,5); //4:1 Multiplexing, OP = ACM#2 on ACM#1 OP & OP = ACM#4 on ACM#3 OP
				break;
			default:
				return false;
				//Error Should in  no way get here
			}//ACM Switch
			break;
		case 1:
			nACM = LocalFLii.nTXStartElement/32;
			switch(nACM) {
			case 0:
				CommandTransmit(0x43,0); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP & OP = ACM#3 on ACM#3 OP
				CommandTransmit(0x43,8); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP
				break;
			case 1:
				CommandTransmit(0x43,0); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP & OP = ACM#3 on ACM#3 OP
				CommandTransmit(0x43,9); //4:1 Multiplexing, OP = ACM#2 on ACM#1 OP
				break;
			case 2:
				CommandTransmit(0x43,0); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP & OP = ACM#3 on ACM#3 OP
				CommandTransmit(0x43,10); //4:1 Multiplexing, OP = ACM#3 on ACM#1 OP
				break;
			case 3:
				CommandTransmit(0x43,0); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP & OP = ACM#3 on ACM#3 OP
				CommandTransmit(0x43,11); //4:1 Multiplexing, OP = ACM#4 on ACM#1 OP
				break;
			default:
				return false;
				//Error Should in  no way get here
			}//ACM Switch
			break;
		}//nPM30Count switch


		iter1 = Delaysns.begin();
		for(element=0;element<nElements;element++) {				
			if(iter1!=Delaysns.end()){
				jj=*iter1;
				CmdSetTXDelays(ii,LocalFLii.nTXStartElement+element,jj);
				iter1++;
			}
		
		}
		
	}//for

	nFocalLawsL = nFocalLaws;
	m_nActualLawsL = nActualNumberofFLs;

	return true;
}


bool CLSA::ApplyFocalLaws(int &nFocalLaws, int nElements, int nStartPosition, bool bFocused, int nFocusDepth, int nScanPitch, float fArrayPitchmm)
{
	int nActualNumberofFLs,nTemp,nFL,jj,nStartElement,nFinishElement;
	struct LSAFocalLaw LocalFLii;
	int nDelaysns[128];

	for(nFL=0,nActualNumberofFLs=0;nFL<256;nFL++) {

		GetFL(nFL, &LocalFLii);
		for(jj=0;jj<4;jj++){
			LocalFLii.bOutput[jj] = false; //set all beams off, will repopulate later
			LocalFLii.nOutputFocalLawNumber[jj] = -1;
		}
		SetFL(nFL, &LocalFLii);

		nStartElement = nStartPosition+(nFL*nScanPitch) + 1;
		nFinishElement = nStartElement+nElements-1;

		if((nStartElement>(theApp.m_PhasedArray[PORTSIDE].getTxLastElement()+1)) || (nFinishElement>(theApp.m_PhasedArray[PORTSIDE].getTxLastElement()+1)) ) {
			nFocalLaws = nFL;
			break;
		}


		if(theApp.m_PhasedArray[PORTSIDE].getReverseArray() == true) {
			nTemp = nStartElement;
			nStartElement = (theApp.m_PhasedArray[PORTSIDE].getTxLastElement() + 1) - nFinishElement + 1;
			nFinishElement = (theApp.m_PhasedArray[PORTSIDE].getTxLastElement() + 1) - nTemp + 1;
		}

		GetSetFL(nFL, &LocalFLii,true);
		LocalFLii.bOutput[0] = true; //Only 1 PM30 only first output active and always active
		LocalFLii.nOutputFocalLawNumber[0] = nFL;
		GetSetFL(nFL, &LocalFLii,false);

//		CmdSetTXBeamPos(nFL+1, nStartElement, nFinishElement);
//		CmdSetRXBeamPos(nFL+1, nStartElement, nFinishElement);
		nActualNumberofFLs++; 
		
	}
	
	theApp.m_PhasedArray[PORTSIDE].CalculateDelays(nDelaysns);

	for(nFL=0;nFL<nActualNumberofFLs;nFL++) {
/*
		GetSetFL(nFL+1, &LocalFLii,true);
		CmdSetWriteFL(nFL+1);

//		nACM = LocalFLii.nTXStartElement/32;
		nACM = (LocalFLii.nTXStartElement-1)/32; //??
		switch(nACM) {
		case 0:
			CommandTransmit(0x43,0); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP & OP = ACM#3 on ACM#3 OP
			CommandTransmit(0x43,8); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP
			break;
		case 1:
			CommandTransmit(0x43,0); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP & OP = ACM#3 on ACM#3 OP
			CommandTransmit(0x43,9); //4:1 Multiplexing, OP = ACM#2 on ACM#1 OP
			break;
		case 2:
			CommandTransmit(0x43,0); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP & OP = ACM#3 on ACM#3 OP
			CommandTransmit(0x43,10); //4:1 Multiplexing, OP = ACM#3 on ACM#1 OP
			break;
		case 3:
			CommandTransmit(0x43,0); //4:1 Multiplexing, OP = ACM#1 on ACM#1 OP & OP = ACM#3 on ACM#3 OP
			CommandTransmit(0x43,11); //4:1 Multiplexing, OP = ACM#4 on ACM#1 OP
			break;
		}


		for(nElement=0;nElement<nElements;nElement++) {				
			CmdSetTXDelays(nFL+1,LocalFLii.nTXStartElement+nElement,nDelaysns[nElement]);
		}
	*/	
	}

	nFocalLawsL = nFocalLaws;
	m_nActualLawsL = nActualNumberofFLs;

	if (m_nScopeViewLaw < 0) m_nScopeViewLaw = 0;
//	if(nFocalLawsL)
//		CmdSetWriteFL(m_nScopeViewLaw+1);

	return true;

}

