// Inclusions.cpp: implementation of the CInclusions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CInclusions::CInclusions()
{
	m_pDef = NULL;
	m_nDefL = 0;


}

CInclusions::~CInclusions()
{
	SAFE_DELETE( m_pDef );
	m_nDefL = 0;
}

float CInclusions::CalculateMass(PVOID pVoidData)
{
	float fArea, fDepth, fVolume, fOuterRadius, fInnerRadius,fDelay,fWidth;
	float fOuterVolume, fInnerVolume, fLength;
	CData* pData = (CData* )pVoidData;

	switch(GetComponentShape(pData)) {
	default:	//rectilinear
		fArea = ((float)m_rrArea.Width() * pData->m_fFastIncrement) * ((float)m_rrArea.Height() * pData->m_fSlowIncrement);
		fDepth = (float)(pData->m_TS[0].Gate.nNsWidth[1]*theApp.m_LastSettings.nMaterialVelocity)/2e6f;
		fVolume = fArea * fDepth;
		break;
	case 1:
		fLength = (float)m_rrArea.Height() * pData->m_fSlowIncrement;
		fDelay = (float)(pData->m_TS[0].Gate.nNsDelay[1]*theApp.m_LastSettings.nMaterialVelocity)/2e6f;
		fWidth = (float)(pData->m_TS[0].Gate.nNsWidth[1]*theApp.m_LastSettings.nMaterialVelocity)/2e6f;
		fOuterRadius = pData->m_fComponentDiameter[0] / 2.0f - fDelay;
		fInnerRadius = pData->m_fComponentDiameter[0] / 2.0f - fDelay - fWidth;
		fOuterVolume = PIf * (fOuterRadius * fOuterRadius) * fLength;
		fInnerVolume = PIf * (fInnerRadius * fInnerRadius) * fLength;
		fVolume = fOuterVolume - fInnerVolume;
		break;
	}

	m_fTestMass = ((fVolume / 1000.0f) * theApp.m_LastSettings.fMaterialDensity ) /1000.0f; // mm^2 * g/cm^3

	return m_fTestMass;
}





ULONGLONG CInclusions::Save(CUSLFile *pFile)
{
	ULONGLONG n64VariableOffset, n64StringOffset, n64DefectOffset;

	ULONGLONG n64StartOffset = pFile->GetPosition();
	pFile->Write(&theApp.m_fVersion, sizeof theApp.m_fVersion);
	pFile->Write(&n64VariableOffset, sizeof n64VariableOffset);
	pFile->Write(&n64StringOffset, sizeof n64StringOffset);
	pFile->Write(&n64DefectOffset, sizeof n64DefectOffset);


	pFile->Seek(n64VariableOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
	pFile->Write(&m_fTSA, sizeof m_fTSA);
	pFile->Write(&m_fTestMass, sizeof m_fTestMass);
	pFile->Write(&m_fInclusionLength, sizeof m_fInclusionLength);
	pFile->Write(&m_fVolume, sizeof m_fVolume);
	pFile->Write(&m_fTEDA, sizeof m_fTEDA);
	pFile->Write(&m_fTotalLength, sizeof m_fTotalLength);
	pFile->Write(m_fWeight, sizeof m_fWeight);
	pFile->Write(&m_osRefDate, sizeof m_osRefDate);
	pFile->Write(&m_osTestDate, sizeof m_osTestDate);

	pFile->Seek(n64StringOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
	CArchive* pArchFile = new CArchive(pFile,CArchive::store);
	pArchFile->WriteString(m_ProbeName+L"\r\n");
	pArchFile->WriteString(m_RefGain+L"\r\n");
	pArchFile->WriteString(m_TestGain+L"\r\n");
	pArchFile->WriteString(m_RefSensitivityFBH+L"\r\n");
	pArchFile->WriteString(m_RefFBH+L"\r\n");
	pArchFile->WriteString(m_RefStandard+L"\r\n");
	pArchFile->WriteString(m_FocalLength+L"\r\n");
	pArchFile->WriteString(m_FocalDepth+L"\r\n");
	pArchFile->WriteString(m_Customer+L"\r\n");
	pArchFile->WriteString(m_OrderNumber+L"\r\n");
	pArchFile->WriteString(m_Cast+L"\r\n");
	pArchFile->WriteString(m_Grade+L"\r\n");
	pArchFile->WriteString(m_TestSensitivityFBH+L"\r\n");
	pArchFile->Close();
	delete pArchFile;


	pFile->Seek(n64DefectOffset=pFile->GetPosition()+0x200,CUSLFile::begin);
	pFile->Write(&m_nDefL, sizeof m_nDefL);
	for(int ii=0;ii<m_nDefL; ii++) {
		pFile->Write(&m_pDef[ii], sizeof DefectStruct);
	};

	ULONGLONG n64FinishOffset = pFile->GetPosition();

	pFile->Seek(n64StartOffset,CUSLFile::begin);
	pFile->Write(&theApp.m_fVersion, sizeof theApp.m_fVersion);
	pFile->Write(&n64VariableOffset, sizeof n64VariableOffset);
	pFile->Write(&n64StringOffset, sizeof n64StringOffset);
	pFile->Write(&n64DefectOffset, sizeof n64DefectOffset);

	pFile->Seek(n64FinishOffset,CUSLFile::begin);

	return n64StartOffset;

}

void  CInclusions::Retrieve(CUSLFile* pFile)
{
	ULONGLONG n64VariableOffset, n64StringOffset, n64DefectOffset;
	float fVersion;

	SAFE_DELETE( m_pDef );
	m_nDefL = 0;

	pFile->Read(&fVersion, sizeof fVersion);
	if (fVersion <= 0.0f) return;
	if (fVersion < 5.0f) {
		int nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	n64VariableOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	n64StringOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	n64DefectOffset = nOffset;
	}
	else {
		pFile->Read(&n64VariableOffset, sizeof n64VariableOffset);
		pFile->Read(&n64StringOffset, sizeof n64StringOffset);
		pFile->Read(&n64DefectOffset, sizeof n64DefectOffset);
	}

	pFile->Seek(n64VariableOffset,CUSLFile::begin);
	pFile->Read(&m_fTSA, sizeof m_fTSA);
	pFile->Read(&m_fTestMass, sizeof m_fTestMass);
	pFile->Read(&m_fInclusionLength, sizeof m_fInclusionLength);
	pFile->Read(&m_fVolume, sizeof m_fVolume);
	pFile->Read(&m_fTEDA, sizeof m_fTEDA);
	pFile->Read(&m_fTotalLength, sizeof m_fTotalLength);
	pFile->Read(m_fWeight, sizeof m_fWeight);
	pFile->Read(&m_osRefDate, sizeof m_osRefDate);
	pFile->Read(&m_osTestDate, sizeof m_osTestDate);


	pFile->Seek(n64StringOffset,CUSLFile::begin);
	CArchive* pArchFile = new CArchive(pFile,CArchive::load);
	pArchFile->ReadString(m_ProbeName);
	pArchFile->ReadString(m_RefGain);
	pArchFile->ReadString(m_TestGain);
	pArchFile->ReadString(m_RefSensitivityFBH);
	pArchFile->ReadString(m_RefFBH);
	pArchFile->ReadString(m_RefStandard);
	pArchFile->ReadString(m_FocalLength);
	pArchFile->ReadString(m_FocalDepth);
	pArchFile->ReadString(m_Customer);
	pArchFile->ReadString(m_OrderNumber);
	pArchFile->ReadString(m_Cast);
	pArchFile->ReadString(m_Grade);
	pArchFile->ReadString(m_TestSensitivityFBH);
	pArchFile->Close();
	delete pArchFile;


	pFile->Seek(n64DefectOffset,CUSLFile::begin);
	pFile->Read(&m_nDefL, sizeof m_nDefL);
	if(m_nDefL > 0 ) {
		m_pDef = new DefectStruct[m_nDefL];
		for(int ii=0;ii<m_nDefL; ii++) {
			pFile->Read(&m_pDef[ii], sizeof DefectStruct);
		};
	}

}

void CInclusions::operator = (CInclusions* pInclusions)
{
	SAFE_DELETE( m_pDef );
	m_nDefL = 0;

	m_fTSA = pInclusions->m_fTSA;
	m_fTestMass = pInclusions->m_fTestMass;
	m_fInclusionLength = pInclusions->m_fInclusionLength;
	m_fVolume = pInclusions->m_fVolume;
	m_fTEDA = pInclusions->m_fTEDA;
	m_fTotalLength = pInclusions->m_fTotalLength;
	CopyMemory(m_fWeight,pInclusions->m_fWeight, sizeof m_fWeight);
	m_osRefDate = pInclusions->m_osRefDate;
	m_osTestDate = pInclusions->m_osTestDate;


	m_ProbeName = pInclusions->m_ProbeName;
	m_RefGain = pInclusions->m_RefGain;
	m_TestGain = pInclusions->m_TestGain;
	m_RefSensitivityFBH = pInclusions->m_RefSensitivityFBH;
	m_TestSensitivityFBH = pInclusions->m_TestSensitivityFBH;
	m_RefFBH = pInclusions->m_RefFBH;
	m_RefStandard = pInclusions->m_RefStandard;
	m_FocalLength = pInclusions->m_FocalLength;
	m_FocalDepth = pInclusions->m_FocalDepth;
	
	m_nDefL = pInclusions->m_nDefL;
	if(m_nDefL > 0) {
		m_pDef = new DefectStruct[m_nDefL];
		for(int ii=0;ii<m_nDefL; ii++) {
			CopyMemory(&m_pDef[ii], &pInclusions->m_pDef[ii], sizeof DefectStruct);
		};
	}
}

int CInclusions::GetDefectSize()
{

	return m_nDefL;

}

bool CInclusions::IsDefects()
{

	if((m_nDefL > 0) && (m_pDef != NULL) ) {
		return true;
	}

	return false;

}

DefectStruct* CInclusions::GetPtrDefect(int nIndex)
{
	if((nIndex<m_nDefL) && (m_pDef != NULL) ) {
		return &m_pDef[nIndex];
	}

	return NULL;
}


CString CInclusions::GetComponentShapeVerbose(PVOID pVoidData)
{
	CData* pData = (CData*)pVoidData;

	switch(pData->m_nScanType) {
	case TURNTABLE_SCAN: return L"Cylindrical";
		break;
	default: return L"Cuboid";
		break;
	}

}

int CInclusions::GetComponentShape(PVOID pVoidData)
{
	CData* pData = (CData*)pVoidData;

	switch(pData->m_nScanType) {
	case TURNTABLE_SCAN: return 1;
		break;
	default: return 0;
		break;
	}

}

bool CInclusions::BuildInclusionData(PVOID pVoidData, int nImageType,int nImageNumber,CRect rrSample)
{
	int	nLine,nSample,ii,jj,nAmp;
	bool bFlag;
	DefectStruct DefNew;
	CRect rr;
	CPoint pt;
	CData* pData = (CData* )pVoidData;

	if(rrSample.Width() == 0 || rrSample.Height() == 0) return false;

	m_rrArea = rrSample;
	m_rrArea.NormalizeRect();

	m_nDefL=0;
	SAFE_DELETE( m_pDef );
	m_pDef=NULL;

//	int	nThresh = theApp.m_UtUser.m_TS[0].Gate.nDefectThreshold[1];
	int nThresh = _ttoi(m_RefSensitivityFBH);

	for(nLine=m_rrArea.top;nLine<=m_rrArea.bottom;nLine++) {
		for(nSample=m_rrArea.left;nSample<m_rrArea.right;nSample++) {
			unsigned char* pAmpBuffer = pData->m_cAmpBuffer[nImageNumber] + nLine * pData->m_nSamplesLine;
			unsigned int* pnTimeBuffer = pData->m_nTimeBuffer[0];

			nAmp=MulDiv(pAmpBuffer[nSample]-128,100,127);
			if(nAmp>=nThresh) {

				for(ii=0,bFlag=FALSE;ii<m_nDefL;ii++) {
					rr.left=m_pDef[ii].nX[0]-1;
					rr.right=m_pDef[ii].nX[1]+1;
					rr.top=m_pDef[ii].nY[0]-1;
					rr.bottom=m_pDef[ii].nY[1]+1;
					pt.x=nSample;
					pt.y=nLine;
					if((pt.x>=rr.left) && (pt.x<=rr.right) && (pt.y>=rr.top) && (pt.y<=rr.bottom)) {
						bFlag=TRUE;
						m_pDef[ii].nCount++;
						if(m_pDef[ii].nAmp<nAmp) {
							m_pDef[ii].nAmp=nAmp;
							m_pDef[ii].nXs=nSample;
							m_pDef[ii].nYs=nLine;
							m_pDef[ii].fXp = ((float)m_pDef[ii].nXs * pData->m_fFastScanSize) / (float)pData->m_nSamplesLine;
							m_pDef[ii].fYp = ((float)m_pDef[ii].nYs * pData->m_fSlowScanSize) / (float)pData->m_nNumberLines;
							m_pDef[ii].fDepth = (float)pData->GetFilteredTimeThickness(nSample, nLine, 0, 1) / 100.0f;
							SetClass(&m_pDef[ii]);
						}
						if(m_pDef[ii].nX[0]>nSample) m_pDef[ii].nX[0]=nSample;
						if(m_pDef[ii].nX[1]<nSample) m_pDef[ii].nX[1]=nSample;
						if(m_pDef[ii].nY[0]>nLine) m_pDef[ii].nY[0]=nLine;
						if(m_pDef[ii].nY[1]<nLine) m_pDef[ii].nY[1]=nLine;
						m_pDef[ii].nXs = (m_pDef[ii].nX[1] + m_pDef[ii].nX[0]) / 2;
						m_pDef[ii].nYs = (m_pDef[ii].nY[1] + m_pDef[ii].nY[0]) / 2;
					}
				}
				if(bFlag==FALSE) {
					memset(&DefNew,0,sizeof DefNew);
					DefNew.nXs=nSample;
					DefNew.nYs=nLine;
					DefNew.nX[0]=DefNew.nX[1]=DefNew.nXs;
					DefNew.nY[0]=DefNew.nY[1]=DefNew.nYs;
					DefNew.nAmp=nAmp;
					DefNew.nCount=1;
					DefNew.fDepth=0.0;
					DefNew.nConfirmed=0;
					DefNew.fXp = ((float)DefNew.nXs * pData->m_fFastScanSize) / (float)pData->m_nSamplesLine;
					DefNew.fYp = ((float)DefNew.nYs * pData->m_fSlowScanSize) / (float)pData->m_nNumberLines;
					DefNew.fDepth = (float)pData->GetFilteredTimeThickness(nSample, nLine, 0, 1) / 100.0f;
					SetClass(&DefNew);
					AddDefect(DefNew);
				}
			}
		}
	}

	//Check for interlinking areas
	for(int nn=0;nn<2;nn++) { // do it twice because its better
		for(ii=0;ii<m_nDefL;ii++) {
			rr.left=m_pDef[ii].nX[0]-1;
			rr.right=m_pDef[ii].nX[1]+1;
			rr.top=m_pDef[ii].nY[0]-1;
			rr.bottom=m_pDef[ii].nY[1]+1;
			for(jj=ii+1;jj<m_nDefL;jj++) {
				if(((m_pDef[jj].nX[0]>=m_pDef[ii].nX[0]) && (m_pDef[jj].nX[0]<=m_pDef[ii].nX[1]) && (m_pDef[jj].nY[0]>=m_pDef[ii].nY[0]) && (m_pDef[jj].nY[0]<=m_pDef[ii].nY[1])) ||
				   ((m_pDef[jj].nX[1]>=m_pDef[ii].nX[0]) && (m_pDef[jj].nX[1]<=m_pDef[ii].nX[1]) && (m_pDef[jj].nY[0]>=m_pDef[ii].nY[0]) && (m_pDef[jj].nY[0]<=m_pDef[ii].nY[1])) ||
				   ((m_pDef[jj].nX[0]>=m_pDef[ii].nX[0]) && (m_pDef[jj].nX[0]<=m_pDef[ii].nX[1]) && (m_pDef[jj].nY[1]>=m_pDef[ii].nY[0]) && (m_pDef[jj].nY[1]<=m_pDef[ii].nY[1])) ||
				   ((m_pDef[jj].nX[1]>=m_pDef[ii].nX[0]) && (m_pDef[jj].nX[1]<=m_pDef[ii].nX[1]) && (m_pDef[jj].nY[1]>=m_pDef[ii].nY[0]) && (m_pDef[jj].nY[1]<=m_pDef[ii].nY[1]))) {

					if(m_pDef[ii].nX[0]>m_pDef[jj].nX[0]) m_pDef[ii].nX[0]=m_pDef[jj].nX[0];
					if(m_pDef[ii].nX[1]<m_pDef[jj].nX[1]) m_pDef[ii].nX[1]=m_pDef[jj].nX[1];
					if(m_pDef[ii].nY[0]>m_pDef[jj].nY[0]) m_pDef[ii].nY[0]=m_pDef[jj].nY[0];
					if(m_pDef[ii].nY[1]<m_pDef[jj].nY[1]) m_pDef[ii].nY[1]=m_pDef[jj].nY[1];
					if(m_pDef[ii].nAmp<m_pDef[jj].nAmp) m_pDef[ii].nAmp=m_pDef[jj].nAmp;

					m_pDef[ii].nXs = (m_pDef[ii].nX[1] + m_pDef[ii].nX[0]) / 2;
					m_pDef[ii].nYs = (m_pDef[ii].nY[1] + m_pDef[ii].nY[0]) / 2;
					m_pDef[ii].fDepth = (float)pData->GetFilteredTimeThickness(m_pDef[ii].nXs, m_pDef[ii].nYs, 0, 1) / 100.0f;

					SetClass(&m_pDef[ii]);

					DeleteDefect(jj);
				}
			}
		}
	}

	float fRefFBH;
	float fRefSensitivityFBHE;
	float fRefGain;
	float fExtraGain;
	float fTestGain;

	_WTOF(m_RefFBH, fRefFBH);
	_WTOF(m_RefSensitivityFBH, fRefSensitivityFBHE);
	_WTOF(m_RefGain, fRefGain);
	_WTOF(m_TestGain, fTestGain);


	m_fInclusionLength=0.0;
	for(ii=0;ii<m_nDefL;ii++) {
		fExtraGain = (float)(20.0 * log10((double)m_pDef[ii].nAmp / (double)nThresh));
		m_pDef[ii].fGain=fTestGain-fExtraGain;
		m_pDef[ii].fFBH=powf(10.0f,fExtraGain / 40.0f) * fRefFBH;
		if((m_pDef[ii].nX[1]-m_pDef[ii].nX[0])<1) {
			m_pDef[ii].fLength=(float)(m_pDef[ii].nX[1]-m_pDef[ii].nX[0]+2) * pData->m_fFastIncrement;
		} else {
			m_pDef[ii].fLength=(float)(m_pDef[ii].nX[1]-m_pDef[ii].nX[0]+1) * pData->m_fFastIncrement;
		}
		m_fInclusionLength+=m_pDef[ii].fLength;
	}




	return true;
}



void CInclusions::AddDefect(DefectStruct NewDef)
{

	m_nDefL++;
	if(m_nDefL==1) {
		m_pDef = new DefectStruct[m_nDefL];
	} else {
		m_pDef = (DefectStruct*)realloc(m_pDef,m_nDefL * sizeof DefectStruct);
	}
	int nn=m_nDefL-1;
	m_pDef[nn]=NewDef;

}

void CInclusions::DeleteDefect(int nIndex)
{
	int	nn;

	if(m_nDefL>1) {
		m_nDefL--;
		for(nn=nIndex; nn<m_nDefL ;nn++) {
			m_pDef[nn]=m_pDef[nn+1];
		};
		m_pDef = (DefectStruct *)realloc(m_pDef,m_nDefL * sizeof DefectStruct);
	} else {
		m_nDefL=0;
		SAFE_DELETE( m_pDef );
	}

}

void CInclusions::SetClass(DefectStruct *pDef)
{
	for(int ii=0;ii<3;ii++) {
		if(pDef->nAmp >= theApp.m_LastSettings.fClassFSH[ii]) {
			pDef->nClass=ii+1;
		}
	}
}

float CInclusions::CalculateTSA(PVOID pVoidData)
{
	int nClass[8];

	nClass[0]=nClass[1]=nClass[2]=0;

	for(int ii=0;ii<m_nDefL;ii++) {
		if(m_pDef[ii].nClass>0) {
			nClass[m_pDef[ii].nClass-1]++;
		}
	}
	m_fWeight[0]=(float)nClass[0]*theApp.m_LastSettings.fWeightFSH[0];
	m_fWeight[1]=(float)nClass[1]*theApp.m_LastSettings.fWeightFSH[1];
	m_fWeight[2]=(float)nClass[2]*theApp.m_LastSettings.fWeightFSH[2];

	float fWeightedIndex=m_fWeight[0]+m_fWeight[1]+m_fWeight[2];

	if(m_fTestMass<0.0) m_fTestMass=0.0;
	m_fTSA = (fWeightedIndex * 10.0f) / m_fTestMass;

	return m_fTSA;
}


float CInclusions::CalculateVolume(PVOID pVoidData)
{
	CData* pData = (CData* )pVoidData;
	float fArea = ((float)m_rrArea.Width() * pData->m_fFastIncrement) * ((float)m_rrArea.Height() * pData->m_fSlowIncrement);
	float fDepth = (float)(theApp.m_UtUser.m_TS[0].Gate.nNsWidth[1]*theApp.m_LastSettings.nMaterialVelocity)/2e6f;
	float fVolume = fArea * fDepth / 1e6f;

	return 	(m_fTestMass / theApp.m_LastSettings.fMaterialDensity) / 1e6f ;

}

float CInclusions::CalculateTotalEquivelantDefectArea(PVOID pVoidData)
{
	float	fFbhTotal=0.0;
	float  fLengthTotal=0.0;
	float 	fEDATotal=0.0;

	for(int ii=0;ii<m_nDefL;ii++) {
		fFbhTotal += m_pDef[ii].fFBH;
		fLengthTotal += m_pDef[ii].fLength;
		fEDATotal += (m_pDef[ii].fLength*m_pDef[ii].fFBH);
	}
	return fEDATotal;

}


void CInclusions::SaveToTextFile(CUSLFile* pFile, PVOID pVoidData)
{
	CData* pData = (CData* )pVoidData;
	CString Buff;
	CString Sep;
	CString Temp;
	double dVolume = CalculateVolume(pData);
	int ii;
	float fFbhTotal = 0.0;
	float fLengthTotal = 0.0;
	float fEDATotal = 0.0;
	DefectStruct* pDef;

	switch(theApp.m_LastSettings.nTextSeparator) {
	default: Sep = " ";
		break;
	case 1: Sep = ",";
		break;
	case 2: Sep = "\t";
		break;
	}

	CArchive archFile(pFile, CArchive::store);

		Buff = L"Cast" + Sep + m_Cast + L"\r\n";
		archFile.WriteString(Buff);
		Buff = L"Customer" + Sep + m_Customer + L"\r\n";
		archFile.WriteString(Buff);
		Buff = L"Grade" + Sep + m_Grade + L"\r\n";
		archFile.WriteString(Buff);
		Buff = L"Order Number" + Sep + m_OrderNumber + L"\r\n";
		archFile.WriteString(Buff);
		Buff = L"SampleRef" + Sep + 	m_SampleRef + L"\r\n";
		archFile.WriteString(Buff);
		Buff = L"Focal Depth Fe" + Sep + m_FocalDepth + L"\r\n";
		archFile.WriteString(Buff);
		Buff = L"Focal Length H20" + Sep + m_FocalLength + L"\r\n";
		archFile.WriteString(Buff);
		Buff = L"Probe" + Sep + m_Probe + L"\r\n";
		archFile.WriteString(Buff);
		Buff = L"Ref FBH" + Sep + m_RefFBH + L"\r\n";
		archFile.WriteString(Buff);
		Buff = L"Ref Sensitivity" + Sep + m_RefSensitivityFBH + L"\r\n";
		archFile.WriteString(Buff);
		Temp = m_RefGain;
		Temp.Delete(Temp.Find(_T("dB")),2);
		Buff = L"RefGain" + Sep + Temp + L"\r\n";
		archFile.WriteString(Buff);
		Temp = m_TestGain;
		Temp.Delete(Temp.Find(_T("dB")),2);
		Buff = L"TestGain" + Sep + Temp + L"\r\n";
		archFile.WriteString(Buff);

		CTime time(m_osRefDate);
		Temp = time.Format(_T("Date: %d %b %Y %Hh%M"));
		Buff = L"TestDate" + Sep + Temp + L"\r\n";
		archFile.WriteString(Buff);
		Temp.Format(L"Mass: %.02f",m_fTestMass);
		Buff = L"Test Mass" + Sep + Temp + L"\r\n";
		archFile.WriteString(Buff);
		Temp.Format(L"Mass: %.02f Kg\r\n",m_fTestMass); Buff+=Temp;

		for(ii=0;ii<GetDefectSize();ii++) {
			pDef = GetPtrDefect(ii);
			fFbhTotal += pDef->fFBH;
			fLengthTotal += pDef->fLength;
			fEDATotal += (pDef->fLength * pDef->fFBH);
		}
		Buff.Format(L"FBHTotal: %.02f\r\n",fFbhTotal);										archFile.WriteString(Buff);
		Buff.Format(L"Inclusion length: %.01f mm\r\n",m_fInclusionLength);					archFile.WriteString(Buff);
		Buff.Format(L"TSA/10Kg: %.01f\r\n",m_fTSA);											archFile.WriteString(Buff);
		Buff.Format(L"Volume/dm^3:%.02f\r\n",(m_fTestMass / theApp.m_LastSettings.fMaterialDensity) / 1e6  );	archFile.WriteString(Buff);
		Buff.Format(L"Targets/Kg: %.02f\r\n",(float)m_nDefL / m_fTestMass);					archFile.WriteString(Buff);


		archFile.WriteString(L"\r\n");
		archFile.WriteString(L"Target No., X , Y , Z , Amp , Gain , FBH , Length , EDA \r\n");
		for(ii=0;ii<m_nDefL;ii++) {
			Buff.Format(L"%d%s",ii+1,Sep);							archFile.WriteString(Buff);
			Buff.Format(L"%.01f%s",m_pDef[ii].fXp,Sep);	archFile.WriteString(Buff);
			Buff.Format(L"%.01f%s",m_pDef[ii].fYp,Sep);	archFile.WriteString(Buff);
			Buff.Format(L"%.01f%s",m_pDef[ii].fDepth,Sep);	archFile.WriteString(Buff);
			Buff.Format(L"%d%s",m_pDef[ii].nAmp,Sep);		archFile.WriteString(Buff);
			Buff.Format(L"%.01f%s",m_pDef[ii].fGain,Sep);	archFile.WriteString(Buff);
			Buff.Format(L"%.02f%s",m_pDef[ii].fFBH,Sep);	archFile.WriteString(Buff);
//			if(pData->m_pDef[ii].nAmp==100) {
//				Buff += " **";
//			}
			Buff.Format(L"%.02f%s",m_pDef[ii].fLength,Sep);	archFile.WriteString(Buff);
			Buff.Format(L"%.02f%s",m_pDef[ii].fLength*m_pDef[ii].fFBH,Sep);	archFile.WriteString(Buff);


			Buff = L"\r\n";
			archFile.WriteString(Buff);


		}
		archFile.WriteString(L"\r\n");


	archFile.Close();
}