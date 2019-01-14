// Laser.cpp: implementation of the CLaser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "Laser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif





long myCallback(const HSIF_DATA_PT * pD1, const long N1, const HSIF_DATA_PT * pD2, const long N2)
{
	APP App = (APP) AfxGetApp();
	long i;
	int	nPtr;
	int nCount=0;
	LASER_ETHERNET_MSG msg;
	/*
	App->m_nLazerLoopCount++;
	App->m_LaserUser.m_nRangeDataMap.clear();


	for (i=0; i < N1; i++) {
		nPtr = pD1[i].rawE1 % NSR;
		if((pD1[i].R_X>0)) {
			App->m_LaserUser.m_nRangeDataMap.insert( std::make_pair(pD1[i].A1_Y,pD1[i].R_X)); //to display locally
	
			if(App->m_LaserUser.m_bSendData){

				if( ((nCount*sizeof(HSIF_DATA_PT))+sizeof(HSIF_DATA_PT))>=PACKET_SIZE-8) {
					msg.Instruction=2;
					msg.nDataSize=nCount*sizeof(HSIF_DATA_PT);
					App->m_LaserUser.m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);
					nCount=0;
				}

				if( ((nCount*sizeof(HSIF_DATA_PT))+sizeof(HSIF_DATA_PT))<PACKET_SIZE-8) {
					memcpy(&msg.Data[(nCount*sizeof(HSIF_DATA_PT))],&pD1[i],sizeof(HSIF_DATA_PT));
					nCount++;
				}
			}

		}
	}


	if(App->m_LaserUser.m_bSendData)
	{
		msg.Instruction=2;
		msg.nDataSize=nCount*sizeof(HSIF_DATA_PT);
		App->m_LaserUser.m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);

		msg.Instruction=13;
		msg.nDataSize=0;
		App->m_LaserUser.m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);

	}

//	if(App->m_pLazerTopographyPage!=NULL)
//		App->m_pLazerTopographyPage->Invalidate(FALSE);

	
	// return non-zero to tell lib to remove samples from its buffer
	*/
	return 1;
}


long EthernetCallback(CWnd* pParent, int nDataSize, char* Data)
{
	APP App = (APP) AfxGetApp();
	LASER_ETHERNET_MSG msg;
	int nMsgDataSize;
	int nCount=0;
	long nPtr;
	HSIF_DATA_PT RangeData;
	long nEncoder,nMotor,nRPM,nDir,nSamplesPerSec,nMinValidAmp;
	float fMinAngle,fMaxAngle,fMinRange,fMaxRange,fSingleShotData;
	int nCallbackPercent,nOffset;
	DWORD nDWTemp;

	memcpy(msg.AllData,Data,nDataSize);
	/*
	switch(msg.Instruction) {
	case 0: //handshake
		break;
	case 1: //send data
		App->m_LaserUser.m_bSendData = true;
		break;
	case 2: //recevied data
		nMsgDataSize = msg.nDataSize;
		App->m_nLazerLoopCount++;
		while(nMsgDataSize>=sizeof(HSIF_DATA_PT)){
			memcpy(&RangeData,&msg.Data[(nCount*sizeof(HSIF_DATA_PT))],sizeof(HSIF_DATA_PT));
			if(RangeData.rawE1 >=0) {
				nPtr = (abs(RangeData.rawE1) + (int)((App->m_LastSettings.fRotateOffset * (float)(NSR-1)) / 360.0f)) % NSR;
			} else {
				nOffset = abs(0x7fffffff - RangeData.rawE1);
				nPtr = (nOffset + (int)((App->m_LastSettings.fRotateOffset * (float)(NSR-1)) / 360.0f)) % NSR;
			}
			fMaxAngle = RangeData.R_X;
			fMinAngle = RangeData.A1_Y * (4096/360);
			if( (nPtr<0) || (nPtr>NSR) ) {
				//error
				//Keep Falling int here but cant work out why!!!!!
				//RangeData.rawE is wrong but other data correct how!!!!!!!
				fMinAngle=99;
			}else{
				memcpy(&App->m_LaserUser.m_DataSet[App->m_LaserUser.m_nDataSet][nPtr],&RangeData,sizeof(HSIF_DATA_PT));
				App->m_LaserUser.m_fRange[nPtr] = RangeData.R_X-App->m_LastSettings.fLaserMirrorDistance;
				App->m_LaserUser.m_nTime[nPtr] = App->m_nLazerLoopCount;
			}
			nCount++;
			nMsgDataSize-=sizeof(HSIF_DATA_PT);

		}
//		App->m_LaserUser.FilterRange(App->m_nLazerLoopCount);
//		if(App->m_pLazerTopographyPage!=NULL)
//			App->m_pLazerTopographyPage->SendMessage(WM_PAINT);
		break;
	case 3: //set rotate offset
		nMsgDataSize =0;
		float fOffset;
		memcpy(&nEncoder,&msg.Data[nMsgDataSize],sizeof(long)); nMsgDataSize+=sizeof(long);
		memcpy(&fOffset,&msg.Data[nMsgDataSize],sizeof(float)); nMsgDataSize+=sizeof(float);
		if(msg.nDataSize==nMsgDataSize){ //check this is correct
			App->m_LaserUser.setAngleOffset(nEncoder,fOffset);
		}
		break;
	case 4: //set min angle
		nMsgDataSize =0;
		memcpy(&nEncoder,&msg.Data[nMsgDataSize],sizeof(long)); nMsgDataSize+=sizeof(long);
		memcpy(&fMinAngle,&msg.Data[nMsgDataSize],sizeof(float)); nMsgDataSize+=sizeof(float);
		if(msg.nDataSize==nMsgDataSize){ //check this is correct
			App->m_LaserUser.setMinValidAngle(nEncoder,fMinAngle);
		}	
		break;
	case 5: //set max angle
		nMsgDataSize =0;
		memcpy(&nEncoder,&msg.Data[nMsgDataSize],sizeof(long)); nMsgDataSize+=sizeof(long);
		memcpy(&fMaxAngle,&msg.Data[nMsgDataSize],sizeof(float)); nMsgDataSize+=sizeof(float);
		if(msg.nDataSize==nMsgDataSize){ //check this is correct
			App->m_LaserUser.setMaxValidAngle(nEncoder,fMaxAngle);
		}	
		break;
	case 6: //set min range
		nMsgDataSize =0;
		memcpy(&fMinRange,&msg.Data[nMsgDataSize],sizeof(float)); nMsgDataSize+=sizeof(float);
		if(msg.nDataSize==nMsgDataSize){ //check this is correct
			App->m_LaserUser.setMinValidRange(fMinRange);
		}
		break;
	case 7: //set max range
		nMsgDataSize =0;
		memcpy(&fMaxRange,&msg.Data[nMsgDataSize],sizeof(float)); nMsgDataSize+=sizeof(float);
		if(msg.nDataSize==nMsgDataSize){ //check this is correct
			App->m_LaserUser.setMaxValidRange(fMaxRange);
		}
		break;
	case 8: //set SamplesPerSec
		nMsgDataSize =0;
		memcpy(&nSamplesPerSec,&msg.Data[nMsgDataSize],sizeof(long)); nMsgDataSize+=sizeof(long);
		if(msg.nDataSize==nMsgDataSize){ //check this is correct
			App->m_LaserUser.setSamplesPerSec(nSamplesPerSec);
		}
		break;
	case 9: //set Motor RPM
		nMsgDataSize =0;
		memcpy(&nMotor,&msg.Data[nMsgDataSize],sizeof(long)); nMsgDataSize+=sizeof(long);
		memcpy(&nRPM,&msg.Data[nMsgDataSize],sizeof(long)); nMsgDataSize+=sizeof(long);
		memcpy(&nDir,&msg.Data[nMsgDataSize],sizeof(long)); nMsgDataSize+=sizeof(long);
		if(msg.nDataSize==nMsgDataSize){ //check this is correct
			App->m_LaserUser.setMotorRPM(nMotor,nRPM,nDir);
		}
		break;
	case 10: //set Discard Off
		nMsgDataSize =0;
		if(msg.nDataSize==nMsgDataSize){ //check this is correct
			App->m_LaserUser.setDiscardInvalidOff();
		}
		break;
	case 11: //set Discard On
		nMsgDataSize =0;
		if(msg.nDataSize==nMsgDataSize){ //check this is correct
			App->m_LaserUser.setDiscardInvalidOn();
		}
		break;
	case 12: //set callback threshold
		nMsgDataSize =0;
		memcpy(&nCallbackPercent,&msg.Data[nMsgDataSize],sizeof(int)); nMsgDataSize+=sizeof(int);
		if(msg.nDataSize==nMsgDataSize){ //check this is correct
			App->m_LaserUser.setCallbackThreshold(nCallbackPercent);
		}
		break;
	case 13: //new data set
		App->m_LaserUser.m_nDataSet++;

		if(App->m_LaserUser.m_nDataSet>MAX_DATASETS)
			App->m_LaserUser.m_nDataSet = MAX_DATASETS;

		if(App->m_LaserUser.m_nDataSet>=App->m_LastSettings.nLaserAverageLines) {
			App->m_LaserUser.m_bNewData = true;
			App->m_LaserUser.CalculateData();
			App->m_LaserUser.m_nDataSet=0;
			
			nDWTemp = GetTickCount();
			if(nDWTemp>App->m_LaserUser.m_Timer)
				App->m_LaserUser.m_fDataSamplePerSec = 1000.0f / ((float)nDWTemp - (float)App->m_LaserUser.m_Timer);
			App->m_LaserUser.m_Timer = nDWTemp;
			
//			if(App->m_pLazerTopographyPage!=NULL)
//				App->m_pLazerTopographyPage->Invalidate(FALSE);

//			if(App->m_p3DViewEditDlg!=NULL)
//				if(App->m_p3DViewEditDlg->m_PaintMode==3)
//					App->m_p3DViewEditDlg->Invalidate(FALSE);

		}
//			if(App->m_pLazerTopographyPage!=NULL)
//				App->m_pLazerTopographyPage->Invalidate(FALSE);
		break;
	case 14: //min valid amplitude
		nMsgDataSize =0;
		memcpy(&nMinValidAmp,&msg.Data[nMsgDataSize],sizeof(long)); nMsgDataSize+=sizeof(long);
		if(msg.nDataSize==nMsgDataSize){ //check this is correct
			App->m_LaserUser.setMinValidAmplitude(nMinValidAmp);
		}
		break;
	case 15: //Require Single Shot?
		nMsgDataSize=0;
		if(msg.nDataSize==sizeof(bool)){ //check this is correct
			memcpy(&App->m_LastSettings.bLaserSingleShot,&msg.Data[nMsgDataSize],sizeof(bool));
			App->SaveLastSettings();
		}
		break;
	case 16: //Receive Single Shot Data
		nMsgDataSize=0;
		memcpy(&fSingleShotData,&msg.Data[nMsgDataSize],sizeof(long)); nMsgDataSize+=sizeof(long);
		if(msg.nDataSize==nMsgDataSize){ //check this is correct
			App->m_LaserUser.m_fSingleShotData = fSingleShotData;
		}
		break;
	case 17: // laser off
		nMsgDataSize=0;
		memcpy(&fSingleShotData,&msg.Data[nMsgDataSize],sizeof(long)); nMsgDataSize+=sizeof(long);
		if(msg.nDataSize==nMsgDataSize){ //check this is correct
			App->m_LaserUser.LaserOff();
		}
		break;
	case 18: // laser on
		nMsgDataSize=0;
		memcpy(&fSingleShotData,&msg.Data[nMsgDataSize],sizeof(long)); nMsgDataSize+=sizeof(long);
		if(msg.nDataSize==nMsgDataSize){ //check this is correct
			App->m_LaserUser.LaserOn();
		}
		break;

	default:
		int gg=99;
		break;
	}//switch
	*/
	return 0;

}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLaser::CLaser()
{
	m_bDevicePresent=false;
	m_nDataSet = 0;
	m_Timer = GetTickCount();
	m_CpRect[0].Zero();
	m_CpRect[1].Zero();
	m_DataCount = 0;


}

CLaser::~CLaser()
{
	m_LaserScanner.setCommClosed();
}

bool CLaser::FindDevice() 
{

	APP App = (APP) AfxGetApp();
	int ii=0;


	if(App->m_LastSettings.nLaserLocation==0)
		return false;

	if(App->m_LastSettings.nLaserLocation==1) {
		long result;
		result = m_LaserScanner.setDriverOpen();
		if (result != CTI_SUCCESS) {
			TRACE1("ERROR: setDriverOpen returned error: %d\n", result);
			return false;
		}

		CString srcString = App->m_WorkingDirectory + _T("\\Crandun Technologies\\Calibration\\lookuphs");
		char *pStr = new char[srcString.GetLength()+1];
		for(int ii=0;ii<srcString.GetLength();ii++) {
			pStr[ii]=(char)srcString.GetAt(ii);
		}
		pStr[ii]=0;
		result = m_LaserScanner.setCalibrationFile(pStr);
		if (result != CTI_SUCCESS) {
			TRACE1("ERROR: setCalibrationFile returned error: %d\n", result);
			return false;
		}
		SAFE_DELETE( pStr );

		result = m_LaserScanner.setCommOpen("COM1", 9600);
		if (result != CTI_SUCCESS) {
			TRACE1("ERROR: setCommOpen returned error: %d\n",result);
			return false;
		}
		App->m_nLazerLoopCount = 0;
	}

	m_bDevicePresent=true;


	if(App->m_LastSettings.nLaserLocation==1) { //local so act as server
  		m_Ethernet.Connect(NULL,true,EthernetCallback);
		m_bSendData=true; //if we own laser spend out data even if we don't request it
	}else if(App->m_LastSettings.nLaserLocation==2){ //remote so act as client
		m_Ethernet.Connect(NULL,false,EthernetCallback,App->m_LastSettings.nLaserIPAddrs);
	}

	while( (!m_Ethernet.Connected()) && (ii<5000) ){
		Sleep(1);
		ii++;
	}

//	Sleep(5000);

	return true;
	
}



void CLaser::InitializeLaser()
{
	APP App = (APP) AfxGetApp();
	long result;
	if(m_bDevicePresent != true) return;

	if(App->m_LastSettings.nLaserLocation==1) {
	
		result = m_LaserScanner.setMotorMaxRPM(1, 2600);
		CheckLazerSensorReturnValue(result);

		result = m_LaserScanner.setEncoderCountsPerRev(1, NSR);
		CheckLazerSensorReturnValue(result);

		result = m_LaserScanner.setSensorMaxRange(81);

		result = m_LaserScanner.setCallbackFunction(myCallback);
		CheckLazerSensorReturnValue(result);

		result = m_LaserScanner.setBufferSize(MAX_SAMPLES);
		CheckLazerSensorReturnValue(result);

		result = m_LaserScanner.setOutputFormatMetric();
		CheckLazerSensorReturnValue(result);

		m_LaserScanner.setClearBuffer();	
	}

	setMotorRPM(1, App->m_LastSettings.nLaserMotorRPM, 1);

	setSamplesPerSec(App->m_LastSettings.nLaserSamplesPerSecond);

	setCallbackThreshold(App->m_LastSettings.nLaserCallbackThresholdPercent); 

	//offset
	setAngleOffset(1,App->m_LastSettings.fRotateOffset); 

	//Angles
	setMinValidAngle(1,360-(App->m_LastSettings.fAngleRange/2)); 
	setMaxValidAngle(1,(App->m_LastSettings.fAngleRange/2));	
	
	//Range	
	setMinValidRange(App->m_LastSettings.fMinDistance);
	setMaxValidRange(App->m_LastSettings.fMaxDistance);

	//Set Min Valid Amplitude
	setMinValidAmplitude(App->m_LastSettings.nLaserMinValidAmplitude);

	if(App->m_LastSettings.bLaserDiscard) {
		setDiscardInvalidOn();
	}else{
		setDiscardInvalidOff();
	}

}

void CLaser::CheckLazerSensorReturnValue(long nResult)
{
	APP App = (APP) AfxGetApp();

	if(App->m_LastSettings.nLaserLocation==1)	{ //laser local
		if (nResult < 0) {
			TRACE1("ERROR: fnName returned error code: %d\n",nResult);
			char errMsg[300]; 
			if (m_LaserScanner.getIsError()) {
				m_LaserScanner.getErrorMessage(errMsg, sizeof(errMsg));
				TRACE1("Library error msg is: %d\n",errMsg);
			}
		}
	}

}

void CLaser::UpdateAllControls()
{
	APP App = (APP) AfxGetApp();

//	if(App->m_pLazerTopographyPage!=NULL)
//		App->m_pLazerTopographyPage->UpdateAllControls();
}

void CLaser::CalculateData()
{
	APP App = (APP) AfxGetApp();
	int i,j,k;
	float fCumRange;	
	std::map<float,float>::iterator iter;


	return;
	int nDataSet=0; //need to loop through each line

	/***********************************************************************************************************************/
	//Go through each datset and averge smaples within it
	for(nDataSet=0;nDataSet<App->m_LastSettings.nLaserAverageLines;nDataSet++) {

		for(i=0;i<NSR;i+=App->m_LastSettings.nLaserAverageSamples){
			fCumRange=0;
			k=0;
			for(j=0;j<App->m_LastSettings.nLaserAverageSamples;j++){
				if(m_DataSet[nDataSet][i+j].R_X>0) {
					fCumRange+=m_DataSet[nDataSet][i+j].R_X;
					k++;
				}
			}
			for(j=0;j<App->m_LastSettings.nLaserAverageSamples;j++){
				if(k==0) {
					m_DataSet[nDataSet][i+j].R_X = 0;
				}else{
					m_DataSet[nDataSet][i+j].R_X = (fCumRange/k)-App->m_LastSettings.fLaserMirrorDistance;
				}
			}
		}
	}



	/************************************************************************************************************************/
	// average accross data sets

	m_CriticalSection.Lock();	
	m_nRangeDataMap.clear();
	float ff;
	bool bMarkerAngle = false;

	for(i=0;i<NSR;i++){
		k=0;
		fCumRange=0;

		for(nDataSet=0;nDataSet<App->m_LastSettings.nLaserAverageLines;nDataSet++){
			if(m_DataSet[nDataSet][i].R_X>0){
			
				ff = m_fMarkerAngle*RAD_TO_DEG;
				if(App->m_LastSettings.bLaserReverseEncoder) 
					ff = 360-ff;
			
				if(i!=NSR) {
					if( (m_DataSet[0][i].A1_Y<ff) && (m_DataSet[0][i+1].A1_Y>ff) ) {
						bMarkerAngle = true;
					}
				}
			
				fCumRange += m_DataSet[nDataSet][i].R_X;
				k++;
			}
		}

		if(bMarkerAngle) {
			m_nActualAverageLines = k;
			bMarkerAngle = false;
		}

	


		if((k!=0)&&((fCumRange/k)>0)){
			if(App->m_LastSettings.bLaserReverseEncoder) {
				m_nRangeDataMap.insert( std::make_pair((360.0-m_DataSet[0][i].A1_Y),fCumRange/k));
			}else{
				m_nRangeDataMap.insert( std::make_pair(m_DataSet[0][i].A1_Y,fCumRange/k));
			}
		}



	}

	m_CriticalSection.Unlock();
	


	/*************************************************************************************************/
	//Remove points with excessive variance from nearby points
	//First forwards

	float fAngles[NSR];
	float fDeleteAngles[NSR];
	int nDeleteAnglesL;
	float fAllowedDiffinY, y1, y2;
	bool bDeleteFlag;

	nDeleteAnglesL = 0;
	m_CriticalSection.Lock();

	for (iter = m_nRangeDataMap.begin(), i=0; iter != m_nRangeDataMap.end(); iter++, i++) {
		fAngles[i] = iter->first;
	}

	if( abs(m_nRangeDataMap[fAngles[i]]-m_nRangeDataMap[fAngles[i+1]])>5) {
		fDeleteAngles[nDeleteAnglesL] = fAngles[i];
		nDeleteAnglesL++;
	}


	//forwards
	for(i=0;i<(int)m_nRangeDataMap.size()-1;i++){
		bDeleteFlag = true;
		
		fAllowedDiffinY = App->m_LastSettings.fLaserMaxYDeviation*fabs(fAngles[i]-fAngles[i+1]);
		y1 = m_nRangeDataMap[fAngles[i]]*cosf(fAngles[i]*DEG_TO_RAD);
		y2 = m_nRangeDataMap[fAngles[i+1]]*cosf(fAngles[i+1]*DEG_TO_RAD);
		if( fabs(y1-y2) <= fAllowedDiffinY)
			bDeleteFlag=false;


		if(bDeleteFlag) {
			fDeleteAngles[nDeleteAnglesL] = fAngles[i];
			nDeleteAnglesL++;
		}
	}

	//backwards
	for(i=m_nRangeDataMap.size();i>0;i--){

		bDeleteFlag = true;

		fAllowedDiffinY = App->m_LastSettings.fLaserMaxYDeviation*fabs(fAngles[i]-fAngles[i-1]);
		y1 = m_nRangeDataMap[fAngles[i]]*cosf(fAngles[i]*DEG_TO_RAD);
		y2 = m_nRangeDataMap[fAngles[i-1]]*cosf(fAngles[i-1]*DEG_TO_RAD);

		if( fabs(y1-y2) <= fAllowedDiffinY)
			bDeleteFlag=false;

		if(bDeleteFlag) {
			fDeleteAngles[nDeleteAnglesL] = fAngles[i];
			nDeleteAnglesL++;
		}

	}
	
	for(i=0;i<nDeleteAnglesL;i++) {
		m_nRangeDataMap.erase(fDeleteAngles[i]);
	}


	m_CriticalSection.Unlock();

	/*******************************************************************************************/
	//FIR 

	if(App->m_LastSettings.bLaserFIR) {
		
		int N=40;
		float sample;
		int oldest=0;
		double total=0;

		//generated using FIRDsgn Low pass filter
		double taps[41] = 
		{
			0.003671,
			0.003933,
			0.004709,
			0.005982,
			0.007719,
			0.009880,
			0.012411,
			0.015250,
			0.018329,
			0.021570,
			0.024894,
			0.028220,
			0.031465,
			0.034549,
			0.037396,
			0.039936,
			0.042106,
			0.043851,
			0.045130,
			0.045910,
			0.046173,
			0.045910,
			0.045130,
			0.043851,
			0.042106,
			0.039936,
			0.037396,
			0.034549,
			0.031465,
			0.028220,
			0.024894,
			0.021570,
			0.018329,
			0.015250,
			0.012411,
			0.009880,
			0.007719,
			0.005982,
			0.004709,
			0.003933,
			0.003671
		};

		for(i=0;i<N;i++) {
			total+=taps[i];
		}
		for(i=0;i<N;i++) {
			taps[i]/=total; //scale around 0
		}

		float CirBuff[40];
		for(i=0;i<N;i++) {
			CirBuff[i]=0;
		}

		m_CriticalSection.Lock();

		for (iter = m_nRangeDataMap.begin(), i=0; iter != m_nRangeDataMap.end(); iter++, i++) {

			sample = iter->second;
			CirBuff[oldest] = sample; 
			float y = 0; 
			for (int k = 0; k < N; k++) 
			{ 
				y += (float)taps[k] * CirBuff[(oldest + k) % N]; 
			} 
			oldest = (oldest + 1) % N; 

			iter->second = y;

			//m_points[0][m_nNumofPoints[0]].x=i-(N/2);
			//m_points[0][m_nNumofPoints[0]].y=(FindModulus((char)y))*m_fGain;
			//m_points[0][m_nNumofPoints[0]].y=y*m_fGain;
		}


		m_CriticalSection.Unlock();
	}//if


	/*******************************************************************************************/
	//reset data sets
	ResetDataSets();


	/*******************************************************************************************/
	//Create Taught Line
	if(	App->m_bLaserCollectTaughtLines==TRUE) {
		App->m_Thread.CollectLaserTaughtLineData();
	}

	/*******************************************************************************************/
	//Increment Data Count
	m_DataCount++;

	/*******************************************************************************************/
	//Update user managed event
	if(!m_bDataCollected)
		m_bDataCollected = true;

}

void CLaser::TakeTaughtPoints(int nLine, float fMaxZ, float fMinZ)
{
	APP App = (APP) AfxGetApp();
	int ii;
	float fAngle;
	float nZ,nOldZ=-1000;
	float nZIncrement=0;
	CCoord CpLaser,CpSurface;
	bool bNewLine=false;
	CPolyCoord CpLine;
	CCoord CpHead;
	App->m_Motors.GetHeadPos(&CpHead);
	bool bRecordPoint=true;
	bool bUseTolerances=false;


	if(fMaxZ!=fMinZ)
		bUseTolerances = true;


	float fStartAngle = 360 - (App->m_LastSettings.fAngleRange/2);
	float fFinishAngle = (App->m_LastSettings.fAngleRange/2);

	if((nLine==-1)||(nLine>=PROFILE->m_nTaughtLineL)) {
			bNewLine=true;
	}else{
		for(ii=0;ii<PROFILE->m_TaughtLine[nLine].m_nCoordL; ii++) {
//			TRACE1("%f\n",PROFILE->m_TaughtLine[nLine].m_pCp[ii].Side0.fX);
			if(PROFILE->m_TaughtLine[nLine].m_pCp[ii].Side0.fZ > fMinZ)
				fMinZ = PROFILE->m_TaughtLine[nLine].m_pCp[ii].Side0.fZ;		
		}
	}


//	App->m_LaserUser.m_CriticalSection.Lock();
	std::map<float,float>::iterator iter;

	for (iter = m_nRangeDataMap.begin(); iter != m_nRangeDataMap.end(); iter++ ) {
		fAngle = iter->first * DEG_TO_RAD;	
		if( (fAngle>0) && (fAngle<(360*DEG_TO_RAD)) ) {
			if((fAngle>(DEG_TO_RAD*fStartAngle))||(fAngle<(DEG_TO_RAD*fFinishAngle))) {
				
				float fRange = iter->second;

				if( (fRange<App->m_LastSettings.fMaxDistance) && (fRange>App->m_LastSettings.fMinDistance) ) {

					CpLaser.Side0.fX = CpHead.Side0.fX;				
					CpLaser.Side0.fY = cosf(fAngle) * fRange; 
				
					if(App->m_LastSettings.bLaserReverseEncoder) {
						CpLaser.Side0.fZ = (sinf(fAngle) * fRange) + CpHead.Side0.fZ; 	
					}else{
						CpLaser.Side0.fZ = (-sinf(fAngle) * fRange) + CpHead.Side0.fZ; 	
					}
					
					CpLaser.Side0.fI = 0;
					CpLaser.Side0.fJ = -1;
					CpLaser.Side0.fK = 0;

					App->m_Kinematics.SurfaceFromLaser(&CpLaser, &CpSurface);

					CpSurface.CopySide(STARBOARD,CpSurface);


					nZ = CpSurface.Side0.fZ;

					if(bUseTolerances) {
						if((nZ<fMinZ)||(nZ>fMaxZ))
							bRecordPoint=false;
					}

					if(bRecordPoint){
						if(bNewLine) {
							CpLine.Zero();
							CpLine.AddCoord(CpSurface);
							nLine=PROFILE->AddLine(TAUGHTLINES, &CpLine,PROFILE->m_nSlowAxis); //slowaxis???
							bNewLine=false;
						} else if(abs(nZ-nOldZ)>nZIncrement){
							nOldZ = nZ;
							if(PROFILE->m_TaughtLine==NULL) {
								PROFILE->m_TaughtLine = new CPolyCoord[PROFILE->m_nTaughtLineL=1];
							}

							PROFILE->m_TaughtLine[nLine].AddCoord(CpSurface);
						}
					}		
				}
				
				bRecordPoint = true;
			}
		}
	}

//	App->m_LaserUser.m_CriticalSection.Unlock();
}

int CLaser::setAngleOffset(long EncoderNumber, float fOffset) 
{
	APP App = (APP) AfxGetApp();
	LASER_ETHERNET_MSG msg;
	int nDataSize=0;
	int nRetVal=-1;

	switch(App->m_LastSettings.nLaserLocation) {
	case 0: //no laser
		break;
	case 1: //local
		nRetVal = m_LaserScanner.setAngleOffset(EncoderNumber, fOffset);
		App->m_LastSettings.fRotateOffset = fOffset;
		UpdateAllControls();
		break;
	case 2: //remote
		msg.Instruction = 3;
		memcpy(&msg.Data[nDataSize],&EncoderNumber,sizeof(long)); nDataSize += sizeof(long);
		memcpy(&msg.Data[nDataSize],&fOffset,sizeof(float)); nDataSize += sizeof(float);
		msg.nDataSize = nDataSize;
		nRetVal = m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);
		break;
	}//switch

	return nRetVal;
}

int CLaser::setMinValidAngle(long EncoderNumber, float fMinAngle) 
{
	APP App = (APP) AfxGetApp();
	LASER_ETHERNET_MSG msg;
	int nDataSize=0;
	int nRetVal=-1;

	fMinAngle=0.0f;
	switch(App->m_LastSettings.nLaserLocation) {
	case 0: //no laser
		break;
	case 1: //local
		nRetVal = m_LaserScanner.setMinValidAngle(EncoderNumber, fMinAngle);
		App->m_LastSettings.fAngleRange = (360-fMinAngle)*2;
		UpdateAllControls();
		break;
	case 2: //remote
		msg.Instruction = 4;
		memcpy(&msg.Data[nDataSize],&EncoderNumber,sizeof(long)); nDataSize += sizeof(long);
		memcpy(&msg.Data[nDataSize],&fMinAngle,sizeof(float)); nDataSize += sizeof(float);
		msg.nDataSize = nDataSize;
		nRetVal = m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);
		break;
	}//switch

	return nRetVal;
}


int CLaser::setMaxValidAngle(long EncoderNumber, float fMaxAngle) 
{
	APP App = (APP) AfxGetApp();
	LASER_ETHERNET_MSG msg;
	int nDataSize=0;
	int nRetVal=-1;

	fMaxAngle=360.0f;
	switch(App->m_LastSettings.nLaserLocation) {
	case 0: //no laser
		break;
	case 1: //local
		nRetVal = m_LaserScanner.setMaxValidAngle(EncoderNumber, fMaxAngle);
		App->m_LastSettings.fAngleRange = fMaxAngle*2;
		UpdateAllControls();
		break;
	case 2: //remote
		msg.Instruction = 5;
		memcpy(&msg.Data[nDataSize],&EncoderNumber,sizeof(long)); nDataSize += sizeof(long);
		memcpy(&msg.Data[nDataSize],&fMaxAngle,sizeof(float)); nDataSize += sizeof(float);
		msg.nDataSize = nDataSize;
		nRetVal = m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);
		break;
	}//switch

	return nRetVal;
}

int CLaser::setMinValidRange(float fMinRange) 
{
	APP App = (APP) AfxGetApp();
	LASER_ETHERNET_MSG msg;
	int nDataSize=0;
	int nRetVal=-1;

	switch(App->m_LastSettings.nLaserLocation) {
	case 0: //no laser
		break;
	case 1: //local
		nRetVal = m_LaserScanner.setMinValidRange(fMinRange);
		App->m_LastSettings.fMinDistance = fMinRange;
		UpdateAllControls();
		break;
	case 2: //remote
		msg.Instruction = 6;
		memcpy(&msg.Data[nDataSize],&fMinRange,sizeof(float)); nDataSize += sizeof(float);
		msg.nDataSize = nDataSize;
		nRetVal = m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);
		break;
	}//switch

	return nRetVal;
}


int CLaser::setMaxValidRange(float fMaxRange) 
{
	APP App = (APP) AfxGetApp();
	LASER_ETHERNET_MSG msg;
	int nDataSize=0;
	int nRetVal=-1;

	switch(App->m_LastSettings.nLaserLocation) {
	case 0: //no laser
		break;
	case 1: //local
		nRetVal = m_LaserScanner.setMaxValidRange(fMaxRange);
		App->m_LastSettings.fMaxDistance = fMaxRange;
		UpdateAllControls();
		break;
	case 2: //remote
		msg.Instruction = 7;
		memcpy(&msg.Data[nDataSize],&fMaxRange,sizeof(float)); nDataSize += sizeof(float);
		msg.nDataSize = nDataSize;
		nRetVal = m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);
		break;
	}//switch

	return nRetVal;
}

int CLaser::setSamplesPerSec(long nSamplesPerSec) 
{
	APP App = (APP) AfxGetApp();
	LASER_ETHERNET_MSG msg;
	int nDataSize=0;
	int nRetVal=-1;

	switch(App->m_LastSettings.nLaserLocation) {
	case 0: //no laser
		break;
	case 1: //local
		nRetVal = m_LaserScanner.setSamplesPerSec(nSamplesPerSec);
		App->m_LastSettings.nLaserSamplesPerSecond = nSamplesPerSec;
		UpdateAllControls();
		break;
	case 2: //remote
		msg.Instruction = 8;
		memcpy(&msg.Data[nDataSize],&nSamplesPerSec,sizeof(long)); nDataSize += sizeof(long);
		msg.nDataSize = nDataSize;
		nRetVal = m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);
		break;
	}//switch

	return nRetVal;
}

int CLaser::setMotorRPM(long nMotor, long nRPM, long nDir) 
{
	APP App = (APP) AfxGetApp();
	LASER_ETHERNET_MSG msg;
	int nDataSize=0;
	int nRetVal=-1;

	switch(App->m_LastSettings.nLaserLocation) {
	case 0: //no laser
		break;
	case 1: //local
		nRetVal = m_LaserScanner.setMotorRPM(nMotor,nRPM,nDir);
		App->m_LastSettings.nLaserMotorRPM = nRPM;
		UpdateAllControls();
		break;
	case 2: //remote
		msg.Instruction = 9;
		memcpy(&msg.Data[nDataSize],&nMotor,sizeof(long)); nDataSize += sizeof(long);
		memcpy(&msg.Data[nDataSize],&nRPM,sizeof(long)); nDataSize += sizeof(long);
		memcpy(&msg.Data[nDataSize],&nDir,sizeof(long)); nDataSize += sizeof(long);
		msg.nDataSize = nDataSize;
		nRetVal = m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);
		break;
	}//switch

	return nRetVal;
}

int CLaser::setDiscardInvalidOff()
{

	APP App = (APP) AfxGetApp();
	LASER_ETHERNET_MSG msg;
	int nDataSize=0;
	int nRetVal=-1;

	switch(App->m_LastSettings.nLaserLocation) {
	case 0: //no laser
		break;
	case 1: //local
		nRetVal = m_LaserScanner.setDiscardInvalidOff();
		App->m_LastSettings.bLaserDiscard = false;
		UpdateAllControls();
		break;
	case 2: //remote
		msg.Instruction = 10;
		msg.nDataSize = nDataSize;
		nRetVal = m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);
		break;
	}//switch

	return nRetVal;
}

int CLaser::setDiscardInvalidOn()
{

	APP App = (APP) AfxGetApp();
	LASER_ETHERNET_MSG msg;
	int nDataSize=0;
	int nRetVal=-1;

	switch(App->m_LastSettings.nLaserLocation) {
	case 0: //no laser
		break;
	case 1: //local
		nRetVal = m_LaserScanner.setDiscardInvalidOn();
		App->m_LastSettings.bLaserDiscard = true;
		UpdateAllControls();
		break;
	case 2: //remote
		msg.Instruction = 11;
		msg.nDataSize = nDataSize;
		nRetVal = m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);
		break;
	}//switch

	return nRetVal;
}


int CLaser::setCallbackThreshold(int nPercent)
{
	APP App = (APP) AfxGetApp();
	LASER_ETHERNET_MSG msg;
	int nDataSize=0;
	int nRetVal=-1;
	int nNumberSamples;

	switch(App->m_LastSettings.nLaserLocation) {
	case 0: //no laser
		break;
	case 1: //local
		if(nPercent<1)nPercent=1;
		if(nPercent>100)nPercent=100;
		nNumberSamples = MAX_SAMPLES/(100/nPercent);
		nRetVal = m_LaserScanner.setCallbackThreshold(nNumberSamples);
		App->m_LastSettings.nLaserCallbackThresholdPercent = nPercent;
		UpdateAllControls();
		break;
	case 2: //remote
		msg.Instruction = 12;
		memcpy(&msg.Data[nDataSize],&nPercent,sizeof(int)); nDataSize += sizeof(int);
		msg.nDataSize = nDataSize;
		nRetVal = m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);
		break;
	}//switch

	return nRetVal;
}

int CLaser::setMinValidAmplitude(long amp)
{

	APP App = (APP) AfxGetApp();
	LASER_ETHERNET_MSG msg;
	int nDataSize=0;
	int nRetVal=-1;

	switch(App->m_LastSettings.nLaserLocation) {
	case 0: //no laser
		break;
	case 1: //local
		if(amp<0) amp=0;
		if(amp>255) amp=255;
		nRetVal = m_LaserScanner.setMinValidAmplitude(amp);
		App->m_LastSettings.nLaserMinValidAmplitude = amp;
		UpdateAllControls();
		break;
	case 2: //remote
		msg.Instruction = 14;
		memcpy(&msg.Data[nDataSize],&amp,sizeof(long)); nDataSize += sizeof(long);
		msg.nDataSize = nDataSize;
		nRetVal = m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);
		break;
	}//switch

	return nRetVal;

}

int CLaser::setSingleShotData(bool bSingleShot)
{
	APP App = (APP) AfxGetApp();
	LASER_ETHERNET_MSG msg;
	int nDataSize=0;
	int nRetVal=-1;

	switch(App->m_LastSettings.nLaserLocation) {
	case 0: //no laser
		break;
	case 1: //local
		App->m_LastSettings.bLaserSingleShot = bSingleShot;
		App->SaveLastSettings();
		UpdateAllControls();
		break;
	case 2: //remote
		msg.Instruction = 15;
		memcpy(&msg.Data[nDataSize],&bSingleShot,sizeof(bool)); nDataSize += sizeof(bool);
		msg.nDataSize = nDataSize;
		nRetVal = m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);
		break;
	}//switch

	return nRetVal;
}

bool CLaser::IsThereAReadingAtMarker()
{
	float fAngle;

	m_CriticalSection.Lock();
	std::map<float,float>::iterator iter;

	iter = m_nRangeDataMap.upper_bound(m_fMarkerAngle*RAD_TO_DEG);
	fAngle = iter->first;

	m_CriticalSection.Unlock();

	if(abs(fAngle-m_fMarkerAngle)<1)
		return true;

	return false;
}

void CLaser::ResetDataSets()
{
	APP App = (APP) AfxGetApp();

	for(int nDataSet=0;nDataSet<App->m_LastSettings.nLaserAverageLines;nDataSet++) {
		for(int i=0;i<NSR;i++){
	//		m_DataSet[nDataSet][i].A1_Y = 0;
			m_DataSet[nDataSet][i].R_X = 0;
		}
	}

	ZeroMemory(m_fRange, sizeof m_fRange);

}

int CLaser::LaserOff()
{
	APP App = (APP) AfxGetApp();
	LASER_ETHERNET_MSG msg;
	int nDataSize=0;
	int nRetVal=-1;
	bool bDummy;

	switch(App->m_LastSettings.nLaserLocation) {
	case 0: //no laser
		break;
	case 1: //local
		nRetVal = m_LaserScanner.setLaserOff();
		break;
	case 2: //remote
		msg.Instruction = 17;
		memcpy(&msg.Data[nDataSize],&bDummy,sizeof(bool)); nDataSize += sizeof(bool);
		msg.nDataSize = nDataSize;
		nRetVal = m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);
		break;
	}

	return nRetVal;

}

int CLaser::LaserOn()
{
	APP App = (APP) AfxGetApp();
	LASER_ETHERNET_MSG msg;
	int nDataSize=0;
	int nRetVal=-1;
	bool bDummy;

	switch(App->m_LastSettings.nLaserLocation) {
	case 0: //no laser
		break;
	case 1: //local
		nRetVal = m_LaserScanner.setLaserOn();
		break;
	case 2: //remote
		msg.Instruction = 18;
		memcpy(&msg.Data[nDataSize],&bDummy,sizeof(bool)); nDataSize += sizeof(bool);
		msg.nDataSize = nDataSize;
		nRetVal = m_Ethernet.SendMessage(msg.nDataSize+8,msg.AllData);
		break;
	}

	return nRetVal;

}

void CLaser::FilterRange(int nCount)
{

	for(int ii=0;ii<NSR;ii++) {
		if((nCount - m_nTime[ii]) > 400) {
			m_fRange[ii]=0.0f;
		}
	}

}
