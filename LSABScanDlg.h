#if !defined(AFX_LSABSCANDLG_H__91C73BF7_9DA3_4E53_9299_839346EC276D__INCLUDED_)
#define AFX_LSABSCANDLG_H__91C73BF7_9DA3_4E53_9299_839346EC276D__INCLUDED_

#include "StaticPosManage.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LSABScanDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLSABScanDlg dialog

class CLSABScanDlg : public CDialog
{
// Construction
public:
	HANDLE m_hSemaphore;
	COLORREF * m_pRGBArray;
	CRect m_rectArray;
	void DrawFrameRate(CDC* pDC, CRect rr);
	void DrawDeltaWp(CDC* pDC, CRect rr);
	void SetScalePosition();
	void SetToolBarCheckedState();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CToolBar   m_wndToolBar;
	CReBar     m_wndRebar;
	CLSABScanDlg(CWnd* pParent = NULL,  CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLSABScanDlg)
	enum { IDD = IDD_LSA_BSCAN_DIALOG };
	CStatic	m_staticView;
	CStatic	m_staticScale;
	//}}AFX_DATA

	void CalculatePalette();

	void DrawGates(CDC* pDC,CRect rr);
	void DrawBScan();

	CStaticPosManage m_StaticPosView;
	CStaticPosManage m_StaticPosScale;

	char m_cColor[256][4];

	bool m_bDisplayGates;
	bool m_bDisplayTOF;
	int m_nRectification;
	CWnd** m_pDlg;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLSABScanDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLSABScanDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonLsaHighlight();
	afx_msg void OnButtonLsaInvert();
	afx_msg void OnButtonLsaFullwave();
	afx_msg void OnButtonLsaPositiveHw();
	afx_msg void OnButtonLsaNegativeHw();
	afx_msg void OnButtonLsaGates();
	afx_msg void OnButtonLsaSmooth();
	afx_msg void OnButtonLsaTof();
	afx_msg void OnButtonLsaScale();
	afx_msg void OnButtonDeltaWp();
	afx_msg void OnButtonFrameRate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LSABSCANDLG_H__91C73BF7_9DA3_4E53_9299_839346EC276D__INCLUDED_)
