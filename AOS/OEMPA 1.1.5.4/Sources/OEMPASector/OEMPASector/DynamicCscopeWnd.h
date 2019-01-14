#pragma once

// CDynamicCscopeWnd

class CDynamicCscopeWnd : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CDynamicCscopeWnd)

public:
	CDynamicCscopeWnd();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual ~CDynamicCscopeWnd();

protected:
	int m_iIndexLayoutFile;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnExitSizeMove(WPARAM wParam, LPARAM lParam);
};
