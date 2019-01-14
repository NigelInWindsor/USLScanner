#if !defined(AFX_PR15DACPAGE_H__F14815BC_A87A_43B9_B217_428B574D313C__INCLUDED_)
#define AFX_PR15DACPAGE_H__F14815BC_A87A_43B9_B217_428B574D313C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pr15DacPage.h : header file
//
#include "USLListCtrl1.h"
#include "EditSpinItem.h"


/////////////////////////////////////////////////////////////////////////////
// CPr15DacPage dialog

class CPr15DacPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPr15DacPage)

// Construction
public:
	struct	TimeSlotData* m_pTS;
	CEditSpinItem* m_pEditSpinItem;
	void DacTableChanged();
	void OldDacTableChanged();
	void SetAccessPrivelage();
	void CalculateDacHardware();
	void CreateColumnList();
	void FillList();
	void UpdateAllControls();
	Pr15Data*	m_pPr;
	bool m_bDialogCreated;
	CPr15DacPage();
	~CPr15DacPage();

// Dialog Data
	//{{AFX_DATA(CPr15DacPage)
	enum { IDD = IDD_DAC_DIALOG };
	CListCtrl	m_listDac;
	CComboBox	m_comboTriggerMode;
	CSpinButtonCtrl	m_spinDacThreshold;
	CSpinButtonCtrl	m_spinDacBlanking;
	CEdit	m_editDacThreshold;
	CEdit	m_editDacBlanking;
	CComboBox	m_comboAction;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPr15DacPage)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPr15DacPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboAction();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditDacBlanking();
	afx_msg void OnChangeEditDacThreshold();
	afx_msg void OnSelchangeComboTriggerMode();
	afx_msg void OnDblclkListDac(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListDac(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListDac(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PR15DACPAGE_H__F14815BC_A87A_43B9_B217_428B574D313C__INCLUDED_)
