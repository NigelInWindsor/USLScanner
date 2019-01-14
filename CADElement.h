// CADElement.h: interface for the CCADElement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADELEMENT_H__E3B6AF4E_3D33_4B07_BF47_7ADDC64322C5__INCLUDED_)
#define AFX_CADELEMENT_H__E3B6AF4E_3D33_4B07_BF47_7ADDC64322C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	CAD_LENGTH			0x00000001
#define	CAD_WIDTH			0x00000002
#define	CAD_HEIGHT			0x00000004
#define	CAD_TOTAL_AREA		0x00000008
#define	CAD_CIRCUMFERENCE	0x00000010
#define	CAD_HISTOGRAM		0x00001f00
#define	CAD_REF_LEVEL		0x00000100
#define	CAD_DEFECT_LEVEL	0x00000200
#define	CAD_DEFECT_AREA		0x00000400
#define	CAD_DEFECT_WIDTH	0x00000800
#define	CAD_DEFECT_HEIGHT	0x00001000
#define	CAD_START_ARROW		0x00002000
#define	CAD_FINISH_ARROW	0x00004000
#define CAD_LINE_TEXT		0x00008000

class CCADElement  
{
public:
	void Retrieve(CUSLFile *pFile);
	ULONGLONG Save(CUSLFile *pFile);
	float GetLength();
	void SetLastSettings();
	void SetToLastSettings();
	HistogramStruct	m_Hist;
	float			m_fWidth;
	float			m_fHeight;
	float			m_fTotalArea;
	float			m_fLength;
	LOGFONT			m_lf;
	int				m_nFlags;
	int				m_nLineWidth;
	int				m_nTS;
	int				m_nGate;
	int				m_nImageNumber;
	int				m_nImageType;
	int				m_nPaletteNumber;
	int				m_nCADDefectPaintMode;
	WCHAR			m_wText[256];
	COLORREF		m_rgbColor;
	COLORREF		m_rgbText;
	CADElementType	m_Type;
	CPolyCoord		m_Nodes;

	void operator = (CCADElement* pEl);
	void Zero();
	CCADElement();
	virtual ~CCADElement();

};

#endif // !defined(AFX_CADELEMENT_H__E3B6AF4E_3D33_4B07_BF47_7ADDC64322C5__INCLUDED_)
