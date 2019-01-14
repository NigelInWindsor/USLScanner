#if !defined(AFX_BLADESTATISTICSDLG_H__89ECE742_5742_4C3F_9131_8BCB11FBC97C__INCLUDED_)
#define AFX_BLADESTATISTICSDLG_H__89ECE742_5742_4C3F_9131_8BCB11FBC97C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BladeStatisticsDlg.h : header file
//
#include "EditSpinItem.h"
#include "StaticPosManage.h"

/////////////////////////////////////////////////////////////////////////////
// CBladeStatisticsDlg dialog

class CBladeStatisticsDlg : public CDialog
{
// Construction
public:
	void setScanType(int nScanType);
	void SetWindowTitle();
	HRESULT UpdateMessage(WPARAM, LPARAM);
	void UpdateAllControls();
	int m_nIndex;
	void FillList();
	void CreateColumns();

	CStaticPosManage m_StaticPosList;
	CEditSpinItem* m_pEditSpinItem;
	int m_nScanType;
	CData* m_pData;
	CWnd** m_pDlg;
	
	CBladeStatisticsDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL,CData* pData = NULL,int nScanType = 0);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBladeStatisticsDlg)
	enum { IDD = IDD_BLADE_STATISTICS };
	CListCtrl	m_listCoords;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBladeStatisticsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBladeStatisticsDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetdispinfoListCoords(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLADESTATISTICSDLG_H__89ECE742_5742_4C3F_9131_8BCB11FBC97C__INCLUDED_)
