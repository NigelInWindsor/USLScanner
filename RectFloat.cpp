// RectFloat.cpp: implementation of the CRectFloat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "RectFloat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRectFloat::CRectFloat()
{

}

CRectFloat::CRectFloat(CRect &rr)
{
	fLeft = (float)rr.left;
	fRight = (float)rr.right;
	fTop = (float)rr.top;
	fBottom = (float)rr.bottom;
	fWidth = (float)rr.Width();
	fHeight = (float)rr.Height();
	left = rr.left;
	top = rr.top;
	bottom = rr.bottom;
	right = rr.right;
}

CRectFloat::~CRectFloat()
{

}

CPointFloat::CPointFloat()
{
	x = 0.0f;
	y = 0.0f;
}


CPointFloat::CPointFloat(float fX, float fY)
{
	x = fX;
	y = fY;
}

float CRectFloat::FloatWidth()
{
	return fabsf(fLeft - fRight);
}

float CRectFloat::FloatHeight()
{
	return fabsf(fBottom - fTop);
}