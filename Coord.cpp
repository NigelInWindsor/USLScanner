// Coord.cpp: implementation of the CCoord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "Coord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCoord::CCoord()
{

}

CCoord::~CCoord()
{

}


void CCoord::Save(CUSLFile *pFile)
{
	pFile->Write(this,sizeof COORDDATA);
}

void CCoord::Retrieve(CUSLFile *pFile)
{
	if(pFile->m_fVersion >= 4.0f && pFile->m_fVersion <= theApp.m_fVersion) {
		pFile->Read(this, sizeof COORDDATA);
	} else {
		if (pFile->m_fVersion>=3.0f) {
			int	nSize;
			int jj = sizeof nSize;
			jj *= -1;
			pFile->Read(&nSize, sizeof(int));
			if (nSize > 0 && nSize < sizeof COORDDATA) {
				pFile->Seek(jj, CFile::current);
				pFile->Read(&nStructSize, nSize);
				InvertNorm(STARBOARD);
			};
		}
	}

}





void CCoord::FormatQuaternion(CString *pBuff, int nStyle, int nOffset)
{
	D3DXMATRIXA16 matQuatRot;
	D3DXQUATERNION qNormal;
	CString strSide[2];
	int nSide;

	for(nSide = 0;nSide < 2;nSide++, nOffset += 7) {

		QuaternionFromNormal(nSide,&qNormal);

		switch(nStyle) {
		case 0:
			if(nSide==0) {
				strSide[nSide].Format(L"X %.02f Y %.02f Z %.02f A %.07f B %.07f C %.07f U %.07f",Side[nSide].fX,Side[nSide].fY,Side[nSide].fZ,qNormal.x,qNormal.y,qNormal.z,qNormal.w);
			} else {
				strSide[nSide].Format(L"XX %.02f YY %.02f ZZ %.02f AA %.07f BB %.07f CC %.07f UU %.07f",Side[nSide].fX,Side[nSide].fY,Side[nSide].fZ,qNormal.x,qNormal.y,qNormal.z,qNormal.w);
			};
			break;
		case 1:	strSide[nSide].Format(L"M%d=%.02f M%d=%.02f M%d=%.02f M%d=%.09f M%d=%.09f M%d=%.09f M%d=%.09f",nOffset,Side[nSide].fX,nOffset+1,Side[nSide].fY,nOffset+2,Side[nSide].fZ,nOffset+3,qNormal.x,nOffset+4,qNormal.y,nOffset+5,qNormal.z,nOffset+6,qNormal.w);
			break;
		}
	}

	pBuff->Format(L"%s %s",strSide[0],strSide[1]);

}

void CCoord::FormatNormal(CString *pBuff, int nStyle, int nOffset)
{
	CString strSide[2];
	int nSide;
	float fTurntable;
	CCoord Cp;

	fTurntable = Side[0].fR + (float)theApp.m_Axes[theApp.m_Tank.nRLeft].nRevCounter * 360.0f;

	switch (theApp.m_Axes[theApp.m_Tank.nRLeft].nModuloMode) {
	default: 
		break;
	case 1: //Shortest
		theApp.m_Motors.GetTurntablePos(&Cp);
		if ((Cp.Side0.fR - Side[0].fR) < -180.0f) fTurntable -= 360.0f;
		if ((Cp.Side0.fR - Side[0].fR) > 180.0f) fTurntable += 360.0f;
		break;
	case 2: //Positive
		theApp.m_Motors.GetTurntablePos(&Cp);
		if ((Side[0].fR - Cp.Side0.fR) < 0.0f) fTurntable += 360.0f;
		break;
	case 3: //Negative
		theApp.m_Motors.GetTurntablePos(&Cp);
		if ((Side[0].fR - Cp.Side0.fR) > 0.0f) fTurntable -= 360.0f;
		break;
	}

	for(nSide = 0;nSide < 2;nSide++, nOffset += 7) {

		switch(nStyle) {
		case 0:
			if(nSide==0) {
				strSide[nSide].Format(L"X %.02f Y %.02f Z %.02f A %.07f B %.07f C %.07f U %.07f",Side[nSide].fX,Side[nSide].fY,Side[nSide].fZ,Side[nSide].fI,Side[nSide].fJ,Side[nSide].fK,0.0f);
			} else {
				strSide[nSide].Format(L"XX %.02f YY %.02f ZZ %.02f AA %.07f BB %.07f CC %.07f UU %.07f",Side[nSide].fX,Side[nSide].fY,Side[nSide].fZ,Side[nSide].fI,Side[nSide].fJ,Side[nSide].fK,0.0f);
			};
			break;
		case 1:	
			if (nSide == 0) {
				strSide[nSide].Format(L"M%d=%.02f M%d=%.02f M%d=%.02f M%d=%.07f M%d=%.07f M%d=%.07f M%d=%.07f M%d=%.02f", nOffset, Side[nSide].fX, nOffset + 1, Side[nSide].fY, nOffset + 2, Side[nSide].fZ, nOffset + 3, Side[nSide].fI, nOffset + 4, Side[nSide].fJ, nOffset + 5, Side[nSide].fK, nOffset + 6, 0.0f, nOffset + 14, fTurntable);
			}
			else {
				strSide[nSide].Format(L"M%d=%.02f M%d=%.02f M%d=%.02f M%d=%.07f M%d=%.07f M%d=%.07f M%d=%.07f", nOffset, Side[nSide].fX, nOffset + 1, Side[nSide].fY, nOffset + 2, Side[nSide].fZ, nOffset + 3, Side[nSide].fI, nOffset + 4, Side[nSide].fJ, nOffset + 5, Side[nSide].fK, nOffset + 6, 0.0f);
			}
			break;
		}
	}

	pBuff->Format(L"%s %s",strSide[0],strSide[1]);

}


void CCoord::TransformCoordinate(int nSide, D3DXVECTOR3 *vTranslate, float fXRotateDegrees, float fYRotateDegrees, float fZRotateDegrees)
{
	D3DXMATRIXA16 matBase, matRotX, matRotZ;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		D3DXMatrixRotationZ(&matBase,fZRotateDegrees * DEG_TO_RAD);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&Side[nSide].norm,(D3DXVECTOR3*)&Side[nSide].norm,&matBase);
		Side[nSide].pt -= *vTranslate;
		D3DXVec3TransformCoord(&Side[nSide].pt,&Side[nSide].pt,&matBase);
		break;
	case DUAL_ROBOT_9_PLUS_9:
		D3DXMatrixRotationZ(&matBase,fZRotateDegrees * DEG_TO_RAD);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&Side[nSide].norm,(D3DXVECTOR3*)&Side[nSide].norm,&matBase);
		Side[nSide].pt -= *vTranslate;
		D3DXVec3TransformCoord(&Side[nSide].pt,&Side[nSide].pt,&matBase);
		break;
	case SPX_ROBOT:
		D3DXMatrixRotationZ(&matBase,-fZRotateDegrees * DEG_TO_RAD);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&Side[nSide].norm,(D3DXVECTOR3*)&Side[nSide].norm,&matBase);
		Side[nSide].pt -= *vTranslate;
		if(nSide == PORTSIDE) Side[nSide].pt.y *= -1.0f;
		D3DXVec3TransformCoord(&Side[nSide].pt,&Side[nSide].pt,&matBase);
		break;
	};
}

void CCoord::RotateCoordinate(int nSide, float fXRotateDegrees, float fYRotateDegrees, float fZRotateDegrees)
{
	D3DXMATRIXA16 matBase, matRotX, matRotZ;

	D3DXMatrixRotationZ(&matBase, fZRotateDegrees * DEG_TO_RAD);
	D3DXVec3TransformCoord((D3DXVECTOR3*)&Side[nSide].norm, (D3DXVECTOR3*)&Side[nSide].norm, &matBase);
	D3DXVec3TransformCoord(&Side[nSide].pt, &Side[nSide].pt, &matBase);
}
