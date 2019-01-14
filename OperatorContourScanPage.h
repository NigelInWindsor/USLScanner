#if !defined(AFX_OPERATORCONTOURSCANPAGE_H__0E30F1B3_EB16_49C3_B7AF_3EDEE4AA120C__INCLUDED_)
#define AFX_OPERATORCONTOURSCANPAGE_H__0E30F1B3_EB16_49C3_B7AF_3EDEE4AA120C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OperatorContourScanPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COperatorContourScanPage dialog

class COperatorContourScanPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COperatorContourScanPage)

// Construction
public:
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	void SetToolBarCheckedState();
	CToolBar    m_wndToolBar;
	CReBar		m_wndRebar;
	
	void UpdateAllControls();
	COperatorContourScanPage();
	~COperatorContourScanPage();

// Dialog Data
	//{{AFX_DATA(COperatorContourScanPage)
	enum { IDD = IDD_OPERATOR_CONTOUR_SCAN_PAGE };
	CEdit	m_editBladeType;
	CEdit	m_editBatchID;
	CButton	m_checkUseBrane;
	CEdit	m_editDate;
	CEdit	m_editOperator;
	CEdit	m_editSerialNumber;
	CEdit	m_editPartNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COperatorContourScanPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COperatorContourScanPage)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChangeEditPartNumber();
	afx_msg void OnButtonPartNumber();
	afx_msg void OnButtonSerialNumber();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonResume();
	afx_msg void OnButtonAbort();
	afx_msg void OnCheckUseBrane();
	afx_msg void OnChangeEditSerialNumber();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPERATORCONTOURSCANPAGE_H__0E30F1B3_EB16_49C3_B7AF_3EDEE4AA120C__INCLUDED_)
