#pragma once

class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

// CPropertiesPane

class CPropertiesPane : public CDockablePane
{
	DECLARE_DYNAMIC(CPropertiesPane)

public:
	int m_nToolBarID[10] = { IDR_AIR_WATER_SIDE_0_TOOLBAR, IDR_AIR_WATER_SIDE_1_TOOLBAR, IDR_DUAL_WATERPATH_THICKNESS_TOOLBAR, IDR_MOTION_TOOLBAR_DUAL_SQUIRTER ,
		IDR_ESTOP_JOY_SAFE_TOOLBAR , IDR_LOG_ON_OFF_TOOLBAR , IDR_USEFULL_TOOLBAR, IDR_DANGERPLANE_TOOLBAR, IDR_TIPS_TOOLBAR, IDR_WP_PL_TIPS };

	PropertiesList	m_nWhichList;
	int m_nComboHeight;
	CPropertiesPane();
	virtual ~CPropertiesPane();

protected:
	CFont m_Font;
	CComboBox m_wndObjectCombo;
	CPropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void AdjustLayout();
	int CreateFont();
	HRESULT SetAccessPrivelages(WPARAM wp = NULL, LPARAM lp = NULL);
	int ChangeFontSize();
	int MainFrameRedraw(bool bAction);
	void InitPropList(PropertiesList nWhichList);
	afx_msg void OnSelendokProperty();
	void InitPropListApplication();
	void InitPropListPositionDisplay();
	void InitPropListRuler();
	void InitPropListKinematics();
	void InitPropListProfile();
	void InitPropListScope();
	void InitPropListTabs();
	void InitPropListToolbars();
	void InitPropListCScan();
	void InitPropListNAD();
	void InitPropListMachineDescription();
	void InitPropListUtSubset();
	void InitPropListUtCopy();
	afx_msg void OnContextMenu(CWnd *, CPoint);
	afx_msg void OnRclickPropertyList(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnUtSubsetFillall();
	afx_msg void OnUtSubsetClearall();
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUtAlltrue();
	afx_msg void OnUtAllfalse();
};


