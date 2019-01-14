// NURBS.cpp: implementation of the CNURBS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "NURBS.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNURBS::CNURBS()
{

}

CNURBS::~CNURBS()
{

}

void CNURBS::knot(int nCtrlPts,int nOrder,float *fKnots)
{
	int nPtsPlusOrder,nPtsPlus2;

	nPtsPlusOrder = nCtrlPts + nOrder;
	nPtsPlus2 = nCtrlPts + 1;

	fKnots[0] = 0.0f;
	for (int ii = 1; ii < nPtsPlusOrder; ii++){
		if ( (ii >= nOrder) && (ii < nPtsPlus2) ) {
			fKnots[ii] = fKnots[ii-1] + 1;
		} else {
 			fKnots[ii] = fKnots[ii-1];
		}
	}
}


void CNURBS::basis(int nOrder,float t,int nCtrlPts,float *fKnots,float *fBasis)
{
	int nplusc;
	int i,k;
	float d,e;
	float temp[2000];

	nplusc = nCtrlPts + nOrder;

/* calculate the first order basis functions n[i][1]	*/

	for (i = 0; i< nplusc-1; i++){
    	if (( t >= fKnots[i]) && (t < fKnots[i+1]))
			temp[i] = 1;
	    else
			temp[i] = 0;
	}

/* calculate the higher order basis functions */

	for (k = 2; k <= nOrder; k++){
    	for (i = 0; i < nplusc-k; i++){
        	if (temp[i] != 0)    /* if the lower order basis function is zero skip the calculation */
           		d = ((t-fKnots[i])*temp[i])/(fKnots[i+k-1]-fKnots[i]);
	        else
				d = 0;

    	    if (temp[i+1] != 0)     /* if the lower order basis function is zero skip the calculation */
        		e = ((fKnots[i+k]-t)*temp[i+1])/(fKnots[i+k]-fKnots[i+1]);
	        else
    			e = 0;

    	    temp[i] = d + e;
		}
	}

	if (t == (float)fKnots[nplusc-1]){		/*    pick up last point	*/
 		temp[nCtrlPts-1] = 1;
	}


	CopyMemory(fBasis,temp,sizeof (float) * nCtrlPts);
}

void CNURBS::bspline(int nCtrlPts, int nOrder, int nCurvePts,D3DXVECTOR3 *vecCtrlPts, float *fKnots,D3DXVECTOR3 *vecCurvePts)
{
	int nplusc;

	float step;
	float t;
	float *fBasis = new float[nCtrlPts];
	ZeroMemory(fBasis,sizeof (float) * nCtrlPts);

	nplusc = nCtrlPts + nOrder;

	t = 0;
	step = (fKnots[nplusc-1])/((float)(nCurvePts-1));

	for (int nn = 0; nn< nCurvePts; nn++){

		if (fKnots[nplusc-1] - t < 5e-6) t = fKnots[nplusc-1];

	    basis(nOrder,t,nCtrlPts,fKnots,fBasis);

		vecCurvePts[nn] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		for (int mm = 0; mm < nCtrlPts; mm++) {
		    vecCurvePts[nn].x += (fBasis[mm]*vecCtrlPts[mm].x);
		    vecCurvePts[nn].y += (fBasis[mm]*vecCtrlPts[mm].y);
		    vecCurvePts[nn].z += (fBasis[mm]*vecCtrlPts[mm].z);
		}

		t = t + step;
	}

	delete fBasis;
}

void CNURBS::rationalbasis(int nOrder,float t,int nCtrlPts,float *fKnots,float *fHomogeneous, float *fBasis)
{
	int nplusc;
	int i,k;
	float d,e,sum;
	float temp[2000];

	nplusc = nCtrlPts + nOrder;

/* calculate the first order basis functions n[i][1]	*/

	for (i = 0; i< nplusc-1; i++){
    	if (( t >= fKnots[i]) && (t < fKnots[i+1]))
			temp[i] = 1;
	    else
			temp[i] = 0;
	}

/* calculate the higher order basis functions */

	for (k = 2; k <= nOrder; k++){
    	for (i = 0; i < nplusc-k; i++){
        	if (temp[i] != 0)    /* if the lower order basis function is zero skip the calculation */
           		d = ((t-fKnots[i])*temp[i])/(fKnots[i+k-1]-fKnots[i]);
	        else
				d = 0;

    	    if (temp[i+1] != 0)     /* if the lower order basis function is zero skip the calculation */
        		e = ((fKnots[i+k]-t)*temp[i+1])/(fKnots[i+k]-fKnots[i+1]);
	        else
    			e = 0;

    	    temp[i] = d + e;
		}
	}

	if (t == (float)fKnots[nplusc-1]){		/*    pick up last point	*/
 		temp[nCtrlPts-1] = 1;
	}


/* calculate sum for denominator of rational basis functions */

	sum = 0.0f;
	for (i = 0; i < nCtrlPts; i++){
		sum += (temp[i]*fHomogeneous[i]);
	}

/* form rational basis functions and put in r vector */

	for (i = 0; i < nCtrlPts; i++){
   		if (sum != 0){
			fBasis[i] = (temp[i]*fHomogeneous[i])/(sum);}
		else
			fBasis[i] = 0;
	}
}

void CNURBS::rationalbspline(int nCtrlPts, int nOrder, int nCurvePts,D3DXVECTOR3 *vecCtrlPts, float *fKnots, float *fHomogeneous,D3DXVECTOR3 *vecCurvePts)
{
	int nplusc;

	float step;
	float t;
	float *fBasis = new float[nCtrlPts];
	ZeroMemory(fBasis,sizeof (float) * nCtrlPts);

	nplusc = nCtrlPts + nOrder;

	t = 0;
	step = (fKnots[nplusc-1])/((float)(nCurvePts-1));

	for (int nn = 0; nn< nCurvePts; nn++){

		if (fKnots[nplusc-1] - t < 5e-6) t = fKnots[nplusc-1];

	    rationalbasis(nOrder,t,nCtrlPts,fKnots,fHomogeneous,fBasis);

		vecCurvePts[nn] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		for (int mm = 0; mm < nCtrlPts; mm++) {
		    vecCurvePts[nn].x += (fBasis[mm]*vecCtrlPts[mm].x);
		    vecCurvePts[nn].y += (fBasis[mm]*vecCtrlPts[mm].y);
		    vecCurvePts[nn].z += (fBasis[mm]*vecCtrlPts[mm].z);
		}

		t = t + step;
	}

	delete fBasis;
}


//vecCtrlPts[i][j]  j varying fastest
void CNURBS::RationalBSplineSurface(D3DXVECTOR4 *vecCtrlPts,int nUOrder,int nWOrder,int nUCtrlPts,int nWCtrlPts,int nPUPts,int nPWPts,D3DXVECTOR3 *vecCurvePts)
{

    int i,j,j1,jbas;
    int icount;
    int uinc,winc;
    int nplusc,mplusc;

    float pbasis;
    float sum, sumrbas();
    float u,w;

	float *fUBasis = new float[nUCtrlPts];
	ZeroMemory(fUBasis,sizeof (float) * nUCtrlPts);
	float *fWBasis = new float[nWCtrlPts];
	ZeroMemory(fWBasis,sizeof (float) * nWCtrlPts);
	
    nplusc = nUCtrlPts + nUOrder;
    mplusc = nWCtrlPts + nWOrder;

	float *fUKnots = new float[nplusc];
	float *fWKnots = new float[mplusc];

    knot(nUCtrlPts,nUOrder,fUKnots);
    knot(nWCtrlPts,nWOrder,fWKnots);

    icount = 0;

	ZeroMemory(vecCurvePts, nPUPts * nPWPts * sizeof (D3DXVECTOR3));

    float stepu = fUKnots[nplusc]/(float)(nPUPts-1);
    float stepw = fWKnots[mplusc]/(float)(nPWPts-1);
    u = 0.;
    for (uinc = 0; uinc < nPUPts; uinc++){
        if (fUKnots[nplusc-1] - u < 5e-6) u = fUKnots[nplusc-1];
        basis(nUOrder,u,nUCtrlPts,fUKnots,fUBasis);    /* basis function for this value of u */
        w = 0.;
        for (winc = 0; winc < nPWPts; winc++){
            if (fWKnots[mplusc-1] - w < 5e-6) w = fWKnots[mplusc-1];
            basis(nWOrder,w,nWCtrlPts,fWKnots,fWBasis);    /* basis function for this value of w */

			sum = SumRationalBasis(vecCtrlPts,fUBasis,fWBasis,nUCtrlPts,nWCtrlPts);
            for (i = 0; i < nUCtrlPts; i++){
                if (fUBasis[i] != 0.0f){
					jbas = nWCtrlPts*i;
					for (j = 0; j < nWCtrlPts; j++){
						if (fWBasis[j] != 0.0f){
							j1 = jbas + j;
							pbasis = vecCtrlPts[j1].w*fUBasis[i]*fWBasis[j]/sum;
							vecCurvePts[icount].x += (vecCtrlPts[j1].x * pbasis);
							vecCurvePts[icount].y += (vecCtrlPts[j1].y * pbasis);
							vecCurvePts[icount].z += (vecCtrlPts[j1].z * pbasis);
						}
					}
				}
			}
            icount++;
            w = w + stepw;
        }
        u = u + stepu;
    }
	delete fUKnots;
	delete fWKnots;
	delete fUBasis;
	delete fWBasis;
}

float CNURBS::SumRationalBasis(D3DXVECTOR4 *vec,float *fUBasis,float *fWBasis,int nUPts,int nWPts)
{

	int i,j,jbas,j1;
	float sum = 0.0f;

	for (i = 0; i < nUPts; i++){
		if (fUBasis[i] != 0.0f){
			jbas = nWPts*i;
		    for (j = 0; j < nWPts; j++){
		    	if (fWBasis[j] != 0.0f){
		    	    j1 = jbas + j;
	    	    	sum = sum + vec[j1].w*fUBasis[i]*fWBasis[j];
				}
			}
		}
	}
	return(sum);
}
