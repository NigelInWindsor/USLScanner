#include "stdafx.h"
#include "FocalLaw.h"
#include "USLScanner.h"


CFocalLawTxRx::CFocalLawTxRx()
{
	m_dWedgeDelay = 0.0;
	m_dCenterDelay = 0.0;
	m_nApertureCount = 1;
	m_dAscanRange = 60e-6f;
	m_dAscanDelay = 0.0f;
	ZeroMemory(m_dwAperture, sizeof m_dwAperture);
	m_dwAperture[0] = 0x01;
	m_nStartElement=0;
	m_nLastElement = 5;
	m_nFocalTOFCount = 1;
	m_fBeamAngle = 0.0f;

	ZeroMemory(m_fTxDelay, sizeof m_fTxDelay);
	ZeroMemory(m_fPulseWidth, sizeof m_fPulseWidth);
	ZeroMemory(m_fRxDelay, sizeof m_fRxDelay);
	for (int nElement = 0; nElement < MAX_APERTURE_COUNT; nElement++) {
		m_fPulseWidth[nElement] = 100e-9f;
	}

}


CFocalLawTxRx::~CFocalLawTxRx()
{
}

void CFocalLawTxRx::setFocalLength(float fLength, int nTOF)
{
	m_fFocalLength[nTOF] = fLength;
}

void CFocalLawTxRx::setBeamAngle(float fAngle)
{
	m_fBeamAngle = fAngle;
}

void CFocalLawTxRx::CalculateTxFocalLaw(PVOID pParent)
{
	CPhasedArrayProbe* pPA = (CPhasedArrayProbe*)pParent;
	float *fArray = new float[m_nApertureCount];
	float fMaxTime = 0.0f;
	float fVelocity = (float)theApp.m_LastSettings.nMaterialVelocity * 1000.0f;
	float fS;
	int nTOF=0;
	ZeroMemory(m_fTxDelay, sizeof m_fTxDelay);

	float fHalfApertureCount = (float)m_nApertureCount / 2.0f - 0.5f;

	if (m_fFocalLength[nTOF] != 0.0f) {
		for (int ii = 0; ii < m_nApertureCount; ii++) {

			float fOpposite = pPA->m_fElementPitch * ((float)ii - fHalfApertureCount);

			float fHypotTime = hypotf(fOpposite, m_fFocalLength[nTOF]) / fVelocity;

			if (fMaxTime < fHypotTime) fMaxTime = fHypotTime;
			fArray[ii] = fHypotTime;
		}
		for (int ii = 0; ii < m_nApertureCount; ii++) {
			m_fTxDelay[ii] = fMaxTime - fArray[ii];
		}
	}

	if (m_fBeamAngle != 0.0f) {
		for (int ii = 0; ii < m_nApertureCount; ii++) {
			if (m_fBeamAngle > 0.0f) {
				fS = (ii * pPA->m_fElementPitch) * sinf(m_fBeamAngle * DEG_TO_RAD);
			}
			else {
				fS = ((m_nApertureCount - 1 - ii) * pPA->m_fElementPitch) * sinf(m_fBeamAngle * DEG_TO_RAD);
			}
			m_fTxDelay[ii] += (fabsf(fS) / fVelocity);
		}
	}

	delete (fArray);
}

void CFocalLawTxRx::CalculateRxFocalLaw(PVOID pParent)
{
	CPhasedArrayProbe* pPA = (CPhasedArrayProbe*)pParent;
	float *fArray = new float[m_nApertureCount];
	float fMaxTime = 0.0f;
	float fVelocity = (float)theApp.m_LastSettings.nMaterialVelocity * 1000.0f;
	float fS;

	ZeroMemory(m_fRxDelay, sizeof m_fRxDelay);

	float fHalfApertureCount = (float)m_nApertureCount / 2.0f - 0.5f;

	m_nFocalTOFCount = 0;
	for (int nTOF = 0; nTOF < 4; nTOF++) {
		if (m_fFocalLength[nTOF] != 0.0f || nTOF == 0) {
			m_nFocalTOFCount++;
			m_dFocalTOFDelay[nTOF] = 0.0;
		}
		if (m_fFocalLength[nTOF] > 0.0f) {
			m_dFocalTOFDelay[nTOF] = (double)(m_fFocalLength[nTOF] / fVelocity);
			fMaxTime = 0.0f;
			for (int ii = 0; ii < m_nApertureCount; ii++) {

				float fOpposite = pPA->m_fElementPitch * ((float)ii - fHalfApertureCount);

				float fHypotTime = hypotf(fOpposite, m_fFocalLength[nTOF]) / fVelocity;

				if (fMaxTime < fHypotTime) fMaxTime = fHypotTime;
				fArray[ii] = fHypotTime;
			}
			for (int ii = 0; ii < m_nApertureCount; ii++) {
				m_fRxDelay[nTOF][ii] = fMaxTime - fArray[ii];
			}
		}

		if (m_fBeamAngle != 0.0f) {
			for (int ii = 0; ii < m_nApertureCount; ii++) {
				if (m_fBeamAngle > 0.0f) {
					fS = (ii * pPA->m_fElementPitch) * sinf(m_fBeamAngle * DEG_TO_RAD);
				}
				else {
					fS = ((m_nApertureCount - 1 - ii) * pPA->m_fElementPitch) * sinf(m_fBeamAngle * DEG_TO_RAD);
				}
				m_fRxDelay[nTOF][ii] += (fabsf(fS) / fVelocity);
			}
		}
	}

	delete (fArray);
}


D3DXVECTOR3 &CFocalLawTxRx::getCenterPos()
{
	return m_vectO;
}

void CFocalLawTxRx::setStartLastElement(int nStart, int nFinish)
{
	m_nStartElement = nStart;
	m_nLastElement = nFinish;
	m_nApertureCount = abs(nFinish - nStart) + 1;
}

int CFocalLawTxRx::getFirstElement()
{
	return m_nStartElement;
}

int CFocalLawTxRx::getLastElement()
{
	return m_nLastElement;
}

float CFocalLawTxRx::getTxDelay(int nElement)
{
	return m_fTxDelay[nElement];
}

float CFocalLawTxRx::getRxDelay(int nTOF, int nElement)
{
	return m_fRxDelay[nTOF][nElement];

}