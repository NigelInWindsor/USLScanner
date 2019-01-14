#if !defined(AFX_DETAILSPAGE_H__6D7CCE8A_1FC4_41F0_BB4F_B024488F1AB9__INCLUDED_)
#define AFX_DETAILSPAGE_H__6D7CCE8A_1FC4_41F0_BB4F_B024488F1AB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DetailsPage.h : header file
//

#include "CtrlPropertiesDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDetailsPage dialog

class CDetailsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDetailsPage)

// Construction
public:
	void TopAlign();
	void SpaceVertically();
	void SameHeight();
	void SameWidth();
	void RightAlign();
	void LeftAlign();
	void SetAccessPrivelages();
	void ChangeOrder(int nSrc,int nDest);
	void OnTabOrder();
	CCtrlPropertiesDlg* m_pCtrlPropertiesDlg;
	CRect m_rrDrag;
	CPoint	m_ptButtonDown;
	bool m_bLButtonDown;
	HCURSOR m_hcursorVMove;
	HCURSOR m_hcursorHMove;
	HCURSOR m_hcursorAllMove;
	void DrawDrag(CPaintDC *pDC);
	void DrawCtrlHighLight(CPaintDC* pDC,CRect rr,COLORREF rgbColor);
	void AddCtrl(CtrlType nType);
	CToolBar m_wndCtrlToolBar;
	CReBar     m_wndRebar;
	void FindNearestCtrl();
	void Zero();
	CButton	*m_pButton;
	CEdit *m_pEdit;
	CStatic *m_pStatic;
	void DisplayControls(CDC* pDC);
	void UpdateAllControls();
	
	bool m_bDesignDlg;
	bool m_bCtrlMoved;
	bool m_bTabOrder;
	bool m_bShiftKeyDown;
	int	m_nWhichEdge;
	int	m_nTabNumber;
	int	m_nIndex;
	CRect m_rrIndex;
	CData* m_pData;
	CWnd** m_pDlg;
	CDetailsPage(CWnd** pDlg = NULL,CData* pData=NULL,bool bDesignDlg = FALSE);
	~CDetailsPage();

// Dialog Data
	//{{AFX_DATA(CDetailsPage)
	enum { IDD = IDD_DETAILS_PAGE };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDetailsPage)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDetailsPage)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCut();
	afx_msg void OnEditProperties();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DETAILSPAGE_H__6D7CCE8A_1FC4_41F0_BB4F_B024488F1AB9__INCLUDED_)
