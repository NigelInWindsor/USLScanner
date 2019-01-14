// USLScans.h: interface for the CUSLScans class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USLSCANS_H__E232230C_AA54_4AC6_A1F9_D7AD57B25BC6__INCLUDED_)
#define AFX_USLSCANS_H__E232230C_AA54_4AC6_A1F9_D7AD57B25BC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUSLScans  
{
public:
	bool Initialize(CString *pFilePath);
	UINT64 m_n64DataOffsetArray[1024];
	int m_nDataL;
	void Zero();
	void operator = (CUSLScans* pScans);
	bool Save(CUSLFile * pFile, CData * pData);
	CString m_FilePath;
	CString m_FileName;
	CUSLScans();
	virtual ~CUSLScans();

};

#endif // !defined(AFX_USLSCANS_H__E232230C_AA54_4AC6_A1F9_D7AD57B25BC6__INCLUDED_)
