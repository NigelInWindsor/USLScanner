#if !defined(AFX_PROMPTDLG_H__CB5E3299_B39F_400A_934F_E37B2680E05E__INCLUDED_)
#define AFX_PROMPTDLG_H__CB5E3299_B39F_400A_934F_E37B2680E05E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PromptDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPromptDlg dialog

class CPromptDlg : public CDialog
{
// Construction
public:
	void SetText(CString Buff);
	bool m_bShowVariableRequest;
	CString m_strVariable;
	CString m_Text;
	CString m_WindowName;
	CPromptDlg(CWnd* pParent = NULL,CString Text = L"Hello",CString strWindowName = L"Prompt", bool bShowVariableRequest = false);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPromptDlg)
	enum { IDD = IDD_PROMPT_DIALOG };
	CEdit	m_editVariable;
	CStatic	m_editText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPromptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPromptDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditVariable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROMPTDLG_H__CB5E3299_B39F_400A_934F_E37B2680E05E__INCLUDED_)
