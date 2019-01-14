#if !defined(AFX_3DVIEWEDITDLG_H__FADF5846_45EB_4403_A6BF_1B018D9CB3D8__INCLUDED_)
#define AFX_3DVIEWEDITDLG_H__FADF5846_45EB_4403_A6BF_1B018D9CB3D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3DViewEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3DViewEditDlg dialog
#include "StaticPosManage.h"
#include "EditSpinItem.h"
#include "TranslationDlg.h"
#include <d3d9.h>
#include <d3dx9math.h>
#include "BrainEditDlg.h"
#include "HolesEditDlg.h"
#include "ScanLinesEditDlg.h"
#include "TaughtLinesEditDlg.h"
#include "EnvelopeEditDlg.h"
#include "HoldingFixtureEditDlg.h"
#include "PolyCoord.h"	// Added by ClassView
#include "Coord.h"	// Added by ClassView
#include "D3DFont.h"
#include "WayPointsDlg.h"
#include "3DObject.h"
#include "NonCadVariablesDlg.h"
#include "ToolPathDlg1.h"
#include "PolyCoordDlg1.h"
#include "PolyMeshCoord.h"
#include "TaughtToolsDlg.h"
#include "WayLinesEditDlg.h"
#include "EditBladeDlg.h"
#include "CircumcentreDlg.h"
#include "USLMFCToolBar.h"

#include "ResizablePage.h"



#define	FILLLIST	0x01
#define	PAINT		0x02
#define	INVALIDATE	0x04

#define	_MAXLIGHTS 7

#define	PRIM_TANK			0
#define	PRIM_SCANLINES		1
#define	PRIM_DANGERZONE		2
#define	PRIM_CSCAN			3
#define PRIM_ISOMETRIC		4
#define	PRIM_ROTORY_COMP	5
#define	PRIM_TURNTABLE		6
#define	PRIM_MESH			7

//#define MAXLIGHTS 7

class C3DViewEditDlg : public CResizablePage
{
// Construction
	DECLARE_DYNCREATE(C3DViewEditDlg)

public:
	long FillListFromChild(unsigned int nT, long nL);
	CFont* m_pFont;
	LOGFONT m_lf;
	
	CCoord m_CpComponent[3];
	CCoord m_CpMeasured[3];

	HCURSOR	m_hcursorFinger;
public:
	bool m_bDisplayIsometric;
	int m_nIsometricImageNumber;
	int	m_nINormal;
	int	m_nJNormal;
	int	m_nKNormal;
	bool m_bDisplayFrame;
	COLORREF	*m_pArray;
	int	m_nNormal;
	CPoint m_ptClient;
	float m_fX0p;
	float m_fX1p;
	float m_fY0p;
	float m_fY1p;
	float m_fZ0p;
	float m_fZ1p;
	float m_fI0p;
	float m_fI1p;
	float m_fJ0p;
	float m_fJ1p;
	float m_fK0p;
	float m_fK1p;
	float m_fRotatePtX;
	float m_fRotatePtY;
	float m_fRotatePtZ;
	int m_nXRotate;
	int m_nYRotate;
	int m_nZRotate;
	float m_fXRotate;
	float m_fYRotate;
	float m_fZRotate;

	int	m_nDisplayFastAxis;
	int	m_nDisplaySlowAxis;

	D3DXVECTOR3 m_vLookatPt;
	D3DXVECTOR3 m_vUpVec;
	D3DXVECTOR3 m_vEyePt;
	D3DXMATRIXA16 m_matWorld;
	CD3DArcBall_10   m_WorldBall;
	CD3DArcBall_10	m_CompBall;

	CPoint m_ptDown;
	bool	m_bWallPicture;
	bool m_bLBDown;
	bool m_bRBDown;
	bool m_bMBDown;
	int m_nProjectionMoveOption;
	int	m_n3DMoveEyeComponent;


	bool					m_bWireframe;
	bool					m_b3DCScan;
	bool					m_b3DDisplayScanLines;

	int						m_nNextPrim[20];
	int						m_nPrimL[20];
	PrimitiveData*			m_pPrim[20];

	
	int m_nPaintMode;
	/*
	CToolBar   m_wndCoordinateToolBar;
	CToolBar   m_wnd3DToolBar;
	CToolBar   m_wndFileToolBar;
	CToolBar   m_wndViewToolBar;
	CToolBar   m_wndEditToolBar;
	CToolBar	m_wndBladeToolBar;
	*/
	CUSLMFCToolBar   m_wndCoordinateToolBar;
	CUSLMFCToolBar   m_wnd3DToolBar;
	CUSLMFCToolBar   m_wndFileToolBar;
	CUSLMFCToolBar   m_wndViewToolBar;
	CUSLMFCToolBar   m_wndEditToolBar;
	CUSLMFCToolBar	m_wndBladeToolBar;

	CDialogBar m_wndDialogBar;
	CReBar     m_wndRebar;
	CEditSpinItem* m_pEditSpinItem;
	CStaticPosManage m_StaticPosView;
	CStaticPosManage m_StaticPosList;
	CTranslationDlg* m_pTranslationDlg;
	CWnd** m_pDlg;
	
	CToolPathDlg *m_ToolsPathDlg;
	CPolyCoordDlg* m_pPolyCoordDlg;
	CTaughtToolsDlg* m_pTaughtTools;
	CCircumcentreDlg* m_pCircumcenterDlg;


public:
	void AddInsertCoordinate(int nAddInsert);
	void DeleteCoordinate();
	void DrawPerspectiveArrows(CPaintDC* pDC,CRect rr);
	bool IsCoordinateValid();
	void ChangeLimitsForAspectRatio(CRect rrView);
	void Move(int x,int y);
	void ColumnclickListCoords();
	void SetAccessPrivelage();
	bool m_bDrawWayPts;
	void UpdatAll3DDependents();
	void ComponentMove();
	void GetVolumeTrace(int nSample, int nLine, char * pTrace, CData * pData, CUSLFile * pFile);
	float m_fEditInitialValue;
	int	m_nEditAxis;
	void DrawXtipYTipGraticule(CDC* pDC,CRect rr);
	CCoord m_CpSelectArea[2];
	bool m_bSelectArea;
	bool m_bModified;
	void SetWindowTitle();
	void FindAllLimits(bool bCenter);
	void GetDispInfoListCoordsBladeLines(LV_DISPINFO* pDispInfo,WCHAR *str);
	void GetDispInfoListCoordsBrainLines(LV_DISPINFO* pDispInfo,WCHAR *str);
	void GetDispInfoListCoordsEditLines(LV_DISPINFO* pDispInfo,WCHAR *str);
	void GetDispInfoListHoles(LV_DISPINFO* pDispInfo,WCHAR *str);
	bool m_bDisplayDangerZone;
	HBRUSH m_hBr;
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	void OpenOnLastExit();
	void InitializeEditLine();
	void SaveThumbNail(CUSLFile* pFile);
	void InvalidateAllChildren();
	void CloseAllChildren();
	CEditBladeDlg* m_pBladeEditDlg;
	CBrainEditDlg* m_pBrainEditDlg;
	CHolesEditDlg* m_pHolesEditDlg;
	CScanLinesEditDlg* m_pScanLinesEditDlg;
	CTaughtLinesEditDlg* m_pTaughtLinesEditDlg;
	CEnvelopeEditDlg* m_pEnvelopeLinesEditDlg;
	CHoldingFixtureEditDlg* m_pHoldingFixtureEditDlg;
	CWayPointsDlg* m_pWayPointsDlg;
	CNonCadVariablesDlg* m_pNonCadVariablesDlg;
	CWayLinesEditDlg* m_pWayLinesEditDlg;

	long ItemChanged(unsigned int nTemp,long nlTemp);
	int m_nEditLineL;
	CPolyCoord* m_pEditLine;
	int			m_nMaxLine;
	int			m_nMinLine;
	int			m_nToolBarStyles;
	float		m_fXAngle;
	float		m_fYAngle;
	float		m_fZAngle;
	float		m_fLength;
	CPointFloat m_ptZoom;
	CCoord		m_CpZoom;
	float		m_fZoomLevel;


	long SetToolBarCheckedState(unsigned int nTemp = 0, long nlTemp = 0);
	int m_nEditLinesEnvelope;
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	void RefreshView();
	void ClientToWorld(CPoint* pt, CRect rr, COORDDATA *pCp);
	void WorldToClient(COORDDATA *pCp, CRect rr, CPoint *pt,int nSide = PORTSIDE);
	void WorldToClient(COORDDATA *pCp, CRectFloat rr, CPoint *pt,int nSide = PORTSIDE);
	void DrawWayPoints(CPaintDC *pDC, CRect rr, int nDevice);
	void DrawSelectedArea(CPaintDC *pDC, CRect rr, int nDevice);
	void DrawProjectedSurfaceInGreyMode(CPaintDC *pDC, CRect Rect, int nDevice);
	void DrawProjectedWaterJets(CPaintDC *pDC, CRect rr, int nDevice);
	void DrawProjectedHoles(CPaintDC *pDC, CRect rr, int nDevice);
	void DrawProjectedLine(CPolyCoord *pLine,CDC* pDC,CRect rr,bool bJoinUpLastPoint=FALSE);
	void DrawProjectedLines(CPaintDC *pDC, CRect rr, int nDevice,int nWhichLines);
	void DrawNormals(CPolyCoord *pLine,CDC* pDC,CRect rr);
	void ClearView(CPaintDC* pDC,CRect rr,int nDevice);
	void UpdateAllControls();
	void CreateColumns();
	void FillList();
//	C3DViewEditDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor
	C3DViewEditDlg(int nToolBarStyles = 0);
	~C3DViewEditDlg();

// Dialog Data
	//{{AFX_DATA(C3DViewEditDlg)
	enum { IDD = IDD_3D_EDIT_DIALOG };
	CEdit	m_editMaxLine;
	CEdit	m_editMinLine;
	CEdit	m_editGeneral;
	CButton	m_checkMathNormal;
	CButton	m_checkKNormal;
	CButton	m_checkJNormal;
	CButton	m_checkINormal;
	CStatic	m_staticView;
	CSpinButtonCtrl	m_spinLine;
	CMFCListCtrl	m_listCoord;
	CEdit	m_editLine;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3DViewEditDlg)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(C3DViewEditDlg)
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKickIdle();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditLine();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditScanlines();
	afx_msg void OnEditenvelopelines();
	afx_msg void OnRclickListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnList3dGotoLeft();
	afx_msg void OnList3dGotoRight();
	afx_msg void OnList3dGotoBoth();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnEditBrainLines();
	afx_msg void OnUpdateEditBrainLines(CCmdUI* pCmdUI);
	afx_msg void OnEditHoles();
	afx_msg void OnUpdateEditHoles(CCmdUI* pCmdUI);
	afx_msg void OnToolsCopy();
	afx_msg void OnEditTaughtLines();
	afx_msg void OnList3dDelete();
	afx_msg void OnToolsEnvelope();
	afx_msg void On3dviewDeletecoordinate();
	afx_msg void On3dviewDeleteline();
	afx_msg void On3dviewCopyline();
	afx_msg void On3dviewPasteline();
	afx_msg void On3dviewDecimateline();
	afx_msg void OnList3dDecimate();
	afx_msg void OnEditTransform();
	afx_msg void OnList3dCopy();
	afx_msg void OnList3dPaste();
	afx_msg void OnEditHoldingfixture();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg void OnToolsInvertallnormals();
	afx_msg void OnFileNew();
	afx_msg void OnButtonAddCoordinate();
	afx_msg void OnButtonNewLine();
	afx_msg void OnToolsTaughtlines();
	afx_msg void OnButtonDeleteCoordinate();
	afx_msg void OnList3dEdit();
	afx_msg void OnList3dTranslate();
	afx_msg void OnList3dModifyleft();
	afx_msg void OnList3dModifyright();
	afx_msg void OnList3dModifyboth();
	afx_msg void OnEnvelopeTranslate();
	afx_msg void OnList3dBisect();
	afx_msg void OnHolesStore();
	afx_msg void OnHolesGoto();
	afx_msg void OnHolesSwap0();
	afx_msg void OnHolesSwap1();
	afx_msg void OnHolesInvertnormal();
	afx_msg void OnBrainModifydeltas();
	afx_msg void OnBrainGotoNoBrain();
	afx_msg void OnBrainGotobrain();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnList3dDeleteline();
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnViewIges();
	afx_msg void OnViewLines();
	afx_msg void OnViewCrosses();
	afx_msg void OnButtonSelectarea();
	afx_msg void OnAreaDelete();
	afx_msg void OnAreaDeleteallscanlines();
	afx_msg void OnEnvelopePaste();
	afx_msg void On3dviewDecimatealllines();
	afx_msg void On3dviewSortalllines();
	afx_msg void OnBrainResetalldeltas();
	afx_msg void OnViewZoomIn();
	afx_msg void OnViewZoomOut();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnBrainResetleftdeltas();
	afx_msg void OnBrainResetrightdeltas();
	afx_msg void OnBrainCopy();
	afx_msg void OnBrainPaste();
	afx_msg void OnBrainResetijk();
	afx_msg void OnList3dCalculatextangle();
	afx_msg void OnList3dStraiteline();
	afx_msg void OnToolsSinewavegenerator();
	afx_msg void OnList3dModifyijk();
	afx_msg void OnList3dOptomise();
	afx_msg void OnBrainAddtobrainline();
	afx_msg void On3dviewReorderlines();
	afx_msg void On3dview3dlookatthispoint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnToolsWaypoints();
	afx_msg void OnEditWaypoints();
	afx_msg void OnToolsUntwist();
	afx_msg void OnHeaderRaxis();
	afx_msg void OnHeaderEndofline();
	afx_msg void OnHeaderGain();
	afx_msg void OnHeaderWaterpathleft();
	afx_msg void OnHeaderWaterpathright();
	afx_msg void OnHeaderInterfacedelay();
	afx_msg void OnHeaderInterfacewidth();
	afx_msg void OnHeaderGate1delay();
	afx_msg void OnHeaderGate1width();
	afx_msg void OnHeaderGate2delay();
	afx_msg void OnHeaderGate2width();
	afx_msg void OnTankModifyut();
	afx_msg void OnList3dNoncadvariables();
	afx_msg void OnBrainDelete();
	afx_msg void OnTankModifyxyzijk();
	afx_msg void OnBrainNewbraneline();
	afx_msg void OnBrainSortbranelines();
	afx_msg void OnBrainPasteline();
	afx_msg void OnViewRThetaPath();
	afx_msg void OnHolesGototipangles();
	afx_msg void OnBrainDeleteLine();
	afx_msg void OnList3dDivideinto2lines();
	afx_msg void OnList3dExpandline();
	afx_msg void OnEnvelopeBisect();
	afx_msg void OnSurfaceSendtotaughtlines();
	afx_msg void OnSurfaceCopyline();
	afx_msg void OnEditSurfaceLines();
	afx_msg void On3dviewMovelasertothispoint();
	afx_msg void OnButtonSurfaceGenerator();
	afx_msg void OnDeletePrecedingPoints();
	afx_msg void OnDeleteFollowingPoints();
	afx_msg void OnHolesCopyallcadmeasured();
	afx_msg void OnHolesPasteallcadmeasured();
	afx_msg void OnBrainSendtoscanlines();
	afx_msg void OnBrainSendalltoscanlines();
	afx_msg void OnHoles3dlookatthispoint();
	afx_msg void OnViewXy();
	afx_msg void OnViewXz();
	afx_msg void OnViewYz();
	afx_msg void OnButtonInsertCoordinate();
	afx_msg void On3dviewSortX();
	afx_msg void On3dviewSortY();
	afx_msg void On3dviewSortZ();
	afx_msg void OnButtonEditInside();
	afx_msg void OnButtonEditOutside();
	afx_msg void On3dviewBisectaddlinebetween();
	afx_msg void OnList3dCircumcenterbetween();
	afx_msg void OnEnvelopeAdd();
	afx_msg void OnEnvelopeDelete();
	afx_msg void OnEnvelopeMove();
	afx_msg void OnEnvelopeTaborder();
	afx_msg void OnHolesImportcadptsfromtextfile();
	afx_msg void OnHolesGuesscadnorms();
	afx_msg void OnHolesGotoViadangerplane();
	afx_msg void OnList3dModifynormalsonnearest();
	afx_msg void OnList3dPastenormals();
	afx_msg void OnKeydownListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void On3dviewComputeoppositeside();
	afx_msg void On3dviewAllCopyalllines();
	afx_msg void On3dviewAllPastealllines();
	afx_msg void On3dviewAllDeletealllines();
	afx_msg void On3dviewAllDecimatealllines();
	bool IsCtrlPressed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

private:
	bool m_bCtrlKeyPressed = false;
	bool m_bAKeyPressed = false;
	bool m_bDKeyPressed = false;
	bool m_bNKeyPressed = false;

	union KeyState {
		LPARAM lparam;

		struct {
			WORD nRepeatCount : 16;
			BYTE nScanCode : 8;
			BYTE nExtended : 1;
			BYTE nReserved : 4;
			BYTE nContext : 1;
			BYTE nPrevious : 1;
			BYTE nTransition : 1;
		};


		struct {
			WORD nRepeatCount : 16;
			WORD nFlags : 16;
		};

	};

	KeyState m_keyState;

	void TranslateKeydown(MSG* pMsg);
	void InvalidateCoordArrayPane(int nFlag);
public:
	afx_msg void On3dviewRecenterimage();
	void Centerimage();
	afx_msg void On3dviewProperties();
	afx_msg void OnUpdateViewLines(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewCrosses(CCmdUI *pCmdUI);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DVIEWEDITDLG_H__FADF5846_45EB_4403_A6BF_1B018D9CB3D8__INCLUDED_)
