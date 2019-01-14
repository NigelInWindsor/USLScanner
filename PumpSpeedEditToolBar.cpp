#include "stdafx.h"
#include "PumpSpeedEditToolBar.h"


CPumpSpeedEditToolBar::CPumpSpeedEditToolBar() : CMFCToolBar()
{
	m_pEditSide0 = NULL;
	m_pEditSide1 = NULL;
}


CPumpSpeedEditToolBar::~CPumpSpeedEditToolBar()
{
}




BOOL CPumpSpeedEditToolBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
	m_pEditSide0 = new CMFCToolBarEditBoxButton();
	m_pEditSide1 = new CMFCToolBarEditBoxButton();

	return CMFCToolBar::Create(pParentWnd, dwStyle, nID);
}
