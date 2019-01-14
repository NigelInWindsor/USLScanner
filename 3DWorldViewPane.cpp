#include "stdafx.h"
#include "USLScanner.h"
#include "3DWorldViewPane.h"

#include "../DirectXTK/Inc/GeometricPrimitive.h"
#include <DirectXCollision.h>


extern UINT	WorldViewCallBack(LPVOID pParam);

HINSTANCE                           g_hInst = nullptr;
HWND                                g_hWnd = nullptr;
D3D_DRIVER_TYPE                     g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL                   g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*                       g_pd3dDevice = nullptr;
ID3D11DeviceContext*                g_pImmediateContext = nullptr;
IDXGISwapChain*                     g_pSwapChain = nullptr;
ID3D11RenderTargetView*             g_pRenderTargetView = nullptr;
ID3D11Texture2D*                    g_pDepthStencil = nullptr;
ID3D11DepthStencilView*             g_pDepthStencilView = nullptr;

ID3D11ShaderResourceView*           g_pTexturePlainMetal = nullptr;
ID3D11ShaderResourceView*           g_pTextureRV2 = nullptr;
ID3D11ShaderResourceView*           g_pTextureGaugePlate = nullptr;
ID3D11InputLayout*                  g_pBatchInputLayout = nullptr;

ID3D11Debug *						g_d3dDebug = nullptr;

CModelViewerCamera					g_Camera;               // A model viewing camera

std::unique_ptr<CommonStates>                           g_States;
std::unique_ptr<BasicEffect>                            g_BatchEffect;
std::unique_ptr<EffectFactory>                          g_FXFactory;
std::unique_ptr<Model>                                  g_Model;
std::unique_ptr<PrimitiveBatch<VertexPositionColor>>    g_Batch;
std::unique_ptr<PrimitiveBatch<VertexPositionNormalColor>>    g_BatchComponent;
std::unique_ptr<SpriteBatch>                            g_Sprites;
std::unique_ptr<SpriteFont>                             g_Font;

std::unique_ptr<GeometricPrimitive>                     g_CoordSphere;
std::unique_ptr<GeometricPrimitive>                     g_Cylinder;
std::unique_ptr<GeometricPrimitive>                     g_VerticalLeg;
std::unique_ptr<GeometricPrimitive>                     g_LongBeam;
std::unique_ptr<GeometricPrimitive>                     g_ShortBeam;
std::unique_ptr<GeometricPrimitive>                     g_LongGRPFloorStringer;
std::unique_ptr<GeometricPrimitive>                     g_ShortGRPFloorStringer;
std::unique_ptr<GeometricPrimitive>                     g_FloorPanel;
std::unique_ptr<GeometricPrimitive>                     g_TwinTowerBlueZTower;
std::unique_ptr<GeometricPrimitive>                     g_TwinTowerRedZTower;
std::unique_ptr<GeometricPrimitive>                     g_TwinTowerYArm;
std::unique_ptr<GeometricPrimitive>						g_TopOfRedTower;
std::unique_ptr<GeometricPrimitive>						g_ScanLines[2][5000];
std::unique_ptr<GeometricPrimitive>						g_DangerPlane[2];
std::unique_ptr<GeometricPrimitive>						g_SlicePlane;
std::unique_ptr<GeometricPrimitive>						g_RefDisc;
std::unique_ptr<GeometricPrimitive>						g_RefPost;
std::unique_ptr<GeometricPrimitive>						g_RefSphere;
std::unique_ptr<GeometricPrimitive>						g_StepCloudSet;
size_t													g_CircleL;
std::unique_ptr<GeometricPrimitive>						g_Circle[10000];
size_t													g_LineL;
std::unique_ptr<GeometricPrimitive>						g_Line[10000];
size_t													g_EdgeCurveL;
std::unique_ptr<GeometricPrimitive>						g_EdgeCurve[10000];
std::unique_ptr<GeometricPrimitive>						g_CartesianPlane[3];
std::unique_ptr<GeometricPrimitive>						g_QuaternionTest;
std::unique_ptr<GeometricPrimitive>						g_SliceNorm;

int														g_nScanLinesL[2];

XMFLOAT3 g_VerticalLegSize;
XMFLOAT3 g_LongBeamSize;
XMFLOAT3 g_ShortBeamSize;
XMFLOAT3 g_TwinTowerXCarriageSize;
XMFLOAT3 g_BLueTowerSize;
XMFLOAT3 g_RedTowerSize;
XMFLOAT3 g_TwinTowerYArmSize;
XMFLOAT3 g_FloorPanelSize;

bool g_bRightHanded = true;


#ifdef DXTK_AUDIO
std::unique_ptr<DirectX::AudioEngine>                   g_audEngine;
std::unique_ptr<DirectX::WaveBank>                      g_waveBank;
std::unique_ptr<DirectX::SoundEffect>                   g_soundEffect;
std::unique_ptr<DirectX::SoundEffectInstance>           g_effect1;
std::unique_ptr<DirectX::SoundEffectInstance>           g_effect2;

uint32_t                                                g_audioEvent = 0;
float                                                   g_audioTimerAcc = 0.f;

HDEVNOTIFY                                              g_hNewAudio = nullptr;
#endif

XMMATRIX                            g_World;
XMMATRIX                            g_View;
XMMATRIX							g_Profile;
XMMATRIX                            g_Projection;

XMVECTOR g_Eye = XMVectorSet(0.0f, -2500.0f, 00.0f, 0.0f);
XMVECTOR g_At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
XMVECTOR g_Up = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
FLOAT g_fObjectRadius = 8000.0f;
XMVECTOR g_Colour;
XMVECTOR g_vSelectedPt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
XMVECTOR g_vSelectedNorm = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
int	g_nSelectedSurface = 0;
int	g_nSelectedUCurve = 0;
int	g_nSelectedVCurve = 0;


IMPLEMENT_DYNAMIC(C3DWorldViewPane, CDockablePane)

C3DWorldViewPane::C3DWorldViewPane()
{
	m_p3DToolsSheet = NULL;
}


C3DWorldViewPane::~C3DWorldViewPane()
{
	CleanUp3D();
}

BEGIN_MESSAGE_MAP(C3DWorldViewPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_BUTTON_WORLDVIEW_SETUP, &C3DWorldViewPane::OnButtonWorldviewSetup)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


int C3DWorldViewPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	DWORD dwViewStyle = WS_CHILD | WS_VISIBLE;
	dwViewStyle += 7;

	if (!m_staticView.Create(NULL, dwViewStyle, rectDummy, this, ID_STATIC_VIEW))
	{
		TRACE0("Failed to create Static View in profiles import \n");
		return -1;      // fail to create
	}

	m_wndToolBar[0].Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_WORLDVIEW_TOOLBAR);
	m_wndToolBar[0].LoadToolBar(IDR_WORLDVIEW_TOOLBAR, 0, 0, TRUE /* Is locked */);

	m_wndToolBar[0].SetPaneStyle(m_wndToolBar[0].GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar[0].SetPaneStyle(m_wndToolBar[0].GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar[0].SetOwner(this);

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar[0].SetRouteCommandsViaFrame(FALSE);

	CreateFont();

	AdjustLayout();

	return 0;
}


void C3DWorldViewPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd())
	{
		SuspendThread();
		CleanUp3D();
		AdjustLayout();

		if (cx > 30 && cy > 30) {
			m_bInvalidateFlag = true;
			Start3D();
			StartThread();
		}
	}
}

void C3DWorldViewPane::Restart()
{
	SuspendThread();
	CleanUp3D();
	AdjustLayout();
	if (GetSafeHwnd())
	{
		Start3D();
		StartThread();
	}
}

void C3DWorldViewPane::AdjustLayout()
{
	if (GetSafeHwnd() == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar[0].CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar[0].SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_staticView.SetWindowPos(NULL, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}

int C3DWorldViewPane::CreateFont()
{
	LOGFONT lf;
	::DeleteObject(m_Font.Detach());

	if (theApp.m_LastSettings.bUseAlernativeToSystemFont == false) {
		afxGlobalData.fontRegular.GetLogFont(&lf);

		NONCLIENTMETRICS info;
		info.cbSize = sizeof(info);

		afxGlobalData.GetNonClientMetrics(info);

		lf.lfHeight = info.lfMenuFont.lfHeight;
		lf.lfWeight = info.lfMenuFont.lfWeight;
		lf.lfItalic = info.lfMenuFont.lfItalic;
	}
	else {
		lf = theApp.m_LastSettings.lfMenus;
	}
	lf.lfHeight = MulDiv(lf.lfHeight, theApp.nPercentageTextSize[theApp.m_LastSettings.nTextPercentageSize = 3], 100);
	m_Font.CreateFontIndirect(&lf);

	return 0;
}

void C3DWorldViewPane::Start3D()
{
	HWND hWnd = m_staticView.m_hWnd;
	if (SUCCEEDED(InitD3D(hWnd))) {
		InitializeObjects();
	}
}


HRESULT C3DWorldViewPane::InitD3D(HWND hWnd)
{
	HRESULT hr = S_OK;
	CString FilePathName;

	if (g_pd3dDevice) return true;

	g_hWnd = hWnd;

	RECT rc;
	m_staticView.GetWindowRect(&rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	//	ID3D11Debug *d3dDebug = nullptr;
	if (SUCCEEDED(g_pd3dDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&g_d3dDebug)))
	{
		ID3D11InfoQueue *d3dInfoQueue = nullptr;
		if (SUCCEEDED(g_d3dDebug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&d3dInfoQueue)))
		{
#ifdef _DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif

			D3D11_MESSAGE_ID hide[] =
			{
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// Add more message IDs here as needed
			};

			D3D11_INFO_QUEUE_FILTER filter;
			memset(&filter, 0, sizeof(filter));
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
			d3dInfoQueue->Release();
		}
		//		d3dDebug->Release();
	}

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);

	// Create DirectXTK objects
	g_States.reset(new CommonStates(g_pd3dDevice));
	g_Sprites.reset(new SpriteBatch(g_pImmediateContext));
	g_FXFactory.reset(new EffectFactory(g_pd3dDevice));
	g_Batch.reset(new PrimitiveBatch<VertexPositionColor>(g_pImmediateContext));
	g_BatchComponent.reset(new PrimitiveBatch<VertexPositionNormalColor>(g_pImmediateContext));

	g_BatchEffect.reset(new BasicEffect(g_pd3dDevice));
	g_BatchEffect->SetVertexColorEnabled(true);



	{
		void const* shaderByteCode;
		size_t byteCodeLength;

		g_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

		hr = g_pd3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
			VertexPositionColor::InputElementCount,
			shaderByteCode, byteCodeLength,
			&g_pBatchInputLayout);
		if (FAILED(hr))
			return hr;
	}

	if (CUSLFile::IsAvailable(FilePathName = theApp.m_WorkingDirectory + L"\\DynamicFiles\\italic.spritefont")) {
		g_Font.reset(new SpriteFont(g_pd3dDevice, FilePathName));
	}
	else {
		CString Buff;
		Buff.Format(L"Failed to find fonts %s", FilePathName);
		MessageBox(Buff, L"Warning", MB_ICONWARNING);
	}

	SetLights();

	g_Cylinder = GeometricPrimitive::CreateCylinder(g_pImmediateContext, 200.0f, 20.0f, 32, true);
	g_CoordSphere = GeometricPrimitive::CreateSphere(g_pImmediateContext, 10.0f, 16U, true, false);

	//	g_Model = Model::CreateFromSDKMESH(g_pd3dDevice, L"..\\DynamicFiles\\tiny.sdkmesh", *g_FXFactory, true);
	//	g_Model = Model::CreateFromSDKMESH(g_pd3dDevice, L"..\\DynamicFiles\\Tiny_Skin.dds", *g_FXFactory, true);

	// Load the Texture

	if (CUSLFile::IsAvailable(FilePathName = theApp.m_WorkingDirectory + L"\\DynamicFiles\\PlainMetal.dds")) {
		hr = CreateDDSTextureFromFile(g_pd3dDevice, FilePathName, nullptr, &g_pTexturePlainMetal);
		if (FAILED(hr))
			return hr;
	}

	if (CUSLFile::IsAvailable(FilePathName = theApp.m_WorkingDirectory + L"\\DynamicFiles\\windowslogo.dds")) {
		hr = CreateDDSTextureFromFile(g_pd3dDevice, FilePathName, nullptr, &g_pTextureRV2);
		if (FAILED(hr))
			return hr;
	}

	if (CUSLFile::IsAvailable(FilePathName = theApp.m_WorkingDirectory + L"\\DynamicFiles\\MetalFloorsBare0056_1_M.dds")) {
		hr = CreateDDSTextureFromFile(g_pd3dDevice, FilePathName, nullptr, &g_pTextureGaugePlate);
		if (FAILED(hr))
			return hr;
	}

	// Initialize the world matrices
	g_World = XMMatrixIdentity();
	g_Camera.SetWorldMatrix(g_World);

	// Initialize the view matrix
	g_Eye = XMVectorSet(0.0f, -2000.f, 0.0f, 0.0f);
	g_At = XMVectorSet(0.f, 0.f, 0.f, 0.0f);
	g_Up = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	g_fObjectRadius = 2000.0f;


	g_Camera.SetViewParams(&g_Eye, &g_At, &g_Up);
	g_Camera.SetRadius(g_fObjectRadius * 1.0f, g_fObjectRadius * 0.01f, g_fObjectRadius * 10.0f);
	g_Camera.SetProjParams(XM_PIDIV4, width / (FLOAT)height, 100.0f, 40000.0f);
	g_Camera.SetButtonMasks(MOUSE_MIDDLE_BUTTON, MOUSE_WHEEL, MOUSE_LEFT_BUTTON);
	g_Camera.SetWindow(width, height);

	g_BatchEffect->SetView(*g_Camera.GetViewMatrix());
	g_BatchEffect->SetProjection(*g_Camera.GetProjMatrix());


#ifdef DXTK_AUDIO

	// Create DirectXTK for Audio objects
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	g_audEngine.reset(new AudioEngine(eflags));

	g_audioEvent = 0;
	g_audioTimerAcc = 10.f;

	g_waveBank.reset(new WaveBank(g_audEngine.get(), L"adpcmdroid.xwb"));
	g_soundEffect.reset(new SoundEffect(g_audEngine.get(), L"MusicMono_adpcm.wav"));
	g_effect1 = g_soundEffect->CreateInstance();
	g_effect2 = g_waveBank->CreateInstance(10);

	g_effect1->Play(true);
	g_effect2->Play();

#endif // DXTK_AUDIO

	m_b3DInit = true;
	return S_OK;
}

HRESULT C3DWorldViewPane::Render3D()
{
	CString Buff;

#ifdef DXTK_AUDIO

	g_audioTimerAcc -= dt;
	if (g_audioTimerAcc < 0)
	{
		g_audioTimerAcc = 4.f;

		g_waveBank->Play(g_audioEvent++);

		if (g_audioEvent >= 11)
			g_audioEvent = 0;
	}

	if (!g_audEngine->Update())
	{
		// Error cases are handled by the message loop
	}

#endif // DXTK_AUDIO

	// Clear the back buffer
	if (theApp.m_LastSettings.rgbBackGroundWorld.v.m128_f32[0] == 0.0f && theApp.m_LastSettings.rgbBackGroundWorld.v.m128_f32[1] == 0.0f && theApp.m_LastSettings.rgbBackGroundWorld.v.m128_f32[2] == 0.0f) {
		theApp.m_LastSettings.rgbBackGroundWorld = Colors::MidnightBlue;
		theApp.m_LastSettings.rgbBackGroundProfile = Colors::LightPink;
	}
	if (g_Camera.GetDragWhich() == DRAG_VIEW) {
		g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, theApp.m_LastSettings.rgbBackGroundWorld);
	}
	else {
		g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, theApp.m_LastSettings.rgbBackGroundProfile);
	}
	// Clear the depth buffer to 1.0 (max depth)
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	g_Camera.FeatureMove(0);

	// Draw sprite
	if (g_Font != NULL) {
		g_Sprites->Begin(SpriteSortMode_Deferred);
		//	g_Sprites->Draw(g_pTextureRV2, XMFLOAT2(50, 105), nullptr, Colors::White);

		float fDirection = 0.0f;
		XMFLOAT2 vOrigin(0.0f, 0.0f);
		XMVECTOR vPt = *g_Camera.GetEyePt();
		Buff.Format(L"eye pt %.01f %.01f %.01f", vPt.m128_f32[0], vPt.m128_f32[1], vPt.m128_f32[2]);
		g_Font->DrawString(g_Sprites.get(), Buff, XMFLOAT2(100, 0), Colors::Yellow, fDirection, vOrigin, 0.5f);

		g_Profile = *g_Camera.GetProfileMatrix();
		Buff.Format(L"mProfile %.02f %.02f %.02f", g_Profile.r[0].m128_f32[0], g_Profile.r[0].m128_f32[1], g_Profile.r[0].m128_f32[2]);
		g_Font->DrawString(g_Sprites.get(), Buff, XMFLOAT2(100, 120), Colors::Yellow, fDirection, vOrigin, 0.5f);
		Buff.Format(L"mProfile %.02f %.02f %.02f", g_Profile.r[1].m128_f32[0], g_Profile.r[1].m128_f32[1], g_Profile.r[1].m128_f32[2]);
		g_Font->DrawString(g_Sprites.get(), Buff, XMFLOAT2(100, 140), Colors::Yellow, fDirection, vOrigin, 0.5f);
		Buff.Format(L"mProfile %.02f %.02f %.02f", g_Profile.r[2].m128_f32[0], g_Profile.r[2].m128_f32[1], g_Profile.r[2].m128_f32[2]);
		g_Font->DrawString(g_Sprites.get(), Buff, XMFLOAT2(100, 160), Colors::Yellow, fDirection, vOrigin, 0.5f);

		g_Sprites->End();
	}

	//////
//	m_Animate.FrameMove(&g_Camera);
	g_Camera.FrameMove(0);
	g_World = *g_Camera.GetWorldMatrix();
	g_Projection = *g_Camera.GetProjMatrix();
	g_View = *g_Camera.GetViewMatrix();
	g_At = *g_Camera.GetLookAtPt();

	g_BatchEffect->SetView(g_View);
	g_BatchEffect->SetProjection(g_Projection);



	XMMATRIX local;
	/*
	//Draw 3D Point
	//Red x
	local = XMMatrixRotationZ(XM_PIDIV2);
	local = XMMatrixMultiply(local, XMMatrixTranslation(100.0f, 0.0f, 0.0f));
	g_Cylinder->Draw(local, g_View, g_Projection, Colors::Red, g_pTexturePlainMetal);
	//Green Y
	local = XMMatrixTranslation(0.f, 100.f, 0.f);
	g_Cylinder->Draw(local, g_View, g_Projection, Colors::Green, g_pTexturePlainMetal);
	//Blue Z
	local = XMMatrixTranslation(0.f, 100.f, 0.f);
	local = XMMatrixMultiply(local, XMMatrixRotationX(XM_PIDIV2));
	g_Cylinder->Draw(local, g_View, g_Projection, Colors::Blue, g_pTexturePlainMetal);
	*/

	RenderTwinTowerScanner();

//	RenderCartesianPlanes();
	//	RenderQuaternionTest();

	//	DrawLinesImmediate(*g_BatchComponent);
	InitializeProfile();
	InitializeDangerPlanes();
//	InitializeSlicePlane();
	InitializeRefPoints();
//	InitializeCAD();

	RenderProfile();
//	RenderCAD();
//	RenderDangerPlanes();
//	RenderSlicePlane();

	XMVECTOR qid = XMQuaternionIdentity();
	const XMVECTORF32 scale = { 0.91f, 0.91f, 0.91f };
	const XMVECTORF32 translate = { 3.f, -2.f, 4.f };
	XMVECTOR rotate = XMQuaternionRotationRollPitchYaw(0, XM_PI / 2.f, XM_PI / 2.f);
	local = XMMatrixMultiply(g_World, XMMatrixTransformation(g_XMZero, qid, scale, g_XMZero, rotate, translate));
	//	g_Model->Draw(g_pImmediateContext, *g_States, local, g_View, g_Projection);
	//
	// Present our back buffer to our front buffer
	//
	g_pSwapChain->Present(0, 0);


	return S_OK;
}

HRESULT C3DWorldViewPane::InitializeObjects()
{
//	InitializeLookAtPoint();
//	InitializeCartesianPlanes();
//	InitializeQuaternionTest();

	InitializeTwinTowerScanner();

	return S_OK;
}




void C3DWorldViewPane::CleanUp3D()
{
	if (g_pd3dDevice) {
		m_b3DInit = false;
		if (g_pImmediateContext) g_pImmediateContext->ClearState();

		SAFE_RELEASE(g_d3dDebug);
		SAFE_RELEASE(g_pBatchInputLayout);
		SAFE_RELEASE(g_pTexturePlainMetal);
		SAFE_RELEASE(g_pTextureRV2);
		SAFE_RELEASE(g_pTextureGaugePlate);
		SAFE_RELEASE(g_pDepthStencilView);
		SAFE_RELEASE(g_pDepthStencil);
		SAFE_RELEASE(g_pRenderTargetView);
		SAFE_RELEASE(g_pSwapChain);
		SAFE_RELEASE(g_pImmediateContext);
		SAFE_RELEASE(g_pd3dDevice);
	}

#ifdef DXTK_AUDIO
	g_audEngine.reset();
#endif
}

void C3DWorldViewPane::DrawGrid(PrimitiveBatch<VertexPositionColor>& batch, FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color)
{
	g_pImmediateContext->OMSetBlendState(g_States->Opaque(), nullptr, 0xFFFFFFFF);
	g_pImmediateContext->OMSetDepthStencilState(g_States->DepthNone(), 0);
	g_pImmediateContext->RSSetState(g_States->CullCounterClockwise());

	g_BatchEffect->Apply(g_pImmediateContext);

	g_pImmediateContext->IASetInputLayout(g_pBatchInputLayout);

	g_Batch->Begin();

	xdivs = std::max<size_t>(1, xdivs);
	ydivs = std::max<size_t>(1, ydivs);

	for (size_t i = 0; i <= xdivs; ++i)
	{
		float fPercent = float(i) / float(xdivs);
		fPercent = (fPercent * 2.0f) - 1.0f;
		XMVECTOR vScale = XMVectorScale(xAxis, fPercent);
		vScale = XMVectorAdd(vScale, origin);

		VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color);
		VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color);
		batch.DrawLine(v1, v2);
	}

	for (size_t i = 0; i <= ydivs; i++)
	{
		FLOAT fPercent = float(i) / float(ydivs);
		fPercent = (fPercent * 2.0f) - 1.0f;
		XMVECTOR vScale = XMVectorScale(yAxis, fPercent);
		vScale = XMVectorAdd(vScale, origin);

		VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color);
		VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color);
		batch.DrawLine(v1, v2);
	}

	g_Batch->End();
}

void C3DWorldViewPane::SetLights()
{
	int nMask = 1;

	//	g_BatchEffect->SetVertexColorEnabled(true);

	//	g_BatchEffect->SetLightingEnabled(true);
	//	g_BatchEffect->SetPerPixelLighting(true);

	g_Colour = XMLoadFloat3(&XMFLOAT3(theApp.m_LastSettings.vProfileAmbientColour));
	g_BatchEffect->SetAmbientLightColor(g_Colour);

	for (int i = 0, nMask = 1; i < 3; i++, nMask <<= 1) {
		if (theApp.m_LastSettings.nProfileLightOffOnMask & nMask) {
			g_BatchEffect->SetLightEnabled(i, true);
			g_Colour = XMLoadFloat3(&XMFLOAT3(theApp.m_LastSettings.vProfileLightDirection[i])); g_BatchEffect->SetLightDirection(i, g_Colour);
			g_Colour = XMLoadFloat3(&XMFLOAT3(theApp.m_LastSettings.vProfileDiffuse[i]));		g_BatchEffect->SetLightDiffuseColor(i, g_Colour);
			g_Colour = XMLoadFloat3(&XMFLOAT3(theApp.m_LastSettings.vProfileSpecular[i]));		g_BatchEffect->SetLightSpecularColor(i, g_Colour);
		}
		else {
			g_BatchEffect->SetLightEnabled(i, false);
		}
	}

	//	g_BatchEffect->Apply(g_pImmediateContext);
}

void C3DWorldViewPane::StartThread()
{
	theApp.StartThread(L"WorldView", &WorldViewCallBack, this, 100, THREAD_PRIORITY_NORMAL);
}


void C3DWorldViewPane::SuspendThread()
{
	TRACE0("3DWorldView Suspend thread\n");
	theApp.SuspendThread(this);
	TRACE0("3DWorldView Suspended\n");
}



UINT WorldViewCallBack(LPVOID pParam)
{
	C3DWorldViewPane* pParent = (C3DWorldViewPane*)pParam;
	static int nProfileDisplayMask = -1;

	if (pParent->GetSafeHwnd() && pParent->IsWindowVisible() == TRUE && pParent->m_b3DInit == true) {
		if (theApp.m_LastSettings.nProfileDisplayMask - nProfileDisplayMask || pParent->m_bInvalidateFlag == true || PROFILE->getNeedsRendering() == true/*|| pParent->m_Animate.NeedsAnimating() == true*/) {
			pParent->Render3D();
			pParent->m_bInvalidateFlag = false;
			nProfileDisplayMask = theApp.m_LastSettings.nProfileDisplayMask;
			return THREAD_CONTINUE;
		}
	}

	return THREAD_CONTINUE;
}

void C3DWorldViewPane::Invalidate()
{
	m_bInvalidateFlag = true;
}

HRESULT C3DWorldViewPane::InitializeTwinTowerScanner()
{
	XMFLOAT3 vTop;

	g_TwinTowerXCarriageSize = XMFLOAT3(theApp.m_Tank.fXLength, 1000.0f, 200.0f);
	g_FloorPanelSize = XMFLOAT3(1500, 1000.0f, 200.0f);
	g_FloorPanel = GeometricPrimitive::CreateBox(g_pImmediateContext, g_FloorPanelSize, true, false);

	g_BLueTowerSize = XMFLOAT3(500.0f, 1000.0f, theApp.m_Tank.fZLength);
	g_TwinTowerBlueZTower = GeometricPrimitive::CreateBox(g_pImmediateContext, g_BLueTowerSize, true, false);
	g_RedTowerSize = XMFLOAT3(900.0f, 1000.0f, theApp.m_Tank.fZLength / 2.0f);
	g_TwinTowerRedZTower = GeometricPrimitive::CreateBox(g_pImmediateContext, g_RedTowerSize, true, false);

	vTop = g_RedTowerSize;
	vTop.x /= 2.0f;
	g_TopOfRedTower = GeometricPrimitive::Wedge(g_pImmediateContext, g_RedTowerSize, vTop, true, false);

	g_TwinTowerYArmSize = XMFLOAT3(150.0f, 3000.0f, 150.0f);
	g_TwinTowerYArm = GeometricPrimitive::CreateBox(g_pImmediateContext, g_TwinTowerYArmSize, true, false);

	XMFLOAT3 LongStringerSize = XMFLOAT3(theApp.m_Tank.fXLength, 10.0f, 50.0f);
	g_LongGRPFloorStringer = GeometricPrimitive::CreateBox(g_pImmediateContext, LongStringerSize, true, false);
	XMFLOAT3 ShortStringerSize = XMFLOAT3(10.0f, theApp.m_Tank.fYLength, 50.0f);
	g_ShortGRPFloorStringer = GeometricPrimitive::CreateBox(g_pImmediateContext, ShortStringerSize, true, false);


	return S_OK;
}

void C3DWorldViewPane::RenderTwinTowerScanner()
{
	XMMATRIX local;
	XMMATRIX Zero;
	XMMATRIX Carriage;
	XMMATRIX Arm;
	float fXPos[2] = { 0.0f, 0.0f };
	float fYPos[2] = { -1000.0f, 1000.0f };
	float fZPos[2] = { 0.0f, 0.0f };

	theApp.m_LastSettings.nProfileDisplayMask |= DISPLAY_SCANNER;
	if (theApp.m_LastSettings.nProfileDisplayMask & DISPLAY_SCANNER) {

		Zero = XMMatrixTranslation(-theApp.m_Tank.fXSurfaceZero, -theApp.m_Tank.fYSurfaceZero, -theApp.m_Tank.fZSurfaceZero);

		for (float fX = 0.0f; fX < theApp.m_Tank.fXLength; fX += g_FloorPanelSize.x) {
			local = XMMatrixTranslation(fX + g_FloorPanelSize.x / 2.0f, g_TwinTowerXCarriageSize.y / -2.0f, g_TwinTowerXCarriageSize.z / -2.0f);
			g_FloorPanel->Draw(local * Zero, g_View, g_Projection, Colors::White, g_pTextureGaugePlate);
			local = XMMatrixTranslation(fX + g_FloorPanelSize.x / 2.0f, theApp.m_Tank.fYLength + g_TwinTowerXCarriageSize.y / 2.0f, g_TwinTowerXCarriageSize.z / -2.0f);
			g_FloorPanel->Draw(local * Zero, g_View, g_Projection, Colors::White, g_pTextureGaugePlate);
		}

		for (float fY = 0.0f; fY < theApp.m_Tank.fYLength; fY += 100.0f) {
			local = XMMatrixTranslation(g_TwinTowerXCarriageSize.x / 2.0f, fY, -25.0f);
			g_LongGRPFloorStringer->Draw(local * Zero, g_View, g_Projection, Colors::DarkOliveGreen, g_pTexturePlainMetal);
		}
		//Short floor
		for (float fX = 0.0f; fX < theApp.m_Tank.fXLength; fX += 400.0f) {
			local = XMMatrixTranslation(fX, theApp.m_Tank.fYLength / 2.0f, -25.0f);
			g_ShortGRPFloorStringer->Draw(local * Zero, g_View, g_Projection, Colors::DarkOliveGreen, g_pTexturePlainMetal);
		}


		Carriage = XMMatrixTranslation(fXPos[0], -theApp.m_Tank.fYSurfaceZero, -theApp.m_Tank.fZSurfaceZero);
		local = XMMatrixTranslation(-1000.0f + g_BLueTowerSize.x / 2.0f, -g_BLueTowerSize.y / 2.0f, g_BLueTowerSize.z / 2.0f);
		g_TwinTowerBlueZTower->Draw(local * Carriage, g_View, g_Projection, Colors::DarkSlateBlue, g_pTexturePlainMetal);
		local = XMMatrixTranslation(g_RedTowerSize.x / 2.0f + 100.0f, -g_RedTowerSize.y / 2.0f, g_RedTowerSize.z / 2.0f);
		g_TwinTowerRedZTower->Draw(local * Carriage, g_View, g_Projection, Colors::DarkRed, g_pTexturePlainMetal);
		local = XMMatrixTranslation(100.0f, -g_RedTowerSize.y / 2.0f, g_RedTowerSize.z);
		g_TopOfRedTower->Draw(local * Carriage, g_View, g_Projection, Colors::DarkRed, g_pTexturePlainMetal);

		Carriage = XMMatrixTranslation(fXPos[1], -theApp.m_Tank.fYSurfaceZero, -theApp.m_Tank.fZSurfaceZero);
		local = XMMatrixTranslation(-1000.0f + g_BLueTowerSize.x / 2.0f, theApp.m_Tank.fYLength + g_BLueTowerSize.y / 2.0f, g_BLueTowerSize.z / 2.0f);
		g_TwinTowerBlueZTower->Draw(local * Carriage, g_View, g_Projection, Colors::DarkSlateBlue, g_pTexturePlainMetal);
		local = XMMatrixTranslation(g_RedTowerSize.x / 2.0f + 100.0f, theApp.m_Tank.fYLength + g_RedTowerSize.y / 2.0f, g_RedTowerSize.z / 2.0f);
		g_TwinTowerRedZTower->Draw(local * Carriage, g_View, g_Projection, Colors::DarkRed, g_pTexturePlainMetal);
		local = XMMatrixTranslation(100.0f, theApp.m_Tank.fYLength + g_RedTowerSize.y / 2.0f, g_RedTowerSize.z);
		g_TopOfRedTower->Draw(local * Carriage, g_View, g_Projection, Colors::DarkRed, g_pTexturePlainMetal);

		Arm = XMMatrixTranslation(fXPos[0], fYPos[0], fZPos[0]);
		local = XMMatrixTranslation(0.0f, -g_TwinTowerYArmSize.y / 2.0f, 0.0f);
		g_TwinTowerYArm->Draw(local * Arm, g_View, g_Projection, Colors::White, g_pTexturePlainMetal);
		Arm = XMMatrixTranslation(fXPos[1], fYPos[1], fZPos[1]);
		local = XMMatrixTranslation(0.0f, theApp.m_Tank.fYLength - g_TwinTowerYArmSize.y / 2.0f, 0.0f);
		g_TwinTowerYArm->Draw(local * Arm, g_View, g_Projection, Colors::White, g_pTexturePlainMetal);
	}
}




void C3DWorldViewPane::InitializeDangerPlanes()
{
	XMFLOAT3 vDangerplaneSize = PROFILE->getDangerPlaneGlassSize(0);
	g_DangerPlane[0] = GeometricPrimitive::CreateBox(g_pImmediateContext, vDangerplaneSize, false, false);
	vDangerplaneSize = PROFILE->getDangerPlaneGlassSize(1);
	g_DangerPlane[1] = GeometricPrimitive::CreateBox(g_pImmediateContext, vDangerplaneSize, false, false);

}

void C3DWorldViewPane::OnButtonWorldviewSetup()
{
	if (m_p3DToolsSheet == NULL) {
		m_p3DToolsSheet = new C3DToolsSheet(_T("3D Tools"), this, 0, (CWnd**)&m_p3DToolsSheet, 1);
		m_p3DToolsSheet->Create(this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_3D_TOOLS_SHEET] = TRUE;
	}
	else {
		m_p3DToolsSheet->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

BOOL C3DWorldViewPane::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	CRect rr;
	CPoint pt;

	switch (message) {
	case WM_HOTKEY:
		GetCursorPos(&pt);
		break;
	}

	if (GetSafeHwnd()) {
		GetCursorPos(&pt);
		GetClientRect(rr);
		ScreenToClient(&pt);
		if (rr.PtInRect(pt) == false) {
			return CDockablePane::OnWndMsg(message, wParam, lParam, pResult);
		}
		switch (message) {
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_MOUSEMOVE:
//			SAFE_DELETE(m_pGeneralInputDlg);
			g_Camera.HandleMessages(GetSafeHwnd(), message, wParam, lParam);
			m_bInvalidateFlag = true;
			return true;
			break;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
//			SAFE_DELETE(m_pGeneralInputDlg);
			if (theApp.m_LastSettings.nProfileDisplayMask & RBUTTON_PROFILE_MODE) {
				g_Camera.HandleMessages(GetSafeHwnd(), message, wParam, lParam);
				m_bInvalidateFlag = true;
				return true;
			}
			break;
		}
	}
	return CDockablePane::OnWndMsg(message, wParam, lParam, pResult);
}

HRESULT C3DWorldViewPane::InitializeProfile()
{
	XMVECTOR *pVertexArray0, *pVertexArray1, *pNormalArray0, *pNormalArray1, *pVertexArrayTemp, *pNormalArrayTemp;
	float *pfDistance;
	float fShortest = 1e6f;
	LineTypes	nLineType = LineTypes::scan;
	int nIndex, nDestIndex, nMaxIndexL;


	if (PROFILE->getNeedsRendering() == true) {

		nMaxIndexL = 0;
		for (int nLine = 0; nLine < PROFILE->GetNumberLines((int)nLineType); nLine++) {
			CPolyCoord* pLine0 =  PROFILE->GetLineElement(nLineType, nLine);
			int nIndexL = pLine0->GetSize();
			if (nMaxIndexL < nIndexL) nMaxIndexL = nIndexL;
		}
		pVertexArray0 = (XMVECTOR *)_aligned_malloc(nMaxIndexL * sizeof XMVECTOR, 16);
		pVertexArray1 = (XMVECTOR *)_aligned_malloc(nMaxIndexL * sizeof XMVECTOR, 16);
		pVertexArrayTemp = (XMVECTOR *)_aligned_malloc(nMaxIndexL * sizeof XMVECTOR, 16);
		pNormalArrayTemp = (XMVECTOR *)_aligned_malloc(nMaxIndexL * sizeof XMVECTOR, 16);
		pNormalArray0 = (XMVECTOR *)_aligned_malloc(nMaxIndexL * sizeof XMVECTOR, 16);
		pNormalArray1 = (XMVECTOR *)_aligned_malloc(nMaxIndexL * sizeof XMVECTOR, 16);
		pfDistance = new float[nMaxIndexL];

		theApp.m_LastSettings.nProfileDisplayMask |= FILL_IN_SCAN_LINES;
//		theApp.m_LastSettings.nProfileDisplayMask &= ~FILL_IN_SCAN_LINES;

		for (int nSide = 0; nSide < 2; nSide++) {
			if (theApp.m_LastSettings.nProfileDisplayMask & FILL_IN_SCAN_LINES) {

				for (g_nScanLinesL[nSide] = 0; g_nScanLinesL[nSide] < PROFILE->GetNumberLines((int)nLineType) - 1; g_nScanLinesL[nSide]++) {
					CPolyCoord* pLine0 = PROFILE->GetLineElement(nLineType, g_nScanLinesL[nSide]);
					CPolyCoord* pLine1 = PROFILE->GetLineElement(nLineType, g_nScanLinesL[nSide] + 1);
					int nIndexL = pLine0->GetSize();
					if (nIndexL > 0) {
						fShortest = 1e6f;
						for (nIndex = 0; nIndex < nIndexL; nIndex++) {
							pfDistance[nIndex] = pLine1->NearestPointDistance(&XMLoadFloat3((XMFLOAT3*)&pLine0->ElementAt(nIndex)->Side[nSide].pt), nSide, &pVertexArrayTemp[nIndex], &pNormalArrayTemp[nIndex], 1.15f);
							if (fShortest > pfDistance[nIndex]) fShortest = pfDistance[nIndex];
						}
						fShortest *= 1.5f;
						for (nIndex = 0, nDestIndex = 0; nIndex < nIndexL; nIndex++) {
							if (pfDistance[nIndex] < fShortest) {
								pVertexArray0[nDestIndex] = XMLoadFloat3((XMFLOAT3*)&pLine0->ElementAt(nIndex)->Side[nSide].pt);
								pNormalArray0[nDestIndex] = XMLoadFloat4((XMFLOAT4*)&pLine0->ElementAt(nIndex)->Side[nSide].norm);
								pVertexArray1[nDestIndex] = pVertexArrayTemp[nIndex];
								pNormalArray1[nDestIndex] = pNormalArrayTemp[nIndex];
								nDestIndex++;
							}
						}

						g_ScanLines[nSide][g_nScanLinesL[nSide]] = GeometricPrimitive::CreateFilledCoordArray(g_pImmediateContext, nDestIndex, pVertexArray0, pVertexArray1, pNormalArray0, pNormalArray1, true, false);
					}
				}

			}
			else {
				for (g_nScanLinesL[nSide] = 0; g_nScanLinesL[nSide] < PROFILE->GetNumberLines((int)nLineType); g_nScanLinesL[nSide]++) {
					CPolyCoord* pLine = PROFILE->GetLineElement(nLineType, g_nScanLinesL[nSide]);
					int nIndexL = pLine->GetSize();
					if (nIndexL > 0) {
						for (int nIndex = 0; nIndex < nIndexL; nIndex++) {
							pVertexArray0[nIndex] = XMLoadFloat3((XMFLOAT3*)&pLine->ElementAt(nIndex)->Side[nSide].pt);
							pNormalArray0[nIndex] = XMLoadFloat4((XMFLOAT4*)&pLine->ElementAt(nIndex)->Side[nSide].norm);
						}
						g_ScanLines[nSide][g_nScanLinesL[nSide]] = GeometricPrimitive::CreateCoordArray(g_pImmediateContext, nIndexL, pVertexArray0, pNormalArray0, true, false);
					}
				}
			}
		}

		_aligned_free(pVertexArray0);
		_aligned_free(pVertexArray1);
		_aligned_free(pVertexArrayTemp);
		_aligned_free(pNormalArrayTemp);
		_aligned_free(pNormalArray0);
		_aligned_free(pNormalArray1);
		delete pfDistance;

		PROFILE->setNeedsRendering(false);
	}

	return S_OK;
}

void C3DWorldViewPane::RenderProfile()
{
	LineTypes nLineType;
	XMMATRIX local = XMMatrixIdentity();
	XMMATRIX mNorm;

	theApp.m_LastSettings.rgbFloodFill[0] = Colors::Blue;
	theApp.m_LastSettings.rgbFloodFill[1] = Colors::Green;

	theApp.m_LastSettings.nProfileDisplayMask |= DISPLAY_SCAN_LINES;
	theApp.m_LastSettings.nProfileDisplayMask |= DISPLAY_BOTH_FACES;

	if (theApp.m_LastSettings.nProfileDisplayMask & DISPLAY_SCAN_LINES) {
		nLineType = LineTypes::scan;
		for (int nLine = 0; nLine < g_nScanLinesL[0]; nLine++) {
			g_ScanLines[0][nLine]->Draw(local, g_View, g_Projection, theApp.m_LastSettings.rgbFloodFill[0], NULL);
		}
		if (theApp.m_LastSettings.nProfileDisplayMask & DISPLAY_BOTH_FACES) {
			for (int nLine = 0; nLine < g_nScanLinesL[1]; nLine++) {
				g_ScanLines[1][nLine]->Draw(local, g_View, g_Projection, theApp.m_LastSettings.rgbFloodFill[1], NULL);
			}
		}
	}
	local = XMMatrixIdentity();
	/*
	if (theApp.m_LastSettings.nProfileDisplayMask & DISPLAY_DANGER_PLANES) {
		//	m_WS.getP
		XMPT CpCenter;
		m_WS.getProfileDangerPlaneGlassCenter(-1, CpCenter);

		local = XMMatrixTranslationFromVector(XMLoadFloat3(&CpCenter.Side0.pt));
		XMVECTOR alpha = theApp.m_LastSettings.rgbDangerPlane[0];
		alpha.m128_f32[3] = 0.5f;
		g_DangerPlane[0]->Draw(local, g_View, g_Projection, alpha, NULL);

		local = XMMatrixTranslationFromVector(XMLoadFloat3(&CpCenter.Side1.pt));
		alpha = theApp.m_LastSettings.rgbDangerPlane[1];
		alpha.m128_f32[3] = 0.5f;
		g_DangerPlane[1]->Draw(local, g_View, g_Projection, alpha, NULL);
	}

	if (m_WS.getLineTypeEdit() != LineTypes::reference) {
		CCoordArray* pLine = m_WS.getProfileCurrentLineElement();
		int nIndexEdit = m_WS.getIndexEdit();
		if (pLine != NULL) {
			for (int nIndex = 0; nIndex < pLine->GetSize(); nIndex++) {
				local = XMMatrixTranslationFromVector(XMLoadFloat3(&pLine->ElementAt(nIndex)->Side[0].pt));
				if (nIndex == nIndexEdit) {
					g_CoordSphere->Draw(local, g_View, g_Projection, Colors::Red, NULL);
				}
				else {
					g_CoordSphere->Draw(local, g_View, g_Projection, Colors::Silver, NULL);
				}
			}
		}
	}
	*/
	theApp.m_LastSettings.nProfileDisplayMask |= DISPLAY_REF_POINTS;
	if (theApp.m_LastSettings.nProfileDisplayMask & DISPLAY_REF_POINTS) {
		FXMVECTOR vColor[3] = { Colors::Red, Colors::Green, Colors::Blue };
		XMVECTOR vStart, vTopPost;
		XMMATRIX	mLocal;
		for (int nRef = 0; nRef < 3; nRef++) {
			CCoord* pCp = &PROFILE->m_CpCadHole[nRef];
			float fYaw = atan2f(pCp->Side[0].fI, -pCp->Side[0].fJ);
			float fPitch = -atan2f(pCp->Side[0].fK, sqrtf(pCp->Side[0].fI * pCp->Side[0].fI + pCp->Side[0].fJ *pCp->Side[0].fJ));
			if (pCp->nType == 0) { // Surface
				vStart = XMLoadFloat3(&XMFLOAT3(0, -2, 0));
			}
			else {
				vStart = XMLoadFloat3(&XMFLOAT3(0, (theApp.m_LastSettings.fBallHeight - theApp.m_LastSettings.fBallDiameter / 2.0f) / -2.0f, 0));
				vTopPost = XMLoadFloat3(&XMFLOAT3(0, -(theApp.m_LastSettings.fBallHeight - theApp.m_LastSettings.fBallDiameter / 2.0f), 0));
			}
			mLocal = XMMatrixTranslationFromVector(vStart);
			mNorm = XMMatrixMultiply(mLocal, XMMatrixRotationX(fPitch));
			mNorm = XMMatrixMultiply(mNorm, XMMatrixRotationZ(fYaw));
			mNorm = XMMatrixMultiply(mNorm, XMMatrixTranslationFromVector(XMLoadFloat3((XMFLOAT3*)&pCp->Side[0].pt)));
			if (pCp->nType == 0) { // Surface
				g_RefDisc->Draw(mNorm, g_View, g_Projection, vColor[nRef], NULL);
			}
			else {
				g_RefPost->Draw(mNorm, g_View, g_Projection, vColor[nRef], NULL);
				mLocal = XMMatrixTranslationFromVector(vTopPost);
				mNorm = XMMatrixMultiply(mLocal, XMMatrixRotationX(fPitch));
				mNorm = XMMatrixMultiply(mNorm, XMMatrixRotationZ(fYaw));
				mNorm = XMMatrixMultiply(mNorm, XMMatrixTranslationFromVector(XMLoadFloat3((XMFLOAT3*)&pCp->Side[0].pt)));
			g_RefSphere->Draw(mNorm, g_View, g_Projection, vColor[nRef], NULL);
			}
		}
	}
	
}

void C3DWorldViewPane::InitializeRefPoints()
{
	g_RefDisc = GeometricPrimitive::CreateCylinder(g_pImmediateContext, 4.0f, theApp.m_LastSettings.fBallDiameter, 32, true);
	g_RefPost = GeometricPrimitive::CreateCylinder(g_pImmediateContext, theApp.m_LastSettings.fBallHeight - theApp.m_LastSettings.fBallDiameter / 2.0f, theApp.m_LastSettings.fBallDiameter / 2.0f, 32, true);
	g_RefSphere = GeometricPrimitive::CreateSphere(g_pImmediateContext, theApp.m_LastSettings.fBallDiameter, 16U, true, false);
}


void C3DWorldViewPane::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDockablePane::OnClose();
}


void C3DWorldViewPane::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: Add your message handler code here
}


void C3DWorldViewPane::PostNcDestroy()
{
	CleanUp3D();

	CDockablePane::PostNcDestroy();
}
