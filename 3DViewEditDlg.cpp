// 3DViewEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "USLScanner.h"
#include "3DViewEditDlg.h"
#include "CopyCoordsDlg.h"
#include "EnvelopeToolsDlg.h"
#include "DecimateDlg.h"
#include "FileThumbNailDlg.h"
#include "TaughtToolsDlg.h"
#include "EditCoordDlg.h"
#include "TranslatDlg.h"
#include "MainFrm.h"
#include "DrawingToollsDlg.h"
#include "SineWaveDlg.h"
#include "ProfileSheet.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	BLUE3D	0xff0000ff
#define	GREEN3D	0xff00ff00
#define	RED3D	0xffff0000

const DWORD WorldVertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

/////////////////////////////////////////////////////////////////////////////
// C3DViewEditDlg dialog

IMPLEMENT_DYNCREATE(C3DViewEditDlg, CResizablePage)

C3DViewEditDlg::C3DViewEditDlg(int nToolBarStyles)	: CResizablePage(C3DViewEditDlg::IDD)
{
	//{{AFX_DATA_INIT(C3DViewEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	m_pDlg = pDlg;
	
	m_pEditSpinItem = NULL;
	m_nPaintMode = 0;
	m_pArray = NULL;

	m_hcursorFinger = AfxGetApp()->LoadCursor(IDC_FINGER_CURSOR);

	m_pBrainEditDlg = NULL;
	m_pHolesEditDlg = NULL;
	m_pScanLinesEditDlg = NULL;
	m_pTaughtLinesEditDlg = NULL;
	m_pEnvelopeLinesEditDlg = NULL;
	m_pWayLinesEditDlg = NULL;
	m_pTranslationDlg = NULL;
	m_pHoldingFixtureEditDlg = NULL;
	m_pWayPointsDlg = NULL;
	m_pNonCadVariablesDlg = NULL;
	m_ToolsPathDlg=NULL;
	m_pPolyCoordDlg= NULL;
	m_pTaughtTools = NULL;
	m_pBladeEditDlg = NULL;
	m_pCircumcenterDlg = NULL;

	theApp.m_nEditLineType = TAUGHTLINES;
	m_bSelectArea = FALSE;
	m_bDrawWayPts = FALSE;
	m_fZoomLevel = 1.0f;
	m_CpZoom.Zero();

	m_nIsometricImageNumber = 0;

	m_nToolBarStyles = nToolBarStyles;

	InitializeEditLine();

	m_bWireframe = FALSE;
	m_b3DCScan = FALSE;
	m_b3DDisplayScanLines = TRUE;
	m_bWallPicture = FALSE;

	m_nProjectionMoveOption = 0;
	m_n3DMoveEyeComponent = 0;
	m_bLBDown = FALSE;
	m_bRBDown = FALSE;
	m_bMBDown = FALSE;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		m_vEyePt = D3DXVECTOR3(500.0f, -theApp.m_Tank.fYLength, 500.0f );
		m_vLookatPt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		m_vUpVec = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		break;
	case SINGLE_BRIDGE_TT_ROLLERS:
	case SINGLE_BRIDGE_TANK_WITH_R:
	case SINGLE_BRIDGE_TANK:
	case DOUBLE_BRIDGE_TANK:
		m_vEyePt = D3DXVECTOR3(500.0f, -theApp.m_Tank.fYLength, 500.0f );
		m_vLookatPt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		m_vUpVec = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		break;
	}

	m_fRotatePtX = 0.0f;
	m_fRotatePtY = 0.0f;
	m_fRotatePtZ = 0.0f;

	m_bDisplayIsometric = FALSE;
	m_bDisplayFrame = TRUE;
	m_bDisplayDangerZone = TRUE;
	m_nNormal=0;
	m_nEditLinesEnvelope = 0;

	m_nMinLine=0;
	m_nMaxLine=0;

	theApp.m_LastSettings.f3DXSign = 1.0f;// -1.0;
	theApp.m_LastSettings.f3DYSign = 1.0f;
	theApp.m_LastSettings.f3DZSign = 1.0f;//-1.0f;

}

C3DViewEditDlg::~C3DViewEditDlg()
{

	SAFE_DELETE( m_pArray );
}

void C3DViewEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3DViewEditDlg)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	DDX_Control(pDX, IDC_LIST_COORDS, m_listCoord);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3DViewEditDlg, CResizablePage)
	//{{AFX_MSG_MAP(C3DViewEditDlg)
	ON_WM_CTLCOLOR()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_MESSAGE_VOID(WM_KICKIDLE, OnKickIdle)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_COORDS, OnClickListCoords)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COORDS, OnDblclkListCoords)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_COORDS, OnGetdispinfoListCoords)
	ON_EN_CHANGE(IDC_EDIT_LINE, OnChangeEditLine)
	ON_WM_VSCROLL()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_SCAN_LINES, OnEditScanlines)
	ON_COMMAND(ID_EDIT_ENVELOPE_LINES, OnEditenvelopelines)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_COORDS, OnRclickListCoords)
	ON_COMMAND(ID_LIST3D_GOTO_LEFT, OnList3dGotoLeft)
	ON_COMMAND(ID_LIST3D_GOTO_RIGHT, OnList3dGotoRight)
	ON_COMMAND(ID_LIST3D_GOTO_BOTH, OnList3dGotoBoth)
	ON_WM_MOVE()
	ON_COMMAND(ID_EDIT_BRAIN_LINES, OnEditBrainLines)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BRAIN_LINES, OnUpdateEditBrainLines)
	ON_COMMAND(ID_EDIT_HOLES, OnEditHoles)
	ON_UPDATE_COMMAND_UI(ID_EDIT_HOLES, OnUpdateEditHoles)
	ON_COMMAND(ID_TOOLS_COPY, OnToolsCopy)
	ON_COMMAND(ID_EDIT_TAUGHT_LINES, OnEditTaughtLines)
	ON_COMMAND(ID_LIST3D_DELETE, OnList3dDelete)
	ON_COMMAND(ID_TOOLS_ENVELOPE, OnToolsEnvelope)
	ON_COMMAND(ID_3DVIEW_DELETECOORDINATE, On3dviewDeletecoordinate)
	ON_COMMAND(ID_3DVIEW_DELETELINE, On3dviewDeleteline)
	ON_COMMAND(ID_3DVIEW_COPYLINE, On3dviewCopyline)
	ON_COMMAND(ID_3DVIEW_PASTELINE, On3dviewPasteline)
	ON_COMMAND(ID_3DVIEW_DECIMATELINE, On3dviewDecimateline)
	ON_COMMAND(ID_LIST3D_DECIMATE, OnList3dDecimate)
	ON_COMMAND(ID_EDIT_TRANSFORM, OnEditTransform)
	ON_COMMAND(ID_LIST3D_COPY, OnList3dCopy)
	ON_COMMAND(ID_LIST3D_PASTE, OnList3dPaste)
	ON_COMMAND(ID_EDIT_HOLDINGFIXTURE, OnEditHoldingfixture)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_TOOLS_INVERTALLNORMALS, OnToolsInvertallnormals)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_BUTTON_ADD_COORDINATE, OnButtonAddCoordinate)
	ON_COMMAND(ID_BUTTON_NEW_LINE, OnButtonNewLine)
	ON_COMMAND(ID_TOOLS_TAUGHTLINES, OnToolsTaughtlines)
	ON_COMMAND(ID_BUTTON_DELETE_COORDINATE, OnButtonDeleteCoordinate)
	ON_COMMAND(ID_LIST3D_EDIT, OnList3dEdit)
	ON_COMMAND(ID_LIST3D_TRANSLATE, OnList3dTranslate)
	ON_COMMAND(ID_LIST3D_MODIFYLEFT, OnList3dModifyleft)
	ON_COMMAND(ID_LIST3D_MODIFYRIGHT, OnList3dModifyright)
	ON_COMMAND(ID_LIST3D_MODIFYBOTH, OnList3dModifyboth)
	ON_COMMAND(ID_ENVELOPE_TRANSLATE, OnEnvelopeTranslate)
	ON_COMMAND(ID_LIST3D_BISECT, OnList3dBisect)
	ON_COMMAND(ID_HOLES_STORE, OnHolesStore)
	ON_COMMAND(ID_HOLES_GOTO, OnHolesGoto)
	ON_COMMAND(ID_HOLES_SWAP0, OnHolesSwap0)
	ON_COMMAND(ID_HOLES_SWAP1, OnHolesSwap1)
	ON_COMMAND(ID_HOLES_INVERTNORMAL, OnHolesInvertnormal)
	ON_COMMAND(ID_BRAIN_MODIFYDELTAS, OnBrainModifydeltas)
	ON_COMMAND(ID_BRAIN_GOTO_NO_BRAIN, OnBrainGotoNoBrain)
	ON_COMMAND(ID_BRAIN_GOTOBRAIN, OnBrainGotobrain)
	ON_WM_TIMER()
	ON_COMMAND(ID_LIST3D_DELETELINE, OnList3dDeleteline)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_VIEW_IGES, OnViewIges)
	ON_COMMAND(ID_VIEW_LINES, OnViewLines)
	ON_COMMAND(ID_VIEW_CROSSES, OnViewCrosses)
	ON_COMMAND(ID_BUTTON_SELECTAREA, OnButtonSelectarea)
	ON_COMMAND(ID_AREA_DELETE, OnAreaDelete)
	ON_COMMAND(ID_AREA_DELETEALLSCANLINES, OnAreaDeleteallscanlines)
	ON_COMMAND(ID_ENVELOPE_PASTE, OnEnvelopePaste)
	ON_COMMAND(ID_3DVIEW_DECIMATEALLLINES, On3dviewDecimatealllines)
	ON_COMMAND(ID_3DVIEW_SORTALLLINES, On3dviewSortalllines)
	ON_COMMAND(ID_BRAIN_RESETALLDELTAS, OnBrainResetalldeltas)
	ON_COMMAND(ID_VIEW_ZOOM_IN, OnViewZoomIn)
	ON_COMMAND(ID_VIEW_ZOOM_OUT, OnViewZoomOut)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_BRAIN_RESETLEFTDELTAS, OnBrainResetleftdeltas)
	ON_COMMAND(ID_BRAIN_RESETRIGHTDELTAS, OnBrainResetrightdeltas)
	ON_COMMAND(ID_BRAIN_COPY, OnBrainCopy)
	ON_COMMAND(ID_BRAIN_PASTE, OnBrainPaste)
	ON_COMMAND(ID_BRAIN_RESETIJK, OnBrainResetijk)
	ON_COMMAND(ID_LIST3D_CALCULATEXTANGLE, OnList3dCalculatextangle)
	ON_COMMAND(ID_LIST3D_STRAITELINE, OnList3dStraiteline)
	ON_COMMAND(ID_TOOLS_SINEWAVEGENERATOR, OnToolsSinewavegenerator)
	ON_COMMAND(ID_LIST3D_MODIFYIJK, OnList3dModifyijk)
	ON_COMMAND(ID_LIST3D_OPTOMISE, OnList3dOptomise)
	ON_COMMAND(ID_BRAIN_ADDTOBRAINLINE, OnBrainAddtobrainline)
	ON_COMMAND(ID_3DVIEW_REORDERLINES, On3dviewReorderlines)
	ON_COMMAND(ID_3DVIEW_3DLOOKATTHISPOINT, On3dview3dlookatthispoint)
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_HELPINFO()
	ON_COMMAND(ID_TOOLS_WAYPOINTS, OnToolsWaypoints)
	ON_COMMAND(ID_EDIT_WAYPOINTS, OnEditWaypoints)
	ON_COMMAND(ID_TOOLS_UNTWIST, OnToolsUntwist)
	ON_COMMAND(ID_HEADER_RAXIS, OnHeaderRaxis)
	ON_COMMAND(ID_HEADER_ENDOFLINE, OnHeaderEndofline)
	ON_COMMAND(ID_HEADER_GAIN, OnHeaderGain)
	ON_COMMAND(ID_HEADER_WATERPATHLEFT, OnHeaderWaterpathleft)
	ON_COMMAND(ID_HEADER_WATERPATHRIGHT, OnHeaderWaterpathright)
	ON_COMMAND(ID_HEADER_INTERFACEDELAY, OnHeaderInterfacedelay)
	ON_COMMAND(ID_HEADER_INTERFACEWIDTH, OnHeaderInterfacewidth)
	ON_COMMAND(ID_HEADER_GATE1DELAY, OnHeaderGate1delay)
	ON_COMMAND(ID_HEADER_GATE1WIDTH, OnHeaderGate1width)
	ON_COMMAND(ID_HEADER_GATE2DELAY, OnHeaderGate2delay)
	ON_COMMAND(ID_HEADER_GATE2WIDTH, OnHeaderGate2width)
	ON_COMMAND(ID_TANK_MODIFYUT, OnTankModifyut)
	ON_COMMAND(ID_LIST3D_NONCADVARIABLES, OnList3dNoncadvariables)
	ON_COMMAND(ID_BRAIN_DELETE, OnBrainDelete)
	ON_COMMAND(ID_TANK_MODIFYXYZIJK, OnTankModifyxyzijk)
	ON_COMMAND(ID_BRAIN_NEWBRANELINE, OnBrainNewbraneline)
	ON_COMMAND(ID_BRAIN_SORTBRANELINES, OnBrainSortbranelines)
	ON_COMMAND(ID_BRAIN_PASTELINE, OnBrainPasteline)
	ON_COMMAND(ID_VIEW_RTHETAPATH, OnViewRThetaPath)
	ON_COMMAND(ID_HOLES_GOTOTIPANGLES, OnHolesGototipangles)
	ON_COMMAND(ID_BRAIN_DELETELINE, OnBrainDeleteLine)
	ON_COMMAND(ID_LIST3D_DIVIDEINTO2LINES, OnList3dDivideinto2lines)
	ON_COMMAND(ID_LIST3D_EXPANDLINE, OnList3dExpandline)
	ON_COMMAND(ID_ENVELOPE_BISECT, OnEnvelopeBisect)
	ON_COMMAND(ID_SURFACE_SENDTOTAUGHTLINES, OnSurfaceSendtotaughtlines)
	ON_COMMAND(ID_SURFACE_COPYLINE, OnSurfaceCopyline)
	ON_COMMAND(ID_EDIT_SURFACE_LINES, OnEditSurfaceLines)
	ON_COMMAND(ID_3DVIEW_MOVELASERTOTHISPOINT, On3dviewMovelasertothispoint)
	ON_COMMAND(ID_BUTTON_SURFACE_GENERATOR, OnButtonSurfaceGenerator)
	ON_COMMAND(ID_LIST3D_DELETEPRECEDINGPTS, OnDeletePrecedingPoints)
	ON_COMMAND(ID_LIST3D_DELETEFOLLOWINGPTS, OnDeleteFollowingPoints)
	ON_COMMAND(ID_HOLES_COPYALLCADMEASURED, OnHolesCopyallcadmeasured)
	ON_COMMAND(ID_HOLES_PASTEALLCADMEASURED, OnHolesPasteallcadmeasured)
	ON_COMMAND(ID_BRAIN_SENDTOSCANLINES, OnBrainSendtoscanlines)
	ON_COMMAND(ID_BRAIN_SENDALLTOSCANLINES, OnBrainSendalltoscanlines)
	ON_COMMAND(ID_HOLES_3DLOOKATTHISPOINT, OnHoles3dlookatthispoint)
	ON_COMMAND(ID_VIEW_XY, OnViewXy)
	ON_COMMAND(ID_VIEW_XZ, OnViewXz)
	ON_COMMAND(ID_VIEW_YZ, OnViewYz)
	ON_COMMAND(ID_3DVIEW_SORT_X, On3dviewSortX)
	ON_COMMAND(ID_3DVIEW_SORT_Y, On3dviewSortY)
	ON_COMMAND(ID_3DVIEW_SORT_Z, On3dviewSortZ)
	ON_COMMAND(ID_BUTTON_EDIT_INSIDE, OnButtonEditInside)
	ON_COMMAND(ID_BUTTON_EDIT_OUTSIDE, OnButtonEditOutside)
	ON_COMMAND(ID_3DVIEW_BISECTADDLINEBETWEEN, On3dviewBisectaddlinebetween)
	ON_COMMAND(ID_LIST3D_CIRCUMCENTERBETWEEN, OnList3dCircumcenterbetween)
	ON_COMMAND(ID_ENVELOPE_ADD, OnEnvelopeAdd)
	ON_COMMAND(ID_ENVELOPE_DELETE, OnEnvelopeDelete)
	ON_COMMAND(ID_ENVELOPE_MOVE, OnEnvelopeMove)
	ON_COMMAND(ID_ENVELOPE_TABORDER, OnEnvelopeTaborder)
	ON_COMMAND(ID_HOLES_IMPORTCADPTSFROMTEXTFILE, OnHolesImportcadptsfromtextfile)
	ON_COMMAND(ID_HOLES_GUESSCADNORMS, OnHolesGuesscadnorms)
	ON_COMMAND(ID_HOLES_GOTO_VIADANGERPLANE, OnHolesGotoViadangerplane)
	ON_COMMAND(ID_LIST3D_MODIFYNORMALSONNEAREST, OnList3dModifynormalsonnearest)
	ON_COMMAND(ID_LIST3D_PASTENORMALS, OnList3dPastenormals)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_COORDS, OnKeydownListCoords)
	ON_COMMAND(ID_3DVIEW_COMPUTEOPPOSITESIDE, On3dviewComputeoppositeside)
	ON_COMMAND(ID_3DVIEW_DELETEPRECEDINGPTS, OnDeletePrecedingPoints)
	ON_COMMAND(ID_3DVIEW_DELETEFOLLOWINGPTS, OnDeleteFollowingPoints)
	ON_COMMAND(ID_TANK_DELETEPRECEDINGPTS, OnDeletePrecedingPoints)
	ON_COMMAND(ID_TANK_DELETEFOLLOWINGPTS, OnDeleteFollowingPoints)	
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_3DVIEW_ALL_COPYALLLINES, On3dviewAllCopyalllines)
	ON_COMMAND(ID_3DVIEW_ALL_PASTEALLLINES, On3dviewAllPastealllines)
	ON_COMMAND(ID_3DVIEW_ALL_DELETEALLLINES, On3dviewAllDeletealllines)
	ON_COMMAND(ID_3DVIEW_ALL_DECIMATEALLLINES, On3dviewAllDecimatealllines)
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_ITEMCHANGED,ListChanged)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_MESSAGE(UI_ITEMCHANGED,ItemChanged)
	ON_MESSAGE(WM_IDLEUPDATECMDUI, SetToolBarCheckedState)
	ON_MESSAGE(UI_3D_FILLLIST, FillListFromChild)
		ON_WM_KEYDOWN()
		ON_COMMAND(ID_3DVIEW_RECENTERIMAGE, &C3DViewEditDlg::On3dviewRecenterimage)
		ON_COMMAND(ID_3DVIEW_PROPERTIES, &C3DViewEditDlg::On3dviewProperties)
		ON_UPDATE_COMMAND_UI(ID_VIEW_LINES, &C3DViewEditDlg::OnUpdateViewLines)
		ON_UPDATE_COMMAND_UI(ID_VIEW_CROSSES, &C3DViewEditDlg::OnUpdateViewCrosses)
		END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DViewEditDlg message handlers
/*
int C3DViewEditDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CRect temp;
	CSize size;
	int nRebarWidth = 0;

	if (CResizablePage::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndFileToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndFileToolBar.LoadToolBar(IDR_FILE_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndFileToolBar.GetToolBarCtrl().GetMaxSize(&size);
	nRebarWidth += size.cx;


	if (!m_wndViewToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndViewToolBar.LoadToolBar(IDR_PROFILE_VIEW_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndViewToolBar.GetToolBarCtrl().GetMaxSize(&size);
	nRebarWidth += size.cx;

	if (!m_wndEditToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndEditToolBar.LoadToolBar(IDR_PROFILE_EDIT_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndEditToolBar.GetToolBarCtrl().GetMaxSize(&size);
	nRebarWidth += size.cx;

	if (!m_wndCoordinateToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC,
		WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndCoordinateToolBar.LoadToolBar(IDR_PROFILE_COORDINATE_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndCoordinateToolBar.GetToolBarCtrl().GetMaxSize(&size);
	nRebarWidth += size.cx;

	if(theApp.m_LastSettings.nToolBar[CARTESIAN_BLADE_TOOLBAR]) {
		if (!m_wndBladeToolBar.CreateEx(this,
			TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC,
			WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS  | CBRS_FLYBY ,
			CRect(0,2,0,0)) ||	!m_wndBladeToolBar.LoadToolBar(IDR_BLADE_EDIT_TOOLBAR)) {
				TRACE0("Failed to create toolbar\n");
			return -1;      // fail to create
		}
		m_wndBladeToolBar.GetToolBarCtrl().GetMaxSize(&size);
		nRebarWidth += size.cx;
	}

	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	if(m_wndFileToolBar.m_hWnd) m_wndRebar.AddBar(&m_wndFileToolBar);
	if(m_wndViewToolBar.m_hWnd) m_wndRebar.AddBar(&m_wndViewToolBar);
	if(m_wndCoordinateToolBar.m_hWnd) m_wndRebar.AddBar(&m_wndCoordinateToolBar);
	if(m_wndEditToolBar.m_hWnd) m_wndRebar.AddBar(&m_wndEditToolBar);
	if(m_wndBladeToolBar.m_hWnd) m_wndRebar.AddBar(&m_wndBladeToolBar);

	m_wndRebar.SetWindowPos(&wndTop, 0, 0, nRebarWidth + 80, 30, SWP_SHOWWINDOW);

	SetWindowTitle();

	return 0;
}
*/

int C3DViewEditDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CResizablePage::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndFileToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_FILE_TOOLBAR);
	m_wndFileToolBar.LoadToolBar(IDR_FILE_TOOLBAR, 0, 0, TRUE);
		//	m_wndFileToolBar.SetPaneStyle(m_wndFileToolBar.GetPaneStyle() | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY);
		//	m_wndFileToolBar.SetPaneStyle(m_wndFileToolBar.GetPaneStyle() & ~(CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndFileToolBar.SetOwner(this);
	m_wndFileToolBar.SetRouteCommandsViaFrame(false);

	m_wndViewToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROFILE_VIEW_TOOLBAR);
	m_wndViewToolBar.LoadToolBar(IDR_PROFILE_VIEW_TOOLBAR, 0, 0, TRUE /* Is locked */);
	m_wndViewToolBar.SetPaneStyle(m_wndViewToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndViewToolBar.SetPaneStyle(m_wndViewToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndViewToolBar.SetOwner(this);
	m_wndViewToolBar.SetRouteCommandsViaFrame(false);

	m_wndEditToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROFILE_EDIT_TOOLBAR);
	m_wndEditToolBar.LoadToolBar(IDR_PROFILE_EDIT_TOOLBAR, 0, 0, TRUE /* Is locked */);
	m_wndEditToolBar.SetPaneStyle(m_wndEditToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndEditToolBar.SetPaneStyle(m_wndEditToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndEditToolBar.SetOwner(this);
	m_wndEditToolBar.SetRouteCommandsViaFrame(false);

	m_wndCoordinateToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROFILE_COORDINATE_TOOLBAR);
	m_wndCoordinateToolBar.LoadToolBar(IDR_PROFILE_COORDINATE_TOOLBAR, 0, 0, TRUE /* Is locked */);
	m_wndCoordinateToolBar.SetPaneStyle(m_wndCoordinateToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndCoordinateToolBar.SetPaneStyle(m_wndCoordinateToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndCoordinateToolBar.SetOwner(this);
	m_wndCoordinateToolBar.SetRouteCommandsViaFrame(false);

	m_wndBladeToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_BLADE_EDIT_TOOLBAR);
	m_wndBladeToolBar.LoadToolBar(IDR_BLADE_EDIT_TOOLBAR, 0, 0, TRUE /* Is locked */);
	m_wndBladeToolBar.SetPaneStyle(m_wndBladeToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndBladeToolBar.SetPaneStyle(m_wndBladeToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndBladeToolBar.SetOwner(this);
	m_wndBladeToolBar.SetRouteCommandsViaFrame(false);

	CRect rrClient,rr;
	GetClientRect(rrClient);

	int cyTlb = m_wndFileToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndFileToolBar.SetWindowPos(NULL, rrClient.left, rrClient.top, m_wndFileToolBar.CalcFixedLayout(FALSE, TRUE).cx, cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);							m_wndFileToolBar.GetWindowRect(rr);
	m_wndViewToolBar.SetWindowPos(NULL, rr.right, rrClient.top, m_wndViewToolBar.CalcFixedLayout(FALSE, TRUE).cx, cyTlb, SWP_NOACTIVATE | SWP_NOZORDER | SWP_SHOWWINDOW);				m_wndViewToolBar.GetWindowRect(rr);
	m_wndCoordinateToolBar.SetWindowPos(NULL, rr.right, rrClient.top, m_wndCoordinateToolBar.CalcFixedLayout(FALSE, TRUE).cx, cyTlb, SWP_NOACTIVATE | SWP_NOZORDER | SWP_SHOWWINDOW);	m_wndCoordinateToolBar.GetWindowRect(rr);
	m_wndEditToolBar.SetWindowPos(NULL, rr.right, rrClient.top, m_wndEditToolBar.CalcFixedLayout(FALSE, TRUE).cx, cyTlb, SWP_NOACTIVATE | SWP_NOZORDER | SWP_SHOWWINDOW);				m_wndEditToolBar.GetWindowRect(rr);

/*
	if (theApp.m_LastSettings.nToolBar[CARTESIAN_BLADE_TOOLBAR]) {
		if (!m_wndBladeToolBar.CreateEx(this,
			TBSTYLE_FLAT | TBSTYLE_WRAPABLE | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC,
			WS_CHILD | WS_VISIBLE | CBRS_LEFT | CBRS_TOOLTIPS | CBRS_FLYBY,
			CRect(0, 2, 0, 0)) || !m_wndBladeToolBar.LoadToolBar(IDR_BLADE_EDIT_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
			return -1;      // fail to create
		}
		m_wndBladeToolBar.GetToolBarCtrl().GetMaxSize(&size);
		nRebarWidth += size.cx;
	}

	if (!m_wndRebar.Create(this, RBS_BANDBORDERS, CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	if (m_wndFileToolBar.m_hWnd) m_wndRebar.AddBar(&m_wndFileToolBar);
	if (m_wndViewToolBar.m_hWnd) m_wndRebar.AddBar(&m_wndViewToolBar);
	if (m_wndCoordinateToolBar.m_hWnd) m_wndRebar.AddBar(&m_wndCoordinateToolBar);
	if (m_wndEditToolBar.m_hWnd) m_wndRebar.AddBar(&m_wndEditToolBar);
	if (m_wndBladeToolBar.m_hWnd) m_wndRebar.AddBar(&m_wndBladeToolBar);

	m_wndRebar.SetWindowPos(&wndTop, 0, 0, nRebarWidth + 80, 30, SWP_SHOWWINDOW);
	*/
	SetWindowTitle();

	return 0;
}

BOOL C3DViewEditDlg::OnInitDialog()
{
	CResizablePage::OnInitDialog();
	CString	Buff;

	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);
	m_StaticPosList.Initialize(this, m_listCoord, FALSE, TRUE, TRUE, TRUE);


	if (C3DViewEditDlg::IDD < MAXIMUMWINDOWSIDNUMBER) {
		CRect rect = theApp.m_LastSettings.RectWindowPos[C3DViewEditDlg::IDD];
		if (theApp.m_LastSettings.bWindowPos&&!rect.IsRectEmpty())
			SetWindowPos( &wndTop , rect.left, rect.top, rect.Width(), rect.Height(), NULL);
	}
	m_StaticPosView.SetNewPos();
	m_StaticPosList.SetNewPos();

	

	OpenOnLastExit();
	UpdateAllControls();
	CreateColumns();
	FillList();


	SetAccessPrivelage();
	m_hBr = CreateSolidBrush(GetSysColor(COLOR_MENU)); // <= create on initDialog

	SetTimer(1,500,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


HBRUSH C3DViewEditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor) {
	default: return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	case 4: return m_hBr;
		break;
	}

	return m_hBr;
}

void C3DViewEditDlg::OpenOnLastExit()
{
	switch(theApp.m_nEditLineType) {
	case TAUGHTLINES: OnEditTaughtLines();
		break;
	case SCANLINES: OnEditTaughtLines();
		break;
	case BRAINLINES: OnEditTaughtLines();
		break;
	case SURFACELINES: OnEditSurfaceLines();
		break;
	case ENVELOPELINES: OnEditenvelopelines();
		break;
	case WAYLINES: OnEditWaypoints();
		break;
	case OUTSIDELINES: OnButtonEditOutside();
		break;
	case INSIDELINES: OnButtonEditInside();
		break;
	}

}

void C3DViewEditDlg::OnKickIdle()
{
	UpdateDialogControls(this, FALSE);
}
void C3DViewEditDlg::Move(int x, int y)
{
	OnMove(x, y);
}


void C3DViewEditDlg::OnMove(int x, int y)
{
	if (GetSafeHwnd() == NULL) return;
	CResizablePage::OnMove(x, y);

	CRect rr,rrReBar;
	GetClientRect(rr);
	ClientToScreen(rr);
//	m_wndRebar.GetWindowRect(rrReBar);
	m_wndFileToolBar.GetWindowRect(rrReBar);

	if(m_pBrainEditDlg) m_pBrainEditDlg->SetWindowPos(NULL,rr.left,rr.top+rrReBar.Height(),0,0,SWP_NOSIZE);
	if(m_pHolesEditDlg) m_pHolesEditDlg->SetWindowPos(NULL,rr.left,rr.top+rrReBar.Height(),0,0,SWP_NOSIZE);
	if(m_pScanLinesEditDlg) m_pScanLinesEditDlg->SetWindowPos(NULL,rr.left,rr.top+rrReBar.Height(),0,0,SWP_NOSIZE);
	if(m_pTaughtLinesEditDlg) m_pTaughtLinesEditDlg->SetWindowPos(NULL,rr.left,rr.top+rrReBar.Height(),0,0,SWP_NOSIZE);
	if(m_pEnvelopeLinesEditDlg) m_pEnvelopeLinesEditDlg->SetWindowPos(NULL,rr.left,rr.top+rrReBar.Height(),0,0,SWP_NOSIZE);
	if(m_pTranslationDlg) m_pTranslationDlg->SetWindowPos(NULL,rr.left,rr.top+rrReBar.Height(),0,0,SWP_NOSIZE);
	if(m_pHoldingFixtureEditDlg) m_pHoldingFixtureEditDlg->SetWindowPos(NULL,rr.left,rr.top+rrReBar.Height(),0,0,SWP_NOSIZE);
	if(m_pWayLinesEditDlg) m_pWayLinesEditDlg->SetWindowPos(NULL,rr.left,rr.top+rrReBar.Height(),0,0,SWP_NOSIZE);
	if(m_pBladeEditDlg) m_pBladeEditDlg->SetWindowPos(NULL,rr.left,rr.top+rrReBar.Height(),0,0,SWP_NOSIZE);
}

void C3DViewEditDlg::CloseAllChildren()
{
	if(m_pBrainEditDlg) m_pBrainEditDlg->SendMessage(WM_CLOSE);
	if(m_pHolesEditDlg) m_pHolesEditDlg->SendMessage(WM_CLOSE);
	if(m_pScanLinesEditDlg) m_pScanLinesEditDlg->SendMessage(WM_CLOSE);
	if(m_pTaughtLinesEditDlg) m_pTaughtLinesEditDlg->SendMessage(WM_CLOSE);
	if(m_pEnvelopeLinesEditDlg) m_pEnvelopeLinesEditDlg->SendMessage(WM_CLOSE);
	if(m_pTranslationDlg) m_pTranslationDlg->SendMessage(WM_CLOSE);
	if(m_pHoldingFixtureEditDlg) m_pHoldingFixtureEditDlg->SendMessage(WM_CLOSE);
	if(m_pWayLinesEditDlg) m_pWayLinesEditDlg->SendMessage(WM_CLOSE);
	if(m_pBladeEditDlg) m_pBladeEditDlg->SendMessage(WM_CLOSE);

}

void C3DViewEditDlg::InvalidateAllChildren()
{
	if(m_pBrainEditDlg) m_pBrainEditDlg->Invalidate(FALSE);
	if(m_pHolesEditDlg) m_pHolesEditDlg->Invalidate(FALSE);
	if(m_pScanLinesEditDlg) m_pScanLinesEditDlg->Invalidate(FALSE);
	if(m_pTaughtLinesEditDlg) m_pTaughtLinesEditDlg->Invalidate(FALSE);
	if(m_pEnvelopeLinesEditDlg) m_pEnvelopeLinesEditDlg->Invalidate(FALSE);
	if(m_pTranslationDlg) m_pTranslationDlg->Invalidate(FALSE);
	if(m_pHoldingFixtureEditDlg) m_pHoldingFixtureEditDlg->Invalidate(FALSE);
	if(m_pWayLinesEditDlg) m_pWayLinesEditDlg->Invalidate(FALSE);
	if(m_pBladeEditDlg) m_pBladeEditDlg->Invalidate(FALSE);

}


void C3DViewEditDlg::UpdateAllControls()
{
	CString Buff;

//	if(m_pNormalsEditDlg) m_pNormalsEditDlg->UpdateAllControls();
	if(m_pBrainEditDlg) m_pBrainEditDlg->UpdateAllControls();
	if(m_pHolesEditDlg) m_pHolesEditDlg->UpdateAllControls();
	if(m_pScanLinesEditDlg) m_pScanLinesEditDlg->UpdateAllControls();
	if(m_pTaughtLinesEditDlg) m_pTaughtLinesEditDlg->UpdateAllControls();
	if(m_pEnvelopeLinesEditDlg) m_pEnvelopeLinesEditDlg->UpdateAllControls();
	if(m_pTranslationDlg) m_pTranslationDlg->UpdateAllControls();
	if(m_pHoldingFixtureEditDlg) m_pHoldingFixtureEditDlg->UpdateAllControls();
	if(m_ToolsPathDlg) m_ToolsPathDlg->CalculateFromScanLine(theApp.m_nLine);
//	if(m_pWayLinesEditDlg) m_pWayLinesEditDlg->UpdateAllControls();
	if(m_pBladeEditDlg) m_pBladeEditDlg->UpdateAllControls();

/*
	CMenu* pMenu = GetMenu();
	CMenu* pPopup = pMenu->GetSubMenu(0);

	if(theApp.m_nAccessLevel>=theApp.m_cAccess[ID_PROFILES_SAVEAS]) {
		pPopup->EnableMenuItem( ID_FILE_SAVE_AS   , MF_BYCOMMAND);
	} else {
		pPopup->EnableMenuItem( ID_FILE_SAVE_AS   , MF_GRAYED|MF_BYCOMMAND);
	}
	*/
}

void C3DViewEditDlg::OnSize(UINT nType, int cx, int cy)
{
	CResizablePage::OnSize(nType, cx, cy);
	CRect rr;

	if(GetSafeHwnd()) {
		m_StaticPosView.SetNewPos();
		m_StaticPosList.SetNewPos();

		Centerimage();

		Invalidate(false);
	}

}

void C3DViewEditDlg::CreateColumns()
{
	CString ColumnNameWrist[27] = { L"#",L"X", L"Y", L"Z", L"i",L"j",L"k",L"X", L"Y", L"Z", L"i",L"j",L"k",L"R",L"EOL",L"Wp0",L"Wp1",L"Gain",L"I'Delay",L"I'Width",L"Delay1",L"Width1",L"Delay2",L"Width2"};
	CString ColumnNameHoles[12] = { L"#",L"X", L"Y", L"Z", L"i",L"j",L"k",L"EOL",L"Vect L'gth"};
	CString ColumnNameFinger[12] = { L"#",L"X", L"Y", L"Z", L"I",L"J",L"K",L"EOL", L"ZTip"};
	CString ColumnNameBrain[18] = { L"#",L"X", L"Y", L"Z",L"R",L"I",L"J",L"K",L"EOL",L"x", L"y", L"z", L"i",L"j",L"k",L"x", L"y", L"z"};
	CString ColumnNameBlade[18] = { L"#",L"X", L"Y", L"Z",L"I",L"J",L"K",L"R",L"TL",L"Min Thick'", L"Nom Thick", L"Max Thick"};
	int ii;

	int ColumnWidth[30] = { 20,60,60,60,60,60,60,60,60,60,60,60,60,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	int nColumnCount = (&m_listCoord.GetHeaderCtrl())->GetItemCount();
	for (ii=0;ii < nColumnCount;ii++) {
		m_listCoord.DeleteColumn(0);
	}
	switch(theApp.m_nEditLineType) {
	default:
		for (ii=0;ii<13;ii++) m_listCoord.InsertColumn(ii,ColumnNameWrist[ii], LVCFMT_CENTER,ColumnWidth[ii]);
		if(PROFILE->m_nHeader[H_RAXIS]==1) ColumnWidth[ii++]=60;
		if(PROFILE->m_nHeader[H_EOL]==1) ColumnWidth[ii++]=60;
		if(PROFILE->m_nHeader[H_WP0]==1) ColumnWidth[ii++]=60;
		if(PROFILE->m_nHeader[H_WP1]==1) ColumnWidth[ii++]=60;
		if(PROFILE->m_nHeader[H_GAIN0]==1) ColumnWidth[ii++]=60;
		if(PROFILE->m_nHeader[H_DELAY0]==1) ColumnWidth[ii++]=60;
		if(PROFILE->m_nHeader[H_WIDTH0]==1) ColumnWidth[ii++]=60;
		if(PROFILE->m_nHeader[H_DELAY1]==1) ColumnWidth[ii++]=60;
		if(PROFILE->m_nHeader[H_WIDTH1]==1) ColumnWidth[ii++]=60;
		if(PROFILE->m_nHeader[H_DELAY2]==1) ColumnWidth[ii++]=60;
		if(PROFILE->m_nHeader[H_WIDTH2]==1) ColumnWidth[ii++]=60;
		nColumnCount = ii;
		for (ii=13;ii<nColumnCount;ii++) m_listCoord.InsertColumn(ii,ColumnNameWrist[ii], LVCFMT_CENTER,ColumnWidth[ii]);
		break;
	case BRAINLINES:
		for (ii=0;ii<18;ii++) {
			if(ColumnWidth[ii] == 0) ColumnWidth[ii] = 60;
			m_listCoord.InsertColumn(ii,ColumnNameBrain[ii], LVCFMT_CENTER,ColumnWidth[ii]);
		}
		break;
	case HOLES:
		for (ii=0;ii<9;ii++) {
			if(ColumnWidth[ii] == 0) ColumnWidth[ii] = 60;
			m_listCoord.InsertColumn(ii,ColumnNameHoles[ii], LVCFMT_CENTER,ColumnWidth[ii]);
		}
		break;
	case TRANSFORM:
	case NORMALS:
	case DETAILS:
	case HOLDINGFIXTURE:
		break;
	case OUTSIDELINES:
	case INSIDELINES:
		for (ii=0;ii<12;ii++)
			m_listCoord.InsertColumn(ii,ColumnNameBlade[ii], LVCFMT_CENTER,ColumnWidth[ii]);
		break;

	}
	m_listCoord.SetExtendedStyle( LVS_EX_FULLROWSELECT   );

}

long C3DViewEditDlg::FillListFromChild(unsigned int nT, long nL)
{
	FillList();
	return 0;
}

void C3DViewEditDlg::FillList()
{
	CString Buff,Which;
	int	ii;
	int nCoordL = 0;

	InitializeEditLine();

	int nCount = m_listCoord.GetItemCount();
	switch(theApp.m_nEditLineType) {
	case TAUGHTLINES:
	case SCANLINES:
	case BRAINLINES:
	case ENVELOPELINES:
	case SURFACELINES:
	case WAYLINES:
	case OUTSIDELINES:
	case INSIDELINES:
		if(m_pEditLine) nCoordL = m_pEditLine[theApp.m_nLine].m_nCoordL;

		if(nCount<nCoordL) {
			for (int ii=nCount;ii<nCoordL;ii++) {
				Buff.Format(_T("%d"),ii+1);
				m_listCoord.InsertItem(ii, Buff.GetBuffer(255), ii);
			};
		} else {
			for(ii=nCount-1;ii>=nCoordL;ii--) {
				m_listCoord.DeleteItem(ii);
			}
		}
		m_listCoord.EnsureVisible(theApp.m_nIndex,FALSE);
		for(ii=0;ii<nCoordL;ii++)
			m_listCoord.SetItemState(ii , 0,LVIS_SELECTED);
		m_listCoord.SetItemState(theApp.m_nIndex , LVIS_SELECTED,LVIS_SELECTED);
		break;
	case HOLES:
		m_listCoord.DeleteAllItems();
		Which.LoadString(IDS_Measured);
		m_listCoord.InsertItem(0, Which + " " + theApp.m_3PtName[0], 0);
		m_listCoord.InsertItem(1, Which + " " + theApp.m_3PtName[1], 1);
		m_listCoord.InsertItem(2, Which + " " + theApp.m_3PtName[2], 2);

		Which.LoadString(IDS_CAD);
		m_listCoord.InsertItem(3, Which + " " + theApp.m_3PtName[0], 3);
		m_listCoord.InsertItem(4, Which + " " + theApp.m_3PtName[1], 4);
		m_listCoord.InsertItem(5, Which + " " + theApp.m_3PtName[2], 5);
		m_listCoord.SetColumnWidth(0,120);
		break;
	case TRANSFORM:
	case NORMALS:
	case DETAILS:
	case HOLDINGFIXTURE:
		break;
	}

}

void C3DViewEditDlg::OnClickListCoords(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	SAFE_DELETE( m_pEditSpinItem );
	theApp.m_nIndex = pDispInfo->item.mask;
	Invalidate(FALSE);

	*pResult = 0;
}

void C3DViewEditDlg::OnDblclkListCoords(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CRect	Rect;
	CRect	CtrlRect;
	CString	Type[2],EolScanOption[2];
	CCoord* pCp = NULL;

	Type[0].LoadString(IDS_Surface);
	Type[1].LoadString(IDS_Ball);
	EolScanOption[0].LoadString(IDS_Moveto);
	EolScanOption[1].LoadString(IDS_Via_Danger_Plane);

	m_listCoord.GetWindowRect(CtrlRect);
	m_listCoord.GetSubItemRect(pDispInfo->item.mask,pDispInfo->item.iItem,LVIR_BOUNDS,Rect);
	Rect.OffsetRect(CtrlRect.left,CtrlRect.top);
	Rect.InflateRect(0,1,0,2);

	if(m_pEditSpinItem!=NULL) delete m_pEditSpinItem;
	m_pEditSpinItem = new CEditSpinItem(this,(CWnd**)&m_pEditSpinItem,&Rect);


	switch(theApp.m_nEditLineType) {
	case TAUGHTLINES:
	case SCANLINES:
	case BRAINLINES:
	case ENVELOPELINES:
	case OUTSIDELINES:
	case INSIDELINES:

		theApp.m_nIndex = pDispInfo->item.mask;
		if(m_pEditLine==NULL) return;
		pCp = (CCoord*)&m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex];

		break;
	case HOLES:
		switch(pDispInfo->item.mask) {
		case 0:
		case 1:
		case 2: pCp = (CCoord*)&PROFILE->m_CpMeasuredHole[pDispInfo->item.mask];

			break;
		case 3:
		case 4:
		case 5: pCp = (CCoord*)&PROFILE->m_CpCadHole[pDispInfo->item.mask-3];
			break;
		}
		break;
	}

	if(pCp) {
		switch(theApp.m_nEditLineType) {
		default:
			switch(pDispInfo->item.iItem) {
			case 1:
				m_fEditInitialValue = pCp->Side0.fX;
				m_nEditAxis = 0;
				m_pEditSpinItem->Initialize(&pCp->Side0.fX, -10000.0f, 10000.0f, 1.0f, _T("%.01f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 2:
				m_fEditInitialValue = pCp->Side0.fY;
				m_nEditAxis = 1;
				m_pEditSpinItem->Initialize(&pCp->Side0.fY, -10000.0f, 10000.0f, 1.0f, _T("%.01f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 3:
				m_fEditInitialValue = pCp->Side0.fZ;
				m_nEditAxis = 2;
				m_pEditSpinItem->Initialize(&pCp->Side0.fZ, -10000.0f, 10000.0f, 1.0f, _T("%.01f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 4:
				m_fEditInitialValue = pCp->Side0.fI;
				m_nEditAxis = 6;
				m_pEditSpinItem->Initialize(&pCp->Side0.fI, -1.0f, 1.0f, 0.01f, _T("%.02f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 5:
				m_fEditInitialValue = pCp->Side0.fJ;
				m_nEditAxis = 7;
				m_pEditSpinItem->Initialize(&pCp->Side0.fJ, -1.0f, 1.0f, 0.01f, _T("%.02f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 6:
				m_fEditInitialValue = pCp->Side0.fK;
				m_nEditAxis = 8;
				m_pEditSpinItem->Initialize(&pCp->Side0.fK, -1.0f, 1.0f, 0.01f, _T("%.02f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 7:
				switch (theApp.m_nEditLineType) {
				default:
					m_fEditInitialValue = pCp->Side1.fX;
					m_nEditAxis = 9;
					m_pEditSpinItem->Initialize(&pCp->Side1.fX, -10000.0f, 10000.0f, 1.0f, _T("%.01f"));
					m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
					break;
				case HOLES:
					m_pEditSpinItem->Initialize(&pCp->nType, Type, 2);
					m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
					break;
				case OUTSIDELINES:
				case INSIDELINES:
					break;
				}

				break;
			case 8:
				m_fEditInitialValue = pCp->Side1.fY;
				m_nEditAxis = 10;
				m_pEditSpinItem->Initialize(&pCp->Side1.fY, -10000.0f, 10000.0f, 1.0f, _T("%.01f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 9:
				m_fEditInitialValue = pCp->Side1.fZ;
				m_nEditAxis = 11;
				m_pEditSpinItem->Initialize(&pCp->Side1.fZ, -10000.0f, 10000.0f, 1.0f, _T("%.01f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 10:
				m_fEditInitialValue = pCp->Side1.fI;
				m_nEditAxis = 12;
				m_pEditSpinItem->Initialize(&pCp->Side1.fI, -1.0f, 1.0f, 0.01f, _T("%.02f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 11:
				m_fEditInitialValue = pCp->Side1.fJ;
				m_nEditAxis = 13;
				m_pEditSpinItem->Initialize(&pCp->Side1.fJ, -1.0f, 1.0f, 0.01f, _T("%.02f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 12:
				m_fEditInitialValue = pCp->Side1.fK;
				m_nEditAxis = 14;
				m_pEditSpinItem->Initialize(&pCp->Side1.fK, -1.0f, 1.0f, 0.01f, _T("%.02f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 13:
				m_fEditInitialValue = pCp->Side0.fR;
				m_nEditAxis=15;
				m_pEditSpinItem->Initialize(&pCp->Side0.fR,-360.0f,360.0f,0.1f,_T("%.01f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 14:
				switch(theApp.m_nEditLineType) {
				default:
					if(theApp.m_nIndex==0) {
						m_pEditSpinItem->Initialize(&m_pEditLine[theApp.m_nLine].m_fExtrapolate[0],0.0f,500.0f,0.1f,_T("%.01f"));
						m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
					}
					if(theApp.m_nIndex==(m_pEditLine[theApp.m_nLine].m_nCoordL-1)) {
						m_pEditSpinItem->Initialize(&m_pEditLine[theApp.m_nLine].m_fExtrapolate[1],0.0f,500.0f,0.1f,_T("%.01f"));
						m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
					}
					break;
				case SCANLINES:
					if(theApp.m_nIndex==0 || theApp.m_nIndex==(m_pEditLine[theApp.m_nLine].m_nCoordL-1)) {
						m_pEditSpinItem->Initialize(&pCp->nType,EolScanOption,2);
						m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
					}
					break;
				case HOLES:
					m_pEditSpinItem->Initialize(&pCp->nType,Type,2);
					m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
					break;
				}
				break;
			case 15:
				switch(theApp.m_nEditLineType) {
				default:
					m_fEditInitialValue = pCp->fWp[0];
					m_nEditAxis=pDispInfo->item.iItem;
					m_pEditSpinItem->Initialize(&m_fEditInitialValue,0.0f,10000.0f,1.0f,_T("%.01f"));
					m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
					break;
				case OUTSIDE:
				case INSIDE:
					break;
				}
				break;
			case 16:
				switch(theApp.m_nEditLineType) {
				default:
					m_fEditInitialValue = pCp->fWp[0];
					m_nEditAxis=pDispInfo->item.iItem;
					m_pEditSpinItem->Initialize(&m_fEditInitialValue,0.0f,10000.0f,1.0f,_T("%.01f"));
					m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
					break;
				case OUTSIDE:
				case INSIDE:
					break;
				}
				break;
			case 17:
				switch(theApp.m_nEditLineType) {
				default:
					m_fEditInitialValue = (float)pCp->nGain[0]/10.0f;
					m_nEditAxis=pDispInfo->item.iItem;
					m_pEditSpinItem->Initialize(&m_fEditInitialValue,0.0f,10000.0f,1.0f,_T("%.01f"));
					m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
					break;
				case OUTSIDE:
				case INSIDE:
					break;
				}
				break;
			case 18:
				switch(theApp.m_nEditLineType) {
				default:
					m_fEditInitialValue = (float)pCp->nDelay[0] / 1000.0f;
					m_nEditAxis=pDispInfo->item.iItem;
					m_pEditSpinItem->Initialize(&m_fEditInitialValue,0.0f,10000.0f,1.0f,_T("%.01f"));
					m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
					break;
				case OUTSIDE:
				case INSIDE:
					break;
				}
				break;
			case 19:
				switch(theApp.m_nEditLineType) {
				default:
					m_fEditInitialValue = (float)pCp->nWidth[0] / 1000.0f;
					m_nEditAxis=pDispInfo->item.iItem;
					m_pEditSpinItem->Initialize(&m_fEditInitialValue,0.0f,10000.0f,1.0f,_T("%.01f"));
					m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
					break;
				case OUTSIDE:
				case INSIDE:
					break;
				}
				break;
			case 20:
				switch(theApp.m_nEditLineType) {
				default:
					m_fEditInitialValue = (float)pCp->nDelay[1] / 1000.0f;
					m_nEditAxis=pDispInfo->item.iItem;
					m_pEditSpinItem->Initialize(&m_fEditInitialValue,0.0f,10000.0f,1.0f,_T("%.01f"));
					m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
					break;
				case OUTSIDE:
				case INSIDE:
					break;
				}
				break;
			case 21:
				switch(theApp.m_nEditLineType) {
				default:
					m_fEditInitialValue = (float)pCp->nWidth[1] / 1000.0f;
					m_nEditAxis=pDispInfo->item.iItem;
					m_pEditSpinItem->Initialize(&m_fEditInitialValue,0.0f,10000.0f,1.0f,_T("%.01f"));
					m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
					break;
				case OUTSIDE:
				case INSIDE:
					break;
				}
				break;
			case 22:
				switch(theApp.m_nEditLineType) {
				default:
					m_fEditInitialValue = (float)pCp->nDelay[2] / 1000.0f;
					m_nEditAxis=pDispInfo->item.iItem;
					m_pEditSpinItem->Initialize(&m_fEditInitialValue,0.0f,10000.0f,1.0f,_T("%.01f"));
					m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
					break;
				case OUTSIDE:
				case INSIDE:
					break;
				}
				break;
			case 23:
				switch(theApp.m_nEditLineType) {
				default:
					m_fEditInitialValue = (float)pCp->nWidth[2] / 1000.0f;
					m_nEditAxis=pDispInfo->item.iItem;
					m_pEditSpinItem->Initialize(&m_fEditInitialValue,0.0f,10000.0f,1.0f,_T("%.01f"));
					m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
					break;
				case OUTSIDE:
				case INSIDE:
					break;
				}
				break;
			}
			break;
		case BRAINLINES:
			m_nEditAxis=-1;
			switch(pDispInfo->item.iItem) {
			case 9:
				m_pEditSpinItem->Initialize(&pCp->Delta.fX0,-100.0f,100.0f,0.1f,_T("%.01f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 10:
				m_pEditSpinItem->Initialize(&pCp->Delta.fY0,-100.0f,100.0f,0.1f,_T("%.01f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 11:
				m_pEditSpinItem->Initialize(&pCp->Delta.fZ0,-100.0f,100.0f,0.1f,_T("%.01f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 12:
				m_pEditSpinItem->Initialize(&pCp->Delta.fI,-1.0f,1.0f,0.01f,_T("%.02f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 13:
				m_pEditSpinItem->Initialize(&pCp->Delta.fJ,-1.0f,1.0f,0.01f,_T("%.02f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 14:
				m_pEditSpinItem->Initialize(&pCp->Delta.fK,-1.0f,1.0f,0.01f,_T("%.02f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 15:
				m_pEditSpinItem->Initialize(&pCp->Delta.fX1,-100.0f,100.0f,0.1f,_T("%.01f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 16:
				m_pEditSpinItem->Initialize(&pCp->Delta.fY1,-100.0f,100.0f,0.1f,_T("%.01f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			case 17:
				m_pEditSpinItem->Initialize(&pCp->Delta.fZ1,-100.0f,100.0f,0.1f,_T("%.01f"));
				m_pEditSpinItem->Create(IDD_DIALOG_LIST_EDIT, this);
				break;
			}
			break;
		}
	}

	*pResult = 0;
}

long C3DViewEditDlg::ItemChanged(unsigned int nTemp,long nlTemp)
{
	if(m_pEditLine==NULL) return 0;
	switch(m_nEditAxis) {
	case 0:	m_fEditInitialValue = m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side[0].fX;
		break;
	case 1:	m_fEditInitialValue = m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side[0].fY;
		break;
	case 2:	m_fEditInitialValue = m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side[0].fZ;
		break;
	case 9:	m_fEditInitialValue = m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side[1].fX;
		break;
	case 10:	m_fEditInitialValue = m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side[1].fY;
		break;
	case 11:	m_fEditInitialValue = m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side[1].fZ;
		break;
	case 15: m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].fWp[0] = m_fEditInitialValue;
		break;
	case 16: m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].fWp[1] = m_fEditInitialValue;
		break;
	case 17: m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].nGain[0] = (int)(m_fEditInitialValue * 10.0f);
		break;
	case 18: m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].nDelay[0] = (int)(m_fEditInitialValue * 1000.0f);
		break;
	case 19: m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].nDelay[1] = (int)(m_fEditInitialValue * 1000.0f);
		break;
	case 20: m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].nDelay[2] = (int)(m_fEditInitialValue * 1000.0f);
		break;
	case 21: m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].nWidth[0] = (int)(m_fEditInitialValue * 1000.0f);
		break;
	case 22: m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].nWidth[1] = (int)(m_fEditInitialValue * 1000.0f);
		break;
	case 23: m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].nWidth[2] = (int)(m_fEditInitialValue * 1000.0f);
		break;
	}

	PROFILE->m_bModified = TRUE;

	Invalidate(FALSE);
	return 0;
}

void C3DViewEditDlg::OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	static	WCHAR str[100];

	str[0]=0;
	pDispInfo->item.pszText=str;

	switch(theApp.m_nEditLineType) {
	case TAUGHTLINES:
	case SCANLINES:
	case ENVELOPELINES:
	case SURFACELINES:
	case WAYLINES:
		GetDispInfoListCoordsEditLines(pDispInfo,str);
		break;
	case BRAINLINES:
		GetDispInfoListCoordsBrainLines(pDispInfo,str);
		break;
	case HOLES:
		GetDispInfoListHoles(pDispInfo,str);
		break;
	case OUTSIDELINES:
	case INSIDELINES:
		GetDispInfoListCoordsBladeLines(pDispInfo,str);
		break;
	}

	*pResult = 0;
}


void C3DViewEditDlg::GetDispInfoListCoordsEditLines(LV_DISPINFO* pDispInfo,WCHAR *str)
{
	CCoord* pCp;

	if((m_pEditLine==NULL) || (m_nEditLineL<=0)) {
		str[0]=0;
		return;
	}
	if(theApp.m_nLine>=m_nEditLineL) return;
	if(m_pEditLine[theApp.m_nLine].m_nCoordL<pDispInfo->item.iItem) return;

	pCp = (CCoord*)&m_pEditLine[theApp.m_nLine].m_pCp[pDispInfo->item.iItem];
/*
	pCp->Side1.fX = pCp->Side0.fX;
	pCp->Side1.fY = pCp->Side0.fY;
	pCp->Side1.fZ = pCp->Side0.fZ;
	pCp->Side1.fXt = pCp->Side0.fXt;
	pCp->Side1.fYt = pCp->Side0.fYt;
	pCp->Side1.fR = pCp->Side0.fR;
	pCp->Side1.fI = pCp->Side0.fI;
	pCp->Side1.fJ = pCp->Side0.fJ;
	pCp->Side1.fK = pCp->Side0.fK;
*/
	if(pCp == NULL) {
		str[0]=0;
		return;
	}

	switch(pDispInfo->item.iSubItem) {
	case 0:	swprintf_s(str,20,_T("%d"),pDispInfo->item.iItem+1);
		break;
	case 1:	swprintf_s(str, 20, _T("%.01f"), pCp->Side[PORTSIDE].fX);
		break;
	case 2:	swprintf_s(str, 20, _T("%.01f"), pCp->Side[PORTSIDE].fY);
		break;
	case 3:	swprintf_s(str, 20, _T("%.01f"), pCp->Side[PORTSIDE].fZ);
		break;
	case 4:	swprintf_s(str, 20, _T("%.04f"), pCp->Side[PORTSIDE].fI);
		break;
	case 5:	swprintf_s(str, 20, _T("%.04f"), pCp->Side[PORTSIDE].fJ);
		break;
	case 6:	swprintf_s(str, 20, _T("%.04f"), pCp->Side[PORTSIDE].fK);
		break;
	case 7:	swprintf_s(str, 20, _T("%.01f"), pCp->Side[STARBOARD].fX);
		break;
	case 8:	swprintf_s(str, 20, _T("%.01f"), pCp->Side[STARBOARD].fY);
		break;
	case 9:	swprintf_s(str, 20, _T("%.01f"), pCp->Side[STARBOARD].fZ);
		break;
	case 10:	swprintf_s(str, 20, _T("%.04f"), pCp->Side[STARBOARD].fI);
		break;
	case 11:	swprintf_s(str, 20, _T("%.04f"), pCp->Side[STARBOARD].fJ);
		break;
	case 12:	swprintf_s(str, 20, _T("%.04f"), pCp->Side[STARBOARD].fK);
		break;
	case 13:	swprintf_s(str, 20,_T("%.04f"),pCp->Side[PORTSIDE].fR);
		break;
	case 14:
		str[0]=0;
		if(m_pEditLine) {
			switch(theApp.m_nEditLineType) {
			default:
				if(pDispInfo->item.iItem==0) {
					if(m_pEditLine[theApp.m_nLine].m_fExtrapolate[0]==0.0f) {
						swprintf_s(str, 20,_T("First coord"));
					} else {
						swprintf_s(str, 20,_T("%.01f"),m_pEditLine[theApp.m_nLine].m_fExtrapolate[0]);
					}
				}
				if(pDispInfo->item.iItem==m_pEditLine[theApp.m_nLine].m_nCoordL-1) {
					if(m_pEditLine[theApp.m_nLine].m_fExtrapolate[1]==0.0f) {
						swprintf_s(str, 20,_T("Last coord"));
					} else {
						swprintf_s(str, 20,_T("%.01f"),m_pEditLine[theApp.m_nLine].m_fExtrapolate[1]);
					}
				}
				break;
			case SCANLINES:
				if(pDispInfo->item.iItem==m_pEditLine[theApp.m_nLine].m_nCoordL-1 || pDispInfo->item.iItem == 0) {
					if(pCp->nType == 1) {
						swprintf_s(str, 20,L"Via danger plane");
					} else {
						swprintf_s(str, 20,L"Move to");
					}
				};
				break;
			}
		}
		break;
	case 15:	swprintf_s(str, 20,_T("%.02f"),pCp->fWp[0]);
		break;
	case 16:	swprintf_s(str, 20,_T("%.02f"),pCp->fWp[1]);
		break;
	case 17:	swprintf_s(str, 20,_T("%.01f"),(float)pCp->nGain[0]/10.0f);
		break;
	case 18:	swprintf_s(str, 20,_T("%.02f"),(float)pCp->nDelay[0]/1000.0f);
		break;
	case 19:	swprintf_s(str, 20,_T("%.02f"),(float)pCp->nWidth[0]/1000.0f);
		break;
	case 20:	swprintf_s(str, 20,_T("%.02f"),(float)pCp->nDelay[1]/1000.0f);
		break;
	case 21:	swprintf_s(str, 20,_T("%.02f"),(float)pCp->nWidth[1]/1000.0f);
		break;
	case 22:	swprintf_s(str, 20,_T("%.02f"),(float)pCp->nDelay[1]/1000.0f);
		break;
	case 23:	swprintf_s(str, 20,_T("%.02f"),(float)pCp->nWidth[1]/1000.0f);
		break;
	}

}

void C3DViewEditDlg::GetDispInfoListCoordsBrainLines(LV_DISPINFO* pDispInfo,WCHAR *str)
{
	CCoord* pCp;

	m_pEditLine = PROFILE->m_BrainLine;
	m_nEditLineL = PROFILE->m_nBrainLineL;
	if((m_pEditLine==NULL) || (m_nEditLineL<=0)) {
		str[0]=0;
		return;
	}
	pCp = (CCoord*)&m_pEditLine[theApp.m_nLine].m_pCp[pDispInfo->item.iItem];

	switch(pDispInfo->item.iSubItem) {
	case 0:	swprintf_s(str, 20,_T("%d"),pDispInfo->item.iItem+1);
		break;
	case 1:	swprintf_s(str, 20,_T("%.01f"),pCp->Side[PORTSIDE].fX);
		break;
	case 2:	swprintf_s(str, 20,_T("%.01f"),pCp->Side[PORTSIDE].fY);
		break;
	case 3:	swprintf_s(str, 20,_T("%.01f"),pCp->Side[PORTSIDE].fZ);
		break;
	case 4: swprintf_s(str, 20,_T("%.01f"),pCp->Side[PORTSIDE].fR);
		break;
	case 5:	swprintf_s(str, 20,_T("%.03f"),pCp->Side[PORTSIDE].fI);
		break;
	case 6:	swprintf_s(str, 20,_T("%.03f"),pCp->Side[PORTSIDE].fJ);
		break;
	case 7:	swprintf_s(str, 20,_T("%.03f"),pCp->Side[PORTSIDE].fK);
		break;
	case 8:
		str[0]=0;
		if(m_pEditLine) {
			if(pDispInfo->item.iItem==0) {
				if(m_pEditLine[theApp.m_nLine].m_fExtrapolate[0]==0.0f) {
					swprintf_s(str, 20,_T("First coord"));
				} else {
					swprintf_s(str, 20,_T("%.01f"),m_pEditLine[theApp.m_nLine].m_fExtrapolate[0]);
				}
			}
			if(pDispInfo->item.iItem==m_pEditLine[theApp.m_nLine].m_nCoordL-1) {
				if(m_pEditLine[theApp.m_nLine].m_fExtrapolate[1]==0.0f) {
					swprintf_s(str, 20,_T("Last coord"));
				} else {
					swprintf_s(str, 20,_T("%.01f"),m_pEditLine[theApp.m_nLine].m_fExtrapolate[1]);
				}
			}
		}
		break;
	case 9:	swprintf_s(str, 20,_T("%.01f"),pCp->Delta.fX0);
		break;
	case 10:	swprintf_s(str, 20,_T("%.01f"),pCp->Delta.fY0);
		break;
	case 11:	swprintf_s(str, 20,_T("%.01f"),pCp->Delta.fZ0);
		break;
	case 12:	swprintf_s(str, 20,_T("%.03f"),pCp->Delta.fI);
		break;
	case 13:	swprintf_s(str, 20,_T("%.03f"),pCp->Delta.fJ);
		break;
	case 14:	swprintf_s(str, 20,_T("%.03f"),pCp->Delta.fK);
		break;
	case 15:	swprintf_s(str, 20,_T("%.01f"),pCp->Delta.fX1);
		break;
	case 16:	swprintf_s(str, 20,_T("%.01f"),pCp->Delta.fY1);
		break;
	case 17:	swprintf_s(str, 20,_T("%.01f"),pCp->Delta.fZ1);
		break;
	}

}

void C3DViewEditDlg::GetDispInfoListHoles(LV_DISPINFO* pDispInfo,WCHAR *str)
{
	CCoord* pCp = NULL;
	static CCoord CpMeasuredCentre[3];
	static CCoord CpCadCentre[3];
	float fLength;
	CString Type[2];

	Type[0].LoadString(IDS_Surface);
	Type[1].LoadString(IDS_Ball);

	switch(pDispInfo->item.iItem) {
	case 0:
		if(PROFILE->m_CpCadHole[0].nType) {
			theApp.m_Kinematics.CalculateBallCenterFromSurface((CCoord)PROFILE->m_CpMeasuredHole[0],&CpMeasuredCentre[0]);
			pCp = &CpMeasuredCentre[0];
		} else {
			pCp = (CCoord*)&PROFILE->m_CpMeasuredHole[0];
			CpMeasuredCentre[0] = PROFILE->m_CpMeasuredHole[0];
		}
		break;
	case 1:
		if(PROFILE->m_CpCadHole[1].nType) {
			theApp.m_Kinematics.CalculateBallCenterFromSurface((CCoord)PROFILE->m_CpMeasuredHole[1],&CpMeasuredCentre[1]);
			pCp = &CpMeasuredCentre[1];
		} else {
			pCp = (CCoord*)&PROFILE->m_CpMeasuredHole[1];
			CpMeasuredCentre[1] = PROFILE->m_CpMeasuredHole[1];
		}
		break;
	case 2:
		if(PROFILE->m_CpCadHole[2].nType) {
			theApp.m_Kinematics.CalculateBallCenterFromSurface((CCoord)PROFILE->m_CpMeasuredHole[2],&CpMeasuredCentre[2]);
			pCp = &CpMeasuredCentre[2];
		} else {
			pCp = (CCoord*)&PROFILE->m_CpMeasuredHole[2];
			CpMeasuredCentre[2] = PROFILE->m_CpMeasuredHole[2];
		}
		break;

	case 3:
		if(PROFILE->m_CpCadHole[0].nType) {
			theApp.m_Kinematics.CalculateBallCenterFromHole((CCoord)PROFILE->m_CpCadHole[0],&CpCadCentre[0]);
			pCp = &CpCadCentre[0];
		} else {
			pCp = (CCoord*)&PROFILE->m_CpCadHole[0];
			CpCadCentre[0] = PROFILE->m_CpCadHole[0];
		}
		break;
	case 4:
		if(PROFILE->m_CpCadHole[1].nType) {
			theApp.m_Kinematics.CalculateBallCenterFromHole((CCoord)PROFILE->m_CpCadHole[1],&CpCadCentre[1]);
			pCp = &CpCadCentre[1];
		} else {
			pCp = (CCoord*)&PROFILE->m_CpCadHole[1];
			CpCadCentre[1] = PROFILE->m_CpCadHole[1];
		}
		break;
	case 5:
		if(PROFILE->m_CpCadHole[2].nType) {
			theApp.m_Kinematics.CalculateBallCenterFromHole((CCoord)PROFILE->m_CpCadHole[2],&CpCadCentre[2]);
			pCp = &CpCadCentre[2];
		} else {
			pCp = (CCoord*)&PROFILE->m_CpCadHole[2];
			CpCadCentre[2] = PROFILE->m_CpCadHole[2];
		}
		break;
	}
	if (pCp == NULL) return;

	switch(pDispInfo->item.iSubItem) {
	case 0:	swprintf_s(str, 20,_T("%d"),pDispInfo->item.iItem+1);
		break;
	case 1:	swprintf_s(str, 20,_T("%.01f"),pCp->Side[PORTSIDE].fX);
		break;
	case 2:	swprintf_s(str, 20,_T("%.01f"),pCp->Side[PORTSIDE].fY);
		break;
	case 3:	swprintf_s(str, 20,_T("%.01f"),pCp->Side[PORTSIDE].fZ);
		break;
	case 4:	swprintf_s(str, 20,_T("%.02f"),pCp->Side[PORTSIDE].fI);
		break;
	case 5:	swprintf_s(str, 20,_T("%.02f"),pCp->Side[PORTSIDE].fJ);
		break;
	case 6:	swprintf_s(str, 20,_T("%.02f"),pCp->Side[PORTSIDE].fK);
		break;
	case 7:
		if(pDispInfo->item.iItem>2) {
			swprintf_s(str, 20,Type[pCp->nType&=1]);
		} else {
			str[0]=0;
		}
		break;
	case 8:
		switch(pDispInfo->item.iItem) {
		case 0:	swprintf_s(str, 20,_T(""));
			break;
		case 1:	fLength = CCoord::Length(CpMeasuredCentre[0],CpMeasuredCentre[1],PORTSIDE);
			swprintf_s(str, 20,_T("%.1f"),fLength);
			break;
		case 2:	fLength = CCoord::Length(CpMeasuredCentre[0],CpMeasuredCentre[2],PORTSIDE);
			swprintf_s(str, 20,_T("%.1f"),fLength);
			break;
		case 3:	swprintf_s(str, 20,_T(""));
			break;
		case 4:	fLength = CCoord::Length(CpCadCentre[0],CpCadCentre[1],PORTSIDE);
			swprintf_s(str, 20,_T("%.1f"),fLength);
			break;
		case 5:	fLength = CCoord::Length(CpCadCentre[0],CpCadCentre[2],PORTSIDE);
			swprintf_s(str, 20,_T("%.1f"),fLength);
			break;
		}
		break;
	}
}

void C3DViewEditDlg::GetDispInfoListCoordsBladeLines(LV_DISPINFO* pDispInfo,WCHAR *str)
{
	CCoord* pCp;

	if((m_pEditLine==NULL) || (m_nEditLineL<=0)) {
		str[0]=0;
		return;
	}
	pCp = (CCoord*)&m_pEditLine[theApp.m_nLine].m_pCp[pDispInfo->item.iItem];

	switch(pDispInfo->item.iSubItem) {
	case 0:	swprintf_s(str, 20,_T("%d"),pDispInfo->item.iItem+1);
		break;
	case 1:	swprintf_s(str, 20,_T("%.01f"),pCp->Side[PORTSIDE].fX);
		break;
	case 2:	swprintf_s(str, 20,_T("%.01f"),pCp->Side[PORTSIDE].fY);
		break;
	case 3:	swprintf_s(str, 20,_T("%.01f"),pCp->Side[PORTSIDE].fZ);
		break;
	case 4:	swprintf_s(str, 20,_T("%.03f"),pCp->Side[PORTSIDE].fI);
		break;
	case 5:	swprintf_s(str, 20,_T("%.03f"),pCp->Side[PORTSIDE].fJ);
		break;
	case 6:	swprintf_s(str, 20,_T("%.03f"),pCp->Side[PORTSIDE].fK);
		break;
	case 7: swprintf_s(str, 20,_T("%.01f"),pCp->Side[PORTSIDE].fR);
		break;
	case 8:
		str[0]=0;
		if(pCp->nType == TRAFFIC_LIGHT) swprintf_s(str, 20,L"%s",L"*");
		break;
	case 9:	swprintf_s(str, 20,_T("%.03f"),pCp->fMinThickness);
		break;
	case 10:	swprintf_s(str, 20,_T("%.03f"),pCp->fNomThickness);
		break;
	case 11:	swprintf_s(str, 20,_T("%.03f"),pCp->fMaxThickness);
		break;
	}

}

void C3DViewEditDlg::OnChangeEditLine()
{

}

void C3DViewEditDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSpinButtonCtrl *pSpin = (CSpinButtonCtrl *)pScrollBar;
	CSliderCtrl *pSlider = (CSliderCtrl *)pScrollBar;
	int	nTemp;
	CString Buff;

	switch(pSpin->GetDlgCtrlID()) {
	case IDC_SPIN_LINE:
		nTemp = theApp.m_nLine;
		theApp.m_nLine=pSpin->GetPos()&0xffff;
		if(nTemp - theApp.m_nLine) {
			FillList();
			UpdateAllControls();
			Invalidate(FALSE);
		}
		break;
		/*
	case IDC_SPIN_FAST_INCREMENT:
		PROFILE->m_dFastIncrement=(double)(pSpin->GetPos()&0xffff)/100;
		Buff.Format(_T("%.02f mm"),PROFILE->m_dFastIncrement);
		m_editFastIncrement.SetWindowText(Buff);
		break;
	case IDC_SPIN_SLOW_INCREMENT:
		PROFILE->m_dSlowIncrement=(double)(pSpin->GetPos()&0xffff)/100;
		Buff.Format(_T("%.02f mm"),PROFILE->m_dSlowIncrement);
		m_editSlowIncrement.SetWindowText(Buff);
		break;
	case IDC_SPIN_SCAN_SPEED:
		theApp.m_LastSettings.dActualScanSpeed=(double)(pSpin->GetPos()&0xffff);
		Buff.Format(_T("%.0f mm/s"),theApp.m_LastSettings.dActualScanSpeed);
		m_editScanSpeed.SetWindowText(Buff);
		break;
		*/
	}

	CResizablePage::OnVScroll(nSBCode, nPos, pScrollBar);
}

void C3DViewEditDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect Rect = m_StaticPosView.m_Rect;
	CRect		rr,rect;
	CDC			dcMem;
	CBitmap		bmp;
	static	int	nWidth;
	static	int	nHeight;
	static int nLoop=0;
	CString Buff;

	if(theApp.m_nJobNumber == GKN) {
		m_nDisplayFastAxis = 0;
		m_nDisplaySlowAxis = 2;
	} else {
		m_nDisplayFastAxis = PROFILE->m_nFastAxis;
		m_nDisplaySlowAxis = PROFILE->m_nSlowAxis;
	}

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
	rect.right = rr.Width();
	rect.bottom = rr.Height();

	bmp.CreateCompatibleBitmap(&dc,rr.Width(),rr.Height());
	bmp.SetBitmapBits((DWORD)(rr.Width()*rr.Height()*4),m_pArray);
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

	switch(m_nPaintMode) {
	case 0:
		ChangeLimitsForAspectRatio(rect);

		DrawXtipYTipGraticule((CPaintDC*)&dcMem,rect);
		DrawPerspectiveArrows((CPaintDC*)&dcMem,rect);

		DrawProjectedLines((CPaintDC*)&dcMem,rect,SCREEN,SCANLINES);
		DrawProjectedLines((CPaintDC*)&dcMem,rect,SCREEN,TAUGHTLINES);
		DrawProjectedLines((CPaintDC*)&dcMem,rect,SCREEN,BRAINLINES);
		DrawProjectedLines((CPaintDC*)&dcMem,rect,SCREEN,ENVELOPELINES);
		DrawProjectedLines((CPaintDC*)&dcMem,rect,SCREEN,NEWLINE);
		DrawProjectedLines((CPaintDC*)&dcMem,rect,SCREEN,SURFACELINES);

		if(theApp.m_LastSettings.nDisplayOutSideInside==OUTSIDE) DrawProjectedLines((CPaintDC*)&dcMem,rect,SCREEN,OUTSIDELINES);
		if(theApp.m_LastSettings.nDisplayOutSideInside==INSIDE) DrawProjectedLines((CPaintDC*)&dcMem,rect,SCREEN,INSIDELINES);

		DrawProjectedHoles((CPaintDC*)&dcMem,rect,SCREEN);
		DrawProjectedWaterJets((CPaintDC*)&dcMem,rect,SCREEN);
		DrawSelectedArea((CPaintDC*)&dcMem,rect,SCREEN);
		DrawWayPoints((CPaintDC*)&dcMem,rect,SCREEN);


		dc.BitBlt(rr.left,rr.top,rr.Width(),rr.Height(),&dcMem,0,0,SRCCOPY);
		break;
	case 1:
		break;
	case 2:
		break;
	}


}

void C3DViewEditDlg::ClearView(CPaintDC *pDC, CRect rr, int nDevice)
{

	pDC->FillRect(rr,&CBrush(RGB(255,255,255)));

}

void C3DViewEditDlg::DrawWayPoints(CPaintDC *pDC, CRect rr, int nDevice)
{
	CRgn rgn;
	CRect Rect;
	CPoint	pt;
	CRectFloat rrFloat = rr;

	if(m_bDrawWayPts != TRUE) return;
	if(PROFILE->m_nWayPointsL<=0) return;

	pDC->GetClipBox(&Rect);
	rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
	pDC->SelectClipRgn(&rgn);


	for(int nLine = 0;nLine<PROFILE->m_nWayPointsL;nLine++) {
		for(int ss=0;ss<PROFILE->m_WayPoints[nLine].m_nCoordL;ss++) {
			WorldToClient(PROFILE->m_WayPoints[nLine].GetPtrCoord(ss),rrFloat,&pt);
			if(rr.PtInRect(pt)) {
				pDC->SetPixel(pt,RGB(0,0,255));
			}
		}
	}
	rgn.SetRectRgn(&rr);
	pDC->SelectClipRgn(&rgn);
}

void C3DViewEditDlg::DrawProjectedLines(CPaintDC *pDC, CRect rr, int nDevice,int nWhichLines)
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CRgn rgn;
	CRect Rect;
	CPoint	pt,ptPrevious,ptStart;
	CString Buff;
	int	nLineL,nStep,nStartLine,nFinishLine;
	CPolyCoord *pLine = NULL;
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
		rgbDraw = PROFILE->m_EnvelopeLine[0].m_rgb;
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
	case SURFACELINES:
		if(PROFILE->m_nSurfaceLineL<=0) return;
		if((theApp.m_LastSettings.bDisplaySurfaceLines!=TRUE) && (theApp.m_LastSettings.bDisplaySurfaceCrosses!=TRUE)) return;
		bDisplayCrosses = theApp.m_LastSettings.bDisplaySurfaceCrosses;
		bDisplayLines = theApp.m_LastSettings.bDisplaySurfaceLines;
		nLineL = PROFILE->m_nSurfaceLineL;
		rgbDraw = theApp.m_LastSettings.rgbDisplaySurfaceColor;
		nStep = 1;
		break;
	case OUTSIDELINES:
		if(PROFILE->m_nLineBladeL[OUTSIDE]<=0) return;
		if((theApp.m_LastSettings.bDisplayBladeLines[OUTSIDE]!=TRUE) && (theApp.m_LastSettings.bDisplayBladeCrosses[OUTSIDE]!=TRUE)) return;
		bDisplayCrosses = theApp.m_LastSettings.bDisplayBladeCrosses[OUTSIDE];
		bDisplayLines = theApp.m_LastSettings.bDisplayBladeLines[OUTSIDE];
		nLineL = PROFILE->m_nLineBladeL[OUTSIDE];
		rgbDraw = theApp.m_LastSettings.rgbBladeColor[OUTSIDE];
		nStep = 1;
		break;
	case INSIDELINES:
		if(PROFILE->m_nLineBladeL[INSIDE]<=0) return;
		if((theApp.m_LastSettings.bDisplayBladeLines[INSIDE]!=TRUE) && (theApp.m_LastSettings.bDisplayBladeCrosses[INSIDE]!=TRUE)) return;
		bDisplayCrosses = theApp.m_LastSettings.bDisplayBladeCrosses[INSIDE];
		bDisplayLines = theApp.m_LastSettings.bDisplayBladeLines[INSIDE];
		nLineL = PROFILE->m_nLineBladeL[INSIDE];
		rgbDraw = theApp.m_LastSettings.rgbBladeColor[INSIDE];
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

	CFont *pFont = GetFont();
	pFont->GetLogFont(&theApp.m_LastSettings.lfViewGraticule);

	CFont Font;
	CSize size;
	CRect rrText;
	LOGFONT	lf = theApp.m_LastSettings.lfViewGraticule;
	lf.lfEscapement=0;
	theApp.SetFontLanguage(&lf);
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);
	pDC->SetBkMode(TRANSPARENT);
	CBrush redBrush(RGB(255,0,0));
	CBrush* pOldBrush = pDC->SelectObject(&redBrush);


	pDC->GetClipBox(&Rect);
	rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
	pDC->SelectClipRgn(&rgn);

	CPen	penRed(PS_SOLID  ,1,RGB(255,0,0));
	CPen	penBlack(PS_SOLID  ,1,RGB(0,0,0));
	CPen * pOldPen = pDC->SelectObject(&penBlack);

	CPoint *pPt = NULL;
	BYTE *pByte = NULL;
	int nCount = 0;
	CPen *pPen = NULL;
	CRect rrEllipse;
	int nLine;

	if(bDisplayCrosses==TRUE) {
		for(nLine = nStartLine; nLine<nFinishLine ; nLine+=nStep) {
			SAFE_DELETE(pPen);
			pPen = new CPen;
			pPen->CreatePen(PS_SOLID,1,rgbDraw);
			switch(nWhichLines) {
			case TAUGHTLINES: pLine = &PROFILE->m_TaughtLine[nLine];
				break;
			case SCANLINES:	pLine = &PROFILE->m_ScanLine[nLine];
				break;
			case BRAINLINES: pLine = &PROFILE->m_BrainLine[nLine];
				break;
			case ENVELOPELINES:	pLine = &PROFILE->m_EnvelopeLine[nLine];
				SAFE_DELETE(pPen);
				pPen = new CPen;
				pPen->CreatePen(PS_SOLID,1,pLine->m_rgb);
				break;
			case NEWLINE:	pLine = &pFrame->m_CpNewLine;
				break;
			case SURFACELINES: pLine = &PROFILE->m_SurfaceLine[nLine];
				break;
			case OUTSIDELINES: pLine = &PROFILE->m_LineBlade[OUTSIDE][nLine];
				break;
			case INSIDELINES: pLine = &PROFILE->m_LineBlade[INSIDE][nLine];
				break;
			}
			pDC->SelectObject(pPen);
			SAFE_DELETE(pPt);
			SAFE_DELETE(pByte);
			pPt = new CPoint[pLine->m_nCoordL * 4];
			pByte = new BYTE[pLine->m_nCoordL * 4];
			nCount=0;
			for(int ss=0;ss<pLine->m_nCoordL;ss++) {
				WorldToClient(pLine->GetPtrCoord(ss),rrFloat,&pt);
				if((ss == theApp.m_nIndex) && (nLine == theApp.m_nLine) && (nWhichLines == theApp.m_nEditLineType)) {
					pDC->SelectObject(&penRed);
					pDC->MoveTo(pt.x-5,pt.y-5);
					pDC->LineTo(pt.x+6,pt.y+6);
					pDC->MoveTo(pt.x-5,pt.y+5);
					pDC->LineTo(pt.x+6,pt.y-6);
					Buff.Format(_T("%s %d %s %d"),Line,theApp.m_nLine+1,Index,theApp.m_nIndex+1);
					size = pDC->GetTextExtent(Buff);
					rrText.SetRect(pt.x+5,pt.y-15,pt.x+5+size.cx,pt.y-15+size.cy);
					pDC->FillRect(rrText,&CBrush(RGB(230,230,230)));
					pDC->TextOut(pt.x+5,pt.y-15,Buff);
				} else {
					if(rr.PtInRect(pt)) {
						pPt[nCount].x = pt.x-2; pPt[nCount].y = pt.y-2; pByte[nCount++] = PT_MOVETO;
						pPt[nCount].x = pt.x+3; pPt[nCount].y = pt.y+3; pByte[nCount++] = PT_LINETO;
						pPt[nCount].x = pt.x-2; pPt[nCount].y = pt.y+2; pByte[nCount++] = PT_MOVETO;
						pPt[nCount].x = pt.x+3; pPt[nCount].y = pt.y-3; pByte[nCount++] = PT_LINETO;
					}
				}

				if(nWhichLines == SCANLINES && (ss==0 || ss == (pLine->m_nCoordL-1))) {
					if(pLine->GetCoordType(ss) == 1) {
						rrEllipse.left = pt.x-5;
						rrEllipse.right = pt.x+6;
						rrEllipse.top = pt.y-5;
						rrEllipse.bottom = pt.y+6;
						pDC->SelectObject(penRed);
						pDC->SetBkMode(OPAQUE);
						pDC->Ellipse(&rrEllipse);
						pDC->SetBkMode(TRANSPARENT);
					}
				}
			};
			pDC->SelectObject(pPen);
			pDC->PolyDraw(pPt,pByte,nCount);
			SAFE_DELETE(pPen);
		}
	}
	if(bDisplayLines==TRUE) {
		for(int nLine = nStartLine; nLine<nFinishLine ; nLine+=nStep) {
			SAFE_DELETE(pPen);
			pPen = new CPen;
			pPen->CreatePen(PS_SOLID,1,rgbDraw);
			switch(nWhichLines) {
			case TAUGHTLINES:	pLine = &PROFILE->m_TaughtLine[nLine];
				break;
			case SCANLINES:	pLine = &PROFILE->m_ScanLine[nLine];
				break;
			case BRAINLINES: pLine = &PROFILE->m_BrainLine[nLine];
				break;
			case ENVELOPELINES:	pLine = &PROFILE->m_EnvelopeLine[nLine];
				bJoinUpLastPoint=TRUE;
				SAFE_DELETE(pPen);
				pPen = new CPen;
				pPen->CreatePen(PS_SOLID,1,pLine->m_rgb);
				break;
			case NEWLINE:	pLine = &pFrame->m_CpNewLine;
				bJoinUpLastPoint=FALSE;
				break;
			case SURFACELINES:	pLine = &PROFILE->m_SurfaceLine[nLine];
				break;
			case OUTSIDELINES:	pLine = &PROFILE->m_LineBlade[OUTSIDE][nLine];
				break;
			case INSIDELINES:	pLine = &PROFILE->m_LineBlade[INSIDE][nLine];
				break;
			}
			if((nLine==theApp.m_nLine) && (nWhichLines!=NEWLINE)) {
				pDC->SelectObject(&penRed);
			} else {
				pDC->SelectObject(pPen);
			}
			DrawProjectedLine(pLine,pDC,rr,bJoinUpLastPoint);
			DrawNormals(pLine,pDC,rr);
			SAFE_DELETE(pPen);
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
		pDC->SelectObject(&penBlack);
	}

	SAFE_DELETE(pPt);
	SAFE_DELETE(pByte);

	rgn.SetRectRgn(&rr);
	pDC->SelectClipRgn(&rgn);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldBrush);

}

void C3DViewEditDlg::DrawProjectedLine(CPolyCoord *pLine,CDC* pDC,CRect rr,bool bJoinUpLastPoint)
{
	int ss;
	float fPos,fStep;
	int	nCoordL = pLine->m_nCoordL;
	CCoord Cp,CpStart;
	CRectFloat rrFloat = rr;
	CPoint *pPt = NULL;
	CPolyCoord Line;

	switch(pLine->m_nStyle) {
	case LINEAR_LINE:
		pPt = new CPoint[nCoordL];
		for(ss=0;ss<nCoordL;ss++) {
			WorldToClient(pLine->GetPtrCoord(ss),rr,&pPt[ss]);
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
		Line = pLine;
		Line.SetPrimaryAxis(30);
		Line.PreProcessLine();
		fStep = Line.m_fLinearLength / 200.0f;
		if(fStep<=0.0f) fStep=1.0;
		pPt = new CPoint[802];
		for(fPos=0,ss=0;fPos<=Line.m_fLinearLength;fPos+=fStep,ss++) {
			Line.CalculateCoord(fPos,Cp);
			WorldToClient(&Cp,rrFloat,&pPt[ss]);
		}
		pDC->Polyline(pPt,ss);
		break;
	case POLYNOMIAL_LINE:
	case QUADRATIC_LINE:
		pLine->m_bModified=TRUE;
		pLine->SetPrimaryAxis(PROFILE->m_nFastAxis);
		pLine->PreProcessLine();
		fStep = pLine->m_fLinearLength / 200.0f;
		if(fStep<=0.0f) fStep=1.0;
		pLine->GetCoord(0,&CpStart);
		pPt = new CPoint[802];
		for(fPos=0,ss=0;fPos<=pLine->m_fLinearLength;fPos+=fStep,ss++) {
			pLine->CalculateCoord(fPos + CpStart.fPos[pLine->m_nPrimaryAxis],Cp);
			WorldToClient(&Cp,rrFloat,&pPt[ss]);
		}
		pDC->Polyline(pPt,ss);
		break;
	}

	SAFE_DELETE_ARRAY( pPt );


}

void C3DViewEditDlg::DrawNormals(CPolyCoord *pLine,CDC* pDC,CRect rr)
{
	if(theApp.m_LastSettings.nDisplayEditNormals != 1) return;

	CPen *pPen = NULL,*pOldPen = NULL;
	int ss,nNorm;
	int	nCoordL = pLine->m_nCoordL;
	CPoint pt;
	CCoord Cp;



//	switch(pLine->m_nStyle) {
//	case LINEAR_LINE:
	for(nNorm=6;nNorm<9;nNorm++) {
		switch(nNorm) {
		case 6: pPen = new CPen(PS_SOLID,1,RGB(255,0,0));
			break;
		case 7: pPen = new CPen(PS_SOLID,1,RGB(0,255,0));
			break;
		case 8: pPen = new CPen(PS_SOLID,1,RGB(0,0,255));
			break;
		}
		pOldPen = pDC->SelectObject(pPen);
		for(ss=0;ss<nCoordL;ss++) {
			WorldToClient(pLine->GetPtrCoord(ss),rr,&pt);
			pt.y = (int)((float)(rr.bottom+rr.top)/2 * pLine->m_pCp[ss].Side0.fPos[nNorm]) + (rr.bottom+rr.top)/2;
			if(ss>0) {
				pDC->LineTo(pt);
			} else {
				pDC->MoveTo(pt);
			}
		};
		pDC->SelectObject(pOldPen);
		delete pPen;
	}
/*
		break;
	case SPLINE_LINE:
		pLine->m_bModified=TRUE;
		pLine->PreProcessLine();
		fStep = pLine->m_fLinearLength / 800.0f;
		if(fStep<=0.0f) fStep=1.0;
		for(fPos=0;fPos<=pLine->m_fLinearLength;fPos+=fStep) {
			pLine->CalculateCoord(fPos,Cp);
			WorldToClient(&Cp,rr,&pt);
			if(fPos>0.0f) {
				pDC->LineTo(pt);
			} else {
				pDC->MoveTo(pt);
			}
		}
		break;
	}
*/

}
void C3DViewEditDlg::DrawProjectedHoles(CPaintDC *pDC, CRect rr, int nDevice)
{
	CPoint point[3],ptCentre;
	COORDDATA Cp;
	CRgn rgn;
	CRect Rect,rectExclusion;
	CPoint	pt,ptPrevious,ptStart;
	CSize size;
	CString Buff;
	int	nRadius = 2;
	float fLength;
	LOGFONT lf;
	memset( (void*)&lf, 0, sizeof(lf));
	lf.lfHeight = 14;
	theApp.SetFontLanguage(&lf);

	CFont Font;
	Font.CreateFontIndirect(&lf);
	CFont* pOldFont = pDC->SelectObject(&Font);
	int OldBkMode=pDC->GetBkMode();
	COLORREF OldTextColor = pDC->GetTextColor();
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0,0,255));

	pDC->GetClipBox(&Rect);
	rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
	pDC->SelectClipRgn(&rgn);


	CPen penBlue(PS_SOLID,1,RGB(0,0,255));
	CBrush brushRed(RGB(255,0,0));
	CPen* pOldPen = pDC->SelectObject(&penBlue);
	CBrush* pOldBrush = pDC->SelectObject(&brushRed);

	for(int hh=0;hh<2;hh++) {
		if(PROFILE->m_bExclusionZones[hh]) {
			Cp = PROFILE->m_CpExclusion[hh][0];
			WorldToClient(&Cp,rr,&point[0]);
			rectExclusion.left = point[0].x;
			rectExclusion.top = point[0].y;
			Cp = PROFILE->m_CpExclusion[hh][1];
			WorldToClient(&Cp,rr,&point[0]);
			rectExclusion.right = point[0].x;
			rectExclusion.bottom = point[0].y;

			pDC->Rectangle(&rectExclusion);
		}
	}

	for (int nIndex = 0; nIndex < 3; nIndex++) {
		WorldToClient(&PROFILE->m_CpCadHole[nIndex], rr, &point[nIndex]);
		pDC->Ellipse(point[nIndex].x - nRadius, point[nIndex].y - nRadius, point[nIndex].x + nRadius, point[nIndex].y + nRadius);
		size = pDC->GetTextExtent(theApp.m_3PtName[nIndex]);
		pDC->TextOut(point[nIndex].x - size.cx / 2, point[nIndex].y + 4, theApp.m_3PtName[nIndex]);
	}


	if(theApp.m_LastSettings.nRenderHoleRefLengths == 1) {

		pDC->MoveTo(point[0]);
		pDC->LineTo(point[1]);
		pDC->LineTo(point[2]);
		pDC->LineTo(point[0]);

		for(int vv=0;vv<3;vv++) {
			int ww=(vv+1)%3;
			lf.lfHeight = 18;
			lf.lfEscapement=(long)(atan2(point[vv].y-point[ww].y,point[vv].x-point[ww].x) * RAD_TO_DEG * -10.0);
			if(lf.lfEscapement>900) lf.lfEscapement -= 1800;
			if(lf.lfEscapement<-900) lf.lfEscapement += 1800;

			CFont* fontVector = (CFont *) new CFont;
			fontVector->CreateFontIndirect(&lf);
			pDC->SelectObject(fontVector);

			fLength = CCoord::Length(PROFILE->m_CpCadHole[vv],PROFILE->m_CpCadHole[ww],0);

			Buff.Format(L"%.01fmm",fLength);

			ptCentre = point[vv] + point[ww];
			ptCentre.x /=2;
			ptCentre.y /=2;

			pDC->TextOut(ptCentre.x,ptCentre.y,Buff);


			delete fontVector;
		}
	}



	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pDC->SetTextColor(OldTextColor);
	pDC->SetBkMode(OldBkMode);

	rgn.SetRectRgn(&rr);
	pDC->SelectClipRgn(&rgn);

}

void C3DViewEditDlg::DrawProjectedWaterJets(CPaintDC *pDC, CRect rr, int nDevice)
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

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	theApp.m_Kinematics.CalculateCoordAlongNorm(CpSurface,&CpNozzle,PORTSIDE,-20.0f);


	WorldToClient(&CpSurface,rr,&ptSurface);
	WorldToClient(&CpNozzle,rr,&ptNozzle);
	pDC->MoveTo(ptSurface);
	pDC->LineTo(ptNozzle);
	pDC->SelectObject(&penBlack);
	pDC->MoveTo(ptSurface.x-4,ptSurface.y);
	pDC->LineTo(ptSurface.x+5,ptSurface.y);
	pDC->MoveTo(ptSurface.x,ptSurface.y-4);
	pDC->LineTo(ptSurface.x,ptSurface.y+5);

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

	pDC->SelectObject(pOldPen);

	rgn.SetRectRgn(&rr);
	pDC->SelectClipRgn(&rgn);

}

void C3DViewEditDlg::DrawSelectedArea(CPaintDC *pDC, CRect rr, int nDevice)
{
	if(m_bSelectArea!=TRUE) return;
	CPoint pt[4];
	CCoord Cp;
	CRgn rgn;
	CRect Rect;
	CPen penBlue(PS_DASH,1,RGB(0,0,255));
	CPen* pOldPen = pDC->SelectObject(&penBlue);

	pDC->GetClipBox(&Rect);
	rgn.CreateRectRgn(rr.left,rr.top,rr.right,rr.bottom);
	pDC->SelectClipRgn(&rgn);


	WorldToClient(&m_CpSelectArea[0],rr,&pt[0]);

	switch (m_nDisplayFastAxis) {
	case 0:
		Cp = m_CpSelectArea[0];
		Cp.Side0.fX = m_CpSelectArea[1].Side0.fX;
		WorldToClient(&Cp, rr, &pt[1]);

		WorldToClient(&m_CpSelectArea[1], rr, &pt[2]);

		Cp = m_CpSelectArea[1];
		Cp.Side0.fX = m_CpSelectArea[0].Side0.fX;
		WorldToClient(&Cp, rr, &pt[3]);
		break;
	case 1:
		Cp = m_CpSelectArea[0];
		Cp.Side0.fY = m_CpSelectArea[1].Side0.fY;
		WorldToClient(&Cp, rr, &pt[1]);

		WorldToClient(&m_CpSelectArea[1], rr, &pt[2]);

		Cp = m_CpSelectArea[1];
		Cp.Side0.fY = m_CpSelectArea[0].Side0.fY;
		WorldToClient(&Cp, rr, &pt[3]);
		break;
	}

	pDC->MoveTo(pt[0]);
	pDC->LineTo(pt[1]);
	pDC->LineTo(pt[2]);
	pDC->LineTo(pt[3]);
	pDC->LineTo(pt[0]);

	pDC->SelectObject(pOldPen);
	rgn.SetRectRgn(&rr);
	pDC->SelectClipRgn(&rgn);

}


void C3DViewEditDlg::DrawPerspectiveArrows(CPaintDC *pDC, CRect rr)
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






void C3DViewEditDlg::OnViewRThetaPath()
{
	if (m_ToolsPathDlg == NULL) {
		m_ToolsPathDlg = new CToolPathDlg(this, (CWnd**)&m_ToolsPathDlg);
		m_ToolsPathDlg->Create(CToolPathDlg::IDD, this);
	} else {
		m_ToolsPathDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}


}

void C3DViewEditDlg::RefreshView()
{

	Invalidate(FALSE);

}




void C3DViewEditDlg::GetVolumeTrace(int nSample, int nLine, char *pTrace,CData* pData,CUSLFile* pFile)
{
	int	nIntPos = 0;
	ULONGLONG nFileLength = pFile->GetLength();

	UINT64 nOffset = pData->m_n64VolumeOffset[0] + 0x100 + ((UINT64)pData->m_nSamplesLine * (UINT64)pData->m_nAscanLength * (UINT64)nLine) + (UINT64)(nSample * pData->m_nAscanLength);
	if((nOffset + pData->m_nAscanLength) <= nFileLength) {
		pFile->Seek(nOffset,CFile::begin);
		pFile->Read(&nIntPos,4);
		if((nIntPos >=0) && (nIntPos<pData->m_nAscanLength)) {
			if((nIntPos + pData->m_nAscanLength-4) <= pData->m_nRealAscanLength) {
				if(pData->m_nAscanLength>=4) {
					pFile->Read(&pTrace[nIntPos],pData->m_nAscanLength-4);
				}
			}
		}
	}
}





void C3DViewEditDlg::OnFileSaveAs()
{
	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = PROFILE->m_FileName;
	CProfileSheet* pSheet = (CProfileSheet*)GetParent();

	if((nPos = PROFILE->m_FileName.ReverseFind('.')) >0) {
		InitialFile = PROFILE->m_FileName.Left(nPos);
	}

	CFileDialog FileDlg(FALSE,_T(".pro"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Profile (*.pro)|*.pro|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_ProfilePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_ProfilePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			PROFILE->Save(&File);
			File.Close();
			SetWindowTitle();
			pSheet->SetWindowTitle();
			theApp.ActivityLog(L"Profile saved : " + FilePath);
		}
	}
}

void C3DViewEditDlg::OnUpdateFileOpen(CCmdUI* pCmdUI)
{
}

void C3DViewEditDlg::OnFileOpen()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CProfileSheet* pSheet = (CProfileSheet*)GetParent();
	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;

	if(PROFILE->CheckNewProfileAllowed()!=TRUE) return;

	CFileThumbNailDlg FileDlg(TRUE,_T(".pro"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Profile (*.pro)|*.pro|All Files (*.*)|*.*|"),NULL);
//	CFileDialog FileDlg(TRUE,_T(".pro"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Profile (*.pro)|*.pro|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_ProfilePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_ProfilePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			CWaitCursor Wait;
			PROFILE->Retrieve(&File);
			if(theApp.m_LastSettings.nLoadUltrasonicsFromProfile==1) {
				PROFILE->CopyLocalUtToGlobal();
			}
			File.Close();
			Wait.Restore();
			CreateColumns();
			FillList();
			UpdateAllControls();
			FindAllLimits(true);
			Invalidate(FALSE);
			SetWindowTitle();
			pSheet->SetWindowTitle();
			pFrame->SendMessage(UI_PROFILE_OPENED,(WPARAM)this,NULL);
			theApp.ActivityLog(L"Profile loaded : " + FilePath);
		}
	}

}

void C3DViewEditDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu* menu = (CMenu *) new CMenu;
	CMenu* pPopup;
	CPoint pt = point;
	CRect Rect = m_StaticPosView.m_Rect;
	ScreenToClient(&point);
	m_ptClient = point;
	CString Buff,Danger, strGoto;

	if(Rect.PtInRect(point)) {
		if(m_bSelectArea==FALSE) {
			switch(theApp.m_nEditLineType) {
			case TAUGHTLINES:
			case SCANLINES:
			case OUTSIDELINES:
			case INSIDELINES:
				menu->LoadMenu(IDR_RB_3D_VIEW_MENU);
				pPopup = menu->GetSubMenu(0);
				strGoto.LoadString(IDS_Goto);

				Buff.Format(L"%s %s", strGoto, theApp.m_DlgSideName[0]);
				pPopup->ModifyMenu(ID_LIST3D_GOTO_LEFT,MF_BYCOMMAND,ID_LIST3D_GOTO_LEFT,Buff);
				Buff.Format(L"%s %s", strGoto, theApp.m_DlgSideName[1]);
				pPopup->ModifyMenu(ID_LIST3D_GOTO_RIGHT,MF_BYCOMMAND,ID_LIST3D_GOTO_RIGHT,Buff);
				Buff.Format(L"%s %s && %s", strGoto, theApp.m_DlgSideName[0], theApp.m_DlgSideName[1]);
				pPopup->ModifyMenu(ID_LIST3D_GOTO_BOTH,MF_BYCOMMAND,ID_LIST3D_GOTO_BOTH,Buff);

				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
				break;
			case BRAINLINES:
				menu->LoadMenu(IDR_RB_3D_VIEW_BRAIN_MENU);
				pPopup = menu->GetSubMenu(0);
				if(theApp.m_LastSettings.bGotoViaDangerPlane==TRUE) {
					Danger.LoadString(IDS_Via_Danger_Plane);
					Buff.LoadString(IDS_Goto_Left);
					pPopup->ModifyMenu(ID_LIST3D_GOTO_LEFT,MF_BYCOMMAND,ID_LIST3D_GOTO_LEFT,Buff + _T(" ") + Danger);
					Buff.LoadString(IDS_Goto_Right);
					pPopup->ModifyMenu(ID_LIST3D_GOTO_RIGHT,MF_BYCOMMAND,ID_LIST3D_GOTO_RIGHT,Buff + _T(" ") + Danger);
					Buff.LoadString(IDS_Goto_Both);
					pPopup->ModifyMenu(ID_LIST3D_GOTO_BOTH,MF_BYCOMMAND,ID_LIST3D_GOTO_BOTH,Buff + _T(" ") + Danger);
				}
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
				break;
			case ENVELOPELINES:
				menu->LoadMenu(IDR_RB_3D_VIEW_ENVELOPE_MENU);
				pPopup = menu->GetSubMenu(0);
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
				break;
			}
		} else {
			menu->LoadMenu(IDR_RB_SELECTAREA_MENU);
			pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,this);
		};
	}

	delete menu;
}

BOOL C3DViewEditDlg::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
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
	case ID_FILE_NEW:  strTipText = "New";
		break;
	case ID_FILE_OPEN:  strTipText = "Open";
		break;
	case ID_FILE_SAVE_AS:  strTipText = "Save as";
		break;
	case ID_VIEW_XY:  strTipText = "X Y";
		break;
	case ID_VIEW_YZ:  strTipText = "Y Z";
		break;
	case ID_VIEW_XZ:  strTipText = "X Z";
		break;
	case ID_EDIT_SCAN_LINES:  strTipText.LoadString(IDS_Scan_Lines);
		break;
	case ID_EDIT_BRAIN_LINES:  strTipText.LoadString(IDS_Brane_Lines);
		break;
	case ID_EDIT_HOLES:  strTipText.LoadString(IDS_Ref_Pts);
		break;
	case ID_EDIT_TRANSFORM:  strTipText.LoadString(IDS_Transform);
		break;
	case ID_EDIT_ENVELOPE_LINES:  strTipText.LoadString(IDS_Envelope);
		break;
	case ID_EDIT_HOLDINGFIXTURE:  strTipText.LoadString(IDS_Holding_Fixture);
		break;
	case ID_BUTTON_3D_VIEW_POINT: strTipText.LoadString(IDS_View_Position);
		break;
	case ID_BUTTON_3D_COMPONENT: strTipText.LoadString(IDS_Move_Component);
		break;
	case ID_BUTTON_3D_MACHINE: strTipText.LoadString(IDS_Machine);
		break;
	case ID_BUTTON_3D_DANGER_ZONE: strTipText.LoadString(IDS_Danger_Zone);
		break;
	case ID_BUTTON_3D_ISOMETRIC: strTipText.LoadString(IDS_Isometric);
		break;
	case ID_BUTTON_3D_CSCAN: strTipText.LoadString(IDS_Overlay_CScan);
		break;
	case ID_BUTTON_3D_SCANLINES: strTipText.LoadString(IDS_Scan_Lines);
		break;
	case ID_BUTTON_3D_ENEVLOPE: strTipText.LoadString(IDS_Envelope);
		break;
	case ID_BUTTON_3D_REF_POINTS: strTipText.LoadString(IDS_Ref_Points);
		break;
	case ID_BUTTON_ADD_COORDINATE: strTipText.LoadString(IDS_Add_Coordinate);
		break;
	case ID_BUTTON_INSERT_COORDINATE: strTipText.LoadString(IDS_Insert_Coordinate);
		break;
	case ID_BUTTON_DELETE_COORDINATE: strTipText.LoadString(IDS_Delete_Coordinate);
		break;
	case ID_BUTTON_NEW_LINE: strTipText.LoadString(IDS_New_Line);
		break;
	case ID_EDIT_WAYPOINTS: strTipText.LoadString(IDS_Waypoints);
		break;
	case ID_BUTTON_SELECTAREA:  strTipText.LoadString(IDS_Select_Area);
		break;
	case ID_BUTTON_EDIT_OUTSIDE:  strTipText.LoadString(IDS_Outside_form);
		break;
	case ID_BUTTON_EDIT_INSIDE:  strTipText.LoadString(IDS_Inside_form);
		break;
	}

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}


void C3DViewEditDlg::OnEditScanlines()
{
	
	if (m_pScanLinesEditDlg == NULL) {
		CloseAllChildren();
		CRect rr;
		m_wndFileToolBar.GetWindowRect(rr);
		m_pScanLinesEditDlg = new CScanLinesEditDlg(this,(CWnd**)&m_pScanLinesEditDlg,rr.left,rr.bottom);
		m_pScanLinesEditDlg->Create(CScanLinesEditDlg::IDD, this);
	}
	theApp.m_nEditLineType = SCANLINES;
	InitializeEditLine();
	CreateColumns();
	FillList();
	SetToolBarCheckedState();
	InvalidateCoordArrayPane(0xffff);
}

void C3DViewEditDlg::OnEditTaughtLines()
{
	
	if (m_pTaughtLinesEditDlg == NULL) {
		CloseAllChildren();
		CRect rr;
		m_wndFileToolBar.GetWindowRect(rr);

		m_pTaughtLinesEditDlg = new CTaughtLinesEditDlg(this,(CWnd**)&m_pTaughtLinesEditDlg,rr.left,rr.bottom);
		m_pTaughtLinesEditDlg->Create(CTaughtLinesEditDlg::IDD, this);
	}
	theApp.m_nEditLineType = TAUGHTLINES;
	InitializeEditLine();
	CreateColumns();
	FillList();
	SetToolBarCheckedState();
	InvalidateCoordArrayPane(0xffff);
}

void C3DViewEditDlg::OnEditenvelopelines()
{
	if (m_pEnvelopeLinesEditDlg == NULL) {
		CloseAllChildren();
		CRect rr;
		m_wndFileToolBar.GetWindowRect(rr);
		m_pEnvelopeLinesEditDlg = new CEnvelopeEditDlg(this,(CWnd**)&m_pEnvelopeLinesEditDlg,rr.left,rr.bottom);
		m_pEnvelopeLinesEditDlg->Create(CEnvelopeEditDlg::IDD, this);
	}
	theApp.m_nEditLineType = ENVELOPELINES;
	InitializeEditLine();
	CreateColumns();
	FillList();
	SetToolBarCheckedState();
	InvalidateCoordArrayPane(0xffff);
}

void C3DViewEditDlg::OnEditHoldingfixture()
{
	
	if (m_pHoldingFixtureEditDlg == NULL) {
		CloseAllChildren();
		CRect rr;
		m_wndFileToolBar.GetWindowRect(rr);
		m_pHoldingFixtureEditDlg = new CHoldingFixtureEditDlg(this,(CWnd**)&m_pHoldingFixtureEditDlg,rr.left,rr.bottom);
		m_pHoldingFixtureEditDlg->Create(CHoldingFixtureEditDlg::IDD, this);
//		m_pHoldingFixtureEditDlg->SetFont(m_pFont);
	}
	theApp.m_nEditLineType = HOLDINGFIXTURE;
	InitializeEditLine();
	CreateColumns();
	FillList();
	SetToolBarCheckedState();
	InvalidateCoordArrayPane(0xffff);
}

void C3DViewEditDlg::OnEditSurfaceLines() 
{
	if (m_pScanLinesEditDlg == NULL) {
		CloseAllChildren();
		CRect rr;
		m_wndFileToolBar.GetWindowRect(rr);
		m_pScanLinesEditDlg = new CScanLinesEditDlg(this,(CWnd**)&m_pScanLinesEditDlg,rr.left,rr.bottom);
		m_pScanLinesEditDlg->Create(CScanLinesEditDlg::IDD, this);
	}
	theApp.m_nEditLineType = SURFACELINES;
	InitializeEditLine();
	CreateColumns();
	FillList();
	SetToolBarCheckedState();
}

void C3DViewEditDlg::OnEditWaypoints()
{
	if(m_bDrawWayPts == FALSE) {
		m_bDrawWayPts = TRUE;
	} else {
		m_bDrawWayPts = FALSE;
	}


	if (m_pWayLinesEditDlg == NULL) {
		CloseAllChildren();
		CRect rr;
		m_wndFileToolBar.GetWindowRect(rr);
		m_pWayLinesEditDlg = new CWayLinesEditDlg(this,(CWnd**)&m_pWayLinesEditDlg,rr.left,rr.bottom);
		m_pWayLinesEditDlg->Create(CWayLinesEditDlg::IDD, this);
	}
	theApp.m_nEditLineType = WAYLINES;
	InitializeEditLine();
	CreateColumns();
	FillList();
	SetToolBarCheckedState();

	Invalidate(FALSE);
}

void C3DViewEditDlg::OnRclickListCoords(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CMenu *pPopup, *pPopup1;
	CPoint	Point;
	CString Buff,Language;

	if(theApp.m_nEditLineType != HOLES) {
		if(m_pEditLine == NULL) return;
		if(m_nEditLineL <= theApp.m_nLine) return;
	}
	
	CMenu* menu = (CMenu *) new CMenu;
	POSITION pos = m_listCoord.GetFirstSelectedItemPosition();
	theApp.m_nIndex=m_listCoord.GetNextSelectedItem(pos);

	GetCursorPos(&Point);

	switch(	theApp.m_nEditLineType) {
	default:
		switch(theApp.m_Tank.nScannerDescription) {
		case DUAL_SIDED_7_AXES:
		case DUAL_SIDED_SQUIRTER:
		case SINGLE_BRIDGE_DUAL_SIDED:
		case DUAL_SIDED_SQUIRTER_2:
		case DUAL_ROBOT:
		case DUAL_ROBOT_9_PLUS_9:
		case FB_11_AXIS:
		case SPX_ROBOT:
			if (menu->LoadMenu(IDR_RB_3D_LIST_MENU)) {
				pPopup = menu->GetSubMenu(0);
				if(theApp.m_nIndex<(m_pEditLine[theApp.m_nLine].m_nCoordL-1)) {
					Buff.Format(_T("Bisect %d && %d"),theApp.m_nIndex+1,theApp.m_nIndex+2);
					pPopup->ModifyMenu(ID_LIST3D_BISECT,MF_BYCOMMAND,ID_LIST3D_BISECT,Buff);
					Buff.Format(_T("Circumcenter between %d && %d"),theApp.m_nIndex+1,theApp.m_nIndex+2);
					pPopup->ModifyMenu(ID_LIST3D_CIRCUMCENTERBETWEEN,MF_BYCOMMAND,ID_LIST3D_CIRCUMCENTERBETWEEN,Buff);
				} else {
					pPopup->EnableMenuItem( ID_LIST3D_BISECT   , MF_GRAYED|MF_BYCOMMAND);
					pPopup->EnableMenuItem( ID_LIST3D_CIRCUMCENTERBETWEEN   , MF_GRAYED|MF_BYCOMMAND);
				}
				if(theApp.m_LastSettings.bGotoViaDangerPlane==TRUE) {
					pPopup1 = pPopup->GetSubMenu(0);
					Buff.LoadString(IDS_Via_Danger_Plane);
					Language.LoadString(IDS_Goto_Left);
					pPopup1->ModifyMenu(ID_LIST3D_GOTO_LEFT,MF_BYCOMMAND,ID_LIST3D_GOTO_LEFT,Language + _T(" ") + Buff);
					Language.LoadString(IDS_Goto_Right);
					pPopup1->ModifyMenu(ID_LIST3D_GOTO_RIGHT,MF_BYCOMMAND,ID_LIST3D_GOTO_RIGHT,Language + _T(" ") + Buff);
					Language.LoadString(IDS_Goto_Both);
					pPopup1->ModifyMenu(ID_LIST3D_GOTO_BOTH,MF_BYCOMMAND,ID_LIST3D_GOTO_BOTH,Language + _T(" ") + Buff);
				}
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);
			}
			break;
		case SINGLE_ROBOT:
			if (menu->LoadMenu(IDR_RB_3D_LIST_ROBOT_MENU)) {
				pPopup = menu->GetSubMenu(0);
				if(theApp.m_nIndex<(m_pEditLine[theApp.m_nLine].m_nCoordL-1)) {
					Buff.Format(_T("Bisect %d && %d"),theApp.m_nIndex+1,theApp.m_nIndex+2);
					pPopup->ModifyMenu(ID_LIST3D_BISECT,MF_BYCOMMAND,ID_LIST3D_BISECT,Buff);
				} else {
					pPopup->EnableMenuItem( ID_LIST3D_BISECT   , MF_GRAYED|MF_BYCOMMAND);
					pPopup->EnableMenuItem( ID_LIST3D_CIRCUMCENTERBETWEEN   , MF_GRAYED|MF_BYCOMMAND);
				}

				
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);
			};
			break;
		case SINGLE_BRIDGE_TT_ROLLERS:
		case SINGLE_BRIDGE_TANK_WITH_R:
		case SINGLE_BRIDGE_TANK:
		case DOUBLE_BRIDGE_TANK:
			if (menu->LoadMenu(IDR_RB_3D_LIST_TANK_MENU)) {
				pPopup = menu->GetSubMenu(0);
				if(theApp.m_nIndex<(m_pEditLine[theApp.m_nLine].m_nCoordL-1)) {
					Buff.Format(_T("Bisect %d && %d"),theApp.m_nIndex+1,theApp.m_nIndex+2);
					pPopup->ModifyMenu(ID_LIST3D_BISECT,MF_BYCOMMAND,ID_LIST3D_BISECT,Buff);
				} else {
					pPopup->EnableMenuItem( ID_LIST3D_BISECT   , MF_GRAYED|MF_BYCOMMAND);
				}
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);
			};
			break;
		case TRACKED_PROBE:
			if (menu->LoadMenu(IDR_RB_3D_LIST_MENU)) {
				pPopup = menu->GetSubMenu(0);
				if (theApp.m_nIndex<(m_pEditLine[theApp.m_nLine].m_nCoordL - 1)) {
					Buff.Format(_T("Bisect %d && %d"), theApp.m_nIndex + 1, theApp.m_nIndex + 2);
					pPopup->ModifyMenu(ID_LIST3D_BISECT, MF_BYCOMMAND, ID_LIST3D_BISECT, Buff);
					Buff.Format(_T("Circumcenter between %d && %d"), theApp.m_nIndex + 1, theApp.m_nIndex + 2);
					pPopup->ModifyMenu(ID_LIST3D_CIRCUMCENTERBETWEEN, MF_BYCOMMAND, ID_LIST3D_CIRCUMCENTERBETWEEN, Buff);
				}
				else {
					pPopup->EnableMenuItem(ID_LIST3D_BISECT, MF_GRAYED | MF_BYCOMMAND);
					pPopup->EnableMenuItem(ID_LIST3D_CIRCUMCENTERBETWEEN, MF_GRAYED | MF_BYCOMMAND);
				}
				pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x, Point.y, this);
			}
			break;
		}
		
		break;
	case BRAINLINES:
		if (menu->LoadMenu(IDR_RB_3D_BRAIN_MENU)) {

			pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

		};
		break;
	case ENVELOPELINES:
		if (menu->LoadMenu(IDR_RB_3D_ENVELOPE_MENU)) {

			pPopup = menu->GetSubMenu(0);
			if(theApp.m_nIndex<(m_pEditLine[theApp.m_nLine].m_nCoordL-1)) {
				Buff.Format(_T("Bisect %d && %d"),theApp.m_nIndex+1,theApp.m_nIndex+2);
				pPopup->ModifyMenu(ID_ENVELOPE_BISECT,MF_BYCOMMAND,ID_ENVELOPE_BISECT,Buff);
			} else {
				Buff.Format(_T("Bisect %d && 1"),theApp.m_nIndex+1);
				pPopup->ModifyMenu(ID_ENVELOPE_BISECT,MF_BYCOMMAND,ID_ENVELOPE_BISECT,Buff);
			}
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

		};
		break;
	case HOLES:
		if (menu->LoadMenu(IDR_RB_3D_HOLES_MENU)) {

			pPopup = menu->GetSubMenu(0);
			switch(theApp.m_nIndex) {
			case 0:
			case 1:
			case 2:
				pPopup->EnableMenuItem( ID_HOLES_SWAP0   , MF_GRAYED|MF_BYCOMMAND);
				pPopup->EnableMenuItem( ID_HOLES_SWAP1   , MF_GRAYED|MF_BYCOMMAND);
				pPopup->EnableMenuItem( ID_HOLES_INVERTNORMAL   , MF_GRAYED|MF_BYCOMMAND);
				break;
			case 3:
				pPopup->ModifyMenu(ID_HOLES_SWAP0,MF_BYCOMMAND,ID_HOLES_SWAP0,_T("Swap with Floating"));
				pPopup->ModifyMenu(ID_HOLES_SWAP1,MF_BYCOMMAND,ID_HOLES_SWAP1,_T("Swap with Third"));
				break;
			case 4:
				pPopup->ModifyMenu(ID_HOLES_SWAP0,MF_BYCOMMAND,ID_HOLES_SWAP0,_T("Swap with Fixed"));
				pPopup->ModifyMenu(ID_HOLES_SWAP1,MF_BYCOMMAND,ID_HOLES_SWAP1,_T("Swap with Third"));
				break;
			case 5:
				pPopup->ModifyMenu(ID_HOLES_SWAP0,MF_BYCOMMAND,ID_HOLES_SWAP0,_T("Swap with Fixed"));
				pPopup->ModifyMenu(ID_HOLES_SWAP1,MF_BYCOMMAND,ID_HOLES_SWAP1,_T("Swap with Floating"));
				break;
			}
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);
		};
		break;
	case SURFACELINES:
		if (menu->LoadMenu(IDR_RB_LIST_SURFACE_MENU)) {

			pPopup = menu->GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

		};
		break;
	}
	delete menu;

	*pResult = 0;
}

void C3DViewEditDlg::OnList3dGotoLeft()
{
	CCoord	Cp,CpSurface,CpHead;

	if(IsCoordinateValid() == false) return;

	theApp.m_Thread.StopCurrentThreadAction();
	CpSurface = m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex];
	theApp.m_UtUser.SetAllUt(CpSurface,PROFILE->m_nHeader);

//	CpSurface.Side0.fR = CpHead.Side0.fR;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		if(theApp.m_LastSettings.bGotoViaDangerPlane == FALSE) {

			theApp.m_Motors.GetHeadPos(&CpHead);

			theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,PORTSIDE,TRUE,TRUE, NULL, CURRENT_FL);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,STARBOARD,TRUE,TRUE, NULL, CURRENT_FL);

			//Keep right side where it is
			switch(theApp.m_nSide0Orientation) {
			case 0:	Cp.CopySide(STARBOARD,CpHead);
				break;
			case 1:	Cp.CopySide(PORTSIDE,CpHead);
				break;
			}

			theApp.m_Thread.m_CpMoveTo = Cp;
			theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
		} else {

			//Keep right side where it is
			switch(theApp.m_nSide0Orientation) {
			case 0:	theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide = 0;
				break;
			case 1:	theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide = 1;
				break;
			}
			theApp.m_Thread.m_CpMoveTo = CpSurface;
			theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
		}
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TANK_5AXIS:
	case TANK_6AXIS:
	case TANK_2AXIS:
		theApp.m_Motors.IntelligentGotoSurfaceCoord(&CpSurface, 1, theApp.m_LastSettings.nDangerPlaneMoveMask);
		break;
	case TRACKED_PROBE:
		break;
	}

}

void C3DViewEditDlg::OnList3dGotoRight()
{
	CCoord	Cp,CpSurface,CpHead;
	CString Buff;

	if(IsCoordinateValid() == false) return;

	if(theApp.m_ServoUser.AreMotorsPowerd()==FALSE) return;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.GetHeadPos(&CpHead);

	CpSurface = m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex];

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		if(theApp.m_LastSettings.bGotoViaDangerPlane == FALSE) {
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,PORTSIDE,TRUE,TRUE, NULL, CURRENT_FL);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,STARBOARD,TRUE,TRUE, NULL, CURRENT_FL);

			//Keep left side where it is
			switch(theApp.m_nSide0Orientation) {
			case 0:	Cp.CopySide(PORTSIDE,CpHead);
				break;
			case 1:	Cp.CopySide(STARBOARD,CpHead);
				break;
			}

			theApp.m_Thread.m_CpMoveTo = Cp;
			theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
		} else {

			//Keep left side where it is
			switch(theApp.m_nSide0Orientation) {
			case 0:	theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide = 1;
				break;
			case 1:	theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide = 0;
				break;
			}
			theApp.m_Thread.m_CpMoveTo = CpSurface;
			theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
		}
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		theApp.m_Motors.IntelligentGotoSurfaceCoord(&CpSurface, 2, theApp.m_LastSettings.nDangerPlaneMoveMask);
		break;
	case TRACKED_PROBE:
		break;
	}
}

void C3DViewEditDlg::OnList3dGotoBoth()
{
	CCoord	Cp,CpSurface,CpHead;
	CString Buff;

	if(IsCoordinateValid() == false) return;
	if(theApp.m_ServoUser.AreMotorsPowerd()==FALSE) return;

	theApp.m_Thread.StopCurrentThreadAction();

	theApp.m_Motors.GetHeadPos(&CpHead);

	CpSurface = m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex];


	switch(theApp.m_Tank.nScannerDescription) {
	default:
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
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		theApp.m_Motors.IntelligentGotoSurfaceCoord(&CpSurface, 3, theApp.m_LastSettings.nDangerPlaneMoveMask);
		break;
	case TRACKED_PROBE:
		break;
	};

}


void C3DViewEditDlg::OnEditBrainLines()
{
	
	if (m_pBrainEditDlg == NULL) {
		CloseAllChildren();
		CRect rr;
		m_wndFileToolBar.GetWindowRect(rr);
		m_pBrainEditDlg = new CBrainEditDlg(this,(CWnd**)&m_pBrainEditDlg,rr.left,rr.bottom);
		m_pBrainEditDlg->Create(CBrainEditDlg::IDD, this);
	}
	theApp.m_nEditLineType = BRAINLINES;
	InitializeEditLine();
	CreateColumns();
	FillList();
	SetToolBarCheckedState();
	InvalidateCoordArrayPane(0xffff);

}

void C3DViewEditDlg::OnUpdateEditBrainLines(CCmdUI* pCmdUI)
{
	m_pBrainEditDlg > NULL ? m_wndEditToolBar.SetButtonStyle(2,TBBS_CHECKED) : m_wndEditToolBar.SetButtonStyle(2,0);
}


void C3DViewEditDlg::OnEditHoles()
{
	if (m_pHolesEditDlg == NULL) {
		CloseAllChildren();
		CRect rr;
		m_wndFileToolBar.GetWindowRect(rr);
		m_pHolesEditDlg = new CHolesEditDlg(this,(CWnd**)&m_pHolesEditDlg,rr.left,rr.bottom);
		m_pHolesEditDlg->Create(CHolesEditDlg::IDD, this);
	}
	theApp.m_nEditLineType=HOLES;
	CreateColumns();
	FillList();
	SetToolBarCheckedState();
	InvalidateCoordArrayPane(0xffff);
}

void C3DViewEditDlg::OnUpdateEditHoles(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck( m_pHolesEditDlg > NULL);

}


void C3DViewEditDlg::OnEditTransform()
{
	if (m_pTranslationDlg == NULL) {
		CloseAllChildren();
		CRect rr;
		m_wndFileToolBar.GetWindowRect(rr);
		m_pTranslationDlg = new CTranslationDlg(this,(CWnd**)&m_pTranslationDlg,rr.left,rr.bottom);
		m_pTranslationDlg->Create(CTranslationDlg::IDD, this);
	}
	theApp.m_nEditLineType=TRANSFORM;
	SetToolBarCheckedState();
	CreateColumns();
	FillList();

}

void C3DViewEditDlg::DrawProjectedSurfaceInGreyMode(CPaintDC *pDC, CRect Rect, int nDevice)
{
	CPoint	point;
	int	ii,cc;
	CCoord	Cp;
	CPen penBlack(PS_SOLID,1,RGB(0,0,0));
	CPen* pOldPen=pDC->SelectObject(&penBlack);
	CPen* pPen[256];
	float fX0p;
	float fX1p;
	float fStep;
	int nLine;
//	float fMin = theApp.m_LastSettings.fSurfaceMin;
//	float fMax = theApp.m_LastSettings.fSurfaceMax;
	float fMin = 0.0f;
	float fMax = 1.0f;
	CPolyCoord* pLine;
	int	nSurfaceDisplayAxis = LINEAR_LINE;

	if(fMax<=fMin) fMax = fMin+1.0f;


	for(ii=0;ii<256;ii++) {
		pPen[ii]=new CPen(PS_SOLID,1,RGB(ii,ii,ii));
	}

	for(nLine=0;nLine<PROFILE->m_nScanLineL;nLine++) {

		pLine = &PROFILE->m_ScanLine[nLine];
		fX0p = pLine->m_pCp[0].Side[0].fX;
		fX1p = pLine->m_pCp[pLine->m_nCoordL-1].Side[0].fX;
		fStep = (fX1p - fX0p) / 50.0f;

		switch(pLine->m_nStyle) {
		case LINEAR_LINE:
			for(ii=0;ii<pLine->m_nCoordL;ii++) {
				switch(nSurfaceDisplayAxis=2) {
				default:	cc = (int)(((pLine->m_pCp[ii].Side[0].fI-fMin) * 256.0f) / (fMax-fMin));
					break;
				case 1:	cc = (int)(((pLine->m_pCp[ii].Side[0].fJ-fMin) * 256.0f) / (fMax-fMin));
					break;
				case 2:	cc = (int)(((pLine->m_pCp[ii].Side[0].fK-fMin) * 256.0f) / (fMax-fMin));
					break;
				case 3:	cc = (int)(((pLine->m_pCp[ii].Side[0].fY-fMin) * 256.0f) / (fMax-fMin));
					break;
				}
				if(cc<0) cc=0;
				if(cc>255) cc=255;
				pDC->SelectObject(pPen[cc]);
				WorldToClient(pLine->GetPtrCoord(ii),Rect,&point);
				ii==0 ?	pDC->MoveTo(point) : pDC->LineTo(point);
			}
			break;
			/*
		case SPLINE_LINE:
			if(fStep>0.0) {
				for(dXp=fX0p ;dXp <= fX1p; dXp+=fStep) {
					m_PolyLine.Splint(m_pProfile->m_PolyLine[nLine].m_dX[theApp.m_nSide],m_pProfile->m_PolyLine[nLine].m_dZ[theApp.m_nSide],m_pProfile->m_PolyLine[nLine].m_dXZdiff[0],m_pProfile->m_PolyLine[nLine].m_nCoordL,dXp,&dYp);
					Cp.dXp[theApp.m_nSide]=dXp;
					Cp.dZp[theApp.m_nSide]=dYp;
					WorldToRect(Cp,Rect,theApp.m_nSide,&point);
					dXp==fX0p ?	pDC->MoveTo(point) : pDC->LineTo(point) ;
				}
				m_PolyLine.Splint(m_pProfile->m_PolyLine[nLine].m_dX[theApp.m_nSide],m_pProfile->m_PolyLine[nLine].m_dZ[theApp.m_nSide],m_pProfile->m_PolyLine[nLine].m_dXZdiff[theApp.m_nSide],m_pProfile->m_PolyLine[nLine].m_nCoordL,fX1p,&dYp);
				Cp.dXp[theApp.m_nSide]=dXp;
				Cp.dZp[theApp.m_nSide]=dYp;
				WorldToRect(Cp,Rect,theApp.m_nSide,&point);
				dXp==fX0p ?	pDC->MoveTo(point) : pDC->LineTo(point) ;
			}
			break;
		case POLYNOMIAL_LINE:
			if(fStep>0.0) {
				for(dXp=fX0p ;dXp <= fX1p; dXp+=fStep) {
					m_pProfile->m_PolyLine[nLine].PolynomialInterpolation(m_pProfile->m_nPerspective,theApp.m_nSide,dXp,&dYp);
					Cp.dXp[theApp.m_nSide]=dXp;
					Cp.dZp[theApp.m_nSide]=dYp;
					WorldToRect(Cp,Rect,theApp.m_nSide,&point);
					dXp==fX0p ?	pDC->MoveTo(point) : pDC->LineTo(point) ;
				}
			}
			break;
		case QUADRATIC_LINE:
			if(fStep>0.0) {
				for(dXp=fX0p ;dXp <= fX1p; dXp+=fStep) {
					m_pProfile->m_PolyLine[nLine].QuadraticInterpolation(m_pProfile->m_nPerspective,theApp.m_nSide,dXp,&dYp);
					Cp.dXp[theApp.m_nSide]=dXp;
					Cp.dZp[theApp.m_nSide]=dYp;
					WorldToRect(Cp,Rect,theApp.m_nSide,&point);
					dXp==fX0p ?	pDC->MoveTo(point) : pDC->LineTo(point) ;
				}
			}
			break;
			*/
		}
	}

	for(ii=0;ii<256;ii++) {
		delete pPen[ii];
	}

	pDC->SelectObject(pOldPen);


}


void C3DViewEditDlg::OnToolsCopy()
{
	CCopyCoordsDlg dlg;

	if(dlg.DoModal()==IDOK) {
		FillList();
		UpdateAllControls();
		Invalidate(FALSE);
	}
}

void C3DViewEditDlg::OnToolsEnvelope()
{
	CEnvelopeToolsDlg dlg(this);

	if(dlg.DoModal()==IDOK) {
		FillList();
		UpdateAllControls();
		Invalidate(FALSE);
	}

}


void C3DViewEditDlg::OnToolsTaughtlines()
{
	OnEditTaughtLines();
	m_pTaughtLinesEditDlg->ToolTaughtLinesFromParent();

/*	CTaughtToolsDlg dlg;

	if(dlg.DoModal()==IDOK) {
		FillList();
		UpdateAllControls();
		Invalidate(FALSE);
	}
	*/
}

void C3DViewEditDlg::SaveThumbNail(CUSLFile* pFile)
{
	CDC* pDC = GetDC();
	CRect rr;
	CDC  dcMem;
	CBitmap bmp;
	COLORREF *pArray;
#define NUMBERENTRIESPALETTE 256

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	rr.DeflateRect(2,2);

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

	pFile->Write(&BmpFileHeader, sizeof(BITMAPFILEHEADER));
	pFile->Write(&BmpHeader, sizeof(BITMAPINFOHEADER));
	pFile->Write(pArray, rr.Width() * rr.Height() * (sizeof COLORREF));

	delete pArray;
	ReleaseDC(pDC);
}



void C3DViewEditDlg::InitializeEditLine()
{
	m_pEditLine=NULL;
	m_nEditLineL=0;
	switch(theApp.m_nEditLineType) {
	case TAUGHTLINES: m_pEditLine = PROFILE->m_TaughtLine;
		m_nEditLineL = PROFILE->m_nTaughtLineL;
		break;
	case SCANLINES: m_pEditLine = PROFILE->m_ScanLine;
		m_nEditLineL = PROFILE->m_nScanLineL;
		break;
	case BRAINLINES: m_pEditLine = PROFILE->m_BrainLine;
		m_nEditLineL = PROFILE->m_nBrainLineL;
		break;
	case ENVELOPELINES: m_pEditLine = PROFILE->m_EnvelopeLine;
		m_nEditLineL = PROFILE->m_nEnvelopeLineL;
		break;
	case SURFACELINES: m_pEditLine = PROFILE->m_SurfaceLine;
		m_nEditLineL = PROFILE->m_nSurfaceLineL;
		break;
	case WAYLINES: m_pEditLine = PROFILE->m_WayPoints;
		m_nEditLineL = PROFILE->m_nWayPointsL;
		break;
	case OUTSIDELINES: m_pEditLine = PROFILE->m_LineBlade[OUTSIDE];
		m_nEditLineL = PROFILE->m_nLineBladeL[OUTSIDE];
		break;
	case INSIDELINES: m_pEditLine = PROFILE->m_LineBlade[INSIDE];
		m_nEditLineL = PROFILE->m_nLineBladeL[INSIDE];
		break;
	}
	if(theApp.m_nLine>=m_nEditLineL) theApp.m_nLine = m_nEditLineL-1;
	if(theApp.m_nLine<0) theApp.m_nLine = 0;
	if(m_pEditLine)
		if(theApp.m_nIndex>=m_pEditLine[theApp.m_nLine].m_nCoordL) theApp.m_nIndex=m_pEditLine[theApp.m_nLine].m_nCoordL-1;
	if(theApp.m_nIndex<0) theApp.m_nIndex = 0;

}

void C3DViewEditDlg::OnButtonDeleteCoordinate()
{
	DeleteCoordinate();
}


void C3DViewEditDlg::On3dviewDeletecoordinate()
{
	DeleteCoordinate();
}

void C3DViewEditDlg::DeleteCoordinate()
{
	if(m_pEditLine==NULL) return;
	

	if(m_pEditLine[theApp.m_nLine].m_nCoordL == 1) {
		if(MessageBox(_T("This is the last vertex on this line. Deleteing this point will remove the entire line\r\nDo you wish to continue ?"),_T("Warning"),MB_YESNO|MB_ICONEXCLAMATION)!=IDYES) return;
		PROFILE->DeleteLine(theApp.m_nEditLineType,theApp.m_nLine);
		UpdateAllControls();
	} else {
		m_pEditLine[theApp.m_nLine].DeleteCoords(theApp.m_nIndex,theApp.m_nIndex);
	}
	InitializeEditLine();

	FillList();
	Invalidate(FALSE);
	InvalidateCoordArrayPane(_FILL_LIST);
}

void C3DViewEditDlg::OnList3dDelete()
{
	
	int nStartIndex,nFinishIndex;

	POSITION pos = m_listCoord.GetFirstSelectedItemPosition();
	nStartIndex=(int)pos;
	nFinishIndex=nStartIndex=m_listCoord.GetNextSelectedItem(pos);
	while(pos) {
		nFinishIndex=m_listCoord.GetNextSelectedItem(pos);
	}

	if((nStartIndex==0) && ((nFinishIndex-nStartIndex+1) == m_pEditLine[theApp.m_nLine].m_nCoordL)) {
		if(MessageBox(_T("This is the last vertex on this line. Deleteing this point will remove the entire line\r\nDo you wish to continue ?"),_T("Warning"),MB_YESNO|MB_ICONEXCLAMATION)!=IDYES) return;
		PROFILE->DeleteLine(theApp.m_nEditLineType,theApp.m_nLine);
		UpdateAllControls();
		InvalidateCoordArrayPane(_FILL_LIST | _FILL_LINE_LIST);
	} else {
		m_pEditLine[theApp.m_nLine].DeleteCoords(nStartIndex,nFinishIndex);
		InvalidateCoordArrayPane(_FILL_LIST);
	}
	InitializeEditLine();

	FillList();
	Invalidate(FALSE);

}

void C3DViewEditDlg::On3dviewDeleteline()
{
	
	PROFILE->DeleteLine(theApp.m_nEditLineType,theApp.m_nLine);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
	InvalidateCoordArrayPane(_FILL_LIST | _FILL_LINE_LIST);
}

void C3DViewEditDlg::On3dviewCopyline()
{
	theApp.m_CopyLine.Zero();
	theApp.m_CopyLine = &m_pEditLine[theApp.m_nLine];
}

void C3DViewEditDlg::On3dviewPasteline()
{
	
	PROFILE->AddLine(theApp.m_nEditLineType,&theApp.m_CopyLine,PROFILE->m_nSlowAxis);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
	InvalidateCoordArrayPane(_FILL_LIST | _FILL_LINE_LIST);
}

void C3DViewEditDlg::On3dviewDecimateline()
{
	CDecimateDlg dlg;

	if(dlg.DoModal() == IDOK) {
		
		m_pEditLine[theApp.m_nLine].Decimate(dlg.m_nMaxPts);
		FillList();
		Invalidate(FALSE);
		InvalidateCoordArrayPane(_FILL_LIST);
	}
}


void C3DViewEditDlg::On3dviewDecimatealllines()
{
	CDecimateDlg dlg;
	int	nn;

	if(dlg.DoModal() == IDOK) {
		for(nn=0;nn<m_nEditLineL;nn++) {
			m_pEditLine[nn].Decimate(dlg.m_nMaxPts);
			FillList();
			Invalidate(FALSE);
			
			InvalidateCoordArrayPane(_FILL_LIST);
		}
	}
}

void C3DViewEditDlg::OnList3dDecimate()
{
	On3dviewDecimateline();

}





void C3DViewEditDlg::OnList3dCopy()
{
	int nIndex;
	CString Buff;
	CCoord Cp;

	theApp.m_CopyLine.Zero();

	POSITION pos = m_listCoord.GetFirstSelectedItemPosition();
	nIndex=(int)pos;
	nIndex=m_listCoord.GetNextSelectedItem(pos);
	switch(theApp.m_nEditLineType) {
	case TAUGHTLINES:
	case SCANLINES:
	case BRAINLINES:
	case ENVELOPELINES:
	case SURFACELINES:
	case OUTSIDELINES:
	case INSIDELINES:

		theApp.m_CopyLine.Add(m_pEditLine[theApp.m_nLine].m_pCp[nIndex]);
		while(pos) {
			nIndex=m_listCoord.GetNextSelectedItem(pos);
			theApp.m_CopyLine.Add(m_pEditLine[theApp.m_nLine].m_pCp[nIndex]);
		}
		break;
	case HOLES:
		switch(theApp.m_nIndex) {
			break;
		case 0:
		case 1:
		case 2:	theApp.m_CopyLine.Add(PROFILE->m_CpMeasuredHole[theApp.m_nIndex]);
			break;
		case 3:
		case 4:
		case 5: theApp.m_CopyLine.Add(PROFILE->m_CpCadHole[theApp.m_nIndex-3]);
			break;
		}
		break;
	}
	theApp.m_CopyLine.GetCoord(0,&Cp);
	Buff.Format(L"X:%.02f Y:%.02f, Z:%.02f",Cp.Side0.fX,Cp.Side0.fY,Cp.Side0.fZ);
	theApp.SetStringOnToClipboard(Buff);

}

void C3DViewEditDlg::OnList3dPaste()
{
	
	int nn;

	switch(theApp.m_nEditLineType) {
	case TAUGHTLINES:
	case SCANLINES:
	case BRAINLINES:
	case ENVELOPELINES:
	case SURFACELINES:
	case OUTSIDELINES:
	case INSIDELINES:
		if((theApp.m_CopyLine.m_nCoordL==1) && (m_pEditLine[theApp.m_nLine].m_nCoordL<theApp.m_nIndex)) {
			m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex] = theApp.m_CopyLine.m_pCp[0];
		} else {
			for(nn=0;nn<theApp.m_CopyLine.m_nCoordL;nn++) {
				m_pEditLine[theApp.m_nLine].Add(theApp.m_CopyLine.m_pCp[nn]);
			}
		}
		break;
	case HOLES:
		switch(theApp.m_nIndex) {
			break;
		case 0:
		case 1:
		case 2:	PROFILE->m_CpMeasuredHole[theApp.m_nIndex] = theApp.m_CopyLine.m_pCp[0];
			break;
		case 3:
		case 4:
		case 5: PROFILE->m_CpCadHole[theApp.m_nIndex-3] = theApp.m_CopyLine.m_pCp[0];
			break;
		}
		break;
	}
	FillList();
	Invalidate(FALSE);
	InvalidateCoordArrayPane(_FILL_LIST);

}


void C3DViewEditDlg::OnList3dPastenormals() 
{
	
	switch(theApp.m_nEditLineType) {
	case TAUGHTLINES:
	case SCANLINES:
	case BRAINLINES:
	case ENVELOPELINES:
	case SURFACELINES:
	case OUTSIDELINES:
	case INSIDELINES:
		if((theApp.m_CopyLine.m_nCoordL==1) && (theApp.m_nIndex<m_pEditLine[theApp.m_nLine].m_nCoordL)) {
			m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.norm = theApp.m_CopyLine.m_pCp[0].Side0.norm;
		}
		break;
	case HOLES:
		switch(theApp.m_nIndex) {
			break;
		case 0:
		case 1:
		case 2:	PROFILE->m_CpMeasuredHole[theApp.m_nIndex].Side0.norm = theApp.m_CopyLine.m_pCp[0].Side0.norm;
			break;
		case 3:
		case 4:
		case 5: PROFILE->m_CpCadHole[theApp.m_nIndex-3].Side0.norm = theApp.m_CopyLine.m_pCp[0].Side0.norm;
			break;
		}
		break;
	}
	FillList();
	Invalidate(FALSE);
	InvalidateCoordArrayPane(_FILL_LIST);
}



void C3DViewEditDlg::OnToolsInvertallnormals()
{
	
	PROFILE->InvertAllNormals();
	Invalidate(FALSE);
	InvalidateCoordArrayPane(_FILL_LIST);
}



void C3DViewEditDlg::OnFileNew()
{
	if(PROFILE->CheckNewProfileAllowed()!=TRUE) return;

	PROFILE->New();
	FillList();
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(0xffff);

}

void C3DViewEditDlg::OnButtonAddCoordinate()
{
	AddInsertCoordinate(0);
}

void C3DViewEditDlg::OnButtonInsertCoordinate() 
{
	AddInsertCoordinate(1);
}

void C3DViewEditDlg::AddInsertCoordinate(int nAddInsert)
{
	CCoord	CpSurface;
	CpSurface.Zero();

	if (theApp.m_Tank.nScannerDescription == TRACKED_PROBE) {
		theApp.m_pTracker->GetSurfacePos(&CpSurface);
	}
	else {
		theApp.m_Motors.GetSurfacePos(&CpSurface);
	}

	if((theApp.m_LastSettings.nManipulatorType[0] == ROTATE_THETA_HORIZONTAL) || (theApp.m_LastSettings.nManipulatorType[0] == ROTATE_THETA_VERTICAL)) {
		if(fabs(CpSurface.Side0.fYt) < 0.02f) CpSurface.Side0.fYt = 0.02f;
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
	}
	if((theApp.m_LastSettings.nManipulatorType[1] == ROTATE_THETA_HORIZONTAL) || (theApp.m_LastSettings.nManipulatorType[1] == ROTATE_THETA_VERTICAL)) {
		if(fabs(CpSurface.Side1.fYt) < 0.02f) CpSurface.Side1.fYt = 0.02f;
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
	}
	theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface,PORTSIDE);
	theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface,STARBOARD);
	theApp.m_Kinematics.GetComponentThickness(&CpSurface);
	theApp.m_UtUser.GetAllUt(&CpSurface);

	if(m_pEditLine==NULL) {
		switch(theApp.m_nEditLineType) {
		case TAUGHTLINES:
			PROFILE->m_TaughtLine = new CPolyCoord[PROFILE->m_nTaughtLineL=1];
			PROFILE->m_TaughtLine[theApp.m_nLine=0].Add(CpSurface);
			break;
		case SCANLINES:
			PROFILE->m_ScanLine = new CPolyCoord[PROFILE->m_nScanLineL=1];
			PROFILE->m_ScanLine[theApp.m_nLine=0].Add(CpSurface);
			break;
		case BRAINLINES:
//			PROFILE->m_BrainLine = new CPolyCoord[PROFILE->m_nBrainLineL=1];
//			PROFILE->m_BrainLine[theApp.m_nLine=0].Add(CpSurface);
			break;
		case ENVELOPELINES:
			PROFILE->m_EnvelopeLine = new CPolyCoord[PROFILE->m_nEnvelopeLineL=1];
			PROFILE->m_EnvelopeLine[theApp.m_nLine=0].Add(CpSurface);
			break;
		case OUTSIDELINES:
			PROFILE->m_LineBlade[OUTSIDE] = new CPolyCoord[PROFILE->m_nLineBladeL[OUTSIDE]=1];
			PROFILE->m_LineBlade[OUTSIDE][theApp.m_nLine=0].Add(CpSurface);
			break;
		case INSIDELINES:
			PROFILE->m_LineBlade[INSIDE] = new CPolyCoord[PROFILE->m_nLineBladeL[INSIDE]=1];
			PROFILE->m_LineBlade[INSIDE][theApp.m_nLine=0].Add(CpSurface);
			break;
		}
		InitializeEditLine();
	} else {
		switch(nAddInsert) {
		case 0: m_pEditLine[theApp.m_nLine].Add(CpSurface);
			break;
		case 1: m_pEditLine[theApp.m_nLine].InsertCoord(CpSurface);
			break;
		};
	}

	UpdateAllControls();
	FillList();
	Centerimage();
	FindAllLimits(true);

	Invalidate(FALSE);
	if (m_pEditLine[theApp.m_nLine].GetSize() == 1) {
		InvalidateCoordArrayPane(_FILL_LIST | _FILL_LINE_LIST);
	}
	else {
		InvalidateCoordArrayPane(_FILL_LIST);
	}
}

void C3DViewEditDlg::OnButtonNewLine()
{
	
	CCoord	CpSurface;
	CPolyCoord CpLine;
	CpSurface.Zero();

	if (theApp.m_Tank.nScannerDescription == TRACKED_PROBE) {
		theApp.m_pTracker->GetSurfacePos(&CpSurface);
	}
	else {
		theApp.m_Motors.GetSurfacePos(&CpSurface);
	}
	if((theApp.m_LastSettings.nManipulatorType[0] == ROTATE_THETA_HORIZONTAL) || (theApp.m_LastSettings.nManipulatorType[0] == ROTATE_THETA_VERTICAL)) {
		if(fabs(CpSurface.Side0.fYt) < 0.02f) CpSurface.Side0.fYt = 0.02f;
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
	}
	if((theApp.m_LastSettings.nManipulatorType[1] == ROTATE_THETA_HORIZONTAL) || (theApp.m_LastSettings.nManipulatorType[1] == ROTATE_THETA_VERTICAL)) {
		if(fabs(CpSurface.Side1.fYt) < 0.02f) CpSurface.Side1.fYt = 0.02f;
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
	}
	theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface,PORTSIDE);
	theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface,STARBOARD);
	theApp.m_Kinematics.GetComponentThickness(&CpSurface);
	theApp.m_UtUser.GetAllUt(&CpSurface);

	CpLine.Add(CpSurface);
	theApp.m_nLine=PROFILE->AddLine(theApp.m_nEditLineType, &CpLine,PROFILE->m_nSlowAxis);
	InitializeEditLine();

	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
	InvalidateCoordArrayPane(_FILL_LIST | _FILL_LINE_LIST);

}



void C3DViewEditDlg::OnList3dEdit()
{
	
	CEditCoordDlg dlg(this);
	int nStartIndex,nFinishIndex,nn;

	POSITION pos = m_listCoord.GetFirstSelectedItemPosition();
	nStartIndex=(int)pos;
	nStartIndex=nFinishIndex=m_listCoord.GetNextSelectedItem(pos);
	while(pos) {
		nFinishIndex=m_listCoord.GetNextSelectedItem(pos);
	}
	if(dlg.DoModal()==IDOK) {
		for(nn=nStartIndex;nn<=nFinishIndex;nn++) {
			m_pEditLine[theApp.m_nLine].m_pCp[nn].fPos[dlg.m_nAxis] = dlg.m_fValue;
		}
		m_pEditLine[theApp.m_nLine].m_bModified = TRUE;
	}
	FillList();
	Invalidate(FALSE);
	InvalidateCoordArrayPane(_FILL_LIST);

}

void C3DViewEditDlg::OnList3dTranslate()
{
	CEditCoordDlg dlg(this);
	int nStartIndex,nFinishIndex,nn;

	POSITION pos = m_listCoord.GetFirstSelectedItemPosition();
	nStartIndex=(int)pos;
	nStartIndex=nFinishIndex=m_listCoord.GetNextSelectedItem(pos);
	while(pos) {
		nFinishIndex=m_listCoord.GetNextSelectedItem(pos);
	}
	dlg.SetText(_T("Translate"),L"By");
	if(dlg.DoModal()==IDOK) {
		for(nn=nStartIndex;nn<=nFinishIndex;nn++) {
			m_pEditLine[theApp.m_nLine].m_pCp[nn].Side0.fPos[dlg.m_nAxis] += dlg.m_fValue;
			m_pEditLine[theApp.m_nLine].m_pCp[nn].Side1.fPos[dlg.m_nAxis] += dlg.m_fValue;
		}
		m_pEditLine[theApp.m_nLine].m_bModified = TRUE;
	}
	FillList();
	Invalidate(FALSE);
}

void C3DViewEditDlg::OnList3dModifyleft()
{
	CCoord CpSurface,CpHead;
	int	nSide;

	switch(theApp.m_nSide0Orientation) {
	case 0: nSide = PORTSIDE;
		break;
	case 1: nSide = STARBOARD;
		break;
	}

	theApp.m_Motors.GetHeadPos(&CpHead);
	switch(theApp.m_LastSettings.nPositionDisplayMode) {
	default:
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,nSide,TRUE,TRUE, CURRENT_FL);
		break;
	case 3:
		theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[nSide]);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,nSide,TRUE,FALSE, CURRENT_FL);
		break;
	}
	theApp.m_Kinematics.NormalsFromTips(&CpSurface,nSide);
	m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side[nSide] = CpSurface.Side[nSide];
	m_pEditLine[theApp.m_nLine].m_bModified = TRUE;
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST);

}

void C3DViewEditDlg::OnList3dModifyright()
{
	CCoord CpSurface,CpHead;
	int	nSide;

	switch(theApp.m_nSide0Orientation) {
	case 0: nSide = STARBOARD;
		break;
	case 1: nSide = PORTSIDE;
		break;
	}

	theApp.m_Motors.GetHeadPos(&CpHead);
	switch(theApp.m_LastSettings.nPositionDisplayMode) {
	default:
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,nSide,TRUE,TRUE, CURRENT_FL);
		break;
	case 3:
		theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[nSide]);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,nSide,TRUE,FALSE, CURRENT_FL);
		break;
	}
	theApp.m_Kinematics.NormalsFromTips(&CpSurface,nSide);
	m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side[nSide] = CpSurface.Side[nSide];
	m_pEditLine[theApp.m_nLine].m_bModified = TRUE;
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST);
}

void C3DViewEditDlg::OnList3dModifyboth()
{
	CCoord	CpSurface;
	CpSurface.Zero();

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	if((theApp.m_LastSettings.nManipulatorType[0] == ROTATE_THETA_HORIZONTAL) || (theApp.m_LastSettings.nManipulatorType[0] == ROTATE_THETA_VERTICAL)) {
		if(fabs(CpSurface.Side0.fYt) < 0.02f) CpSurface.Side0.fYt = 0.02f;
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
	}
	if((theApp.m_LastSettings.nManipulatorType[1] == ROTATE_THETA_HORIZONTAL) || (theApp.m_LastSettings.nManipulatorType[1] == ROTATE_THETA_VERTICAL)) {
		if(fabs(CpSurface.Side1.fYt) < 0.02f) CpSurface.Side1.fYt = 0.02f;
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
	}
	theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface,PORTSIDE);
	theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface,STARBOARD);
	theApp.m_Kinematics.GetComponentThickness(&CpSurface);
	theApp.m_UtUser.GetAllUt(&CpSurface);

	m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex] = CpSurface;
	m_pEditLine[theApp.m_nLine].m_bModified = TRUE;
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST);
}

void C3DViewEditDlg::OnTankModifyxyzijk() 
{
	CCoord CpSurface,CpHead;
	int	nSide;

	switch(theApp.m_nSide0Orientation) {
	case 0: nSide = PORTSIDE;
		break;
	case 1: nSide = STARBOARD;
		break;
	}

	theApp.m_Motors.GetHeadPos(&CpHead);
	switch(theApp.m_LastSettings.nPositionDisplayMode) {
	default:
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,nSide,TRUE,TRUE, CURRENT_FL);
		break;
	case 3:
		theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[nSide]);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,nSide,TRUE,FALSE, CURRENT_FL);
		break;
	}
	theApp.m_Kinematics.NormalsFromTips(&CpSurface,nSide);
	m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side[nSide] = CpSurface.Side[nSide];
	m_pEditLine[theApp.m_nLine].m_bModified = TRUE;
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST);
}

void C3DViewEditDlg::OnTankModifyut() 
{
	CCoord Cp;

	theApp.m_UtUser.GetAllUt(&Cp);
	for(int ii=0;ii<6;ii++) {
		m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].nGain[0] = Cp.nGain[0];
		m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].nGain[1] = Cp.nGain[1];
		for(ii=0;ii<6;ii++) {
			m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].nDelay[ii] = Cp.nDelay[ii];
			m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].nWidth[ii] = Cp.nWidth[ii];
		}
	}
	m_pEditLine[theApp.m_nLine].m_bModified = TRUE;
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST);
}

void C3DViewEditDlg::OnList3dModifyijk()
{
	CCoord CpSurface,CpHead;

	theApp.m_Motors.GetHeadPos(&CpHead);
	switch(theApp.m_LastSettings.nPositionDisplayMode) {
	default:
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE, CURRENT_FL);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE, CURRENT_FL);
		break;
	case 3:
		theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
		theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE, CURRENT_FL);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,FALSE, CURRENT_FL);
		break;
	}
	theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
	theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
	m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fI = CpSurface.Side0.fI;
	m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fJ = CpSurface.Side0.fJ;
	m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fK = CpSurface.Side0.fK;
	m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fXt = CpSurface.Side0.fXt;
	m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side0.fYt = CpSurface.Side0.fYt;
	m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side1.fI = CpSurface.Side0.fI;
	m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side1.fJ = CpSurface.Side0.fJ;
	m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side1.fK = CpSurface.Side0.fK;
	m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side1.fXt = CpSurface.Side0.fXt;
	m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side1.fYt = CpSurface.Side0.fYt;

	m_pEditLine[theApp.m_nLine].m_bModified = TRUE;
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST);

}

void C3DViewEditDlg::OnList3dModifynormalsonnearest() 
{
	CCoord CpSurface;
	int nIndex;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	PROFILE->FindNearestCoordinate(theApp.m_nEditLineType,CpSurface,&theApp.m_nLine,&nIndex,0);

	m_pEditLine[theApp.m_nLine].m_pCp[nIndex].Side0.fI = CpSurface.Side0.fI;
	m_pEditLine[theApp.m_nLine].m_pCp[nIndex].Side0.fJ = CpSurface.Side0.fJ;
	m_pEditLine[theApp.m_nLine].m_pCp[nIndex].Side0.fK = CpSurface.Side0.fK;
	
	m_pEditLine[theApp.m_nLine].m_bModified = TRUE;
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST);
}



void C3DViewEditDlg::OnEnvelopeTranslate()
{
	CTranslatDlg dlg;
	int nIndex;
	POSITION pos = m_listCoord.GetFirstSelectedItemPosition();

	if(dlg.DoModal() == IDOK) {

		while(pos) {
			nIndex=m_listCoord.GetNextSelectedItem(pos);
			m_pEditLine[theApp.m_nLine].m_pCp[nIndex].fPos[0] += dlg.m_fXDev;
			m_pEditLine[theApp.m_nLine].m_pCp[nIndex].fPos[1] += dlg.m_fYDev;
			m_pEditLine[theApp.m_nLine].m_pCp[nIndex].fPos[2] += dlg.m_fZDev;
		}
		m_pEditLine[theApp.m_nLine].m_bModified = TRUE;
		FillList();
		Invalidate(FALSE);
		
		InvalidateCoordArrayPane(_FILL_LIST);
	}

}

void C3DViewEditDlg::OnList3dBisect()
{
	m_pEditLine[theApp.m_nLine].Bisect(theApp.m_nIndex);
	FillList();
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST);
}

void C3DViewEditDlg::OnHolesStore()
{
	CCoord CpCentre;
	CCoord CpSurface,CpHead;
	int nType;

	theApp.m_Motors.GetSurfacePos(&CpSurface);

	switch(theApp.m_nIndex) {
	case 0:
	case 1:
	case 2:	PROFILE->m_CpMeasuredHole[theApp.m_nIndex] = CpSurface;
		break;
	case 3:
	case 4:
	case 5:
		nType = PROFILE->m_CpCadHole[theApp.m_nIndex-3].nType;
		PROFILE->m_CpCadHole[theApp.m_nIndex-3] = CpSurface;
		PROFILE->m_CpCadHole[theApp.m_nIndex-3].nType = nType;
		break;
	}
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST);

}

void C3DViewEditDlg::OnHolesGoto()
{
	CCoord Cp;

	theApp.m_Thread.StopCurrentThreadAction();

	switch(theApp.m_nIndex) {
	case 0:
	case 1:
	case 2:	theApp.m_Thread.m_CpMoveTo = PROFILE->m_CpMeasuredHole[theApp.m_nIndex];
		break;
	case 3:
	case 4:
	case 5:
		theApp.m_Kinematics.TipsFromNormals((CCoord*)&PROFILE->m_CpCadHole[theApp.m_nIndex-3],PORTSIDE);
		theApp.m_Thread.m_CpMoveTo = PROFILE->m_CpCadHole[theApp.m_nIndex-3];
		break;
	}

	theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide = 0;
	theApp.m_Motors.GotoSurfaceCoord(&theApp.m_Thread.m_CpMoveTo, PORTSIDE);

}

void C3DViewEditDlg::OnHolesGotoViadangerplane() 
{
	theApp.m_Thread.StopCurrentThreadAction();

	switch(theApp.m_nIndex) {
	case 0:
	case 1:
	case 2:	theApp.m_Thread.m_CpMoveTo = PROFILE->m_CpMeasuredHole[theApp.m_nIndex];
		break;
	case 3:
	case 4:
	case 5:
		theApp.m_Kinematics.TipsFromNormals((CCoord*)&PROFILE->m_CpCadHole[theApp.m_nIndex-3],PORTSIDE);
		theApp.m_Thread.m_CpMoveTo = PROFILE->m_CpCadHole[theApp.m_nIndex-3];
		break;
	}

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide = 0;
		theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TANK_5AXIS:
	case TANK_6AXIS:
	case TANK_2AXIS:
		switch(theApp.m_nSide0Orientation) {
		case 0:	theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide = 1;
			break;
		case 1:	theApp.m_PmacUser.m_nScanSide = PROFILE->m_nScanSide = 0;
			break;
		}
		theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
		break;
	case TRACKED_PROBE:
		break;
	}
}

void C3DViewEditDlg::OnHolesGototipangles() 
{
	CCoord CpSurface,Cp;

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Motors.GetSurfacePos(&CpSurface);

	switch(theApp.m_nIndex) {
	case 0:
	case 1:
	case 2:
		CpSurface.Side0.norm = PROFILE->m_CpMeasuredHole[theApp.m_nIndex].Side0.norm;
		break;
	case 3:
	case 4:
	case 5:
		CpSurface.Side0.norm = PROFILE->m_CpCadHole[theApp.m_nIndex-3].Side0.norm;
		break;
	}

	theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
	theApp.m_Motors.GotoSurfaceCoord(&CpSurface, PORTSIDE);
}

void C3DViewEditDlg::OnHolesSwap0()
{
	CCoord Cp;

	switch(theApp.m_nIndex) {
	case 3:
		Cp = PROFILE->m_CpCadHole[0];
		PROFILE->m_CpCadHole[0] = PROFILE->m_CpCadHole[1];
		PROFILE->m_CpCadHole[1] = Cp;
		break;
	case 4:
		Cp = PROFILE->m_CpCadHole[1];
		PROFILE->m_CpCadHole[1] = PROFILE->m_CpCadHole[0];
		PROFILE->m_CpCadHole[0] = Cp;
		break;
	case 5:
		Cp = PROFILE->m_CpCadHole[2];
		PROFILE->m_CpCadHole[2] = PROFILE->m_CpCadHole[0];
		PROFILE->m_CpCadHole[0] = Cp;
		break;
	}
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST);
}

void C3DViewEditDlg::OnHolesSwap1()
{
	CCoord Cp;

	switch(theApp.m_nIndex) {
	case 3:
		Cp = PROFILE->m_CpCadHole[0];
		PROFILE->m_CpCadHole[0] = PROFILE->m_CpCadHole[2];
		PROFILE->m_CpCadHole[2] = Cp;
		break;
	case 4:
		Cp = PROFILE->m_CpCadHole[1];
		PROFILE->m_CpCadHole[1] = PROFILE->m_CpCadHole[2];
		PROFILE->m_CpCadHole[2] = Cp;
		break;
	case 5:
		Cp = PROFILE->m_CpCadHole[2];
		PROFILE->m_CpCadHole[2] = PROFILE->m_CpCadHole[1];
		PROFILE->m_CpCadHole[1] = Cp;
		break;
	}
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST);
}

void C3DViewEditDlg::OnHolesInvertnormal()
{
	switch(theApp.m_nIndex) {
	case 3:
	case 4:
	case 5:
		PROFILE->m_CpCadHole[theApp.m_nIndex-3].InvertBothNormals();
		break;
	}
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST);
}

void C3DViewEditDlg::OnBrainModifydeltas()
{
	PROFILE->ModifyBrainDeltas(theApp.m_nLine,theApp.m_nIndex);
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST);
}

void C3DViewEditDlg::OnBrainGotoNoBrain()
{
	CCoord	Cp,CpSurface,CpHead;

	if(theApp.m_ServoUser.AreMotorsPowerd()==FALSE) return;

	theApp.m_Thread.StopCurrentThreadAction();

	theApp.m_Motors.GetHeadPos(&CpHead);

	CpSurface = m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex];

	theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
	theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

	theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,PORTSIDE,TRUE,TRUE, NULL, CURRENT_FL);
	theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,STARBOARD,TRUE,TRUE, NULL, CURRENT_FL);

	theApp.m_Thread.m_CpMoveTo = CpSurface;
	theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide=2;
	theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
//	theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
}

void C3DViewEditDlg::OnBrainGotobrain()
{
	CCoord	Cp,CpSurface,CpHead;
	int nSlowAxis=1;

	if(theApp.m_ServoUser.AreMotorsPowerd()==FALSE) return;

	theApp.m_Thread.StopCurrentThreadAction();

	theApp.m_Motors.GetHeadPos(&CpHead);

	CpSurface = m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex];

	PROFILE->CalculateBrainCoord(PROFILE->m_nFastAxis,PROFILE->m_nSlowAxis,CpSurface);

	theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
	theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

	theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,PORTSIDE,TRUE,TRUE, NULL, CURRENT_FL);
	theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,STARBOARD,TRUE,TRUE, NULL, CURRENT_FL);

	theApp.m_Thread.m_CpMoveTo = CpSurface;
	theApp.m_PmacUser.m_nScanSide=PROFILE->m_nScanSide=2;
	theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
}

void C3DViewEditDlg::OnTimer(UINT nIDEvent)
{
	if(PROFILE->m_bModified != m_bModified) {
//		SetWindowTitle();
	}

	m_bModified = PROFILE->m_bModified;

	CResizablePage::OnTimer(nIDEvent);
}


void C3DViewEditDlg::SetWindowTitle()
{
	CString	Buff;
	int nPos;

	Buff = PROFILE->m_FileName;
	if((nPos=Buff.ReverseFind('.')) > -1) {
		Buff.GetBufferSetLength(nPos);
	}

	if(PROFILE->m_bModified) {
		Buff += " (Not Saved)";
	}

	SetWindowText(Buff);
}

void C3DViewEditDlg::FindAllLimits(bool bCenter)
{
	COORDDATA	CpTopLeft,CpBottomRight;
	CRect rr;

	m_staticView.GetWindowRect(&rr);

	PROFILE->FindAllLineLimits(&CpTopLeft,&CpBottomRight);
	if(theApp.m_LastSettings.bIncludeRefPtsInScaling == true) 
		PROFILE->FindAllHoleLimits(&CpTopLeft,&CpBottomRight);

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

	/*
	float fXSize = fabs(m_fX0p - m_fX1p);
	float fYSize = fabs(m_fY0p - m_fY1p);
	float fZSize = fabs(m_fZ0p - m_fZ1p);
	switch (m_nDisplayFastAxis) {
	case 0:	
		switch (m_nDisplaySlowAxis) {
		case 1:
			if (fXSize > fYSize) {
				fYSize = fXSize * (float)rr.Height() / (float)rr.Width();
			}
			else {
				fXSize = fYSize * (float)rr.Width() / (float)rr.Height();
			}
			break;
		case 2:
			break;
		}
		break;
	case 1:
		break;
	case 2:
		break;
	}
	*/

	if (bCenter == true) {
		m_CpZoom.Side0.fX = (m_fX1p + m_fX0p) / 2.0f;
		m_CpZoom.Side0.fY = (m_fY1p + m_fY0p) / 2.0f;
		m_CpZoom.Side0.fZ = (m_fZ1p + m_fZ0p) / 2.0f;
	}

	m_fI0p = -1.0f;
	m_fI1p = 1.0f;
	m_fJ0p = -1.0f;
	m_fJ1p = 1.0f;
	m_fK0p = -1.0f;
	m_fK1p = 1.0f;

}

void C3DViewEditDlg::ChangeLimitsForAspectRatio(CRect rrView)
{
	float fComponentWidth;
	float fScreenRatio = (float)rrView.Height() / (float)rrView.Width();
	float fXC;

	if(theApp.m_LastSettings.bMaintainAspectRaioInScaling == true) {
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
		}
	}
}


void C3DViewEditDlg::OnList3dDeleteline()
{
	On3dviewDeleteline();
}

void C3DViewEditDlg::OnDeletePrecedingPoints()
{
	if(m_pEditLine==NULL) return;

	if(theApp.m_nIndex<=0) return;
	
	m_pEditLine[theApp.m_nLine].DeleteCoords(0,theApp.m_nIndex);

	InitializeEditLine();

	FillList();
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST);

}

void C3DViewEditDlg::OnDeleteFollowingPoints()
{
	if(m_pEditLine==NULL) return;

	if((theApp.m_nIndex+2)>m_pEditLine[theApp.m_nLine].m_nCoordL) return;

	m_pEditLine[theApp.m_nLine].DeleteCoords(theApp.m_nIndex+2,m_pEditLine[theApp.m_nLine].m_nCoordL);

	InitializeEditLine();

	FillList();
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST);
}


void C3DViewEditDlg::OnUpdateFileSaveAs(CCmdUI* pCmdUI)
{

}

void C3DViewEditDlg::OnViewIges()
{
	m_nPaintMode = 2;
	RefreshView();
	SetToolBarCheckedState();
}

void C3DViewEditDlg::OnViewLines()
{
	switch(theApp.m_nEditLineType) {
	case TAUGHTLINES: theApp.m_LastSettings.bDisplayTaughtLines==TRUE ? theApp.m_LastSettings.bDisplayTaughtLines=FALSE : theApp.m_LastSettings.bDisplayTaughtLines=TRUE;
		break;
	case SCANLINES: theApp.m_LastSettings.bDisplayScanLines==TRUE ? theApp.m_LastSettings.bDisplayScanLines=FALSE : theApp.m_LastSettings.bDisplayScanLines=TRUE;
		break;
	case BRAINLINES: theApp.m_LastSettings.bDisplayBrainLines==TRUE ? theApp.m_LastSettings.bDisplayBrainLines=FALSE : theApp.m_LastSettings.bDisplayBrainLines=TRUE;
		break;
	case ENVELOPELINES: theApp.m_LastSettings.bDisplayEnvelopeLines==TRUE ? theApp.m_LastSettings.bDisplayEnvelopeLines=FALSE : theApp.m_LastSettings.bDisplayEnvelopeLines=TRUE;
		break;
	case SURFACELINES: theApp.m_LastSettings.bDisplaySurfaceLines==TRUE ? theApp.m_LastSettings.bDisplaySurfaceLines=FALSE : theApp.m_LastSettings.bDisplaySurfaceLines=TRUE;
		break;
	case OUTSIDELINES: theApp.m_LastSettings.bDisplayBladeLines[OUTSIDE]==TRUE ? theApp.m_LastSettings.bDisplayBladeLines[OUTSIDE]=FALSE : theApp.m_LastSettings.bDisplayBladeLines[OUTSIDE]=TRUE;
		break;
	case INSIDELINES: theApp.m_LastSettings.bDisplayBladeLines[INSIDE]==TRUE ? theApp.m_LastSettings.bDisplayBladeLines[INSIDE]=FALSE : theApp.m_LastSettings.bDisplayBladeLines[INSIDE]=TRUE;
		break;
	}
	Invalidate(FALSE);
	SetToolBarCheckedState();
}

void C3DViewEditDlg::OnViewCrosses()
{
	switch(theApp.m_nEditLineType) {
	case TAUGHTLINES: theApp.m_LastSettings.bDisplayTaughtCrosses==TRUE ? theApp.m_LastSettings.bDisplayTaughtCrosses=FALSE : theApp.m_LastSettings.bDisplayTaughtCrosses=TRUE;
		break;
	case SCANLINES: theApp.m_LastSettings.bDisplayScanCrosses==TRUE ? theApp.m_LastSettings.bDisplayScanCrosses=FALSE : theApp.m_LastSettings.bDisplayScanCrosses=TRUE;
		break;
	case BRAINLINES: theApp.m_LastSettings.bDisplayBrainCrosses==TRUE ? theApp.m_LastSettings.bDisplayBrainCrosses=FALSE : theApp.m_LastSettings.bDisplayBrainCrosses=TRUE;
		break;
	case ENVELOPELINES: theApp.m_LastSettings.bDisplayEnvelopeCrosses==TRUE ? theApp.m_LastSettings.bDisplayEnvelopeCrosses=FALSE : theApp.m_LastSettings.bDisplayEnvelopeCrosses=TRUE;
		break;
	case SURFACELINES: theApp.m_LastSettings.bDisplaySurfaceCrosses==TRUE ? theApp.m_LastSettings.bDisplaySurfaceCrosses=FALSE : theApp.m_LastSettings.bDisplaySurfaceCrosses=TRUE;
		break;
	case OUTSIDELINES: theApp.m_LastSettings.bDisplayBladeCrosses[OUTSIDE]==TRUE ? theApp.m_LastSettings.bDisplayBladeCrosses[OUTSIDE]=FALSE : theApp.m_LastSettings.bDisplayBladeCrosses[OUTSIDE]=TRUE;
		break;
	case INSIDELINES: theApp.m_LastSettings.bDisplayBladeCrosses[INSIDE]==TRUE ? theApp.m_LastSettings.bDisplayBladeCrosses[INSIDE]=FALSE : theApp.m_LastSettings.bDisplayBladeCrosses[INSIDE]=TRUE;
		break;
	}
	Invalidate(FALSE);
	SetToolBarCheckedState();
}

void C3DViewEditDlg::OnButtonSelectarea()
{
	m_bSelectArea==FALSE ? m_bSelectArea=TRUE : m_bSelectArea=FALSE;
	SetToolBarCheckedState();
}

void C3DViewEditDlg::OnAreaDelete()
{
	int nIgnoreAxis;

	switch(PROFILE->m_nFastAxis) {
	case 0:
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
	switch (theApp.m_nEditLineType) {
	case TAUGHTLINES: PROFILE->DeleteCoordInArea(theApp.m_nEditLineType, &m_CpSelectArea[0], &m_CpSelectArea[1], nIgnoreAxis);
		break;
	case SCANLINES: PROFILE->CropCoordInArea(theApp.m_nEditLineType, &m_CpSelectArea[0], &m_CpSelectArea[1], nIgnoreAxis);
		break;
	}

	InitializeEditLine();
	UpdateAllControls();
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST | _FILL_LINE_LIST);

}


void C3DViewEditDlg::OnAreaDeleteallscanlines()
{
	PROFILE->DeleteAllLines(SCANLINES);
	InitializeEditLine();
	UpdateAllControls();
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST | _FILL_LINE_LIST);

}

void C3DViewEditDlg::OnEnvelopePaste()
{
	CCoord Cp;

	theApp.m_CopyLine.GetCoord(0,&Cp);
	if(PROFILE->m_EnvelopeLine==NULL) {
		PROFILE->m_EnvelopeLine = new CPolyCoord[PROFILE->m_nEnvelopeLineL=1];
	}
	PROFILE->m_EnvelopeLine[theApp.m_nLine=0].Add(Cp);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST);

}



void C3DViewEditDlg::On3dviewSortalllines()
{
	PROFILE->SortEachLine(theApp.m_nEditLineType,PROFILE->m_nFastAxis);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
	
	InvalidateCoordArrayPane(_FILL_LIST | _FILL_LINE_LIST);

}

void C3DViewEditDlg::On3dviewReorderlines()
{
	switch(PROFILE->m_nComputeAlgorithm) {
	default:
		PROFILE->OrderLines(theApp.m_nEditLineType,PROFILE->m_nSlowAxis); 
		break;
	case 0:
	case 2:
	case 4:
	case 6:
		PROFILE->OrderLines(theApp.m_nEditLineType,PROFILE->m_nFastAxis); 
		break;
	}
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
	InvalidateCoordArrayPane(_FILL_LIST | _FILL_LINE_LIST);
}


void C3DViewEditDlg::OnViewZoomIn()
{

	Invalidate(FALSE);
}

void C3DViewEditDlg::OnViewZoomOut()
{

	Invalidate(FALSE);

}

BOOL C3DViewEditDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint	point;
	CRect rr;

	GetCursorPos(&point);
	ScreenToClient(&point);
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	if(m_nProjectionMoveOption==3) {
		if(rr.PtInRect(point)) {
			SetCursor(m_hcursorFinger);
			return TRUE;
		}
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


void C3DViewEditDlg::OnBrainResetalldeltas()
{
	PROFILE->ZeroAllBrainDeltas(0);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);

}

void C3DViewEditDlg::OnBrainResetleftdeltas()
{
	PROFILE->ZeroAllBrainDeltas(1);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);

}

void C3DViewEditDlg::OnBrainResetrightdeltas()
{
	PROFILE->ZeroAllBrainDeltas(2);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);

}

void C3DViewEditDlg::OnBrainResetijk()
{
	PROFILE->ZeroAllBrainDeltas(3);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
}

void C3DViewEditDlg::OnBrainCopy()
{
	OnList3dCopy();
}

void C3DViewEditDlg::OnBrainPaste()
{
	OnList3dPaste();

}


void C3DViewEditDlg::OnList3dCalculatextangle()
{
	int nStartIndex,nFinishIndex;

	POSITION pos = m_listCoord.GetFirstSelectedItemPosition();
	nStartIndex=nFinishIndex=m_listCoord.GetNextSelectedItem(pos);

	while(pos) {
		nFinishIndex=m_listCoord.GetNextSelectedItem(pos);
	}
	for(nStartIndex;nStartIndex<=nFinishIndex;nStartIndex++) {
		m_pEditLine[theApp.m_nLine].CalculateXtip(nStartIndex);
	}
	FillList();
	Invalidate(FALSE);

}


void C3DViewEditDlg::DrawXtipYTipGraticule(CDC* pDC,CRect rr)
{
	if(theApp.m_LastSettings.nDisplayEditNormals != 1) return;

	pDC->MoveTo(rr.left,(rr.top+rr.bottom)/2);
	pDC->LineTo(rr.right,(rr.top+rr.bottom)/2);


}

void C3DViewEditDlg::OnList3dStraiteline()
{
	int nStartIndex,nFinishIndex;

	POSITION pos = m_listCoord.GetFirstSelectedItemPosition();
	nStartIndex=nFinishIndex=m_listCoord.GetNextSelectedItem(pos);

	while(pos) {
		nFinishIndex=m_listCoord.GetNextSelectedItem(pos);
	}
	m_pEditLine[theApp.m_nLine].Straighten(nStartIndex,nFinishIndex);
	FillList();
	Invalidate(FALSE);

}



void C3DViewEditDlg::OnToolsSinewavegenerator()
{
	CSineWaveDlg dlg;

	if(dlg.DoModal() == IDOK) {
		if(PROFILE->m_TaughtLine) {
			PROFILE->m_TaughtLine[theApp.m_nLine].GenerateSineWave(dlg.m_nMode,dlg.m_Cp,dlg.m_fPitch,dlg.m_fAmp,dlg.m_fPhase,dlg.m_fSeg,dlg.m_fNumberCycles);
		}
		FillList();
		Invalidate(FALSE);
	}
}


void C3DViewEditDlg::OnList3dOptomise()
{
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	POSITION pos = m_listCoord.GetFirstSelectedItemPosition();
	theApp.m_nIndex=m_listCoord.GetNextSelectedItem(pos);

	pFrame->StopCurrentTimerAction();
	pFrame->m_nTimerAction = OPTOMISE_COORDINATE;


}

void C3DViewEditDlg::ComponentMove()
{

//    D3DXMatrixIdentity( &m_matWorld);
//	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_CompBall.GetTranslationMatrix() );
//	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_CompBall.GetRotationMatrix() );
//	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_WorldBall.GetTranslationMatrix() );
//	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_WorldBall.GetRotationMatrix() );

}




//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
CD3DArcBall_10::CD3DArcBall_10()
{
	Init();
}


void CD3DArcBall_10::Init(bool bJustMatrices)
{
	D3DXQuaternionIdentity(&m_qDown);
	D3DXQuaternionIdentity(&m_qNow);
	D3DXMatrixIdentity(&m_matRotation);
	D3DXMatrixIdentity(&m_matRotationDelta);
	D3DXMatrixIdentity(&m_matTranslation);
	D3DXMatrixIdentity(&m_matTranslationDelta);

	if (bJustMatrices == FALSE) {
		m_bDrag = FALSE;
		m_fRadiusTranslation = 1.0f;
		m_bRightHanded = TRUE;
		m_nDragAxes = 0;
	}
}

VOID CD3DArcBall_10::SetWindow( int iWidth, int iHeight, float fRadius )
{
    // Set ArcBall info
    m_iWidth  = iWidth;
    m_iHeight = iHeight;
    m_fRadius = fRadius;
}

void CD3DArcBall_10::SetTranslationMatrix(float fx, float fy, float fz)
{
		D3DXMatrixTranslation( &m_matTranslation, -fx, -fy, -fz );
		D3DXMatrixIdentity( &m_matTranslationDelta );

}

void CD3DArcBall_10::SetRotationMatrix(float fI, float fJ, float fK)
{
		D3DXMatrixRotationZ( &m_matRotation,atan2f(fI * -1.0f , fJ * -1.0f));
		D3DXMatrixIdentity( &m_matRotationDelta );

		D3DXMatrixMultiply( &m_matTranslation, &m_matTranslation, &m_matTranslationDelta );



}
//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
D3DXVECTOR3 CD3DArcBall_10::ScreenToVector( int sx, int sy )
{
	D3DXVECTOR3 vec;
	D3DXMATRIXA16 matx;

    // Scale to screen
    FLOAT x   = -(sx - m_iWidth/2)  / (m_fRadius*m_iWidth/2);
    FLOAT y   =  (sy - m_iHeight/2) / (m_fRadius*m_iHeight/2);

	switch(m_nDragAxes) {
	case 0:
		break;
	case 1:
		x = 0.0f;
		break;
	case 2:
		break;
	case 3:
		y = 0.0f;
		break;
	}

    if( m_bRightHanded )
    {
        x = -x;
        y = -y;
    }

    FLOAT z   = 0.0f;
    FLOAT mag = x*x + y*y;

    if( mag > 1.0f )
    {
        FLOAT scale = 1.0f/sqrtf(mag);
        x *= scale;
        y *= scale;
    }
    else
        z = sqrtf( 1.0f - mag );

	if(m_nDragAxes == 2) {
		z = 0.0f;
	}

	vec = D3DXVECTOR3( x, y, z );
	if(theApp.m_LastSettings.n3DUpVector == 1) {
		D3DXMatrixRotationX(&matx,PIf);
		D3DXVec3TransformCoord(&vec,&vec,&matx);
	}
    return vec;
}


VOID CD3DArcBall_10::SetRadius( FLOAT fRadius )
{
    m_fRadiusTranslation = fRadius;
}

void C3DViewEditDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rr;
	CCoord Cp;
	int	nIgnoreAxis;
	m_bLBDown = TRUE;
	m_ptDown = point;
	bool bFlag = false;

	SAFE_DELETE( m_pEditSpinItem );

	switch(PROFILE->m_nFastAxis) {
	case 0:
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

	point.Offset(-rr.TopLeft());
	ClientToWorld(&point,rr,&Cp);

	if(m_bSelectArea==FALSE) {
		if(m_nProjectionMoveOption!= 3) {
			switch(theApp.m_nEditLineType) {
			case TAUGHTLINES: bFlag = PROFILE->FindNearestCoordinate(theApp.m_nEditLineType,Cp,&theApp.m_nLine,&theApp.m_nIndex,nIgnoreAxis);
				break;
			case SCANLINES:	bFlag = PROFILE->FindNearestCoordinate(theApp.m_nEditLineType,Cp,&theApp.m_nLine,&theApp.m_nIndex,nIgnoreAxis);
				break;
			case BRAINLINES: bFlag = PROFILE->FindNearestCoordinate(theApp.m_nEditLineType,Cp,&theApp.m_nLine,&theApp.m_nIndex,nIgnoreAxis);
				break;
			case ENVELOPELINES: bFlag = PROFILE->FindNearestCoordinate(theApp.m_nEditLineType,Cp,&theApp.m_nLine,&theApp.m_nIndex,nIgnoreAxis);
				break;
			case OUTSIDELINES: bFlag = PROFILE->FindNearestCoordinate(theApp.m_nEditLineType,Cp,&theApp.m_nLine,&theApp.m_nIndex,nIgnoreAxis);
				break;
			case INSIDELINES: bFlag = PROFILE->FindNearestCoordinate(theApp.m_nEditLineType,Cp,&theApp.m_nLine,&theApp.m_nIndex,nIgnoreAxis);
				break;
			}
			if(bFlag==TRUE) {
				UpdateAllControls();
				FillList();
				Invalidate(FALSE);
				InvalidateCoordArrayPane(_FILL_LIST |_ENSURE_LINE_LIST_VISIBLE);
			}
		} else {
			m_bLBDown=TRUE;
		}
	} else {
		m_CpSelectArea[0]=Cp;
		m_CpSelectArea[1]=Cp;
		Invalidate(FALSE);
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void C3DViewEditDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLBDown = false;

	CDialog::OnLButtonUp(nFlags, point);
}



void C3DViewEditDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rr;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	point.Offset(-rr.TopLeft());

	if(m_bSelectArea==TRUE && m_bLBDown == true) {
		ClientToWorld(&point,rr,&m_CpSelectArea[1]);
		Invalidate(FALSE);
	}

	if (m_bMBDown == true) {
		m_ptZoom = point;
		Invalidate(FALSE);
	}

	CDialog::OnMouseMove(nFlags, point);
}

void C3DViewEditDlg::UpdatAll3DDependents()
{
	CProfileSheet* pSheet = (CProfileSheet*)GetParent();
	if(pSheet->m_p3DToolsSheet) pSheet->m_p3DToolsSheet->UpdateAllPages();
	if(m_pTranslationDlg) m_pTranslationDlg->UpdateAllControls();
}

void C3DViewEditDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnRButtonDown(nFlags, point);
}

void C3DViewEditDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_bRBDown = FALSE;
	CDialog::OnRButtonUp(nFlags, point);
}


void C3DViewEditDlg::OnToolsWaypoints()
{
	if (m_pWayPointsDlg == NULL) {
		m_pWayPointsDlg = new CWayPointsDlg(this, (CWnd**)&m_pWayPointsDlg);
		m_pWayPointsDlg->Create(CWayPointsDlg::IDD,this);
	} else {
		m_pWayPointsDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}



void C3DViewEditDlg::OnToolsUntwist()
{
	if(MessageBox(_T("This utility works its way along the X axis rotating the component in a boss\nkeeping the vector through leading and trailing edge vertical.\nProceed?"),_T("Message"),MB_YESNO) == IDYES) {
		PROFILE->Untwist();
		Invalidate(FALSE);
	}

}



void C3DViewEditDlg::ColumnclickListCoords() 
{
	CMenu* menu = (CMenu *) new CMenu;
	CMenu *pPopup;
	CPoint	Point;
	CString Buff,Language;


	GetCursorPos(&Point);

	if (menu->LoadMenu(IDR_LIST_HEADER_MENU)) {

		pPopup = menu->GetSubMenu(0);
		if(PROFILE->m_nHeader[H_RAXIS]) pPopup->CheckMenuItem(ID_HEADER_RAXIS, MF_CHECKED);
		if(PROFILE->m_nHeader[H_EOL]) pPopup->CheckMenuItem(ID_HEADER_ENDOFLINE, MF_CHECKED);
		if(PROFILE->m_nHeader[H_WP0]) pPopup->CheckMenuItem(ID_HEADER_WATERPATHLEFT, MF_CHECKED);
		if(PROFILE->m_nHeader[H_WP1]) pPopup->CheckMenuItem(ID_HEADER_WATERPATHRIGHT, MF_CHECKED);
		if(PROFILE->m_nHeader[H_GAIN0]) pPopup->CheckMenuItem(ID_HEADER_GAIN, MF_CHECKED);
		if(PROFILE->m_nHeader[H_DELAY0]) pPopup->CheckMenuItem(ID_HEADER_INTERFACEDELAY, MF_CHECKED);
		if(PROFILE->m_nHeader[H_DELAY1]) pPopup->CheckMenuItem(ID_HEADER_GATE1DELAY, MF_CHECKED);
		if(PROFILE->m_nHeader[H_DELAY2]) pPopup->CheckMenuItem(ID_HEADER_GATE2DELAY, MF_CHECKED);
		if(PROFILE->m_nHeader[H_WIDTH0]) pPopup->CheckMenuItem(ID_HEADER_INTERFACEWIDTH, MF_CHECKED);
		if(PROFILE->m_nHeader[H_WIDTH1]) pPopup->CheckMenuItem(ID_HEADER_GATE1WIDTH, MF_CHECKED);
		if(PROFILE->m_nHeader[H_WIDTH2]) pPopup->CheckMenuItem(ID_HEADER_GATE2WIDTH, MF_CHECKED);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x,Point.y,this);

	};
	delete menu;
}

void C3DViewEditDlg::OnHeaderRaxis() 
{
	PROFILE->m_nHeader[H_RAXIS] == 0 ? PROFILE->m_nHeader[H_RAXIS] = 1 : PROFILE->m_nHeader[H_RAXIS] = 0;
	CreateColumns();
	FillList();
	
}

void C3DViewEditDlg::OnHeaderEndofline() 
{
	PROFILE->m_nHeader[H_EOL] == 0 ? PROFILE->m_nHeader[H_EOL] = 1 : PROFILE->m_nHeader[H_EOL] = 0;
	CreateColumns();
	FillList();
}

void C3DViewEditDlg::OnHeaderGain() 
{
	PROFILE->m_nHeader[H_GAIN0] == 0 ? PROFILE->m_nHeader[H_GAIN0] = 1 : PROFILE->m_nHeader[H_GAIN0] = 0;
	CreateColumns();
	FillList();
}

void C3DViewEditDlg::OnHeaderWaterpathleft() 
{
	PROFILE->m_nHeader[H_WP0] == 0 ? PROFILE->m_nHeader[H_WP0] = 1 : PROFILE->m_nHeader[H_WP0] = 0;
	CreateColumns();
	FillList();
}

void C3DViewEditDlg::OnHeaderWaterpathright() 
{
	PROFILE->m_nHeader[H_WP1] == 0 ? PROFILE->m_nHeader[H_WP1] = 1 : PROFILE->m_nHeader[H_WP1] = 0;
	CreateColumns();
	FillList();
}

void C3DViewEditDlg::OnHeaderInterfacedelay() 
{
	PROFILE->m_nHeader[H_DELAY0] == 0 ? PROFILE->m_nHeader[H_DELAY0] = 1 : PROFILE->m_nHeader[H_DELAY0] = 0;
	CreateColumns();
	FillList();
}

void C3DViewEditDlg::OnHeaderInterfacewidth() 
{
	PROFILE->m_nHeader[H_WIDTH0] == 0 ? PROFILE->m_nHeader[H_WIDTH0] = 1 : PROFILE->m_nHeader[H_WIDTH0] = 0;
	CreateColumns();
	FillList();
}

void C3DViewEditDlg::OnHeaderGate1delay() 
{
	PROFILE->m_nHeader[H_DELAY1] == 0 ? PROFILE->m_nHeader[H_DELAY1] = 1 : PROFILE->m_nHeader[H_DELAY1] = 0;
	CreateColumns();
	FillList();
}

void C3DViewEditDlg::OnHeaderGate1width() 
{
	PROFILE->m_nHeader[H_WIDTH1] == 0 ? PROFILE->m_nHeader[H_WIDTH1] = 1 : PROFILE->m_nHeader[H_WIDTH1] = 0;
	CreateColumns();
	FillList();
}

void C3DViewEditDlg::OnHeaderGate2delay() 
{
	PROFILE->m_nHeader[H_DELAY2] == 0 ? PROFILE->m_nHeader[H_DELAY2] = 1 : PROFILE->m_nHeader[H_DELAY2] = 0;
	CreateColumns();
	FillList();
}

void C3DViewEditDlg::OnHeaderGate2width() 
{
	PROFILE->m_nHeader[H_WIDTH2] == 0 ? PROFILE->m_nHeader[H_WIDTH2] = 1 : PROFILE->m_nHeader[H_WIDTH2] = 0;
	CreateColumns();
	FillList();
}


void C3DViewEditDlg::SetAccessPrivelage()
{
	if(GetSafeHwnd() == NULL) return;

	SetToolBarCheckedState();
}

BOOL C3DViewEditDlg::PreTranslateMessage(MSG* pMsg) 
{
	CRect rr;
	CPoint pt;
	
	switch(pMsg->message) {
	case 0x204:
		switch(pMsg->wParam) {
		case 0x02:	//rbdown on header
			GetCursorPos(&pt);
			m_listCoord.GetWindowRect(&rr);
			rr.bottom = rr.top+20;
			if(rr.PtInRect(pt)) {
				ColumnclickListCoords();
				return true;
			}
			break;
		}
	case WM_KEYDOWN:
		TranslateKeydown(pMsg);
	}
	


	return CDialog::PreTranslateMessage(pMsg);
}

void C3DViewEditDlg::OnList3dNoncadvariables() 
{
	if (m_pNonCadVariablesDlg == NULL) {
		m_pNonCadVariablesDlg = new CNonCadVariablesDlg(this, (CWnd**)&m_pNonCadVariablesDlg);
		m_pNonCadVariablesDlg->Create(CNonCadVariablesDlg::IDD,this);
	} else {
		m_pNonCadVariablesDlg->SetWindowPos(&wndTop,0,0,0,0,NULL);
	}
	
}

void C3DViewEditDlg::OnBrainDelete() 
{
	if(m_pEditLine) m_pEditLine[theApp.m_nLine].DeleteCoord(theApp.m_nIndex);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
	
}

void C3DViewEditDlg::OnBrainAddtobrainline()
{
	CRect rr;
	CCoord Cp,CpNew;
	int	nIgnoreAxis;
	int nLine;
	CPolyCoord Line;

	switch(PROFILE->m_nFastAxis) {
	case 0:
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

	//???????
	Cp.Zero();

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	GotoDlgCtrl(GetDlgItem(IDC_STATIC_VIEW));
	m_ptClient.Offset(-rr.TopLeft());
	ClientToWorld(&m_ptClient, rr, &Cp);
	
	for(nLine=0;nLine<PROFILE->m_nScanLineL;nLine++) {
		PROFILE->m_ScanLine[nLine].m_nPrimaryAxis = PROFILE->m_nFastAxis;
		PROFILE->m_ScanLine[nLine].CalculateCoord(Cp.fPos[PROFILE->m_nFastAxis],CpNew);
		CpNew.Delta.Zero();
		Line.Add(CpNew);
	}
	Line.m_nPrimaryAxis = PROFILE->m_nSlowAxis;
	Line.CalculateCoord(Cp.fPos[PROFILE->m_nSlowAxis],CpNew);

	if(PROFILE->m_BrainLine == NULL) {
		PROFILE->m_nBrainLineL=1;
		PROFILE->m_BrainLine = new CPolyCoord[PROFILE->m_nBrainLineL];
		theApp.m_nLine = 0;
	}
	if(theApp.m_nLine<PROFILE->m_nBrainLineL) {
		PROFILE->m_BrainLine[theApp.m_nLine].Add(CpNew);
		PROFILE->m_BrainLine[theApp.m_nLine].Sort(PROFILE->m_nFastAxis);

	} else {
		CPolyCoord CpLine;
		CpLine.Add(CpNew);
		theApp.m_nLine=PROFILE->AddLine(BRAINLINES, &CpLine,PROFILE->m_nSlowAxis);
	}


	UpdateAllControls();
	FillList();
	Invalidate(FALSE);

}

void C3DViewEditDlg::OnBrainNewbraneline() 
{
	theApp.m_nLine = 0;
	if(PROFILE->m_nBrainLineL>0) {
		theApp.m_nLine=PROFILE->m_nBrainLineL;
	}
	OnBrainAddtobrainline();
}

void C3DViewEditDlg::OnBrainSortbranelines() 
{
	PROFILE->SortEachLine(BRAINLINES,PROFILE->m_nFastAxis);
	PROFILE->OrderLines(BRAINLINES,PROFILE->m_nSlowAxis);  
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
	
}

void C3DViewEditDlg::OnBrainPasteline() 
{
	theApp.m_CopyLine.ZeroAllDeltas(0);
	PROFILE->AddLine(theApp.m_nEditLineType,&theApp.m_CopyLine,PROFILE->m_nSlowAxis);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
}

BOOL C3DViewEditDlg::OnSetActive() 
{
	OpenOnLastExit();

	return CResizablePage::OnSetActive();
}


BOOL C3DViewEditDlg::OnKillActive() 
{
	CloseAllChildren();
	

	return CResizablePage::OnKillActive();
}

bool C3DViewEditDlg::IsCoordinateValid()
{
	bool bFlag = true;

	if(m_pEditLine == NULL) {
		bFlag = false;
	} else {
		if(theApp.m_nLine > m_nEditLineL) {
			bFlag = false;
		} else {
			if(m_pEditLine[theApp.m_nLine].m_nCoordL < theApp.m_nIndex) {
				bFlag = false;
			}
		}
	}
	if(bFlag == false) {
		MessageBox(_T("Invalid coordinate to move to"),_T("Error"),MB_ICONERROR);
		return false;
	}
	return true;
}


void C3DViewEditDlg::OnBrainDeleteLine() 
{
	PROFILE->DeleteLine(theApp.m_nEditLineType,theApp.m_nLine);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);

	
}

void C3DViewEditDlg::OnList3dDivideinto2lines() 
{
	PROFILE->DivideScanLineInTwo(theApp.m_nLine,theApp.m_nIndex);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);


}

void C3DViewEditDlg::OnList3dExpandline() 
{
	if (m_pPolyCoordDlg == NULL) {
		m_pPolyCoordDlg = new CPolyCoordDlg(this, (CWnd**)&m_pPolyCoordDlg, &m_pEditLine[theApp.m_nLine], theApp.m_nEditLineType ,theApp.m_nLine);
		m_pPolyCoordDlg->Create(CPolyCoordDlg::IDD,this);
	} else {
		m_pPolyCoordDlg->SetWindowPos(&wndTop,0,0,0,0,NULL);
	}
	
}



void C3DViewEditDlg::OnEnvelopeBisect() 
{
	m_pEditLine[theApp.m_nLine].Bisect(theApp.m_nIndex);
	FillList();
	Invalidate(FALSE);
	
}



void C3DViewEditDlg::OnSurfaceSendtotaughtlines() 
{
	theApp.m_CopyLine.Zero();
	theApp.m_CopyLine = &m_pEditLine[theApp.m_nLine];

	PROFILE->AddLine(TAUGHTLINES,&theApp.m_CopyLine,PROFILE->m_nSlowAxis);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
}

void C3DViewEditDlg::OnSurfaceCopyline() 
{
	theApp.m_CopyLine.Zero();
	theApp.m_CopyLine = &m_pEditLine[theApp.m_nLine];
}


void C3DViewEditDlg::On3dview3dlookatthispoint()
{
	CCoord CpSurface;

	if(m_pEditLine==NULL) {
		MessageBox(L"No points selected", L"Warning", MB_ICONWARNING);
		return;
	}
	CpSurface = m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex];
	CProfileSheet* pSheet = (CProfileSheet*)GetParent();


	m_WorldBall.SetTranslationMatrix(CpSurface.Side0.fX, CpSurface.Side0.fY, CpSurface.Side0.fZ);
	m_WorldBall.SetRotationMatrix(CpSurface.Side0.fI, CpSurface.Side0.fJ, CpSurface.Side0.fK);
	D3DXQuaternionRotationMatrix(&m_WorldBall.m_qNow,m_WorldBall.GetRotationMatrix());


	if(pSheet->m_p3DViewPage) {
		pSheet->m_p3DViewPage->LookAtPoint(CpSurface);
	}

}


void C3DViewEditDlg::On3dviewMovelasertothispoint() 
{
	CCoord CpSurface,CpHead;
	CpSurface = m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex];

	theApp.m_Motors.GetHeadPos(&CpHead);

	CpHead.Side0.fX = CpSurface.Side0.fX + theApp.m_LastSettings.vLaserTranslation.x;
	CpHead.Side0.fZ = CpSurface.Side0.fZ + theApp.m_LastSettings.vLaserTranslation.z;

	theApp.m_Thread.StopCurrentThreadAction();

	theApp.m_Thread.m_CpMoveTo = CpHead;
	theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;

}

void C3DViewEditDlg::OnButtonSurfaceGenerator() 
{
	if (m_pTaughtTools == NULL) {
		m_pTaughtTools = new CTaughtToolsDlg(this, (CWnd**)&m_pTaughtTools);
		m_pTaughtTools->Create(CTaughtToolsDlg::IDD,this);
	} else {
		m_pTaughtTools->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}		
	
}

void C3DViewEditDlg::OnHolesCopyallcadmeasured() 
{
	theApp.m_CopyLine.Zero();
	for(int ii=0;ii<3;ii++) {
		theApp.m_CopyLine.Add(PROFILE->m_CpMeasuredHole[ii]);
	}
	for(int ii=0;ii<3;ii++) {
		theApp.m_CopyLine.Add(PROFILE->m_CpCadHole[ii]);
	}
	
}

void C3DViewEditDlg::OnHolesPasteallcadmeasured() 
{
	int ii,jj;

	for(ii=0;(ii<3) && (ii<theApp.m_CopyLine.GetSize());ii++) {
		PROFILE->m_CpMeasuredHole[ii] = theApp.m_CopyLine.m_pCp[ii];
	}
	for(jj=0,ii=3;(ii<6) && (ii<theApp.m_CopyLine.GetSize());ii++,jj++) {
		PROFILE->m_CpCadHole[jj] = theApp.m_CopyLine.m_pCp[ii];
	}

	FillList();
	Invalidate(FALSE);

}

void C3DViewEditDlg::OnBrainSendtoscanlines() 
{
	theApp.m_CopyLine.Zero();
	theApp.m_CopyLine = &m_pEditLine[theApp.m_nLine];

	PROFILE->AddLine(TAUGHTLINES,&theApp.m_CopyLine,PROFILE->m_nSlowAxis);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);

}

void C3DViewEditDlg::OnBrainSendalltoscanlines() 
{
	for(int nLine = 0;nLine<PROFILE->m_nBrainLineL;nLine++) {
		theApp.m_CopyLine.Zero();
		theApp.m_CopyLine = &m_pEditLine[nLine];

		PROFILE->AddLine(TAUGHTLINES,&theApp.m_CopyLine,PROFILE->m_nSlowAxis);
	}

	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
}

void C3DViewEditDlg::OnHoles3dlookatthispoint() 
{
	CCoord CpSurface;

	int nIndex = theApp.m_nIndex % 3;	

	CpSurface = PROFILE->m_CpCadHole[nIndex];
	CProfileSheet* pSheet = (CProfileSheet*)GetParent();


	m_WorldBall.SetTranslationMatrix(CpSurface.Side0.fX, CpSurface.Side0.fY, CpSurface.Side0.fZ);
	m_WorldBall.SetRotationMatrix(CpSurface.Side0.fI, CpSurface.Side0.fJ, CpSurface.Side0.fK);
	D3DXQuaternionRotationMatrix(&m_WorldBall.m_qNow,m_WorldBall.GetRotationMatrix());


	if(pSheet->m_p3DViewPage) {
		pSheet->m_p3DViewPage->LookAtPoint(CpSurface);
	}

}

void C3DViewEditDlg::OnViewXy() 
{
	PROFILE->m_nFastAxis = 0;
	PROFILE->m_nSlowAxis = 1;
	Invalidate(false);
	SetToolBarCheckedState();
}

void C3DViewEditDlg::OnViewXz() 
{
	PROFILE->m_nFastAxis = 0;
	PROFILE->m_nSlowAxis = 2;
	Invalidate(false);
	SetToolBarCheckedState();
}

void C3DViewEditDlg::OnViewYz() 
{
	PROFILE->m_nFastAxis = 1;
	PROFILE->m_nSlowAxis = 2;
	Invalidate(false);
	SetToolBarCheckedState();
}


void C3DViewEditDlg::On3dviewSortX() 
{
	PROFILE->SortEachLine(theApp.m_nEditLineType,0);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
}

void C3DViewEditDlg::On3dviewSortY() 
{
	PROFILE->SortEachLine(theApp.m_nEditLineType,1);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
}

void C3DViewEditDlg::On3dviewSortZ() 
{
	PROFILE->SortEachLine(theApp.m_nEditLineType,2);
	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
}

void C3DViewEditDlg::OnButtonEditInside() 
{
	if (m_pBladeEditDlg == NULL) {
		CloseAllChildren();
		CRect rr;
		m_wndFileToolBar.GetWindowRect(rr);
		m_pBladeEditDlg = new CEditBladeDlg(this,(CWnd**)&m_pBladeEditDlg,rr.left,rr.bottom);
		m_pBladeEditDlg->Create(CEditBladeDlg::IDD, this);
	}

	if(theApp.m_nEditLineType == OUTSIDELINES) {
		PROFILE->CopyBraneLinesToBladeBraneLines(OUTSIDE);
	}
	
	theApp.m_nEditLineType = INSIDELINES;
	theApp.m_LastSettings.nDisplayOutSideInside = INSIDE;
	InitializeEditLine();
	CreateColumns();
	FillList();
	SetToolBarCheckedState();
	Invalidate(false);
	m_pBladeEditDlg->SetAccessPrivelage();
	
}

void C3DViewEditDlg::OnButtonEditOutside() 
{
	if (m_pBladeEditDlg == NULL) {
		CloseAllChildren();
		CRect rr;
		m_wndFileToolBar.GetWindowRect(rr);
		m_pBladeEditDlg = new CEditBladeDlg(this,(CWnd**)&m_pBladeEditDlg,rr.left,rr.bottom);
		m_pBladeEditDlg->Create(CEditBladeDlg::IDD, this);
	}

	if(theApp.m_nEditLineType == INSIDELINES) {
		PROFILE->CopyBraneLinesToBladeBraneLines(INSIDE);
	}

	theApp.m_nEditLineType = OUTSIDELINES;
	theApp.m_LastSettings.nDisplayOutSideInside = OUTSIDE;
	InitializeEditLine();
	CreateColumns();
	FillList();
	SetToolBarCheckedState();
	Invalidate(false);
	m_pBladeEditDlg->SetAccessPrivelage();
}

void C3DViewEditDlg::On3dviewBisectaddlinebetween() 
{
	PROFILE->BisectAndAddLine(theApp.m_nEditLineType,theApp.m_nLine);

	InitializeEditLine();
	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
}

void C3DViewEditDlg::OnList3dCircumcenterbetween() 
{
	if (m_pCircumcenterDlg == NULL) {
		m_pCircumcenterDlg = new CCircumcentreDlg(this, (CWnd**)&m_pCircumcenterDlg);
		m_pCircumcenterDlg->Create(CCircumcentreDlg::IDD,this);
	} else {
		m_pCircumcenterDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void C3DViewEditDlg::OnEnvelopeAdd() 
{
	CRect rr;
	CCoord Cp,CpNew;
	int	nIgnoreAxis;
	int nLine;
	CPolyCoord Line;

	switch(PROFILE->m_nFastAxis) {
	case 0:
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

	Cp.Zero();
	ClientToWorld(&m_ptClient,rr,&Cp);

	
	for(nLine=0;nLine<PROFILE->m_nScanLineL;nLine++) {
		PROFILE->m_ScanLine[nLine].m_nPrimaryAxis = PROFILE->m_nFastAxis;
		PROFILE->m_ScanLine[nLine].CalculateCoord(Cp.fPos[PROFILE->m_nFastAxis],CpNew);
		CpNew.Delta.Zero();
		Line.Add(CpNew);
	}
	Line.m_nPrimaryAxis = PROFILE->m_nSlowAxis;
	Line.CalculateCoord(Cp.fPos[PROFILE->m_nSlowAxis],CpNew);

	if(PROFILE->m_EnvelopeLine == NULL) {
		PROFILE->m_nEnvelopeLineL=1;
		PROFILE->m_EnvelopeLine = new CPolyCoord[PROFILE->m_nEnvelopeLineL];
		theApp.m_nLine = 0;
	}
	if(theApp.m_nLine<PROFILE->m_nEnvelopeLineL) {
		PROFILE->m_EnvelopeLine[theApp.m_nLine].InsertCoord(CpNew);
	} else {
		CPolyCoord CpLine;
		CpLine.Add(CpNew);
		theApp.m_nLine=PROFILE->AddLine(ENVELOPELINES, &CpLine,PROFILE->m_nSlowAxis);
	}


	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
}

void C3DViewEditDlg::OnEnvelopeDelete() 
{
	DeleteCoordinate();
}

void C3DViewEditDlg::OnEnvelopeMove() 
{
	CRect rr;
	CCoord Cp,CpNew;
	int	nIgnoreAxis;
	int nLine;
	CPolyCoord Line;

	switch(PROFILE->m_nFastAxis) {
	case 0:
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

	Cp.Zero();
	ClientToWorld(&m_ptClient,rr,&Cp);

	
	for(nLine=0;nLine<PROFILE->m_nScanLineL;nLine++) {
		PROFILE->m_ScanLine[nLine].m_nPrimaryAxis = PROFILE->m_nFastAxis;
		PROFILE->m_ScanLine[nLine].CalculateCoord(Cp.fPos[PROFILE->m_nFastAxis],CpNew);
		CpNew.Delta.Zero();
		Line.Add(CpNew);
	}
	Line.m_nPrimaryAxis = PROFILE->m_nSlowAxis;
	Line.CalculateCoord(Cp.fPos[PROFILE->m_nSlowAxis],CpNew);

	if(theApp.m_nLine<PROFILE->m_nEnvelopeLineL) {
		PROFILE->m_EnvelopeLine[theApp.m_nLine].ModifyCoord(theApp.m_nIndex,CpNew);
	}


	UpdateAllControls();
	FillList();
	Invalidate(FALSE);
}

void C3DViewEditDlg::OnEnvelopeTaborder() 
{
	if(theApp.m_nLine<PROFILE->m_nEnvelopeLineL) {
		PROFILE->m_EnvelopeLine[theApp.m_nLine].MoveToFirstCoordinate(theApp.m_nIndex);
		UpdateAllControls();
		FillList();
		Invalidate(FALSE);
	}
}

void C3DViewEditDlg::OnHolesImportcadptsfromtextfile() 
{
	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;

	CFileDialog FileDlg(TRUE,_T(".txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Text (*.txt)|*.txt|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_AptSourcePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_AptSourcePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,_T("Error"),MB_ICONERROR);
		} else {
			PROFILE->ImportCADRefPts(&File);
			File.Close();
			FillList();
			Invalidate(FALSE);
		}
	}
}

void C3DViewEditDlg::OnHolesGuesscadnorms() 
{
	PROFILE->GuessCADNorms();
	FillList();
	Invalidate(FALSE);

}




void C3DViewEditDlg::OnKeydownListCoords(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void C3DViewEditDlg::On3dviewComputeoppositeside() 
{
	CCoord CpSurface;

	CpSurface = m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex];

	theApp.m_Kinematics.CalculateOppositeCoord(STARBOARD,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
	m_pEditLine[theApp.m_nLine].m_pCp[theApp.m_nIndex].Side1 = CpSurface.Side1;
	
}

void C3DViewEditDlg::On3dviewAllCopyalllines() 
{
	PROFILE->CopyAllLinesToGlobal(theApp.m_nEditLineType);
}

void C3DViewEditDlg::On3dviewAllPastealllines() 
{
	PROFILE->PasteAllLinesFromGlobal(theApp.m_nEditLineType);
}

void C3DViewEditDlg::On3dviewAllDeletealllines() 
{
	// TODO: Add your command handler code here
	
}

void C3DViewEditDlg::On3dviewAllDecimatealllines() 
{
	// TODO: Add your command handler code here
	
}

bool C3DViewEditDlg::IsCtrlPressed() {
	return (GetAsyncKeyState(VK_CONTROL) & 0xFFFF);
}

void C3DViewEditDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
		case 0x41: //'A' key	
			if (IsCtrlPressed()) OnButtonAddCoordinate();
			break;
		case 0x44: //'D' key	
			if (IsCtrlPressed()) OnButtonDeleteCoordinate();
			break;
		case 0x4E: //'N' key	
			if (IsCtrlPressed()) OnButtonNewLine();
			break;
	};
	CResizablePage::OnKeyDown(nChar, nRepCnt, nFlags);
}

void C3DViewEditDlg::TranslateKeydown(MSG* pMsg) {
	m_keyState.lparam = pMsg->lParam;
	OnKeyDown(pMsg->wParam, m_keyState.nRepeatCount, m_keyState.nFlags);
}


void C3DViewEditDlg::InvalidateCoordArrayPane(int nFlag)
{
	FRAME;
	pFrame->SendMessage(UI_INVALIDATE_COORD_ARRAY_PANE, nFlag, 0x00);
}

void C3DViewEditDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	CRect rr;
	m_bMBDown = TRUE;

	SAFE_DELETE(m_pEditSpinItem);

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);
	point.x -= rr.left;
	point.y -= rr.top;

	ClientToWorld(&point, rr, &m_CpZoom);
	m_ptZoom = point;

	CDialog::OnMButtonDown(nFlags, point);
}

void C3DViewEditDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_bMBDown = FALSE;

	CDialog::OnMButtonUp(nFlags, point);
}

BOOL C3DViewEditDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CRect rr;

	m_staticView.GetWindowRect(rr);
	ScreenToClient(rr);
	ScreenToClient(&pt);
	pt.x -= rr.left;
	pt.y -= rr.top;
	ClientToWorld(&pt, rr, &m_CpZoom);

	m_ptZoom = pt;

	if (zDelta > 0) {
		m_fZoomLevel *= 1.10f;
	}
	else {
		m_fZoomLevel *= 0.90f;
	}
	MinMax(&m_fZoomLevel, 0.2f, 6.0f);
	Invalidate(FALSE);

	if (m_pTranslationDlg) m_pTranslationDlg->UpdateAllControls();

	return CResizablePage::OnMouseWheel(nFlags, zDelta, pt);
}

void C3DViewEditDlg::WorldToClient(COORDDATA *pCp, CRect rr, CPoint *pt, int nSide)
{
	float fWidth = (float)rr.Width();
	float fHeight = (float)rr.Height();

	if ((theApp.m_LastSettings.nOrientation3DEditImage & 2) == 0) {
		switch (m_nDisplayFastAxis) {
		case 0:	pt->x = (int)(m_ptZoom.x + (int)((pCp->Side[nSide].fX - m_CpZoom.Side0.fX)*fWidth / (m_fX1p - m_fX0p) * m_fZoomLevel));
			break;
		case 1:	pt->x = (int)(m_ptZoom.x + (int)((pCp->Side[nSide].fY - m_CpZoom.Side0.fY)*fWidth / (m_fY1p - m_fY0p) * m_fZoomLevel));
			break;
		case 2:	pt->x = (int)(m_ptZoom.x + (int)((pCp->Side[nSide].fZ - m_CpZoom.Side0.fZ)*fWidth / (m_fZ1p - m_fZ0p) * m_fZoomLevel));
			break;
		}
	}
	else {
		switch (m_nDisplayFastAxis) {
		case 0:	pt->x = (int)(m_ptZoom.x - (int)((pCp->Side[nSide].fX - m_CpZoom.Side0.fX)*fWidth / (m_fX1p - m_fX0p) * m_fZoomLevel));
			break;
		case 1:	pt->x = (int)(m_ptZoom.x - (int)((pCp->Side[nSide].fY - m_CpZoom.Side0.fY)*fWidth / (m_fY1p - m_fY0p) * m_fZoomLevel));
			break;
		case 2:	pt->x = (int)(m_ptZoom.x - (int)((pCp->Side[nSide].fZ - m_CpZoom.Side0.fZ)*fWidth / (m_fZ1p - m_fZ0p) * m_fZoomLevel));
			break;
		}
	}

	switch (m_nDisplaySlowAxis) {
	case 0:
		if ((theApp.m_LastSettings.nOrientation3DEditImage & 1) == 0) {
			pt->y = (int)(m_ptZoom.y - (int)((pCp->Side[nSide].fX - m_CpZoom.Side0.fX)*fHeight / (m_fX1p - m_fX0p) * m_fZoomLevel));
		}
		else {
			pt->y = (int)(m_ptZoom.y + (int)((pCp->Side[nSide].fX - m_CpZoom.Side0.fX)*fHeight / (m_fX1p - m_fX0p) * m_fZoomLevel));
		}
		break;
	case 1:
		if ((theApp.m_LastSettings.nOrientation3DEditImage & 1) == 0) {
			pt->y = (int)(m_ptZoom.y - (int)((pCp->Side[nSide].fY - m_CpZoom.Side0.fY)*fHeight / (m_fY1p - m_fY0p) * m_fZoomLevel));
		}
		else {
			pt->y = (int)(m_ptZoom.y + (int)((pCp->Side[nSide].fY - m_CpZoom.Side0.fY)*fHeight / (m_fY1p - m_fY0p) * m_fZoomLevel));
		}
		break;
	case 2:
		if ((theApp.m_LastSettings.nOrientation3DEditImage & 1) == 0) {
			pt->y = (int)(m_ptZoom.y - (int)((pCp->Side[nSide].fZ - m_CpZoom.Side0.fZ)*fHeight / (m_fZ1p - m_fZ0p) * m_fZoomLevel));
		}
		else {
			pt->y = (int)(m_ptZoom.y + (int)((pCp->Side[nSide].fZ - m_CpZoom.Side0.fZ)*fHeight / (m_fZ1p - m_fZ0p) * m_fZoomLevel));
		}
		break;
	}
}

void C3DViewEditDlg::WorldToClient(COORDDATA *pCp, CRectFloat rr, CPoint *pt, int nSide)
{
	CRect rrect((int)rr.left, (int)rr.top, (int)rr.right, (int)rr.bottom);

	WorldToClient(pCp, rrect, pt, nSide);
}

void C3DViewEditDlg::ClientToWorld(CPoint* pt, CRect rr, COORDDATA *pCp)
{
	float fWidth = (float)rr.Width();
	float fHeight = (float)rr.Height();

	if ((theApp.m_LastSettings.nOrientation3DEditImage & 2) == 0) {
		switch (m_nDisplayFastAxis) {
		case 0:	pCp->Side[PORTSIDE].fX = (((float)pt->x - m_ptZoom.x) * (m_fX1p - m_fX0p) / fWidth) / m_fZoomLevel + m_CpZoom.Side0.fX;
			break;
		case 1: pCp->Side[PORTSIDE].fY = (((float)pt->x - m_ptZoom.x) * (m_fY1p - m_fY0p) / fWidth) / m_fZoomLevel + m_CpZoom.Side0.fY;
			break;
		case 2: pCp->Side[PORTSIDE].fZ = (((float)pt->x - m_ptZoom.x) * (m_fZ1p - m_fZ0p) / fWidth) / m_fZoomLevel + m_CpZoom.Side0.fZ;
			break;
		}
	}
	else {
		switch (m_nDisplayFastAxis) {
		case 0:	pCp->Side[PORTSIDE].fX = (((float)m_ptZoom.x - pt->x) * (m_fX1p - m_fX0p) / fWidth) / m_fZoomLevel + m_CpZoom.Side0.fX;
			break;
		case 1: pCp->Side[PORTSIDE].fY = (((float)m_ptZoom.x - pt->x) * (m_fY1p - m_fY0p) / fWidth) / m_fZoomLevel + m_CpZoom.Side0.fY;
			break;
		case 2: pCp->Side[PORTSIDE].fZ = (((float)m_ptZoom.x - pt->x) * (m_fZ1p - m_fZ0p) / fWidth) / m_fZoomLevel + m_CpZoom.Side0.fZ;
			break;
		}
	}

	switch (m_nDisplaySlowAxis) {
	case 0:
		if ((theApp.m_LastSettings.nOrientation3DEditImage & 1) == 0) {
			pCp->Side[PORTSIDE].fX = ((float)(m_ptZoom.y - pt->y) * (m_fX1p - m_fX0p) / fHeight) / m_fZoomLevel + m_CpZoom.Side0.fX;
		}
		else {
			pCp->Side[PORTSIDE].fX = ((float)(pt->y - m_ptZoom.y) * (m_fX1p - m_fX0p) / fHeight) / m_fZoomLevel + m_CpZoom.Side0.fX;
		}
		break;
	case 1:
		if ((theApp.m_LastSettings.nOrientation3DEditImage & 1) == 0) {
			pCp->Side[PORTSIDE].fY = ((float)(m_ptZoom.y - pt->y) * (m_fY1p - m_fY0p) / fHeight) / m_fZoomLevel + m_CpZoom.Side0.fY;
		}
		else {
			pCp->Side[PORTSIDE].fY = ((float)(pt->y - m_ptZoom.y) * (m_fY1p - m_fY0p) / fHeight) / m_fZoomLevel + m_CpZoom.Side0.fY;
		}
		break;
	case 2:
		if ((theApp.m_LastSettings.nOrientation3DEditImage & 1) == 0) {
			pCp->Side[PORTSIDE].fZ = ((float)(m_ptZoom.y - pt->y) * (m_fZ1p - m_fZ0p) / fHeight) / m_fZoomLevel + m_CpZoom.Side0.fZ;
		}
		else {
			pCp->Side[PORTSIDE].fZ = ((float)(pt->y - m_ptZoom.y) * (m_fZ1p - m_fZ0p) / fHeight) / m_fZoomLevel + m_CpZoom.Side0.fZ;
		}
		break;
	}

}


void C3DViewEditDlg::On3dviewRecenterimage()
{
	Centerimage();
	FindAllLimits(true);

	Invalidate(false);
}

void C3DViewEditDlg::Centerimage()
{
	CRect rr;

	if (m_staticView.GetSafeHwnd()) {
		m_staticView.GetWindowRect(&rr);
		ScreenToClient(&rr);

		m_ptZoom = rr.CenterPoint();
		m_ptZoom.x -= rr.left;
		m_ptZoom.y -= rr.top;
	}
}

void C3DViewEditDlg::On3dviewProperties()
{
	FRAME;
	pFrame->SendMessage(UI_SHOW_PROPERTIES_PANE, NULL, Profile);
}

long C3DViewEditDlg::SetToolBarCheckedState(unsigned int nTemp, long nlTemp)
{
	int	nIndex, nFlag;

	if (m_wndViewToolBar.m_hWnd) {
		
		if ((nIndex = m_wndViewToolBar.CommandToIndex(ID_VIEW_CROSSES)) >= 0) {
			switch (theApp.m_nEditLineType) {
			case TAUGHTLINES: theApp.m_LastSettings.bDisplayTaughtCrosses == TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex, 0);
				break;
			case SCANLINES: theApp.m_LastSettings.bDisplayScanCrosses == TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex, 0);
				break;
			case BRAINLINES: theApp.m_LastSettings.bDisplayBrainCrosses == TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex, 0);
				break;
			case ENVELOPELINES: theApp.m_LastSettings.bDisplayEnvelopeCrosses == TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex, 0);
				break;
			case SURFACELINES: theApp.m_LastSettings.bDisplaySurfaceCrosses == TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex, 0);
				break;
			case OUTSIDELINES: theApp.m_LastSettings.bDisplayBladeCrosses[OUTSIDE] == TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex, 0);
				break;
			case INSIDELINES: theApp.m_LastSettings.bDisplayBladeCrosses[INSIDE] == TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex, 0);
				break;
			}
		}
		if ((nIndex = m_wndViewToolBar.CommandToIndex(ID_VIEW_LINES)) >= 0) {
			switch (theApp.m_nEditLineType) {
			case TAUGHTLINES: theApp.m_LastSettings.bDisplayTaughtLines == TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex, 0);
				break;
			case SCANLINES: theApp.m_LastSettings.bDisplayScanLines == TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex, 0);
				break;
			case BRAINLINES: theApp.m_LastSettings.bDisplayBrainLines == TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex, 0);
				break;
			case ENVELOPELINES: theApp.m_LastSettings.bDisplayEnvelopeLines == TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex, 0);
				break;
			case SURFACELINES: theApp.m_LastSettings.bDisplaySurfaceLines == TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex, 0);
				break;
			case OUTSIDELINES: theApp.m_LastSettings.bDisplayBladeLines[OUTSIDE] == TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex, 0);
				break;
			case INSIDELINES: theApp.m_LastSettings.bDisplayBladeLines[INSIDE] == TRUE ? m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndViewToolBar.SetButtonStyle(nIndex, 0);
				break;
			}
		}
		
		nIndex = m_wndViewToolBar.CommandToIndex(ID_VIEW_XY);
		if (PROFILE->m_nFastAxis == 0 && PROFILE->m_nSlowAxis == 1) {
			m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED);
		}
		else {
			m_wndViewToolBar.SetButtonStyle(nIndex, 0);
		}
		nIndex = m_wndViewToolBar.CommandToIndex(ID_VIEW_XZ);
		if (PROFILE->m_nFastAxis == 0 && PROFILE->m_nSlowAxis == 2) {
			m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED);
		}
		else {
			m_wndViewToolBar.SetButtonStyle(nIndex, 0);
		}
		nIndex = m_wndViewToolBar.CommandToIndex(ID_VIEW_YZ);
		if (PROFILE->m_nFastAxis == 1 && PROFILE->m_nSlowAxis == 2) {
			m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_CHECKED);
		}
		else {
			m_wndViewToolBar.SetButtonStyle(nIndex, 0);
		}
		m_wndViewToolBar.RedrawWindow();
	}


	if (theApp.m_nLogonLevelMask & theApp.m_cAccess[IDD_3D_EDIT_DIALOG]) {

		if (m_wndEditToolBar.m_hWnd) {
			if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_TAUGHT_LINES)) >= 0)
				theApp.m_nEditLineType == TAUGHTLINES ? m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndEditToolBar.SetButtonStyle(nIndex, 0);
			if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_SCAN_LINES)) >= 0)
				theApp.m_nEditLineType == SCANLINES ? m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndEditToolBar.SetButtonStyle(nIndex, 0);
			if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_BRAIN_LINES)) >= 0)
				theApp.m_nEditLineType == BRAINLINES ? m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndEditToolBar.SetButtonStyle(nIndex, 0);
			if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_ENVELOPE_LINES)) >= 0)
				theApp.m_nEditLineType == ENVELOPELINES ? m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndEditToolBar.SetButtonStyle(nIndex, 0);
			if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_HOLES)) >= 0)
				theApp.m_nEditLineType == HOLES ? m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndEditToolBar.SetButtonStyle(nIndex, 0);
			if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_TRANSFORM)) >= 0)
				theApp.m_nEditLineType == TRANSFORM ? m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndEditToolBar.SetButtonStyle(nIndex, 0);
			if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_HOLDINGFIXTURE)) >= 0)
				theApp.m_nEditLineType == HOLDINGFIXTURE ? m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndEditToolBar.SetButtonStyle(nIndex, 0);
			if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_WAYPOINTS)) >= 0)
				m_bDrawWayPts == TRUE ? m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndEditToolBar.SetButtonStyle(nIndex, 0);
			if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_SURFACE_LINES)) >= 0)
				theApp.m_nEditLineType == SURFACELINES ? m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndEditToolBar.SetButtonStyle(nIndex, 0);
			if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_BUTTON_EDIT_OUTSIDE)) >= 0)
				theApp.m_nEditLineType == OUTSIDELINES ? m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndEditToolBar.SetButtonStyle(nIndex, 0);
			if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_BUTTON_EDIT_INSIDE)) >= 0)
				theApp.m_nEditLineType == INSIDELINES ? m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_CHECKED) : m_wndEditToolBar.SetButtonStyle(nIndex, 0);
		}


		switch (theApp.m_nEditLineType) {
		case TAUGHTLINES:
		case ENVELOPELINES: nFlag = 0;
			break;
		default: nFlag = TBBS_DISABLED;
			break;
		}
		if ((m_wndCoordinateToolBar.m_hWnd != 0) && (nIndex = m_wndCoordinateToolBar.CommandToIndex(ID_BUTTON_ADD_COORDINATE)) >= 0) m_wndCoordinateToolBar.SetButtonStyle(nIndex, nFlag);

		switch (theApp.m_nEditLineType) {
		case TAUGHTLINES:
		case BRAINLINES:
		case SCANLINES:
		case ENVELOPELINES: nFlag = 0;
			break;
		default: nFlag = TBBS_DISABLED;
			break;
		}
		if ((m_wndCoordinateToolBar.m_hWnd != 0) && (nIndex = m_wndCoordinateToolBar.CommandToIndex(ID_BUTTON_DELETE_COORDINATE)) >= 0) m_wndCoordinateToolBar.SetButtonStyle(nIndex, nFlag);
		if ((m_wndCoordinateToolBar.m_hWnd != 0) && (nIndex = m_wndCoordinateToolBar.CommandToIndex(ID_BUTTON_NEW_LINE)) >= 0) m_wndCoordinateToolBar.SetButtonStyle(nIndex, nFlag);
		if ((m_wndCoordinateToolBar.m_hWnd != 0) && (nIndex = m_wndCoordinateToolBar.CommandToIndex(ID_BUTTON_SELECTAREA)) >= 0) {
			m_bSelectArea == TRUE ? nFlag |= TBBS_CHECKED : nFlag;
			m_wndCoordinateToolBar.SetButtonStyle(nIndex, nFlag);
		}
	}
	else {

		if ((m_wndViewToolBar.m_hWnd != 0) && (nIndex = m_wndViewToolBar.CommandToIndex(ID_VIEW_CROSSES)) >= 0) m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wndViewToolBar.m_hWnd != 0) && (nIndex = m_wndViewToolBar.CommandToIndex(ID_VIEW_LINES)) >= 0) m_wndViewToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);

		if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_TAUGHT_LINES)) >= 0) m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_SCAN_LINES)) >= 0) m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_BRAIN_LINES)) >= 0) m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_ENVELOPE_LINES)) >= 0) m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_HOLES)) >= 0) m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_TRANSFORM)) >= 0) m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_HOLDINGFIXTURE)) >= 0) m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wndEditToolBar.m_hWnd != 0) && (nIndex = m_wndEditToolBar.CommandToIndex(ID_EDIT_WAYPOINTS)) >= 0) m_wndEditToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);


		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_VIEW_POINT)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_COMPONENT)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_MACHINE)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_DANGER_ZONE)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_ISOMETRIC)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_CSCAN)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_SCANLINES)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_REF_POINTS)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_ENEVLOPE)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);


		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_VIEW_POINT)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_COMPONENT)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_MACHINE)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_DANGER_ZONE)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_ISOMETRIC)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_CSCAN)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_SCANLINES)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_REF_POINTS)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wnd3DToolBar.m_hWnd != 0) && (nIndex = m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_ENEVLOPE)) >= 0) m_wnd3DToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wndCoordinateToolBar.m_hWnd != 0) && (nIndex = m_wndCoordinateToolBar.CommandToIndex(ID_BUTTON_ADD_COORDINATE)) >= 0) m_wndCoordinateToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wndCoordinateToolBar.m_hWnd != 0) && (nIndex = m_wndCoordinateToolBar.CommandToIndex(ID_BUTTON_DELETE_COORDINATE)) >= 0) m_wndCoordinateToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wndCoordinateToolBar.m_hWnd != 0) && (nIndex = m_wndCoordinateToolBar.CommandToIndex(ID_BUTTON_NEW_LINE)) >= 0) m_wndCoordinateToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
		if ((m_wndCoordinateToolBar.m_hWnd != 0) && (nIndex = m_wndCoordinateToolBar.CommandToIndex(ID_BUTTON_SELECTAREA)) >= 0) m_wndCoordinateToolBar.SetButtonStyle(nIndex, TBBS_DISABLED);
	}
	return 0;
}


void C3DViewEditDlg::OnUpdateViewLines(CCmdUI *pCmdUI)
{
	switch (theApp.m_nEditLineType) {
	case TAUGHTLINES: theApp.m_LastSettings.bDisplayTaughtLines == TRUE ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
		break;
	case SCANLINES: theApp.m_LastSettings.bDisplayScanLines == TRUE ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
		break;
	case BRAINLINES: theApp.m_LastSettings.bDisplayBrainLines == TRUE ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
		break;
	case ENVELOPELINES: theApp.m_LastSettings.bDisplayEnvelopeLines == TRUE ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
		break;
	case SURFACELINES: theApp.m_LastSettings.bDisplaySurfaceLines == TRUE ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
		break;
	case OUTSIDELINES: theApp.m_LastSettings.bDisplayBladeLines[OUTSIDE] == TRUE ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
		break;
	case INSIDELINES: theApp.m_LastSettings.bDisplayBladeLines[INSIDE] == TRUE ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
		break;
	}
}


void C3DViewEditDlg::OnUpdateViewCrosses(CCmdUI *pCmdUI)
{
	switch (theApp.m_nEditLineType) {
	case TAUGHTLINES: theApp.m_LastSettings.bDisplayTaughtCrosses == TRUE ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
		break;
	case SCANLINES: theApp.m_LastSettings.bDisplayScanCrosses == TRUE ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
		break;
	case BRAINLINES: theApp.m_LastSettings.bDisplayBrainCrosses == TRUE ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
		break;
	case ENVELOPELINES: theApp.m_LastSettings.bDisplayEnvelopeCrosses == TRUE ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
		break;
	case SURFACELINES: theApp.m_LastSettings.bDisplaySurfaceCrosses == TRUE ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
		break;
	case OUTSIDELINES: theApp.m_LastSettings.bDisplayBladeCrosses[OUTSIDE] == TRUE ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
		break;
	case INSIDELINES: theApp.m_LastSettings.bDisplayBladeCrosses[INSIDE] == TRUE ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
		break;
	}
}
