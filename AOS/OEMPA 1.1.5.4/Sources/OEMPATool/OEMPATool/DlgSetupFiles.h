#pragma once
#include "afxwin.h"


// CDlgSetupFiles dialog
class CRunToolbox;

class CDlgSetupFiles : public CDialog
{
	DECLARE_DYNAMIC(CDlgSetupFiles)

public:
	CDlgSetupFiles(int iDeviceId,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSetupFiles();
	static void Display(int iDeviceId);

// Dialog Data
	enum { IDD = IDD_DIALOG_SETUPFILES };

	static bool WizardNew();
	static bool WizardDelete();
	static void UpdateDialog();
	static void SetConnect(bool bConnect);
protected:
	static CDlgSetupFiles *m_pDlgWizard;
	static DWORD WINAPI CallbackThreadToolbox(HWND hWnd,CRunToolbox *pRunToolbox);

	void UpdateDialog_();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

	bool WizardUpdateScan();
	bool WizardLoad(CString &strFileName);
	bool WizardSave(CString strFile);
	bool View(bool bEdit,CString strFileName);
	bool RemoveFile(CString strFileName,bool bDisplayMsg);
	bool WizardFileSamples(bool bLinear,bool bRequest=false);
	bool OEMPAFileSamples(bool bWizard);
	bool IsKernelObject(wchar_t *wcObjectName,enumRootType eInRootID,int &iKernelObjectId);

	void WizardWriteEnum(CStdioFile &file,wchar_t *pKeyName,int iIndex,wchar_t *pEnumList,int iEnum);
	void WizardWriteEnumList(CStdioFile &file,wchar_t *pEnumList);
	bool WizardReadFile(
									CString &strFileName,
									bool bLinear,
									double &dSpecimenVelocity,
									int iWaveSize,
									wchar_t *wcWave,
									bool &bLongitudinalWave,
									bool &bWedgeEnable,
									double &dSpecimenRadius,
									double &dWedgeVelocity,
									double &dWedgeHeight,
									double &dWedgeAngle,
									int &iProbeElementCount,
									double &dProbePitch,
									double &dProbeFrequency,
									double &dProbeRadius,
									double &dScanDepthEmission,
									double *adScanDepthReception,
									int &iScanDepthReception,
									int &iScanElementStart,
									int &iScanElementStop,
									int &iScanElementStep,
									int &iScanElementCount,
									double &dScanAngleStart,
									double &dScanAngleStop,
									double &dScanAngleStep,
									double &dAscanStart,
									double &dAscanRange,
									double &dTimeSlot,
									int &iCscanCount,
									double *adCscanStart,
									double *adCscanRange,
									double *adCscanThreshold,
									enumRectification *aeRectification,
									enumGateModeAmp *aeModeAmp,
									enumGateModeTof *aeModeTof,
									enumDepthMode &eDepthMode
									);
	bool WizardWriteFile(
									CString &strFileName,
									bool bLinear,
									double &dSpecimenVelocity,
									int iWaveSize,
									wchar_t *wcWave,
									bool &bLongitudinalWave,
									bool &bWedgeEnable,
									double &dSpecimenRadius,
									double &dWedgeVelocity,
									double &dWedgeHeight,
									double &dWedgeAngle,
									int &iProbeElementCount,
									double &dProbePitch,
									double &dProbeFrequency,
									double &dProbeRadius,
									double &dScanDepthEmission,
									double *adScanDepthReception,
									int &iScanDepthReception,
									int &iScanElementStart,
									int &iScanElementStop,
									int &iScanElementStep,
									int &iScanElementCount,
									double &dScanAngleStart,
									double &dScanAngleStop,
									double &dScanAngleStep,
									double &dAscanStart,
									double &dAscanRange,
									double &dTimeSlot,
									int &iCscanCount,
									double *adCscanStart,
									double *adCscanRange,
									double *adCscanThreshold,
									enumRectification *aeRectification,
									enumGateModeAmp *aeModeAmp,
									enumGateModeTof *aeModeTof,
									enumDepthMode &eDepthMode
									);
	bool WizardCompare(
									CString &strFileName,
									bool bLinear,
									double &dSpecimenVelocity,
									int iWaveSize,
									wchar_t *wcWave,
									bool &bLongitudinalWave,
									bool &bWedgeEnable,
									double &dSpecimenRadius,
									double &dWedgeVelocity,
									double &dWedgeHeight,
									double &dWedgeAngle,
									int &iProbeElementCount,
									double &dProbePitch,
									double &dProbeFrequency,
									double &dProbeRadius,
									double &dScanDepthEmission,
									double *adScanDepthReception,
									int &iScanDepthReception,
									int iScanElementStart,
									int iScanElementStop,
									int &iScanElementStep,
									int &iScanElementCount,
									double &dScanAngleStart,
									double &dScanAngleStop,
									double &dScanAngleStep,
									double &dAscanStart,
									double &dAscanRange,
									double &dTimeSlot,
									int &iCscanCount,
									double *adCscanStart,
									double *adCscanRange,
									double *adCscanThreshold,
									enumRectification *aeRectification,
									enumGateModeAmp *aeModeAmp,
									enumGateModeTof *aeModeTof,
									enumDepthMode &eDepthMode
									);
	bool dEqual(double d1,double d2,double dPrecision);

	DECLARE_MESSAGE_MAP()
public:
	static int m_iDeviceId;
	static CUTWizardSystem *m_pWizardSystem;
	static CUTChannels *m_pChannel;
	static BOOL m_bCheckWizardEnable;
	static int m_iIndexLayoutFile;
	static bool m_bConnect;
	CButton m_ButtonSystemLoad;
	CButton m_ButtonSystemSave;
	CButton m_ButtonNotepad;
	CButton m_ButtonToolbox;
	CButton m_ButtonLinear;
	CButton m_ButtonSectorial;
	CButton m_ButtonWizardUpdate;
	CButton m_CheckWizardEnable;
	CButton m_ButtonWizardToFile;
	CButton m_ButtonFileSample;
	CButton m_ButtonTxtToBin;
	CButton m_ButtonBinToTxt;
	afx_msg LRESULT OnEventMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedButtonSystemLoad();
	afx_msg void OnBnClickedButtonSystemSave();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonWizardUpdate();
	afx_msg void OnBnClickedButtonNotepad();
	afx_msg void OnBnClickedCheckWizardEnable();
	afx_msg void OnBnClickedButtonToolbox();
	afx_msg void OnBnClickedButtonScanLinear();
	afx_msg void OnBnClickedButtonScanSectorial();
	afx_msg void OnBnClickedButtonWizardToFile();
	afx_msg void OnBnClickedButtonFileSample();
	afx_msg void OnBnClickedButtonTxtToBin();
	afx_msg void OnBnClickedButtonBinToTxt();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedButtonFileOption();
};
