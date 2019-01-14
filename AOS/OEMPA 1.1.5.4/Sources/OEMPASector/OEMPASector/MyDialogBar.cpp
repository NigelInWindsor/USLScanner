// MyDialogBar.cpp : implementation file
//

#include "stdafx.h"
#include "SectorApp.h"
#include "MyDialogBar.h"

// CMyDialogBar

IMPLEMENT_DYNAMIC(CMyDialogBar, CDialogBar)

CMyDialogBar::CMyDialogBar()
{

}

CMyDialogBar::~CMyDialogBar()
{
}

BEGIN_MESSAGE_MAP(CMyDialogBar, CDialogBar)
END_MESSAGE_MAP()

// CMyDialogBar message handlers

BOOL CMyDialogBar::OnInitDialogBar() 
{   
	UpdateData(FALSE);  
	return TRUE; 
} 

void CMyDialogBar::DoDataExchange(CDataExchange* pDX) 
{ 
  ASSERT(pDX); 
  CDialogBar::DoDataExchange(pDX); 
} 

BOOL CMyDialogBar::Create(CWnd* pParentWnd, LPCTSTR lpszTemplateName, UINT nStyle, UINT nID) 
{ 
    if (!CDialogBar::Create(pParentWnd, lpszTemplateName, nStyle, nID)) 
        return FALSE; 

    if (!OnInitDialogBar()) 
        return FALSE; 

    return TRUE; 
} 

BOOL CMyDialogBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID) 
{ 
    if (!CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID)) 
        return FALSE; 

    if (!OnInitDialogBar()) 
        return FALSE; 

    return TRUE; 
} 