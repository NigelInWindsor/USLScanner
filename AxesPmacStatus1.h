#if !defined(AFX_AXESPMACSTATUS_H__8EAA5445_E78C_487B_B1A1_116FBFD8CB1E__INCLUDED_)
#define AFX_AXESPMACSTATUS_H__8EAA5445_E78C_487B_B1A1_116FBFD8CB1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxesPmacStatus1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxesPmacStatus dialog

class CAxesPmacStatus : public CPropertyPage
{
	DECLARE_DYNCREATE(CAxesPmacStatus)

// Construction
public:
	int m_nGateStatus;
	int m_nESwitchStatus;
	int	m_nOldJoyDev[10];
	int	m_nOldPos[10];
	int	m_nOldDesirePos[10];
	int m_nOldStatus[10];
	CAxesPmacStatus();
	~CAxesPmacStatus();

// Dialog Data
	//{{AFX_DATA(CAxesPmacStatus)
	enum { IDD = IDD_AXES_STATUS_DIALOG };
	BOOL	m_bCheckHardError1;
	BOOL	m_bCheckHomed1;
	BOOL	m_bCheckInPos1;
	BOOL	m_bCheckMaxSwitch1;
	BOOL	m_bCheckMinSwitch1;
	BOOL	m_bCheckSoftError1;
	CString	m_stringDesirePos1;
	CString	m_stringPosition1;
	CString	m_stringStatus1;
	BOOL	m_bCheckHardError2;
	BOOL	m_bCheckHomed2;
	BOOL	m_bCheckInPos2;
	BOOL	m_bCheckMaxSwitch2;
	BOOL	m_bCheckMinSwitch2;
	BOOL	m_bCheckSoftError2;
	CString	m_stringDesirePos2;
	CString	m_stringPosition2;
	CString	m_stringStatus2;
	BOOL	m_bCheckHardError3;
	BOOL	m_bCheckHomed3;
	BOOL	m_bCheckInPos3;
	BOOL	m_bCheckMaxSwitch3;
	BOOL	m_bCheckMinSwitch3;
	BOOL	m_bCheckSoftError3;
	CString	m_stringDesirePos3;
	CString	m_stringPosition3;
	CString	m_stringStatus3;
	BOOL	m_bCheckHardError4;
	BOOL	m_bCheckHomed4;
	BOOL	m_bCheckInPos4;
	BOOL	m_bCheckMaxSwitch4;
	BOOL	m_bCheckMinSwitch4;
	BOOL	m_bCheckSoftError4;
	CString	m_stringDesirePos4;
	CString	m_stringPosition4;
	CString	m_stringStatus4;
	BOOL	m_bCheckHardError5;
	BOOL	m_bCheckHomed5;
	BOOL	m_bCheckInPos5;
	BOOL	m_bCheckMaxSwitch5;
	BOOL	m_bCheckMinSwitch5;
	BOOL	m_bCheckSoftError5;
	CString	m_stringDesirePos5;
	CString	m_stringPosition5;
	CString	m_stringStatus5;
	BOOL	m_bCheckHardError6;
	BOOL	m_bCheckHomed6;
	BOOL	m_bCheckInPos6;
	BOOL	m_bCheckMaxSwitch6;
	BOOL	m_bCheckMinSwitch6;
	BOOL	m_bCheckSoftError6;
	CString	m_stringDesirePos6;
	CString	m_stringPosition6;
	CString	m_stringStatus6;
	BOOL	m_bCheckHardError7;
	BOOL	m_bCheckHomed7;
	BOOL	m_bCheckInPos7;
	BOOL	m_bCheckMaxSwitch7;
	BOOL	m_bCheckMinSwitch7;
	BOOL	m_bCheckSoftError7;
	CString	m_stringDesirePos7;
	CString	m_stringPosition7;
	CString	m_stringStatus7;
	BOOL	m_bCheckHardError8;
	BOOL	m_bCheckHomed8;
	BOOL	m_bCheckInPos8;
	BOOL	m_bCheckMaxSwitch8;
	BOOL	m_bCheckMinSwitch8;
	BOOL	m_bCheckSoftError8;
	CString	m_stringDesirePos8;
	CString	m_stringPosition8;
	CString	m_stringStatus8;
	BOOL	m_bCheckHardError9;
	BOOL	m_bCheckHomed9;
	BOOL	m_bCheckInPos9;
	BOOL	m_bCheckMaxSwitch9;
	BOOL	m_bCheckMinSwitch9;
	BOOL	m_bCheckSoftError9;
	CString	m_stringDesirePos9;
	CString	m_stringPosition9;
	CString	m_stringStatus9;
	BOOL	m_bCheckHardError10;
	BOOL	m_bCheckHomed10;
	BOOL	m_bCheckInPos10;
	BOOL	m_bCheckMaxSwitch10;
	BOOL	m_bCheckMinSwitch10;
	BOOL	m_bCheckSoftError10;
	CString	m_stringDesirePos10;
	CString	m_stringPosition10;
	CString	m_stringStatus10;
	CString	m_stringJoyDev1;
	CString	m_stringJoyDev10;
	CString	m_stringJoyDev2;
	CString	m_stringJoyDev3;
	CString	m_stringJoyDev4;
	CString	m_stringJoyDev5;
	CString	m_stringJoyDev6;
	CString	m_stringJoyDev7;
	CString	m_stringJoyDev8;
	CString	m_stringJoyDev9;
	BOOL	m_bGateStatus;
	BOOL	m_bESwitchStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAxesPmacStatus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAxesPmacStatus)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXESPMACSTATUS_H__8EAA5445_E78C_487B_B1A1_116FBFD8CB1E__INCLUDED_)
