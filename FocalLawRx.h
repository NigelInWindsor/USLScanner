#pragma once

#define	MAX_APERTURE	32

class CFocalLawRx
{
public:
	CFocalLawRx();
	~CFocalLawRx();

	void CalculateFocalLaw(PVOID pParent, float fAngle);

public:
	double m_dWedgeDelay;
	double m_dCenterDelay;
	int	m_nApertureCount;
	DWORD m_dwAperture[4];
	int	m_nFocalTOFCount;

	double m_dAscanRange;
	double m_dAscanDelay;
	double m_dFocalTOFDelay[4];
	float m_fElementDelay[4][MAX_APERTURE];
};

