#if !defined(AFX_ENVELOPETOOLSDLG_H__4ABEC4B9_E706_4576_9493_B95943D31B18__INCLUDED_)
#define AFX_ENVELOPETOOLSDLG_H__4ABEC4B9_E706_4576_9493_B95943D31B18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnvelopeToolsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEnvelopeToolsDlg dialog

class CEnvelopeToolsDlg : public CDialog
{
// Construction
public:
	void Process();
	void UpdateParent(int nAction);
	void UpdateAllControls();
	int m_nAlgorithm;
	CWnd* 	m_pParent;
	CEnvelopeToolsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEnvelopeToolsDlg)
	enum { IDD = IDD_TOOLSENVELOPE_DIALOG };
	CSpinButtonCtrl	m_spinTop;
	CSpinButtonCtrl	m_spinRight;
	CSpinButtonCtrl	m_spinLeft;
	CSpinButtonCtrl	m_spinBottom;
	CEdit	m_editTop;
	CEdit	m_editRight;
	CEdit	m_editLeft;
	CEdit	m_editBottom;
	CComboBox	m_comboEnlarge;
	CComboBox	m_comboWhichLines;
	CComboBox	m_comboAlgorithm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnvelopeToolsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEnvelopeToolsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDeltaposSpinTop(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinRight(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinLeft(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinBottom(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEditTop();
	afx_msg void OnChangeEditRight();
	afx_msg void OnChangeEditLeft();
	afx_msg void OnChangeEditBottom();
	afx_msg void OnSelchangeComboAlogorithm();
	afx_msg void OnSelchangeComboWhichlines();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENVELOPETOOLSDLG_H__4ABEC4B9_E706_4576_9493_B95943D31B18__INCLUDED_)
