// MultipleScansPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "MultipleScansPage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultipleScansPage property page

IMPLEMENT_DYNCREATE(CMultipleScansPage, CResizablePage)

CMultipleScansPage::CMultipleScansPage() : CResizablePage(CMultipleScansPage::IDD)
{
	//{{AFX_DATA_INIT(CMultipleScansPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nUSLScansL = 0;
	m_pUSLScans = NULL;
	m_pScansItem = NULL;

}

CMultipleScansPage::~CMultipleScansPage()
{
	SAFE_DELETE_ARRAY( m_pUSLScans );
}

void CMultipleScansPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultipleScansPage)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_SCANS_TREE, m_treeScans);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMultipleScansPage, CResizablePage)
	//{{AFX_MSG_MAP(CMultipleScansPage)
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_NOTIFY(NM_RCLICK, IDC_SCANS_TREE, OnRclickScansTree)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultipleScansPage message handlers

BOOL CMultipleScansPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();
	
	m_imageList.Create (16, 16, ILC_COLOR32 , 1,1);
	m_bitmap[0].LoadBitmap(IDB_BITMAP_OPEN_DIRECTORY);	m_imageList.Add(&m_bitmap[0],RGB(0, 0, 0));
	m_bitmap[1].LoadBitmap(IDB_BITMAP_PROFILE);			m_imageList.Add(&m_bitmap[1],RGB(0, 0, 0));
	m_bitmap[2].LoadBitmap(IDB_BITMAP_VARIABLES);		m_imageList.Add(&m_bitmap[2],RGB(0, 0, 0));
	m_bitmap[3].LoadBitmap(IDB_BITMAP_LINES);			m_imageList.Add(&m_bitmap[3],RGB(0, 0, 0));

	m_treeScans.SetImageList (&m_imageList, TVSIL_NORMAL);

	if(theApp.m_LastSettings.fMultipleScansVDividerPos < 0.05f) theApp.m_LastSettings.fMultipleScansVDividerPos = 0.05f;
	
	SendMessage(WM_SIZE);

	FillScansTree();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMultipleScansPage::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	
	if(GetSafeHwnd()) {
		InvalidateResizeableDlgs();
	}
	
}

BOOL CMultipleScansPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastWorkspaceTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CResizablePage::OnSetActive();
}

void CMultipleScansPage::InvalidateResizeableDlgs()
{
	CRect rr,rrDlg;

	GetClientRect(&rr);

	m_rrVBorder = rr;

	int nDivide = (int)((float)rr.Width() * theApp.m_LastSettings.fMultipleScansVDividerPos) + rr.left;
	m_rrVBorder.left = nDivide - 4;
	m_rrVBorder.right = nDivide + 4;

	rrDlg = rr;
	rrDlg.right = m_rrVBorder.left;
	m_treeScans.SetWindowPos( NULL , rrDlg.left, rrDlg.top, rrDlg.Width(), rrDlg.Height(), NULL);

	rrDlg = rr;
	rrDlg.left = m_rrVBorder.right;
	m_staticView.SetWindowPos( NULL , rrDlg.left, rrDlg.top, rrDlg.Width(), rrDlg.Height(), NULL);

}

BOOL CMultipleScansPage::PreTranslateMessage(MSG* pMsg) 
{
	CString Buff;
	CPoint point;
	CRect rr;

	GetCursorPos(&point);
	ScreenToClient(&point);

	switch(pMsg->message) {
	case 0x100:
		switch(pMsg->wParam) {
		case 13:

			return TRUE;
		}
		break;
	case 0x200: // Mouse move
		switch(pMsg->wParam) {
		case 1:
			if((m_bLbDown == true) && (point != m_ptLbDown)) {

				if(m_bBorderSelected == true) {
					GetClientRect(&rr);
					theApp.m_LastSettings.fMultipleScansVDividerPos = (float)point.x * 1.0f / (float)rr.Width();
					InvalidateResizeableDlgs();
				}

				m_ptLbDown = point;
			}
			break;
		};
		break;
	case 0x201://LBUTTONDOWN
		m_ptLbDown = point;
		m_bLbDown = true;
		if(m_rrVBorder.PtInRect(point)) {
			m_bBorderSelected = true;
		}
		break;

	case 0x202:	//LBUTTONUP
		m_bLbDown = false;
		m_bBorderSelected = false;
		break;
	}
	
	return CResizablePage::PreTranslateMessage(pMsg);
}

BOOL CMultipleScansPage::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint	point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	if(m_rrVBorder.PtInRect(point)) {
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		return TRUE;
	}
	
	return CResizablePage::OnSetCursor(pWnd, nHitTest, message);
}

void CMultipleScansPage::OnRclickScansTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;
	int nMenu = IDR_RB_MUTIPLESCANS_TREE_MENU;
/*
	if((nSize = theApp.m_WS.m_nProfileL)>0) {
		hItem = m_treeProfile.GetSelectedItem();
		for(int nn=0;nn<nSize;nn++) {
			if(hItem == m_pProfileItem[nn].hItem) {
				nMenu = IDR_RB_PROFILESELETED_MENU;
				theApp.m_WS.SelectProfile(nn);
			}
			if(hItem == m_pProfileItem[nn].hFileName) {
				nMenu = IDR_RB_PROFILEFILENAME_MENU;
				theApp.m_WS.SelectProfile(nn);
			}
			if(hItem == m_pProfileItem[nn].hLines) {
				nMenu = 0;
				theApp.m_WS.SelectProfile(nn);
			}
		}
	}
	*/
	GetCursorPos(&Point);

	if(nMenu) {
		if (menu->LoadMenu(nMenu)) {

			pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

		};
	}
	delete menu;
	
	*pResult = 0;
}

void CMultipleScansPage::OnFileOpen() 
{
	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;

	CFileDialog FileDlg(TRUE,_T(".USLScans"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Multiple scans (*.USLScans)|*.USLScans|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath[0];
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_PalettePath = FilePath.Left(nPos);
			}
		}

		AddToUSLScans(&FilePath);
		FillScansTree();
	}
	
}

bool CMultipleScansPage::AddToUSLScans(CString *pFilePath)
{
	int ii;
	CUSLScans* pTempUSLScans;

	if(m_pUSLScans == NULL) {
		m_nUSLScansL = 0;
		m_pUSLScans = new CUSLScans[1];
	} else {
		pTempUSLScans = new CUSLScans[m_nUSLScansL];
		for(ii=0;ii<m_nUSLScansL;ii++) {
			pTempUSLScans[ii] = &m_pUSLScans[ii];
		}
		SAFE_DELETE_ARRAY( m_pUSLScans );
		m_pUSLScans = new CUSLScans[m_nUSLScansL+1];
		for(ii=0;ii<m_nUSLScansL;ii++) {
			m_pUSLScans[ii] = &pTempUSLScans[ii];
		}
		SAFE_DELETE_ARRAY( pTempUSLScans );

	};
	m_pUSLScans[m_nUSLScansL].Initialize( pFilePath );
	m_nUSLScansL++;
	return true;
}

void CMultipleScansPage::FillScansTree()
{
	CString Buff;
	int	nn;

	m_treeScans.DeleteAllItems();
	m_treeScans.InsertItem(L"Scans",0,0,TVI_ROOT);

	if(m_nUSLScansL>0) {

		if(m_pScansItem == NULL) {
			m_pScansItem = new ItemScansData [m_nUSLScansL];
		} else {
			m_pScansItem = (ItemScansData *)realloc(m_pScansItem,m_nUSLScansL * sizeof ItemScansData);
		}

		for(nn=0; nn<m_nUSLScansL;nn++) {
			Buff =m_pUSLScans[nn].m_FilePath;
			m_pScansItem[nn].hItem=m_treeScans.InsertItem(Buff,1,1);

//			Buff.Format(L"File Name %s", theApp.m_WS.m_pProfileArray[nn].m_FileName);				m_pProfileItem[nn].hFileName		= m_treeProfile.InsertItem(Buff,2,2,m_pProfileItem[nn].hItem);
//			Buff.Format(L"Lines");																m_pProfileItem[nn].hLines			= m_treeProfile.InsertItem(Buff,2,2,m_pProfileItem[nn].hItem);
//			Buff.Format(L"Taught Lines %d",theApp.m_WS.m_pProfileArray[nn].m_nTaughtLineL);		m_pProfileItem[nn].hTaughtLines		= m_treeProfile.InsertItem(Buff,3,3,m_pProfileItem[nn].hLines);
//			Buff.Format(L"Scan Lines %d",theApp.m_WS.m_pProfileArray[nn].m_nScanLineL);			m_pProfileItem[nn].hScanLines		= m_treeProfile.InsertItem(Buff,3,3,m_pProfileItem[nn].hLines);
//			Buff.Format(L"Brane Lines %d",theApp.m_WS.m_pProfileArray[nn].m_nBrainLineL);			m_pProfileItem[nn].hBraneLines		= m_treeProfile.InsertItem(Buff,3,3,m_pProfileItem[nn].hLines);
//			Buff.Format(L"Envelope Lines %d",theApp.m_WS.m_pProfileArray[nn].m_nEnvelopeLineL);	m_pProfileItem[nn].hEnvelopeLines	= m_treeProfile.InsertItem(Buff,3,3,m_pProfileItem[nn].hLines);
		}
	}


}
