// 3DPrimitive.cpp: implementation of the C3DPrimitive class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "3DPrimitive.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DPrimitive::C3DPrimitive()
{
	m_pVB = NULL;
	m_nPrimitiveType = 0;
	m_nPrimitiveCount = 0;
	m_nCullMode = D3DCULL_CCW;
	m_nZMode = true;
	m_pd3dDevice = NULL;

}

C3DPrimitive::~C3DPrimitive()
{
	SAFE_RELEASE(m_pVB);
}

bool C3DPrimitive::InvalidateDeviceObjects()
{
//	m_pVB->InvalidateDeviceObjects();

	return true;
}
bool C3DPrimitive::DeleteDeviceObjects()
{
	SAFE_RELEASE(m_pVB);
	
	return true;
}


HRESULT C3DPrimitive::LineStrip(LPDIRECT3DDEVICE9 pd3dDevice, CUSTOMVERTEX *pExternalVertices, int nNumberPts ,int nCullMode, int nZMode)
{
	m_pd3dDevice = pd3dDevice;

	m_pVB				= NULL;
	m_nCullMode			= nCullMode;
	m_nZMode			= nZMode;
	m_nPrimitiveCount	= nNumberPts-1;
	m_nPrimitiveType	= D3DPT_LINESTRIP;


    // Create the vertex buffer.
    if( FAILED( m_pd3dDevice->CreateVertexBuffer( nNumberPts*sizeof(CUSTOMVERTEX),0, D3DFVF_CUSTOMVERTEX,D3DPOOL_DEFAULT, &m_pVB, NULL ) ) ) {
        return E_FAIL;
    }

    // Fill the vertex buffer.
	int nSize = sizeof CUSTOMVERTEX * nNumberPts;
    VOID* pVertices;
    if( FAILED( m_pVB->Lock( 0, nSize, (void**)&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, pExternalVertices, nSize) ;
    m_pVB->Unlock();


    return S_OK;

}


HRESULT C3DPrimitive::FillTriangleStrip(LPDIRECT3DDEVICE9 pd3dDevice, CUSTOMVERTEX *pExternalVertices, int nTrianglesL ,int nCullMode, int nZMode)
{
	m_pd3dDevice = pd3dDevice;

	m_pVB				= NULL;
	m_nCullMode			= nCullMode;
	m_nZMode			= nZMode;
	m_nPrimitiveCount	= nTrianglesL;
	m_nPrimitiveType	= D3DPT_TRIANGLESTRIP;


    // Create the vertex buffer.
    if( FAILED( m_pd3dDevice->CreateVertexBuffer( (nTrianglesL+2)*sizeof(CUSTOMVERTEX),0, D3DFVF_CUSTOMVERTEX,D3DPOOL_DEFAULT, &m_pVB, NULL ) ) ) {
        return E_FAIL;
    }

    // Fill the vertex buffer.
	int nSize = sizeof CUSTOMVERTEX * (nTrianglesL + 2);
    VOID* pVertices;
    if( FAILED( m_pVB->Lock( 0, nSize, (void**)&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, pExternalVertices, nSize) ;
    m_pVB->Unlock();


    return S_OK;

}


HRESULT C3DPrimitive::FillTriangleFan(LPDIRECT3DDEVICE9 pd3dDevice, CUSTOMVERTEX *pExternalVertices, int nTrianglesL, int nCullMode, int nZMode)
{
	m_pd3dDevice = pd3dDevice;

	m_pVB = NULL;
	m_nCullMode = nCullMode;
	m_nZMode = nZMode;
	m_nPrimitiveCount	= nTrianglesL;
	m_nPrimitiveType	= D3DPT_TRIANGLEFAN;

    // Create the vertex buffer.
    if( FAILED( m_pd3dDevice->CreateVertexBuffer( (nTrianglesL+2)*sizeof(CUSTOMVERTEX),0, D3DFVF_CUSTOMVERTEX,D3DPOOL_DEFAULT, &m_pVB, NULL ) ) ) {
        return E_FAIL;
    }

    // Fill the vertex buffer.
	int nSize = sizeof CUSTOMVERTEX * (nTrianglesL + 2);
    VOID* pVertices;
    if( FAILED( m_pVB->Lock( 0, nSize, (void**)&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, pExternalVertices, nSize) ;
    m_pVB->Unlock();

    return S_OK;

}

void C3DPrimitive::Draw3DLine(LPDIRECT3DDEVICE9 pd3dDevice,float fX0,float fY0,float fZ0,float fX1,float fY1,float fZ1,int nColor)
{
	m_pd3dDevice = pd3dDevice;

	CUSTOMVERTEX Vertex[2];


	Vertex[0].vecPos.x=fX0;
	Vertex[0].vecPos.y=fY0;
	Vertex[0].vecPos.z=fZ0;
	Vertex[0].dwColor=nColor;
	Vertex[1].vecPos.x=fX1;
	Vertex[1].vecPos.y=fY1;
	Vertex[1].vecPos.z=fZ1;
	Vertex[1].dwColor=nColor;
	Vertex[0].vecNorm.x = 0.0f;
	Vertex[0].vecNorm.y = 1.0f;
	Vertex[0].vecNorm.z = 0.0f;
	Vertex[1].vecNorm.x = 0.0f;
	Vertex[1].vecNorm.y = 1.0f;
	Vertex[1].vecNorm.z = 0.0f;

	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
    m_pd3dDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, Vertex, sizeof(CUSTOMVERTEX) );


}

void C3DPrimitive::Draw3DLine(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 vec0,D3DXVECTOR3 vec1,int nColor)
{
	m_pd3dDevice = pd3dDevice;

	CUSTOMVERTEX Vertex[2];


	Vertex[0].vecPos = vec0;
	Vertex[0].dwColor=nColor;
	Vertex[1].vecPos = vec1;
	Vertex[1].dwColor=nColor;
	Vertex[0].vecNorm.x = 0.0f;
	Vertex[0].vecNorm.y = 1.0f;
	Vertex[0].vecNorm.z = 0.0f;
	Vertex[1].vecNorm.x = 0.0f;
	Vertex[1].vecNorm.y = 1.0f;
	Vertex[1].vecNorm.z = 0.0f;

	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
    m_pd3dDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, Vertex, sizeof(CUSTOMVERTEX) );


}

void C3DPrimitive::Draw3DCircle(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 Centre, D3DXVECTOR3 Normal, float fRadius,int nColor)
{
	m_pd3dDevice = pd3dDevice;

	D3DXMATRIXA16 matWorld,matNormal,matTranslation,matRotX,matRotY,matRotZ;
	D3DXVECTOR3 vecP;
	CUSTOMVERTEX Vertex[102];
	int nn;
	float fAngle;
	float fYaw,fPitch;

	//
	fYaw = atan2f(Normal.y,Normal.x);
	fPitch = acosf(Normal.z);
	D3DXMatrixRotationY(&matRotY,fPitch);
	D3DXMatrixRotationZ(&matRotZ,fYaw);
	D3DXMatrixMultiply(&matNormal,&matRotY,&matRotZ);
	D3DXMatrixTranslation(&matTranslation,Centre.x,Centre.y,Centre.z);
	D3DXMatrixMultiply(&matNormal,&matNormal,&matTranslation);


	for(fAngle = 0.0f,nn=0;fAngle <= 2.0f*PIf ; fAngle += (2.0f*PIf/100.0f), nn++) {
		D3DXMatrixRotationZ(&matWorld,fAngle);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matNormal);

		vecP.x = fRadius;
		vecP.y = 0.0f;
		vecP.z = 0.0f;

		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);

		Vertex[nn].vecPos.x = vecP.x;
		Vertex[nn].vecPos.y = vecP.y;
		Vertex[nn].vecPos.z = vecP.z;
		Vertex[nn].dwColor = nColor;
		Vertex[nn].vecNorm.x = 0.0f;
		Vertex[nn].vecNorm.y = 1.0f;
		Vertex[nn].vecNorm.z = 0.0f;
	}

	Vertex[nn++] = Vertex[0];

	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
    m_pd3dDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, nn-1, Vertex, sizeof(CUSTOMVERTEX) );


}

void C3DPrimitive::Draw3DCone(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 Centre, D3DXVECTOR3 Normal, float fRadius,float fHeight,int nColor)
{
	m_pd3dDevice = pd3dDevice;

	int nNumberTriangles = 100;
	D3DXMATRIXA16 matWorld,matNormal,matTranslation;
	D3DXVECTOR3 vecN,vecP,vecA,vecB,vecT;
	CUSTOMVERTEX Vertex[110];
	int nn;
	float fAngle;

	D3DXVec3Normalize(&vecN,&Normal);

	float fYaw = atan2f(vecN.x, vecN.z);
	float fPitch = atan2f(-vecN.y , sqrtf(vecN.x * vecN.x + vecN.z * vecN.z));

	D3DXMatrixRotationYawPitchRoll( &matNormal, fYaw, fPitch, 0 );
	D3DXMatrixTranslation(&matTranslation,Centre.x,Centre.y,Centre.z);
	D3DXMatrixMultiply(&matNormal,&matNormal,&matTranslation);


	vecP.x=0.0;
	vecP.y=0.0;
	vecP.z=fHeight;

	D3DXVec3TransformCoord(&vecP,&vecP,&matNormal);

	Vertex[nn=0].vecPos.x = vecP.x;
	Vertex[nn].vecPos.y = vecP.y;
	Vertex[nn].vecPos.z = vecP.z;
	Vertex[nn++].dwColor = nColor;

	for(nn=0;nn<=nNumberTriangles ; nn++) {
		fAngle = (2.0f * PIf) * (float)nn / (float)nNumberTriangles;
		D3DXMatrixRotationZ(&matWorld,fAngle);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matNormal);

		vecP.x = fRadius;
		vecP.y = 0.0f;
		vecP.z = 0.0f;

		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
		Vertex[nn+1].vecPos= vecP;
		Vertex[nn+1].dwColor = nColor;
	}
	Vertex[nn+1] = Vertex[1];

	int nNode = ++nn;

	for(nn=1;nn<(nNode-1);nn++) {
		vecA = Vertex[nn+1].vecPos - Vertex[0].vecPos;
		vecB = Vertex[nn+0].vecPos - Vertex[0].vecPos;
		D3DXVec3Normalize(&vecB,&vecB);
		D3DXVec3Normalize(&vecA,&vecA);
		D3DXVec3Cross(&vecT, &vecB, &vecA);
		D3DXVec3Normalize(&vecT,&vecT);
		Vertex[nn].vecNorm = vecT;
	}
	Vertex[nNode-1].vecNorm = vecT;
	Vertex[0].vecNorm = vecT;

	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, true );
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
    m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, nNode-1, Vertex, sizeof(CUSTOMVERTEX) );


}

void C3DPrimitive::Draw3DTube(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 vec0, D3DXVECTOR3 vec1,int nColor, float fRadius0, float fRadius1)
{
#define NN  1000

	m_pd3dDevice = pd3dDevice;

	D3DXMATRIXA16 matWorld0,matTranslation0,matNormal0;
	D3DXMATRIXA16 matWorld1,matTranslation1,matNormal1;
	D3DXVECTOR3 vecN,vecP,vecA,vecB,vecT;
	CUSTOMVERTEX Vertex[NN+4];
	float fAngle;
	int nNode, nn;

	vecN = vec1 - vec0;
	float fLength = D3DXVec3Length(&vecN);
	D3DXVec3Normalize(&vecN,&vecN);


	float fYaw = atan2f(vecN.x, vecN.z);
	float fPitch = atan2f(-vecN.y , sqrtf(vecN.x * vecN.x + vecN.z * vecN.z));

	D3DXMatrixRotationYawPitchRoll( &matNormal0, fYaw, fPitch, 0 );
	D3DXMatrixTranslation(&matTranslation0,vec0.x,vec0.y,vec0.z);
	D3DXMatrixMultiply(&matNormal0,&matNormal0,&matTranslation0);

	D3DXMatrixTranslation(&matTranslation1,vec1.x,vec1.y,vec1.z);
	D3DXMatrixRotationYawPitchRoll( &matNormal1, fYaw, fPitch, 0 );
	D3DXMatrixMultiply(&matNormal1,&matNormal1,&matTranslation1);

	for(nNode=0,nn=0;nNode<NN/2;nNode++) {
		fAngle = (float)nNode * (2.0f*PIf) / (float)((NN/2)-1);
		D3DXMatrixRotationZ(&matWorld0,fAngle);
		D3DXMatrixMultiply(&matWorld0,&matWorld0,&matNormal0);
		D3DXMatrixRotationZ(&matWorld1,fAngle);
		D3DXMatrixMultiply(&matWorld1,&matWorld1,&matNormal1);

		vecP.x = fRadius0;
		vecP.y = 0.0f;
		vecP.z = 0.0f;
		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld0);
		Vertex[nn].vecPos.x = vecP.x;
		Vertex[nn].vecPos.y = vecP.y;
		Vertex[nn].vecPos.z = vecP.z;
		Vertex[nn].dwColor = nColor;
		nn++;

		vecP.x = fRadius1;
		vecP.y = 0.0f;
		vecP.z = 0.0f;
		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld1);
		Vertex[nn].vecPos.x = vecP.x;
		Vertex[nn].vecPos.y = vecP.y;
		Vertex[nn].vecPos.z = vecP.z;
		Vertex[nn].dwColor = nColor;
		nn++;
	}


	for(nn=0;nn<((nNode*2)+1);nn+=2) {
		vecA = Vertex[nn+2].vecPos - Vertex[nn+1].vecPos;
		vecB = Vertex[nn+0].vecPos - Vertex[nn+1].vecPos;
		D3DXVec3Normalize(&vecB,&vecB);
		D3DXVec3Normalize(&vecA,&vecA);
		D3DXVec3Cross(&vecT, &vecB, &vecA);
		D3DXVec3Normalize(&vecT,&vecT);
		Vertex[nn].vecNorm = vecT;
		Vertex[nn+1].vecNorm = vecT;
	}

	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, true );
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
//	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);

	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
    m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, nNode*2-2, Vertex, sizeof(CUSTOMVERTEX) );


}

void C3DPrimitive::Draw3DArc(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 Centre, D3DXVECTOR3 vNode0, D3DXVECTOR3 vNode1,int nColor)
{
	m_pd3dDevice = pd3dDevice;

	D3DXMATRIXA16 matWorld,matNormal,matTranslation,matRotX,matRotY,matRotZ;
	D3DXVECTOR3 vecP,vecQ;
	CUSTOMVERTEX Vertex[102];
	int nn;

	vecP = vNode0 - Centre;
	vecQ = vNode1 - Centre;
	float fRadius = D3DXVec3Length(&vecP);
	D3DXVec3Normalize(&vecP,&vecP);
	D3DXVec3Normalize(&vecQ,&vecQ);

	for(nn=0;nn<100;nn++) {
		Vertex[nn].vecPos.x = (vecQ.x - vecP.x) * (float)nn / 99.0f + vecP.x;
		Vertex[nn].vecPos.y = (vecQ.y - vecP.y) * (float)nn / 99.0f + vecP.y;
		Vertex[nn].vecPos.z = (vecQ.z - vecP.z) * (float)nn / 99.0f + vecP.z;
		D3DXVec3Normalize(&Vertex[nn].vecPos,&Vertex[nn].vecPos);
		Vertex[nn].vecPos *= fRadius;
		Vertex[nn].vecPos += Centre;

		Vertex[nn].dwColor = nColor;
		Vertex[nn].vecNorm.x = 0.0f;
		Vertex[nn].vecNorm.y = 1.0f;
		Vertex[nn].vecNorm.z = 0.0f;
	}

	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
    m_pd3dDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, nn-1, Vertex, sizeof(CUSTOMVERTEX) );


}

void C3DPrimitive::Draw3DFan(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 Centre, D3DXVECTOR3 vNode0, D3DXVECTOR3 vNode1,int nColor)
{
	m_pd3dDevice = pd3dDevice;

	D3DXMATRIXA16 matWorld,matNormal,matTranslation,matRotX,matRotY,matRotZ;
	D3DXVECTOR3 vecP,vecQ,vecT;
	CUSTOMVERTEX Vertex[102];
	int nn;

	vecP = vNode0 - Centre;
	vecQ = vNode1 - Centre;
	float fRadius = D3DXVec3Length(&vecP);
	D3DXVec3Normalize(&vecP,&vecP);
	D3DXVec3Normalize(&vecQ,&vecQ);

	D3DXVec3Cross(&vecT, &vecQ, &vecP);
	D3DXVec3Normalize(&vecT,&vecT);

	Vertex[0].vecPos = Centre;
	Vertex[0].dwColor = nColor;
	Vertex[0].vecNorm = vecT;

	for(nn=0;nn<100;nn++) {
		Vertex[nn+1].vecPos.x = (vecQ.x - vecP.x) * (float)nn / 99.0f + vecP.x;
		Vertex[nn+1].vecPos.y = (vecQ.y - vecP.y) * (float)nn / 99.0f + vecP.y;
		Vertex[nn+1].vecPos.z = (vecQ.z - vecP.z) * (float)nn / 99.0f + vecP.z;
		D3DXVec3Normalize(&Vertex[nn+1].vecPos,&Vertex[nn+1].vecPos);
		Vertex[nn+1].vecPos *= fRadius;
		Vertex[nn+1].vecPos += Centre;

		Vertex[nn+1].dwColor = nColor;
		Vertex[nn+1].vecNorm = vecT;
	}

	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, true );
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
    m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, nn-1, Vertex, sizeof(CUSTOMVERTEX) );


}

void C3DPrimitive::Draw3DRod(LPDIRECT3DDEVICE9 pd3dDevice,D3DXVECTOR3 vec0, D3DXVECTOR3 vec1,int nColor, float fRadius0, float fRadius1)
{
#define NN  1000

	m_pd3dDevice = pd3dDevice;

	D3DXMATRIXA16 matWorld0,matTranslation0,matNormal0;
	D3DXMATRIXA16 matWorld1,matTranslation1,matNormal1;
	D3DXVECTOR3 vecN,vecP,vecA,vecB,vecT;
	CUSTOMVERTEX Vertex[NN+4];
	float fAngle;
	int nNode, nn;

	vecN = vec1 - vec0;
	float fLength = D3DXVec3Length(&vecN);
	D3DXVec3Normalize(&vecN,&vecN);


	float fYaw = atan2f(vecN.x, vecN.z);
	float fPitch = atan2f(-vecN.y , sqrtf(vecN.x * vecN.x + vecN.z * vecN.z));

	D3DXMatrixRotationYawPitchRoll( &matNormal0, fYaw, fPitch, 0 );
	D3DXMatrixTranslation(&matTranslation0,vec0.x,vec0.y,vec0.z);
	D3DXMatrixMultiply(&matNormal0,&matNormal0,&matTranslation0);

	D3DXMatrixTranslation(&matTranslation1,vec1.x,vec1.y,vec1.z);
	D3DXMatrixRotationYawPitchRoll( &matNormal1, fYaw, fPitch, 0 );
	D3DXMatrixMultiply(&matNormal1,&matNormal1,&matTranslation1);

	for(nNode=0,nn=0;nNode<NN/2;nNode++) {
		fAngle = (float)nNode * (2.0f*PIf) / (float)((NN/2)-1);
		D3DXMatrixRotationZ(&matWorld0,fAngle);
		D3DXMatrixMultiply(&matWorld0,&matWorld0,&matNormal0);
		D3DXMatrixRotationZ(&matWorld1,fAngle);
		D3DXMatrixMultiply(&matWorld1,&matWorld1,&matNormal1);

		vecP.x = fRadius0;
		vecP.y = 0.0f;
		vecP.z = 0.0f;
		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld0);
		Vertex[nn].vecPos.x = vecP.x;
		Vertex[nn].vecPos.y = vecP.y;
		Vertex[nn].vecPos.z = vecP.z;
		Vertex[nn].dwColor = nColor;
		nn++;

		vecP.x = fRadius1;
		vecP.y = 0.0f;
		vecP.z = 0.0f;
		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld1);
		Vertex[nn].vecPos.x = vecP.x;
		Vertex[nn].vecPos.y = vecP.y;
		Vertex[nn].vecPos.z = vecP.z;
		Vertex[nn].dwColor = nColor;
		nn++;
	}


	for(nn=0;nn<((nNode*2)+1);nn+=2) {
		vecA = Vertex[nn+2].vecPos - Vertex[nn+1].vecPos;
		vecB = Vertex[nn+0].vecPos - Vertex[nn+1].vecPos;
		D3DXVec3Normalize(&vecB,&vecB);
		D3DXVec3Normalize(&vecA,&vecA);
		D3DXVec3Cross(&vecT, &vecB, &vecA);
		D3DXVec3Normalize(&vecT,&vecT);
		Vertex[nn].vecNorm = vecT;
		Vertex[nn+1].vecNorm = vecT;
	}

	m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, true );
	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
//	m_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);

	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
    m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, nNode*2-2, Vertex, sizeof(CUSTOMVERTEX) );



	int nNumberTriangles = 500;

	//Base Disc
	vecP.x=0.0f;
	vecP.y=0.0f;
	vecP.z=0.0f;

	D3DXVec3TransformCoord(&vecP,&vecP,&matNormal0);

	Vertex[nn=0].vecPos.x = vecP.x;
	Vertex[nn].vecPos.y = vecP.y;
	Vertex[nn].vecPos.z = vecP.z;
	Vertex[nn++].dwColor = nColor;

	for(nn=0;nn<=nNumberTriangles ; nn++) {
		fAngle = (2.0f * PIf) * (float)nn / (float)nNumberTriangles;
		D3DXMatrixRotationZ(&matWorld0,fAngle);
		D3DXMatrixMultiply(&matWorld0,&matWorld0,&matNormal0);

		vecP.x = fRadius0;
		vecP.y = 0.0f;
		vecP.z = 0.0f;

		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld0);
		Vertex[nn+1].vecPos= vecP;
		Vertex[nn+1].dwColor = nColor;
	}
	Vertex[nn+1] = Vertex[1];

	nNode = ++nn;

	for(nn=1;nn<(nNode-1);nn++) {
		vecA = Vertex[nn+0].vecPos - Vertex[0].vecPos;
		vecB = Vertex[nn+1].vecPos - Vertex[0].vecPos;
		D3DXVec3Normalize(&vecB,&vecB);
		D3DXVec3Normalize(&vecA,&vecA);
		D3DXVec3Cross(&vecT, &vecB, &vecA);
		D3DXVec3Normalize(&vecT,&vecT);
		Vertex[nn].vecNorm = vecT;
	}
	Vertex[nNode-1].vecNorm = vecT;
	Vertex[0].vecNorm = vecT;

    m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, nNode-1, Vertex, sizeof(CUSTOMVERTEX) );

	//Top Disc
	vecP.x=0.0f;
	vecP.y=0.0f;
	vecP.z=0.0f;

	D3DXVec3TransformCoord(&vecP,&vecP,&matNormal1);

	Vertex[nn=0].vecPos.x = vecP.x;
	Vertex[nn].vecPos.y = vecP.y;
	Vertex[nn].vecPos.z = vecP.z;
	Vertex[nn++].dwColor = nColor;

	for(nn=0;nn<=nNumberTriangles ; nn++) {
		fAngle = (2.0f * PIf) * (float)nn / (float)nNumberTriangles;
		D3DXMatrixRotationZ(&matWorld1,fAngle);
		D3DXMatrixMultiply(&matWorld1,&matWorld1,&matNormal1);

		vecP.x = fRadius1;
		vecP.y = 0.0f;
		vecP.z = 0.0f;

		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld1);
		Vertex[nn+1].vecPos= vecP;
		Vertex[nn+1].dwColor = nColor;
	}
	Vertex[nn+1] = Vertex[1];

	nNode = ++nn;

	for(nn=1;nn<(nNode-1);nn++) {
		vecA = Vertex[nn+1].vecPos - Vertex[0].vecPos;
		vecB = Vertex[nn+0].vecPos - Vertex[0].vecPos;
		D3DXVec3Normalize(&vecB,&vecB);
		D3DXVec3Normalize(&vecA,&vecA);
		D3DXVec3Cross(&vecT, &vecB, &vecA);
		D3DXVec3Normalize(&vecT,&vecT);
		Vertex[nn].vecNorm = vecT;
	}
	Vertex[nNode-1].vecNorm = vecT;
	Vertex[0].vecNorm = vecT;

    m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, nNode-1, Vertex, sizeof(CUSTOMVERTEX) );

}