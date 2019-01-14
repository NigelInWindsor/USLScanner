#pragma once
#include "USLPaneToolBar.h"
#include "../DirectXTK/Inc/CommonStates.h"
#include "../DirectXTK/Inc/DDSTextureLoader.h"
#include "../DirectXTK/Inc/Effects.h"
#include "../DirectXTK/Inc/GeometricPrimitive.h"
#include "../DirectXTK/Inc/Model.h"
#include "../DirectXTK/Inc/PrimitiveBatch.h"
#include "../DirectXTK/Inc/ScreenGrab.h"
#include "../DirectXTK/Inc/SpriteBatch.h"
#include "../DirectXTK/Inc/SpriteFont.h"
#include "../DirectXTK/Inc/VertexTypes.h"
#include "3DCamera.h"
#include "3DToolsSheet1.h"


using namespace DirectX;

class C3DWorldViewPane : public CDockablePane
{
	DECLARE_DYNAMIC(C3DWorldViewPane)

public:
	bool	m_bInvalidateFlag;
	bool	m_b3DInit;
	CFont	m_Font;
	CStatic m_staticView;
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	HRESULT InitializeProfile();
	void RenderProfile();
	void InitializeRefPoints();
	CUSLPaneToolBar m_wndToolBar[4];
	C3DToolsSheet* m_p3DToolsSheet;


	C3DWorldViewPane();
	~C3DWorldViewPane();
	void Restart();
	void AdjustLayout();
	int CreateFont();
	void Start3D();
	HRESULT InitD3D(HWND hWnd);
	HRESULT Render3D();
	HRESULT InitializeObjects();
	void CleanUp3D();
	void DrawGrid(PrimitiveBatch<VertexPositionColor>& batch, FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color);
	void SetLights();
	void StartThread();
	void SuspendThread();
	void Invalidate();
	HRESULT InitializeTwinTowerScanner();
	void RenderTwinTowerScanner();
	void InitializeDangerPlanes();

	void OnButtonWorldviewSetup();

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual void PostNcDestroy();
};

