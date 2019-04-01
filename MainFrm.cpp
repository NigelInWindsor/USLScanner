// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "USLScanner.h"
#include "CardsDlg.h"
#include "OptionsDlg.h"
#include "ImportRulesDlg.h"
#include "FileThumbNailDlg.h"
#include "LogonDlg.h"
#include "BisectMoveDlg.h"
#include "EvaluationDlg.h"
#include "CMFCToolBarSpinButton.h"
#include "ToolBarLabel.h"
#include "USLDockablePane.h"


#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
/*
#define ID_VIEW_APPLICATIONLOOK         34037
#define ID_APPLICATIONLOOK_WINDOWS2000  34038
#define ID_APPLICATIONLOOK_OFFICEXP     34039
#define ID_APPLICATIONLOOK_WINDOWSXP    34040
#define ID_APPLICATIONLOOK_OFFICE2003   34041
#define ID_APPLICATIONLOOK_VISUALSTUDIO2005 34042
#define ID_APPLICATIONLOOK_VISUALSTUDIO2008 34043
#define ID_APPLICATIONLOOK_OFFICE2007   34044
#define ID_OFFICE2007_BLUESTYLE         34045
#define ID_OFFICE2007_BLACKSTYLE        34046
#define ID_OFFICE2007_SILVERSTYLE       34047
#define ID_OFFICE2007_AQUASTYLE         34048
*/
IMPLEMENT_DYNAMIC(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 20;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 200;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;
int g_nWaterSpeed0 = -1;
int g_nWaterSpeed1 = -1;

//AFX_WM_RESETTOOLBAR OnToolBarReset()

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	//{{AFX_MSG_MAP(CMainFrame)
	//ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_MESSAGE(UI_RESETTOOLBAR, OnToolbarReset)
	ON_MESSAGE(UI_INVALIDATE_TOOLBARS, OnInvalidateToolBars)
	//ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_REGISTERED_MESSAGE(AFX_WM_TOOLBARMENU, OnToolbarContextMenu)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
	ON_COMMAND_RANGE(ID_VIEW_APPLICATIONLOOK, ID_OFFICE2007_AQUASTYLE, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLICATIONLOOK, ID_OFFICE2007_AQUASTYLE, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCRBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_ULTRASONICS_OSCILLOSCOPE, OnUltrasonicsOscilloscope)
	ON_COMMAND(ID_SYSTEM_CARDS, OnSystemCards)
	ON_COMMAND(ID_SYSTEM_OPTIONS, OnSystemOptions)
	ON_COMMAND(ID_SYSTEM_AXES, OnSystemAxes)
	ON_COMMAND(ID_PROFILES_IMPORT, OnProfilesImport)
	ON_COMMAND(ID_PROFILES_EDIT, OnProfilesEdit)
	ON_UPDATE_COMMAND_UI(ID_PROFILES_EDIT, OnUpdateProfilesEdit)
	ON_COMMAND(ID_PROFILES_OPEN, OnProfilesOpen)
	ON_COMMAND(ID_PROFILES_SAVEAS, OnProfilesSaveas)
	ON_COMMAND(ID_SYSTEM_ASSIGNAXES, OnSystemAssignaxes)
	ON_COMMAND(ID_PALETTE_OPEN, OnPaletteOpen)
	ON_COMMAND(ID_PALETTE_SAVEAS, OnPaletteSaveas)
	ON_COMMAND(ID_PALETTE_EDIT, OnPaletteEdit)
	ON_UPDATE_COMMAND_UI(ID_PALETTE_EDIT, OnUpdatePaletteEdit)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_MOTION_TOOLS, OnMotionTools)
	ON_COMMAND(ID_SYSTEM_HOLDINGFIXTURE, OnSystemHoldingfixture)
	ON_COMMAND(ID_SYSTEM_PMAC, OnSystemPmac)
	ON_WM_CLOSE()
	ON_COMMAND(ID_BUTTON_ABORT, OnButtonAbort)
	ON_COMMAND(ID_BUTTON_WATERPATH, OnButtonWaterPath)
	ON_COMMAND(ID_BUTTON_JOYSTICK, OnButtonJoystick)
	ON_COMMAND(ID_SYSTEM_KINEMATICS, OnSystemKinematics)
	ON_COMMAND(ID_BUTTON_SAFE, OnButtonSafe)
	ON_COMMAND(ID_ULTRASONICS_TILEOSCILLOSCOPESVERTICALLY, OnUltrasonicsTileoscilloscopesvertically)
	ON_COMMAND(ID_ULTRASONICS_TILEOSCILLOSCOPESHORIZONTALLY, OnUltrasonicsTileoscilloscopeshorizontally)
	ON_COMMAND(ID_ULTRASONICS_OSCILLOSCOPEPOSITIONS_SHIFTALLLEFT, OnUltrasonicsOscilloscopepositionsShiftallleft)
	ON_COMMAND(ID_ULTRASONICS_OSCILLOSCOPEPOSITIONS_SHIFTALLRIGHT, OnUltrasonicsOscilloscopepositionsShiftallright)
	ON_COMMAND(ID_BUTTON_LEFT_PUMP, OnButtonLeftPump)
	ON_COMMAND(ID_BUTTON_RIGHT_PUMP, OnButtonRightPump)
	ON_COMMAND(ID_MAINFRM_MOVERIGHTTOFINDLEFT, OnMainfrmMoverighttofindleft)
	ON_COMMAND(ID_MAINFRM_MOVELEFTTOFINDRIGHT, OnMainfrmMovelefttofindright)
	ON_COMMAND(ID_SCAN_SCAN, OnScanScan)
	ON_WM_TIMER()
	ON_COMMAND(ID_ULTRASONICS_OPEN, OnUltrasonicsOpen)
	ON_COMMAND(ID_ULTRASONICS_SAVEAS, OnUltrasonicsSaveas)
	ON_COMMAND(ID_ULTRASONICS_SPECTRUMANALYZER, OnUltrasonicsSpectrumanalyzer)
	ON_COMMAND(ID_SYSTEM_IMPORTTHIRDPARTYIMAGE, OnSystemImportthirdpartyimage)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_USEFULL_HT, OnUpdateButtonUsefullHt)
	ON_COMMAND(ID_BUTTON_USEFULL_HT, OnButtonUsefullHt)
	ON_COMMAND(ID_BUTTON_LEFT_AIR, OnButtonLeftAir)
	ON_COMMAND(ID_BUTTON_RIGHT_AIR, OnButtonRightAir)
	ON_COMMAND(ID_SYSTEM_EDITSCANDETAILS, OnSystemEditscandetails)
	ON_COMMAND(ID_SYSTEM_USERS, OnSystemUsers)
	ON_COMMAND(ID_FILE_LOGON, OnFileLogon)
	ON_COMMAND(ID_SYSTEM_ACCESSLEVELS, OnSystemAccesslevels)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_ACCESSLEVELS, OnUpdateSystemAccesslevels)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_ASSIGNAXES, OnUpdateSystemAssignaxes)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_AXES, OnUpdateSystemAxes)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_CARDS, OnUpdateSystemCards)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_DEVICERESOURCES, OnUpdateSystemDeviceresources)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_EDITSCANDETAILS, OnUpdateSystemEditscandetails)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_HOLDINGFIXTURE, OnUpdateSystemHoldingfixture)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_IMPORTTHIRDPARTYIMAGE, OnUpdateSystemImportthirdpartyimage)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_KINEMATICS, OnUpdateSystemKinematics)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_OPTIONS, OnUpdateSystemOptions)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_ORIENTATIONLANGUAGE, OnUpdateSystemOrientationlanguage)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_PMAC, OnUpdateSystemPmac)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_USERS, OnUpdateSystemUsers)
	ON_UPDATE_COMMAND_UI(ID_PROFILES_IMPORT, OnUpdateProfilesImport)
	ON_UPDATE_COMMAND_UI(ID_PROFILES_OPEN, OnUpdateProfilesOpen)
	ON_UPDATE_COMMAND_UI(ID_PROFILES_SAVEAS, OnUpdateProfilesSaveas)
	ON_UPDATE_COMMAND_UI(ID_ULTRASONICS_OPEN, OnUpdateUltrasonicsOpen)
	ON_UPDATE_COMMAND_UI(ID_ULTRASONICS_OSCILLOSCOPE, OnUpdateUltrasonicsOscilloscope)
	ON_UPDATE_COMMAND_UI(ID_ULTRASONICS_OSCILLOSCOPEPOSITIONS_SHIFTALLLEFT, OnUpdateUltrasonicsOscilloscopepositionsShiftallleft)
	ON_UPDATE_COMMAND_UI(ID_ULTRASONICS_OSCILLOSCOPEPOSITIONS_SHIFTALLRIGHT, OnUpdateUltrasonicsOscilloscopepositionsShiftallright)
	ON_UPDATE_COMMAND_UI(ID_ULTRASONICS_SAVEAS, OnUpdateUltrasonicsSaveas)
	ON_COMMAND(ID_PROFILES_IGESFILEIMPORT, OnProfilesIgesfileimport)
	ON_COMMAND(ID_FILE_DATABASE, OnFileDatabase)
	ON_COMMAND(ID_VIEW_BMPVIEWER, OnViewBmpviewer)
	ON_COMMAND(ID_MOTION_BISECT, OnMotionBisect)
	ON_COMMAND(ID_SYSTEM_SAFETYINTERLOCKS, OnSystemSafetyinterlocks)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_SAFETYINTERLOCKS, OnUpdateSystemSafetyinterlocks)
	ON_UPDATE_COMMAND_UI(ID_ULTRASONICS_SPECTRUMANALYZER, OnUpdateUltrasonicsSpectrumanalyzer)
	ON_WM_HELPINFO()
	ON_COMMAND(ID_ULTRASONICS_FILTERS, OnUltrasonicsFilters)
	ON_COMMAND(ID_ULTRASONICS_LSABSCAN, OnUltrasonicsLsabscan)
	ON_COMMAND(ID_ULTRASONICS_PULSERRECEIVER, OnUltrasonicsPulserreceiver)
	ON_COMMAND(ID_BUTTON_CTRL_POINT, OnButtonCtrlPoint)
	ON_COMMAND(ID_BUTTON_CTRL_TEXT, OnButtonCtrlText)
	ON_COMMAND(ID_BUTTON_CTRL_EDIT, OnButtonCtrlEdit)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CTRL_POINT, OnUpdateButtonCtrlPoint)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CTRL_TEXT, OnUpdateButtonCtrlText)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CTRL_EDIT, OnUpdateButtonCtrlEdit)
	ON_COMMAND(ID_BUTTON_CTRL_BUTTON, OnButtonCtrlButton)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CTRL_BUTTON, OnUpdateButtonCtrlButton)
	ON_COMMAND(ID_BUTTON_LEFT_ALIGN, OnButtonLeftAlign)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LEFT_ALIGN, OnUpdateButtonLeftAlign)
	ON_COMMAND(ID_BUTTON_RIGHT_ALIGN, OnButtonRightAlign)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_RIGHT_ALIGN, OnUpdateButtonRightAlign)
	ON_COMMAND(ID_BUTTON_SAME_WIDTH, OnButtonSameWidth)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SAME_WIDTH, OnUpdateButtonSameWidth)
	ON_COMMAND(ID_BUTTON_SAME_HEIGHT, OnButtonSameHeight)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SAME_HEIGHT, OnUpdateButtonSameHeight)
	ON_COMMAND(ID_BUTTON_SPACE_VERTICALLY, OnButtonSpaceVertically)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SPACE_VERTICALLY, OnUpdateButtonSpaceVertically)
	ON_COMMAND(ID_BUTTON_TOP_ALIGN, OnButtonTopAlign)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_TOP_ALIGN, OnUpdateButtonTopAlign)
	ON_COMMAND(ID_ULTRASONICS_PROBECHARACTERSATION, OnUltrasonicsProbecharactersation)
	ON_COMMAND(ID_BUTTON_POWER, OnButtonPower)
	ON_COMMAND(ID_BUTTON_AUTO, OnButtonAuto)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_MACHINEDESCRIPTION, OnUpdateSystemMachinedescription)
	ON_COMMAND(ID_SYSTEM_CUSTOMIZE, OnSystemCustomize)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_CUSTOMIZE, OnUpdateSystemCustomize)
	ON_COMMAND(ID_MOTION_KINEMATICS, OnMotionKinematics)
	ON_COMMAND(ID_BUTTON_ORIGIN, OnButtonOrigin)
	ON_COMMAND(ID_BUTTON_MOVETO_HOME, OnButtonMovetoHome)
	ON_COMMAND(ID_SYSTEM_ARCHIVE, OnSystemArchive)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_JOYSTICK, OnUpdateButtonJoystick)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SAFE, OnUpdateButtonSafe)
	ON_COMMAND(ID_FILE_LOGOFF, OnFileLogoff)
	ON_UPDATE_COMMAND_UI(ID_SYSTEM_ARCHIVE, OnUpdateSystemArchive)
	ON_UPDATE_COMMAND_UI(ID_PALETTE_OPEN, OnUpdatePaletteOpen)
	ON_UPDATE_COMMAND_UI(ID_PALETTE_SAVEAS, OnUpdatePaletteSaveas)
	ON_COMMAND(ID_SYSTEM_MAINTENANCEDIAGNOSTICS, OnSystemMaintenancediagnostics)
	ON_COMMAND(ID_BUTTON_MOUSE, OnButtonMouse)
	ON_COMMAND(ID_BUTTON_LINE, OnButtonLine)
	ON_COMMAND(ID_BUTTON_ELLIPSE, OnButtonEllipse)
	ON_COMMAND(ID_BUTTON_MULTI_LINE, OnButtonMultiLine)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MOUSE, OnUpdateButtonMouse)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LINE, OnUpdateButtonLine)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ELLIPSE, OnUpdateButtonEllipse)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MULTI_LINE, OnUpdateButtonMultiLine)
	ON_COMMAND(ID_BUTTON_MOVE, OnButtonMove)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MOVE, OnUpdateButtonMove)
	ON_UPDATE_COMMAND_UI(ID_MOTION_KINEMATICS, OnUpdateMotionKinematics)
	ON_COMMAND(ID_PROFILES_IMPORTDESIGNER, OnProfilesImportdesigner)
	ON_COMMAND(ID_FILE_OPENBSCAN, OnFileOpenBScan)
	ON_UPDATE_COMMAND_UI(ID_FILE_OPENBSCAN, OnUpdateFileOpenBScan)
	ON_UPDATE_COMMAND_UI(ID_ULTRASONICS_PROBECHARACTERSATION, OnUpdateUltrasonicsProbecharactersation)
	ON_UPDATE_COMMAND_UI(ID_ULTRASONICS_LSABSCAN, OnUpdateUltrasonicsLsabscan)
	ON_UPDATE_COMMAND_UI(ID_ULTRASONICS_FILTERS, OnUpdateUltrasonicsFilters)
	ON_UPDATE_COMMAND_UI(ID_PROFILES_IMPORTDESIGNER, OnUpdateProfilesImportdesigner)
	ON_UPDATE_COMMAND_UI(ID_PROFILES_IGESFILEIMPORT, OnUpdateProfilesIgesfileimport)
	ON_UPDATE_COMMAND_UI(ID_MOTION_BISECT, OnUpdateMotionBisect)
	ON_COMMAND(ID_ULTRASONICS_ENABLEETHERNET, OnUltrasonicsEnableethernet)
	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_COMMAND(ID_BUTTON_TIPS_POINT_DOWN, OnButtonTipsPointDown)
	ON_COMMAND(ID_BUTTON_TIPS_REMOVE_AIR, OnButtonTipsRemoveAir)
	ON_COMMAND(ID_BUTTON_TIPS_TO_45, OnButtonTipsTo45)
	ON_COMMAND(ID_BUTTON_TIPS_TO_HORIZONTAL, OnButtonTipsToHorizontal)
	ON_COMMAND(ID_BUTTON_TIPS_TO_ZERO, OnButtonTipsToHorizontal)
	ON_COMMAND(ID_BUTTON_SWIVAL_TO_ZERO, OnButtonSwivalToZero)
	ON_COMMAND(ID_BUTTON_INSIDE_JOYSTICK, OnButtonInsideJoystick)
	ON_COMMAND(ID_BUTTON_OUTSIDE_JOYSTICK, OnButtonOutsideJoystick)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_INSIDE_JOYSTICK, OnUpdateButtonInsideJoystick)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_OUTSIDE_JOYSTICK, OnUpdateButtonOutsideJoystick)
	ON_COMMAND(ID_BUTTON_PULSE_ECHO, OnButtonPulseEcho)
	ON_COMMAND(ID_BUTTON_THROUGH_TRANSMISSION, OnButtonThroughTransmission)
	ON_COMMAND(ID_HELP_USLSCANNEROPERATORMANUAL, OnHelpUslscanneroperatormanual)
	ON_COMMAND(ID_HELP_PMACSOFTWAREMANUAL, OnHelpPmacsoftwaremanual)
	ON_COMMAND(ID_SYSTEM_ROBOTS, OnSystemRobots)
	ON_COMMAND(ID_BUTTON_PR30_0, OnButtonPr300)
	ON_COMMAND(ID_BUTTON_PR30_1, OnButtonPr301)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_PR30_0, OnUpdateButtonPr300)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_PR30_1, OnUpdateButtonPr301)
	ON_COMMAND(ID_FILE_UPDATELICENSE,OnFileUpdateLicense)
	ON_COMMAND(ID_BUTTON_BLADE_ZERO_DEGREES, OnButtonBladeZeroDegrees)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_BLADE_ZERO_DEGREES, OnUpdateButtonBladeZeroDegrees)
	ON_COMMAND(ID_BUTTON_BLADE_180_DEGREES, OnButtonBlade180Degrees)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_BLADE_180_DEGREES, OnUpdateButtonBlade180Degrees)
	ON_COMMAND(ID_BUTTON_BLADE_HOME, OnButtonBladeHome)
	ON_COMMAND(ID_BUTTON_TURNTABLE_ENABLE, OnButtonTurntableEnable)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_TURNTABLE_ENABLE, OnUpdateButtonTurntableEnable)
	ON_COMMAND(ID_BUTTON_ROLLERS_ENABLE, OnButtonRollersEnable)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ROLLERS_ENABLE, OnUpdateButtonRollersEnable)
	ON_COMMAND(ID_SYSTEM_EDIT_PROPERTIES, OnSystemEditProperties)
	ON_COMMAND(ID_SYSTEM_EDIT_DETAILS, OnSystemEditDetails)
	ON_COMMAND(ID_BUTTON_MARKER_PEN, OnButtonMarkerPen)
	ON_COMMAND(ID_ULTRASONICS_PHASEDARRAYPROBE, OnUltrasonicsPhasedarrayprobe)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_BUTTON_PA_EQUAL_WATER_PATH, OnButtonPaEqualWaterPath)
	ON_COMMAND(ID_VIEW_BLADE, OnViewBlade)
	ON_COMMAND(ID_BUTTON_WATER_SPRAY, OnButtonWaterSpray)
	ON_COMMAND(ID_BUTTON_PARK, OnButtonPark)
	ON_COMMAND(ID_BUTTON_STEP_OFF_ON, OnButtonStepOffOn)
	ON_COMMAND(ID_BUTTON_PRE_WET_ROLLER, OnButtonPreWetRoller)
	ON_COMMAND(ID_BUTTON_ENABLE_POWER_TO_ROBOTS, OnButtonEnablePowerToRobots)
	ON_COMMAND(ID_VIEW_LINEEXPANSION, OnViewLineexpansion)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_XYZ_MODE, OnUpdateButtonXyzMode)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_YZR_MODE, OnUpdateButtonYzrMode)
	ON_COMMAND(ID_ULTRASONICS_INCLUSIONS, OnUltrasonicsInclusions)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MARKER_PEN, OnUpdateButtonMarkerPen)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_RIGHT_AIR, OnUpdateButtonRightAir)
	ON_COMMAND(ID_SYSTEM_QUATERNIONS, OnSystemQuaternions)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LEFT_AIR, OnUpdateButtonLeftAir)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LEFT_PUMP, OnUpdateButtonLeftPump)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_RIGHT_PUMP, OnUpdateButtonRightPump)
	ON_COMMAND(ID_BUTTON_MOVETO_SURFACE_ZERO, OnButtonMovetoSurfaceZero)
	ON_COMMAND(ID_BUTTON_MOVETO_MACHINE_ZERO, OnButtonMovetoMachineZero)
	ON_COMMAND(ID_ULTRASONICS_OSCILLOSCOPE_1, OnUltrasonicsOscilloscope1)
	ON_COMMAND(ID_ULTRASONICS_OSCILLOSCOPE_2, OnUltrasonicsOscilloscope2)
	ON_COMMAND(ID_ULTRASONICS_OSCILLOSCOPE_3, OnUltrasonicsOscilloscope3)
	ON_COMMAND(ID_ULTRASONICS_OSCILLOSCOPE_4, OnUltrasonicsOscilloscope4)
	ON_COMMAND(ID_ULTRASONICS_OSCILLOSCOPE_5, OnUltrasonicsOscilloscope5)
	ON_COMMAND(ID_ULTRASONICS_OSCILLOSCOPE_6, OnUltrasonicsOscilloscope6)
	ON_COMMAND(ID_ULTRASONICS_OSCILLOSCOPE_7, OnUltrasonicsOscilloscope7)
	ON_COMMAND(ID_ULTRASONICS_OSCILLOSCOPE_8, OnUltrasonicsOscilloscope8)
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_BUTTON_DANGERPLANE_0, OnButtonDangerplane0)
	ON_COMMAND(ID_BUTTON_DANGERPLANE_1, OnButtonDangerplane1)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_DANGERPLANE_0, OnUpdateButtonDangerplane0)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_DANGERPLANE_1, OnUpdateButtonDangerplane1)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(ID_EDIT_LEFT_WATER_PATH, OnChangeEditLeftWaterPath)
	ON_EN_CHANGE(ID_EDIT_RIGHT_WATER_PATH, OnChangeEditRightWaterPath)
	ON_EN_CHANGE(ID_EDIT_THICKNESS, OnChangeEditThickness)
	ON_EN_CHANGE(ID_EDIT_LEFT_PROBE_LENGTH, OnChangeEditProbeLength)
	ON_MESSAGE(INVALIDATE_VIEW,OnMessageInvalidateView)
	ON_MESSAGE(UI_PROFILE_OPENED,ProfileRetrievedUpdateDialogs)
	ON_EN_CHANGE(IDC_EDIT_PUMP_SPEED_00, OnChangeEditPumpSpeed00)
	ON_EN_CHANGE(IDC_EDIT_PUMP_SPEED_01, OnChangeEditPumpSpeed01)
	ON_EN_CHANGE(IDC_EDIT_PUMP_SPEED_0, OnChangeEditPumpSpeed0)
	ON_EN_CHANGE(IDC_EDIT_PUMP_SPEED_1, OnChangeEditPumpSpeed1)
	ON_EN_VSCROLL(IDC_EDIT_PUMP_SPEED_00, OnVScrollEditPumpSpeed00)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PUMP_SPEED_0, OnDeltaposSpinPumpSpeed0)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PUMP_SPEED_1, OnDeltaposSpinPumpSpeed1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PUMP_SPEED_00, OnDeltaposSpinPumpSpeed00)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PUMP_SPEED_01, OnDeltaposSpinPumpSpeed01)
	ON_NOTIFY(UDN_DELTAPOS, ID_SPIN_LEFT_WATER_PATH, OnDeltaposSpinLeftWaterPath)
	ON_NOTIFY(UDN_DELTAPOS, ID_SPIN_RIGHT_WATER_PATH, OnDeltaposSpinRightWaterPath)
	ON_NOTIFY(UDN_DELTAPOS, ID_SPIN_THICKNESS, OnDeltaposSpinThickness)
	ON_NOTIFY(UDN_DELTAPOS, ID_SPIN_LEFT_PROBE_LENGTH, OnDeltaposSpinProbeLength)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_FEED_RATE, OnDeltaposSliderFeedRate)
	ON_MESSAGE(UI_UPDATE_AXES, UpdateAxes)
	ON_MESSAGE(UI_UPDATE_3D_VIEW_DLG, Invalidate3DViewPage)
	ON_MESSAGE(UI_TRACKER_COORDINATE_ADDED, TrackerCoordinateAdded)
	ON_MESSAGE(UI_VIEW_LASER_TRACKER_COORDINATES, OnViewLasertracker)
	ON_MESSAGE(UI_UPDATE_PROFILE_DLG, Update3DViewDlg)
	ON_MESSAGE(UI_UPDATE_ROBOT_SHEET, UpdateRobotSheet)
	ON_MESSAGE(UI_UPDATE_PALETTE_SHEET, UpdatePaletteSheet)
	ON_MESSAGE(UI_UPDATE_PMAC_SHEET, UpdatePmacSheet)
	ON_MESSAGE(UI_UPDATE_PHASED_ARRAY_SHEET, UpdatePhasedArraySheet)
	ON_MESSAGE(UI_UPDATE_MOTION_TOOLS, RefreshMotionTools)
	ON_MESSAGE(UI_UPDATE_ULTRASONICS_SHEET, RefreshUltrasonicsSheet)
	ON_MESSAGE(UI_CLOSE_ALL_IMAGES, CloseAllImages)
	ON_MESSAGE(UI_FOCAL_LAW_CHANGED, OnUpdateFocalLawChanged)
	ON_MESSAGE(UI_UPDATE_GATES_UT_PAGE, UpdateGatesPage)
	ON_MESSAGE(UI_UPDATE_DIALOG_BAR, InitializeDialogBar)
	ON_MESSAGE(UI_INVALIDATE_SCAN_START_TIME, InvalidateScanTime)
	ON_MESSAGE(UI_UPDATE_PUMPS_DIALOGBAR, UpdatePumpsDialogBar)
	ON_MESSAGE(UI_LANGUAGE_CHANGED,LanguageChanged)
	ON_MESSAGE(UI_UPDATE_MOTION_TOOLS_SHEET, UpdateMotionToolsSheet)
	ON_MESSAGE(UI_INVALIDATE_COORD_ARRAY_PANE, InvalidateCoordArrayPane)
	ON_MESSAGE(UI_SHOW_PROPERTIES_PANE, &CMainFrame::OnShowPropertiesPane)
	ON_MESSAGE(UI_INVALIDATE_FOR_ROLLER_KINEMATICS, &CMainFrame::UpdateAxes)
	ON_MESSAGE(UI_INVALIDATE_OPTIMIZE_PANE, &CMainFrame::InvalidateOptimizePane)
	ON_MESSAGE(UI_SET_ACCESS_PRIVELAGES, &CMainFrame::SetAccessPrivelages)
	ON_MESSAGE(UI_INVALIDATE_VIEW_SHEETS, &CMainFrame::InvalidateViewSheets)

	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	ON_MESSAGE(REMOTE_HANDLER, RemoteHandler)
	ON_MESSAGE(REMOTE_HANDLER_PROFILE, RemoteHandlerProfile)
	ON_MESSAGE(REMOTE_RMESSAGE_SEND, RemoteRMessageSend)
	ON_MESSAGE(REMOTE_RMESSAGE_RECEIVE, RemoteRMessageReceive)
	ON_MESSAGE(REMOTE_START_SERVER, RemoteStartServer)
	ON_MESSAGE(REMOTE_HANDLER_ULTRASONICS, RemoteHandlerUltrasonics)
	ON_MESSAGE(REMOTE_HANDLER_FLSC10, RemoteHandlerFLSC10)
	ON_MESSAGE(REMOTE_HANDLER_EGISMOSRF, RemoteHandlerEgismosRF)
	ON_MESSAGE(REMOTE_PAGE_UPDATE_CONTROLS, RemotePageUpdateControls)

		ON_UPDATE_COMMAND_UI(IDC_EDIT_PUMP_SPEED_00, &CMainFrame::OnUpdateEditPumpSpeed00)
		ON_UPDATE_COMMAND_UI(IDC_SPIN_PUMP_SPEED_00, &CMainFrame::OnUpdateSpinPumpSpeed00)
		ON_UPDATE_COMMAND_UI(IDC_EDIT_PUMP_SPEED_01, &CMainFrame::OnUpdateEditPumpSpeed01)
		ON_UPDATE_COMMAND_UI(IDC_SPIN_PUMP_SPEED_01, &CMainFrame::OnUpdateSpinPumpSpeed01)
		ON_UPDATE_COMMAND_UI(ID_EDIT_LEFT_WATER_PATH, &CMainFrame::OnUpdateEditLeftWaterPath)
		ON_UPDATE_COMMAND_UI(ID_EDIT_RIGHT_WATER_PATH, &CMainFrame::OnUpdateEditRightWaterPath)
		ON_UPDATE_COMMAND_UI(ID_EDIT_THICKNESS, &CMainFrame::OnUpdateEditThickness)
		ON_UPDATE_COMMAND_UI(ID_EDIT_LEFT_PROBE_LENGTH, &CMainFrame::OnUpdateEditProbeLength)
		ON_COMMAND(IDC_EDIT_PUMP_SPEED_00, &CMainFrame::OnEditPumpSpeed00)
		ON_WM_MOUSEWHEEL()
		ON_COMMAND(ID_VIEW_STATUS, &CMainFrame::OnViewStatus)
		ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS, &CMainFrame::OnUpdateViewStatus)
		ON_COMMAND(ID_POSITION_DOCKINGPANE, &CMainFrame::OnPositionDockingpane)
		ON_UPDATE_COMMAND_UI(ID_POSITION_DOCKINGPANE, &CMainFrame::OnUpdatePositionDockingpane)
		ON_COMMAND(ID_POSITION_DIALOGE, &CMainFrame::OnPositionDialoge)
		ON_UPDATE_COMMAND_UI(ID_POSITION_DIALOGE, &CMainFrame::OnUpdatePositionDialoge)
		ON_COMMAND(ID_SYSTEM_PROPERTIES, &CMainFrame::OnSystemProperties)
		ON_UPDATE_COMMAND_UI(ID_SYSTEM_PROPERTIES, &CMainFrame::OnUpdateSystemProperties)
		ON_COMMAND(ID_DOCKABLES_AXESSTATUS, &CMainFrame::OnDockablesAxesstatus)
		ON_UPDATE_COMMAND_UI(ID_DOCKABLES_AXESSTATUS, &CMainFrame::OnUpdateDockablesAxesstatus)
		ON_COMMAND(ID_DOCKABLES_AXES, &CMainFrame::OnDockablesAxes)
		ON_UPDATE_COMMAND_UI(ID_DOCKABLES_AXES, &CMainFrame::OnUpdateDockablesAxes)
		ON_COMMAND(ID_DOCKABLES_COORDINATES, &CMainFrame::OnDockablesCoordinates)
		ON_UPDATE_COMMAND_UI(ID_DOCKABLES_COORDINATES, &CMainFrame::OnUpdateDockablesCoordinates)
		ON_COMMAND(ID_DOCKABLES_3DWORLDVIEW, &CMainFrame::OnDockables3dworldview)
		ON_UPDATE_COMMAND_UI(ID_DOCKABLES_3DWORLDVIEW, &CMainFrame::OnUpdateDockables3dworldview)
		ON_COMMAND(ID_WINDOW_RESETWINDOWLAYOUT, &CMainFrame::OnWindowResetwindowlayout)
		ON_COMMAND(ID_DOCKABLES_OPTIMIZE, &CMainFrame::OnDockablesOptimize)
		ON_UPDATE_COMMAND_UI(ID_DOCKABLES_OPTIMIZE, &CMainFrame::OnUpdateDockablesOptimize)
		ON_COMMAND(ID_DOCKABLES_AXIS, &CMainFrame::OnDockablesAxis)
		ON_UPDATE_COMMAND_UI(ID_DOCKABLES_AXIS, &CMainFrame::OnUpdateDockablesAxis)
		END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	IDS_SCAN_TIME_MESSAGE,
	IDS_STATUS_MESSAGES,
	IDS_STATUS_OPERATOR,
	IDS_STATUS_POSITION,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

#define	SCAN_TIME_PANE		1
#define	STATUS_MESSAGE_PANE	2
#define OPERATOR_PANE		3
#define POSITION_PANE		4

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

BOOL CMainFrame::OnToolTipNotify(UINT id, NMHDR *pNMHDR,   LRESULT *pResult)
{

   TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
   TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
   CString strTipText, Buff;
   UINT nID = pNMHDR->idFrom;
   if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
      pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
   {
      nID = ::GetDlgCtrlID((HWND)nID);
   }

   switch(nID) {
   case ID_BUTTON_ABORT: strTipText.LoadString(IDS_Abort);
	   break;
   case ID_BUTTON_JOYSTICK: strTipText.LoadString(IDS_Joystick);
	   break;
   case ID_BUTTON_LEFT_PUMP:
	   Buff.LoadString(IDS_Pump);
	   strTipText.Format(L"%s %s",theApp.m_DlgSideName[0], Buff);
	   break;
   case ID_BUTTON_RIGHT_PUMP:
	   Buff.LoadString(IDS_Pump);
	   strTipText.Format(L"%s %s",theApp.m_DlgSideName[1], Buff);
	   break;
   case ID_BUTTON_LEFT_AIR:
	   Buff.LoadString(IDS_Air);
	   strTipText.Format(L"%s %s",theApp.m_DlgSideName[0], Buff);
	   break;
   case ID_BUTTON_RIGHT_AIR:
	   Buff.LoadString(IDS_Air);
	   strTipText.Format(L"%s %s",theApp.m_DlgSideName[1], Buff);
	   break;
   case ID_BUTTON_DANGERPLANE_0:
	   Buff.LoadString(IDS_Goto_via_danger_plane);
	   strTipText.Format(L"%s %s",theApp.m_DlgSideName[0], Buff);
	   break;
   case ID_BUTTON_DANGERPLANE_1:
	   Buff.LoadString(IDS_Goto_via_danger_plane);
	   strTipText.Format(L"%s %s",theApp.m_DlgSideName[1], Buff);
	   break;
   case ID_BUTTON_TIPS_REMOVE_AIR: strTipText.LoadString(IDS_Remove_air);
	   break;
   case ID_BUTTON_TIPS_TO_ZERO: strTipText.LoadString(IDS_Tips_to_zero);
	   break;
 //  case ID_BUTTON_USEFULL_HT: strTipText.LoadString(IDS_HT);
//	   break;
   case ID_FILE_LOGON: strTipText.LoadString(IDS_Log_on);
	   break;
   case ID_FILE_LOGOFF: strTipText.LoadString(IDS_Log_off);
	   break;
   }
//      strTipText.Format((L"Control ID = %d", nID);

//   if (pNMHDR->code == TTN_NEEDTEXTA)
      lstrcpyn((WCHAR*)pTTTA->szText, strTipText, sizeof(pTTTA->szText));
//   else
//      ::MultiByteToWideChar( CP_ACP , 0, strTipText, -1, pTTTW->szText, sizeof(pTTTW->szText) );
   *pResult = 0;

   return TRUE;    // message was handled
}

CMainFrame::CMainFrame()
{
	int	nn;

	m_CtrlType = (CtrlType)0;

	m_pScanView = NULL;
	m_pAxesSheet = NULL;
	m_p3DViewEditDlg = NULL;
	m_pPaletteSheet = NULL;
	m_pUltrasonicsSheet = NULL;
	m_pMotionToolsSheet=NULL;
	m_pStepperSheet=NULL;
	m_pSerialJoystickDlg=NULL;
	m_pPmacSheet = NULL;
	m_pPositionSheet = NULL;
	m_pCompensationSheet=NULL;
	m_pScanSheet=NULL;
	m_pSpectrumAnalyzerDlg=NULL;
	m_pEditDetailsDlg=NULL;
	m_pUsersDlg=NULL;
	m_pAccessLevelsDlg=NULL;
	m_pIGESDlg=NULL;
	m_pDataBaseDlg=NULL;
	m_pBMPViewerDlg=NULL;
	m_pSafetyInterlocksDlg=NULL;
	m_pFilterSheet = NULL;
	m_pLSABScanDlg=NULL;
	m_pDetailsPage=NULL;
	m_pProbeCharacterisationSheet=NULL;
	m_pCustomizeSheet=NULL;
	m_pArchiveSheet=NULL;
	m_pProfileSheet = NULL;
	m_pMaintenanceDiagnosticsSheet = NULL;
	m_pImportToolPathsDlg = NULL;
	m_pMultipleBScansDlg = NULL;
	m_pWorkSpaceSheet = NULL;
	m_pAssignAxesDlg = NULL;
	m_pRobotSheet = NULL;
	m_pFaroCoordinateDlg = NULL;
	m_pDataPropertiesDlg = NULL;
	m_pPolyCoordDlg = NULL;
	m_pInclusionsSheet = NULL;
	m_pQuaternionsPage = NULL;
	m_pPhasedArraySheet = NULL;
	m_nOldMeasurementUnits = theApp.m_LastSettings.nMeasurementUnits;

	m_pToolBarMotion = NULL;
	m_pToolBarHt = NULL;
	m_pToolBarPowered = NULL;
//	m_pToolBarManipulator = NULL;
//	m_pDialogBarPumps = NULL;
	m_pToolBarWaterPath = NULL;
	m_pToolBarTipAxes = NULL;
	m_pToolBarJoystick = NULL;
	m_pToolBarUltrasonics = NULL;
	m_pToolBarBlade = NULL;
	m_pToolBarTurntableRoller = NULL;
	m_pToolBarMarkerPen = NULL;
	m_pToolBarPhasedArray = NULL;
	m_pToolBarXRSwap = NULL;

	m_pToolBarMotion = NULL;
	m_pToolBarUltrasonics = NULL;
	m_pDialogBarPumps = NULL;
//	m_pToolBarManipulator = NULL;
//	m_pToolBarAirWater0 = NULL;
//	m_pToolBarAirWater1 = NULL;

	m_pEditBoxButtonLeft = NULL;
	m_pEditBoxButtonRight = NULL;
	m_pSpinLeftPump = NULL;
	m_pSpinRightPump = NULL;
	m_pEditLeftWp = NULL;
	m_pEditRightWp = NULL;
	m_pEditThickness = NULL;
	m_pSpinLeftWp = NULL;
	m_pSpinRightWp = NULL;
	m_pSpinThickness = NULL;
	m_pEditWp0 = NULL;
	m_pSpinWp0 = NULL;
	m_pEditProbeLength = NULL;
	m_pSpinProbeLength = NULL;

	for(nn=0;nn<2;nn++) {
		m_pThirdPartyImageDlg[nn]=NULL;
	}
	for(nn=0;nn<8;nn++) {
		m_pScopeDlg[nn] = NULL;
	}
	for(nn=0;nn<MAX_NUMBER_VIEWS;nn++) {
		m_pViewSheet[nn]=NULL;
		m_pViewBladeThicknessDlg[nn] = NULL;
	}
	m_pScanViewBladeThicknessDlg = NULL;

	ZeroMemory(m_pwndToolBar, sizeof m_pwndToolBar);
	ZeroMemory(m_pwndDialogBar, sizeof m_pwndDialogBar);

	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_APPLICATIONLOOK_VISUALSTUDIO2008);

}

CMainFrame::~CMainFrame()
{
	for(int ii=0;ii<MAX_TOOLBARS;ii++) {
		SAFE_DELETE(m_pwndToolBar[ii]);
		SAFE_DELETE(m_pwndDialogBar[ii]);
	}

	SAFE_DELETE(m_pEditBoxButtonLeft);
	SAFE_DELETE(m_pEditBoxButtonRight);
	SAFE_DELETE(m_pSpinLeftPump);
	SAFE_DELETE(m_pSpinRightPump);
	SAFE_DELETE(m_pEditLeftWp);
	SAFE_DELETE(m_pEditRightWp);
	SAFE_DELETE(m_pEditThickness);
	SAFE_DELETE(m_pSpinLeftWp);
	SAFE_DELETE(m_pSpinRightWp);
	SAFE_DELETE(m_pSpinThickness);
	SAFE_DELETE(m_pEditWp0);
	SAFE_DELETE(m_pSpinWp0);
	SAFE_DELETE(m_pEditProbeLength);
	SAFE_DELETE(m_pSpinProbeLength);


}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	InitToolbarButtons();

	CString Buff, Version, Title;

	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;


//	if (!m_wndMenuBar.CreateEx(this))
//	{
//		TRACE0("Failed to create menubar\n");
//		return -1;      // fail to create
//	}
	
	TRY
	{
//		m_wndMenuBar.Create(this);
		m_wndMenuBar.CreateFromMenu(this->m_hMenu);
	}
	CATCH(CInvalidArgException, e)
	{
		e->ReportError();
	}
	END_CATCH

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL)) {
		TRACE0("Failed to create view window\n");
		return -1;
	}


	CMFCPopupMenu::SetForceMenuFocus(FALSE);	// prevent the menu bar from taking the focus on activation


	CMFCToolBar::EnableQuickCustomization();

	CList<UINT, UINT> lstBasicCommands;
	lstBasicCommands.AddTail(ID_BUTTON_LEFT_AIR);
	lstBasicCommands.AddTail(ID_BUTTON_LEFT_PUMP);
	lstBasicCommands.AddTail(IDC_EDIT_PUMP_SPEED_00);
	lstBasicCommands.AddTail(IDC_SPIN_PUMP_SPEED_00);
	lstBasicCommands.AddTail(IDC_EDIT_PUMP_SPEED_01);
	lstBasicCommands.AddTail(IDC_SPIN_PUMP_SPEED_01);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	CreateToolBars();

//	SetToolBarCheckedState(FALSE);
	if (!m_wndStatusBar.Create(this) ||	!m_wndStatusBar.SetIndicators(indicators,  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetPaneText(POSITION_PANE,L"");

	SetIcon( AfxGetApp()->LoadIcon(IDI_USL), TRUE);
	if(!(theApp.m_nLicenseMask & _MINIMAL)) {
		Title.Format((L"%s"),theApp.m_strVersion);
	} else {
		Title.Format((L"%s :Minimal Build"),theApp.m_strVersion);
	}
	SetWindowText(Title);

	StatusBarPrompt(L"Nothing",0);

	OnApplicationLook(theApp.m_nAppLook);

	theApp.m_StatusMessage.Empty();
	theApp.m_rgbStatusMessageText = RGB(255,0,0);
	theApp.m_rgbStatusMessageBk = GetSysColor(COLOR_MENU);

	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_LEFT);
	EnableAutoHidePanes(CBRS_ALIGN_RIGHT);

	// Load menu item image (not placed on any standard toolbars):
//	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}


	SetTimer(1,333,NULL);

	return 0;
}

void CMainFrame::OnClose()
{
	theApp.m_Thread.SuspendThread();
	m_wnd3DWorldViewPane.SuspendThread();

	CFrameWndEx::OnClose();
}


HRESULT CMainFrame::OnInvalidateToolBars(WPARAM wp, LPARAM lp)
{
	CString strBasePane;

	strBasePane.Format(L"BasePane-%d", IDR_MOTION_TOOLBAR_DUAL_SQUIRTER);
	theApp.GetSectionInt(strBasePane, L"IsVisible") == 1 ? m_ToolBarDualMotion.ShowPane(true, false, true) : m_ToolBarDualMotion.ShowPane(false, false, true);

	strBasePane.Format(L"BasePane-%d", IDR_AIR_WATER_SIDE_1_TOOLBAR);
	theApp.GetSectionInt(strBasePane, L"IsVisible") == 1 ? m_ToolBarAirWater1.ShowPane(true, false, true) : m_ToolBarAirWater1.ShowPane(false, false, true);

	strBasePane.Format(L"BasePane-%d", IDR_AIR_WATER_SIDE_0_TOOLBAR);
	theApp.GetSectionInt(strBasePane, L"IsVisible") == 1 ? m_ToolBarAirWater0.ShowPane(true, false, true) : m_ToolBarAirWater0.ShowPane(false, false, true);

	strBasePane.Format(L"BasePane-%d", IDR_TIPS_TOOLBAR);
	theApp.GetSectionInt(strBasePane, L"IsVisible") == 1 ? m_ToolBarTips.ShowPane(true, false, true) : m_ToolBarTips.ShowPane(false, false, true);

	strBasePane.Format(L"BasePane-%d", IDR_DUAL_WATERPATH_THICKNESS_TOOLBAR);
	theApp.GetSectionInt(strBasePane, L"IsVisible") == 1 ? m_ToolBarDualWaterPath.ShowPane(true, false, true) : m_ToolBarDualWaterPath.ShowPane(false, false, true);

	strBasePane.Format(L"BasePane-%d", IDR_USEFULL_TOOLBAR);
	theApp.GetSectionInt(strBasePane, L"IsVisible") == 1 ? m_ToolBarHt.ShowPane(true, false, true) : m_ToolBarHt.ShowPane(false, false, true);

	strBasePane.Format(L"BasePane-%d", IDR_LOG_ON_OFF_TOOLBAR);
	theApp.GetSectionInt(strBasePane, L"IsVisible") == 1 ? m_ToolBarLogOnOff.ShowPane(true, false, true) : m_ToolBarLogOnOff.ShowPane(false, false, true);

	strBasePane.Format(L"BasePane-%d", IDR_DANGERPLANE_TOOLBAR);
	theApp.GetSectionInt(strBasePane, L"IsVisible") == 1 ? m_ToolBarDangerPlanes.ShowPane(true, false, true) : m_ToolBarDangerPlanes.ShowPane(false, false, true);

	strBasePane.Format(L"BasePane-%d", IDR_ESTOP_JOY_SAFE_TOOLBAR);
	theApp.GetSectionInt(strBasePane, L"IsVisible") == 1 ? m_ToolBarEstopJoySafe.ShowPane(true, false, true) : m_ToolBarEstopJoySafe.ShowPane(false, false, true);

	strBasePane.Format(L"BasePane-%d", IDR_WP_PL_TIPS);
	theApp.GetSectionInt(strBasePane, L"IsVisible") == 1 ? m_ToolBarTips.ShowPane(true, false, true) : m_ToolBarTips.ShowPane(false, false, true);

	return NULL;
}

void CMainFrame::CreateToolBars()
{

	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!(theApp.m_nLicenseMask & _MINIMAL)) {

		if (m_ToolBarAirWater1.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_AIR_WATER_SIDE_1_TOOLBAR) &&
			m_ToolBarAirWater1.LoadToolBar(IDR_AIR_WATER_SIDE_1_TOOLBAR)) {
			m_ToolBarAirWater1.SetWindowText(L"AirWater1");
		}

		if (m_ToolBarAirWater0.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_AIR_WATER_SIDE_0_TOOLBAR) &&
			m_ToolBarAirWater0.LoadToolBar(IDR_AIR_WATER_SIDE_0_TOOLBAR)) {
			m_ToolBarAirWater0.SetWindowText(L"AirWater0");
		}
	
		InitializePumpsDialogBar();

		if(m_ToolBarDualWaterPath.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_DUAL_WATERPATH_THICKNESS_TOOLBAR) &&
			m_ToolBarDualWaterPath.LoadToolBar(IDR_DUAL_WATERPATH_THICKNESS_TOOLBAR)) {
			m_ToolBarDualWaterPath.SetWindowText(L"DualWaterPath");
		}
		if (m_ToolBarEstopJoySafe.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_ESTOP_JOY_SAFE_TOOLBAR) &&
			m_ToolBarEstopJoySafe.LoadToolBar(IDR_ESTOP_JOY_SAFE_TOOLBAR)) {
			m_ToolBarEstopJoySafe.SetWindowText(L"Estop Joy Safe");
		}

		if (m_ToolBarHt.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_USEFULL_TOOLBAR) &&
			m_ToolBarHt.LoadToolBar(IDR_USEFULL_TOOLBAR)) {
			m_ToolBarHt.SetWindowTextW(L"Ht");
		}
		if (m_ToolBarLogOnOff.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_LOG_ON_OFF_TOOLBAR) &&
			m_ToolBarLogOnOff.LoadToolBar(IDR_LOG_ON_OFF_TOOLBAR)) {
			m_ToolBarLogOnOff.SetWindowTextW(L"Log On Off");
		}
		if (m_ToolBarDualMotion.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_MOTION_TOOLBAR_DUAL_SQUIRTER) &&
			m_ToolBarDualMotion.LoadToolBar(IDR_MOTION_TOOLBAR_DUAL_SQUIRTER)) {
			m_ToolBarDualMotion.SetWindowTextW(L"Dual Motion");
		}
		if (m_ToolBarTips.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_TIPS_TOOLBAR) &&
			m_ToolBarTips.LoadToolBar(IDR_TIPS_TOOLBAR)) {
			m_ToolBarTips.SetWindowTextW(L"Tips toolbar");
		}
		if (m_ToolBarDangerPlanes.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_DANGERPLANE_TOOLBAR) &&
			m_ToolBarDangerPlanes.LoadToolBar(IDR_DANGERPLANE_TOOLBAR)) {
			m_ToolBarDangerPlanes.SetWindowTextW(L"Danger planes");
		}
		
		if (m_ToolBarWaterPathProbeLengthTips.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), IDR_WP_PL_TIPS) &&
			m_ToolBarWaterPathProbeLengthTips.LoadToolBar(IDR_WP_PL_TIPS)) {
			m_ToolBarWaterPathProbeLengthTips.SetWindowText(L"Water path Probe length");
		}

		m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
		m_ToolBarAirWater1.EnableDocking(CBRS_ALIGN_ANY);
		m_ToolBarAirWater0.EnableDocking(CBRS_ALIGN_ANY);
		m_ToolBarDualWaterPath.EnableDocking(CBRS_ALIGN_ANY);
		m_ToolBarEstopJoySafe.EnableDocking(CBRS_ALIGN_ANY);
		m_ToolBarHt.EnableDocking(CBRS_ALIGN_ANY);
		m_ToolBarLogOnOff.EnableDocking(CBRS_ALIGN_ANY);
		m_ToolBarDualMotion.EnableDocking(CBRS_ALIGN_ANY);
		m_ToolBarTips.EnableDocking(CBRS_ALIGN_ANY);
		m_ToolBarDangerPlanes.EnableDocking(CBRS_ALIGN_ANY);
		m_ToolBarWaterPathProbeLengthTips.EnableDocking(CBRS_ALIGN_ANY);
		EnableDocking(CBRS_ALIGN_ANY);
		if(m_wndMenuBar.GetSafeHwnd()) DockPane(&m_wndMenuBar);
		DockPane(&m_ToolBarAirWater1);
		DockPane(&m_ToolBarAirWater0);
		DockPane(&m_ToolBarDualWaterPath);
		DockPane(&m_ToolBarEstopJoySafe);
		DockPane(&m_ToolBarHt);
		DockPane(&m_ToolBarLogOnOff);
		DockPane(&m_ToolBarDualMotion);
		DockPane(&m_ToolBarTips);
		DockPane(&m_ToolBarDangerPlanes);
		DockPane(&m_ToolBarWaterPathProbeLengthTips);
	}

	PostMessage(UI_UPDATE_DIALOG_BAR, NULL, NULL);
	m_bToolBarsInitialized = TRUE;

}


bool CMainFrame::CreateDockingWindows()
{
	CString strName;


	strName.LoadString(IDS_Status);
	m_wndStatusPane.Create(strName, this, CRect(0, 0, 200, 200), TRUE, ID_STATUS_PANEWND, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI);
	m_wndStatusPane.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndStatusPane);

	strName.LoadString(IDS_Position);
	m_wndPositionPane.Create(strName, this, CRect(0, 0, 200, 200), TRUE, ID_POSITION_PANEWND, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI);
	m_wndPositionPane.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndPositionPane);

	strName.LoadString(IDS_Properties);
	m_wndPropertiesPane.Create(strName, this, CRect(0, 0, 200, 200), TRUE, ID_PROPERTIES_PANEWND, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI);
	m_wndPropertiesPane.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndPropertiesPane);

	strName.LoadString(IDS_Axes);
	m_wndAxesPane.Create(strName, this, CRect(0, 0, 200, 200), TRUE, ID_AXES_PANEWND, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI);
	m_wndAxesPane.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndAxesPane);

	strName.LoadString(IDS_Axes);
	m_wndAxisPane.Create(strName, this, CRect(0, 0, 200, 200), TRUE, ID_AXIS_PANEWND, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI);
	m_wndAxisPane.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndAxisPane);
	int nStyle = m_wndAxisPane.GetStyle();
	m_wndAxisPane.ModifyStyleEx(0, nStyle | EM_SETREADONLY | ES_READONLY);


	strName.LoadString(IDS_Axes_status);
	m_wndAxisStatusPane.Create(strName, this, CRect(0, 0, 200, 200), TRUE, ID_AXES_STATUS_PANEWND, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI);
	m_wndAxisStatusPane.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndAxisStatusPane);
	
	strName.LoadString(IDS_Coordinates);
	m_wndCoordArrayPane.Create(strName, this, CRect(0, 0, 200, 200), TRUE, ID_COORD_ARRAY_PANEWND, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI);
	m_wndCoordArrayPane.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndCoordArrayPane);
	
	strName.LoadString(IDS_World);
	m_wnd3DWorldViewPane.Create(strName, this, CRect(0, 0, 200, 200), TRUE, ID_WORLD_VIEW_PANEWND, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI);
	m_wnd3DWorldViewPane.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wnd3DWorldViewPane);

	strName.LoadString(IDS_Optimize);
	m_wndOptimizePane.Create(strName, this, CRect(0, 0, 200, 200), TRUE, ID_OPTIMIZE_PANEWND, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI);
	m_wndOptimizePane.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOptimizePane);

	return true;
}

void CMainFrame::DockPaneConfineToScreen(CDockablePane* pPane)
{
//	if (pPane->IsFloating()) {
		theApp.ConfineToScreen(&pPane->m_recentDockInfo.m_rectRecentFloatingRect);
		pPane->FloatPane(pPane->m_recentDockInfo.m_rectRecentFloatingRect);
//	}
//	else {
//		pPane->DockToRecentPos();
//	}
		/*
	if (pPane->GetRecentVisibleState()) {
		pPane->ShowPane(true, false, true);
	}
	else {
		pPane->ShowPane(true, false, true);
	}
	*/
}


#define ID_VIEW_APPLICATIONLOOK         34037
#define ID_APPLICATIONLOOK_WINDOWS2000  34038
#define ID_APPLICATIONLOOK_OFFICEXP     34039
#define ID_APPLICATIONLOOK_WINDOWSXP    34040
#define ID_APPLICATIONLOOK_OFFICE2003   34041
#define ID_APPLICATIONLOOK_VISUALSTUDIO2005 34042
#define ID_APPLICATIONLOOK_VISUALSTUDIO2008 34043
#define ID_APPLICATIONLOOK_OFFICE2007   34044
#define ID_OFFICE2007_BLUESTYLE         34045
#define ID_OFFICE2007_BLACKSTYLE        34046
#define ID_OFFICE2007_SILVERSTYLE       34047
#define ID_OFFICE2007_AQUASTYLE         34048

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_APPLICATIONLOOK_WINDOWS2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_APPLICATIONLOOK_OFFICEXP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_APPLICATIONLOOK_WINDOWSXP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_APPLICATIONLOOK_OFFICE2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_APPLICATIONLOOK_VISUALSTUDIO2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_APPLICATIONLOOK_VISUALSTUDIO2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_APPLICATIONLOOK_OFFICE2007:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_OFFICE2007_BLUESTYLE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_OFFICE2007_BLACKSTYLE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_OFFICE2007_SILVERSTYLE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_OFFICE2007_AQUASTYLE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);

		CMFCVisualManager* pVisMan = CMFCVisualManager::GetInstance();
		CMFCToolBarButton* pButton = m_ToolBarAirWater0.GetButton(1);
		theApp.m_rgbToolBar = pVisMan->GetToolbarButtonTextColor(pButton, CMFCVisualManager::ButtonsIsRegular);

	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nCode == AFX_WM_RESETTOOLBAR) {
		ASSERT(0);
	}

	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;



	// otherwise, do default handling
	return CFrameWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::DockBarNextTo(CControlBar *pNewBar, CControlBar *pDockedBar, UINT nDockedBarID)
{
/*
	CFrameWndEx* pFrame = pDockedBar->GetDockingFrame();
	pFrame->RecalcLayout();

	CRect rect;
	pDockedBar->GetWindowRect(&rect);
	rect.OffsetRect(1,1);
	pFrame->DockControlBar(pNewBar,nDockedBarID,&rect);
	*/
}

void CMainFrame::InitializePumpsDialogBar()
{
	CString Buff;

	CMFCToolBarSpinButton::SetRange(IDC_SPIN_PUMP_SPEED_00, 0, 300);
	CMFCToolBarSpinButton::SetRange(IDC_SPIN_PUMP_SPEED_01, 0, 300);

	if (theApp.m_nSide0Orientation == 0) {
		Buff.Format(L"%d", theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed);
		CMFCToolBarEditBoxButton::SetContentsAll(IDC_EDIT_PUMP_SPEED_00, Buff);

		Buff.Format(L"%d", theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed);
		CMFCToolBarEditBoxButton::SetContentsAll(IDC_EDIT_PUMP_SPEED_01, Buff);

		CMFCToolBarSpinButton::SetPos(IDC_SPIN_PUMP_SPEED_00, (int)theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed);
		CMFCToolBarSpinButton::SetPos(IDC_SPIN_PUMP_SPEED_01, (int)theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed);
	}
	else {
		Buff.Format(L"%d", theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed);
		CMFCToolBarEditBoxButton::SetContentsAll(IDC_EDIT_PUMP_SPEED_00, Buff);

		Buff.Format(L"%d", theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed);
		CMFCToolBarEditBoxButton::SetContentsAll(IDC_EDIT_PUMP_SPEED_01, Buff);

		CMFCToolBarSpinButton::SetPos(IDC_SPIN_PUMP_SPEED_00, (int)theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed);
		CMFCToolBarSpinButton::SetPos(IDC_SPIN_PUMP_SPEED_01, (int)theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed);
	}

}

HRESULT CMainFrame::UpdatePumpsDialogBar(WPARAM,LPARAM)
{
	InitializePumpsDialogBar();
	return NULL;
}


HRESULT CMainFrame::InitializeDialogBar(WPARAM,LPARAM)
{
	CString Buff;

	int side0, side1;
	if (theApp.m_nSide0Orientation == 0) {
		side0 = PORTSIDE;
		side1 = STARBOARD;
	}
	else {
		side0 = STARBOARD;
		side1 = PORTSIDE;
	}

	const int wpMin(0), wpMax(200);
	CMFCToolBarSpinButton::SetRange(ID_SPIN_LEFT_WATER_PATH, wpMin, wpMax);
	CMFCToolBarSpinButton::SetRange(ID_SPIN_RIGHT_WATER_PATH, wpMin, wpMax);
	CMFCToolBarSpinButton::SetRange(ID_SPIN_THICKNESS, wpMin, wpMax);
	CMFCToolBarSpinButton::SetRange(ID_SPIN_LEFT_PROBE_LENGTH, wpMin, wpMax);

	Buff.Format((L"%.*f%s"),theApp.m_nWpPrecision,theApp.m_Kinematics.m_fDesiredWaterPath[side0]*theApp.m_fUnits,theApp.m_Units);
	CMFCToolBarEditBoxButton::SetContentsAll(ID_EDIT_LEFT_WATER_PATH, Buff);

	Buff.Format((L"%.*f%s"),theApp.m_nWpPrecision,theApp.m_Kinematics.m_fDesiredWaterPath[side1]*theApp.m_fUnits,theApp.m_Units);
	CMFCToolBarEditBoxButton::SetContentsAll(ID_EDIT_RIGHT_WATER_PATH, Buff);

	Buff.Format((L"%.*f%s"), theApp.m_nUnitsPrecision, theApp.m_Kinematics.m_fComponentThickness*theApp.m_fUnits, theApp.m_Units);
	CMFCToolBarEditBoxButton::SetContentsAll(ID_EDIT_THICKNESS, Buff);

	Buff.Format((L"%.*f%s"), theApp.m_nUnitsPrecision, theApp.m_Kinematics.m_fProbeLength[0]*theApp.m_fUnits, theApp.m_Units);
	CMFCToolBarEditBoxButton::SetContentsAll(ID_EDIT_LEFT_PROBE_LENGTH, Buff);

	CMFCToolBarSpinButton::SetPos(ID_SPIN_LEFT_WATER_PATH, (int)theApp.m_Kinematics.m_fDesiredWaterPath[side0]);
	CMFCToolBarSpinButton::SetPos(ID_SPIN_RIGHT_WATER_PATH, (int)theApp.m_Kinematics.m_fDesiredWaterPath[side1]);
	CMFCToolBarSpinButton::SetPos(ID_SPIN_THICKNESS, (int)theApp.m_Kinematics.m_fComponentThickness);
	CMFCToolBarSpinButton::SetPos(ID_SPIN_LEFT_PROBE_LENGTH, (int)theApp.m_Kinematics.m_fProbeLength[0]);

	return NULL;
}



void CMainFrame::OpenOnLastExit()
{
	if(!(theApp.m_nLicenseMask & _MINIMAL)) {
		for(int nn=0;nn<8;nn++) {
			if(theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_OSCILLOSCOPE+nn]==TRUE) OnUltrasonicsOscilloscope();
		}

		if(theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_WORKSPACE_SHEET % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnViewWorkspace();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_ULTRASONICS_SHEET % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnUltrasonicsPulserreceiver();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_MOTIONTOOLS_SHEET % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnMotionTools();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_PMAC_SHEET % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnSystemPmac();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_AXES_SHEET % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnSystemAxes();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_STEPPER_SHEET % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnSystemHoldingfixture();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_COMPENSATION_SHEET % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnSystemKinematics();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_SCAN_SHEET % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnScanScan();
	//	if(theApp.m_LastSettings.bWindowOpenOnLastExit[CEditDetailsDlg::IDD % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnSystemEditscandetails();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_PALETTE_SHEET % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnPaletteEdit();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_PROFILE_SHEET % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnProfilesEdit();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_FILTER_SHEET % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnUltrasonicsFilters();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[CIGESDlg::IDD % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnProfilesIgesfileimport();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_ROBOT_SHEET % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnSystemRobots();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_MAINTENANCE_DIAGNOSTICS_SHEET % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnSystemMaintenancediagnostics();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[CLSABScanDlg::IDD % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnUltrasonicsLsabscan();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[CPhasedArrayProbeDlg::IDD % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnUltrasonicsPhasedarrayprobe();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[CViewBladeThickneessDlg::IDD % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnViewBlade();
		if(theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_INCLUSIONS_SHEET % MAXIMUMWINDOWSIDNUMBER]==TRUE) OnUltrasonicsInclusions();
	}

//	CString strTitle = _T("NormalizeDlg");
//	UINT style = WS_CHILD | CBRS_RIGHT | CBRS_FLOAT_MULTI;

//	m_NormalizeDlg.Create(strTitle, this, CRect(0, 0, 300, 400), TRUE,
//		IDD_NORMALIZE_PAGE, style, AFX_CBRS_OUTLOOK_TABS);
//	m_NormalizeDlg.EnableDocking(CBRS_ALIGN_ANY);
	//	m_pNormalizeDlg->SetWindowPos(NULL, 100, 100, 200, 200, NULL);


}


HRESULT CMainFrame::LanguageChanged(WPARAM,LPARAM)
{

	CMenu* pNewMenu = new CMenu;
	pNewMenu->LoadMenu(IDR_MAINFRAME);
	SetMenu(pNewMenu);
	delete(pNewMenu);

	if(	m_pPaletteSheet ) {
		m_pPaletteSheet->SendMessage(WM_CLOSE);
		OnPaletteEdit();
	}

	if(m_pUltrasonicsSheet ) {
		m_pUltrasonicsSheet->SendMessage(WM_CLOSE);
		OnUltrasonicsPulserreceiver();
	}
	if( m_pMotionToolsSheet ) {
		m_pMotionToolsSheet->SendMessage(WM_CLOSE);
		OnMotionTools();
	}
	if( m_pProfileSheet ) {
		m_pProfileSheet->SendMessage(WM_CLOSE);
		OnProfilesEdit();
	}
	if( m_pCompensationSheet ) {
		m_pCompensationSheet->SendMessage(WM_CLOSE);
		OnMotionKinematics();
	}

/*
	m_pStepperSheet=NULL;
	m_pSerialJoystickDlg=NULL;
	m_pPmacSheet = NULL;
	m_pPositionSheet = NULL;
	m_pScanSheet=NULL;
	m_pSpectrumAnalyzerDlg=NULL;
	m_pEditDetailsDlg=NULL;
	m_pUsersDlg=NULL;
	m_pAccessLevelsDlg=NULL;
	m_pIGESDlg=NULL;
	m_pDataBaseDlg=NULL;
	m_pBMPViewerDlg=NULL;
	m_pSafetyInterlocksDlg=NULL;
	m_pFilterSheet = NULL;
	m_pLSABScanDlg=NULL;
	m_pDetailsPage=NULL;
	m_pProbeCharacterisationSheet=NULL;
	m_pCustomizeSheet=NULL;
	m_pArchiveSheet=NULL;
	m_pMaintenanceDiagnosticsSheet = NULL;
	m_pImportToolPathsDlg = NULL;
	m_pMultipleBScansDlg = NULL;
	m_pWorkSpaceSheet = NULL;
	m_pAssignAxesDlg = NULL;
	m_pRobotSheet = NULL;
	m_pFaroCoordinateDlg = NULL;
	m_pDataPropertiesDlg = NULL;
	m_pPolyCoordDlg = NULL;
	m_pInclusionsSheet = NULL;
	m_pQuaternionsPage = NULL;
*/
	return NULL;
}

void CMainFrame::OnSystemCards()
{
	CCardsDlg	dlg;

	dlg.DoModal();

}

void CMainFrame::OnSystemOptions()
{
	COptionsDlg	dlg;

	dlg.DoModal();

}


void CMainFrame::OnSystemHoldingfixture()
{


	if (m_pStepperSheet == NULL) {
		m_pStepperSheet = new CStepperSheet((L"Axes"), this, 0, (CWnd**)&m_pStepperSheet);
		m_pStepperSheet->Create(this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_STEPPER_SHEET % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pAxesSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}

void CMainFrame::OnProfilesImport()
{

	if(PROFILE->CheckNewProfileAllowed()!=TRUE) return;

	CImportRulesDlg dlg;

	dlg.DoModal();

}

void CMainFrame::OnProfilesEdit()
{

	CString Buff;

	Buff.LoadString(IDS_Profile);

	if (m_pProfileSheet == NULL) {
		m_pProfileSheet = new CProfileSheet(Buff,this,0,(CWnd**)&m_pProfileSheet);
		if(m_pProfileSheet->Create( this)){
			theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_PROFILE_SHEET % MAXIMUMWINDOWSIDNUMBER]=TRUE;
		}else{
			m_pProfileSheet=NULL;
		}
	} else {
		m_pProfileSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	
/*	
	if (m_p3DViewEditDlg == NULL) {
		m_p3DViewEditDlg = new C3DViewEditDlg(this,(CWnd**)&m_p3DViewEditDlg);
		m_p3DViewEditDlg->Create(C3DViewEditDlg::IDD, this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[C3DViewEditDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_p3DViewEditDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
*/
}




void CMainFrame::OnProfilesOpen()
{

	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;

	if(PROFILE->CheckNewProfileAllowed()!=TRUE) return;

	CFileThumbNailDlg FileDlg(TRUE,(L".pro"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,(L"Profile (*.pro)|*.pro|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_ProfilePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_ProfilePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,(L"Error"),MB_ICONERROR);
		} else {
			CWaitCursor Wait;
			PROFILE->Retrieve(&File);
			if(theApp.m_LastSettings.nLoadUltrasonicsFromProfile==1) {
				PROFILE->CopyLocalUtToGlobal();
			}
			File.Close();
			ProfileRetrievedUpdateDialogs((WPARAM)this,NULL);
			Wait.Restore();
			theApp.ActivityLog(L"Profile loaded : " + FilePath);

		}
	}
}

void CMainFrame::OnProfilesSaveas()
{

	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = PROFILE->m_FileName;
	if((nPos = PROFILE->m_FileName.ReverseFind('.')) >0) {
		InitialFile = PROFILE->m_FileName.Left(nPos);
	}

	CFileDialog FileDlg(FALSE,(L".pro"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,(L"Profile (*.pro)|*.pro|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_ProfilePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_ProfilePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,(L"Error"),MB_ICONERROR);
		} else {
			PROFILE->Save(&File);
			File.Close();
		}
	}


}



void CMainFrame::OnPaletteOpen()
{

	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;

	CFileDialog FileDlg(TRUE,(L".pal"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,(L"Palette (*.pal)|*.pal|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_PalettePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_PalettePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,(L"Error"),MB_ICONERROR);
		} else {
			theApp.m_Palette.Retrieve(&File);
			File.Close();
			if(m_pPaletteSheet) m_pPaletteSheet->UpdateAllPages();
		}
	}
}

void CMainFrame::OnPaletteSaveas()
{

	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = theApp.m_Palette.m_FileName;
	if((nPos = PROFILE->m_FileName.ReverseFind('.')) >0) {
		InitialFile = PROFILE->m_FileName.Left(nPos);
	}

	CFileDialog FileDlg(FALSE,(L".pal"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,(L"Palette (*.pal)|*.pal|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_PalettePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_PalettePath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,(L"Error"),MB_ICONERROR);
		} else {
			theApp.m_Palette.Save(&File);
			File.Close();
		}
	}
}

void CMainFrame::OnPaletteEdit()
{
	PaletteEdit();
}

void CMainFrame::PaletteEdit()
{
	if (m_pPaletteSheet == NULL) {
		m_pPaletteSheet = new CPaletteSheet((L"Palette"), this, 0, (CWnd**)&m_pPaletteSheet);
		if (m_pPaletteSheet->Create(this)) {
			theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_PALETTE_SHEET % MAXIMUMWINDOWSIDNUMBER] = TRUE;
		}
		else {
			m_pPaletteSheet = NULL;
		}
	}
	else {
		m_pPaletteSheet->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}
void CMainFrame::CallProtectedPaletteEdit()
{
	OnPaletteEdit();
}

void CMainFrame::OnUpdatePaletteEdit(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}



void CMainFrame::OnUltrasonicsLsabscan()
{
 
 	if (m_pLSABScanDlg == NULL) {
 		m_pLSABScanDlg = new CLSABScanDlg(this,(CWnd**)&m_pLSABScanDlg);
 		m_pLSABScanDlg->Create(CLSABScanDlg::IDD, this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[CLSABScanDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
 	} else {
 		m_pLSABScanDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
 	}
}

void CMainFrame::OnFileOpen()
{

	CString FilePath,Buff;
	int	nPos;

	CFileThumbNailDlg FileDlg(TRUE,(L".dat"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,(L"Image (*.dat)|*.dat|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath[0];
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_StorePath[0] = FilePath.Left(nPos);
			}
		}

		for(int nn=0;nn<MAX_NUMBER_VIEWS;nn++) {
			if(m_pViewSheet[nn]==NULL) {
				m_pViewSheet[nn] = new CViewSheet(FilePath,this,0,(CWnd**)&m_pViewSheet[nn],FilePath);
				if(m_pViewSheet[nn]->m_bCompatibleFile == true) {
					m_pViewSheet[nn]->Create(this);
					AfxGetApp()->AddToRecentFileList(FilePath);

				} else {
					SAFE_DELETE( m_pViewSheet[nn] );
				};
				return;
			};
		};
		MessageBox(NULL,(L"Too Many Views Allocated"),NULL);

	}

}

void CMainFrame::OnFileOpenBScan()
{

	CString	FileName;

	CFileDialog File(TRUE,(L".vol"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,(L"Volume (*.vol)|*.vol|BMP (*.bmp)|*.bmp|All Files (*.*)|*.*|"),NULL);
	File.m_ofn.lpstrInitialDir = theApp.m_StorePath[0];
	if(File.DoModal()==IDOK) {


		FileName.Format((L"%s"),File.GetPathName());
		if (m_pMultipleBScansDlg != NULL) {
			m_pMultipleBScansDlg->SendMessage(WM_CLOSE);
			theApp.m_LastSettings.bWindowOpenOnLastExit[CVolumeViewDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=FALSE;
		}
		m_pMultipleBScansDlg = new CMultipleBScansDlg(this,(CWnd**)&m_pMultipleBScansDlg,FileName,REPLAY);
		m_pMultipleBScansDlg->Create(CMultipleBScansDlg::IDD, this);	

		theApp.m_LastSettings.bWindowOpenOnLastExit[CMultipleBScansDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	};	

}

void CMainFrame::OnUpdateFileOpenBScan(CCmdUI* pCmdUI)
{
	//if not Dowty dont show
	//TODO


}

void CMainFrame::FileView(CString &FilePathName)
{
	int nn;

	for(nn=0;nn<MAX_NUMBER_VIEWS;nn++) {
		if(m_pViewSheet[nn]==NULL) {
			m_pViewSheet[nn] = new CViewSheet(FilePathName,this,0,(CWnd**)&m_pViewSheet[nn],FilePathName);
			m_pViewSheet[nn]->Create(this);
			AfxGetApp()->AddToRecentFileList(FilePathName);
			return;
		};
	};
	MessageBox(NULL,(L"Too Many Views Allocated"),NULL);

}


void CMainFrame::OnMotionTools()
{

	CString Buff;

	if (m_pMotionToolsSheet == NULL) {
		Buff.LoadString(IDS_Motion_Tools);
		m_pMotionToolsSheet = new CMotionToolsSheet(Buff,this,0,(CWnd**)&m_pMotionToolsSheet);
		if(m_pMotionToolsSheet->Create( this)) {
			theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_MOTIONTOOLS_SHEET % MAXIMUMWINDOWSIDNUMBER]=TRUE;
		}else{
			m_pMotionToolsSheet=NULL;
		}
	} else {
		m_pMotionToolsSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}

HRESULT CMainFrame::RefreshMotionTools(WPARAM, LPARAM)
{
	CString Buff;

	if (m_pMotionToolsSheet) {
		SAFE_DELETE(m_pMotionToolsSheet);

		Buff.LoadString(IDS_Motion_Tools);
		m_pMotionToolsSheet = new CMotionToolsSheet(Buff, this, 0, (CWnd**)&m_pMotionToolsSheet);
		m_pMotionToolsSheet->Create(this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_MOTIONTOOLS_SHEET % MAXIMUMWINDOWSIDNUMBER] = TRUE;
	}
	return NULL;
}

HRESULT CMainFrame::RefreshUltrasonicsSheet(WPARAM, LPARAM)
{
	CString Buff;

	if (m_pUltrasonicsSheet) {
		SAFE_DELETE(m_pUltrasonicsSheet);

		Buff.LoadString(IDS_Ultrasonics);
		m_pUltrasonicsSheet = new CUltrasonicsSheet(Buff, this, 0, (CWnd**)&m_pUltrasonicsSheet);
		m_pUltrasonicsSheet->Create(this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_ULTRASONICS_SHEET % MAXIMUMWINDOWSIDNUMBER] = TRUE;
	}
	return NULL;
}



void CMainFrame::OnSystemPmac()
{
	if (m_pPmacSheet == NULL) {
		m_pPmacSheet = new CPmacSheet(theApp.m_PmacUser.m_strPmacType,this,0,(CWnd**)&m_pPmacSheet);
		m_pPmacSheet->Create( this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_PMAC_SHEET % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pPmacSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}




void CMainFrame::OnButtonAbort()
{
	theApp.m_Thread.m_bAbortFlag = TRUE;

	AndonEventType AndonFlag = MachineWarning;
	theApp.AndonEvent(AndonFlag,L"Abort button pressed in Mainframe");

	theApp.StopLogging();
	theApp.m_bCollectBladeData=FALSE;
	theApp.m_bCollectComplexData=FALSE;
	theApp.m_bCollectNormalizeData=FALSE;
	theApp.m_bCollectTunning=FALSE;
	theApp.m_bCollectEccentricData=FALSE;
	theApp.m_bLaserCollectTaughtLines=FALSE;
	theApp.m_Si10User.m_Si10.bCollectData = FALSE;


	theApp.m_FBCtrl.KillAllMotionPrograms();
	Sleep(100);
	theApp.m_FBCtrl.KillMotors();
	theApp.m_Motors.StopContinuousRotate(theApp.m_Tank.nRLeft, false);

	theApp.m_PmacUser.AbortRotateThetaJoystick();

	theApp.m_Thread.StopCurrentThreadAction();
	StopCurrentTimerAction();

}

void CMainFrame::OnButtonWaterPath()
{

	CCoord CpSurface,CpHead;

	theApp.m_Motors.GetHeadPos(&CpHead);
	if(theApp.m_nSide0Orientation == 0) {
		theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
	} else {
		theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,FALSE);
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
	}
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = CpHead;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
}

void CMainFrame::OnButtonJoystick()
{


	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
}

void CMainFrame::OnButtonSafe()
{


	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nLadderExecuteSide=3;
		theApp.m_Thread.m_nLadder = 0;
		theApp.m_Thread.m_nThreadAction=EXECUTE_LADDER;
		break;
	case SINGLE_ROBOT:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nLadderExecuteSide=1;
		theApp.m_Thread.m_nLadder = 0;
		theApp.m_Thread.m_nThreadAction=EXECUTE_LADDER;
		break;
	case TRACKED_PROBE:
		break;
	}
}

void CMainFrame::OnButtonPark() 
{


	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nLadderExecuteSide=3;
		theApp.m_Thread.m_nLadder = 0;
		theApp.m_Thread.m_nThreadAction=EXECUTE_LADDER;
		break;
	case SINGLE_ROBOT:
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nLadderExecuteSide=1;
		theApp.m_Thread.m_nLadder = 0;
		theApp.m_Thread.m_nThreadAction=EXECUTE_LADDER;
		theApp.m_FBCtrl.RRWaterSpray(0);
		break;
	case TRACKED_PROBE:
		break;
	}
}


void CMainFrame::OnSystemKinematics()
{

	CString Buff;

	if (m_pCompensationSheet == NULL) {
		Buff.LoadString(IDS_Compensation_Kinematics);
		m_pCompensationSheet = new CCompensationSheet(Buff,this,0,(CWnd**)&m_pCompensationSheet);
		m_pCompensationSheet->Create( this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_COMPENSATION_SHEET % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pCompensationSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}


void CMainFrame::InvalidateWaterPaths()
{

	CString	Buff;
	int nIDEdit,nIDSpin,nSide;
	CEdit* pEdit;

//	if(m_pToolBarManipulator == NULL) return;

	nIDEdit = IDC_EDIT_LEFT_WATERPATH;
	nIDSpin = IDC_SPIN_LEFT_WATERPATH;

	if(theApp.m_nSide0Orientation==0) {
		nSide = PORTSIDE;
	} else {
		nSide = STARBOARD;
	}

	pEdit = (CEdit* ) m_ToolBarDualWaterPath.GetDlgItem(nIDEdit);
	if(pEdit) {
		Buff.Format((L"%.*f%s"),theApp.m_nWpPrecision,theApp.m_Kinematics.m_fDesiredWaterPath[nSide]*theApp.m_fUnits,theApp.m_Units);
		pEdit->SetWindowText(Buff);

		CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)m_ToolBarDualWaterPath.GetDlgItem(nIDSpin);
		pSpin->SetPos((int)theApp.m_Kinematics.m_fDesiredWaterPath[nSide]);
	}

	nIDEdit = IDC_EDIT_RIGHT_WATERPATH;
	nIDSpin = IDC_SPIN_RIGHT_WATERPATH;

	if(theApp.m_nSide0Orientation==0) {
		nSide = STARBOARD;
	} else {
		nSide = PORTSIDE;
	}

	pEdit = (CEdit* )m_ToolBarDualWaterPath.GetDlgItem(nIDEdit);
	if(pEdit) {
		Buff.Format((L"%.*f%s"),theApp.m_nWpPrecision,theApp.m_Kinematics.m_fDesiredWaterPath[nSide]*theApp.m_fUnits,theApp.m_Units);
		pEdit->SetWindowText(Buff);

		CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)m_ToolBarDualWaterPath.GetDlgItem(nIDSpin);
		pSpin->SetPos((int)theApp.m_Kinematics.m_fDesiredWaterPath[nSide]);
	}
}

void CMainFrame::OnChangeEditLeftWaterPath()
{
	if (!m_bToolBarsInitialized) return;

	CString	Buff;
	int nIDEdit,nIDSpin,nSide;
	float fOldWp;

//	if(m_pToolBarManipulator == NULL ) return;

	nIDEdit = ID_EDIT_LEFT_WATER_PATH;
	nIDSpin = ID_SPIN_LEFT_WATER_PATH;

	if(theApp.m_nSide0Orientation==0) {
		nSide = PORTSIDE;
	} else {
		nSide = STARBOARD;
	}
	fOldWp = theApp.m_Kinematics.m_fDesiredWaterPath[nSide];

	CEdit* pEdit = (CEdit*)m_ToolBarDualWaterPath.GetDlgItem(nIDEdit);
	if (pEdit) {
		Buff = CMFCToolBarEditBoxButton::GetContentsAll(nIDEdit);

		_WTOF(Buff, theApp.m_Kinematics.m_fDesiredWaterPath[nSide]);
		theApp.m_Kinematics.m_fDesiredWaterPath[nSide] /= theApp.m_fUnits;

		CMFCToolBarSpinButton::SetPos(nIDSpin, (int)theApp.m_Kinematics.m_fDesiredWaterPath[nSide]);

		switch (theApp.m_Tank.nScannerDescription) {
		default:
			break;
		case SINGLE_ROBOT:
		case DUAL_ROBOT:
		case DUAL_ROBOT_9_PLUS_9:
		case SPX_ROBOT:
		case TWIN_TOWER_KINEMATIC:
			if (fOldWp - theApp.m_Kinematics.m_fDesiredWaterPath[nSide]) {
				if (theApp.m_Thread.IsJoystickRunning()) {
					theApp.m_Thread.StopCurrentThreadAction();
					theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(nSide, true);
					theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
				}
				else {
					theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(nSide, false);
				}
			}
			break;
		case TRACKED_PROBE:
			break;
		}
	}

	pEdit = (CEdit*)m_ToolBarWaterPathProbeLengthTips.GetDlgItem(nIDEdit);
	if (pEdit) {
		Buff = CMFCToolBarEditBoxButton::GetContentsAll(nIDEdit);

		_WTOF(Buff, theApp.m_Kinematics.m_fDesiredWaterPath[nSide]);
		theApp.m_Kinematics.m_fDesiredWaterPath[nSide] /= theApp.m_fUnits;

		CMFCToolBarSpinButton::SetPos(nIDSpin, (int)theApp.m_Kinematics.m_fDesiredWaterPath[nSide]);

		switch (theApp.m_Tank.nScannerDescription) {
		default:
			break;
		case SINGLE_ROBOT:
		case DUAL_ROBOT:
		case DUAL_ROBOT_9_PLUS_9:
		case SPX_ROBOT:
		case TWIN_TOWER_KINEMATIC:
			if (fOldWp - theApp.m_Kinematics.m_fDesiredWaterPath[nSide]) {
				if (theApp.m_Thread.IsJoystickRunning()) {
					theApp.m_Thread.StopCurrentThreadAction();
					theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(nSide, true);
					theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
				}
				else {
					theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(nSide, false);
				}
			}
			break;
		case TRACKED_PROBE:
			break;
		}
	}
}

void CMainFrame::OnChangeEditRightWaterPath()
{
	if (!m_bToolBarsInitialized) return;


	CString	Buff;
	int nIDEdit,nIDSpin,nSide;
	float fOldWp;

//	if(m_pToolBarManipulator == NULL) return;

	nIDEdit = ID_EDIT_RIGHT_WATER_PATH;
	nIDSpin = ID_SPIN_RIGHT_WATER_PATH;

	if(theApp.m_nSide0Orientation==0) {
		nSide = STARBOARD;
	} else {
		nSide = PORTSIDE;
	}
	fOldWp = theApp.m_Kinematics.m_fDesiredWaterPath[nSide];

	CEdit* pEdit = (CEdit* )m_ToolBarDualWaterPath.GetDlgItem(nIDEdit);
	if(pEdit) {
		Buff = CMFCToolBarEditBoxButton::GetContentsAll(nIDEdit);

		_WTOF(Buff,theApp.m_Kinematics.m_fDesiredWaterPath[nSide]);
		theApp.m_Kinematics.m_fDesiredWaterPath[nSide] /= theApp.m_fUnits;

		CMFCToolBarSpinButton::SetPos(nIDSpin, (int)theApp.m_Kinematics.m_fDesiredWaterPath[nSide]);
		
		switch(theApp.m_Tank.nScannerDescription) {
		default:
			break;
		case SINGLE_ROBOT:
		case DUAL_ROBOT:
		case DUAL_ROBOT_9_PLUS_9:
		case SPX_ROBOT:
		case TWIN_TOWER_KINEMATIC:
			if(fOldWp - theApp.m_Kinematics.m_fDesiredWaterPath[nSide]) {
				if(theApp.m_Thread.IsJoystickRunning()) {
					theApp.m_Thread.StopCurrentThreadAction();
					theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(nSide,true);
					theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
				} else {
					theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(nSide,false);
				}
			}
			break;
		case TRACKED_PROBE:
			break;
		}
	}

}

void CMainFrame::OnChangeEditThickness()
{
	if (!m_bToolBarsInitialized) return;

	CString	Buff;
	int nIDEdit, nIDSpin;

	//	if(m_pToolBarManipulator == NULL) return;

	nIDEdit = ID_EDIT_THICKNESS;
	nIDSpin = ID_SPIN_THICKNESS;

	CEdit* pEdit = (CEdit*)m_ToolBarDualWaterPath.GetDlgItem(nIDEdit);
	if (pEdit) {
		Buff = CMFCToolBarEditBoxButton::GetContentsAll(nIDEdit);

		_WTOF(Buff, theApp.m_Kinematics.m_fComponentThickness);
		theApp.m_Kinematics.m_fComponentThickness /= theApp.m_fUnits;

		CMFCToolBarSpinButton::SetPos(nIDSpin, (int)theApp.m_Kinematics.m_fComponentThickness);
	}
}

void CMainFrame::OnChangeEditProbeLength()
{
	if (!m_bToolBarsInitialized) return;

	CString	Buff;
	int nIDEdit, nIDSpin;

	return;
	//	if(m_pToolBarManipulator == NULL) return;

	nIDEdit = ID_EDIT_LEFT_PROBE_LENGTH;
	nIDSpin = ID_SPIN_LEFT_PROBE_LENGTH;

	CEdit* pEdit = (CEdit*)m_ToolBarWaterPathProbeLengthTips.GetDlgItem(nIDEdit);
	if (pEdit) {
		Buff = CMFCToolBarEditBoxButton::GetContentsAll(nIDEdit);

		_WTOF(Buff, theApp.m_Kinematics.m_fProbeLength[0]);
		theApp.m_Kinematics.m_fProbeLength[0] /= theApp.m_fUnits;

		CMFCToolBarSpinButton::SetPos(nIDSpin, (int)theApp.m_Kinematics.m_fProbeLength[0]);
	}
}

void CMainFrame::OnUltrasonicsTileoscilloscopesvertically()
{
	CRect rrDlg[8];
	int	nTS,nTotal, nHeight , nTop;
	CRect rr;
	CRect rrScreen,rrStatus,rrClient;
	bool bScopesInActiveWindow = false;

	GetClientRect(rrClient);
	ClientToScreen(rrClient);
	GetWindowRect(rrScreen);
	m_wndStatusBar.GetWindowRect(rrStatus);
	rrClient.bottom = rrStatus.top;

	rr.SetRectEmpty();
	rr.top=2000;
	rr.left=3000;

	nTotal = 0;
	for(nTS=0;nTS<8;nTS++) {
		if(m_pScopeDlg[nTS]) {
			nTotal++;
			m_pScopeDlg[nTS]->GetWindowRect(rrDlg[nTS]);

			if(rrDlg[nTS].left < rr.left) rr.left = rrDlg[nTS].left;
			if(rrDlg[nTS].top < rr.top) rr.top = rrDlg[nTS].top;
			if(rrDlg[nTS].right > rr.right) rr.right = rrDlg[nTS].right;
			if(rrDlg[nTS].bottom > rr.bottom) rr.bottom = rrDlg[nTS].bottom;
			if (bScopesInActiveWindow == false) {
				if (rrScreen.PtInRect(rrDlg[nTS].TopLeft()) || rrScreen.PtInRect(rrDlg[nTS].BottomRight())) bScopesInActiveWindow = true;
			}
		}
	}
	int nHPixel = GetSystemMetrics(78);	
	int nVPixel = GetSystemMetrics(17);	//Not 79, 17
	if (bScopesInActiveWindow == false) {
		nHeight = nVPixel;
		nTop = 0;
	} else {
		nHeight = rrClient.Height();
		nTop = rrClient.top;
	}

	for(nTS=0;nTS<8;nTS++) {
		if(m_pScopeDlg[nTS]) {
			int nY = nTop+MulDiv(nTS,nHeight,nTotal);
			m_pScopeDlg[nTS]->SetWindowPos(&wndTop,rr.left,nY,rr.Width(), nHeight /nTotal,NULL);
		}
	}

}

void CMainFrame::OnUltrasonicsTileoscilloscopeshorizontally()
{
	CRect rrDlg[8];
	int	nTS,nTotal;
	CRect rr;

	rr.SetRectEmpty();
	rr.top=2000;
	rr.left=3000;

	nTotal = 0;
	for(nTS=0;nTS<8;nTS++) {
		if(m_pScopeDlg[nTS]) {
			nTotal++;
			m_pScopeDlg[nTS]->GetWindowRect(rrDlg[nTS]);

			if(rrDlg[nTS].left < rr.left) rr.left = rrDlg[nTS].left;
			if(rrDlg[nTS].top < rr.top) rr.top = rrDlg[nTS].top;
			if(rrDlg[nTS].right > rr.right) rr.right = rrDlg[nTS].right;
			if(rrDlg[nTS].bottom > rr.bottom) rr.bottom = rrDlg[nTS].bottom;
		}
	}
	for(nTS=0;nTS<8;nTS++) {
		if(m_pScopeDlg[nTS]) {
			m_pScopeDlg[nTS]->SetWindowPos(&wndTop,rr.left+MulDiv(nTS,rr.Width(),nTotal),rr.top,rr.Width()/nTotal,rr.Height(),NULL);
		}
	}
}


void CMainFrame::OnUltrasonicsOscilloscopepositionsShiftallleft()
{
	int	nTS;
	CRect rrDlg[8];

	for(nTS=0;nTS<8;nTS++) {
		if(m_pScopeDlg[nTS]) {
			m_pScopeDlg[nTS]->GetWindowRect(rrDlg[nTS]);
			m_pScopeDlg[nTS]->SetWindowPos(&wndTop,0,rrDlg[nTS].top,rrDlg[nTS].Width(),rrDlg[nTS].Height(),NULL);
		}
	}
}
void CMainFrame::OnUltrasonicsOscilloscopepositionsShiftallright()
{

	int	nTS;
	CRect rrDlg[8];

	int nHPixel = GetSystemMetrics(78);

	for(nTS=0;nTS<8;nTS++) {
		if(m_pScopeDlg[nTS]) {
			m_pScopeDlg[nTS]->GetWindowRect(rrDlg[nTS]);
			m_pScopeDlg[nTS]->SetWindowPos(&wndTop,nHPixel-rrDlg[nTS].Width(),rrDlg[nTS].top,rrDlg[nTS].Width(),rrDlg[nTS].Height(),NULL);
		}
	}
}







void CMainFrame::OnMainfrmMoverighttofindleft()
{

	CCoord CpHead,CpSurface;

	theApp.m_Motors.GetHeadPos(&CpHead);
	theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
	CpSurface.CopySides(STARBOARD,PORTSIDE);
	theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = CpHead;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;

}

void CMainFrame::OnMainfrmMovelefttofindright()
{

	CCoord CpHead,CpSurface;

	theApp.m_Motors.GetHeadPos(&CpHead);
	theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
	CpSurface.CopySides(PORTSIDE,STARBOARD);
	theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = CpHead;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
}

void CMainFrame::OnScanScan()
{


	if (m_pScanSheet == NULL) {
		m_pScanSheet = new CScanSheet((L"Scan"),this,0,(CWnd**)&m_pScanSheet);
		m_pScanSheet->Create( this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_SCAN_SHEET % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pScanSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}


void CMainFrame::StartBladeScan(int nScanType, int nScanSide, int nAction)
{

	int	nLine;
	CTime CurrentTime = CTime::GetCurrentTime();


	if((m_pScanViewBladeThicknessDlg != m_pViewBladeThicknessDlg[0]) || (m_pScanViewBladeThicknessDlg == 0)) {

		if(m_pViewBladeThicknessDlg[0]) m_pViewBladeThicknessDlg[0]->SendMessage(WM_CLOSE);
		m_pViewBladeThicknessDlg[0] = new CViewBladeThickneessDlg(this,(CWnd**)&m_pViewBladeThicknessDlg[0]);

		m_pScanViewBladeThicknessDlg = m_pViewBladeThicknessDlg[0];
		m_pScanViewBladeThicknessDlg->InitializeFromProfile(nScanType);

		switch(theApp.m_Thread.m_nBladeSide) {
		case OUTSIDE:	PROFILE->CopyAllLines(SCANLINES, OUTSIDELINES);
			PROFILE->CopyAllLines(BRAINLINES, BRANEOUTSIDELINES);
			break;
		case INSIDE:	PROFILE->CopyAllLines(SCANLINES, INSIDELINES);
			PROFILE->CopyAllLines(BRAINLINES, BRANEINSIDELINES);
		break;
		}

	//	m_pScanViewBladeThicknessDlg->InitializeFromUt();
		theApp.m_pScanData = &m_pScanViewBladeThicknessDlg->m_Data;
		theApp.m_pScanData->m_FileName = theApp.m_Data.m_FileName;
		theApp.m_pScanData->m_FilePath = theApp.m_Data.m_FilePath;
		theApp.m_pScanData->m_nNumberProbesPerImage = 1;
		theApp.m_pScanData->m_ProfileName = PROFILE->m_FileName;
		theApp.m_pScanData->m_TestDateTime = CurrentTime.Format((L"%d %b %Y %Hh%M"));
		theApp.m_pScanData->m_osTestDateTime=(int)CurrentTime.GetTime();
		theApp.m_pScanData->m_Operator = theApp.m_Operator;
		m_pScanViewBladeThicknessDlg->Create(CViewBladeThickneessDlg::IDD, this);
	} else {

	}

	theApp.m_Thread.m_nBladeSide = nScanSide;
	switch(nAction) {
	case SCAN_ALL:
		for(nLine=0;nLine<PROFILE->m_nLineBladeL[theApp.m_Thread.m_nBladeSide];nLine++) {
			PROFILE->m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].m_nUseToScan = 1;
		};
		break;
	case SCAN_CURRENT_LINE:
		for(nLine=0;nLine<PROFILE->m_nLineBladeL[theApp.m_Thread.m_nBladeSide];nLine++) {
			PROFILE->m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].m_nUseToScan = 0;
		};
		PROFILE->m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_nUseToScan = 1;
		break;
	case SCAN_REMAINDER:
		theApp.m_nBladeLine++;
		if(theApp.m_nBladeLine>=PROFILE->m_nLineBladeL[theApp.m_Thread.m_nBladeSide]) theApp.m_nBladeLine = PROFILE->m_nLineBladeL[theApp.m_Thread.m_nBladeSide] -1;

		for(nLine=0;nLine<PROFILE->m_nLineBladeL[theApp.m_Thread.m_nBladeSide];nLine++) {
			PROFILE->m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].m_nUseToScan = 0;
		};
		for(nLine=theApp.m_nBladeLine;nLine<PROFILE->m_nLineBladeL[theApp.m_Thread.m_nBladeSide];nLine++) {
			PROFILE->m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].m_nUseToScan = 1;
		}
		break;
	case SCAN_PREVIOUS_LINE:
		theApp.m_nBladeLine--;
		if(theApp.m_nBladeLine<0) theApp.m_nBladeLine = 0;

		for(nLine=0;nLine<PROFILE->m_nLineBladeL[theApp.m_Thread.m_nBladeSide];nLine++) {
			PROFILE->m_LineBlade[theApp.m_Thread.m_nBladeSide][nLine].m_nUseToScan = 0;
		};
		PROFILE->m_LineBlade[theApp.m_Thread.m_nBladeSide][theApp.m_nBladeLine].m_nUseToScan = 1;
		break;
	}
		

	theApp.m_nScanWhat = 0;
	theApp.m_PmacUser.m_nScanSide = 0;
	PROFILE->m_nCollectAxis = PROFILE->m_nFastAxis = 0;
	theApp.m_LastSettings.bUseBrainCompensation = true;

	MinMax(&PROFILE->m_nScanFinishLine,PROFILE->m_nScanStartLine,PROFILE->m_nLineBladeL[ theApp.m_Thread.m_nBladeSide ] - 1);
	MinMax(&PROFILE->m_nScanStartLine,0,PROFILE->m_nScanFinishLine);

	theApp.m_Thread.m_nScanStartLine = PROFILE->m_nScanStartLine;
	theApp.m_Thread.StopCurrentThreadAction();


	switch(nScanType) {
	case BLADE_SCAN:
		theApp.m_Thread.m_nThreadAction=BLADE_SCAN;
		break;
	}

}

bool CMainFrame::IsScanImageStillOpen()
{

	for(int nn=0;nn<MAX_NUMBER_VIEWS;nn++) {
		if((m_pViewSheet[nn] == m_pScanView) && (m_pViewSheet[nn] > NULL)) {
			return TRUE;
		}
	};
	return FALSE;
}

void CMainFrame::OnTimer(UINT nIDEvent)
{

	int nIndex,nStyle;
	static int nOldThreadAction = -1;
	static int m_nOldPreampMode = -1;
	CString Buff,Warning;
	CCoord Cp;
/*
	static int nEthercatLoop = 0;


	nEthercatLoop++;

	if(nEthercatLoop == 6) {
		theApp.m_FBCtrl.StartEthercat();
	}
	if(nEthercatLoop == 16) {
		theApp.m_Ethercat.Start();	
	}
	*/

	//	theApp.GetAvailableMemory();
	CheckLogOffTimeOut();
	ChangeMeasurementUnits();
	theApp.m_Motors.TotalDistanceTravelled();
	CheckHT();
	theApp.m_Kinematics.InvalidateToolchangerKinematics();
	InvalidateScanTime(NULL,NULL);


	if(nOldThreadAction - 	theApp.m_Thread.m_nThreadAction) {
		nOldThreadAction = theApp.m_Thread.m_nThreadAction;
	}

	theApp.m_PmacUser.PurgePump(theApp.m_Tank.nLeftPump);
	theApp.m_PmacUser.PurgePump(theApp.m_Tank.nRightPump);

	switch(theApp.m_nMotorType) {
	case SERVO_TURBO:
	case SERVO: //theApp.m_PmacUser.ChangePumpSpeedDueToHeight();
		break;
	case FIELD_BUS: theApp.m_FBCtrl.ChangePumpSpeedDueToHeight();
		break;
	}

	if(theApp.m_Kinematics.m_bDesiredWaterPathChanged == true) {
		InvalidateWaterPaths();
		theApp.m_Kinematics.m_bDesiredWaterPathChanged = false;
	}

	switch(m_nTimerAction) {
	case 0:
		break;
	case RELEARN_LINE:
		ReLearnTaught();
		break;
	case RELEARN_BRAIN:
		ReLearnBrane();
		break;
	case OPTOMISE_COORDINATE:
		OptomiseCoordinate();
		break;
	case ZERO_THETA:
		ZeroTheta();
		break;
	case WORKSHEET_SCAN:
		WorkSheetScan();
		break;
	case WORKSHEET_SINGLE_SCAN:
		WorkSheetSingleScan();
		break;
	}

	if(theApp.m_bServoHardware == TRUE) {
		if(theApp.m_PmacUser.CheckPumps() == true) {
			SetToolBarCheckedState(false);
		}
	}

	
	if(m_pToolBarPowered) {
		theApp.m_Motors.GetInputPort() & theApp.m_Tank.nEmergencyBit ? nStyle = 0 : nStyle = TBBS_CHECKED;
		if((nIndex=m_pToolBarPowered->CommandToIndex(ID_BUTTON_POWER))>=0) m_pToolBarPowered->SetButtonStyle(nIndex,nStyle);

		theApp.m_Motors.GetInputPort() & theApp.m_Tank.nAutomaticBit ? nStyle = TBBS_CHECKED : nStyle = 0;
		if((nIndex=m_pToolBarPowered->CommandToIndex(ID_BUTTON_AUTO))>=0) m_pToolBarPowered->SetButtonStyle(nIndex,nStyle);
	}



	int nInputIO = theApp.m_Motors.GetInputPort();


	if((theApp.m_LastSettings.bAutoEnableJoystick == true) && (theApp.m_Thread.m_nThreadAction==0) && (m_nTimerAction == 0)) {
		theApp.m_Thread.m_nThreadAction=JOYSTICK_START;
	}

	if( ((nInputIO&theApp.m_Tank.nEmergencyBit) && (theApp.m_Thread.m_nThreadAction==JOYSTICK_START)) ||
		((nInputIO&theApp.m_Tank.nAutomaticBit) && (nInputIO&theApp.m_Tank.nDoorBit) &&(theApp.m_Thread.m_nThreadAction==JOYSTICK_START))) 
	{
		theApp.m_bCollectComplexData=FALSE;
		theApp.m_bCollectBladeData=FALSE;
		theApp.m_bCollectNormalizeData=FALSE;
		theApp.m_bCollectTunning=FALSE;
		theApp.m_bCollectEccentricData=FALSE;
		theApp.m_Si10User.m_Si10.bCollectData = FALSE;
		theApp.m_bLaserCollectTaughtLines=FALSE;

		theApp.m_Thread.StopCurrentThreadAction();
		StopCurrentTimerAction();
		theApp.ActivityLog(L"Stopped code: 0x80001");

	}

	if(nInputIO&theApp.m_Tank.nKillBit) {

		theApp.m_PmacUser.KillAllMotors();
		theApp.m_bCollectComplexData=FALSE;
		theApp.m_bCollectBladeData=FALSE;
		theApp.m_bCollectNormalizeData=FALSE;
		theApp.m_bCollectTunning=FALSE;
		theApp.m_bCollectEccentricData=FALSE;
		theApp.m_Si10User.m_Si10.bCollectData = FALSE;
		theApp.m_bLaserCollectTaughtLines=FALSE;

		theApp.m_Thread.StopCurrentThreadAction();
		StopCurrentTimerAction();
		theApp.ActivityLog(L"Stopped code: 0x8002");
	}
	if( ((IsFatalFollowingError())||(IsZAmpliferDisabled())) && (theApp.m_LastSettings.nIOController == SAFETY_UNIT)){
		
		if((theApp.m_Motors.GetBrakeStatus(0)==TRUE)||(theApp.m_Motors.GetBrakeStatus(1)==TRUE)) { // brakes disengaged

			if(theApp.m_Tank.nBrakeInterlockPLCEnable) {
				theApp.m_Motors.ZBrakesEnable(true,0);
				theApp.m_Motors.ZBrakesEnable(true,1);
			}

			theApp.m_PmacUser.KillAllMotors();
			theApp.m_bCollectComplexData=FALSE;
			theApp.m_bCollectBladeData=FALSE;
			theApp.m_bCollectNormalizeData=FALSE;
			theApp.m_bCollectTunning=FALSE;
			theApp.m_bCollectEccentricData=FALSE;
			theApp.m_Si10User.m_Si10.bCollectData = FALSE;
			theApp.m_bLaserCollectTaughtLines=FALSE;

			theApp.m_Thread.StopCurrentThreadAction();
			StopCurrentTimerAction();
			theApp.ActivityLog(L"Stopped code: 0x8003");
		}
	}



	if(((nInputIO-m_nEmergencyBits)&theApp.m_Tank.nEmergencyBit)){
		m_nEmergencyBits = nInputIO;
		if(nInputIO&theApp.m_Tank.nEmergencyBit){
			if(theApp.m_Tank.nBrakeInterlockPLCEnable) {
				theApp.m_Motors.ZBrakesEnable(true,0);
				theApp.m_Motors.ZBrakesEnable(true,1);
			}
			switch(theApp.m_nMotorType) {
			case SERVO_TURBO:
			case SERVO:
				break;
			case SI6:
				break;
			case SI10:
				break;
			case FIELD_BUS:	theApp.m_FBCtrl.KillMotors();
				break;
			};

			theApp.m_Motors.SuspendMotors();
			theApp.ActivityLog(L"Stopped code: E-stop switch pressed");
			CheckHT(true);

			Buff.LoadString(IDS_Emergency_Switch_Pressed);
			Warning.LoadString(IDS_Warning);
			MessageBox(Buff,Warning,MB_ICONERROR);
			//if(m_pScanView != NULL) {
			//	m_pScanView->OnSave();
			//}
		}
	}

	if(((nInputIO-m_nDoorBits)&theApp.m_Tank.nDoorBit)){
		m_nDoorBits = nInputIO;
		if((nInputIO&theApp.m_Tank.nDoorBit) && (nInputIO&theApp.m_Tank.nAutomaticBit)){
			theApp.ActivityLog(L"Stopped code: 0x8005");

			Buff.LoadString(IDS_Perimeter_Tripped);
			Warning.LoadString(IDS_Warning);
			MessageBox(Buff,Warning,MB_ICONERROR);
			if(m_pScanView != NULL) {
				m_pScanView->OnSave();
			}
		}
	}

	if(((nInputIO-m_nGateBits)&theApp.m_Tank.nGateBit)){
		m_nGateBits = nInputIO;
		if((nInputIO&theApp.m_Tank.nGateBit) && (nInputIO&theApp.m_Tank.nAutomaticBit)){
			theApp.ActivityLog(L"Stopped code: 0x8006");
			Buff.LoadString(IDS_Perimeter_Tripped);
			Warning.LoadString(IDS_Warning);
			MessageBox(Buff,Warning,MB_ICONERROR);
			if(m_pScanView != NULL) {
				m_pScanView->OnSave();
			}
		}
	}

	
	if( ((nInputIO&theApp.m_Tank.nEmergencyBit)) ||
		((nInputIO&theApp.m_Tank.nAutomaticBit) && (nInputIO&theApp.m_Tank.nDoorBit)) ||
		((nInputIO&theApp.m_Tank.nAutomaticBit) && (nInputIO&theApp.m_Tank.nGateBit))
		) {

		theApp.m_bCollectComplexData=FALSE;
		theApp.m_bCollectBladeData=FALSE;
		theApp.m_bCollectNormalizeData=FALSE;
		theApp.m_bCollectTunning=FALSE;
		theApp.m_bCollectEccentricData=FALSE;
		theApp.m_Si10User.m_Si10.bCollectData = FALSE;
		theApp.m_bLaserCollectTaughtLines=FALSE;

		if(theApp.m_Thread.m_nThreadAction != 0) {
			theApp.ActivityLog(L"Stopped code: 0x8007");
		}
		theApp.m_Thread.StopCurrentThreadAction();
		StopCurrentTimerAction();
	}

	if(m_LoggedOnOperator != theApp.m_Operator) {
		if(theApp.m_nLogonLevelMask & MANUFACTURER) {
			Buff.LoadString(IDS_Manufacturer);
		} else {
			if(theApp.m_nLogonLevelMask & SUPERVISOR) {
				Buff.LoadString(IDS_Supervisor);
			} else {
				if(theApp.m_nLogonLevelMask & MAINTENANCE) {
					Buff.LoadString(IDS_Maintenance);
				} else {
					Buff.LoadString(IDS_Operator);
				}
			}
		}

		Buff += (L":" +  theApp.m_Operator);
		m_wndStatusBar.SetPaneText(OPERATOR_PANE,Buff);
		m_LoggedOnOperator = theApp.m_Operator;
	}
	RefreshMessageInStatusBar(); 
	theApp.m_RRMES.StateMachine();
	theApp.AudioAlarm();

	Pr30Data* pPr30 = &theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Pr30;
	if(m_pToolBarUltrasonics) {
		if(pPr30->nPreampMode != m_nOldPreampMode) { 
			pPr30->nPreampMode==0 ? nStyle = TBBS_CHECKED : nStyle = 0;
			if((nIndex=m_pToolBarUltrasonics->CommandToIndex(ID_BUTTON_PULSE_ECHO))>=0) m_pToolBarUltrasonics->SetButtonStyle(nIndex,nStyle);
			pPr30->nPreampMode==1 ? nStyle = TBBS_CHECKED : nStyle = 0;
			if((nIndex=m_pToolBarUltrasonics->CommandToIndex(ID_BUTTON_THROUGH_TRANSMISSION))>=0) m_pToolBarUltrasonics->SetButtonStyle(nIndex,nStyle);
		}
	}

	CFrameWndEx::OnTimer(nIDEvent);
}


bool CMainFrame::IsFatalFollowingError()
{

	int	ii;
	int nAxes=theApp.m_PmacUser.GetNumberAxes();

	if(theApp.m_nJobNumber==GKN){
		nAxes=12;
	}

	for(ii=0;ii<nAxes;ii++) {
		if((theApp.m_Axes[ii].bAvailable)&&(theApp.m_Axes[ii].nMotorType==SERVO)&&(theApp.m_Axes[ii].bEnabled)) {
			if(theApp.m_Axes[ii].nStatus&PMAC_HARD_LIMIT) return true;
		}
	}
	return false;
}

bool CMainFrame::IsZAmpliferDisabled()
{


	if(theApp.m_LastSettings.nBrakeScannerMode==1) {
		if( (theApp.m_Axes[theApp.m_Tank.nZRight].bAvailable) && (!(theApp.m_Axes[theApp.m_Tank.nZRight].nStatus&PMAC_AMPLIFIER_ENABLED))) return true;
		if( (theApp.m_Axes[theApp.m_Tank.nZLeft].bAvailable) && (!(theApp.m_Axes[theApp.m_Tank.nZLeft].nStatus&PMAC_AMPLIFIER_ENABLED))) return true;
	}
	

	return false;
}


void CMainFrame::StopCurrentTimerAction()
{
	m_nTimerAction=0;
	m_nSubTimerAction=0;
}

void CMainFrame::ReLearnLine()
{

	CCoord CpSurface,CpHead,CpApproach,CpDanger,Cp;

	switch(m_nSubTimerAction) {
	case 0:
		theApp.m_nIndex=0;
		m_CpNewLine.Zero();
		theApp.m_Thread.m_bEnableJoystick=FALSE;
		theApp.m_Thread.StopCurrentThreadAction();
		if(theApp.m_LastSettings.nRelearnSurfacePos[0]) {
			m_nSubTimerAction=10;
			return;
		}
		if(theApp.m_LastSettings.nRelearnNormals[0]) {
			m_nSubTimerAction=110;
			return;
		}
		if(theApp.m_LastSettings.nRelearnPeak[1]) {
			m_nSubTimerAction=210;
			return;
		}
		break;
	case 10:	//SURFACE POS
		if(theApp.m_nIndex<PROFILE->GetTaughtLineCoordL(theApp.m_nLine)) {
			PROFILE->GetTaughtCoord(theApp.m_nLine,theApp.m_nIndex,&CpSurface);
			theApp.m_Motors.GetHeadPos(&CpHead);
			theApp.m_Kinematics.CalculateApproachPoint(CpSurface,&CpApproach,PORTSIDE);
			theApp.m_Kinematics.HeadFromSurface(&CpApproach,&theApp.m_Thread.m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Thread.m_CpMoveTo.CopySide(STARBOARD,CpHead); //Keep right side where it is

			theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
			m_nSubTimerAction=20;
		} else {
			m_nSubTimerAction=999;
		}
		break;
	case 20:
		if(theApp.m_Thread.HasMovetoStraightLineFinished()==TRUE) {
			theApp.m_Motors.GetHeadPos(&CpHead);
			theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
			theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.CalculateOppositeCoord(STARBOARD,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
			m_CpNewLine.Add(CpSurface);
			m_CpNewLine.m_nStyle = PROFILE->m_TaughtLine[theApp.m_nLine].m_nStyle;
			Update3DViewDlg(NULL,NULL);
			theApp.m_nIndex++;
			m_nSubTimerAction=10;
		}
		break;

	case 110:	//NORMALS
		if(theApp.m_nIndex<PROFILE->GetTaughtLineCoordL(theApp.m_nLine)) {
			theApp.m_Thread.StopCurrentThreadAction();
			PROFILE->GetTaughtCoord(theApp.m_nLine,theApp.m_nIndex,&CpSurface);
			theApp.m_Motors.GetHeadPos(&CpHead);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&theApp.m_Thread.m_CpMoveTo,PORTSIDE,TRUE,TRUE);
			theApp.m_Thread.m_CpMoveTo.CopySide(STARBOARD,CpHead); //Keep right side where it is

			theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
			m_nSubTimerAction=120;
		} else {
			m_nSubTimerAction=999;
		}
		break;
	case 120:
		if(theApp.m_Thread.HasMovetoStraightLineFinished()==TRUE) {
			theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXtLeft;
			theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYtLeft;
			theApp.m_Thread.m_nNormAction = NORMALIZE;
			theApp.m_Thread.m_nNormSide = PORTSIDE;

			theApp.m_Thread.m_nThreadAction=NORMALIZE;
			m_nSubTimerAction=130;
		}
		break;
	case 130:
		if(theApp.m_Thread.HasNormalizeFinished()==TRUE) {
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Motors.GetHeadPos(&CpHead);
			theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
			theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.CalculateOppositeCoord(STARBOARD,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
			m_CpNewLine.Add(CpSurface);
			m_CpNewLine.m_nStyle = PROFILE->m_TaughtLine[theApp.m_nLine].m_nStyle;
			Update3DViewDlg(NULL,NULL);
			theApp.m_nIndex++;
			m_nSubTimerAction=110;
		}
		break;

	case 210:	//PEAK_OPPOSITE
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Motors.GetHeadPos(&CpHead);
		theApp.m_Thread.m_CpMoveTo = CpHead;
		CpDanger = PROFILE->m_CpDanger[DANGER_MINS];
		theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.HeadFromSurface(&CpDanger,&Cp,STARBOARD,TRUE,TRUE);
		theApp.m_Thread.m_CpMoveTo.Side0.fY = Cp.Side0.fY;
		theApp.m_Thread.m_CpMoveTo.Side1.fY = Cp.Side1.fY;

		theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
		Sleep(300);
		m_nSubTimerAction=220;
		break;
	case 220:
		if(theApp.m_Thread.HasMovetoStraightLineFinished()==TRUE) {
			if(theApp.m_nIndex<PROFILE->GetTaughtLineCoordL(theApp.m_nLine)) {
				theApp.m_Thread.StopCurrentThreadAction();
				theApp.m_Motors.GetHeadPos(&CpHead);
				PROFILE->GetTaughtCoord(theApp.m_nLine,theApp.m_nIndex,&CpSurface);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&theApp.m_Thread.m_CpMoveTo,PORTSIDE,TRUE,TRUE);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&theApp.m_Thread.m_CpMoveTo,STARBOARD,TRUE,TRUE);
				theApp.m_Thread.m_CpMoveTo.Side0.fY = CpHead.Side0.fY;
				theApp.m_Thread.m_CpMoveTo.Side1.fY = CpHead.Side1.fY;

				theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
				Sleep(300);
				m_nSubTimerAction=230;
			} else {
				m_nSubTimerAction=999;
			}
		}
		break;
	case 230:
		if(theApp.m_Thread.HasMovetoStraightLineFinished()==TRUE) {
			if(theApp.m_nIndex<PROFILE->GetTaughtLineCoordL(theApp.m_nLine)) {
				theApp.m_Thread.StopCurrentThreadAction();
				PROFILE->GetTaughtCoord(theApp.m_nLine,theApp.m_nIndex,&CpSurface);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&theApp.m_Thread.m_CpMoveTo,PORTSIDE,TRUE,TRUE);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&theApp.m_Thread.m_CpMoveTo,STARBOARD,TRUE,TRUE);

				theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
				m_nSubTimerAction=240;
			} else {
				m_nSubTimerAction=999;
			}
		}
		break;
	case 240:
		if(theApp.m_Thread.HasMovetoStraightLineFinished()==TRUE) {
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXRight;
			theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYRight;
			theApp.m_Thread.m_nNormAction = PEAK_OPPOSITE;
			theApp.m_Thread.m_nNormSide = STARBOARD;

			theApp.m_Thread.m_nThreadAction=PEAK_OPPOSITE;
			m_nSubTimerAction=250;
		}
		break;
	case 250:
		if(theApp.m_Thread.HasPeakOppositeFinished()==TRUE) {
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Motors.GetHeadPos(&CpHead);
			theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
			theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
			m_CpNewLine.Add(CpSurface);
			m_CpNewLine.m_nStyle = PROFILE->m_TaughtLine[theApp.m_nLine].m_nStyle;
			Update3DViewDlg(NULL, NULL);
			theApp.m_nIndex++;
			m_nSubTimerAction=230;
		}
		break;

	case 999:
		m_nTimerAction=0;
		m_nSubTimerAction=0;
		theApp.m_Thread.m_bEnableJoystick=TRUE;
		theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
		break;
	}

}

HRESULT CMainFrame::OnMessageInvalidateView(WPARAM, LPARAM)
{
	m_pScanView->InvalidateChildCommandedFromScanThread();
	return NULL;
}



void CMainFrame::OnUltrasonicsOpen()
{

	CString FilePath,Buff;
	int	nPos;
	CUSLFile File;
	CFileException e;

	CFileDialog FileDlg(TRUE,(L".ul"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,(L"Ultrasonics (*.ul)|*.ul|All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_UltrasonicsPath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_UltrasonicsPath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,(L"Error"),MB_ICONERROR);
		} else {
			theApp.m_UtUser.Retrieve(&File);
			File.Close();
			theApp.m_UtUser.ResetMaxTimeSlots();
			theApp.m_UtUser.Initialize(theApp.m_nJobNumber);
			if(m_pUltrasonicsSheet) m_pUltrasonicsSheet->UpdateAllPages();
		}
	}
}

void CMainFrame::OnUltrasonicsSaveas()
{

	int nPos;
	CUSLFile File;
	CFileException e;
	CString FilePath,Buff;
	CString InitialFile = theApp.m_UtUser.m_FileName;
	if((nPos = theApp.m_UtUser.m_FileName.ReverseFind('.')) >0) {
		InitialFile = theApp.m_UtUser.m_FileName.Left(nPos);
	}

	CFileDialog FileDlg(FALSE,(L".ul"),InitialFile,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,(L"Ultrasonics (*.ul)|*.ul|All Files (*.*)|*.*||"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_PalettePath;
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_UltrasonicsPath = FilePath.Left(nPos);
			}
		}

		if(!File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &e)) {
			File.DecodeFileException(&e,&Buff,FilePath);
			MessageBox(Buff,(L"Error"),MB_ICONERROR);
		} else {
			theApp.m_UtUser.Save(&File);
			File.Close();
		}
	}
}

void CMainFrame::StatusBarPrompt(CString Text,int nSeconds)
{

	CString Buff,Temp;

	m_wndStatusBar.SetPaneText(0,Text);
	Sleep(nSeconds);

	Buff.LoadString(AFX_IDS_IDLEMESSAGE);


}

void CMainFrame::OnUltrasonicsSpectrumanalyzer()
{
	if (m_pSpectrumAnalyzerDlg == NULL) {
		m_pSpectrumAnalyzerDlg = new CSpectrumAnalyzerDlg(this,(CWnd**)&m_pSpectrumAnalyzerDlg);
		m_pSpectrumAnalyzerDlg->Create(CSpectrumAnalyzerDlg::IDD, this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[CSpectrumAnalyzerDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pSpectrumAnalyzerDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}

HRESULT CMainFrame::ProfileRetrievedUpdateDialogs(WPARAM wp, LPARAM lp)
{

	CWnd* pWnd = (CWnd*)wp;

	theApp.m_Data.m_ProfileName = PROFILE->m_FileName;
	theApp.m_Data.m_ProfileDescription = PROFILE->m_strDescription;

	if(m_pProfileSheet!=NULL) {
		m_pProfileSheet->UpdateAllPages();
	};
	if((m_p3DViewEditDlg) && (pWnd!=m_p3DViewEditDlg)) {
		m_p3DViewEditDlg->CreateColumns();
		m_p3DViewEditDlg->FillList();
		m_p3DViewEditDlg->Invalidate(FALSE);
	}
	if(m_pMotionToolsSheet) {
		m_pMotionToolsSheet->UpdateAllPages();
	}
	if(m_pUltrasonicsSheet) {
		m_pUltrasonicsSheet->UpdateAllPages();
	}
	if(m_pScanSheet) {
		m_pScanSheet->UpdateAllPages();
	}
	PostMessage(UI_UPDATE_DIALOG_BAR, NULL, NULL);
	PostMessage(UI_INVALIDATE_COORD_ARRAY_PANE, 0xFF, 0x00);

	return NULL;
}

LRESULT CMainFrame::UpdateMotionToolsSheet(WPARAM wp, LPARAM lp)
{
	if(m_pMotionToolsSheet) {
		m_pMotionToolsSheet->UpdateAllPages();
	}
	return true;
}

HRESULT CMainFrame::Update3DViewDlg(WPARAM, LPARAM)
{
	if(m_pProfileSheet) {
		m_pProfileSheet->UpdateAllPages();
	}
	return NULL;
}

HRESULT CMainFrame::Invalidate3DViewPage(WPARAM wp, LPARAM lp)
{
	C3DViewPage *pPage;
	if(m_pProfileSheet) {
		pPage = (C3DViewPage*)m_pProfileSheet->GetActivePage();
		if(pPage == m_pProfileSheet->m_p3DViewPage) {
			PROFILE->CalculateDangerZone();
			pPage->InitializeDangerZone();

			if(wp & 1) pPage->InitializeCScanOverlay();

			pPage->Invalidate(false);
		}
	}
	return NULL;
}


HRESULT CMainFrame::UpdateGatesPage(WPARAM, LPARAM)
{
	if(m_pUltrasonicsSheet) {
		m_pUltrasonicsSheet->UpdateGatesPage();
	}
	return NULL;
}

void CMainFrame::ChangePalletteNumber(int nImageType, int nImageNumber)
{
	if (m_pPaletteSheet != NULL) {
		switch(nImageType) {
		case 0: m_pPaletteSheet->m_pAmplitude->m_nWhichAmp = nImageNumber;
			break;
		case 1: m_pPaletteSheet->m_pThickness->m_nWhich = nImageNumber;
			break;
		}
		m_pPaletteSheet->UpdateAllPages();
	}
}

void CMainFrame::OnSystemImportthirdpartyimage()
{

	CString FilePath;
	int	nPos,nn;

	CFileDialog FileDlg(TRUE,(L".*"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,(L"All Files (*.*)|*.*|"),NULL);
	FileDlg.m_ofn.lpstrInitialDir=theApp.m_StorePath[0];
	if(FileDlg.DoModal()==IDOK) {
		FilePath=FileDlg.GetPathName();
		if(theApp.m_LastSettings.bUpdateDirectoryPath) {
			if((nPos = FilePath.ReverseFind('\\')) >0) {
				theApp.m_StorePath[0] = FilePath.Left(nPos);
			}
		}

		for(nn=0;nn<2;nn++) {
			if (m_pThirdPartyImageDlg[nn] == NULL) {
				m_pThirdPartyImageDlg[nn] = new CThirdPartyImageDlg(this,(CWnd**)&m_pThirdPartyImageDlg[nn],FilePath);
				m_pThirdPartyImageDlg[nn]->Create(CThirdPartyImageDlg::IDD, this);
				theApp.m_LastSettings.bWindowOpenOnLastExit[CThirdPartyImageDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
				return;
			}
		}
	}


}

void CMainFrame::OnDeltaposSpinPumpSpeed0(NMHDR* pNMHDR, LRESULT* pResult)
{

	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CEdit* pEdit;
	CString Buff;
	int nAxis, nSide;

	if(theApp.m_nSide0Orientation==0) {
		nAxis = theApp.m_Tank.nLeftPump;
		nSide = 0;
	} else {
		nAxis = theApp.m_Tank.nRightPump;
		nSide = 1;
	}

	theApp.m_Axes[nAxis].nMaxSpeed  += pNMUpDown->iDelta;
	theApp.m_Motors.SetPumpSpeed(nAxis,nSide);


	if(m_pDialogBarPumps) {
		Buff.Format(L"%d",theApp.m_Axes[nAxis].nMaxSpeed);
		pEdit = (CEdit * )m_pDialogBarPumps->GetDlgItem(IDC_EDIT_PUMP_SPEED_0);
		pEdit->SetWindowText(Buff);
		if(m_pMotionToolsSheet && m_pMotionToolsSheet->m_pPumpPage) m_pMotionToolsSheet->m_pPumpPage->UpdateAllControls();
	}

	*pResult = 0;
}

void CMainFrame::OnDeltaposSpinPumpSpeed1(NMHDR* pNMHDR, LRESULT* pResult)
{

	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CEdit* pEdit;
	CString Buff;
	int nAxis, nSide;

	if(theApp.m_nSide0Orientation==0) {
		nAxis = theApp.m_Tank.nRightPump;
		nSide = 1;
	} else {
		nAxis = theApp.m_Tank.nLeftPump;
		nSide = 0;
	}

	theApp.m_Axes[nAxis].nMaxSpeed  += pNMUpDown->iDelta;
	theApp.m_Motors.SetPumpSpeed(nAxis,nSide);

	if(m_pDialogBarPumps) {
		Buff.Format(L"%d",theApp.m_Axes[nAxis].nMaxSpeed);
		pEdit = (CEdit* ) m_pDialogBarPumps->GetDlgItem(IDC_EDIT_PUMP_SPEED_1);
		pEdit->SetWindowText(Buff);
		if(m_pMotionToolsSheet && m_pMotionToolsSheet->m_pPumpPage) m_pMotionToolsSheet->m_pPumpPage->UpdateAllControls();
	}

	*pResult = 0;
}

void CMainFrame::OnVScrollEditPumpSpeed00()
{
	int foo = 0;
}

void CMainFrame::OnDeltaposSpinPumpSpeed00(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	int nAxis, nSide;

	if (theApp.m_nSide0Orientation == 0) {
		nAxis = theApp.m_Tank.nLeftPump;
		nSide = 0;
	}
	else {
		nAxis = theApp.m_Tank.nRightPump;
		nSide = 1;
	}

	theApp.m_Axes[nAxis].nMaxSpeed += pNMUpDown->iDelta;
	theApp.m_Motors.SetPumpSpeed(nAxis, nSide);


	Buff.Format(L"%d", theApp.m_Axes[nAxis].nMaxSpeed);
	CMFCToolBarEditBoxButton::SetContentsAll(IDC_EDIT_PUMP_SPEED_00, Buff);
	
	if (m_pMotionToolsSheet && m_pMotionToolsSheet->m_pPumpPage) m_pMotionToolsSheet->m_pPumpPage->UpdateAllControls();

	*pResult = 0;
}

void CMainFrame::OnDeltaposSpinPumpSpeed01(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	int nAxis, nSide;

	if (theApp.m_nSide0Orientation == 0) {
		nAxis = theApp.m_Tank.nRightPump;
		nSide = 1;
	}
	else {
		nAxis = theApp.m_Tank.nLeftPump;
		nSide = 0;
	}

	theApp.m_Axes[nAxis].nMaxSpeed += pNMUpDown->iDelta;
	theApp.m_Motors.SetPumpSpeed(nAxis, nSide);


	Buff.Format(L"%d", theApp.m_Axes[nAxis].nMaxSpeed);
	CMFCToolBarEditBoxButton::SetContentsAll(IDC_EDIT_PUMP_SPEED_01, Buff);
	if (m_pMotionToolsSheet && m_pMotionToolsSheet->m_pPumpPage) m_pMotionToolsSheet->m_pPumpPage->UpdateAllControls();


	*pResult = 0;
}




void CMainFrame::OnChangeEditPumpSpeed0()
{

	CString	Buff;
	int nAxis, nSide;

	if(m_pDialogBarPumps == NULL) return;

	if(theApp.m_nSide0Orientation==0) {
		nAxis = theApp.m_Tank.nLeftPump;
		nSide = 0;
	} else {
		nAxis = theApp.m_Tank.nRightPump;
		nSide = 1;
	}

	CEdit* pEdit = (CEdit* )m_pDialogBarPumps->GetDlgItem(IDC_EDIT_PUMP_SPEED_0);
	if(pEdit) {
		pEdit->GetWindowText(Buff);

		theApp.m_Axes[nAxis].nMaxSpeed = _ttoi(Buff);
		theApp.m_Motors.SetPumpSpeed(nAxis,nSide);


		CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)m_pDialogBarPumps->GetDlgItem(IDC_SPIN_PUMP_SPEED_0);
		pSpin->SetPos(theApp.m_Axes[nAxis].nMaxSpeed);
		if(m_pMotionToolsSheet && m_pMotionToolsSheet->m_pPumpPage) m_pMotionToolsSheet->m_pPumpPage->UpdateAllControls();
	}
}

void CMainFrame::OnChangeEditPumpSpeed1()
{

	CString	Buff;
	int nAxis, nSide;

	if(m_pDialogBarPumps == NULL) return;

	if(theApp.m_nSide0Orientation==0) {
		nAxis = theApp.m_Tank.nRightPump;
		nSide = 1;
	} else {
		nAxis = theApp.m_Tank.nLeftPump;
		nSide = 0;
	}

	CEdit* pEdit = (CEdit* )m_pDialogBarPumps->GetDlgItem(IDC_EDIT_PUMP_SPEED_1);
	if(pEdit) {
		pEdit->GetWindowText(Buff);

		theApp.m_Axes[nAxis].nMaxSpeed = _ttoi(Buff);
		theApp.m_Motors.SetPumpSpeed(nAxis,nSide);

		CSpinButtonCtrl* pSpin = (CSpinButtonCtrl*)m_pDialogBarPumps->GetDlgItem(IDC_SPIN_PUMP_SPEED_1);
		pSpin->SetPos(theApp.m_Axes[nAxis].nMaxSpeed);
		if(m_pMotionToolsSheet && m_pMotionToolsSheet->m_pPumpPage) m_pMotionToolsSheet->m_pPumpPage->UpdateAllControls();
	}
}

void CMainFrame::OnChangeEditPumpSpeed00()
{
	if (!m_bToolBarsInitialized) return;

	CString	Buff;
	int nAxis, nSide;

//	if (m_pToolBarAirWater0 == NULL) return;

	if (theApp.m_nSide0Orientation == 0) {
		nAxis = theApp.m_Tank.nLeftPump;
		nSide = 0;
	}
	else {
		nAxis = theApp.m_Tank.nRightPump;
		nSide = 1;
	}

		Buff = CMFCToolBarEditBoxButton::GetContentsAll(IDC_EDIT_PUMP_SPEED_00);

		theApp.m_Axes[nAxis].nMaxSpeed = _ttoi(Buff);
		theApp.m_Motors.SetPumpSpeed(nAxis, nSide);

		CMFCToolBarSpinButton::SetPos(IDC_SPIN_PUMP_SPEED_00, (int)theApp.m_Axes[nAxis].nMaxSpeed);

		if (m_pMotionToolsSheet && m_pMotionToolsSheet->m_pPumpPage) m_pMotionToolsSheet->m_pPumpPage->UpdateAllControls();
		
}

void CMainFrame::OnChangeEditPumpSpeed01()
{
	if (!m_bToolBarsInitialized) return;

	CString	Buff;
	int nAxis, nSide;

//	if (m_pToolBarAirWater1 == NULL) return;

	if (theApp.m_nSide0Orientation == 0) {
		nAxis = theApp.m_Tank.nRightPump;
		nSide = 1;
	}
	else {
		nAxis = theApp.m_Tank.nLeftPump;
		nSide = 0;
	}


		Buff = CMFCToolBarEditBoxButton::GetContentsAll(IDC_EDIT_PUMP_SPEED_01);

		theApp.m_Axes[nAxis].nMaxSpeed = _ttoi(Buff);
		theApp.m_Motors.SetPumpSpeed(nAxis, nSide);

		CMFCToolBarSpinButton::SetPos(IDC_SPIN_PUMP_SPEED_01, (int)theApp.m_Axes[nAxis].nMaxSpeed);

		if (m_pMotionToolsSheet && m_pMotionToolsSheet->m_pPumpPage) m_pMotionToolsSheet->m_pPumpPage->UpdateAllControls();

}


void CMainFrame::OnDeltaposSpinLeftWaterPath(NMHDR* pNMHDR, LRESULT* pResult)
{
//	if (m_pToolBarManipulator == NULL) return;


	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	int nSide;
	float fOldWp;

	if(theApp.m_nSide0Orientation==0) {
		nSide = PORTSIDE;
	} else {
		nSide = STARBOARD;
	}

	fOldWp = theApp.m_Kinematics.m_fDesiredWaterPath[nSide];
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		theApp.m_Kinematics.m_fDesiredWaterPath[nSide] += (float)(pNMUpDown->iDelta*5);
	} else {
		theApp.m_Kinematics.m_fDesiredWaterPath[nSide] += ((float)pNMUpDown->iDelta * 2.54f);
	}
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TWIN_TOWER_KINEMATIC:
		if(fOldWp - theApp.m_Kinematics.m_fDesiredWaterPath[nSide]) {
			if(theApp.m_Thread.IsJoystickRunning()) {
				theApp.m_Thread.StopCurrentThreadAction();
				theApp.m_FBCtrl.KillAllMotionPrograms();
				theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(nSide,true);
				theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
			} else {
				theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(nSide,false);
			}
		}
		break;
	case TRACKED_PROBE:
		break;
	}

	Buff.Format((L"%.*f%s"),theApp.m_nWpPrecision,theApp.m_Kinematics.m_fDesiredWaterPath[nSide]*theApp.m_fUnits,theApp.m_Units);
	CMFCToolBarEditBoxButton::SetContentsAll(ID_EDIT_LEFT_WATER_PATH, Buff);

	*pResult = 0;
}

void CMainFrame::OnDeltaposSpinRightWaterPath(NMHDR* pNMHDR, LRESULT* pResult)
{
//	if (m_pToolBarManipulator == NULL) return;

	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;
	int nSide;
	float fOldWp;

	if(theApp.m_nSide0Orientation==0) {
		nSide = STARBOARD;
	} else {
		nSide = PORTSIDE;
	}

	fOldWp = theApp.m_Kinematics.m_fDesiredWaterPath[nSide];
	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		theApp.m_Kinematics.m_fDesiredWaterPath[nSide] += (float)(pNMUpDown->iDelta*5);
	} else {
		theApp.m_Kinematics.m_fDesiredWaterPath[nSide] += ((float)pNMUpDown->iDelta * 2.54f);
	}
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TWIN_TOWER_KINEMATIC:
		if(fOldWp - theApp.m_Kinematics.m_fDesiredWaterPath[nSide]) {
			if(theApp.m_Thread.IsJoystickRunning()) {
				theApp.m_Thread.StopCurrentThreadAction();
				theApp.m_FBCtrl.KillAllMotionPrograms();
				theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(nSide,true);
				theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
			} else {
				theApp.m_FBCtrl.DownloadEndEffectorWithWaterPath(nSide,false);
			}
		}
		break;
	case TRACKED_PROBE:
		break;
	}

	Buff.Format((L"%.*f%s"),theApp.m_nWpPrecision,theApp.m_Kinematics.m_fDesiredWaterPath[nSide]*theApp.m_fUnits,theApp.m_Units);
	CMFCToolBarEditBoxButton::SetContentsAll(ID_EDIT_RIGHT_WATER_PATH, Buff);

	*pResult = 0;
}

void CMainFrame::OnDeltaposSpinThickness(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;

	if(theApp.m_LastSettings.nMeasurementUnits==0) {
		theApp.m_Kinematics.m_fComponentThickness += (float)(pNMUpDown->iDelta*5);
	} else {
		theApp.m_Kinematics.m_fComponentThickness += ((float)pNMUpDown->iDelta * 2.54f);
	}
	MinMax(&theApp.m_Kinematics.m_fComponentThickness, 0.0f, 2000.0f);

	Buff.Format((L"%.*f%s"),theApp.m_nUnitsPrecision,theApp.m_Kinematics.m_fComponentThickness*theApp.m_fUnits,theApp.m_Units);
	CMFCToolBarEditBoxButton::SetContentsAll(ID_EDIT_THICKNESS, Buff);

	*pResult = 0;
}

void CMainFrame::OnDeltaposSpinProbeLength(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString Buff;

	if (theApp.m_LastSettings.nMeasurementUnits == 0) {
		theApp.m_Kinematics.m_fProbeLength[0] += (float)(pNMUpDown->iDelta * 5);
	}
	else {
		theApp.m_Kinematics.m_fProbeLength[0] += ((float)pNMUpDown->iDelta * 2.54f);
	}
	MinMax(&theApp.m_Kinematics.m_fProbeLength[0], 0.0f, 2000.0f);

	Buff.Format((L"%.*f%s"), theApp.m_nUnitsPrecision, theApp.m_Kinematics.m_fProbeLength[0] *theApp.m_fUnits, theApp.m_Units);
	CMFCToolBarEditBoxButton::SetContentsAll(ID_EDIT_LEFT_PROBE_LENGTH, Buff);

	*pResult = 0;
}

void CMainFrame::OnDeltaposSliderFeedRate(NMHDR* pNMHDR, LRESULT* pResult)
{
	CSliderCtrl *pSlider = NULL;
}

CCoord m_CpSurfaceStart;
CCoord m_CpBranePt;
bool m_bSingleSided;


void CMainFrame::RefreshSideFromRelearn()
{
	if((theApp.m_LastSettings.nRelearnSurfacePos[1] != 1) && (theApp.m_LastSettings.nRelearnPeak[1] != 1)) {
		m_bSingleSided = true;
		PROFILE->m_nScanSide = theApp.m_PmacUser.m_nScanSide = 0;
	} else {
		m_bSingleSided = false;
		PROFILE->m_nScanSide = theApp.m_PmacUser.m_nScanSide = 2;
	}
}

void CMainFrame::ReLearnTaught()
{
	switch (theApp.m_Tank.nScannerDescription) {
	default:	ReLearnTaughtOld();
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TWIN_TOWER_KINEMATIC:
		ReLearnFieldBus(TAUGHTLINES);
		break;
	case TRACKED_PROBE:
		break;
	}
}


void CMainFrame::ReLearnBrane()
{
	switch (theApp.m_Tank.nScannerDescription) {
	default:	ReLearnBraneOld();
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TWIN_TOWER_KINEMATIC:
		ReLearnFieldBus(BRAINLINES);
		break;
	case TRACKED_PROBE:
		break;
	}
}

void CMainFrame::ReLearnBraneOld()
{
	CCoord CpHead, CpSurface,CpApproach,CpDanger,Cp;
	CPolyCoord* pLine = PROFILE->m_BrainLine;
	int nLineL = PROFILE->m_nBrainLineL;

	switch(m_nSubTimerAction) {
	case 0:
		RefreshSideFromRelearn();
		theApp.m_nIndex=0;
		theApp.m_Thread.m_bEnableJoystick=FALSE;
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Motors.GetSurfacePos(&m_CpSurfaceStart);
		m_nLine = 0;
		m_nIndex = 0;
		m_nSubTimerAction=10;
		break;
	case 10:	//Find new line and new index
		for(m_nLine;m_nLine<nLineL;m_nLine++) {
			for(m_nIndex=0;m_nIndex<pLine[m_nLine].m_nCoordL;m_nIndex++) {
				if(pLine[m_nLine].m_pCp[m_nIndex].bModified==TRUE) {
					m_nSubTimerAction=20;
					return;
					break;
				}
			}
		}
		m_nSubTimerAction = 999;
		break;
	case 20:
		if(theApp.m_Thread.HasAnyTypeOfMoveToFinished()==TRUE) {
			RefreshSideFromRelearn();
			if(theApp.m_LastSettings.nBrainMoveToOption==0) {
				theApp.m_Thread.m_CpMoveTo = PROFILE->m_BrainLine[m_nLine].m_pCp[m_nIndex];
			} else {
				m_CpBranePt = PROFILE->m_BrainLine[m_nLine].m_pCp[m_nIndex];

				PROFILE->CalculateBrainCoord(PROFILE->m_nFastAxis,PROFILE->m_nSlowAxis,m_CpBranePt);

				theApp.m_Kinematics.TipsFromNormals(&m_CpBranePt,PORTSIDE);
				theApp.m_Kinematics.TipsFromNormals(&m_CpBranePt,STARBOARD);

				theApp.m_Kinematics.HeadFromSurface(&m_CpBranePt,&Cp,PORTSIDE,TRUE,TRUE);
				theApp.m_Kinematics.HeadFromSurface(&m_CpBranePt,&Cp,STARBOARD,TRUE,TRUE);

				if(m_bSingleSided == true) m_CpBranePt.Side1 = m_CpSurfaceStart.Side1;
				theApp.m_Thread.m_CpMoveTo = m_CpBranePt;
			}

			if(theApp.m_LastSettings.nBraneUseDangerPlane) {
				theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
			} else {
				theApp.m_Thread.m_nThreadAction = MOVETO_SURFACE;
			}
			m_nSubTimerAction = 30;

			Sleep(500);
		}
		break;
	case 30:
		if(theApp.m_Thread.HasAnyTypeOfMoveToFinished()==TRUE) {
			m_nSubTimerAction = 100;
		}
		break;
//Normalize Left
	case 100:
		if(theApp.m_LastSettings.nRelearnNormals[0] == 1) {
			RefreshSideFromRelearn();
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXtLeft;
			theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYtLeft;
			theApp.m_Thread.m_nNormAction = NORMALIZE;
			theApp.m_Thread.m_nNormSide = theApp.m_nPrimarySide;
			theApp.m_Thread.m_nThreadAction=NORMALIZE;
			m_nSubTimerAction += 10;
		} else {
			m_nSubTimerAction = 200;
		}
		break;
	case 110: 
		if(theApp.m_Thread.HasNormalizeStarted()==TRUE) {
			m_nSubTimerAction += 10;
		}
		break;
	case 120:
		if(theApp.m_Thread.HasNormalizeFinished()==TRUE) {
			m_nSubTimerAction = 200;
		}
		break;

//Water Path left
	case 200:
		if(theApp.m_LastSettings.nRelearnSurfacePos[0] == 1) {
			RefreshSideFromRelearn();
			theApp.m_Motors.GetHeadPos(&CpHead);
			if(theApp.m_nPrimarySide == 0) {
				if(theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]) == false) {
					m_nSubTimerAction = 999;
					MessageBox(L"Failed to measure a sensible water path\nBrane has aborted",L"Error",MB_ICONERROR);
					return;
				};

				theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
				theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			} else {
				if(theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]) == false) {
					m_nSubTimerAction = 999;
					MessageBox(L"Failed to measure a sensible water path\nBrane has aborted",L"Error",MB_ICONERROR);
					return;
				};

				theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,FALSE);
				theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
			}
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Thread.m_CpMoveTo = CpHead;
			theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
			Sleep(500);
			m_nSubTimerAction = 210;
		} else {
			m_nSubTimerAction = 300;
		}
		break;
	case 210:
		if(theApp.m_Thread.HasAnyTypeOfMoveToFinished()==TRUE) {
			m_nSubTimerAction = 300;
		}
		break;
//Move Right to Left
	case 300:
		if(theApp.m_LastSettings.nRelearnSurfacePos[1] == 1) {
			RefreshSideFromRelearn();
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Motors.GetHeadPos(&CpHead);
			if(theApp.m_nPrimarySide==0) {
				theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
				theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
				theApp.m_Kinematics.CalculateOppositeCoord(STARBOARD,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
				theApp.m_Kinematics.CalculateCompensationPos(&CpHead);
			} else {
				theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
				theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
				theApp.m_Kinematics.CalculateOppositeCoord(PORTSIDE,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
				theApp.m_Kinematics.CalculateCompensationPos(&CpHead);
			}
			theApp.m_Thread.m_CpMoveTo = CpHead;
			theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
			Sleep(500);
			m_nSubTimerAction += 10;
		} else {
			m_nSubTimerAction = 400;
		}
		break;
	case 310:
		if(theApp.m_Thread.HasAnyTypeOfMoveToFinished()==TRUE) {
			m_nSubTimerAction = 400;
		}
		break;

	case 400:
		if(theApp.m_LastSettings.nRelearnPeak[1] == 1) {
			RefreshSideFromRelearn();
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXRight;
			theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYRight;
			theApp.m_Thread.m_nNormAction = PEAK_OPPOSITE;
			theApp.m_Thread.m_nNormSide = STARBOARD;

			theApp.m_Thread.m_nThreadAction=PEAK_OPPOSITE;
			m_nSubTimerAction += 10;
			Sleep(200);
		} else {
			m_nSubTimerAction = 900;
		}

		break;
	case 410:
		if(theApp.m_Thread.HasPeakOppositeStarted()==true) {
			m_nSubTimerAction += 10;
		}
		break;
	case 420:
		if(theApp.m_Thread.HasPeakOppositeFinished()==TRUE) {
			m_nSubTimerAction = 900;
		}
		break;

	case 900:
		theApp.m_Thread.StopCurrentThreadAction();
		RefreshSideFromRelearn();
		PROFILE->ModifyBrainDeltas(m_nLine,m_nIndex,m_bSingleSided);
		pLine[m_nLine].m_pCp[m_nIndex].bModified=FALSE;
		Update3DViewDlg(NULL,NULL);
		m_nIndex++;
		m_nSubTimerAction=10;
		break;

	case 999:
		m_nTimerAction=0;
		m_nSubTimerAction=0;
		theApp.m_Thread.m_bEnableJoystick=TRUE;
		theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
		break;
	}


}

void CMainFrame::OptomiseCoordinate()
{

	CCoord CpSurface,CpHead,CpApproach,CpDanger,Cp;

	switch(m_nSubTimerAction) {
	case 0:
		theApp.m_Thread.m_bEnableJoystick=FALSE;
		theApp.m_Thread.StopCurrentThreadAction();
		m_nSubTimerAction=10;
		break;
	case 10:
		if(theApp.m_Thread.HasMovetoStraightLineFinished()==TRUE) {
			theApp.m_Motors.GetHeadPos(&CpHead);
			CpSurface=PROFILE->m_TaughtLine[theApp.m_nLine].m_pCp[theApp.m_nIndex];
			CpSurface.Side0.fR = CpHead.Side0.fR;

			theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
			theApp.m_Thread.m_CpMoveTo = CpHead;
			PROFILE->m_nScanSide = theApp.m_PmacUser.m_nScanSide = 2;
//			theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
			theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
			m_nSubTimerAction = 20;
			Sleep(200);
		}
		break;
	case 20:
		if(theApp.m_Thread.HasMovetoStraightLineFinished()==TRUE) {
			m_nSubTimerAction = 100;
		}
		break;
//Normalize Left
	case 100:
		if(theApp.m_LastSettings.nOptomiseNormals == 1) {
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXtLeft;
			theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYtLeft;
			theApp.m_Thread.m_nNormAction = NORMALIZE;
			theApp.m_Thread.m_nNormSide = theApp.m_nPrimarySide;
			theApp.m_Thread.m_nThreadAction=NORMALIZE;
			Sleep(200);
			m_nSubTimerAction = 110;
		} else {
			m_nSubTimerAction = 200;
		}
		break;
	case 110:
		if(theApp.m_Thread.HasNormalizeFinished()==TRUE) {
			m_nSubTimerAction = 200;
		}
		break;

//Water Path left
	case 200:
		if(theApp.m_LastSettings.nOptomiseWp == 1) {
			theApp.m_Motors.GetHeadPos(&CpHead);
			if(theApp.m_nSide0Orientation == 0) {
				theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
				theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
				theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			} else {
				theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
				theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,FALSE);
				theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
			}
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Thread.m_CpMoveTo = CpHead;
			theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
			Sleep(200);
			m_nSubTimerAction = 210;
		} else {
			m_nSubTimerAction = 300;
		}
		break;
	case 210:
		if(theApp.m_Thread.HasMovetoStraightLineFinished()==TRUE) {
			m_nSubTimerAction = 300;
		}
		break;
//Move Right to Left
	case 300:
		if(theApp.m_LastSettings.nOptomiseMoveRightToLeft == 1) {
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Motors.GetHeadPos(&CpHead);
			if(theApp.m_nSide0Orientation==0) {
				theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
				theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
				theApp.m_Kinematics.CalculateOppositeCoord(STARBOARD,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
				theApp.m_Kinematics.CalculateCompensationPos(&CpHead);
			} else {
				theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
				theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
				theApp.m_Kinematics.CalculateOppositeCoord(PORTSIDE,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
				theApp.m_Kinematics.CalculateCompensationPos(&CpHead);
			}
			theApp.m_Thread.m_CpMoveTo = CpHead;
			theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
			Sleep(200);
			m_nSubTimerAction = 310;
		} else {
			m_nSubTimerAction = 400;
		}
		break;
	case 310:
		if(theApp.m_Thread.HasMovetoStraightLineFinished()==TRUE) {
			m_nSubTimerAction = 400;
		}
		break;

//Peak Opposite

	/*case 400:
		for(m_nIndex;m_nIndex<PROFILE->m_BrainLine[m_nLine].m_nCoordL;m_nIndex++) {
			if(PROFILE->m_BrainLine[m_nLine].m_pCp[m_nIndex].bModified==TRUE) {
				theApp.m_Thread.StopCurrentThreadAction();

				theApp.m_Motors.GetHeadPos(&CpHead);

				CpSurface = pLine[m_nLine].m_pCp[m_nIndex];
//				PROFILE->CalculateBrainCoord(0,2,CpSurface);

				theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
				theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,PORTSIDE,TRUE,TRUE);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,STARBOARD,TRUE,TRUE);

				theApp.m_Thread.m_CpMoveTo = Cp;
				theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;

				m_nSubTimerAction = 410;
				Sleep(200);
				return;
			}
		}
		m_nLine++;
		m_nIndex=0;
		m_nSubTimerAction=10;
		break;
		*/

	case 400:
		if(theApp.m_LastSettings.nOptomiseOpposite == 1) {
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXRight;
			theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYRight;
			theApp.m_Thread.m_nNormAction = PEAK_OPPOSITE;
			theApp.m_Thread.m_nNormSide = STARBOARD;

			theApp.m_Thread.m_nThreadAction=PEAK_OPPOSITE;
			m_nSubTimerAction=410;
			Sleep(200);
		} else {
			m_nSubTimerAction = 900;
		}

		break;
	case 410:
		if(theApp.m_Thread.HasPeakOppositeFinished()==TRUE) {
			m_nSubTimerAction = 900;
		}
		break;

	case 900:
		theApp.m_Thread.StopCurrentThreadAction();
		PROFILE->ModifyTaughtCoordinate(theApp.m_nLine,theApp.m_nIndex);
		Update3DViewDlg(NULL,NULL);
		m_nSubTimerAction = 999;
		break;
	case 999:
		m_nTimerAction=0;
		m_nSubTimerAction=0;
		theApp.m_Thread.m_bEnableJoystick=TRUE;
		theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
		break;
	}



}



void CMainFrame::SetToolBarCheckedState(bool bAction)
{

	int	nStyle,nIndex;

	if(m_pToolBarMotion && m_pToolBarMotion->GetSafeHwnd()) {
		switch(theApp.m_nSide0Orientation) {
		case 0:

			theApp.m_Motors.ReportAirStatus(theApp.m_Tank.nLeftAirBit, 0) == FALSE ? nStyle = 0 : nStyle = TBBS_CHECKED;
			if((nIndex=m_pToolBarMotion->CommandToIndex(ID_BUTTON_LEFT_AIR))>=0) m_pToolBarMotion->SetButtonStyle(nIndex,nStyle);
			theApp.m_Motors.ReportAirStatus(theApp.m_Tank.nRightAirBit, 1) == FALSE ? nStyle = 0 : nStyle = TBBS_CHECKED;
			if((nIndex=m_pToolBarMotion->CommandToIndex(ID_BUTTON_RIGHT_AIR))>=0) m_pToolBarMotion->SetButtonStyle(nIndex,nStyle);

			break;
		case 1:

			theApp.m_Motors.ReportAirStatus(theApp.m_Tank.nLeftAirBit, 0) == FALSE ? nStyle = 0 : nStyle = TBBS_CHECKED;
			if((nIndex=m_pToolBarMotion->CommandToIndex(ID_BUTTON_RIGHT_AIR))>=0) m_pToolBarMotion->SetButtonStyle(nIndex,nStyle);
			theApp.m_Motors.ReportAirStatus(theApp.m_Tank.nRightAirBit, 1) == FALSE ? nStyle = 0 : nStyle = TBBS_CHECKED;
			if((nIndex=m_pToolBarMotion->CommandToIndex(ID_BUTTON_LEFT_AIR))>=0) m_pToolBarMotion->SetButtonStyle(nIndex,nStyle);

			break;
		}
	}

	if(m_pToolBarJoystick && m_pToolBarJoystick->GetSafeHwnd()) {
		theApp.m_LastSettings.nJoystickCommPort==1 ? nStyle = TBBS_CHECKED : nStyle = 0;
		if((nIndex=m_pToolBarJoystick->CommandToIndex(ID_BUTTON_INSIDE_JOYSTICK))>=0) m_pToolBarJoystick->SetButtonStyle(nIndex,nStyle);
		theApp.m_LastSettings.nJoystickCommPort==0 ? nStyle = TBBS_CHECKED : nStyle = 0;
		if((nIndex=m_pToolBarJoystick->CommandToIndex(ID_BUTTON_OUTSIDE_JOYSTICK))>=0) m_pToolBarJoystick->SetButtonStyle(nIndex,nStyle);
	}
	if(m_pToolBarTurntableRoller && m_pToolBarTurntableRoller->GetSafeHwnd()) {
		theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable==FALSE ? nStyle = 0 : nStyle = TBBS_CHECKED;
		if((nIndex=m_pToolBarTurntableRoller->CommandToIndex(ID_BUTTON_TURNTABLE_ENABLE))>=0) m_pToolBarTurntableRoller->SetButtonStyle(nIndex,nStyle);
		theApp.m_Axes[theApp.m_Tank.nRRight].bAvailable==FALSE ? nStyle = 0 : nStyle = TBBS_CHECKED;
		if((nIndex=m_pToolBarTurntableRoller->CommandToIndex(ID_BUTTON_ROLLERS_ENABLE))>=0) m_pToolBarTurntableRoller->SetButtonStyle(nIndex,nStyle);
	}

	if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable == TRUE) theApp.m_nUseTurntableOrRollers = 0;
	if(theApp.m_Axes[theApp.m_Tank.nRRight].bAvailable == TRUE) theApp.m_nUseTurntableOrRollers = 1;

	if(theApp.m_LastSettings.nToolBar[ROBOT_ROLLER_PROBE_TOOLBAR] > 0) {
		if(m_pToolBarMotion != NULL) {
			if((nIndex=m_pToolBarMotion->CommandToIndex(ID_BUTTON_WATER_SPRAY))>=0) {
				if(theApp.m_Ethercat.ReportWaterSpray() == 0) {
					m_pToolBarMotion->SetButtonStyle(nIndex,0);
				} else {
					m_pToolBarMotion->SetButtonStyle(nIndex,TBBS_CHECKED);
				}
			}
		}
	}


}



void CMainFrame::OnButtonLeftPump()
{


	switch(theApp.m_nSide0Orientation) {
	case 0:
		if(theApp.m_Motors.ReportPumpStatus(theApp.m_Tank.nLeftPump, 0)==FALSE) {
			theApp.m_Motors.SetPumpSpeed(theApp.m_Tank.nLeftPump, 0);
			theApp.m_Motors.PumpStart(theApp.m_Tank.nLeftPump, 0);
		} else {
			theApp.m_Motors.PumpStop(theApp.m_Tank.nLeftPump, 0);
		}
		break;
	case 1:
		if(theApp.m_Motors.ReportPumpStatus(theApp.m_Tank.nRightPump, 1)==FALSE) {
			theApp.m_Motors.SetPumpSpeed(theApp.m_Tank.nRightPump, 1);
			theApp.m_Motors.PumpStart(theApp.m_Tank.nRightPump, 1);
		} else {
			theApp.m_Motors.PumpStop(theApp.m_Tank.nRightPump, 1);
		}
		break;
	}
	Sleep(200);


}

void CMainFrame::OnButtonRightPump()
{


	switch(theApp.m_nSide0Orientation) {
	case 0:
		if(theApp.m_Motors.ReportPumpStatus(theApp.m_Tank.nRightPump, 1)==FALSE) {
			theApp.m_Motors.SetPumpSpeed(theApp.m_Tank.nRightPump, 1);
			theApp.m_Motors.PumpStart(theApp.m_Tank.nRightPump, 1);
		} else {
			theApp.m_Motors.PumpStop(theApp.m_Tank.nRightPump, 1);
		}
		break;
	case 1:
		if(theApp.m_Motors.ReportPumpStatus(theApp.m_Tank.nLeftPump, 0)==FALSE) {
			theApp.m_Motors.SetPumpSpeed(theApp.m_Tank.nLeftPump, 0);
			theApp.m_Motors.PumpStart(theApp.m_Tank.nLeftPump, 0);
		} else {
			theApp.m_Motors.PumpStop(theApp.m_Tank.nLeftPump, 0);
		}
		break;
	}
	Sleep(200);

}


void CMainFrame::OnButtonLeftAir()
{


	switch(theApp.m_nSide0Orientation) {
	case 0:
		if(theApp.m_Motors.ReportAirStatus(theApp.m_Tank.nLeftAirBit, 0)==FALSE) {
			theApp.m_Motors.AirStart(theApp.m_Tank.nLeftAirBit, 0);
		} else {
			theApp.m_Motors.AirStop(theApp.m_Tank.nLeftAirBit, 0);
		}
		break;
	case 1:
		if(theApp.m_Motors.ReportAirStatus(theApp.m_Tank.nRightAirBit, 1)==FALSE) {
			theApp.m_Motors.AirStart(theApp.m_Tank.nRightAirBit, 1);
		} else {
			theApp.m_Motors.AirStop(theApp.m_Tank.nRightAirBit, 1);
		}
		break;
	}
	Sleep(100);
//	SetToolBarCheckedState(TRUE);
}

void CMainFrame::OnButtonRightAir()
{


	switch(theApp.m_nSide0Orientation) {
	case 0:
		if(theApp.m_Motors.ReportAirStatus(theApp.m_Tank.nRightAirBit, 1)==FALSE) {
			theApp.m_Motors.AirStart(theApp.m_Tank.nRightAirBit, 1);
		} else {
			theApp.m_Motors.AirStop(theApp.m_Tank.nRightAirBit, 1);
		}
		break;
	case 1:
		if(theApp.m_Motors.ReportAirStatus(theApp.m_Tank.nLeftAirBit, 0)==FALSE) {
			theApp.m_Motors.AirStart(theApp.m_Tank.nLeftAirBit, 0);
		} else {
			theApp.m_Motors.AirStop(theApp.m_Tank.nLeftAirBit, 0);
		}
		break;
	}
	Sleep(100);
//	SetToolBarCheckedState(TRUE);
}


void CMainFrame::OnSystemUsers()
{

	if (m_pUsersDlg == NULL) {
		m_pUsersDlg = new CUsersDlg(this,(CWnd**)&m_pUsersDlg);
		m_pUsersDlg->Create(CUsersDlg::IDD, this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[CUsersDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pUsersDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}



void CMainFrame::OnSystemAccesslevels()
{

	if (m_pAccessLevelsDlg == NULL) {
		m_pAccessLevelsDlg = new CAccessLevelsDlg(this,(CWnd**)&m_pAccessLevelsDlg);
		m_pAccessLevelsDlg->Create(CAccessLevelsDlg::IDD, this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[CAccessLevelsDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pAccessLevelsDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CMainFrame::OnUpdateSystemAccesslevels(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}


void CMainFrame::OnUpdateSystemAssignaxes(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}


void CMainFrame::OnUpdateSystemCards(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateSystemDeviceresources(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}


void CMainFrame::OnUpdateSystemEditscandetails(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateSystemHoldingfixture(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateSystemImportthirdpartyimage(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateSystemKinematics(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateSystemOptions(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateSystemOrientationlanguage(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateSystemMachinedescription(CCmdUI* pCmdUI) 
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateSystemPmac(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateSystemUsers(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateProfilesEdit(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
	pCmdUI->SetCheck( m_p3DViewEditDlg > NULL);

}

void CMainFrame::OnUpdateProfilesImport(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateProfilesOpen(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateProfilesSaveas(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateUltrasonicsOpen(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateUltrasonicsOscilloscope(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateUltrasonicsOscilloscopepositionsShiftallleft(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateUltrasonicsOscilloscopepositionsShiftallright(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateUltrasonicsSaveas(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}


void CMainFrame::OnUpdateUltrasonicsSpectrumanalyzer(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateSystemSafetyinterlocks(CCmdUI* pCmdUI)
{

	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnProfilesIgesfileimport()
{


	if (m_pIGESDlg == NULL) {
		m_pIGESDlg = new CIGESDlg(this,(CWnd**)&m_pIGESDlg);
		m_pIGESDlg->Create(CIGESDlg::IDD, this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[CIGESDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pIGESDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}


void CMainFrame::OnFileDatabase()
{

	if (m_pDataBaseDlg == NULL) {
		m_pDataBaseDlg = new CDataBaseDlg(this,(CWnd**)&m_pDataBaseDlg);
		m_pDataBaseDlg->Create(CDataBaseDlg::IDD, this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[CDataBaseDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pDataBaseDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}


void CMainFrame::OnViewBmpviewer()
{

	if (m_pBMPViewerDlg == NULL) {
		m_pBMPViewerDlg = new CBMPViewerDlg(this,(CWnd**)&m_pBMPViewerDlg);
		m_pBMPViewerDlg->Create(CBMPViewerDlg::IDD, this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[CBMPViewerDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pBMPViewerDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}



void CMainFrame::OnMotionBisect()
{
	CBisectMoveDlg dlg;

	dlg.DoModal();
}

void CMainFrame::OnSystemSafetyinterlocks()
{

	if (m_pSafetyInterlocksDlg == NULL) {
		m_pSafetyInterlocksDlg = new CSafetyInterlocksDlg(this,(CWnd**)&m_pSafetyInterlocksDlg);
		m_pSafetyInterlocksDlg->Create(CSafetyInterlocksDlg::IDD, this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[CSafetyInterlocksDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pSafetyInterlocksDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}



BOOL CMainFrame::OnHelpInfo(HELPINFO* pHelpInfo)
{
	theApp.m_Help.Open((L"Welcome.html"),NULL);

	return CFrameWndEx::OnHelpInfo(pHelpInfo);
}


void CMainFrame::OnUltrasonicsPulserreceiver()
{
	if (m_pUltrasonicsSheet != NULL) {
		m_pUltrasonicsSheet->SendMessage(WM_CLOSE);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_ULTRASONICS_SHEET % MAXIMUMWINDOWSIDNUMBER]=FALSE;
	}

	if (m_pUltrasonicsSheet == NULL) {
		m_pUltrasonicsSheet = new CUltrasonicsSheet((L"Ultrasonics"), this, 0, (CWnd**)&m_pUltrasonicsSheet);
		if( m_pUltrasonicsSheet->Create(this) == 1 ) {
			theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_ULTRASONICS_SHEET % MAXIMUMWINDOWSIDNUMBER]=TRUE;
		} else {
			m_pUltrasonicsSheet = NULL;
		}
	} else {
		m_pUltrasonicsSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CMainFrame::OnUltrasonicsFilters()
{

	if (m_pFilterSheet == NULL) {
		m_pFilterSheet = new CFilterSheet((L"Filters"), this, 0, (CWnd**)&m_pFilterSheet);
		m_pFilterSheet->Create(this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_FILTER_SHEET % MAXIMUMWINDOWSIDNUMBER] = TRUE;
	};

}

void CMainFrame::OnSystemEditscandetails()
{
}


void CMainFrame::OnButtonCtrlPoint()
{
	m_CtrlType = (CtrlType)0;

}

void CMainFrame::OnButtonCtrlText()
{
	m_CtrlType = TEXT_CTL;

}

void CMainFrame::OnButtonCtrlEdit()
{
	m_CtrlType = EDIT_CTL;
}

void CMainFrame::OnUpdateButtonCtrlPoint(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_CtrlType == 0);

}

void CMainFrame::OnUpdateButtonCtrlText(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_CtrlType == TEXT_CTL);

}

void CMainFrame::OnUpdateButtonCtrlEdit(CCmdUI* pCmdUI)
{

	pCmdUI->SetCheck(m_CtrlType == EDIT_CTL);
}

void CMainFrame::OnButtonCtrlButton()
{
	m_CtrlType = BUTTON_CTL;
}

void CMainFrame::OnUpdateButtonCtrlButton(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_CtrlType == BUTTON_CTL);
}


void CMainFrame::OnButtonLeftAlign()
{
	if(m_pDetailsPage) {
		m_pDetailsPage->LeftAlign();
	}
}

void CMainFrame::OnUpdateButtonLeftAlign(CCmdUI* pCmdUI)
{
	bool bFlag = FALSE;

	if(m_pDetailsPage) {
		if(m_pDetailsPage->m_bShiftKeyDown) bFlag = TRUE;
	}
	pCmdUI->Enable(bFlag);
}

void CMainFrame::OnButtonTopAlign()
{
	if(m_pDetailsPage) {
		m_pDetailsPage->TopAlign();
	}
}

void CMainFrame::OnUpdateButtonTopAlign(CCmdUI* pCmdUI)
{
	bool bFlag = FALSE;

	if(m_pDetailsPage) {
		if(m_pDetailsPage->m_bShiftKeyDown) bFlag = TRUE;
	}
	pCmdUI->Enable(bFlag);
}


void CMainFrame::OnButtonRightAlign()
{
	if(m_pDetailsPage) {
		m_pDetailsPage->RightAlign();
	}
}

void CMainFrame::OnUpdateButtonRightAlign(CCmdUI* pCmdUI)
{
	bool bFlag = FALSE;

	if(m_pDetailsPage) {
		if(m_pDetailsPage->m_bShiftKeyDown) bFlag = TRUE;
	}
	pCmdUI->Enable(bFlag);
}

void CMainFrame::OnButtonSameWidth()
{
	if(m_pDetailsPage) {
		m_pDetailsPage->SameWidth();
	}
}

void CMainFrame::OnUpdateButtonSameWidth(CCmdUI* pCmdUI)
{
	bool bFlag = FALSE;

	if(m_pDetailsPage) {
		if(m_pDetailsPage->m_bShiftKeyDown) bFlag = TRUE;
	}
	pCmdUI->Enable(bFlag);
}

void CMainFrame::OnButtonSameHeight()
{
	if(m_pDetailsPage) {
		m_pDetailsPage->SameHeight();
	}
}

void CMainFrame::OnUpdateButtonSameHeight(CCmdUI* pCmdUI)
{
	bool bFlag = FALSE;

	if(m_pDetailsPage) {
		if(m_pDetailsPage->m_bShiftKeyDown) bFlag = TRUE;
	}
	pCmdUI->Enable(bFlag);
}

void CMainFrame::OnButtonSpaceVertically()
{
	if(m_pDetailsPage) {
		m_pDetailsPage->SpaceVertically();
	}
}

void CMainFrame::OnUpdateButtonSpaceVertically(CCmdUI* pCmdUI)
{
	bool bFlag = FALSE;

	if(m_pDetailsPage) {
		if(m_pDetailsPage->m_bShiftKeyDown) bFlag = TRUE;
	}
	pCmdUI->Enable(bFlag);
}


bool CMainFrame::ForceLogon()
{
	if(theApp.m_LastSettings.bForceLogon != FALSE) {
		if(m_LogonDlg.DoModal()==IDOK) {
	
		} else {
			OnClose();
			return FALSE;
		}
	}

	SetAccessPrivelages();
	return TRUE;
}


void CMainFrame::OnFileLogon()
{

	CString Buff;
	CLogonDlg dlg;

	dlg.DoModal();

	if(m_p3DViewEditDlg) m_p3DViewEditDlg->UpdateAllControls();
	if(m_pUltrasonicsSheet) m_pUltrasonicsSheet->UpdateAllPages();
	if(m_pMotionToolsSheet) m_pMotionToolsSheet->UpdateAllPages();

	SetToolBarCheckedState(TRUE);

	SetAccessPrivelages();

}

void CMainFrame::OnFileUpdateLicense()
{
	CEvaluationDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnFileLogoff()
{

	CString Buff;

	theApp.m_nLogonLevel = 0;
	theApp.m_nLogonLevelMask = 0;
	theApp.m_Operator = "Any One";

	if(m_p3DViewEditDlg) m_p3DViewEditDlg->UpdateAllControls();
	if(m_pUltrasonicsSheet) m_pUltrasonicsSheet->UpdateAllPages();
	if(m_pMotionToolsSheet) m_pMotionToolsSheet->UpdateAllPages();

	SetToolBarCheckedState(TRUE);


	SetAccessPrivelages();

}

void CMainFrame::CheckLogOffTimeOut()
{

	CTime CurrentTime = CTime::GetCurrentTime();


	if(theApp.m_bResetTimeOutTimer==TRUE) {
		m_osStartTimeOutTimer=(int)CurrentTime.GetTime();
	}
	theApp.m_bResetTimeOutTimer = FALSE;
	if(theApp.m_LastSettings.nTimoutPeriodMinutes>0) {
		if((((CurrentTime.GetTime() - m_osStartTimeOutTimer) / 60) >= theApp.m_LastSettings.nTimoutPeriodMinutes) && (theApp.m_nLogonLevel>0)) {
			OnFileLogoff();
		}
	}

}


void CMainFrame::OnUltrasonicsProbecharactersation() 
{
//	if(CheckValidLicence(L"ProbeCharacterisationLicence") != TRUE) return;

	if (m_pProbeCharacterisationSheet == NULL) {
		m_pProbeCharacterisationSheet = new CProbeCharacterisationSheet((L"Probe Characterisation"), this, 0, (CWnd**)&m_pProbeCharacterisationSheet);
		m_pProbeCharacterisationSheet->Create(this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_PROBE_CHARACTER_SHEET % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pProbeCharacterisationSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	
}

void CMainFrame::OnButtonPower() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnButtonAuto() 
{
	// TODO: Add your command handler code here
	
}


bool CMainFrame::CheckValidLicence(CString cstrLicence)
{

	MessageBox((L"You do not have a valid licence for this utility"),(L"Sorry"),NULL);
	return FALSE;

}


HRESULT CMainFrame::SetAccessPrivelages(WPARAM wp, LPARAM lp)
{
	bool bFlag = true;

	if(m_pUltrasonicsSheet && m_pUltrasonicsSheet->GetSafeHwnd()) m_pUltrasonicsSheet->SetAccessPrivelage();
	if(m_pProfileSheet && m_pProfileSheet->GetSafeHwnd()) m_pProfileSheet->SetAccessPrivelage();
	if(m_pMotionToolsSheet && m_pMotionToolsSheet->GetSafeHwnd()) m_pMotionToolsSheet->UpdateAllPages();
	if(m_pPaletteSheet && m_pPaletteSheet->GetSafeHwnd()) m_pPaletteSheet->UpdateAllPages();
	if(m_wndAxisPane && m_wndAxisPane.GetSafeHwnd()) m_wndAxisPane.SendMessage(UI_SET_ACCESS_PRIVELAGES);
	if (m_wndAxesPane && m_wndAxesPane.GetSafeHwnd()) m_wndAxesPane.SendMessage(UI_SET_ACCESS_PRIVELAGES);
	if (m_wndPropertiesPane && m_wndPropertiesPane.GetSafeHwnd()) m_wndPropertiesPane.SendMessage(UI_SET_ACCESS_PRIVELAGES);


	if((theApp.m_LastSettings.nScanMask & _LOCK_WP) && (theApp.m_pScanData != NULL)) {
		bFlag = false;
	}
/*	if(m_pToolBarManipulator) {
		pEdit = (CEdit* ) m_pToolBarManipulator->GetDlgItem(IDC_EDIT_LEFT_WATERPATH);
		if(pEdit) pEdit->EnableWindow(bFlag);
		pEdit = (CEdit* ) m_pToolBarManipulator->GetDlgItem(IDC_EDIT_RIGHT_WATERPATH);
		if(pEdit) pEdit->EnableWindow(bFlag);

		pSpin = (CSpinButtonCtrl*) m_pToolBarManipulator->GetDlgItem(IDC_SPIN_LEFT_WATERPATH);
		if(pSpin) pSpin->EnableWindow(bFlag);
		pSpin = (CSpinButtonCtrl*) m_pToolBarManipulator->GetDlgItem(IDC_SPIN_RIGHT_WATERPATH);
		if(pSpin) pSpin->EnableWindow(bFlag);
	} */
	return 0;
}




bool CMainFrame::TryToLoadProfile(CString FileName)
{
	CString PathName;
	CUSLFile File;
	CFileException e;

	PathName = theApp.m_ProfilePath + (L"\\") + FileName + (L".pro");

	if(!File.Open(PathName,CFile::modeReadWrite|CFile::typeBinary,&e)) {
		return false;
	} else {
		PROFILE->Retrieve(&File);
		if(theApp.m_LastSettings.nLoadUltrasonicsFromProfile==1) {
			PROFILE->CopyLocalUtToGlobal();
		}
		File.Close();
		ProfileRetrievedUpdateDialogs((WPARAM)this, NULL);
		return true;
	}


	return false;

}

void CMainFrame::OnButtonOrigin() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_nSide0Orientation==0 ? theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_LEFT : theApp.m_Thread.m_nThreadAction = ORIGIN_ALL_RIGHT;
}

void CMainFrame::OnButtonMovetoHome() 
{
	CCoord Cp;
	int m_nSide = 0;

	theApp.m_Motors.GetHeadPos(&Cp);
	switch(m_nSide) {
	case 0:
		Cp.Side0.fX=(float)theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize * -1.0f;
		Cp.Side0.fY=(float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
		Cp.Side0.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize * -1.0f;
		Cp.Side0.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize * -1.0f;
		Cp.Side0.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize * -1.0f;
		break;
	case 1:
		Cp.Side1.fX=(float)theApp.m_Axes[theApp.m_Tank.nXRight].nOffset * theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize * -1.0f;
		Cp.Side1.fY=(float)theApp.m_Axes[theApp.m_Tank.nYRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize * -1.0f;
		Cp.Side1.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZRight].nOffset * theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize * -1.0f;
		Cp.Side1.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset * theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize * -1.0f;
		Cp.Side1.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize * -1.0f;
		break;
	case 2:
		Cp.Side0.fX=(float)theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize * -1.0f;
		Cp.Side0.fY=(float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
		Cp.Side0.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize * -1.0f;
		Cp.Side0.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize * -1.0f;
		Cp.Side0.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize * -1.0f;
		Cp.Side1.fX=(float)theApp.m_Axes[theApp.m_Tank.nXRight].nOffset * theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize * -1.0f;
		Cp.Side1.fY=(float)theApp.m_Axes[theApp.m_Tank.nYRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize * -1.0f;
		Cp.Side1.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZRight].nOffset * theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize * -1.0f;
		Cp.Side1.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset * theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize * -1.0f;
		Cp.Side1.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize * -1.0f;
		break;
	}
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
}

void CMainFrame::OnSystemArchive() 
{
	if (m_pArchiveSheet == NULL) {
		m_pArchiveSheet = new CArchiveSheet((L"Archive"),this,0,(CWnd**)&m_pArchiveSheet);
		m_pArchiveSheet->Create( this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_ARCHIVE_SHEET % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pArchiveSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CMainFrame::OnUpdateButtonJoystick(CCmdUI* pCmdUI) 
{
	if(theApp.m_Motors.IsAlive() == false) {
		pCmdUI->Enable(FALSE);
	} else {
		pCmdUI->Enable(TRUE);
		theApp.m_Thread.m_nThreadAction == JOYSTICK_START ? pCmdUI->SetRadio(TRUE) : pCmdUI->SetRadio(FALSE);
	}
}

void CMainFrame::OnUpdateButtonSafe(CCmdUI* pCmdUI) 
{
	if(theApp.m_Motors.IsAlive() == false) {
		pCmdUI->Enable(FALSE);
	} else {
		theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
	}
}


void CMainFrame::OnUpdateSystemArchive(CCmdUI* pCmdUI) 
{
	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdatePaletteOpen(CCmdUI* pCmdUI) 
{
	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdatePaletteSaveas(CCmdUI* pCmdUI) 
{
	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnSystemMaintenancediagnostics() 
{
	if (m_pMaintenanceDiagnosticsSheet == NULL) {
		m_pMaintenanceDiagnosticsSheet = new CMaintenanceDiagnosticsSheet((L"Maintenance & Diagnostics"),this,0,(CWnd**)&m_pMaintenanceDiagnosticsSheet);
		m_pMaintenanceDiagnosticsSheet->Create( this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_MAINTENANCE_DIAGNOSTICS_SHEET % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pMaintenanceDiagnosticsSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CMainFrame::DisplayCADToolBar()
{
	CRect rect;
	CRect rr;

	m_pViewSheet[0]->GetWindowRect(rect);

	if(m_wndCADToolBar.m_hWnd==NULL) {
		if (!m_wndCADToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
			| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
			!m_wndCADToolBar.LoadToolBar(IDR_CAD_TOOLBAR))
		{
			TRACE0("Failed to create CAD toolbar\n");
			return ;      // fail to create
		}
	}
	m_wndCADToolBar.EnableDocking(CBRS_ALIGN_ANY);
	/*
	FloatControlBar(&m_wndCADToolBar,CPoint(0,0));
	m_wndCADToolBar.GetToolBarCtrl().SetRows(4,FALSE,&rr);
	m_wndCADToolBar.CalcDynamicLayout(rr.Width(),LM_HORZ | LM_COMMIT);
	FloatControlBar(&m_wndCADToolBar,CPoint(rect.right+40,rect.top),CBRS_ALIGN_ANY | CBRS_SIZE_DYNAMIC);
	*/
	ShowControlBar((CControlBar*)&m_wndCADToolBar,TRUE,FALSE);


}

void CMainFrame::OnButtonMouse() 
{
	theApp.m_nCADTool = CAD_MOUSE;
}

void CMainFrame::OnButtonLine() 
{
	theApp.m_nCADTool = CAD_LINE;
}

void CMainFrame::OnButtonEllipse() 
{
	theApp.m_nCADTool = CAD_ELLIPSE;
}

void CMainFrame::OnButtonMultiLine() 
{
	theApp.m_nCADTool = CAD_MULTI_LINE;
}

void CMainFrame::OnButtonMove() 
{
	theApp.m_nCADTool = CAD_MOVE_NODE;
}

void CMainFrame::OnUpdateButtonMouse(CCmdUI* pCmdUI) 
{

	pCmdUI->SetCheck(theApp.m_nCADTool == CAD_MOUSE);
}

void CMainFrame::OnUpdateButtonLine(CCmdUI* pCmdUI) 
{

	pCmdUI->SetCheck(theApp.m_nCADTool == CAD_LINE);
}

void CMainFrame::OnUpdateButtonEllipse(CCmdUI* pCmdUI) 
{

	pCmdUI->SetCheck(theApp.m_nCADTool == CAD_ELLIPSE);
}

void CMainFrame::OnUpdateButtonMultiLine(CCmdUI* pCmdUI) 
{

	pCmdUI->SetCheck(theApp.m_nCADTool == CAD_MULTI_LINE);
}

void CMainFrame::OnUpdateButtonMove(CCmdUI* pCmdUI) 
{

	pCmdUI->SetCheck(theApp.m_nCADTool == CAD_MOVE_NODE);
}

void CMainFrame::OnMotionKinematics() 
{
	CString Buff;

	if (m_pCompensationSheet == NULL) {
		Buff.LoadString(IDS_Compensation_Kinematics);
		m_pCompensationSheet = new CCompensationSheet(Buff,this,0,(CWnd**)&m_pCompensationSheet);
		m_pCompensationSheet->Create( this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_COMPENSATION_SHEET % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pMotionToolsSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CMainFrame::OnUpdateMotionKinematics(CCmdUI* pCmdUI) 
{
	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnProfilesImportdesigner() 
{
	if (m_pImportToolPathsDlg == NULL) {
		m_pImportToolPathsDlg = new CImportToolPathsDlg(this,(CWnd**)&m_pImportToolPathsDlg);
		m_pImportToolPathsDlg->Create(CImportToolPathsDlg::IDD, this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[CImportToolPathsDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pImportToolPathsDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}



void CMainFrame::OnUpdateUltrasonicsProbecharactersation(CCmdUI* pCmdUI) 
{
	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}


void CMainFrame::OnUpdateUltrasonicsLsabscan(CCmdUI* pCmdUI) 
{
	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateUltrasonicsFilters(CCmdUI* pCmdUI) 
{
	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}


void CMainFrame::OnUpdateProfilesImportdesigner(CCmdUI* pCmdUI) 
{
	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);	
}

void CMainFrame::OnUpdateProfilesIgesfileimport(CCmdUI* pCmdUI) 
{
	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);		
}

void CMainFrame::OnUpdateMotionBisect(CCmdUI* pCmdUI) 
{
	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);		
}

HRESULT CMainFrame::UpdateAxes(WPARAM, LPARAM)
{
	if(m_pAxesSheet != NULL) {
		m_pAxesSheet->UpdateAllPages();
	}
	return NULL;
}

void CMainFrame::OnUltrasonicsEnableethernet() 
{
	theApp.m_UtUser.EthernetConnect();
}



void CMainFrame::OnSystemAssignaxes()
{
	if (m_pAssignAxesDlg == NULL) {
		m_pAssignAxesDlg = new CAssignAxesDlg(this, (CWnd**)&m_pAssignAxesDlg);
		m_pAssignAxesDlg->Create(CAssignAxesDlg::IDD,this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[CAssignAxesDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pAssignAxesDlg->SendMessage(WM_CLOSE);
	}
}

void CMainFrame::Invalidate3DImages()
{
	if (m_pProfileSheet) {
		m_pProfileSheet->Invalidate3DImages();
	}

}


void CMainFrame::OnButtonTipsRemoveAir() 
{

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction=TIPS_REMOVE_AIR;
}

void CMainFrame::OnButtonTipsTo45() 
{

	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		Cp.Side0.fYt=-45.0f;
		Cp.Side1.fYt=45.0f;
		break;
	case SINGLE_BRIDGE_TANK_WITH_R:
	case SINGLE_BRIDGE_TANK:
		Cp.Side0.fXt=45.0f;
		break;
	case GOODRICH_MACHINE_4:
		Cp.Side0.fYt=45.0f;
		Cp.Side1.fYt=-45.0f;
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		theApp.m_Motors.GetSurfacePos(&Cp);
		Cp.Side0.norm = D3DXVECTOR4(-0.707f, 0.0f, 0.707f, 0.0f);
		Cp.Side1.norm = D3DXVECTOR4(0.707f, 0.0f, 0.707f, 0.0f);
		break;
	case TWIN_TOWER_KINEMATIC:
		theApp.m_Motors.GetSurfacePos(&Cp);
		Cp.Side0.norm = D3DXVECTOR4(0.0f, -0.707f, 0.707f, 0.0f);
		Cp.Side1.norm = D3DXVECTOR4(0.0f, 0.707f,  0.707f, 0.0f);
		break;
	case TRACKED_PROBE:
		return;
	}

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	
}
void CMainFrame::OnButtonTipsPointDown()
{
	CCoord Cp;

	theApp.m_Motors.GetHeadPos(&Cp);

	switch (theApp.m_Tank.nScannerDescription) {
	default:
		Cp.Side0.fYt = -90.0f;
		Cp.Side1.fYt = 90.0f;
		break;
	case SINGLE_BRIDGE_TANK_WITH_R:
	case SINGLE_BRIDGE_TANK:
		Cp.Side0.fXt = 0.0f;
		Cp.Side0.fYt = 0.0f;
		break;
	case GOODRICH_MACHINE_4:
	case TWI:
		Cp.Side0.fYt = 90.0f;
		Cp.Side1.fYt = -90.0f;
		break;

	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case TANK_6AXIS:
	case TANK_5AXIS:
		theApp.m_Motors.GetSurfacePos(&Cp);
		Cp.Side0.norm = D3DXVECTOR4(0.0f, 0.0f, -1.0f, 0.0f);
		Cp.Side1.norm = D3DXVECTOR4(0.0f, 0.0f, -1.0f, 0.0f);
		break;
	case TWIN_TOWER_KINEMATIC:
		theApp.m_Motors.GetSurfacePos(&Cp);
		Cp.Side0.norm = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 0.0f);
		Cp.Side1.norm = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 0.0f);
		break;
	case TRACKED_PROBE:
		return;
	}

	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;

}


void CMainFrame::OnButtonTipsToHorizontal() 
{
	CCoord Cp;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Motors.GetHeadPos(&Cp);
		Cp.Side0.fXt=0.0f;
		Cp.Side0.fYt=0.0f;
		Cp.Side1.fXt=0.0f;
		Cp.Side1.fYt=0.0f;
		break;
	case SINGLE_BRIDGE_TANK_WITH_R:
	case SINGLE_BRIDGE_TANK:
		theApp.m_Motors.GetHeadPos(&Cp);
		Cp.Side0.fXt=0.0f;
		Cp.Side0.fYt=0.0f;
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		theApp.m_Motors.GetSurfacePos(&Cp);
		Cp.Side0.norm=D3DXVECTOR4(-1.0f,0.0f,0.0f,0.0f);
		Cp.Side1.norm=D3DXVECTOR4(1.0f,0.0f,0.0f,0.0f);
		break;
	case TANK_6AXIS:
	case TANK_5AXIS:
	case TWIN_TOWER_KINEMATIC:
		theApp.m_Motors.GetSurfacePos(&Cp);
		Cp.Side0.norm = D3DXVECTOR4(0.0f, -1.0f, 0.0f, 0.0f);
		Cp.Side1.norm = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 0.0f);
		break;
	case TRACKED_PROBE:
		return;
	}
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
}

void CMainFrame::OnButtonSwivalToZero()
{
	CCoord Cp;

	switch (theApp.m_Tank.nScannerDescription) {
	default:
		break;
	case TANK_6AXIS:
	case TANK_5AXIS:
		theApp.m_Motors.SuspendMotors();
		theApp.m_Motors.AbsoluteMove(&theApp.m_Axes[3], 0);
		break;
	case TRACKED_PROBE:
		return;
	}
//	theApp.m_Thread.StopCurrentThreadAction();
//	theApp.m_Thread.m_CpMoveTo = Cp;
//	theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;
}

void CMainFrame::OnButtonInsideJoystick()
{
	if(theApp.m_LastSettings.nJoystickCommPort != 1) {
		theApp.m_LastSettings.nJoystickCommPort = 1;
		theApp.m_SerialJoystick.Close();
		theApp.m_SerialJoystick.Start();
		SetToolBarCheckedState(false);
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
	}
}

void CMainFrame::OnButtonOutsideJoystick() 
{


	if(theApp.m_LastSettings.nJoystickCommPort != 0) {
		theApp.m_LastSettings.nJoystickCommPort = 0;
		theApp.m_SerialJoystick.Close();
		theApp.m_SerialJoystick.Start();
		SetToolBarCheckedState(false);
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
	}
	
}

void CMainFrame::OnUpdateButtonInsideJoystick(CCmdUI* pCmdUI) 
{


//	theApp.m_LastSettings.nJoystickCommPort == 1 ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateButtonOutsideJoystick(CCmdUI* pCmdUI) 
{


//	theApp.m_LastSettings.nJoystickCommPort == 0 ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnButtonPulseEcho() 
{


	Pr30Data* pPr30 = &theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Pr30;

	if(pPr30->nPreampMode != 0) {
		pPr30->nPreampMode = 0;
		theApp.m_UtUser.Pr30PreampMode(pPr30);
		SetToolBarCheckedState(false);
		if(m_pUltrasonicsSheet) m_pUltrasonicsSheet->UpdateAllPages();
	};
}

void CMainFrame::OnButtonThroughTransmission() 
{


	Pr30Data* pPr30 = &theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Pr30;

	if(pPr30->nPreampMode != 1) {
		pPr30->nPreampMode = 1;
		theApp.m_UtUser.Pr30PreampMode(pPr30);
		SetToolBarCheckedState(false);
		if(m_pUltrasonicsSheet) m_pUltrasonicsSheet->UpdateAllPages();
	};
}

void CMainFrame::ChangeMeasurementUnits()
{


	if(m_nOldMeasurementUnits - theApp.m_LastSettings.nMeasurementUnits) {

		switch(theApp.m_LastSettings.nMeasurementUnits) {
		case 0: theApp.m_Units = "mm";
			theApp.m_fUnits = 1.0f;
			theApp.m_nUnitsPrecision = 1;
			theApp.m_nWpPrecision = 1;
			break;
		case 1: theApp.m_Units = "\x022";
			theApp.m_fUnits = 1.0f / 25.4f;
			theApp.m_nUnitsPrecision = 3;
			theApp.m_nWpPrecision = 2;
			break;
		}

		PostMessage(UI_UPDATE_DIALOG_BAR, NULL, NULL);
		if(m_pMotionToolsSheet) {
			m_pMotionToolsSheet->UpdateAllPages();
		}

		m_nOldMeasurementUnits = theApp.m_LastSettings.nMeasurementUnits;
	}
}

void CMainFrame::OnHelpUslscanneroperatormanual() 
{

	ShellExecute(NULL,L"Open",theApp.m_AcrobatPathName,theApp.m_USLScannerManualPathName,NULL,SW_SHOW);
	
}

void CMainFrame::OnHelpPmacsoftwaremanual() 
{


	ShellExecute(NULL,L"Open",theApp.m_AcrobatPathName,theApp.m_PMACSRMPathName,NULL,SW_SHOW);
}

void CMainFrame::OnViewWorkspace() 
{


	if (m_pWorkSpaceSheet == NULL) {
		m_pWorkSpaceSheet = new CWorkSpaceSheet(L"Work Space",this,0,(CWnd**)&m_pWorkSpaceSheet);
		m_pWorkSpaceSheet->Create( this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_WORKSPACE_SHEET % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pWorkSpaceSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

}

int CMainFrame::PointSizeFromHeight(int lfHeight)
{

	CDC *pDC = GetDC();

	int nLogPixelsY = pDC->GetDeviceCaps(LOGPIXELSY);
	int nPointSize = MulDiv(-lfHeight, 72, nLogPixelsY);
	ReleaseDC(pDC);

	return nPointSize;
}

int CMainFrame::lfHeightFromPointSize(int nPointSize)
{

	CDC *pDC = GetDC();

	int nLogPixelsY = pDC->GetDeviceCaps(LOGPIXELSY);
	int lfHeight = -MulDiv(nPointSize, nLogPixelsY, 72);
	ReleaseDC(pDC);

	return lfHeight;
}

void CMainFrame::OnSystemRobots() 
{

	CString Buff;

	if (m_pRobotSheet == NULL) {
		Buff.LoadString(IDS_Robotics);
		m_pRobotSheet = new CRobotSheet(Buff,this,0,(CWnd**)&m_pRobotSheet);
		m_pRobotSheet->Create( this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_ROBOT_SHEET % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pRobotSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

HRESULT CMainFrame::OnViewLasertracker(WPARAM, LPARAM) 
{

	if (m_pFaroCoordinateDlg == NULL) {
		m_pFaroCoordinateDlg = new CFaroCoordinateDlg(this,(CWnd**)&m_pFaroCoordinateDlg);
		m_pFaroCoordinateDlg->Create(CFaroCoordinateDlg::IDD, this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[CFaroCoordinateDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pFaroCoordinateDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	return NULL;
}


HRESULT CMainFrame::TrackerCoordinateAdded(WPARAM, LPARAM)
{

	if(m_pFaroCoordinateDlg != NULL) {
		m_pFaroCoordinateDlg->m_nCoordinateAddedCountDown = 10;
	}
	return NULL;
}

void CMainFrame::OnButtonPr300() 
{

	CString Warning, Buff;
	Warning.LoadString(IDS_Warning);
	int nWhich, ii;
	bool bFoundDevice=false;

	//BAE 5M has a switch on front to change between LSA and Single Probe (SP)
	//So 1st PM30 and 1st ADC input could be either
//	if(theApp.m_nJobNumber==BAE_5M_TANK) {
//		theApp.m_LastSettings.ADCInputDevice[0] = PM30_LSA;
//		theApp.m_LastSettings.nAttachedToPr30[0] = LSA;
//	}

	for(ii=0;ii<4;ii++){
		theApp.m_LastSettings.bUseWhichPM30[ii]=false;
		if((theApp.m_LastSettings.bPr30Present[ii])&&(!bFoundDevice)){
			if((theApp.m_LastSettings.nAttachedToPr30[ii]==PPA15)||(theApp.m_LastSettings.nAttachedToPr30[ii]==MUX8)){
				bFoundDevice=true;
				theApp.m_LastSettings.bUseWhichPM30[ii]=true;
				nWhich=ii;
			}
		}
	}

	if(bFoundDevice) {
		Buff.LoadString(IDS_Program_must_be_restarted_Proceed);
		theApp.m_LastSettings.nDesiredDevice = NOT_PHASED_ARRAY;
		if(MessageBox(Buff,Warning,MB_YESNO)==IDYES) {
			theApp.m_LastSettings.nUseWhichPR30 = nWhich;
			SendMessage(WM_CLOSE);
		}
	}else{
		Buff.LoadString(IDS_No_PM30_Configured_for_use_with_PPA_or_Mux);
		MessageBox(Buff,Warning,ERROR);
	}
//	if (m_pUltrasonicsSheet != NULL) m_pUltrasonicsSheet->SendMessage(UPDATE_TIMESLOTS);
}

void CMainFrame::OnButtonPr301() 
{

	CString Warning, Buff;
	Warning.LoadString(IDS_Warning);
	int ii, nWhich;
	bool bFoundDevice;


	//BAE 5M has a switch on front to change between LSA and Single Probe (SP)
	//So 1st PM30 and 1st ADC input could be either
//	if(theApp.m_nJobNumber==BAE_5M_TANK) {
//		theApp.m_LastSettings.ADCInputDevice[0] = PM30_LSA;
//		theApp.m_LastSettings.nAttachedToPr30[0] = LSA;
//	}


	for(ii=0;ii<4;ii++){
		theApp.m_LastSettings.bUseWhichPM30[ii]=false;
		if(theApp.m_LastSettings.bPr30Present[ii]){
			if(theApp.m_LastSettings.nAttachedToPr30[ii]==LSA){
				bFoundDevice=true;
				theApp.m_LastSettings.bUseWhichPM30[ii]=true; //So we know which PM30s to talk to
				nWhich=ii;
			}
		}
	}

	if(bFoundDevice) {
		theApp.m_LastSettings.nDesiredDevice = PHASED_ARRAY;
		Buff.LoadString(IDS_Program_must_be_restarted_Proceed);
		if(MessageBox(Buff,Warning,MB_YESNO)==IDYES) {
			theApp.m_LastSettings.nUseWhichPR30 = nWhich;
			SendMessage(WM_CLOSE);
		}
	}else{
		Buff.LoadString(IDS_No_PM30_Configured_for_use_with_LSA);
		MessageBox(Buff,Warning,ERROR);
	}
//	if (m_pUltrasonicsSheet != NULL) m_pUltrasonicsSheet->SendMessage(UPDATE_TIMESLOTS);
}

void CMainFrame::OnUpdateButtonPr300(CCmdUI* pCmdUI) 
{
	theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30]==LSA ? pCmdUI->SetCheck(0) : pCmdUI->SetCheck(1);
}

void CMainFrame::OnUpdateButtonPr301(CCmdUI* pCmdUI) 
{
	theApp.m_LastSettings.nAttachedToPr30[theApp.m_LastSettings.nUseWhichPR30]==LSA ? pCmdUI->SetCheck(1) : pCmdUI->SetCheck(0);
}

void CMainFrame::ZeroTheta()
{

	float fToolLength,fAngle;

	switch(m_nSubTimerAction) {
	case 0:
		theApp.m_Thread.m_bEnableJoystick=FALSE;
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Motors.GetHeadPos(&m_CpStart);
		switch(m_nRThetaZeroSide) {
		case PORTSIDE:
			m_CpStart.Side0.fXt = 0.0f;
			m_CpStart.Side0.fYt = 0.0f;
			break;
		case STARBOARD:
			m_CpStart.Side1.fXt = 0.0f;
			m_CpStart.Side1.fYt = 0.0f;
			break;
		}
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_CpMoveTo = m_CpStart;
		theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
		Sleep(200);

		m_nSubTimerAction=10;
		break;

	case 10:	//First peak routine
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Thread.StopCurrentThreadAction();
			switch(m_nRThetaZeroSide) {
			case PORTSIDE:
				theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXLeft;
				theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYLeft;
				theApp.m_Thread.m_nNormSide = PORTSIDE;
				break;
			case STARBOARD:
				theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXRight;
				theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYRight;
				theApp.m_Thread.m_nNormSide = STARBOARD;
				break;
			}

			theApp.m_Thread.m_pNormalizePage = NULL;
			theApp.m_Thread.m_pProbeBeamSectionsPage = NULL;
			theApp.m_Thread.m_nNormAction = PEAK_OPPOSITE;
			theApp.m_Thread.m_nThreadAction=PEAK_OPPOSITE;
			m_nSubTimerAction=20;
			Sleep(200);
		}

		break;
	case 20:
		if(theApp.m_Thread.HasPeakOppositeFinished()==TRUE) {
			theApp.m_Motors.GetHeadPos(&m_CpResult[0]);
			m_nSubTimerAction = 30;
		}
		break;


	case 30:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			switch(m_nRThetaZeroSide) {
			case PORTSIDE:
				m_CpStart.Side0.fXt = 180.0f;
				m_CpStart.Side0.fYt = 0.0f;
				break;
			case STARBOARD:
				m_CpStart.Side1.fXt = 180.0f;
				m_CpStart.Side1.fYt = 0.0f;
				break;
			}
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Thread.m_CpMoveTo = m_CpStart;
			theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
			Sleep(200);
			m_nSubTimerAction=40;
		}
		break;

	case 40:	//Second peak routine
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Thread.StopCurrentThreadAction();
			switch(m_nRThetaZeroSide) {
			case PORTSIDE:
				theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXLeft;
				theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYLeft;
				theApp.m_Thread.m_nNormSide = PORTSIDE;
				break;
			case STARBOARD:
				theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXRight;
				theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYRight;
				theApp.m_Thread.m_nNormSide = STARBOARD;
				break;
			}

			theApp.m_Thread.m_pNormalizePage = NULL;
			theApp.m_Thread.m_pProbeBeamSectionsPage = NULL;
			theApp.m_Thread.m_nNormAction = PEAK_OPPOSITE;
			theApp.m_Thread.m_nThreadAction=PEAK_OPPOSITE;
			m_nSubTimerAction=50;
			Sleep(200);
		}

		break;
	case 50:
		if(theApp.m_Thread.HasPeakOppositeFinished()==TRUE) {
			theApp.m_Motors.GetHeadPos(&m_CpResult[1]);
			m_nSubTimerAction = 60;
		}
		break;
	
	case 60:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {

			fToolLength = theApp.m_Kinematics.m_fDesiredWaterPath[m_nRThetaZeroSide] + theApp.m_Kinematics.m_fProbeLength[m_nRThetaZeroSide] + theApp.m_Kinematics.m_fArmLength[m_nRThetaZeroSide];

			switch(m_nRThetaZeroSide) {
			case PORTSIDE:
				m_CpStart.Side0.fZ = (m_CpResult[0].Side[m_nRThetaZeroSide].fZ+m_CpResult[1].Side[m_nRThetaZeroSide].fZ) / 2.0f;
				m_CpStart.Side0.fXt = 0.0f;
				fAngle = atan2f((m_CpResult[0].Side[m_nRThetaZeroSide].fZ-m_CpResult[1].Side[m_nRThetaZeroSide].fZ) / 2.0f, fToolLength);
				m_CpStart.Side0.fYt = fAngle * RAD_TO_DEG;
				 
				break;
			case STARBOARD:
				m_CpStart.Side0.fZ = (m_CpResult[1].Side[m_nRThetaZeroSide].fZ+m_CpResult[0].Side[m_nRThetaZeroSide].fZ) / 2.0f;
				m_CpStart.Side1.fXt = 0.0f;
				fAngle = atan2f((m_CpResult[1].Side[m_nRThetaZeroSide].fZ-m_CpResult[0].Side[m_nRThetaZeroSide].fZ) / 2.0f, fToolLength);
				m_CpStart.Side1.fYt = fAngle * RAD_TO_DEG;
				break;
			}
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Thread.m_CpMoveTo = m_CpStart;
			theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
			Sleep(200);
			m_nSubTimerAction=999;
		}
		break;

	case 999:
		if(theApp.m_Motors.IsMotionFinished() == TRUE) {
			theApp.m_Motors.SuspendMotors();
			theApp.m_Motors.EnableAxes(ENABLE_JOYSTICK);
			switch(m_nRThetaZeroSide) {
			case PORTSIDE:
				theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset = 0;
				Sleep(100);
				theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset = theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos;
				break;
			case STARBOARD:
				theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset = 0;
				Sleep(100);
				theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset = theApp.m_Axes[theApp.m_Tank.nYtRight].nPos;
				break;
			}

			m_nTimerAction=0;
			m_nSubTimerAction=0;
			theApp.m_Thread.m_bEnableJoystick=TRUE;
			theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
		}
		break;
	}
}




void CMainFrame::ReLearnTaughtOld()
{

	CCoord CpSurface,CpHead,CpApproach,CpDanger,Cp;
	CPolyCoord* pLine = PROFILE->m_TaughtLine;
	int nLineL = PROFILE->m_nTaughtLineL;
	int nSlowAxis=2;

	switch(m_nSubTimerAction) {
	case 0:
		theApp.m_nIndex=0;
		theApp.m_Thread.m_bEnableJoystick=FALSE;
		theApp.m_Thread.StopCurrentThreadAction();
		m_nLine = 0;
		m_nIndex = 0;
		m_nSubTimerAction=10;
		break;
	case 10:	//Find new line and new index
		for(m_nLine;m_nLine<nLineL;m_nLine++) {
			for(m_nIndex=0;m_nIndex<pLine[m_nLine].m_nCoordL;m_nIndex++) {
				if(pLine[m_nLine].m_pCp[m_nIndex].bModified==TRUE) {
					m_nSubTimerAction=20;
					return;
					break;
				}
			}
		}
		m_nSubTimerAction = 999;
		break;
	case 20:
		if(theApp.m_Thread.HasMovetoStraightLineFinished()==TRUE) {
			theApp.m_Thread.m_CpMoveTo = PROFILE->m_TaughtLine[m_nLine].m_pCp[m_nIndex];

			PROFILE->m_nScanSide = theApp.m_PmacUser.m_nScanSide = 2;
			theApp.m_Thread.m_nThreadAction = MOVETO_VIA_DANGER_PLANE;
			m_nSubTimerAction = 30;
			Sleep(200);
		}
		break;
	case 30:
		if(theApp.m_Thread.HasMovetoViaDangerPlaneFinished()==TRUE) {
			m_nSubTimerAction = 100;
		}
		break;
//Normalize Left
	case 100:
		if(theApp.m_LastSettings.nOptomiseNormals == 1) {
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXtLeft;
			theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYtLeft;
			theApp.m_Thread.m_nNormAction = NORMALIZE;
			theApp.m_Thread.m_nNormSide = theApp.m_nPrimarySide;
			theApp.m_Thread.m_nThreadAction=NORMALIZE;
			Sleep(200);
			m_nSubTimerAction = 110;
		} else {
			m_nSubTimerAction = 200;
		}
		break;
	case 110:
		if(theApp.m_Thread.HasNormalizeFinished()==TRUE) {
			m_nSubTimerAction = 200;
		}
		break;

//Water Path left
	case 200:
		if(theApp.m_LastSettings.nOptomiseWp == 1) {
			theApp.m_Motors.GetHeadPos(&CpHead);
			if(theApp.m_nPrimarySide == 0) {
				theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
				theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
				theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
			} else {
				theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
				theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,FALSE);
				theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
			}
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Thread.m_CpMoveTo = CpHead;
			theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
			Sleep(200);
			m_nSubTimerAction = 210;
		} else {
			m_nSubTimerAction = 300;
		}
		break;
	case 210:
		if(theApp.m_Thread.HasMovetoStraightLineFinished()==TRUE) {
			m_nSubTimerAction = 300;
		}
		break;
//Move Right to Left
	case 300:
		if(theApp.m_LastSettings.nOptomiseMoveRightToLeft == 1) {
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Motors.GetHeadPos(&CpHead);
			if(theApp.m_nPrimarySide==0) {
				theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
				theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
				theApp.m_Kinematics.CalculateOppositeCoord(STARBOARD,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
				theApp.m_Kinematics.CalculateCompensationPos(&CpHead);
			} else {
				theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
				theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
				theApp.m_Kinematics.CalculateOppositeCoord(PORTSIDE,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE);
				theApp.m_Kinematics.CalculateCompensationPos(&CpHead);
			}
			theApp.m_Thread.m_CpMoveTo = CpHead;
			theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
			Sleep(200);
			m_nSubTimerAction = 310;
		} else {
			m_nSubTimerAction = 400;
		}
		break;
	case 310:
		if(theApp.m_Thread.HasMovetoStraightLineFinished()==TRUE) {
			m_nSubTimerAction = 400;
		}
		break;

//Peak Opposite

	/*case 400:
		for(m_nIndex;m_nIndex<PROFILE->m_BrainLine[m_nLine].m_nCoordL;m_nIndex++) {
			if(PROFILE->m_BrainLine[m_nLine].m_pCp[m_nIndex].bModified==TRUE) {
				theApp.m_Thread.StopCurrentThreadAction();

				theApp.m_Motors.GetHeadPos(&CpHead);

				CpSurface = pLine[m_nLine].m_pCp[m_nIndex];
//				PROFILE->CalculateBrainCoord(0,2,CpSurface);

				theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
				theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,PORTSIDE,TRUE,TRUE);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&Cp,STARBOARD,TRUE,TRUE);

				theApp.m_Thread.m_CpMoveTo = Cp;
				theApp.m_Thread.m_nThreadAction = MOVETO_STRAITE_LINE;

				m_nSubTimerAction = 410;
				Sleep(200);
				return;
			}
		}
		m_nLine++;
		m_nIndex=0;
		m_nSubTimerAction=10;
		break;
		*/

	case 400:
		if(theApp.m_LastSettings.nOptomiseOpposite == 1) {
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXRight;
			theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYRight;
			theApp.m_Thread.m_nNormAction = PEAK_OPPOSITE;
			theApp.m_Thread.m_nNormSide = STARBOARD;

			theApp.m_Thread.m_nThreadAction=PEAK_OPPOSITE;
			m_nSubTimerAction=410;
			Sleep(200);
		} else {
			m_nSubTimerAction = 900;
		}

		break;
	case 410:
		if(theApp.m_Thread.HasPeakOppositeFinished()==TRUE) {
			m_nSubTimerAction = 900;
		}
		break;

	case 900:
		theApp.m_Thread.StopCurrentThreadAction();

		theApp.m_Motors.GetHeadPos(&CpHead);
		switch(theApp.m_LastSettings.nPositionDisplayMode) {
		default:
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE);
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE);
			break;
		case 3:
			theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
			theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
			theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,FALSE);
			break;
		}
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
		theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);
		theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface,PORTSIDE);
		theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface,STARBOARD);
		theApp.m_UtUser.GetAllUt(&CpSurface);
		pLine[m_nLine].m_pCp[m_nIndex] = CpSurface;
		pLine[m_nLine].m_pCp[m_nIndex].bModified=FALSE;

		Update3DViewDlg(NULL, NULL);
		m_nIndex++;
		m_nSubTimerAction=10;
		break;

	case 999:
		m_nTimerAction=0;
		m_nSubTimerAction=0;
		theApp.m_Thread.m_bEnableJoystick=TRUE;
		theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
		break;
	}


}

void CMainFrame::ReLearnFieldBus(int nLineType)
{
	CCoord CpSurface,CpHead,CpApproach,CpDanger,Cp;
	CPolyCoord* pLine = NULL;
	int nLineL;
	int nSlowAxis=2;
	float fWp;

	switch(nLineType) {
	case TAUGHTLINES:
		pLine = PROFILE->m_TaughtLine;
		nLineL = PROFILE->m_nTaughtLineL;
		break;
	case BRAINLINES:
		pLine = PROFILE->m_BrainLine;
		nLineL = PROFILE->m_nBrainLineL;
		break;
	default: return;
		break;
	}

	switch(m_nSubTimerAction) {
	case 0:
		theApp.m_nIndex=0;
		theApp.m_Thread.m_bEnableJoystick=FALSE;
		theApp.m_Thread.StopCurrentThreadAction();
		m_nLine = 0;
		m_nIndex = 0;
		m_nSubTimerAction=10;
		break;
	case 10:	//Find new line and new index
		for(m_nLine;m_nLine<nLineL;m_nLine++) {
			for(m_nIndex=0;m_nIndex<pLine[m_nLine].m_nCoordL;m_nIndex++) {
				if(pLine[m_nLine].m_pCp[m_nIndex].bModified==TRUE) {
					m_nSubTimerAction=20;
					return;
					break;
				}
			}
		}
		m_nSubTimerAction = 999;
		break;
	case 20:
		if(theApp.m_Thread.HasIntelligentGotoFinished()==TRUE) {
			CpSurface = pLine[m_nLine].m_pCp[m_nIndex];
			theApp.m_Motors.IntelligentGotoSurfaceCoord(&CpSurface, 3, theApp.m_LastSettings.nDangerPlaneMoveMask);
			m_nSubTimerAction += 10;
			Sleep(200);
		}
		break;
	case 30:
		if(theApp.m_Thread.HasIntelligentGotoFinished()==TRUE) {
			m_nSubTimerAction = 100;
		}
		break;
//Normalize Left
	case 100:
		if(theApp.m_LastSettings.nRelearnNormals[0] == 1) {
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXtLeft;
			theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYtLeft;
			theApp.m_Thread.m_nNormAction = NORMALIZE;
			theApp.m_Thread.m_nNormSide = theApp.m_nPrimarySide;
			theApp.m_Thread.m_nThreadAction=NORMALIZE;
			Sleep(200);
			m_nSubTimerAction = 110;
		} else {
			m_nSubTimerAction = 200;
		}
		break;
	case 110:
		if(theApp.m_Thread.HasNormalizeFinished()==TRUE) {
			m_nSubTimerAction = 200;
		}
		break;

//Water Path left
	case 200:
		if(theApp.m_LastSettings.nRelearnSurfacePos[0] == 1) {
			theApp.m_Motors.GetSurfacePos(&CpSurface);
			theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE],&fWp);
			fWp -= theApp.m_Kinematics.m_fDesiredWaterPath[PORTSIDE];				//Checked okay
			CpSurface.Side[PORTSIDE].fX -= CpSurface.Side[PORTSIDE].fI * fWp;
			CpSurface.Side[PORTSIDE].fY -= CpSurface.Side[PORTSIDE].fJ * fWp;
			CpSurface.Side[PORTSIDE].fZ -= CpSurface.Side[PORTSIDE].fK * fWp;

			theApp.m_Thread.m_CpMoveTo = CpSurface;
			theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
			Sleep(200);
			m_nSubTimerAction = 210;
		} else {
			m_nSubTimerAction = 300;
		}
		break;
	case 210:
		if(theApp.m_Thread.HasMovetoStraightLineFinished()==TRUE) {
			m_nSubTimerAction = 300;
		}
		break;
//Move Right to Left
	case 300:
		if(theApp.m_LastSettings.nRelearnSurfacePos[1] == 1) {
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Motors.GetSurfacePos(&CpSurface);
			theApp.m_Kinematics.CalculateOppositeCoord(STARBOARD,&CpSurface,theApp.m_Kinematics.m_fComponentThickness);
			theApp.m_Motors.IntelligentGotoSurfaceCoord(&CpSurface, 2, 0);
			Sleep(200);
			m_nSubTimerAction = 310;
		} else {
			m_nSubTimerAction = 400;
		}
		break;
	case 310:
		if(theApp.m_Thread.HasIntelligentGotoFinished()==TRUE) {
			m_nSubTimerAction = 400;
		}
		break;

//Peak Opposite

	case 400:
		if(theApp.m_LastSettings.nRelearnPeak[1] == 1) {
			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Thread.m_nNormAxis0 = theApp.m_Tank.nXRight;
			theApp.m_Thread.m_nNormAxis1 = theApp.m_Tank.nYRight;
			theApp.m_Thread.m_nNormAction = PEAK_OPPOSITE;
			theApp.m_Thread.m_nNormSide = STARBOARD;

			theApp.m_Thread.m_nThreadAction=PEAK_OPPOSITE;
			m_nSubTimerAction=410;
			Sleep(200);
		} else {
			m_nSubTimerAction = 900;
		}

		break;
	case 410:
		if(theApp.m_Thread.HasPeakOppositeFinished()==TRUE) {
			m_nSubTimerAction = 900;
		}
		break;

	case 900:
		theApp.m_Thread.StopCurrentThreadAction();

		switch (nLineType) {
		case TAUGHTLINES:
			theApp.m_Motors.GetSurfacePos(&CpSurface);
			theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface, PORTSIDE);
			theApp.m_Kinematics.GetDesiredWaterPath(&CpSurface, STARBOARD);
			theApp.m_UtUser.GetAllUt(&CpSurface);
			pLine[m_nLine].m_pCp[m_nIndex] = CpSurface;
			pLine[m_nLine].m_pCp[m_nIndex].bModified = FALSE;
			break;
		case BRAINLINES:
			theApp.m_Thread.StopCurrentThreadAction();
			PROFILE->ModifyBrainDeltas(m_nLine, m_nIndex, m_bSingleSided);
			pLine[m_nLine].m_pCp[m_nIndex].bModified = FALSE;
			break;
		}

		Update3DViewDlg(NULL,NULL);
		m_nIndex++;
		m_nSubTimerAction=10;
		break;

	case 999:
		m_nTimerAction=0;
		m_nSubTimerAction=0;
		theApp.m_Thread.m_bEnableJoystick=TRUE;
		theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
		break;
	}


}

void CMainFrame::OnButtonBladeZeroDegrees() 
{


	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo.Side0.fR = 0.0f;
	theApp.m_Thread.m_nThreadAction=BLADE_TO_DEGREES;
	
}

void CMainFrame::OnUpdateButtonBladeZeroDegrees(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMainFrame::OnButtonBlade180Degrees() 
{


	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo.Side0.fR = 180.0f;
	theApp.m_Thread.m_nThreadAction=BLADE_TO_DEGREES;
	
}

void CMainFrame::OnUpdateButtonBlade180Degrees(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CMainFrame::OnButtonBladeHome() 
{


	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction=BLADE_GO_HOME;
}

void CMainFrame::OpenChildView(int nImageType, int nImageNumber, CData *pData, int nTS, int nGate)
{


}

void CMainFrame::OnButtonTurntableEnable() 
{

	
	if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable==FALSE) {
		theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable=TRUE;
		theApp.m_Axes[theApp.m_Tank.nRRight].bAvailable=FALSE;
	} else {
		theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable=FALSE;
	}
	SetToolBarCheckedState(false);
}

void CMainFrame::OnUpdateButtonTurntableEnable(CCmdUI* pCmdUI) 
{
	
}

void CMainFrame::OnButtonRollersEnable() 
{

	
	if(theApp.m_Axes[theApp.m_Tank.nRRight].bAvailable==FALSE) {
		theApp.m_Axes[theApp.m_Tank.nRRight].bAvailable=TRUE;
		theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable=FALSE;
	} else {
		theApp.m_Axes[theApp.m_Tank.nRRight].bAvailable=FALSE;
	}
	SetToolBarCheckedState(false);
}

void CMainFrame::OnUpdateButtonRollersEnable(CCmdUI* pCmdUI) 
{


//	pCmdUI->Enable(theApp.m_Axes[theApp.m_Tank.nRRight].bAvailable);
}

HRESULT CMainFrame::UpdateRobotSheet(WPARAM,LPARAM)
{
	if (m_pRobotSheet != NULL) {
		m_pRobotSheet->UpdateAllPages();
	}
	return NULL;
}

void CMainFrame::OnSystemEditProperties() 
{
	if (m_pDataPropertiesDlg == NULL) {
		m_pDataPropertiesDlg = new CDataPropertiesDlg(this,(CWnd**)&m_pDataPropertiesDlg,&theApp.m_Data);
		m_pDataPropertiesDlg->Create(CDataPropertiesDlg::IDD,this);
	} else {
		m_pDataPropertiesDlg->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	
}

void CMainFrame::OnSystemEditDetails() 
{

	CRect rr;


	if (m_pDetailsPage == NULL) {
		m_pDetailsPage = new CDetailsPage((CWnd**)&m_pDetailsPage,&theApp.m_Data,TRUE);
		m_pDetailsPage->Create(CDetailsPage::IDD);
		theApp.m_LastSettings.bWindowOpenOnLastExit[CDetailsPage::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pDetailsPage->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

	CRect rect = theApp.m_LastSettings.RectWindowPos[CDetailsPage::IDD];

	if(m_wndCtrlToolBar.m_hWnd==NULL) {
		if (!m_wndCtrlToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
			| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
			!m_wndCtrlToolBar.LoadToolBar(IDR_CONTROL_TOOLBAR))
		{
			TRACE0("Failed to create Motion toolbar\n");
			return ;      // fail to create
		}
	}
	m_wndCtrlToolBar.EnableDocking(CBRS_ALIGN_ANY);
	/*
	FloatControlBar(&m_wndCtrlToolBar,CPoint(0,0));
	m_wndCtrlToolBar.GetToolBarCtrl().SetRows(4,FALSE,&rr);
	m_wndCtrlToolBar.CalcDynamicLayout(rr.Width(),LM_HORZ | LM_COMMIT);
	FloatControlBar(&m_wndCtrlToolBar,CPoint(rect.right+40,rect.top),CBRS_ALIGN_ANY | CBRS_SIZE_DYNAMIC);
	*/

	ShowControlBar((CControlBar*)&m_wndCtrlToolBar,TRUE,FALSE);


}



void CMainFrame::OnUltrasonicsPhasedarrayprobe() 
{

	CString Buff;

	Buff.LoadString(IDS_Phased_array);

	if (m_pPhasedArraySheet == NULL) {
		m_pPhasedArraySheet = new CPhasedArraySheet(Buff, this, theApp.m_LastSettings.nLastPhasedArrayTab, (CWnd**)&m_pPhasedArraySheet);
		if (m_pPhasedArraySheet->Create(this)) {
			theApp.m_LastSettings.bWindowOpenOnLastExit[CPhasedArrayProbeDlg::IDD % MAXIMUMWINDOWSIDNUMBER] = TRUE;
		}
		else {
			SAFE_DELETE(m_pPhasedArraySheet);
		}
	}
	else {
		m_pPhasedArraySheet->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}

}

void CMainFrame::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFrameWndEx::OnRButtonDown(nFlags, point);
}

void CMainFrame::OnButtonPaEqualWaterPath() 
{

	CCoord Cp;
	D3DXVECTOR3 vecP[2];
	float fWp[2];
	bool bInterfaceFound[2];
	int nFl,nGStart;
	float fAngle,fLength,fDeltaWp;

	bInterfaceFound[0] = false;
	for(nFl=0;(nFl<theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws()) && (bInterfaceFound[0]==false);nFl++) {
		nGStart = theApp.m_UtUser.m_TS[nFl].Gate.nTOFStartGate[0];
		if(theApp.m_UtUser.m_TS[nFl].Gate.nTimeStatus[nGStart]>0) {
			fWp[0]=theApp.m_UtUser.GetGateWaterPath(nFl);
			bInterfaceFound[0] = true;
			vecP[0] = theApp.m_PhasedArray[PORTSIDE].getFocalLawPos(RX_FL, nFl);
		}
	}
	for(nFl;(nFl<theApp.m_PhasedArray[PORTSIDE].getNumberFocalLaws());nFl++) {
		nGStart = theApp.m_UtUser.m_TS[nFl].Gate.nTOFStartGate[0];
		if(theApp.m_UtUser.m_TS[nFl].Gate.nTimeStatus[nGStart]>0) {
			fWp[1]=theApp.m_UtUser.GetGateWaterPath(nFl);
			bInterfaceFound[1] = true;
			vecP[1] = theApp.m_PhasedArray[PORTSIDE].getFocalLawPos(RX_FL, nFl);
		}
	}

	if((bInterfaceFound[0] == true) && (bInterfaceFound[1] == true)) {

		fLength = D3DXVec3Length(&(vecP[0] - vecP[1]));
		fDeltaWp = fWp[1] - fWp[0];
		fAngle = atan2f(fDeltaWp,fLength) * RAD_TO_DEG;

		theApp.m_Motors.GetHeadPos(&Cp);
		Cp.Side0.fXt-=fAngle;
		theApp.m_Thread.StopCurrentThreadAction();
		theApp.m_Thread.m_CpMoveTo = Cp;
		theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
	}
}

HRESULT CMainFrame::UpdatePaletteSheet(WPARAM wp,LPARAM lp)
{
	if (wp != NULL) {
		SAFE_DELETE(m_pPaletteSheet);
		PaletteEdit();
	}
	else {
		if (m_pPaletteSheet != NULL) {
			m_pPaletteSheet->UpdateAllPages();
		}
	}
	return NULL;
}

HRESULT CMainFrame::UpdatePmacSheet(WPARAM,LPARAM)
{
	if (m_pPmacSheet != NULL) {
		m_pPmacSheet->UpdateAllPages();
	}
	return NULL;
}

void CMainFrame::RedrawUltrasonicsSheet()
{
	if(m_pUltrasonicsSheet) m_pUltrasonicsSheet->RedrawAllPages();
}

void CMainFrame::Customize()
{
	if (m_pCustomizeSheet == NULL) {
		m_pCustomizeSheet = new CCustomizeSheet((L"Customize"), this, 0, (CWnd**)&m_pCustomizeSheet);
		m_pCustomizeSheet->Create(this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_CUSTOMIZE_SHEET % MAXIMUMWINDOWSIDNUMBER] = TRUE;
	}
	else {
		m_pCustomizeSheet->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

void CMainFrame::OnSystemCustomize()
{
	Customize();
}

void CMainFrame::OnUpdateSystemCustomize(CCmdUI* pCmdUI)
{
	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);
}

void CMainFrame::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if ((nHitTest == 2) && (theApp.m_bCtrlButton == true)) {
		Customize();
	}

	CWnd::OnNcLButtonDown(nHitTest, point);
}

void CMainFrame::OnNcRButtonDown(UINT nHitTest, CPoint point)
{
	Customize();

	CWnd::OnNcLButtonDown(nHitTest, point);
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	CRect rr;
//	CStatic* pStatic;

	switch(pMsg->message) {
	case 0x201://LBUTTONDOWN
//		TRACE3(L"Msg->%08x wParam->%08x lParam->%08x\r\n",pMsg->message,pMsg->wParam, pMsg->lParam);
//		TRACE2(L" X:%d Y:%d\r\n", pMsg->pt.x, pMsg->pt.y);
		switch(pMsg->wParam) {
		case 1:	
/*
			if(m_pDialogBarPumps) {
				pStatic = (CStatic*)m_pDialogBarPumps->GetDlgItem(IDC_STATIC_PUMP_0);
				pStatic->GetWindowRect(rr);
				if(rr.PtInRect(pMsg->pt)) {
					OnButtonLeftPump();
					return true;
				}
				pStatic = (CStatic*)m_pDialogBarPumps->GetDlgItem(IDC_STATIC_PUMP_1);
				pStatic->GetWindowRect(rr);
				if(rr.PtInRect(pMsg->pt)) {
					OnButtonRightPump();
					return true;
				}
			}
*/
			break;
		}
		break;

	case 0x202:	//LBUTTONUP
//		TRACE2(L"Msg->%d Param->%d\r\n",pMsg->message,pMsg->wParam);
		break;
		
	}


	return CFrameWndEx::PreTranslateMessage(pMsg);
}

void CMainFrame::OnViewBlade() 
{

	if (m_pViewBladeThicknessDlg[0] == NULL) {
		m_pViewBladeThicknessDlg[0] = new CViewBladeThickneessDlg(this,(CWnd**)&m_pViewBladeThicknessDlg[0]);
		m_pViewBladeThicknessDlg[0]->Create(CViewBladeThickneessDlg::IDD, this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[CViewBladeThickneessDlg::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pViewBladeThicknessDlg[0]->SetWindowPos(&wndTop,0,0,0,0,NULL);
	}
	
}


void CMainFrame::OnButtonWaterSpray() 
{
	theApp.m_FBCtrl.ToggleRRWaterSpray();
	Sleep(100);
	SetToolBarCheckedState(false);
}



void CMainFrame::InitializeProfileSheetPtrs()
{
	if(m_pProfileSheet!=NULL) {
		m_pProfileSheet->UpdateAllPages();
	};

}

HRESULT CMainFrame::UpdatePhasedArraySheet(WPARAM wp,LPARAM lp)
{
	if(m_pPhasedArraySheet!=NULL) {
		m_pPhasedArraySheet->UpdateActivePage();
	};
	return NULL;
}

LRESULT CMainFrame::RemoteHandler(WPARAM Wp, LPARAM Lp)
{
	if(m_pMotionToolsSheet!=NULL) m_pMotionToolsSheet->SendMessage(REMOTE_HANDLER,Wp);
	return 0;
}

LRESULT CMainFrame::RemoteHandlerProfile(WPARAM Wp, LPARAM Lp)
{
	if(m_pProfileSheet == NULL){
		OnProfilesEdit();
	}
	m_pProfileSheet->SendMessage(REMOTE_HANDLER,Wp);
	return 0;
}

LRESULT CMainFrame::RemoteHandlerUltrasonics(WPARAM Wp, LPARAM Lp)
{
	if(m_pUltrasonicsSheet != NULL) m_pUltrasonicsSheet->SendMessage(REMOTE_HANDLER,Wp);
	return 0;
}

LRESULT CMainFrame::RemoteHandlerFLSC10(WPARAM Wp, LPARAM Lp)
{
	if(m_pMaintenanceDiagnosticsSheet != NULL) m_pMaintenanceDiagnosticsSheet->PostMessage(REMOTE_HANDLER_FLSC10, Wp);
	return 0;
}

LRESULT CMainFrame::RemoteHandlerEgismosRF(WPARAM Wp, LPARAM Lp)
{
	if(m_pMaintenanceDiagnosticsSheet != NULL) m_pMaintenanceDiagnosticsSheet->PostMessage(REMOTE_HANDLER_EGISMOSRF, Wp);
	return 0;
}

LRESULT CMainFrame::RemotePageUpdateControls(WPARAM Wp, LPARAM Lp)
{
	if (m_pMaintenanceDiagnosticsSheet != NULL) m_pMaintenanceDiagnosticsSheet->PostMessage(REMOTE_PAGE_UPDATE_CONTROLS, Wp);
	return 0;
}

void CMainFrame::OnButtonStepOffOn() 
{
	CCoord CpSurface;	

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	theApp.m_Motors.GotoSurfaceCoord(&CpSurface,MOVE_PORTSIDE);
}

void CMainFrame::OnButtonPreWetRoller() 
{
	if(PROFILE->m_nScanLineL<=0) {
		MessageBox(L"There is no profile loaded",L"Error",MB_ICONERROR);
		return;
	}
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction=PRE_WET_ROLLER_PROBE;

}

void CMainFrame::OnButtonEnablePowerToRobots() 
{


	AndonEventType AndonFlag = MachineWarning;
	
	theApp.AndonEvent(AndonFlag, L"Kill power to robots in Mainframe");
	
	theApp.m_FBCtrl.KillMotors();
	Sleep(100);
	theApp.m_FBCtrl.EnableMotors();

	AndonFlag = MachineOkay;

	theApp.AndonEvent(AndonFlag, L"Enable power to robots in Mainframe");
	
}

void CMainFrame::OnViewLineexpansion() 
{


	if (m_pPolyCoordDlg == NULL) {
		m_pPolyCoordDlg = new CPolyCoordDlg(this, (CWnd**)&m_pPolyCoordDlg, NULL, 0 ,0);
		m_pPolyCoordDlg->Create(CPolyCoordDlg::IDD,this);
	} else {
		m_pPolyCoordDlg->SetWindowPos(&wndTop,0,0,0,0,NULL);
	}
	
}


void CMainFrame::OnUpdateButtonXyzMode(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(theApp.m_Axes[theApp.m_Tank.nYLeft].bAvailable == 1);
}


void CMainFrame::OnUpdateButtonYzrMode(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable == 1);
}

void CMainFrame::RefreshAxesSheet()
{
	if (m_pAxesSheet) {
		m_pAxesSheet->SendMessage(WM_CLOSE);
		m_pAxesSheet = new CAxesSheet((L"Axes"), this, 0, (CWnd**)&m_pAxesSheet);
		m_pAxesSheet->Create(this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_AXES_SHEET % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	}
}


void CMainFrame::OnUltrasonicsInclusions() 
{

	CString Buff;

	if (m_pInclusionsSheet == NULL) {
		m_pInclusionsSheet = new CInclusionsSheet(L"Inclusions",this,0,(CWnd**)&m_pInclusionsSheet, &theApp.m_Inclusions,0);
		m_pInclusionsSheet->Create( this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_INCLUSIONS_SHEET % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pInclusionsSheet->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CMainFrame::OnButtonMarkerPen() 
{
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_nThreadAction=DO_MARKER_PEN;
}


void CMainFrame::OnUpdateButtonMarkerPen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}


HRESULT CMainFrame::CloseAllImages(WPARAM,LPARAM)
{
	for(int nn=0;nn<MAX_NUMBER_VIEWS;nn++) {
		SAFE_DELETE(m_pViewSheet[nn] );
	}

	m_pScanView = NULL;
	theApp.m_pScanData = NULL;
	return NULL;
}

int CMainFrame::StartScan(int nScanType,bool bForceNewWindow)
{

	CString Buff,Message,Warning,Error;
	Message.LoadString(IDS_Scan_is_going);
	Warning.LoadString(IDS_Warning);
	Error.LoadString(IDS_Error);
	int nn;

	for(nn=0;nn<MAX_NUMBER_VIEWS;nn++) {
		if((m_pViewSheet[nn]) && (bForceNewWindow == false)) {

			if(m_pViewSheet[nn] == m_pScanView) {
				if(m_pScanView->IsContinueScanAllowable()) {
					Buff.Format((L"%s %d"),Message,PROFILE->m_nScanResumeLine+1);
					if(MessageBox(Buff,Warning,MB_YESNO)==IDYES) {
						theApp.m_Thread.m_nScanStartLine = PROFILE->m_nScanResumeLine;
						break;
					} else {
						return -1;
					}
				} else {
					Buff.LoadString(IDS_Profile_parameters);
					MessageBox(Buff,Error,MB_ICONERROR);
					return -1;
				}
			}

		} else {
			if(m_pViewSheet[nn] == NULL) {
				if(theApp.m_LastSettings.nScanMask & _LOAD_UT_FILE_AT_SCAN_START) PROFILE->LoadDesiredUtFile();
				m_pViewSheet[nn] = new CViewSheet(theApp.m_Data.m_FileName,this,0,(CWnd**)&m_pViewSheet[nn],NULL);
				m_pViewSheet[nn]->InitializeFromProfile(nScanType);
				if(m_pViewSheet[nn]->InitializeFromUt(nScanType) == false) {
					SAFE_DELETE( m_pViewSheet[nn] );
					return -1;
				};
				m_pViewSheet[nn]->AllocatePages(nScanType);
				m_pViewSheet[nn]->Create(this);
				m_pScanView=m_pViewSheet[nn];

				theApp.m_pScanData = &m_pScanView->m_Data;
				theApp.m_pScanData->m_FileName = theApp.m_Data.m_FileName;
				theApp.m_pScanData->m_FilePath = theApp.m_Data.m_FilePath;
				theApp.m_pScanData->SaveScanLines();
				theApp.m_pScanData->AllocateToolPathAndOffsets();
				theApp.m_pScanData->m_DataProperties = &theApp.m_Data.m_DataProperties;
				theApp.m_pScanData->AutoFillDataProperties();
				theApp.m_pScanData->CopyInclusions(&theApp.m_Inclusions);
				PROFILE->SetScanStartTime();


				if(theApp.m_LastSettings.nReverseScanLines == 0) {
					theApp.m_Thread.m_nScanStartLine = PROFILE->m_nScanStartLine;
				} else {
					theApp.m_Thread.m_nScanStartLine = PROFILE->m_nScanFinishLine;
					if(PROFILE->m_nLineIncrement > 1) theApp.m_Thread.m_nScanStartLine -= PROFILE->m_nLineIncrement;
				}

				theApp.m_Kinematics.CalculateProbeSampleOffsets(theApp.m_pScanData);

				SetAccessPrivelages();
				if(theApp.m_LastSettings.nScanMask & _FORCE_PROPERTY_PAGE) m_pViewSheet[nn]->FileProperties();
				break;
			}
		};
	}
	if(nn>=MAX_NUMBER_VIEWS) {
		MessageBox(NULL,(L"Too Many Views Allocated"),NULL);
		return -1;
	}

	theApp.m_Thread.StopCurrentThreadAction();

	switch(theApp.m_LastSettings.nScanType = nScanType) {
	case CONTOUR_SCAN:
		theApp.m_Thread.m_nThreadAction=CONTOUR_SCAN;
		theApp.ActivityLog(L"Contour scan started");
		break;
	case TURNTABLE_SCAN:
		theApp.m_Thread.m_nThreadAction=TURNTABLE_SCAN;
		theApp.ActivityLog(L"Turntable scan started");
		break;
	case CONTINUOUS_TURNTABLE_SCAN:
		theApp.m_Thread.m_nThreadAction=CONTINUOUS_TURNTABLE_SCAN;
		theApp.ActivityLog(L"Continuous turntable scan started");
		break;
	case SEGMENT_TURNTABLE_SCAN:
		theApp.m_Thread.m_nThreadAction=SEGMENT_TURNTABLE_SCAN;
		theApp.ActivityLog(L"Segment turntable scan started");
		break;
	case ROBOT_CONTOUR_SCAN:
		theApp.m_Thread.m_nThreadAction=ROBOT_CONTOUR_SCAN;
		theApp.ActivityLog(L"Robot Contour scan started");
		break;
	case MULTI_PROBE_SCAN:
		theApp.m_Thread.m_nThreadAction=MULTI_PROBE_SCAN;
		theApp.ActivityLog(L"Multi Probe scan started");
		break;
	case ROBOT_ROLLER_PROBE_SCAN:
		theApp.m_Thread.m_nThreadAction=ROBOT_ROLLER_PROBE_SCAN;
		theApp.ActivityLog(L"Robot Roller probe scan started");
		break;
	}

	return nn;
}


void CMainFrame::ContinueScan(int nViewNumber)
{

	CString Buff,Message,Warning,Error;
	Error.LoadString(IDS_Error);
	CRect rr;

	theApp.m_Thread.StopCurrentThreadAction();

	if(m_pViewSheet[nViewNumber] != NULL) {
		m_pScanView = m_pViewSheet[nViewNumber];
		theApp.m_pScanData = &m_pScanView->m_Data;
		theApp.m_Thread.m_nScanStartLine = PROFILE->m_nScanResumeLine;

		m_pScanView->GetWindowRect(rr);
		m_pScanView->SetWindowPos(&wndTop,rr.left,rr.top,rr.Width(),rr.Height(),SWP_NOMOVE | SWP_NOSIZE);
	} else {
		m_bPauseWorksheetScan = true;
		MessageBox(L"The scan could not be restarted as it no longer exists",Error,MB_ICONERROR);
		return;
	}


	switch(theApp.m_LastSettings.nScanType) {
	case CONTOUR_SCAN:
		theApp.m_Thread.m_nThreadAction=CONTOUR_SCAN;
		theApp.ActivityLog(L"Contour scan started");
		break;
	case TURNTABLE_SCAN:
		theApp.m_Thread.m_nThreadAction=TURNTABLE_SCAN;
		theApp.ActivityLog(L"Turntable scan started");
		break;
	case CONTINUOUS_TURNTABLE_SCAN:
		theApp.m_Thread.m_nThreadAction=CONTINUOUS_TURNTABLE_SCAN;
		theApp.ActivityLog(L"Continuous turntable scan started");
		break;
	case SEGMENT_TURNTABLE_SCAN:
		theApp.m_Thread.m_nThreadAction=SEGMENT_TURNTABLE_SCAN;
		theApp.ActivityLog(L"Segment turntable scan started");
		break;
	case ROBOT_CONTOUR_SCAN:
		theApp.m_Thread.m_nThreadAction=ROBOT_CONTOUR_SCAN;
		theApp.ActivityLog(L"Robot Contour scan started");
		break;
	case MULTI_PROBE_SCAN:
		theApp.m_Thread.m_nThreadAction=MULTI_PROBE_SCAN;
		theApp.ActivityLog(L"Multi Probe scan started");
		break;
	case ROBOT_ROLLER_PROBE_SCAN:
		theApp.m_Thread.m_nThreadAction=ROBOT_ROLLER_PROBE_SCAN;
		theApp.ActivityLog(L"Robot Roller probe scan started");
		break;
	}
}


void CMainFrame::WorkSheetScan()
{
	CString Buff;

	if(m_bPauseWorksheetScan == true) {
		PROFILE->m_nScanResumeLine = theApp.m_Thread.m_nScanLine;
		m_nTimerAction = 0;
		m_nSubTimerAction = 0;
		theApp.m_Thread.m_bAbortFlag = true;
		return;
	}

	switch(m_nSubTimerAction) {
	case 0:
		if(m_pMotionToolsSheet) {
			m_pMotionToolsSheet->KillAllTimers();
		}
		switch(m_nPlayListIndex = theApp.m_WS.GetNextPlayListItem()) {
		case -1:
			theApp.m_WS.CheckSwitchOffPumps();
			m_nSubTimerAction = 0;
			m_nTimerAction = 0;
			return;
			break;
		default:
			theApp.m_WS.SelectProfile(m_nProfileIndex = theApp.m_WS.m_pPlayList[m_nPlayListIndex].nProfileIndex);

			switch(	theApp.m_WS.m_pPlayList[m_nPlayListIndex].nStatus) {
			case 0:
				PROFILE->m_nScanStartLine = 0;
				PROFILE->m_nScanFinishLine = PROFILE->m_nScanLineL-1;
				PROFILE->m_nCollectAxis = PROFILE->m_nFastAxis;
				theApp.m_Data.m_FilePath = theApp.m_WS.m_pPlayList[m_nPlayListIndex].strComponentPathName;
				theApp.m_Data.m_FileName = theApp.m_WS.m_pPlayList[m_nPlayListIndex].strComponentFileName;

				m_nWSCurrentView = StartScan(PROFILE->m_nScanType = CONTOUR_SCAN, true);

				theApp.m_WS.m_pPlayList[m_nPlayListIndex].nStatus = _SCANNING;
				UpdateWorkSpace();
				break;
			case _SCANNING: ContinueScan(m_nWSCurrentView);
				break;
			};
			break;
		}
		m_nSubTimerAction = 10;
		break;
	case 10:
		if(theApp.m_Thread.IsScanning() == false) {
			theApp.m_WS.m_pPlayList[m_nPlayListIndex].nStatus = _FINISHED;

			if(theApp.m_pScanData->Save() == true) {

				theApp.m_WS.m_pPlayList[m_nPlayListIndex].nStatus = _SAVED;
			}

			UpdateWorkSpace();
			m_nSubTimerAction = 0;
		}
		if(theApp.m_WS.IsCalibrationExpired() == true && theApp.m_WS.IsPartitionScan()==true) {
			theApp.m_Thread.m_bAbortFlag = true;
			m_nSubTimerAction = 20;		//need to do standard
		}
		break;
	case 20:
		if(theApp.m_Thread.IsScanning() == false) {

			//Archive this so the scan can restart
			PROFILE->m_nScanResumeLine = (theApp.m_Thread.m_nScanLine - 2) & 0xfffe;
			if(PROFILE->m_nScanResumeLine < 0) PROFILE->m_nScanResumeLine = 0;

			//do a new standard
			theApp.m_WS.SelectProfile(theApp.m_WS.m_pPlayList[0].nProfileIndex);
			theApp.m_WS.m_pPlayList[0].nStatus = 0;
			PROFILE->m_nScanStartLine = 0;
			PROFILE->m_nScanFinishLine = PROFILE->m_nScanLineL-1;
			PROFILE->m_nCollectAxis = PROFILE->m_nFastAxis;

			theApp.m_Data.m_FilePath = theApp.m_WS.m_pPlayList[0].strComponentPathName;
			for(int nn=0;nn<MAX_NUMBER_VIEWS;nn++) {
				if(m_pViewSheet[nn] == NULL) {
					Buff.Format(L"-%d",nn+1);
					theApp.m_Data.m_FileName = theApp.m_WS.m_pPlayList[0].strComponentFileName + Buff;
					break;
				}
			}

			theApp.m_Data.m_FileName = theApp.m_WS.m_pPlayList[0].strComponentFileName;
			theApp.m_Thread.m_bAbortFlag = false;

			if(StartScan(PROFILE->m_nScanType = CONTOUR_SCAN, true) == -1) {
				m_bPauseWorksheetScan = true;
				MessageBox(L"Failed to start calibration scan",L"Error",MB_ICONERROR);
			}
			theApp.m_WS.m_pPlayList[0].nStatus = _SCANNING;
			UpdateWorkSpace();
			m_nSubTimerAction = 30;
		}
		break;
	case 30:
		if(theApp.m_Thread.IsScanning() == true) {
			m_nSubTimerAction = 40;
		}
		break;
	case 40:
		if(theApp.m_Thread.IsScanning() == false) { //check standard results
			theApp.m_WS.m_pPlayList[0].nStatus = _FINISHED;

			theApp.m_bCtrlButton = true;
			m_pScanView->SetActivePage(theApp.m_WS.m_nRefImage);
			if(theApp.m_pScanData->BuildHistogramData(AMP_IMAGE,theApp.m_WS.m_nRefImage,theApp.m_WS.m_rrHistogramSample) == TRUE) {

				if(theApp.m_WS.AddToRefArrayAndCheck(theApp.m_pScanData->m_Hist.fRefAmp) == true) {
					theApp.m_WS.SetCalibrationTime();
					m_nSubTimerAction = 0;
					UpdateWorkSpace();

					theApp.m_WS.SelectProfile(m_nProfileIndex);
					ContinueScan(m_nWSCurrentView);
					m_nSubTimerAction = 10;
				} else {
					m_bPauseWorksheetScan = true;
					UpdateWorkSpace();
					MessageBox(L"Calibration out of range",L"Error",MB_ICONERROR);
				}

			} else {
				MessageBox(L"Failed to calculate histogram data",L"Error",MB_ICONERROR);
				m_bPauseWorksheetScan = true;
			}
			theApp.m_bCtrlButton = false;

		}
		break;

	}
}


void CMainFrame::UpdateWorkSpace()
{
	if (m_pWorkSpaceSheet != NULL) {
		m_pWorkSpaceSheet->UpdateAllPages();
	}

}

void CMainFrame::WorkSheetSingleScan()
{
	if(m_bPauseWorksheetScan == true) {
		m_nTimerAction = 0;
		m_nSubTimerAction = 0;
		theApp.m_Thread.m_bAbortFlag = true;
		return;
	}

	switch(m_nSubTimerAction) {
	case 0:
		m_nPlayListIndex = theApp.m_WS.m_nPlayListIndex;
		theApp.m_WS.SelectProfile(m_nProfileIndex = theApp.m_WS.m_pPlayList[m_nPlayListIndex].nProfileIndex);

		switch(	theApp.m_WS.m_pPlayList[m_nPlayListIndex].nStatus) {
		case 0:
			PROFILE->m_nScanStartLine = 0;
			PROFILE->m_nScanFinishLine = PROFILE->m_nScanLineL-1;
			PROFILE->m_nCollectAxis = PROFILE->m_nFastAxis;
			theApp.m_Data.m_FilePath = theApp.m_WS.m_pPlayList[m_nPlayListIndex].strComponentPathName;
			theApp.m_Data.m_FileName = theApp.m_WS.m_pPlayList[m_nPlayListIndex].strComponentFileName;

			m_nWSSingleCurrentView = StartScan(PROFILE->m_nScanType = CONTOUR_SCAN, true);

			theApp.m_WS.m_pPlayList[m_nPlayListIndex].nStatus = _SCANNING;
			UpdateWorkSpace();
			break;
		case _SCANNING:
		case _FINISHED:
			PROFILE->m_nScanResumeLine,PROFILE->m_nScanStartLine;
			ContinueScan(m_nWSSingleCurrentView);
			break;
		};
		m_nSubTimerAction = 10;
		break;
	case 10:
		if(theApp.m_Thread.IsScanning() == false) {
			theApp.m_WS.m_pPlayList[m_nPlayListIndex].nStatus = _FINISHED;
			UpdateWorkSpace();
			m_nTimerAction = 0;
			m_nSubTimerAction = 0;
		}
		break;
	}
}

void CMainFrame::CheckHT(bool bEStop)
{
	if(theApp.m_UtUser.m_TS[0].Pr30.nHtOffOn == 1) {

		if(theApp.m_LastSettings.nHTMask & _HT_OFF_PUMPS_OFF) {
			if((theApp.m_Motors.ReportPumpStatus(theApp.m_Tank.nLeftPump, 0) == false) && (theApp.m_Motors.ReportPumpStatus(theApp.m_Tank.nRightPump, 1) == false)) {
				HTOffOn(0);
			}
		}

		if(theApp.m_LastSettings.nHTMask & _HT_OFF_MACHINE_ZERO) {
			if(theApp.m_Motors.IsMachineZero() == true) {
				HTOffOn(0);
			}
		}

		if(theApp.m_LastSettings.nHTMask & _HT_OFF_E_STOP && bEStop == true) {
			HTOffOn(0);
		}

	}
}

void CMainFrame::OnButtonUsefullHt()
{
	switch(theApp.m_UtUser.m_TS[0].Pr30.nHtOffOn) {
	case 0:
		theApp.m_UtUser.m_TS[0].Pr30.nHtOffOn=1;
		theApp.m_UtUser.m_Global.LSA.nPulsarOnOff=1;
		if((theApp.m_LastSettings.bPr20Present) || (theApp.m_LastSettings.bPr50Present) ) {
			theApp.m_UtUser.Pr2050PulseVoltage(&theApp.m_UtUser.m_TS[0].Pr20,1);
			theApp.m_UtUser.Pr2050PulseVoltage(&theApp.m_UtUser.m_TS[0].Pr50,1);
		}
		break;
	case 1:
		theApp.m_UtUser.m_TS[0].Pr30.nHtOffOn=0;
		theApp.m_UtUser.m_Global.LSA.nPulsarOnOff=0;
		if((theApp.m_LastSettings.bPr20Present) || (theApp.m_LastSettings.bPr50Present) ) {
			theApp.m_UtUser.Pr2050PulseVoltage(&theApp.m_UtUser.m_TS[0].Pr20,0);
			theApp.m_UtUser.Pr2050PulseVoltage(&theApp.m_UtUser.m_TS[0].Pr50,0);
		}
		break;
	}
	for(int nT=0;nT<8;nT++) {
		theApp.m_UtUser.m_TS[nT].Pr30.nHtOffOn=theApp.m_UtUser.m_TS[0].Pr30.nHtOffOn;
	}
	theApp.m_UtUser.Pr30HtOffOn(&theApp.m_UtUser.m_TS[0].Pr30);
	theApp.m_UtUser.MailboxHt(theApp.m_UtUser.m_TS[0].Pr30.nHtOffOn & 0x01,theApp.m_UtUser.m_TS[0].Pr30.nPulseVoltage);
	if((theApp.m_LSA.IsConnected()) && (theApp.m_LastSettings.nDesiredDevice==PHASED_ARRAY)) {
		theApp.m_LSA.CmdSetHTOnOff(theApp.m_UtUser.m_Global.LSA.nPulsarOnOff & 0x01);
	}
	SetToolBarCheckedState(FALSE);
	if(m_pUltrasonicsSheet) m_pUltrasonicsSheet->UpdateAllPages();

}

void CMainFrame::OnUpdateButtonUsefullHt(CCmdUI* pCmdUI)
{

//	pCmdUI->SetRadio(theApp.m_UtUser.m_TS[0].Pr30.nHtOffOn);
	if (theApp.m_UtUser.m_TS[0].Pr30.nHtOffOn == 1) {
		pCmdUI->SetCheck(TRUE);
	}
	else {
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnUpdateHTOffOn(CCmdUI* pCmdUI)
{

}

void CMainFrame::HTOffOn(int nHtOffOn)
{

	switch(theApp.m_UtUser.m_TS[0].Pr30.nHtOffOn=nHtOffOn) {
	case 0:
		if((theApp.m_LastSettings.bPr20Present) || (theApp.m_LastSettings.bPr50Present) ) {
			theApp.m_UtUser.Pr2050PulseVoltage(&theApp.m_UtUser.m_TS[0].Pr20,1);
			theApp.m_UtUser.Pr2050PulseVoltage(&theApp.m_UtUser.m_TS[0].Pr50,1);
		}
		break;
	case 1:
		if((theApp.m_LastSettings.bPr20Present) || (theApp.m_LastSettings.bPr50Present) ) {
			theApp.m_UtUser.Pr2050PulseVoltage(&theApp.m_UtUser.m_TS[0].Pr20,0);
			theApp.m_UtUser.Pr2050PulseVoltage(&theApp.m_UtUser.m_TS[0].Pr50,0);
		}
		break;
	}
	for(int nT=0;nT<8;nT++) {
		theApp.m_UtUser.m_TS[nT].Pr30.nHtOffOn=theApp.m_UtUser.m_TS[0].Pr30.nHtOffOn;
	}
	theApp.m_UtUser.Pr30HtOffOn(&theApp.m_UtUser.m_TS[0].Pr30);
	theApp.m_UtUser.MailboxHt(theApp.m_UtUser.m_TS[0].Pr30.nHtOffOn & 0x01,theApp.m_UtUser.m_TS[0].Pr30.nPulseVoltage);
	SetToolBarCheckedState(FALSE);
	if(m_pUltrasonicsSheet) m_pUltrasonicsSheet->UpdateAllPages();


}

void CMainFrame::OnSystemQuaternions() 
{

	if (m_pQuaternionsPage == NULL) {
		m_pQuaternionsPage = new CQuaternionPage(this,(CWnd**)&m_pQuaternionsPage);
		m_pQuaternionsPage->Create(CQuaternionPage::IDD, this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[CQuaternionPage::IDD % MAXIMUMWINDOWSIDNUMBER]=TRUE;
	} else {
		m_pQuaternionsPage->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
}

void CMainFrame::OnUpdateButtonLeftAir(CCmdUI* pCmdUI) 
{
	theApp.m_Motors.ReportAirStatus(theApp.m_Tank.nLeftAirBit, 0) == 1 ? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
}

void CMainFrame::OnUpdateButtonRightAir(CCmdUI* pCmdUI) 
{
	theApp.m_Motors.ReportAirStatus(theApp.m_Tank.nRightAirBit, 1) == 1 ? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
}

void CMainFrame::OnUpdateButtonLeftPump(CCmdUI* pCmdUI) 
{
	int nAxis, nSide;
	if (theApp.m_nSide0Orientation == 0) {
		nSide = 0;
		nAxis = theApp.m_Tank.nLeftPump;
	}
	else {
		nSide = 1;
		nAxis = theApp.m_Tank.nRightPump;
	}
	theApp.m_Motors.ReportPumpStatus(nAxis, nSide) == 1 ? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
}

void CMainFrame::OnUpdateButtonRightPump(CCmdUI* pCmdUI) 
{
	int nAxis, nSide;
	if (theApp.m_nSide0Orientation == 0) {
		nSide = 1;
		nAxis = theApp.m_Tank.nRightPump;
	}
	else {
		nSide = 0;
		nAxis = theApp.m_Tank.nLeftPump;
	}
	theApp.m_Motors.ReportPumpStatus(nAxis, nSide) == 1 ? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
}


void CMainFrame::OnButtonMovetoSurfaceZero() 
{
	CCoord CpSurface;
	int nSide = 2;

	theApp.m_Motors.GetSurfacePos(&CpSurface);

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		theApp.m_Thread.StopCurrentThreadAction();
		switch(nSide) {
		case 0:
			CpSurface.Side[PORTSIDE].fYt = CpSurface.Side[PORTSIDE].fXt =  CpSurface.Side[PORTSIDE].fZ = CpSurface.Side[PORTSIDE].fY = CpSurface.Side[PORTSIDE].fX = 0.0f;
			break;
		case 1:
			CpSurface.Side[STARBOARD].fYt = CpSurface.Side[STARBOARD].fXt = CpSurface.Side[STARBOARD].fZ = CpSurface.Side[STARBOARD].fY = CpSurface.Side[STARBOARD].fX = 0.0f;
			break;
		case 2:
			CpSurface.Side[PORTSIDE].fYt = CpSurface.Side[PORTSIDE].fXt =  CpSurface.Side[PORTSIDE].fZ = CpSurface.Side[PORTSIDE].fY = CpSurface.Side[PORTSIDE].fX = 0.0f;
			CpSurface.Side[STARBOARD].fYt = CpSurface.Side[STARBOARD].fXt = CpSurface.Side[STARBOARD].fZ = CpSurface.Side[STARBOARD].fY = CpSurface.Side[STARBOARD].fX = 0.0f;
			break;
		}
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&theApp.m_Thread.m_CpMoveTo,PORTSIDE,TRUE,TRUE);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&theApp.m_Thread.m_CpMoveTo,STARBOARD,TRUE,TRUE);
		theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
		break;
	case SINGLE_ROBOT:
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
			CpSurface.Side[PORTSIDE].fZ = CpSurface.Side[PORTSIDE].fY = CpSurface.Side[PORTSIDE].fX = 0.0f;
			CpSurface.Side[STARBOARD].fZ = CpSurface.Side[STARBOARD].fY = CpSurface.Side[STARBOARD].fX = 0.0f;
		theApp.m_Motors.IntelligentGotoSurfaceCoord(&CpSurface,3,theApp.m_LastSettings.nDangerPlaneMoveMask);
		break;
	case TRACKED_PROBE:
		break;
	}
		
}

void CMainFrame::OnButtonMovetoMachineZero() 
{
	CCoord Cp;
	int nSide = 2;

	theApp.m_Motors.GetHeadPos(&Cp);
	switch(nSide) {
	case 0:
		Cp.Side0.fX=(float)theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize * -1.0f;
		Cp.Side0.fY=(float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
		Cp.Side0.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize * -1.0f;
		Cp.Side0.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize * -1.0f;
		Cp.Side0.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize * -1.0f;
		break;
	case 1:
		Cp.Side1.fX=(float)theApp.m_Axes[theApp.m_Tank.nXRight].nOffset * theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize * -1.0f;
		Cp.Side1.fY=(float)theApp.m_Axes[theApp.m_Tank.nYRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize * -1.0f;
		Cp.Side1.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZRight].nOffset * theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize * -1.0f;
		Cp.Side1.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset * theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize * -1.0f;
		Cp.Side1.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize * -1.0f;
		break;
	case 2:
		Cp.Side0.fX=(float)theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize * -1.0f;
		Cp.Side0.fY=(float)theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize * -1.0f;
		Cp.Side0.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize * -1.0f;
		Cp.Side0.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize * -1.0f;
		Cp.Side0.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset * theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize * -1.0f;
		Cp.Side1.fX=(float)theApp.m_Axes[theApp.m_Tank.nXRight].nOffset * theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize * -1.0f;
		Cp.Side1.fY=(float)theApp.m_Axes[theApp.m_Tank.nYRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize * -1.0f;
		Cp.Side1.fZ=(float)theApp.m_Axes[theApp.m_Tank.nZRight].nOffset * theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize * -1.0f;
		Cp.Side1.fXt=(float)theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset * theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize * -1.0f;
		Cp.Side1.fYt=(float)theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset * theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize * -1.0f;
		break;
	}
	theApp.m_Thread.StopCurrentThreadAction();
	theApp.m_Thread.m_CpMoveTo = Cp;
	theApp.m_Thread.m_nThreadAction=MOVETO_STRAITE_LINE;
}


HRESULT CMainFrame::InvalidateScanTime(WPARAM,LPARAM)
{
	CString Buff, strStartTime, strFinishTime, strElapsedTime;
	static CTime LastStartTime, LastFinishTime;
	CTime FinishTime;
	static int nLastScanLine;
	float fTotalTime = 0.0f;
	float fRevTime;
	int nLine;
	CCoord Cp;

	if(((LastStartTime != PROFILE->m_StartTime) && (PROFILE->m_ScanLine != NULL)) || (nLastScanLine-theApp.m_Thread.m_nScanLine)) {
		switch(theApp.m_Thread.m_nThreadAction) {
		case CONTOUR_SCAN:
		case ROBOT_CONTOUR_SCAN:
		case ROBOT_ROLLER_PROBE_SCAN:
		case MULTI_PROBE_SCAN:
			for(nLine = theApp.m_Thread.m_nScanLine;nLine<PROFILE->m_nScanFinishLine;nLine+=PROFILE->m_nLineIncrement) {
				fTotalTime += PROFILE->CalculateLineScanTime(nLine); 
			}
			if(PROFILE->m_nWaypointMode>4)	fTotalTime *= 1.20f;
			break;

		case TURNTABLE_SCAN:
		case CONTINUOUS_TURNTABLE_SCAN:
		case SEGMENT_TURNTABLE_SCAN:

			fRevTime = 360.0f / PROFILE->m_fScanSpeed;
			for(nLine = theApp.m_Thread.m_nScanLine;nLine<PROFILE->m_nScanFinishLine;nLine++) {
				PROFILE->m_ScanLine[0].GetCoord(nLine,&Cp);
				if(Cp.nType != 2) {
					fTotalTime += fRevTime;
				}
			}
			break;
		}

		fTotalTime /= PROFILE->m_nNumberProbesPerImage;

		FinishTime = CTime::GetCurrentTime();
		FinishTime = FinishTime + (int)fTotalTime;
			
		strStartTime.LoadString(IDS_Start);
		strFinishTime.LoadString(IDS_Finish);
		strElapsedTime.LoadString(IDS_Elapsed);

		CTimeSpan TimeDiff(FinishTime - PROFILE->m_StartTime);

		int nHours = TimeDiff.GetHours();
		int nMinutes = TimeDiff.GetMinutes();
		Buff.Format(L"%s: %s    %s: %s   %s: %dh%02d",strStartTime, PROFILE->m_StartTime.Format(L"%Hh%M"), strFinishTime, FinishTime.Format(L"%Hh%M"), strElapsedTime, nHours, nMinutes );
		m_wndStatusBar.SetPaneText(SCAN_TIME_PANE,Buff);

		LastFinishTime = FinishTime;
		LastStartTime = PROFILE->m_StartTime;
		nLastScanLine=theApp.m_Thread.m_nScanLine;
	}
	return NULL;
}

void CMainFrame::RefreshMessageInStatusBar()
{
	static CString LastMessage;
	static COLORREF rgbLastText;
	static COLORREF rgbLastBk;
	CRect rect;
/*
	if(LastMessage - theApp.m_StatusMessage || rgbLastText - theApp.m_rgbStatusMessageText || rgbLastBk - theApp.m_rgbStatusMessageBk) {
		LastMessage = theApp.m_StatusMessage;
		rgbLastText = theApp.m_rgbStatusMessageText;
		rgbLastBk = theApp.m_rgbStatusMessageBk;

		CFont* pFont = m_wndStatusBar.GetStatusBarCtrl().GetFont();

		m_wndStatusBar.GetItemRect(STATUS_MESSAGE_PANE,&rect);
		rect.InflateRect(-1,-1,-1,-1);
		CDC* pDC = m_wndStatusBar.GetDC();
		CFont* pOldFont = pDC->SelectObject(pFont);
		pDC->FillRect(rect, &CBrush(rgbLastBk));
		pDC->TextOut(rect.left,rect.top,LastMessage);
		pDC->SelectObject(pOldFont);
		m_wndStatusBar.ReleaseDC(pDC);
	}
	*/
}


void CMainFrame::OnUltrasonicsOscilloscope1() 
{
	DisplayOscilloscopes(1);
}

void CMainFrame::OnUltrasonicsOscilloscope2() 
{
	DisplayOscilloscopes(2);
}

void CMainFrame::OnUltrasonicsOscilloscope3() 
{
	DisplayOscilloscopes(3);
}

void CMainFrame::OnUltrasonicsOscilloscope4() 
{
	DisplayOscilloscopes(4);
}

void CMainFrame::OnUltrasonicsOscilloscope5() 
{
	DisplayOscilloscopes(5);
}

void CMainFrame::OnUltrasonicsOscilloscope6() 
{
	DisplayOscilloscopes(6);
}

void CMainFrame::OnUltrasonicsOscilloscope7() 
{
	DisplayOscilloscopes(7);
}

void CMainFrame::OnUltrasonicsOscilloscope8() 
{
	DisplayOscilloscopes(8);
	
}

void CMainFrame::OnUltrasonicsOscilloscope()
{
	int nn;

	for(nn=0;nn<8;nn++) {
		if (m_pScopeDlg[nn] == NULL) {
			m_pScopeDlg[nn] = new CScopeDlg(this,(CWnd**)&m_pScopeDlg[nn],nn,nn);
			m_pScopeDlg[nn]->Create(CScopeDlg::IDD, this);
			theApp.m_LastSettings.bWindowOpenOnLastExit[(IDD_OSCILLOSCOPE + nn) % MAXIMUMWINDOWSIDNUMBER]=TRUE;
			return;
		} else {
			m_pScopeDlg[nn]->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		}
	}
}

void CMainFrame::DisplayOscilloscopes(int nCount)
{
	int nn;

	for(nn=0;nn<nCount;nn++) {
		if (m_pScopeDlg[nn] == NULL) {
			m_pScopeDlg[nn] = new CScopeDlg(this,(CWnd**)&m_pScopeDlg[nn],nn,nn);
			m_pScopeDlg[nn]->Create(CScopeDlg::IDD, this);
			theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_OSCILLOSCOPE+nn]=TRUE;
		} else {
			m_pScopeDlg[nn]->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		}
	}
	for(nn;nn<8;nn++) {
		if(m_pScopeDlg[nn] != NULL) {
			m_pScopeDlg[nn]->SendMessage(WM_CLOSE);
		}
		SAFE_DELETE(m_pScopeDlg[nn]);
	}
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFrameWndEx::OnLButtonDown(nFlags, point);
}

void CMainFrame::OnButtonDangerplane0() 
{


	theApp.m_LastSettings.nDangerPlaneMoveMask ^= 1;
	
}

void CMainFrame::OnButtonDangerplane1() 
{


	theApp.m_LastSettings.nDangerPlaneMoveMask ^= 2;
}

void CMainFrame::OnUpdateButtonDangerplane0(CCmdUI* pCmdUI) 
{


	theApp.m_LastSettings.nDangerPlaneMoveMask & 0x01 ? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
}

void CMainFrame::OnUpdateButtonDangerplane1(CCmdUI* pCmdUI) 
{


	theApp.m_LastSettings.nDangerPlaneMoveMask & 0x02 ? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
}

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM wp, LPARAM)
{
	UINT uiToolBarId = (UINT)wp;
	TRACE("CMainFrame::OnToolbarReset : %i\n", uiToolBarId);
	CString Buff = L"Speed";

	int nReplaced;

	switch (uiToolBarId)
	{
	case IDR_AIR_WATER_SIDE_0_TOOLBAR:
		//		if (m_pToolBarAirWater0 == NULL) return -1;
		m_pEditBoxButtonLeft->SetContextMenuID(IDC_EDIT_PUMP_SPEED_00);
		m_pEditBoxButtonLeft->CanBeStretched();
		m_pEditBoxButtonLeft->HaveHotBorder();
		m_pEditBoxButtonLeft->SetFlatMode(true);
		m_pEditBoxButtonLeft->SetStyle(TBBS_PRESSED);
		m_pSpinLeftPump->CanBeStretched();
		m_pSpinLeftPump->HaveHotBorder();
		m_pSpinLeftPump->SetStyle(TBBS_PRESSED);
		nReplaced = m_ToolBarAirWater0.ReplaceButton(IDC_EDIT_PUMP_SPEED_00, *m_pEditBoxButtonLeft);
		nReplaced = m_ToolBarAirWater0.ReplaceButton(IDC_SPIN_PUMP_SPEED_00, *m_pSpinLeftPump);
		break;

	case IDR_AIR_WATER_SIDE_1_TOOLBAR:
		//		if (m_pToolBarAirWater1 == NULL) return -1;
		m_pEditBoxButtonRight->SetContextMenuID(IDC_EDIT_PUMP_SPEED_01);
		m_pEditBoxButtonRight->CanBeStretched();
		m_pEditBoxButtonRight->HaveHotBorder();
		m_pEditBoxButtonRight->SetFlatMode(true);
		m_pEditBoxButtonRight->SetStyle(TBBS_PRESSED);
		m_pSpinRightPump->CanBeStretched();
		m_pSpinRightPump->HaveHotBorder();
		m_pSpinRightPump->SetStyle(TBBS_PRESSED);

		nReplaced = m_ToolBarAirWater1.ReplaceButton(IDC_EDIT_PUMP_SPEED_01, *m_pEditBoxButtonRight);
		nReplaced = m_ToolBarAirWater1.ReplaceButton(IDC_SPIN_PUMP_SPEED_01, *m_pSpinRightPump);

		break;

	case IDR_DUAL_WATERPATH_THICKNESS_TOOLBAR:
		//		if(m_pToolBarManipulator == NULL) return -1; 
		
		m_pEditLeftWp->SetContextMenuID(ID_EDIT_LEFT_WATER_PATH);
		m_pEditLeftWp->CanBeStretched();
		m_pEditLeftWp->HaveHotBorder();
		m_pEditLeftWp->SetFlatMode(true);
		m_pEditLeftWp->SetStyle(TBBS_PRESSED);
		m_pSpinLeftWp->CanBeStretched();
		m_pSpinLeftWp->HaveHotBorder();
		m_pSpinLeftWp->SetStyle(TBBS_PRESSED);


		m_pEditRightWp->SetContextMenuID(ID_EDIT_RIGHT_WATER_PATH);
		m_pEditRightWp->CanBeStretched();
		m_pEditRightWp->HaveHotBorder();
		m_pEditRightWp->SetFlatMode(true);
		m_pEditRightWp->SetStyle(TBBS_PRESSED);
		m_pSpinRightWp->CanBeStretched();
		m_pSpinRightWp->HaveHotBorder();
		m_pSpinRightWp->SetStyle(TBBS_PRESSED);

		m_pEditThickness->SetContextMenuID(ID_EDIT_THICKNESS);
		m_pEditThickness->CanBeStretched();
		m_pEditThickness->HaveHotBorder();
		m_pEditThickness->SetFlatMode(true);
		m_pEditThickness->SetStyle(TBBS_PRESSED);
		m_pSpinThickness->CanBeStretched();
		m_pSpinThickness->HaveHotBorder();
		m_pSpinThickness->SetStyle(TBBS_PRESSED);


		nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_EDIT_LEFT_WATER_PATH, *m_pEditLeftWp);
		nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_SPIN_LEFT_WATER_PATH, *m_pSpinLeftWp);
		nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_EDIT_RIGHT_WATER_PATH, *m_pEditRightWp);
		nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_SPIN_RIGHT_WATER_PATH, *m_pSpinRightWp);
		nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_EDIT_THICKNESS, *m_pEditThickness);
		nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_SPIN_THICKNESS, *m_pSpinThickness);

		//Labels
		nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_LABEL_LEFT_WP, CToolBarLabel(ID_LABEL_LEFT_WP, L"Left Wp"));
		nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_LABEL_THICKNESS, CToolBarLabel(ID_LABEL_THICKNESS, L"Thickness"));
		nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_LABEL_RIGHT_WP, CToolBarLabel(ID_LABEL_RIGHT_WP, L"Right Wp"));

		break;
	case IDR_WP_PL_TIPS:
		//		if(m_pToolBarManipulator == NULL) return -1; 

		m_pEditWp0->SetContextMenuID(ID_EDIT_LEFT_WATER_PATH);
		m_pEditWp0->CanBeStretched();
		m_pEditWp0->HaveHotBorder();
		m_pEditWp0->SetFlatMode(true);
		m_pEditWp0->SetStyle(TBBS_PRESSED);
		m_pSpinWp0->CanBeStretched();
		m_pSpinWp0->HaveHotBorder();
		m_pSpinWp0->SetStyle(TBBS_PRESSED);


		m_pEditProbeLength->SetContextMenuID(ID_EDIT_LEFT_PROBE_LENGTH);
		m_pEditProbeLength->CanBeStretched();
		m_pEditProbeLength->HaveHotBorder();
		m_pEditProbeLength->SetFlatMode(true);
		m_pEditProbeLength->SetStyle(TBBS_PRESSED);
		m_pSpinProbeLength->CanBeStretched();
		m_pSpinProbeLength->HaveHotBorder();
		m_pSpinProbeLength->SetStyle(TBBS_PRESSED);

		nReplaced = m_ToolBarWaterPathProbeLengthTips.ReplaceButton(ID_EDIT_LEFT_WATER_PATH, *m_pEditWp0);
		nReplaced = m_ToolBarWaterPathProbeLengthTips.ReplaceButton(ID_SPIN_LEFT_WATER_PATH, *m_pSpinWp0);
		nReplaced = m_ToolBarWaterPathProbeLengthTips.ReplaceButton(ID_EDIT_LEFT_PROBE_LENGTH, *m_pEditProbeLength);
		nReplaced = m_ToolBarWaterPathProbeLengthTips.ReplaceButton(ID_SPIN_LEFT_PROBE_LENGTH, *m_pSpinProbeLength);

		//Labels
		nReplaced = m_ToolBarWaterPathProbeLengthTips.ReplaceButton(ID_LABEL_LEFT_WP, CToolBarLabel(ID_LABEL_LEFT_WP, L"Water path"));
		nReplaced = m_ToolBarWaterPathProbeLengthTips.ReplaceButton(ID_LABEL_LEFT_PROBE_LENGTH, CToolBarLabel(ID_LABEL_LEFT_PROBE_LENGTH, L"Probe length"));

		break;

	}

	return 0;
}

LRESULT CMainFrame::OnViewCustomize(WPARAM wp, LPARAM) {
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this,
		TRUE /* Automatic menus scaning */);

	UINT uiToolBarId = (UINT)wp;

	TRACE("CMainFrame::OnViewCustomize : %i\n", uiToolBarId);
	CString Buff = L"Speed";

	int nReplaced;

	switch (uiToolBarId)
	{
	case IDR_AIR_WATER_SIDE_0_TOOLBAR:
		//		if (m_pToolBarAirWater0 == NULL) return -1;

		m_pEditBoxButtonLeft->SetContextMenuID(IDC_EDIT_PUMP_SPEED_00);
		m_pEditBoxButtonLeft->CanBeStretched();
		m_pEditBoxButtonLeft->HaveHotBorder();
		m_pEditBoxButtonLeft->SetFlatMode(true);
		m_pEditBoxButtonLeft->SetStyle(TBBS_PRESSED);
		m_pSpinLeftPump->CanBeStretched();
		m_pSpinLeftPump->HaveHotBorder();
		m_pSpinLeftPump->SetStyle(TBBS_PRESSED);

		nReplaced = m_ToolBarAirWater0.ReplaceButton(IDC_EDIT_PUMP_SPEED_00, *m_pEditBoxButtonLeft);
		nReplaced = m_ToolBarAirWater0.ReplaceButton(IDC_SPIN_PUMP_SPEED_00, *m_pSpinLeftPump);

		pDlgCust->AddButton(_T("Edit"), *m_pEditBoxButtonLeft);
		pDlgCust->AddButton(_T("Edit"), *m_pSpinLeftPump);

		SAFE_DELETE(m_pEditBoxButtonLeft);
		break;

	case IDR_AIR_WATER_SIDE_1_TOOLBAR:
		//		if (m_pToolBarAirWater1 == NULL) return -1;
		m_pEditBoxButtonRight->SetContextMenuID(IDC_EDIT_PUMP_SPEED_01);
		m_pEditBoxButtonRight->CanBeStretched();
		m_pEditBoxButtonRight->HaveHotBorder();
		m_pEditBoxButtonRight->SetFlatMode(true);
		m_pEditBoxButtonRight->SetStyle(TBBS_PRESSED);
		m_pSpinRightPump->CanBeStretched();
		m_pSpinRightPump->HaveHotBorder();
		m_pSpinRightPump->SetStyle(TBBS_PRESSED);

		nReplaced = m_ToolBarAirWater1.ReplaceButton(IDC_EDIT_PUMP_SPEED_01, *m_pEditBoxButtonRight);
		nReplaced = m_ToolBarAirWater1.ReplaceButton(IDC_SPIN_PUMP_SPEED_01, *m_pSpinRightPump);


		pDlgCust->AddButton(_T("Edit"), *m_pEditBoxButtonRight);
		pDlgCust->AddButton(_T("Edit"), *m_pSpinRightPump);


		SAFE_DELETE(m_pEditBoxButtonRight);
		break;

	case IDR_DUAL_WATERPATH_THICKNESS_TOOLBAR:
		//		if(m_pToolBarManipulator == NULL) return -1; 

		m_pEditLeftWp->SetContextMenuID(ID_EDIT_LEFT_WATER_PATH);
		m_pEditLeftWp->CanBeStretched();
		m_pEditLeftWp->HaveHotBorder();
		m_pEditLeftWp->SetFlatMode(true);
		m_pEditLeftWp->SetStyle(TBBS_PRESSED);
		m_pSpinLeftWp->CanBeStretched();
		m_pSpinLeftWp->HaveHotBorder();
		m_pSpinLeftWp->SetStyle(TBBS_PRESSED);


		m_pEditRightWp->SetContextMenuID(ID_EDIT_RIGHT_WATER_PATH);
		m_pEditRightWp->CanBeStretched();
		m_pEditRightWp->HaveHotBorder();
		m_pEditRightWp->SetFlatMode(true);
		m_pEditRightWp->SetStyle(TBBS_PRESSED);
		m_pSpinRightWp->CanBeStretched();
		m_pSpinRightWp->HaveHotBorder();
		m_pSpinRightWp->SetStyle(TBBS_PRESSED);

		m_pEditThickness->SetContextMenuID(ID_EDIT_THICKNESS);
		m_pEditThickness->CanBeStretched();
		m_pEditThickness->HaveHotBorder();
		m_pEditThickness->SetFlatMode(true);
		m_pEditThickness->SetStyle(TBBS_PRESSED);
		m_pSpinThickness->CanBeStretched();
		m_pSpinThickness->HaveHotBorder();
		m_pSpinThickness->SetStyle(TBBS_PRESSED);


		nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_EDIT_LEFT_WATER_PATH, *m_pEditLeftWp);
		nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_SPIN_LEFT_WATER_PATH, *m_pSpinLeftWp);
		nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_EDIT_RIGHT_WATER_PATH, *m_pEditRightWp);
		nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_SPIN_RIGHT_WATER_PATH, *m_pSpinRightWp);
		nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_EDIT_THICKNESS, *m_pEditThickness);
		nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_SPIN_THICKNESS, *m_pSpinThickness);

		//Labels
		{
			CToolBarLabel labelLeft(ID_LABEL_LEFT_WP, L"Left Wp");
			CToolBarLabel labelThickness(ID_LABEL_THICKNESS, L"Thickness");
			CToolBarLabel labelRight(ID_LABEL_RIGHT_WP, L"Right Wp");
			nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_LABEL_LEFT_WP, labelLeft);
			nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_LABEL_THICKNESS, labelRight);
			nReplaced = m_ToolBarDualWaterPath.ReplaceButton(ID_LABEL_RIGHT_WP, labelThickness);

			pDlgCust->AddButton(_T("Edit"), *m_pEditLeftWp);
			pDlgCust->AddButton(_T("Edit"), *m_pSpinLeftWp);
			pDlgCust->AddButton(_T("Edit"), *m_pEditRightWp);
			pDlgCust->AddButton(_T("Edit"), *m_pSpinRightWp);
			pDlgCust->AddButton(_T("Edit"), *m_pEditThickness);
			pDlgCust->AddButton(_T("Edit"), *m_pSpinThickness);
		}
		break;
	case IDR_WP_PL_TIPS:
		//		if(m_pToolBarManipulator == NULL) return -1; 

		m_pEditWp0->SetContextMenuID(ID_EDIT_LEFT_WATER_PATH);
		m_pEditWp0->CanBeStretched();
		m_pEditWp0->HaveHotBorder();
		m_pEditWp0->SetFlatMode(true);
		m_pEditWp0->SetStyle(TBBS_PRESSED);
		m_pSpinWp0->CanBeStretched();
		m_pSpinWp0->HaveHotBorder();
		m_pSpinWp0->SetStyle(TBBS_PRESSED);

		m_pEditProbeLength->SetContextMenuID(ID_EDIT_THICKNESS);
		m_pEditProbeLength->CanBeStretched();
		m_pEditProbeLength->HaveHotBorder();
		m_pEditProbeLength->SetFlatMode(true);
		m_pEditProbeLength->SetStyle(TBBS_PRESSED);
		m_pSpinProbeLength->CanBeStretched();
		m_pSpinProbeLength->HaveHotBorder();
		m_pSpinProbeLength->SetStyle(TBBS_PRESSED);


		nReplaced = m_ToolBarWaterPathProbeLengthTips.ReplaceButton(ID_EDIT_LEFT_WATER_PATH, *m_pEditWp0);
		nReplaced = m_ToolBarWaterPathProbeLengthTips.ReplaceButton(ID_SPIN_LEFT_WATER_PATH, *m_pSpinWp0);
		nReplaced = m_ToolBarWaterPathProbeLengthTips.ReplaceButton(ID_EDIT_THICKNESS, *m_pEditProbeLength);
		nReplaced = m_ToolBarWaterPathProbeLengthTips.ReplaceButton(ID_SPIN_THICKNESS, *m_pSpinProbeLength);

		//Labels
		CToolBarLabel labelLeftWp(ID_LABEL_LEFT_WP, L"Water path");
		CToolBarLabel labelProbeLength(ID_LABEL_THICKNESS, L"Probe length");
		nReplaced = m_ToolBarWaterPathProbeLengthTips.ReplaceButton(ID_LABEL_LEFT_WP, labelLeftWp);
		nReplaced = m_ToolBarWaterPathProbeLengthTips.ReplaceButton(ID_LABEL_LEFT_PROBE_LENGTH, labelProbeLength);

		pDlgCust->AddButton(_T("Edit"), *m_pEditWp0);
		pDlgCust->AddButton(_T("Edit"), *m_pSpinWp0);
		pDlgCust->AddButton(_T("Edit"), *m_pEditProbeLength);
		pDlgCust->AddButton(_T("Edit"), *m_pSpinProbeLength);

		break;
	}
	SAFE_DELETE(pDlgCust);
	return 0;
}



//This ensures that this edit box, which is in a MFCToolBar, will be enabled.
void CMainFrame::OnUpdateEditPumpSpeed00(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateSpinPumpSpeed00(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateEditPumpSpeed01(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateSpinPumpSpeed01(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}





void CMainFrame::OnUpdateEditLeftWaterPath(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateEditRightWaterPath(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CMainFrame::OnUpdateEditThickness(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateEditProbeLength(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}



void CMainFrame::OnEditPumpSpeed00()
{
	int foo = 0;
}

//We are doing this as a workaround because it is not possible to use CSpinCtrl.SetBuddy(CEdit) in a MFCToolBar.
BOOL CMainFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//Get the window that currently has the focus. 
	CWnd* pWndFocus;
	pWndFocus = GetFocus();
	HWND hWndFocus = pWndFocus->GetSafeHwnd();
	//Get the resource ID for the window currently in focus 
	UINT nResID = GetWindowLong(hWndFocus, GWL_ID);
	
	//Calculate the number of clicks moved by the mouse wheel.
	int nClicks = zDelta / 120;
	NM_UPDOWN updown;
	LRESULT pResult=0;
	updown.iDelta = nClicks;

	//Now spin the appropriate spin box.
	switch (nResID) {
		case(IDC_EDIT_PUMP_SPEED_00):
			OnDeltaposSpinPumpSpeed00((NMHDR*)&updown, &pResult);
			break;
		case(IDC_EDIT_PUMP_SPEED_01):
			OnDeltaposSpinPumpSpeed01((NMHDR*)&updown, &pResult);
			break;
		case(ID_EDIT_LEFT_WATER_PATH):
			OnDeltaposSpinLeftWaterPath((NMHDR*)&updown, &pResult);
			break;
		case(ID_EDIT_RIGHT_WATER_PATH):
			OnDeltaposSpinRightWaterPath((NMHDR*)&updown, &pResult);
			break;
		case(ID_EDIT_THICKNESS):
			OnDeltaposSpinThickness((NMHDR*)&updown, &pResult);
			break;
		case(ID_EDIT_LEFT_PROBE_LENGTH):
			OnDeltaposSpinProbeLength((NMHDR*)&updown, &pResult);
			break;
	}

	return CFrameWndEx::OnMouseWheel(nFlags, zDelta, pt);
}




void CMainFrame::ShowDockablePane(CDockablePane* pPane, bool bShowState)
{
	CString strPane;
	int nLocalID;

	pPane->EnableDocking(CBRS_ALIGN_ANY);
	strPane.Format(L"Pane-%d", pPane->GetDlgCtrlID());
	nLocalID = theApp.GetSectionInt(strPane, L"ID", -1);
	if (pPane->GetDlgCtrlID() == nLocalID) {
//		DockPaneConfineToScreen(pPane);
//		if (pPane->IsHideInAutoHideMode() == TRUE) {
			pPane->ShowPane(bShowState, false, true);
//		}
//		else {
//			pPane->Slide(TRUE, TRUE);
//		}
	}
	else {
		pPane->FloatPane(CRect(0, 0, 200, 200));
		pPane->ShowPane(bShowState, false, true);
	}
}

void CMainFrame::InitToolbarButtons()
{
	CDC* pDC = GetDC();
	CSize sizePump = pDC->GetTextExtent(L"999");
	CSize sizeWaterPath = pDC->GetTextExtent(L"999 mm");
	ReleaseDC(pDC);
	sizePump.cx = MulDiv(sizePump.cx, 3, 2);
	sizeWaterPath.cx = MulDiv(sizeWaterPath.cx, 3, 2);

	m_pEditBoxButtonLeft = new CMFCToolBarEditBoxButton(
		IDC_EDIT_PUMP_SPEED_00,
		GetCmdMgr()->GetCmdImage(IDC_EDIT_PUMP_SPEED_00, FALSE),
		ES_AUTOHSCROLL, sizePump.cx);
	m_pEditBoxButtonRight = new CMFCToolBarEditBoxButton(
		IDC_EDIT_PUMP_SPEED_01,
		GetCmdMgr()->GetCmdImage(IDC_EDIT_PUMP_SPEED_01, FALSE),
		ES_AUTOHSCROLL, sizePump.cx);
	m_pSpinLeftPump = new CMFCToolBarSpinButton(IDC_SPIN_PUMP_SPEED_00,
		GetCmdMgr()->GetCmdImage(IDC_SPIN_PUMP_SPEED_00, FALSE));
	m_pSpinRightPump = new CMFCToolBarSpinButton(IDC_SPIN_PUMP_SPEED_01,
		GetCmdMgr()->GetCmdImage(IDC_SPIN_PUMP_SPEED_01, FALSE));
	m_pEditLeftWp = new CMFCToolBarEditBoxButton(
		ID_EDIT_LEFT_WATER_PATH,
		GetCmdMgr()->GetCmdImage(ID_EDIT_LEFT_WATER_PATH, FALSE),
		ES_AUTOHSCROLL, sizeWaterPath.cx);
	m_pEditRightWp = new CMFCToolBarEditBoxButton(
		ID_EDIT_RIGHT_WATER_PATH,
		GetCmdMgr()->GetCmdImage(ID_EDIT_RIGHT_WATER_PATH, FALSE),
		ES_AUTOHSCROLL, sizeWaterPath.cx);
	m_pEditThickness = new CMFCToolBarEditBoxButton(
		ID_EDIT_THICKNESS,
		GetCmdMgr()->GetCmdImage(ID_EDIT_THICKNESS, FALSE),
		ES_AUTOHSCROLL, sizeWaterPath.cx);
	m_pEditWp0 = new CMFCToolBarEditBoxButton(
		ID_EDIT_LEFT_WATER_PATH,
		GetCmdMgr()->GetCmdImage(ID_EDIT_LEFT_WATER_PATH, FALSE),
		ES_AUTOHSCROLL, sizeWaterPath.cx);
	m_pEditProbeLength = new CMFCToolBarEditBoxButton(
		ID_EDIT_LEFT_PROBE_LENGTH,
		GetCmdMgr()->GetCmdImage(ID_EDIT_LEFT_PROBE_LENGTH, FALSE),
		ES_AUTOHSCROLL, sizeWaterPath.cx);

	m_pSpinLeftWp = new CMFCToolBarSpinButton(ID_SPIN_LEFT_WATER_PATH,
		GetCmdMgr()->GetCmdImage(ID_SPIN_LEFT_WATER_PATH, FALSE));
	m_pSpinRightWp = new CMFCToolBarSpinButton(ID_SPIN_RIGHT_WATER_PATH,
		GetCmdMgr()->GetCmdImage(ID_SPIN_RIGHT_WATER_PATH, FALSE));
	m_pSpinThickness = new CMFCToolBarSpinButton(ID_SPIN_THICKNESS,
		GetCmdMgr()->GetCmdImage(ID_SPIN_THICKNESS, FALSE));
	m_pSpinWp0 = new CMFCToolBarSpinButton(ID_SPIN_LEFT_WATER_PATH,
		GetCmdMgr()->GetCmdImage(ID_SPIN_LEFT_WATER_PATH, FALSE));
	m_pSpinProbeLength = new CMFCToolBarSpinButton(ID_SPIN_LEFT_PROBE_LENGTH,
		GetCmdMgr()->GetCmdImage(ID_SPIN_LEFT_PROBE_LENGTH, FALSE));
}

LRESULT CMainFrame::RemoteRMessageSend(WPARAM Wp, LPARAM Lp){
	theApp.m_pRemote->Send();
	return 0;
}

LRESULT CMainFrame::RemoteRMessageReceive(WPARAM Wp, LPARAM Lp){
	theApp.m_pRemote->Receive();
	return 0;
}

LRESULT CMainFrame::RemoteStartServer(WPARAM Wp, LPARAM Lp){
	theApp.m_pRemote->StartServer();
	return 0;
}



void CMainFrame::OnViewStatus()
{
	ShowDockablePane(&m_wndStatusPane,true);
	m_wndStatusPane.SetFocus();
}


void CMainFrame::OnUpdateViewStatus(CCmdUI *pCmdUI)
{
	if (m_wndStatusPane.GetSafeHwnd())
		pCmdUI->SetCheck(m_wndStatusPane.IsVisible());
}


void CMainFrame::OnPositionDialoge()
{
	if (theApp.m_LastSettings.nPositionTabMask == 0) return;

	if (m_pPositionSheet == NULL) {
		m_pPositionSheet = new CPositionSheet((L"Position"), this, 0, (CWnd**)&m_pPositionSheet);
		if (m_pPositionSheet->Create(this)) {
			theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_POSITION_SHEET % MAXIMUMWINDOWSIDNUMBER] = TRUE;
		}
		else {
			m_pPositionSheet = NULL;
		}
	}
	else {
		m_pPositionSheet->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}


void CMainFrame::OnUpdatePositionDialoge(CCmdUI *pCmdUI)
{
	m_pPositionSheet->GetSafeHwnd() ? pCmdUI->SetCheck(TRUE) : pCmdUI->SetCheck(FALSE);
}



void CMainFrame::OnSystemProperties()
{
	ShowDockablePane(&m_wndPropertiesPane, true);
	m_wndPropertiesPane.SetFocus();
}


void CMainFrame::OnUpdateSystemProperties(CCmdUI *pCmdUI)
{
	if (m_wndPropertiesPane.GetSafeHwnd())
		pCmdUI->SetCheck(m_wndPropertiesPane.IsVisible());
}


void CMainFrame::OnSystemAxes()
{
	if (m_pAxesSheet == NULL) {
		m_pAxesSheet = new CAxesSheet((L"Axes"), this, 0, (CWnd**)&m_pAxesSheet);
		m_pAxesSheet->Create(this);
		theApp.m_LastSettings.bWindowOpenOnLastExit[IDD_AXES_SHEET % MAXIMUMWINDOWSIDNUMBER] = TRUE;
	}
	else {
		m_pAxesSheet->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE);
	}


}

void CMainFrame::OnUpdateSystemAxes(CCmdUI* pCmdUI)
{
	theApp.m_nLogonLevelMask&theApp.m_cAccess[pCmdUI->m_nID] ? pCmdUI->Enable(TRUE) : pCmdUI->Enable(FALSE);


}


void CMainFrame::OnDockablesAxesstatus()
{
	ShowDockablePane(&m_wndAxisStatusPane, true);
	m_wndAxisStatusPane.SetFocus();
}


void CMainFrame::OnUpdateDockablesAxesstatus(CCmdUI *pCmdUI)
{
	if (m_wndAxisStatusPane.GetSafeHwnd())
		pCmdUI->SetCheck(m_wndAxisStatusPane.IsVisible());
}


void CMainFrame::OnDockablesAxes()
{
	ShowDockablePane(&m_wndAxesPane, true);
	m_wndAxesPane.SetFocus();
}


void CMainFrame::OnUpdateDockablesAxes(CCmdUI *pCmdUI)
{
	if (m_wndAxesPane.GetSafeHwnd())
		pCmdUI->SetCheck(m_wndAxesPane.IsVisible());
}

void CMainFrame::OnDockablesAxis()
{
	ShowDockablePane(&m_wndAxisPane, true);
	m_wndAxisPane.SetFocus();
}


void CMainFrame::OnUpdateDockablesAxis(CCmdUI *pCmdUI)
{
	if (m_wndAxisPane.GetSafeHwnd())
		pCmdUI->SetCheck(m_wndAxisPane.IsVisible());
}


void CMainFrame::OnDockablesCoordinates()
{
	ShowDockablePane(&m_wndCoordArrayPane, true);
	m_wndCoordArrayPane.SetFocus();
}


void CMainFrame::OnUpdateDockablesCoordinates(CCmdUI *pCmdUI)
{
	if (m_wndCoordArrayPane.GetSafeHwnd())
		pCmdUI->SetCheck(m_wndCoordArrayPane.IsVisible());
}

HRESULT CMainFrame::InvalidateCoordArrayPane(WPARAM wp, LPARAM lp)
{
	if (m_wndCoordArrayPane.GetSafeHwnd()) {
		if (wp & _ADJUST_LAYOUT) m_wndCoordArrayPane.AdjustLayout();
		if(wp & _CREATE_COLUMNS) m_wndCoordArrayPane.CreateColumns();
		if (wp & _FILL_LIST) m_wndCoordArrayPane.FillList();
		if (wp & _FILL_LINE_LIST) m_wndCoordArrayPane.FillLineList();
		if (wp && _ENSURE_LINE_LIST_VISIBLE) m_wndCoordArrayPane.EnsureLineListVisible();
	}
	return NULL;
}

LRESULT CMainFrame::OnShowPropertiesPane(WPARAM wp, LPARAM lp)
{
	m_wndPropertiesPane.InitPropList((PropertiesList)lp);
	ShowDockablePane(&m_wndPropertiesPane, true);
	m_wndPropertiesPane.SetFocus();

	return true;
}




void CMainFrame::OnDockables3dworldview()
{
	ShowDockablePane(&m_wnd3DWorldViewPane, true);
	m_wnd3DWorldViewPane.SetFocus();
}


void CMainFrame::OnUpdateDockables3dworldview(CCmdUI *pCmdUI)
{
	if (m_wnd3DWorldViewPane.GetSafeHwnd())
		pCmdUI->SetCheck(m_wnd3DWorldViewPane.IsVisible());
}

void CMainFrame::OnPositionDockingpane()
{
	ShowDockablePane(&m_wndPositionPane, true);
	m_wndPositionPane.SetFocus();
}


void CMainFrame::OnUpdatePositionDockingpane(CCmdUI *pCmdUI)
{
	if (m_wndPositionPane.GetSafeHwnd())
		pCmdUI->SetCheck(m_wndPositionPane.IsVisible());
}

void CMainFrame::OnDockablesOptimize()
{
	ShowDockablePane(&m_wndOptimizePane, true);
	m_wndOptimizePane.SetFocus();
}


void CMainFrame::OnUpdateDockablesOptimize(CCmdUI *pCmdUI)
{
	if (m_wndOptimizePane.GetSafeHwnd())
		pCmdUI->SetCheck(m_wndOptimizePane.IsVisible());
}

void CMainFrame::OnWindowResetwindowlayout()
{
	CDockingManager* pDockMgr = GetDockingManager();
	if (pDockMgr == NULL)return;
	CRect rect;
	rect.SetRectEmpty();
	ClientToScreen(rect);
	SetRedraw(FALSE);

	CObList list;
	pDockMgr->GetPaneList(list, TRUE, 0, TRUE);
	// UnDock and hide DockingControlBars

	POSITION pos;
	for (pos = list.GetHeadPosition(); pos != NULL;)
	{
		CBasePane* pBarNext = (CBasePane*)list.GetNext(pos);
		if (!::IsWindow(pBarNext->m_hWnd))continue;
		CDockablePane* pBar = DYNAMIC_DOWNCAST(CDockablePane, pBarNext);
		if (pBar != NULL)
		{
			if (pBar->IsAutoHideMode()) pBar->SetAutoHideMode(FALSE, CBRS_ALIGN_ANY);//ToggleAutoHide();
			if (pBar->IsMDITabbed())
				continue;
			pBar->UndockPane();
			ShowPane(pBar, FALSE, FALSE, FALSE);
		}
		CMFCToolBar* pToolBar = DYNAMIC_DOWNCAST(CMFCToolBar, pBarNext);
		if (pToolBar)
			pToolBar->m_recentDockInfo.m_recentSliderInfo.m_rectDockedRect = rect;

	}

	m_wndPositionPane.DockToFrameWindow(CBRS_LEFT, m_wndPositionPane.GetAHRestoredRect());
	ShowPane((CBasePane*)&m_wndPositionPane, TRUE, FALSE, FALSE);
	m_wndPropertiesPane.DockToFrameWindow(CBRS_RIGHT, m_wndPropertiesPane.GetAHRestoredRect());
	ShowPane((CBasePane*)&m_wndPropertiesPane, TRUE, FALSE, FALSE);
	m_wndStatusPane.DockToFrameWindow(CBRS_RIGHT, m_wndStatusPane.GetAHRestoredRect());
	ShowPane((CBasePane*)&m_wndStatusPane, TRUE, FALSE, FALSE);
	m_wndAxesPane.DockToFrameWindow(CBRS_RIGHT, m_wndAxesPane.GetAHRestoredRect());
	ShowPane((CBasePane*)&m_wndAxesPane, TRUE, FALSE, FALSE);
	m_wndAxisPane.DockToFrameWindow(CBRS_RIGHT, m_wndAxisPane.GetAHRestoredRect());
	ShowPane((CBasePane*)&m_wndAxisPane, TRUE, FALSE, FALSE);
	m_wndAxisStatusPane.DockToFrameWindow(CBRS_RIGHT, m_wndAxisStatusPane.GetAHRestoredRect());
	ShowPane((CBasePane*)&m_wndAxisStatusPane, TRUE, FALSE, FALSE);
	m_wndCoordArrayPane.DockToFrameWindow(CBRS_RIGHT, m_wndCoordArrayPane.GetAHRestoredRect());
	ShowPane((CBasePane*)&m_wndCoordArrayPane, TRUE, FALSE, FALSE);
	m_wnd3DWorldViewPane.DockToFrameWindow(CBRS_RIGHT, m_wnd3DWorldViewPane.GetAHRestoredRect());
	ShowPane((CBasePane*)&m_wnd3DWorldViewPane, TRUE, FALSE, FALSE);
	m_wndOptimizePane.DockToFrameWindow(CBRS_LEFT, m_wndOptimizePane.GetAHRestoredRect());
	ShowPane((CBasePane*)&m_wndOptimizePane, TRUE, FALSE, FALSE);

	/*
	//for tabbed pane
	CTabbedPane *pTabbedPane;
	m_wndTab1.DockToFrameWindow(CBRS_BOTTOM, m_wndTab1.GetAHRestoredRect());
	m_wndTab2.AttachToTabWnd(&m_wndTab1, DM_SHOW, FALSE, reinterpret_cast<CDockablePane**>(&pTabbedPane));
	m_wndTab3.AttachToTabWnd(&m_wndTab1, DM_SHOW, FALSE, reinterpret_cast<CDockablePane**>(&pTabbedPane));

	ShowPane(m_wndTab1, TRUE, FALSE, FALSE);
	ShowPane(m_wndTab2, TRUE, FALSE, FALSE);
	ShowPane(m_wndTab3, TRUE, FALSE, FALSE);
	*/
	
	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	AdjustClientArea();
	
}

HRESULT CMainFrame::InvalidateOptimizePane(WPARAM wp, LPARAM lp)
{
	OptimizePaneMessage *pMsg = (OptimizePaneMessage*)wp;
	m_wndOptimizePane.TransferAndRender(pMsg);
	return NULL;
}




BOOL CMainFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (message == AFX_WM_RESETTOOLBAR) {
		for (int nn = 0; nn < MAX_NUMBER_VIEWS; nn++) {
			if (m_pViewSheet[nn]) {
				m_pViewSheet[nn]->SendMessage(message, wParam, lParam);
			}
		};
	}

	return CFrameWndEx::OnWndMsg(message, wParam, lParam, pResult);
}

HRESULT CMainFrame::InvalidateViewSheets(WPARAM wParam, LPARAM lParam)
{
	for (int nn = 0; nn < MAX_NUMBER_VIEWS; nn++) {
		if (m_pViewSheet[nn]) {
			m_pViewSheet[nn]->InvalidateChild();
		}
	};
	return NULL;
}

HRESULT CMainFrame::OnUpdateFocalLawChanged(WPARAM wp, LPARAM lp)
{
	int nMask = (int)wp;

	if (nMask & _UPDATE_ULTRASONICS_SHEET) {
		if (m_pUltrasonicsSheet) {
			m_pUltrasonicsSheet->UpdateLSAPage();
		}
	}

	if (nMask & _UPDATE_ULTRASONICS_SHEET) {
		if (m_pPhasedArraySheet) {
			m_pPhasedArraySheet->UpdateAllPages();
		}
	}
	return NULL;
}
