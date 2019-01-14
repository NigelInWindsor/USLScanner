#if !defined(AFX_USLFILE_H__D4B84B87_FB2E_4B71_B239_E65042E8AA89__INCLUDED_)
#define AFX_USLFILE_H__D4B84B87_FB2E_4B71_B239_E65042E8AA89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// USLFile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUSLFile dialog

#define	ATTRIB_NORMAL	 0x00
#define	ATTRIB_READONLY  0x01
#define	ATTRIB_HIDDEN    0x02
#define	ATTRIB_SYSTEM    0x04
#define	ATTRIB_VOLUME    0x08
#define	ATTRIB_DIRECTORY 0x10
#define	ATTRIB_ARCHIVE   0x20



class CUSLFile : public CFile
{
	DECLARE_DYNAMIC(CUSLFile)

public:
	float m_fVersion;
	int ReadXMLLine(CString &Buff);
	void WriteStringChar(LPCTSTR lpsz);
	bool ZeroBlockOfMemory(ULONGLONG n64StartOffset, unsigned int nSize);
	bool static IsAvailable(CString FileName);
	void DecodeFileException(CFileException* pExcep, CString *pBuff,CString FileName);
	int GetAttributes(CString FileName);
	int WriteUnicodeStringAsAscii(CString *pBuff);
	int Read(void *pArray,int nSize);
	int Write(void *pArray,int nSize);
	int WriteStruct(void *pArray,int nSize);
	int ReadStruct(void *pArray, int nMaxSize);
	int WriteString(CString* pBuff);
	int ReadString(CString* pBuff);
	int Open(LPCTSTR lpszFileName,UINT nOpenFlags,CFileException *pError=NULL);
	CUSLFile(LPCTSTR lpszFileName=NULL, UINT nOpenFlags=0 ); 

protected:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USLFILE_H__D4B84B87_FB2E_4B71_B239_E65042E8AA89__INCLUDED_)
