// CoordV3.cpp: implementation of the CCoordV3 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "CoordV3.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCoordV3::CCoordV3()
{

}

CCoordV3::~CCoordV3()
{

}

void CCoordV3::Save(CFile *pFile)
{
	nStructSize = sizeof tagCOORDDATAV3;
	pFile->Write(&nStructSize,nStructSize);
}

void CCoordV3::Retrieve(CFile *pFile)
{
	int	nSize;
	int jj = sizeof nSize;
	jj *= -1;

	pFile->Read(&nSize,sizeof (int));
	if(nSize>0) {
		pFile->Seek(jj,CFile::current);
		pFile->Read(&nStructSize,nSize);
	};

}
