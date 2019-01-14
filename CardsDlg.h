#if !defined(AFX_CARDSDLG_H__DDC5DF1F_74C3_4631_820E_05A5AE0F6C84__INCLUDED_)
#define AFX_CARDSDLG_H__DDC5DF1F_74C3_4631_820E_05A5AE0F6C84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CardsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCardsDlg dialog

class CCardsDlg : public CDialog
{
// Construction
public:
	CCardsDlg(CWnd* pParent = NULL);   // standard constructor

	void UpdateCommsItems();
	void UpdateDataCollectionItems();
	bool m_bRestartADC;
	int m_nWhichADC;
	int m_nWhichPM30;

// Dialog Data
	//{{AFX_DATA(CCardsDlg)
	enum { IDD = IDD_CARDS_DIALOG };
	CComboBox	m_comboUmbilicalChannel;
	CButton	m_staticPM30;
	CComboBox	m_comboPM30Number;
	CComboBox	m_comboADCNumber;
	CEdit	m_editADCNumber;
	CButton	m_buttonResetMailbox;
	CButton	m_checkMailBox;
	CButton	m_checkSi10;
	CEdit	m_editUSBI2C;
	CButton	m_checkUSBI2C;
	CComboBox	m_comboADCInput2;
	CComboBox	m_comboADCInput1;
	CComboBox	m_comboMuxVersion;
	CComboBox	m_comboPM30Address_0;
	CComboBox	m_comboPr30Version_0;
	CComboBox	m_comboPr30Attached_0;
	CComboBox	m_comboPRFSource;
	CComboBox	m_comboLogVersion;
	CComboBox	m_comboLinearVersion;
	CEdit	m_editDelayClkPeriod;
	CButton	m_checkPr30_0;
	CComboBox	m_comboAdcCardType;
	CEdit	m_editAdc200MemoryAddrs;
	CEdit	m_editDspAddrs;
	CButton	m_checkDsp;
	CEdit	m_editMscAddrs;
	CButton	m_checkMsc;
	CEdit	m_editPr50Addrs;
	CEdit	m_editPr20Addrs;
	CEdit	m_editMailBoxAddrs;
	CEdit	m_editATPAddrs;
	CEdit	m_editAdc100Addrs;
	CButton	m_checkPr50;
	CButton	m_checkPr20;
	CButton	m_checkAtpTims0;
	CButton	m_checkAtpGpd1;
	CButton	m_checkAtpGpd0;
	CButton	m_checkAdc100;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCardsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCardsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonResetMailbox();
	afx_msg void OnChangeEditDelayClk();
	afx_msg void OnSelchangeComboPm30Address();
	afx_msg void OnSelchangeComboAdcInput1();
	afx_msg void OnSelchangeComboAdcInput2();
	afx_msg void OnSelchangeComboPm30Number();
	afx_msg void OnSelchangeComboAdcNumber();
	afx_msg void OnCheckPr30();
	afx_msg void OnSelchangeComboPr30Version();
	afx_msg void OnSelchangeComboPr30Pulser();
	afx_msg void OnSelchangeComboUmbilicalChannel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARDSDLG_H__DDC5DF1F_74C3_4631_820E_05A5AE0F6C84__INCLUDED_)
