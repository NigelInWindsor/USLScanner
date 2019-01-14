
#pragma once

const int g_iEncoderMaxCount=32;//maximum encoder count.

#ifdef PSAPI_STUB_EXPORTS
	#ifndef PSAPI_STUB_API
		#define PSAPI_STUB_API __declspec(dllexport)
	#endif PSAPI_STUB_API
#else
	#ifndef PSAPI_STUB_API
		#define PSAPI_STUB_API __declspec(dllimport)
	#endif PSAPI_STUB_API
#endif

PSAPI_STUB_API BOOL IsWow64();

PSAPI_STUB_API bool GetKernelPathExe(const wchar_t *wcFileName,int iSize,wchar_t *wcPathExe);
PSAPI_STUB_API bool GetKernelMsgBoxCmd(int iSize,wchar_t *pText);
PSAPI_STUB_API const wchar_t* GetKernelMsgBoxCmd();
PSAPI_STUB_API bool SetKernelMsgBoxCmd(const wchar_t *pText);
PSAPI_STUB_API bool GetKernelMsgBoxText(int iSize,wchar_t *pText);
PSAPI_STUB_API const wchar_t* GetKernelMsgBoxText();
PSAPI_STUB_API bool SetKernelMsgBoxText(const wchar_t *pText);
PSAPI_STUB_API bool GetKernelMsgBoxTitle(int iSize,wchar_t *pText);
PSAPI_STUB_API const wchar_t* GetKernelMsgBoxTitle();
PSAPI_STUB_API bool SetKernelMsgBoxTitle(const wchar_t *pText);
PSAPI_STUB_API HWND GetKernelMsgBoxHwnd();
PSAPI_STUB_API void SetKernelMsgBoxHwnd(HWND hWnd);
PSAPI_STUB_API LONG GetKernelMsgBoxType();
PSAPI_STUB_API void SetKernelMsgBoxType(LONG alSharedKernelMsgBoxType);
PSAPI_STUB_API LONG GetKernelMsgBoxReturn();
PSAPI_STUB_API void SetKernelMsgBoxReturn(LONG alSharedKernelMsgBoxReturn);
PSAPI_STUB_API HWND GetKernelMsgBoxHwndDebug();
PSAPI_STUB_API void SetKernelMsgBoxHwndDebug(HWND hWnd);


typedef struct structToolboxWizard{
	//Wizard beam management
	int iCount;
	int iIndex;
	double dProbeX,dProbeY,dProbeZ;
	double dEmergenceX,dEmergenceY,dEmergenceZ;
	double dFocalX,dFocalY,dFocalZ;
	wchar_t pInput[MAX_PATH];
	wchar_t pOutput[MAX_PATH];
	//Wizard device
	wchar_t pIPAddress[50];
	int iPort;
	int iDeviceId;
} structToolboxWizard;
#ifndef DllExport
extern "C"{
	__declspec(dllimport) structToolboxWizard g_ToolboxWizardParameter;
}
#endif

#define WPARAM_TOOLBOX_HWND					0x00000001
#define WPARAM_TOOLBOX_CLOSE				0x00000002
#define WPARAM_TOOLBOX_WIZARD_NEW			0x00000003
#define WPARAM_TOOLBOX_WIZARD_DEL			0x00000004
#define WPARAM_TOOLBOX_WIZARD_UPDATE		0x00000005
#define WPARAM_TOOLBOX_WIZARD_RESET			0x00000006
#define WPARAM_TOOLBOX_WIZARD_ISENABLED		0x00000007
#define WPARAM_TOOLBOX_LOAD_WIZARD			0x00000008
#define WPARAM_TOOLBOX_SAVE_WIZARD			0x00000009
#define WPARAM_TOOLBOX_LOAD_MATRIX			0x0000000A
#define WPARAM_TOOLBOX_SAVE_MATRIX			0x0000000B
#define WPARAM_TOOLBOX_WIZARD_BEAM_COUNT	0x0000000C
#define WPARAM_TOOLBOX_WIZARD_BEAM			0x0000000D
#define WPARAM_TOOLBOX_WIZARD_ELEMENT_COUNT	0x0000000E
#define WPARAM_TOOLBOX_WIZARD_ELEMENT		0x0000000F
#define WPARAM_TOOLBOX_TOOLBOX_GET			0x00000010
#define WPARAM_TOOLBOX_TOOLBOX_SET			0x00000011
#define WPARAM_TOOLBOX_KEEPALIVE			0x00000012
#define WPARAM_TOOLBOX_WIZARD_GETID			0x00000013
#define WPARAM_TOOLBOX_SAVE_SETUP_DEFAULT	0x00000014
#define WPARAM_TOOLBOX_DEVICE_GETID			0x00000015
#define WPARAM_TOOLBOX_DEVICE_ADDRESS		0x00000016
#define WPARAM_TOOLBOX_DEVICE_CONNECTION	0x00000017
#define WPARAM_TOOLBOX_DEVICE_ISCONNECTED	0x00000018
#define WPARAM_TOOLBOX_DISPLAY				0x00000019
#define WPARAM_TOOLBOX_GETROOTID			0x0000001A
#define WPARAM_TOOLBOX_VISIBLE				0x0000001B
#define WPARAM_TOOLBOX_SAVE_SETUP			0x0000001C
#define WPARAM_TOOLBOX_DEVICE_TYPE			0x0000001D
#define WPARAM_TOOLBOX_ECHO					0x0000001E
#define WPARAM_ENABLE_EVENT_UPDATE			0x0000001F
