#include "afxwin.h"
#if !defined(AFX_OPTIONSDLG_H__946C0596_4066_42DD_89C6_AAD4D35490B0__INCLUDED_)
#define AFX_OPTIONSDLG_H__946C0596_4066_42DD_89C6_AAD4D35490B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg dialog

class COptionsDlg : public CDialog
{
// Construction
public:
	void SetRegistry();
	APP App;
	COptionsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionsDlg)
	enum { IDD = IDD_OPTIONS_DIALOG };
	CButton	m_checkHTOffEStop;
	CButton	m_checkLoadRobotWeightings;
	CEdit	m_edit3PointName2;
	CEdit	m_edit3PointName1;
	CEdit	m_edit3PointName0;
	CButton	m_checkHTOffMachineZero;
	CButton	m_checkHTOffPumps;
	CButton	m_checkAuto1to1CScans;
	CEdit	m_editMaxPulseVoltage;
	CSpinButtonCtrl	m_spinMaxPulseVoltage;
	CEdit	m_editRollerStepSize;
	CButton	m_checkLoadUtFileAtStartOfScan;
	CButton	m_checkSaveIndividual;
	CEdit	m_editRollerStepsPerRev;
	CSpinButtonCtrl	m_spinRollerDiameter;
	CEdit	m_editStepSizePrecision;
	CComboBox	m_comboVolumeMonitor;
	CButton	m_checkRollerProbeBackoff;
	CButton	m_checkLockWP;
	CButton	m_checkLockUT;
	CButton	m_checkScanStartViaDangerPlane;
	CButton	m_checkLockFile;
	CButton	m_checkPropertyPageComplete;
	CButton	m_checkForcePropertyPage;
	CEdit	m_editWayPointChangeIJK;
	CEdit	m_editWayPointChangeXYZ;
	CComboBox	m_comboWaypointsMode;
	CComboBox	m_combo1To1Monitor;
	CComboBox	m_combo1To1Position;
	CButton	m_checkAutoEnableJoystick;
	CButton	m_checkProfileNewTaught;
	CEdit	m_editNumberLines;
	CEdit	m_editSlowIncrement;
	CEdit	m_editFastIncrement;
	CEdit	m_editDangerPlaneClearance;
	CIPAddressCtrl	m_ipaddressLaser;
	CComboBox	m_comboLaser;
	CButton	m_buttonRedBlueReverse;
	CButton	m_checkDPRAM;
	CButton	m_buttonSaveScanEveryLine;
	CButton	m_checkLoadOriginFromProfile;
	CEdit	m_editMonitorWidth;
	CButton	m_checkGotoViaDangerPlane;
	CButton	m_checkLoadWaterPaths;
	CComboBox	m_comboAreYouSure;
	CButton	m_checkLoadUtFormProfile;
	CButton	m_checkImportPalettesFromSam;
	CButton	m_checkTimeSlotName;
	CButton	m_checkEnterFileName;
	CComboBox	m_comboGraticuleType;
	CEdit	m_editRollerDiameter;
	CComboBox	m_comboTextSeparator;
	CEdit	m_editMemoryAvailable;
	CComboBox	m_comboForceLogon;
	CComboBox	m_comboJoystickSource;
	CComboBox	m_comboRememberDlgs;
	CComboBox	m_comboMotorController;
	CSpinButtonCtrl	m_spinMaxScanSpeed;
	CEdit	m_editMaxScanSpeed;
	CComboBox	m_comboImageStartPos;
	CSpinButtonCtrl	m_spinMaterialVelocity;
	CSpinButtonCtrl	m_spinWaterVelocity;
	CEdit	m_editWaterVelocity;
	CEdit	m_editMaterialVelocity;
	CComboBox	m_comboCustomer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChangeEditMaterialVelocity();
	afx_msg void OnChangeEditWaterVelocity();
	afx_msg void OnSelchangeComboImageStartPos();
	afx_msg void OnChangeEditMaxScanSpeed();
	afx_msg void OnSelchangeComboMotorController();
	afx_msg void OnSelchangeComboRememberDialogs();
	afx_msg void OnSelchangeComboJoystickSource();
	afx_msg void OnEditchangeComboForceLogon();
	afx_msg void OnSelchangeComboForceLogon();
	afx_msg void OnChangeEditMemoryAvailable();
	afx_msg void OnSelchangeComboTextEparator();
	afx_msg void OnChangeEditRollerStepsize();
	afx_msg void OnChangeEditRollerDiameter();
	virtual void OnOK();
	afx_msg void OnSelchangeComboGraticuleType();
	afx_msg void OnCheckEnterFilename();
	afx_msg void OnCheckTimeslotname();
	afx_msg void OnCheckDPRam();
	afx_msg void OnCheckAutoEnableJoystick();
	afx_msg void OnCheckPropertyPageComplete();
	afx_msg void OnCheckForceProperty();
	afx_msg void OnCheckLockFiles();
	afx_msg void OnCheckScanStartViaDangerplane();
	afx_msg void OnCheckScanLockUt();
	afx_msg void OnCheckScanLockWp();
	afx_msg void OnCheckRollerProbeBackoff();
	afx_msg void OnSelchangeComboVolumeMonitor();
	afx_msg void OnChangeEditRollerStepsperrev();
	afx_msg void OnDeltaposSpinRollerDiameter(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckSaveIndividual();
	afx_msg void OnCheckLoadUtFile();
	afx_msg void OnCheckAuto1to1();
	afx_msg void OnChangeEditMaxPulseVoltage();
	afx_msg void OnCheckRobotWeightings();
	afx_msg void OnCheckHtOffOnEstop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editNumberWayPoints;
	CEdit m_editDeGlitchThreshold;
	CButton m_checkScanDeGlitch;
	afx_msg void OnBnClickedCheckDeGlitch();
	afx_msg void OnEnChangeEditDeGlitchThreshold();
	CComboBox m_comboDangerPlaneDir;
	afx_msg void OnCbnSelchangeComboDangerPlaneDir();
	CButton m_checkScanDPCFill;
	afx_msg void OnBnClickedCheckDpc();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSDLG_H__946C0596_4066_42DD_89C6_AAD4D35490B0__INCLUDED_)
