#if !defined(AFX_VIEWSTATSDLG_H__7AA4CF57_55E1_485A_923A_FA3D9AFB45C5__INCLUDED_)
#define AFX_VIEWSTATSDLG_H__7AA4CF57_55E1_485A_923A_FA3D9AFB45C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewStatsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewStatsDlg dialog

class CViewStatsDlg : public CDialog
{
// Construction
public:
	CViewStatsDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,CData* pData = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CViewStatsDlg)
	enum { IDD = IDD_VIEW_STATS_DLG };
	CEdit	m_editMachinLocation;
	CEdit	m_editMachineName;
	CEdit	m_editSentence;
	CEdit	m_editOperator;
	CEdit	m_editFile;
	CEdit	m_editTestDate;
	CEdit	m_editStartLine;
	CEdit	m_editSlowIncrement;
	CEdit	m_editSlowAxis;
	CEdit	m_editProfileName;
	CEdit	m_editLineIncrement;
	CEdit	m_editFinishLine;
	CEdit	m_editFastIncrement;
	CEdit	m_editFastAxis;
	//}}AFX_DATA

	void UpdateAllControls();

	CWnd** m_pDlg;
	
	CData *m_pData;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewStatsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewStatsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSTATSDLG_H__7AA4CF57_55E1_485A_923A_FA3D9AFB45C5__INCLUDED_)
