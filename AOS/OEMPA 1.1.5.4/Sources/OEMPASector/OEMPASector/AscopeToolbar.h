#pragma once

#include "MyDialogBar.h"

// CAscopeToolbar dialog

class CAscopeToolbar : public CMyDialogBar
{
	DECLARE_DYNAMIC(CAscopeToolbar)

public:
	CAscopeToolbar(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAscopeToolbar();

	virtual BOOL OnInitDialogBar();
	
	int m_iProbe;
	int m_iCycle;
	int m_iDisplayMode;

	CComboBox m_cCycle;

	void FillCycleCombo();

// Dialog Data
	enum { IDD = IDD_DIALOG_ASCOPE_TOOLBAR };

	afx_msg void OnCbnSelchangeComboProbe();
	afx_msg void OnCbnSelchangeComboCycle();
	afx_msg void OnCbnSelchangeComboDisplayMode();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()
};
