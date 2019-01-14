#pragma once
class CUSLDockCreate {
public:
	int		m_nID;
	int		m_nIDHighBitMap;
	bool	m_bCombo;
	bool	m_bList;
	BOOL	m_bHasGripper;

	void Init(int nId, int nIDHighBitMap, bool bCombo, bool bList, BOOL bHasGripper)
	{
		m_nID = nId;
		m_nIDHighBitMap = nIDHighBitMap;
		m_bCombo = bCombo;
		m_bList = bList;
		m_bHasGripper = bHasGripper;
	};

public:
	CUSLDockCreate();
	CUSLDockCreate(int nId = -1, int nIDHighBitMap = 0, bool bCombo = false, bool bList = false, BOOL bHasGripper = false);
	virtual ~CUSLDockCreate();
};


class CUSLDockToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

// CUSLDockPane

class CUSLDockablePane : public CDockablePane
{
	DECLARE_DYNAMIC(CUSLDockablePane)

public:
	CUSLDockablePane();
	virtual ~CUSLDockablePane();
	virtual void AdjustLayout();



public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

public:
	int		m_nID;
	int		m_nIDHighBitMap;
	bool	m_bCombo;
	bool	m_bList;
	int	m_nIDDlg;
	int	m_nComboHeight;
	CDialogEx*	m_pwndDlg;
	CFont m_Font;
	CComboBox m_wndObjectCombo;
	CUSLDockToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszCaption, CWnd * pParentWnd, const RECT & rect, CUSLDockCreate *pCreate, DWORD dwStyle, DWORD dwTabbedStyle = AFX_CBRS_REGULAR_TABS, DWORD dwControlBarStyle = AFX_DEFAULT_DOCKING_PANE_STYLE, CCreateContext * pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual int InitPropList();
	virtual void SetAccessibility();
	void CreateFont();
	void ChangeFontSize();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};



