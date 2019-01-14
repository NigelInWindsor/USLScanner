// FFT.h: interface for the CFFT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FFT_H__8ADA35AC_6ED8_4B5E_8FCB_F8012583564B__INCLUDED_)
#define AFX_FFT_H__8ADA35AC_6ED8_4B5E_8FCB_F8012583564B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFFT  
{
public:
	void SetRFLength(float fSamplePeriod,float fResolvableFrequency);
	void DeAllocateMemory();
	void AllocateMemory();
	void Fft(short *real,short *imag,int length);
	void InverseFft(short *real,short *imag,int length);
	void ArrayReversal(short *real,short *imag,int number,int length);
	void InitializeDataCapture();
	void HardwareClock();
	void CalculateSharedVariables();
	void GenerateFftCoefficients();
	void DoFastFourierTransform(int* pArray,int nArrayLength,int nWindowLength,int nNumberAverages);
	void DoFastFourierTransform(char* pArray,int nArrayLength,int nWindowLength);
	void ClearPeakArray();
	void GenerateArtifitialSignal(int nFrequency);
	short ShortHypot(short x,short y);
	short AddressReverse(int n,int address);
	int AmplitudeComponent(short x,short y);
	int Mult(short x,short y);

	short	m_DataArray[0x80000];
	short	*rcoef;
	short	*icoef;
	short	*rdata;
	short	*idata;
	short	*peak;
	int		*nAddress;
	int		m_nRFLength;

	CFFT();
	virtual ~CFFT();

};

#endif // !defined(AFX_FFT_H__8ADA35AC_6ED8_4B5E_8FCB_F8012583564B__INCLUDED_)
