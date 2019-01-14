// FileThumbNailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "FileThumbNailDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileThumbNailDlg

IMPLEMENT_DYNAMIC(CFileThumbNailDlg, CFileDialog)

CFileThumbNailDlg::CFileThumbNailDlg(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
    m_ofn.Flags |= OFN_ENABLESIZING;
    m_ofn.hInstance = AfxGetInstanceHandle();
	
}

CFileThumbNailDlg::~CFileThumbNailDlg()
{
}

void CFileThumbNailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3DViewEditDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileThumbNailDlg, CFileDialog)
	//{{AFX_MSG_MAP(CFileThumbNailDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()





BOOL CFileThumbNailDlg::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	
	CRect rect;
	CWnd * parent = GetParent();
	parent->GetWindowRect(&rect);
	parent->SetWindowPos(0,rect.left,rect.top,rect.Width()+200,rect.Height(),0);

	ScreenToClient(rect);
	CRect staticWinRect(rect.Width(),rect.top+65,rect.Width()+200-20,rect.Height()-100);
	m_staticPreview.Create(_T("VC++ shows its mystries"),WS_CHILD|WS_VISIBLE|WS_BORDER,staticWinRect,parent);

//	CRect buttonWinRect = staticWinRect;
//	buttonWinRect.top = staticWinRect.bottom + 20;
//	buttonWinRect.bottom = buttonWinRect.top + 20;
//	m_buttonCheck.Create(_T("Keep current measured ref pts"),WS_CHILD|WS_VISIBLE|WS_BORDER,buttonWinRect,parent,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFileThumbNailDlg::UpdatePreview(CString csTemp)
{
	CDC* pDC;
	CRect rrScreen,rrDevice;
	CProfile* pProfile;
	CData* pData;

	m_staticPreview.GetWindowRect(&rrScreen);
	rrScreen.DeflateRect(1,1);
	rrDevice.SetRect(0,0,rrScreen.Width(),rrScreen.Height());

	pDC = m_staticPreview.GetDC();
	pDC->FillRect(&rrDevice,&CBrush(RGB(255,255,255)));

	if(csTemp.Find(_T(".pro"),0)>0) {
		pProfile = new CProfile();
		pProfile->DisplayThumbNail(csTemp,pDC,rrDevice);
		delete pProfile;
	}
	if(csTemp.Find(_T(".dat"),0)>0) {
		pData = new CData();
		pData->DisplayThumbNail(csTemp,pDC,rrDevice);
		delete pData;
	}

	m_staticPreview.ReleaseDC(pDC);

}

BOOL CFileThumbNailDlg::OnNotify(WPARAM, LPARAM lp, LRESULT *pResult)
{
	LPOFNOTIFY of =	(LPOFNOTIFY) lp;
	CString	   csTemp;
	WCHAR	   tszBuffer[_MAX_PATH];

	switch (of->hdr.code)
	{
	case CDN_SELCHANGE:
		if (CommDlg_OpenSave_GetFilePath(GetParent()->GetSafeHwnd(), tszBuffer, sizeof(tszBuffer)) > 0) {
//			TRACE0(_T("%s\n"), tszBuffer);
			UpdatePreview(tszBuffer);
		}
		break;

	case CDN_FOLDERCHANGE:
		//	Once we	get	this notification our old subclassing of
		//	the	SHELL window is	lost, so we	have to
		//	subclass it	again. (Changing the folder	causes a 
		//	destroy	and	recreate of	the	SHELL window).

//		m_wndHook.SubclassWindow(GetParent()->GetDlgItem(lst2)->GetSafeHwnd());
//		UpdatePreview(_T(""));
		break;
	}

	*pResult = 0;
	return FALSE;
}
