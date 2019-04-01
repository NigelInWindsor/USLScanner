#pragma once

#define	MAX_APERTURE_COUNT	32
#define	MAX_ELEMENT_COUNT	128

class CFocalLawTxRx 
{
public:
	CFocalLawTxRx();
	~CFocalLawTxRx();

	void setFocalLength(float fLength, int nTOF);
	void setBeamAngle(float fAngle);
	void CalculateTxFocalLaw(PVOID pParent);
	void CalculateRxFocalLaw(PVOID pParent);

	D3DXVECTOR3 & getCenterPos();
	void setStartLastElement(int nStart, int nFinish);
	int getFirstElement();
	int getLastElement();
	float getTxDelay(int nElement);
	float getRxDelay(int nTOF, int nElement);
	float getGain();
	float setGain(float fGain);


public:
	int		m_nTypeMask;
	double m_dWedgeDelay;
	double m_dCenterDelay;
	double	m_dAscanRange;
	double	m_dAscanDelay;
	int	m_nApertureCount;
	DWORD m_dwAperture[4];
	int m_nStartElement;
	int	m_nLastElement;
	float m_fBeamAngle;
	float m_fGain;
	int	m_nFocalTOFCount;
	float m_fFocalLength[4];

	//TX Specific
	float m_fTxDelay[MAX_APERTURE_COUNT];

	//RX Specific
	double	m_dFocalTOFDelay[4];
	float m_fRxDelay[4][MAX_APERTURE_COUNT];

	D3DXVECTOR3 m_vectO;
	D3DXVECTOR3	m_vectN;

	float m_fPulseWidth[MAX_APERTURE_COUNT];

};

