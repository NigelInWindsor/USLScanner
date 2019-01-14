#if !defined(AFX_ANNOTATEDLG_H__2B88BD45_E12F_4620_9476_4474AC8D2E37__INCLUDED_)
#define AFX_ANNOTATEDLG_H__2B88BD45_E12F_4620_9476_4474AC8D2E37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnnotateDlg.h : header file
//
#include "EditSpinItem.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CAnnotateDlg dialog

class CAnnotateDlg : public CDialog
{
// Construction
public:
	void InvalidateParent();
	void UpdateAllControls();
	void BlockFill(CPaintDC * pDC, CStatic * mStatic, COLORREF rgbColor);
	CString m_Buffer;
	void ClearEditSpin();
	CEditSpinItem* m_pEditSpinItem;
	void FillList();
	void CreateColumns();
	int	m_nEditWhich;
	int	m_nImageType;
	int	m_nImageNumber;
	float m_fXp;
	float m_fYp;
	CPoint m_pt;
	CData* m_pData;
	int	m_nIndex;
	
	CWnd* m_pParent;
	CAnnotateDlg(CWnd* pParent = NULL,CData* pData = NULL,float fXp=0.0,float fYp=0.0,CPoint* pt=NULL,int nEditWhich=-1,int nImageType=0,int nImageNumber=0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnnotateDlg)
	enum { IDD = IDD_ANNOTATE_DIALOG };
	CButton	m_checkThisImage;
	CButton	m_checkAllImages;
	CListCtrl	m_listReasons;
	CEdit	m_editID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnnotateDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnnotateDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnButtonFont();
	afx_msg void OnGetdispinfoListReasons(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListReasons(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListReasons(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListReasons(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditAdd();
	afx_msg HRESULT ItemChanged(WPARAM, LPARAM);
	afx_msg void OnEditDelete();
	afx_msg void OnCheckThisImage();
	afx_msg void OnCheckAllImages();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnChangeEditId();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_staticBkColor;
	CComboBox m_comboBkMode;
	afx_msg void OnCbnSelchangeComboBackgroundMode();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANNOTATEDLG_H__2B88BD45_E12F_4620_9476_4474AC8D2E37__INCLUDED_)
