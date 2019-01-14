#if !defined(AFX_DATAPROPERTIESDLG_H__FAF07B77_E3FF_4385_BC46_41E3CC794ABC__INCLUDED_)
#define AFX_DATAPROPERTIESDLG_H__FAF07B77_E3FF_4385_BC46_41E3CC794ABC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataPropertiesDlg.h : header file
//
#include "EditSpinItem.h"
#include "ColourListCtrl.h"
#include "StaticPosManage.h"

/////////////////////////////////////////////////////////////////////////////
// CDataPropertiesDlg dialog

class CDataPropertiesDlg : public CDialog
{
// Construction
public:
	void SetAccessPrivelages();
	void AutoFillAll();
	void SetToolBarCheckedState();
	CStatic	*m_pStatic;
	CEdit	*m_pEdit;
	void SetDlgLayout();
	void SaveColumnWidths();
	CStaticPosManage m_StaticPosList;
	LOGFONT m_LogFont;
	CFont* m_pFont;
	void CreateFont();
	int m_nFontHeight[20];
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CToolBar   m_wndFileToolBar;
	CReBar     m_wndRebar;
	CString m_strBuff;
	int m_nColumn;
	CEditSpinItem* m_pEditSpinItem;
	int m_nIndex;
	void FillList();
	void CreateColumns();
	void UpdateAllControls();
	
	int	m_nTableFormat;
	CData *m_pData;
	CWnd** m_pDlg;
	CDataPropertiesDlg(CWnd* pParent = NULL,CWnd **pDlg = NULL, CData *pData = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataPropertiesDlg)
	enum { IDD = IDD_DATA_PROPERTIES_DLG };
	CStatic	m_staticDate;
	CStatic	m_staticFontSize;
	CComboBox	m_comboFontSize;
	CColourListCtrl	m_listDetails;
	CEdit	m_editDate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataPropertiesDlg)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataPropertiesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnGetdispinfoListDetails(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListDetails(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListDetails(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListDetails(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDataAdd();
	afx_msg void OnDataDelete();
	afx_msg void OnDataInsert();
	afx_msg HRESULT ItemChanged(WPARAM, LPARAM);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveas();
	afx_msg void OnSelchangeComboFontSize();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonLanguage0();
	afx_msg void OnButtonLanguage1();
	afx_msg void OnButtonClassic();
	afx_msg void OnButtonTable();
	afx_msg void OnDataAutooperator();
	afx_msg void OnDataAutoinspectiondate();
	afx_msg void OnDataAutofaststepsize();
	afx_msg void OnDataAutoslowstepsize();
	afx_msg void OnDataAutoclear();
	afx_msg void OnButtonErase();
	afx_msg void OnButtonFill();
	afx_msg void OnButtonExportToWord();
	afx_msg void OnButtonExportText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAPROPERTIESDLG_H__FAF07B77_E3FF_4385_BC46_41E3CC794ABC__INCLUDED_)
