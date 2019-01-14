#if !defined(AFX_3DEYEPOINTPAGE_H__DEDEB20C_16C0_4D00_A83E_EC5FE5FF1B10__INCLUDED_)
#define AFX_3DEYEPOINTPAGE_H__DEDEB20C_16C0_4D00_A83E_EC5FE5FF1B10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3DEyePointPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3DEyePointPage dialog

class C3DEyePointPage : public CPropertyPage
{
	DECLARE_DYNCREATE(C3DEyePointPage)

// Construction
public:
	void RenderParent(int nWhich);
	void SetAccessPrivelages();
	void UpdateAllControls();
	
	CPropertySheet* m_pToolSheet;
	CPropertyPage* m_pViewPage;
	C3DEyePointPage(CPropertySheet* pToolSheet = NULL, CPropertyPage* pViewPage = NULL);
	~C3DEyePointPage();

// Dialog Data
	//{{AFX_DATA(C3DEyePointPage)
	enum { IDD = IDD_3D_EYEPOINT_PAGE };
	CComboBox	m_comboCoordinateFrame;
	CComboBox	m_comboUpVector;
	CComboBox	m_comboResolution;
	CComboBox	m_comboCScanOverlayMode;
	CButton	m_checkTurnTable;
	CButton	m_checkFillScanLines;
	CButton m_checkWireFrame;
	CEdit	m_editWallPicture;
	CEdit	m_editRows;
	CEdit	m_editColumns;
	CEdit	m_editViewPt;
	CEdit	m_editEyePt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(C3DEyePointPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(C3DEyePointPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckWireFrame();
	afx_msg void OnButtonImportCscan();
	afx_msg void OnChangeEditRows();
	afx_msg void OnChangeEditColumns();
	afx_msg void OnChangeEditWallPicture();
	afx_msg void OnButtonBrowse();
	afx_msg void OnCheckFillScanLines();
	afx_msg void OnSelchangeComboResolution();
	afx_msg void OnSelchangeComboCscanOverlaymode();
	afx_msg void OnCheckTurntable();
	afx_msg void OnSelchangeComboUpVector();
	afx_msg void OnSelchangeComboCoordinateFrame();
	afx_msg void OnChangeEditEyePt();
	afx_msg void OnChangeEditViewPt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DEYEPOINTPAGE_H__DEDEB20C_16C0_4D00_A83E_EC5FE5FF1B10__INCLUDED_)
