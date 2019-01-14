#if !defined(AFX_COPYCOORDSDLG_H__4838B563_893C_4417_BCF1_4A21D1CB39B2__INCLUDED_)
#define AFX_COPYCOORDSDLG_H__4838B563_893C_4417_BCF1_4A21D1CB39B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CopyCoordsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCopyCoordsDlg dialog

class CCopyCoordsDlg : public CDialog
{
// Construction
public:
	void UpdateAllControls();
	CCopyCoordsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCopyCoordsDlg)
	enum { IDD = IDD_COPY_COORDS_DIALOG };
	CComboBox	m_comboMode;
	CEdit	m_editNumberPts;
	CComboBox	m_comboPercentageLines;
	CComboBox	m_comboDestinationLines;
	CComboBox	m_comboSourceLines;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCopyCoordsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCopyCoordsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPYCOORDSDLG_H__4838B563_893C_4417_BCF1_4A21D1CB39B2__INCLUDED_)
