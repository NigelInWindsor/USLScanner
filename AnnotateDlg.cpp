// AnnotateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "AnnotateDlg.h"
#include "ViewPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnnotateDlg dialog


CAnnotateDlg::CAnnotateDlg(CWnd* pParent /*=NULL*/,CData* pData,float fXp,float fYp,CPoint* pt,int nEditWhich,int nImageType,int nImageNumber)
	: CDialog(CAnnotateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAnnotateDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pData = pData;
	m_fXp = fXp;
	m_fYp = fYp;
	m_pt = *pt;
	m_pParent = pParent;
	m_nEditWhich = nEditWhich;
	
	m_nIndex=0;
	m_pEditSpinItem=NULL;
	m_nImageType = nImageType;
	m_nImageNumber = nImageNumber;
}


void CAnnotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnnotateDlg)
	DDX_Control(pDX, IDC_CHECK_THIS_IMAGE, m_checkThisImage);
	DDX_Control(pDX, IDC_CHECK_ALL_IMAGES, m_checkAllImages);
	DDX_Control(pDX, IDC_LIST_REASONS, m_listReasons);
	DDX_Control(pDX, IDC_EDIT_ID, m_editID);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_BK_COLOR, m_staticBkColor);
	DDX_Control(pDX, IDC_COMBO_BACKGROUND_MODE, m_comboBkMode);
}


BEGIN_MESSAGE_MAP(CAnnotateDlg, CDialog)
	//{{AFX_MSG_MAP(CAnnotateDlg)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_REASONS, OnGetdispinfoListReasons)
	ON_NOTIFY(NM_CLICK, IDC_LIST_REASONS, OnClickListReasons)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_REASONS, OnDblclkListReasons)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_REASONS, OnRclickListReasons)
	ON_COMMAND(ID_EDIT_ADD, OnEditAdd)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_BN_CLICKED(IDC_CHECK_THIS_IMAGE, OnCheckThisImage)
	ON_BN_CLICKED(IDC_CHECK_ALL_IMAGES, OnCheckAllImages)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_ID, OnChangeEditId)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,ItemChanged)
	ON_CBN_SELCHANGE(IDC_COMBO_BACKGROUND_MODE, &CAnnotateDlg::OnCbnSelchangeComboBackgroundMode)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnnotateDlg message handlers
BOOL CAnnotateDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CAnnotateDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CAnnotateDlg::OnDestroy() 
{
	GetWindowRect(theApp.m_LastSettings.RectWindowPos[CAnnotateDlg::IDD & MAXIMUMWINDOWSIDNUMBER]);

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CAnnotateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CPoint point;
	CString Buff;

	GetCursorPos(&point);

	CRect rect = theApp.m_LastSettings.RectWindowPos[CAnnotateDlg::IDD & MAXIMUMWINDOWSIDNUMBER];
	if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
		theApp.ConfineToScreen(&rect);
		SetWindowPos( &wndTop , point.x, point.y, rect.Width(), rect.Height(), SWP_NOSIZE);
	}

	Buff.LoadString(IDS_Transparent);	 m_comboBkMode.AddString(Buff);
	Buff.LoadString(IDS_Opaque);		 m_comboBkMode.AddString(Buff);


	if(m_nEditWhich>=0) {
		m_editID.SetWindowText(m_pData->m_pAnnotation[m_nEditWhich].Text);
		theApp.m_LastSettings.lfAnnotation = m_pData->m_pAnnotation[m_nEditWhich].lf;
		theApp.m_LastSettings.rgbAnnotation = m_pData->m_pAnnotation[m_nEditWhich].rgbTextColor;
		theApp.m_LastSettings.nAnnotateBkMode = m_pData->m_pAnnotation[m_nEditWhich].nBkMode;
		theApp.m_LastSettings.rgbAnnotateBkColor = m_pData->m_pAnnotation[m_nEditWhich].rgbBkColor;
	}

	switch (theApp.m_LastSettings.nAnnotateBkMode) {
	default: m_comboBkMode.SetCurSel(0);
		break;
	case OPAQUE: m_comboBkMode.SetCurSel(1);
		break;
	}

	
	UpdateAllControls();

	CreateColumns();
	FillList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAnnotateDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	if(	theApp.m_LastSettings.nAnnotateApplyToAllImages == 1)  {
		m_checkAllImages.SetCheck(true);
		m_checkThisImage.SetCheck(false);
	} else {
		m_checkAllImages.SetCheck(false);
		m_checkThisImage.SetCheck(true);
	}

}

void CAnnotateDlg::BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor)
{
	CRect rr;

	mStatic->GetWindowRect(&rr);
	ScreenToClient(rr);

	CBrush	Brush(rgbColor);

	pDC->FillRect(rr, &Brush);

}

void CAnnotateDlg::OnOK() 
{
	CString ID;
	int	nSentence = 0;

	m_editID.GetWindowText(ID);
	if(ID.GetLength() >= 255) ID.GetBufferSetLength(255);
	if(m_nEditWhich>=0) {
		swprintf_s(m_pData->m_pAnnotation[m_nEditWhich].Text,256,_T("%s"),LPCTSTR(ID));
		m_pData->m_pAnnotation[m_nEditWhich].lf = theApp.m_LastSettings.lfAnnotation;
		m_pData->m_pAnnotation[m_nEditWhich].rgbTextColor = theApp.m_LastSettings.rgbAnnotation;
		m_pData->m_pAnnotation[m_nEditWhich].nApplyToAllImage = theApp.m_LastSettings.nAnnotateApplyToAllImages;
		m_pData->m_pAnnotation[m_nEditWhich].nImageNumber = m_nImageNumber;
		m_pData->m_pAnnotation[m_nEditWhich].nImageType = m_nImageType;
		m_pData->m_pAnnotation[m_nEditWhich].nBkMode = theApp.m_LastSettings.nAnnotateBkMode;
		m_pData->m_pAnnotation[m_nEditWhich].rgbBkColor = theApp.m_LastSettings.rgbAnnotateBkColor;
	} else {
		m_pData->AddAnnotation(m_fXp,m_fYp,ID,theApp.m_LastSettings.lfAnnotation,theApp.m_LastSettings.rgbAnnotation,m_nImageType,m_nImageNumber,theApp.m_LastSettings.nAnnotateApplyToAllImages, theApp.m_LastSettings.nAnnotateBkMode, theApp.m_LastSettings.rgbAnnotateBkColor);
	}
	CDialog::OnOK();
	InvalidateParent();

}


void CAnnotateDlg::OnCancel() 
{
	CDialog::OnCancel();
}


void CAnnotateDlg::OnButtonFont() 
{
	LOGFONT lf = theApp.m_LastSettings.lfAnnotation;

	if(m_nEditWhich>=0) {
		lf=m_pData->m_pAnnotation[m_nEditWhich].lf;
	}

	CFontDialog dlg(&lf);
	dlg.m_cf.rgbColors = theApp.m_LastSettings.rgbAnnotation;
	if(dlg.DoModal() == IDOK) {
		dlg.GetCurrentFont(&theApp.m_LastSettings.lfAnnotation);
		theApp.m_LastSettings.rgbAnnotation = dlg.m_cf.rgbColors;
		if(m_nEditWhich>=0) {
			m_pData->m_pAnnotation[m_nEditWhich].lf = theApp.m_LastSettings.lfAnnotation;
			m_pData->m_pAnnotation[m_nEditWhich].rgbTextColor = theApp.m_LastSettings.rgbAnnotation;
		};
		InvalidateParent();
	}
}

void CAnnotateDlg::OnGetdispinfoListReasons(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];

	if(theApp.m_AnnotateReasons.GetSize()<=0) return;

	switch(pDispInfo->item.iSubItem) {
	case 0:
		break;
	case 1:
		swprintf_s(str,100,_T("%s"),(LPCWSTR)theApp.m_AnnotateReasons.GetAt(pDispInfo->item.iItem));
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	*pResult = 0;
}

void CAnnotateDlg::OnClickListReasons(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	ClearEditSpin();
	m_nIndex = pDispInfo->item.mask;
	if((m_nIndex<theApp.m_AnnotateReasons.GetSize()) && (m_nIndex>=0)) {
		m_editID.SetWindowText(theApp.m_AnnotateReasons.GetAt(m_nIndex));
	}
	
	*pResult = 0;
}

void CAnnotateDlg::OnDblclkListReasons(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	ClearEditSpin();

	m_nIndex=pDispInfo->item.mask;
	if((m_nIndex<theApp.m_AnnotateReasons.GetSize()) && (m_nIndex>=0)) {

		m_listReasons.GetWindowRect(CtrlRect);
		m_listReasons.GetSubItemRect(m_nIndex,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
		Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
		Rect.InflateRect(0,1,0,2);

		m_pEditSpinItem = new CEditSpinItem(this,(CWnd**)&m_pEditSpinItem,&Rect);

		switch(pDispInfo->item.iItem) {
		case 0:
			break;
		case 1:	m_pEditSpinItem->Initialize(&m_Buffer);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
			break;
		}
	}	
	
	*pResult = 0;
}

void CAnnotateDlg::CreateColumns()
{
	int ColumnWidth[10] = { 0,170,100,100,100,50,50,50,80,50};
	CString ColumnName[10];
	ColumnName[0]= "#";
	ColumnName[1].LoadString(IDS_Reason);

	CRect rr;
	m_listReasons.GetWindowRect(rr);
	ColumnWidth[1]=rr.Width()-2;

	int nColumnCount = m_listReasons.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listReasons.DeleteColumn(0);
	}
	for (int ii=0;ii<2;ii++) {
		m_listReasons.InsertColumn(ii,ColumnName[ii], LVCFMT_LEFT,ColumnWidth[ii]);
	}
	m_listReasons.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CAnnotateDlg::FillList()
{
	CString Buff;

	m_listReasons.DeleteAllItems();
	for (int ii=0;ii<theApp.m_AnnotateReasons.GetSize();ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listReasons.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listReasons.EnsureVisible(m_nIndex,FALSE);
	for(int ii=0;ii<theApp.m_AnnotateReasons.GetSize();ii++) 
		m_listReasons.SetItemState(ii , 0,LVIS_SELECTED);
	m_listReasons.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listReasons.SetFocus();


}

void CAnnotateDlg::OnRclickListReasons(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;

	POSITION pos = m_listReasons.GetFirstSelectedItemPosition();
	m_nIndex=m_listReasons.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_EDIT_DESCRIPTION_MENU)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
	
	
	*pResult = 0;
}

void CAnnotateDlg::ClearEditSpin()
{
	if(m_pEditSpinItem!=NULL) {
		delete m_pEditSpinItem;
		m_pEditSpinItem=NULL;
	}

}


void CAnnotateDlg::OnEditAdd() 
{
	CString Buff;
	CRect	Rect;
	CRect	CtrlRect;

	Buff.Empty();
	m_nIndex=theApp.m_AnnotateReasons.Add(Buff);
	FillList();
/*
	m_listReasons.GetWindowRect(CtrlRect);
	m_listReasons.GetSubItemRect(m_nIndex,0,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	ClearEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

	m_Buffer.Empty();
	m_pEditSpinItem->Initialize(&m_Buffer);
	m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
*/
}

HRESULT CAnnotateDlg::ItemChanged(WPARAM,LPARAM)
{
	theApp.m_AnnotateReasons.SetAt(m_nIndex,m_Buffer);
	m_editID.SetWindowText(m_Buffer);
	return NULL;
}

void CAnnotateDlg::OnEditDelete() 
{
	if((m_nIndex>=0) && (m_nIndex<theApp.m_AnnotateReasons.GetSize())) {
		theApp.m_AnnotateReasons.RemoveAt(m_nIndex,1);
		FillList();	
	}
}

void CAnnotateDlg::OnCheckThisImage() 
{
	theApp.m_LastSettings.nAnnotateApplyToAllImages = 0;
	UpdateAllControls();
}

void CAnnotateDlg::OnCheckAllImages() 
{
	theApp.m_LastSettings.nAnnotateApplyToAllImages = 1;
	UpdateAllControls();
}



void CAnnotateDlg::InvalidateParent()
{
	CViewPage *pView = 	(CViewPage*)m_pParent;

	pView->m_bRenderView=TRUE;
	pView->Invalidate(false);

}

void CAnnotateDlg::OnChangeEditId() 
{
	CString ID;

	m_editID.GetWindowText(ID);
	if(ID.GetLength() >= 255) ID.GetBufferSetLength(255);

	if(m_nEditWhich>=0) {
		swprintf_s(m_pData->m_pAnnotation[m_nEditWhich].Text,256,_T("%s"),LPCTSTR(ID));
		m_pData->m_pAnnotation[m_nEditWhich].lf = theApp.m_LastSettings.lfAnnotation;
		m_pData->m_pAnnotation[m_nEditWhich].rgbTextColor = theApp.m_LastSettings.rgbAnnotation;
		m_pData->m_pAnnotation[m_nEditWhich].nApplyToAllImage = theApp.m_LastSettings.nAnnotateApplyToAllImages;
		m_pData->m_pAnnotation[m_nEditWhich].nImageNumber = m_nImageNumber;
		m_pData->m_pAnnotation[m_nEditWhich].nImageType = m_nImageType;
		m_pData->m_pAnnotation[m_nEditWhich].nBkMode = theApp.m_LastSettings.nAnnotateBkMode;
		m_pData->m_pAnnotation[m_nEditWhich].rgbBkColor = theApp.m_LastSettings.rgbAnnotateBkColor;
	}
	else {
		m_nEditWhich = m_pData->AddAnnotation(m_fXp, m_fYp, ID, theApp.m_LastSettings.lfAnnotation, theApp.m_LastSettings.rgbAnnotation, m_nImageType, m_nImageNumber, theApp.m_LastSettings.nAnnotateApplyToAllImages, theApp.m_LastSettings.nAnnotateBkMode, theApp.m_LastSettings.rgbAnnotateBkColor);
	}
	InvalidateParent();
	
}


void CAnnotateDlg::OnCbnSelchangeComboBackgroundMode()
{
	switch (m_comboBkMode.GetCurSel()) {
	case 0:	theApp.m_LastSettings.nAnnotateBkMode = TRANSPARENT;
		break;
	case 1:	theApp.m_LastSettings.nAnnotateBkMode = OPAQUE;
		break;
	}
	if (m_nEditWhich >= 0) {
		m_pData->m_pAnnotation[m_nEditWhich].nBkMode = theApp.m_LastSettings.nAnnotateBkMode;
	};
	InvalidateParent();
}


void CAnnotateDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rr;
	CMFCColorDialog dlg;

	m_staticBkColor.GetWindowRect(&rr);
	ScreenToClient(rr);
	if (rr.PtInRect(point)) {
		if (dlg.DoModal() == IDOK) {
			theApp.m_LastSettings.rgbAnnotateBkColor = dlg.GetColor();
			if (m_nEditWhich >= 0) {
				m_pData->m_pAnnotation[m_nEditWhich].rgbBkColor = theApp.m_LastSettings.rgbAnnotateBkColor;
			};
			Invalidate(FALSE);
			InvalidateParent();
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}


void CAnnotateDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (m_nEditWhich >= 0) {
		theApp.m_LastSettings.rgbAnnotateBkColor = m_pData->m_pAnnotation[m_nEditWhich].rgbBkColor;
	};
	BlockFill(&dc, &m_staticBkColor, theApp.m_LastSettings.rgbAnnotateBkColor);

}
