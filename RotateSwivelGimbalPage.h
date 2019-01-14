#if !defined(AFX_ROTATESWIVELGIMBALPAGE_H__3E834874_4B9E_403F_9B7B_261EC59555C5__INCLUDED_)
#define AFX_ROTATESWIVELGIMBALPAGE_H__3E834874_4B9E_403F_9B7B_261EC59555C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RotateSwivelGimbalPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRotateSwivelGimbalPage dialog

class CRotateSwivelGimbalPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRotateSwivelGimbalPage)

// Construction
public:
	void UpdateAllControls();
	bool m_bVirtual;
	CCoord m_CpOldActual;
	CCoord m_CpOldForward;
	CCoord m_CpOldInverse;
	
	CRotateSwivelGimbalPage();
	~CRotateSwivelGimbalPage();

// Dialog Data
	//{{AFX_DATA(CRotateSwivelGimbalPage)
	enum { IDD = IDD_ROT_SWIVEL_GIMBAL_PAGE };
	CEdit	m_editLimb1;
	CEdit	m_editLimb0;
	CSpinButtonCtrl	m_spinSwivel;
	CSpinButtonCtrl	m_spinRotate;
	CSpinButtonCtrl	m_spinGimbal;
	CButton	m_checkvirtual;
	CEdit	m_editInverseX;
	CEdit	m_editInverseY;
	CEdit	m_editInverseZ;
	CEdit	m_editInverseRotate;
	CEdit	m_editInverseSwivel;
	CEdit	m_editInverseGimbal;
	CEdit	m_editForwardX;
	CEdit	m_editForwardY;
	CEdit	m_editForwardZ;
	CEdit	m_editForwardI;
	CEdit	m_editForwardJ;
	CEdit	m_editForwardK;
	CEdit	m_editActualX;
	CEdit	m_editActualY;
	CEdit	m_editActualZ;
	CEdit	m_editActualRotate;
	CEdit	m_editActualSwivel;
	CEdit	m_editActualGimbal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRotateSwivelGimbalPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRotateSwivelGimbalPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonHmzRotswivelgimbal();
	afx_msg void OnButtonRotateToZero();
	afx_msg void OnCheckVirtual();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditLimb0();
	afx_msg void OnChangeEditLimb1();
	afx_msg void OnChangeEditActualX();
	afx_msg void OnChangeEditActualY();
	afx_msg void OnChangeEditActualZ();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROTATESWIVELGIMBALPAGE_H__3E834874_4B9E_403F_9B7B_261EC59555C5__INCLUDED_)
