// Vertex.cpp: implementation of the CUSLVertex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "Vertex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUSLVertex::CUSLVertex()
{
	ZeroMemory(fPos, sizeof fPos);
}

CUSLVertex::~CUSLVertex()
{

}

void CUSLVertex::Save(CFile *pFile)
{
	pFile->Write(fPos, sizeof fPos);
}

void CUSLVertex::Retrieve(CFile *pFile)
{
	int nSize = sizeof fPos;

	pFile->Read(fPos,nSize);

}




