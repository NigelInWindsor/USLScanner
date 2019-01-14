#if !defined(AFX_AXISSERVOSTATUS_H__1760C400_2F35_40F8_BBF0_4CFD396A391B__INCLUDED_)
#define AFX_AXISSERVOSTATUS_H__1760C400_2F35_40F8_BBF0_4CFD396A391B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxisServoStatus.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxisServoStatus dialog

class CAxisServoStatus : public CDialog
{
// Construction
public:
	HBRUSH m_hBr;
	int	m_nOldFollowingError;
	int m_nOldStatus;
	int m_nOldVelocity;
	int m_nOldJoyDev;
	int m_nOldDesirePos;
	int m_nOldPos;
	CWnd* m_pParent;
	CWnd** m_pDlg;
	int		m_nLeft;
	int		m_nTop;
	int		m_nAxis;
	CAxisServoStatus(CWnd* pParent = NULL,CWnd** pDlg = NULL,int nLeft = 0, int nTop = 0,int nAxis = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAxisServoStatus)
	enum { IDD = IDD_AXIS_STATUS };
	CString	m_stringDesirePos;
	CString	m_stringPosition;
	CString	m_stringStatus;
	CString	m_stringJoyDev;
	BOOL	m_bCheckHomed;
	BOOL	m_bCheckInPos;
	BOOL	m_bCheckMaxSwitch;
	BOOL	m_bCheckMinSwitch;
	BOOL	m_bCheckSoftError;
	BOOL	m_bCheckHardError;
	CString	m_stringTitle;
	BOOL	m_bCheckEncoderFailure;
	BOOL	m_bCheckAmplifierEnabled;
	BOOL	m_bCheckAmplifierFault;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisServoStatus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAxisServoStatus)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISSERVOSTATUS_H__1760C400_2F35_40F8_BBF0_4CFD396A391B__INCLUDED_)
