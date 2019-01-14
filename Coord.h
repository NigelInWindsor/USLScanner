// Coord.h: interface for the CCoord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COORD_H__CCE31637_9019_4C24_BC84_ACBDE1607EF1__INCLUDED_)
#define AFX_COORD_H__CCE31637_9019_4C24_BC84_ACBDE1607EF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "math.h"
#include <d3dx9math.h>
#include <d3d9.h>

#define	DEG_TO_RAD	0.0174532925f
#define TRAFFIC_LIGHT	1

//nType
#define		TYPE_CONTINUOUS		0
#define		TYPE_SEGMENT		1
#define		TYPE_JUMP			2
#define		TYPE_LINEAR			0x00
#define		TYPE_SPLINE			0x10


class SUBCOORD {

public:
	SUBCOORD() {};
	~SUBCOORD() {};

	union
	{
		struct 
		{
			float fX,fY,fZ,fXt,fYt,fR,fI,fJ,fK,fW; // 10 real components of the vector
		};
		struct 
		{
			D3DXVECTOR3 pt;
			D3DXVECTOR3 tip;
			D3DXVECTOR4 norm;
		};
		float	fPos[10]; // Array access useful in loops
	};

	void Normalize()
	{
		D3DXVec3Normalize(&tip,&tip);
	}


};

class SUBDELTACOORD {
public:
	SUBDELTACOORD() {};
	~SUBDELTACOORD() {};

	union
	{
		struct 
		{
			float fX0,fY0,fZ0,fX1,fY1,fZ1,fI,fJ,fK,fW; // 10 real components of the vector
		};
		struct 
		{
			D3DVECTOR pt0;
			D3DVECTOR pt1;
			D3DXVECTOR4 tip;
		};
		float	fPos[10]; // Array access useful in loops
	};
	inline void Zero()
	{
		ZeroMemory(fPos, sizeof fPos);
	};

};




class COORDDATA
{ 
public:
	COORDDATA() {};
	~COORDDATA() {};

	int	nStructSize;

	union
	{
		struct
		{
			SUBCOORD Side0;
			SUBCOORD Side1;
			SUBDELTACOORD Delta;
		};
		struct
		{
			SUBCOORD Side[2];
			SUBDELTACOORD DeltaSide;
		};
		float fPos[30];
	};
	union {
		struct
		{
			int nGain[2];
			int	nDelay[6];
			int	nWidth[6];
		};
		int nElec[14];
	};
	union {
		struct {
			int		nAmp;
			int		nThickness;
			bool	bModified;
			int		nType;
			float	fWeight;
			float	fWp[2];
			float	fThickness;
			int		nTS;
		};
		struct {
			int		nVelocity;
			float	fVelocity;
			bool	bNotAssigned;
			int		nNotAssignedFirstSet[6];
		};
	};
	union {
		struct
		{
			int		nLine;
			int		nSample;
			float	fDiameter;
			int		nMoveMode[2];
		};
		struct
		{
			float	fActThickness;
			float	fNomThickness;
			int		nMaxTimens;
			int		nMinTimens;
			int		nTimens;
		};
		struct
		{
			float	fSpare_1[2];
			float	fMinThickness;
			float	fMaxThickness;
			float	fSpare_2;
		};
		struct
		{
			float	fTunningPos;
			float	fTunningFollowingError;
			float	fTunningDAC;
			int		nTunningFollowingError;
			int		nTunningDAC;
		};
		struct
		{
			int		nSpare;
			float	fRadius[2];
			float	fAngle[2];
		};
		struct
		{
			float	fLine;
			float	fSample;
			float	fLength;
			float	fSpare[2];
		};
	};

	inline void CopySides(int nDest,int nSrc) { Side[nDest] = Side[nSrc]; };

	inline void Zero()
	{ 
		memset(this, 0, sizeof COORDDATA);
		nStructSize = sizeof COORDDATA;
	};

	inline bool IsZero()
	{
		float fTemp=0.0f;
		for(int ii=0;ii<30;ii++) fTemp += fPos[ii];
		if(fTemp == 0.0f) return true;

		return false;

	}

	inline void ZeroSide(int nSide)
	{ 
		memset(&Side[nSide], 0, sizeof SUBCOORD);
	};

	inline void InvertBothNormals()
	{
		Side0.fI *= -1.0f;
		Side0.fJ *= -1.0f;
		Side0.fK *= -1.0f;
		Side1.fI *= -1.0f;
		Side1.fJ *= -1.0f;
		Side1.fK *= -1.0f;
	};

	inline void InvertNorm(int nSide)
	{
		Side[nSide].fI *= -1.0f;
		Side[nSide].fJ *= -1.0f;
		Side[nSide].fK *= -1.0f;
	};

	inline float	DistanceToSegment(COORDDATA Cp0, COORDDATA Cp1)
	{
		D3DXVECTOR3 V(Cp1.Side[0].fX-Cp0.Side[0].fX,Cp1.Side[0].fY-Cp0.Side[0].fY,Cp1.Side[0].fZ-Cp0.Side[0].fZ);
		D3DXVECTOR3 W(Side[0].fX-Cp0.Side[0].fX,Side[0].fY-Cp0.Side[0].fY,Side[0].fZ-Cp0.Side[0].fZ);

		float fC1 = D3DXVec3Dot(&W, &V);

		if(fC1<=0) {
			return D3DXVec3Length(&W);
		}

		float fC2 = D3DXVec3Dot(&V, &V);
		if(fC2<=fC1) {
			W = D3DXVECTOR3(Side[0].fX-Cp1.Side[0].fX,Side[0].fY-Cp1.Side[0].fY,Side[0].fZ-Cp1.Side[0].fZ);
			return D3DXVec3Length(&W);
		}

		float fB = fC1 / fC2;
		V = V * fB;
		D3DXVECTOR3 Pb(Cp0.Side[0].fX + V.x,Cp0.Side[0].fY + V.y,Cp0.Side[0].fZ + V.z);
		W = D3DXVECTOR3(Side[0].fX-Pb.x, Side[0].fY-Pb.y, Side[0].fZ-Pb.z);
		return D3DXVec3Length(&W);
	}

	inline float	DistanceToLine(COORDDATA Cp0, COORDDATA Cp1)
	{
		D3DXVECTOR3 V(Cp1.Side[0].fX-Cp0.Side[0].fX,Cp1.Side[0].fY-Cp0.Side[0].fY,Cp1.Side[0].fZ-Cp0.Side[0].fZ);
		D3DXVECTOR3 W(Side[0].fX-Cp0.Side[0].fX,Side[0].fY-Cp0.Side[0].fY,Side[0].fZ-Cp0.Side[0].fZ);

		float fC1 = D3DXVec3Dot(&W, &V);
		float fC2 = D3DXVec3Dot(&V, &V);
		float fB = fC1 / fC2;

		V = V * fB;
		D3DXVECTOR3 Pb(Cp0.Side[0].fX + V.x,Cp0.Side[0].fY + V.y,Cp0.Side[0].fZ + V.z);
		W = D3DXVECTOR3(Side[0].fX-Pb.x, Side[0].fY-Pb.y, Side[0].fZ-Pb.z);

		return D3DXVec3Length(&W);
	}

	void ModifyUt(COORDDATA Cp)
	{
		int ii;

		nGain[0] = Cp.nGain[0];
		nGain[1] = Cp.nGain[1];
		for(ii=0;ii<6;ii++) {
			nDelay[ii] = Cp.nDelay[ii];
			nWidth[ii] = Cp.nWidth[ii];
		}
	}

	inline void CalculatePolarAboutZ(D3DXVECTOR3 vecZero,int nSide)
	{
		D3DXVECTOR3 pt;

		pt = Side[nSide].pt;
		pt.z = 0.0f;

		fRadius[nSide] = D3DXVec3Length(&pt);
		D3DXVec3Normalize(&pt,&pt);

		Side[nSide].fR = acosf(D3DXVec3Dot(&pt, &vecZero)) * 57.29577951f;

		if((vecZero.x < 0.0f) && (vecZero.y == 0.0f)) {
			if(pt.y > 0.0f) Side[nSide].fR *= -1.0f;
		}
		if((vecZero.x == 0.0f) && (vecZero.y < 0.0f)) {
			if(pt.x < 0.0f) Side[nSide].fR *= -1.0f;
		}

	}

	inline void TransformCoord(D3DXMATRIXA16 &Matrix)
	{
		D3DXMATRIXA16 matNorm = Matrix;

		D3DXVec3TransformCoord((D3DXVECTOR3*)&Side0.pt,(D3DXVECTOR3*)&Side0.pt,&Matrix);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&Side1.pt,(D3DXVECTOR3*)&Side1.pt,&Matrix);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&Side[2].pt,(D3DXVECTOR3*)&Side[2].pt,&Matrix);
		matNorm.m[3][0]=0.0f;
		matNorm.m[3][1]=0.0f;
		matNorm.m[3][2]=0.0f;
		D3DXVec3TransformCoord((D3DXVECTOR3*)&Side0.norm,(D3DXVECTOR3*)&Side0.norm,&matNorm);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&Side1.norm,(D3DXVECTOR3*)&Side1.norm,&matNorm);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&Side[2].norm,(D3DXVECTOR3*)&Side[2].norm,&matNorm);
	}

};


class CCoord : public COORDDATA
{
public:
	void FormatNormal(CString *pBuff,int nStyle,int nOffset=0);
	void TransformCoordinate(int nSide, D3DXVECTOR3 * vTranslate, float fXRotateDegrees, float fYRotateDegrees, float fZRotateDegrees);
	void RotateCoordinate(int nSide, float fXRotateDegrees, float fYRotateDegrees, float fZRotateDegrees);
	void FormatQuaternion(CString *pBuff,int nStyle,int nOffset=0);
	void Retrieve(CUSLFile* pFile);
	void Save(CUSLFile* pFile);


	inline void QuaternionFromNormal(int nSide, D3DXQUATERNION *quat)
	{
		D3DXMATRIXA16 matNormal,matToolRot;
		D3DXVECTOR3 vUp,vDir, vSide;

		D3DXMatrixIdentity(&matNormal);
		D3DXVec3Normalize(&vDir,&(D3DXVECTOR3)Side[nSide].norm);

		vUp = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		D3DXVec3Cross(&vSide, &vUp, &vDir);
		D3DXVec3Normalize(&vSide, &vSide);
		D3DXVec3Cross(&vUp, &vSide, &vDir);
		D3DXVec3Normalize(&vUp, &vUp);
		CopyMemory(&matNormal._11, &vUp, sizeof D3DXVECTOR3);
		CopyMemory(&matNormal._21, &vSide, sizeof D3DXVECTOR3);
		CopyMemory(&matNormal._31, &vDir, sizeof D3DXVECTOR3);
		D3DXMatrixRotationZ(&matToolRot, Side[nSide].fW * DEG_TO_RAD);
		D3DXMatrixMultiply(&matNormal, &matToolRot, &matNormal);
		D3DXQuaternionRotationMatrix(quat,&matNormal);
		D3DXQuaternionNormalize(quat,quat);
	}

	inline void NormalFromQuaternion(int nSide, D3DXQUATERNION *quat)
	{
		D3DXMATRIXA16 matQuatRot;

		D3DXMatrixRotationQuaternion(&matQuatRot,quat);
		Side[nSide].norm.x = matQuatRot._31;
		Side[nSide].norm.y = matQuatRot._32;
		Side[nSide].norm.z = matQuatRot._33;
	}

	void ModifyUt(CCoord Cp)
	{
		int ii;

		nGain[0] = Cp.nGain[0];
		nGain[1] = Cp.nGain[1];
		for(ii=0;ii<6;ii++) {
			nDelay[ii] = Cp.nDelay[ii];
			nWidth[ii] = Cp.nWidth[ii];
		}
	}

	inline void LimitIJK()
	{
		MinMax(&Side0.fI, -1.0f, 1.0f);
		MinMax(&Side0.fJ, -1.0f, 1.0f);
		MinMax(&Side0.fK, -1.0f, 1.0f);
		MinMax(&Side1.fI, -1.0f, 1.0f);
		MinMax(&Side1.fJ, -1.0f, 1.0f);
		MinMax(&Side1.fK, -1.0f, 1.0f);

	}

	inline float MinMax(float *pfV, float fMin, float fMax)
	{

		if (*pfV<fMin) *pfV = fMin;
		if (*pfV>fMax) *pfV = fMax;

		return *pfV;
	}

	inline void UnityVector()
	{
		float mag = Side0.fI*Side0.fI + Side0.fJ*Side0.fJ + Side0.fK*Side0.fK;
		if(( mag > 1.0f )  && (mag < 10.0f)) {
			float scale = 1.0f/sqrtf(mag);
			Side0.fI *= scale;
			Side0.fJ *= scale;
			Side0.fK *= scale;
		}
		
		mag = Side1.fI*Side1.fI + Side1.fJ*Side1.fJ + Side1.fK*Side1.fK;
		if(( mag > 1.0f )  && (mag < 10.0f)) {
			float scale = 1.0f/sqrtf(mag);
			Side1.fI *= scale;
			Side1.fJ *= scale;
			Side1.fK *= scale;
		}
	}

	inline void InvertBothNormals()
	{
		Side0.fI *= -1.0f;
		Side0.fJ *= -1.0f;
		Side0.fK *= -1.0f;
		Side1.fI *= -1.0f;
		Side1.fJ *= -1.0f;
		Side1.fK *= -1.0f;
	};

	inline void InvertNorm(int nSide)
	{
		Side[nSide].fI *= -1.0f;
		Side[nSide].fJ *= -1.0f;
		Side[nSide].fK *= -1.0f;
	};



	inline void SetSide0(CCoord Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<10;nLoop++) {
			Side0.fPos[nLoop] = Cp.Side0.fPos[nLoop];
		}
	};

	inline void SetSide1(CCoord Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<10;nLoop++) {
			Side1.fPos[nLoop] = Cp.Side1.fPos[nLoop];
		}
	};

	inline void SetSide(CCoord Cp,int nSide)
	{
		int nLoop;
		switch(nSide) {
		case 0:
			for(nLoop=0;nLoop<10;nLoop++) {
				Side0.fPos[nLoop] = Cp.Side0.fPos[nLoop];
			}
			break;
		case 1:
			for(nLoop=0;nLoop<10;nLoop++) {
				Side1.fPos[nLoop] = Cp.Side1.fPos[nLoop];
			}
			break;
		}
	};


	inline static float Length(CCoord Cp0, CCoord Cp1, int nSide = 0)
	{
		float fX = Cp0.Side[nSide].fX - Cp1.Side[nSide].fX;
		float fY = Cp0.Side[nSide].fY - Cp1.Side[nSide].fY;
		float fZ = Cp0.Side[nSide].fZ - Cp1.Side[nSide].fZ;

		return (float)sqrt((double)(fX*fX + fY*fY + fZ*fZ));
	};

	inline float Length(CCoord Cp, int nSide = 0)
	{
		float fX = Side[nSide].fX - Cp.Side[nSide].fX;
		float fY = Side[nSide].fY - Cp.Side[nSide].fY;
		float fZ = Side[nSide].fZ - Cp.Side[nSide].fZ;

		return (float)sqrt((double)(fX*fX + fY*fY + fZ*fZ));
	};

	inline float Length(COORDDATA Cp, int nSide = 0)
	{
		float fX = Side[nSide].fX - Cp.Side[nSide].fX;
		float fY = Side[nSide].fY - Cp.Side[nSide].fY;
		float fZ = Side[nSide].fZ - Cp.Side[nSide].fZ;

		return (float)sqrt((double)(fX*fX + fY*fY + fZ*fZ));
	};

	inline static float Length(COORDDATA Cp0, COORDDATA Cp1, int nSide = 0)
	{
		float fX = Cp0.Side[nSide].fX - Cp1.Side[nSide].fX;
		float fY = Cp0.Side[nSide].fY - Cp1.Side[nSide].fY;
		float fZ = Cp0.Side[nSide].fZ - Cp1.Side[nSide].fZ;

		return (float)sqrt((double)(fX*fX + fY*fY + fZ*fZ));
	};

	inline void CopySides(int nDest,int nSrc) { Side[nDest] = Side[nSrc]; };
	
	inline void CopySide(int nSide,CCoord Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<10;nLoop++) {	Side[nSide].fPos[nLoop] = Cp.Side[nSide].fPos[nLoop]; };
	}

	inline CCoord operator + (CCoord Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<20;nLoop++) {	fPos[nLoop] += Cp.fPos[nLoop]; };
		for(nLoop=0;nLoop<14;nLoop++) {	nElec[nLoop] += Cp.nElec[nLoop]; };
		return (*this);
	};

	inline CCoord operator - (CCoord Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<20;nLoop++) {	fPos[nLoop] -= Cp.fPos[nLoop]; };
		for(nLoop=0;nLoop<14;nLoop++) {	nElec[nLoop] -= Cp.nElec[nLoop]; };
		return (*this);
	};

	inline void operator -=(CCoord Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<20;nLoop++) {	fPos[nLoop] -= Cp.fPos[nLoop]; };
		for(nLoop=0;nLoop<14;nLoop++) {	nElec[nLoop] -= Cp.nElec[nLoop]; };
	};

	inline void operator +=(CCoord Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<20;nLoop++) {	fPos[nLoop] += Cp.fPos[nLoop]; };
		for(nLoop=0;nLoop<14;nLoop++) {	nElec[nLoop] += Cp.nElec[nLoop]; };
	};

	inline void operator -=(COORDDATA Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<20;nLoop++) {	fPos[nLoop] -= Cp.fPos[nLoop]; }
		for(nLoop=0;nLoop<14;nLoop++) { nElec[nLoop] -= Cp.nElec[nLoop]; };
	}

	inline void operator +=(COORDDATA Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<20;nLoop++) { fPos[nLoop] += Cp.fPos[nLoop]; }
		for(nLoop=0;nLoop<14;nLoop++) { nElec[nLoop] += Cp.nElec[nLoop]; }
	};

	inline bool operator !=(CCoord Cp)
	{
		int nLoop;

		for(nLoop=0;nLoop!=20;nLoop++) {
			if(fPos[nLoop] - Cp.fPos[nLoop]) return true;
		}
		return false;
	}

	inline void operator =(CCoord Cp)	{ CopyMemory(this, &Cp, sizeof COORDDATA); };
	inline void operator =(COORDDATA Cp) {
		nStructSize = Cp.nStructSize;
		CopyMemory(&Side0, &Cp.Side0, sizeof Cp.Side0);
		CopyMemory(&Side1, &Cp.Side1, sizeof Cp.Side1);
		CopyMemory(&Delta, &Cp.Delta, sizeof Cp.Delta);
		CopyMemory(nElec, Cp.nElec, sizeof Cp.nElec);
		nVelocity = Cp.nVelocity;
		fVelocity = Cp.fVelocity;
		bNotAssigned = Cp.bNotAssigned;
		CopyMemory(nNotAssignedFirstSet, Cp.nNotAssignedFirstSet, sizeof Cp.nNotAssignedFirstSet);
		nLine = Cp.nLine;
		nSample = Cp.nSample;
		fDiameter = Cp.fDiameter;
		CopyMemory(nMoveMode, Cp.nMoveMode, sizeof Cp.nMoveMode);
	}
//	inline operator LPCCOORD() const { return this; }

	inline void Zero()
	{ 
		memset(&nStructSize, 0, sizeof COORDDATA);
		nStructSize = sizeof COORDDATA;
	};

	inline void Zero(int nSide)
	{ 
		ZeroMemory(&Side[nSide], sizeof Side0);
	};

//	inline CCoord(float fX,float fY,float fZ,float fI,float fJ,float fK,float fW,int nSide) {
//		Side[nSide].fX = fX; Side[nSide].fY = fY; Side[nSide].fZ = fZ;
//		Side[nSide].fI = fI; Side[nSide].fJ = fJ; Side[nSide].fK = fK; Side[nSide].fW = fW;
//	};

//	inline CCoord(SUBCOORD Cp,int nSide)	{ Side[nSide] = Cp; };
//	inline CCoord(const COORDDATA& Cp) { CopyMemory(&nStructSize, &Cp, sizeof tagCOORDDATA); }; //Checked ok
	CCoord();
	virtual ~CCoord();

private:
};


#endif // !defined(AFX_COORD_H__CCE31637_9019_4C24_BC84_ACBDE1607EF1__INCLUDED_)
