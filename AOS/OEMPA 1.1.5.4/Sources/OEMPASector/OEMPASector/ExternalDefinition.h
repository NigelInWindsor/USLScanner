
#pragma once


typedef struct structInfo
{
	int iCycleCount;			// Cycle quantity for one Ascan
	unsigned int PointQuantity;	// Point quantity for one Ascan
	double* pdX;				// table of emergence point (offset) for each cycle (meter).
	double* pdAngle;			// table of angle for each cycle (radian).
}structInfo;

typedef struct structSystem
{
	double dVelocity;			// Ultrasound velocity in the material.
	double dDigitizingPeriod;	// Digitizing frequency corresponding period (cycle).
	double dEncoderStep;		// mechanical acquisition step.
	double dEncoderLength;		//mechanical acquisition length.
	bool bInternal;				//internal synchronisation for the encoder.
	bool bCscanTofEnable;		//fw time of flight ON.
	double dTofMax;				//maximum time of flight (unit digitizing period).
	bool bGateEnable[4];		//gate.
	double dTofStart[4];		//gate start (unit digitizing period).
	double dTofRange[4];		//gate range (unit digitizing period).
}structSystem;

struct structCscanAmp_0x0102;
struct structCscanAmpTof_0x0202;

__declspec(dllimport) bool GetSystem(structSystem& system);
__declspec(dllimport) bool GetInfo(int iSystem, structInfo& Info);
typedef bool (WINAPI* pTypeCallbackAscan)(int iSystem, int iCycle, double dEncoder, const char *pData, int iSize);
typedef bool (WINAPI* pTypeCallbackCscan)(int iSystem, int iCycle, double dEncoder, const structCscanAmp_0x0102 *pAmp, const structCscanAmpTof_0x0202 *pAmpTof, int iSize);
__declspec(dllimport) void SetDemoCallback(pTypeCallbackAscan pCallbackAscan,pTypeCallbackCscan pCallbackCscan);
__declspec(dllimport) bool DemoInit(int iSystemCount);
__declspec(dllimport) bool DemoLoad(int iSystem, CString strFileBin, CString strFileTxt);
__declspec(dllimport) bool DemoReset();
__declspec(dllimport) bool DemoRun(bool bStart,double dFactor,HWND hWndEnd);
__declspec(dllimport) bool DemoExit();
__declspec(dllimport) bool DemoDestroy();
__declspec(dllimport) long GetEncoderResolution();
__declspec(dllimport) bool AutoResetAcq(double dEncoder);
__declspec(dllimport) bool IsMechanicalManagement(bool Ascan);

//integration
typedef void (*pAllocDisplayCallback)(bool bAlloc);
__declspec(dllimport) void SetAllocDisplay(pAllocDisplayCallback pCallback,CWnd* pMainFrame);
__declspec(dllimport) void DemoHWDialog(CWnd *pWnd,UINT &uiUTEventMessage,double &dPieceThickness,double &dAcqLength,bool &bDisplayLastButOne);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Definition des constantes de test
const double PI = 3.14159;
const BOOL MANAGE_REBOUND = FALSE;						// Defines whether we manage rebound or not
const unsigned int CYCLE_QUANTITY_1_MAX = 128;			// maximum cycle quantity for the probe 1
const unsigned int CYCLE_QUANTITY_2_MAX = 128;			// maximum cycle quantity for the probe 2
const unsigned int POINT_QUANTITY_1_MAX = 1024 * 4;     // maximum point quantity of the Ascan for the probe 1 
const unsigned int POINT_QUANTITY_2_MAX = 1024 * 4;     // maximum point quantity of the Ascan for the probe 2
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern char** AscanMemory1;								// Ascan memory for the probe 1
extern char** AscanMemory2;								// Ascan memory for the probe 2
extern BYTE* CscanAmpMemory1[4];						// Cscan memory for the probe 1 (4 gates)
extern BYTE* CscanAmpMemory2[4];						// Cscan memory for the probe 2 (4 gates)
extern short* CscanTofMemory1[4];						// Cscan memory for the probe 1 (4 gates)
extern short* CscanTofMemory2[4];						// Cscan memory for the probe 2 (4 gates)
extern int g_iCscanAmpMemorySize1;						// Cscan memory for the probe 1

extern CRITICAL_SECTION CRT_MEMORY_1;					// Critical section to access the probe 1
extern CRITICAL_SECTION CRT_MEMORY_2;					// Critical section to access the probe 2

extern CRITICAL_SECTION CRT_UPDATE_SETTING;				// Critical section to manage the dynamic update of setting

extern unsigned int g_uiCycleQuantity[2];
extern unsigned int g_uiPointQuantity[2];
extern unsigned int g_uiProbe;
extern double g_dMinimumAngle[2];
extern double g_dStepAngle[2];
extern double g_dEncoderAxisLength;
extern double g_dEncoderAxisStep;
extern double g_dPieceThickness;
extern double g_dVelocity;
extern double g_dSamplingPeriod;
extern bool g_bAscanDisplayLastButOne;//integration
extern bool g_bBackwallRebound;

// La fonction retourne true si pas d’erreur et false sinon.
bool acq_data_ascan(int iProbe,						// numbering of the probe (0 ou 1)
			  int iCycle,						// numbering of the  cycle
			  double dEncoder,					// Ecoder value (meter)
			  int iDataCount,					// count the acquisition point
			  const char* pAcqData);			// Buffer of acquisition data

// La fonction retourne true si pas d’erreur et false sinon.
bool acq_data_cscan(int iProbe,						// numbering of the probe  (0 ou 1)
			  int iCycle,						// numbering of the  cycle
			  double dEncoder,					// Ecoder value (meter)
			  int iDataCount,					// count the acquisition point
			  const structCscanAmp_0x0102 *pAmp, const structCscanAmpTof_0x0202 *pAmpTof);			// Buffer of acquisition data

extern structInfo InfoProbe1;					// Information for the probe 1
extern structInfo InfoProbe2;					// Information for the probe 2
extern structSystem System;

extern void InitProgram();						// Initialize the data
extern void CloseProgram();						// Clean memory
extern unsigned int g_LastAscanEncoderPosition;		// Last updated encoder position



/////////////////////////////////////////////////////////////////////////////////
// FOR ANALYSIS
extern char** AscanAnalysisMemory1;
extern char** AscanAnalysisMemory2;

extern CRITICAL_SECTION CRT_ANALYSIS_MEMORY_1;
extern CRITICAL_SECTION CRT_ANALYSIS_MEMORY_2;

extern structInfo InfoProbe1Analysis;			// Information for the probe 1 for Analysis
extern structInfo InfoProbe2Analysis;			// Information for the probe 2 for Analysis
extern structSystem SystemAnalysis;
extern unsigned int g_uiProbeAnalysis;
extern unsigned int g_uiMode;
extern double g_dEncoderAxisLengthAnalysis;
extern double g_dPieceThicknessAnalysis;

extern double Analysis_YPOS;
extern double Analysis_XMIN;
extern double Analysis_XMAX;
extern double Analysis_YMIN;
extern double Analysis_YMAX;
extern double Analysis_ZMIN;
extern double Analysis_ZMAX;

extern double Analysis_X_Axis_MIN;
extern double Analysis_X_Axis_MAX;
extern double Analysis_Y_Axis_MIN;
extern double Analysis_Y_Axis_MAX;
extern double Analysis_Z_Axis_MIN;
extern double Analysis_Z_Axis_MAX;
/////////////////////////////////////////////////////////////////////////////////

const UINT_PTR TIMER_DYNAMIC_CORRECTED_VIEW = 100;
const UINT_PTR TIMER_DYNAMIC_NON_CORRECTED_VIEW = 101;
const UINT_PTR TIMER_ASCOPE_VIEW = 102;
const UINT_PTR TIMER_BSCOPE_VIEW = 103;
const UINT_PTR TIMER_CSCOPE_VIEW = 104;
const UINT_PTR TIMER_DSCOPE_VIEW = 105;
const UINT_PTR TIMER_UPDATE_ANALYSIS = 106;