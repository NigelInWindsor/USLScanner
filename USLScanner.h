// USLScanner.h : main header file for the USLSCANNER application
//

#if !defined(AFX_USLSCANNER_H__EDD7806F_CD5B_4FE6_9728_BD63C9AA3A36__INCLUDED_)
#define AFX_USLSCANNER_H__EDD7806F_CD5B_4FE6_9728_BD63C9AA3A36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

//Enable memory leak detection.
#include <stdlib.h>  
#include <crtdbg.h>  

#include <afxmt.h>

#include "resource.h"       // main symbols
#include "Profile.h"	// Added by ClassView
#include "Inclusions.h"
#include "Data.h"
#include "Si6.h"
#include "Servo.h"
#include "Motors.h"
#include "CoordTracker.h"
#include "CRigidBody.h"
#include "USLut.h"
#include "MainThread.h"
#include "Kinematics.h"	// Added by ClassView
#include "Scope.h"
#include "AcqirisDigitizer.h"	// Added by ClassView
#include "AlazarDigitizer.h"
#include "SpectrumDigitizer.h"
#include "USLPalette.h"	// Added by ClassView
#include "SerialJoystickDlg.h"
#include "BootUpDlg.h"	// Added by ClassView
#include "Help.h"
#include "Fir.h"
#include "Si10.h"
#include "SI10x64.h"
#include "LSA.h"
#include "Probe.h"
#include "Pmac.h"
#include "PolyCoord.h"
#include "USBtoI2C.h"
#include "RRMES.h"
#include "WorkSpace.h"
#include "Robot.h"
#include "SafetySerial.h"
#include "FieldBusController.h"
#include "Tracker.h"
#include "WorkerThread.h"
#include "USBKey.h"
#include "RemoteInterface.h"
#include "PhasedArrayProbe.h"
#include "RRImportCSVThicknessOverlay.h"
#include "Ethercat.h"
#include "DataTranslationHW.h"
#include "ToolChangers.h"
#include "RMessageDefs.h" 
#include "Remote.h" 
#include "EgismosRangefinder.h"
#include "FLSC10Rangefinder.h"
#include "CV120Trio.h"
#include "AOSPhasedArray.h"
#include "USLThread.h"
#include "Joints.h"
#include "PM40.h"



#define APP				CUSLScannerApp*
#define	FRAME			CMainFrame*  pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd
#define	PROGRAM_NAME	"USLScanner"

#define PROFILE		theApp.m_WS.m_pProfile
#define	DEGREES		theApp.m_Degrees
#define	CURRENT_FL	theApp.m_LSA.m_nScopeViewLaw

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#define POSITIVE	1
#define	NEGATIVE	-1

#define	NOT_PHASED_ARRAY	0
#define	PHASED_ARRAY		1

#define	MAX_THREAD_COUNT	30

inline ULONGLONG MulDiv(ULONGLONG nValue, ULONGLONG nNumerator, ULONGLONG nDenominator)
{
	if (nDenominator == 0) nDenominator = 1;
	return (nValue * nNumerator) / nDenominator;
}

inline int RoundDownMulDiv(int nValue, int nNumerator, int nDenominator)
{
	if (nDenominator == 0) nDenominator = 1;
	return (nValue * nNumerator) / nDenominator;
}

inline int FloatMulDiv(int nValue, int nNumerator, int nDenominator)
{
	if (nDenominator == 0) nDenominator = 1;
	return (int)((float)(nValue * nNumerator) / (float)nDenominator);
}

inline int MinMax(int *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}

inline long MinMax(long *pnV, int nMin, int nMax)
{

	if(*pnV<nMin) *pnV=nMin;
	if(*pnV>nMax) *pnV=nMax;

	return *pnV;
}

inline float MinMax(float *pfV, float fMin, float fMax)
{

	if (*pfV<fMin) *pfV = fMin;
	if (*pfV>fMax) *pfV = fMax;

	return *pfV;
}

inline double MinMax(double *pdV, double dMin, double dMax)
{

	if (*pdV<dMin) *pdV = dMin;
	if (*pdV>dMax) *pdV = dMax;

	return *pdV;
}

inline int Signf(float fValue)
{
	if(fValue<0.0f) return NEGATIVE;
	return POSITIVE;
}

inline int Signf(float *fValue)
{
	if(*fValue<0.0f) return NEGATIVE;
	return POSITIVE;
}

inline void Swap(float *fV0, float *fV1)
{
	float fTemp = *fV0;
	*fV0 = *fV1;
	*fV1 = fTemp;
}

inline char Quadrant(float fValue)
{
	float fV = fmodf(fValue, 360.0f);

	if(fV >=0.0f) {
		if(fV<90.0f) return 'A';
		if(fV<180.0f) return 'S';
		if(fV<270.0f) return 'T';
		if(fV<360.0f) return 'C';
	} else {
		if(fV>-90.0f) return 'C';
		if(fV>-180.0f) return 'T';
		if(fV>-270.0f) return 'S';
		if(fV>-360.0f) return 'A';
	}
	return 'A';
}

inline bool ExtractVector(CString &Buff, D3DXVECTOR3 *pVec, D3DXVECTOR3 *pVecNorm=NULL,float *pfAngle=NULL)
{
	int ii,nStart;

	Buff.MakeUpper();

	for(ii=0;ii<Buff.GetLength();ii++) {
		if(!(isalnum(Buff.GetAt(ii)) || Buff.GetAt(ii) == '.' || Buff.GetAt(ii) == '-')) {
			Buff.Delete(ii);
			ii--;
		}
	}
	if (pVec != NULL) {
		if ((nStart = Buff.Find(L"X", 0)) != -1) swscanf_s(Buff.GetBuffer(255) + nStart, L"X%f", &pVec->x);
		if ((nStart = Buff.Find(L"Y", 0)) != -1) swscanf_s(Buff.GetBuffer(255) + nStart, L"Y%f", &pVec->y);
		if ((nStart = Buff.Find(L"Z", 0)) != -1) swscanf_s(Buff.GetBuffer(255) + nStart, L"Z%f", &pVec->z);
	}

	if(pVecNorm != NULL) {
		if((nStart=Buff.Find(L"I",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"I%f",&pVecNorm->x);
		if((nStart=Buff.Find(L"J",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"J%f",&pVecNorm->y);
		if((nStart=Buff.Find(L"K",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"K%f",&pVecNorm->z);
	}
	if(pfAngle != NULL) {
		if((nStart=Buff.Find(L"U",0))!=-1) swscanf_s(Buff.GetBuffer(255)+nStart,L"U%f",pfAngle);
	}

	return true;
}

inline CString LoadString(int nID)
{
	CString Buff;
	Buff.LoadString(nID);
	return Buff;
}

#define	MAXIMUMWINDOWSIDNUMBER	500
#define MAX_NUMBER_VIEWS		20
#define	IDD_VIEW				480
#define	IDD_OSCILLOSCOPE		490



#define	AMP_IMAGE				0
#define	TIME_IMAGE				1
#define	PHASE_IMAGE				2
#define	EXT_IMAGE				3
#define	FILTER_IMAGE			4
#define	PHASE_OVERLAY_IMAGE		5

#define PERSPECTIVE_XY			0
#define PERSPECTIVE_XZ			1
#define PERSPECTIVE_YZ			2
#define PERSPECTIVE_ZX			3

#define	DANGER_MINS				0
#define	DANGER_MAXS				1

#define	EXPORT_CAD_COORDINATES		1
#define	EXPORT_TWIST_COORDINATES	2

// Maximum number of colors = 10
#define	RGB_AMP_TRACE	0
#define	RGB_TOF_TRACE	1
#define	RGB_BKGROUND	2
#define	RGB_GRATICULE	3
#define	RGB_MAXIMUM		4
#define	RGB_NOMINAL		5
#define	RGB_MINIMUM		6
#define	RGB_FONT		7

#define	DISPLAY_ACT_THICKNESS	0x00001
#define	DISPLAY_NOM_THICKNESS	0x00002
#define	DISPLAY_MIN_TOLERANCE	0x00004
#define	DISPLAY_MAX_TOLERANCE	0x00008
#define	DISPLAY_AMPLITUDE		0x00010

#define	DISPLAY_PHASE_POSITIVE			0x00001
#define	DISPLAY_PHASE_NEGATIVE			0x00002
#define	PHASE_POSITIVE_GREATER_LESS		0x00004
#define	PHASE_NEGATIVE_GREATER_LESS		0x00008

#define	_HT_OFF_PUMPS_OFF				0x00001
#define _HT_OFF_MACHINE_ZERO			0x00002
#define _HT_OFF_E_STOP					0x00004

#define	MAX_ZOOM_FACTOR	1000
#define	MIN_ZOOM_FACTOR	20

#define MAX_ARM_ENTRIES	50

//m_LicenseMask
#define	_ALL			0xfffffffffffffffe
#define	_MINIMAL	0x0000001
#define _FILE_OPEN	0x0000002

//nScopeMask
#define	SCOPE_STATISTICS					0x000001
#define	SCOPE_GRATICULE						0x000002
#define	SCOPE_HORIZONTAL_TEXT				0x000004
#define	SCOPE_LEFT_SCALE					0x000008
#define	SCOPE_RIGHT_SCALE					0x000010
#define	SCOPE_TOP_SCALE						0x000020
#define	SCOPE_BOTTOM_SCALE					0x000040
#define SCOPE_RENDER_CONVERSION_RATE		0x000080
#define SCOPE_RENDER_SAMPLE_RATE			0x000100
#define	SCOPE_SAMPLES_PER_TRACE				0x000200
#define	SCOPE_INTERFACE_MARKER_LINE			0x000400
#define	SCOPE_INTERFACE_MARKER_ARROW		0x000800
#define	SCOPE_AMPLITUDE_MARKER_LINE			0x001000
#define	SCOPE_AMPLITUDE_MARKER_ARROW		0x002000
#define	SCOPE_RENDER_COLLECT_TEXT			0x004000
#define	SCOPE_RENDER_WATER_PATH				0x008000
#define	SCOPE_RENDER_MATERIAL_PATH			0x010000
#define	SCOPE_TOF_MARKER_LINE				0x020000
#define	SCOPE_TOF_MARKER_ARROW				0x040000
#define SCOPE_TIME_TO_READ_ADC				0x080000
#define SCOPE_TIME_PROCESS_ASCAN			0x100000

//#define DAC_MAX_THRESHOLD		50		//Now a variable in digitizer
#define	FFT_MAX_RANGE			33		//-42dB theoretical max

#define POSITION_SIDE_0				0x001
#define POSITION_SIDE_1				0x002
#define POSITION_DISPLAY_NORM		0x004
#define POSITION_DISPLAY_A			0x008
#define POSITION_DISPLAY_B			0x010
#define POSITION_DISPLAY_ROT_NORM	0x020
#define POSITION_DISPLAY_QUAT		0x040
#define POSITION_DISPLAY_R0			0x080
#define POSITION_DISPLAY_R1			0x100

#define	PR20_TAB				0x001
#define	PR50_TAB				0x002
#define	PR30_TAB				0x004
#define	PR40_TAB				0x008
#define	SCOPE_TAB				0x010
#define	GLOBAL_TAB				0x020
#define	TIMESLOTS_TAB			0x040
#define	GATES_TAB				0x080
#define	SELF_TEST_TAB			0x100
#define	EXT_SOURCE_TAB			0x200
#define	TRAFFIC_LIGHTS_TAB		0x400
#define	PHASED_ARRAY_TAB		0x800

#define	Rectilinear_TAB			0x001
#define	Turntable_TAB			0x002
#define	Rollers_TAB				0x004
#define	Contour_TAB				0x008
#define	Tubes_TAB				0x010
#define	Disc_TAB				0x020
#define	Tip_Axes_TAB			0x040
#define	Optimize_TAB			0x080
#define	Pump_TAB				0x100
#define	Manipulator_TAB			0x200
#define	Origin_TAB				0x400
#define	Move_To_TAB				0x800
#define	Robots_TAB				0x00001000
#define	Robots_Position_TAB		0x00002000
#define	Scan_Lines_TAB			0x00004000
#define	TOF_CAL_TAB					0x00008000
#define	TRANSPLY_TAB				0x00010000
#define PHASEDARRAY_LINE_SCAN_TAB	0x00020000
#define	Jog_TAB						0x00040000
#define DISCS_SHAFTS_TAB		0x00080000

#define	Amplitude_Palette_TAB	0x01
#define	Thickness_Palette_TAB	0x02
#define	Porosity_Palette_TAB	0x04

#define ProfileCartesianTab		0x001
#define ProfileTurTableTab		0x002
#define Profile3DDesignTab		0x004
#define Profile3DViewTab		0x008
#define Profile3DStiffenerTab	0x010
#define Profile3DBladeTab		0x020

#define PositionPositionTab			0x001
#define PositionSingleBridgeTab		0x002
#define PositionHoldingTab			0x004
#define PositionRobotDualTab		0x008
#define PositionGraphicalTab		0x008
#define PositionSwivGimTab			0x010

#define _MACHINE_SIDE0				0x0001
#define _MACHINE_SIDE1				0x0002
#define _MACHINE_TURNTABLE			0x0004
#define _MACHINE_ROLLER				0x0008
#define _MACHINE_HOLDING			0x0010
#define _MACHINE_SQUIRTER			0x0020
#define _MACHINE_TANK				0x0040


#define		XY 01
#define		XZ 02
#define		YZ 12
#define		YX 10 
#define		ZX 20

enum PropertiesList {
	All = -1,
	Application,
	Kinematics,
	Scope,
	Tabs,
	Toolbars,
	Annotation,
	Archive,
	Motion,
	Machine,
	Profile,
	CScan,
	BScan,
	Palettes,
	Joysticks,
	World_View,
	NetworkAttachedDevices,
	UtSubset,
	UtCopy,
	PositionDisplay,
	View_Ruler,
	View_Histogram,
};


enum WindowedType {
	RECTANGULAR,
	HANNING,
	HAMMING,
	BLACKMAN,
	WELCH,
};

enum MachineDrawingStyle {
	STANDARD_10_AXIS,
	TANK_ZERO_NEAR_SIDE,
	TANK_ZERO_FAR_SIDE,
	TWIN_TOWER,
};

enum SafetyActions {
	SAFETY_STOP,
	SAFETY_JOYSTICK,
	SAFETY_MOVETO_STRAITE_LINE,
	SAFETY_POLY_MOVE,
	SAFETY_ORIGIN,
};

enum MainThreadTimerActions {

	WAIT_FOR_MOVE_FINISHED = 1,
	JOYSTICK_START,
	MOVETO_STRAITE_LINE,
	POLY_MOVE,
	REPETETIVE_POLY_MOVE,
	TEST_ALL_LINES,
	DUAL_POLY_MOVE,
	ORIGIN_X_LEFT,
	ORIGIN_Y_LEFT,
	ORIGIN_Z_LEFT,
	ORIGIN_XT_LEFT,
	ORIGIN_YT_LEFT,
	ORIGIN_X_RIGHT,
	ORIGIN_Y_RIGHT,
	ORIGIN_Z_RIGHT,
	ORIGIN_XT_RIGHT,
	ORIGIN_YT_RIGHT,
	ORIGIN_ALL,
	ORIGIN_ALL_LEFT,
	ORIGIN_ALL_RIGHT,
	ORIGIN_XYZ_LEFT,
	ORIGIN_XYZ_RIGHT,
	ORIGIN_XTYT_RIGHT,
	ORIGIN_RIGHT_ALL,
	ORIGIN_SEQUENTIAL,
	ORIGIN_X_BRIDGE,
	ORIGIN_R_LEFT,
	ORIGIN_X_MOVING,
	ORIGIN_Y_MOVING,
	ORIGIN_Z_MOVING,
	ORIGIN_X_FIXED,
	ORIGIN_Y_FIXED,
	ORIGIN_Z_FIXED,
	ORIGIN_HOLDING_FIXTURE,
	ORIGIN_HOLDING_FIXTURE_FIXED,
	ORIGIN_HOLDING_FIXTURE_MOVEING,
	ORIGIN_X_Y_Z_R_THETA_LEFT,
	ORIGIN_X_Y_Z_R_THETA_RIGHT,
	MOVETO_SAFE,
	SINGLE_AUTO_ALIGN,
	MULTIPLE_AUTO_ALIGN,
	MEASURE_WATER_VELOCITY,
	SINGLE_TIP_AUTO_ALIGN,
	MULTIPLE_TIP_AUTO_ALIGN,
	NORMALIZE,
	CONTOUR_SCAN,
	TURNTABLE_SCAN,
	CONTINUOUS_TURNTABLE_SCAN,
	SEGMENT_TURNTABLE_SCAN,
	MOVETO_HIGH_GAIN,
	MOVETO_HOLDING_FIXTURE,
	FLAT_SCAN,
	MOVETO_SAFELY,
	FIND_ZERO,
	PEAK_OPPOSITE,
	WATERPATH_ADJUST,
	OPTIMIZE_BRAIN_COORDINATE,
	OPTIMIZE_BRAIN_LINE,
	OPTIMIZE_BRAIN_ALL_LINES,
	MOVETO_ORIGIN,
	MOVE_ROUND_BALL,
	TUNING_MOVE,
	TIPS_REMOVE_AIR,
	MOVETO_CONTOUR_START,
	MOVETO_VIA_DANGER_PLANE,
	MOVETO_VIEW_VIA_DANGER_PLANE,
	MOVETO_PARK,
	PROBE_AXIAL_PLOT,
	MOVE_JUST_R,
	ORIGIN_ALL_FOREPLANE,
	ECCENTRIC_TEST,
	MOVETO_TURNTABLE_SAFE,
	PROBE_CROSS_SECTION,
	TRACK_ALONG_LINE,
	EXECUTE_LADDER,
	REPAIR_AXIS,
	BLADE_TO_DEGREES,
	BLADE_GO_HOME,
	ROBOT_CONTOUR_SCAN,
	MULTI_PROBE_SCAN,
	THICKNESS_CALIBRATION_TWO_PT,
	THICKNESS_CALIBRATION_All_PTS,
	CHECK_THICKNESS_GAUGE,
	DO_MARKER_PEN,
	BLADE_SCAN,
	MOVETO_BLADE_START_POS,
	ROBOT_ROLLER_PROBE_SCAN,
	MOVE_ROBOT_WITH_BACKOFF,
	PRE_WET_ROLLER_PROBE,
	ORIGIN_ALL_ROBO_9_PLUS_9,
	JOG_TO_POSITION,
	MOVETO_SURFACE,
	DEPOSIT_TOOL_0,
	DEPOSIT_TOOL_1,
	RETRIEVE_TOOL_0,
	RETRIEVE_TOOL_1,
	LEADSCREW_COMPENSATION,
	INTELLIGENT_GOTO,

//Mainframe time actions
	RELEARN_LINE,
	RELEARN_BRAIN,
	OPTOMISE_COORDINATE,
	BEAM_PROFILE_SECTIONS,
	LASER_TAUGHT_LINE_SCAN,
	ZERO_THETA,
	WORKSHEET_SCAN,
	WORKSHEET_SINGLE_SCAN,
};

enum ToolBarType {
	MOTION_DUAL_SQUIRTER_TOOLBAR = 0,
	ESTOP_JOY_SAFE_TOOLBAR,
	SINGLE_BRIDGE_WITH_HOME_TOOLBAR,
	LOG_OFF_ON_TOOLBAR,
	HT_OFF_ON_TOOLBAR,
	DUAL_WATERPATH_TOOLBAR,
	SINGLE_WATERPATH_TOOLBAR,
	POWERED_TOOLBAR,
	WATERPATH_TOOLBAR,
	TIPS_TOOLBAR,
	JOYSTICK_TOOLBAR,
	ULTRASONICS_TOOLBAR,
	PR30_SELECT_TOOLBAR,
	BLADE_TOOLBAR,
	TURNTABLE_ROLLER_TOOLBAR,
	MARKER_PEN_TOOLBAR,
	PHASED_ARRAY_TOOLBAR,
	CARTESIAN_VIEW_TOOLBAR,
	CARTESIAN_EDIT_TOOLBAR,
	CARTESIAN_LINE_TYPE_TOOLBAR,
	CARTESIAN_BLADE_TOOLBAR,
	ROBOT_ROLLER_PROBE_TOOLBAR,
	X_ROLLER_SWAP,
	AIR_PUMPS_TOOLBAR_0,
	PUMPS_TOOLBAR,
	AIR_PUMPS_TOOLBAR_1,
	DANGERPLANES_TOOLBAR,
};

enum StringType {
	PPMAC_Address = 0,
	PPMAC_User,
	PPMAC_Password,
	MachineSide0,
	MachineSide1,
	MachineBoth,
	ComponentSide0,
	ComponentSide1,
	Android_IP_Address,
	AOS_Address,
};

enum AccessLevel {
	ANYONE = 0x00,
	OPERATOR = 0x01,
	LEVEL_1 = 0x02,
	LEVEL_2 = 0x04,
	LEVEL_3 = 0x08,
	SUPERVISOR = 0x10,
	MANAGEMENT = 0x20,
	MAINTENANCE = 0x40,
	MANUFACTURER = 0x80,
};


#define	CAD_NODE0_ARROW_I	0x00000001
#define	CAD_NODE1_ARROW_I	0x00000002
#define	CAD_NODE0_ARROW_O	0x00000004
#define	CAD_NODE1_ARROW_O	0x00000008
#define	CAD_SHOW_LINE		0x00000010
#define	CAD_SHOW_LENGTH		0x00000020

#define	CONTINUOUS_TURNTABLE	0
#define	SEGMENT_TURNTABLE		1
#define	JUMP_TURNTABLE			2
 

#define	DEG_RAD		0.0174532925f
#define	DEG_TO_RAD	0.0174532925f
#define	RAD_TO_DEG	57.29577951f
#define	MMPERINCH	25.39876054f

#define	HORIZONTAL	0
#define	VERTICAL	1

#define	VERBOSE			1

#define	SCREEN	0
#define	PRINTER	1

#define	ANYONE			0x00
#define	OPERATOR		0x01
#define	LEVEL_1			0x02
#define	LEVEL_2			0x04
#define	LEVEL_3			0x08
#define	SUPERVISOR		0x10
#define	MANAGEMENT		0x20
#define	MAINTENANCE		0x40
#define	MANUFACTURER	0x80

#define	GRATICULE_BOTTOM	1
#define	GRATICULE_TOP		2
#define	GRATICULE_LEFT		4
#define	GRATICULE_RIGHT		8
#define	GRATICULE_LINES		0x10
#define	GRATICULE_DOTS		0x20
#define	GRATICULE_CROSSES	0x40

#define	TOOL_CURSOR			0
#define	TOOL_RULER			1
#define	TOOL_HISTOGRAM		2
#define	TOOL_VALUE			3
#define	TOOL_POINTER		4
#define	TOOL_POLYGON		5
#define TOOL_CAD			6
#define	TOOL_ZOOM_VARIABLE	7
#define	TOOL_RECTANGLE		8
#define	TOOL_ELLIPSE		9
#define	TOOL_LINE			10
#define	TOOL_PARALLELOGRAM	11
#define TOOL_CENTRELINE		12
#define	TOOL_CROSS_SECTION	13
#define	TOOL_INCLUSION		14

#define	RULER_LINE_STYLE		0
#define	RULER_RECTANGLE_STYLE	1
#define	RULER_ELLIPSE_STYLE		2

#define HISTOGRAM_RECTANGLE_STYLE 0
#define HISTOGRAM_CIRCLE_STYLE 1

#define SCALE_PERCENTAGE	0
#define SCALE_LINEAR_DB		1
#define SCALE_LOG			2


#define	UI_INVALIDATE_CHILD					0x8002
#define UI_PROFILE_OPENED					0x8003
#define	UI_SETTTOLBARCHECKEDSTATE			0x8004
#define	UI_UPDATE_AXES						0x8005
#define	UI_UPDATE_3D_VIEW_DLG				0x8006
#define	UI_TRACKER_COORDINATE_ADDED			0x8007
#define	WM_CHECKBOXCHANGED					0x8008
#define UI_VIEW_LASER_TRACKER_COORDINATES	0x8009
#define UI_3D_FILLLIST						0x800A
#define UI_UPDATE_PROFILE_DLG				0x800B
#define UI_UPDATE_ROBOT_SHEET				0x800C
#define UI_UPDATE_PALETTE_SHEET				0x800D
#define UI_UPDATE_BLADE_STATISTICS			0x800E
#define	UI_UPDATE_PHASED_ARRAY_DLG			0x800F
#define	UI_UPDATE_MOTION_TOOLS				0x8010
#define UI_CHANGE_INTERFACE_GATE			0x8011
#define UI_CHANGE_OTHER_GATES				0x8012
#define	UI_CLOSE_ALL_IMAGES					0x8013
#define UI_UPDATE_PMAC_SHEET				0x8014
#define	UI_UPDATE_LSA_UT_PAGE				0x8015
#define UI_UPDATE_DIALOG_BAR				0x8016
#define	UI_INVALIDATE_SCAN_START_TIME		0x8017
#define UI_UPDATE_PUMPS_DIALOGBAR			0x8018
#define UI_CALCULATE_WAYPOINTS_FINISHED		0x8019
#define UI_LANGUAGE_CHANGED					0x801A
#define UI_UPDATE_MOTION_TOOLS_SHEET		0x801B
#define UI_REFRESH_SCOPE_DISPLAY            0x801C
#define UI_RESETTOOLBAR						0x801D
#define UI_INVALIDATE_TOOLBARS				0x801E
#define UI_INVALIDATE_COORD_ARRAY_PANE		0x801F
#define UI_UPDATE_ULTRASONICS_SHEET			0x8020
#define	UI_SHOW_PROPERTIES_PANE				0x8021
#define	UI_MOUSEWHEEL_FROM_PARENT			0x8022
#define UI_RESTART_3D_WORLDVIEW				0x8023
#define UI_UPDATE_GATES_UT_PAGE				0x8024
#define UI_UPDATE_ALL_PAGES					0x8025
#define UI_INVALIDATE_FOR_ROLLER_KINEMATICS	0x8026
#define UI_INVALIDATE_OPTIMIZE_PANE			0x8027
#define UI_ITEMCHANGED						0x8028
#define UI_SET_ACCESS_PRIVELAGES			0x8029
#define	UI_INVALIDATE_VIEW_SHEETS			0x802A

#define	WM_ITEMCHANGED	100
#define WM_ITEMFINISHED 101

#define	_UT_PREAMP_GAIN				0x0000000000000001
#define	_UT_PREAMP_MODE				0x0000000000000002
#define	_UT_DAMPING					0x0000000000000004
#define	_UT_PULSE_WIDTH				0x0000000000000008
#define	_UT_PULSE_VOLTAGE			0x0000000000000010
#define	_UT_PRF						0x0000000000000020
#define	_UT_LINEAR_GAIN				0x0000000000000040
#define	_UT_LOG_GAIN				0x0000000000000080
#define	_UT_OUTPUT_MODE				0x0000000000000100
#define	_UT_RECTIFY_MODE			0x0000000000000200
#define	_UT_FILTER_TYPE				0x0000000000000400
#define	_UT_FILTER_RANGE			0x0000000000000800
#define	_UT_FILTER_CENTER			0x0000000000001000
#define	_UT_FILTER_CUT_OFF			0x0000000000002000
#define	_UT_DAC_MODE				0x0000000000004000
#define	_UT_DAC_TRIGGER				0x0000000000008000
#define	_UT_DAC_DELAY				0x0000000000010000
#define	_UT_DAC_THRESHOLD			0x0000000000020000
#define	_UT_DAC_TABLE				0x0000000000040000
#define	_UT_SCOPE_DELAY				0x0000000000080000
#define	_UT_SCOPE_WIDTH				0x0000000000100000
#define	_UT_GATE_MODE				0x0000000000200000
#define	_UT_GATE_DELAY				0x0000000000400000
#define	_UT_GATE_WIDTH				0x0000000000800000
#define	_UT_GATE_THRESHOLD			0x0000000001000000
#define _UT_GATE_FAR_DEAD_ZONE		0x0000000002000000
#define _UT_GATE_FAR_PEAK_WIDTH		0x0000000004000000
#define _UT_AMPLITUDE_COLLECT		0x0000000008000000
#define _UT_TOF_COLLECT				0x0000000010000000
#define _UT_TOF_MODE				0x0000000020000000
#define _UT_PULSER_OFF_ON			0x0000000040000000
#define _UT_TX						0x0000000080000000
#define _UT_RX						0x0000000100000000

#define	FEATURE_UT_PREAMP_GAIN			0x0000000000000001
#define	FEATURE_UT_TX_RX				0x0000000000000002
#define	FEATURE_UT_DAMPING				0x0000000000000004
#define	FEATURE_UT_PULSE_WIDTH			0x0000000000000008
#define	FEATURE_UT_PULSE_VOLTAGE		0x0000000000000010
#define	FEATURE_UT_GAIN					0x0000000000000020
#define	FEATURE_UT_OUTPUT_MODE			0x0000000000000040
#define	FEATURE_UT_FILTER_TYPE			0x0000000000000080
#define	FEATURE_UT_FILTER_RANGE			0x0000000000000100
#define	FEATURE_UT_FILTER_CENTER		0x0000000000000200
#define	FEATURE_UT_FILTER_CUT_OFF		0x0000000000000400
#define	FEATURE_UT_DAC_MODE				0x0000000000000800
#define	FEATURE_UT_GATE_MODE			0x0000000000001000
#define	FEATURE_UT_GATE_DELAY			0x0000000000002000
#define	FEATURE_UT_GATE_WIDTH			0x0000000000004000
#define	FEATURE_UT_GATE_THRESHOLD		0x0000000000008000
#define FEATURE_UT_GATE_FAR_DEAD_ZONE	0x0000000000010000
#define FEATURE_UT_TOF_MODE				0x0000000000020000

#define	REPORT_TEXT				1
#define	REPORT_IMAGE			2
#define	REPORT_RECTANGLE		3
#define	REPORT_LINE				4
#define	REPORT_DATE				5
#define REPORT_LIST				6
#define REPORT_BARGRAPH			7
#define	REPORT_DATA_VALUE		8
#define	REPORT_TABLE			9
#define	REPORT_ULTRASONICS		10
#define	REPORT_HEADER_FOOTER	11
#define	REPORT_BLADE_PLAN		12

#define	MOVE_LEFT		1
#define	MOVE_RIGHT		2
#define	MOVE_TOP		3
#define	MOVE_BOTTOM		4
#define	MOVE_ALL		5

#define LADDER_NO_ACTION	0
#define	LADDER_ABSOLUTE		1
#define	LADDER_RELATIVE		2

//data value type
#define	REPORT_SIZE_DETAILS				10
#define	REPORT_CUSTOMER_DETAILS			11
#define	REPORT_CONTRACT_DETAILS			12
#define	REPORT_SPECIFICATION_DETAILS	13
#define	REPORT_ITEM_DETAILS				14
#define	REPORT_TECHNIQUE_DETAILS		15
#define	REPORT_LOT_DETAILS				16
#define	REPORT_ALLOY_DETAILS			17
#define REPORT_ITEM_CODE_DETAILS		18
#define	REPORT_NUMBER_ITEMS_IN_LOT_DETAILS	19
#define	REPORT_HEAT_NO_DETAILS			20
#define	REPORT_FILE_NAME				21
#define	REPORT_SCANSIZE					22
#define	REPORT_DATE_TIME				23
#define REPORT_ULTRASONIC_SPEC_DETAILS	24
#define REPORT_CRITERIA_DETAILS			25
#define REPORT_DOCUMENT_DETAILS			26
#define REPORT_FORMULA_DETAILS			27
#define REPORT_PRODUCTION_FACILITY_DETAILS	28
#define REPORT_BATCH_NUMBER_DETAILS		29

#define	REPORT_DEPTH_0					60
#define	REPORT_DEPTH_1					61
#define	REPORT_DEPTH_2					62
#define	REPORT_DEPTH_3					63
#define	REPORT_DEPTH_4					64
#define	REPORT_DEPTH_5					65
#define	REPORT_DEPTH_6					66
#define	REPORT_DEPTH_7					67
#define	REPORT_DEPTH_8					68
#define	REPORT_DEPTH_9					62
#define PROBE_SERIAL_0					110
#define PROBE_SERIAL_1					111
#define PROBE_SERIAL_2					112
#define PROBE_SERIAL_3					113
#define PROBE_SERIAL_4					114
#define PROBE_SERIAL_5					115
#define PROBE_SERIAL_6					116
#define PROBE_SERIAL_7					117
#define PROBE_FREQ_0					120
#define PROBE_FREQ_1					121
#define PROBE_FREQ_2					122
#define PROBE_FREQ_3					123
#define PROBE_FREQ_4					124
#define PROBE_FREQ_5					125
#define PROBE_FREQ_6					126
#define PROBE_FREQ_7					127
#define PROBE_LENGTH_0					130
#define PROBE_LENGTH_1					131
#define PROBE_LENGTH_2					132
#define PROBE_LENGTH_3					133
#define PROBE_LENGTH_4					134
#define PROBE_LENGTH_5					135
#define PROBE_LENGTH_6					136
#define PROBE_LENGTH_7					137
#define PROBE_SPOT_0					140
#define PROBE_SPOT_1					141
#define PROBE_SPOT_2					142
#define PROBE_SPOT_3					143
#define PROBE_SPOT_4					144
#define PROBE_SPOT_5					145
#define PROBE_SPOT_6					146
#define PROBE_SPOT_7					147
#define PROBE_DIA_0						150
#define PROBE_DIA_1						151
#define PROBE_DIA_2						152
#define PROBE_DIA_3						153
#define PROBE_DIA_4						154
#define PROBE_DIA_5						155
#define PROBE_DIA_6						156
#define PROBE_DIA_7						157
#define	REPORT_AMP_AT_DEPTH_0			160
#define	REPORT_AMP_AT_DEPTH_1			170
#define	REPORT_AMP_AT_DEPTH_2			180
#define	REPORT_NOMINAL_GAIN				190
#define	REPORT_COMPENSATION_GAIN		200
#define	REPORT_INSPECTION_GAIN			210

//nScanMask
#define	_ENTER_FILENAME_AT_START			0x001
#define _FORCE_PROPERTY_PAGE				0x002
#define _PROPERTY_PAGE_MUST_BE_COMPLETE		0x004
#define _LOCK_FILE_ON_SENTENCE				0x008
#define _SCAN_START_VIA_DANGER_PLANE		0x010
#define	_LOCK_UT							0x020
#define	_LOCK_WP							0x040
#define	_ROLLERPROBE_BACKOFF				0x080
#define	_SAVE_INDIVIDUAL_CSCANS				0x100
#define _LOAD_UT_FILE_AT_SCAN_START			0x200
#define	_AUTO_1TO1							0x400
#define _LOAD_ROBOT_WEIGHTINGS_FROM_PROFILE	0x800
#define _DEGLITCH_FILTER					0x1000
#define _FILL_DPC_ERROR						0x2000

#define	_CREATE_COLUMNS						0x001
#define	_FILL_LIST							0x002
#define	_ADJUST_LAYOUT						0x004
#define	_FILL_LINE_LIST						0x100
#define _ENSURE_LINE_LIST_VISIBLE			0x200

//nProfileDisplayMask
#define	DISPLAY_TAUGHT_LINES	0x0001
#define	DISPLAY_SCAN_LINES		0x0002
#define	DISPLAY_ENVELOPE_LINES	0x0004
#define	DISPLAY_MORPH_LINES		0x0008
#define	DISPLAY_REF_POINTS		0x0010
#define FILL_IN_SCAN_LINES		0x0020
#define DISPLAY_BOTH_FACES		0x0040
#define DISPLAY_DANGER_PLANES	0x0080
#define DISPLAY_SCANNER			0x0100
#define CAD_SHADED				0x0200
#define CAD_WIREFRAME			0x0400
#define CAD_SELECTED_WIREFRAME	0x0800
#define DISPLAY_CARTESIAN_PLANES 0x1000
#define RBUTTON_PROFILE_MODE	0x2000
#define	DISPLAY_SLICE_PLANE		0x4000


//atof for unicode support, macro//////////////////////////
#define _WTOF(string,f) {					\
		WCHAR *STOPstring;					\
		f = (float)wcstod( string, &STOPstring );	\
		}
#define _WTOD(string,f) {					\
		WCHAR *STOPstring;					\
		f = wcstod( string, &STOPstring );	\
		}

enum ScannerDescription{
		DUAL_SIDED_SQUIRTER = 0,
		SINGLE_BRIDGE_TANK,
		DOUBLE_BRIDGE_TANK,
		SINGLE_BRIDGE_DUAL_SIDED,
		DUAL_BRIDGE_DUAL_SIDED,
		DUAL_SIDED_SQUIRTER_2,
		DUAL_SIDED_7_AXES,
		SINGLE_ROBOT,
		DUAL_ROBOT,
		SINGLE_BRIDGE_TANK_WITH_R,
		VERTICAL_BORE_SCANNER,
		SINGLE_BRIDGE_TT_ROLLERS,
		DUAL_ROBOT_9_PLUS_9,
		FB_11_AXIS,
		FB_TWIN_TOWER,
		SPX_ROBOT, //15
		RAILWAY_AXLE, //16
		TRACKED_PROBE, //17
		TANK_5AXIS, //18
		QUICKSTEP_FLATBED, //19
		TANK_6AXIS, //20
		TWIN_TOWER_KINEMATIC,	//21	Quickstep Twin Tower
	};

enum PM30AttachedDevices{
	PPA15 = 0,
	MUX8 = 1,
	LSA = 2,
};

enum ADCInputDevices{
	UNUSED =0,
	PM30_PPA15,
	PM30_MUX8,
	PM30_LSA, //Potential upto 4 of these should automatically increment through Beams. Linked to No. of PM30s in ADC class
	PM30_AA,
	PR2050,
};

#define	Acqiris_DP210		100
#define Alazar_9462			101
#define Alazar_860			102
#define	Alazar_9626			103
#define	Acqiris_U1071AHZ4	1071
#define	Acqiris_U1071AHZ2	1072
#define	Acqiris_DP1400		410
#define	SpectrumM3i_2120	32120
#define	SpectrumM3i_2122	32122
#define	SpectrumM3i_2130	32130
#define	SpectrumM3i_2132	32132


#define	SCAN_ALL			0
#define	SCAN_CURRENT_LINE	1
#define	SCAN_REMAINDER		2
#define	SCAN_PREVIOUS_LINE	3


#define	FB_AMPLIFIER_ENABLED	1
#define	FB_IN_POS				2
#define	FB_HOMED				4
#define	FB_MIN_SWITCH			0x10
#define	FB_MAX_SWITCH			0x20
#define	FB_SOFT_LIMIT			0x80
#define	FB_HARD_LIMIT			0x40
#define	FB_AMPLIFIER_FAULT		8
/////////////////////////////////////////////////////////////////////////////
// CUSLScannerApp:
// See USLScanner.cpp for the implementation of this class
//

struct	ResourcesStruct {
	int	nIOAddrs;
	int	nIORange;
	int	nMemAddrs;
	int	nMemRange;
	int	nIrq;
};

struct OptimizePaneMessage {
	int		nDirection;
	CPolyCoord *pLine;
	CCoord		*pCp;
};


class CLastSettings {

public:
	CLastSettings() {};
	~CLastSettings() {};

	int	nStructSize;

	CRect	RectWindowPos[MAXIMUMWINDOWSIDNUMBER];
	bool	bWindowOpenOnLastExit[MAXIMUMWINDOWSIDNUMBER];
	bool	bWindowPos;

	bool	bPr15Present;
	bool	bPl15Present;
	bool	bPr20Present;
	bool	bPr50Present;

	bool	bSparePresent;

	bool	bAtpGpd0Present;
	bool	bAtpGpd1Present;
	bool	bAtpTims0Present;
	bool	bAtpTims1Present;
	bool	bAdcPresent;
	bool	bDspPresent;
	bool	bMscPresent;
	bool	bPr30Present[8];
	bool	bMorePresent[2];

	int	nPr15Addrs;
	int	nPl15Addrs;
	int	nPr20Addrs;
	int	nPr50Addrs;
	int	nAtpGpd0Addrs;
	int	nAtpGpd1Addrs;
	int	nAtpTims0Addrs;
	int	nAtpTims1Addrs;
	int	nAdcAddrs;
	int	nDspAddrs;
	int	nMailBoxAddrs;
	int	nMscAddrs;
	int	nAdcMemoryAddrs;
	int	nPr30DestAddrs[8];
	int	nPr30SrcAddrs;
	int	nMoreAddrs[3];

	int	nProcessCard;
	int	nAdcCardType;
	int	nDspCardType;
	int	nPr15Version;
	int	nPl15Version;
	int	nVersionSpare;
	int	nAdcDelayClkPeriod;

	int	nShearWaveVelocity;
	int	nWaterVelocity;
	int	nMaterialVelocity;
	int	nMaterialConstant;
	float	fMaterialDensity;
	int	nLogRange;

	bool	bAutoEnableJoystick;
	int		nJoystickCommPort;
	float	fRollerDiameter;
	float	fRollerStepSize;

	int		nManipulatorType[2];		//Left, right
	int		nNotUsed;
	int		nNormalizeMode;
	float	fNormalizeAngle;
	float	fBallHeight;
	float	fBallDiameter;
	int		nBrainMeshMode;


	bool	bPr30Tab;
	bool	bPr20Tab;
	bool	bPr50Tab;
	bool	bPr15Tab;
	bool	bPl15Tab;
	bool	bScopeTab;
	bool	bGlobalTab;
	bool	bProbeTab;
	bool	bTimsTab;
	bool	bThicknessTab;
	bool	bDACTab;
	bool	bFlawTab;
	bool	bGatesTab;
	bool	bTimeSlotTabs;
	bool	bAmplitudeTab;
	bool	bRectilinearScanTab;
	bool	bTurnTableScanTab;
	bool	bRollerScanTab;
	bool	bContourScanTab;
	bool	bTubesScanTab;
	bool	bDiscScanTab;
	bool	bNormalizeTab;
	bool	bHoldingFixtureTab;
	bool	bKinematicsTab;
	bool	bPumpTab;
	bool	bManipulatorTab;
	bool	bDetailsTab;
	bool	bMotionToolsOriginTab;
	bool	bMotionToolsMoveToTab;
	bool	bMotionToolsTipAxesTab;
	bool	bUltrasonicsSelfTestTab;
	bool	bMotionToolsLiftTab;
	bool	bMotionToolsOperatorContourTab;
	bool	bMotionToolsScanLinesTab;
	bool	bMotionToolsRRWIPTab;
	bool	bMotionToolsRotateThetaLiftTab;
	bool	bProfileCartesianTab;
	bool	bProfileTurTableTab;
	bool	bProfileLaserTopographyTab;
	bool	bPorosityTab;
	bool	bPositionDualSidedTab;
	bool	bPositionSingleBridgeTab;
	bool	bPositionSingleBridgeDualSidedTab;
	bool	bPositionHoldingFixtureTab;
	bool	bProfile3DDesignTab;
	bool	bExtTrigScanTab;
	bool	bMotionMaterialVelocityTab;
	bool	bMotionJogTab;
	bool	bProfile3DViewTab;
	bool	bPositionDualRobotTab;
	bool	bPositionGraphicTab;
	bool	bMotionFlatScanTab;
	bool	bMotionRollerScanTab;
	bool	bProfile3DStiffenerTab;
	bool	bProfile3DBladeTab;
	bool	bMotionBladeScanTab;
	bool	bMotionVerticalBoreScanTab;
	bool	bUltrasonicsExtSourceTab;
	bool	bMotionRollerProbeTab;
	bool	bMotionArmPositionsTab;

	int		nLastViewOptionsTab;
	int		nLastPaletteTab;
	int		nLastUtTab;

	bool	bForceLogon;
	bool	bEnterFileNamePriorToScan_NotUsed;
	int		nTextSeparator;
	int		nGraticuleScrollMode;
	int		nPolarImageOrientation;
	bool	bUpdateDirectoryPath;
	bool	bRedBlueReverse;
	int		nImageOrientation;			//0=TL, 1=BL, 2=TR, 3=BR,
	int		nAmpPriority;
	int		nTimePriority;
	int		nViewPagePercent;
	float	fMeasureDistance;
	int		nJoystickTipMode;
	int		nPositionDisplayMode;
	int		nBrakeScannerMode;
	int		nBrakeHoldingMode;			//						nNotUsedPerspective;
	int		nLastMotionToolTab;
	int		m_nNumberProbesPerImage;					//nDefaultPerspective;//							nNotUsedToolPathL;
	int		nLastPmacTab;
	float	fFeedRate;
	int		nLastCompensationToolTab;
	float	fDACRefAmplitude;
	int		nDangerPlaneMoveMask;
	int		nLastPhasedArrayTab;
	int		nPAFilterDisplayMode;
	int		nPositionPaneMask;

	int		nSerialPortFIRCount;

	int		nRelearnTimeSlot;
	float	fRelearnApproachDistance;
	float	fRelearnOverShootDistance;
	int		nRelearnNormals[2];
	int		nRelearnSurfacePos[2];
	int		nRelearnPeak[2];

	int		nRemoveAirTime;

	float	fFFTMinFrequency;
	float	fFFTMaxFrequency;
	int		nFFTResolvableFrequency;
	int		nFFTGain;
	int		nFFTRfSource;
	int		nFFTTS;

	int		nGateAction;
	int		nDoorAction;
	int		nPeakSide;
	int		nLoadWaterPathsFromProfile;
	int		nInPosGainMultiplier;
	int		nRAxisGraticuleMode;
	int		nLast3DDrawingToolTab;
	float	fMaterialVelocityThickness;
	bool	bGotoViaDangerPlane;
	int		nUltrasonicSheetConfig;
	int		nScreenHorizontalSizeMM;
	int		nScreenWidthPixels;
	int		nTimoutPeriodMinutes;
	int		nEnvelopeAlgorithm;
	int		nPeakOrthogonalMode;
	int		nLoadOriginFromProfile;
	int		nNormSide;
	int		nOrientation3DEditImage;
	float	f3DXSign;
	float	f3DYSign;
	float	f3DZSign;
	float	fDangerMargin[2];
	int		nLastMaintenanceTab;
	float	fDeltaThetaAngle;

	float	fFastIncrement;
	float	fSlowIncrement;
	float	fFastScanSize;
	float	fSlowScanSize;
	float					fNotUsed;
	int		nScanSide;
	int		m_nRectilinearScanMode;
	int		nLeadScrewCompensationMode;
	int		nPorosityGraphType;
	float	fPorosityThreshold;
	int		nDataBaseSortColumn;
	int		nDataBaseSortDir;
	int		nHistogramPorosityStats;
	int		nFastAxis;
	int		nSlowAxis;
	int		nMaterialVelocityMode;
	float	fLeftEnlarge;
	float	fRightEnlarge;
	float	fTopEnlarge;
	float	fBottomEnlarge;
	int		nRulerFlagMode;
	float	fRemoveAirAngle[2];
	float	fDropOutAspectRatio;
	float	fDropOutArea;
	int		nHistogramDropOut;
	float	fDropOutPercentage;
	float	fDropOutMaxPercentage;
	int		nCheckPmacPrograms;
	int		nCheckPmacPLCs;
	int		nRAxisGraticuleReverse;
	int		nRotateSurfaceCoordinates;
	int		nVertexDisplayMode;
	int		nVertexAxisDisplayMask;

	int		nOptomiseOption;

	int		nImportToolPathDest;
	int		nImportAptInvertISpare;
	int		nImportAptInvertJSpare;
	int		nImportAptInvertKSpare;
	int		nImportAptUnits;
	int		nImportAptDirection;
	int		nImportExtractionMethod;
	int		nImportAptFileType;

	bool	bDisplayTaughtNotUsed;
	bool	bDisplayTaughtCrosses;
	bool	bDisplayTaughtLines;
	int		nDisplayTaughtPercentage;
	COLORREF	rgbDisplayTaughtColor;

	bool	bDisplayScanNotUsed;
	bool	bDisplayScanCrosses;
	bool	bDisplayScanLines;
	int			nDisplayNotUsed1;
	COLORREF	rgbDisplayScanColor;

	bool	bDisplayBrainNotUsed;
	bool	bDisplayBrainCrosses;
	bool	bDisplayBrainLines;
	int		nDisplayBrainPercentage;
	COLORREF	rgbDisplayBrainColor;

	bool	bDisplayEnvelopeNotUsed;
	bool	bDisplayEnvelopeCrosses;
	bool	bDisplayEnvelopeLines;
	int		nDisplayEnvelopePercentage;
	COLORREF	rgbDisplayEnvelopeColor;

	int		nViewGraticuleStyle;
	COLORREF	rgbViewGraticuleColor;
	LOGFONT	 lfViewGraticule;

	int											nAnotherSpare[4];

	int		nHistogramAbsoluteCummulative;
	LOGFONT	lfHistogram;
	bool	bHistogramFilled;
	float	fHistogramStepAmplitude;
	float	fHistogramAmpThreshold;
	int		nHistogramAreamode;
	bool	bHistogramMaxWidth;
	bool	bHistogramMaxHeight;
	int		nHistogramThresholdMode;
	int		nHistogramAmpDeltaThreshold;

	int		nCheckPmacIVariables;
	int		nCheckPmacMVariables;
	int		nCheckPmacMotionPrograms;
	int		nCheckPmacPlcPrograms;
	int		nCheckPmacPlccPrograms;

	float	fZeroBallDiameter;
	int		nWaterPathTS[2];

	int		nDisplayWhichLines;

	int		nSelfTestFrequency_Redundant;
	int		nSelfTestAmplitude_Redundant;
	int		nSelfTestNomAttenuation_Redundant;
	int		nSelfTestOffOn;
	float	fSelfTestMinFrequency_Redundant;
	float	fSelfTestMaxFrequency_Redundant;
	float	fSelfTestStepFrequency_Redundant;
	float	fSelfTestMainGain_Redundant;

	int		nValueAlgorithm;
	float	fValueCursorSize;
	int		nValueDataType;
	int		nValueSpare[5];

	int		nViewTabTSName;
	int		nViewTabGateName;

	float	fYDangerMargin[2];

	int		nCalculateMathematicalNormals;
	int		nDacBlankingUnits;
	int		nDacDelayUnits;
	int		nLoadUltrasonicsFromProfile;

	int		nThirdStartOffset;
	int		nThirdSamplesLine;
	int		nThirdNumberLines;
	int		nThirdMin;
	int		nThirdMax;

	bool	bUseAxesCompensation;
	bool	bUseBrainCompensation;
	int		nBrainOptomizeOption;
	int		nBrainMoveToOption;

	float	fMeshFastSize;
	float	fMeshSlowSize;

	bool	bHistogramPaintView[2];

	COLORREF	rgbAnnotation;
	LOGFONT	 lfAnnotation;

	int		nDisplayAreYouSure;

	float	fMeshTopMargin;
	float	fMeshBottomMargin;
	float	fMeshLeftMargin;
	float	fMeshRightMargin;

	int		nFFTWindow;

	int		nVolumeAscanGraticule;
	int		nLockSurface;
	int		nVolumeRfType;
	int		nVolumeCrossHairValue;
	int		nVolumeSpare[9];

	int		nPlaceComponentInScanner;

	float							NotUsedAgain;
	int		nMeasurementUnits;
	int		nImageEdge[8];
	int		nMuxVersion;
	bool							bRedundantExclusionZones[3];
	int		nReverseLineOrder;

	int		nWaterVelocitySide;
	int		nWaterVelocityTS;
	float	fWaterVelocityDistance;

	bool	bEnableSide[2];
	float	fZDangerMargin[2];

	int		nDisplayEditNormals;
	int		nDisplayEditWhichLines;
	int		nDisplayEditFirstLastCurve;

	int		nNormalizeXtip;
	int		nNormalizeYtip;
	int		nPeakMode[2];

	int		nOptomiseOpposite;
	int		nOptomiseNormals;
	int		nOptomiseWp;

	int		nHistogramMask;
	float	fHistogramRelativedB[2];	//Percentage

	int		nTesselateRows;
	int		nTesselateColumns;

	int		nTaughtSortSelection;
	int		nDisplayPositionCross;

	int		nIsometricOverlayMesh;
	COLORREF	rgbIsometricMeshColor;
	int		nIsometricMeshDensity;
	int		nIsometricDecimateSamples;
	int		nIsometricDecimateLines;
	int		nIsometricSpares[6];

	int		nDisplay3DMachine;
	int		nDisplay3DDangerZone;
	int		nDisplay3DEnvelope;
	int		nDisplay3DRefPoints;
	int		nDisplay3DTaughtLines;
	int		nDisplay3DScanLines;
	int		nDisplay3DEnvelopeLines;
	int		nDisplay3DRobot;
	int		nDisplay3DTracker;
	int		nDisplay3DBraneLines;
	int		nDisplay3DOutsideLines;
	int		nDisplay3DInsideLines;
	int		nDisplay3DWorksheet;
	int		nDisplay3DSpare[7];
	int		nDisplay3DCScanOverlayCoordinateFrame;
	int		n3DUpVector;

	float	fHistogramRelativeTime[2];
	float	fHistogramRelativeThickness[2];
	COLORREF	rgbHistogramBkColor;
	COLORREF	rgbHistogramColor[2];
	int		nHistogramSpare[18];

	//Filters
	FrequencyFilterType	nFilterType;
	float	fLowCutOffFreq;
	float	fHighCutOffFreq;
	float	fCentreFreq;
	float	fTopBandWidth;
	float	fBottomBandWidth;
	float	fPassRipple;
	float	fStopAttenuation;
	int		nNumberTaps;
	WindowedType	nFilterWindowType;
	int		nFilterSpare[10];

	int		nLastFilterToolTab;
	int		nLastVolumeToolTab;
	int		nLastWorkspaceTab;
	int		nLastPositionTab;
	int		nLastRobotTab;
	int		nLastCustomizeTab;
	int		nUltrasonicsTabMask;
	int		nMotoionToolsTabMask[2];
	int		nPaletteTabMask;
	int		nProfileTabMask;
	int		nLastTabsSpare;

	int		nCorrelationTaps;
	int		nCorrelationPreTrigger;
	int		nCorrelationPostTrigger;


	bool	bInternalPRF;
	bool	bContinuousPRF;
	bool	bPRFCreateInt;
	int		nHTMask;
	int		nSpareSi10[1];

	//Phase array
	int		nPAEdgeLine[5];
	int		nPAEdgeOffset[5];
	int		nPAEdgeDir;

	float	fIsometricVerticalExaggeration;
	bool	bInvertBScan;
	int		bPMACUsesHardwareIRQ;

	int		nProbeNumberSections;
	float	fProbeFastScanSize;
	float	fProbeSlowScanSize;
	float	fProbeFastIncrement;
	float	fProbeSlowIncrement;
	float	fProbeAtAttenuation;
	int		nProbeCharacterisationSide;
	int		nProbeCharacterisationTS;
	int		nProbeCharacterisationGate;
	bool	bProbePulseDuration;
	bool	bProbePeakFrequency;
	bool	bProbeCentralFrequency;
	bool	bProbeRatioBWCF;
	bool	bProbeBandWidth;
	bool	bProbeLowerFrequency;
	bool	bProbeUpperFrequency;
	bool	bProbeDisplayAttenuation;
	bool	bProbeDisplaySpotSize;
	bool	bProbeDisplayWaterPath;
	bool	bProbeDisplayNearZone;
	bool	bProbeTrackgate;
	int		nProbeSpare[20];

	int		nXMLSaveHeader;
	int		nXMLSaveCoordinates;
	int		nXMLSaveData;
	int		nXMLTOFRule;
	int		nXMLSpare[9];

	int		nHistogramStyle; //0-square, 1-circle
	bool	bHistogramEnterSize;
	float	fHistogramCircleSize;

	float	fAutoExclusionX;
	float	fAutoExclusionY;
	float	fAutoExclusionZ;

	ToolBarType	nToolBar[40];
	int		nUseStorePath[10];
	int		nReportImageScale;

	bool	bUSBI2CPresent;
	int		nUSBI2CComNum;

	int		nEccentricTimeSlot;
	int		nUseEccentricCompensation;
	int		nConvolutionFilter;
	int		nDifferentiatorThreshold;
	int		nSaveScanEveryLine;
	int		nDisplayTurnTableIn3D;
	MachineDrawingStyle		n3DMachineDrawingStyle;
	int		n3DSpare;
	int		n3DFillInScanLines;

	int		nCADFlags;
	int		nCADLineWidth;
	COLORREF	rgbCADColor;
	COLORREF	rgbCADText;
	LOGFONT	 lfCAD;

	COLORREF	rgbPorosityCurve[10];
	COLORREF	rgbStartToolPath;
	COLORREF	rgbFinishToolPath;
	COLORREF	rgbEOLToolPath;
	COLORREF	rgbReference;
	COLORREF	rgbAncillary;

	
	int		nBscanRectifyMode;
	int		bBWReverse;
	float	fBscanZoomLength;

	float	fWpVariation;

	int		nExtScanMode;
	int		nExtDataCollectionStrobeSrc;
	double	dExtDisplayPeriod;
	int		nExtSpare[10];
	
	bool	bDPRamWriteMemCheck;

	int		nDacMaxThreshold;

	int		nIOController;

	//Laser View
	float			fRotateOffset;
	float			fAngleRange;
	float			fMaxDistance;
	float			fMinDistance;

	//Laser Scan
	float			fLaserFastIncrement;
	float			fLaserSlowIncrement;

	int				nLaserLocation;
	int				nLaserIPAddrs;

	int				nLaserSamplesPerSecond;
	int				nLaserMotorRPM;
	int				nLaserAverageSamples;
	int				nLaserCallbackThresholdPercent;
	bool			bLaserDiscard;
	int				nLaserAverageLines;

	COLORREF		rgbStartEnvelope;
	COLORREF		rgbFinishEnvelope;

	int				nOverlayLineMode[10];
	COLORREF		rgbOverlayColor[10];
	bool			bOverlayDisplayCrosses[10];
	bool			bMaintainAspectRaioInScaling;
	bool			bIncludeRefPtsInScaling;
	int				nAnnotateApplyToAllImages;
	int				nPrintOptionsMask;
	float			fPrintPaperWidth;
	float			fPrintPaperHeight;
	int				nConeFontSize;
	int				nConeTextTransparent;
	COLORREF		rgbConeTextColor;

	int				nTunningRenderDacVoltage;
	int				nTunningRenderFollowingError;
	int				nTunningVelocity;
	int				nTunningSpare[10];

	float			fXJogDistance;
	float			fYJogDistance;
	float			fZJogDistance;
	float			fXtJogDistance;
	float			fYtJogDistance;
	float			fMinimumWaterPathJog;
	float			fMinVectorLengthJog[2];

	int				nCScanOverlayMode;
	int				nCScanOverlayResolution;

	D3DLIGHT9		light[8];
	D3DVECTOR		vecLightLookAtPt[8];
	WCHAR			strLight[8][30];

	COLORREF		rgbRoomWallColor;
	COLORREF		rgbVerticesColor;
	COLORREF		rgbBKPositionPane;
	COLORREF		rgbTextPositionPane;
	COLORREF		rgbSpareColors[8];

	int				nLightAmbient;

	float			fSpare_sdggfj;
	float			fSpare_fgjf;
	float			fSurfaceResolution;
	int				nSelectTaughtButtonOnNew;

	bool			bLaserSingleShot;
	bool			bLaserReverseEncoder;
	bool			bLaserFIR;
	bool			bContinuousLaserScan;
	bool			bLaserFlipDisplay;
	float			fLaserMirrorDistance;
	float			fLaserMaxYDeviation;
	float			fLaserBlockSize;
	int				nLaserMinValidAmplitude;
	int				nLaserMarkerDisplay;
	int				nLaserSpare[20];
	COORDDATA		m_CpLaserHole[3];
	COORDDATA		m_CpSquirterHole[3];

	COLORREF		rgbDisplaySurfaceColor;
	bool			bDisplaySurfaceLines;
	bool			bDisplaySurfaceCrosses;
	int				nHAxis;
	int				nVAxis;

	ADCInputDevices ADCInputDevice[8]; //Possible upto 8 inputs
	PM30AttachedDevices n_Not_used_anyMore_WhatIsConnectedToPr30;

	float			m_fWorkSpaceVDividerPos;
	int				nPloygonLengthAreaMode;

	float			fLaserMinY;
	float			fLaserMinZ;
	float			fLaserMaxY;
	float			fLaserMaxZ;
	D3DVECTOR		vLaserTranslation;
	bool			bLaserOffOn;

	COLORREF		rgbDropOut;

	LOGFONT			lfReport;

	D3DXVECTOR3				vecNotUsed;
	int				n1to1Monitor;
	int				n1to1Position;
	int				nCADTableDisplayMode;
	int				nCADDefectPaintMode;
	bool			nVirtualActualRobot;
	int				nRobotJogMode;

	int				nFaroTrackerType;
	int				nFaroTrackerIPAddrs;
	int				nFaroTrackerRunMode;
	int				nFaroTrackerCoordinateDisplayMode;
	int				nFaroRemotePageDown;
	int				nFaroDisplayJustify;
	int				nFaroDisplayRectangularPolar;
	int				nFaroRemotePageUp;
	float			fFaroCloudLength;
	COLORREF		rgbFaroText;
	COLORREF		rgbFaroBk;

	int				nCoordMask;
	int				nDisplayRobotScanEnvelope;
	int				nRobotSpare[19];

	float			fMultipleScansVDividerPos;

	PM30AttachedDevices	nAttachedToPr30[8];
	int				nUseWhichPR30;
	int				nPr30Version[8];

	int				nRTheta180Check;
	int				nRThetaLookBack;

	int				nWaypointMode;
	int				nNumberOfWayPoints;
	float			fWaypointXYZChange;
	float			fWaypointIJKChange;

	int				nOptomiseMoveRightToLeft;
	int				nExtrapolateMode;
	int				nSliceOffMode;
	int				nRTheta90Check;
	int				nRTheta360Check;
	int				nRThetaUseW;

	int				nProtractorPortNumber;
	int				nIJKDiagCombo;
	D3DXVECTOR3		vecTurnTableNormal;
	int				nUnTwistMode;
	int				nThicknessDisplayOptions;
	COLORREF		rgbCrossSection[10];
	int				nPositionGraphicMode;

	float			fProbeSeparation;
	__int64			n64CopyUtVariable;

	int				nViewGraticuleSpacingMode;
	float			fViewGraticuleSpacing;
	bool			bEthernetRemoteClientServer;
	int				nRemoteServerIPAddrs;
	int				nRenderHoleRefLengths;
	int				nImportDesignRulesMode;
	int				nThicknessTestPt0;
	int				nThicknessTestPt1;
	int				nCrossSectionMask;
	
	int				osThicknessCalibrationDate;
	int				osThicknessReCalibrationPeriodMinutes;
	bool			bThicknessCalibrationStatus;

	int				nDataPropertiesFontHeight;
	int				nParameterColumnWidth[4];

	int				bUseWhichPM30[8];
	int				nPALatRot;
	int				nAxialPlotAxes1;
	int				nAxialPlotAxes2;
	int				nWhichAxialPlot;
	int				nDesiredDevice;

	int				nLSAMask;
	int				nLSACalibrationGate;

	int				nPositionTabMask;
	int				nDataPropertiesMask;

	float			fThresholdTop;
	float			fThresholdSpan;
	COLORREF		rgbThresholdTop;
	COLORREF		rgbThresholdBottom;
	int				nThresholdMode;

	D3DXVECTOR3		vecEyePt;
	D3DXVECTOR3		vecLookAtPt;
	bool			bWireframe;

	int				nScanMask;

	bool			bBluetoothRemoteClientServer;
	int				nDefaultTOF;

	__int64			n64FeatureUtVariables;
	int				nPhasedArrayMask;
	int				nUtSubsetMask;
	__int64			n64SubsetUtVariable;

	float			fBladeMinX;
	float			fBladeMaxX;
	float			fBladeMinThick;
	float			fBladeMaxThick;
	float			fLePercentageDrop;
	float			fBladeMinTolerance;
	float			fBladeMaxTolerance;
	int				nBladeFontSize;
	int				nBladeDisplayMask;
	COLORREF		rgbBladeColor[2];
	bool			bDisplayBladeCrosses[2];
	bool			bDisplayBladeLines[2];
	int				nDisplayOutSideInside;
	float			m_fYBladeBorderPos;
	float			fBladeYOffset[2];
	int				nBladeAutoVerticalScale;
	int				nLEAlgorithm;
	int				nBladeCSVMask;

	int				nReverseScanLines;
	int				nReportAnnotationMultiple;
	int				nBothLanguagesOneColumn;
	int				nColumnJustification;
	int				nBluetoothDeviceNumber;
	int				nRemoteInterfaceEthernetBluetooth;
	int				nVolumeCScanGate;
	int				nWaterTemperature;
	int				nEthercatCommunicationMode;
	int				nExtSourceDisplayMode;

	float			fJogSpeed;
	float			fMoveSpeed;
	float			fScanSpeed;
	float			fNormalizeSpeed;
	float			fPeekOrthogonalSpeed;

	int				nUntwistDirection;
	int				nGotoWithStepOff;
	int				nExtraArmLength;
	int				nVolumeMonitor;
	float			fVolumeHScreenRatio;
	float			fVolumeVScreenRatio;

	int				nFieldBusCollectStrings;
	int				nStepSizePrecision;
	int				nScanType;
	int				nDefXSamples;
	int				nDefYSamples;
	float			fClassFSH[8];
	float			fWeightFSH[8];

	int				nAlarmTimeSlot;
	int				nAlarmGate;
	COLORREF		rgbAlarm[2];

	int				bUltrasonicsTrafficLightTab;
	int				bMotionTransPlyTab;
	int				bMotionAmplitudeTab;
	int				bMotionThicknessTab;
	int				bMotionRobotTab;
	int				bMoreTabs[16];
	int				nBraneUseDangerPlane;

	float			fVolGateWidthNs;
	float			fVolGateIntervalNs;
	float			fVolStartDepthNs;
	float			fVolStopDepthNs;
	int				nVolGatesL;
	int				nVolumeDisplayMask;

	int							nVolumeSpares[1];
	bool			bWSPumpsSwitchOff;

	unsigned int	nPhaseMask;
	int				nPhaseOverlayImageNumber;
	int				nPhasePositiveAmp;
	int				nPhaseNegativeAmp;
	int					nPhaseSpare[20];

	D3DXVECTOR4		vQuatTest;

	int				nHilscherDisplayMask;
	int				nLaserAxis;
	int				bLinearScanningArrayTab;
	int					bSpareTabs[20];

	int		nNormalizeTimeSlot[2];
	int		nNormalizeGate;

	float	fPeakDistance;
	int		nPeakTimeSlot[2];
	int		nPeakGate;

	D3DXVECTOR3		vRobotWeighting[2];

	int		nForceSi10FirmwareLoad;
	time_t	osLastStandardCalibration;
	time_t	osTimeBetweenStandards;

	float	fSpeedOrthogonal[2];
	float	fSpeedNormalize[2];
	float	fJogDirSpeedX[2];
	float	fJogDirSpeedY[2];
	float	fJogDirSpeedZ[2];
	float	fJogDirSpeedHTip[2];
	float	fJogDirSpeedVTip[2];

	LOGFONT	lfPositionPane;
	LOGFONT	lfAmplitudePalette;
	LOGFONT	lfThicknessPalette;
	LOGFONT	lfPorosity;
	LOGFONT	lfSpare[17];

	int		nAnnotateBkMode;
	COLORREF rgbAnnotateBkColor;

	bool	bUseAlernativeToSystemFont;
	int		nTextPercentageSize;
	LOGFONT lfMenus;

	int		nPrimarySide;

	int		nScopeMask;
	LOGFONT	lfScope;

	int		nRibbonMask;
	int		nRibbonSpare[30];

	int			nProfileDisplayMask;
	int							nMachineSpare[9];
	XMVECTORF32	vProfileAmbientColour;
	float		fProfileAmbientIntensity;
	int			nProfileLightOffOnMask;
	XMVECTORF32	vProfileLightDirection[8];
	XMVECTORF32 vProfileDiffuse[8];
	XMVECTORF32 vProfileSpecular[8];

	float		fDangerPlaneClearance[2];
	XMVECTORF32	rgbDangerPlane[2];
	XMVECTORF32	rgbFloodFill[2];
	XMVECTORF32	rgbLineColour[2][10];
	XMVECTORF32 rgbCADShade;
	XMVECTORF32 rgbCADWireFrame;
	XMVECTORF32 rgbCADSurfaceSelect;
	XMVECTORF32 rgbCADWireFrameSelect;
	XMVECTORF32 rgbBackGroundWorld;
	XMVECTORF32 rgbBackGroundProfile;
	XMVECTORF32 rgbSlice;
	XMVECTORF32								rgbSpare[97];

	float		fSquelchMax;
	float		fSquelchMin;
	float		fSquelchAttenuation;

	int			nDeGitchThreshold;
	int			nDangerPlaneDir[2];
	float		fDeltaScanSpeed;
	float		fDeltaThickness;
	float		fDeltaWaterPath;
	float		fDeltaScanIncrement;

	LOGFONT		lfRuler;
	int			nRulerWidth;
	COLORREF	rgbRulerLine;
	COLORREF	rgbRulerText;
	COLORREF	rgbRulerBk;
	int			nRulerStyle;
	int			nRulerTextMode;
	float		fRulerScale;
	int				nRulerSpare[6];

	int		nSpare[100];
};

struct SafetyRegions {
	int AxesL;
	int Axis[12];
	int Direction[12]; //0-Less than, 1-Greater than
	double Position[12];

	int FunctionL;
	MainThreadTimerActions PermissableFunctions[20];

	int nSpare[100];
};


struct TankStruct {
	int nStructSize;

	int nXLeft;
	int nYLeft;
	int nZLeft;
	int nXtLeft;
	int nYtLeft;
	int nRLeft;

	int nXRight;
	int nYRight;
	int nZRight;
	int nXtRight;
	int nYtRight;
	int nRRight;

	int	nXStep;
	int	nYStep;
	int	nZStep;
	int	nXtStep;
	int	nYtStep;
	int	nRStep;

	int	nLeftPump;
	int	nRightPump;

	int	nBrakeBit0;
	int nFrameToolJoyMode;
	int nSpeedBit;
	int nScannerFixtureBit;
	int nMainManipBit;

	int	nTenthAxisFeedForward;

	int	nXtLeftMaster;
	int	nXtRightMaster;
	int	nFingerLeftMaster;
	int	nFingerRightMaster;

	int	nTA;
	int	nTS;

	int	nGateBit;
	int	nDoorBit;
	int	nLeftAirBit;
	int	nRightAirBit;
	int	nBrakeBit1;
	int	nEmergencyBit;
	int	nDeadManBit0;
	int	nDeadManBit1;
	int	nLeftSideBit;
	int	nRightSideBit;
	int	nDockedBit;
	int	nAutomaticBit;
	int nKillBit;
	int	nLaserCoverBit;
	int	nSpareBits[1];

	JoystickSource	nJoystickSource;

	float	fXLength;
	float	fYLength;
	float	fZLength;
	float	fXSurfaceZero;
	float	fYSurfaceZero;
	float	fZSurfaceZero;

	int	nXBridge;					//Structure number for the bridge

	D3DXVECTOR3	vTTCentre;

	float	fPumpScaleFactor[2];

	bool	bHoldingFixture;			//Is there an holding fixture
	bool	bTurnTable;
	bool	bBridge;
	bool	bSpare[20];

	ScannerDescription nScannerDescription;

	SafetyRegions SafetyRegion[10];
	int		nSafetyRegionsL;
	int		nGeneralInputXOR;

	CCoord	CpSafe;

	float	fTTXOffset;			// for drawing 3D graphics
	float	fTTYOffset;
	float	fTTZOffset;
	float	fTTDiameter;

	int		nTM;
	int		nCommandLiftUp;
	int		nCommandLiftDown;
	int		nLiftUpLimitSwitch;
	int		nLiftDownLimitSwitch;
	int		nMachineFeatures;

	int		nAmpType[24];
	int		nBrakeInterlockPLCEnable;
	int		nManipulatorInterlockPLCEnable;

	int		nAndonOkay;
	int		nAndonCycleRunning;
	int		nAndonWarning;
	int		nAndonFault;

	int		nJoystickDeadBand;
	int		nSi10SwitchEor;
	int		nAndonOutputPort;
	int		nCalibrateAdc;

	int		nAudioPort;
	int		nAudioBit;
	int		nAudioMaskSelect;

	float	fServoUpdateRate;

	int		nPumpBit[2];
	int		nTrackingTenthAxis;

	int		nRobotOutputPort;
	int		nRobotOutCollectDataBit;
	int		nRobotOutSynchPulseBit;
	int		nRobotInputPort;
	int		nRobotInCollectDataBit;
	int		nRobotInSynchPulseBit;

	int		nGeneralInputXORSi10;

	D3DXVECTOR3	vecMarkerPen[2];
	int		nMarkerPenOnTime[2];
	int		nMarkerPenOutputPort[2];
	float	fPumpVerticalMultiplier[2];

	float	fADCTriggerLevel;
	int		nMaxADCConversionRate;
	int		nMaxPulseVoltage;
	unsigned int nPulserOffMask;

	float m_fTAPoly[3];
	float m_fTDPoly[3];
	float m_fTMPoly[3];
	float m_fTSPoly[3];

	float fToolLatchSpeed[2];
	float fToolRegionSpeed[2];
	D3DXVECTOR3	vToolRegion[2];
	D3DXVECTOR3	vToolLatch[2];

	unsigned char	cSi10GPIOIrqMode[8];

	int		nPumpMode[2];
	int		nFingerLeft;
	int		nFingerRight;

	//Remote page
	bool bStartRemoteServerOnStartup;
	int nRemotePort;
	int nRemoteConType;
	int		nRemoteSpare[40];
	float	fSpare;

	D3DXVECTOR3	vFrameZero[2];
	D3DXVECTOR3	vParkPt[2];
	D3DXVECTOR4	vParkNorm[2];

	int		nAOSIPAddrs[8];
	bool	bConnectAOSAtStartUp[8];

	float	fEthercatSyncShift[20];
	int		nAxisEnableMask[8];

	D3DXVECTOR3	vTTNorm;
	D3DXVECTOR3 vTTZeroDegrees;

	int		nSpare[100];

};


#define	FEATURE_TITLEBAR			1
#define	FEATURE_LEFT_GRATICULE		2
#define	FEATURE_RIGHT_GRATICULE		4
#define	FEATURE_TOP_GRATICULE		8
#define	FEATURE_BOTTOM_GRATICULE	0x10


class	FeatureStruct {

public:
	FeatureStruct() {};
	~FeatureStruct() {};

	int nStructSize;

	CRect	rectClient; //Position drawn on client
	CRect	rectUndoClient;
	bool	bModify;

	union 
	{
		struct {
			CRect rr;			//Top Left Corner storred 0.01% res
			CRect rectView;		//Size of Client at teach time
		};
		struct {
			CRect rrHeader;
			CRect rrFooter;
		};
	};
	int	nType;

	COLORREF rgbText;
	COLORREF rgbBkTextColor;
	int	nTextBkMode;
	LOGFONT	 lf;
	CString* pCaption;
	int	nCaptionPosition;

	int	nLineWidth;
	int	nLineStyle;
	COLORREF rgbLine;

	int	nImageType;
	int	nImageNumber;

	int	nBorderType;
	int	nBorderWidth;
	COLORREF rgbBorderColor;

	union 
	{
		struct {
			bool bLeftGraticule;
			bool bRightGraticule;
			bool bTopGraticule;
			bool bBottomGraticule;
		};
		struct {
			bool bHeader;
			bool bFooter;
			bool bSpare[2];
		};
	};

	int nDateFormat;
	int	nWhichDate;

	int	nListType;

	int nDataValueType;

	int	nScalePosition;

	int	nNumberRows;
	int	nNumberColumns;
	union
	{
		struct {
			COLORREF rgbTitle;
			COLORREF rgbBkTitle;
		};
		struct {
			COLORREF rgbHeader;
			COLORREF rgbFooter;
		};
	};

	__int64 n64UtVariables;

	unsigned int unFlags;
	unsigned int *pnIDArray;
	int	nPageNumber;
	int	nLanguage;

	bool AllocateIDArray()
	{
		SAFE_DELETE(pnIDArray);
		if(nNumberRows * nNumberColumns) {
			pnIDArray = new unsigned int[nNumberRows * nNumberColumns];
			ZeroMemory(pnIDArray, nNumberRows * nNumberColumns * sizeof (int));
			return true;
		};
		return false;
	};

	inline void operator =(FeatureStruct *pFe)
	{
		nStructSize = pFe->nStructSize;
		rectClient = pFe->rectClient;
		rectUndoClient = pFe->rectUndoClient;
		bModify = pFe->bModify;
		rr = pFe->rr;
		rectView = pFe->rectView;
		nType = pFe->nType;

		rgbText = pFe->rgbText;
		rgbBkTextColor = pFe->rgbBkTextColor;
		nTextBkMode = pFe->nTextBkMode;
		lf = pFe->lf;
		nCaptionPosition = pFe->nCaptionPosition;
		nLineWidth = pFe->nLineWidth;
		nLineStyle = pFe->nLineStyle;
		rgbLine = pFe->rgbLine;

		nImageType = pFe->nImageType;
		nImageNumber = pFe->nImageNumber;

		nBorderType = pFe->nBorderType;
		nBorderWidth = pFe->nBorderWidth;
		rgbBorderColor = pFe->rgbBorderColor;

		nDateFormat = pFe->nDateFormat;
		nWhichDate = pFe->nWhichDate;

		nListType = pFe->nListType;
		nDataValueType = pFe->nDataValueType;

		nScalePosition = pFe->nScalePosition;

		nNumberRows = pFe->nNumberRows;
		nNumberColumns = pFe->nNumberColumns;
		rgbTitle = pFe->rgbTitle;
		rgbBkTitle = pFe->rgbBkTitle;
		unFlags = pFe->unFlags;

		SAFE_DELETE(pCaption);
		pCaption = new CString(*pFe->pCaption);

		SAFE_DELETE(pnIDArray);
		if(AllocateIDArray()) {
			CopyMemory(pnIDArray,pFe->pnIDArray,nNumberRows * nNumberColumns * sizeof (int));
		};

		n64UtVariables = pFe->n64UtVariables;
		nPageNumber = pFe->nPageNumber;
		nLanguage = pFe->nLanguage;
	}

};

enum CtrlType {
	TEXT_CTL = 1,
	EDIT_CTL,
	BUTTON_CTL,
};

#define	CTL_LEVEL_0			0x0001
#define	CTL_LEVEL_1			0x0002
#define	CTL_LEVEL_2			0x0004
#define	CTL_DATE			0x0008
#define	CTL_OPERATOR		0x0010
#define	CTL_TIME			0x0020
#define CTL_PRINT			0x0040
#define	CTL_FILENAME		0x0080
#define	CTL_FULLPATHNAME	0x0100
#define	CTL_HIGHLIGHT		0x80000000L

struct	DescriptionStruct {
	int nStructSize;

	CString*	pCaption;
	CRect		rrCtrl;
	CtrlType	nCtrlType;
	LOGFONT		lf;
	int			nStyle;
	int			nActionFlag;
	int			nExStyle;
	int			nSpare[8];
};

enum AndonEventType {
	Undefined,
	MachineOkay,
	CycleRunning,
	MachineWarning,
	MachineFault,
};

#define	COLLECT_SIZE	100000

struct DataCollectionStruct {
	int nAmplitude[8];
	int	nNsPath[8];
};

struct TimeSlotDataCollectionStruct {
	int	nFieldBusInputReg;
	int	nFieldBusCount;

	DataCollectionStruct TS[8];
};


class TrackerBusyListener : public BusyListener
{
	public:
		void stateChanged(BusyEvent* event) {};
};

class CUSLScannerApp : public CWinAppEx
{
public:
	~CUSLScannerApp();
	CUSLScannerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUSLScannerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
//	virtual BOOL LoadState(CFrameWndEx * pFrame = NULL, LPCTSTR lpszSectionName = NULL);
//	virtual BOOL LoadState(LPCTSTR lpszSectionName = NULL, CFrameImpl* pFrameImpl = NULL) override;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation

public:
	CUSLThread		m_USLThread[MAX_THREAD_COUNT];
	void InvertNormScannerSpecific(CCoord* Cp, int nSide );
	LARGE_INTEGER getHighResCount();
	double getHighResTime();
	int getElapsedTime_us(LARGE_INTEGER StartCount, LARGE_INTEGER *pStopCount = NULL);
	bool StartThread(CString Name, AFX_THREADPROC pfnCallback, LPVOID pParam, int nRefreshRate_ms, int nPriority);
	bool SuspendThread(LPVOID pParam);
	bool IsAccessible(int nID);
	bool IsAccessLevel(AccessLevel Level);
	void TestTurntableRollOver();
	void FormatTime(CString & Buff, int nTimens, int nUnits, CString Units);
	int ScanfDistanceTimens(CString Buff, int nUnits);
	void CheckQuaternionMaths();
	void AddToLogging(CString *pBuff);
	void StopLogging();
	void StartLogging();
	bool m_bCollectBladeData;
	void SetStringOnToClipboard(CString Buff);
	CRemoteInterface	m_RemoteInterface;
	CRemote*            m_pRemote = NULL; 
	CEgismosRangefinder* m_pEgismosRangefinder = NULL;
	CFLSC10Rangefinder* m_pFLSC10 = NULL;
#ifdef NPTRACKINGTOOLS
	CV120Trio*          m_pV120Trio;
#endif
	int	m_nOldPumpArray[4];
	void AudioAlarm();
	void ConfineToScreen(CRect *pRect);
	void ConfineToScreen(RECT *pRect);
	void SaveMaintenanceVariables();
	void RetrieveMaintenanceVariables();
	void SaveCardEntries();
	void RetrieveCardEntries();
	void SaveRegistryVariables();
	float TimeToThickness(int nTimens, float * pfThickness = NULL, int nUnits = 0, CString * pBuff = NULL);
	void RetrieveRegistryVariables();
	CStringArray	m_strToolPath[10];

	void SaveRestore();
	CADElementType m_nCADTool;

	int m_nFilter[5][5];

	void ComputeVelocityTable();
	void CStringToChar(CString str, char* chr);
	void CharToCString(char* chr, CString &str);

	COLORREF	m_rgbStatusMessageText;
	COLORREF	m_rgbStatusMessageBk;
	CString		m_StatusMessage;
	void AndonEvent(AndonEventType nAction, CString Message);
	void ActivityLog(CString Activity);
	CFIR Fir;
	CRRMES m_RRMES;
	int m_nScratch;

	CString m_SafetyBitDescription[16];		//Maximum 16 bits of input
	int	m_nSafetyLogic[1024][16];			//Maximum 1024 possible safety options

	LARGE_INTEGER m_CPUFrequency;
	bool m_b3DModified;
	int	m_n3DImageType;
	int	m_n3DImageNumber;
	int	m_n3DPaletteNumber;
	CData* m_p3DData;
	CRobot m_Robot[2];


	CRRImportCSVThicknessOverlay m_RRImportCSVThicknessOverlay[2];

	CStringArray	m_Strings;
	CStringArray	m_AnnotateReasons;
	CString	m_PmacProgramPath;
	CString m_AndonPathName;
	CString m_DataBasePathName;
	CString m_DataBaseFailedToSavePathName;
	CString m_HelpPathName;
	CString	m_LimitAndKinematicOffset;
	CString m_ThicknessCalibrationPathName;
	CString m_DlgSideName[3];
	CString	m_AcrobatPathName;
	CString	m_WordPadPathName;
	CString m_USLScannerManualPathName;
	CString m_PMACSRMPathName;
	CString	m_TrackerPathName;
	CString m_WorkspacePathName;
	CString	m_CalibrationPathName;
	CString m_EndEffectorPathName[2];
	CString m_ProbeSerialNumber[2];
	CString	m_CalibrationComponent[2];
	CString m_3PtName[3];

	CString m_Degrees;
	CString m_Units;
	float	m_fUnits;
	int		m_nUnitsPrecision;
	int		m_nWpPrecision;
	float	m_fRotateThetaRevNumber[2];
	int		m_nUseTurntableOrRollers;

	CFile m_FileLogging;
	CArchive* m_pLoggingAr;

	int nPercentageTextSize[8] = { 20, 50, 70, 100, 120, 150, 170, 200 };
	void FormatLength(float fValue, int nPresicion,CString &Buff);
	void GetAvailableMemory();
	CBootUpDlg* m_pBootUpDlg;
	bool m_bCollectTunning;
	CData* m_pScanData;			//This is where data is sent that is generated during a scan
	CData* m_pViewData;			//This is the top most cscan so 3D stuff knows which one to access;
	VOID* m_pViewSheet;	//This is the top most View sheet so 3D stuff knows which one to access;
	bool m_bResetTimeOutTimer;
	bool m_bCollectNormalizeData;
	bool m_bCollectComplexData;
	bool m_bCollectEccentricData;
	bool m_bCollectWaterPath;
	bool m_bLaserCollectTaughtLines;
	bool m_bRThetaJoystick;
	bool m_bCtrlButton;
	bool m_bShiftButton;
	bool m_bAudioAlarm;
	bool m_bPumpOffOnStatus[2];
	bool m_bHiColorIcons;
	CString m_Operator;
	int	m_nAccessLevelMask;
	int m_nScanWhat;
	int m_nSortAxis;
	int	m_nSortDir;
	int m_nElementIndex;
	void ResumeThreads();
	void SuspendThreads();
	int	m_nArmCount[2];
	CJoints m_Arm[2][MAX_ARM_ENTRIES];
	HoldingFixtureCoord m_ChBall;	//Holding fixture Ball position
	CCoord m_CpRandomPosition;
	CCoord m_CpBall;				//Coordinate system Ball Zero
	int			m_nLadderL;
	CPolyCoord *m_pLadder;
	CPolyCoord m_CopyLine;
	CPolyCoord m_ThickCal;
	CPolyCoord* m_pCopyManyLine;

	int					m_nCopyLinesL;
	CPolyCoord			*m_pCopyLines;
	CPolyCoord			m_CopyCoords;

	int	m_nManyLineL;
	float m_fPath[300000];
	int	m_nBladeLine;
	int m_nLine;
	int m_nIndex;
	int m_nEditLineType;
	int	m_nListOrderArray[1024];	//Used by the faro tracker list
	bool CheckScreenSettings();
	bool SaveToolChanger();
	bool RetrieveTank();
	bool SaveTank();
	bool RetrieveLastSettings();
	bool SaveLastSettings();
	void RetrieveTank(CUSLFile* pFile);
	void SaveTank(CUSLFile* pFile);
	void RetrieveLastSettings(CUSLFile* pFile);
	void SaveLastSettings(CUSLFile* pFile);
	void GetWorkingDirectory();
	void GlobalDefaults(PropertiesList nWhichList = PropertiesList::All);
	void CheckLastSettingsPathName();
	void SetLanguage();
	void SetFontLanguage(LOGFONT * plf);
	void ChangeGlobalUnits();
	bool FirstInstance(CString strVersion);
	void LinearTracker();

	int	m_nSuspendProcessingCount;
	int	m_nLanguage;
	int	m_nPrimarySide;
	int	m_nSecondarySide;
	int	m_nSide0Orientation;

	CString m_DetailsWindowName;
	CString m_WallImagePathName;
	CString m_UtFileTitle;
	CString m_PmacPath;
	CString m_ScanPath;
	CString m_UltrasonicsPath;
	CString m_AptSourcePath;
	CString m_ViewReportTemplateName;
	CString m_PalettePath;
	CString m_ProfilePath;
	CString m_ArchivePath;
	CString m_WorkingDirectory;
	CString m_StorePath[10];
	CString	m_LastSettingsPath;
	CString	m_ProbesPath;

	CAOSPhasedArray		m_AOSPhasedArray;
	CDataTranslationHW	m_DataTranslationHW;
	CUSLPalette			m_Palette;
	CAlazarDigitizer	m_Alazar;
	CAcqirisDigitizer	m_Acqiris;
	CSpectrumDigitizer	m_Spectrum;
	CKinematics			m_Kinematics;
	CMainThread			m_Thread;
	CCriticalSection	m_CriticalSection;

	ResourcesStruct	m_Si6Res;
	ResourcesStruct	m_UtRes;
	ResourcesStruct	m_PmacRes;

	bool m_bScanning;
	bool m_bSi6Hardware;
	bool m_bUtHardware;
	bool m_bServoHardware;
	bool m_bUTEthernet;
	bool m_bFaroTrackerConnected;

	int	m_nExitMode;
	int	m_nRealPRFPerTS;
	int	m_nRealTotalPRF;
	int	m_nMotorType;
	int m_nJobNumber;
	int	m_nInitialLogonLevel;
	int	m_nLogonLevel;
	int	m_nLogonLevelMask;
	unsigned char m_cAccess[0x10000];

	float m_fXScanOffset;
	float m_fYScanOffset;
	float m_fZScanOffset;
	float m_fRotateAboutXDuringScan;
	float m_fRotateAboutYDuringScan;
	float m_fRotateAboutZDuringScan;

	COLORREF			m_rgbToolBar;
	int					m_nAppLook;
	int					m_nLazerLoopCount;
	CHelp				m_Help;
	CData				m_Data;
	CSerialJoystick		m_SerialJoystick;
	CMotors				m_Motors;
	CCoordTracker*      m_pTracker = NULL;
	TankStruct			m_Tank;
	CUSLut				m_UtUser;
	CServo				m_ServoUser;
	CSi6				m_Si6User;
	CData				m_ScanData;
	CScope				m_Scope;
	CProfile			m_Profile;
	AxisData			m_Axes[32];
	CLastSettings		m_LastSettings;
	CString				m_strVersion;
	float				m_fVersion;
	unsigned __int64	m_nLicenseMask;
	DescriptionStruct	*m_pDescription = NULL;
	int					m_nDescriptionL;
	CSI10x64			m_Si10Userx64;
	CSi10				m_Si10User;
 	CLSA				m_LSA;
	CUSBtoI2C			m_USBtoI2C;
	CPmac				m_PmacUser;
	CWorkSpace			m_WS;
	AndonEventType		m_nOldAndonAction;
	CSafetySerial		m_SafetySerial;
	CFieldBusController m_FBCtrl;
	Tracker*			m_pFaroTracker = NULL;
	CWorkerThread		m_FaroWorkerThread;
	CUSBKey				m_usbKey;
	CPhasedArrayProbe	m_PhasedArray[2];
	CEthercat			m_Ethercat;
	CInclusions			m_Inclusions;
	CToolChangers		m_ToolChangers;
	CPM40				m_PM40User;



	int m_nTrackingTS;
	int m_nTrackingAmp[8];
	int m_bTracking;
	float m_fTrackingLogGain;
	float m_fTrackingLinGain;

	int		m_nNumADCs;



	//Program Name Variable
	int SplitString(CString InitString, CStringArray *OuputStrings, int nMaxStrings, bool ToLowerCase=false);
	CString m_ProgramName;

	//{{AFX_MSG(CUSLScannerApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CUSLScannerApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USLSCANNER_H__EDD7806F_CD5B_4FE6_9728_BD63C9AA3A36__INCLUDED_)
