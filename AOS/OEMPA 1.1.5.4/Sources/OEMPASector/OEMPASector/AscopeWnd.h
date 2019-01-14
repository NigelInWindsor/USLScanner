#pragma once

#include "AscopeToolbar.h"

// CAscopeWnd frame

class CAscopeWnd : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CAscopeWnd)
protected:
	CAscopeWnd();           // protected constructor used by dynamic creation
	virtual ~CAscopeWnd();

	CAscopeToolbar m_Toolbar;
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

protected:
	int m_iIndexLayoutFile;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


