#if !defined(AFX_RRWIPPAGE_H__FF2A12EA_067E_423C_B89E_6B16C2E563D0__INCLUDED_)
#define AFX_RRWIPPAGE_H__FF2A12EA_067E_423C_B89E_6B16C2E563D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RRWIPPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRRWIPPage dialog

class CRRWIPPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRRWIPPage)

// Construction
public:
	void UpdateAllControls();
	
	CRRWIPPage();
	~CRRWIPPage();

// Dialog Data
	//{{AFX_DATA(CRRWIPPage)
	enum { IDD = IDD_RR_WIP_PAGE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRRWIPPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRRWIPPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RRWIPPAGE_H__FF2A12EA_067E_423C_B89E_6B16C2E563D0__INCLUDED_)
