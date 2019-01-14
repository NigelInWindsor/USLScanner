#if !defined(AFX_CADTABLEDLG_H__EE83765D_AFFA_40D9_97B7_32BDBB482811__INCLUDED_)
#define AFX_CADTABLEDLG_H__EE83765D_AFFA_40D9_97B7_32BDBB482811__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CADTableDlg.h : header file
//
#include "EditSpinItem.h"
#include "StaticPosManage.h"

/////////////////////////////////////////////////////////////////////////////
// CCADTableDlg dialog

class CCADTableDlg : public CDialog
{
// Construction
public:
	void InvalidateParent();
	void UpdateAllControls();
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	void SetToolBarCheckedState();
	void FillList();
	void CreateColumns();
	
	CEditSpinItem* m_pEditSpinItem;
	CCADElement m_CADElement;
	CStaticPosManage m_StaticPosTable;
	CToolBar   m_wndPropertiesToolBar;
	CReBar     m_wndRebar;
	CData* m_pData;
	CWnd* m_pParent;
	CWnd** m_pDlg;
	CCADTableDlg(CWnd* pParent /*=NULL*/,CWnd** pDlg = NULL,CData* pData = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCADTableDlg)
	enum { IDD = IDD_CAD_TABLE_DLG };
	CButton	m_checkLineText;
	CButton	m_checkFinishArrow;
	CButton	m_checkStartArrow;
	CSpinButtonCtrl	m_spinLineWidth;
	CEdit	m_editLineWidth;
	CComboBox	m_comboDefectMode;
	CEdit	m_editFontSize;
	CStatic	m_staticLineColor;
	CSpinButtonCtrl	m_spinFontSize;
	CButton	m_buttonFont;
	CListCtrl	m_listTable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCADTableDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCADTableDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetdispinfoListTable(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListTable(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonCadLength();
	afx_msg void OnButtonCadWidth();
	afx_msg void OnButtonCadHeight();
	afx_msg void OnButtonCadCircumferance();
	afx_msg void OnButtonCadArea();
	afx_msg void OnButtonFont();
	afx_msg void OnChangeEditFontSize();
	afx_msg void OnDeltaposSpinFontSize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnRclickListTable(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCadCopy();
	afx_msg void OnCadPaste();
	afx_msg void OnCadDelete();
	afx_msg void OnButtonCadHistogram();
	afx_msg void OnButtonCadRefLevel();
	afx_msg void OnButtonCadDefectLevel();
	afx_msg void OnButtonCadDefectArea();
	afx_msg void OnCadCopytoclipboard();
	afx_msg void OnDblclkListTable(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HRESULT TableChanged(WPARAM, LPARAM);
	afx_msg void OnButtonVerboseTable();
	afx_msg void OnButtonColumnTable();
	afx_msg void OnButtonCadDefectHeight();
	afx_msg void OnButtonCadDefectWidth();
	afx_msg void OnSelchangeComboDefectMode();
	afx_msg void OnDeltaposSpinLineWidth(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditLineWidth();
	afx_msg void OnCheckStartArrow();
	afx_msg void OnCheckFinishArrow();
	afx_msg void OnCheckLineText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CADTABLEDLG_H__EE83765D_AFFA_40D9_97B7_32BDBB482811__INCLUDED_)
