// RectFloat.h: interface for the CRectFloat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECTFLOAT_H__1D2D803D_0F2D_4F20_8B0B_866887603B07__INCLUDED_)
#define AFX_RECTFLOAT_H__1D2D803D_0F2D_4F20_8B0B_866887603B07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRectFloat : public CRect  
{
public:
	float fLeft;
	float fRight;
	float fTop;
	float fBottom;
	float fHeight;
	float fWidth;
	CRectFloat(CRect &rr);
	CRectFloat();
	virtual ~CRectFloat();

	float FloatWidth();

	float FloatHeight();

};

class CPointFloat
{
public:
	float x;
	float y;
	CPointFloat();
	CPointFloat(float fX, float fY);
	void operator = (CPoint pt);
};

inline void CPointFloat::operator = (CPoint pt)
{
	x = (float)pt.x;
	y = (float)pt.y;
};

#endif // !defined(AFX_RECTFLOAT_H__1D2D803D_0F2D_4F20_8B0B_866887603B07__INCLUDED_)
