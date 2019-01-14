// Shape.cpp: implementation of the CShape class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "Shape.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShape::CShape()
{
	m_nLinesL = 0;
	m_pLines = NULL;
	m_ShapeType = S_NA;

}

CShape::~CShape()
{

}

void CShape::Zero()
{
	SAFE_DELETE_ARRAY( m_pLines );
	m_nLinesL = 0;
	m_ShapeType = S_NA;
}

void CShape::Rectangle(D3DXVECTOR3 vecO, D3DXVECTOR3 vecA, D3DXVECTOR3 vecB)
{
	CCoord Cp;

	Zero();
	m_ShapeType = S_RECTANGLE;
	m_pLines = new CPolyCoord[m_nLinesL=1];

	Cp.Zero();

	Cp.Side0.pt = vecO;
	m_pLines[0].Add(Cp);
	Cp.Side0.pt = vecA;
	m_pLines[0].Add(Cp);
	Cp.Side0.pt = vecB;
	m_pLines[0].Add(Cp);
}

