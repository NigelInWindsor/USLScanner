// CADElement.cpp: implementation of the CCADElement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "CADElement.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCADElement::CCADElement()
{
	Zero();
}

CCADElement::~CCADElement()
{

}

void CCADElement::Zero()
{

	m_Type = CAD_MOUSE;
	m_fWidth = 0.0f;
	m_fHeight =	0.0f;
	m_fTotalArea = 0.0f;
	m_fLength =	0.0f;
	m_Nodes.Zero();
	ZeroMemory(&m_Hist, sizeof m_Hist);
	SetToLastSettings();
}

void CCADElement::operator = (CCADElement* pEl)
{
	m_Type =					pEl->m_Type;
	m_nLineWidth =				pEl->m_nLineWidth;
	m_rgbColor =				pEl->m_rgbColor;
	m_rgbText =					pEl->m_rgbText;
	m_nTS =						pEl->m_nTS;
	m_nGate =					pEl->m_nGate;
	m_nImageNumber =			pEl->m_nImageNumber;
	m_nImageType =				pEl->m_nImageType;
	m_nPaletteNumber =			pEl->m_nPaletteNumber;
	m_nFlags =					pEl->m_nFlags;
	m_fWidth =					pEl->m_fWidth;
	m_fHeight =					pEl->m_fHeight;
	m_fTotalArea =				pEl->m_fTotalArea;
	m_fLength =					pEl->m_fLength;
	m_nCADDefectPaintMode =		pEl->m_nCADDefectPaintMode;

	m_Nodes =					&pEl->m_Nodes;

	CopyMemory(&m_lf,	&pEl->m_lf, sizeof m_lf);
	CopyMemory(m_wText, pEl->m_wText , sizeof m_wText);
	CopyMemory(&m_Hist, &pEl->m_Hist, sizeof m_Hist);

}

void CCADElement::SetToLastSettings()
{
	CopyMemory(&m_lf, &theApp.m_LastSettings.lfCAD, sizeof m_lf);
	m_nFlags = theApp.m_LastSettings.nCADFlags;
	m_nLineWidth = theApp.m_LastSettings.nCADLineWidth;
	m_rgbColor = theApp.m_LastSettings.rgbCADColor;
	m_rgbText = theApp.m_LastSettings.rgbCADText;
	m_nCADDefectPaintMode = theApp.m_LastSettings.nCADDefectPaintMode;

}

void CCADElement::SetLastSettings()
{
	CopyMemory(&theApp.m_LastSettings.lfCAD,&m_lf,  sizeof m_lf);
	theApp.m_LastSettings.nCADFlags = m_nFlags;
	theApp.m_LastSettings.nCADLineWidth = m_nLineWidth;
	theApp.m_LastSettings.rgbCADColor = m_rgbColor;
	theApp.m_LastSettings.rgbCADText = m_rgbText;
	theApp.m_LastSettings.nCADDefectPaintMode = m_nCADDefectPaintMode;

}

float CCADElement::GetLength()
{
	return m_Nodes.GetLength();
}

ULONGLONG CCADElement::Save(CUSLFile *pFile)
{
	ULONGLONG nFileOffset = pFile->GetPosition();

	pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);

	pFile->Write(&m_Type, sizeof m_Type);
	pFile->Write(&m_nLineWidth, sizeof m_nLineWidth);
	pFile->Write(&m_rgbColor, sizeof m_rgbColor);
	pFile->Write(&m_rgbText, sizeof m_rgbText);
	pFile->Write(&m_nTS, sizeof m_nTS);
	pFile->Write(&m_nGate, sizeof m_nGate);
	pFile->Write(&m_nImageNumber, sizeof m_nImageNumber);
	pFile->Write(&m_nImageType, sizeof m_nImageType);
	pFile->Write(&m_nPaletteNumber, sizeof m_Type);
	pFile->Write(&m_nFlags, sizeof m_nFlags);
	pFile->Write(&m_fWidth, sizeof m_fWidth);
	pFile->Write(&m_fHeight, sizeof m_fHeight);
	pFile->Write(&m_fTotalArea, sizeof m_fTotalArea);
	pFile->Write(&m_fLength, sizeof m_fLength);
	pFile->Write(&m_nCADDefectPaintMode, sizeof m_nCADDefectPaintMode);

	m_Nodes.Save(pFile);

	pFile->Write(&m_lf, sizeof m_lf);
	pFile->Write(m_wText, sizeof m_wText);
	pFile->Write(&m_Hist, sizeof m_Hist);

	return nFileOffset;
}

void CCADElement::Retrieve(CUSLFile *pFile)
{
	float fVersion;

	pFile->Read(&fVersion,sizeof fVersion);

	pFile->Read(&m_Type, sizeof m_Type);
	pFile->Read(&m_nLineWidth, sizeof m_nLineWidth);
	pFile->Read(&m_rgbColor, sizeof m_rgbColor);
	pFile->Read(&m_rgbText, sizeof m_rgbText);
	pFile->Read(&m_nTS, sizeof m_nTS);
	pFile->Read(&m_nGate, sizeof m_nGate);
	pFile->Read(&m_nImageNumber, sizeof m_nImageNumber);
	pFile->Read(&m_nImageType, sizeof m_nImageType);
	pFile->Read(&m_nPaletteNumber, sizeof m_Type);
	pFile->Read(&m_nFlags, sizeof m_nFlags);
	pFile->Read(&m_fWidth, sizeof m_fWidth);
	pFile->Read(&m_fHeight, sizeof m_fHeight);
	pFile->Read(&m_fTotalArea, sizeof m_fTotalArea);
	pFile->Read(&m_fLength, sizeof m_fLength);
	pFile->Read(&m_nCADDefectPaintMode, sizeof m_nCADDefectPaintMode);

	m_Nodes.Retrieve(pFile);

	pFile->Read(&m_lf, sizeof m_lf);
	pFile->Read(m_wText, sizeof m_wText);
	pFile->Read(&m_Hist, sizeof m_Hist);


}
