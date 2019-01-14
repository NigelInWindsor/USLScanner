#if !defined(AFX_TRANSPLYPAGE_H__79ADFE2A_836F_409C_BADD_0F7E54752307__INCLUDED_)
#define AFX_TRANSPLYPAGE_H__79ADFE2A_836F_409C_BADD_0F7E54752307__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransPlyPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTransPlyPage dialog

class CTransPlyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTransPlyPage)

// Construction
public:
	bool		m_bUseCalPath[2];
	int			m_nSentence[2];
	CString		m_FileName[2];
	int			m_nOldScanLine;
	void		SetAccessPrivelage();
	BOOL		OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	void		SetToolBarCheckedState();
	CToolBar	m_wndToolBar;
	CReBar		m_wndRebar;
	
	void UpdateAllControls();
	CTransPlyPage();
	~CTransPlyPage();

// Dialog Data
	//{{AFX_DATA(CTransPlyPage)
	enum { IDD = IDD_TRANSPLY_PAGE };
	CEdit	m_editGain2;
	CEdit	m_editGain1;
	CSpinButtonCtrl	m_spinGain2;
	CSpinButtonCtrl	m_spinGain1;
	CSpinButtonCtrl	m_spinPulseWidth2;
	CSpinButtonCtrl	m_spinPulseWidth1;
	CEdit	m_editPulseWidth2;
	CEdit	m_editPulseWidth1;
	CComboBox	m_comboSentence1;
	CComboBox	m_comboSentence0;
	CEdit	m_editUniqueID1;
	CEdit	m_editUniqueID0;
	CEdit	m_editProbe1;
	CEdit	m_editProbe0;
	CSpinButtonCtrl	m_spinScanSpeed;
	CEdit	m_editScanSpeed;
	CEdit	m_editResumeLine;
	CEdit	m_editScanLine;
	CSpinButtonCtrl	m_spinSlowIncrement;
	CSpinButtonCtrl	m_spinFastIncrement;
	CEdit	m_editSlowIncrement;
	CEdit	m_editFastIncrement;
	CEdit	m_editFinishPt;
	CEdit	m_editStartPt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTransPlyPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTransPlyPage)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChangeEditPt0();
	afx_msg void OnChangeEditPt1();
	afx_msg void OnChangeEditIncrement0();
	afx_msg void OnChangeEditIncrement1();
	afx_msg void OnDeltaposSpinIncrement0(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinIncrement1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinSpeed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonStoreStart();
	afx_msg void OnButtonStoreFinish();
	afx_msg void OnButtonGotoStart();
	afx_msg void OnButtonGotoFinish();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonAbort();
	afx_msg void OnChangeEditUniqueId0();
	afx_msg void OnChangeEditUniqueId1();
	afx_msg void OnChangeEditProbe0();
	afx_msg void OnChangeEditProbe1();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveas();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeEditResumeLine();
	afx_msg void OnSelchangeComboSentence0();
	afx_msg void OnSelchangeComboSentence1();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonCal0();
	afx_msg void OnButtonCal1();
	afx_msg void OnChangeEditGain();
	afx_msg void OnChangeEditGain2();
	afx_msg void OnChangeEditPulseWidth();
	afx_msg void OnChangeEditPulseWidth2();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSPLYPAGE_H__79ADFE2A_836F_409C_BADD_0F7E54752307__INCLUDED_)
