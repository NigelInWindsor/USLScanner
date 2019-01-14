#if !defined(AFX_THICKNESSCALIBRATIONPAGE_H__C738B6AE_267D_4610_96E3_375B550BBF14__INCLUDED_)
#define AFX_THICKNESSCALIBRATIONPAGE_H__C738B6AE_267D_4610_96E3_375B550BBF14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThicknessCalibrationpage.h : header file
//
#include "EditSpinItem.h"
#include "EditColour.h"
/////////////////////////////////////////////////////////////////////////////
// CThicknessCalibrationpage dialog

class CThicknessCalibrationpage : public CPropertyPage
{
	DECLARE_DYNCREATE(CThicknessCalibrationpage)

// Construction
public:
	void RenderThicknessAgainstTime(CPaintDC* pDC,CRect *rr);
	BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	CToolBar   m_wndToolBar;
	CReBar     m_wndRebar;
	HBRUSH m_hBr;
	void UpdateAllControls();
	void SetAccessPrivelage();
	CEditSpinItem* m_pEditSpinItem;
	void ClearEditSpin();
	CString m_Action[4];
	int m_nIndex;
	int	m_nColumn;
	float m_fTemp;
	
	void FillList();
	void CreateColumns();
	CThicknessCalibrationpage();
	~CThicknessCalibrationpage();

// Dialog Data
	//{{AFX_DATA(CThicknessCalibrationpage)
	enum { IDD = IDD_THICKNESS_CALIBRATION_DLG };
	CEditColour	m_editTestDate;
	CEdit	m_editPt1;
	CEdit	m_editPt0;
	CEdit	m_editCalibrationBlockName;
	CStatic	m_staticView;
	CSpinButtonCtrl	m_spinMaterialVelocity;
	CSpinButtonCtrl	m_spinMaterialConstant;
	CComboBox	m_comboMaterialVelocityMode;
	CEdit	m_editMaterialVelocity;
	CEdit	m_editMaterialConstant;
	CListCtrl	m_listCoords;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CThicknessCalibrationpage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CThicknessCalibrationpage)
	afx_msg void OnGetdispinfoListCal(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	afx_msg void OnRclickListCal(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCalibrationAdd();
	afx_msg void OnCalibrationModifyCoord();
	afx_msg void OnCalibrationUpdatetime();
	afx_msg void OnCalibrationGoto();
	afx_msg void OnDblclkListCal(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListCal(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonComputeLookup();
	afx_msg void OnButtonCalibrateThickness();
	afx_msg void OnCalibrationDelete();
	afx_msg void OnPaint();
	afx_msg void OnChangeEditMaterialConstant();
	afx_msg void OnChangeEditMaterialVelocity();
	afx_msg void OnSelchangeComboMaterialVelocityMode();
	HRESULT TableChanged(WPARAM, LPARAM);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileSaveas();
	afx_msg void OnFileOpen();
	afx_msg void OnCalibrationSort();
	afx_msg void OnChangeEditPt0();
	afx_msg void OnChangeEditPt1();
	afx_msg void OnButtonCalibrate();
	afx_msg void OnButtonCheckGauge();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THICKNESSCALIBRATIONPAGE_H__C738B6AE_267D_4610_96E3_375B550BBF14__INCLUDED_)
