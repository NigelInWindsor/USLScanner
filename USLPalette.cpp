// USLPalette.cpp: implementation of the CUSLPalette class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "USLPalette.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUSLPalette::CUSLPalette()
{
	int ii;

	memset(m_Thickness,0,sizeof m_Thickness);
	memset(m_Amplitude,0,sizeof m_Amplitude);
	for(ii=0;ii<2;ii++) {
		m_dStepSize[ii]=0.5;
		m_dMinThickness[ii]=0.0;
		m_dMaxThickness[ii]=10.0;
		m_dThicknessRange[ii]=10.0;
		m_rgbThickness[ii]=NULL;
	}
	for(ii=0;ii<8;ii++) {
		m_rgb16[ii]=NULL;
		m_rgbAmplitude[ii]=NULL;
		m_nMaxThicknessNumber[ii]=0;
		m_nMaxAmplitudeNumber[ii]=255;
		m_ddBStepSize[ii]=1.0;
		m_ddBRange[ii]=90.0;
		m_ddBTopLevel[ii]=0.0;
		m_ddBZeroLevel[ii]=0.0;
		m_nBrightness[ii]=100;
		m_nStepMode[ii]=0;
	}

	m_pPorosity = NULL;
	m_nPorosityL = 0;
	m_nPorosityCurveL = 0;
	m_pPorosityCurve = NULL;
	m_fPorositySquirterDiameter = 6.0f;
	m_fPorosityProbeDiameter = 19.0f;
	m_fPorosityFrequency = 1.0f;
	m_fPorosityRefAttenuation = 0.0f;
	m_PorosityRefTarget.Empty();


}

CUSLPalette::~CUSLPalette()
{
	Destroy();
}

void CUSLPalette::Destroy()
{
	int	ii;

	for (ii = 0; ii<8; ii++) {
		SAFE_DELETE(m_rgbAmplitude[ii]);
		SAFE_DELETE(m_rgb16[ii]);
	}
	for (ii = 0; ii<2; ii++) {
		SAFE_DELETE(m_rgbThickness[ii]);
	}

	SAFE_DELETE(m_pPorosity);
	SAFE_DELETE_ARRAY(m_pPorosityCurve);
}


void CUSLPalette::Retrieve(CString FilePath)
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

void CUSLPalette::Retrieve(CUSLFile* pFile,int nWhich)
{
	int nAmpOffset,nThickOffset,nPhaseOffset,nPorosityOffset;
	m_FileName = pFile->GetFileName();

	ULONGLONG nStartOffset = pFile->GetPosition();

	pFile->Read(&pFile->m_fVersion,sizeof pFile->m_fVersion);
	if((pFile->m_fVersion >= 4.01f)  && (pFile->m_fVersion < 20.00f)) {

		pFile->Seek(nStartOffset+0x100,CFile::begin);
		pFile->Read(&nAmpOffset,sizeof nAmpOffset);
		pFile->Read(&nThickOffset,sizeof nThickOffset);
		pFile->Read(&nPhaseOffset,sizeof nPhaseOffset);
		pFile->Read(&nPorosityOffset,sizeof nPorosityOffset);

		if((nAmpOffset>0) && (nAmpOffset<pFile->GetLength()) && (nWhich & RETRIEVE_AMPLITUDE)) {
			pFile->Seek(nAmpOffset,CFile::begin);

			pFile->Read(&m_Amplitude,sizeof m_Amplitude);
			pFile->Read(&m_nModeAmplitude,sizeof m_nModeAmplitude);
			pFile->Read(&m_nAmplitudeScaleType,sizeof m_nAmplitudeScaleType);
			pFile->Read(&m_ddBStepSize,sizeof m_ddBStepSize);
			pFile->Read(&m_ddBRange,sizeof m_ddBRange);
			pFile->Read(&m_ddBTopLevel,sizeof m_ddBTopLevel);
			pFile->Read(&m_ddBZeroLevel,sizeof m_ddBZeroLevel);
			pFile->Read(&m_nAmplitudeEntriesl,sizeof m_nAmplitudeEntriesl);
			pFile->Read(&m_nGraphType,sizeof m_nGraphType);
			pFile->Read(&m_nStepMode,sizeof m_nStepMode);
		}

		if((nThickOffset>0) && (nThickOffset<pFile->GetLength()) && (nWhich & RETRIEVE_THICKNESS)) {
			pFile->Seek(nThickOffset,CFile::begin);

			pFile->Read(&m_Thickness,sizeof m_Thickness);
			pFile->Read(&m_dStepSize,sizeof m_dStepSize);
			pFile->Read(&m_dMinThickness,sizeof m_dMinThickness);
			pFile->Read(&m_dMaxThickness,sizeof m_dMaxThickness);
			pFile->Read(&m_nModeThickness,sizeof m_nModeThickness);
			pFile->Read(&m_dThicknessRange,sizeof m_dThicknessRange);
			pFile->Read(&m_nThicknessEntriesl,sizeof m_nThicknessEntriesl);
			pFile->Read(&m_nGraphType,sizeof m_nGraphType);
		}

		if((nPhaseOffset>0) && (nPhaseOffset<pFile->GetLength()) && (nWhich & RETRIEVE_PHASE)) {
			pFile->Seek(nPhaseOffset,CFile::begin);

			pFile->Read(&m_rgbBWPassed,sizeof m_rgbBWPassed);
			pFile->Read(&m_rgbBWFailed,sizeof m_rgbBWFailed);
			pFile->Read(&m_rgbPosPhase,sizeof m_rgbPosPhase);
			pFile->Read(&m_rgbNegPhase,sizeof m_rgbNegPhase);
		}

		if((nPorosityOffset>0) && (nPorosityOffset<pFile->GetLength()) && (nWhich & RETRIEVE_POROSITY)) {
			pFile->Seek(nPorosityOffset,CFile::begin);

			pFile->Read(&m_fPorosityFrequency,sizeof m_fPorosityFrequency);
			pFile->Read(&m_fPorosityProbeDiameter,sizeof m_fPorosityProbeDiameter);
			pFile->Read(&m_fPorositySquirterDiameter,sizeof m_fPorositySquirterDiameter);

			pFile->Read(&m_nPorosityL,sizeof m_nPorosityL);
			SAFE_DELETE( m_pPorosity );
			if(m_nPorosityL>0) {
				m_pPorosity = new PorosityData[m_nPorosityL];
				for(int ii=0;ii<m_nPorosityL;ii++) {
					pFile->ReadStruct(&m_pPorosity[ii], sizeof m_pPorosity[0]);
				}
			}
			if(pFile->m_fVersion >= 4.25f) {
				pFile->Read(&m_fPorosityRefAttenuation,sizeof m_fPorosityRefAttenuation);
				CArchive ar(pFile,CArchive::load);
				ar.ReadString(m_PorosityRefTarget);
				ar.ReadString(m_PorosityStandardID);
				ar.Close();
			}
		}

	} else {

		pFile->Seek(0,CFile::begin);
		pFile->Read(&m_Amplitude,sizeof m_Amplitude);
		pFile->Read(&m_Thickness,sizeof m_Thickness);
		pFile->Read(&m_dStepSize,sizeof m_dStepSize);
		pFile->Read(&m_dMinThickness,sizeof m_dMinThickness);
		pFile->Read(&m_dMaxThickness,sizeof m_dMaxThickness);
		pFile->Read(&m_nModeThickness,sizeof m_nModeThickness);
		pFile->Read(&m_nModeAmplitude,sizeof m_nModeAmplitude);
		pFile->Read(&m_nAmplitudeScaleType,sizeof m_nAmplitudeScaleType);
		pFile->Read(&m_ddBStepSize,sizeof m_ddBStepSize);
		pFile->Read(&m_ddBRange,sizeof m_ddBRange);
		pFile->Read(&m_ddBTopLevel,sizeof m_ddBTopLevel);
		pFile->Read(&m_ddBZeroLevel,sizeof m_ddBZeroLevel);
		pFile->Read(&m_dThicknessRange,sizeof m_dThicknessRange);
		pFile->Read(&m_nThicknessEntriesl,sizeof m_nThicknessEntriesl);
		pFile->Read(&m_nAmplitudeEntriesl,sizeof m_nAmplitudeEntriesl);
		pFile->Read(&m_nGraphType,sizeof m_nGraphType);
		pFile->Read(&m_nStepMode,sizeof m_nStepMode);
		pFile->Read(&m_rgbBWPassed,sizeof m_rgbBWPassed);
		pFile->Read(&m_rgbBWFailed,sizeof m_rgbBWFailed);
		pFile->Read(&m_rgbPosPhase,sizeof m_rgbPosPhase);
		pFile->Read(&m_rgbNegPhase,sizeof m_rgbNegPhase);

		pFile->Read(&pFile->m_fVersion,sizeof pFile->m_fVersion);

		if((pFile->m_fVersion >= 4.01f) && (pFile->m_fVersion < 20.00f) && (nWhich & nWhich)) {

			pFile->Read(&m_fPorosityFrequency,sizeof m_fPorosityFrequency);
			pFile->Read(&m_fPorosityProbeDiameter,sizeof m_fPorosityProbeDiameter);
			pFile->Read(&m_fPorositySquirterDiameter,sizeof m_fPorositySquirterDiameter);

			pFile->Read(&m_nPorosityL,sizeof m_nPorosityL);
			SAFE_DELETE( m_pPorosity );
			if(m_nPorosityL>0) {
				m_pPorosity = new PorosityData[m_nPorosityL];
				for(int ii=0;ii<m_nPorosityL;ii++) {
					pFile->ReadStruct(&m_pPorosity[ii], sizeof m_pPorosity[0]);
				}
			}
		}
	}

	CalculatePalette();
	ProcessPorosityData();

}

void CUSLPalette::Save(CString FilePath)
{
	CUSLFile file;

	if(file.Open(FilePath,CFile::modeWrite|CFile::typeBinary|CFile::modeCreate)) {
		Save(&file);
		file.Close();
	};
}

void CUSLPalette::Save(CUSLFile* pFile)
{
	
	int	nAmplitudeOffset,nThicknessOffset,nPhaseOffset,nPorosityOffset;
	int nArray[500];
	CString Buff;

	m_FileName = pFile->GetFileName();

	ULONGLONG nStartOffset = pFile->GetPosition();

	ZeroMemory(nArray,sizeof nArray);
	pFile->Write(nArray,sizeof nArray);
	pFile->Seek(nStartOffset,CFile::begin);
	pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);


	pFile->Seek(nStartOffset+0x200,CFile::begin);
	nAmplitudeOffset = (int)pFile->GetPosition();
	pFile->Write(&m_Amplitude,sizeof m_Amplitude);
	pFile->Write(&m_nModeAmplitude,sizeof m_nModeAmplitude);
	pFile->Write(&m_nAmplitudeScaleType,sizeof m_nAmplitudeScaleType);
	pFile->Write(&m_ddBStepSize,sizeof m_ddBStepSize);
	pFile->Write(&m_ddBRange,sizeof m_ddBRange);
	pFile->Write(&m_ddBTopLevel,sizeof m_ddBTopLevel);
	pFile->Write(&m_ddBZeroLevel,sizeof m_ddBZeroLevel);
	pFile->Write(&m_nAmplitudeEntriesl,sizeof m_nAmplitudeEntriesl);
	pFile->Write(&m_nGraphType,sizeof m_nGraphType);
	pFile->Write(&m_nStepMode,sizeof m_nStepMode);

	pFile->Seek(nThicknessOffset=(int)(pFile->GetPosition() + 0x100),CFile::begin);
	pFile->Write(&m_Thickness,sizeof m_Thickness);
	pFile->Write(&m_dStepSize,sizeof m_dStepSize);
	pFile->Write(&m_dMinThickness,sizeof m_dMinThickness);
	pFile->Write(&m_dMaxThickness,sizeof m_dMaxThickness);
	pFile->Write(&m_nModeThickness,sizeof m_nModeThickness);
	pFile->Write(&m_dThicknessRange,sizeof m_dThicknessRange);
	pFile->Write(&m_nThicknessEntriesl,sizeof m_nThicknessEntriesl);
	pFile->Write(&m_nGraphType,sizeof m_nGraphType);

	pFile->Seek(nPhaseOffset=(int)(pFile->GetPosition() + 0x100),CFile::begin);
	pFile->Write(&m_rgbBWPassed,sizeof m_rgbBWPassed);
	pFile->Write(&m_rgbBWFailed,sizeof m_rgbBWFailed);
	pFile->Write(&m_rgbPosPhase,sizeof m_rgbPosPhase);
	pFile->Write(&m_rgbNegPhase,sizeof m_rgbNegPhase);

	pFile->Seek(nPorosityOffset=(int)(pFile->GetPosition() + 0x100),CFile::begin);
	pFile->Write(&m_fPorosityFrequency,sizeof m_fPorosityFrequency);
	pFile->Write(&m_fPorosityProbeDiameter,sizeof m_fPorosityProbeDiameter);
	pFile->Write(&m_fPorositySquirterDiameter,sizeof m_fPorositySquirterDiameter);
	pFile->Write(&m_nPorosityL,sizeof m_nPorosityL);
	for(int ii=0;ii<m_nPorosityL;ii++) {
		pFile->WriteStruct(&m_pPorosity[ii],m_pPorosity[ii].nStructSize = sizeof PorosityData);
	}
	pFile->Write(&m_fPorosityRefAttenuation,sizeof m_fPorosityRefAttenuation);
	CArchive* pAr = new CArchive(pFile,CArchive::store);
	pAr->WriteString(m_PorosityRefTarget + _T("\r\n"));
	pAr->WriteString(m_PorosityStandardID + _T("\r\n"));
	pAr->Close();
	SAFE_DELETE(pAr);

	int nFinishOffset = (int)pFile->GetPosition();

	pFile->Seek(nStartOffset+0x100,CFile::begin);
	pFile->Write(&nAmplitudeOffset,sizeof nAmplitudeOffset);
	pFile->Write(&nThicknessOffset,sizeof nThicknessOffset);
	pFile->Write(&nPhaseOffset,sizeof nPhaseOffset);
	pFile->Write(&nPorosityOffset,sizeof nPorosityOffset);

	pFile->Seek(nFinishOffset,CFile::begin);
}


void CUSLPalette::Zero()
{
	int ii;

	memset(m_Thickness,0,sizeof m_Thickness);
	memset(m_Amplitude,0,sizeof m_Amplitude);
	for(ii=0;ii<2;ii++) {
		m_dStepSize[ii]=0.5;
		m_dMinThickness[ii]=0.0;
		m_dMaxThickness[ii]=10.0;
		m_dThicknessRange[ii]=10.0;
		delete m_rgbThickness[ii];
		m_rgbThickness[ii]=NULL;
	}
	for(ii=0;ii<8;ii++) {
		SAFE_DELETE( m_rgbAmplitude[ii] );
		SAFE_DELETE( m_rgb16[ii] );
		m_nMaxThicknessNumber[ii]=0;
		m_nMaxAmplitudeNumber[ii]=255;
		m_ddBStepSize[ii]=1.0;
		m_ddBRange[ii]=90.0;
		m_ddBTopLevel[ii]=0.0;
		m_ddBZeroLevel[ii]=0.0;
		m_nBrightness[ii]=100;
	}

	m_nPorosityL = 0;
	SAFE_DELETE( m_pPorosity );
	m_fPorositySquirterDiameter = 6.0f;
	m_fPorosityProbeDiameter = 19.0f;
	m_fPorosityFrequency = 1.0f;
}



void CUSLPalette::CalculatePalette()
{
	int	nOldWhichAmp = m_nWhichAmp;

	for(m_nWhichAmp=0;m_nWhichAmp<8;m_nWhichAmp++) {
		CalculateAmplitudePalette();
		CalculateExtPalette();
	}
	m_nWhichAmp=nOldWhichAmp;
	CalculateThicknessPalette();

}


void CUSLPalette::CalculateThicknessPalette()
{
	int	nMin,nMax;
	int	nS0,nS1;
	int	ii,jj,kk;
	int	nRed,nGreen,nBlue;
	int	nStep,nColor;
	int	nRange;

	if(m_nThicknessEntriesl[m_nWhichThick] < 1) m_nThicknessEntriesl[m_nWhichThick]=1;
	if(m_nThicknessEntriesl[m_nWhichThick] > 100) m_nThicknessEntriesl[m_nWhichThick] = 100;
	SAFE_DELETE( m_rgbThickness[m_nWhichThick] );

	if(m_dThicknessRange[m_nWhichThick] < 0.1) m_dThicknessRange[m_nWhichThick]= 2.0;
	if(m_dMinThickness[m_nWhichThick] < 0.0) m_dMinThickness[m_nWhichThick]=0.0;

	nRange=(int)(m_dThicknessRange[m_nWhichThick] * 100.0);
	m_dMaxThickness[m_nWhichThick]=m_dMinThickness[m_nWhichThick]+m_dThicknessRange[m_nWhichThick];
	m_nMinThicknessNumber[m_nWhichThick]=(int)(m_dMinThickness[m_nWhichThick] * 100.0);
	m_nMaxThicknessNumber[m_nWhichThick]=(int)(m_dMaxThickness[m_nWhichThick] * 100.0);
	m_rgbThickness[m_nWhichThick] = new COLORREF[m_nMaxThicknessNumber[m_nWhichThick]+1];
	COLORREF* rgbTemp = new COLORREF[m_nMaxThicknessNumber[m_nWhichThick]+1];

	nMin=(int)(m_dMinThickness[m_nWhichThick] * 100.0);
	nMax=(int)(m_dMaxThickness[m_nWhichThick] * 100.0);

	//Linear Section
	for(ii=m_nThicknessEntriesl[m_nWhichThick]-1;ii>=0;ii--) {
		if(m_Thickness[m_nWhichThick][ii].bSet==TRUE) {
			nS0=ii;
			nS1=-1;
			for(kk=ii-1;(kk>=0)&&(nS1==-1);kk--) {
				if(m_Thickness[m_nWhichThick][kk].bSet==TRUE) {
					nS1=kk;
					nMin=MulDiv(m_nThicknessEntriesl[m_nWhichThick]-1-nS0,m_nMaxThicknessNumber[m_nWhichThick],m_nThicknessEntriesl[m_nWhichThick]-1);
					nMax=MulDiv(m_nThicknessEntriesl[m_nWhichThick]-1-nS1,m_nMaxThicknessNumber[m_nWhichThick],m_nThicknessEntriesl[m_nWhichThick]-1);

					for(jj=0;jj<(nMax-nMin);jj++) {
						nRed=MulDiv(jj,GetRValue(m_Thickness[m_nWhichThick][nS1].rgbValue)-GetRValue(m_Thickness[m_nWhichThick][nS0].rgbValue),(nMax-nMin))+GetRValue(m_Thickness[m_nWhichThick][nS0].rgbValue);
						nGreen=MulDiv(jj,GetGValue(m_Thickness[m_nWhichThick][nS1].rgbValue)-GetGValue(m_Thickness[m_nWhichThick][nS0].rgbValue),(nMax-nMin))+GetGValue(m_Thickness[m_nWhichThick][nS0].rgbValue);
						nBlue=MulDiv(jj,GetBValue(m_Thickness[m_nWhichThick][nS1].rgbValue)-GetBValue(m_Thickness[m_nWhichThick][nS0].rgbValue),(nMax-nMin))+GetBValue(m_Thickness[m_nWhichThick][nS0].rgbValue);

						rgbTemp[jj+nMin]=(nBlue<<16) | (nGreen<<8) | nRed;
					};
				};
			};
		};
	};


	//Squash it into the range
	for(ii=0;ii<m_nMinThicknessNumber[m_nWhichThick];ii++) {
		m_rgbThickness[m_nWhichThick][ii]=rgbTemp[0];
	}
	for(ii=0;ii<nRange;ii++) {
		jj=MulDiv(ii,m_nMaxThicknessNumber[m_nWhichThick],nRange-1);
		kk=(ii+m_nMinThicknessNumber[m_nWhichThick]);
		if(kk>=m_nMaxThicknessNumber[m_nWhichThick]) kk=m_nMaxThicknessNumber[m_nWhichThick];
		m_rgbThickness[m_nWhichThick][kk]=rgbTemp[jj];
	};
	for(ii=m_nMinThicknessNumber[m_nWhichThick]+nRange;ii<m_nMaxThicknessNumber[m_nWhichThick];ii++) {
		m_rgbThickness[m_nWhichThick][ii]=rgbTemp[m_nMaxThicknessNumber[m_nWhichThick]-1];
	}





	//Block Section
	if(m_nModeThickness[m_nWhichThick]==1) {
		nStep=(int)(m_dStepSize[m_nWhichThick]*100);
		if(nStep<=0) goto safe_return;
		for(jj=0;jj<m_nMaxThicknessNumber[m_nWhichThick];jj++) {
			nColor=(jj/nStep)*nStep;
			m_rgbThickness[m_nWhichThick][jj]=m_rgbThickness[m_nWhichThick][nColor];
		};
	};

safe_return:
	delete rgbTemp;
}


void CUSLPalette::CalculateAmplitudePalette()
{
	
	int	nn,nDest0,nDest1,nSep,mm;
	int	nRed,nGreen,nBlue;
	int	nTopOffset,nTopThreshold,nBottomThreshold,nSpanThreshold;
	float fTopThreshold,fBottomThreshold,fSpanThreshold;
	float fTop,fBottom,fSpan;

	SAFE_DELETE( m_rgbAmplitude[m_nWhichAmp]);
	m_rgbAmplitude[m_nWhichAmp] = new COLORREF[256];

	float fRange = 100.0;
	if(m_nAmplitudeScaleType[m_nWhichAmp] == 1) fRange = 32.0;
	if(m_nAmplitudeScaleType[m_nWhichAmp] == 2) fRange = (float)theApp.m_LastSettings.nLogRange;

	if(m_ddBStepSize[m_nWhichAmp] <= 0.1) return;
	if(m_nAmplitudeEntriesl[m_nWhichAmp]<1) m_nAmplitudeEntriesl[m_nWhichAmp] = 1;
	if(m_nAmplitudeEntriesl[m_nWhichAmp]>100) m_nAmplitudeEntriesl[m_nWhichAmp] = 100;
	int nEntries = m_nAmplitudeEntriesl[m_nWhichAmp];

	
	for(nDest0=255;nDest0>128;nDest0--) {
		nRed=GetRValue(m_Amplitude[m_nWhichAmp][0].rgbValue);
		nGreen=GetGValue(m_Amplitude[m_nWhichAmp][0].rgbValue);
		nBlue=GetBValue(m_Amplitude[m_nWhichAmp][0].rgbValue);
		m_rgbAmplitude[m_nWhichAmp][nDest0]=(nBlue<<16) | (nGreen<<8) | nRed;
	}

	//Threshold
	switch(m_nAmplitudeScaleType[m_nWhichAmp]) {
	case 0:
	case 1:
		fTopThreshold = (100.0f-theApp.m_LastSettings.fThresholdTop) * 128.0f / 100.0f;					//yes it is 128
		fSpanThreshold = (1.0f - (float)(pow(10.0,((double)theApp.m_LastSettings.fThresholdSpan / -20.0)))) * fTopThreshold;
		fTopThreshold += 128.0f;
		fBottomThreshold = fTopThreshold - fSpanThreshold;
		break;
	case 2:
		fTopThreshold = 128.0f + ((100.0f-theApp.m_LastSettings.fThresholdTop) * 128.0f / 100.0f);
		fSpanThreshold = (float)(theApp.m_LastSettings.fThresholdSpan * 128.0f / 100.0f)+1.0f;
		fBottomThreshold = fTopThreshold - fSpanThreshold;
		break;
	}
	if(fTopThreshold>256.0f) fTopThreshold = 256.0f;
	if(fBottomThreshold<128.0f) fBottomThreshold = 128.0f;
	if(fSpanThreshold>127.0f) fSpanThreshold = 127.0f;

	switch(theApp.m_LastSettings.nThresholdMode) {
	default:
		fTop = 256.0f;
		fBottom = 128.0f;
		fSpan = 127.0f;
		break;
	case 2:
		fTop = fTopThreshold;
		fBottom = fBottomThreshold;
		fSpan = fSpanThreshold;
		break;
	};


	//calculate the spread of entries
	for(nn=0;nn<nEntries;nn++) {
		switch(m_nStepMode[m_nWhichAmp]) {
		case 0:
			switch(m_nAmplitudeScaleType[m_nWhichAmp]) {
			case 0:
			case 2:
				nTopOffset = (int)(((float)m_ddBTopLevel[m_nWhichAmp] * fSpan /fRange) + fBottom);
				nDest0 = nTopOffset-(int)(m_ddBStepSize[m_nWhichAmp]*(double)nn*fSpan/fRange);
				nDest1 = nTopOffset-(int)(m_ddBStepSize[m_nWhichAmp]*(double)(nn+1)*fSpan/fRange);
				break;
			case 1:
				nDest0 = (int)((pow(10,((m_ddBTopLevel[m_nWhichAmp]/2.0)+(m_ddBStepSize[m_nWhichAmp]*(double)nn))/-20.0)*fSpan) +fBottom);
				nDest1 = (int)((pow(10,((m_ddBTopLevel[m_nWhichAmp]/2.0)+(m_ddBStepSize[m_nWhichAmp]*(double)(nn+1)))/-20.0)*fSpan) +fBottom);
				break;
			}
			break;
		case 1:
		case 2:
			nDest0=(int)(((m_Amplitude[m_nWhichAmp][nn].fValue * fSpan) / fRange) + fBottom);
			if((nn+1) < nEntries) {
				nDest1=(int)(((m_Amplitude[m_nWhichAmp][nn+1].fValue * fSpan) / fRange) + fBottom);
			} else {
				nDest1 = (int)fBottom;
			}
			break;
		}

		MinMax(&nDest0,128,255);
		MinMax(&nDest1,128,255);
		nSep=nDest0-nDest1;
		mm=nn+1;
		if(mm>=nEntries) mm=nEntries-1;
		switch(m_nModeAmplitude[m_nWhichAmp]) {
		case 0: // Blended
			for(nDest0;nDest0>nDest1;nDest0--) {
				nRed=MulDiv(GetRValue(m_Amplitude[m_nWhichAmp][nn].rgbValue)-GetRValue(m_Amplitude[m_nWhichAmp][mm].rgbValue),nDest0-nDest1,nSep)+GetRValue(m_Amplitude[m_nWhichAmp][mm].rgbValue);
				nGreen=MulDiv(GetGValue(m_Amplitude[m_nWhichAmp][nn].rgbValue)-GetGValue(m_Amplitude[m_nWhichAmp][mm].rgbValue),nDest0-nDest1,nSep)+GetGValue(m_Amplitude[m_nWhichAmp][mm].rgbValue);
				nBlue=MulDiv(GetBValue(m_Amplitude[m_nWhichAmp][nn].rgbValue)-GetBValue(m_Amplitude[m_nWhichAmp][mm].rgbValue),nDest0-nDest1,nSep)+GetBValue(m_Amplitude[m_nWhichAmp][mm].rgbValue);
				m_rgbAmplitude[m_nWhichAmp][nDest0]=(nBlue<<16) | (nGreen<<8) | nRed;
			}
			break;
		case 1:	//Block
			for(nDest0;nDest0>=nDest1;nDest0--) {
				nRed=GetRValue(m_Amplitude[m_nWhichAmp][nn].rgbValue);
				nGreen=GetGValue(m_Amplitude[m_nWhichAmp][nn].rgbValue);
				nBlue=GetBValue(m_Amplitude[m_nWhichAmp][nn].rgbValue);
				m_rgbAmplitude[m_nWhichAmp][nDest0]=(nBlue<<16) | (nGreen<<8) | nRed;
			}
			break;
		}
	}

	for(nDest0;nDest0>=128;nDest0--) {
		nRed=GetRValue(m_Amplitude[m_nWhichAmp][nn-1].rgbValue);
		nGreen=GetGValue(m_Amplitude[m_nWhichAmp][nn-1].rgbValue);
		nBlue=GetBValue(m_Amplitude[m_nWhichAmp][nn-1].rgbValue);
		m_rgbAmplitude[m_nWhichAmp][nDest0]=(nBlue<<16) | (nGreen<<8) | nRed;
	}

	//Threshold
	switch(theApp.m_LastSettings.nThresholdMode) {
	case 1:
	case 2:
		nTopThreshold = (int)fTopThreshold;
		nBottomThreshold = (int)fBottomThreshold;
		nSpanThreshold = (int)fSpanThreshold;
		MinMax(&nTopThreshold,0,256);
		MinMax(&nBottomThreshold,0,255);
		MinMax(&nSpanThreshold,0,255);
		for(nn=nTopThreshold;nn<256;nn++) m_rgbAmplitude[m_nWhichAmp][nn] = theApp.m_LastSettings.rgbThresholdTop;
		for(nn=128;nn<=nBottomThreshold;nn++) m_rgbAmplitude[m_nWhichAmp][nn] = theApp.m_LastSettings.rgbThresholdBottom;
		break;
	}
}

void CUSLPalette::CalculateExtPalette()
{
	try{
	
	int	nn,nDest0,nDest1,nSep,mm;
	int	nRed,nGreen,nBlue;
	int	nTopOffset,nTopThreshold,nBottomThreshold,nSpanThreshold;
	float fTopThreshold,fBottomThreshold,fSpanThreshold;
	float fTop,fBottom,fSpan;

	SAFE_DELETE( m_rgb16[m_nWhichAmp ]);
	m_rgb16[m_nWhichAmp] = new COLORREF[0x10000];

	float fRange = 100.0;
	if(m_nAmplitudeScaleType[m_nWhichAmp] == 1) fRange = 32.0;
	if(m_nAmplitudeScaleType[m_nWhichAmp] == 2) fRange = (float)theApp.m_LastSettings.nLogRange;

	if(m_ddBStepSize[m_nWhichAmp] <= 0.1) return;
	if(m_nAmplitudeEntriesl[m_nWhichAmp]<1) m_nAmplitudeEntriesl[m_nWhichAmp] = 1;
	if(m_nAmplitudeEntriesl[m_nWhichAmp]>100) m_nAmplitudeEntriesl[m_nWhichAmp] = 100;
	int nEntries = m_nAmplitudeEntriesl[m_nWhichAmp];

	
	for(nDest0=0x7fff;nDest0>=0;nDest0--) {
		nRed=GetRValue(m_Amplitude[m_nWhichAmp][0].rgbValue);
		nGreen=GetGValue(m_Amplitude[m_nWhichAmp][0].rgbValue);
		nBlue=GetBValue(m_Amplitude[m_nWhichAmp][0].rgbValue);
		m_rgb16[m_nWhichAmp][nDest0]=(nBlue<<16) | (nGreen<<8) | nRed;
	}

	//Threshold
	switch(m_nAmplitudeScaleType[m_nWhichAmp]) {
	case 0:
	case 1:
		fTopThreshold = (100.0f-theApp.m_LastSettings.fThresholdTop) * 65535.0f / 100.0f;					//yes it is 128
		fSpanThreshold = (1.0f - (float)(pow(10.0,((double)theApp.m_LastSettings.fThresholdSpan / -20.0)))) * fTopThreshold;
		fBottomThreshold = fTopThreshold - fSpanThreshold;
		break;
	case 2:
		fTopThreshold = (100.0f-theApp.m_LastSettings.fThresholdTop) * 65535.0f / 100.0f;
		fSpanThreshold = (float)(theApp.m_LastSettings.fThresholdSpan * 65535.0f / 100.0f)+1.0f;
		fBottomThreshold = fTopThreshold - fSpanThreshold;
		break;
	}
	if(fTopThreshold>65535.0f) fTopThreshold = 65535.0f;
	if(fBottomThreshold<0.0f) fBottomThreshold = 0.0f;
	if(fSpanThreshold>65535.0f) fSpanThreshold = 65535.0f;

	switch(theApp.m_LastSettings.nThresholdMode) {
	default:
		fTop = 65535.0f;
		fBottom = 0.0f;
		fSpan = 65535.0f;
		break;
	case 2:
		fTop = fTopThreshold;
		fBottom = fBottomThreshold;
		fSpan = fSpanThreshold;
		break;
	};


	//calculate the spread of entries
	for(nn=0;nn<nEntries;nn++) {
		switch(m_nStepMode[m_nWhichAmp]) {
		case 0:
			switch(m_nAmplitudeScaleType[m_nWhichAmp]) {
			case 0:
			case 2:
				nTopOffset = (int)((m_ddBTopLevel[m_nWhichAmp] * fSpan /fRange) + fBottom);
				nDest0 = nTopOffset-(int)(m_ddBStepSize[m_nWhichAmp]*(double)nn*fSpan/fRange);
				nDest1 = nTopOffset-(int)(m_ddBStepSize[m_nWhichAmp]*(double)(nn+1)*fSpan/fRange);
				break;
			case 1:
				nDest0 = (int)((powf(10.0f,(float)((m_ddBTopLevel[m_nWhichAmp]/2.0)+(m_ddBStepSize[m_nWhichAmp]*(double)nn))/-20.0f)*fSpan) +fBottom);
				nDest1 = (int)((powf(10.0f,(float)((m_ddBTopLevel[m_nWhichAmp]/2.0)+(m_ddBStepSize[m_nWhichAmp]*(double)(nn+1)))/-20.0f)*fSpan) +fBottom);
				break;
			}
			break;
		case 1:
		case 2:
			nDest0=(int)(((m_Amplitude[m_nWhichAmp][nn].fValue * fSpan) / fRange) + fBottom);
			if((nn+1) < nEntries) {
				nDest1=(int)(((m_Amplitude[m_nWhichAmp][nn+1].fValue * fSpan) / fRange) + fBottom);
			} else {
				nDest1 = (int)fBottom;
			}
			break;
		}

		MinMax(&nDest0,0,65535);
		MinMax(&nDest1,0,65535);
		nSep=nDest0-nDest1;
		mm=nn+1;
		if(mm>=nEntries) mm=nEntries-1;
		switch(m_nModeAmplitude[m_nWhichAmp]) {
		case 0: // Blended
			for(nDest0;nDest0>nDest1;nDest0--) {
				nRed=MulDiv(GetRValue(m_Amplitude[m_nWhichAmp][nn].rgbValue)-GetRValue(m_Amplitude[m_nWhichAmp][mm].rgbValue),nDest0-nDest1,nSep)+GetRValue(m_Amplitude[m_nWhichAmp][mm].rgbValue);
				nGreen=MulDiv(GetGValue(m_Amplitude[m_nWhichAmp][nn].rgbValue)-GetGValue(m_Amplitude[m_nWhichAmp][mm].rgbValue),nDest0-nDest1,nSep)+GetGValue(m_Amplitude[m_nWhichAmp][mm].rgbValue);
				nBlue=MulDiv(GetBValue(m_Amplitude[m_nWhichAmp][nn].rgbValue)-GetBValue(m_Amplitude[m_nWhichAmp][mm].rgbValue),nDest0-nDest1,nSep)+GetBValue(m_Amplitude[m_nWhichAmp][mm].rgbValue);
				m_rgb16[m_nWhichAmp][nDest0]=(nBlue<<16) | (nGreen<<8) | nRed;
			}
			break;
		case 1:	//Block
			for(nDest0;nDest0>=nDest1;nDest0--) {
				nRed=GetRValue(m_Amplitude[m_nWhichAmp][nn].rgbValue);
				nGreen=GetGValue(m_Amplitude[m_nWhichAmp][nn].rgbValue);
				nBlue=GetBValue(m_Amplitude[m_nWhichAmp][nn].rgbValue);
				m_rgb16[m_nWhichAmp][nDest0]=(nBlue<<16) | (nGreen<<8) | nRed;
			}
			break;
		}
	}

	for(nDest0;nDest0>=0;nDest0--) {
		nRed=GetRValue(m_Amplitude[m_nWhichAmp][nn-1].rgbValue);
		nGreen=GetGValue(m_Amplitude[m_nWhichAmp][nn-1].rgbValue);
		nBlue=GetBValue(m_Amplitude[m_nWhichAmp][nn-1].rgbValue);
		m_rgb16[m_nWhichAmp][nDest0]=(nBlue<<16) | (nGreen<<8) | nRed;
	}

	//Threshold
	switch(theApp.m_LastSettings.nThresholdMode) {
	case 1:
	case 2:
		nTopThreshold = (int)fTopThreshold;
		nBottomThreshold = (int)fBottomThreshold;
		nSpanThreshold = (int)fSpanThreshold;
		MinMax(&nTopThreshold,0,65535);
		MinMax(&nBottomThreshold,0,65535);
		MinMax(&nSpanThreshold,0,65535);
		for(nn=nTopThreshold;nn<65535;nn++) m_rgb16[m_nWhichAmp][nn] = theApp.m_LastSettings.rgbThresholdTop;
		for(nn=0;nn<=nBottomThreshold;nn++) m_rgb16[m_nWhichAmp][nn] = theApp.m_LastSettings.rgbThresholdBottom;
		break;
	}
	}catch(...){}
}

void CUSLPalette::CalculateLogPalette()
{

	int	nMin,nMax;
	int	nS0,nS1;
	int	ii,jj,kk;
	int	nRed,nGreen,nBlue;
	double	dStep;
	COLORREF	rgbTemp[256];
	int nEntries;



	if(m_ddBStepSize[m_nWhichAmp] <= 0.1) return;
	if(m_nAmplitudeEntriesl[m_nWhichAmp]<1) m_nAmplitudeEntriesl[m_nWhichAmp] = 1;
	if(m_nAmplitudeEntriesl[m_nWhichAmp]>100) m_nAmplitudeEntriesl[m_nWhichAmp] = 100;
	int	nLevels = (int)(m_nAmplitudeEntriesl[m_nWhichAmp]*m_ddBStepSize[m_nWhichAmp]*127/100.0);
	if(nLevels<=0) nLevels=1;
	if(nLevels>127) nLevels=127;

	nEntries = m_nAmplitudeEntriesl[m_nWhichAmp]-1;

	//Linear Section
	for(ii=nEntries;ii>=0;ii--) {
		if(m_Amplitude[m_nWhichAmp][ii].bSet==TRUE) {
			nS0=ii;
			nS1=-1;
			for(kk=ii-1;(kk>=0)&&(nS1==-1);kk--) {
				if(m_Amplitude[m_nWhichAmp][kk].bSet==TRUE) {
					nS1=kk;
					nMin=MulDiv(nEntries-nS0,nLevels,nEntries);
					nMax=MulDiv(nEntries-nS1,nLevels,nEntries);

					for(jj=0;jj<=(nMax-nMin);jj++) {
						nRed=MulDiv(jj,GetRValue(m_Amplitude[m_nWhichAmp][nS1].rgbValue)-GetRValue(m_Amplitude[m_nWhichAmp][nS0].rgbValue),(nMax-nMin))+GetRValue(m_Amplitude[m_nWhichAmp][nS0].rgbValue);
						nGreen=MulDiv(jj,GetGValue(m_Amplitude[m_nWhichAmp][nS1].rgbValue)-GetGValue(m_Amplitude[m_nWhichAmp][nS0].rgbValue),(nMax-nMin))+GetGValue(m_Amplitude[m_nWhichAmp][nS0].rgbValue);
						nBlue=MulDiv(jj,GetBValue(m_Amplitude[m_nWhichAmp][nS1].rgbValue)-GetBValue(m_Amplitude[m_nWhichAmp][nS0].rgbValue),(nMax-nMin))+GetBValue(m_Amplitude[m_nWhichAmp][nS0].rgbValue);

						rgbTemp[jj+nMin]=(nBlue<<16) | (nGreen<<8) | nRed;
					};
				};
			};
		};
	};
	//Block Mode
	if((m_nModeAmplitude[m_nWhichAmp]==1) && (m_ddBStepSize[m_nWhichAmp]>0.0)) {
		for(dStep=m_ddBStepSize[m_nWhichAmp],ii=0,jj=nLevels;(dStep<100)&&(jj>=0);dStep+=m_ddBStepSize[m_nWhichAmp],ii++) {
			kk=(int)(nLevels-((dStep*nLevels)/m_ddBRange[m_nWhichAmp]));
			for(jj;(jj>kk)&&(jj>=0);jj--) {
				if(m_Amplitude[m_nWhichAmp][ii].bSet==TRUE) {
					rgbTemp[jj]=m_Amplitude[m_nWhichAmp][ii].rgbValue;
				} else {
					rgbTemp[jj]=RGB(0,0,0);
				};
			};
		};
	};
	//Pan it
	m_nMaxAmplitudeNumber[m_nWhichAmp]=(int)((127.0*(90+m_ddBTopLevel[m_nWhichAmp]-m_ddBZeroLevel[m_nWhichAmp]))/90.0)+128;
	if(m_nMaxAmplitudeNumber[m_nWhichAmp]<129) m_nMaxAmplitudeNumber[m_nWhichAmp]=129;
	if(m_nMaxAmplitudeNumber[m_nWhichAmp]>255) m_nMaxAmplitudeNumber[m_nWhichAmp]=255;
	for(jj=255;jj>m_nMaxAmplitudeNumber[m_nWhichAmp];jj--) {
		m_rgb16[m_nWhichAmp][jj]=rgbTemp[nLevels];
	};
	for(jj,ii=nLevels;(jj>127)&&(ii>=0);jj--,ii--) {
		m_rgb16[m_nWhichAmp][jj]=rgbTemp[ii];
	}

	for(jj;jj>127;jj--) {
		m_rgb16[m_nWhichAmp][jj]=rgbTemp[0];
	};

	//Block Section
/*
	if(m_nModeAmplitude==1) {
		switch(m_nAmplitudeScaleType) {
		case 0:
			if(m_ddBStepSize<=0.0) return;
			for(dStep=-m_ddBStepSize,jj=255;(dStep>-100)&&(jj>127);dStep-=m_ddBStepSize) {
				kk=(int)(pow(10,(dStep/20))*127) +128;
				nColor=jj;
				for(jj;(jj>kk)&&(jj>127);jj--) {
					m_rgb16[jj]=m_rgb16[nColor];
				};
			};
			break;
		case 1:
			if(m_ddBStepSize<=0.0) return;
			for(dStep=m_ddBStepSize,jj=255;(dStep<100)&&(jj>127);dStep+=m_ddBStepSize) {
				kk=(int)(255-((dStep*127)/90.0));
				nColor=jj;
				for(jj;(jj>kk)&&(jj>127);jj--) {
					m_rgb16[jj]=m_rgb16[nColor];
				};
			};
			break;
		};
	}
*/
	for(ii=0;ii<256;ii++) {
		nRed = MulDiv(GetRValue(m_rgb16[m_nWhichAmp][ii]),m_nBrightness[m_nWhichAmp],100);
		if(nRed>255) nRed=255;
		if(nRed<0) nRed=0;
		nBlue = MulDiv(GetBValue(m_rgb16[m_nWhichAmp][ii]),m_nBrightness[m_nWhichAmp],100);
		if(nBlue>255) nBlue=255;
		if(nBlue<0) nBlue=0;
		nGreen = MulDiv(GetGValue(m_rgb16[m_nWhichAmp][ii]),m_nBrightness[m_nWhichAmp],100);
		if(nGreen>255) nGreen=255;
		if(nGreen<0) nGreen=0;

		m_rgb16[m_nWhichAmp][ii]=(nBlue<<16) | (nGreen<<8) | nRed;

	}
}

void CUSLPalette::GreyScale()
{

	for(int ii=0;ii<256;ii++) {
		m_rgbAmplitude[m_nWhichAmp][ii]=RGB(ii,ii,ii);
	};
}


bool CUSLPalette::CopyAmpltudePaletteTo(int nSrc,int nDest)
{
	if(nSrc==nDest) return FALSE;

	for(int ii=0;ii<256;ii++) {
		if(m_rgbAmplitude[nDest]==NULL)	m_rgbAmplitude[nDest] = new COLORREF[256];
		m_rgbAmplitude[nDest][ii]=m_rgbAmplitude[nSrc][ii];
	}
	CopyMemory(m_Amplitude[nDest],m_Amplitude[nSrc],sizeof m_Amplitude[nDest]);
	m_nBrightness[nDest]=m_nBrightness[nSrc];
	m_ddBZeroLevel[nDest]=m_ddBZeroLevel[nSrc];
	m_ddBTopLevel[nDest]=m_ddBTopLevel[nSrc];
	m_ddBRange[nDest]=m_ddBRange[nSrc];
	m_ddBStepSize[nDest]=m_ddBStepSize[nSrc];
	m_nAmplitudeScaleType[nDest]=m_nAmplitudeScaleType[nSrc];
	m_nMaxAmplitudeNumber[nDest]=m_nMaxAmplitudeNumber[nSrc];
	m_nModeAmplitude[nDest]=m_nModeAmplitude[nSrc];
	m_nAmplitudeEntriesl[nDest]=m_nAmplitudeEntriesl[nSrc];
	m_nGraphType[nDest]=m_nGraphType[nSrc];

	return TRUE;
}


void CUSLPalette::MinMax(int *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;
}

void CUSLPalette::SamPaletteExtraction(int *Array)
{
	int	nRed,nGreen,nBlue;

	m_nAmplitudeEntriesl[m_nWhichAmp] = 16;
	for(int ii=0;ii<16;ii++) {
		nRed = (Array[ii] & 0x3f) << 2;
		nGreen = (Array[ii] & 0x3f00) >> 6;
		nBlue = (Array[ii] & 0x3f0000) >> 14;
		m_Amplitude[m_nWhichAmp][ii].rgbValue = RGB(nRed,nGreen,nBlue);
		m_Amplitude[m_nWhichAmp][ii].bSet=TRUE;
	}
	m_ddBStepSize[m_nWhichAmp] = 100/16;
	CalculateAmplitudePalette();
	CalculateExtPalette();
}


int CUSLPalette::AddEmptyPorosityEntry()
{
	int nn = m_nPorosityL;
	m_nPorosityL++;

	if(m_pPorosity == NULL) {
		m_pPorosity = new PorosityData[m_nPorosityL];
	} else {
		m_pPorosity = (PorosityData *)realloc(m_pPorosity,m_nPorosityL * sizeof PorosityData);
	};

	ZeroMemory(&m_pPorosity[nn],sizeof PorosityData);
	if(nn>0) {
		m_pPorosity[nn].nPlys = m_pPorosity[nn-1].nPlys;
	}

	return nn;
}

void CUSLPalette::PorosityDeleteItem(int nIndex)
{

	for(int nn=nIndex;nn<(m_nPorosityL-1);nn++) {
		m_pPorosity[nn] = m_pPorosity[nn+1];
	}
	m_nPorosityL--;
	if(m_nPorosityL>0) {
		m_pPorosity = (PorosityData *)realloc(m_pPorosity,m_nPorosityL * sizeof PorosityData);
	} else {
		SAFE_DELETE( m_pPorosity );
	}

}


void CUSLPalette::PorosityMeanChanged(int nIndex)
{

	for(int nn=0;nn<m_nPorosityL;nn++) {
		if(m_pPorosity[nn].fPorosity == 0.0f) {
			if(m_pPorosity[nn].nPlys == m_pPorosity[nIndex].nPlys) {
				m_pPorosity[nIndex].fAttenuation = m_pPorosity[nn].fMeandBAttenuation-m_pPorosity[nIndex].fMeandBAttenuation;
				return;
			}
		}
	}
}

void CUSLPalette::GetMaxAttenuationPorosity(int nIndex, float *fAttenuation, float *fPorosity)
{
	*fAttenuation = 0.0f;
	*fPorosity = 0.0f;

	for(int nn=0;nn<m_nPorosityL;nn++) {
		if(*fAttenuation < m_pPorosity[nn].fAttenuation) *fAttenuation = m_pPorosity[nn].fAttenuation;
		if(*fPorosity < m_pPorosity[nn].fPorosity) *fPorosity = m_pPorosity[nn].fPorosity;
	}

}

void CUSLPalette::GetMaxAttenuationPlys(int nIndex, float *fAttenuation, int *nPlys)
{
	*fAttenuation = 0.0f;
	*nPlys = 0;

	for(int nn=0;nn<m_nPorosityL;nn++) {
		if(*fAttenuation < m_pPorosity[nn].fAttenuation) *fAttenuation = m_pPorosity[nn].fAttenuation;
		if(*nPlys < m_pPorosity[nn].nPlys) *nPlys = m_pPorosity[nn].nPlys;
	}

}

void CUSLPalette::ProcessPorosityData()
{
	
	#define MAX_CURVES 50
	int nn,cc;
	CCoord Cp;
	Cp.Zero();
	m_nPorosityCurveL = 0;
	SAFE_DELETE_ARRAY( m_pPorosityCurve );
	m_pPorosityCurve = new CPolyCoord[MAX_CURVES];


	for(nn=0;nn<m_nPorosityL;nn++) {
		Cp.fPos[1] = m_pPorosity[nn].fAttenuation;
		Cp.fPos[0] = (float)m_pPorosity[nn].fPorosity;
		bool bCurveFound = false;
		for(cc=0;cc<m_nPorosityCurveL;cc++) {
			if(m_pPorosityCurve[cc].m_nPlys == m_pPorosity[nn].nPlys) {
				m_pPorosityCurve[cc].Add(Cp);
				bCurveFound = true;
			}
		}
		if((bCurveFound == false) && (cc < MAX_CURVES)) {
			m_nPorosityCurveL++;
			m_pPorosityCurve[cc].m_nPlys = m_pPorosity[nn].nPlys;
			m_pPorosityCurve[cc].Add(Cp);
		}
	}


	m_PorosityThresholdCurve.Zero();
	Cp.Zero();
	m_PorosityThresholdCurve.Add(Cp);

	for(cc=0;cc<m_nPorosityCurveL;cc++) {
		m_pPorosityCurve[cc].CalculateCoord(theApp.m_LastSettings.fPorosityThreshold,Cp);
		Cp.fPos[0] = (float)m_pPorosityCurve[cc].m_nPlys;
		Cp.fPos[1] = Cp.fPos[1];
		m_PorosityThresholdCurve.Add(Cp);
	}

}


void CUSLPalette::PorositySort()
{
	qsort(m_pPorosity,m_nPorosityL,sizeof PorosityData,ComparePorosityData);
}

int ComparePorosityData(const void *Cp0,const void *Cp1)
{
	PorosityData *Cl0 = (PorosityData*)Cp0;
	PorosityData *Cl1 = (PorosityData*)Cp1;

	if(Cl0->nPlys < Cl1->nPlys) return -1;
	if(Cl0->nPlys > Cl1->nPlys) return 1;

	if(Cl0->fPorosity < Cl1->fPorosity) return -1;
	if(Cl0->fPorosity > Cl1->fPorosity) return 1;
	
	return 0;

}


void CUSLPalette::PorosityGetMinMaxPlies(int *nMinPlies, int *nMaxPlies)
{
	CCoord Cp;
	m_PorosityThresholdCurve.GetCoord(0,&Cp);
	*nMinPlies = (int)Cp.fPos[0];
	m_PorosityThresholdCurve.GetLastCoord(&Cp);
	*nMaxPlies = (int)Cp.fPos[0];
}

void CUSLPalette::PorosityDeleteAll()
{
	m_nPorosityL = 0;
	SAFE_DELETE( m_pPorosity );
	m_fPorositySquirterDiameter = 6.0f;
	m_fPorosityProbeDiameter = 19.0f;
	m_fPorosityFrequency = 1.0f;

}
