#if !defined(AFX_TESSELLATIONPAGE_H__46B15619_A3B5_4884_9EDE_DA5DA7BA9F12__INCLUDED_)
#define AFX_TESSELLATIONPAGE_H__46B15619_A3B5_4884_9EDE_DA5DA7BA9F12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TessellationPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTessellationPage dialog

class CTessellationPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTessellationPage)

// Construction
public:
	int	m_nIndex;
	int m_nOrder;
	bool m_bCtrlButton;
	bool m_bShiftButton;
	void Update3DView();
	void FillList();
	void CreateColumns();
	void UpdateAllControls();
	
	void RenderParent();
	CPropertyPage* m_pPage;
	CPropertySheet *m_pSheet;
	CTessellationPage(CPropertySheet* pToolSheet = NULL, CPropertyPage* pViewPage = NULL);
	~CTessellationPage();

// Dialog Data
	//{{AFX_DATA(CTessellationPage)
	enum { IDD = IDD_TESSELLATION_PAGE };
	CListCtrl	m_listEdges;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTessellationPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTessellationPage)
	afx_msg void OnButtonAutomatic();
	virtual BOOL OnInitDialog();
	afx_msg void OnGetdispinfoListEdges(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListEdges(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListEdges(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTessellationDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESSELLATIONPAGE_H__46B15619_A3B5_4884_9EDE_DA5DA7BA9F12__INCLUDED_)
