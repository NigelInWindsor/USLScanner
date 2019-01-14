#if !defined(AFX_ENVELOPERECTANGULARDLG_H__49CDF748_C59A_4088_BD31_A903F1ABE608__INCLUDED_)
#define AFX_ENVELOPERECTANGULARDLG_H__49CDF748_C59A_4088_BD31_A903F1ABE608__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnvelopeRectangularDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEnvelopeRectangularDlg dialog

class CEnvelopeRectangularDlg : public CDialog
{
// Construction
public:
	void UpdateAllControls();
	
	CWnd**	m_pDlg;
	CEnvelopeRectangularDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEnvelopeRectangularDlg)
	enum { IDD = IDD_ENVELOPE_RECTANGULAR_DLG };
	CComboBox	m_comboGotoSide;
	CEdit	m_editStart;
	CEdit	m_editFinish;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnvelopeRectangularDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEnvelopeRectangularDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditStart();
	afx_msg void OnChangeEditFinish();
	afx_msg void OnButtonStoreStart();
	afx_msg void OnButtonStoreFinish();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeComboGotoSide();
	afx_msg void OnButtonGotoStart();
	afx_msg void OnButtonGotoFinish();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENVELOPERECTANGULARDLG_H__49CDF748_C59A_4088_BD31_A903F1ABE608__INCLUDED_)
