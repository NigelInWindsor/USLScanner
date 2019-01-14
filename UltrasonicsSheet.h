#if !defined(AFX_ULTRASONICSSHEET_H__B5193B69_45BE_47B7_9642_F60B23891872__INCLUDED_)
#define AFX_ULTRASONICSSHEET_H__B5193B69_45BE_47B7_9642_F60B23891872__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UltrasonicsSheet.h : header file
//
//#include "TimeSlotPr15.h"
//#include "TimeSlotPl15.h"
//#include "TimeSlotPlPr15Page.h"
#include "TimeSlotPr30.h"
//#include "Pr15DacPage.h"
#include "ScopeOptionsPage.h"
#include "Pr20Dlg.h"
#include "GlobalUtPage.h"
#include "GatesPage.h"
#include "SelfTestDlg.h"
//#include "ProbePage.h"
//#include "RavensCourtPage.h"
#include "LSAPage.h"
#include "ExtSourcePage.h"
#include "UtTrafficLightPage.h"

/////////////////////////////////////////////////////////////////////////////
// CUltrasonicsSheet

class CUltrasonicsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CUltrasonicsSheet)


// Construction
public:
	CUltrasonicsSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CUltrasonicsSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** DlgPtr = NULL);

// Attributes
protected:
	CTimeSlotPr30* m_pTimeSlotPr300;
	CTimeSlotPr30* m_pTimeSlotPr301;
	CTimeSlotPr30* m_pTimeSlotPr302;
	CTimeSlotPr30* m_pTimeSlotPr303;
	CTimeSlotPr30* m_pTimeSlotPr304;
	CTimeSlotPr30* m_pTimeSlotPr305;
	CTimeSlotPr30* m_pTimeSlotPr306;
	CTimeSlotPr30* m_pTimeSlotPr307;

//	CTimeSlotPr15* m_pTimeSlotPr150;
//	CTimeSlotPr15* m_pTimeSlotPr151;
//	CTimeSlotPr15* m_pTimeSlotPr152;
//	CTimeSlotPr15* m_pTimeSlotPr153;
//	CTimeSlotPr15* m_pTimeSlotPr154;
//	CTimeSlotPr15* m_pTimeSlotPr155;
//	CTimeSlotPr15* m_pTimeSlotPr156;
//	CTimeSlotPr15* m_pTimeSlotPr157;

//	CTimeSlotPl15* m_pTimeSlotPl150;
//	CTimeSlotPl15* m_pTimeSlotPl151;
//	CTimeSlotPl15* m_pTimeSlotPl152;
//	CTimeSlotPl15* m_pTimeSlotPl153;
//	CTimeSlotPl15* m_pTimeSlotPl154;
//	CTimeSlotPl15* m_pTimeSlotPl155;
//	CTimeSlotPl15* m_pTimeSlotPl156;
//	CTimeSlotPl15* m_pTimeSlotPl157;

//	CTimeSlotPlPr15Page* m_pTimeSlotPlPr15;


//	CPr15DacPage* m_pPr15Dac;

public:
	CScopeOptionsPage* m_pScopeOptions;

	CSelfTestDlg* m_pSelfTestPage;
	CPr20Dlg*	m_pPr20;
	CPr20Dlg*	m_pPr50;
//	CAtpGpd*	m_pAtpGpd;
//	CTimsDlg*	m_pAtpTims;
	CGlobalUtPage* m_pGlobalUt;
	CGatesPage* m_pGatesPage;
//	CProbePage* m_pProbe;
//	CRavensCourtPage* m_pRavensCourtPage;
	CLSAPage* m_pLSAPage;
	CExtSourcePage* m_pExtSourcePage;
	CUtTrafficLightPage*	m_pUtTrafficLightPage;

	CWnd** m_DlgPtr;
	bool m_FlagEnableClose;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUltrasonicsSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateLSAPage();
	void RedrawAllPages();
	void UpdateGatesPage();
	void SetTabNames();
	void SetTabName(int nTabNumber, CString cName);
	HRESULT SetNumberTimeslots(WPARAM wp, LPARAM lp);
	void UpdateDacTable();
	void SetWindowText();
	void SetMaxTimeSlots();
	void UpdatePr15Sheet();
	void SetAccessPrivelage();
	void UpdateAllPages();
	virtual ~CUltrasonicsSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUltrasonicsSheet)
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	void RemoveTimeslots();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ULTRASONICSSHEET_H__B5193B69_45BE_47B7_9642_F60B23891872__INCLUDED_)
