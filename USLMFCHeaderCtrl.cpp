#include "stdafx.h"
#include "USLMFCHeaderCtrl.h"

IMPLEMENT_DYNAMIC(CUSLMFCHeaderCtrl, CMFCHeaderCtrl)

BEGIN_MESSAGE_MAP(CUSLMFCHeaderCtrl, CMFCHeaderCtrl)
//ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


CUSLMFCHeaderCtrl::CUSLMFCHeaderCtrl()
{
}


CUSLMFCHeaderCtrl::~CUSLMFCHeaderCtrl()
{
}



//176, 196, 222
/*
BOOL CUSLMFCHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	CUSLMFCHeaderCtrl::GetClientRect(rect);

	CBrush brush0(RGB(176, 0, 0));
	pDC->FillRect(&rect, &brush0);
	brush0.DeleteObject();
	return FALSE;
}

void CUSLMFCHeaderCtrl::OnFillBackground(CDC* pDC)
{
	CRect rect;
	CUSLMFCHeaderCtrl::GetClientRect(rect);
	CBrush brush0(RGB(176, 0, 0));
	pDC->FillRect(&rect, &brush0);
	brush0.DeleteObject();
	return CMFCHeaderCtrl::OnFillBackground(pDC);
}
*/

