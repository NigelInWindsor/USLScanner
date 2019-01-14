#if !defined(AFX_IGESDLG_H__11AA274B_8F04_49CD_B2A9_759E06EFFF75__INCLUDED_)
#define AFX_IGESDLG_H__11AA274B_8F04_49CD_B2A9_759E06EFFF75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IGESDlg.h : header file
//
#include "IGES.h"

/////////////////////////////////////////////////////////////////////////////
// CIGESDlg dialog

class CIGESDlg : public CDialog
{
// Construction
public:
	void WorldToClient(float fX,float fY,float fZ,CRect rr,CPoint *pt);
	bool RenderEntityRationalBSpline(CPaintDC* pDC,CRect rr,EntityStruct *pEntity);
	bool RenderEntityConicArc(CPaintDC* pDC,CRect rr,EntityStruct *pEntity);
	bool RenderEntityCircleArc(CPaintDC* pDC,CRect rr,EntityStruct *pEntity);
	bool RenderEntityLine(CPaintDC* pDC,CRect rr,EntityStruct *pEntity);
	bool RenderEntityPoint(CPaintDC* pDC,CRect rr,EntityStruct *pEntity);
	void RenderHeader(CPaintDC* pDC,CRect rr,char cHeaderType);
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	void SetToolBarCheckedState();
	void FillList();
	void UpdateAllControls();
	void CreateColumns();

	D3DXVECTOR3 vecMin;
	D3DXVECTOR3 vecMax;
	CIGES m_IGES;
	int m_nIndex;
	int	m_nPaintOption;
	CToolBar   m_wndToolBar;
	CReBar     m_wndRebar;
	
	CWnd** m_pDlg;
	CIGESDlg(CWnd* pParent = NULL,CWnd** pDlg=NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIGESDlg)
	enum { IDD = IDD_IGES_DIALOG };
	CEdit	m_editParam;
	CEdit	m_editData;
	CStatic	m_staticView;
	CListCtrl	m_listEntity;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIGESDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIGESDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetdispinfoListEntity(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListEntity(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonDrawing();
	afx_msg void OnButtonGlobal();
	afx_msg void OnButtonStart();
	afx_msg void OnFileOpen();
	afx_msg void OnKeydownListEntity(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IGESDLG_H__11AA274B_8F04_49CD_B2A9_759E06EFFF75__INCLUDED_)
