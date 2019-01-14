// ATS9462Digitizer.h: interface for the CAlazarDigitizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATS9462DIGITIZER_H__E13D588C_992F_4C94_A2D2_3D55ED218101__INCLUDED_)
#define AFX_ATS9462DIGITIZER_H__E13D588C_992F_4C94_A2D2_3D55ED218101__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\Include\Alazar\AlazarApi.h"
#include ".\Include\Alazar\AlazarCmd.h"
#include ".\Include\Alazar\AlazarError.h"

class CAlazarDigitizer  
{
public:
	bool m_bReEntrant;
	unsigned char cLogConversionTable[256];
	__int16 wLogConversionTable[0x10000];
	void CalculateLogConversionTable();
	int m_nSamplePeriodCodes[11];
	float m_fSamplePeriod[11];
	CAlazarDigitizer();
	virtual ~CAlazarDigitizer();
	int FindDevices();
	void Configure(ADC200Data* pAdc);
	void Configure(ADC200Data* pAdc, int BoardNumber);
	void ConfigureWidthDelay(ADC200Data* pAdc);
	void ConfigureWidthDelay(ADC200Data* pAdc, int BoardNumber);

	ALAZAR_BOARDTYPES m_Model;

	BoardDef bd;
	bool bDevicesFound;
	HANDLE	InstrumentID[5];

	CWinThread* m_pThread;
	bool m_bThreadFinished;
	bool m_bThreadEnabled;
	void SuspendThread();
	void StartThread();
	bool m_bDelayWidthChanged;

	U8 *m_pUserData[8];
	int m_nBytesPerBuffer;
	int m_nAcqLength;
	int m_nSlot;

	int m_nNumOfChannels;
	int m_nBoards;
	int m_nBoardSampleRate[8];


	
	LONG m_nDelay; //Samples

};

UINT	ATS860Thread(LPVOID pParam);
UINT	ATS860LSAThread(LPVOID pParam);
UINT	ATS9462Thread(LPVOID pParam);
UINT	ATS9626Thread(LPVOID pParam);
BOOL AbortAsyncRead();

#endif // !defined(AFX_ATS9462DIGITIZER_H__E13D588C_992F_4C94_A2D2_3D55ED218101__INCLUDED_)
