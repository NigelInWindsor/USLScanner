// PositionGraphicalPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "PositionGraphicalPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	_CLASSIC	0x001
#define	_STATUS		0x002

/////////////////////////////////////////////////////////////////////////////
// CPositionGraphicalPage property page

IMPLEMENT_DYNCREATE(CPositionGraphicalPage, CPropertyPage)

CPositionGraphicalPage::CPositionGraphicalPage() : CPropertyPage(CPositionGraphicalPage::IDD)
{
	//{{AFX_DATA_INIT(CPositionGraphicalPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bDlgCreated = false;
	m_pArray = NULL;
	m_pFont = NULL;

}

CPositionGraphicalPage::~CPositionGraphicalPage()
{
	SAFE_DELETE( m_pArray );
	SAFE_DELETE( m_pFont );
}

void CPositionGraphicalPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPositionGraphicalPage)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPositionGraphicalPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPositionGraphicalPage)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_POSITION_CLASSIC, OnButtonPositionClassic)
	ON_COMMAND(ID_BUTTON_POSITION_STATUS, OnButtonPositionStatus)
	ON_COMMAND(ID_BUTTON_POSITION_MACHINE, OnButtonPositionMachine)
	ON_COMMAND(ID_BUTTON_POSITION_HEAD, OnButtonPositionHead)
	ON_COMMAND(ID_BUTTON_POSITION_SURFACE, OnButtonPositionSurface)
	ON_COMMAND(ID_BUTTON_POSITION_SURFACE_WATER, OnButtonPositionSurfaceWater)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPositionGraphicalPage message handlers
int CPositionGraphicalPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndToolBar.LoadToolBar(IDR_POSITION_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()+210,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndToolBar);
	
	return 0;
}

BOOL CPositionGraphicalPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_StaticView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);

	m_StaticView.SetNewPos();

	CFont* pFont = GetFont();
	pFont->GetLogFont(&m_LogFont);

	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&m_LogFont);
	m_bDlgCreated = true;

	SetToolBarCheckedState();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPositionGraphicalPage::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	
	m_StaticView.SetNewPos();

	if(m_bDlgCreated == TRUE) {
		SetFontSize();
		Invalidate(false);
	}
}

BOOL CPositionGraphicalPage::OnKillActive() 
{
	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

BOOL CPositionGraphicalPage::OnSetActive() 
{
	APP App = (APP) AfxGetApp();
	
	App->m_LastSettings.nLastPositionTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
	SetTimer(1,200,NULL);
	
	return CPropertyPage::OnSetActive();
}

void CPositionGraphicalPage::OnTimer(UINT nIDEvent) 
{
	
	Invalidate(false);

	CPropertyPage::OnTimer(nIDEvent);
}

void CPositionGraphicalPage::OnPaint() 
{
	if(m_bDlgCreated != true) return;
	CPaintDC dc(this); // device context for painting
	APP App = (APP) AfxGetApp();
	CRect		rr,rect,rrStore;
	CDC			dcMem;
	CBitmap		bmp;
	static	int	nWidth;
	static	int	nHeight;
	bool bFlagInvalidate = false;



	if(	m_nOldMeasurementUnits - App->m_LastSettings.nMeasurementUnits) {
		m_nOldMeasurementUnits = App->m_LastSettings.nMeasurementUnits;
		m_CpOld.Zero();
	}


	switch(App->m_LastSettings.nPositionDisplayMode) {
	case 0: App->m_Motors.GetEncoderPos(&m_CpSurface);
		break;
	case 1:	App->m_Motors.GetHeadPos(&m_CpSurface);
		break;
	case 2:	App->m_Motors.GetHeadPos(&m_CpHead);
		App->m_Kinematics.SurfaceFromHead(&m_CpHead,&m_CpSurface,PORTSIDE,TRUE,TRUE);
		App->m_Kinematics.SurfaceFromHead(&m_CpHead,&m_CpSurface,STARBOARD,TRUE,TRUE);
		break;
	case 3:	App->m_Motors.GetHeadPos(&m_CpHead);
		App->m_Kinematics.CheckValidWaterPath(PORTSIDE,App->m_LastSettings.nWaterPathTS[PORTSIDE]);
		App->m_Kinematics.CheckValidWaterPath(STARBOARD,App->m_LastSettings.nWaterPathTS[STARBOARD]);
		App->m_Kinematics.SurfaceFromHead(&m_CpHead,&m_CpSurface,PORTSIDE,TRUE,FALSE);
		App->m_Kinematics.SurfaceFromHead(&m_CpHead,&m_CpSurface,STARBOARD,TRUE,FALSE);
		break;
	}




	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
//	rr.DeflateRect(1,1);
	if((m_pArray==NULL) || (rr.Width()-nWidth) || (rr.Height()-nHeight)) {
		SAFE_DELETE( m_pArray );
		m_pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
		nWidth=rr.Width();
		nHeight=rr.Height();
		memset(m_pArray,0xff,rr.Width()*sizeof(COLORREF)*rr.Height());
		bFlagInvalidate = true;
	}

	rect.left=0;
	rect.top=0;
	rect.right = rr.Width();
	rect.bottom = rr.Height();

	bmp.CreateCompatibleBitmap(&dc,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),m_pArray);
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	CFont* pOldFont = dcMem.SelectObject(m_pFont);
	dcMem.SetTextColor(App->m_LastSettings.rgbFaroText);
	dcMem.SetBkMode(TRANSPARENT);

	dcMem.FillRect(&rect,&CBrush(App->m_LastSettings.rgbFaroBk));

	if(App->m_LastSettings.nPositionGraphicMode & _CLASSIC) {
		RenderClassic(&dcMem,&rect);
	}
	if(App->m_LastSettings.nPositionGraphicMode & _STATUS) {
		RenderStatus(&dcMem,&rect);
	}


//	if(bFlagInvalidate == true) {
		dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);

//	}

	dcMem.SelectObject(pOldFont);

}
BOOL CPositionGraphicalPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
   // need to handle both ANSI and UNICODE versions of the message
   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      // idFrom is actually the HWND of the tool
      nID = ::GetDlgCtrlID((HWND)nID);
   }

	switch(nID) {
	case ID_BUTTON_POSITION_CLASSIC: strTipText.LoadString(IDS_Classic);
	   break;
	case ID_BUTTON_POSITION_STATUS: strTipText.LoadString(IDS_Status);
	   break;
	case ID_BUTTON_POSITION_MACHINE: strTipText.LoadString(IDS_Machine);
	   break;
	case ID_BUTTON_POSITION_HEAD: strTipText.LoadString(IDS_Head);
	   break;
	case ID_BUTTON_POSITION_SURFACE: strTipText.LoadString(IDS_Surface);
	   break;
	case ID_BUTTON_POSITION_SURFACE_WATER: strTipText.LoadString(IDS_Surface_Water);
	   break;
	}
//      strTipText.Format(_T("Control ID = %d", nID);

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}


void CPositionGraphicalPage::SetToolBarCheckedState()
{
	APP App = (APP) AfxGetApp(); 
	int	nIndex;


	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_POSITION_CLASSIC))>=0) 
	App->m_LastSettings.nPositionGraphicMode & _CLASSIC ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_POSITION_STATUS))>=0) 
	App->m_LastSettings.nPositionGraphicMode & _STATUS ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);


	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_POSITION_MACHINE))>=0) 
	App->m_LastSettings.nPositionDisplayMode == 0 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_POSITION_HEAD))>=0) 
	App->m_LastSettings.nPositionDisplayMode == 1 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_POSITION_SURFACE))>=0) 
	App->m_LastSettings.nPositionDisplayMode == 2 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_POSITION_SURFACE_WATER))>=0) 
	App->m_LastSettings.nPositionDisplayMode == 3 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);


}


void CPositionGraphicalPage::OnButtonPositionClassic() 
{
	APP App = (APP) AfxGetApp();

	App->m_LastSettings.nPositionGraphicMode &= ~_STATUS;
	App->m_LastSettings.nPositionGraphicMode |= _CLASSIC;

	SetFontSize();
	SetToolBarCheckedState();
	
}

void CPositionGraphicalPage::OnButtonPositionStatus() 
{
	APP App = (APP) AfxGetApp();

	App->m_LastSettings.nPositionGraphicMode &= ~_CLASSIC;
	App->m_LastSettings.nPositionGraphicMode |= _STATUS;

	SetFontSize();
	SetToolBarCheckedState();
}

void CPositionGraphicalPage::OnButtonPositionMachine() 
{
	APP App = (APP) AfxGetApp();
	App->m_LastSettings.nPositionDisplayMode = 0;
	SetToolBarCheckedState();
}

void CPositionGraphicalPage::OnButtonPositionHead() 
{
	APP App = (APP) AfxGetApp();
	App->m_LastSettings.nPositionDisplayMode = 1;
	SetToolBarCheckedState();
}

void CPositionGraphicalPage::OnButtonPositionSurface() 
{
	APP App = (APP) AfxGetApp();
	App->m_LastSettings.nPositionDisplayMode = 2;
	SetToolBarCheckedState();
}

void CPositionGraphicalPage::OnButtonPositionSurfaceWater() 
{
	APP App = (APP) AfxGetApp();
	App->m_LastSettings.nPositionDisplayMode = 3;
	SetToolBarCheckedState();
}

void CPositionGraphicalPage::SetFontSize()
{
	APP App = (APP) AfxGetApp();
	CString Buff;
	CRect rr;
	LOGFONT lf;
	CSize size;
	CDC* pDC = GetDC();
	int nWidth=0;
	int nNumberAxes=0;
	CString strName;


	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	lf = m_LogFont;
	if(App->m_LastSettings.nPositionGraphicMode & _CLASSIC) {
		lf.lfHeight = (int)((float)rr.Height() / -5.8f);
		_tcscpy(lf.lfFaceName, _T("Times"));
		SAFE_DELETE( m_pFont );
		m_pFont = new CFont();
		m_pFont->CreateFontIndirect(&lf);

		CFont* pOldFont = pDC->SelectObject(m_pFont);
		for(int nAxis=0;nAxis<10;nAxis++) {
			Buff.Format(L"%s:-99999.9mm", App->m_Axes[nAxis].cName);
			size = pDC->GetTextExtent(Buff);
			if(size.cx > nWidth) nWidth = size.cx;
		}
		pDC->SelectObject(pOldFont);

		if(nWidth > (rr.Width()/2)) {
			
			pDC->SelectObject(pOldFont);

			lf.lfHeight = MulDiv( rr.Width()/2, lf.lfHeight, nWidth);
			_tcscpy(lf.lfFaceName, _T("Times"));
			SAFE_DELETE( m_pFont );
			m_pFont = new CFont();
			m_pFont->CreateFontIndirect(&lf);
		
		}
	}


	if(App->m_LastSettings.nPositionGraphicMode & _STATUS) {
		lf.lfHeight = (int)((float)rr.Height() / -10.8f);
		_tcscpy(lf.lfFaceName, _T("Times"));
		SAFE_DELETE( m_pFont );
		m_pFont = new CFont();
		m_pFont->CreateFontIndirect(&lf);

		CFont* pOldFont = pDC->SelectObject(m_pFont);

		for(int nAxis=0;nAxis<12;nAxis++) {
			if((App->m_Axes[nAxis].bAvailable) && (App->m_Axes[nAxis].nType == SERVO)) {
				nNumberAxes++;
				Buff.Format(L" -9999.99mm");
				size = pDC->GetTextExtent(Buff);
				if(size.cx > nWidth) {
					strName = Buff;
					nWidth = size.cx;
				}
			}
		}

		if((nWidth * nNumberAxes) > rr.Width()) {
			pDC->SelectObject(pOldFont);

			lf.lfHeight = MulDiv( rr.Width(), lf.lfHeight, (nWidth * nNumberAxes));
			_tcscpy(lf.lfFaceName, _T("Times"));
			SAFE_DELETE( m_pFont );
			m_pFont = new CFont();
			m_pFont->CreateFontIndirect(&lf);
		}
	}

	m_nLongestNameLength = 0;
	ReleaseDC(pDC);

}

void CPositionGraphicalPage::RenderClassic(CDC *pDC, CRect *rr)
{

	APP App = (APP) AfxGetApp();
	CString Buff;
	int nXp[6],nYp;
	CString strName,strPos,strUnit;
	int nYStep = rr->Height() / 5;
	CString Unit[] = {"mm", "\x022", "\x0b0"};
	CSize size;

	if(App->m_nSide0Orientation == 0) {
		nXp[0]=1;
		nXp[1]=rr->Width()/2 - 10;
		nXp[3]=rr->Width()/2;
		nXp[4]=rr->Width() -10;
		nYp=1;
	} else {
		nXp[3]=1;
		nXp[4]=rr->Width()/2 - 10;
		nXp[0]=rr->Width()/2;
		nXp[1]=rr->Width() - 10;
		nYp=1;
	}
	strName.Format(L"%s:",App->m_Axes[App->m_Tank.nXLeft].cName);
	strPos.Format(_T("%.*f %s"),App->m_Axes[App->m_Tank.nXLeft].nPrecision,m_CpSurface.Side0.fX * App->m_fUnits, App->m_Units);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[0],&nYp);

	strName.Format(L"%s:",App->m_Axes[App->m_Tank.nXRight].cName);
	strPos.Format(_T("%.*f %s"),App->m_Axes[App->m_Tank.nXRight].nPrecision,m_CpSurface.Side1.fX * App->m_fUnits, App->m_Units);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[3],&nYp);
	nYp += nYStep;

	strName.Format(L"%s:",App->m_Axes[App->m_Tank.nYLeft].cName);
	strPos.Format(_T("%.*f %s"),App->m_Axes[App->m_Tank.nYLeft].nPrecision,m_CpSurface.Side0.fY * App->m_fUnits, App->m_Units);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[0],&nYp);

	strName.Format(L"%s:",App->m_Axes[App->m_Tank.nYRight].cName);
	strPos.Format(_T("%.*f %s"),App->m_Axes[App->m_Tank.nYRight].nPrecision,m_CpSurface.Side1.fY * App->m_fUnits, App->m_Units);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[3],&nYp);
	nYp += nYStep;

	strName.Format(L"%s:",App->m_Axes[App->m_Tank.nZLeft].cName);
	strPos.Format(_T("%.*f %s"),App->m_Axes[App->m_Tank.nZLeft].nPrecision,m_CpSurface.Side0.fZ * App->m_fUnits, App->m_Units);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[0],&nYp);

	strName.Format(L"%s:",App->m_Axes[App->m_Tank.nZRight].cName);
	pDC->TextOut(nXp[3],nYp,Buff);
	strPos.Format(_T("%.*f %s"),App->m_Axes[App->m_Tank.nZRight].nPrecision,m_CpSurface.Side1.fZ * App->m_fUnits, App->m_Units);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[3],&nYp);
	nYp += nYStep;

	strName.Format(L"%s:",App->m_Axes[App->m_Tank.nXtLeft].cName);
	strPos.Format(_T("%.*f %s"),App->m_Axes[App->m_Tank.nXtLeft].nPrecision,m_CpSurface.Side0.fXt, Unit[App->m_Axes[App->m_Tank.nXtLeft].nUnitSelected]);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[0],&nYp);

	strName.Format(L"%s:",App->m_Axes[App->m_Tank.nXtRight].cName);
	strPos.Format(_T("%.*f %s"),App->m_Axes[App->m_Tank.nXtRight].nPrecision,m_CpSurface.Side1.fXt, Unit[App->m_Axes[App->m_Tank.nXtRight].nUnitSelected]);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[3],&nYp);
	nYp += nYStep;

	strName.Format(L"%s:",App->m_Axes[App->m_Tank.nYtLeft].cName);
	strPos.Format(_T("%.*f %s"),App->m_Axes[App->m_Tank.nYtLeft].nPrecision,m_CpSurface.Side0.fYt, Unit[App->m_Axes[App->m_Tank.nYtLeft].nUnitSelected]);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[0],&nYp);

	strName.Format(L"%s:",App->m_Axes[App->m_Tank.nYtRight].cName);
	strPos.Format(_T("%.*f %s"),App->m_Axes[App->m_Tank.nYtRight].nPrecision,m_CpSurface.Side1.fYt, Unit[App->m_Axes[App->m_Tank.nYtRight].nUnitSelected]);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[3],&nYp);
	nYp += nYStep;

}

void CPositionGraphicalPage::TextOut(CDC* pDC,CString *strName, CString *strPos, CString *strUnit, int *nXp, int *nYp)
{
	CSize size;
	int nX;

	size = pDC->GetTextExtent(*strName);
	if(size.cx > m_nLongestNameLength) m_nLongestNameLength = size.cx;

	nX = nXp[0]+m_nLongestNameLength-size.cx;
	if(nX<0) nX = 0;
	pDC->TextOut(nX,*nYp,*strName);

	size = pDC->GetTextExtent(*strPos);
	pDC->TextOut(nXp[1]-size.cx,*nYp,*strPos);

}

#define _RED	RGB(255,  0,  0)
#define _GREEN	RGB(  0,255,  0)
#define _BLACK	RGB(  0,  0,  0)

void CPositionGraphicalPage::RenderStatus(CDC *pDC, CRect *rr)
{

	APP App = (APP) AfxGetApp();
	CString Buff;
	int nNumberAxes=0;
	int nWidth,nAxis,nXp,nYp,nn,nYStep;
	CSize size;
	CString Unit[] = {"mm", "\x022", "\x0b0"};
	CString strInPos,strMin,strMax,strFault,strEnable,strSoft,strHard,strHome;

	strInPos.LoadString(IDS_InPos);
	strMin.LoadString(IDS_Min);
	strMax.LoadString(IDS_Max);
	strFault.LoadString(IDS_Fault);
	strEnable.LoadString(IDS_Enabled);
	strSoft.LoadString(IDS_Soft);
	strHard.LoadString(IDS_Hard);
	strHome.LoadString(IDS_Homed);

	for(nAxis=0;nAxis<12;nAxis++) {
		if((App->m_Axes[nAxis].bAvailable) && (App->m_Axes[nAxis].nType == SERVO)) {
			nNumberAxes++;
		}
	}

	nWidth = rr->Width() / nNumberAxes;
	nYStep = rr->Height() / 10;

	for(nAxis=0,nn=0;nAxis<12;nAxis++) {

		if((App->m_Axes[nAxis].bAvailable) && (App->m_Axes[nAxis].nType == SERVO)) {

			pDC->SetTextColor(App->m_LastSettings.rgbFaroText);

			nYp = 1;
			nXp=nn * nWidth + nWidth / 2 ;
			Buff.Format(L" %s",App->m_Axes[nAxis].cName);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(nXp - size.cx / 2,nYp,Buff);
			nYp += nYStep;

			Buff.Format(_T("%.*f %s"),App->m_Axes[nAxis].nPrecision,(float)App->m_Axes[nAxis].nPos * App->m_Axes[nAxis].fStepSize, Unit[App->m_Axes[nAxis].nUnitSelected]);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(nXp - size.cx / 2,nYp,Buff);
			nYp += nYStep;

			App->m_Axes[nAxis].nStatus & PMAC_IN_POS	? 	pDC->SetTextColor(_GREEN)	: pDC->SetTextColor(_BLACK);
			size = pDC->GetTextExtent(strInPos);
			pDC->TextOut(nXp - size.cx / 2,nYp,strInPos);
			nYp += nYStep;

			App->m_Axes[nAxis].nStatus & PMAC_HOMED	? 	pDC->SetTextColor(_GREEN)	: pDC->SetTextColor(_BLACK);
			size = pDC->GetTextExtent(strHome);
			pDC->TextOut(nXp - size.cx / 2,nYp,strHome);
			nYp += nYStep;

			App->m_Axes[nAxis].nStatus & PMAC_AMPLIFIER_ENABLED	? 	pDC->SetTextColor(_GREEN)	: pDC->SetTextColor(_BLACK);
			size = pDC->GetTextExtent(strEnable);
			pDC->TextOut(nXp - size.cx / 2,nYp,strEnable);
			nYp += nYStep;

			App->m_Axes[nAxis].nStatus & PMAC_SOFT_LIMIT	? 	pDC->SetTextColor(_RED)	: pDC->SetTextColor(_BLACK);
			size = pDC->GetTextExtent(strSoft);
			pDC->TextOut(nXp - size.cx / 2,nYp,strSoft);
			nYp += nYStep;

			App->m_Axes[nAxis].nStatus & PMAC_HARD_LIMIT	? 	pDC->SetTextColor(_RED)	: pDC->SetTextColor(_BLACK);
			size = pDC->GetTextExtent(strHard);
			pDC->TextOut(nXp - size.cx / 2,nYp,strHard);
			nYp += nYStep;

			App->m_Axes[nAxis].nStatus & PMAC_MIN_SWITCH	? 	pDC->SetTextColor(_RED)	: pDC->SetTextColor(_BLACK);
			size = pDC->GetTextExtent(strMin);
			pDC->TextOut(nXp - size.cx / 2,nYp,strMin);
			nYp += nYStep;

			App->m_Axes[nAxis].nStatus & PMAC_MAX_SWITCH	? 	pDC->SetTextColor(_RED)	: pDC->SetTextColor(_BLACK);
			size = pDC->GetTextExtent(strMax);
			pDC->TextOut(nXp - size.cx / 2,nYp,strMax);
			nYp += nYStep;


			App->m_Axes[nAxis].nStatus & PMAC_AMPLIFIER_FAULT	? 	pDC->SetTextColor(_RED)	: pDC->SetTextColor(_BLACK);
			size = pDC->GetTextExtent(strFault);
			pDC->TextOut(nXp - size.cx / 2,nYp,strFault);
			nYp += nYStep;

			nn++;
		}
	}

}

