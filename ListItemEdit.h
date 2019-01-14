#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CListItemEdit dialog

class CListItemEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CListItemEdit)
public:
	enum VariableType {
		INT = 0,
		UINT,
		FLOAT,
		DOUBLE,
		COMBO,
		_WCHAR,
		STRING,
	};


protected:
	WCHAR *			m_pwcArray;
	double*			m_pdValue;
	float*			m_pfValue;
	float			m_fMin;
	float			m_fMax;
	float			m_fStep;
	CString			m_Format;
	VariableType	m_Type;
	int*			m_pnValue;
	int				m_nMin;
	int				m_nMax;
	int				m_nStep;
	int				m_nNumberItems;
	int				m_nMaxArrayLength;
	CString*		m_pComboStrings;

public:

	void	CreateFont();
	CRect	m_rrWnd;
	CRect *m_pRect;
	CFont	m_Font;
	CFont*	m_pFont;
	LPNMITEMACTIVATE m_pNMItemActivate;
	CListCtrl* m_pListCtrl;
	CWnd** m_pDlg;
	CWnd* m_pParent;
	CListItemEdit(CWnd* pParent = NULL, CWnd** pDlg = NULL, CListCtrl* pListCtrl = NULL, LPNMITEMACTIVATE pNMItemActivate = NULL, CRect *pRect = NULL);   // standard constructor
	virtual ~CListItemEdit();

// Dialog Data
	enum { IDD = IDD_LIST_ITEM_EDIT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	void SetWindowPosition(CRect rr);
	CComboBox m_comboValue;
	CEdit m_editValue;
	CSpinButtonCtrl m_spinValue;
	afx_msg void OnDeltaposSpinItem(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL Create(int nIID, CWnd* pParentWnd, float* pfValue, float fMin, float fMax, float fStep, CString Format);
	virtual BOOL Create(int nIID, CWnd * pParentWnd, double * pdValue, float fMin, float fMax, float fStep, CString Format);
	virtual BOOL Create(int nIID, CWnd* pParentWnd, int* pnValue, int nMin, int nMax, int nStep, CString Format);
	virtual BOOL Create(int nIID, CWnd * pParentWnd, int * pnValue, CString * Buff, int nNumberItems);
	virtual BOOL Create(int nIID, CWnd * pParentWnd, WCHAR * pwcArray, int nMaxLength, CString Format);
	afx_msg void OnChangeEditItem();
	afx_msg void OnSelchangeComboItem();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
