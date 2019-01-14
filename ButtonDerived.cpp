// ButtonDerived.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ButtonDerived.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonDerived

CButtonDerived::CButtonDerived()
{
}

CButtonDerived::~CButtonDerived()
{
}


BEGIN_MESSAGE_MAP(CButtonDerived, CButton)
	//{{AFX_MSG_MAP(CButtonDerived)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonDerived message handlers

void CButtonDerived::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    CDC* pDC   = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect = lpDrawItemStruct->rcItem;
    UINT state = lpDrawItemStruct->itemState;


    // draw the control edges (DrawFrameControl is handy!)
    if (state & ODS_SELECTED)
        pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_FLAT | DFCS_PUSHED);
    else
        pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_FLAT);

    // Deflate the drawing rect by the size of the button's edges
    rect.DeflateRect( CSize(GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CYEDGE)));
	
}

void CButtonDerived::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CButton::PreSubclassWindow();

	ModifyStyle(0, BS_OWNERDRAW);	// make the button owner drawn

}
