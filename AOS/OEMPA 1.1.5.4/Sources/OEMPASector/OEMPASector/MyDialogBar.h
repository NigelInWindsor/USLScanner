#pragma once

// CMyDialogBar

class CMyDialogBar : public CDialogBar
{
	DECLARE_DYNAMIC(CMyDialogBar)

public:
	CMyDialogBar();
	virtual ~CMyDialogBar();

    virtual BOOL Create(CWnd * pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID); 
    BOOL Create(CWnd * pParentWnd, LPCTSTR lpszTemplateName, UINT nStyle, UINT nID); 
    virtual BOOL OnInitDialogBar(); 

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	DECLARE_MESSAGE_MAP()
};


