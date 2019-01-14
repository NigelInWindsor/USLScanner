// RegisterActiveX.h: interface for the CRegisterActiveX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGISTERACTIVEX_H__3DE62784_3C54_11D4_A108_0050DAE2EB3D__INCLUDED_)
#define AFX_REGISTERACTIVEX_H__3DE62784_3C54_11D4_A108_0050DAE2EB3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRegisterActiveX  
{
public:
	CRegisterActiveX();
	virtual ~CRegisterActiveX();
	void Register(CString DllPathName);

};

#endif // !defined(AFX_REGISTERACTIVEX_H__3DE62784_3C54_11D4_A108_0050DAE2EB3D__INCLUDED_)
