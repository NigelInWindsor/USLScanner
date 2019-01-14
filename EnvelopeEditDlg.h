#if !defined(AFX_ENVELOPEEDITDLG_H__97008837_E4F5_49AD_AD23_06376464F927__INCLUDED_)
#define AFX_ENVELOPEEDITDLG_H__97008837_E4F5_49AD_AD23_06376464F927__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnvelopeEditDlg.h : header file
//
#include "EnvelopeRectangularDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CEnvelopeEditDlg dialog

class CEnvelopeEditDlg : public CDialog
{
// Construction
public:
	void BlockFill(CPaintDC *pDC, CStatic *mStatic, COLORREF rgbColor);
	CEnvelopeRectangularDlg* m_pEnvelopeRectangularDlg;
	void UpdateParent(int nAction);
	void UpdateAllControls();
	CWnd* m_pParent;
	CWnd** m_pDlg;
	
	int		m_nLeft;
	int		m_nTop;
	CEnvelopeEditDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,int nLeft = 0, int nTop = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEnvelopeEditDlg)
	enum { IDD = IDD_EDITENVELOPELINES_DIALOG };
	CStatic	m_staticColor;
	CEdit	m_editLabel;
	CSpinButtonCtrl	m_spinLine;
	CEdit	m_editLine;
	CButton	m_checkOn;
	CButton	m_checkLines;
	CButton	m_checkCrosses;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnvelopeEditDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEnvelopeEditDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRectilinear();
	afx_msg void OnToolsEnvelope();
	afx_msg void OnChangeEditLine();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditLabel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENVELOPEEDITDLG_H__97008837_E4F5_49AD_AD23_06376464F927__INCLUDED_)
