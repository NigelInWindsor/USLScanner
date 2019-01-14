// 3DObject.cpp: implementation of the C3DObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "3DObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DObject::C3DObject(LPDIRECT3DDEVICE9 pd3dDevice,int nHowManyParts)
{
	m_nCurrentPart = 0;
	m_pParts = NULL;
	m_pd3dDevice = pd3dDevice;
	m_nCullMode = D3DCULL_CCW;
	m_nZMode = true;

	if((m_nPartsL = nHowManyParts) > 0) m_pParts = new C3DPart[m_nPartsL];

}

C3DObject::~C3DObject()
{
	SAFE_DELETE_ARRAY(m_pParts);
}

bool C3DObject::InvalidateDeviceObjects()
{
	for(int nn=0;nn<m_nCurrentPart;nn++) {
		m_pParts[nn].InvalidateDeviceObjects();
	}

	return true;
}

bool C3DObject::DeleteDeviceObjects()
{
	for(int nn=0;nn<m_nCurrentPart;nn++) {
		m_pParts[nn].DeleteDeviceObjects();
	}

	return true;
}

bool C3DObject::Render()
{

	for(int nn=0;nn<m_nCurrentPart;nn++) {
		m_pParts[nn].Render();
	}

	return true;
}

//Must be set before any drawing primitives
int C3DObject::SetCullMode(int nMode)
{
	int nOldMode = m_nCullMode;
	m_nCullMode = nMode;

	return nOldMode;
}

//Must be set before any drawing primitives
int C3DObject::SetZMode(int nMode)
{
	int nOldMode = m_nZMode;
	m_nZMode = nMode;

	return nOldMode;

}


bool C3DObject::Rectangle(D3DXVECTOR3 vecSize, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate, D3DXVECTOR3 *vecRotate, bool bShowOppositeFace)
{

	if((m_pParts != NULL) && (m_nCurrentPart < m_nPartsL)) {
		m_pParts[m_nCurrentPart].m_nCullMode = m_nCullMode;
		m_pParts[m_nCurrentPart].m_nZMode = m_nZMode;
		m_pParts[m_nCurrentPart++].Rectangle(m_pd3dDevice, vecSize, rgb ,vecTranslate, vecRotate, bShowOppositeFace);
		return false;
	}

	return false;
}

bool C3DObject::Cuboid(D3DXVECTOR3 vecSize, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate, D3DXVECTOR3 *vecRotate)
{

	if((m_pParts != NULL) && (m_nCurrentPart < m_nPartsL)) {
		m_pParts[m_nCurrentPart].m_nCullMode = m_nCullMode;
		m_pParts[m_nCurrentPart].m_nZMode = m_nZMode;
		m_pParts[m_nCurrentPart++].Cuboid(m_pd3dDevice, vecSize, rgb ,vecTranslate, vecRotate);
		return true;
	}

	return false;
}

bool C3DObject::Disc(float fDiameter, D3DXVECTOR3 vecNormal, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate)
{
	if((m_pParts != NULL) && (m_nCurrentPart < m_nPartsL)) {
		m_pParts[m_nCurrentPart].m_nCullMode = m_nCullMode;
		m_pParts[m_nCurrentPart].m_nZMode = m_nZMode;
		m_pParts[m_nCurrentPart++].Disc(m_pd3dDevice, fDiameter, vecNormal, rgb ,vecTranslate);
	}

	return true;
}

bool C3DObject::Cylinder(float fDiameter, float fHeight, D3DXVECTOR3 vecNormal, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate, bool bShowInside)
{
	if((m_pParts != NULL) && (m_nCurrentPart < m_nPartsL)) {
		m_pParts[m_nCurrentPart].m_nCullMode = m_nCullMode;
		m_pParts[m_nCurrentPart].m_nZMode = m_nZMode;
		m_pParts[m_nCurrentPart++].Cylinder(m_pd3dDevice, fDiameter, fHeight, vecNormal, rgb ,vecTranslate, bShowInside);
	}

	return true;
}

bool C3DObject::Rod(float fDiameter, float fHeight, D3DXVECTOR3 vecNormal, D3DXCOLOR rgb, D3DXVECTOR3 *vecTranslate)
{
	if((m_pParts != NULL) && (m_nCurrentPart < m_nPartsL)) {
		m_pParts[m_nCurrentPart].m_nCullMode = m_nCullMode;
		m_pParts[m_nCurrentPart].m_nZMode = m_nZMode;
		m_pParts[m_nCurrentPart++].Rod(m_pd3dDevice, fDiameter, fHeight, vecNormal, rgb ,vecTranslate);
	}

	return true;
}


bool C3DObject::PolyLine(CPolyCoord *pLine, D3DXCOLOR rgb,float fOffsetAlongNormal, bool bConnectEnds)
{
	if((m_pParts != NULL) && (m_nCurrentPart < m_nPartsL)) {
		m_pParts[m_nCurrentPart].m_nCullMode = m_nCullMode;
		m_pParts[m_nCurrentPart].m_nZMode = m_nZMode;
		m_pParts[m_nCurrentPart++].PolyLine(m_pd3dDevice, pLine, rgb,NULL,NULL,fOffsetAlongNormal, bConnectEnds);
		return true;
	}

	return false;
}

bool C3DObject::FillTwoPolyLines(CPolyCoord *pLine0, CPolyCoord *pLine1, D3DXCOLOR rgb)
{
	if((m_pParts != NULL) && (m_nCurrentPart < m_nPartsL)) {
		m_pParts[m_nCurrentPart].m_nCullMode = m_nCullMode;
		m_pParts[m_nCurrentPart].m_nZMode = m_nZMode;
		m_pParts[m_nCurrentPart++].FillTwoPolyLines(m_pd3dDevice, pLine0, pLine1, rgb);
		return true;
	}

	return false;
}

bool C3DObject::BuildCScanLine(int nLine, CData *pData, int nImageNumber, int nImageType)
{
	if((m_pParts != NULL) && (m_nCurrentPart < m_nPartsL)) {
		m_pParts[m_nCurrentPart].m_nCullMode = m_nCullMode;
		m_pParts[m_nCurrentPart].m_nZMode = m_nZMode;
		if(pData->m_vLineSample == NULL) {
			m_pParts[m_nCurrentPart++].BuildCScanLineUsingProfileScanLines(m_pd3dDevice, nLine, pData, nImageNumber, nImageType);
		} else {
			m_pParts[m_nCurrentPart++].BuildCScanLineUsingDataVertices(m_pd3dDevice, nLine, pData, nImageNumber, nImageType);
		}
		return true;
	}

	return false;

}


bool C3DObject::PolyLine(CVertexArray *pLine, D3DXCOLOR rgb,float fOffsetAlongNormal, bool bConnectEnds)
{
	if((m_pParts != NULL) && (m_nCurrentPart < m_nPartsL)) {
		m_pParts[m_nCurrentPart].m_nCullMode = m_nCullMode;
		m_pParts[m_nCurrentPart].m_nZMode = m_nZMode;
		m_pParts[m_nCurrentPart++].PolyLine(m_pd3dDevice, pLine, rgb,NULL,NULL,fOffsetAlongNormal, bConnectEnds);
		return true;
	}

	return false;
}

bool C3DObject::FillTwoPolyLines(CVertexArray *pLine0, CVertexArray *pLine1, D3DXCOLOR rgb)
{
	if((m_pParts != NULL) && (m_nCurrentPart < m_nPartsL)) {
		m_pParts[m_nCurrentPart].m_nCullMode = m_nCullMode;
		m_pParts[m_nCurrentPart].m_nZMode = m_nZMode;
		m_pParts[m_nCurrentPart++].FillTwoPolyLines(m_pd3dDevice, pLine0, pLine1, rgb);
		return true;
	}

	return false;
}
