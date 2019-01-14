#if !defined(AFX_MOVELADDERPAGE_H__187E52D3_37F8_4329_8B2B_395AA733CAE8__INCLUDED_)
#define AFX_MOVELADDERPAGE_H__187E52D3_37F8_4329_8B2B_395AA733CAE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MoveLadderPage.h : header file
//

#include "EditSpinItem.h"

/////////////////////////////////////////////////////////////////////////////
// CMoveLadderPage dialog

class CMoveLadderPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CMoveLadderPage)

// Construction
public:
	CString m_strEdit;
	CEditSpinItem *m_pEditSpinItem;
	CString m_strAction[3];
	int m_nIndex;
	int	m_nColumn;
	void UpdateAllControls();
	void FillList();
	void CreateColumns();
	
	CMoveLadderPage();
	~CMoveLadderPage();

// Dialog Data
	//{{AFX_DATA(CMoveLadderPage)
	enum { IDD = IDD_MOVE_LADDER_PAGE };
	CListCtrl	m_listLadder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMoveLadderPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMoveLadderPage)
	afx_msg void OnClickListLadder(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListLadder(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListLadder(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickListLadder(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLadderAdd();
	afx_msg void OnLadderInsert();
	afx_msg void OnLadderDelete();
	afx_msg void OnLadderStoreleftheadposition();
	afx_msg void OnLadderStorerightheadposition();
	afx_msg HRESULT ItemChanged(WPARAM, LPARAM);
	afx_msg void OnLadderExecutethisline();
	afx_msg void OnLadderExecuteall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOVELADDERPAGE_H__187E52D3_37F8_4329_8B2B_395AA733CAE8__INCLUDED_)
