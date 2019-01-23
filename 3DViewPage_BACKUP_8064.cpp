<<<<<<< HEAD
// 3DViewPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "3DViewPage.h"
#include "ProfileSheet.h"
#include "NURBS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DViewPage property page
bool m_bSizeChanged;
 
IMPLEMENT_DYNCREATE(C3DViewPage, CResizablePage)

C3DViewPage::C3DViewPage() : CResizablePage(C3DViewPage::IDD)
{
	//{{AFX_DATA_INIT(C3DViewPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_p3DObjectScanner = NULL;
	m_p3DObjectScanLines = NULL;
	m_p3DObjectBraneLines = NULL;
	m_p3DObjectTaughtLines = NULL;
	m_p3DObjectEnvelopeLines = NULL;
	m_p3DObjectRefPoints = NULL;
	m_p3DObjectCScanOverlay = NULL;
	m_p3DObjectRoom = NULL;
	m_p3DObjectDangerPlanes = NULL;
	m_p3DObjectTurntable = NULL;
	m_p3DObjectLookAtPoint = NULL;
	m_p3DObjectScratchLines = NULL;
	m_p3DObjectSurfaceLines = NULL;
	m_p3DObjectDataVertices = NULL;
	m_p3DObjectOutsideLines = NULL;
	m_p3DObjectInsideLines = NULL;
	m_p3DObjectRobot = NULL;
	m_p3DObjectXYZ = NULL;


	m_pSphere = NULL;
	m_pCone = NULL;
	m_pRobotElement0 = NULL;
	m_pRobotElement1 = NULL;
	m_pRobotElement2 = NULL;
	m_pRobotElement3 = NULL;
	m_pRobotElement4 = NULL;
	m_pRobotElement5 = NULL;

	m_pD3D       = NULL;
	m_pd3dDevice = NULL;

	m_p3DFont = NULL;
	m_p3DFontSmall = NULL;
	m_pComponentMesh = NULL;
	m_pWorldMesh = NULL;
	m_pTexFloor = NULL;
	m_pTexWall = NULL;
	m_pTexScope = NULL;
	m_pTexComponent = NULL;
	m_nTexComponentL = 0;
	m_b3DCScan = FALSE;
	m_b3DSurface = FALSE;
	m_b3DDataVertices = FALSE;
	ZeroMemory(	m_pMesh3DText, sizeof m_pMesh3DText);


	m_bSizeChanged = false;

	ZeroMemory(&m_PrimFrame,sizeof m_PrimFrame);
//	ZeroMemory(&m_PrimComp,sizeof m_PrimComp);


	InitializeEyeAndLookAtPt();

	switch(theApp.m_LastSettings.n3DUpVector) {
	default: m_vUpVec = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		break;
	case 1: m_vUpVec = D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
		break;
	}

	m_bLbDown = false;
	m_bRbDown = false;
	m_bMbDown = false;
	m_n3DMoveEyeComponent = 0;


}

C3DViewPage::~C3DViewPage()
{
	CleanUp3D();

}

void C3DViewPage::OnKickIdle()
{
	UpdateDialogControls(this, FALSE);
}


void C3DViewPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3DViewPage)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3DViewPage, CResizablePage)
	//{{AFX_MSG_MAP(C3DViewPage)
	ON_MESSAGE_VOID(WM_KICKIDLE, OnKickIdle)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_3D_VIEW_POINT, OnButton3dViewPoint)
	ON_COMMAND(ID_BUTTON_3D_COMPONENT, OnButton3dComponent)
	ON_COMMAND(ID_BUTTON_3D_DANGER_ZONE, OnButton3dDangerZone)
	ON_COMMAND(ID_BUTTON_3D_MACHINE, OnButton3dMachine)
	ON_COMMAND(ID_BUTTON_3D_REF_POINTS, OnButton3dRefPoints)
	ON_COMMAND(ID_BUTTON_3D_SCANLINES, OnButton3dScanlines)
	ON_COMMAND(ID_BUTTON_3D_ENVELOPE, OnButton3dEnvelope)
	ON_COMMAND(ID_BUTTON_3D_CSCAN, OnButton3dCscan)
	ON_COMMAND(ID_BUTTON_3D_TOOLS, OnButton3dTools)
	ON_COMMAND(ID_BUTTON_3D_TAUGHTLINES, OnButton3dTaughtlines)
	ON_COMMAND(ID_BUTTON_3D_DATA_VERTICES, OnButton3dDataVertices)
	ON_COMMAND(ID_BUTTON_3D_ROBOT, OnButton3dRobot)
	ON_COMMAND(ID_BUTTON_3D_BALL_ROLL, OnButton3dBallRoll)
	ON_COMMAND(ID_BUTTON_3D_ROTATE_ABOUT_Z, OnButton3dRotateAboutZ)
	ON_COMMAND(ID_BUTTON_3D_ROTATE_ABOUT_X, OnButton3dRotateAboutX)
	ON_COMMAND(ID_BUTTON_3D_TRACKER, OnButton3dTracker)
	ON_COMMAND(ID_BUTTON_3D_ROTATE_ABOUT_Y, OnButton3dRotateAboutY)
	ON_COMMAND(ID_BUTTON_3D_BRANELINES, OnButton3dBranelines)
	ON_COMMAND(ID_BUTTON_DISPLAY_INSIDE, OnButtonDisplayInside)
	ON_COMMAND(ID_BUTTON_DISPLAY_OUTSIDE, OnButtonDisplayOutside)
	ON_COMMAND(ID_BUTTON_DISPLAY_WORKSHEET_PROFILES, OnButtonDisplayWorksheetProfiles)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DViewPage message handlers
int C3DViewPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CResizablePage::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wnd3DToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wnd3DToolBar.LoadToolBar(IDR_PROFILE_3D_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}


	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()+270,30,SWP_SHOWWINDOW);


	m_wndRebar.AddBar(&m_wnd3DToolBar);
	
	return 0;
}

BOOL C3DViewPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();

	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);
	m_StaticPosView.SetNewPos();
	
	
	Start3D();
	SetToolBarCheckedState();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3DViewPage::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	
	if(GetSafeHwnd()) {
		m_StaticPosView.SetNewPos();
		m_bSizeChanged = true;
	}
}

BOOL C3DViewPage::OnSetActive() 
{
	CRect rr;
	m_staticView.GetWindowRect(rr);

	if(m_bSizeChanged == true) {
		if((rr.Width() > (int)m_d3dpp.BackBufferWidth) || (rr.Height() > (int)m_d3dpp.BackBufferHeight)) {
			CleanUp3D();
			Start3D();
		}
		m_bSizeChanged = false;
	}
	InitializeComponent();
	if((theApp.m_LastSettings.nDisplayTurnTableIn3D != 0) && (m_p3DObjectTurntable == NULL)) InitializeTurnTable();
	if((theApp.m_LastSettings.nDisplay3DRobot != 0) && (m_p3DObjectRobot == NULL)) InitializeRobot();

	return CResizablePage::OnSetActive();
}

inline int C3DViewPage::MinMax(int *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}


void C3DViewPage::Start3D()
{
	HWND hWnd = m_staticView.m_hWnd;
    if( SUCCEEDED( InitD3D( hWnd ) ) )  {
       InitializeObjects();
	}

}

void C3DViewPage::CleanUp3D()
{
	int	ii;

	if( m_pTexComponent ) {
		for(ii = 0; ii < m_nTexComponentL; ii++ ) {
			SAFE_RELEASE(m_pTexComponent[ii]);
        }
		SAFE_DELETE_ARRAY(m_pTexComponent);
		m_nTexComponentL=0;
    }
	SAFE_RELEASE(m_pTexScope);
	SAFE_RELEASE(m_pTexWall);
	SAFE_RELEASE(m_pTexFloor);
	SAFE_RELEASE(m_pComponentMesh);
	SAFE_RELEASE(m_pWorldMesh);
	SAFE_RELEASE(m_pd3dDevice);
	SAFE_RELEASE(m_pD3D);
	for(ii=0;ii<MAX_TEXT_STRINGS;ii++) {
		SAFE_RELEASE( m_pMesh3DText[ii] );
	}

    if(m_p3DFont) {
		m_p3DFont->InvalidateDeviceObjects();
	    m_p3DFont->DeleteDeviceObjects();
	}
    if(m_p3DFontSmall) {
	    m_p3DFontSmall->InvalidateDeviceObjects();
	    m_p3DFontSmall->DeleteDeviceObjects();
	}
    SAFE_DELETE( m_p3DFont );
    SAFE_DELETE( m_p3DFontSmall );

	SAFE_DELETE( m_p3DObjectScanner );
	SAFE_DELETE( m_p3DObjectScanLines );
	SAFE_DELETE( m_p3DObjectBraneLines );
	SAFE_DELETE( m_p3DObjectTaughtLines );
	SAFE_DELETE( m_p3DObjectEnvelopeLines );
	SAFE_DELETE( m_p3DObjectRefPoints );
	SAFE_DELETE( m_p3DObjectCScanOverlay );
	SAFE_DELETE( m_p3DObjectRoom );
	SAFE_DELETE( m_p3DObjectDangerPlanes );
	SAFE_DELETE( m_p3DObjectTurntable );
	SAFE_DELETE( m_p3DObjectLookAtPoint );
	SAFE_DELETE( m_p3DObjectScratchLines );
	SAFE_DELETE( m_p3DObjectSurfaceLines );
	SAFE_DELETE( m_p3DObjectDataVertices );
	SAFE_DELETE( m_p3DObjectRobot );
	SAFE_DELETE( m_p3DObjectXYZ );
	SAFE_DELETE( m_p3DObjectOutsideLines );
	SAFE_DELETE( m_p3DObjectInsideLines );
	SAFE_RELEASE( m_pSphere );
	SAFE_RELEASE( m_pCone );
	SAFE_RELEASE( m_pRobotElement0 );
	SAFE_RELEASE( m_pRobotElement1 );
	SAFE_RELEASE( m_pRobotElement2 );
	SAFE_RELEASE( m_pRobotElement3 );
	SAFE_RELEASE( m_pRobotElement4 );
	SAFE_RELEASE( m_pRobotElement5 );


}


HRESULT C3DViewPage::InitD3D( HWND hWnd )
{
    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice
    ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
    m_d3dpp.Windowed = TRUE;
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &m_d3dpp, &m_pd3dDevice ) ) )
    {
        return E_FAIL;
    }


    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

    m_WorldBall.SetWindow( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 1.0f );
    m_WorldBall.SetRadius( 1000.0f );
    m_CompBall.SetWindow( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 1.0f );
    m_CompBall.SetRadius( 1000.0f );

	D3DXCreateSphere(m_pd3dDevice, 15.0f, 60, 60, &m_pSphere, NULL);
	D3DXCreateCylinder(m_pd3dDevice, 0.0f, 20.0f, 80.0f,60, 10, &m_pCone, NULL);
	D3DXCreateCylinder(m_pd3dDevice, 337/2.0f, 337/2.0f, 300.0f,60, 30, &m_pRobotElement0, NULL);
	D3DXCreateCylinder(m_pd3dDevice, 250.0f, 250.0f, 380.0f,300, 60, &m_pRobotElement1, NULL);

//    hr = D3DXCreateTextureFromFile( m_pd3dDevice, theApp.m_WorkingDirectory + _T("\\Calibration Nominal Gain.bmp"), &m_pTexScope );

//    hr = D3DXCreateTextureFromFile( m_pd3dDevice, theApp.m_WallImagePathName, &m_pTexWall );
/*
    if( FAILED(hr) ) {
		m_bWallPicture = FALSE;
	} else {
		m_bWallPicture = TRUE;
	}
*/
//    hr = D3DXCreateTextureFromFile( m_pd3dDevice, theApp.m_WorkingDirectory + _T("\\ground2.bmp"), &m_pTexFloor );

//    hr = BuildWorldMesh();


//   m_p3DFont            = new CD3DFont( _T("Arial"), 12, D3DFONT_NORMAL );
//    m_p3DFontSmall       = new CD3DFont( _T("Arial"),  9, D3DFONT_NORMAL );
//    m_p3DFont->InitDeviceObjects( m_pd3dDevice );
//    m_p3DFontSmall->InitDeviceObjects( m_pd3dDevice );
//    m_p3DFont->RestoreDeviceObjects();
 //   m_p3DFontSmall->RestoreDeviceObjects();


    return S_OK;
}




void C3DViewPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	Render3D();

}

D3DXMATRIXA16    m_matText;

VOID C3DViewPage::Render3D()
{
	CString Buff;

	if(m_pd3dDevice==NULL) return;

	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(200,200,200), 1.0f, 0 );
//    m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255,255,255), 1.0f, 0 );


    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )  {
		if(theApp.m_LastSettings.bWireframe )
			m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		else
			m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

		m_pd3dDevice->SetTexture(0, NULL );

		m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

		DrawLookAtPoint();
//		if(m_bRbDown || m_bLbDown || m_bMbDown) {
//			if(m_p3DObjectLookAtPoint)		m_p3DObjectLookAtPoint->Render();
//		}

		FrameMove();
		SetupLights();

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );
		if(theApp.m_LastSettings.nDisplay3DMachine) {

//			if(m_p3DObjectRoom)			m_p3DObjectRoom->Render();
			if(m_p3DObjectScanner)		m_p3DObjectScanner->Render();
			if(m_p3DObjectTurntable &&	theApp.m_LastSettings.nDisplayTurnTableIn3D)
										m_p3DObjectTurntable->Render();
		}
		
		RenderXYZ();
		DrawRobot();
		DrawTrackerElements();

		ComponentMove();
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );

		if(m_p3DObjectTaughtLines	&& theApp.m_LastSettings.nDisplay3DTaughtLines)		m_p3DObjectTaughtLines->Render();
		if(m_p3DObjectScanLines		&& theApp.m_LastSettings.nDisplay3DScanLines)	{
			m_p3DObjectScanLines->Render();
			DrawScanViaDangerPlanePoints();
		}
		if(m_p3DObjectBraneLines	&& theApp.m_LastSettings.nDisplay3DBraneLines)		m_p3DObjectBraneLines->Render();
		if(m_p3DObjectOutsideLines	&& theApp.m_LastSettings.nDisplay3DOutsideLines)		m_p3DObjectOutsideLines->Render();
		if(m_p3DObjectInsideLines	&& theApp.m_LastSettings.nDisplay3DInsideLines)		m_p3DObjectInsideLines->Render();

		if(m_p3DObjectSurfaceLines	&& m_b3DSurface)									m_p3DObjectSurfaceLines->Render();
		if(m_p3DObjectDataVertices && m_b3DDataVertices)								m_p3DObjectDataVertices->Render();

		if(m_b3DCScan) {
			switch(theApp.m_LastSettings.nCScanOverlayMode) {
			case 0: if(m_p3DObjectCScanOverlay != NULL) m_p3DObjectCScanOverlay->Render();
				break;
			case 1:// RenderCscanOverLay();
				break;
			}
		}

		if(m_p3DObjectEnvelopeLines	&& theApp.m_LastSettings.nDisplay3DEnvelope)			m_p3DObjectEnvelopeLines->Render();
		if(m_p3DObjectRefPoints		&& theApp.m_LastSettings.nDisplay3DRefPoints)			m_p3DObjectRefPoints->Render();
//		if(m_p3DObjectRobot			&& theApp.m_LastSettings.nDisplay3DRobot)				m_p3DObjectRobot->Render();
		if(m_p3DObjectScratchLines)														m_p3DObjectScratchLines->Render();

//		if(m_bDisplayIsometric)							RenderPrimitives(m_pPrim[PRIM_ISOMETRIC],m_nNextPrim[PRIM_ISOMETRIC]);


		if((m_p3DObjectDangerPlanes != NULL) && (theApp.m_LastSettings.nDisplay3DDangerZone>0)) {
			m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
			m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
			m_p3DObjectDangerPlanes->Render();
		}




		D3DXVECTOR3 vecLength;
		D3DXVec3Subtract(&vecLength,&m_vEyePt,&m_vLookatPt);
		Buff.Format(_T("Distance to look at point %.01f mm"),D3DXVec3Length(&vecLength));
//		m_p3DFontSmall->DrawText( 2,  0, D3DCOLOR_ARGB(255,0,0,0),Buff );

        m_pd3dDevice->EndScene();
    }

    m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}


void C3DViewPage::RenderXYZ()
{
    D3DMATERIAL9 mtrl;
	D3DXMATRIX mScale,matTrans,matRotate,matText;
	float fScale = 50.0f;
	DWORD out[6];

	m_p3DObjectXYZ->Render();

	return;
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLOROP, &out[0]);
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAOP, &out[1]);
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &out[2]);
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &out[3]);
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAARG1, &out[4]);
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_CONSTANT, &out[5]);

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	//D3DTOP_SELECTARG1
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CONSTANT);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CONSTANT);

	m_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, false );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

	ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
	mtrl.Ambient.r = 1.0f;
	mtrl.Ambient.g = 0.0f;
	mtrl.Ambient.b = 0.0f;
	mtrl.Ambient.a = 1.0f;
	mtrl.Diffuse.r = 0.0f;
	mtrl.Diffuse.g = 1.0f;
	mtrl.Diffuse.b = 0.0f;
	mtrl.Diffuse.a = 0.5f;

	const D3DXCOLOR RED(D3DCOLOR_XRGB(255, 0, 0));

	mtrl.Ambient  = RED;
	mtrl.Diffuse  = RED;
	mtrl.Specular = RED;
	mtrl.Emissive = RED;
	mtrl.Power    = 0.5f;

	m_pd3dDevice->SetMaterial( &mtrl );
	D3DXMatrixScaling( &mScale, fScale,fScale,fScale);

	if( m_pMesh3DText[0] != NULL ) {	//X
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_CONSTANT, D3DCOLOR_XRGB(255,0,0));

		D3DXMatrixTranslation( &matTrans,120.0f,0.0f,-10.0f );
		D3DXMatrixRotationYawPitchRoll( &matRotate,0.0f,PIf/2.0f, 0.0f );

		matText = matRotate * mScale;
		matText *= matTrans;
		matText *= m_matWorld;

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matText );
		m_pMesh3DText[0]->DrawSubset(0);
	}

	if( m_pMesh3DText[1] != NULL ) {	//Y
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_CONSTANT, D3DCOLOR_XRGB(0,255,0));

		D3DXMatrixTranslation( &matTrans,0.0f,120.0f,-10.0f );
		D3DXMatrixRotationYawPitchRoll( &matRotate,PIf/2.0f,0.0f, PIf/2.0f );

		matText = matRotate * mScale;
		matText *= matTrans;
		matText *= m_matWorld;

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matText );
		m_pMesh3DText[1]->DrawSubset(0);
	}

	if( m_pMesh3DText[2] != NULL ) {	//Z
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_CONSTANT, D3DCOLOR_XRGB(0,0,255));

		D3DXMatrixTranslation( &matTrans,-10.0f,0.0f,120.0f );
		D3DXMatrixRotationYawPitchRoll( &matRotate,0.0f,PIf/2.0f,0.0f );

		matText = matRotate * mScale;
		matText *= matTrans;
		matText *= m_matWorld;

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matText );
		m_pMesh3DText[2]->DrawSubset(0);
	}

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, out[0]);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, out[1]);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, out[2]);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, out[3]);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, out[4]);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_CONSTANT, out[5]);

	m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, true );

}

void C3DViewPage::FrameMove()
{

	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

    D3DXMATRIXA16 matTrans;
    D3DXMATRIXA16 matRotate;
	D3DXMATRIXA16 matView;
    D3DXMATRIXA16 matProj;
    D3DXMATRIXA16 matEyePt;
	D3DXVECTOR4 vEyePt,vEyePtRot;
	D3DXVECTOR3 vLookatPt;
	D3DXVECTOR3 vTrans;


    D3DXMatrixIdentity( &m_matWorld);
    D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_WorldBall.GetTranslationMatrix() );
    D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_WorldBall.GetRotationMatrix() );


	D3DXMatrixRotationYawPitchRoll(&matEyePt,m_PrimFrame.Rotate.x,m_PrimFrame.Rotate.y, m_PrimFrame.Rotate.z );
	D3DXVec3Transform(&vEyePtRot,&m_vEyePt,&matEyePt);

	vLookatPt = m_vLookatPt + m_PrimFrame.Translate;
	vEyePtRot.x += m_PrimFrame.Translate.x;
	vEyePtRot.y += m_PrimFrame.Translate.y;
	vEyePtRot.z += m_PrimFrame.Translate.z;
	D3DXMatrixLookAtRH( &matView, (D3DXVECTOR3*)&vEyePtRot, &vLookatPt, &m_vUpVec );
	m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );


    FLOAT m_fObjectRadius = 1.0f;
	FLOAT fAspect = (FLOAT)rr.Width() / (FLOAT)rr.Height();
	D3DXMatrixPerspectiveFovRH( &matProj, D3DX_PI/4, fAspect,1.0f, 20000.0f );
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

}

void C3DViewPage::ComponentMove()
{

    D3DXMatrixIdentity( &m_matWorld);
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_CompBall.GetTranslationMatrix() );
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_CompBall.GetRotationMatrix() );
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_WorldBall.GetTranslationMatrix() );
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_WorldBall.GetRotationMatrix() );

}

void C3DViewPage::SetupLights()
{
    D3DXVECTOR3 vecDir;
	int nLight;
   
	D3DMATERIAL9 mtrl;
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
    m_pd3dDevice->SetMaterial( &mtrl );

	for(nLight = 0;nLight<8;nLight++) {
		if(theApp.m_LastSettings.light[nLight].Type) {
			theApp.m_LastSettings.light[nLight].Diffuse.r = 0.6f;
			theApp.m_LastSettings.light[nLight].Diffuse.g = 0.6f;
			theApp.m_LastSettings.light[nLight].Diffuse.b = 0.6f;

			theApp.m_LastSettings.light[nLight].Specular.r = 0.0f;
			theApp.m_LastSettings.light[nLight].Specular.g = 0.0f;
			theApp.m_LastSettings.light[nLight].Specular.b = 0.0f;

			theApp.m_LastSettings.light[nLight].Ambient.r = 0.0f;
			theApp.m_LastSettings.light[nLight].Ambient.g = 0.0f;
			theApp.m_LastSettings.light[nLight].Ambient.b = 0.0f;

			vecDir.x = theApp.m_LastSettings.vecLightLookAtPt[nLight].x - theApp.m_LastSettings.light[nLight].Position.x;
			vecDir.y = theApp.m_LastSettings.vecLightLookAtPt[nLight].y - theApp.m_LastSettings.light[nLight].Position.y;
			vecDir.z = theApp.m_LastSettings.vecLightLookAtPt[nLight].z - theApp.m_LastSettings.light[nLight].Position.z;

			D3DXVec3Normalize( (D3DXVECTOR3*)&theApp.m_LastSettings.light[nLight].Direction, &vecDir );

			theApp.m_LastSettings.light[nLight].Range = 6000.0f;
			theApp.m_LastSettings.light[nLight].Falloff = 1.0f;          /* Falloff */
			theApp.m_LastSettings.light[nLight].Attenuation0 = 1.0f;     /* Constant attenuation */
			theApp.m_LastSettings.light[nLight].Attenuation1 = 0.0f;     /* Linear attenuation */
			theApp.m_LastSettings.light[nLight].Attenuation2 = 0.0f;     /* Quadratic attenuation */

			m_pd3dDevice->SetLight( nLight, &theApp.m_LastSettings.light[nLight] );
			m_pd3dDevice->LightEnable( nLight, TRUE );
		} else {
			m_pd3dDevice->LightEnable( nLight, FALSE );
		}
	}

	int nAmbient = theApp.m_LastSettings.nLightAmbient;
	int nTemp = (nAmbient << 16) | (nAmbient << 8) | nAmbient;
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_AMBIENT, nTemp) ;

}



void C3DViewPage::OnLButtonDown(UINT nFlags, CPoint point) 
{

	CRect rr;
	m_staticView.GetWindowRect(rr);

    m_WorldBall.SetWindow( rr.Width(), rr.Height(), 1.0f );
    m_WorldBall.SetRadius( 1000.0f );
    m_CompBall.SetWindow( rr.Width(), rr.Height(), 1.0f );
    m_CompBall.SetRadius( 1000.0f );

	if(m_n3DMoveEyeComponent==0) {
		m_WorldBall.m_vDown = m_WorldBall.ScreenToVector( point.x, point.y );
        m_WorldBall.m_qDown = m_WorldBall.m_qNow;
		m_WorldBall.m_bDrag = TRUE;
	} else {
		m_CompBall.m_vDown = m_CompBall.ScreenToVector( point.x, point.y );
        m_CompBall.m_qDown = m_CompBall.m_qNow;
		m_CompBall.m_bDrag = TRUE;
	}

	m_bLbDown = true;
	Render3D();
	
	CResizablePage::OnLButtonDown(nFlags, point);
}

void C3DViewPage::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLbDown = false;
	Render3D();

	CResizablePage::OnLButtonUp(nFlags, point);
}

void C3DViewPage::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(m_n3DMoveEyeComponent==0) {
		m_WorldBall.m_iCurMouseX = point.x;
		m_WorldBall.m_iCurMouseY = point.y;
	} else {
		m_CompBall.m_iCurMouseX = point.x;
		m_CompBall.m_iCurMouseY = point.y;
	}
	m_bRbDown = true;
	Render3D();
	
	CResizablePage::OnRButtonDown(nFlags, point);
}

void C3DViewPage::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_bRbDown = false;
	Render3D();
	
	CResizablePage::OnRButtonUp(nFlags, point);
}

void C3DViewPage::OnMButtonDown(UINT nFlags, CPoint point)
{
	if(m_n3DMoveEyeComponent==0) {
		m_WorldBall.m_iCurMouseX = point.x;
		m_WorldBall.m_iCurMouseY = point.y;
	} else {
		m_CompBall.m_iCurMouseX = point.x;
		m_CompBall.m_iCurMouseY = point.y;
	}
	m_bMbDown = TRUE;
	Render3D();
	CDialog::OnRButtonDown(nFlags, point);
}

void C3DViewPage::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_bMbDown = FALSE;
	Render3D();

	CDialog::OnRButtonUp(nFlags, point);
}

BOOL C3DViewPage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	MouseWheel(nFlags, zDelta, pt);
	return CResizablePage::OnMouseWheel(nFlags, zDelta, pt);
}

void C3DViewPage::MouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if(zDelta>0) {
		m_vEyePt.x = (float)MulDiv((int)m_vEyePt.x,110,100);
		m_vEyePt.y = (float)MulDiv((int)m_vEyePt.y,110,100);
		m_vEyePt.z = (float)MulDiv((int)m_vEyePt.z,110,100);
	} else {
		m_vEyePt.x = (float)MulDiv((int)m_vEyePt.x,90,100);
		m_vEyePt.y = (float)MulDiv((int)m_vEyePt.y,90,100);
		m_vEyePt.z = (float)MulDiv((int)m_vEyePt.z,90,100);
	}

	Render3D();

}


inline D3DXQUATERNION* WINAPI D3DXQuaternionUnitAxisToUnitAxis2
( D3DXQUATERNION *pOut, const D3DXVECTOR3 *pvFrom, const D3DXVECTOR3 *pvTo, const BOOL bRightHanded)
{
    D3DXVECTOR3 vAxis;
    D3DXVec3Cross(&vAxis, pvFrom, pvTo);    // proportional to sin(theta)
	if(bRightHanded == TRUE) {
		pOut->x = vAxis.x;
		pOut->y = -vAxis.z;
		pOut->z = vAxis.y;
	} else {
		pOut->x = vAxis.x;
		pOut->y = vAxis.y;
		pOut->z = vAxis.z;
	}
//		pOut->x = vAxis.x;
//		pOut->y = -vAxis.z;
//		pOut->z = -vAxis.y;
    pOut->w = D3DXVec3Dot( pvFrom, pvTo );
    return pOut;
}

inline D3DXQUATERNION* WINAPI D3DXQuaternionAxisToAxis
( D3DXQUATERNION *pOut, const D3DXVECTOR3 *pvFrom, const D3DXVECTOR3 *pvTo, const BOOL bRightHanded)
{
    D3DXVECTOR3 vA, vB;
    D3DXVec3Normalize(&vA, pvFrom);
    D3DXVec3Normalize(&vB, pvTo);
    D3DXVECTOR3 vHalf(vA + vB);
    D3DXVec3Normalize(&vHalf, &vHalf);
    return D3DXQuaternionUnitAxisToUnitAxis2(pOut, &vA, &vHalf,bRightHanded);
}

void C3DViewPage::OnMouseMove(UINT nFlags, CPoint point) 
{
	CD3DArcBall_10 *pBall;
	D3DXVECTOR3 vec;
	D3DXQUATERNION  q;
	float fAngle;
	D3DXMATRIX invMat;

	if(m_n3DMoveEyeComponent==0) {
		pBall = &m_WorldBall;
	} else {
		pBall = &m_CompBall;
	}
	if( m_bLbDown == TRUE ) {
		if( pBall->m_bDrag ) {
			// recompute m_qNow
			D3DXVECTOR3 vCur = pBall->ScreenToVector( point.x, point.y );
			D3DXQUATERNION qAxisToAxis;
			D3DXQuaternionAxisToAxis(&qAxisToAxis, &pBall->m_vDown, &vCur, pBall->m_bRightHanded);
			pBall->m_qNow = pBall->m_qDown;
			pBall->m_qNow *= qAxisToAxis;
			D3DXMatrixRotationQuaternion(&pBall->m_matRotationDelta, &qAxisToAxis);
		} else {
			D3DXMatrixIdentity(&pBall->m_matRotationDelta);
		}
		D3DXMatrixRotationQuaternion(&pBall->m_matRotation, &pBall->m_qNow);
		pBall->m_bDrag = TRUE;
	} else {
		if( m_bRbDown || m_bMbDown)  {
			// Normalize based on size of window and bounding sphere radius
			FLOAT fDeltaX = ( pBall->m_iCurMouseX-point.x ) * pBall->m_fRadiusTranslation / pBall->m_iWidth;
			FLOAT fDeltaY = ( pBall->m_iCurMouseY-point.y ) * pBall->m_fRadiusTranslation / pBall->m_iHeight;
			if( m_bRbDown )
			{
				vec = D3DXVECTOR3( 2*fDeltaX, 2*fDeltaY, 0.0f );
				D3DXMatrixInverse(&invMat,NULL,&pBall->m_matRotation);
				D3DXVec3TransformCoord(&vec, &vec, &invMat);
				if(theApp.m_LastSettings.n3DUpVector == 1) {
					D3DXMatrixTranslation( &pBall->m_matTranslationDelta,-vec.x, -vec.y, vec.z );
				} else {
					D3DXMatrixTranslation( &pBall->m_matTranslationDelta,-vec.x, vec.y, vec.z );
				}
				D3DXMatrixMultiply( &pBall->m_matTranslation, &pBall->m_matTranslation, &pBall->m_matTranslationDelta );
			} else  {// wParam & MK_MBUTTON
				if(theApp.m_LastSettings.n3DUpVector == 1) {
					D3DXMatrixTranslation( &pBall->m_matTranslationDelta, 0.0f, 0.0f, -5*fDeltaY );
				} else {
					D3DXMatrixTranslation( &pBall->m_matTranslationDelta, 0.0f, 0.0f, 5*fDeltaY );
				}
				D3DXMatrixMultiply( &pBall->m_matTranslation, &pBall->m_matTranslation, &pBall->m_matTranslationDelta );
			}

			// Store mouse coordinate
			pBall->m_iCurMouseX=point.x;
			pBall->m_iCurMouseY=point.y;
		}
	}


	D3DXQuaternionToAxisAngle(D3DXQuaternionNormalize(&q,&m_CompBall.m_qNow),&vec,&fAngle);
	PROFILE->m_fRotateX = vec.x * (RAD_TO_DEG * fAngle);
	PROFILE->m_fRotateY = vec.y * (RAD_TO_DEG * fAngle);
	PROFILE->m_fRotateZ = vec.z * (RAD_TO_DEG * fAngle);
	PROFILE->m_fTranslateX = m_CompBall.m_matTranslation._41;
	PROFILE->m_fTranslateY = m_CompBall.m_matTranslation._42;
	PROFILE->m_fTranslateZ = m_CompBall.m_matTranslation._43;

	Render3D();

	CResizablePage::OnMouseMove(nFlags, point);
}


void C3DViewPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CResizablePage::OnVScroll(nSBCode, nPos, pScrollBar);
}


void C3DViewPage::InitializeObjects()
{
	InitializeRoom();
	InitializeLookAtPoint();
	InitializeXYZ();

	switch(theApp.m_LastSettings.n3DMachineDrawingStyle= STANDARD_10_AXIS) {
	default: InitializeDualSidedSquirter();
		break;
	case TANK_ZERO_NEAR_SIDE:	InitializeTankOfWaterZeroNearSide();
		break;
	case TANK_ZERO_FAR_SIDE:	InitializeTankOfWaterZeroFarSide();
		break;
	}


//	InitPrimitivesForThicknessCScan();
	if(theApp.m_LastSettings.nDisplayTurnTableIn3D == 1) {
//		InitializePrimitiveForRotaryComponent();
		InitializeTurnTable();
	}

	InitializeComponent();
	/*
	if(m_b3DCScan) InitCScanOverlay();
	DefineMesh();
*/
}

void C3DViewPage::InitializeRoom()
{
	SAFE_DELETE( m_p3DObjectRoom );

	int nNumberParts = 5;
	D3DXCOLOR grey(0.5f,0.9f,1.0f,0.0f);
	float fRed = (float)GetRValue(theApp.m_LastSettings.rgbRoomWallColor) / 255.0f;
	float fGreen = (float)GetGValue(theApp.m_LastSettings.rgbRoomWallColor) / 255.0f;
	float fBlue = (float)GetBValue(theApp.m_LastSettings.rgbRoomWallColor) / 255.0f;
	D3DXCOLOR wall = D3DXCOLOR(fRed,fGreen,fBlue,0.0f);

	float fXLength =  theApp.m_Tank.fXLength + 6000.0f;
	float fYLength =  theApp.m_Tank.fYLength + 6000.0f;

	D3DXVECTOR3 vecFloorZero = D3DXVECTOR3(-theApp.m_Tank.fXSurfaceZero-3000.0f,-theApp.m_Tank.fYSurfaceZero-3000.0f,-theApp.m_Tank.fZSurfaceZero-50.0f);
//	D3DXVECTOR3 vecFloorZero = D3DXVECTOR3(-3000.0f,-3000.0f,-theApp.m_Tank.fZSurfaceZero-50.0f);
	D3DXVECTOR3 vecFloorSize = D3DXVECTOR3(fXLength,theApp.m_Tank.fYLength+6000.0f,0.0f);
	D3DXVECTOR3 vecBkWallSize = D3DXVECTOR3(fXLength,0.0f,4000.0f);
	D3DXVECTOR3 vecLeftWallSize = D3DXVECTOR3(0.0f,fYLength,4000.0f);
	D3DXVECTOR3 vecRightWallSize = D3DXVECTOR3(0.0f,-fYLength,4000.0f);

	m_p3DObjectRoom = new C3DObject(m_pd3dDevice,nNumberParts);

	m_p3DObjectRoom->Rectangle(vecFloorSize,grey,&vecFloorZero);
	m_p3DObjectRoom->Rectangle(vecBkWallSize,wall,&D3DXVECTOR3(-theApp.m_Tank.fXSurfaceZero-3000.0f,-theApp.m_Tank.fYSurfaceZero-3000.0f+fYLength,-theApp.m_Tank.fZSurfaceZero-50.0f),&D3DXVECTOR3(0.0f,0.0f,0.0f));
	m_p3DObjectRoom->Rectangle(vecLeftWallSize,wall,&D3DXVECTOR3(-theApp.m_Tank.fXSurfaceZero-3000.0f,-theApp.m_Tank.fYSurfaceZero-3000.0f,-theApp.m_Tank.fZSurfaceZero-50.0f),&D3DXVECTOR3(0.0f,0.0f,0.0f));
	m_p3DObjectRoom->Rectangle(vecRightWallSize,wall,&D3DXVECTOR3(-theApp.m_Tank.fXSurfaceZero-3000.0f+fXLength,-theApp.m_Tank.fYSurfaceZero-3000.0f+fYLength,-theApp.m_Tank.fZSurfaceZero-50.0f),&D3DXVECTOR3(0.0f,0.0f,0.0f));


}

void C3DViewPage::InitializeXYZ()
{
	SAFE_DELETE( m_p3DObjectXYZ );

	m_p3DObjectXYZ = new C3DObject(m_pd3dDevice,3);

	m_p3DObjectXYZ->Rod(10.0f,100.0f,D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXCOLOR(RGB(0,0,255)), &D3DXVECTOR3(0.0f, 0.0f, 0.0f));	//red
	m_p3DObjectXYZ->Rod(10.0f,100.0f,D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(RGB(0,255,0)), &D3DXVECTOR3(0.0f, 0.0f, 0.0f));	//green
	m_p3DObjectXYZ->Rod(10.0f,100.0f,D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXCOLOR(RGB(255,0,0)), &D3DXVECTOR3(0.0f, 0.0f, 0.0f));	//blue

	SAFE_RELEASE( m_pMesh3DText[0] );
	SAFE_RELEASE( m_pMesh3DText[1] );
	SAFE_RELEASE( m_pMesh3DText[2] );
	CreateD3DXTextMesh( &m_pMesh3DText[0], L"X", TEXT("Arial"), 12, FALSE, FALSE );
	CreateD3DXTextMesh( &m_pMesh3DText[1], L"Y", TEXT("Arial"), 12, FALSE, FALSE );
	CreateD3DXTextMesh( &m_pMesh3DText[2], L"Z", TEXT("Arial"), 12, FALSE, FALSE );

	D3DXComputeNormals(m_pMesh3DText[0],NULL); 
	D3DXComputeNormals(m_pMesh3DText[1],NULL); 
	D3DXComputeNormals(m_pMesh3DText[2],NULL); 

}

void C3DViewPage::InitializeDualSidedSquirter()
{
	SAFE_DELETE( m_p3DObjectScanner );

	D3DXVECTOR3	vecTranslation,vecRotate;
	int nNumberParts = (int)(theApp.m_Tank.fXLength / 100.0f);
	nNumberParts += (int)(theApp.m_Tank.fYLength / 100.0f);
	nNumberParts += 20;
	float fX,fY;
	D3DXCOLOR grey(200,200,200,0);
	D3DXCOLOR blue(0,0,255,0);

	D3DXVECTOR3 vecFloorZero = D3DXVECTOR3(-theApp.m_Tank.fXSurfaceZero,-theApp.m_Tank.fYSurfaceZero,-theApp.m_Tank.fZSurfaceZero);
	D3DXVECTOR3 vecFloorSize = D3DXVECTOR3(theApp.m_Tank.fXLength,theApp.m_Tank.fYLength,0.0f);
	D3DXVECTOR3 vecPillar = D3DXVECTOR3(100.0f,100.0f,theApp.m_Tank.fZLength);

	m_p3DObjectScanner = new C3DObject(m_pd3dDevice,nNumberParts);

	m_p3DObjectScanner->Rectangle(vecFloorSize,blue,&vecFloorZero);

	int nOldCullMode = m_p3DObjectScanner->SetCullMode(D3DCULL_NONE);
	for(fX=0.0f;fX<vecFloorSize.x;fX+=100.0f) {
		vecTranslation = vecFloorZero;
		vecTranslation.x += fX;
		m_p3DObjectScanner->Rectangle(D3DXVECTOR3(0.0f,vecFloorSize.y,20.0f),grey,&vecTranslation);
	}
	for(fY=0.0f;fY<vecFloorSize.y;fY+=100.0f) {
		vecTranslation = vecFloorZero;
		vecTranslation.y += fY;
		m_p3DObjectScanner->Rectangle(D3DXVECTOR3(vecFloorSize.x,0.0f,20.0f),grey,&vecTranslation);
	}
	
	m_p3DObjectScanner->SetCullMode(nOldCullMode);
	m_p3DObjectScanner->Cuboid(vecPillar,grey, &D3DXVECTOR3(vecFloorZero.x,vecFloorZero.y,vecFloorZero.z));
	m_p3DObjectScanner->Cuboid(vecPillar,grey, &D3DXVECTOR3(vecFloorZero.x+vecFloorSize.x-100.0f,vecFloorZero.y,vecFloorZero.z));
	m_p3DObjectScanner->Cuboid(vecPillar,grey, &D3DXVECTOR3(vecFloorZero.x+vecFloorSize.x-100.0f,vecFloorZero.y+vecFloorSize.y-100.0f,vecFloorZero.z));
	m_p3DObjectScanner->Cuboid(vecPillar,grey, &D3DXVECTOR3(vecFloorZero.x,vecFloorZero.y+vecFloorSize.y-100.0f,vecFloorZero.z));
}

void C3DViewPage::InitializeTankOfWaterZeroNearSide()
{
	SAFE_DELETE( m_p3DObjectScanner );

	int nNumberParts = 6;

	D3DXCOLOR grey(200,200,200,0);
	D3DXCOLOR blue(0,0,255,0);

	D3DXVECTOR3 vecCornerFarLeft = D3DXVECTOR3(theApp.m_Tank.fXSurfaceZero,-theApp.m_Tank.fYSurfaceZero,theApp.m_Tank.fZLength-theApp.m_Tank.fZSurfaceZero);
	D3DXVECTOR3 vecCornerNearLeft = D3DXVECTOR3(-theApp.m_Tank.fXSurfaceZero,-theApp.m_Tank.fYSurfaceZero,theApp.m_Tank.fZLength-theApp.m_Tank.fZSurfaceZero);
	D3DXVECTOR3 vecCornerFarRight = D3DXVECTOR3(theApp.m_Tank.fXSurfaceZero,theApp.m_Tank.fYLength-theApp.m_Tank.fYSurfaceZero,theApp.m_Tank.fZLength-theApp.m_Tank.fZSurfaceZero);
	D3DXVECTOR3 vecFloorSize = D3DXVECTOR3(-theApp.m_Tank.fXLength,theApp.m_Tank.fYLength,0.0f);
	D3DXVECTOR3 vecLeftWall = D3DXVECTOR3(theApp.m_Tank.fXLength,0.0f,-theApp.m_Tank.fZLength);
	D3DXVECTOR3 vecFarWall = D3DXVECTOR3(0.0f,theApp.m_Tank.fYLength,-theApp.m_Tank.fZLength);
	D3DXVECTOR3 vecRightWall = D3DXVECTOR3(-theApp.m_Tank.fXLength,0.0f,-theApp.m_Tank.fZLength);

	m_p3DObjectScanner = new C3DObject(m_pd3dDevice,nNumberParts);

	m_p3DObjectScanner->Rectangle(vecFloorSize,blue,&vecCornerFarLeft);
	m_p3DObjectScanner->Rectangle(vecLeftWall,grey,&vecCornerNearLeft);
	m_p3DObjectScanner->Rectangle(vecFarWall,grey,&vecCornerFarLeft);
	m_p3DObjectScanner->Rectangle(vecRightWall,grey,&vecCornerFarRight);

}

void C3DViewPage::InitializeTankOfWaterZeroFarSide()
{
	SAFE_DELETE( m_p3DObjectScanner );

	int nNumberParts = 6;

	D3DXCOLOR grey(200,200,200,0);
	D3DXCOLOR blue(0,0,255,0);

	D3DXVECTOR3 vecCornerFarLeft =			D3DXVECTOR3(-theApp.m_Tank.fXSurfaceZero,-theApp.m_Tank.fYSurfaceZero,-theApp.m_Tank.fZSurfaceZero);
	D3DXVECTOR3 vecFarWall =				D3DXVECTOR3(theApp.m_Tank.fXLength,0.0f,theApp.m_Tank.fZLength);
	D3DXVECTOR3 vecLeftWall =				D3DXVECTOR3(0.0f,theApp.m_Tank.fYLength,theApp.m_Tank.fZLength);
	D3DXVECTOR3 vecCornerFarLeftBottom =	D3DXVECTOR3(-theApp.m_Tank.fXSurfaceZero,-theApp.m_Tank.fYSurfaceZero,-theApp.m_Tank.fZSurfaceZero+theApp.m_Tank.fZLength);
	D3DXVECTOR3 vecFloorSize =				D3DXVECTOR3(theApp.m_Tank.fXLength,theApp.m_Tank.fYLength,0.0f);
	D3DXVECTOR3 vecCornerFarRight =			D3DXVECTOR3(theApp.m_Tank.fXLength-theApp.m_Tank.fXSurfaceZero,-theApp.m_Tank.fYSurfaceZero,-theApp.m_Tank.fZSurfaceZero);
	D3DXVECTOR3 vecRightWall =				D3DXVECTOR3(0.0f,theApp.m_Tank.fYLength,theApp.m_Tank.fZLength);

	m_p3DObjectScanner = new C3DObject(m_pd3dDevice,nNumberParts);

	m_p3DObjectScanner->SetCullMode(D3DCULL_CW);	m_p3DObjectScanner->Rectangle(vecFloorSize,blue,&vecCornerFarLeftBottom);
	m_p3DObjectScanner->SetCullMode(D3DCULL_CCW);	m_p3DObjectScanner->Rectangle(vecLeftWall,grey,&vecCornerFarLeft);
	m_p3DObjectScanner->SetCullMode(D3DCULL_CW);	m_p3DObjectScanner->Rectangle(vecFarWall,grey,&vecCornerFarLeft); 	
	m_p3DObjectScanner->SetCullMode(D3DCULL_CW);	m_p3DObjectScanner->Rectangle(vecRightWall,grey,&vecCornerFarRight);

}

void C3DViewPage::InitializeTurnTable()
{
	SAFE_DELETE( m_p3DObjectTurntable );

	int nNumberParts = 2;
	D3DXCOLOR grey(0.5f,0.5f,0.5f,0);

	D3DXVECTOR3 vecCentre = D3DXVECTOR3(theApp.m_Tank.fTTXOffset,theApp.m_Tank.fTTYOffset,theApp.m_Tank.fTTZOffset);
	D3DXVECTOR3 vecNormal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_p3DObjectTurntable = new C3DObject(m_pd3dDevice,nNumberParts);

	m_p3DObjectTurntable->Rod(theApp.m_Tank.fTTDiameter, 50.0f, vecNormal, grey, &vecCentre);

}

void C3DViewPage::InitializeRobot()
{
	SAFE_DELETE( m_p3DObjectRobot );

	return;

	int nNumberParts = 20;
	D3DXCOLOR yellow(0.3f,0.7f,0.0f,0);

	D3DXVECTOR3	vecBase = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3	vecBaseNormal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXVECTOR3	vecJ1 =			D3DXVECTOR3(0.0f, 0.0f, 550.0f);
	D3DXVECTOR3	vecJ1Normal =	D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vecJ1ArmSize =	D3DXVECTOR3(100.0f,100.0f,150.0f);

	D3DXVECTOR3	vecJ2 =			D3DXVECTOR3(0.0f, 150.0f, 550.0f);
	D3DXVECTOR3	vecJ2Normal =	D3DXVECTOR3(1.0f, 0.0f, 0.0f);



	m_p3DObjectRobot = new C3DObject(m_pd3dDevice,nNumberParts);

	//diameter, height 
	m_p3DObjectRobot->Rod(337.0f, 400.0f, vecBaseNormal, yellow, &vecBase);
	m_p3DObjectRobot->Cuboid(vecJ1ArmSize, yellow, &vecJ1, &vecJ1Normal);

}

void C3DViewPage::InitializeComponent()
{
	int nLine;

	if (GetSafeHwnd() == NULL) return;

	SAFE_DELETE( m_p3DObjectTaughtLines );
	SAFE_DELETE( m_p3DObjectScanLines );
	SAFE_DELETE( m_p3DObjectBraneLines );
	SAFE_DELETE( m_p3DObjectEnvelopeLines );
	SAFE_DELETE( m_p3DObjectRefPoints );
	SAFE_DELETE( m_p3DObjectOutsideLines );
	SAFE_DELETE( m_p3DObjectInsideLines );

	//Taught lines
	if(PROFILE->m_nTaughtLineL > 0) {
		m_p3DObjectTaughtLines = new C3DObject(m_pd3dDevice,PROFILE->m_nTaughtLineL);
		for(nLine=0;nLine<PROFILE->m_nTaughtLineL;nLine++) {
			m_p3DObjectTaughtLines->PolyLine(&PROFILE->m_TaughtLine[nLine],COLORREF(RGB(0,255,0)));
		}
	}

	//Scan Lines
	switch(PROFILE->m_nComponentDescription) {
	case RECTILINEAR:
		InitializeRectilinearScanLines();
		InitializeRectilinearDataVertices();
		break;
	case CLOSED_ROTARY:
	case CIRCULAR:
		if(PROFILE->m_nScanLineL == 1) InitializeRotoSymmetricScanLines();
		break;
	case OPEN_ROTARY:
		InitializeRectilinearScanLines();
		InitializeRectilinearDataVertices();
		break;
	}

	InitializeScratchLines();
	InitializeBraneLines();
	InitializeOutsideInsideLines();


	//Envelope lines
	if(PROFILE->m_nEnvelopeLineL > 0) {
		m_p3DObjectEnvelopeLines = new C3DObject(m_pd3dDevice,PROFILE->m_nEnvelopeLineL);
		for(nLine=0;nLine<PROFILE->m_nEnvelopeLineL;nLine++) {
			m_p3DObjectEnvelopeLines->SetZMode(false);
			m_p3DObjectEnvelopeLines->PolyLine(&PROFILE->m_EnvelopeLine[nLine],COLORREF(RGB(0,0,255)),1.0,true);
		}
	}

	//Ref points
	m_p3DObjectRefPoints = new C3DObject(m_pd3dDevice,3);
	m_p3DObjectRefPoints->Rod(10.0f,100.0f,(D3DXVECTOR3)PROFILE->m_CpCadHole[0].Side0.norm,D3DXCOLOR(RGB(0,0,255)),(D3DXVECTOR3*)&PROFILE->m_CpCadHole[0].Side0.pt);
	m_p3DObjectRefPoints->Rod(10.0f,100.0f,(D3DXVECTOR3)PROFILE->m_CpCadHole[1].Side0.norm,D3DXCOLOR(RGB(0,255,0)),(D3DXVECTOR3*)&PROFILE->m_CpCadHole[1].Side0.pt);
	m_p3DObjectRefPoints->Rod(10.0f,100.0f,(D3DXVECTOR3)PROFILE->m_CpCadHole[2].Side0.norm,D3DXCOLOR(RGB(255,0,0)),(D3DXVECTOR3*)&PROFILE->m_CpCadHole[2].Side0.pt);

	InitializeDangerZone();

}

void C3DViewPage::InitializeScratchLines()
{
	SAFE_DELETE( m_p3DObjectScratchLines );
	CPolyCoord Line;
	CPolyCoord CurvedLine;
	CCoord CpSurface;
	float fLength,fPos;

	if(PROFILE->m_nTaughtLineL < 2) return;
	m_p3DObjectScratchLines = new C3DObject(m_pd3dDevice,40);

	if(theApp.m_pScanData) {
//		m_p3DObjectScratchLines->PolyLine(&theApp.m_pScanData->m_LineSamplePos,COLORREF(RGB(0.0f,0.0f,1.0f)));
	}

	return;
	CCoord Cp[100];
	Line.Zero();
	for(int nn = 0;nn < PROFILE->m_nTaughtLineL; nn++) {
		PROFILE->m_TaughtLine[nn].GetCoord(0,&Cp[nn]);
		Line.Add(Cp[nn]);
	}
	m_p3DObjectScratchLines->PolyLine(&Line,COLORREF(RGB(1.0f,0,0)));

	Line.SetStyle(1);
	Line.SetPrimaryAxis(30);
	Line.PreProcessLine();
	fLength = Line.GetLength();

	CurvedLine.Zero();
	for(int ii=0;ii<100;ii++) {
		fPos = (float)ii * fLength / 99.0f;
		Line.CalculateCoord(fPos,CpSurface);
		CurvedLine.Add(CpSurface);
	}
//	m_p3DObjectScratchLines->PolyLine(&CurvedLine,COLORREF(RGB(1.0f,0,0)));


	CNURBS	Nurb;
	int nCoordL = PROFILE->m_nTaughtLineL + 1;
	int nCurvePts = 100;
	int nOrder = 3;
	D3DXVECTOR3 *vecCtrlPts = new D3DXVECTOR3[nCoordL];
	D3DXVECTOR3 vecCurvePts[300];
	D3DXVECTOR3 vecIntersection,vecZ0,vecZ1;
	D3DXPLANE plane[2];
	ZeroMemory(vecCtrlPts,(sizeof D3DXVECTOR3) * nCoordL);
	ZeroMemory(vecCurvePts,sizeof vecCurvePts);

	float *fKnots = new float[nCoordL + nOrder + 1];
	int mm,nn;

	vecCtrlPts[0] = Cp[0].Side0.pt;
	int jj=1;
	for(nn = 0, mm = 1;nn < (PROFILE->m_nTaughtLineL-1); nn++, mm++) {

		mm %= PROFILE->m_nTaughtLineL;

		D3DXPlaneFromPointNormal(&plane[0],&Cp[nn].Side0.pt,(D3DXVECTOR3*)&Cp[nn].Side0.norm);
		D3DXPlaneFromPointNormal(&plane[1],&Cp[mm].Side0.pt,(D3DXVECTOR3*)&Cp[mm].Side0.norm);
		D3DXVec3Cross(&vecIntersection, (D3DXVECTOR3*)&plane[1], (D3DXVECTOR3*)&plane[0]);

		//if Z == 0 then the two planes cross at
		vecZ0.z = 0.0f;
		SolveForZEqualZero(&vecZ0,&plane[0],&plane[1]);
		vecZ1.x = vecZ0.x + 1500.0f * vecIntersection.x;
		vecZ1.y = vecZ0.y + 1500.0f * vecIntersection.y;
		vecZ1.z = vecZ0.z + 1500.0f * vecIntersection.z;

		CurvedLine.Zero();
		CpSurface.Side0.pt = vecZ0;
		CurvedLine.Add(CpSurface);

		float fMag = 1.0f;

		CpSurface.Side0.pt.z = Cp[nn].Side0.pt.z;
		CpSurface.Side0.pt.x = ((vecZ1.x - vecZ0.x) * (Cp[nn].Side0.pt.z - vecZ0.z) / (vecZ1.z - vecZ0.z) * fMag) + vecZ0.x;
		CpSurface.Side0.pt.y = ((vecZ1.y - vecZ0.y) * (Cp[nn].Side0.pt.z - vecZ0.z) / (vecZ1.z - vecZ0.z) * fMag) + vecZ0.y;
		CurvedLine.Add(CpSurface);
		m_p3DObjectScratchLines->PolyLine(&CurvedLine,COLORREF(RGB(1.0f,0,0)));

		vecCtrlPts[jj++] = CpSurface.Side0.pt;
	}
	vecCtrlPts[jj++] = Cp[PROFILE->m_nTaughtLineL-1].Side0.pt;

	Nurb.knot(nCoordL,nOrder,fKnots);
	Nurb.bspline(nCoordL,nOrder,nCurvePts,vecCtrlPts,fKnots,vecCurvePts);

	CurvedLine.Zero();
	CpSurface.Zero();
	for(int ii=0;ii<nCurvePts;ii++) {
		CpSurface.Side0.pt = vecCurvePts[ii];
		CurvedLine.Add(CpSurface);
	}
//	m_p3DObjectScratchLines->PolyLine(&CurvedLine,COLORREF(RGB(1.0f,0,0)));

//	float fHomogeneous[6] = {1.0f, 10.0f, 6.0f, 10.0f, 4.0f, 1.0f};
	float fHomogeneous[6] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
	Nurb.rationalbspline(nCoordL,nOrder,nCurvePts,vecCtrlPts,fKnots,fHomogeneous,vecCurvePts);

	CurvedLine.Zero();
	CpSurface.Zero();
	for(int ii=0;ii<nCurvePts;ii++) {
		CpSurface.Side0.pt = vecCurvePts[ii];
		CurvedLine.Add(CpSurface);
	}
	m_p3DObjectScratchLines->PolyLine(&CurvedLine,COLORREF(RGB(1.0f,0,0)));



	delete fKnots;
	delete vecCtrlPts;
}

HRESULT C3DViewPage::CreateD3DXTextMesh( LPD3DXMESH* ppMesh, CString strText, TCHAR* pstrFont, DWORD dwSize, BOOL bBold, BOOL bItalic )
{
    HRESULT hr;
    LPD3DXMESH pMeshNew = NULL;
    HDC hdc = CreateCompatibleDC( NULL );
    INT nHeight = -MulDiv( dwSize, GetDeviceCaps(hdc, LOGPIXELSY), 72 );
    HFONT hFont;
    HFONT hFontOld;


    hFont = CreateFont(nHeight, 0, 0, 0, bBold ? FW_BOLD : FW_NORMAL, bItalic, FALSE, FALSE, DEFAULT_CHARSET, 
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, pstrFont);
    
    hFontOld = (HFONT)SelectObject(hdc, hFont); 

    hr = D3DXCreateText(m_pd3dDevice, hdc, strText, 
                        0.101f, 0.101f, &pMeshNew, NULL, NULL);

    SelectObject(hdc, hFontOld);
    DeleteObject( hFont );
    DeleteDC( hdc );

    if( SUCCEEDED( hr ) )
        *ppMesh = pMeshNew;

//	hr = D3DXComputeNormals( pMeshNew, NULL );

    return hr;
}


//uses Cramer's rule
void C3DViewPage::SolveForZEqualZero(D3DXVECTOR3* pvecOut, D3DXPLANE* plane0, D3DXPLANE* plane1)
{

	float fDeterminant;
	float a,b,c,d,e,f;
	a = plane0->a;
	b = plane0->b;
	c = plane1->a;
	d = plane1->b;
	e = plane0->d;
	f = plane1->d;

	fDeterminant = ( (a * d) - (c * b) ) * -1.0f;	//I've had to multiply by -1. I dont know why. I shouldn't have to.
	pvecOut->x = ((e * d) - (f * b)) / fDeterminant;
	pvecOut->y = ((a * f) -(c * e)) / fDeterminant;

}

void C3DViewPage::InitializeRectilinearScanLines()
{
	int nLine;
	SAFE_DELETE( m_p3DObjectScanLines );

	if(PROFILE->m_nScanLineL<=0) return;
	m_p3DObjectScanLines = new C3DObject(m_pd3dDevice,PROFILE->m_nScanLineL);
	if(theApp.m_LastSettings.n3DFillInScanLines ==0) {
		for(nLine=0;nLine<PROFILE->m_nScanLineL;nLine++) {
			m_p3DObjectScanLines->PolyLine(&PROFILE->m_ScanLine[nLine],COLORREF(RGB(0,255,0)));
		}
	} else {
		m_p3DObjectScanLines->SetCullMode(D3DCULL_NONE);
		for(nLine=0;nLine<(PROFILE->m_nScanLineL-1);nLine++) {
			m_p3DObjectScanLines->FillTwoPolyLines(&PROFILE->m_ScanLine[nLine],&PROFILE->m_ScanLine[nLine+1],D3DXCOLOR(RGB(0xa0,0xa0,0xa0)));
		}
	}

}

void C3DViewPage::InitializeBraneLines()
{
	int nLine;
	SAFE_DELETE( m_p3DObjectBraneLines );
	CPolyCoord Line;

	if(PROFILE->m_nBrainLineL<=0) return;
	m_p3DObjectBraneLines = new C3DObject(m_pd3dDevice,PROFILE->m_nBrainLineL);
	for(nLine=0;nLine<PROFILE->m_nBrainLineL;nLine++) {
		Line.Zero();
		Line = &PROFILE->m_BrainLine[nLine];
		Line.ShiftAllCoordsAlongVector(1.0f);
		m_p3DObjectBraneLines->PolyLine(&Line,COLORREF(RGB(255,0,0)));
	}

}

void C3DViewPage::InitializeOutsideInsideLines()
{
	int nLine;
	SAFE_DELETE( m_p3DObjectOutsideLines );
	SAFE_DELETE( m_p3DObjectInsideLines );

	if(PROFILE->m_nLineBladeL[OUTSIDE] > 0) {
		m_p3DObjectOutsideLines = new C3DObject(m_pd3dDevice,PROFILE->m_nLineBladeL[OUTSIDE]);
		for(nLine=0;nLine<PROFILE->m_nLineBladeL[OUTSIDE];nLine++) {
			m_p3DObjectOutsideLines->PolyLine(&PROFILE->m_LineBlade[OUTSIDE][nLine],COLORREF(RGB(0,255,0)));
		}
	}
	if(PROFILE->m_nLineBladeL[INSIDE] > 0) {
		m_p3DObjectInsideLines = new C3DObject(m_pd3dDevice,PROFILE->m_nLineBladeL[INSIDE]);
		for(nLine=0;nLine<PROFILE->m_nLineBladeL[INSIDE];nLine++) {
			m_p3DObjectInsideLines->PolyLine(&PROFILE->m_LineBlade[INSIDE][nLine],COLORREF(RGB(0,255,0)));
		}
	}

}

void C3DViewPage::InitializeRectilinearDataVertices()
{
	int nLine;
	D3DXCOLOR rgb;
	SAFE_DELETE( m_p3DObjectDataVertices );

	if(theApp.m_pViewData == NULL) return;
	if(theApp.m_pViewData->m_vLineSample == NULL) return;
//	if((PROFILE->m_nComponentDescription != RECTILINEAR) || (PROFILE->m_nComponentDescription != OPEN_ROTARY)) return;

    CONST FLOAT f = 1.0f / 255.0f;
    rgb.r = f * (FLOAT) (unsigned char) (theApp.m_LastSettings.rgbVerticesColor >>  0);
    rgb.g = f * (FLOAT) (unsigned char) (theApp.m_LastSettings.rgbVerticesColor >>  8);
    rgb.b = f * (FLOAT) (unsigned char) (theApp.m_LastSettings.rgbVerticesColor >> 16);
    rgb.a = f * (FLOAT) (unsigned char) (theApp.m_LastSettings.rgbVerticesColor >> 24);


	m_p3DObjectDataVertices = new C3DObject(m_pd3dDevice,theApp.m_pViewData->m_nNumberLines);
	if(theApp.m_LastSettings.n3DFillInScanLines ==0) {
		for(nLine=0;nLine<theApp.m_pViewData->m_nNumberLines;nLine++) {
			m_p3DObjectDataVertices->PolyLine(&theApp.m_pViewData->m_vLineSample[nLine],rgb);
		}
	} else {
		m_p3DObjectDataVertices->SetCullMode(D3DCULL_NONE);
		for(nLine=0;nLine<(theApp.m_pViewData->m_nNumberLines-1);nLine++) {
			m_p3DObjectDataVertices->FillTwoPolyLines(&theApp.m_pViewData->m_vLineSample[nLine],&theApp.m_pViewData->m_vLineSample[nLine+1],rgb);
		}
	}

}



void C3DViewPage::InitializeRotoSymmetricScanLines()
{
	CPolyCoord Line[2];
	CCoord Cp,Cp1;
	float fRadius,fAngle,fI,fJ, fX, fY;
	int nStart,nFinish;

	SAFE_DELETE( m_p3DObjectScanLines );

	if(PROFILE->m_nComponentDescription != CIRCULAR) return;
	if(PROFILE->m_ScanLine == NULL) return;
	if(PROFILE->m_nScanLineL != 1) return;

	int nSamplesLine = 30;
	int	nNumberLines = PROFILE->m_ScanLine[0].m_nCoordL;

	Line[0].AllocateCoords(nSamplesLine);
	Line[1].AllocateCoords(nSamplesLine);

	m_p3DObjectScanLines = new C3DObject(m_pd3dDevice,nNumberLines);
	m_p3DObjectScanLines->SetCullMode(D3DCULL_NONE);

	nStart = 0;
	nFinish = nSamplesLine;

	for(int nLine=0;nLine<nNumberLines;nLine++) {
		PROFILE->m_ScanLine[0].GetCoord(nLine,&Cp1);
		
		fX = Cp1.Side0.fX - theApp.m_Tank.vTTCentre.x;
		fY = Cp1.Side0.fY - theApp.m_Tank.vTTCentre.y;

		fRadius = sqrtf(fX * fX + fY * fY);

		int nn,ii;
		for(nn=nStart,ii=0;nn<nFinish; nn++) {
			fAngle = (2 * PIf * (float)nn) / (float)(nSamplesLine-2);

			fI = cosf(fAngle) * -1.0f;
			fJ = sinf(fAngle) * -1.0f;

			Cp.Side0.fX = fRadius * fI + theApp.m_Tank.vTTCentre.x;
			Cp.Side0.fY = fRadius * fJ + theApp.m_Tank.vTTCentre.y;
			Cp.Side0.fZ = Cp1.Side0.fZ;
			Cp.Side0.fI = fI;
			Cp.Side0.fJ = fJ;
			Cp.Side0.fK = Cp1.Side0.fK;
			Line[1].ModifyCoord(nn, Cp);
		}
		Line[1].m_nCoordL = nn;
		if(nLine>0) {
			if(theApp.m_LastSettings.n3DFillInScanLines ==0) {
				m_p3DObjectScanLines->FillTwoPolyLines(&Line[0], &Line[1], D3DXCOLOR(RGB(0,255,0)));
			} else {
				m_p3DObjectScanLines->SetCullMode(D3DCULL_NONE);
				m_p3DObjectScanLines->FillTwoPolyLines(&Line[0], &Line[1],D3DXCOLOR(RGB(0xa0,0xa0,0xa0)));
			}
		}
		Line[0] = &Line[1];
	}
}

void C3DViewPage::InitializeDangerZone()
{
	SAFE_DELETE( m_p3DObjectDangerPlanes );
	if((PROFILE->m_nScanLineL<=0) && (PROFILE->m_nTaughtLineL<=0) && (PROFILE->m_nLineBladeL[OUTSIDE]==0) && (PROFILE->m_nLineBladeL[INSIDE]==0) ) return;

	int nNumberParts = 2;
	D3DXCOLOR red(1.0f,0.0f,0.0f,0.2f);
	D3DXCOLOR green(0.0f,1.0f,0.0f,0.2f);
	D3DXVECTOR3 vecPortZero;
	D3DXVECTOR3 vecStarboardZero;
	D3DXVECTOR3 vecSize;
	float fWidth;


	CCoord CpMin = PROFILE->m_CpDanger[DANGER_MINS];
	CCoord CpMax = PROFILE->m_CpDanger[DANGER_MAXS];

	vecPortZero			= CpMin.Side0.pt;
	vecStarboardZero	= CpMin.Side1.pt;
	if(PROFILE->m_vecToDangerPlane[0].x != 0.0f) {
		vecStarboardZero.x	= CpMax.Side1.fX;
		vecSize				= D3DXVECTOR3(5.0f, fabs(CpMax.Side0.fY-CpMin.Side0.fY), fabs(CpMax.Side0.fZ-CpMin.Side0.fZ));
	}
	if(PROFILE->m_vecToDangerPlane[0].y != 0.0f) {
		vecStarboardZero.y	= CpMax.Side1.fY;
		vecSize				= D3DXVECTOR3(fabs(CpMax.Side0.fX-CpMin.Side0.fX), 5.0f, fabs(CpMax.Side0.fZ-CpMin.Side0.fZ));
	}
	if(PROFILE->m_vecToDangerPlane[0].z != 0.0f) {
		vecStarboardZero.z	= CpMax.Side1.fZ;
		vecSize				= D3DXVECTOR3(fabs(CpMax.Side0.fX-CpMin.Side0.fX), fabs(CpMax.Side0.fY-CpMin.Side0.fY), 5.0f);
	}

	m_p3DObjectDangerPlanes = new C3DObject(m_pd3dDevice,nNumberParts);
//	m_p3DObjectDangerPlanes->SetCullMode(0);

	switch(PROFILE->m_nComponentDescription) { 
	case RECTILINEAR:
		switch(theApp.m_Tank.nScannerDescription) {
		default:
			m_p3DObjectDangerPlanes->Cuboid(vecSize,red,&vecPortZero);
			m_p3DObjectDangerPlanes->Cuboid(vecSize,green,&vecStarboardZero);
			break;
		case SINGLE_BRIDGE_TT_ROLLERS:
		case SINGLE_BRIDGE_TANK_WITH_R:
		case SINGLE_BRIDGE_TANK:
		case DOUBLE_BRIDGE_TANK:
			m_p3DObjectDangerPlanes->Rectangle(vecSize,red,&vecPortZero);
			break;
		}

		break;
	case CLOSED_ROTARY:
	case OPEN_ROTARY:
	case CIRCULAR:
		vecStarboardZero.x = theApp.m_Tank.vTTCentre.x;
		vecStarboardZero.y = theApp.m_Tank.vTTCentre.y;
		vecStarboardZero.z = CpMin.Side1.fZ;
		PROFILE->m_fComponentDiameter[0] > PROFILE->m_fComponentDiameter[1] ? fWidth = PROFILE->m_fComponentDiameter[0] : fWidth = PROFILE->m_fComponentDiameter[1];

		if(theApp.m_Tank.vTTZeroDegrees.x) {
			vecSize	= D3DXVECTOR3(10.0f, fWidth, fabs(CpMax.Side0.fZ-CpMin.Side0.fZ));
		}
		if(theApp.m_Tank.vTTZeroDegrees.y) {
			vecSize	= D3DXVECTOR3(fWidth, 10.0f, fabs(CpMax.Side0.fZ-CpMin.Side0.fZ));
		}
		m_p3DObjectDangerPlanes->SetCullMode(D3DCULL_NONE);
		m_p3DObjectDangerPlanes->Rectangle(vecSize,red,&vecPortZero, NULL, true);
		m_p3DObjectDangerPlanes->SetCullMode(D3DCULL_CW);
		m_p3DObjectDangerPlanes->Cylinder(CpMin.fRadius[STARBOARD]*2.0f, fabs(CpMax.Side1.fZ-CpMin.Side1.fZ), D3DXVECTOR3(0.0f,0.0f,1.0f), green, &vecStarboardZero,true);
		break;
	}

}

void C3DViewPage::InitializeCScanOverlay()
{
	int nLine;

	if(theApp.m_p3DData == NULL) return;

	CWaitCursor Wait;

	if(theApp.m_LastSettings.nCScanOverlayResolution < 0) theApp.m_LastSettings.nCScanOverlayResolution = 0;
	if(theApp.m_LastSettings.nCScanOverlayResolution > 4) theApp.m_LastSettings.nCScanOverlayResolution = 4;
	int nRes = theApp.m_LastSettings.nCScanOverlayResolution + 1;
	
	theApp.m_p3DData->PutvLineSampleInCADCoordinates();

	SAFE_DELETE( m_p3DObjectCScanOverlay );
	m_p3DObjectCScanOverlay = new C3DObject(m_pd3dDevice,(theApp.m_p3DData->m_nNumberLines / nRes));
	m_p3DObjectCScanOverlay->SetCullMode(D3DCULL_NONE);
	for(nLine=0;nLine<(theApp.m_p3DData->m_nNumberLines-nRes);nLine+=nRes) {
		m_p3DObjectCScanOverlay->BuildCScanLine(nLine,theApp.m_p3DData,theApp.m_n3DImageNumber,theApp.m_n3DImageType);
	}

	theApp.m_p3DData->PutvLineSamplesBackInttoScanCoordinates();

	Wait.Restore();
}
#define	BLUE3D		0xff0000ff
#define	GREEN3D		0xff00ff00
#define	RED3D		0xffff0000
#define	YELLOW3D	0xffffff00

void C3DViewPage::InitializeLookAtPoint()
{
	SAFE_DELETE( m_p3DObjectLookAtPoint );
	CPolyCoord Line;
	CCoord Cp;

	m_p3DObjectLookAtPoint = new C3DObject(m_pd3dDevice,20);


	Line.Zero();	Cp.Zero();
	Cp.Side0.fX=-1000.0;	Line.Add(Cp);
	Cp.Side0.fX=+1000.0;	Line.Add(Cp);
	m_p3DObjectLookAtPoint->PolyLine(&Line,COLORREF(RGB(0,0,1.0f)));
	Line.Zero();	Cp.Zero();
	Cp.Side0.fY=-1000.0;	Line.Add(Cp);
	Cp.Side0.fY=+1000.0;	Line.Add(Cp);
	m_p3DObjectLookAtPoint->PolyLine(&Line,COLORREF(RGB(0,1.0f,0)));
	Line.Zero();	Cp.Zero();
	Cp.Side0.fZ=-1000.0;	Line.Add(Cp);
	Cp.Side0.fZ=+1000.0;	Line.Add(Cp);
	m_p3DObjectLookAtPoint->PolyLine(&Line,COLORREF(RGB(1.0f,0,0)));
	
}

void C3DViewPage::DrawLookAtPoint()
{
	D3DXMATRIXA16 matWorld;
	C3DPrimitive Prim;

	if(m_bRbDown || m_bLbDown || m_bMbDown) {

		m_pd3dDevice->SetTexture(0, NULL );
		D3DXMatrixIdentity(&matWorld);

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		Prim.Draw3DLine(m_pd3dDevice,-1000.0f,0.0f,0.0f,1000.0f,0.0f,0.0f,RED3D);
		Prim.Draw3DLine(m_pd3dDevice,0.0f,-1000.0f,0.0f,0.0f,1000.0f,0.0f,GREEN3D);
		Prim.Draw3DLine(m_pd3dDevice,0.0f,0.0f,-1000.0f,0.0f,0.0f,1000.0f,BLUE3D);
		Prim.Draw3DCircle(m_pd3dDevice,D3DXVECTOR3(0.0,0.0,0.0),D3DXVECTOR3(1.0,0.0,0.0),200.0,RED3D);
		Prim.Draw3DCircle(m_pd3dDevice,D3DXVECTOR3(0.0,0.0,0.0),D3DXVECTOR3(0.0,1.0,0.0),200.0,GREEN3D);
		Prim.Draw3DCircle(m_pd3dDevice,D3DXVECTOR3(0.0,0.0,0.0),D3DXVECTOR3(0.0,0.0,1.0),200.0,BLUE3D);

		if(m_bRbDown) {
			Prim.Draw3DCone(m_pd3dDevice,D3DXVECTOR3(1000.0,0.0,0.0),D3DXVECTOR3(1.0,0.0,0.0),20.0,100.0,RED3D);
			Prim.Draw3DCone(m_pd3dDevice,D3DXVECTOR3(0.0,1000.0,0.0),D3DXVECTOR3(0.0,1.0,0.0),20.0,-100.0,GREEN3D);
		}

		if(m_bMbDown) {
			Prim.Draw3DCone(m_pd3dDevice,D3DXVECTOR3(0.0,0.0,1000.0),D3DXVECTOR3(0.0,0.0,1.0),20.0,100.0,BLUE3D);
		}

	}
}


void C3DViewPage::OnButton3dViewPoint() 
{
	D3DXMATRIXA16 matWorld;

	if(m_n3DMoveEyeComponent == 1) {
		CWaitCursor Wait;

		D3DXMatrixIdentity( &matWorld);
		D3DXMatrixMultiply( &matWorld, &matWorld, m_CompBall.GetTranslationMatrix() );
		D3DXMatrixMultiply( &matWorld, &matWorld, m_CompBall.GetRotationMatrix() );

		PROFILE->ComponentMove(matWorld);

		m_CompBall.Init(TRUE);
		CleanUp3D();
		Start3D();

		Wait.Restore();
	}

	m_n3DMoveEyeComponent = 0;
	SetToolBarCheckedState();
	
}

void C3DViewPage::OnButton3dComponent() 
{
	m_n3DMoveEyeComponent = 1;
	SetToolBarCheckedState();
	
}

void C3DViewPage::OnButton3dDangerZone() 
{
	theApp.m_LastSettings.nDisplay3DDangerZone>0 ? theApp.m_LastSettings.nDisplay3DDangerZone=0 : theApp.m_LastSettings.nDisplay3DDangerZone=1;
	if(theApp.m_LastSettings.nDisplay3DDangerZone==1) {
		theApp.m_WS.CalculateDangerZone();
		InitializeDangerZone();
	}
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dMachine() 
{
	theApp.m_LastSettings.nDisplay3DMachine>0 ? theApp.m_LastSettings.nDisplay3DMachine=0 : theApp.m_LastSettings.nDisplay3DMachine=1;
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dRefPoints() 
{
	theApp.m_LastSettings.nDisplay3DRefPoints>0 ? theApp.m_LastSettings.nDisplay3DRefPoints=0 : theApp.m_LastSettings.nDisplay3DRefPoints=1;
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dScanlines() 
{
	theApp.m_LastSettings.nDisplay3DScanLines==FALSE ? theApp.m_LastSettings.nDisplay3DScanLines=TRUE : theApp.m_LastSettings.nDisplay3DScanLines=FALSE;
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dTaughtlines() 
{
	theApp.m_LastSettings.nDisplay3DTaughtLines==FALSE ? theApp.m_LastSettings.nDisplay3DTaughtLines=TRUE : theApp.m_LastSettings.nDisplay3DTaughtLines=FALSE;
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dEnvelope() 
{
	theApp.m_LastSettings.nDisplay3DEnvelope>0 ? theApp.m_LastSettings.nDisplay3DEnvelope=0 : theApp.m_LastSettings.nDisplay3DEnvelope=1;
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dBranelines() 
{
	theApp.m_LastSettings.nDisplay3DBraneLines==FALSE ? theApp.m_LastSettings.nDisplay3DBraneLines=TRUE : theApp.m_LastSettings.nDisplay3DBraneLines=FALSE;
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dRobot() 
{
	theApp.m_LastSettings.nDisplay3DRobot>0 ? theApp.m_LastSettings.nDisplay3DRobot=0 : theApp.m_LastSettings.nDisplay3DRobot=1;
	if(theApp.m_LastSettings.nDisplay3DRobot != 0) InitializeRobot();
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dTracker() 
{
	theApp.m_LastSettings.nDisplay3DTracker>0 ? theApp.m_LastSettings.nDisplay3DTracker=0 : theApp.m_LastSettings.nDisplay3DTracker=1;
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dCscan() 
{
	if(m_b3DCScan==FALSE) {
		if(theApp.m_LastSettings.nCScanOverlayMode == 0) {
			InitializeCScanOverlay();
		} else {
//			if( FAILED(BuildComponentMesh(NULL)) )	return ;
		}
		m_b3DCScan = TRUE;
	} else {
		m_b3DCScan = FALSE;
	}

	SetToolBarCheckedState();
	Render3D();
}

BOOL C3DViewPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
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
	case ID_BUTTON_3D_TAUGHTLINES: strTipText.LoadString(IDS_Taught_Lines);
		break;
	case ID_BUTTON_3D_SCANLINES: strTipText.LoadString(IDS_Scan_Lines);
		break;
	case ID_BUTTON_3D_ENVELOPE: strTipText.LoadString(IDS_Envelope);
		break;
	case ID_BUTTON_3D_REF_POINTS: strTipText.LoadString(IDS_Ref_Points);
		break;
	case ID_BUTTON_3D_ROBOT: strTipText.LoadString(IDS_Robot);
		break;
	case ID_BUTTON_3D_BALL_ROLL: strTipText.LoadString(IDS_Ball_Roll);
		break;
	case ID_BUTTON_3D_ROTATE_ABOUT_Z: strTipText.LoadString(IDS_Rotate_about_Z);
		break;
	case ID_BUTTON_3D_ROTATE_ABOUT_X: strTipText.LoadString(IDS_Rotate_about_X);
		break;
	case ID_BUTTON_3D_ROTATE_ABOUT_Y: strTipText.LoadString(IDS_Rotate_about_Y);
		break;
	case ID_BUTTON_DISPLAY_OUTSIDE: strTipText.LoadString(IDS_Outside_blade);
		break;
	case ID_BUTTON_DISPLAY_INSIDE: strTipText.LoadString(IDS_Inside_blade);
		break;
	}

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}

void C3DViewPage::SetToolBarCheckedState()
{
	int nIndex;

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_VIEW_POINT))>=0)
		m_n3DMoveEyeComponent==0 ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_COMPONENT))>=0)
		m_n3DMoveEyeComponent==1 ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_MACHINE))>=0)
		theApp.m_LastSettings.nDisplay3DMachine==1 ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_DANGER_ZONE))>=0)
		theApp.m_LastSettings.nDisplay3DDangerZone==1 ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_CSCAN))>=0)
		m_b3DCScan==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_TAUGHTLINES))>=0)
		theApp.m_LastSettings.nDisplay3DTaughtLines==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_SCANLINES))>=0)
		theApp.m_LastSettings.nDisplay3DScanLines==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_BRANELINES))>=0)
		theApp.m_LastSettings.nDisplay3DBraneLines==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_REF_POINTS))>=0)
		theApp.m_LastSettings.nDisplay3DRefPoints==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_ENVELOPE))>=0)
		theApp.m_LastSettings.nDisplay3DEnvelope==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

//	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_SURFACE))>=0)
//		m_b3DSurface==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_DATA_VERTICES))>=0)
		m_b3DDataVertices==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_ROBOT))>=0)
		theApp.m_LastSettings.nDisplay3DRobot==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_TRACKER))>=0)
		theApp.m_LastSettings.nDisplay3DTracker==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);
	
	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_DISPLAY_OUTSIDE))>=0)
		theApp.m_LastSettings.nDisplay3DOutsideLines==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_DISPLAY_INSIDE))>=0)
		theApp.m_LastSettings.nDisplay3DInsideLines==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_DISPLAY_WORKSHEET_PROFILES))>=0) {
		theApp.m_LastSettings.nDisplay3DWorksheet==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);
		if(theApp.m_WS.GetSize() <= 0 ) 
			m_wnd3DToolBar.GetToolBarCtrl().SetState(ID_BUTTON_DISPLAY_WORKSHEET_PROFILES,0);
	}


	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_BALL_ROLL))>=0)
		m_WorldBall.m_nDragAxes == 0 ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_ROTATE_ABOUT_Z))>=0)
		m_WorldBall.m_nDragAxes == 3 ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_ROTATE_ABOUT_Y))>=0)
		m_WorldBall.m_nDragAxes == 2 ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_ROTATE_ABOUT_X))>=0)
		m_WorldBall.m_nDragAxes == 1 ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	//	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_ISOMETRIC))>=0)
//		m_bDisplayIsometric==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

}


void C3DViewPage::OnButton3dTools() 
{

	CProfileSheet *pSheet = (CProfileSheet*)GetParent();
	pSheet->ToolsDrawingoptions();
}

void C3DViewPage::OnButtonSurface() 
{
	if(m_b3DSurface == false) {
		m_b3DSurface = true;

		InitializeSurface();

	} else {
		m_b3DSurface = false;
	}
	SetToolBarCheckedState();
	Render3D();
	
}

void C3DViewPage::InitializeSurface()
{
	SAFE_DELETE( m_p3DObjectSurfaceLines );
	CPolyCoord Line;

	if(PROFILE->m_SurfaceLine == NULL) return;
	if(PROFILE->m_nSurfaceLineL == 0) return;
	m_p3DObjectSurfaceLines = new C3DObject(m_pd3dDevice,PROFILE->m_nSurfaceLineL);

	for(int nLine=0;nLine<PROFILE->m_nSurfaceLineL;nLine++) {
		Line = &PROFILE->m_SurfaceLine[nLine];
		Line.StripOutZeroCoords();
		if(Line.m_nCoordL>0)	m_p3DObjectSurfaceLines->PolyLine(&Line,COLORREF(RGB(0.0f,0.0f,1.0f)));
	}

}

void C3DViewPage::OnButton3dDataVertices() 
{
	if(m_b3DDataVertices == false) {
		m_b3DDataVertices = true;
		InitializeRectilinearDataVertices();
	} else {
		m_b3DDataVertices = false;
	}
	SetToolBarCheckedState();
	Render3D();
	
}


void C3DViewPage::LookAtPoint(CCoord Cp,bool bRotate)
{

	m_WorldBall.SetTranslationMatrix(Cp.Side0.fX, Cp.Side0.fY, Cp.Side0.fZ);
	if(bRotate == true) 
		m_WorldBall.SetRotationMatrix(Cp.Side0.fI, Cp.Side0.fJ, Cp.Side0.fK);
	D3DXQuaternionRotationMatrix(&m_WorldBall.m_qNow,m_WorldBall.GetRotationMatrix());

}



void C3DViewPage::OnButton3dBallRoll() 
{
	m_WorldBall.m_nDragAxes = 0;
	m_CompBall.m_nDragAxes = 0;
	SetToolBarCheckedState();
}

void C3DViewPage::OnButton3dRotateAboutZ() 
{
	m_WorldBall.m_nDragAxes = 3;
	m_CompBall.m_nDragAxes = 3;
	SetToolBarCheckedState();
}

void C3DViewPage::OnButton3dRotateAboutY() 
{
	m_WorldBall.m_nDragAxes = 2;
	m_CompBall.m_nDragAxes = 2;
	SetToolBarCheckedState();
}

void C3DViewPage::OnButton3dRotateAboutX() 
{
	m_WorldBall.m_nDragAxes = 1;
	m_CompBall.m_nDragAxes = 1;
	SetToolBarCheckedState();
}

void C3DViewPage::DrawRobot()
{
	int nRobot;
	C3DPrimitive Prim;
	D3DXMATRIXA16 matCoord,matTranslation,matRotX,matRotY;

	D3DXVECTOR3 vecBase, vecFirstColumn;
	D3DXVECTOR3 vecBaseHArm, vecFirstHArm;
	D3DXVECTOR3 vecBaseVArm, vecFirstVArm;
	D3DXVECTOR3 vecBaseSecondHArm, vecSecondHArm;
	D3DXVECTOR3 vecBaseSecondVArm, vecSecondVArm;
	D3DXVECTOR3 vecBaseThirdHArm, vecThirdHArm;
	D3DXVECTOR3 vecBaseThirdVArm, vecThirdVArm;
	D3DXVECTOR3 vecBaseSearchTube, vecSearchTubeArm, vecWaterNozzle;


	if(theApp.m_LastSettings.nDisplay3DRobot == TRUE) {

		for(nRobot = 0;nRobot<1;nRobot++) {

			vecBase				=	D3DXVECTOR3(0.0f,0.0f,0.0f);
			vecFirstColumn		=	D3DXVECTOR3(0.0f,0.0f,300.0f);

			vecBaseHArm			=	D3DXVECTOR3(0.0f,-190.0f,250.0f);
			vecFirstHArm		=	D3DXVECTOR3(0.0f,190.0f,250.0f);

			vecBaseVArm			=	D3DXVECTOR3(0.0f,250.0f,-250.0f);
			vecFirstVArm		=	D3DXVECTOR3(0.0f,250.0f,1075.0f);

			vecBaseSecondHArm	=	D3DXVECTOR3(0.0f,-190.0f,0.0f);
			vecSecondHArm		=	D3DXVECTOR3(0.0f,190.0f,0.0f);

			vecBaseSecondVArm	=	D3DXVECTOR3(0.0f,0.0f,-153.0f);
			vecSecondVArm		=	D3DXVECTOR3(0.0f,0.0f,625.0f);

			vecBaseThirdHArm	=	D3DXVECTOR3(0.0f,-100.0f,625.0f);
			vecThirdHArm		=	D3DXVECTOR3(0.0f,100.0f,625.0f);

			vecBaseThirdVArm	=	D3DXVECTOR3(0.0f,0.0f,0.0f);
			vecThirdVArm		=	D3DXVECTOR3(0.0f,0.0f,110.0f);

			vecBaseSearchTube	=	D3DXVECTOR3(0.0f,0.0f,0.0f);
			vecSearchTubeArm	=	theApp.m_Robot[nRobot].m_vJ6ToolTip * 0.7f;
			vecWaterNozzle		=	theApp.m_Robot[nRobot].m_vJ6ToolTip;
			
			
			
			theApp.m_Robot[nRobot].InitializeJointMatrices();

			theApp.m_Robot[nRobot].TransformVectorFromBase(&vecBase);
			theApp.m_Robot[nRobot].TransformVectorFromBase(&vecFirstColumn);
			Prim.Draw3DTube(m_pd3dDevice,vecBase,vecFirstColumn,YELLOW3D,337/2.0f,337/2.0f);

			theApp.m_Robot[nRobot].TransformVectorFromJ1(&vecBaseHArm);
			theApp.m_Robot[nRobot].TransformVectorFromJ1(&vecFirstHArm);
			Prim.Draw3DRod(m_pd3dDevice,vecBaseHArm,vecFirstHArm,YELLOW3D,250.0f,250.0f);

			theApp.m_Robot[nRobot].TransformVectorFromJ2(&vecBaseVArm);
			theApp.m_Robot[nRobot].TransformVectorFromJ2(&vecFirstVArm);
			Prim.Draw3DTube(m_pd3dDevice,vecBaseVArm,vecFirstVArm,YELLOW3D,125.0f,125.0f);

			theApp.m_Robot[nRobot].TransformVectorFromJ3(&vecBaseSecondHArm);
			theApp.m_Robot[nRobot].TransformVectorFromJ3(&vecSecondHArm);
			Prim.Draw3DRod(m_pd3dDevice,vecBaseSecondHArm,vecSecondHArm,YELLOW3D,125.0f,125.0f);

			theApp.m_Robot[nRobot].TransformVectorFromJ3(&vecBaseSecondVArm);
			theApp.m_Robot[nRobot].TransformVectorFromJ3(&vecSecondVArm);
			Prim.Draw3DTube(m_pd3dDevice,vecBaseSecondVArm,vecSecondVArm,YELLOW3D,70.0f,70.0f);

			theApp.m_Robot[nRobot].TransformVectorFromJ4(&vecBaseThirdHArm);
			theApp.m_Robot[nRobot].TransformVectorFromJ4(&vecThirdHArm);
			Prim.Draw3DRod(m_pd3dDevice,vecBaseThirdHArm,vecThirdHArm,YELLOW3D,50.0f,50.0f);

			theApp.m_Robot[nRobot].TransformVectorFromJ5(&vecBaseThirdVArm);
			theApp.m_Robot[nRobot].TransformVectorFromJ5(&vecThirdVArm);
			Prim.Draw3DTube(m_pd3dDevice,vecBaseThirdVArm,vecThirdVArm,GREEN3D,30.0f,30.0f);


			theApp.m_Robot[nRobot].TransformVectorFromJ6(&vecBaseSearchTube);
			theApp.m_Robot[nRobot].TransformVectorFromJ6(&vecSearchTubeArm);
			theApp.m_Robot[nRobot].TransformVectorFromJ6(&vecWaterNozzle);
			Prim.Draw3DTube(m_pd3dDevice,vecBaseSearchTube,vecSearchTubeArm,RED3D,20.0f,20.0f);
			Prim.Draw3DTube(m_pd3dDevice,vecSearchTubeArm,vecWaterNozzle,BLUE3D,60.0f,10.0f);
		}
	}


}
void C3DViewPage::DrawScanViaDangerPlanePoints()
{
	CCoord CpSurface;
	D3DXMATRIXA16 matCoord;
	D3DXMATRIXA16 matTranslation;


	if(PROFILE->m_ScanLine == NULL) return;

	m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );

	for(int nLine=0;nLine < PROFILE->m_nScanLineL; nLine++) {

		for(int nEnd=0;nEnd<2;nEnd++) {
			if(nEnd == 0) {
				CpSurface = PROFILE->m_ScanLine[nLine].m_pCp[0];
			} else {
				CpSurface = PROFILE->m_ScanLine[nLine].m_pCp[ PROFILE->m_ScanLine[nLine].m_nCoordL-1 ];
			}

			if(CpSurface.nType == 1) {
				D3DXMatrixTranslation(&matTranslation,CpSurface.Side0.pt.x,CpSurface.Side0.pt.y,CpSurface.Side0.pt.z);
				D3DXMatrixMultiply(&matCoord,&matTranslation,&m_matWorld);
				m_pd3dDevice->SetTransform( D3DTS_WORLD, &matCoord );
				m_pSphere->DrawSubset(0);
			}
		}
	}

}


void C3DViewPage::DrawTrackerElements()
{
	C3DPrimitive Prim;
	D3DXMATRIXA16 matCoord;
	D3DXMATRIXA16 matTranslation;
	D3DXVECTOR3 vPt,vCenter,vNorm;
	D3DXVECTOR3 vNode[3];
	int nIndex,nElement,nColor,ii;
	float fRadius;

	if(theApp.m_LastSettings.nDisplay3DTracker == TRUE) {

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );
		

		for(ii=0;ii<PROFILE->m_Edge.GetSize();ii++) {
			PROFILE->m_Edge.GetPt(ii, 0, &vNode[0]);
			PROFILE->m_Edge.GetPt(ii, 1, &vNode[1]);
			theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vNode[0], &vNode[0]);
			theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vNode[1], &vNode[1]);
			Prim.Draw3DLine(m_pd3dDevice,vNode[0],vNode[1],BLUE3D);
		}


		for(nElement = 0;nElement < theApp.m_FaroWorkerThread.m_nElementL ; nElement++) {

			if(nElement == theApp.m_nElementIndex) {
				nColor = RED3D;
			} else {
				nColor = BLUE3D;
			}
			switch(theApp.m_FaroWorkerThread.m_Element[nElement].m_Type) {
			case VA_LINE:
			case VA_ANGLE:
				theApp.m_FaroWorkerThread.m_Element[nElement].GetPt(0,&vNode[0]);
				theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vNode[0], &vNode[0]);
				for(ii=1; ii<theApp.m_FaroWorkerThread.m_Element[nElement].GetSize(); ii++) {
					theApp.m_FaroWorkerThread.m_Element[nElement].GetPt(ii,&vNode[1]);
					theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vNode[1], &vNode[1]);
					Prim.Draw3DLine(m_pd3dDevice,vNode[0],vNode[1],nColor);
					vNode[0] = vNode[1];
				}
				break;
			case VA_ARC:
				theApp.m_FaroWorkerThread.m_Element[nElement].CalculateCircumCenter(&fRadius, &vCenter,NULL,NULL,&vNorm);
				theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vCenter, &vCenter);
				theApp.m_FaroWorkerThread.RotateToFrameNorm(&vNorm, &vNorm);
//				Prim.Draw3DCircle(m_pd3dDevice,vCenter,vNorm,fRadius,nColor);

				theApp.m_FaroWorkerThread.m_Element[nElement].GetPt(0,&vNode[0]);
				theApp.m_FaroWorkerThread.m_Element[nElement].GetPt(2,&vNode[1]);
				theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vNode[0], &vNode[0]);
				theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vNode[1], &vNode[1]);
				Prim.Draw3DFan(m_pd3dDevice,vCenter,vNode[0],vNode[1],nColor);
				break;
			}
		}

		for(nIndex = 0;nIndex<PROFILE->m_vCloud.GetSize();nIndex++) {
			PROFILE->m_vCloud.GetPt(nIndex, &vPt);
			theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vPt, &vPt);

			D3DXMatrixTranslation(&matTranslation,vPt.x,vPt.y,vPt.z);
			D3DXMatrixMultiply(&matCoord,&matTranslation,&m_matWorld);
			m_pd3dDevice->SetTransform( D3DTS_WORLD, &matCoord );
			m_pSphere->DrawSubset(0);

			if(theApp.m_nListOrderArray[nIndex]>0) {
				D3DXMatrixTranslation(&matTranslation,vPt.x,vPt.y,vPt.z+70.0f);
				D3DXMatrixMultiply(&matCoord,&matTranslation,&m_matWorld);
				m_pd3dDevice->SetTransform( D3DTS_WORLD, &matCoord );
				m_pCone->DrawSubset(0);
			}

		}

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );

	}

}




void C3DViewPage::InitializeEyeAndLookAtPt()
{
	m_vEyePt = theApp.m_LastSettings.vecEyePt;
	m_vLookatPt = theApp.m_LastSettings.vecLookAtPt;
	if(D3DXVec3Length(&(m_vEyePt-m_vLookatPt)) <= 1.0f) {
		MessageBox(L"Your eye pt and your look at pt are too close together",L"Warning",MB_ICONWARNING);
	}

}

void C3DViewPage::OnButtonDisplayInside() 
{
	theApp.m_LastSettings.nDisplay3DInsideLines>0 ? theApp.m_LastSettings.nDisplay3DInsideLines=0 : theApp.m_LastSettings.nDisplay3DInsideLines=1;
	if(theApp.m_LastSettings.nDisplay3DInsideLines != 0) InitializeOutsideInsideLines();
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButtonDisplayOutside() 
{
	theApp.m_LastSettings.nDisplay3DOutsideLines>0 ? theApp.m_LastSettings.nDisplay3DOutsideLines=0 : theApp.m_LastSettings.nDisplay3DOutsideLines=1;
	if(theApp.m_LastSettings.nDisplay3DOutsideLines != 0) InitializeOutsideInsideLines();
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButtonDisplayWorksheetProfiles() 
{
	theApp.m_LastSettings.nDisplay3DWorksheet>0 ? theApp.m_LastSettings.nDisplay3DWorksheet=0 : theApp.m_LastSettings.nDisplay3DWorksheet=1;
	SetToolBarCheckedState();
	Render3D();
}

=======
// 3DViewPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "3DViewPage.h"
#include "ProfileSheet.h"
#include "NURBS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DViewPage property page
bool m_bSizeChanged;
 
IMPLEMENT_DYNCREATE(C3DViewPage, CResizablePage)

C3DViewPage::C3DViewPage() : CResizablePage(C3DViewPage::IDD)
{
	//{{AFX_DATA_INIT(C3DViewPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_p3DObjectScanner = NULL;
	m_p3DObjectScanLines = NULL;
	m_p3DObjectBraneLines = NULL;
	m_p3DObjectTaughtLines = NULL;
	m_p3DObjectEnvelopeLines = NULL;
	m_p3DObjectRefPoints = NULL;
	m_p3DObjectCScanOverlay = NULL;
	m_p3DObjectRoom = NULL;
	m_p3DObjectDangerPlanes = NULL;
	m_p3DObjectTurntable = NULL;
	m_p3DObjectLookAtPoint = NULL;
	m_p3DObjectScratchLines = NULL;
	m_p3DObjectSurfaceLines = NULL;
	m_p3DObjectDataVertices = NULL;
	m_p3DObjectOutsideLines = NULL;
	m_p3DObjectInsideLines = NULL;
	m_p3DObjectRobot = NULL;
	m_p3DObjectXYZ = NULL;


	m_pSphere = NULL;
	m_pCone = NULL;
	m_pRobotElement0 = NULL;
	m_pRobotElement1 = NULL;
	m_pRobotElement2 = NULL;
	m_pRobotElement3 = NULL;
	m_pRobotElement4 = NULL;
	m_pRobotElement5 = NULL;

	m_pD3D       = NULL;
	m_pd3dDevice = NULL;

	m_p3DFont = NULL;
	m_p3DFontSmall = NULL;
	m_pComponentMesh = NULL;
	m_pWorldMesh = NULL;
	m_pTexFloor = NULL;
	m_pTexWall = NULL;
	m_pTexScope = NULL;
	m_pTexComponent = NULL;
	m_nTexComponentL = 0;
	m_b3DCScan = FALSE;
	m_b3DSurface = FALSE;
	m_b3DDataVertices = FALSE;
	ZeroMemory(	m_pMesh3DText, sizeof m_pMesh3DText);


	m_bSizeChanged = false;

	ZeroMemory(&m_PrimFrame,sizeof m_PrimFrame);
//	ZeroMemory(&m_PrimComp,sizeof m_PrimComp);


	InitializeEyeAndLookAtPt();

	switch(theApp.m_LastSettings.n3DUpVector) {
	default: m_vUpVec = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
		break;
	case 1: m_vUpVec = D3DXVECTOR3( 0.0f, 0.0f, -1.0f );
		break;
	}

	m_bLbDown = false;
	m_bRbDown = false;
	m_bMbDown = false;
	m_n3DMoveEyeComponent = 0;


}

C3DViewPage::~C3DViewPage()
{
	CleanUp3D();

}

void C3DViewPage::OnKickIdle()
{
	UpdateDialogControls(this, FALSE);
}


void C3DViewPage::DoDataExchange(CDataExchange* pDX)
{
	CResizablePage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3DViewPage)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3DViewPage, CResizablePage)
	//{{AFX_MSG_MAP(C3DViewPage)
	ON_MESSAGE_VOID(WM_KICKIDLE, OnKickIdle)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_VSCROLL()
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_3D_VIEW_POINT, OnButton3dViewPoint)
	ON_COMMAND(ID_BUTTON_3D_COMPONENT, OnButton3dComponent)
	ON_COMMAND(ID_BUTTON_3D_DANGER_ZONE, OnButton3dDangerZone)
	ON_COMMAND(ID_BUTTON_3D_MACHINE, OnButton3dMachine)
	ON_COMMAND(ID_BUTTON_3D_REF_POINTS, OnButton3dRefPoints)
	ON_COMMAND(ID_BUTTON_3D_SCANLINES, OnButton3dScanlines)
	ON_COMMAND(ID_BUTTON_3D_ENVELOPE, OnButton3dEnvelope)
	ON_COMMAND(ID_BUTTON_3D_CSCAN, OnButton3dCscan)
	ON_COMMAND(ID_BUTTON_3D_TOOLS, OnButton3dTools)
	ON_COMMAND(ID_BUTTON_3D_TAUGHTLINES, OnButton3dTaughtlines)
	ON_COMMAND(ID_BUTTON_3D_DATA_VERTICES, OnButton3dDataVertices)
	ON_COMMAND(ID_BUTTON_3D_ROBOT, OnButton3dRobot)
	ON_COMMAND(ID_BUTTON_3D_BALL_ROLL, OnButton3dBallRoll)
	ON_COMMAND(ID_BUTTON_3D_ROTATE_ABOUT_Z, OnButton3dRotateAboutZ)
	ON_COMMAND(ID_BUTTON_3D_ROTATE_ABOUT_X, OnButton3dRotateAboutX)
	ON_COMMAND(ID_BUTTON_3D_TRACKER, OnButton3dTracker)
	ON_COMMAND(ID_BUTTON_3D_ROTATE_ABOUT_Y, OnButton3dRotateAboutY)
	ON_COMMAND(ID_BUTTON_3D_BRANELINES, OnButton3dBranelines)
	ON_COMMAND(ID_BUTTON_DISPLAY_INSIDE, OnButtonDisplayInside)
	ON_COMMAND(ID_BUTTON_DISPLAY_OUTSIDE, OnButtonDisplayOutside)
	ON_COMMAND(ID_BUTTON_DISPLAY_WORKSHEET_PROFILES, OnButtonDisplayWorksheetProfiles)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DViewPage message handlers
int C3DViewPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CResizablePage::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wnd3DToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wnd3DToolBar.LoadToolBar(IDR_PROFILE_3D_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}


	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,rect.Width()+270,30,SWP_SHOWWINDOW);


	m_wndRebar.AddBar(&m_wnd3DToolBar);
	
	return 0;
}

BOOL C3DViewPage::OnInitDialog() 
{
	CResizablePage::OnInitDialog();

	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);
	m_StaticPosView.SetNewPos();
	
	
	Start3D();
	SetToolBarCheckedState();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3DViewPage::OnSize(UINT nType, int cx, int cy) 
{
	CResizablePage::OnSize(nType, cx, cy);
	
	if(GetSafeHwnd()) {
		m_StaticPosView.SetNewPos();
		m_bSizeChanged = true;
	}
}

BOOL C3DViewPage::OnSetActive() 
{
	CRect rr;
	m_staticView.GetWindowRect(rr);

	if(m_bSizeChanged == true) {
		if((rr.Width() > (int)m_d3dpp.BackBufferWidth) || (rr.Height() > (int)m_d3dpp.BackBufferHeight)) {
			CleanUp3D();
			Start3D();
		}
		m_bSizeChanged = false;
	}
	InitializeComponent();
	if((theApp.m_LastSettings.nDisplayTurnTableIn3D != 0) && (m_p3DObjectTurntable == NULL)) InitializeTurnTable();
	if((theApp.m_LastSettings.nDisplay3DRobot != 0) && (m_p3DObjectRobot == NULL)) InitializeRobot();

	return CResizablePage::OnSetActive();
}

inline int C3DViewPage::MinMax(int *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}


void C3DViewPage::Start3D()
{
	HWND hWnd = m_staticView.m_hWnd;
    if( SUCCEEDED( InitD3D( hWnd ) ) )  {
       InitializeObjects();
	}

}

void C3DViewPage::CleanUp3D()
{
	int	ii;

	if( m_pTexComponent ) {
		for(ii = 0; ii < m_nTexComponentL; ii++ ) {
			SAFE_RELEASE(m_pTexComponent[ii]);
        }
		SAFE_DELETE_ARRAY(m_pTexComponent);
		m_nTexComponentL=0;
    }
	SAFE_RELEASE(m_pTexScope);
	SAFE_RELEASE(m_pTexWall);
	SAFE_RELEASE(m_pTexFloor);
	SAFE_RELEASE(m_pComponentMesh);
	SAFE_RELEASE(m_pWorldMesh);
	SAFE_RELEASE(m_pd3dDevice);
	SAFE_RELEASE(m_pD3D);
	for(ii=0;ii<MAX_TEXT_STRINGS;ii++) {
		SAFE_RELEASE( m_pMesh3DText[ii] );
	}

    if(m_p3DFont) {
		m_p3DFont->InvalidateDeviceObjects();
	    m_p3DFont->DeleteDeviceObjects();
	}
    if(m_p3DFontSmall) {
	    m_p3DFontSmall->InvalidateDeviceObjects();
	    m_p3DFontSmall->DeleteDeviceObjects();
	}
    SAFE_DELETE( m_p3DFont );
    SAFE_DELETE( m_p3DFontSmall );

	SAFE_DELETE( m_p3DObjectScanner );
	SAFE_DELETE( m_p3DObjectScanLines );
	SAFE_DELETE( m_p3DObjectBraneLines );
	SAFE_DELETE( m_p3DObjectTaughtLines );
	SAFE_DELETE( m_p3DObjectEnvelopeLines );
	SAFE_DELETE( m_p3DObjectRefPoints );
	SAFE_DELETE( m_p3DObjectCScanOverlay );
	SAFE_DELETE( m_p3DObjectRoom );
	SAFE_DELETE( m_p3DObjectDangerPlanes );
	SAFE_DELETE( m_p3DObjectTurntable );
	SAFE_DELETE( m_p3DObjectLookAtPoint );
	SAFE_DELETE( m_p3DObjectScratchLines );
	SAFE_DELETE( m_p3DObjectSurfaceLines );
	SAFE_DELETE( m_p3DObjectDataVertices );
	SAFE_DELETE( m_p3DObjectRobot );
	SAFE_DELETE( m_p3DObjectXYZ );
	SAFE_DELETE( m_p3DObjectOutsideLines );
	SAFE_DELETE( m_p3DObjectInsideLines );
	SAFE_RELEASE( m_pSphere );
	SAFE_RELEASE( m_pCone );
	SAFE_RELEASE( m_pRobotElement0 );
	SAFE_RELEASE( m_pRobotElement1 );
	SAFE_RELEASE( m_pRobotElement2 );
	SAFE_RELEASE( m_pRobotElement3 );
	SAFE_RELEASE( m_pRobotElement4 );
	SAFE_RELEASE( m_pRobotElement5 );


}


HRESULT C3DViewPage::InitD3D( HWND hWnd )
{
    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice
    ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
    m_d3dpp.Windowed = TRUE;
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    m_d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &m_d3dpp, &m_pd3dDevice ) ) )
    {
        return E_FAIL;
    }


    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

    m_WorldBall.SetWindow( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 1.0f );
    m_WorldBall.SetRadius( 1000.0f );
    m_CompBall.SetWindow( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 1.0f );
    m_CompBall.SetRadius( 1000.0f );

	D3DXCreateSphere(m_pd3dDevice, 15.0f, 60, 60, &m_pSphere, NULL);
	D3DXCreateCylinder(m_pd3dDevice, 0.0f, 20.0f, 80.0f,60, 10, &m_pCone, NULL);
	D3DXCreateCylinder(m_pd3dDevice, 337/2.0f, 337/2.0f, 300.0f,60, 30, &m_pRobotElement0, NULL);
	D3DXCreateCylinder(m_pd3dDevice, 250.0f, 250.0f, 380.0f,300, 60, &m_pRobotElement1, NULL);

//    hr = D3DXCreateTextureFromFile( m_pd3dDevice, theApp.m_WorkingDirectory + _T("\\Calibration Nominal Gain.bmp"), &m_pTexScope );

//    hr = D3DXCreateTextureFromFile( m_pd3dDevice, theApp.m_WallImagePathName, &m_pTexWall );
/*
    if( FAILED(hr) ) {
		m_bWallPicture = FALSE;
	} else {
		m_bWallPicture = TRUE;
	}
*/
//    hr = D3DXCreateTextureFromFile( m_pd3dDevice, theApp.m_WorkingDirectory + _T("\\ground2.bmp"), &m_pTexFloor );

//    hr = BuildWorldMesh();


//   m_p3DFont            = new CD3DFont( _T("Arial"), 12, D3DFONT_NORMAL );
//    m_p3DFontSmall       = new CD3DFont( _T("Arial"),  9, D3DFONT_NORMAL );
//    m_p3DFont->InitDeviceObjects( m_pd3dDevice );
//    m_p3DFontSmall->InitDeviceObjects( m_pd3dDevice );
//    m_p3DFont->RestoreDeviceObjects();
 //   m_p3DFontSmall->RestoreDeviceObjects();


    return S_OK;
}




void C3DViewPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	Render3D();

}

D3DXMATRIXA16    m_matText;

VOID C3DViewPage::Render3D()
{
	CString Buff;

	if(m_pd3dDevice==NULL) return;

	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(200,200,200), 1.0f, 0 );
//    m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255,255,255), 1.0f, 0 );


    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )  {
		if(theApp.m_LastSettings.bWireframe )
			m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		else
			m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

		m_pd3dDevice->SetTexture(0, NULL );

		m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

		DrawLookAtPoint();
//		if(m_bRbDown || m_bLbDown || m_bMbDown) {
//			if(m_p3DObjectLookAtPoint)		m_p3DObjectLookAtPoint->Render();
//		}

		FrameMove();
		SetupLights();

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );
		if(theApp.m_LastSettings.nDisplay3DMachine) {

//			if(m_p3DObjectRoom)			m_p3DObjectRoom->Render();
			if(m_p3DObjectScanner)		m_p3DObjectScanner->Render();
			if(m_p3DObjectTurntable &&	theApp.m_LastSettings.nDisplayTurnTableIn3D)
										m_p3DObjectTurntable->Render();
		}
		
		RenderXYZ();
		DrawRobot();
		DrawTrackerElements();

		ComponentMove();
		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );

		if(m_p3DObjectTaughtLines	&& theApp.m_LastSettings.nDisplay3DTaughtLines)		m_p3DObjectTaughtLines->Render();
		if(m_p3DObjectScanLines		&& theApp.m_LastSettings.nDisplay3DScanLines)	{
			m_p3DObjectScanLines->Render();
			DrawScanViaDangerPlanePoints();
		}
		if(m_p3DObjectBraneLines	&& theApp.m_LastSettings.nDisplay3DBraneLines)		m_p3DObjectBraneLines->Render();
		if(m_p3DObjectOutsideLines	&& theApp.m_LastSettings.nDisplay3DOutsideLines)		m_p3DObjectOutsideLines->Render();
		if(m_p3DObjectInsideLines	&& theApp.m_LastSettings.nDisplay3DInsideLines)		m_p3DObjectInsideLines->Render();

		if(m_p3DObjectSurfaceLines	&& m_b3DSurface)									m_p3DObjectSurfaceLines->Render();
		if(m_p3DObjectDataVertices && m_b3DDataVertices)								m_p3DObjectDataVertices->Render();

		if(m_b3DCScan) {
			switch(theApp.m_LastSettings.nCScanOverlayMode) {
			case 0: if(m_p3DObjectCScanOverlay != NULL) m_p3DObjectCScanOverlay->Render();
				break;
			case 1:// RenderCscanOverLay();
				break;
			}
		}

		if(m_p3DObjectEnvelopeLines	&& theApp.m_LastSettings.nDisplay3DEnvelope)			m_p3DObjectEnvelopeLines->Render();
		if(m_p3DObjectRefPoints		&& theApp.m_LastSettings.nDisplay3DRefPoints)			m_p3DObjectRefPoints->Render();
//		if(m_p3DObjectRobot			&& theApp.m_LastSettings.nDisplay3DRobot)				m_p3DObjectRobot->Render();
		if(m_p3DObjectScratchLines)														m_p3DObjectScratchLines->Render();

//		if(m_bDisplayIsometric)							RenderPrimitives(m_pPrim[PRIM_ISOMETRIC],m_nNextPrim[PRIM_ISOMETRIC]);


		if((m_p3DObjectDangerPlanes != NULL) && (theApp.m_LastSettings.nDisplay3DDangerZone>0)) {
			m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
			m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
			m_p3DObjectDangerPlanes->Render();
		}




		D3DXVECTOR3 vecLength;
		D3DXVec3Subtract(&vecLength,&m_vEyePt,&m_vLookatPt);
		Buff.Format(_T("Distance to look at point %.01f mm"),D3DXVec3Length(&vecLength));
//		m_p3DFontSmall->DrawText( 2,  0, D3DCOLOR_ARGB(255,0,0,0),Buff );

        m_pd3dDevice->EndScene();
    }

    m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}


void C3DViewPage::RenderXYZ()
{
    D3DMATERIAL9 mtrl;
	D3DXMATRIX mScale,matTrans,matRotate,matText;
	float fScale = 50.0f;
	DWORD out[6];

	m_p3DObjectXYZ->Render();

	return;
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLOROP, &out[0]);
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAOP, &out[1]);
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &out[2]);
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG2, &out[3]);
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAARG1, &out[4]);
	m_pd3dDevice->GetTextureStageState(0, D3DTSS_CONSTANT, &out[5]);

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	//D3DTOP_SELECTARG1
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CONSTANT);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CONSTANT);

	m_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, false );
	m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );

	ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
	mtrl.Ambient.r = 1.0f;
	mtrl.Ambient.g = 0.0f;
	mtrl.Ambient.b = 0.0f;
	mtrl.Ambient.a = 1.0f;
	mtrl.Diffuse.r = 0.0f;
	mtrl.Diffuse.g = 1.0f;
	mtrl.Diffuse.b = 0.0f;
	mtrl.Diffuse.a = 0.5f;

	const D3DXCOLOR RED(D3DCOLOR_XRGB(255, 0, 0));

	mtrl.Ambient  = RED;
	mtrl.Diffuse  = RED;
	mtrl.Specular = RED;
	mtrl.Emissive = RED;
	mtrl.Power    = 0.5f;

	m_pd3dDevice->SetMaterial( &mtrl );
	D3DXMatrixScaling( &mScale, fScale,fScale,fScale);

	if( m_pMesh3DText[0] != NULL ) {	//X
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_CONSTANT, D3DCOLOR_XRGB(255,0,0));

		D3DXMatrixTranslation( &matTrans,120.0f,0.0f,-10.0f );
		D3DXMatrixRotationYawPitchRoll( &matRotate,0.0f,PIf/2.0f, 0.0f );

		matText = matRotate * mScale;
		matText *= matTrans;
		matText *= m_matWorld;

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matText );
		m_pMesh3DText[0]->DrawSubset(0);
	}

	if( m_pMesh3DText[1] != NULL ) {	//Y
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_CONSTANT, D3DCOLOR_XRGB(0,255,0));

		D3DXMatrixTranslation( &matTrans,0.0f,120.0f,-10.0f );
		D3DXMatrixRotationYawPitchRoll( &matRotate,PIf/2.0f,0.0f, PIf/2.0f );

		matText = matRotate * mScale;
		matText *= matTrans;
		matText *= m_matWorld;

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matText );
		m_pMesh3DText[1]->DrawSubset(0);
	}

	if( m_pMesh3DText[2] != NULL ) {	//Z
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_CONSTANT, D3DCOLOR_XRGB(0,0,255));

		D3DXMatrixTranslation( &matTrans,-10.0f,0.0f,120.0f );
		D3DXMatrixRotationYawPitchRoll( &matRotate,0.0f,PIf/2.0f,0.0f );

		matText = matRotate * mScale;
		matText *= matTrans;
		matText *= m_matWorld;

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matText );
		m_pMesh3DText[2]->DrawSubset(0);
	}

	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, out[0]);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, out[1]);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, out[2]);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, out[3]);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, out[4]);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_CONSTANT, out[5]);

	m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, true );

}

void C3DViewPage::FrameMove()
{

	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

    D3DXMATRIXA16 matTrans;
    D3DXMATRIXA16 matRotate;
	D3DXMATRIXA16 matView;
    D3DXMATRIXA16 matProj;
    D3DXMATRIXA16 matEyePt;
	D3DXVECTOR4 vEyePt,vEyePtRot;
	D3DXVECTOR3 vLookatPt;
	D3DXVECTOR3 vTrans;


    D3DXMatrixIdentity( &m_matWorld);
    D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_WorldBall.GetTranslationMatrix() );
    D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_WorldBall.GetRotationMatrix() );


	D3DXMatrixRotationYawPitchRoll(&matEyePt,m_PrimFrame.Rotate.x,m_PrimFrame.Rotate.y, m_PrimFrame.Rotate.z );
	D3DXVec3Transform(&vEyePtRot,&m_vEyePt,&matEyePt);

	vLookatPt = m_vLookatPt + m_PrimFrame.Translate;
	vEyePtRot.x += m_PrimFrame.Translate.x;
	vEyePtRot.y += m_PrimFrame.Translate.y;
	vEyePtRot.z += m_PrimFrame.Translate.z;
	D3DXMatrixLookAtRH( &matView, (D3DXVECTOR3*)&vEyePtRot, &vLookatPt, &m_vUpVec );
	m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );


    FLOAT m_fObjectRadius = 1.0f;
	FLOAT fAspect = (FLOAT)rr.Width() / (FLOAT)rr.Height();
	D3DXMatrixPerspectiveFovRH( &matProj, D3DX_PI/4, fAspect,1.0f, 20000.0f );
	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

}

void C3DViewPage::ComponentMove()
{

    D3DXMatrixIdentity( &m_matWorld);
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_CompBall.GetTranslationMatrix() );
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_CompBall.GetRotationMatrix() );
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_WorldBall.GetTranslationMatrix() );
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_WorldBall.GetRotationMatrix() );

}

void C3DViewPage::SetupLights()
{
    D3DXVECTOR3 vecDir;
	int nLight;
   
	D3DMATERIAL9 mtrl;
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
    m_pd3dDevice->SetMaterial( &mtrl );

	for(nLight = 0;nLight<8;nLight++) {
		if(theApp.m_LastSettings.light[nLight].Type) {
			theApp.m_LastSettings.light[nLight].Diffuse.r = 0.6f;
			theApp.m_LastSettings.light[nLight].Diffuse.g = 0.6f;
			theApp.m_LastSettings.light[nLight].Diffuse.b = 0.6f;

			theApp.m_LastSettings.light[nLight].Specular.r = 0.0f;
			theApp.m_LastSettings.light[nLight].Specular.g = 0.0f;
			theApp.m_LastSettings.light[nLight].Specular.b = 0.0f;

			theApp.m_LastSettings.light[nLight].Ambient.r = 0.0f;
			theApp.m_LastSettings.light[nLight].Ambient.g = 0.0f;
			theApp.m_LastSettings.light[nLight].Ambient.b = 0.0f;

			vecDir.x = theApp.m_LastSettings.vecLightLookAtPt[nLight].x - theApp.m_LastSettings.light[nLight].Position.x;
			vecDir.y = theApp.m_LastSettings.vecLightLookAtPt[nLight].y - theApp.m_LastSettings.light[nLight].Position.y;
			vecDir.z = theApp.m_LastSettings.vecLightLookAtPt[nLight].z - theApp.m_LastSettings.light[nLight].Position.z;

			D3DXVec3Normalize( (D3DXVECTOR3*)&theApp.m_LastSettings.light[nLight].Direction, &vecDir );

			theApp.m_LastSettings.light[nLight].Range = 6000.0f;
			theApp.m_LastSettings.light[nLight].Falloff = 1.0f;          /* Falloff */
			theApp.m_LastSettings.light[nLight].Attenuation0 = 1.0f;     /* Constant attenuation */
			theApp.m_LastSettings.light[nLight].Attenuation1 = 0.0f;     /* Linear attenuation */
			theApp.m_LastSettings.light[nLight].Attenuation2 = 0.0f;     /* Quadratic attenuation */

			m_pd3dDevice->SetLight( nLight, &theApp.m_LastSettings.light[nLight] );
			m_pd3dDevice->LightEnable( nLight, TRUE );
		} else {
			m_pd3dDevice->LightEnable( nLight, FALSE );
		}
	}

	int nAmbient = theApp.m_LastSettings.nLightAmbient;
	int nTemp = (nAmbient << 16) | (nAmbient << 8) | nAmbient;
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    m_pd3dDevice->SetRenderState( D3DRS_AMBIENT, nTemp) ;

}



void C3DViewPage::OnLButtonDown(UINT nFlags, CPoint point) 
{

	CRect rr;
	m_staticView.GetWindowRect(rr);

    m_WorldBall.SetWindow( rr.Width(), rr.Height(), 1.0f );
    m_WorldBall.SetRadius( 1000.0f );
    m_CompBall.SetWindow( rr.Width(), rr.Height(), 1.0f );
    m_CompBall.SetRadius( 1000.0f );

	if(m_n3DMoveEyeComponent==0) {
		m_WorldBall.m_vDown = m_WorldBall.ScreenToVector( point.x, point.y );
        m_WorldBall.m_qDown = m_WorldBall.m_qNow;
		m_WorldBall.m_bDrag = TRUE;
	} else {
		m_CompBall.m_vDown = m_CompBall.ScreenToVector( point.x, point.y );
        m_CompBall.m_qDown = m_CompBall.m_qNow;
		m_CompBall.m_bDrag = TRUE;
	}

	m_bLbDown = true;
	Render3D();
	
	CResizablePage::OnLButtonDown(nFlags, point);
}

void C3DViewPage::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLbDown = false;
	Render3D();

	CResizablePage::OnLButtonUp(nFlags, point);
}

void C3DViewPage::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(m_n3DMoveEyeComponent==0) {
		m_WorldBall.m_iCurMouseX = point.x;
		m_WorldBall.m_iCurMouseY = point.y;
	} else {
		m_CompBall.m_iCurMouseX = point.x;
		m_CompBall.m_iCurMouseY = point.y;
	}
	m_bRbDown = true;
	Render3D();
	
	CResizablePage::OnRButtonDown(nFlags, point);
}

void C3DViewPage::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_bRbDown = false;
	Render3D();
	
	CResizablePage::OnRButtonUp(nFlags, point);
}

void C3DViewPage::OnMButtonDown(UINT nFlags, CPoint point)
{
	if(m_n3DMoveEyeComponent==0) {
		m_WorldBall.m_iCurMouseX = point.x;
		m_WorldBall.m_iCurMouseY = point.y;
	} else {
		m_CompBall.m_iCurMouseX = point.x;
		m_CompBall.m_iCurMouseY = point.y;
	}
	m_bMbDown = TRUE;
	Render3D();
	CDialog::OnRButtonDown(nFlags, point);
}

void C3DViewPage::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_bMbDown = FALSE;
	Render3D();

	CDialog::OnRButtonUp(nFlags, point);
}

BOOL C3DViewPage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	MouseWheel(nFlags, zDelta, pt);
	return CResizablePage::OnMouseWheel(nFlags, zDelta, pt);
}

void C3DViewPage::MouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if(zDelta>0) {
		m_vEyePt.x = (float)MulDiv((int)m_vEyePt.x,110,100);
		m_vEyePt.y = (float)MulDiv((int)m_vEyePt.y,110,100);
		m_vEyePt.z = (float)MulDiv((int)m_vEyePt.z,110,100);
	} else {
		m_vEyePt.x = (float)MulDiv((int)m_vEyePt.x,90,100);
		m_vEyePt.y = (float)MulDiv((int)m_vEyePt.y,90,100);
		m_vEyePt.z = (float)MulDiv((int)m_vEyePt.z,90,100);
	}

	Render3D();

}


inline D3DXQUATERNION* WINAPI D3DXQuaternionUnitAxisToUnitAxis2
( D3DXQUATERNION *pOut, const D3DXVECTOR3 *pvFrom, const D3DXVECTOR3 *pvTo, const BOOL bRightHanded)
{
    D3DXVECTOR3 vAxis;
    D3DXVec3Cross(&vAxis, pvFrom, pvTo);    // proportional to sin(theta)
	if(bRightHanded == TRUE) {
		pOut->x = vAxis.x;
		pOut->y = -vAxis.z;
		pOut->z = vAxis.y;
	} else {
		pOut->x = vAxis.x;
		pOut->y = vAxis.y;
		pOut->z = vAxis.z;
	}
//		pOut->x = vAxis.x;
//		pOut->y = -vAxis.z;
//		pOut->z = -vAxis.y;
    pOut->w = D3DXVec3Dot( pvFrom, pvTo );
    return pOut;
}

inline D3DXQUATERNION* WINAPI D3DXQuaternionAxisToAxis
( D3DXQUATERNION *pOut, const D3DXVECTOR3 *pvFrom, const D3DXVECTOR3 *pvTo, const BOOL bRightHanded)
{
    D3DXVECTOR3 vA, vB;
    D3DXVec3Normalize(&vA, pvFrom);
    D3DXVec3Normalize(&vB, pvTo);
    D3DXVECTOR3 vHalf(vA + vB);
    D3DXVec3Normalize(&vHalf, &vHalf);
    return D3DXQuaternionUnitAxisToUnitAxis2(pOut, &vA, &vHalf,bRightHanded);
}

void C3DViewPage::OnMouseMove(UINT nFlags, CPoint point) 
{
	CD3DArcBall_10 *pBall;
	D3DXVECTOR3 vec;
	D3DXQUATERNION  q;
	float fAngle;
	D3DXMATRIX invMat;

	if(m_n3DMoveEyeComponent==0) {
		pBall = &m_WorldBall;
	} else {
		pBall = &m_CompBall;
	}
	if( m_bLbDown == TRUE ) {
		if( pBall->m_bDrag ) {
			// recompute m_qNow
			D3DXVECTOR3 vCur = pBall->ScreenToVector( point.x, point.y );
			D3DXQUATERNION qAxisToAxis;
			D3DXQuaternionAxisToAxis(&qAxisToAxis, &pBall->m_vDown, &vCur, pBall->m_bRightHanded);
			pBall->m_qNow = pBall->m_qDown;
			pBall->m_qNow *= qAxisToAxis;
			D3DXMatrixRotationQuaternion(&pBall->m_matRotationDelta, &qAxisToAxis);
		} else {
			D3DXMatrixIdentity(&pBall->m_matRotationDelta);
		}
		D3DXMatrixRotationQuaternion(&pBall->m_matRotation, &pBall->m_qNow);
		pBall->m_bDrag = TRUE;
	} else {
		if( m_bRbDown || m_bMbDown)  {
			// Normalize based on size of window and bounding sphere radius
			FLOAT fDeltaX = ( pBall->m_iCurMouseX-point.x ) * pBall->m_fRadiusTranslation / pBall->m_iWidth;
			FLOAT fDeltaY = ( pBall->m_iCurMouseY-point.y ) * pBall->m_fRadiusTranslation / pBall->m_iHeight;
			if( m_bRbDown )
			{
				vec = D3DXVECTOR3( 2*fDeltaX, 2*fDeltaY, 0.0f );
				D3DXMatrixInverse(&invMat,NULL,&pBall->m_matRotation);
				D3DXVec3TransformCoord(&vec, &vec, &invMat);
				if(theApp.m_LastSettings.n3DUpVector == 1) {
					D3DXMatrixTranslation( &pBall->m_matTranslationDelta,-vec.x, -vec.y, vec.z );
				} else {
					D3DXMatrixTranslation( &pBall->m_matTranslationDelta,-vec.x, vec.y, vec.z );
				}
				D3DXMatrixMultiply( &pBall->m_matTranslation, &pBall->m_matTranslation, &pBall->m_matTranslationDelta );
			} else  {// wParam & MK_MBUTTON
				if(theApp.m_LastSettings.n3DUpVector == 1) {
					D3DXMatrixTranslation( &pBall->m_matTranslationDelta, 0.0f, 0.0f, -5*fDeltaY );
				} else {
					D3DXMatrixTranslation( &pBall->m_matTranslationDelta, 0.0f, 0.0f, 5*fDeltaY );
				}
				D3DXMatrixMultiply( &pBall->m_matTranslation, &pBall->m_matTranslation, &pBall->m_matTranslationDelta );
			}

			// Store mouse coordinate
			pBall->m_iCurMouseX=point.x;
			pBall->m_iCurMouseY=point.y;
		}
	}


	D3DXQuaternionToAxisAngle(D3DXQuaternionNormalize(&q,&m_CompBall.m_qNow),&vec,&fAngle);
	PROFILE->m_fRotateX = vec.x * (RAD_TO_DEG * fAngle);
	PROFILE->m_fRotateY = vec.y * (RAD_TO_DEG * fAngle);
	PROFILE->m_fRotateZ = vec.z * (RAD_TO_DEG * fAngle);
	PROFILE->m_fTranslateX = m_CompBall.m_matTranslation._41;
	PROFILE->m_fTranslateY = m_CompBall.m_matTranslation._42;
	PROFILE->m_fTranslateZ = m_CompBall.m_matTranslation._43;

	Render3D();

	CResizablePage::OnMouseMove(nFlags, point);
}


void C3DViewPage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CResizablePage::OnVScroll(nSBCode, nPos, pScrollBar);
}


void C3DViewPage::InitializeObjects()
{
	InitializeRoom();
	InitializeLookAtPoint();
	InitializeXYZ();

	switch(theApp.m_LastSettings.n3DMachineDrawingStyle= STANDARD_10_AXIS) {
	default: InitializeDualSidedSquirter();
		break;
	case TANK_ZERO_NEAR_SIDE:	InitializeTankOfWaterZeroNearSide();
		break;
	case TANK_ZERO_FAR_SIDE:	InitializeTankOfWaterZeroFarSide();
		break;
	}


//	InitPrimitivesForThicknessCScan();
	if(theApp.m_LastSettings.nDisplayTurnTableIn3D == 1) {
//		InitializePrimitiveForRotaryComponent();
		InitializeTurnTable();
	}

	InitializeComponent();
	/*
	if(m_b3DCScan) InitCScanOverlay();
	DefineMesh();
*/
}

void C3DViewPage::InitializeRoom()
{
	SAFE_DELETE( m_p3DObjectRoom );

	int nNumberParts = 5;
	D3DXCOLOR grey(0.5f,0.9f,1.0f,0.0f);
	float fRed = (float)GetRValue(theApp.m_LastSettings.rgbRoomWallColor) / 255.0f;
	float fGreen = (float)GetGValue(theApp.m_LastSettings.rgbRoomWallColor) / 255.0f;
	float fBlue = (float)GetBValue(theApp.m_LastSettings.rgbRoomWallColor) / 255.0f;
	D3DXCOLOR wall = D3DXCOLOR(fRed,fGreen,fBlue,0.0f);

	float fXLength =  theApp.m_Tank.fXLength + 6000.0f;
	float fYLength =  theApp.m_Tank.fYLength + 6000.0f;

	D3DXVECTOR3 vecFloorZero = D3DXVECTOR3(-theApp.m_Tank.fXSurfaceZero-3000.0f,-theApp.m_Tank.fYSurfaceZero-3000.0f,-theApp.m_Tank.fZSurfaceZero-50.0f);
//	D3DXVECTOR3 vecFloorZero = D3DXVECTOR3(-3000.0f,-3000.0f,-theApp.m_Tank.fZSurfaceZero-50.0f);
	D3DXVECTOR3 vecFloorSize = D3DXVECTOR3(fXLength,theApp.m_Tank.fYLength+6000.0f,0.0f);
	D3DXVECTOR3 vecBkWallSize = D3DXVECTOR3(fXLength,0.0f,4000.0f);
	D3DXVECTOR3 vecLeftWallSize = D3DXVECTOR3(0.0f,fYLength,4000.0f);
	D3DXVECTOR3 vecRightWallSize = D3DXVECTOR3(0.0f,-fYLength,4000.0f);

	m_p3DObjectRoom = new C3DObject(m_pd3dDevice,nNumberParts);

	m_p3DObjectRoom->Rectangle(vecFloorSize,grey,&vecFloorZero);
	m_p3DObjectRoom->Rectangle(vecBkWallSize,wall,&D3DXVECTOR3(-theApp.m_Tank.fXSurfaceZero-3000.0f,-theApp.m_Tank.fYSurfaceZero-3000.0f+fYLength,-theApp.m_Tank.fZSurfaceZero-50.0f),&D3DXVECTOR3(0.0f,0.0f,0.0f));
	m_p3DObjectRoom->Rectangle(vecLeftWallSize,wall,&D3DXVECTOR3(-theApp.m_Tank.fXSurfaceZero-3000.0f,-theApp.m_Tank.fYSurfaceZero-3000.0f,-theApp.m_Tank.fZSurfaceZero-50.0f),&D3DXVECTOR3(0.0f,0.0f,0.0f));
	m_p3DObjectRoom->Rectangle(vecRightWallSize,wall,&D3DXVECTOR3(-theApp.m_Tank.fXSurfaceZero-3000.0f+fXLength,-theApp.m_Tank.fYSurfaceZero-3000.0f+fYLength,-theApp.m_Tank.fZSurfaceZero-50.0f),&D3DXVECTOR3(0.0f,0.0f,0.0f));


}

void C3DViewPage::InitializeXYZ()
{
	SAFE_DELETE( m_p3DObjectXYZ );

	m_p3DObjectXYZ = new C3DObject(m_pd3dDevice,3);

	m_p3DObjectXYZ->Rod(10.0f,100.0f,D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXCOLOR(RGB(0,0,255)), &D3DXVECTOR3(0.0f, 0.0f, 0.0f));	//red
	m_p3DObjectXYZ->Rod(10.0f,100.0f,D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXCOLOR(RGB(0,255,0)), &D3DXVECTOR3(0.0f, 0.0f, 0.0f));	//green
	m_p3DObjectXYZ->Rod(10.0f,100.0f,D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXCOLOR(RGB(255,0,0)), &D3DXVECTOR3(0.0f, 0.0f, 0.0f));	//blue

	SAFE_RELEASE( m_pMesh3DText[0] );
	SAFE_RELEASE( m_pMesh3DText[1] );
	SAFE_RELEASE( m_pMesh3DText[2] );
	CreateD3DXTextMesh( &m_pMesh3DText[0], L"X", TEXT("Arial"), 12, FALSE, FALSE );
	CreateD3DXTextMesh( &m_pMesh3DText[1], L"Y", TEXT("Arial"), 12, FALSE, FALSE );
	CreateD3DXTextMesh( &m_pMesh3DText[2], L"Z", TEXT("Arial"), 12, FALSE, FALSE );

	D3DXComputeNormals(m_pMesh3DText[0],NULL); 
	D3DXComputeNormals(m_pMesh3DText[1],NULL); 
	D3DXComputeNormals(m_pMesh3DText[2],NULL); 

}

void C3DViewPage::InitializeDualSidedSquirter()
{
	SAFE_DELETE( m_p3DObjectScanner );

	D3DXVECTOR3	vecTranslation,vecRotate;
	int nNumberParts = (int)(theApp.m_Tank.fXLength / 100.0f);
	nNumberParts += (int)(theApp.m_Tank.fYLength / 100.0f);
	nNumberParts += 20;
	float fX,fY;
	D3DXCOLOR grey(200,200,200,0);
	D3DXCOLOR blue(0,0,255,0);

	D3DXVECTOR3 vecFloorZero = D3DXVECTOR3(-theApp.m_Tank.fXSurfaceZero,-theApp.m_Tank.fYSurfaceZero,-theApp.m_Tank.fZSurfaceZero);
	D3DXVECTOR3 vecFloorSize = D3DXVECTOR3(theApp.m_Tank.fXLength,theApp.m_Tank.fYLength,0.0f);
	D3DXVECTOR3 vecPillar = D3DXVECTOR3(100.0f,100.0f,theApp.m_Tank.fZLength);

	m_p3DObjectScanner = new C3DObject(m_pd3dDevice,nNumberParts);

	m_p3DObjectScanner->Rectangle(vecFloorSize,blue,&vecFloorZero);

	int nOldCullMode = m_p3DObjectScanner->SetCullMode(D3DCULL_NONE);
	for(fX=0.0f;fX<vecFloorSize.x;fX+=100.0f) {
		vecTranslation = vecFloorZero;
		vecTranslation.x += fX;
		m_p3DObjectScanner->Rectangle(D3DXVECTOR3(0.0f,vecFloorSize.y,20.0f),grey,&vecTranslation);
	}
	for(fY=0.0f;fY<vecFloorSize.y;fY+=100.0f) {
		vecTranslation = vecFloorZero;
		vecTranslation.y += fY;
		m_p3DObjectScanner->Rectangle(D3DXVECTOR3(vecFloorSize.x,0.0f,20.0f),grey,&vecTranslation);
	}
	
	m_p3DObjectScanner->SetCullMode(nOldCullMode);
	m_p3DObjectScanner->Cuboid(vecPillar,grey, &D3DXVECTOR3(vecFloorZero.x,vecFloorZero.y,vecFloorZero.z));
	m_p3DObjectScanner->Cuboid(vecPillar,grey, &D3DXVECTOR3(vecFloorZero.x+vecFloorSize.x-100.0f,vecFloorZero.y,vecFloorZero.z));
	m_p3DObjectScanner->Cuboid(vecPillar,grey, &D3DXVECTOR3(vecFloorZero.x+vecFloorSize.x-100.0f,vecFloorZero.y+vecFloorSize.y-100.0f,vecFloorZero.z));
	m_p3DObjectScanner->Cuboid(vecPillar,grey, &D3DXVECTOR3(vecFloorZero.x,vecFloorZero.y+vecFloorSize.y-100.0f,vecFloorZero.z));
}

void C3DViewPage::InitializeTankOfWaterZeroNearSide()
{
	SAFE_DELETE( m_p3DObjectScanner );

	int nNumberParts = 6;

	D3DXCOLOR grey(200,200,200,0);
	D3DXCOLOR blue(0,0,255,0);

	D3DXVECTOR3 vecCornerFarLeft = D3DXVECTOR3(theApp.m_Tank.fXSurfaceZero,-theApp.m_Tank.fYSurfaceZero,theApp.m_Tank.fZLength-theApp.m_Tank.fZSurfaceZero);
	D3DXVECTOR3 vecCornerNearLeft = D3DXVECTOR3(-theApp.m_Tank.fXSurfaceZero,-theApp.m_Tank.fYSurfaceZero,theApp.m_Tank.fZLength-theApp.m_Tank.fZSurfaceZero);
	D3DXVECTOR3 vecCornerFarRight = D3DXVECTOR3(theApp.m_Tank.fXSurfaceZero,theApp.m_Tank.fYLength-theApp.m_Tank.fYSurfaceZero,theApp.m_Tank.fZLength-theApp.m_Tank.fZSurfaceZero);
	D3DXVECTOR3 vecFloorSize = D3DXVECTOR3(-theApp.m_Tank.fXLength,theApp.m_Tank.fYLength,0.0f);
	D3DXVECTOR3 vecLeftWall = D3DXVECTOR3(theApp.m_Tank.fXLength,0.0f,-theApp.m_Tank.fZLength);
	D3DXVECTOR3 vecFarWall = D3DXVECTOR3(0.0f,theApp.m_Tank.fYLength,-theApp.m_Tank.fZLength);
	D3DXVECTOR3 vecRightWall = D3DXVECTOR3(-theApp.m_Tank.fXLength,0.0f,-theApp.m_Tank.fZLength);

	m_p3DObjectScanner = new C3DObject(m_pd3dDevice,nNumberParts);

	m_p3DObjectScanner->Rectangle(vecFloorSize,blue,&vecCornerFarLeft);
	m_p3DObjectScanner->Rectangle(vecLeftWall,grey,&vecCornerNearLeft);
	m_p3DObjectScanner->Rectangle(vecFarWall,grey,&vecCornerFarLeft);
	m_p3DObjectScanner->Rectangle(vecRightWall,grey,&vecCornerFarRight);

}

void C3DViewPage::InitializeTankOfWaterZeroFarSide()
{
	SAFE_DELETE( m_p3DObjectScanner );

	int nNumberParts = 6;

	D3DXCOLOR grey(200,200,200,0);
	D3DXCOLOR blue(0,0,255,0);

	D3DXVECTOR3 vecCornerFarLeft =			D3DXVECTOR3(-theApp.m_Tank.fXSurfaceZero,-theApp.m_Tank.fYSurfaceZero,-theApp.m_Tank.fZSurfaceZero);
	D3DXVECTOR3 vecFarWall =				D3DXVECTOR3(theApp.m_Tank.fXLength,0.0f,theApp.m_Tank.fZLength);
	D3DXVECTOR3 vecLeftWall =				D3DXVECTOR3(0.0f,theApp.m_Tank.fYLength,theApp.m_Tank.fZLength);
	D3DXVECTOR3 vecCornerFarLeftBottom =	D3DXVECTOR3(-theApp.m_Tank.fXSurfaceZero,-theApp.m_Tank.fYSurfaceZero,-theApp.m_Tank.fZSurfaceZero+theApp.m_Tank.fZLength);
	D3DXVECTOR3 vecFloorSize =				D3DXVECTOR3(theApp.m_Tank.fXLength,theApp.m_Tank.fYLength,0.0f);
	D3DXVECTOR3 vecCornerFarRight =			D3DXVECTOR3(theApp.m_Tank.fXLength-theApp.m_Tank.fXSurfaceZero,-theApp.m_Tank.fYSurfaceZero,-theApp.m_Tank.fZSurfaceZero);
	D3DXVECTOR3 vecRightWall =				D3DXVECTOR3(0.0f,theApp.m_Tank.fYLength,theApp.m_Tank.fZLength);

	m_p3DObjectScanner = new C3DObject(m_pd3dDevice,nNumberParts);

	m_p3DObjectScanner->SetCullMode(D3DCULL_CW);	m_p3DObjectScanner->Rectangle(vecFloorSize,blue,&vecCornerFarLeftBottom);
	m_p3DObjectScanner->SetCullMode(D3DCULL_CCW);	m_p3DObjectScanner->Rectangle(vecLeftWall,grey,&vecCornerFarLeft);
	m_p3DObjectScanner->SetCullMode(D3DCULL_CW);	m_p3DObjectScanner->Rectangle(vecFarWall,grey,&vecCornerFarLeft); 	
	m_p3DObjectScanner->SetCullMode(D3DCULL_CW);	m_p3DObjectScanner->Rectangle(vecRightWall,grey,&vecCornerFarRight);

}

void C3DViewPage::InitializeTurnTable()
{
	SAFE_DELETE( m_p3DObjectTurntable );

	int nNumberParts = 2;
	D3DXCOLOR grey(0.5f,0.5f,0.5f,0);

	D3DXVECTOR3 vecCentre = D3DXVECTOR3(theApp.m_Tank.fTTXOffset,theApp.m_Tank.fTTYOffset,theApp.m_Tank.fTTZOffset);
	D3DXVECTOR3 vecNormal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	m_p3DObjectTurntable = new C3DObject(m_pd3dDevice,nNumberParts);

	m_p3DObjectTurntable->Rod(theApp.m_Tank.fTTDiameter, 50.0f, vecNormal, grey, &vecCentre);

}

void C3DViewPage::InitializeRobot()
{
	SAFE_DELETE( m_p3DObjectRobot );

	return;

	int nNumberParts = 20;
	D3DXCOLOR yellow(0.3f,0.7f,0.0f,0);

	D3DXVECTOR3	vecBase = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3	vecBaseNormal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXVECTOR3	vecJ1 =			D3DXVECTOR3(0.0f, 0.0f, 550.0f);
	D3DXVECTOR3	vecJ1Normal =	D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vecJ1ArmSize =	D3DXVECTOR3(100.0f,100.0f,150.0f);

	D3DXVECTOR3	vecJ2 =			D3DXVECTOR3(0.0f, 150.0f, 550.0f);
	D3DXVECTOR3	vecJ2Normal =	D3DXVECTOR3(1.0f, 0.0f, 0.0f);



	m_p3DObjectRobot = new C3DObject(m_pd3dDevice,nNumberParts);

	//diameter, height 
	m_p3DObjectRobot->Rod(337.0f, 400.0f, vecBaseNormal, yellow, &vecBase);
	m_p3DObjectRobot->Cuboid(vecJ1ArmSize, yellow, &vecJ1, &vecJ1Normal);

}

void C3DViewPage::InitializeComponent()
{
	int nLine;

	if (GetSafeHwnd() == NULL) return;

	SAFE_DELETE( m_p3DObjectTaughtLines );
	SAFE_DELETE( m_p3DObjectScanLines );
	SAFE_DELETE( m_p3DObjectBraneLines );
	SAFE_DELETE( m_p3DObjectEnvelopeLines );
	SAFE_DELETE( m_p3DObjectRefPoints );
	SAFE_DELETE( m_p3DObjectOutsideLines );
	SAFE_DELETE( m_p3DObjectInsideLines );

	//Taught lines
	if(PROFILE->m_nTaughtLineL > 0) {
		m_p3DObjectTaughtLines = new C3DObject(m_pd3dDevice,PROFILE->m_nTaughtLineL);
		for(nLine=0;nLine<PROFILE->m_nTaughtLineL;nLine++) {
			m_p3DObjectTaughtLines->PolyLine(&PROFILE->m_TaughtLine[nLine],COLORREF(RGB(0,255,0)));
		}
	}

	//Scan Lines
	switch(PROFILE->m_nComponentDescription) {
	case RECTILINEAR:
		InitializeRectilinearScanLines();
		InitializeRectilinearDataVertices();
		break;
	case CLOSED_ROTARY:
	case CIRCULAR:
		if(PROFILE->m_nScanLineL == 1) InitializeRotoSymmetricScanLines();
		break;
	case OPEN_ROTARY:
		InitializeRectilinearScanLines();
		InitializeRectilinearDataVertices();
		break;
	}

	InitializeScratchLines();
	InitializeBraneLines();
	InitializeOutsideInsideLines();


	//Envelope lines
	if(PROFILE->m_nEnvelopeLineL > 0) {
		m_p3DObjectEnvelopeLines = new C3DObject(m_pd3dDevice,PROFILE->m_nEnvelopeLineL);
		for(nLine=0;nLine<PROFILE->m_nEnvelopeLineL;nLine++) {
			m_p3DObjectEnvelopeLines->SetZMode(false);
			m_p3DObjectEnvelopeLines->PolyLine(&PROFILE->m_EnvelopeLine[nLine],COLORREF(RGB(0,0,255)),1.0,true);
		}
	}

	//Ref points
	m_p3DObjectRefPoints = new C3DObject(m_pd3dDevice,3);
	m_p3DObjectRefPoints->Rod(10.0f,100.0f,(D3DXVECTOR3)PROFILE->m_CpCadHole[0].Side0.norm,D3DXCOLOR(RGB(0,0,255)),(D3DXVECTOR3*)&PROFILE->m_CpCadHole[0].Side0.pt);
	m_p3DObjectRefPoints->Rod(10.0f,100.0f,(D3DXVECTOR3)PROFILE->m_CpCadHole[1].Side0.norm,D3DXCOLOR(RGB(0,255,0)),(D3DXVECTOR3*)&PROFILE->m_CpCadHole[1].Side0.pt);
	m_p3DObjectRefPoints->Rod(10.0f,100.0f,(D3DXVECTOR3)PROFILE->m_CpCadHole[2].Side0.norm,D3DXCOLOR(RGB(255,0,0)),(D3DXVECTOR3*)&PROFILE->m_CpCadHole[2].Side0.pt);

	InitializeDangerZone();

}

void C3DViewPage::InitializeScratchLines()
{
	SAFE_DELETE( m_p3DObjectScratchLines );
	CPolyCoord Line;
	CPolyCoord CurvedLine;
	CCoord CpSurface;
	float fLength,fPos;

	if(PROFILE->m_nTaughtLineL < 2) return;
	m_p3DObjectScratchLines = new C3DObject(m_pd3dDevice,40);

	if(theApp.m_pScanData) {
//		m_p3DObjectScratchLines->PolyLine(&theApp.m_pScanData->m_LineSamplePos,COLORREF(RGB(0.0f,0.0f,1.0f)));
	}

	return;
	CCoord Cp[100];
	Line.Zero();
	for(int nn = 0;nn < PROFILE->m_nTaughtLineL; nn++) {
		PROFILE->m_TaughtLine[nn].GetCoord(0,&Cp[nn]);
		Line.Add(Cp[nn]);
	}
	m_p3DObjectScratchLines->PolyLine(&Line,COLORREF(RGB(1.0f,0,0)));

	Line.SetStyle(1);
	Line.SetPrimaryAxis(30);
	Line.PreProcessLine();
	fLength = Line.GetLength();

	CurvedLine.Zero();
	for(int ii=0;ii<100;ii++) {
		fPos = (float)ii * fLength / 99.0f;
		Line.CalculateCoord(fPos,CpSurface);
		CurvedLine.Add(CpSurface);
	}
//	m_p3DObjectScratchLines->PolyLine(&CurvedLine,COLORREF(RGB(1.0f,0,0)));


	CNURBS	Nurb;
	int nCoordL = PROFILE->m_nTaughtLineL + 1;
	int nCurvePts = 100;
	int nOrder = 3;
	D3DXVECTOR3 *vecCtrlPts = new D3DXVECTOR3[nCoordL];
	D3DXVECTOR3 vecCurvePts[300];
	D3DXVECTOR3 vecIntersection,vecZ0,vecZ1;
	D3DXPLANE plane[2];
	ZeroMemory(vecCtrlPts,(sizeof D3DXVECTOR3) * nCoordL);
	ZeroMemory(vecCurvePts,sizeof vecCurvePts);

	float *fKnots = new float[nCoordL + nOrder + 1];
	int mm,nn;

	vecCtrlPts[0] = Cp[0].Side0.pt;
	int jj=1;
	for(nn = 0, mm = 1;nn < (PROFILE->m_nTaughtLineL-1); nn++, mm++) {

		mm %= PROFILE->m_nTaughtLineL;

		D3DXPlaneFromPointNormal(&plane[0],&Cp[nn].Side0.pt,(D3DXVECTOR3*)&Cp[nn].Side0.norm);
		D3DXPlaneFromPointNormal(&plane[1],&Cp[mm].Side0.pt,(D3DXVECTOR3*)&Cp[mm].Side0.norm);
		D3DXVec3Cross(&vecIntersection, (D3DXVECTOR3*)&plane[1], (D3DXVECTOR3*)&plane[0]);

		//if Z == 0 then the two planes cross at
		vecZ0.z = 0.0f;
		SolveForZEqualZero(&vecZ0,&plane[0],&plane[1]);
		vecZ1.x = vecZ0.x + 1500.0f * vecIntersection.x;
		vecZ1.y = vecZ0.y + 1500.0f * vecIntersection.y;
		vecZ1.z = vecZ0.z + 1500.0f * vecIntersection.z;

		CurvedLine.Zero();
		CpSurface.Side0.pt = vecZ0;
		CurvedLine.Add(CpSurface);

		float fMag = 1.0f;

		CpSurface.Side0.pt.z = Cp[nn].Side0.pt.z;
		CpSurface.Side0.pt.x = ((vecZ1.x - vecZ0.x) * (Cp[nn].Side0.pt.z - vecZ0.z) / (vecZ1.z - vecZ0.z) * fMag) + vecZ0.x;
		CpSurface.Side0.pt.y = ((vecZ1.y - vecZ0.y) * (Cp[nn].Side0.pt.z - vecZ0.z) / (vecZ1.z - vecZ0.z) * fMag) + vecZ0.y;
		CurvedLine.Add(CpSurface);
		m_p3DObjectScratchLines->PolyLine(&CurvedLine,COLORREF(RGB(1.0f,0,0)));

		vecCtrlPts[jj++] = CpSurface.Side0.pt;
	}
	vecCtrlPts[jj++] = Cp[PROFILE->m_nTaughtLineL-1].Side0.pt;

	Nurb.knot(nCoordL,nOrder,fKnots);
	Nurb.bspline(nCoordL,nOrder,nCurvePts,vecCtrlPts,fKnots,vecCurvePts);

	CurvedLine.Zero();
	CpSurface.Zero();
	for(int ii=0;ii<nCurvePts;ii++) {
		CpSurface.Side0.pt = vecCurvePts[ii];
		CurvedLine.Add(CpSurface);
	}
//	m_p3DObjectScratchLines->PolyLine(&CurvedLine,COLORREF(RGB(1.0f,0,0)));

//	float fHomogeneous[6] = {1.0f, 10.0f, 6.0f, 10.0f, 4.0f, 1.0f};
	float fHomogeneous[6] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
	Nurb.rationalbspline(nCoordL,nOrder,nCurvePts,vecCtrlPts,fKnots,fHomogeneous,vecCurvePts);

	CurvedLine.Zero();
	CpSurface.Zero();
	for(int ii=0;ii<nCurvePts;ii++) {
		CpSurface.Side0.pt = vecCurvePts[ii];
		CurvedLine.Add(CpSurface);
	}
	m_p3DObjectScratchLines->PolyLine(&CurvedLine,COLORREF(RGB(1.0f,0,0)));



	delete fKnots;
	delete vecCtrlPts;
}

HRESULT C3DViewPage::CreateD3DXTextMesh( LPD3DXMESH* ppMesh, CString strText, TCHAR* pstrFont, DWORD dwSize, BOOL bBold, BOOL bItalic )
{
    HRESULT hr;
    LPD3DXMESH pMeshNew = NULL;
    HDC hdc = CreateCompatibleDC( NULL );
    INT nHeight = -MulDiv( dwSize, GetDeviceCaps(hdc, LOGPIXELSY), 72 );
    HFONT hFont;
    HFONT hFontOld;


    hFont = CreateFont(nHeight, 0, 0, 0, bBold ? FW_BOLD : FW_NORMAL, bItalic, FALSE, FALSE, DEFAULT_CHARSET, 
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, pstrFont);
    
    hFontOld = (HFONT)SelectObject(hdc, hFont); 

    hr = D3DXCreateText(m_pd3dDevice, hdc, strText, 
                        0.101f, 0.101f, &pMeshNew, NULL, NULL);

    SelectObject(hdc, hFontOld);
    DeleteObject( hFont );
    DeleteDC( hdc );

    if( SUCCEEDED( hr ) )
        *ppMesh = pMeshNew;

//	hr = D3DXComputeNormals( pMeshNew, NULL );

    return hr;
}


//uses Cramer's rule
void C3DViewPage::SolveForZEqualZero(D3DXVECTOR3* pvecOut, D3DXPLANE* plane0, D3DXPLANE* plane1)
{

	float fDeterminant;
	float a,b,c,d,e,f;
	a = plane0->a;
	b = plane0->b;
	c = plane1->a;
	d = plane1->b;
	e = plane0->d;
	f = plane1->d;

	fDeterminant = ( (a * d) - (c * b) ) * -1.0f;	//I've had to multiply by -1. I dont know why. I shouldn't have to.
	pvecOut->x = ((e * d) - (f * b)) / fDeterminant;
	pvecOut->y = ((a * f) -(c * e)) / fDeterminant;

}

void C3DViewPage::InitializeRectilinearScanLines()
{
	int nLine;
	SAFE_DELETE( m_p3DObjectScanLines );

	if(PROFILE->m_nScanLineL<=0) return;
	m_p3DObjectScanLines = new C3DObject(m_pd3dDevice,PROFILE->m_nScanLineL);
	if(theApp.m_LastSettings.n3DFillInScanLines ==0) {
		for(nLine=0;nLine<PROFILE->m_nScanLineL;nLine++) {
			m_p3DObjectScanLines->PolyLine(&PROFILE->m_ScanLine[nLine],COLORREF(RGB(0,255,0)));
		}
	} else {
		m_p3DObjectScanLines->SetCullMode(D3DCULL_NONE);
		for(nLine=0;nLine<(PROFILE->m_nScanLineL-1);nLine++) {
			m_p3DObjectScanLines->FillTwoPolyLines(&PROFILE->m_ScanLine[nLine],&PROFILE->m_ScanLine[nLine+1],D3DXCOLOR(RGB(0xa0,0xa0,0xa0)));
		}
	}

}

void C3DViewPage::InitializeBraneLines()
{
	int nLine;
	SAFE_DELETE( m_p3DObjectBraneLines );
	CPolyCoord Line;

	if(PROFILE->m_nBrainLineL<=0) return;
	m_p3DObjectBraneLines = new C3DObject(m_pd3dDevice,PROFILE->m_nBrainLineL);
	for(nLine=0;nLine<PROFILE->m_nBrainLineL;nLine++) {
		Line.Zero();
		Line = &PROFILE->m_BrainLine[nLine];
		Line.ShiftAllCoordsAlongVector(1.0f);
		m_p3DObjectBraneLines->PolyLine(&Line,COLORREF(RGB(255,0,0)));
	}

}

void C3DViewPage::InitializeOutsideInsideLines()
{
	int nLine;
	SAFE_DELETE( m_p3DObjectOutsideLines );
	SAFE_DELETE( m_p3DObjectInsideLines );

	if(PROFILE->m_nLineBladeL[OUTSIDE] > 0) {
		m_p3DObjectOutsideLines = new C3DObject(m_pd3dDevice,PROFILE->m_nLineBladeL[OUTSIDE]);
		for(nLine=0;nLine<PROFILE->m_nLineBladeL[OUTSIDE];nLine++) {
			m_p3DObjectOutsideLines->PolyLine(&PROFILE->m_LineBlade[OUTSIDE][nLine],COLORREF(RGB(0,255,0)));
		}
	}
	if(PROFILE->m_nLineBladeL[INSIDE] > 0) {
		m_p3DObjectInsideLines = new C3DObject(m_pd3dDevice,PROFILE->m_nLineBladeL[INSIDE]);
		for(nLine=0;nLine<PROFILE->m_nLineBladeL[INSIDE];nLine++) {
			m_p3DObjectInsideLines->PolyLine(&PROFILE->m_LineBlade[INSIDE][nLine],COLORREF(RGB(0,255,0)));
		}
	}

}

void C3DViewPage::InitializeRectilinearDataVertices()
{
	int nLine;
	D3DXCOLOR rgb;
	SAFE_DELETE( m_p3DObjectDataVertices );

	if(theApp.m_pViewData == NULL) return;
	if(theApp.m_pViewData->m_vLineSample == NULL) return;
//	if((PROFILE->m_nComponentDescription != RECTILINEAR) || (PROFILE->m_nComponentDescription != OPEN_ROTARY)) return;

    CONST FLOAT f = 1.0f / 255.0f;
    rgb.r = f * (FLOAT) (unsigned char) (theApp.m_LastSettings.rgbVerticesColor >>  0);
    rgb.g = f * (FLOAT) (unsigned char) (theApp.m_LastSettings.rgbVerticesColor >>  8);
    rgb.b = f * (FLOAT) (unsigned char) (theApp.m_LastSettings.rgbVerticesColor >> 16);
    rgb.a = f * (FLOAT) (unsigned char) (theApp.m_LastSettings.rgbVerticesColor >> 24);


	m_p3DObjectDataVertices = new C3DObject(m_pd3dDevice,theApp.m_pViewData->m_nNumberLines);
	if(theApp.m_LastSettings.n3DFillInScanLines ==0) {
		for(nLine=0;nLine<theApp.m_pViewData->m_nNumberLines;nLine++) {
			m_p3DObjectDataVertices->PolyLine(&theApp.m_pViewData->m_vLineSample[nLine],rgb);
		}
	} else {
		m_p3DObjectDataVertices->SetCullMode(D3DCULL_NONE);
		for(nLine=0;nLine<(theApp.m_pViewData->m_nNumberLines-1);nLine++) {
			m_p3DObjectDataVertices->FillTwoPolyLines(&theApp.m_pViewData->m_vLineSample[nLine],&theApp.m_pViewData->m_vLineSample[nLine+1],rgb);
		}
	}

}



void C3DViewPage::InitializeRotoSymmetricScanLines()
{
	CPolyCoord Line[2];
	CCoord Cp,Cp1;
	float fRadius,fAngle,fI,fJ, fX, fY;
	int nStart,nFinish;

	SAFE_DELETE( m_p3DObjectScanLines );

	if(PROFILE->m_nComponentDescription != CIRCULAR) return;
	if(PROFILE->m_ScanLine == NULL) return;
	if(PROFILE->m_nScanLineL != 1) return;

	int nSamplesLine = 30;
	int	nNumberLines = PROFILE->m_ScanLine[0].m_nCoordL;

	Line[0].AllocateCoords(nSamplesLine);
	Line[1].AllocateCoords(nSamplesLine);

	m_p3DObjectScanLines = new C3DObject(m_pd3dDevice,nNumberLines);
	m_p3DObjectScanLines->SetCullMode(D3DCULL_NONE);

	nStart = 0;
	nFinish = nSamplesLine;

	for(int nLine=0;nLine<nNumberLines;nLine++) {
		PROFILE->m_ScanLine[0].GetCoord(nLine,&Cp1);
		
		fX = Cp1.Side0.fX - theApp.m_Tank.vTTCentre.x;
		fY = Cp1.Side0.fY - theApp.m_Tank.vTTCentre.y;

		fRadius = sqrtf(fX * fX + fY * fY);

		int nn,ii;
		for(nn=nStart,ii=0;nn<nFinish; nn++) {
			fAngle = (2 * PIf * (float)nn) / (float)(nSamplesLine-2);

			fI = cosf(fAngle) * -1.0f;
			fJ = sinf(fAngle) * -1.0f;

			Cp.Side0.fX = fRadius * fI + theApp.m_Tank.vTTCentre.x;
			Cp.Side0.fY = fRadius * fJ + theApp.m_Tank.vTTCentre.y;
			Cp.Side0.fZ = Cp1.Side0.fZ;
			Cp.Side0.fI = fI;
			Cp.Side0.fJ = fJ;
			Cp.Side0.fK = Cp1.Side0.fK;
			Line[1].ModifyCoord(nn, Cp);
		}
		Line[1].m_nCoordL = nn;
		if(nLine>0) {
			if(theApp.m_LastSettings.n3DFillInScanLines ==0) {
				m_p3DObjectScanLines->FillTwoPolyLines(&Line[0], &Line[1], D3DXCOLOR(RGB(0,255,0)));
			} else {
				m_p3DObjectScanLines->SetCullMode(D3DCULL_NONE);
				m_p3DObjectScanLines->FillTwoPolyLines(&Line[0], &Line[1],D3DXCOLOR(RGB(0xa0,0xa0,0xa0)));
			}
		}
		Line[0] = &Line[1];
	}
}

void C3DViewPage::InitializeDangerZone()
{
	SAFE_DELETE( m_p3DObjectDangerPlanes );
	if((PROFILE->m_nScanLineL<=0) && (PROFILE->m_nTaughtLineL<=0) && (PROFILE->m_nLineBladeL[OUTSIDE]==0) && (PROFILE->m_nLineBladeL[INSIDE]==0) ) return;

	int nNumberParts = 2;
	D3DXCOLOR red(1.0f,0.0f,0.0f,0.2f);
	D3DXCOLOR green(0.0f,1.0f,0.0f,0.2f);
	D3DXVECTOR3 vecPortZero;
	D3DXVECTOR3 vecStarboardZero;
	D3DXVECTOR3 vecSize;
	float fWidth;


	CCoord CpMin = PROFILE->m_CpDanger[DANGER_MINS];
	CCoord CpMax = PROFILE->m_CpDanger[DANGER_MAXS];

	vecPortZero			= CpMin.Side0.pt;
	vecStarboardZero	= CpMin.Side1.pt;
	if(PROFILE->m_vecToDangerPlane[0].x != 0.0f) {
		vecStarboardZero.x	= CpMax.Side1.fX;
		vecSize				= D3DXVECTOR3(5.0f, fabs(CpMax.Side0.fY-CpMin.Side0.fY), fabs(CpMax.Side0.fZ-CpMin.Side0.fZ));
	}
	if(PROFILE->m_vecToDangerPlane[0].y != 0.0f) {
		vecStarboardZero.y	= CpMax.Side1.fY;
		vecSize				= D3DXVECTOR3(fabs(CpMax.Side0.fX-CpMin.Side0.fX), 5.0f, fabs(CpMax.Side0.fZ-CpMin.Side0.fZ));
	}
	if(PROFILE->m_vecToDangerPlane[0].z != 0.0f) {
		vecStarboardZero.z	= CpMax.Side1.fZ;
		vecSize				= D3DXVECTOR3(fabs(CpMax.Side0.fX-CpMin.Side0.fX), fabs(CpMax.Side0.fY-CpMin.Side0.fY), 5.0f);
	}

	m_p3DObjectDangerPlanes = new C3DObject(m_pd3dDevice,nNumberParts);
//	m_p3DObjectDangerPlanes->SetCullMode(0);

	switch(PROFILE->m_nComponentDescription) { 
	case RECTILINEAR:
		switch(theApp.m_Tank.nScannerDescription) {
		default:
			m_p3DObjectDangerPlanes->Cuboid(vecSize,red,&vecPortZero);
			m_p3DObjectDangerPlanes->Cuboid(vecSize,green,&vecStarboardZero);
			break;
		case SINGLE_BRIDGE_TT_ROLLERS:
		case SINGLE_BRIDGE_TANK_WITH_R:
		case SINGLE_BRIDGE_TANK:
		case DOUBLE_BRIDGE_TANK:
			m_p3DObjectDangerPlanes->Rectangle(vecSize,red,&vecPortZero);
			break;
		}

		break;
	case CLOSED_ROTARY:
	case OPEN_ROTARY:
	case CIRCULAR:
		vecStarboardZero.x = theApp.m_Tank.vTTCentre.x;
		vecStarboardZero.y = theApp.m_Tank.vTTCentre.y;
		vecStarboardZero.z = CpMin.Side1.fZ;
		PROFILE->m_fComponentDiameter[0] > PROFILE->m_fComponentDiameter[1] ? fWidth = PROFILE->m_fComponentDiameter[0] : fWidth = PROFILE->m_fComponentDiameter[1];

		if(theApp.m_Tank.vTTZeroDegrees.x) {
			vecSize	= D3DXVECTOR3(10.0f, fWidth, fabs(CpMax.Side0.fZ-CpMin.Side0.fZ));
		}
		if(theApp.m_Tank.vTTZeroDegrees.y) {
			vecSize	= D3DXVECTOR3(fWidth, 10.0f, fabs(CpMax.Side0.fZ-CpMin.Side0.fZ));
		}
		m_p3DObjectDangerPlanes->SetCullMode(D3DCULL_NONE);
		m_p3DObjectDangerPlanes->Rectangle(vecSize,red,&vecPortZero, NULL, true);
		m_p3DObjectDangerPlanes->SetCullMode(D3DCULL_CW);
		m_p3DObjectDangerPlanes->Cylinder(CpMin.fRadius[STARBOARD]*2.0f, fabs(CpMax.Side1.fZ-CpMin.Side1.fZ), D3DXVECTOR3(0.0f,0.0f,1.0f), green, &vecStarboardZero,true);
		break;
	}

}

void C3DViewPage::InitializeCScanOverlay()
{
	int nLine;

	if(theApp.m_p3DData == NULL) return;

	CWaitCursor Wait;

	if(theApp.m_LastSettings.nCScanOverlayResolution < 0) theApp.m_LastSettings.nCScanOverlayResolution = 0;
	if(theApp.m_LastSettings.nCScanOverlayResolution > 4) theApp.m_LastSettings.nCScanOverlayResolution = 4;
	int nRes = theApp.m_LastSettings.nCScanOverlayResolution + 1;
	
	theApp.m_p3DData->PutvLineSampleInCADCoordinates();

	SAFE_DELETE( m_p3DObjectCScanOverlay );
	m_p3DObjectCScanOverlay = new C3DObject(m_pd3dDevice,(theApp.m_p3DData->m_nNumberLines / nRes));
	m_p3DObjectCScanOverlay->SetCullMode(D3DCULL_NONE);
	for(nLine=0;nLine<(theApp.m_p3DData->m_nNumberLines-nRes);nLine+=nRes) {
		m_p3DObjectCScanOverlay->BuildCScanLine(nLine,theApp.m_p3DData,theApp.m_n3DImageNumber,theApp.m_n3DImageType);
	}

	theApp.m_p3DData->PutvLineSamplesBackInttoScanCoordinates();

	Wait.Restore();
}
#define	BLUE3D		0xff0000ff
#define	GREEN3D		0xff00ff00
#define	RED3D		0xffff0000
#define	YELLOW3D	0xffffff00

void C3DViewPage::InitializeLookAtPoint()
{
	SAFE_DELETE( m_p3DObjectLookAtPoint );
	CPolyCoord Line;
	CCoord Cp;

	m_p3DObjectLookAtPoint = new C3DObject(m_pd3dDevice,20);


	Line.Zero();	Cp.Zero();
	Cp.Side0.fX=-1000.0;	Line.Add(Cp);
	Cp.Side0.fX=+1000.0;	Line.Add(Cp);
	m_p3DObjectLookAtPoint->PolyLine(&Line,COLORREF(RGB(0,0,1.0f)));
	Line.Zero();	Cp.Zero();
	Cp.Side0.fY=-1000.0;	Line.Add(Cp);
	Cp.Side0.fY=+1000.0;	Line.Add(Cp);
	m_p3DObjectLookAtPoint->PolyLine(&Line,COLORREF(RGB(0,1.0f,0)));
	Line.Zero();	Cp.Zero();
	Cp.Side0.fZ=-1000.0;	Line.Add(Cp);
	Cp.Side0.fZ=+1000.0;	Line.Add(Cp);
	m_p3DObjectLookAtPoint->PolyLine(&Line,COLORREF(RGB(1.0f,0,0)));
	
}

void C3DViewPage::DrawLookAtPoint()
{
	D3DXMATRIXA16 matWorld;
	C3DPrimitive Prim;

	if(m_bRbDown || m_bLbDown || m_bMbDown) {

		m_pd3dDevice->SetTexture(0, NULL );
		D3DXMatrixIdentity(&matWorld);

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
		Prim.Draw3DLine(m_pd3dDevice,-1000.0f,0.0f,0.0f,1000.0f,0.0f,0.0f,RED3D);
		Prim.Draw3DLine(m_pd3dDevice,0.0f,-1000.0f,0.0f,0.0f,1000.0f,0.0f,GREEN3D);
		Prim.Draw3DLine(m_pd3dDevice,0.0f,0.0f,-1000.0f,0.0f,0.0f,1000.0f,BLUE3D);
		Prim.Draw3DCircle(m_pd3dDevice,D3DXVECTOR3(0.0,0.0,0.0),D3DXVECTOR3(1.0,0.0,0.0),200.0,RED3D);
		Prim.Draw3DCircle(m_pd3dDevice,D3DXVECTOR3(0.0,0.0,0.0),D3DXVECTOR3(0.0,1.0,0.0),200.0,GREEN3D);
		Prim.Draw3DCircle(m_pd3dDevice,D3DXVECTOR3(0.0,0.0,0.0),D3DXVECTOR3(0.0,0.0,1.0),200.0,BLUE3D);

		if(m_bRbDown) {
			Prim.Draw3DCone(m_pd3dDevice,D3DXVECTOR3(1000.0,0.0,0.0),D3DXVECTOR3(1.0,0.0,0.0),20.0,100.0,RED3D);
			Prim.Draw3DCone(m_pd3dDevice,D3DXVECTOR3(0.0,1000.0,0.0),D3DXVECTOR3(0.0,1.0,0.0),20.0,-100.0,GREEN3D);
		}

		if(m_bMbDown) {
			Prim.Draw3DCone(m_pd3dDevice,D3DXVECTOR3(0.0,0.0,1000.0),D3DXVECTOR3(0.0,0.0,1.0),20.0,100.0,BLUE3D);
		}

	}
}


void C3DViewPage::OnButton3dViewPoint() 
{
	D3DXMATRIXA16 matWorld;

	if(m_n3DMoveEyeComponent == 1) {
		CWaitCursor Wait;

		D3DXMatrixIdentity( &matWorld);
		D3DXMatrixMultiply( &matWorld, &matWorld, m_CompBall.GetTranslationMatrix() );
		D3DXMatrixMultiply( &matWorld, &matWorld, m_CompBall.GetRotationMatrix() );

		PROFILE->ComponentMove(matWorld);

		m_CompBall.Init(TRUE);
		CleanUp3D();
		Start3D();

		Wait.Restore();
	}

	m_n3DMoveEyeComponent = 0;
	SetToolBarCheckedState();
	
}

void C3DViewPage::OnButton3dComponent() 
{
	m_n3DMoveEyeComponent = 1;
	SetToolBarCheckedState();
	
}

void C3DViewPage::OnButton3dDangerZone() 
{
	theApp.m_LastSettings.nDisplay3DDangerZone>0 ? theApp.m_LastSettings.nDisplay3DDangerZone=0 : theApp.m_LastSettings.nDisplay3DDangerZone=1;
	if(theApp.m_LastSettings.nDisplay3DDangerZone==1) {
		theApp.m_WS.CalculateDangerZone();
		InitializeDangerZone();
	}
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dMachine() 
{
	theApp.m_LastSettings.nDisplay3DMachine>0 ? theApp.m_LastSettings.nDisplay3DMachine=0 : theApp.m_LastSettings.nDisplay3DMachine=1;
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dRefPoints() 
{
	theApp.m_LastSettings.nDisplay3DRefPoints>0 ? theApp.m_LastSettings.nDisplay3DRefPoints=0 : theApp.m_LastSettings.nDisplay3DRefPoints=1;
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dScanlines() 
{
	theApp.m_LastSettings.nDisplay3DScanLines==FALSE ? theApp.m_LastSettings.nDisplay3DScanLines=TRUE : theApp.m_LastSettings.nDisplay3DScanLines=FALSE;
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dTaughtlines() 
{
	theApp.m_LastSettings.nDisplay3DTaughtLines==FALSE ? theApp.m_LastSettings.nDisplay3DTaughtLines=TRUE : theApp.m_LastSettings.nDisplay3DTaughtLines=FALSE;
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dEnvelope() 
{
	theApp.m_LastSettings.nDisplay3DEnvelope>0 ? theApp.m_LastSettings.nDisplay3DEnvelope=0 : theApp.m_LastSettings.nDisplay3DEnvelope=1;
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dBranelines() 
{
	theApp.m_LastSettings.nDisplay3DBraneLines==FALSE ? theApp.m_LastSettings.nDisplay3DBraneLines=TRUE : theApp.m_LastSettings.nDisplay3DBraneLines=FALSE;
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dRobot() 
{
	theApp.m_LastSettings.nDisplay3DRobot>0 ? theApp.m_LastSettings.nDisplay3DRobot=0 : theApp.m_LastSettings.nDisplay3DRobot=1;
	if(theApp.m_LastSettings.nDisplay3DRobot != 0) InitializeRobot();
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dTracker() 
{
	theApp.m_LastSettings.nDisplay3DTracker>0 ? theApp.m_LastSettings.nDisplay3DTracker=0 : theApp.m_LastSettings.nDisplay3DTracker=1;
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButton3dCscan() 
{
	if(m_b3DCScan==FALSE) {
		if(theApp.m_LastSettings.nCScanOverlayMode == 0) {
			InitializeCScanOverlay();
		} else {
//			if( FAILED(BuildComponentMesh(NULL)) )	return ;
		}
		m_b3DCScan = TRUE;
	} else {
		m_b3DCScan = FALSE;
	}

	SetToolBarCheckedState();
	Render3D();
}

BOOL C3DViewPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
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
	case ID_BUTTON_3D_TAUGHTLINES: strTipText.LoadString(IDS_Taught_Lines);
		break;
	case ID_BUTTON_3D_SCANLINES: strTipText.LoadString(IDS_Scan_Lines);
		break;
	case ID_BUTTON_3D_ENVELOPE: strTipText.LoadString(IDS_Envelope);
		break;
	case ID_BUTTON_3D_REF_POINTS: strTipText.LoadString(IDS_Ref_Points);
		break;
	case ID_BUTTON_3D_ROBOT: strTipText.LoadString(IDS_Robot);
		break;
	case ID_BUTTON_3D_BALL_ROLL: strTipText.LoadString(IDS_Ball_Roll);
		break;
	case ID_BUTTON_3D_ROTATE_ABOUT_Z: strTipText.LoadString(IDS_Rotate_about_Z);
		break;
	case ID_BUTTON_3D_ROTATE_ABOUT_X: strTipText.LoadString(IDS_Rotate_about_X);
		break;
	case ID_BUTTON_3D_ROTATE_ABOUT_Y: strTipText.LoadString(IDS_Rotate_about_Y);
		break;
	case ID_BUTTON_DISPLAY_OUTSIDE: strTipText.LoadString(IDS_Outside_blade);
		break;
	case ID_BUTTON_DISPLAY_INSIDE: strTipText.LoadString(IDS_Inside_blade);
		break;
	}

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}

void C3DViewPage::SetToolBarCheckedState()
{
	int nIndex;

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_VIEW_POINT))>=0)
		m_n3DMoveEyeComponent==0 ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_COMPONENT))>=0)
		m_n3DMoveEyeComponent==1 ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_MACHINE))>=0)
		theApp.m_LastSettings.nDisplay3DMachine==1 ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_DANGER_ZONE))>=0)
		theApp.m_LastSettings.nDisplay3DDangerZone==1 ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_CSCAN))>=0)
		m_b3DCScan==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_TAUGHTLINES))>=0)
		theApp.m_LastSettings.nDisplay3DTaughtLines==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_SCANLINES))>=0)
		theApp.m_LastSettings.nDisplay3DScanLines==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_BRANELINES))>=0)
		theApp.m_LastSettings.nDisplay3DBraneLines==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_REF_POINTS))>=0)
		theApp.m_LastSettings.nDisplay3DRefPoints==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_ENVELOPE))>=0)
		theApp.m_LastSettings.nDisplay3DEnvelope==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

//	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_SURFACE))>=0)
//		m_b3DSurface==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_DATA_VERTICES))>=0)
		m_b3DDataVertices==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_ROBOT))>=0)
		theApp.m_LastSettings.nDisplay3DRobot==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_TRACKER))>=0)
		theApp.m_LastSettings.nDisplay3DTracker==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);
	
	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_DISPLAY_OUTSIDE))>=0)
		theApp.m_LastSettings.nDisplay3DOutsideLines==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_DISPLAY_INSIDE))>=0)
		theApp.m_LastSettings.nDisplay3DInsideLines==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_DISPLAY_WORKSHEET_PROFILES))>=0) {
		theApp.m_LastSettings.nDisplay3DWorksheet==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);
		if(theApp.m_WS.GetSize() <= 0 ) 
			m_wnd3DToolBar.GetToolBarCtrl().SetState(ID_BUTTON_DISPLAY_WORKSHEET_PROFILES,0);
	}


	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_BALL_ROLL))>=0)
		m_WorldBall.m_nDragAxes == 0 ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_ROTATE_ABOUT_Z))>=0)
		m_WorldBall.m_nDragAxes == 3 ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_ROTATE_ABOUT_Y))>=0)
		m_WorldBall.m_nDragAxes == 2 ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_ROTATE_ABOUT_X))>=0)
		m_WorldBall.m_nDragAxes == 1 ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

	//	if((nIndex=m_wnd3DToolBar.CommandToIndex(ID_BUTTON_3D_ISOMETRIC))>=0)
//		m_bDisplayIsometric==TRUE ? m_wnd3DToolBar.SetButtonStyle(nIndex,TBBS_CHECKED) : m_wnd3DToolBar.SetButtonStyle(nIndex,0);

}


void C3DViewPage::OnButton3dTools() 
{

	CProfileSheet *pSheet = (CProfileSheet*)GetParent();
	pSheet->ToolsDrawingoptions();
}

void C3DViewPage::OnButtonSurface() 
{
	if(m_b3DSurface == false) {
		m_b3DSurface = true;

		InitializeSurface();

	} else {
		m_b3DSurface = false;
	}
	SetToolBarCheckedState();
	Render3D();
	
}

void C3DViewPage::InitializeSurface()
{
	SAFE_DELETE( m_p3DObjectSurfaceLines );
	CPolyCoord Line;

	if(PROFILE->m_SurfaceLine == NULL) return;
	if(PROFILE->m_nSurfaceLineL == 0) return;
	m_p3DObjectSurfaceLines = new C3DObject(m_pd3dDevice,PROFILE->m_nSurfaceLineL);

	for(int nLine=0;nLine<PROFILE->m_nSurfaceLineL;nLine++) {
		Line = &PROFILE->m_SurfaceLine[nLine];
		Line.StripOutZeroCoords();
		if(Line.m_nCoordL>0)	m_p3DObjectSurfaceLines->PolyLine(&Line,COLORREF(RGB(0.0f,0.0f,1.0f)));
	}

}

void C3DViewPage::OnButton3dDataVertices() 
{
	if(m_b3DDataVertices == false) {
		m_b3DDataVertices = true;
		InitializeRectilinearDataVertices();
	} else {
		m_b3DDataVertices = false;
	}
	SetToolBarCheckedState();
	Render3D();
	
}


void C3DViewPage::LookAtPoint(CCoord Cp,bool bRotate)
{

	m_WorldBall.SetTranslationMatrix(Cp.Side0.fX, Cp.Side0.fY, Cp.Side0.fZ);
	if(bRotate == true) 
		m_WorldBall.SetRotationMatrix(Cp.Side0.fI, Cp.Side0.fJ, Cp.Side0.fK);
	D3DXQuaternionRotationMatrix(&m_WorldBall.m_qNow,m_WorldBall.GetRotationMatrix());

}



void C3DViewPage::OnButton3dBallRoll() 
{
	m_WorldBall.m_nDragAxes = 0;
	m_CompBall.m_nDragAxes = 0;
	SetToolBarCheckedState();
}

void C3DViewPage::OnButton3dRotateAboutZ() 
{
	m_WorldBall.m_nDragAxes = 3;
	m_CompBall.m_nDragAxes = 3;
	SetToolBarCheckedState();
}

void C3DViewPage::OnButton3dRotateAboutY() 
{
	m_WorldBall.m_nDragAxes = 2;
	m_CompBall.m_nDragAxes = 2;
	SetToolBarCheckedState();
}

void C3DViewPage::OnButton3dRotateAboutX() 
{
	m_WorldBall.m_nDragAxes = 1;
	m_CompBall.m_nDragAxes = 1;
	SetToolBarCheckedState();
}

void C3DViewPage::DrawRobot()
{
	int nRobot;
	C3DPrimitive Prim;
	D3DXMATRIXA16 matCoord,matTranslation,matRotX,matRotY;

	D3DXVECTOR3 vecBase, vecFirstColumn;
	D3DXVECTOR3 vecBaseHArm, vecFirstHArm;
	D3DXVECTOR3 vecBaseVArm, vecFirstVArm;
	D3DXVECTOR3 vecBaseSecondHArm, vecSecondHArm;
	D3DXVECTOR3 vecBaseSecondVArm, vecSecondVArm;
	D3DXVECTOR3 vecBaseThirdHArm, vecThirdHArm;
	D3DXVECTOR3 vecBaseThirdVArm, vecThirdVArm;
	D3DXVECTOR3 vecBaseSearchTube, vecSearchTubeArm, vecWaterNozzle;


	if(theApp.m_LastSettings.nDisplay3DRobot == TRUE) {

		for(nRobot = 0;nRobot<1;nRobot++) {

			vecBase				=	D3DXVECTOR3(0.0f,0.0f,0.0f);
			vecFirstColumn		=	D3DXVECTOR3(0.0f,0.0f,300.0f);

			vecBaseHArm			=	D3DXVECTOR3(0.0f,-190.0f,250.0f);
			vecFirstHArm		=	D3DXVECTOR3(0.0f,190.0f,250.0f);

			vecBaseVArm			=	D3DXVECTOR3(0.0f,250.0f,-250.0f);
			vecFirstVArm		=	D3DXVECTOR3(0.0f,250.0f,1075.0f);

			vecBaseSecondHArm	=	D3DXVECTOR3(0.0f,-190.0f,0.0f);
			vecSecondHArm		=	D3DXVECTOR3(0.0f,190.0f,0.0f);

			vecBaseSecondVArm	=	D3DXVECTOR3(0.0f,0.0f,-153.0f);
			vecSecondVArm		=	D3DXVECTOR3(0.0f,0.0f,625.0f);

			vecBaseThirdHArm	=	D3DXVECTOR3(0.0f,-100.0f,625.0f);
			vecThirdHArm		=	D3DXVECTOR3(0.0f,100.0f,625.0f);

			vecBaseThirdVArm	=	D3DXVECTOR3(0.0f,0.0f,0.0f);
			vecThirdVArm		=	D3DXVECTOR3(0.0f,0.0f,110.0f);

			vecBaseSearchTube	=	D3DXVECTOR3(0.0f,0.0f,0.0f);
			vecSearchTubeArm	=	theApp.m_Robot[nRobot].m_vJ6ToolTip * 0.7f;
			vecWaterNozzle		=	theApp.m_Robot[nRobot].m_vJ6ToolTip;
			
			
			
			theApp.m_Robot[nRobot].InitializeJointMatrices();

			theApp.m_Robot[nRobot].TransformVectorFromBase(&vecBase);
			theApp.m_Robot[nRobot].TransformVectorFromBase(&vecFirstColumn);
			Prim.Draw3DTube(m_pd3dDevice,vecBase,vecFirstColumn,YELLOW3D,337/2.0f,337/2.0f);

			theApp.m_Robot[nRobot].TransformVectorFromJ1(&vecBaseHArm);
			theApp.m_Robot[nRobot].TransformVectorFromJ1(&vecFirstHArm);
			Prim.Draw3DRod(m_pd3dDevice,vecBaseHArm,vecFirstHArm,YELLOW3D,250.0f,250.0f);

			theApp.m_Robot[nRobot].TransformVectorFromJ2(&vecBaseVArm);
			theApp.m_Robot[nRobot].TransformVectorFromJ2(&vecFirstVArm);
			Prim.Draw3DTube(m_pd3dDevice,vecBaseVArm,vecFirstVArm,YELLOW3D,125.0f,125.0f);

			theApp.m_Robot[nRobot].TransformVectorFromJ3(&vecBaseSecondHArm);
			theApp.m_Robot[nRobot].TransformVectorFromJ3(&vecSecondHArm);
			Prim.Draw3DRod(m_pd3dDevice,vecBaseSecondHArm,vecSecondHArm,YELLOW3D,125.0f,125.0f);

			theApp.m_Robot[nRobot].TransformVectorFromJ3(&vecBaseSecondVArm);
			theApp.m_Robot[nRobot].TransformVectorFromJ3(&vecSecondVArm);
			Prim.Draw3DTube(m_pd3dDevice,vecBaseSecondVArm,vecSecondVArm,YELLOW3D,70.0f,70.0f);

			theApp.m_Robot[nRobot].TransformVectorFromJ4(&vecBaseThirdHArm);
			theApp.m_Robot[nRobot].TransformVectorFromJ4(&vecThirdHArm);
			Prim.Draw3DRod(m_pd3dDevice,vecBaseThirdHArm,vecThirdHArm,YELLOW3D,50.0f,50.0f);

			theApp.m_Robot[nRobot].TransformVectorFromJ5(&vecBaseThirdVArm);
			theApp.m_Robot[nRobot].TransformVectorFromJ5(&vecThirdVArm);
			Prim.Draw3DTube(m_pd3dDevice,vecBaseThirdVArm,vecThirdVArm,GREEN3D,30.0f,30.0f);


			theApp.m_Robot[nRobot].TransformVectorFromJ6(&vecBaseSearchTube);
			theApp.m_Robot[nRobot].TransformVectorFromJ6(&vecSearchTubeArm);
			theApp.m_Robot[nRobot].TransformVectorFromJ6(&vecWaterNozzle);
			Prim.Draw3DTube(m_pd3dDevice,vecBaseSearchTube,vecSearchTubeArm,RED3D,20.0f,20.0f);
			Prim.Draw3DTube(m_pd3dDevice,vecSearchTubeArm,vecWaterNozzle,BLUE3D,60.0f,10.0f);
		}
	}


}
void C3DViewPage::DrawScanViaDangerPlanePoints()
{
	CCoord CpSurface;
	D3DXMATRIXA16 matCoord;
	D3DXMATRIXA16 matTranslation;


	if(PROFILE->m_ScanLine == NULL) return;

	m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );

	for(int nLine=0;nLine < PROFILE->m_nScanLineL; nLine++) {

		for(int nEnd=0;nEnd<2;nEnd++) {
			if(nEnd == 0) {
				CpSurface = PROFILE->m_ScanLine[nLine].m_pCp[0];
			} else {
				CpSurface = PROFILE->m_ScanLine[nLine].m_pCp[ PROFILE->m_ScanLine[nLine].m_nCoordL-1 ];
			}

			if(CpSurface.nType == 1) {
				D3DXMatrixTranslation(&matTranslation,CpSurface.Side0.pt.x,CpSurface.Side0.pt.y,CpSurface.Side0.pt.z);
				D3DXMatrixMultiply(&matCoord,&matTranslation,&m_matWorld);
				m_pd3dDevice->SetTransform( D3DTS_WORLD, &matCoord );
				m_pSphere->DrawSubset(0);
			}
		}
	}

}


void C3DViewPage::DrawTrackerElements()
{
	C3DPrimitive Prim;
	D3DXMATRIXA16 matCoord;
	D3DXMATRIXA16 matTranslation;
	D3DXVECTOR3 vPt,vCenter,vNorm;
	D3DXVECTOR3 vNode[3];
	int nIndex,nElement,nColor,ii;
	float fRadius;

	if(theApp.m_LastSettings.nDisplay3DTracker == TRUE) {

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );
		

		for(ii=0;ii<PROFILE->m_Edge.GetSize();ii++) {
			PROFILE->m_Edge.GetPt(ii, 0, &vNode[0]);
			PROFILE->m_Edge.GetPt(ii, 1, &vNode[1]);
			theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vNode[0], &vNode[0]);
			theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vNode[1], &vNode[1]);
			Prim.Draw3DLine(m_pd3dDevice,vNode[0],vNode[1],BLUE3D);
		}


		for(nElement = 0;nElement < theApp.m_FaroWorkerThread.m_nElementL ; nElement++) {

			if(nElement == theApp.m_nElementIndex) {
				nColor = RED3D;
			} else {
				nColor = BLUE3D;
			}
			switch(theApp.m_FaroWorkerThread.m_Element[nElement].m_Type) {
			case VA_LINE:
			case VA_ANGLE:
				theApp.m_FaroWorkerThread.m_Element[nElement].GetPt(0,&vNode[0]);
				theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vNode[0], &vNode[0]);
				for(ii=1; ii<theApp.m_FaroWorkerThread.m_Element[nElement].GetSize(); ii++) {
					theApp.m_FaroWorkerThread.m_Element[nElement].GetPt(ii,&vNode[1]);
					theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vNode[1], &vNode[1]);
					Prim.Draw3DLine(m_pd3dDevice,vNode[0],vNode[1],nColor);
					vNode[0] = vNode[1];
				}
				break;
			case VA_ARC:
				theApp.m_FaroWorkerThread.m_Element[nElement].CalculateCircumCenter(&fRadius, &vCenter,NULL,NULL,&vNorm);
				theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vCenter, &vCenter);
				theApp.m_FaroWorkerThread.RotateToFrameNorm(&vNorm, &vNorm);
//				Prim.Draw3DCircle(m_pd3dDevice,vCenter,vNorm,fRadius,nColor);

				theApp.m_FaroWorkerThread.m_Element[nElement].GetPt(0,&vNode[0]);
				theApp.m_FaroWorkerThread.m_Element[nElement].GetPt(2,&vNode[1]);
				theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vNode[0], &vNode[0]);
				theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vNode[1], &vNode[1]);
				Prim.Draw3DFan(m_pd3dDevice,vCenter,vNode[0],vNode[1],nColor);
				break;
			}
		}

		for(nIndex = 0;nIndex<PROFILE->m_vCloud.GetSize();nIndex++) {
			PROFILE->m_vCloud.GetPt(nIndex, &vPt);
			theApp.m_FaroWorkerThread.FrameCoordinateFromWorld(&vPt, &vPt);

			D3DXMatrixTranslation(&matTranslation,vPt.x,vPt.y,vPt.z);
			D3DXMatrixMultiply(&matCoord,&matTranslation,&m_matWorld);
			m_pd3dDevice->SetTransform( D3DTS_WORLD, &matCoord );
			m_pSphere->DrawSubset(0);

			if(theApp.m_nListOrderArray[nIndex]>0) {
				D3DXMatrixTranslation(&matTranslation,vPt.x,vPt.y,vPt.z+70.0f);
				D3DXMatrixMultiply(&matCoord,&matTranslation,&m_matWorld);
				m_pd3dDevice->SetTransform( D3DTS_WORLD, &matCoord );
				m_pCone->DrawSubset(0);
			}

		}

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );

	}

}




void C3DViewPage::InitializeEyeAndLookAtPt()
{
	m_vEyePt = theApp.m_LastSettings.vecEyePt;
	m_vLookatPt = theApp.m_LastSettings.vecLookAtPt;
	if(D3DXVec3Length(&(m_vEyePt-m_vLookatPt)) <= 1.0f) {
		MessageBox(L"Your eye pt and your look at pt are too close together",L"Warning",MB_ICONWARNING);
	}

}

void C3DViewPage::OnButtonDisplayInside() 
{
	theApp.m_LastSettings.nDisplay3DInsideLines>0 ? theApp.m_LastSettings.nDisplay3DInsideLines=0 : theApp.m_LastSettings.nDisplay3DInsideLines=1;
	if(theApp.m_LastSettings.nDisplay3DInsideLines != 0) InitializeOutsideInsideLines();
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButtonDisplayOutside() 
{
	theApp.m_LastSettings.nDisplay3DOutsideLines>0 ? theApp.m_LastSettings.nDisplay3DOutsideLines=0 : theApp.m_LastSettings.nDisplay3DOutsideLines=1;
	if(theApp.m_LastSettings.nDisplay3DOutsideLines != 0) InitializeOutsideInsideLines();
	SetToolBarCheckedState();
	Render3D();
}

void C3DViewPage::OnButtonDisplayWorksheetProfiles() 
{
	theApp.m_LastSettings.nDisplay3DWorksheet>0 ? theApp.m_LastSettings.nDisplay3DWorksheet=0 : theApp.m_LastSettings.nDisplay3DWorksheet=1;
	SetToolBarCheckedState();
	Render3D();
}

>>>>>>> bc6110d859e6ee8424f37efbb7a0adbf4f06ea81
