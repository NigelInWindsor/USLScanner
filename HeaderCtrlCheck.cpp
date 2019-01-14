// HeaderCtrlCheck.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "HeaderCtrlCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrlCheck

CHeaderCtrlCheck::CHeaderCtrlCheck()
{
}

CHeaderCtrlCheck::~CHeaderCtrlCheck()
{
}


BEGIN_MESSAGE_MAP(CHeaderCtrlCheck, CHeaderCtrl)
	//{{AFX_MSG_MAP(CHeaderCtrlCheck)
	ON_NOTIFY_REFLECT(HDN_ITEMCLICK, OnItemClicked)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckHeadCtrl message handlers
void CHeaderCtrlCheck::OnItemClicked(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMHEADER* pNMHead = (NMHEADER*)pNMHDR;
	*pResult = 0;

	int nItem = pNMHead->iItem;
	if (0 != nItem)
		return;

	HDITEM hdItem;
	hdItem.mask = HDI_IMAGE;
	VERIFY( GetItem(nItem, &hdItem) );

	if (hdItem.iImage == 1)
		hdItem.iImage = 2;
	else
		hdItem.iImage = 1;

	VERIFY( SetItem(nItem, &hdItem) );
	
	BOOL bl = hdItem.iImage == 2 ? TRUE : FALSE;
	CListCtrl* pListCtrl = (CListCtrl*)GetParent();
	int nCount = pListCtrl->GetItemCount();	
	for(nItem = 0; nItem < nCount; nItem++)	{
		pListCtrl->SetCheck(nItem,bl);
	}
	
}
