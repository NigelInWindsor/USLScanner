#include "afxwin.h"
#include "afxcmn.h"
#if !defined(AFX_AOSPHASEDARRAYPAGE_H__439F55FC_DE9A_456D_BF04_0080ECA7ACC4__INCLUDED_)
#define AFX_AOSPHASEDARRAYPAGE_H__439F55FC_DE9A_456D_BF04_0080ECA7ACC4__INCLUDED_

#include "EditSpinCtrl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AOSPhasedArrayPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAOSPhasedArrayPage dialog

class CAOSPhasedArrayPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAOSPhasedArrayPage)

// Construction
public:
	HBRUSH m_hBr;

public:
	void UpdateAllControls();
	CAOSPhasedArrayPage();
	~CAOSPhasedArrayPage();

// Dialog Data
	//{{AFX_DATA(CAOSPhasedArrayPage)
	enum { IDD = IDD_AOS_PHASEDARRAY_PAGE };
	CIPAddressCtrl	m_ipAddress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAOSPhasedArrayPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAOSPhasedArrayPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnFieldchangedAosIpaddress(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedCheckConnectAtStartup();
	CButton m_checkConnectAtStartUp;
	CButton m_buttonConnect;
	CEdit m_editStatus;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_editFrameCount;
	CEdit m_editFrameRate;
	afx_msg void OnBnClickedButtonHwFile();
	CEdit m_editPRFRate;
	CEdit m_editDataRate;
	CEditSpinCtrl m_editspinAScanLength;
	afx_msg void OnDeltaposSpinAscanLength(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditAscanLength();
	afx_msg void OnBnClickedButtonDefault();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AOSPHASEDARRAYPAGE_H__439F55FC_DE9A_456D_BF04_0080ECA7ACC4__INCLUDED_)
