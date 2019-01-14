#if !defined(AFX_DRAWINGTOOLLSDLG_H__CF232A6C_1B92_4E71_9829_9AEBFEE0FF39__INCLUDED_)
#define AFX_DRAWINGTOOLLSDLG_H__CF232A6C_1B92_4E71_9829_9AEBFEE0FF39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawingToollsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawingToollsDlg dialog

class CDrawingToollsDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CDrawingToollsDlg)
// Construction
public:
	void Update3DEditView(int nAction);
	void UpdateAllControls();
	
	CWnd* m_p3DViewEditDlg;
	CDrawingToollsDlg(CWnd* pParent = NULL,CWnd* p3DViewEditDlg = NULL);   // standard constructor
	~CDrawingToollsDlg();

// Dialog Data
	//{{AFX_DATA(CDrawingToollsDlg)
	enum { IDD = IDD_DRAWINTOOLS_DLG };
	CButton	m_checkMaintainRefPts;
	CButton	m_checkIncludeRefPts;
	CComboBox	m_comboPerspective;
	CButton	m_checkFirstLast;
	CComboBox	m_comboWhichLines;
	CButton	m_checkNormals;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawingToollsDlg)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDrawingToollsDlg)
	afx_msg void OnSelchangeComboWhichLines();
	afx_msg void OnCheckXtip();
	afx_msg void OnCheckYtip();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckFirstlast();
	afx_msg void OnSelchangeComboPerspective();
	afx_msg void OnCheckMaintainAspectRatio();
	afx_msg void OnCheckIncludeRefPts();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGTOOLLSDLG_H__CF232A6C_1B92_4E71_9829_9AEBFEE0FF39__INCLUDED_)
