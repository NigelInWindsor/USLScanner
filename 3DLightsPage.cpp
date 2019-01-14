// 3DLightsPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "3DLightsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DLightsPage property page

IMPLEMENT_DYNCREATE(C3DLightsPage, CPropertyPage)

C3DLightsPage::C3DLightsPage(CPropertySheet* pToolSheet, CPropertyPage* pViewPage) 
	: CPropertyPage(C3DLightsPage::IDD)
{
	//{{AFX_DATA_INIT(C3DLightsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nIndex = 0;
	m_pViewPage = pViewPage;
	m_pToolSheet = pToolSheet;
	m_pEditSpinItem = NULL;
	m_strType[0].LoadString(IDS_Off);
	m_strType[1].LoadString(IDS_Point);
	m_strType[2].LoadString(IDS_Spot);
	m_strType[3].LoadString(IDS_Direction);
}

C3DLightsPage::~C3DLightsPage()
{
	SAFE_DELETE( m_pEditSpinItem );

}

void C3DLightsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3DLightsPage)
	DDX_Control(pDX, IDC_SLIDER_AMBIENT, m_sliderAmbient);
	DDX_Control(pDX, IDC_LIST_LIGHTS, m_listLights);
	DDX_Control(pDX, IDC_SPIN_THETA, m_spinTheta);
	DDX_Control(pDX, IDC_SPIN_PHI, m_spinPhi);
	DDX_Control(pDX, IDC_EDIT_THETA, m_editTheta);
	DDX_Control(pDX, IDC_EDIT_PHI, m_editPhi);
	DDX_Control(pDX, IDC_EDIT_LOOKATPT, m_editLookAtPt);
	DDX_Control(pDX, IDC_EDIT_POSITION, m_editPosition);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3DLightsPage, CPropertyPage)
	//{{AFX_MSG_MAP(C3DLightsPage)
	ON_EN_CHANGE(IDC_EDIT_LOOKATPT, OnChangeEditLookatpt)
	ON_EN_CHANGE(IDC_EDIT_POSITION, OnChangeEditPosition)
	ON_EN_CHANGE(IDC_EDIT_THETA, OnChangeEditTheta)
	ON_EN_CHANGE(IDC_EDIT_PHI, OnChangeEditPhi)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_THETA, OnDeltaposSpinTheta)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PHI, OnDeltaposSpinPhi)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LIGHTS, OnClickListLights)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LIGHTS, OnDblclkListLights)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_LIGHTS, OnGetdispinfoListLights)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST_LIGHTS, OnKillfocusListLights)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,ItemChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DLightsPage message handlers

BOOL C3DLightsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CString Buff;

	m_spinTheta.SetRange(0,1800);
	m_spinPhi.SetRange(0,1800);
	m_sliderAmbient.SetRange(0,255);

	
	CreateColumns();
	FillList();
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL C3DLightsPage::OnSetActive() 
{
	theApp.m_LastSettings.nLast3DDrawingToolTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	
	return CPropertyPage::OnSetActive();
}

void C3DLightsPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;

	CString Buff;
	if(m_nIndex<0) m_nIndex=0; 
	if(m_nIndex>7) m_nIndex=7; 
	D3DLIGHT9 *pLight = &theApp.m_LastSettings.light[m_nIndex];

	Buff.Format(L"X:%.0f Y:%.0f Z:%.0f",pLight->Position.x,pLight->Position.y,pLight->Position.z);
	m_editPosition.SetWindowText(Buff);

	Buff.Format(L"X:%.0f Y:%.0f Z:%.0f",theApp.m_LastSettings.vecLightLookAtPt[m_nIndex].x,theApp.m_LastSettings.vecLightLookAtPt[m_nIndex].y,theApp.m_LastSettings.vecLightLookAtPt[m_nIndex].z);
	m_editLookAtPt.SetWindowText(Buff);

	Buff.Format(L"%.01f",pLight->Theta * RAD_TO_DEG);
	m_editTheta.SetWindowText(Buff);

	Buff.Format(L"%.01f",pLight->Phi * RAD_TO_DEG);
	m_editPhi.SetWindowText(Buff);

	m_spinTheta.SetPos((int)(pLight->Theta * RAD_TO_DEG * 10.0f));
	m_spinPhi.SetPos((int)(pLight->Phi * RAD_TO_DEG * 10.0f));

	m_sliderAmbient.SetPos(theApp.m_LastSettings.nLightAmbient);

}


void C3DLightsPage::OnChangeEditLookatpt() 
{
	CString Buff;

	m_editLookAtPt.GetWindowText(Buff);
	swscanf_s(Buff,L"X:%f Y:%f Z:%f",&theApp.m_LastSettings.vecLightLookAtPt[m_nIndex].x,&theApp.m_LastSettings.vecLightLookAtPt[m_nIndex].y,&theApp.m_LastSettings.vecLightLookAtPt[m_nIndex].z);
	RenderParent();
}

void C3DLightsPage::OnChangeEditPosition() 
{
	CString Buff;

	m_editPosition.GetWindowText(Buff);
	swscanf_s(Buff,L"X:%f Y:%f Z:%f",&theApp.m_LastSettings.light[m_nIndex].Position.x,&theApp.m_LastSettings.light[m_nIndex].Position.y,&theApp.m_LastSettings.light[m_nIndex].Position.z);
	RenderParent();
	
}

void C3DLightsPage::OnChangeEditTheta() 
{
	CString Buff;
	float fTemp;
	
	m_editTheta.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	theApp.m_LastSettings.light[m_nIndex].Theta = fTemp * DEG_TO_RAD;
	m_spinTheta.SetPos((int)(theApp.m_LastSettings.light[m_nIndex].Theta * RAD_TO_DEG * 10.0f));
	RenderParent();
}

void C3DLightsPage::OnChangeEditPhi() 
{
	CString Buff;
	float fTemp;
	
	m_editPhi.GetWindowText(Buff);
	_WTOF(Buff,fTemp);

	theApp.m_LastSettings.light[m_nIndex].Phi = fTemp * DEG_TO_RAD;
	m_spinPhi.SetPos((int)(theApp.m_LastSettings.light[m_nIndex].Phi * RAD_TO_DEG * 10.0f));
	RenderParent();

}

void C3DLightsPage::OnDeltaposSpinTheta(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	float fTemp;
	
	m_editTheta.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp += ((float)pNMUpDown->iDelta * 0.1f);
	theApp.m_LastSettings.light[m_nIndex].Theta = fTemp * DEG_TO_RAD;

	Buff.Format(L"%.01f",theApp.m_LastSettings.light[m_nIndex].Theta * RAD_TO_DEG);
	m_editTheta.SetWindowText(Buff);
	RenderParent();
	
	*pResult = 0;
}

void C3DLightsPage::OnDeltaposSpinPhi(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	float fTemp;
	
	m_editPhi.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp += ((float)pNMUpDown->iDelta * 0.1f);
	theApp.m_LastSettings.light[m_nIndex].Phi = fTemp * DEG_TO_RAD;

	Buff.Format(L"%.01f",theApp.m_LastSettings.light[m_nIndex].Phi * RAD_TO_DEG);
	m_editPhi.SetWindowText(Buff);
	RenderParent();
	
	*pResult = 0;
}

void C3DLightsPage::RenderParent()
{
	if(m_pViewPage)	m_pViewPage->Invalidate(false);
}

void C3DLightsPage::OnClickListLights(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SAFE_DELETE( m_pEditSpinItem );

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	m_nIndex=pDispInfo->item.mask;
	UpdateAllControls();

	*pResult = 0;
}

void C3DLightsPage::OnDblclkListLights(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;


	SAFE_DELETE( m_pEditSpinItem );
	m_nIndex=pDispInfo->item.mask;

	m_listLights.GetWindowRect(CtrlRect);
	m_listLights.GetSubItemRect(m_nIndex,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

	switch(pDispInfo->item.iItem) {
	case 0:
		break;
	case 1:	m_pEditSpinItem->Initialize(theApp.m_LastSettings.strLight[m_nIndex]);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	case 2: m_pEditSpinItem->Initialize((int *)&theApp.m_LastSettings.light[m_nIndex].Type,m_strType,4);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
		break;
	}
	
	*pResult = 0;
}

void C3DLightsPage::OnGetdispinfoListLights(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	int nn = pDispInfo->item.iItem;

	int nType = (int)theApp.m_LastSettings.light[nn].Type;
	if(nType<0) nType =0;
	if(nType>3) nType = 3;
	theApp.m_LastSettings.light[nn].Type = (D3DLIGHTTYPE)nType;

	switch(pDispInfo->item.iSubItem) {
	case 0:	swprintf_s(str,100,_T("%d"),nn + 1);
		break;
	case 1:	swprintf_s(str,100,_T("%s"),theApp.m_LastSettings.strLight[nn]);
		break;
	case 2:	swprintf_s(str,100,_T("%s"),(LPCWSTR)m_strType[theApp.m_LastSettings.light[nn].Type]);
		break;
	case 3:
		break;
	case 4:
		break;
	}
	
	pDispInfo->item.pszText=str;
	
	
	*pResult = 0;
}

void C3DLightsPage::CreateColumns()
{
	int ColumnWidth[10] = { 20,140,100,100,100,50,50,50,80,50};
	CString ColumnName[10];
	ColumnName[0]= "#";
	ColumnName[1].LoadString(IDS_Name);
	ColumnName[2].LoadString(IDS_Type);

	int nColumnCount = m_listLights.GetHeaderCtrl()->GetItemCount();
	for (int ii=0;ii < nColumnCount;ii++) {
		m_listLights.DeleteColumn(0);
	}
	for (int ii=0;ii<3;ii++) {
		m_listLights.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	}
	m_listLights.SetExtendedStyle( LVS_EX_FULLROWSELECT  );

}

void C3DLightsPage::FillList()
{
	CString Buff;

	m_listLights.DeleteAllItems();
	for (int ii=0;ii<8;ii++) {
		Buff.Format(_T("%d"),ii+1);
		m_listLights.InsertItem(ii, Buff.GetBuffer(255), ii);
	};

	m_listLights.EnsureVisible(m_nIndex,FALSE);
	for(int ii=0;ii<8;ii++) 
		m_listLights.SetItemState(ii , 0,LVIS_SELECTED);
	m_listLights.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
	m_listLights.SetFocus();

}

void C3DLightsPage::OnKillfocusListLights(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CRect	Rect;
	CRect	CtrlRect;

	m_listLights.GetSubItemRect(m_nIndex,0,LVIR_BOUNDS,Rect);

	CDC* pDC = m_listLights.GetDC();

	CPen pen(PS_SOLID,1,RGB(0,0,255));
	CPen *pOldPen = pDC->SelectObject(&pen);

	Rect.left += 2;
	Rect.right -= 2;
	Rect.bottom -= 1;
	pDC->MoveTo(Rect.TopLeft());
	pDC->LineTo(Rect.right,Rect.top);
	pDC->LineTo(Rect.BottomRight());
	pDC->LineTo(Rect.left,Rect.bottom);
	pDC->LineTo(Rect.TopLeft());

	pDC->SelectObject(pOldPen);
	m_listLights.ReleaseDC(pDC);
	*pResult = 0;
}

void C3DLightsPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect	Rect;
	CRect	CtrlRect;
	m_listLights.GetWindowRect(CtrlRect);
	m_listLights.GetSubItemRect(m_nIndex,0,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	CDC* pDC = m_listLights.GetDC();

	CPen pen(PS_SOLID,1,RGB(0,0,255));
	CPen *pOldPen = pDC->SelectObject(&pen);

	pDC->Rectangle(Rect);
	pDC->SelectObject(pOldPen);
	m_listLights.ReleaseDC(pDC);
}

void C3DLightsPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int	nTemp;

	nTemp=pSlider->GetPos()&0xffff;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SLIDER_AMBIENT:
		if(theApp.m_LastSettings.nLightAmbient - nTemp) {
			theApp.m_LastSettings.nLightAmbient = nTemp;
			RenderParent();
		}
		break;
	}
	
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}

HRESULT C3DLightsPage::ItemChanged(WPARAM,LPARAM)
{
	RenderParent();
	return NULL;
}
