// ViewBladeThickneessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "ViewBladeThickneessDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CViewBladeThickneessDlg dialog


CViewBladeThickneessDlg::CViewBladeThickneessDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,LPCTSTR FilePath)
	: CDialog(CViewBladeThickneessDlg::IDD, pParent)
{
	
	//{{AFX_DATA_INIT(CViewBladeThickneessDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_pDlg = pDlg;
	
	m_pViewOptionsSheet = NULL;
	m_pBladeStatisticsDlg = NULL;
	m_pReportPrintDlg = NULL;

	m_bLbDown = false;
	if(theApp.m_LastSettings.m_fYBladeBorderPos < 0.01f) theApp.m_LastSettings.m_fYBladeBorderPos = 0.01f;
	if(theApp.m_LastSettings.m_fYBladeBorderPos > 0.99f) theApp.m_LastSettings.m_fYBladeBorderPos = 0.99f;

	if(theApp.m_LastSettings.fBladeMinX>=0.0f) theApp.m_LastSettings.fBladeMinX = -142.0f;
	if(theApp.m_LastSettings.fBladeMaxX<=0.0f) theApp.m_LastSettings.fBladeMaxX = 152.0f;
	if(theApp.m_LastSettings.fBladeMinThick<0.0f) theApp.m_LastSettings.fBladeMinThick = 0.0f;
	if(theApp.m_LastSettings.fBladeMaxThick<=theApp.m_LastSettings.fBladeMinThick) theApp.m_LastSettings.fBladeMaxThick = theApp.m_LastSettings.fBladeMinThick + 0.1f;
	theApp.m_LastSettings.fBladeMaxThick = 20.0f;
	m_nNumberLines = 0;

	m_bLButtonDown = false;
	m_bChangingMaxThickness = false;
	m_nLButtonAction = 0;
	m_fXCursorPos = 0.0f;

	CUSLFile File;
	CFileException e;
	CString Buff,Language;

	if(FilePath!=NULL) {
		if(!File.Open(FilePath, CFile::modeRead | CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			Language.LoadString(IDS_Error);
			MessageBox(Buff,Language,MB_ICONERROR);
		} else {
			m_Data.Retrieve(&File);
			File.Close();
		}
	}

}


void CViewBladeThickneessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewBladeThickneessDlg)
	DDX_Control(pDX, IDC_STATIC_PROFILE, m_staticProfile);
	DDX_Control(pDX, IDC_STATIC_PLAN, m_staticPlan);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewBladeThickneessDlg, CDialog)
	//{{AFX_MSG_MAP(CViewBladeThickneessDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_BUTTON_POINTER, OnButtonPointer)
	ON_COMMAND(ID_BUTTON_VERTICAL_RULER, OnButtonVerticalRuler)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_BLADE_ADDTRAFFICLIGHT, OnBladeAddtrafficlight)
	ON_COMMAND(ID_BUTTON_TRAFFICLIGHTS, OnButtonTrafficlights)
	ON_COMMAND(ID_BLADE_DELETETRAFFICLIGHT, OnBladeDeletetrafficlight)
	ON_COMMAND(ID_BUTTON_DRAG_LINE, OnButtonDragLine)
	ON_COMMAND(IDC_BUTTON_TOOLS, OnButtonTools)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_CROSSSECTION_COLOURS_TRACE, OnCrosssectionColoursTrace)
	ON_COMMAND(ID_CROSSSECTION_COLOURS_BACKGROUND, OnCrosssectionColoursBackground)
	ON_COMMAND(ID_CROSSSECTION_COLOURS_GRATICULE, OnCrosssectionColoursGraticule)
	ON_COMMAND(ID_CROSSSECTION_COLOURS_MAXIMUM, OnCrosssectionColoursMaximum)
	ON_COMMAND(ID_CROSSSECTION_COLOURS_MINIMUM, OnCrosssectionColoursMinimum)
	ON_COMMAND(ID_CROSSSECTION_COLOURS_NOMINAL, OnCrosssectionColoursNominal)
	ON_COMMAND(ID_CROSSSECTION_COLOURS_TOFTRACE, OnCrosssectionColoursToftrace)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_CROSSSECTION_GOTO, OnCrosssectionGoto)
	ON_COMMAND(ID_BUTTON_DISPLAY_AMPLITUDE, OnButtonDisplayAmplitude)
	ON_COMMAND(ID_BUTTON_DISPLAY_TOF, OnButtonDisplayTof)
	ON_COMMAND(ID_CROSSSECTION_NORMALIZE, OnCrosssectionNormalize)
	ON_COMMAND(ID_CROSSSECTION_ADDBRANEPT, OnCrosssectionAddbranept)
	ON_COMMAND(ID_FILE_STATISTICS, OnFileStatistics)
	ON_COMMAND(ID_BUTTON_SCAN_ALL, OnButtonScanAll)
	ON_COMMAND(ID_BUTTON_SCAN_CURRENT_LINE, OnButtonScanCurrentLine)
	ON_COMMAND(ID_BUTTON_SCAN_REMAINING_LINES, OnButtonScanRemainingLines)
	ON_COMMAND(ID_CROSSSECTION_NORMALIZEDAC, OnCrosssectionNormalizedac)
	ON_COMMAND(ID_PLAN_PRINTCROSSSECTION, OnPlanPrintcrosssection)
	ON_COMMAND(ID_PLAN_PRINTALLCROSSSECTIONS, OnPlanPrintallcrosssections)
	ON_COMMAND(ID_PLAN_SELECTTOPRINT, OnPlanSelecttoprint)
	ON_COMMAND(ID_PLAN_PRINTSELECTION, OnPlanPrintselection)
	ON_COMMAND(ID_PLAN_CLEARALLSELECTIONS, OnPlanClearallselections)
	ON_COMMAND(ID_PLAN_PRINTCOLOURTRAFFICLIGHTIMAGE, OnPlanPrintcolourtrafficlightimage)
	ON_COMMAND(ID_FILE_SAVE, OnFileSaveas)
	ON_COMMAND(ID_CROSSSECTION_ERASEENTIREBRANE, OnCrosssectionEraseentirebrane)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LINE, OnDeltaposSpinLine)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SPEED, OnDeltaposSpinSpeed)
	ON_CBN_SELENDOK(IDC_COMBO_SCAN_TYPE, OnSelendokScanType)
	ON_EN_CHANGE(IDC_EDIT_LINE, OnChangeEditLine)
	ON_EN_CHANGE(IDC_EDIT_SPEED, OnChangeEditSpeed)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewBladeThickneessDlg message handlers
int CViewBladeThickneessDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CRect rect;

	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndArchiveToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(2,2,0,0)) ||	!m_wndArchiveToolBar.LoadToolBar(IDR_VIEW_ARCHIVE_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	if (!m_wndToolsToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(2,2,0,0)) ||	!m_wndToolsToolBar.LoadToolBar(IDR_BLADE_VIEW_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndScanToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(2,2,0,0)) ||	!m_wndScanToolBar.LoadToolBar(IDR_BLADE_SCAN_TOOLBAR)) {
			TRACE0("Failed to create scan toolbar\n");
		return -1;      // fail to create
	}

	//	m_wndDialogBar.Create(this, IDD_VIEW_DIALOGBAR, CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY, IDD_VIEW_DIALOGBAR);
	m_wndImageDialogBar.Create(this, IDD_VIEW_THICKNESS_DIALOGBAR, CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY, IDD_VIEW_THICKNESS_DIALOGBAR);


	if (!m_wndRebar.Create(this,NULL,CBRS_ALIGN_TOP | AFX_IDW_DIALOGBAR)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	GetClientRect(rect);

	m_wndRebar.SetWindowPos(&wndTop   , 0,0,760,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndArchiveToolBar);
	m_wndRebar.AddBar(&m_wndToolsToolBar);
	m_wndRebar.AddBar(&m_wndImageDialogBar);
	m_wndRebar.AddBar(&m_wndScanToolBar);

	
	
	return 0;
}

void CViewBladeThickneessDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}

void CViewBladeThickneessDlg::OnClose()
{
	
	theApp.m_LastSettings.bWindowOpenOnLastExit[CViewBladeThickneessDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=FALSE;

	CDialog::OnClose();
	DestroyWindow();
}

void CViewBladeThickneessDlg::OnDestroy()
{
	

	GetWindowRect(theApp.m_LastSettings.RectWindowPos[CViewBladeThickneessDlg::IDD % MAXIMUMWINDOWSIDNUMBER]);

	CDialog::OnDestroy();

	*m_pDlg = NULL;
}

BOOL CViewBladeThickneessDlg::OnInitDialog()
{
	

	CDialog::OnInitDialog();
	CString Buff;


	GetClientRect(m_rrClient);

	CRect rect = theApp.m_LastSettings.RectWindowPos[CViewBladeThickneessDlg::IDD % MAXIMUMWINDOWSIDNUMBER];
	if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty()) {
		SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	};


	CComboBox* pCombo = (CComboBox* ) m_wndImageDialogBar.GetDlgItem(IDC_COMBO_SCAN_TYPE);
	pCombo->ResetContent();
	pCombo->AddString(_T("Outside"));
	pCombo->AddString(_T("Inside"));

	

	UpdateAllControls();

	SendMessage(WM_SIZE);

	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewBladeThickneessDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	CRect rrProfile,rrPlan,rrReBar;
	
	
	if(GetSafeHwnd()) {
		GetClientRect(&m_rrClient);

		m_staticProfile.GetWindowRect(&rrProfile);
		ScreenToClient(&rrProfile);
		m_staticPlan.GetWindowRect(&rrPlan);
		ScreenToClient(&rrPlan);

		m_wndRebar.GetWindowRect(&rrReBar);
		ScreenToClient(&rrReBar);
		m_rrClient.top = rrReBar.bottom + 4;

		rrProfile = m_rrClient;
		rrProfile.bottom = (int)((float)m_rrClient.Height() * theApp.m_LastSettings.m_fYBladeBorderPos) - 4;
		m_staticProfile.SetWindowPos( NULL , rrProfile.left, rrProfile.top, rrProfile.Width(), rrProfile.Height(), NULL);

		rrPlan = m_rrClient;
		rrPlan.top =(int)((float)m_rrClient.Height() * theApp.m_LastSettings.m_fYBladeBorderPos) + 4;
		m_staticPlan.SetWindowPos( NULL , rrPlan.left, rrPlan.top, rrPlan.Width(), rrPlan.Height(), NULL);

		m_rrBorder = rrPlan;
		m_rrBorder.top = rrProfile.bottom;
		m_rrBorder.bottom = rrPlan.top;

		Invalidate(false);
	}
	
}

void CViewBladeThickneessDlg::UpdateAllControls()
{
	if (GetSafeHwnd() == NULL) return;
	
	CString Buff;
	CEdit *pEdit;
	CSpinButtonCtrl* pSpinLineNumber = (CSpinButtonCtrl*) m_wndImageDialogBar.GetDlgItem(IDC_SPIN_LINE);
	CSpinButtonCtrl* pSpinSpeed = (CSpinButtonCtrl*) m_wndImageDialogBar.GetDlgItem(IDC_SPIN_SPEED);
	CComboBox* pCombo = (CComboBox* ) m_wndImageDialogBar.GetDlgItem(IDC_COMBO_SCAN_TYPE);

	pCombo->SetCurSel(theApp.m_Thread.m_nBladeSide);

	if(m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide]) {
		m_nNumberLines = m_Data.m_nLineBladeL[ theApp.m_Thread.m_nBladeSide ];

		if(theApp.m_nBladeLine >= m_nNumberLines) theApp.m_nBladeLine = m_nNumberLines-1;
		pSpinLineNumber->SetRange(0,m_nNumberLines);
		pSpinLineNumber->SetPos(theApp.m_nBladeLine);

		Buff.Format(_T("%d %s"),theApp.m_nBladeLine+1,m_Data.m_LineBlade[ theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_wStr);
		pEdit = (CEdit* ) m_wndImageDialogBar.GetDlgItem(IDC_EDIT_LINE);
		pEdit->SetWindowText(Buff);

		pSpinSpeed->SetRange(50,400);
		pSpinLineNumber->SetPos((int)theApp.m_Profile.m_fScanSpeed);
	
		Buff.Format(_T("%.0f mm/s"),theApp.m_Profile.m_fScanSpeed);
		pEdit = (CEdit* ) m_wndImageDialogBar.GetDlgItem(IDC_EDIT_SPEED);
		pEdit->SetWindowText(Buff);
	
	
	}


	SetToolBarCheckedState();
	SetWindowTitle();
}

void CViewBladeThickneessDlg::OnPaint() 
{
	
	CPaintDC dc(this); // device context for painting
	
	m_Data.FindLineMaxXExtent(theApp.m_Thread.m_nBladeSide,&theApp.m_LastSettings.fBladeMinX,&theApp.m_LastSettings.fBladeMaxX);
	RenderProfile(&dc);


	CRect rrView;
	m_staticPlan.GetWindowRect(rrView);
	rrView.DeflateRect(1,1);
	ScreenToClient(rrView);
	RenderPlan(&dc, &rrView, 0);
}

void CViewBladeThickneessDlg::PrintCallBack(CPaintDC *pDC,CRect* prr,int nDevice)
{

	RenderPlan(pDC,prr,nDevice);
}

void CViewBladeThickneessDlg::RenderProfile(CPaintDC *pDC)
{
	
	CRect rrView,rr;
	CDC  dcMem;
	CBitmap bmp;
	CString Buff;
	int nPnX,nPnY,ii,nCoordL;
	CCoord Cp;
	float fX,fStep,fLastThickness;

	LOGFONT lf;
	memset((void*)&lf,0,sizeof(lf));
	lf.lfHeight = theApp.m_LastSettings.nBladeFontSize;
	theApp.SetFontLanguage(&lf);
	CFont fontHorizontal;
	CFont fontVertical;
	fontHorizontal.CreateFontIndirect(&lf);
	lf.lfEscapement = -900;
	fontVertical.CreateFontIndirect(&lf);


	m_staticProfile.GetWindowRect(rrView);
	rrView.DeflateRect(1,1);
	ScreenToClient(rrView);
	rr.left=rr.top=0;
	rr.right=rrView.Width();
	rr.bottom=rrView.Height();

	COLORREF *pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
	ZeroMemory(pArray,(rr.Width()+2) * (rr.Height()+2) * sizeof COLORREF);

	bmp.CreateCompatibleBitmap(pDC,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),pArray);
	dcMem.CreateCompatibleDC(pDC);

	dcMem.SetBkMode(TRANSPARENT);
	dcMem.SetTextColor(theApp.m_LastSettings.rgbCrossSection[RGB_FONT]);
	CPen penWhite(PS_SOLID,1,RGB(255,255,255));
	CBitmap *pOldBmp = dcMem.SelectObject(&bmp);
	CPen* pOldPen = dcMem.SelectObject(&penWhite);
	CFont* pOldFont = dcMem.SelectObject(&fontHorizontal);
	CPen penMin(PS_SOLID,1,theApp.m_LastSettings.rgbCrossSection[RGB_MINIMUM]);
	CPen penMax(PS_SOLID,1,theApp.m_LastSettings.rgbCrossSection[RGB_MAXIMUM]);
	CPen penNom(PS_SOLID,1,theApp.m_LastSettings.rgbCrossSection[RGB_NOMINAL]);
	CPen penAct(PS_SOLID,1,theApp.m_LastSettings.rgbCrossSection[RGB_TOF_TRACE]);
	CPen penAmp(PS_SOLID,1,theApp.m_LastSettings.rgbCrossSection[RGB_AMP_TRACE]);

	//drawgraticule
	Buff.Format(_T("%.01f"),theApp.m_LastSettings.fBladeMinX);
	CSize size = dcMem.GetTextExtent(Buff);
	m_rrProfileGrat=rr;
	m_rrProfileGrat.left += size.cx;
	m_rrProfileGrat.right -= size.cx;
	m_rrProfileGrat.top += (2 * size.cy);
	m_rrProfileGrat.bottom -= (2 * size.cy);
	dcMem.MoveTo(m_rrProfileGrat.TopLeft());
	dcMem.LineTo(m_rrProfileGrat.left,m_rrProfileGrat.bottom);
	dcMem.LineTo(m_rrProfileGrat.BottomRight());
	dcMem.LineTo(m_rrProfileGrat.right,m_rrProfileGrat.top);


	//Horizontal scale
	fX = (float)((int)(theApp.m_LastSettings.fBladeMinX / 10.0f) * 10);
	for(fX;fX<=theApp.m_LastSettings.fBladeMaxX;fX+=50.0f) {
		nPnX = m_rrProfileGrat.left + (int)((fX-theApp.m_LastSettings.fBladeMinX) * (float)m_rrProfileGrat.Width() / (theApp.m_LastSettings.fBladeMaxX - theApp.m_LastSettings.fBladeMinX));
		dcMem.MoveTo(nPnX,m_rrProfileGrat.bottom);
		dcMem.LineTo(nPnX,m_rrProfileGrat.bottom+3);
		Buff.Format(_T("%.01f"),fX);
		size = dcMem.GetTextExtent(Buff);
		dcMem.TextOut(nPnX-size.cx/2,m_rrProfileGrat.bottom+3,Buff);
	}

	//Vertical scale
	dcMem.SelectObject(&fontVertical);

	if((theApp.m_LastSettings.fBladeMaxThick - theApp.m_LastSettings.fBladeMinThick) <= 10.0f) {
		fStep = 1.0f;
	} else {
		fStep = 5.0f;
	};

	float fY = (float)((int)(theApp.m_LastSettings.fBladeMinThick / 10.0f) * 10);
	for(fY;fY<=theApp.m_LastSettings.fBladeMaxThick;fY+=fStep) {
		nPnY = m_rrProfileGrat.bottom - (int)((fY-theApp.m_LastSettings.fBladeMinThick) * (float)m_rrProfileGrat.Height() / (theApp.m_LastSettings.fBladeMaxThick - theApp.m_LastSettings.fBladeMinThick));
		dcMem.MoveTo(m_rrProfileGrat.left,nPnY);
		dcMem.LineTo(m_rrProfileGrat.left-3,nPnY);
		Buff.Format(_T("%.01f"),fY);
		size = dcMem.GetTextExtent(Buff);
		dcMem.TextOut(m_rrProfileGrat.left-3,nPnY-size.cx/2,Buff);
	}

	//draw vertical ruler
	float fNomThickness;
	float fActualThickness;
	dcMem.SelectObject(&fontHorizontal);
	if(m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide]) {
		if((m_nLButtonAction == 1) || (m_nLButtonAction == 2)) {
			nPnX = m_rrProfileGrat.left + (int)((m_fXCursorPos - theApp.m_LastSettings.fBladeMinX) * (float)m_rrProfileGrat.Width() / (theApp.m_LastSettings.fBladeMaxX - theApp.m_LastSettings.fBladeMinX));
			dcMem.MoveTo(nPnX,m_rrProfileGrat.bottom-1);
			dcMem.LineTo(nPnX,m_rrProfileGrat.top);

			nPnY = m_rrProfileGrat.top;
			Buff.Format(_T("%.02f mm"),m_fXCursorPos);
			size = dcMem.GetTextExtent(Buff);
			if(nPnX < m_rrProfileGrat.Width()/2) {
				dcMem.TextOut(nPnX + 2,nPnY,Buff);
			} else {
				dcMem.TextOut(nPnX - size.cx - 2,nPnY,Buff);
			}

			nPnY += size.cy;
			m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].GetNominalThickness(m_fXCursorPos-m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_Matrix.m[3][0],&Buff,&fNomThickness);
			size = dcMem.GetTextExtent(Buff);
			if(nPnX < m_rrProfileGrat.Width()/2) {
				dcMem.TextOut(nPnX + 2,nPnY,Buff);
			} else {
				dcMem.TextOut(nPnX - size.cx - 2,nPnY,Buff);
			}

			nPnY += size.cy;
			m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].GetActualThickness(m_fXCursorPos,&Buff,&fActualThickness);
			size = dcMem.GetTextExtent(Buff);
			if(nPnX < m_rrProfileGrat.Width()/2) {
				dcMem.TextOut(nPnX + 2,nPnY,Buff);
			} else {
				dcMem.TextOut(nPnX - size.cx - 2,nPnY,Buff);
			}

			nPnY += size.cy;
			Buff.Format(_T("Thinckness Deviation = %.03fmm"),fActualThickness - fNomThickness);
			size = dcMem.GetTextExtent(Buff);
			if(nPnX < m_rrProfileGrat.Width()/2) {
				dcMem.TextOut(nPnX + 2,nPnY,Buff);
			} else {
				dcMem.TextOut(nPnX - size.cx - 2,nPnY,Buff);
			}
/*
			if(m_nLButtonAction == 2) {		//print traffic light data
				nPnY += size.cy;
				m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].GetNominalActualThickness(m_nTrafficLight,&fNomThickness,&fActualThickness);
				Buff.Format(_T("TL %d Nom Thickness = %.03fmm"),m_nTrafficLight, fNomThickness);
				size = dcMem.GetTextExtent(Buff);
				if(nPnX < m_rrProfileGrat.Width()/2) {
					dcMem.TextOut(nPnX + 2,nPnY,Buff);
				} else {
					dcMem.TextOut(nPnX - size.cx - 2,nPnY,Buff);
				}
				nPnY += size.cy;
				Buff.Format(_T("TL Act Thickness = %.03fmm"), fActualThickness);
				size = dcMem.GetTextExtent(Buff);
				if(nPnX < m_rrProfileGrat.Width()/2) {
					dcMem.TextOut(nPnX + 2,nPnY,Buff);
				} else {
					dcMem.TextOut(nPnX - size.cx - 2,nPnY,Buff);
				}
				nPnY += size.cy;
				Buff.Format(_T("TL Dev Thickness = %.03fmm"), fActualThickness - fNomThickness);
				size = dcMem.GetTextExtent(Buff);
				if(nPnX < m_rrProfileGrat.Width()/2) {
					dcMem.TextOut(nPnX + 2,nPnY,Buff);
				} else {
					dcMem.TextOut(nPnX - size.cx - 2,nPnY,Buff);
				}

			}
*/
		}

		//draw nominal thickness;
		if(theApp.m_LastSettings.nBladeDisplayMask & DISPLAY_NOM_THICKNESS) {
			fLastThickness=0.0f;
			nCoordL = m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_nCoordL;
			dcMem.SelectObject(&penNom);
			for(ii=0;ii<nCoordL;ii++) {
				m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].GetCoord(ii,&Cp);

				fX = Cp.Side0.fX + m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_Matrix.m[3][0];
				nPnX = m_rrProfileGrat.left + (int)((fX - theApp.m_LastSettings.fBladeMinX) * (float)m_rrProfileGrat.Width() / (theApp.m_LastSettings.fBladeMaxX - theApp.m_LastSettings.fBladeMinX));
				nPnY = m_rrProfileGrat.bottom - (int)((Cp.fNomThickness-theApp.m_LastSettings.fBladeMinThick) * (float)m_rrProfileGrat.Height() / (theApp.m_LastSettings.fBladeMaxThick - theApp.m_LastSettings.fBladeMinThick));

				if((ii==0) || (fLastThickness == 0.0f) || (Cp.fNomThickness==0.0f)) {
					dcMem.MoveTo(nPnX,nPnY);
				} else {
					dcMem.LineTo(nPnX,nPnY);
				}
				fLastThickness = Cp.fNomThickness;
			}
		}

		//draw min thickness
		if(theApp.m_LastSettings.nBladeDisplayMask & DISPLAY_MIN_TOLERANCE) {
			nCoordL = m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_nCoordL;
			dcMem.SelectObject(&penMin);
			for(ii=0;ii<nCoordL;ii++) {
				m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].GetCoord(ii,&Cp);

				fX = Cp.Side0.fX + m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_Matrix.m[3][0];
				nPnX = m_rrProfileGrat.left + (int)((fX - theApp.m_LastSettings.fBladeMinX) * (float)m_rrProfileGrat.Width() / (theApp.m_LastSettings.fBladeMaxX - theApp.m_LastSettings.fBladeMinX));
				nPnY = m_rrProfileGrat.bottom - (int)((Cp.fMinThickness-theApp.m_LastSettings.fBladeMinThick) * (float)m_rrProfileGrat.Height() / (theApp.m_LastSettings.fBladeMaxThick - theApp.m_LastSettings.fBladeMinThick));

				if((ii==0) || (fLastThickness == 0.0f) || (Cp.fMinThickness==0.0f)) {
					dcMem.MoveTo(nPnX,nPnY);
				} else {
					dcMem.LineTo(nPnX,nPnY);
				}
				fLastThickness = Cp.fMinThickness;
			}
		}

		//draw max thickness
		if(theApp.m_LastSettings.nBladeDisplayMask & DISPLAY_MAX_TOLERANCE) {
			nCoordL = m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_nCoordL;
			dcMem.SelectObject(&penMax);
			for(ii=0;ii<nCoordL;ii++) {
				m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].GetCoord(ii,&Cp);

				fX = Cp.Side0.fX + m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_Matrix.m[3][0];
				nPnX = m_rrProfileGrat.left + (int)((fX - theApp.m_LastSettings.fBladeMinX) * (float)m_rrProfileGrat.Width() / (theApp.m_LastSettings.fBladeMaxX - theApp.m_LastSettings.fBladeMinX));
				nPnY = m_rrProfileGrat.bottom - (int)((Cp.fMaxThickness-theApp.m_LastSettings.fBladeMinThick) * (float)m_rrProfileGrat.Height() / (theApp.m_LastSettings.fBladeMaxThick - theApp.m_LastSettings.fBladeMinThick));

				if((ii==0) || (fLastThickness == 0.0f) || (Cp.fMaxThickness==0.0f)) {
					dcMem.MoveTo(nPnX,nPnY);
				} else {
					dcMem.LineTo(nPnX,nPnY);
				}
				fLastThickness = Cp.fMaxThickness;
			}
		}

		//draw actual thickness;
		if(theApp.m_LastSettings.nBladeDisplayMask & DISPLAY_ACT_THICKNESS) {
			fLastThickness = 0.0f;
			nCoordL = m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_nCoordL;
			dcMem.SelectObject(&penAct);
			for(ii=0;ii<nCoordL;ii++) {
				m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].GetCoord(ii,&Cp);

				nPnX = m_rrProfileGrat.left + (int)((Cp.Side0.fX - theApp.m_LastSettings.fBladeMinX) * (float)m_rrProfileGrat.Width() / (theApp.m_LastSettings.fBladeMaxX - theApp.m_LastSettings.fBladeMinX));
				nPnY = m_rrProfileGrat.bottom - (int)((Cp.fActThickness-theApp.m_LastSettings.fBladeMinThick) * (float)m_rrProfileGrat.Height() / (theApp.m_LastSettings.fBladeMaxThick - theApp.m_LastSettings.fBladeMinThick));

//				if((ii==0) || (Cp.fActThickness == 0.0f) || (fLastThickness == 0.0f)) {
				if(ii==0) {
					dcMem.MoveTo(nPnX,nPnY);
				} else {
					dcMem.LineTo(nPnX,nPnY);
				}
				fLastThickness = Cp.fActThickness;
			}
		};

		//draw traffic lights
		for(ii=0;ii<nCoordL;ii++) {
			m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].GetCoord(ii,&Cp);

			if(Cp.nType & TRAFFIC_LIGHT) {
				fX = Cp.Side0.fX + m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_Matrix.m[3][0];
				nPnX = m_rrProfileGrat.left + (int)((fX - theApp.m_LastSettings.fBladeMinX) * (float)m_rrProfileGrat.Width() / (theApp.m_LastSettings.fBladeMaxX - theApp.m_LastSettings.fBladeMinX));
				nPnY = m_rrProfileGrat.bottom - (int)((Cp.fNomThickness-theApp.m_LastSettings.fBladeMinThick) * (float)m_rrProfileGrat.Height() / (theApp.m_LastSettings.fBladeMaxThick - theApp.m_LastSettings.fBladeMinThick));

				dcMem.MoveTo(nPnX-3,nPnY-3);
				dcMem.LineTo(nPnX+4,nPnY+4);
				dcMem.MoveTo(nPnX-3,nPnY+3);
				dcMem.LineTo(nPnX+4,nPnY-4);
			}
		}

		
		if(theApp.m_LastSettings.nBladeDisplayMask & DISPLAY_AMPLITUDE) {
			nCoordL = m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_nCoordL;
			dcMem.SelectObject(&penAmp);
			for(ii=0;ii<nCoordL;ii++) {
				m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].GetCoord(ii,&Cp);

				nPnX = m_rrProfileGrat.left + (int)((Cp.Side0.fX - theApp.m_LastSettings.fBladeMinX) * (float)m_rrProfileGrat.Width() / (theApp.m_LastSettings.fBladeMaxX - theApp.m_LastSettings.fBladeMinX));
				nPnY = m_rrProfileGrat.bottom - (int)((float)Cp.nAmp * (float)m_rrProfileGrat.Height() / 127.0f);

				if(ii==0) {
					dcMem.MoveTo(nPnX,nPnY);
				} else {
					dcMem.LineTo(nPnX,nPnY);
				}
			}
		}		
		
		//render translation value
		fX = m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_Matrix.m[3][0];
		Buff.Format(_T("Leading edge translation : %.01f mm"),fX);
		size = dcMem.GetTextExtent(Buff);
		dcMem.TextOut(m_rrProfileGrat.CenterPoint().x-size.cx/2,3,Buff);

	}



	pDC->BitBlt(rrView.left,rrView.top,rrView.Width(),rrView.Height(),&dcMem,0,0,SRCCOPY);

	dcMem.SelectObject(pOldFont);
	dcMem.SelectObject(pOldBmp);
	dcMem.SelectObject(pOldPen);
	delete pArray;
}

void CViewBladeThickneessDlg::RenderPlan(CPaintDC *pDC,CRect *prrView,int nDevice)
{
	
	int nCoordL,nPnX,nPnY;
	float fX,fActThickness;
	CRect rr,rrLight;
	CDC  dcMem;
	CBitmap bmp;
	CString Buff;
	CCoord Cp;
	CPoint *ptLeft;
	CPoint *ptRight;
	CBrush brushMin(theApp.m_LastSettings.rgbCrossSection[RGB_MINIMUM]);
	CBrush brushMax(theApp.m_LastSettings.rgbCrossSection[RGB_MAXIMUM]);
	CBrush brushWhite(RGB(255,255,255));
	int nCircleRadius = 4;
	if(nDevice == 1) nCircleRadius *= 12;


	LOGFONT lf;
	memset((void*)&lf,0,sizeof(lf));
	lf.lfHeight = theApp.m_LastSettings.nBladeFontSize;
	if(nDevice == 1) lf.lfHeight *= 12;

	theApp.SetFontLanguage(&lf);
	CFont fontHorizontal;
	CFont fontVertical;
	fontHorizontal.CreateFontIndirect(&lf);
	lf.lfEscapement = -900;
	fontVertical.CreateFontIndirect(&lf);


	rr.left=rr.top=0;
	rr.right=prrView->Width();
	rr.bottom=prrView->Height();

	COLORREF *pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
	if(nDevice == 0) {
		ZeroMemory(pArray,(rr.Width()+2) * (rr.Height()+2) * sizeof COLORREF);
	} else {
		FillMemory(pArray,(rr.Width()+2) * (rr.Height()+2) * sizeof COLORREF, 0xff);
	}

	bmp.CreateCompatibleBitmap(pDC,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),pArray);
	dcMem.CreateCompatibleDC(pDC);

	dcMem.SetBkMode(TRANSPARENT);
	dcMem.SetTextColor(theApp.m_LastSettings.rgbCrossSection[RGB_FONT]);
	CPen penWhite(PS_SOLID,1,RGB(255,255,255));
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	CPen penGrey(PS_SOLID,1,RGB(50,50,50));
	CBitmap *pOldBmp = dcMem.SelectObject(&bmp);
	CPen* pOldPen = dcMem.SelectObject(&penWhite);
	CFont* pOldFont = dcMem.SelectObject(&fontHorizontal);
	CBrush* pOldBrush = dcMem.SelectObject(&brushWhite);


	//drawgraticule
	Buff.Format(_T("%.01f"),theApp.m_LastSettings.fBladeMinX);
	CSize size = dcMem.GetTextExtent(Buff);
	m_rrPlanGrat=rr;
	m_rrPlanGrat.left += size.cx;
	m_rrPlanGrat.right -= size.cx;
	m_rrPlanGrat.top += (2 * size.cy);
	m_rrPlanGrat.bottom -= (2 * size.cy);
	dcMem.MoveTo(m_rrPlanGrat.TopLeft());
	dcMem.LineTo(m_rrPlanGrat.left,m_rrPlanGrat.bottom);
	dcMem.LineTo(m_rrPlanGrat.BottomRight());
	dcMem.LineTo(m_rrPlanGrat.right,m_rrPlanGrat.top);


	if(m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide]) {

		ptLeft = new CPoint[m_nNumberLines];
		ptRight = new CPoint[m_nNumberLines];
		for(int nLine=0;nLine < m_nNumberLines;nLine++) {
			m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].GetFirstCoord(&Cp);
			ptLeft[nLine].x = m_rrPlanGrat.left + (int)((Cp.Side0.fX - theApp.m_LastSettings.fBladeMinX) * (float)m_rrPlanGrat.Width() / (theApp.m_LastSettings.fBladeMaxX - theApp.m_LastSettings.fBladeMinX));
			ptLeft[nLine].y = m_rrPlanGrat.top + MulDiv(nLine,m_rrPlanGrat.Height(),m_nNumberLines);

			m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].GetLastCoord(&Cp);
			ptRight[nLine].x = m_rrPlanGrat.left + (int)((Cp.Side0.fX - theApp.m_LastSettings.fBladeMinX) * (float)m_rrPlanGrat.Width() / (theApp.m_LastSettings.fBladeMaxX - theApp.m_LastSettings.fBladeMinX));
			ptRight[nLine].y = ptLeft[nLine].y;
		}

		//draw horizontal lines
		for(int nLine = 0;nLine<m_nNumberLines;nLine++) {
			if(nDevice == 0) {
				if(nLine == theApp.m_nBladeLine) {
					dcMem.SelectObject(&penWhite);
				} else {
					dcMem.SelectObject(&penGrey);
				}
			} else {
				dcMem.SelectObject(&penBlack);
			};

			dcMem.MoveTo(ptLeft[nLine]);
			dcMem.LineTo(ptRight[nLine]);

			Buff.Format(_T("%d"),nLine + 1);
			size = dcMem.GetTextExtent(Buff);
			dcMem.TextOut(m_rrPlanGrat.left-3-size.cx,ptLeft[nLine].y-size.cy/2,Buff);
		}
		

		//draw vertical end of lines
		if(nDevice == 0) {
			dcMem.SelectObject(&penWhite);
		} else {
			dcMem.SelectObject(&penBlack);
		};
		dcMem.Polyline(ptLeft,m_nNumberLines);
		dcMem.Polyline(ptRight,m_nNumberLines);

		//draw traffic lights
		for(int nLine=0;nLine < m_nNumberLines;nLine++) {

			nCoordL = m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].m_nCoordL;
			for(int nSample=0;nSample<nCoordL;nSample++) {

				m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].GetCoord(nSample,&Cp);
				if(Cp.nType & TRAFFIC_LIGHT) {

					fX = Cp.Side0.fX + m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].m_Matrix.m[3][0];
					nPnX = m_rrPlanGrat.left + (int)((fX - theApp.m_LastSettings.fBladeMinX) * (float)m_rrPlanGrat.Width() / (theApp.m_LastSettings.fBladeMaxX - theApp.m_LastSettings.fBladeMinX));
					nPnY = m_rrPlanGrat.top + MulDiv(nLine,m_rrPlanGrat.Height(),m_nNumberLines);

					m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].GetActualThickness(fX,NULL,&fActThickness);

					if(fActThickness >= Cp.fMaxThickness) {
						dcMem.SelectObject(&brushMax);
					} else {
						if(fActThickness <= Cp.fMinThickness) {
							dcMem.SelectObject(&brushMin);
						} else {
							dcMem.SelectObject(&brushWhite);
						}
					}
					
					rrLight.left = nPnX-nCircleRadius;
					rrLight.right = nPnX+nCircleRadius;
					rrLight.top = nPnY-nCircleRadius;
					rrLight.bottom = nPnY+nCircleRadius;
					dcMem.Ellipse(&rrLight);

				}
			}
		}

		//draw line selected;
		Buff = L"*";
		size = dcMem.GetTextExtent(Buff);
		for(int nLine=0;nLine < m_nNumberLines;nLine++) {
			if(m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].m_nUseToPrint) {
				dcMem.TextOut(ptRight[nLine].x,ptRight[nLine].y-size.cy/2+2,Buff);
			}
		}


		delete ptLeft;
		delete ptRight;

	}
	pDC->BitBlt(prrView->left,prrView->top,prrView->Width(),prrView->Height(),&dcMem,0,0,SRCCOPY);

	dcMem.SelectObject(pOldFont);
	dcMem.SelectObject(pOldBmp);
	dcMem.SelectObject(pOldPen);
	dcMem.SelectObject(pOldBrush);
	delete pArray;

}


int CViewBladeThickneessDlg::MinMax(int *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}

void CViewBladeThickneessDlg::OnChangeEditLine()
{
	
	CString	Buff;
	int nTemp = theApp.m_nBladeLine;

	CEdit* pEdit = (CEdit* ) m_wndImageDialogBar.GetDlgItem(IDC_EDIT_LINE);
	pEdit->GetWindowText(Buff);

	theApp.m_nBladeLine = _ttoi(Buff) - 1;
	MinMax(&theApp.m_nBladeLine,0,m_nNumberLines-1);

	if(nTemp - theApp.m_nBladeLine) {

		CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*) m_wndImageDialogBar.GetDlgItem(IDC_SPIN_LINE);
		pSpin->SetPos(theApp.m_nBladeLine);
		UpdateAllControls();

		Invalidate(FALSE);
	}

}

void CViewBladeThickneessDlg::OnDeltaposSpinLine(NMHDR* pNMHDR, LRESULT* pResult)
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	int nTemp = theApp.m_nBladeLine;

	theApp.m_nBladeLine += pNMUpDown->iDelta;
	MinMax(&theApp.m_nBladeLine,0,m_nNumberLines-1);

	if(nTemp - theApp.m_nBladeLine) {
		Buff.Format(_T("%d %s"),theApp.m_nBladeLine+1,m_Data.m_LineBlade[ theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_wStr);
		CEdit* pEdit = (CEdit* ) m_wndImageDialogBar.GetDlgItem(IDC_EDIT_LINE);
		pEdit->SetWindowText(Buff);

		SetProfileMinMaxThickness();

		Invalidate(FALSE);
		RefreshStatisticsDlg(FILLLIST);
	}
	*pResult = 0;
}

void CViewBladeThickneessDlg::OnChangeEditSpeed()
{
	
	CString	Buff;
	float fTemp = PROFILE->m_fScanSpeed;

	CEdit* pEdit = (CEdit* ) m_wndImageDialogBar.GetDlgItem(IDC_EDIT_SPEED);
	pEdit->GetWindowText(Buff);

	_WTOF(Buff,PROFILE->m_fScanSpeed);
	if(PROFILE->m_fScanSpeed<50.0f) PROFILE->m_fScanSpeed = 50.0f;
	if(PROFILE->m_fScanSpeed>400.0f) PROFILE->m_fScanSpeed = 400.0f;

	if(fTemp - PROFILE->m_fScanSpeed ) {

		CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*) m_wndImageDialogBar.GetDlgItem(IDC_SPIN_SPEED);
		pSpin->SetPos((int)PROFILE->m_fScanSpeed);
	}

}

void CViewBladeThickneessDlg::OnDeltaposSpinSpeed(NMHDR* pNMHDR, LRESULT* pResult)
{
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	float fTemp = PROFILE->m_fScanSpeed;

	PROFILE->m_fScanSpeed += (float)(pNMUpDown->iDelta*10);
	if(PROFILE->m_fScanSpeed<50.0f) PROFILE->m_fScanSpeed = 50.0f;
	if(PROFILE->m_fScanSpeed>400.0f) PROFILE->m_fScanSpeed = 400.0f;

	if(fTemp - PROFILE->m_fScanSpeed ) {
		Buff.Format(_T("%.0f mm/s"),PROFILE->m_fScanSpeed);
		CEdit* pEdit = (CEdit* ) m_wndImageDialogBar.GetDlgItem(IDC_EDIT_SPEED);
		pEdit->SetWindowText(Buff);

	}
	*pResult = 0;
}

void CViewBladeThickneessDlg::OnSelendokScanType()
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CComboBox* pCmb = (CComboBox* ) m_wndImageDialogBar.GetDlgItem(IDC_COMBO_SCAN_TYPE);

	theApp.m_Thread.m_nBladeSide=pCmb->GetCurSel();

	switch(theApp.m_Thread.m_nBladeSide) {
	case OUTSIDE:	PROFILE->CopyAllLines(SCANLINES, OUTSIDELINES);
			PROFILE->CopyAllLines(BRAINLINES, BRANEOUTSIDELINES);
		break;
	case INSIDE:	PROFILE->CopyAllLines(SCANLINES, INSIDELINES);
			PROFILE->CopyAllLines(BRAINLINES, BRANEINSIDELINES);
		break;
	}

	UpdateAllControls();

	Invalidate(FALSE);
	RefreshStatisticsDlg(FILLLIST);
	pFrame->SendMessage(UI_UPDATE_PROFILE_DLG);

}

void CViewBladeThickneessDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	CRect rrPlan,rrProfile;
	float fXPos;
	CRect rrLeftThinkness;

	m_bLButtonDown = true;

	m_staticPlan.GetWindowRect(rrPlan);
	rrPlan.DeflateRect(1,1);
	ScreenToClient(rrPlan);

	if(rrPlan.PtInRect(point)) {
		point.y -= rrPlan.top;
		theApp.m_nBladeLine = MulDiv(point.y-m_rrPlanGrat.top, m_nNumberLines, m_rrPlanGrat.Height());
		MinMax(&theApp.m_nBladeLine,0,m_nNumberLines-1);
		SetProfileMinMaxThickness();

		UpdateAllControls();
		Invalidate(FALSE);
		RefreshStatisticsDlg(FILLLIST);

		return;
	}
	
	m_staticProfile.GetWindowRect(rrProfile);
	rrProfile.DeflateRect(1,1);
	ScreenToClient(rrProfile);

	//Change the max thickness;
	rrLeftThinkness = rrProfile;
	rrLeftThinkness.right = m_rrProfileGrat.left;
	if(rrLeftThinkness.PtInRect(point)) {

		m_fThicknessLbDown = (float)(m_rrProfileGrat.bottom - point.y + rrProfile.top) * (theApp.m_LastSettings.fBladeMaxThick - theApp.m_LastSettings.fBladeMinThick) / (float)m_rrProfileGrat.Height();
		if(m_fThicknessLbDown<0.0f) m_fThicknessLbDown = 0.0f;
		m_bChangingMaxThickness = true;
	}


	if(m_rrProfileGrat.PtInRect(point)) {
		point.x -= rrProfile.left;
		fXPos = theApp.m_LastSettings.fBladeMinX + (float)(point.x - m_rrProfileGrat.left) * (theApp.m_LastSettings.fBladeMaxX - theApp.m_LastSettings.fBladeMinX) / (float)m_rrProfileGrat.Width();

		switch(m_nLButtonAction) {
		case 0:
			break;
		case 1: m_fXCursorPos = fXPos;
			break;
		case 2:	
			if(m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide]) {
				fXPos -= m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_Matrix.m[3][0];
				m_nTrafficLight = m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].FindNearestTrafficLight(fXPos);
				m_fXCursorPos = fXPos + m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_Matrix.m[3][0];
			}
			break;
		case 3:
			if(m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide]) {
				m_fXCursorPos = fXPos - m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_Matrix.m[3][0];
			}
			break;
		}

		Invalidate(FALSE);
		return;
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CViewBladeThickneessDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLButtonDown = false;
	m_bChangingMaxThickness = false;
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CViewBladeThickneessDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	
	CRect rrProfile;
	float fXPos;

	if( m_bLButtonDown == true ) {

		m_staticProfile.GetWindowRect(rrProfile);
		rrProfile.DeflateRect(1,1);
		ScreenToClient(rrProfile);

		if(m_bChangingMaxThickness == true) {

			theApp.m_LastSettings.fBladeMaxThick = (m_fThicknessLbDown * (float)m_rrProfileGrat.Height() / (float)(m_rrProfileGrat.bottom - point.y + rrProfile.top) ) + theApp.m_LastSettings.fBladeMinThick;
			if(theApp.m_LastSettings.fBladeMaxThick <= theApp.m_LastSettings.fBladeMinThick) theApp.m_LastSettings.fBladeMaxThick = theApp.m_LastSettings.fBladeMinThick + 0.5f;

			Invalidate(FALSE);

		} else {

			if(rrProfile.PtInRect(point)) {
				point.x -= rrProfile.left;
				fXPos = theApp.m_LastSettings.fBladeMinX + (float)(point.x - m_rrProfileGrat.left) * (theApp.m_LastSettings.fBladeMaxX - theApp.m_LastSettings.fBladeMinX) / (float)m_rrProfileGrat.Width();

				switch(m_nLButtonAction) {
				case 0:
					break;
				case 1: m_fXCursorPos = fXPos;
					break;
				case 2:
					if(m_nTrafficLight<0) m_nTrafficLight = 0;
					if(m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide]) {
						fXPos -= m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_Matrix.m[3][0];
						m_nTrafficLight = m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].MoveTrafficLight(m_nTrafficLight,fXPos);
						m_fXCursorPos = fXPos + m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_Matrix.m[3][0];
					}
					break;
				case 3: 
					if(m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide]) {
						m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_Matrix.m[3][0] = (fXPos - m_fXCursorPos);
					}
					break;
				}
				Invalidate(FALSE);
				RefreshStatisticsDlg(FILLLIST);
			}
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CViewBladeThickneessDlg::OnButtonPointer() 
{
	m_nLButtonAction = 0;
	SetToolBarCheckedState();
	Invalidate(FALSE);
	
}

void CViewBladeThickneessDlg::OnButtonVerticalRuler() 
{
	m_nLButtonAction == 1 ? m_nLButtonAction = 0 : m_nLButtonAction = 1;
	SetToolBarCheckedState();
	Invalidate(FALSE);
}

void CViewBladeThickneessDlg::OnButtonTrafficlights() 
{
	m_nLButtonAction == 2 ? m_nLButtonAction = 0 : m_nLButtonAction = 2;
	SetToolBarCheckedState();
	Invalidate(FALSE);
}

void CViewBladeThickneessDlg::OnButtonDragLine() 
{
	m_nLButtonAction == 3 ? m_nLButtonAction = 0 : m_nLButtonAction = 3;
	SetToolBarCheckedState();
	Invalidate(FALSE);
}

BOOL CViewBladeThickneessDlg::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
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
   case ID_BUTTON_DISPLAY_TOF: strTipText.LoadString(IDS_Thickness);
	   break;
   case ID_BUTTON_DISPLAY_AMPLITUDE: strTipText.LoadString(IDS_Amplitude);
	   break;
   case ID_FILE_STATISTICS: strTipText.LoadString(IDS_Statistics);
	   break;
   case ID_BUTTON_SCAN_ALL: strTipText.LoadString(IDS_Scan_all_lines);
	   break;
   case ID_BUTTON_SCAN_CURRENT_LINE: strTipText.LoadString(IDS_Scan_current_line);
	   break;
   case ID_BUTTON_SCAN_REMAINING_LINES: strTipText.LoadString(IDS_Scan_remaining_lines);
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

void CViewBladeThickneessDlg::SetToolBarCheckedState()
{
	
	int	nIndex;

	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_POINTER))>=0)
		m_nLButtonAction==0 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_VERTICAL_RULER))>=0)
		m_nLButtonAction==1 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_TRAFFICLIGHTS))>=0)
		m_nLButtonAction==2 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_DRAG_LINE))>=0)
		m_nLButtonAction==3 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_DISPLAY_TOF))>=0)
		theApp.m_LastSettings.nBladeDisplayMask & DISPLAY_ACT_THICKNESS ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_BUTTON_DISPLAY_AMPLITUDE))>=0)
		theApp.m_LastSettings.nBladeDisplayMask & DISPLAY_AMPLITUDE ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);

}

void CViewBladeThickneessDlg::InitializeFromProfile(int nScanType)
{
	

	m_Data.Zero();
	m_Data.CopyBladeLinesFromProfile();
	m_Data.m_nScanType = nScanType;
	m_Data.m_bModified = true;

}

void CViewBladeThickneessDlg::UpdateBladeTolerances()
{
	

	m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].SetMinMaxThicknesses(theApp.m_LastSettings.fBladeMinTolerance,theApp.m_LastSettings.fBladeMaxTolerance);
}


void CViewBladeThickneessDlg::SetWindowTitle()
{
	
	CString	Buff;
	int nPos;

	Buff = m_Data.m_FileName;
	if((nPos=Buff.ReverseFind('.')) > -1) {
		Buff.GetBufferSetLength(nPos);
	}

	if(m_Data.m_bModified) {
		Buff += " (Not Saved)";
	}


	SetWindowText(Buff);

}

void CViewBladeThickneessDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint pt = point;
	CRect rr;
	CString Buff,Danger;

	ScreenToClient(&point);
	m_ptClient = point;

	//Is it in the profile area
	m_staticProfile.GetWindowRect(rr);
	if(rr.PtInRect(pt)) {
		switch(m_nLButtonAction) {
		default:
			if (menu->LoadMenu(IDR_RB_CROSSSECTION_MENU)) {
				CMenu* pPopup = menu->GetSubMenu(0);

				if(theApp.m_LastSettings.bGotoViaDangerPlane==TRUE) {
					Danger.LoadString(IDS_Via_Danger_Plane);
					Buff.LoadString(IDS_Goto);
					pPopup->ModifyMenu(ID_CROSSSECTION_GOTO,MF_BYCOMMAND,ID_CROSSSECTION_GOTO,Buff + _T(" ") + Danger);
				}

				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
			};
			break;
		case 2:
			menu->LoadMenu(IDR_RB_BLADE_PROFILE_MENU);
			pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
			break;
		}

	}

	//Is it in the plan area
	m_staticPlan.GetWindowRect(rr);
	if(rr.PtInRect(pt)) {
		switch(m_nLButtonAction) {
		default:
			if (menu->LoadMenu(IDR_RB_BLADE_PLAN_MENU)) {
				CMenu* pPopup = menu->GetSubMenu(0);

				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
			}
			break;
		case 2:
			break;
		}

	}

	delete menu;
	
}

void CViewBladeThickneessDlg::OnBladeAddtrafficlight() 
{
	
	CRect rr;
	float fX;

	m_staticProfile.GetWindowRect(rr);
	rr.DeflateRect(1,1);
	ScreenToClient(rr);

	if(rr.PtInRect(m_ptClient)) {
		m_ptClient.x -= rr.left;
		m_fXCursorPos = theApp.m_LastSettings.fBladeMinX + (float)(m_ptClient.x - m_rrProfileGrat.left) * (theApp.m_LastSettings.fBladeMaxX - theApp.m_LastSettings.fBladeMinX) / (float)m_rrProfileGrat.Width();

		fX = m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_Matrix.m[3][0];

		m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].AddTrafficLight(m_fXCursorPos - fX);

		Invalidate(FALSE);
	}
	
}


void CViewBladeThickneessDlg::OnBladeDeletetrafficlight() 
{
	
	CRect rr;
	float fX;

	m_staticProfile.GetWindowRect(rr);
	rr.DeflateRect(1,1);
	ScreenToClient(rr);

	if(rr.PtInRect(m_ptClient)) {
		m_ptClient.x -= rr.left;
		m_fXCursorPos = theApp.m_LastSettings.fBladeMinX + (float)(m_ptClient.x - m_rrProfileGrat.left) * (theApp.m_LastSettings.fBladeMaxX - theApp.m_LastSettings.fBladeMinX) / (float)m_rrProfileGrat.Width();

		fX = m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_Matrix.m[3][0];

		m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].DeleteTrafficLight(m_fXCursorPos - fX);

		Invalidate(FALSE);
	}
}


void CViewBladeThickneessDlg::OnButtonTools() 
{
	if (m_pViewOptionsSheet == NULL) {
		m_pViewOptionsSheet = new CViewOptionsSheet(_T("Options"),this,0,(CWnd**)&m_pViewOptionsSheet,&m_Data,1);
		m_pViewOptionsSheet->Create( this);	
	} else {
		m_pViewOptionsSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}




void CViewBladeThickneessDlg::OnFileSave() 
{
	
	CString FilePath;
	bool	bFlag=true;
/*	
	for(int nLoop=0;nLoop<5;nLoop++) {
		if(theApp.m_Profile.m_nUseRootPath[nLoop]) {
			if((nLoop<theApp.m_Profile.m_RootPath.GetSize()) && (nLoop<theApp.m_Profile.m_SubPath.GetSize())){
				FilePath = theApp.m_Profile.m_RootPath.GetAt(nLoop) + "\\" + m_Data.m_FileName + theApp.m_Profile.m_SubPath.GetAt(nLoop) + "\\" + m_Data.m_FileName + ".dat";
				if(Save(FilePath) == false) {
					bFlag = false;
				}
			}
		}
	}
	SetWindowTitle();
	if(bFlag == true) {
		MessageBox(_T("Data has been sucesfully save to all locations"),NULL,NULL);
	}
	*/
}

void CViewBladeThickneessDlg::OnFileSaveas() 
{
	
	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = m_Data.m_FileName;

	if((nPos = m_Data.m_FileName.ReverseFind('.')) >0) {
		InitialFile = m_Data.m_FileName.Left(nPos);
	}

	CFileDialog FileDlg(FALSE,_T(".dat"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Image (*.dat)|*.dat|CIP (*.cip)|*.cip|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath[0];
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_StorePath[0] = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeReadWrite | CFile::typeBinary | CFile::modeCreate, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			Buff = FileDlg.GetFileExt();
			if(Buff == _T("cip")) {
				m_Data.SaveCIPFile(&File);
			}else{
				m_Data.Save(&File);
			}
			File.Close();
			m_Data.MESSaveOEM();
		}
	}
	SetWindowTitle();
	SetToolBarCheckedState();
	
}

bool CViewBladeThickneessDlg::Save(CString PathName)
{
	CString Buff;
	CUSLFile File;
	CFileException e;

	if(!File.Open(PathName, CFile::modeReadWrite | CFile::typeBinary | CFile::modeCreate, &e)) {
		File.DecodeFileException(&e,&Buff,PathName);
		MessageBox(Buff,_T("Error"),MB_ICONERROR);
		return false;
	} else {
		m_Data.SaveBladeXML(&File);
		File.Close();

		for(int nCad=0;nCad<4;nCad++) {
			Buff = PathName;
			switch(nCad) {
			case 0: Buff.Replace(_T(".dat"),_T(".Suction Swept"));
				break;
			case 1: Buff.Replace(_T(".dat"),_T(".Suction Parallel"));
				break;
			case 2: Buff.Replace(_T(".dat"),_T(".Pressure Swept"));
				break;
			case 3: Buff.Replace(_T(".dat"),_T(".Pressure Parallel"));
				break;
			}

			if(!File.Open(Buff, CFile::modeReadWrite | CFile::typeBinary | CFile::modeCreate, &e)) {
				File.DecodeFileException(&e,&Buff,Buff);
				MessageBox(Buff,_T("Error"),MB_ICONERROR);
			} else {
				m_Data.SaveBladeXML(&File,nCad);
				File.Close();
			}
		}
	}
	return true;
}

BOOL CViewBladeThickneessDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint	point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	if(m_rrBorder.PtInRect(point)) {
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		return TRUE;
	}
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CViewBladeThickneessDlg::PreTranslateMessage(MSG* pMsg) 
{
	
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	CRect rr = m_rrClient;
	CRect rrProfile,rrPlan;
	CString Buff;
	CDC* pDC;

	switch(pMsg->message) {
	case 0x100:
		switch(pMsg->wParam) {
		case CURSOR_DOWN:
//			m_nIndex++;
//			if(m_nIndex>=m_pToolPath->m_nCoordL) m_nIndex=m_pToolPath->m_nCoordL-1;
//			if(m_nIndex<0) m_nIndex=0;
//			if(m_bGotoIfHighlighted == true) {
//				Goto(m_nIndex);
//			}
			break;
		case CURSOR_UP:
//			m_nIndex--;
//			if(m_nIndex<0) m_nIndex=0;
//			if(m_bGotoIfHighlighted == true) {
//				Goto(m_nIndex);
//			}
			break;
		}
		break;
	case 0x200: // Mouse move
		switch(pMsg->wParam) {
		case 1:
			if((m_bLbDown == true) && (point != m_ptLbDown)) {
				theApp.m_LastSettings.m_fYBladeBorderPos = (float)point.y * 1.0f / (float)rr.Height();

				rrProfile = m_rrClient;
				rrProfile.bottom = (int)((float)m_rrClient.Height() * theApp.m_LastSettings.m_fYBladeBorderPos) - 4;
				m_staticProfile.SetWindowPos( NULL , rrProfile.left, rrProfile.top, rrProfile.Width(), rrProfile.Height(), NULL);

				rrPlan = m_rrClient;
				rrPlan.top =(int)((float)m_rrClient.Height() * theApp.m_LastSettings.m_fYBladeBorderPos) + 4;
				m_staticPlan.SetWindowPos( NULL , rrPlan.left, rrPlan.top, rrPlan.Width(), rrPlan.Height(), NULL);

				m_rrBorder = rrPlan;
				m_rrBorder.top = rrProfile.bottom;
				m_rrBorder.bottom = rrPlan.top;

//				m_staticPlan.UpdateWindow();
//				m_staticProfile.UpdateWindow();
//				Invalidate(true);
				pDC = GetDC();
				m_Data.FindLineMaxXExtent(theApp.m_Thread.m_nBladeSide,&theApp.m_LastSettings.fBladeMinX,&theApp.m_LastSettings.fBladeMaxX);
//				RenderProfile((CPaintDC*)pDC);
//				RenderPlan((CPaintDC*)pDC);
				ReleaseDC(pDC);
				Invalidate(false);

				m_ptLbDown = point;
				return TRUE;
			}
			break;
		};
		break;
	case 0x201://LBUTTONDOWN
		if(m_rrBorder.PtInRect(point)) {
			m_bLbDown = true;
			m_ptLbDown = point;
			return TRUE;
		}
		break;

	case 0x202:	//LBUTTONUP
		m_bLbDown = false;
		break;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


void CViewBladeThickneessDlg::OnCrosssectionColoursTrace() 
{
	
	CColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbCrossSection[RGB_AMP_TRACE]=dlg.GetColor();
		Invalidate(FALSE);
	}
	
}

void CViewBladeThickneessDlg::OnCrosssectionColoursBackground() 
{
	
	CColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbCrossSection[RGB_BKGROUND]=dlg.GetColor();
		Invalidate(FALSE);
	}
}

void CViewBladeThickneessDlg::OnCrosssectionColoursGraticule() 
{
	
	CColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbCrossSection[RGB_GRATICULE]=dlg.GetColor();
		Invalidate(FALSE);
	}
}

void CViewBladeThickneessDlg::OnCrosssectionColoursMaximum() 
{
	
	CColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbCrossSection[RGB_MAXIMUM]=dlg.GetColor();
		Invalidate(FALSE);
	}
	
}

void CViewBladeThickneessDlg::OnCrosssectionColoursMinimum() 
{
	
	CColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbCrossSection[RGB_MINIMUM]=dlg.GetColor();
		Invalidate(FALSE);
	}
}

void CViewBladeThickneessDlg::OnCrosssectionColoursNominal() 
{
	
	CColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbCrossSection[RGB_NOMINAL]=dlg.GetColor();
		Invalidate(FALSE);
	}
	
}

void CViewBladeThickneessDlg::OnCrosssectionColoursToftrace() 
{
	
	CColorDialog dlg;

	if(dlg.DoModal()==IDOK) {
		theApp.m_LastSettings.rgbCrossSection[RGB_TOF_TRACE]=dlg.GetColor();
		Invalidate(FALSE);
	}
	
}

void CViewBladeThickneessDlg::OnFileOpen() 
{
	
	CUSLFile File;
	CFileException e;
	CString Buff,Language;
	CString FilePath;
	int	nPos;

	CFileDialog FileDlg(TRUE,_T(".dat"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Image (*.dat)|*.dat|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath[0];
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_StorePath[0] = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeRead | CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			Language.LoadString(IDS_Error);
			MessageBox(Buff,Language,MB_ICONERROR);
		} else {
			m_Data.Retrieve(&File);
			File.Close();
			UpdateAllControls();
			Invalidate(FALSE);
		}

	}
	
}

void CViewBladeThickneessDlg::OnCrosssectionGoto() 
{
	
	CPoint point;
	CRect rrProfile;
	CCoord	Cp,CpSurface,CpHead;

	m_staticProfile.GetWindowRect(rrProfile);
	rrProfile.DeflateRect(1,1);
	ScreenToClient(rrProfile);


	point = m_ptClient;
	point.x -= rrProfile.left;
	float fXPos = theApp.m_LastSettings.fBladeMinX + (float)(point.x - m_rrProfileGrat.left) * (theApp.m_LastSettings.fBladeMaxX - theApp.m_LastSettings.fBladeMinX) / (float)m_rrProfileGrat.Width();

	m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].CalculateCoord(fXPos,CpSurface);

	PROFILE->CalculateBrainCoord(PROFILE->m_nFastAxis,PROFILE->m_nSlowAxis,CpSurface);
	theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
	theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

	theApp.m_Thread.StopCurrentThreadAction();

	theApp.m_Motors.GetHeadPos(&CpHead);


	if(theApp.m_LastSettings.bGotoViaDangerPlane == FALSE) {
		theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
		theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,PORTSIDE,TRUE,TRUE, NULL, CURRENT_FL);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,STARBOARD,TRUE,TRUE, NULL, CURRENT_FL);

		theApp.m_Thread.m_CpMoveTo = Cp;
		theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
	} else {
		theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide = 2;
		theApp.m_Thread.m_CpMoveTo = CpSurface;
		theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
	}


}

void CViewBladeThickneessDlg::OnButtonDisplayAmplitude() 
{
	
	theApp.m_LastSettings.nBladeDisplayMask & DISPLAY_AMPLITUDE ? theApp.m_LastSettings.nBladeDisplayMask &= ~DISPLAY_AMPLITUDE	: theApp.m_LastSettings.nBladeDisplayMask |= DISPLAY_AMPLITUDE;
	SetToolBarCheckedState();
	Invalidate(false);
}

void CViewBladeThickneessDlg::OnButtonDisplayTof() 
{
	
	theApp.m_LastSettings.nBladeDisplayMask & DISPLAY_ACT_THICKNESS ? theApp.m_LastSettings.nBladeDisplayMask &= ~DISPLAY_ACT_THICKNESS : theApp.m_LastSettings.nBladeDisplayMask |= DISPLAY_ACT_THICKNESS;
	SetToolBarCheckedState();
	Invalidate(false);
}


void CViewBladeThickneessDlg::OnCrosssectionAddbranept() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	
	PROFILE->AddBladeBranePt(theApp.m_nBladeLine);

	if(pFrame->m_pProfileSheet != NULL) {
		pFrame->m_pProfileSheet->UpdateAllPages();
	};

}

void CViewBladeThickneessDlg::OnFileStatistics() 
{
	

	if (m_pBladeStatisticsDlg == NULL) {
		m_pBladeStatisticsDlg = new CBladeStatisticsDlg(this,(CWnd**)&m_pBladeStatisticsDlg,&m_Data,theApp.m_Thread.m_nBladeSide);
		m_pBladeStatisticsDlg->Create(CBladeStatisticsDlg::IDD, this);
	} else {
		m_pBladeStatisticsDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CViewBladeThickneessDlg::RefreshStatisticsDlg(int nAction)
{
	

	if (m_pBladeStatisticsDlg) {

		m_pBladeStatisticsDlg->setScanType(theApp.m_Thread.m_nBladeSide);
		m_pBladeStatisticsDlg->SendMessage(UI_UPDATE_BLADE_STATISTICS);
	}
}

void CViewBladeThickneessDlg::OnButtonScanAll() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	pFrame->StartBladeScan(BLADE_SCAN, theApp.m_Thread.m_nBladeSide, SCAN_ALL);
}

void CViewBladeThickneessDlg::OnButtonScanCurrentLine() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	pFrame->StartBladeScan(BLADE_SCAN, theApp.m_Thread.m_nBladeSide, SCAN_CURRENT_LINE);
}

void CViewBladeThickneessDlg::OnButtonScanRemainingLines() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	pFrame->StartBladeScan(BLADE_SCAN, theApp.m_Thread.m_nBladeSide, SCAN_REMAINDER);
}

void CViewBladeThickneessDlg::SetProfileMinMaxThickness()
{
	
	CCoord Cp;

	if(theApp.m_LastSettings.nBladeAutoVerticalScale) {
		if(m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide]) {
			m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].GetCenterCoord(&Cp);
			theApp.m_LastSettings.fBladeMinThick = 0.0f;
			theApp.m_LastSettings.fBladeMaxThick = Cp.fNomThickness * 3.0f;
		}
	}


}

void CViewBladeThickneessDlg::OnCrosssectionNormalize() 
{
	

	if(theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) 
		theApp.m_LastSettings.nNormalizeTimeSlot[theApp.m_LastSettings.nNormSide] = CURRENT_FL;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_bNormDisableDAC = false;
	theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXtLeft;
	theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYtLeft;
	theApp.m_Thread.m_nNormAction = NORMALIZE;
	theApp.m_Thread.m_nNormSide = theApp.m_LastSettings.nNormSide;
	theApp.m_Thread.m_nThreadAction=NORMALIZE;
}

void CViewBladeThickneessDlg::OnCrosssectionNormalizedac() 
{
	

	if(theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY) 
		theApp.m_LastSettings.nNormalizeTimeSlot[theApp.m_LastSettings.nNormSide] = CURRENT_FL;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_bNormDisableDAC = TRUE;
	theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXtLeft;
	theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYtLeft;
	theApp.m_Thread.m_nNormAction = NORMALIZE;
	theApp.m_Thread.m_nNormSide = theApp.m_LastSettings.nNormSide;
	theApp.m_Thread.m_nThreadAction=NORMALIZE;
	
}

void CViewBladeThickneessDlg::OnPlanPrintcrosssection() 
{
	

	for(int nLine=0;nLine<m_Data.m_nLineBladeL[theApp.m_Thread.m_nBladeSide];nLine++) 
		m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].m_nUseToPrint = 0;

	m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_nUseToPrint = 1;

	if (m_pReportPrintDlg == NULL) {
		m_pReportPrintDlg = new CReportPrintDlg(this,(CWnd**)&m_pReportPrintDlg,NULL,&m_Data);
		m_pReportPrintDlg->Create(CReportPrintDlg::IDD,this);
		m_pReportPrintDlg->FormatBladeSectionReport();
	} else {
		m_pReportPrintDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CViewBladeThickneessDlg::OnPlanPrintallcrosssections() 
{
	

	for(int nLine=0;nLine<m_Data.m_nLineBladeL[theApp.m_Thread.m_nBladeSide];nLine++) 
		m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].m_nUseToPrint = 1;

	if (m_pReportPrintDlg == NULL) {
		m_pReportPrintDlg = new CReportPrintDlg(this,(CWnd**)&m_pReportPrintDlg,NULL,&m_Data);
		m_pReportPrintDlg->Create(CReportPrintDlg::IDD,this);
		m_pReportPrintDlg->FormatBladeSectionReport();
	} else {
		m_pReportPrintDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CViewBladeThickneessDlg::OnPlanSelecttoprint() 
{
	

	m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_nUseToPrint ^= 1;
	Invalidate(false);
}

void CViewBladeThickneessDlg::OnPlanPrintselection() 
{
	

	if (m_pReportPrintDlg == NULL) {
		m_pReportPrintDlg = new CReportPrintDlg(this,(CWnd**)&m_pReportPrintDlg,NULL,&m_Data);
		m_pReportPrintDlg->Create(CReportPrintDlg::IDD,this);
		m_pReportPrintDlg->FormatBladeSectionReport();
	} else {
		m_pReportPrintDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CViewBladeThickneessDlg::OnPlanClearallselections() 
{
	

	for(int nLine=0;nLine<m_Data.m_nLineBladeL[theApp.m_Thread.m_nBladeSide];nLine++) 
		m_Data.m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].m_nUseToPrint = 0;
	Invalidate(false);

}

void CViewBladeThickneessDlg::OnPlanPrintcolourtrafficlightimage() 
{
	

	if (m_pReportPrintDlg == NULL) {
		m_pReportPrintDlg = new CReportPrintDlg(this,(CWnd**)&m_pReportPrintDlg,NULL,&m_Data);
		m_pReportPrintDlg->Create(CReportPrintDlg::IDD,this);
		m_pReportPrintDlg->FormatBladePlanReport();
	} else {
		m_pReportPrintDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CViewBladeThickneessDlg::OnCrosssectionEraseentirebrane() 
{
	
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	
	PROFILE->DeleteAllLines(BRAINLINES);
	if(pFrame->m_pProfileSheet != NULL) {
		pFrame->m_pProfileSheet->UpdateAllPages();
	};

}
