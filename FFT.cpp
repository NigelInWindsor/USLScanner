// FFT.cpp: implementation of the CFFT class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "FFT.h"
#include "math.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFFT::CFFT()
{
	rcoef = NULL;
	icoef = NULL;
	rdata = NULL;
	idata = NULL;
	peak = NULL;
	nAddress = NULL;
	m_nRFLength = 8192;

	AllocateMemory();
	GenerateFftCoefficients();
}

CFFT::~CFFT()
{

	DeAllocateMemory();

}




/****************************************
This fft does a decimation in time fft.
d0=mult(rdata[lli],cosine);
d1=mult(rdata[lli],sine);
d2=mult(idata[lli],cosine);
d3=mult(idata[lli],sine);
i0=Ar(lli)cos(wn)+Ai(lli)sin(wn)
i1=Ai(lli)cos(wn)-Ar(lli)sin(wn)
i2=rdata[uli] /2;
i4=idata[uli] /2;
rdata[uli]=i2+i0;
idata[uli]=i4+i1;
rdata[lli]=i2-i0;
idata[lli]=i4-i1;
*****************************************/

void CFFT::Fft(short *real,short *imag,int length)
{
int	half_n,uli,lli,ls,gs,gpp,bpg,ci;
int	pass,pass_num,group,butts;
int	ciindex;

short	sine,cosine;
short	d0,d1,d2,d3;
short	i0,i1,i2,i4;
short	re,im;


switch (length) {
case 65536 :	pass_num=16;
			ciindex=1;
		break;
case 32768 :	pass_num=15;
			ciindex=1;
		break;
case 16384 :	pass_num=14;
			ciindex=1;
		break;
case 8192 :	pass_num=13;
			ciindex=1;
		break;
case 4096 :	pass_num=12;
			ciindex=1;
		break;
case 2048 :	pass_num=11;
			ciindex=1;
		break;
case 1024 :	pass_num=10;
			ciindex=1;
		break;
case 512 :	pass_num=9;
			ciindex=2;
		break;
case 256 :	pass_num=8;
			ciindex=4;
		break;
case 128 :	pass_num=7;
			ciindex=8;
		break;
case 64 :		pass_num=6;
			ciindex=16;
		break;
case 32 :		pass_num=5;
			ciindex=32;
		break;
};
//THIS LINE ADDED SINCE RCOEF AND ICOEF ARE NOW VARIABLE LENGTH
ciindex=1;

/*initialize*/


half_n=length/2;
half_n--;
gpp=length/2;
bpg=1;
gs=2;
ls=1;

ArrayReversal(real,imag,pass_num,length);
for (pass=0;pass<pass_num;pass++) {
	ci=0;
	for (butts=0;butts<bpg;butts++) {
		cosine=rcoef[ci];
		sine=icoef[ci];
		uli=butts;
		lli=uli+ls;
		for (group=0;group<gpp;group++) {
			/***butterfly***/
			re = real[lli];
			im = imag[lli];
			_asm {
				mov	ax,WORD PTR cosine
				imul	WORD PTR	re
				mov	WORD PTR d0,dx
				mov	ax,WORD PTR sine
				imul	WORD PTR	re
				mov	WORD PTR d1,dx
				mov	ax,WORD PTR cosine
				imul	WORD PTR	im
				mov	WORD PTR d2,dx
				mov	ax,WORD PTR sine
				imul	WORD PTR	im
				mov	WORD PTR d3,dx
			};
			i0=d0+d3;
			i1=d2-d1;
			i2=real[uli] /2;
			i4=imag[uli] /2;
			real[uli]=i2+i0;
			imag[uli]=i4+i1;
			real[lli]=i2-i0;
			imag[lli]=i4-i1;

			uli=uli+gs;
			lli+=gs;
		};
		ci/=ciindex;
		ci+=gpp;
		ci&=half_n;
		ci*=ciindex;
	};
	ls<<=1;
	gs<<=1;
	gpp>>=1;
	bpg<<=1;
};
}


/****************************************
This fft does a decimation in time fft.
d0=mult(real[lli],cosine);
d1=mult(real[lli],sine);
d2=mult(imag[lli],cosine);
d3=mult(imag[lli],sine);
i0=Ar(lli)cos(wn)+Ai(lli)sin(wn)
i1=Ai(lli)cos(wn)-Ar(lli)sin(wn)
i2=real[uli] /2;
i4=imag[uli] /2;
real[uli]=i2+i0;
imag[uli]=i4+i1;
real[lli]=i2-i0;
imag[lli]=i4-i1;
*****************************************/


void CFFT::InverseFft(short *real,short *imag,int length)
{
int	half_n,uli,lli,ls,gs,gpp,bpg,ci;
int	pass,pass_num,group,butts;
int	ciindex;
int	sine,cosine;
int	d0,d1,d2,d3;
int	i0,i1,i2,i4;

switch (length) {
case 1024 :	pass_num=10;
			ciindex=1;
		break;
case 512 :	pass_num=9;
			ciindex=2;
		break;
case 256 :	pass_num=8;
			ciindex=4;
		break;
case	128 :	pass_num=7;
			ciindex=8;
		break;
case 64 :		pass_num=6;
			ciindex=16;
		break;
case	32 :		pass_num=5;
			ciindex=32;
		break;
};


/*initialize*/


half_n=length/2;
half_n--;
gpp=length/2;
bpg=1;
gs=2;
ls=1;
ArrayReversal(real,imag,pass_num,length);
for (pass=0;pass<pass_num;pass++) {
	ci=0;
	for (butts=0;butts<bpg;butts++) {
		cosine=rcoef[ci];
		sine=icoef[ci];
		uli=butts;
		lli=uli+ls;
		for (group=0;group<gpp;group++) {
			/***butterfly***/
			d0=Mult(real[lli],cosine);
			d1=Mult(real[lli],sine);
			d2=Mult(imag[lli],cosine);
			d3=Mult(imag[lli],sine);
			i0=d1+d2;
			i1=d3-d0;
			i2=real[uli] / 2;
			i4=imag[uli] / 2;
			real[uli]=i2-i1;
			imag[uli]=i4+i0;
			real[lli]=i2+i1;
			imag[lli]=i4-i0;

			uli=uli+gs;
			lli+=gs;
		};
		ci/=ciindex;
		ci+=gpp;
		ci&=half_n;
		ci*=ciindex;
	};
	ls<<=1;
	gs<<=1;
	gpp>>=1;
	bpg<<=1;
};
}





int CFFT::AmplitudeComponent(short x,short y)
{
int	product;
int	sum;
int	guess;
int	temp;
int	xi,xj;

sum=0;
sum=(long)x * (long)x;
product=(long)y * (long)y;
sum=sum + product;
if (sum==0) return(0);
if (sum<0) {
	sum+=2;
	sum*=-1;
};
/****************************
xj=xi - (xi / 2) + (sum / 2xi)
****************************/
xi=1;
xj=sum;
sum=sum / 2;
while (((xi-xj) > 6) || ((xi-xj) < -6)) {
	xj=xi;
	guess=sum / xi;
     /******************
	guess=guess - (xi / 2);
	xi=xi + guess;
	guess is in dx and ax
     ******************/
	temp = xi / 2l;
	guess -= temp;
	xi += guess;
};
return(xi);
}

short CFFT::ShortHypot(short x,short y)
{
int	product;
int	sum;
int	guess;
int	temp;
int	xi,xj;

_asm {
	sub	dx,dx
	mov	ax,WORD PTR x
	imul	WORD PTR x
	mov	WORD PTR sum,ax
	mov	WORD PTR sum+2,dx
	sub	dx,dx
	mov	ax,WORD PTR y
	imul	WORD PTR y
	mov	WORD PTR product,ax
	mov	WORD PTR product+2,dx
};

sum=sum + product;
if (sum==0) return(0);
if (sum<0) {
	sum+=2;
	sum*=-1;
};
/****************************
xj=xi - (xi / 2) + (sum / 2xi)
****************************/
xi=1;
xj=sum;
//sum=sum / 2;
_asm {
	mov	ax,WORD PTR sum
	mov	dx,WORD PTR sum+2
	sar	dx,1
	rcr	ax,1
	sar	dx,1
	rcr	ax,1
	sar	dx,1
	rcr	ax,1
	mov	WORD PTR sum,ax
	mov	WORD PTR sum+2,dx
};
while ((abs(xi-xj) > 2) && (xi)) {
	xj=xi;
	guess=sum / xi;
     /******************
	guess=guess - (xi / 2);
	xi=xi + guess;
	guess is in dx and ax
     ******************/
	_asm {
	mov	ax,WORD PTR xi
	mov	dx,WORD PTR xi+2
	sar	dx,1
	rcr	ax,1
	mov	WORD PTR temp,ax
	mov	WORD PTR temp+2,dx
	};
//	temp = xi / 2l;
	guess -= temp;
	xi += guess;
};
return((short)xi);
}










void CFFT::ArrayReversal(short *real,short *imag,int number,int length)
{
	int ii,jj;
	short	*rtemp;
	short	*itemp;

	rtemp=new short[length];
	itemp=new short[length];
	for (ii=0;ii<length;ii++) {

		jj=nAddress[ii];
		rtemp[jj]=real[ii];
		itemp[jj]=imag[ii];
	};
	memcpy(real,rtemp,length*sizeof(short));
	memcpy(imag,itemp,length*sizeof(short));
	delete rtemp;
	delete itemp;
}








short CFFT::AddressReverse(int n,int address)
{


_asm {
	pusha
	mov	ax,0
	mov	bx,WORD PTR address
	mov	cx,WORD PTR n
loop_point:
	rcr	bx,1
	rcl	ax,1
	loop	loop_point
	popa



};
}


int CFFT::Mult(short x,short y)
{

_asm {
	push	dx
	mov		ax,WORD PTR x
	imul	WORD PTR y
	mov		ax,dx
	pop		dx
};
}




void CFFT::GenerateFftCoefficients()
{
	short	j;
	int	ii, nSrcMask, nDestMask, nMask;
	short result, number;
	double	angle;

	switch (m_nRFLength) {
	case 64: number = 6;
		nMask = 0x0020;
		break;
	case 128:	number = 7;
		nMask = 0x0040;
		break;
	case 256:	number = 8;
		nMask = 0x0080;
		break;
	case 512:	number = 9;
		nMask = 0x0100;
		break;
	case 1024:	number = 10;
		nMask = 0x0200;
		break;
	case 2048:	number = 11;
		nMask = 0x0400;
		break;
	case 4096:	number = 12;
		nMask = 0x0800;
		break;
	case 8192:	number = 13;
		nMask = 0x1000;
		break;
	case 16384:	number = 14;
		nMask = 0x2000;
		break;
	case 32768: number = 15;
		nMask = 0x4000;
		break;
	case 65536: number = 16;
		nMask = 0x8000;
		break;
	}

	for (ii = 0; ii<m_nRFLength; ii++) {
		nAddress[ii] = 0;
		for (j = 0, nSrcMask = 1, nDestMask = nMask; j<number; j++, nSrcMask <<= 1, nDestMask >>= 1) {
			if (ii&nSrcMask) {
				nAddress[ii] |= nDestMask;
			}
		}
	}


	int nHalfLength = m_nRFLength / 2;
	memset(peak, 0, nHalfLength * sizeof(short));
	for (ii = 0; ii<nHalfLength; ii++) {
		angle = (PI*(double)ii) / nHalfLength;
		result = (short)(0x7fff * cos(angle));
		rcoef[ii] = result;
	};
	for (ii = 0; ii<nHalfLength; ii++) {
		angle = (PI*(double)ii) / nHalfLength;
		result = (short)(0x7fff * sin(angle));
		icoef[ii] = result;
	}

}

void CFFT::DoFastFourierTransform(char* pArray,int nArrayLength,int nWindowLength)
{
	int	ii,jj;
	short vv;
	int	nHalfWindowLength=nWindowLength/2;
	double 	dTemp;


	memset(idata,0,m_nRFLength*sizeof(short));
	memset(rdata,0,m_nRFLength*sizeof(short));
	for(ii=0;ii<nArrayLength;ii++) {
		rdata[ii] = (int)pArray[ii]*256;
	}

	switch(theApp.m_LastSettings.nFFTWindow) {
	case 0:
		break;
	case 1:
		for(ii=0;ii<nHalfWindowLength;ii++) {
			vv=(short)((0x7fff * ii) / nHalfWindowLength);
			rdata[ii] = Mult(rdata[ii],vv);
		}
		for(ii,jj=0;ii<nWindowLength;ii++,jj++) {
			vv=0x7fff - (short)((0x7fff * jj) / nHalfWindowLength);
			rdata[ii] = Mult(rdata[ii],vv);
		}
		break;
	case 2:
		for(ii=0;ii<nWindowLength;ii++) {
			vv=0x7fff;
			dTemp = (double)(ii-nHalfWindowLength) / (double)nHalfWindowLength;
			vv = 0x7fff - (short)((double)vv * (dTemp * dTemp));
			rdata[ii] = Mult(rdata[ii],vv);
		}
		break;
	case 3:
		for(ii=0;ii<nWindowLength;ii++) {
			vv=0x7fff;
			dTemp = ((double)ii * 2.0 * PI) / (double)nWindowLength;
			dTemp = 0.5 * (1.0 - cos(dTemp));
			vv = (short)((double)vv * dTemp);
			rdata[ii] = Mult(rdata[ii],vv);
		}
		break;
	}

	Fft(rdata,idata,m_nRFLength);

	memset(peak,0,m_nRFLength);
	for(ii=0;ii<(m_nRFLength/2);ii++) {
		rdata[ii]=ShortHypot(rdata[ii],idata[ii]);
		if(rdata[ii]>peak[ii]) peak[ii]=rdata[ii];
	};

}

void CFFT::DoFastFourierTransform(int* pArray,int nArrayLength,int nWindowLength,int nNumberAverages)
{
	int	ii,jj;
	short vv;
	int	nHalfWindowLength=nWindowLength/2;
	double 	dTemp;
	int nMult = 256;

	nMult /= nNumberAverages;


	memset(idata,0,m_nRFLength*sizeof(short));
	memset(rdata,0,m_nRFLength*sizeof(short));
	for(ii=0;ii<nArrayLength;ii++) {
		rdata[ii] = (int)pArray[ii]*nMult;
	}

	switch(theApp.m_LastSettings.nFFTWindow) {
	case 0:
		break;
	case 1:
		for(ii=0;ii<nHalfWindowLength;ii++) {
			vv=(short)((0x7fff * ii) / nHalfWindowLength);
			rdata[ii] = Mult(rdata[ii],vv);
		}
		for(ii,jj=0;ii<nWindowLength;ii++,jj++) {
			vv=0x7fff - (short)((0x7fff * jj) / nHalfWindowLength);
			rdata[ii] = Mult(rdata[ii],vv);
		}
		break;
	case 2:
		for(ii=0;ii<nWindowLength;ii++) {
			vv=0x7fff;
			dTemp = (double)(ii-nHalfWindowLength) / (double)nHalfWindowLength;
			vv = 0x7fff - (short)((double)vv * (dTemp * dTemp));
			rdata[ii] = Mult(rdata[ii],vv);
		}
		break;
	case 3:
		for(ii=0;ii<nWindowLength;ii++) {
			vv=0x7fff;
			dTemp = ((double)ii * 2.0 * PI) / (double)nWindowLength;
			dTemp = 0.5 * (1.0 - cos(dTemp));
			vv = (short)((double)vv * dTemp);
			rdata[ii] = Mult(rdata[ii],vv);
		}
		break;
	}

	Fft(rdata,idata,m_nRFLength);

	memset(peak,0,m_nRFLength);
	for(ii=0;ii<(m_nRFLength/2);ii++) {
		rdata[ii]=ShortHypot(rdata[ii],idata[ii]);
		if(rdata[ii]>peak[ii]) peak[ii]=rdata[ii];
	};

}

void CFFT::ClearPeakArray()
{
	memset(peak,0,(m_nRFLength/2)*sizeof(short));
}


void CFFT::GenerateArtifitialSignal(int nFrequency)
{
	int	nStep = (10000 / nFrequency) / 2;
	int	nAmp = 0;
	int ii,jj;

	for(ii=0;ii<m_nRFLength;ii+=nStep) {
		for(jj=0;jj<nStep;jj++) {
			m_DataArray[ii+jj] = nAmp;
		}
		if(nAmp==0) {
			nAmp=10000;
		} else {
			nAmp=0;
		}
	}

}

void CFFT::AllocateMemory()
{
	DeAllocateMemory();

	rcoef = new short[m_nRFLength/2];
	icoef = new short[m_nRFLength/2];
	rdata = new short[m_nRFLength];
	idata = new short[m_nRFLength];
	peak = new short[m_nRFLength/2];
	nAddress = new int[m_nRFLength];

}

void CFFT::DeAllocateMemory()
{

	if(rcoef) delete rcoef;
	if(icoef) delete icoef;
	if(rdata) delete rdata;
	if(idata) delete idata;
	if(peak) delete peak;
	if(nAddress) delete nAddress;

	rcoef = NULL;
	icoef = NULL;
	rdata = NULL;
	idata = NULL;
	peak = NULL;
	nAddress = NULL;

}

void CFFT::SetRFLength(float fSamplePeriod, float fResolvableFrequency)
{
	m_nRFLength = (int)(((500.0 / fSamplePeriod) / fResolvableFrequency) * 2.0);

	if (m_nRFLength>32768) {
		m_nRFLength = 65536;
	}
	else {
		if (m_nRFLength>16384) {
			m_nRFLength = 32768;
		}
		else {
			if (m_nRFLength>8192) {
				m_nRFLength = 16384;
			}
			else {
				if (m_nRFLength>4096) {
					m_nRFLength = 8192;
				}
				else {
					if (m_nRFLength>2048) {
						m_nRFLength = 4096;
					}
					else {
						if (m_nRFLength>1024) {
							m_nRFLength = 2048;
						}
						else {
							if (m_nRFLength>512) {
								m_nRFLength = 1024;
							}
							else {
								if (m_nRFLength>256) {
									m_nRFLength = 512;
								}
								else {
									if (m_nRFLength>128) {
										m_nRFLength = 256;
									}
									else {
										if (m_nRFLength>64) {
											m_nRFLength = 128;
										}
										else {
											if (m_nRFLength>32) {
												m_nRFLength = 64;
											}
											else {
												m_nRFLength = 64;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	AllocateMemory();
	GenerateFftCoefficients();
}
