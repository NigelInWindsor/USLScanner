#pragma once

// CHardwareCscopeWnd

class CHardwareCscopeWnd : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CHardwareCscopeWnd)

public:
	CHardwareCscopeWnd();

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual ~CHardwareCscopeWnd();

protected:
	int m_iIndexLayoutFile;
	int m_iGateId;
	wchar_t m_pTitle[20];
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnExitSizeMove(WPARAM wParam, LPARAM lParam);
};
