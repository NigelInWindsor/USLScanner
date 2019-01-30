// Data.cpp: implementation of the CData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "Data.h"
#include "GenericScanner.h"
#include "ProgressDlg.h"
#include "ViewSheet.h"
#include "MatecFileImport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CompareUnsignedChar(const void *Cp0,const void *Cp1);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define	INIT_VALUE	0x10

WCHAR wDataIdentifier[]=_T("Ultrasonic Sciences Ltd USLScanner Image Data");


CData::CData(CWnd* pParent)
{

	m_pParent = pParent;
	for(int ii=0;ii<NUMBER_BUFFERS;ii++) {
		m_cAmpBuffer[ii] = NULL;
		m_nTimeBuffer[ii] = NULL;
		m_cPhaseBuffer[ii] = NULL;
		m_cFilterBuffer[ii] = NULL;
		m_LineBlade[ii] = NULL;
		m_nLineBladeL[ii] = 0;
		m_16ExtBuffer[ii] = NULL;
	}
	for(int ii=0;ii<NUMBER_BSCAN_BUFFERS;ii++) {
		m_cBscanBuffer[ii] = NULL;
	}
	for(int ii=0;ii<NUMBER_VOLUME_BUFFERS;ii++) {
		for(int jj=0;jj<256;jj++) { //Multiple lines/probes
			m_cVolumeBuffer[ii][jj] = NULL;
		}
	}
	m_nLineScannedStatus = NULL;

	m_pARGBArray=NULL;
	m_pCpPos = NULL;
	m_Hist.nCoordL = 0;
	m_Hist.pnData=NULL;
	m_Hist.pfArea = NULL;
	ZeroMemory(&m_Hist,sizeof m_Hist);
	m_Hist.fLatchAmp = 100.0f;

	m_n64FileLength=0;
	ZeroMemory( m_n64VolumeOffset, sizeof m_n64VolumeOffset);
	m_n64ScanLinesOffset = 0;
	m_n64ToolPathOffset = 0;

	m_pAnnotation = NULL;
	m_nAnnotationL = 0;
	m_bModified = FALSE;
	m_bCScansSaved = FALSE;
	m_bVolScansSaved = FALSE;

	m_fLineLength = NULL;
	m_fSampleWidth = NULL;

	m_pTextureFace=NULL;
	m_nTextureFaceL=0;
	m_bThreadEnabled = false;
	m_bThreadFinished = false;

	m_pPolygon = NULL;
	m_nPolygonL=0;

	m_vLineSample = NULL;

	m_nCADElementL = 0;
	m_pCADElement = NULL;
	m_nCADPtr = 0;

	ZeroMemory(m_nLastPtr, sizeof m_nLastPtr);
	m_nFlattenMode = 0;

	m_nEnvelopeLineL = 0;
	m_pEnvelopeLine = NULL;

	m_nNumberProbesPerImage = 1;
	m_nFirstProbe = 0;

	m_nVolBytesPerSample = 0;

	m_nSentence = 0;

}

CData::~CData()
{
	Zero();
}

void CData::Zero()
{
	m_bModified = FALSE;
	m_bCScansSaved = FALSE;
	m_bVolScansSaved = FALSE;
	m_nNumberTimeSlots=1;
	m_nNumberImages=0;
	ZeroMemory(m_nNumberAmpImages,sizeof m_nNumberAmpImages);
	ZeroMemory(m_nNumberTimeImages,sizeof m_nNumberTimeImages);
	ZeroMemory(m_nNumberPhaseImages,sizeof m_nNumberPhaseImages);
	m_nNumberExtImages = 0;
	m_nNumberVolumeImages=0;
	m_n64FileLength=0;
	ZeroMemory( m_n64VolumeOffset, sizeof m_n64VolumeOffset);
	m_n64ScanLinesOffset = 0;
	m_n64ToolPathOffset = 0;
	m_bHeaderForVolumeIsValid = FALSE;
	m_nFlattenMode = 0;
	m_nNumberProbesPerImage = 1;
	m_nFirstProbe = 0;
	m_nSentence = 0;
	m_nVolBytesPerSample = 0;

	for(int ii=0;ii<NUMBER_BUFFERS;ii++) {
		SAFE_DELETE( m_cAmpBuffer[ii] );
		SAFE_DELETE( m_nTimeBuffer[ii] );
		SAFE_DELETE( m_cPhaseBuffer[ii] );
		SAFE_DELETE( m_cFilterBuffer[ii] );
		SAFE_DELETE_ARRAY( m_LineBlade[ii] );
		SAFE_DELETE( m_16ExtBuffer[ii] );
		m_nLineBladeL[ii] = 0;
	}
	for(int ii=0;ii<NUMBER_BSCAN_BUFFERS;ii++) {
		SAFE_DELETE( m_cBscanBuffer[ii] );
	}
	for(int ii=0;ii<NUMBER_VOLUME_BUFFERS;ii++) {
		for(int jj=0; jj<256; jj++){ //Multiple lines/probes
			SAFE_DELETE( m_cVolumeBuffer[ii][jj] );
		}
	}
	SAFE_DELETE( m_nLineScannedStatus );
	SAFE_DELETE_ARRAY( m_pCpPos );
	SAFE_DELETE_ARRAY( m_pCADElement );
	m_nCADElementL = 0;
	m_nCADPtr = 0;


	ZeroMemory(m_TS,sizeof m_TS);
	ZeroMemory(&m_Global,sizeof m_Global);
	CopyMemory(m_TS,&theApp.m_UtUser.m_TS, sizeof m_TS);
	CopyMemory(&m_Global,&theApp.m_UtUser.m_Global, sizeof m_Global);

	SAFE_DELETE( m_Hist.pnData );
	SAFE_DELETE(m_Hist.pfArea);
	m_Hist.nCoordL = 0;

	if(m_pAnnotation) delete m_pAnnotation;
	m_pAnnotation = NULL;
	m_nAnnotationL = 0;
	SAFE_DELETE(m_pARGBArray);
	SAFE_DELETE_ARRAY(m_pTextureFace);
	m_nTextureFaceL = 0;

	m_nMinViewLine = 0;
	m_nMaxViewLine = 1000000;

	m_bThreadEnabled = false;
	m_bThreadFinished = false;

	SAFE_DELETE( m_fLineLength );
	SAFE_DELETE( m_fSampleWidth );

	SAFE_DELETE_ARRAY( m_pPolygon );
	m_nPolygonL = 0;

	SAFE_DELETE_ARRAY( m_vLineSample );

	SAFE_DELETE_ARRAY( m_pEnvelopeLine );

	for(int ii=0;ii<MAX_LINE_TYPES;ii++) {
		m_CpTranslation[ii].Zero();
	}

	for(int ii=0;ii<NUMBER_BUFFERS;ii++) m_ImageName[ii].Empty();
}


bool CData::Save()
{
	CUSLFile File;
	CFileException e;
	CString FilePathName;

	if(m_FileName.IsEmpty() == TRUE) return false;

	FilePathName = m_FilePath + L"\\" + m_FileName;
	if(FilePathName.Find(L".dat") < 0) {
		FilePathName += L".dat";
	}

	if(!File.Open(FilePathName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary | CFile::modeNoTruncate, &e)) {
		return false;
	} else {
		Save(&File);
		File.Close();
		MESSaveOEM();
	}
	return true;
}




int CData::Save(CUSLFile *pFile,bool bSaveJustHeader)
{
	int nOffsetArray[2048];
	ZeroMemory(nOffsetArray,sizeof nOffsetArray);

	UINT64 n64VariablesOffset,n64StringsOffset,n64GlobalOffset,n64TimeSlotOffset,n64DataOffset,n64AnnotationOffset,n64DetailsOffset,n64CoordOffset;
	UINT64 n64ImageNameOffset,n64PolygonOffset,n64LineSampleOffset,n64CADOffset,n64EnvelopeLineOffset,n64BladeOffset,n64BladeLineOffset[4];
	UINT64 n64TranslationOffset,n64InclusionsOffset;
	UINT64 n64LineLengthOffset;

	CArchive* pArchFile;
	CString Buff;
	int nTS,nSize,gg,tt,pp,nn,nLine;


	n64VariablesOffset=0;
	n64StringsOffset=0;
	n64GlobalOffset=0;
	n64TimeSlotOffset=0;
	n64DataOffset=0;
	n64AnnotationOffset=0;
	//m_n64VolumeOffset,sizeof m_n64VolumeOffset);						//[8]
	n64DetailsOffset=0;
	n64CoordOffset=0;
	n64ImageNameOffset=0;
	m_n64ScanLinesOffset=0;
	m_n64ToolPathOffset=0;
	n64PolygonOffset=0;
	n64LineSampleOffset=0;
	n64CADOffset=0;
	n64EnvelopeLineOffset=0;
	n64BladeOffset=0;
	ZeroMemory(n64BladeLineOffset,sizeof n64BladeLineOffset);
	n64TranslationOffset=0;
	n64InclusionsOffset=0;
	n64LineLengthOffset;

	m_FileName = pFile->GetFileName();
	m_FilePath = pFile->GetFilePath();


	UINT64 n64StartPosition = pFile->GetPosition();

	pFile->Seek(n64StartPosition,CUSLFile::begin);
	pFile->Write(nOffsetArray,sizeof nOffsetArray);
	pFile->Seek(n64StartPosition,CUSLFile::begin);

	pFile->Write(wDataIdentifier,sizeof wDataIdentifier);
	pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);

	pFile->Seek(n64VariablesOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
	pFile->Write(&m_nSamplesLine,sizeof m_nSamplesLine);
	pFile->Write(&m_nNumberLines,sizeof m_nNumberLines);
	pFile->Write(&m_fFastScanSize,sizeof m_fFastScanSize);
	pFile->Write(&m_fSlowScanSize,sizeof m_fSlowScanSize);
	pFile->Write(&m_fFastIncrement,sizeof m_fFastIncrement);
	pFile->Write(&m_fSlowIncrement,sizeof m_fSlowIncrement);
	pFile->Write(&m_nNumberImages,sizeof m_nNumberImages);
	pFile->Write(&m_nNumberTimeSlots,sizeof m_nNumberTimeSlots);
	pFile->Write(m_nNumberAmpImages,sizeof m_nNumberAmpImages);
	pFile->Write(m_nNumberTimeImages,sizeof m_nNumberTimeImages);
	pFile->Write(m_nNumberPhaseImages,sizeof m_nNumberPhaseImages);
	pFile->Write(&m_nSentence,sizeof m_nSentence);
	pFile->Write(&m_osTestDateTime,sizeof m_osTestDateTime);
	pFile->Write(&m_nNumberVolumeImages,sizeof m_nNumberVolumeImages);
	pFile->Write(&m_nScanStartLine,sizeof m_nScanStartLine);
	pFile->Write(&m_nScanFinishLine,sizeof m_nScanFinishLine);
	pFile->Write(&m_nLineIncrement,sizeof m_nLineIncrement);
	pFile->Write(&m_nComponentDescription,sizeof m_nComponentDescription);
	pFile->Write(&m_nScanType,sizeof m_nScanType);
	pFile->Write(m_fComponentDiameter,sizeof m_fComponentDiameter);
	pFile->Write(&m_nFastAxis,sizeof m_nFastAxis);
	pFile->Write(&m_nSlowAxis,sizeof m_nSlowAxis);
	pFile->Write(&m_nNumberProbesPerImage,sizeof m_nNumberProbesPerImage);
	pFile->Write(&m_nLineIncrementMode,sizeof m_nLineIncrementMode);
	pFile->Write(&m_nCollectScanLines,sizeof m_nCollectScanLines);
	pFile->Write(&m_nCollectSurface,sizeof m_nCollectSurface);
	pFile->Write(&m_nFlattenMode,sizeof m_nFlattenMode);
	pFile->Write(&m_matCadToScan,sizeof m_matCadToScan);
	pFile->Write(&m_nFirstProbe,sizeof m_nFirstProbe);
	pFile->Write(&m_nNumberExtImages,sizeof m_nNumberExtImages);
	pFile->Write(&m_nLogRange,sizeof m_nLogRange);
	pFile->Write(&vTTCentre, sizeof vTTCentre);

	if(bSaveJustHeader == false) {
		pFile->Seek(n64CoordOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
		m_CpStart.Save(pFile);
		m_CpFinish.Save(pFile);
		m_CpTurnTableCentre.Save(pFile);

		pFile->Seek(n64StringsOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
		pArchFile = new CArchive(pFile,CArchive::store);
		pArchFile->WriteString(m_Operator + _T("\r\n"));
		pArchFile->WriteString(m_TestDateTime + _T("\r\n"));
		pArchFile->WriteString(m_ProfileName + _T("\r\n"));
		pArchFile->WriteString(m_UniqueIdentification + _T("\r\n"));
		pArchFile->WriteString(m_MachineName + _T("\r\n"));
		pArchFile->WriteString(m_MachineLocation + _T("\r\n"));
		pArchFile->Close();
		delete pArchFile;

		pFile->Seek(n64GlobalOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
		pFile->WriteStruct(&m_Global, m_Global.nStructSize=sizeof m_Global);

		pFile->Seek(n64TimeSlotOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
		for(nTS=0;nTS<256;nTS++) {
			pFile->WriteStruct(&m_TS[nTS], m_TS[nTS].nStructSize=sizeof m_TS[0]);
			pFile->Seek(0x200,CUSLFile::current);
		}


		pFile->Seek(n64DataOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
		nSize = m_nSamplesLine * m_nNumberLines;
		gg=tt=pp=0;
		for(nTS=0;nTS<m_nNumberTimeSlots;nTS++) {
			for(nn=0;nn<m_nNumberAmpImages[nTS];nn++) {
				pFile->Write(m_cAmpBuffer[gg++],nSize * sizeof (unsigned char));
			}
			for(nn=0;nn<m_nNumberTimeImages[nTS];nn++) {
				pFile->Write(m_nTimeBuffer[tt++],nSize * sizeof (unsigned int));
			}
			for(nn=0;nn<m_nNumberPhaseImages[nTS];nn++) {
				pFile->Write(m_cPhaseBuffer[pp++],nSize * sizeof (unsigned char));
			}
		}
		for(nn=0;nn<m_nNumberExtImages;nn++) {
			pFile->Write(m_16ExtBuffer[nn],nSize * sizeof (unsigned __int16));
		}
		m_bCScansSaved = true;

		pFile->Seek(n64AnnotationOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
		pFile->Write(&m_nAnnotationL,sizeof m_nAnnotationL);
		for(nn=0;nn<m_nAnnotationL;nn++) {
			pFile->WriteStruct(&m_pAnnotation[nn],m_pAnnotation[nn].nStructSize=sizeof ANNOTATION);
		}
		
		pFile->Seek(0x200,CUSLFile::end);
		n64DetailsOffset=pFile->GetPosition();
		m_DataProperties.Save(pFile);

		pFile->Seek(0x200,CUSLFile::end);
		n64ImageNameOffset=pFile->GetPosition();
		pFile->Write(&m_nImageNameL,sizeof m_nImageNameL);
		pArchFile = new CArchive(pFile,CArchive::store);
		for(nn=0;nn<m_nImageNameL;nn++) {
			pArchFile->WriteString(m_ImageName[nn]+_T("\r\n"));
		}
		pArchFile->Close();
		delete pArchFile;

		pFile->Seek(0x200,CUSLFile::end);
		n64PolygonOffset=pFile->GetPosition();
		pFile->Write(&m_nPolygonL,sizeof m_nPolygonL);
		for(nn=0;nn<m_nPolygonL;nn++) {
			m_pPolygon[nn].Save(pFile);
		}

		if(m_vLineSample) {
			pFile->Seek(0x200,CUSLFile::end);
			n64LineSampleOffset=pFile->GetPosition();
			pFile->Write(&m_nNumberLines,sizeof m_nNumberLines);
			for(nn=0;nn<m_nNumberLines;nn++) {
				m_vLineSample[nn].Save(pFile);
			}
		} else {
			n64LineSampleOffset = 0;
		}

		if(m_nCADElementL>0) {
			pFile->Seek(0x200,CUSLFile::end);
			n64CADOffset=pFile->GetPosition();
			pFile->Write(&m_nCADElementL,sizeof m_nCADElementL);
			for(nn=0;nn<m_nCADElementL;nn++) {
				m_pCADElement[nn].Save(pFile);
			}
		} else {
			n64CADOffset = 0;
		}

		//Write the Envelopelines
		pFile->Seek(0x200,CUSLFile::end);
		n64EnvelopeLineOffset=pFile->GetPosition();
		pFile->Write(&m_nEnvelopeLineL,sizeof m_nEnvelopeLineL);
		for(nLine = 0;nLine < m_nEnvelopeLineL; nLine++) m_pEnvelopeLine[nLine].Save(pFile);

		pFile->Seek(0x200,CUSLFile::end);
		n64BladeOffset=pFile->GetPosition();
		for(int nCad=0;nCad<2;nCad++) {
			pFile->Seek(n64BladeLineOffset[nCad]=pFile->GetPosition()+0x200,CUSLFile::begin);
			pFile->Write(&m_nLineBladeL[nCad],sizeof m_nLineBladeL[nCad]);
			for(int nLine=0;nLine<m_nLineBladeL[nCad];nLine++) m_LineBlade[nCad][nLine].Save(pFile);
		};

		pFile->Seek(n64TranslationOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
		for(nn=0;nn<MAX_LINE_TYPES;nn++) {
			m_CpTranslation[nn].Save(pFile);
		};

		pFile->Seek(n64InclusionsOffset = pFile->GetPosition() + 0x200, CUSLFile::begin);
		m_Inclusions.Save(pFile);

		pFile->Seek(n64LineLengthOffset = pFile->GetPosition() + 0x200, CUSLFile::begin);
		pFile->Write(m_fLineLength, m_nNumberLines * sizeof(float));
		pFile->Write(m_fSampleWidth, m_nNumberLines * sizeof(float));


		AppendDataBase();
		theApp.ActivityLog(L"Image saved :" + m_FilePath);

	}

	//Write the offsets
	pFile->Seek(0x100,CUSLFile::begin);
	pFile->Write(nOffsetArray,0x100);
	pFile->Seek(0x100,CUSLFile::begin);
	pFile->Write(&n64VariablesOffset,sizeof n64VariablesOffset);
	pFile->Write(&n64StringsOffset,sizeof n64StringsOffset);
	pFile->Write(&n64GlobalOffset,sizeof n64GlobalOffset);
	pFile->Write(&n64TimeSlotOffset,sizeof n64TimeSlotOffset);
	pFile->Write(&n64DataOffset,sizeof n64DataOffset);
	pFile->Write(&n64AnnotationOffset,sizeof n64AnnotationOffset);
	pFile->Write(m_n64VolumeOffset,sizeof m_n64VolumeOffset);						//[8]
	pFile->Write(&n64DetailsOffset,sizeof n64DetailsOffset);
	pFile->Write(&n64CoordOffset,sizeof n64CoordOffset);
	pFile->Write(&n64ImageNameOffset,sizeof n64ImageNameOffset);
	pFile->Write(&m_n64ScanLinesOffset,sizeof m_n64ScanLinesOffset);
	pFile->Write(&m_n64ToolPathOffset,sizeof m_n64ToolPathOffset);
	pFile->Write(&n64PolygonOffset,sizeof n64PolygonOffset);
	pFile->Write(&n64LineSampleOffset,sizeof n64LineSampleOffset);
	pFile->Write(&n64CADOffset,sizeof n64CADOffset);
	pFile->Write(&n64EnvelopeLineOffset,sizeof n64EnvelopeLineOffset);
	pFile->Write(&n64BladeOffset,sizeof n64BladeOffset);
	pFile->Write(n64BladeLineOffset,sizeof n64BladeLineOffset);
	pFile->Write(&n64TranslationOffset,sizeof n64TranslationOffset);
	pFile->Write(&n64InclusionsOffset, sizeof n64InclusionsOffset);
	pFile->Write(&n64LineLengthOffset, sizeof n64LineLengthOffset);


	pFile->SeekToEnd();

	m_bModified = FALSE;
	m_bHeaderForVolumeIsValid = TRUE;

	return 0;
}

bool CData::Retrieve(CUSLFile *pFile)
{
	CString Buff;

	UINT64 n64StartPosition = pFile->GetPosition();

	m_FileName = pFile->GetFileName();
	m_FilePath = pFile->GetFilePath();


	theApp.ActivityLog(L"Image retrieve :" + m_FilePath);

/////////////////////////////////////////////////
//	UNICODE
	WCHAR wStr[100];
	ZeroMemory(wStr,sizeof wStr);

	pFile->Seek(n64StartPosition,CUSLFile::begin);
	pFile->Read(wStr,sizeof wDataIdentifier);
	if(wcscmp(wStr,wDataIdentifier)==0) {
		pFile->Seek(n64StartPosition,CUSLFile::begin);
		return RetrieveCompatibleFile(pFile);
	}

////////////////////////////////////////////////
//	ASCII

	char aIdentifier[60];
	char aStr[100];
	ZeroMemory(aStr,sizeof aStr);
	strcpy_s(aIdentifier,"Ultrasonic Sciences Ltd USLScanner Image Data");

	pFile->Seek(n64StartPosition,CUSLFile::begin);
	pFile->Read(aStr,strlen(aIdentifier));

	if(strcmp(aStr,aIdentifier)==0) {
		pFile->Seek(n64StartPosition,CUSLFile::begin);
		return RetrieveCompatibleFile(pFile);
	}


	if(strcmp(aStr,"Ultrasonic Sciences Ltd")==0) {
		return RetrieveGenericScannerFile(pFile);
	}


	ZeroMemory(aStr,sizeof aStr);
	strcpy_s(aIdentifier,"Ultrasonic Sciences Complex Geometry Data");
	pFile->Seek(n64StartPosition,CUSLFile::begin);
	pFile->Read(aStr,strlen(aIdentifier));

	if(strcmp(aStr,aIdentifier)==0) {
		return RetrieveComplexGeometryData(pFile);
	}


	Buff = (CString)aStr;
	if(m_FileName.Compare(Buff)==0) {
		return RetrieveFokkerDosFile(pFile);
	}

	CMatecFileImport Matec;
	if (Matec.Retrieve(pFile, this) == true) {
		return true;
	}

	MessageBox(NULL,L"This file doesn't have a recognised file format\r\n and can not be openned",L"Error",MB_ICONERROR);

	return FALSE;
}

bool CData::RetrieveCompatibleFile(CUSLFile *pFile)
{
	int nVariablesOffset,nStringsOffset,nGlobalOffset,nTimeSlotOffset,nDataOffset,nAnnotationOffset,nDetailsOffset,nCoordOffset,nImageNameOffset;
	UINT64 n64VariablesOffset,n64StringsOffset,n64GlobalOffset,n64TimeSlotOffset,n64DataOffset,n64AnnotationOffset,n64DetailsOffset,n64CoordOffset;
	UINT64 n64ImageNameOffset,n64PolygonOffset,n64LineSampleOffset,n64CADOffset,n64EnvelopeLineOffset,n64BladeOffset,n64BladeLineOffset[4];
	UINT64 n64TranslationOffset,n64InclusionsOffset,n64LineLengthOffset;
	CArchive* pArchFile;
	int nTS,gg,tt,pp,nn,nSize,nLine;
	float fVersion;
	CString Buff,Error;
	CCoord CpTemp;

	Zero();

	UINT64 n64StartPosition = pFile->GetPosition();

	WCHAR wStr[100];
	pFile->Seek(n64StartPosition,CUSLFile::begin);
	pFile->Read(wStr,sizeof wDataIdentifier);
	if(wcscmp(wStr,wDataIdentifier)!=0) {
		return false;
	}

	pFile->Read(&fVersion,sizeof fVersion);
	pFile->m_fVersion = fVersion;

	if(fVersion > theApp.m_fVersion) {
		Buff.LoadString(IDS_File_version_error);
		Error.LoadString(IDS_Error);
		MessageBox(NULL,Buff,Error,MB_ICONERROR);
		return false;
	};

	if(fVersion<3.07f) {
		pFile->Seek(0x100,CUSLFile::begin);
		pFile->Read(&nVariablesOffset,sizeof nVariablesOffset);
		pFile->Read(&nStringsOffset,sizeof nStringsOffset);
		pFile->Read(&nGlobalOffset,sizeof nGlobalOffset);
		pFile->Read(&nTimeSlotOffset,sizeof nTimeSlotOffset);
		pFile->Read(&nDataOffset,sizeof nDataOffset);
		pFile->Read(&nAnnotationOffset,sizeof nAnnotationOffset);
		pFile->Read(m_n32VolumeOffset,sizeof m_n32VolumeOffset);
		pFile->Read(&nDetailsOffset,sizeof nDetailsOffset);
		pFile->Read(&nCoordOffset,sizeof nCoordOffset);
		pFile->Read(&nImageNameOffset, sizeof nImageNameOffset);
		n64VariablesOffset = nVariablesOffset;
		n64StringsOffset = nStringsOffset;
		n64GlobalOffset = nGlobalOffset;
		n64TimeSlotOffset = nTimeSlotOffset;
		n64DataOffset = nDataOffset;
		n64AnnotationOffset = nAnnotationOffset;
		n64DetailsOffset = nDetailsOffset =0;
		n64CoordOffset = nCoordOffset = 0;
		n64ImageNameOffset = nImageNameOffset;
		for(int ii=0;ii<8;ii++) {
			m_n64VolumeOffset[ii] = m_n32VolumeOffset[ii];
		}
		m_n64ScanLinesOffset = 0;
		m_n64ToolPathOffset = 0;
		n64PolygonOffset = 0;
		n64LineSampleOffset = 0;
		n64CADOffset = 0;
		n64EnvelopeLineOffset = 0;
		n64BladeOffset = 0;
		n64TranslationOffset = 0;
		n64InclusionsOffset = 0;
	} else {
		pFile->Seek(0x100,CUSLFile::begin);
		pFile->Read(&n64VariablesOffset,sizeof n64VariablesOffset);
		pFile->Read(&n64StringsOffset,sizeof n64StringsOffset);
		pFile->Read(&n64GlobalOffset,sizeof n64GlobalOffset);
		pFile->Read(&n64TimeSlotOffset,sizeof n64TimeSlotOffset);
		pFile->Read(&n64DataOffset,sizeof n64DataOffset);
		pFile->Read(&n64AnnotationOffset,sizeof n64AnnotationOffset);
		pFile->Read(m_n64VolumeOffset,sizeof m_n64VolumeOffset);						//[8]
		pFile->Read(&n64DetailsOffset,sizeof n64DetailsOffset);
		pFile->Read(&n64CoordOffset,sizeof n64CoordOffset);
		pFile->Read(&n64ImageNameOffset, sizeof n64ImageNameOffset);
		pFile->Read(&m_n64ScanLinesOffset,sizeof m_n64ScanLinesOffset);				//[2]
		pFile->Read(&m_n64ToolPathOffset,sizeof m_n64ToolPathOffset);
		pFile->Read(&n64PolygonOffset,sizeof n64PolygonOffset);
		pFile->Read(&n64LineSampleOffset,sizeof n64LineSampleOffset);
		pFile->Read(&n64CADOffset,sizeof n64CADOffset);
		pFile->Read(&n64EnvelopeLineOffset,sizeof n64EnvelopeLineOffset);
		pFile->Read(&n64BladeOffset,sizeof n64BladeOffset);
		pFile->Read(n64BladeLineOffset,sizeof n64BladeLineOffset);
		pFile->Read(&n64TranslationOffset,sizeof n64TranslationOffset);
		pFile->Read(&n64InclusionsOffset,sizeof n64InclusionsOffset);
		pFile->Read(&n64LineLengthOffset, sizeof n64LineLengthOffset);
	}

	if(n64VariablesOffset>0) {
		pFile->Seek(n64VariablesOffset,CUSLFile::begin);

		pFile->Read(&m_nSamplesLine,sizeof m_nSamplesLine);
		pFile->Read(&m_nNumberLines,sizeof m_nNumberLines);
		pFile->Read(&m_fFastScanSize,sizeof m_fFastScanSize);
		pFile->Read(&m_fSlowScanSize,sizeof m_fSlowScanSize);
		pFile->Read(&m_fFastIncrement,sizeof m_fFastIncrement);
		pFile->Read(&m_fSlowIncrement,sizeof m_fSlowIncrement);
		pFile->Read(&m_nNumberImages,sizeof m_nNumberImages);
		pFile->Read(&m_nNumberTimeSlots,sizeof m_nNumberTimeSlots);
		if(fVersion<4.25f) {
			ZeroMemory(m_nNumberAmpImages,sizeof m_nNumberAmpImages);
			ZeroMemory(m_nNumberTimeImages,sizeof m_nNumberTimeImages);
			ZeroMemory(m_nNumberPhaseImages,sizeof m_nNumberPhaseImages);
			pFile->Read(m_nNumberAmpImages,sizeof (int) * 8);
			pFile->Read(m_nNumberTimeImages,sizeof (int) * 8);
			pFile->Read(m_nNumberPhaseImages,sizeof (int) * 8);
		} else {
			pFile->Read(m_nNumberAmpImages,sizeof m_nNumberAmpImages);
			pFile->Read(m_nNumberTimeImages,sizeof m_nNumberTimeImages);
			pFile->Read(m_nNumberPhaseImages,sizeof m_nNumberPhaseImages);
		}
		pFile->Read(&m_nSentence,sizeof m_nSentence);
		pFile->Read(&m_osTestDateTime,sizeof m_osTestDateTime);
		pFile->Read(&m_nNumberVolumeImages,sizeof m_nNumberVolumeImages);
		pFile->Read(&m_nScanStartLine,sizeof m_nScanStartLine);
		pFile->Read(&m_nScanFinishLine,sizeof m_nScanFinishLine);
		pFile->Read(&m_nLineIncrement,sizeof m_nLineIncrement);
		pFile->Read(&m_nComponentDescription,sizeof m_nComponentDescription);
		pFile->Read(&m_nScanType,sizeof m_nScanType);
		pFile->Read(m_fComponentDiameter,sizeof m_fComponentDiameter);
		pFile->Read(&m_nFastAxis,sizeof m_nFastAxis);
		pFile->Read(&m_nSlowAxis,sizeof m_nSlowAxis);
		pFile->Read(&m_nNumberProbesPerImage,sizeof m_nNumberProbesPerImage);
		pFile->Read(&m_nLineIncrementMode,sizeof m_nLineIncrementMode);
		pFile->Read(&m_nCollectScanLines,sizeof m_nCollectScanLines);
		pFile->Read(&m_nCollectSurface,sizeof m_nCollectSurface);
		pFile->Read(&m_nFlattenMode,sizeof m_nFlattenMode);
		pFile->Read(&m_matCadToScan,sizeof m_matCadToScan);
		pFile->Read(&m_nFirstProbe,sizeof m_nFirstProbe);
		if(fVersion > 4.51) {
			pFile->Read(&m_nNumberExtImages,sizeof m_nNumberExtImages);
		}
		if(fVersion >= 4.63) {
			pFile->Read(&m_nLogRange,sizeof m_nLogRange);
		};
		if (fVersion >= 5.02f) {
			pFile->Read(&vTTCentre, sizeof vTTCentre);
		}
	}
	MinMax(&m_nNumberProbesPerImage,0,255); 
	MinMax(&m_nFirstProbe,0,255);

	if(n64CoordOffset>0) {
		pFile->Seek(n64CoordOffset,CUSLFile::begin);
		m_CpStart.Retrieve(pFile);
		m_CpFinish.Retrieve(pFile);
		m_CpTurnTableCentre.Retrieve(pFile);
	}

	if(n64StringsOffset>0) {
		pFile->Seek(n64StringsOffset,CUSLFile::begin);
		pArchFile = new CArchive(pFile,CArchive::load);
		pArchFile->ReadString(m_Operator);
		pArchFile->ReadString(m_TestDateTime);
		pArchFile->ReadString(m_ProfileName);
		if(fVersion>=4.31f) {
			pArchFile->ReadString(m_UniqueIdentification);
			if(fVersion>=4.63f) {
				pArchFile->ReadString(m_MachineName);
				pArchFile->ReadString(m_MachineLocation);
			};
		}
		pArchFile->Close();
		delete pArchFile;
	}
	

	if(fVersion<1.05) {
		theApp.m_UtUser.RetrieveVersion1((CUSLFile*)pFile,&m_Global,m_TS,(int)n64GlobalOffset,(int)n64TimeSlotOffset,0);
	} else {
		if(n64GlobalOffset>0) {
			pFile->Seek(n64GlobalOffset,CUSLFile::begin);
			pFile->ReadStruct(&m_Global, sizeof m_Global);
			if(fVersion<4.22f) {
				m_Global.LSA.nOldElementsInArray = 64;
				m_Global.LSA.fOldArrayPitch = 0.5f;
				m_Global.LSA.nOldStartElement = 0;
				m_Global.LSA.bOldReverseArrayOrder = false;
				ZeroMemory(&m_Global.Ext, sizeof m_Global.Ext);
			}
		}
		if(n64TimeSlotOffset>0) {
			pFile->Seek(n64TimeSlotOffset,CUSLFile::begin);
			int nMaxTS = 256;
			if(fVersion<4.22f) nMaxTS = 8;

			for(nTS=0;nTS<nMaxTS;nTS++) {
				int nStructSize = sizeof m_TS[0];
				pFile->ReadStruct(&m_TS[nTS], sizeof m_TS[0]);
				pFile->Seek(0x200,CUSLFile::current);

				if(fVersion < 4.22f) {
					m_TS[nTS].Pr30.nDACDeltaDelay = 0;
					m_TS[nTS].Pr30.nI2CUmbilicalSelect = 0;
					m_TS[nTS].Pr30.nRxRfUmbilicalSelect = 0;
					ZeroMemory(m_TS[nTS].Gate.nTimeStatus, sizeof m_TS[nTS].Gate.nTimeStatus);
					m_TS[nTS].Gate.nNsPeakWidth = 0;
				}
			}
		}
	}

	if(n64DataOffset>0) {
		AllocateImageBuffers();
		pFile->Seek(n64DataOffset,CUSLFile::begin);
		nSize = m_nSamplesLine * m_nNumberLines;
		gg=tt=pp=0;
		for(nTS=0;nTS<m_nNumberTimeSlots;nTS++) {
			for(nn=0;nn<m_nNumberAmpImages[nTS];nn++) {
				pFile->Read(m_cAmpBuffer[gg++],nSize * sizeof (unsigned char));

//				unsigned char* pData = m_cAmpBuffer[gg-1];
//				for(nn=0;nn<nSize;nn++) {
//					if(pData[nn] > INIT_VALUE) pData[nn] = 200 + MulDiv(rand(),2,RAND_MAX);
//				};
			}


			for(nn=0;nn<m_nNumberTimeImages[nTS];nn++) {
				pFile->Read(m_nTimeBuffer[tt++],nSize * sizeof (unsigned int));
			}
			for(nn=0;nn<m_nNumberPhaseImages[nTS];nn++) {
				pFile->Read(m_cPhaseBuffer[pp++],nSize * sizeof (unsigned char));
			}
		}
		for(nn=0;nn<m_nNumberExtImages;nn++) {
			pFile->Read(m_16ExtBuffer[nn],nSize * sizeof (unsigned __int16));
		}
	}

	if(n64AnnotationOffset>0) {
		pFile->Seek(n64AnnotationOffset,CUSLFile::begin);
		pFile->Read(&m_nAnnotationL,sizeof m_nAnnotationL);
		if(m_nAnnotationL) {
			m_pAnnotation = new ANNOTATION[m_nAnnotationL];
			for(nn=0;nn<m_nAnnotationL;nn++) {
				pFile->ReadStruct(&m_pAnnotation[nn], sizeof ANNOTATION);
				if(fVersion<4.19f) {
					m_pAnnotation[nn].nApplyToAllImage=1;
					m_pAnnotation[nn].nImageNumber=0;
					m_pAnnotation[nn].nImageType=0;
				}
			}
		}
	}
	CString Degrees = L"\x0bo\x0b0";

	if(n64DetailsOffset>0) {
		pFile->Seek(n64DetailsOffset,CUSLFile::begin);
		if(fVersion<=4.38f) {
			pFile->Read(&m_nDetailsL,sizeof m_nDetailsL);
			if((m_nDetailsL>0) && (m_nDetailsL < MAX_DETAILS)) {
				pArchFile = new CArchive(pFile,CArchive::load);
				for(nn=0;nn<m_nDetailsL;nn++) {
					pArchFile->ReadString(m_Details[nn]);
					pArchFile->ReadString(m_Caption[nn]);
					m_Details[nn].Replace(Degrees,_T("\r\n"));

				}
				pArchFile->Close();
				delete pArchFile;
			} else {
				m_nDetailsL = 0;
			}
		} else {
			m_DataProperties.Retrieve(pFile);
		}
	}

	if(n64ImageNameOffset>0){
		pFile->Seek(n64ImageNameOffset,CUSLFile::begin);
		pFile->Read(&m_nImageNameL,sizeof m_nImageNameL);
		pArchFile = new CArchive(pFile,CArchive::load);
		for(nn=0;nn<m_nImageNameL;nn++) {
			pArchFile->ReadString(m_ImageName[nn]);
		}
		pArchFile->Close();
		delete pArchFile;
	}

	if(n64PolygonOffset>0){
		pFile->Seek(n64PolygonOffset,CUSLFile::begin);
		pFile->Read(&m_nPolygonL,sizeof m_nPolygonL);
		if(m_nPolygonL) m_pPolygon = new CPolyCoord[m_nPolygonL];
		for(nn=0;nn<m_nPolygonL;nn++) m_pPolygon[nn].Retrieve(pFile);
	}

	m_nNumberVertexLines=0;
	if(n64LineSampleOffset>0) {
		pFile->Seek(n64LineSampleOffset,CUSLFile::begin);
		pFile->Read(&m_nNumberVertexLines,sizeof m_nNumberVertexLines);	//yes this should be the same as m_nNumberLines
		if(m_nNumberVertexLines>0) {
			SAFE_DELETE_ARRAY(m_vLineSample);
			m_vLineSample = new CVertexArray[ m_nNumberVertexLines ];
			for(nn=0;nn<m_nNumberVertexLines;nn++) {
				m_vLineSample[nn].Retrieve(pFile);
			}
		}
	}

	if(n64CADOffset>0) {
		pFile->Seek(n64CADOffset,CUSLFile::begin);
		pFile->Read(&m_nCADElementL,sizeof m_nCADElementL);
		if(m_nCADElementL>0) {
			SAFE_DELETE_ARRAY( m_pCADElement );
			m_pCADElement = new CCADElement[m_nCADElementL];
			for(nn=0;nn<m_nCADElementL;nn++) {
				m_pCADElement[nn].Retrieve(pFile);
			}
		}
	}

	//Read the envelopelines
	if(n64EnvelopeLineOffset>0) {
		pFile->Seek(n64EnvelopeLineOffset,CUSLFile::begin);
		pFile->Read(&m_nEnvelopeLineL,sizeof m_nEnvelopeLineL);
		if(m_nEnvelopeLineL) m_pEnvelopeLine = new CPolyCoord[m_nEnvelopeLineL];
		for(nLine = 0;nLine < m_nEnvelopeLineL; nLine++) {
			m_pEnvelopeLine[nLine].Retrieve(pFile);
			m_pEnvelopeLine[nLine].m_nOffOn = 1;
		}

	}

	//Read Blades
	if(n64BladeOffset>0) {
		pFile->Seek(n64BladeOffset,CUSLFile::begin);
		for(nn=0; nn<2; nn++) {
			pFile->Seek(n64BladeLineOffset[nn],CUSLFile::begin);
			pFile->Read(&m_nLineBladeL[nn],sizeof m_nLineBladeL[nn]);
			if(m_nLineBladeL[nn]) m_LineBlade[nn] = new CPolyCoord[m_nLineBladeL[nn]];
			for(int nLine=0;nLine<m_nLineBladeL[nn];nLine++) m_LineBlade[nn][nLine].Retrieve(pFile);
		};
	}

	if(n64TranslationOffset>0) {
		pFile->Seek(n64TranslationOffset,CUSLFile::begin);
		for(nn=0;nn<MAX_LINE_TYPES;nn++) {
			m_CpTranslation[nn].Retrieve(pFile);
		}
	}

	if(fVersion >= 4.59f) {
		if(n64InclusionsOffset > 0) {
			pFile->Seek(n64InclusionsOffset,CUSLFile::begin);
			m_Inclusions.Retrieve(pFile);
		}
	}
	GenerateOneTimeOnlyLineLength();
	if (fVersion >= 5.02f) {
		if (n64LineLengthOffset > 0) {
			SAFE_DELETE(m_fSampleWidth);
			SAFE_DELETE(m_fLineLength);
			m_fSampleWidth = new float[m_nNumberLines];
			m_fLineLength = new float[m_nNumberLines];
			pFile->Seek(n64LineLengthOffset, CUSLFile::begin);
			pFile->Read(m_fLineLength, m_nNumberLines * sizeof(float));
			pFile->Read(m_fSampleWidth, m_nNumberLines * sizeof(float));
		}
	}

	m_bHeaderForVolumeIsValid = TRUE;

	PROFILE->m_nCollectAxis = m_nCollectAxis = m_nFastAxis;

	for(nTS=0;nTS<8;nTS++) {
		if(m_nNumberAmpImages[nTS] < 0) m_nNumberAmpImages[nTS]=0;
		if(m_nNumberTimeImages[nTS] < 0) m_nNumberTimeImages[nTS]=0;
		if(m_nNumberPhaseImages[nTS] < 0) m_nNumberPhaseImages[nTS]=0;
	}

	if(m_nLogRange<10 || m_nLogRange>130) {
		m_nLogRange = theApp.m_LastSettings.nLogRange;
	}
	m_bCScansSaved = true;
	if(m_nNumberVolumeImages>0) m_bVolScansSaved = true;

	return TRUE;
}

bool CData::RetrieveGenericScannerFile(CUSLFile *pFile)
{
	CProgressDlg dlg(NULL);
	dlg.Create(IDD_PROGRESS_DIALOG, NULL);	

	CString Buff;
	double	dDataVersion;
	int	nAnnotateOffset,nFileOffset,nInfoOffset,nUtOffset,nResultsOffset,nCTGOffset;
	int	nLogicalOffset,nGateNameOffset,nReportOffset;
	int	nNumberScans,nScanAxes,nMode;
	int nBufferOffset[8],nScanOffset[20];
	int	nSize;
	double dFastScanSize,dSlowScanSize,dFastIncrement,dSlowIncrement;
	double dGain[8];
	int	nRfDataType[8];
	char Identifier[30];
	CoordStructGeneric StartCp,FinishCp;

	Zero();

	pFile->Seek(sizeof Identifier,CUSLFile::begin);
	pFile->Read(nBufferOffset,sizeof nBufferOffset);
	pFile->Read(&nAnnotateOffset,sizeof nAnnotateOffset);
	pFile->Read(&dDataVersion,sizeof dDataVersion);
	if(dDataVersion>=2.1) {
		pFile->Read(&nNumberScans,sizeof nNumberScans);
		if(nNumberScans<=0) nNumberScans=1;
		pFile->Read(nScanOffset,sizeof nScanOffset);
		nFileOffset=nScanOffset[nNumberScans-1];
		pFile->Seek(nFileOffset,CUSLFile::begin);
		pFile->Read(Identifier,sizeof Identifier);
		pFile->Read(nBufferOffset,sizeof nBufferOffset);
		pFile->Read(&nAnnotateOffset,sizeof nAnnotateOffset);
		pFile->Read(&nInfoOffset,sizeof nInfoOffset);
		pFile->Read(&nUtOffset,sizeof nUtOffset);
		pFile->Read(&nResultsOffset,sizeof nResultsOffset);
		pFile->Read(&nCTGOffset,sizeof nCTGOffset);
		pFile->Read(&nLogicalOffset,sizeof nLogicalOffset);
		pFile->Read(&nGateNameOffset,sizeof nGateNameOffset);
		pFile->Read(&nReportOffset,sizeof nReportOffset);
	}

	pFile->Seek(nFileOffset+1024,CUSLFile::begin);
	pFile->Read(&m_nSamplesLine,sizeof m_nSamplesLine);
	pFile->Read(&m_nNumberLines,sizeof m_nNumberLines);
	pFile->Read(&dFastScanSize,sizeof dFastScanSize);
	pFile->Read(&dSlowScanSize,sizeof dSlowScanSize);
	pFile->Read(&dFastIncrement,sizeof dFastIncrement);
	pFile->Read(&dSlowIncrement,sizeof dSlowIncrement);
	pFile->Read(&nScanAxes,sizeof nScanAxes);		//different
	pFile->Read(&m_nNumberImages,sizeof m_nNumberImages);
	pFile->Read(&StartCp,sizeof StartCp);
	pFile->Seek(StartCp.nStructSize - (sizeof CoordStructGeneric),CUSLFile::current);
	pFile->Read(&FinishCp,sizeof FinishCp);
	pFile->Read(&m_nSentence,sizeof m_nSentence);
	pFile->Read(&nMode,sizeof nMode);
	pFile->Read(nRfDataType,sizeof nRfDataType);
	pFile->Read(dGain,sizeof dGain);
	pFile->Read(&m_nNumberAmpImages[0],sizeof m_nNumberAmpImages[0]);
	pFile->Read(&m_nNumberTimeImages[0],sizeof m_nNumberTimeImages[0]);
/*		pFile->Read(&m_nSide,sizeof m_nSide);
		pFile->Read(&m_nImageType,sizeof m_nImageType);
		pFile->Read(&m_dPolarIncrement,sizeof m_dPolarIncrement);
		pFile->Read(&m_dCartesianIncrement,sizeof m_dCartesianIncrement);
		pFile->Read(&m_nTTScanMode,sizeof m_nTTScanMode);
		pFile->Read(&m_nRollerScanMode,sizeof m_nRollerScanMode);
		pFile->Read(&m_dComponentDiameter,sizeof m_dComponentDiameter);
		pFile->Read(&m_nTubeScanMode,sizeof m_nTubeScanMode);
		pFile->Read(&m_nUsedTimeSlot,sizeof m_nUsedTimeSlot);
		pFile->Read(&theApp.m_LastSettings.dActualScanSpeed,sizeof theApp.m_LastSettings.dActualScanSpeed);
		pFile->Read(&m_osTestDate,sizeof m_osTestDate);
		pFile->Read(&m_nDiscScanAxes,sizeof m_nDiscScanAxes);
		pFile->Read(&m_nSentence,sizeof m_nSentence);
		pFile->Read(&m_nNumberPhaseImages,sizeof m_nNumberPhaseImages);

		theApp.m_Motors.CalculateRollerStepSize();
		if(m_osTestDate==0) {
			pFile->GetStatus(rStatus);
			m_osTestDate=rStatus.m_ctime.GetTime();;
		}
*/
	nSize=m_nSamplesLine * m_nNumberLines;
	int	gg,tt;
	if(dDataVersion>=2.0) {
		dlg.SetProgressRange(0,nSize*m_nNumberImages);
		for(gg=0;gg<m_nNumberAmpImages[0];gg++) {
			dlg.SetProgressPos(nSize*gg);
			pFile->Seek(nBufferOffset[gg],CUSLFile::begin);
			m_cAmpBuffer[gg] = new unsigned char[nSize];
			pFile->Read(m_cAmpBuffer[gg],nSize);
		};
		for(tt=0;tt<m_nNumberTimeImages[0];tt++,gg++) {
			Buff.Format(_T("Loading %.01f MBytes"),(float)(nSize*sizeof(int))/1e6);
			dlg.SetProgressPos(nSize*gg);
			dlg.SetProgressText(Buff);
			pFile->Seek(nBufferOffset[gg],CUSLFile::begin);
			m_nTimeBuffer[tt] = new unsigned int[nSize];
			pFile->Read(m_nTimeBuffer[tt],nSize * sizeof(int));
		};
		if(dDataVersion>=2.8) {
			for(tt=0;tt<m_nNumberPhaseImages[0];tt++,gg++) {
				Buff.Format(_T("Loading %.01f MBytes"),(float)(nSize)/1e6);
				dlg.SetProgressPos(nSize*gg);
				dlg.SetProgressText(Buff);
				pFile->Seek(nBufferOffset[gg],CUSLFile::begin);
				m_cPhaseBuffer[tt] = new unsigned char[nSize];
				pFile->Read(m_cPhaseBuffer[tt],nSize);
			};
		}
	}


	m_nCollectAxis = nScanAxes;
	m_fFastScanSize = (float)dFastScanSize;
	m_fSlowScanSize = (float)dSlowScanSize;
	m_fFastIncrement = (float)dFastIncrement;
	m_fSlowIncrement = (float)dSlowIncrement;
	return TRUE;
}

bool CData::RetrieveFokkerDosFile(CUSLFile *pFile)
{
	int gg,ii;
	ULONGLONG nOffset;
	ULONGLONG nFileLength = pFile->GetLength();
	float* fArray;
	unsigned char cArray[1024];

	Zero();

	pFile->Seek(0,CUSLFile::begin);
	pFile->Read(cArray,1024);

	m_nSamplesLine = 1280;
	m_nNumberLines = 950;
	
	fArray = (float*)(cArray + 546);
	m_fFastScanSize = fArray[0];
	m_fSlowScanSize = fArray[1];
	m_fFastIncrement = m_fFastScanSize / (float)m_nSamplesLine;
	m_fSlowIncrement = m_fSlowScanSize / (float)m_nNumberLines;

	m_nNumberTimeSlots=1;
	m_nNumberAmpImages[0]=1;

	int nSize = m_nSamplesLine * m_nNumberLines;
	for(gg=0;gg<m_nNumberAmpImages[0];gg++) {
		nOffset = nFileLength - (nSize * (gg+1));
		pFile->Seek(nOffset,CUSLFile::begin);
		m_cAmpBuffer[gg] = new unsigned char[nSize];
		pFile->Read(m_cAmpBuffer[gg],nSize);
		for(ii=0;ii<nSize;ii++) {
			m_cAmpBuffer[gg][ii] = (m_cAmpBuffer[gg][ii] / 2) + 128;
		}
	};

	return TRUE;

}

bool CData::RemoveWaterSplash(int nImageType,int nImageNumber,CRect rrSampleLine)
{
	int nPtr;
	CRect rr = rrSampleLine;
	rr.NormalizeRect();
	int nArraySize = m_nSamplesLine * m_nNumberLines;
	int nTemp, nSample, nLine;
	int nMin = 1000;
	int nMax = 0;
	CPoint ptMin;

	switch(nImageType) {
	case AMP_IMAGE:
		for(nLine=rr.top; nLine<rr.bottom;nLine++) {
			for(nSample=rr.left;nSample<rr.right;nSample++) {
				nPtr=nSample + (nLine*m_nSamplesLine);
				if((nPtr>=0) && (nPtr<nArraySize)) {
					if(nMin > (nTemp=m_cAmpBuffer[nImageNumber][nPtr])) {
						nMin = nTemp;
						ptMin = CPoint(nSample,nLine);
					}
				}
			}
		}
		if(ptMin.x > 0 && ptMin.x < (m_nSamplesLine-1) && ptMin.y > 0 && ptMin.y < (m_nNumberLines-1)) {
			nPtr = ptMin.x + ptMin.y * m_nSamplesLine;
			if(nMax < (nTemp = m_cAmpBuffer[nImageNumber][nPtr+1])) nMax = nTemp;
			if(nMax < (nTemp = m_cAmpBuffer[nImageNumber][nPtr-1])) nMax = nTemp;
			if(nMax < (nTemp = m_cAmpBuffer[nImageNumber][nPtr+m_nSamplesLine])) nMax = nTemp;
			if(nMax < (nTemp = m_cAmpBuffer[nImageNumber][nPtr-m_nSamplesLine])) nMax = nTemp;
			m_cAmpBuffer[nImageNumber][nPtr] = nTemp;
		}
		break;
	case TIME_IMAGE: return false;
		break;
	case EXT_IMAGE: return false;
		break;
	}

	return true;
}

bool CData::BuildHistogramData(int nImageType,int nImageNumber,CRect rr)
{

	rr.NormalizeRect();

	m_Hist.nImageType = nImageType;
	m_Hist.nImageNumber = nImageNumber;
	m_Hist.rr = rr;

	switch(nImageType) {
	case AMP_IMAGE:	return BuildAmpHistogramData();
		break;
	case TIME_IMAGE: 	return BuildTimeHistogramData();
		break;
	case EXT_IMAGE:	return BuildExtHistogramData();
		break;
	}
	return FALSE;
}

bool CData::ReBuildHistogramData()
{

	switch(m_Hist.nImageType) {
	case AMP_IMAGE:	return BuildAmpHistogramData();
		break;
	case TIME_IMAGE: 	return BuildTimeHistogramData();
		break;
	case EXT_IMAGE:	return BuildExtHistogramData();
	}
	return FALSE;
}


bool CData::BuildAmpHistogramData()
{
	int	xx,yy,nTemp,ii;
	float fStepSize = theApp.m_LastSettings.fHistogramStepAmplitude;
	int nArraySize = m_nSamplesLine * m_nNumberLines;
	int	nMaxLength;
	int	nX0,nX1,nY0,nY1;
	bool bUseSample=false;
	float fSampleArea;

	CRect rr = m_Hist.rr;

	if(rr.top<0) rr.top=0;
	if(rr.left<0) rr.left = 0;

	if(m_Hist.nCoordL!=256) {
		SAFE_DELETE(m_Hist.pnData);
		SAFE_DELETE(m_Hist.pfArea);
	}

	if(theApp.m_LastSettings.fHistogramRelativedB[0] <= 0.0f) {
		m_Hist.nAmpMode = HIST_ATTENUATION;
	} else {
		m_Hist.nAmpMode = HIST_REFLECTION;
	}

	m_Hist.nCoordL=256;
	if (m_Hist.pnData == NULL)	m_Hist.pnData = new int[m_Hist.nCoordL];
	ZeroMemory(m_Hist.pnData, m_Hist.nCoordL * sizeof(int));
	if (m_Hist.pfArea == NULL)	m_Hist.pfArea = new float[m_Hist.nCoordL];
	ZeroMemory(m_Hist.pfArea, m_Hist.nCoordL * sizeof(float));
	m_Hist.nMaxAmp = 0;
	m_Hist.nPeakAmp=128;
	m_Hist.nMaxValue = 0;
	m_Hist.nMinValue = 255;
	m_Hist.nTotalNumberSamples=0;
	if(m_Hist.bMeanLatched!=TRUE) m_Hist.nAverageAmp=0;

	MinMax(&rr.top,0,m_nNumberLines);
	MinMax(&rr.bottom,0,m_nNumberLines);

	for(yy=rr.top;yy<rr.bottom;yy++) {
		fSampleArea = GetAreaOfSample(yy);
		for(xx=rr.left;xx<rr.right;xx++) {
			if(theApp.m_LastSettings.nHistogramStyle) {
				if((hypot(abs((xx-rr.left)-(rr.Width()/2)),abs((yy-rr.top)-(rr.Height()/2)))) <= (rr.Width()/2) )
					bUseSample = true;
				else
					bUseSample = false;
			}else{
				bUseSample =true;
			}

			if(bUseSample){
		
				nTemp = GetFilteredAmp(xx,yy,m_Hist.nImageNumber);
				nTemp=MinMax(nTemp,0,255);
				m_Hist.pnData[nTemp]++;
				m_Hist.pfArea[nTemp] += fSampleArea;
				if(m_Hist.bMeanLatched!=TRUE) m_Hist.nAverageAmp+=nTemp;
				if(m_Hist.pnData[nTemp]>m_Hist.nMaxAmp) {
					m_Hist.nMaxAmp=m_Hist.pnData[nTemp];
					m_Hist.nPeakAmp = nTemp;
				}
				if(nTemp<m_Hist.nMinValue) {
					m_Hist.nMinValue=nTemp;
				}
				if(nTemp>m_Hist.nMaxValue) {
					m_Hist.nMaxValue=nTemp;
				}
				m_Hist.nTotalNumberSamples++;
			}

		};
	};

	int nHalfMaxAmp = m_Hist.nMaxAmp / 2;
	for(ii=m_Hist.nPeakAmp;ii>128;ii--) {
		if(m_Hist.pnData[ii]<nHalfMaxAmp) {
			m_Hist.nAttenAmp = ii;
			break;
		}
	}

	if(m_Hist.nTotalNumberSamples>0) m_Hist.nAverageAmp /= m_Hist.nTotalNumberSamples;

	if(theApp.m_bCtrlButton == true) {
		switch(theApp.m_LastSettings.nHistogramMask) {
		default:
			break;
		case 1: m_Hist.fLatchAmp = ((float)(m_Hist.nPeakAmp-128) * 100.0f) / 127.0f;
			break;
		case 2: m_Hist.fLatchAmp = ((float)(m_Hist.nAverageAmp-128) * 100.0f) / 127.0f;
			break;
		case 4: m_Hist.fLatchAmp = ((float)(m_Hist.nAttenAmp-128) * 100.0f) / 127.0f;
			break;
		}
	}

	switch(m_Hist.nAmplitudeScaleType) {
	case 0:
	case 1: m_Hist.nSampleThreshold[0] = (int)(((m_Hist.fLatchAmp / (float)pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[0]/-20.0f)) * 127.0f) / 100.0f) + 128;
			m_Hist.nSampleThreshold[1] = (int)(((m_Hist.fLatchAmp / (float)pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[1]/-20.0f)) * 127.0f) / 100.0f) + 128;
		break;
	case 2: m_Hist.nSampleThreshold[0] = (int)((m_Hist.fLatchAmp + (theApp.m_LastSettings.fHistogramRelativedB[0]*100.0f/(float)m_nLogRange))*1.27f) + 128;
			m_Hist.nSampleThreshold[1] = (int)((m_Hist.fLatchAmp + (theApp.m_LastSettings.fHistogramRelativedB[1]*100.0f/(float)m_nLogRange))*1.27f) + 128;
			m_Hist.fMaxValue = ((float)(127-(m_Hist.nMaxValue - 128)) * (float)m_nLogRange / -127.0f);
			m_Hist.fMinValue = ((float)(127-(m_Hist.nMinValue - 128)) * (float)m_nLogRange / -127.0f);
		break;
	}

	m_Hist.nSampleThreshold[0]=MinMax(m_Hist.nSampleThreshold[0],0,255);
	m_Hist.nSampleThreshold[1]=MinMax(m_Hist.nSampleThreshold[1],0,255);
	m_Hist.nTotalBelowThreshold[0]=0;
	m_Hist.nTotalBelowThreshold[1]=0;
	m_Hist.nTotalAboveThreshold[0]=0;
	m_Hist.nTotalAboveThreshold[1]=0;
	m_Hist.fTotalAreaBelowThreshold[0] = 0.0f;
	m_Hist.fTotalAreaBelowThreshold[1] = 0.0f;
	m_Hist.fTotalAreaAboveThreshold[0] = 0.0f;
	m_Hist.fTotalAreaAboveThreshold[1] = 0.0f;
	for (ii = 0; ii < m_Hist.nSampleThreshold[0]; ii++) {
		m_Hist.nTotalBelowThreshold[0] += m_Hist.pnData[ii];
		m_Hist.fTotalAreaBelowThreshold[0] += m_Hist.pfArea[ii];
	}
	for (ii = 0; ii < m_Hist.nSampleThreshold[1]; ii++) {
		m_Hist.nTotalBelowThreshold[1] += m_Hist.pnData[ii];
		m_Hist.fTotalAreaBelowThreshold[1] += m_Hist.pfArea[ii];
	}
	if (m_Hist.nSampleThreshold[0] < 0) m_Hist.nSampleThreshold[0] = 0;
	if (m_Hist.nSampleThreshold[1] < 0) m_Hist.nSampleThreshold[1] = 0;
	for (ii = m_Hist.nSampleThreshold[0]; ii < 256; ii++) {
		m_Hist.nTotalAboveThreshold[0] += m_Hist.pnData[ii];
		m_Hist.fTotalAreaAboveThreshold[0] += m_Hist.pfArea[ii];
	}
	for (ii = m_Hist.nSampleThreshold[1]; ii < 256; ii++) {
		m_Hist.nTotalAboveThreshold[1] += m_Hist.pnData[ii];
		m_Hist.fTotalAreaAboveThreshold[1] += m_Hist.pfArea[ii];
	}

////////////////////////////////////////
// Calculate stats for values between the thresholds
////////////////////////////////////////
	int nAverage = 0;
	int nSum = 0;
	int nAmp;
	for(nAmp = m_Hist.nSampleThreshold[1]; nAmp <= m_Hist.nSampleThreshold[0]; nAmp++) {
		nAverage += (m_Hist.pnData[nAmp] * nAmp);
		nSum += m_Hist.pnData[nAmp];
	}
	m_Hist.fAverageBetween = (float) nAverage / (float)nSum;

	float fAverageAmp = (float)m_Hist.nAverageAmp;
	float fVariance = 0.0;
	float fVarianceBetween = 0.0;

	bUseSample=false;
	int nn,nnBetween;
	if(rr.Width() && rr.Height()) {
		for(yy=rr.top,nn=0,nnBetween=0;yy<rr.bottom;yy++) {
			for(xx=rr.left;xx<rr.right;xx++,nn++) {

				if(theApp.m_LastSettings.nHistogramStyle) {
					if((hypot(abs((xx-rr.left)-(rr.Width()/2)),abs((yy-rr.top)-(rr.Height()/2)))) <= (rr.Width()/2) )
						bUseSample = true;
					else
						bUseSample = false;
				}else{
					bUseSample =true;
				}

				if(bUseSample){

					nAmp = GetFilteredAmp(xx,yy,m_Hist.nImageNumber);
					fVariance += (((float)nAmp-fAverageAmp) * ((float)nAmp-fAverageAmp));

					if((nAmp>=m_Hist.nSampleThreshold[1]) && (nAmp<=m_Hist.nSampleThreshold[0])) {
						fVarianceBetween += (((float)nAmp-m_Hist.fAverageBetween) * ((float)nAmp-m_Hist.fAverageBetween));
						nnBetween++;
					}

				}
			}
		}
		fVariance /= (float)(nn-1);
		m_Hist.fStd = (float)sqrt((double)fVariance);

		fVarianceBetween /= (float)(nnBetween-1);
		m_Hist.fStdBetween = (float)sqrt((double)fVarianceBetween);
	}


	//////////////////////////////////////////////
	// Width ane height;
	m_Hist.rrThreshWidth.SetRectEmpty();
	m_Hist.rrThreshHeight.SetRectEmpty();
	m_Hist.rrFloatThreshWidth.SetRectEmpty();
	m_Hist.rrFloatThreshHeight.SetRectEmpty();
	bUseSample = false;

	float fLineLength, fSampleWidth;

	nMaxLength = 0;
	for (yy = rr.top; yy < rr.bottom; yy++) {
		GetFastScanSizeIncrement(&fLineLength, &fSampleWidth, yy);

		for (xx = rr.left; xx < rr.right; xx++) {

			if (theApp.m_LastSettings.nHistogramStyle) {
				if ((hypot(abs((xx - rr.left) - (rr.Width() / 2)), abs((yy - rr.top) - (rr.Height() / 2)))) <= (rr.Width() / 2))
					bUseSample = true;
				else
					bUseSample = false;
			}
			else {
				bUseSample = true;
			}

			if (bUseSample) {

				nTemp = GetFilteredAmp(xx, yy, m_Hist.nImageNumber);
				if (((nTemp <= m_Hist.nSampleThreshold[0]) && (m_Hist.nAmpMode == HIST_ATTENUATION)) ||
					((nTemp >= m_Hist.nSampleThreshold[0]) && (m_Hist.nAmpMode == HIST_REFLECTION))) {
					nX0 = xx;
					nX1 = xx + 1;
					for (xx; xx < rr.right; xx++) {
						nTemp = GetFilteredAmp(xx, yy, m_Hist.nImageNumber);
						if (((nTemp <= m_Hist.nSampleThreshold[0]) && (m_Hist.nAmpMode == HIST_ATTENUATION)) ||
							((nTemp >= m_Hist.nSampleThreshold[0]) && (m_Hist.nAmpMode == HIST_REFLECTION))) {

							nX1 = xx;
							if (nMaxLength < (nX1 - nX0)) {
								nMaxLength = nX1 - nX0;
								m_Hist.rrThreshWidth.left = nX0;
								m_Hist.rrThreshWidth.right = nX1;
								m_Hist.rrThreshWidth.top = m_Hist.rrThreshWidth.bottom = yy;
								m_Hist.rrFloatThreshWidth.fLeft = (float)nX0 * fSampleWidth;
								m_Hist.rrFloatThreshWidth.fRight = (float)nX1 * fSampleWidth;
								m_Hist.rrFloatThreshWidth.fTop = m_Hist.rrFloatThreshWidth.fBottom = yy * m_fSlowIncrement;
							}
						}
						else {
							break;
						}
					}
				}
			}
		}
	}

	bUseSample = false;

	nMaxLength = 0;
	for (xx = rr.left; xx < rr.right; xx++) {
		for (yy = rr.top; yy < rr.bottom; yy++) {
			GetFastScanSizeIncrement(&fLineLength, &fSampleWidth, yy);

			if (theApp.m_LastSettings.nHistogramStyle) {
				if ((hypot(abs((xx - rr.left) - (rr.Width() / 2)), abs((yy - rr.top) - (rr.Height() / 2)))) <= (rr.Width() / 2))
					bUseSample = true;
				else
					bUseSample = false;
			}
			else {
				bUseSample = true;
			}
			if (bUseSample) {

				nTemp = GetFilteredAmp(xx, yy, m_Hist.nImageNumber);
				if (((nTemp = m_Hist.nSampleThreshold[0]) && (m_Hist.nAmpMode == HIST_ATTENUATION)) ||
					((nTemp >= m_Hist.nSampleThreshold[0]) && (m_Hist.nAmpMode == HIST_REFLECTION))) {
					nY0 = yy;
					nY1 = yy + 1;
					for (yy; yy < rr.bottom; yy++) {
						nTemp = GetFilteredAmp(xx, yy, m_Hist.nImageNumber);
						if (((nTemp <= m_Hist.nSampleThreshold[0]) && (m_Hist.nAmpMode == HIST_ATTENUATION)) ||
							((nTemp >= m_Hist.nSampleThreshold[0]) && (m_Hist.nAmpMode == HIST_REFLECTION))) {

							nY1 = yy;
							if (nMaxLength < (nY1 - nY0)) {
								nMaxLength = nY1 - nY0;
								m_Hist.rrThreshHeight.left = m_Hist.rrThreshHeight.right = xx;
								m_Hist.rrThreshHeight.top = nY0;
								m_Hist.rrThreshHeight.bottom = nY1;
								m_Hist.rrFloatThreshHeight.fLeft = m_Hist.rrFloatThreshHeight.fRight = xx * fSampleWidth;
								m_Hist.rrFloatThreshHeight.fTop = nY0 * m_fSlowIncrement;
								m_Hist.rrFloatThreshHeight.fBottom = nY1 * m_fSlowIncrement;
							}
						}
						else {
							break;
						}
					}
				}
			}
		}
	}


	////////////////////////////////////////////
	//Put Real numbers in to ref level
	if(m_Hist.nAmpMode == HIST_ATTENUATION) {

		switch(m_Hist.nAmplitudeScaleType) {
		case 0:	m_Hist.fRefAmp = m_Hist.fLatchAmp;
			break;
		case 1:	m_Hist.fRefAmp =  -20.0f * (float)log10(100.0/m_Hist.fLatchAmp);
			break;
		case 2:	m_Hist.fRefAmp = (100.0f-m_Hist.fLatchAmp) * (float)m_nLogRange / 100.0f;
			break;
		}
	} else {	//HIST REFLECTION
		switch(m_Hist.nAmplitudeScaleType) {
		case 0:	m_Hist.fRefAmp = m_Hist.fLatchAmp;
			break;
		case 1:	m_Hist.fRefAmp = -20.0f * (float)log10(100.0/m_Hist.fLatchAmp);
			break;
		case 2:	m_Hist.fRefAmp = (100.0f-m_Hist.fLatchAmp) * (float)m_nLogRange / 100.0f;
			break;
		}
	}


	return TRUE;

}


bool CData::BuildTimeHistogramData()
{
	int	xx,yy,nTemp;
	float fStepSize = theApp.m_LastSettings.fHistogramStepAmplitude;
	int nArraySize = m_nSamplesLine * m_nNumberLines;
	int	nMaxLength;
	int	nX0,nX1,nY0,nY1,ii;

	CRect rr = m_Hist.rr;

	if(m_Hist.nCoordL!=theApp.m_Palette.m_nMaxThicknessNumber[0]) {
		delete m_Hist.pnData;
		m_Hist.pnData=NULL;
	}

	m_Hist.nCoordL=theApp.m_Palette.m_nMaxThicknessNumber[0];
	if(m_Hist.pnData==NULL)	m_Hist.pnData = new int[m_Hist.nCoordL];
	if(!rr.IsRectEmpty()) {  //Only re process if sizeof rr has changed
		ZeroMemory(m_Hist.pnData,m_Hist.nCoordL * sizeof (int));
		m_Hist.nMaxThickness = 0;
		m_Hist.nPeakThickness = 0;
		m_Hist.nTotalNumberSamples=0;
		if(m_Hist.bMeanLatched!=TRUE) m_Hist.nAverageThickness=0;

		for(yy=rr.top;yy<=rr.bottom;yy++) {
			for(xx=rr.left;xx<rr.right;xx++) {
				nTemp = GetFilteredTimeThickness(xx,yy,m_Hist.nImageNumber,1);		// returns Thickness not time
				m_Hist.pnData[nTemp]++;
				if(m_Hist.bMeanLatched!=TRUE) m_Hist.nAverageThickness+=nTemp;
				if(m_Hist.pnData[nTemp]>m_Hist.nMaxThickness) {
					m_Hist.nMaxThickness=m_Hist.pnData[nTemp];
					m_Hist.nPeakThickness = nTemp;
				}
				m_Hist.nTotalNumberSamples++;

			};
		};

		if((m_Hist.nTotalNumberSamples>0) && (m_Hist.bMeanLatched!=TRUE)) 
			m_Hist.nAverageThickness /= m_Hist.nTotalNumberSamples;

	}

	if(theApp.m_bCtrlButton == true) {
		switch(theApp.m_LastSettings.nHistogramMask) {
		default:
			break;
		case 1:// m_Hist.fLatchTime = (float)m_Hist.nPeakTime;
	//		m_Hist.fLatchThickness = (m_Hist.fLatchTime * (float)theApp.m_LastSettings.nMaterialVelocity) / 200000.0f;
			m_Hist.fLatchThickness = (float)m_Hist.nPeakThickness / 100.0f;
			break;
		case 2:// m_Hist.fLatchTime = (float)m_Hist.nAverageTime;
	//		m_Hist.fLatchThickness = (m_Hist.fLatchTime * (float)theApp.m_LastSettings.nMaterialVelocity) / 200000.0f;
			m_Hist.fLatchThickness = (float)m_Hist.nAverageThickness / 100.0f;
			break;
		case 4:// m_Hist.fLatchTime = (float)m_Hist.nAverageTime;
	//		m_Hist.fLatchThickness = (m_Hist.fLatchTime * (float)theApp.m_LastSettings.nMaterialVelocity) / 200000.0f;
			m_Hist.fLatchThickness = (float)m_Hist.nAverageThickness / 100.0f;
			break;
		}
	}

	int nThicknessThresh[3];
	nThicknessThresh[0] = (int)(m_Hist.fLatchThickness * 100.0f);
	nThicknessThresh[1] = (int)((m_Hist.fLatchThickness - theApp.m_LastSettings.fHistogramRelativeThickness[0]) * 100.0f);
	nThicknessThresh[2] = (int)((m_Hist.fLatchThickness  - theApp.m_LastSettings.fHistogramRelativeThickness[1]) * 100.0f);
	m_Hist.nTotalBelowThreshold[1] = m_Hist.nTotalBelowThreshold[0] = 0;

	for(ii=nThicknessThresh[1];ii<nThicknessThresh[0];ii++) m_Hist.nTotalBelowThreshold[0] += m_Hist.pnData[ii];
	for(ii=nThicknessThresh[2];ii<nThicknessThresh[1];ii++) m_Hist.nTotalBelowThreshold[1] += m_Hist.pnData[ii];

	

	//Find Maximum width and height of a feature
	m_Hist.fAverageAmp = (float)MulDiv(m_Hist.nAverageAmp-128,100,127);
	theApp.m_LastSettings.fHistogramAmpThreshold = (float)MulDiv(m_Hist.nSampleThreshold[0],100,127);
	m_Hist.nSampleThreshold[0]+=128;
	m_Hist.rrThreshWidth.SetRectEmpty();
	m_Hist.rrThreshHeight.SetRectEmpty();

	if(theApp.m_LastSettings.bHistogramMaxWidth) {
		nMaxLength=0;
		for(yy=rr.top;yy<=rr.bottom;yy++) {
			for(xx=rr.left;xx<rr.right;xx++) {
				nTemp = GetFilteredTimeThickness(xx,yy,m_Hist.nImageNumber,1);
				if(nTemp<m_Hist.nSampleThreshold[0]) {
					nX0=xx;
					nX1=xx+1;
					for(xx;xx<rr.right;xx++) {
						nTemp = GetFilteredTimeThickness(xx,yy,m_Hist.nImageNumber,1);
						if(nTemp<m_Hist.nSampleThreshold[0]) {
							nX1=xx;
							if(nMaxLength<(nX1-nX0)) {
								nMaxLength=nX1-nX0;
								m_Hist.rrThreshWidth.left=nX0;
								m_Hist.rrThreshWidth.right=nX1;
								m_Hist.rrThreshWidth.top = m_Hist.rrThreshWidth.bottom = yy;
							}
						} else {
							break;
						}
					}
				}
			}
		}
	}

	if(theApp.m_LastSettings.bHistogramMaxHeight) {
		nMaxLength=0;
		for(xx=rr.left;xx<rr.right;xx++) {
			for(yy=rr.top;yy<=rr.bottom;yy++) {
				nTemp = GetFilteredTimeThickness(xx,yy,m_Hist.nImageNumber,1);
				if(nTemp<m_Hist.nSampleThreshold[0]) {
					nY0=yy;
					nY1=yy+1;
					for(yy;yy<rr.bottom;yy++) {
						nTemp = GetFilteredTimeThickness(xx,yy,m_Hist.nImageNumber,1);
						if(nTemp<m_Hist.nSampleThreshold[0]) {
							nY1=yy;
							if(nMaxLength<(nY1-nY0)) {
								nMaxLength=nY1-nY0;
								m_Hist.rrThreshHeight.left=m_Hist.rrThreshHeight.right=xx;
								m_Hist.rrThreshHeight.top = nY0;
								m_Hist.rrThreshHeight.bottom = nY1;
							}
						} else {
							break;
						}
					}
				}
			}
		}
	}
	return TRUE;


}


bool CData::BuildExtHistogramData()
{
	int	xx,yy,nTemp,ii;
	float fStepSize = theApp.m_LastSettings.fHistogramStepAmplitude;
	int nArraySize = m_nSamplesLine * m_nNumberLines;
	int	nMaxLength;
	int	nX0,nX1,nY0,nY1;
	bool bUseSample=false;;

	CRect rr = m_Hist.rr;

	if(rr.top<0) rr.top=0;
	if(rr.left<0) rr.left = 0;

	if(m_Hist.nCoordL!=65536) {
		SAFE_DELETE(m_Hist.pnData);
	}

	if(theApp.m_LastSettings.fHistogramRelativedB[0] <= 0.0f) {
		m_Hist.nAmpMode = HIST_ATTENUATION;
	} else {
		m_Hist.nAmpMode = HIST_REFLECTION;
	}

	m_Hist.nCoordL=65536;
	if(m_Hist.pnData==NULL)	m_Hist.pnData = new int[m_Hist.nCoordL];
	ZeroMemory(m_Hist.pnData,m_Hist.nCoordL * sizeof (int));
	m_Hist.nMaxAmp = 0;
	m_Hist.nPeakAmp=0;
	m_Hist.nMaxValue = 0;
	m_Hist.nMinValue = 65536;
	m_Hist.nTotalNumberSamples=0;
	if(m_Hist.bMeanLatched!=TRUE) m_Hist.nAverageAmp=0;

	MinMax(&rr.top,0,m_nNumberLines);
	MinMax(&rr.bottom,0,m_nNumberLines);

	for(yy=rr.top;yy<rr.bottom;yy++) {
		for(xx=rr.left;xx<rr.right;xx++) {
			if(theApp.m_LastSettings.nHistogramStyle) {
				if((hypot(abs((xx-rr.left)-(rr.Width()/2)),abs((yy-rr.top)-(rr.Height()/2)))) <= (rr.Width()/2) )
					bUseSample = true;
				else
					bUseSample = false;
			}else{
				bUseSample =true;
			}

			if(bUseSample){
		//?????????
				nTemp = GetFilteredAmp(xx,yy,m_Hist.nImageNumber,EXT_IMAGE);
				nTemp=MinMax(nTemp,0,65535);
				m_Hist.pnData[nTemp]++;
				if(m_Hist.bMeanLatched!=TRUE) m_Hist.nAverageAmp+=nTemp;
				if(m_Hist.pnData[nTemp]>m_Hist.nMaxAmp) {
					m_Hist.nMaxAmp=m_Hist.pnData[nTemp];
					m_Hist.nPeakAmp = nTemp;
				}
				if(nTemp<m_Hist.nMinValue) {
					m_Hist.nMinValue=nTemp;
				}
				if(nTemp>m_Hist.nMaxValue) {
					m_Hist.nMaxValue=nTemp;
				}
				m_Hist.nTotalNumberSamples++;
			}

		};
	};

	int nHalfMaxAmp = m_Hist.nMaxAmp / 2;
	for(ii=m_Hist.nPeakAmp;ii>0;ii--) {
		if(m_Hist.pnData[ii]<nHalfMaxAmp) {
			m_Hist.nAttenAmp = ii;
			break;
		}
	}

	if(m_Hist.nTotalNumberSamples>0) m_Hist.nAverageAmp /= m_Hist.nTotalNumberSamples;
	return true;

	if(theApp.m_bCtrlButton == true) {
		switch(theApp.m_LastSettings.nHistogramMask) {
		default:
			break;
		case 1: m_Hist.fLatchAmp = ((float)(m_Hist.nPeakAmp) * 100.0f) / 65535.0f;
			break;
		case 2: m_Hist.fLatchAmp = ((float)(m_Hist.nAverageAmp) * 100.0f) / 65535.0f;
			break;
		case 4: m_Hist.fLatchAmp = ((float)(m_Hist.nAttenAmp) * 100.0f) / 65535.0f;
			break;
		}
	}

	switch(m_Hist.nAmplitudeScaleType) {
	case 0:
	case 1: m_Hist.nSampleThreshold[0] = (int)(((m_Hist.fLatchAmp / (float)pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[0]/-20.0f)) * 65535.0f) / 100.0f);
			m_Hist.nSampleThreshold[1] = (int)(((m_Hist.fLatchAmp / (float)pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[1]/-20.0f)) * 65535.0f) / 100.0f);
		break;
	case 2: m_Hist.nSampleThreshold[0] = (int)((m_Hist.fLatchAmp + (theApp.m_LastSettings.fHistogramRelativedB[0]*100.0f/(float)m_nLogRange))*1.27f);
			m_Hist.nSampleThreshold[1] = (int)((m_Hist.fLatchAmp + (theApp.m_LastSettings.fHistogramRelativedB[1]*100.0f/(float)m_nLogRange))*1.27f);
			m_Hist.fMaxValue = ((float)m_Hist.nMaxValue * (float)m_nLogRange / -65535.0f);
			m_Hist.fMinValue = ((float)m_Hist.nMinValue * (float)m_nLogRange / -65535.0f);
		break;
	}

	m_Hist.nSampleThreshold[0]=MinMax(m_Hist.nSampleThreshold[0],0,65535);
	m_Hist.nSampleThreshold[1]=MinMax(m_Hist.nSampleThreshold[1],0,65535);
	m_Hist.nTotalBelowThreshold[0]=0;
	m_Hist.nTotalBelowThreshold[1]=0;
	m_Hist.nTotalAboveThreshold[0]=0;
	m_Hist.nTotalAboveThreshold[1]=0;
	for(ii=0;ii<m_Hist.nSampleThreshold[0];ii++) m_Hist.nTotalBelowThreshold[0] += m_Hist.pnData[ii];
	for(ii=0;ii<m_Hist.nSampleThreshold[1];ii++) m_Hist.nTotalBelowThreshold[1] += m_Hist.pnData[ii];
	if(m_Hist.nSampleThreshold[0]<0) m_Hist.nSampleThreshold[0]=0;
	if(m_Hist.nSampleThreshold[1]<0) m_Hist.nSampleThreshold[1]=0;
	for(ii=m_Hist.nSampleThreshold[0];ii<65536;ii++) m_Hist.nTotalAboveThreshold[0] += m_Hist.pnData[ii];
	for(ii=m_Hist.nSampleThreshold[1];ii<65536;ii++) m_Hist.nTotalAboveThreshold[1] += m_Hist.pnData[ii];

////////////////////////////////////////
// Calculate stats for values between the thresholds
////////////////////////////////////////
	int nAverage = 0;
	int nSum = 0;
	int nAmp;
	for(nAmp = m_Hist.nSampleThreshold[1]; nAmp <= m_Hist.nSampleThreshold[0]; nAmp++) {
		nAverage += (m_Hist.pnData[nAmp] * nAmp);
		nSum += m_Hist.pnData[nAmp];
	}
	m_Hist.fAverageBetween = (float) nAverage / (float)nSum;

	float fAverageAmp = (float)m_Hist.nAverageAmp;
	float fVariance = 0.0;
	float fVarianceBetween = 0.0;

	bUseSample=false;
	int nn,nnBetween;
	if(rr.Width() && rr.Height()) {
		for(yy=rr.top,nn=0,nnBetween=0;yy<rr.bottom;yy++) {
			for(xx=rr.left;xx<rr.right;xx++,nn++) {

				if(theApp.m_LastSettings.nHistogramStyle) {
					if((hypot(abs((xx-rr.left)-(rr.Width()/2)),abs((yy-rr.top)-(rr.Height()/2)))) <= (rr.Width()/2) )
						bUseSample = true;
					else
						bUseSample = false;
				}else{
					bUseSample =true;
				}

				if(bUseSample){

					nAmp = GetFilteredAmp(xx,yy,m_Hist.nImageNumber);
					fVariance += (((float)nAmp-fAverageAmp) * ((float)nAmp-fAverageAmp));

					if((nAmp>=m_Hist.nSampleThreshold[1]) && (nAmp<=m_Hist.nSampleThreshold[0])) {
						fVarianceBetween += (((float)nAmp-m_Hist.fAverageBetween) * ((float)nAmp-m_Hist.fAverageBetween));
						nnBetween++;
					}

				}
			}
		}
		fVariance /= (float)(nn-1);
		m_Hist.fStd = (float)sqrt((double)fVariance);

		fVarianceBetween /= (float)(nnBetween-1);
		m_Hist.fStdBetween = (float)sqrt((double)fVarianceBetween);
	}




	m_Hist.rrThreshWidth.SetRectEmpty();
	m_Hist.rrThreshHeight.SetRectEmpty();
	bUseSample=false;

	nMaxLength=0;
	for(yy=rr.top;yy<rr.bottom;yy++) {
		for(xx=rr.left;xx<rr.right;xx++) {

			if(theApp.m_LastSettings.nHistogramStyle) {
				if((hypot(abs((xx-rr.left)-(rr.Width()/2)),abs((yy-rr.top)-(rr.Height()/2)))) <= (rr.Width()/2) )
					bUseSample = true;
				else
					bUseSample = false;
			}else{
				bUseSample =true;
			}

			if(bUseSample){

				nTemp = GetFilteredAmp(xx,yy,m_Hist.nImageNumber);
				if(((nTemp<=m_Hist.nSampleThreshold[0]) && (m_Hist.nAmpMode == HIST_ATTENUATION)) || 
					((nTemp>=m_Hist.nSampleThreshold[0]) && (m_Hist.nAmpMode == HIST_REFLECTION))) {
					nX0=xx;
					nX1=xx+1;
					for(xx;xx<rr.right;xx++) {
						nTemp = GetFilteredAmp(xx,yy,m_Hist.nImageNumber);
						if(((nTemp<=m_Hist.nSampleThreshold[0]) && (m_Hist.nAmpMode == HIST_ATTENUATION)) || 
							((nTemp>=m_Hist.nSampleThreshold[0]) && (m_Hist.nAmpMode == HIST_REFLECTION))) {

							nX1=xx;
							if(nMaxLength<(nX1-nX0)) {
								nMaxLength=nX1-nX0;
								m_Hist.rrThreshWidth.left=nX0;
								m_Hist.rrThreshWidth.right=nX1;
								m_Hist.rrThreshWidth.top = m_Hist.rrThreshWidth.bottom = yy;
							}
						} else {
							break;
						}
					}
				}
			}
		}
	}

	bUseSample=false;

	nMaxLength=0;
	for(xx=rr.left;xx<rr.right;xx++) {
		for(yy=rr.top;yy<rr.bottom;yy++) {

			if(theApp.m_LastSettings.nHistogramStyle) {
				if((hypot(abs((xx-rr.left)-(rr.Width()/2)),abs((yy-rr.top)-(rr.Height()/2)))) <= (rr.Width()/2) )
					bUseSample = true;
				else
					bUseSample = false;
			}else{
				bUseSample =true;
			}
			if(bUseSample){

				nTemp = GetFilteredAmp(xx,yy,m_Hist.nImageNumber);
				if(((nTemp=m_Hist.nSampleThreshold[0]) && (m_Hist.nAmpMode == HIST_ATTENUATION)) || 
					((nTemp>=m_Hist.nSampleThreshold[0]) && (m_Hist.nAmpMode == HIST_REFLECTION))) {
					nY0=yy;
					nY1=yy+1;
					for(yy;yy<rr.bottom;yy++) {
						nTemp = GetFilteredAmp(xx,yy,m_Hist.nImageNumber);
						if(((nTemp<=m_Hist.nSampleThreshold[0]) && (m_Hist.nAmpMode == HIST_ATTENUATION)) || 
							((nTemp>=m_Hist.nSampleThreshold[0]) && (m_Hist.nAmpMode == HIST_REFLECTION))) {

							nY1=yy;
							if(nMaxLength<(nY1-nY0)) {
								nMaxLength=nY1-nY0;
								m_Hist.rrThreshHeight.left=m_Hist.rrThreshHeight.right=xx;
								m_Hist.rrThreshHeight.top = nY0;
								m_Hist.rrThreshHeight.bottom = nY1;
							}
						} else {
							break;
						}
					}
				}
			}
		}
	}
	return TRUE;

}




void CData::CalculateNumberOfImages()
{
	int	nTS,gg;

	switch(m_nScanType) {
	default:
		if(theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) {
			m_nNumberTimeSlots = 1;
		} else {
			m_nNumberTimeSlots = m_Global.Mux.nMaxTimeslots;
		}
		break;
	case MULTI_PROBE_SCAN: m_nNumberTimeSlots = 1;
		break;
	}


	m_nNumberImages=0;
	m_nNumberVolumeImages=0;
	m_nNumberExtImages = 0;
	ZeroMemory(m_nNumberAmpImages, sizeof m_nNumberAmpImages);
	ZeroMemory(m_nNumberTimeImages, sizeof m_nNumberTimeImages);
	ZeroMemory(m_nNumberPhaseImages, sizeof m_nNumberPhaseImages);
	for(nTS=0;nTS<m_nNumberTimeSlots;nTS++) {
		m_nNumberAmpImages[nTS]=0;
		m_nNumberTimeImages[nTS]=0;
		m_nNumberPhaseImages[nTS]=0;
		for(gg=0;gg<8;gg++) {
			if((m_TS[nTS].Gate.nCollectAmp[gg]>0)&&(m_TS[nTS].nScanLine==0)) m_nNumberAmpImages[nTS]++;
			if((m_TS[nTS].Gate.nCollectTime[gg]>0)&&(m_TS[nTS].nScanLine==0)) m_nNumberTimeImages[nTS]++;
			if((m_TS[nTS].Gate.nCollectPhase[gg]>0)&&(m_TS[nTS].nScanLine==0)) m_nNumberPhaseImages[nTS]++;
		}
		if((m_TS[nTS].Adc.nBscanMode)&&(m_TS[nTS].nScanLine==0)) {
			m_nNumberVolumeImages++;
		}
		m_nNumberImages += m_nNumberAmpImages[nTS];
		m_nNumberImages += m_nNumberTimeImages[nTS];
		m_nNumberImages += m_nNumberPhaseImages[nTS];
	}
	for(gg=0;gg<8;gg++) {
		if(m_Global.Ext.bCollect[gg] == TRUE) m_nNumberExtImages++;
	}
	m_nNumberImages += m_nNumberExtImages;
}

void CData::AllocateImageBuffers()
{
	int nTS,gg,tt,pp,nn;
	int	nSize = m_nSamplesLine * (m_nNumberLines + 1);	//one extra line for extra memory for the line shifting
	m_nImageNameL=0;
	int ii,hh;

	gg=tt=pp=0;
	for(nTS=0;nTS<m_nNumberTimeSlots;nTS++) {
		for(nn=0;nn<m_nNumberAmpImages[nTS];nn++) {
			m_cAmpBuffer[gg] = new unsigned char[nSize];
			FillMemory(m_cAmpBuffer[gg++],nSize,INIT_VALUE);
		
			if(theApp.m_LastSettings.nViewTabTSName)
				m_ImageName[gg-1] = m_TS[nTS].cName;
			if(theApp.m_LastSettings.nViewTabGateName) {
				for(hh=0,ii=0;hh<8;hh++) {
					if(m_TS[nTS].Gate.nCollectAmp[gg]) {
						if(ii==nn) {
							m_ImageName[gg-1] += " ";
							m_ImageName[gg-1] += m_TS[nTS].Gate.cName[hh];
						}
						ii++;
					}
				}
			}	
			m_nImageNameL++;
		};
		for(nn=0;nn<m_nNumberTimeImages[nTS];nn++) {
			m_nTimeBuffer[tt] = new unsigned int[nSize];
			ZeroMemory(m_nTimeBuffer[tt++],nSize * sizeof(int));
		}
		for(nn=0;nn<m_nNumberPhaseImages[nTS];nn++) {
			m_cPhaseBuffer[pp++] = new unsigned char[nSize];
		}
	}
	for(nn=0;nn<m_nNumberExtImages;nn++) {
		m_16ExtBuffer[nn] = new unsigned __int16[nSize];
		ZeroMemory(m_16ExtBuffer[nn],nSize * sizeof(__int16));
	}

	if(m_nSamplesLine>0) {
		m_pCpPos = new CCoord[m_nSamplesLine];
	} else {
		m_nSamplesLine = 0;
	}

	SAFE_DELETE( m_nLineScannedStatus );
	if(m_nNumberLines>0) {
		m_nLineScannedStatus = new int[m_nNumberLines];
		ZeroMemory(m_nLineScannedStatus, m_nNumberLines * sizeof(int));
	} else {
		m_nNumberLines = 0;
	}
}

void CData::CalculateStandardFileLength()
{
	int nTS;

	// calculate it if it hasn't already been calculated either by
	// AllocateVolumeBuffersAndOffsets()
	// CalculateToolActualPathOffsets()

	if(m_n64FileLength == 0) {
		UINT64 nSize = (UINT64)m_nSamplesLine * (UINT64)m_nNumberLines;
		UINT64 nInitialFileLength = (UINT64)(0x8000 + sizeof m_TS + sizeof m_Global + (8 * 0x200));	//This is just an approximation

		for(nTS=0;nTS<m_nNumberTimeSlots;nTS++) {
			nInitialFileLength += (UINT64)(nSize * m_nNumberAmpImages[nTS] * sizeof (unsigned char));
			nInitialFileLength += (UINT64)(nSize * m_nNumberTimeImages[nTS] * sizeof (unsigned int));
			nInitialFileLength += (UINT64)(nSize * m_nNumberPhaseImages[nTS] * sizeof (unsigned char));
		}
		nInitialFileLength += (UINT64)(nSize * m_nNumberExtImages * sizeof (unsigned __int16));

		nInitialFileLength += 1000000;		//Add a million for annotation, Details[200], Image offset, Polygon

		for(int nn=0;nn<m_nNumberLines;nn++) {
			if(m_vLineSample) {
				nInitialFileLength += m_vLineSample[nn].GetSpaceTakenInFile();
			}
		}

		m_n64FileLength = nInitialFileLength + 1000000;	//Add a 10 million for good measure
	}
}


void CData::AllocateVolumeBuffersAndOffsets()
{
	int /*nTS,*/nAscanLength;
	int bb=0,jj=0;
	int ii;

	InvalidateVolBytesPerSample();

	for(ii=0;ii<NUMBER_VOLUME_BUFFERS;ii++) {
		for(int jj=0; jj<256; jj++){ //Multiple lines/probes
			SAFE_DELETE( m_cVolumeBuffer[ii][jj] );
		}
	}

	CalculateStandardFileLength();

	ZeroMemory(m_n64VolumeOffset,sizeof m_n64VolumeOffset);

	for(int nImageNumber=0;nImageNumber<m_nNumberVolumeImages;nImageNumber++) {
		if(m_TS[0].Adc.nBscanMode) {
			nAscanLength = m_TS[0].Adc.nBscanLength;
			//Each image may be constructed from multiple probes(or virtual probes) so need to create a buffer to put these
			for(int ii=0; ii<m_nNumberProbesPerImage; ii++){ //needs sorting //TODO
				m_cVolumeBuffer[bb][ii] = new unsigned char[nAscanLength * m_nSamplesLine * m_nVolBytesPerSample];
				ZeroMemory(m_cVolumeBuffer[bb][ii],nAscanLength * m_nSamplesLine * m_nVolBytesPerSample);
			}
			m_n64VolumeOffset[bb] = m_n64FileLength;
			m_n64FileLength += (((UINT64)nAscanLength * (UINT64)m_nSamplesLine * (UINT64)m_nNumberLines * (UINT64)m_nVolBytesPerSample) + 0x200);
			bb++;
		}
	}
	m_n64FileLength += 1000000;

}

void CData::SaveScanLines()
{
	CUSLFile File;
	CFileException e;
	CString Buff,Language;

	CalculateStandardFileLength();

	if(m_nCollectScanLines) {

		if(!File.Open(m_FilePath,CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary | CFile::modeNoTruncate, &e)) {
			File.DecodeFileException(&e,&Buff,m_FilePath);
			Language.LoadString(IDS_Error);
			MessageBox(NULL,Buff,Language,MB_ICONERROR);
		} else {
			File.Seek(m_n64ScanLinesOffset = m_n64FileLength,CUSLFile::begin);
			for(int nLine = PROFILE->m_nScanStartLine;nLine < PROFILE->m_nScanFinishLine; nLine += PROFILE->m_nLineIncrement) {
				PROFILE->m_ScanLine[nLine].Save(&File);
			}
			m_n64ToolPathOffset = m_n64FileLength = File.GetLength() + 0x200;
			File.Close();
		}

	}

}

void CData::AllocateToolPathAndOffsets()
{

	CalculateStandardFileLength();


}


void CData::CollectComplexData()
{
	switch(theApp.m_Thread.m_nThreadAction) {
	default:
		if(theApp.m_LastSettings.nDesiredDevice==NOT_PHASED_ARRAY) {
//			if(m_nNumberProbesPerImage<=1) {
//				CollectComplexDataSingleProbe();
//			} else {
				CollectComplexDataWithProbeOffsets();
//			}
		} else {
			CollectComplexDataPhasedArray();
		}
		break;
	case MULTI_PROBE_SCAN:
		CollectComplexDataMultiProbeScan();
		break;
	}
}

void CData::CollectComplexDataWithProbeOffsets()
{
	CCoord CpHead,CpNominalSurface,CpSurface;
	int nn,gg,tt,pp,bb,nTS,nPtr0,nPtr1,nPtr;
	int	nGStop,nGStart,nPath,nTrigPos,nAxis;
	int	nStart,nFinish,nVolumePtr,nSample,nLine,nNominalLine;
	int nLookAhead;

	m_bModified = TRUE;
	int	nArraySize = m_nSamplesLine * m_nNumberLines;

	theApp.m_Motors.GetSurfacePos(&CpNominalSurface);

	switch(m_nCollectAxis) {
	default: nAxis = m_nCollectAxis;
		nLookAhead = 1;
		break;
	case 5:
	case 6:
		nLookAhead = 10;
		switch(theApp.m_nUseTurntableOrRollers) {
		default: nAxis = 5;
			break;
		case 1: nAxis = 15;
			break;
		}
		break;
	}

	if(theApp.m_Thread.m_bScanLineUpdateByMotorRevCounter!=true) { 

		if(PROFILE->m_nLineIncrementMode == FIXED_INC) {
			nNominalLine = (theApp.m_Thread.m_nScanLine-PROFILE->m_nScanStartLine)/PROFILE->m_nLineIncrement;
		} else {
			nNominalLine = theApp.m_Thread.m_nScanLine-PROFILE->m_nScanStartLine;
		}
	} else {
		nNominalLine = theApp.m_Thread.m_nScanLine = theApp.m_Motors.GetRevCount();
	}
	MinMax(&nNominalLine,0,m_nNumberLines-1);


	for(nTS=0;nTS<m_nNumberTimeSlots;nTS++) {
		if(nTS==0 || (m_TS[nTS].nScanLine != m_TS[nTS-1].nScanLine)) {
			gg=tt=pp=bb=0;
		}


		CpSurface = CpNominalSurface;
		CpSurface.Side0.pt.x += theApp.m_Kinematics.m_Probe[ m_TS[nTS].nScanLine ].fX;
		CpSurface.Side0.pt.y += theApp.m_Kinematics.m_Probe[ m_TS[nTS].nScanLine ].fY;
		CpSurface.Side0.pt.z += theApp.m_Kinematics.m_Probe[ m_TS[nTS].nScanLine ].fZ;

		nSample = (int)((CpSurface.fPos[nAxis]-m_CpStart.fPos[nAxis]) * (float)(m_nSamplesLine-1) / (m_CpFinish.fPos[nAxis] - m_CpStart.fPos[nAxis]));
	
		nLine = nNominalLine + m_TS[nTS].nScanLine;
		MinMax(&nLine,0,m_nNumberLines-1);

		if(	m_vLineSample != NULL && m_nCollectAxis != 5) {
			if(	m_vLineSample[nLine].m_nCoordL>=2) {
				m_vLineSample[nLine].FindNearestSegment(CpSurface,PORTSIDE,&nSample);
				nSample += m_vLineSample[nLine].m_nFirstSample;
			}
		}
		MinMax(&nSample,0,m_nSamplesLine-1);

		m_nPtr[ m_TS[nTS].nScanLine ] = (m_nSamplesLine * nLine) + nSample;
		MinMax(&m_nPtr[ m_TS[nTS].nScanLine ],0,nArraySize - 1);
		MinMax(m_nLastPtr[ m_TS[nTS].nScanLine ], 0, m_nPtr[ m_TS[nTS].nScanLine ]);

		nPtr0 = m_nPtr[ m_TS[nTS].nScanLine ];
		theApp.m_Thread.m_nDir==0 ? nPtr1 = m_nPtr[ m_TS[nTS].nScanLine ]+ nLookAhead :	nPtr1 = m_nPtr[ m_TS[nTS].nScanLine ]- nLookAhead;
		MinMax(&nPtr1,0,nArraySize - 1);
		if (nPtr1 < nPtr0) {
			int nTemp = nPtr0;
			nPtr0 = nPtr1;
			nPtr1 = nTemp;
		}
		m_pCpPos[nSample] = CpSurface;

		theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
		PROFILE->ModifySurfaceLine(CpSurface,MulDiv(nSample,(m_nSamplesLine-1)/10,(m_nSamplesLine-1)),nLine);

		for(nn=0;nn<8;nn++) {
			if(m_TS[nTS].Gate.nCollectAmp[nn]>0) {

				for(nPtr=nPtr0;nPtr<=nPtr1;nPtr++)
					m_cAmpBuffer[gg][nPtr] = theApp.m_UtUser.m_TS[nTS].Gate.nAmplitude[nn]+128;

				gg++;
			}
			if(m_TS[nTS].Gate.nCollectTime[nn]>0) {
				nGStop = theApp.m_UtUser.m_TS[nTS].Gate.nTOFStopGate[nn];
				nGStart = theApp.m_UtUser.m_TS[nTS].Gate.nTOFStartGate[nn];
				if((theApp.m_UtUser.m_TS[nTS].Gate.nTimeStatus[nGStart-1]>0) && (theApp.m_UtUser.m_TS[nTS].Gate.nTimeStatus[nGStop]>0)) {
					nPath = (int)((float)(theApp.m_UtUser.m_TS[nTS].Gate.nTimeSample[nGStop] - theApp.m_UtUser.m_TS[nTS].Gate.nTimeSample[nGStart-1]) * theApp.m_UtUser.m_TS[nTS].Adc.fSamplePeriod);
				} else {
					switch(theApp.m_LastSettings.nDefaultTOF) {
					default: nPath = -1;
						break;
					case 1:	nPath = 0;
						break;
					case 2: nPath = theApp.m_UtUser.m_TS[nTS].Gate.nNsDelay[nGStart];
						break;
					case 3: nPath = theApp.m_UtUser.m_TS[nTS].Gate.nNsDelay[nGStart] + theApp.m_UtUser.m_TS[nTS].Gate.nNsWidth[nGStop];
						break;
					};
				};
				m_nTimeBuffer[tt][nPtr0] = m_nTimeBuffer[tt][nPtr1] = nPath;

				tt++;
			}
			
			if((m_TS[nTS].Gate.nCollectPhase[nn]>0) && (pp < m_nNumberPhaseImages[nTS])) {
				m_cPhaseBuffer[pp][nPtr0] = (char) theApp.m_UtUser.m_TS[nTS].Gate.nPhase[nn];
				pp++;
			}

		}
		switch(m_TS[nTS].Adc.nBscanMode) {
		case 0:
			break;
		case 1:
			nVolumePtr=nSample * theApp.m_UtUser.m_TS[nTS].Adc.nBscanLength;
			nTrigPos=0;

			if(m_nVolBytesPerSample == 1) {
				memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr],&nTrigPos,sizeof (int));
				memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr + 4],&theApp.m_Scope.m_RFTrace[nTS][theApp.m_Scope.m_nAdcIndexFirstPoint[nTS]],theApp.m_UtUser.m_TS[nTS].Adc.nBscanLength - 4);
			} else {
				memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr * m_nVolBytesPerSample],&nTrigPos,sizeof (int));
				memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr * m_nVolBytesPerSample + 4],&theApp.m_Scope.m_wRFTrace[nTS][theApp.m_Scope.m_nAdcIndexFirstPoint[nTS]],(theApp.m_UtUser.m_TS[nTS].Adc.nBscanLength *m_nVolBytesPerSample) - 4);
			}
			bb++;
			break;
		case 2:
			nVolumePtr=nSample * theApp.m_UtUser.m_TS[nTS].Adc.nBscanLength;
			nTrigPos=theApp.m_UtUser.m_TS[nTS].Dsp.InterfacePos;
			nStart=nTrigPos-(int)((float)theApp.m_UtUser.m_TS[nTS].Adc.nBscanPreTrigger/theApp.m_UtUser.m_TS[nTS].Adc.fSamplePeriod);
			nFinish=nTrigPos+(int)((float)theApp.m_UtUser.m_TS[nTS].Adc.nBscanPostTrigger/theApp.m_UtUser.m_TS[nTS].Adc.fSamplePeriod);
			if(nStart<0) nStart=0;
			if(nStart>=(int)theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength) nStart=theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength-1;
			if(nFinish<nStart) nFinish=nStart;
			if(nFinish>=(int)theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength) nFinish=theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength-1;
			if(m_nVolBytesPerSample == 1) {
				memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr],&nStart,sizeof (int));
				memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr + 4],&theApp.m_Scope.m_RFTrace[nTS][theApp.m_Scope.m_nAdcIndexFirstPoint[nTS]+nStart],nFinish-nStart);
			} else {
				memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr * m_nVolBytesPerSample],&nStart,sizeof (int));
				memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr * m_nVolBytesPerSample + 4],&theApp.m_Scope.m_wRFTrace[nTS][theApp.m_Scope.m_nAdcIndexFirstPoint[nTS]+nStart],(nFinish-nStart) * m_nVolBytesPerSample);
			}
			bb++;
			break;
		}
	}

	gg=0;
	for(nn=0;nn<6;nn++) {
		if(m_Global.Ext.bCollect[nn]>0) {
			if(nn<4) {
				m_16ExtBuffer[gg][nPtr0] = m_16ExtBuffer[gg][nPtr1] = theApp.m_DataTranslationHW.m_nExtADCArray[nn];
			} else {
				m_16ExtBuffer[gg][nPtr0] = m_16ExtBuffer[gg][nPtr1] = theApp.m_Si10User.ReadJoystick(nn-4) << 7;
			};
			if(((m_nCollectAxis==5) || (m_nCollectAxis==6)) && (m_nLastLine == theApp.m_Thread.m_nScanLine) && (theApp.m_Thread.m_bScanLineUpdateByMotorRevCounter == true)) {
				if(theApp.m_Thread.m_nScanLine < (m_nNumberLines-1)) {
					m_16ExtBuffer[gg][nPtr0+m_nSamplesLine] = m_16ExtBuffer[gg][nPtr0];
				}
			}
			gg++;
		}
	}

	CopyMemory(m_nLastPtr, m_nPtr, sizeof m_nLastPtr);
	m_nLastLine = theApp.m_Thread.m_nScanLine;
}


void CData::CollectComplexDataSingleProbe()
{
	CCoord CpHead,CpSurface;
	CPolyCoord* pLine = &PROFILE->m_ToolPath;
	int nn,gg,tt,pp,bb,nTS,nPtr0,nPtr1;
	int	nGStop,nGStart,nPath,nTrigPos,nAxis;
	int	nStart,nFinish,nVolumePtr,nSample,nLine;

	m_bModified = TRUE;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Motors.GetHeadPos(&CpHead);
		theApp.m_Kinematics.NormalsFromTips(&CpHead,PORTSIDE);
		theApp.m_Kinematics.NormalsFromTips(&CpHead,STARBOARD);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		theApp.m_Ethercat.InvalidateRobotJoints();
		theApp.m_Motors.GetSurfacePos(&CpSurface);
		break;
	}


	if(PROFILE->m_nLineIncrementMode == FIXED_INC) {
		nLine = (theApp.m_Thread.m_nScanLine-PROFILE->m_nScanStartLine)/PROFILE->m_nLineIncrement;
	} else {
		nLine = theApp.m_Thread.m_nScanLine-PROFILE->m_nScanStartLine;
	}
	MinMax(&nLine,0,m_nNumberLines-1);

	switch(m_nCollectAxis) {
	default: nAxis = m_nCollectAxis;
		break;
	case 5:
	case 6:
		switch(theApp.m_nUseTurntableOrRollers) {
		default: nAxis = 5;
			break;
		case 1: nAxis = 15;
			break;
		}
	}

	nSample = (int)((CpSurface.fPos[nAxis]-m_CpStart.fPos[nAxis]) * (float)(m_nSamplesLine-1) / (m_CpFinish.fPos[nAxis] - m_CpStart.fPos[nAxis]));
	
	if(	m_vLineSample != NULL) {
		if(	m_vLineSample[nLine].m_nCoordL>=2) {
			m_vLineSample[nLine].FindNearestSegment(CpSurface,PORTSIDE,&nSample);
			nSample += m_vLineSample[nLine].m_nFirstSample;
		}
	}
	MinMax(&nSample,0,m_nSamplesLine-1);

	if(theApp.m_Thread.m_bScanLineUpdateByMotorRevCounter==true) 
		theApp.m_Thread.m_nScanLine = theApp.m_Motors.GetRevCount();

	int	nSize = m_nSamplesLine * m_nNumberLines;

	nPtr0 = (m_nSamplesLine * nLine) + nSample;
	MinMax(&nPtr0,0,nSize - 1);

	if(m_nLastPtr[0]<0) m_nLastPtr[0] = 0;
	if(m_nLastPtr[0]>nPtr0) m_nLastPtr[0] = nPtr0;

	if(theApp.m_Thread.m_nDir==0) {
		nPtr1 = nPtr0+1;
	} else {
		nPtr1 = nPtr0-1;
	}
	MinMax(&nPtr1,0,nSize - 1);

	m_pCpPos[nSample] = CpSurface;

	theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
	theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
	theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
	PROFILE->ModifySurfaceLine(CpSurface,MulDiv(nSample,(m_nSamplesLine-1)/10,(m_nSamplesLine-1)),nLine);

	gg=tt=pp=bb=0;
	for(nTS=0;nTS<m_nNumberTimeSlots;nTS++) {
		for(nn=0;nn<8;nn++) {
			if(m_TS[nTS].Gate.nCollectAmp[nn]>0) {
				m_cAmpBuffer[gg][nPtr0] =m_cAmpBuffer[gg][nPtr1] = theApp.m_UtUser.m_TS[nTS].Gate.nAmplitude[nn]+128;
				//Automatically fill if in continuous mode because they will scan faster than the prf
				//Dont fill in segmented mode
				if(((m_nCollectAxis==5) || (m_nCollectAxis==6)) && (m_nLastLine == theApp.m_Thread.m_nScanLine) && (theApp.m_Thread.m_bScanLineUpdateByMotorRevCounter == true)) {
//					if(abs(nPtr0 - m_nLastPtr[0]) < 50) {
//						for(nPtr=m_nLastPtr[0]+1;nPtr<nPtr0;nPtr++) {
//							m_cAmpBuffer[gg][nPtr] = m_cAmpBuffer[gg][nPtr0];
							if(theApp.m_Thread.m_nScanLine < (m_nNumberLines-1)) {
								m_cAmpBuffer[gg][nPtr0+m_nSamplesLine] = m_cAmpBuffer[gg][nPtr0];
							}
	//					}
	//				}
				}
				
				gg++;
			}
			if(m_TS[nTS].Gate.nCollectTime[nn]>0) {
				nGStop = theApp.m_UtUser.m_TS[nTS].Gate.nTOFStopGate[nn];
				nGStart = theApp.m_UtUser.m_TS[nTS].Gate.nTOFStartGate[nn];
				if((theApp.m_UtUser.m_TS[nTS].Gate.nTimeStatus[nGStart-1]>0) && (theApp.m_UtUser.m_TS[nTS].Gate.nTimeStatus[nGStop]>0)) {
					nPath = (int)((float)(theApp.m_UtUser.m_TS[nTS].Gate.nTimeSample[nGStop] - theApp.m_UtUser.m_TS[nTS].Gate.nTimeSample[nGStart-1]) * theApp.m_UtUser.m_TS[nTS].Adc.fSamplePeriod);
				} else {
					switch(theApp.m_LastSettings.nDefaultTOF) {
					default: nPath = -1;
						break;
					case 1:	nPath = 0;
						break;
					case 2: nPath = theApp.m_UtUser.m_TS[nTS].Gate.nNsDelay[nGStart];
						break;
					case 3: nPath = theApp.m_UtUser.m_TS[nTS].Gate.nNsDelay[nGStart] + theApp.m_UtUser.m_TS[nTS].Gate.nNsWidth[nGStop];
						break;
					};
				};
				m_nTimeBuffer[tt][nPtr0] = m_nTimeBuffer[tt][nPtr1] = nPath;

				tt++;
			}
			
			if((m_TS[nTS].Gate.nCollectPhase[nn]>0) && (pp < m_nNumberPhaseImages[nTS])) {
				m_cPhaseBuffer[pp][nPtr0] = (char) theApp.m_UtUser.m_TS[nTS].Gate.nPhase[nn];
				pp++;
			}

		}
		switch(m_TS[nTS].Adc.nBscanMode) {
		case 0:
			break;
		case 1:
			nVolumePtr=nSample * theApp.m_UtUser.m_TS[nTS].Adc.nBscanLength;
			nTrigPos=0;

			if(m_nVolBytesPerSample == 1) {
				memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr],&nTrigPos,sizeof (int));
				memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr + 4],&theApp.m_Scope.m_RFTrace[nTS][theApp.m_Scope.m_nAdcIndexFirstPoint[nTS]],theApp.m_UtUser.m_TS[nTS].Adc.nBscanLength - 4);
			} else {
				memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr * m_nVolBytesPerSample],&nTrigPos,sizeof (int));
				memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr * m_nVolBytesPerSample + 4],&theApp.m_Scope.m_wRFTrace[nTS][theApp.m_Scope.m_nAdcIndexFirstPoint[nTS]],(theApp.m_UtUser.m_TS[nTS].Adc.nBscanLength *m_nVolBytesPerSample) - 4);
			}
			bb++;
			break;
		case 2:
			nVolumePtr=nSample * theApp.m_UtUser.m_TS[nTS].Adc.nBscanLength;
			nTrigPos=theApp.m_UtUser.m_TS[nTS].Dsp.InterfacePos;
			nStart=nTrigPos-(int)((float)theApp.m_UtUser.m_TS[nTS].Adc.nBscanPreTrigger/theApp.m_UtUser.m_TS[nTS].Adc.fSamplePeriod);
			nFinish=nTrigPos+(int)((float)theApp.m_UtUser.m_TS[nTS].Adc.nBscanPostTrigger/theApp.m_UtUser.m_TS[nTS].Adc.fSamplePeriod);
			if(nStart<0) nStart=0;
			if(nStart>=(int)theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength) nStart=theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength-1;
			if(nFinish<nStart) nFinish=nStart;
			if(nFinish>=(int)theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength) nFinish=theApp.m_UtUser.m_TS[nTS].Adc.nAcqLength-1;
			if(m_nVolBytesPerSample == 1) {
				memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr],&nStart,sizeof (int));
				memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr + 4],&theApp.m_Scope.m_RFTrace[nTS][theApp.m_Scope.m_nAdcIndexFirstPoint[nTS]+nStart],nFinish-nStart);
			} else {
				memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr * m_nVolBytesPerSample],&nStart,sizeof (int));
				memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr * m_nVolBytesPerSample + 4],&theApp.m_Scope.m_wRFTrace[nTS][theApp.m_Scope.m_nAdcIndexFirstPoint[nTS]+nStart],(nFinish-nStart) * m_nVolBytesPerSample);
			}
			bb++;
			break;
		}

	}

	gg=0;
	for(nn=0;nn<4;nn++) {
		if(m_Global.Ext.bCollect[nn]>0) {
			if(nn<4) {
				m_16ExtBuffer[gg][nPtr0] = m_16ExtBuffer[gg][nPtr1] = theApp.m_DataTranslationHW.m_nExtADCArray[nn];
			} else {
				m_16ExtBuffer[gg][nPtr0] = m_16ExtBuffer[gg][nPtr1] = theApp.m_Si10User.ReadJoystick(nn-4) << 7;
			}
			if(((m_nCollectAxis==5) || (m_nCollectAxis==6)) && (m_nLastLine == theApp.m_Thread.m_nScanLine) && (theApp.m_Thread.m_bScanLineUpdateByMotorRevCounter == true)) {
				if(theApp.m_Thread.m_nScanLine < (m_nNumberLines-1)) {
					m_16ExtBuffer[gg][nPtr0+m_nSamplesLine] = m_16ExtBuffer[gg][nPtr0];
				}
			}
			gg++;
		}
	}

	m_nLastPtr[0] = nPtr0;
	m_nLastLine = theApp.m_Thread.m_nScanLine;
}



void CData::CollectComplexDataPhasedArray()
{
	CCoord CpHead,CpSurface,CpFirstSurface;
	CPolyCoord* pLine = &PROFILE->m_ToolPath;
	int nn,gg,tt,pp,nPtr0,nPtr1,nFl;
	int	nGStop,nGStart,nPath,nTrigPos,nAxis;
	int	nStart,nFinish,nVolumePtr,nPtr,nSample,nLine;

	m_bModified = TRUE;

	theApp.m_Motors.GetSurfacePos(&CpFirstSurface,0);

	for(nFl = 0; nFl < m_nNumberProbesPerImage; nFl++) {

		CpSurface = CpFirstSurface;
		CpSurface.Side0.pt += theApp.m_PhasedArray[0].m_FL[nFl].vecPt;


		if(PROFILE->m_nLineIncrementMode == FIXED_INC) {
			nLine = nFl + (theApp.m_Thread.m_nScanLine-PROFILE->m_nScanStartLine)/PROFILE->m_nLineIncrement;
		} else {
			nLine = nFl + theApp.m_Thread.m_nScanLine-PROFILE->m_nScanStartLine;
		}
		if(nLine>=m_nNumberLines) {
			break;
		}

		switch(m_nCollectAxis) {
		default: nAxis = m_nCollectAxis;
			break;
		case 5:
			switch(theApp.m_nUseTurntableOrRollers) {
			default: nAxis = 5;
				break;
			case 1: nAxis = 15;
				break;
			}
		}

		nSample = (int)((CpSurface.fPos[nAxis]-m_CpStart.fPos[m_nCollectAxis]) * (float)(m_nSamplesLine-1) / (m_CpFinish.fPos[m_nCollectAxis] - m_CpStart.fPos[m_nCollectAxis]));
		
		if(	m_vLineSample ) {
			if(	m_vLineSample[nLine].m_nCoordL>=2) {
				m_vLineSample[nLine].FindNearestSegment(CpSurface,PORTSIDE,&nSample);
				nSample += m_vLineSample[nLine].m_nFirstSample;
			}
		}
		MinMax(&nSample, 0, m_nSamplesLine - 1);

		if(theApp.m_Thread.m_bScanLineUpdateByMotorRevCounter==true) 
			theApp.m_Thread.m_nScanLine = theApp.m_Motors.GetRevCount();

		if(theApp.m_Thread.m_nDir==0) {
			nPtr0 = nSample;
			nPtr1 = nSample + 4;
		} else {
			nPtr0 = nSample - 4;
			nPtr1 = nSample;
		}
		MinMax(&nPtr0, 0, m_nSamplesLine-1);
		MinMax(&nPtr1, 0, m_nSamplesLine-1);
		nPtr0 += (m_nSamplesLine * nLine);
		nPtr1 += (m_nSamplesLine * nLine);


		if(m_nLastPtr[0]<0) m_nLastPtr[0] = 0;
		if(m_nLastPtr[0]>nPtr0) m_nLastPtr[0] = nPtr0;


		m_pCpPos[nSample] = CpSurface;

		theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE, nFl);
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
		PROFILE->ModifySurfaceLine(CpSurface,MulDiv(nSample,(m_nSamplesLine-1)/10,(m_nSamplesLine-1)),nLine);

		gg=tt=pp=0;
		for(nn=0;nn<8;nn++) {
			if(m_TS[0].Gate.nCollectAmp[nn]>0) {
				for(nPtr=nPtr0;nPtr<=nPtr1;nPtr++) {
					m_cAmpBuffer[gg][nPtr] = theApp.m_UtUser.m_TS[nFl].Gate.nAmplitude[nn]+128;
				}
				if((m_nCollectAxis==5) && (m_nLastLine == theApp.m_Thread.m_nScanLine) && (theApp.m_Thread.m_bScanLineUpdateByMotorRevCounter == true)) {
					if(abs(nPtr0 - m_nLastPtr[0]) < 50) {
						for(nPtr=m_nLastPtr[0]+1;nPtr<nPtr0;nPtr++) {
							m_cAmpBuffer[gg][nPtr] = m_cAmpBuffer[gg][nPtr0];
						}
					}
				}
					
				gg++;
			}
			if(m_TS[0].Gate.nCollectTime[nn]>0) {
				nGStop = theApp.m_UtUser.m_TS[0].Gate.nTOFStopGate[nn];
				nGStart = theApp.m_UtUser.m_TS[0].Gate.nTOFStartGate[nn];
				if((theApp.m_UtUser.m_TS[nFl].Gate.nTimeStatus[nGStart-1]>0) && (theApp.m_UtUser.m_TS[nFl].Gate.nTimeStatus[nGStop]>0)) {
					nPath = (int)((float)(theApp.m_UtUser.m_TS[nFl].Gate.nTimeSample[nGStop] - theApp.m_UtUser.m_TS[nFl].Gate.nTimeSample[nGStart-1]) * theApp.m_UtUser.m_TS[0].Adc.fSamplePeriod);
				} else {
					switch(theApp.m_LastSettings.nDefaultTOF) {
					default: nPath = -1;
						break;
					case 1:	nPath = 0;
						break;
					case 2: nPath = theApp.m_UtUser.m_TS[nFl].Gate.nNsDelay[nGStart-1];
						break;
					case 3: nPath = theApp.m_UtUser.m_TS[nFl].Gate.nNsDelay[nGStart-1] + theApp.m_UtUser.m_TS[nFl].Gate.nNsWidth[nGStop];
						break;
					};
				};
				for(nPtr=nPtr0;nPtr<=nPtr1;nPtr++) {
					m_nTimeBuffer[tt][nPtr] = nPath;
				}
				tt++;
			}
				
			if((m_TS[0].Gate.nCollectPhase[nn]>0) && (nn==0)) {
				for(nPtr=nPtr0;nPtr<=nPtr1;nPtr++) {
					m_cPhaseBuffer[pp][nPtr0] = (char) theApp.m_UtUser.m_TS[nFl].Gate.nPhase[nn];
				}
				pp++;
			}

		}
		switch(m_TS[0].Adc.nBscanMode) {
		case 0:
			break;
		case 1:
			nVolumePtr=nSample * theApp.m_UtUser.m_TS[0].Adc.nBscanLength;
			nTrigPos=0;
			memcpy(&m_cVolumeBuffer[0][nFl][nVolumePtr],&nTrigPos,sizeof (int));
			memcpy(&m_cVolumeBuffer[0][nFl][nVolumePtr+4],&theApp.m_Scope.m_RFTrace[nFl][theApp.m_Scope.m_nAdcIndexFirstPoint[nFl]],(theApp.m_UtUser.m_TS[0].Adc.nBscanLength * m_nVolBytesPerSample) - 4);
			break;
		case 2:
			nVolumePtr=nSample * theApp.m_UtUser.m_TS[0].Adc.nBscanLength;
			nTrigPos=theApp.m_UtUser.m_TS[nFl].Dsp.InterfacePos;
			nStart=nTrigPos-(int)((float)theApp.m_UtUser.m_TS[0].Adc.nBscanPreTrigger/theApp.m_UtUser.m_TS[0].Adc.fSamplePeriod);
			nFinish=nTrigPos+(int)((float)theApp.m_UtUser.m_TS[0].Adc.nBscanPostTrigger/theApp.m_UtUser.m_TS[0].Adc.fSamplePeriod);
			if(nStart<0) nStart=0;
			if(nStart>=(int)theApp.m_UtUser.m_TS[0].Adc.nAcqLength) nStart=theApp.m_UtUser.m_TS[0].Adc.nAcqLength-1;
			if(nFinish<nStart) nFinish=nStart;
			if(nFinish>=(int)theApp.m_UtUser.m_TS[0].Adc.nAcqLength) nFinish=theApp.m_UtUser.m_TS[0].Adc.nAcqLength-1;
			memcpy(&m_cVolumeBuffer[0][nFl][nVolumePtr],&nStart,sizeof (int));
			memcpy(&m_cVolumeBuffer[0][nFl][nVolumePtr+4],&theApp.m_Scope.m_RFTrace[nFl][theApp.m_Scope.m_nAdcIndexFirstPoint[nFl]+nStart],(nFinish-nStart) * m_nVolBytesPerSample);
			break;
		}

	}
	m_nLastPtr[0] = nPtr0;
	m_nLastLine = theApp.m_Thread.m_nScanLine;

}


//This is the one used by the multiple probe flat scan routine
void CData::CollectComplexDataMultiProbeScan()
{
	CCoord CpHead,CpSurface;
	CPolyCoord* pLine = &PROFILE->m_ToolPath;
	int nn,gg,tt,pp,bb,nTS,nPtr0,nPtr1;
	int	nGStop,nGStart,nPath,nTrigPos;
	int	nStart,nFinish,nVolumePtr,nPtr,nSample,nLine,nProbe;

	m_bModified = TRUE;

	theApp.m_Motors.GetHeadPos(&CpHead);
	theApp.m_Kinematics.NormalsFromTips(&CpHead,PORTSIDE);
	theApp.m_Kinematics.NormalsFromTips(&CpHead,STARBOARD);
	theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
	theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);

	if(PROFILE->m_nLineIncrementMode == FIXED_INC) {
		nLine = (theApp.m_Thread.m_nScanLine-PROFILE->m_nScanStartLine)/PROFILE->m_nLineIncrement;
	} else {
		nLine = theApp.m_Thread.m_nScanLine-PROFILE->m_nScanStartLine;
	}

	nSample = (int)((CpSurface.fPos[m_nCollectAxis]-m_CpStart.fPos[m_nCollectAxis]) * (float)(m_nSamplesLine-1) / (m_CpFinish.fPos[m_nCollectAxis] - m_CpStart.fPos[m_nCollectAxis]));
	if(	m_vLineSample ) {
		if(	m_vLineSample[nLine].m_nCoordL>=2) {
			m_vLineSample[nLine].FindNearestSegment(CpSurface,PORTSIDE,&nSample);
			nSample += m_vLineSample[nLine].m_nFirstSample;
		}
	}

	if(nSample<0) nSample = 0;
	if(nSample>=m_nSamplesLine) nSample = m_nSamplesLine-1;

	if(theApp.m_Thread.m_bScanLineUpdateByMotorRevCounter==true) 
		theApp.m_Thread.m_nScanLine = theApp.m_Motors.GetRevCount();

	int	nSize = m_nSamplesLine * m_nNumberLines;


	m_pCpPos[nSample] = CpSurface;

	theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
	theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
	theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
	PROFILE->ModifySurfaceLine(CpSurface,MulDiv(nSample,(m_nSamplesLine-1)/10,(m_nSamplesLine-1)),nLine);

	for(nProbe=0; nProbe<m_nNumberProbesPerImage; nProbe++) {

		if(PROFILE->m_nLineIncrementMode == FIXED_INC) {
			nLine = (theApp.m_Thread.m_nScanLine-PROFILE->m_nScanStartLine)/PROFILE->m_nLineIncrement;
		} else {
			nLine = theApp.m_Thread.m_nScanLine-PROFILE->m_nScanStartLine;
		}

		nLine += ((int)(theApp.m_LastSettings.fProbeSeparation / PROFILE->m_fSlowIncrement) * nProbe);

		if(nLine<m_nNumberLines) {

			nPtr0 = (m_nSamplesLine * nLine) + nSample;
			nPtr0 = MinMax(nPtr0,0,nSize-1);
			m_nLastPtr[nProbe] = MinMax(m_nLastPtr[nProbe],0,nPtr0);

			if(theApp.m_Thread.m_nDir==0) {
				nPtr1 = nPtr0+1;
				if(nPtr1>=nSize) nPtr1=nSize-1;
			} else {
				nPtr1 = nPtr0-1;
				if(nPtr1<0) nPtr1=0;
			}


			gg=tt=pp=bb=0;
			for(nTS=0;nTS<m_nNumberTimeSlots;nTS++) {
				for(nn=0;nn<8;nn++) {
					if(m_TS[nProbe+PROFILE->m_nFirstProbe].Gate.nCollectAmp[nn]>0) {
						m_cAmpBuffer[gg][nPtr0] =m_cAmpBuffer[gg][nPtr1] = theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Gate.nAmplitude[nn]+128;
						//Automatically fill if in continuous mode because they will scan faster than the prf
						//Dont fill in segmented mode
						
						if((m_nCollectAxis==5) && (m_nLastLine == theApp.m_Thread.m_nScanLine) && (theApp.m_Thread.m_bScanLineUpdateByMotorRevCounter == true)) {
							if(abs(nPtr0 - m_nLastPtr[nProbe+PROFILE->m_nFirstProbe]) < 50) {
								for(nPtr=m_nLastPtr[nProbe+PROFILE->m_nFirstProbe]+1;nPtr<nPtr0;nPtr++) {
									m_cAmpBuffer[gg][nPtr] = m_cAmpBuffer[gg][nPtr0];
								}
							}
						}
						
						gg++;
					}
					if(m_TS[nProbe+PROFILE->m_nFirstProbe].Gate.nCollectTime[nn]>0) {
						nGStop = theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Gate.nTOFStopGate[nn];
						nGStart = theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Gate.nTOFStartGate[nn];
						nPath = (int)((float)(theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Gate.nTimeSample[nGStop] - theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Gate.nTimeSample[nGStart-1]) * theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Adc.fSamplePeriod);
						m_nTimeBuffer[tt][nPtr0] = m_nTimeBuffer[tt][nPtr1] = nPath;
						tt++;
					}
					
					if((m_TS[nProbe+PROFILE->m_nFirstProbe].Gate.nCollectPhase[nn]>0) && (nn==0)) {
						m_cPhaseBuffer[pp][nPtr0] = (char) theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Gate.nPhase[nn];
						pp++;
					}

				}
				switch(m_TS[nProbe+PROFILE->m_nFirstProbe].Adc.nBscanMode) {
				case 0:
					break;
				case 1:
					nVolumePtr=nSample * theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Adc.nBscanLength;
					nTrigPos=0;
					memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr],&nTrigPos,sizeof (int));
					memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr+4],&theApp.m_Scope.m_RFTrace[nProbe+PROFILE->m_nFirstProbe][theApp.m_Scope.m_nAdcIndexFirstPoint[nProbe+PROFILE->m_nFirstProbe]],(theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Adc.nBscanLength * m_nVolBytesPerSample) - 4);
					bb++;
					break;
				case 2:
					nVolumePtr=nSample * theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Adc.nBscanLength;
					nTrigPos=theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Dsp.InterfacePos;
					nStart=nTrigPos-(int)((float)theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Adc.nBscanPreTrigger/theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Adc.fSamplePeriod);
					nFinish=nTrigPos+(int)((float)theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Adc.nBscanPostTrigger/theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Adc.fSamplePeriod);
					if(nStart<0) nStart=0;
					if(nStart>=(int)theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Adc.nAcqLength) nStart=theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Adc.nAcqLength-1;
					if(nFinish<nStart) nFinish=nStart;
					if(nFinish>=(int)theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Adc.nAcqLength) nFinish=theApp.m_UtUser.m_TS[nProbe+PROFILE->m_nFirstProbe].Adc.nAcqLength-1;
					memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr],&nStart,sizeof (int));
					memcpy(&m_cVolumeBuffer[bb][0][nVolumePtr+4],&theApp.m_Scope.m_RFTrace[nProbe+PROFILE->m_nFirstProbe][theApp.m_Scope.m_nAdcIndexFirstPoint[nProbe+PROFILE->m_nFirstProbe]+nStart],(nFinish-nStart) * m_nVolBytesPerSample);
					bb++;
					break;
				}

			}
			m_nLastPtr[nProbe] = nPtr0;
		}
	}

	m_nLastLine = theApp.m_Thread.m_nScanLine;
}


bool CData::GetStringValue(int nImageType, int nImageNumber, int nPaletteNumber,int nTS, CPoint pt, CString *pBuff)
{
	int nPtr[2];
	int nArraySize = m_nSamplesLine * m_nNumberLines;
	double ddB;
	float fPath,fMult,fVelocity;
	bool bMaterialWater;
	CString Label;
	int nExt;
	double dRange[4] = {10.0, 5.0, 2.5, 1.25};
	double dExtValue;

	if(m_TS[nTS].Pr30.nMuxTx == m_TS[nTS].Pr30.nMuxRx) {
		fMult = 0.5e-6f;
	} else {
		fMult = 1e-6f;
	}


	pBuff->Empty();
	if(pt.x<0) return FALSE;
	if(pt.y<0) return FALSE;
	if(pt.x>=m_nSamplesLine) return FALSE;
	if(pt.y>=m_nNumberLines) return FALSE;

	nPtr[0]=pt.x + (pt.y*m_nSamplesLine);
	nPtr[1]=pt.x + ((pt.y+1)*m_nSamplesLine);

	//Average, Minimum
	int nSamples = 	(int)(theApp.m_LastSettings.fValueCursorSize/m_fFastIncrement);
	int nLines = (int)(theApp.m_LastSettings.fValueCursorSize/m_fSlowIncrement);
	double dValue;
	int nReadingCount =0;
	int nLine = 0;
	int nSample =0;

	if(nSamples<=2) nSamples = 2;
	if(nLines<=2) nLines = 2;

	switch(nImageType) {
	case 0: 
		/*nTemp=(m_cAmpBuffer[nImageNumber][nPtr[0]] + m_cAmpBuffer[nImageNumber][nPtr[1]]) / 2;
		nTemp=m_cAmpBuffer[nImageNumber][nPtr[0]];*/
		Label.LoadString(IDS_Amplitude);
	

		switch(theApp.m_LastSettings.nValueAlgorithm)
		{
		case 0: //min
			dValue = 99999999.9;
			break;
		case 1: //max
			dValue = -99999999.9;
			break;
		case 2: //ave
			dValue = 0;
			break;
		}

		
		for(nLine = pt.y-(nLines/2); nLine< pt.y+nLines/2; nLine++)
		{
			for(nSample = pt.x-(nSamples/2); nSample<pt.x+nSamples/2; nSample++)
			{
				if((nSample>0) && (nSample<m_nSamplesLine) && (nLine>0) && (nLine<m_nNumberLines))
				{
					switch(theApp.m_LastSettings.nValueAlgorithm)
					{
					case 0: //min
						if(dValue>m_cAmpBuffer[nImageNumber][nSample + (nLine*m_nSamplesLine)])
						{
							dValue = m_cAmpBuffer[nImageNumber][nSample + (nLine*m_nSamplesLine)];
						}
						break;
					case 1: //max
						if(dValue<m_cAmpBuffer[nImageNumber][nSample + (nLine*m_nSamplesLine)])
						{
							dValue = m_cAmpBuffer[nImageNumber][nSample + (nLine*m_nSamplesLine)];
						}
						break;
					case 2:
						dValue += m_cAmpBuffer[nImageNumber][nSample + (nLine*m_nSamplesLine)];
						nReadingCount++;
						break;
					}
				}
			}
		}

		if(theApp.m_LastSettings.nValueAlgorithm==2)
		{
			dValue /= nReadingCount;
		}


		switch(m_Hist.nAmplitudeScaleType) {
		case 0: pBuff->Format(_T("%s:%d%%"),Label,MulDiv((int)(dValue-128.0),100,127));
			break;
		case 1:
			ddB = -20.0 * log10((double)(dValue - 128) / 127.0);
			pBuff->Format(_T("%s:%.01fdB"),Label,ddB);
			break;
		case 2:
			pBuff->Format(_T("%s:%ddB"),Label,MulDiv((int)(255.0-dValue),m_nLogRange,-127));
			break;
		}
		break;
	case 1:

		switch(theApp.m_LastSettings.nValueAlgorithm)
		{
		case 0: //min
			fPath = 99999999.9f;
			break;
		case 1: //max
			fPath = -99999999.9f;
			break;
		case 2: //ave
			fPath = 0;
			break;
		}

		
		for(nLine = pt.y-(nLines/2); nLine< pt.y+nLines/2; nLine++)
		{
			for(nSample = pt.x-(nSamples/2); nSample<pt.x+nSamples/2; nSample++)
			{
				if((nSample>0) && (nSample<m_nSamplesLine) && (nLine>0) && (nLine<m_nNumberLines))
				{
					switch(theApp.m_LastSettings.nValueAlgorithm)
					{
					case 0: //min
						if((m_TS[nTS].Gate.nCollectTime[nImageNumber]>0) && (nImageNumber==0)) {
							fVelocity = (float)theApp.m_LastSettings.nWaterVelocity;
							bMaterialWater = TRUE;
							if(fPath > (float)m_nTimeBuffer[nImageNumber][nSample + (nLine*m_nSamplesLine)] * fVelocity * fMult)
							{
								fPath = (float)m_nTimeBuffer[nImageNumber][nSample + (nLine*m_nSamplesLine)] * fVelocity * fMult;
							}
						} else {
							bMaterialWater = FALSE;
							if(fPath > theApp.TimeToThickness(m_nTimeBuffer[nImageNumber][nSample + (nLine*m_nSamplesLine)])/2.0f )
							{
								fPath = theApp.TimeToThickness(m_nTimeBuffer[nImageNumber][nSample + (nLine*m_nSamplesLine)])/2.0f;
							}
						}
						break;
					case 1: //max
						if((m_TS[nTS].Gate.nCollectTime[nImageNumber]>0) && (nImageNumber==0)) {
							fVelocity = (float)theApp.m_LastSettings.nWaterVelocity;
							bMaterialWater = TRUE;
							if(fPath < (float)m_nTimeBuffer[nImageNumber][nSample + (nLine*m_nSamplesLine)] * fVelocity * fMult)
							{
								fPath = (float)m_nTimeBuffer[nImageNumber][nSample + (nLine*m_nSamplesLine)] * fVelocity * fMult;
							}
						} else {
							bMaterialWater = FALSE;
							if(fPath < theApp.TimeToThickness(m_nTimeBuffer[nImageNumber][nSample + (nLine*m_nSamplesLine)])/2.0f )
							{
								fPath = theApp.TimeToThickness(m_nTimeBuffer[nImageNumber][nSample + (nLine*m_nSamplesLine)])/2.0f;
							}
						}
						break;
					case 2:
						if((m_TS[nTS].Gate.nCollectTime[nImageNumber]>0) && (nImageNumber==0)) {
							fVelocity = (float)theApp.m_LastSettings.nWaterVelocity;
							bMaterialWater = TRUE;							
							fPath += (float)m_nTimeBuffer[nImageNumber][nSample + (nLine*m_nSamplesLine)] * fVelocity * fMult;
							
						} else {
							bMaterialWater = FALSE;
							fPath += theApp.TimeToThickness(m_nTimeBuffer[nImageNumber][nSample + (nLine*m_nSamplesLine)])/2.0f;
							
						}
						nReadingCount++;
						break;
					}
				}
			}
		}

		if(theApp.m_LastSettings.nValueAlgorithm==2)
		{
			fPath /= nReadingCount;
		}

	
		if(bMaterialWater == FALSE) {
			Label.LoadString(IDS_Thickness);
			if(theApp.m_LastSettings.nMeasurementUnits == 0) {
				pBuff->Format(_T("%s:%.02fmm"),Label,fPath);
			} else {
				pBuff->Format(_T("%s:%.03f\x022"),Label,fPath/25.4f);
			}
		} else {
			Label.LoadString(IDS_Water_Path);
			pBuff->Format(_T("%s:%.01fmm"),Label,fPath);
		}
		break;
	case EXT_IMAGE:
		nExt=(m_16ExtBuffer[nImageNumber][nPtr[0]] + m_16ExtBuffer[nImageNumber][nPtr[1]]) / 2;
		switch(theApp.m_LastSettings.nExtSourceDisplayMode) {
		case 0:	//Unsigned decimal
			pBuff->Format(L"Decimal = %d",nExt);
			break;
		case 1:	//Signed Decimal
			pBuff->Format(L"Signed = %d",nExt-32768);
			break;
		case 2: //Volts;
			dExtValue = (double)(nExt-32768) * dRange[ theApp.m_UtUser.m_Global.Ext.cGain[nImageNumber] & 3 ] / 32768.0;
			pBuff->Format(L"Voltage = %.02f",dExtValue);
			break;
		case 3: //%
			dExtValue = (double)(nExt-32768) * 100.0 / 32768.0;
			pBuff->Format(L"Amplitude = %.02f%%",dExtValue);
			break;
		}
		break;

	}

	return TRUE;
}

int CData::GetFilteredAmp(int nSample, int nLine, int nImageNumber,int nImageType)
{
	int nPtr,nTemp;
	int nArraySize = m_nSamplesLine * m_nNumberLines;

	switch(nImageType) {
	case AMP_IMAGE:
		switch(theApp.m_LastSettings.nDisplayWhichLines) {
		case 0:nPtr=nSample + (nLine*m_nSamplesLine);
			if((nPtr>=0) && (nPtr<nArraySize)) nTemp=m_cAmpBuffer[nImageNumber][nPtr];
			break;
		case 1:
			nPtr = nSample + ((nLine & 0xfffe)*m_nSamplesLine);
			if((nPtr>=0) && (nPtr<nArraySize)) nTemp=m_cAmpBuffer[nImageNumber][nPtr];
			break;
		case 2:
			nPtr = nSample + (((nLine & 0xfffe)+1)*m_nSamplesLine);
			if((nPtr>=0) && (nPtr<nArraySize)) nTemp=m_cAmpBuffer[nImageNumber][nPtr];
			break;
		case 3:nPtr=nSample + (nLine*m_nSamplesLine);
			if(nPtr<nArraySize) nTemp=m_cAmpBuffer[nImageNumber][nPtr];
			nPtr=nSample + ((nLine+1)*m_nSamplesLine);
			if((nPtr>=0) && (nPtr<nArraySize)) {
				nTemp+=m_cAmpBuffer[nImageNumber][nPtr];
				nTemp/=2;
			}
			break;
		}
		break;
	case EXT_IMAGE:
		switch(theApp.m_LastSettings.nDisplayWhichLines) {
		case 0:nPtr=nSample + (nLine*m_nSamplesLine);
			if((nPtr>=0) && (nPtr<nArraySize)) nTemp=m_16ExtBuffer[nImageNumber][nPtr];
			break;
		case 1:
			nPtr = nSample + ((nLine & 0xfffe)*m_nSamplesLine);
			if((nPtr>=0) && (nPtr<nArraySize)) nTemp=m_16ExtBuffer[nImageNumber][nPtr];
			break;
		case 2:
			nPtr = nSample + (((nLine & 0xfffe)+1)*m_nSamplesLine);
			if((nPtr>=0) && (nPtr<nArraySize)) nTemp=m_16ExtBuffer[nImageNumber][nPtr];
			break;
		case 3:nPtr=nSample + (nLine*m_nSamplesLine);
			if(nPtr<nArraySize) nTemp=m_16ExtBuffer[nImageNumber][nPtr];
			nPtr=nSample + ((nLine+1)*m_nSamplesLine);
			if((nPtr>=0) && (nPtr<nArraySize)) {
				nTemp+=m_16ExtBuffer[nImageNumber][nPtr];
				nTemp/=2;
			}
			break;
		}
		break;
	}

	return nTemp;

}

char CData::GetFilteredAmpChar(int nSample, int nLine, int nImageNumber)
{
	int nPtr;
	char cTemp;
	short sTemp;
	int nArraySize = m_nSamplesLine * m_nNumberLines;

	switch(theApp.m_LastSettings.nDisplayWhichLines) {
	case 0:nPtr=nSample + (nLine*m_nSamplesLine);
		if((nPtr>=0) && (nPtr<nArraySize)) cTemp=m_cAmpBuffer[nImageNumber][nPtr];
		break;
	case 1:
		nPtr = nSample + ((nLine & 0xfffe)*m_nSamplesLine);
		if((nPtr>=0) && (nPtr<nArraySize)) cTemp=m_cAmpBuffer[nImageNumber][nPtr];
		break;
	case 2:
		nPtr = nSample + (((nLine & 0xfffe)+1)*m_nSamplesLine);
		if((nPtr>=0) && (nPtr<nArraySize)) cTemp=m_cAmpBuffer[nImageNumber][nPtr];
		break;
	case 3:nPtr=nSample + (nLine*m_nSamplesLine);
		if(nPtr<nArraySize) sTemp=m_cAmpBuffer[nImageNumber][nPtr];
		nPtr=nSample + ((nLine+1)*m_nSamplesLine);
		if((nPtr>=0) && (nPtr<nArraySize)) {
			sTemp+=m_cAmpBuffer[nImageNumber][nPtr];
			sTemp/=2;
		}
		cTemp = (char)sTemp;
		break;
	}

	return cTemp;

}

void CData::GetAmplitudeCrossSection(int nImageNumber, int nLine, int *pAmplitudeArray)
{

	if(nLine<0) nLine = 0;
	if(nLine>=m_nNumberLines) nLine = m_nNumberLines-1;

	for(int nSample = 0;nSample<m_nSamplesLine;nSample++) {
		pAmplitudeArray[nSample] = MulDiv(GetFilteredAmp(nSample,nLine,nImageNumber)-128,100,127);
	}
}

void CData::GetThicknessCrossSection(int nImageNumber, int nLine, float *pThicknessArray)
{

	if(nLine<0) nLine = 0;
	if(nLine>=m_nNumberLines) nLine = m_nNumberLines-1;

	for(int nSample = 0;nSample<m_nSamplesLine;nSample++) {
		pThicknessArray[nSample] = (float)GetFilteredTimeThickness(nSample,nLine,nImageNumber,1) / 100.0f;
	}
}


int CData::GetFilteredTimeThickness(int nSample, int nLine, int nImageNumber,int nTimeThickness)
{
	int nPtr,nTemp;
	int nArraySize = m_nSamplesLine * m_nNumberLines;
	float fPath;

	switch(theApp.m_LastSettings.nDisplayWhichLines) {
	case 0:nPtr=nSample + (nLine*m_nSamplesLine);
		if(nPtr<nArraySize) nTemp=m_nTimeBuffer[nImageNumber][nPtr];
		break;
	case 1:
		nPtr = nSample + ((nLine & 0xfffe)*m_nSamplesLine);
		if(nPtr<nArraySize) nTemp=m_nTimeBuffer[nImageNumber][nPtr];
		break;
	case 2:
		nPtr = nSample + (((nLine & 0xfffe)+1)*m_nSamplesLine);
		if(nPtr<nArraySize) nTemp=m_nTimeBuffer[nImageNumber][nPtr];
		break;
	case 3:nPtr=nSample + (nLine*m_nSamplesLine);
		if(nPtr<nArraySize) nTemp=m_nTimeBuffer[nImageNumber][nPtr];
		nPtr=nSample + ((nLine+1)*m_nSamplesLine);
		if(nPtr<nArraySize) {
			nTemp+=m_nTimeBuffer[nImageNumber][nPtr];
			nTemp/=2;
		}
		break;
	}

	if(nTimeThickness==1) {
		//Return thickness to the nearent 0.01 mm
		//208 = 2.08mm
		fPath = theApp.TimeToThickness(nTemp) / 2.0f;
		if(fPath < 0.0f) fPath = 0.0f;
		nTemp = (int)(fPath * 100.0f);

		if(nTemp<0) nTemp=0;
		if(nTemp>=theApp.m_Palette.m_nMaxThicknessNumber[0]) nTemp=theApp.m_Palette.m_nMaxThicknessNumber[0]-2;
	}

	return nTemp;

}


int CData::AddAnnotation(float fXp,float fYp,CString Text,LOGFONT lf,COLORREF rgbTextColor,int nImageType,int nImageNumber,int nApplyToAllImages, int nBkMode, COLORREF rgbBkColor)
{
	m_nAnnotationL++;
	if(m_nAnnotationL==1) {
		m_pAnnotation = new ANNOTATION[m_nAnnotationL];
	} else {
		m_pAnnotation = (ANNOTATION *)realloc(m_pAnnotation, m_nAnnotationL * sizeof (ANNOTATION));
	};

	int	nn = m_nAnnotationL-1;
	m_pAnnotation[nn].nStructSize = sizeof (ANNOTATION);
	m_pAnnotation[nn].fXp = fXp;
	m_pAnnotation[nn].fYp = fYp;
	m_pAnnotation[nn].lf = lf;
	m_pAnnotation[nn].rgbTextColor = rgbTextColor;
	m_pAnnotation[nn].nImageType = nImageType;
	m_pAnnotation[nn].nImageNumber = nImageNumber;
	m_pAnnotation[nn].nApplyToAllImage = nApplyToAllImages;
	m_pAnnotation[nn].nBkMode = nBkMode;
	m_pAnnotation[nn].rgbBkColor = rgbBkColor;
	swprintf_s(m_pAnnotation[nn].Text,256,_T("%s"),LPCWSTR(Text));

	return nn;
}

void CData::DeleteAnnotation(int nIndex)
{
	int nn;

	if(nIndex<0) return;
	if(m_pAnnotation==NULL) return;
	if(m_nAnnotationL<0) return;

	for(nn=nIndex;nn<(m_nAnnotationL-1);nn++) {
		CopyMemory(&m_pAnnotation[nn],&m_pAnnotation[nn+1],sizeof (ANNOTATION));
	}
	m_nAnnotationL--;
	if(m_nAnnotationL>0) {
		m_pAnnotation = (ANNOTATION *)realloc(m_pAnnotation, m_nAnnotationL * sizeof (ANNOTATION));
	} else {
		delete m_pAnnotation;
		m_pAnnotation=NULL;
		m_nAnnotationL=0;
	}


}

bool CData::IsNameRequired()
{
	if(m_FileName.IsEmpty()) {

		for(int nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
			if(theApp.m_UtUser.m_TS[nTS].Adc.nBscanMode) return TRUE;
		}

		if(PROFILE->m_nCollectScanLines) {
			return TRUE;
		}
		if(theApp.m_LastSettings.nScanMask & _ENTER_FILENAME_AT_START) return true;
	}
	return FALSE;

}


void CData::RetrieveVolumeHeader(CUSLFile *pFile, int nImageNumber,int *pnTimeSlot)
{
	int nVariablesOffset,nStringsOffset,nGlobalOffset,nTimeSlotOffset,nDataOffset,nAnnotationOffset,nTimeSlot,nTemp,nn;
	UINT64 n64VariablesOffset,n64StringsOffset,n64GlobalOffset,n64TimeSlotOffset,n64DataOffset,n64AnnotationOffset,n64Temp;
	float fVersion;
/////////////////////////////////////////////////
//	UNICODE
	WCHAR wIdentifier[]=_T("Ultrasonic Sciences Ltd USLScanner Image Data");
	WCHAR wStr[100];
	ZeroMemory(wStr,sizeof wStr);

	pFile->Seek(0,CUSLFile::begin);
	pFile->Read(wStr,sizeof wIdentifier);

	pFile->Read(&fVersion,sizeof fVersion);

	pFile->Seek(0x100,CUSLFile::begin);
	if(fVersion<3.07f) {
		pFile->Read(&nVariablesOffset,sizeof nVariablesOffset);
		pFile->Read(&nStringsOffset,sizeof nStringsOffset);
		pFile->Read(&nGlobalOffset,sizeof nGlobalOffset);
		pFile->Read(&nTimeSlotOffset,sizeof nTimeSlotOffset);
		pFile->Read(&nDataOffset,sizeof nDataOffset);
		pFile->Read(&nAnnotationOffset,sizeof nAnnotationOffset);
		pFile->Read(m_n32VolumeOffset,sizeof m_n32VolumeOffset);
		pFile->Read(&nTemp,sizeof nTemp);
		pFile->Read(&nTemp,sizeof nTemp);
		for(nn=0;nn<8;nn++) {
			m_n64VolumeOffset[nn] = (UINT64)m_n32VolumeOffset[nn];
		}
	} else {
		pFile->Read(&n64VariablesOffset,sizeof n64VariablesOffset);
		pFile->Read(&n64StringsOffset,sizeof n64StringsOffset);
		pFile->Read(&n64GlobalOffset,sizeof n64GlobalOffset);
		pFile->Read(&n64TimeSlotOffset,sizeof n64TimeSlotOffset);
		pFile->Read(&n64DataOffset,sizeof n64DataOffset);
		pFile->Read(&n64AnnotationOffset,sizeof n64AnnotationOffset);
		pFile->Read(m_n64VolumeOffset,sizeof m_n64VolumeOffset);
		pFile->Read(&n64Temp,sizeof n64Temp);
		pFile->Read(&n64Temp,sizeof n64Temp);
	}

	pFile->Seek(m_n64VolumeOffset[nImageNumber],CUSLFile::begin);
	pFile->Read(&m_nAscanLength,sizeof m_nAscanLength);
	pFile->Read(&m_nRealAscanLength,sizeof m_nRealAscanLength);
	pFile->Read(&nTimeSlot,sizeof nTimeSlot);
	m_nVolBytesPerSample = 1;
	float fSamplePeriod;
	if(fVersion >= 4.60f) {
		pFile->Read(&fSamplePeriod, sizeof fSamplePeriod);
		pFile->Read(&m_nVolBytesPerSample,sizeof m_nVolBytesPerSample);
	}

	*pnTimeSlot = nTimeSlot;

}

void CData::SaveDuringScan()
{
	CUSLFile File;
	CFileException e;
	CString Buff,Language;
	

	if(m_FilePath.IsEmpty()) {
		m_FilePath = theApp.m_StorePath[0] + L"\\" + m_FileName + L".dat";
	}

	if(!File.Open(m_FilePath, CFile::modeReadWrite | CFile::typeBinary, &e)) {
		File.DecodeFileException(&e,&Buff,m_FilePath);
		Language.LoadString(IDS_Error);
		MessageBox(NULL,Buff,Language,MB_ICONERROR);
	} else {
		Save(&File, false);
		File.Close();
	}

}

void CData::SaveVolumeDuringScan(int nLine)
{
	CUSLFile File;
	CFileException e;
	CString Buff,Language;
	int nAscanLength,bb,nTS,nFL;
	UINT64 nOffset,n64Temp;
	char nOffsetArray[256];
	ZeroMemory(nOffsetArray,sizeof nOffsetArray);
	n64Temp=0;
	
	if(m_nNumberVolumeImages==0) return;

	if(m_FilePath.IsEmpty()) {
		m_FilePath = theApp.m_StorePath[0] + L"\\" + m_FileName + L".dat";
	}

	if(!File.Open(m_FilePath, CFile::modeReadWrite | CFile::typeBinary | CFile::modeCreate | CFile::modeNoTruncate, &e)) {
		File.DecodeFileException(&e,&Buff,m_FilePath);
		Language.LoadString(IDS_Error);
		MessageBox(NULL,Buff,Language,MB_ICONERROR);
	} else {
		Save(&File, true);

		for(nTS=0,bb=-1;nTS<m_nNumberTimeSlots;nTS++) {
			if(m_TS[nTS].Adc.nBscanMode) {
				if(m_TS[nTS].nScanLine==0) //bb=image number, increment to 0 on first pass 
					bb++;
			
				nAscanLength = m_TS[nTS].Adc.nBscanLength;
				File.Seek(m_n64VolumeOffset[0],CUSLFile::begin);
				File.Write(&nAscanLength,sizeof nAscanLength);
				File.Write(&m_TS[nTS].Adc.nAcqLength,sizeof m_TS[nTS].Adc.nAcqLength);
				File.Write(&nTS,sizeof nTS);
				File.Write(&m_TS[nTS].Adc.fSamplePeriod, sizeof m_TS[nTS].Adc.fSamplePeriod);
				File.Write(&m_nVolBytesPerSample, sizeof m_nVolBytesPerSample);

				for(nFL=0;nFL<m_nNumberProbesPerImage;nFL++) {
					nOffset = ((UINT64)nAscanLength * (UINT64)m_nSamplesLine * (UINT64)(nLine+nFL) * (UINT64)m_nVolBytesPerSample) + 0x100;
					File.Seek(nOffset + m_n64VolumeOffset[bb],CUSLFile::begin);
					File.Write(m_cVolumeBuffer[0][nFL] , nAscanLength * m_nSamplesLine * m_nVolBytesPerSample);
					ZeroMemory(m_cVolumeBuffer[0][nFL] , nAscanLength * m_nSamplesLine * m_nVolBytesPerSample);
				};

			}
		}
		File.Close();
		m_bVolScansSaved = true;
	}

}

int CData::VolumeGenerateAmpCscan(CUSLFile* pFile, CProgressCtrl *pProgress, GatesData *pGate, int nVolumeNumber, int nTS, int nEncoding, CString Name)
{
	if (m_nVolBytesPerSample == 1) {
		return VolumeGenerateAmpCscanByte(pFile, pProgress, pGate, nVolumeNumber, nTS, nEncoding, Name);
	}
	else {
		return VolumeGenerateAmpCscanWord(pFile, pProgress, pGate, nVolumeNumber, nTS, nEncoding, Name);
	}
}

int CData::VolumeGenerateTimeCscan(CUSLFile* pFile, CProgressCtrl *pProgress, GatesData *pGate, int nVolumeNumber, int nTS, int nEncoding)
{
	if (m_nVolBytesPerSample == 1) {
		return VolumeGenerateTimeCscanByte(pFile, pProgress, pGate, nVolumeNumber, nTS, nEncoding);
	}
	else {
		return VolumeGenerateTimeCscanWord(pFile, pProgress, pGate, nVolumeNumber, nTS, nEncoding);
	}
}

int CData::VolumeGenerateAmpCscanByte(CUSLFile* pFile, CProgressCtrl *pProgress, GatesData *pGate, int nVolumeNumber, int nTS, int nEncoding, CString Name)
{
	int	gg, nLine, nSample, nFirstGate;
	char *Buffer = new char[m_nSamplesLine * m_nAscanLength];
	char *pTrace;
	int *pnTrace;
	UINT64 nOffset;
	int nS, nF, nStart[3], nFinish[3], nColor, ii, nPtr, nInterfacePos, nIntPos, nStopPos;
	int nRfType = theApp.m_LastSettings.nVolumeRfType;
	CString Buff;

	int nGate = theApp.m_LastSettings.nVolumeCScanGate;

	for (gg = 0; gg<NUMBER_BUFFERS; gg++) {
		if (m_cAmpBuffer[gg] == NULL) {
			m_cAmpBuffer[gg] = new unsigned char[m_nSamplesLine * m_nNumberLines];
			m_nNumberAmpImages[0]++;
			m_nNumberImages++;
			break;
		}
	}

	nStart[0] = (int)(((float)pGate->nNsDelay[0] - (float)m_TS[nTS].Adc.nDelay) / m_TS[nTS].Adc.fSamplePeriod);
	nFinish[0] = nStart[0] + (int)((float)pGate->nNsWidth[0] / m_TS[nTS].Adc.fSamplePeriod);

	pProgress->SetRange(0, m_nNumberLines);
	for (nLine = 0, nPtr = 0; nLine<m_nNumberLines; nLine++) {
		pProgress->SetPos(nLine);
		nOffset = m_n64VolumeOffset[nVolumeNumber] + 0x100 + ((UINT64)m_nSamplesLine * (UINT64)m_nAscanLength * (UINT64)nLine);
		pFile->Seek(nOffset, CUSLFile::begin);
		pFile->Read(Buffer, m_nSamplesLine * m_nAscanLength);


		for (nSample = 0; nSample<m_nSamplesLine; nSample++) {
			nColor = 0;

			pTrace = &Buffer[nSample * m_nAscanLength];
			pnTrace = (int*)pTrace;
			pTrace += 4;

			nIntPos = pnTrace[0];
			nS = nStart[0] - nIntPos;
			nF = nFinish[0] - nIntPos;
			if (nS<0) nS = 0;
			if (nF<0) nF = 0;
			if (nS>(m_nAscanLength - 5)) nS = m_nAscanLength - 4;
			if (nF>(m_nAscanLength - 5)) nF = m_nAscanLength - 4;

			switch (pGate->nGateMode[nGate]) {
			case 0:
				break;
			case 1:
				for (ii = nS; ii<nF; ii++) {
					switch (nRfType) {
					case RFTYPELINEAR:
					case 1:
					case 4:
						if (pTrace[ii]>nColor) nColor = pTrace[ii];
						break;
					case 2:
						if ((~pTrace[ii])>nColor) nColor = ~pTrace[ii];
						break;
					case 3:
						if (abs(pTrace[ii])>nColor) nColor = abs(pTrace[ii]);
						break;
					}
				}
				break;
			case 2:
				nInterfacePos = nS;
				for (ii = nS; ii <= nF; ii++) {
					if (pGate->nThreshold[0] >= 0) {
						if (pTrace[ii] >= pGate->nThreshold[0]) {
							nInterfacePos = ii;
							break;
						}
					}
					else {
						if (pTrace[ii] <= pGate->nThreshold[0]) {
							nInterfacePos = ii;
							break;
						}
					}

				}
				nStart[1] = (int)((float)pGate->nNsDelay[nGate] / m_TS[nTS].Adc.fSamplePeriod) + nInterfacePos;
				nFinish[1] = nStart[1] + (int)((float)pGate->nNsWidth[nGate] / m_TS[nTS].Adc.fSamplePeriod);
				if (nFinish[1] >= (m_nAscanLength - 5)) nFinish[1] = m_nAscanLength - 5;
				for (ii = nStart[1]; ii<nFinish[1]; ii++) {
					switch (nRfType) {
					case RFTYPELINEAR:
					case 1:
					case 4:
						if (pTrace[ii]>nColor) nColor = pTrace[ii];
						break;
					case 2:
						if ((~pTrace[ii])>nColor) nColor = ~pTrace[ii];
						break;
					case 3:
						if (abs(pTrace[ii])>nColor) nColor = abs(pTrace[ii]);
						break;
					}
				}
				break;
			default:
				nInterfacePos = nS;			//Find the interface
				for (ii = nS; ii <= nF; ii++) {
					if (pGate->nThreshold[0] >= 0) {
						if (pTrace[ii] >= pGate->nThreshold[0]) {
							nInterfacePos = ii;
							break;
						}
					}
					else {
						if (pTrace[ii] <= pGate->nThreshold[0]) {
							nInterfacePos = ii;
							break;
						}
					}
				}
				nFirstGate = pGate->nGateMode[nGate] - 2;	//Find the next interface
				nStart[1] = (int)((float)pGate->nNsDelay[nFirstGate] / m_TS[nTS].Adc.fSamplePeriod) + nInterfacePos;
				nFinish[1] = nStart[1] + (int)((float)pGate->nNsWidth[nFirstGate] / m_TS[nTS].Adc.fSamplePeriod);
				nStopPos = nStart[1];
				for (ii = nStart[1]; ii <= nFinish[1]; ii++) {
					if (pTrace[ii] >= pGate->nThreshold[nFirstGate]) {
						nStopPos = ii;
						break;
					}
				}
				nStart[2] = (int)((float)pGate->nNsDelay[nGate] / m_TS[nTS].Adc.fSamplePeriod) + nInterfacePos;
				nFinish[2] = nStopPos - (int)((float)pGate->nNsFarDeadZone[nGate] / m_TS[nTS].Adc.fSamplePeriod);
				for (ii = nStart[2]; ii<nFinish[2]; ii++) {
					switch (nRfType) {
					case RFTYPELINEAR:
					case 1:
					case 4:
						if (pTrace[ii]>nColor) nColor = pTrace[ii];
						break;
					case 2:
						if ((~pTrace[ii])>nColor) nColor = ~pTrace[ii];
						break;
					case 3:
						if (abs(pTrace[ii])>nColor) nColor = abs(pTrace[ii]);
						break;
					}
				}
				break;
			};
			m_cAmpBuffer[gg][nPtr++] = nColor + 128;
		}


	}
	pProgress->SetPos(0);
	delete Buffer;

	if (Name != "") {
		m_ImageName[gg] = Name;
	}
	else {
		Buff.Format(_T("Amp %d"), gg + 1);
		m_ImageName[gg] = Buff;
	}


	m_nImageNameL = gg + 1;

	return gg;
}

int CData::VolumeGenerateAmpCscanWord(CUSLFile* pFile, CProgressCtrl *pProgress, GatesData *pGate, int nVolumeNumber, int nTS, int nEncoding, CString Name)
{
	int	gg, nLine, nSample, nFirstGate;
	__int16 *wBuffer = new __int16[m_nSamplesLine * m_nAscanLength];
	__int16 *pTrace;
	int *pnTrace;
	UINT64 nOffset;
	int nS, nF, nStart[3], nFinish[3], nColor, ii, nPtr, nInterfacePos, nIntPos, nStopPos;
	int nRfType = theApp.m_LastSettings.nVolumeRfType;
	CString Buff;

	int nGate = theApp.m_LastSettings.nVolumeCScanGate;
	__int16 nInterfaceThreshold = MulDiv(pGate->nThreshold[0], 0x7fff, 100);
	__int16 nGateThreshold = MulDiv(pGate->nThreshold[nGate], 0x7fff, 100);

	for (gg = 0; gg<NUMBER_BUFFERS; gg++) {
		if (m_cAmpBuffer[gg] == NULL) {
			m_cAmpBuffer[gg] = new unsigned char[m_nSamplesLine * m_nNumberLines];
			m_nNumberAmpImages[0]++;
			m_nNumberImages++;
			break;
		}
	}

	nStart[0] = (int)(((float)pGate->nNsDelay[0] - (float)m_TS[nTS].Adc.nDelay) / m_TS[nTS].Adc.fSamplePeriod);
	nFinish[0] = nStart[0] + (int)((float)pGate->nNsWidth[0] / m_TS[nTS].Adc.fSamplePeriod);

	pProgress->SetRange(0, m_nNumberLines);
	for (nLine = 0, nPtr = 0; nLine<m_nNumberLines; nLine++) {
		pProgress->SetPos(nLine);
		nOffset = m_n64VolumeOffset[nVolumeNumber] + 0x100 + ((UINT64)m_nSamplesLine * (UINT64)m_nAscanLength * (UINT64)nLine * (UINT64)m_nVolBytesPerSample);
		pFile->Seek(nOffset, CUSLFile::begin);
		pFile->Read(wBuffer, m_nSamplesLine * m_nAscanLength * m_nVolBytesPerSample);


		for (nSample = 0; nSample<m_nSamplesLine; nSample++) {
			nColor = 0;

			pTrace = &wBuffer[nSample * m_nAscanLength];
			pnTrace = (int*)pTrace;
			pTrace += 4;

			nIntPos = pnTrace[0];
			nS = nStart[0] - nIntPos;
			nF = nFinish[0] - nIntPos;
			if (nS<0) nS = 0;
			if (nF<0) nF = 0;
			if (nS>(m_nAscanLength - 5)) nS = m_nAscanLength - 4;
			if (nF>(m_nAscanLength - 5)) nF = m_nAscanLength - 4;

			switch (pGate->nGateMode[nGate]) {
			case 0:
				break;
			case 1:
				for (ii = nS; ii<nF; ii++) {
					switch (nRfType) {
					case RFTYPELINEAR:
					case 1:
					case 4:
						if (pTrace[ii]>nColor) nColor = pTrace[ii];
						break;
					case 2:
						if ((~pTrace[ii])>nColor) nColor = ~pTrace[ii];
						break;
					case 3:
						if (abs(pTrace[ii])>nColor) nColor = abs(pTrace[ii]);
						break;
					}
				}
				break;
			case 2:
				nInterfacePos = nS;
				for (ii = nS; ii <= nF; ii++) {
					if (nInterfaceThreshold >= 0) {
						if (pTrace[ii] >= nInterfaceThreshold) {
							nInterfacePos = ii;
							break;
						}
					}
					else {
						if (pTrace[ii] <= nInterfaceThreshold) {
							nInterfacePos = ii;
							break;
						}
					}

				}
				nStart[1] = (int)((float)pGate->nNsDelay[nGate] / m_TS[nTS].Adc.fSamplePeriod) + nInterfacePos;
				nFinish[1] = nStart[1] + (int)((float)pGate->nNsWidth[nGate] / m_TS[nTS].Adc.fSamplePeriod);
				if (nFinish[1] >= (m_nAscanLength - 5)) nFinish[1] = m_nAscanLength - 5;
				for (ii = nStart[1]; ii<nFinish[1]; ii++) {
					switch (nRfType) {
					case RFTYPELINEAR:
					case 1:
					case 4:
						if (pTrace[ii]>nColor) nColor = pTrace[ii];
						break;
					case 2:
						if ((~pTrace[ii])>nColor) nColor = ~pTrace[ii];
						break;
					case 3:
						if (abs(pTrace[ii])>nColor) nColor = abs(pTrace[ii]);
						break;
					}
				}
				break;
			default:
				nInterfacePos = nS;			//Find the interface
				for (ii = nS; ii <= nF; ii++) {
					if (nInterfaceThreshold >= 0) {
						if (pTrace[ii] >= nInterfaceThreshold) {
							nInterfacePos = ii;
							break;
						}
					}
					else {
						if (pTrace[ii] <= nInterfaceThreshold) {
							nInterfacePos = ii;
							break;
						}
					}
				}
				nFirstGate = pGate->nGateMode[nGate] - 2;	//Find the next interface
				nStart[1] = (int)((float)pGate->nNsDelay[nFirstGate] / m_TS[nTS].Adc.fSamplePeriod) + nInterfacePos;
				nFinish[1] = nStart[1] + (int)((float)pGate->nNsWidth[nFirstGate] / m_TS[nTS].Adc.fSamplePeriod);
				nStopPos = nStart[1];
				for (ii = nStart[1]; ii <= nFinish[1]; ii++) {
					if (pTrace[ii] >= nGateThreshold) {
						nStopPos = ii;
						break;
					}
				}
				nStart[2] = (int)((float)pGate->nNsDelay[nGate] / m_TS[nTS].Adc.fSamplePeriod) + nInterfacePos;
				nFinish[2] = nStopPos - (int)((float)pGate->nNsFarDeadZone[nGate] / m_TS[nTS].Adc.fSamplePeriod);
				for (ii = nStart[2]; ii<nFinish[2]; ii++) {
					switch (nRfType) {
					case RFTYPELINEAR:
					case 1:
					case 4:
						if (pTrace[ii]>nColor) nColor = pTrace[ii];
						break;
					case 2:
						if ((~pTrace[ii])>nColor) nColor = ~pTrace[ii];
						break;
					case 3:
						if (abs(pTrace[ii])>nColor) nColor = abs(pTrace[ii]);
						break;
					}
				}
				break;
			};
			m_cAmpBuffer[gg][nPtr++] = (nColor>>8) + 128;
		}
	}
	pProgress->SetPos(0);
	delete wBuffer;

	if (Name != "") {
		m_ImageName[gg] = Name;
	}
	else {
		Buff.Format(_T("Amp %d"), gg + 1);
		m_ImageName[gg] = Buff;
	}


	m_nImageNameL = gg + 1;

	return gg;
}


int CData::VolumeGenerateTimeCscanByte(CUSLFile* pFile, CProgressCtrl *pProgress, GatesData *pGate, int nVolumeNumber, int nTS, int nEncoding)
{
	int	nLine, nSample;
	char *Buffer = new char[m_nSamplesLine * m_nAscanLength];
	char *pTrace;
	int *pnTrace;
	UINT64 nOffset;
	int nStart, nFinish, ii, nPtr, nIntPos, gg, tt, nStartPos[2], nThreshold;
	char cMax[8];

	for (tt = 0; tt<NUMBER_BUFFERS; tt++) {
		if (m_nTimeBuffer[tt] == NULL) {
			m_nTimeBuffer[tt] = new unsigned int[m_nSamplesLine * m_nNumberLines];
			m_nNumberTimeImages[0]++;
			break;
		}
	}

	float fDelay = (float)m_TS[nTS].Adc.nDelay;

	pProgress->SetRange(0, m_nNumberLines);
	for (nLine = 0, nPtr = 0; nLine<m_nNumberLines; nLine++) {
		pProgress->SetPos(nLine);
		nOffset = m_n64VolumeOffset[nVolumeNumber] + 0x100 + ((UINT64)m_nSamplesLine * (UINT64)m_nAscanLength * (UINT64)nLine);
		pFile->Seek(nOffset, CFile::begin);
		pFile->Read(Buffer, m_nSamplesLine * m_nAscanLength);

		for (nSample = 0; nSample<m_nSamplesLine; nSample++) {

			pTrace = &Buffer[nSample * m_nAscanLength];
			pnTrace = (int*)pTrace;
			pTrace += 4;
			nIntPos = pnTrace[0];
			for (gg = 0; gg<2; gg++) {
				nStartPos[gg] = -1;
				cMax[gg] = 0;
				switch (pGate->nGateMode[gg]) {
				case 0:
					break;
				case 1:
					nStart = (int)(((float)pGate->nNsDelay[gg] - fDelay) / m_TS[nTS].Adc.fSamplePeriod) - nIntPos;
					nFinish = nStart + (int)((float)pGate->nNsWidth[gg] / m_TS[nTS].Adc.fSamplePeriod);
					break;
				case 2:
					nStart = (int)((float)pGate->nNsDelay[gg] / m_TS[nTS].Adc.fSamplePeriod) + nStartPos[gg - 1];
					nFinish = nStart + (int)((float)pGate->nNsWidth[gg] / m_TS[nTS].Adc.fSamplePeriod);
					break;
				};
				if (nStart<0) nStart = 0;
				if (nFinish<0) nFinish = 0;
				if (nStart>(m_nAscanLength - 5)) nStart = m_nAscanLength - 5;
				if (nFinish>(m_nAscanLength - 5)) nFinish = m_nAscanLength - 5;
				nThreshold = MulDiv(pGate->nThreshold[gg], 127, 100);

				switch (pGate->nTimsDetectionMode[gg]) {
				case 1:
				case 2:
					for (ii = nStart; ii <= nFinish; ii++) {
						switch (pGate->nTimsDetectionMode[gg]) {
						case 1:	if ((nStartPos[gg] == -1) && (pTrace[ii] >= nThreshold)) nStartPos[gg] = ii;
							break;
						case 2:
							if (pTrace[ii] >= cMax[gg]) {
								nStartPos[gg] = ii;
								cMax[gg] = pTrace[ii];
							}
							break;
						}
					}
					break;
				case 3:
				case 4:
					for (ii = nFinish; ii >= nStart; ii--) {
						switch (pGate->nTimsDetectionMode[gg]) {
						case 3:	if ((nStartPos[gg] == -1) && (pTrace[ii] >= nThreshold)) nStartPos[gg] = ii;
							break;
						case 4:
							if (pTrace[ii] >= cMax[gg]) {
								nStartPos[gg] = ii;
								cMax[gg] = pTrace[ii];
							}
							break;
						}
					}
					break;
				}

			}
			if ((nStartPos[0] >= 0) && (nStartPos[1] >= 0)) {
				m_nTimeBuffer[tt][nPtr++] = (int)((float)(nStartPos[1] - nStartPos[0])*m_TS[nTS].Adc.fSamplePeriod);
			}
			else {
				if ((nStartPos[0] >= 0) && (nStartPos[1] == -1)) {
					m_nTimeBuffer[tt][nPtr++] = 0;//0x07ffff;
				}
				else {
					m_nTimeBuffer[tt][nPtr++] = 0;
				}
			}
		}
	}
	pProgress->SetPos(0);
	delete Buffer;
	return tt;
}

int CData::VolumeGenerateTimeCscanWord(CUSLFile* pFile, CProgressCtrl *pProgress, GatesData *pGate, int nVolumeNumber, int nTS, int nEncoding)
{
	int	nLine, nSample;
	__int16 *wBuffer = new __int16[m_nSamplesLine * m_nAscanLength];
	__int16 *pTrace;
	int *pnTrace;
	UINT64 nOffset;
	int nStart, nFinish, ii, nPtr, nIntPos, tt, nStartPos[2];
	__int16 wMax[8],nThreshold;

	int nGate = theApp.m_LastSettings.nVolumeCScanGate;
	__int16 nInterfaceThreshold = MulDiv(pGate->nThreshold[0], 0x7fff, 100);
	__int16 nGateThreshold = MulDiv(pGate->nThreshold[nGate], 0x7fff, 100);

	for (tt = 0; tt<NUMBER_BUFFERS; tt++) {
		if (m_nTimeBuffer[tt] == NULL) {
			m_nTimeBuffer[tt] = new unsigned int[m_nSamplesLine * m_nNumberLines];
			m_nNumberTimeImages[0]++;
			break;
		}
	}

	float fDelay = (float)m_TS[nTS].Adc.nDelay;

	pProgress->SetRange(0, m_nNumberLines);
	for (nLine = 0, nPtr = 0; nLine<m_nNumberLines; nLine++) {
		pProgress->SetPos(nLine);
		nOffset = m_n64VolumeOffset[nVolumeNumber] + 0x100 + ((UINT64)m_nSamplesLine * (UINT64)m_nAscanLength * (UINT64)nLine * (UINT64)m_nVolBytesPerSample);
		pFile->Seek(nOffset, CFile::begin);
		pFile->Read(wBuffer, m_nSamplesLine * m_nAscanLength * m_nVolBytesPerSample);

		for (nSample = 0; nSample<m_nSamplesLine; nSample++) {

			pTrace = &wBuffer[nSample * m_nAscanLength];
			pnTrace = (int*)pTrace;
			pTrace += 4;
			nIntPos = pnTrace[0];
			for (int nn = 0; nn<2; nn++) {

				nn == 0 ? nGate = 0 : nGate = theApp.m_LastSettings.nVolumeCScanGate;
				nn == 0 ? nThreshold = nInterfaceThreshold : nThreshold = nGateThreshold;

				nStartPos[nn] = -1;
				wMax[nn] = 0;
				switch (pGate->nGateMode[nGate]) {
				case 0:
					break;
				case 1:
					nStart = (int)(((float)pGate->nNsDelay[nGate] - fDelay) / m_TS[nTS].Adc.fSamplePeriod) - nIntPos;
					nFinish = nStart + (int)((float)pGate->nNsWidth[nGate] / m_TS[nTS].Adc.fSamplePeriod);
					break;
				case 2:
					nStart = (int)((float)pGate->nNsDelay[nGate] / m_TS[nTS].Adc.fSamplePeriod) + nStartPos[nn - 1];
					nFinish = nStart + (int)((float)pGate->nNsWidth[nGate] / m_TS[nTS].Adc.fSamplePeriod);
					break;
				};
				if (nStart<0) nStart = 0;
				if (nFinish<0) nFinish = 0;
				if (nStart>(m_nAscanLength - 5)) nStart = m_nAscanLength - 5;
				if (nFinish>(m_nAscanLength - 5)) nFinish = m_nAscanLength - 5;

				switch (pGate->nTimsDetectionMode[nGate]) {
				case 1:
				case 2:
					for (ii = nStart; ii <= nFinish; ii++) {
						switch (pGate->nTimsDetectionMode[nGate]) {
						case 1:	if ((nStartPos[nn] == -1) && (pTrace[ii] >= nThreshold)) nStartPos[nn] = ii;
							break;
						case 2:
							if (pTrace[ii] >= wMax[nn]) {
								nStartPos[nn] = ii;
								wMax[nn] = pTrace[ii];
							}
							break;
						}
					}
					break;
				case 3:
				case 4:
					for (ii = nFinish; ii >= nStart; ii--) {
						switch (pGate->nTimsDetectionMode[nGate]) {
						case 3:	if ((nStartPos[nn] == -1) && (pTrace[ii] >= nThreshold)) nStartPos[nn] = ii;
							break;
						case 4:
							if (pTrace[ii] >= wMax[nn]) {
								nStartPos[nn] = ii;
								wMax[nn] = pTrace[ii];
							}
							break;
						}
					}
					break;
				}

			}
			if ((nStartPos[0] >= 0) && (nStartPos[1] >= 0)) {
				m_nTimeBuffer[tt][nPtr++] = (int)((float)(nStartPos[1] - nStartPos[0])*m_TS[nTS].Adc.fSamplePeriod);
			}
			else {
				if ((nStartPos[0] >= 0) && (nStartPos[1] == -1)) {
					m_nTimeBuffer[tt][nPtr++] = 0;//0x07ffff;
				}
				else {
					m_nTimeBuffer[tt][nPtr++] = 0;
				}
			}
		}
	}
	pProgress->SetPos(0);
	delete wBuffer;
	return tt;
}

void CData::VolumeGenerateHorizontalBscan(CUSLFile* pFile,CProgressCtrl *pProgress,int nVolumeNumber,int nTS,int nLine,GatesData* pGate)
{
	char *Buffer = new char[(m_nSamplesLine+2) * m_nAscanLength];
	__int16 *wBuffer = new __int16[(m_nSamplesLine+2) * m_nAscanLength];
	int *nBuffer;
	UINT64	nOffset = m_n64VolumeOffset[nVolumeNumber] + 0x100 + ((UINT64)m_nSamplesLine * (UINT64)m_nAscanLength * (UINT64)nLine * m_nVolBytesPerSample);
	int	nDest,nSrc,nIntPos,nn;
	int nThreshold = MulDiv(pGate->nThreshold[0],127,100);

	if(pFile->Seek(nOffset,CUSLFile::begin)) {
		if(m_nVolBytesPerSample == 1) {
			pFile->Read(Buffer,m_nSamplesLine * m_nAscanLength);
		} else {
			pFile->Read(wBuffer,m_nSamplesLine * m_nAscanLength * m_nVolBytesPerSample);
		}


		if(m_cBscanBuffer[0]==NULL) {
			m_cBscanBuffer[0] = new unsigned char[m_nSamplesLine * m_nRealAscanLength];
		}
		ZeroMemory(m_cBscanBuffer[0],m_nSamplesLine * m_nRealAscanLength);
		for(int nSample=0;nSample<m_nSamplesLine;nSample++) {
			if(m_nVolBytesPerSample == 1) {
				nBuffer = (int*)(&Buffer[nSample*m_nAscanLength]);
				nIntPos = *nBuffer;
				if(nIntPos<0) nIntPos=0;
				nSrc=(nSample * m_nAscanLength) + 4;
				nn=4;
				if(theApp.m_LastSettings.nLockSurface==1) {
					nIntPos=0;
					for(nn=4;nn<m_nAscanLength;nn++,nSrc++) {
						if(nThreshold>=0) {
							if(Buffer[nSrc]>nThreshold) break;
						} else {
							if(Buffer[nSrc]<nThreshold) break;
						}
					}
				}
				nDest=nIntPos + m_nRealAscanLength * nSample;
				CopyMemory((void*)(&m_cBscanBuffer[0][nDest]),(void*)(&Buffer[nSrc]),m_nAscanLength-nn);
			} else {
				nBuffer = (int*)(&wBuffer[nSample*m_nAscanLength]);
				nIntPos = *nBuffer;
				if(nIntPos<0) nIntPos=0;
				nSrc=(nSample * m_nAscanLength) + 4;
				nn=4;
				if(theApp.m_LastSettings.nLockSurface==1) {
					nIntPos=0;
					for(nn=4;nn<m_nAscanLength;nn++,nSrc++) {
						if(nThreshold>=0) {
							if(wBuffer[nSrc]>nThreshold) break;
						} else {
							if(wBuffer[nSrc]<nThreshold) break;
						}
					}
				}
				nDest=nIntPos + m_nRealAscanLength * nSample;
				for(int ii=0;ii<m_nAscanLength-nn;ii++) {
					m_cBscanBuffer[0][nDest + ii] = (char)(wBuffer[nSrc + ii] >> 8);
				}
			}
		}
	}
	delete wBuffer;
	delete Buffer;

}

void CData::VolumeGenerateVerticalBscan(CUSLFile* pFile,CProgressCtrl *pProgress,int nVolumeNumber,int nTS,int nSample,GatesData* pGate)
{
	char *Buffer = new char[m_nAscanLength*2];
	__int16 *wBuffer = new __int16[m_nAscanLength*2];
	int *nBuffer;
	UINT64	nOffset;
	int	nDest,nIntPos,nLine,nSrc,nn;
	int nThreshold = MulDiv(pGate->nThreshold[0],127,100);

	if(m_cBscanBuffer[1]==NULL) {
		m_cBscanBuffer[1] = new unsigned char[m_nNumberLines * m_nRealAscanLength];
	}
//	ZeroMemory(m_cBscanBuffer[1],m_nNumberLines * m_nRealAscanLength);
	for(nLine=0;nLine<m_nNumberLines;nLine++) {
		if(m_nVolBytesPerSample==1) {
			nOffset = m_n64VolumeOffset[nVolumeNumber] + 0x100 + ((UINT64)m_nSamplesLine * (UINT64)m_nAscanLength * (UINT64)nLine) + (UINT64)(nSample * m_nAscanLength);
			if(pFile->Seek(nOffset,CUSLFile::begin)) {
				pFile->Read(Buffer,m_nAscanLength);

				nBuffer = (int*)Buffer;
				nIntPos = *nBuffer;

				nSrc=4;
				if(nIntPos<0) nIntPos=0;
				if(theApp.m_LastSettings.nLockSurface==1) {
					nIntPos=0;
					for(nn=4;nn<m_nAscanLength;nn++,nSrc++) {
						if(nThreshold>=0) {
							if(Buffer[nSrc]>nThreshold) break;
						} else {
							if(Buffer[nSrc]<nThreshold) break;
						}
					}
				}

				nDest=nIntPos + m_nRealAscanLength * nLine;

				CopyMemory((void*)(&m_cBscanBuffer[1][nDest]),(void*)(&Buffer[nSrc]),m_nAscanLength-4);
			}
		} else {
			nOffset = m_n64VolumeOffset[nVolumeNumber] + 0x100 + ((UINT64)m_nSamplesLine * (UINT64)m_nAscanLength * (UINT64)nLine * 2) + (UINT64)(nSample * m_nAscanLength * 2);
			if(pFile->Seek(nOffset,CUSLFile::begin)) {
				pFile->Read(wBuffer,m_nAscanLength * 2);

				nBuffer = (int*)wBuffer;
				nIntPos = *nBuffer;

				nSrc=4;
				if(nIntPos<0) nIntPos=0;
				if(theApp.m_LastSettings.nLockSurface==1) {
					nIntPos=0;
					for(nn=4;nn<m_nAscanLength;nn++,nSrc++) {
						if(nThreshold>=0) {
							if(wBuffer[nSrc]>nThreshold) break;
						} else {
							if(wBuffer[nSrc]<nThreshold) break;
						}
					}
				}

				nDest=nIntPos + m_nRealAscanLength * nLine;
				for(int ii=0;ii<m_nAscanLength-4;ii++) {
					m_cBscanBuffer[1][nDest + ii] = (char)(wBuffer[nSrc + ii] >> 8);
				}
			}
		}
	}
	delete wBuffer;
	delete Buffer;

}


void CData::GetBscanTrace(int nWhich,int nSample,int nLine,char *pTrace)
{
	if(nLine<0) nLine = 0;
	if(nSample<0) nSample=0;

	switch(nWhich) {
	case 0: if(m_cBscanBuffer[0]==NULL) return;
		CopyMemory(pTrace,(void*)(&m_cBscanBuffer[0][nSample*m_nRealAscanLength]),m_nRealAscanLength);
		break;
	case 1:  if(m_cBscanBuffer[1]==NULL) return;
		CopyMemory(pTrace,(void*)(&m_cBscanBuffer[1][nLine*m_nRealAscanLength]),m_nRealAscanLength);
		break;
	}

}

void CData::RenderAmpImage(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer,char* cColor,int nLineDirection /*=HORIZONTAL*/)
{

	switch(nLineDirection) {
	case 0: RenderAmpImageHorizontal(pDC,rr,prrSamples,nImageNumber,pScreenBuffer,cColor);
		break;
	case 1: RenderAmpImageVertical(pDC,rr,prrSamples,nImageNumber,pScreenBuffer,cColor);
		break;
	}
}

void CData::RenderAmpImageHorizontal(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer,char* cColor)
{
	if (m_cAmpBuffer[nImageNumber] == NULL) return;
	char* pBuffer;
	int	yy,xx,nOffset,nVFilter,xxCount;
	int	xs0=prrSamples->left;
	int	xs1=prrSamples->right;
	int	ys0=prrSamples->top;
	int ys1=prrSamples->bottom;
	int	nLine,s0,s1,c[2],color,ss,ff;
	int src,source[2];
	int nBand,nProbe,nn;
	int* pnMultipleProbeOffset = new int[m_nNumberLines];
	ZeroMemory(pnMultipleProbeOffset,m_nNumberLines * sizeof(int));

	int nBytesPixel = pDC->GetDeviceCaps(BITSPIXEL) / 8;

	if(m_nMinViewLine<0) m_nMinViewLine=0;
	if(m_nMinViewLine>=m_nNumberLines) m_nMinViewLine=m_nNumberLines-1;
	if(m_nMaxViewLine<0) m_nMaxViewLine=0;
	if(m_nMaxViewLine>=m_nNumberLines) m_nMaxViewLine=m_nNumberLines-1;

	int nXs1=prrSamples->right;
	int nXs0=prrSamples->left;

	int nWidth = rr.Width()+2;
	int* m_nXpixelOdd = new int[rr.Width()+10];
	int* m_nXpixelEven = new int[rr.Width()+10];
	for(xx=0;xx<=(rr.Width()+1);xx++) {
		m_nXpixelOdd[xx]=RoundDownMulDiv(xx,(nXs1-nXs0),rr.Width())+nXs0;
		m_nXpixelEven[xx]=RoundDownMulDiv(xx,(nXs1-nXs0),rr.Width())+nXs0;
	};
	CPolyCoord lineEdge;
	CCoord Cp;
	for(int ii=0;ii<8;ii++) {
		Cp.Side0.fX = (float)((ii*nWidth)/7);
		Cp.Side0.fY = (float)theApp.m_LastSettings.nImageEdge[ii];
		lineEdge.Add(Cp);
	}
	int nEdge;
	for(xx=1;xx<nWidth;xx++) {
		lineEdge.CalculateCoord((float)xx,Cp);
		nEdge=(int)Cp.Side0.fY;
		m_nXpixelEven[xx] += nEdge;
		if(nWidth>m_nSamplesLine) {
			if((m_nXpixelEven[xx]<m_nXpixelEven[xx-1])) {
				m_nXpixelEven[xx]=m_nXpixelEven[xx-1];
			}
		}
	}
	int nLineBlockEdge = 0;
	if((m_nNumberProbesPerImage>1) && (m_nScanType != MULTI_PROBE_SCAN)) {
		nLineBlockEdge = 1;
		for(nLine=0;nLine<m_nNumberLines;) {
			for(nBand=0,nProbe=0;nBand<4;nBand++) {
				for(nn=0;(nn<32) && (nLine<m_nNumberLines) && (nProbe<m_nNumberProbesPerImage);nn++,nLine++,nProbe++) {
					pnMultipleProbeOffset[nLine] = RoundDownMulDiv(nn,theApp.m_LastSettings.nPAEdgeOffset[nBand+1],31);
					if((nLine / m_nNumberProbesPerImage) & 1) {
						pnMultipleProbeOffset[nLine] += theApp.m_LastSettings.nPAEdgeDir;
					}
				}
			}
		}
	}

	unsigned char* pData0 = m_cAmpBuffer[nImageNumber];
	int nArrayLength=m_nSamplesLine * m_nNumberLines;


	for(yy=0;yy<rr.Height();yy++) {
		if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
			nOffset=yy*rr.Width();
		} else {
			nOffset=(rr.Height()-1-yy)*rr.Width();
		};
		nLine=RoundDownMulDiv(yy,ys1-ys0,rr.Height())+ys0;
		nVFilter=1;
		switch(theApp.m_LastSettings.nDisplayWhichLines) {
		case 0:
			break;
		case 1:nLine = nLine & 0xfffe;
			break;
		case 2:nLine = (nLine & 0xfffe) + 1;
			if(nLine>=m_nNumberLines) nLine = m_nNumberLines-1;
			break;
		case 3: nVFilter=2;
			break;
		}
		if((nLine>=m_nMinViewLine) && (nLine<m_nMaxViewLine)) {
			source[0] = nLine * m_nSamplesLine;
			source[1] = (nLine + 1) * m_nSamplesLine;
			if(source[1]>nArrayLength) source[1]=source[0];

			pBuffer=&pScreenBuffer[nOffset * nBytesPixel];

			for(xxCount=0;xxCount<=rr.Width();xxCount++) {
				
				if((theApp.m_LastSettings.nImageOrientation==2) || (theApp.m_LastSettings.nImageOrientation==3)){
					xx=(rr.Width()-xxCount);
				}else{
					xx=xxCount;
				}

				if((nLine&1) && (nLineBlockEdge==0)){
					s0=m_nXpixelEven[xx];
					s1=m_nXpixelEven[xx+1];
				} else {
					s0=m_nXpixelOdd[xx];
					s1=m_nXpixelOdd[xx+1];
				}
				s0 += pnMultipleProbeOffset[nLine];
				s1 += pnMultipleProbeOffset[nLine];

				(theApp.m_LastSettings.nAmpPriority==1)?(c[0]=0):(c[0]=0xff);
				c[1]=c[0];
				for(ff=0;ff<nVFilter;ff++) {
					src=source[ff];
					if((s1-s0)>0) {
						for(ss=s0;ss<s1;ss++) {
							if((ss+src)<nArrayLength) {
								if(theApp.m_LastSettings.nAmpPriority==1) {
									if(pData0[ss+src]>c[ff]) c[ff]=pData0[ss+src];
								} else {
									if(pData0[ss+src]<c[ff]) c[ff]=pData0[ss+src];
								}
							} else {
								c[ff]=128;
							}
						};
					} else {
						if((s0+src)<nArrayLength) {
							c[ff]=pData0[s0+src];
						} else {
							c[ff]=128;
						};
					};
				}
				if(nVFilter==2) {
					color = ((c[0]+c[1])/2) & 0xff;
				} else {
					color = c[0]& 0xff;
				}

				color*=4;
				*pBuffer++=cColor[color];
				*pBuffer++=cColor[color+1];
				*pBuffer++=cColor[color+2];
				if(nBytesPixel>3) pBuffer++;
			};
		};
	};
	delete pnMultipleProbeOffset;
	delete m_nXpixelOdd;
	delete m_nXpixelEven;

}

void CData::RenderAmpImageVertical(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer,char* cColor)
{
	char* pBuffer;
	int	yy,xx,nOffset,nVFilter,yyCount;
	int	xs0=prrSamples->left;
	int	xs1=prrSamples->right;
	int	ys0=prrSamples->top;
	int ys1=prrSamples->bottom;
	int	nLine,s0,s1,c[2],color,ss,ff;
	int src,source[2];
	int nBand,nProbe,nn;
	int* pnMultipleProbeOffset = new int[m_nNumberLines];
	ZeroMemory(pnMultipleProbeOffset,m_nNumberLines * sizeof(int));

	int nBytesPixel = pDC->GetDeviceCaps(BITSPIXEL) / 8;

	if(m_nMinViewLine<0) m_nMinViewLine=0;
	if(m_nMinViewLine>=m_nNumberLines) m_nMinViewLine=m_nNumberLines-1;
	if(m_nMaxViewLine<0) m_nMaxViewLine=0;
	if(m_nMaxViewLine>=m_nNumberLines) m_nMaxViewLine=m_nNumberLines-1;

	int nXs1=prrSamples->right;
	int nXs0=prrSamples->left;

	int nHeight = rr.Height()+2;
	int* m_nYpixelOdd = new int[rr.Height()+10];
	int* m_nYpixelEven = new int[rr.Height()+10];
	for(yy=0;yy<=(rr.Height()+1);yy++) {
		m_nYpixelOdd[yy]=RoundDownMulDiv(yy,(nXs1-nXs0),rr.Height())+nXs0;
		m_nYpixelEven[yy]=RoundDownMulDiv(yy,(nXs1-nXs0),rr.Height())+nXs0;
	};
	CPolyCoord lineEdge;
	CCoord Cp;
	for(int ii=0;ii<8;ii++) {
		Cp.Side0.fX = (float)((ii*nHeight)/7);
		Cp.Side0.fY = (float)theApp.m_LastSettings.nImageEdge[ii];
		lineEdge.Add(Cp);
	}
	int nEdge;
	for(yy=1;yy<nHeight;yy++) {
		lineEdge.CalculateCoord((float)yy,Cp);
		nEdge=(int)Cp.Side0.fY;
		m_nYpixelEven[yy] += nEdge;
		if(nHeight>m_nSamplesLine) {
			if((m_nYpixelEven[yy]<m_nYpixelEven[yy-1])) {
				m_nYpixelEven[yy]=m_nYpixelEven[yy-1];
			}
		}
	}
	if(m_nNumberProbesPerImage>1) {
		for(nLine=0;nLine<m_nNumberLines;) {
			for(nBand=0,nProbe=0;nBand<4;nBand++) {
				for(nn=0;(nn<32) && (nLine<m_nNumberLines) && (nProbe<m_nNumberProbesPerImage);nn++,nLine++,nProbe++) {
					pnMultipleProbeOffset[nLine] = RoundDownMulDiv(nn,theApp.m_LastSettings.nPAEdgeOffset[nBand+1],31);
					if((nLine / m_nNumberProbesPerImage) & 1) {
						pnMultipleProbeOffset[nLine] += theApp.m_LastSettings.nPAEdgeDir;
					}
				}
			}
		}
	}

	unsigned char* pData0 = m_cAmpBuffer[nImageNumber];
	int nArrayLength=m_nSamplesLine * m_nNumberLines;

	for(xx=0;xx<rr.Width();xx++) {
		if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
			nOffset=yy*rr.Height();
		} else {
			nOffset=(rr.Width()-1-xx)*rr.Height();
		};
		nLine=RoundDownMulDiv(xx,ys1-ys0,rr.Width())+ys0;
		nVFilter=1;
		switch(theApp.m_LastSettings.nDisplayWhichLines) {
		case 0:
			break;
		case 1:nLine = nLine & 0xfffe;
			break;
		case 2:nLine = (nLine & 0xfffe) + 1;
			if(nLine>=m_nNumberLines) nLine = m_nNumberLines-1;
			break;
		case 3: nVFilter=2;
			break;
		}
		if((nLine>=m_nMinViewLine) && (nLine<m_nMaxViewLine)) {
			source[0] = nLine * m_nSamplesLine;
			source[1] = (nLine + 1) * m_nSamplesLine;
			if(source[1]>nArrayLength) source[1]=source[0];

			
			for(yyCount=0;yyCount<=rr.Height();yyCount++) {
				
				if((theApp.m_LastSettings.nImageOrientation==2) || (theApp.m_LastSettings.nImageOrientation==3)){
					yy=(rr.Height()-yyCount);
				}else{
					yy=yyCount;
				}
				nOffset = (rr.Width() * (rr.Height()-yy)) + xx;
				pBuffer=&pScreenBuffer[nOffset * nBytesPixel];

				if((nLine&1) && (m_nNumberProbesPerImage<=1)){
					s0=m_nYpixelEven[yy];
					s1=m_nYpixelEven[yy+1];
				} else {
					s0=m_nYpixelOdd[yy];
					s1=m_nYpixelOdd[yy+1];
				}
				s0 += pnMultipleProbeOffset[nLine];
				s1 += pnMultipleProbeOffset[nLine];

				(theApp.m_LastSettings.nAmpPriority==1)?(c[0]=0):(c[0]=0xff);
				c[1]=c[0];
				for(ff=0;ff<nVFilter;ff++) {
					src=source[ff];
					if((s1-s0)>0) {
						for(ss=s0;ss<s1;ss++) {
							if((ss+src)<nArrayLength) {
								if(theApp.m_LastSettings.nAmpPriority==1) {
									if(pData0[ss+src]>c[ff]) c[ff]=pData0[ss+src];
								} else {
									if(pData0[ss+src]<c[ff]) c[ff]=pData0[ss+src];
								}
							} else {
								c[ff]=128;
							}
						};
					} else {
						if((s0+src)<nArrayLength) {
							c[ff]=pData0[s0+src];
						} else {
							c[ff]=128;
						};
					};
				}
				if(nVFilter==2) {
					color = ((c[0]+c[1])/2) & 0xff;
				} else {
					color = c[0]& 0xff;
				}
				color*=4;
				*pBuffer++=cColor[color];
				*pBuffer++=cColor[color+1];
				*pBuffer++=cColor[color+2];
				if(nBytesPixel>3) pBuffer++;
			};
		};
	};
	delete pnMultipleProbeOffset;
	delete m_nYpixelOdd;
	delete m_nYpixelEven;

}

void CData::RenderAmpImagePolar(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer,char* cColor)
{
	int m_nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nBytesPixel = m_nBitsPixel/8;
	int nArrayWidth = rr.Width();
	int	nVFilter;
	int x,y;
	int nMaxRings;

	int nRadius;
	if(rr.Height() < rr.Width()) {
		nRadius = rr.Height() / 2;
	} else {
		nRadius = rr.Width() / 2;
	}
	nRadius-=2;
	CPoint ptCentre = rr.CenterPoint();
	ptCentre.Offset(-rr.left,-rr.top);

	nMaxRings = nRadius;
	if(m_fComponentDiameter[1] > m_fComponentDiameter[0] && m_fComponentDiameter[1] > 0.0f) {
		nMaxRings = (int)((float)nRadius * (1.0f - (m_fComponentDiameter[0] / m_fComponentDiameter[1])));
	} else {
		if(m_fComponentDiameter[0] > m_fComponentDiameter[1] && m_fComponentDiameter[0] > 0.0f) {
			nMaxRings = (int)((float)nRadius * (1.0f - (m_fComponentDiameter[1] / m_fComponentDiameter[0])));
		}
	}


	for(int nRing=0;nRing<nMaxRings;nRing++) {
		x = 0;
		y = nRadius-nRing;
//		y= nRing;
		int p = (5 - y*4)/4;

 		int nMax=1;
		while (x < y) {
			x++;
			if (p < 0) {
				p += 2*x+1;
			} else {
				y--;
				p += 2*(x-y)+1;
			}
 			nMax++;
		}

		int nn=0;
		int nLine;
		x = 0;
		y = nRadius-nRing;
//		y=nRing;
		p = (5 - y*4)/4;
		nLine = RoundDownMulDiv(nRing,m_nNumberLines-1,nMaxRings);
		if(m_fComponentDiameter[0] <= m_fComponentDiameter[1]) {
			nLine = (m_nNumberLines-1) - nLine;
		}
		nVFilter=1;
		switch(theApp.m_LastSettings.nDisplayWhichLines) {
		case 0:
			break;
		case 1:nLine = nLine & 0xfffe;
			break;
		case 2:nLine = (nLine & 0xfffe) + 1;
			if(nLine>=m_nNumberLines) nLine = m_nNumberLines-1;
			break;
		case 3: nVFilter=2;
			break;
		}
		CirclePoints(pScreenBuffer, ptCentre, x, y, nn,nMax,nLine,nImageNumber,nBytesPixel,cColor,nArrayWidth);
		while (x < y) {
			x++;
			if (p < 0) {
				p += 2*x+1;
			} else {
				y--;
				p += 2*(x-y)+1;
			}
			CirclePoints(pScreenBuffer, ptCentre,  x, y, nn,nMax,nLine,nImageNumber,nBytesPixel,cColor,nArrayWidth);
			nn++;
		}
	}


}


void CData::RenderPhaseImage(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer)
{
	int	yy,xx,nOffset,xxCount;
	int	xs0=prrSamples->left;
	int	xs1=prrSamples->right;
	int	ys0=prrSamples->top;
	int ys1=prrSamples->bottom;
	int	nLine,s0,s1,ss,nVFilter,ff;
	char c[2];
	int src,source[2];
	char* pBuffer;
	int nBand,nProbe,nn;
	int* pnMultipleProbeOffset = new int[m_nNumberLines];
	ZeroMemory(pnMultipleProbeOffset,m_nNumberLines * sizeof(int));

	int m_nMinViewLine=0;
	int m_nMaxViewLine=m_nNumberLines-1;

	int nXs1=prrSamples->right;
	int nXs0=prrSamples->left;

	int nWidth = rr.Width()+2;
	int* m_nXpixelOdd = new int[rr.Width()+10];
	int* m_nXpixelEven = new int[rr.Width()+10];
	for(xx=0;xx<=(rr.Width()+1);xx++) {
		m_nXpixelOdd[xx]=MulDiv(xx,(nXs1-nXs0),rr.Width())+nXs0;
		m_nXpixelEven[xx]=MulDiv(xx,(nXs1-nXs0),rr.Width())+nXs0;
	};
	CPolyCoord lineEdge;
	CCoord Cp;
	for(int ii=0;ii<8;ii++) {
		Cp.Side0.fX = (float)((ii*nWidth)/7);
		Cp.Side0.fY = (float)theApp.m_LastSettings.nImageEdge[ii];
		lineEdge.Add(Cp);
	}
	int nEdge;
	for(xx=1;xx<nWidth;xx++) {
		lineEdge.CalculateCoord((float)xx,Cp);
		nEdge=(int)Cp.Side0.fY;
		m_nXpixelEven[xx] += nEdge;
		if(nWidth>m_nSamplesLine) {
			if((m_nXpixelEven[xx]<m_nXpixelEven[xx-1])) {
				m_nXpixelEven[xx]=m_nXpixelEven[xx-1];
			}
		}
	}
	if(m_nNumberProbesPerImage>1) {
		for(nLine=0;nLine<m_nNumberLines;) {
			for(nBand=0,nProbe=0;nBand<4;nBand++) {
				for(nn=0;(nn<32) && (nLine<m_nNumberLines) && (nProbe<m_nNumberProbesPerImage);nn++,nLine++,nProbe++) {
					pnMultipleProbeOffset[nLine] = MulDiv(nn,theApp.m_LastSettings.nPAEdgeOffset[nBand+1],31);
					if((nLine / m_nNumberProbesPerImage) & 1) {
						pnMultipleProbeOffset[nLine] += theApp.m_LastSettings.nPAEdgeDir;
					}
				}
			}
		}
	}

	int nBytesPixel = pDC->GetDeviceCaps(BITSPIXEL) / 8;

	unsigned char* pData0 = m_cPhaseBuffer[0];
	int nArrayLength=m_nSamplesLine * m_nNumberLines;

	for(yy=0;yy<rr.Height();yy++) {
		if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
			nOffset=yy*rr.Width();
		} else {
			nOffset=(rr.Height()-1-yy)*rr.Width();
		};
		nLine=MulDiv(yy,ys1-ys0,rr.Height())+ys0;
		nVFilter=1;
		switch(theApp.m_LastSettings.nDisplayWhichLines) {
		case 0:
			break;
		case 1:nLine = nLine & 0xfffe;
			break;
		case 2:nLine = (nLine & 0xfffe) + 1;
			if(nLine>=m_nNumberLines) nLine = m_nNumberLines-1;
			break;
		case 3: nVFilter=2;
			break;
		}
		if((nLine>=m_nMinViewLine) && (nLine<m_nMaxViewLine)) {
			source[0] = nLine * m_nSamplesLine;
			source[1] = (nLine + 1) * m_nSamplesLine;
			if(source[1]>nArrayLength) source[1]=source[0];

			pBuffer=&pScreenBuffer[nOffset * nBytesPixel];
			for(xxCount=0;xxCount<=rr.Width();xxCount++) {
				
				if((theApp.m_LastSettings.nImageOrientation==2) || (theApp.m_LastSettings.nImageOrientation==3)){
					xx=(rr.Width()-xxCount);
				}else{
					xx=xxCount;
				}

				if((nLine&1) && (m_nNumberProbesPerImage<=1)) {
					s0=m_nXpixelEven[xx];
					s1=m_nXpixelEven[xx+1];
				} else {
					s0=m_nXpixelOdd[xx];
					s1=m_nXpixelOdd[xx+1];
				}
				s0 += pnMultipleProbeOffset[nLine];
				s1 += pnMultipleProbeOffset[nLine];

				for(ff=0;ff<nVFilter;ff++) {
					src=source[ff];
					if((s1-s0)>0) {
						for(ss=s0;ss<s1;ss++) {
							if((ss+src)<nArrayLength) {
								c[ff]=pData0[ss+src];
							} else {
								c[ff]=-1;
							}
						};
					} else {
						if((s0+src)<nArrayLength) {
							c[ff]=pData0[s0+src];
						} else {
							c[ff]=-1;
						};
					};
				}


				if(c[0]==0){
					*pBuffer++=char((theApp.m_Scope.m_rgbPosPhaseColor>>16)&0xff);
					*pBuffer++=char((theApp.m_Scope.m_rgbPosPhaseColor>>8)&0xff);
					*pBuffer++=char((theApp.m_Scope.m_rgbPosPhaseColor>>0)&0xff);
				}else if(c[0]==1){
					*pBuffer++=char((theApp.m_Scope.m_rgbNegPhaseColor>>16)&0xff);
					*pBuffer++=char((theApp.m_Scope.m_rgbNegPhaseColor>>8)&0xff);
					*pBuffer++=char((theApp.m_Scope.m_rgbNegPhaseColor>>0)&0xff);
				}else{
					*pBuffer++=0;
					*pBuffer++=0;
					*pBuffer++=0;
				}


				if(nBytesPixel>3) pBuffer++;
			};
		};
	};
	delete pnMultipleProbeOffset;
	delete m_nXpixelOdd;
	delete m_nXpixelEven;

}


void CData::CirclePoints(char *pScreenBuffer,CPoint pt, int x, int y, int nn,int nMax,int nLine,int nImageNumber,int nBytesPixel,char* cColor,int nArrayWidth)
{
	unsigned char* pData = m_cAmpBuffer[nImageNumber];
	int nStartLine = nLine * m_nSamplesLine;
	int	nSamplesLine = m_nSamplesLine-1;
	int	ptr,nSample;
	int nColor;

        if (x == 0) {
			ptr=nStartLine + MulDiv(nSamplesLine,1,4);
			theApp.m_LastSettings.nDisplayWhichLines==3 ? nColor = (pData[ptr] + pData[ptr + nSamplesLine]) / 2 : nColor = pData[ptr];
			SetPixel(pScreenBuffer,pt.x,pt.y-y,&cColor[nColor*4],nBytesPixel,nArrayWidth);

			ptr=nStartLine + MulDiv(nSamplesLine,3,4);
			theApp.m_LastSettings.nDisplayWhichLines==3 ? nColor = (pData[ptr] + pData[ptr + nSamplesLine]) / 2 : nColor = pData[ptr];
			SetPixel(pScreenBuffer,pt.x,pt.y+y,&cColor[nColor*4],nBytesPixel,nArrayWidth);

			ptr=nStartLine + MulDiv(nSamplesLine,2,4);
			theApp.m_LastSettings.nDisplayWhichLines==3 ? nColor = (pData[ptr] + pData[ptr + nSamplesLine]) / 2 : nColor = pData[ptr];
			SetPixel(pScreenBuffer,pt.x+y,pt.y,&cColor[nColor*4],nBytesPixel,nArrayWidth);

			ptr=nStartLine + MulDiv(nSamplesLine,0,4);
			theApp.m_LastSettings.nDisplayWhichLines==3 ? nColor = (pData[ptr] + pData[ptr + nSamplesLine]) / 2 : nColor = pData[ptr];
			SetPixel(pScreenBuffer,pt.x-y,pt.y,&cColor[nColor*4],nBytesPixel,nArrayWidth);
        } else 
        if (x == y) {
			ptr=nStartLine + MulDiv(nSamplesLine,5,8);
			theApp.m_LastSettings.nDisplayWhichLines==3 ? nColor = (pData[ptr] + pData[ptr + nSamplesLine]) / 2 : nColor = pData[ptr];
			SetPixel(pScreenBuffer,pt.x+x,pt.y+y,&cColor[nColor*4],nBytesPixel,nArrayWidth);
			SetPixel(pScreenBuffer,pt.x+x-1,pt.y+y-1,&cColor[nColor*4],nBytesPixel,nArrayWidth);

			ptr=nStartLine + MulDiv(nSamplesLine,7,8);
			theApp.m_LastSettings.nDisplayWhichLines==3 ? nColor = (pData[ptr] + pData[ptr + nSamplesLine]) / 2 : nColor = pData[ptr];
			SetPixel(pScreenBuffer,pt.x-x,pt.y+y,&cColor[nColor*4],nBytesPixel,nArrayWidth);
			SetPixel(pScreenBuffer,pt.x-x+1,pt.y+y-1,&cColor[nColor*4],nBytesPixel,nArrayWidth);

			ptr=nStartLine + MulDiv(nSamplesLine,3,8);
			theApp.m_LastSettings.nDisplayWhichLines==3 ? nColor = (pData[ptr] + pData[ptr + nSamplesLine]) / 2 : nColor = pData[ptr];
			SetPixel(pScreenBuffer,pt.x+x,pt.y-y,&cColor[nColor*4],nBytesPixel,nArrayWidth);
			SetPixel(pScreenBuffer,pt.x+x-1,pt.y-y+1,&cColor[nColor*4],nBytesPixel,nArrayWidth);

			ptr=nStartLine + MulDiv(nSamplesLine,1,8);
			theApp.m_LastSettings.nDisplayWhichLines==3 ? nColor = (pData[ptr] + pData[ptr + nSamplesLine]) / 2 : nColor = pData[ptr];
			SetPixel(pScreenBuffer,pt.x-x,pt.y-y,&cColor[nColor*4],nBytesPixel,nArrayWidth);
			SetPixel(pScreenBuffer,pt.x-x+1,pt.y-y+1,&cColor[nColor*4],nBytesPixel,nArrayWidth);

        } else 
        if (x < y) {
			nSample = MulDiv(nn,nSamplesLine/8+4,nMax); //if there is a mismatch at 45 degrees change the 4
			//quadrant6
			ptr=nStartLine + MulDiv(nSamplesLine,3,4) - nSample;
			theApp.m_LastSettings.nDisplayWhichLines==3 ? nColor = (pData[ptr] + pData[ptr + nSamplesLine]) / 2 : nColor = pData[ptr];
			SetPixel(pScreenBuffer,pt.x+x,pt.y+y,&cColor[nColor*4],nBytesPixel,nArrayWidth);
			SetPixel(pScreenBuffer,pt.x+x-1,pt.y+y-1,&cColor[nColor*4],nBytesPixel,nArrayWidth);

			//quadrant7
			ptr=nStartLine + MulDiv(nSamplesLine,3,4) + nSample;
			theApp.m_LastSettings.nDisplayWhichLines==3 ? nColor = (pData[ptr] + pData[ptr + nSamplesLine]) / 2 : nColor = pData[ptr];
			SetPixel(pScreenBuffer,pt.x-x,pt.y+y,&cColor[nColor*4],nBytesPixel,nArrayWidth);
			SetPixel(pScreenBuffer,pt.x-x+1,pt.y+y-1,&cColor[nColor*4],nBytesPixel,nArrayWidth);

			//quadrant3
			ptr=nStartLine + MulDiv(nSamplesLine,1,4) + nSample;
			theApp.m_LastSettings.nDisplayWhichLines==3 ? nColor = (pData[ptr] + pData[ptr + nSamplesLine]) / 2 : nColor = pData[ptr];
			SetPixel(pScreenBuffer,pt.x+x,pt.y-y,&cColor[nColor*4],nBytesPixel,nArrayWidth);
			SetPixel(pScreenBuffer,pt.x+x-1,pt.y-y+1,&cColor[nColor*4],nBytesPixel,nArrayWidth);

			//quadrant2
			ptr=nStartLine + MulDiv(nSamplesLine,1,4) - nSample;
			theApp.m_LastSettings.nDisplayWhichLines==3 ? nColor = (pData[ptr] + pData[ptr + nSamplesLine]) / 2 : nColor = pData[ptr];
			SetPixel(pScreenBuffer,pt.x-x,pt.y-y,&cColor[nColor*4],nBytesPixel,nArrayWidth);
			SetPixel(pScreenBuffer,pt.x-x+1,pt.y-y+1,&cColor[nColor*4],nBytesPixel,nArrayWidth);

			//quadrant5
			ptr=nStartLine + MulDiv(nSamplesLine,1,2) + nSample;
			theApp.m_LastSettings.nDisplayWhichLines==3 ? nColor = (pData[ptr] + pData[ptr + nSamplesLine]) / 2 : nColor = pData[ptr];
			SetPixel(pScreenBuffer,pt.x+y,pt.y+x,&cColor[nColor*4],nBytesPixel,nArrayWidth);
			SetPixel(pScreenBuffer,pt.x+y-1,pt.y+x-1,&cColor[nColor*4],nBytesPixel,nArrayWidth);

			//quadrant8
			ptr=nStartLine + nSamplesLine - nSample;
			theApp.m_LastSettings.nDisplayWhichLines==3 ? nColor = (pData[ptr] + pData[ptr + nSamplesLine]) / 2 : nColor = pData[ptr];
			SetPixel(pScreenBuffer,pt.x-y,pt.y+x,&cColor[nColor*4],nBytesPixel,nArrayWidth);
			SetPixel(pScreenBuffer,pt.x-y+1,pt.y+x-1,&cColor[nColor*4],nBytesPixel,nArrayWidth);

			//quadrant4
			ptr=nStartLine + MulDiv(nSamplesLine,1,2) - nSample;
			theApp.m_LastSettings.nDisplayWhichLines==3 ? nColor = (pData[ptr] + pData[ptr + nSamplesLine]) / 2 : nColor = pData[ptr];
			SetPixel(pScreenBuffer,pt.x+y,pt.y-x,&cColor[nColor*4],nBytesPixel,nArrayWidth);
			SetPixel(pScreenBuffer,pt.x+y-1,pt.y-x+1,&cColor[nColor*4],nBytesPixel,nArrayWidth);

			//quadrant1
			ptr=nStartLine  + nSample;
			theApp.m_LastSettings.nDisplayWhichLines==3 ? nColor = (pData[ptr] + pData[ptr + nSamplesLine]) / 2 : nColor = pData[ptr];
			SetPixel(pScreenBuffer,pt.x-y,pt.y-x,&cColor[nColor*4],nBytesPixel,nArrayWidth);
			SetPixel(pScreenBuffer,pt.x-y+1,pt.y-x+1,&cColor[nColor*4],nBytesPixel,nArrayWidth);

        }
}

void CData::SetPixel(char *pScreenBuffer,int nX,int nY,char* cColor,int nBytesPixel,int nArrayWidth)
{

	int nOffset = (nY * nArrayWidth) + nX;
	char* pBuffer=&pScreenBuffer[nOffset * nBytesPixel];

	*pBuffer++=cColor[0];
	*pBuffer++=cColor[1];
	*pBuffer++=cColor[2];

}

void CData::RenderTimeImage(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer)
{
	int	yy,xx,nOffset,xxCount;
	int	xs0=prrSamples->left;
	int	xs1=prrSamples->right;
	int	ys0=prrSamples->top;
	int ys1=prrSamples->bottom;
	int	nLine,s0,s1,c[2],nColor,ss,nVFilter,ff;
	int src,source[2];
	char* pBuffer;
	int nBand,nProbe,nn;
	int* pnMultipleProbeOffset = new int[m_nNumberLines];
	ZeroMemory(pnMultipleProbeOffset,m_nNumberLines * sizeof(int));

	int m_nMinViewLine=0;
	int m_nMaxViewLine=m_nNumberLines-1;

	int nXs1=prrSamples->right;
	int nXs0=prrSamples->left;

	int nWidth = rr.Width()+2;
	int* m_nXpixelOdd = new int[rr.Width()+10];
	int* m_nXpixelEven = new int[rr.Width()+10];
	for(xx=0;xx<=(rr.Width()+1);xx++) {
		m_nXpixelOdd[xx]=RoundDownMulDiv(xx,(nXs1-nXs0),rr.Width())+nXs0;
		m_nXpixelEven[xx]=RoundDownMulDiv(xx,(nXs1-nXs0),rr.Width())+nXs0;
	};
	CPolyCoord lineEdge;
	CCoord Cp;
	for(int ii=0;ii<8;ii++) {
		Cp.Side0.fX = (float)((ii*nWidth)/7);
		Cp.Side0.fY = (float)theApp.m_LastSettings.nImageEdge[ii];
		lineEdge.Add(Cp);
	}
	int nEdge;
	for(xx=1;xx<nWidth;xx++) {
		lineEdge.CalculateCoord((float)xx,Cp);
		nEdge=(int)Cp.Side0.fY;
		m_nXpixelEven[xx] += nEdge;
		if(nWidth>m_nSamplesLine) {
			if((m_nXpixelEven[xx]<m_nXpixelEven[xx-1])) {
				m_nXpixelEven[xx]=m_nXpixelEven[xx-1];
			}
		}
	}
	int nLineBlockEdge = 0;
	if((m_nNumberProbesPerImage>1) && (m_nScanType != MULTI_PROBE_SCAN)) {
		nLineBlockEdge = 1;
		for(nLine=0;nLine<m_nNumberLines;) {
			for(nBand=0,nProbe=0;nBand<4;nBand++) {
				for(nn=0;(nn<32) && (nLine<m_nNumberLines) && (nProbe<m_nNumberProbesPerImage);nn++,nLine++,nProbe++) {
					pnMultipleProbeOffset[nLine] = RoundDownMulDiv(nn,theApp.m_LastSettings.nPAEdgeOffset[nBand+1],31);
					if((nLine / m_nNumberProbesPerImage) & 1) {
						pnMultipleProbeOffset[nLine] += theApp.m_LastSettings.nPAEdgeDir;
					}
				}
			}
		}
	}

	int nBytesPixel = pDC->GetDeviceCaps(BITSPIXEL) / 8;

	unsigned int* pData0 = m_nTimeBuffer[nImageNumber];
	int nArrayLength=m_nSamplesLine * m_nNumberLines;

	for(yy=0;yy<rr.Height();yy++) {
		if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
			nOffset=yy*rr.Width();
		} else {
			nOffset=(rr.Height()-1-yy)*rr.Width();
		};
		nLine=RoundDownMulDiv(yy,ys1-ys0,rr.Height())+ys0;
		nVFilter=1;
		switch(theApp.m_LastSettings.nDisplayWhichLines) {
		case 0:
			break;
		case 1:nLine = nLine & 0xfffe;
			break;
		case 2:nLine = (nLine & 0xfffe) + 1;
			if(nLine>=m_nNumberLines) nLine = m_nNumberLines-1;
			break;
		case 3: nVFilter=2;
			break;
		}
		if((nLine>=m_nMinViewLine) && (nLine<m_nMaxViewLine)) {
			source[0] = nLine * m_nSamplesLine;
			source[1] = (nLine + 1) * m_nSamplesLine;
			if(source[1]>nArrayLength) source[1]=source[0];

			pBuffer=&pScreenBuffer[nOffset * nBytesPixel];
			for(xxCount=0;xxCount<=rr.Width();xxCount++) {
				
				if((theApp.m_LastSettings.nImageOrientation==2) || (theApp.m_LastSettings.nImageOrientation==3)){
					xx=(rr.Width()-xxCount);
				}else{
					xx=xxCount;
				}

				if((nLine&1) && (nLineBlockEdge==0)){
					s0=m_nXpixelEven[xx];
					s1=m_nXpixelEven[xx+1];
				} else {
					s0=m_nXpixelOdd[xx];
					s1=m_nXpixelOdd[xx+1];
				}
				s0 += pnMultipleProbeOffset[nLine];
				s1 += pnMultipleProbeOffset[nLine];

				(theApp.m_LastSettings.nTimePriority==1)?(c[0]=0):(c[0]=1000000);
				c[1]=c[0];
				for(ff=0;ff<nVFilter;ff++) {
					src=source[ff];
					if((s1-s0)>0) {
						for(ss=s0;ss<s1;ss++) {
							if((ss+src)<nArrayLength) {
								if(theApp.m_LastSettings.nTimePriority==1) {
									if(pData0[ss+src]>(unsigned int)c[ff]) c[ff]=pData0[ss+src];
								} else {
									if(pData0[ss+src]<(unsigned int)c[ff]) c[ff]=pData0[ss+src];
								}
							} else {
								c[ff]=0;
							}
						};
					} else {
						if((s0+src)<nArrayLength) {
							c[ff]=pData0[s0+src];
						} else {
							c[ff]=0;
						};
					};
				}
				if(nVFilter==2) {
					nColor = (c[0]+c[1])/2;
				} else {
					nColor = c[0];
				}
				nColor=(nColor*theApp.m_LastSettings.nMaterialVelocity)/20000;
				if(nColor<0) nColor=1;
				if(nColor>=theApp.m_Palette.m_nMaxThicknessNumber[0]) nColor=theApp.m_Palette.m_nMaxThicknessNumber[0]-2;

				*pBuffer++=char((theApp.m_Palette.m_rgbThickness[0][nColor]>>16)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbThickness[0][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbThickness[0][nColor]>>0)&0xff);
				if(nBytesPixel>3) pBuffer++;
			};
		};
	};
	delete pnMultipleProbeOffset;
	delete m_nXpixelOdd;
	delete m_nXpixelEven;

}

void CData::RenderExtImage(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer,char* cColor)
{
	int	yy,xx,nOffset,xxCount;
	int	xs0=prrSamples->left;
	int	xs1=prrSamples->right;
	int	ys0=prrSamples->top;
	int ys1=prrSamples->bottom;
	int	nLine,s0,s1,nColor,ss,nVFilter,ff;
	unsigned __int16 c[2];
	int src,source[2];
	char* pBuffer;
	int nBand,nProbe,nn;
	int* pnMultipleProbeOffset = new int[m_nNumberLines];
	ZeroMemory(pnMultipleProbeOffset,m_nNumberLines * sizeof(int));

	int m_nMinViewLine=0;
	int m_nMaxViewLine=m_nNumberLines-1;

	int nXs1=prrSamples->right;
	int nXs0=prrSamples->left;

	int nWidth = rr.Width()+2;
	int* m_nXpixelOdd = new int[rr.Width()+10];
	int* m_nXpixelEven = new int[rr.Width()+10];
	for(xx=0;xx<=(rr.Width()+1);xx++) {
		m_nXpixelOdd[xx]=MulDiv(xx,(nXs1-nXs0),rr.Width())+nXs0;
		m_nXpixelEven[xx]=MulDiv(xx,(nXs1-nXs0),rr.Width())+nXs0;
	};
	CPolyCoord lineEdge;
	CCoord Cp;
	for(int ii=0;ii<8;ii++) {
		Cp.Side0.fX = (float)((ii*nWidth)/7);
		Cp.Side0.fY = (float)theApp.m_LastSettings.nImageEdge[ii];
		lineEdge.Add(Cp);
	}
	int nEdge;
	for(xx=1;xx<nWidth;xx++) {
		lineEdge.CalculateCoord((float)xx,Cp);
		nEdge=(int)Cp.Side0.fY;
		m_nXpixelEven[xx] += nEdge;
		if(nWidth>m_nSamplesLine) {
			if((m_nXpixelEven[xx]<m_nXpixelEven[xx-1])) {
				m_nXpixelEven[xx]=m_nXpixelEven[xx-1];
			}
		}
	}
	if(m_nNumberProbesPerImage>1) {
		for(nLine=0;nLine<m_nNumberLines;) {
			for(nBand=0,nProbe=0;nBand<4;nBand++) {
				for(nn=0;(nn<32) && (nLine<m_nNumberLines) && (nProbe<m_nNumberProbesPerImage);nn++,nLine++,nProbe++) {
					pnMultipleProbeOffset[nLine] = MulDiv(nn,theApp.m_LastSettings.nPAEdgeOffset[nBand+1],31);
					if((nLine / m_nNumberProbesPerImage) & 1) {
						pnMultipleProbeOffset[nLine] += theApp.m_LastSettings.nPAEdgeDir;
					}
				}
			}
		}
	}

	int nBytesPixel = pDC->GetDeviceCaps(BITSPIXEL) / 8;

	unsigned __int16* pData0 = m_16ExtBuffer[nImageNumber];
	int nArrayLength=m_nSamplesLine * m_nNumberLines;

	for(yy=0;yy<rr.Height();yy++) {
		if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
			nOffset=yy*rr.Width();
		} else {
			nOffset=(rr.Height()-1-yy)*rr.Width();
		};
		nLine=MulDiv(yy,ys1-ys0,rr.Height())+ys0;
		nVFilter=1;
		switch(theApp.m_LastSettings.nDisplayWhichLines) {
		case 0:
			break;
		case 1:nLine = nLine & 0xfffe;
			break;
		case 2:nLine = (nLine & 0xfffe) + 1;
			if(nLine>=m_nNumberLines) nLine = m_nNumberLines-1;
			break;
		case 3: nVFilter=2;
			break;
		}
		if((nLine>=m_nMinViewLine) && (nLine<m_nMaxViewLine)) {
			source[0] = nLine * m_nSamplesLine;
			source[1] = (nLine + 1) * m_nSamplesLine;
			if(source[1]>nArrayLength) source[1]=source[0];

			pBuffer=&pScreenBuffer[nOffset * nBytesPixel];
			for(xxCount=0;xxCount<=rr.Width();xxCount++) {
				
				if((theApp.m_LastSettings.nImageOrientation==2) || (theApp.m_LastSettings.nImageOrientation==3)){
					xx=(rr.Width()-xxCount);
				}else{
					xx=xxCount;
				}

				if((nLine&1) && (m_nNumberProbesPerImage<=1)) {
					s0=m_nXpixelEven[xx];
					s1=m_nXpixelEven[xx+1];
				} else {
					s0=m_nXpixelOdd[xx];
					s1=m_nXpixelOdd[xx+1];
				}
				s0 += pnMultipleProbeOffset[nLine];
				s1 += pnMultipleProbeOffset[nLine];

				(theApp.m_LastSettings.nAmpPriority==1)?(c[0]=0):(c[0]=0x8000);
				c[1]=c[0];
				for(ff=0;ff<nVFilter;ff++) {
					src=source[ff];
					if((s1-s0)>0) {
						for(ss=s0;ss<s1;ss++) {
							if((ss+src)<nArrayLength) {
								if(theApp.m_LastSettings.nAmpPriority==1) {
									if(pData0[ss+src]>c[ff]) c[ff]=pData0[ss+src];
								} else {
									if(pData0[ss+src]<c[ff]) c[ff]=pData0[ss+src];
								}
							} else {
								c[ff]=0;
							}
						};
					} else {
						if((s0+src)<nArrayLength) {
							c[ff]=pData0[s0+src];
						} else {
							c[ff]=0;
						};
					};
				}
				if(nVFilter==2) {
					nColor = (c[0]+c[1])/2;
				} else {
					nColor = c[0];
				}

				*pBuffer++=char((theApp.m_Palette.m_rgb16[0][nColor]>>16)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgb16[0][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgb16[0][nColor]>>0)&0xff);
				if(nBytesPixel>3) pBuffer++;
			};
		};
	};
	delete pnMultipleProbeOffset;
	delete m_nXpixelOdd;
	delete m_nXpixelEven;

}

void CData::AppendDataBase(bool bWhich)
{
	CUSLFile File;
	CFileException e;
	CString Buff,Language,FilePath;
	int nn;


	if(bWhich == true) {
		FilePath = theApp.m_DataBasePathName;
	} else {
		FilePath = theApp.m_DataBaseFailedToSavePathName;
	}

	if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite|CFile::modeNoTruncate, &e)) {
		File.DecodeFileException(&e,&Buff,FilePath);
		Language.LoadString(IDS_Error);
		MessageBox(NULL,Buff,Language,MB_ICONERROR);
	} else {
		File.SeekToEnd();
		CArchive* pArchFile;
		pArchFile = new CArchive(&File,CArchive::store);

		if(File.GetLength()<=0) {
			Buff = L"FileName";				pArchFile->WriteString(Buff);
			Buff = L",FilePath";				pArchFile->WriteString(Buff);
			Buff = L",ProfileName";			pArchFile->WriteString(Buff);
			Buff = L",Date Time";			pArchFile->WriteString(Buff);

			for(nn=0;nn<m_nDetailsL;nn++) {
				pArchFile->WriteString(_T(",L") + m_Caption[nn]);
			}
			pArchFile->WriteString(_T("\r\n"));
		}
		
		Buff = m_FileName + L",L";		pArchFile->WriteString(Buff);
		Buff = m_FilePath + L",L";		pArchFile->WriteString(Buff);
		Buff = m_ProfileName + L",L";		pArchFile->WriteString(Buff);
		Buff = m_TestDateTime;			pArchFile->WriteString(Buff);


		for(nn=0;nn<m_nDetailsL;nn++) {
			pArchFile->WriteString(_T(",L") + m_Details[nn]);
		}
		pArchFile->WriteString(_T("\r\n"));


		pArchFile->Close();
		delete pArchFile;

		File.Close();
	}

}


bool CData::RetrieveComplexGeometryData(CUSLFile* pFile)
{
	char Identifier[50];
	char	str[100];
	memset(str,0,sizeof str);
	double	dVersion;
	int	nGeneralOffset,nNumberSamplesOffset,nBufferOffset;
	int	nGlobalOffset,nTimeSlotOffset;
	int	nTestPlateOffset,nScanOffset;

	int	nItemSize,nNumberAmpImages,nNumberTimeImages,nOffset,gg,nImageType,ii,nn,c0,nColor,nTS;
	int* pnNumberSamples;
	unsigned char *pBuffer,*pcPtr;
	int	*pnBuffer;
	float *pfBuffer;
	float fX0,fX1,fX;
	strcpy_s(Identifier,"Ultrasonic Sciences Complex Geometry Data");
	TimeSlotDataCG	TS[8];
	UtGlobalDataCG	Global;


	Zero();

	pFile->Seek(0,CUSLFile::begin);

	m_FileName=pFile->GetFileName();
	m_FilePath=pFile->GetFilePath();

	pFile->Seek(0,CUSLFile::begin);
	pFile->Read(str,strlen(Identifier));
	if(strcmp(str,Identifier)) {
		MessageBox(NULL,_T("Incorrect Data File type"),_T("Read Error"),MB_ICONERROR);
		pFile->Close();
		return FALSE;
	}
	pFile->Read(&dVersion,sizeof dVersion);

	if(dVersion>=1.02) {
		pFile->Seek(0x300,CUSLFile::begin);
		pFile->Read(&nScanOffset,sizeof nScanOffset);
		pFile->Read(&nTestPlateOffset,sizeof nTestPlateOffset);
	} else {
		nScanOffset = 0x400;
	}

	pFile->Seek(nScanOffset,CUSLFile::begin);
	pFile->Read(&nGeneralOffset,sizeof nGeneralOffset);
	pFile->Read(&nNumberSamplesOffset,sizeof nNumberSamplesOffset);
	pFile->Read(&nBufferOffset,sizeof nBufferOffset);
	pFile->Read(&nGlobalOffset,sizeof nGlobalOffset);
	pFile->Read(&nTimeSlotOffset,sizeof nTimeSlotOffset);

	pFile->Seek(nGeneralOffset,CUSLFile::begin);
	pFile->Read(&m_nSamplesLine,sizeof m_nSamplesLine);
	pFile->Read(&m_nNumberLines,sizeof m_nNumberLines);
	pFile->Read(&nNumberAmpImages,sizeof nNumberAmpImages);
	pFile->Read(&nNumberTimeImages,sizeof nNumberTimeImages);
	pFile->Read(&nItemSize,sizeof nItemSize);
	pFile->Read(&m_fFastScanSize,sizeof m_fFastScanSize);
	pFile->Read(&m_fSlowScanSize,sizeof m_fSlowScanSize);
	pFile->Read(&m_fFastIncrement,sizeof m_fFastIncrement);
	pFile->Read(&m_fSlowIncrement,sizeof m_fSlowIncrement);
	pFile->Read(&m_osTestDateTime,sizeof m_osTestDateTime);
//	pFile->ReadString(&m_TestDateTime);
//	pFile->ReadString(&m_ProfileName);
//	pFile->ReadString(&m_Operator);

	m_nNumberAmpImages[0]=nNumberAmpImages;
	m_nNumberTimeImages[0]=nNumberTimeImages;
	m_nNumberTimeSlots = 2;
	m_nNumberImages = nNumberAmpImages + nNumberTimeImages;

	AllocateImageBuffers();

	pFile->Seek(nNumberSamplesOffset,CUSLFile::begin);
	pnNumberSamples = new int[m_nNumberLines];
	pFile->Read(pnNumberSamples,m_nNumberLines * sizeof (int));

	fX0=10000.0;
	fX1=-10000.0;
	pBuffer = new unsigned char [m_nNumberLines * nItemSize * m_nSamplesLine];
	pFile->Seek(nBufferOffset,CUSLFile::begin);
	for(int nLine=0;nLine<m_nNumberLines;nLine++) {
		pFile->Read(&pBuffer[m_nSamplesLine*nLine*nItemSize],nItemSize * pnNumberSamples[nLine] );
		pcPtr = &pBuffer[m_nSamplesLine*nLine*nItemSize];
		pfBuffer = (float*)pcPtr;
		if((fX0 > *pfBuffer) && (*pfBuffer>-1000.0)) fX0 = *pfBuffer;
		pcPtr += ((nItemSize * pnNumberSamples[nLine]) - (nItemSize *2));
		pfBuffer = (float*)pcPtr;
		if(fX1 < *pfBuffer) fX1 = *pfBuffer;
	}
	for(gg=0;gg<m_nNumberImages;gg++) {
		if(gg<m_nNumberAmpImages[0]) {
			nImageType=0;
		} else {
			nImageType=1;
		}
		for(int nLine=0;nLine<m_nNumberLines;nLine++) {
			for(ii=0;ii<pnNumberSamples[nLine];ii++) {
				pcPtr = (unsigned char *)(pBuffer + (ii * nItemSize) + (nLine * nItemSize * m_nSamplesLine));
				if(ii==0) {
					pfBuffer = (float *)pcPtr;
					fX = pfBuffer[0];
					nOffset = (int)(((fX-fX0)*(float)m_nSamplesLine)/(fX1-fX0));
				}
				nn=ii+nOffset;
				if(nn>=m_nSamplesLine) nn=m_nSamplesLine-1;
				if(nn<0) nn=0;
				switch(nImageType) {
				case 0: //Amplitude
					nColor = pcPtr[20 + gg];
					if(nColor>255) nColor=255;
					if(nColor<128) nColor=128;
					m_cAmpBuffer[gg][nLine*m_nSamplesLine + nn] = nColor;
					break;
				case 1:	//Thickness
					pnBuffer = (int *)(&pcPtr[20 + m_nNumberAmpImages[0]]);
					c0 = pnBuffer[0];
					m_nTimeBuffer[0][nLine*m_nSamplesLine + nn] = c0;
					break;
				}
			}
		}
	}

	if(dVersion>=1.02) {
		pFile->Seek(nGlobalOffset,CUSLFile::begin);
		pFile->ReadStruct(&Global, sizeof m_Global);
	
		m_nNumberImages=0;
		pFile->Seek(nTimeSlotOffset,CUSLFile::begin);
		for(nTS=0;nTS<8;nTS++) {
			pFile->ReadStruct(&TS[nTS], sizeof TS[0]);
			if(TS[nTS].nTransmitter != TS[nTS].nReceiver) {
				swprintf_s(m_TS[nTS].cName,_T("Tx Left Rx Right"));
			} else {
				if(TS[nTS].nTransmitter==0) {
					swprintf_s(m_TS[nTS].cName,_T("Pulse Echo Left"));
				} else {
					swprintf_s(m_TS[nTS].cName,_T("Pulse Echo Right"));
				}
			}
			m_nNumberAmpImages[nTS]=0;
			m_nNumberTimeImages[nTS]=0;
			for(gg=0;gg<8;gg++) {
				m_TS[nTS].Gate.nCollectAmp[gg] = TS[nTS].Gate.nCollectAmp[gg];
				m_TS[nTS].Gate.nCollectTime[gg] = TS[nTS].Gate.nCollectTime[gg];
				if(TS[nTS].Gate.nCollectAmp[gg]==1) {
					m_nNumberAmpImages[nTS]++;
					m_nNumberImages++;
				}
				if(TS[nTS].Gate.nCollectTime[gg]==1) {
					m_nNumberTimeImages[nTS]++;
					m_nNumberImages++;
				}
				if(gg==0) {
					swprintf_s(m_TS[nTS].Gate.cName[gg],_T("Interface"));
				} else {
					swprintf_s(m_TS[nTS].Gate.cName[gg],_T("Gate %d"),gg);
				}
			}

		}
		/*
		pFile->Seek(nTestPlateOffset,CFile::begin);
		pFile->Read(&nGeneralOffset,sizeof nGeneralOffset);
		pFile->Read(&nNumberSamplesOffset,sizeof nNumberSamplesOffset);
		pFile->Read(&nBufferOffset,sizeof nBufferOffset);

		pFile->Seek(nGeneralOffset,CFile::begin);
		pFile->Read(&m_nTPSamplesLine,sizeof m_nTPSamplesLine);
		pFile->Read(&m_nTPNumberLines,sizeof m_nNumberLines);
		pFile->Read(&m_nNumberAmpImages,sizeof m_nNumberAmpImages);
		pFile->Read(&m_nNumberTimeImages,sizeof m_nNumberTimeImages);
		pFile->Read(&m_nTPItemSize,sizeof m_nTPItemSize);
		pFile->Read(&m_fTPFastScanSize,sizeof m_fTPFastScanSize);
		pFile->Read(&m_fTPSlowScanSize,sizeof m_fTPSlowScanSize);
		pFile->Read(&m_fTPFastIncrement,sizeof m_fTPFastIncrement);
		pFile->Read(&m_fTPSlowIncrement,sizeof m_fTPSlowIncrement);
		pFile->Read(&m_osTPTestDateTime,sizeof m_osTPTestDateTime);
		pFile->ReadString(&m_TPTestDateTime);
		pFile->ReadString(&m_ProfileName);
		pFile->ReadString(&m_Operator);
		pFile->ReadStruct(&m_CpTPStart);
		pFile->ReadStruct(&m_CpTPFinish);


		AllocateTestPieceMemory();
		pFile->Seek(nBufferOffset,CFile::begin);
		for(int nLine=0;nLine<m_nTPNumberLines;nLine++) {
			pFile->Read(&m_pTPBuffer[m_nTPSamplesLine*nLine*m_nTPItemSize],m_nTPItemSize * m_nTPSamplesLine );
		}
		*/
	}

	delete pBuffer;
	delete pnNumberSamples;
	return TRUE;
}

HRESULT CData::BuildARGBBuffer(int nImageNumber, int nImageType)
{
	int nn;

	SAFE_DELETE(m_pARGBArray);
	m_pARGBArray = new COLORREF[m_nSamplesLine * m_nNumberLines];
	unsigned char* pSrcData = m_cAmpBuffer[nImageNumber];
	int nOffset,nColor;
	COLORREF* pColor = theApp.m_Palette.m_rgbAmplitude[theApp.m_n3DPaletteNumber];
	char *pDest,*pSrc;

	for(int nLine=0;nLine<(m_nNumberLines-1);nLine++) {
		switch(theApp.m_LastSettings.nDisplayWhichLines) {
		case 0:nn = nLine;
			break;
		case 1:nn = nLine & 0xfffe;
			break;
		case 2:nn = (nLine & 0xfffe) + 1;
			if(nn>=m_nNumberLines) nn = m_nNumberLines-1;
			break;
		case 3: nn = nLine;
			break;
		}
		nOffset = nn * m_nSamplesLine;
		for(int nSample=0;nSample<m_nSamplesLine;nSample++,nOffset++) {
			switch(theApp.m_LastSettings.nDisplayWhichLines) {
			default: 
				pSrc = (char*)(&pColor[pSrcData[nOffset]]);
				pDest = (char*)(&m_pARGBArray[nOffset]);
				pDest[0] = pSrc[2];
				pDest[1] = pSrc[1];
				pDest[2] = pSrc[0];
				pDest[3] = (char)0xff;
				break;
			case 3: nColor = (pSrcData[nOffset] + pSrcData[nOffset + m_nSamplesLine]) / 2;
				pSrc = (char*)(&pColor[nColor]);
				pDest = (char*)(&m_pARGBArray[nOffset]);
				pDest[0] = pSrc[2];
				pDest[1] = pSrc[1];
				pDest[2] = pSrc[0];
				pDest[3] = (char)0xff;
				break;
			}
		}
	}

	return S_OK;
}

//Tessalate
HRESULT CData::GenerateTextureFacesCoordinates(CProgressCtrl* pProgress)
{
	int nMaxRows=theApp.m_LastSettings.nTesselateRows;
	int	nMaxColumns=theApp.m_LastSettings.nTesselateColumns;
	float fPos,fSize;
	int nn=0;
	int nRow,nColumn;
	TextureFace TF;
	CString Buff,FileName;
	CUSLFile File;
	CFileException e;
	CRect rr;
	int nCount;


	switch(m_nFastAxis) {
	default: fSize = m_CpFinish.fPos[m_nFastAxis] - m_CpStart.fPos[m_nFastAxis];
		break;
	case 5:	fSize = 360.0f;
		break;
	}

	Vertex* pVertex = new Vertex[nMaxRows * nMaxColumns];
	
	for(nRow=0;nRow<nMaxRows;nRow++) {
		for(nColumn=0;nColumn<nMaxColumns;nColumn++,nn++) {
			pVertex[nn].pt.x = MulDiv(nColumn,m_nSamplesLine,nMaxColumns-1);
			pVertex[nn].pt.y = MulDiv(nRow,(m_nNumberLines-1) * m_nLineIncrement,nMaxRows-1) + m_nScanStartLine;

			fPos = ((float)pVertex[nn].pt.x * fSize) / (float)(m_nSamplesLine-1) + m_CpStart.Side0.fPos[m_nFastAxis];
			pVertex[nn].bOnComponent = PROFILE->GetSurfaceCoordinate(pVertex[nn].p,pVertex[nn].pt,fPos,m_nFastAxis,true);
			pVertex[nn].pt.y -= m_nScanStartLine;
			pVertex[nn].pt.y /= m_nLineIncrement;
		}
	}

	SAFE_DELETE_ARRAY(m_pTextureFace);
	m_nTextureFaceL=0;
	for(nRow=0;nRow<(nMaxRows-1);nRow++) {
		for(nColumn=0;nColumn<(nMaxColumns-1);nColumn++) {
			TF.v[0] = pVertex[(nRow+1)*nMaxColumns + nColumn];
			TF.v[1] = pVertex[(nRow+1)*nMaxColumns + nColumn + 1];
			TF.v[2] = pVertex[nRow*nMaxColumns + nColumn + 1];
			TF.v[3] = pVertex[nRow*nMaxColumns + nColumn];
//			if(TF.v[0].bOnComponent | TF.v[1].bOnComponent | TF.v[2].bOnComponent | TF.v[3].bOnComponent) {
			if((TF.v[0].bOnComponent == TRUE) ||  (TF.v[1].bOnComponent == TRUE) || (TF.v[2].bOnComponent == TRUE) || (TF.v[3].bOnComponent == TRUE)) {
				m_nTextureFaceL++;
			} else {
				ASSERT(TRUE);
			}
 		}
	}
	if(pProgress != NULL) pProgress->SetRange(0,m_nTextureFaceL);

	m_pTextureFace = new TextureFace[m_nTextureFaceL];
	m_nTextureFaceL=0;
	for(nRow=0,nCount=0;nRow<(nMaxRows-1);nRow++) {
		for(nColumn=0;nColumn<(nMaxColumns-1);nColumn++) {

			if(pProgress != NULL) pProgress->SetPos(nCount++);

			TF.v[0] = pVertex[(nRow+1)*nMaxColumns + nColumn];
			TF.v[1] = pVertex[(nRow+1)*nMaxColumns + nColumn + 1];
			TF.v[2] = pVertex[nRow*nMaxColumns + nColumn + 1];
			TF.v[3] = pVertex[nRow*nMaxColumns + nColumn];
			if((TF.v[0].bOnComponent == TRUE) ||  (TF.v[1].bOnComponent == TRUE) || (TF.v[2].bOnComponent == TRUE) || (TF.v[3].bOnComponent == TRUE)) {
				m_pTextureFace[m_nTextureFaceL] = TF;

				FileName.Format(_T("C:\\Temp\\Texture%d.bmp"),m_nTextureFaceL);
				if(!File.Open(FileName, CFile::modeCreate|CFile::typeBinary|CFile::modeReadWrite, &e)) {
					File.DecodeFileException(&e,&Buff,FileName);
				} else {
					rr.TopLeft() = TF.v[3].pt;
					rr.BottomRight() = TF.v[1].pt;
					SaveBmpTexture(&File,rr);
					File.Close();

					m_nTextureFaceL++;
				}
			}
 		}
	}

	if(pProgress != NULL) pProgress->SetPos(0);

	SAFE_DELETE_ARRAY(pVertex);
	return S_OK;
}

int CData::GetNumberTextureFaces()
{
	return m_nTextureFaceL;
}

void CData::SaveBmpTexture(CUSLFile *pFile, CRect rrArea)
{
	#define NUMBERENTRIESPALETTE 0
	int yy,nStart;
	BITMAPFILEHEADER BmpFileHeader;
	BITMAPINFOHEADER BmpHeader;

	char *Ptr = (char*)&BmpFileHeader.bfType;
	strcpy_s(Ptr,2,"BM");
	BmpFileHeader.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 4*NUMBERENTRIESPALETTE + (rrArea.Width() * rrArea.Height() * sizeof (COLORREF));
	BmpFileHeader.bfReserved1 = 0;
	BmpFileHeader.bfReserved2 = 0;
	BmpFileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 4*NUMBERENTRIESPALETTE  ;


	BmpHeader.biSize          = sizeof(BITMAPINFOHEADER);
	BmpHeader.biWidth         = rrArea.Width();
	BmpHeader.biHeight        = rrArea.Height();
	BmpHeader.biPlanes        = 1;
	BmpHeader.biBitCount      = sizeof (COLORREF) * 8;
	BmpHeader.biCompression   = 0;
	BmpHeader.biSizeImage     = rrArea.Width() * rrArea.Height() * sizeof (COLORREF);
	BmpHeader.biXPelsPerMeter = 0;
	BmpHeader.biYPelsPerMeter = 0;
	BmpHeader.biClrUsed       = 0;
	BmpHeader.biClrImportant  = 0;
	
	pFile->SeekToBegin();
	pFile->Write(&BmpFileHeader, sizeof(BITMAPFILEHEADER));
	pFile->Write(&BmpHeader, sizeof(BITMAPINFOHEADER));

	if(((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==1)) && (m_nFastAxis!=5)){
		for(yy=rrArea.Height()-1;yy>=0;yy--) {
			nStart = (yy + rrArea.top) * m_nSamplesLine + rrArea.left;
			Ptr = (char *) &(m_pARGBArray[nStart]);
			pFile->Write(Ptr, rrArea.Width() * (sizeof COLORREF));
		}
	} else {
		if(theApp.m_LastSettings.nImageOrientation!=0) {
			for(yy=0;yy<rrArea.Height();yy++) {
				nStart = (yy + rrArea.top) * m_nSamplesLine + rrArea.left;
				Ptr = (char *) &(m_pARGBArray[nStart]);
				pFile->Write(Ptr, rrArea.Width() * (sizeof COLORREF));
			}
		} else {
			for(yy=rrArea.Height()-1;yy>=0;yy--) {
				nStart = (yy + rrArea.top) * m_nSamplesLine + rrArea.left;
				Ptr = (char *) &(m_pARGBArray[nStart]);
				pFile->Write(Ptr, rrArea.Width() * (sizeof COLORREF));
			}
		}
	}
}


void CData::ConvertVolumeFromThermometerToTwosComplement(CUSLFile* pFile)
{
	int	nIntPos = 0;
	ULONGLONG	nOffset;
	int ii;
	char Trace[10000];
	ULONGLONG nFileLength = pFile->GetLength();

	for(int nLine=0;nLine<m_nNumberLines;nLine++) {
		for(int nSample=0;nSample<m_nSamplesLine;nSample++) {

			nOffset = m_n64VolumeOffset[0] + 0x100 + (m_nSamplesLine * m_nAscanLength * nLine) + (nSample * m_nAscanLength) + 4;
			if((nOffset + m_nAscanLength) <= nFileLength) {
				pFile->Seek(nOffset,CFile::begin);
				pFile->Read(&Trace,m_nAscanLength-4);
				for(ii=0;ii<m_nAscanLength-4;ii++) {
					Trace[ii] += (char)128;
				}
				pFile->Seek(nOffset,CFile::begin);
				pFile->Write(&Trace,m_nAscanLength-4);
			}
		}
	}


}

void CData::DisplayThumbNail(CString csFilePath, CDC *pDC, CRect rr)
{
	CUSLFile File;
	CFileException e;

	if(!File.Open(csFilePath, CFile::modeRead | CFile::typeBinary,&e)) return;

/////////////////////////////////////////////////
//	UNICODE
	WCHAR wIdentifier[]=_T("Ultrasonic Sciences Ltd USLScanner Image Data");
	WCHAR wStr[100];
	ZeroMemory(wStr,sizeof wStr);

	File.Seek(0,CUSLFile::begin);
	File.Read(wStr,sizeof wIdentifier);
	if(wcscmp(wStr,wIdentifier)==0) {
		RetrieveCompatibleThumbNail(&File,pDC,rr);
		File.Close();
		return;
	}


	File.Close();
}

void CData::RetrieveCompatibleThumbNail(CUSLFile *pFile, CDC *pDC, CRect rr)
{
	int nVariablesOffset,nStringsOffset,nGlobalOffset,nTimeSlotOffset,nDataOffset,nAnnotationOffset,nDetailsOffset,nCoordOffset,nImageNameOffset;
	UINT64 n64VariablesOffset,n64StringsOffset,n64GlobalOffset,n64TimeSlotOffset,n64DataOffset,n64AnnotationOffset,n64DetailsOffset,n64CoordOffset,n64ImageNameOffset;
	int gg,tt,pp,nSize,nLine,nSample,xx,yy,nTS,ii,xxCount;
	float fVersion;
	char cc;
	CBitmap bmp;
	CDC  dcMem;

	Zero();

	pFile->Read(&fVersion,sizeof fVersion);

	if(fVersion<3.07f) {
		pFile->Seek(0x100,CUSLFile::begin);
		pFile->Read(&nVariablesOffset,sizeof nVariablesOffset);
		pFile->Read(&nStringsOffset,sizeof nStringsOffset);
		pFile->Read(&nGlobalOffset,sizeof nGlobalOffset);
		pFile->Read(&nTimeSlotOffset,sizeof nTimeSlotOffset);
		pFile->Read(&nDataOffset,sizeof nDataOffset);
		pFile->Read(&nAnnotationOffset,sizeof nAnnotationOffset);
		pFile->Read(m_n32VolumeOffset,sizeof m_n32VolumeOffset);
		pFile->Read(&nDetailsOffset,sizeof nDetailsOffset);
		pFile->Read(&nCoordOffset,sizeof nCoordOffset);
		pFile->Read(&nImageNameOffset,sizeof nImageNameOffset);
		n64VariablesOffset = nVariablesOffset;
		n64StringsOffset = nStringsOffset;
		n64GlobalOffset = nGlobalOffset;
		n64TimeSlotOffset = nTimeSlotOffset;
		n64DataOffset = nDataOffset;
		n64AnnotationOffset = nAnnotationOffset;
		n64DetailsOffset = nDetailsOffset;
		n64CoordOffset = nCoordOffset;
		n64ImageNameOffset = nImageNameOffset;
		for(ii=0;ii<8;ii++) {
			m_n64VolumeOffset[ii] = m_n32VolumeOffset[ii];
		}
	} else {
		pFile->Seek(0x100,CUSLFile::begin);
		pFile->Read(&n64VariablesOffset,sizeof n64VariablesOffset);
		pFile->Read(&n64StringsOffset,sizeof n64StringsOffset);
		pFile->Read(&n64GlobalOffset,sizeof n64GlobalOffset);
		pFile->Read(&n64TimeSlotOffset,sizeof n64TimeSlotOffset);
		pFile->Read(&n64DataOffset,sizeof n64DataOffset);
		pFile->Read(&n64AnnotationOffset,sizeof n64AnnotationOffset);
		pFile->Read(m_n64VolumeOffset,sizeof m_n64VolumeOffset);
		pFile->Read(&n64DetailsOffset,sizeof n64DetailsOffset);
		pFile->Read(&n64CoordOffset,sizeof n64CoordOffset);
		pFile->Read(&n64ImageNameOffset,sizeof n64ImageNameOffset);
	}

	if(n64VariablesOffset>0) {
		pFile->Seek(n64VariablesOffset,CUSLFile::begin);
		pFile->Read(&m_nSamplesLine,sizeof m_nSamplesLine);
		pFile->Read(&m_nNumberLines,sizeof m_nNumberLines);
		pFile->Read(&m_fFastScanSize,sizeof m_fFastScanSize);
		pFile->Read(&m_fSlowScanSize,sizeof m_fSlowScanSize);
		pFile->Read(&m_fFastIncrement,sizeof m_fFastIncrement);
		pFile->Read(&m_fSlowIncrement,sizeof m_fSlowIncrement);
		pFile->Read(&m_nNumberImages,sizeof m_nNumberImages);
		pFile->Read(&m_nNumberTimeSlots,sizeof m_nNumberTimeSlots);
		pFile->Read(m_nNumberAmpImages,sizeof m_nNumberAmpImages);
		pFile->Read(m_nNumberTimeImages,sizeof m_nNumberTimeImages);
		pFile->Read(m_nNumberPhaseImages,sizeof m_nNumberPhaseImages);
		pFile->Read(&m_nSentence,sizeof m_nSentence);
		pFile->Read(&m_osTestDateTime,sizeof m_osTestDateTime);
		pFile->Read(&m_nNumberVolumeImages,sizeof m_nNumberVolumeImages);
		pFile->Read(&m_nScanStartLine,sizeof m_nScanStartLine);
		pFile->Read(&m_nScanFinishLine,sizeof m_nScanFinishLine);
		pFile->Read(&m_nLineIncrement,sizeof m_nLineIncrement);
		pFile->Read(&m_nComponentDescription,sizeof m_nComponentDescription);
		pFile->Read(&m_nScanType,sizeof m_nScanType);
		pFile->Read(m_fComponentDiameter,sizeof m_fComponentDiameter);
		pFile->Read(&m_nFastAxis,sizeof m_nFastAxis);
		pFile->Read(&m_nSlowAxis,sizeof m_nSlowAxis);
		pFile->Read(&m_nNumberProbesPerImage,sizeof m_nNumberProbesPerImage);
		pFile->Read(&m_nLineIncrementMode,sizeof m_nLineIncrementMode);
	}

	if((n64DataOffset>0) && (m_nNumberTimeSlots>0)) {
		pFile->Seek(n64DataOffset,CUSLFile::begin);
		nSize = m_nSamplesLine * m_nNumberLines;
		gg=tt=pp=0;
		for(nTS=0;nTS<m_nNumberTimeSlots;nTS++) {
			if(m_nNumberAmpImages[nTS]) {
				char *pSrc = new char[nSize];
				char *pScreenArray = new char[rr.Width() * rr.Height() * 4];
				char *pDest;

				pFile->Read(pSrc,nSize * sizeof (unsigned char));
				for(yy=0;yy<rr.Height();yy++) {
					if( (theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2) ) {
						nLine = MulDiv(yy,m_nNumberLines-1,rr.Height()-1);
					} else {
						nLine = m_nNumberLines-1-MulDiv(yy,m_nNumberLines-1,rr.Height()-1);
					}
					pDest = &pScreenArray[yy * rr.Width() * 4];
					for(xxCount=0;xxCount<rr.Width();xxCount++) {
						if((theApp.m_LastSettings.nImageOrientation==2) || (theApp.m_LastSettings.nImageOrientation==3)){
							xx=(rr.Width()-xxCount);
						}else{
							xx=xxCount;
						}
						nSample = MulDiv(xx,m_nSamplesLine-1,rr.Width()-1);
						cc=pSrc[nLine * m_nSamplesLine + nSample] - 128;
						*pDest++ = cc;
						*pDest++ = cc;
						*pDest++ = cc;
						*pDest++ = 0;
					}
				}
				bmp.CreateCompatibleBitmap(pDC,rr.Width(),rr.Height());
				bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),pScreenArray);
				dcMem.CreateCompatibleDC(pDC);
				CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

				pDC->BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
				dcMem.SelectObject(pOldBitmap);
			
				delete pSrc;
				delete pScreenArray;
				return;

			} else {
				if(m_nNumberTimeImages[nTS]) {
	//				pFile->Read(m_nTimeBuffer[tt++],nSize * sizeof (unsigned int));
					return;
				} else {
					if(m_nNumberPhaseImages[nTS]) {
	//					pFile->Read(m_cPhaseBuffer[pp++],nSize * sizeof (unsigned char));
						return;
					}
				}
			}
		}
	}


}


void CData::ClearDetails(int nActionFlag)
{
	int nn,nEditL;

	for(nn=0,nEditL=0;nn<theApp.m_nDescriptionL;nn++) {
		switch(theApp.m_pDescription[nn].nCtrlType) {
		case EDIT_CTL:
			if((nActionFlag & CTL_LEVEL_0) && (theApp.m_pDescription[nn].nActionFlag & CTL_LEVEL_0)) {
				m_Details[nEditL].Empty();
			}
			if((nActionFlag & CTL_LEVEL_1) && (theApp.m_pDescription[nn].nActionFlag & CTL_LEVEL_1)) {
				m_Details[nEditL].Empty();
			}
			if((nActionFlag & CTL_LEVEL_2) && (theApp.m_pDescription[nn].nActionFlag & CTL_LEVEL_2)) {
				m_Details[nEditL].Empty();
			}
			nEditL++;
			break;

		}
	}
}

void CData::FillAutomaticDetails(int nActionFlag)
{
	int nn;
	int nEditL;

	COleDateTime CurrentTime = COleDateTime::GetCurrentTime();

	TCHAR sLongDate[255];
	TCHAR sShortDate[255];
	TCHAR sShortTime[255];

    SYSTEMTIME st;

    st.wYear = (WORD)CurrentTime.GetYear();
    st.wMonth = (WORD)CurrentTime.GetMonth();
    st.wDayOfWeek = 0;
    st.wDay = (WORD)CurrentTime.GetDay();
    st.wHour = (WORD)CurrentTime.GetHour();
    st.wMinute = (WORD)CurrentTime.GetMinute();
    st.wSecond = (WORD)CurrentTime.GetSecond();;
    st.wMilliseconds = 0;

    GetDateFormat(LOCALE_USER_DEFAULT,DATE_LONGDATE,&st,NULL,sLongDate,254);
    GetDateFormat(LOCALE_USER_DEFAULT,DATE_SHORTDATE,&st,NULL,sShortDate,254);
	GetTimeFormat(LOCALE_USER_DEFAULT,TIME_NOSECONDS,&st,NULL,sShortTime,254);


	for(nn=0,nEditL=0;nn<theApp.m_nDescriptionL;nn++) {
		switch(theApp.m_pDescription[nn].nCtrlType) {
		case EDIT_CTL:

			if(((nActionFlag & theApp.m_pDescription[nn].nActionFlag) & (CTL_DATE | CTL_TIME)) == (CTL_DATE | CTL_TIME)) {
				m_Details[nEditL].Format(_T("%s %s"),sShortDate, sShortTime);
			}

			if(((nActionFlag & theApp.m_pDescription[nn].nActionFlag) & (CTL_DATE | CTL_TIME)) == CTL_DATE) {
				m_Details[nEditL] = sShortDate;
			}

			if(((nActionFlag & theApp.m_pDescription[nn].nActionFlag) & (CTL_DATE | CTL_TIME)) == CTL_TIME) {
				m_Details[nEditL] = sShortTime;
			}


			if((nActionFlag & CTL_OPERATOR) && (theApp.m_pDescription[nn].nActionFlag & CTL_OPERATOR)) {
				m_Details[nEditL] = theApp.m_Operator;
			}

			if((nActionFlag & CTL_FILENAME) && (theApp.m_pDescription[nn].nActionFlag & CTL_FILENAME)) {

				CFileDialog FileDlg(FALSE,_T(".dat"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Image (*.dat)|*.dat|All Files (*.*)|*.*|"),NULL);
				FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath[0];
				if(FileDlg.DoModal()==IDOK) {
					m_Details[nEditL] = FileDlg.GetPathName();
				}
			}

			nEditL++;
			break;

		}
	}

}


void CData::ReadEveryAscan(CUSLFile* pFile,int nVolumeNumber)
{
	m_pFile64 = pFile;
	m_nVolumneNumber = nVolumeNumber;

	AfxBeginThread(&ReadAllVolume,this,THREAD_PRIORITY_NORMAL ,0,NULL);
}

UINT ReadAllVolume(LPVOID pParam)
{
	CData* pData = (CData*)pParam;
	char *Buffer = new char[100000000];
	UINT64 n64FileLength = pData->m_pFile64->GetLength();
	UINT64 n64Offset = pData->m_n64VolumeOffset[0] + 0x100;
	UINT64 n64Stop = ((UINT64)pData->m_nSamplesLine * (UINT64)pData->m_nAscanLength * (UINT64)pData->m_nNumberLines) + n64Offset;

	pData->m_pFile64->Seek(n64Offset,CUSLFile::begin);
	for(n64Offset;n64Offset<n64Stop;n64Offset+=100000000) {
		pData->m_pFile64->Read(Buffer,100000000);
	}
	delete Buffer;

	return 0;
}


bool CData::SaveVolumeXMLAttempt0(CUSLFile *pDestFile,CUSLFile *pSrcFile,int nVolumeNumber)
{
	CProgressDlg dlg(NULL);
	dlg.Create(IDD_PROGRESS_DIALOG, NULL);	
	TCHAR Number[256][10];
	int nLength[256];
	CString Buff;
	ULONGLONG n64Offset;
	char *Buffer = new char[m_nSamplesLine * m_nAscanLength];
	char *pTrace;
	int *pnTrace;
	TCHAR *str = new TCHAR[m_nAscanLength * 10];
	int nDestPtr,nAmp;

	for(int ii=0;ii<256;ii++) {
		swprintf_s(Number[ii],_T("%d,L"),ii-128);
		nLength[ii]=wcslen(Number[ii]);
	}



	Buff.Format(_T("<?xml version=\x022%.01f\x022?>\r\n"),1.0f);	pDestFile->WriteStringChar(Buff);


	pDestFile->WriteStringChar(_T("<all_info>\r\n"));
	pDestFile->WriteStringChar(_T("<header>\r\n"));

	Buff.Format(_T("<data_file>%s</data_file>\r\n"),m_FileName);	pDestFile->WriteStringChar(Buff);
	Buff.Format(_T("<image_type>volume</image_type>\r\n"));		pDestFile->WriteStringChar(Buff);
	Buff.Format(_T("<data_range>-128 to 127</data_range>\r\n"));	pDestFile->WriteStringChar(Buff);
	Buff.Format(_T("<units>percentage</units>\r\n"));	pDestFile->WriteStringChar(Buff);
	Buff.Format(_T("<volume_number>%d</volume_number>\r\n"),nVolumeNumber+1);	pDestFile->WriteStringChar(Buff);
	Buff.Format(_T("<samples_per_line>%d</samples_per_line>\r\n"),m_nSamplesLine);	pDestFile->WriteStringChar(Buff);
	Buff.Format(_T("<number_of_lines>%d</number_of_lines>\r\n"),m_nNumberLines);	pDestFile->WriteStringChar(Buff);
	Buff.Format(_T("<increment_along_line>%.02f</increment_along_line>\r\n"),m_fFastIncrement);	pDestFile->WriteStringChar(Buff);
	Buff.Format(_T("<increment_between_lines>%.02f</increment_between_lines>\r\n"),m_fSlowIncrement);	pDestFile->WriteStringChar(Buff);
	Buff.Format(_T("<scan_width>%.02f</scan_width>\r\n"),m_fFastScanSize);	pDestFile->WriteStringChar(Buff);
	Buff.Format(_T("<scan_height>%.02f</scan_height>\r\n"),m_fSlowScanSize);	pDestFile->WriteStringChar(Buff);


	pDestFile->WriteStringChar(_T("</header>\r\n"));


	pDestFile->WriteStringChar(_T("<data>\r\n"));

	dlg.SetProgressRange(0,m_nNumberLines);

	for(int nLine=0,nPtr=0;nLine<m_nNumberLines;nLine++) {
		dlg.SetProgressPos(nLine);
		Buff.Format(_T("%d of %d"),nLine,m_nNumberLines);
		dlg.SetProgressText(Buff);

		Buff.Format(_T("  <line_%d>\r\n"),nLine);	pDestFile->WriteStringChar(Buff);

		n64Offset = m_n64VolumeOffset[nVolumeNumber] + 0x100 + ((UINT64)m_nSamplesLine * (UINT64)m_nAscanLength * (UINT64)nLine);
		pSrcFile->Seek(n64Offset,CUSLFile::begin);
		pSrcFile->Read(Buffer,m_nSamplesLine * m_nAscanLength);


		for(int nSample=0;nSample<m_nSamplesLine;nSample++) {
			Buff.Format(_T("    <sample_%d>\r\n"),nSample);	pDestFile->WriteStringChar(Buff);

			pTrace = &Buffer[nSample * m_nAscanLength];
			pnTrace = (int*)pTrace;
			
			int nIntPos = pnTrace[0];
			Buff.Format(_T("      <interface_pos>%d</interface_pos>\r\n"),nIntPos);	pDestFile->WriteStringChar(Buff);
	
			Buff.Format(_T("      <trace_data>"));	pDestFile->WriteStringChar(Buff);
			ZeroMemory(str,m_nAscanLength * 10 * sizeof(TCHAR));
			for(nPtr=4,nDestPtr=0;nPtr<m_nAscanLength;nPtr++) {
				nAmp = pTrace[nPtr]+128;
				CopyMemory(&str[nDestPtr],Number[nAmp],nLength[nAmp] * sizeof(TCHAR));
				nDestPtr += nLength[nAmp];
			}
			pDestFile->WriteStringChar(str);
			Buff="</trace_data>\r\n";	pDestFile->WriteStringChar(Buff);

			Buff.Format(_T("    </sample_%d>\r\n"),nSample);	pDestFile->WriteStringChar(Buff);
		}
		Buff.Format(_T("  </line_%d>\r\n"),nLine);	pDestFile->WriteStringChar(Buff);
	}
	pDestFile->WriteStringChar(_T("</data>\r\n"));

	pDestFile->WriteStringChar(_T("</all_info>\r\n"));

	delete Buffer;
	delete str;

	pDestFile->SetLength(pDestFile->GetPosition());
	return TRUE;
}

bool CData::SaveCSV(CUSLFile *pFile, int nTimeSlot, int nImageType, int nImageNumber)
{
	CString Buff;
	CString Temp;
	CArchive *pArch;
	pArch = new CArchive(pFile,CArchive::store);
	int nFSH;
	float fThickness;
	int nExt;
	double dRange[4] = {10.0, 5.0, 2.5, 1.25};
	double dValue;

	CProgressDlg dlg(NULL);
	dlg.Create(IDD_PROGRESS_DIALOG, NULL);	

	switch(nImageType) {
	case 0:	Buff.Format(_T("ImageType = Amplitude\n"));
		break;
	case 1:
		switch(theApp.m_LastSettings.nXMLTOFRule) {
		case 0:Buff.Format(_T("ImageType = Thickness Units = ns\n"));
			break;
		case 1:Buff.Format(_T("ImageType = Thickness Units = us\n"));
			break;
		case 2:Buff.Format(_T("ImageType = Thickness Units = mm\n"));
			break;
		}
		break;
	case 2:	Buff.Format(_T("ImageType = Phase\n"));
		break;
	case EXT_IMAGE: 
		switch(theApp.m_LastSettings.nExtSourceDisplayMode) {
		case 0:	Buff.Format(_T("ImageType = External,Units = Unsigned decimal\n"));
			break;
		case 1:	Buff.Format(_T("ImageType = External,Units = Signed decimal\n"));
			break;
		case 2:	Buff.Format(_T("ImageType = External,Units = Volts\n"));
			break;
		case 3:	Buff.Format(_T("ImageType = External,Units = %%\n"));
			break;
		};
		break;
	}

	pArch->WriteString(Buff);
	
	int nPtr = 0;
	for(int nLine=0;nLine<m_nNumberLines;nLine++) {
		dlg.SetProgressPos(MulDiv(nLine, 100, m_nNumberLines));
		Buff.Format(_T("Saving coordinates %d of %d"),nLine,m_nNumberLines);
		dlg.SetProgressText(Buff);

		Buff.Format(L"Line = %d",nLine+1);
		for(int nSample=0;nSample<m_nSamplesLine;nSample++) {
			switch(nImageType) {
			case 0:
				nFSH = MulDiv(m_cAmpBuffer[nImageNumber][nPtr++]-128,100,127);
				if(nFSH>=0) {
					Temp.Format(_T(",%d"),nFSH);
				}else{
					Temp.Format(_T(",L"));
				}
				Buff += Temp;
				break;
			case 1:
				switch(theApp.m_LastSettings.nXMLTOFRule) {
				case 0:	Temp.Format(_T(",%d"),m_nTimeBuffer[nImageNumber][nPtr++]);
					break;
				case 1:	Temp.Format(_T(",%.03f"),(float)m_nTimeBuffer[nImageNumber][nPtr++] / 1000.0f);
					break;
				case 2:
					theApp.TimeToThickness(m_nTimeBuffer[nImageNumber][nPtr++], &fThickness);
					Temp.Format(_T(",%0.3f"),fThickness);
					break;
				}
				Buff += Temp;
				break;
			case EXT_IMAGE:
				nExt = m_16ExtBuffer[nImageNumber][nPtr++];
				switch(theApp.m_LastSettings.nExtSourceDisplayMode) {
				default:	//Unsigned decimal
					Temp.Format(L",%d",nExt);
					break;
				case 1:	//Signed Decimal
					Temp.Format(L",%d",nExt-32768);
					break;
				case 2: //Volts;
					dValue = (double)(nExt-32768) * dRange[ theApp.m_UtUser.m_Global.Ext.cGain[nImageNumber] & 3 ] / 32768.0;
					Temp.Format(L",%.02f",dValue);
					break;
				case 3: //%
					dValue = (double)(nExt-32768) * 100.0 / 32768.0;
					Temp.Format(L",%.02f",dValue);
					break;
				}
				Buff += Temp;

				break;
			} //switch
		}//for sample
		Temp.Format(_T("\n"));
		Buff += Temp;
		pArch->WriteString(Buff);
	}//for line


	pArch->Close();
	delete pArch;

	pFile->SetLength(pFile->GetPosition());

	return TRUE;

}


bool CData::SaveXMLAttempt0(CUSLFile *pFile, int nTimeSlot, int nImageType, int nImageNumber)
{
	CProgressDlg dlg(NULL);
	dlg.Create(IDD_PROGRESS_DIALOG, NULL);	
	CString Buff;
	CArchive *pArch;
	pArch = new CArchive(pFile,CArchive::store);

	Buff.Format(_T("<?xml version=\x022%.01f\x022?>\r\n"),1.0f);	pArch->WriteString(Buff);


	pArch->WriteString(_T("<all_info>\r\n"));
	pArch->WriteString(_T("<header>\r\n"));

	Buff.Format(_T("<data_file>%s</data_file>\r\n"),m_FileName);	pArch->WriteString(Buff);
	switch(nImageType) {
	case 0:	Buff.Format(_T("<image_type>amplitude</image_type>\r\n"));		pArch->WriteString(Buff);
		Buff.Format(_T("<data_range>0-100</data_range>\r\n"));	pArch->WriteString(Buff);
		Buff.Format(_T("<units>percentage</units>\r\n"));	pArch->WriteString(Buff);
		break;
	case 1:	Buff.Format(_T("<image_type>time of flight</image_type>\r\n"));		pArch->WriteString(Buff);
		Buff.Format(_T("<data_range>0-400000</data_range>\r\n"));	pArch->WriteString(Buff);
		Buff.Format(_T("<units>ns</units>\r\n"));	pArch->WriteString(Buff);
		break;
	case 2:	Buff.Format(_T("<image_type>phase</image_type>\r\n"));		pArch->WriteString(Buff);
		Buff.Format(_T("<data_range>0-1</data_range>\r\n"));	pArch->WriteString(Buff);
		Buff.Format(_T("<units>none</units>\r\n"));	pArch->WriteString(Buff);
		break;
	}
	Buff.Format(_T("<image_number>%d</image_number>\r\n"),nImageNumber+1);	pArch->WriteString(Buff);
	Buff.Format(_T("<samples_per_line>%d</samples_per_line>\r\n"),m_nSamplesLine);	pArch->WriteString(Buff);
	Buff.Format(_T("<number_of_lines>%d</number_of_lines>\r\n"),m_nNumberLines);	pArch->WriteString(Buff);
	Buff.Format(_T("<increment_along_line>%.02f</increment_along_line>\r\n"),m_fFastIncrement);	pArch->WriteString(Buff);
	Buff.Format(_T("<increment_between_lines>%.02f</increment_between_lines>\r\n"),m_fSlowIncrement);	pArch->WriteString(Buff);
	Buff.Format(_T("<scan_width>%.02f</scan_width>\r\n"),m_fFastScanSize);	pArch->WriteString(Buff);
	Buff.Format(_T("<scan_height>%.02f</scan_height>\r\n"),m_fSlowScanSize);	pArch->WriteString(Buff);


	pArch->WriteString(_T("</header>\r\n"));


	pArch->WriteString(_T("<data>\r\n"));

	int nPtr = 0;
	for(int nLine=0;nLine<m_nNumberLines;nLine++) {
		dlg.SetProgressPos(nLine);
		Buff.Format(_T("%d of %d"),nLine,m_nNumberLines);
		dlg.SetProgressText(Buff);

		Buff.Format(_T("  <line_%d>"),nLine);	pArch->WriteString(Buff);

		for(int nSample=0;nSample<m_nSamplesLine;nSample++) {
			switch(nImageType) {
			case 0:	Buff.Format(_T("%d,L"),MulDiv(m_cAmpBuffer[nImageNumber][nPtr++]-128,100,127));
				break;
			case 1:	Buff.Format(_T("%d,L"),m_nTimeBuffer[nImageNumber][nPtr++]);
				break;
			case 2:	Buff.Format(_T("%d,L"),m_cPhaseBuffer[nImageNumber][nPtr++]);
				break;
			}
			pArch->WriteString(Buff);
		}
		Buff.Format(_T("</line_%d>\r\n"),nLine);	pArch->WriteString(Buff);
	}
	pArch->WriteString(_T("</data>\r\n"));

	pArch->WriteString(_T("</all_info>\r\n"));

	pArch->Close();
	delete pArch;

	pFile->SetLength(pFile->GetPosition());

	return TRUE;
}


void CData::XMLEntryLabel(CString strLabel,int nValue,CArchive *pArch)
{
	CString Buff;

	Buff.Format(_T("<entry label=\x022%s\x022>%d</entry>\r\n"),strLabel,nValue);
	pArch->WriteString(Buff);
}

void CData::XMLEntryLabel(CString strLabel,CString strField,CArchive *pArch)
{
	CString Buff;

	Buff.Format(_T("<entry label=\x022%s\x022>%s</entry>\r\n"),strLabel,strField);
	pArch->WriteString(Buff);
}

void CData::XMLEntryLabel(CString strLabel,CString strFormat,float fValue,CArchive *pArch)
{
	CString Buff,Temp;

	Temp.Format(_T("<entry label=\x022%s\x022>%s</entry>\r\n"),strLabel,strFormat);
	Buff.Format(Temp,fValue);
	pArch->WriteString(Buff);
}

void CData::XMLValuePid(int nSample, int nValue, CArchive *pArch)
{
	CString Buff;

	Buff.Format(_T("\t<value pid=\x022%d\x022>%d</value>\r\n"),nSample,nValue);
	pArch->WriteString(Buff);
}

void CData::XMLValuePid(int nSample, float fValue, int nPrecision, CArchive *pArch)
{
	CString Buff;

	Buff.Format(_T("\t<value pid=\x022%d\x022>%0.*f</value>\r\n"),nSample,nPrecision,fValue);
	pArch->WriteString(Buff);
}

void CData::XMLValuePid(int nSample, double dValue, int nPrecision, CArchive *pArch)
{
	CString Buff;

	Buff.Format(_T("\t<value pid=\x022%d\x022>%0.*f</value>\r\n"),nSample,nPrecision,dValue);
	pArch->WriteString(Buff);
}

void CData::XLMPointId(int nPtr, CCoord Cp, CArchive *pArch)
{
	CString Buff;

	Buff.Format(_T("<point id=\x022%d\x022>%.01f %.01f %.01f</point>\r\n"),nPtr,Cp.Side0.fX,Cp.Side0.fY,Cp.Side0.fZ);
	pArch->WriteString(Buff);

}

bool CData::SaveXMLAttempt1(CUSLFile *pFile, int nTimeSlot, int nImageType, int nImageNumber)
{
	CProgressDlg dlg(NULL);
	dlg.Create(IDD_PROGRESS_DIALOG, NULL);	
	CString Buff,Field;
	CArchive *pArch;
	pArch = new CArchive(pFile,CArchive::store);
	int nLine,nPtr,nSample;
	CPoint pt;
	float fPos,fThickness;
	CCoord Cp;
	CString Micro = L"\x0b5s";
	int nExt;
	double dRange[4] = {10.0, 5.0, 2.5, 1.25 };
	double dValue;

	Buff.Format(_T("<?xml version=\x022%.01f\x022?>\r\n"),1.0f);	pArch->WriteString(Buff);
	Buff.Format(_T("<usl version=\x022%.01f\x022>\r\n"),1.0f);			pArch->WriteString(Buff);

	if(theApp.m_LastSettings.nXMLSaveHeader == 1) {
		pArch->WriteString(_T("<header>\r\n"));

		XMLEntryLabel(_T("data_file"),m_FileName,pArch);
		switch(nImageType) {
		case 0:
			XMLEntryLabel(L"image_type",L"amplitude",pArch);
			XMLEntryLabel(L"data_range",L"0-100",pArch);
			XMLEntryLabel(L"units",L"percentage",pArch);
			break;
		case 1:
			XMLEntryLabel(L"image_type",L"time of flight",pArch);
			XMLEntryLabel(L"data_range",L"0-400000",pArch);
			switch(theApp.m_LastSettings.nXMLTOFRule) {
			case 0:	XMLEntryLabel(L"units",L"ns",pArch);
				break;
			case 1:	XMLEntryLabel(L"units",Micro,pArch);
				break;
			case 2:	XMLEntryLabel(L"units",L"mm",pArch);
				break;
			}
			break;
		case 2:
			XMLEntryLabel(L"image_type",L"phase",pArch);
			XMLEntryLabel(L"data_range",L"0-1",pArch);
			XMLEntryLabel(L"units",L"none",pArch);
			break;
		case EXT_IMAGE:
			XMLEntryLabel(L"image_type",L"External",pArch);
			switch(theApp.m_LastSettings.nExtSourceDisplayMode) {
			case 0:	XMLEntryLabel(L"data_range",L"0-65535",pArch);
				XMLEntryLabel(L"units",L"none",pArch);
				break;
			case 1:	XMLEntryLabel(L"data_range",L"-32768-32767",pArch);
				XMLEntryLabel(L"units",L"none",pArch);
				break;
			case 2:	XMLEntryLabel(L"data_range",L"-10.0-10.0",pArch);
				XMLEntryLabel(L"units",L"Volts",pArch);
				break;
			case 3:	XMLEntryLabel(L"data_range",L"-100.00-100.00",pArch);
				XMLEntryLabel(L"units",L"%",pArch);
				break;
			};
			break;
		}
		XMLEntryLabel(L"image_number",nImageNumber+1,pArch);
		XMLEntryLabel(L"samples_per_line",m_nSamplesLine,pArch);
		XMLEntryLabel(L"number_of_lines",m_nNumberLines,pArch);
		XMLEntryLabel(L"increment_along_line",L"%.02f",m_fFastIncrement,pArch);
		XMLEntryLabel(L"increment_between_lines",L"%.02f",m_fSlowIncrement,pArch);
		XMLEntryLabel(L"scan_width",L"%.02f",m_fFastScanSize,pArch);
		XMLEntryLabel(L"scan_height",L"%.02f",m_fSlowScanSize,pArch);


		pArch->WriteString(_T("</header>\r\n"));
	}

	////////////////// sample points ///////////////////////
	if(theApp.m_LastSettings.nXMLSaveCoordinates == 1) {
		pArch->WriteString(_T("<sample_points>\r\n"));
		for(nLine=0,nPtr=1;nLine<m_nNumberLines;nLine++) {
			dlg.SetProgressPos(MulDiv(nLine, 100, m_nNumberLines));
			Buff.Format(_T("Saving coordinates %d of %d"),nLine,m_nNumberLines);
			dlg.SetProgressText(Buff);

			for(nSample=0;nSample<m_nSamplesLine;nSample++,nPtr++) {
				pt.x = nSample;
				pt.y = nLine;
				pt.y *= m_nLineIncrement;
				pt.y += m_nScanStartLine;
				fPos = ((float)pt.x * m_fFastScanSize) / (float)(m_nSamplesLine-1) + m_CpStart.Side0.fX;
				PROFILE->GetSurfaceCoordinate(Cp,pt,fPos,m_nFastAxis);
				XLMPointId(nPtr,Cp,pArch);
			}
		}
		pArch->WriteString(_T("</sample_points>\r\n"));
	}
	////////////////// data ////////////////////////////////
	if(theApp.m_LastSettings.nXMLSaveData == 1) {
		pArch->WriteString(_T("<data>\r\n"));
		for(nLine=0,nPtr=1;nLine<m_nNumberLines;nLine++) {
			dlg.SetProgressPos(MulDiv(nLine, 100, m_nNumberLines));
			Buff.Format(_T("Saving values %d of %d"),nLine,m_nNumberLines);
			dlg.SetProgressText(Buff);

			Buff.Format(_T("  <line id=\x022%d\x022>\r\n"),nLine+1);	pArch->WriteString(Buff);

			for(nSample=0;nSample<m_nSamplesLine;nSample++,nPtr++) {
				switch(nImageType) {
				case 0:
					XMLValuePid(nPtr, MulDiv(m_cAmpBuffer[nImageNumber][nPtr]-128,100,127), pArch);
					break;
				case 1:
					switch(theApp.m_LastSettings.nXMLTOFRule) {
					case 0:	XMLValuePid(nPtr, m_nTimeBuffer[nImageNumber][nPtr], pArch);	//ns
						break;
					case 1:	XMLValuePid(nPtr, (float)m_nTimeBuffer[nImageNumber][nPtr]/1000.0f,3, pArch);	//us
						break;
					case 2:
						theApp.TimeToThickness(m_nTimeBuffer[nImageNumber][nPtr],&fThickness);
						XMLValuePid(nPtr, fThickness, 3, pArch);	//mm
						break;
					}
					break;
				case 2:
					XMLValuePid(nPtr, m_cPhaseBuffer[nImageNumber][nPtr], pArch);
					break;
				case EXT_IMAGE:
					nExt = m_16ExtBuffer[nImageNumber][nPtr];
					switch(theApp.m_LastSettings.nExtSourceDisplayMode) {
					case 0:	//Unsigned decimal
						XMLValuePid(nPtr, nExt, pArch);
						break;
					case 1:	//Signed Decimal
						XMLValuePid(nPtr, nExt-32768, pArch);
						break;
					case 2: //Volts;
						dValue = (double)(nExt-32768) * dRange[ theApp.m_UtUser.m_Global.Ext.cGain[nImageNumber] & 3 ] / 32768.0;
						XMLValuePid(nPtr, dValue, 2, pArch);
						break;
					case 3: //%
						dValue = (double)(nExt-32768) * 100.0 / 32768.0;
						XMLValuePid(nPtr, dValue, 2, pArch);
						break;
					}
					break;

				}
			}
			Buff.Format(_T("  </line>\r\n"));	pArch->WriteString(Buff);
		}
		pArch->WriteString(_T("</data>\r\n"));
	}

	////////////////// the end /////////////////////////
	pArch->WriteString(_T("</usl>\r\n"));

	pArch->Close();
	delete pArch;

	pFile->SetLength(pFile->GetPosition());

	return TRUE;
}




void CData::FillForLineIncrement(int nLine, int nIncrement)
{
	int nTS,nn,gg,tt,ll;

	if(nIncrement<=1) return;
	if((nLine+nIncrement) >= m_nNumberLines) return;
	int nOffset = nLine * m_nSamplesLine;
	int nSize = m_nSamplesLine * (nIncrement - 1);


	for(ll=1;ll<nIncrement;ll++) {
		gg=tt=0;
		for(nTS=0;nTS<m_nNumberTimeSlots;nTS++) {
			for(nn=0;nn<m_nNumberAmpImages[nTS];nn++) {
				CopyMemory(	&m_cAmpBuffer[gg][nOffset + (m_nSamplesLine * ll)] ,&m_cAmpBuffer[gg][nOffset] , nSize);
				gg++;
			}
			for(nn=0;nn<m_nNumberTimeImages[nTS];nn++) {
				CopyMemory(	&m_nTimeBuffer[tt][nOffset + (m_nSamplesLine * ll)] , &m_nTimeBuffer[tt][nOffset] , nSize * sizeof (int));
				tt++;
			}
		}
	}
}

bool CData::MESRetrieveConsumable(CString FileName)
{
	/*
	CUSLFile File;
	CFileException e;
	CString PathName = theApp.m_MesConsumablePath + "\\" + FileName + ".xml";
	CString Buff,Language;
	char *pcArray;
	int ii,nStart,nFinish;

	for(ii=0;ii<100;ii++) {
		m_Details[ii].Empty();
	}


	if(!File.Open(PathName,CFile::modeRead|CFile::typeBinary)) {
		File.DecodeFileException(&e,&Buff,PathName);
		Language.LoadString(IDS_Error);
		if(MessageBox(NULL,_T("There is no MES consumable data for this part\nSeek supervisors permission to continue"),
			_T("MES failure"),MB_ICONERROR|MB_YESNO) == IDYES) {

			return true;
		}
		return false;
	} else {
		int nFileLength = File.GetLength();
		pcArray = new char[nFileLength+2];
		ZeroMemory(pcArray,nFileLength+2);
		File.Read(pcArray,nFileLength);
		File.Close();

		Buff.GetBufferSetLength(nFileLength);
		for(ii=0;ii<nFileLength;ii++) {
			Buff.SetAt(ii,pcArray[ii]);
		}
		delete pcArray;

		if(((nStart=Buff.Find(_T("<Batch ID>")))>0) && ((nFinish=Buff.Find(_T("</Batch ID>")))>0)) {
			nStart += 10;
			m_Details[MES_BATCH_ID] = Buff.Mid(nStart,nFinish - nStart);
		}
		if(((nStart=Buff.Find(_T("<Blade Serial Number>")))>0) && ((nFinish=Buff.Find(_T("</Blade Serial Number>")))>0)) {
			nStart += 21;
			m_Details[MES_BLADE_SERIAL_NUMBER] = Buff.Mid(nStart,nFinish - nStart);
		}
		if(((nStart=Buff.Find(_T("<Part Number>")))>0) && ((nFinish=Buff.Find(_T("</Part Number>")))>0)) {
			nStart += 13;
			m_Details[MES_PART_NUMBER] = Buff.Mid(nStart,nFinish - nStart);
		}
		if(((nStart=Buff.Find(_T("<Blade Type>")))>0) && ((nFinish=Buff.Find(_T("</Blade Type>")))>0)) {
			nStart += 12;
			m_Details[MES_BLADE_TYPE] = Buff.Mid(nStart,nFinish - nStart);
		}

	}
	*/
	return true;
}

void CData::MESSaveOEM()
{
	/*
	CUSLFile File;
	CFileException e;
	CString PathName = theApp.m_MesOEMPath + "\\" + m_FileName + ".xml";
	CString Buff,Language;


	if(!File.Open(PathName,CFile::modeCreate|CFile::typeBinary|CFile::modeWrite,&e)) {
		File.DecodeFileException(&e,&Buff,PathName);
		Language.LoadString(IDS_Error);
	} else {

		Buff.Format(_T("<?xml version=\x022%.01f\x022>\r\n"),1.0f);							XMLWriteMES(&File,Buff);
		XMLWriteMES(&File,_T("<all info>\r\n"));
			Buff.Format(_T("\t<Batch ID>%s</Batch ID>\r\n"),m_FileName);					XMLWriteMES(&File,Buff);
			Buff.Format(_T("\t<Status>Finished</Status>\r\n"));								XMLWriteMES(&File,Buff);
			Buff.Format(_T("\t<Date Time>%s</Date Time>\r\n"),m_TestDateTime);				XMLWriteMES(&File,Buff);
		XMLWriteMES(&File,_T("</all info>\r\n"));
		XMLWriteMES(&File,_T("</xml>\r\n"));

		File.Close();

	}
	*/
}

void CData::XMLWriteMES(CUSLFile *pFile,CString Buff)
{
	char str[256];
	int ii;

	for(ii=0;ii<Buff.GetLength();ii++) {
		str[ii] = (char)Buff.GetAt(ii);
	}
	str[ii]=0;
	pFile->Write(str,strlen(str));

}


void CData::RenderFilterImage(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer,char* cColor,int nLineDirection /*=HORIZONTAL*/)
{

	switch(nLineDirection) {
	case 0: RenderFilterImageHorizontal(pDC,rr,prrSamples,nImageNumber,pScreenBuffer,cColor);
		break;
	case 1:
		break;
	}
}

void CData::RenderFilterImageHorizontal(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer,char* cColor)
{
	char* pBuffer;
	int	yy,xx,nOffset,nVFilter,xxCount;
	int	xs0=prrSamples->left;
	int	xs1=prrSamples->right;
	int	ys0=prrSamples->top;
	int ys1=prrSamples->bottom;
	int	nLine,s0,s1,c[2],color,ss,ff;
	int src,source[2];
	int nBand,nProbe,nn;
	int* pnMultipleProbeOffset = new int[m_nNumberLines];
	ZeroMemory(pnMultipleProbeOffset,m_nNumberLines * sizeof(int));

	int nBytesPixel = pDC->GetDeviceCaps(BITSPIXEL) / 8;

	if(m_nMinViewLine<0) m_nMinViewLine=0;
	if(m_nMinViewLine>=m_nNumberLines) m_nMinViewLine=m_nNumberLines-1;
	if(m_nMaxViewLine<0) m_nMaxViewLine=0;
	if(m_nMaxViewLine>=m_nNumberLines) m_nMaxViewLine=m_nNumberLines-1;

	int nXs1=prrSamples->right;
	int nXs0=prrSamples->left;

	int nWidth = rr.Width()+2;
	int* m_nXpixelOdd = new int[rr.Width()+10];
	int* m_nXpixelEven = new int[rr.Width()+10];
	for(xx=0;xx<=(rr.Width()+1);xx++) {
		m_nXpixelOdd[xx]=MulDiv(xx,(nXs1-nXs0),rr.Width())+nXs0;
		m_nXpixelEven[xx]=MulDiv(xx,(nXs1-nXs0),rr.Width())+nXs0;
	};
	CPolyCoord lineEdge;
	CCoord Cp;
	for(int ii=0;ii<8;ii++) {
		Cp.Side0.fX = (float)((ii*nWidth)/7);
		Cp.Side0.fY = (float)theApp.m_LastSettings.nImageEdge[ii];
		lineEdge.Add(Cp);
	}
	int nEdge;
	for(xx=1;xx<nWidth;xx++) {
		lineEdge.CalculateCoord((float)xx,Cp);
		nEdge=(int)Cp.Side0.fY;
		m_nXpixelEven[xx] += nEdge;
		if(nWidth>m_nSamplesLine) {
			if((m_nXpixelEven[xx]<m_nXpixelEven[xx-1])) {
				m_nXpixelEven[xx]=m_nXpixelEven[xx-1];
			}
		}
	}
	if(m_nNumberProbesPerImage>1) {
		for(nLine=0;nLine<m_nNumberLines;) {
			for(nBand=0,nProbe=0;nBand<4;nBand++) {
				for(nn=0;(nn<32) && (nLine<m_nNumberLines) && (nProbe<m_nNumberProbesPerImage);nn++,nLine++,nProbe++) {
					pnMultipleProbeOffset[nLine] = MulDiv(nn,theApp.m_LastSettings.nPAEdgeOffset[nBand+1],31);
					if((nLine / m_nNumberProbesPerImage) & 1) {
						pnMultipleProbeOffset[nLine] += theApp.m_LastSettings.nPAEdgeDir;
					}
				}
			}
		}
	}

	unsigned char* pData0 = m_cFilterBuffer[nImageNumber];
	int nArrayLength=m_nSamplesLine * m_nNumberLines;

	for(yy=0;yy<rr.Height();yy++) {
		if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
			nOffset=yy*rr.Width();
		} else {
			nOffset=(rr.Height()-1-yy)*rr.Width();
		};
		nLine=MulDiv(yy,ys1-ys0,rr.Height())+ys0;
		nVFilter=1;
		switch(theApp.m_LastSettings.nDisplayWhichLines) {
		case 0:
			break;
		case 1:nLine = nLine & 0xfffe;
			break;
		case 2:nLine = (nLine & 0xfffe) + 1;
			if(nLine>=m_nNumberLines) nLine = m_nNumberLines-1;
			break;
		case 3: nVFilter=2;
			break;
		}
		if((nLine>=m_nMinViewLine) && (nLine<m_nMaxViewLine)) {
			source[0] = nLine * m_nSamplesLine;
			source[1] = (nLine + 1) * m_nSamplesLine;
			if(source[1]>nArrayLength) source[1]=source[0];

			pBuffer=&pScreenBuffer[nOffset * nBytesPixel];
			for(xxCount=0;xxCount<=rr.Width();xxCount++) {
				
				if((theApp.m_LastSettings.nImageOrientation==2) || (theApp.m_LastSettings.nImageOrientation==3)){
					xx=(rr.Width()-xxCount);
				}else{
					xx=xxCount;
				}

				if((nLine&1) && (m_nNumberProbesPerImage<=1)){
					s0=m_nXpixelEven[xx];
					s1=m_nXpixelEven[xx+1];
				} else {
					s0=m_nXpixelOdd[xx];
					s1=m_nXpixelOdd[xx+1];
				}
				s0 += pnMultipleProbeOffset[nLine];
				s1 += pnMultipleProbeOffset[nLine];

				(theApp.m_LastSettings.nAmpPriority==1)?(c[0]=0):(c[0]=0xff);
				c[1]=c[0];
				for(ff=0;ff<nVFilter;ff++) {
					src=source[ff];
					if((s1-s0)>0) {
						for(ss=s0;ss<s1;ss++) {
							if((ss+src)<nArrayLength) {
								if(theApp.m_LastSettings.nAmpPriority==1) {
									if(pData0[ss+src]>c[ff]) c[ff]=pData0[ss+src];
								} else {
									if(pData0[ss+src]<c[ff]) c[ff]=pData0[ss+src];
								}
							} else {
								c[ff]=128;
							}
						};
					} else {
						if((s0+src)<nArrayLength) {
							c[ff]=pData0[s0+src];
						} else {
							c[ff]=128;
						};
					};
				}
				if(nVFilter==2) {
					color = ((c[0]+c[1])/2) & 0xff;
				} else {
					color = c[0]& 0xff;
				}
				color*=4;
				*pBuffer++=cColor[color];
				*pBuffer++=cColor[color+1];
				*pBuffer++=cColor[color+2];
				if(nBytesPixel>3) pBuffer++;
			};
		};
	};
	delete pnMultipleProbeOffset;
	delete m_nXpixelOdd;
	delete m_nXpixelEven;

}




void CData::DifferentiateAmpBuffer(int nSrcType, int nSrc,int nDest)
{
	int nLine,nSample;
	int	nLineOffset;

	unsigned char* pSrc = NULL;

	switch(nSrcType) {
	case AMP_IMAGE: pSrc = m_cAmpBuffer[nSrc];
		break;
	case 1: pSrc = m_cFilterBuffer[nSrc];
		break;
	}
	if(pSrc == NULL) return;

	if(m_cFilterBuffer[nDest] == NULL) {
		int	nSize = m_nSamplesLine * (m_nNumberLines + 1);	//one extra line for extra memory for the line shifting
		m_cFilterBuffer[nDest] = new unsigned char[nSize];
	}
	unsigned char* pDest = m_cFilterBuffer[nDest];

	unsigned char cThreshold = (theApp.m_LastSettings.nDifferentiatorThreshold * 127) / 100 + 128;

	for(nLine=0;nLine<m_nNumberLines;nLine++) {
		nLineOffset = nLine * m_nSamplesLine;
		for(nSample=0;nSample<(m_nSamplesLine+3);nSample++) {
			if(pSrc[nLineOffset + nSample] > cThreshold) {
				pDest[nLineOffset + nSample] = pSrc[nLineOffset + nSample];
			} else {
				if(abs(pSrc[nLineOffset + nSample] -  pSrc[nLineOffset + nSample + 1]) > 3) {
					pDest[nLineOffset + nSample] = 128;
				} else {
					pDest[nLineOffset + nSample] = pSrc[nLineOffset + nSample];
				}
			}

		}

	}

}

void CData::SobelEdgeFilter(int nSrcType,int nSrc, int nDest)
{
	int nH[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
	int nV[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
	int	nLine,nSample,xx,yy,nLineOffset;
	int nRows = 3;
	int	nColumns = 3;
	unsigned char cB[9];

	unsigned char* pSrc = NULL;

	switch(nSrcType) {
	case AMP_IMAGE: pSrc = m_cAmpBuffer[nSrc];
		break;
	case 1: pSrc = m_cFilterBuffer[nSrc];
		break;
	}
	if(pSrc == NULL) return;

	if(m_cFilterBuffer[nDest] == NULL) {
		int	nSize = m_nSamplesLine * (m_nNumberLines + 1);	//one extra line for extra memory for the line shifting
		m_cFilterBuffer[nDest] = new unsigned char[nSize];
	}
	unsigned char* pDest = m_cFilterBuffer[nDest];

	int nSumX,nSumY;

	for(nLine=0;nLine<m_nNumberLines-9;nLine++) {
		nLineOffset = nLine * m_nSamplesLine;
		for(nSample=0;nSample<m_nSamplesLine-9;nSample++) {
			nSumY=nSumX=0;

			for(yy=0;yy<nRows;yy++) {
				CopyMemory(cB,&pSrc[nLineOffset + yy*m_nSamplesLine + nSample],4);
				for(xx=0;xx<nColumns;xx++) {
					nSumX += (cB[xx] * nH[yy][xx]);
					nSumY += (cB[xx] * nV[yy][xx]);
				}
			}
			pDest[nLineOffset + nSample] = MinMax(abs(nSumX) + abs(nSumY),128,255);
		}
	}

}




void CData::FilterAmpBufferWithConvolutionFilter(int nSrcType,int nSrc, int nDest)
{
	m_nConvolutionImageType = nSrcType;
	m_nConvolutionSrc = nSrc;
	m_nConvolutionDest = nDest;

	m_bThreadEnabled=TRUE;
	m_bThreadFinished = false;
	m_pThread=AfxBeginThread(&ConvolutionThread,this,THREAD_PRIORITY_NORMAL ,0,NULL);
}

UINT ConvolutionThread(LPVOID pParam)
{
	CData* pData = (CData *)pParam;
	pData->m_bThreadFinished=FALSE;

	int nK[5][5];
	unsigned char cB[8];
	int nRows,nColumns;
	int nLine,nSample;
	int	nSum,nLineOffset;
	int	xx,yy;

	unsigned char* pSrc = NULL;

	switch(pData->m_nConvolutionImageType) {
	case AMP_IMAGE: pSrc = pData->m_cAmpBuffer[pData->m_nConvolutionSrc];
		break;
	case 1: pSrc = pData->m_cFilterBuffer[pData->m_nConvolutionSrc];
		break;
	}
	if(pSrc == NULL) return 0;

	if(pData->m_cFilterBuffer[pData->m_nConvolutionDest] == NULL) {
		int	nSize = pData->m_nSamplesLine * (pData->m_nNumberLines + 1);	//one extra line for extra memory for the line shifting
		pData->m_cFilterBuffer[pData->m_nConvolutionDest] = new unsigned char[nSize];
	}
	unsigned char* pDest = pData->m_cFilterBuffer[pData->m_nConvolutionDest];


	CopyMemory(nK,theApp.m_nFilter,sizeof nK);
	nRows=0;
	nColumns=0;
	int nDenominator=0;
	for(nLine=0;nLine<5;nLine++) {
		for(nSample=0;nSample<5;nSample++) {
			nDenominator += nK[nLine][nSample];
			if(nK[nLine][nSample]) {
				if(nLine>nRows) nRows = nLine;
				if(nSample>nColumns) nColumns = nSample;
			}
		}
	}
	if(nDenominator==0) nDenominator = 1;
	nRows++;
	nColumns++;

	for(nLine=0;nLine<pData->m_nNumberLines-5;nLine++) {
		nLineOffset = nLine * pData->m_nSamplesLine;
		for(nSample=0;nSample<pData->m_nSamplesLine-5;nSample++) {
			nSum=0;

			for(yy=0;yy<nRows;yy++) {
				CopyMemory(cB,&pSrc[nLineOffset + yy*pData->m_nSamplesLine + nSample],8);
				for(xx=0;xx<nColumns;xx++) {
					nSum += (cB[xx] * nK[yy][xx]);
				}
			}
			pDest[nLineOffset + nSample] = pData->MinMax(nSum/nDenominator,128,255);
		}
	}
	pData->m_bThreadEnabled=false;
	pData->m_bThreadFinished = true;


	return 0;
}





inline int CData::MinMax(int pn, int nMin, int nMax)
{
	if(pn<nMin) return nMin;
	if(pn>nMax) return nMax;
	return pn;
}


bool CData::RetrieveCIPFile(CUSLFile *pFile) //CIP - Combined Image Position
{
	return false;

}

bool CData::SaveCIPFile(CUSLFile *pFile)
{
	CString Buff;
	CString Identifier = L"Ultrasonic Sciences Ltd USLScanner Combined Image Position Data\r\n";
	int nLine,nAmp,nTime,nSample,ii;
	float fPos, fTemp;
	CPoint pt;
	CCoord	CpSurface;
	float fSize = m_fFastScanSize;
	int nTimeImages=0,nAmpImages=0,nTemp;
	char cTemp;
	CProfile LocalProfile;
	CProfile *pLocalProfile;
	CString ProfilePath;
	CUSLFile File;
	CFileException e;

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

	for(ii=0;ii<m_nNumberTimeSlots;ii++) {
		nAmpImages+=m_nNumberAmpImages[ii];
		nTimeImages+=m_nNumberTimeImages[ii];
	}

	//check Profile correct
	if(m_ProfileName == PROFILE->m_FileName) {
		pLocalProfile = &theApp.m_Profile;
	}else{
		//Not correct profile attempt to load correct profile
		ProfilePath = theApp.m_ProfilePath + _T("\\") + m_ProfileName;
		if(!File.Open(ProfilePath,CFile::modeCreate | CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,ProfilePath);
			MessageBox(NULL,Buff,_T("Error Loading Correct Profile"),MB_ICONERROR);
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			return false;
		} else {
			LocalProfile.Retrieve(&File);
			pLocalProfile = &LocalProfile;
			File.Close();
		}
	}


	m_CIPFileName = pFile->GetFileName();
	m_CIPFilePath = pFile->GetFilePath();

	
	pFile->Seek(0,CUSLFile::begin);
	pFile->WriteUnicodeStringAsAscii(&Identifier);

	Buff.Format(_T("\nDatFilename=\"%s\"\r\n"),m_FileName);
	pFile->WriteUnicodeStringAsAscii(&Buff);

	Buff.Format(_T("ProfileName=\"%s\"\r\n"),m_ProfileName);
	pFile->WriteUnicodeStringAsAscii(&Buff);
	
	Buff.Format(_T("NumberOfLines=\"%d\"\r\n"),m_nNumberLines);
	pFile->WriteUnicodeStringAsAscii(&Buff);

	Buff.Format(_T("FastIncrement=\"%fmm\"\r\n"),m_fFastIncrement);
	pFile->WriteUnicodeStringAsAscii(&Buff);

	Buff.Format(_T("SlowIncrement=\"%fmm\"\r\n"),m_fSlowIncrement);
	pFile->WriteUnicodeStringAsAscii(&Buff);

	Buff.Format(_T("NumberOfAmplitudeImages=\"%d\"\r\n"),nAmpImages);
	pFile->WriteUnicodeStringAsAscii(&Buff);

	Buff.Format(_T("NumberOfTimeImages=\"%d\"\r\n"),nTimeImages);
	pFile->WriteUnicodeStringAsAscii(&Buff);

	Buff.Format(_T("UnitsOfTimeImage=\"ns\"\r\n"));
	pFile->WriteUnicodeStringAsAscii(&Buff);

	Buff.Format(_T("FormatOfPositionData=\"6 x 4 byte floats (XYZIJK)\"\r\n"));
	pFile->WriteUnicodeStringAsAscii(&Buff);

	Buff.Format(_T("FormatOfAmpData=\"Signed 1 byte integer\"\r\n"));
	pFile->WriteUnicodeStringAsAscii(&Buff);

	Buff.Format(_T("FormatOfTimeData=\"Signed 4 byte integer\"\r\n"));
	pFile->WriteUnicodeStringAsAscii(&Buff);

	Buff.Format(_T("ItemDataFormat=\"X(4F) Y(4F) Z(4F) I(4F) J(4F) K(4F)"));
	for(ii=0;ii<nAmpImages;ii++) {
		Buff+=_T(" A(1I)");
	}
	for(ii=0;ii<nTimeImages;ii++) {
		Buff+=_T(" T(4I)");
	}
	Buff+=_T("\"\r\n");
	pFile->WriteUnicodeStringAsAscii(&Buff);

	Buff.Format(_T("TotalBytesPerItem=\"%d\"\r\n"),(24+nAmpImages+(nTimeImages*4)));
	pFile->WriteUnicodeStringAsAscii(&Buff);

	Buff.Format(_T("\r\nAt Offset 0x1000 in this file the Fast and Slow increments and number of Amp and Time images can be found as 4Bytes Float,Float,Signed Int, Signed Int"));
	pFile->WriteUnicodeStringAsAscii(&Buff);

	Buff.Format(_T("\r\nAt Offset 0x1100 in this file the total length of each data item in bytes and how these bytes split into seperate numbers in signed 4 byte Int"));
	pFile->WriteUnicodeStringAsAscii(&Buff);

	Buff.Format(_T("\r\nAt Offset 0x1200 in this file the data Starts"));
	pFile->WriteUnicodeStringAsAscii(&Buff);

	ULONGLONG gg = pFile->GetPosition();

	pFile->Seek(0x1000,CUSLFile::begin);
	fTemp = m_fFastIncrement;
	pFile->Write(&fTemp,sizeof(fTemp));
	fTemp = m_fSlowIncrement;
	pFile->Write(&fTemp,sizeof(fTemp));
	nTemp = nAmpImages;
	pFile->Write(&fTemp,sizeof(nTemp));
	nTemp = nTimeImages;
	pFile->Write(&fTemp,sizeof(nTemp));


	pFile->Seek(0x1100,CUSLFile::begin);
	nTemp = 24+nAmpImages+(nTimeImages*4);
	pFile->Write(&nTemp,sizeof(nTemp));
	nTemp=4;
	for(ii=0;ii<6;ii++){
		pFile->Write(&nTemp,sizeof(nTemp));
	}
	nTemp=1;
	for(ii=0;ii<nAmpImages;ii++) {
		pFile->Write(&nTemp,sizeof(nTemp));
	}
	nTemp=4;
	for(ii=0;ii<nTimeImages;ii++) {
		pFile->Write(&nTemp,sizeof(nTemp));
	}


	


	pFile->Seek(0x1200,CUSLFile::begin);
	fSize = m_CpFinish.Side0.fPos[m_nFastAxis] - m_CpStart.Side0.fPos[m_nFastAxis];
	if(m_nFastAxis==5){
		if(fSize==0) fSize=360;
	}


	for(nLine=0;nLine<m_nNumberLines;nLine++) { //line increment???

		for(nSample=0;nSample<m_nSamplesLine;nSample++) {
			
			pt.x=nSample;
			pt.y=nLine+m_nScanStartLine;
			
			fPos = ((float)pt.x * fSize) / (float)(m_nSamplesLine-1) + m_CpStart.Side0.fPos[m_nFastAxis];
		
			
			if(pLocalProfile->GetSurfaceCoordinate(CpSurface,pt,fPos,m_nFastAxis) == TRUE) {
				pFile->Write(&CpSurface.Side0.fX,sizeof(float));
				pFile->Write(&CpSurface.Side0.fY,sizeof(float));
				pFile->Write(&CpSurface.Side0.fZ,sizeof(float));
				pFile->Write(&CpSurface.Side0.fI,sizeof(float));
				pFile->Write(&CpSurface.Side0.fJ,sizeof(float));
				pFile->Write(&CpSurface.Side0.fK,sizeof(float));
				for(nAmp=0;nAmp<nAmpImages;nAmp++) {
					cTemp = GetFilteredAmpChar(nSample,nLine,nAmp);
					pFile->Write(&cTemp,sizeof(char));
				}
				for(nTime=0;nTime<nTimeImages;nTime++) {
					nTemp = GetFilteredTimeThickness(nSample,nLine,nTime,0);
					pFile->Write(&nTemp,sizeof(int));
				} //for
				Buff = _T("\r\n");
				pFile->WriteUnicodeStringAsAscii(&Buff);
			}//if
			
			/* //Write as strings
			if(pLocalProfile->GetSurfaceCoordinate(CpSurface,pt,fPos,m_nFastAxis) == TRUE) {
				Buff.Format(_T("%.1f,L"),CpSurface.Side0.fX);
				pFile->WriteUnicodeStringAsAscii(&Buff);
				Buff.Format(_T("%.1f,L"),CpSurface.Side0.fY);
				pFile->WriteUnicodeStringAsAscii(&Buff);
				Buff.Format(_T("%.1f,L"),CpSurface.Side0.fZ);
				pFile->WriteUnicodeStringAsAscii(&Buff);
				Buff.Format(_T("%.1f,L"),CpSurface.Side0.fI);
				pFile->WriteUnicodeStringAsAscii(&Buff);
				Buff.Format(_T("%.1f,L"),CpSurface.Side0.fJ);
				pFile->WriteUnicodeStringAsAscii(&Buff);
				Buff.Format(_T("%.1f,L"),CpSurface.Side0.fK);
				pFile->WriteUnicodeStringAsAscii(&Buff);
				for(nAmp=0;nAmp<nAmpImages;nAmp++) {
					cTemp = GetFilteredAmpChar(nSample,nLine,nAmp);
					Buff.Format(_T("%c,L"),cTemp);
					pFile->WriteUnicodeStringAsAscii(&Buff);
				}
				for(nTime=0;nTime<nTimeImages;nTime++) {
					nTemp = GetFilteredTimeThickness(nSample,nLine,nTime,0);
					Buff.Format(_T("%d,L"),nTemp);
					pFile->WriteUnicodeStringAsAscii(&Buff);
				} //for
				Buff = _T("\n");
				pFile->WriteUnicodeStringAsAscii(&Buff);
			}//if
			*/



		} //for sample
	} //for line

	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	return true;
}

//
//bend the linear Amp buffer in to an arc 
//
double CData::GenerateArcAmpBufferNieve(int nImageNumber,ConeData *pCone,DonutData *pDonut,float fGratStartAngle)
{
	float fAngle,fHeight,fWidth,fRadius;
	int nSize = pDonut->nArcBufferWidth * pDonut->nArcBufferHeight;
	int nSample,nLine,nDestPtr,nSrcPtr;
	int nS[2];
	int nSrcSize = m_nSamplesLine * m_nNumberLines;

	SAFE_DELETE( pDonut->cArcAmpBuffer );
	pDonut->cArcAmpBuffer = new unsigned char[ nSize ];
	ZeroMemory(pDonut->cArcAmpBuffer, nSize);

	if((pDonut->fSector[0] == 0.0f) && (pDonut->fSector[1] == 360.0f)) {
		nS[0] = (int)(pDonut->fSector[0] * (float)m_nSamplesLine / 360.0f);
		nS[1] = (int)(pDonut->fSector[1] * (float)m_nSamplesLine / 360.0f);
	} else {
		nS[0] = (int)((pDonut->fSector[0] - fGratStartAngle) * (float)m_nSamplesLine / 360.0f);
		nS[1] = (int)((pDonut->fSector[1] - fGratStartAngle) * (float)m_nSamplesLine / 360.0f);
	}

	unsigned char *pSrcBuffer = m_cAmpBuffer[nImageNumber];

	int		nHalfWidth = pDonut->nArcBufferWidth / 2;
	float	fHalfPaperWidth = pDonut->fPaperWidth / 2.0f;


	m_HR.StartTimer();
	for(int nY=0;nY<pDonut->nArcBufferHeight;nY++) {
		fHeight = (pDonut->fR[CONE_OUTER]-pDonut->fPaperHeight) + ((float)nY * pDonut->fPaperHeight / (float)(pDonut->nArcBufferHeight-1));

		for(int nX=0;nX<pDonut->nArcBufferWidth;nX++) {
			fWidth = (float)(nX-nHalfWidth) * fHalfPaperWidth / (float)nHalfWidth;
			fAngle = atanf(fWidth / fHeight);

			fRadius = sqrtf(fWidth * fWidth + fHeight * fHeight);

			if((fAngle>=-pDonut->fTriangleAngle) && (fAngle<=pDonut->fTriangleAngle) && (fRadius >= pDonut->fR[CONE_INNER]) && (fRadius<=pDonut->fR[CONE_OUTER])) {

				nSample = (int)((float)(nS[1]-nS[0]) * (pDonut->fTriangleAngle+fAngle) / pDonut->fArcAngle) + nS[0];
				nLine = (int)((float)m_nNumberLines  * (fRadius - pDonut->fR[CONE_INNER]) / (pDonut->fR[CONE_OUTER] - pDonut->fR[CONE_INNER]));

				nDestPtr = nX + nY * pDonut->nArcBufferWidth;
				nSrcPtr = nSample + nLine * m_nSamplesLine;

				if((nSrcPtr>=0) && (nSrcPtr<nSrcSize)) {
					pDonut->cArcAmpBuffer[nDestPtr] = pSrcBuffer[nSrcPtr];
				}
			}
		}
	}
	return m_HR.GetTime();
}


double CData::GenerateArcAmpBuffer(int nImageNumber,ConeData *pCone,DonutData *pDonut,float fGratStartAngle)
{
	float fAngle,fHeight,fWidth,fRadius,fRadiusSqr;
	int nSize = pDonut->nArcBufferWidth * pDonut->nArcBufferHeight;
	int nSample,nLine,nDestPtr[2],nSrcPtr;
	int nS[2];
	int nSrcSize = m_nSamplesLine * m_nNumberLines;

	SAFE_DELETE( pDonut->cArcAmpBuffer );
	pDonut->cArcAmpBuffer = new unsigned char[ nSize ];
	ZeroMemory(pDonut->cArcAmpBuffer, nSize);

	SAFE_DELETE( pDonut->nSrcPtrArray );
	pDonut->nSrcPtrArray = new int [nSize];
	ZeroMemory(pDonut->nSrcPtrArray, nSize * sizeof (int));

	if((pDonut->fSector[0] == 0.0f) && (pDonut->fSector[1] == 360.0f)) {
		nS[0] = (int)(pDonut->fSector[0] * (float)m_nSamplesLine / 360.0f);
		nS[1] = (int)(pDonut->fSector[1] * (float)m_nSamplesLine / 360.0f);
	} else {
		nS[0] = (int)((pDonut->fSector[0] - fGratStartAngle) * (float)m_nSamplesLine / 360.0f);
		nS[1] = (int)((pDonut->fSector[1] - fGratStartAngle) * (float)m_nSamplesLine / 360.0f);
	}

	unsigned char *pSrcBuffer = m_cAmpBuffer[nImageNumber];

	int		nHalfWidth = pDonut->nArcBufferWidth / 2;
	float	fHalfPaperWidth = pDonut->fPaperWidth / 2.0f;

	/////////////////////////////////////
	pDonut->fRSqr[0] = pDonut->fR[0] * pDonut->fR[0];
	pDonut->fRSqr[1] = pDonut->fR[1] * pDonut->fR[1];
	float fNumberSamples = (float)(nS[1]-nS[0]);
	float fDonutThickness = pDonut->fR[CONE_OUTER] - pDonut->fR[CONE_INNER];

	m_HR.StartTimer();
	for(int nY=0;nY<pDonut->nArcBufferHeight;nY++) {
		fHeight = (pDonut->fR[CONE_OUTER]-pDonut->fPaperHeight) + ((float)nY * pDonut->fPaperHeight / (float)(pDonut->nArcBufferHeight-1));
		int nDestLineStartOffset = nY * pDonut->nArcBufferWidth;
		nDestPtr[0] = nDestLineStartOffset;
		nDestPtr[1] = nDestLineStartOffset + pDonut->nArcBufferWidth - 1;

		for(int nX=0;nX<((pDonut->nArcBufferWidth/2)+1);nX++,nDestPtr[0]++,nDestPtr[1]--) {
			fWidth = (float)(nX-nHalfWidth) * fHalfPaperWidth / (float)nHalfWidth;
			fAngle = atanf(fWidth / fHeight);

			fRadiusSqr = fWidth * fWidth + fHeight * fHeight;

			if((fAngle>=-pDonut->fTriangleAngle) && (fAngle<=pDonut->fTriangleAngle) && (fRadiusSqr >= pDonut->fRSqr[CONE_INNER]) && (fRadiusSqr<=pDonut->fRSqr[CONE_OUTER])) {

				fRadius = sqrtf(fRadiusSqr);
				nSample = (int)(fNumberSamples * (pDonut->fTriangleAngle+fAngle) / pDonut->fArcAngle);
				nLine = (int)((float)m_nNumberLines  * (fRadius - pDonut->fR[CONE_INNER]) / fDonutThickness);

				nSrcPtr = nSample + (nLine * m_nSamplesLine);
				if((nSrcPtr>0) && (nSrcPtr<nSrcSize)) {
					pDonut->cArcAmpBuffer[nDestPtr[0]] = pSrcBuffer[nSrcPtr];
					pDonut->nSrcPtrArray[nDestPtr[0]] = nSrcPtr;
				}

				nSrcPtr = (nS[1]-nS[0]) - nSample - 1 + (nLine * m_nSamplesLine);
				if((nSrcPtr>0) && (nSrcPtr<nSrcSize)) {
					pDonut->cArcAmpBuffer[nDestPtr[1]] = pSrcBuffer[nSrcPtr];
					pDonut->nSrcPtrArray[nDestPtr[1]] = nSrcPtr;
				}
			}
		}
	}
	return m_HR.GetTime();
}



void CData::AddPolygonPt(int *nPolyPtr, CCoord Cp)
{
	if((m_pPolygon == NULL) || (*nPolyPtr>= m_nPolygonL)) {
		AddPolygon(nPolyPtr);
	}
	m_pPolygon[*nPolyPtr].AddIfNotAlreadyHere(Cp);

}

void CData::AddPolygon(int *nPolyPtr)
{
	int ii;
	CPolyCoord *pLine;
	int nn=m_nPolygonL;
	m_nPolygonL++;

	if(m_nPolygonL==1) {
		m_pPolygon = new CPolyCoord[m_nPolygonL];
	} else {
		pLine = new CPolyCoord[nn];
		for(ii=0;ii<nn;ii++) pLine[ii] = &m_pPolygon[ii];
		SAFE_DELETE_ARRAY( m_pPolygon );
		m_pPolygon = new CPolyCoord[m_nPolygonL];
		for(ii=0;ii<nn;ii++) m_pPolygon[ii] = &pLine[ii];
		SAFE_DELETE_ARRAY( pLine );
	};
	*nPolyPtr = m_nPolygonL-1;

	SetPloygonLengthAreaMode(*nPolyPtr, theApp.m_LastSettings.nPloygonLengthAreaMode);

}

void CData::ClearPolygon(int nIndex)
{
	if((nIndex < m_nPolygonL) && (m_pPolygon != NULL)) {
		m_pPolygon[nIndex].Zero();
	}
}

void CData::ClearAllPolygons()
{

	SAFE_DELETE_ARRAY( m_pPolygon);
	m_nPolygonL = 0;
}

void CData::SetPloygonLengthAreaMode(int nWhich, int nMode)
{
	if((m_pPolygon != NULL) && (nWhich>=0) && (nWhich<m_nPolygonL)) {
		m_pPolygon[nWhich].m_nClosedEnds = nMode;
		m_pPolygon[nWhich].m_nPrimaryAxis = 30;
	}
}


void CData::GenerateSamplePositionLine(int nLine,CPolyCoord *pLine)
{
	CCoord Cp,Cpc,Cp0,Cp1;
	float fPos;
	D3DXVECTOR3 vecSide0,vecSide1;

	if((pLine->m_nLine - nLine) || (pLine->m_nCoordL<1)){
		pLine->Zero();
		pLine->AllocateCoords(m_nSamplesLine);


		PROFILE->m_ScanLine[nLine].SetPrimaryAxis(30);
		PROFILE->m_ScanLine[nLine].SetStyle(0);
		float fLength = PROFILE->m_ScanLine[nLine].GetLength();

		PROFILE->m_ScanLine[nLine].GetCoord(0,&Cp);
		int nSample = (int)((Cp.fPos[m_nCollectAxis]-m_CpStart.fPos[m_nCollectAxis]) * (float)(m_nSamplesLine-1) / (m_CpFinish.fPos[m_nCollectAxis] - m_CpStart.fPos[m_nCollectAxis]));
		if(nSample<0) nSample = 0;

		int nStartSample = nSample;

		for(fPos=0,nSample;fPos<=fLength;fPos+=m_fFastIncrement,nSample++) {
			if(nSample>=m_nSamplesLine) nSample = m_nSamplesLine-1;
			PROFILE->m_ScanLine[nLine].CalculateCoord(fPos,Cp);
			pLine->ModifyCoord(nSample,Cp);
			if(fPos==0.0f) Cp0 = Cp;
			Cp1 = Cp;
		}
		//Sort out end of line
		PROFILE->m_ScanLine[nLine].GetCoord((pLine->m_nCoordL-1)/2,&Cpc);

		vecSide0 = Cp1.Side0.pt - Cpc.Side0.pt;
		D3DXVec3Normalize(&vecSide0,&vecSide0);
		vecSide0 *= m_fFastIncrement;

		vecSide1 = Cp1.Side1.pt - Cpc.Side1.pt;
		D3DXVec3Normalize(&vecSide1,&vecSide1);
		vecSide1 *= m_fFastIncrement;

		Cp=Cp1;
		for(nSample;nSample<m_nSamplesLine;nSample++) {
			Cp.Side0.pt += vecSide0;
			Cp.Side1.pt += vecSide1;
			pLine->ModifyCoord(nSample,Cp);
		}

		//Sort out start of line
		vecSide0 = Cp0.Side0.pt - Cpc.Side0.pt;
		D3DXVec3Normalize(&vecSide0,&vecSide0);
		vecSide0 *= m_fFastIncrement;

		vecSide1 = Cp0.Side1.pt - Cpc.Side1.pt;
		D3DXVec3Normalize(&vecSide1,&vecSide1);
		vecSide1 *= m_fFastIncrement;

		Cp = Cp0;
		for(nSample=nStartSample-1;nSample>=0;nSample--) {
			Cp.Side0.pt += vecSide0;
			Cp.Side1.pt += vecSide1;
			pLine->ModifyCoord(nSample,Cp);
		}
		pLine->m_nLine = nLine;
	}
}


void CData::ChangeFlattenMode(int nNewFlattenMode)
{
	int nTS,gg,tt,pp,nn,nLine,nOffset,nSamples;
	
	int *nFirstSample = new int[m_nNumberLines];
	for(nLine = 0;nLine<m_nNumberLines;nLine++) {
		nFirstSample[nLine] = m_vLineSample[nLine].m_nFirstSample;
	}

	m_nFlattenMode = nNewFlattenMode;
	GenerateSamplePositionVertices();

	char *pcArray = new char[m_nSamplesLine];
	int *pnArray = new int[m_nSamplesLine];

	gg=tt=pp=0;
	for(nTS=0;nTS<m_nNumberTimeSlots;nTS++) {
		for(nn=0;nn<m_nNumberAmpImages[nTS];nn++) {
			if(m_cAmpBuffer[gg]) {
				for(nLine=0;nLine<m_nNumberLines;nLine++) {
					//Get origional line
					nOffset = nLine * m_nSamplesLine + nFirstSample[nLine];
					nSamples = m_nSamplesLine - nFirstSample[nLine];
					CopyMemory(pcArray,(void*)(&m_cAmpBuffer[gg][nOffset]),nSamples);
					//Fill line with initial value
					FillMemory((void*)(&m_cAmpBuffer[gg][nLine * m_nSamplesLine]),m_nSamplesLine,INIT_VALUE);
					//Replace line in correct position
					nOffset = nLine * m_nSamplesLine + m_vLineSample[nLine].m_nFirstSample;
					CopyMemory((void*)(&m_cAmpBuffer[gg][nOffset]),pcArray,nSamples);
				}
				gg++;
			}	
		};
		for(nn=0;nn<m_nNumberTimeImages[nTS];nn++) {
			if(m_nTimeBuffer[tt]) {

				tt++;
			}
		}
		for(nn=0;nn<m_nNumberPhaseImages[nTS];nn++) {
			if(m_cPhaseBuffer[pp++]) {

				pp++;
			}
		}
	}

	SAFE_DELETE( pnArray );
	SAFE_DELETE( pcArray );
	SAFE_DELETE( nFirstSample );
}


void CData::GenerateSamplePositionVertices()
{
	int nLine,nDestLine,nOldPrimaryAxis,nOldStyle,nSample,nCentreSample;
	CCoord Cp,Cp0,Cp1,Cpc,CpMin,CpMax,CpCentre;
	float fPos,fStartLength,fFinishLength,fLength;
	D3DXVECTOR3 vecSide0,vecSide1;
	bool bCalculateVertexCoords = false;

	if(PROFILE->m_nScanLineL <= 0) {
		MessageBox(NULL,L"Please load the correct profile",L"Error",MB_ICONERROR);
		return;
	}


	CpMin = m_CpStart;
	CpMax = m_CpFinish;

	switch(m_nFlattenMode) {
	case 4:
		nLine = (PROFILE->m_nScanFinishLine + PROFILE->m_nScanStartLine) / 2;
		nOldPrimaryAxis = PROFILE->m_ScanLine[nLine].m_nPrimaryAxis;
		nOldStyle = PROFILE->m_ScanLine[nLine].m_nStyle;

		PROFILE->m_ScanLine[nLine].SetPrimaryAxis(30);
		PROFILE->m_ScanLine[nLine].SetStyle(0);

		fLength = PROFILE->m_ScanLine[nLine].GetLength();
		PROFILE->m_ScanLine[nLine].CalculateCoord(fLength / 2.0f,CpCentre);

		PROFILE->m_ScanLine[nLine].SetPrimaryAxis(nOldPrimaryAxis);
		PROFILE->m_ScanLine[nLine].SetStyle(nOldStyle);

		break;
	}

	nCentreSample = m_nSamplesLine / 2;
	
	if(m_vLineSample == NULL) {
		m_vLineSample = new CVertexArray[ m_nNumberLines ];
		bCalculateVertexCoords = true;
	}

start_again:
	for(nLine=PROFILE->m_nScanStartLine,nDestLine=0;nLine<=PROFILE->m_nScanFinishLine;nLine += PROFILE->m_nLineIncrement,nDestLine++) {

		if(nDestLine>=m_nNumberLines) return;

		nOldPrimaryAxis = PROFILE->m_ScanLine[nLine].m_nPrimaryAxis;
		nOldStyle = PROFILE->m_ScanLine[nLine].m_nStyle;

		PROFILE->m_ScanLine[nLine].SetPrimaryAxis(30);
		PROFILE->m_ScanLine[nLine].SetStyle(0);
		float fLength = PROFILE->m_ScanLine[nLine].GetLength();

		switch(m_nFlattenMode) {
		case 0:
		case 3:
			PROFILE->m_ScanLine[nLine].GetCoord(0,&Cp);
			switch(m_nFastAxis) {
			case 0: fStartLength = fabs(Cp.Side0.fX - CpMin.Side0.fX);
				break;
			case 1: fStartLength = fabs(Cp.Side0.fY - CpMin.Side0.fY);
				break;
			case 2: fStartLength = fabs(Cp.Side0.fZ - CpMin.Side0.fZ);
				break;
			}
			m_vLineSample[nDestLine].m_nFirstSample = (int)(fStartLength / m_fFastIncrement);
			break;
		case 1:
			m_vLineSample[nDestLine].m_nFirstSample = (m_nSamplesLine / 2) - (int)((fLength / 2.0f) / m_fFastIncrement);
			break;
		case 2:
			PROFILE->m_ScanLine[nLine].GetLastCoord(&Cp);
			switch(m_nFastAxis) {
			case 0: fFinishLength = fabs(Cp.Side0.fX - CpMax.Side0.fX);
				break;
			case 1: fFinishLength = fabs(Cp.Side0.fY - CpMax.Side0.fY);
				break;
			case 2: fFinishLength = fabs(Cp.Side0.fZ - CpMax.Side0.fZ);
				break;
			}
			m_vLineSample[nDestLine].m_nFirstSample = m_nSamplesLine - (int)(fFinishLength / m_fFastIncrement) - (int)(fLength / m_fFastIncrement);
			break;
		case 4:
			fPos = PROFILE->m_ScanLine[nLine].HowFarAlongLineIsCoordinate(CpCentre,PROFILE->m_nFastAxis);
			m_vLineSample[nDestLine].m_nFirstSample = nCentreSample - (int)(fPos / m_fFastIncrement);
			if(m_vLineSample[nDestLine].m_nFirstSample < 0) {
				m_vLineSample[nDestLine].m_nFirstSample = 0;
				nCentreSample = (int)(fPos / m_fFastIncrement);
				goto start_again;
			}
			break;
		}
		if(m_vLineSample[nDestLine].m_nFirstSample < 0) m_vLineSample[nDestLine].m_nFirstSample = 0;

		if(bCalculateVertexCoords == true) {	//only calculate these coords the once. Not necessary if just changing flatten mode
			switch(m_nFlattenMode) {
			default:
				m_vLineSample[nDestLine].SetSize((int)(fLength / m_fFastIncrement));
				for(fPos=0,nSample=0;fPos<=fLength;fPos+=m_fFastIncrement,nSample++) {
					if(nSample>=m_vLineSample[nDestLine].m_nCoordL) nSample = m_vLineSample[nDestLine].m_nCoordL-1;
					PROFILE->m_ScanLine[nLine].CalculateCoord(fPos,Cp);
					m_vLineSample[nDestLine].ModifyCoord(nSample,Cp);
				}
				break;
			case 3:
				m_vLineSample[nDestLine].SetSize(m_nSamplesLine);
				PROFILE->m_ScanLine[nLine].GetCoord(0,&Cp0);
				PROFILE->m_ScanLine[nLine].GetLastCoord(&Cp1);
				PROFILE->m_ScanLine[nLine].SetPrimaryAxis(m_nFastAxis);
				for(fPos = Cp0.fPos[m_nFastAxis] ,nSample = 0; fPos<Cp1.fPos[m_nFastAxis]; fPos+=m_fFastIncrement,nSample++) {
					if(nSample>=m_vLineSample[nDestLine].m_nCoordL) nSample = m_vLineSample[nDestLine].m_nCoordL-1;
					PROFILE->m_ScanLine[nLine].CalculateCoord(fPos,Cp);
					m_vLineSample[nDestLine].ModifyCoord(nSample,Cp);
				};
				break;
			}

			if(nSample>0)
				m_vLineSample[nDestLine].SetSize(nSample);
		}

		PROFILE->m_ScanLine[nLine].SetPrimaryAxis(nOldPrimaryAxis);
		PROFILE->m_ScanLine[nLine].SetStyle(nOldStyle);
	}

}

void CData::GenerateRotarySamplePositionVertices()
{
	int nLine, nDestLine, nSample;
	CCoord Cp;

	if (PROFILE->m_nScanLineL <= 0) {
		MessageBox(NULL, L"Please load the correct profile", L"Error", MB_ICONERROR);
		return;
	}

	if (m_vLineSample) return;
	m_vLineSample = new CVertexArray[m_nNumberLines];

	for (nLine = PROFILE->m_nScanStartLine, nDestLine = 0; nLine <= PROFILE->m_nScanFinishLine; nLine += PROFILE->m_nLineIncrement, nDestLine++) {

		if (nDestLine >= m_nNumberLines) return;

		m_vLineSample[nDestLine].m_nFirstSample = 0;

		m_vLineSample[nDestLine].SetSize((int)(360.0f / m_fFastIncrement));
		PROFILE->m_ScanLine[0].GetCoord(nLine, &Cp);
		for (nSample = 0; nSample < m_vLineSample[nDestLine].m_nCoordL; nSample++) {
			m_vLineSample[nDestLine].ModifyCoord(nSample, Cp);
		}
	}

}

bool CData::IsLineSample()
{

	if(m_vLineSample == NULL) return false;
	return true;
}

bool CData::GetCoordFromLineSample(CPoint ptSample, CCoord *pCp)
{

	if(m_vLineSample == NULL) return false;

	if((ptSample.y>=0) && (ptSample.y<m_nNumberLines)) {
		int nPtr = ptSample.x - m_vLineSample[ptSample.y].m_nFirstSample;

		if((nPtr>=0) && (nPtr<m_vLineSample[ptSample.y].m_nCoordL)) {
			pCp->Side0.fX = m_vLineSample[ptSample.y].m_pVp[nPtr].fX;
			pCp->Side0.fY = m_vLineSample[ptSample.y].m_pVp[nPtr].fY;
			pCp->Side0.fZ = m_vLineSample[ptSample.y].m_pVp[nPtr].fZ;
			pCp->Side0.fI = m_vLineSample[ptSample.y].m_pVp[nPtr].fI;
			pCp->Side0.fJ = m_vLineSample[ptSample.y].m_pVp[nPtr].fJ;
			pCp->Side0.fK = m_vLineSample[ptSample.y].m_pVp[nPtr].fK;
			return true;
		}
	}
	return false;
}

int	CData::TempStoreNodesCADElement(float fSample, float fLine)
{
	m_CpCursor.fSample = fSample;
	m_CpCursor.fLine = fLine;

	if(IsCADElement()) {
		for(int ii=0;ii<4;ii++) {
			m_CpNode[ii] = m_pCADElement[m_nCADPtr].m_Nodes.m_pCp[ii];
		}
	}
	return m_nCADPtr;
}

int CData::OffsetCADElement(float fSample, float fLine)
{

	for(int ii=0;ii<4;ii++) {
		m_pCADElement[m_nCADPtr].m_Nodes.m_pCp[ii].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[ii].fSample;
		m_pCADElement[m_nCADPtr].m_Nodes.m_pCp[ii].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[ii].fLine;
	}

	return m_nCADPtr;
}

void CData::MoveCornerOfParallelogram(int nNode, float fSample, float fLine)
{
	CPolyCoord* pNodes = &m_pCADElement[m_nCADPtr].m_Nodes;

	pNodes->m_pCp[nNode].fSample = fSample;
	pNodes->m_pCp[nNode].fLine = fLine;

	switch(nNode) {
	case 0:
		pNodes->m_pCp[1].fSample = pNodes->m_pCp[2].fSample + (pNodes->m_pCp[0].fSample - pNodes->m_pCp[3].fSample);
		pNodes->m_pCp[1].fLine = pNodes->m_pCp[2].fLine + (pNodes->m_pCp[0].fLine - pNodes->m_pCp[3].fLine);
		break;
	case 1:
		pNodes->m_pCp[2].fSample = pNodes->m_pCp[1].fSample + (pNodes->m_pCp[3].fSample - pNodes->m_pCp[0].fSample);
		pNodes->m_pCp[2].fLine = pNodes->m_pCp[1].fLine + (pNodes->m_pCp[3].fLine - pNodes->m_pCp[0].fLine);
		break;
	case 2:
		pNodes->m_pCp[3].fSample = pNodes->m_pCp[0].fSample + (pNodes->m_pCp[2].fSample - pNodes->m_pCp[1].fSample);
		pNodes->m_pCp[3].fLine = pNodes->m_pCp[0].fLine + (pNodes->m_pCp[2].fLine - pNodes->m_pCp[1].fLine);
		break;
	case 3:
		pNodes->m_pCp[0].fSample = pNodes->m_pCp[3].fSample + (pNodes->m_pCp[1].fSample - pNodes->m_pCp[2].fSample);
		pNodes->m_pCp[0].fLine = pNodes->m_pCp[3].fLine + (pNodes->m_pCp[1].fLine - pNodes->m_pCp[2].fLine);
		break;
	}
}

void CData::MoveSideOfParallelogram(int nSide, float fSample, float fLine)
{
	CPolyCoord* pNodes = &m_pCADElement[m_nCADPtr].m_Nodes;

	switch(nSide) {
	case 1://top
		pNodes->m_pCp[0].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[0].fSample;
		pNodes->m_pCp[0].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[0].fLine;
		pNodes->m_pCp[1].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[1].fSample;
		pNodes->m_pCp[1].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[1].fLine;
		break;
	case 3://right
		pNodes->m_pCp[1].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[1].fSample;
		pNodes->m_pCp[1].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[1].fLine;
		pNodes->m_pCp[2].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[2].fSample;
		pNodes->m_pCp[2].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[2].fLine;
		break;
	case 5://bottom
		pNodes->m_pCp[2].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[2].fSample;
		pNodes->m_pCp[2].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[2].fLine;
		pNodes->m_pCp[3].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[3].fSample;
		pNodes->m_pCp[3].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[3].fLine;
		break;
	case 7://left
		pNodes->m_pCp[0].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[0].fSample;
		pNodes->m_pCp[0].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[0].fLine;
		pNodes->m_pCp[3].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[3].fSample;
		pNodes->m_pCp[3].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[3].fLine;
		break;
	}
}


void CData::MoveSideOfCentreLine(int nSide, float fSample, float fLine)
{
	CPolyCoord* pNodes = &m_pCADElement[m_nCADPtr].m_Nodes;

	float fRatio;
	CRect rrLeft,rrRight;
	CPoint ptLeftCentre,ptRightCentre,pt;


	switch(nSide) {
	case 1://left
		pNodes->m_pCp[0].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[0].fSample;
		pNodes->m_pCp[0].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[0].fLine;
		pNodes->m_pCp[1].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[1].fSample;
		pNodes->m_pCp[1].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[1].fLine;
		break;
	case 3://top

		//We shouldn't be able to change the angle
		//Find angle and so ratio 
		//if nominally vertical if so use sample else nominally horizontal and use line

		//if distance in Y/Lines is greater than int X/Samples we are vertical
		if( fabs(pNodes->m_pCp[1].Side0.fY-pNodes->m_pCp[2].Side0.fY) > fabs(pNodes->m_pCp[1].Side0.fX-pNodes->m_pCp[2].Side0.fX) ) {
			fRatio = (pNodes->m_pCp[1].Side0.fX-pNodes->m_pCp[2].Side0.fX)/(pNodes->m_pCp[2].Side0.fY-pNodes->m_pCp[1].Side0.fY);
			pNodes->m_pCp[1].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[1].fSample;
			pNodes->m_pCp[1].fLine = ((fSample - m_CpCursor.fSample)*fRatio) + m_CpNode[1].fLine;
			pNodes->m_pCp[2].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[2].fSample;
			pNodes->m_pCp[2].fLine = ((fSample - m_CpCursor.fSample)*fRatio) + m_CpNode[2].fLine;

			pNodes->m_pCp[0].fSample = m_CpNode[0].fSample- (fSample - m_CpCursor.fSample);
			pNodes->m_pCp[0].fLine =  m_CpNode[0].fLine - ((fSample - m_CpCursor.fSample)*fRatio);
			pNodes->m_pCp[3].fSample = m_CpNode[3].fSample - (fSample - m_CpCursor.fSample);
			pNodes->m_pCp[3].fLine = m_CpNode[3].fLine - ((fSample - m_CpCursor.fSample)*fRatio);
		}else{
			fRatio = (pNodes->m_pCp[2].Side0.fY-pNodes->m_pCp[1].Side0.fY)/(pNodes->m_pCp[1].Side0.fX-pNodes->m_pCp[2].Side0.fX);
			pNodes->m_pCp[1].fSample = ((fLine - m_CpCursor.fLine)*fRatio) + m_CpNode[1].fSample;
			pNodes->m_pCp[1].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[1].fLine;
			pNodes->m_pCp[2].fSample = ((fLine - m_CpCursor.fLine)*fRatio) + m_CpNode[2].fSample;
			pNodes->m_pCp[2].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[2].fLine;

			pNodes->m_pCp[0].fSample = m_CpNode[0].fSample- ((fLine - m_CpCursor.fLine)*fRatio);
			pNodes->m_pCp[0].fLine =  m_CpNode[0].fLine - (fLine - m_CpCursor.fLine);
			pNodes->m_pCp[3].fSample = m_CpNode[3].fSample - ((fLine - m_CpCursor.fLine)*fRatio);
			pNodes->m_pCp[3].fLine = m_CpNode[3].fLine - (fLine - m_CpCursor.fLine);
		}
		break;
	case 5://Right

		pNodes->m_pCp[2].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[2].fSample;
		pNodes->m_pCp[2].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[2].fLine;
		pNodes->m_pCp[3].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[3].fSample;
		pNodes->m_pCp[3].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[3].fLine;

		break;
	case 7://Bottom
		//We shouldn't be able to change the angle
		//Find angle and so ratio 
		//if nominally vertical if so use sample else nominally horizontal and use line

		//if distance in Y/Lines is greater than int X/Samples we are vertical
		if( fabs(pNodes->m_pCp[1].fLine-pNodes->m_pCp[2].fLine) > fabs(pNodes->m_pCp[1].fSample-pNodes->m_pCp[2].fSample) ) {
			fRatio = (pNodes->m_pCp[1].Side0.fX-pNodes->m_pCp[2].Side0.fX)/(pNodes->m_pCp[2].Side0.fY-pNodes->m_pCp[1].Side0.fY);
			pNodes->m_pCp[0].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[0].fSample;
			pNodes->m_pCp[0].fLine = ((fSample - m_CpCursor.fSample)*fRatio) + m_CpNode[0].fLine;
			pNodes->m_pCp[3].fSample = (fSample - m_CpCursor.fSample) + m_CpNode[3].fSample;
			pNodes->m_pCp[3].fLine = ((fSample - m_CpCursor.fSample)*fRatio) + m_CpNode[3].fLine;

			pNodes->m_pCp[1].fSample = m_CpNode[1].fSample- (fSample - m_CpCursor.fSample);
			pNodes->m_pCp[1].fLine =  m_CpNode[1].fLine - ((fSample - m_CpCursor.fSample)*fRatio);
			pNodes->m_pCp[2].fSample = m_CpNode[2].fSample - (fSample - m_CpCursor.fSample);
			pNodes->m_pCp[2].fLine = m_CpNode[2].fLine - ((fSample - m_CpCursor.fSample)*fRatio);
		}else{
			fRatio = (pNodes->m_pCp[2].Side0.fY-pNodes->m_pCp[1].Side0.fY)/(pNodes->m_pCp[1].Side0.fX-pNodes->m_pCp[2].Side0.fX);
			pNodes->m_pCp[0].fSample = ((fLine - m_CpCursor.fLine)*fRatio) + m_CpNode[0].fSample;
			pNodes->m_pCp[0].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[0].fLine;
			pNodes->m_pCp[3].fSample = ((fLine - m_CpCursor.fLine)*fRatio) + m_CpNode[3].fSample;
			pNodes->m_pCp[3].fLine = (fLine - m_CpCursor.fLine) + m_CpNode[3].fLine;

			pNodes->m_pCp[1].fSample = m_CpNode[1].fSample- ((fLine - m_CpCursor.fLine)*fRatio);
			pNodes->m_pCp[1].fLine =  m_CpNode[1].fLine - (fLine - m_CpCursor.fLine);
			pNodes->m_pCp[2].fSample = m_CpNode[2].fSample - ((fLine - m_CpCursor.fLine)*fRatio);
			pNodes->m_pCp[2].fLine = m_CpNode[2].fLine - (fLine - m_CpCursor.fLine);
		}
		break;
	}

	return ;
}

int CData::ModifyTopCADElement(float fSample, float fLine)
{

	m_pCADElement[m_nCADPtr].m_Nodes.m_pCp[0].fLine = fLine;

	return m_nCADPtr;
}

int CData::ModifyBottomCADElement(float fSample, float fLine)
{

	m_pCADElement[m_nCADPtr].m_Nodes.m_pCp[1].fLine = fLine;

	return m_nCADPtr;
}

int CData::ModifyLeftCADElement(float fSample, float fLine)
{

	m_pCADElement[m_nCADPtr].m_Nodes.m_pCp[0].fSample = fSample;

	return m_nCADPtr;
}

int CData::ModifyRightCADElement(float fSample, float fLine)
{

	m_pCADElement[m_nCADPtr].m_Nodes.m_pCp[1].fSample = fSample;

	return m_nCADPtr;
}

int CData::ModifyNodeCADElement(int nNode,float fSample, float fLine)
{

	m_pCADElement[m_nCADPtr].m_Nodes.m_pCp[nNode].fSample = fSample;
	m_pCADElement[m_nCADPtr].m_Nodes.m_pCp[nNode].fLine = fLine;

	return m_nCADPtr;
}

int CData::AddCADElement(CCADElement *pElement)
{
	AddCADElement();

	m_pCADElement[m_nCADPtr] = pElement;
	swprintf_s(m_pCADElement[m_nCADPtr].m_wText,L"%d",m_nCADPtr+1);

	return m_nCADPtr;
}


int CData::AddCADElement(CADElementType Type,float fSample, float fLine,int nImageType,int nImageNumber,int nPaletteNumber)
{
	m_nCADPtr=AddCADElement();

	m_pCADElement[m_nCADPtr].m_Type = Type;
	m_pCADElement[m_nCADPtr].m_Nodes.AllocateCoords(4);
	for(int ii=0;ii<4;ii++) {
		m_pCADElement[m_nCADPtr].m_Nodes.m_pCp[ii].fSample = fSample;
		m_pCADElement[m_nCADPtr].m_Nodes.m_pCp[ii].fLine = fLine;
	}
	m_pCADElement[m_nCADPtr].m_nImageType		= nImageType;
	m_pCADElement[m_nCADPtr].m_nImageNumber		= nImageNumber;
	m_pCADElement[m_nCADPtr].m_nPaletteNumber	= nPaletteNumber;

	ComputeAllElementStats(m_nCADPtr);

	return m_nCADPtr;
}

int CData::AddCADElement()
{
	CCADElement *pElementTemp;

	if(m_pCADElement == NULL) {
		m_pCADElement = new CCADElement[m_nCADElementL = 1];
		m_nCADPtr = 0;
	} else {
		pElementTemp = new CCADElement[m_nCADElementL];
		for(int ii=0;ii<m_nCADElementL;ii++) {
			pElementTemp[ii] = &m_pCADElement[ii];
		}
		SAFE_DELETE_ARRAY( m_pCADElement );
		m_pCADElement = new CCADElement[m_nCADElementL + 1];
		for(int ii=0;ii<m_nCADElementL;ii++) {
			m_pCADElement[ii] = &pElementTemp[ii];
		}
		SAFE_DELETE_ARRAY( pElementTemp );
		m_nCADElementL++;
	}
	m_nCADPtr = m_nCADElementL-1;
	m_pCADElement[m_nCADPtr].Zero();
	swprintf_s(m_pCADElement[m_nCADPtr].m_wText,L"%d",m_nCADPtr+1);


	return m_nCADPtr;
}

int CData::DeleteCADElement(int nPtr)
{
	CCADElement *pElementTemp;
	int nSrc,nDest;

	if(m_pCADElement == NULL) return 0;
	if(m_nCADElementL <= 0) return 0;

	if(m_nCADElementL == 1) {
		SAFE_DELETE_ARRAY( m_pCADElement );
		return m_nCADPtr = m_nCADElementL = 0;
	} else {
		pElementTemp = new CCADElement[m_nCADElementL];
		for(nSrc=0;nSrc<m_nCADElementL;nSrc++) {
			pElementTemp[nSrc] = &m_pCADElement[nSrc];
		}
		SAFE_DELETE_ARRAY( m_pCADElement );
		m_pCADElement = new CCADElement[--m_nCADElementL];
		for(nSrc=0,nDest=0;nSrc<nPtr;nSrc++,nDest++) {
			m_pCADElement[nDest] = &pElementTemp[nSrc];
		}
		for(nSrc=nPtr+1;nDest<m_nCADElementL;nSrc++,nDest++) {
			m_pCADElement[nDest] = &pElementTemp[nSrc];
		}

		SAFE_DELETE_ARRAY( pElementTemp );
	}
	m_nCADPtr = nPtr;
	if(m_nCADPtr >= m_nCADElementL) m_nCADPtr = m_nCADElementL - 1;

	return m_nCADPtr;
}

int CData::AddCADNode(int nElement, CADElementType Type, CCoord Cp)
{

	if(m_nCADElementL==0) nElement=AddCADElement();

	switch(Type) {
	case CAD_LINE:
		if(m_pCADElement[nElement].m_Nodes.m_nCoordL>=2) {
			nElement=AddCADElement();
		}
		m_pCADElement[nElement].m_Nodes.SetPrimaryAxis(30);
		m_pCADElement[nElement].m_Type = Type;
		m_pCADElement[nElement].m_Nodes.Add(Cp);
		break;
	}
	return nElement;
}

int CData::FindNearestCADNode(CCoord Cp)
{
	float fMinLength = 1000000.0f;
	float fLength;
	CCoord CpE;

	for(int nn=0;nn<m_nCADElementL;nn++) {
		for(int ii=0;ii < m_pCADElement[nn].m_Nodes.m_nCoordL;ii++) {
			m_pCADElement[nn].m_Nodes.GetCoord(ii,&CpE);
			fLength = Cp.Length(CpE);
			if(fLength < fMinLength) {
				m_nCADPtr = nn;
				m_nCADNode = ii;
				fMinLength = fLength;
			}
		}
	}
	return m_nCADPtr;
}

int CData::GetCADElement()
{
	return m_nCADPtr;

}

void CData::ComputeAllElementStats(int nPtr)
{
	CRect rrSample;
	float fFastScanSize, fFastIncrement;
	float fHeight,fWidth,fOpposite,fAngle;
	float fW[2],fH[2];
	CPolyCoord* pNode = &m_pCADElement[nPtr].m_Nodes;
	D3DXVECTOR3 vNode[4],vAO,vBO;
	ZeroMemory(vNode, sizeof vNode);
	int ii;

	if(m_pCADElement) {
		GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement, 0);
		fWidth = m_pCADElement[nPtr].m_fWidth = fabsf(m_pCADElement[nPtr].m_Nodes.m_pCp[0].fSample - m_pCADElement[nPtr].m_Nodes.m_pCp[1].fSample) * fFastIncrement;

		fHeight = m_pCADElement[nPtr].m_fHeight = fabsf(m_pCADElement[nPtr].m_Nodes.m_pCp[0].fLine - m_pCADElement[nPtr].m_Nodes.m_pCp[1].fLine) * m_fSlowIncrement;

		switch(m_pCADElement[nPtr].m_Type) {
		case CAD_LINE: m_pCADElement[nPtr].m_fTotalArea = 0.0f;
			m_pCADElement[nPtr].m_fLength =  sqrtf(fHeight * fHeight + fWidth * fWidth);
			m_pCADElement[nPtr].m_nFlags &= (CAD_LENGTH | CAD_WIDTH | CAD_HEIGHT | CAD_TOTAL_AREA |CAD_LINE_TEXT);
			break;
		case CAD_RECTANGLE:	m_pCADElement[nPtr].m_fTotalArea = fWidth * fHeight;
			m_pCADElement[nPtr].m_fLength =  2.0f * fHeight + 2.0f * fWidth;
			break;
		case CAD_ELLIPSE: m_pCADElement[nPtr].m_fTotalArea = PIf * (fWidth / 2.0f) * (fHeight / 2.0f);
			m_pCADElement[nPtr].m_fLength = CircumferenceEllipse(fWidth,fHeight);
			m_pCADElement[nPtr].m_nFlags &= (CAD_LENGTH | CAD_WIDTH | CAD_HEIGHT | CAD_TOTAL_AREA |CAD_LINE_TEXT);
			break;
		case CAD_CENTRELINE:
			m_pCADElement[nPtr].m_nFlags &= (CAD_LENGTH | CAD_WIDTH | CAD_HEIGHT | CAD_TOTAL_AREA |CAD_LINE_TEXT);

			//Notice we are working with real world coordinates
			for(ii=0;ii<4;ii++) {
				vNode[ii] = D3DXVECTOR3(pNode->m_pCp[ii].Side0.fX,pNode->m_pCp[ii].Side0.fY,0.0f);
			}

			m_pCADElement[nPtr].m_fWidth = D3DXVec3Length(&(vNode[0]-vNode[1]));
			m_pCADElement[nPtr].m_fLength = D3DXVec3Length(&(vNode[0]-vNode[3]));
			m_pCADElement[nPtr].m_fHeight = 0.0;

			break;
		case CAD_PARALLELOGRAM:
			m_pCADElement[nPtr].m_nFlags &= (CAD_LENGTH | CAD_WIDTH | CAD_HEIGHT | CAD_TOTAL_AREA |CAD_LINE_TEXT);
				
			fW[0] =  fabsf(pNode->m_pCp[1].fSample - pNode->m_pCp[3].fSample);
			fW[1] =  fabsf(pNode->m_pCp[0].fSample - pNode->m_pCp[2].fSample);
			fH[0] =  fabsf(pNode->m_pCp[1].fLine - pNode->m_pCp[3].fLine);
			fH[1] =  fabsf(pNode->m_pCp[0].fLine - pNode->m_pCp[2].fLine);
			fW[0] > fW[1] ? fWidth = fW[0] : fWidth = fW[1];
			fH[0] > fH[1] ? fHeight = fH[0] : fHeight = fH[1];
			m_pCADElement[nPtr].m_fWidth = fWidth * fFastIncrement;
			m_pCADElement[nPtr].m_fHeight = fHeight * m_fSlowIncrement;

			for(ii=0;ii<4;ii++) {
				vNode[ii] = D3DXVECTOR3(pNode->m_pCp[ii].fSample,pNode->m_pCp[ii].fLine,0.0f);
				vNode[ii] = D3DXVECTOR3(pNode->m_pCp[ii].fSample,pNode->m_pCp[ii].fLine,0.0f);
			}

			D3DXVec3Normalize(&vAO, &(vNode[0]-vNode[3]));
			D3DXVec3Normalize(&vBO, &(vNode[2]-vNode[3]));
			fAngle = acosf(D3DXVec3Dot(&vAO, &vBO));
			fOpposite = sinf(fAngle) * D3DXVec3Length(&(vNode[0]-vNode[3]));

			m_pCADElement[nPtr].m_fTotalArea = fOpposite * D3DXVec3Length(&(vNode[2]-vNode[3]));
			m_pCADElement[nPtr].m_fLength =  2.0f * D3DXVec3Length(&(vNode[0]-vNode[3])) + 2.0f * D3DXVec3Length(&(vNode[2]-vNode[3]));
			break;
		}

	

		rrSample.SetRect((int)m_pCADElement[nPtr].m_Nodes.m_pCp[0].fSample, (int)m_pCADElement[nPtr].m_Nodes.m_pCp[0].fLine, (int)m_pCADElement[nPtr].m_Nodes.m_pCp[1].fSample, (int)m_pCADElement[nPtr].m_Nodes.m_pCp[1].fLine);
		BuildHistogramData(m_pCADElement[nPtr].m_nImageType,m_pCADElement[nPtr].m_nImageNumber,rrSample);
		BuildHistogramString(m_pCADElement[nPtr].m_nFlags,&m_Hist,true);
		CopyMemory(&m_pCADElement[nPtr].m_Hist, &m_Hist, sizeof m_Hist);
	}
}

float CData::GetCADElementWidth(int nPtr,CString *pBuff,bool bVerbose)
{
	if(pBuff) {
		if(bVerbose == true) {
			pBuff->Format(L"Width: %.*f%s",theApp.m_nUnitsPrecision,m_pCADElement[nPtr].m_fWidth * theApp.m_fUnits, theApp.m_Units);
		} else {
			pBuff->Format(L"%.*f%s",theApp.m_nUnitsPrecision,m_pCADElement[nPtr].m_fWidth * theApp.m_fUnits, theApp.m_Units);
		};

	}

	return m_pCADElement[nPtr].m_fWidth;

}

float CData::GetCADElementHeight(int nPtr,CString *pBuff,bool bVerbose)
{
	if(pBuff) {
		if(bVerbose == true) {
			pBuff->Format(L"Height: %.*f%s",theApp.m_nUnitsPrecision,m_pCADElement[nPtr].m_fHeight * theApp.m_fUnits, theApp.m_Units);
		} else {
			pBuff->Format(L"%.*f%s",theApp.m_nUnitsPrecision,m_pCADElement[nPtr].m_fHeight * theApp.m_fUnits, theApp.m_Units);
		}
	}

	return m_pCADElement[nPtr].m_fHeight;
}

float CData::GetCADElementArea(int nPtr,CString *pBuff,bool bVerbose)
{
	int nResolution = 1;

	if(theApp.m_fUnits != 1.0f) nResolution = 3;

	if(pBuff) {
		if(bVerbose == true) {
			pBuff->Format(L"Area: %.*f%s",nResolution,m_pCADElement[nPtr].m_fTotalArea * (theApp.m_fUnits * theApp.m_fUnits), theApp.m_Units);
		} else {
			pBuff->Format(L"%.*f%s",nResolution,m_pCADElement[nPtr].m_fTotalArea * (theApp.m_fUnits * theApp.m_fUnits), theApp.m_Units);
		}
	}

	return m_pCADElement[nPtr].m_fTotalArea;
}

float CData::GetCADElementLength(int nPtr,CString *pBuff,bool bVerbose)
{
	if(pBuff) {
		if(bVerbose == true) {
			pBuff->Format(L"Length: %.*f%s",theApp.m_nUnitsPrecision,m_pCADElement[nPtr].m_fLength * theApp.m_fUnits, theApp.m_Units);
		} else {
			pBuff->Format(L"%.*f%s",theApp.m_nUnitsPrecision,m_pCADElement[nPtr].m_fLength * theApp.m_fUnits, theApp.m_Units);
		}
	}

	return m_pCADElement[nPtr].m_fLength;
}

void CData::GetCADElementHistogram(int nPtr,CString *pBuff,bool bVerbose)
{
	if(pBuff) {
		pBuff->Format(L"%s",m_pCADElement[nPtr].m_Hist.wStr);
	}
}

void CData::GetCADVerboseStats(int nPtr,CString *pBuff)
{
	CString Buff,Temp;

	pBuff->Empty();
	Buff.Empty();
	if(m_pCADElement[nPtr].m_nFlags & CAD_LENGTH) {
		GetCADElementLength(nPtr,&Temp,true);
		Buff += Temp;
		Buff += "\t";
	}
	if(m_pCADElement[nPtr].m_nFlags & CAD_WIDTH) {
		GetCADElementWidth(nPtr,&Temp,true);
		Buff += Temp;
		Buff += "\t";
	}
	if(m_pCADElement[nPtr].m_nFlags & CAD_HEIGHT) {
		GetCADElementHeight(nPtr,&Temp,true);
		Buff += Temp;
		Buff += "\t";
	}
	if(m_pCADElement[nPtr].m_nFlags & CAD_TOTAL_AREA) {
		GetCADElementArea(nPtr,&Temp,true);
		Buff += Temp;
		Buff += "\t";
	}
	if(m_pCADElement[nPtr].m_nFlags & CAD_HISTOGRAM) {
		GetCADElementHistogram(nPtr,&Temp,true);
		Buff += Temp;
		Buff += "\t";
	}

	if(Buff.GetLength() > 0) Buff.GetBufferSetLength(Buff.GetLength()-1);
	pBuff->Format(L"%s", Buff);
}

bool CData::GetCADElementFlags(int *pnFlags)
{
	if(m_nCADElementL<=0) return false;

	*pnFlags = m_pCADElement[m_nCADPtr].m_nFlags;
	return true;
}

bool CData::SetCADElementFlags(int nFlags)
{
	if(m_nCADElementL<=0) return false;

	m_pCADElement[m_nCADPtr].m_nFlags = nFlags;
	return true;

}

void CData::ToggleCADElementFlag(int nFlag)
{

	if(IsCADElement()) {
		m_pCADElement[m_nCADPtr].m_nFlags & nFlag ? m_pCADElement[m_nCADPtr].m_nFlags &= ~nFlag : m_pCADElement[m_nCADPtr].m_nFlags |= nFlag;
		ComputeAllElementStats(m_nCADPtr);
	}
}

bool CData::IsCADElement()
{
	if(m_pCADElement == NULL) return false;
	return true;

}

void CData::SnapToNearestCADElement(int nSrcElement, int nFeature)
{
	CCoord CpSrc,CpDest,CpShortest,CpNode[2];
	float fShortest = 1e9f;
	float fLength,fX,fY;
	int ii,ff;

	CpNode[0] = m_pCADElement[nSrcElement].m_Nodes.m_pCp[0];
	CpNode[1] = m_pCADElement[nSrcElement].m_Nodes.m_pCp[1];

	switch(nFeature) {
	case 0: CpSrc = CpNode[0];
		break;
	case 1: CpSrc.fSample = (CpNode[1].fSample + CpNode[0].fSample) / 2.0f;
		CpSrc.fLine = CpNode[0].fLine;
		break;
	case 2: CpSrc.fSample = CpNode[1].fSample;
		CpSrc.fLine = CpNode[0].fLine;
		break;
	case 3: CpSrc.fSample = CpNode[1].fSample;
		 CpSrc.fLine = (CpNode[1].fLine + CpNode[0].fLine) / 2.0f;
		 break;
	case 4: CpSrc = CpNode[1];
		break;
	case 5: CpSrc.fSample = (CpNode[1].fSample + CpNode[0].fSample) / 2.0f;
		CpSrc.fLine = CpNode[1].fLine;
		break;
	case 6: CpSrc.fSample = CpNode[0].fSample;
		CpSrc.fLine = CpNode[1].fLine;
		break;
	case 7: CpSrc.fSample = CpNode[0].fSample;
		 CpSrc.fLine = (CpNode[1].fLine + CpNode[0].fLine) / 2.0f;
		 break;
	default: return;
		break;
	}

	for(ii=0;ii<m_nCADElementL;ii++) {
		if(ii - nSrcElement) {

			for(ff=0;ff<8;ff++) {

				CpNode[0] = m_pCADElement[ii].m_Nodes.m_pCp[0];
				CpNode[1] = m_pCADElement[ii].m_Nodes.m_pCp[1];

				switch(ff) {
				case 0: CpDest = CpNode[0];
					break;
				case 1: CpDest.fSample = (CpNode[1].fSample + CpNode[0].fSample) / 2.0f;
					CpDest.fLine = CpNode[0].fLine;
					break;
				case 2: CpDest.fSample = CpNode[1].fSample;
					CpDest.fLine = CpNode[0].fLine;
					break;
				case 3: CpDest.fSample = CpNode[1].fSample;
					 CpDest.fLine = (CpNode[1].fLine + CpNode[0].fLine) / 2.0f;
					 break;
				case 4: CpDest = CpNode[1];
					break;
				case 5: CpDest.fSample = (CpNode[1].fSample + CpNode[0].fSample) / 2.0f;
					CpDest.fLine = CpNode[1].fLine;
					break;
				case 6: CpDest.fSample = CpNode[0].fSample;
					CpDest.fLine = CpNode[1].fLine;
					break;
				case 7: CpDest.fSample = CpNode[0].fSample;
					 CpDest.fLine = (CpNode[1].fLine + CpNode[0].fLine) / 2.0f;
					 break;
				}
				
				fX = CpDest.fSample - CpSrc.fSample;
				fY = CpDest.fLine - CpSrc.fLine;

				fLength = sqrtf(fX * fX + fY * fY);

				if(fLength < fShortest) {
					fShortest = fLength;
					CpShortest = CpDest;
				}
			}
		}
	}

	CpNode[0] = m_pCADElement[nSrcElement].m_Nodes.m_pCp[0];
	CpNode[1] = m_pCADElement[nSrcElement].m_Nodes.m_pCp[1];

	switch(nFeature) {
	case 0:  m_pCADElement[nSrcElement].m_Nodes.m_pCp[0] = CpShortest;
		break;
	case 1:	fX = fabsf(CpNode[1].fSample - CpNode[0].fSample) / 2.0f;
		m_pCADElement[nSrcElement].m_Nodes.m_pCp[0].fSample = CpShortest.fSample - fX;
		m_pCADElement[nSrcElement].m_Nodes.m_pCp[1].fSample = CpShortest.fSample + fX;
		m_pCADElement[nSrcElement].m_Nodes.m_pCp[0].fLine = CpShortest.fLine;
		break;
	case 2:m_pCADElement[nSrcElement].m_Nodes.m_pCp[1].fSample = CpShortest.fSample;
		m_pCADElement[nSrcElement].m_Nodes.m_pCp[0].fLine = CpShortest.fLine;
		break;
	case 3:	fY = fabsf(CpNode[1].fLine - CpNode[0].fLine) / 2.0f;
		m_pCADElement[nSrcElement].m_Nodes.m_pCp[1].fSample = CpShortest.fSample;
		m_pCADElement[nSrcElement].m_Nodes.m_pCp[0].fLine = CpShortest.fLine - fY;
		m_pCADElement[nSrcElement].m_Nodes.m_pCp[1].fLine = CpShortest.fLine + fY;
		break;
	case 4:  m_pCADElement[nSrcElement].m_Nodes.m_pCp[1] = CpShortest;
		break;
	case 5:	fX = fabsf(CpNode[1].fSample - CpNode[0].fSample) / 2.0f;
		m_pCADElement[nSrcElement].m_Nodes.m_pCp[0].fSample = CpShortest.fSample - fX;
		m_pCADElement[nSrcElement].m_Nodes.m_pCp[1].fSample = CpShortest.fSample + fX;
		m_pCADElement[nSrcElement].m_Nodes.m_pCp[1].fLine = CpShortest.fLine;
		break;
	case 6:m_pCADElement[nSrcElement].m_Nodes.m_pCp[0].fSample = CpShortest.fSample;
		m_pCADElement[nSrcElement].m_Nodes.m_pCp[1].fLine = CpShortest.fLine;
		break;
	case 7:	fY = fabsf(CpNode[1].fLine - CpNode[0].fLine) / 2.0f;
		m_pCADElement[nSrcElement].m_Nodes.m_pCp[0].fSample = CpShortest.fSample;
		m_pCADElement[nSrcElement].m_Nodes.m_pCp[0].fLine = CpShortest.fLine - fY;
		m_pCADElement[nSrcElement].m_Nodes.m_pCp[1].fLine = CpShortest.fLine + fY;
		break;
	}
	ComputeAllElementStats(nSrcElement);

}

float CData::CircumferenceEllipse(float fWidth, float fHeight)
{
	float fH, fA, fB, fLength;

	fA = fWidth / 2.0f;
	fB = fHeight / 2.0f;

	fH = ( (fA - fB) * (fA - fB) ) / ( (fA + fB) * (fA + fB) );
								//1 + 1/4 . fH + 1/64 . fH^2 + 1/256 . fH^3 
	fLength = PIf * ( fA + fB) * (1.0f + (0.25f * fH) + (0.015625f * (fH * fH)) + (0.00390625f * (fH * fH *fH)));

	return fLength;
}

void CData::SetHeightSameWidthCADElement(int nPtr)
{
	float fWidth, fFastScanSize, fFastIncrement;

	GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement, 0);
	fWidth = m_pCADElement[nPtr].m_fWidth = fabsf(m_pCADElement[nPtr].m_Nodes.m_pCp[0].fSample - m_pCADElement[nPtr].m_Nodes.m_pCp[1].fSample) * fFastIncrement;

	m_pCADElement[nPtr].m_Nodes.m_pCp[1].fLine = m_pCADElement[nPtr].m_Nodes.m_pCp[0].fLine + (fWidth / m_fSlowIncrement);
	ComputeAllElementStats(nPtr);
}

void CData::SetWidthSameHeightCADElement(int nPtr)
{
	float  fHeight,fFastScanSize, fFastIncrement;

	GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement, 0);
	fHeight = fabsf(m_pCADElement[nPtr].m_Nodes.m_pCp[0].fLine - m_pCADElement[nPtr].m_Nodes.m_pCp[1].fLine) * m_fSlowIncrement;

	m_pCADElement[nPtr].m_Nodes.m_pCp[1].fSample = m_pCADElement[nPtr].m_Nodes.m_pCp[0].fSample + (fHeight / fFastIncrement);
	ComputeAllElementStats(nPtr);

}

void CData::BuildHistogramString(int nFlags,HistogramStruct *pHist,bool bVerbose)
{
	CString strTotal,Buff;
	CString	Reference,DefectLevel,DefectArea;
	int nStrLength;;

	strTotal.Empty();

	if(nFlags & CAD_REF_LEVEL) {
		GetHistogramRefLevel(pHist, &Buff, true);
		strTotal += Buff;
		strTotal += "\t";
	}

	if(nFlags & CAD_DEFECT_LEVEL) {
		GetHistogramDefectLevel(pHist, &Buff, true);
		strTotal += Buff;
		strTotal += "\t";
	}

	if(nFlags & CAD_DEFECT_AREA) {
		GetHistogramDefectArea(pHist, &Buff, true);
		strTotal += Buff;
		strTotal += "\t";
	}

	if(nFlags & CAD_DEFECT_WIDTH) {
		GetHistogramDefectWidth(pHist, &Buff, true);
		strTotal += Buff;
		strTotal += "\t";
	}
	if(nFlags & CAD_DEFECT_HEIGHT) {
		GetHistogramDefectHeight(pHist, &Buff, true);
		strTotal += Buff;
		strTotal += "\t";
	}

	pHist->wStr[0]=0;
	if(strTotal.GetLength() > 0) {
		strTotal.GetBufferSetLength(strTotal.GetLength()-1);
		nStrLength = sizeof(pHist->wStr) / sizeof WCHAR - 1;
		if(strTotal.GetLength() >= nStrLength) strTotal.GetBufferSetLength(nStrLength);
		swprintf_s(pHist->wStr,512,L"%s",(LPCWSTR)strTotal);
	}
}

float CData::GetHistogramRefLevel(HistogramStruct *pHist,CString *pBuff,bool bVerbose)
{
	CString Reference,Buff;
	float fRef;

	if(pHist->nAmpMode == HIST_ATTENUATION) {
		switch(pHist->nAmplitudeScaleType) {
		case 0:	Buff.Format(_T("%.01f %%"),fRef = pHist->fLatchAmp);
			break;
		case 1:	fRef = -20.0f * (float)log10(100.0/pHist->fLatchAmp);
			Buff.Format(_T("%.01f dB"),fRef);
			break;
		case 2:	Buff.Format(_T("%.01f dB"),fRef = (100.0f-pHist->fLatchAmp) * (float)m_nLogRange / -100.0f);
			break;
		}
	} else {	//HIST REFLECTION
		switch(pHist->nAmplitudeScaleType) {
		case 0:	Buff.Format(_T("%.01f %%"),fRef = pHist->fLatchAmp);
			break;
		case 1:	fRef = -20.0f * (float)log10(100.0/pHist->fLatchAmp);
			Buff.Format(_T("%.01f dB"),fRef);
			break;
		case 2:	Buff.Format(_T("%.01f dB"),fRef = (100.0f-pHist->fLatchAmp) * (float)m_nLogRange / -100.0f);
			break;
		}
	}
	if(pBuff) {
		if(bVerbose) {
			Reference.LoadString(IDS_Ref);
			pBuff->Format(L"%s: %s",Reference,Buff);
		} else {
			pBuff->Format(L"%s",Buff);
		}
	}


	return fRef;

}

float CData::GetHistogramDefectLevel(HistogramStruct *pHist,CString *pBuff,bool bVerbose)
{
	CString DefectLevel,Buff;
	float fDefectLevel;

	int	nLevel = 0;
	
	switch(pHist->nAmplitudeScaleType) {
	case 0:	Buff.Format(_T("%.01f %%"),fDefectLevel = -100.0f / (float)pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[nLevel]/-20.0f));
		break;
	case 1:	Buff.Format(_T("%.01f dB"),fDefectLevel = theApp.m_LastSettings.fHistogramRelativedB[nLevel]);
		break;
	case 2:	Buff.Format(_T("%.01f dB"),fDefectLevel = theApp.m_LastSettings.fHistogramRelativedB[nLevel]);
		break;
	}


	if(pBuff) {
		if(bVerbose) {
			DefectLevel.LoadString(IDS_Defect_Level);
			pBuff->Format(L"%s: %s",DefectLevel,Buff);
		} else {
			pBuff->Format(L"%s",Buff);
		}
	}

	return fDefectLevel;
}

float CData::GetHistogramDefectArea(HistogramStruct *pHist,CString *pBuff,bool bVerbose)
{
	CString DefectArea,Buff;
	float fDefectArea;

	int	nLevel = 0;

	switch(theApp.m_LastSettings.nMeasurementUnits) {
	case 0:	Buff.Format(_T("%.0f sq mm"),fDefectArea = (float)pHist->nTotalBelowThreshold[nLevel]*m_fFastIncrement*m_fSlowIncrement);
		break;
	case 1:	Buff.Format(_T("%.02f sq \x022"),fDefectArea = ((float)pHist->nTotalBelowThreshold[nLevel]*m_fFastIncrement*m_fSlowIncrement) / 645.16f);
		break;
	}

	if(pBuff) {
		if(bVerbose) {
			DefectArea.LoadString(IDS_Defect_Area);
			pBuff->Format(L"%s: %s",DefectArea,Buff);
		} else {
			pBuff->Format(L"%s",Buff);
		}
	}

	return fDefectArea;
}

float CData::GetHistogramDefectWidth(HistogramStruct *pHist,CString *pBuff,bool bVerbose)
{
	CString DefectWidth,Buff;
	float fDefectWidth;

	int	nLevel = 0;

	switch(theApp.m_LastSettings.nMeasurementUnits) {
	case 0:	Buff.Format(_T("%.0f mm"),fDefectWidth = pHist->rrFloatThreshWidth.FloatWidth());
		break;
	case 1:	Buff.Format(_T("%.02f\x022"),fDefectWidth = pHist->rrFloatThreshWidth.FloatWidth() / 25.4f);
		break;
	}

	if(pBuff) {
		if(bVerbose) {
			DefectWidth.LoadString(IDS_Defect_Width);
			pBuff->Format(L"%s: %s",DefectWidth,Buff);
		} else {
			pBuff->Format(L"%s",Buff);
		}
	}

	return fDefectWidth;
}

float CData::GetHistogramDefectHeight(HistogramStruct *pHist,CString *pBuff,bool bVerbose)
{
	CString DefectHeight,Buff;
	float fDefectHeight;

	int	nLevel = 0;

	switch(theApp.m_LastSettings.nMeasurementUnits) {
	case 0:	Buff.Format(_T("%.0f mm"),fDefectHeight = pHist->rrFloatThreshHeight.FloatHeight()*m_fSlowIncrement);
		break;
	case 1:	Buff.Format(_T("%.02f\x022"),fDefectHeight = pHist->rrFloatThreshHeight.FloatHeight() / 25.4f);
		break;
	}

	if(pBuff) {
		if(bVerbose) {
			DefectHeight.LoadString(IDS_Defect_Height);
			pBuff->Format(L"%s: %s",DefectHeight,Buff);
		} else {
			pBuff->Format(L"%s",Buff);
		}
	}

	return fDefectHeight;
}




bool CData::TransformScanVertexToCADVertexWithTwist(D3DXVECTOR3 *ptDest, D3DXVECTOR3 *normDest, CUSLVertex *vertSrc, int nLine)
{
	CCoord Cp;

	D3DXMATRIXA16 matCAD,matCADNorm,matRotate,matWorld;

	D3DXMatrixInverse(&matCAD,NULL,&m_matCadToScan);

	if(PROFILE->m_ScanLine) {
		PROFILE->m_ScanLine[nLine].GetCoord(0,&Cp);

		D3DXMatrixRotationY(&matRotate,Cp.Side0.fR*DEG_TO_RAD);
		matCAD = matRotate * matCAD;

	}

	matCADNorm = matCAD;
	matCADNorm._41 = 0.0f;
	matCADNorm._42 = 0.0f;
	matCADNorm._43 = 0.0f;

	D3DXVec3TransformCoord(ptDest,&vertSrc->pt,&matCAD);
	D3DXVec3TransformCoord(normDest,&(D3DXVECTOR3)vertSrc->norm,&matCADNorm);

	return true;
}

void CData::GetResultsForALine(int nLine, CStringArray &Array)
{
	CString Buff,Temp;
	Array.RemoveAll();
	int	gg,tt,nTS, nSample;
	float fThickness;

	for(nSample=0; nSample<m_nSamplesLine; nSample++) {
		gg=tt=0;
		Buff.Empty();
		for(nTS=0;nTS<m_nNumberTimeSlots;nTS++) {
			for(gg;gg<m_nNumberAmpImages[nTS];gg++) {
				Temp.Format(L"%d,L",MulDiv(GetFilteredAmp(nSample,nLine,gg)-128,100,127));
				Buff += Temp;
			}
			for(tt;tt<m_nNumberTimeImages[nTS];tt++) {
				fThickness = (float)GetFilteredTimeThickness(nSample, nLine, tt,1) / 100.0f;
				Temp.Format(L"%.02f,L",fThickness);
				Buff += Temp;
			}
		}
		Buff += L"\r\n";
		Array.Add(Buff);
	}

}


bool CData::GetVerticesAndResultsForALine(int nLine, CStringArray &Array)
{
	D3DXVECTOR3 pt,norm;
	Array.RemoveAll();
	int nTS,tt,gg,nSample,ii;
	float fThickness;
	CString Temp,Buff;

	if(m_vLineSample == NULL) return false;
	if(nLine>=m_nNumberLines) return false;

	for(ii=0;ii<m_vLineSample[nLine].GetSize();ii++) {
		Buff.Empty();
		if(theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame==1) {
			TransformScanVertexToCADVertexWithTwist(&pt, &norm, &m_vLineSample[nLine].m_pVp[ii], nLine);
		} else {
			pt = m_vLineSample[nLine].m_pVp[ii].pt;
			norm = (D3DXVECTOR3)m_vLineSample[nLine].m_pVp[ii].norm;
		}
		Buff.Format(L"%0.03f,%0.03f,%0.03f,%0.5f,%0.5f,%0.5f",pt.x,pt.y,pt.z,norm.x,norm.y,norm.z);

		nSample = ii + m_vLineSample[nLine].m_nFirstSample;
		gg=0;tt=0;
		for(nTS=0;nTS<m_nNumberTimeSlots;nTS++) {
			for(gg;gg<m_nNumberAmpImages[nTS];gg++) {
				Temp.Format(L",%d",MulDiv(GetFilteredAmp(nSample,nLine,gg)-128,100,127));
				Buff += Temp;
			}
			for(tt;tt<m_nNumberTimeImages[nTS];tt++) {
				fThickness = (float)GetFilteredTimeThickness(nSample, nLine, tt,1) / 100.0f;
				Temp.Format(L",%.02f",fThickness);
				Buff += Temp;
			}
		}
		Array.Add(Buff);
	}
	return true;
}


void CData::MeanFilter3X3(int nSrcType,int nSrc, int nDest)
{
	int	nLine,nSample,xx,yy,nLineOffset;
	int nRows = 3;
	int	nColumns = 3;

	unsigned char* pSrc = NULL;

	switch(nSrcType) {
	case AMP_IMAGE: pSrc = m_cAmpBuffer[nSrc];
		break;
	case 1: pSrc = m_cFilterBuffer[nSrc];
		break;
	}
	if(pSrc == NULL) return;

	if(m_cFilterBuffer[nDest] == NULL) {
		int	nSize = m_nSamplesLine * (m_nNumberLines + 1);	//one extra line for extra memory for the line shifting
		m_cFilterBuffer[nDest] = new unsigned char[nSize];
	}
	unsigned char* pDest = m_cFilterBuffer[nDest];

	int nSum;

	for(nLine=1;nLine<m_nNumberLines-2;nLine++) {
		for(nSample=1;nSample<m_nSamplesLine-2;nSample++) {
			nSum=0;

			for(yy=-1;yy<2;yy++) {
				nLineOffset = (nLine + yy) * m_nSamplesLine;
				for(xx=-1;xx<2;xx++) {
					nSum += pSrc[nLineOffset + nSample + xx];
				}
			}
			nLineOffset = nLine * m_nSamplesLine;
			pDest[nLineOffset + nSample] = MinMax(nSum / 9,128,255);
		}
	}

}

void CData::MedianFilter3X3(int nSrcType,int nSrc, int nDest)
{
	int	nLine,nSample,xx,yy,nLineOffset;
	int nRows = 3;
	int	nColumns = 3;
	unsigned char cB[9];
	int nn;

	unsigned char* pSrc = NULL;

	switch(nSrcType) {
	case AMP_IMAGE: pSrc = m_cAmpBuffer[nSrc];
		break;
	case 1: pSrc = m_cFilterBuffer[nSrc];
		break;
	}
	if(pSrc == NULL) return;

	if(m_cFilterBuffer[nDest] == NULL) {
		int	nSize = m_nSamplesLine * (m_nNumberLines + 1);	//one extra line for extra memory for the line shifting
		m_cFilterBuffer[nDest] = new unsigned char[nSize];
	}
	unsigned char* pDest = m_cFilterBuffer[nDest];


	for(nLine=1;nLine<m_nNumberLines-2;nLine++) {
		for(nSample=1;nSample<m_nSamplesLine-2;nSample++) {
			nn=0;

			for(yy=-1;yy<2;yy++) {
				nLineOffset = (nLine + yy) * m_nSamplesLine;
				for(xx=-1;xx<2;xx++,nn++) {
					cB[nn] = pSrc[nLineOffset + nSample + xx];
				}
			}
			nLineOffset = nLine * m_nSamplesLine;
			qsort(cB,9,sizeof (unsigned char),CompareUnsignedChar);
			pDest[nLineOffset + nSample] = cB[4];
		}
	}


}


int CompareUnsignedChar(const void *Cp0,const void *Cp1)
{
	unsigned char *C0 = (unsigned char *)Cp0;
	unsigned char *C1 = (unsigned char *)Cp1;

	return *C0 - *C1;

}

bool CData::HaveAllLinesBeenScanned()
{
	int	ii;
	for(ii=0;ii<m_nNumberLines;ii++) {
		if(m_nLineScannedStatus[ii] == 0) return false;
	}
	return true;
}

void CData::PutvLineSampleInCADCoordinates()
{
	if(m_vLineSample == NULL) return;

	int nLine;
	CCoord Cp;
	D3DXMATRIXA16 matRotate,matCAD,matWorld;

	D3DXMatrixIdentity( &matRotate );
	D3DXMatrixInverse(&matCAD,NULL,&m_matCadToScan);


	for(nLine=0;nLine<m_nNumberLines;nLine++) {

		if(theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame & EXPORT_TWIST_COORDINATES) {
			if(m_ProfileName == PROFILE->m_FileName) {
				PROFILE->m_ScanLine[nLine].GetCoord(0,&Cp);
				D3DXMatrixRotationY(&matRotate,-Cp.Side0.fR*DEG_TO_RAD);
			}
		}
		if(theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame & EXPORT_CAD_COORDINATES) {
			matWorld = matRotate * matCAD;
		} else {
			matWorld = matRotate;
		}
		m_vLineSample[nLine].TransformLine(matWorld);
	}

}

void CData::PutvLineSamplesBackInttoScanCoordinates()
{
	if(m_vLineSample == NULL) return;

	int nLine;
	CCoord Cp;
	D3DXMATRIXA16 matRotate,matWorld;

	D3DXMatrixIdentity( &matRotate );


	for(nLine=0;nLine<m_nNumberLines;nLine++) {

		if(theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame & EXPORT_TWIST_COORDINATES) {
			if(m_ProfileName == PROFILE->m_FileName) {
				PROFILE->m_ScanLine[nLine].GetCoord(0,&Cp);
				D3DXMatrixRotationY(&matRotate,Cp.Side0.fR*DEG_TO_RAD);
			}
		}
		if(theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame & EXPORT_CAD_COORDINATES) {
			matWorld = m_matCadToScan * matRotate;
		} else {
			matWorld = matRotate;
		}
		m_vLineSample[nLine].TransformLine(matWorld);
	}


}

int CData::MinMax(int *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}

long CData::MinMax(long *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}



void CData::AppendDataSet(int nWhichSide, CData *pNewData)
{
	CVertexArray	*vLineSample = NULL;
	int nTS,nn,gg,tt;
	char *cAmpBuffer = NULL;
	int	*nTimeBuffer = NULL;
	int nSize,nLine,nSrc,nDest;

	int nNumberLines = m_nNumberLines;
	int nSamplesLine = m_nSamplesLine;


	switch(nWhichSide) {
	case APPEND_RIGHT:
		if(pNewData->m_nNumberLines > nNumberLines) nNumberLines = pNewData->m_nNumberLines;
		nSamplesLine += pNewData->m_nSamplesLine;
		nSize = nSamplesLine * nNumberLines;

		cAmpBuffer = new char[nSize];
		nTimeBuffer = new int[nSize];

		gg=tt=0;
		for(nTS=0;nTS<m_nNumberTimeSlots;nTS++) {
			for(nn=0;nn<m_nNumberAmpImages[nTS];nn++) {
				FillMemory(cAmpBuffer,nSize,INIT_VALUE);
				if(m_cAmpBuffer[gg]) {
					for(nLine=0;nLine<m_nNumberLines;nLine++) {
						nSrc = nLine * m_nSamplesLine;
						nDest = nLine * nSamplesLine;
						CopyMemory(&cAmpBuffer[nDest], &m_cAmpBuffer[gg][nSrc], m_nSamplesLine);
					}
				}
				if(pNewData->m_cAmpBuffer[gg]) {
					for(nLine=0;nLine<pNewData->m_nNumberLines;nLine++) {
						nSrc = nLine * pNewData->m_nSamplesLine;
						nDest = nLine * nSamplesLine + m_nSamplesLine;
						CopyMemory(&cAmpBuffer[nDest], &pNewData->m_cAmpBuffer[gg][nSrc], pNewData->m_nSamplesLine);
						if(m_vLineSample) {
							if((nLine<m_nNumberVertexLines) && (nLine < pNewData->m_nNumberVertexLines)){
								m_vLineSample[nLine].Append(&pNewData->m_vLineSample[nLine]);
							}
						}
					}
				}
				delete m_cAmpBuffer[gg];
				m_cAmpBuffer[gg] = new unsigned char[nSize];
				CopyMemory(&m_cAmpBuffer[gg][0], &cAmpBuffer[0], nSize);
				gg++;
			}
		}

		break;
	case APPEND_BOTTOM:
		if(pNewData->m_nSamplesLine > nSamplesLine) nSamplesLine = pNewData->m_nSamplesLine;
		nNumberLines += pNewData->m_nNumberLines;
		nSize = nSamplesLine * nNumberLines;

		vLineSample = new CVertexArray[ m_nNumberVertexLines = nNumberLines ];

		cAmpBuffer = new char[nSize];
		nTimeBuffer = new int[nSize];

		for(gg=0;gg<NUMBER_BUFFERS;gg++) {
			if(m_cAmpBuffer[gg] || pNewData->m_cAmpBuffer[gg]) {
				FillMemory(cAmpBuffer,nSize,INIT_VALUE);
				if(m_cAmpBuffer[gg]) {
					for(nLine=0;nLine<m_nNumberLines;nLine++) {
						nSrc = nLine * m_nSamplesLine;
						nDest = nLine * nSamplesLine;
						CopyMemory(&cAmpBuffer[nDest], &m_cAmpBuffer[gg][nSrc], m_nSamplesLine);
						if(m_vLineSample) {
							vLineSample[nLine] = &m_vLineSample[nLine];
						}
					}
				}
				if(pNewData->m_cAmpBuffer[gg]) {
					for(nLine=0;nLine<pNewData->m_nNumberLines;nLine++) {
						nSrc = nLine * pNewData->m_nSamplesLine;
						nDest = (nLine + m_nNumberLines) * nSamplesLine;
						CopyMemory(&cAmpBuffer[nDest], &pNewData->m_cAmpBuffer[gg][nSrc], pNewData->m_nSamplesLine);
						if(m_vLineSample) {
							if(((nLine + m_nNumberLines)<m_nNumberVertexLines) && (nLine <pNewData->m_nNumberVertexLines)) {
								vLineSample[nLine + m_nNumberLines] = &pNewData->m_vLineSample[nLine];
							}
						}
					}
				}
				delete m_cAmpBuffer[gg];
				m_cAmpBuffer[gg] = new unsigned char[nSize];
				CopyMemory(&m_cAmpBuffer[gg][0], &cAmpBuffer[0], nSize);
				SAFE_DELETE_ARRAY( m_vLineSample );
				m_vLineSample = new CVertexArray[ m_nNumberVertexLines ];
				for(nLine = 0; nLine<m_nNumberVertexLines; nLine++) {
					m_vLineSample[nLine] = &vLineSample[nLine];
				}
			}
		}

		break;
	case APPEND_TOP:
		if(pNewData->m_nSamplesLine > nSamplesLine) nSamplesLine = pNewData->m_nSamplesLine;
		nNumberLines += pNewData->m_nNumberLines;
		nSize = nSamplesLine * nNumberLines;

		vLineSample = new CVertexArray[ m_nNumberVertexLines = nNumberLines ];

		cAmpBuffer = new char[nSize];
		nTimeBuffer = new int[nSize];

		for(gg=0;gg<NUMBER_BUFFERS;gg++) {
			if(m_cAmpBuffer[gg] || pNewData->m_cAmpBuffer[gg]) {
				FillMemory(cAmpBuffer,nSize,INIT_VALUE);
				if(pNewData->m_cAmpBuffer[gg]) {
					for(nLine=0;nLine<pNewData->m_nNumberLines;nLine++) {
						nSrc = nLine * pNewData->m_nSamplesLine;
						nDest = nLine * nSamplesLine;
						CopyMemory(&cAmpBuffer[nDest], &pNewData->m_cAmpBuffer[gg][nSrc], pNewData->m_nSamplesLine);
						if(m_vLineSample) {
							if((nLine < m_nNumberVertexLines) && (nLine < pNewData->m_nNumberVertexLines)) {
								vLineSample[nLine] = &pNewData->m_vLineSample[nLine];
							}
						}
					}
				}
				if(m_cAmpBuffer[gg]) {
					for(nLine=0;nLine<m_nNumberLines;nLine++) {
						nSrc = nLine * m_nSamplesLine;
						nDest = (nLine + pNewData->m_nNumberLines) * nSamplesLine;
						CopyMemory(&cAmpBuffer[nDest], &m_cAmpBuffer[gg][nSrc], m_nSamplesLine);
						if(m_vLineSample) {
							if((nLine + pNewData->m_nNumberLines)<m_nNumberVertexLines) {
								vLineSample[nLine + pNewData->m_nNumberLines] = &m_vLineSample[nLine];
							}
						}
					}
				}
				delete m_cAmpBuffer[gg];
				m_cAmpBuffer[gg] = new unsigned char[nSize];
				CopyMemory(&m_cAmpBuffer[gg][0], &cAmpBuffer[0], nSize);
				SAFE_DELETE_ARRAY( m_vLineSample );
				m_vLineSample = new CVertexArray[ m_nNumberVertexLines ];
				for(nLine = 0; nLine<m_nNumberVertexLines; nLine++) {
					m_vLineSample[nLine] = &vLineSample[nLine];
				}
			}
		}

		break;
	}
	m_nNumberLines = nNumberLines;
	m_nSamplesLine = nSamplesLine;

	m_fSlowScanSize = (float)m_nSamplesLine * m_fSlowIncrement;
	m_fFastScanSize = (float)m_nNumberLines * m_fFastIncrement;

	m_nMinViewLine = 0;
	m_nMaxViewLine = m_nNumberLines - 1;

	SAFE_DELETE( cAmpBuffer );
	SAFE_DELETE( nTimeBuffer );
	SAFE_DELETE_ARRAY( vLineSample );
}

void CData::CropRightEdge(int nNumberSamples)
{
	int gg,nLine,nSrc,nDest;
	
	int nSize = m_nNumberLines * (m_nSamplesLine - nNumberSamples);

	for(gg=0;gg<NUMBER_BUFFERS;gg++) {
		if(m_cAmpBuffer[gg]) {
			for(nLine=0;nLine<m_nNumberLines;nLine++) {
				nSrc = nLine * m_nSamplesLine;
				nDest = nLine * (m_nSamplesLine - nNumberSamples);
				CopyMemory(&m_cAmpBuffer[gg][nDest], &m_cAmpBuffer[gg][nSrc], (m_nSamplesLine  - nNumberSamples));
				m_vLineSample[nLine].DeleteLastSamples(nNumberSamples);
			}
		}
	}
	m_nSamplesLine -= nNumberSamples;

}

void CData::CropLeftEdge(int nNumberSamples)
{
	int gg,nLine,nSrc,nDest;
	

	for(gg=0;gg<NUMBER_BUFFERS;gg++) {
		if(m_cAmpBuffer[gg]) {
			for(nLine=0;nLine<m_nNumberLines;nLine++) {
				nSrc = (nLine * m_nSamplesLine) + nNumberSamples;
				nDest = nLine * (m_nSamplesLine - nNumberSamples);
				CopyMemory(&m_cAmpBuffer[gg][nDest], &m_cAmpBuffer[gg][nSrc], (m_nSamplesLine  - nNumberSamples));
				m_vLineSample[nLine].DeleteFirstSamples(nNumberSamples);
			}
		}
	}
	m_nSamplesLine -= nNumberSamples;

}

void CData::FindLineMaxXExtent(int nWhich, float *pfMin, float *pfMax)
{
	float fMin=20000.0;
	float fMax=-20000.0f;
	CCoord Cp;

	for(int nLine=0;nLine<m_nLineBladeL[ nWhich ];nLine++) {
		for(int nSample=0;nSample < m_LineBlade[ nWhich ][nLine].m_nCoordL;nSample++) {
			Cp = m_LineBlade[ nWhich ][nLine].m_pCp[nSample];
			if(fMin > Cp.Side0.fX) fMin = Cp.Side0.fX;
			if(fMax < Cp.Side0.fX) fMax = Cp.Side0.fX;
		}
	}
	*pfMin = fMin;
	*pfMax = fMax;
}

bool CData::SaveBladeXML(CUSLFile *pFile)
{
	char str[256];
	char nn = 0;
	D3DXMATRIXA16 matInv;


	char strHeaderName[] = "Ultrasonic Sciences Ltd USLScanner Blade Scan Data\r\n";
	pFile->Write(strHeaderName, strlen(strHeaderName));

	sprintf_s(str,"%.02f\r\n",theApp.m_fVersion);
	pFile->Write(str, strlen(str));


	for(int nCad = 0 ; nCad < 4;nCad++ ) {
		D3DXMatrixInverse(&matInv, NULL, &m_Matrix[nCad]);

		if(m_LineBlade[nCad]) {

			switch(nCad) {
			case 0: strcpy_s(str,"Suction Swept\r\n");
				break;
			case 1: strcpy_s(str,"Suction Parallel\r\n");
				break;
			case 2: strcpy_s(str,"Pressure Swept\r\n");
				break;
			case 3: strcpy_s(str,"Pressure Parallel\r\n");
				break;
			}
			pFile->Write(str,strlen(str));
			
			sprintf_s(str,"Number of scan Lines = %d\r\n",m_nLineBladeL[nCad]);
			pFile->Write(str,strlen(str));

			for(int nLine = 0;nLine < m_nLineBladeL[nCad]; nLine++) {
				sprintf_s(str,"%% %.02f mm\r\n",m_LineBlade[nCad][nLine].m_Matrix.m[3][0]);
				pFile->Write(str,strlen(str));
				for(int nSample = 0;nSample < m_LineBlade[nCad][nLine].m_nCoordL;nSample++) {
					m_LineBlade[nCad][nLine].FormatBladeString(nSample,str,matInv,m_CpTranslation[nCad],true);
					pFile->Write(str,strlen(str));
				}
			}
		}
	}


	strcpy_s(str,"End of human readable section\r\n");
	pFile->Write(str,strlen(str));

	ULONGLONG nPos = pFile->GetPosition();
	nPos += 2048;
	nPos = (nPos / 2048) * 2048;
	pFile->Seek(nPos,CUSLFile::begin);
	pFile->Write(&nn,sizeof nn);

	Save(pFile);

	return true;
}


bool CData::SaveBladeXML(CUSLFile *pFile,int nCad)
{
	char str[256];
	char nn = 0;
	CCoord CpStart,CpFinish;
	D3DXMATRIXA16 matInv;

	char strHeaderName[] = "Ultrasonic Sciences Ltd USLScanner Blade Scan Data\r\n";
	pFile->Write(strHeaderName, strlen(strHeaderName));

	sprintf_s(str,"%.02f\r\n",theApp.m_fVersion);
	pFile->Write(str, strlen(str));


	D3DXMatrixInverse(&matInv, NULL, &m_Matrix[nCad]);

	if(m_LineBlade[nCad]) {

		switch(nCad) {
		case 0: strcpy_s(str,"Suction Swept\r\n");
			break;
		case 1: strcpy_s(str,"Suction Parallel\r\n");
			break;
		case 2: strcpy_s(str,"Pressure Swept\r\n");
			break;
		case 3: strcpy_s(str,"Pressure Parallel\r\n");
			break;
		}
		pFile->Write(str,strlen(str));
			
		sprintf_s(str,"Number of scan Lines = %d\r\n",m_nLineBladeL[nCad]);
		pFile->Write(str,strlen(str));

		for(int nLine = 0;nLine < m_nLineBladeL[nCad]; nLine++) {

			m_LineBlade[nCad][nLine].GetFirstCoord(&CpStart);
			m_LineBlade[nCad][nLine].GetLastCoord(&CpFinish);
			CpStart.TransformCoord(matInv);
			CpFinish.TransformCoord(matInv);

			for(int nSample = 0;nSample < m_LineBlade[nCad][nLine].m_nCoordL;nSample++) {
				m_LineBlade[nCad][nLine].FormatBladeString(nSample,str,matInv,m_CpTranslation[nCad],true);
				pFile->Write(str,strlen(str));
			}
			pFile->Write("%\r\n",strlen("%\r\n"));
		}
	}


	return true;
}


void CData::CopyBladeLinesFromProfile()
{
	for(int nn=0;nn<2;nn++) {
		m_Matrix[ nn ] = PROFILE->m_matBladeLine[nn];
		if(PROFILE->m_LineBlade[nn]) {
			m_nLineBladeL[ nn ] = PROFILE->m_nLineBladeL[ nn];
			m_LineBlade[ nn ] = new CPolyCoord[m_nLineBladeL[ nn ]];
			for(int nLine=0; nLine<m_nLineBladeL[ nn ]; nLine++) {
				m_LineBlade[ nn ][nLine] = &PROFILE->m_LineBlade[nn][nLine];

			}
		}
		m_CpTranslation[nn] = PROFILE->m_CpTranslation[nn];
	}

}

void CData::CollectBladeData()
{
	CCoord CpHead,CpSurface,CpToolCentre;
	int	nGStop,nGStart;
	ADC200Data* pAdc = &theApp.m_UtUser.m_TS[0].Adc;
	GatesData* pGate = &theApp.m_UtUser.m_TS[0].Gate;
	CCoord CpStart,CpFinish;

	m_bModified = TRUE;

	theApp.m_Motors.GetHeadPos(&CpHead);
	theApp.m_Kinematics.NormalsFromTips(&CpHead,PORTSIDE);
	theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpToolCentre,PORTSIDE,TRUE,TRUE);
	theApp.m_Kinematics.NormalsFromTips(&CpToolCentre,PORTSIDE);


	theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,0);
	theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
	theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);


	int nLine = theApp.m_Thread.m_nScanLine;
	CPolyCoord* pLine = &m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine];
	int nSamplesLine = pLine->m_nCoordL;
	m_nCollectAxis = 0;

	pLine->GetCoord(0,&CpStart);
	pLine->GetCoord(nSamplesLine-1,&CpFinish);

	int nSample = (int)((CpToolCentre.fPos[m_nCollectAxis]-CpStart.fPos[m_nCollectAxis]) * (float)(nSamplesLine-1) / (CpFinish.fPos[m_nCollectAxis] - CpStart.fPos[m_nCollectAxis]));
	MinMax(&nSample,0,nSamplesLine-1);

	nSample = pLine->FindNearestCoord(CpToolCentre.fPos[m_nCollectAxis],nSample);
	MinMax(&nSample,0,nSamplesLine-1);

	//check to see if the sample in there is nearest to actual position
	//This really needs to be changed to a vector calculation
	if(fabs(pLine->m_pCp[nSample].Side1.fX - pLine->m_pCp[nSample].Side0.fX) > fabs(CpToolCentre.Side0.fX - pLine->m_pCp[nSample].Side0.fX)) {
		pLine->m_pCp[nSample].Side1 = CpToolCentre.Side0;
		pLine->m_pCp[nSample].Side[2] = CpSurface.Side0;
	}

	nGStop = pGate->nTOFStopGate[1];
	nGStart = pGate->nTOFStartGate[1];
	MinMax(&nGStart,0,7);
	MinMax(&nGStop,0,7);

	int nTime = theApp.m_UtUser.GetGateMaterialTimeNs(0,1);
	MinMax(&nTime,0,9999);
	pLine->m_pCp[nSample].fActThickness = theApp.m_fPath[nTime] / 2.0f;
	pLine->m_pCp[nSample].nAmp = theApp.m_UtUser.m_TS[0].Gate.nAmplitude[0];


}


void CData::ClearLinePriorToScan(int nWhichScanLineType, int nLine)
{
	CPolyCoord* pLine = &m_LineBlade[nWhichScanLineType][nLine];
	int nSamplesLine = pLine->m_nCoordL;

	for(int ii=0;ii<nSamplesLine;ii++) {
		pLine->m_pCp[ii].Side1.fX = -10000.0f;
		pLine->m_pCp[ii].Side1.fY = -10000.0f;
		pLine->m_pCp[ii].Side1.fZ = -10000.0f;
	}

}

bool CData::SaveBladeCSV(CFile *pFile, int nSide, int nLine, bool bEverything_TrafficLights)
{
	char str[256];
	char nn = 0;
	CCoord CpStart,CpFinish;
	D3DXMATRIXA16 matInv;
	float fX;


	sprintf_s(str,"Version,%.02f\r\n",theApp.m_fVersion);
	pFile->Write(str, strlen(str));


//	D3DXMatrixInverse(&matInv, NULL, &m_Matrix[nSide]);
	D3DXMatrixIdentity(&matInv);

	if(m_LineBlade[nSide]) {

		switch(nSide) {
		case 0: strcpy_s(str,"Side,Outside form\r\n");
			break;
		case 1: strcpy_s(str,"Side,Inside form\r\n");
			break;
		}
		pFile->Write(str,strlen(str));
			
		sprintf_s(str,"Line number,%d\r\n",nLine + 1);
		pFile->Write(str,strlen(str));

		sprintf_s(str,"Line name,%s\r\n",m_LineBlade[nSide][nLine].GetCharString());
		pFile->Write(str,strlen(str));

		fX = m_LineBlade[nSide][nLine].m_Matrix.m[3][0];
		sprintf_s(str,"Leading edge translation,%.01f\r\n",fX);
		pFile->Write(str,strlen(str));


		m_LineBlade[nSide][nLine].GetFirstCoord(&CpStart);
		m_LineBlade[nSide][nLine].GetLastCoord(&CpFinish);
		CpStart.TransformCoord(matInv);
		CpFinish.TransformCoord(matInv);

		for(int nSample = 0;nSample < m_LineBlade[nSide][nLine].m_nCoordL;nSample++) {
			m_LineBlade[nSide][nLine].FormatBladeString(nSample,str,matInv,m_CpTranslation[nSide],true);
			pFile->Write(str,strlen(str));
		}
		pFile->Write("%\r\n",strlen("%\r\n"));
	}


	return true;
}

void CData::AutoFillDataProperties()
{
	CString Buff;

	for(int nIndex=0;nIndex<m_DataProperties.GetSize();nIndex++) {

		if(m_DataProperties.GetModeAt(nIndex) & AUTO_INSPECTION_DATE) {
			CTime time(m_osTestDateTime);
			Buff = time.Format(_T("%d %b %Y %Hh%M"));
			m_DataProperties.SetValueAt(nIndex,Buff);
		}

		if(m_DataProperties.GetModeAt(nIndex) & AUTO_OPERATOR) {
			m_DataProperties.SetValueAt(nIndex,m_Operator);
		}

		if(m_DataProperties.GetModeAt(nIndex) & AUTO_FAST_STEPSIZE) {
			Buff.Format(L"%.01f mm",m_fFastIncrement);
			m_DataProperties.SetValueAt(nIndex,Buff);
		}

		if(m_DataProperties.GetModeAt(nIndex) & AUTO_SLOW_STEPSIZE) {
			Buff.Format(L"%.01f mm",m_fSlowIncrement);
			m_DataProperties.SetValueAt(nIndex,Buff);
		}
	}

}



void CData::SaveImageNumber(CUSLFile *pFile, int nImageNumber,int nSentence)
{
	int nOffsetArray[2048];
	ZeroMemory(nOffsetArray,sizeof nOffsetArray);

	UINT64 n64VariablesOffset,n64StringsOffset,n64GlobalOffset,n64TimeSlotOffset,n64DataOffset,n64AnnotationOffset,n64DetailsOffset,n64CoordOffset;
	UINT64 n64ImageNameOffset,n64PolygonOffset,n64LineSampleOffset,n64CADOffset,n64EnvelopeLineOffset,n64BladeOffset,n64BladeLineOffset[4];
	UINT64 n64TranslationOffset,n64InclusionsOffset;

	CArchive* pArchFile;
	CString Buff;
	int nTS,nSize,gg,tt,pp,nn,nLine,nTemp,nImage;
	int	nAmpImages[256],nTimeImages[256];

	ZeroMemory(nAmpImages, sizeof nAmpImages);
	ZeroMemory(nTimeImages, sizeof nTimeImages);
	gg=tt=pp=nImage=0;
	for(nTS=0;nTS<m_nNumberTimeSlots;nTS++) {
		for(nn=0;nn<m_nNumberAmpImages[nTS];nn++) {
			if(nImage == nImageNumber) nAmpImages[0]++;
			nImage++;
		}
		for(nn=0;nn<m_nNumberTimeImages[nTS];nn++) {
			if(nImage == nImageNumber) nTimeImages[0]++;
			nImage++;
		}
		for(nn=0;nn<m_nNumberPhaseImages[nTS];nn++) {
			nImage++;
		}
	}

	m_nSentence = nSentence;

	m_FileName = pFile->GetFileName();
	m_FilePath = pFile->GetFilePath();


	UINT64 n64StartPosition = pFile->GetPosition();

	pFile->Seek(n64StartPosition,CUSLFile::begin);
	pFile->Write(nOffsetArray,sizeof nOffsetArray);
	pFile->Seek(n64StartPosition,CUSLFile::begin);

	pFile->Write(wDataIdentifier,sizeof wDataIdentifier);
	pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);

	pFile->Seek(n64VariablesOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
	pFile->Write(&m_nSamplesLine,sizeof m_nSamplesLine);
	pFile->Write(&m_nNumberLines,sizeof m_nNumberLines);
	pFile->Write(&m_fFastScanSize,sizeof m_fFastScanSize);
	pFile->Write(&m_fSlowScanSize,sizeof m_fSlowScanSize);
	pFile->Write(&m_fFastIncrement,sizeof m_fFastIncrement);
	pFile->Write(&m_fSlowIncrement,sizeof m_fSlowIncrement);

	nTemp = 1;
	pFile->Write(&nTemp,sizeof m_nNumberImages);
	pFile->Write(&nTemp,sizeof m_nNumberTimeSlots);
	pFile->Write(nAmpImages,sizeof nAmpImages);
	pFile->Write(nTimeImages,sizeof nTimeImages);
	pFile->Write(m_nNumberPhaseImages,sizeof m_nNumberPhaseImages);

	pFile->Write(&m_nSentence,sizeof m_nSentence);
	pFile->Write(&m_osTestDateTime,sizeof m_osTestDateTime);
	pFile->Write(&m_nNumberVolumeImages,sizeof m_nNumberVolumeImages);
	pFile->Write(&m_nScanStartLine,sizeof m_nScanStartLine);
	pFile->Write(&m_nScanFinishLine,sizeof m_nScanFinishLine);
	pFile->Write(&m_nLineIncrement,sizeof m_nLineIncrement);
	pFile->Write(&m_nComponentDescription,sizeof m_nComponentDescription);
	pFile->Write(&m_nScanType,sizeof m_nScanType);
	pFile->Write(m_fComponentDiameter,sizeof m_fComponentDiameter);
	pFile->Write(&m_nFastAxis,sizeof m_nFastAxis);
	pFile->Write(&m_nSlowAxis,sizeof m_nSlowAxis);
	pFile->Write(&m_nNumberProbesPerImage,sizeof m_nNumberProbesPerImage);
	pFile->Write(&m_nLineIncrementMode,sizeof m_nLineIncrementMode);
	pFile->Write(&m_nCollectScanLines,sizeof m_nCollectScanLines);
	pFile->Write(&m_nCollectSurface,sizeof m_nCollectSurface);
	pFile->Write(&m_nFlattenMode,sizeof m_nFlattenMode);
	pFile->Write(&m_matCadToScan,sizeof m_matCadToScan);
	pFile->Write(&m_nFirstProbe,sizeof m_nFirstProbe);
	pFile->Write(&m_nNumberExtImages,sizeof m_nNumberExtImages);

	pFile->Seek(n64CoordOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
	m_CpStart.Save(pFile);
	m_CpFinish.Save(pFile);
	m_CpTurnTableCentre.Save(pFile);

	pFile->Seek(n64StringsOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
	pArchFile = new CArchive(pFile,CArchive::store);
	pArchFile->WriteString(m_Operator + _T("\r\n"));
	pArchFile->WriteString(m_TestDateTime + _T("\r\n"));
	pArchFile->WriteString(m_ProfileName + _T("\r\n"));
	pArchFile->WriteString(m_UniqueIdentification + _T("\r\n"));
	pArchFile->WriteString(m_MachineName + _T("\r\n"));
	pArchFile->WriteString(m_MachineLocation + _T("\r\n"));
	pArchFile->Close();
	delete pArchFile;

	pFile->Seek(n64GlobalOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
	pFile->WriteStruct(&m_Global, m_Global.nStructSize=sizeof m_Global);

	pFile->Seek(n64TimeSlotOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
	for(nTS=0;nTS<256;nTS++) {
		pFile->WriteStruct(&m_TS[nTS], m_TS[nTS].nStructSize=sizeof m_TS[0]);
		pFile->Seek(0x200,CUSLFile::current);
	}


	pFile->Seek(n64DataOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
	nSize = m_nSamplesLine * m_nNumberLines;
	gg=tt=pp=nImage=0;
	for(nTS=0;nTS<m_nNumberTimeSlots;nTS++) {
		for(nn=0;nn<m_nNumberAmpImages[nTS];nn++) {
			if(nImage == nImageNumber) pFile->Write(m_cAmpBuffer[gg],nSize * sizeof (unsigned char));
			nImage++; gg++;
		}
		for(nn=0;nn<m_nNumberTimeImages[nTS];nn++) {
			if(nImage == nImageNumber) pFile->Write(m_nTimeBuffer[tt],nSize * sizeof (unsigned int));
			nImage++; tt++;
		}
		for(nn=0;nn<m_nNumberPhaseImages[nTS];nn++) {
			if(nImage == nImageNumber) pFile->Write(m_cPhaseBuffer[pp],nSize * sizeof (unsigned char));
			nImage++; pp++;
		}
	}
	for(nn=0;nn<m_nNumberExtImages;nn++) {
		pFile->Write(m_16ExtBuffer[nn],nSize * sizeof (unsigned __int16));
	}


	pFile->Seek(n64AnnotationOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
	pFile->Write(&m_nAnnotationL,sizeof m_nAnnotationL);
	for(nn=0;nn<m_nAnnotationL;nn++) {
		pFile->WriteStruct(&m_pAnnotation[nn],m_pAnnotation[nn].nStructSize=sizeof ANNOTATION);
	}
	
	pFile->Seek(0x200,CUSLFile::end);
	n64DetailsOffset=pFile->GetPosition();
	m_DataProperties.Save(pFile);


	pFile->Seek(0x200,CUSLFile::end);
	n64ImageNameOffset=pFile->GetPosition();
	pFile->Write(&m_nImageNameL,sizeof m_nImageNameL);
	pArchFile = new CArchive(pFile,CArchive::store);
	for(nn=0;nn<m_nImageNameL;nn++) {
		pArchFile->WriteString(m_ImageName[nn]+_T("\r\n"));
	}
	pArchFile->Close();
	delete pArchFile;

	pFile->Seek(0x200,CUSLFile::end);
	n64PolygonOffset=pFile->GetPosition();
	pFile->Write(&m_nPolygonL,sizeof m_nPolygonL);
	for(nn=0;nn<m_nPolygonL;nn++) {
		m_pPolygon[nn].Save(pFile);
	}

	if(m_vLineSample) {
		pFile->Seek(0x200,CUSLFile::end);
		n64LineSampleOffset=pFile->GetPosition();
		pFile->Write(&m_nNumberLines,sizeof m_nNumberLines);
		for(nn=0;nn<m_nNumberLines;nn++) {
			m_vLineSample[nn].Save(pFile);
		}
	} else {
		n64LineSampleOffset = 0;
	}

	if(m_nCADElementL>0) {
		pFile->Seek(0x200,CUSLFile::end);
		n64CADOffset=pFile->GetPosition();
		pFile->Write(&m_nCADElementL,sizeof m_nCADElementL);
		for(nn=0;nn<m_nCADElementL;nn++) {
			m_pCADElement[nn].Save(pFile);
		}
	} else {
		n64CADOffset = 0;
	}

	//Write the Envelopelines
	pFile->Seek(0x200,CUSLFile::end);
	n64EnvelopeLineOffset=pFile->GetPosition();
	pFile->Write(&m_nEnvelopeLineL,sizeof m_nEnvelopeLineL);
	for(nLine = 0;nLine < m_nEnvelopeLineL; nLine++) m_pEnvelopeLine[nLine].Save(pFile);

	pFile->Seek(0x200,CUSLFile::end);
	n64BladeOffset=pFile->GetPosition();
	for(int nCad=0;nCad<2;nCad++) {
		pFile->Seek(n64BladeLineOffset[nCad]=pFile->GetPosition()+0x200,CUSLFile::begin);
		pFile->Write(&m_nLineBladeL[nCad],sizeof m_nLineBladeL[nCad]);
		for(int nLine=0;nLine<m_nLineBladeL[nCad];nLine++) m_LineBlade[nCad][nLine].Save(pFile);
	};

	pFile->Seek(n64TranslationOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
	for(nn=0;nn<MAX_LINE_TYPES;nn++) {
		m_CpTranslation[nn].Save(pFile);
	};

	pFile->Seek(n64InclusionsOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
	m_Inclusions.Save(pFile);


	//Write the offsets
	pFile->Seek(0x100,CUSLFile::begin);
	pFile->Write(nOffsetArray,0x100);
	pFile->Seek(0x100,CUSLFile::begin);
	pFile->Write(&n64VariablesOffset,sizeof n64VariablesOffset);
	pFile->Write(&n64StringsOffset,sizeof n64StringsOffset);
	pFile->Write(&n64GlobalOffset,sizeof n64GlobalOffset);
	pFile->Write(&n64TimeSlotOffset,sizeof n64TimeSlotOffset);
	pFile->Write(&n64DataOffset,sizeof n64DataOffset);
	pFile->Write(&n64AnnotationOffset,sizeof n64AnnotationOffset);
	pFile->Write(m_n64VolumeOffset,sizeof m_n64VolumeOffset);						//[8]
	pFile->Write(&n64DetailsOffset,sizeof n64DetailsOffset);
	pFile->Write(&n64CoordOffset,sizeof n64CoordOffset);
	pFile->Write(&n64ImageNameOffset,sizeof n64ImageNameOffset);
	pFile->Write(&m_n64ScanLinesOffset,sizeof m_n64ScanLinesOffset);
	pFile->Write(&m_n64ToolPathOffset,sizeof m_n64ToolPathOffset);
	pFile->Write(&n64PolygonOffset,sizeof n64PolygonOffset);
	pFile->Write(&n64LineSampleOffset,sizeof n64LineSampleOffset);
	pFile->Write(&n64CADOffset,sizeof n64CADOffset);
	pFile->Write(&n64EnvelopeLineOffset,sizeof n64EnvelopeLineOffset);
	pFile->Write(&n64BladeOffset,sizeof n64BladeOffset);
	pFile->Write(n64BladeLineOffset,sizeof n64BladeLineOffset);
	pFile->Write(&n64TranslationOffset,sizeof n64TranslationOffset);
	pFile->Write(&n64InclusionsOffset,sizeof n64InclusionsOffset);
	

	pFile->SeekToEnd();

	AppendDataBase();
	theApp.ActivityLog(L"Image saved :" + m_FilePath);

	m_bModified = FALSE;
	m_bHeaderForVolumeIsValid = TRUE;


}


void CData::Rotate90Degrees(CProgressCtrl* pProgress)
{
	int	nSize = m_nSamplesLine * (m_nNumberLines + 1);	//one extra line for extra memory for the line shifting
	unsigned char* pAmpArray = new unsigned char[nSize];
	unsigned int* pTimeArray = new unsigned int[nSize];
	unsigned char	*pcSrc,*pcDest;
	unsigned int *pnSrc,*pnDest;
	int nLine,nSample;

	for(int ii=0;ii<NUMBER_BUFFERS;ii++) {
		if(m_cAmpBuffer[ii]) {
			for(nLine=0;nLine<m_nNumberLines;nLine++) {
				pcSrc = m_cAmpBuffer[ii] + nLine * m_nSamplesLine;
				pcDest = pAmpArray + nLine;
				for(nSample=0;nSample<m_nSamplesLine;nSample++) {
					*pcDest = *pcSrc;
					pcSrc++;
					pcDest+=m_nNumberLines;
				}
			}
			CopyMemory(m_cAmpBuffer[ii], pAmpArray, nSize);
		}
		if(m_nTimeBuffer[ii]) {
			for(nLine=0;nLine<m_nNumberLines;nLine++) {
				pnSrc = m_nTimeBuffer[ii] + nLine * m_nSamplesLine;
				pnDest = pTimeArray + nLine;
				for(nSample=0;nSample<m_nSamplesLine;nSample++) {
					*pnDest = *pnSrc;
					pnSrc++;
					pnDest+=m_nNumberLines;
				}
			}
			CopyMemory(m_nTimeBuffer[ii], pTimeArray, nSize * sizeof (int));
		}
		if(m_16ExtBuffer[ii]) {

		}
	}

	delete pAmpArray;
	delete pTimeArray;


	CUSLFile Src,Dest;
	CFileException	e;
	CString m_DestFileName = theApp.m_StorePath[0] + "\\Temp.dat";
	int nVolumeNumber = 0;
	ULONGLONG n64Src,n64Dest;
	int nSrc,nCount;

	if(m_nNumberVolumeImages>0) {
		if(!Src.Open(m_FilePath, CFile::modeReadWrite | CFile::typeBinary,&e)) {

		} else {
			if(!Dest.Open(m_DestFileName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary, &e)) {

			} else {
				Src.Seek(m_n64VolumeOffset[0],CUSLFile::begin);
				Src.Read(&m_nAscanLength,sizeof m_nAscanLength);
				Dest.SetLength(m_nSamplesLine * m_nAscanLength * m_nNumberLines);

				char *pArray = new char[m_nSamplesLine * m_nAscanLength];
				n64Src = m_n64VolumeOffset[0] + 0x100;
				Src.Seek(n64Src,CUSLFile::begin);
				pProgress->SetRange(0,m_nNumberLines);
				for(nLine=0;nLine<m_nNumberLines;nLine++) {
					pProgress->SetPos(nLine);
					Src.Read(pArray,(UINT64)m_nSamplesLine * (UINT64)m_nAscanLength);

					n64Dest = (UINT64)nLine * (UINT64)m_nAscanLength;
					nSrc = 0;
					for(nSample=0;nSample<m_nSamplesLine;nSample++) {
						Dest.Seek(n64Dest,CUSLFile::begin);
						Dest.Write((PVOID)&pArray[nSrc],m_nAscanLength);
						nSrc += m_nAscanLength;
						n64Dest += ((UINT64)m_nNumberLines * (UINT64)m_nAscanLength);
					}
				}
				delete pArray;
				pProgress->SetPos(0);

				UINT64 n64Size = (UINT64)m_nSamplesLine * (UINT64)m_nAscanLength * (UINT64)m_nNumberLines;
				UINT64 n64Count,n64ReadSize;

				pArray = new char[100000000];
				n64Dest = m_n64VolumeOffset[0] + 0x100;
				Src.Seek(n64Dest,CUSLFile::begin);
				Dest.Seek(0,CUSLFile::begin);
				n64ReadSize = 100000000;

				pProgress->SetRange(0,(int)(n64Size / n64ReadSize));
				for(n64Count=0,nCount=0; n64Count<n64Size;n64Count+=100000000,nCount++) {
					pProgress->SetPos(nCount);
					if((n64Size - n64Count) < 100000000) n64ReadSize = n64Size - n64Count;
					Dest.Read(pArray,(unsigned int)n64ReadSize);
					Src.Write(pArray,(unsigned int)n64ReadSize);
				};
				Dest.Close();
				CFile::Remove(m_DestFileName);

				int nTemp = m_nSamplesLine;
				m_nSamplesLine = m_nNumberLines;
				m_nNumberLines = nTemp;
				nTemp = m_nFastAxis;
				m_nFastAxis = m_nSlowAxis;
				m_nSlowAxis = nTemp;
				float fTemp = m_fSlowIncrement;
				m_fSlowIncrement = m_fFastIncrement;
				m_fFastIncrement = fTemp;
				fTemp = m_fFastScanSize;
				m_fFastScanSize = m_fSlowScanSize;
				m_fSlowScanSize = fTemp;
				m_nMaxViewLine = m_nNumberLines - 1;

				Save(&Src);
				Src.Close();
				delete pArray;
				pProgress->SetPos(0);
			}
		}
	}

}

void CData::ImportAllEnvelopesFromProfile()
{
	int nLine;

	if(PROFILE->m_nEnvelopeLineL) {
		SAFE_DELETE_ARRAY(m_pEnvelopeLine);
		m_pEnvelopeLine = new CPolyCoord[m_nEnvelopeLineL = PROFILE->m_nEnvelopeLineL];
		for(nLine=0;nLine<m_nEnvelopeLineL;nLine++) {
			m_pEnvelopeLine[nLine] = &PROFILE->m_EnvelopeLine[nLine];
			m_pEnvelopeLine[nLine].m_nOffOn = 1;
			m_pEnvelopeLine[nLine].m_rgb = theApp.m_LastSettings.rgbRulerLine;
		}
	}
	m_bThreadEnabled=TRUE;
	m_bThreadFinished = false;
	m_pThread=AfxBeginThread(&CalculateLineAndSampleThread,this,THREAD_PRIORITY_NORMAL ,0,NULL);
}

UINT CalculateLineAndSampleThread(LPVOID pParam)
{
	CData* pData = (CData *)pParam;
	pData->m_bThreadFinished=FALSE;
	int nEnvelopeLine,nSample,nScanLine;
	CCoord Cp;

	int nAxis = pData->m_nCollectAxis;

	for(nEnvelopeLine=0;nEnvelopeLine<pData->m_nEnvelopeLineL;nEnvelopeLine++) {

		for(int nIndex=0;nIndex<pData->m_pEnvelopeLine[nEnvelopeLine].GetSize();nIndex++) {

			pData->m_pEnvelopeLine[nEnvelopeLine].GetCoord(nIndex, &Cp);

			PROFILE->FindScanLineSample(Cp, &nSample, &nScanLine);
	

			//Calculate in the old fashion way incase the two if statements fail
			nSample = (int)((Cp.fPos[nAxis]-pData->m_CpStart.fPos[nAxis]) * (float)(pData->m_nSamplesLine-1) / (pData->m_CpFinish.fPos[nAxis] - pData->m_CpStart.fPos[nAxis]));

	/*
			int nVertexLine  = (nScanLine - pData->m_nScanStartLine) / pData->m_nLineIncrement;
			if(nVertexLine<0) nVertexLine = 0;
			if(nVertexLine>=pData->m_nNumberLines) nVertexLine=pData->m_nNumberLines-1;
			if(pData->m_vLineSample) {
				if(pData->m_vLineSample[nVertexLine].m_nCoordL>2) {
					pData->m_vLineSample[nVertexLine].FindNearestSegment(Cp,PORTSIDE,&nSample);
					nSample += pData->m_vLineSample[nVertexLine].m_nFirstSample;
				}
			}
			*/

			MinMax(&nSample,0,pData->m_nSamplesLine-1);

			pData->m_pEnvelopeLine[nEnvelopeLine].m_pCp[nIndex].nSample = nSample;
			pData->m_pEnvelopeLine[nEnvelopeLine].m_pCp[nIndex].nLine = nScanLine;
		}
	}


	pData->m_bThreadEnabled=false;
	pData->m_bThreadFinished = true;

	return 0;
}


void CData::CopyInclusions(CInclusions *pInclusions)
{
	m_Inclusions = pInclusions;
}

bool CData::BuildInclusionData(int nImageType,int nImageNumber,CRect rrSample)
{

	return m_Inclusions.BuildInclusionData(this, nImageType,nImageNumber,rrSample);
}


void CData::InvalidateVolBytesPerSample()
{
	if(m_nVolBytesPerSample == 0) {
		m_nVolBytesPerSample = 1;
		if(theApp.m_Scope.m_nBits>8) m_nVolBytesPerSample = 2;
	}
}


void CData::RenderOverlayPhaseImage(CPaintDC* pDC,CRect rr,CRect* prrSamples,int nImageNumber,char *pScreenBuffer,char* cColor)
{
	char* pBuffer;
	int	yy,xx,nOffset,nVFilter,xxCount;
	int	xs0=prrSamples->left;
	int	xs1=prrSamples->right;
	int	ys0=prrSamples->top;
	int ys1=prrSamples->bottom;
	int	nLine,s0,s1,c[2],color,ss,ff;
	int src,source[2];
	int nBand,nProbe,nn;
	int* pnMultipleProbeOffset = new int[m_nNumberLines];
	ZeroMemory(pnMultipleProbeOffset,m_nNumberLines * sizeof(int));

	int nBytesPixel = pDC->GetDeviceCaps(BITSPIXEL) / 8;

	if(m_nMinViewLine<0) m_nMinViewLine=0;
	if(m_nMinViewLine>=m_nNumberLines) m_nMinViewLine=m_nNumberLines-1;
	if(m_nMaxViewLine<0) m_nMaxViewLine=0;
	if(m_nMaxViewLine>=m_nNumberLines) m_nMaxViewLine=m_nNumberLines-1;

	int nXs1=prrSamples->right;
	int nXs0=prrSamples->left;

	int nWidth = rr.Width()+2;
	int* m_nXpixelOdd = new int[rr.Width()+10];
	int* m_nXpixelEven = new int[rr.Width()+10];
	for(xx=0;xx<=(rr.Width()+1);xx++) {
		m_nXpixelOdd[xx]=MulDiv(xx,(nXs1-nXs0),rr.Width())+nXs0;
		m_nXpixelEven[xx]=MulDiv(xx,(nXs1-nXs0),rr.Width())+nXs0;
	};
	CPolyCoord lineEdge;
	CCoord Cp;
	for(int ii=0;ii<8;ii++) {
		Cp.Side0.fX = (float)((ii*nWidth)/7);
		Cp.Side0.fY = (float)theApp.m_LastSettings.nImageEdge[ii];
		lineEdge.Add(Cp);
	}
	int nEdge;
	for(xx=1;xx<nWidth;xx++) {
		lineEdge.CalculateCoord((float)xx,Cp);
		nEdge=(int)Cp.Side0.fY;
		m_nXpixelEven[xx] += nEdge;
		if(nWidth>m_nSamplesLine) {
			if((m_nXpixelEven[xx]<m_nXpixelEven[xx-1])) {
				m_nXpixelEven[xx]=m_nXpixelEven[xx-1];
			}
		}
	}
	int nLineBlockEdge = 0;
	if((m_nNumberProbesPerImage>1) && (m_nScanType != MULTI_PROBE_SCAN)) {
		nLineBlockEdge = 1;
		for(nLine=0;nLine<m_nNumberLines;) {
			for(nBand=0,nProbe=0;nBand<4;nBand++) {
				for(nn=0;(nn<32) && (nLine<m_nNumberLines) && (nProbe<m_nNumberProbesPerImage);nn++,nLine++,nProbe++) {
					pnMultipleProbeOffset[nLine] = MulDiv(nn,theApp.m_LastSettings.nPAEdgeOffset[nBand+1],31);
					if((nLine / m_nNumberProbesPerImage) & 1) {
						pnMultipleProbeOffset[nLine] += theApp.m_LastSettings.nPAEdgeDir;
					}
				}
			}
		}
	}

	unsigned char* pData0 = m_cAmpBuffer[nImageNumber];
	int nArrayLength=m_nSamplesLine * m_nNumberLines;

	//Phase stuff added
	int nMask = theApp.m_LastSettings.nPhaseMask;
	int nPositiveAmp = MulDiv(theApp.m_LastSettings.nPhasePositiveAmp, 127, 100) + 128;
	int nNegativeAmp = MulDiv(theApp.m_LastSettings.nPhaseNegativeAmp, 127, 100) + 128;
	unsigned char* pPhaseData = m_cPhaseBuffer[0];
	char cPhase;

	for(yy=0;yy<rr.Height();yy++) {
		if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
			nOffset=yy*rr.Width();
		} else {
			nOffset=(rr.Height()-1-yy)*rr.Width();
		};
		nLine=MulDiv(yy,ys1-ys0,rr.Height())+ys0;
		nVFilter=1;
		switch(theApp.m_LastSettings.nDisplayWhichLines) {
		case 0:
			break;
		case 1:nLine = nLine & 0xfffe;
			break;
		case 2:nLine = (nLine & 0xfffe) + 1;
			if(nLine>=m_nNumberLines) nLine = m_nNumberLines-1;
			break;
		case 3: nVFilter=2;
			break;
		}
		if((nLine>=m_nMinViewLine) && (nLine<m_nMaxViewLine)) {
			source[0] = nLine * m_nSamplesLine;
			source[1] = (nLine + 1) * m_nSamplesLine;
			if(source[1]>nArrayLength) source[1]=source[0];

			pBuffer=&pScreenBuffer[nOffset * nBytesPixel];
			for(xxCount=0;xxCount<=rr.Width();xxCount++) {
				
				if((theApp.m_LastSettings.nImageOrientation==2) || (theApp.m_LastSettings.nImageOrientation==3)){
					xx=(rr.Width()-xxCount);
				}else{
					xx=xxCount;
				}

				if((nLine&1) && (nLineBlockEdge==0)){
					s0=m_nXpixelEven[xx];
					s1=m_nXpixelEven[xx+1];
				} else {
					s0=m_nXpixelOdd[xx];
					s1=m_nXpixelOdd[xx+1];
				}
				s0 += pnMultipleProbeOffset[nLine];
				s1 += pnMultipleProbeOffset[nLine];

				(theApp.m_LastSettings.nAmpPriority==1)?(c[0]=0):(c[0]=0xff);
				c[1]=c[0];
				for(ff=0;ff<nVFilter;ff++) {
					src=source[ff];
					if((s1-s0)>0) {
						for(ss=s0;ss<s1;ss++) {
							if((ss+src)<nArrayLength) {
								if(theApp.m_LastSettings.nAmpPriority==1) {
									if(pData0[ss+src]>c[ff]) c[ff]=pData0[ss+src];
								} else {
									if(pData0[ss+src]<c[ff]) c[ff]=pData0[ss+src];
								}
							} else {
								c[ff]=128;
							}
						};
					} else {
						if((s0+src)<nArrayLength) {
							c[ff]=pData0[s0+src];
						} else {
							c[ff]=128;
						};
					};
					cPhase = pPhaseData[s0+src];
				}
				if(nVFilter==2) {
					color = ((c[0]+c[1])/2) & 0xff;
				} else {
					color = c[0]& 0xff;
				}

				color*=4;
				*pBuffer++=cColor[color];
				*pBuffer++=cColor[color+1];
				*pBuffer++=cColor[color+2];
				if(nBytesPixel>3) pBuffer++;
				color/=4;

				pBuffer -= 4;
				if ((nMask & DISPLAY_PHASE_POSITIVE) && (cPhase == 0)) {
					if ((((nMask & PHASE_POSITIVE_GREATER_LESS) == 0) && (color >= nPositiveAmp)) || ((nMask & PHASE_POSITIVE_GREATER_LESS) && (color <= nPositiveAmp))) {
						*pBuffer++ = char((theApp.m_Scope.m_rgbPosPhaseColor >> 16) & 0xff);
						*pBuffer++ = char((theApp.m_Scope.m_rgbPosPhaseColor >> 8) & 0xff);
						*pBuffer++ = char((theApp.m_Scope.m_rgbPosPhaseColor >> 0) & 0xff);
						pBuffer -= 3;
					}
				}
				if ((nMask & DISPLAY_PHASE_NEGATIVE) && (cPhase == 1)) {
					if ((((nMask & PHASE_NEGATIVE_GREATER_LESS) == 0) && (color >= nNegativeAmp)) || ((nMask & PHASE_NEGATIVE_GREATER_LESS) && (color <= nNegativeAmp))) {
						*pBuffer++ = char((theApp.m_Scope.m_rgbNegPhaseColor >> 16) & 0xff);
						*pBuffer++ = char((theApp.m_Scope.m_rgbNegPhaseColor >> 8) & 0xff);
						*pBuffer++ = char((theApp.m_Scope.m_rgbNegPhaseColor >> 0) & 0xff);
						pBuffer -= 3;
					}
				};
				pBuffer += 4;
			};
		};
	};
	delete pnMultipleProbeOffset;
	delete m_nXpixelOdd;
	delete m_nXpixelEven;
}


void CData::GenerateOneTimeOnlyLineLength()
{
	APP App = (APP)AfxGetApp();
	CCoord CpSurface, Cp;
	D3DXVECTOR3 vect;
	float fRadius, fCircumference;


	if (m_fSampleWidth == NULL && m_fLineLength == NULL) {

		m_fSampleWidth = new float[m_nNumberLines];
		m_fLineLength = new float[m_nNumberLines];
		for (int nLine = 0; nLine < m_nNumberLines; nLine++) {
			m_fLineLength[nLine] = m_fFastIncrement * (float)m_nSamplesLine;
			m_fSampleWidth[nLine] = m_fFastIncrement;
		}

		if (m_ProfileName != PROFILE->m_FileName) return;
		if (PROFILE->m_ScanLine == NULL) return;

		if (m_nFastAxis == 5) {
			for (int nLine = 0; nLine < m_nNumberLines; nLine++) {
				PROFILE->m_ScanLine[0].GetCoord(nLine, &Cp);
				vect = Cp.Side0.pt - theApp.m_Tank.vTTCentre;

				//Assume horizontal turntable
				fRadius = hypot(vect.x, vect.y);
				fCircumference = 2.0f * PIf * fRadius;
				if (nLine == 0) m_fComponentDiameter[0] = fRadius * 2.0f;
				if (nLine == m_nNumberLines - 1) m_fComponentDiameter[1] = fRadius * 2.0f;

				m_fLineLength[nLine] = fCircumference;
				m_fSampleWidth[nLine] = fCircumference / (float)m_nSamplesLine;
			};
			
		}
	}

}


float CData::GetCircumference(int nLine)
{
	float fCircumference, fC0, fC1;

	if (m_fLineLength == NULL) {
		fC0 = m_fComponentDiameter[0] * PIf;
		fC1 = m_fComponentDiameter[1] * PIf;

		fCircumference = ((float)nLine * (fC1 - fC0) / (float)m_nNumberLines) + fC0;
		return fCircumference;

	}
	if (nLine >= 0 && nLine < m_nNumberLines) {
		return m_fLineLength[nLine];
	}
	return m_fComponentDiameter[0];
}

void CData::GetFastScanSizeIncrement(float *pfSize, float *pfIncrement, int nLine)
{
	APP App = (APP)AfxGetApp();

	switch (m_nFastAxis) {
	default:
		*pfSize = m_fFastScanSize;
		*pfSize = m_fFastIncrement * (float)m_nSamplesLine;
		*pfIncrement = m_fFastIncrement;
		break;
	case 5:
	case 6:
		float fCircumference = GetCircumference(nLine);
		switch (theApp.m_LastSettings.nRAxisGraticuleMode) {
		case 0:
		case 3:
			*pfSize = (m_CpFinish.Side0.fR - m_CpStart.Side0.fR);
			*pfIncrement = m_fFastScanSize / (float)m_nSamplesLine;
			break;
		case 1:
			*pfSize = (m_CpFinish.Side0.fR - m_CpStart.Side0.fR) / 360.0f * fCircumference;
			*pfIncrement = fCircumference / (float)m_nSamplesLine;
			break;
		case 2:
			*pfSize = (m_CpFinish.Side0.fR - m_CpStart.Side0.fR) / 360.0f * fCircumference;
			*pfIncrement = fCircumference / (float)m_nSamplesLine;
			break;
		}
		break;
	}

}

float CData::GetAreaOfSample(int nLine)
{
	float fLineLength, fIncrement;

	GetFastScanSizeIncrement(&fLineLength, &fIncrement, nLine);

	return m_fSlowIncrement * fIncrement;

}


void CData::FillForDPCError(int nLine)
{
	if (!(theApp.m_LastSettings.nScanMask & _FILL_DPC_ERROR)) return;
	if (nLine <= 0) return;
	nLine /= m_nLineIncrement;
	if (nLine >= m_nNumberLines) return;
	if (m_cAmpBuffer[0] == NULL) return;

	int nS0 = 10;
	int nS1 = m_nSamplesLine - 10;

	int nTS;
	int nn;
	int nS;

	unsigned int *pTimeBuffer = m_nTimeBuffer[0];
	unsigned char *pAmpBuff = m_cAmpBuffer[0];
	int nOffset = nLine * m_nSamplesLine;

	for (nS0; nS0 < m_nSamplesLine; nS0++) {
		if (pAmpBuff[nOffset + nS0] - INIT_VALUE) break;
	}
	for (nS1; nS1 > 0; nS1--) {
		if (pAmpBuff[nOffset + nS1] - INIT_VALUE) break;
	}

	nOffset = nLine * m_nSamplesLine + nS0;
	for (nS = nS0; nS <= nS1; nS++, nOffset++) {
		if (m_cAmpBuffer[0][nOffset] == INIT_VALUE) {

			int gg = 0;
			int tt = 0;

			for (nTS = 0; nTS < m_nNumberTimeSlots; nTS++) {
				for (nn = 0; nn < m_nNumberAmpImages[nTS]; nn++) {
					m_cAmpBuffer[gg][nOffset] = m_cAmpBuffer[gg][nOffset - m_nSamplesLine];
					gg++;
				}
				for (nn = 0; nn < m_nNumberTimeImages[nTS]; nn++) {
					m_nTimeBuffer[tt][nOffset] = m_nTimeBuffer[tt][nOffset - m_nSamplesLine];
					tt++;
				}
			}
			for (nn = 0; nn < m_nNumberExtImages; nn++) {
				m_16ExtBuffer[nn][nOffset] = m_16ExtBuffer[nn][nOffset - m_nSamplesLine];
			}
		}


	}
}


void CData::DeGlitch(int nLine)
{
	if (!(theApp.m_LastSettings.nScanMask & _DEGLITCH_FILTER)) return;
	if (nLine <= 0) return;
	nLine /= m_nLineIncrement;
	if (nLine >= m_nNumberLines) return;
	if (m_cAmpBuffer[0] == NULL) return;

	int nOffset = nLine * m_nSamplesLine;
	int nDG = theApp.m_LastSettings.nDeGitchThreshold;
	int gg = 2;
	if (m_cAmpBuffer[gg] == NULL) return;

	for (int nS = 0; nS < (m_nSamplesLine-2); nS++, nOffset++) {
		if ( (abs(m_cAmpBuffer[gg][nOffset] - m_cAmpBuffer[gg][nOffset + 1]) > nDG) && (abs(m_cAmpBuffer[gg][nOffset+2] - m_cAmpBuffer[gg][nOffset + 1]) > nDG)) {
			m_cAmpBuffer[gg][nOffset + 1] = ((int)m_cAmpBuffer[gg][nOffset] + (int)m_cAmpBuffer[gg][nOffset]) / 2;
		}
	}
}
