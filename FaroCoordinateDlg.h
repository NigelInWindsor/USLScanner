#if !defined(AFX_FAROCOORDINATEDLG_H__BEA086DA_AEE7_470B_B68F_C83F4C1B792F__INCLUDED_)
#define AFX_FAROCOORDINATEDLG_H__BEA086DA_AEE7_470B_B68F_C83F4C1B792F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FaroCoordinateDlg.h : header file
//
#include "StaticPosManage.h"

/////////////////////////////////////////////////////////////////////////////
// CFaroCoordinateDlg dialog

class CFaroCoordinateDlg : public CDialog
{
// Construction
public:
	int m_nCoordinateAddedCountDown;
	COLORREF	*m_pArray;
	CFont* m_pFont;
	CRect m_rrClient;
	LOGFONT m_LogFont;
	CStaticPosManage m_StaticView;
	CWnd** m_pDlg;
	
	CFaroCoordinateDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFaroCoordinateDlg)
	enum { IDD = IDD_FARO_COORDINATE_DLG };
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFaroCoordinateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFaroCoordinateDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnFarocoordsRightjustify();
	afx_msg void OnFarocoordsLeftjustify();
	afx_msg void OnFarocoordsTextcolour();
	afx_msg void OnFarocoordsBackgroundcolour();
	afx_msg void OnButtonPolar();
	afx_msg void OnButtonRectangle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAROCOORDINATEDLG_H__BEA086DA_AEE7_470B_B68F_C83F4C1B792F__INCLUDED_)
