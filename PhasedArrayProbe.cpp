// PhasedArrayProbe.cpp: implementation of the CPhasedArrayProbe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "PhasedArrayProbe.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPhasedArrayProbe::CPhasedArrayProbe()
{
	Zero();
}

CPhasedArrayProbe::~CPhasedArrayProbe()
{

}

void CPhasedArrayProbe::Zero()
{

	ZeroMemory(m_Element, sizeof m_Element);
	ZeroMemory(m_FL,sizeof m_FL);

	m_fElementPitch = 0.5;
	m_nNumberElements = 32;
	m_nNumberFocalLaws = 32;
	m_nReverseArray = 0;
	m_nFirstElement[TX_FL] = 0;
	m_nLastElement[TX_FL] = 31;
	m_nAperture[TX_FL] = 1;
	m_fFocalLawPitch[TX_FL] = 0.5;
	m_nFocalLawPitch[TX_FL] = (int)(m_fFocalLawPitch[2] / m_fElementPitch);
	m_fAnalogueGain = 20.0f;
	m_fDigitalGain = 0.0f;
	m_fPulseWidth = 100e-9f;
	m_fFilterFreq[0] = 4.0e6f;
	m_fFilterFreq[1] = 6.0e6f;
	m_fRipple = 0.5f;
	m_nFilterGain = 18;


	m_nFocalLawAlgorithm = 0;
	m_nReverseArray = 0;
	m_fTxFocalLength = 1000.0f;
	m_fWedgeAngle = 0.0f;
	m_nWedgeVelocity = 1480;
	m_fWedgeHeightElementOne = 5.0f;
	ZeroMemory(m_fRxFocalLength, sizeof m_fRxFocalLength);
	ZeroMemory(m_fBeamAngle, sizeof m_fBeamAngle);

	setRxEqualTx();
}


bool CPhasedArrayProbe::Save(CString FilePath)
{
	CUSLFile File;
	CFileException e;
	CString Buff, Language;

	if(!File.Open(FilePath, CFile::modeReadWrite | CFile::typeBinary | CFile::modeCreate, &e)) {
		File.DecodeFileException(&e,&Buff,FilePath);
		Language.LoadString(IDS_Error);
		MessageBox(NULL,Buff,Language,MB_ICONERROR);
		return false;
	} else {

		Save( &File );
		File.Close();
	}
	return true;
}

bool CPhasedArrayProbe::Retrieve(CString FilePath)
{
	CUSLFile File;
	CFileException e;
	CString Buff, Language;

	if(!File.Open(FilePath, CFile::modeRead | CFile::typeBinary,&e)) {
		File.DecodeFileException(&e,&Buff,FilePath);
		Language.LoadString(IDS_Error);
		MessageBox(NULL,Buff,Language,MB_ICONERROR);
		return false;
	} else {

		Retrieve( &File );
		File.Close();
	}
	return true;
}

ULONGLONG CPhasedArrayProbe::Save(CUSLFile *pFile)
{
	int	ii;

	m_strFileName = pFile->GetFileName();
	ULONGLONG n64ElementOffset,n64FocalLawOffset;
	ULONGLONG n64StartOffset = pFile->GetPosition();

	pFile->Write(&n64ElementOffset,sizeof n64ElementOffset);
	pFile->Write(&n64FocalLawOffset,sizeof n64FocalLawOffset);

	pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);
	pFile->Write(&m_nNumberElements,sizeof m_nNumberElements);
	pFile->Write(&m_nNumberFocalLaws,sizeof m_nNumberFocalLaws);
	pFile->Write(&m_fElementPitch,sizeof m_fElementPitch);
	pFile->Write(&m_fFocalLawPitch,sizeof m_fFocalLawPitch);
	pFile->Write(m_nAperture,sizeof m_nAperture);
	pFile->Write(m_nFirstElement,sizeof m_nFirstElement);
	pFile->Write(&m_nFocalLawAlgorithm,sizeof m_nFocalLawAlgorithm);
	pFile->Write(&m_nReverseArray,sizeof m_nReverseArray);
	pFile->Write(&m_nFocalLawPitch,sizeof m_nFocalLawPitch);
	pFile->Write(m_nLastElement,sizeof m_nLastElement);
	pFile->Write(&m_fTxFocalLength,sizeof m_fTxFocalLength);
	pFile->Write(&m_fWedgeAngle, sizeof m_fWedgeAngle);
	pFile->Write(m_fBeamAngle, sizeof m_fBeamAngle);
	pFile->Write(m_fRxFocalLength, sizeof m_fRxFocalLength);
	pFile->Write(&m_fAnalogueGain, sizeof m_fAnalogueGain);
	pFile->Write(&m_fDigitalGain, sizeof m_fRxFocalLength);
	pFile->Write(&m_nWedgeVelocity, sizeof m_nWedgeVelocity);
	pFile->Write(&m_fWedgeHeightElementOne, sizeof m_fWedgeHeightElementOne);
	pFile->Write(&m_fWedgeRoofAngle, sizeof m_fWedgeRoofAngle);
	pFile->Write(&m_eProbeType, sizeof m_eProbeType);
	pFile->Write(&m_nFiringOrder, sizeof m_nFiringOrder);
	pFile->Write(&m_nRectification, sizeof m_nRectification);
	pFile->Write(m_fFilterFreq, sizeof m_fFilterFreq);
	pFile->Write(&m_fRipple, sizeof m_fRipple);
	pFile->Write(&m_fStopGain, sizeof m_fStopGain);
	pFile->Write(&m_eFilterType, sizeof m_eFilterType);


	n64ElementOffset=pFile->GetPosition();
	for(ii=0;ii<256;ii++) {
		m_Element[ii].nStructSize = sizeof ElementData;
		m_Element[ii].fVersion = theApp.m_fVersion;
		pFile->WriteStruct(&m_Element[ii],sizeof ElementData);
	};

	n64FocalLawOffset=pFile->GetPosition();
	for(ii=0;ii<257;ii++) {
		m_FL[ii].nStructSize = sizeof FocalLawData;
		m_FL[ii].fVersion = theApp.m_fVersion;
		pFile->WriteStruct(&m_FL[ii],sizeof FocalLawData);
	};

	UINT64 n64Eof=pFile->GetPosition();

	pFile->Seek(n64StartOffset,CFile::begin);
	pFile->Write(&n64ElementOffset,sizeof n64ElementOffset);
	pFile->Write(&n64FocalLawOffset,sizeof n64FocalLawOffset);

	pFile->Seek(n64Eof,CFile::begin);

	return n64StartOffset;
}

void CPhasedArrayProbe::Retrieve(CUSLFile *pFile)
{
	int	ii;
	float fVersion;
	ULONGLONG n64ElementOffset,n64FocalLawOffset;

	Zero();
	m_strFileName = pFile->GetFileName();
	pFile->Read(&n64ElementOffset,sizeof n64ElementOffset);
	pFile->Read(&n64FocalLawOffset,sizeof n64FocalLawOffset);

	pFile->Read(&fVersion,sizeof fVersion);
	pFile->Read(&m_nNumberElements,sizeof m_nNumberElements);
	pFile->Read(&m_nNumberFocalLaws,sizeof m_nNumberFocalLaws);
	pFile->Read(&m_fElementPitch,sizeof m_fElementPitch);
	pFile->Read(m_fFocalLawPitch,sizeof m_fFocalLawPitch);
	pFile->Read(m_nAperture,sizeof m_nAperture);
	pFile->Read(m_nFirstElement,sizeof m_nFirstElement);
	pFile->Read(&m_nFocalLawAlgorithm,sizeof m_nFocalLawAlgorithm);
	pFile->Read(&m_nReverseArray,sizeof m_nReverseArray);
	pFile->Read(&m_nFocalLawPitch,sizeof m_nFocalLawPitch);
	pFile->Read(m_nLastElement,sizeof m_nLastElement);
	pFile->Read(&m_fTxFocalLength,sizeof m_fTxFocalLength);
	pFile->Read(&m_fWedgeAngle, sizeof m_fWedgeAngle);
	pFile->Read(m_fBeamAngle, sizeof m_fBeamAngle);
	pFile->Read(m_fRxFocalLength, sizeof m_fRxFocalLength);
	pFile->Read(&m_fAnalogueGain, sizeof m_fAnalogueGain);
	pFile->Read(&m_fDigitalGain, sizeof m_fRxFocalLength);
	pFile->Read(&m_nWedgeVelocity, sizeof m_nWedgeVelocity);
	pFile->Read(&m_fWedgeHeightElementOne, sizeof m_fWedgeHeightElementOne);
	pFile->Read(&m_fWedgeRoofAngle, sizeof m_fWedgeRoofAngle);
	pFile->Read(&m_eProbeType, sizeof m_eProbeType);
	pFile->Read(&m_nFiringOrder, sizeof m_nFiringOrder);
	pFile->Read(&m_nRectification, sizeof m_nRectification);
	pFile->Read(m_fFilterFreq, sizeof m_fFilterFreq);
	pFile->Read(&m_fRipple, sizeof m_fRipple);
	pFile->Read(&m_fStopGain, sizeof m_fStopGain);
	pFile->Read(&m_eFilterType, sizeof m_eFilterType);


	pFile->Seek(n64ElementOffset,CFile::begin);
	for(ii=0;ii<256;ii++) {
		pFile->ReadStruct(&m_Element[ii], sizeof m_Element[0]);
	};

	pFile->Seek(n64FocalLawOffset,CFile::begin);
	for(ii=0;ii<257;ii++) {
		pFile->ReadStruct(&m_FL[ii], sizeof m_FL[0]);
	};

	CalculateFiringOrder();
}

void CPhasedArrayProbe::setNumberElements(int nNumberElements)
{
	m_nNumberElements = MinMax(&nNumberElements,1,256);
	setNumberFocalLaws(getNumberFocalLaws());
}

int CPhasedArrayProbe::getNumberElements()
{
	return m_nNumberElements;

}

void CPhasedArrayProbe::setRxEqualTx()
{
	m_nFirstElement[RX_FL] = m_nFirstElement[TX_FL];
	m_nLastElement[RX_FL] = m_nLastElement[TX_FL];
	m_nAperture[RX_FL] = m_nAperture[TX_FL];
	m_fFocalLawPitch[RX_FL] = m_fFocalLawPitch[TX_FL];
	m_nFocalLawPitch[RX_FL] = m_nFocalLawPitch[TX_FL];
	m_fBeamAngle[RX_FL][0] = m_fBeamAngle[TX_FL][0];
	m_fBeamAngle[RX_FL][1] = m_fBeamAngle[TX_FL][1];

}

void CPhasedArrayProbe::setNumberFocalLaws(int nNumberFocalLaws)
{
	m_nNumberFocalLaws = MinMax(&nNumberFocalLaws,1,m_nNumberElements);

}

int CPhasedArrayProbe::getNumberFocalLaws()
{
	return m_nNumberFocalLaws;
}


void CPhasedArrayProbe::setElementPitch(float fPitch)
{

	m_fElementPitch = fPitch;

}

float CPhasedArrayProbe::getElementPitch()
{
	return m_fElementPitch;
}

CString CPhasedArrayProbe::getFileName()
{

	return m_strFileName;

}

int CPhasedArrayProbe::getIntTxFocalLawPitch()
{
	return m_nFocalLawPitch[TX_FL];
}


float CPhasedArrayProbe::getTxFocalLawPitch()
{
	return m_fFocalLawPitch[TX_FL];
}

void CPhasedArrayProbe::setTxFocalLawPitch(float fPitch)
{

	//focal law pitch must be a multiple of elementPitch
	m_nFocalLawPitch[TX_FL] = (int)(fPitch / m_fElementPitch);
	MinMax(&m_nFocalLawPitch[TX_FL],1,16);
	m_fFocalLawPitch[TX_FL] = (float)m_nFocalLawPitch[TX_FL] * m_fElementPitch;
}

void CPhasedArrayProbe::setTxFocalLawPitch(int nPitch)
{
	m_nFocalLawPitch[TX_FL] = MinMax(&nPitch,1,16);
	m_fFocalLawPitch[TX_FL] = (float)m_nFocalLawPitch[TX_FL] * m_fElementPitch;
}


int CPhasedArrayProbe::getIntRxFocalLawPitch()
{
	return m_nFocalLawPitch[RX_FL];
}


float CPhasedArrayProbe::getRxFocalLawPitch()
{
	return m_fFocalLawPitch[RX_FL];
}

void CPhasedArrayProbe::setRxFocalLawPitch(float fPitch)
{

	//focal law pitch must be a multiple of elementPitch
	m_nFocalLawPitch[RX_FL] = (int)(fPitch / m_fElementPitch);
	MinMax(&m_nFocalLawPitch[RX_FL], 1, 16);
	m_fFocalLawPitch[RX_FL] = (float)m_nFocalLawPitch[RX_FL] * m_fElementPitch;
}

void CPhasedArrayProbe::setRxFocalLawPitch(int nPitch)
{
	m_nFocalLawPitch[RX_FL] = MinMax(&nPitch, 1, 16);
	m_fFocalLawPitch[RX_FL] = (float)m_nFocalLawPitch[RX_FL] * m_fElementPitch;
}

bool CPhasedArrayProbe::getReverseArray()
{

	if(m_nReverseArray == 1) return true;
	return false;
}

void CPhasedArrayProbe::setReverseArray(bool bFlag)
{
	bFlag == true ? m_nReverseArray = 1 : m_nReverseArray = 0;

}

int CPhasedArrayProbe::getTxAperture()
{
	return m_nAperture[TX_FL];
}

void CPhasedArrayProbe::setTxAperture(int nNumber)
{
	m_nAperture[TX_FL] = MinMax(&nNumber, 1, 16);
}

int CPhasedArrayProbe::getRxAperture()
{
	return m_nAperture[RX_FL];
}

void CPhasedArrayProbe::setRxAperture(int nNumber)
{
	m_nAperture[RX_FL] = MinMax(&nNumber, 1, 16);
}

int CPhasedArrayProbe::getTxFirstElement()
{
	return m_nFirstElement[TX_FL];
}

void CPhasedArrayProbe::setTxFirstElement(int nFirst)
{

	m_nFirstElement[TX_FL] = MinMax(&nFirst, 0, m_nNumberElements - 1);
}

int CPhasedArrayProbe::getRxFirstElement()
{
	return m_nFirstElement[RX_FL];
}

void CPhasedArrayProbe::setRxFirstElement(int nFirst)
{

	m_nFirstElement[RX_FL] = MinMax(&nFirst, 0, m_nNumberElements - 1);
}


void CPhasedArrayProbe::AutoFillCoordinates(char cAxis)
{
	D3DXVECTOR3 vNorm = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int ii;

	for(ii=0;ii<256;ii++) {
		switch(cAxis) {
		case 'X':vPos.x = (float)ii * m_fElementPitch;
			m_Element[ii].vecPt = vPos;
			break;
		case 'Y':vPos.y = (float)ii * m_fElementPitch;
			m_Element[ii].vecPt = vPos;
			break;
		case 'Z':vPos.z = (float)ii * m_fElementPitch;
			m_Element[ii].vecPt = vPos;
			break;
		case 'i': m_Element[ii].vecNorm = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
			break;
		case 'j': m_Element[ii].vecNorm = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
			break;
		case 'k': m_Element[ii].vecNorm = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			break;
		}
	}
}


void CPhasedArrayProbe::setTxLastElement(int nLast)
{
	m_nLastElement[TX_FL] = MinMax(&nLast, m_nFirstElement[TX_FL], m_nNumberElements - 1);
}

int CPhasedArrayProbe::getTxLastElement()
{

	return m_nLastElement[TX_FL];
}

void CPhasedArrayProbe::setRxLastElement(int nLast)
{
	m_nLastElement[RX_FL] = MinMax(&nLast, m_nFirstElement[RX_FL], m_nNumberElements - 1);
}

int CPhasedArrayProbe::getRxLastElement()
{

	return m_nLastElement[RX_FL];
}

float CPhasedArrayProbe::getTxFocalLength()
{
	return m_fTxFocalLength;
}

void CPhasedArrayProbe::setTxFocalLength(float fLength)
{
	m_fTxFocalLength = fLength;
	if (m_fTxFocalLength < 0.0f) m_fTxFocalLength = 0.0f;
	if (m_fTxFocalLength > 1000.0f) m_fTxFocalLength = 1000.0f;
}

float CPhasedArrayProbe::getRxFocalTime(int nFL,int nTOF)
{
	return (float)m_FLRx[nFL].m_dFocalTOFDelay[nTOF];
}

float CPhasedArrayProbe::getRxFocalLength(int nWhich)
{
	return m_fRxFocalLength[nWhich];
}

void CPhasedArrayProbe::setRxFocalLength(float fLength, int nWhich)
{
	m_fRxFocalLength[nWhich] = fLength;
	if (m_fRxFocalLength[nWhich] < 0.0f) m_fRxFocalLength[nWhich] = 0.0f;
	if (m_fRxFocalLength[nWhich] > 1000.0f) m_fRxFocalLength[nWhich] = 1000.0f;
}

int CPhasedArrayProbe::getFocalLawAlgorithm()
{
	return m_nFocalLawAlgorithm;
}

void CPhasedArrayProbe::setFocalLawAlgorithm(int nAlgorithm)
{
	m_nFocalLawAlgorithm = nAlgorithm;
}

bool CPhasedArrayProbe::CalculateDelays(int *nDelayArray)
{

	ZeroMemory(nDelayArray,sizeof(int) * m_nAperture[TX_FL]);
	ZeroMemory(m_fDelayArray_ns,sizeof(float) * m_nAperture[TX_FL]);

	switch(m_nFocalLawAlgorithm) {
	case 0: 
		break;
	case 1: CalculateFocussedDelays(nDelayArray);
		break;
	}

	return true;
}

void CPhasedArrayProbe::CalculateFocussedDelays(int *pnDelayArray)	//delay calculated in ns
{
	int nElement;
	float fOpposite, fHypot;
	float fShortestns = 1000000.0f;
	float fVelocity = (float)theApp.m_LastSettings.nMaterialVelocity / 1e6f;

	for(nElement=0;nElement<m_nAperture[TX_FL];nElement++) {

		fOpposite = m_fElementPitch * (float)m_nAperture[TX_FL] / 2.0f;

		fOpposite = (((float)m_nAperture[TX_FL] / 2.0f) - (float)nElement) * fOpposite / ((float)m_nAperture[TX_FL] / 2.0f) - (m_fElementPitch / 2.0f);

		fHypot = (float)hypot((double)fOpposite, (double)m_fTxFocalLength);

		m_fDelayArray_ns[nElement] = fHypot / fVelocity;
		if(fShortestns > m_fDelayArray_ns[nElement]) fShortestns = m_fDelayArray_ns[nElement];
	}

	for(nElement=0;nElement<m_nAperture[TX_FL];nElement++) {
		m_fDelayArray_ns[nElement] -= fShortestns;
		pnDelayArray[nElement] = (int)m_fDelayArray_ns[nElement];
	}
	
}

float CPhasedArrayProbe::setWedgeAngle(float fAngle)
{
	return m_fWedgeAngle = fAngle;
}

float CPhasedArrayProbe::getWedgeAngle()
{
	return m_fWedgeAngle;
}

int CPhasedArrayProbe::setWedgeVelocity(int nVelocity)
{
	return m_nWedgeVelocity = nVelocity;
}

int CPhasedArrayProbe::getWedgeVelocity()
{
	return m_nWedgeVelocity;
}

float CPhasedArrayProbe::setWedgeHeightElementOne(float fHeight)
{
	return m_fWedgeHeightElementOne = fHeight;
}

float CPhasedArrayProbe::getWedgeHeightElementOne()
{
	return m_fWedgeHeightElementOne;
}

void CPhasedArrayProbe::getDelays(CString *pBuff)
{
	CString Temp,All;
	int nElement;

	pBuff->Empty();
	All.Format(L"Delay / element ");
	
	for(nElement=0;nElement<m_nAperture[TX_FL];nElement++) {
		Temp.Format(L"%0.01f  ",m_fDelayArray_ns[nElement]);
		All += Temp;
	}
	pBuff->Format(L"%s in ns",All);

}


D3DXVECTOR3 &CPhasedArrayProbe::getFocalLawCenterPos(int nTXRx, int nFL)
{
	static D3DXVECTOR3 vect0;

	if (nTXRx == TX_FL) {
		vect0 = (m_vElement[m_FLTx[nFL].getFirstElement()] + m_vElement[m_FLTx[nFL].getLastElement()]) / 2.0f;
	}
	else {
		vect0 = (m_vElement[m_FLRx[nFL].getFirstElement()] + m_vElement[m_FLRx[nFL].getLastElement()]) / 2.0f;
	}
	return vect0;
}

D3DXVECTOR3 &CPhasedArrayProbe::getFocalLawElementPos(int nTXRx, int nFL, int nEl)
{
	static D3DXVECTOR3 vect0;

	if (nTXRx == TX_FL) {
		vect0 = m_vElement[m_FLTx[nFL].getFirstElement() + nEl];
	}
	else {
		vect0 = m_vElement[m_FLRx[nFL].getFirstElement() + nEl];
	}
	return vect0;
}

D3DXVECTOR3 &CPhasedArrayProbe::getElementPos(int nIndex)
{
	return m_vElement[nIndex];
}

float CPhasedArrayProbe::setTxBeamAngle(float fAngle, int StartFinish)
{

	return m_fBeamAngle[TX_FL][StartFinish] = fAngle;
}
float CPhasedArrayProbe::setRxBeamAngle(float fAngle, int StartFinish)
{

	return m_fBeamAngle[RX_FL][StartFinish] = fAngle;
}

float CPhasedArrayProbe::getTxBeamAngle(int StartFinish)
{

	return m_fBeamAngle[TX_FL][StartFinish];
}
float CPhasedArrayProbe::getRxBeamAngle(int StartFinish)
{

	return m_fBeamAngle[RX_FL][StartFinish];
}

float CPhasedArrayProbe::getProbeLength()
{
	return (float)m_nNumberElements * m_fElementPitch;
}

void CPhasedArrayProbe::CalculateElementCoordinates()
{
	ZeroMemory(m_vElement, sizeof m_vElement);

	float fProbeHalfLength = (float)(m_nNumberElements / 2) * m_fElementPitch;

	for (int ii = 0; ii < m_nNumberElements; ii++) {
//		float fH = -fProbeHalfLength + ((m_fElementPitch / 2.0f) + (float)ii * m_fElementPitch);
		float fH = (float)ii * m_fElementPitch;
		m_Element[ii].vecPt = m_vElement[ii] = D3DXVECTOR3(0.0f, 0.0f, fH);
	}
}

void CPhasedArrayProbe::CalculateFocalLawVertices()
{
	int nEl0, nEl1;
	int	nFL;

	for (nFL = 0; nFL <= m_nNumberFocalLaws; nFL++) {
		nEl0 = m_nFirstElement[TX_FL] + nFL * m_nFocalLawPitch[TX_FL];
		nEl1 = nEl0 + m_nAperture[TX_FL] - 1;

		m_FL[nFL].nRXStartElement = m_FL[nFL].nTXStartElement = nEl0;
		m_FL[nFL].nRXFinishElement = m_FL[nFL].nTXFinishElement = nEl1;

		m_FL[nFL].vecPt = (m_Element[nEl0].vecPt + m_Element[nEl1].vecPt) / 2.0f;
		m_FL[nFL].vecNorm = m_Element[(nEl0 + nEl1) / 2].vecNorm;
	}
}


void CPhasedArrayProbe::CalculateTxFocalLaws()
{
	int nEl0, nEl1;
	int	nFL;

	for (nFL = 0; nFL <= m_nNumberFocalLaws; nFL++) {
		nEl0 = m_nFirstElement[TX_FL] + nFL * m_nFocalLawPitch[TX_FL];
		nEl1 = nEl0 + m_nAperture[TX_FL] - 1;

		m_FLTx[nFL].setStartLastElement(nEl0, nEl1);
		m_FLTx[nFL].setFocalLength(m_fTxFocalLength,0);
		m_FLTx[nFL].setBeamAngle(m_fBeamAngle[TX_FL][0]);
		m_FLTx[nFL].CalculateTxFocalLaw(this);
	}

}

void CPhasedArrayProbe::CalculateRxFocalLaws()
{
	int nEl0, nEl1;
	int	nFL;

	for (nFL = 0; nFL <= m_nNumberFocalLaws; nFL++) {
		nEl0 = m_nFirstElement[RX_FL] + nFL * m_nFocalLawPitch[RX_FL];
		nEl1 = nEl0 + m_nAperture[RX_FL] - 1;

		m_FLRx[nFL].setStartLastElement(nEl0, nEl1);
		for(int nTOF=0;nTOF<4;nTOF++)
			m_FLRx[nFL].setFocalLength(m_fRxFocalLength[nTOF],nTOF);
		m_FLRx[nFL].setBeamAngle(m_fBeamAngle[RX_FL][0]);
		m_FLRx[nFL].CalculateRxFocalLaw(this);
	}

}

float CPhasedArrayProbe::getTxDelay(int nFL, int nElement)
{

	return m_FLTx[nFL].getTxDelay(nElement);
}

float CPhasedArrayProbe::getRxDelay(int nFL, int nTOF, int nElement)
{

	return m_FLRx[nFL].getRxDelay(nTOF, nElement);
}

void CPhasedArrayProbe::DownloadAllToHardware()
{
	if (theApp.m_AOSPhasedArray.isConnected()) {
		theApp.m_AOSPhasedArray.setAllHardwareVariables(this);

	}
}

void CPhasedArrayProbe::DownloadTxFocalLawsToHardware()
{
	if (theApp.m_AOSPhasedArray.isConnected()) {
		theApp.m_AOSPhasedArray.setTxFocalLaws(this);
	}
}

void CPhasedArrayProbe::DownloadRxFocalLawsToHardware()
{
	if (theApp.m_AOSPhasedArray.isConnected()) {
		theApp.m_AOSPhasedArray.setRxFocalLaws(this);
	}
}

ULONGLONG CPhasedArrayProbe::getElementEnableMask(int nFL,int nTxRx)
{
	static ULONGLONG u64Mask;
	ULONGLONG uBit = 0x01;
	DWORD dMask = 1;

	u64Mask = 0;
	for (int dW = 0; dW < 4; dW++) {
		dMask = 1;
		for (int nPtr = 0; nPtr<32; nPtr++, uBit <<= 1, dMask <<= 1) {
			if (nTxRx == 0) {
				if (m_FLTx[nFL].m_dwAperture[dW] & dMask) {
					u64Mask |= uBit;
				}
			}
			else {
				if (m_FLRx[nFL].m_dwAperture[dW] & dMask) {
					u64Mask |= uBit;
				}
			}
		}

	}

	return u64Mask;
}

float CPhasedArrayProbe::setAnalogueGain(float fGain, bool bWriteToHardware)
{
	m_fAnalogueGain = fGain;
	if (bWriteToHardware == true && theApp.m_AOSPhasedArray.isConnected()) {
		theApp.m_AOSPhasedArray.setAnalogueGain(m_fAnalogueGain);
	}
	return m_fAnalogueGain;
}

float CPhasedArrayProbe::setDigitalGain(float fGain, bool bWriteToHardware)
{
	m_fDigitalGain = fGain;
	if (bWriteToHardware == true && theApp.m_AOSPhasedArray.isConnected()) {
		theApp.m_AOSPhasedArray.setDigitalGain(m_fDigitalGain);
	}
	return m_fDigitalGain;
}

float CPhasedArrayProbe::setPulseWidth(float fPulseWidth, bool bWriteToHardware)
{
	m_fPulseWidth = fPulseWidth;
	if (bWriteToHardware == true && theApp.m_AOSPhasedArray.isConnected()) {
		theApp.m_AOSPhasedArray.setPulseWidth(this, m_fPulseWidth);
	}
	return m_fPulseWidth;
}

int CPhasedArrayProbe::setPRF(int nPRF, bool bWriteToHardware)
{

	if (bWriteToHardware == true && theApp.m_AOSPhasedArray.isConnected()) {
		theApp.m_AOSPhasedArray.setPRF(this);
	}
	return nPRF;
}

void CPhasedArrayProbe::setRectification(int nRectify, bool bWriteToHardware)
{
	int nType;

	switch (m_nRectification = nRectify) {
	case 0: nType = RFTYPELINEAR;
		break;
	default: nType = RFTYPERECTIFIED;
		break;
	}


	for (int nTS = 0; nTS < 128; nTS++) {
		theApp.m_UtUser.m_TS[nTS].Adc.nRfType = nType;
	}

	if (bWriteToHardware == true && theApp.m_AOSPhasedArray.isConnected()) {
		theApp.m_AOSPhasedArray.setRectify(this);
	}
}

int CPhasedArrayProbe::getRectification()
{
	return m_nRectification;
}

float CPhasedArrayProbe::getAnalogueGain()
{
	return m_fAnalogueGain;
}

float CPhasedArrayProbe::getDigitalGain()
{
	return m_fDigitalGain;
}

float CPhasedArrayProbe::getPulseWidth()
{
	return m_fPulseWidth;
}

void CPhasedArrayProbe::setWidthDelay()
{
	if (theApp.m_AOSPhasedArray.isConnected()) {
		theApp.m_AOSPhasedArray.setWidthDelay(this);
	}
}

void CPhasedArrayProbe::setFiringOrder(int nFiringOrder)
{
	m_nFiringOrder = nFiringOrder;
	CalculateFiringOrder();
}

void CPhasedArrayProbe::setProbeType(int nProbeType)
{
	m_eProbeType = (ProbeType)nProbeType;
}

int CPhasedArrayProbe::getFiringOrder()
{
	return m_nFiringOrder;
}

int CPhasedArrayProbe::getProbeType()
{
	return (int)m_eProbeType;
}

int CPhasedArrayProbe::getFilterType()
{
	return (int)m_eFilterType;
}

int CPhasedArrayProbe::setFilterType(FrequencyFilterType eFilterType)
{
	m_eFilterType = eFilterType;
	return (int)m_eFilterType;
}

void CPhasedArrayProbe::ApplyFilter()
{
	float f1, f2, fr;
	int numBands, i;

	switch (m_eFilterType) {
	default:
		m_eFilterType = FILTER_OFF;
		numBands = 1;
		break;
	case LOWPASS:
	case HIGHPASS:	numBands = 2;
		break;
	case BANDPASS:
	case BANDSTOP:	numBands = 3;
		break;
	}
	double* desired = new double[numBands];
	double* bands = new double[2 * numBands];
	double* weights = new double[numBands];

	if (m_fRipple <= 0.0f) m_fRipple = 0.5f;

	float deltaP = 0.5f*(1.0f - (float)pow(10.0f, -0.05f*m_fRipple));
	float deltaS = (float)pow(10.0f, -0.05f*m_fStopGain);
	float rippleRatio = deltaP / deltaS;

	float fMaxFilterFrequency = 50.0e6f;
	float fRollOffRate = 2.0e6f;

	f1 = 0.5f * m_fFilterFreq[0] / fMaxFilterFrequency;
	f2 = 0.5f * m_fFilterFreq[1] / fMaxFilterFrequency;
	fr = 0.5f * fRollOffRate / fMaxFilterFrequency;
	switch (m_eFilterType) {
	case FILTER_OFF:
		desired[0] = 1.0;
		bands[0] = 0.0;
		bands[1] = 0.5;
		weights[0] = 1.0;
		break;
	case LOWPASS:
		desired[0] = 1.0;
		desired[1] = 0.0;
		bands[0] = 0.0;
		bands[1] = f1;
		bands[2] = f1 + fr;
		bands[3] = 0.5;
		weights[0] = 1.0;
		weights[1] = rippleRatio;
		break;
	case HIGHPASS:
		f1 = 0.5f * m_fFilterFreq[0] / fMaxFilterFrequency;
		f2 = 0.5f * m_fFilterFreq[1] / fMaxFilterFrequency;
		desired[0] = 0.0;
		desired[1] = 1.0;
		bands[0] = 0.0;
		bands[1] = f1 - fr;
		bands[2] = f1;
		bands[3] = 0.5;
		weights[0] = rippleRatio;
		weights[1] = 1.0;
		break;
	case BANDPASS:
		desired[0] = 0.0;
		desired[1] = 1.0;
		desired[2] = 0.0;
		bands[0] = 0.0;
		bands[1] = f1 - fr;
		bands[2] = f1;
		bands[3] = f2;
		bands[4] = f2 + fr;
		bands[5] = 0.5;
		weights[0] = rippleRatio;
		weights[1] = 1.0;
		weights[2] = rippleRatio;
		break;
	case BANDSTOP:
		desired[0] = 1.0;
		desired[1] = 0.0;
		desired[2] = 1.0;
		bands[0] = 0.0;
		bands[1] = f1 - fr;
		bands[2] = f1;
		bands[3] = f2;
		bands[4] = f2 + fr;
		bands[5] = 0.5;
		weights[0] = 1;
		weights[1] = rippleRatio;
		weights[2] = 1;
		break;
	}

	for (i = 0; i<(numBands + 1); i++) {
		if (bands[i]<0.0f) goto finished;
		if (bands[i + 1] <= bands[i]) goto finished;
		if (bands[i]>0.5) goto finished;
	}

	m_FIR.nNumTaps = 64;
	m_FIR.remez(numBands, bands, desired, weights, FIR_BANDPASS);	//FIR_BANDPASS FIR_DIFFERENTIATOR FIR_POSITIVE


finished:
	delete desired;
	delete bands;
	delete weights;

	if (theApp.m_AOSPhasedArray.isConnected()) {
		theApp.m_AOSPhasedArray.setFilter(this);
	}

}

void CPhasedArrayProbe::CalculateFiringOrder()
{
	ZeroMemory(m_nFlOrder, sizeof m_nFlOrder);

	if (m_nFiringOrder < 0) m_nFiringOrder = 0;

	int nOrder = m_nFiringOrder + 1;

	for (int nIndex=0; nIndex < m_nNumberFocalLaws; ) {

		int nFLStart = nIndex / nOrder;

		for (int nLoop = 0; nLoop < nOrder; nLoop++, nIndex++) {

			int nFL = nFLStart + (nLoop * (m_nNumberFocalLaws / nOrder));

			m_nFlOrder[nIndex] = nFL;
		}
	}

}