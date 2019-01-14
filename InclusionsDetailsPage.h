#if !defined(AFX_INCLUSIONSDETAILSPAGE_H__90A6C62D_0DF5_4380_8BF4_3911B2283A67__INCLUDED_)
#define AFX_INCLUSIONSDETAILSPAGE_H__90A6C62D_0DF5_4380_8BF4_3911B2283A67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InclusionsDetailsPage.h : header file
//

#include "Inclusions.h"

/////////////////////////////////////////////////////////////////////////////
// CInclusionsDetailsPage dialog

class CInclusionsDetailsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CInclusionsDetailsPage)

// Construction
public:
	CInclusions*	m_pInclusions;
	
	void UpdateAllControls();
	CInclusionsDetailsPage(CInclusions*	pInclusions = NULL);
	~CInclusionsDetailsPage();

// Dialog Data
	//{{AFX_DATA(CInclusionsDetailsPage)
	enum { IDD = IDD_INCLUSIONS_DETAILS_DLG };
	CEdit	m_editCast;
	CEdit	m_editGrade;
	CEdit	m_editOrderNumber;
	CEdit	m_editCustomer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CInclusionsDetailsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CInclusionsDetailsPage)
	afx_msg void OnChangeEditCustomer();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditCast();
	afx_msg void OnChangeEditGrade();
	afx_msg void OnChangeEditOrderNumber();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INCLUSIONSDETAILSPAGE_H__90A6C62D_0DF5_4380_8BF4_3911B2283A67__INCLUDED_)
