// PhasedArrayProbe.h: interface for the CPhasedArrayProbe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHASEDARRAYPROBE_H__DE344E16_13DC_45F0_BAB7_3FCCFCBB0888__INCLUDED_)
#define AFX_PHASEDARRAYPROBE_H__DE344E16_13DC_45F0_BAB7_3FCCFCBB0888__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FocalLaw.h"

#define	TX_FL		0
#define	RX_FL		1

enum ProbeType {
	LINEAR_FLAT,
	LINEAR_CONCAVE,
	LINEAR_CONVEX,
};

struct	PAElementData {
	int nStructSize;

	float fVersion;

	D3DXVECTOR3	vecPt;
	D3DXVECTOR3	vecNorm;
};



class CPhasedArrayProbe  
{
public:
	int setWedgeVelocity(int nVelocity);
	int getWedgeVelocity();
	float setWedgeHeightElementOne(float fHeight);
	float getWedgeHeightElementOne();
	float setWedgeAngle(float fAngle);
	float getWedgeAngle();
	void getDelays(CString *pBuff);
	void CalculateFocussedDelays(int *pnDelayArray);
	bool CalculateDelays(int *nDelayArray);
	void setFocalLawAlgorithm(int nAlgorithm);
	int getFocalLawAlgorithm();
	void CalculateTxFocalLaws();
	void CalculateRxFocalLaws();
	D3DXVECTOR3 & getFocalLawPos(int nTxRx, int nFL);
	D3DXVECTOR3 & getFocalLawNorm(int nTxRx, int nFL);
	CString & getstrFocalLawPos(int nTxRx, int nIndex);
	float getTxDelay(int nFL, int nElement);
	float getRxDelay(int nFL, int nTOF, int nElement);
	void DownloadAllToHardware();
	void DownloadTxFocalLawsToHardware();
	void DownloadRxFocalLawsToHardware();
	ULONGLONG getElementEnableMask(int nFL, int nTxRx);
	float setAnalogueGain(float fGain, bool bWriteToHardware);
	float setDigitalGain(float fGain, bool bWriteToHardware);
	float setPulseWidth(float fPulseWidth, bool bWriteToHardware);
	int setPRF(int nPRF, bool bWriteToHardware);
	void setRectification(int nRectify, bool bWriteToHardware);
	int getRectification();
	float getAnalogueGain();
	float getDigitalGain();
	float getPulseWidth();
	void setWidthDelay();
	void setFiringOrder(int nFiringOrder);
	void setProbeType(int nProbeType);
	int getFiringOrder();
	int getProbeType();
	int getFilterType();
	int setFilterType(FrequencyFilterType eFilterType);
	void ApplyFilter();
	void CalculateFiringOrder();
	int setDacMode(int nMode);
	int getDacMode();
	void setAllDacVariables();
	void setDacInterfaceGate(bool bEnable);
	int getDACCount(int nFL);
	void ZeroDacGains(int nFL);
	void ZeroDacTimes(int nFL);
	void setTxLastElement(int nLast);
	int getTxLastElement();
	void setRxLastElement(int nLast);
	int getRxLastElement();
	float getTxFocalLength();
	void setTxFocalLength(float fLength);
	float getRxFocalTime(int nFL, int nTOF);
	float getRxFocalLength(int nWhich);
	void setRxFocalLength(float fLength, int nWhich);
	void AutoFillCoordinates(char cAxis);
	void setTxAperture(int nNumber);
	int getRxAperture();
	void setRxAperture(int nNumber);
	int getTxFirstElement();
	void setTxFirstElement(int nFirst);
	int getRxFirstElement();
	void setRxFirstElement(int nFirst);
	int getTxAperture();
	void setReverseArray(bool bFlag);
	bool getReverseArray();
	void setTxFocalLawPitch(int nPitch);
	int getIntRxFocalLawPitch();
	float getRxFocalLawPitch();
	void setRxFocalLawPitch(float fPitch);
	void setRxFocalLawPitch(int nPitch);
	float getTxFocalLawPitch();
	void setTxFocalLawPitch(float fPitch);
	CString getFileName();
	int getIntTxFocalLawPitch();
	float getElementPitch();
	void setElementPitch(float fPitch);
	bool Save(CString FilePath);
	bool Retrieve(CString FilePath);
	void Retrieve(CUSLFile *pFile);
	ULONGLONG Save(CUSLFile *pFile);
	void Zero();
	int getNumberFocalLaws();
	void setRxEqualTx();
	void setNumberFocalLaws(int nNumberFocalLaws);
	int getNumberElements();
	void setNumberElements(int nNumberElements);
	void CalculateElementCoordinates();
	D3DXVECTOR3 &			getFocalLawCenterPos(int nTXRx, int nFL);
	D3DXVECTOR3 &			getFocalLawElementPos(int nTXRx, int nFL, int nEl);
	D3DXVECTOR3 &			getElementPos(int nIndex);
	CString &				getstrElementPos(int nIndex);
	float					setTxBeamAngle(float fAngle, int StartFinish);
	float					setRxBeamAngle(float fAngle,int StartFinish);
	float					getTxBeamAngle(int StartFinish);
	float					getRxBeamAngle(int StartFinish);
	float					getProbeLength();
	CString					m_strFileName;
	float					m_fTxFocalLength;
	float					m_fRxFocalLength[10];
	float					m_fElementPitch;
	float					m_fAnalogueGain;
	float					m_fDigitalGain;
	float					m_fPulseWidth;
	int						m_nNumberElements;
	int						m_nNumberFocalLaws;
	int						m_nWedgeVelocity;
	FrequencyFilterType		m_eFilterType;
	float					m_fFilterFreq[2];
	float					m_fRipple;
	float					m_fStopGain;
	float					m_fWedgeAngle;
	float					m_fWedgeRoofAngle;
	float					m_fWedgeHeightElementOne;
	float					m_fFocalLawPitch[2];
	int						m_nAperture[2];
	int						m_nFirstElement[2];
	int						m_nLastElement[2];
	int						m_nFocalLawPitch[2];
	int						m_nFocalLawAlgorithm;
	int						m_nReverseArray;
	int						m_nFiringOrder;
	int						m_nRectification;
	int						m_nFilterGain;
	int						m_nFlOrder[256];
	ProbeType				m_eProbeType;
	struct	PAElementData	m_PAElement[256];
	CFocalLawTxRx			m_FLTx[2048];
	CFocalLawTxRx			m_FLRx[2048];
	D3DXVECTOR3				m_vElement[256];
	float					m_fBeamAngle[2][2];		//TX/RX, Start/Finish
	CFIR					m_FIR;

	float					m_fDelayArray_ns[256];

	int						m_nDacMode;
	int						m_nDacCount[2048];
	float 					m_fDacDelay[2048][64];
	float 					m_fDacGain[2048][64];


	void operator = (CPhasedArrayProbe* pPhasedArray);
	CRITICAL_SECTION	m_CriticalSection;


	CPhasedArrayProbe();
	virtual ~CPhasedArrayProbe();

};

#endif // !defined(AFX_PHASEDARRAYPROBE_H__DE344E16_13DC_45F0_BAB7_3FCCFCBB0888__INCLUDED_)
