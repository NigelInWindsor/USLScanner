#if !defined(AFX_LASEROPTIONSDLG_H__A75EDB80_B179_49A9_9604_031D5E4847C2__INCLUDED_)
#define AFX_LASEROPTIONSDLG_H__A75EDB80_B179_49A9_9604_031D5E4847C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LaserOptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLaserOptionsDlg dialog

class CLaserOptionsDlg : public CDialog
{
// Construction
public:
	CLaserOptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLaserOptionsDlg)
	enum { IDD = IDD_EDITLASER_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLaserOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLaserOptionsDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LASEROPTIONSDLG_H__A75EDB80_B179_49A9_9604_031D5E4847C2__INCLUDED_)
