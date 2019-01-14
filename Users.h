// Users.h: interface for the CUsers class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERS_H__0907EB11_0A98_494B_98C6_18502227F5C6__INCLUDED_)
#define AFX_USERS_H__0907EB11_0A98_494B_98C6_18502227F5C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct	IDStruct {
	WCHAR	wName[256];
	WCHAR	wPassword[256];
	WCHAR	wID[256];
	int		nAccessLevel;
};

class CUsers  
{
public:
	bool Logon(CString Name,CString Password);
	void Add(IDStruct* pID);
	void ExtractToken(WCHAR * pwStr, CString Buff);
	void AddEmpty();
	bool Save();
	bool Retrieve();
	IDStruct* m_pID;
	void Zero();
	int m_nIDL;
	CUsers();
	virtual ~CUsers();
};

#endif // !defined(AFX_USERS_H__0907EB11_0A98_494B_98C6_18502227F5C6__INCLUDED_)
