// PositionGraphicalPage1.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "PositionGraphicalPage1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPositionGraphicalPage property page
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
	
	m_pArray = NULL;
	m_pFont = NULL;

	Unit[0] = "mm";
	Unit[1] = "\x022";
	Unit[2] = "\x0b0";

	strInPos.LoadString(IDS_InPos);
	strMin.LoadString(IDS_Min);
	strMax.LoadString(IDS_Max);
	strFault.LoadString(IDS_Fault);
	strEnable.LoadString(IDS_Enabled);
	strSoft.LoadString(IDS_Soft);
	strHard.LoadString(IDS_Hard);
	strHome.LoadString(IDS_Homed);

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
	ON_COMMAND(ID_BUTTON_POSITION_STATUS_1, OnButtonPositionStatus)
	ON_COMMAND(ID_BUTTON_POSITION_MACHINE, OnButtonPositionMachine)
	ON_COMMAND(ID_BUTTON_POSITION_HEAD, OnButtonPositionHead)
	ON_COMMAND(ID_BUTTON_POSITION_SURFACE, OnButtonPositionSurface)
	ON_COMMAND(ID_BUTTON_POSITION_SURFACE_WATER, OnButtonPositionSurfaceWater)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_FAROCOORDS_RIGHTJUSTIFY, OnFarocoordsRightjustify)
	ON_COMMAND(ID_FAROCOORDS_LEFTJUSTIFY, OnFarocoordsLeftjustify)
	ON_COMMAND(ID_FAROCOORDS_TEXTCOLOUR, OnFarocoordsTextcolour)
	ON_COMMAND(ID_FAROCOORDS_BACKGROUNDCOLOUR, OnFarocoordsBackgroundcolour)
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
	

	SetToolBarCheckedState();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPositionGraphicalPage::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	
	m_StaticView.SetNewPos();

	if(GetSafeHwnd()) {
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
	theApp.m_LastSettings.nLastPositionTab = ((CPropertySheet*)GetParent())->GetActiveIndex();
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
	if (GetSafeHwnd() == NULL) return;
	CPaintDC dc(this); // device context for painting
	CRect		rr,rect,rrStore;
	CDC			dcMem;
	CBitmap		bmp;
	static	int	nWidth;
	static	int	nHeight;
	bool bFlagInvalidate = false;

	if(	m_nOldMeasurementUnits - theApp.m_LastSettings.nMeasurementUnits) {
		m_nOldMeasurementUnits = theApp.m_LastSettings.nMeasurementUnits;
		m_CpOld.Zero();
	}

	theApp.m_Ethercat.InvalidateRobotJoints();

	switch(theApp.m_LastSettings.nPositionDisplayMode) {
	case 0: theApp.m_Motors.GetEncoderPos(&m_CpSurface);
		break;
	case 1:	theApp.m_Motors.GetHeadPos(&m_CpSurface);
		break;
	case 2:	theApp.m_Motors.GetHeadPos(&m_CpHead);
		theApp.m_Kinematics.SurfaceFromHead(&m_CpHead,&m_CpSurface,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.SurfaceFromHead(&m_CpHead,&m_CpSurface,STARBOARD,TRUE,TRUE);
		break;
	case 3:	theApp.m_Motors.GetHeadPos(&m_CpHead);
		theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
		theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
		theApp.m_Kinematics.SurfaceFromHead(&m_CpHead,&m_CpSurface,PORTSIDE,TRUE,FALSE);
		theApp.m_Kinematics.SurfaceFromHead(&m_CpHead,&m_CpSurface,STARBOARD,TRUE,FALSE);
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
	dcMem.SetTextColor(theApp.m_LastSettings.rgbFaroText);
	dcMem.SetBkMode(TRANSPARENT);

	dcMem.FillRect(&rect,&CBrush(theApp.m_LastSettings.rgbFaroBk));

	if(theApp.m_LastSettings.nPositionGraphicMode & _CLASSIC) {
		switch(theApp.m_Tank.nScannerDescription) {
		default:	RenderClassic(&dcMem,&rect);
			break;
		case SINGLE_BRIDGE_TT_ROLLERS:
		case SINGLE_BRIDGE_TANK_WITH_R:
		case SINGLE_BRIDGE_TANK: RenderClassicSingleBridgeTank(&dcMem,&rect);
			break;
		case DUAL_ROBOT_9_PLUS_9:
			break;
		}
	}
	if(theApp.m_LastSettings.nPositionGraphicMode & _STATUS) {
		switch (theApp.m_Tank.nScannerDescription) {
		default: RenderStatus(&dcMem, &rect);
			break;
		case DUAL_ROBOT_9_PLUS_9:  RenderStatusDualRobot9Plus9(&dcMem, &rect);
			break;
		}
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
	case ID_BUTTON_POSITION_STATUS_1: strTipText.LoadString(IDS_Status);
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
	int	nIndex;

	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_POSITION_CLASSIC))>=0) 
	theApp.m_LastSettings.nPositionGraphicMode & _CLASSIC ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_POSITION_STATUS_1))>=0) 
	theApp.m_LastSettings.nPositionGraphicMode & _STATUS ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);


	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_POSITION_MACHINE))>=0) 
	theApp.m_LastSettings.nPositionDisplayMode == 0 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_POSITION_HEAD))>=0) 
	theApp.m_LastSettings.nPositionDisplayMode == 1 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_POSITION_SURFACE))>=0) 
	theApp.m_LastSettings.nPositionDisplayMode == 2 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolBar.CommandToIndex(ID_BUTTON_POSITION_SURFACE_WATER))>=0) 
	theApp.m_LastSettings.nPositionDisplayMode == 3 ? m_wndToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolBar.SetButtonStyle(nIndex,0);


}


void CPositionGraphicalPage::OnButtonPositionClassic() 
{
	theApp.m_LastSettings.nPositionGraphicMode &= ~_STATUS;
	theApp.m_LastSettings.nPositionGraphicMode |= _CLASSIC;

	SetFontSize();
	SetToolBarCheckedState();
	
}

void CPositionGraphicalPage::OnButtonPositionStatus() 
{
	theApp.m_LastSettings.nPositionGraphicMode &= ~_CLASSIC;
	theApp.m_LastSettings.nPositionGraphicMode |= _STATUS;

	SetFontSize();
	SetToolBarCheckedState();
}

void CPositionGraphicalPage::OnButtonPositionMachine() 
{
	theApp.m_LastSettings.nPositionDisplayMode = 0;
	SetToolBarCheckedState();
}

void CPositionGraphicalPage::OnButtonPositionHead() 
{
	theApp.m_LastSettings.nPositionDisplayMode = 1;
	SetToolBarCheckedState();
}

void CPositionGraphicalPage::OnButtonPositionSurface() 
{
	theApp.m_LastSettings.nPositionDisplayMode = 2;
	SetToolBarCheckedState();
}

void CPositionGraphicalPage::OnButtonPositionSurfaceWater() 
{
	theApp.m_LastSettings.nPositionDisplayMode = 3;
	SetToolBarCheckedState();
}

void CPositionGraphicalPage::SetFontSize()
{
	CString Buff;
	CRect rr;
	LOGFONT lf;
	CSize size;
	CDC* pDC = GetDC();
	int nWidth=0;
	int nNumberAxes = FindNumberAxes();
	CString strName;
	float fHeight;
	CFont* pOldFont;
	int		nAxis;

	if (GetSafeHwnd() && m_staticView.GetSafeHwnd()) {

		m_staticView.GetWindowRect(&rr);
		ScreenToClient(&rr);

		lf = m_LogFont;
		lf.lfWeight = 550;
		if (theApp.m_LastSettings.nPositionGraphicMode & _CLASSIC) {
			SAFE_DELETE(m_pFont);
			_tcsncpy_s(lf.lfFaceName, LF_FACESIZE, _T("Times"), 7);

			switch (theApp.m_Tank.nScannerDescription) {
			default:
				fHeight = (float)(nNumberAxes / 2) + 0.8f;
				lf.lfHeight = (int)((float)rr.Height() / fHeight);
				m_pFont = new CFont();
				m_pFont->CreateFontIndirect(&lf);

				pOldFont = pDC->SelectObject(m_pFont);
				for (nAxis = 0; nAxis < nNumberAxes; nAxis++) {
					Buff.Format(L"%s:-99999.9mm", theApp.m_Axes[nAxis].cName);
					size = pDC->GetTextExtent(Buff);
					if (size.cx > nWidth) nWidth = size.cx;
				}
				pDC->SelectObject(pOldFont);

				if (nWidth > (rr.Width() / 2)) {

					pDC->SelectObject(pOldFont);

					lf.lfHeight = MulDiv(rr.Width() / 2, lf.lfHeight, nWidth);
					_tcsncpy_s(lf.lfFaceName, LF_FACESIZE, _T("Times"), 7);
					SAFE_DELETE(m_pFont);
					m_pFont = new CFont();
					m_pFont->CreateFontIndirect(&lf);

				}
				break;
			case SINGLE_BRIDGE_TT_ROLLERS:
			case SINGLE_BRIDGE_TANK_WITH_R:
			case SINGLE_BRIDGE_TANK:
				fHeight = (float)nNumberAxes + 0.2f;
				lf.lfHeight = (int)((float)rr.Height() / fHeight);
				m_pFont = new CFont();
				m_pFont->CreateFontIndirect(&lf);

				pOldFont = pDC->SelectObject(m_pFont);
				for (nAxis = 0; nAxis < nNumberAxes; nAxis++) {
					Buff.Format(L"%s:-99999.9mm", theApp.m_Axes[nAxis].cName);
					size = pDC->GetTextExtent(Buff);
					if (size.cx > nWidth) nWidth = size.cx;
				}
				pDC->SelectObject(pOldFont);

				if (nWidth > rr.Width()) {

					pDC->SelectObject(pOldFont);

					lf.lfHeight = MulDiv(rr.Width(), lf.lfHeight, nWidth);
					_tcsncpy_s(lf.lfFaceName, LF_FACESIZE, _T("Times"), 7);
					SAFE_DELETE(m_pFont);
					m_pFont = new CFont();
					m_pFont->CreateFontIndirect(&lf);

				}
				break;
			}
		}


		if (theApp.m_LastSettings.nPositionGraphicMode & _STATUS) {
			lf.lfHeight = (int)((float)rr.Height() / -10.8f);
			_tcsncpy_s(lf.lfFaceName, LF_FACESIZE, _T("Times"), 7);
			SAFE_DELETE(m_pFont);
			m_pFont = new CFont();
			m_pFont->CreateFontIndirect(&lf);

			CFont* pOldFont = pDC->SelectObject(m_pFont);

			nNumberAxes = 0;
			for (int nAxis = 0; nAxis < 12; nAxis++) {
				if ((theApp.m_Axes[nAxis].bAvailable) && (theApp.m_Axes[nAxis].nMotorType == SERVO)) {
					nNumberAxes++;
					Buff.Format(L" -9999.99mm");
					size = pDC->GetTextExtent(Buff);
					if (size.cx > nWidth) {
						strName = Buff;
						nWidth = size.cx;
					}
				}
			}

			if ((nWidth * nNumberAxes) > rr.Width()) {
				pDC->SelectObject(pOldFont);

				lf.lfHeight = MulDiv(rr.Width(), lf.lfHeight, (nWidth * nNumberAxes));
				_tcsncpy_s(lf.lfFaceName, LF_FACESIZE, _T("Times"), 7);
				SAFE_DELETE(m_pFont);
				m_pFont = new CFont();
				m_pFont->CreateFontIndirect(&lf);
			}
		}

	}
	m_nLongestNameLength = 0;
	ReleaseDC(pDC);

}

void CPositionGraphicalPage::RenderClassic(CDC *pDC, CRect *rr)
{
	CString Buff;
	int nXp[6],nYp;
	CString strName,strPos,strUnit;
	int nYStep = rr->Height() / 5;
	CString Unit[] = {L"mm", L"\x022", L"\x0b0"};
	CSize size;

	if(theApp.m_nSide0Orientation == 0) {
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
	strName.Format(L"%s:",theApp.m_Axes[theApp.m_Tank.nXLeft].cName);
	strPos.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPrecision,m_CpSurface.Side0.fX * theApp.m_fUnits, theApp.m_Units);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[0],&nYp);

	strName.Format(L"%s:",theApp.m_Axes[theApp.m_Tank.nXRight].cName);
	strPos.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXRight].nPrecision,m_CpSurface.Side1.fX * theApp.m_fUnits, theApp.m_Units);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[3],&nYp);
	nYp += nYStep;

	strName.Format(L"%s:",theApp.m_Axes[theApp.m_Tank.nYLeft].cName);
	strPos.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPrecision,m_CpSurface.Side0.fY * theApp.m_fUnits, theApp.m_Units);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[0],&nYp);

	strName.Format(L"%s:",theApp.m_Axes[theApp.m_Tank.nYRight].cName);
	strPos.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYRight].nPrecision,m_CpSurface.Side1.fY * theApp.m_fUnits, theApp.m_Units);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[3],&nYp);
	nYp += nYStep;

	strName.Format(L"%s:",theApp.m_Axes[theApp.m_Tank.nZLeft].cName);
	strPos.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPrecision,m_CpSurface.Side0.fZ * theApp.m_fUnits, theApp.m_Units);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[0],&nYp);

	strName.Format(L"%s:",theApp.m_Axes[theApp.m_Tank.nZRight].cName);
	pDC->TextOut(nXp[3],nYp,Buff);
	strPos.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZRight].nPrecision,m_CpSurface.Side1.fZ * theApp.m_fUnits, theApp.m_Units);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[3],&nYp);
	nYp += nYStep;

	strName.Format(L"%s:",theApp.m_Axes[theApp.m_Tank.nXtLeft].cName);
	strPos.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPrecision,m_CpSurface.Side0.fXt, Unit[theApp.m_Axes[theApp.m_Tank.nXtLeft].nUnitSelected]);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[0],&nYp);

	strName.Format(L"%s:",theApp.m_Axes[theApp.m_Tank.nXtRight].cName);
	strPos.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXtRight].nPrecision,m_CpSurface.Side1.fXt, Unit[theApp.m_Axes[theApp.m_Tank.nXtRight].nUnitSelected]);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[3],&nYp);
	nYp += nYStep;

	strName.Format(L"%s:",theApp.m_Axes[theApp.m_Tank.nYtLeft].cName);
	strPos.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPrecision,m_CpSurface.Side0.fYt, Unit[theApp.m_Axes[theApp.m_Tank.nYtLeft].nUnitSelected]);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[0],&nYp);

	strName.Format(L"%s:",theApp.m_Axes[theApp.m_Tank.nYtRight].cName);
	strPos.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYtRight].nPrecision,m_CpSurface.Side1.fYt, Unit[theApp.m_Axes[theApp.m_Tank.nYtRight].nUnitSelected]);
	TextOut(pDC,&strName,&strPos,&strUnit,&nXp[3],&nYp);
	nYp += nYStep;

}

void CPositionGraphicalPage::RenderClassicSingleBridgeTank(CDC *pDC, CRect *rr)
{
	CString Buff;
	int nXp[6],nYp;
	CString strName,strPos,strUnit;
	int nYStep = rr->Height() / 5;
	CString Unit[] = {L"mm", L"\x022", L"\x0b0"};
	CSize size;
	int nNumberAxes = FindNumberAxes();

	if(nNumberAxes == 0) return;

	nXp[0]=1;
	nXp[1]=rr->Width()/2 - 10;
	nXp[3]=rr->Width()/2;
	nXp[4]=rr->Width() -10;
	nYp=1;

	nYStep = rr->Height() / nNumberAxes;

	if(theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable) {
		strName.Format(L"%s:",theApp.m_Axes[theApp.m_Tank.nXLeft].cName);
		strPos.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXLeft].nPrecision,m_CpSurface.Side0.fX * theApp.m_fUnits, theApp.m_Units);
		TextOut(pDC,&strName,&strPos,&strUnit,&nXp[0],&nYp);
		nYp += nYStep;
	}

	if(theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable) {
		strName.Format(L"%s:",theApp.m_Axes[theApp.m_Tank.nYLeft].cName);
		strPos.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYLeft].nPrecision,m_CpSurface.Side0.fY * theApp.m_fUnits, theApp.m_Units);
		TextOut(pDC,&strName,&strPos,&strUnit,&nXp[0],&nYp);
		nYp += nYStep;
	}

	if(theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable) {
		strName.Format(L"%s:",theApp.m_Axes[theApp.m_Tank.nZLeft].cName);
		strPos.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nZLeft].nPrecision,m_CpSurface.Side0.fZ * theApp.m_fUnits, theApp.m_Units);
		TextOut(pDC,&strName,&strPos,&strUnit,&nXp[0],&nYp);
		nYp += nYStep;
	}

	if(theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable) {
		strName.Format(L"%s:",theApp.m_Axes[theApp.m_Tank.nXtLeft].cName);
		strPos.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nXtLeft].nPrecision,m_CpSurface.Side0.fXt * theApp.m_fUnits, theApp.m_Units);
		TextOut(pDC,&strName,&strPos,&strUnit,&nXp[0],&nYp);
		nYp += nYStep;
	}

	if(theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable) {
		strName.Format(L"%s:",theApp.m_Axes[theApp.m_Tank.nYtLeft].cName);
		strPos.Format(_T("%.*f %s"),theApp.m_Axes[theApp.m_Tank.nYtLeft].nPrecision,m_CpSurface.Side0.fYt * theApp.m_fUnits, theApp.m_Units);
		TextOut(pDC,&strName,&strPos,&strUnit,&nXp[0],&nYp);
		nYp += nYStep;
	}


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

	if(theApp.m_LastSettings.nFaroDisplayJustify == 0) {
		pDC->TextOut(nX+size.cx+10,*nYp,*strPos);
	} else {
		size = pDC->GetTextExtent(*strPos);
		pDC->TextOut(nXp[1]-size.cx,*nYp,*strPos);
	}
}

#define _RED	RGB(255,  0,  0)
#define _GREEN	RGB(  0,255,  0)
#define _BLACK	RGB(  0,  0,  0)
#define _YELLOW	RGB(  255,255,  0)

void CPositionGraphicalPage::RenderStatus(CDC *pDC, CRect *rr)
{
	CString Buff;
	int nNumberAxes = FindNumberAxes();
	int nWidth,nAxis,nXp,nn,nYStep;
	CSize size;

	if(nNumberAxes == 0) return;

	nWidth = rr->Width() / nNumberAxes;
	nYStep = rr->Height() / 10;

	if(theApp.m_nSide0Orientation == 0) {

		for(nAxis=0,nn=0;nAxis<12;nAxis++) {
			if((theApp.m_Axes[nAxis].bAvailable) && (theApp.m_Axes[nAxis].nMotorType == SERVO)) {

				nXp=nn * nWidth + nWidth / 2 ;

				RenderStatusColumn(pDC,nAxis,nXp,nYStep);
				nn++;
			}
		}
	} else {

		for(nAxis=5,nn=0;nAxis<10;nAxis++) {
			if((theApp.m_Axes[nAxis].bAvailable) && (theApp.m_Axes[nAxis].nMotorType == SERVO)) {

				nXp=nn * nWidth + nWidth / 2 ;

				RenderStatusColumn(pDC,nAxis,nXp,nYStep);
				nn++;
			}
		}
		for(nAxis=0;nAxis<5;nAxis++) {
			if((theApp.m_Axes[nAxis].bAvailable) && (theApp.m_Axes[nAxis].nMotorType == SERVO)) {

				nXp=nn * nWidth + nWidth / 2 ;

				RenderStatusColumn(pDC,nAxis,nXp,nYStep);
				nn++;
			}
		}

	}
}




void CPositionGraphicalPage::RenderStatusColumn(CDC* pDC,int nAxis,int nXp,int nYStep)
{ 
	CString Buff;

	pDC->SetTextColor(theApp.m_LastSettings.rgbFaroText);

	int	nYp = 1;
	Buff.Format(L" %s",theApp.m_Axes[nAxis].cName);
	CSize	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(nXp - size.cx / 2,nYp,Buff);
	nYp += nYStep;

	Buff.Format(_T("%.*f"),theApp.m_Axes[nAxis].nPrecision,(float)theApp.m_Axes[nAxis].nPos * theApp.m_Axes[nAxis].fStepSize);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(nXp - size.cx / 2,nYp,Buff);
	nYp += nYStep;

	switch(theApp.m_Tank.nScannerDescription) {
	default:

		theApp.m_Axes[nAxis].nStatus & PMAC_IN_POS	? 	pDC->SetTextColor(_YELLOW)	: pDC->SetTextColor(_BLACK);
		size = pDC->GetTextExtent(strInPos);
		pDC->TextOut(nXp - size.cx / 2,nYp,strInPos);
		nYp += nYStep;

		theApp.m_Axes[nAxis].nStatus & PMAC_HOMED	? 	pDC->SetTextColor(_YELLOW)	: pDC->SetTextColor(_BLACK);
		size = pDC->GetTextExtent(strHome);
		pDC->TextOut(nXp - size.cx / 2,nYp,strHome);
		nYp += nYStep;

		theApp.m_Axes[nAxis].nStatus & PMAC_AMPLIFIER_ENABLED	? 	pDC->SetTextColor(_YELLOW)	: pDC->SetTextColor(_BLACK);
		size = pDC->GetTextExtent(strEnable);
		pDC->TextOut(nXp - size.cx / 2,nYp,strEnable);
		nYp += nYStep;

		theApp.m_Axes[nAxis].nStatus & PMAC_MIN_SWITCH	? 	pDC->SetTextColor(_RED)	: pDC->SetTextColor(_BLACK);
		size = pDC->GetTextExtent(strMin);
		pDC->TextOut(nXp - size.cx / 2,nYp,strMin);
		nYp += nYStep;

		theApp.m_Axes[nAxis].nStatus & PMAC_MAX_SWITCH	? 	pDC->SetTextColor(_RED)	: pDC->SetTextColor(_BLACK);
		size = pDC->GetTextExtent(strMax);
		pDC->TextOut(nXp - size.cx / 2,nYp,strMax);
		nYp += nYStep;

		theApp.m_Axes[nAxis].nStatus & PMAC_SOFT_LIMIT	? 	pDC->SetTextColor(_RED)	: pDC->SetTextColor(_BLACK);
		size = pDC->GetTextExtent(strSoft);
		pDC->TextOut(nXp - size.cx / 2,nYp,strSoft);
		nYp += nYStep;

		theApp.m_Axes[nAxis].nStatus & PMAC_HARD_LIMIT	? 	pDC->SetTextColor(_RED)	: pDC->SetTextColor(_BLACK);
		size = pDC->GetTextExtent(strHard);
		pDC->TextOut(nXp - size.cx / 2,nYp,strHard);
		nYp += nYStep;

		theApp.m_Axes[nAxis].nStatus & PMAC_AMPLIFIER_FAULT	? 	pDC->SetTextColor(_RED)	: pDC->SetTextColor(_BLACK);
		size = pDC->GetTextExtent(strFault);
		pDC->TextOut(nXp - size.cx / 2,nYp,strFault);
		nYp += nYStep;
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
		theApp.m_Axes[nAxis].nStatus & FB_IN_POS	? 	pDC->SetTextColor(_YELLOW)	: pDC->SetTextColor(_BLACK);
		size = pDC->GetTextExtent(strInPos);
		pDC->TextOut(nXp - size.cx / 2,nYp,strInPos);
		nYp += nYStep;

		theApp.m_Axes[nAxis].nStatus & FB_HOMED	? 	pDC->SetTextColor(_YELLOW)	: pDC->SetTextColor(_BLACK);
		size = pDC->GetTextExtent(strHome);
		pDC->TextOut(nXp - size.cx / 2,nYp,strHome);
		nYp += nYStep;

		theApp.m_Axes[nAxis].nStatus & FB_AMPLIFIER_ENABLED	? 	pDC->SetTextColor(_YELLOW)	: pDC->SetTextColor(_BLACK);
		size = pDC->GetTextExtent(strEnable);
		pDC->TextOut(nXp - size.cx / 2,nYp,strEnable);
		nYp += nYStep;

		theApp.m_Axes[nAxis].nStatus & FB_MIN_SWITCH	? 	pDC->SetTextColor(_RED)	: pDC->SetTextColor(_BLACK);
		size = pDC->GetTextExtent(strMin);
		pDC->TextOut(nXp - size.cx / 2,nYp,strMin);
		nYp += nYStep;

		theApp.m_Axes[nAxis].nStatus & FB_MAX_SWITCH	? 	pDC->SetTextColor(_RED)	: pDC->SetTextColor(_BLACK);
		size = pDC->GetTextExtent(strMax);
		pDC->TextOut(nXp - size.cx / 2,nYp,strMax);
		nYp += nYStep;

		theApp.m_Axes[nAxis].nStatus & FB_SOFT_LIMIT	? 	pDC->SetTextColor(_RED)	: pDC->SetTextColor(_BLACK);
		size = pDC->GetTextExtent(strSoft);
		pDC->TextOut(nXp - size.cx / 2,nYp,strSoft);
		nYp += nYStep;

		theApp.m_Axes[nAxis].nStatus & FB_HARD_LIMIT	? 	pDC->SetTextColor(_RED)	: pDC->SetTextColor(_BLACK);
		size = pDC->GetTextExtent(strHard);
		pDC->TextOut(nXp - size.cx / 2,nYp,strHard);
		nYp += nYStep;

		theApp.m_Axes[nAxis].nStatus & FB_AMPLIFIER_FAULT	? 	pDC->SetTextColor(_RED)	: pDC->SetTextColor(_BLACK);
		size = pDC->GetTextExtent(strFault);
		pDC->TextOut(nXp - size.cx / 2,nYp,strFault);
		nYp += nYStep;
		break;
	}
}


void CPositionGraphicalPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu* menu = (CMenu *) new CMenu;
	CRect rr;

	m_staticView.GetWindowRect(rr);
	if(rr.PtInRect(point)) {
		if (menu->LoadMenu(IDR_RB_FARO_DLG_MENU)) {
			CMenu* pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);

		}
	}

	delete menu;
	
}

void CPositionGraphicalPage::OnFarocoordsRightjustify() 
{
	theApp.m_LastSettings.nFaroDisplayJustify = 1;
}

void CPositionGraphicalPage::OnFarocoordsLeftjustify() 
{
	theApp.m_LastSettings.nFaroDisplayJustify = 0;
}

void CPositionGraphicalPage::OnFarocoordsTextcolour() 
{
	CMFCColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbFaroText=dlg.GetColor();
	};
}

void CPositionGraphicalPage::OnFarocoordsBackgroundcolour() 
{
	CMFCColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbFaroBk=dlg.GetColor();
	};
}

int CPositionGraphicalPage::FindNumberAxes()
{
	int nNumberAxes = 0;
	int nAxis;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		for(nAxis=0;nAxis<12;nAxis++) {
			if((theApp.m_Axes[nAxis].bAvailable) && (theApp.m_Axes[nAxis].nMotorType == SERVO)) {
				nNumberAxes++;
			}
		}
		break;
	case SINGLE_BRIDGE_TT_ROLLERS:
	case SINGLE_BRIDGE_TANK_WITH_R:
	case SINGLE_BRIDGE_TANK:
		if(theApp.m_Axes[theApp.m_Tank.nXLeft].bAvailable) nNumberAxes++;
		if(theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable) nNumberAxes++;
		if(theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable) nNumberAxes++;
		if(theApp.m_Axes[theApp.m_Tank.nXtLeft].bAvailable) nNumberAxes++;
		if(theApp.m_Axes[theApp.m_Tank.nYtLeft].bAvailable) nNumberAxes++;
		if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable) nNumberAxes++;
		break;
	case DUAL_ROBOT_9_PLUS_9: nNumberAxes = 20;
		break;
	}

	return nNumberAxes;

}


void CPositionGraphicalPage::RenderStatusDualRobot9Plus9(CDC *pDC, CRect *rr)
{
	CString Buff;
	int nNumberAxes = FindNumberAxes();
	int nWidth, nAxis, nXp, nn, nYStep;
	CSize size;

	if (nNumberAxes == 0) return;

	swprintf_s(theApp.m_Axes[0].cName, 10, L"X");
	swprintf_s(theApp.m_Axes[1].cName, 10, L"Y");
	swprintf_s(theApp.m_Axes[2].cName, 10, L"Z");
	swprintf_s(theApp.m_Axes[3].cName, 10, L"1");
	swprintf_s(theApp.m_Axes[4].cName, 10, L"2");
	swprintf_s(theApp.m_Axes[5].cName, 10, L"3");
	swprintf_s(theApp.m_Axes[6].cName, 10, L"4");
	swprintf_s(theApp.m_Axes[7].cName, 10, L"5");
	swprintf_s(theApp.m_Axes[8].cName, 10, L"6");
	swprintf_s(theApp.m_Axes[9].cName, 10, L"Pump");
	swprintf_s(theApp.m_Axes[10].cName, 10, L"X");
	swprintf_s(theApp.m_Axes[11].cName, 10, L"Y");
	swprintf_s(theApp.m_Axes[12].cName, 10, L"Z");
	swprintf_s(theApp.m_Axes[13].cName, 10, L"1");
	swprintf_s(theApp.m_Axes[14].cName, 10, L"2");
	swprintf_s(theApp.m_Axes[15].cName, 10, L"3");
	swprintf_s(theApp.m_Axes[16].cName, 10, L"4");
	swprintf_s(theApp.m_Axes[17].cName, 10, L"5");
	swprintf_s(theApp.m_Axes[18].cName, 10, L"6");
	swprintf_s(theApp.m_Axes[19].cName, 10, L"Pump");

	nWidth = rr->Width() / nNumberAxes;
	nYStep = rr->Height() / 10;

	for (nAxis = 0, nn = 0; nAxis<nNumberAxes; nAxis++) {
		nXp = nn * nWidth + nWidth / 2;
		RenderStatusColumn(pDC, nAxis, nXp, nYStep);
		nn++;
	}

}
