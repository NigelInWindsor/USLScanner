#if !defined(AFX_AXESCOMPENSATIONPAGE_H__57D1D110_CFE0_4DF7_A537_0462531223BA__INCLUDED_)
#define AFX_AXESCOMPENSATIONPAGE_H__57D1D110_CFE0_4DF7_A537_0462531223BA__INCLUDED_

#include "Coord.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxesCompensationPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxesCompensationPage dialog

class CAxesCompensationPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAxesCompensationPage)

// Construction
public:
	CCoord m_CpError;
	int m_nLeft;
	int	m_nRight;
	int m_nIndex;
	int m_nAxis;
	void FillList();
	void CreateColumns();
	void UpdateAllControls();
	CAxesCompensationPage();
	~CAxesCompensationPage();

// Dialog Data
	//{{AFX_DATA(CAxesCompensationPage)
	enum { IDD = IDD_AXES_COMPENSATION_PAGE };
	CEdit	m_editGash;
	CButton	m_checkUse;
	CEdit	m_editPosXRight;
	CEdit	m_editPosYRight;
	CEdit	m_editPosZRight;
	CListCtrl	m_listCoords;
	CComboBox	m_comboAxis;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAxesCompensationPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAxesCompensationPage)
	afx_msg void OnSelchangeComboAxis();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRclickListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCompensateAdd();
	afx_msg void OnCompensateDelete();
	afx_msg void OnCompensateModify();
	afx_msg void OnCompensateClearall();
	afx_msg void OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckUse();
	afx_msg void OnChangeEditGash();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXESCOMPENSATIONPAGE_H__57D1D110_CFE0_4DF7_A537_0462531223BA__INCLUDED_)
