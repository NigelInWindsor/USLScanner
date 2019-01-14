// FIR.h: interface for the CFIR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIR_H__252E45BA_222A_4A68_AA16_CB0411913BBB__INCLUDED_)
#define AFX_FIR_H__252E45BA_222A_4A68_AA16_CB0411913BBB__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FIR_BANDPASS		1
#define FIR_DIFFERENTIATOR	2
#define FIR_HILBERT			3

#define FIR_NEGATIVE	0
#define FIR_POSITIVE	1

#define Pi             3.1415926535897932
#define Pi2            6.2831853071795865

#define GRIDDENSITY    16
#define MAXITERATIONS  50

#define SAMPLE  double      /* define the type used for data samples */


class CFIR  
{
public:
	void DoubleAmpFilterAndFractional2sComplement();
	void NormalizeAmpFilter();
	void WindowedSinc(int nFilterType, int nWindowType,float fLow,float fHigh,float fMaxFrequency);

	void ProcessTrace(int nTraceL,char *pArray);
	int Mult(short x,short y);

	SAMPLE double_h(SAMPLE input, int *p_state);
	SAMPLE double_z(SAMPLE input, int *p_state);
	SAMPLE split(SAMPLE input, int *p_state);
	SAMPLE shuffle(SAMPLE input);       
	SAMPLE circular(SAMPLE input, int *p_state);         
	SAMPLE basic(SAMPLE input);

	int		nNumTaps;
	double* m_dAmpFilter;
	double* m_dDelayLine;
	WORD*	m_nAmpFilter;
	WORD*	m_nDelayLine;

	
	void FilterGain(int numTabs,double a[],int freqPoints,double f[]);
	void CreateDenseGrid(int r, int numtaps, int numband, double bands[],
                     double des[], double weight[], int *gridsize,
                     double Grid[], double D[], double W[],
                     int symmetry);
	void InitialGuess(int r, int Ext[], int gridsize);
	void CalcParms(int r, int Ext[], double Grid[], double D[], double W[],
                double ad[], double x[], double y[]);
	double ComputeA(double freq, int r, double ad[], double x[], double y[]);
	void CalcError(int r, double ad[], double x[], double y[],
               int gridsize, double Grid[],
               double D[], double W[], double E[]);
	void Search(int r, int Ext[],
            int gridsize, double E[]);
	void OldSearch(int r, int Ext[],
            int gridsize, double E[]);
	void FreqSample(int N, double A[], double h[], int symm);
	short isDone(int r, int Ext[], double E[]);
	void remez(int numband, double bands[], double des[], double weight[],
           int type);
	CFIR();
	virtual ~CFIR();

};

#endif // !defined(AFX_FIR_H__252E45BA_222A_4A68_AA16_CB0411913BBB__INCLUDED_)
