// 3DEditTurnTablePage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "3DEditTurnTablePage.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	CARTESIAN		0
#define	CARTESIAN_POLAR	1
#define	POLAR			2

/////////////////////////////////////////////////////////////////////////////
// C3DEditTurnTablePage property page

IMPLEMENT_DYNCREATE(C3DEditTurnTablePage, CPropertyPage)

C3DEditTurnTablePage::C3DEditTurnTablePage() : CPropertyPage(C3DEditTurnTablePage::IDD)
{
	//{{AFX_DATA_INIT(C3DEditTurnTablePage)
	//}}AFX_DATA_INIT
	
	m_pArray = NULL;
	m_pEditSpinItem = NULL;
	m_nPaintMode = CARTESIAN;
	m_nZoomLevel = 100;
	m_fZoomLeft = 0.0f;
	m_fZoomTop = 0.0f;
	m_nEditWhich = TAUGHTLINES;

	PROFILE->m_nFastAxis = 0;
	PROFILE->m_nSlowAxis = 2;

}

C3DEditTurnTablePage::~C3DEditTurnTablePage()
{
	SAFE_DELETE( m_pArray );
}

void C3DEditTurnTablePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3DEditTurnTablePage)
	DDX_Control(pDX, IDC_COMBO_ALGORITHM, m_comboAlgorithm);
	DDX_Control(pDX, IDC_SPIN_SLOW_INCREMENT, m_spinSlowIncrement);
	DDX_Control(pDX, IDC_EDIT_SLOW_INCREMENT, m_editSlowIncrement);
	DDX_Control(pDX, IDC_CHECK_ZERO_DEGREE, m_checkZeroDegreeRotate);
	DDX_Control(pDX, IDC_COMBO_DESCRIPTION, m_comboDescription);
	DDX_Control(pDX, IDC_SPIN_LINE_NUMBER, m_spinLineNumber);
	DDX_Control(pDX, IDC_EDIT_LINE_NUMBER, m_editLineNumber);
	DDX_Control(pDX, IDC_COMBO_TIMESLOT, m_comboTimeSlot);
	DDX_Control(pDX, IDC_LIST_COORDS, m_listCoords);
	DDX_Control(pDX, IDC_EDIT_R_POS, m_editRPos);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_DIAMETER, m_editDiameter);
	DDX_Control(pDX, IDC_SPIN_DIAMETER, m_spinDiameter);
}


BEGIN_MESSAGE_MAP(C3DEditTurnTablePage, CPropertyPage)
	//{{AFX_MSG_MAP(C3DEditTurnTablePage)
	ON_WM_CTLCOLOR()
	ON_MESSAGE_VOID(WM_KICKIDLE, OnKickIdle)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_CW_MOVE, OnButtonCwMove)
	ON_COMMAND(ID_BUTTON_CCW_MOVE, OnButtonCcwMove)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NOM_DIAMETER, OnDeltaposSpinNomDiameter)
	ON_BN_CLICKED(IDC_BUTTON_ZERO, OnButtonZero)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_LIST_COORDS, OnClickListCoords)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COORDS, OnDblclkListCoords)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_COORDS, OnRclickListCoords)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_COORDS, OnGetdispinfoListCoords)
	ON_COMMAND(ID_VIEW_CROSSES, OnViewCrosses)
	ON_COMMAND(ID_VIEW_LINES, OnViewLines)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_BUTTON_CARTESIAN, OnButtonCartesian)
	ON_COMMAND(ID_BUTTON_POLAR, OnButtonPolar)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_TURNTABLE_GOTOLEFT, OnTurntableGotoleft)
	ON_COMMAND(ID_TURNTABLE_GOTORIGHT, OnTurntableGotoright)
	ON_COMMAND(ID_TURNTABLE_GOTOBOTH, OnTurntableGotoboth)
	ON_CBN_SELCHANGE(IDC_COMBO_TIMESLOT, OnSelchangeComboTimeslot)
	ON_COMMAND(ID_BUTTON_ECCENTRIC, OnButtonEccentric)
	ON_COMMAND(ID_TTPOLAR_ROTATETONEARESTPOINT, OnTtpolarRotatetonearestpoint)
	ON_COMMAND(ID_TTPOLAR_ROTATETOFURTHESTPOINT, OnTtpolarRotatetofurthestpoint)
	ON_COMMAND(ID_TTPOLAR_ROTATETOTHISANGLE, OnTtpolarRotatetothisangle)
	ON_COMMAND(ID_TTLIST_ADD, OnTtlistAdd)
	ON_COMMAND(ID_TTLIST_DELETE, OnTtlistDelete)
	ON_COMMAND(ID_TTPOLAR_ROTATETONOMINAL, OnTtpolarRotatetonominal)
	ON_COMMAND(ID_TTLIST_TRANSLATEVERTICALLYTOHERE, OnTtlistTranslateverticallytohere)
	ON_COMMAND(ID_TTLIST_INSERT, OnTtlistInsert)
	ON_COMMAND(ID_BUTTON_TAUGHT_LINE, OnButtonTaughtLine)
	ON_COMMAND(ID_BUTTON_SCAN_LINE, OnButtonScanLine)
	ON_COMMAND(ID_TTLIST_SORTINZ, OnTtlistSortinz)
	ON_COMMAND(ID_TTLIST_MODIFY, OnTtlistModify)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT_TO_LEFT, OnButtonRightToLeft)
	ON_EN_CHANGE(IDC_EDIT_LINE_NUMBER, OnChangeEditLineNumber)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LINE_NUMBER, OnDeltaposSpinLineNumber)
	ON_COMMAND(ID_TTLIST_NEWLINE, OnTtlistNewline)
	ON_COMMAND(ID_TTLIST_COPYLINE, OnTtlistCopyline)
	ON_COMMAND(ID_TTLIST_PASTELINE, OnTtlistPasteline)
	ON_COMMAND(ID_BUTTON_CARTESIAN_POLAR, OnButtonCartesianPolar)
	ON_COMMAND(ID_TTLIST_COMPUTEANDFILLPOLARVALUES, OnTtlistComputeandfillpolarvalues)
	ON_COMMAND(ID_TTLIST_GOTOLEFTPOLARCOORD, OnTtlistGotoleftpolarcoord)
	ON_COMMAND(ID_TTLIST_GOTORIGHTPOLARCOORD, OnTtlistGotorightpolarcoord)
	ON_COMMAND(ID_TTLIST_GOTOBOTHPOLARCOORD, OnTtlistGotobothpolarcoord)
	ON_CBN_SELCHANGE(IDC_COMBO_DESCRIPTION, OnSelchangeComboDescription)
	ON_COMMAND(ID_BUTTON_GOTO_ZERO_DEGREES, OnButtonGotoZeroDegrees)
	ON_BN_CLICKED(IDC_CHECK_ZERO_DEGREE, OnCheckZeroDegree)
	ON_COMMAND(ID_TTLIST_SORTINY, OnTtlistSortiny)
	ON_EN_CHANGE(IDC_EDIT_SLOW_INCREMENT, OnChangeEditSlowIncrement)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SLOW_INCREMENT, OnDeltaposSpinSlowIncrement)
	ON_COMMAND(ID_TTLIST_COPY, OnTtlistCopy)
	ON_COMMAND(ID_VIEW_XY, OnViewXy)
	ON_COMMAND(ID_VIEW_XZ, OnViewXz)
	ON_COMMAND(ID_VIEW_YZ, OnViewYz)
	ON_CBN_SELCHANGE(IDC_COMBO_ALGORITHM, OnSelchangeComboAlgorithm)
	ON_EN_CHANGE(IDC_EDIT_TT_CENTRE, OnChangeEditTtCentre)
	ON_COMMAND(ID_TTLIST_3DLOOKATTHISPOINT, OnTtlist3dlookatthispoint)
	ON_COMMAND(ID_TTLIST_3DLOOKATTTCENTER, OnTtlist3dlookatttcenter)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_MESSAGE(WM_IDLEUPDATECMDUI, SetToolBarCheckedState)
	ON_MESSAGE(UI_ITEMCHANGED,ItemChanged)
	ON_COMMAND(ID_TT_SORTINX, &C3DEditTurnTablePage::OnTtSortinx)
	ON_EN_CHANGE(IDC_EDIT_DIAMETER, &C3DEditTurnTablePage::OnEnChangeEditDiameter)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DIAMETER, &C3DEditTurnTablePage::OnDeltaposSpinDiameter)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DEditTurnTablePage message handlers
int C3DEditTurnTablePage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndFileToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndFileToolBar.LoadToolBar(IDR_FILE_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
//	nNumberButtons = m_wndFileToolBar.GetToolBarCtrl().GetButtonCount();
//	m_wndFileToolBar.GetItemRect(0, &temp);
//	m_wndFileToolBar.GetToolBarCtrl().SetButtonSize(CSize(temp.Width(), temp.Height()));

	if (!m_wndMotionToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndMotionToolBar.LoadToolBar(IDR_TT_MOTION_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndViewToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndViewToolBar.LoadToolBar(IDR_TT_VIEW_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()-10,30,SWP_SHOWWINDOW);


	m_wndRebar.AddBar(&m_wndFileToolBar);
	m_wndRebar.AddBar(&m_wndMotionToolBar);
	m_wndRebar.AddBar(&m_wndViewToolBar);

	return 0;
}

BOOL C3DEditTurnTablePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CString Buff;

	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);
	m_StaticPosList.Initialize(this, m_listCoords, FALSE, TRUE, TRUE, TRUE);

	m_StaticPosView.SetNewPos();
	m_StaticPosList.SetNewPos();
	
	m_Action[0].LoadString(IDS_Continuous);
	m_Action[1].LoadString(IDS_Segment);
	m_Action[2].LoadString(IDS_Jump);

	m_Style[0].LoadString(IDS_Linear);
	m_Style[1].LoadString(IDS_Spline);


	m_spinLineNumber.SetRange(0,30000);
	m_spinSlowIncrement.SetRange(0,200);
	m_spinDiameter.SetRange(0, 10000);

	Buff.LoadString(IDS_Rectilinear);	m_comboDescription.AddString(Buff);
	Buff.LoadString(IDS_Open_Rotary);	m_comboDescription.AddString(Buff);
	Buff.LoadString(IDS_Closed_Rotary);	m_comboDescription.AddString(Buff);
	Buff.LoadString(IDS_Circular);		m_comboDescription.AddString(Buff);

	Buff.LoadString(IDS_All_vertices);				m_comboAlgorithm.AddString(Buff);
	Buff.LoadString(IDS_remove_vertices_at_jumps);	m_comboAlgorithm.AddString(Buff);


	CreateColumns();
	FillList();
	
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH C3DEditTurnTablePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void C3DEditTurnTablePage::UpdateAllControls()
{
	if(GetSafeHwnd() == NULL) return;
	CString Buff;


	Buff.Format(_T("%.02f %s"), PROFILE->m_fComponentDiameter[0] * theApp.m_fUnits, theApp.m_Units);
	m_editDiameter.SetWindowText(Buff);
	m_editDiameter.SetReadOnly(PROFILE->m_bDiametersDervivedFromCoords);


	m_comboTimeSlot.ResetContent();
	for(int nTS=0;nTS<theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;nTS++) {
		m_comboTimeSlot.AddString(theApp.m_UtUser.m_TS[nTS].cName);
	}
	m_comboTimeSlot.SetCurSel(theApp.m_LastSettings.nEccentricTimeSlot);

	m_spinLineNumber.SetPos(theApp.m_nLine);
	Buff.Format(L"%d",theApp.m_nLine + 1);
	m_editLineNumber.SetWindowText(Buff);

	m_comboDescription.SetCurSel(PROFILE->m_nComponentDescription);

	m_checkZeroDegreeRotate.SetCheck(theApp.m_LastSettings.nRotateSurfaceCoordinates);

	Buff.Format(_T("%.*f %s"), theApp.m_LastSettings.nStepSizePrecision,PROFILE->m_fSlowIncrement * theApp.m_fUnits,theApp.m_Units);
	m_editSlowIncrement.SetWindowText(Buff);

	m_comboAlgorithm.SetCurSel(PROFILE->m_nComputeAlgorithm);

	SetToolBarCheckedState();
}

void C3DEditTurnTablePage::OnKickIdle()
{
	SetToolBarCheckedState();
}

void C3DEditTurnTablePage::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	
	m_StaticPosView.SetNewPos();
	m_StaticPosList.SetNewPos();
	Invalidate(FALSE);
	
}

void C3DEditTurnTablePage::OnPaint() 
{
	if(GetSafeHwnd() == NULL) return;
	CPaintDC dc(this); // device context for painting
	CRect Rect = m_StaticPosView.m_Rect;
	CRect		rr,rect;
	CDC			dcMem;
	CBitmap		bmp;
	static	int	nWidth;
	static	int	nHeight;
	static int nLoop=0;
	CString Buff;
	
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	if((m_pArray==NULL) || (rr.Width()-nWidth) || (rr.Height()-nHeight)) {
		SAFE_DELETE( m_pArray );
		m_pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
		nWidth=rr.Width();
		nHeight=rr.Height();
		memset(m_pArray,0xff,rr.Width()*sizeof(COLORREF)*rr.Height());
	}

	rect.left=0;
	rect.top=0;
	rect.right = rr.Width()-2;
	rect.bottom = rr.Height()-2;

	bmp.CreateCompatibleBitmap(&dc,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),m_pArray);
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	

	switch(m_nPaintMode) {
	case CARTESIAN:
	case CARTESIAN_POLAR:
		DrawPerspectiveArrows((CPaintDC*)&dcMem,rect);
		FindAllLimits();
		ChangeLimitsForAspectRatio(rect);
		DrawProjectedLines((CPaintDC*)&dcMem,rect,SCREEN,m_nEditWhich);
		DrawProjectedWaterJets((CPaintDC*)&dcMem,rect,SCREEN);
		break;
	case POLAR:
		DrawCircularGraticule((CPaintDC*)&dcMem,rect);
		DrawEccentric((CPaintDC*)&dcMem,rect);
		DrawPolarPosition((CPaintDC*)&dcMem,rect);
		break;
	};


	dc.BitBlt(rr.left+1,rr.top+1,rr.Width()-2,rr.Height()-2,&dcMem,0,0,SRCCOPY);


}

void C3DEditTurnTablePage::DrawCircularGraticule(CDC* pDC,CRect rr)
{
	CFont HorizontalFont;
	CFont VerticalFont;
	CPen penGrey(PS_SOLID,1,RGB(100,100,100));
	CString Buff = L"999.9";
	CSize size = pDC->GetTextExtent(Buff);
	int nRadius = rr.Height()/2;
	if(nRadius > rr.Width()/2) nRadius = rr.Width()/2;
	nRadius -= (size.cy * 2);
	CPoint pt = rr.CenterPoint();
	CRect rrCircle(pt.x-nRadius,pt.y-nRadius,pt.x+nRadius,pt.y+nRadius);


	CPen* pOldPen = pDC->SelectObject(&penGrey);
	pDC->SelectStockObject(HOLLOW_BRUSH);
	pDC->Ellipse(&rrCircle);

	pDC->MoveTo(pt.x-nRadius-10,pt.y);
	pDC->LineTo(pt.x+nRadius+10,pt.y);
	pDC->MoveTo(pt.x,pt.y-nRadius-10);
	pDC->LineTo(pt.x,pt.y+nRadius+10);

	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
	theApp.m_LastSettings.lfViewGraticule.lfEscapement=900;
	VerticalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);

	CFont* pOldFont = pDC->SelectObject(&HorizontalFont);
	pDC->SetBkMode(TRANSPARENT);

	Buff = "90.0";
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(pt.x-size.cx/2,pt.y+nRadius+2,Buff);

	Buff = "270.0";
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(pt.x-size.cx/2,pt.y-nRadius-2-size.cy,Buff);

	pDC->SelectObject(&VerticalFont);

	Buff = "0.0";
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(pt.x-nRadius-size.cx,pt.y+size.cx/2,Buff);

	Buff = "180.0";
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(pt.x+nRadius+2,pt.y+size.cx/2,Buff);

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);

}

void C3DEditTurnTablePage::DrawEccentric(CDC* pDC,CRect rr)
{
	if(theApp.m_Thread.m_CpEccentric.m_nCoordL<=0) return;
	CFont HorizontalFont;
	CFont VerticalFont;
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CString Buff = L"999.9";
	CSize size = pDC->GetTextExtent(Buff);
	int nRadius = rr.Height()/2;
	if(nRadius > rr.Width()/2) nRadius = rr.Width()/2;
	nRadius -= (size.cy * 2);
	CPoint pt = rr.CenterPoint();
	CPoint ptR;
	CRect rrCircle(pt.x-nRadius,pt.y-nRadius,pt.x+nRadius,pt.y+nRadius);

	CPolyCoord *pCp = &theApp.m_Thread.m_CpEccentric;
	float fWaterPath = 0.0f;
	for(int nn=0;nn<pCp->m_nCoordL;nn++) {
		fWaterPath += pCp->m_pCp[nn].fWp[0];
	}
	fWaterPath /= (float)pCp->m_nCoordL;

	CPen* pOldPen = pDC->SelectObject(&penBlue);

	for(int nn=0;nn<pCp->m_nCoordL;nn++) {
		float fAngle = (2.0f * PIf * (float)nn) / (float)pCp->m_nCoordL;
		ptR.x = (int)((float)nRadius * ((cosf(fAngle) * pCp->m_pCp[nn].fWp[0]) / -fWaterPath));
		ptR.y = (int)((float)nRadius * ((sinf(fAngle) * pCp->m_pCp[nn].fWp[0]) / fWaterPath));
		
		if(nn==0) {
			pDC->MoveTo(ptR.x+pt.x,ptR.y+pt.y);
		} else {
			pDC->LineTo(ptR.x+pt.x,ptR.y+pt.y);
		}
	}

	
	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
	theApp.m_LastSettings.lfViewGraticule.lfEscapement=900;
	VerticalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);

	CFont* pOldFont = pDC->SelectObject(&HorizontalFont);
	pDC->SetBkMode(TRANSPARENT);


	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);

}

void C3DEditTurnTablePage::DrawPolarPosition(CDC* pDC,CRect rr)
{
	CCoord Cp;
	CFont HorizontalFont;
	CFont VerticalFont;
	CPen penBlue(PS_SOLID,1,RGB(0,0,100));
	CString Buff = L"999.9";
	CSize size = pDC->GetTextExtent(Buff);
	int nRadius = rr.Height()/2;
	if(nRadius > rr.Width()/2) nRadius = rr.Width()/2;
	nRadius -= (size.cy * 2);
	CPoint pt = rr.CenterPoint();
	CPoint ptSurface;
	CRect rrCircle(pt.x-nRadius,pt.y-nRadius,pt.x+nRadius,pt.y+nRadius);


	CPen* pOldPen = pDC->SelectObject(&penBlue);

	theApp.m_Motors.GetSurfacePos(&Cp);
	float fAngle = (Cp.Side0.fR * 2 * PIf) / 360.0f;
	ptSurface.x = (int)((float)nRadius * cosf(fAngle) * -1.0f);
	ptSurface.y = (int)((float)nRadius * sinf(fAngle));

	pDC->MoveTo(pt.x,pt.y);
	pDC->LineTo(pt.x+ptSurface.x,pt.y+ptSurface.y);
	pDC->SelectObject(pOldPen);

	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
	CFont *pOldFont = pDC->SelectObject(&HorizontalFont);

	Buff.Format(_T("Closest point is %.*f%s off centre"),theApp.m_nUnitsPrecision,theApp.m_Thread.m_CpEccentricCentre.fWp[0]*theApp.m_fUnits,theApp.m_Units);
	pDC->TextOut(2,2,Buff);
	size = pDC->GetTextExtent(Buff);

	Buff.Format(_T("at %.02f%s"),theApp.m_Thread.m_CpEccentricCentre.Side0.fR,DEGREES);
	pDC->TextOut(2,size.cy+4,Buff);

	pDC->SelectObject(pOldFont);
}
BOOL C3DEditTurnTablePage::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
   LRESULT *pResult)
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
	case ID_VIEW_XY:  strTipText = "X Y";
		break;
	case ID_VIEW_YZ:  strTipText = "Y Z";
		break;
	case ID_VIEW_XZ:  strTipText = "X Z";
		break;
	case ID_BUTTON_CW_MOVE:  strTipText.LoadString(IDS_Clock_wise_move);
		break;
	case ID_BUTTON_CCW_MOVE:  strTipText.LoadString(IDS_Counter_clock_wise_move);
		break;
	case ID_BUTTON_ECCENTRIC:  strTipText.LoadString(IDS_Eccentric);
		break;
	case ID_BUTTON_GOTO_ZERO_DEGREES:  strTipText.LoadString(IDS_Goto_Zero_Degrees);
		break;
	case ID_VIEW_CROSSES:  strTipText.LoadString(IDS_Crosses);
		break;
	case ID_VIEW_LINES:  strTipText.LoadString(IDS_Lines);
		break;
	case ID_BUTTON_CARTESIAN:  strTipText.LoadString(IDS_Cartesian);
		break;
	case ID_BUTTON_CARTESIAN_POLAR:  strTipText.LoadString(IDS_Cartesian_Polar);
		break;
	case ID_BUTTON_POLAR:  strTipText.LoadString(IDS_Polar);
		break;
	case ID_BUTTON_TAUGHT_LINE:  strTipText.LoadString(IDS_Taught_Lines);
		break;
	case ID_BUTTON_SCAN_LINE:  strTipText.LoadString(IDS_Scan_Lines);
		break;
	}

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}

void C3DEditTurnTablePage::OnButtonCwMove() 
{
	if(theApp.m_Motors.IsRotateRunning() == false) {
		theApp.m_Motors.StartContinuousRotate(theApp.m_Tank.nRLeft,PROFILE->m_fScanSpeed, m_nRunningDir = 0);
	} else {
		theApp.m_Motors.StopContinuousRotate(theApp.m_Tank.nRLeft);
	}
//	SetToolBarCheckedState();
}

void C3DEditTurnTablePage::OnButtonCcwMove() 
{
	if(theApp.m_Motors.IsRotateRunning() == false) {
		theApp.m_Motors.StartContinuousRotate(theApp.m_Tank.nRLeft,PROFILE->m_fScanSpeed, m_nRunningDir = 1);
	} else {
		theApp.m_Motors.StopContinuousRotate(theApp.m_Tank.nRLeft);
	}	
//	SetToolBarCheckedState();
}

long C3DEditTurnTablePage::SetToolBarCheckedState(unsigned int nTemp, long nlTemp)
{
	int nIndex;

	if(theApp.m_nLogonLevelMask & theApp.m_cAccess[IDD_3D_EDIT_DIALOG]) {
		if((nIndex=m_wndMotionToolBar.CommandToIndex(ID_BUTTON_CW_MOVE))>=0) {
			if((theApp.m_PmacUser.IsRotateRunning() == true) && (m_nRunningDir == 0)) {
				m_wndMotionToolBar.SetButtonStyle(nIndex,TBBS_CHECKED);
			} else {
				m_wndMotionToolBar.SetButtonStyle(nIndex,0);
			}
		}

		if((nIndex=m_wndMotionToolBar.CommandToIndex(ID_BUTTON_CCW_MOVE))>=0) {
			if((theApp.m_PmacUser.IsRotateRunning() == true) && (m_nRunningDir == 1)) {
				m_wndMotionToolBar.SetButtonStyle(nIndex,TBBS_CHECKED);
			} else {
				m_wndMotionToolBar.SetButtonStyle(nIndex,0);
			}
		}
		if((nIndex=m_wndMotionToolBar.CommandToIndex(ID_BUTTON_ECCENTRIC))>=0) m_wndMotionToolBar.SetButtonStyle(nIndex,0 );

		if((nIndex=m_wndViewToolBar.CommandToIndex(ID_VIEW_CROSSES))>=0) {
			switch(m_nEditWhich) {
			case TAUGHTLINES: theApp.m_LastSettings.bDisplayTaughtCrosses==TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex,0);
				break;
			case SCANLINES: theApp.m_LastSettings.bDisplayScanCrosses==TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex,0);
				break;
			case BRAINLINES: theApp.m_LastSettings.bDisplayBrainCrosses==TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex,0);
				break;
			case ENVELOPELINES: theApp.m_LastSettings.bDisplayEnvelopeCrosses==TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex,0);
				break;
			}
		}
		if((nIndex=m_wndViewToolBar.CommandToIndex(ID_VIEW_LINES))>=0) {
			switch(m_nEditWhich) {
			case TAUGHTLINES: theApp.m_LastSettings.bDisplayTaughtLines==TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex,0);
				break;
			case SCANLINES: theApp.m_LastSettings.bDisplayScanLines==TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex,0);
				break;
			case BRAINLINES: theApp.m_LastSettings.bDisplayBrainLines==TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex,0);
				break;
			case ENVELOPELINES: theApp.m_LastSettings.bDisplayEnvelopeLines==TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex,0);
				break;
			}
		}
		if((nIndex=m_wndViewToolBar.CommandToIndex(ID_BUTTON_POLAR))>=0)
			m_nPaintMode == POLAR ? m_wndViewToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex,0);

		if((nIndex=m_wndViewToolBar.CommandToIndex(ID_BUTTON_CARTESIAN_POLAR))>=0)
			m_nPaintMode == CARTESIAN_POLAR ? m_wndViewToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex,0);

		if((nIndex=m_wndViewToolBar.CommandToIndex(ID_BUTTON_CARTESIAN))>=0)
			m_nPaintMode == CARTESIAN ? m_wndViewToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex,0);

		if((nIndex=m_wndViewToolBar.CommandToIndex(ID_BUTTON_TAUGHT_LINE))>=0)
			m_nEditWhich == TAUGHTLINES ? m_wndViewToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex,0);
		if((nIndex=m_wndViewToolBar.CommandToIndex(ID_BUTTON_SCAN_LINE))>=0)
			m_nEditWhich == SCANLINES ? m_wndViewToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex,0);


		nIndex=m_wndViewToolBar.CommandToIndex(ID_VIEW_XY);
		if(PROFILE->m_nFastAxis == 0 && PROFILE->m_nSlowAxis == 1) {
			m_wndViewToolBar.SetButtonStyle(nIndex,TBBS_CHECKED);
		} else {
			m_wndViewToolBar.SetButtonStyle(nIndex,0);
		}
		nIndex=m_wndViewToolBar.CommandToIndex(ID_VIEW_XZ);
		if(PROFILE->m_nFastAxis == 0 && PROFILE->m_nSlowAxis == 2) {
			m_wndViewToolBar.SetButtonStyle(nIndex,TBBS_CHECKED);
		} else {
			m_wndViewToolBar.SetButtonStyle(nIndex,0);
		}
		nIndex=m_wndViewToolBar.CommandToIndex(ID_VIEW_YZ);
		if(PROFILE->m_nFastAxis == 1 && PROFILE->m_nSlowAxis == 2) {
			m_wndViewToolBar.SetButtonStyle(nIndex,TBBS_CHECKED);
		} else {
			m_wndViewToolBar.SetButtonStyle(nIndex,0);
		}

	} else {

		if((nIndex=m_wndMotionToolBar.CommandToIndex(ID_BUTTON_CW_MOVE))>=0) m_wndMotionToolBar.SetButtonStyle(nIndex,TBBS_DISABLED );
		if((nIndex=m_wndMotionToolBar.CommandToIndex(ID_BUTTON_CCW_MOVE))>=0) m_wndMotionToolBar.SetButtonStyle(nIndex,TBBS_DISABLED );
		if((nIndex=m_wndMotionToolBar.CommandToIndex(ID_BUTTON_ECCENTRIC))>=0) m_wndMotionToolBar.SetButtonStyle(nIndex,TBBS_DISABLED );
		if((nIndex=m_wndMotionToolBar.CommandToIndex(ID_BUTTON_TAUGHT_LINE))>=0) m_wndMotionToolBar.SetButtonStyle(nIndex,TBBS_DISABLED );
		if((nIndex=m_wndMotionToolBar.CommandToIndex(ID_BUTTON_SCAN_LINE))>=0) m_wndMotionToolBar.SetButtonStyle(nIndex,TBBS_DISABLED );
	}

	return 0;
}


BOOL C3DEditTurnTablePage::OnSetActive() 
{
	
	CreateColumns();
	FillList();
	UpdateAllControls();

	SetTimer(1,200,NULL);

	return CPropertyPage::OnSetActive();
}

BOOL C3DEditTurnTablePage::OnKillActive() 
{
	

	KillTimer(1);
	
	return CPropertyPage::OnKillActive();
}

void C3DEditTurnTablePage::OnDeltaposSpinRotSpeedDegSec(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	PROFILE->m_fScanSpeed += ((float)pNMUpDown->iDelta * -0.1f);
	if(PROFILE->m_fScanSpeed<0.0f) PROFILE->m_fScanSpeed = 0.0f;
	if(PROFILE->m_fScanSpeed>1080.0f) PROFILE->m_fScanSpeed = 1080.0f;
	UpdateAllControls();
	


	*pResult = 0;
}

void C3DEditTurnTablePage::OnChangeEditNomDiameter() 
{
	CString Buff;
	float fTemp;

	m_editNominalDiameter.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;

	if(PROFILE->m_fComponentDiameter[0]-fTemp) {
		PROFILE->m_fComponentDiameter[0]=fTemp;
		switch(theApp.m_Tank.nScannerDescription) {
		default:
			break;
		case TWI:
			theApp.m_Kinematics.CalculateRollerKinematics(PROFILE->m_fComponentDiameter[0], true, true);
			break;
		}
	};

}

void C3DEditTurnTablePage::OnDeltaposSpinNomDiameter(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fComponentDiameter[0] -= ((float)pNMUpDown->iDelta * 0.1f);
	} else {
		PROFILE->m_fComponentDiameter[0] -= ((float)pNMUpDown->iDelta * 0.254f);
	}
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		break;
	case TWI:
		theApp.m_Kinematics.CalculateRollerKinematics(PROFILE->m_fComponentDiameter[0], true, true);
		break;
	}
	UpdateAllControls();
	
	*pResult = 0;
}


void C3DEditTurnTablePage::OnButtonZero() 
{
	switch(theApp.m_nMotorType) {
	default:
		theApp.m_Motors.ZeroAxis(theApp.m_Tank.nRLeft);
		theApp.m_Motors.ZeroAxis(theApp.m_Tank.nRRight);
		break;
	case FIELD_BUS:
		theApp.m_FBCtrl.Origin(ORIGIN_R_LEFT);
		break;
	}
	theApp.m_Axes[31].nOffset = 0;
	theApp.m_PmacUser.HomeAxisWithoutMoving(32);
}

void C3DEditTurnTablePage::OnTimer(UINT nIDEvent) 
{
	CCoord Cp;
	CString Buff;
	CString Unit[] = {L"mm", L"\x022", L"\x0b0"};
	static int nOldRevCounter=-1;
	static int nOldUseTurntableOrRollers;

	if(nOldUseTurntableOrRollers - theApp.m_nUseTurntableOrRollers) {
		m_CpSurface.Zero();
	}


	theApp.m_Motors.GetSurfacePos(&Cp);

	if(theApp.m_nUseTurntableOrRollers == 0) {
		if(Cp.Side0.fR - m_CpSurface.Side0.fR) {
			Buff.Format(_T("%.01f %s Rev:%d"),fmodf(Cp.Side0.fR,360.0f),Unit[2],theApp.m_Axes[theApp.m_Tank.nRLeft].nRevCounter);
			m_editRPos.SetWindowText(Buff);
			if(nOldRevCounter - theApp.m_Axes[theApp.m_Tank.nRLeft].nRevCounter) {
				Invalidate(false);
			}
			nOldRevCounter = theApp.m_Axes[theApp.m_Tank.nRLeft].nRevCounter;
		}
	} else {
		if(Cp.Side1.fR - m_CpSurface.Side1.fR) {
			Buff.Format(_T("%.01f %s Rev:%d"),fmodf(Cp.Side1.fR,360.0f),Unit[theApp.m_Axes[theApp.m_Tank.nRRight].nUnitSelected],theApp.m_Axes[theApp.m_Tank.nRRight].nRevCounter);
			m_editRPos.SetWindowText(Buff);
			if(nOldRevCounter - theApp.m_Axes[theApp.m_Tank.nRRight].nRevCounter) {
				Invalidate(false);
			}
			nOldRevCounter = theApp.m_Axes[theApp.m_Tank.nRRight].nRevCounter;

		}
	}

	if(m_nPaintMode == CARTESIAN) {
		float fX = m_CpSurface.Side0.fX-Cp.Side0.fX;
		float fY = m_CpSurface.Side0.fY-Cp.Side0.fY;
		float fZ = m_CpSurface.Side0.fZ-Cp.Side0.fZ;
		float fLength = sqrtf((fX*fX) + (fY*fY) + (fZ*fZ));
		fX = m_CpSurface.Side1.fX-Cp.Side1.fX;
		fY = m_CpSurface.Side1.fY-Cp.Side1.fY;
		fZ = m_CpSurface.Side1.fZ-Cp.Side1.fZ;
		fLength += sqrtf((fX*fX) + (fY*fY) + (fZ*fZ));

		if(fLength > 0.5) {
			Invalidate(FALSE);
		}
	}

	if(m_nPaintMode == POLAR) {
		if(theApp.m_nUseTurntableOrRollers == 0) {
			if(fabsf(m_CpSurface.Side0.fR - Cp.Side0.fR)>0.1f) {
				Invalidate(FALSE);
			}
		} else {
			if(fabsf(m_CpSurface.Side1.fR - Cp.Side1.fR)>0.1f) {
				Invalidate(FALSE);
			}
		}
	}




	m_CpSurface = Cp;
	nOldUseTurntableOrRollers = theApp.m_nUseTurntableOrRollers;
	
	CPropertyPage::OnTimer(nIDEvent);
}

void C3DEditTurnTablePage::InitializeEditLine()
{
	m_pEditLine=NULL;
	m_nEditLineL=0;

	switch(m_nEditWhich) {
	case TAUGHTLINES:
		m_pEditLine = PROFILE->m_TaughtLine;
		m_nEditLineL = PROFILE->m_nTaughtLineL;
		break;
	case SCANLINES:
		m_pEditLine = PROFILE->m_ScanLine;
		m_nEditLineL = PROFILE->m_nScanLineL;
		break;
	}
	if(theApp.m_nLine>=m_nEditLineL) theApp.m_nLine = m_nEditLineL-1;
	if(theApp.m_nLine<0) theApp.m_nLine = 0;
	if(m_pEditLine)
		if(theApp.m_nIndex>=m_pEditLine[theApp.m_nLine].m_nCoordL) theApp.m_nIndex=m_pEditLine[theApp.m_nLine].m_nCoordL-1;
	if(theApp.m_nIndex<0) theApp.m_nIndex = 0;

}

void C3DEditTurnTablePage::CreateColumns()
{
	CString ColumnName[] = { L"#",L"X", L"Y", L"Z", L"I",L"J",L"K",L"R ->",L"R",L"Mode",L"X", L"Y", L"Z", L"I",L"J",L"K",L"Gain",L"On TS",L"Style"};
	int ColumnWidth[] = { 36,50,50,50,50,50,50,50,50,80,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50};
	int nColumns = 19;

	int nColumnCount = (&m_listCoords.GetHeaderCtrl())->GetItemCount();

	for (int ii=0;ii < nColumnCount;ii++) {
		m_listCoords.DeleteColumn(0);
	}

	if(m_nPaintMode == CARTESIAN_POLAR) {
		ColumnName[1] = "Radius";
		ColumnName[2] = "Angle";
		ColumnName[3] = "Z";
		ColumnName[4] = "I";
		ColumnName[5] = "J";
		ColumnName[6] = "K";
		ColumnName[7] = "Radius";
		ColumnName[8] = "Angle";
		ColumnName[9] = "Z";
		ColumnName[10] = "I";
		ColumnName[11] = "J";
		ColumnName[12] = "K";
		nColumns = 13;
	}

	for (int ii=0;ii<nColumns;ii++) m_listCoords.InsertColumn(ii,ColumnName[ii], LVCFMT_CENTER,ColumnWidth[ii]);
	m_listCoords.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

void C3DEditTurnTablePage::FillList()
{
	CString Buff,Which;
	int	ii;
	int nCoordL = 0;

	InitializeEditLine();

	int nCount = m_listCoords.GetItemCount();
	if(m_pEditLine) nCoordL = m_pEditLine[theApp.m_nLine].m_nCoordL;

	if(nCount<nCoordL) {
		for (int ii=nCount;ii<nCoordL;ii++) {
			Buff.Format(_T("%d"),ii+1);
			m_listCoords.InsertItem(ii, Buff.GetBuffer(255), ii);
		};
	} else {
		for(ii=nCount-1;ii>=nCoordL;ii--) {
			m_listCoords.DeleteItem(ii);
		}
	}
	m_listCoords.EnsureVisible(theApp.m_nIndex,FALSE);
	for(ii=0;ii<nCoordL;ii++)
		m_listCoords.SetItemState(ii , 0,LVIS_SELECTED);
	m_listCoords.SetItemState(theApp.m_nIndex , LVIS_SELECTED,LVIS_SELECTED);

}

void C3DEditTurnTablePage::OnClickListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	CloseEditSpin();

	theApp.m_nIndex = pDispInfo->item.mask;
	Invalidate(FALSE);
	
	*pResult = 0;
}


void C3DEditTurnTablePage::OnDblclkListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;
	CCoord* pCp = NULL;

	m_listCoords.GetWindowRect(CtrlRect);
	m_listCoords.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	CloseEditSpin();

	if(m_nEditWhich != TAUGHTLINES) return;

	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**)&m_pEditSpinItem,&Rect);


	theApp.m_nIndex = pDispInfo->item.mask;
	if(m_pEditLine==NULL) return;
	pCp = (CCoord*)&m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex];

	if(pCp) {
		switch(m_nColumn = pDispInfo->item.iItem) {
		case 1:
			m_fEdit = m_fEditInitialValue = pCp->Side0.fX * theApp.m_fUnits;
			theApp.m_LastSettings.nMeasurementUnits == 0 ?
				m_pEditSpinItem->Initialize(&m_fEdit,-10000.0f,10000.0f,1.0f,_T("%.01f")) :
				m_pEditSpinItem->Initialize(&m_fEdit,-10000.0f,10000.0f,0.001f,_T("%.03f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 2:
			m_fEdit = m_fEditInitialValue = pCp->Side0.fY * theApp.m_fUnits;
			theApp.m_LastSettings.nMeasurementUnits == 0 ?
				m_pEditSpinItem->Initialize(&m_fEdit,-10000.0f,10000.0f,1.0f,_T("%.01f")) :
				m_pEditSpinItem->Initialize(&m_fEdit,-10000.0f,10000.0f,0.001f,_T("%.03f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 3:
			m_fEdit = m_fEditInitialValue = pCp->Side0.fZ * theApp.m_fUnits;
			theApp.m_LastSettings.nMeasurementUnits == 0 ?
				m_pEditSpinItem->Initialize(&m_fEdit,-10000.0f,10000.0f,1.0f,_T("%.01f")) :
				m_pEditSpinItem->Initialize(&m_fEdit,-10000.0f,10000.0f,0.001f,_T("%.03f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 4:
			m_fEditInitialValue = pCp->Side0.fI;
			m_pEditSpinItem->Initialize(&pCp->Side0.fI,-1.0f,1.0f,0.01f,_T("%.02f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 5:
			m_fEditInitialValue = pCp->Side0.fJ;
			m_pEditSpinItem->Initialize(&pCp->Side0.fJ,-1.0f,1.0f,0.01f,_T("%.02f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 6:
			m_fEditInitialValue = pCp->Side0.fK;
			m_pEditSpinItem->Initialize(&pCp->Side0.fK,-1.0f,1.0f,0.01f,_T("%.02f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 7:
			m_fEditInitialValue = pCp->fAngle[0];
			m_pEditSpinItem->Initialize(&pCp->fAngle[0],0.0f,360.0f,0.1f,_T("%.01f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 8:
			m_fEditInitialValue = pCp->fAngle[1];
			m_pEditSpinItem->Initialize(&pCp->fAngle[1],0.0f,360.0f,0.1f,_T("%.01f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 9:
			m_nEditType = pCp->nType & 0x03;
			m_pEditSpinItem->Initialize(&m_nEditType,m_Action,3);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 10:
			m_fEdit = m_fEditInitialValue = pCp->Side1.fX * theApp.m_fUnits;
			theApp.m_LastSettings.nMeasurementUnits == 0 ?
				m_pEditSpinItem->Initialize(&m_fEdit,-10000.0f,10000.0f,1.0f,_T("%.01f")) :
				m_pEditSpinItem->Initialize(&m_fEdit,-10000.0f,10000.0f,0.001f,_T("%.03f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 11:
			m_fEdit = m_fEditInitialValue = pCp->Side1.fY * theApp.m_fUnits;
			theApp.m_LastSettings.nMeasurementUnits == 0 ?
				m_pEditSpinItem->Initialize(&m_fEdit,-10000.0f,10000.0f,1.0f,_T("%.01f")) :
				m_pEditSpinItem->Initialize(&m_fEdit,-10000.0f,10000.0f,0.001f,_T("%.03f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 12:
			m_fEdit = m_fEditInitialValue = pCp->Side1.fZ * theApp.m_fUnits;
			theApp.m_LastSettings.nMeasurementUnits == 0 ?
				m_pEditSpinItem->Initialize(&m_fEdit,-10000.0f,10000.0f,1.0f,_T("%.01f")) :
				m_pEditSpinItem->Initialize(&m_fEdit,-10000.0f,10000.0f,0.001f,_T("%.03f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 13:
			m_fEditInitialValue = pCp->Side1.fI;
			m_pEditSpinItem->Initialize(&pCp->Side1.fI,-1.0f,1.0f,0.01f,_T("%.02f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 14:
			m_fEditInitialValue = pCp->Side1.fJ;
			m_pEditSpinItem->Initialize(&pCp->Side1.fJ,-1.0f,1.0f,0.01f,_T("%.02f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 15:
			m_fEditInitialValue = pCp->Side1.fK;
			m_pEditSpinItem->Initialize(&pCp->Side1.fK,-1.0f,1.0f,0.01f,_T("%.02f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 16:
			m_fEdit = m_fEditInitialValue = (float)pCp->nGain[0] * 10.0f;
			m_pEditSpinItem->Initialize(&m_fEdit,-30.0,30.0,0.5,_T("%.01fdB"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 17:
			m_nEdit = m_nEditInitialValue = pCp->nTS + 1;
			m_pEditSpinItem->Initialize(&m_nEdit,1,8,1);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		case 18:
			pCp->nType & TYPE_SPLINE ? m_nEditType = 1 : m_nEditType = 0;
			m_pEditSpinItem->Initialize(&m_nEditType,m_Style,2);
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;

		case 999:
			m_fEdit = m_fEditInitialValue = pCp->fThickness * theApp.m_fUnits;
			theApp.m_LastSettings.nMeasurementUnits == 0 ?
				m_pEditSpinItem->Initialize(&m_fEdit,0.0f,1000.0f,1.0f,_T("%.01f")) :
				m_pEditSpinItem->Initialize(&m_fEdit,0.0f,1000.0f,0.001f,_T("%.03f"));
			m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
			break;
		}
	}

	
	*pResult = 0;
}

HRESULT C3DEditTurnTablePage::ItemChanged(WPARAM,LPARAM)
{
	CCoord* pCp = (CCoord*)&m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex];

	switch(m_nColumn) {
	case 1:	pCp->Side0.fX = m_fEdit / theApp.m_fUnits;
		break;
	case 2:	pCp->Side0.fY = m_fEdit / theApp.m_fUnits;
		break;
	case 3:	pCp->Side0.fZ = m_fEdit / theApp.m_fUnits;
		break;
	case 9:	pCp->nType = (pCp->nType & 0xfff0) | m_nEditType;
		break;
	case 10:	pCp->Side1.fX = m_fEdit / theApp.m_fUnits;
		break;
	case 11:	pCp->Side1.fY = m_fEdit / theApp.m_fUnits;
		break;
	case 12:	pCp->Side1.fZ = m_fEdit / theApp.m_fUnits;
		break;
	case 16:	pCp->nGain[0] = (int)(m_fEdit * 10.0f);
		break;
	case 17:	pCp->nTS = (int)m_nEdit - 1;
		break;
	case 18:
		m_nEditType & 1 ? pCp->nType |= TYPE_SPLINE : pCp->nType &= ~TYPE_SPLINE;
		break;
	case 999:	pCp->fThickness = m_fEdit / theApp.m_fUnits;
		break;
	}
	GenerateScanLines();
	Invalidate(FALSE);
	return NULL;
}

void C3DEditTurnTablePage::OnRclickListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu* menu = (CMenu *) new CMenu;
	CMenu *pPopup;
	CPoint	Point;
	CString Buff,Language;

	POSITION pos = m_listCoords.GetFirstSelectedItemPosition();
	theApp.m_nIndex=m_listCoords.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_RB_TT_LIST_MENU)) {
		switch(m_nEditWhich) {
		case TAUGHTLINES:
			pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);
			break;
		case SCANLINES:
			break;
		}
	}
	delete menu;

	*pResult = 0;
}

void C3DEditTurnTablePage::OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{

	switch(m_nPaintMode) {
	case CARTESIAN: OnGetdispinfoListCoordsCartesian(pNMHDR,pResult);
		break;
	case CARTESIAN_POLAR: OnGetdispinfoListCoordsPolar(pNMHDR,pResult);
		break;
	case POLAR: OnGetdispinfoListCoordsPolar(pNMHDR,pResult);
		break;
	}
}


void C3DEditTurnTablePage::OnGetdispinfoListCoordsCartesian(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CCoord* pCp;
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];


	str[0]=0;
	pDispInfo->item.pszText=str;

	InitializeEditLine();

	if((m_pEditLine==NULL) || (m_nEditLineL<=0)) {
		return;
	}
	if(theApp.m_nLine>=m_nEditLineL) return;

	pCp = (CCoord*)&m_pEditLine[theApp.m_nLine].m_pCp[pDispInfo->item.iItem];

	switch(pDispInfo->item.iSubItem) {
	case 0:	swprintf_s(str,100,_T("%d"),pDispInfo->item.iItem+1);
		break;
	case 1:	swprintf_s(str,100,_T("%.*f"),theApp.m_nUnitsPrecision,pCp->Side[PORTSIDE].fX*theApp.m_fUnits);
		break;
	case 2:	swprintf_s(str,100,_T("%.*f"),theApp.m_nUnitsPrecision,pCp->Side[PORTSIDE].fY*theApp.m_fUnits);
		break;
	case 3:	swprintf_s(str,100,_T("%.*f"),theApp.m_nUnitsPrecision,pCp->Side[PORTSIDE].fZ*theApp.m_fUnits);
		break;
	case 4:	swprintf_s(str,100,_T("%.03f"),pCp->Side[PORTSIDE].fI);
		break;
	case 5:	swprintf_s(str,100,_T("%.03f"),pCp->Side[PORTSIDE].fJ);
		break;
	case 6:	swprintf_s(str,100,_T("%.03f"),pCp->Side[PORTSIDE].fK);
		break;
	case 7:	if((pCp->nType & 0x03)!=0) swprintf_s(str,100,_T("%.01f%s"),pCp->fAngle[PORTSIDE],(LPCWSTR)DEGREES);
		break;
	case 8:	if((pCp->nType & 0x03)!=0) swprintf_s(str,100,_T("%.01f%s"),pCp->fAngle[STARBOARD], (LPCWSTR)DEGREES);
		break;
	case 9:;
		swprintf_s(str,100,_T("%s"), (LPCWSTR)m_Action[(pCp->nType & 0x03) % 3]);
		break;
	case 10:	swprintf_s(str,100,_T("%.*f"),theApp.m_nUnitsPrecision,pCp->Side[STARBOARD].fX*theApp.m_fUnits);
		break;
	case 11:	swprintf_s(str,100,_T("%.*f"),theApp.m_nUnitsPrecision,pCp->Side[STARBOARD].fY*theApp.m_fUnits);
		break;
	case 12:	swprintf_s(str,100,_T("%.*f"),theApp.m_nUnitsPrecision,pCp->Side[STARBOARD].fZ*theApp.m_fUnits);
		break;
	case 13:	swprintf_s(str,100,_T("%.03f"),pCp->Side[STARBOARD].fI);
		break;
	case 14:	swprintf_s(str,100,_T("%.03f"),pCp->Side[STARBOARD].fJ);
		break;
	case 15:	swprintf_s(str,100,_T("%.03f"),pCp->Side[STARBOARD].fK);
		break;
	case 16:	swprintf_s(str,100,_T("%.1fdB"),(float)pCp->nGain[0]/10.0f);
		break;
	case 17:	swprintf_s(str,100,_T("%d"),pCp->nTS + 1);
		break;
	case 18:
		if(pCp->nType & TYPE_SPLINE) {
			swprintf_s(str,100,_T("%s"), (LPCWSTR)m_Style[1]);
		} else {
			swprintf_s(str,100,_T("%s"), (LPCWSTR)m_Style[0]);
		}
		break;
	case 999:	swprintf_s(str,100,_T("%.*f"),theApp.m_nUnitsPrecision,pCp->fThickness*theApp.m_fUnits);
		break;
	}

	
	*pResult = 0;
}

void C3DEditTurnTablePage::OnGetdispinfoListCoordsPolar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CCoord* pCp;
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];


	str[0]=0;
	pDispInfo->item.pszText=str;

	InitializeEditLine();

	if((m_pEditLine==NULL) || (m_nEditLineL<=0)) {
		return;
	}
	if(theApp.m_nLine>=m_nEditLineL) return;

	pCp = (CCoord*)&m_pEditLine[theApp.m_nLine].m_pCp[pDispInfo->item.iItem];

	switch(pDispInfo->item.iSubItem) {
	case 0:	swprintf_s(str,100,_T("%d"),pDispInfo->item.iItem+1);
		break;
	case 1:	swprintf_s(str,100,_T("%.*f"),theApp.m_nUnitsPrecision,pCp->fRadius[PORTSIDE]*theApp.m_fUnits);
		break;
	case 2:	 swprintf_s(str,100,_T("%.01f%s"),pCp->Side[PORTSIDE].fR, (LPCWSTR)DEGREES);
		break;
	case 3:	swprintf_s(str,100,_T("%.*f"),theApp.m_nUnitsPrecision,pCp->Side[PORTSIDE].fZ*theApp.m_fUnits);
		break;
	case 4:	swprintf_s(str,100,_T("%.02f"),pCp->Side[PORTSIDE].fI);
		break;
	case 5:	swprintf_s(str,100,_T("%.02f"),pCp->Side[PORTSIDE].fJ);
		break;
	case 6:	swprintf_s(str,100,_T("%.02f"),pCp->Side[PORTSIDE].fK);
		break;
	case 7:	swprintf_s(str,100,_T("%.*f"),theApp.m_nUnitsPrecision,pCp->fRadius[STARBOARD]*theApp.m_fUnits);
		break;
	case 8:	 swprintf_s(str,100,_T("%.01f%s"),pCp->Side[STARBOARD].fR, (LPCWSTR)DEGREES);
		break;
	case 9:	swprintf_s(str,100,_T("%.*f"),theApp.m_nUnitsPrecision,pCp->Side[STARBOARD].fZ*theApp.m_fUnits);
		break;
	case 10:	swprintf_s(str,100,_T("%.02f"),pCp->Side[STARBOARD].fI);
		break;
	case 11:	swprintf_s(str,100,_T("%.02f"),pCp->Side[STARBOARD].fJ);
		break;
	case 12:	swprintf_s(str,100,_T("%.02f"),pCp->Side[STARBOARD].fK);
		break;
	case 13:	swprintf_s(str,100,_T("%.1fdB"),(float)pCp->nGain[0]/10.0f);
		break;
	case 14:	swprintf_s(str,100,_T("%d"),pCp->nTS + 1);
		break;
	case 999:	swprintf_s(str,100,_T("%.*f"),theApp.m_nUnitsPrecision,pCp->fThickness*theApp.m_fUnits);
		break;
	}

	
	*pResult = 0;
}

void C3DEditTurnTablePage::CloseEditSpin()
{
	SAFE_DELETE( m_pEditSpinItem );
}

void C3DEditTurnTablePage::FindAllLimits()
{
	COORDDATA	CpTopLeft,CpBottomRight;

	PROFILE->FindAllLineLimits(&CpTopLeft,&CpBottomRight);
	m_fX0p = CpTopLeft.Side[PORTSIDE].fX - (CpBottomRight.Side[PORTSIDE].fX - CpTopLeft.Side[PORTSIDE].fX) * 0.1f;
	m_fX1p = CpBottomRight.Side[PORTSIDE].fX + (CpBottomRight.Side[PORTSIDE].fX - CpTopLeft.Side[PORTSIDE].fX) * 0.1f;
	m_fY0p = CpTopLeft.Side[PORTSIDE].fY - (CpBottomRight.Side[PORTSIDE].fY - CpTopLeft.Side[PORTSIDE].fY) * 0.1f;
	m_fY1p = CpBottomRight.Side[PORTSIDE].fY + (CpBottomRight.Side[PORTSIDE].fY - CpTopLeft.Side[PORTSIDE].fY) * 0.1f;
	m_fZ0p = CpTopLeft.Side[PORTSIDE].fZ - (CpBottomRight.Side[PORTSIDE].fZ - CpTopLeft.Side[PORTSIDE].fZ) * 0.1f;
	m_fZ1p = CpBottomRight.Side[PORTSIDE].fZ + (CpBottomRight.Side[PORTSIDE].fZ - CpTopLeft.Side[PORTSIDE].fZ) * 0.1f;

	m_fI0p = CpTopLeft.Side[PORTSIDE].fI - (CpBottomRight.Side[PORTSIDE].fI - CpTopLeft.Side[PORTSIDE].fI) * 0.1f;
	m_fI1p = CpBottomRight.Side[PORTSIDE].fI + (CpBottomRight.Side[PORTSIDE].fI - CpTopLeft.Side[PORTSIDE].fI) * 0.1f;
	m_fJ0p = CpTopLeft.Side[PORTSIDE].fJ - (CpBottomRight.Side[PORTSIDE].fJ - CpTopLeft.Side[PORTSIDE].fJ) * 0.1f;
	m_fJ1p = CpBottomRight.Side[PORTSIDE].fJ + (CpBottomRight.Side[PORTSIDE].fJ - CpTopLeft.Side[PORTSIDE].fJ) * 0.1f;
	m_fK0p = CpTopLeft.Side[PORTSIDE].fK - (CpBottomRight.Side[PORTSIDE].fK - CpTopLeft.Side[PORTSIDE].fK) * 0.1f;
	m_fK1p = CpBottomRight.Side[PORTSIDE].fK + (CpBottomRight.Side[PORTSIDE].fK - CpTopLeft.Side[PORTSIDE].fK) * 0.1f;

	if(fabs(m_fX0p-m_fX1p) < 10.0f) {
		m_fX0p-=5.0f;
		m_fX1p+=5.0f;
	}
	if(fabs(m_fY0p-m_fY1p) < 10.0f) {
		m_fY0p-=5.0f;
		m_fY1p+=5.0f;
	}
	if(fabs(m_fZ0p-m_fZ1p) < 10.0f) {
		m_fZ0p-=5.0f;
		m_fZ1p+=5.0f;
	}

	m_fX0p -= m_fZoomLeft;
	m_fX1p -= m_fZoomLeft;
	m_fY0p += m_fZoomTop;
	m_fY1p += m_fZoomTop;
	m_fZ0p += m_fZoomTop;
	m_fZ1p += m_fZoomTop;
	m_fX1p = ((m_fX1p-m_fX0p) * (float)m_nZoomLevel) / 100.0f + m_fX0p;
	m_fY1p = ((m_fY1p-m_fY0p) * (float)m_nZoomLevel) / 100.0f + m_fY0p;
	m_fZ1p = ((m_fZ1p-m_fZ0p) * (float)m_nZoomLevel) / 100.0f + m_fZ0p;

	m_fI0p = -1.0f;
	m_fI1p = 1.0f;
	m_fJ0p = -1.0f;
	m_fJ1p = 1.0f;
	m_fK0p = -1.0f;
	m_fK1p = 1.0f;

	m_fR0p = -180.0f;
	m_fR1p = 180.0f;
}

void C3DEditTurnTablePage::ChangeLimitsForAspectRatio(CRect rrView)
{
	float fComponentWidth;
	float fScreenRatio = (float)rrView.Height() / (float)rrView.Width();
	float fXC;

	return;
	switch(PROFILE->m_nFastAxis) {
	case 0:
		switch(PROFILE->m_nSlowAxis) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			fComponentWidth = (m_fZ1p-m_fZ0p) / fScreenRatio;
			fXC = (m_fX1p+m_fX0p) / 2.0f;
			m_fX1p = fXC + fComponentWidth / 2.0f;
			m_fX0p = fXC - fComponentWidth / 2.0f;
			break;
		}
		break;
	case 1:
		switch(PROFILE->m_nSlowAxis) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		}
		break;
	case 2:
		switch(PROFILE->m_nSlowAxis) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		}
		break;
	case 3:
		switch(PROFILE->m_nSlowAxis) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			fComponentWidth = (m_fZ1p-m_fZ0p) / fScreenRatio;
			fXC = (m_fX1p+m_fX0p) / 2.0f;
			m_fX1p = fXC + fComponentWidth / 2.0f;
			m_fX0p = fXC - fComponentWidth / 2.0f;
			break;
		}
		break;
	}
}

void C3DEditTurnTablePage::DrawProjectedLines(CPaintDC *pDC, CRect rr, int nDevice,int nWhichLines)
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRgn rgn;
	CRect Rect;
	CPoint	pt,ptPrevious,ptStart;
	CString Buff;
	int	nLineL,nStep,nStartLine,nFinishLine,nSide;
	CPolyCoord *pLine=NULL;
	COLORREF rgbDraw;
	bool bDisplayCrosses,bDisplayLines;
	CCoord Cp;
	bool bJoinUpLastPoint=FALSE;
	CString Line,Index;
	Line.LoadString(IDS_Line);
	Index.LoadString(IDS_Index);
	CRectFloat rrFloat = rr;


	switch(nWhichLines) {
	case TAUGHTLINES:
		if(PROFILE->m_nTaughtLineL<=0) return;
		if((theApp.m_LastSettings.bDisplayTaughtLines!=TRUE) && (theApp.m_LastSettings.bDisplayTaughtCrosses!=TRUE)) return;
		bDisplayCrosses = theApp.m_LastSettings.bDisplayTaughtCrosses;
		bDisplayLines = theApp.m_LastSettings.bDisplayTaughtLines;
		nLineL = PROFILE->m_nTaughtLineL;
		rgbDraw = theApp.m_LastSettings.rgbDisplayTaughtColor;
		nStep = 1;
		break;
	case SCANLINES:
		if(PROFILE->m_nScanLineL<=0) return;
		if((theApp.m_LastSettings.bDisplayScanLines!=TRUE) && (theApp.m_LastSettings.bDisplayScanCrosses!=TRUE)) return;
		bDisplayCrosses = theApp.m_LastSettings.bDisplayScanCrosses;
		bDisplayLines = theApp.m_LastSettings.bDisplayScanLines;
		nLineL = PROFILE->m_nScanLineL;
		rgbDraw = theApp.m_LastSettings.rgbDisplayScanColor;
		nStep = 1;
		break;
	case BRAINLINES:
		if(PROFILE->m_nBrainLineL<=0) return;
		if((theApp.m_LastSettings.bDisplayBrainLines!=TRUE) && (theApp.m_LastSettings.bDisplayBrainCrosses!=TRUE)) return;
		bDisplayCrosses = theApp.m_LastSettings.bDisplayBrainCrosses;
		bDisplayLines = theApp.m_LastSettings.bDisplayBrainLines;
		nLineL = PROFILE->m_nBrainLineL;
		rgbDraw = theApp.m_LastSettings.rgbDisplayBrainColor;
		nStep = 1;
		break;
	case ENVELOPELINES:
		if(PROFILE->m_nEnvelopeLineL<=0) return;
		if((theApp.m_LastSettings.bDisplayEnvelopeLines!=TRUE) && (theApp.m_LastSettings.bDisplayEnvelopeCrosses!=TRUE)) return;
		bDisplayCrosses = theApp.m_LastSettings.bDisplayEnvelopeCrosses;
		bDisplayLines = theApp.m_LastSettings.bDisplayEnvelopeLines;
		nLineL = PROFILE->m_nEnvelopeLineL;
		rgbDraw = theApp.m_LastSettings.rgbDisplayEnvelopeColor;
		nStep = 1;
		break;
	case NEWLINE:
		if(pFrame->m_CpNewLine.m_nCoordL<=0) return;
		bDisplayCrosses = TRUE;
		bDisplayLines = TRUE;
		nLineL = 1;
		rgbDraw = RGB(0,0,255);
		nStep = 1;
		break;
	}

	if(theApp.m_LastSettings.nDisplayEditWhichLines==0) {
		nStartLine=0;
		nFinishLine=nLineL;
	} else {
		nStartLine=theApp.m_nLine;
		nFinishLine=nStartLine+1;
		if(nStartLine>nLineL) return;
		if(nFinishLine>nLineL) return;
	}

	CFont Font;
	CSize size;
	CRect rrText;
	LOGFONT	lf = theApp.m_LastSettings.lfViewGraticule;
	lf.lfEscapement=0;
	theApp.SetFontLanguage(&lf);
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);


	pDC->GetClipBox(&Rect);
	rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
	pDC->SelectClipRgn(&rgn);

	CPen	penDraw(PS_SOLID  ,1,rgbDraw);
	CPen	penRed(PS_SOLID  ,1,RGB(255,0,0));
	CPen	penBlack(PS_SOLID  ,1,RGB(0,0,0));
	CPen * pOldPen = pDC->SelectObject(&penDraw);

	CPoint *pPt = NULL;
	BYTE *pByte = NULL;
	int nCount = 0;
	int nLine;


	if(bDisplayCrosses==TRUE) {
		pDC->SelectObject(&penDraw);
		for(nLine = nStartLine; nLine<nFinishLine ; nLine+=nStep) {
			switch(nWhichLines) {
			case TAUGHTLINES: pLine = &PROFILE->m_TaughtLine[nLine];
				break;
			case SCANLINES:	pLine = &PROFILE->m_ScanLine[nLine];
				break;
			case BRAINLINES: pLine = &PROFILE->m_BrainLine[nLine];
				break;
			case ENVELOPELINES:	pLine = &PROFILE->m_EnvelopeLine[nLine];
				break;
			case NEWLINE:	pLine = &pFrame->m_CpNewLine;
				break;
			}
			for(nSide=0;nSide<2;nSide++) {
				SAFE_DELETE(pPt);
				SAFE_DELETE(pByte);
				pPt = new CPoint[pLine->m_nCoordL * 4];
				pByte = new BYTE[pLine->m_nCoordL * 4];
				nCount=0;
				for(int ss=0;ss<pLine->m_nCoordL;ss++) {
					WorldToClient(pLine->GetPtrCoord(ss),rrFloat,&pt,nSide);
					if((ss==theApp.m_nIndex) && (nLine == theApp.m_nLine)) {
						pDC->SelectObject(&penRed);
						pDC->MoveTo(pt.x-5,pt.y-5);
						pDC->LineTo(pt.x+6,pt.y+6);
						pDC->MoveTo(pt.x-5,pt.y+5);
						pDC->LineTo(pt.x+6,pt.y-6);
						if(nSide==1) {
							Buff.Format(_T("%s %d %s %d"),Line,theApp.m_nLine+1,Index,theApp.m_nIndex+1);
							size = pDC->GetTextExtent(Buff);
							rrText.SetRect(pt.x+5,pt.y-15,pt.x+5+size.cx,pt.y-15+size.cy);
							pDC->FillRect(rrText,&CBrush(RGB(230,230,230)));
							pDC->TextOut(pt.x+5,pt.y-15,Buff);
						}
					} else {
						if(rr.PtInRect(pt)) {
							pPt[nCount].x = pt.x-2; pPt[nCount].y = pt.y-2; pByte[nCount++] = PT_MOVETO;
							pPt[nCount].x = pt.x+3; pPt[nCount].y = pt.y+3; pByte[nCount++] = PT_LINETO;
							pPt[nCount].x = pt.x-2; pPt[nCount].y = pt.y+2; pByte[nCount++] = PT_MOVETO;
							pPt[nCount].x = pt.x+3; pPt[nCount].y = pt.y-3; pByte[nCount++] = PT_LINETO;
						}
					}
				};
				pDC->SelectObject(&penDraw);
				pDC->PolyDraw(pPt,pByte,nCount);
			}
		}
	}
	if(bDisplayLines==TRUE) {
		for(nLine = nStartLine; nLine<nFinishLine ; nLine+=nStep) {
			switch(nWhichLines) {
			case TAUGHTLINES:	pLine = &PROFILE->m_TaughtLine[nLine];
				break;
			case SCANLINES:	pLine = &PROFILE->m_ScanLine[nLine];
				break;
			case BRAINLINES: pLine = &PROFILE->m_BrainLine[nLine];
				break;
			case ENVELOPELINES:	pLine = &PROFILE->m_EnvelopeLine[nLine];
				bJoinUpLastPoint=TRUE;
				break;
			case NEWLINE:	pLine = &pFrame->m_CpNewLine;
				bJoinUpLastPoint=FALSE;
				break;
			}
			if((nLine==theApp.m_nLine) && (nWhichLines!=NEWLINE)) {
				pDC->SelectObject(&penRed);
			} else {
				pDC->SelectObject(&penDraw);
			}
			DrawProjectedLine(pLine,pDC,rr,bJoinUpLastPoint,PORTSIDE);
			DrawProjectedLine(pLine,pDC,rr,bJoinUpLastPoint,STARBOARD);
		}

		if(	(nWhichLines == SCANLINES) && (theApp.m_LastSettings.nDisplayEditFirstLastCurve == 1)) {
			pDC->SelectObject(&penBlack);
			nLine = PROFILE->m_nScanStartLine;
			if(nLine<0) nLine = 0;
			if(nLine>=PROFILE->m_nScanLineL) nLine = PROFILE->m_nScanLineL-1;
			pLine = &PROFILE->m_ScanLine[nLine];

			DrawProjectedLine(pLine,pDC,rr,FALSE);
			nLine = PROFILE->m_nScanFinishLine;
			if(nLine<0) nLine = 0;
			if(nLine>=PROFILE->m_nScanLineL) nLine = PROFILE->m_nScanLineL-1;
			pLine = &PROFILE->m_ScanLine[nLine];
			DrawProjectedLine(pLine,pDC,rr,FALSE);
		}
	}

	SAFE_DELETE(pPt);
	SAFE_DELETE(pByte);

	rgn.SetRectRgn(&rr);
	pDC->SelectClipRgn(&rgn);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);

}

void C3DEditTurnTablePage::DrawProjectedLine(CPolyCoord *pLine,CDC* pDC,CRect rr,bool bJoinUpLastPoint,int nSide)
{
	int ss;
	float fPos,fStep;
	int	nCoordL = pLine->m_nCoordL;
	CPoint pt,pt0;
	CCoord Cp;
	CRectFloat rrFloat = rr;
	CPoint *pPt;
	pPt = new CPoint[nCoordL];

	switch(pLine->m_nStyle) {
	case LINEAR_LINE:
		for(ss=0;ss<nCoordL;ss++) {
			WorldToClient(pLine->GetPtrCoord(ss),rr,&pPt[ss],nSide);
		};
		pDC->Polyline(pPt,nCoordL);
		if(bJoinUpLastPoint==TRUE) {
			pDC->MoveTo(pPt[nCoordL-1]);
			pDC->LineTo(pPt[0]);
		}
		break;
	case SPLINE_LINE:
		pLine->m_bModified=TRUE;
		pLine->PreProcessLine();
		fStep = pLine->m_fLinearLength / 800.0f;
		if(fStep<=0.0f) fStep=1.0;
		for(fPos=0;fPos<=pLine->m_fLinearLength;fPos+=fStep) {
			pLine->CalculateCoord(fPos,Cp);
			WorldToClient(&Cp,rrFloat,&pt);
			if(fPos>0.0f) {
				pDC->LineTo(pt);
			} else {
				pDC->MoveTo(pt);
			}
		}
		break;
	}
	delete pPt;


}

void C3DEditTurnTablePage::DrawProjectedWaterJets(CPaintDC *pDC, CRect rr, int nDevice)
{
	CPoint ptSurface,ptNozzle;
	CCoord CpHead,CpSurface,CpNozzle,Cp;
	CRgn rgn;
	CRect Rect;
	CPen penBlue(PS_SOLID,3,RGB(0,0,255));
	CPen* pOldPen = pDC->SelectObject(&penBlue);
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));

	pDC->GetClipBox(&Rect);
	rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
	pDC->SelectClipRgn(&rgn);

	CpSurface.Zero();
	theApp.m_Motors.GetSurfacePos(&CpSurface);

	theApp.m_Kinematics.CalculateCoordAlongNorm(CpSurface,&CpNozzle,PORTSIDE,-20.0f);

	WorldToClient(&CpSurface,rr,&ptSurface);
	WorldToClient(&CpNozzle,rr,&ptNozzle);
	//Water jet
//	pDC->MoveTo(ptSurface);
//	pDC->LineTo(ptNozzle);
	//Surface
	pDC->SelectObject(&penBlack);
	pDC->MoveTo(ptSurface.x-4,ptSurface.y);
	pDC->LineTo(ptSurface.x+5,ptSurface.y);
	pDC->MoveTo(ptSurface.x,ptSurface.y-4);
	pDC->LineTo(ptSurface.x,ptSurface.y+5);

	/*
	//Starboard
	pDC->SelectObject(&penBlue);
	theApp.m_Kinematics.CalculateCoordAlongNorm(CpSurface,&CpNozzle,STARBOARD,20.0f);
	WorldToClient(&CpSurface,rr,&ptSurface,STARBOARD);
	WorldToClient(&CpNozzle,rr,&ptNozzle,STARBOARD);
	pDC->MoveTo(ptSurface);
	pDC->LineTo(ptNozzle);
	pDC->SelectObject(&penBlack);
	pDC->MoveTo(ptSurface.x-4,ptSurface.y);
	pDC->LineTo(ptSurface.x+5,ptSurface.y);
	pDC->MoveTo(ptSurface.x,ptSurface.y-4);
	pDC->LineTo(ptSurface.x,ptSurface.y+5);
	*/

	pDC->SelectObject(pOldPen);

	rgn.SetRectRgn(&rr);
	pDC->SelectClipRgn(&rgn);

}

void C3DEditTurnTablePage::WorldToClient(COORDDATA* pCp, CRect rr, CPoint *pt,int nSide)
{
	if(m_nPaintMode == CARTESIAN_POLAR) {
		pt->x = rr.left + (int)((pCp->Side[nSide].fR-m_fR0p)*(float)rr.Width()/(m_fR1p-m_fR0p));
	} else {
		if((theApp.m_LastSettings.nOrientation3DEditImage  & 2) == 0) {
			switch(PROFILE->m_nFastAxis) {
			case 0:
			case 3:	pt->x = rr.left + (int)((pCp->Side[nSide].fX-m_fX0p)*(float)rr.Width()/(m_fX1p-m_fX0p));
				break;
			case 1:	pt->x = rr.left + (int)((pCp->Side[nSide].fY-m_fY0p)*(float)rr.Width()/(m_fY1p-m_fY0p));
				break;
			case 2:	pt->x = rr.left + (int)((pCp->Side[nSide].fZ-m_fZ0p)*(float)rr.Width()/(m_fZ1p-m_fZ0p));
				break;
			}
		} else {
			switch(PROFILE->m_nFastAxis) {
			case 0:
			case 3:	pt->x = rr.right - (int)((pCp->Side[nSide].fX-m_fX0p)*(float)rr.Width()/(m_fX1p-m_fX0p));
				break;
			case 1:	pt->x = rr.right - (int)((pCp->Side[nSide].fY-m_fY0p)*(float)rr.Width()/(m_fY1p-m_fY0p));
				break;
			case 2:	pt->x = rr.right - (int)((pCp->Side[nSide].fZ-m_fZ0p)*(float)rr.Width()/(m_fZ1p-m_fZ0p));
				break;
			}
		}
	}

	switch(PROFILE->m_nSlowAxis) {
	case 0:
		if((theApp.m_LastSettings.nOrientation3DEditImage & 1) == 0) {
			pt->y = rr.bottom - (int)((pCp->Side[nSide].fX-m_fX0p)*(float)rr.Height()/(m_fX1p-m_fX0p));
		} else {
			pt->y = rr.top + (int)((pCp->Side[nSide].fX-m_fX0p)*(float)rr.Height()/(m_fX1p-m_fX0p));
		}
		break;
	case 1:
		if((theApp.m_LastSettings.nOrientation3DEditImage & 1) == 0) {
			pt->y = rr.bottom - (int)((pCp->Side[nSide].fY-m_fY0p)*(float)rr.Height()/(m_fY1p-m_fY0p));
		} else {
			pt->y = rr.top + (int)((pCp->Side[nSide].fY-m_fY0p)*(float)rr.Height()/(m_fY1p-m_fY0p));
		}
		break;
	case 2:
		if((theApp.m_LastSettings.nOrientation3DEditImage  & 1) == 0) {
			pt->y = rr.bottom - (int)((pCp->Side[nSide].fZ-m_fZ0p)*(float)rr.Height()/(m_fZ1p-m_fZ0p));
		} else {
			pt->y = rr.top + (int)((pCp->Side[nSide].fZ-m_fZ0p)*(float)rr.Height()/(m_fZ1p-m_fZ0p));
		}
		break;
	}
}

void C3DEditTurnTablePage::WorldToClient(COORDDATA* pCp, CRectFloat rr, CPoint *pt,int nSide)
{
	if(m_nPaintMode == CARTESIAN_POLAR) {
		pt->x = rr.left + (int)((pCp->Side[nSide].fR-m_fR0p)*rr.fWidth/(m_fR1p-m_fR0p));
	} else {
		if((theApp.m_LastSettings.nOrientation3DEditImage  & 2) == 0) {
			switch(PROFILE->m_nFastAxis) {
			case 0:
			case 3: pt->x = rr.left + (int)((pCp->Side[nSide].fX-m_fX0p)*rr.fWidth/(m_fX1p-m_fX0p));
				break;
			case 1: pt->x = rr.left + (int)((pCp->Side[nSide].fY-m_fY0p)*rr.fWidth/(m_fY1p-m_fY0p));
				break;
			case 2: pt->x = rr.left + (int)((pCp->Side[nSide].fZ-m_fZ0p)*rr.fWidth/(m_fZ1p-m_fZ0p));
				break;
			}
		} else {
			switch(PROFILE->m_nFastAxis) {
			case 0:
			case 3: pt->x = rr.right - (int)((pCp->Side[nSide].fX-m_fX0p)*rr.fWidth/(m_fX1p-m_fX0p));
				break;
			case 1: pt->x = rr.right - (int)((pCp->Side[nSide].fY-m_fY0p)*rr.fWidth/(m_fY1p-m_fY0p));
				break;
			case 2: pt->x = rr.right - (int)((pCp->Side[nSide].fZ-m_fZ0p)*rr.fWidth/(m_fZ1p-m_fZ0p));
				break;
			}
		}
	}
	switch(PROFILE->m_nSlowAxis) {
	case 0:
		if((theApp.m_LastSettings.nOrientation3DEditImage  & 1) == 0) {
			pt->y = rr.bottom - (int)((pCp->Side[nSide].fX-m_fX0p)*rr.fHeight/(m_fX1p-m_fX0p));
		} else {
			pt->y = rr.top + (int)((pCp->Side[nSide].fX-m_fX0p)*rr.fHeight/(m_fX1p-m_fX0p));
		}
		break;
	case 1:
		if((theApp.m_LastSettings.nOrientation3DEditImage  & 1) == 0) {
			pt->y = rr.bottom - (int)((pCp->Side[nSide].fY-m_fY0p)*rr.fHeight/(m_fY1p-m_fY0p));
		} else {
			pt->y = rr.top + (int)((pCp->Side[nSide].fY-m_fY0p)*rr.fHeight/(m_fY1p-m_fY0p));
		}
		break;
	case 2:
		if((theApp.m_LastSettings.nOrientation3DEditImage  & 1) == 0) {
			pt->y = rr.bottom - (int)((pCp->Side[nSide].fZ-m_fZ0p)*rr.fHeight/(m_fZ1p-m_fZ0p));
		} else {
			pt->y = rr.top + (int)((pCp->Side[nSide].fZ-m_fZ0p)*rr.fHeight/(m_fZ1p-m_fZ0p));
		}
		break;
	}
}

void C3DEditTurnTablePage::ClientToWorld(CPoint* pt, CRect rr, COORDDATA* pCp)
{
	if(m_nPaintMode == CARTESIAN_POLAR) {
		pCp->Side[PORTSIDE].fR = ((pt->x - rr.left) * (m_fR1p-m_fR0p) / (float)rr.Width()) + m_fR0p;
	} else {
		if((theApp.m_LastSettings.nOrientation3DEditImage  & 2) == 0) {
			switch(PROFILE->m_nFastAxis) {
			case 0:
			case 3:	pCp->Side[PORTSIDE].fX = MulDiv(pt->x - rr.left,(int)(m_fX1p-m_fX0p),rr.Width()) + m_fX0p;
				break;
			case 1: pCp->Side[PORTSIDE].fY = MulDiv(pt->x - rr.left,(int)(m_fY1p-m_fY0p),rr.Width()) + m_fY0p;
				break;
			case 2: pCp->Side[PORTSIDE].fZ = MulDiv(pt->x - rr.left,(int)(m_fZ1p-m_fZ0p),rr.Width()) + m_fZ0p;
				break;
			}
		} else {
			switch(PROFILE->m_nFastAxis) {
			case 0:
			case 3:	pCp->Side[PORTSIDE].fX = MulDiv(rr.right - pt->x,(int)(m_fX1p-m_fX0p),rr.Width()) + m_fX0p;
				break;
			case 1: pCp->Side[PORTSIDE].fY = MulDiv(rr.right - pt->x,(int)(m_fY1p-m_fY0p),rr.Width()) + m_fY0p;
				break;
			case 2: pCp->Side[PORTSIDE].fZ = MulDiv(rr.right - pt->x,(int)(m_fZ1p-m_fZ0p),rr.Width()) + m_fZ0p;
				break;
			}
		}
	}

	switch(PROFILE->m_nSlowAxis) {
	case 0:
		if((theApp.m_LastSettings.nOrientation3DEditImage  & 1) == 0) {
			pCp->Side[PORTSIDE].fX = MulDiv(rr.bottom - pt->y,(int)(m_fX1p-m_fX0p),rr.Height()) + m_fX0p;
		} else {
			pCp->Side[PORTSIDE].fX = MulDiv(pt->y - rr.top ,(int)(m_fX1p-m_fX0p),rr.Height()) + m_fX0p;
		}
		break;
	case 1:
		if((theApp.m_LastSettings.nOrientation3DEditImage  & 1) == 0) {
			pCp->Side[PORTSIDE].fY = MulDiv(rr.bottom - pt->y,(int)(m_fY1p-m_fY0p),rr.Height()) + m_fY0p;
		} else {
			pCp->Side[PORTSIDE].fY = MulDiv(pt->y - rr.top ,(int)(m_fY1p-m_fY0p),rr.Height()) + m_fY0p;
		}
		break;
	case 2:
		if((theApp.m_LastSettings.nOrientation3DEditImage  & 1) == 0) {
			pCp->Side[PORTSIDE].fZ = MulDiv(rr.bottom - pt->y,(int)(m_fZ1p-m_fZ0p),rr.Height()) + m_fZ0p;
		} else {
			pCp->Side[PORTSIDE].fZ = MulDiv(pt->y - rr.top,(int)(m_fZ1p-m_fZ0p),rr.Height()) + m_fZ0p;
		}
		break;
	}


}

void C3DEditTurnTablePage::OnViewLines() 
{
	switch(m_nEditWhich) {
	case TAUGHTLINES: theApp.m_LastSettings.bDisplayTaughtLines==TRUE ? theApp.m_LastSettings.bDisplayTaughtLines=FALSE : theApp.m_LastSettings.bDisplayTaughtLines=TRUE;
		break;
	case SCANLINES: theApp.m_LastSettings.bDisplayScanLines==TRUE ? theApp.m_LastSettings.bDisplayScanLines=FALSE : theApp.m_LastSettings.bDisplayScanLines=TRUE;
		break;
	case BRAINLINES: theApp.m_LastSettings.bDisplayBrainLines==TRUE ? theApp.m_LastSettings.bDisplayBrainLines=FALSE : theApp.m_LastSettings.bDisplayBrainLines=TRUE;
		break;
	case ENVELOPELINES: theApp.m_LastSettings.bDisplayEnvelopeLines==TRUE ? theApp.m_LastSettings.bDisplayEnvelopeLines=FALSE : theApp.m_LastSettings.bDisplayEnvelopeLines=TRUE;
		break;
	}
	SetToolBarCheckedState();
	Invalidate(FALSE);
}

void C3DEditTurnTablePage::OnViewCrosses()
{
	switch(m_nEditWhich) {
	case TAUGHTLINES: theApp.m_LastSettings.bDisplayTaughtCrosses==TRUE ? theApp.m_LastSettings.bDisplayTaughtCrosses=FALSE : theApp.m_LastSettings.bDisplayTaughtCrosses=TRUE;
		break;
	case SCANLINES: theApp.m_LastSettings.bDisplayScanCrosses==TRUE ? theApp.m_LastSettings.bDisplayScanCrosses=FALSE : theApp.m_LastSettings.bDisplayScanCrosses=TRUE;
		break;
	case BRAINLINES: theApp.m_LastSettings.bDisplayBrainCrosses==TRUE ? theApp.m_LastSettings.bDisplayBrainCrosses=FALSE : theApp.m_LastSettings.bDisplayBrainCrosses=TRUE;
		break;
	case ENVELOPELINES: theApp.m_LastSettings.bDisplayEnvelopeCrosses==TRUE ? theApp.m_LastSettings.bDisplayEnvelopeCrosses=FALSE : theApp.m_LastSettings.bDisplayEnvelopeCrosses=TRUE;
		break;
	}
	SetToolBarCheckedState();
	Invalidate(FALSE);
}

void C3DEditTurnTablePage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	CCoord Cp;
	int	nIgnoreAxis;
	m_bLBDown = TRUE;
	m_ptDown = point;
	bool bFlag;

	switch(PROFILE->m_nFastAxis) {
	case 0:
	case 3:
		switch(PROFILE->m_nSlowAxis) {
		case 1: nIgnoreAxis = 'Z';
			break;
		case 2: nIgnoreAxis = 'Y';
			break;
		}
		break;
	case 1:
		switch(PROFILE->m_nSlowAxis) {
		case 0: nIgnoreAxis = 'Z';
			break;
		case 2: nIgnoreAxis = 'X';
			break;
		}
		break;
	case 2:
		switch(PROFILE->m_nSlowAxis) {
		case 0: nIgnoreAxis = 'Y';
			break;
		case 1: nIgnoreAxis = 'X';
			break;
		}
		break;
	}

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	GotoDlgCtrl(GetDlgItem(IDC_STATIC_VIEW));

	switch(m_nPaintMode) {
	case CARTESIAN:
		ClientToWorld(&point,rr,&Cp);

		bFlag = PROFILE->FindNearestCoordinate(m_nEditWhich,Cp,&theApp.m_nLine,&theApp.m_nIndex,nIgnoreAxis);
		if(bFlag==TRUE) {
			UpdateAllControls();
			FillList();
			Invalidate(FALSE);
		}
		break;
	case CARTESIAN_POLAR:
		ClientToWorld(&point,rr,&Cp);

		bFlag = PROFILE->FindNearestAngleCoordinate(m_nEditWhich,Cp,&theApp.m_nLine,&theApp.m_nIndex);
		if(bFlag==TRUE) {
			UpdateAllControls();
			FillList();
			Invalidate(FALSE);
		}
		break;
	case POLAR:
		break;
	}

	
	CPropertyPage::OnLButtonDown(nFlags, point);
}

void C3DEditTurnTablePage::OnButtonCartesian() 
{
	m_nPaintMode = CARTESIAN;
	CreateColumns();
	Invalidate(FALSE);
	SetToolBarCheckedState();
	
}

void C3DEditTurnTablePage::OnButtonCartesianPolar() 
{
	m_nPaintMode = CARTESIAN_POLAR;
	CreateColumns();
	Invalidate(FALSE);
	SetToolBarCheckedState();
	
}

void C3DEditTurnTablePage::OnButtonPolar() 
{
	m_nPaintMode = POLAR;
	Invalidate(FALSE);
	SetToolBarCheckedState();
}

void C3DEditTurnTablePage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint pt = point;
	CRect Rect = m_StaticPosView.m_Rect;
	ScreenToClient(&point);
	m_ptClient = point;
	CString Buff,Danger, Goto;

	Goto.LoadString(IDS_Goto);

	switch(m_nPaintMode) {
	case CARTESIAN:

		if(Rect.PtInRect(point)) {
			menu->LoadMenu(IDR_RB_TT_VIEW_MENU);
			pPopup = menu->GetSubMenu(0);
			Buff.Format(L"%s %s",Goto, theApp.m_DlgSideName[0]);
			pPopup->ModifyMenu(ID_TURNTABLE_GOTOLEFT,MF_BYCOMMAND,ID_TURNTABLE_GOTOLEFT,Buff);
			Buff.Format(L"%s %s",Goto, theApp.m_DlgSideName[1]);
			pPopup->ModifyMenu(ID_TURNTABLE_GOTORIGHT,MF_BYCOMMAND,ID_TURNTABLE_GOTORIGHT,Buff);

			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);

		};
		break;
	case POLAR:

		if(Rect.PtInRect(point)) {
			menu->LoadMenu(IDR_RB_TTPOLAR_MENU);
			pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
		};
		break;
	}

	delete menu;
	
}


void C3DEditTurnTablePage::OnSelchangeComboTimeslot() 
{
	theApp.m_LastSettings.nEccentricTimeSlot = m_comboTimeSlot.GetCurSel();
}

void C3DEditTurnTablePage::OnButtonEccentric() 
{
	m_nPaintMode = POLAR;
	Invalidate(FALSE);
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction=ECCENTRIC_TEST;
}


void C3DEditTurnTablePage::OnTtpolarRotatetonearestpoint() 
{
	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);

	Cp.Side0.fR = theApp.m_Thread.m_CpEccentricCentre.Side0.fR;
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVE_JUST_R;
	
}

void C3DEditTurnTablePage::OnTtpolarRotatetofurthestpoint() 
{
	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);

	Cp.Side0.fR = theApp.m_Thread.m_CpEccentricCentre.Side0.fR + 180.0f;
	Cp.Side0.fR = fmodf(Cp.Side0.fR,360.0f);
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVE_JUST_R;
}

void C3DEditTurnTablePage::OnTtpolarRotatetothisangle() 
{
	CRect rr;
	CCoord Cp;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	CPoint pt = rr.CenterPoint();


	float fX = (float)(m_ptClient.x - pt.x) * -1.0f;
	float fY = (float)(m_ptClient.y - pt.y);
	float fAngle = atan2f(fY,fX) * RAD_TO_DEG;

	if(fAngle<0)
		fAngle = fAngle+360;

	theApp.m_Motors.GetHeadPos(&Cp);

	Cp.Side0.fR = fAngle;
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVE_JUST_R;

}

void C3DEditTurnTablePage::OnTtpolarRotatetonominal() 
{
	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);

	Cp.Side0.fR = theApp.m_Thread.m_CpEccentricCentre.Side0.fR + 90.0f;
	Cp.Side0.fR = fmodf(Cp.Side0.fR,360.0f);
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVE_JUST_R;
	
}

void C3DEditTurnTablePage::OnTtlistAdd() 
{
	CCoord	CpSurface;

	CpSurface.Zero();
	theApp.m_Motors.GetSurfacePos(&CpSurface);
	CpSurface.Side1.fR = CpSurface.Side0.fR;

	if(theApp.m_LastSettings.nRotateSurfaceCoordinates) {
		theApp.m_Kinematics.CalculateRandP(&CpSurface,PORTSIDE,0.0f);
		theApp.m_Kinematics.CalculateRandP(&CpSurface,STARBOARD,0.0f);
	};

	theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface,PORTSIDE);
	theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface,STARBOARD);
//	theApp.m_UtUser.GetAllUt(&CpSurface);

	float fX = CpSurface.Side0.fX - CpSurface.Side1.fX;
	float fY = CpSurface.Side0.fY - CpSurface.Side1.fY;
	float fZ = CpSurface.Side0.fZ - CpSurface.Side1.fZ;

	CpSurface.fThickness = sqrtf((fX*fX) + (fY*fY) + (fZ*fZ));

	CalculateRadius(&CpSurface);

	m_nEditWhich = TAUGHTLINES;
	InitializeEditLine();

	if(m_pEditLine==NULL) {
		switch(m_nEditWhich) {
		case TAUGHTLINES:
			PROFILE->m_TaughtLine = new CPolyCoord[PROFILE->m_nTaughtLineL=1];
			PROFILE->m_TaughtLine[theApp.m_nLine=0].Add(CpSurface);
			break;
		case SCANLINES:
			PROFILE->m_ScanLine = new CPolyCoord[PROFILE->m_nScanLineL=1];
			PROFILE->m_ScanLine[theApp.m_nLine=0].Add(CpSurface);
			break;
		}
		InitializeEditLine();
	} else {
		m_pEditLine[theApp.m_nLine].Add(CpSurface);
	}


	GenerateScanLines();
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
	PROFILE->CalculateDangerZone();
}

void C3DEditTurnTablePage::OnTtlistModify() 
{
	CCoord	CpSurface;

	CpSurface.Zero();
	theApp.m_Motors.GetSurfacePos(&CpSurface);
	CpSurface.Side1.fR = CpSurface.Side0.fR;

	if(theApp.m_LastSettings.nRotateSurfaceCoordinates) {
		theApp.m_Kinematics.CalculateRandP(&CpSurface,PORTSIDE,0.0f);
		theApp.m_Kinematics.CalculateRandP(&CpSurface,STARBOARD,0.0f);
	}
	theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface,PORTSIDE);
	theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface,STARBOARD);
//	theApp.m_UtUser.GetAllUt(&CpSurface);

	float fX = CpSurface.Side0.fX - CpSurface.Side1.fX;
	float fY = CpSurface.Side0.fY - CpSurface.Side1.fY;
	float fZ = CpSurface.Side0.fZ - CpSurface.Side1.fZ;

	CpSurface.fThickness = sqrtf((fX*fX) + (fY*fY) + (fZ*fZ));

	if(m_pEditLine) {
		m_pEditLine[theApp.m_nLine].ModifyCoord(theApp.m_nIndex,CpSurface);
	}

	GenerateScanLines();
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
	PROFILE->CalculateDangerZone();
}

void C3DEditTurnTablePage::OnTtlistDelete() 
{
	if(m_pEditLine==NULL) return;

	if(m_pEditLine[theApp.m_nLine].m_nCoordL == 1) {
		if(MessageBox(_T("This is the last vertex on this line. Deleteing this point will remove the entire line\r\nDo you wish to continue ?"),_T("Warning"),MB_YESNO|MB_ICONEXCLAMATION)!=IDYES) return;
		PROFILE->DeleteLine(m_nEditWhich,theApp.m_nLine);
		InitializeEditLine();
		UpdateAllControls();
	} else {
		m_pEditLine[theApp.m_nLine].DeleteCoords(theApp.m_nIndex,theApp.m_nIndex);
	}

	GenerateScanLines();
	InitializeEditLine();
	FillList();
	Invalidate(FALSE);
	PROFILE->CalculateDangerZone();
}

void C3DEditTurnTablePage::OnTtlistInsert() 
{
	CCoord CpSurface;
	int k;
	float fX,fY,fZ;

	CpSurface.Zero();
	theApp.m_Motors.GetSurfacePos(&CpSurface);
	CpSurface.Side1.fR = CpSurface.Side0.fR;

	if(theApp.m_LastSettings.nRotateSurfaceCoordinates) {
		theApp.m_Kinematics.CalculateRandP(&CpSurface,PORTSIDE,0.0f);
		theApp.m_Kinematics.CalculateRandP(&CpSurface,STARBOARD,0.0f);
	}
	theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface,PORTSIDE);
	theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface,STARBOARD);
//	theApp.m_UtUser.GetAllUt(&CpSurface);

	fX = CpSurface.Side0.fX - CpSurface.Side1.fX;
	fY = CpSurface.Side0.fY - CpSurface.Side1.fY;
	fZ = CpSurface.Side0.fZ - CpSurface.Side1.fZ;

	CpSurface.fThickness = sqrtf((fX*fX) + (fY*fY) + (fZ*fZ));

	if(m_pEditLine==NULL) {
		switch(m_nEditWhich) {
		case TAUGHTLINES:
			PROFILE->m_TaughtLine = new CPolyCoord[PROFILE->m_nTaughtLineL=1];
			PROFILE->m_TaughtLine[theApp.m_nLine=0].Add(CpSurface);
			break;
		case SCANLINES:
			PROFILE->m_ScanLine = new CPolyCoord[PROFILE->m_nScanLineL=1];
			PROFILE->m_ScanLine[theApp.m_nLine=0].Add(CpSurface);
			break;
		}
	} else {
		k=0;
		for(int ii=0;ii<(m_pEditLine[theApp.m_nLine].m_nCoordL-1);ii++) {
			if((m_pEditLine[theApp.m_nLine].m_pCp[ii].Side0.fZ>=CpSurface.Side0.fZ) && (CpSurface.Side0.fZ>=m_pEditLine[theApp.m_nLine].m_pCp[ii+1].Side0.fZ)) {
				k=ii+1;
				break;
			}
		}
		if(CpSurface.Side0.fZ<m_pEditLine[theApp.m_nLine].m_pCp[m_pEditLine[theApp.m_nLine].m_nCoordL-1].Side0.fZ) {
			m_pEditLine[theApp.m_nLine].Add(CpSurface);
		} else {
			m_pEditLine[theApp.m_nLine].InsertCoord(k,CpSurface);
		}
	}

	GenerateScanLines();
	InitializeEditLine();
	FillList();
	Invalidate(FALSE);
	PROFILE->CalculateDangerZone();

}

void C3DEditTurnTablePage::OnTtlistTranslateverticallytohere() 
{
	CCoord CpSurface,CpComp;

	if(theApp.m_nLine == 0) {
		if(theApp.m_nIndex >= 0 && theApp.m_nIndex <m_pEditLine[theApp.m_nLine].GetSize()) {
			theApp.m_Motors.GetSurfacePos(&CpSurface);
			m_pEditLine[theApp.m_nLine].GetCoord(theApp.m_nIndex,&CpComp);

			for(int nn=0;nn<=m_pEditLine[theApp.m_nLine].m_nCoordL;nn++) {
				m_pEditLine[theApp.m_nLine].m_pCp[nn].Side0.fZ += (CpSurface.Side0.fZ - CpComp.Side0.fZ);
				m_pEditLine[theApp.m_nLine].m_pCp[nn].Side1.fZ += (CpSurface.Side0.fZ - CpComp.Side0.fZ);
			}
			m_pEditLine[theApp.m_nLine].m_bModified = TRUE;

			GenerateScanLines();
			InitializeEditLine();
			FillList();
			Invalidate(FALSE);
			PROFILE->CalculateDangerZone();
		}
	}
	
}


void C3DEditTurnTablePage::GenerateScanLines()
{
	if(PROFILE->m_nComponentDescription != CIRCULAR) return;

	if(PROFILE->m_TaughtLine!=NULL) {
		if(PROFILE->m_TaughtLine[0].m_nCoordL>1) {
			PROFILE->GenerateRotoSymetricScanLineV2();
		}
	}


}

void C3DEditTurnTablePage::SetAccessPrivelage()
{
	if(GetSafeHwnd() == NULL) return;
	bool bFlag;

	theApp.m_nLogonLevelMask&theApp.m_cAccess[IDD_3D_TURNTABLE_PAGE] ? bFlag = true : bFlag = false;

	m_comboTimeSlot.EnableWindow(bFlag);
	m_listCoords.EnableWindow(bFlag);
//	m_editRPos.EnableWindow(bFlag);

	SetToolBarCheckedState();
}

void C3DEditTurnTablePage::OnButtonTaughtLine() 
{
	CloseEditSpin();
	m_nEditWhich = TAUGHTLINES;
	SetToolBarCheckedState();
	InitializeEditLine();
	FillList();
	Invalidate(FALSE);
	
}

void C3DEditTurnTablePage::OnButtonScanLine() 
{
	CloseEditSpin();
	m_nEditWhich = SCANLINES;
	SetToolBarCheckedState();
	InitializeEditLine();
	FillList();
	Invalidate(FALSE);
}

void C3DEditTurnTablePage::OnTtlistSortinz() 
{
	if(PROFILE->m_TaughtLine) {
		PROFILE->m_TaughtLine[0].Sort(2,-1);
	}

	GenerateScanLines();

	InitializeEditLine();
	FillList();
	Invalidate(FALSE);
	
}

void C3DEditTurnTablePage::OnTtlistSortiny() 
{
	if(PROFILE->m_TaughtLine) {
		PROFILE->m_TaughtLine[0].Sort(1,0);
	}

	GenerateScanLines();

	InitializeEditLine();
	FillList();
	Invalidate(FALSE);
}

void C3DEditTurnTablePage::OnTtSortinx()
{
	if (PROFILE->m_TaughtLine) {
		PROFILE->m_TaughtLine[0].Sort(0, 0);
	}

	GenerateScanLines();

	InitializeEditLine();
	FillList();
	Invalidate(FALSE);
}


void C3DEditTurnTablePage::OnButtonRightToLeft() 
{
	CCoord CpHead1,CpHead2,CpSurface;

	if(theApp.m_LastSettings.nDisplayAreYouSure!=0)
		if(MessageBox(_T("Are You Sure\nYou want to move the Right to the Left"),_T("Warning"),MB_YESNO) != IDYES) return;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.GetHeadPos(&CpHead1);
	CpHead2 = CpHead1;
	if(theApp.m_nSide0Orientation==0) {
		theApp.m_Kinematics.SurfaceFromHead(&CpHead1,&CpSurface,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
		theApp.m_Kinematics.CalculateOppositeCoord(STARBOARD,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
		CpSurface.Side1.fI = CpHead2.Side1.fI;
		CpSurface.Side1.fJ = CpHead2.Side1.fJ;
		CpSurface.Side1.fK = CpHead2.Side1.fK;
		CpSurface.Side1.fXt = CpHead2.Side1.fXt;
		CpSurface.Side1.fYt = CpHead2.Side1.fYt;
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead1,STARBOARD,TRUE,TRUE);
		theApp.m_Kinematics.CalculateCompensationPos(&CpHead1);
	} else {
		theApp.m_Kinematics.SurfaceFromHead(&CpHead1,&CpSurface,STARBOARD,TRUE,TRUE);
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
		theApp.m_Kinematics.CalculateOppositeCoord(PORTSIDE,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
		CpSurface.Side0.fI = CpHead2.Side0.fI;
		CpSurface.Side0.fJ = CpHead2.Side0.fJ;
		CpSurface.Side0.fK = CpHead2.Side0.fK;
		CpSurface.Side0.fXt = CpHead2.Side0.fXt;
		CpSurface.Side0.fYt = CpHead2.Side0.fYt;
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead1,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.CalculateCompensationPos(&CpHead1);
	}
	theApp.m_Thread.m_CpMoveTo = CpHead1;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	
}

void C3DEditTurnTablePage::OnChangeEditLineNumber() 
{
	CString Buff;
	int		nTemp = theApp.m_nLine;

	m_editLineNumber.GetWindowText(Buff);
	theApp.m_nLine = _ttoi(Buff) - 1;
	if(theApp.m_nLine>=m_nEditLineL) theApp.m_nLine = m_nEditLineL - 1;
	if(theApp.m_nLine<0) theApp.m_nLine = 0;

	if(nTemp - theApp.m_nLine) {
		m_spinLineNumber.SetPos(theApp.m_nLine);
		UpdateAllControls();
	}
}

void C3DEditTurnTablePage::OnDeltaposSpinLineNumber(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	int nTemp = theApp.m_nLine;

	theApp.m_nLine += pNMUpDown->iDelta;
	if(theApp.m_nLine>=m_nEditLineL) theApp.m_nLine = m_nEditLineL-1;
	if(theApp.m_nLine<0) theApp.m_nLine = 0;
	if(nTemp - theApp.m_nLine) {
		UpdateAllControls();
		FillList();
		Invalidate(FALSE);
	}

	*pResult = 0;
}

void C3DEditTurnTablePage::OnTtlistNewline() 
{
	CPolyCoord CpLine;

	PROFILE->AddLine(theApp.m_nEditLineType);
	InitializeEditLine();
	theApp.m_nLine = m_nEditLineL-1;
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
}

void C3DEditTurnTablePage::OnTtlistCopyline() 
{
	theApp.m_CopyLine.Zero();
	theApp.m_CopyLine = &m_pEditLine[theApp.m_nLine];
}

void C3DEditTurnTablePage::OnTtlistPasteline() 
{
	CString Buff;
	float fAngle = 0.0f;
	D3DXMATRIXA16 matWorld;

	CPromptDlg dlg(this,L"Please enter the turntable angle",L"Request Information",true);
	dlg.DoModal();

	Buff = dlg.m_strVariable;
	int nn = Buff.FindOneOf(L"0123456789");
	if(nn>=0) {
		Buff.Delete(0,nn);
		swscanf_s(Buff,L"%f",&fAngle);
	}

	fAngle *= DEG_TO_RAD;

	D3DXMatrixRotationZ(&matWorld,fAngle);
	for (int ii=0;ii<theApp.m_CopyLine.m_nCoordL;ii++) {
		D3DXVec3TransformCoord(&theApp.m_CopyLine.m_pCp[ii].Side0.pt,&theApp.m_CopyLine.m_pCp[ii].Side0.pt,&matWorld);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&theApp.m_CopyLine.m_pCp[ii].Side0.norm,(D3DXVECTOR3*)&theApp.m_CopyLine.m_pCp[ii].Side0.norm,&matWorld);
		D3DXVec3TransformCoord(&theApp.m_CopyLine.m_pCp[ii].Side1.pt,&theApp.m_CopyLine.m_pCp[ii].Side1.pt,&matWorld);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&theApp.m_CopyLine.m_pCp[ii].Side1.norm,(D3DXVECTOR3*)&theApp.m_CopyLine.m_pCp[ii].Side1.norm,&matWorld);
	}


	PROFILE->AddLine(theApp.m_nEditLineType,&theApp.m_CopyLine,PROFILE->m_nSlowAxis);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
}


void C3DEditTurnTablePage::OnTtlistComputeandfillpolarvalues() 
{
	PROFILE->ComputePolarValues(TAUGHTLINES);
	PROFILE->ComputePolarValues(SCANLINES);
	Invalidate(FALSE);
}

void C3DEditTurnTablePage::OnSelchangeComboDescription() 
{
	PROFILE->m_nComponentDescription = (ComponentDescription)m_comboDescription.GetCurSel();
}

void C3DEditTurnTablePage::OnTtlistGotoleftpolarcoord() 
{
	GotoPolarCoord(1);
}

void C3DEditTurnTablePage::OnTtlistGotorightpolarcoord() 
{
	GotoPolarCoord(2);
}

void C3DEditTurnTablePage::OnTtlistGotobothpolarcoord() 
{
	GotoPolarCoord(3);

}	
	
void C3DEditTurnTablePage::GotoPolarCoord(int nMoveSide) 
{
	CCoord CpSurface;
	D3DXMATRIXA16 matRotate;

	m_pEditLine[theApp.m_nLine].GetCoord(theApp.m_nIndex,&CpSurface);

	D3DXMatrixRotationZ(&matRotate,CpSurface.Side0.fR * -DEG_TO_RAD);

	D3DXVec3TransformCoord(&CpSurface.Side[PORTSIDE].pt,&CpSurface.Side[PORTSIDE].pt,&matRotate);
	D3DXVec3TransformCoord((D3DXVECTOR3*)&CpSurface.Side[PORTSIDE].norm,(D3DXVECTOR3*)&CpSurface.Side[PORTSIDE].norm,&matRotate);
	D3DXVec3Normalize((D3DXVECTOR3*)&CpSurface.Side[PORTSIDE].norm,(D3DXVECTOR3*)&CpSurface.Side[PORTSIDE].norm);

	D3DXVec3TransformCoord(&CpSurface.Side[STARBOARD].pt,&CpSurface.Side[STARBOARD].pt,&matRotate);
	D3DXVec3TransformCoord((D3DXVECTOR3*)&CpSurface.Side[STARBOARD].norm,(D3DXVECTOR3*)&CpSurface.Side[STARBOARD].norm,&matRotate);
	D3DXVec3Normalize((D3DXVECTOR3*)&CpSurface.Side[STARBOARD].norm,(D3DXVECTOR3*)&CpSurface.Side[STARBOARD].norm);
	
	if(theApp.m_LastSettings.bGotoViaDangerPlane == FALSE) {
		switch(nMoveSide) {
		case 1: theApp.m_Motors.GotoSurfaceCoord(&CpSurface,0);
			break;
		case 2: theApp.m_Motors.GotoSurfaceCoord(&CpSurface,1);
			break;
		case 3: theApp.m_Motors.GotoSurfaceCoord(&CpSurface,-1);
			break;
		}
	} else {
		theApp.m_Thread.m_nMoveSide = nMoveSide;
		theApp.m_Thread.m_CpMoveTo = CpSurface;
		theApp.m_Thread.m_nDangerPlaneThreadAction = 0;
		theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
	}

}


void C3DEditTurnTablePage::OnTurntableGotoleft() 
{
	TurntableGoto(1);
}

void C3DEditTurnTablePage::OnTurntableGotoright() 
{
	TurntableGoto(2);
}

void C3DEditTurnTablePage::OnTurntableGotoboth() 
{
	TurntableGoto(3);
}


void C3DEditTurnTablePage::TurntableGoto(int nMoveSide)
{
	CCoord CpSurface,CpComponent;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	theApp.m_Motors.SetRevCount(0);
	m_pEditLine[theApp.m_nLine].GetCoord(theApp.m_nIndex,&CpComponent);
	CpComponent.Side0.fR = CpSurface.Side0.fR;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		if(theApp.m_LastSettings.bGotoViaDangerPlane == FALSE) {
			switch(nMoveSide) {
			case 1: theApp.m_Motors.GotoSurfaceCoord(&CpComponent,0);
				break;
			case 2: theApp.m_Motors.GotoSurfaceCoord(&CpComponent,1);
				break;
			case 3: theApp.m_Motors.GotoSurfaceCoord(&CpComponent,-1);
				break;
			}
		} else {
			theApp.m_Thread.m_nMoveSide = nMoveSide;
			theApp.m_Thread.m_CpMoveTo = CpComponent;
			theApp.m_Thread.m_nDangerPlaneThreadAction = 0;
			theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
		}
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		theApp.m_Motors.IntelligentGotoSurfaceCoord(&CpComponent, nMoveSide, theApp.m_LastSettings.nDangerPlaneMoveMask);
		break;
	};

}



void C3DEditTurnTablePage::OnButtonGotoZeroDegrees() 
{
	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);

	Cp.Side0.fR = 0.0f;
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVE_JUST_R;
	
}

void C3DEditTurnTablePage::OnCheckZeroDegree() 
{
	theApp.m_LastSettings.nRotateSurfaceCoordinates = m_checkZeroDegreeRotate.GetCheck();
}


void C3DEditTurnTablePage::OnChangeEditSlowIncrement() 
{
	CString Buff;
	float fTemp;

	m_editSlowIncrement.GetWindowText(Buff);
	_WTOF(Buff,fTemp);
	fTemp /= theApp.m_fUnits;
	if(PROFILE->m_fSlowIncrement - fTemp) {
		PROFILE->m_fSlowIncrement = fTemp;
		UpdateAllControls();
		UpdateMainFrameTools();
		GenerateScanLines();

		InitializeEditLine();
		FillList();
		Invalidate(FALSE);
//		m_spinSlowIncrement.SetFocus();
		m_editSlowIncrement.SetFocus();

	}
}

void C3DEditTurnTablePage::OnDeltaposSpinSlowIncrement(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	float fStep = 1.0f / powf(10.0f, (float)theApp.m_LastSettings.nStepSizePrecision);
	
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * fStep);
	} else {
		fStep *= 25.4f;
		PROFILE->m_fSlowIncrement += ((float)pNMUpDown->iDelta * fStep);
	}
	UpdateAllControls();
	UpdateMainFrameTools();

	GenerateScanLines();

	InitializeEditLine();
	FillList();
//	Invalidate(FALSE);
	SendMessage(WM_PAINT);


	*pResult = 0;
}

void C3DEditTurnTablePage::UpdateMainFrameTools()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(	pFrame->m_pMotionToolsSheet ) {
		pFrame->m_pMotionToolsSheet->UpdateAllPages();
	}

}

void C3DEditTurnTablePage::OnTtlistCopy() 
{
	CCoord CpSurface;
	CString Buff;

	m_pEditLine[theApp.m_nLine].GetCoord(theApp.m_nIndex,&CpSurface);
	Buff.Format(L"X:%.02f Y:%.02f, Z:%.02f",CpSurface.Side0.fX,CpSurface.Side0.fY,CpSurface.Side0.fZ);
	theApp.SetStringOnToClipboard(Buff);

}

void C3DEditTurnTablePage::CalculateRadius(CCoord *pCp)
{
	if(theApp.m_Tank.vTTZeroDegrees.z == 0.0f) {
		pCp->fRadius[PORTSIDE] = hypot(pCp->Side0.fX,pCp->Side0.fY);
		pCp->fRadius[STARBOARD] = hypot(pCp->Side1.fX,pCp->Side1.fY);
	};

}

void C3DEditTurnTablePage::OnViewXy() 
{
	PROFILE->m_nFastAxis = 0;
	PROFILE->m_nSlowAxis = 1;
	Invalidate(false);
	SetToolBarCheckedState();
}

void C3DEditTurnTablePage::OnViewXz() 
{
	PROFILE->m_nFastAxis = 0;
	PROFILE->m_nSlowAxis = 2;
	Invalidate(false);
	SetToolBarCheckedState();
}

void C3DEditTurnTablePage::OnViewYz() 
{
	PROFILE->m_nFastAxis = 1;
	PROFILE->m_nSlowAxis = 2;
	Invalidate(false);
	SetToolBarCheckedState();
}

void C3DEditTurnTablePage::DrawPerspectiveArrows(CPaintDC *pDC, CRect rr)
{
	CString Buff;

	LOGFONT lf;
	ZeroMemory(&lf,sizeof lf);
	CFont FontHorizontal;
	CFont FontVertical;
	CFont* pOldFont;
	theApp.SetFontLanguage(&lf);
	lf.lfHeight = 12;
	FontHorizontal.CreateFontIndirect(&lf);
	lf.lfEscapement = 900;
	FontVertical.CreateFontIndirect(&lf);
	pOldFont = pDC->SelectObject(&FontHorizontal);


	CPen penRed(PS_SOLID,1,RGB(255,0,0));
	CPen penGreen(PS_SOLID,1,RGB(0,255,0));
	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));

	CPen* pOldPen = pDC->SelectObject(&penBlack);

	switch(PROFILE->m_nFastAxis) {
	case 0: pDC->SelectObject(&penRed);
		Buff = 'X';
		break;
	case 1: pDC->SelectObject(&penGreen);
		Buff = 'Y';
		break;
	case 2: pDC->SelectObject(&penBlue);
		Buff = 'Z';
		break;
	}
	pDC->MoveTo(10,10);
	pDC->LineTo(30,10);
	pDC->LineTo(26,6);
	pDC->MoveTo(30,10);
	pDC->LineTo(26,14);
	CSize size = pDC->GetTextExtent(Buff);
	pDC->TextOut(30,10-size.cy/2,Buff);

	switch(PROFILE->m_nSlowAxis) {
	case 0: pDC->SelectObject(&penRed);
		Buff = 'X';
		break;
	case 1: pDC->SelectObject(&penGreen);
		Buff = 'Y';
		break;
	case 2: pDC->SelectObject(&penBlue);
		Buff = 'Z';
		break;
	}
	pDC->MoveTo(10,10);
	pDC->LineTo(10,30);
	pDC->LineTo(6,26);
	pDC->MoveTo(10,30);
	pDC->LineTo(14,26);
	size = pDC->GetTextExtent(Buff);
	pDC->TextOut(10-size.cx/2,30,Buff);

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}

void C3DEditTurnTablePage::OnSelchangeComboAlgorithm() 
{
	PROFILE->m_nComputeAlgorithm = m_comboAlgorithm.GetCurSel();

	GenerateScanLines();
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
	PROFILE->CalculateDangerZone();

}

void C3DEditTurnTablePage::OnChangeEditTtCentre() 
{
	CString Buff;

	m_editTTCentre.GetWindowText(Buff);
	ExtractVector(Buff,&theApp.m_Tank.vTTCentre);
	
}

void C3DEditTurnTablePage::OnTtlist3dlookatthispoint() 
{
	CCoord CpSurface;
	CProfileSheet* pSheet = (CProfileSheet*)GetParent();

	m_pEditLine[theApp.m_nLine].GetCoord(theApp.m_nIndex,&CpSurface);

	if(pSheet->m_p3DViewPage) {
		pSheet->m_p3DViewPage->LookAtPoint(CpSurface);
	}
	
}

void C3DEditTurnTablePage::OnTtlist3dlookatttcenter() 
{
	CCoord CpSurface;
	CProfileSheet* pSheet = (CProfileSheet*)GetParent();

	CpSurface.Side0.fX = theApp.m_Tank.vTTCentre.x;
	CpSurface.Side0.fY = theApp.m_Tank.vTTCentre.y;
	CpSurface.Side0.fZ = theApp.m_Tank.vTTCentre.z;

	if(pSheet->m_p3DViewPage) {
		pSheet->m_p3DViewPage->LookAtPoint(CpSurface);
	}
}




void C3DEditTurnTablePage::OnEnChangeEditDiameter()
{
	FRAME;
	CString Buff;
	float fTemp;

	m_editDiameter.GetWindowText(Buff);
	_WTOF(Buff, fTemp);
	fTemp /= theApp.m_fUnits;

	if (PROFILE->m_fComponentDiameter[0] - fTemp) {
		PROFILE->m_fComponentDiameter[1] = PROFILE->m_fComponentDiameter[0] = fTemp;
		theApp.m_Kinematics.CalculateRollerKinematics(PROFILE->m_fComponentDiameter[0], true, true);
		pFrame->SendMessage(UI_INVALIDATE_FOR_ROLLER_KINEMATICS);
	};
}


void C3DEditTurnTablePage::OnDeltaposSpinDiameter(NMHDR *pNMHDR, LRESULT *pResult)
{
	FRAME;
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	if (theApp.m_LastSettings.nMeasurementUnits == 0) {
		PROFILE->m_fComponentDiameter[0] -= ((float)pNMUpDown->iDelta * 0.01f);
	}
	else {
		PROFILE->m_fComponentDiameter[0] -= ((float)pNMUpDown->iDelta * 0.0254f);
	}
	PROFILE->m_fComponentDiameter[1] = PROFILE->m_fComponentDiameter[0];
	theApp.m_Kinematics.CalculateRollerKinematics(PROFILE->m_fComponentDiameter[0], true, true);
	pFrame->SendMessage(UI_INVALIDATE_FOR_ROLLER_KINEMATICS);
	UpdateAllControls();

	*pResult = 0;
}
