// VolumeViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "VolumeViewDlg.h"
#include "MainFrm.h"
#include "ViewSheet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVolumeViewDlg dialog
#define	MOVE_ALL_BORDERS			1
#define	CURSOR_IN_VIEW				2
#define	CURSOR_IN_HBSCAN			3
#define	CURSOR_IN_VBSCAN			4
#define	CURSOR_IN_SCOPE				5
#define	MOVE_TIMEBASE_DELAY_HBSCAN	6
#define	MOVE_TIMEBASE_DELAY_VBSCAN	7
//nVolumeDisplayKask
#define H_BSCAN		0x0001
#define V_BSCAN		0x0002

CVolumeViewDlg::CVolumeViewDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg,CData* pData,int nVolumeNumber,int nCscanNumber)
	: CDialog(CVolumeViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVolumeViewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	srand(GetTickCount());

	CString Buff,Language;
	CFileException e;
	m_pViewSheet = pParent;
	CViewSheet* pViewSheet = (CViewSheet*)m_pViewSheet;

	m_nVolumeNumber = nVolumeNumber;
	m_nCscanNumber = nCscanNumber;
	m_pData = pData;

	if(m_pData->m_nNumberAmpImages[0]>0) {
		m_nImageType = AMP_IMAGE;
	}else{
		if(m_pData->m_nNumberTimeImages[0]>0) {
			m_nImageType = TIME_IMAGE;
		} else {
			MessageBox(L"There are no C scans so this file can't be opened",L"Error",MB_ICONERROR);
			return;
		}
	}

	
	m_pDlg = pDlg;
	m_bModifiedFlag = FALSE;
	m_nCScanPalette = 0;
	m_nBScanPalette = 0;
	m_prrSamples = &pViewSheet->m_rrSamples;
	m_bInitialize=FALSE;
	m_bPalette=TRUE;
	m_bRenderView=FALSE;
	m_nToolOption=0;
	m_rrCscan.SetRectEmpty();
	m_nZoomFactor = 1000;

	m_pVolumeOptionsSheet=NULL;
	m_pArrayView=NULL;
	m_nXpixelOdd=NULL;
	m_nXpixelEven=NULL;
	m_pArray = NULL;
	m_pTrace=NULL;
	m_pMenu=NULL;
	m_fTimeBaseScaleDelay=0.0;
	m_pFIRDlg = NULL;
	m_nTimeSlot = 0;
	m_bHistogramPalette = false;


	if(!m_File.Open(m_pData->m_FilePath, CFile::modeReadWrite | CFile::typeBinary,&e)) {
		m_File.DecodeFileException(&e,&Buff,m_pData->m_FilePath);
		Language.LoadString(IDS_Error);
		MessageBox(Buff,Language,MB_ICONERROR);
		return;
	} else {
		m_n64FileLength=m_File.GetLength();
		m_pData->RetrieveVolumeHeader(&m_File,m_nVolumeNumber,&m_nTimeSlot);
		m_pTrace = new char[m_pData->m_nRealAscanLength];

		m_nDelaySamples = 0;
		m_nWidthSamples = m_pData->m_nRealAscanLength;
		m_Gate = m_pData->m_TS[m_nTimeSlot].Gate;
	}


	m_fTimeBaseMM = (float)m_nWidthSamples * m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod * (float)theApp.m_LastSettings.nMaterialVelocity / 2e6f;

//	m_pData->ConvertVolumeFromThermometerToTwosComplement(&m_File);

	m_hcursorHMove = AfxGetApp()->LoadCursor(IDC_CURSOR_H_MOVE);
	m_hcursorVMove = AfxGetApp()->LoadCursor(IDC_CURSOR_V_MOVE);
	m_hcursorAllMove = AfxGetApp()->LoadCursor(IDC_CURSOR_ALL_MOVE);

	m_pointScreen.x = 0;
	m_pointScreen.y = 0;

	m_nMouseAction = 0;
	m_nOldWidth = -1;
	m_nOldHeight = -1;

	m_nLine=0;
	m_nSample=0;

	m_rectHBscan.right=m_rectHBscan.left=m_rectHBscan.top=m_rectHBscan.bottom=0;
	m_rectVBscan.right=m_rectVBscan.left=m_rectVBscan.top=m_rectVBscan.bottom=0;
	m_pHBscanArray = NULL;
	m_pVBscanArray = NULL;

	bModifyGatePosition=FALSE;
	m_bScopeDir=FALSE;

	m_rectZoom.SetRect(0,1000,0,1000);
	m_nVZoomScale=100;
	m_nHZoomScale=100;


	MinMax(&theApp.m_LastSettings.fVolumeHScreenRatio, 0.05f, 0.95f);
	MinMax(&theApp.m_LastSettings.fVolumeVScreenRatio, 0.05f, 0.95f);

}

CVolumeViewDlg::~CVolumeViewDlg()
{
}

void CVolumeViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVolumeViewDlg)
	DDX_Control(pDX, IDC_SCROLLBAR_VERTICAL, m_scrollVCscanBar);
	DDX_Control(pDX, IDC_SCROLLBAR_HORIZONTAL, m_scrollHCscanBar);
	DDX_Control(pDX, IDC_PROGRESS, m_progressBar);
	DDX_Control(pDX, IDC_STATIC_V_BSCAN, m_staticVBscan);
	DDX_Control(pDX, IDC_STATIC_SCOPE, m_staticScope);
	DDX_Control(pDX, IDC_STATIC_H_BSCAN, m_staticHBscan);
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_USLBARCTRL_SCOPE, m_scrollbarDelayWidth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVolumeViewDlg, CDialog)
	//{{AFX_MSG_MAP(CVolumeViewDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_UPDATE_COMMAND_UI(ID_OSCILLOSCOPE_RECTIFIER_OFF, OnUpdateOscilloscopeRectifierOff)
	ON_UPDATE_COMMAND_UI(ID_OSCILLOSCOPE_RECTIFIER_HALFWAVE, OnUpdateOscilloscopeRectifierHalfwave)
	ON_UPDATE_COMMAND_UI(ID_OSCILLOSCOPE_RECTIFIER_FULLWAVE, OnUpdateOscilloscopeRectifierFullwave)
	ON_UPDATE_COMMAND_UI(ID_OSCILLOSCOPE_GRATICULE_OFF, OnUpdateOscilloscopeGraticuleOff)
	ON_UPDATE_COMMAND_UI(ID_OSCILLOSCOPE_GRATICULE_LOG, OnUpdateOscilloscopeGraticuleLog)
	ON_UPDATE_COMMAND_UI(ID_OSCILLOSCOPE_GRATICULE_8LINE, OnUpdateOscilloscopeGraticule8line)
	ON_UPDATE_COMMAND_UI(ID_OSCILLOSCOPE_DIRECTION_VERTICAL, OnUpdateOscilloscopeDirectionVertical)
	ON_UPDATE_COMMAND_UI(ID_OSCILLOSCOPE_DIRECTION_HORIZONTAL, OnUpdateOscilloscopeDirectionHorizontal)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_OSCILLOSCOPE_DIRECTION_HORIZONTAL, OnOscilloscopeDirectionHorizontal)
	ON_COMMAND(ID_OSCILLOSCOPE_DIRECTION_VERTICAL, OnOscilloscopeDirectionVertical)
	ON_COMMAND(ID_FILE_SAVEASBMP_ASCAN, OnFileSaveasbmpAscan)
	ON_COMMAND(ID_FILE_SAVEASBMP_COMPLETEDIALOG, OnFileSaveasbmpCompletedialog)
	ON_COMMAND(ID_FILE_SAVEASBMP_HORIZONTALBSCAN, OnFileSaveasbmpHorizontalbscan)
	ON_COMMAND(ID_FILE_SAVEASBMP_VERTICALBSCAN, OnFileSaveasbmpVerticalbscan)
	ON_COMMAND(ID_FILE_SAVEASBMP_CSCAN, OnFileSaveasbmpCscan)
	ON_COMMAND(ID_ZOOM_10, OnCbscanZoom10)
	ON_COMMAND(ID_ZOOM_100, OnCbscanZoom100)
	ON_COMMAND(ID_ZOOM_25, OnCbscanZoom25)
	ON_COMMAND(ID_ZOOM_50, OnCbscanZoom50)
	ON_WM_CREATE()
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_COMMAND(ID_TOOLS_CROSSHAIR, OnToolsCrosshair)
	ON_COMMAND(ID_TOOLS_H_BSCAN, OnToolsHBscan)
	ON_COMMAND(ID_TOOLS_V_BSCAN, OnToolsVBscan)
	ON_COMMAND(ID_TOOLS_THICK_CSCAN, OnToolsThickCscan)
	ON_COMMAND(ID_TOOLS_AMP_CSCAN, OnToolsAmpCscan)
	ON_COMMAND(ID_TOOLS_RULER, OnToolsRuler)
	ON_COMMAND(ID_FILE_REPORT, OnFileReport)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_BUTTON_ZOOM_IN, OnButtonZoomIn)
	ON_COMMAND(ID_BUTTON_ZOOM_OUT, OnButtonZoomOut)
	ON_COMMAND(ID_FILE_IMAGESAVEAS, OnFileImagesaveas)
	ON_WM_HELPINFO()
	ON_COMMAND(ID_PROCESS_FIRFILTER, OnProcessFirfilter)
	ON_COMMAND(ID_FILE_SAVEVOLUMEDATAASXML, OnFileSavevolumedataasxml)
	ON_COMMAND(ID_BUTTON_FULL_SCREEN, OnButtonFullScreen)
	ON_COMMAND(ID_VOLUME_PALETTE_1, OnVolumePalette1)
	ON_COMMAND(ID_VOLUME_PALETTE_2, OnVolumePalette2)
	ON_COMMAND(ID_VOLUME_PALETTE_3, OnVolumePalette3)
	ON_COMMAND(ID_VOLUME_PALETTE_4, OnVolumePalette4)
	ON_COMMAND(ID_VOLUME_PALETTE_5, OnVolumePalette5)
	ON_COMMAND(ID_VOLUME_PALETTE_6, OnVolumePalette6)
	ON_COMMAND(ID_VOLUME_PALETTE_7, OnVolumePalette7)
	ON_COMMAND(ID_VOLUME_PALETTE_8, OnVolumePalette8)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_CBN_SELENDOK(IDC_COMBO_IMAGE, OnSelendokImage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVolumeViewDlg message handlers
int CVolumeViewDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CRect rect;

	theApp.ActivityLog(L"OnCreate");

	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	theApp.ActivityLog(L"Archive toolbar");
	if (!m_wndArchiveToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(2,2,0,0)) ||	!m_wndArchiveToolBar.LoadToolBar(IDR_VIEW_ARCHIVE_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		theApp.ActivityLog(L"Failed to create volume archive toolbar");
		return -1;      // fail to create
	}

	theApp.ActivityLog(L"Tools Toolbar");
	if (!m_wndToolsToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(2,2,0,0)) ||	!m_wndToolsToolBar.LoadToolBar(IDR_VOLUME_TOOLS_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		theApp.ActivityLog(L"Failed to create volume tools toolbar");
		return -1;      // fail to create
	}

	theApp.ActivityLog(L"Dialog bar");
	m_wndDialogBar.Create(this, IDD_VIEW_DIALOGBAR, CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY, IDD_VIEW_DIALOGBAR);
	m_wndImageDialogBar.Create(this, IDD_VOLUME_DIALOGBAR, CBRS_LEFT|CBRS_TOOLTIPS|CBRS_FLYBY, IDD_VOLUME_DIALOGBAR);


	theApp.ActivityLog(L"Rebar");
	if (!m_wndRebar.Create(this,NULL,CBRS_ALIGN_TOP | AFX_IDW_DIALOGBAR)) {
		TRACE0("Failed to create Volume View ReBar\n");
		theApp.ActivityLog(L"Failed to create volume Rebar");
		return -1;
	}

	GetClientRect(rect);

	m_wndRebar.SetWindowPos(&wndTop   , 0,0,800,30,SWP_SHOWWINDOW);

	m_wndRebar.AddBar(&m_wndArchiveToolBar);
	m_wndRebar.AddBar(&m_wndDialogBar);
	m_wndRebar.AddBar(&m_wndImageDialogBar);
	m_wndRebar.AddBar(&m_wndToolsToolBar);

	
	
	return 0;
}

BOOL CVolumeViewDlg::PreTranslateMessage(MSG* pMsg) 
{
	bool bFlag = false;
	int	nWidth,nHeight;
	
	if(pMsg->message == WM_KEYDOWN){

		CPoint pt;
		CRect rr;

		switch(pMsg->wParam){
		case 37://LeftKeyPressed
			if((theApp.m_LastSettings.nImageOrientation==2) || (theApp.m_LastSettings.nImageOrientation==3)){
				m_nSample++;
			} else {
				m_nSample--;
			}
			bFlag = true;
			break;
		case 38://UpKeyPressed
			m_nLine++;
			bFlag = true;
			break;
		case 39://RightKeyPressed
			if((theApp.m_LastSettings.nImageOrientation==2) || (theApp.m_LastSettings.nImageOrientation==3)){
				m_nSample--;
			} else {
				m_nSample++;
			}
			bFlag = true;
			break;
		case 40://DownKeyPressed
			m_nLine--;
			bFlag = true;
			break;
		case 34://PageDown
			nHeight = m_prrSamples->Height();
			m_prrSamples->top -= ((nHeight * 9) / 10);
			MinMax(&m_prrSamples->top,0,m_pData->m_nNumberLines-1);
			m_prrSamples->bottom = m_prrSamples->top + nHeight;
			m_bRenderView=TRUE;
			Invalidate(false);
			bFlag = true;
			break;
		case 33://PageUp
			nHeight = m_prrSamples->Height();
			m_prrSamples->bottom += ((nHeight * 9) / 10);
			MinMax(&m_prrSamples->bottom,0,m_pData->m_nNumberLines-1);
			m_prrSamples->top = m_prrSamples->bottom - nHeight;
			m_bRenderView=TRUE;
			Invalidate(false);
			bFlag = true;
			break;
		case 36://Home
			nWidth = m_prrSamples->Width();
			m_prrSamples->left -= ((nWidth * 9) / 10);
			MinMax(&m_prrSamples->left,0,m_pData->m_nSamplesLine-1);
			m_prrSamples->right = m_prrSamples->left + nWidth;
			m_bRenderView=TRUE;
			Invalidate(false);
			bFlag = true;
			break;
		case 35://Home
			nWidth = m_prrSamples->Width();
			m_prrSamples->right += ((nWidth * 9) / 10);
			MinMax(&m_prrSamples->right,0,m_pData->m_nSamplesLine-1);
			m_prrSamples->left = m_prrSamples->right - nWidth;
			m_bRenderView=TRUE;
			Invalidate(false);
			bFlag = true;
			break;

		default:
			return TRUE;
			break;
		}

		if(	bFlag == true ) {

			CDC* pDC = GetDC();
			MinMax(&m_nSample,0, m_pData->m_nSamplesLine-1);
			MinMax(&m_nLine,0, m_pData->m_nNumberLines-1);

			PresentRenderdView((CPaintDC*)pDC);
			DrawTrace(m_nSample,m_nLine);
			WorldToClient(&pt, m_nSample, m_nLine);
			m_pointScreen = pt;
			RenderCross(pDC, pt, 0);
			if(theApp.m_LastSettings.nVolumeDisplayMask & H_BSCAN) {
				m_pData->VolumeGenerateHorizontalBscan(&m_File,&m_progressBar,m_nVolumeNumber,m_nTimeSlot,m_nLine,&m_Gate);
				m_staticHBscan.GetWindowRect(rr);
				ScreenToClient(rr);
				DrawStaticHBscan(pDC,&rr,SCREEN);
			}

			if(theApp.m_LastSettings.nVolumeDisplayMask & V_BSCAN) {
				m_pData->VolumeGenerateVerticalBscan(&m_File,&m_progressBar,m_nVolumeNumber,m_nTimeSlot,m_nSample,&m_Gate);
				m_staticVBscan.GetWindowRect(rr);
				ScreenToClient(rr);
				DrawStaticVBscan(pDC,&rr,SCREEN);
			}
			ReleaseDC(pDC);
			return true;
		}

	}


	return CDialog::PreTranslateMessage(pMsg);

}
void CVolumeViewDlg::PostNcDestroy() 
{
	m_File.Close();
	if(m_pArray!=NULL) delete m_pArray;
	if(m_pHBscanArray!=NULL) delete m_pHBscanArray;
	if(m_pVBscanArray!=NULL) delete m_pVBscanArray;
	if(m_pTrace!=NULL) delete m_pTrace;
	if(m_nXpixelOdd!=NULL) delete m_nXpixelOdd;
	if(m_nXpixelEven!=NULL) delete m_nXpixelEven;
	if(m_pArrayView!=NULL) delete	m_pArrayView;
	
	CDialog::PostNcDestroy();
	delete this;
}

void CVolumeViewDlg::OnClose() 
{

	CDialog::OnClose();
	DestroyWindow();
}

void CVolumeViewDlg::OnDestroy() 
{

	GetWindowRect(theApp.m_LastSettings.RectWindowPos[CVolumeViewDlg::IDD % MAXIMUMWINDOWSIDNUMBER]);
	CDialog::OnDestroy();
	
	*m_pDlg = NULL;
}

BOOL CVolumeViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect rectStatic,rr;
	CString	Buff,Warning;

	CRect rect = theApp.m_LastSettings.RectWindowPos[CVolumeViewDlg::IDD % MAXIMUMWINDOWSIDNUMBER];
	theApp.ConfineToScreen(&rect);
	if (theApp.m_LastSettings.bWindowPos && !rect.IsRectEmpty()) {
		SetWindowPos(&wndTop, rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	};

	GetClientRect(rr);
	theApp.ActivityLog(L"On Adjust layout");
	AdjustLayout();

	m_progressBar.SetPos(0);

	CalculateDragBox();
	

	Buff.Format(L"SamplesLine = %d NumberLines = %d",m_pData->m_nSamplesLine,m_pData->m_nNumberLines);
	theApp.ActivityLog(Buff);

	m_rectZoom.SetRect(0,0,m_pData->m_nSamplesLine-1,m_pData->m_nNumberLines-1);
	SetScrollBarParameters();



	Buff.Format(L"Delay = %d Width = %d",m_nDelaySamples,m_nWidthSamples);
	theApp.ActivityLog(Buff);

	m_scrollbarDelayWidth.SetMax(m_nWidthSamples);
	m_scrollbarDelayWidth.SetMin(0);
	m_scrollbarDelayWidth.SetHorizontal(TRUE);
	m_scrollbarDelayWidth.SetWidthHighCursor(15);
	m_scrollbarDelayWidth.SetWidthLowCursor(15);
	m_scrollbarDelayWidth.SetMinimumScreenWidth(1);
	m_scrollbarDelayWidth.SetLowPosition(m_nDelaySamples);
	m_scrollbarDelayWidth.SetHighPosition(m_nWidthSamples);

	m_scrollHCscanBar.SetScrollRange(0,m_pData->m_nSamplesLine);
	m_scrollHCscanBar.SetScrollPos(m_rectZoom.left);
	SetScrollBarParameters();
	

	m_pointScreen = rr.TopLeft();
	ScreenToClient(&m_pointScreen);

	AddStringsImageCombo();

	InitializePalette(SCREEN);
//	SetZoomSamples();

	SetWindowTitle();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVolumeViewDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	AdjustLayout();

	if (GetSafeHwnd()) {
		CalculateDragBox();
		Invalidate(TRUE);
	}	
}

void CVolumeViewDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	rr;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(1,1);

	InitializePalette(SCREEN);
	SetZoomSamples();
	InitializeViewVariables(&dc,&rr);
	RenderView(&dc);

	PresentRenderdView(&dc);
	DrawRuler(&dc,0);
	DrawCScanScale(&dc);
	RenderCross(&dc,m_pointScreen,0);
//	DrawHistogram(&dc);
//	DrawAnnotation(&dc);

	m_bInitialize=FALSE;
	m_bPalette=FALSE;
	m_bRenderView=FALSE;

//	SetScrollBarParameters();

	DrawTrace(m_nSample,m_nLine);

	m_staticHBscan.GetWindowRect(rr);
	ScreenToClient(rr);
	DrawStaticHBscan(&dc,&rr,SCREEN);
	DrawRuler(&dc,1);

	DrawBScanScale(&dc);
	m_staticVBscan.GetWindowRect(rr);
	ScreenToClient(rr);
	DrawStaticVBscan(&dc,&rr,SCREEN);
	DrawRuler(&dc,2);


	CalculateDragBox();

	// Do not call CDialog::OnPaint() for painting messages
}

void CVolumeViewDlg::InitializeViewVariables(CPaintDC* pDC,CRect* prr)
{
	int	nXs0,nXs1,xx;

	if((prr->Width() - m_rrCscan.Width()) || (prr->Height() - m_rrCscan.Height()) || (m_bInitialize==TRUE)){
		m_rrCscan = prr;

		m_nBytesPixel = pDC->GetDeviceCaps(BITSPIXEL) / 8;

		if(m_pArrayView) delete m_pArrayView;
		m_pArrayView = new char[(m_rrCscan.Width()+2) * (m_rrCscan.Height()+2) * m_nBytesPixel];

		m_nMinViewLine=0;
		m_nMaxViewLine=m_pData->m_nNumberLines-1;
		m_bRenderView=TRUE;

		nXs1=m_prrSamples->right;
		nXs0=m_prrSamples->left;

		if(m_nXpixelOdd) delete m_nXpixelOdd;
		if(m_nXpixelEven) delete m_nXpixelEven;
		int nWidth = m_rrCscan.Width()+2;
		m_nXpixelOdd = new int[m_rrCscan.Width()+10];
		m_nXpixelEven = new int[m_rrCscan.Width()+10];
		for(xx=0;xx<=(m_rrCscan.Width()+1);xx++) {
			m_nXpixelOdd[xx]=RoundDownMulDiv(xx,(nXs1-nXs0),m_rrCscan.Width())+nXs0;
			m_nXpixelEven[xx]=RoundDownMulDiv(xx,(nXs1-nXs0),m_rrCscan.Width())+nXs0;
		};
/*
		CPolyLine lineEdge;
		CoordStruct Cp;
		for(ii=0;ii<8;ii++) {
			Cp.dX0p = (double)((ii*nWidth)/7);
			Cp.dY0p = (double)theApp.m_LastSettings.nScanEdge[ii];
			lineEdge.AddCoord(Cp);
		}
		int nEdge;
		for(xx=1;xx<nWidth;xx++) {
			lineEdge.CalculateCoord((float)xx,&Cp,X0AXIS);
			nEdge=(int)Cp.dY0p;
			m_nXpixeleven[xx] += nEdge;
			if(nWidth>m_pData->m_nSamplesLine) {
				if((m_nXpixeleven[xx]<m_nXpixeleven[xx-1])) {
					m_nXpixeleven[xx]=m_nXpixeleven[xx-1];
				}
			}
		}
*/
		m_bRenderView=TRUE;

	}
}

void CVolumeViewDlg::RenderView(CPaintDC* pDC)
{
	if(m_bRenderView != TRUE) return;

	switch(m_nImageType) {
	case AMP_IMAGE: m_pData->RenderAmpImage(pDC,m_rrCscan,m_prrSamples,m_nCscanNumber,m_pArrayView,(char*)m_cColor,0);
		break;
	case TIME_IMAGE: m_pData->RenderTimeImage(pDC,m_rrCscan,m_prrSamples,m_nCscanNumber,m_pArrayView);
		break;
	}

}

void CVolumeViewDlg::PresentRenderdView(CPaintDC* pDC)
{
	CDC  dcMem;
	CBitmap bmp;

	bmp.CreateCompatibleBitmap(pDC,m_rrCscan.Width(),m_rrCscan.Height());
	bmp.SetBitmapBits((DWORD)(m_rrCscan.Width()*m_rrCscan.Height()*m_nBytesPixel),m_pArrayView);

	dcMem.CreateCompatibleDC(pDC);

	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	pDC->BitBlt(m_rrCscan.left,m_rrCscan.top,m_rrCscan.Width(),m_rrCscan.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);


}
/*
void CVolumeViewDlg::InitializePalette(int nDevice)
{
	int	ii;
	COLORREF	rgbHist(RGB(255,0,0));

	if(m_bPalette!=TRUE) return;

	if(nDevice==SCREEN) {
		for(ii=0;ii<256;ii++) {
			m_cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>16)&0xff);
			m_cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>8)&0xff);
			m_cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>0)&0xff);
		}
	}
	if(nDevice==PRINTER) {
		for(ii=0;ii<256;ii++) {
			if(theApp.m_LastSettings.bRedBlueReverse==FALSE) {
				m_cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>16)&0xff);
				m_cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>8)&0xff);
				m_cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>0)&0xff);
			} else {
				m_cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>0)&0xff);
				m_cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>8)&0xff);
				m_cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>16)&0xff);
			}
		}
	}

	if(theApp.m_LastSettings.bHistogramPaintView[0]==TRUE) {
		int nThresh0 = m_pData->m_Hist.nSampleThreshold[0];
		int nThresh1 = m_pData->m_Hist.nPaintThreshold;
		if(nThresh0>nThresh1) {
			ii=nThresh0;
			nThresh0=nThresh1;
			nThresh1=ii;
		}
		MinMax(&nThresh0,128,255);
		MinMax(&nThresh1,128,255);
		for(ii=nThresh0;ii<nThresh1;ii++) {
			m_cColor[ii][0]=char((rgbHist>>16)&0xff);
			m_cColor[ii][1]=char((rgbHist>>8)&0xff);
			m_cColor[ii][2]=char((rgbHist>>0)&0xff);
		}
	}
}
*/
void CVolumeViewDlg::InitializePalette(int nDevice)
{
	int	ii;

	if(m_bPalette!=TRUE) return;

	if(nDevice==SCREEN) {
		for(ii=0;ii<127;ii++) {
			m_cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][256-ii]>>16)&0xff);
			m_cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][256-ii]>>8)&0xff);
			m_cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][256-ii]>>0)&0xff);
		}
		for(ii=128;ii<256;ii++) { //Fix for -ve threshold gate
			m_cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>16)&0xff);
			m_cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>8)&0xff);
			m_cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>0)&0xff);
		}
	}
	if(nDevice==PRINTER) {
		for(ii=0;ii<127;ii++) {
			if(theApp.m_LastSettings.bRedBlueReverse==FALSE) {
				m_cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][256-ii]>>16)&0xff);
				m_cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][256-ii]>>8)&0xff);
				m_cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][256-ii]>>0)&0xff);
			} else {
				m_cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][256-ii]>>0)&0xff);
				m_cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][256-ii]>>8)&0xff);
				m_cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][256-ii]>>16)&0xff);
			}
		}
		for(ii=128;ii<256;ii++) { //Fix for -ve threshold gate
			if(theApp.m_LastSettings.bRedBlueReverse==FALSE) {
				m_cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>16)&0xff);
				m_cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>8)&0xff);
				m_cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>0)&0xff);
			} else {
				m_cColor[ii][0]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>0)&0xff);
				m_cColor[ii][1]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>8)&0xff);
				m_cColor[ii][2]=char((theApp.m_Palette.m_rgbAmplitude[m_nCScanPalette][ii]>>16)&0xff);
			}
		}
	}

	int nThresh0 = m_pData->m_Hist.nSampleThreshold[0];
	int nThresh1 = m_pData->m_Hist.nSampleThreshold[1];
	if(nThresh0>nThresh1) {
		ii=nThresh0;
		nThresh0=nThresh1;
		nThresh1=ii;
	}

/*
	float fAmp;
	for(int gg=0;gg<2;gg++) {
		switch(theApp.m_Palette.m_nAmplitudeScaleType[m_nCScanPalette]) {
		case 0:
		case 1:	fAmp = m_pData->m_Hist.fLatchAmp / (float)pow(10.0,theApp.m_LastSettings.fHistogramRelativedB[gg]/-20.0f);
			break;
		case 2:	fAmp = m_pData->m_Hist.fLatchAmp + (theApp.m_LastSettings.fHistogramRelativedB[gg]*100.0f/(float)m_pData->m_nLogRange);
			break;
		}
		if(fAmp>100.0f) fAmp = 100.0f;
		if(fAmp<0.0f) fAmp = 0.0f;
		if(gg==0) {
			nThresh1= (int)((fAmp*(float)127)/100.0f) + 128;
		} else {
			nThresh0= (int)((fAmp*(float)127)/100.0f) + 128;
		}
	}
*/


	MinMax(&nThresh0,128,255);
	MinMax(&nThresh1,128,255);
	if((m_nToolOption==TOOL_HISTOGRAM) || (m_bHistogramPalette == true)) {
		if(m_pData->m_Hist.nAmpMode == HIST_ATTENUATION) {
			if(theApp.m_LastSettings.bHistogramPaintView[0]==TRUE) {
				for(ii=nThresh0;ii<nThresh1;ii++) {
					if((theApp.m_LastSettings.bRedBlueReverse==TRUE) && (nDevice == PRINTER)) {
						m_cColor[ii][0]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>0)&0xff);
						m_cColor[ii][1]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>8)&0xff);
						m_cColor[ii][2]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>16)&0xff);
					} else {
						m_cColor[ii][0]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>16)&0xff);
						m_cColor[ii][1]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>8)&0xff);
						m_cColor[ii][2]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>0)&0xff);
					}
				}
			}
			if(theApp.m_LastSettings.bHistogramPaintView[1]==TRUE) {
				for(ii=128;ii<nThresh0;ii++) {
					if((theApp.m_LastSettings.bRedBlueReverse==TRUE) && (nDevice == PRINTER)) {
						m_cColor[ii][0]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>0)&0xff);
						m_cColor[ii][1]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>8)&0xff);
						m_cColor[ii][2]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>16)&0xff);
					} else {
						m_cColor[ii][0]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>16)&0xff);
						m_cColor[ii][1]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>8)&0xff);
						m_cColor[ii][2]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>0)&0xff);
					}
				}
			}
		} else {
			if(theApp.m_LastSettings.bHistogramPaintView[0]==TRUE) {
				for(ii=nThresh0;ii<nThresh1;ii++) {
					if((theApp.m_LastSettings.bRedBlueReverse==TRUE) && (nDevice == PRINTER)) {
						m_cColor[ii][0]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>0)&0xff);
						m_cColor[ii][1]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>8)&0xff);
						m_cColor[ii][2]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>16)&0xff);
					} else {
						m_cColor[ii][0]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>16)&0xff);
						m_cColor[ii][1]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>8)&0xff);
						m_cColor[ii][2]=char((theApp.m_LastSettings.rgbHistogramColor[0]>>0)&0xff);
					}
				}
			}
			if(theApp.m_LastSettings.bHistogramPaintView[1]==TRUE) {
				for(ii=nThresh1;ii<256;ii++) {
					if((theApp.m_LastSettings.bRedBlueReverse==TRUE) && (nDevice == PRINTER)) {
						m_cColor[ii][0]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>0)&0xff);
						m_cColor[ii][1]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>8)&0xff);
						m_cColor[ii][2]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>16)&0xff);
					} else {
						m_cColor[ii][0]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>16)&0xff);
						m_cColor[ii][1]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>8)&0xff);
						m_cColor[ii][2]=char((theApp.m_LastSettings.rgbHistogramColor[1]>>0)&0xff);
					}
				}
			}
		}
	}
	m_bHistogramPalette = false;
}


void CVolumeViewDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rr;
	m_pointScreen = point;
	ClientToScreen(&point);

	if(m_rectDragBox.PtInRect(point)) {
		m_nMouseAction=MOVE_ALL_BORDERS;
		m_ptDragFrom = point;
		return;
	}
	m_staticView.GetWindowRect(rr);
	if(rr.PtInRect(point)) {
		m_nMouseAction=CURSOR_IN_VIEW;
		switch(m_nToolOption) {
		case 0:
//			ViewCross(m_pointScreen);
			DrawTrace(m_pointScreen);
			Invalidate(FALSE);
			break;
		case TOOL_RULER:
			ScreenToClient(&point);
			m_rrRuler.TopLeft()=point;
			m_rrRuler.BottomRight()=point;
			Invalidate(FALSE);
			break;
		case TOOL_HISTOGRAM:
			ScreenToClient(&point);
			m_rrRuler.TopLeft()=point;
			m_rrRuler.BottomRight()=point;
			Invalidate(FALSE);
			break;
		case TOOL_VALUE:
			break;
		}
		return;
	}
	m_staticHBscan.GetWindowRect(rr);
	if(rr.PtInRect(point)) {
		m_nMouseAction=CURSOR_IN_HBSCAN;
		switch(m_nToolOption) {
		case TOOL_RULER:
			ScreenToClient(&point);
			m_rrRuler.TopLeft()=point;
			m_rrRuler.BottomRight()=point;
			Invalidate(FALSE);
			break;
		}
		return;
	}

	m_staticHBscan.GetWindowRect(rr);
	rr.left=rr.right;
	rr.right=rr.left + 16;
	if(rr.PtInRect(point)) {
		m_nMouseAction=MOVE_TIMEBASE_DELAY_HBSCAN;
		m_ptDragFrom = point;
		return;
	}

	m_staticVBscan.GetWindowRect(rr);
	rr.top=rr.bottom;
	rr.bottom=rr.top + 16;
	if(rr.PtInRect(point)) {
		m_nMouseAction=MOVE_TIMEBASE_DELAY_VBSCAN;
		m_ptDragFrom = point;
		return;
	}


	int	nGate,nEnd;
	m_staticScope.GetWindowRect(&rr);
	if(rr.PtInRect(point)) {
		m_nMouseAction|=CURSOR_IN_SCOPE;
		if(FindNearestGate(point, rr, &nGate, &nEnd)==TRUE) {
			m_nGate=nGate;
			m_nEnd=nEnd;
			bModifyGatePosition=TRUE;
			return;
		}
	};


	CDialog::OnLButtonDown(nFlags, point);
}

void CVolumeViewDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{

	if(m_nMouseAction == MOVE_ALL_BORDERS) {
/*
		m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);
		m_StaticPosVBscan.Initialize(this, m_staticVBscan, TRUE, TRUE, TRUE, FALSE);
		m_StaticPosHBscan.Initialize(this, m_staticHBscan, FALSE, TRUE, TRUE, TRUE);
		m_StaticPosScope.Initialize(this, m_staticScope, FALSE, TRUE, TRUE, FALSE);
		m_StaticPosProgress.Initialize(this,m_progressBar,FALSE,TRUE,TRUE,TRUE);
		m_StaticPosDelayWidth.Initialize(this, m_scrollbarDelayWidth, FALSE, TRUE, TRUE, FALSE);
		m_StaticPosHCscan.Initialize(this, m_scrollHCscanBar, FALSE, TRUE, TRUE, TRUE);
		m_StaticPosVCscan.Initialize(this, m_scrollVCscanBar, TRUE, TRUE, TRUE, FALSE);
*/		
	}
	bModifyGatePosition=FALSE;	
	m_nMouseAction = 0;	
	CDialog::OnLButtonUp(nFlags, point);
}

void CVolumeViewDlg::AdjustLayout() 
{
	int dX,dY;
	CRect rrClient,rr,rrClear;
#define	MG	20

	if (m_staticView.GetSafeHwnd()) {

		GetClientRect(rrClient);

		CBrush brushBK(GetSysColor(COLOR_BTNFACE));
		CDC* pDC = GetDC();

		dX = (int)(theApp.m_LastSettings.fVolumeHScreenRatio * (float)rrClient.Width()) + rrClient.left;
		dY = (int)(theApp.m_LastSettings.fVolumeVScreenRatio * (float)rrClient.Height()) + rrClient.top;

		m_staticView.GetWindowRect(rr);
		ScreenToClient(rr);
		rr.right = dX - MG;
		rr.bottom = dY - MG;
		m_staticView.MoveWindow(rr.left, rr.top, rr.Width(), rr.Height(), TRUE);
		m_pointScreen = rr.TopLeft();

		if (m_scrollHCscanBar.GetSafeHwnd()) {
			m_scrollHCscanBar.GetWindowRect(rr);
			ScreenToClient(rr);
			rr.right = dX - MG;
			rr.top = dY - 4;
			rr.bottom = dY + 10;
			m_scrollHCscanBar.MoveWindow(rr.left, rr.top, rr.Width(), rr.Height(), TRUE);
		}
		if (m_scrollVCscanBar.GetSafeHwnd()) {
			m_scrollVCscanBar.GetWindowRect(rr);
			ScreenToClient(rr);
			rr.left = dX - 4;
			rr.right = dX + 10;
			rr.bottom = dY - MG;
			m_scrollVCscanBar.MoveWindow(rr.left, rr.top, rr.Width(), rr.Height(), TRUE);
		}

		m_staticVBscan.GetWindowRect(rr);
		ScreenToClient(rr);
		rr.left = dX + MG;
		rr.right = rrClient.right;
		rr.bottom = dY - MG;
		m_staticVBscan.MoveWindow(rr.left, rr.top, rr.Width(), rr.Height(), TRUE);

		m_staticHBscan.GetWindowRect(rr);
		ScreenToClient(rr);
		rr.right = dX - MG;
		rr.top = dY + MG;
		rr.bottom = rrClient.bottom - MG;
		m_staticHBscan.MoveWindow(rr.left, rr.top, rr.Width(), rr.Height(), TRUE);

		m_progressBar.GetWindowRect(rr);
		ScreenToClient(rr);
		rr.right = dX - MG;
		rr.bottom = rrClient.bottom;
		rr.top = rr.bottom - 18;
		m_progressBar.MoveWindow(rr.left, rr.top, rr.Width(), rr.Height(), TRUE);


		m_staticScope.GetWindowRect(rr);
		ScreenToClient(rr);
		rr.left = dX + MG;
		rr.top = dY + MG;
		rr.bottom = rrClient.bottom - 20;
		rr.right = rrClient.right;
		m_staticScope.MoveWindow(rr.left, rr.top, rr.Width(), rr.Height(), TRUE);

		m_scrollbarDelayWidth.GetWindowRect(rr);
		ScreenToClient(rr);
		rr.left = dX + MG;
		rr.right = rrClient.right;
		rr.top = rrClient.bottom - 18;
		rr.bottom = rrClient.bottom;
		m_scrollbarDelayWidth.MoveWindow(rr.left, rr.top, rr.Width(), rr.Height(), TRUE);

		//Clear the horizontal bar
		GetClientRect(rrClear);
		m_staticScope.GetWindowRect(rr);
		ScreenToClient(rr);
		rrClear.bottom = rr.top;
		m_staticView.GetWindowRect(rr);
		ScreenToClient(rr);
		rrClear.top = rr.bottom;
		pDC->FillRect(rrClear, &brushBK);

		//Clear the vertical bar
		m_staticView.GetWindowRect(rr);
		ScreenToClient(rr);
		rrClear.top = rr.top;
		rrClear.left = rr.right;
		m_staticScope.GetWindowRect(rr);
		ScreenToClient(rr);
		rrClear.bottom = rr.bottom;
		rrClear.right = rr.left;
		pDC->FillRect(rrClear, &brushBK);

		ReleaseDC(pDC);
	}


}

void CVolumeViewDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPoint pointScreen = point;
	CRect rr,rrClear;
	ClientToScreen(&point);
	float fX0,fX1,fY0,fY1,fZ0,fLength,fPos;
	CString Buff;
	int dX,dY,nn;
	CRect rrClient;
	CString Message,Length,Width,Height;
	CPoint pt,pt0,pt1;
	CCoord Cp,Cp0;

	GetClientRect(rrClient);

	if(m_nMouseAction == MOVE_ALL_BORDERS) {

		ScreenToClient(&point);
		theApp.m_LastSettings.fVolumeHScreenRatio = (float)(point.x - rrClient.left) / (float)rrClient.Width();
		theApp.m_LastSettings.fVolumeVScreenRatio = (float)(point.y - rrClient.top) / (float)rrClient.Height();

		AdjustLayout();
		Invalidate(FALSE);

		CalculateDragBox();
		return;
	}

	m_staticView.GetWindowRect(rr);
	if(rr.PtInRect(point) && (m_nMouseAction == CURSOR_IN_VIEW)) {
		switch(m_nToolOption) {
		case 0:
		{
			CDC * pDC = GetDC();
			PresentRenderdView((CPaintDC*)pDC);
			RenderCross(pDC, pointScreen, 0);
			DrawTrace(pointScreen);
			if (theApp.m_LastSettings.nVolumeDisplayMask & H_BSCAN) {
				m_pData->VolumeGenerateHorizontalBscan(&m_File, &m_progressBar, m_nVolumeNumber, m_nTimeSlot, m_nLine, &m_Gate);
				m_staticHBscan.GetWindowRect(rr);
				ScreenToClient(rr);
				DrawStaticHBscan(pDC, &rr, SCREEN);
			}

			if (theApp.m_LastSettings.nVolumeDisplayMask & V_BSCAN) {
				m_pData->VolumeGenerateVerticalBscan(&m_File, &m_progressBar, m_nVolumeNumber, m_nTimeSlot, m_nSample, &m_Gate);
				m_staticVBscan.GetWindowRect(rr);
				ScreenToClient(rr);
				DrawStaticVBscan(pDC, &rr, SCREEN);
			}
			ReleaseDC(pDC);
		}
			break;
		case TOOL_RULER:
			ScreenToClient(&point);
			m_rrRuler.BottomRight()=point;
			Invalidate(FALSE);
			if(m_pData->m_nFastAxis == 5) {
				if(m_pData->m_ProfileName == theApp.m_Profile.m_FileName) {
					ComputeRulerStats(&m_strRuler);
				} else {
					m_strRuler = "Please load correct profile";
				}
			}else{
				ClientToWorld(m_rrRuler.TopLeft(),&fX0,&fY0);
				ClientToWorld(m_rrRuler.BottomRight(),&fX1,&fY1);
				switch(theApp.m_LastSettings.nRulerTextMode) {
				default:
					m_fRulerLength=(float)sqrt((fX0-fX1)*(fX0-fX1) + (fY0-fY1)*(fY0-fY1));
					Message.LoadString(IDS_Ruler_Length);
					theApp.FormatLength(m_fRulerLength,1,Length);
					m_strRuler.Format(_T("%s:%s"),Message,Length);
					break;
				case 1:
					if(m_pData->m_ProfileName == theApp.m_Profile.m_FileName) {
						ClientToSample(m_rrRuler.BottomRight(),&pt);
						pt.y *= m_pData->m_nLineIncrement;
						pt.y+=m_pData->m_nScanStartLine;
						fPos = ((float)pt.x * m_pData->m_fFastScanSize) / (float)(m_pData->m_nSamplesLine-1) + m_pData->m_CpStart.Side0.fX;
						theApp.m_Profile.GetSurfaceCoordinate(Cp,pt,fPos,m_pData->m_nFastAxis);
						ClientToSample(m_rrRuler.TopLeft(),&pt);
						pt.y *= m_pData->m_nLineIncrement;
						pt.y+=m_pData->m_nScanStartLine;
						fPos = ((float)pt.x * m_pData->m_fFastScanSize) / (float)(m_pData->m_nSamplesLine-1) + m_pData->m_CpStart.Side0.fX;
						theApp.m_Profile.GetSurfaceCoordinate(Cp0,pt,fPos,m_pData->m_nFastAxis);
						fX0 = (Cp0.Side0.fX - Cp.Side0.fX) * (Cp0.Side0.fX - Cp.Side0.fX);
						fY0 = (Cp0.Side0.fY - Cp.Side0.fY) * (Cp0.Side0.fY - Cp.Side0.fY);
						fZ0 = (Cp0.Side0.fZ - Cp.Side0.fZ) * (Cp0.Side0.fZ - Cp.Side0.fZ);
						m_fRulerLength=(float)sqrt(fX0 + fY0 + fZ0);
						Message.LoadString(IDS_Vector_Length);
						theApp.FormatLength(m_fRulerLength,1,Length);
						m_strRuler.Format(_T("%s:%s"),Message,Length);
					} else {
						m_strRuler = "Please load correct profile";
					}
					break;
				case 2:
					if(m_pData->m_ProfileName == theApp.m_Profile.m_FileName) {
						ClientToSample(m_rrRuler.BottomRight(),&pt0);
						ClientToSample(m_rrRuler.TopLeft(),&pt1);
						m_fRulerLength=0.0f;
						for(nn=0;nn<30;nn++) {
							pt.x=RoundDownMulDiv(nn,pt1.x-pt0.x,29) + pt0.x;
							pt.y=RoundDownMulDiv(nn,pt1.y-pt0.y,29) + pt0.y;
							pt.y *= m_pData->m_nLineIncrement;
							pt.y+=m_pData->m_nScanStartLine;
							fPos = ((float)pt.x * m_pData->m_fFastScanSize) / (float)(m_pData->m_nSamplesLine-1) + m_pData->m_CpStart.Side0.fX;
							theApp.m_Profile.GetSurfaceCoordinate(Cp,pt,fPos,m_pData->m_nFastAxis);
							if(nn>0) {
								fX0 = (Cp0.Side0.fX - Cp.Side0.fX) * (Cp0.Side0.fX - Cp.Side0.fX);
								fY0 = (Cp0.Side0.fY - Cp.Side0.fY) * (Cp0.Side0.fY - Cp.Side0.fY);
								fZ0 = (Cp0.Side0.fZ - Cp.Side0.fZ) * (Cp0.Side0.fZ - Cp.Side0.fZ);
								m_fRulerLength+=(float)sqrt(fX0 + fY0 + fZ0);
							}
							Cp0 = Cp;
						}
						Message.LoadString(IDS_Surface_Length);
						theApp.FormatLength(m_fRulerLength,1,Length);
						m_strRuler.Format(_T("%s:%s"),Message,Length);
					} else {
						m_strRuler = "Please load correct profile";
					}
					break;
				case 3:
					Width.LoadString(IDS_Width);
					Height.LoadString(IDS_Height);
					m_strRuler.Format(_T("%s:%.01fmm %s:%.01fmm"),Width,fabs(fX0-fX1),Height,fabs(fY0-fY1));
					break;
				case 4:
					Message.LoadString(IDS_Diameter);
					m_strRuler.Format(_T("%s:%.01fmm"),Message,fabs(fY0-fY1));
					break;
				}
			}
			SetRulerText(m_strRuler);
			break;
		}
	}

	m_staticHBscan.GetWindowRect(rr);
	if(rr.PtInRect(point) && (m_nMouseAction == CURSOR_IN_HBSCAN)) {
		switch(m_nToolOption) {
		case TOOL_RULER:
			ScreenToClient(&point);
			m_rrRuler.BottomRight()=point;
			Invalidate(FALSE);
			HBscanToWorld(m_rrRuler.TopLeft(),&fX0,&fY0);
			HBscanToWorld(m_rrRuler.BottomRight(),&fX1,&fY1);
			switch(theApp.m_LastSettings.nRulerTextMode) {
			default:	fLength=(float)sqrt((fX0-fX1)*(fX0-fX1) + (fY0-fY1)*(fY0-fY1));
				Buff.Format(_T("Ruler Length:%.01fmm"),fLength);
				break;
			case 3:Buff.Format(_T("Width:%.01fmm Height:%.01fmm"),fabs(fX0-fX1),fabs(fY0-fY1));
				break;
			case 4:Buff.Format(_T("Diameter:%.01fmm"),fabs(fY0-fY1));
				break;
			}
			CEdit* pEdit = (CEdit* ) m_wndDialogBar.GetDlgItem(IDC_EDIT_TEXT);
			pEdit->SetWindowText(Buff);
			break;
		}
	}

	m_staticHBscan.GetWindowRect(rr);
	if(m_nMouseAction == MOVE_TIMEBASE_DELAY_HBSCAN) {
		dY = point.y - m_ptDragFrom.y;
		m_fTimeBaseScaleDelay += (float)dY * m_fTimeBaseMM / (float)rr.Height();
		CDC* pDC = GetDC();
		DrawBScanScale((CPaintDC*)pDC);
		ReleaseDC(pDC);
		m_ptDragFrom = point;
		return;
	}

	m_staticVBscan.GetWindowRect(rr);
	if(m_nMouseAction == MOVE_TIMEBASE_DELAY_VBSCAN) {
		dX = point.x - m_ptDragFrom.x;
		m_fTimeBaseScaleDelay += (float)dX * m_fTimeBaseMM / (float)rr.Width();
		CDC* pDC = GetDC();
		DrawBScanScale((CPaintDC*)pDC);
		ReleaseDC(pDC);
		m_ptDragFrom = point;
		return;
	}

	int	nStart,nFinish,nInterfacePos;

	if(bModifyGatePosition==TRUE) {
		m_staticScope.GetWindowRect(&rr);
		if(rr.PtInRect(point)) {
			point.x-=rr.left;
			point.y-=rr.top;
		
			int nDelay = m_pData->m_TS[m_nTimeSlot].Adc.nDelay + (int)((float)m_nDelaySamples  * m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);
			int nWidth = (int)((float)m_nWidthSamples  * m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);

			switch(m_Gate.nGateMode[m_nGate]) {
			case 0:
				break;
			case 1:
				nStart=RoundDownMulDiv(point.x,nWidth,rr.Width())+nDelay;
				nFinish=RoundDownMulDiv(point.x,nWidth,rr.Width())+nDelay;
				if(m_nEnd==0) m_Gate.nNsDelay[m_nGate]=nStart;
				if(m_nEnd==1) m_Gate.nNsWidth[m_nGate]=nFinish-m_Gate.nNsDelay[m_nGate];
				break;
			case 2:
				nInterfacePos=(int)((float)(m_nIntPos[0]-m_nDelaySamples)*m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);
				nStart=RoundDownMulDiv(point.x,nWidth,rr.Width())-nInterfacePos;
				nFinish=RoundDownMulDiv(point.x,nWidth,rr.Width())-nInterfacePos;
				if(m_nEnd==0) m_Gate.nNsDelay[m_nGate]=nStart;
				if(m_nEnd==1) m_Gate.nNsWidth[m_nGate]=nFinish-m_Gate.nNsDelay[m_nGate];
				break;
			default:
				nInterfacePos=(int)((float)(m_nIntPos[0]-m_nDelaySamples)*m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);
				nStart=RoundDownMulDiv(point.x,nWidth,rr.Width())-nInterfacePos;
				nFinish=RoundDownMulDiv(point.x,nWidth,rr.Width())-nInterfacePos;
				nFinish=(int)((float)m_nIntPos[m_pData->m_TS[m_nTimeSlot].Gate.nGateMode[m_nGate]-2]*m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod) - RoundDownMulDiv(point.x,m_pData->m_TS[m_nTimeSlot].Adc.nWidth,rr.Width());
				if(m_nEnd==0) {
					m_pData->m_TS[m_nTimeSlot].Gate.nNsDelay[m_nGate]=nStart;
				}
				if(m_nEnd==1) m_pData->m_TS[m_nTimeSlot].Gate.nNsFarDeadZone[m_nGate]=nFinish;
			}
			switch(theApp.m_LastSettings.nVolumeRfType) {
			case RFTYPELINEAR:
				m_Gate.nThreshold[m_nGate]=RoundDownMulDiv(rr.Height()/2-point.y,100,rr.Height()/2);
				break;
			default:
				m_Gate.nThreshold[m_nGate]=RoundDownMulDiv(rr.Height()-point.y,100,rr.Height());
				break;
			};

			if(m_pVolumeOptionsSheet) {
				m_pVolumeOptionsSheet->m_pVolumeAscanPage->m_nGate = m_nGate;
				m_pVolumeOptionsSheet->SendMessage(UI_UPDATE_ALL_PAGES);
			};
		
/*			
			switch(m_Gate.nGateMode[m_nGate]) {
			case 0:
				break;
			case 1:
				nStart=RoundDownMulDiv(point.x,m_nWidthSamples,rr.Width())+m_nDelaySamples;
				nFinish=RoundDownMulDiv(point.x,m_nWidthSamples,rr.Width())+m_nDelaySamples;
				if(m_nEnd==0) m_Gate.nNsDelay[m_nGate]=nStart;
				if(m_nEnd==1) m_Gate.nNsWidth[m_nGate]=nFinish-m_Gate.nNsDelay[m_nGate];
				break;
			case 2:
				nInterfacePos=m_pData->m_nRealAscanLength;
				for(ii=0;ii<m_pData->m_nRealAscanLength;ii++) {
					if(m_pTrace[ii]>=m_Gate.nThreshold[0]) {
						nInterfacePos=RoundDownMulDiv(ii,m_Gate.nNsTimeBaseWidth,m_pData->m_nRealAscanLength);
						ii=30000;
					};
				}
				nStart=RoundDownMulDiv(point.x,m_nWidthSamples,rr.Width())-nInterfacePos+m_nDelaySamples;
				nFinish=RoundDownMulDiv(point.x,m_nWidthSamples,rr.Width())-nInterfacePos+m_nDelaySamples;
				if(m_nEnd==0) m_Gate.nNsDelay[m_nGate]=nStart;
				if(m_nEnd==1) m_Gate.nNsWidth[m_nGate]=nFinish-m_Gate.nNsDelay[m_nGate];
				break;
			}
			switch(theApp.m_LastSettings.nVolumeRfType) {
			case RFTYPELINEAR:
				m_Gate.nThreshold[m_nGate]=RoundDownMulDiv(rr.Height()/2-point.y,127,rr.Height()/2);
				break;
			case RFTYPELOG:
				m_Gate.nThreshold[m_nGate]=RoundDownMulDiv(rr.Height()-point.y,127,rr.Height());
				break;
			};
			*/
			DrawTrace(m_nSample,m_nLine);
		};
	};	
	CDialog::OnMouseMove(nFlags, point);
}

//?????
void CVolumeViewDlg::RenderCross(CDC* pDC, CPoint point,int nAction)
{
	if(m_nToolOption != 0) return;

	CRect rr;
	m_staticView.GetWindowRect(rr);
	ScreenToClient(rr);
	CPen pen(PS_SOLID,1, theApp.m_LastSettings.rgbRulerLine);
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(point.x,rr.top);
	pDC->LineTo(point.x,rr.bottom);
	pDC->MoveTo(rr.left,point.y);
	pDC->LineTo(rr.right,point.y);

	m_pointOld = point;
	
	pDC->SelectObject(pOldPen);

}

void CVolumeViewDlg::DrawTrace(CPoint point)
{
	CRect rr;
	m_staticView.GetWindowRect(rr);
	ScreenToClient(rr);

	switch(theApp.m_LastSettings.nImageOrientation) {
	case 0:
		m_nSample = RoundDownMulDiv(point.x-rr.left,m_prrSamples->Width(),rr.Width())+m_prrSamples->left;
		m_nLine = RoundDownMulDiv(point.y-rr.top,m_prrSamples->Height(),rr.Height())+m_prrSamples->top;
		break;
	case 1:
		m_nSample = RoundDownMulDiv(point.x-rr.left,m_prrSamples->Width(),rr.Width())+m_prrSamples->left;
		m_nLine = m_prrSamples->top + (m_prrSamples->Height() - RoundDownMulDiv(point.y-rr.top,m_prrSamples->Height(),rr.Height()));
		break;
	case 2:
		m_nSample = m_prrSamples->left + (m_prrSamples->Width() - RoundDownMulDiv(point.x-rr.left,m_prrSamples->Width(),rr.Width()));
		m_nLine = RoundDownMulDiv(point.y-rr.top,m_prrSamples->Height(),rr.Height());
		break;
	case 3:
		m_nSample = m_prrSamples->left + (m_prrSamples->Width() - RoundDownMulDiv(point.x-rr.left,m_prrSamples->Width(),rr.Width()));
		m_nLine = m_prrSamples->top + (m_prrSamples->Height() - RoundDownMulDiv(point.y-rr.top,m_prrSamples->Height(),rr.Height()));
		break;
	}

	GetTrace(m_nSample,m_nLine,m_pTrace,1);
	DrawTrace(m_pTrace);
	RenderPositionDlg(m_nSample, m_nLine);
	SaveCurrentTraceAsText(m_nSample,m_nLine,m_pTrace);
}

void CVolumeViewDlg::RenderPositionDlg(int nSample, int nLine)
{
	CString Buff;

	Buff.Format(L"Sample %d Line %d",nSample, nLine);
	SetRulerText(Buff);
}


void CVolumeViewDlg::DrawTrace(int nSample,int nLine)
{
	CRect rr;
	m_staticView.GetWindowRect(rr);
	ScreenToClient(rr);

	GetTrace(m_nSample,m_nLine,m_pTrace,1);
	DrawTrace(m_pTrace);


}





void CVolumeViewDlg::GetTrace(int nSample, int nLine, char *pTrace,int nClearMode)
{
	int	nIntPos = 0;
	CFileException e;
	int AScanLength;

	if(nSample<0) nSample=0;
	if(nSample>=m_pData->m_nSamplesLine) nSample=m_pData->m_nSamplesLine-1;
	if(nLine<0) nLine=0;
	if(nLine>=m_pData->m_nNumberLines) nLine=m_pData->m_nNumberLines-1;

	switch(nClearMode) {
	case 0:	memset(pTrace,0,m_pData->m_nRealAscanLength);
		break;
	case 1:
		for(int ii=0;ii<m_pData->m_nRealAscanLength;ii++) {
			pTrace[ii]=rand()>>14;
		}
		break;
	}

	if(m_pData->m_nVolBytesPerSample == 1) {
		UINT64 nOffset = m_pData->m_n64VolumeOffset[m_nVolumeNumber] + 0x100 + ((UINT64)m_pData->m_nSamplesLine * (UINT64)m_pData->m_nAscanLength * (UINT64)nLine) + (UINT64)(nSample * m_pData->m_nAscanLength);
		if((nOffset + (UINT64)m_pData->m_nAscanLength) <= m_n64FileLength) {
			m_File.Seek(nOffset,CUSLFile::begin);
			m_File.Read(&nIntPos,4);
			if(nIntPos >=0) {
				if((nIntPos + m_pData->m_nAscanLength-4) <= m_pData->m_nRealAscanLength) {
					AScanLength = m_pData->m_nAscanLength;
				}else{
					AScanLength = m_pData->m_nRealAscanLength - nIntPos;
				}
					
				if(AScanLength>=4) {
					m_File.Read(&pTrace[nIntPos],AScanLength-4);
				}
			}
		}
	} else {
		__int16 *wArray = new __int16[8192];

		UINT64 nOffset = m_pData->m_n64VolumeOffset[m_nVolumeNumber] + 0x100;
		nOffset += ((UINT64)m_pData->m_nSamplesLine * (UINT64)m_pData->m_nAscanLength * (UINT64)nLine * 2);
		nOffset += (UINT64)(nSample * m_pData->m_nAscanLength * 2);
		if((nOffset + ((UINT64)m_pData->m_nAscanLength * 2)) <= m_n64FileLength) {
			m_File.Seek(nOffset,CUSLFile::begin);
			m_File.Read(&nIntPos,4);
			if(nIntPos >=0) {
				if((nIntPos + m_pData->m_nAscanLength-4) <= m_pData->m_nRealAscanLength) {
					AScanLength = m_pData->m_nAscanLength;
				}else{
					AScanLength = m_pData->m_nRealAscanLength - nIntPos;
				}
					
				if(AScanLength>=4) {
					m_File.Read(&wArray[nIntPos],(AScanLength * 2) -4);
					for(int ii=0;ii<AScanLength - 4;ii++) {
						pTrace[nIntPos + ii] = (unsigned char)(wArray[nIntPos + ii] >> 8);
					}
				}
			}
		}

		delete wArray;
	}
}



void CVolumeViewDlg::WriteTrace(int nSample, int nLine, char *pTrace)
{

	UINT64	nOffset = (m_pData->m_n64VolumeOffset[m_nVolumeNumber] + 0x100 + (UINT64)m_pData->m_nSamplesLine * (UINT64)m_pData->m_nAscanLength * (UINT64)nLine) + (UINT64)(nSample * m_pData->m_nAscanLength);
	m_File.Seek(nOffset,CUSLFile::begin);
	m_File.Write(pTrace,m_pData->m_nAscanLength);
}




void CVolumeViewDlg::DrawTrace(char *pTrace)
{
	if(GetSafeHwnd() == NULL) return;

	CDC* pDC = GetDC();
	CString Buff,Language;
	CString BuffDelay,BuffWidth;
	CRect	rr,rrAscan;
	CPen* pOldPen;
	int	jj;
	CDC  dcMem;
	CBitmap bmp;

	LOGFONT lf;
	memset((void*)&lf,0,sizeof(lf));
	lf.lfHeight = theApp.m_Scope.m_nTextSize;
	Language.LoadString(IDS_MS_Sans_Serif);
	wcscpy_s(lf.lfFaceName, wcslen(Language) + 1, Language);
	CFont Font;
	Font.CreateFontIndirect(&lf);


	m_staticScope.GetWindowRect(&rr);
	ScreenToClient(&rr);
	
	if((m_nOldWidth-rr.Width()) || m_nOldHeight-rr.Height()) {
		if(m_pArray!=NULL) delete m_pArray;
		m_pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
		m_nOldWidth=rr.Width();
		m_nOldHeight=rr.Height();
	}
	memset(m_pArray,0,rr.Width()*sizeof(COLORREF)*rr.Height());

	bmp.CreateCompatibleBitmap(pDC,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),m_pArray);
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	ASSERT(pOldBitmap);
	CFont* pOldFont = dcMem.SelectObject(&Font);

	rrAscan.SetRectEmpty();
	rrAscan.right=rr.Width();
	rrAscan.bottom=rr.Height();
	DrawGraticule(&dcMem,rrAscan);

	//Draw Trace
	CPen OnPen(PS_SOLID,1,RGB(255,255,255));
	pOldPen = dcMem.SelectObject(&OnPen);

	int	m_nMaxSample = 127;
	int xx,nn,nS0,nS1,nMin,nMax,nPtr,vv;
	CPoint m_pPtTrace[4000];
	switch(theApp.m_LastSettings.nVolumeRfType) {
	case RFTYPELINEAR:
		if(rr.Width()<m_nWidthSamples) {
			for(xx=0,nn=0;xx<(rr.Width()-1);xx++) {
				nS0=RoundDownMulDiv(xx,m_nWidthSamples,rr.Width())+m_nDelaySamples;
				nS1=RoundDownMulDiv(xx+1,m_nWidthSamples,rr.Width())+m_nDelaySamples;
				nMin=10000;
				nMax=0;
				for(nPtr=nS0;nPtr<nS1;nPtr++) {
					vv=RoundDownMulDiv(pTrace[nPtr],rr.Height()/2,-m_nMaxSample)+rr.Height()/2;
					MinMax(&vv,0,rr.Height()-1);
					if(vv<nMin) nMin=vv;
					if(vv>nMax) nMax=vv;
				}
				m_pPtTrace[nn].x = xx;
				m_pPtTrace[nn++].y = nMin;
				m_pPtTrace[nn].x = xx;
				m_pPtTrace[nn++].y = nMax;
			}
			dcMem.Polyline(m_pPtTrace,nn);
		} else {
			for(xx=0,nn=0;xx<rr.Width();xx++) {
				nPtr=RoundDownMulDiv(xx,m_nWidthSamples,rr.Width())+m_nDelaySamples;
				vv=RoundDownMulDiv(pTrace[nPtr],rr.Height()/2,-m_nMaxSample)+rr.Height()/2;
				MinMax(&vv,0,rr.Height()-1);
				m_pPtTrace[nn].x = xx;
				m_pPtTrace[nn++].y = vv;
			}
			dcMem.Polyline(m_pPtTrace,nn);
		}
		break;
	default:
		if(rr.Width()<m_nWidthSamples) {
			for(xx=0,nn=0;xx<(rr.Width()-1);xx++) {
				nS0=RoundDownMulDiv(xx,m_nWidthSamples,rr.Width())+m_nDelaySamples;
				nS1=RoundDownMulDiv(xx+1,m_nWidthSamples,rr.Width())+m_nDelaySamples;
				nMin=10000;
				nMax=0;
				for(nPtr=nS0;nPtr<nS1;nPtr++) {
					switch(theApp.m_LastSettings.nVolumeRfType) {
					case 1:	vv=RoundDownMulDiv(pTrace[nPtr],rr.Height(),-m_nMaxSample)+rr.Height();
						break;
					case 2:	vv=RoundDownMulDiv(~pTrace[nPtr],rr.Height(),-m_nMaxSample)+rr.Height();
						break;
					case 3:	vv=RoundDownMulDiv(abs(pTrace[nPtr]),rr.Height(),-m_nMaxSample)+rr.Height();
						break;
					}
					MinMax(&vv,0,rr.Height()-1);
					if(vv<nMin) nMin=vv;
					if(vv>nMax) nMax=vv;
				}
				m_pPtTrace[nn].x = xx;
				m_pPtTrace[nn++].y = nMin;
			}
			dcMem.Polyline(m_pPtTrace,nn);
		} else {
			for(xx=0,nn=0;xx<rr.Width();xx++) {
				nPtr=RoundDownMulDiv(xx,m_nWidthSamples,rr.Width())+m_nDelaySamples;
				switch(theApp.m_LastSettings.nVolumeRfType) {
				case 1:	vv=RoundDownMulDiv(pTrace[nPtr],rr.Height(),-m_nMaxSample)+rr.Height();
					break;
				case 2:	vv=RoundDownMulDiv(~pTrace[nPtr],rr.Height(),-m_nMaxSample)+rr.Height();
					break;
				case 3:	vv=RoundDownMulDiv(abs(pTrace[nPtr]),rr.Height(),-m_nMaxSample)+rr.Height();
					break;
				}
				MinMax(&vv,0,rr.Height()-1);
				m_pPtTrace[nn].x = xx;
				m_pPtTrace[nn++].y = vv;
			}
			dcMem.Polyline(m_pPtTrace,nn);
		}
		break;
	}


	DrawGates(&dcMem,rrAscan);
	DrawText(&dcMem,rrAscan);
	
	pDC->BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);

	m_staticScope.GetWindowRect(&rr);
	ScreenToClient(&rr);
	CBrush brushBorder(RGB(180,180,180));
	for(jj=0;jj<3;jj++) {
		rr.InflateRect(1,1);
		pDC->FrameRect(&rr,&brushBorder);
	}
	
	
	dcMem.SelectObject(pOldBitmap);
	dcMem.SelectObject(pOldPen);
	dcMem.SelectObject(pOldFont);


	ReleaseDC(pDC);

}


void CVolumeViewDlg::OnUpdateOscilloscopeRectifierOff(CCmdUI* pCmdUI) 
{
	theApp.m_LastSettings.nVolumeRfType==0 ?	pCmdUI->SetCheck( 1) : pCmdUI->SetCheck( 0);
}

void CVolumeViewDlg::OnUpdateOscilloscopeRectifierHalfwave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CVolumeViewDlg::OnUpdateOscilloscopeRectifierFullwave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CVolumeViewDlg::OnUpdateOscilloscopeGraticuleOff(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CVolumeViewDlg::OnUpdateOscilloscopeGraticuleLog(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CVolumeViewDlg::OnUpdateOscilloscopeGraticule8line(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CVolumeViewDlg::OnUpdateOscilloscopeDirectionVertical(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CVolumeViewDlg::OnUpdateOscilloscopeDirectionHorizontal(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}



void CVolumeViewDlg::DrawStaticView(CDC *pDC, CRect *prr, int nAction)
{

	CBrush brushWhite(RGB(255,255,255));
	pDC->FillRect(prr,&brushWhite);
}

void CVolumeViewDlg::DrawStaticVBscan(CDC *pDC, CRect *prr, int nAction)
{
	if(GetSafeHwnd() == NULL) return;

	CString Buff;
	CString BuffDelay,BuffWidth;
	CRect	rr;
	CDC  dcMem;
	CBitmap bmp;
	CFont font;
	int	nColor,nOffset,nLine;
	char	*pBuffer;
	int xx, xxCount;
	
	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nBytesPixel = nBitsPixel/8;
	if(nBytesPixel<0) nBytesPixel=1;

	if((m_rectVBscan.Width()-prr->Width()) || m_rectVBscan.Height()-prr->Height()) {
		if(m_pVBscanArray!=NULL) delete m_pVBscanArray;
		m_pVBscanArray = new char[(prr->Width()+2) * (prr->Height()+2) * nBytesPixel];
		m_rectVBscan = *prr;
	}
	char *pTrace = new char[m_pData->m_nRealAscanLength];
	int nDelaySamples;
	(theApp.m_LastSettings.nLockSurface==1) ? nDelaySamples = 0 : nDelaySamples = m_nDelaySamples;
//	memset(m_pHBscanArray,0,prr->Width()*sizeof(COLORREF)*prr->Height());
//	m_progressBar.SetRange(0,prr->Height());
	for(int yy=0;yy<prr->Height();yy++) {
//		m_progressBar.SetPos(yy);
		if( (theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2) ) {
			nLine = RoundDownMulDiv(yy,m_prrSamples->Height(),prr->Height()) + m_prrSamples->top;
		} else {
			nLine = m_prrSamples->bottom - 1 - (RoundDownMulDiv(yy,m_prrSamples->Height(),prr->Height()));
		}
		m_pData->GetBscanTrace(1,m_nSample,nLine,pTrace);
		for(xxCount=0;xxCount<prr->Width();xxCount++) {
			if((theApp.m_LastSettings.nImageOrientation==2) || (theApp.m_LastSettings.nImageOrientation==3)){
				xx=(prr->Width()-xxCount);
			}else{
				xx=xxCount;
			}	
			int nPtr = RoundDownMulDiv(xx,m_nWidthSamples,prr->Width()) + nDelaySamples;
			nOffset=(xx + (yy * prr->Width())) * nBytesPixel;
			pBuffer=&m_pVBscanArray[nOffset];
			nColor = pTrace[nPtr];

			switch(theApp.m_LastSettings.nVolumeRfType) {
			case RFTYPELINEAR:
			case 1:
			case 4:
				break;
			case 2:
				nColor=~nColor;
				break;
			case 3:
				nColor=abs(nColor);
				break;
			}
			nColor>=0 ?	nColor+=128 : nColor=128;
			if(nAction==SCREEN) {
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nBScanPalette][nColor]>>16)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nBScanPalette][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nBScanPalette][nColor]>>0)&0xff);
			} else {
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nBScanPalette][nColor]>>0)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nBScanPalette][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nBScanPalette][nColor]>>16)&0xff);
			}

			if(nBytesPixel>3) pBuffer++;
		}
	}
	delete pTrace;

	bmp.CreateCompatibleBitmap(pDC,prr->Width(),prr->Height());
	bmp.SetBitmapBits((DWORD)(prr->Width()*prr->Height()*nBytesPixel),m_pVBscanArray);
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	
	pDC->BitBlt(prr->left,prr->top,prr->Width(),prr->Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);
	
//	m_progressBar.SetPos(0);


}

void CVolumeViewDlg::DrawStaticHBscan(CDC *pDC, CRect *prr, int nAction)
{
	if(GetSafeHwnd() == NULL) return;

	CString Buff;
	CString BuffDelay,BuffWidth;
	CRect	rr;
	CDC  dcMem;
	CBitmap bmp;
	CFont font;
	int	nColor,nOffset;
	char	*pBuffer;
	int xx, xxCount;
	
	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nBytesPixel = nBitsPixel/8;
	if(nBytesPixel<0) nBytesPixel=1;

	if((m_rectHBscan.Width()-prr->Width()) || m_rectHBscan.Height()-prr->Height()) {
		if(m_pHBscanArray!=NULL) delete m_pHBscanArray;
		m_pHBscanArray = new char[(prr->Width()+2) * (prr->Height()+2) * nBytesPixel];
		m_rectHBscan = *prr;
	}
	char *pTrace = new char[m_pData->m_nRealAscanLength];
	int nDelaySamples;
	(theApp.m_LastSettings.nLockSurface==1) ? nDelaySamples = 0 : nDelaySamples = m_nDelaySamples;
	for(xxCount=0;xxCount<prr->Width();xxCount++) {
		if((theApp.m_LastSettings.nImageOrientation==2) || (theApp.m_LastSettings.nImageOrientation==3)){
			xx=(prr->Width()-xxCount);
		}else{
			xx=xxCount;
		}
		int nSample = RoundDownMulDiv(xxCount,m_prrSamples->Width(),prr->Width()) + m_prrSamples->left;
		m_pData->GetBscanTrace(0,nSample,m_nLine,pTrace);
		for(int yy=0;yy<prr->Height();yy++) {
			int nPtr = RoundDownMulDiv(yy,m_nWidthSamples,prr->Height()) + nDelaySamples;
			nOffset=(xx + (yy * prr->Width())) * nBytesPixel;
			pBuffer=&m_pHBscanArray[nOffset];
			nColor = pTrace[nPtr];

			switch(theApp.m_LastSettings.nVolumeRfType) {
			case RFTYPELINEAR:
			case 1:
			case 4:
				break;
			case 2:
				nColor=~nColor;
				break;
			case 3:
				nColor=abs(nColor);
				break;
			}
			nColor>=0 ?	nColor+=128 : nColor=128;
			if(nAction==SCREEN) {
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nBScanPalette][nColor]>>16)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nBScanPalette][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nBScanPalette][nColor]>>0)&0xff);
			} else {
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nBScanPalette][nColor]>>0)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nBScanPalette][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[m_nBScanPalette][nColor]>>16)&0xff);
			}

			if(nBytesPixel>3) pBuffer++;
		}
	}
	delete pTrace;

	bmp.CreateCompatibleBitmap(pDC,prr->Width(),prr->Height());
	bmp.SetBitmapBits((DWORD)(prr->Width()*prr->Height()*nBytesPixel),m_pHBscanArray);
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
/*
	//Draw Trace
	CPen OnPen(PS_SOLID,1,RGB(255,255,255));
	pOldPen = dcMem.SelectObject(&OnPen);

	dcMem.MoveTo(0,nGArray[0]);
	for(jj=0;jj<prr->Width();jj++) {
		dcMem.LineTo(jj,nGArray[jj]);
	};
*/
	
	pDC->BitBlt(prr->left,prr->top,prr->Width(),prr->Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);
	

}

/*
void CVolumeViewDlg::DrawStaticHBscan(CDC *pDC, CRect *prr, int nAction)
{
	if(GetSafeHwnd() == NULL) return;

	CString Buff;
	CString BuffDelay,BuffWidth;
	CRect	rr;
	CDC  dcMem;
	CBitmap bmp;
	CFont font;
	int	nColor,nOffset;
	char	*pBuffer;
	
	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nBytesPixel = nBitsPixel/8;
	if(nBytesPixel<0) nBytesPixel=1;

	if((m_rectHBscan.Width()-prr->Width()) || m_rectHBscan.Height()-prr->Height()) {
		if(m_pHBscanArray!=NULL) delete m_pHBscanArray;
		m_pHBscanArray = new char[(prr->Width()+2) * (prr->Height()+2) * nBytesPixel];
		m_rectHBscan = *prr;
	}
	char *pTrace = new char[m_pData->m_nRealAscanLength];
	for(int xx=0;xx<prr->Width();xx++) {
		int nSample = RoundDownMulDiv(xx,m_pData->m_nSamplesLine,prr->Width());
		GetTrace(nSample,m_nLine,pTrace);
		for(int yy=0;yy<prr->Height();yy++) {
			int nPtr = RoundDownMulDiv(yy,m_nWidthSamples,prr->Height()) + m_nDelaySamples;
			nOffset=(xx + (yy * prr->Width())) * nBytesPixel;
			pBuffer=&m_pHBscanArray[nOffset];
			nColor = pTrace[nPtr];

			if(nColor>=0) {
				nColor+=128;
			} else {
				nColor=abs(nColor)+128;
			}
			if(nAction==SCREEN) {
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[0][nColor]>>16)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[0][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[0][nColor]>>0)&0xff);
			} else {
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[0][nColor]>>0)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[0][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[0][nColor]>>16)&0xff);
			}

			if(nBytesPixel>3) pBuffer++;
		}
	}
	delete pTrace;

	bmp.CreateCompatibleBitmap(pDC,prr->Width(),prr->Height());
	bmp.SetBitmapBits((DWORD)(prr->Width()*prr->Height()*nBytesPixel),m_pHBscanArray);
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	pDC->BitBlt(prr->left,prr->top,prr->Width(),prr->Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);
}
*/

void CVolumeViewDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{

	CRect rr;
	m_staticView.GetWindowRect(rr);

	m_bInCScan = false;
	m_bInBScan = false;

	if(rr.PtInRect(point)) {
		CMenu* menu = (CMenu *) new CMenu;
		if (menu->LoadMenu(IDR_RB_VOLUME_MENU)) {
			CMenu* pPopup = menu->GetSubMenu(0);
			ASSERT(pPopup != NULL);
			m_bInCScan = true;
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);

		};
		delete menu;
	}
	CRect hh,vv;
	m_staticHBscan.GetWindowRect(hh);
	m_staticVBscan.GetWindowRect(vv);
	if(hh.PtInRect(point) || vv.PtInRect(point)) {
		CMenu* menu = (CMenu *) new CMenu;
		if (menu->LoadMenu(IDR_RB_VOLUME_MENU)) {
			CMenu* pPopup = menu->GetSubMenu(0);
			ASSERT(pPopup != NULL);
			m_bInBScan = true;
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);

		};
		delete menu;
	}


}

void CVolumeViewDlg::DrawGates(CDC *pDC,CRect rr)
{

	if(m_bScopeDir==FALSE) {
		DrawHorizontalGates(pDC,rr);
	} else {
		DrawVerticalGates(pDC,rr);
	}


}


void CVolumeViewDlg::DrawHorizontalGates(CDC *pDC,CRect rr)
{
	int	nStart,nFinish,nPny,gg,nPnt,ii,nX[2],nT,nThreshold;
	CPen* pPen;
	CPen *pOldPen;
	char cMax[8];
	ZeroMemory(cMax,sizeof cMax);

	for(ii=0;ii<8;ii++)
		m_nIntPos[ii]=m_pData->m_nRealAscanLength;

	float fDelay = (float)m_pData->m_TS[m_nTimeSlot].Adc.nDelay;// - m_nDelaySamples  * m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod;

	for(gg=0;gg<8;gg++) {
		switch(m_Gate.nGateMode[gg]) {
		case 0:
			break;
		case 1:
			nStart = (int)(((float)m_Gate.nNsDelay[gg] - fDelay) / m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);
			nFinish = nStart + (int)((float)m_Gate.nNsWidth[gg]/m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);
			break;
		case 2:
			nStart = (int)((float)m_Gate.nNsDelay[gg] / m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod) + m_nIntPos[0];
			nFinish = nStart + (int)((float)m_Gate.nNsWidth[gg]/m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);
			break;
		default:
			nStart = (int)((float)m_Gate.nNsDelay[gg] / m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod) + m_nIntPos[0];
			nFinish = m_nIntPos[m_Gate.nGateMode[gg]-2] - (int)((float)m_Gate.nNsFarDeadZone[gg]/m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);
			break;
		};
		if(nFinish>=m_pData->m_nRealAscanLength) nFinish=m_pData->m_nRealAscanLength-1;
		nX[0]=RoundDownMulDiv(nStart-m_nDelaySamples,rr.Width(),m_nWidthSamples);
		nX[1]=RoundDownMulDiv(nFinish-m_nDelaySamples,rr.Width(),m_nWidthSamples);
		m_Gate.nAmplitude[gg] = 0;
		nThreshold = RoundDownMulDiv(m_Gate.nThreshold[gg],127,100);
		//Find amplitude
		for(ii=nStart;ii<=nFinish;ii++) {
			if(nThreshold>=0) {
				switch(theApp.m_LastSettings.nVolumeRfType) {
				case RFTYPELINEAR:
				case 1:
				case 4: if(m_pTrace[ii]>m_Gate.nAmplitude[gg]) m_Gate.nAmplitude[gg] = m_pTrace[ii];
					break;
				case 2: if((~m_pTrace[ii])>m_Gate.nAmplitude[gg]) m_Gate.nAmplitude[gg] = ~m_pTrace[ii];
					break;
				case 3: if(abs(m_pTrace[ii])>m_Gate.nAmplitude[gg]) m_Gate.nAmplitude[gg] = abs(m_pTrace[ii]);
					break;
				}
			} else {
				switch(theApp.m_LastSettings.nVolumeRfType) {
				case RFTYPELINEAR:
				case 1:
				case 4: if(m_pTrace[ii]<m_Gate.nAmplitude[gg]) m_Gate.nAmplitude[gg] = m_pTrace[ii];
					break;
				case 2: if((~m_pTrace[ii])<m_Gate.nAmplitude[gg]) m_Gate.nAmplitude[gg] = ~m_pTrace[ii];
					break;
				case 3: if(abs(m_pTrace[ii])<m_Gate.nAmplitude[gg]) m_Gate.nAmplitude[gg] = abs(m_pTrace[ii]);
					break;
				}
			}
		}
		//Find Time Of Flight
		switch(m_Gate.nTimsDetectionMode[gg]) {
		case 1:
		case 2:
			for(ii=nStart;ii<=nFinish;ii++) {
				switch(m_Gate.nTimsDetectionMode[gg]) {
				case 1:	
					if(nThreshold>=0) {
						if((m_nIntPos[gg]==m_pData->m_nRealAscanLength) && (m_pTrace[ii]>=nThreshold)) m_nIntPos[gg] = ii;
					} else {
						if((m_nIntPos[gg]==m_pData->m_nRealAscanLength) && (m_pTrace[ii]<=nThreshold)) m_nIntPos[gg] = ii;
					}
					break;
				case 2:	
					if(nThreshold>=0) {
						if(m_pTrace[ii]>=cMax[gg]) {
							m_nIntPos[gg] = ii;
							cMax[gg] = m_pTrace[ii];
						}
					} else {
						if(m_pTrace[ii]<=cMax[gg]) {
							m_nIntPos[gg] = ii;
							cMax[gg] = m_pTrace[ii];
						}
					}
					break;
				}
			}
			break;
		case 3:
		case 4:
			for(ii=nFinish;ii>=nStart;ii--) {
				switch(m_Gate.nTimsDetectionMode[gg]) {
				case 3:	if((m_nIntPos[gg]==m_pData->m_nRealAscanLength) && (m_pTrace[ii]>=nThreshold)) m_nIntPos[gg] = ii;
					break;
				case 4:
					if(m_pTrace[ii]>=cMax[gg]) {
						m_nIntPos[gg] = ii;
						cMax[gg] = m_pTrace[ii];
					}
					break;
				}
			}
			break;
		}
		nT = RoundDownMulDiv(m_nIntPos[gg]-m_nDelaySamples,rr.Width(),m_nWidthSamples);

		switch(theApp.m_LastSettings.nVolumeRfType) {
		case RFTYPELINEAR:
			nPny=rr.Height()/2-RoundDownMulDiv(m_Gate.nAmplitude[gg],rr.Height()/2,127);
			nPnt=rr.Height()/2-RoundDownMulDiv(m_Gate.nThreshold[gg],rr.Height()/2,100);
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			nPny=rr.Height()-RoundDownMulDiv(m_Gate.nAmplitude[gg],rr.Height(),127);
			nPnt=rr.Height()-RoundDownMulDiv(m_Gate.nThreshold[gg],rr.Height(),100);
			break;
		}
		if((nX[0]<rr.Width()&&(nX[1]>=0))&&m_Gate.nGateMode[gg]) {
			pPen = new CPen;
			pPen->CreatePen(PS_SOLID,0,theApp.m_Scope.m_rgbGateColor[gg]);
			pOldPen=pDC->SelectObject(pPen);
			if(nX[1]>rr.Width()) nX[1]=rr.Width();
			pDC->MoveTo(nX[0],nPny);
			pDC->LineTo(nX[1],nPny);
			pDC->MoveTo(nX[0],nPnt);
			pDC->LineTo(nX[1],nPnt);

			pDC->SelectObject(pOldPen);
			delete pPen;
		};
		if((nT>0) && (nT<rr.Width()) && (m_Gate.nTimsDetectionMode[gg]>0)) {
			pPen = new CPen;
			pPen->CreatePen(PS_SOLID,0,theApp.m_Scope.m_rgbGateColor[gg]);
			pOldPen=pDC->SelectObject(pPen);
			pDC->MoveTo(nT,0);
			pDC->LineTo(nT,rr.Height());

			pDC->SelectObject(pOldPen);
			delete pPen;

		}
		m_Gate.nTimeSample[gg] = m_nIntPos[gg];
	};

	//Draw library
	pPen = new CPen;
	pPen->CreatePen(PS_SOLID, 0, RGB(200,0,200));
	pOldPen = pDC->SelectObject(pPen);
	int nVolGate = theApp.m_LastSettings.nVolumeCScanGate;
	for (int ii = 0; ii < theApp.m_LastSettings.nVolGatesL; ii++) {
		int nNsDelay = (int)((theApp.m_LastSettings.fVolGateIntervalNs*ii) + theApp.m_LastSettings.fVolStartDepthNs);
		int nNsWidth = (int)theApp.m_LastSettings.fVolGateWidthNs;

		switch (m_Gate.nGateMode[nVolGate]) {
		case 0:
			break;
		case 1:
			nStart = (int)(((float)nNsDelay - fDelay) / m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);
			nFinish = nStart + (int)((float)nNsWidth / m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);
			break;
		case 2:
			nStart = (int)((float)nNsDelay / m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod) + m_nIntPos[0];
			nFinish = nStart + (int)((float)nNsWidth / m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);
			break;
		default:
			nStart = (int)((float)m_Gate.nNsDelay[gg] / m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod) + m_nIntPos[0];
			nFinish = m_nIntPos[m_Gate.nGateMode[gg] - 2] - (int)((float)m_Gate.nNsFarDeadZone[gg] / m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);
			break;
		};
		if (nFinish >= m_pData->m_nRealAscanLength) nFinish = m_pData->m_nRealAscanLength - 1;
		nX[0] = RoundDownMulDiv(nStart - m_nDelaySamples, rr.Width(), m_nWidthSamples);
		nX[1] = RoundDownMulDiv(nFinish - m_nDelaySamples, rr.Width(), m_nWidthSamples);

		pDC->MoveTo(nX[0], 20 + ((ii % 2) * 4));
		pDC->LineTo(nX[1], 20 + ((ii % 2) * 4));

	}
	pDC->SelectObject(pOldPen);
	delete pPen;

}

void CVolumeViewDlg::DrawVerticalGates(CDC *pDC,CRect rr)
{

}


/*
void CVolumeViewDlg::OnProcessCscan() 
{
	CRect rr;
	CDC* pDC = GetDC();

	m_staticView.GetWindowRect(rr);
	ScreenToClient(rr);
	DrawStaticCscan(pDC,&rr,SCREEN);
	ViewCross(m_pointScreen,1);

	ReleaseDC(pDC);
	
}
*/
void CVolumeViewDlg::DrawStaticCscan(CDC *pDC, CRect *prr, int nAction)
{
	if(GetSafeHwnd() == NULL) return;

	m_nCscanNumber = m_pData->VolumeGenerateAmpCscan(&m_File,&m_progressBar,&m_Gate,m_nVolumeNumber,m_nTimeSlot,m_nEncoding,L""); 
	m_bRenderView=TRUE;
	Invalidate(FALSE);

}


/*
void CVolumeViewDlg::DrawStaticCscan(CDC *pDC, CRect *prr, int nAction)
{
	if(GetSafeHwnd() == NULL) return;

	CString Buff;
	CString BuffDelay,BuffWidth;
	CRect	rr;
	int	ii,gg=1;
	CDC  dcMem;
	CBitmap bmp;
	CFont font;
	int	nColor,nOffset,nStart[2],nFinish[2],nInterfacePos;
	char	*pBuffer,*pArray;
	

	int nBitsPixel = pDC->GetDeviceCaps(BITSPIXEL);
	int nBytesPixel = nBitsPixel/8;
	if(nBytesPixel<0) nBytesPixel=1;

	pArray = new char[(prr->Width()+2) * (prr->Height()+2) * nBytesPixel];

	nStart[0] = (int)(((float)m_Gate.nNsDelay[0] - (float)m_pData->m_TS[m_nTimeSlot].Adc.nDelay) / m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);
	nFinish[0] = nStart[0] + m_Gate.nNsWidth[0]/m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod;

	char *pTrace = new char[m_pData->m_nRealAscanLength];
	m_progressBar.SetRange(0,prr->Height());
	for(int yy=0;yy<prr->Height();yy++) {
		m_progressBar.SetPos(yy);
		for(int xx=0;xx<prr->Width();xx++) {
			int nSample = RoundDownMulDiv(xx,m_pData->m_nSamplesLine,prr->Width());
			int	nLine = RoundDownMulDiv(yy,m_pData->m_nNumberLines,prr->Height());
			GetTrace(nSample,nLine,pTrace);

			nColor=0;
			switch(m_Gate.nGateMode[gg]) {
			case 0:
				break;
			case 1:
				for(ii=nStart[0];ii<=nFinish[0];ii++) {
					if(pTrace[ii]>nColor) nColor = pTrace[ii];
				}
				break;
			case 2:
				for(ii=nStart[0];(ii<=nFinish[0]) && (pTrace[ii]<m_Gate.nThreshold[gg]) ;ii++) {
					nInterfacePos = ii;
				}
				nStart[1] = (m_Gate.nNsDelay[gg] / m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod) + nInterfacePos ;
				nFinish[1] = nStart[1] + m_Gate.nNsWidth[gg]/m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod;
				if(nFinish[1]>=m_pData->m_nRealAscanLength) nFinish[1]=m_pData->m_nRealAscanLength-1;
				for(ii=nStart[1];ii<=nFinish[1];ii++) {
					if(pTrace[ii]>nColor) nColor = pTrace[ii];
				}
				break;
			};

			if(nColor>=0) {
				nColor+=128;
			} else {
				nColor=abs(nColor)+128;
			}
			nOffset=(xx + (yy * prr->Width())) * nBytesPixel;
			pBuffer=&pArray[nOffset];

			if(nAction==SCREEN) {
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[0][nColor]>>16)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[0][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[0][nColor]>>0)&0xff);
			} else {
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[0][nColor]>>0)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[0][nColor]>>8)&0xff);
				*pBuffer++=char((theApp.m_Palette.m_rgbAmplitude[0][nColor]>>16)&0xff);
			}

			if(nBytesPixel>3) pBuffer++;
		}
	}
	bmp.CreateCompatibleBitmap(pDC,prr->Width(),prr->Height());
	bmp.SetBitmapBits((DWORD)(prr->Width()*prr->Height()*nBytesPixel),pArray);
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);
	pDC->BitBlt(prr->left,prr->top,prr->Width(),prr->Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);
	delete pTrace;
	delete pArray;
	
	m_progressBar.SetPos(0);

}
*/
/*
void CVolumeViewDlg::OnProcessLeftbuttonbscans() 
{
	m_bContinuousBscans == TRUE ? m_bContinuousBscans = FALSE : m_bContinuousBscans = TRUE;

	
}

void CVolumeViewDlg::OnUpdateProcessLeftbuttonbscans(CCmdUI* pCmdUI) 
{

	pCmdUI->SetCheck(m_bContinuousBscans);
}
*/

bool CVolumeViewDlg::FindNearestGate(CPoint point, CRect rr, int *nGate, int *nEnd)
{
	int	nStart,nFinish,gg,nInterfacePos,nMaxDx,nTemp;
	ADC200Data* pAdc = &m_pData->m_TS[m_nTimeSlot].Adc;
	bool bFlag=FALSE;

	point.x-=rr.left;
	point.y-=rr.top;
	nMaxDx=8000;

	int nDelay = m_pData->m_TS[m_nTimeSlot].Adc.nDelay + (int)((float)m_nDelaySamples  * m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);
	int nWidth = (int)((float)m_nWidthSamples  * m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);

	for(gg=0;gg<8;gg++) {
		switch(m_Gate.nGateMode[gg]) {
		case 0:
			break;
		case 1:
			nStart=RoundDownMulDiv(m_Gate.nNsDelay[gg]-nDelay,rr.Width(),nWidth);
			nFinish=RoundDownMulDiv(m_Gate.nNsWidth[gg],rr.Width(),nWidth);
			if((nTemp=abs(nStart-point.x)) < nMaxDx) {
				nMaxDx=nTemp;
				*nGate=gg;
				*nEnd=0;
				bFlag=TRUE;
			};
			if((nTemp=abs(nFinish+nStart-point.x)) < nMaxDx) {
				nMaxDx=nTemp;
				*nGate=gg;
				*nEnd=1;
				bFlag=TRUE;
			};
			break;
		case 2:
			nInterfacePos=(int)((float)(m_nIntPos[0]-m_nDelaySamples)*pAdc->fSamplePeriod);
			nStart=RoundDownMulDiv(m_Gate.nNsDelay[gg]+nInterfacePos,rr.Width(),nWidth);
			nFinish=RoundDownMulDiv(m_Gate.nNsWidth[gg],rr.Width(),nWidth);
			if((nTemp=abs(nStart-point.x)) < nMaxDx) {
				nMaxDx=nTemp;
				*nGate=gg;
				*nEnd=0;
				bFlag=TRUE;
			};
			if((nTemp=abs(nFinish+nStart-point.x)) < nMaxDx) {
				nMaxDx=nTemp;
				*nGate=gg;
				*nEnd=1;
				bFlag=TRUE;
			};
			break;
		default:
			nStart = RoundDownMulDiv(m_Gate.nSampleDelay[gg]+m_Gate.nTimeSample[0],rr.Width(),pAdc->nAcqLength);
			nFinish = RoundDownMulDiv(m_Gate.nSampleWidth[gg], rr.Width(), pAdc->nAcqLength);
			if((nTemp=abs(nStart-point.x)) < nMaxDx) {
				nMaxDx=nTemp;
				*nGate=gg;
				*nEnd=0;
				bFlag=TRUE;
			};
			if((nTemp=abs(nFinish+nStart-point.x)) < nMaxDx) {
				nMaxDx=nTemp;
				*nGate=gg;
				*nEnd=1;
				bFlag=TRUE;
			};
			break;
		};
	};
	return bFlag;
}

/*
bool CVolumeViewDlg::FindNearestGate(CPoint point, CRect rr, int *nGate, int *nEnd)
{
	int	nStart,nFinish,gg,nInterfacePos,nMaxDx,nTemp;
	bool bFlag=FALSE;

	point.x-=rr.left;
	point.y-=rr.top;
	nMaxDx=8000;

	for(gg=0;gg<8;gg++) {
		switch(m_Gate.nGateMode[gg]) {
		case 0:
			break;
		case 1:
			nStart=RoundDownMulDiv(m_Gate.nNsDelay[gg]-m_nDelaySamples,rr.Width(),m_nWidthSamples);
			nFinish=RoundDownMulDiv(m_Gate.nNsWidth[gg],rr.Width(),m_nWidthSamples);
			if((nTemp=abs(nStart-point.x)) < nMaxDx) {
				nMaxDx=nTemp;
				*nGate=gg;
				*nEnd=0;
				bFlag=TRUE;
			};
			if((nTemp=abs(nFinish+nStart-point.x)) < nMaxDx) {
				nMaxDx=nTemp;
				*nGate=gg;
				*nEnd=1;
				bFlag=TRUE;
			};
			break;
		case 2:
			nInterfacePos=m_nWidthSamples;
			for(int ii=0;ii<m_pData->m_nRealAscanLength;ii++) {
				if(m_pTrace[ii]>=m_Gate.nThreshold[0]) {
					nInterfacePos=RoundDownMulDiv(ii,m_Gate.nNsTimeBaseWidth,m_pData->m_nRealAscanLength);
					ii=30000;
				};
			}
			nStart=RoundDownMulDiv(m_Gate.nNsDelay[gg]+nInterfacePos-m_nDelaySamples,rr.Width(),m_nWidthSamples);
			nFinish=RoundDownMulDiv(m_Gate.nNsWidth[gg],rr.Width(),m_nWidthSamples);
			if((nTemp=abs(nStart-point.x)) < nMaxDx) {
				nMaxDx=nTemp;
				*nGate=gg;
				*nEnd=0;
				bFlag=TRUE;
			};
			if((nTemp=abs(nFinish+nStart-point.x)) < nMaxDx) {
				nMaxDx=nTemp;
				*nGate=gg;
				*nEnd=1;
				bFlag=TRUE;
			};
			break;
		};
	};

	return bFlag;
}
*/
void CVolumeViewDlg::OnOscilloscopeDirectionHorizontal() 
{
	m_bScopeDir=FALSE;
	DrawTrace(m_nSample,m_nLine);

	
}

void CVolumeViewDlg::OnOscilloscopeDirectionVertical() 
{
	m_bScopeDir=TRUE;	
	DrawTrace(m_nSample,m_nLine);
}

void CVolumeViewDlg::OnFileSaveasbmpAscan() 
{
	CDC* pDC = GetDC();
	CRect	rr;

	m_staticScope.GetWindowRect(rr);
	ScreenToClient(rr);

//	rr.left=0;
//	rr.top=0;
//	rr.right+=12;
//	rr.bottom+=12;
	if(rr.Width()%8) rr.right=(((rr.Width()/8)+1)*8) + rr.left;

	SaveRectAsBmp(pDC,rr);

	ReleaseDC(pDC);
	
}

void CVolumeViewDlg::OnFileSaveasbmpCompletedialog() 
{
	CDC* pDC = GetDC();
	CRect	rr,rScope;

	m_staticHBscan.GetWindowRect(rr);
	ScreenToClient(rr);
	m_staticScope.GetWindowRect(rScope);
	ScreenToClient(rScope);

	rr.left=0;
	rr.top=0;
	rr.right=rScope.right;
	
	if(rr.Width()%8) rr.right=(((rr.Width()/8)+1)*8) + rr.left;

	SaveRectAsBmp(pDC,rr);

	ReleaseDC(pDC);
}

void CVolumeViewDlg::OnFileSaveasbmpHorizontalbscan() 
{
	CDC* pDC = GetDC();
	CRect	rr;

	m_staticHBscan.GetWindowRect(rr);
	ScreenToClient(rr);

//	rr.left=0;
//	rr.top=0;
//	rr.right+=12;
//	rr.bottom+=12;
	if(rr.Width()%8) rr.right=(((rr.Width()/8)+1)*8) + rr.left;

	SaveRectAsBmp(pDC,rr);

	ReleaseDC(pDC);
	
}

void CVolumeViewDlg::OnFileSaveasbmpVerticalbscan() 
{
	CDC* pDC = GetDC();
	CRect	rr;

	m_staticVBscan.GetWindowRect(rr);
	ScreenToClient(rr);

//	rr.left=0;
//	rr.top=0;
//	rr.right+=12;
//	rr.bottom+=12;
	if(rr.Width()%8) rr.right=(((rr.Width()/8)+1)*8) + rr.left;

	SaveRectAsBmp(pDC,rr);

	ReleaseDC(pDC);
	
}

void CVolumeViewDlg::OnFileSaveasbmpCscan() 
{
	CDC* pDC = GetDC();
	CRect	rr;

	m_staticView.GetWindowRect(rr);
	ScreenToClient(rr);

//	rr.left=0;
//	rr.top=0;
//	rr.right+=12;
//	rr.bottom+=12;
	if(rr.Width()%8) rr.right=(((rr.Width()/8)+1)*8) + rr.left;

	SaveRectAsBmp(pDC,rr);

	ReleaseDC(pDC);
	
}

void CVolumeViewDlg::SaveRectAsBmp(CDC *pDC, CRect rr)
{
	#define NUMBERENTRIESPALETTE 0
	CDC  dcMem;
	CBitmap bmp;
	COLORREF *pArray;

//	CDialog* pDlg = (CDialog* )GetActiveFrame();


	bmp.CreateCompatibleBitmap(pDC,rr.Width(),rr.Height());
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	dcMem.BitBlt(0,0,rr.Width(),rr.Height(),pDC,rr.left,rr.top,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);

	pArray = new COLORREF[(rr.Width()+2) * (rr.Height()+2)];
	bmp.GetBitmapBits(rr.Width()*rr.Height()*4,pArray);

	int m_SizePixels = rr.Width()*rr.Height()*(sizeof COLORREF);
	BITMAPFILEHEADER BmpFileHeader;
	
	char *Ptr = (char*)&BmpFileHeader.bfType;
	strcpy_s(Ptr,2,"BM");
	BmpFileHeader.bfSize      = sizeof(BITMAPFILEHEADER) + 
		                        sizeof(BITMAPINFOHEADER) + 
								4*NUMBERENTRIESPALETTE     + 
								m_SizePixels;

	BmpFileHeader.bfReserved1 = 0;
	BmpFileHeader.bfReserved2 = 0;
	BmpFileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + 
		                        sizeof(BITMAPINFOHEADER) + 
								4*NUMBERENTRIESPALETTE  ;


	BITMAPINFOHEADER BmpHeader;
	BmpHeader.biSize          = sizeof(BITMAPINFOHEADER);
	BmpHeader.biWidth         = rr.Width();
	BmpHeader.biHeight        = rr.Height();
	BmpHeader.biPlanes        = 1;
	BmpHeader.biBitCount      = 32;
	BmpHeader.biCompression   = 0;
	BmpHeader.biSizeImage     = m_SizePixels;
	BmpHeader.biXPelsPerMeter = 0;
	BmpHeader.biYPelsPerMeter = 0;
	BmpHeader.biClrUsed       = 0;
	BmpHeader.biClrImportant  = 0;
	
	CFileDialog FileDlg(FALSE,_T(".bmp"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("BitMap (*.bmp)|*.bmp|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath[0];
	if(FileDlg.DoModal()==IDOK) {

		CFile File;
		CFileException ExceptionInstance;
		int flag = File.Open(FileDlg.GetPathName(), CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &ExceptionInstance);
	
		File.Write(&BmpFileHeader, sizeof(BITMAPFILEHEADER));
		File.Write(&BmpHeader, sizeof(BITMAPINFOHEADER));

		char* Buff;
		int	yy;

		for(yy=rr.Height()-1;yy>=0;yy--) {
			Buff = (char *) &(pArray[yy*rr.Width()]);
			File.Write(Buff, rr.Width() * (sizeof COLORREF));
		};
		File.Close();

	}	
	delete pArray;
}


BEGIN_EVENTSINK_MAP(CVolumeViewDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CVolumeViewDlg)
	ON_EVENT(CVolumeViewDlg, IDC_USLBARCTRL_SCOPE, 1 /* ScrollHasMoved */, OnScrollHasMovedUslbarctrlScope, VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CVolumeViewDlg::OnScrollHasMovedUslbarctrlScope(long LowPosition, long HighPosition, long Width) 
{
	m_nDelaySamples = m_scrollbarDelayWidth.GetLowPosition();
	m_nWidthSamples = m_scrollbarDelayWidth.GetHighPosition() - m_nDelaySamples;
	m_fTimeBaseMM = (float)m_nWidthSamples * m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod * (float)theApp.m_LastSettings.nMaterialVelocity / 2e6f;

	m_nDelaySamples = m_nDelaySamples;
	m_nWidthSamples = m_nWidthSamples;

	DrawTrace(m_nSample,m_nLine);

	CDC* pDC = GetDC();
	DrawBScanScale((CPaintDC*)pDC);
	ReleaseDC(pDC);
}

void CVolumeViewDlg::OnFileSaveasdatfileNormal() 
{
	OnFileSaveasdatfile(0);
	
}

void CVolumeViewDlg::OnFileSaveasdatfile90rotate() 
{
	OnFileSaveasdatfile(90);
	
}

void CVolumeViewDlg::OnFileSaveasdatfile(int nOrientation) 
{
/*
	CString	FileName;

	CFileDialog FileDlg(FALSE,".dat",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Data (*.dat)|*.dat|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath;
	if(FileDlg.DoModal()==IDOK) {

		if(nOrientation == 0) {
			m_Data.m_nSamplesLine = m_pData->m_nSamplesLine;
			m_Data.m_pData->m_nNumberLines = m_pData->m_nNumberLines;
		} else {
			m_Data.m_nSamplesLine = m_pData->m_nNumberLines;
			m_Data.m_nNumberLines = m_pData->m_nSamplesLine;
		}
		m_Data.m_dFastScanSize = (double)m_Data.m_nSamplesLine * 0.1;
		m_Data.m_dSlowScanSize = (double)m_Data.m_nNumberLines * 0.1;
		m_Data.m_dFastIncrement = 0.1;
		m_Data.m_dSlowIncrement = 0.1;
		m_Data.m_nScanAxes = SCANXY;
		m_Data.m_nNumberImages = 1;
		memset(&m_StartCp,0,sizeof m_StartCp);
		memset(&m_FinishCp,0,sizeof m_FinishCp);
		m_FinishCp.dX0p = m_Data.m_dFastScanSize;
		m_FinishCp.dY0p = m_Data.m_dSlowScanSize;
		m_Data.m_StartCp = m_StartCp;
		m_Data.m_FinishCp = m_FinishCp;
		m_Data.m_nSentence = 0;
		m_Data.m_nMode = 0;
		m_Data.m_nRfDataType[0] = RFTYPELINEAR;
		m_Data.m_dGain[0] = 0.0;
		m_Data.m_nNumberAmpImages = 1;
		m_Data.m_nNumberTimeImages = 0;
		m_Data.m_nSide = 0;
		m_Data.m_nImageType = VIEW_AMPLITUDE;

		m_Data.m_Buffer[0] = new unsigned char[m_pData->m_nSamplesLine * m_pData->m_nNumberLines];
		unsigned char *pData = m_Data.m_Buffer[0];
		char *pTrace = new char[m_pData->m_nRealAscanLength];
		int	gg=1;
		int	nStart;
		int	nFinish;
		int	ii;
		int nInterfacePos;

		m_progressBar.SetRange(0,m_pData->m_nNumberLines);
		for(int nLine=0;nLine<m_pData->m_nNumberLines;nLine++) {
			m_progressBar.SetPos(nLine);
			for(int nSample=0;nSample<m_pData->m_nSamplesLine;nSample++) {
				GetTrace(nSample,nLine,pTrace);

				switch(m_Gate.nGateMode[gg]) {
				case 0:
					break;
				case 1:
					nStart = m_Gate.nNsDelay[gg]/10;
					nFinish = nStart + m_Gate.nNsWidth[gg]/10;
					m_Gate.nAmplitude[gg] = 0;
					for(ii=nStart;ii<=nFinish;ii++) {
						if(pTrace[ii]>m_Gate.nAmplitude[gg]) m_Gate.nAmplitude[gg] = pTrace[ii];
					}
					break;
				case 2:
					nInterfacePos=m_pData->m_nRealAscanLength;
					for(ii=0;ii<m_pData->m_nRealAscanLength;ii++) {
						if(pTrace[ii]>=m_Gate.nThreshold[0]) {
							nInterfacePos=RoundDownMulDiv(ii,m_Gate.nNsTimeBaseWidth,m_pData->m_nRealAscanLength);
							ii=30000;
						};
					}
					nStart = (m_Gate.nNsDelay[gg] + nInterfacePos) /10 ;
					nFinish = nStart + m_Gate.nNsWidth[gg]/10;
					m_Gate.nAmplitude[gg] = 0;
					for(ii=nStart;ii<=nFinish;ii++) {
						if(pTrace[ii]>m_Gate.nAmplitude[gg]) m_Gate.nAmplitude[gg] = pTrace[ii];
					}
					break;
				}
				if(nOrientation==0) {
					pData[m_pData->m_nSamplesLine * nLine + nSample] = m_Gate.nAmplitude[gg] + 128;
				} else {
					pData[nSample * m_pData->m_nNumberLines + nLine] = m_Gate.nAmplitude[gg] + 128;
				}

			}
		}
		m_progressBar.SetPos(0);
		delete pTrace;
		CString	Buff = FileDlg.GetPathName();
		m_Data.SaveData(&Buff);
		delete m_Data.m_Buffer[0];
		m_Data.m_Buffer[0] = NULL;
	}
*/	
}






void CVolumeViewDlg::OnToolsOptions() 
{
	if (m_pVolumeOptionsSheet == NULL) {
		m_pVolumeOptionsSheet = new CVolumeOptionsSheet(_T("Volume Options"),this,0,(CWnd**)&m_pVolumeOptionsSheet,&m_Gate,m_pData,m_pViewSheet);
		m_pVolumeOptionsSheet->Create( this);	
	} else {
		m_pVolumeOptionsSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	
}


void CVolumeViewDlg::DrawGraticule(CDC *pDC,CRect rr)
{
	CPen* pPen;
	CPen *pOldPen;
	int	nPnx,nPny,nRow,nCol,nY0;
	CString Buff;
	Buff="100";
	CSize size = pDC->GetTextExtent(Buff);
	float fHDiv;


	if(theApp.m_LastSettings.nVolumeAscanGraticule<=0) return;
	pDC->SetTextColor(theApp.m_Scope.m_rgbGraticuleColor);
	pDC->SetBkMode(TRANSPARENT);

	pPen = new CPen;
	pPen->CreatePen(PS_SOLID,0,theApp.m_Scope.m_rgbGraticuleColor);
	pOldPen=pDC->SelectObject(pPen);

	int nMaxRows = m_pData->m_nLogRange / 10;
	if(nMaxRows<=0) nMaxRows=1;

	switch(theApp.m_LastSettings.nVolumeRfType) {
	case RFTYPELINEAR:
		nY0=(rr.top+rr.bottom)/2;
		for(nRow=1;nRow<4;nRow++) {
			nPny=RoundDownMulDiv(nRow,rr.Height()/2,4);
			pDC->MoveTo(0,nY0-nPny);
			pDC->LineTo(rr.right,nY0-nPny);
			pDC->MoveTo(0,nY0+nPny);
			pDC->LineTo(rr.right,nY0+nPny);
			if(theApp.m_Scope.m_bLeftScale==TRUE) {
				Buff.Format(_T("%d %%"),nRow*25);
				pDC->TextOut(0,nY0+nPny,Buff);
				pDC->TextOut(0,nY0-nPny-size.cy,Buff);
			}
			if(theApp.m_Scope.m_bRightScale==TRUE) {
				Buff.Format(_T("%d %%"),nRow*25);
				size=pDC->GetTextExtent(Buff);
				pDC->TextOut(rr.right-size.cx-20,nY0+nPny,Buff);
				pDC->TextOut(rr.right-size.cx-20,nY0-nPny-size.cy,Buff);
			}
		};
		break;
	case 1:
	case 2:
	case 3:
		for(nRow=1;nRow<10;nRow++) {
			nPny=RoundDownMulDiv(nRow,rr.Height(),10);
			pDC->MoveTo(0,nPny+rr.top);
			pDC->LineTo(rr.right,nPny+rr.top);
			if(theApp.m_Scope.m_bLeftScale==TRUE) {
				Buff.Format(_T("%d %%"),100-nRow*10);
				pDC->TextOut(0,nPny+rr.top-size.cy,Buff);
			}
			if(theApp.m_Scope.m_bRightScale==TRUE) {
				Buff.Format(_T("%d %%"),100-nRow*10);
				size=pDC->GetTextExtent(Buff);
				pDC->TextOut(rr.right-size.cx-20,nPny+rr.top-size.cy,Buff);
			}
		};
		break;
	case 4:
		for(nRow=1;nRow<=nMaxRows;nRow++) {
			nPny=RoundDownMulDiv(nRow*10,rr.Height(),m_pData->m_nLogRange);
			pDC->MoveTo(0,nPny+rr.top);
			pDC->LineTo(rr.right,nPny+rr.top);
			if(theApp.m_Scope.m_bLeftScale==TRUE) {
				Buff.Format(_T("%ddB"),(nRow)*10);
				pDC->TextOut(0,nPny+rr.top-size.cy,Buff);
			}
			if(theApp.m_Scope.m_bRightScale==TRUE) {
				Buff.Format(_T("%ddB"),(nRow)*10);
				size=pDC->GetTextExtent(Buff);
				pDC->TextOut(rr.right-size.cx-20,nPny+rr.top-size.cy,Buff);
			}
		};
		break;
	};

	switch(theApp.m_Scope.m_nTraceWidthUnits) {
	case 0:	fHDiv = (float)m_pData->m_TS[m_nTimeSlot].Adc.nWidth/10000;
		break;
	case 1:	fHDiv = (float)(m_pData->m_TS[m_nTimeSlot].Adc.nWidth*theApp.m_LastSettings.nWaterVelocity)/20e6f;
		break;
	case 2:	fHDiv = (float)(m_pData->m_TS[m_nTimeSlot].Adc.nWidth*theApp.m_LastSettings.nMaterialVelocity)/20e6f;
		break;
	}

	for(nCol=1;nCol<10;nCol++) {
		nPnx=RoundDownMulDiv(nCol,rr.Width(),10);
		pDC->MoveTo(nPnx,rr.top);
		pDC->LineTo(nPnx,rr.bottom);
		if(theApp.m_Scope.m_bBottomScale) {
			Buff.Format(_T("%.01f"),(float)nCol * fHDiv);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(nPnx-size.cx,rr.bottom-size.cy,Buff);
		}
		if(theApp.m_Scope.m_bTopScale) {
			Buff.Format(_T("%.01f"),(float)nCol * fHDiv);
			size = pDC->GetTextExtent(Buff);
			pDC->TextOut(nPnx-size.cx,rr.top+size.cy,Buff);
		}
	};

	pDC->SelectObject(pOldPen);
	delete pPen;
}

void CVolumeViewDlg::DrawText(CDC *pDC,CRect rr)
{
	CString Buff,BuffDelay,BuffWidth,Language;
	CString Units[2]={{L"\x0b5s"} , {L"mm"}};

	pDC->SetTextColor(theApp.m_Scope.m_rgbTextColor);
	pDC->SetBkMode(TRANSPARENT);
	
	float fDelay = (float)m_pData->m_TS[m_nTimeSlot].Adc.nDelay + m_nDelaySamples * m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod;
	float fWidth = m_nWidthSamples * m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod;
	switch(theApp.m_Scope.m_nTraceDelayUnits) {
	case 0:	BuffDelay.Format(_T("%.02f"),fDelay/1000.0f);
		BuffDelay+=Units[0];
		break;
	case 1:	BuffDelay.Format(_T("%.01f"),fDelay*(float)theApp.m_LastSettings.nWaterVelocity/2e6f);
		BuffDelay+=Units[1];
		break;
	case 2:	BuffDelay.Format(_T("%.01f"),fDelay*(float)theApp.m_LastSettings.nMaterialVelocity/2e6f);
		BuffDelay+=Units[1];
		break;
	}
	switch(theApp.m_Scope.m_nTraceWidthUnits) {
	case 0:	BuffWidth.Format(_T("%.04f"),fWidth/10000.0f);
		BuffWidth+=Units[0];
		break;
	case 1:	BuffWidth.Format(_T("%.01f"),fWidth*(float)theApp.m_LastSettings.nWaterVelocity/20e6f);
		BuffWidth+=Units[1];
		break;
	case 2:	BuffWidth.Format(_T("%.01f"),fWidth*(float)theApp.m_LastSettings.nMaterialVelocity/20e6f);
		BuffWidth+=Units[1];
		break;
	}
	BuffWidth+="/div";
	Buff=BuffDelay+"   "+BuffWidth;

	float fPath;
	if(theApp.m_Scope.m_bWaterPathText==TRUE) {
		fPath=GetGateWaterPath();
		CString BuffPath;
		Language.LoadString(IDS_WP);
		BuffPath.Format(_T(" %s=%.02fmm"),Language,fPath);
		Buff += BuffPath;
	}
	if(theApp.m_Scope.m_nTOFText[0]>0) {
		fPath=GetGateThickness();
		CString BuffPath;
		Language.LoadString(IDS_MP);
		BuffPath.Format(_T(" %s=%.03fmm"),Language,fPath);
		Buff += BuffPath;
	}
	if(theApp.m_Scope.m_bTimeSlotName) {
		Language.LoadString(IDS_Probe);
		Buff += L" " + Language + L"=";
		Buff += theApp.m_UtUser.m_TS[m_nTimeSlot].cName;
	}

	CSize size=pDC->GetTextExtent(Buff);
	pDC->TextOut(rr.Width()/2-size.cx/2,0,Buff);

}

float CVolumeViewDlg::GetGateWaterPath()
{
	double WaterPathnS;

	if(m_nIntPos[0]==m_pData->m_nRealAscanLength)
		return 0;

	WaterPathnS = (m_nIntPos[0]*m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);

	return (float)(( WaterPathnS/1000.0f) * ((double)theApp.m_LastSettings.nWaterVelocity/1000.0f) /2.0);
}

float CVolumeViewDlg::GetGateThickness()
{
	double MaterialPathnS;

	if(m_nIntPos[1]==m_pData->m_nRealAscanLength)
		return 0;

	MaterialPathnS = ((m_nIntPos[1]-m_nIntPos[0])*m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod);

	return (float)((MaterialPathnS/1000.0f) * ((double)theApp.m_LastSettings.nMaterialVelocity/1000.0f) / 2.0);

}





int CVolumeViewDlg::MinMax(int *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}


long CVolumeViewDlg::MinMax(long *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}

float CVolumeViewDlg::MinMax(float *pfV, float fMin, float fMax)
{

	if(*pfV<fMin) *pfV=fMin;
	if(*pfV>fMax) *pfV=fMax;

	return *pfV;
}

void CVolumeViewDlg::SetToolBarCheckedState()
{
	int	nIndex;

	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_TOOLS_CROSSHAIR))>=0)
		m_nToolOption==0 ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_TOOLS_RULER))>=0)
		m_nToolOption==TOOL_RULER ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_TOOLS_H_BSCAN))>=0)
		theApp.m_LastSettings.nVolumeDisplayMask & H_BSCAN ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
	if((nIndex=m_wndToolsToolBar.CommandToIndex(ID_TOOLS_V_BSCAN))>=0)
		theApp.m_LastSettings.nVolumeDisplayMask & V_BSCAN ? m_wndToolsToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wndToolsToolBar.SetButtonStyle(nIndex,0);
}

BOOL CVolumeViewDlg::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
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
   case ID_TOOLS_CROSSHAIR: strTipText.LoadString(IDS_CrossHair);
	   break;
   case ID_TOOLS_H_BSCAN: strTipText.LoadString(IDS_Horizontal_Bscan);
	   break;
   case ID_TOOLS_V_BSCAN: strTipText.LoadString(IDS_Vertical_Bscan);
	   break;
   case ID_TOOLS_AMP_CSCAN: strTipText.LoadString(IDS_Amplitude_Cscan);
	   break;
   case ID_TOOLS_THICK_CSCAN: strTipText.LoadString(IDS_Thickness_Cscan);
	   break;
   case ID_TOOLS_OPTIONS: strTipText.LoadString(IDS_Options);
	   break;
   case ID_TOOLS_RULER: strTipText.LoadString(IDS_Ruler);
	   break;
   case ID_BUTTON_FULL_SCREEN: strTipText.LoadString(IDS_Full_screen);
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


void CVolumeViewDlg::OnToolsHBscan() 
{
	theApp.m_LastSettings.nVolumeDisplayMask & H_BSCAN ? theApp.m_LastSettings.nVolumeDisplayMask &= ~H_BSCAN : theApp.m_LastSettings.nVolumeDisplayMask |= H_BSCAN;
	SetToolBarCheckedState();
}

void CVolumeViewDlg::OnToolsVBscan() 
{
	theApp.m_LastSettings.nVolumeDisplayMask & V_BSCAN ? theApp.m_LastSettings.nVolumeDisplayMask &= ~V_BSCAN : theApp.m_LastSettings.nVolumeDisplayMask |= V_BSCAN;
	SetToolBarCheckedState();
}

void CVolumeViewDlg::OnToolsThickCscan() 
{
	m_nCscanNumber = m_pData->VolumeGenerateTimeCscan(&m_File,&m_progressBar,&m_Gate,m_nVolumeNumber,m_nTimeSlot,m_nEncoding);
	m_nImageType=TIME_IMAGE;
	m_bRenderView=TRUE;
	Invalidate(FALSE);
	AddStringsImageCombo();
}

void CVolumeViewDlg::OnToolsAmpCscan() 
{
	CreateAmpCscan();
	
}

int CVolumeViewDlg::CreateAmpCscan(CString Name)
{
	m_nCscanNumber = m_pData->VolumeGenerateAmpCscan(&m_File,&m_progressBar,&m_Gate,m_nVolumeNumber,m_nTimeSlot,m_nEncoding,Name);
	m_nImageType=AMP_IMAGE;
	m_bRenderView=TRUE;
	Invalidate(FALSE);
	AddStringsImageCombo();

	return m_nCscanNumber;

}

void CVolumeViewDlg::OnToolsCrosshair() 
{
	m_nToolOption = 0;
	SetToolBarCheckedState();
}

void CVolumeViewDlg::OnToolsRuler() 
{
	m_nToolOption = TOOL_RULER;
	SetToolBarCheckedState();
}

void CVolumeViewDlg::ClientToWorld(CPoint pt, float *fX, float *fY)
{
	*fX = m_pData->m_fFastIncrement * (float)m_prrSamples->Width();
	*fY = m_pData->m_fSlowIncrement * (float)m_prrSamples->Height();


	switch(theApp.m_LastSettings.nImageOrientation) {
	case 0:
		*fX = *fX * (float)(pt.x - m_rrCscan.left) / (float)(m_rrCscan.Width()-1) + ((float)m_prrSamples->left * m_pData->m_fFastIncrement);
		*fY = *fY * (float)(pt.y - m_rrCscan.top) / (float)(m_rrCscan.Height()-1) + ((float)m_prrSamples->top * m_pData->m_fSlowIncrement);
		break;
	case 1:
		*fX = *fX * (float)(pt.x - m_rrCscan.left) / (float)(m_rrCscan.Width()-1) + ((float)m_prrSamples->left * m_pData->m_fFastIncrement);
		*fY = *fY * (float)(m_rrCscan.bottom - pt.y) / (float)(m_rrCscan.Height()-1) + ((float)m_prrSamples->top * m_pData->m_fSlowIncrement);
		break;
	case 2:
		*fX = *fX * (float)(m_rrCscan.right - pt.x) / (float)(m_rrCscan.Width()-1) + ((float)m_prrSamples->left * m_pData->m_fFastIncrement);
		*fY = *fY * (float)(pt.y - m_rrCscan.top) / (float)(m_rrCscan.Height()-1) + ((float)m_prrSamples->top * m_pData->m_fSlowIncrement);
		break;
	case 3:
		*fX = *fX * (float)(m_rrCscan.right - pt.x) / (float)(m_rrCscan.Width()-1) + ((float)m_prrSamples->left * m_pData->m_fFastIncrement);
		*fY = *fY * (float)(m_rrCscan.bottom - pt.y) / (float)(m_rrCscan.Height()-1) + ((float)m_prrSamples->top * m_pData->m_fSlowIncrement);
		break;
	}
}

void CVolumeViewDlg::WorldToClient(CPoint *pt, int nSample, int nLine)
{

	switch(theApp.m_LastSettings.nImageOrientation) {
	case 0:
		pt->x = RoundDownMulDiv(nSample-m_prrSamples->left,m_rrCscan.Width(),m_prrSamples->Width())+m_rrCscan.left;
		pt->y = RoundDownMulDiv(nLine-m_prrSamples->top,m_rrCscan.Height(),m_prrSamples->Height())+m_rrCscan.top;
		
//		m_nSample = RoundDownMulDiv(point.x-rr.left,m_prrSamples->Width(),rr.Width())+m_prrSamples->left;
//		m_nLine = RoundDownMulDiv(point.y-rr.top,m_prrSamples->Height(),rr.Height())+m_prrSamples->top;
		break;
	case 1:
		pt->x = RoundDownMulDiv(nSample-m_prrSamples->left,m_rrCscan.Width(),m_prrSamples->Width())+m_rrCscan.left;
		pt->y = m_rrCscan.top + (m_rrCscan.Height() - RoundDownMulDiv(nLine-m_prrSamples->top,m_rrCscan.Height(),m_prrSamples->Height()));

//		m_nSample = RoundDownMulDiv(point.x-rr.left,m_prrSamples->Width(),rr.Width())+m_prrSamples->left;
//		m_nLine = m_prrSamples->top + (m_prrSamples->Height() - RoundDownMulDiv(point.y-rr.top,m_prrSamples->Height(),rr.Height()));
		break;
	case 2:
		pt->x = m_rrCscan.left + (m_rrCscan.Width() - RoundDownMulDiv(nSample-m_prrSamples->left,m_rrCscan.Width(),m_prrSamples->Width()));
		pt->y = RoundDownMulDiv(nLine-m_prrSamples->top,m_rrCscan.Height(),m_prrSamples->Height())+m_rrCscan.top;

//		m_nSample = m_prrSamples->left + (m_prrSamples->Width() - RoundDownMulDiv(point.x-rr.left,m_prrSamples->Width(),rr.Width()));
//		m_nLine = RoundDownMulDiv(point.y-rr.top,m_prrSamples->Height(),rr.Height());
		break;
	case 3:
  		pt->x = m_rrCscan.left + (m_rrCscan.Width() - RoundDownMulDiv(nSample-m_prrSamples->left,m_rrCscan.Width(),m_prrSamples->Width()));
		pt->y = m_rrCscan.top + (m_rrCscan.Height() - RoundDownMulDiv(nLine-m_prrSamples->top,m_rrCscan.Height(),m_prrSamples->Height()));

//		m_nSample = m_prrSamples->left + (m_prrSamples->Width() - RoundDownMulDiv(point.x-rr.left,m_prrSamples->Width(),rr.Width()));
//		m_nLine = m_prrSamples->top + (m_prrSamples->Height() - RoundDownMulDiv(point.y-rr.top,m_prrSamples->Height(),rr.Height()));
		break;
	}

}

void CVolumeViewDlg::DrawRuler(CPaintDC *pDC, int nWhich)
{
	CRect rr;
	int	nX,nY;
	CPoint pt[4];
	float fLength,fX0,fX1,fY0,fY1,fX;
	CFont Font;
	CString Buff;

	if(m_nToolOption!=TOOL_RULER) return;
	if(nWhich==0) {
		m_staticView.GetWindowRect(rr);
		ScreenToClient(rr);
		if(rr.PtInRect(m_rrRuler.TopLeft()) != TRUE) return;
	}
	if(nWhich==1) {
		m_staticHBscan.GetWindowRect(rr);
		ScreenToClient(rr);
		if(rr.PtInRect(m_rrRuler.TopLeft()) != TRUE) return;
	}
	if(nWhich==2) {
		m_staticVBscan.GetWindowRect(rr);
		ScreenToClient(rr);
		if(rr.PtInRect(m_rrRuler.TopLeft()) != TRUE) return;
	}


	CPen penBkRuler(PS_SOLID,1,GetSysColor(COLOR_INFOBK));
	CPen pen(PS_SOLID,theApp.m_LastSettings.nRulerWidth,theApp.m_LastSettings.rgbRulerLine);
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush brushBkRuler(GetSysColor(COLOR_INFOBK));
	CBrush* pOldBrush = pDC->SelectObject(&brushBkRuler);

	int nLength=(int)sqrt((double)(m_rrRuler.Width()*m_rrRuler.Width() + m_rrRuler.Height()*m_rrRuler.Height()));

	switch(theApp.m_LastSettings.nRulerStyle) {
	case RULER_RECTANGLE_STYLE:
		pDC->MoveTo(m_rrRuler.TopLeft());
		pDC->LineTo(m_rrRuler.right,m_rrRuler.top);
		pDC->LineTo(m_rrRuler.BottomRight());
		pDC->LineTo(m_rrRuler.left,m_rrRuler.bottom);
		pDC->LineTo(m_rrRuler.TopLeft());
		break;
	case RULER_ELLIPSE_STYLE:
		pDC->SelectStockObject(HOLLOW_BRUSH);
		rr = m_rrRuler;
		rr.left=m_rrRuler.right-m_rrRuler.Height()/2;
		rr.right=m_rrRuler.right+m_rrRuler.Height()/2;
		pDC->Ellipse(&rr);
		break;
	default:
		switch(theApp.m_LastSettings.nRulerWidth) {
		default:
			pDC->MoveTo(m_rrRuler.TopLeft());
			pDC->LineTo(m_rrRuler.BottomRight());
			break;
		case 4:
			if(nLength) {
				theApp.m_LastSettings.lfViewGraticule.lfEscapement=(long)(atan2(m_rrRuler.Height(),m_rrRuler.Width()) * RAD_TO_DEG * -10.0);
				Font.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
				CFont* pOldFont = pDC->SelectObject(&Font);

				pt[0]=m_rrRuler.TopLeft();
				pt[1]=m_rrRuler.BottomRight();
				pt[2].x = m_rrRuler.right - RoundDownMulDiv(m_rrRuler.Height(),14,nLength);
				pt[2].y = m_rrRuler.bottom + RoundDownMulDiv(m_rrRuler.Width(),14,nLength);
				pt[3].x = m_rrRuler.left - RoundDownMulDiv(m_rrRuler.Height(),14,nLength);
				pt[3].y = m_rrRuler.top + RoundDownMulDiv(m_rrRuler.Width(),14,nLength);
				pDC->SelectObject(&penBkRuler);
				pDC->Polygon(pt,4);
				if(nWhich==0) {
					ClientToWorld(m_rrRuler.TopLeft(),&fX0,&fY0);
					ClientToWorld(m_rrRuler.BottomRight(),&fX1,&fY1);
				} else {
					HBscanToWorld(m_rrRuler.TopLeft(),&fX0,&fY0);
					HBscanToWorld(m_rrRuler.BottomRight(),&fX1,&fY1);
				}
				fLength=(float)sqrt((fX0-fX1)*(fX0-fX1) + (fY0-fY1)*(fY0-fY1));
				pDC->SelectObject(pOldPen);
				pDC->MoveTo(m_rrRuler.TopLeft());
				pDC->LineTo(m_rrRuler.BottomRight());
				float fStep = theApp.m_LastSettings.fRulerScale;
				if(fStep<0.1f) fStep=0.1f;

				switch(theApp.m_LastSettings.nMeasurementUnits) {
				case 1: fLength /= 25.4f;
					break;
				}
				for(fX=0.0f;fX<fLength;fX+=fStep) {
					nX=m_rrRuler.left + (int)(fX*(float)(m_rrRuler.Width())/fLength);
					nY=m_rrRuler.top + (int)(fX*(float)(m_rrRuler.Height())/fLength);
					pDC->MoveTo(nX,nY);
					Buff.Format(_T("%.0f"),fX);
					pDC->TextOut(nX,nY,Buff);

					nX = nX - RoundDownMulDiv(m_rrRuler.Height(),5,nLength);
					nY = nY + RoundDownMulDiv(m_rrRuler.Width(),5,nLength);
					pDC->LineTo(nX,nY);

				}

				pDC->SelectObject(pOldFont);
			}
			break;
		}
		break;
	}


	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);

}

void CVolumeViewDlg::OnFileReport() 
{
/*
	if (m_pReportPrintDlg == NULL) {
		m_pReportPrintDlg = new CReportPrintDlg(this,(CWnd**)&m_pReportPrintDlg,this,m_pData);
		m_pReportPrintDlg->Create(CReportPrintDlg::IDD,this);	
	} else {
		m_pReportPrintDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
*/	
}

void CVolumeViewDlg::DrawCScanScale(CPaintDC *pDC)
{
	float	fStep=100.0f;
	int	nTextStep=500;
	int ii;
	float xx,yy,fXSize,fYSize,fFastScanSize,fFastIncrement;
	int	pnx,pny;
	CString Buff;
	CSize TextSize;
	CFont HorizontalFont;
	CFont VerticalFont;
	CFont* pOldFont;
	CRect rrClient,rr;
	CBrush brushBK(GetSysColor(COLOR_BTNFACE));

	m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);

	GetGraticuleSize(&fXSize,&fYSize,&fStep,&nTextStep);
	float xStart = (float)m_prrSamples->left * (float)fFastIncrement;
	float yStart = (float)m_prrSamples->top * (float)m_pData->m_fSlowIncrement;

	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
	theApp.m_LastSettings.lfViewGraticule.lfEscapement=900;
	VerticalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);

	pOldFont = pDC->SelectObject(&HorizontalFont);
	pDC->SetBkMode(TRANSPARENT);

	Buff.Format(_T("999.99"));
	TextSize = pDC->GetTextExtent(Buff);

	//Clear bottom border
	rr=m_rrCscan;
	rr.top=m_rrCscan.bottom+1;
	rr.bottom=rr.top + 6 + TextSize.cy;
	pDC->FillRect(&rr,&brushBK);


	rr=m_rrCscan;
	rr.InflateRect(2,2);

	if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_BOTTOM) {
		for(xx=0,ii=0;xx<fXSize;xx+=fStep,ii++) {
			pnx=RoundDownMulDiv((int)xx,rr.Width()-1,(int)fXSize)+rr.left;
			pDC->MoveTo(pnx,rr.bottom);
			pDC->LineTo(pnx,rr.bottom+4);
			if(((int)xx%nTextStep) == 0) {
				if(theApp.m_LastSettings.nGraticuleScrollMode==0) {
					Buff.Format(_T("%.01f"),xx);
				} else {
					Buff.Format(_T("%.01f"),xx+xStart);
				}
				TextSize=pDC->GetTextExtent(Buff);
				if(ii==0) {
					pDC->TextOut(pnx,rr.bottom+3,Buff);
				} else {
					pDC->TextOut(pnx-TextSize.cx/2,rr.bottom+3,Buff);
				}
			};
		}
		for(xx=0,ii=0;xx<fXSize;xx+=(fStep/10),ii++) {
			pnx=RoundDownMulDiv((int)xx,rr.Width(),(int)fXSize)+rr.left;
			if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_BOTTOM) {
				pDC->MoveTo(pnx,rr.bottom);
				pDC->LineTo(pnx,rr.bottom+2);
			}
		};
	};

	pDC->SelectObject(&VerticalFont);
	Buff.Format(_T("999.99"));
	TextSize = pDC->GetTextExtent(Buff);

	//Clear right border
	rr=m_rrCscan;
	rr.left=m_rrCscan.right+1;
	rr.right=rr.left + 6 + TextSize.cy;
	pDC->FillRect(&rr,&brushBK);

	
	rr=m_rrCscan;
	rr.InflateRect(2,2);
	if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_RIGHT) {
		for(yy=0,ii=0;yy<fYSize;yy+=fStep,ii++) {
			if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
				pny=RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize)+rr.top;
			} else {
				pny=rr.bottom-RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize);
			};
			pDC->MoveTo(rr.right,pny);
			pDC->LineTo(rr.right+6,pny);
			if(((int)yy%nTextStep) == 0) {
				if(theApp.m_LastSettings.nGraticuleScrollMode==0) {
					Buff.Format(_T("%.01f"),yy);
				} else {
					Buff.Format(_T("%.01f"),yy+yStart);
				}
				TextSize=pDC->GetTextExtent(Buff);
				if(ii==0) {
					pDC->TextOut(rr.right+3,pny,Buff);
				} else {
					pDC->TextOut(rr.right+3,pny+TextSize.cx/2,Buff);
				}
			};
		}
		for(yy=0,ii=0;yy<fYSize;yy+=(fStep/10),ii++) {
			if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
				pny=RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize)+rr.top;
			} else {
				pny=rr.bottom-RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize);
			};
			pDC->MoveTo(rr.right,pny);
			pDC->LineTo(rr.right+3,pny);
		}
	}
	if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_LEFT) {
		for(yy=0,ii=0;yy<fYSize;yy+=fStep,ii++) {
			if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
				pny=RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize)+rr.top;
			} else {
				pny=rr.bottom-RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize);
			};
			pDC->MoveTo(rr.left,pny);
			pDC->LineTo(rr.left-6,pny);
			if(((int)yy%nTextStep) == 0) {
				if(theApp.m_LastSettings.nGraticuleScrollMode==0) {
					Buff.Format(_T("%.01f"),yy);
				} else {
					Buff.Format(_T("%.01f"),yy+yStart);
				}
				TextSize=pDC->GetTextExtent(Buff);
				if(ii==0) {
					pDC->TextOut(rr.left-3-TextSize.cy,pny+TextSize.cx,Buff);
				} else {
					pDC->TextOut(rr.left-3-TextSize.cy,pny+TextSize.cx/2,Buff);
				}
			};
		}
		for(yy=0,ii=0;yy<fYSize;yy+=(fStep/10),ii++) {
			if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
				pny=RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize)+rr.top;
			} else {
				pny=rr.bottom-RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize);
			};
			pDC->MoveTo(rr.left,pny);
			pDC->LineTo(rr.left-3,pny);
		}
	};

	pDC->SelectObject(pOldFont);
	
	fStep/=10;
	CPen penLine(PS_SOLID,1,theApp.m_LastSettings.rgbViewGraticuleColor);
	CPen* pOldPen = pDC->SelectObject(&penLine);
	if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_LINES) {
		for(xx=fStep;xx<fXSize;xx+=fStep,ii++) {
			pnx=RoundDownMulDiv((int)xx,rr.Width()-1,(int)fXSize)+rr.left;
			if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_BOTTOM) {
				pDC->MoveTo(pnx,m_rrCscan.bottom);
				pDC->LineTo(pnx,m_rrCscan.top);
			}
		};
		for(yy=fStep,ii=0;yy<fYSize;yy+=fStep,ii++) {
			if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
				pny=RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize)+rr.top;
			} else {
				pny=rr.bottom-RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize);
			};
			pDC->MoveTo(m_rrCscan.left,pny);
			pDC->LineTo(m_rrCscan.right,pny);
		}
	}
	if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_DOTS) {
		for(yy=fStep,ii=0;yy<fYSize;yy+=fStep,ii++) {
			if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
				pny=RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize)+rr.top;
			} else {
				pny=rr.bottom-RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize);
			};
			for(xx=fStep,ii=0;xx<fXSize;xx+=fStep,ii++) {
				pnx=RoundDownMulDiv((int)xx,rr.Width()-1,(int)fXSize)+rr.left;
				pDC->SetPixel(pnx,pny,theApp.m_LastSettings.rgbViewGraticuleColor);
			};
		}
	}
	if(theApp.m_LastSettings.nViewGraticuleStyle & GRATICULE_CROSSES) {
		for(yy=fStep,ii=0;yy<(fYSize-(fStep/2));yy+=fStep,ii++) {
			if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
				pny=RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize)+rr.top;
			} else {
				pny=rr.bottom-RoundDownMulDiv((int)yy,rr.Height()-1,(int)fYSize);
			};
			for(xx=fStep,ii=0;xx<(fXSize-(fStep/2));xx+=fStep,ii++) {
				pnx=RoundDownMulDiv((int)xx,rr.Width()-1,(int)fXSize)+rr.left;
				pDC->MoveTo(pnx-3,pny);
				pDC->LineTo(pnx+4,pny);
				pDC->MoveTo(pnx,pny-3);
				pDC->LineTo(pnx,pny+4);
			};
		}
	}


	pDC->SelectObject(pOldPen);

}

void CVolumeViewDlg::GetGraticuleSize(float *fXSize, float *fYSize,float *fStep,int *nTextStep)
{
	float fTemp,fFastScanSize,fFastIncrement;

	m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);

	*fXSize = fFastIncrement * (float)m_prrSamples->Width();
	*fYSize = m_pData->m_fSlowIncrement *  (float)m_prrSamples->Height();

	*fXSize>=*fYSize ? fTemp = *fXSize : fTemp = *fYSize;

	if(fTemp<=10000) {
		*fStep = 500.0;
		*nTextStep=1000;
	};
	if(fTemp<=1000) {
		*fStep = 100.0;
		*nTextStep=500;
	};
	if(fTemp<=500) {
		*fStep = 100.0;
		*nTextStep=100;
	};
	if(fTemp<=100) {
		*fStep = 10.0;
		*nTextStep=50;
	};
	if(fTemp<=50) {
		*fStep = 10.0;
		*nTextStep=10;
	};
}

void CVolumeViewDlg::AddStringsImageCombo()
{
	int gg,nAmp,nThick;
	CString Buff;
	CString Amp;
	CString Thick;
	Amp.LoadString(IDS_Amp);
	Thick.LoadString(IDS_Thick);

	CComboBox* pCombo = (CComboBox* ) m_wndImageDialogBar.GetDlgItem(IDC_COMBO_IMAGE);
	pCombo->ResetContent();


	for(gg=0,nAmp=0;gg<NUMBER_BUFFERS;gg++) {
		if(m_pData->m_cAmpBuffer[gg]!=NULL) {
			if(m_pData->m_ImageName[gg]=="") {
				Buff.Format(_T("%s %d"),Amp,gg+1);
			}else{
				Buff.Format(_T("%s"),m_pData->m_ImageName[gg]); 
			}
			pCombo->AddString(Buff);
			nAmp++;
		}
	}
	for(gg=0,nThick=0;gg<NUMBER_BUFFERS;gg++) {
		if(m_pData->m_nTimeBuffer[gg]!=NULL) {
			Buff.Format(_T("%s %d"),Thick,gg+1);
			pCombo->AddString(Buff);
			nThick++;
		}
	}
	switch(m_nImageType) {
	case AMP_IMAGE: pCombo->SetCurSel(m_nCscanNumber);
		break;
	case TIME_IMAGE: pCombo->SetCurSel(m_nCscanNumber + nAmp);
		break;
	}

}

void CVolumeViewDlg::OnSelendokImage()
{
	CComboBox* pCombo = (CComboBox* ) m_wndImageDialogBar.GetDlgItem(IDC_COMBO_IMAGE);
	int gg,nAmp,nThick;

	int nn = pCombo->GetCurSel();

	for(gg=0,nAmp=0;gg<NUMBER_BUFFERS;gg++) {
		if(m_pData->m_cAmpBuffer[gg]!=NULL) {
			nAmp++;
		}
	}
	for(gg=0,nThick=0;gg<NUMBER_BUFFERS;gg++) {
		if(m_pData->m_nTimeBuffer[gg]!=NULL) {
			nThick++;
		}
	}

	if(nn<nAmp) {
		m_nCscanNumber=nn;
		m_nImageType=AMP_IMAGE;
	} else {
		m_nCscanNumber=nn-nAmp;
		m_nImageType=TIME_IMAGE;
	}
	m_bRenderView=TRUE;
	Invalidate(FALSE);

}

void CVolumeViewDlg::SetZoomSamples()
{
	CViewSheet* pParent = (CViewSheet*)GetParent();
	int	nWidth,nHeight;
	float	dXSize,dYSize,fFastScanSize,fFastIncrement;
	SCROLLINFO	info;
	CRect rr;
	m_staticView.GetWindowRect(&rr);

	m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);

	if(rr.Height()>0) {
		float dScreenRatio=(float)rr.Width()/(float)rr.Height();
		dXSize=(fFastScanSize*(float)m_nZoomFactor)/1000.0f;
		dYSize=(m_pData->m_fSlowScanSize*(float)m_nZoomFactor)/1000.0f;
		float dScanRatio=dXSize/dYSize;
		dYSize=dXSize / dScreenRatio;
		nWidth = (int)(dXSize / fFastIncrement);
		nHeight = (int)(dYSize / m_pData->m_fSlowIncrement);
		m_prrSamples->right=m_prrSamples->left + nWidth;
		m_prrSamples->bottom=m_prrSamples->top + nHeight;
	};
	if(m_prrSamples->bottom<m_prrSamples->top) {
		ASSERT(TRUE);
	};

	if(m_prrSamples->right>(m_pData->m_nSamplesLine-1)) {
		m_prrSamples->right = m_pData->m_nSamplesLine-1;
		m_prrSamples->left = m_prrSamples->right - nWidth;
	};
	if(m_prrSamples->left<0) m_prrSamples->left=0;
	if(m_prrSamples->bottom>(m_pData->m_nNumberLines-1)) {
		m_prrSamples->bottom = m_pData->m_nNumberLines-1;
		m_prrSamples->top = m_prrSamples->bottom - nHeight;
	};

	MinMax(&m_prrSamples->top,0,100000);
	MinMax(&m_prrSamples->bottom,0,100000);
	MinMax(&m_prrSamples->left,0,100000);
	MinMax(&m_prrSamples->right,0,100000);

	m_scrollHCscanBar.SetScrollRange(0,m_pData->m_nSamplesLine-1,FALSE);
	m_scrollHCscanBar.SetScrollPos(m_prrSamples->left,FALSE);
	m_scrollHCscanBar.GetScrollInfo(&info,SIF_ALL);
	info.nPage = m_prrSamples->Width();
	m_scrollHCscanBar.SetScrollInfo(&info,TRUE);

	m_scrollVCscanBar.SetScrollRange(0,m_pData->m_nNumberLines-1,FALSE);
	m_scrollVCscanBar.GetScrollInfo(&info,SIF_ALL);
	info.nPage = m_prrSamples->Height();
	m_scrollVCscanBar.SetScrollInfo(&info,TRUE);
	if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
		m_scrollVCscanBar.SetScrollPos(m_prrSamples->top,FALSE);
	} else {
		m_scrollVCscanBar.SetScrollPos(m_pData->m_nNumberLines-1-(m_prrSamples->top+m_prrSamples->Height()),FALSE);
	};

	if(m_pVolumeOptionsSheet)
		m_pVolumeOptionsSheet->SendMessage(UI_UPDATE_ALL_PAGES);

}

void CVolumeViewDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int	nPage = RoundDownMulDiv(theApp.m_LastSettings.nViewPagePercent,m_prrSamples->Width(),100);

	int CurrentPos = m_scrollHCscanBar.GetScrollPos();
	switch(nSBCode) {
	case SB_LINEDOWN:
		CurrentPos+=(nPage/10);
		break;
	case SB_LINEUP:
		 CurrentPos-=(nPage/10);
		break;
	case SB_PAGEDOWN:
		CurrentPos+=nPage;
		break;
	case SB_PAGEUP:
		CurrentPos-=nPage;
		break;
	case SB_THUMBTRACK:
		CurrentPos = nPos;
		break;
	}
	if (CurrentPos < 0) CurrentPos = 0;
	if(CurrentPos>=m_pData->m_nSamplesLine) CurrentPos=m_pData->m_nSamplesLine-1;

	m_scrollHCscanBar.SetScrollPos(CurrentPos,TRUE);

	m_prrSamples->left=CurrentPos;
	m_bRenderView=TRUE;
	m_bInitialize=TRUE;
	Invalidate(FALSE);
	
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CVolumeViewDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int	nPage = RoundDownMulDiv(theApp.m_LastSettings.nViewPagePercent,m_prrSamples->Width(),100);

	int CurrentPos = m_scrollVCscanBar.GetScrollPos();
	switch(nSBCode) {
	case SB_LINEDOWN:
		CurrentPos+=(nPage/10);
		break;
	case SB_LINEUP:
		 CurrentPos-=(nPage/10);
		break;
	case SB_PAGEDOWN:
		CurrentPos+=nPage;
		break;
	case SB_PAGEUP:
		CurrentPos-=nPage;
		break;
	case SB_THUMBTRACK:
		CurrentPos = nPos;
		break;
	}
	if(CurrentPos < 0) CurrentPos = 0;
	if(CurrentPos>=m_pData->m_nNumberLines) CurrentPos=m_pData->m_nNumberLines-1;

	m_scrollVCscanBar.SetScrollPos(CurrentPos,TRUE);

	if((theApp.m_LastSettings.nImageOrientation==0) || (theApp.m_LastSettings.nImageOrientation==2)){
		m_prrSamples->top=CurrentPos;
	} else {
		m_prrSamples->top=m_pData->m_nNumberLines-1-(CurrentPos+m_prrSamples->Height());
	};
	MinMax(&m_prrSamples->top,0,m_pData->m_nNumberLines-1);

	m_bRenderView=TRUE;
	Invalidate(FALSE);	
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CVolumeViewDlg::SetScrollBarParameters()
{
	SCROLLINFO info;
	int	nHeight;

	m_rectZoom.bottom=RoundDownMulDiv((m_pData->m_nNumberLines-1),m_nVZoomScale,100)+m_rectZoom.top;
	if(m_rectZoom.bottom>=(m_pData->m_nNumberLines-1)) {
		nHeight=m_rectZoom.Height();
		m_rectZoom.bottom=m_pData->m_nNumberLines-1;
		m_rectZoom.top=m_rectZoom.bottom-nHeight;
	}
	m_scrollVCscanBar.SetScrollRange(0,m_pData->m_nNumberLines-1);
	m_scrollVCscanBar.SetScrollPos(m_rectZoom.top);
	m_scrollVCscanBar.GetScrollInfo(&info,SIF_ALL);
	info.nPage = m_rectZoom.Height();
	m_scrollVCscanBar.SetScrollInfo(&info,TRUE);

	m_scrollHCscanBar.GetScrollInfo(&info,SIF_ALL);
	info.nPage = m_rectZoom.Width();
	m_scrollHCscanBar.SetScrollInfo(&info,TRUE);


}

void CVolumeViewDlg::OnCbscanZoom10() 
{
	m_nVZoomScale=100;
	m_nZoomFactor=100;
	m_bRenderView=TRUE;
	Invalidate(FALSE);
}

void CVolumeViewDlg::OnCbscanZoom100() 
{
	m_nVZoomScale=1000;
	m_nZoomFactor=1000;
	m_bRenderView=TRUE;
	Invalidate(FALSE);
	
}

void CVolumeViewDlg::OnCbscanZoom25() 
{
	m_nVZoomScale=250;
	m_nZoomFactor=250;
	m_bRenderView=TRUE;
	Invalidate(FALSE);
	
}

void CVolumeViewDlg::OnCbscanZoom50() 
{
	m_nVZoomScale=500;
	m_nZoomFactor=500;
	m_bRenderView=TRUE;
	Invalidate(FALSE);
	
}


void CVolumeViewDlg::OnButtonZoomIn() 
{
	m_nZoomFactor-=100;
	if(m_nZoomFactor<100) m_nZoomFactor=100;
	m_bRenderView=TRUE;
	Invalidate(FALSE);
}

void CVolumeViewDlg::OnButtonZoomOut() 
{
	m_nZoomFactor+=100;
	if(m_nZoomFactor>1000) m_nZoomFactor=1000;
	m_bRenderView=TRUE;
	Invalidate(FALSE);
}


BOOL CVolumeViewDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CRect rr;
	int	nAction=0;
	CPoint	point;
	GetCursorPos(&point);

	if(m_rectDragBox.PtInRect(point)) {
		nAction=MOVE_ALL_BORDERS;
	}
	m_staticHBscan.GetWindowRect(rr);
	rr.left=rr.right;
	rr.right=rr.left + 16;
	if(rr.PtInRect(point)) {
		nAction=MOVE_TIMEBASE_DELAY_HBSCAN;
	}
	m_staticVBscan.GetWindowRect(rr);
	rr.top=rr.bottom;
	rr.bottom=rr.top + 16;
	if(rr.PtInRect(point)) {
		nAction=MOVE_TIMEBASE_DELAY_VBSCAN;
	}



	switch(nAction) {
	case MOVE_ALL_BORDERS: SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
		return TRUE;
		break;
	case MOVE_TIMEBASE_DELAY_HBSCAN:  SetCursor(m_hcursorVMove);
		return TRUE;
		break;
	case MOVE_TIMEBASE_DELAY_VBSCAN:  SetCursor(m_hcursorHMove);
		return TRUE;
		break;
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CVolumeViewDlg::CalculateDragBox()
{
	CRect rrH,rrV;
	
	if (m_scrollVCscanBar.GetSafeHwnd() && m_scrollHCscanBar.GetSafeHwnd()) {
		m_scrollVCscanBar.GetWindowRect(rrV);
		m_scrollHCscanBar.GetWindowRect(rrH);
		m_rectDragBox.left = rrH.right + 1;
		m_rectDragBox.right = rrV.right + 6;
		m_rectDragBox.top = rrV.bottom + 1;
		m_rectDragBox.bottom = rrH.bottom + 6;
	}
}

void CVolumeViewDlg::HBscanToWorld(CPoint pt, float *fX, float *fY)
{
	CRect rr;

	m_staticHBscan.GetWindowRect(rr);

	*fX = m_pData->m_fFastIncrement * (float)m_prrSamples->Width();
	*fY = (float)m_nWidthSamples * m_pData->m_TS[m_nTimeSlot].Adc.fSamplePeriod * (float)theApp.m_LastSettings.nMaterialVelocity / 2e6f;


	*fX = *fX * (float)(pt.x - rr.left) / (float)(rr.Width()-1) + ((float)m_prrSamples->left * m_pData->m_fFastIncrement);
	*fY = *fY * (float)(pt.y - rr.top) / (float)(rr.Height()-1);
}

void CVolumeViewDlg::DrawBScanScale(CPaintDC *pDC)
{
	int	nTextStep=500;
	float fT;
	int	pnx,pny,yy,xx;
	CString Buff;
	CSize TextSize;
	CFont HorizontalFont;
	CFont VerticalFont;
	CFont* pOldFont;
	CRect rrClient,rr;
	CBrush brushBK(GetSysColor(COLOR_BTNFACE));

	float fStep = 50.0f;
	if(m_fTimeBaseMM<=500.0f) fStep = 50.0f;
	if(m_fTimeBaseMM<=100.0f) fStep = 10.0f;
	if(m_fTimeBaseMM<=10.0f) fStep = 1.0f;
	if(m_fTimeBaseMM<=1.0f) fStep = 0.1f;

	theApp.m_LastSettings.lfViewGraticule.lfEscapement=0;
	HorizontalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);
	theApp.m_LastSettings.lfViewGraticule.lfEscapement=900;
	VerticalFont.CreateFontIndirect(&theApp.m_LastSettings.lfViewGraticule);

	pDC->SetBkMode(TRANSPARENT);

	pOldFont = pDC->SelectObject(&VerticalFont);
	Buff.Format(_T("999.99"));
	TextSize = pDC->GetTextExtent(Buff);

	//Clear right border of horizontal scan
	m_staticHBscan.GetWindowRect(rr);
	ScreenToClient(rr);
	rr.left=rr.right;
	rr.right=rr.left + 6 + TextSize.cy;
	pDC->FillRect(&rr,&brushBK);

	for(fT=0.0;fT<m_fTimeBaseMM;fT+=fStep) {
		pny = (int)((fT + m_fTimeBaseScaleDelay) * (float)rr.Height() / m_fTimeBaseMM) + rr.top;
		if(pny>=rr.top && pny<rr.bottom) {
			pDC->MoveTo(rr.left,pny);
			pDC->LineTo(rr.left+4,pny);
			Buff.Format(_T("%.01f"),fT);
			TextSize=pDC->GetTextExtent(Buff);
			yy = pny+TextSize.cx/2;
			if(yy - TextSize.cx < rr.top) yy = rr.top + TextSize.cx;
			if(yy > rr.bottom) yy = rr.bottom;
			pDC->TextOut(rr.left+4,yy,Buff);
		}
	};

	pDC->SelectObject(&HorizontalFont);
	Buff.Format(_T("999.99"));
	TextSize = pDC->GetTextExtent(Buff);

	//Clear right border of horizontal scan
	m_staticVBscan.GetWindowRect(rr);
	ScreenToClient(rr);
	rr.top=rr.bottom;
	rr.bottom=rr.top + 6 + TextSize.cy;
	pDC->FillRect(&rr,&brushBK);

	for(fT=0.0;fT<m_fTimeBaseMM;fT+=fStep) {
		pnx = (int)((fT + m_fTimeBaseScaleDelay) * (float)rr.Width() / m_fTimeBaseMM) + rr.left;
		if(pnx>=rr.left && pnx<rr.right) {
			pDC->MoveTo(pnx,rr.top);
			pDC->LineTo(pnx,rr.top+4);
			Buff.Format(_T("%.01f"),fT);
			TextSize=pDC->GetTextExtent(Buff);
			xx = pnx-TextSize.cx/2;
			if(xx < rr.left) xx = rr.left;
			if(xx + TextSize.cx > rr.right) xx = rr.right - TextSize.cx;
			pDC->TextOut(xx,rr.top+4,Buff);
		};
	};




	pDC->SetBkMode(OPAQUE);
	pDC->SelectObject(pOldFont);

}

void CVolumeViewDlg::OnFileImagesaveas() 
{
	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = m_pData->m_FileName;
	if((nPos = m_pData->m_FileName.ReverseFind('.')) >0) {
		InitialFile = "";
	}

//	theApp.SuspendThreads();
	CFileDialog FileDlg(FALSE,_T(".dat"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Image (*.dat)|*.dat|All Files (*.*)|*.*||"),NULL);
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
			int m_nNumberVolumeImages = m_pData->m_nNumberVolumeImages;
			m_pData->m_nNumberVolumeImages = 0;
			m_pData->Save(&File);
			m_pData->m_nNumberVolumeImages = m_nNumberVolumeImages;
			File.Close();
		}
	}
//	theApp.ResumeThreads();
	
}

BOOL CVolumeViewDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	theApp.m_Help.Open(_T("VolumeDlg.html"),NULL);
	
	return CDialog::OnHelpInfo(pHelpInfo);
}

void CVolumeViewDlg::OnProcessFirfilter() 
{
	if (m_pFIRDlg == NULL) {
		m_pFIRDlg = new CFIRDlg(this,(CWnd**)&m_pFIRDlg);
		m_pFIRDlg->Create(CFIRDlg::IDD, this);	
		theApp.m_LastSettings.bWindowOpenOnLastExit[CFIRDlg::IDD]=TRUE;
	} else {
		m_pFIRDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	
}

void CVolumeViewDlg::OnFileSavevolumedataasxml() 
{
	CUSLFile File;
	CFileException ExceptionInstance;

	CFileDialog FileDlg(FALSE,_T(".xml"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("XML Files (*.xml)|*.xml|All Files (*.*)|*.*||"),NULL);
	if(FileDlg.DoModal()==IDOK) {

		int flag = File.Open(FileDlg.GetPathName(), CFile::modeWrite | CFile::typeBinary | CFile::modeCreate, &ExceptionInstance);
		
		m_pData->SaveVolumeXMLAttempt0(&File,&m_File,m_nVolumeNumber);


		File.Close();
	}
	
}

void CVolumeViewDlg::OnButtonFullScreen() 
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRect rrMainFrm,rr,	rrWindow;

	int nHPixel = GetSystemMetrics(78);
	int nVPixel = GetSystemMetrics(79);
	int	nNumberMonitors = GetSystemMetrics(80);


	pFrame->GetClientRect(rrMainFrm);
	if(nNumberMonitors>1) {
		switch(theApp.m_LastSettings.nVolumeMonitor) {
		case 0: 
			if(pFrame->m_pwndToolBar[0]) {
				pFrame->m_pwndToolBar[0]->GetWindowRect(&rr);
				rrMainFrm.top = rr.bottom;
				rrMainFrm.bottom += rr.Height();
			}
			if(pFrame->m_wndStatusBar) {
				pFrame->m_wndStatusBar.GetWindowRect(&rr);
				rrMainFrm.bottom = rr.top;
			}
			break;
		case 1:
			rrMainFrm.top = 0;
			rrMainFrm.bottom = nVPixel;
			rrMainFrm.left = nHPixel / 2;
			rrMainFrm.right = nHPixel;
			break;
		case 2: 
			if(pFrame->m_pwndToolBar[0]) {
				pFrame->m_pwndToolBar[0]->GetWindowRect(&rr);
				rrMainFrm.top = rr.bottom;
				rrMainFrm.bottom += rr.Height();
			}
			if(pFrame->m_wndStatusBar) {
				pFrame->m_wndStatusBar.GetWindowRect(&rr);
				rrMainFrm.bottom = rr.top;
			}
			rrMainFrm.left = 2;
			rrMainFrm.right = nHPixel-4;
			break;
		}
	} else {
		if(pFrame->m_pwndToolBar[0]) {
			pFrame->m_pwndToolBar[0]->GetWindowRect(&rr);
			rrMainFrm.top = rr.bottom;
			rrMainFrm.bottom += rr.Height();
		}
	}

	rrWindow = rrMainFrm;

	SetWindowPos( &wndTop , rrWindow.left, rrWindow.top, rrWindow.Width(), rrWindow.Height(), NULL);

}

void CVolumeViewDlg::OnVolumePalette1() 
{
	if(m_bInCScan == true) m_nCScanPalette = 0;
	if(m_bInBScan == true) m_nBScanPalette = 0;
	m_bRenderView=TRUE;
	m_bPalette=TRUE;
	Invalidate(FALSE);
	
}

void CVolumeViewDlg::OnVolumePalette2() 
{
	if(m_bInCScan == true) m_nCScanPalette = 1;
	if(m_bInBScan == true) m_nBScanPalette = 1;
	m_bRenderView=TRUE;
	m_bPalette=TRUE;
	Invalidate(FALSE);
}

void CVolumeViewDlg::OnVolumePalette3() 
{
	if(m_bInCScan == true) m_nCScanPalette = 2;
	if(m_bInBScan == true) m_nBScanPalette = 2;
	m_bRenderView=TRUE;
	m_bPalette=TRUE;
	Invalidate(FALSE);
}

void CVolumeViewDlg::OnVolumePalette4() 
{
	if(m_bInCScan == true) m_nCScanPalette = 3;
	if(m_bInBScan == true) m_nBScanPalette = 3;
	m_bRenderView=TRUE;
	m_bPalette=TRUE;
	Invalidate(FALSE);
}

void CVolumeViewDlg::OnVolumePalette5() 
{
	if(m_bInCScan == true) m_nCScanPalette = 4;
	if(m_bInBScan == true) m_nBScanPalette = 4;
	m_bRenderView=TRUE;
	m_bPalette=TRUE;
	Invalidate(FALSE);
}

void CVolumeViewDlg::OnVolumePalette6() 
{
	if(m_bInCScan == true) m_nCScanPalette = 5;
	if(m_bInBScan == true) m_nBScanPalette = 5;
	m_bRenderView=TRUE;
	m_bPalette=TRUE;
	Invalidate(FALSE);
}

void CVolumeViewDlg::OnVolumePalette7() 
{
	if(m_bInCScan == true) m_nCScanPalette = 6;
	if(m_bInBScan == true) m_nBScanPalette = 6;
	m_bRenderView=TRUE;
	m_bPalette=TRUE;
	Invalidate(FALSE);
}

void CVolumeViewDlg::OnVolumePalette8() 
{
	if(m_bInCScan == true) m_nCScanPalette = 7;
	if(m_bInBScan == true) m_nBScanPalette = 7;
	m_bRenderView=TRUE;
	m_bPalette=TRUE;
	Invalidate(FALSE);
}

void CVolumeViewDlg::SetRulerText(CString Buff)
{
	CEdit* pEdit = (CEdit* ) m_wndDialogBar.GetDlgItem(IDC_EDIT_TEXT);
	pEdit->SetWindowText(Buff);

}

float CVolumeViewDlg::ComputeRulerStats(CString *pBuff)
{
	CCoord Cp0,Cp1,Cp;
	CPoint pt,pt0,pt1;
	CString Message,Width,Height,Length;;
	float fLength,fCircumferance,fAngle,fStep;
	int nTemp,nn;
	CViewSheet* pViewSheet = (CViewSheet*)m_pViewSheet;
	CString Buff;
	float fFastScanSize,fFastIncrement,fRadialLength,fTangentalLength;

	ClientToWorld(m_rrRuler.TopLeft(),&Cp0);
	ClientToWorld(m_rrRuler.BottomRight(),&Cp1);

	if(pViewSheet->m_nRectPolarDisplayMode == 0) {


		switch(theApp.m_LastSettings.nRulerTextMode) {
		default:
			break;
		case 1:
			fCircumferance = 2.0f * PIf * Cp0.Side0.fY;
			fAngle=fabs(Cp0.Side0.fR - Cp1.Side0.fR);
			fLength = fAngle * fCircumferance / 360.0f;
			theApp.FormatLength(fLength,1,Length);
			pBuff->Format(_T("Circumferential Length:%s"),Length);
			break;

		case 2:
			ClientToSample(m_rrRuler.BottomRight(),&pt0);
			ClientToSample(m_rrRuler.TopLeft(),&pt1);
			if(pt0.y>pt1.y) {
				nTemp = pt0.y;
				pt0.y=pt1.y;
				pt1.y=nTemp;
			}
			fLength = 0.0f;
			for(nn=pt0.y;nn<pt1.y;nn++) {
				Cp = theApp.m_Profile.m_ScanLine[0].m_pCp[nn];
				fStep = Cp.Length(theApp.m_Profile.m_ScanLine[0].m_pCp[nn+1]);
				fLength += fStep;
			}
			theApp.FormatLength(fLength,1,Length);
			pBuff->Format(_T("Tangental Length:%s"),Length);
			break;
		case 3:
			ClientToSample(m_rrRuler.BottomRight(),&pt0);
			ClientToSample(m_rrRuler.TopLeft(),&pt1);
			if(pt0.y>pt1.y) {
				nTemp = pt0.y;
				pt0.y=pt1.y;
				pt1.y=nTemp;
			}
			fLength = 0.0f;
			for(nn=pt0.y;nn<pt1.y;nn++) {
				Cp = theApp.m_Profile.m_ScanLine[0].m_pCp[nn];
				fStep = (float)Cp.Length(theApp.m_Profile.m_ScanLine[0].m_pCp[nn+1]);
				fLength += fStep;
			}
			fCircumferance = 2.0f * PIf * Cp0.Side0.fY;
			fAngle=fabs(Cp0.Side0.fR - Cp1.Side0.fR);
			fCircumferance = fAngle * fCircumferance / 360.0f;
			fLength = sqrt(fCircumferance*fCircumferance + fLength*fLength);
			theApp.FormatLength(fLength,1,Length);
			pBuff->Format(_T("Surface Length:%s"),Length);
			break;
		case 5:	//Circumferential angle
			fAngle=fabs(Cp0.Side0.fR - Cp1.Side0.fR);
			pBuff->Format(_T("Angle:%.02f%s"),fAngle,DEGREES);
			break;
		case 6://Circumferential distance
			m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);
			fAngle=fabs(Cp0.Side0.fR - Cp1.Side0.fR);
			fLength = fFastScanSize * fAngle / 360.0f;
			theApp.FormatLength(fLength,1,Length);
			pBuff->Format(_T("Length:%s"),Length);
			break;
		case 7:
			ClientToSample(m_rrRuler.BottomRight(),&pt0);
			pt0.y *= m_pData->m_nLineIncrement;
			pt0.y+=m_pData->m_nScanStartLine;
			ClientToSample(m_rrRuler.TopLeft(),&pt1);
			pt1.y *= m_pData->m_nLineIncrement;
			pt1.y+=m_pData->m_nScanStartLine;
			fLength = m_pData->m_fSlowScanSize / (float)m_pData->m_nNumberLines * (float)abs(pt0.y-pt1.y);
			theApp.FormatLength(fLength,1,Length);
			pBuff->Format(_T("Tangental Length:%s"),Length);
			break;
		case 8:
			m_pData->GetFastScanSizeIncrement(&fFastScanSize,&fFastIncrement,0);
			fAngle=fabs(Cp0.Side0.fR - Cp1.Side0.fR);
			fRadialLength = fFastScanSize * fAngle / 360.0f;

			ClientToSample(m_rrRuler.BottomRight(),&pt0);
			pt0.y *= m_pData->m_nLineIncrement;
			pt0.y+=m_pData->m_nScanStartLine;
			ClientToSample(m_rrRuler.TopLeft(),&pt1);
			pt1.y *= m_pData->m_nLineIncrement;
			pt1.y+=m_pData->m_nScanStartLine;
			fTangentalLength = m_pData->m_fSlowScanSize / (float)m_pData->m_nNumberLines * (float)abs(pt0.y-pt1.y);

			fLength = sqrtf(fTangentalLength * fTangentalLength + fRadialLength * fRadialLength);

			theApp.FormatLength(fLength,1,Length);
			pBuff->Format(_T("Length:%s"),Length);
			break;
		}
	}else {
		fLength = 0.0f;
		//Straight line
//		ClientToWorld(m_rrRuler.TopLeft(),&Cp0);
//		for(nn=1;nn<100;nn++) {
//			pt.x = RoundDownMulDiv(nn,m_rrRuler.right-m_rrRuler.left,99) + m_rrRuler.left;
//			pt.y = RoundDownMulDiv(nn,m_rrRuler.bottom-m_rrRuler.top,99) + m_rrRuler.top;
//			ClientToWorld(pt,&Cp);
//			fLength += Cp0.RadialLength(Cp,0);
//			Cp0=Cp;
//		}
//		theApp.FormatLength(fLength,1,Length);
//		pBuff->Format(_T("Length:%s"),Length);

			ClientToSample(m_rrRuler.BottomRight(),&pt0);
			ClientToSample(m_rrRuler.TopLeft(),&pt1);
			if(pt0.y>pt1.y) {
				nTemp = pt0.y;
				pt0.y=pt1.y;
				pt1.y=nTemp;
			}
			fLength = 0.0f;
			for(nn=pt0.y;nn<pt1.y;nn++) {
				if(nn>=0) {
					Cp = theApp.m_Profile.m_ScanLine[0].m_pCp[nn];
					fStep = Cp.Length(theApp.m_Profile.m_ScanLine[0].m_pCp[nn+1]);
					fLength += fStep;
				}
			}
			if(abs(Cp0.Side0.fY) > abs(Cp1.Side0.fY)) {
				fCircumferance = 2.0f * PIf * abs(Cp0.Side0.fY);
			} else {
				fCircumferance = 2.0f * PIf * abs(Cp1.Side0.fY);
			}
			fAngle=fabs(Cp0.Side0.fR - Cp1.Side0.fR);
			if(fAngle>180.0) {
				fAngle = 360.0f - fAngle;
			}
			fCircumferance = fAngle * fCircumferance / 360.0f;
			fLength = sqrt(fCircumferance*fCircumferance + fLength*fLength);
			theApp.FormatLength(fLength,1,Length);
			pBuff->Format(_T("Surface Length:%s"),Length);

	}


	return fLength;
}

void CVolumeViewDlg::ClientToSample(CPoint ptPixel,CPoint* pPtSample)
{
	switch(theApp.m_LastSettings.nImageOrientation) {
	case 0:
		pPtSample->x = RoundDownMulDiv(ptPixel.x - m_rrCscan.left,m_prrSamples->Width(),m_rrCscan.Width()-1) + m_prrSamples->left;
		pPtSample->y = RoundDownMulDiv(ptPixel.y - m_rrCscan.top,m_prrSamples->Height()-1,m_rrCscan.Height()-1) + m_prrSamples->top;
		break;
	case 1:
		pPtSample->x = RoundDownMulDiv(ptPixel.x - m_rrCscan.left,m_prrSamples->Width(),m_rrCscan.Width()-1) + m_prrSamples->left;
		pPtSample->y = RoundDownMulDiv(m_rrCscan.bottom - ptPixel.y,m_prrSamples->Height()-1,m_rrCscan.Height()-1) + m_prrSamples->top;
		break;
	case 2:
		pPtSample->x = RoundDownMulDiv(m_rrCscan.right - ptPixel.x,m_prrSamples->Width(),m_rrCscan.Width()-1) + m_prrSamples->left;
		pPtSample->y = RoundDownMulDiv(ptPixel.y - m_rrCscan.top,m_prrSamples->Height()-1,m_rrCscan.Height()-1) + m_prrSamples->top;
		break;
	case 3:
		pPtSample->x = RoundDownMulDiv(m_rrCscan.right - ptPixel.x,m_prrSamples->Width(),m_rrCscan.Width()-1) + m_prrSamples->left;
		pPtSample->y = RoundDownMulDiv(m_rrCscan.bottom - ptPixel.y,m_prrSamples->Height()-1,m_rrCscan.Height()-1) + m_prrSamples->top;
		break;
	}
}


void CVolumeViewDlg::ClientToSample(CPoint ptPixel,float *pfSample, float *pfLine)
{
	switch(theApp.m_LastSettings.nImageOrientation) {
	case 0:
		*pfSample =	(float)(ptPixel.x - m_rrCscan.left) * (float)m_prrSamples->Width() / (float)(m_rrCscan.Width()-1) + (float)m_prrSamples->left;
		*pfLine =	(float)(ptPixel.y - m_rrCscan.top) * (float)(m_prrSamples->Height()-1) / (float)(m_rrCscan.Height()-1) + (float)m_prrSamples->top;
		break;
	case 1:
		*pfSample = (float)(ptPixel.x - m_rrCscan.left) * (float)m_prrSamples->Width() / (float)(m_rrCscan.Width()-1) + (float)m_prrSamples->left;
		*pfLine = (float)(m_rrCscan.bottom - ptPixel.y) * (float)(m_prrSamples->Height()-1) / (float)(m_rrCscan.Height()-1) + (float)m_prrSamples->top;
		break;
	case 2:
		*pfSample = (float)(m_rrCscan.right - ptPixel.x) * (float)m_prrSamples->Width() / (float)(m_rrCscan.Width()-1) + (float)m_prrSamples->left;
		*pfLine = (float)(ptPixel.y - m_rrCscan.top) * (float)(m_prrSamples->Height()-1) / (float)(m_rrCscan.Height()-1) + (float)m_prrSamples->top;
		break;
	case 3:
		*pfSample = (float)(m_rrCscan.right - ptPixel.x) * (float)m_prrSamples->Width() / (float)(m_rrCscan.Width()-1) + (float)m_prrSamples->left;
		*pfLine = (float)(m_rrCscan.bottom - ptPixel.y) * (float)(m_prrSamples->Height()-1) / (float)(m_rrCscan.Height()-1) + (float)m_prrSamples->top;
		break;
	}
}

void CVolumeViewDlg::ClientToWorld(CPoint ptPixel,CCoord *pCp)
{
	CPoint ptSample;
	CCoord Cp;

	ClientToSample(ptPixel,&ptSample);
	RotoSampleToWorld(ptSample,pCp);

}

void CVolumeViewDlg::RotoSampleToWorld(CPoint ptSample,CCoord *pCp)
{
	CCoord Cp;

	MinMax(&ptSample.x,0,m_pData->m_nSamplesLine-1);
	MinMax(&ptSample.y,0,m_pData->m_nNumberLines-1);

	Cp = theApp.m_Profile.m_ScanLine[0].m_pCp[ptSample.y];
//	Cp =
	Cp.Side0.fR = (float)ptSample.x * 360.0f / (float)(m_pData->m_nSamplesLine - 1);

	CopyMemory(pCp,&Cp,sizeof Cp);


}

void CVolumeViewDlg::TextBoxAtCursor(CPaintDC *pDC, CString Buff)
{
	CRect rr,rrStatic;
	CPoint ptCursor;
	CPaintDC *pdc = pDC;

	if(pDC==NULL) {
		pdc = (CPaintDC*)GetDC();
	}

	m_staticView.GetWindowRect(rrStatic);
	ScreenToClient(rrStatic);

	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	pdc->SetBkColor(GetSysColor(COLOR_INFOBK));
	pdc->SetBkMode(OPAQUE);
	CSize size = pdc->GetTextExtent(Buff);

	if((ptCursor.x + size.cx) < rrStatic.right) {
		rr.left = ptCursor.x;
	} else {
		rr.left = ptCursor.x - size.cx;
	};

	if((ptCursor.y + size.cy) < rrStatic.bottom) {
		rr.top = ptCursor.y;
	} else {
		rr.top = ptCursor.y - size.cy;
	};

	rr.bottom = rr.top + size.cy+2;
	rr.right = rr.left + size.cx+2;

	pdc->Rectangle(rr);
	pdc->TextOut(rr.left+1,rr.top+1,Buff);

	if(pDC == NULL) {
		ReleaseDC(pdc);
	}

}


void CVolumeViewDlg::SaveCurrentTraceAsText(int nSample, int nLine, char *pTrace)
{
	CString FileName = theApp.m_LastSettingsPath + "\\LastTrace.txt";
	CFile File;
	CFileException e;
	CString Buff;


	if(!File.Open(FileName, CFile::modeCreate|CFile::typeBinary|CFile::modeReadWrite, &e)) {

	} else {

		File.Seek(0, CFile::begin);

		CArchive* pAr = new CArchive(&File,CArchive::store);
		Buff.Format(L"%d,%d",nSample, nLine);
		pAr->WriteString(Buff + L"\r\n");

		for(int ii=0;ii < m_nWidthSamples; ii++) {
			Buff.Format(L"%d\r\n",pTrace[ii]);
			pAr->WriteString(Buff);
		}
		pAr->Close();
		delete pAr;

		File.Close();
	}

}

void CVolumeViewDlg::SetWindowTitle()
{
	CString	Buff, Temp;
	int nPos;

	Buff = m_pData->m_FileName;
	if ((nPos = Buff.ReverseFind('.')) > -1) {
		Buff.GetBufferSetLength(nPos);
	}

	if (m_pData->m_bModified) {
		Buff += " (Not Saved)";
	}

	Temp.Format(_T(" Profile: %s"), m_pData->m_ProfileName);

	Buff += Temp;
	SetWindowText(Buff);
}
