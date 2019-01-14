// WorkSpace.cpp: implementation of the CWorkSpace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "WorkSpace.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkSpace::CWorkSpace()
{
	

	m_pDataArray = NULL;
	m_pProfileArray = NULL;
	m_pProfile = &theApp.m_Profile;
	m_pData = NULL;
	m_pCopyData = NULL;
	m_pCopyProfile = NULL;
	m_pPlayList = NULL;
	m_nPlayListL = 0;
	m_ScanSize = CSize(0,0);
	m_SampleSize = CSize(0,0);
	m_rrHistogramSample = CRect(0,0,0,0);



	Zero();
}

CWorkSpace::~CWorkSpace()
{

	SAFE_DELETE( m_pCopyData );
	SAFE_DELETE( m_pCopyProfile );
	Zero();

}

void CWorkSpace::Zero()
{
	SAFE_DELETE_ARRAY( m_pDataArray );
	SAFE_DELETE_ARRAY( m_pProfileArray );
	SAFE_DELETE_ARRAY( m_pPlayList );
	m_nDataIndex = -1;
	m_nProfileIndex = -1;
	m_nDataL = 0;
	m_nProfileL = 0;
	m_nPlayListL = 0;
	m_bProfileModified = false;
	m_bDataModified = false;

	m_nScanMethod = 0;
	m_osTimeBetweenStandards = 0;
	m_rrHistogramSample.SetRect(0,0,0,0);
	m_fAttenuationRange[0] = 0.0f;
	m_fAttenuationRange[1] = 0.0f;
	m_fdBRefLevel = 0.0f;
	m_nRefTimeSlot = 0;
	m_nRefGate = 0;
	m_nFlag = 0;
	m_ScanSize = CSize(0,0);
	m_SampleSize = CSize(0,0);
	m_rrHistogramSample = CRect(0,0,0,0);

	ZeroMemory(m_fdBRefLevelArray, sizeof m_fdBRefLevelArray);
	m_ndBRefLevelL = 0;

}

void CWorkSpace::AddProfile()
{
	CProfile* pProfile = NULL;
	int	nP;

	if((m_pProfileArray!=NULL) && (m_nProfileL>0)) {
		pProfile = new CProfile[m_nProfileL];
		for(nP=0;nP<m_nProfileL;nP++) {
			pProfile[nP] = &m_pProfileArray[nP];
		}
		SAFE_DELETE_ARRAY( m_pProfileArray );

		m_nProfileL++;
		m_pProfileArray = new CProfile[m_nProfileL];
		for(nP=0;nP<(m_nProfileL-1);nP++) {
			m_pProfileArray[nP] = &pProfile[nP];
		}
		SAFE_DELETE_ARRAY( pProfile );
		SelectLastProfile();
	} else {
		m_nProfileL=1;
		m_pProfileArray = new CProfile[m_nProfileL];
		SelectProfile(0);
	}

	m_bProfileModified=TRUE;

}

bool CWorkSpace::RetrieveProfile(CUSLFile *pFile)
{
	bool bFlag = false;

	if((m_nProfileIndex>=0) && (m_nProfileIndex<m_nProfileL)) {
		m_pProfile->Zero();
		bFlag = m_pProfile->Retrieve(pFile);
		if(m_pProfile->m_strDescription.IsEmpty()) {
			m_pProfile->m_strDescription = pFile->GetFileTitle();
			m_pProfile->m_strDescription.Replace(L".pro",L"");
		}
		return bFlag;
	}

	return false;

}

bool CWorkSpace::SelectProfile(int nIndex)
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if((nIndex>=0) && (nIndex<m_nProfileL)) {
		m_nProfileIndex = nIndex;
		m_pProfile = &m_pProfileArray[nIndex];
	}
	if(m_nProfileL<=0) {
		m_pProfile = &theApp.m_Profile;
	};

	if(theApp.m_LastSettings.nLoadUltrasonicsFromProfile==1) {
		PROFILE->CopyLocalUtToGlobal();
	}
	pFrame->SendMessage(UI_PROFILE_OPENED,(WPARAM)this,NULL);
	theApp.ActivityLog(L"Profile changed : ");

	return true;
}

bool CWorkSpace::SelectLastProfile()
{
	
	if(m_nProfileL<=0) return false;
	return SelectProfile(m_nProfileL - 1 );
}

bool CWorkSpace::DeleteProfile(int nIndex)
{
	int nn;

	if(nIndex <= -1) nIndex = m_nProfileIndex;
		
	for(nn=m_nProfileIndex;nn<(m_nProfileL-1);nn++) {
		m_pProfileArray[nn].Zero();
		m_pProfileArray[nn] = &m_pProfileArray[nn+1];
	}
	m_nProfileL--;
	if(m_nProfileL==0) {
		SAFE_DELETE_ARRAY( m_pProfileArray );
	};
	SelectProfile(m_nProfileIndex);

	DeleteAllLadderItemsWithIndex(m_nProfileIndex);

	return true;

}

bool CWorkSpace::Save(CUSLFile *pFile)
{
	
	WCHAR Identifier[]={_T("Ultrasonic Sciences Ltd USLScanner Work Space")};
	char cOffsetArray[2048];
	ZeroMemory(cOffsetArray,2048);

	if(m_nProfileL<=0) return false;

	int	nVariablesOffset,nLadderOffset;
	int *pnProfileOffset = new int[m_nProfileL];
	ZeroMemory(pnProfileOffset, sizeof (int) * m_nProfileL);

	m_FileName = pFile->GetFileName();

	int nStartOffset = (int)pFile->GetPosition();

	pFile->Write(cOffsetArray,sizeof cOffsetArray);
	pFile->Seek(nStartOffset,CFile::begin);

	pFile->Write(Identifier,sizeof Identifier);
	pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);

	pFile->Seek(nVariablesOffset=(int)pFile->GetPosition()+0x200 + (sizeof (int) * m_nProfileL),CFile::begin);
	pFile->Write(&m_nProfileL, sizeof m_nProfileL);
	pFile->Write(&m_nScanMethod,sizeof m_nScanMethod);
	pFile->Write(&m_osTimeBetweenStandards,sizeof m_osTimeBetweenStandards);
	pFile->Write(&m_rrHistogramSample,sizeof m_rrHistogramSample);
	pFile->Write(m_fAttenuationRange,sizeof m_fAttenuationRange);
	pFile->Write(&m_fdBRefLevel,sizeof m_fdBRefLevel);
	pFile->Write(&m_nRefTimeSlot,sizeof m_nRefTimeSlot);
	pFile->Write(&m_nRefGate,sizeof m_nRefGate);
	pFile->Write(&m_nFlag,sizeof m_nFlag);
	pFile->Write(&m_ScanSize, sizeof m_ScanSize);
	pFile->Write(&m_SampleSize, sizeof m_SampleSize);

	for(int ii=0;ii<m_nProfileL;ii++) {
		pFile->Seek(pnProfileOffset[ii]=(int)pFile->GetPosition()+0x200,CFile::begin);
		m_pProfileArray[ii].Save(pFile);
	}


	pFile->Seek(nLadderOffset=(int)pFile->GetPosition()+0x200 + (sizeof (int) * m_nProfileL),CFile::begin);
	pFile->Write(&m_nPlayListL, sizeof m_nPlayListL);
	for(int ii=0;ii<m_nPlayListL;ii++) {
		m_pPlayList[ii].Save(pFile, theApp.m_fVersion);
	};

	int nFinishOffset = (int)pFile->GetPosition();

	pFile->Seek(nStartOffset + 0x100,CFile::begin);
	pFile->Write(&nVariablesOffset, sizeof (int));
	pFile->Write(pnProfileOffset, sizeof (int) * m_nProfileL);
	pFile->Write(&nLadderOffset, sizeof (int));

	delete pnProfileOffset;

	return TRUE;
}

bool CWorkSpace::Retrieve(CUSLFile *pFile)
{
	
	float fVersion;
	WCHAR wIdentifier[]={L"Ultrasonic Sciences Ltd USLScanner Work Space"};
	WCHAR wStr[60];
	int nVariablesOffset,nLadderOffset;

	Zero();

	m_FileName = pFile->GetFileName();

	int nStartOffset = (int)pFile->GetPosition();

	ZeroMemory(wStr,sizeof wStr);
	pFile->Seek(nStartOffset,CFile::begin);
		
	pFile->Read(wStr,sizeof wIdentifier);
	pFile->Read(&fVersion,sizeof fVersion);
	if(wcscmp(wStr,wIdentifier)!=0) return false;

	pFile->Seek(nStartOffset + 0x100,CFile::begin);
	pFile->Read(&nVariablesOffset, sizeof (int));

	pFile->Seek(nVariablesOffset,CFile::begin);
	pFile->Read(&m_nProfileL, sizeof m_nProfileL);
	pFile->Read(&m_nScanMethod,sizeof m_nScanMethod);
	pFile->Read(&m_osTimeBetweenStandards,sizeof m_osTimeBetweenStandards);
	pFile->Read(&m_rrHistogramSample,sizeof m_rrHistogramSample);
	pFile->Read(m_fAttenuationRange,sizeof m_fAttenuationRange);
	pFile->Read(&m_fdBRefLevel,sizeof m_fdBRefLevel);
	pFile->Read(&m_nRefTimeSlot,sizeof m_nRefTimeSlot);
	pFile->Read(&m_nRefGate,sizeof m_nRefGate);
	pFile->Read(&m_nFlag,sizeof m_nFlag);
	pFile->Read(&m_ScanSize, sizeof m_ScanSize);
	pFile->Read(&m_SampleSize, sizeof m_SampleSize);

	int *pnProfileOffset = new int[m_nProfileL];
	pFile->Seek(nStartOffset + 0x100,CFile::begin);
	pFile->Read(&nVariablesOffset, sizeof (int));
	pFile->Read(pnProfileOffset, sizeof (int) * m_nProfileL);
	pFile->Read(&nLadderOffset, sizeof (int));

	if(m_nProfileL>0) {
		m_pProfileArray = new CProfile[m_nProfileL];

		for(int ii=0;ii<m_nProfileL;ii++) {
			pFile->Seek(pnProfileOffset[ii],CFile::begin);
			m_pProfileArray[ii].Retrieve(pFile);
		}
	}

	if(nLadderOffset>0) {
		pFile->Seek(nLadderOffset,CFile::begin);
		pFile->Read(&m_nPlayListL, sizeof m_nPlayListL);
		if(m_nPlayListL>0) {
			m_pPlayList = new PlayListItem[m_nPlayListL];
			for(int ii=0;ii<m_nPlayListL;ii++) {
				m_pPlayList[ii].Retrieve(pFile);
				m_pPlayList[ii].nStatus = 0;
			};
		}
	};

	int nFinishOffset = (int)pFile->GetPosition();

	delete pnProfileOffset;

	return true;
}

void CWorkSpace::CopyProfile()
{

	if(m_pCopyProfile == NULL) m_pCopyProfile = new CProfile();
	m_pCopyProfile[0].Zero();
	m_pCopyProfile[0] = &m_pProfile[0];
}

void CWorkSpace::PasteProfile()
{
	CString Buff;
	if(m_pCopyProfile == NULL) return;
	AddProfile();
	SelectLastProfile();
	m_pProfile[0] = &m_pCopyProfile[0];

	Buff = m_pProfile->m_strDescription;
	m_pProfile->m_strDescription = L"Copy - " + Buff;
}

int CWorkSpace::GetLadderSize()
{
	return m_nPlayListL;
}


bool CWorkSpace::GetProfileDescription(int nIndex, CString *pBuff)
{
	if(nIndex<0 || nIndex >= m_nProfileL) return false;
	
	pBuff->Format(L"%s",m_pProfileArray[nIndex].m_strDescription);

	return true;

}

void CWorkSpace::AddToLadder()
{
	int ii;
	PlayListItem *pTempLadder = NULL;

	if(m_pPlayList == NULL) {
		m_nPlayListL = 0;
		m_pPlayList = new PlayListItem[1];
	} else {
		pTempLadder = new PlayListItem[m_nPlayListL];
		for(ii=0;ii<m_nPlayListL;ii++) {
			pTempLadder[ii] = m_pPlayList[ii];
		};

		SAFE_DELETE_ARRAY( m_pPlayList );

		m_pPlayList = new PlayListItem[ m_nPlayListL + 1];
		for(ii=0;ii<m_nPlayListL;ii++) {
			m_pPlayList[ii] = pTempLadder[ii];
		};

		SAFE_DELETE_ARRAY( pTempLadder );
	}
	m_pPlayList[m_nPlayListL].Zero();
	m_pPlayList[m_nPlayListL].nProfileIndex = m_nProfileIndex;
	m_nPlayListL++;

}

bool CWorkSpace::DeleteLadder(int nIndex)
{
	int ii,jj;
	PlayListItem *pTempLadder;

	if(nIndex<0 || nIndex>=m_nPlayListL) return false;

	if(m_nPlayListL==1) {
		m_nPlayListL=0;
		SAFE_DELETE_ARRAY( m_pPlayList );
		return true;
	}

	pTempLadder = new PlayListItem[m_nPlayListL];
	for(ii=0;ii<nIndex;ii++) {
		pTempLadder[ii] = m_pPlayList[ii];
	};
	for(jj=ii+1;jj<m_nPlayListL;ii++,jj++) {
		pTempLadder[ii] = m_pPlayList[jj];
	};

	SAFE_DELETE_ARRAY( m_pPlayList );
	m_pPlayList = new PlayListItem[--m_nPlayListL];
	for(ii=0;ii<m_nPlayListL;ii++) {
		m_pPlayList[ii] = pTempLadder[ii];
	};
	SAFE_DELETE_ARRAY( pTempLadder );

	return true;
}


void CWorkSpace::DeleteAllLadderItemsWithIndex(int nIndex)
{
	int ii;

	for(ii=0;ii<m_nPlayListL;ii++) {
		if(m_pPlayList[ii].nProfileIndex == nIndex) {
			DeleteLadder(ii);
			ii--;
		}
	}

	for(ii=0;ii<m_nPlayListL;ii++) {
		if(m_pPlayList[ii].nProfileIndex > nIndex) {
			m_pPlayList[ii].nProfileIndex--;
		}
	}

}


bool CWorkSpace::DeleteLadder(int nStartIndex, int nFinishIndex)
{
	int	ii,nLength;

	nLength = nFinishIndex-nStartIndex+1;
	for(ii=nStartIndex;ii<m_nPlayListL-nLength;ii++ ) {
		m_pPlayList[ii]=m_pPlayList[ii+nLength];
	}
	m_nPlayListL -= nLength;
	if(m_nPlayListL<=0) {
		SAFE_DELETE_ARRAY( m_pPlayList );
		m_nPlayListL = 0;
	}
	return true;
}

void CWorkSpace::AddToLadder(int nStartIndex, int nAddLength, int *pnArray)
{
	int ii,jj;
	PlayListItem *pTempLadder;

	if(m_pPlayList == NULL) {
		m_pPlayList = new PlayListItem[m_nPlayListL = nAddLength];
		for(ii=0;ii<nAddLength;ii++) {
			m_pPlayList[ii].Zero();
			m_pPlayList[ii].nProfileIndex = pnArray[ii];
		}
	} else {
		pTempLadder = new PlayListItem[m_nPlayListL + nAddLength];
		for(ii=0;ii<nStartIndex;ii++) {
			pTempLadder[ii] = m_pPlayList[ii];
		}
		for(ii,jj=0;jj<nAddLength;jj++,ii++) {
			pTempLadder[ii].Zero();
			pTempLadder[ii].nProfileIndex = pnArray[jj];
		}
		for(ii,jj=nStartIndex;jj<m_nPlayListL;jj++,ii++) {
			pTempLadder[ii] = m_pPlayList[jj];
		}

		m_nPlayListL += nAddLength;

		SAFE_DELETE_ARRAY( m_pPlayList );

		m_pPlayList = new PlayListItem[ m_nPlayListL ];
		for(ii=0;ii<m_nPlayListL;ii++) {
			m_pPlayList[ii] = pTempLadder[ii];
		}
	}
}

int CWorkSpace::GetSize()
{
	return m_nProfileL;

}

int CWorkSpace::GetNextPlayListItem()
{
	if(m_nPlayListL <= 0) return -1;

	for(int ii=0;ii<m_nPlayListL;ii++) {
		if((m_pPlayList[ii].nStatus & _FINISHED) == 0) {
			return ii;
		}
	}
	return -1;

}

void CWorkSpace::EraseAllLadderFileNames()
{
	for(int ii=0;ii<m_nPlayListL;ii++) {
		m_pPlayList[ii].strComponentFileName.Empty();
		m_pPlayList[ii].nStatus = 0;;
	}

}

void CWorkSpace::ClearAllPlayListStandards()
{
	for(int ii=0;ii<m_nPlayListL;ii++) {
		m_pPlayList[ii].nUsageMask &= ~_STANDARD;
	}
}

bool CWorkSpace::IsCalibrationExpired()
{
	
	int nDiff;

	CTime CurrentTime = CTime::GetCurrentTime();
	nDiff = (int)((theApp.m_LastSettings.osLastStandardCalibration + theApp.m_LastSettings.osTimeBetweenStandards) - CurrentTime.GetTime());

	if(nDiff <= 0) return true;
	return false;
}

int CWorkSpace::TimeTillCalibration(CString *pBuff)
{
	
	int nDiff;
	CString Buff;
	CTime CurrentTime = CTime::GetCurrentTime();

	nDiff = (int)((theApp.m_LastSettings.osLastStandardCalibration + theApp.m_LastSettings.osTimeBetweenStandards) - CurrentTime.GetTime());
	CTimeSpan TimeDiff(nDiff);
	int nNumberMinutes = TimeDiff.GetMinutes();
	int nNumberHours = TimeDiff.GetHours();

	if(nDiff <= 0) {
		pBuff->LoadString(IDS_Expired);
	} else {
		pBuff->Format(L"%dh %02dm",nNumberHours, nNumberMinutes);
	}

	return nDiff;
}

time_t CWorkSpace::SetCalibrationTime()
{
	
	CTime CurrentTime = CTime::GetCurrentTime();

	return theApp.m_LastSettings.osLastStandardCalibration = CurrentTime.GetTime();
}

bool CWorkSpace::IsPartitionScan()
{
	if(m_nScanMethod == _PARTITION) return true;
	return false;
}

void CWorkSpace::CalculateDangerZone()
{
	
	int nP;

	if(m_nProfileL<=0) {
		PROFILE->CalculateDangerZone();
		return;
	}


	for(nP=0;nP < m_nProfileL; nP++) {
		m_pProfileArray[nP].CalculateDangerZone();
	}

	CCoord CpMin = m_pProfileArray[0].m_CpDanger[DANGER_MINS];
	CCoord CpMax = m_pProfileArray[0].m_CpDanger[DANGER_MAXS];
	for(nP=1;nP < m_nProfileL; nP++) {
		if(CpMin.Side0.fX > m_pProfileArray[nP].m_CpDanger[DANGER_MINS].Side0.fX) CpMin.Side0.fX = m_pProfileArray[nP].m_CpDanger[DANGER_MINS].Side0.fX;
		if(CpMin.Side0.fY > m_pProfileArray[nP].m_CpDanger[DANGER_MINS].Side0.fY) CpMin.Side0.fY = m_pProfileArray[nP].m_CpDanger[DANGER_MINS].Side0.fY;
		if(CpMin.Side0.fZ > m_pProfileArray[nP].m_CpDanger[DANGER_MINS].Side0.fZ) CpMin.Side0.fZ = m_pProfileArray[nP].m_CpDanger[DANGER_MINS].Side0.fZ;
		if(CpMin.Side1.fX > m_pProfileArray[nP].m_CpDanger[DANGER_MINS].Side1.fX) CpMin.Side1.fX = m_pProfileArray[nP].m_CpDanger[DANGER_MINS].Side1.fX;
		if(CpMin.Side1.fY > m_pProfileArray[nP].m_CpDanger[DANGER_MINS].Side1.fY) CpMin.Side1.fY = m_pProfileArray[nP].m_CpDanger[DANGER_MINS].Side1.fY;
		if(CpMin.Side1.fZ > m_pProfileArray[nP].m_CpDanger[DANGER_MINS].Side1.fZ) CpMin.Side1.fZ = m_pProfileArray[nP].m_CpDanger[DANGER_MINS].Side1.fZ;

		if(CpMax.Side0.fX < m_pProfileArray[nP].m_CpDanger[DANGER_MAXS].Side0.fX) CpMax.Side0.fX = m_pProfileArray[nP].m_CpDanger[DANGER_MAXS].Side0.fX;
		if(CpMax.Side0.fY < m_pProfileArray[nP].m_CpDanger[DANGER_MAXS].Side0.fY) CpMax.Side0.fY = m_pProfileArray[nP].m_CpDanger[DANGER_MAXS].Side0.fY;
		if(CpMax.Side0.fZ < m_pProfileArray[nP].m_CpDanger[DANGER_MAXS].Side0.fZ) CpMax.Side0.fZ = m_pProfileArray[nP].m_CpDanger[DANGER_MAXS].Side0.fZ;
		if(CpMax.Side1.fX < m_pProfileArray[nP].m_CpDanger[DANGER_MAXS].Side1.fX) CpMax.Side1.fX = m_pProfileArray[nP].m_CpDanger[DANGER_MAXS].Side1.fX;
		if(CpMax.Side1.fY < m_pProfileArray[nP].m_CpDanger[DANGER_MAXS].Side1.fY) CpMax.Side1.fY = m_pProfileArray[nP].m_CpDanger[DANGER_MAXS].Side1.fY;
		if(CpMax.Side1.fZ < m_pProfileArray[nP].m_CpDanger[DANGER_MAXS].Side1.fZ) CpMax.Side1.fZ = m_pProfileArray[nP].m_CpDanger[DANGER_MAXS].Side1.fZ;
	}
	for(nP=0;nP < m_nProfileL; nP++) {
		m_pProfileArray[nP].m_CpDanger[DANGER_MINS] = CpMin;
		m_pProfileArray[nP].m_CpDanger[DANGER_MAXS] = CpMax;
	}
}

void CWorkSpace::ResetAllStatus()
{
	for(int ii=0;ii<m_nPlayListL;ii++) {
		m_pPlayList[ii].nStatus = 0;;
	}


}

bool CWorkSpace::AddToRefArrayAndCheck(float fValue)
{
	
	CString Buff;

	float fMax = theApp.m_WS.m_fAttenuationRange[1] + theApp.m_WS.m_fdBRefLevel;
	float fMin = theApp.m_WS.m_fAttenuationRange[0] + theApp.m_WS.m_fdBRefLevel;

	m_fdBRefLevelArray[theApp.m_WS.m_ndBRefLevelL] = fValue;
	m_ndBRefLevelL++;

	if((fValue > fMax) || (fValue < fMin)) {
		Buff.Format(L"Auto Calibration %.01f dB Failed",fValue);
		theApp.ActivityLog(Buff);
		return false;
	}
	Buff.Format(L"Auto Calibration %.01f dB",fValue);
	theApp.ActivityLog(Buff);
	return true;

}

void CWorkSpace::CheckSwitchOffPumps()
{
	

	if(m_nPlayListL>0) {

		if(theApp.m_LastSettings.bWSPumpsSwitchOff == true) {
			if(m_pPlayList[m_nPlayListL - 1].nStatus >= _FINISHED) {
				theApp.m_Motors.PumpStop(theApp.m_Tank.nRightPump, 1);
				theApp.m_Motors.PumpStop(theApp.m_Tank.nLeftPump, 0);
			}
		}
	}
}
