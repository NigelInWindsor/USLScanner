// Probe.cpp: implementation of the CProbe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "Probe.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProbe::CProbe()
{
	int	ii;

	m_FileName.Empty();
	m_bModified = FALSE;

	InitializeFields();
	m_Fields.Add(_T("Manufacturer"));
	m_Fields.Add(_T("Centre Frequency"));
	m_Fields.Add(_T("Active Diameter"));
	m_Fields.Add(_T("Mechanical focusing"));
	m_Fields.Add(_T("Focal length"));
	m_Fields.Add(_T("Serial number"));
	m_Fields.Add(_T("Description"));
	m_Fields.Add(_T("Supplied date"));
	m_Fields.Add(_T("Model Type"));

	m_strValues.SetSize(m_nNumberFields=9);

	m_nNumberDataSheets=0;
	for(ii=0;ii<256;ii++) {
		m_pProbeDataSheet[ii] = NULL;
	}

	m_nNumberBeamProfiles=0;
	for(ii=0;ii<256;ii++) {
		m_pProbeBeamProfile[ii] = NULL;
	}
}

CProbe::~CProbe()
{
	for(int ii=0;ii<256;ii++) {
		SAFE_DELETE(m_pProbeDataSheet[ii]);
		SAFE_DELETE(m_pProbeBeamProfile[ii]);
	}

}

void CProbe::InitializeFields()
{
	m_Fields.RemoveAll();
	m_Fields.Add(_T("Manufacturer"));
	m_Fields.Add(_T("Centre Frequency"));
	m_Fields.Add(_T("Active Diameter"));
	m_Fields.Add(_T("Mechanical focusing"));
	m_Fields.Add(_T("Focal length"));
	m_Fields.Add(_T("Serial number"));
	m_Fields.Add(_T("Description"));
	m_Fields.Add(_T("Model Type"));
	m_Fields.Add(_T("Supplied date"));

}

int CProbe::Save(CUSLFile *pFile)
{
	WCHAR Identifier[]={_T("Ultrasonic Sciences Ltd USLScanner Probe")};
	int nDataSheetOffsets[256],nBeamProfileOffsets[256];
	CArchive* pArchFile;
	int nStartOffset = 0;
	int	nStringOffset,nVariablesOffset;
	int	ii;

	ZeroMemory(nDataSheetOffsets,sizeof nDataSheetOffsets);
	ZeroMemory(nBeamProfileOffsets,sizeof nBeamProfileOffsets);


	m_FileName = pFile->GetFileName();

	pFile->Seek(nStartOffset,CFile::begin);
	pFile->Write(nDataSheetOffsets,sizeof nDataSheetOffsets);
	pFile->Seek(nStartOffset,CFile::begin);

	pFile->Write(Identifier,sizeof Identifier);
	pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);

	pFile->Seek(nStringOffset = (int)pFile->GetPosition() + 0x2000,CFile::begin);
	pArchFile = new CArchive(pFile,CArchive::store);
	m_Fields.Serialize(*pArchFile);
	m_strValues.Serialize(*pArchFile);
	pArchFile->Close();
	delete pArchFile;

	pFile->Seek(nVariablesOffset = (int)pFile->GetPosition() + 0x200,CFile::begin);
	m_Cp[0].Save(pFile);
	m_Cp[1].Save(pFile);

	for(ii=0;ii<m_nNumberDataSheets;ii++) {
		nDataSheetOffsets[ii] = m_pProbeDataSheet[ii]->Save(pFile);
	}
	for(ii=0;ii<m_nNumberBeamProfiles;ii++) {
		nBeamProfileOffsets[ii] = m_pProbeBeamProfile[ii]->Save(pFile);
	}


	pFile->Seek(0x100,CFile::begin);
	pFile->Write(&nStringOffset,sizeof nStringOffset);
	pFile->Write(&m_nNumberDataSheets,sizeof m_nNumberDataSheets);
	for(ii=0;ii<m_nNumberDataSheets;ii++) {
		pFile->Write(&nDataSheetOffsets[ii],sizeof (int));
	}
	pFile->Write(&m_nNumberBeamProfiles,sizeof m_nNumberBeamProfiles);
	for(ii=0;ii<m_nNumberBeamProfiles;ii++) {
		pFile->Write(&nBeamProfileOffsets[ii],sizeof (int));
	}
	pFile->Write(&nVariablesOffset,sizeof nVariablesOffset);

	pFile->Seek(0,CFile::end);
	m_bModified = FALSE;
	return nStartOffset;
}

bool CProbe::Retrieve(CUSLFile *pFile)
{
	WCHAR wIdentifier[]={_T("Ultrasonic Sciences Ltd USLScanner Probe")};
	WCHAR wStr[60];
	CArchive* pArchFile;
	int	nStringOffset,nVariablesOffset;
	int	ii;
	int nDataSheetOffsets[256],nBeamProfileOffsets[256];

	ZeroMemory(wStr,sizeof wStr);
	pFile->Seek(0,CFile::begin);
	pFile->Read(wStr,sizeof wIdentifier);
	pFile->Read(&pFile->m_fVersion,sizeof pFile->m_fVersion);
	if(wcscmp(wStr,wIdentifier)!=0) {
		MessageBox(NULL,_T("This is not a compatible file containing probe characteristics"),_T("Error"),MB_ICONERROR);
		return FALSE;

	}

	Zero();

	m_FileName = pFile->GetFileName();

	pFile->Seek(0x100,CFile::begin);
	pFile->Read(&nStringOffset,sizeof nStringOffset);
	pFile->Read(&m_nNumberDataSheets,sizeof m_nNumberDataSheets);
	for(ii=0;ii<m_nNumberDataSheets;ii++) {
		pFile->Read(&nDataSheetOffsets[ii],sizeof (int));
	}
	pFile->Read(&m_nNumberBeamProfiles,sizeof m_nNumberBeamProfiles);
	for(ii=0;ii<m_nNumberBeamProfiles;ii++) {
		pFile->Read(&nBeamProfileOffsets[ii],sizeof (int));
	}
	pFile->Read(&nVariablesOffset,sizeof nVariablesOffset);

	if(nStringOffset>0) {
		pFile->Seek(nStringOffset,CFile::begin);
		pArchFile = new CArchive(pFile,CArchive::load);
		m_Fields.Serialize(*pArchFile);
		m_strValues.Serialize(*pArchFile);
		pArchFile->Close();
		delete pArchFile;
	}
//	if(m_Fields.GetSize() < m_nNumberFields) {
		InitializeFields();
//	}
	if(m_strValues.GetSize() < m_nNumberFields) {
		m_strValues.SetSize(m_nNumberFields);
	}


	for(ii=0;ii<m_nNumberDataSheets;ii++) {
		if(nDataSheetOffsets[ii]) {
			pFile->Seek(nDataSheetOffsets[ii],CFile::begin);

			SAFE_DELETE(m_pProbeDataSheet[ii]);
			m_pProbeDataSheet[ii] = new CProbeDataSheet();
			m_pProbeDataSheet[ii]->Retrieve(pFile);
		}
	}

	for(ii=0;ii<m_nNumberBeamProfiles;ii++) {
		if(nBeamProfileOffsets[ii]) {
			pFile->Seek(nBeamProfileOffsets[ii],CFile::begin);

			SAFE_DELETE(m_pProbeBeamProfile[ii]);
			m_pProbeBeamProfile[ii] = new CProbeBeamProfile();
			m_pProbeBeamProfile[ii]->Retrieve(pFile);
		}
	}

	if(nVariablesOffset>0) {
		pFile->Seek(nVariablesOffset,CFile::begin);
		m_Cp[0].Retrieve(pFile);
		m_Cp[1].Retrieve(pFile);
	}

	return TRUE;
}

void CProbe::Zero()
{
	int	ii;

	m_bModified = FALSE;
	m_strValues.RemoveAll();
	m_strValues.SetSize(m_nNumberFields=9);
	for(ii=0;ii<m_strValues.GetSize();ii++) {
		m_strValues.SetAt(ii,_T(""));
	}

	for(ii=0;ii<256;ii++) {
		SAFE_DELETE(m_pProbeDataSheet[ii]);
	}

	for(ii=0;ii<256;ii++) {
		SAFE_DELETE(m_pProbeBeamProfile[ii]);
	}

	m_nNumberDataSheets=0;
	m_nNumberBeamProfiles=0;
	m_Cp[0].Zero();
	m_Cp[1].Zero();

}

bool CProbe::CheckNewProfileAllowed()
{
	if(m_bModified == TRUE) {
		if(MessageBox(NULL,_T("There have been changes to the current profile that have not been saved.\nDo you wish to proceen without saving modifications?"),_T("Warning"),MB_YESNO) != IDYES) return FALSE;
	}
	return TRUE;

}



bool CProbe::DeleteProbeDataSheet(int nWhich)
{
	int	ii;

	if(m_nNumberDataSheets<=0) return FALSE;
	if(nWhich>=m_nNumberDataSheets) return FALSE;

	for(ii=nWhich;ii<(m_nNumberDataSheets-1);ii++) {
		m_pProbeDataSheet[ii]->m_Fields.RemoveAll();
		m_pProbeDataSheet[ii]->m_strValues.RemoveAll();
		m_pProbeDataSheet[ii]->m_Fields.Copy(m_pProbeDataSheet[ii+1]->m_Fields);
		m_pProbeDataSheet[ii]->m_strValues.Copy(m_pProbeDataSheet[ii+1]->m_strValues);

		m_pProbeDataSheet[ii]->m_bModified =		m_pProbeDataSheet[ii+1]->m_bModified;
		m_pProbeDataSheet[ii]->m_fDelay =			m_pProbeDataSheet[ii+1]->m_fDelay;
		m_pProbeDataSheet[ii]->m_fSamplePeriod =	m_pProbeDataSheet[ii+1]->m_fSamplePeriod;
		m_pProbeDataSheet[ii]->m_nFFTLength =		m_pProbeDataSheet[ii+1]->m_nFFTLength;
		m_pProbeDataSheet[ii]->m_nAcqLength =		m_pProbeDataSheet[ii+1]->m_nAcqLength;
		m_pProbeDataSheet[ii]->m_nNumberFields =	m_pProbeDataSheet[ii+1]->m_nNumberFields;
		m_pProbeDataSheet[ii]->m_osTime =			m_pProbeDataSheet[ii+1]->m_osTime;
		m_pProbeDataSheet[ii]->m_fFFTMinFrequency =	m_pProbeDataSheet[ii+1]->m_fFFTMinFrequency;
		m_pProbeDataSheet[ii]->m_fFFTMaxFrequency =	m_pProbeDataSheet[ii+1]->m_fFFTMaxFrequency;
		m_pProbeDataSheet[ii]->m_bLocked =			m_pProbeDataSheet[ii+1]->m_bLocked;
		m_pProbeDataSheet[ii]->m_nInterfaceDelay =	m_pProbeDataSheet[ii+1]->m_nInterfaceDelay;
		m_pProbeDataSheet[ii]->m_nInterfaceWidth =	m_pProbeDataSheet[ii+1]->m_nInterfaceWidth;
		

		CopyMemory(&m_pProbeDataSheet[ii]->m_TS,			&m_pProbeDataSheet[ii+1]->m_TS, sizeof m_pProbeDataSheet[ii+1]->m_TS);
		CopyMemory(&m_pProbeDataSheet[ii]->m_nAmpFrequency,	&m_pProbeDataSheet[ii+1]->m_nAmpFrequency, sizeof m_pProbeDataSheet[ii+1]->m_nAmpFrequency);
		CopyMemory(&m_pProbeDataSheet[ii]->m_cArray,		&m_pProbeDataSheet[ii+1]->m_cArray, sizeof m_pProbeDataSheet[ii+1]->m_cArray);

	}
	SAFE_DELETE(m_pProbeDataSheet[m_nNumberDataSheets-1]);
	m_nNumberDataSheets--;

	return TRUE;

}

void CProbe::AddBeamProfileScan(int nBscan_CScan,int nWhich,char *pArray, int nSamplesLine, int nNumberLines, float fFastSize, float fSlowSize, float fFastInc, float fSlowInc,CCoord &CpCentre)
{

	if(nWhich>=m_nNumberBeamProfiles) {
		nWhich = m_nNumberBeamProfiles;
		m_nNumberBeamProfiles++;
		m_pProbeBeamProfile[nWhich] = new CProbeBeamProfile();
	}
	if(nBscan_CScan == 0) {
		m_pProbeBeamProfile[nWhich]->DeleteAllBScans();
		m_pProbeBeamProfile[nWhich]->AddBScan(pArray, nSamplesLine, nNumberLines, fFastSize, fSlowSize, fFastInc, fSlowInc,CpCentre,m_Cp[0],m_Cp[1]);
	} else {
		m_pProbeBeamProfile[nWhich]->AddCScan(pArray, nSamplesLine, nNumberLines, fFastSize, fSlowSize, fFastInc, fSlowInc,CpCentre,m_Cp[0],m_Cp[1]);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


CProbeDataSheet::CProbeDataSheet()
{
	CTime CurrentTime = CTime::GetCurrentTime();
	m_osTime=(int)CurrentTime.GetTime();

	m_bModified = FALSE;
	m_bLocked = FALSE;

	m_Fields.Add(_T("Date"));
	m_Fields.Add(_T("Operator"));
	m_Fields.Add(_T("Instruction"));
	m_Fields.Add(_T("Target type"));
	m_Fields.Add(_T("Target position"));

	m_strValues.SetSize(m_nNumberFields=5);
	m_strValues.SetAt(0,CurrentTime.Format(_T("%d/%m/%Y")));
	m_strValues.SetAt(1,theApp.m_Operator);

	CopyMemory(&m_TS,&theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot],sizeof m_TS);
	CopyMemory(m_cArray,theApp.m_Scope.m_RFTrace[theApp.m_UtUser.m_Global.nTimeSlot],8192);

	m_nAcqLength =			m_TS.Adc.nAcqLength;
	m_fSamplePeriod =		m_TS.Adc.fSamplePeriod;
	m_fFFTMinFrequency =	theApp.m_LastSettings.fFFTMinFrequency;
	m_fFFTMaxFrequency =	theApp.m_LastSettings.fFFTMaxFrequency;
	m_nInterfaceDelay = 0;
	m_nInterfaceWidth = m_nAcqLength;
	FastFourierTransform();

}

CProbeDataSheet::~CProbeDataSheet()
{

}

int CProbeDataSheet::Save(CUSLFile* pFile)
{
	CArchive* pArchFile;
	int nStartOffset;

	pFile->Seek(nStartOffset = (int)pFile->GetPosition() + 0x100,CFile::begin);

	pArchFile = new CArchive(pFile,CArchive::store);
	m_Fields.Serialize(*pArchFile);
	m_strValues.Serialize(*pArchFile);
	pArchFile->Close();
	delete pArchFile;

	pFile->Write(&m_nAcqLength,sizeof m_nAcqLength);
	pFile->Write(m_cArray,sizeof m_cArray);
	pFile->Write(&m_nFFTLength,sizeof m_nFFTLength);
	pFile->Write(m_nAmpFrequency,sizeof m_nAmpFrequency);
	pFile->Write(&m_fDelay,sizeof m_fDelay);
	pFile->Write(&m_fSamplePeriod,sizeof m_fSamplePeriod);
	pFile->Write(&m_osTime,sizeof m_osTime);
	pFile->Write(&m_TS,sizeof m_TS);
	pFile->Write(&m_bLocked, sizeof m_bLocked);
	pFile->Write(&m_nInterfaceDelay, sizeof m_nInterfaceDelay);
	pFile->Write(&m_nInterfaceWidth, sizeof m_nInterfaceWidth);


	return nStartOffset;
}

bool CProbeDataSheet::Retrieve(CUSLFile* pFile)
{
	CArchive* pArchFile;

	Zero();

	pArchFile = new CArchive(pFile,CArchive::load);
	m_Fields.Serialize(*pArchFile);
	m_strValues.Serialize(*pArchFile);
	pArchFile->Close();
	delete pArchFile;

	pFile->Read(&m_nAcqLength,sizeof m_nAcqLength);
	pFile->Read(m_cArray,sizeof m_cArray);
	pFile->Read(&m_nFFTLength,sizeof m_nFFTLength);
	pFile->Read(m_nAmpFrequency,sizeof m_nAmpFrequency);
	pFile->Read(&m_fDelay,sizeof m_fDelay);
	pFile->Read(&m_fSamplePeriod,sizeof m_fSamplePeriod);
	pFile->Read(&m_osTime,sizeof m_osTime);
	pFile->Read(&m_TS,sizeof m_TS);
	pFile->Read(&m_bLocked, sizeof m_bLocked);
	pFile->Read(&m_nInterfaceDelay, sizeof m_nInterfaceDelay);
	pFile->Read(&m_nInterfaceWidth, sizeof m_nInterfaceWidth);

	return TRUE;
}

void CProbeDataSheet::Zero()
{

}

void CProbeDataSheet::Refresh()
{
	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Adc;
	DSP200Data* pDsp = &theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Dsp;
	CTime CurrentTime = CTime::GetCurrentTime();
	m_osTime=(int)CurrentTime.GetTime();

	m_bModified = TRUE;

	m_strValues.SetAt(0,CurrentTime.Format(_T("%d/%m/%Y")));
//	m_strValues.SetAt(1,theApp.m_Operator);

	CopyMemory(&m_TS,&theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot],sizeof m_TS);
	CopyMemory(m_cArray,theApp.m_Scope.m_RFTrace[theApp.m_UtUser.m_Global.nTimeSlot],8192);

	m_nAcqLength =			m_TS.Adc.nAcqLength;
	m_fSamplePeriod =		m_TS.Adc.fSamplePeriod;
	m_fFFTMinFrequency =	theApp.m_LastSettings.fFFTMinFrequency;
	m_fFFTMaxFrequency =	theApp.m_LastSettings.fFFTMaxFrequency;

	switch(theApp.m_LastSettings.nFFTRfSource) {
	case 0: m_nInterfaceDelay = 0;
		m_nInterfaceWidth = pAdc->nAcqLength;
		break;
	case 1:
		m_nInterfaceDelay = pDsp->Delay[0];
		m_nInterfaceWidth = pDsp->Width[0];
		if(m_nInterfaceDelay > (int)pAdc->nAcqLength) m_nInterfaceDelay = 0;
		if(m_nInterfaceDelay < 0) m_nInterfaceDelay = 0;
		if(m_nInterfaceWidth<0) m_nInterfaceWidth = 10;
		if(m_nInterfaceWidth > (int)pAdc->nAcqLength) m_nInterfaceWidth = 10;
		break;
	}


	FastFourierTransform();

}

void CProbeDataSheet::FastFourierTransform()
{
	int ii,jj;
	char Array[8192];
	ZeroMemory(Array, sizeof Array);

	for(ii=0,jj=m_nInterfaceDelay;ii<m_nInterfaceWidth;ii++,jj++) {
		Array[ii]=m_cArray[jj];
	}

	m_FFT.DoFastFourierTransform(Array,m_nInterfaceWidth,m_nInterfaceWidth);
	m_nFFTLength = m_FFT.m_nRFLength / 2;
	for(ii=0;ii<m_nFFTLength;ii++) {
		m_nAmpFrequency[ii] = m_FFT.peak[ii] * theApp.m_LastSettings.nFFTGain;
	}

}

void CProbeDataSheet::CalculateStats(float fAtAttenuation)
{
	int ii;
	int nMin=32767;
	int	nMax=-32767;
	int nMinPos=0;
	int	nMaxPos=0;
	int nPeakPos;
	int nThreshold;
	int	nStartPulsePos=0,nFinishPulsePos=10000;

	//Find pulse duration
	for(ii=0;ii<m_nAcqLength;ii++) {
		if(nMin > m_cArray[ii]) {
			nMin = m_cArray[ii];
			nMinPos = ii;
		}
		if(nMax < m_cArray[ii]) {
			nMax = m_cArray[ii];
			nMaxPos = ii;
		}
	}
	if(abs(nMin) > abs(nMax)) {
		nPeakPos = nMinPos;
		nThreshold = (int)(pow(10,fAtAttenuation / 20.0f) * (float)abs(nMin));
	} else {
		nPeakPos = nMaxPos;
		nThreshold = (int)(pow(10,fAtAttenuation / 20.0f) * (float)abs(nMax));
	}
	for(ii=0;ii<nPeakPos;ii++) {
		if(abs(m_cArray[ii]) >= nThreshold) {
			nStartPulsePos = ii;
			break;
		}
	}
	for(ii=m_nAcqLength-1;ii>nPeakPos;ii--) {
		if(abs(m_cArray[ii]) >= nThreshold) {
			nFinishPulsePos = ii;
			break;
		}
	}
	m_fPulseDurationNs = (float)(nFinishPulsePos - nStartPulsePos) * m_fSamplePeriod;

	//Find Peak frequency
	nMax = 0;
	nMaxPos = 0;
	for(ii=0;ii<m_nFFTLength;ii++) {
		if(m_nAmpFrequency[ii] >= nMax) {
			nMax = m_nAmpFrequency[ii];
			nMaxPos = ii;
		}
	}
	float fMaxFrequency = 1.0f / (m_fSamplePeriod * 2e-3f);

	m_fPeakFrequency = fMaxFrequency * (float)nMaxPos / (float)(m_nFFTLength - 1);
	nThreshold = (int)(pow(10,fAtAttenuation / 20.0f) * (float)abs(nMax));

	for(ii=nMaxPos;ii>=0;ii--) {
		if(m_nAmpFrequency[ii] <= nThreshold) {
			m_fLowerFrequency = fMaxFrequency * (float)ii / (float)(m_nFFTLength - 1);
			break;
		}
	}
	for(ii=nMaxPos;ii<m_nFFTLength;ii++) {
		if(m_nAmpFrequency[ii] <= nThreshold) {
			m_fUpperFrequency = fMaxFrequency * (float)ii / (float)(m_nFFTLength - 1);
			break;
		}
	}

	m_fCentralFrequency = (m_fUpperFrequency + m_fLowerFrequency) / 2.0f;
	m_fBandWidth = m_fUpperFrequency - m_fLowerFrequency;
}


float CProbeDataSheet::GetPulseDuration(CString &Buff)
{

	Buff.Format(_T("%.02f us"),m_fPulseDurationNs / 1000.0f);
	return m_fPulseDurationNs / 1000.0f;
}

float CProbeDataSheet::GetPeakFrequency(CString &Buff)
{

	Buff.Format(_T("%.02f MHz"),m_fPeakFrequency);
	return m_fPeakFrequency;
}

float CProbeDataSheet::GetCentralFrequency(CString &Buff)
{

	Buff.Format(_T("%.02f MHz"),m_fCentralFrequency);
	return m_fCentralFrequency;
}

float CProbeDataSheet::GetBandWidth(CString &Buff)
{

	if(m_fBandWidth < 1.0f) {
		Buff.Format(_T("%.0f KHz"),m_fBandWidth * 1000.0f);
	} else {
		Buff.Format(_T("%.02f MHz"),m_fBandWidth);
	}
	return m_fBandWidth;
}

float CProbeDataSheet::GetLowerFrequency(CString &Buff)
{
	Buff.Format(_T("%.02f MHz"),m_fLowerFrequency);
	return m_fLowerFrequency;

}

float CProbeDataSheet::GetUpperFrequency(CString &Buff)
{
	Buff.Format(_T("%.02f MHz"),m_fUpperFrequency);
	return m_fUpperFrequency;

}

float CProbeDataSheet::GetRatioBWCF(CString &Buff)
{
	float fRatio = m_fBandWidth/m_fCentralFrequency*100.0f;

	Buff.Format(_T("%.0f%%"),fRatio);
	return fRatio;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

CProbeBeamProfile::CProbeBeamProfile()
{
	CTime CurrentTime = CTime::GetCurrentTime();
	m_osTime=(int)CurrentTime.GetTime();

	m_bModified = FALSE;
	m_nBScansL = 0;
	m_pBScans = NULL;
	m_nCScansL = 0;
	m_pCScans = NULL;
	m_bLocked = FALSE;

	m_Fields.Add(_T("Date"));
	m_Fields.Add(_T("Operator"));
	m_Fields.Add(_T("Instruction"));
	m_Fields.Add(_T("Target type"));

	m_strValues.SetSize(m_nNumberFields=4);
	m_strValues.SetAt(0,CurrentTime.Format(_T("%d/%m/%Y")));
	m_strValues.SetAt(1,theApp.m_Operator);

	CopyMemory(&m_TS,&theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot],sizeof m_TS);

}

CProbeBeamProfile::~CProbeBeamProfile()
{
	Zero();
}

int CProbeBeamProfile::Save(CUSLFile* pFile)
{
	CArchive* pArchFile;
	int nStartOffset,ii;

	pFile->Seek(nStartOffset = (int)pFile->GetPosition() + 0x100,CFile::begin);

	pArchFile = new CArchive(pFile,CArchive::store);
	m_Fields.Serialize(*pArchFile);
	m_strValues.Serialize(*pArchFile);
	pArchFile->Close();
	delete pArchFile;

	pFile->Write(&m_osTime,sizeof m_osTime);
	pFile->Write(&m_bLocked, sizeof m_bLocked);
	pFile->Write(&m_TS,sizeof m_TS);

	pFile->Write(&m_nCScansL,sizeof m_nCScansL);
	for(ii=0;ii<m_nCScansL;ii++) {
		m_pCScans[ii].Save(pFile);
	}

	pFile->Write(&m_nBScansL,sizeof m_nCScansL);
	for(ii=0;ii<m_nBScansL;ii++) {
		m_pBScans[ii].Save(pFile);
	}



	return nStartOffset;
}

bool CProbeBeamProfile::Retrieve(CUSLFile* pFile)
{
	CArchive* pArchFile;
	int ii;

	Zero();

	pArchFile = new CArchive(pFile,CArchive::load);
	m_Fields.Serialize(*pArchFile);
	m_strValues.Serialize(*pArchFile);
	pArchFile->Close();
	delete pArchFile;

	pFile->Read(&m_osTime,sizeof m_osTime);
	pFile->Read(&m_bLocked, sizeof m_bLocked);
	pFile->Read(&m_TS,sizeof m_TS);

	pFile->Read(&m_nCScansL,sizeof m_nCScansL);
	if(m_nCScansL) {
		m_pCScans = new CProbeScanData[m_nCScansL];
		for(ii=0;ii<m_nCScansL;ii++) {
			m_pCScans[ii].Retrieve(pFile);
		}
	}

	pFile->Read(&m_nBScansL,sizeof m_nBScansL);
	if(m_nBScansL) {
		m_pBScans = new CProbeScanData[m_nBScansL];
		for(ii=0;ii<m_nBScansL;ii++) {
			m_pBScans[ii].Retrieve(pFile);
		}
	}

	return TRUE;
}

void CProbeBeamProfile::Zero()
{
	SAFE_DELETE_ARRAY(m_pCScans);
	m_nCScansL = 0;
	SAFE_DELETE_ARRAY(m_pBScans);
	m_nBScansL = 0;

	m_bLocked = FALSE;
}

void CProbeBeamProfile::AllocateMemory()
{

}

void CProbeBeamProfile::DeleteAllBScans()
{
	SAFE_DELETE_ARRAY(m_pBScans);
	m_nBScansL = 0;
}

void CProbeBeamProfile::DeleteAllCScans()
{
	SAFE_DELETE_ARRAY(m_pCScans);
	m_nCScansL = 0;
}



void CProbeBeamProfile::AddCScan(char *pArray, int nSamplesLine, int nNumberLines, float fFastSize, float fSlowSize, float fFastInc, float fSlowInc,CCoord &CpCentre,CCoord &CpStart,CCoord &CpFinish)
{
	CProbeScanData* pProbeScanTemp = NULL;
	int ii,nArraySize;
	
	if(m_nCScansL>0) {
		pProbeScanTemp = new CProbeScanData[m_nCScansL];
		for(ii=0;ii<m_nCScansL;ii++) {
			pProbeScanTemp[ii] = m_pCScans[ii];
		}
		SAFE_DELETE_ARRAY(m_pCScans);

		m_pCScans = new CProbeScanData[m_nCScansL+1];
		for(ii=0;ii<m_nCScansL;ii++) {
			m_pCScans[ii] = pProbeScanTemp[ii];
		}
		SAFE_DELETE_ARRAY(pProbeScanTemp);
	} else {
		m_pCScans = new CProbeScanData[1];
	}

	ii=m_nCScansL;

	m_pCScans[ii].m_cAmpBuffer = new unsigned char[nArraySize = nSamplesLine * nNumberLines];
	CopyMemory(m_pCScans[ii].m_cAmpBuffer,pArray,  nArraySize);
	m_pCScans[ii].m_CpCentre		= CpCentre;
	m_pCScans[ii].m_fFastIncrement	= fFastInc;
	m_pCScans[ii].m_fFastScanSize	= fFastSize;
	m_pCScans[ii].m_fSlowIncrement	= fSlowInc;
	m_pCScans[ii].m_fSlowScanSize	= fSlowSize;
	m_pCScans[ii].m_nNumberLines	= nNumberLines;
	m_pCScans[ii].m_nSamplesLine	= nSamplesLine;
	m_pCScans[ii].m_Cp[0]			= CpStart;
	m_pCScans[ii].m_Cp[1]			= CpFinish;

	m_nCScansL++;

}

void CProbeBeamProfile::AddBScan(char *pArray, int nSamplesLine, int nNumberLines, float fFastSize, float fSlowSize, float fFastInc, float fSlowInc,CCoord &CpCentre,CCoord &CpStart,CCoord &CpFinish)
{
	CProbeScanData* pProbeScanTemp = NULL;
	int ii,nArraySize;
	
	if(m_nBScansL>0) {
		pProbeScanTemp = new CProbeScanData[m_nBScansL];
		for(ii=0;ii<m_nBScansL;ii++) {
			pProbeScanTemp[ii] = m_pBScans[ii];
		}
		SAFE_DELETE_ARRAY(m_pBScans);

		m_pBScans = new CProbeScanData[m_nBScansL+1];
		for(ii=0;ii<m_nBScansL;ii++) {
			m_pBScans[ii] = pProbeScanTemp[ii];
		}
		SAFE_DELETE_ARRAY(pProbeScanTemp);
	} else {
		m_pBScans = new CProbeScanData[1];
	}

	ii=m_nBScansL;

	m_pBScans[ii].m_cAmpBuffer = new unsigned char[nArraySize = nSamplesLine * nNumberLines];
	CopyMemory(m_pBScans[ii].m_cAmpBuffer,pArray,  nArraySize);
	m_pBScans[ii].m_CpCentre		= CpCentre;
	m_pBScans[ii].m_fFastIncrement	= fFastInc;
	m_pBScans[ii].m_fFastScanSize	= fFastSize;
	m_pBScans[ii].m_fSlowIncrement	= fSlowInc;
	m_pBScans[ii].m_fSlowScanSize	= fSlowSize;
	m_pBScans[ii].m_nNumberLines	= nNumberLines;
	m_pBScans[ii].m_nSamplesLine	= nSamplesLine;
	m_pBScans[ii].m_Cp[0]			= CpStart;
	m_pBScans[ii].m_Cp[1]			= CpFinish;

	m_nBScansL++;

}

int CProbeBeamProfile::CalculateStatsFromCScans(float fAtAttenuation)
{
	FindPeakOfAllCScans();

	m_fSpotSizeFromCScans = FindSpotSizeOfAllCScans(fAtAttenuation);

	return m_nPeakFromCScans;
}

int CProbeBeamProfile::FindPeakOfAllCScans()
{
	int	ii,nTemp;

	m_nPeakFromCScans=0;
	for(ii=0;ii<m_nCScansL;ii++) {
		if((nTemp = m_pCScans[ii].FindPeak()) > m_nPeakFromCScans) m_nPeakFromCScans = nTemp;
	}
	return m_nPeakFromCScans;
}

float CProbeBeamProfile::FindSpotSizeOfAllCScans(float fAtAttenuation)
{	
	float fTemp;
	int ii;
	int nDropValue = (int)(pow(10,fAtAttenuation / 20.0f) * m_nPeakFromCScans);
	m_fSpotSizeFromCScans = 100.0f;

	for(ii=0;ii<m_nCScansL;ii++) {
		if((fTemp = m_pCScans[ii].FindCScanSpotSize(nDropValue)) < m_fSpotSizeFromCScans) {
			if(fTemp > 0.0f) {
				m_fSpotSizeFromCScans = fTemp;
			}
		}
	}

	return m_fSpotSizeFromCScans;
}

int CProbeBeamProfile::CalculateStatsFromBScans(float fAtAttenuation)
{
	FindPeakOfAllBScans();

	m_fSpotSizeFromBScans = FindFocalZoneOfAllBScans(fAtAttenuation);

	return m_nPeakFromBScans;
}

int CProbeBeamProfile::FindPeakOfAllBScans()
{
	int	ii,nTemp;

	m_nPeakFromBScans=0;
	for(ii=0;ii<m_nBScansL;ii++) {
		if((nTemp = m_pBScans[ii].FindPeak()) > m_nPeakFromBScans) m_nPeakFromBScans = nTemp;
	}
	return m_nPeakFromBScans;
}

float CProbeBeamProfile::FindFocalZoneOfAllBScans(float fAtAttenuation)
{	
	float fTemp;
	int ii;
	int nDropValue = (int)(pow(10,fAtAttenuation / 20.0f) * m_nPeakFromBScans);
	m_fSpotSizeFromBScans = 300.0f;

	for(ii=0;ii<m_nBScansL;ii++) {
		if((fTemp = m_pBScans[ii].FindBScanFocalZone(nDropValue)) < m_fSpotSizeFromBScans) {
			if(fTemp > 0.0f) {
				m_fSpotSizeFromBScans = fTemp;
			}
		}
	}

	return m_fSpotSizeFromCScans;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

CProbeScanData::CProbeScanData()
{
	m_nSamplesLine = 0;
	m_nNumberLines = 0;
	m_fFastScanSize = 0.0f;
	m_fSlowScanSize = 0.0f;
	m_fFastIncrement = 0.1f;
	m_fSlowIncrement = 0.1f;
	m_CpCentre.Zero();
	m_Cp[0].Zero();
	m_Cp[1].Zero();
	m_cAmpBuffer = NULL;

}

CProbeScanData::~CProbeScanData()
{
	SAFE_DELETE(m_cAmpBuffer);
}

void CProbeScanData::Zero()
{

	SAFE_DELETE(m_cAmpBuffer);
}

void CProbeScanData::AllocateMemory()
{
	int nArraySize;

	SAFE_DELETE(m_cAmpBuffer);
	if(nArraySize = (m_nSamplesLine * m_nNumberLines)) {
		m_cAmpBuffer = new unsigned char[nArraySize];
	}

}

void CProbeScanData::operator = (CProbeScanData &CProbeScan)
{
	int nArraySize = CProbeScan.m_nSamplesLine * CProbeScan.m_nNumberLines;
	m_cAmpBuffer = new unsigned char[nArraySize];

	CopyMemory(m_cAmpBuffer, CProbeScan.m_cAmpBuffer, nArraySize);
	m_CpCentre			= CProbeScan.m_CpCentre;
	m_Cp[0]				= CProbeScan.m_Cp[0];
	m_Cp[1]				= CProbeScan.m_Cp[1];
	m_fFastIncrement	= CProbeScan.m_fFastIncrement;
	m_fFastScanSize		= CProbeScan.m_fFastScanSize;
	m_fSlowIncrement	= CProbeScan.m_fSlowIncrement;
	m_fSlowScanSize		= CProbeScan.m_fSlowScanSize;
	m_nNumberLines		= CProbeScan.m_nNumberLines;
	m_nSamplesLine		= CProbeScan.m_nSamplesLine;
		
};

int CProbeScanData::Save(CUSLFile *pFile)
{
	int nStartOffset = (int)pFile->GetPosition();

	pFile->Write(&m_nSamplesLine,sizeof m_nSamplesLine);
	pFile->Write(&m_nNumberLines, sizeof m_nNumberLines);
	pFile->Write(&m_fFastScanSize,sizeof m_fFastScanSize);
	pFile->Write(&m_fSlowScanSize,sizeof m_fSlowScanSize);
	pFile->Write(&m_fFastIncrement,sizeof m_fFastIncrement);
	pFile->Write(&m_fSlowIncrement,sizeof m_fSlowIncrement);
	m_CpCentre.Save(pFile);
	m_Cp[0].Save(pFile);
	m_Cp[1].Save(pFile);
	pFile->Write(m_cAmpBuffer,m_nSamplesLine * m_nNumberLines);

	return nStartOffset;

}

bool CProbeScanData::Retrieve(CUSLFile *pFile)
{
	Zero();

	pFile->Read(&m_nSamplesLine,sizeof m_nSamplesLine);
	pFile->Read(&m_nNumberLines, sizeof m_nNumberLines);
	pFile->Read(&m_fFastScanSize,sizeof m_fFastScanSize);
	pFile->Read(&m_fSlowScanSize,sizeof m_fSlowScanSize);
	pFile->Read(&m_fFastIncrement,sizeof m_fFastIncrement);
	pFile->Read(&m_fSlowIncrement,sizeof m_fSlowIncrement);

	if(pFile->m_fVersion>=4.0f) {
		m_CpCentre.Retrieve(pFile);
		m_Cp[0].Retrieve(pFile);
		m_Cp[1].Retrieve(pFile);
	}
	AllocateMemory();
	if(m_cAmpBuffer) pFile->Read(m_cAmpBuffer,m_nSamplesLine * m_nNumberLines);

	return TRUE;

}




int CProbeScanData::FindPeak()
{

	int nArraySize = m_nSamplesLine * m_nNumberLines;
	int nPtr,nAmp;

	m_nPeakValue = 0;
	for(int nLine = 0;nLine<(m_nNumberLines-1);nLine++) {
		nPtr = nLine * m_nSamplesLine;
		for(int nSample=0;nSample<(m_nSamplesLine-2);nSample++,nPtr++) {

			nAmp = (m_cAmpBuffer[nPtr] + m_cAmpBuffer[nPtr+1] + m_cAmpBuffer[nPtr+2] + 
				m_cAmpBuffer[nPtr+m_nSamplesLine] + m_cAmpBuffer[nPtr+m_nSamplesLine+1] + m_cAmpBuffer[nPtr+m_nSamplesLine+2]) / 6;

			if(nAmp>m_nPeakValue) {
				m_nPeakValue = nAmp;
				m_ptCentre.x=nSample;
				m_ptCentre.y=nLine;
			}
		}
	}
	return m_nPeakValue;
}


float CProbeScanData::FindCScanSpotSize(int nDropValue)
{
	int ii,nAmp,nPtr;
	int nRight,nLeft,nTop,nBottom;
	int nMask = 0;
	int nThresh = m_nPeakValue / 2;

	//Find centre
	nBottom = m_nNumberLines-1;

	nPtr=0;
	nTop=-1;
	nLeft = m_nSamplesLine-1;
	nRight = 0;
	for(int nLine = 0;nLine<m_nNumberLines;nLine++) {
		for(int nSample=0;nSample<m_nSamplesLine;nSample++,nPtr++) {
			nAmp = m_cAmpBuffer[nPtr];
			if(nAmp > nThresh) {
				if(nTop==-1) nTop = nLine;
				nBottom = nLine;
				if(nSample<nLeft) nLeft = nSample;
				if(nSample>nRight) nRight = nSample;
			}
		}
	}
	m_ptCentre.x = (nRight+nLeft)/2;
	m_ptCentre.y = (nTop+nBottom)/2;

	//right dB rise
	if(m_cAmpBuffer != NULL) {
		nPtr = m_ptCentre.y * m_nSamplesLine;
		for(ii=m_nSamplesLine-1;ii>=m_ptCentre.x;ii--) {
			nAmp = (m_cAmpBuffer[nPtr+ii] + m_cAmpBuffer[nPtr+m_nSamplesLine+ii]) / 2;
			if(nAmp >= nDropValue) {
				m_rrArc.right = ii;
				nMask |= 1;
				break;
			}
		}
		//left dB rise
		nPtr = m_ptCentre.y * m_nSamplesLine;
		for(ii=0;ii<=m_ptCentre.x;ii++) {
			nAmp = (m_cAmpBuffer[nPtr+ii] + m_cAmpBuffer[nPtr+m_nSamplesLine+ii]) / 2;
			if(nAmp >= nDropValue) {
				m_rrArc.left = ii;
				nMask |= 2;
				break;
			}
		}
		//bottom dB rise
		for(ii=m_nNumberLines-2;ii>=m_ptCentre.y;ii--) {
			nPtr = ii * m_nSamplesLine + m_ptCentre.x;
			nAmp = (m_cAmpBuffer[nPtr] + m_cAmpBuffer[nPtr+m_nSamplesLine]) / 2;
			if(nAmp >= nDropValue) {
				m_rrArc.bottom = ii;
				nMask |= 4;
				break;
			}
		}
		//top dB rise
		for(ii=0;ii<=m_ptCentre.y;ii++) {
			nPtr = ii * m_nSamplesLine + m_ptCentre.x;
			nAmp = (m_cAmpBuffer[nPtr] + m_cAmpBuffer[nPtr+m_nSamplesLine]) / 2;
			if(nAmp >= nDropValue) {
				m_rrArc.top = ii;
				nMask |= 8;
				break;
			}
		}
	}
	if(nMask == 0x0f) {
		m_fWidthSpot = (float)(m_rrArc.right-m_rrArc.left) * m_fFastIncrement;
		m_fHeightSpot = (float)(m_rrArc.bottom-m_rrArc.top) * m_fSlowIncrement;

		if(m_fWidthSpot < m_fHeightSpot) {
			m_fDiameterSpot = m_fWidthSpot;
			if(m_fHeightSpot) m_fSymmetrySpot = m_fWidthSpot / m_fHeightSpot;
		} else {
			m_fDiameterSpot = m_fHeightSpot;
			if(m_fWidthSpot) m_fSymmetrySpot = m_fHeightSpot / m_fWidthSpot;
		}
	} else {
		m_rrArc.right = m_ptCentre.x;
		m_rrArc.left = m_ptCentre.x;
		m_rrArc.top = m_ptCentre.y;
		m_rrArc.bottom = m_ptCentre.y;
		m_fDiameterSpot = 0.0f;
		m_fWidthSpot = 0.0f;
		m_fHeightSpot = 0.0f;
		m_fSymmetrySpot = 1.0f;
	}
	return m_fDiameterSpot;
}

float CProbeScanData::FindBScanFocalZone(int nDropValue)
{
	int nPtr,nAmp,nPtr1;
	float fX,fY;
	int nMask = 0;
	int nLeft,nRight;

	m_ptEllipseTop.x = m_nSamplesLine / 2;
	m_ptEllipseTop.y = m_nNumberLines - 1;

	m_ptEllipseLeft.x = m_nSamplesLine / 2;
	m_ptEllipseLeft.y = m_nNumberLines / 2;

	m_ptEllipseRight.x = m_nSamplesLine / 2;
	m_ptEllipseRight.y = m_nNumberLines / 2;

	m_ptEllipseBottom.x = m_nSamplesLine / 2;
	m_ptEllipseBottom.y = 0;


	for(int nLine=0;nLine<(m_nNumberLines-1);nLine++) {
		nPtr = nLine * m_nSamplesLine;
		nPtr1 = (nLine + 1) * m_nSamplesLine;
		nLeft = m_nSamplesLine-1;
		nRight = 0;
		nMask &= 1;
		for(int nSample=0;nSample<(m_nSamplesLine-2);nSample++,nPtr++,nPtr1++) {
			nAmp = (m_cAmpBuffer[nPtr] + m_cAmpBuffer[nPtr+1] + m_cAmpBuffer[nPtr+2] +
					m_cAmpBuffer[nPtr1] + m_cAmpBuffer[nPtr1+1] + m_cAmpBuffer[nPtr1+2] ) / 6;

			if(nAmp > nDropValue) {
				if((m_ptEllipseTop.y>nLine) && ((nMask & 1) == 0)) {
					m_ptEllipseTop.x=nSample;
					m_ptEllipseTop.y=nLine;
					nMask |= 1;
				}
				if(m_ptEllipseBottom.y<nLine) {
					m_ptEllipseBottom.x=nSample;
					m_ptEllipseBottom.y=nLine;
				}
				if((nSample<nLeft) && ((nMask & 2) == 0)) {
					nLeft = nSample;
					nMask|=2;
				}
				nRight = nSample;
			}
		}
		if((nMask & 1) && (m_ptEllipseTop.y==nLine)) {
			m_ptEllipseTop.x = (nRight + nLeft) / 2;
		}
		if(m_ptEllipseBottom.y==nLine) {
			m_ptEllipseBottom.x = (nRight + nLeft) / 2;
		}

		if(nMask & 2) {
			if(nLine > m_ptCentre.y) {
				if(abs(nRight-nLeft) > abs(m_ptEllipseRight.x-m_ptEllipseLeft.x)) {
					m_ptEllipseLeft.x=nLeft;
					m_ptEllipseLeft.y=nLine;
					m_ptEllipseRight.x=nRight;
					m_ptEllipseRight.y=nLine;
				}
			}
		}
	}
	fX = (float)(m_ptEllipseRight.x-m_ptEllipseLeft.x) * m_fFastIncrement;
	m_fDiameterSpot = fX;

	fX = (float)(m_ptEllipseBottom.x-m_ptEllipseTop.x) * m_fFastIncrement;
	fY = (float)(m_ptEllipseBottom.y-m_ptEllipseTop.y) * m_fFastIncrement;
	m_fFocalLength = hypot(fX,fY);

	return m_fDiameterSpot;
}

