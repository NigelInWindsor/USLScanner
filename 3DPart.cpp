// 3DPart.cpp: implementation of the C3DPart class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "3DPart.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DPart::C3DPart()
{
	m_nPrimL = 0;
	m_pPrim = NULL;
	m_pd3dDevice = NULL;
	m_nCullMode = D3DCULL_CCW;
	m_nZMode = true;

}

C3DPart::~C3DPart()
{
	SAFE_DELETE_ARRAY(m_pPrim);
}

bool C3DPart::InvalidateDeviceObjects()
{
	for(int ii=0;ii<m_nPrimL;ii++) {
		m_pPrim[ii].InvalidateDeviceObjects();
	}
	return true;
}

bool C3DPart::DeleteDeviceObjects()
{
	for(int ii=0;ii<m_nPrimL;ii++) {
		m_pPrim[ii].DeleteDeviceObjects();
	}
	
	return true;
}

inline int C3DPart::MinMax(int *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}

bool C3DPart::Render()
{

	for(int ii=0;ii<m_nPrimL;ii++) {

		m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, m_pPrim[ii].m_nCullMode );
		m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, m_pPrim[ii].m_nZMode );

		m_pd3dDevice->SetStreamSource( 0, m_pPrim[ii].m_pVB, 0, sizeof(CUSTOMVERTEX) );
		m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

		m_pd3dDevice->DrawPrimitive( (D3DPRIMITIVETYPE)m_pPrim[ii].m_nPrimitiveType, 0, m_pPrim[ii].m_nPrimitiveCount );
	}

	return true;
}



//Either define it as a size and then rotate it and translate it
//Or if its orthogonal to the axes define it as size and simply translate it

void C3DPart::Rectangle(LPDIRECT3DDEVICE9 pd3dDevice, D3DXVECTOR3 vecSize, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate, D3DXVECTOR3 *vecRotate, bool bShowOppositeFace)
{
	D3DXVECTOR3 vecT,vecA,vecB;
	D3DXMATRIXA16 matWorld,matTranslation,matRotation;

	m_pd3dDevice = pd3dDevice;
	m_pPrim = new C3DPrimitive[m_nPrimL = 1];

	CUSTOMVERTEX *pVxCorners = new CUSTOMVERTEX[4];
	ZeroMemory(pVxCorners, sizeof CUSTOMVERTEX * 4);

	if(m_pPrim != NULL) {

		D3DXMatrixIdentity(&matWorld);
		if(vecRotate) {
			float fYaw = atan2f(-vecRotate->z, vecRotate->x);
			float fRoll = atan2f(vecRotate->y, sqrtf(vecRotate->x * vecRotate->x + vecRotate->z * vecRotate->z));
			D3DXMatrixRotationYawPitchRoll( &matWorld, fYaw, 0.0f, fRoll );
		}
		if(vecTranslate) {
			D3DXMatrixTranslation(&matTranslation,vecTranslate->x,vecTranslate->y,vecTranslate->z);
			D3DXMatrixMultiply(&matWorld,&matWorld,&matTranslation);
		}

		if(vecSize.z == 0.0f) {
			pVxCorners[1].vecPos.y = vecSize.y;
			pVxCorners[2].vecPos.x = vecSize.x;
			pVxCorners[3].vecPos.x = vecSize.x;
			pVxCorners[3].vecPos.y = vecSize.y;
		} else {
			pVxCorners[1].vecPos.z = vecSize.z;
			pVxCorners[2].vecPos.x = vecSize.x;
			pVxCorners[2].vecPos.y = vecSize.y;
			pVxCorners[3].vecPos.x = vecSize.x;
			pVxCorners[3].vecPos.y = vecSize.y;
			pVxCorners[3].vecPos.z = vecSize.z;
		}

		for(int ii=0;ii<4;ii++) {
			D3DXVec3TransformCoord(&pVxCorners[ii].vecPos,&pVxCorners[ii].vecPos,&matWorld);
			pVxCorners[ii].dwColor = rgb;
		}


		vecA = pVxCorners[1].vecPos - pVxCorners[0].vecPos;
		vecB = pVxCorners[2].vecPos - pVxCorners[0].vecPos;
		D3DXVec3Cross(&vecT, &vecB, &vecA);
		D3DXVec3Normalize(&vecT,&vecT);
		if(bShowOppositeFace == true) vecT *= -1.0f;
		for(int ii=0;ii<4;ii++) {
			pVxCorners[ii].vecNorm = vecT;
		}
		m_pPrim[0].FillTriangleStrip(m_pd3dDevice,pVxCorners,2,m_nCullMode,m_nZMode);

	}

	delete pVxCorners;
}




//Either define it as a size and then rotate it and translate it
//Or if its orthogonal to the axes define it as size and simply translate it

void C3DPart::Cuboid(LPDIRECT3DDEVICE9 pd3dDevice, D3DXVECTOR3 vecSize, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate, D3DXVECTOR3 *vecRotate)
{
	D3DXVECTOR3 vecT,vecA,vecB;
	D3DXMATRIXA16 matWorld,matTranslation,matRotation;

	m_pd3dDevice = pd3dDevice;
	m_pPrim = new C3DPrimitive[m_nPrimL = 6];

	CUSTOMVERTEX *pVxCorners = new CUSTOMVERTEX[8];
	CUSTOMVERTEX *pVxFace = new CUSTOMVERTEX[4];
	ZeroMemory(pVxCorners, sizeof CUSTOMVERTEX * 8);

	if(m_pPrim != NULL) {

		D3DXMatrixIdentity(&matWorld);
		if(vecRotate) {
			float fYaw = atan2f(-vecRotate->z, vecRotate->x);
			float fRoll = atan2f(vecRotate->y, sqrtf(vecRotate->x * vecRotate->x + vecRotate->z * vecRotate->z));
			D3DXMatrixRotationYawPitchRoll( &matWorld, fYaw, 0.0f, fRoll );
		}
		if(vecTranslate) {
			D3DXMatrixTranslation(&matTranslation,vecTranslate->x,vecTranslate->y,vecTranslate->z);
			D3DXMatrixMultiply(&matWorld,&matWorld,&matTranslation);
		}

		pVxCorners[1].vecPos.z = vecSize.z;
		pVxCorners[2].vecPos.x = vecSize.x;
		pVxCorners[3].vecPos.x = vecSize.x;
		pVxCorners[3].vecPos.z = vecSize.z;
		pVxCorners[4].vecPos.x = vecSize.x;
		pVxCorners[4].vecPos.y = vecSize.y;
		pVxCorners[5].vecPos.x = vecSize.x;
		pVxCorners[5].vecPos.y = vecSize.y;
		pVxCorners[5].vecPos.z = vecSize.z;
		pVxCorners[6].vecPos.y = vecSize.y;
		pVxCorners[7].vecPos.y = vecSize.y;
		pVxCorners[7].vecPos.z = vecSize.z;
		for(int ii=0;ii<8;ii++) {
			D3DXVec3TransformCoord(&pVxCorners[ii].vecPos,&pVxCorners[ii].vecPos,&matWorld);
			pVxCorners[ii].dwColor = rgb;
		}

		for(int nFace = 0;nFace<6;nFace++ ){
			switch(nFace) {
			case 0:		//Front
				pVxFace[0] = pVxCorners[0];	//0
				pVxFace[1] = pVxCorners[1];	//1
				pVxFace[2] = pVxCorners[2];	//2
				pVxFace[3] = pVxCorners[3];	//3
				break;
			case 1:	//Back
				pVxFace[0] = pVxCorners[4];	//4
				pVxFace[1] = pVxCorners[5];	//5
				pVxFace[2] = pVxCorners[6];	//6
				pVxFace[3] = pVxCorners[7];	//7
				break;
			case 2:	//Left
				pVxFace[0] = pVxCorners[6];
				pVxFace[1] = pVxCorners[7];
				pVxFace[2] = pVxCorners[0];
				pVxFace[3] = pVxCorners[1];
				break;
			case 3:	//Right
				pVxFace[0] = pVxCorners[2];
				pVxFace[1] = pVxCorners[3];
				pVxFace[2] = pVxCorners[4];
				pVxFace[3] = pVxCorners[5];
				break;
			case 4:	//Bottom
				pVxFace[0] = pVxCorners[6];
				pVxFace[1] = pVxCorners[0];
				pVxFace[2] = pVxCorners[4];
				pVxFace[3] = pVxCorners[2];
				break;
			case 5: //Top
				pVxFace[0] = pVxCorners[1];
				pVxFace[1] = pVxCorners[7];
				pVxFace[2] = pVxCorners[3];
				pVxFace[3] = pVxCorners[5];
				break;
			}

			vecA = pVxFace[1].vecPos - pVxFace[0].vecPos;
			vecB = pVxFace[2].vecPos - pVxFace[0].vecPos;
			D3DXVec3Cross(&vecT, &vecB, &vecA);
			D3DXVec3Normalize(&vecT,&vecT);
			for(int ii=0;ii<4;ii++) {
				pVxFace[ii].vecNorm = vecT;
			}
			m_pPrim[nFace].FillTriangleStrip(m_pd3dDevice,pVxFace,2,m_nCullMode,m_nZMode);
		}

	}

	delete pVxCorners;
	delete pVxFace;
}

void C3DPart::Disc(LPDIRECT3DDEVICE9 pd3dDevice, float fDiameter, D3DXVECTOR3 vecNormal, D3DXCOLOR rgb,D3DXVECTOR3 *vecTranslate)
{
	int nNumberTriangles = 100;
	D3DXMATRIXA16 matWorld,matNormal,matTranslation;
	int nn;
	float fAngle;
	D3DXVECTOR3 vecA,vecB,vecT;

	m_pd3dDevice = pd3dDevice;
	m_pPrim = new C3DPrimitive[m_nPrimL = 1];

	CUSTOMVERTEX *pVxCorners = new CUSTOMVERTEX[nNumberTriangles+2];
	ZeroMemory(pVxCorners, sizeof CUSTOMVERTEX * (nNumberTriangles+2));

	if(m_pPrim != NULL) {
		float fYaw = atan2f(vecNormal.x, vecNormal.z);
		float fPitch = atan2f(-vecNormal.y , sqrtf(vecNormal.x * vecNormal.x + vecNormal.z * vecNormal.z));
		D3DXMatrixRotationYawPitchRoll( &matNormal, fYaw, fPitch, 0 );

		if(vecTranslate) {
			D3DXMatrixTranslation(&matTranslation,vecTranslate->x,vecTranslate->y,vecTranslate->z);
			D3DXMatrixMultiply(&matNormal,&matNormal,&matTranslation);
		}

		for(fAngle = 0.0f,nn=0;fAngle <= 2.0f*PIf ; fAngle += (2.0f*PIf/(float)nNumberTriangles), nn++) {
			D3DXMatrixRotationZ(&matWorld,-fAngle);
			D3DXMatrixMultiply(&matWorld,&matWorld,&matNormal);

			pVxCorners[nn].vecPos.x = fDiameter / 2.0f;
			pVxCorners[nn].vecPos.y = 0;
			pVxCorners[nn].vecPos.z = 0;

			D3DXVec3TransformCoord(&pVxCorners[nn].vecPos,&pVxCorners[nn].vecPos,&matWorld);

			pVxCorners[nn].dwColor = rgb;
		}

		pVxCorners[nn++] = pVxCorners[0];

		vecA = pVxCorners[1].vecPos - pVxCorners[0].vecPos;
		vecB = pVxCorners[2].vecPos - pVxCorners[0].vecPos;
		D3DXVec3Cross(&vecT, &vecB, &vecA);
		D3DXVec3Normalize(&vecT,&vecT);
		for(nn=0;nn<=nNumberTriangles;nn++) {
			pVxCorners[nn].vecNorm = vecT;
		}

		m_pPrim[0].FillTriangleFan(m_pd3dDevice, pVxCorners,nn-1,m_nCullMode,m_nZMode);
	}

	delete pVxCorners;
}

void C3DPart::Cylinder(LPDIRECT3DDEVICE9 pd3dDevice, float fDiameter, float fHeight, D3DXVECTOR3 vecNormal, D3DXCOLOR rgb,D3DXVECTOR3 *vecTranslate, bool bShowInside)
{
	int nNumberTriangles = 100;
	int	nNumberNodes = nNumberTriangles + 2;
	D3DXMATRIXA16 matWorld,matNormal,matTranslation;
	float fAngle;
	D3DXVECTOR3 vecA,vecB,vecT;
	int nNode, nn;

	m_pd3dDevice = pd3dDevice;
	m_pPrim = new C3DPrimitive[m_nPrimL = 1];

	CUSTOMVERTEX *pVxCorners = new CUSTOMVERTEX[nNumberTriangles+2];
	ZeroMemory(pVxCorners, sizeof CUSTOMVERTEX * (nNumberTriangles+2));

	if(m_pPrim != NULL) {
		float fYaw = atan2f(vecNormal.x, vecNormal.z);
		float fPitch = atan2f(-vecNormal.y , sqrtf(vecNormal.x * vecNormal.x + vecNormal.z * vecNormal.z));
		D3DXMatrixRotationYawPitchRoll( &matNormal, fYaw, fPitch, 0 );

		if(vecTranslate) {
			D3DXMatrixTranslation(&matTranslation,vecTranslate->x,vecTranslate->y,vecTranslate->z);
			D3DXMatrixMultiply(&matNormal,&matNormal,&matTranslation);
		}

		for(nNode=0,nn=0;nNode<50;nNode++,nn++) {
			fAngle = (float)nNode * (2.0f*PIf) / 49.0f;
			D3DXMatrixRotationZ(&matWorld,-fAngle);
			D3DXMatrixMultiply(&matWorld,&matWorld,&matNormal);

			pVxCorners[nn].vecPos.x = fDiameter / 2.0f;
			pVxCorners[nn].vecPos.y = 0.0f;
			pVxCorners[nn].vecPos.z = fHeight;
			D3DXVec3TransformCoord(&pVxCorners[nn].vecPos,&pVxCorners[nn].vecPos,&matWorld);
			pVxCorners[nn].dwColor = rgb;

			nn++;
			pVxCorners[nn].vecPos.x = fDiameter / 2.0f;
			pVxCorners[nn].vecPos.y = 0.0f;
			pVxCorners[nn].vecPos.z = 0.0f;
			D3DXVec3TransformCoord(&pVxCorners[nn].vecPos,&pVxCorners[nn].vecPos,&matWorld);
			pVxCorners[nn].dwColor = rgb;
		}


		nNumberTriangles = nn-2;
		nNumberNodes = nn;

		for(nn=0;nn<(nNumberNodes+2);nn+=2) {
			vecA = pVxCorners[nn+2].vecPos - pVxCorners[nn+1].vecPos;
			vecB = pVxCorners[nn+0].vecPos - pVxCorners[nn+1].vecPos;
			D3DXVec3Cross(&vecT, &vecB, &vecA);
			D3DXVec3Normalize(&vecT,&vecT);
			if(bShowInside == true) vecT *= -1.0f;
			pVxCorners[nn].vecNorm = vecT;
			pVxCorners[nn+1].vecNorm = vecT;
		}
		pVxCorners[nNumberNodes-2] = pVxCorners[0];
		pVxCorners[nNumberNodes-1] = pVxCorners[1];

		m_pPrim[0].FillTriangleStrip(m_pd3dDevice, pVxCorners,nNumberTriangles,m_nCullMode,m_nZMode);
	}

	delete pVxCorners;
}

void C3DPart::Rod(LPDIRECT3DDEVICE9 pd3dDevice, float fDiameter, float fHeight, D3DXVECTOR3 vecNormal, D3DXCOLOR rgb,D3DXVECTOR3 *vecTranslate)
{
	int nNumberTriangles = 100;
	int	nNumberNodes = nNumberTriangles + 2;
	D3DXMATRIXA16 matWorld,matNormal,matTranslation;
	float fAngle;
	D3DXVECTOR3 vecA,vecB,vecT;
	int nNode, nn;

	m_pd3dDevice = pd3dDevice;
	m_pPrim = new C3DPrimitive[m_nPrimL = 3];

	CUSTOMVERTEX *pVxCorners = new CUSTOMVERTEX[nNumberTriangles+2];
	ZeroMemory(pVxCorners, sizeof CUSTOMVERTEX * (nNumberTriangles+2));

	//Cylinder part
	if(m_pPrim != NULL) {
		float fYaw = atan2f(vecNormal.x, vecNormal.z);
		float fPitch = atan2f(-vecNormal.y , sqrtf(vecNormal.x * vecNormal.x + vecNormal.z * vecNormal.z));
		D3DXMatrixRotationYawPitchRoll( &matNormal, fYaw, fPitch, 0 );

		if(vecTranslate) {
			D3DXMatrixTranslation(&matTranslation,vecTranslate->x,vecTranslate->y,vecTranslate->z);
			D3DXMatrixMultiply(&matNormal,&matNormal,&matTranslation);
		}

		for(nNode=0,nn=0;nNode<50;nNode++,nn++) {
			fAngle = (float)nNode * (2.0f*PIf) / 49.0f;
			D3DXMatrixRotationZ(&matWorld,-fAngle);
			D3DXMatrixMultiply(&matWorld,&matWorld,&matNormal);

			pVxCorners[nn].vecPos.x = fDiameter / 2.0f;
			pVxCorners[nn].vecPos.y = 0.0f;
			pVxCorners[nn].vecPos.z = fHeight;
			D3DXVec3TransformCoord(&pVxCorners[nn].vecPos,&pVxCorners[nn].vecPos,&matWorld);
			pVxCorners[nn].dwColor = rgb;

			nn++;
			pVxCorners[nn].vecPos.x = fDiameter / 2.0f;
			pVxCorners[nn].vecPos.y = 0.0f;
			pVxCorners[nn].vecPos.z = 0.0f;
			D3DXVec3TransformCoord(&pVxCorners[nn].vecPos,&pVxCorners[nn].vecPos,&matWorld);
			pVxCorners[nn].dwColor = rgb;
		}


		nNumberTriangles = nn-2;
		nNumberNodes = nn;

		for(nn=0;nn<(nNumberNodes+2);nn+=2) {
			vecA = pVxCorners[nn+2].vecPos - pVxCorners[nn+1].vecPos;
			vecB = pVxCorners[nn+0].vecPos - pVxCorners[nn+1].vecPos;
			D3DXVec3Cross(&vecT, &vecB, &vecA);
			D3DXVec3Normalize(&vecT,&vecT);
			pVxCorners[nn].vecNorm = vecT;
			pVxCorners[nn+1].vecNorm = vecT;
		}
		pVxCorners[nNumberNodes-2] = pVxCorners[0];
		pVxCorners[nNumberNodes-1] = pVxCorners[1];

		m_pPrim[0].FillTriangleStrip(m_pd3dDevice, pVxCorners,nNumberTriangles,m_nCullMode,m_nZMode);

		//Top disc
		for(fAngle = 0.0f,nn=0;fAngle <= 2.0f*PIf ; fAngle += (2.0f*PIf/(float)nNumberTriangles), nn++) {
			D3DXMatrixRotationZ(&matWorld,-fAngle);
			D3DXMatrixMultiply(&matWorld,&matWorld,&matNormal);

			pVxCorners[nn].vecPos.x = fDiameter / 2.0f;
			pVxCorners[nn].vecPos.y = 0;
			pVxCorners[nn].vecPos.z = fHeight;

			D3DXVec3TransformCoord(&pVxCorners[nn].vecPos,&pVxCorners[nn].vecPos,&matWorld);

			pVxCorners[nn].dwColor = rgb;
		}

		pVxCorners[nn++] = pVxCorners[0];

		vecA = pVxCorners[1].vecPos - pVxCorners[0].vecPos;
		vecB = pVxCorners[2].vecPos - pVxCorners[0].vecPos;
		D3DXVec3Cross(&vecT, &vecB, &vecA);
		D3DXVec3Normalize(&vecT,&vecT);
		for(nn=0;nn<=nNumberTriangles;nn++) {
			pVxCorners[nn].vecNorm = vecT;
		}

		m_pPrim[1].FillTriangleFan(m_pd3dDevice, pVxCorners,nn-1,m_nCullMode,m_nZMode);
	
		//Bottom disc
		for(fAngle = 0.0f,nn=0;fAngle <= 2.0f*PIf ; fAngle += (2.0f*PIf/(float)nNumberTriangles), nn++) {
			D3DXMatrixRotationZ(&matWorld,fAngle);
			D3DXMatrixMultiply(&matWorld,&matWorld,&matNormal);

			pVxCorners[nn].vecPos.x = fDiameter / 2.0f;
			pVxCorners[nn].vecPos.y = 0;
			pVxCorners[nn].vecPos.z = 0.0f;

			D3DXVec3TransformCoord(&pVxCorners[nn].vecPos,&pVxCorners[nn].vecPos,&matWorld);

			pVxCorners[nn].dwColor = rgb;
		}

		pVxCorners[nn++] = pVxCorners[0];

		vecA = pVxCorners[1].vecPos - pVxCorners[0].vecPos;
		vecB = pVxCorners[2].vecPos - pVxCorners[0].vecPos;
		D3DXVec3Cross(&vecT, &vecB, &vecA);
		D3DXVec3Normalize(&vecT,&vecT);
		for(nn=0;nn<=nNumberTriangles;nn++) {
			pVxCorners[nn].vecNorm = vecT;
		}

		m_pPrim[2].FillTriangleFan(m_pd3dDevice, pVxCorners,nn-1,m_nCullMode,m_nZMode);
	
	
	}

	delete pVxCorners;

}

void C3DPart::PolyLine(LPDIRECT3DDEVICE9 pd3dDevice, CPolyCoord *pLine, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate, D3DXVECTOR3 *vecRotate, float fOffsetAlongNormal, bool bConnectEnds)
{
	D3DXVECTOR3 vecT,vecA,vecB;
	D3DXMATRIXA16 matWorld,matTranslation,matRotation;
	int ii;

	m_pd3dDevice = pd3dDevice;
	m_pPrim = new C3DPrimitive[m_nPrimL = 1];

	int nLength = pLine->m_nCoordL;
	if(bConnectEnds == true) nLength++;
	CUSTOMVERTEX *pVertices = new CUSTOMVERTEX[nLength];
	ZeroMemory(pVertices, sizeof CUSTOMVERTEX * nLength);

	if(m_pPrim != NULL) {

		D3DXMatrixIdentity(&matWorld);
		if(vecRotate) {
			float fYaw = atan2f(-vecRotate->z, vecRotate->x);
			float fRoll = atan2f(vecRotate->y, sqrtf(vecRotate->x * vecRotate->x + vecRotate->z * vecRotate->z));
			D3DXMatrixRotationYawPitchRoll( &matWorld, fYaw, 0.0f, fRoll );
		}
		if(vecTranslate) {
			D3DXMatrixTranslation(&matTranslation,vecTranslate->x,vecTranslate->y,vecTranslate->z);
			D3DXMatrixMultiply(&matWorld,&matWorld,&matTranslation);
		}

		for(ii=0;ii<pLine->m_nCoordL;ii++) {
			pVertices[ii].vecPos = pLine->m_pCp[ii].Side[PORTSIDE].pt;
			if(fOffsetAlongNormal) {
				pVertices[ii].vecPos.x += (fOffsetAlongNormal * pLine->m_pCp[ii].Side[PORTSIDE].norm.x);
				pVertices[ii].vecPos.y += (fOffsetAlongNormal * pLine->m_pCp[ii].Side[PORTSIDE].norm.y);
				pVertices[ii].vecPos.z += (fOffsetAlongNormal * pLine->m_pCp[ii].Side[PORTSIDE].norm.z);
			}
			pVertices[ii].vecNorm = (D3DXVECTOR3)pLine->m_pCp[ii].Side[PORTSIDE].norm;
			pVertices[ii].dwColor = rgb;
		}
		if(bConnectEnds == true) {
			pVertices[ii] = pVertices[0];
			ii++;
		}
		m_pPrim[0].LineStrip(m_pd3dDevice, pVertices,nLength,m_nCullMode,m_nZMode);


	}
	delete pVertices;

}

void C3DPart::FillTwoPolyLines(LPDIRECT3DDEVICE9 pd3dDevice, CPolyCoord *pLine0 , CPolyCoord *pLine1, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate, D3DXVECTOR3 *vecRotate)
{
	D3DXVECTOR3 vecT,vecA,vecB;
	D3DXMATRIXA16 matWorld,matTranslation,matRotation;

	m_pd3dDevice = pd3dDevice;
	m_pPrim = new C3DPrimitive[m_nPrimL = 1];

	CUSTOMVERTEX *pVertices = new CUSTOMVERTEX[pLine0->m_nCoordL * 2];
	ZeroMemory(pVertices, sizeof CUSTOMVERTEX * pLine0->m_nCoordL * 2);

	if(m_pPrim != NULL) {

		D3DXMatrixIdentity(&matWorld);
		if(vecRotate) {
			float fYaw = atan2f(-vecRotate->z, vecRotate->x);
			float fRoll = atan2f(vecRotate->y, sqrtf(vecRotate->x * vecRotate->x + vecRotate->z * vecRotate->z));
			D3DXMatrixRotationYawPitchRoll( &matWorld, fYaw, 0.0f, fRoll );
		}
		if(vecTranslate) {
			D3DXMatrixTranslation(&matTranslation,vecTranslate->x,vecTranslate->y,vecTranslate->z);
			D3DXMatrixMultiply(&matWorld,&matWorld,&matTranslation);
		}

		int ii,nSrc0,nSrc1;
		for(ii=nSrc0=nSrc1=0;nSrc0<pLine0->m_nCoordL;nSrc0++,ii++) {
			nSrc1 = MulDiv(nSrc0,pLine1->m_nCoordL-1,pLine0->m_nCoordL-1);

			pVertices[ii].vecPos = pLine0->m_pCp[nSrc0].Side[PORTSIDE].pt;
			pVertices[ii].vecNorm = (D3DXVECTOR3)pLine0->m_pCp[nSrc0].Side[PORTSIDE].norm;
			pVertices[ii].dwColor = rgb;
			ii++;
			pVertices[ii].vecPos = pLine1->m_pCp[nSrc1].Side[PORTSIDE].pt;
			pVertices[ii].vecNorm = (D3DXVECTOR3)pLine1->m_pCp[nSrc1].Side[PORTSIDE].norm;
			pVertices[ii].dwColor = rgb;
		}
		m_pPrim[0].FillTriangleStrip(m_pd3dDevice,pVertices,ii-2,m_nCullMode,m_nZMode);


	}
	delete pVertices;

}

void C3DPart::BuildCScanLineUsingProfileScanLines(LPDIRECT3DDEVICE9 pd3dDevice, int nLine, CData *pData, int nImageNumber, int nImageType, D3DXVECTOR3 *vecTranslate, D3DXVECTOR3 *vecRotate)
{
	D3DXVECTOR3 vecT,vecA,vecB;
	D3DXMATRIXA16 matWorld,matTranslation,matRotation;
	CPoint pt[2];
	CCoord Cp0,Cp1;
	float fPos;
	int nSample,ii,nAmp;
	COLORREF* pColor = theApp.m_Palette.m_rgbAmplitude[theApp.m_n3DPaletteNumber];
	bool bGenerateVirtualXY = false;

	m_pd3dDevice = pd3dDevice;
	m_pPrim = new C3DPrimitive[m_nPrimL = 1];

	CUSTOMVERTEX *pVertices = new CUSTOMVERTEX[pData->m_nSamplesLine * 2];
	ZeroMemory(pVertices, sizeof CUSTOMVERTEX * pData->m_nSamplesLine * 2);

	if(m_pPrim != NULL) {

		D3DXMatrixIdentity(&matWorld);
		if(vecRotate) {
			float fYaw = atan2f(-vecRotate->z, vecRotate->x);
			float fRoll = atan2f(vecRotate->y, sqrtf(vecRotate->x * vecRotate->x + vecRotate->z * vecRotate->z));
			D3DXMatrixRotationYawPitchRoll( &matWorld, fYaw, 0.0f, fRoll );
		}
		if(vecTranslate) {
			D3DXMatrixTranslation(&matTranslation,vecTranslate->x,vecTranslate->y,vecTranslate->z);
			D3DXMatrixMultiply(&matWorld,&matWorld,&matTranslation);
		}

		int nRes = theApp.m_LastSettings.nCScanOverlayResolution + 1;

		pt[0].y = nLine * pData->m_nLineIncrement;
		pt[0].y += pData->m_nScanStartLine;
		pt[1].y = pt[0].y + (pData->m_nLineIncrement * nRes);
		for(ii=nSample=0;nSample<(pData->m_nSamplesLine-1);nSample+=nRes) {
			if(pData->m_nFastAxis == 5) {
				fPos = ((float)nSample * pData->m_fFastScanSize) / (float)(pData->m_nSamplesLine-1);
				bGenerateVirtualXY = true;
			} else {
				fPos = ((float)nSample * pData->m_fFastScanSize) / (float)(pData->m_nSamplesLine-1) + pData->m_CpStart.fPos[pData->m_nFastAxis];
			}
			pt[1].x=pt[0].x = nSample;
			if(PROFILE->GetSurfaceCoordinate(Cp0,pt[0],fPos,pData->m_nFastAxis,bGenerateVirtualXY) == true) {
				if(PROFILE->GetSurfaceCoordinate(Cp1,pt[1],fPos,pData->m_nFastAxis,bGenerateVirtualXY) == true) {

					nAmp = pData->GetFilteredAmp(nSample, nLine, nImageNumber);
					D3DXCOLOR rgb;
					rgb.r = (float)(pColor[nAmp]&0xff)/255.0f;
					rgb.g = (float)((pColor[nAmp]>>8)&0xff)/255.0f;
					rgb.b = (float)((pColor[nAmp]>>16)&0xff)/255.0f;

					pVertices[ii].vecPos = Cp0.Side[PORTSIDE].pt;
					pVertices[ii].vecNorm = (D3DXVECTOR3)Cp0.Side[PORTSIDE].norm;
					pVertices[ii].dwColor = rgb;
					ii++;
					
					pVertices[ii].vecPos = Cp1.Side[PORTSIDE].pt;
					pVertices[ii].vecNorm = (D3DXVECTOR3)Cp1.Side[PORTSIDE].norm;
					pVertices[ii].dwColor = rgb;
					ii++;
				}
			}
		}
		m_pPrim[0].FillTriangleStrip(m_pd3dDevice,pVertices,ii-2,m_nCullMode,m_nZMode);


	}
	delete pVertices;
}

void C3DPart::BuildCScanLineUsingDataVertices(LPDIRECT3DDEVICE9 pd3dDevice, int nLine, CData *pData, int nImageNumber, int nImageType, D3DXVECTOR3 *vecTranslate, D3DXVECTOR3 *vecRotate)
{
	D3DXVECTOR3 vecT,vecA,vecB,vW;
	D3DXMATRIXA16 matWorld,matTranslation,matRotation;
	CPoint pt[2];
	CCoord Cp0,Cp1;
	int nSample0,nSample1,ii,nAmp,kLo,kHi,k;
	COLORREF* pColor = theApp.m_Palette.m_rgbAmplitude[theApp.m_n3DPaletteNumber];
	COLORREF* pThickColor = theApp.m_Palette.m_rgbThickness[0];
	D3DXCOLOR rgb0,rgb1;
	float fLength,fLengthHi,fLengthLo;

	float fRed[256],fGreen[256],fBlue[256];
	for(nAmp=0;nAmp<256;nAmp++) {
		fRed[nAmp]   = (float)(pColor[nAmp]&0xff)/255.0f;
		fGreen[nAmp] = (float)((pColor[nAmp]>>8)&0xff)/255.0f;
		fBlue[nAmp]  = (float)((pColor[nAmp]>>16)&0xff)/255.0f;
	}

	m_pd3dDevice = pd3dDevice;
	m_pPrim = new C3DPrimitive[m_nPrimL = 1];

	CUSTOMVERTEX *pVertices = new CUSTOMVERTEX[pData->m_nSamplesLine * 2];
	ZeroMemory(pVertices, sizeof CUSTOMVERTEX * pData->m_nSamplesLine * 2);

	if(m_pPrim != NULL) {

		D3DXMatrixIdentity(&matWorld);
		if(vecRotate) {
			float fYaw = atan2f(-vecRotate->z, vecRotate->x);
			float fRoll = atan2f(vecRotate->y, sqrtf(vecRotate->x * vecRotate->x + vecRotate->z * vecRotate->z));
			D3DXMatrixRotationYawPitchRoll( &matWorld, fYaw, 0.0f, fRoll );
		}
		if(vecTranslate) {
			D3DXMatrixTranslation(&matTranslation,vecTranslate->x,vecTranslate->y,vecTranslate->z);
			D3DXMatrixMultiply(&matWorld,&matWorld,&matTranslation);
		}

		int nRes = theApp.m_LastSettings.nCScanOverlayResolution + 1;

		pt[0].y = nLine * pData->m_nLineIncrement;
		pt[0].y += pData->m_nScanStartLine;
		pt[1].y = pt[0].y + (pData->m_nLineIncrement * nRes);

		int nSrc0,nSrc1,nPixel;
		for(nPixel=ii=nSrc0=0;nSrc0<(pData->m_vLineSample[nLine].m_nCoordL-2);nSrc0++,nPixel++) {
			nSrc1 = MulDiv(nSrc0,pData->m_vLineSample[nLine+1].m_nCoordL-1,pData->m_vLineSample[nLine].m_nCoordL-1);

			fLength = 1000000000.0f;
			kLo=0;
			kHi=pData->m_vLineSample[nLine+1].m_nCoordL-1;
			fLengthLo = D3DXVec3Length(&(pData->m_vLineSample[nLine+1].m_pVp[kLo].pt - pData->m_vLineSample[nLine].m_pVp[nSrc0].pt));
			fLengthHi = D3DXVec3Length(&(pData->m_vLineSample[nLine+1].m_pVp[kHi].pt - pData->m_vLineSample[nLine].m_pVp[nSrc0].pt));
			while((kHi-kLo)>1) {
				k=(kHi+kLo)/2;
				if(fLengthHi > fLengthLo) {
					if((kHi-k) > 1) {
						kHi = (kHi+k)/2;
					} else {
						kHi=k;
					}
					fLengthHi = D3DXVec3Length(&(pData->m_vLineSample[nLine+1].m_pVp[kHi].pt - pData->m_vLineSample[nLine].m_pVp[nSrc0].pt));
				} else {
					if((k-kLo) > 1) {
						kLo = (k+kLo)/2;
					} else {
						kLo=k;
					}
					fLengthLo = D3DXVec3Length(&(pData->m_vLineSample[nLine+1].m_pVp[kLo].pt - pData->m_vLineSample[nLine].m_pVp[nSrc0].pt));
				}
				if(fLength > fLengthHi) fLength = fLengthHi;
				if(fLength > fLengthLo) fLength = fLengthLo;
			}
			if(fLengthLo < fLengthHi) {
				nSrc1 = kLo;
			} else {
				nSrc1 = kHi;
			}
			MinMax(&nSrc1,0,pData->m_vLineSample[nLine+1].m_nCoordL-1);
				
			nSample0 = nSrc0 + pData->m_vLineSample[nLine].m_nFirstSample;
			nSample1 = nSrc1 + pData->m_vLineSample[nLine+1].m_nFirstSample;

			switch(nImageType) {
			default:
			case 0:
				nAmp = pData->GetFilteredAmp(nSample0, nLine, nImageNumber);
				nAmp = MinMax(&nAmp,0,255);
				rgb0.r = fRed[nAmp];
				rgb0.g = fGreen[nAmp];
				rgb0.b = fBlue[nAmp];
				nAmp = pData->GetFilteredAmp(nSample1, nLine+1, nImageNumber);
				nAmp = MinMax(&nAmp,0,255);
				rgb1.r = fRed[nAmp];
				rgb1.g = fGreen[nAmp];
				rgb1.b = fBlue[nAmp];
			break;
			case 1:
				nAmp = pData->GetFilteredTimeThickness(nSample0, nLine, nImageNumber,1);
				rgb0.r = (float)(pThickColor[nAmp]&0xff)/255.0f;
				rgb0.g = (float)((pThickColor[nAmp]>>8)&0xff)/255.0f;
				rgb0.b = (float)((pThickColor[nAmp]>>16)&0xff)/255.0f;
				nAmp = pData->GetFilteredTimeThickness(nSample1, nLine+1, nImageNumber,1);
				rgb1.r = (float)(pThickColor[nAmp]&0xff)/255.0f;
				rgb1.g = (float)((pThickColor[nAmp]>>8)&0xff)/255.0f;
				rgb1.b = (float)((pThickColor[nAmp]>>16)&0xff)/255.0f;
			break;
			}

			if(theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame==1) {
				pData->TransformScanVertexToCADVertexWithTwist(&pVertices[ii].vecPos, &pVertices[ii].vecNorm, &pData->m_vLineSample[nLine].m_pVp[nSrc0], nLine);
			} else {
				pVertices[ii].vecPos = pData->m_vLineSample[nLine].m_pVp[nSrc0].pt;
				pVertices[ii].vecNorm = (D3DXVECTOR3)pData->m_vLineSample[nLine].m_pVp[nSrc0].norm;
			}
			pVertices[ii].dwColor = rgb0;
			ii++;
							
			if(theApp.m_LastSettings.nDisplay3DCScanOverlayCoordinateFrame==1) {
				pData->TransformScanVertexToCADVertexWithTwist(&pVertices[ii].vecPos, &pVertices[ii].vecNorm, &pData->m_vLineSample[nLine+1].m_pVp[nSrc1], nLine+1);
			} else {
				pVertices[ii].vecPos = pData->m_vLineSample[nLine+1].m_pVp[nSrc1].pt;
				pVertices[ii].vecNorm = (D3DXVECTOR3)pData->m_vLineSample[nLine+1].m_pVp[nSrc1].norm;
			}
			pVertices[ii].dwColor = rgb1;
			ii++;

		}
		m_pPrim[0].FillTriangleStrip(m_pd3dDevice,pVertices,ii-2,m_nCullMode,m_nZMode);


	}
	delete pVertices;
}

void C3DPart::PolyLine(LPDIRECT3DDEVICE9 pd3dDevice, CVertexArray *pLine, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate, D3DXVECTOR3 *vecRotate, float fOffsetAlongNormal, bool bConnectEnds)
{
	D3DXVECTOR3 vecT,vecA,vecB;
	D3DXMATRIXA16 matWorld,matTranslation,matRotation;
	int ii;

	m_pd3dDevice = pd3dDevice;
	m_pPrim = new C3DPrimitive[m_nPrimL = 1];

	int nLength = pLine->m_nCoordL;
	if(bConnectEnds == true) nLength++;
	CUSTOMVERTEX *pVertices = new CUSTOMVERTEX[nLength];
	ZeroMemory(pVertices, sizeof CUSTOMVERTEX * nLength);

	if(m_pPrim != NULL) {

		D3DXMatrixIdentity(&matWorld);
		if(vecRotate) {
			float fYaw = atan2f(-vecRotate->z, vecRotate->x);
			float fRoll = atan2f(vecRotate->y, sqrtf(vecRotate->x * vecRotate->x + vecRotate->z * vecRotate->z));
			D3DXMatrixRotationYawPitchRoll( &matWorld, fYaw, 0.0f, fRoll );
		}
		if(vecTranslate) {
			D3DXMatrixTranslation(&matTranslation,vecTranslate->x,vecTranslate->y,vecTranslate->z);
			D3DXMatrixMultiply(&matWorld,&matWorld,&matTranslation);
		}

		for(ii=0;ii<pLine->m_nCoordL;ii++) {
			pVertices[ii].vecPos = pLine->m_pVp[ii].pt;
			if(fOffsetAlongNormal) {
				pVertices[ii].vecPos.x += (fOffsetAlongNormal * pLine->m_pVp[ii].norm.x);
				pVertices[ii].vecPos.y += (fOffsetAlongNormal * pLine->m_pVp[ii].norm.y);
				pVertices[ii].vecPos.z += (fOffsetAlongNormal * pLine->m_pVp[ii].norm.z);
			}
			pVertices[ii].vecNorm = (D3DXVECTOR3)pLine->m_pVp[ii].norm;
			pVertices[ii].dwColor = rgb;
		}
		if(bConnectEnds == true) {
			pVertices[ii] = pVertices[0];
			ii++;
		}
		m_pPrim[0].LineStrip(m_pd3dDevice, pVertices,nLength,m_nCullMode,m_nZMode);


	}
	delete pVertices;

}

void C3DPart::FillTwoPolyLines(LPDIRECT3DDEVICE9 pd3dDevice, CVertexArray *pLine0 , CVertexArray *pLine1, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate, D3DXVECTOR3 *vecRotate)
{
	D3DXVECTOR3 vecT,vecA,vecB;
	D3DXMATRIXA16 matWorld,matTranslation,matRotation;

	m_pd3dDevice = pd3dDevice;
	m_pPrim = new C3DPrimitive[m_nPrimL = 1];

	CUSTOMVERTEX *pVertices = new CUSTOMVERTEX[pLine0->m_nCoordL * 2];
	ZeroMemory(pVertices, sizeof CUSTOMVERTEX * pLine0->m_nCoordL * 2);

	if(m_pPrim != NULL) {

		D3DXMatrixIdentity(&matWorld);
		if(vecRotate) {
			float fYaw = atan2f(-vecRotate->z, vecRotate->x);
			float fRoll = atan2f(vecRotate->y, sqrtf(vecRotate->x * vecRotate->x + vecRotate->z * vecRotate->z));
			D3DXMatrixRotationYawPitchRoll( &matWorld, fYaw, 0.0f, fRoll );
		}
		if(vecTranslate) {
			D3DXMatrixTranslation(&matTranslation,vecTranslate->x,vecTranslate->y,vecTranslate->z);
			D3DXMatrixMultiply(&matWorld,&matWorld,&matTranslation);
		}

		int ii,nSrc0,nSrc1;
		for(ii=nSrc0=nSrc1=0;nSrc0<pLine0->m_nCoordL;nSrc0++,ii++) {
			nSrc1 = MulDiv(nSrc0,pLine1->m_nCoordL-1,pLine0->m_nCoordL-1);

			pVertices[ii].vecPos = pLine0->m_pVp[nSrc0].pt;
			pVertices[ii].vecNorm = (D3DXVECTOR3)pLine0->m_pVp[nSrc0].norm;
			pVertices[ii].dwColor = rgb;
			ii++;
			pVertices[ii].vecPos = pLine1->m_pVp[nSrc1].pt;
			pVertices[ii].vecNorm = (D3DXVECTOR3)pLine1->m_pVp[nSrc1].norm;
			pVertices[ii].dwColor = rgb;
		}
		m_pPrim[0].FillTriangleStrip(m_pd3dDevice,pVertices,ii-2,m_nCullMode,m_nZMode);


	}
	delete pVertices;

}

