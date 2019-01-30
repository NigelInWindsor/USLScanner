// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__DDE10F64_8284_47B1_BFFC_46C8D9A5342B__INCLUDED_)
#define AFX_MAINFRM_H__DDE10F64_8284_47B1_BFFC_46C8D9A5342B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"
#include "ScopeDlg.h"	// Added by ClassView
#include "AxesSheet.h"
#include "3DViewEditDlg.h"
#include "PaletteSheet.h"
#include "UltrasonicsSheet.h"
#include "ViewSheet.h"
#include "MotionToolsSheet.h"
#include "StepperSheet.h"
#include "SerialJoystickDlg.h"
#include "PmacSheet.h"
#include "PositionSheet.h"
#include "CompensationSheet.h"
#include "ScanSheet.h"
#include "PolyCoord.h"	// Added by ClassView
#include "SpectrumAnalyzerDlg.h"
#include "ThirdPartyImageDlg.h"	// Added by ClassView
#include "EditDetailsDlg.h"
#include "UsersDlg.h"
#include "AccessLevelsDlg.h"
#include "IGESDlg.h"
#include "DataBaseDlg.h"
#include "BMPViewerDlg.h"
#include "MachineDescriptionDlg.h"
#include "SafetyInterlocksDlg.h"
#include "FilterSheet.h"
#include "LSABScanDlg.h"
#include "ProbeCharacterisationSheet.h"
#include "LogonDlg.h"
#include "CustomizeSheet.h"
#include "Coord.h"	// Added by ClassView
#include "ArchiveSheet.h"
#include "ProfileSheet.h"
#include "MaintenanceDiagnosticsSheet.h"	// Added by ClassView
#include "ImportToolPathsDlg.h"
#include "MultipleBScansDlg.h"
#include "WorkSpaceSheet.h"
#include "AssignAxesDlg.h"
#include "RobotSheet.h"
#include "FaroCoordinateDlg.h"
#include "DataPropertiesDlg.h"
#include "PhasedArrayProbeDlg.h"
#include "ViewBladeThickneessDlg.h"
#include "InclusionsSheet.h"
#include "QuaternionPage.h"
#include "DialogBarPumps.h"
#include "MFCNormalizeDlg.h"
#include "CPhasedArraySheet.h"
#include "StatusPane.h"
#include "CMFCToolBarSpinButton.h"
#include "PositionPane.h"
#include "PropertiesPane.h"
#include "AxesPane.h"
#include "AxisPane.h"
#include "AxisStatusPane.h"
#include "CoordArrayPane.h"
#include "3DWorldViewPane.h"
#include "OptimizePane.h"

#define	MAX_NUMBER_VIEWS	20
#define MAX_TOOLBARS		20

class CMainFrame : public CFrameWndEx
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	LRESULT UpdateMotionToolsSheet(WPARAM wp, LPARAM lp);
	HRESULT Update3DViewDlg(WPARAM, LPARAM);
	HRESULT InitializeDialogBar(WPARAM, LPARAM);
	void DisplayOscilloscopes(int nCount);
	afx_msg HRESULT InvalidateScanTime(WPARAM, LPARAM);
	void RefreshMessageInStatusBar();
	void UpdateWorkSpace();
	void WorkSheetSingleScan();
	void WorkSheetScan();
	void ChangeMeasurementUnits();
	void Invalidate3DImages();
    void DisplayCADToolBar();
	CCoord m_CpStatus;
	bool TryToLoadProfile(CString FileName);
	int m_nEmergencyBits;
	int m_nDoorBits;
	int m_nGateBits;
	void PaletteEdit();
	void CallProtectedPaletteEdit();
	void InvalidateWaterPaths();
	bool CheckValidLicence(CString cstrLicence);
	HRESULT SetAccessPrivelages(WPARAM wp = NULL, LPARAM lp = NULL);
	bool ForceLogon();
	int m_osStartTimeOutTimer;
	void CheckLogOffTimeOut();
	CtrlType m_CtrlType;
	bool IsScanImageStillOpen();
	void FileView(CString &FilePathName);
	void RefreshSideFromRelearn();
	void ReLearnBrane();
	void ReLearnBraneOld();
	void ReLearnFieldBus(int nLineType);
	void ReLearnTaughtOld();
	void ReLearnTaught();
	void OptomiseCoordinate();
	void OnChangeEditPumpSpeed0();
	void OnChangeEditPumpSpeed1();
	void OnDeltaposSpinPumpSpeed0(NMHDR* pNMHDR, LRESULT* pResult);
	void OnDeltaposSpinPumpSpeed1(NMHDR* pNMHDR, LRESULT* pResult);
	void OnVScrollEditPumpSpeed00();
	void OnChangeEditPumpSpeed00();
	void OnChangeEditPumpSpeed01();
	void OnDeltaposSpinPumpSpeed00(NMHDR* pNMHDR, LRESULT* pResult);
	void OnDeltaposSpinPumpSpeed01(NMHDR* pNMHDR, LRESULT* pResult);
	void OnDeltaposSliderFeedRate(NMHDR* pNMHDR, LRESULT* pResult);
	void OnDeltaposSpinLeftWaterPath(NMHDR* pNMHDR, LRESULT* pResult);
	void OnDeltaposSpinRightWaterPath(NMHDR* pNMHDR, LRESULT* pResult);
	void OnDeltaposSpinThickness(NMHDR* pNMHDR, LRESULT* pResult);
	void OnDeltaposSpinProbeLength(NMHDR * pNMHDR, LRESULT * pResult);
	CThirdPartyImageDlg* m_pThirdPartyImageDlg[2];
	void ChangePalletteNumber(int nImageType,int nImageNumber);
	void StatusBarPrompt(CString Text,int nSeconds);
	void ReLearnLine();
	HRESULT OnMessageInvalidateView(WPARAM, LPARAM);
	void StopCurrentTimerAction();
	CPolyCoord m_CpNewLine;
	int m_nSubTimerAction;
	int m_nTimerAction;
	void ContinueScan(int nViewNumber);
	afx_msg HRESULT CloseAllImages(WPARAM, LPARAM);
	int StartScan(int nScanType,bool bForceNewWindow = false);
	void SetToolBarCheckedState(bool bAction);
	void OnChangeEditLeftWaterPath();
	void OnChangeEditRightWaterPath();
	void OnChangeEditThickness();
	void OnChangeEditProbeLength();
	void DockBarNextTo(CControlBar *pNewBar, CControlBar *pDockedBar, UINT nDockedBarID);
	void InitializePumpsDialogBar();
	afx_msg HRESULT UpdatePumpsDialogBar(WPARAM, LPARAM);
	void OpenOnLastExit();
	LRESULT RemoteHandler(WPARAM Wp, LPARAM Lp);
	LRESULT RemoteHandlerProfile(WPARAM Wp, LPARAM Lp);
	LRESULT RemoteHandlerUltrasonics(WPARAM Wp, LPARAM Lp);
	LRESULT RemoteHandlerFLSC10(WPARAM Wp, LPARAM Lp);
	LRESULT RemoteHandlerEgismosRF(WPARAM Wp, LPARAM Lp);
	LRESULT RemotePageUpdateControls(WPARAM Wp, LPARAM Lp);
	LRESULT RemoteRMessageSend(WPARAM Wp, LPARAM Lp);
	LRESULT RemoteRMessageReceive(WPARAM Wp, LPARAM Lp);
	LRESULT RemoteStartServer(WPARAM Wp, LPARAM Lp);
	void HTOffOn(int nHtOffOn);
	void CreateToolBars();
	bool CreateDockingWindows();
	void DockPaneConfineToScreen(CDockablePane * pPane);
	void OnApplicationLook(UINT id);
	void OnUpdateApplicationLook(CCmdUI * pCmdUI);
	bool IsFatalFollowingError();
	bool IsZAmpliferDisabled();

	int	m_nWSSingleCurrentView;
	int	m_nWSCurrentView;
	int	m_nOldMeasurementUnits;
	int	m_nLine;
	int	m_nIndex;
	int	m_nPlayListIndex;
	int	m_nProfileIndex;
	float m_fOldFeedRate;
	bool m_bPauseWorksheetScan;
	CString m_LoggedOnOperator;
	CCoord m_CpResult[2];
	CCoord m_CpStart;

	CMFCNormalizeDlg m_NormalizeDlg;
	CRobotSheet* m_pRobotSheet;
	CMaintenanceDiagnosticsSheet* m_pMaintenanceDiagnosticsSheet;
	CArchiveSheet* m_pArchiveSheet;
	CLogonDlg m_LogonDlg;
	CDetailsPage* m_pDetailsPage;
	CFilterSheet* m_pFilterSheet;
	CScanSheet* m_pScanSheet;
	CViewSheet* m_pScanView;
	CViewSheet* m_pViewSheet[MAX_NUMBER_VIEWS];
	CViewBladeThickneessDlg* m_pViewBladeThicknessDlg[MAX_NUMBER_VIEWS];
	CViewBladeThickneessDlg* m_pScanViewBladeThicknessDlg;
	CMotionToolsSheet* m_pMotionToolsSheet;
	CPaletteSheet* m_pPaletteSheet;
	C3DViewEditDlg* m_p3DViewEditDlg;
	CAxesSheet *m_pAxesSheet;
	CUltrasonicsSheet* m_pUltrasonicsSheet;
	CStepperSheet* m_pStepperSheet;
	CScopeDlg *m_pScopeDlg[8];
	CSerialJoystickDlg* m_pSerialJoystickDlg;
	CPmacSheet* m_pPmacSheet;
	CPositionSheet* m_pPositionSheet;
	CCompensationSheet* m_pCompensationSheet;
	CSpectrumAnalyzerDlg* m_pSpectrumAnalyzerDlg;
	CEditDetailsDlg* m_pEditDetailsDlg;
	CUsersDlg* m_pUsersDlg;
	CAccessLevelsDlg* m_pAccessLevelsDlg;
	CIGESDlg* m_pIGESDlg;
	CDataBaseDlg* m_pDataBaseDlg;
	CBMPViewerDlg* m_pBMPViewerDlg;
	CSafetyInterlocksDlg* m_pSafetyInterlocksDlg;
	CLSABScanDlg* m_pLSABScanDlg;
	CProbeCharacterisationSheet* m_pProbeCharacterisationSheet;
	CCustomizeSheet* m_pCustomizeSheet;
	CProfileSheet* m_pProfileSheet;
	CImportToolPathsDlg* m_pImportToolPathsDlg;
	CMultipleBScansDlg* m_pMultipleBScansDlg;
	CWorkSpaceSheet* m_pWorkSpaceSheet;
	CAssignAxesDlg* m_pAssignAxesDlg;
	CFaroCoordinateDlg*	m_pFaroCoordinateDlg;
	CDataPropertiesDlg* m_pDataPropertiesDlg;
	CPhasedArrayProbeDlg* m_pPhasedArrayProbeDlg;
	CPolyCoordDlg* m_pPolyCoordDlg;
	CInclusionsSheet* m_pInclusionsSheet;
	CQuaternionPage* m_pQuaternionsPage;
	CPhasedArraySheet* m_pPhasedArraySheet;

	CStatusPane		m_wndStatusPane;
	CPositionPane	m_wndPositionPane;
	CPropertiesPane	m_wndPropertiesPane;
	CAxesPane		m_wndAxesPane;
	CAxisPane		m_wndAxisPane;
	CAxisStatusPane	m_wndAxisStatusPane;
	CCoordArrayPane m_wndCoordArrayPane;
	C3DWorldViewPane m_wnd3DWorldViewPane;
	COptimizePane	m_wndOptimizePane;

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	void CheckHT(bool bEStop = false);
	void RefreshAxesSheet();
	void InitializeProfileSheetPtrs();
	afx_msg HRESULT UpdatePhasedArrayDlg(WPARAM, LPARAM);
	void StartBladeScan(int nScanType, int nScanSide, int nAction);
	afx_msg HRESULT UpdatePmacSheet(WPARAM, LPARAM);
	void RedrawUltrasonicsSheet();
	void Customize();
	void OpenChildView(int nImageType,int nImageNumber,CData *pData,int nTS,int nGate);
	int m_nRThetaZeroSide;
	void ZeroTheta();
	afx_msg HRESULT Invalidate3DViewPage(WPARAM wp, LPARAM lp);
	afx_msg HRESULT UpdateUltrasonicsLSAPage(WPARAM, LPARAM);
	afx_msg HRESULT UpdateGatesPage(WPARAM, LPARAM);
	int lfHeightFromPointSize(int nPointSize);
	int PointSizeFromHeight(int nHeight);
	CMFCMenuBar       m_wndMenuBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBar	m_ToolBarAirWater0;
	CMFCToolBar	m_ToolBarAirWater1;
	CMFCToolBar	m_ToolBarDualWaterPath;
	CMFCToolBar	m_ToolBarSingleWaterPath;
	CMFCToolBar m_ToolBarEstopJoySafe;
	CMFCToolBar	m_ToolBarHt;
	CMFCToolBar m_ToolBarLogOnOff;
	CMFCToolBar m_ToolBarTips;
	CMFCToolBar m_ToolBarDualMotion;
	CMFCToolBar m_ToolBarDangerPlanes;
	CMFCToolBar m_ToolBarWaterPathProbeLengthTips;

	CMFCToolBar    *m_pwndToolBar[MAX_TOOLBARS];
	CMFCToolBar	m_wndCADToolBar;
	CMFCToolBar	m_wndCtrlToolBar;
	CMFCToolBar	*m_pToolBarTipAxes;
	CMFCToolBar	*m_pToolBarMotion;
	CMFCToolBar	*m_pToolBarHt;
	CMFCToolBar	*m_pToolBarPowered;
	CMFCToolBar	*m_pToolBarWaterPath;
	CMFCToolBar	*m_pToolBarJoystick;
	CMFCToolBar	*m_pToolBarUltrasonics;
	CMFCToolBar	*m_pToolBarBlade;
	CMFCToolBar	*m_pToolBarTurntableRoller;
	CMFCToolBar	*m_pToolBarMarkerPen;
	CMFCToolBar	*m_pToolBarPhasedArray;
	CMFCToolBar	*m_pToolBarXRSwap;
	CChildView  m_wndView;
	CReBar		m_wndReBar;
	CPaneDialog	*m_pwndDialogBar[MAX_TOOLBARS];
	CDialogBarPumps*	m_pDialogBarPumps;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,   LRESULT *pResult);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnUltrasonicsOscilloscope();
	afx_msg	HRESULT LanguageChanged(WPARAM, LPARAM);
	void OnSystemCards();
	afx_msg void OnSystemOptions();
	afx_msg void OnSystemAxes();
	afx_msg void OnProfilesImport();
	afx_msg void OnProfilesEdit();
	afx_msg void OnUpdateProfilesEdit(CCmdUI* pCmdUI);
	afx_msg void OnProfilesOpen();
	afx_msg void OnProfilesSaveas();
	afx_msg void OnSystemAssignaxes();
	afx_msg void OnPaletteOpen();
	afx_msg void OnPaletteSaveas();
	afx_msg void OnPaletteEdit();
	afx_msg void OnUpdatePaletteEdit(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnMotionTools();
	afx_msg HRESULT RefreshMotionTools(WPARAM, LPARAM);
	afx_msg HRESULT RefreshUltrasonicsSheet(WPARAM, LPARAM);
	afx_msg void OnSystemHoldingfixture();
	afx_msg void OnSystemPmac();
	afx_msg void OnClose();
	afx_msg void OnButtonAbort();
	afx_msg void OnButtonWaterPath();
	afx_msg void OnButtonJoystick();
	afx_msg void OnSystemKinematics();
	afx_msg void OnButtonSafe();
	afx_msg void OnUltrasonicsTileoscilloscopesvertically();
	afx_msg void OnUltrasonicsTileoscilloscopeshorizontally();
	afx_msg void OnUltrasonicsOscilloscopepositionsShiftallleft();
	afx_msg void OnUltrasonicsOscilloscopepositionsShiftallright();
	afx_msg void OnButtonLeftPump();
	afx_msg void OnButtonRightPump();
	afx_msg void OnMainfrmMoverighttofindleft();
	afx_msg void OnMainfrmMovelefttofindright();
	afx_msg void OnScanScan();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUltrasonicsOpen();
	afx_msg void OnUltrasonicsSaveas();
	afx_msg void OnUltrasonicsSpectrumanalyzer();
	afx_msg HRESULT ProfileRetrievedUpdateDialogs(WPARAM wp, LPARAM lp);
	afx_msg void OnSystemImportthirdpartyimage();
	afx_msg void OnButtonUsefullHt();
	afx_msg void OnUpdateButtonUsefullHt(CCmdUI* pCmdUI);
	afx_msg void OnUpdateHTOffOn(CCmdUI * pCmdUI);
	afx_msg void OnButtonLeftAir();
	afx_msg void OnButtonRightAir();
	afx_msg void OnSystemEditscandetails();
	afx_msg void OnSystemUsers();
	afx_msg void OnFileLogon();
	afx_msg void OnSystemAccesslevels();
	afx_msg void OnUpdateSystemAccesslevels(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSystemAssignaxes(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSystemAxes(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSystemCards(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSystemDeviceresources(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSystemEditscandetails(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSystemHoldingfixture(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSystemImportthirdpartyimage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSystemKinematics(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSystemOptions(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSystemOrientationlanguage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSystemPmac(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSystemUsers(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProfilesImport(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProfilesOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProfilesSaveas(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUltrasonicsOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUltrasonicsOscilloscope(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUltrasonicsOscilloscopepositionsShiftallleft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUltrasonicsOscilloscopepositionsShiftallright(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUltrasonicsSaveas(CCmdUI* pCmdUI);
	afx_msg void OnProfilesIgesfileimport();
	afx_msg void OnFileDatabase();
	afx_msg void OnViewBmpviewer();
	afx_msg void OnMotionBisect();
	afx_msg void OnSystemSafetyinterlocks();
	afx_msg void OnUpdateSystemSafetyinterlocks(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUltrasonicsSpectrumanalyzer(CCmdUI* pCmdUI);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnUltrasonicsFilters();
	afx_msg void OnUltrasonicsLsabscan();
	afx_msg void OnUltrasonicsPulserreceiver();
	afx_msg void OnButtonCtrlPoint();
	afx_msg void OnButtonCtrlText();
	afx_msg void OnButtonCtrlEdit();
	afx_msg void OnUpdateButtonCtrlPoint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonCtrlText(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonCtrlEdit(CCmdUI* pCmdUI);
	afx_msg void OnButtonCtrlButton();
	afx_msg void OnUpdateButtonCtrlButton(CCmdUI* pCmdUI);
	afx_msg void OnButtonLeftAlign();
	afx_msg void OnUpdateButtonLeftAlign(CCmdUI* pCmdUI);
	afx_msg void OnButtonRightAlign();
	afx_msg void OnUpdateButtonRightAlign(CCmdUI* pCmdUI);
	afx_msg void OnButtonSameWidth();
	afx_msg void OnUpdateButtonSameWidth(CCmdUI* pCmdUI);
	afx_msg void OnButtonSameHeight();
	afx_msg void OnUpdateButtonSameHeight(CCmdUI* pCmdUI);
	afx_msg void OnButtonSpaceVertically();
	afx_msg void OnUpdateButtonSpaceVertically(CCmdUI* pCmdUI);
	afx_msg void OnButtonTopAlign();
	afx_msg void OnUpdateButtonTopAlign(CCmdUI* pCmdUI);
	afx_msg void OnUltrasonicsProbecharactersation();
	afx_msg void OnButtonPower();
	afx_msg void OnButtonAuto();
	afx_msg void OnUpdateSystemMachinedescription(CCmdUI* pCmdUI);
	afx_msg void OnSystemCustomize();
	afx_msg void OnUpdateSystemCustomize(CCmdUI* pCmdUI);
	afx_msg void OnMotionKinematics();
	afx_msg void OnButtonOrigin();
	afx_msg void OnButtonMovetoHome();
	afx_msg void OnSystemArchive();
	afx_msg void OnUpdateButtonJoystick(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonSafe(CCmdUI* pCmdUI);
	afx_msg void OnFileLogoff();
	afx_msg void OnUpdateSystemArchive(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePaletteOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePaletteSaveas(CCmdUI* pCmdUI);
	afx_msg void OnSystemMaintenancediagnostics();
	afx_msg void OnButtonMouse();
	afx_msg void OnButtonLine();
	afx_msg void OnButtonEllipse();
	afx_msg void OnButtonMultiLine();
	afx_msg void OnUpdateButtonMouse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonEllipse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonMultiLine(CCmdUI* pCmdUI);
	afx_msg void OnButtonMove();
	afx_msg void OnUpdateButtonMove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMotionKinematics(CCmdUI* pCmdUI);
	afx_msg void OnProfilesImportdesigner();
	afx_msg void OnFileOpenBScan();
	afx_msg void OnUpdateFileOpenBScan(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUltrasonicsProbecharactersation(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUltrasonicsLsabscan(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUltrasonicsFilters(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProfilesImportdesigner(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProfilesIgesfileimport(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMotionBisect(CCmdUI* pCmdUI);
	afx_msg HRESULT UpdateAxes(WPARAM, LPARAM);
	afx_msg void OnUltrasonicsEnableethernet();
	afx_msg void OnViewWorkspace();
	afx_msg void OnButtonTipsPointDown();
	afx_msg void OnButtonTipsRemoveAir();
	afx_msg void OnButtonTipsTo45();
	afx_msg void OnButtonTipsToHorizontal();
	void OnButtonSwivalToZero();
	afx_msg void OnButtonInsideJoystick();
	afx_msg void OnButtonOutsideJoystick();
	afx_msg void OnUpdateButtonInsideJoystick(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonOutsideJoystick(CCmdUI* pCmdUI);
	afx_msg void OnButtonPulseEcho();
	afx_msg void OnButtonThroughTransmission();
	afx_msg void OnHelpUslscanneroperatormanual();
	afx_msg void OnHelpPmacsoftwaremanual();
	afx_msg void OnSystemRobots();
	afx_msg HRESULT OnViewLasertracker(WPARAM, LPARAM);
	afx_msg	HRESULT TrackerCoordinateAdded(WPARAM, LPARAM);
	void OnButtonPr300();
	afx_msg void OnButtonPr301();
	afx_msg void OnUpdateButtonPr300(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonPr301(CCmdUI* pCmdUI);
	afx_msg void OnFileUpdateLicense();
	afx_msg void OnButtonBladeZeroDegrees();
	afx_msg void OnUpdateButtonBladeZeroDegrees(CCmdUI* pCmdUI);
	afx_msg void OnButtonBlade180Degrees();
	afx_msg void OnUpdateButtonBlade180Degrees(CCmdUI* pCmdUI);
	afx_msg void OnButtonBladeHome();
	afx_msg void OnButtonTurntableEnable();
	afx_msg void OnUpdateButtonTurntableEnable(CCmdUI* pCmdUI);
	afx_msg void OnButtonRollersEnable();
	afx_msg void OnUpdateButtonRollersEnable(CCmdUI* pCmdUI);
	afx_msg HRESULT UpdateRobotSheet(WPARAM, LPARAM);
	afx_msg void OnSystemEditProperties();
	afx_msg void OnSystemEditDetails();
	afx_msg void OnButtonMarkerPen();
	afx_msg void OnUltrasonicsPhasedarrayprobe();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonPaEqualWaterPath();
	afx_msg HRESULT UpdatePaletteSheet(WPARAM, LPARAM);
	afx_msg void OnViewBlade();
	afx_msg void OnButtonWaterSpray();
	afx_msg void OnButtonPark();
	afx_msg void OnButtonStepOffOn();
	afx_msg void OnButtonPreWetRoller();
	afx_msg void OnButtonEnablePowerToRobots();
	afx_msg void OnViewLineexpansion();
	afx_msg void OnUpdateButtonXyzMode(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonYzrMode(CCmdUI* pCmdUI);
	afx_msg void OnUltrasonicsInclusions();
	afx_msg void OnUpdateButtonMarkerPen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonRightAir(CCmdUI* pCmdUI);
	afx_msg void OnSystemQuaternions();
	afx_msg void OnUpdateButtonLeftAir(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonLeftPump(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonRightPump(CCmdUI* pCmdUI);
	afx_msg void OnButtonMovetoSurfaceZero();
	afx_msg void OnButtonMovetoMachineZero();
	afx_msg void OnUltrasonicsOscilloscope1();
	afx_msg void OnUltrasonicsOscilloscope2();
	afx_msg void OnUltrasonicsOscilloscope3();
	afx_msg void OnUltrasonicsOscilloscope4();
	afx_msg void OnUltrasonicsOscilloscope5();
	afx_msg void OnUltrasonicsOscilloscope6();
	afx_msg void OnUltrasonicsOscilloscope7();
	afx_msg void OnUltrasonicsOscilloscope8();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonDangerplane0();
	afx_msg void OnButtonDangerplane1();
	afx_msg void OnUpdateButtonDangerplane0(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonDangerplane1(CCmdUI* pCmdUI);
	afx_msg LRESULT OnToolbarReset(WPARAM wp, LPARAM);
	LRESULT OnViewCustomize(WPARAM wp, LPARAM);
	afx_msg void OnUpdateEditPumpSpeed00(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSpinPumpSpeed00(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPumpSpeed01(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSpinPumpSpeed01(CCmdUI *pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL m_bToolBarsInitialized = FALSE;
public:

	afx_msg void OnUpdateEditLeftWaterPath(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRightWaterPath(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditThickness(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditProbeLength(CCmdUI * pCmdUI);
	afx_msg void OnEditPumpSpeed00();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg HRESULT OnInvalidateToolBars(WPARAM wp, LPARAM lp);
	afx_msg void OnViewStatus();
	afx_msg void OnUpdateViewStatus(CCmdUI *pCmdUI);

private:
	void ShowDockablePane(CDockablePane * pPane, bool bShowState);
	//Toolbar buttons
	void InitToolbarButtons();
	CMFCToolBarEditBoxButton* m_pEditBoxButtonLeft;
	CMFCToolBarEditBoxButton* m_pEditBoxButtonRight;
	CMFCToolBarSpinButton* m_pSpinLeftPump;
	CMFCToolBarSpinButton* m_pSpinRightPump;
	CMFCToolBarEditBoxButton* m_pEditLeftWp;
	CMFCToolBarEditBoxButton* m_pEditRightWp;
	CMFCToolBarEditBoxButton* m_pEditThickness;
	CMFCToolBarSpinButton* m_pSpinLeftWp;
	CMFCToolBarSpinButton* m_pSpinRightWp;
	CMFCToolBarSpinButton* m_pSpinThickness;
	CMFCToolBarEditBoxButton* m_pEditWp0;
	CMFCToolBarSpinButton* m_pSpinWp0;
	CMFCToolBarEditBoxButton* m_pEditProbeLength;
	CMFCToolBarSpinButton* m_pSpinProbeLength;

public:
	afx_msg void OnPositionDockingpane();
	afx_msg void OnUpdatePositionDockingpane(CCmdUI *pCmdUI);
	afx_msg void OnPositionDialoge();
	afx_msg void OnUpdatePositionDialoge(CCmdUI *pCmdUI);
	afx_msg void OnSystemProperties();
	afx_msg void OnUpdateSystemProperties(CCmdUI *pCmdUI);
	afx_msg void OnDockablesAxesstatus();
	afx_msg void OnUpdateDockablesAxesstatus(CCmdUI *pCmdUI);
	afx_msg void OnDockablesAxes();
	afx_msg void OnUpdateDockablesAxes(CCmdUI *pCmdUI);
	afx_msg void OnDockablesCoordinates();
	afx_msg void OnUpdateDockablesCoordinates(CCmdUI *pCmdUI);
	afx_msg HRESULT InvalidateCoordArrayPane(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnShowPropertiesPane(WPARAM wp, LPARAM lp);
	afx_msg void OnDockables3dworldview();
	afx_msg void OnUpdateDockables3dworldview(CCmdUI *pCmdUI);
	afx_msg void OnWindowResetwindowlayout();
	afx_msg HRESULT InvalidateOptimizePane(WPARAM wp, LPARAM lp);
	afx_msg void OnDockablesOptimize();
	afx_msg void OnUpdateDockablesOptimize(CCmdUI *pCmdUI);
	afx_msg void OnDockablesAxis();
	afx_msg void OnUpdateDockablesAxis(CCmdUI *pCmdUI);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg HRESULT InvalidateViewSheets(WPARAM wParam, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__DDE10F64_8284_47B1_BFFC_46C8D9A5342B__INCLUDED_)
