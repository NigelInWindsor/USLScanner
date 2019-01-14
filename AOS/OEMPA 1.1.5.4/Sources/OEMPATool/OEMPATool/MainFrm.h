// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "DisplayImage.h"
#include "ChildView.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CChildView    m_wndView;

// Generated message map functions
protected:
	int m_iIndexLayoutFile;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditHWDialog();
	afx_msg void OnEditSetupFilesDialog();
	afx_msg void OnClose( );
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnEditMaxdialog();
	afx_msg void OnEditKernelmessagebox();
	afx_msg void OnEditTestfwmemory();
	afx_msg void OnUpdateEditTestfwmemory(CCmdUI *pCmdUI);
};


