// Data.cpp: implementation of the CData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Data.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
WCHAR m_wDataIdentifier[]=L"Ultrasonic Sciences Ltd USLScanner Image Data";

CData::CData()
{
	for(int ii=0;ii<NUMBER_BUFFERS;ii++) {
		m_cAmpBuffer[ii] = NULL;
		m_nTimeBuffer[ii] = NULL;
		m_cPhaseBuffer[ii] = NULL;
		m_cFilterBuffer[ii] = NULL;
		m_16ExtBuffer[ii] = NULL;
	}
	for(ii=0;ii<NUMBER_BSCAN_BUFFERS;ii++) {
		m_cBscanBuffer[ii] = NULL;
	}
	for(ii=0;ii<NUMBER_VOLUME_BUFFERS;ii++) {
		for(int jj=0;jj<256;jj++) { //Multiple lines/probes
			m_cVolumeBuffer[ii][jj] = NULL;
		}
	}
	m_nLineScannedStatus = NULL;

	m_pARGBArray=NULL;

	m_n64FileLength=0;
	ZeroMemory( m_n64VolumeOffset, sizeof m_n64VolumeOffset);
	m_n64ScanLinesOffset = 0;
	m_n64ToolPathOffset = 0;

	m_nAnnotationL = 0;

	m_bThreadEnabled = false;
	m_bThreadFinished = false;



	ZeroMemory(m_nLastPtr, sizeof m_nLastPtr);
	m_nFlattenMode = 0;

	m_nEnvelopeLineL = 0;

	m_nNumberProbesPerImage = 1;
	m_nFirstProbe = 0;

	m_nSentence = 0;

}

CData::~CData()
{

}

bool CData::Open(CString *FileName)
{
	CString FilePath;
	CFile File;
	CFileException e;

	CFileDialog FileDlg(TRUE,_T(".dat"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Data (*.dat)|*.dat|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir="C:\\Images";
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			return false;
		} else {
			File.Close();
			return true;
		}
	}

	return false;
}
