#if !defined(AFX_BISECTMOVEDLG_H__7DE95DEC_8580_4A1F_8BF6_7A3ACE943A89__INCLUDED_)
#define AFX_BISECTMOVEDLG_H__7DE95DEC_8580_4A1F_8BF6_7A3ACE943A89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BisectMoveDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBisectMoveDlg dialog

class CBisectMoveDlg : public CDialog
{
// Construction
public:
	CCoord m_Cp[2];
	int	m_nSide;
	bool	m_nDlgCreated;
	CBisectMoveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBisectMoveDlg)
	enum { IDD = IDD_BISECT_DLG };
	CComboBox	m_comboSide;
	CButton	m_buttonStore1;
	CButton	m_buttonStore0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBisectMoveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBisectMoveDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonStore0();
	afx_msg void OnButtonStore1();
	afx_msg void OnButtonBisect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BISECTMOVEDLG_H__7DE95DEC_8580_4A1F_8BF6_7A3ACE943A89__INCLUDED_)
