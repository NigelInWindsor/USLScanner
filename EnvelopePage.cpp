// EnvelopePage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "EnvelopePage.h"
#include "ViewSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEnvelopePage property page

IMPLEMENT_DYNCREATE(CEnvelopePage, CPropertyPage)

CEnvelopePage::CEnvelopePage(CWnd* pViewSheet, CData* pData) : CPropertyPage(CEnvelopePage::IDD)
{
	//{{AFX_DATA_INIT(CEnvelopePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pViewSheet = pViewSheet;
	m_pData = pData;
	
	m_nIndex = 0;
	m_pEditSpinItem = NULL;
}

CEnvelopePage::~CEnvelopePage()
{
}

void CEnvelopePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnvelopePage)
	DDX_Control(pDX, IDC_LIST_ENVELOPE, m_listEnvelope);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEnvelopePage, CPropertyPage)
	//{{AFX_MSG_MAP(CEnvelopePage)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_ENVELOPE, OnGetdispinfoListEnvelope)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ENVELOPE, OnClickListEnvelope)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ENVELOPE, OnDblclkListEnvelope)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ENVELOPE, OnRclickListEnvelope)
	ON_COMMAND(ID_ENVELOPE_SETALLTO1COLOUR, OnEnvelopeSetallto1colour)
	ON_COMMAND(ID_ENVELOPE_INIDIVIDUALCOLOUR, OnEnvelopeInidividualcolour)
	ON_COMMAND(ID_ENVELOPE_IMPORTALLENVELOPESFROMPROFILE, OnEnvelopeImportallenvelopesfromprofile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnvelopePage message handlers

BOOL CEnvelopePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	

	CreateColumns();
	FillList();
	
	UpdateAllControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEnvelopePage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

}

BOOL CEnvelopePage::OnSetActive() 
{
	theApp.m_LastSettings.nLastViewOptionsTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

void CEnvelopePage::OnGetdispinfoListEnvelope(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	int nn;
	CString strOff,strOn;
	strOff.LoadString(IDS_Off);
	strOn.LoadString(IDS_On);

	nn = pDispInfo->item.iItem;

	switch(pDispInfo->item.iSubItem) {
	case 0:
		break;
	case 1:
		switch(m_pData->m_pEnvelopeLine[nn].m_nOffOn) {
		case 0:	swprintf_s(str,_T("%s"),strOff.GetBuffer());
			break;
		case 1:	swprintf_s(str,_T("%s"),strOn.GetBuffer());
			break;
		}
		break;
	case 2:
		swprintf_s(str,L"%d:%d:%d",
			MulDiv((m_pData->m_pEnvelopeLine[nn].m_rgb&0x0000ff),100,255),
			MulDiv((m_pData->m_pEnvelopeLine[nn].m_rgb&0x00ff00)>>8,100,255),
			MulDiv((m_pData->m_pEnvelopeLine[nn].m_rgb&0xff0000)>>16,100,255));
		break;
	case 3:	str[0]=0;
		if(wcslen(m_pData->m_pEnvelopeLine[nn].m_wStr) < 99) {
			swprintf_s(str,L"%s",m_pData->m_pEnvelopeLine[nn].m_wStr);
		} else {
			swprintf_s(str,L"%-20.20s...",m_pData->m_pEnvelopeLine[nn].m_wStr);
		}
		break;
	}
	
	pDispInfo->item.pszText=str;
	*pResult = 0;
}

void CEnvelopePage::OnClickListEnvelope(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SAFE_DELETE( m_pEditSpinItem );
	
	*pResult = 0;
}

void CEnvelopePage::OnDblclkListEnvelope(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;

	if(m_pData->m_nEnvelopeLineL<=0) return;
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;

	SAFE_DELETE( m_pEditSpinItem );
	m_nIndex=pDispInfo->item.mask;


	POSITION pos = m_listEnvelope.GetFirstSelectedItemPosition();
	m_nStartIndex=(int)pos;
	m_nFinishIndex=m_nStartIndex=m_listEnvelope.GetNextSelectedItem(pos);
	while(pos) {
		m_nFinishIndex=m_listEnvelope.GetNextSelectedItem(pos);
	}


	if((m_nIndex>=0) && (m_nIndex<m_pData->m_nEnvelopeLineL)) {
		m_listEnvelope.GetWindowRect(CtrlRect);
		m_listEnvelope.GetSubItemRect(m_nIndex,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
		Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
		Rect.InflateRect(0,1,0,2);

		m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

		switch(pDispInfo->item.iItem) {
		case 0:
			break;
		case 1:
			for(m_nIndex=m_nStartIndex; m_nIndex<=m_nFinishIndex; m_nIndex++) {
				m_pData->m_pEnvelopeLine[m_nIndex].m_nOffOn ^= 1;
			};
			Invalidate(false);
			pSheet->InvalidateChild();
			break;
		case 2: OnEnvelopeInidividualcolour();
			break;
		case 3:	m_pEditSpinItem->Initialize(m_pData->m_pEnvelopeLine[m_nIndex].m_wStr);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		}
	}	
	*pResult = 0;
}

void CEnvelopePage::OnRclickListEnvelope(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;


	POSITION pos = m_listEnvelope.GetFirstSelectedItemPosition();
	m_nIndex =	m_nStartIndex=(int)pos;
	m_nFinishIndex=m_nStartIndex=m_listEnvelope.GetNextSelectedItem(pos);
	while(pos) {
		m_nFinishIndex=m_listEnvelope.GetNextSelectedItem(pos);
	}

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_ENVELOPE_MENU)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
	*pResult = 0;
}

void CEnvelopePage::CreateColumns()
{
	CRect rr;
	int ii;
	int ColumnWidth[10] = { 22,90,90,110};
	CString ColumnName[10];
	ColumnName[0]= "#";
	ColumnName[1].LoadString(IDS_On_Off);
	ColumnName[2].Format(L"R G B");
	ColumnName[3].LoadString(IDS_Label);

	int nTotal = ColumnWidth[0];
	m_listEnvelope.GetWindowRect(&rr);
	if(m_pData->m_nEnvelopeLineL > 9) rr.right-=22;
	nTotal += (ColumnWidth[1] = rr.Width()/4);
	nTotal += (ColumnWidth[2] = (rr.Width()*1)/4);
	ColumnWidth[3] = (rr.Width() - nTotal - 4);

	for (ii=0;ii<4;ii++) {
		m_listEnvelope.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	}
	m_listEnvelope.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CEnvelopePage::FillList()
{
	CString Buff;
	int nSize = m_pData->m_nEnvelopeLineL;

	m_listEnvelope.DeleteAllItems();
	for (int ii=0;ii<nSize;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listEnvelope.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listEnvelope.EnsureVisible(m_nIndex,FALSE);
	for(int ii=0;ii<nSize;ii++) 
		m_listEnvelope.SetItemState(ii , 0,LVIS_SELECTED);
	m_listEnvelope.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listEnvelope.SetFocus();
}

void CEnvelopePage::OnEnvelopeSetallto1colour() 
{
	
}

void CEnvelopePage::OnEnvelopeInidividualcolour() 
{
	CMFCColorDialog dlg;
	CViewSheet* pSheet = (CViewSheet*)m_pViewSheet;

	if(dlg.DoModal()==IDOK) {
		for(m_nIndex=m_nStartIndex;m_nIndex<=m_nFinishIndex;m_nIndex++) {
			m_pData->m_pEnvelopeLine[m_nIndex].m_rgb = dlg.GetColor();
		}
		Invalidate(FALSE);
		pSheet->InvalidateChild();
	};
}

void CEnvelopePage::OnEnvelopeImportallenvelopesfromprofile() 
{
	m_pData->ImportAllEnvelopesFromProfile();
	FillList();
	Invalidate(false);
	
}
