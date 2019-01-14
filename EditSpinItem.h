#if !defined(AFX_EDITSPINITEM_H__A24316FF_AD03_42F4_824A_58E2553B3D3F__INCLUDED_)
#define AFX_EDITSPINITEM_H__A24316FF_AD03_42F4_824A_58E2553B3D3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditSpinItem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditSpinItem dialog

class CEditSpinItem : public CDialog
{
// Construction
public:
	void CreateFont();
	LOGFONT m_LogFont;
	CFont* m_pFont;
	float Round(float dValue,int nSd);
	double Round(double dValue,int nSd);
	CWnd** m_pDlg;
	CWnd* m_pParent;
	void Initialize(CString *pBuff);
	void Initialize(WCHAR *pStr);
	void Initialize(int *pnValue,CString* Buff,int nNumberItems);
	void Initialize(int *pnValue,int nMin,int nMax,int nStep,double dCoefficient,WCHAR *Format);
	void Initialize(double *pdValue,double dMin,double dMax,double dStep,WCHAR *Format);
	void Initialize(int *pnValue,int nMin,int nMax,int nStep);
	void Initialize(float *pfValue,float fMin,float fMax,float fStep,WCHAR *Format);
	void CEditSpinItem::Initialize(unsigned char *pcValue,CString* Buff,int nNumberItems);
	CRect m_Rect;
	int m_nVariableType;
	float	*m_pfValue;
	double *m_pdValue;
	double	m_dMin;
	double	m_dMax;
	double	m_dStep;
	double m_dCoefficient;
	int *m_pnValue;
	unsigned char *m_pcValue;
	int	m_nMin;
	int	m_nMax;
	int	m_nStep;
	int m_nNumberItems;
	int	m_nFontHeight;
	CString* m_pBuff;
	WCHAR *m_pStr;
	WCHAR m_Format[20];
	CEditSpinItem(CWnd* pParent = NULL,CWnd** pDlg = NULL,CRect* pRect=NULL,int *pnFontHeight=NULL);   // standard constructor
	~CEditSpinItem();

// Dialog Data
	//{{AFX_DATA(CEditSpinItem)
	enum { IDD = IDD_DIALOG_LIST_EDIT };
	CComboBox	m_comboValue;
	CSpinButtonCtrl	m_spinValue;
	CEdit	m_editValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditSpinItem)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditSpinItem)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEdit();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeCombo();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITSPINITEM_H__A24316FF_AD03_42F4_824A_58E2553B3D3F__INCLUDED_)
