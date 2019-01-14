// Token.cpp: implementation of the CToken class.

#include "stdafx.h"
#include "Token.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToken::CToken(CString str)
{
	m_strToToken = str;
	m_strLeft = str;
	m_strToken = " ";
	m_bFoundAny = FALSE;
	m_bFinalToken = FALSE;
}

CToken::~CToken()
{

}

BOOL CToken::MoreTokens()
{
	if ((m_strLeft.Find(m_strToken,0) == -1) && (m_bFinalToken))
	{
		return TRUE; // The string that is left over is a token
	}
	else if ((m_strLeft.Find(m_strToken,0) == -1) && (!m_bFinalToken))
	{
		return FALSE; // The string contains no tokens
	}
	else
		return TRUE;
}


CString CToken::GetNextToken()
{
	
	if(m_bFinalToken)
	{
		m_bFinalToken = FALSE;
		return m_strLeft;
	}
	

	int pos, len;
	CString newtok = m_strLeft; // Our own working copy of the string left
	len = newtok.GetLength();
	pos = newtok.Find(m_strToken,0);
	if (pos == -1)
	{
		// This should never happen
	}
	m_bFoundAny = TRUE;
	newtok = newtok.Left(pos);
	pos++;
	m_strLeft = m_strLeft.Right(len - pos);

	//Check to see if there anymore tokens
	if (m_strLeft.Find(m_strToken,0) == -1)
	{
		// No more tokens but the string left is the remaining token
		m_bFinalToken = TRUE;
	}

	return newtok;

}
