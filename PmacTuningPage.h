#if !defined(AFX_PMACTUNINGPAGE_H__9760A964_8B9D_49C8_8253_1A596FE37FB8__INCLUDED_)
#define AFX_PMACTUNINGPAGE_H__9760A964_8B9D_49C8_8253_1A596FE37FB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PmacTuningPage.h : header file
//
#include "EditSpinItem.h"
#include "ResizablePage.h"

/////////////////////////////////////////////////////////////////////////////
// CPmacTuningPage dialog

class CPmacTuningPage : public CResizablePage
{
	DECLARE_DYNCREATE(CPmacTuningPage)

// Construction
public:
	HBRUSH m_hBr;
	void CalculateNotchFilter(bool bDownLoad);
	int	m_nMoveWhich;
	bool m_bLBDown;
	int	m_pnX[2];
	CRect m_rrClient;
	void AddAnchor(CWnd *pCtrl);
	AxisData *m_pAxis;
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	int m_nOldPos;
	float m_fStepSize;
	float	m_fPos[3][16];
	void DrawMoveTrace(CPaintDC* pDC,CRect rr);
	int	m_nDistance;
	int	m_nTest;
	void ClearEditSpin();
	long UpdatePmacFromList(unsigned int nTemp,long nlTemp);
	void InitializeStrings();
	CString m_strValue[20];
	CString m_strDescription[20];
	int m_nIndex;
	int	m_nListEdit;
	CEditSpinItem* m_pEditSpinItem;

	CString m_strDecodeDescription[8];
	CString	m_strEncoderDecode[16];
	int	m_nEncoderAddrs[16];
	int	m_nEncoderDecode[16];

	
	int m_nOffset[20];
	int m_nAxisNumber;
	
	void CreateColumns();
	void FillList();
	void UpdateAllControls();
	CPmacTuningPage();
	~CPmacTuningPage();

// Dialog Data
	//{{AFX_DATA(CPmacTuningPage)
	enum { IDD = IDD_PMAC_TUNING_PAGE };
	CListCtrl	m_listEncoders;
	CButton	m_checkVelocity;
	CButton	m_checkDACVoltage;
	CButton	m_checkFollowingError;
	CEdit	m_editIX30Gain;
	CEdit	m_editBandPassDamping;
	CEdit	m_editBandPassFrequency;
	CEdit	m_editNotchDamping;
	CEdit	m_editNotchFrequency;
	CEdit	m_editPosition;
	CEdit	m_editEncoderCount;
	CEdit	m_editPos2;
	CEdit	m_editPos1;
	CEdit	m_editPos0;
	CEdit	m_editStepSize;
	CEdit	m_editInPosMultiplier;
	CEdit	m_editTA;
	CEdit	m_editTS;
	CEdit	m_editDistance;
	CComboBox	m_comboTest;
	CStatic	m_staticView;
	CListCtrl	m_listVariables;
	CComboBox	m_comboAxisNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPmacTuningPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPmacTuningPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeComboAxisNumber();
	afx_msg void OnPaint();
	afx_msg void OnGetdispinfoListVariable(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListVariable(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListVariable(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDoIt();
	afx_msg void OnChangeEditTa();
	afx_msg void OnChangeEditTs();
	afx_msg void OnButtonGoto0();
	afx_msg void OnButtonGoto1();
	afx_msg void OnButtonGoto2();
	afx_msg void OnChangeEditmultiply();
	afx_msg void OnChangeEditPos0();
	afx_msg void OnChangeEditPos1();
	afx_msg void OnChangeEditPos2();
	afx_msg void OnChangeEditStepsize();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonNotchFilter();
	afx_msg void OnButtonFactory();
	afx_msg void OnButtonStore();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCheckDacVoltage();
	afx_msg void OnCheckFollowingError();
	afx_msg void OnCheckVelocity();
	afx_msg void OnDblclkListEncoders(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListEncoders(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListEncoders(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMACTUNINGPAGE_H__9760A964_8B9D_49C8_8253_1A596FE37FB8__INCLUDED_)
