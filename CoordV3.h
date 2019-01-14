// CoordV3.h: interface for the CCoordV3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COORDV3_H__6E86A25D_0EEA_46D6_A92C_85ABED6AA25E__INCLUDED_)
#define AFX_COORDV3_H__6E86A25D_0EEA_46D6_A92C_85ABED6AA25E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "math.h"
#include <d3dx9math.h>
#include <d3d9.h>



typedef struct tagSUBCOORDV3 {

	union
	{
		struct 
		{
			float fX,fY,fZ,fXt,fYt,fR,fI,fJ,fK; // 9 real components of the vector
		};
		struct 
		{
			D3DVECTOR pt;
			D3DVECTOR tip;
			D3DVECTOR norm;
		};
		float	fPos[9]; // Array access useful in loops
	};

} SUBCOORDV3, *PSUBCOORDV3;

typedef struct tagSUBDELTACOORDV3 {

	union
	{
		struct 
		{
			float fX0,fY0,fZ0,fI,fJ,fK,fX1,fY1,fZ1; // 9 real components of the vector
		};
		struct 
		{
			D3DVECTOR pt0;
			D3DVECTOR tip;
			D3DVECTOR pt1;
		};
		float	fPos[9]; // Array access useful in loops
	};

} SUBDELTACOORDV3, *PSUBDELTACOORDV3;




typedef struct tagCOORDDATAV3
{ 
	int	nStructSize;

	union
	{
		struct
		{
			SUBCOORDV3 Side0;
			SUBCOORDV3 Side1;
			SUBDELTACOORDV3 Delta;
		};
		SUBCOORDV3 Side[3];
		float fPos[27];
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
	int		nAmp;
	int		nThickness;
	bool	bModified;
	int		nType;
	float	fWeight;
	float	fWp[2];
	int		nSpare[7];

	inline void CopySides(int nDest,int nSrc) { Side[nDest] = Side[nSrc]; };

	inline void Zero()
	{ 
		memset(this, 0, sizeof COORDDATAV3);
		nStructSize = sizeof COORDDATAV3;
	};

	inline void ZeroSide(int nSide)
	{ 
		memset(&Side[nSide], 0, sizeof SUBCOORDV3);
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

	inline float	DistanceToSegment(tagCOORDDATAV3 Cp0, tagCOORDDATAV3 Cp1)
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

	inline float	DistanceToLine(tagCOORDDATAV3 Cp0, tagCOORDDATAV3 Cp1)
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

	void ModifyUt(tagCOORDDATAV3 Cp)
	{
		int ii;

		nGain[0] = Cp.nGain[0];
		nGain[1] = Cp.nGain[1];
		for(ii=0;ii<6;ii++) {
			nDelay[ii] = Cp.nDelay[ii];
			nWidth[ii] = Cp.nWidth[ii];
		}
	}


} COORDDATAV3, *PCOORDDATAV3, NEAR *NPCOORDDATAV3, FAR *LPCOORDDATAV3;

typedef const COORDDATAV3 FAR *LPCCOORDV3;
typedef const COORDDATAV3* LPCCOORDV3;    // pointer to read/only COORD

class CCoordV3 : public tagCOORDDATAV3
{
public:
	CCoordV3();
	virtual ~CCoordV3();

	void Retrieve(CFile* pFile);
	void Save(CFile* pFile);

	void ModifyUt(CCoordV3 Cp)
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
		if(Side0.fI > 1.0f) Side0.fI = 1.0f;
		if(Side0.fJ > 1.0f) Side0.fJ = 1.0f;
		if(Side0.fK > 1.0f) Side0.fK = 1.0f;
		if(Side0.fI < -1.0f) Side0.fI = -1.0f;
		if(Side0.fJ < -1.0f) Side0.fJ = -1.0f;
		if(Side0.fK < -1.0f) Side0.fK = -1.0f;
		if(Side1.fI > 1.0f) Side1.fI = 1.0f;
		if(Side1.fJ > 1.0f) Side1.fJ = 1.0f;
		if(Side1.fK > 1.0f) Side1.fK = 1.0f;
		if(Side1.fI < -1.0f) Side1.fI = -1.0f;
		if(Side1.fJ < -1.0f) Side1.fJ = -1.0f;
		if(Side1.fK < -1.0f) Side1.fK = -1.0f;
	}

	inline void UnityVector()
	{
		float mag = Side0.fI*Side0.fI + Side0.fJ*Side0.fI + Side0.fK*Side0.fK;
		if(( mag > 1.0f )  && (mag < 10.0f)) {
			float scale = 1.0f/sqrtf(mag);
			Side0.fI *= scale;
			Side0.fJ *= scale;
			Side0.fK *= scale;
		}
		
		mag = Side1.fI*Side1.fI + Side1.fJ*Side1.fI + Side1.fK*Side1.fK;
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

	inline void SetSide0(CCoordV3 Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<9;nLoop++) {
			Side0.fPos[nLoop] = Cp.Side0.fPos[nLoop];
		}
	};

	inline void SetSide1(CCoordV3 Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<9;nLoop++) {
			Side1.fPos[nLoop] = Cp.Side1.fPos[nLoop];
		}
	};

	inline void SetSide(CCoordV3 Cp,int nSide)
	{
		int nLoop;
		switch(nSide) {
		case 0:
			for(nLoop=0;nLoop<9;nLoop++) {
				Side0.fPos[nLoop] = Cp.Side0.fPos[nLoop];
			}
			break;
		case 1:
			for(nLoop=0;nLoop<9;nLoop++) {
				Side1.fPos[nLoop] = Cp.Side1.fPos[nLoop];
			}
			break;
		}
	};


	inline static float Length(CCoordV3 Cp0, CCoordV3 Cp1, int nSide = 0)
	{
		float fX = Cp0.Side[nSide].fX - Cp1.Side[nSide].fX;
		float fY = Cp0.Side[nSide].fY - Cp1.Side[nSide].fY;
		float fZ = Cp0.Side[nSide].fZ - Cp1.Side[nSide].fZ;

		return (float)sqrt((double)(fX*fX + fY*fY + fZ*fZ));
	};

	inline float Length(CCoordV3 Cp, int nSide = 0)
	{
		float fX = Side[nSide].fX - Cp.Side[nSide].fX;
		float fY = Side[nSide].fY - Cp.Side[nSide].fY;
		float fZ = Side[nSide].fZ - Cp.Side[nSide].fZ;

		return (float)sqrt((double)(fX*fX + fY*fY + fZ*fZ));
	};

	inline static float Length(COORDDATAV3 Cp0, COORDDATAV3 Cp1, int nSide = 0)
	{
		float fX = Cp0.Side[nSide].fX - Cp1.Side[nSide].fX;
		float fY = Cp0.Side[nSide].fY - Cp1.Side[nSide].fY;
		float fZ = Cp0.Side[nSide].fZ - Cp1.Side[nSide].fZ;

		return (float)sqrt((double)(fX*fX + fY*fY + fZ*fZ));
	};

	inline void CopySides(int nDest,int nSrc) { Side[nDest] = Side[nSrc]; };
	
	inline void CopySide(int nSide,CCoordV3 Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<9;nLoop++) {	Side[nSide].fPos[nLoop] = Cp.Side[nSide].fPos[nLoop]; };
	}

	inline CCoordV3 operator + (CCoordV3 Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<18;nLoop++) {	fPos[nLoop] += Cp.fPos[nLoop]; };
		for(nLoop=0;nLoop<14;nLoop++) {	nElec[nLoop] += Cp.nElec[nLoop]; };
		return (*this);
	};

	inline CCoordV3 operator - (CCoordV3 Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<18;nLoop++) {	fPos[nLoop] -= Cp.fPos[nLoop]; };
		for(nLoop=0;nLoop<14;nLoop++) {	nElec[nLoop] -= Cp.nElec[nLoop]; };
		return (*this);
	};

	inline void operator -=(CCoordV3 Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<18;nLoop++) {	fPos[nLoop] -= Cp.fPos[nLoop]; };
		for(nLoop=0;nLoop<14;nLoop++) {	nElec[nLoop] -= Cp.nElec[nLoop]; };
	};

	inline void operator +=(CCoordV3 Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<18;nLoop++) {	fPos[nLoop] += Cp.fPos[nLoop]; };
		for(nLoop=0;nLoop<14;nLoop++) {	nElec[nLoop] += Cp.nElec[nLoop]; };
	};

	inline void operator -=(COORDDATAV3 Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<18;nLoop++) {	fPos[nLoop] -= Cp.fPos[nLoop]; }
		for(nLoop=0;nLoop<14;nLoop++) { nElec[nLoop] -= Cp.nElec[nLoop]; };
	}

	inline void operator +=(COORDDATAV3 Cp)
	{
		int nLoop;
		for(nLoop=0;nLoop<18;nLoop++) { fPos[nLoop] += Cp.fPos[nLoop]; }
		for(nLoop=0;nLoop<14;nLoop++) { nElec[nLoop] += Cp.nElec[nLoop]; }
	};

	inline void operator =(CCoordV3 Cp)	{ CopyMemory(&nStructSize, &Cp.nStructSize, sizeof tagCOORDDATAV3); };
	inline void operator =(COORDDATAV3 Cp)	{ CopyMemory(&nStructSize, &Cp, sizeof tagCOORDDATAV3); };

	inline operator LPCCOORDV3() const { return this; }

	inline void Zero()
	{ 
		memset(&nStructSize, 0, sizeof COORDDATAV3);
		nStructSize = sizeof COORDDATAV3;
	};

	inline void Zero(int nSide)
	{ 
		ZeroMemory(&Side[nSide], sizeof Side0);
	};

	inline CCoordV3(float fX,float fY,float fZ,float fI,float fJ,float fK,int nSide) {
		Side[nSide].fX = fX; Side[nSide].fY = fY; Side[nSide].fZ = fZ;
		Side[nSide].fI = fI; Side[nSide].fJ = fJ; Side[nSide].fK = fK;
	};
	inline CCoordV3(SUBCOORDV3 Cp,int nSide)	{ Side[nSide] = Cp; };
	inline CCoordV3(const COORDDATAV3& Cp) { CopyMemory(&nStructSize, &Cp, sizeof tagCOORDDATAV3); }; //Checked ok

private:
};


#endif // !defined(AFX_COORDV3_H__6E86A25D_0EEA_46D6_A92C_85ABED6AA25E__INCLUDED_)
