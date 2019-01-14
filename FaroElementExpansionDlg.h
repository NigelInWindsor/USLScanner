#if !defined(AFX_FAROELEMENTEXPANSIONDLG_H__2487CAF6_4B69_4BBE_B968_28A0D97A9252__INCLUDED_)
#define AFX_FAROELEMENTEXPANSIONDLG_H__2487CAF6_4B69_4BBE_B968_28A0D97A9252__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FaroElementExpansionDlg.h : header file
//
#include "StaticPosManage.h"
#include "EditColour.h"

/////////////////////////////////////////////////////////////////////////////
// CFaroElementExpansionDlg dialog

class CFaroElementExpansionDlg : public CDialog
{
// Construction
public:
	void UpdateAllControls();
	int	m_nElementIndex;
	CStaticPosManage m_StaticText;
	CWnd** m_pDlg;
	
	CFaroElementExpansionDlg(CWnd* pParent = NULL, CWnd** pDlg = NULL, int nElementIndex = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFaroElementExpansionDlg)
	enum { IDD = IDD_FARO_ELEMENT_EXPANSION_DLG };
	CEdit	m_editText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFaroElementExpansionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFaroElementExpansionDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAROELEMENTEXPANSIONDLG_H__2487CAF6_4B69_4BBE_B968_28A0D97A9252__INCLUDED_)
