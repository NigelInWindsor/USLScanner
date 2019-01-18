#pragma once
#include "ListItemEdit.h"
#include "USLMFCListCtrl.h"

class CAxesPane : public CDockablePane
{
	DECLARE_DYNAMIC(CAxesPane)

protected:
	CListItemEdit * m_pListItemEdit;
	CFont		m_Font;
	CUSLMFCListCtrl	m_listAxes;
	CString		m_strHomeMode[3];
	CString		m_strAmplifier[10];
	CString		m_strUnits[3];
	int	m_nNumberColumns;
	int	m_nColumnAxis[30];
	int	m_nIndex;
	int	m_nColumn;
	int m_nRow;
	float m_fEditValue;

public:
	void CreateFont();
	void ChangeFontSize();
	HRESULT SetAccessPrivelages(WPARAM wp = NULL, LPARAM lp = NULL);
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	void CreateColumns();
	void FillList();
	void AdjustLayout();
	CAxesPane();
	~CAxesPane();
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetdispinfoListAxes(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnDblclkListAxes(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnClickListAxes(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg LRESULT ItemChanged(WPARAM wp, LPARAM lp);
	afx_msg void OnRclickListUsers(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnAxesSethomevalue();
	afx_msg void OnAxesDownloadtoppmac();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnAxesDefault();
};

