// 3DPrimitive.h: interface for the C3DPrimitive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DPRIMITIVE_H__DA3B51EB_DA91_48EC_9577_B08FDE2D81FD__INCLUDED_)
#define AFX_3DPRIMITIVE_H__DA3B51EB_DA91_48EC_9577_B08FDE2D81FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class C3DPrimitive  
{
public:
	void Draw3DRod(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 vec0, D3DXVECTOR3 vec1,int nColor, float fRadius0,float fRadius1);
	void Draw3DFan(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 Centre, D3DXVECTOR3 vNode0, D3DXVECTOR3 vNode1,int nColor);
	void Draw3DArc(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 Centre, D3DXVECTOR3 vNode0, D3DXVECTOR3 vNode1,int nColor);
	void Draw3DTube(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 vec0, D3DXVECTOR3 vec1,int nColor, float fRadius0,float fRadius1);
	void Draw3DCone(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 Centre, D3DXVECTOR3 Normal, float fRadius,float fHeight,int nColor);
	void Draw3DCircle(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 Centre, D3DXVECTOR3 Normal, float fRadius,int nColor);
	void Draw3DLine(LPDIRECT3DDEVICE9 pd3dDevice,float fX0,float fY0,float fZ0,float fX1,float fY1,float fZ1,int nColor);
	void Draw3DLine(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 vec0,D3DXVECTOR3 vec1,int nColor);

	HRESULT LineStrip(LPDIRECT3DDEVICE9 pd3dDevice, CUSTOMVERTEX *pExternalVertices, int nNumberPts ,int nCullMode=D3DCULL_CCW, int nZMode=TRUE);
	HRESULT FillTriangleFan(LPDIRECT3DDEVICE9 pd3dDevice, CUSTOMVERTEX *pExternalVertices, int nTrianglesL ,int nCullMode=D3DCULL_CCW, int nZMode=TRUE);
	HRESULT FillTriangleStrip(LPDIRECT3DDEVICE9 pd3dDevice,CUSTOMVERTEX *pExternalVertices, int nTrianglesL ,int nCullMode=D3DCULL_CCW, int nZMode=TRUE);
	inline void operator = (C3DPrimitive Prim) {
		m_pd3dDevice		= Prim.m_pd3dDevice;
		m_nPrimitiveType	= Prim.m_nPrimitiveType;
		m_nPrimitiveCount	= Prim.m_nPrimitiveCount;
		m_nCullMode			= Prim.m_nCullMode;
		m_nZMode			= Prim.m_nZMode;
		m_pVB				= Prim.m_pVB;
	};

	LPDIRECT3DDEVICE9       m_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	int	m_nPrimitiveType;
	int	m_nPrimitiveCount;
	int	m_nCullMode;
	int	m_nZMode;

//	HRESULT FillTriangleStrip(CUSTOMVERTEX *pExternalVertices, int nTrianglesL ,int nCullMode, int nZMode,bool bComponent);
	bool InvalidateDeviceObjects();
	bool DeleteDeviceObjects();
	C3DPrimitive();
	virtual ~C3DPrimitive();

};

#endif // !defined(AFX_3DPRIMITIVE_H__DA3B51EB_DA91_48EC_9577_B08FDE2D81FD__INCLUDED_)
