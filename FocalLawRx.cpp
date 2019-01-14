#include "stdafx.h"
#include "FocalLawRx.h"
#include "USLScanner.h"


CFocalLawRx::CFocalLawRx()
{
	m_dWedgeDelay = 0.0;
	m_dCenterDelay = 0.0;
	m_nApertureCount = 1;
	ZeroMemory(m_dwAperture, sizeof m_dwAperture);
	m_dwAperture[0] = 0x01;

	m_dAscanDelay = 0.0;
	m_dAscanRange = 100.0e-6;
	m_nFocalTOFCount = 1;
	ZeroMemory(m_dFocalTOFDelay, sizeof m_dFocalTOFDelay);
	ZeroMemory(m_fElementDelay, sizeof m_fElementDelay);

}


CFocalLawRx::~CFocalLawRx()
{
}

void CFocalLawRx::CalculateFocalLaw(PVOID pParent, float fAngle)
{
	CAOSPhasedArray* pPA = (CAOSPhasedArray*)pParent;
	float *fArray = new float[m_nApertureCount];
	float fMaxTime = 0.0f;
	float fVelocity = (float)theApp.m_LastSettings.nMaterialVelocity * 1000.0f;

	ZeroMemory(m_fElementDelay, sizeof m_fElementDelay);
	ZeroMemory(m_dFocalTOFDelay, sizeof m_dFocalTOFDelay);

	m_nFocalTOFCount = 0;
	for (int nLoop = 0; nLoop < 1; nLoop++) {

		float fX = pPA->m_fRxFocalLength[nLoop] * tanf(fAngle * DEG_TO_RAD);

		if (pPA->m_fRxFocalLength[nLoop] <= 0.0f) {
			goto safe_return;
		}

		float fHalfApertureCount = (float)m_nApertureCount / 2.0f - 0.5f;

		for (int ii = 0; ii < m_nApertureCount; ii++) {

			float fOpposite = pPA->m_fElementPitch * ((float)ii - fHalfApertureCount) - fX;

			float fHypotTime = hypotf(fOpposite, pPA->m_fRxFocalLength[nLoop]) / fVelocity;

			if (fMaxTime < fHypotTime) fMaxTime = fHypotTime;
			fArray[ii] = fHypotTime;
		}
		m_dFocalTOFDelay[nLoop] = (double)fMaxTime * 2.0;

		for (int ii = 0; ii < m_nApertureCount; ii++) {
			m_fElementDelay[nLoop][ii] = (fMaxTime - fArray[ii]) * 2.0f;		// * 2.0 because of there and back
		}
		m_nFocalTOFCount++;
	}

safe_return:
	delete (fArray);
}