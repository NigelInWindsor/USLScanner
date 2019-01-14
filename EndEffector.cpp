// EndEffector.cpp: implementation of the CEndEffector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "EndEffector.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WCHAR EndEffectorIdentifier[]={L"Ultrasonic Sciences Ltd USLScanner EndEffector"};

CEndEffector::CEndEffector()
{
	Zero();
}

CEndEffector::~CEndEffector()
{

}

void CEndEffector::Zero()
{
	m_Description.Empty();
	m_CarouselLabel.Empty();

	ZeroMemory(&m_vectAtParkPosition, sizeof m_vectAtParkPosition);
	ZeroMemory(&m_vParkPosition, sizeof m_vParkPosition);
	ZeroMemory(&m_vectAtToolTip, sizeof m_vectAtToolTip);
	ZeroMemory(&m_vClampToToolTip, sizeof m_vClampToToolTip);
	ZeroMemory(&m_vToolTipRotate, sizeof m_vToolTipRotate);
	ZeroMemory(&m_vClampToProbeRotate, sizeof m_vClampToProbeRotate);
	ZeroMemory(&m_vProbeLength, sizeof m_vProbeLength);

	m_fOverallLength = 0.0f;

	ZeroMemory(m_vProbePos, sizeof m_vProbePos);
	m_Type = SINGLE_PROBE;
	m_nNumberProbes = 1;
	m_nUniqueID = 0;
	m_nSide = 0;
	m_fWp = 0.0f;

}

HRESULT CEndEffector::Retrieve(CString FilePath)
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

HRESULT CEndEffector::Save(CString FilePath)
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


HRESULT CEndEffector::Save(CUSLFile* pFile)
{
	char Array[1000];
	ZeroMemory(Array, sizeof Array);

	unsigned int nStartOffset = (unsigned int)pFile->GetPosition();
	pFile->Write(Array, sizeof Array);
	pFile->Seek(nStartOffset,CFile::begin);

	pFile->Write(EndEffectorIdentifier,sizeof EndEffectorIdentifier);
	pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);

	pFile->Seek(0x100 + nStartOffset,CFile::begin);

	CArchive* pAr = new CArchive(pFile,CArchive::store);
	pAr->WriteString(m_Description + L"\r\n");
	pAr->WriteString(m_CarouselLabel + L"\r\n");
	pAr->Close();
	SAFE_DELETE(pAr);

	pFile->Write(&m_vectAtParkPosition, sizeof m_vectAtParkPosition);
	pFile->Write(&m_vParkPosition, sizeof m_vParkPosition);
	pFile->Write(&m_vectAtToolTip, sizeof m_vectAtToolTip);
	pFile->Write(&m_vClampToToolTip, sizeof m_vClampToToolTip);
	pFile->Write(&m_vToolTipRotate, sizeof m_vToolTipRotate);
	pFile->Write(&m_fOverallLength, sizeof m_fOverallLength);

	pFile->Write(m_vProbePos, sizeof m_vProbePos);
	pFile->Write(&m_Type, sizeof m_Type);
	pFile->Write(&m_nNumberProbes, sizeof m_nNumberProbes);
	pFile->Write(&m_nUniqueID, sizeof m_nUniqueID);
	pFile->Write(&m_nSide, sizeof m_nSide);
	pFile->Write(&m_fWp, sizeof m_fWp);

	pFile->Write(&m_vClampToProbeRotate, sizeof m_vClampToProbeRotate);
	pFile->Write(&m_vProbeLength, sizeof m_vProbeLength);

	return true;
}

HRESULT CEndEffector::Retrieve(CUSLFile* pFile)
{
	float fVersion;
	WCHAR Identifier[0x100];

	unsigned int nStartOffset = (unsigned int)pFile->GetPosition();

	pFile->Read(Identifier,sizeof EndEffectorIdentifier);
	pFile->Read(&fVersion,sizeof fVersion);

	pFile->Seek(0x100 + nStartOffset,CFile::begin);

	CArchive* pAr = new CArchive(pFile,CArchive::load);
	pAr->ReadString(m_Description);
	pAr->ReadString(m_CarouselLabel);
	pAr->Close();
	SAFE_DELETE(pAr);

	pFile->Read(&m_vectAtParkPosition, sizeof m_vectAtParkPosition);
	pFile->Read(&m_vParkPosition, sizeof m_vParkPosition);
	pFile->Read(&m_vectAtToolTip, sizeof m_vectAtToolTip);
	pFile->Read(&m_vClampToToolTip, sizeof m_vClampToToolTip);
	pFile->Read(&m_vToolTipRotate, sizeof m_vToolTipRotate);
	pFile->Read(&m_fOverallLength, sizeof m_fOverallLength);

	pFile->Read(m_vProbePos, sizeof m_vProbePos);
	pFile->Read(&m_Type, sizeof m_Type);
	pFile->Read(&m_nNumberProbes, sizeof m_nNumberProbes);
	pFile->Read(&m_nUniqueID, sizeof m_nUniqueID);
	pFile->Read(&m_nSide, sizeof m_nSide);
	pFile->Read(&m_fWp, sizeof m_fWp);

	pFile->Read(&m_vClampToProbeRotate, sizeof m_vClampToProbeRotate);
	pFile->Read(&m_vProbeLength, sizeof m_vProbeLength);

	return true;
}


float CEndEffector::getClampToolTipLength()
{

	return D3DXVec3Length(&m_vClampToToolTip);
}

void CEndEffector::setClampToolTipLength(float fEdit)
{

	m_vClampToToolTip.x = 0.0f;
	m_vClampToToolTip.y = fEdit;
	m_vClampToToolTip.z = 0.0f;
}

float CEndEffector::getWaterPath()
{
	return m_fWp;
}

void CEndEffector::setWaterPath(float fValue)
{
	m_fWp = fValue;
}
