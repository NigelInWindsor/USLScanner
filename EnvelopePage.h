#if !defined(AFX_ENVELOPEPAGE_H__82EB0A07_CDF8_4EE1_9949_8AC28FE51191__INCLUDED_)
#define AFX_ENVELOPEPAGE_H__82EB0A07_CDF8_4EE1_9949_8AC28FE51191__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnvelopePage.h : header file
//
#include "EditSpinItem.h"
/////////////////////////////////////////////////////////////////////////////
// CEnvelopePage dialog

class CEnvelopePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CEnvelopePage)

// Construction
public:
	CEditSpinItem* m_pEditSpinItem;
	int	m_nStartIndex;
	int	m_nFinishIndex;
	int m_nIndex;
	void FillList();
	void CreateColumns();
	CData *m_pData;
	
	void UpdateAllControls();
	CWnd* m_pViewSheet;
	CEnvelopePage(CWnd* pViewSheet = NULL, CData *pData = NULL);
	~CEnvelopePage();

// Dialog Data
	//{{AFX_DATA(CEnvelopePage)
	enum { IDD = IDD_ENVELOPE_PAGE };
	CListCtrl	m_listEnvelope;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEnvelopePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEnvelopePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetdispinfoListEnvelope(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListEnvelope(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListEnvelope(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListEnvelope(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnvelopeSetallto1colour();
	afx_msg void OnEnvelopeInidividualcolour();
	afx_msg void OnEnvelopeImportallenvelopesfromprofile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENVELOPEPAGE_H__82EB0A07_CDF8_4EE1_9949_8AC28FE51191__INCLUDED_)
