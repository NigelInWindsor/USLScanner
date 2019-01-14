// VolumeData.cpp: implementation of the CVolumeData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "VolumeData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define	BSCANXY		0 //TODO
#define	BSCANXR		1 //TODO


CVolumeData::CVolumeData()
{
	m_pData = NULL;
	m_pBscanArray = NULL;
	Zero();
}

CVolumeData::~CVolumeData()
{
	if(m_pBscanArray) delete m_pBscanArray;
	m_File.Close();
	Zero();

}

void CVolumeData::Zero()
{

	if(m_pData) delete m_pData;
	m_pData=NULL;
	if(m_pBscanArray) delete m_pBscanArray;
	m_pBscanArray=NULL;

	m_bDataInMemory = FALSE;
	m_ullDataOffset = 0;
	m_nAscanLength = 0;
	m_nNumberLines = 0;
	m_nSamplesLine = 0;
	m_nNumberBscans = 0;
	m_nFileType = GENERIC_VOLUME_VERSION_DOWTY;
	m_FileName.Empty();
	m_bModifiedFlag=FALSE;
}

bool CVolumeData::Retrieve(CString FilePath)
{

	m_FileName = FilePath;
	switch(m_nFileType = CheckFileType(FilePath)) {
	case GENERIC_VOLUME_VERSION_DOWTY:
		RetrieveDowtyAeroSpaceWindows(FilePath);
		break;
	case DOWTY_AEROSPACE_DOS:
		RetrieveDowtyAeroSpaceDos(FilePath);
		break;
	}
	return TRUE;
}

bool CVolumeData::OpenForWriting(CString FilePath)
{
	CString Buff;
	CFileException e;

	if (m_File.Open(FilePath, CFile::typeBinary|CFile::modeReadWrite|CFile::modeCreate, &e)==0)	{
		//m_File.DecodeFileException(&e,&Buff);
		MessageBox(NULL,Buff,_T("Error"), MB_ICONERROR);
		return FALSE;
	} else {
		m_FileName = m_File.GetFileTitle();
	}

	return TRUE;

}

int CVolumeData::CheckFileType(CString FilePath)
{
	CUSLFile File;
	CFileException e;
	CString Buff;
	int nFileType = 0;
	char Array[100];

	if (File.Open(FilePath, CFile::typeBinary|CFile::modeRead, &e)==0)	{
//		File.DecodeFileException(&e,&Buff);
		MessageBox(NULL,Buff,_T("Error"), MB_ICONERROR);
	} else {
		
		File.Read(Array,100);
		if(Array[0]=='B' && Array[1]=='M') {
			nFileType = DOWTY_AEROSPACE_DOS;
		} else {
			nFileType = GENERIC_VOLUME_VERSION_DOWTY;
		}




		File.Close();
	}

	return nFileType;
}

bool CVolumeData::RetrieveDowtyAeroSpaceDos(CString FilePath)
{
	
	CString Buff;
	CFileException e;
	bmp_data bmp;
	header_data	header;

	if (m_File.Open(FilePath, CFile::typeBinary|CFile::modeRead, &e)==0)	{
//		m_File.DecodeFileException(&e,&Buff);
		MessageBox(NULL,Buff,_T("Error"), MB_ICONERROR);
		return FALSE;
	} else {
		Zero();
		m_nFileType = DOWTY_AEROSPACE_DOS;
		m_FileName = m_File.GetFileTitle();

		int	nFileLength = (int)m_File.GetLength();
		int nOffset = sizeof bmp_data + 1024;
		m_File.Read(&bmp,sizeof bmp_data);
		m_File.Seek(nOffset,CFile::begin);
		m_File.Read(&header,sizeof header);

		m_ullDataOffset = bmp.headersize;
		m_nAscanLength = header.samplesline - 24;	//-24 because first 24 bytes contain coordinates;
		m_nNumberLines = header.numberlines;

		m_nMaterialVelocity= theApp.m_LastSettings.nMaterialVelocity;
		m_fScanLength = (header.f_stepsize * (float)header.f_scan_size);
		m_fDepth = (float)(((m_nAscanLength*10) + theApp.m_LastSettings.nMaterialConstant) * m_nMaterialVelocity) / 1e6f;
		m_nNumberBscans = (nFileLength - m_ullDataOffset) / ((m_nAscanLength+24) * m_nNumberLines);

		delete m_pBscanArray;
		m_pBscanArray = new char[m_nAscanLength * m_nNumberLines];


		m_CpStart.dX0p = (double)header.x_start * theApp.m_Axes[X0AXIS].fStepSize;
		m_CpStart.dY0p = (double)header.y_start * theApp.m_Axes[Y0AXIS].fStepSize;

		if(m_bDataInMemory == TRUE) {
			m_File.Close();
		}

		return TRUE;
	}
}

bool CVolumeData::RetrieveDowtyAeroSpaceWindows(CString FilePath)
{
	
	CString Buff;
	CFileException e;
	int	nHeaderOffset;

	if (m_File.Open(FilePath, CFile::typeBinary|CFile::modeRead, &e)==0)	{
//		m_File.DecodeFileException(&e,&Buff);
		MessageBox(NULL,Buff,_T("Error"), MB_ICONERROR);
		return FALSE;
	} else {
		Zero();
		m_nFileType = GENERIC_VOLUME_VERSION_DOWTY;
		m_FileName = m_File.GetFileTitle();

//		m_File.Seek(0x0,CFile::begin);
//		m_File.Write(Identifier,sizeof Identifier);

		m_File.Seek(0x500,CFile::begin);
		m_File.Read(&m_ullDataOffset,sizeof m_ullDataOffset);
		m_File.Read(&nHeaderOffset,sizeof nHeaderOffset);
	
		m_File.Seek(nHeaderOffset,CFile::begin);
		m_File.Read(&m_nSamplesLine,sizeof m_nSamplesLine);
		m_File.Read(&m_nNumberLines,sizeof m_nNumberLines);
		m_File.Read(&m_nAscanLength,sizeof m_nAscanLength);
		m_File.Read(&m_fDepth,sizeof m_fDepth);
		m_File.Read(&m_fScanLength,sizeof m_fScanLength);
		m_File.Read(&m_nNumberBscans,sizeof m_nNumberBscans);
		m_File.Read(&m_nMaterialVelocity,sizeof m_nMaterialVelocity);
		m_File.ReadStruct(&m_CpStart, sizeof m_CpStart);
		m_File.ReadStruct(&m_CpFinish, sizeof m_CpFinish);

		if(m_bDataInMemory == TRUE) {
			m_File.Close();
		}

		delete m_pBscanArray;
		m_pBscanArray = new char[m_nAscanLength * m_nNumberLines];


	}
	return true;

}

void CVolumeData::BuildBscanArray(int nWhich)
{
	ULONGLONG	ullOffset;
	char* pData = NULL;
	int	nLine;
	int nSrcOffset = 0;
	int nDestOffset = 0;

	if(m_bDataInMemory == FALSE) {
		switch(m_nFileType) {
		case GENERIC_VOLUME_VERSION_DOWTY:
			ullOffset = m_ullDataOffset + ((m_nAscanLength+26) * m_nNumberLines) * nWhich + 26;
			m_File.Seek(ullOffset,CFile::begin);
			pData = new char [(m_nAscanLength+26) * m_nNumberLines];
			m_File.Read(pData,(m_nAscanLength+26) * m_nNumberLines);
			for(nLine=0;nLine<m_nNumberLines;nLine++) {
				memcpy(&m_pBscanArray[nDestOffset],&pData[nSrcOffset],m_nAscanLength);
				nDestOffset += m_nAscanLength;
				nSrcOffset += (m_nAscanLength+26);
			}
			break;
		case DOWTY_AEROSPACE_DOS:
			ullOffset = m_ullDataOffset + ((m_nAscanLength+24) * m_nNumberLines) * nWhich;
			m_File.Seek(ullOffset,CFile::begin);
			pData = new char [(m_nAscanLength+24) * m_nNumberLines];
			m_File.Read(pData,(m_nAscanLength+24) * m_nNumberLines);
			for(nLine=0;nLine<m_nNumberLines;nLine++) {
				memcpy(&m_pBscanArray[nDestOffset],&pData[nSrcOffset],m_nAscanLength);
				nDestOffset += m_nAscanLength;
				nSrcOffset += (m_nAscanLength+24);
			}
			break;
		}
	}

	if(pData) delete pData;

}

void CVolumeData::CheckAllBScanVariables()
{
	
	CTime CurrentTime = CTime::GetCurrentTime();
	int	ii;
	double dTemp;


	m_Global=theApp.m_UtUser.m_Global;
	for(ii=0;ii<8;ii++) 
		m_TS[ii]=theApp.m_UtUser.m_TS[ii];

	m_TestDate=CurrentTime.Format("%d, %b, %Y, %Hh%M");

	CalculateNumberOfImages();

//	PROFILE->FindEnvelopeScanLimits(&m_CpStart,&m_CpFinish); //TODO
//	m_nScanAxes = PROFILE->m_nScanDirection; //TODO
	switch(m_nScanAxes) {
	case BSCANXY:
		if((dTemp=m_CpFinish.dX0p)<m_CpStart.dX0p) {
			m_CpFinish.dX0p=m_CpStart.dX0p;
			m_CpStart.dX0p=dTemp;
		};
		if((dTemp=m_CpFinish.dY0p)<m_CpStart.dY0p) {
			m_CpFinish.dY0p=m_CpStart.dY0p;
			m_CpStart.dY0p=dTemp;
		};
		m_dFastScanSize=m_CpFinish.dX0p-m_CpStart.dX0p;
		m_dSlowScanSize=m_CpFinish.dY0p-m_CpStart.dY0p;
		theApp.m_Axes[X0AXIS].nStartPos=(int)(m_CpStart.dX0p/theApp.m_Axes[X0AXIS].fStepSize);
		theApp.m_Axes[X0AXIS].nFinishPos=(int)(m_CpFinish.dX0p/theApp.m_Axes[X0AXIS].fStepSize);
		theApp.m_ServoUser.m_Pmac.nWhichAxis=X0AXIS;
		break;
	case BSCANXR:
		if((dTemp=m_CpFinish.dX0p)<m_CpStart.dX0p) {
			m_CpFinish.dX0p=m_CpStart.dX0p;
			m_CpStart.dX0p=dTemp;
		};
		if((dTemp=m_CpFinish.dR0p)<m_CpStart.dR0p) {
			m_CpFinish.dR0p=m_CpStart.dR0p;
			m_CpStart.dR0p=dTemp;
		};
		m_dFastScanSize=m_CpFinish.dX0p-m_CpStart.dX0p;
		m_dSlowScanSize=m_CpFinish.dR0p-m_CpStart.dR0p;
		theApp.m_Axes[X0AXIS].nStartPos=(int)(m_CpStart.dX0p/theApp.m_Axes[X0AXIS].fStepSize);
		theApp.m_Axes[X0AXIS].nFinishPos=(int)(m_CpFinish.dX0p/theApp.m_Axes[X0AXIS].fStepSize);
		theApp.m_ServoUser.m_Pmac.nWhichAxis=X0AXIS;
		break;
	};
	m_dFastIncrement = PROFILE->m_fFastIncrement;
	m_dSlowIncrement = PROFILE->m_fSlowIncrement;
	if(m_dFastIncrement<=0.0) m_dFastIncrement=m_dFastScanSize/1000;
	if(m_dSlowIncrement<=0.0) m_dSlowIncrement=m_dSlowScanSize/1000;

	theApp.m_ServoUser.m_Pmac.nSamplesLine = (int)(m_dFastScanSize/m_dFastIncrement);
	m_nSamplesLine=m_nAscanLength + 26;
	m_nNumberLines=(int)(m_dFastScanSize/m_dFastIncrement);

	m_nSamplePeriod=theApp.m_UtUser.m_TS[0].Adc.nSamplePeriod;
	m_nMaterialVelocity= theApp.m_LastSettings.nMaterialVelocity;
	m_fScanLength = (float)m_dFastScanSize;
	m_fDepth = (float)(((m_nAscanLength*m_nSamplePeriod) + theApp.m_LastSettings.nMaterialConstant) * m_nMaterialVelocity) / 2e6f;
	m_nNumberBscans = (int)(m_dSlowScanSize/m_dSlowIncrement);

	delete m_pBscanArray;
	m_pBscanArray = new char[m_nAscanLength * m_nNumberLines];

	m_nFileType = GENERIC_VOLUME_VERSION_DOWTY;
	m_nSentence = 0;
	m_bModifiedFlag = TRUE;


}

void CVolumeData::CalculateNumberOfImages()
{
	


	//Calculate how many images
	m_nNumberImages=0;
	m_nNumberAmpImages=0;
	m_nNumberTimeImages=0;
	theApp.m_ServoUser.m_Pmac.nNumberAmpGates[0]=0;
	theApp.m_ServoUser.m_Pmac.nNumberTimeGates[0]=0;
/*
	for(gg=0;gg<8;gg++) {
		if(theApp.m_UtUser.m_TS[0].Gate.nCollectAmp[gg]) {
			m_nNumberImages++;
			m_nNumberAmpImages++;
			theApp.m_ServoUser.m_Pmac.nNumberAmpGates[0]++;
		};
	};
	if(theApp.m_UtUser.m_TS[0].Gate.nTimsMode[0]>0) {
		m_nNumberImages++;
		m_nNumberTimeImages++;
		theApp.m_ServoUser.m_Pmac.nNumberTimeGates[0]++;
	};

	for(gg=0;gg<m_nNumberAmpImages;gg++) {
		if(m_Buffer[gg]) delete m_Buffer[gg];
		m_Buffer[gg] = new unsigned char[m_nSamplesLine*m_nNumberLines];
		memset(m_Buffer[gg],128,m_nSamplesLine*m_nNumberLines);
		m_nRfDataType[gg]=RFTYPELOG;
	};
	if(m_nNumberTimeImages>0) {
		if(m_nTimeBuffer[0]) delete m_nTimeBuffer[0];
		m_nTimeBuffer[0] = new unsigned int[m_nSamplesLine*m_nNumberLines];
		memset(m_nTimeBuffer[0],0,m_nSamplesLine*m_nNumberLines*sizeof(int));
	};
*/
	m_bCollectAscans = /*PROFILE->m_bCollectAscans =*/ TRUE; //TODO
	if(m_bCollectAscans==TRUE) {
		theApp.m_UtUser.AllocateAndShareBscanMemory();
		switch(theApp.m_UtUser.m_TS[0].Adc.nBscanMode) {
		case 0:
			break;
		case 1:	m_nAscanLength=theApp.m_UtUser.m_TS[0].Adc.nAcqLength;
			break;
		case 2:	m_nAscanLength=(theApp.m_UtUser.m_TS[0].Adc.nBscanPreTrigger+theApp.m_UtUser.m_TS[0].Adc.nBscanPostTrigger)/theApp.m_UtUser.m_TS[0].Adc.nSamplePeriod;
			break;
		}
		theApp.m_UtUser.m_TS[0].Adc.nBscanLength=m_nAscanLength+26;
	}


//	theApp.m_ServoUser.m_Pmac.nJobNumber = theApp.m_LastSettings.nJobNumber;
}

void CVolumeData::SaveScanData(int nScanLine)
{
	
	double dDataVersion=2.1;
	char Identifier[]={"Ultrasonic Sciences Ltd Dowty Bscan"};
	CString	FileVolume = m_FileName;
	int	nOffset,nHeaderOffset;

	m_ullDataOffset = 0x10000;

	m_File.Seek(0x0,CFile::begin);
	m_File.Write(Identifier,sizeof Identifier);

	m_File.Seek(nHeaderOffset=0x1000,CFile::begin);
	m_File.Write(&m_nSamplesLine,sizeof m_nSamplesLine);
	m_File.Write(&m_nNumberLines,sizeof m_nNumberLines);
	m_File.Write(&m_nAscanLength,sizeof m_nAscanLength);
	m_File.Write(&m_fDepth,sizeof m_fDepth);
	m_File.Write(&m_fScanLength,sizeof m_fScanLength);
	m_File.Write(&m_nNumberBscans,sizeof m_nNumberBscans);
	m_File.Write(&m_nMaterialVelocity,sizeof m_nMaterialVelocity);
	m_File.WriteStruct(&m_CpStart,m_CpStart.nStructSize=sizeof CoordStructGeneric);
	m_File.WriteStruct(&m_CpFinish,m_CpFinish.nStructSize=sizeof CoordStructGeneric);


	nOffset = m_ullDataOffset + (nScanLine * (m_nNumberLines * (m_nAscanLength+26)));
	m_File.Seek(nOffset,CFile::begin);
	m_File.Write(theApp.m_UtUser.m_BscanArray, m_nNumberLines * (m_nAscanLength+26));
	memset(theApp.m_UtUser.m_BscanArray,0, m_nNumberLines * (m_nAscanLength+26));

	m_File.Seek(0x500,CFile::begin);
	m_File.Write(&m_ullDataOffset,sizeof m_ullDataOffset);
	m_File.Write(&nHeaderOffset,sizeof nHeaderOffset);
}

void CVolumeData::ChangeVelocityDepth(int nVelocity)
{

	m_fDepth = m_fDepth * ((float)m_nMaterialVelocity / (float)nVelocity);
	m_nMaterialVelocity = nVelocity;

}


void CVolumeData::Rename(LPCTSTR lpszNewFilePath)
{
	CFileException e;

	m_FilePath = m_File.GetFilePath();
	m_File.Close();
	m_bModifiedFlag=FALSE;

	CFile::Rename(m_FilePath,lpszNewFilePath);

	m_File.Open(lpszNewFilePath, CFile::typeBinary|CFile::modeReadWrite|CFile::modeNoTruncate, &e);
	m_FilePath = m_File.GetFilePath();
	m_FileName = m_File.GetFileTitle();
}
