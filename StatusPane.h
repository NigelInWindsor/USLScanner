#pragma once
#define	_NO_FUNCTION	0
#define	_ECAT			1
#define	_JOYSTICK		2
#define	_SI10			3
#define	_THREADS		4


// CStatusPane
class CStatusToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CStatusPane : public CDockablePane
{
	DECLARE_DYNAMIC(CStatusPane)

public:
	int	m_nBorder;
	int m_nRows;
	int m_nColumns;
	int	m_nToolOption;
	CFont *m_pFont;
	CRect* m_pRect;
	CStatusToolBar m_wndToolBar;
	CStatusToolBar m_wndEcatToolBar;
	CStatusToolBar m_wndJoystickToolBar;
	CStatusToolBar m_wndSi10ToolBar;
	void RenderEthercat(CDC *pDC, CRect rr);
	void RenderJoystick(CDC * pDC, CRect rect);
	void RenderThreads(CDC * pDC, CRect rect);
	CStatic m_staticView;
	CStatusPane();
	virtual ~CStatusPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CFont * SetRowsColumnsFonts(CDC * pDC, int nRows, int nColumns, CString strLargestStr);
	void SetRowsColumns(int nRows, int nColumns);
	void TextAt(CDC * pDC, int nRow, int nColumn, CString Buff, int nJustify = 0);
	void TextAt(CDC * pDC, int nRow, int nColumn, int nValue, int nJustify = 0);
	void TextAt(CDC * pDC, int nRow, int nColumn, float fValue, int nJustify = 0);
	void RenderGrid(CDC * pDC, COLORREF rgb);
	void AdjustLayout();
	afx_msg void OnButtonStatusEcat();
	afx_msg void OnUpdateButtonStatusEcat(CCmdUI * pCmdUI);
	afx_msg void OnButtonEthercatRestart();
	afx_msg void OnButtonStatusJoystick();
	afx_msg void OnUpdateButtonStatusJoystick(CCmdUI * pCmdUI);
	afx_msg void OnButtonStatusThreads();
	afx_msg void OnUpdateButtonStatusThreads(CCmdUI * pCmdUI);
};


