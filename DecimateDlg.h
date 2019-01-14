#if !defined(AFX_DECIMATEDLG_H__038D84D3_62F6_46FB_8F5E_9CE4CB9F265B__INCLUDED_)
#define AFX_DECIMATEDLG_H__038D84D3_62F6_46FB_8F5E_9CE4CB9F265B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DecimateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDecimateDlg dialog

class CDecimateDlg : public CDialog
{
// Construction
public:
	int m_nMaxPts;
	CDecimateDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDecimateDlg)
	enum { IDD = IDD_DECIMATE_DIALOG };
	CComboBox	m_comboMaxPts;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDecimateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDecimateDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECIMATEDLG_H__038D84D3_62F6_46FB_8F5E_9CE4CB9F265B__INCLUDED_)
