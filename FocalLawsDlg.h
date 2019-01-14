#if !defined(AFX_FOCALLAWSDLG_H__FD4131F6_2774_46F1_93DB_1A2A44A799B8__INCLUDED_)
#define AFX_FOCALLAWSDLG_H__FD4131F6_2774_46F1_93DB_1A2A44A799B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FocalLawsDlg.h : header file
//
#include "StaticPosManage.h"
#include "USLFile.h"

/////////////////////////////////////////////////////////////////////////////
// CFocalLawsDlg dialog

struct ElementStruct {
	int nStructSize;

	CPoint	pt;
	float	fX;
	float	fY;
	float	fH;
	float	fTime;
};


class CFocalLawsDlg : public CDialog
{
// Construction
public:
	ElementStruct	m_el[16];
	float	m_fFocalLength;
	
	void RenderFoalPoints(CDC* pDC,CRect rr);
	void RenderFocalPlane(CDC* pDC,CRect rr);
	void RenderElements(CDC* pDC,CRect rr);
	bool m_bDlgCreated;
	void UpdateAllControls();
	CWnd** m_pDlg;
	CStaticPosManage m_StaticPosView;
	CFocalLawsDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFocalLawsDlg)
	enum { IDD = IDD_FOCAL_LAW_DLG };
	CSpinButtonCtrl	m_spinPitch;
	CSpinButtonCtrl	m_spinNumberElements;
	CEdit	m_editPitch;
	CEdit	m_editNumberElements;
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFocalLawsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFocalLawsDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditPitch();
	afx_msg void OnChangeEditNumberElements();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOCALLAWSDLG_H__FD4131F6_2774_46F1_93DB_1A2A44A799B8__INCLUDED_)
