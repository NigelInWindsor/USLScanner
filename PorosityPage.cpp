// PorosityPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "PorosityPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPorosityPage property page

IMPLEMENT_DYNCREATE(CPorosityPage, CResizablePage)

CPorosityPage::CPorosityPage() : CResizablePage(CPorosityPage::IDD)
{
	//{{AFX_DATA_INIT(CPorosityPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nIndex = 0;
	m_pEditSpinItem = NULL;
	m_bLBDown = false;
	m_fBorderPos = 0.5f;
	m_bMoveBorder = false;
	m_bMoveThreshold = false;

}

CPorosityPage::~CPorosityPage()
{
}

void CPorosityPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPorosityPage)
	DDX_Control(pDX, IDC_EDIT_STANDARD_ID, m_editStandardID);
	DDX_Control(pDX, IDC_EDIT_REF_TARGET, m_editRefTarget);
	DDX_Control(pDX, IDC_EDIT_REF_ATTENUATION, m_editRefAttenuation);
	DDX_Control(pDX, IDC_EDIT_SQUIRTER_DIAMETER, m_editSquirterDiameter);
	DDX_Control(pDX, IDC_EDIT_PROBE_DIAMETER, m_editProbeDiameter);
	DDX_Control(pDX, IDC_EDIT_FREQUENCY, m_editFrequency);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_LIST_POINTS, m_listPts);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPorosityPage, CResizablePage)
	//{{AFX_MSG_MAP(CPorosityPage)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_POINTS, OnDblclkListPoints)
	ON_NOTIFY(NM_CLICK, IDC_LIST_POINTS, OnClickListPoints)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_POINTS, OnGetdispinfoListPoints)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_COMMAND(ID_POROSITY_DELETE, OnPorosityDelete)
	ON_COMMAND(ID_POROSITY_SORT, OnPorositySort)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_POINTS, OnRclickListPoints)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_POROSITY_CURVE1_COLOUR, OnPorosityCurve1Colour)
	ON_COMMAND(ID_POROSITY_CURVE2_COLOUR, OnPorosityCurve2Colour)
	ON_COMMAND(ID_POROSITY_CURVE3_COLOUR, OnPorosityCurve3Colour)
	ON_COMMAND(ID_POROSITY_CURVE4_COLOUR, OnPorosityCurve4Colour)
	ON_COMMAND(ID_POROSITY_CURVE5_COLOUR, OnPorosityCurve5Colour)
	ON_EN_CHANGE(IDC_EDIT_FREQUENCY, OnChangeEditFrequency)
	ON_EN_CHANGE(IDC_EDIT_PROBE_DIAMETER, OnChangeEditProbeDiameter)
	ON_EN_CHANGE(IDC_EDIT_SQUIRTER_DIAMETER, OnChangeEditSquirterDiameter)
	ON_COMMAND(ID_POROSITY_DELETEALL, OnPorosityDeleteall)
	ON_WM_SETCURSOR()
	ON_EN_CHANGE(IDC_EDIT_REF_TARGET, OnChangeEditRefTarget)
	ON_EN_CHANGE(IDC_EDIT_REF_ATTENUATION, OnChangeEditRefAttenuation)
	ON_EN_CHANGE(IDC_EDIT_STANDARD_ID, OnChangeEditStandardId)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UI_ITEMCHANGED,PorosityItemChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPorosityPage message handlers
int CPorosityPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CResizablePage::OnCreate(lpCreateStruct) == -1)
		return -1;


	
	return 0;
}

BOOL CPorosityPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();

	GetClientRect(m_rrClient);

	
	UpdateAllControls();

	CreateColumns();
	FillList();
	
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CPorosityPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}



void CPorosityPage::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	CString Buff;

	Buff.Format(_T("%.02fMHz"),theApp.m_Palette.m_fPorosityFrequency);
	m_editFrequency.SetWindowText(Buff);
	Buff.Format(_T("%.01fmm"),theApp.m_Palette.m_fPorosityProbeDiameter);
	m_editProbeDiameter.SetWindowText(Buff);
	Buff.Format(_T("%.01fmm"),theApp.m_Palette.m_fPorositySquirterDiameter);
	m_editSquirterDiameter.SetWindowText(Buff);


	Buff.Format(L"%.01f dB",theApp.m_Palette.m_fPorosityRefAttenuation);
	m_editRefAttenuation.SetWindowText(Buff);
	m_editRefTarget.SetWindowText(theApp.m_Palette.m_PorosityRefTarget);
	m_editStandardID.SetWindowText(theApp.m_Palette.m_PorosityStandardID);

	FillList();
}

BOOL CPorosityPage::OnSetActive() 
{
	theApp.m_LastSettings.nLastPaletteTab = CPorosityPage::IDD;
	
	return CResizablePage::OnSetActive();
}


void CPorosityPage::CreateColumns()
{
	int ColumnWidth[10] = { 0,100,60,80,80,80,80,80,80,80};
	CString ColumnName[10];
	ColumnName[0]= "#";
	ColumnName[1].LoadString(IDS_Plies);
	ColumnName[2].LoadString(IDS_Porosity_Percentage);
	ColumnName[3].LoadString(IDS_Mean_dB);
	ColumnName[4].LoadString(IDS_Attenuation_dB);
	int ii;

	int nColumnCount = (&m_listPts.GetHeaderCtrl())->GetItemCount();
	for (ii=0;ii < nColumnCount;ii++) {
		m_listPts.DeleteColumn(0);
	}

	switch(theApp.m_LastSettings.nPorosityGraphType) {
	case 0:
		for (ii=0;ii<6;ii++) {
			m_listPts.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
		}
		break;
	case 1:
		ColumnName[1].LoadString(IDS_No_Plies);
		ColumnName[2].LoadString(IDS_Attenuation);
		ColumnName[3].LoadString(IDS_Atten_with_respect_to_Ref);
		for (ii=0;ii<4;ii++) {
			m_listPts.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
		}
		break;
	}
	m_listPts.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void CPorosityPage::FillList()
{
	int	ii;
	CString Buff;

	m_listPts.DeleteAllItems();

	switch(theApp.m_LastSettings.nPorosityGraphType) {
	case 0:
		for(ii=0;ii<theApp.m_Palette.m_nPorosityL;ii++) {
			Buff.Format(_T("%d"),ii+1);
			m_listPts.InsertItem(ii, Buff.GetBuffer(255), ii);
		};

		m_listPts.EnsureVisible(m_nIndex,FALSE);
		for(ii=0;ii<theApp.m_Palette.m_nPorosityL;ii++) 
			m_listPts.SetItemState(ii , 0,LVIS_SELECTED);
		m_listPts.SetItemState(m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
		m_listPts.SetFocus();
		break;
	case 1:
		theApp.m_Palette.ProcessPorosityData();
		theApp.m_Palette.PorosityGetMinMaxPlies(&m_nMinPlies,&m_nMaxPlies);
		for(ii=m_nMinPlies;ii<=m_nMaxPlies;ii++) {
			Buff.Format(_T("%d"),ii+1);
			m_listPts.InsertItem(ii, Buff.GetBuffer(255), ii);
		};

		m_listPts.EnsureVisible(m_nIndex,FALSE);
		for(ii=m_nMinPlies;ii<m_nMaxPlies;ii++) 
			m_listPts.SetItemState(ii , 0,LVIS_SELECTED);
		m_listPts.SetItemState(0 , LVIS_SELECTED,LVIS_SELECTED);
		m_listPts.SetFocus();
		break;
	}

}



void CPorosityPage::OnDblclkListPoints(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;


	m_nIndex=pDispInfo->item.mask;
	if((m_nIndex >= theApp.m_Palette.m_nPorosityL) || (m_nIndex == -1)) {
		m_nIndex = theApp.m_Palette.AddEmptyPorosityEntry();
		FillList();
	}

	m_listPts.GetWindowRect(CtrlRect);
	m_listPts.GetSubItemRect(m_nIndex,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	ClearEditSpin();
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**) &m_pEditSpinItem,&Rect);

	switch(pDispInfo->item.iItem) {
	case 0:
		break;
	case 1:	m_pEditSpinItem->Initialize(&theApp.m_Palette.m_pPorosity[m_nIndex].nPlys,0,100,1);
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	case 2:	m_pEditSpinItem->Initialize(&theApp.m_Palette.m_pPorosity[m_nIndex].fPorosity,0.0f,50.0f,0.1f,_T("%.01f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	case 3:	m_pEditSpinItem->Initialize(&theApp.m_Palette.m_pPorosity[m_nIndex].fMeandBAttenuation,-100,100.0f,0.1f,_T("%.02f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	case 4:	m_pEditSpinItem->Initialize(&theApp.m_Palette.m_pPorosity[m_nIndex].fAttenuation,0.0f,100.0f,0.1f,_T("%.02f"));
		m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);	
		break;
	}

	*pResult = 0;
}

void CPorosityPage::OnClickListPoints(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	ClearEditSpin();
	
	m_nIndex=pDispInfo->item.mask;
	Invalidate(FALSE);
	*pResult = 0;
}

void CPorosityPage::OnGetdispinfoListPoints(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];
	ZeroMemory(str,sizeof str);
	pDispInfo->item.pszText=str;
	CCoord Cp,CpRef;
	float fPlies,fTemp;

	if(theApp.m_Palette.m_nPorosityL<=0) return;

	switch(theApp.m_LastSettings.nPorosityGraphType) {
	case 0:
		if(pDispInfo->item.iItem>=theApp.m_Palette.m_nPorosityL) return;

		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:		swprintf_s(str,_T("%d"),theApp.m_Palette.m_pPorosity[pDispInfo->item.iItem].nPlys);
			break;
		case 2:		swprintf_s(str,_T("%.01f"),theApp.m_Palette.m_pPorosity[pDispInfo->item.iItem].fPorosity);
			break;
		case 3:		swprintf_s(str,_T("%.01f"),theApp.m_Palette.m_pPorosity[pDispInfo->item.iItem].fMeandBAttenuation);
			break;
		case 4:		swprintf_s(str,_T("%.01f"),theApp.m_Palette.m_pPorosity[pDispInfo->item.iItem].fAttenuation);
			break;
		}
		break;
	case 1:
		fPlies = (float)(pDispInfo->item.iItem + m_nMinPlies);
		
		switch(pDispInfo->item.iSubItem) {
		case 0:
			break;
		case 1:		swprintf_s(str,_T("%.0f"),fPlies);
			break;
		case 2:
			theApp.m_Palette.m_PorosityThresholdCurve.SetPrimaryAxis(0);
			theApp.m_Palette.m_PorosityThresholdCurve.CalculateCoord(fPlies,Cp);
			swprintf_s(str,_T("%.02f"),Cp.fPos[1]);
			break;
		case 3:
			theApp.m_Palette.m_PorosityThresholdCurve.SetPrimaryAxis(0);
			theApp.m_Palette.m_PorosityThresholdCurve.CalculateCoord(fPlies,Cp);
			fTemp = theApp.m_Palette.m_pPorosity[0].fMeandBAttenuation - theApp.m_Palette.m_fPorosityRefAttenuation;
			swprintf_s(str,_T("%.02f"),fTemp + Cp.fPos[1]);		//??????????
			break;
		}
		break;
	}

	*pResult = 0;
}

void CPorosityPage::ClearEditSpin()
{
	SAFE_DELETE( m_pEditSpinItem );
}

BOOL CPorosityPage::OnKillActive() 
{

	ClearEditSpin();
	return CResizablePage::OnKillActive();
}

long CPorosityPage::PorosityItemChanged(unsigned int nn,long mm)
{
	theApp.m_Palette.PorosityMeanChanged(m_nIndex);
	Invalidate(FALSE);
	return 0;
}

void CPorosityPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rr,Rect;
	CRgn rgn;

	m_staticView.GetWindowRect(rr);
	ScreenToClient(rr);

	dc.GetClipBox(&Rect);
	rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
	dc.SelectClipRgn(&rgn);

	switch(theApp.m_LastSettings.nPorosityGraphType) {
	default:	RenderPorosityVsAttenuation(&dc,rr);
		break;
	case 1:	RenderFixedPorosityCurve(&dc,rr);
		break;
	}
	rgn.SetRectRgn(&Rect);
	dc.SelectClipRgn(&rgn);
}

void CPorosityPage::RenderPorosityVsAttenuation(CPaintDC *pDC, CRect rr)
{
	CString Buff,Temp;
	int	nn;
	CPen *pOldPen;

	pDC->FillRect(rr,&CBrush(RGB(255,255,255)));
	if(theApp.m_Palette.m_nPorosityL<=0) return;

	m_rrGrat = rr;


	LOGFONT lf;
	memset( (void*)&lf, 0, sizeof(lf));
	lf.lfHeight = 14;
	theApp.SetFontLanguage(&lf);

	CFont FontHorizontal,FontVertical;
	FontHorizontal.CreateFontIndirect(&lf);
	lf.lfEscapement=900;
	FontVertical.CreateFontIndirect(&lf);

	CFont* pOldFont = pDC->SelectObject(&FontHorizontal);
	pDC->SetBkMode(TRANSPARENT);

	Buff.Format(_T("%.02fMHz "),theApp.m_Palette.m_fPorosityFrequency);
	Temp.LoadString(IDS_Porosity_Vs_Attenuation);
	Buff += Temp;

	CSize size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.CenterPoint().x-size.cx/2,rr.top+1,Buff);

	theApp.m_Palette.GetMaxAttenuationPorosity(m_nIndex,&m_fMaxAttenuation,&m_fMaxPorosity);

	int nAttenuation = ((int)m_fMaxAttenuation/5)*5 + 5;
	m_nMaxPorosity = (int)m_fMaxPorosity + 1;

	Buff.Format(_T("%d"),nAttenuation);
	size = pDC->GetTextExtent(Buff);
	size.cx += 4;

	m_rrGrat.left += (size.cx + size.cy);
	m_rrGrat.bottom -= (size.cx * 2);
	m_rrGrat.top += size.cy;
	m_rrGrat.right -= size.cx;

	pDC->MoveTo(m_rrGrat.TopLeft());
	pDC->LineTo(m_rrGrat.left,m_rrGrat.bottom);
	pDC->LineTo(m_rrGrat.BottomRight());

	int	nPnX,nPnY;
	for(int xx=0;xx<=m_nMaxPorosity;xx+=2) {
		nPnX = MulDiv(xx,m_rrGrat.Width(),m_nMaxPorosity) + m_rrGrat.left;
		Buff.Format(_T("%d"),xx);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(nPnX-size.cx/2,m_rrGrat.bottom+2,Buff);
	}
	Buff.LoadString(IDS_Porosity);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(m_rrGrat.CenterPoint().x-size.cx/2,rr.bottom-size.cy,Buff);

	for(int yy=0;yy<=nAttenuation;yy+=5) {
		nPnY = m_rrGrat.bottom - MulDiv(yy,m_rrGrat.Height(),nAttenuation);
		Buff.Format(_T("%d"),yy);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(m_rrGrat.left-size.cx-2,nPnY-size.cy/2,Buff);
	}
	pDC->SelectObject(&FontVertical);
	Buff.LoadString(IDS_Attenuation);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.left,m_rrGrat.CenterPoint().y+size.cx/2,Buff);


	theApp.m_Palette.ProcessPorosityData();

	pDC->SelectObject(&FontHorizontal);
	for(int cc=0;cc<theApp.m_Palette.m_nPorosityCurveL;cc++) {
		CPen *pPen = new CPen(PS_SOLID,1,theApp.m_LastSettings.rgbPorosityCurve[cc]);
		pOldPen = pDC->SelectObject(pPen);
		CPolyCoord *pLine = &theApp.m_Palette.m_pPorosityCurve[cc];
		pDC->MoveTo(m_rrGrat.left,m_rrGrat.bottom);
		for(nn=0;nn<pLine->m_nCoordL;nn++) {
			nPnX = m_rrGrat.left + (int)(pLine->m_pCp[nn].fPos[0]*(float)m_rrGrat.Width() / (float)m_nMaxPorosity);
			nPnY = m_rrGrat.bottom - (int)(pLine->m_pCp[nn].fPos[1]*(float)m_rrGrat.Height() / (float)nAttenuation);
			pDC->LineTo(nPnX,nPnY);
		}
		pDC->SelectObject(pOldPen);
		delete pPen;
		Buff.Format(_T("X%d"),pLine->m_nPlys);
		pDC->TextOut(nPnX,nPnY-size.cy/2,Buff);
	}
	CPen penDot(PS_DOT,1,RGB(0,0,0));
	pOldPen = pDC->SelectObject(&penDot);

	nPnX = (int)(theApp.m_LastSettings.fPorosityThreshold * (float)m_rrGrat.Width() / (float)m_nMaxPorosity) + m_rrGrat.left;
	pDC->MoveTo(nPnX,m_rrGrat.bottom-2);
	pDC->LineTo(nPnX,m_rrGrat.top + size.cy + 2);
	
	pDC->SelectObject(&FontHorizontal);
	Buff.Format(_T("%.02f%%"),theApp.m_LastSettings.fPorosityThreshold);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(nPnX-size.cx/2,m_rrGrat.top,Buff);

	pDC->SelectObject(pOldPen);

	pDC->SelectObject(pOldFont);
}


void CPorosityPage::RenderFixedPorosityCurve(CPaintDC *pDC, CRect rr)
{
	CString Buff,Temp;
	CRect m_rrGrat = rr;
	float fMaxAttenuation;
	int nMaxPlies;
	int	nn;
	CPoint ptArray[20];

	pDC->FillRect(rr,&CBrush(RGB(255,255,255)));
	if(theApp.m_Palette.m_nPorosityL<=0) return;


	LOGFONT lf;
	memset( (void*)&lf, 0, sizeof(lf));
	lf.lfHeight = 14;
	theApp.SetFontLanguage(&lf);

	CFont FontHorizontal,FontVertical;
	FontHorizontal.CreateFontIndirect(&lf);
	lf.lfEscapement=900;
	FontVertical.CreateFontIndirect(&lf);

	CFont* pOldFont = pDC->SelectObject(&FontHorizontal);
	pDC->SetBkMode(TRANSPARENT);

	Temp.LoadString(IDS_Porosity);
	Buff.Format(_T("%.01f%% %s @ %.02fMHz"),theApp.m_LastSettings.fPorosityThreshold, Temp, theApp.m_Palette.m_fPorosityFrequency);

	CSize size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.CenterPoint().x-size.cx/2,rr.top+1,Buff);

	theApp.m_Palette.GetMaxAttenuationPlys(m_nIndex,&fMaxAttenuation,&nMaxPlies);

	int nAttenuation = ((int)fMaxAttenuation/5)*5 + 5;
	int nPlies = ((nMaxPlies/5) + 1) * 5;

	Buff.Format(_T("%d"),nAttenuation);
	size = pDC->GetTextExtent(Buff);
	size.cx += 4;

	m_rrGrat.left += (size.cx + size.cy);
	m_rrGrat.bottom -= (size.cx * 2);
	m_rrGrat.top += size.cy;
	m_rrGrat.right -= size.cx;

	pDC->MoveTo(m_rrGrat.TopLeft());
	pDC->LineTo(m_rrGrat.left,m_rrGrat.bottom);
	pDC->LineTo(m_rrGrat.BottomRight());

	int	nPnX,nPnY;
	for(int xx=0;xx<=nPlies;xx+=5) {
		nPnX = MulDiv(xx,m_rrGrat.Width(),nPlies) + m_rrGrat.left;
		Buff.Format(_T("%d"),xx);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(nPnX-size.cx/2,m_rrGrat.bottom+2,Buff);
	}
	Buff.LoadString(IDS_Plies);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(m_rrGrat.CenterPoint().x-size.cx/2,rr.bottom-size.cy,Buff);

	for(int yy=0;yy<=nAttenuation;yy+=5) {
		nPnY = m_rrGrat.bottom - MulDiv(yy,m_rrGrat.Height(),nAttenuation);
		Buff.Format(_T("%d"),yy);
		size = pDC->GetTextExtent(Buff);
		pDC->TextOut(m_rrGrat.left-size.cx-2,nPnY-size.cy/2,Buff);
	}
	pDC->SelectObject(&FontVertical);
	Buff.LoadString(IDS_Attenuation);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.left,m_rrGrat.CenterPoint().y+size.cx/2,Buff);


	theApp.m_Palette.ProcessPorosityData();

	//?????????????

	CPolyCoord *pLine = &theApp.m_Palette.m_PorosityThresholdCurve;
	for(nn=0;nn<pLine->m_nCoordL;nn++) {
		ptArray[nn].x = m_rrGrat.left + (int)(pLine->m_pCp[nn].fPos[0]*(float)m_rrGrat.Width() / (float)nPlies);
		ptArray[nn].y = m_rrGrat.bottom - (int)(pLine->m_pCp[nn].fPos[1]*(float)m_rrGrat.Height() / (float)nAttenuation);
		if(nn==0) {
			pDC->MoveTo(ptArray[nn]);
		} else {
			pDC->LineTo(ptArray[nn]);
		}
	}

	CPen *pPen = new CPen(PS_DASH,1,RGB(200,200,200));
	CPen* pOldPen = pDC->SelectObject(pPen);
	for(nn=0;nn<pLine->m_nCoordL;nn++) {
		pDC->MoveTo(m_rrGrat.left,ptArray[nn].y);
		pDC->LineTo(ptArray[nn]);
		pDC->LineTo(ptArray[nn].x, m_rrGrat.bottom);
	}


	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
	delete pPen;
}


void CPorosityPage::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	CRect rrView,rrList,rrEdit;

	if(GetSafeHwnd() && m_staticView.GetSafeHwnd()) {
		GetClientRect(&m_rrClient);

		m_staticView.GetWindowRect(&rrView);
		ScreenToClient(&rrView);
		m_listPts.GetWindowRect(&rrList);
		ScreenToClient(&rrList);

		m_editStandardID.GetWindowRect(&rrEdit);
		ScreenToClient(&rrEdit);
		m_rrClient.top = rrEdit.bottom + 4;

		rrView = m_rrClient;
		rrView.bottom = (int)((float)m_rrClient.Height() * m_fBorderPos) - 4;
		m_staticView.SetWindowPos( NULL , rrView.left, rrView.top, rrView.Width(), rrView.Height(), NULL);

		rrList = m_rrClient;
		rrList.top =(int)((float)m_rrClient.Height() * m_fBorderPos) + 4;
		m_listPts.SetWindowPos( NULL , rrList.left, rrList.top, rrList.Width(), rrList.Height(), NULL);

		m_rrBorder = rrList;
		m_rrBorder.top = rrView.bottom;
		m_rrBorder.bottom = rrList.top;

		Invalidate(false);
	}
}

void CPorosityPage::OnPorosityDelete() 
{
	theApp.m_Palette.PorosityDeleteItem(m_nIndex);
	FillList();
	Invalidate(FALSE);
}

void CPorosityPage::OnPorosityDeleteall() 
{
	theApp.m_Palette.PorosityDeleteAll();

	FillList();
	Invalidate(FALSE);
}

void CPorosityPage::OnPorositySort() 
{
	theApp.m_Palette.PorositySort();
	FillList();
	Invalidate(FALSE);
}

void CPorosityPage::OnRclickListPoints(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint	Point;

	POSITION pos = m_listPts.GetFirstSelectedItemPosition();
	m_nIndex=m_listPts.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_POROSITY_LIST_MENU)) {

		pPopup = menu->GetSubMenu(0);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
	
	*pResult = 0;
}


void CPorosityPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CRect Rect;
	m_staticView.GetWindowRect(Rect);
	m_ptClient = point;
	CMenu* menu = (CMenu *) new CMenu;

	switch(theApp.m_LastSettings.nPorosityGraphType) {
	default:
		if(Rect.PtInRect(point)) {
			if (menu->LoadMenu(IDR_RB_POROSITY_VIEW_MENU)) {
				CMenu* pPopup = menu->GetSubMenu(0);

				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);

				delete menu;
			}

		}
		break;
	case 1:
		break;
	}
	
}

void CPorosityPage::OnPorosityCurve1Colour() 
{
	CMFCColorDialog dlg;
	
	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbPorosityCurve[0]=dlg.GetColor();
		Invalidate(FALSE);
	};
	
}

void CPorosityPage::OnPorosityCurve2Colour() 
{
	CMFCColorDialog dlg;
	
	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbPorosityCurve[1]=dlg.GetColor();
		Invalidate(FALSE);
	};
}

void CPorosityPage::OnPorosityCurve3Colour() 
{
	CMFCColorDialog dlg;
	
	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbPorosityCurve[2]=dlg.GetColor();
		Invalidate(FALSE);
	};
	
}

void CPorosityPage::OnPorosityCurve4Colour() 
{
	CMFCColorDialog dlg;
	
	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbPorosityCurve[3]=dlg.GetColor();
		Invalidate(FALSE);
	};
}

void CPorosityPage::OnPorosityCurve5Colour() 
{
	CColorDialog dlg;
	
	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbPorosityCurve[4]=dlg.GetColor();
		Invalidate(FALSE);
	};
}

void CPorosityPage::OnChangeEditFrequency() 
{
	CString Buff;

	m_editFrequency.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Palette.m_fPorosityFrequency);
}

void CPorosityPage::OnChangeEditProbeDiameter() 
{
	CString Buff;

	m_editProbeDiameter.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Palette.m_fPorosityProbeDiameter);
}

void CPorosityPage::OnChangeEditSquirterDiameter() 
{
	CString Buff;

	m_editSquirterDiameter.GetWindowText(Buff);
	_WTOF(Buff,theApp.m_Palette.m_fPorositySquirterDiameter);
}

BOOL CPorosityPage::PreTranslateMessage(MSG* pMsg) 
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	CRect rr = m_rrClient;
	CRect rrView,rrList;
	CString Buff;

	switch(pMsg->message) {
	case 0x100:
		switch(pMsg->wParam) {
		case CURSOR_DOWN:
			break;
		case CURSOR_UP:
			break;
		}
		break;
	case 0x200: // Mouse move
		switch(pMsg->wParam) {
		case 1:
			if((m_bLBDown == true) && (point != m_ptLbDown) && (m_bMoveBorder == true)) {
				m_fBorderPos = (float)point.y * 1.0f / (float)rr.Height();

				rrView = m_rrClient;
				rrView.bottom = (int)((float)m_rrClient.Height() * m_fBorderPos) - 4;
				m_staticView.SetWindowPos( NULL , rrView.left, rrView.top, rrView.Width(), rrView.Height(), NULL);

				rrList = m_rrClient;
				rrList.top =(int)((float)m_rrClient.Height() * m_fBorderPos) + 4;
				m_listPts.SetWindowPos( NULL , rrList.left, rrList.top, rrList.Width(), rrList.Height(), NULL);

				m_rrBorder = rrList;
				m_rrBorder.top = rrView.bottom;
				m_rrBorder.bottom = rrList.top;

				m_listPts.UpdateWindow();
				m_staticView.UpdateWindow();
				Invalidate(true);

				m_ptLbDown = point;
				return TRUE;
			}
			if((theApp.m_LastSettings.nPorosityGraphType == 0) && (m_bLBDown == true) && (m_bMoveThreshold==true)) {
				theApp.m_LastSettings.fPorosityThreshold = (float)(point.x - m_rrGrat.left) * (float)m_nMaxPorosity / (float)m_rrGrat.Width();
				Invalidate();
				return TRUE;
			}
			break;
		};
		break;
	case 0x201://LBUTTONDOWN
		if(m_rrBorder.PtInRect(point)) {
			m_bLBDown = true;
			m_ptLbDown = point;
			m_bMoveBorder = true;
			return TRUE;
		}
		m_staticView.GetWindowRect(rrView);
		ScreenToClient(rrView);
		if((theApp.m_LastSettings.nPorosityGraphType == 0) && rrView.PtInRect(point)) {
			m_bLBDown = true;
			m_bMoveThreshold = true;
			theApp.m_LastSettings.fPorosityThreshold = (float)(point.x - m_rrGrat.left) * (float)m_nMaxPorosity / (float)m_rrGrat.Width();
			Invalidate(FALSE);
			return TRUE;
		}
		break;

	case 0x202:	//LBUTTONUP
		m_bLBDown = false;
		m_bMoveBorder = false;
		m_bMoveThreshold = false;
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CPorosityPage::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint	point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	if(m_rrBorder.PtInRect(point)) {
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		return TRUE;
	}
	
	return CResizablePage::OnSetCursor(pWnd, nHitTest, message);
}

void CPorosityPage::OnChangeEditRefTarget() 
{
	CString Buff;

	m_editRefTarget.GetWindowText(theApp.m_Palette.m_PorosityRefTarget);
}

void CPorosityPage::OnChangeEditRefAttenuation() 
{
	CString Buff;

	m_editRefAttenuation.GetWindowText(Buff);

	_WTOF(Buff,theApp.m_Palette.m_fPorosityRefAttenuation);
}

void CPorosityPage::OnChangeEditStandardId() 
{
	m_editStandardID.GetWindowText(theApp.m_Palette.m_PorosityStandardID);
}
