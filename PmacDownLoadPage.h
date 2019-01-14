#if !defined(AFX_PMACDOWNLOADPAGE_H__5D33227A_77B6_4B07_9AC7_A1AC7796BB81__INCLUDED_)
#define AFX_PMACDOWNLOADPAGE_H__5D33227A_77B6_4B07_9AC7_A1AC7796BB81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PmacDownLoadPage.h : header file
//
#include "EditColour.h"
#include "ResizablePage.h"
#include "StaticPosManage.h"

/////////////////////////////////////////////////////////////////////////////
// CPmacDownLoadPage dialog

class CPmacDownLoadPage : public CResizablePage
{
	DECLARE_DYNCREATE(CPmacDownLoadPage)

// Construction
public:
	HBRUSH m_hBr;
	bool ImportTextFile();
	CStaticPosManage m_StaticPosProgram;
	void SetPathColour();
	bool m_bModified;
	int m_nIndex;
	void GetDirectory(CEdit *pEdit, CString *pInitDir);
	void UpdateAllControls();
	
	CPmacDownLoadPage();
	~CPmacDownLoadPage();

// Dialog Data
	//{{AFX_DATA(CPmacDownLoadPage)
	enum { IDD = IDD_PMAC_DOWNLOAD_DIALOG };
	CComboBox	m_comboTrackingTenthAxis;
	CEditColour	m_editPath;
	CEdit	m_editProgram;
	CProgressCtrl	m_progressDownLoad;
	CListBox	m_listPrograms;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPmacDownLoadPage)
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
	//{{AFX_MSG(CPmacDownLoadPage)
	afx_msg void OnButtonDownload();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeListPrograms();
	afx_msg void OnButtonBrowse();
	afx_msg void OnChangeEditProgram();
	afx_msg void OnButtonSave();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeComboTrackingTenth();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMACDOWNLOADPAGE_H__5D33227A_77B6_4B07_9AC7_A1AC7796BB81__INCLUDED_)
