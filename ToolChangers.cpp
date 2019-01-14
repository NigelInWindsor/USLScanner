// ToolChangers.cpp: implementation of the CToolChangers class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "ToolChangers.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WCHAR ToolChangerIdentifier[]={L"Ultrasonic Sciences Ltd USLScanner Tool Changers"};

CToolChangers::CToolChangers()
{
	Zero();

}

CToolChangers::~CToolChangers()
{

}


void CToolChangers::Zero()
{

	m_nSideID[0] = m_nSideID[1] = -1;
	m_nEndEffectorSize = 0;

}

void CToolChangers::InitializeTypeStrings()
{
	if(m_strType[0].IsEmpty()) {
		m_strType[0] = L"N/A";
		m_strType[1].LoadString(IDS_Single_probe);
		m_strType[2].LoadString(IDS_Coaxial_probes);
		m_strType[3].LoadString(IDS_Adjacent_probes);
		m_strType[4].LoadString(IDS_Phased_array_flat);
		m_strType[5].LoadString(IDS_Phased_array_curved);
		m_strType[6].LoadString(IDS_Finger_manipulator);
		m_strType[7].LoadString(IDS_Arc_multi_position);
		m_strType[8].LoadString(IDS_Rotate_about_single_probe);
		m_strType[9].LoadString(IDS_Rotate_about_coaxial_probe);
	}
}


HRESULT CToolChangers::Retrieve(CString FilePath)
{
	CFileException e;
	CUSLFile file;
	CString Buff;
	HRESULT hr = false;

	if (!file.Open(FilePath, CFile::modeRead | CFile::typeBinary, &e)) {
		file.DecodeFileException(&e, &Buff, FilePath);
		MessageBox(NULL, Buff, _T("Workspace error"), MB_ICONERROR);
		return false;
	}
	else {
		hr = Retrieve(&file);
		file.Close();
		return hr;
	}
	return false;
}

HRESULT CToolChangers::Save(CString FilePath)
{
	CUSLFile File;
	CFileException e;
	CString Buff;

	if (!File.Open(FilePath, CFile::modeCreate | CFile::typeBinary | CFile::modeReadWrite, &e)) {
		File.DecodeFileException(&e, &Buff, FilePath);
		MessageBox(NULL, Buff, _T("Warning"), MB_ICONERROR);
		return FALSE;
	}
	else {
		Save(&File);
		File.Close();
		return TRUE;
	}
	return FALSE;
}


HRESULT CToolChangers::Save(CUSLFile* pFile)
{
	
	unsigned int nEndEffectorOffset[32];
	char Array[1000];
	ZeroMemory(Array, sizeof Array);

	unsigned int nStartOffset = (int)pFile->GetPosition();
	pFile->Write(Array, sizeof Array);
	pFile->Seek(nStartOffset,CFile::begin);

	pFile->Write(ToolChangerIdentifier,sizeof ToolChangerIdentifier);
	pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);

	pFile->Seek(0x500 + nStartOffset,CFile::begin);
	pFile->Write(&m_nEndEffectorSize, sizeof m_nEndEffectorSize);

	for(int nIndex=0;nIndex<32;nIndex++) {
		pFile->Seek(nEndEffectorOffset[nIndex]=(int)pFile->GetPosition()+0x100,CFile::begin);
		m_EndEffector[nIndex].Save(pFile);
	}

	int nFinishOffset = (int)pFile->GetPosition();

	pFile->Seek(0x100 + nStartOffset,CFile::begin);
	pFile->Write(nEndEffectorOffset, sizeof nEndEffectorOffset);

	pFile->Seek(nFinishOffset, CFile::begin);

	return true;
}

HRESULT CToolChangers::Retrieve(CUSLFile* pFile)
{
	
	unsigned int nEndEffectorOffset[32];
	WCHAR Identifier[0x100];
	float fVersion;

	ULONGLONG nStartOffset = pFile->GetPosition();

	pFile->Read(Identifier,sizeof ToolChangerIdentifier);
	pFile->Read(&fVersion,sizeof fVersion);

	if(fVersion <= theApp.m_fVersion && wcscmp(Identifier,ToolChangerIdentifier)==0 ) {

		pFile->Seek(0x100 + nStartOffset,CFile::begin);
		pFile->Read(nEndEffectorOffset, sizeof nEndEffectorOffset);

		pFile->Seek(0x500 + nStartOffset,CFile::begin);
		pFile->Read(&m_nEndEffectorSize, sizeof m_nEndEffectorSize);

		for(int nIndex=0;nIndex<32;nIndex++) {
			pFile->Seek(nEndEffectorOffset[nIndex],CFile::begin);
			m_EndEffector[nIndex].Retrieve(pFile);
		}

		return true;
	}
	return false;
}

int CToolChangers::AddEmptyEndEffector()
{

	if(m_nEndEffectorSize<32) m_nEndEffectorSize++;
	m_EndEffector[m_nEndEffectorSize-1].Zero();

	return m_nEndEffectorSize;
}

int CToolChangers::getEndEffectorSize()
{

	return m_nEndEffectorSize;
}

CString &CToolChangers::getDescription(int nIndex)
{
	static CString strEmptyDescription;

	if(nIndex>=0 && nIndex<m_nEndEffectorSize) {
		return m_EndEffector[nIndex].m_Description;
	}
	return strEmptyDescription;
}

CString *CToolChangers::getPtrDescription(int nIndex)
{

	return &m_EndEffector[nIndex].m_Description;
}

CString &CToolChangers::getType(int nIndex)
{
	static CString strNotAssigned;

	InitializeTypeStrings();

	if(nIndex>=0 && nIndex<m_nEndEffectorSize) {
		return m_strType[m_EndEffector[nIndex].m_Type];
	};
	strNotAssigned = L"N/A";
	return strNotAssigned;
}

int CToolChangers::setType(int nIndex, int nType)
{
	return (m_EndEffector[nIndex].m_Type = (EndEffectorType)nType);
}

int CToolChangers::getUniqueID(int nIndex)
{

	return m_EndEffector[nIndex].m_nUniqueID;
}

int *CToolChangers::getPtrUniqueID(int nIndex)
{

	return &m_EndEffector[nIndex].m_nUniqueID;
}

int *CToolChangers::getPtrType(int nIndex)
{

	return (int*)&m_EndEffector[nIndex].m_Type;
}

int CToolChangers::getSide(int nIndex)
{
	if(nIndex>=0 && nIndex<32) {
		return m_EndEffector[nIndex].m_nSide;
	}
	return -1;
}

int *CToolChangers::getPtrSide(int nIndex)
{

	return &m_EndEffector[nIndex].m_nSide;
}

float CToolChangers::getLength(int nIndex)
{

	return m_EndEffector[nIndex].m_fOverallLength;
}

float *CToolChangers::getPtrLength(int nIndex)
{

	return &m_EndEffector[nIndex].m_fOverallLength;
}

CString& CToolChangers::getCoordAtParkPos(int nIndex)
{
	static CString strCoord;

	strCoord.Format(L"X:%.0f Y:%.0f Z:%.0f, i:%.03f j:%.03f k:%.03f",
		m_EndEffector[nIndex].m_vParkPosition.x,m_EndEffector[nIndex].m_vParkPosition.y,m_EndEffector[nIndex].m_vParkPosition.z,
		m_EndEffector[nIndex].m_vectAtParkPosition.x,m_EndEffector[nIndex].m_vectAtParkPosition.y,m_EndEffector[nIndex].m_vectAtParkPosition.z);


	return strCoord;
}

CString& CToolChangers::getCoordAtParkPos(int nIndex,char cAxis)
{
	static CString strCoord;

	switch(cAxis) {
	case 'X':	strCoord.Format(L"%.0f",m_EndEffector[nIndex].m_vParkPosition.x);
		break;
	case 'Y':	strCoord.Format(L"%.0f",m_EndEffector[nIndex].m_vParkPosition.y);
		break;
	case 'Z':	strCoord.Format(L"%.0f",m_EndEffector[nIndex].m_vParkPosition.z);
		break;
	case 'i':	strCoord.Format(L"%.03f",m_EndEffector[nIndex].m_vectAtParkPosition.x);
		break;
	case 'j':	strCoord.Format(L"%.03f",m_EndEffector[nIndex].m_vectAtParkPosition.y);
		break;
	case 'k':	strCoord.Format(L"%.03f",m_EndEffector[nIndex].m_vectAtParkPosition.z);
		break;
	}


	return strCoord;
}

CString& CToolChangers::getClampToolTipLength(int nIndex, float *fLength)
{
	static CString strLength;

	strLength.Format(L"%.01f",*fLength = m_EndEffector[nIndex].getClampToolTipLength());

	return strLength;
}

float CToolChangers::getClampToolTipLength(int nID)
{

	for(int nIndex=0;nIndex<m_nEndEffectorSize;nIndex++) {
		if(nID == m_EndEffector[nIndex].m_nUniqueID) {

			return m_EndEffector[nIndex].getClampToolTipLength();
		}
	}

	return 0.0f;
}

void CToolChangers::setClampToolTipLength(int nIndex, float fEdit)
{
	if(nIndex>=0 && nIndex<m_nEndEffectorSize) {
		m_EndEffector[nIndex].setClampToolTipLength(fEdit);
	}

}

void CToolChangers::RefreshParkPosition(int nIndex)
{
	
	CCoord Cp;
	int nSide = m_EndEffector[nIndex].m_nSide;

	theApp.m_Motors.GetEncoderPos(&Cp);
	theApp.m_Kinematics.NormalsFromTips(&Cp, nSide);

	m_EndEffector[nIndex].m_vParkPosition = Cp.Side[nSide].pt;
	m_EndEffector[nIndex].m_vectAtParkPosition = Cp.Side[nSide].norm;

}

void CToolChangers::getParkPos(int nIndex, CCoord *pCp)
{
	int nSide = m_EndEffector[nIndex].m_nSide;

	pCp->Side[nSide].pt = m_EndEffector[nIndex].m_vParkPosition;
	pCp->Side[nSide].norm = m_EndEffector[nIndex].m_vectAtParkPosition;

}

CString& CToolChangers::getDescriptionFromID(int nID, int *nStatus)
{
	static CString strNoEndEffector;

	for(int nIndex=0;nIndex<m_nEndEffectorSize;nIndex++) {
		if(m_EndEffector[nIndex].m_nUniqueID == nID) {
			*nStatus = 1;
			return m_EndEffector[nIndex].m_Description;
		}
	}

	*nStatus = 0;
	if(nID == 0) {
		*nStatus = 0;
		strNoEndEffector.LoadString(IDS_No_Endeffector);
	} else {
		*nStatus = -1;
		strNoEndEffector.LoadString(IDS_Endeffector_not_recognised);
	}
	return strNoEndEffector;
}

CString& CToolChangers::getTypeFromID(int nID, int *nStatus)
{
	static CString strNoEndEffector;

	for(int nIndex=0;nIndex<m_nEndEffectorSize;nIndex++) {
		if(m_EndEffector[nIndex].m_nUniqueID == nID) {
			if(nStatus) *nStatus = 1;
			return m_strType[m_EndEffector[nIndex].m_Type];
		}
	}

	if(nID == 0) {
		if(nStatus) *nStatus = 0;
		strNoEndEffector.LoadString(IDS_No_Endeffector);
	} else {
		if(nStatus) *nStatus = -1;
		strNoEndEffector.LoadString(IDS_Endeffector_not_recognised);
	}
	return strNoEndEffector;
}

int CToolChangers::getTypeFromID(int nID)
{

	for(int nIndex=0;nIndex<m_nEndEffectorSize;nIndex++) {
		if(m_EndEffector[nIndex].m_nUniqueID == nID) {
			return m_EndEffector[nIndex].m_Type;
		}
	}
	return 0;
}

void CToolChangers::Sort(int nColumn, int nDir)
{
	int n = m_nEndEffectorSize;

	for(int i=0;i<n;i++) {
		for(int j=0;j<n-i-1;j++){

			switch(nColumn) {
			case 2:
				if(nDir ==0) {
					if(m_EndEffector[j].m_nUniqueID > m_EndEffector[j+1].m_nUniqueID) SwapEndEffector(j, j+1);
				} else {
					if(m_EndEffector[j].m_nUniqueID < m_EndEffector[j+1].m_nUniqueID) SwapEndEffector(j, j+1);
				}
				break;
			}
		}
	}
}

void CToolChangers::SwapEndEffector(int j, int k)
{
	CEndEffector tempEndEffector;

	tempEndEffector = &m_EndEffector[j];
	m_EndEffector[j] = &m_EndEffector[k];
	m_EndEffector[k] = tempEndEffector;
}

void CToolChangers::Delete(int nIndex)
{
	if(m_nEndEffectorSize > 1) {
		for(int ii=nIndex;ii<(m_nEndEffectorSize - 1);ii++) {
			m_EndEffector[ii] = m_EndEffector[ii+1];
		}
	}
	m_nEndEffectorSize--;
	MinMax(&m_nEndEffectorSize,0,31);
}

CString& CToolChangers::getWaterPath(int nIndex, float *fLength)
{
	static CString strLength;

	strLength.Format(L"%.01f",*fLength = m_EndEffector[nIndex].getWaterPath());

	return strLength;
}

float CToolChangers::getWaterPath(int nID)
{

	for(int nIndex=0;nIndex<m_nEndEffectorSize;nIndex++) {
		if(nID == m_EndEffector[nIndex].m_nUniqueID) {

			return m_EndEffector[nIndex].getWaterPath();
		}
	}

	return 0.0f;
}

void CToolChangers::setWaterPath(int nIndex, float fEdit)
{
	if(nIndex>=0 && nIndex<m_nEndEffectorSize) {
		m_EndEffector[nIndex].setWaterPath(fEdit);
	}

}


void CToolChangers::InvalidateKinematicWaterPath(int nSide)
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	theApp.m_Kinematics.m_fDesiredWaterPath[nSide] = getWaterPath(theApp.m_FBCtrl.m_nToolID[nSide]);
	pFrame->SendMessage(UI_UPDATE_DIALOG_BAR);


}

int CToolChangers::getIndexFromID(int nID, int *nStatus)
{
	for(int nIndex=0; nIndex<m_nEndEffectorSize; nIndex++) {
		if(m_EndEffector[nIndex].m_nUniqueID == nID) {
			if(nStatus) *nStatus = 1;
			return nIndex;
		}
	}
	*nStatus = 0;
	return -1;

}

void CToolChangers::setProbeVector(int nIndex, D3DXVECTOR3* vProbeLength)
{

	m_EndEffector[nIndex].m_vProbeLength = *vProbeLength;
}

void CToolChangers::getProbeVector(int nIndex, D3DXVECTOR3* vProbeLength)
{

	*vProbeLength = m_EndEffector[nIndex].m_vProbeLength;
}



void CToolChangers::setArmVector(int nIndex, D3DXVECTOR3* vArmLength)
{

	m_EndEffector[nIndex].m_vClampToProbeRotate = *vArmLength;
}

void CToolChangers::getArmVector(int nIndex, D3DXVECTOR3* vArmLength)
{

	*vArmLength = m_EndEffector[nIndex].m_vClampToProbeRotate;
}

