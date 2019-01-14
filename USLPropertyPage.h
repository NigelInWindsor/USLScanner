#pragma once
class CUSLPropertyPage : CPropertyPage
{
	DECLARE_DYNCREATE(CUSLPropertyPage)

public:
	CUSLPropertyPage(UINT nIDTemplate,	UINT nIDCaption = 0,	DWORD dwSize = sizeof(PROPSHEETPAGE));
	~CUSLPropertyPage();

public:
	HBRUSH m_hBr;

protected:
	// Generated message map functions
	//{{AFX_MSG(CAmplitudePalettePage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

