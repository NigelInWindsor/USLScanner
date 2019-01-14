// Shape.h: interface for the CShape class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAPE_H__0755A23D_328D_4F37_846E_DDE29CDB286A__INCLUDED_)
#define AFX_SHAPE_H__0755A23D_328D_4F37_846E_DDE29CDB286A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum	ShapeType {
	S_NA = 0,
	S_VECTOR,
	S_TRIANGLE,
	S_RECTANGLE,
	S_PARALLELERGRAM,
};


class CShape  
{
public:
	void Zero();
	void Rectangle(D3DXVECTOR3 vecO, D3DXVECTOR3 vecA, D3DXVECTOR3 vecB);
	int			m_nLinesL;
	CPolyCoord* m_pLines;
	ShapeType	m_ShapeType;
	CShape();
	virtual ~CShape();

};

#endif // !defined(AFX_SHAPE_H__0755A23D_328D_4F37_846E_DDE29CDB286A__INCLUDED_)
