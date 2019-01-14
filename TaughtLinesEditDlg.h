#if !defined(AFX_TAUGHTLINESEDITDLG_H__E2B4922E_4FC3_4ABE_9761_7A19F3D869EA__INCLUDED_)
#define AFX_TAUGHTLINESEDITDLG_H__E2B4922E_4FC3_4ABE_9761_7A19F3D869EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaughtLinesEditDlg.h : header file
//
#include "TaughtToolsDlg.h"
#include "FlatPackToolsDlg.h"
#include "EditSpinItem.h"

/////////////////////////////////////////////////////////////////////////////
// CTaughtLinesEditDlg dialog

class CTaughtLinesEditDlg : public CDialog
{
// Construction
public:
	void ClearEditSpin();
	void FillList();
	void CreateColumns();
	void ToolTaughtLinesFromParent();
	CFLatPackToolsDlg* m_pFlatPackToolsDlg;
	CTaughtToolsDlg* m_pTaughtTools;
	void UpdateParent(int nAction);
	void UpdateAllControls();
	CEditSpinItem* m_pEditSpinItem;
	CWnd* m_pParent;
	CWnd** m_pDlg;
	
	int		m_nLeft;
	int		m_nTop;
	int		m_nIndex;
	CTaughtLinesEditDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,int nLeft=0,int nTop=0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaughtLinesEditDlg)
	enum { IDD = IDD_EDITTAUGHTLINES_DIALOG };
	CButton	m_checkPeakOpposite;
	CButton	m_checkMoveRight;
	CButton	m_checkWaterPath;
	CButton	m_checkNormalize;
	CComboBox	m_comboOptomizeWhat;
	CListCtrl	m_listLines;
	CStatic	m_staticColor;
	CComboBox	m_comboPercentageLines;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaughtLinesEditDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaughtLinesEditDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckCrosses();
	afx_msg void OnCheckLines();
	afx_msg void OnCheckOn();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeComboLineStyle();
	afx_msg void OnToolsTaughtlines();
	afx_msg void OnChangeEditLineNumber();
	afx_msg void OnClickListLines(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListLines(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListLines(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListLines(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonFlat();
	HRESULT ItemChanged(WPARAM, LPARAM);
	afx_msg void OnSelchangeComboOptomizeWhat();
	afx_msg void OnCheckMoveRight();
	afx_msg void OnCheckNormalize();
	afx_msg void OnCheckPeakrx();
	afx_msg void OnCheckWp0();
	afx_msg void OnButtonMakeItSo();
	afx_msg void OnTaughtAlllinear();
	afx_msg void OnTaughtAllspline();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAUGHTLINESEDITDLG_H__E2B4922E_4FC3_4ABE_9761_7A19F3D869EA__INCLUDED_)
