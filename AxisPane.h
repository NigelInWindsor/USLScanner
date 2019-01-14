#pragma once
#include "USLDockablePane.h"

class CAxisPane :
	public CDockablePane
{
public:
	CAxisPane();
	~CAxisPane();

public:
	CFont		m_Font;
	CComboBox m_wndObjectCombo;
	CMFCPropertyGridCtrl m_wndPropList;

	int InitPropList();

protected:
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnSelendokProperty();


protected:
	int		m_nAxis;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void AdjustLayout();
	int CreateFont();
	HRESULT SetAccessPrivelages(WPARAM wp = NULL, LPARAM lp = NULL);
};

