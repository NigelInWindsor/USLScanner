#include "stdafx.h"
#include "FocalLawTx.h"
#include "USLScanner.h"


CFocalLawTx::CFocalLawTx()
{
	m_dWedgeDelay = 0.0;
	m_dCenterDelay = 0.0;
	m_nApertureCount = 1;
	ZeroMemory(m_dwAperture, sizeof m_dwAperture);
	m_dwAperture[0] = 0x01;

	ZeroMemory(m_fPulseDelay, sizeof m_fPulseDelay);
	ZeroMemory(m_fPulseWidth, sizeof m_fPulseWidth);
	for (int nElement = 0; nElement < MAX_APERTURE_COUNT; nElement++) {
		m_fPulseWidth[nElement] = 100e-9f;
	}

}


CFocalLawTx::~CFocalLawTx()
{
}

void CFocalLawTx::CalculateFocalLaw(PVOID pParent, float fFocalPlane, float fAngle)
{
	CAOSPhasedArray* pPA = (CAOSPhasedArray*)pParent;
	float *fArray = new float[m_nApertureCount];
	float fMaxTime = 0.0f;
	float fVelocity = (float)theApp.m_LastSettings.nMaterialVelocity * 1000.0f;
	float fX = fFocalPlane * tanf(fAngle * DEG_TO_RAD);

	ZeroMemory(m_fPulseDelay, sizeof m_fPulseDelay);
	if (fFocalPlane <= 0.0f) {
		goto safe_return;
	}

	float fHalfApertureCount = (float)m_nApertureCount / 2.0f - 0.5f;

	for (int ii = 0; ii < m_nApertureCount; ii++) {

		float fOpposite = pPA->m_fElementPitch * ((float)ii - fHalfApertureCount) - fX;

		float fHypotTime = hypotf(fOpposite, fFocalPlane) / fVelocity;

		if (fMaxTime < fHypotTime) fMaxTime = fHypotTime;
		fArray[ii] = fHypotTime;
	}

	for (int ii = 0; ii < m_nApertureCount; ii++) {
		m_fPulseDelay[ii] = fMaxTime - fArray[ii];
	}

safe_return:
	delete (fArray);
}