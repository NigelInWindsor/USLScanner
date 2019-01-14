#if !defined(AFX_EXCLUSIONZONESDLG_H__64D47512_9842_4BD7_9E26_4BEFD466D407__INCLUDED_)
#define AFX_EXCLUSIONZONESDLG_H__64D47512_9842_4BD7_9E26_4BEFD466D407__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExclusionZonesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExclusionZonesDlg dialog

class CExclusionZonesDlg : public CDialog
{
// Construction
public:
	void UpdateProfile();
	COORDDATA m_CpExclusion[3][2];
	void UpdateAllControls();
	
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CExclusionZonesDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExclusionZonesDlg)
	enum { IDD = IDD_EXCLUSION_DLG };
	CEdit	m_editAutoZ;
	CEdit	m_editAutoY;
	CEdit	m_editAutoX;
	CButton	m_checkExclusionZones0;
	CButton	m_checkExclusionZones1;
	CEdit	m_editFloating1;
	CEdit	m_editFloating0;
	CEdit	m_editFixed1;
	CEdit	m_editFixed0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExclusionZonesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExclusionZonesDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditFixed0();
	afx_msg void OnChangeEditFixed1();
	afx_msg void OnChangeEditFloating0();
	afx_msg void OnChangeEditFloating1();
	afx_msg void OnCheckExclusion();
	afx_msg void OnButtonAutomatic();
	afx_msg void OnButtonFixed0();
	afx_msg void OnButtonFixed1();
	afx_msg void OnButtonFloating0();
	afx_msg void OnButtonFloating1();
	afx_msg void OnCheckExclusion2();
	afx_msg void OnChangeEditAutoX();
	afx_msg void OnChangeEditAutoY();
	afx_msg void OnChangeEditAutoZ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXCLUSIONZONESDLG_H__64D47512_9842_4BD7_9E26_4BEFD466D407__INCLUDED_)
