#if !defined(AFX_MACHINEDESCRIPTIONDLG_H__9735E069_2214_4D7E_82D4_DD4D36C49D80__INCLUDED_)
#define AFX_MACHINEDESCRIPTIONDLG_H__9735E069_2214_4D7E_82D4_DD4D36C49D80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MachineDescriptionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMachineDescriptionDlg dialog

class CMachineDescriptionDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CMachineDescriptionDlg)
// Construction
public:
	HBRUSH m_hBr;
	void Invalidate3DImages();
	CMachineDescriptionDlg();

// Dialog Data
	//{{AFX_DATA(CMachineDescriptionDlg)
	enum { IDD = IDD_MACHINE_DESCRIPTION_DLG };
	CComboBox	m_combo3DMachineDrawing;
	CButton	m_checkDisplayIn3D;
	CEdit	m_editTTZOffset;
	CEdit	m_editTTYOffset;
	CEdit	m_editTTXOffset;
	CEdit	m_editTTDiameter;
	CComboBox	m_comboCustomer;
	CEdit	m_editZOffset;
	CEdit	m_editZLength;
	CEdit	m_editYOffset;
	CEdit	m_editYLength;
	CEdit	m_editXOffset;
	CEdit	m_editXLength;
	CComboBox	m_comboStyle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMachineDescriptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMachineDescriptionDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeComboStyle();
	afx_msg void OnChangeEditXLength();
	afx_msg void OnChangeEditYLength();
	afx_msg void OnChangeEditZLength();
	afx_msg void OnChangeEditXOffset();
	afx_msg void OnChangeEditYOffset();
	afx_msg void OnChangeEditZOfset();
	afx_msg void OnSelchangeComboCustomer();
	afx_msg void OnChangeEditXTt();
	afx_msg void OnChangeEditYTt();
	afx_msg void OnChangeEditZTt();
	afx_msg void OnChangeEditTtDiameter();
	afx_msg void OnCheckDisplayTt();
	afx_msg void OnSelchangeCombo3dMachineDrawing();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACHINEDESCRIPTIONDLG_H__9735E069_2214_4D7E_82D4_DD4D36C49D80__INCLUDED_)
