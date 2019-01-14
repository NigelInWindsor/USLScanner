#if !defined(AFX_SCANLINESPAGE_H__F63B6EFA_B417_48BE_BF74_5117ECD1A6D4__INCLUDED_)
#define AFX_SCANLINESPAGE_H__F63B6EFA_B417_48BE_BF74_5117ECD1A6D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScanLinesPage.h : header file
//

#include "EditSpinItem.h"


/////////////////////////////////////////////////////////////////////////////
// CScanLinesPage dialog

class CScanLinesPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CScanLinesPage)

// Construction
public:
	HBRUSH m_hBr;
	void ClearEditSpin();
	HRESULT ItemChanged(WPARAM, LPARAM);
	void FillListCoords();
	void FillListTimeSlots();
	void CreateColumns();
	void UpdateAllControls();
	CEditSpinItem* m_pEditSpinItem;
	
	int	m_nTimeSlotIndex;
	int	m_nCoordIndex;
	int	m_nEditInt;
	int	m_nItemChangeType;
	CScanLinesPage();
	~CScanLinesPage();

// Dialog Data
	//{{AFX_DATA(CScanLinesPage)
	enum { IDD = IDD_SCANLINES_PAGE };
	CListCtrl	m_listTimeSlots;
	CListCtrl	m_listCoords;
	CSpinButtonCtrl	m_spinNumberLines;
	CEdit	m_editNumberLines;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CScanLinesPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CScanLinesPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnChangeEditNumberLines();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnGetdispinfoListTimeslots(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListTimeslots(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListTimeslots(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCANLINESPAGE_H__F63B6EFA_B417_48BE_BF74_5117ECD1A6D4__INCLUDED_)
