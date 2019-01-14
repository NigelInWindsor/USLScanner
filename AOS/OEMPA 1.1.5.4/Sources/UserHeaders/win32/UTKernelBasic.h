
#pragma once
#include <stdio.h>
#include <stdlib.h>


#define VIEW_TITLE_MAX_SIZE		128


#define CONSTRUCTOR_NO_PARAMETER

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __WARN__ __FILE__ "("__STR1__(__LINE__)") : Warning user: "


#include "UTKernelExport.h"
#include "UTBasicEnum.h"
#include "UTBasicType.h"
#include "UTKernelType.h"
#include "UTLimitBasic.h"
#include "UTBaseClass.h"

#include "UTLimitWizard.h"
#include "UTLimit.h"


///////////////////////////////////////////////////////////////////////////////////////
// HEADER KERNEL
///////////////////////////////////////////////////////////////////////////////////////

UTKERNEL_API void UTKernel_Close();
UTKERNEL_API bool UTKernel_IsClosed();
UTKERNEL_API void UTKernel_Open(wchar_t *pApplicationName);
UTKERNEL_API bool UTKernel_IsOpened();
UTKERNEL_API bool UTKernel_IsUserInterfaceThread();
UTKERNEL_API int UTKernel_GetDigitalInput(int iSize,int *aiOwnerRootId,int *aiOwnerIndex);
UTKERNEL_API int UTKernel_GetCurrentApplicationId();
UTKERNEL_API bool UTKernel_SetCurrentApplicationName(wchar_t *pName);//you canno't set the name of another application (only the name of the current application).
UTKERNEL_API bool UTKernel_GetApplicationName(int iNameSize,wchar_t *pName,int iApplicationID=-1);//-1 is the current application.
UTKERNEL_API bool UTKernel_CallbackThread(pCallbackFunction pFunc,const structCallbackParameter &param,LRESULT &lResult);
UTKERNEL_API bool UTKernel_CallbackPostMessage(pCallbackFunction pFunc,const structCallbackParameter &param,structCallbackDispatch &callDispatch,HWND hWnd,UINT msg,WPARAM wParam,LPARAM &lParam);//(a view don't have to modify kernel, so PostMessage is enough).
UTKERNEL_API bool UTKernel_CallbackMessage(pCallbackFunction pFunc,const structCallbackParameter &param,structCallbackDispatch &callDispatch,HWND hWnd,UINT msg,WPARAM wParam,LPARAM &lParam);
UTKERNEL_API bool UTKernel_CallbackReplay(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam,LRESULT &lResult);//call only from inside the window callback.
//UTKERNEL_API bool UTKernel_CallbackUserLayer(const structCallbackParameter &param,int iLayerSize,structLayer *pLayerIndex);
UTKERNEL_API int UTKernel_GetVersionRootID();
UTKERNEL_API int UTKernel_GetMinimumListId();
UTKERNEL_API bool UTKernel_SetMinimumListId(int MinimumListId);

UTKERNEL_API UINT UTKernel_GetMessageBoxMsg();//unique window message for the message box processing.
UTKERNEL_API bool UTKernel_RegisterMessageBoxHwnd(HWND hWnd);
UTKERNEL_API bool UTKernel_UnregisterMessageBoxHwnd();
UTKERNEL_API HWND UTKernel_GetMessageBoxHwnd();
UTKERNEL_API bool UTKernel_MessageBox(const wchar_t *pText,WPARAM &wParam,LPARAM &lParam,int iApplicationID=-1);
UTKERNEL_API bool UTKernel_GetMessageBoxText(LPARAM lParam,int iTextSize,wchar_t *pText);
UTKERNEL_API bool UTKernel_SignalMessageBoxEnd(LRESULT lResult);
UTKERNEL_API LRESULT UTKernel_MessageBox(WPARAM wParam,LPARAM lParam,int iApplicationID=-1);
UTKERNEL_API bool UTKernel_PostMessageBox(WPARAM wParam,LPARAM lParam,int iApplicationID=-1);
UTKERNEL_API bool UTKernel_ToolboxRun(bool bWaitClose,HWND hPostFeedbackWnd,UINT uiPostFeedbackMsg,WPARAM wPostFeedbackParam,LPARAM lPostFeedbackParam);
UTKERNEL_API bool UTKernel_IsToolboxRunning();
UTKERNEL_API bool UTKernel_ToolboxDisplay(int iRootId,HWND hPostFeedbackWnd,UINT uiPostFeedbackMsg,WPARAM wPostFeedbackParam,LPARAM lPostFeedbackParam);
UTKERNEL_API bool UTKernel_ToolboxDisplayCloseAll(HWND hPostFeedbackWnd,UINT uiPostFeedbackMsg,WPARAM wPostFeedbackParam,LPARAM lPostFeedbackParam);
UTKERNEL_API bool UTKernel_FilterCustomRun(bool bWaitClose,HWND hPostFeedbackWnd,UINT uiPostFeedbackMsg,WPARAM wPostFeedbackParam,LPARAM lPostFeedbackParam);
UTKERNEL_API bool UTKernel_IsFilterCustomRunning();



UTKERNEL_API void UTKernel_SetTraceLocalFileName(wchar_t *pFilePath);//local file trace is not shared with others.
UTKERNEL_API bool UTKernel_GetTraceLocalFileName(int iSize,wchar_t *pFilePath);

UTKERNEL_API bool UTKernel_EnableTraceLocal(bool bEnable);//first you need to set the file name "UTKernel_SetLocalTraceFileName".
UTKERNEL_API bool UTKernel_IsTraceLocalEnabled();
UTKERNEL_API bool UTKernel_EnableTraceCallbackCall(bool bEnable);
UTKERNEL_API bool UTKernel_IsTraceCallbackCall();
UTKERNEL_API bool UTKernel_EnableTraceCallbackDone(bool bEnable);
UTKERNEL_API bool UTKernel_IsTraceCallbackDone();
UTKERNEL_API bool UTKernel_EnableTraceCallbackWndMessage(bool bEnable);
UTKERNEL_API bool UTKernel_IsTraceCallbackWndMessage();
UTKERNEL_API bool UTKernel_EnableTraceCallbackViaThread(bool bEnable);
UTKERNEL_API bool UTKernel_IsTraceCallbackViaThread();
UTKERNEL_API bool UTKernel_EnableTraceCallbackReplay(bool bEnable);
UTKERNEL_API bool UTKernel_IsTraceCallbackReplay();
UTKERNEL_API bool UTKernel_EnableTraceCallbackRegister(bool bEnable);
UTKERNEL_API bool UTKernel_IsTraceCallbackRegister();
UTKERNEL_API bool UTKernel_EnableTraceCallbackUnregister(bool bEnable);
UTKERNEL_API bool UTKernel_IsTraceCallbackUnregister();
UTKERNEL_API bool UTKernel_EnableTraceCallbackEvent(enumEvent eEvent,BOOL bTrace);
UTKERNEL_API bool UTKernel_IsTraceCallbackEvent(enumEvent eEvent);
UTKERNEL_API bool UTKernel_EnableTraceCallbackRootType(enumRootType eRootType,BOOL bTrace);
UTKERNEL_API bool UTKernel_IsTraceCallbackRootType(enumRootType eRootType);
UTKERNEL_API void UTKernel_ResetCallbackCallId();

UTKERNEL_API void UTKernel_SetTraceLocalMaxSize(DWORD dwMaxSize);
UTKERNEL_API void UTKernel_SetTraceGlobalMaxSize(DWORD dwMaxSize);
UTKERNEL_API DWORD UTKernel_GetTraceLocalMaxSize();
UTKERNEL_API DWORD UTKernel_GetTraceGlobalMaxSize();
UTKERNEL_API bool UTKernel_EnableTextFileSaveStatus(bool bEnable);
UTKERNEL_API bool UTKernel_IsTextFileSaveStatusEnabled();
UTKERNEL_API bool UTKernel_EnableSimulatorRandomFunction(bool bEnable);
UTKERNEL_API bool UTKernel_IsSimulatorRandomFunctionEnabled();
UTKERNEL_API bool UTKernel_CallbackTimerUpdate(bool bEnable);
UTKERNEL_API bool UTKernel_IsUpdateTimerEnabled();
UTKERNEL_API bool UTKernel_SetCallbackTimerUpdate(DWORD &dwUpdateTimer);//millisecond
UTKERNEL_API DWORD UTKernel_GetCallbackTimerUpdate();//millisecond

UTKERNEL_API void UTKernel_TraceEdit(wchar_t *pFileName,bool bWaitClose);
UTKERNEL_API void UTKernel_TraceError(const char *pString,int iLine);
UTKERNEL_API void UTKernel_TraceError(const wchar_t *pString,int iLine);
UTKERNEL_API void UTKernel_TraceError(const wchar_t *pString);
UTKERNEL_API void UTKernel_Trace(const wchar_t *pString,bool bCallback);
UTKERNEL_API void UTKernel_TraceDebugKernel(const wchar_t *pString);//special debug function please don't use it.
UTKERNEL_API bool UTKernel_EnableTraceMemory(bool bEnable);//special debug function in memory.
UTKERNEL_API bool UTKernel_IsTraceMemoryEnabled();//special debug function in memory.
UTKERNEL_API int UTKernel_GetTraceMemorySizeMax();//maximum size.
UTKERNEL_API int UTKernel_GetTraceMemorySize();//current size.
UTKERNEL_API bool UTKernel_TraceMemory(wchar_t *pString);//No time stamp.
UTKERNEL_API bool UTKernel_TraceMemory(bool bTimeStamp,wchar_t *pString);//special debug function in memory.
UTKERNEL_API bool UTKernel_DumpTraceMemory(bool bTimeStamp,wchar_t *pFile);//special debug function in memory.
UTKERNEL_API bool UTKernel_FlushTraceMemory();//special debug function in memory.

UTKERNEL_API bool UTKernel_DumpRootID(wchar_t *pFileName);
UTKERNEL_API bool UTKernel_DumpPathID(bool bCallback,wchar_t *pFileName);
UTKERNEL_API bool UTKernel_DumpLinkID(wchar_t *pFileName);
UTKERNEL_API bool UTKernel_DumpCallback(wchar_t *pFileName);
UTKERNEL_API bool UTKernel_GetResourcesRootID(int &iCount,int &iMemorySize,double &dCountPercent);
UTKERNEL_API bool UTKernel_GetResourcesPathID(bool bCallback,int &iCount,int &iMemorySize,double &dCountPercent,
											  int &iCallbackCount,int &iCallbackMemorySize,double &dCallbackCountPercent);
UTKERNEL_API bool UTKernel_GetResourcesLinkID(int &iCount,int &iMemorySize,double &dCountPercent);
UTKERNEL_API bool UTKernel_GetResourcesLanguage(int &iMemorySize);

UTKERNEL_API bool UTKernel_SaveRegistryString(HKEY hRootKey,wchar_t *wcSection,wchar_t *wcKey,wchar_t *pValue);
UTKERNEL_API bool UTKernel_LoadRegistryString(HKEY hRootKey,wchar_t *wcSection,wchar_t *wcKey,wchar_t *wcDefault,int iSize,wchar_t *pValue);

UTKERNEL_API bool UTKernel_EqualAtom(ATOM hCallbackComment1,ATOM hCallbackComment2);


class UTKERNEL_API_ALIGN CUTEnumDataType

{

public:
	static bool IsSigned(enumDataType eDataType);
	static bool IsBitwise(enumDataType eDataType);
	static int Sizeof(enumDataType eDataType);
};
