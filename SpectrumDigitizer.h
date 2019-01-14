// SpectrumDigitizer.h: interface for the CSpectrumDigitizer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPECTRUMDIGITIZER_H__5007B0FD_0088_437A_9E1D_3AC5FECD0BC2__INCLUDED_)
#define AFX_SPECTRUMDIGITIZER_H__5007B0FD_0088_437A_9E1D_3AC5FECD0BC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\Spectrum\dlltyp.h"
#include ".\Spectrum\regs.h"
#include ".\Spectrum\spcerr.h"
#include ".\Spectrum\spcm_drv.h"

class CSpectrumDigitizer  
{
public:

	float	m_fSamplingRate[30];
	drv_handle m_hDrv;
	int32	m_nCardType;
	CString m_CardType;
	int32	m_nMaxSupportedSamplingRate;
	int32	m_nBitsPerSample;
	bool	m_bTimeBaseHW;
	bool	m_bConfigureHW;

	CString &getName();
	CWinThread* m_pThread;
	bool m_bThreadFinished;
	bool m_bThreadEnabled;
	void SuspendThread();
	void StartThread();
	void CheckErrorMessage();
	void ConfigureTimeBase();
	void ConfigureHardware();
	void Configure();
	bool m_bCardPresent;

	int FindDevices();
	CSpectrumDigitizer();
	virtual ~CSpectrumDigitizer();

};

#endif // !defined(AFX_SPECTRUMDIGITIZER_H__5007B0FD_0088_437A_9E1D_3AC5FECD0BC2__INCLUDED_)
