#if !defined(AFX_EVALUATIONDLG_H__6A9744DF_B301_409B_83A0_9C9ED4A969CF__INCLUDED_)
#define AFX_EVALUATIONDLG_H__6A9744DF_B301_409B_83A0_9C9ED4A969CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EvaluationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEvaluationDlg dialog
#include <atlbase.h>

class CEvaluationDlg : public CDialog
{
// Construction
public:
	void	GetCodeForADate();
	int		OldCheckReceivingCode();
	CString *GetExpiryDate();
	void	SetSelfDestructDate(CString Name, int nMonths=0);
	int		GetHowManyDaysLeft();
	void	CreateSendingCode(CString *Code);
	int		CheckReceivingCode();
	BYTE	GetVersionNumber();
	BYTE	ReverseBits(BYTE input);
	bool	FindSelfDestructDate(CString Name, time_t * pSelfDestructDate);
	void	SetMachineName(CString Name);
	bool	CheckifEvalCode();

	time_t	m_nSelfDestructDate;
	int		m_nHowManyDaysLeft;
	char	m_MachineID[100];
	int		m_nErrorCode;
	BYTE	m_SendByte[12];



	CEvaluationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEvaluationDlg)
	enum { IDD = IDD_EVALUATION_DIALOG };
	CEdit	m_editCode;
	CEdit	m_editWord6;
	CEdit	m_editWord5;
	CEdit	m_editWord4;
	CEdit	m_editWord3;
	CEdit	m_editWord2;
	CEdit	m_editWord1;
	CString	m_stringText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEvaluationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEvaluationDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeEditRecWord1();
	afx_msg void OnChangeEditRecWord2();
	afx_msg void OnChangeEditRecWord3();
	afx_msg void OnChangeEditRecWord4();
	afx_msg void OnChangeEditRecWord5();
	afx_msg void OnChangeEditRecWord6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVALUATIONDLG_H__6A9744DF_B301_409B_83A0_9C9ED4A969CF__INCLUDED_)
