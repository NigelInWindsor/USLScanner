#if !defined(AFX_VIEWZOOMPAGE_H__11F1FC4C_79A6_463E_ACA3_A0B5A4460EFD__INCLUDED_)
#define AFX_VIEWZOOMPAGE_H__11F1FC4C_79A6_463E_ACA3_A0B5A4460EFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewZoomPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewZoomPage dialog

class CViewZoomPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CViewZoomPage)

// Construction
public:
	CWnd* m_pViewSheet;
	CViewZoomPage(CWnd* pViewSheet=NULL);
	~CViewZoomPage();

// Dialog Data
	//{{AFX_DATA(CViewZoomPage)
	enum { IDD = IDD_VIEW_ZOOM_PAGE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CViewZoomPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CViewZoomPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWZOOMPAGE_H__11F1FC4C_79A6_463E_ACA3_A0B5A4460EFD__INCLUDED_)
