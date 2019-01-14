// Token.h: interface for the CToken class.

#if !defined(AFX_TOKEN_H__7CEAEBEE_D15D_11D2_9CA9_444553540000__INCLUDED_)
#define AFX_TOKEN_H__7CEAEBEE_D15D_11D2_9CA9_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CToken  
{
public:
	virtual CString GetNextToken(); // Returns the next token
	virtual BOOL MoreTokens();				// Do we have any more tokens to retrieve
	void SetToken(CString tok) { m_strToken = tok; };
	CToken(CString str);
	virtual ~CToken();

protected:
	CString m_strToToken;	// The  original string to tokenize
	CString m_strLeft;		// The string we have left after taking tokens out
	CString m_strToken;		// What is the value to tokenize on?
	BOOL	m_bFoundAny;	// Have we found any tokens
	BOOL	m_bFinalToken;	// There might be one last token?
};

#endif // !defined(AFX_TOKEN_H__7CEAEBEE_D15D_11D2_9CA9_444553540000__INCLUDED_)
