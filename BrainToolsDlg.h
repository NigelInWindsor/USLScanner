#if !defined(AFX_BRAINTOOLSDLG_H__A0398B5F_1204_4381_B23B_AEC142E5CA43__INCLUDED_)
#define AFX_BRAINTOOLSDLG_H__A0398B5F_1204_4381_B23B_AEC142E5CA43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BrainToolsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBrainToolsDlg dialog

class CBrainToolsDlg : public CDialog
{
// Construction
public:
	void UpdateAllControls();
	
	CWnd** m_pDlg;
	CBrainToolsDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBrainToolsDlg)
	enum { IDD = IDD_BRAIN_TOOLS_DIALOG };
	CComboBox	m_comboOrientation;
	CStatic	m_staticSlow;
	CStatic	m_staticFast;
	CComboBox	m_comboMode;
	CEdit	m_editTopMargin;
	CEdit	m_editRightMargin;
	CEdit	m_editLeftMargin;
	CEdit	m_editBottomMargin;
	CEdit	m_editMeshSlowSize;
	CEdit	m_editMeshFastSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrainToolsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBrainToolsDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonMesh();
	afx_msg void OnChangeEditMeshSlowSize();
	afx_msg void OnChangeEditMeshFastSize();
	afx_msg void OnChangeEditTopMargin();
	afx_msg void OnChangeEditBottomMargin();
	afx_msg void OnChangeEditLeftMargin();
	afx_msg void OnChangeEditRightMargin();
	afx_msg void OnSelchangeComboMode();
	afx_msg void OnSelchangeComboOrientation();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BRAINTOOLSDLG_H__A0398B5F_1204_4381_B23B_AEC142E5CA43__INCLUDED_)
