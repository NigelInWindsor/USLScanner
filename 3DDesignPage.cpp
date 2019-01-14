// 3DDesignPage.cpp : implementation file
//

#include "stdafx.h"
#include "uslscanner.h"
#include "3DDesignPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3DDesignPage property page

IMPLEMENT_DYNCREATE(C3DDesignPage, CPropertyPage)

D3DXVECTOR3 vPickRayDir;
D3DXVECTOR3 vPickRayOrig;
D3DXMATRIXA16    m_matObj2;


C3DDesignPage::C3DDesignPage() : CPropertyPage(C3DDesignPage::IDD)
{
	//{{AFX_DATA_INIT(C3DDesignPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_bWireframe = false;
	m_bLBDown = false;
	m_bRBDown = false;
	m_bMBDown = false;
	m_bDeviceObjectsRestored = false;
	ZeroMemory(&m_PrimFrame,sizeof m_PrimFrame);
	m_n3DMoveEyeComponent = 0;
	m_pFont = NULL;
    m_pD3DXSprite = NULL;
	m_pMesh3DText = NULL;



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

	m_p3DObject = NULL;

}

C3DDesignPage::~C3DDesignPage()
{
	SAFE_DELETE_ARRAY( m_p3DObject );
    SAFE_RELEASE( m_pFont );
    SAFE_RELEASE( m_pD3DXSprite );
    SAFE_RELEASE( m_pMesh3DText );

}

void C3DDesignPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(C3DDesignPage)
	DDX_Control(pDX, IDC_STATIC_VIEW, m_staticView);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(C3DDesignPage, CPropertyPage)
	//{{AFX_MSG_MAP(C3DDesignPage)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_COMMAND(ID_BUTTON_RECTANGLE, OnButtonRectangle)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C3DDesignPage message handlers
int C3DDesignPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_wndDesignToolBar.CreateEx(this,
		TBSTYLE_FLAT | TBSTYLE_WRAPABLE  | CBRS_GRIPPER |CBRS_SIZE_DYNAMIC|CBRS_ALIGN_LEFT,
		WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS  | CBRS_FLYBY ,
		CRect(0,2,0,0)) ||	!m_wndDesignToolBar.LoadToolBar(IDR_3D_DESIGN_TOOLBAR)) {
			TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}


	if (!m_wndRebar.Create(this,RBS_BANDBORDERS,CBRS_ALIGN_TOP)) {
		TRACE0("Failed to create Profile View ReBar\n");
		return -1;
	}

	CRect rect;
	GetClientRect(rect);
	m_wndRebar.SetWindowPos(&wndTop   , 0,0,250,30,SWP_SHOWWINDOW);


	m_wndRebar.AddBar(&m_wndDesignToolBar);
	
	return 0;
}

BOOL C3DDesignPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_StaticPosView.Initialize(this, m_staticView, TRUE, TRUE, TRUE, TRUE);

	m_StaticPosView.SetNewPos();

	if(SUCCEEDED( InitD3D( m_staticView.m_hWnd ))) {


	}

	
	UpdateAllControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void C3DDesignPage::UpdateAllControls()
{
	if(GetSafeHwnd() == NULL) return;
}


BOOL C3DDesignPage::OnSetActive() 
{
	
//	CreateColumns();
//	FillList();
	UpdateAllControls();

	SetTimer(1,200,NULL);

	return CPropertyPage::OnSetActive();
}

BOOL C3DDesignPage::OnKillActive() 
{
	

	KillTimer(1);
	
	
	return CPropertyPage::OnKillActive();
}

void C3DDesignPage::OnSize(UINT nType, int cx, int cy) 
{
	CPropertyPage::OnSize(nType, cx, cy);
	if(GetSafeHwnd() == NULL) return;
	
	m_StaticPosView.SetNewPos();


	HandlePossibleSizeChange();

	Invalidate(FALSE);
	
}

HRESULT C3DDesignPage::HandlePossibleSizeChange()
{
    HRESULT hr = S_OK;
	CRect rr;

	m_staticView.GetWindowRect(rr);

	if(m_rrView != rr) {

		m_rrView = rr;

        m_d3dpp.BackBufferWidth  = m_rrView.Width();
        m_d3dpp.BackBufferHeight = m_rrView.Height();

	    m_WorldBall.SetWindow( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 1.0f );
	    m_WorldBall.SetRadius( 1000.0f );
	    m_CompBall.SetWindow( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 1.0f );
	    m_CompBall.SetRadius( 1000.0f );
   
        if( m_pd3dDevice != NULL )
        {
		    m_pFont->OnLostDevice();
		    m_pD3DXSprite->OnLostDevice();
			SAFE_DELETE_ARRAY( m_p3DObject );

            if( FAILED( hr = m_pd3dDevice->Reset( &m_d3dpp ) ) )
            {
				MessageBox(_T("Failed to reset graphics"),_T("Error"),MB_ICONERROR);
            }
		    m_pFont->OnResetDevice();
		    m_pD3DXSprite->OnResetDevice();
		    SAFE_RELEASE( m_pMesh3DText );
			CreateD3DXTextMesh( &m_pMesh3DText, m_strFont, m_nFontSize, FALSE, FALSE );
        }
    }
    return hr;
}

HRESULT C3DDesignPage::CreateD3DXTextMesh( LPD3DXMESH* ppMesh, 
                                               TCHAR* pstrFont, DWORD dwSize,
                                               BOOL bBold, BOOL bItalic )
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

    hr = D3DXCreateText(m_pd3dDevice, hdc, _T("This is calling D3DXCreateText"), 
                        0.001f, 0.001f, &pMeshNew, NULL, NULL);

    SelectObject(hdc, hFontOld);
    DeleteObject( hFont );
    DeleteDC( hdc );

    if( SUCCEEDED( hr ) )
        *ppMesh = pMeshNew;

    return hr;
}



void C3DDesignPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	Render3D();
}

HRESULT C3DDesignPage::InitD3D(HWND hWnd)
{
	CRect rr;

	m_staticView.GetWindowRect(&rr);

    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    ZeroMemory( &m_d3dpp, sizeof(m_d3dpp) );
    m_d3dpp.Windowed = TRUE;
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	m_d3dpp.BackBufferWidth = rr.Width();
	m_d3dpp.BackBufferHeight = rr.Height();

    // Create the D3DDevice
    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &m_d3dpp, &m_pd3dDevice ) ) )
    {
        return E_FAIL;
    }


    // Turn off culling, so we see the front and back of the triangle
    m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );

    // Turn off D3D lighting, since we are providing our own vertex colors
    m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

    m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

    m_WorldBall.SetWindow( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 1.0f );
    m_WorldBall.SetRadius( 1000.0f );
    m_CompBall.SetWindow( m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, 1.0f );
    m_CompBall.SetRadius( 1000.0f );

    HRESULT hr;

    CDC* pDC = GetDC();
    int nLogPixelsY =pDC->GetDeviceCaps(LOGPIXELSY);
    ReleaseDC( pDC );


    int nHeight = -18 * nLogPixelsY / 72;
    if( FAILED( hr = D3DXCreateFont( m_pd3dDevice, nHeight, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, 
                         OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                         TEXT("Arial"), &m_pFont ) ) )
        return hr;

    
    lstrcpy( m_strFont, TEXT("Arial") );
	m_nFontSize = 12;

	SAFE_RELEASE( m_pMesh3DText );
	CreateD3DXTextMesh( &m_pMesh3DText, m_strFont, m_nFontSize, FALSE, FALSE );
    D3DXComputeNormals( m_pMesh3DText, NULL );

    if( FAILED( hr = D3DXCreateSprite( m_pd3dDevice, &m_pD3DXSprite ) ) )
        return hr;



	return S_OK;

}



VOID C3DDesignPage::SetupLights()
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



void C3DDesignPage::Render3D()
{
    D3DMATERIAL9 mtrl;
	CString Buff;
	CPoint pt;

	if(m_pd3dDevice==NULL) return;

	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(200,200,255), 1.0f, 0 );


    if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )  {
		if( m_bWireframe )
			m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		else
			m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

		m_pd3dDevice->SetTexture(0, NULL );

		FrameMove();

		m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		if(m_bRBDown || m_bLBDown || m_bMBDown) {
			DrawLookAtPoint();
		}

		SetupLights();

 //  m_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE,   TRUE );
 //  m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,       D3DBLEND_SRCALPHA );
 //  m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,      D3DBLEND_INVSRCALPHA );

		m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matWorld );
		if(m_p3DObject) m_p3DObject->Render();

		WorldToScreen(D3DXVECTOR3(1000.0f, 0.0f, 1000.0f), &pt);
//		ComponentMove();
		Pick();


		D3DXVECTOR3 vecLength;
		D3DXVec3Subtract(&vecLength,&m_vEyePt,&m_vLookatPt);
		Buff.Format(_T("Distance to look at point %.01f mm"),D3DXVec3Length(&vecLength));

        if( m_pMesh3DText != NULL )
        {
			float fScale = 200.0f;

		    D3DXMATRIX mScale,matTrans,matRotate;
			D3DXMatrixTranslation( &matTrans,0.0f,-10.0f,0.0f );
			D3DXMatrixRotationYawPitchRoll( &matRotate,0.0f,PIf/2.0f, 0.0f );
		    D3DXMatrixScaling( &mScale, fScale,fScale,fScale);

			m_matObj2 = matRotate * mScale;
			m_matObj2 *= m_matWorld;
		    m_matObj2 *= matTrans;

            ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
            mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
            mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
            mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
            mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
//			m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
            m_pd3dDevice->SetMaterial( &mtrl );
            m_pd3dDevice->SetTransform( D3DTS_WORLD, &m_matObj2 );
            m_pMesh3DText->DrawSubset(0);
        }

		CRect rc;
        SetRect( &rc, pt.x, pt.y, 0, 0 );
		Buff.Format(_T("A"));
        m_pFont->DrawText( NULL, Buff, -1, &rc, DT_NOCLIP, D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ));

        SetRect( &rc, 20, 40, 0, 0 );
		Buff.Format(_T("Dir : %.01f % .01f %.01f"),vPickRayDir.x,vPickRayDir.y,vPickRayDir.z);
        m_pFont->DrawText( NULL, Buff, -1, &rc, DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ));
        SetRect( &rc, 20, 60, 0, 0 );
		Buff.Format(_T("Dir : %.01f % .01f %.01f"),vPickRayOrig.x,vPickRayOrig.y,vPickRayOrig.z);
        m_pFont->DrawText( NULL, Buff, -1, &rc, DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ));

        m_pd3dDevice->EndScene();
    }

    m_pd3dDevice->Present( NULL, NULL, NULL, NULL );

}


void C3DDesignPage::FrameMove()
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

void C3DDesignPage::ComponentMove()
{

    D3DXMatrixIdentity( &m_matWorld);
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_WorldBall.GetTranslationMatrix() );
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_CompBall.GetTranslationMatrix() );
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_WorldBall.GetRotationMatrix() );
	D3DXMatrixMultiply( &m_matWorld, &m_matWorld, m_CompBall.GetRotationMatrix() );

}

BOOL C3DDesignPage::OnToolTipNotify(UINT id, NMHDR *pNMHDR,
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
	case ID_BUTTON_RECTANGLE:  strTipText.LoadString(IDS_Rectangle);
		break;
	}

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}


void C3DDesignPage::OnButtonRectangle() 
{
	D3DXCOLOR rgbOrange(200,100,200,0);

	D3DXVECTOR3 vecRectZero = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXVECTOR3 vecRectSize = D3DXVECTOR3(1000.0f,1000.0f,0.0f);

	if(m_p3DObject == NULL) {
		m_p3DObject = new C3DObject(m_pd3dDevice,1);
	}

	m_p3DObject->Rectangle(vecRectSize,rgbOrange,&vecRectZero);
	Invalidate(FALSE);
	
}


#define	BLUE3D	0xff0000ff
#define	GREEN3D	0xff00ff00
#define	RED3D	0xffff0000

void C3DDesignPage::DrawLookAtPoint()
{
	D3DXMATRIXA16 matWorld;

	m_pd3dDevice->SetTexture(0, NULL );
	D3DXMatrixIdentity(&matWorld);

	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	Draw3DLine(-1000.0f,0.0f,0.0f,1000.0f,0.0f,0.0f,RED3D);
	Draw3DLine(0.0f,-1000.0f,0.0f,0.0f,1000.0f,0.0f,GREEN3D);
	Draw3DLine(0.0f,0.0f,-1000.0f,0.0f,0.0f,1000.0f,BLUE3D);
	Draw3DCircle(D3DXVECTOR3(0.0,0.0,0.0),D3DXVECTOR3(1.0,0.0,0.0),200.0,RED3D);
	Draw3DCircle(D3DXVECTOR3(0.0,0.0,0.0),D3DXVECTOR3(0.0,1.0,0.0),200.0,GREEN3D);
	Draw3DCircle(D3DXVECTOR3(0.0,0.0,0.0),D3DXVECTOR3(0.0,0.0,1.0),200.0,BLUE3D);

	if(m_bRBDown) {
		Draw3DCone(D3DXVECTOR3(1000.0,0.0,0.0),D3DXVECTOR3(1.0,0.0,0.0),20.0,100.0,RED3D);
//		Draw3DCone(D3DXVECTOR3(-1000.0,0.0,0.0),D3DXVECTOR3(1.0,0.0,0.0),20.0,-100.0,0xff0000ff);

		Draw3DCone(D3DXVECTOR3(0.0,1000.0,0.0),D3DXVECTOR3(0.0,1.0,0.0),20.0,-100.0,GREEN3D);
//		Draw3DCone(D3DXVECTOR3(0.0,-1000.0,0.0),D3DXVECTOR3(0.0,1.0,0.0),20.0,100.0,0xff00ff00);
	}

	if(m_bMBDown) {
		Draw3DCone(D3DXVECTOR3(0.0,0.0,1000.0),D3DXVECTOR3(0.0,0.0,1.0),20.0,100.0,BLUE3D);
//		Draw3DCone(D3DXVECTOR3(0.0,0.0,-1000.0),D3DXVECTOR3(0.0,0.0,1.0),20.0,-100.0,0xffff0000);
	}

}

void C3DDesignPage::Draw3DCone(D3DXVECTOR3 Centre, D3DXVECTOR3 Normal, float fRadius,float fHeight,int nColor)
{
	D3DXMATRIXA16 matWorld,matNormal,matTranslation;
	D3DXVECTOR3 vecP;
	CUSTOMVERTEX Vertex[104];
	int nn;
	float fAngle;


	D3DXMatrixTranslation(&matTranslation,Centre.x,Centre.y,Centre.z);
	D3DXMatrixRotationYawPitchRoll(&matNormal,Normal.x * PIf / 2.0f,Normal.y * PIf / 2.0f,(1.0f - Normal.z) * PIf / 2.0f);
	D3DXMatrixMultiply(&matNormal,&matNormal,&matTranslation);

	vecP.x=0.0;
	vecP.y=0.0;
	vecP.z=fHeight;

	D3DXVec3TransformCoord(&vecP,&vecP,&matNormal);

	Vertex[nn=0].vecPos.x = vecP.x;
	Vertex[nn].vecPos.y = vecP.y;
	Vertex[nn].vecPos.z = vecP.z;
	Vertex[nn++].dwColor = nColor;

	for(fAngle = 0.0f;fAngle >= -2.0f*PIf ; fAngle -= (2.0f*PIf/100.0f), nn++) {
		D3DXMatrixRotationZ(&matWorld,fAngle);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matNormal);

		vecP.x = fRadius;
		vecP.y = 0.0f;
		vecP.z = 0.0f;

		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);

		Vertex[nn].vecPos.x = vecP.x;
		Vertex[nn].vecPos.y = vecP.y;
		Vertex[nn].vecPos.z = vecP.z;
		Vertex[nn].dwColor = nColor;
	}

	Vertex[nn++] = Vertex[0];

	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
    m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, nn-2, Vertex, sizeof(CUSTOMVERTEX) );


}

void C3DDesignPage::Draw3DCircle(D3DXVECTOR3 Centre, D3DXVECTOR3 Normal, float fRadius,int nColor)
{
	D3DXMATRIXA16 matWorld,matNormal,matTranslation;
	D3DXVECTOR3 vecP;
	CUSTOMVERTEX Vertex[102];
	int nn;
	float fAngle;

	D3DXMatrixTranslation(&matTranslation,Centre.x,Centre.y,Centre.z);
	D3DXMatrixRotationYawPitchRoll(&matNormal,Normal.x * PIf / 2.0f,Normal.y * PIf / 2.0f,(1.0f - Normal.z) * PIf / 2.0f);
	D3DXMatrixMultiply(&matNormal,&matNormal,&matTranslation);

	for(fAngle = 0.0f,nn=0;fAngle <= 2.0f*PIf ; fAngle += (2.0f*PIf/100.0f), nn++) {
		D3DXMatrixRotationZ(&matWorld,fAngle);
		D3DXMatrixMultiply(&matWorld,&matWorld,&matNormal);

		vecP.x = fRadius;
		vecP.y = 0.0f;
		vecP.z = 0.0f;

		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);

		Vertex[nn].vecPos.x = vecP.x;
		Vertex[nn].vecPos.y = vecP.y;
		Vertex[nn].vecPos.z = vecP.z;
		Vertex[nn].dwColor = nColor;
		Vertex[nn].vecNorm.x = 0.0f;
		Vertex[nn].vecNorm.y = 1.0f;
		Vertex[nn].vecNorm.z = 0.0f;
	}

	Vertex[nn++] = Vertex[0];

	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
    m_pd3dDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, nn-1, Vertex, sizeof(CUSTOMVERTEX) );


}


void C3DDesignPage::Draw3DLine(float fX0,float fY0,float fZ0,float fX1,float fY1,float fZ1,int nColor)
{

	CUSTOMVERTEX Vertex[2];


	Vertex[0].vecPos.x=fX0;
	Vertex[0].vecPos.y=fY0;
	Vertex[0].vecPos.z=fZ0;
	Vertex[0].dwColor=nColor;
	Vertex[1].vecPos.x=fX1;
	Vertex[1].vecPos.y=fY1;
	Vertex[1].vecPos.z=fZ1;
	Vertex[1].dwColor=nColor;
	Vertex[0].vecNorm.x = 0.0f;
	Vertex[0].vecNorm.y = 1.0f;
	Vertex[0].vecNorm.z = 0.0f;
	Vertex[1].vecNorm.x = 0.0f;
	Vertex[1].vecNorm.y = 1.0f;
	Vertex[1].vecNorm.z = 0.0f;

	m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
    m_pd3dDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, Vertex, sizeof(CUSTOMVERTEX) );


}

void C3DDesignPage::OnLButtonDown(UINT nFlags, CPoint point) 
{


	if(m_n3DMoveEyeComponent==0) {
		m_WorldBall.m_vDown = m_WorldBall.ScreenToVector( point.x, point.y );
        m_WorldBall.m_qDown = m_WorldBall.m_qNow;
		m_WorldBall.m_bDrag = TRUE;
	} else {
		m_CompBall.m_vDown = m_CompBall.ScreenToVector( point.x, point.y );
        m_CompBall.m_qDown = m_CompBall.m_qNow;
		m_CompBall.m_bDrag = TRUE;
	}
	m_bLBDown = true;

	Invalidate(FALSE);
	
	CPropertyPage::OnLButtonDown(nFlags, point);
}

void C3DDesignPage::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLBDown = false;
	
	Invalidate(FALSE);

	CPropertyPage::OnLButtonUp(nFlags, point);
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

void C3DDesignPage::OnMouseMove(UINT nFlags, CPoint point) 
{
	CString	Buff;
	float fXRotate = 0.0f;
	float fYRotate = 0.0f;
	float fZRotate = 0.0f;
	float fXTranslate=0.0f;
	float fYTranslate=0.0f;
	float fZTranslate=0.0f;
	int	nFrameId=0,nCompId=-1;
	CRect rr;
	CD3DArcBall_10 *pBall;
	D3DXVECTOR3 vec;
	D3DXQUATERNION  q;
	float fAngle;

	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	if(m_n3DMoveEyeComponent==0) {
		pBall = &m_WorldBall;
	} else {
		pBall = &m_CompBall;
	}
	if( m_bLBDown == TRUE ) {
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
		if( m_bRBDown || m_bMBDown)  {
			// Normalize based on size of window and bounding sphere radius
			FLOAT fDeltaX = ( pBall->m_iCurMouseX-point.x ) * pBall->m_fRadiusTranslation / pBall->m_iWidth;
			FLOAT fDeltaY = ( pBall->m_iCurMouseY-point.y ) * pBall->m_fRadiusTranslation / pBall->m_iHeight;
			if( m_bRBDown )
			{
				D3DXMatrixTranslation( &pBall->m_matTranslationDelta, -2*fDeltaX, 2*fDeltaY, 0.0f );
				D3DXMatrixMultiply( &pBall->m_matTranslation, &pBall->m_matTranslation, &pBall->m_matTranslationDelta );
			} else  {// wParam & MK_MBUTTON
				D3DXMatrixTranslation( &pBall->m_matTranslationDelta, 0.0f, 0.0f, 5*fDeltaY );
				D3DXMatrixMultiply( &pBall->m_matTranslation, &pBall->m_matTranslation, &pBall->m_matTranslationDelta );
			}
			// Store mouse coordinate
			pBall->m_iCurMouseX=point.x;
			pBall->m_iCurMouseY=point.y;
		}


		D3DXQuaternionToAxisAngle(D3DXQuaternionNormalize(&q,&m_CompBall.m_qNow),&vec,&fAngle);


	}
	Invalidate(FALSE);

	m_ptDown = point;


	
	CPropertyPage::OnMouseMove(nFlags, point);
}

void C3DDesignPage::WorldToScreen(D3DXVECTOR3 vec,CPoint *pt)
{
	CRect rr;
	m_staticView.GetWindowRect(&rr);
	ScreenToClient(&rr);

	D3DXMATRIXA16 matProj;
	D3DXMATRIXA16 matView, matWorld;
	D3DXVECTOR3 vecScreen;
	D3DVIEWPORT9 viewScreen;

	m_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj );
	m_pd3dDevice->GetTransform( D3DTS_VIEW, &matView );
	m_pd3dDevice->GetTransform( D3DTS_WORLD, &matWorld );

	viewScreen.X = 0;
	viewScreen.Y = 0;
	viewScreen.Width = rr.Width();
	viewScreen.Height = rr.Height();
	viewScreen.MinZ = 0.0f;
	viewScreen.MaxZ = 1.0f;

	D3DXVec3Project(&vecScreen, &vec, &viewScreen, &matProj, &matView, &matWorld);

	pt->x = (int)vecScreen.x;
	pt->y = (int)vecScreen.y;
}

bool m_bUseD3DX = true;
DWORD	m_dwNumIntersections;

HRESULT C3DDesignPage::Pick()
{

    m_dwNumIntersections = 0L;

    // Get the pick ray from the mouse position
 //   if( GetCapture() )
//    {
        D3DXMATRIXA16 matProj;
        m_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj );

        POINT ptCursor;
        GetCursorPos( &ptCursor );
        ScreenToClient( &ptCursor );

        // Compute the vector of the pick ray in screen space
        D3DXVECTOR3 v;
        v.x =  ( ( ( 2.0f * ptCursor.x ) / m_d3dsdBackBuffer.Width  ) - 1 ) / matProj._11;
        v.y = -( ( ( 2.0f * ptCursor.y ) / m_d3dsdBackBuffer.Height ) - 1 ) / matProj._22;
        v.z =  1.0f;

        // Get the inverse of the composite view and world matrix
        D3DXMATRIXA16 matView, matWorld, m;
        m_pd3dDevice->GetTransform( D3DTS_VIEW, &matView );
        m_pd3dDevice->GetTransform( D3DTS_WORLD, &matWorld );
        
        m = matWorld * matView;
        D3DXMatrixInverse( &m, NULL, &m );

        // Transform the screen space pick ray into 3D space
        vPickRayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
        vPickRayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
        vPickRayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
        vPickRayOrig.x = m._41;
        vPickRayOrig.y = m._42;
        vPickRayOrig.z = m._43;
//    }
/*
    // Get the picked triangle
    if( GetCapture() )
    {
        LPD3DXBASEMESH          pMesh = m_pObject->GetLocalMesh();
        LPDIRECT3DVERTEXBUFFER9 pVB;
        LPDIRECT3DINDEXBUFFER9  pIB;

        pMesh->GetVertexBuffer( &pVB );
        pMesh->GetIndexBuffer( &pIB );

        WORD*      pIndices;
        D3DVERTEX*    pVertices;

        pIB->Lock( 0, 0, (void**)&pIndices, 0 );
        pVB->Lock( 0, 0, (void**)&pVertices, 0 );

        if( m_bUseD3DX )
        {
            // When calling D3DXIntersect, one can get just the closest intersection and not
            // need to work with a D3DXBUFFER.  Or, to get all intersections between the ray and 
            // the mesh, one can use a D3DXBUFFER to receive all intersections.  We show both
            // methods.
            if( m_bClosestOnly )
            {
                // Collect only the closest intersection
                BOOL bHit;
                DWORD dwFace;
                FLOAT fBary1, fBary2, fDist;
                D3DXIntersect(pMesh, &vPickRayOrig, &vPickRayDir, &bHit, &dwFace, &fBary1, &fBary2, &fDist, 
                    NULL, NULL);
                if( bHit )
                {
                    m_dwNumIntersections = 1;
                    m_IntersectionArray[0].dwFace = dwFace;
                    m_IntersectionArray[0].fBary1 = fBary1;
                    m_IntersectionArray[0].fBary2 = fBary2;
                    m_IntersectionArray[0].fDist = fDist;
                }
                else
                {
                    m_dwNumIntersections = 0;
                }
            }
            else 
            {
                // Collect all intersections
                BOOL bHit;
                LPD3DXBUFFER pBuffer = NULL;
                D3DXINTERSECTINFO* pIntersectInfoArray;
                if( FAILED( hr = D3DXIntersect(pMesh, &vPickRayOrig, &vPickRayDir, &bHit, NULL, NULL, NULL, NULL, 
                    &pBuffer, &m_dwNumIntersections) ) )
                {
                    return hr;
                }
                if( m_dwNumIntersections > 0 )
                {
                    pIntersectInfoArray = (D3DXINTERSECTINFO*)pBuffer->GetBufferPointer();
                    if( m_dwNumIntersections > MAX_INTERSECTIONS )
                        m_dwNumIntersections = MAX_INTERSECTIONS;
                    for( DWORD iIntersection = 0; iIntersection < m_dwNumIntersections; iIntersection++ )
                    {
                        m_IntersectionArray[iIntersection].dwFace = pIntersectInfoArray[iIntersection].FaceIndex;
                        m_IntersectionArray[iIntersection].fBary1 = pIntersectInfoArray[iIntersection].U;
                        m_IntersectionArray[iIntersection].fBary2 = pIntersectInfoArray[iIntersection].V;
                        m_IntersectionArray[iIntersection].fDist = pIntersectInfoArray[iIntersection].Dist;
                    }
                }
                SAFE_RELEASE( pBuffer );
            }

        }
        else
        {
            // Not using D3DX
            DWORD dwNumFaces = m_pObject->GetLocalMesh()->GetNumFaces();
            FLOAT fBary1, fBary2;
            FLOAT fDist;
            for( DWORD i=0; i<dwNumFaces; i++ )
            {
                D3DXVECTOR3 v0 = pVertices[pIndices[3*i+0]].p;
                D3DXVECTOR3 v1 = pVertices[pIndices[3*i+1]].p;
                D3DXVECTOR3 v2 = pVertices[pIndices[3*i+2]].p;

                // Check if the pick ray passes through this point
                if( IntersectTriangle( vPickRayOrig, vPickRayDir, v0, v1, v2,
                                       &fDist, &fBary1, &fBary2 ) )
                {
                    if( !m_bClosestOnly || m_dwNumIntersections == 0 || fDist < m_IntersectionArray[0].fDist )
                    {
                        if( m_bClosestOnly )
                            m_dwNumIntersections = 0;
                        m_IntersectionArray[m_dwNumIntersections].dwFace = i;
                        m_IntersectionArray[m_dwNumIntersections].fBary1 = fBary1;
                        m_IntersectionArray[m_dwNumIntersections].fBary2 = fBary2;
                        m_IntersectionArray[m_dwNumIntersections].fDist = fDist;
                        m_dwNumIntersections++;
                        if( m_dwNumIntersections == MAX_INTERSECTIONS )
                            break;
                    }
                }
            }
        }

        // Now, for each intersection, add a triangle to m_pVB and compute texture coordinates
        if( m_dwNumIntersections > 0 )
        {
            D3DVERTEX* v;
            D3DVERTEX* vThisTri;
            WORD* iThisTri;
            D3DVERTEX  v1, v2, v3;
            INTERSECTION* pIntersection;

            m_pVB->Lock( 0, 0, (void**)&v, 0 );

            for( DWORD iIntersection = 0; iIntersection < m_dwNumIntersections; iIntersection++ )
            {
                pIntersection = &m_IntersectionArray[iIntersection];

                vThisTri = &v[iIntersection * 3];
                iThisTri = &pIndices[3*pIntersection->dwFace];
                // get vertices hit
                vThisTri[0] = pVertices[iThisTri[0]];
                vThisTri[1] = pVertices[iThisTri[1]];
                vThisTri[2] = pVertices[iThisTri[2]];

                // If all you want is the vertices hit, then you are done.  In this sample, we
                // want to show how to infer texture coordinates as well, using the BaryCentric
                // coordinates supplied by D3DXIntersect
                FLOAT dtu1 = vThisTri[1].tu - vThisTri[0].tu;
                FLOAT dtu2 = vThisTri[2].tu - vThisTri[0].tu;
                FLOAT dtv1 = vThisTri[1].tv - vThisTri[0].tv;
                FLOAT dtv2 = vThisTri[2].tv - vThisTri[0].tv;
                pIntersection->tu = vThisTri[0].tu + pIntersection->fBary1 * dtu1 + pIntersection->fBary2 * dtu2;
                pIntersection->tv = vThisTri[0].tv + pIntersection->fBary1 * dtv1 + pIntersection->fBary2 * dtv2;
            }
            m_pVB->Unlock();
        }

        pVB->Unlock();
        pIB->Unlock();

        pVB->Release();
        pIB->Release();

    }

*/
    return S_OK;
}

