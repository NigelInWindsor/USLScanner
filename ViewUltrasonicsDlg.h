#if !defined(AFX_VIEWULTRASONICSDLG_H__1B70EB4E_7C48_463A_8DC8_E6D1A2637ACB__INCLUDED_)
#define AFX_VIEWULTRASONICSDLG_H__1B70EB4E_7C48_463A_8DC8_E6D1A2637ACB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewUltrasonicsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewUltrasonicsDlg dialog

class CViewUltrasonicsDlg : public CDialog
{
// Construction
public:
	void CalculateGateNumber();
	void SetAccessPrivelage();
	void FillList();
	void CreateColumnList();
	void UpdateAllControls();
	int m_nTS;
	int	m_nTOF;
	int	m_nGate;
	int	m_nImageType;
	int	m_nImageNumber;
	CData* m_pData;
	
	CWnd** m_pDlg;
	CString m_Units[2];
	CViewUltrasonicsDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,CData* pData = NULL,int nTS = 0, int nImageType = 0, int nImageNumber = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CViewUltrasonicsDlg)
	enum { IDD = IDD_VIEW_ULTRASONICS_DLG };
	CComboBox	m_comboGateNumber;
	CEdit	m_editThicknessMode;
	CEdit	m_editCentreFrequency;
	CEdit	m_editRectifierDecay;
	CEdit	m_editRectifierMode;
	CEdit	m_editLogPostFilter;
	CEdit	m_editCutOffPoint;
	CEdit	m_editLogGain;
	CEdit	m_editLinGain;
	CButton	m_buttonGroup;
	CStatic	m_text2;
	CStatic	m_text1;
	CStatic	m_text0;
	CListCtrl	m_listDac;
	CString	m_strAmplifier;
	CString	m_strDacBlanking;
	CString	m_strDacMode;
	CString	m_strDacTriggerThreshold;
	CString	m_strDacTrigger;
	CString	m_strFilterRange;
	CString	m_strFilterType;
	CString	m_strPreampDamping;
	CString	m_strPreampGain;
	CString	m_strPreampMode;
	CString	m_strPRF;
	CString	m_strPulseVoltage;
	CString	m_strPulseWidth;
	CString	m_strReceiver;
	CString	m_strTransmitter;
	CString	m_staticBlankingText;
	CString	m_strGateDelay;
	CString	m_strGateThreshold;
	CString	m_strGateWidth;
	CString	m_strTOFMode;
	CString	m_strGateMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewUltrasonicsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CViewUltrasonicsDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnGetdispinfoListDacList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboGateNumber();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWULTRASONICSDLG_H__1B70EB4E_7C48_463A_8DC8_E6D1A2637ACB__INCLUDED_)
