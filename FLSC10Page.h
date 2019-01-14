#if !defined(AFX_FLSC10PAGE_H__439729E1_0C23_498E_999D_C445F732788B__INCLUDED_)
#define AFX_FLSC10PAGE_H__439729E1_0C23_498E_999D_C445F732788B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FLSC10Page.h : header file
//

//#include "FLSC10Rangefinder.h"
#include "IntervalTimer.h"

/////////////////////////////////////////////////////////////////////////////
// CFLSC10Page dialog

class CFLSC10Page : public CPropertyPage
{
	DECLARE_DYNCREATE(CFLSC10Page)

// Construction
public:
	CFLSC10Page();
	~CFLSC10Page();

	static HANDLE m_comsSemaphore; //coms lock

// Dialog Data
	//{{AFX_DATA(CFLSC10Page)
	enum { IDD = IDD_FLSC10_PAGE };
	CEdit	m_editMiscReceived;
	CEdit	m_editDMin;
	CEdit	m_editDMean;
	CEdit	m_editDMax;
	CEdit	m_editDLast;
	CEdit	m_editDErr;
	CComboBox	m_comboPrintModulo;
	CEdit	m_editTLast;
	CEdit	m_editTMin;
	CEdit	m_editTMean;
	CEdit	m_editTMax;
	CEdit	m_editTErr;
	CButton	m_buttonStop;
	CButton	m_buttonMeasureCont;
	CButton	m_buttonMeasure;
	CButton	m_buttonDisconnect;
	CButton	m_buttonConnect;
	CEdit	m_editReceived;
	CEdit	m_editMeasure;
	CComboBox	m_comboPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFLSC10Page)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFLSC10Page)
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonDisconnect();
	afx_msg void OnButtonMeasure();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonMeasureCont();
	afx_msg void OnButtonStopclear();
	afx_msg void OnEditchangeComboPort();
	afx_msg void OnSelchangeComboPort();
	afx_msg void OnButtonResetStats();
	afx_msg void OnEditchangeComboPmodulo();
	afx_msg void OnSelchangeComboPmodulo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	APP App;
	CString m_buff;

	void UpdateErrorOutput();
	void UpdateMiscOutput();
	void UpdateMeasOutput();
	LRESULT RemoteHandler(WPARAM Wp, LPARAM Lp);
	void OnNoMeasurement();
	CIntervalTimer* m_pTimer;
	void UpdateStats();
//	void UpdateWriteOutput();
	int m_printModulo;
	int m_nPrint;
	bool CheckPrint();

	CCriticalSection m_Crit;

	CStats m_CStatsD;

	float m_fDistance;

	CString m_sDistance;

	void OnFLSC10Error();

	int m_errorCode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLSC10PAGE_H__439729E1_0C23_498E_999D_C445F732788B__INCLUDED_)
