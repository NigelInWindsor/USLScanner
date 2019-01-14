// USLScans.cpp: implementation of the CUSLScans class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "USLScans.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//maximum of 1024 data files in one file

WCHAR USLScansIdentifier[] = L"Ultrasonic Sciences Ltd USLScanner Multiple Data files";

CUSLScans::CUSLScans()
{

	Zero();
}

CUSLScans::~CUSLScans()
{

}

void CUSLScans::Zero()
{
	m_FileName.Empty();
	m_FilePath.Empty();

	m_nDataL = 0;
	ZeroMemory(m_n64DataOffsetArray, sizeof m_n64DataOffsetArray);
}

void CUSLScans::operator = (CUSLScans* pScans)
{
	m_nDataL = pScans->m_nDataL;
	m_FileName = pScans->m_FileName;
	m_FilePath = pScans->m_FilePath;
	CopyMemory(m_n64DataOffsetArray, pScans->m_n64DataOffsetArray, sizeof m_n64DataOffsetArray);
}

bool CUSLScans::Save(CUSLFile *pFile, CData *pData)
{
	
	UINT64 n64VariablesOffset,n64DataFilesOffset;

	if(pFile->GetLength()<16384) {
		pFile->ZeroBlockOfMemory(0,16384);
		Zero();

		m_FileName = pFile->GetFileName();
		m_FilePath = pFile->GetFilePath();

		pFile->Write(USLScansIdentifier,sizeof USLScansIdentifier);
		pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);

		pFile->Seek(0x100,CUSLFile::begin);
		pFile->Write(&n64VariablesOffset, sizeof n64VariablesOffset);
		pFile->Write(&n64DataFilesOffset, sizeof n64DataFilesOffset);

		pFile->Seek(n64VariablesOffset=pFile->GetPosition()+0x100,CUSLFile::begin);
		pFile->Write(&m_nDataL, sizeof m_nDataL);

		pFile->Seek(n64DataFilesOffset=pFile->GetPosition()+0x100,CUSLFile::begin);
		pFile->Write(m_n64DataOffsetArray, sizeof m_n64DataOffsetArray);

	} else {
	
		pFile->Seek(0x100,CUSLFile::begin);
		pFile->Read(&n64VariablesOffset, sizeof n64VariablesOffset);
		pFile->Read(&n64DataFilesOffset, sizeof n64DataFilesOffset);

		pFile->Seek(n64VariablesOffset,CUSLFile::begin);
		pFile->Read(&m_nDataL, sizeof m_nDataL);

		pFile->Seek(n64DataFilesOffset,CUSLFile::begin);
		pFile->Read(m_n64DataOffsetArray, sizeof m_n64DataOffsetArray);
	}


	pFile->SeekToEnd();
	m_n64DataOffsetArray[m_nDataL++] = pFile->GetPosition() + 0x100;

	pData->Save(pFile);
	
	pFile->Seek(0x100,CUSLFile::begin);
	pFile->Write(&n64VariablesOffset, sizeof n64VariablesOffset);
	pFile->Write(&n64DataFilesOffset, sizeof n64DataFilesOffset);

	pFile->Seek(n64VariablesOffset,CUSLFile::begin);
	pFile->Write(&m_nDataL, sizeof m_nDataL);

	pFile->Seek(n64DataFilesOffset,CUSLFile::begin);
	pFile->Write(m_n64DataOffsetArray, sizeof m_n64DataOffsetArray);

	return true;
}


bool CUSLScans::Initialize(CString *pFilePath)
{
	WCHAR wStr[100];
	float fVersion;
	UINT64 n64VariablesOffset,n64DataFilesOffset;

	Zero();

	m_FilePath = *pFilePath;

	CUSLFile File;
	CFileException e;
	CString Buff, Language;

	if(!File.Open(m_FilePath, CFile::modeRead | CFile::typeBinary,&e)) {
		File.DecodeFileException(&e,&Buff,m_FilePath);
		Language.LoadString(IDS_Error);
		MessageBox(NULL,Buff,Language,MB_ICONERROR);
		return false;
	} else {
		File.Read(wStr,sizeof USLScansIdentifier);
		if(wcscmp(wStr,USLScansIdentifier)!=0) {
			File.Close();
			return false;
		}

		File.Read(&fVersion,sizeof fVersion);
	
		File.Seek(0x100,CUSLFile::begin);
		File.Read(&n64VariablesOffset, sizeof n64VariablesOffset);
		File.Read(&n64DataFilesOffset, sizeof n64DataFilesOffset);

		File.Seek(n64VariablesOffset,CUSLFile::begin);
		File.Read(&m_nDataL, sizeof m_nDataL);

		File.Seek(n64DataFilesOffset,CUSLFile::begin);
		File.Read(m_n64DataOffsetArray, sizeof m_n64DataOffsetArray);

		File.Close();
	}
	return true;
}
