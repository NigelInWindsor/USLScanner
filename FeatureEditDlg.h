#if !defined(AFX_FEATUREEDITDLG_H__9571398E_FCF2_4A94_B38F_41226F3628F5__INCLUDED_)
#define AFX_FEATUREEDITDLG_H__9571398E_FCF2_4A94_B38F_41226F3628F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FeatureEditDlg.h : header file
//
#include "EditSpinItem.h"
#include "StaticPosManage.h"

/////////////////////////////////////////////////////////////////////////////
// CFeatureEditDlg dialog

class CFeatureEditDlg : public CDialog
{
// Construction
public:
	float m_fTop;
	float m_fBottom;
	float m_fLeft;
	float m_fRight;
	void CloseEditSpin();
	void SetIndex(int nIndex);
	HRESULT TableChanged(WPARAM, LPARAM);
	CString m_DataValueType[400];
	CString m_Type[20];
	void InitializeArrays();
	void FillList();
	void CreateColumns();
	int m_nIndex;
	CStaticPosManage m_StaticPosList;
	CEditSpinItem* m_pEditSpinItem;
	void UpdateAllControls();
	
	CWnd* m_pParent;
	CWnd** m_pDlg;
	int*	m_pnFeatureL;
	FeatureStruct* m_pFeature;
	CFeatureEditDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL, FeatureStruct* pFeature=NULL,int* pnFeatureL = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFeatureEditDlg)
	enum { IDD = IDD_FEATURE_EDIT_DIALOG };
	CListCtrl	m_listFeature;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFeatureEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFeatureEditDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickListFeature(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListFeature(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListFeature(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListFeature(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFeatureDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FEATUREEDITDLG_H__9571398E_FCF2_4A94_B38F_41226F3628F5__INCLUDED_)
