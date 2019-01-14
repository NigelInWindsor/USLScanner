#if !defined(AFX_PRINTOPTIONSDLG_H__DEA62584_BDB0_47D7_8076_FB4E98615804__INCLUDED_)
#define AFX_PRINTOPTIONSDLG_H__DEA62584_BDB0_47D7_8076_FB4E98615804__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintOptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrintOptionsDlg dialog

class CPrintOptionsDlg : public CDialog
{
// Construction
public:
	void InvalidateParent();
	void UpdateAllControls();
	void OnCheckChangeListOptions();
	CData* m_pData;
	CWnd* m_pParent;
	CWnd** m_pDlg;
	CPrintOptionsDlg(CWnd* pParent = NULL, CWnd** pDlg = NULL,CData* pData = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrintOptionsDlg)
	enum { IDD = IDD_PRINT_OPTIONS_DLG };
	CStatic	m_staticColor;
	CButton	m_checkTransparent;
	CSpinButtonCtrl	m_spinFontSize;
	CEdit	m_editFontSize;
	CComboBox	m_comboRaxisMode;
	CSpinButtonCtrl	m_spinGraticuleStartAngle;
	CEdit	m_editGraticuleStartAngle;
	CEdit	m_editPaperWidth;
	CSpinButtonCtrl	m_spinScanHeight;
	CSpinButtonCtrl	m_spinLowerDiameter;
	CSpinButtonCtrl	m_spinUpperDiameter;
	CEdit	m_editComponentHeight;
	CEdit	m_editLowerDiameter;
	CEdit	m_editUpperDiameter;
	CCheckListBox	m_listOptions;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrintOptionsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListOptions();
	virtual void OnOK();
	afx_msg void OnButtonApply();
	afx_msg void OnDeltaposSpinScanHeight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinLowerDiameter(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinUpperDiameter(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinGraticuleStartAngle(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnButtonCone11();
	afx_msg void OnSelchangeComboRAxisUnits();
	afx_msg void OnChangeEditFontSize();
	afx_msg void OnDeltaposSpinFontSize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckTransparent();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChangeEditGraticuleStartAngle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTOPTIONSDLG_H__DEA62584_BDB0_47D7_8076_FB4E98615804__INCLUDED_)
