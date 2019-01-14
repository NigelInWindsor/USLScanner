#if !defined(AFX_AXESSHEET_H__148C5AA4_61B3_4E52_844E_81B596149323__INCLUDED_)
#define AFX_AXESSHEET_H__148C5AA4_61B3_4E52_844E_81B596149323__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxesSheet.h : header file
//
#include "AxisDlg.h"
#include "AxisPmacDlg.h"
#include "AxesPmacStatus.h"
#include "Si6StatusPage.h"
#include "FokkerPumpPage.h"
#include "MoveLadderPage.h"
#include "AxesVirtualPage.h"
#include "AxisPowerPmacPage.h"

/////////////////////////////////////////////////////////////////////////////
// CAxesSheet

class CAxesSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CAxesSheet)

// Construction
public:
	CAxesSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CAxesSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,CWnd** DlgPtr=NULL);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxesSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetTabNames();
	void UpdateAllPages();
	void UpdateStatusSheet();
	void CreateFieldBusSheet();
	virtual ~CAxesSheet();

	// Generated message map functions
protected:
	CWnd** m_DlgPtr;
	bool m_DialogCreatedFlag;
	bool m_FlagEnableClose;
	CAxisPowerPmacPage*	m_pPPMACPage[16];
	CAxisDlg*			m_pAxis[16];
	CAxisPmacDlg*		m_pAxisPmac[16];
	CAxesPmacStatus*	m_pAxesStatus;
	CSi6StatusPage*		m_pSi6Page;
	CFokkerPumpPage*	m_pFokkerPumpPage;
	CMoveLadderPage*	m_pMoveLadderPage;
	CAxesVirtualPage*	m_pAxesVirtualPage;

	//{{AFX_MSG(CAxesSheet)
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXESSHEET_H__148C5AA4_61B3_4E52_844E_81B596149323__INCLUDED_)
