#pragma once
#include "afxwin.h"


// CDlgMaximumDetection dialog

class CDlgMaximumDetection : public CDialog
{
	DECLARE_DYNAMIC(CDlgMaximumDetection)

public:
	CDlgMaximumDetection(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMaximumDetection();

	static void Display();
	static void UpdateDialog();
	static UINT WINAPI ProcessAcquisitionAscan_0x00010103(int iAscanCycle,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat);
	static structCorrectionOEMPA* WINAPI CallbackCustomizedAPI(CHWDeviceOEMPA *pOEMPA,const wchar_t *pFileName,enumStepCustomizedAPI eStep,structRoot *pRoot,structCycle *pCycle,CFocalLaw *pEmission,CFocalLaw *pReception);
	static void OnTimer();

// Dialog Data
	enum { IDD = IDD_DIALOG_MAXIMUM };

protected:
	static CDlgMaximumDetection *m_pDlgHW;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
public:
	static CRITICAL_SECTION m_CriticSection;
	static BOOL m_bCheckEnable;
	static double m_dEditAxisStart;
	static double m_dEditAxisStop;
	static double m_dEditAxisOffset;
	static double m_dEditGateStart;
	static double m_dEditGateStop;
	static bool m_bNewSetup;
	static bool m_bUpdate;
	static double m_dAxisStart[g_iOEMPACycleCountMax];
	static double m_dAxisRange[g_iOEMPACycleCountMax];
	static int m_iCycleCount;
	static int m_iEditCycle;
	static int m_iStart;
	static int m_iRange;
	static double m_dDigitizingPeriod;
	static double m_dMaxTof;
	static double m_dMaxAmp;

	CEdit m_EditAxisStart;
	CEdit m_EditAxisOffset;
	CEdit m_EditAxisStop;
	CEdit m_EditGateStart;
	CEdit m_EditGateStop;
	CEdit m_EditGateResult;
	CEdit m_EditCycle;

	afx_msg void OnBnClickedCheckEnable();
	afx_msg void OnBnClickedCancel();
};
