// FIR.cpp: implementation of the CFIR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "FIR.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFIR::CFIR()
{

	nNumTaps = 0;
	m_dAmpFilter = NULL;
	m_dDelayLine = NULL;
	m_nAmpFilter = NULL;
	m_nDelayLine = NULL;

}

CFIR::~CFIR()
{
	SAFE_DELETE(m_dAmpFilter);
	SAFE_DELETE(m_dDelayLine);
	SAFE_DELETE(m_nAmpFilter);
	SAFE_DELETE(m_nDelayLine);
}


/*******************
 * CreateDenseGrid
 *=================
 * Creates the dense grid of frequencies from the specified bands.
 * Also creates the Desired Frequency Response function (D[]) and
 * the Weight function (W[]) on that dense grid
 *
 *
 * INPUT:
 * ------
 * int      r        - 1/2 the number of filter coefficients
 * int      numtaps  - Number of taps in the resulting filter
 * int      numband  - Number of bands in user specification
 * double   bands[]  - User-specified band edges [2*numband]
 * double   des[]    - Desired response per band [numband]
 * double   weight[] - Weight per band [numband]
 * int      symmetry - Symmetry of filter - used for grid check
 *
 * OUTPUT:
 * -------
 * int    gridsize   - Number of elements in the dense frequency grid
 * double Grid[]     - Frequencies (0 to 0.5) on the dense grid [gridsize]
 * double D[]        - Desired response on the dense grid [gridsize]
 * double W[]        - Weight function on the dense grid [gridsize]
 *******************/

void CFIR::CreateDenseGrid(int r, int numtaps, int numband, double bands[],
                     double des[], double weight[], int *gridsize,
                     double Grid[], double D[], double W[],
                     int symmetry)
{
   int i, j, k, band;
   double delf, lowf, highf;

   delf = 0.5/(GRIDDENSITY*r);

/*
 * For differentiator, hilbert,
 *   symmetry is odd and Grid[0] = max(delf, band[0])
 */

   if ((symmetry == FIR_NEGATIVE) && (delf > bands[0]))
      bands[0] = delf;

   j=0;
   for (band=0; band < numband; band++)
   {
      Grid[j] = bands[2*band];
      lowf = bands[2*band];
      highf = bands[2*band + 1];
      k = (int)((highf - lowf)/delf + 0.5);   /* .5 for rounding */
      for (i=0; i<k; i++)
      {
         D[j] = des[band];
         W[j] = weight[band];
         Grid[j] = lowf;
         lowf += delf;
         j++;
      }
      Grid[j-1] = highf;
   }

/*
 * Similar to above, if odd symmetry, last grid point can't be .5
 *  - but, if there are even taps, leave the last grid point at .5
 */
   if ((symmetry == FIR_NEGATIVE) &&
       (Grid[*gridsize-1] > (0.5 - delf)) &&
       (numtaps % 2))
   {
      Grid[*gridsize-1] = 0.5-delf;
   }
}


/********************
 * InitialGuess
 *==============
 * Places Extremal Frequencies evenly throughout the dense grid.
 *
 *
 * INPUT: 
 * ------
 * int r        - 1/2 the number of filter coefficients
 * int gridsize - Number of elements in the dense frequency grid
 *
 * OUTPUT:
 * -------
 * int Ext[]    - Extremal indexes to dense frequency grid [r+1]
 ********************/

void CFIR::InitialGuess(int r, int Ext[], int gridsize)
{
   int i;

   for (i=0; i<=r; i++)
      Ext[i] = i * (gridsize-1) / r;
}


/***********************
 * CalcParms
 *===========
 *
 *
 * INPUT:
 * ------
 * int    r      - 1/2 the number of filter coefficients
 * int    Ext[]  - Extremal indexes to dense frequency grid [r+1]
 * double Grid[] - Frequencies (0 to 0.5) on the dense grid [gridsize]
 * double D[]    - Desired response on the dense grid [gridsize]
 * double W[]    - Weight function on the dense grid [gridsize]
 *
 * OUTPUT:
 * -------
 * double ad[]   - 'b' in Oppenheim & Schafer [r+1]
 * double x[]    - [r+1]
 * double y[]    - 'C' in Oppenheim & Schafer [r+1]
 ***********************/

void CFIR::CalcParms(int r, int Ext[], double Grid[], double D[], double W[],
                double ad[], double x[], double y[])
{
   int i, j, k, ld;
   double sign, xi, delta, denom, numer;

/*
 * Find x[]
 */
   for (i=0; i<=r; i++)
      x[i] = cos(Pi2 * Grid[Ext[i]]);

/*
 * Calculate ad[]  - Oppenheim & Schafer eq 7.132
 */
   ld = (r-1)/15 + 1;         /* Skips around to avoid round errors */
   for (i=0; i<=r; i++)
   {
       denom = 1.0;
       xi = x[i];
       for (j=0; j<ld; j++)
       {
          for (k=j; k<=r; k+=ld)
             if (k != i)
                denom *= 2.0*(xi - x[k]);
       }
       if (fabs(denom)<0.00001)
          denom = 0.00001;
       ad[i] = 1.0/denom;
   }

/*
 * Calculate delta  - Oppenheim & Schafer eq 7.131
 */
   numer = denom = 0;
   sign = 1;
   for (i=0; i<=r; i++)
   {
      numer += ad[i] * D[Ext[i]];
      denom += sign * ad[i]/W[Ext[i]];
      sign = -sign;
   }
   delta = numer/denom;
   sign = 1;

/*
 * Calculate y[]  - Oppenheim & Schafer eq 7.133b
 */
   for (i=0; i<=r; i++)
   {
      y[i] = D[Ext[i]] - sign * delta/W[Ext[i]];
      sign = -sign;
   }
}


/*********************
 * ComputeA
 *==========
 * Using values calculated in CalcParms, ComputeA calculates the
 * actual filter response at a given frequency (freq).  Uses
 * eq 7.133a from Oppenheim & Schafer.
 *
 *
 * INPUT:
 * ------
 * double freq - Frequency (0 to 0.5) at which to calculate A
 * int    r    - 1/2 the number of filter coefficients
 * double ad[] - 'b' in Oppenheim & Schafer [r+1]
 * double x[]  - [r+1]
 * double y[]  - 'C' in Oppenheim & Schafer [r+1]
 *
 * OUTPUT:
 * -------
 * Returns double value of A[freq]
 *********************/

double CFIR::ComputeA(double freq, int r, double ad[], double x[], double y[])
{
   int i;
   double xc, c, denom, numer;

   denom = numer = 0;
   xc = cos(Pi2 * freq);
   for (i=0; i<=r; i++)
   {
      c = xc - x[i];
      if (fabs(c) < 1.0e-7)
      {
         numer = y[i];
         denom = 1;
         break;
      }
      c = ad[i]/c;
      denom += c;
      numer += c*y[i];
   }
   return numer/denom;
}


/************************
 * CalcError
 *===========
 * Calculates the Error function from the desired frequency response
 * on the dense grid (D[]), the weight function on the dense grid (W[]),
 * and the present response calculation (A[])
 *
 *
 * INPUT:
 * ------
 * int    r      - 1/2 the number of filter coefficients
 * double ad[]   - [r+1]
 * double x[]    - [r+1]
 * double y[]    - [r+1]
 * int gridsize  - Number of elements in the dense frequency grid
 * double Grid[] - Frequencies on the dense grid [gridsize]
 * double D[]    - Desired response on the dense grid [gridsize]
 * double W[]    - Weight function on the desnse grid [gridsize]
 *
 * OUTPUT:
 * -------
 * double E[]    - Error function on dense grid [gridsize]
 ************************/

void CFIR::CalcError(int r, double ad[], double x[], double y[],
               int gridsize, double Grid[],
               double D[], double W[], double E[])
{
   int i;
   double A;

   for (i=0; i<gridsize; i++)
   {
      A = ComputeA(Grid[i], r, ad, x, y);
      E[i] = W[i] * (D[i] - A);
   }
}

/************************
 * Search
 *========
 * Searches for the maxima/minima of the error curve.  If more than
 * r+1 extrema are found, it uses the following heuristic (thanks
 * Chris Hanson):
 * 1) Adjacent non-alternating extrema deleted first.
 * 2) If there are more than one excess extrema, delete the
 *    one with the smallest error.  This will create a non-alternation
 *    condition that is fixed by 1).
 * 3) If there is exactly one excess extremum, delete the smaller
 *    of the first/last extremum
 *
 *
 * INPUT:
 * ------
 * int    r        - 1/2 the number of filter coefficients
 * int    Ext[]    - Indexes to Grid[] of extremal frequencies [r+1]
 * int    gridsize - Number of elements in the dense frequency grid
 * double E[]      - Array of error values.  [gridsize]
 * OUTPUT:
 * -------
 * int    Ext[]    - New indexes to extremal frequencies [r+1]
 ************************/

void CFIR::Search(int r, int Ext[],
            int gridsize, double E[])
{
   int i, j, k, l, extra;     /* Counters */
   int up, alt;
   int *foundExt;             /* Array of found extremals */

/*
 * Allocate enough space for found extremals.
 */
   foundExt = (int *)malloc(gridsize * sizeof(int));
   k = 0;

/*
 * Check for extremum at 0.
 */
   if (((E[0]>0.0) && (E[0]>E[1])) ||
       ((E[0]<0.0) && (E[0]<E[1])))
      foundExt[k++] = 0;

/*
 * Check for extrema inside dense grid
 */
   for (i=1; i<gridsize-1; i++)
   {
      if (((E[i]>=E[i-1]) && (E[i]>E[i+1]) && (E[i]>0.0)) ||
          ((E[i]<=E[i-1]) && (E[i]<E[i+1]) && (E[i]<0.0)))
         foundExt[k++] = i;
   }

/*
 * Check for extremum at 0.5
 */
   j = gridsize-1;
   if (((E[j]>0.0) && (E[j]>E[j-1])) ||
       ((E[j]<0.0) && (E[j]<E[j-1])))
      foundExt[k++] = j;


/*
 * Remove extra extremals
 */
   extra = k - (r+1);

   while (extra > 0)
   {
      if (E[foundExt[0]] > 0.0)
         up = 1;                /* first one is a maxima */
      else
         up = 0;                /* first one is a minima */

      l=0;
      alt = 1;
      for (j=1; j<k; j++)
      {
         if (fabs(E[foundExt[j]]) < fabs(E[foundExt[l]]))
            l = j;               /* new smallest error. */
         if ((up) && (E[foundExt[j]] < 0.0))
            up = 0;             /* switch to a minima */
         else if ((!up) && (E[foundExt[j]] > 0.0))
            up = 1;             /* switch to a maxima */
         else
	 { 
            alt = 0;
            break;              /* Ooops, found two non-alternating */
         }                      /* extrema.  Delete smallest of them */
      }  /* if the loop finishes, all extrema are alternating */

/*
 * If there's only one extremal and all are alternating,
 * delete the smallest of the first/last extremals.
 */
      if ((alt) && (extra == 1))
      {
         if (fabs(E[foundExt[k-1]]) < fabs(E[foundExt[0]]))
            l = foundExt[k-1];   /* Delete last extremal */
         else
            l = foundExt[0];     /* Delete first extremal */
      }

      for (j=l; j<k; j++)        /* Loop that does the deletion */
      {
         foundExt[j] = foundExt[j+1];
      }
      k--;
      extra--;
   }


//   for (i=0; i<=r; i++)
   for (i=0; i<k; i++)
   {
      Ext[i] = foundExt[i];       /* Copy found extremals to Ext[] */
   }

   free(foundExt);
}

void CFIR::OldSearch(int r, int Ext[],
            int gridsize, double E[])
{
   int i, j, k, l, extra;     /* Counters */
   int up, alt;
   int *foundExt;             /* Array of found extremals */

/*
 * Allocate enough space for found extremals.
 */
   foundExt = (int *)malloc((2*r) * sizeof(int));
   k = 0;

/*
 * Check for extremum at 0.
 */
   if (((E[0]>0.0) && (E[0]>E[1])) ||
       ((E[0]<0.0) && (E[0]<E[1])))
      foundExt[k++] = 0;

/*
 * Check for extrema inside dense grid
 */
   for (i=1; i<gridsize-1; i++)
   {
      if (((E[i]>=E[i-1]) && (E[i]>E[i+1]) && (E[i]>0.0)) ||
          ((E[i]<=E[i-1]) && (E[i]<E[i+1]) && (E[i]<0.0)))
         foundExt[k++] = i;
   }

/*
 * Check for extremum at 0.5
 */
   j = gridsize-1;
   if (((E[j]>0.0) && (E[j]>E[j-1])) ||
       ((E[j]<0.0) && (E[j]<E[j-1])))
      foundExt[k++] = j;


/*
 * Remove extra extremals
 */
   extra = k - (r+1);

   while (extra > 0)
   {
      if (E[foundExt[0]] > 0.0)
         up = 1;                /* first one is a maxima */
      else
         up = 0;                /* first one is a minima */

      l=0;
      alt = 1;
      for (j=1; j<k; j++)
      {
         if (fabs(E[foundExt[j]]) < fabs(E[foundExt[l]]))
            l = j;               /* new smallest error. */
         if ((up) && (E[foundExt[j]] < 0.0))
            up = 0;             /* switch to a minima */
         else if ((!up) && (E[foundExt[j]] > 0.0))
            up = 1;             /* switch to a maxima */
         else
	 { 
            alt = 0;
            break;              /* Ooops, found two non-alternating */
         }                      /* extrema.  Delete smallest of them */
      }  /* if the loop finishes, all extrema are alternating */

/*
 * If there's only one extremal and all are alternating,
 * delete the smallest of the first/last extremals.
 */
      if ((alt) && (extra == 1))
      {
         if (fabs(E[foundExt[k-1]]) < fabs(E[foundExt[0]]))
            l = foundExt[k-1];   /* Delete last extremal */
         else
            l = foundExt[0];     /* Delete first extremal */
      }

      for (j=l; j<k; j++)        /* Loop that does the deletion */
      {
         foundExt[j] = foundExt[j+1];
      }
      k--;
      extra--;
   }

   CString Buff;

   for (i=0; i<=r; i++)
   {
	   Buff.Format(_T("%d %d"),Ext[i],foundExt[i]);
      Ext[i] = foundExt[i];       /* Copy found extremals to Ext[] */
   }

   free(foundExt);
}


/*********************
 * FreqSample
 *============
 * Simple frequency sampling algorithm to determine the impulse
 * response h[] from A's found in ComputeA
 *
 *
 * INPUT:
 * ------
 * int      N        - Number of filter coefficients
 * double   A[]      - Sample points of desired response [N/2]
 * int      symmetry - Symmetry of desired filter
 *
 * OUTPUT:
 * -------
 * double h[] - Impulse Response of final filter [N]
 *********************/
 void CFIR::FreqSample(int N, double A[], double h[], int symm)
{
   int n, k;
   double x, val, M;

   M = (N-1.0)/2.0;
   if (symm == FIR_POSITIVE)
   {
      if (N%2)
      {
         for (n=0; n<N; n++)
         {
            val = A[0];
            x = Pi2 * (n - M)/N;
            for (k=1; k<=M; k++)
               val += 2.0 * A[k] * cos(x*k);
            h[n] = val/N;
         }
      }
      else
      {
         for (n=0; n<N; n++)
         {
            val = A[0];
            x = Pi2 * (n - M)/N;
            for (k=1; k<=(N/2-1); k++)
               val += 2.0 * A[k] * cos(x*k);
            h[n] = val/N;
         }
      }
   }
   else
   {
      if (N%2)
      {
         for (n=0; n<N; n++)
         {
            val = 0;
            x = Pi2 * (n - M)/N;
            for (k=1; k<=M; k++)
               val += 2.0 * A[k] * sin(x*k);
            h[n] = val/N;
         }
      }
      else
      {
          for (n=0; n<N; n++)
          {
             val = A[N/2] * sin(Pi * (n - M));
             x = Pi2 * (n - M)/N;
             for (k=1; k<=(N/2-1); k++)
                val += 2.0 * A[k] * sin(x*k);
             h[n] = val/N;
          }
      }
   }
}

/*******************
 * isDone
 *========
 * Checks to see if the error function is small enough to consider
 * the result to have converged.
 *
 * INPUT:
 * ------
 * int    r     - 1/2 the number of filter coeffiecients
 * int    Ext[] - Indexes to extremal frequencies [r+1]
 * double E[]   - Error function on the dense grid [gridsize]
 *
 * OUTPUT:
 * -------
 * Returns 1 if the result converged
 * Returns 0 if the result has not converged
 ********************/

short CFIR::isDone(int r, int Ext[], double E[])
{
   int i;
   double min, max, current;

   min = max = fabs(E[Ext[0]]);
   for (i=1; i<=r; i++)
   {
      current = fabs(E[Ext[i]]);
      if (current < min)
         min = current;
      if (current > max)
         max = current;
   }
   if (((max-min)/max) < 0.0001)
      return 1;
   return 0;
}

/********************
 * remez
 *=======
 * Calculates the optimal (in the Chebyshev/minimax sense)
 * FIR filter impulse response given a set of band edges,
 * the desired reponse on those bands, and the weight given to
 * the error in those bands.
 *
 * INPUT:
 * ------
 * int     numtaps     - Number of filter coefficients
 * int     numband     - Number of bands in filter specification
 * double  bands[]     - User-specified band edges [2 * numband]
 * double  des[]       - User-specified band responses [numband]
 * double  weight[]    - User-specified error weights [numband]
 * int     type        - Type of filter
 *
 * OUTPUT:
 * -------
 * double h[]      - Impulse response of final filter [numtaps]
 ********************/

void CFIR::remez(int numband, double bands[], double des[], double weight[],
           int type)
{
   double *Grid, *W, *D, *E;
   int    i, iter, gridsize, r, *Ext;
   double *taps, c;
   double *x, *y, *ad;
   int    symmetry;

   if (type == FIR_BANDPASS)
      symmetry = FIR_POSITIVE;
   else
      symmetry = FIR_NEGATIVE;

   r = nNumTaps/2;                  /* number of extrema */
   if ((nNumTaps%2) && (symmetry == FIR_POSITIVE))
      r++;

/*
 * Predict dense grid size in advance for memory allocation
 *   .5 is so we round up, not truncate
 */
   gridsize = 0;
   for (i=0; i<numband; i++)
   {
      gridsize += (int)(2*r*GRIDDENSITY*(bands[2*i+1] - bands[2*i]) + .5);
   }
   if (symmetry == FIR_NEGATIVE)
   {
      gridsize--;
   }

/*
 * Dynamically allocate memory for arrays with proper sizes
 */
   Grid = (double *)malloc(gridsize * sizeof(double));
   D = (double *)malloc(gridsize * sizeof(double));
   W = (double *)malloc(gridsize * sizeof(double));
   E = (double *)malloc(gridsize * sizeof(double));
   Ext = (int *)malloc((r+1) * sizeof(int));
   taps = (double *)malloc((r+1) * sizeof(double));
   x = (double *)malloc((r+1) * sizeof(double));
   y = (double *)malloc((r+1) * sizeof(double));
   ad = (double *)malloc((r+1) * sizeof(double));

	SAFE_DELETE(m_dAmpFilter);
	m_dAmpFilter = new double[nNumTaps * 2];	//*2 in order to use the double h2 algorithm
	SAFE_DELETE(m_dDelayLine);
	m_dDelayLine = new double[nNumTaps * 2];

	SAFE_DELETE(m_nAmpFilter);
	m_nAmpFilter = new WORD[nNumTaps * 2];	//*2 in order to use the double h2 algorithm
	SAFE_DELETE(m_nDelayLine);
	m_nDelayLine = new WORD[nNumTaps * 2];

/*
 * Create dense frequency grid
 */
   CreateDenseGrid(r, nNumTaps, numband, bands, des, weight,
                   &gridsize, Grid, D, W, symmetry);
   InitialGuess(r, Ext, gridsize);

/*
 * For Differentiator: (fix grid)
 */
   if (type == FIR_DIFFERENTIATOR)
   {
      for (i=0; i<gridsize; i++)
      {
/* D[i] = D[i]*Grid[i]; */
         if (D[i] > 0.0001)
            W[i] = W[i]/Grid[i];
      }
   }

/*
 * For odd or Negative symmetry filters, alter the
 * D[] and W[] according to Parks McClellan
 */
   if (symmetry == FIR_POSITIVE)
   {
      if (nNumTaps % 2 == 0)
      {
         for (i=0; i<gridsize; i++)
         {
            c = cos(Pi * Grid[i]);
            D[i] /= c;
            W[i] *= c; 
         }
      }
   }
   else
   {
      if (nNumTaps % 2)
      {
         for (i=0; i<gridsize; i++)
         {
            c = sin(Pi2 * Grid[i]);
            D[i] /= c;
            W[i] *= c;
         }
      }
      else
      {
         for (i=0; i<gridsize; i++)
         {
            c = sin(Pi * Grid[i]);
            D[i] /= c;
            W[i] *= c;
         }
      }
   }

/*
 * Perform the Remez Exchange algorithm
 */
   for (iter=0; iter<MAXITERATIONS; iter++)
   {
      CalcParms(r, Ext, Grid, D, W, ad, x, y);
      CalcError(r, ad, x, y, gridsize, Grid, D, W, E);
      Search(r, Ext, gridsize, E);
      if (isDone(r, Ext, E))
         break;
   }
   if (iter == MAXITERATIONS)
   {
      printf("Reached maximum iteration count.\nResults may be bad.\n");
   }

   CalcParms(r, Ext, Grid, D, W, ad, x, y);

/*
 * Find the 'taps' of the filter for use with Frequency
 * Sampling.  If odd or Negative symmetry, fix the taps
 * according to Parks McClellan
 */
   for (i=0; i<=nNumTaps/2; i++)
   {
      if (symmetry == FIR_POSITIVE)
      {
         if (nNumTaps%2)
            c = 1;
         else
            c = cos(Pi * (double)i/nNumTaps);
      }
      else
      {
         if (nNumTaps%2)
            c = sin(Pi2 * (double)i/nNumTaps);
         else
            c = sin(Pi * (double)i/nNumTaps);
      }
      taps[i] = ComputeA((double)i/nNumTaps, r, ad, x, y)*c;
   }

/*
 * Frequency sampling design with calculated taps
 */
   FreqSample(nNumTaps, taps, m_dAmpFilter, symmetry);

	DoubleAmpFilterAndFractional2sComplement();
/*
 * Delete allocated memory
 */
   free(Grid);
   free(W);
   free(D);
   free(E);
   free(Ext);
   free(x);
   free(y);
   free(ad);
   free(taps);
}



void CFIR::FilterGain(int numTaps,double a[],int freqPoints,double f[])
{

	// filter gain at (freqPoints+1) uniformly spaced frequency points
	float theta, s, c, sac, sas;
	float t = (float)Pi / freqPoints;
	int order = numTaps - 1;
	for (int i=0; i<=freqPoints; i++) {
		theta = i*t;
		sac = 0.0f;
		sas = 0.0f;
		for (int k=0; k<=order; k++) {
			c = (float)cos(k*theta);
			s = (float)sin(k*theta);
			sac += c*(float)a[k];
			sas += s*(float)a[k];
		}
		f[i] = 10.0f*log10(sac*sac + sas*sas);
	}
}





/****************************************************************************
* fir_basic: Does the basic FIR algorithm: store input sample, calculate       
* output sample, shift delay line                                           
*****************************************************************************/
SAMPLE CFIR::basic(SAMPLE input)
{
    int ii;
    SAMPLE accum;
    
    /* store input at the beginning of the delay line */
    m_dDelayLine[0] = input;

    /* calc FIR */
    accum = 0;
    for (ii = 0; ii < nNumTaps; ii++) {
        accum += m_dAmpFilter[ii] * m_dDelayLine[ii];
    }

    /* shift delay line */
    for (ii = nNumTaps - 2; ii >= 0; ii--) {
        m_dDelayLine[ii + 1] = m_dDelayLine[ii];
    }

    return accum;
}

/****************************************************************************
* fir_circular: This function illustrates the use of "circular" buffers
* in FIR implementations.  The advantage of circular buffers is that they
* alleviate the need to move data samples around in the delay line (as
* was done in all the functions above).  Most DSP microprocessors implement
* circular buffers in hardware, which allows a single FIR tap to be
* calculated in a single instruction.  That works fine when programming in
* assembly, but since C doesn't have any constructs to represent circular
* buffers, you need to "fake" them in C by adding an extra "if" statement
* inside the FIR calculation, even if the DSP processor provides hardware to
* implement them without overhead.
*****************************************************************************/
SAMPLE CFIR::circular(SAMPLE input, int *p_state)         
{
    int ii, state;
    SAMPLE accum;

    state = *p_state;               /* copy the filter's state to a local */

    /* store input at the beginning of the delay line */
    m_dDelayLine[state] = input;
    if (++state >= nNumTaps) {         /* incr state and check for wrap */
        state = 0;
    }

    /* calc FIR and shift data */
    accum = 0;
    for (ii = nNumTaps - 1; ii >= 0; ii--) {
        accum += m_dAmpFilter[ii] * m_dDelayLine[state];
        if (++state >= nNumTaps) {     /* incr state and check for wrap */
            state = 0;
        }
    }

    *p_state = state;               /* return new state to caller */

    return accum;
}

/****************************************************************************
* fir_shuffle: This is like fir_basic, except that data is shuffled by     
* moving it _inside_ the calculation loop.  This is similar to the MACD    
* instruction on TI's fixed-point processors                               
*****************************************************************************/
SAMPLE CFIR::shuffle(SAMPLE input)         
{
    int ii;
    SAMPLE accum;
    
    /* store input at the beginning of the delay line */
    m_dDelayLine[0] = input;

    /* calc FIR and shift data */
    accum = m_dAmpFilter[nNumTaps - 1] * m_dDelayLine[nNumTaps - 1];
    for (ii = nNumTaps - 2; ii >= 0; ii--) {
        accum += m_dAmpFilter[ii] * m_dDelayLine[ii];
        m_dDelayLine[ii + 1] = m_dDelayLine[ii];
    }

    return accum;
}

/****************************************************************************
* fir_split: This splits the calculation into two parts so the circular    
* buffer logic doesn't have to be done inside the calculation loop         
*****************************************************************************/
SAMPLE CFIR::split(SAMPLE input, int *p_state)
{
    int ii, end_ntaps, state = *p_state;
    SAMPLE accum;
    SAMPLE const *p_h;
    SAMPLE *p_z;

    /* setup the filter */
    accum = 0;
    p_h = m_dAmpFilter;

    /* calculate the end part */
    p_z = m_dDelayLine + state;
    *p_z = input;
    end_ntaps = nNumTaps - state;
    for (ii = 0; ii < end_ntaps; ii++) {
        accum += *p_h++ * *p_z++;
    }

    /* calculate the beginning part */
    p_z = m_dDelayLine;
    for (ii = 0; ii < state; ii++) {
        accum += *p_h++ * *p_z++;
    }

    /* decrement the state, wrapping if below zero */
    if (--state < 0) {
        state += nNumTaps;
    }
    *p_state = state;       /* return new state to caller */

    return accum;
}

/****************************************************************************
* fir_double_z: double the delay line so the FIR calculation always
* operates on a flat buffer 
*****************************************************************************/
SAMPLE CFIR::double_z(SAMPLE input, int *p_state)
{
    SAMPLE accum;
    int ii, state = *p_state;
    SAMPLE const *p_h, *p_z;

    /* store input at the beginning of the delay line as well as ntaps more */
    m_dDelayLine[state] = m_dDelayLine[state + nNumTaps] = input;

    /* calculate the filter */
    p_h = m_dAmpFilter;
    p_z = m_dDelayLine + state;
    accum = 0;
    for (ii = 0; ii < nNumTaps; ii++) {
        accum += *p_h++ * *p_z++;
    }

    /* decrement state, wrapping if below zero */
    if (--state < 0) {
        state += nNumTaps;
    }
    *p_state = state;       /* return new state to caller */

    return accum;
}

/****************************************************************************
* fir_double_h: uses doubled coefficients (supplied by caller) so that the
* filter calculation always operates on a flat buffer.
*****************************************************************************/
SAMPLE CFIR::double_h(SAMPLE input,int *p_state)
{
    SAMPLE accum;
    int ii, state = *p_state;
    SAMPLE const *p_h, *p_z;

    /* store input at the beginning of the delay line */
    m_dDelayLine[state] = input;

    /* calculate the filter */
    p_h = m_dAmpFilter + nNumTaps - state;
    p_z = m_dDelayLine;
    accum = 0;
    for (ii = 0; ii < nNumTaps; ii++) {
        accum += *p_h++ * *p_z++;
    }

    /* decrement state, wrapping if below zero */
    if (--state < 0) {
        state += nNumTaps;
    }
    *p_state = state;       /* return new state to caller */

    return accum;
}

void CFIR::ProcessTrace(int nTraceL,char *pArray)
{
	int		ii,state;

	if(nNumTaps<=0) return;

	ZeroMemory(m_dDelayLine,nNumTaps * sizeof (double) * 2);
	ZeroMemory(m_nDelayLine,nNumTaps * sizeof (WORD) * 2);
/*
	dTrace = new double[nTraceL + nNumTaps];

	for(ii=0;ii<nTraceL;ii++) {
		dTrace[ii] = (double)pArray[ii] / 128.0;
	}

	for(ii=0;ii<nTraceL;ii++) {
		pArray[ii] = (char)(basic(dTrace[ii]) * 127.0);
	}

    state = 0;
	for(ii=0;ii<nTraceL;ii++) {
		pArray[ii] = (char)(circular(dTrace[ii],&state) * 127.0);
	}


    state = 0;
	for(ii=0;ii<nTraceL;ii++) {
		pArray[ii] = (char)(split(dTrace[ii],&state) * 127.0);

	}


    state = 0;
	for(ii=0;ii<nTraceL;ii++) {
		pArray[ii] = (char)(double_z(dTrace[ii],&state) * 127.0);
	}
	
    state = 0;
	for(ii=0;ii<nTraceL;ii++) {
		pArray[ii] = (char)(double_h(dTrace[ii],&state) * 127.0);
	}
	delete dTrace;
*/

/*
	SAMPLE const *p_h, *p_z;
	SAMPLE accum;
	int tt;
	state = 0;
	for(ii=0;ii<nTraceL;ii++) {

		// store input at the beginning of the delay line 
		m_dDelayLine[state] = (double)pArray[ii] / 128.0;

		// calculate the filter
		p_h = m_dAmpFilter + nNumTaps - state;
		p_z = m_dDelayLine;
		accum = 0;
		for (tt = 0; tt < nNumTaps; tt++) {
			accum += *p_h++ * *p_z++;
		}

		// decrement state, wrapping if below zero
		if (--state < 0) {
			state += nNumTaps;
		}
	
		pArray[ii] = (char)(accum * 127.0);
	}
*/

	WORD const *p_h, *p_z;
	WORD accum;
	int tt;
	state = 0;
	for(ii=0;ii<nTraceL;ii++) {

		// store input at the beginning of the delay line 
		m_nDelayLine[state] = (WORD)pArray[ii] << 8;

		// calculate the filter
		p_h = m_nAmpFilter + nNumTaps - state;
		p_z = m_nDelayLine;
		accum = 0;
		for (tt = 0; tt < nNumTaps; tt++) {
//			accum += *p_h++ * *p_z++;
			accum += Mult(*p_h++ , *p_z++);

		}

		// decrement state, wrapping if below zero
		if (--state < 0) {
			state += nNumTaps;
		}
	
		pArray[ii] = accum>>7;
	}

}

//return value is in ax
int CFIR::Mult(short x,short y)
{

_asm {
	push	dx
	mov		ax,WORD PTR x
	imul	WORD PTR y
	mov		ax,dx
	pop		dx
};
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Windowed Sinc functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CFIR::WindowedSinc(int nFilterType, int nWindowType,float fLow,float fHigh,float fMaxFrequency)
{

	int m = nNumTaps / 2;
	float* win = new float[m+1];
	float r;
	float trBand;
	int n;

	switch (nWindowType) {
	case RECTANGULAR:
		for (n=1; n <= m; n++) 
			win[n] = 1.0f;
		trBand = 1.84f*fMaxFrequency/nNumTaps;
		break;
	case HANNING:
		r = (float)PI/(m+1);
		for (n=1; n <= m; n++) {
			win[n] = 0.5f + 0.5f*(float)cos(n*r);
		}
		trBand = 6.22f*fMaxFrequency/nNumTaps;
		break;
	case HAMMING:
		r = (float)PI/m;
		for (n=1; n <= m; n++)
			win[n] = 0.54f + 0.46f*(float)cos(n*r);
		trBand = 6.64f*fMaxFrequency/nNumTaps;
		break;
	case BLACKMAN:
		r = (float)PI/m;
		for (n=1; n <= m; n++)
			win[n] = 0.42f + 0.5f*(float)cos(n*r) + 0.08f*(float)cos(2*n*r);
		trBand = 11.13f*fMaxFrequency/nNumTaps;
		break;
	case WELCH:
		for(n=1; n <= m; n++) {
			win[n] = ((float)(n-m)/(float)m) * ((float)(n-m)/(float)m);
		}
		trBand = 11.13f*fMaxFrequency/nNumTaps;
		break;
	default: for (n=1; n <= m; n++) win[n] = 1.0f;
		trBand = 999.0;
	}

	float w0 = 0.0f;
	float w1 = 0.0f;
	switch (nFilterType) {
	case LOWPASS: w0 = 0.0f;
		w1 = (float)PI*(fLow + 0.5f*trBand)/fMaxFrequency;
		break;
	case HIGHPASS: w0 = (float)PI;
		w1 = (float)PI*(1.0f - (fHigh - 0.5f*trBand)/fMaxFrequency);
		break;
	case BANDPASS: w0 = 0.5f * (float)PI * (fLow + fHigh) / fMaxFrequency;
		w1 = 0.5f * (float)PI * (fHigh - fLow + trBand) / fMaxFrequency;
		break;
	}

	// filter coefficients (NB not normalised to unit maximum gain)
	SAFE_DELETE(m_dAmpFilter);
	m_dAmpFilter = new double[nNumTaps * 2];	//*2 in order to use the double h2 algorithm
	SAFE_DELETE(m_dDelayLine);
	m_dDelayLine = new double[nNumTaps * 2];

	SAFE_DELETE(m_nAmpFilter);
	m_nAmpFilter = new WORD[nNumTaps * 2];	//*2 in order to use the double h2 algorithm
	SAFE_DELETE(m_nDelayLine);
	m_nDelayLine = new WORD[nNumTaps * 2];

	m_dAmpFilter[0] = w1 / (float)PI;
	for (n=1; n <= m; n++)
		m_dAmpFilter[n] = (float)sin(n*w1)*(float)cos(n*w0)*win[n]/(n*(float)PI);
	// shift impulse response to make filter causal:
	for (n=m+1; n<=nNumTaps; n++) m_dAmpFilter[n] = m_dAmpFilter[n - m];
	for (n=0; n<=m-1; n++) m_dAmpFilter[n] = m_dAmpFilter[nNumTaps - n];
	m_dAmpFilter[m] = w1 / (float)PI;

	NormalizeAmpFilter();


	DoubleAmpFilterAndFractional2sComplement();

   delete win;

}

void CFIR::NormalizeAmpFilter()
{
	int i;
	// filter gain at uniform frequency intervals
	int nFreqPoints=500;
	float* g = new float[nFreqPoints+1];
    float theta, s, c, sac, sas;
    float gMax = -100.0f;
    float sc = 10.0f/(float)log(10.0f);
    float t = (float)PI / nFreqPoints;
    for (i=0; i<=nFreqPoints; i++) {
      theta = i*t;
      sac = 0.0f;
      sas = 0.0f;
      for (int k=0; k<=nNumTaps; k++) {
        c = (float)cos(k*theta);
        s = (float)sin(k*theta);
        sac += c*(float)m_dAmpFilter[k];
        sas += s*(float)m_dAmpFilter[k];
      }
      g[i] = sc*(float)log(sac*sac + sas*sas);
      gMax = max(gMax, g[i]);
    }
    // normalise to 0 dB maximum gain
    for (i=0; i<=nFreqPoints; i++) g[i] -= gMax;
    // normalise coefficients
    float normFactor = (float)pow(10.0, -0.05*gMax);
    for (i=0; i<=nNumTaps; i++) m_dAmpFilter[i] *= normFactor;

    delete g;
}

void CFIR::DoubleAmpFilterAndFractional2sComplement()
{

	for(int i=0;i<nNumTaps;i++) {
		m_dAmpFilter[i+nNumTaps] = m_dAmpFilter[i];
		m_nAmpFilter[i+nNumTaps] = m_nAmpFilter[i] = (WORD)(m_dAmpFilter[i] * 32767.0f);
	}


}
