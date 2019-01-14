
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "DynamicSettingDlg.h"
#include "AnalysisToolsDlg.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	static CMainFrame *GetMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void InitPlacement();

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	bool SetCursor(double dX,double dZ);
	bool ResetCursor();

protected:  // control bar embedded members
	static CMainFrame *g_pMainFrame;
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	int m_iIndexLayoutFile;//integration

	CDynamicSettingDlg m_SettingDlg;
	CAnalysisToolsDlg* m_pAnalysisDlg;

	afx_msg void OnFileDemoLoad();

	afx_msg void OnFileLoad();
	afx_msg void OnFileSave();

	afx_msg void OnDynamicView();
	afx_msg void OnUpdateDynamicView(CCmdUI* pCmdUI);
	afx_msg void OnDynamicNCView();
	afx_msg void OnUpdateDynamicNCView(CCmdUI* pCmdUI);
	afx_msg void OnDynamicCscopeView();
	afx_msg void OnUpdateDynamicCscopeView(CCmdUI* pCmdUI);

	afx_msg void OnAscopeView();
	afx_msg void OnUpdateAscopeView(CCmdUI* pCmdUI);
	afx_msg void OnBscopeView();
	afx_msg void OnUpdateBscopeView(CCmdUI* pCmdUI);
	afx_msg void OnCscopeView();
	afx_msg void OnUpdateCscopeView(CCmdUI* pCmdUI);
	afx_msg void OnDscopeView();
	afx_msg void OnUpdateDscopeView(CCmdUI* pCmdUI);
	afx_msg void OnDscopeCorrectedView();
	afx_msg void OnUpdateDscopeCorrectedView(CCmdUI* pCmdUI);

// Generated message map functions
protected:
	void OnViewsHardwareCscope(bool bAmp,int iGate);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnClose( );
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnEventMessage(WPARAM wParam,LPARAM lParam);//integration
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnViewsHardwareCscopeAmp0();
	afx_msg void OnUpdateViewsHardwareCscopeAmp0(CCmdUI *pCmdUI);
	afx_msg void OnViewsHardwareCscopeAmp1();
	afx_msg void OnUpdateViewsHardwareCscopeAmp1(CCmdUI *pCmdUI);
	afx_msg void OnViewsHardwareCscopeAmp2();
	afx_msg void OnUpdateViewsHardwareCscopeAmp2(CCmdUI *pCmdUI);
	afx_msg void OnViewsHardwareCscopeAmp3();
	afx_msg void OnUpdateViewsHardwareCscopeAmp3(CCmdUI *pCmdUI);

	afx_msg void OnViewsHardwareCscopeTof0();
	afx_msg void OnUpdateViewsHardwareCscopeTof0(CCmdUI *pCmdUI);
	afx_msg void OnViewsHardwareCscopeTof1();
	afx_msg void OnUpdateViewsHardwareCscopeTof1(CCmdUI *pCmdUI);
	afx_msg void OnViewsHardwareCscopeTof2();
	afx_msg void OnUpdateViewsHardwareCscopeTof2(CCmdUI *pCmdUI);
	afx_msg void OnViewsHardwareCscopeTof3();
	afx_msg void OnUpdateViewsHardwareCscopeTof3(CCmdUI *pCmdUI);
};


