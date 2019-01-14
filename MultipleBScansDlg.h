//{{AFX_INCLUDES()
#include "uslbar.h"
//}}AFX_INCLUDES
#if !defined(AFX_MULTIPLEBSCANSDLG_H__BD66A8AC_0A2C_4E64_989A_CF5159951559__INCLUDED_)
#define AFX_MULTIPLEBSCANSDLG_H__BD66A8AC_0A2C_4E64_989A_CF5159951559__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MultipleBScansDlg.h : header file
//
#include "StaticPosManage.h"
#include "VolumeData.h"	// Added by ClassView
#include "ScopeDlg.h"
//#include "EnvelopeDetector.h"

/////////////////////////////////////////////////////////////////////////////
// CMultipleBScansDlg dialog

class CMultipleBScansDlg : public CDialog
{
// Construction
public:
	CScopeDlg* m_pScopeDlg;
	bool m_bZoom;
	void UpdateBscanNumber();
	void UpdateWindowName();
	int	m_nMode;
	int m_nLine1;
	int m_nLine0;
	int m_nMaxVertical;
	int m_nMinVertical;
	void CalculateScrollBar();
	CPoint m_pointContext;
	CPoint m_ptRulerFinish;
	CPoint m_ptRulerStart;
	void DrawRuler(CDC *pDC, CRect rr, int nDevice);
	void DrawVerticalGraticule(CDC* pDC,CRect rr,int nDisplay);
	void DrawHorizontalGraticule(CDC* pDC,CRect rr,int nDisplay);
	CPoint m_ptClient;
	void UpdateAllControls();
	int m_nContextOption;
	int m_nBscan;
	void DrawBscan(CDC *pDC, CRect rr, int nDevice);
	CVolumeData m_Data;
	
	CStaticPosManage m_staticPosView;
	CStaticPosManage m_staticScroll;
	CString m_FileName;
	CWnd** m_pDlg;
//	CEnvelopeDetector EnvDetector;
	CMultipleBScansDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,LPCTSTR lpszFileName=NULL,int nMode = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMultipleBScansDlg)
	enum { IDD = IDD_MULTIPLE_BSCANS_DIALOG };
	CButton	m_checkEnvelope;
	CSpinButtonCtrl	m_spinVelocity;
	CEdit	m_editVelocity;
	CEdit	m_editPrompt;
	CComboBox	m_comboRectify;
	CScrollBar	m_scrollBscanNumber;
	CSpinButtonCtrl	m_spinBscanNumber;
	CEdit	m_editBscanNumber;
	CStatic	m_staticView;
	CUSLBar	m_scrollVertical;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultipleBScansDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMultipleBScansDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnChangeEditBscanNumber();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBscanRuler();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBscanColour();
	afx_msg void OnSelchangeComboRctify();
	afx_msg void OnScrollHasMovedUslbarctrlVscroll(long LowPosition, long HighPosition, long Width);
	afx_msg void OnBscanZoom();
	afx_msg void OnChangeEditPrompt();
	afx_msg void OnChangeEditVelocity();
	afx_msg void OnFileSaveas();
	afx_msg void OnViewZoom();
	afx_msg void OnViewRulercolour();
	afx_msg void OnViewOscilloscope();
	afx_msg void OnCheckEnvelope();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIPLEBSCANSDLG_H__BD66A8AC_0A2C_4E64_989A_CF5159951559__INCLUDED_)
