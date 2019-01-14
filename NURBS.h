// NURBS.h: interface for the CNURBS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NURBS_H__62E11AD9_8C54_4C48_B32A_08D757DCD1C1__INCLUDED_)
#define AFX_NURBS_H__62E11AD9_8C54_4C48_B32A_08D757DCD1C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNURBS  
{
public:
	float SumRationalBasis(D3DXVECTOR4 *vec,float *fUBasis,float *fWBasis,int nUPts,int nWPts);
	void RationalBSplineSurface(D3DXVECTOR4 *vecCtrlPts,int nUOrder,int nWOrder,int nUCtrlPts,int nWCtrlPts,int nPUPts,int nPVPts,D3DXVECTOR3 *vecCurvePts);
	void rationalbspline(int nCtrlPts, int nOrder, int nCurvePts,D3DXVECTOR3 *vecCtrlPts, float *fKnots, float *fHomogeneous,D3DXVECTOR3 *vecCurvePts);
	void rationalbasis(int c,float t,int npts,float *fKnots, float *fHomogeneous,float *n);

	void bspline(int nCtrlPts, int nOrder, int nCurvePts, D3DXVECTOR3 *vecCtrlPts, float *fKnots,D3DXVECTOR3 *vecCurvePts);
	void basis(int c,float t,int npts,float *fKnots,float *n);
	void knot(int n,int c,float *x);

	CNURBS();
	virtual ~CNURBS();

};

#endif // !defined(AFX_NURBS_H__62E11AD9_8C54_4C48_B32A_08D757DCD1C1__INCLUDED_)
