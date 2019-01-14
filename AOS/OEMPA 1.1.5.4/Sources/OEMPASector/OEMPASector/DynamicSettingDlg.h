#pragma once

#include "MyDialogBar.h"

// CDynamicSettingDlg dialog

class CDynamicSettingDlg : public CMyDialogBar
{
	DECLARE_DYNAMIC(CDynamicSettingDlg)

public:
	CDynamicSettingDlg(CWnd* pParent = NULL);		// Standard constructor
	virtual ~CDynamicSettingDlg();

	virtual BOOL OnInitDialogBar();

	CString m_csCycleQuantity[2];
	CString m_csPointQuantity[2];
	int m_iProbe;
	CString m_csMinimumAngle[2];
	CString m_csStepAngle[2];
	CString m_csEncoderAxisLength;
	CString m_csEncoderAxisStep;
	CString m_csPieceThickness;
	CString m_csVelocity;
	CString m_csSamplingPeriod;

	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedRadioProbe();
	afx_msg void OnBnClickedButtonApplyUpdate(CCmdUI* pCmdUI);

	enum { IDD = IDD_DIALOG_DYNAMIC_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	DECLARE_MESSAGE_MAP()
};