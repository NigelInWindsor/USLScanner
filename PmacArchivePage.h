#if !defined(AFX_PMACARCHIVEPAGE_H__91B5DE59_AF94_4EB9_9618_4BA75949515B__INCLUDED_)
#define AFX_PMACARCHIVEPAGE_H__91B5DE59_AF94_4EB9_9618_4BA75949515B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PmacArchivePage.h : header file
//
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CPmacArchivePage dialog

class CPmacArchivePage : public CResizablePage
{
	DECLARE_DYNCREATE(CPmacArchivePage)

// Construction
public:
	void UpdateAllControls();
	CPmacArchivePage();
	~CPmacArchivePage();

// Dialog Data
	//{{AFX_DATA(CPmacArchivePage)
	enum { IDD = IDD_PMAC_ARCHIVE_PAGE };
	CButton	m_checkPrograms;
	CButton	m_checkPLCs;
	CButton	m_checkPlccPrograms;
	CButton	m_checkPlcPrograms;
	CButton	m_checkMotionPrograms;
	CButton	m_checkMVariables;
	CButton	m_checkIVariables;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPmacArchivePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPmacArchivePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckMVariables();
	afx_msg void OnCheckIVariables();
	afx_msg void OnCheckMotionPrograms();
	afx_msg void OnCheckPlcPrograms();
	afx_msg void OnCheckPlccPrograms();
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonSaveAs();
	afx_msg void OnCheckPrograms();
	afx_msg void OnCheckPlcs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMACARCHIVEPAGE_H__91B5DE59_AF94_4EB9_9618_4BA75949515B__INCLUDED_)
