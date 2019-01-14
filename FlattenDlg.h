#if !defined(AFX_FLATTENDLG_H__FB3A9498_A793_42D0_9A5D_C9010937FA77__INCLUDED_)
#define AFX_FLATTENDLG_H__FB3A9498_A793_42D0_9A5D_C9010937FA77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlattenDlg.h : header file
//
#include "StaticPosManage.h"

/////////////////////////////////////////////////////////////////////////////
// CFlattenDlg dialog

class CFlattenDlg : public CDialog
{
// Construction
public:
	bool GenerateSamplePositionVertices();
	void SetToolBarCheckedState();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	CToolBar    m_wndToolsToolBar;
	CReBar		m_wndRebar;
	void RenderSamplePosition(CDC* pDC);
	CStaticPosManage m_StaticPosView;
	CRect m_rrView;
	CData m_Data;
	
	CWnd** m_pDlg;
	CFlattenDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFlattenDlg)
	enum { IDD = IDD_FLATTEN_DLG };
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlattenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFlattenDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnButtonFlattenFromLeft();
	afx_msg void OnButtonFlattenFromCentre();
	afx_msg void OnButtonFlattenFromRight();
	afx_msg void OnButtonPressFlat();
	afx_msg void OnButtonFlattenFromVertical();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLATTENDLG_H__FB3A9498_A793_42D0_9A5D_C9010937FA77__INCLUDED_)
