#if !defined(AFX_SENTENCEDLG_H__3AD00EF1_4ADC_49EE_B245_F219945948F0__INCLUDED_)
#define AFX_SENTENCEDLG_H__3AD00EF1_4ADC_49EE_B245_F219945948F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SentenceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSentenceDlg dialog

class CSentenceDlg : public CDialog
{
// Construction
public:
	CData *m_pData;
	CSentenceDlg(CWnd* pParent = NULL,CData *pData = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSentenceDlg)
	enum { IDD = IDD_SENTENCE_DLG };
	CComboBox	m_comboSentence;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSentenceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSentenceDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENTENCEDLG_H__3AD00EF1_4ADC_49EE_B245_F219945948F0__INCLUDED_)
