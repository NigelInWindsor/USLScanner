#pragma once

// CDynamicWnd

class CDynamicWnd : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CDynamicWnd)

public:
	CDynamicWnd();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual ~CDynamicWnd();

protected:
	int m_iIndexLayoutFile;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
