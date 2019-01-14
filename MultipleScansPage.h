#if !defined(AFX_MULTIPLESCANSPAGE_H__AF5FDC7A_2F0C_481F_A799_814A029BA425__INCLUDED_)
#define AFX_MULTIPLESCANSPAGE_H__AF5FDC7A_2F0C_481F_A799_814A029BA425__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MultipleScansPage.h : header file
//
#include "USLScans.h"
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CMultipleScansPage dialog
struct	ItemScansData {
	HTREEITEM hItem;
	HTREEITEM hUniqueDescription;
	HTREEITEM hOperator;
	HTREEITEM hProfileName;
};

class CMultipleScansPage : public CResizablePage
{
	DECLARE_DYNCREATE(CMultipleScansPage)

// Construction
public:
	CImageList			m_imageList;
	CBitmap				m_bitmap[10];
	ItemScansData*		m_pScansItem;

	void FillScansTree();
	bool AddToUSLScans(CString *pFilePath);
	int	m_nUSLScansL;
	CUSLScans *m_pUSLScans;
	bool m_bLbDown;
	CPoint m_ptLbDown;
	bool m_bBorderSelected;
	CRect m_rrVBorder;
	void InvalidateResizeableDlgs();
	
	CMultipleScansPage();
	~CMultipleScansPage();

// Dialog Data
	//{{AFX_DATA(CMultipleScansPage)
	enum { IDD = IDD_MULTIPLE_SCANS_PAGE };
	CStatic	m_staticView;
	CTreeCtrl	m_treeScans;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMultipleScansPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMultipleScansPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRclickScansTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIPLESCANSPAGE_H__AF5FDC7A_2F0C_481F_A799_814A029BA425__INCLUDED_)
