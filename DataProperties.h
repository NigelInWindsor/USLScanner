// DataProperties.h: interface for the CDataProperties class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAPROPERTIES_H__043B99B0_C93B_4213_B62F_4CEC79A47E5A__INCLUDED_)
#define AFX_DATAPROPERTIES_H__043B99B0_C93B_4213_B62F_4CEC79A47E5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	AUTO_INSPECTION_DATE	0x01
#define	AUTO_OPERATOR			0x02
#define	AUTO_FAST_STEPSIZE		0x04
#define	AUTO_SLOW_STEPSIZE		0x08


class CDataProperties
{
public:
	bool IsAllEntriesComplete();
	void SetModeAt(int nIndex, int nMode);
	int GetModeAt(int nIndex);
	CString GetReportFromID(int nIndex);
	void SetValueAt(int nIndex,CString Buff);
	bool SetCaptionAt(int nLanguage,int nIndex,CString Buff);
	CString GetValueAt(int nIndex);
	CString GetCaptionAt(int nLanguage, int nIndex);
	void Add(int nIndex);
	void operator = (CDataProperties* pProperties);
	void Retrieve(CUSLFile* pFile);
	bool Save(CUSLFile *pFile);
	void Zero();
	int GetSize();
	int m_nSize;
	int m_nIndex;
	WCHAR	m_wFileDescription[30];
	CStringArray m_Value;
	CStringArray m_Caption[2];
	int* m_pnMode;
	CDataProperties();
	virtual ~CDataProperties();

};

#endif // !defined(AFX_DATAPROPERTIES_H__043B99B0_C93B_4213_B62F_4CEC79A47E5A__INCLUDED_)
