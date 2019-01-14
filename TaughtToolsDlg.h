#if !defined(AFX_TAUGHTTOOLSDLG_H__5F21CCD6_5F20_493D_BA39_756A42F4925F__INCLUDED_)
#define AFX_TAUGHTTOOLSDLG_H__5F21CCD6_5F20_493D_BA39_756A42F4925F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TaughtToolsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaughtToolsDlg dialog

class CTaughtToolsDlg : public CDialog
{
// Construction
public:
	CPolyCoord* m_pCopyLine;
	int	m_nCopyLineL;
	void InvalidateStartAndFinishValue();
	float	m_fWaterPath;
	int	m_nStartLine;
	int m_nFinishLine;
	void UpdateProfileSheet();
	float m_fExtrapolateValue;
	float m_fSliceOffValue;
	void UpdateMainFrameTools();
	void UpdateAllControls();
	
	CWnd** m_pDlg;
	CTaughtToolsDlg(CWnd* pParent = NULL,CWnd** pDlg = NULL);   // standard constructor
	~CTaughtToolsDlg();

// Dialog Data
	//{{AFX_DATA(CTaughtToolsDlg)
	enum { IDD = IDD_TOOLSTAUGHTLINES_DIALOG };
	CButton	m_buttonDoubleScanLines;
	CComboBox	m_comboUntwistDirection;
	CComboBox	m_comboRotateWhat;
	CEdit	m_editRotateAngle;
	CComboBox	m_comboRotateAxis;
	CSpinButtonCtrl	m_spinWaterPath;
	CEdit	m_editWaterPath;
	CEdit	m_editSetNormalTo;
	CComboBox	m_comboNormalChoice;
	CComboBox	m_comboUnTwistMode;
	CEdit	m_editDeleteFinishLine;
	CEdit	m_editDeleteStartLine;
	CComboBox	m_comboSplitMethod;
	CEdit	m_editSliceOffPos;
	CEdit	m_editExtrapolateDistance;
	CComboBox	m_comboSliceOffMode;
	CComboBox	m_comboExtrapolateMode;
	CSpinButtonCtrl	m_spinSlowIncrement;
	CEdit	m_editSlowIncrement;
	CEdit	m_editResolution;
	CButton	m_checkMathematicalNormals;
	CComboBox	m_comboPerpendicularStyle;
	CComboBox	m_comboComputeScanAxis;
	CComboBox	m_comboAlgorithm;
	CComboBox	m_comboComputeFace;
	CComboBox	m_comboSortAxis;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaughtToolsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaughtToolsDlg)
	afx_msg void OnButtonSortAllLines();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonGenerateScanlines();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnButtonComputeFace();
	afx_msg void OnSelchangeComboComputeScanAxis();
	afx_msg void OnSelchangeComboAlogorithm();
	afx_msg void OnSelchangeComboPerpendicularStyle();
	afx_msg void OnCheckMathematicalNormals();
	afx_msg void OnSelchangeComboSortAxis();
	afx_msg void OnChangeEditResolution();
	afx_msg void OnButtonExtrapolateMinEdge();
	afx_msg void OnButtonExtrapolateMaxEdge();
	afx_msg void OnButtonComputeNormals();
	afx_msg void OnChangeEditSlowIncrement();
	afx_msg void OnDeltaposSpinSlowIncrement(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboExtrapolateMode();
	afx_msg void OnSelchangeComboSliceOffMode();
	afx_msg void OnButtonSliceOffMin();
	afx_msg void OnButtonSliceOffMax();
	afx_msg void OnButtonDeleteGreaterThan();
	afx_msg void OnButtonLeftToRight();
	afx_msg void OnButtonSplit();
	afx_msg void OnButtonDelete();
	afx_msg void OnSelchangeComboUntwistMode();
	afx_msg void OnButtonUntwist();
	afx_msg void OnButtonSetAllNormals();
	afx_msg void OnButtonEnterStartLine();
	afx_msg void OnButtonEnterFinishLine();
	afx_msg void OnButtonSetWp();
	afx_msg void OnDeltaposSpinWp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDeleteNegK();
	afx_msg void OnButtonDeletePosK();
	afx_msg void OnButtonRotate();
	afx_msg void OnButtonMultiply();
	afx_msg void OnButtonSetRPos();
	afx_msg void OnButtonDelete45Degrees();
	afx_msg void OnButtonBladeEdgeClip();
	afx_msg void OnButtonKeepCopy();
	afx_msg void OnButtonReplaceCopy();
	afx_msg void OnButtonScanlinesToOutside();
	afx_msg void OnButtonScanlinesToInside();
	afx_msg void OnSelchangeComboUntwistDirection();
	afx_msg void OnButtonAddRPos();
	afx_msg void OnButtonDoubleScanLines();
	afx_msg void OnButtonRotateBlade();
	afx_msg void OnButtonDeleteNegI();
	afx_msg void OnButtonDeletePosI();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TAUGHTTOOLSDLG_H__5F21CCD6_5F20_493D_BA39_756A42F4925F__INCLUDED_)
