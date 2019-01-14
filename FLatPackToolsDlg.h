#if !defined(AFX_FLATPACKTOOLSDLG_H__3EB4C6ED_2B74_405A_BD06_04F563E09946__INCLUDED_)
#define AFX_FLATPACKTOOLSDLG_H__3EB4C6ED_2B74_405A_BD06_04F563E09946__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FLatPackToolsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFLatPackToolsDlg dialog

class CFLatPackToolsDlg : public CDialog
{
// Construction
public:
	void UpdateAllControls();
	
	CWnd** m_pDlg;
	CFLatPackToolsDlg(CWnd* pParent = NULL, CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFLatPackToolsDlg)
	enum { IDD = IDD_FLATPACK_TOOLS_DLG };
	CEdit	m_editPtsPerLine;
	CEdit	m_editNumberLines;
	CEdit	m_editWaterPathVariation;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFLatPackToolsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFLatPackToolsDlg)
	afx_msg void OnChangeEditDeltaWp();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonWp();
	afx_msg void OnButtonGenerate();
	afx_msg void OnChangeEditPtsPerLine();
	afx_msg void OnChangeEditNumberLines();
	afx_msg void OnButtonNormals();
	afx_msg void OnButtonAllNormals();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLATPACKTOOLSDLG_H__3EB4C6ED_2B74_405A_BD06_04F563E09946__INCLUDED_)
