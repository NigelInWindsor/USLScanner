// AcqirisDigitizer.h: interface for the CAcqirisDigitizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACQIRISDIGITIZER_H__0CFFEE2A_A907_495A_B2CE_E9108A95BA26__INCLUDED_)
#define AFX_ACQIRISDIGITIZER_H__0CFFEE2A_A907_495A_B2CE_E9108A95BA26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AcqirisImport.h"
#include "AcqirisD1Import.h"


class CAcqirisDigitizer  
{
public:
	void SetConversionRates();
	void CalculateLinConversionTable();
	char cLinConversionTable[256];


	int	m_nSlotErrorCount;
	float m_fRate[9];
	int m_nConversions;
	CWinThread* m_pThread;
	bool m_bThreadFinished;
	bool m_bThreadEnabled;
	void SuspendThread();
	void StartThread();
	void ReadArray(int nChannel, ADC200Data* pAdc,char* pArray,int nSlot);
	void Acquire(ADC200Data* pAdc);
	void Configure();
	void Vertical(ADC200Data* pAdc);
	void Horizontal(ADC200Data* pAdc);
	ViSession InstrumentID[10];
	long m_nNumInstruments;
	bool 	m_bCardPresent;

	int FindDevices();
	CAcqirisDigitizer();
	virtual ~CAcqirisDigitizer();

};

UINT	AcqirisLSAThread(LPVOID pParam);
UINT	AcqirisThread(LPVOID pParam);

#endif // !defined(AFX_ACQIRISDIGITIZER_H__0CFFEE2A_A907_495A_B2CE_E9108A95BA26__INCLUDED_)
