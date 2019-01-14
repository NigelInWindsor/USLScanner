#pragma once

#define	MAX_APERTURE_COUNT	32

class CFocalLawTx 
{
public:
	CFocalLawTx();
	~CFocalLawTx();

	void CalculateFocalLaw(PVOID pParent, float fFocalPlane, float fAngle);

public:
	double m_dWedgeDelay;
	double m_dCenterDelay;
	int	m_nApertureCount;
	DWORD m_dwAperture[4];

	float m_fPulseDelay[MAX_APERTURE_COUNT];
	float m_fPulseWidth[MAX_APERTURE_COUNT];

};

