#if !defined(AFX_THIRDPARTYIMAGEDLG_H__A60F12E2_378F_4244_BE04_61C6A31CABC2__INCLUDED_)
#define AFX_THIRDPARTYIMAGEDLG_H__A60F12E2_378F_4244_BE04_61C6A31CABC2__INCLUDED_

#include "Data.h"	// Added by ClassView
#include "StaticPosManage.h"
#include "afxwin.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThirdPartyImageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CThirdPartyImageDlg dialog

class CThirdPartyImageDlg : public CDialog
{
// Construction
public:
	int m_nCoordL;
	void FillList();
	void CreateColumns();
	unsigned char* m_pcBuffer;
	int m_nFileLength;
	CStaticPosManage m_StaticPosView;
	void UpdateAllControls();
	CWnd** m_pDlg;
	
	CData m_Data;
	CString m_FilePath;
	CThirdPartyImageDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,CString FilePath = L"Narda");   // standard constructor

// Dialog Data
	//{{AFX_DATA(CThirdPartyImageDlg)
	enum { IDD = IDD_THIRD_PARTY_IMAGE_DIALOG };
	CListCtrl	m_listDecode;
	CComboBox	m_comboSizeOf;
	CEdit	m_editSearchFor;
	CEdit	m_editResult;
	CSpinButtonCtrl	m_spinStartOffset;
	CSpinButtonCtrl	m_spinNumberLines;
	CSpinButtonCtrl	m_spinSamplesLine;
	CEdit	m_editStartOffset;
	CEdit	m_editSamplesLine;
	CEdit	m_editNumberLines;
	CEdit	m_editMinValue;
	CEdit	m_editMaxValue;
	CStatic	m_staticView;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThirdPartyImageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CThirdPartyImageDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnChangeEditSamplesline();
	afx_msg void OnChangeEditNumberlines();
	afx_msg void OnChangeEditStartoffset();
	afx_msg void OnChangeEditMinValue();
	afx_msg void OnChangeEditMaxValue();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonSearchFor();
	afx_msg void OnGetdispinfoListDecode(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editText;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THIRDPARTYIMAGEDLG_H__A60F12E2_378F_4244_BE04_61C6A31CABC2__INCLUDED_)
