
#pragma once
#include "UTKernelDriverExport.h"
#include "WTypes.h"
#include "limits.h"
#include "UTKernelMessageBox.h"

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __WARN__ __FILE__ "("__STR1__(__LINE__)") : Warning user: "

#define DllExport __declspec(dllexport)
#define DllImport __declspec(dllimport)

const DWORD g_dwSharedNull=0xffffffff;
const LONGLONG g_llDummyCallback=0xffffffffffffffff;

UTKERNEL_DRIVER_API UINT UTKernel_RegisterWindowMessage();
UTKERNEL_DRIVER_API bool ListProcessModules(const wchar_t *pModuleNameSearch,const wchar_t *pPathSearch,int iPathSize,wchar_t *pPathFound,int iModuleNameSize,wchar_t *pModuleName);
UTKERNEL_DRIVER_API bool IsValidThreadID(DWORD dwThreadID);
UTKERNEL_DRIVER_API bool CreateFolderDataCfg(DWORD dwProductVersionLS,DWORD dwProductVersionMS,int iSize,wchar_t *pFolderData,int iSizeCfg,wchar_t *pFolderDataCfg);
UTKERNEL_DRIVER_API bool CreateFolderDataFiles(DWORD dwProductVersionLS,DWORD dwProductVersionMS,int iSize,wchar_t *pFolderData);
UTKERNEL_DRIVER_API bool CreateFolderDataFilesSubFolder(const wchar_t *pSubFolder,int iSize,wchar_t *pFolderData);
UTKERNEL_DRIVER_API bool CreateFolderDataFilesOEMPA(int iSize,wchar_t *pFolderData);
UTKERNEL_DRIVER_API bool CreateFolderDataFilesToolbox(int iSize,wchar_t *pFolderData);
UTKERNEL_DRIVER_API bool UTKernel_GetFolderFromFileName(const wchar_t *wcUTKernelPathExe,int iSize,wchar_t *wcUTKernelPrivateProfile);
UTKERNEL_DRIVER_API bool UTKernel_GetFileVersionInfo(	wchar_t *wcUTKernelPathExe,VS_FIXEDFILEINFO &fixedFileInfo,
									wchar_t *wcProductName,int iSizeProduct,wchar_t *wcCompanyName,int iSizeCompany);

UTKERNEL_DRIVER_API LONG GetSharedLong(LONG volatile* pData);
UTKERNEL_DRIVER_API LONG SetSharedLong(LONG volatile* pData,LONG lValue);
UTKERNEL_DRIVER_API LONG IncrementSharedLong(LONG volatile* pData);
UTKERNEL_DRIVER_API LONG DecrementSharedLong(LONG volatile* pData);
UTKERNEL_DRIVER_API float GetSharedFloat(float volatile* pData);
UTKERNEL_DRIVER_API float SetSharedFloat(float volatile* pData,float fValue);

#ifdef _WIN64
UTKERNEL_DRIVER_API LONGLONG GetSharedLongLong(LONGLONG volatile* pData);
UTKERNEL_DRIVER_API LONGLONG SetSharedLongLong(LONGLONG volatile* pData,LONGLONG lValue);
UTKERNEL_DRIVER_API LONGLONG IncrementSharedLongLong(LONGLONG volatile* pData);
UTKERNEL_DRIVER_API LONGLONG DecrementSharedLongLong(LONGLONG volatile* pData);
#endif _WIN64

UTKERNEL_DRIVER_API bool EnterSharedMutex(LONG volatile* pMutex,LONG volatile* pMutexThreadId,DWORD dwSleepTime,DWORD dwSleepCount);
UTKERNEL_DRIVER_API bool TryEnterSharedMutex(LONG volatile* pMutex,LONG volatile* pMutexThreadId);
UTKERNEL_DRIVER_API bool LeaveSharedMutex(LONG volatile* pMutex,LONG volatile* pMutexThreadId);
UTKERNEL_DRIVER_API bool EnterSharedMutexWithCount(LONG volatile* pMutex,LONG volatile* pMutexThreadId,LONG volatile* pMutexCount,DWORD dwSleepTime,DWORD dwSleepCount);
UTKERNEL_DRIVER_API bool TryEnterSharedMutexWithCount(LONG volatile* pMutex,LONG volatile* pMutexThreadId,LONG volatile* pMutexCount);
UTKERNEL_DRIVER_API bool LeaveSharedMutexWithCount(LONG volatile* pMutex,LONG volatile* pMutexThreadId,LONG volatile* pMutexCount);

UTKERNEL_DRIVER_API bool EnterSharedMutexWithCount2(LONG volatile* pMutex,LONG volatile* pMutexThreadId,LONG volatile* pMutexCount,DWORD dwSleepTime,DWORD dwSleepCount,ULONGLONG &ullStartTime,ULONGLONG &ullMaxWaitingTime,ULONGLONG &ullTotalWaitingTime,bool bBreak);
UTKERNEL_DRIVER_API bool TryEnterSharedMutexWithCount2(LONG volatile* pMutex,LONG volatile* pMutexThreadId,LONG volatile* pMutexCount,ULONGLONG &ullStartTime,ULONGLONG &ullMaxWaitingTime,ULONGLONG &ullTotalWaitingTime,bool bBreak);
UTKERNEL_DRIVER_API bool LeaveSharedMutexWithCount2(LONG volatile* pMutex,LONG volatile* pMutexThreadId,LONG volatile* pMutexCount,ULONGLONG ullStartTime,ULONGLONG &ullMaxProcessingTime,bool bBreak);
//UTKERNEL_DRIVER_API void SetDebugMainThreadId(DWORD dwMainThreadId);

UTKERNEL_DRIVER_API bool AtomToString(bool bDelete,ATOM hAtom,int iSize,wchar_t *string);
UTKERNEL_DRIVER_API bool StringToAtom(const wchar_t *string,ATOM &hAtom);

UTKERNEL_DRIVER_API void UTKernel_Trace(const wchar_t *pString);
UTKERNEL_DRIVER_API void UTKernel_SystemTrace(const wchar_t *pString);

UTKERNEL_DRIVER_API bool UTKernel_FileSave(const wchar_t *pFileName,const wchar_t *pSection,const wchar_t *pKey,const wchar_t *pValue);
UTKERNEL_DRIVER_API bool UTKernel_FileLoad(const wchar_t *pFileName,const wchar_t *pSection,const wchar_t *pKey,int iSize,wchar_t *pValue);

UTKERNEL_DRIVER_API int UTKernel_GetHexaSize(wchar_t *pValue);
UTKERNEL_DRIVER_API bool UTKernel_HexaToLongLong(wchar_t separator,wchar_t *pValue,__int64 &llValue);
UTKERNEL_DRIVER_API wchar_t* UTKernel_HexaToLongLong(wchar_t separator,wchar_t *pValue,__int64 &llValue,bool &bResult);
UTKERNEL_DRIVER_API wchar_t* UTKernel_HexaToLongLong(bool bDecimal,wchar_t separator,wchar_t *pValue,__int64 &llValue,bool &bResult);

UTKERNEL_DRIVER_API void UTDebug_New(void *pointer,int iBufferSize,int iSizeof,char *pFile,int iLine);
UTKERNEL_DRIVER_API void UTDebug_Free(void *pointer,int iBufferSize,int iSizeof,char *pFile,int iLine);

UTKERNEL_DRIVER_API bool TraceFileOpen(wchar_t *pFile,HANDLE &hFile,bool *pbUnicode=NULL);
UTKERNEL_DRIVER_API void TraceFilePrintTimeStamp(int iSize,wchar_t *pValue);
UTKERNEL_DRIVER_API bool TraceFilePrintTimeStamp(HANDLE hFile);
UTKERNEL_DRIVER_API bool TraceFilePrintString(HANDLE hFile,wchar_t *pString);
UTKERNEL_DRIVER_API void TraceFileClose(HANDLE hFile);

UTKERNEL_DRIVER_API const wchar_t* UTKernel_GetFolderDataCfg();
UTKERNEL_DRIVER_API const wchar_t* UTKernel_GetFolderData();
UTKERNEL_DRIVER_API const wchar_t* UTKernel_GetFolderDataFilesOEMPA();
UTKERNEL_DRIVER_API const wchar_t* UTKernel_GetFolderDataFiles(const wchar_t *pSubFolder);
UTKERNEL_DRIVER_API void UTKernel_SetFolderDataFiles(const wchar_t *pSubFolder,const wchar_t *pFolder);
UTKERNEL_DRIVER_API const wchar_t* UTKernel_GetFolderExe();
UTKERNEL_DRIVER_API const wchar_t* UTKernel_GetNameExe();
UTKERNEL_DRIVER_API const wchar_t* UTKernel_GetFolderDll();
UTKERNEL_DRIVER_API const wchar_t* UTKernel_GetConfigurationFile();
UTKERNEL_DRIVER_API void UTKernel_SetTraceGlobalFileName(wchar_t *pFilePath);//global file trace is shared with others.
UTKERNEL_DRIVER_API bool UTKernel_GetTraceGlobalFileName(int iSize,wchar_t *pFilePath);


UTKERNEL_DRIVER_API bool UTKernel_EnableTraceGlobal(bool bEnable);//first you need to set the file name "UTKernel_SetGlobalTraceFileName".
UTKERNEL_DRIVER_API bool UTKernel_IsTraceGlobalEnabled();

UTKERNEL_DRIVER_API bool UTKernel_EnableInspectionCounters(bool bEnable);
UTKERNEL_DRIVER_API bool UTKernel_IsInspectionCountersEnabled();

UTKERNEL_DRIVER_API long lround2(double x);
UTKERNEL_DRIVER_API __int64 llRound(double x);
UTKERNEL_DRIVER_API void UTKernel_CheckMemory();

