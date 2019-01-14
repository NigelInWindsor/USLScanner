// Vertex.h: interface for the CVertex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERTEX_H__D8FA16A2_7C03_44DA_8FA7_30A61C5D2F0C__INCLUDED_)
#define AFX_VERTEX_H__D8FA16A2_7C03_44DA_8FA7_30A61C5D2F0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CUSLVertex
{
public:
	union
	{
		struct
		{
			float fX, fY, fZ, fI, fJ, fK, fW;
		};
		struct
		{
			D3DXVECTOR3 pt;
			D3DXVECTOR4 norm;
		};
		float	fPos[7]; // Array access useful in loops
	};


	void Retrieve(CFile* pFile);
	void Save(CFile* pFile);

	CUSLVertex();
	~CUSLVertex();

};

typedef struct tagEDGE {

	int		nNode[2];
	CUSLVertex	v[2];
	float	fLength;

	float SetLength() {
		return fLength = D3DXVec3Length(&(v[1].pt - v[0].pt));
	}

} EDGE, *PEDGE;


typedef struct tagTRIANGLE {

	EDGE e[3];

} TRIANGLE, *PTRIANGLE;


#endif // !defined(AFX_VERTEX_H__D8FA16A2_7C03_44DA_8FA7_30A61C5D2F0C__INCLUDED_)
