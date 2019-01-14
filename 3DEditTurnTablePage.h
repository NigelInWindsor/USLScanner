#if !defined(AFX_3DEDITTURNTABLEPAGE_H__ACCE3D99_2AA8_46AA_99DE_DAA6447DA6C3__INCLUDED_)
#define AFX_3DEDITTURNTABLEPAGE_H__ACCE3D99_2AA8_46AA_99DE_DAA6447DA6C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3DEditTurnTablePage.h : header file
//
#include "StaticPosManage.h"
#include "EditSpinItem.h"
#include "Coord.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// C3DEditTurnTablePage dialog

class C3DEditTurnTablePage : public CPropertyPage
{
	DECLARE_DYNCREATE(C3DEditTurnTablePage)

// Construction
public:
	void DrawPerspectiveArrows(CPaintDC *pDC, CRect rr);
	void CalculateRadius(CCoord *pCp);
	void UpdateMainFrameTools();
	void TurntableGoto(int nSide);
	void GotoPolarCoord(int nMoveSide);
	void OnGetdispinfoListCoordsCartesian(NMHDR* pNMHDR, LRESULT* pResult);
	void OnGetdispinfoListCoordsPolar(NMHDR* pNMHDR, LRESULT* pResult);
	void SetAccessPrivelage();
	void GenerateScanLines();
	void ClientToWorld(CPoint* pt, CRect rr, COORDDATA *pCp);
	void WorldToClient(COORDDATA *pCp, CRect rr, CPoint *pt,int nSide = PORTSIDE);
	void WorldToClient(COORDDATA *pCp, CRectFloat rr, CPoint *pt,int nSide = PORTSIDE);
	void DrawProjectedWaterJets(CPaintDC *pDC, CRect rr, int nDevice);
	void DrawProjectedLine(CPolyCoord *pLine,CDC* pDC,CRect rr,bool bJoinUpLastPoint=FALSE,int nSide = PORTSIDE);
	void DrawProjectedLines(CPaintDC *pDC, CRect rr, int nDevice,int nWhichLines);
	void ChangeLimitsForAspectRatio(CRect rr);
	void FindAllLimits();
	void CloseEditSpin();
	void InitializeEditLine();
	void FillList();
	void CreateColumns();
	void UpdateAllControls();
	long SetToolBarCheckedState(unsigned int nTemp = 0, long nlTemp = 0);
	void DrawPolarPosition(CDC* pDC,CRect rr);
	void DrawEccentric(CDC* pDC,CRect rr);
	void DrawCircularGraticule(CDC* pDC,CRect rr);
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);

	CToolBar   m_wndCoordinateToolBar;
	CToolBar   m_wndFileToolBar;
	CToolBar   m_wnd3DToolBar;
	CToolBar   m_wndViewToolBar;
	CToolBar   m_wndMotionToolBar;
	CDialogBar m_wndDialogBar;
	CReBar     m_wndRebar;
	HBRUSH		m_hBr;

	CString m_Style[4];
	CString	m_Action[3];
	CCoord m_CpSurface;
	CStaticPosManage m_StaticPosView;
	CStaticPosManage m_StaticPosList;
	CEditSpinItem* m_pEditSpinItem;
	CPolyCoord* m_pEditLine;
	COLORREF	*m_pArray;
	CPoint		m_ptClient;
	CPoint		m_ptDown;
	float		m_fZoomLeft;
	float		m_fZoomTop;
	float		m_fX0p;
	float		m_fX1p;
	float		m_fY0p;
	float		m_fY1p;
	float		m_fZ0p;
	float		m_fZ1p;
	float		m_fI0p;
	float		m_fI1p;
	float		m_fJ0p;
	float		m_fJ1p;
	float		m_fK0p;
	float		m_fK1p;
	float		m_fEdit;
	float		m_fEditInitialValue;
	float		m_fR0p;
	float		m_fR1p;
	int			m_nEdit;
	int			m_nEditInitialValue;
	int			m_nZoomLevel;
	int			m_nRunningDir;
	int			m_nEditLineL;
	int			m_nEditType;
	int			m_nPaintMode;
	int			m_nColumn;
	int			m_nEditWhich;
	bool		m_bLBDown;

	C3DEditTurnTablePage();
	~C3DEditTurnTablePage();

// Dialog Data
	//{{AFX_DATA(C3DEditTurnTablePage)
	enum { IDD = IDD_3D_TURNTABLE_PAGE };
	CEdit	m_editTTCentre;
	CComboBox	m_comboAlgorithm;
	CSpinButtonCtrl	m_spinSlowIncrement;
	CEdit	m_editSlowIncrement;
	CButton	m_checkZeroDegreeRotate;
	CComboBox	m_comboDescription;
	CSpinButtonCtrl	m_spinLineNumber;
	CEdit	m_editLineNumber;
	CComboBox	m_comboTimeSlot;
	CMFCListCtrl	m_listCoords;
	CEdit	m_editRPos;
	CEdit	m_editNominalDiameter;
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(C3DEditTurnTablePage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(C3DEditTurnTablePage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnKickIdle();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonCwMove();
	afx_msg void OnButtonCcwMove();
	afx_msg void OnDeltaposSpinRotSpeedDegSec(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditNomDiameter();
	afx_msg void OnDeltaposSpinNomDiameter(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonZero();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HRESULT ItemChanged(WPARAM, LPARAM);
	afx_msg void OnRclickListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnViewCrosses();
	afx_msg void OnViewLines();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonCartesian();
	afx_msg void OnButtonPolar();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTurntableGotoleft();
	afx_msg void OnTurntableGotoright();
	afx_msg void OnTurntableGotoboth();
	afx_msg void OnSelchangeComboTimeslot();
	afx_msg void OnButtonEccentric();
	afx_msg void OnTtpolarRotatetonearestpoint();
	afx_msg void OnTtpolarRotatetofurthestpoint();
	afx_msg void OnTtpolarRotatetothisangle();
	afx_msg void OnTtlistAdd();
	afx_msg void OnTtlistDelete();
	afx_msg void OnTtpolarRotatetonominal();
	afx_msg void OnTtlistTranslateverticallytohere();
	afx_msg void OnTtlistInsert();
	afx_msg void OnButtonTaughtLine();
	afx_msg void OnButtonScanLine();
	afx_msg void OnTtlistSortinz();
	afx_msg void OnTtlistModify();
	afx_msg void OnButtonRightToLeft();
	afx_msg void OnChangeEditLineNumber();
	afx_msg void OnDeltaposSpinLineNumber(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTtlistNewline();
	afx_msg void OnTtlistCopyline();
	afx_msg void OnTtlistPasteline();
	afx_msg void OnButtonCartesianPolar();
	afx_msg void OnTtlistComputeandfillpolarvalues();
	afx_msg void OnTtlistGotoleftpolarcoord();
	afx_msg void OnTtlistGotorightpolarcoord();
	afx_msg void OnTtlistGotobothpolarcoord();
	afx_msg void OnSelchangeComboDescription();
	afx_msg void OnButtonGotoZeroDegrees();
	afx_msg void OnCheckZeroDegree();
	afx_msg void OnTtlistSortiny();
	afx_msg void OnChangeEditSlowIncrement();
	afx_msg void OnDeltaposSpinSlowIncrement(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTtlistCopy();
	afx_msg void OnViewXy();
	afx_msg void OnViewXz();
	afx_msg void OnViewYz();
	afx_msg void OnSelchangeComboAlgorithm();
	afx_msg void OnChangeEditTtCentre();
	afx_msg void OnTtlist3dlookatthispoint();
	afx_msg void OnTtlist3dlookatttcenter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTtSortinx();
	CEdit m_editDiameter;
	CSpinButtonCtrl m_spinDiameter;
	afx_msg void OnEnChangeEditDiameter();
	afx_msg void OnDeltaposSpinDiameter(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DEDITTURNTABLEPAGE_H__ACCE3D99_2AA8_46AA_99DE_DAA6447DA6C3__INCLUDED_)
