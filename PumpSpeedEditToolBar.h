#pragma once
class CPumpSpeedEditToolBar : public CMFCToolBar
{
public:
	CMFCToolBarEditBoxButton *m_pEditSide0;
	CMFCToolBarEditBoxButton *m_pEditSide1;

	CPumpSpeedEditToolBar();
	~CPumpSpeedEditToolBar();
	virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle = AFX_DEFAULT_TOOLBAR_STYLE, UINT nID = AFX_IDW_TOOLBAR);
};

