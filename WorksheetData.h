#if !defined(AFX_WORKSHEETDATA_H__9BAE34B2_B50E_4EBA_BA24_DDBB00E607F7__INCLUDED_)
#define AFX_WORKSHEETDATA_H__9BAE34B2_B50E_4EBA_BA24_DDBB00E607F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorksheetData.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CWorksheetData dialog

class CWorksheetData : public CDialog
{
// Construction
public:
	CWorksheetData(CWnd* pParent = NULL, CWnd** pDlg = NULL);   // standard constructor
	CWnd** m_pDlg;
	CWnd* m_pParent;

// Dialog Data
	//{{AFX_DATA(CWorksheetData)
	enum { IDD = IDD_WORKSHEET_DATA_DIALOG };
	CEdit	m_editData;
	CComboBox	m_comboItem;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorksheetData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWorksheetData)
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboItem();
	afx_msg void OnChangeEditData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSHEETDATA_H__9BAE34B2_B50E_4EBA_BA24_DDBB00E607F7__INCLUDED_)
