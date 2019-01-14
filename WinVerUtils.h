// WinVerUtils.h: interface for the CWinVerUtils class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINVERUTILS_H__0B67958C_D60E_47D5_92EE_0F633EA20AD1__INCLUDED_)
#define AFX_WINVERUTILS_H__0B67958C_D60E_47D5_92EE_0F633EA20AD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWinVerUtils  
{
public:
	bool IsWin2008R2();
	bool IsWin2k3OrLower();
	bool IsWin2k();
	CWinVerUtils();
	virtual ~CWinVerUtils();

};

#endif // !defined(AFX_WINVERUTILS_H__0B67958C_D60E_47D5_92EE_0F633EA20AD1__INCLUDED_)
