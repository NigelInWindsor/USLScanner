#pragma once
#include "afxwin.h"


// CAnalysisToolsDlg dialog

class CAnalysisToolsDlg : public CDialog
{
	DECLARE_DYNAMIC(CAnalysisToolsDlg)

public:
	CAnalysisToolsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAnalysisToolsDlg();

	void UpdateLimitation();

// Dialog Data
	enum { IDD = IDD_DIALOG_ANALYSIS };

protected:

	CString m_csYPos;
	CString m_csXMin;
	CString m_csXMax;
	CString m_csYMin;
	CString m_csYMax;
	CString m_csZMin;
	CString m_csZMax;

	bool m_bProbeHasChanged;
	bool m_bModeHasChanged;
	bool m_bSlicePositionHasChanged;
	bool m_bGateSettingHasChanged;

	int m_iProbe;
	int m_iMode;

	CComboBox m_ComboCycle;
public:
	static int m_iComboCycle;
	static bool m_bCursor;
	static double m_dX,m_dZ;
protected:
	int m_iIndexLayoutFile;//integration

	afx_msg void OnDeltaposSpinYPos(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinXMin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinXMax(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinYMin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinYMax(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinZMin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinZMax(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnEnKillfocusEditYPos();
	afx_msg void OnEnKillfocusEditXMin();
	afx_msg void OnEnKillfocusEditXMax();
	afx_msg void OnEnKillfocusEditYMin();
	afx_msg void OnEnKillfocusEditYMax();
	afx_msg void OnEnKillfocusEditZMin();
	afx_msg void OnEnKillfocusEditZMax();

	afx_msg void OnBnClickedRadioProbe();
	afx_msg void OnBnClickedRadioMode();

	afx_msg void OnBnClickedButtonResetXMin();
	afx_msg void OnBnClickedButtonResetXMax();
	afx_msg void OnBnClickedButtonResetYMin();
	afx_msg void OnBnClickedButtonResetYMax();
	afx_msg void OnBnClickedButtonResetZMin();
	afx_msg void OnBnClickedButtonResetZMax();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnCbnSelchangeComboCycle();
	afx_msg void OnBnClickedCheckBackwallRebound();
	BOOL m_bCheckBackwallRebound;
	CEdit m_EditCursorX;
	CEdit m_EditCursorZ;
	CStatic m_StaticCursorX;
	CStatic m_StaticCursorZ;
	CStatic m_StaticCursorUnitX;
	CStatic m_StaticCursorUnitZ;
};
