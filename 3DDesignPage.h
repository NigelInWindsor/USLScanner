#if !defined(AFX_3DDESIGNPAGE_H__3C105756_9BE9_4AD0_AF2C_89C0BBC9E2B3__INCLUDED_)
#define AFX_3DDESIGNPAGE_H__3C105756_9BE9_4AD0_AF2C_89C0BBC9E2B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3DDesignPage.h : header file
//
#include "StaticPosManage.h"
#include "EditSpinItem.h"
#include "3DObject.h"
#include "D3DFont.h"

#define	MAXLIGHTS	20

/////////////////////////////////////////////////////////////////////////////
// C3DDesignPage dialog

class C3DDesignPage : public CPropertyPage
{
	DECLARE_DYNCREATE(C3DDesignPage)

// Construction
public:
	void WorldToScreen(D3DXVECTOR3 vec,CPoint *pt);
	HRESULT CreateD3DXTextMesh( LPD3DXMESH* ppMesh, TCHAR* pstrFont, DWORD dwSize, BOOL bBold, BOOL bItalic );
	HRESULT Pick();
	HRESULT HandlePossibleSizeChange();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	void Draw3DCone(D3DXVECTOR3 Centre, D3DXVECTOR3 Normal, float fRadius,float fHeight,int nColor);
	void Draw3DCircle(D3DXVECTOR3 Centre, D3DXVECTOR3 Normal, float fRadius,int nColor);
	void Draw3DLine(float fX0,float fY0,float fZ0,float fX1,float fY1,float fZ1,int nColor);
	void DrawLookAtPoint();
	void SetupLights();
	void SetupDualSidedSquirterLights();
	void SetupTankOfWaterLights();
	void ComponentMove();
	void FrameMove();
	void Render3D();
	HRESULT Reset3DEnvironment();
	D3DSURFACE_DESC		m_d3dsdBackBuffer;
 	C3DObject*			m_p3DObject;
	D3DLIGHT9			m_light[MAXLIGHTS];      // Description of the D3D lights
    D3DLIGHT9           m_lightPortside;          // Light used to add ambient lighting to the walls
    D3DLIGHT9           m_lightStarboard;          // Light used to add ambient lighting to the walls
    D3DLIGHT9           m_lightAmbient;          // Light used to add ambient lighting to the walls
    D3DLIGHT9           m_lightSpot;             // Light used to add ambient lighting to the walls
    D3DLIGHTTYPE        m_CurrentLightType;      // Current type of moving lights (0 to cycle though light types)
	D3DXVECTOR3 m_vLookatPt;
	D3DXVECTOR3 m_vUpVec;
	D3DXVECTOR3 m_vEyePt;
    CD3DArcBall_10   m_WorldBall;
    CD3DArcBall_10   m_CompBall;
	D3DXMATRIXA16 m_matWorld;

    TCHAR         m_strFont[LF_FACESIZE];
    int           m_nFontSize;
    LPD3DXMESH    m_pMesh3DText;
    LPD3DXFONT    m_pFont;
    ID3DXSprite*  m_pD3DXSprite;

	PrimitiveData			m_PrimFrame;
	LPDIRECT3D9             m_pD3D;					// Used to create the D3DDevice
	LPDIRECT3DDEVICE9       m_pd3dDevice;			// Our rendering device
	CPoint m_ptDown;
	bool m_bDeviceObjectsRestored;
	bool m_bLBDown;
	bool m_bRBDown;
	bool m_bMBDown;
	bool m_bWireframe;
	int m_nProjectionMoveOption;
	int	m_n3DMoveEyeComponent;
    D3DPRESENT_PARAMETERS	m_d3dpp;

	HRESULT InitD3D( HWND hWnd );

	CRect		m_rrView;
	CToolBar   m_wndDesignToolBar;
	CDialogBar m_wndDialogBar;
	CReBar     m_wndRebar;
	CStaticPosManage m_StaticPosView;
	void	UpdateAllControls();
	C3DDesignPage();
	~C3DDesignPage();

// Dialog Data
	//{{AFX_DATA(C3DDesignPage)
	enum { IDD = IDD_3D_DESIGN_PAGE };
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(C3DDesignPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(C3DDesignPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonRectangle();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DDESIGNPAGE_H__3C105756_9BE9_4AD0_AF2C_89C0BBC9E2B3__INCLUDED_)
