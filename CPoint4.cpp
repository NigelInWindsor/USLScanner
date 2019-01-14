#include "stdafx.h"
#include "CPoint4.h"


CPoint4::CPoint4()
{
}

CPoint4::CPoint4(float x, float y, float z, float w)
{
	mData[0] = x;
	mData[1] = y;
	mData[2] = z;
	mData[3] = w;
}


CPoint4::~CPoint4()
{
}
