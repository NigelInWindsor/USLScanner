#if !defined(AFX_3DLIGHTSPAGE_H__D9343B41_EFF9_42EE_A6E8_8C5A541CED0F__INCLUDED_)
#define AFX_3DLIGHTSPAGE_H__D9343B41_EFF9_42EE_A6E8_8C5A541CED0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 3DLightsPage.h : header file
//

#include "EditSpinItem.h"
/////////////////////////////////////////////////////////////////////////////
// C3DLightsPage dialog

class C3DLightsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(C3DLightsPage)

// Construction
public:
	CString m_strType[4];
	CEditSpinItem* m_pEditSpinItem;
	void FillList();
	void CreateColumns();
	CPropertySheet* m_pToolSheet;
	CPropertyPage* m_pViewPage;
	void RenderParent();
	int m_nIndex;
	
	void UpdateAllControls();
	C3DLightsPage(CPropertySheet* pToolSheet = NULL, CPropertyPage* pViewPage = NULL);
	~C3DLightsPage();

// Dialog Data
	//{{AFX_DATA(C3DLightsPage)
	enum { IDD = IDD_3D_LIGHTS_PAGE };
	CSliderCtrl	m_sliderAmbient;
	CListCtrl	m_listLights;
	CSpinButtonCtrl	m_spinTheta;
	CSpinButtonCtrl	m_spinPhi;
	CEdit	m_editTheta;
	CEdit	m_editPhi;
	CEdit	m_editLookAtPt;
	CEdit	m_editPosition;
	CComboBox	m_comboType;
	CComboBox	m_comboNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(C3DLightsPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(C3DLightsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditLookatpt();
	afx_msg void OnChangeEditPosition();
	afx_msg void OnChangeEditTheta();
	afx_msg void OnChangeEditPhi();
	afx_msg void OnDeltaposSpinTheta(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinPhi(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListLights(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListLights(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListLights(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusListLights(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HRESULT ItemChanged(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_3DLIGHTSPAGE_H__D9343B41_EFF9_42EE_A6E8_8C5A541CED0F__INCLUDED_)
