#include "stdafx.h"
#include "USLPropertyPage.h"


BEGIN_MESSAGE_MAP(CUSLPropertyPage, CPropertyPage)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

CUSLPropertyPage::CUSLPropertyPage(UINT nIDTemplate,UINT nIDCaption,DWORD dwSize)
	: CPropertyPage(nIDTemplate, nIDCaption, dwSize)
{
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog
}


CUSLPropertyPage::~CUSLPropertyPage()
{
}
