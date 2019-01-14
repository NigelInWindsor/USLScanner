// CArchive64.h: interface for the CArchive64 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARCHIVE64_H__F84193CE_038F_4414_B875_0074B5519980__INCLUDED_)
#define AFX_CARCHIVE64_H__F84193CE_038F_4414_B875_0074B5519980__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CArchive64
{
public:
	void		WriteStringChar(LPCTSTR lpsz);
	void		WriteString(LPCTSTR lpsz);
	LPTSTR		ReadString(LPTSTR lpsz, UINT nMax);
	BOOL		ReadString(CString& rString);
	void		Close();
	void		Flush();

	bool		m_bNoFlushOnDelete;
	int			m_nMode;
	CFile64*	m_pFile;
	CString		m_strFileName;
	CArchive64(CFile64* pFile, UINT nMode, int nBufSize = 0, void* lpBuf = NULL);
	virtual ~CArchive64();

};

#endif // !defined(AFX_CARCHIVE64_H__F84193CE_038F_4414_B875_0074B5519980__INCLUDED_)
