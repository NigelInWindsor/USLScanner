#pragma once
#include "USLPaneToolBar.h"
#include "USLMFCListCtrl.h"


class CAxisStatusPane : public CDockablePane
{
	DECLARE_DYNAMIC(CAxisStatusPane)
public:
	CAxisStatusPane();
	~CAxisStatusPane();


protected:
	CUSLMFCListCtrl	m_listStatus;
	CFont			m_Font;
	CUSLPaneToolBar m_wndToolBar;
	CString			m_strUnits[3];
	int				m_nToolOption;
	int				m_nNumberRows;
	int				m_nOldPos[32];
	int				m_nOldStatus[32];
	int				m_nAxisList[32];

protected:
	DECLARE_MESSAGE_MAP()
public:
	void FillList();
	void CreateColumns();
	void AdjustLayout();
	void OnChangeVisualStyle();
	void CreateFont();
	void ChangeFontSize();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnGetdispinfoListStatus(NMHDR * pNMHDR, LRESULT * pResult);
};

