#if !defined(AFX_ACTIVITYLOGPAGE_H__C6DB7182_8EBA_490C_B63C_3B2E461BFB54__INCLUDED_)
#define AFX_ACTIVITYLOGPAGE_H__C6DB7182_8EBA_490C_B63C_3B2E461BFB54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActivityLogPage.h : header file
//

#include "DoubleScrollBar.h"
/////////////////////////////////////////////////////////////////////////////
// CActivityLogPage dialog

class CActivityLogPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CActivityLogPage)

// Construction
public:
	void MouseWheel(UINT nFlags, short zDelta, CPoint pt);
	CSize m_size;
	int m_nIndex;
	int	m_nTopRow;
	int m_nRowsL;
	int m_nLength;
	bool RetrieveFile(CString FileName);
	CStringArray m_Str;
	CTime	m_LastWriteTime;
	CActivityLogPage();
	~CActivityLogPage();

// Dialog Data
	//{{AFX_DATA(CActivityLogPage)
	enum { IDD = IDD_ACTIVITY_LOG_PAGE };
	CDoubleScrollBar	m_scrollVert;
	CStatic	m_staticView;
	CEdit	m_editList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CActivityLogPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CActivityLogPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIVITYLOGPAGE_H__C6DB7182_8EBA_490C_B63C_3B2E461BFB54__INCLUDED_)
