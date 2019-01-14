#pragma once
#include "afxwin.h"


// CDlgCalibration dialog

class CDlgCalibration : public CDialog
{
	DECLARE_DYNAMIC(CDlgCalibration)

public:
	CDlgCalibration(CHWDeviceOEMPA *pHWDeviceOEMPA,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCalibration();

// Dialog Data
	enum { IDD = IDD_DIALOG_CALIBRATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	void UpdateDialog();

	DECLARE_MESSAGE_MAP()
public:
	CButton m_ButtonReset;
	CButton m_CheckEnable;
	CHWDeviceOEMPA *m_pHWDeviceOEMPA;
	static float m_fCalibrationOffset;
	static bool m_bCalibrationOffset;
	static CString m_strStaticAlignment;
	static CString m_strStaticOffset;
	BOOL m_bCheckEnable;
	CButton m_CheckOffset;
	BOOL m_bCheckOffset;

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonCalibrationReset();
	afx_msg void OnBnClickedCheckCalibrationEnable();
	afx_msg void OnBnClickedButtonCalibrationPerform();
	afx_msg void OnBnClickedCheckCalibrationOffset();
	CButton m_ButtonUpdate;
	afx_msg void OnBnClickedButtonUpdate();
};
