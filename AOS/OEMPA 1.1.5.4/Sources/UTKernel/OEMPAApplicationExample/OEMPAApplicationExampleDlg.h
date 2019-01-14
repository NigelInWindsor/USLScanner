
// OEMPAApplicationExampleDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// COEMPAApplicationExampleDlg dialog
class COEMPAApplicationExampleDlg : public CDialog
{
// Construction
public:
	COEMPAApplicationExampleDlg(CWnd* pParent = NULL);	// standard constructor
	~COEMPAApplicationExampleDlg();

// Dialog Data
	enum { IDD = IDD_OEMPAAPPLICATIONEXAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void UpdateDialog();
	
	bool SetTimer(bool bEnable);
	static structCorrectionOEMPA* WINAPI CallbackCustomizedAPI(void *pAcquisitionParameter,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);
	static UINT WINAPI CallbackAcquisitionAscan(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat);
	static UINT WINAPI CallbackAcquisitionCscan(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp, const structCscanAmpTof_0x0202 *pBufferAmpTof);
	void WriteTrigger();

	CHWDeviceOEMPA m_HWDeviceOEMPA;
	CRITICAL_SECTION m_CriticSection;
	static const UINT_PTR g_kTimerID = 31;
	static const UINT g_kTimerDelay = 1000;
	DWORD m_dwTimerID;
	ULONGLONG m_ullDataSizeAscan;
	ULONGLONG m_ullDataSizeCscan;
	DWORD m_dwFrameId;
	CString m_strFilePath;
	double m_dFileGain;
	int m_iFileCycleCount;
	double m_dEncoderAscan;
	double m_dEncoderCscan;
	bool m_bEncoderAscan;
	bool m_bEncoderCscan;
	ULONGLONG m_ullDisplayDataSizeAscan;
	ULONGLONG m_ullDisplayDataSizeCscan;
	DWORD m_dwDisplayFrameId;
	DWORD m_dwDisplayTickCount;

	BOOL m_bCheckConnect;
	double m_dEditReadGain;
	int m_iEditReadCycleCount;
	double m_dEditWriteGain;
	double m_dEditThroughputCscan;
	double m_dEditThroughputAscan;
	double m_dEditThroughputFrame;
	double m_dEditFileGain;
	double m_dEditEncoderAscan;
	double m_dEditEncoderCscan;
	BOOL m_bCheckPulserAutoEnable;
	int m_iEditFileCycleCount;
	BYTE m_cIP[4];
	double m_dEditAcquisitionStepX;
	int m_iComboSynchronisation;
	DWORD m_dwEditCommunicationErrorCount;
	DWORD m_dwEditLostCountAscan;
	DWORD m_dwEditLostCountCscan;
	DWORD m_dwEditPort;

	CIPAddressCtrl m_IPAddress;
	CButton m_CheckConnect;
	CButton m_ButtonFile2Hw;
	CButton m_ButtonReadCycleCount;
	CButton m_ButtonReadGain;
	CButton m_ButtonWriteGain;
	CButton m_CheckPulserAutoEnable;
	CButton m_ButtonHw2File;
	CEdit m_EditWriteGain;
	CButton m_CheckPulserEnable;
	BOOL m_bCheckPulserEnable;
	CEdit m_EditResolutionX;
	DWORD m_dwEditResolutionX;
	CEdit m_EditAcquisitionStepX;
	CButton m_ButtonWriteResolution;
	CButton m_ButtonWriteAcquisitionStep;
	CComboBox m_ComboSynchronisation;
	CButton m_ButtonResetEncoder;
	CEdit m_EditCommunicationErrorCount;
	CString m_strEditFilePath;
	CEdit m_EditPort;
	CButton m_ButtonHwLink;
	CEdit m_EditDataLostAscan;
	CEdit m_EditDataLostCscan;
	CBrush* m_pEditBkBrush;

	afx_msg void OnBnClickedCheckConnect();
	afx_msg void OnBnClickedButtonRdGain();
	afx_msg void OnBnClickedButtonRdCycleCount();
	afx_msg void OnBnClickedButtonWrGain();
	afx_msg void OnBnClickedButtonFile2hw();
	afx_msg void OnBnClickedCheckPulserAutoEnable();
	afx_msg void OnBnClickedButtonHw2file();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckEnable();
	afx_msg void OnBnClickedButtonWrResolution();
	afx_msg void OnBnClickedButtonWrStep();
	afx_msg void OnCbnSelchangeComboSynchronisation();
	afx_msg void OnBnClickedButtonWrEncoderReset();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonHwLink();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
