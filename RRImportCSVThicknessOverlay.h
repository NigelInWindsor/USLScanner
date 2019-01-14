// RRImportCSVThicknessOverlay.h: interface for the CRRImportCSVThicknessOverlay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RRIMPORTCSVTHICKNESSOVERLAY_H__AB9004BD_9F48_41F0_89C0_9049A4E1CA1E__INCLUDED_)
#define AFX_RRIMPORTCSVTHICKNESSOVERLAY_H__AB9004BD_9F48_41F0_89C0_9049A4E1CA1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct	CSVHeaderData {
	float	fSection;
	float	fLEToTE;
	float	fAxisToLE;
	int		nNumberSamplesToLE;
	float	fLeftOverToLE;
	float	fAxisToTE;
	int		nNumberSamplesToTE;
	float	fLeftOverToTE;
	float	fBladeFormAngle;
};


class CRRImportCSVThicknessOverlay  
{
public:
	CString getSection(int nLine);
	float getBladeFormAngle(int nLine);
	int getStackingAxisToLE(int nLine);
	void getThicknessLine(int nLine,float *pfArray);
	int getSamplesLine();
	struct CSVHeaderData *m_pHeaderData;
	bool Retrieve(CFile* pFile);
	int	m_nLineNumber;
	int	m_nNumberHeaderRows;
	int	m_nDataFirstRow;
	int m_nNumberLines;
	int m_nSamplesLine;
	int	m_nOffset;
	CStringArray m_strTextRows;
	float *m_pfThickness;
	bool m_bMirror;
	CRRImportCSVThicknessOverlay();
	virtual ~CRRImportCSVThicknessOverlay();

};

#endif // !defined(AFX_RRIMPORTCSVTHICKNESSOVERLAY_H__AB9004BD_9F48_41F0_89C0_9049A4E1CA1E__INCLUDED_)
