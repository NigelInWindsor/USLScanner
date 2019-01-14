#if !defined(AFX_EGISMOSRANGEFINDERPAGE_H__0B7F8BAE_BBC1_42C5_802D_D1BBC605A5F7__INCLUDED_)
#define AFX_EGISMOSRANGEFINDERPAGE_H__0B7F8BAE_BBC1_42C5_802D_D1BBC605A5F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EgismosRangefinderPage.h : header file
//

#include "uslscanner.h"
#include "EgismosRangefinder.h"
#include "Stats.h"
#include "IntervalTimer.h"

/////////////////////////////////////////////////////////////////////////////
// CEgismosRangefinderPage dialog

class CEgismosRangefinderPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CEgismosRangefinderPage)

// Construction
public:
	CEgismosRangefinderPage();
	~CEgismosRangefinderPage();

// Dialog Data
	//{{AFX_DATA(CEgismosRangefinderPage)
	enum { IDD = IDD_EGISMOSRANGEFINDER_PAGE };
	CEdit	m_editTMin;
	CEdit	m_editTMean;
	CEdit	m_editTMax;
	CEdit	m_editTLast;
	CEdit	m_editTErr;
	CEdit	m_editDMin;
	CEdit	m_editDMean;
	CEdit	m_editDMax;
	CEdit	m_editDLast;
	CEdit	m_editDErr;
	CButton	m_buttonDisconnect;
	CButton	m_buttonStop;
	CButton	m_buttonReadSlaveAddress;
	CButton	m_buttonReadDeviceType;
	CButton	m_buttonReadDeviceStatus;
	CButton	m_buttonMeasOnce;
	CButton	m_buttonMeasCont;
	CButton	m_buttonLaserOn;
	CButton	m_buttonLaserOff;
	CButton	m_buttonConnect;
	CButton	m_buttonBuzzerOn;
	CButton	m_buttonBuzzerOff;
	CEdit	m_editReceived;
	CEdit	m_editInterp;
	CEdit	m_editSent;
	CComboBox	m_comboPort;
	CEdit	m_editRange_mm;
	//}}AFX_DATA

	static HANDLE m_comsSemaphore; //coms lock

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEgismosRangefinderPage)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEgismosRangefinderPage)
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonBuzzeroff();
	afx_msg void OnButtonBuzzeron();
	afx_msg void OnButtonLaseroff();
	afx_msg void OnButtonLaseron();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonReaddevicetype();
	afx_msg void OnButtonReaddevicestatus();
	afx_msg void OnButtonReadslaveaddress();
	afx_msg void OnButtonMeasCont();
	afx_msg void OnButtonMeasOnce();
	afx_msg void OnButtonDisconnect();
	void OnNoMeasurement();
	afx_msg void OnEditchangeComboPort();
	afx_msg void OnSelchangeComboPort();
	afx_msg void OnButtonResetStats();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	APP App;
	CString m_buff;
	CEgismosRangefinder* m_pRF;

	void UpdateReadOutput();
	void UpdateWriteOutput();
	void UpdateMeasOutput();

	LRESULT RemoteHandler(WPARAM Wp, LPARAM Lp);

	float m_fDistance;

	void UpdateStats();

	CStats m_CStatsD;

	CIntervalTimer* m_pTimer;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EGISMOSRANGEFINDERPAGE_H__0B7F8BAE_BBC1_42C5_802D_D1BBC605A5F7__INCLUDED_)
