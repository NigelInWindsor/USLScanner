#if !defined(AFX_HISTOGRAMTHRESHOLDDLG_H__F33A6EFF_7864_4E4A_B059_63CC2B108789__INCLUDED_)
#define AFX_HISTOGRAMTHRESHOLDDLG_H__F33A6EFF_7864_4E4A_B059_63CC2B108789__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistogramThresholdDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHistogramThresholdDlg dialog

class CHistogramThresholdDlg : public CDialog
{
// Construction
public:
	
	void UpdateAllControls();
	CData* m_pData;
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CHistogramThresholdDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL, CData* pData = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHistogramThresholdDlg)
	enum { IDD = IDD_HISTOGRAM_THRESHOLD_DLG };
	CSpinButtonCtrl	m_spinThreshold1;
	CSpinButtonCtrl	m_spinThreshold0;
	CEdit	m_editThreshold1;
	CEdit	m_editThreshold0;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistogramThresholdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHistogramThresholdDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditThreshold0();
	afx_msg void OnChangeEditThreshold1();
	afx_msg void OnDeltaposSpinThreshold0(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinThreshold1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTOGRAMTHRESHOLDDLG_H__F33A6EFF_7864_4E4A_B059_63CC2B108789__INCLUDED_)
