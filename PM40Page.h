#pragma once
class CPM40Page : public CPropertyPage
{
	DECLARE_DYNCREATE(CPM40Page)

public:
	HBRUSH m_hBr;
	CPM40Page();
	~CPM40Page();

	// Dialog Data
	//{{AFX_DATA(CSI10Page)
	enum { IDD = IDD_PM40_PAGE };
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSI10Page)
public:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	// Generated message map functions
	//{{AFX_MSG(CSI10Page)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	void UpdateAllControls();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	CButton m_buttonConnect;
	CEdit m_editDLLVersion;
	afx_msg void OnBnClickedButtonConnect();
	CEdit m_editDriverVersion;
	CEdit m_editConversionCount;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_editTimedOutCount;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonDmaCap();
	afx_msg void OnBnClickedButtonTestPattern();
	CEdit m_editElapsedTime;
	CEdit m_editIrqCounts;
	afx_msg void OnBnClickedCheckConnectAtStartup();
	afx_msg void OnBnClickedButtonIrqCount();
	CEdit m_editMailboxMaxWait;
	afx_msg void OnBnClickedButtonResetAllTs();
	CEdit m_editCountsTS1;
	CEdit m_editCountsTS2;
	CEdit m_editCountsTS3;
	afx_msg void OnBnClickedButtonResetCounters();
	CEdit m_editGeneral;
	afx_msg void OnBnClickedButtonHwReset();
	afx_msg void OnEnChangeRichedit21();
};

