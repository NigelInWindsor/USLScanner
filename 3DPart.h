// 3DPart.h: interface for the C3DPart class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DPART_H__3C5044E8_F9C5_445F_A451_99133288BA7C__INCLUDED_)
#define AFX_3DPART_H__3C5044E8_F9C5_445F_A451_99133288BA7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "3DPrimitive.h"
// z modes
//D3DZB_FALSE         = 0,
//D3DZB_TRUE          = 1,
//D3DZB_USEW          = 2,

class C3DPart  
{
public:
	inline int MinMax(int *pnV, int nMin, int nMax);
	inline void operator = (C3DPart Part) {
		m_pd3dDevice	= Part.m_pd3dDevice;
		m_nPrimL		= Part.m_nPrimL;
		m_pPrim			= Part.m_pPrim;
	};
	void FillTwoPolyLines(LPDIRECT3DDEVICE9 pd3dDevice, CVertexArray *pLine0, CVertexArray *pLine1, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate = NULL, D3DXVECTOR3 *vecRotate = NULL);
	void PolyLine(LPDIRECT3DDEVICE9 pd3dDevice, CVertexArray *pLine, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate = NULL, D3DXVECTOR3 *vecRotate = NULL,float fOffsetAlongNormal = 0.0f, bool bConnectEnds = false);
	void BuildCScanLineUsingDataVertices(LPDIRECT3DDEVICE9 pd3dDevice, int nLine, CData *pData, int nImageNumber, int nImageType, D3DXVECTOR3 *vecTranslate = NULL, D3DXVECTOR3 *vecRotate = NULL);
	void BuildCScanLineUsingProfileScanLines(LPDIRECT3DDEVICE9 pd3dDevice, int nLine, CData *pData, int nImageNumber, int nImageType, D3DXVECTOR3 *vecTranslate = NULL, D3DXVECTOR3 *vecRotate = NULL);
	void FillTwoPolyLines(LPDIRECT3DDEVICE9 pd3dDevice, CPolyCoord *pLine0, CPolyCoord *pLine1, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate = NULL, D3DXVECTOR3 *vecRotate = NULL);
	void PolyLine(LPDIRECT3DDEVICE9 pd3dDevice, CPolyCoord *pLine, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate = NULL, D3DXVECTOR3 *vecRotate = NULL,float fOffsetAlongNormal = 0.0f, bool bConnectEnds = false);
	void Rod(LPDIRECT3DDEVICE9 pd3dDevice, float fDiameter, float fLength, D3DXVECTOR3 vecNormal, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate = NULL);
	void Cylinder(LPDIRECT3DDEVICE9 pd3dDevice, float fDiameter, float fHeight, D3DXVECTOR3 vecNormal, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate = NULL, bool bShowInside = false);
	void Disc(LPDIRECT3DDEVICE9 pd3dDevice, float fDiameter, D3DXVECTOR3 vecNormal, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate = NULL);
	void Rectangle(LPDIRECT3DDEVICE9 pd3dDevice, D3DXVECTOR3 vecSize, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate = NULL, D3DXVECTOR3 *vecRotate = NULL, bool bShowOppositeFace = false);
	void Cuboid(LPDIRECT3DDEVICE9 pd3dDevice, D3DXVECTOR3 vecSize, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate = NULL, D3DXVECTOR3 *vecRotate = NULL);
	bool Render();

	int					m_nZMode;
	int					m_nCullMode;
	LPDIRECT3DDEVICE9	m_pd3dDevice;
	int					m_nPrimL;
	C3DPrimitive*		m_pPrim;

	bool InvalidateDeviceObjects();
	bool DeleteDeviceObjects();
	C3DPart();
	virtual ~C3DPart();

};

#endif // !defined(AFX_3DPART_H__3C5044E8_F9C5_445F_A451_99133288BA7C__INCLUDED_)
