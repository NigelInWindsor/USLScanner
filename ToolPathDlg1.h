#if !defined(AFX_TOOLPATHDLG_H__23770777_2680_4C46_9B6E_F1ABE9F75BDA__INCLUDED_)
#define AFX_TOOLPATHDLG_H__23770777_2680_4C46_9B6E_F1ABE9F75BDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolPathDlg1.h : header file
//

#include "StaticPosManage.h"
/////////////////////////////////////////////////////////////////////////////
// CToolPathDlg dialog

#define	CURRENT	0
#define MIN_XT	1
#define	MAX_XT	2
#define	MIN_YT	3
#define	MAX_YT	4

class CToolPathDlg : public CDialog
{
// Construction
public:
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CToolBar   m_wndToolBar;
	CReBar     m_wndRebar;
	CToolPathDlg(CWnd* pParent = NULL, CWnd** pDlg = NULL);   // standard constructor
	void Cross(CDC *pDC, int nX, int nY,int nSize);
	void DrawMinMax(CDC *pDC, CRect rr);
	void DrawPlots(CDC *pDC, CRect rr);
	COLORREF	*m_pArray;
	CPolyCoord m_Line;
	CRect m_rrGrat;
	int	m_nSample;
	
	bool m_bLButtonDown;
	CPolyCoord m_LocalToolPath[5];
	void CalculateFromScanLine(int ScanLine);
	CWnd** m_pDlg;
	CStaticPosManage m_StaticPosView;

// Dialog Data
	//{{AFX_DATA(CToolPathDlg)
	enum { IDD = IDD_TOOLPATH_DIALOG };
	CStatic	m_staticView;
	CStatic	m_StaticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolPathDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolPathDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonMinMax();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLPATHDLG_H__23770777_2680_4C46_9B6E_F1ABE9F75BDA__INCLUDED_)
