#pragma once

// CDynamicNCWnd

class CDynamicNCWnd : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CDynamicNCWnd)

public:
	CDynamicNCWnd();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual ~CDynamicNCWnd();

protected:
	int m_iIndexLayoutFile;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
