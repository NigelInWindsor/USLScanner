// Scope.cpp: implementation of the CScope class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "Scope.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScope::CScope()
{
	Zero();
	srand(GetTickCount());
	ZeroMemory(m_RfAverage, sizeof m_RfAverage);
	ZeroMemory(m_nSum, sizeof m_nSum);
	ZeroMemory(m_nAllocatedAverage, sizeof m_nAllocatedAverage);
	ZeroMemory(m_nAverageCount, sizeof m_nAverageCount);
}

CScope::~CScope()
{
	for (int nSlot = 0; nSlot < 257; nSlot++) {
		SAFE_DELETE(m_RfAverage[nSlot]);
		SAFE_DELETE(m_nSum[nSlot]);
	}
}

void CScope::Zero()
{
	m_fMaxRange = 300e-6f;
	m_fDelay = 0;
	m_fWidth = m_fMaxRange;
	m_fMainBangConstant=500e-9f;
	m_fSamplePeriod = 10e-9f;
	m_fSampleRate = 100e6f;
	m_nAcqLength = (int)(m_fWidth / m_fSamplePeriod);
	m_nMaxAcqLength = sizeof m_RFTrace;
	m_nEncodeing = SIGNED_BINARY;
	m_nAverages = 0;
	m_nBits = 8;
	m_fDCOffset = 0.0f;

	m_rgbGraticuleColor = RGB(0,255,0);
	m_rgbTextColor = RGB(255,255,0);
	m_rgbTraceColor = RGB(255,255,255);
	m_rgbBkColor = RGB(0,0,0);

	m_nLogMax=2048;
	m_nLogMin=0;
	ZeroMemory(m_nPlayRecording,sizeof m_nPlayRecording);

	for(int ii=0;ii<10;ii++) {
		m_fSGFrequency[ii] = 1.0e6f;
		m_nSGAmplitude[ii] = 100;
		m_nSGType[ii] = 0;
		m_nSGLengthMode[ii] = 0;
		m_nSGWindow[ii] = 0;
	}

}

int CScope::Save(CString FilePath)
{
	int nOffset=0;
	CUSLFile file;

	if(file.Open(FilePath,CFile::modeWrite|CFile::typeBinary|CFile::modeCreate)) {
		nOffset=Save(&file);
		file.Close();
	};
	return nOffset;
}

int CScope::Save(CFile *pFile)
{
	ULONGLONG nFileOffset = pFile->GetPosition();

	pFile->Write(&m_fMaxRange,sizeof m_fMaxRange);
	pFile->Write(&m_fDelay,sizeof m_fDelay);
	pFile->Write(&m_fWidth,sizeof m_fWidth);
	pFile->Write(&m_fSamplePeriod,sizeof m_fSamplePeriod);
	pFile->Write(&m_fSampleRate,sizeof m_fSampleRate);
	pFile->Write(&m_nAcqLength,sizeof m_nAcqLength);
	pFile->Write(&m_nDelay,sizeof m_nDelay);
	pFile->Write(&m_nWidth,sizeof m_nWidth);
	pFile->Write(&m_nTextSize,sizeof m_nTextSize);
	pFile->Write(&m_bStatistics,sizeof m_bStatistics);
	pFile->Write(&m_bBarGraph,sizeof m_bBarGraph);
	pFile->Write(&m_nTOFText,sizeof m_nTOFText);
	pFile->Write(&m_bWaterPathText,sizeof m_bWaterPathText);
	pFile->Write(&m_bDelayWidthText,sizeof m_bDelayWidthText);
	pFile->Write(&m_bGraticule,sizeof m_bGraticule);
	pFile->Write(&m_rgbTextColor,sizeof m_rgbTextColor);
	pFile->Write(&m_rgbGateColor,sizeof m_rgbGateColor);
	pFile->Write(&m_rgbGraticuleColor,sizeof m_rgbGraticuleColor);
	pFile->Write(&m_rgbTraceColor,sizeof m_rgbTraceColor);
	pFile->Write(&m_rgbBkColor,sizeof m_rgbBkColor);
	pFile->Write(&m_rgbPosPhaseColor,sizeof m_rgbPosPhaseColor);
	pFile->Write(&m_rgbNegPhaseColor,sizeof m_rgbNegPhaseColor);
	pFile->Write(&m_nGateDelayUnits,sizeof m_nGateDelayUnits);
	pFile->Write(&m_nGateWidthUnits,sizeof m_nGateWidthUnits);
	pFile->Write(&m_nTraceDelayUnits,sizeof m_nTraceDelayUnits);
	pFile->Write(&m_nTraceWidthUnits,sizeof m_nTraceWidthUnits);
	pFile->Write(&m_bTraceMode,sizeof m_bTraceMode);
	pFile->Write(&m_bDisplayGateAmp,sizeof m_bDisplayGateAmp);
	pFile->Write(&m_bLeftScale,sizeof m_bLeftScale);
	pFile->Write(&m_bRightScale,sizeof m_bRightScale);
	pFile->Write(&m_bTopScale,sizeof m_bTopScale);
	pFile->Write(&m_bBottomScale,sizeof m_bBottomScale);
	pFile->Write(&m_cDisplayDACPtsMask,sizeof m_cDisplayDACPtsMask);
	pFile->Write(&m_nRfType,sizeof m_nRfType);
	pFile->Write(&m_bTimeSlotName,sizeof m_bTimeSlotName);
	pFile->Write(&m_nCoupling,sizeof m_nCoupling);
	pFile->Write(&m_nLogMax,sizeof m_nLogMax);
	pFile->Write(&m_nLogMin,sizeof m_nLogMin);
	pFile->Write(&m_fMainBangConstant,sizeof m_fMainBangConstant);
	pFile->Write(&m_nEncodeing,sizeof m_nEncodeing);
	pFile->Write(&m_nAverages,sizeof m_nAverages);
	pFile->Write(&m_nBits,sizeof m_nBits);
	pFile->Write(&m_fDCOffset, sizeof m_fDCOffset);
	pFile->Write(&m_fVoltageRange, sizeof m_fVoltageRange);
	pFile->Write(&m_nRulerDisplayMask, sizeof m_nRulerDisplayMask);
	pFile->Write(&m_rgbDACColor, sizeof m_rgbDACColor);

	return (int)nFileOffset;
}

void CScope::Retrieve(CString FilePath)
{
	CFileException e;
	CUSLFile file;
	CString Buff;

	if(!file.Open(FilePath,CFile::modeRead|CFile::typeBinary,&e)) {
		file.DecodeFileException(&e,&Buff,FilePath);
		MessageBox(NULL,Buff,_T("Palette Error"),MB_ICONERROR);
		return;
	} else {
		Retrieve(&file);

		file.Close();
	}
}

void CScope::Retrieve(CFile *pFile)
{

	pFile->Read(&m_fMaxRange,sizeof m_fMaxRange);
	pFile->Read(&m_fDelay,sizeof m_fDelay);
	pFile->Read(&m_fWidth,sizeof m_fWidth);
	pFile->Read(&m_fSamplePeriod,sizeof m_fSamplePeriod);
	pFile->Read(&m_fSampleRate,sizeof m_fSampleRate);
	pFile->Read(&m_nAcqLength,sizeof m_nAcqLength);
	pFile->Read(&m_nDelay,sizeof m_nDelay);
	pFile->Read(&m_nWidth,sizeof m_nWidth);
	pFile->Read(&m_nTextSize,sizeof m_nTextSize);
	pFile->Read(&m_bStatistics,sizeof m_bStatistics);
	pFile->Read(&m_bBarGraph,sizeof m_bBarGraph);
	pFile->Read(&m_nTOFText,sizeof m_nTOFText);
	pFile->Read(&m_bWaterPathText,sizeof m_bWaterPathText);
	pFile->Read(&m_bDelayWidthText,sizeof m_bDelayWidthText);
	pFile->Read(&m_bGraticule,sizeof m_bGraticule);
	pFile->Read(&m_rgbTextColor,sizeof m_rgbTextColor);
	pFile->Read(&m_rgbGateColor,sizeof m_rgbGateColor);
	pFile->Read(&m_rgbGraticuleColor,sizeof m_rgbGraticuleColor);
	pFile->Read(&m_rgbTraceColor,sizeof m_rgbTraceColor);
	pFile->Read(&m_rgbBkColor,sizeof m_rgbBkColor);
	pFile->Read(&m_rgbPosPhaseColor,sizeof m_rgbPosPhaseColor);
	pFile->Read(&m_rgbNegPhaseColor,sizeof m_rgbNegPhaseColor);
	pFile->Read(&m_nGateDelayUnits,sizeof m_nGateDelayUnits);
	pFile->Read(&m_nGateWidthUnits,sizeof m_nGateWidthUnits);
	pFile->Read(&m_nTraceDelayUnits,sizeof m_nTraceDelayUnits);
	pFile->Read(&m_nTraceWidthUnits,sizeof m_nTraceWidthUnits);
	pFile->Read(&m_bTraceMode,sizeof m_bTraceMode);
	pFile->Read(&m_bDisplayGateAmp,sizeof m_bDisplayGateAmp);
	pFile->Read(&m_bLeftScale,sizeof m_bLeftScale);
	pFile->Read(&m_bRightScale,sizeof m_bRightScale);
	pFile->Read(&m_bTopScale,sizeof m_bTopScale);
	pFile->Read(&m_bBottomScale,sizeof m_bBottomScale);
	pFile->Read(&m_cDisplayDACPtsMask,sizeof m_cDisplayDACPtsMask);
	pFile->Read(&m_nRfType,sizeof m_nRfType);
	pFile->Read(&m_bTimeSlotName,sizeof m_bTimeSlotName);
	pFile->Read(&m_nCoupling,sizeof m_nCoupling);
	pFile->Read(&m_nLogMax,sizeof m_nLogMax);
	pFile->Read(&m_nLogMin,sizeof m_nLogMin);
	pFile->Read(&m_fMainBangConstant,sizeof m_fMainBangConstant);
	pFile->Read(&m_nEncodeing,sizeof m_nEncodeing);
	pFile->Read(&m_nAverages,sizeof m_nAverages);
	pFile->Read(&m_nBits,sizeof m_nBits);
	pFile->Read(&m_fDCOffset, sizeof m_fDCOffset);
	pFile->Read(&m_fVoltageRange, sizeof m_fVoltageRange);
	pFile->Read(&m_nRulerDisplayMask, sizeof m_nRulerDisplayMask);
	pFile->Read(&m_rgbDACColor, sizeof m_rgbDACColor);

	MinMax(&m_fVoltageRange, 1.0f, 5.0f); 

}

void CScope::SignalGenerator(ADC200Data *pAdc, char *pArray, int nSlot)
{
	int ii,jj,vv;
	float 	m_fFrequency = m_fSGFrequency[0];
	int m_nType = m_nSGType[0];
	int	m_nLength = 0;
	int nStart,nFinish;
	double dAmp,dTemp;

	int nArrayLength = pAdc->nAcqLength;
	float fSamplePeriod = pAdc->fSamplePeriod * 1e-9f;
	float fSamplesPerCycle = (1.0f / m_fFrequency) / fSamplePeriod;
	double dAngleInc = (double)((2.0 * 3.141592654) / fSamplesPerCycle);
	double dAngle = 0.0;
	int nCycleNumber = 0;
	int nPeakAmp = MulDiv(126, m_nSGAmplitude[0], 100);
	double dPeakAmp = (double)nPeakAmp;
	double dAmpInc;

	switch(m_nSGLengthMode[0]) {
	case 0:	nStart = 0;						//Continuous
		nFinish = nArrayLength;
		break;
	case 1: nStart = (nArrayLength / 2) - (int)(fSamplesPerCycle / 4.0f);
		nFinish = nStart + (int)(fSamplesPerCycle / 2.0f);
		break;
	case 2: nStart = (nArrayLength / 2) - (int)(fSamplesPerCycle / 2.0f);
		nFinish = nStart + (int)(fSamplesPerCycle / 1.0f);
		break;
	case 3: nStart = (nArrayLength / 2) - (int)(fSamplesPerCycle / 1.0f);
		nFinish = nStart + (int)(fSamplesPerCycle * 2.0f);
		break;
	case 4: nStart = (nArrayLength / 2) - (int)(fSamplesPerCycle * 2.5f);
		nFinish = nStart + (int)(fSamplesPerCycle * 5.0f);
		break;
	case 5: nStart = (nArrayLength / 2) - (int)(fSamplesPerCycle * 5.0f);
		nFinish = nStart + (int)(fSamplesPerCycle * 10.0f);
		break;
	}

	if(nStart<0) nStart = 0;
	if(nFinish>nArrayLength) nFinish = nArrayLength;

	for(ii=0;ii<nStart;ii++) pArray[ii]=(rand()>>14);

	switch(m_nType) {
	case SINE_WAVE:
		for(ii=nStart;ii<nFinish;ii++) {
			dAmp = dPeakAmp * sin(dAngle);
			pArray[ii] = (char)dAmp + (rand()>>14);
			dAngle += dAngleInc;
		}
		break;


	case SQUARE_WAVE:
		fSamplesPerCycle /= 2.0f;
		for(ii=nStart;ii<nFinish;ii++) {
			nCycleNumber = (int)((float)(ii-nStart) / fSamplesPerCycle);
			if(nCycleNumber&1) {
				pArray[ii]=nPeakAmp + (rand()>>14);
			} else {
				pArray[ii]=-nPeakAmp + (rand()>>14);
			}
		}
		break;

	case TRIANGLE_WAVE:
		dAmpInc = (double)m_nSGAmplitude[0] / (double)(fSamplesPerCycle / 5.0f);
		fSamplesPerCycle /= 4.0f;
		dAmp=0;
		for(ii=nStart;ii<nFinish;ii++) {
			nCycleNumber = (int)((float)(ii-nStart) / fSamplesPerCycle);
			pArray[ii]=(char)(dAmp + (double)(rand()>>14));
			switch(nCycleNumber&0x03) {
			case 0:
			case 3: dAmp += dAmpInc;
				break;
			case 1:
			case 2: dAmp -= dAmpInc;
				break;
			}
		}
		break;

	case SAWTOOTH_WAVE:
		dAmpInc = (double)m_nSGAmplitude[0] / (double)(fSamplesPerCycle / 2.5f);
		fSamplesPerCycle /= 4.0f;
		dAmp=0;
		for(ii=nStart;ii<nFinish;ii++) {
			pArray[ii]=(char)(dAmp + (double)(rand()>>14));
			dAmp+=dAmpInc;
			if(dAmp >= dPeakAmp) dAmp = -dPeakAmp; 
		}
		break;
	}

	for(ii=nFinish;ii<nArrayLength;ii++) pArray[ii]=(rand()>>14);


	int nHalfWindowLength = (nFinish - nStart) / 2;

	switch(m_nSGWindow[0]) {
	case 0:
		break;
	case 1:
		for(ii=nStart,jj=0;jj<nHalfWindowLength;ii++,jj++) {
			vv=(short)((1000 * jj) / nHalfWindowLength);
			pArray[ii] = MulDiv(pArray[ii],vv,1000);
		}
		for(ii,jj=0;ii<nFinish;ii++,jj++) {
			vv=1000 - (short)((1000 * jj) / nHalfWindowLength);
			pArray[ii] = MulDiv(pArray[ii],vv,1000);
		}
		break;
	case 2:
		for(ii=nStart,jj=0;ii<nFinish;ii++,jj++) {
			vv=1000;
			dTemp = (double)(jj-nHalfWindowLength) / (double)nHalfWindowLength;
			vv = 1000 - (int)((double)vv * (dTemp * dTemp));
			pArray[ii] = MulDiv(pArray[ii],vv,1000);
		}
		break;
	case 3:
		for(ii=nStart,jj=0;ii<nFinish;ii++,jj++) {
			vv=1000;
			dTemp = ((double)jj * 2.0 * PI) / (double)(nFinish - nStart);
			dTemp = 0.5 * (1.0 - cos(dTemp));
			vv = (int)((double)vv * dTemp);
			pArray[ii] = MulDiv(pArray[ii],vv,1000);
		}
		break;
	}

}

void CScope::CalculateLogConversionTable()
{
	int	nMin = theApp.m_Scope.m_nLogMin / 8;
	int	nMax = theApp.m_Scope.m_nLogMax / 8;
	int	ii;
	static int nDelta = 0;
	if (theApp.m_nTrackingAmp[theApp.m_nTrackingTS] < theApp.m_UtUser.m_TS[theApp.m_nTrackingTS].Gate.nAmplitude[0]) {
		nDelta--;
	}
	if (theApp.m_nTrackingAmp[theApp.m_nTrackingTS] > theApp.m_UtUser.m_TS[theApp.m_nTrackingTS].Gate.nAmplitude[0]) {
		nDelta++;
	}
	if (theApp.m_bTracking != TRUE) nDelta = 0;
	if (nDelta > 10) nDelta = 10;
	if (nDelta < -10) nDelta = -10;

	//8 bit
	if (nMax == nMin) nMax = nMin + 2;

	for (ii = 0; ii < nMin; ii++)
		cLogConversionTable[ii] = 0;
	for (ii; ii < nMax; ii++) {
		cLogConversionTable[ii] = (char)(((ii - nMin) * 127) / (nMax - nMin)) + nDelta;

	}
	for (ii; ii < 256; ii++) {
		cLogConversionTable[ii] = 127;
	}

	//8 bit squelch
	float fSMax = theApp.m_LastSettings.fSquelchMax;
	float fSMin = theApp.m_LastSettings.fSquelchMin;
	float fSAtten = theApp.m_LastSettings.fSquelchAttenuation;
	MinMax(&fSAtten, 0.0f, 100.0f);
	float fA = powf(10.0f, fSAtten / -20.0f);

	MinMax(&fSMin, 0.0f, 100.0f);
	MinMax(&fSMax, 0.0f, 100.0f);
	if (fSMax < fSMin) {
		fSMax = fSMin;
	}
	nMax = (int)((255.0f * fSMax) / 100.0f);
	nMin = (int)((255.0f * fSMin) / 100.0f);
	for (int ii = nMin;  ii < nMax; ii++) {
		int nZ = (int)((float)ii * fA);
		cLogConversionTable[ii] = nZ;
	}

	//16 bit

	nMin = theApp.m_Scope.m_nLogMin << 5;
	nMax = theApp.m_Scope.m_nLogMax << 5;
	if (nMax == nMin) nMax = nMin + 0x20;
	int nValue;

	for (ii = 0; ii < nMin; ii++)
		wLogConversionTable[ii] = 0;
	for (ii; ii < nMax; ii++) {
		nValue = (((ii - nMin) * 0x7fff) / (nMax - nMin)) + (nDelta << 6);
		if (nValue < 0) nValue = 0;
		if (nValue > 0x7fff) nValue = 0x7fff;
		wLogConversionTable[ii] = (__int16)nValue;
	}
	for (ii; ii < 0x10000; ii++) {
		wLogConversionTable[ii] = 0x7fff;
	}

}

//nNumberAverages = 0 - 5
void CScope::AllocateAverageSpace(int nSlot, int nNumberAverages)
{
	int ActualCount[6] = { 0, 2, 4, 8, 16, 32 };

	if (m_nAllocatedAverage[nSlot] - ActualCount[nNumberAverages]) {
		SAFE_DELETE(m_RfAverage[nSlot]);
		SAFE_DELETE(m_nSum[nSlot]);

		if ((m_nAllocatedAverage[nSlot] = ActualCount[nNumberAverages]) > 0) {
			m_RfAverage[nSlot] = new int[32 * MAX_ARRAY_LENGTH];
			m_nSum[nSlot] = new int[MAX_ARRAY_LENGTH];

			ZeroMemory(m_RfAverage[nSlot], sizeof(int) * 32 * MAX_ARRAY_LENGTH);
			ZeroMemory(m_nSum[nSlot], sizeof(int) * MAX_ARRAY_LENGTH);
			m_nAverageCount[nSlot] = 0;
		}
	}
}


void CScope::FillScopeArray(int nSlot, char *pSrc, int nAcqLength, bool bLogFlag, int nAverages)
{
	int *pnRfAverage;
	int *pnSum;

	MinMax(&nAverages, 0, 5);
	AllocateAverageSpace(nSlot, nAverages);

	unsigned char* pDest = (unsigned char *)m_RFTrace[nSlot];
	MinMax(&nAcqLength, 0, MAX_ARRAY_LENGTH);

	switch(nAverages) {
	case 0:
		if (bLogFlag == true) {
			for (int nS = 0; nS < nAcqLength; nS++) {
				pDest[nS] = cLogConversionTable[(char)pSrc[nS] + 128];
			}
		}
		else {
			CopyMemory(pDest, pSrc, nAcqLength);
		}
		break;
	default:
		pnRfAverage = m_RfAverage[nSlot] + (m_nAverageCount[nSlot] * MAX_ARRAY_LENGTH );
		pnSum = m_nSum[nSlot];
		for (int nS = 0; nS < nAcqLength; nS++) {
			*pnSum -= *pnRfAverage;
			if (bLogFlag == true) {
				*pnRfAverage = (int)cLogConversionTable[*pSrc + 128];
			}
			else {
				*pnRfAverage = (int)*pSrc;
			}
			*pnSum += *pnRfAverage;
			*pDest = *pnSum >> nAverages;
			pnSum++;
			pnRfAverage++;
			pDest++;
			pSrc++;
		}
		m_nAverageCount[nSlot]++;
		m_nAverageCount[nSlot] %= m_nAllocatedAverage[nSlot];
		break;
	}
}

void CScope::FillScopeArrayShort(int nSlot, __int16 *pSrc, int nAcqLength, bool bLogFlag, int nAverages)
{
	int *pnRfAverage;
	int *pnSum;

	MinMax(&nAverages, 0, 5);
	AllocateAverageSpace(nSlot, nAverages);

	unsigned __int16* pDest = (unsigned __int16 *)m_wRFTrace[nSlot];
	unsigned char* pcDest = (unsigned char *)m_RFTrace[nSlot];

	MinMax(&nAcqLength, 0, MAX_ARRAY_LENGTH);

	switch (nAverages) {
	case 0:
		if (bLogFlag == true) {
			for (int nS = 0; nS < nAcqLength; nS++) {
				pDest[nS] = wLogConversionTable[pSrc[nS] + 0x7fff];
			}
		}
		else {
			CopyMemory(pDest, pSrc, nAcqLength * sizeof (__int16));
			for (int ii = 0; ii < nAcqLength; ii++, pcDest++, pDest++) {
				*pcDest = (unsigned char) (*pDest >> 8);
			}
		}
		break;
	default:
		pnRfAverage = m_RfAverage[nSlot] + (m_nAverageCount[nSlot] * MAX_ARRAY_LENGTH);
		pnSum = m_nSum[nSlot];
		for (int nS = 0; nS < nAcqLength; nS++) {
			*pnSum -= *pnRfAverage;
			if (bLogFlag == true) {
				*pnRfAverage = (int)cLogConversionTable[*pSrc + 0x7fff];
			}
			else {
				*pnRfAverage = (int)*pSrc;
			}
			*pnSum += *pnRfAverage;
			*pDest = *pnSum >> nAverages;
			*pcDest = (unsigned char)(*pDest >> 8);
			pnSum++;
			pnRfAverage++;
			pDest++;
			pSrc++;
			pcDest++;
		}
		m_nAverageCount[nSlot]++;
		m_nAverageCount[nSlot] %= m_nAllocatedAverage[nSlot];
		break;
	}
}