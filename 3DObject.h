// 3DObject.h: interface for the C3DObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DOBJECT_H__C57018F8_0541_4B97_9EDF_CE09BBAE60A2__INCLUDED_)
#define AFX_3DOBJECT_H__C57018F8_0541_4B97_9EDF_CE09BBAE60A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DPart.h"

class C3DObject  
{
public:
	bool FillTwoPolyLines(CVertexArray *pLine0,CVertexArray *pLine1,D3DXCOLOR rgb);
	bool PolyLine(CVertexArray *pLine,D3DXCOLOR rgb,float fOffsetAlongNormal = 0.0f,bool bConnectEnds = false);
	bool BuildCScanLine(int nLine, CData *pData, int nImageNumber, int nImageType);
	bool FillTwoPolyLines(CPolyCoord *pLine0,CPolyCoord *pLine1,D3DXCOLOR rgb);
	bool PolyLine(CPolyCoord *pLine,D3DXCOLOR rgb,float fOffsetAlongNormal = 0.0f,bool bConnectEnds = false);
	bool Cylinder(float fDiameter, float fHeight, D3DXVECTOR3 vecNormal, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate = NULL, bool bShowInside = false);
	bool Rod(float fDiameter, float fLength, D3DXVECTOR3 vecNormal, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate = NULL);
	bool Disc(float fDiameter, D3DXVECTOR3 vecNormal, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate = NULL);
	bool Rectangle(D3DXVECTOR3  vecSize, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate = NULL, D3DXVECTOR3 *vecRotate = NULL, bool bShowOppositeFace = false);
	bool Cuboid(D3DXVECTOR3  vecSize, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate = NULL, D3DXVECTOR3 *vecRotate = NULL);
	int SetZMode(int nMode);
	int SetCullMode(int nMode);
	bool Render();
	int		m_nZMode;
	int		m_nCullMode;
	int		m_nPartsL;
	int		m_nCurrentPart;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	C3DPart* m_pParts;
	bool InvalidateDeviceObjects();
	bool DeleteDeviceObjects();
	C3DObject(LPDIRECT3DDEVICE9 pd3dDevice = NULL,int nHowMany = 0);
	virtual ~C3DObject();

};

#endif // !defined(AFX_3DOBJECT_H__C57018F8_0541_4B97_9EDF_CE09BBAE60A2__INCLUDED_)
