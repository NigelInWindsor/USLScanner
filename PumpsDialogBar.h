#if !defined(AFX_PUMPSDIALOGBAR_H__B6F48B55_0084_4672_A1AC_C77466003474__INCLUDED_)
#define AFX_PUMPSDIALOGBAR_H__B6F48B55_0084_4672_A1AC_C77466003474__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PumpsDialogBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPumpsDialogBar dialog

class CPumpsDialogBar : public CDialog
{
// Construction
public:
	CPumpsDialogBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPumpsDialogBar)
	enum { IDD = IDD_PUMPS_DIALOGBAR };
	CButton	m_buttonRightPump;
	CButton	m_buttonLeftPump;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPumpsDialogBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPumpsDialogBar)
	afx_msg void OnStaticPump0();
	afx_msg void OnStaticPump1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PUMPSDIALOGBAR_H__B6F48B55_0084_4672_A1AC_C77466003474__INCLUDED_)
