#if !defined(AFX_IJKPAGE_H__26277663_38DD_4A4D_8072_084DECD07BFF__INCLUDED_)
#define AFX_IJKPAGE_H__26277663_38DD_4A4D_8072_084DECD07BFF__INCLUDED_

#include "Coord.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IJKPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIJKPage dialog

class CIJKPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CIJKPage)

// Construction
public:
	void UpdateAllControls();
	CCoord m_CpOldSurface;
	CCoord m_CpOldHead;
	CCoord m_CpSurface;
	CCoord m_CpHead;
	int		m_nEditTipsNormals;
	HBRUSH m_hBr;
	
	CIJKPage();
	~CIJKPage();

// Dialog Data
	//{{AFX_DATA(CIJKPage)
	enum { IDD = IDD_IJK_PAGE };
	CEdit	m_editRotateAboutZ;
	CSpinButtonCtrl	m_spinRotateAboutZ;
	CSpinButtonCtrl	m_spinYTRight;
	CSpinButtonCtrl	m_spinIRight;
	CSpinButtonCtrl	m_spinJRight;
	CSpinButtonCtrl	m_spinKRight;
	CSpinButtonCtrl	m_spinXTRight;
	CSpinButtonCtrl	m_spinKLeft;
	CSpinButtonCtrl	m_spinJLeft;
	CSpinButtonCtrl	m_spinILeft;
	CSpinButtonCtrl	m_spinYTipLeft;
	CSpinButtonCtrl	m_spinXTipLeft;
	CComboBox	m_comboVirtualActual;
	CEdit	m_editLeftW;
	CEdit	m_editRightW;
	CEdit	m_editLeftXTip2;
	CEdit	m_editRightYTip2;
	CEdit	m_editRightXTip2;
	CEdit	m_editLeftYTip2;
	CEdit	m_editZ;
	CEdit	m_editY;
	CEdit	m_editX;
	CEdit	m_editRightYt;
	CEdit	m_editRightXt;
	CEdit	m_editRightK;
	CEdit	m_editRightJ;
	CEdit	m_editRightI;
	CEdit	m_editLeftYt;
	CEdit	m_editLeftXt;
	CEdit	m_editLeftK;
	CEdit	m_editLeftJ;
	CEdit	m_editLeftI;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CIJKPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CIJKPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeEditX();
	afx_msg void OnChangeEditY();
	afx_msg void OnChangeEditZ();
	afx_msg void OnChangeEditLeftXt();
	afx_msg void OnChangeEditLeftYt();
	afx_msg void OnSelchangeComboVirtualActual();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditRotateAboutZ();
	afx_msg void OnDeltaposSpinRotateAboutZ(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IJKPAGE_H__26277663_38DD_4A4D_8072_084DECD07BFF__INCLUDED_)
