#if !defined(AFX_3DVIEWPAGE_H__9580F0D5_CE43_4E3D_90E7_D70D75944766__INCLUDED_)
#define AFX_3DVIEWPAGE_H__9580F0D5_CE43_4E3D_90E7_D70D75944766__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3DViewPage.h : header file
//
#include "ResizablePage.h"
#include "StaticPosManage.h"
#include <d3d9.h>
#include <d3dx9math.h>
#include "D3DFont.h"
#include "3DObject.h"

#define	_MAXLIGHTS 7
#define MAX_TEXT_STRINGS	20

#define	PRIM_TANK			0
#define	PRIM_SCANLINES		1
#define	PRIM_DANGERZONE		2
#define	PRIM_CSCAN			3
#define PRIM_ISOMETRIC		4
#define	PRIM_ROTORY_COMP	5
#define	PRIM_TURNTABLE		6
#define	PRIM_MESH			7

/////////////////////////////////////////////////////////////////////////////
// C3DViewPage dialog

class C3DViewPage : public CResizablePage
{
	DECLARE_DYNCREATE(C3DViewPage)

// Construction
public:
	void InitializeEyeAndLookAtPt();
	void CalculateEdges();
	void DrawTrackerElements();
	void DrawScanViaDangerPlanePoints();
	inline int MinMax(int *pnV, int nMin, int nMax);
	void LookAtPoint(CCoord Cp,bool bRotate = true);
	void InitializeSurface();
	void SolveForZEqualZero(D3DXVECTOR3* pvecOut, D3DXPLANE* plane0, D3DXPLANE* plane1);

	void SetToolBarCheckedState();
	void InitializeDangerZone();
	void InitializeRectilinearDataVertices();
	void InitializeRectilinearScanLines();
	void InitializeRotoSymmetricScanLines();
	void InitializeLookAtPoint();
	void InitializeScratchLines();
	void InitializeRobot();
	void InitializeXYZ();
	void InitializeBraneLines();
	void InitializeOutsideInsideLines();


	C3DObject* m_p3DObjectOutsideLines;
	C3DObject* m_p3DObjectInsideLines;
	C3DObject* m_p3DObjectDataVertices;
	C3DObject* m_p3DObjectScratchLines;
	C3DObject* m_p3DObjectLookAtPoint;
	C3DObject* m_p3DObjectTaughtLines;
	C3DObject* m_p3DObjectTurntable;
	C3DObject* m_p3DObjectDangerPlanes;
	C3DObject* m_p3DObjectRefPoints;
	C3DObject* m_p3DObjectEnvelopeLines;
	C3DObject* m_p3DObjectBraneLines;
	C3DObject* m_p3DObjectScanLines;
	C3DObject* m_p3DObjectScanner;
	C3DObject* m_p3DObjectCScanOverlay;
	C3DObject* m_p3DObjectRoom;
	C3DObject* m_p3DObjectSurfaceLines;
	C3DObject* m_p3DObjectRobot;
	C3DObject* m_p3DObjectXYZ;
	ID3DXMesh*  m_pSphere;
	ID3DXMesh*  m_pCone;
	ID3DXMesh*  m_pRobotElement0;
	ID3DXMesh*  m_pRobotElement1;
	ID3DXMesh*  m_pRobotElement2;
	ID3DXMesh*  m_pRobotElement3;
	ID3DXMesh*  m_pRobotElement4;
	ID3DXMesh*  m_pRobotElement5;
    LPD3DXMESH    m_pMesh3DText[MAX_TEXT_STRINGS];
   

	D3DXQUATERNION D3DUtil_GetRotationFromCursor( HWND hWnd,
                                              FLOAT fTrackBallRadius=1.0f );
	void SetTextureCoords( WorldVertex* pVertex, float u, float v );
	HRESULT BuildColumn( WorldVertex* &pV, float x, float y, float z, float width );
	HRESULT BuildWorldMesh();
	HRESULT BuildComponentMesh(CProgressCtrl* pProgress);

	LPD3DXMESH			m_pComponentMesh;
	LPD3DXMESH			m_pWorldMesh;
	PDIRECT3DTEXTURE9	*m_pTexComponent;
	PDIRECT3DTEXTURE9	m_pTexScope;
	PDIRECT3DTEXTURE9	m_pTexWall;
	PDIRECT3DTEXTURE9	m_pTexFloor;
	int					m_nTexComponentL;
    D3DLIGHT9			m_light[_MAXLIGHTS];      // Description of the D3D lights
    D3DLIGHT9           m_lightPortside;          // Light used to add ambient lighting to the walls
    D3DLIGHT9           m_lightStarboard;          // Light used to add ambient lighting to the walls
    D3DLIGHT9           m_lightAmbient;          // Light used to add ambient lighting to the walls
    D3DLIGHT9           m_lightSpot;             // Light used to add ambient lighting to the walls
    D3DLIGHTTYPE        m_CurrentLightType;      // Current type of moving lights (0 to cycle though light types)

    D3DPRESENT_PARAMETERS	m_d3dpp;
	LPDIRECT3D9             m_pD3D;					// Used to create the D3DDevice
	LPDIRECT3DDEVICE9       m_pd3dDevice;			// Our rendering device
	PrimitiveData			m_PrimFrame;
//	PrimitiveData			m_PrimComp;
    LPD3DXPMESH				m_pPMeshSysMem;         // Sysmem version of pmesh, lives through resize's
    LPD3DXPMESH				m_pPMesh;				// Local version of pmesh, rebuilt on resize

    D3DMATERIAL9*			m_mtrlMeshMaterials;
    LPDIRECT3DTEXTURE9*		m_pMeshTextures;        // Array of textures, entries are NULL if no texture specified
    DWORD					m_dwNumMaterials;       // Number of materials
    LPD3DXBUFFER			m_pAdjacencyBuffer;     // Contains the adjaceny info loaded with the mesh

    CD3DFont*				m_p3DFont;
    CD3DFont*				m_p3DFontSmall;

	bool					m_b3DCScan;
	bool					m_b3DSurface;
	bool					m_b3DDataVertices;

	
	D3DXVECTOR3 m_vLookatPt;
	D3DXVECTOR3 m_vUpVec;
	D3DXVECTOR3 m_vEyePt;
    CD3DArcBall_10   m_WorldBall;				// ArcBall used for mouse input
    CD3DArcBall_10   m_CompBall;				// ArcBall used for mouse input
	D3DXMATRIXA16 m_matWorld;
	
	bool m_bLbDown;
	bool m_bRbDown;
	bool m_bMbDown;
	int m_n3DMoveEyeComponent;

	CToolBar   m_wnd3DToolBar;
	CReBar     m_wndRebar;
	
	
	HRESULT CreateD3DXTextMesh( LPD3DXMESH* ppMesh, CString strText, TCHAR* pstrFont, DWORD dwSize, BOOL bBold, BOOL bItalic );
	HRESULT InitD3D( HWND hWnd );
	void RenderXYZ();
	void DrawRobot();
	void DrawLookAtPoint();
	void InitializeCScanOverlay();
	void InitializeTankOfWaterZeroNearSide();
	void InitializeTankOfWaterZeroFarSide();
	void InitializeDualSidedSquirter();
	void InitializeComponent();
	void InitializeTurnTable();
	void InitializeRoom();
	void InitializeObjects();
	void SetupLights();
	void ComponentMove();
	void FrameMove();
	VOID Render3D();
	void CleanUp3D();
	void Start3D();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	void MouseWheel(UINT nFlags, short zDelta, CPoint pt);
	CStaticPosManage m_StaticPosView;
	
	C3DViewPage();
	~C3DViewPage();

// Dialog Data
	//{{AFX_DATA(C3DViewPage)
	enum { IDD = IDD_3D_VIEW_PAGE };
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(C3DViewPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(C3DViewPage)
	afx_msg void OnKickIdle();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButton3dViewPoint();
	afx_msg void OnButton3dComponent();
	afx_msg void OnButton3dDangerZone();
	afx_msg void OnButton3dMachine();
	afx_msg void OnButton3dRefPoints();
	afx_msg void OnButton3dScanlines();
	afx_msg void OnButton3dEnvelope();
	afx_msg void OnButton3dCscan();
	afx_msg void OnButton3dTools();
	afx_msg void OnButton3dTaughtlines();
	afx_msg void OnButton3dDataVertices();
	afx_msg void OnButton3dSurface();
	afx_msg void OnButton3dRobot();
	afx_msg void OnButtonSurface();
	afx_msg void OnButton3dBallRoll();
	afx_msg void OnButton3dRotateAboutZ();
	afx_msg void OnButton3dRotateAboutX();
	afx_msg void OnButton3dTracker();
	afx_msg void OnButton3dRotateAboutY();
	afx_msg void OnButton3dBranelines();
	afx_msg void OnButtonDisplayInside();
	afx_msg void OnButtonDisplayOutside();
	afx_msg void OnButtonDisplayWorksheetProfiles();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DVIEWPAGE_H__9580F0D5_CE43_4E3D_90E7_D70D75944766__INCLUDED_)
