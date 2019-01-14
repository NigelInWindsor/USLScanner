#if !defined(AFX_POSITIONGRAPHICALPAGE_H__938091D0_B069_4080_B92E_2695CE14BDCF__INCLUDED_)
#define AFX_POSITIONGRAPHICALPAGE_H__938091D0_B069_4080_B92E_2695CE14BDCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PositionGraphicalPage.h : header file
//
#include "StaticPosManage.h"

/////////////////////////////////////////////////////////////////////////////
// CPositionGraphicalPage dialog

class CPositionGraphicalPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPositionGraphicalPage)

// Construction
public:
	void TextOut(CDC* pDC,CString *strName,CString *strPos, CString *strUnit,int *nXp, int *nYp);
	int	m_nOldMeasurementUnits;
	int	m_nLongestNameLength;
	CCoord m_CpHead,m_CpSurface,m_CpOld;
	void RenderStatus(CDC* pDC, CRect *rr);
	void RenderClassic(CDC* pDC, CRect *rr);
	void SetFontSize();
	void SetToolBarCheckedState();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CToolBar   m_wndToolBar;
	CReBar     m_wndRebar;
	COLORREF	*m_pArray;
	CFont* m_pFont;
	CRect m_rrClient;
	LOGFONT m_LogFont;
	bool	m_bDlgCreated;
	CStaticPosManage m_StaticView;
	CPositionGraphicalPage();
	~CPositionGraphicalPage();

// Dialog Data
	//{{AFX_DATA(CPositionGraphicalPage)
	enum { IDD = IDD_POSITION_GRAPHICAL_PAGE };
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPositionGraphicalPage)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPositionGraphicalPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonPositionClassic();
	afx_msg void OnButtonPositionStatus();
	afx_msg void OnButtonPositionMachine();
	afx_msg void OnButtonPositionHead();
	afx_msg void OnButtonPositionSurface();
	afx_msg void OnButtonPositionSurfaceWater();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POSITIONGRAPHICALPAGE_H__938091D0_B069_4080_B92E_2695CE14BDCF__INCLUDED_)
