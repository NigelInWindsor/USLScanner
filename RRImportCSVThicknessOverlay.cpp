// RRImportCSVThicknessOverlay.cpp: implementation of the CRRImportCSVThicknessOverlay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "RRImportCSVThicknessOverlay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRRImportCSVThicknessOverlay::CRRImportCSVThicknessOverlay()
{
	m_nNumberHeaderRows = 0;
	m_nLineNumber = 0;
	m_nNumberLines = 0;
	m_nSamplesLine = 0;
	m_pfThickness = NULL;
	m_pHeaderData = NULL;
	m_bMirror = false;
	m_nOffset = 0;

}

CRRImportCSVThicknessOverlay::~CRRImportCSVThicknessOverlay()
{
	SAFE_DELETE ( m_pfThickness );
	SAFE_DELETE ( m_pHeaderData );

}

bool CRRImportCSVThicknessOverlay::Retrieve(CFile *pFile)
{
	CString Buff,Temp;
	char *pText;
	int nn,nLine,nSample,nColumn;
	WCHAR wc;
	float fValue;
	USES_CONVERSION;


	m_nNumberHeaderRows = 10;
	m_nDataFirstRow = 11;


	m_strTextRows.RemoveAll();

	//read the entire file
	int nFileLength = (int)pFile->GetLength();
	pText = new char[nFileLength+1];
	pFile->Read(pText,nFileLength);
	
	
	//Seperate in to individual rows
	Buff.Empty();
	for(nn=0;nn<nFileLength;nn++) {
		if(isprint(pText[nn])) {
			Buff += pText[nn];
		} else {
			if(Buff.GetLength()>0) {
				m_strTextRows.Add(Buff);
				Buff.Empty();
			}
		}
	}
	if(Buff.GetLength()>0) {
		m_strTextRows.Add(Buff);
		Buff.Empty();
	}

	delete pText;

	//Find how many scan lines
	m_nNumberLines=0;
	Buff = m_strTextRows.GetAt(m_nDataFirstRow);
	for(nn=0;nn<Buff.GetLength();nn++) {
		Temp.Empty();
		for( ;(nn<Buff.GetLength()) && Buff.GetAt(nn)!=',';nn++) {
			Temp += Buff.GetAt(nn);
		}
		m_nNumberLines++;
	}
	m_nNumberLines--;	//Reduce by 1 because the first line is just text
	
	m_nSamplesLine = m_strTextRows.GetSize() - m_nDataFirstRow;

	//extract the header
	if(m_pHeaderData == NULL) {
		m_pHeaderData = new CSVHeaderData[m_nNumberLines];
		ZeroMemory(m_pHeaderData,m_nNumberLines * sizeof (m_pHeaderData));
	}

	for(int nRow=0;nRow<m_nNumberHeaderRows; nRow++) {
		nColumn = 0;
		nLine=0;
		Buff = m_strTextRows.GetAt(nRow);
		for(nn=0;nn<Buff.GetLength();nn++) {
			Temp.Empty();
			for( ;nn<Buff.GetLength();nn++) {
				wc = Buff.GetAt(nn);
				if(wc == ',') break;
				switch(wc) {
				default:Temp += wc;
					break;
				case '*':
					break;
				}
			}
			_WTOF(Temp,fValue);


			if(nColumn>0) {
				if(Temp.GetLength()>1) {
					wc = Temp.GetAt(0);

					if(isdigit(wc) || (wc=='.') || (wc=='-')) {
						switch(nRow) {
						case 0:
							break;
						case 1: m_pHeaderData[nLine].fSection = fValue;
							break;
						case 2: m_pHeaderData[nLine].fLEToTE = fValue;
							break;
						case 3: m_pHeaderData[nLine].fAxisToLE = fValue;
							break;
						case 4: m_pHeaderData[nLine].nNumberSamplesToLE = (int)fValue;
							break;
						case 5: m_pHeaderData[nLine].fLeftOverToLE = fValue;
							break;
						case 6: m_pHeaderData[nLine].fAxisToTE = fValue;
							break;
						case 7: m_pHeaderData[nLine].nNumberSamplesToTE = (int)fValue;
							break;
						case 8: m_pHeaderData[nLine].fLeftOverToTE = fValue;
							break;
						case 9: m_pHeaderData[nLine].fBladeFormAngle = fValue;
							break;
						}
					}
				}
				nLine++;
			}
			nColumn++;
		}
	}


	

	//extract the values and stick them in a square array
	SAFE_DELETE( m_pfThickness );
	m_pfThickness = new float[m_nSamplesLine * m_nNumberLines];
	ZeroMemory(m_pfThickness,m_nSamplesLine * m_nNumberLines * sizeof (float));

	for(nSample = 0;nSample<m_nSamplesLine;nSample++) {
		Buff = m_strTextRows.GetAt(m_nDataFirstRow + nSample);
		nColumn=0;
		nLine=0;
		for(nn=0;nn<Buff.GetLength();nn++) {
			Temp.Empty();
			for( ;nn<Buff.GetLength();nn++) {
				wc = Buff.GetAt(nn);
				if(wc == ',') break;
				switch(wc) {
				default:Temp += wc;
					break;
				case '*':
					break;
				}
			}
			_WTOF(Temp,fValue);
			if(nColumn>0) {
				m_pfThickness[nLine * m_nSamplesLine + nSample] = fValue;
				nLine++;
			}
			nColumn++;
		}
	}


	//Remove empty lines by checking to see if th e2nd number is zero
	for(nLine=0;nLine<m_nNumberLines;nLine++) {
		if(m_pfThickness[nLine * m_nSamplesLine + 1] == 0.0f) {
			m_nNumberLines--;
			CopyMemory((void*)&m_pfThickness[nLine * m_nSamplesLine],(void*)&m_pfThickness[(nLine+1) * m_nSamplesLine],(m_nNumberLines-nLine) * m_nSamplesLine * sizeof (float));
		}
	}

	
	return true;
}

int CRRImportCSVThicknessOverlay::getSamplesLine()
{
	return m_nSamplesLine;
}

void CRRImportCSVThicknessOverlay::getThicknessLine(int nLine,float *pfArray)
{
	if(nLine<0) return;
	if(nLine>=m_nNumberLines) return;

	CopyMemory(pfArray, (void*)&m_pfThickness[nLine * m_nSamplesLine], m_nSamplesLine * sizeof (float));
}

int CRRImportCSVThicknessOverlay::getStackingAxisToLE(int nLine)
{

	return m_pHeaderData[nLine].nNumberSamplesToLE;
}

float CRRImportCSVThicknessOverlay::getBladeFormAngle(int nLine)
{
	return m_pHeaderData[nLine].fBladeFormAngle;

}

CString CRRImportCSVThicknessOverlay::getSection(int nLine)
{
	CString Buff;

	Buff.Format(L"%.0f",m_pHeaderData[nLine].fSection);
	return Buff;

}
