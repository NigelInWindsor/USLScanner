// UTCalculator.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _DEBUG
#include <crtdbg.h>
#endif _DEBUG
#include "..\UTKernelDriver\UTBasicEnum.h"
#include "UTStubMatlab.h"
//stack size for mex files: see http://fr.mathworks.com/matlabcentral/newsreader/view_thread/323658
//and see also http://fr.mathworks.com/matlabcentral/newsreader/view_thread/29069
//https://msdn.microsoft.com/fr-fr/library/8cxs58a6.aspx
//http://www.databasesql.info/article/4010169851/
//http://fr.mathworks.com/matlabcentral/newsreader/view_thread/162021
//http://fr.mathworks.com/help/matlab/apiref/mexatexit.html
//http://stackoverflow.com/questions/4764718/giving-more-memory-to-mex-programs-in-matlab
//http://stackoverflow.com/questions/16716821/how-to-build-mex-file-directly-in-visual-studio

//http://fr.mathworks.com/help/matlab/matlab_prog/strategies-for-efficient-use-of-memory.html?requestedDomain=www.mathworks.com
//http://fr.mathworks.com/matlabcentral/newsreader/view_thread/111952
//http://fr.mathworks.com/help/matlab/apiref/mexmakearraypersistent.html
//http://fr.mathworks.com/help/matlab/matlab_external/memory-management.html
//http://fr.mathworks.com/help/matlab/matlab_external/memory-management-issues.html
//http://fr.mathworks.com/help/matlab/apiref/mexputvariable.html

//http://fr.mathworks.com/matlabcentral/answers/167236-how-do-i-avoid-compiler-limit-compiler-stack-overflow-while-building-a-model-for-rapid-accelerato
//editbin.exe /STACK:1000000000 /HEAP:1000000 notepad.exe (be careful with the rights).
//dumpbin.exe /HEADERS notepad.exe
//        3B9ACA00 size of stack reserve
//           11000 size of stack commit
//           F4240 size of heap reserve
//            1000 size of heap commit
//c:\Users\Public>editbin /STACK:100000000,100000000 notepad.exe
//c:\Users\Public>dumpbin /headers notepad.exe
//				...
//         5F5E100 size of stack reserve
//         5F5E100 size of stack commit
//           F4240 size of heap reserve
//            1000 size of heap commit
//               0 loader flags
//				...
//The taskmgr can be used to display the stack commit see the column "Commit Size".
//see also following compiler options: "/GH" "/Gh" "/F" and also "/STACK" ("Properties - Configuration Properties - Linker - System - Stack Reserve Size"/"Stack Commit Size").

void TraceError(const char *pString,int iLine);
void TraceError(const wchar_t *pString,int iLine);
void TraceError(const wchar_t *pString);
void Trace(const wchar_t *pString);


extern "C"{

	__declspec(dllexport) CStubMatlab mxMain;

}

#ifdef _DEBUG
void *CStubMatlab::mxNew(int iSize, int iLine, char *file)
{
	void *ptr;

#ifdef mex_MALLOC
	ptr = mxMalloc(iSize);
#ifdef mex_MALLOC_PERSISTENT
	mexMakeMemoryPersistent(ptr);
#endif mex_MALLOC_PERSISTENT
#else mex_MALLOC
	ptr = (*g_pmxMain->g_pmxNew)(iSize,iLine,file);
#endif mex_MALLOC
	return ptr;
}
void CStubMatlab::mxDelete(void *pBuffer, int iLine, char *file)
{
#ifdef mex_MALLOC
#ifdef mex_MALLOC_PERSISTENT
	mxFree(pBuffer);
#endif mex_MALLOC_PERSISTENT
#else mex_MALLOC
	(*g_pmxMain->g_pmxDelete)(pBuffer,iLine,file);
#endif mex_MALLOC
}
#else _DEBUG
void *CStubMatlab::mxNew(int iSize)
{
	void *ptr;

#ifdef mex_MALLOC
	ptr = mxMalloc(iSize);
#ifdef mex_MALLOC_PERSISTENT
	mexMakeMemoryPersistent(ptr);
#endif mex_MALLOC_PERSISTENT
#else mex_MALLOC
	ptr = (*g_pmxNew)(iSize);
#endif mex_MALLOC
	return  ptr;
}
void CStubMatlab::mxDelete(void *pBuffer)
{
#ifdef mex_MALLOC
#ifdef mex_MALLOC_PERSISTENT
	mxFree(pBuffer);
#endif mex_MALLOC_PERSISTENT
#else mex_MALLOC
	(*g_pmxDelete)((BYTE*)pBuffer);
#endif mex_MALLOC
}
#endif _DEBUG

BYTE g_pBuffer[g_iBufferSize];
BYTE *g_pBuffer2=NULL;
int g_iBufferSize2=0;
BYTE *g_pBuffer3=NULL;
int g_iBufferSize3=0;

int iround(double x)
{
	int nx = (int)x;
	if(x>0)
	{
		if((x-nx)>0.5)
			return nx+1;
		else
			return nx;
	}else{
		if((x-nx)<-0.5)
			return nx-1;
		else
			return nx;
	}
}

bool mxGeneralProcess(void *data)
{
	CStubMatlab *pThis=(CStubMatlab*)data;
	if(!data)
		return false;
	return pThis->mxGeneralProcess();
}
bool mxDeviceProcess(void *data)
{
	CStubMatlab *pThis=(CStubMatlab*)data;
	if(!data)
		return false;
	return pThis->mxDeviceProcess();
}
bool mxDevice256Process(void *data)
{
	CStubMatlab *pThis=(CStubMatlab*)data;
	if(!data)
		return false;
	return pThis->mxDevice256Process();
}

mxClassID CStubMatlab::mxGetClassID(const mxArray *pm)
{
	if(!g_pmxGetClassID)
		return mxUNKNOWN_CLASS;
	return (*g_pmxGetClassID)(pm);
}
bool CStubMatlab::mxIsSparse(const mxArray *pm)
{
	if(!g_pmxIsSparse)
		return false;
	return (*g_pmxIsSparse)(pm);
}
double *CStubMatlab::mxGetPr(const mxArray *pm)
{
	if(!g_pmxGetPr)
		return NULL;
	return (*g_pmxGetPr)(pm);
}
void CStubMatlab::mxSetPr(mxArray *pm, double *pr)
{
	if(!g_pmxSetPr)
		return;
	return (*g_pmxSetPr)(pm, pr);
}
void *CStubMatlab::mxGetData(const mxArray *pm)
{
	if(!g_pmxGetData)
		return NULL;
	return (*g_pmxGetData)(pm);
}
void CStubMatlab::mxSetData(mxArray *pm, void *pr)
{
	if(!g_pmxSetData)
		return;
	return (*g_pmxSetData)(pm, pr);
}
void CStubMatlab::mxGetDimension(const mxArray *pm, structDimension *dim)
{
	if(!g_pmxGetDimension)
		return;
	return (*g_pmxGetDimension)(pm, dim);
}
void *CStubMatlab::mxMalloc2(size_t size)
{
	if(!g_pmxMalloc2)
		return NULL;
	return (*g_pmxMalloc2)(size);
}
void *CStubMatlab::mxCalloc2(size_t n, size_t size)
{
	if(!g_pmxCalloc2)
		return NULL;
	return (*g_pmxCalloc2)(n, size);
}
void CStubMatlab::mxFree(void *ptr)
{
	if(!g_pmxFree)
		return;
	(*g_pmxFree)(ptr);
}
void CStubMatlab::mexMakeMemoryPersistent(void *ptr)
{
	if(!g_pmexMakeMemoryPersistent)
		return;
	(*g_pmexMakeMemoryPersistent)(ptr);
}
void CStubMatlab::mexMakeArrayPersistent(mxArray *pm)
{
	if(!g_pmexMakeArrayPersistent)
		return;
	(*g_pmexMakeArrayPersistent)(pm);
}
const char *CStubMatlab::mexFunctionName(void)
{
	if(!g_pmexFunctionName)
		return NULL;
	return (*g_pmexFunctionName)();
}
mxArray *CStubMatlab::mexGetVariable(const char *workspace, const char *varname)
{
	//Values for "workspace"
	//"base"	Search for the variable in the base workspace.
	//"caller"	Search for the variable in the caller workspace.
	//"global"	Search for the variable in the global workspace.
	if(!g_pmexGetVariable)
		return NULL;
	return (*g_pmexGetVariable)(workspace, varname);
}
int CStubMatlab::mexPutVariable(const char *workspace, const char *varname, const mxArray *pm)
{
	if(!g_pmexPutVariable)
		return 1;
	return (*g_pmexPutVariable)(workspace, varname, pm);
}
void CStubMatlab::mxDestroyArray(mxArray *pm)
{
	if(!g_pmxDestroyArray)
		return;
	(*g_pmxDestroyArray)(pm);
}
mxArray* CStubMatlab::mxCreateDummy()
{
	if(!g_pmxCreateDummy)
		return NULL;
	return (*g_pmxCreateDummy)();
}
mxArray* CStubMatlab::mxCreateString2(const wchar_t *pIn)
{
	if(!g_pmxCreateString2)
		return NULL;
	return (*g_pmxCreateString2)(pIn);
}
mxArray* CStubMatlab::mxCreateByteResult0(BYTE byData2)
{
	if(!g_pmxCreateByteResult0)
		return NULL;
	return (*g_pmxCreateByteResult0)(byData2);
}
mxArray* CStubMatlab::mxCreateByteResult1(int iSize,BYTE *pbyData2)
{
	if(!g_pmxCreateByteResult1)
		return NULL;
	return (*g_pmxCreateByteResult1)(iSize,pbyData2);
}
mxArray* CStubMatlab::mxCreateAscanResult2(int iRow, int iCol, int iByteSize, bool bSigned, BYTE** ppbyData2)
{
	if(!g_pmxCreateAscanResult2)
		return NULL;
	return (*g_pmxCreateAscanResult2)(iRow,iCol,iByteSize,bSigned,ppbyData2);
}
mxArray* CStubMatlab::mxCreateCharResult1(int iSize,char *pbyData2)
{
	if(!g_pmxCreateCharResult1)
		return NULL;
	return (*g_pmxCreateCharResult1)(iSize,pbyData2);
}
mxArray* CStubMatlab::mxCreateCharResult2(int iRow,int iCol,char *pData2)
{
	if(!g_pmxCreateCharResult2)
		return NULL;
	return (*g_pmxCreateCharResult2)(iRow,iCol,pData2);
}
mxArray* CStubMatlab::mxCreateWordResult1(int iSize,WORD *pwData2)
{
	if(!g_pmxCreateWordResult1)
		return NULL;
	return (*g_pmxCreateWordResult1)(iSize,pwData2);
}
mxArray* CStubMatlab::mxCreateShortResult1(int iSize,short *psData2)
{
	if(!g_pmxCreateShortResult1)
		return NULL;
	return (*g_pmxCreateShortResult1)(iSize,psData2);
}
mxArray* CStubMatlab::mxCreateShortResult2(int iRow,int iCol,short *psData2)
{
	if(!g_pmxCreateShortResult2)
		return NULL;
	return (*g_pmxCreateShortResult2)(iRow,iCol,psData2);
}
mxArray* CStubMatlab::mxCreateLongResult1(int iSize,long *pdwData2)
{
	if(!g_pmxCreateLongResult1)
		return NULL;
	return (*g_pmxCreateLongResult1)(iSize,pdwData2);
}
mxArray* CStubMatlab::mxCreateLongResult2(int iRow,int iCol,long *pdwData2)
{
	if(!g_pmxCreateLongResult2)
		return NULL;
	return (*g_pmxCreateLongResult2)(iRow,iCol,pdwData2);
}
mxArray* CStubMatlab::mxCreateLongLongResult1(int iSize,long long *pdwData2)
{
	if(!g_pmxCreateLongLongResult1)
		return NULL;
	return (*g_pmxCreateLongLongResult1)(iSize,pdwData2);
}
mxArray* CStubMatlab::mxCreateLongLongResult2(int iRow,int iCol,long long *pdwData2)
{
	if(!g_pmxCreateLongLongResult2)
		return NULL;
	return (*g_pmxCreateLongLongResult2)(iRow,iCol,pdwData2);
}
mxArray* CStubMatlab::mxCreateDWORDResult1(int iSize,unsigned long *pdwData2)
{
	if(!g_pmxCreateDWORDResult1)
		return NULL;
	return (*g_pmxCreateDWORDResult1)(iSize,pdwData2);
}
mxArray* CStubMatlab::mxCreateIntegerResult2d(int iRow,int iCol,double *pdData2)
{
	if(!g_pmxCreateIntegerResult2d)
		return NULL;
	return (*g_pmxCreateIntegerResult2d)(iRow,iCol,pdData2);
}
mxArray* CStubMatlab::mxCreateIntegerResult2(int iRow,int iCol,int *piData2)
{
	if(!g_pmxCreateIntegerResult2)
		return NULL;
	return (*g_pmxCreateIntegerResult2)(iRow,iCol,piData2);
}
mxArray* CStubMatlab::mxCreateDoubleResult0(double dData)
{
	if(!g_pmxCreateDoubleResult0)
		return NULL;
	return (*g_pmxCreateDoubleResult0)(dData);
}
mxArray* CStubMatlab::mxCreateDoubleResult1(int iSize,double *pData2)
{
	if(!g_pmxCreateDoubleResult1)
		return NULL;
	return (*g_pmxCreateDoubleResult1)(iSize,pData2);
}
mxArray* CStubMatlab::mxCreateDoubleResult2(int iRow,int iCol,double *pdData2)
{
	if(!g_pmxCreateDoubleResult2)
		return NULL;
	return (*g_pmxCreateDoubleResult2)(iRow,iCol,pdData2);
}
void CStubMatlab::mexErrMsgTxt(const char *errormsg)
{
	if(!g_pmexErrMsgTxt)
		return;
	return (*g_pmexErrMsgTxt)(errormsg);
}
bool CStubMatlab::mxGeneralProcess()
{
	DWORD dwAux,dwData;
	int iRet;
	double adData[9];

	if((nrhs==3) && !wcscmp(wcFunc,L"mxAutoDisconnect") && pdMatrix)
	{
		if(nlhs==1)
		{
			if((*g_pmxAutoDisconnect)((pdMatrix[0]?true:false)))
				VALUE_OUT = mxCreateByteResult0(1);
			else
				VALUE_OUT = mxCreateByteResult0(0);
		}else{
			g_pmexPrintf("error : mxAutoDisconnect!\n");
		}
		return true;
	}
	if((nrhs==3) && !wcscmp(wcFunc,L"mxNewDevice") && wcString[0])
	{
		if(nlhs==1)
		{
#ifndef _WIN64
			VALUE_OUT = g_pmxCreateDoubleResult0((double)(*g_pmxNewDevice)(wcString, 4096, 16384, 0));
#else _WIN64
			VALUE_OUT = g_pmxCreateDoubleResult0((double)(*g_pmxNewDevice)(wcString, 4096, 16384, 1));
#endif _WIN64
		}else{
			g_pmexPrintf("error : to create a new device you need to use the returned device identifier!\n",iId);
		}
		return true;
	}
	if((nrhs==4) && !wcscmp(wcFunc,L"mxNewDevice") && pdMatrix && wcName[0])
	{
		if(nlhs==1)
		{
			if (((number_of_dimensions==1) && (dims[0]==1)) || ((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==1)))
				VALUE_OUT = g_pmxCreateDoubleResult0((double)(*g_pmxNewDevice)(wcName, iround(pdMatrix[0]), 16384, 0));
			if (((number_of_dimensions==2) && (dims[0]==2) && (dims[1]==1)) || ((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==2)))
				VALUE_OUT = g_pmxCreateDoubleResult0((double)(*g_pmxNewDevice)(wcName, iround(pdMatrix[0]), iround(pdMatrix[1]), 0));
			if (((number_of_dimensions==2) && (dims[0]==3) && (dims[1]==1)) || ((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==3)))
			{
#ifndef _WIN64
				if(iround(pdMatrix[2]))
					mexErrMsgTxt("For the 32 bits version, the ascan Fifo should be disabled!");
#endif _WIN64
				VALUE_OUT = g_pmxCreateDoubleResult0((double)(*g_pmxNewDevice)(wcName, iround(pdMatrix[0]), iround(pdMatrix[1]), iround(pdMatrix[2])));
			}
		}else{
			g_pmexPrintf("error : to create a new device you need to use the returned device identifier!\n",iId);
		}
		return true;
	}
	if(g_pmxGetCfgKernelFile && (nrhs==3) && !wcscmp(wcFunc,L"mxGetCfgKernelFile") && wcString[0])
	{
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0(g_pmxGetCfgKernelFile(L"Matlab",wcString,0));
		return true;
	}
	if(g_pmxSetCfgKernelFile && (nrhs==4) && !wcscmp(wcFunc,L"mxSetCfgKernelFile") && pdMatrix && wcName[0])
	{
		if(mxCreateDoubleResult0(g_pmxSetCfgKernelFile(L"Matlab",wcName,(DWORD)iround(*pdMatrix))))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult0(1.0);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult0(0.0);
		}
		return true;
	}
	if(g_pmxSaveRegistryString && (nrhs==5) && !wcscmp(wcFunc,L"mxSaveRegistryString"))
	{
		if(mxCreateDoubleResult0(g_pmxSaveRegistryString(wcString,wcName,wcName5)))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult0(1.0);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult0(0.0);
		}
		return true;
	}
	//if(g_pmxGetTraceGlobalFileName && (nrhs==2) && !wcscmp(wcFunc,L"mxGetTraceGlobalFileName"))
	//{
	//	if((*g_pmxGetTraceGlobalFileName)(MAX_PATH,wcName))
	//	{
	//		dwConvert=(DWORD)WideCharToMultiByte(CP_ACP, 0, wcName, -1, pAux, MAX_PATH, NULL, NULL);
	//		mexPrintf("mxGetTraceGlobalFileName => '%s'\n",pAux);
	//	}
	//}
	//EnableTrace( nlhs, plhs, nrhs, prhs, wcFunc, wcString, L"mxEnableTraceGlobal", g_pmxEnableTraceGlobal );
	//IsTrace( nlhs, plhs, nrhs, prhs, wcFunc, wcString, L"mxIsTraceGlobalEnabled",g_pmxIsTraceGlobalEnabled );
	//if(g_pmxTrace && (nrhs==3) && !wcscmp(wcFunc,L"mxTrace") && wcString[0])
	//{
	//	(*g_pmxTrace)(wcString);
	//	//mexPrintf("mxTrace => see file\n");
	//	return;
	//}
	if((nrhs==2) && !wcscmp(wcFunc,L"mxGetCurrentProcessId"))
	{
		dwAux = GetCurrentProcessId();
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0((double)dwAux);
		return true;
	}
//DRIVER MANAGEMENT STOP
//TOOLBOX MANAGEMENT START
	if(g_pmxToolboxRun && (nrhs==3) && !wcscmp(wcFunc,L"mxToolboxRun") && pdMatrix)
	{
		if((*g_pmxToolboxRun)((pdMatrix[0]?true:false)))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxIsToolboxRunning && (nrhs==2) && !wcscmp(wcFunc,L"mxIsToolboxRunning"))
	{
		if((*g_pmxIsToolboxRunning)())
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxToolboxClose && (nrhs==2) && !wcscmp(wcFunc,L"mxToolboxClose"))
	{
		if((*g_pmxToolboxClose)())
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxToolboxEcho && (nrhs==3) && !wcscmp(wcFunc,L"mxToolboxEcho") && pdMatrix)
	{
		dwData = (*g_pmxToolboxEcho)((DWORD)iround(*pdMatrix));
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0((double)dwData);
		return true;
	}
	if(g_pmxToolboxVisible && (nrhs==3) && !wcscmp(wcFunc,L"mxToolboxVisible") && pdMatrix)
	{
		if((*g_pmxToolboxVisible)(iround(*pdMatrix)?true:false))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult0(1.0);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult0(0.0);
		}
		return true;
	}
	if(g_pmxEnableEventUpdate && (nrhs==3) && !wcscmp(wcFunc,L"EnableEventUpdate") && pdMatrix)
	{
		if((*g_pmxEnableEventUpdate)(iround(*pdMatrix)?true:false))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult0(1.0);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult0(0.0);
		}
		return true;
	}
	if(g_pmxToolboxDisplay && (nrhs==3) && !wcscmp(wcFunc,L"mxToolboxDisplay") && pdMatrix)
	{
		if((*g_pmxToolboxDisplay)(iround(*pdMatrix)))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult0(1.0);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult0(0.0);
		}
		return true;
	}
	if(g_pmxToolboxWizardNew && (nrhs==2) && !wcscmp(wcFunc,L"mxToolboxWizardNew"))
	{
		if((*g_pmxToolboxWizardNew)())
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxToolboxWizardDelete && (nrhs==2) && !wcscmp(wcFunc,L"mxToolboxWizardDelete"))
	{
		if((*g_pmxToolboxWizardDelete)())
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxToolboxWizardUpdateScan && (nrhs==2) && !wcscmp(wcFunc,L"mxToolboxWizardUpdateScan"))
	{
		if((*g_pmxToolboxWizardUpdateScan)())
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxToolboxIsWizardEnabled && (nrhs==2) && !wcscmp(wcFunc,L"mxToolboxIsWizardEnabled"))
	{
		if((*g_pmxToolboxIsWizardEnabled)())
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxGetWizardBeamCount && (nrhs==2) && !wcscmp(wcFunc,L"mxGetWizardBeamCount"))
	{
		iRet = (*g_pmxGetWizardBeamCount)();
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0((double)iRet);
		return true;
	}
	if(g_pmxGetWizardBeam && (nrhs==3) && !wcscmp(wcFunc,L"mxGetWizardBeam") && pdMatrix)
	{
		if((*g_pmxGetWizardBeam)(iround(pdMatrix[0]),&adData[0],&adData[1],&adData[2],&adData[3],&adData[4],&adData[5],&adData[6],&adData[7],&adData[8]) && (nlhs==1))
		{
			VALUE_OUT = mxCreateDoubleResult2(3,3,adData);
		}
		return true;
	}
	if(g_pmxToolboxWizardReset && (nrhs==2) && !wcscmp(wcFunc,L"mxToolboxWizardReset"))
	{
		if((*g_pmxToolboxWizardReset)())
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxGetWizardElementCount && (nrhs==2) && !wcscmp(wcFunc,L"mxGetWizardElementCount"))
	{
		iRet = (*g_pmxGetWizardElementCount)();
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0((double)iRet);
		return true;
	}
	if(g_pmxGetWizardElement && (nrhs==3) && !wcscmp(wcFunc,L"mxGetWizardElement") && pdMatrix)
	{
		if((*g_pmxGetWizardElement)(iround(pdMatrix[0]),&adData[0],&adData[1],&adData[2]) && (nlhs==1))
		{
			VALUE_OUT = mxCreateDoubleResult2(3,1,adData);
		}
		return true;
	}
	if(g_pmxGetToolboxParameter && (nrhs==3) && !wcscmp(wcFunc,L"mxGetToolboxParameter") && wcString[0])
	{
		wchar_t out[MAX_PATH];
		if((*g_pmxGetToolboxParameter)(wcString,MAX_PATH,out))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateString2(out);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateString2(L"error");
		}
		return true;
	}
	if(g_pmxSetToolboxParameter && (nrhs==3) && !wcscmp(wcFunc,L"mxSetToolboxParameter") && wcString[0])
	{
		if((*g_pmxSetToolboxParameter)(wcString))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxToolboxLoadWizard && (nrhs==3) && !wcscmp(wcFunc,L"mxToolboxLoadWizard") && wcString[0])
	{
		if((*g_pmxToolboxLoadWizard)(wcString))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxToolboxSaveWizard && (nrhs==4) && !wcscmp(wcFunc,L"mxToolboxSaveWizard") && pdMatrix && wcName[0])
	{
		if((*g_pmxToolboxSaveWizard)(wcName,(*pdMatrix?true:false)))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult0(1.0);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult0(0.0);
		}
		return true;
	}
	if(g_pmxToolboxLoadMatrix && (nrhs==3) && !wcscmp(wcFunc,L"mxToolboxLoadMatrix") && wcString[0])
	{
		if((*g_pmxToolboxLoadMatrix)(wcString))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxToolboxSaveMatrix && (nrhs==3) && !wcscmp(wcFunc,L"mxToolboxSaveMatrix") && wcString[0])
	{
		if((*g_pmxToolboxSaveMatrix)(wcString))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxGetToolboxDeviceId && (nrhs==2) && !wcscmp(wcFunc,L"mxGetToolboxDeviceId"))
	{
		iRet = (*g_pmxGetToolboxDeviceId)();
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0((double)iRet);
		return true;
	}
	if(g_pmxGetToolboxObjectId && (nrhs==3) && !wcscmp(wcFunc,L"mxGetToolboxObjectId") && wcString[0])
	{
		iRet = (*g_pmxGetToolboxObjectId)(wcString);
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0((double)iRet);
		return true;
	}
	if(g_pmxSetToolboxDeviceAddress && (nrhs==3) && !wcscmp(wcFunc,L"mxSetToolboxDeviceAddress") && wcString[0])
	{
		if((*g_pmxSetToolboxDeviceAddress)(wcString))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxToolboxDeviceConnection && (nrhs==3) && !wcscmp(wcFunc,L"mxToolboxDeviceConnection") && pdMatrix)
	{
		if((*g_pmxToolboxDeviceConnection)((*pdMatrix==0.0)?false:true))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxToolboxIsDeviceConnected && (nrhs==2) && !wcscmp(wcFunc,L"mxToolboxIsDeviceConnected"))
	{
		if((*g_pmxToolboxIsDeviceConnected)())
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxGetToolboxDeviceType && (nrhs==2) && !wcscmp(wcFunc,L"mxGetToolboxDeviceType"))
	{
		const wchar_t *pRet=(*g_pmxGetToolboxDeviceType)();
		if((nlhs==1) && pRet)
			VALUE_OUT = mxCreateString2(pRet);
		return true;
	}
	if((nrhs==2) && !wcscmp(wcFunc,L"mxGetTest"))
	{
		BYTE *pbyData;
		int i;
		if(nlhs==1)
		{
			VALUE_OUT = mxCreateAscanResult2(2/*iCycleCount*/, 4/*iPointCount0*/, 1/*iByteSize0*/, true/*bSigned0*/, &pbyData);
			for(i=0;i<8;i++)
				pbyData[i] = i;
			//>> a=utCmd(-1,'mxGetTest')
			//a =
			//	0    2    4    6
			//	1    3    5    7
		}
		return true;
	}
	return false;
}
bool CStubMatlab::mxAscanFifoIndex()
{
	bool bRet=true;
	int iCycleCount=0, iCount, iSize;
	long long *pSequence;
	int *pPointCount,*pIndex,iCycle,iIndex;
	BYTE *pByteSize;
	bool *pSigned;
	bool bFifo, bAscan;
	int *pCycleIn,*pCycle;
	char *pcMatrix=(char*)pMatrix;
	short *psMatrix=(short*)pMatrix;
	long *plMatrix=(long*)pMatrix;

	if(number_of_dimensions==1)
		iCycleCount = dims[0];
	else
	if((number_of_dimensions==2) && (dims[0]==1))
		iCycleCount = dims[1];
	else
		mexErrMsgTxt("Invalid CycleCount");
	if(!iCycleCount)
	{
		mexErrMsgTxt("Invalid CycleCount (too many)");
		return true;
	}
	iSize = iCycleCount*sizeof(int);
	if(g_iBufferSize3<iSize)
	{
#ifdef _DEBUG
		if(g_pBuffer3)
			mxDelete(g_pBuffer3,__LINE__,__FILE__);
		g_pBuffer3 = (BYTE*)mxNew(iSize,__LINE__,__FILE__);
#else _DEBUG
		if(g_pBuffer3)
			mxDelete(g_pBuffer3);
		g_pBuffer3 = (BYTE*)mxNew(iSize);
#endif _DEBUG
		if(g_pBuffer3)
			g_iBufferSize3 = iSize;
	}
//mexPrintf("1 iSize %d buffer 0x%.8lX\n",g_iBufferSize3,g_pBuffer3);
	if(!g_pBuffer3)
	{
		g_pmexPrintf("Allocation error\n");
		return true;
	}
	pCycleIn = (int*)g_pBuffer3;
	if(!wcscmp(wcFunc,L"mxGetAcquisitionAscanFifoIndex"))
	{
		bAscan = true;
		bFifo = true;
	}
	if(!wcscmp(wcFunc,L"mxGetAcquisitionAscanLifoIndex"))
	{
		bAscan = true;
		bFifo = false;
	}
	if(!wcscmp(wcFunc,L"mxGetAcquisitionCscanFifoIndex"))
	{
		bAscan = false;
		bFifo = true;
	}
	if(!wcscmp(wcFunc,L"mxGetAcquisitionCscanLifoIndex"))
	{
		bAscan = false;
		bFifo = false;
	}
	iSize = iCycleCount*(sizeof(int)+sizeof(long long)+3*sizeof(int)+sizeof(BYTE)+sizeof(bool));
	if(g_iBufferSize2<iSize)
	{
#ifdef _DEBUG
		if(g_pBuffer2)
			mxDelete(g_pBuffer2,__LINE__,__FILE__);
		g_pBuffer2 = (BYTE*)mxNew(iSize,__LINE__,__FILE__);
#else _DEBUG
		if(g_pBuffer2)
			mxDelete(g_pBuffer2);
		g_pBuffer2 = (BYTE*)mxNew(iSize);
#endif _DEBUG
		if(g_pBuffer2)
			g_iBufferSize2 = iSize;
	}
//mexPrintf("2 iSize %d buffer 0x%.8lX\n",g_iBufferSize2,g_pBuffer2);
	if(!g_pBuffer2)
	{
		g_pmexPrintf("Allocation error\n");
		return true;
	}
	for(iIndex=0;iIndex<iCycleCount;iIndex++)
	{
		switch(iMatrixClass)
		{
		case mxINT8_CLASS:
		case mxUINT8_CLASS:		iCycle = pcMatrix[iIndex];break;
		case mxINT16_CLASS:
		case mxUINT16_CLASS:	iCycle = psMatrix[iIndex];break;
		case mxINT32_CLASS:
		case mxUINT32_CLASS:	iCycle = plMatrix[iIndex];break;
		case mxDOUBLE_CLASS:	iCycle = iround(pdMatrix[iIndex]);break;
		default:
			g_pmexPrintf("Bad item array type\n");
		}
		pCycleIn[iIndex] = iCycle;
	}
	pCycle = (int*)&g_pBuffer2[iCycleCount*sizeof(long long)];
	pSequence = (long long*)&g_pBuffer2[0];
	pPointCount = (int*)&g_pBuffer2[iCycleCount*(sizeof(long long)+1*sizeof(int))];
	pIndex = (int*)&g_pBuffer2[iCycleCount*(sizeof(long long)+2*sizeof(int))];
	pByteSize = (BYTE*)&g_pBuffer2[iCycleCount*(sizeof(long long)+3*sizeof(int))];
	pSigned = (bool*)&g_pBuffer2[iCycleCount*(sizeof(long long)+3*sizeof(int)+sizeof(BYTE))];
	iCount = 0;
	if(bAscan)
		iCount = g_pmxGetAcquisitionAscanFifoIndex(iId, bFifo, iCycleCount, pCycleIn, pIndex, pCycle, pSequence, pPointCount, pByteSize, pSigned);
	else
		iCount = g_pmxGetAcquisitionCscanFifoIndex(iId, bFifo, iCycleCount, pCycleIn, pIndex, pCycle, pSequence);
	if(!iCount)
	{
		if(nlhs>=1)
			VALUE_OUT = mxCreateDummy();
		if(nlhs>=2)
			plhs[1] = mxCreateDummy();
		if(nlhs>=3)
			plhs[2] = mxCreateDummy();
		if(nlhs>=4)
			plhs[3] = mxCreateDummy();
		if(nlhs>=5)
			plhs[4] = mxCreateDummy();
		if(nlhs>=6)
			plhs[5] = mxCreateDummy();
		return true;
	}
	if(nlhs>=1)
		VALUE_OUT = mxCreateLongResult2(1,iCount,(long*)pIndex);
	if(nlhs>=2)
		plhs[1] = mxCreateLongResult2(1,iCount,(long*)pCycle);
	if(nlhs>=3)
		plhs[2] = mxCreateLongLongResult2(1,iCount,(long long*)pSequence);
	if(bAscan)
	{
		if(nlhs>=4)
			plhs[3] = mxCreateLongResult2(1,iCount,(long*)pPointCount);
		if(nlhs>=5)
			plhs[4] = mxCreateCharResult2(1,iCount,(char*)pByteSize);
		if(nlhs>=6)
			plhs[5] = mxCreateCharResult2(1,iCount,(char*)pSigned);
	}
	return true;
}
bool CStubMatlab::mxAscanFifoData()
{
	BYTE *pbyData;
	long long *pSequence,sequence;
	int *pCycle,*pItem,iItemCount,iPointCount,iByteSize,iItem,iCycle,iSize,iIndex,iCount;
	double *pdEncoder;
	long *plRawEncoder;
	bool bPeek=false,bRet=false,bSigned;
	long *plMatrix = (long*)pMatrix;
	short *psMatrix = (short*)pMatrix;
	char *pcMatrix = (char*)pMatrix;
	mxArray *pGlobal=NULL;
	//example [0 0] => Dimensions: 1 2

	if((number_of_dimensions==1) && (dims[0]==1))
		iItemCount = 1;
	else
	if(number_of_dimensions==1)
		iItemCount = (int)dims[0];
	else
	if((number_of_dimensions==2) && (dims[0]==1))
		iItemCount = (int)dims[1];
	else
		mexErrMsgTxt("Invalid Cycle");
	if(!g_pmxGetAcquisitionAscanFifoInfo)
		mexErrMsgTxt("Bad first item");
	iIndex = 0;
	do{
		switch(iMatrixClass)
		{
		case mxINT8_CLASS:
		case mxUINT8_CLASS:		iItem = pcMatrix[iIndex];break;
		case mxINT16_CLASS:
		case mxUINT16_CLASS:	iItem = psMatrix[iIndex];break;
		case mxINT32_CLASS:
		case mxUINT32_CLASS:	iItem = plMatrix[iIndex];break;
		case mxDOUBLE_CLASS:	iItem = iround(pdMatrix[iIndex]);break;
		default:
			mexErrMsgTxt("Bad item array type");
		}
		iByteSize = 0;//because of the BYTE.
		iCycle = 0;
		iPointCount = -1;
		if(g_pmxGetAcquisitionAscanFifoInfo(iId, iItem, &iCycle, &sequence, &iPointCount, (BYTE*)&iByteSize, &bSigned))
			break;
		iIndex++;
	}while(iIndex<iItemCount);
	if(iIndex==iItemCount)
	{
		if(nlhs>=1)
			VALUE_OUT = mxCreateDummy();
		if(nlhs>=2)
			plhs[1] = mxCreateDummy();
		if(nlhs>=3)
			plhs[2] = mxCreateDummy();
		if(nlhs>=4)
			plhs[3] = mxCreateDummy();
		if(nlhs>=5)
			plhs[4] = mxCreateDummy();
		return true;
	}
	iSize = iItemCount*(2*sizeof(double)+2*sizeof(long)+sizeof(long long)+2*sizeof(int));
	if(g_iBufferSize2<iSize)
	{
#ifdef _DEBUG
		if(g_pBuffer2)
			mxDelete(g_pBuffer2,__LINE__,__FILE__);
		g_pBuffer2 = (BYTE*)mxNew(iSize,__LINE__,__FILE__);
#else _DEBUG
		if(g_pBuffer2)
			mxDelete(g_pBuffer2);
		g_pBuffer2 = (BYTE*)mxNew(iSize);
#endif _DEBUG
		if(g_pBuffer2)
			g_iBufferSize2 = iSize;
	}
	if(!g_pBuffer2)
	{
		g_pmexPrintf("Allocation error\n");
		return true;
	}
	pSequence = (long long*)&g_pBuffer2[0];
	pdEncoder = (double*)&g_pBuffer2[iItemCount*sizeof(long long)];
	plRawEncoder = (long*)&g_pBuffer2[iItemCount*(sizeof(long long)+2*sizeof(double))];
	pCycle = (int*)&g_pBuffer2[iItemCount*(sizeof(long long)+2*sizeof(double)+2*sizeof(long))];
	pItem = (int*)&g_pBuffer2[iItemCount*(sizeof(long long)+2*sizeof(double)+2*sizeof(long)+sizeof(int))];
	for(iIndex=0;iIndex<iItemCount;iIndex++)
	{
		switch(iMatrixClass)
		{
		case mxINT8_CLASS:
		case mxUINT8_CLASS:		iItem = pcMatrix[iIndex];break;
		case mxINT16_CLASS:
		case mxUINT16_CLASS:	iItem = psMatrix[iIndex];break;
		case mxINT32_CLASS:
		case mxUINT32_CLASS:	iItem = plMatrix[iIndex];break;
		case mxDOUBLE_CLASS:	iItem = iround(pdMatrix[iIndex]);break;
		default:
			mexErrMsgTxt("Bad item array type");
		}
		pItem[iIndex] = iItem;
	}
	//if(pGlobal=mexGetVariable("global","Ascan"))
	//{
	//	structDimension dim;
	//	mxGetDimension(pGlobal, &dim);
	//	pbyData = (BYTE*)mxGetPr(pGlobal);
	//	//if((dim.number_of_dimensions!=2) || (dim.dims[0]!=iPointCount*iByteSize) || (dim.dims[1]!=iItemCount))
	//	//{//http://fr.mathworks.com/help/matlab/matlab_external/memory-management-issues.html
	//	//	if(pbyData)
	//	//		mxFree(pbyData);
	//	//	pbyData = (BYTE*)mxCalloc2(iPointCount*iItemCount,iByteSize);
	//	//	mxSetData(pGlobal, (double*)pbyData);
	//	//}
	//}
	VALUE_OUT = mxCreateAscanResult2(iPointCount, iItemCount, iByteSize, bSigned, &pbyData);
	bRet = g_pmxGetAcquisitionAscanFifoData(iId, bPeek, iItemCount, pItem, iItemCount*iPointCount*iByteSize, pbyData, &iCount, pCycle, pSequence, 2, pdEncoder, plRawEncoder);
	if(!bRet)
	{
		if(nlhs>=1)
		{
			if(VALUE_OUT)
				mxDestroyArray(VALUE_OUT);
			VALUE_OUT = mxCreateDummy();
		}
		if(nlhs>=2)
			plhs[1] = mxCreateDummy();
		if(nlhs>=3)
			plhs[2] = mxCreateDummy();
		if(nlhs>=4)
			plhs[3] = mxCreateDummy();
		if(nlhs>=5)
			plhs[4] = mxCreateDummy();
		return true;
	}
	if(nlhs>=2)
		plhs[1] = mxCreateLongResult2(1,iItemCount,(long*)pCycle);
	if(nlhs>=3)
		plhs[2] = mxCreateLongLongResult2(1,iItemCount,(long long*)pSequence);
	if(nlhs>=4)
		plhs[3] = mxCreateDoubleResult2(2,iItemCount,pdEncoder);
	if(nlhs>=5)
		plhs[4] = mxCreateLongResult2(2,iItemCount,(long*)plRawEncoder);
//sprintf((char*)wcAux,"FifoData %d %d %d\n", iItemCount,iCount,iItemCount*iPointCount*iByteSize);
//if(g_pmxTraceMemory)
//	(*g_pmxTraceMemory)((char*)wcAux);
	return true;
}
typedef union unionVoid{
	const void *cVoid;
	void *vVoid;
}unionVoid;
bool CStubMatlab::mxDeviceProcess()
{
	int iElementFMC[3],iCycleIndex,iCount;
	long lAscanCount,lAscanLost;
	wchar_t wcAux[MAX_PATH];

	if(g_pmxDumpAcquisitionAscanFifo && (nrhs==3) && !wcscmp(wcFunc,L"mxDumpAcquisitionAscanFifo") && wcString[0])
	{
		if(g_pmxDumpAcquisitionAscanFifo(iId, wcString))
		{
			if(nlhs>=1)
				VALUE_OUT = mxCreateLongResult1(1,&lAscanCount);
		}else{
			if(nlhs>=1)
				VALUE_OUT = mxCreateDummy();
		}
		return true;
	}
	if(g_pmxGetAcquisitionAscanFifoStatus && (nrhs==2) && !wcscmp(wcFunc,L"mxGetAcquisitionAscanFifoStatus"))
	{
		long long llTotalCount,llTotalByteCount;
		if(g_pmxGetAcquisitionAscanFifoStatus(iId, &lAscanCount, &lAscanLost, &llTotalCount, &llTotalByteCount))
		{
			if(nlhs>=1)
				VALUE_OUT = mxCreateLongResult1(1,&lAscanCount);
			if(nlhs>=2)
				plhs[1] = mxCreateLongResult1(1,&lAscanLost);
			if(nlhs>=3)
				plhs[2] = mxCreateLongLongResult1(1,&llTotalCount);
			if(nlhs>=4)
				plhs[3] = mxCreateLongLongResult1(1,&llTotalByteCount);
		}else{
			if(nlhs>=1)
				VALUE_OUT = mxCreateDummy();
			if(nlhs>=2)
				plhs[1] = mxCreateDummy();
			if(nlhs>=3)
				plhs[2] = mxCreateDummy();
		}
		return true;
	}
	if(	((g_pmxGetAcquisitionAscanFifoIndex && !wcscmp(wcFunc,L"mxGetAcquisitionAscanFifoIndex")) ||
		(g_pmxGetAcquisitionAscanLifoIndex && !wcscmp(wcFunc,L"mxGetAcquisitionAscanLifoIndex")) ||
		(g_pmxGetAcquisitionCscanFifoIndex && !wcscmp(wcFunc,L"mxGetAcquisitionCscanFifoIndex")) ||
		(g_pmxGetAcquisitionCscanLifoIndex && !wcscmp(wcFunc,L"mxGetAcquisitionCscanLifoIndex"))
		) && (nrhs==3) && (pMatrix || pdMatrix))
		return mxAscanFifoIndex();
	if(g_pmxGetAcquisitionAscanFifoData && !wcscmp(wcFunc,L"mxGetAcquisitionAscanFifoData") && (nrhs==3) && (pMatrix || pdMatrix))
		return mxAscanFifoData();
	if(g_pmxFlushAcquisitionAscanFifo && (nrhs==2) && !wcscmp(wcFunc,L"mxFlushAcquisitionAscanFifo"))
	{
		if(g_pmxFlushAcquisitionAscanFifo(iId))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxFlushAcquisitionCscanFifo && (nrhs==2) && !wcscmp(wcFunc,L"mxFlushAcquisitionCscanFifo"))
	{
		if(g_pmxFlushAcquisitionCscanFifo(iId))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxGetAcquisitionCscanFifoStatus && (nrhs==2) && !wcscmp(wcFunc,L"mxGetAcquisitionCscanFifoStatus"))
	{
		long lCscanCount, lCscanLost;
		if(g_pmxGetAcquisitionCscanFifoStatus(iId, &lCscanCount, &lCscanLost))
		{
			if(nlhs>=1)
				VALUE_OUT = mxCreateLongResult1(1,&lCscanCount);
			if(nlhs>=2)
				plhs[1] = mxCreateLongResult1(1,&lCscanLost);
		}else{
			lCscanCount = lCscanLost = 0;
			if(nlhs>=1)
				VALUE_OUT = mxCreateLongResult1(1,&lCscanCount);
			if(nlhs>=2)
				plhs[1] = mxCreateLongResult1(1,&lCscanLost);
		}
		return true;
	}
	if(g_pmxGetAcquisitionCscanFifoData && !wcscmp(wcFunc,L"mxGetAcquisitionCscanFifoData")	&& (nrhs==3) && (pMatrix || pdMatrix))
	{
		long long *pSequence;
		int *pCycle,*pItem,iItem,iIndex,iSize,iItemCount;
		double *pdEncoder;
		long *plRawEncoder;
		bool bPeek=false,bRet=false;
		short *psTof;
		BYTE *pAmp;
		long *plMatrix = (long*)pMatrix;
		short *psMatrix = (short*)pMatrix;
		char *pcMatrix = (char*)pMatrix;
		//example [0 0] => Dimensions: 1 2
		if((number_of_dimensions==1) && (dims[0]==1))
			iItemCount = 1;
		else
		if(number_of_dimensions==1)
			iItemCount = (int)dims[0];
		else
		if((number_of_dimensions==2) && (dims[0]==1))
			iItemCount = (int)dims[1];
		else
			mexErrMsgTxt("Invalid Cycle");
		iSize = iItemCount*(2*sizeof(double)+2*sizeof(long)+sizeof(long long)+2*sizeof(int)+4*sizeof(short)+4*sizeof(BYTE));
		if(g_iBufferSize2<iSize)
		{
#ifdef _DEBUG
			if(g_pBuffer2)
				mxDelete(g_pBuffer2,__LINE__,__FILE__);
			g_pBuffer2 = (BYTE*)mxNew(iSize,__LINE__,__FILE__);
#else _DEBUG
			if(g_pBuffer2)
				mxDelete(g_pBuffer2);
			g_pBuffer2 = (BYTE*)mxNew(iSize);
#endif _DEBUG
			if(g_pBuffer2)
				g_iBufferSize2 = iSize;
		}
		if(!g_pBuffer2)
		{
			g_pmexPrintf("Allocation error\n");
			return true;
		}
		pSequence = (long long*)&g_pBuffer2[0];
		pdEncoder = (double*)&g_pBuffer2[iItemCount*sizeof(long long)];
		plRawEncoder = (long*)&g_pBuffer2[iItemCount*(sizeof(long long)+2*sizeof(double))];
		pCycle = (int*)&g_pBuffer2[iItemCount*(sizeof(long long)+2*sizeof(double)+2*sizeof(long))];
		pItem = (int*)&g_pBuffer2[iItemCount*(sizeof(long long)+2*sizeof(double)+2*sizeof(long)+sizeof(int))];
		psTof = (short*)&g_pBuffer2[iItemCount*(sizeof(long long)+2*sizeof(double)+2*sizeof(long)+2*sizeof(int))];
		pAmp = (BYTE*)&g_pBuffer2[iItemCount*(sizeof(long long)+2*sizeof(double)+2*sizeof(long)+2*sizeof(int)+4*sizeof(short))];
		for(iIndex=0;iIndex<iItemCount;iIndex++)
		{
			switch(iMatrixClass)
			{
			case mxINT8_CLASS:
			case mxUINT8_CLASS:		iItem = pcMatrix[iIndex];break;
			case mxINT16_CLASS:
			case mxUINT16_CLASS:	iItem = psMatrix[iIndex];break;
			case mxINT32_CLASS:
			case mxUINT32_CLASS:	iItem = plMatrix[iIndex];break;
			case mxDOUBLE_CLASS:	iItem = iround(pdMatrix[iIndex]);break;
			default:
				g_pmexPrintf("Bad item array type\n");
			}
			pItem[iIndex] = iItem;
		}
		bRet = g_pmxGetAcquisitionCscanFifoData(iId, bPeek, iItemCount, pItem, pAmp, psTof, pCycle, pSequence, 2, pdEncoder, plRawEncoder);
		if(!bRet)
		{
			if(nlhs>=1)
				VALUE_OUT = mxCreateDummy();
			if(nlhs>=2)
				plhs[1] = mxCreateDummy();
			if(nlhs>=3)
				plhs[2] = mxCreateDummy();
			if(nlhs>=4)
				plhs[3] = mxCreateDummy();
			if(nlhs>=5)
				plhs[4] = mxCreateDummy();
			if(nlhs>=6)
				plhs[5] = mxCreateDummy();
			return true;
		}
		if(nlhs>=1)
			VALUE_OUT = mxCreateCharResult2(4,iItemCount,(char*)pAmp);
		if(nlhs>=2)
			plhs[1] = mxCreateShortResult2(4,iItemCount,(short*)psTof);
		if(nlhs>=3)
			plhs[2] = mxCreateLongResult2(1,iItemCount,(long*)pCycle);
		if(nlhs>=4)
			plhs[3] = mxCreateLongLongResult2(1,iItemCount,(long long*)pSequence);
		if(nlhs>=5)
			plhs[4] = mxCreateDoubleResult2(2,iItemCount,pdEncoder);
		if(nlhs>=6)
			plhs[5] = mxCreateLongResult2(2,iItemCount,(long*)plRawEncoder);
		return true;
	}
	if(g_pmxToolboxSaveSetup && (nrhs==3) && !wcscmp(wcFunc,L"mxToolboxSaveSetup") && wcString[0])
	{
		if((*g_pmxToolboxSaveSetup)(iId,wcString))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxToolboxSaveSetupEx && (nrhs==4) && !wcscmp(wcFunc,L"mxToolboxSaveSetupEx") && wcString[0] && wcName[0])
	{
		if((*g_pmxToolboxSaveSetupEx)(iId,wcString,wcName))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxFindDevice && (nrhs==2) && !wcscmp(wcFunc,L"mxFindDevice"))
	{
		if(g_pmxFindDevice(iId))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult0(1.0);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult0(0.0);
		}
		return true;
	}
	if(g_pmxDeleteDevice && (nrhs==2) && !wcscmp(wcFunc,L"mxDeleteDevice"))
	{
		if((*g_pmxDeleteDevice)(iId))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxReadFileWriteHW && (nrhs==3) && !wcscmp(wcFunc,L"mxReadFileWriteHW") && wcString[0])
	{
		if(g_pmxReadFileWriteHW(iId,wcString))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxReadHWWriteFile && (nrhs==3) && !wcscmp(wcFunc,L"mxReadHWWriteFile") && wcString[0])
	{
		if(g_pmxReadHWWriteFile(iId,wcString))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxConnect && (nrhs==3) && !wcscmp(wcFunc,L"mxConnect") && pdMatrix)
	{
		if(g_pmxConnect(iId,(pdMatrix[0]?true:false)))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxIsConnected && (nrhs==2) && !wcscmp(wcFunc,L"mxIsConnected"))
	{
		if((*g_pmxIsConnected)(iId))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxIsUsb3Connected && (nrhs==2) && !wcscmp(wcFunc,L"mxIsUsb3Connected"))
	{
		if((*g_pmxIsUsb3Connected)(iId))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxIsFifo && (nrhs==2) && !wcscmp(wcFunc,L"mxIsFifo"))
	{
		if((*g_pmxIsFifo)(iId))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxIsFullMatrixCaptureReadWrite && (nrhs==2) && !wcscmp(wcFunc,L"mxIsFullMatrixCaptureReadWrite"))
	{
		if((*g_pmxIsFullMatrixCaptureReadWrite)(iId))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxResetCounters && (nrhs==2) && !wcscmp(wcFunc,L"mxResetCounters"))
	{
		if((*g_pmxResetCounters)(iId))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxResetEncoder && (nrhs==2) && !wcscmp(wcFunc,L"mxResetEncoder"))
	{
		if((*g_pmxResetEncoder)(iId))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxGetSerialNumber && (nrhs==2) && !wcscmp(wcFunc,L"mxGetSerialNumber"))
	{
		if(g_pmxGetSerialNumber(iId,wcAux,MAX_PATH))
		{
			if(nlhs==1)
			{
				//dwConvert=(DWORD)WideCharToMultiByte(CP_ACP, 0, wcAux, -1, dir, _MAX_DIR, NULL, NULL);
				VALUE_OUT = (mxArray*)mxCreateString2(wcAux);
			}
		}
		return true;
	}
	if(g_pmxGetStatus && (nrhs==2) && !wcscmp(wcFunc,L"mxGetStatus"))
	{
		if(g_pmxGetStatus(iId,wcAux,MAX_PATH))
		{
			if(nlhs==1)
			{
				//dwConvert=(DWORD)WideCharToMultiByte(CP_ACP, 0, wcAux, -1, dir, _MAX_DIR, NULL, NULL);
				VALUE_OUT = (mxArray*)mxCreateString2(wcAux);
			}
		}
		return true;
	}
	if(g_pmxEnableShot && (nrhs==3) && !wcscmp(wcFunc,L"mxEnableShot") && pdMatrix)
	{
		if(g_pmxEnableShot(iId,(pdMatrix[0]?true:false)))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxEnableUsb3 && (nrhs==3) && !wcscmp(wcFunc,L"mxEnableUsb3") && pdMatrix)
	{
		if(g_pmxEnableUsb3(iId,(pdMatrix[0]?true:false)))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxGetAcquisitionStep && (nrhs==2) && !wcscmp(wcFunc,L"mxGetAcquisitionStep"))
	{
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0((double)(*g_pmxGetAcquisitionStep)(iId));
		return true;
	}
	if(g_pmxGetSWCycleCount && (nrhs==2) && !wcscmp(wcFunc,L"mxGetSWCycleCount"))
	{
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0((double)(*g_pmxGetSWCycleCount)(iId));
		return true;
	}
	if(g_pmxGetHWCycleCount && (nrhs==2) && !wcscmp(wcFunc,L"mxGetHWCycleCount"))
	{
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0((double)(*g_pmxGetHWCycleCount)(iId));
		return true;
	}
	if(g_pmxPerformCalibration && (nrhs==2) && !wcscmp(wcFunc,L"mxPerformCalibration"))
	{
		double dTimeOffset, dDelayCorrectionMax;
		if((*g_pmxPerformCalibration)(iId, &dTimeOffset, &dDelayCorrectionMax))
		{
			if(nlhs>=1)
				VALUE_OUT = mxCreateDoubleResult0((double)dTimeOffset);
			if(nlhs>=1)
				plhs[1] = mxCreateDoubleResult0((double)dDelayCorrectionMax);
		}else{
			if(nlhs>=1)
				VALUE_OUT = mxCreateDoubleResult0((double)0.0);
			if(nlhs>=1)
				plhs[1] = mxCreateDoubleResult0((double)0.0);
		}
		return true;
	}
	if(g_pmxEnableAlignment && (nrhs==3) && !wcscmp(wcFunc,L"mxEnableAlignment") && pdMatrix)
	{
		if(((number_of_dimensions==1) && (dims[0]==1)) || ((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==1)))
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0((double)(*g_pmxEnableAlignment)(iId, *pdMatrix==0.0?false:true));
		return true;
	}
	if(g_pmxSetTimeOffset && (nrhs==3) && !wcscmp(wcFunc,L"mxSetTimeOffset") && pdMatrix)
	{
		if(((number_of_dimensions==1) && (dims[0]==1)) || ((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==1)))
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0((double)(*g_pmxSetTimeOffset)(iId, *pdMatrix));
		return true;
	}
	if(g_pmxSetDelayOffset && (nrhs==3) && !wcscmp(wcFunc,L"mxSetDelayOffset") && pdMatrix)
	{
		if(((number_of_dimensions==1) && (dims[0]==1)) || ((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==1)))
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0((double)(*g_pmxSetDelayOffset)(iId, *pdMatrix));
		return true;
	}
	if(g_pmxGetFullMatrixCapture && (nrhs==2) && !wcscmp(wcFunc,L"mxGetFullMatrixCapture"))
	{
		if((nlhs==1) && (*g_pmxGetFullMatrixCapture)(iId,&iElementFMC[0],&iElementFMC[1],&iElementFMC[2]))
			VALUE_OUT = mxCreateIntegerResult2(1,3,&iElementFMC[0]);
		return true;
	}
	if(g_pmxGetAcquisitionAscanCount && (nrhs==2) && !wcscmp(wcFunc,L"mxGetAcquisitionAscanCount"))
	{
		__int64 i64Count, i64TotalByte;
		if((*g_pmxGetAcquisitionAscanCount)(iId,i64Count,i64TotalByte))
		{
			if(nlhs>=1)
				VALUE_OUT = mxCreateLongLongResult1(1, &i64Count);
			if(nlhs>=2)
				plhs[1] = mxCreateLongLongResult1(1, &i64TotalByte);
		}
		return true;
	}
	if(g_pmxGetAcquisitionAscanEncoder && (nrhs==3) && !wcscmp(wcFunc,L"mxGetAcquisitionAscanEncoder") && pdMatrix)
	{
		double dEncoder;
		long lRawEncoder;
		if(((number_of_dimensions==1) && (dims[0]==1)) || ((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==1)))
		{
			if((*g_pmxGetAcquisitionAscanEncoder)(iId,iround(*pdMatrix),&dEncoder,&lRawEncoder))
			{
				if(nlhs>=1)
					VALUE_OUT = mxCreateDoubleResult0((double)dEncoder);
				if(nlhs>=2)
					plhs[1] = mxCreateLongResult1(1,&lRawEncoder);
			}
		}
		return true;
	}
	if(g_pmxGetAcquisitionAscanSize && (nrhs==3) && !wcscmp(wcFunc,L"mxGetAcquisitionAscanSize") && pdMatrix)
	{
		if(((number_of_dimensions==1) && (dims[0]==1)) || ((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==1)))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult0((double)(*g_pmxGetAcquisitionAscanSize)(iId,iround(*pdMatrix)));
		}
		return true;
	}
	if(g_pmxGetAcquisitionAscanBitSize && (nrhs==3) && !wcscmp(wcFunc,L"mxGetAcquisitionAscanBitSize") && pdMatrix)
	{
		if(((number_of_dimensions==1) && (dims[0]==1)) || ((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==1)))
		{
			wchar_t out[20];
			bool bSigned;
			switch((*g_pmxGetAcquisitionAscanBitSize)(iId,iround(*pdMatrix),&bSigned))
			{
			default:
				if(nlhs==1)
					VALUE_OUT = mxCreateDummy();
				if(nlhs>=2)
					plhs[1] = mxCreateDummy();
				return true;
			case e8Bits:	wcscpy_s(out,20,L"8Bits");break;
			case e12Bits:	wcscpy_s(out,20,L"12Bits");break;
			case e16Bits:	wcscpy_s(out,20,L"16Bits");break;
			case eLog8Bits:	wcscpy_s(out,20,L"Log8Bits");break;
			}
			VALUE_OUT = mxCreateString2(out);
			if(nlhs>=2)
			{
				if(bSigned)
					plhs[1] = mxCreateString2(L"Signed");
				else
					plhs[1] = mxCreateString2(L"Unsigned");
			}
		}
		return true;
	}
	if(g_pmxGetAcquisitionAscanData && (nrhs==3) && !wcscmp(wcFunc,L"mxGetAcquisitionAscanData") && pdMatrix)
	{
		if(((number_of_dimensions==1) && (dims[0]==1)) || ((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==1)))
		{
			bool bSigned;
			iCycleIndex = iround(*pdMatrix);
			if((nlhs==1) && (*g_pmxGetAcquisitionAscanData)(iId,iCycleIndex,16384,&g_pBuffer[0],&iCount))
			{
				switch((*g_pmxGetAcquisitionAscanBitSize)(iId,iCycleIndex,&bSigned))
				{
				case e8Bits:
				case eLog8Bits:
					if(bSigned)
						VALUE_OUT = mxCreateCharResult1(iCount,(char*)g_pBuffer);
					else
						VALUE_OUT = mxCreateByteResult1(iCount,(BYTE*)g_pBuffer);
					break;
				case e12Bits:
				case e16Bits:
					if(bSigned)
						VALUE_OUT = mxCreateShortResult1(iCount/2,(short*)g_pBuffer);
					else
						VALUE_OUT = mxCreateWordResult1(iCount/2,(WORD*)g_pBuffer);
					break;
				}
				return true;
			}else
				VALUE_OUT = mxCreateDummy();
		}
		return true;
	}
	if(g_pmxGetAcquisitionCscanCount && (nrhs==2) && !wcscmp(wcFunc,L"mxGetAcquisitionCscanCount"))
	{
		__int64 i64Count;
		if((*g_pmxGetAcquisitionCscanCount)(iId, i64Count))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateLongLongResult1(1,&i64Count);
		}
		return true;
	}
	if(g_pmxGetAcquisitionCscanEncoder && (nrhs==3) && !wcscmp(wcFunc,L"mxGetAcquisitionCscanEncoder") && pdMatrix)
	{
		double dEncoder;
		long lRawEncoder;
		if(((number_of_dimensions==1) && (dims[0]==1)) || ((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==1)))
		{
			if((*g_pmxGetAcquisitionCscanEncoder)(iId,iround(*pdMatrix),&dEncoder,&lRawEncoder))
			if(nlhs>=1)
				VALUE_OUT = mxCreateDoubleResult0((double)dEncoder);
			if(nlhs>=2)
				plhs[1] = mxCreateLongResult1(1,&lRawEncoder);
		}
		return true;
	}
	if(g_pmxGetAcquisitionCscanEnable && (nrhs==3) && !wcscmp(wcFunc,L"mxGetAcquisitionCscanEnable") && pdMatrix)
	{
		if(((number_of_dimensions==1) && (dims[0]==1)) || ((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==1)))
		{
			if((nlhs==1) && (*g_pmxGetAcquisitionCscanEnable)(iId,iround(*pdMatrix),g_iBufferSize,&g_pBuffer[0],&iCount))
			{
				VALUE_OUT = mxCreateCharResult1(iCount,(char*)g_pBuffer);
			}
		}
		return true;
	}
	if(g_pmxGetAcquisitionCscanAmplitude && (nrhs==3) && !wcscmp(wcFunc,L"mxGetAcquisitionCscanAmplitude") && pdMatrix)
	{
		if(((number_of_dimensions==1) && (dims[0]==1)) || ((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==1)))
		{
			if((nlhs==1) && (*g_pmxGetAcquisitionCscanAmplitude)(iId,iround(*pdMatrix),g_iBufferSize,&g_pBuffer[0],&iCount))
			{
				VALUE_OUT = mxCreateCharResult1(iCount,(char*)g_pBuffer);
			}
		}
		return true;
	}
	if(g_pmxGetAcquisitionCscanTimeOfFlight && (nrhs==3) && !wcscmp(wcFunc,L"mxGetAcquisitionCscanTimeOfFlight") && pdMatrix)
	{
		if(((number_of_dimensions==1) && (dims[0]==1)) || ((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==1)))
		{
			if((nlhs==1) && (*g_pmxGetAcquisitionCscanTimeOfFlight)(iId,iround(*pdMatrix),g_iBufferSize/sizeof(short),(short*)&g_pBuffer[0],&iCount))
			{
				VALUE_OUT = mxCreateShortResult1(iCount,(short*)g_pBuffer);
			}
		}
		return true;
	}
	if(g_pmxSetGainAnalog && (nrhs==3) && !wcscmp(wcFunc,L"mxSetGainAnalog") && pdMatrix) // SetGainAnalog
	{
		int iCycle;
		float fGain;
		iCycle = iround(*pdMatrix);
		fGain = (float)pdMatrix[1];
		if((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==2))
		{
			if((*g_pmxSetGainAnalog)(iId,iCycle,&fGain))
			{
				if(nlhs>=1)
					VALUE_OUT = mxCreateByteResult0(1);
			}else{
				if(nlhs>=1)
					VALUE_OUT = mxCreateByteResult0(0);
			}
			if(nlhs==2)
				plhs[1] = mxCreateDoubleResult0((double)fGain);
		}
		return true;
	}
	if(g_pmxSetGainDigital && (nrhs==3) && !wcscmp(wcFunc,L"mxSetGainDigital") && pdMatrix) // SetGainDigital
	{
		int iCycle;
		double dGain;

		iCycle = iround(*pdMatrix);
		dGain = (double)pdMatrix[1];
		if(((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==2)))
		{
			if((*g_pmxSetGainDigital)(iId,iCycle,&dGain))
			{
				if(nlhs>=1)
					VALUE_OUT = mxCreateByteResult0(1);
			}else{
				if(nlhs>=1)
					VALUE_OUT = mxCreateByteResult0(0);
			}
			if (nlhs==2)
				plhs[1] = mxCreateDoubleResult0(dGain);
		}
		return true;
	}
	if(g_pmxSetAscanRange && (nrhs==3) && !wcscmp(wcFunc,L"mxSetAscanRange") && pdMatrix) // SetAscanRange
	{
		long lPointCount,lPointFactor;
		int iCycle;
		BYTE bCompression;
		double dRange;
		iCycle = iround(pdMatrix[0]);
		dRange = pdMatrix[1];
		if(((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==2)))
		{
			if((*g_pmxSetAscanRange)(iId,iCycle,&dRange,&bCompression,&lPointCount,&lPointFactor))
			{
				if(nlhs>=1)
					VALUE_OUT = mxCreateByteResult0(1);
			}else{
				if(nlhs>=1)
					VALUE_OUT = mxCreateByteResult0(0);
			}
			if(nlhs>=2)
				plhs[1] = mxCreateDoubleResult0(dRange);
			if(nlhs>=3)
				plhs[2] = mxCreateCharResult1(1,(char*)&bCompression);
			if(nlhs>=4)
				plhs[3] = mxCreateLongResult1(1,(long*)&lPointCount);
			if(nlhs==5)
				plhs[4] = mxCreateLongResult1(1,(long*)&lPointFactor);
		}
		return true;
	}
	if(g_pmxSetAscanRangeWithFactor && (nrhs==3) && !wcscmp(wcFunc,L"mxSetAscanRangeWithFactor") && pdMatrix) // SetAscanRangeWithFactor
	{
		long lPointCount,lPointFactor;
		int iCycle;
		BYTE bCompression;
		double dRange;
		iCycle = iround(pdMatrix[0]);
		dRange = pdMatrix[1];
		bCompression = (BYTE)iround(pdMatrix[2]);
		lPointFactor = (long)iround(pdMatrix[3]);
		if(((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==4)))
		{
			if((*g_pmxSetAscanRangeWithFactor)(iId,iCycle,&dRange,&bCompression,&lPointFactor,&lPointCount))
			{
				if(nlhs>=1)
					VALUE_OUT = mxCreateByteResult0(1);
			}else{
				if(nlhs>=1)
					VALUE_OUT = mxCreateByteResult0(0);
			}
			if(nlhs>=2)
				plhs[1] = mxCreateDoubleResult0(dRange);
			if(nlhs>=3)
				plhs[2] = mxCreateCharResult1(1,(char*)&bCompression);
			if(nlhs>=4)
				plhs[3] = mxCreateLongResult1(1,(long*)&lPointCount);
			if(nlhs==5)
				plhs[4] = mxCreateLongResult1(1,(long*)&lPointFactor);
		}
		return true;
	}
	if(g_pmxSetAscanStart && (nrhs==3) && !wcscmp(wcFunc,L"mxSetAscanStart") && pdMatrix) // SetAscanStart
	{
        int iCycle;
		double dStart;
		iCycle = iround(pdMatrix[0]);
		dStart = pdMatrix[1];
		if(((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==2)))
		{
			if((*g_pmxSetAscanStart)(iId,iCycle,&dStart))
			{
				if(nlhs>=1)
					VALUE_OUT = mxCreateByteResult0(1);
			}else{
				if(nlhs>=1)
					VALUE_OUT = mxCreateByteResult0(0);
			}
			if(nlhs==2)
				plhs[1] = mxCreateDoubleResult0(dStart);
		}
		return true;
	}
	if(g_pmxSetFilterIndex && (nrhs==3) && !wcscmp(wcFunc,L"mxSetFilterIndex") && pdMatrix) // SetFilterIndex
	{
        int iCycle, iFilterIndex;
		iCycle = iround(pdMatrix[0]);
		iFilterIndex = iround(pdMatrix[1]);
		if((iFilterIndex < 0) || (iFilterIndex > 15))
			mexErrMsgTxt("Invalid Filter Index");
		if(((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==2)))
		{
			if((*g_pmxSetFilterIndex)(iId,iCycle,iFilterIndex))
			{
				if(nlhs>=1)
					VALUE_OUT = mxCreateByteResult0(1);
			}else{
				if(nlhs>=1)
					VALUE_OUT = mxCreateByteResult0(0);
			}
		}
		return true;
	}
	if(g_pmxSetFilter && (nrhs==3) && !wcscmp(wcFunc,L"mxSetFilter") && pdMatrix) // SetFilter
	{
        int iFilterIndex, i;
		WORD wScale;
		short wValue[g_iOEMPAFilterCoefficientMax];
		iFilterIndex = iround(pdMatrix[0]);
		wScale = iround(pdMatrix[1]);
		if((number_of_dimensions!=1) && (number_of_dimensions!=2))
			mexErrMsgTxt("Invalid Filter Coefficients");
		if((number_of_dimensions==1) && (dims[0]!=g_iOEMPAFilterCoefficientMax+2))
			mexErrMsgTxt("Invalid Filter Coefficients");
		if((number_of_dimensions==2) && ((dims[0]==1) && (dims[1]!=g_iOEMPAFilterCoefficientMax+2)))
			mexErrMsgTxt("Invalid Filter Coefficients");
		for (i = 0; i < g_iOEMPAFilterCoefficientMax; i++)
			wValue[i] = (short)iround(pdMatrix[i+2]);
		if((iFilterIndex < 1) || (iFilterIndex > 15))
			mexErrMsgTxt("Invalid Filter Index");
		if(((number_of_dimensions==2) && (dims[0]==1) && (dims[1]==36)))
		{
			if((*g_pmxSetFilter)(iId,iFilterIndex,&wScale,wValue))
			{
				if(nlhs>=1)
					VALUE_OUT = mxCreateByteResult0(1);
			}else{
				if(nlhs>=1)
					VALUE_OUT = mxCreateByteResult0(0);
			}
		}
		return true;
	}
	if(g_pmxLockDevice && (nrhs==3) && !wcscmp(wcFunc,L"mxLockDevice") && pdMatrix) // LockDevice
	{
		if((*g_pmxLockDevice)(iId,iround(*pdMatrix)))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxUnlockDevice && (nrhs==3) && !wcscmp(wcFunc,L"mxUnlockDevice") && pdMatrix) // UnlockDevice
	{
		if((*g_pmxUnlockDevice)(iId,iround(*pdMatrix)))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
    if(g_pmxGetLostCountAscan && (nrhs==2) && !wcscmp(wcFunc,L"mxGetLostCountAscan")) // GetLostCountAscan
	{
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0((double)(*g_pmxGetLostCountAscan)(iId));
		return true;
	}
    if(g_pmxGetLostCountCscan && (nrhs==2) && !wcscmp(wcFunc,L"mxGetLostCountCscan")) // GetLostCountCscan
	{
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0((double)(*g_pmxGetLostCountCscan)(iId));
		return true;
	}
    if(g_pmxGetLostCountEncoder && (nrhs==2) && !wcscmp(wcFunc,L"mxGetLostCountEncoder")) // GetLostCountEncoder
	{
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0((double)(*g_pmxGetLostCountEncoder)(iId));
		return true;
	}
    if(g_pmxGetLostCountUSB3 && (nrhs==2) && !wcscmp(wcFunc,L"mxGetLostCountUSB3")) // GetLostCountUSB3
	{
		if(nlhs==1)
			VALUE_OUT = mxCreateDoubleResult0((double)(*g_pmxGetLostCountUSB3)(iId));
		return true;
	}
	if(g_pmxGetRecoveryTimes && (nrhs==2) && !wcscmp(wcFunc,L"mxGetRecoveryTimes"))
	{
		double pdData[3];
		double dAscanRecoveryTime, dFMCSubCycleRecoveryTime, dFMCCycleRecoveryTime;
		if(g_pmxGetRecoveryTimes(iId, dAscanRecoveryTime, dFMCSubCycleRecoveryTime, dFMCCycleRecoveryTime))
		{
			pdData[0] = dAscanRecoveryTime;
			pdData[1] = dFMCSubCycleRecoveryTime;
			pdData[2] = dFMCCycleRecoveryTime;
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult2(3,1,pdData);
		}
		return true;
	}
    if(g_pmxGetFMCTimeLimitation && (nrhs==3) && !wcscmp(wcFunc,L"mxGetFMCTimeLimitation") && pdMatrix) // GetLostCountUSB3
	{
		double dAscanStart,dAscanRange,dTimeSlot,dTimeSlotMin,dHWAcqSubTimeSlotMin,dReplaySubTimeSlotMin,dReplaySubTimeSlotOptimizedForThroughput;
		int iDim=0,iFMCSubCycleCount;
		//g_pmexPrintf("number_of_dimensions %d dims[0] %d dims[1] %d\n",number_of_dimensions,dims[0],dims[1]);
		if((number_of_dimensions==1) && (dims[0]>=4))
			iDim = 1;
		else
		if((number_of_dimensions==2) && (dims[0]==1) && (dims[1]>=4))
			iDim = 1;
		if(!iDim)
			mexErrMsgTxt("Five inputs are required");
		iFMCSubCycleCount = iround(pdMatrix[0]);
		dAscanStart = pdMatrix[1];
		dAscanRange = pdMatrix[2];
		dTimeSlot = pdMatrix[3];
		if(!(*g_pmxGetFMCTimeLimitation)(iId,iFMCSubCycleCount,dAscanStart,dAscanRange,dTimeSlot,&dTimeSlotMin,&dHWAcqSubTimeSlotMin,&dReplaySubTimeSlotMin,&dReplaySubTimeSlotOptimizedForThroughput))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateDummy();
		}else{
			double pdData[4];
			pdData[0] = dTimeSlotMin;
			pdData[1] = dHWAcqSubTimeSlotMin;
			pdData[2] = dReplaySubTimeSlotMin;
			pdData[3] = dReplaySubTimeSlotOptimizedForThroughput;
			if(nlhs==1)
				VALUE_OUT = mxCreateDoubleResult2(4,1,pdData);
		}
		return true;
	}
    if(g_pmxSetFMCSubTimeSlot && (nrhs==3) && !wcscmp(wcFunc,L"mxSetFMCSubTimeSlot") && pdMatrix) // GetLostCountUSB3
	{
		double dHWAcqSubTimeSlot,dReplaySubTimeSlot;
		int iDim=0,iCycle;
		//g_pmexPrintf("number_of_dimensions %d dims[0] %d dims[1] %d\n",number_of_dimensions,dims[0],dims[1]);
		if((number_of_dimensions==1) && (dims[0]>=3))
			iDim = 1;
		else
		if((number_of_dimensions==2) && (dims[0]==1) && (dims[1]>=3))
			iDim = 1;
		if(!iDim)
			mexErrMsgTxt("Four inputs are required");
		iCycle = iround(pdMatrix[0]);
		dHWAcqSubTimeSlot = pdMatrix[1];
		dReplaySubTimeSlot = pdMatrix[2];
		if((*g_pmxSetFMCSubTimeSlot)(iId,iCycle,dHWAcqSubTimeSlot,dReplaySubTimeSlot))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
    if(g_pmxSetTimeSlot && (nrhs==3) && !wcscmp(wcFunc,L"mxSetTimeSlot") && pdMatrix) // GetLostCountUSB3
	{
		double dTimeSlot;
		int iDim=0,iCycle;
		//g_pmexPrintf("number_of_dimensions %d dims[0] %d dims[1] %d\n",number_of_dimensions,dims[0],dims[1]);
		if((number_of_dimensions==1) && (dims[0]>=2))
			iDim = 1;
		else
		if((number_of_dimensions==2) && (dims[0]==1) && (dims[1]>=2))
			iDim = 1;
		if(!iDim)
			mexErrMsgTxt("Four inputs are required");
		iCycle = iround(pdMatrix[0]);
		dTimeSlot = pdMatrix[1];
		if((*g_pmxSetTimeSlot)(iId,iCycle,dTimeSlot))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxExample1 && (nrhs==2) && !wcscmp(wcFunc,L"mxExample1"))
	{
		g_pmexPrintf("mxExample1 : example of trace in 'utCmd.c'!\n");
		if((nlhs==1) && (*g_pmxExample1)(iId))
		{
			VALUE_OUT = mxCreateDoubleResult0((double)1.23);
		}
		return true;
	}
	if(g_pmxExample2 && (nrhs==3) && !wcscmp(wcFunc,L"mxExample2") && pdMatrix)
	{
		if((number_of_dimensions==1) || ((number_of_dimensions==2) && (dims[0]==1) || (dims[1]==1)))
		{
			if(nlhs==1)
			{
				double pDataOut[5];
				int iSizeOut=5;
				if((*g_pmxExample2)(iId,dims[0]>dims[1]?dims[0]:dims[1],pdMatrix,&iSizeOut,pDataOut))
					VALUE_OUT = mxCreateDoubleResult1(iSizeOut,pDataOut);
			}
		}
		return true;
	}
	return false;
}
bool CStubMatlab::mxDevice256Process()
{
	long lAscanCount,lAscanLost,lAscanTotal;
	long long llByteTotal;

	if(g_pmxReadFileWriteHW256 && (nrhs==4) && !wcscmp(wcFunc,L"mxReadFileWriteHW256") && wcString[0] && wcName[0])
    {
        int iCyclesMaster=-1, iCyclesSlave=-1;
		if((*g_pmxReadFileWriteHW256)(iId,iId2,wcString,wcName,&iCyclesMaster,&iCyclesSlave))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
            {
                if(iCyclesMaster!=iCyclesSlave)
                    g_pmexPrintf("Setup files have different number of cycles");
				VALUE_OUT = mxCreateByteResult0(0);
            }
		}
		return true;
	}
	if(g_pmxEnableShot256 && (nrhs==3) && !wcscmp(wcFunc,L"mxEnableShot256") && pdMatrix)
	{
		if(g_pmxEnableShot256(iId,iId2,(pdMatrix[0]?true:false)))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxLockDevice256 && (nrhs==3) && !wcscmp(wcFunc,L"mxLockDevice256") && pdMatrix) // LockDevice256
	{
		if((*g_pmxLockDevice256)(iId,iId2,iround(*pdMatrix)))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxUnlockDevice256 && (nrhs==3) && !wcscmp(wcFunc,L"mxUnlockDevice256") && pdMatrix) // UnlockDevice256
	{
		if((*g_pmxUnlockDevice256)(iId,iId2,iround(*pdMatrix)))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxGetAcquisitionAscanFifoStatus256 && (nrhs==2) && !wcscmp(wcFunc,L"mxGetAcquisitionAscanFifoStatus256"))
	{
		if(g_pmxGetAcquisitionAscanFifoStatus256(iId, &lAscanCount, &lAscanLost, &lAscanTotal, &llByteTotal))
		{
			if(nlhs>=1)
				VALUE_OUT = mxCreateLongResult1(1,&lAscanCount);
			if(nlhs>=2)
				plhs[1] = mxCreateLongResult1(1,&lAscanLost);
			if(nlhs>=3)
				plhs[2] = mxCreateLongResult1(1,&lAscanTotal);
			if(nlhs>=4)
				plhs[3] = mxCreateLongLongResult1(1,&llByteTotal);
		}else{
			lAscanCount = lAscanLost = lAscanTotal = 0;
			llByteTotal = 0;
			if(nlhs>=1)
				VALUE_OUT = mxCreateLongResult1(1,&lAscanCount);
			if(nlhs>=2)
				plhs[1] = mxCreateLongResult1(1,&lAscanLost);
			if(nlhs>=3)
				plhs[2] = mxCreateLongResult1(1,&lAscanTotal);
			if(nlhs>=4)
				plhs[3] = mxCreateLongLongResult1(1,&llByteTotal);
		}
		return true;
	}
	if(g_pmxResetCounters256 && (nrhs==2) && !wcscmp(wcFunc,L"mxResetCounters256"))
	{
		if(g_pmxResetCounters256(iId,iId2))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	if(g_pmxFlushAcquisitionAscanFifo256 && (nrhs==2) && !wcscmp(wcFunc,L"mxFlushAcquisitionAscanFifo256"))
	{
		if(g_pmxFlushAcquisitionAscanFifo256(iId,iId2))
		{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(1);
		}else{
			if(nlhs==1)
				VALUE_OUT = mxCreateByteResult0(0);
		}
		return true;
	}
	return false;
}

#ifdef _DEBUG
bool CrtCheckMemory()
{
	if (::_CrtCheckMemory())
		return true;
	return false;//error
}
void CrtSetDBFlag(bool bEnable)
{
	if (bEnable)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF & _CRTDBG_CHECK_ALWAYS_DF);
	else
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
}
#endif _DEBUG
int g_aiDeviceId[]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
					-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
					-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
					-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
const int g_iDeviceCountMax=sizeof(g_aiDeviceId)/sizeof(int);
pmxNewDevice g_pmxNewDevice=NULL;
int mxNewDevice(wchar_t *pIPAddress, int iOEMPACycleCountMax, int iAscanPointCount, int bEnableFifo)
{
	int iRet;
	int iDeviceId=-1;

	for(int iDeviceIndex=0;iDeviceIndex<g_iDeviceCountMax;iDeviceIndex++)
	{
		if(g_aiDeviceId[iDeviceIndex]==-1)
		{
			iDeviceId = iDeviceIndex;
			break;
		}
	}
	if(iDeviceId==-1)
		return -1;
	iRet = g_pmxNewDevice(pIPAddress, iOEMPACycleCountMax, iAscanPointCount, bEnableFifo);
	if(iRet>=0)
		g_aiDeviceId[iDeviceId] = iRet;
	return iRet;
}
pmxDeleteDevice g_pmxDeleteDevice=NULL;
bool mxDeleteDevice(int iDeviceId)
{
	bool bRet;
	int iDeviceId2=-1;

	for(int iDeviceIndex=0;iDeviceIndex<g_iDeviceCountMax;iDeviceIndex++)
	{
		if(g_aiDeviceId[iDeviceIndex]==iDeviceId)
		{
			iDeviceId2 = iDeviceIndex;
			break;
		}
	}
	bRet = g_pmxDeleteDevice(iDeviceId);
	if(bRet && (iDeviceId2>=0))
		g_aiDeviceId[iDeviceId2] = -1;
	return bRet;
}
void mxEnter()
{
#ifdef _DEBUG
	CrtCheckMemory();
#endif _DEBUG
}
void mxExit()
{
#if defined(mex_MALLOC) && !defined(mex_MALLOC_PERSISTENT)
	g_iBufferSize2 = 0;
	g_iBufferSize3 = 0;
#endif
#ifdef _DEBUG
	CrtCheckMemory();
#endif _DEBUG
}
void ExitDll()
{
	if(mxMain.g_pExitDll)
		(*mxMain.g_pExitDll)();
	for(int iDeviceIndex=0;iDeviceIndex<g_iDeviceCountMax;iDeviceIndex++)
	{
		if(g_aiDeviceId[iDeviceIndex]>=0)
			mxMain.g_pmxDeleteDevice(g_aiDeviceId[iDeviceIndex]);
		g_aiDeviceId[iDeviceIndex] = -1;
	}
}
void mxStackTest(BYTE *pBuffer,int iSize)
{
	//int i;
	//for(i=0;i<iSize;i++)
	//	pBuffer[i] = 0;
}
bool mxCheckCompilation(bool bDebug)
{
#ifdef _DEBUG
	if(!bDebug)
	{
		mxMain.g_pmexPrintf("FATAL ERROR: Compilation insconsistancy\n");
		return false;
	}
#else _DEBUG
	if(bDebug)
	{
		mxMain.g_pmexPrintf("FATAL ERROR: Compilation insconsistancy\n");
		return false;
	}
#endif _DEBUG
	return true;
}


const int DLL_COUNT=9;
static VS_FIXEDFILEINFO *g_paVS_FIXEDFILEINFO[DLL_COUNT]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
static wchar_t g_pFileVersion[DLL_COUNT][MAX_PATH]={L"",L"",L"",L"",L"",L"",L"",L"",L""};
static wchar_t g_pProductVersion[DLL_COUNT][MAX_PATH]={L"",L"",L"",L"",L"",L"",L"",L"",L""};
static HMODULE g_hModule[DLL_COUNT]={0,0,0,0,0,0,0,0,0};
static int g_iLoadCount=0;

typedef struct structLocalVariables{
	//Local variables of functions "mxStubLibrary"
	wchar_t drive[_MAX_DRIVE];
	wchar_t dir[_MAX_DIR];
	wchar_t fnameIn[_MAX_FNAME];
	wchar_t fname[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];
	wchar_t wcStubMatlab[MAX_PATH];
	wchar_t wcThisFileVersion[MAX_PATH];
	wchar_t wcThisProductVersion[MAX_PATH];
	//Local variables of functions "myLoadLibrary"
	wchar_t pRet[MAX_PATH];
	char cName[_MAX_FNAME];
}structLocalVariables;
structLocalVariables g_Local;

bool myLoadLibrary(	const wchar_t *_pName, wchar_t *wcThisFileVersion,wchar_t *wcThisProductVersion,
					wchar_t *pFileVersion,wchar_t *pProductVersion,VS_FIXEDFILEINFO **ppVS_FIXEDFILEINF,HMODULE *pModule)
{
	bool bRet=true;
	VS_FIXEDFILEINFO *pVS_FIXEDFILEINF;
	DWORD dwConvert,dwError;

	wcscpy_s(g_Local.pRet,MAX_PATH,g_Local.drive);
	wcscat_s(g_Local.pRet,MAX_PATH,g_Local.dir);
	wcscat_s(g_Local.pRet,MAX_PATH,_pName);
	wcscat_s(g_Local.pRet,MAX_PATH,g_Local.ext);
	Sleep(10);
	*pModule = LoadLibrary(g_Local.pRet);
	if(*pModule)
	{
		pVS_FIXEDFILEINF = (VS_FIXEDFILEINFO*)GetProcAddress(*pModule,"g_VS_FIXEDFILEINFO");
		*ppVS_FIXEDFILEINF = pVS_FIXEDFILEINF;
		if(pVS_FIXEDFILEINF)
		{
			swprintf(pFileVersion,MAX_PATH,L"%d.%d.%d.%d",
				(int)HIWORD(pVS_FIXEDFILEINF->dwFileVersionMS),(int)LOWORD(pVS_FIXEDFILEINF->dwFileVersionMS),
				(int)HIWORD(pVS_FIXEDFILEINF->dwFileVersionLS),(int)LOWORD(pVS_FIXEDFILEINF->dwFileVersionLS));
			swprintf(pProductVersion,MAX_PATH,L"%d.%d.%d.%d",
				(int)HIWORD(pVS_FIXEDFILEINF->dwProductVersionMS),(int)LOWORD(pVS_FIXEDFILEINF->dwProductVersionMS),
				(int)HIWORD(pVS_FIXEDFILEINF->dwProductVersionLS),(int)LOWORD(pVS_FIXEDFILEINF->dwProductVersionLS));
			if(wcscmp(wcThisFileVersion,pFileVersion) || wcscmp(wcThisProductVersion,pProductVersion))
			{
				dwConvert = (DWORD)WideCharToMultiByte(CP_ACP, 0, _pName, -1, g_Local.cName, _MAX_FNAME, NULL, NULL);
				mxMain.g_pmexPrintf("Version is different: \"%s.dll\" (FileVersion=%s ProductVersion=%s)\n",
					g_Local.cName,pFileVersion,pProductVersion);
			}
			//else
			//	mexPrintf("\"%s\" OK 0x%.8lX\n",pName,*pModule);
		}else{
			dwConvert = (DWORD)WideCharToMultiByte(CP_ACP, 0, _pName, -1, g_Local.cName, _MAX_FNAME, NULL, NULL);
			mxMain.g_pmexPrintf("\"%s\" no version number\n",g_Local.cName);
			swprintf(pFileVersion,MAX_PATH,L"0.0.0.0");
			swprintf(pProductVersion,MAX_PATH,L"0.0.0.0");
			//bRet = false;
		}
		return true;
	}else{
		dwError = GetLastError();
		dwConvert = (DWORD)WideCharToMultiByte(CP_ACP, 0, _pName, -1, g_Local.cName, _MAX_FNAME, NULL, NULL);
		mxMain.g_pmexPrintf("LoadLibrary \"%s.dll\"\n\t=> failed (GetLastError(): 0x%.8lX)\n",g_Local.pRet,dwError);
		//bRet = false;
		return false;
	}
	return bRet;
}

bool mxStubLibrary(bool bLoad, const char *pStubMatlab, char *pThisFileVersion, char *pThisProductVersion)
{
	int iError=0;

	if(bLoad)
	{
		if(g_iLoadCount==DLL_COUNT)
			return true;
		if(!MultiByteToWideChar( CP_ACP , 0, (LPCSTR)pStubMatlab, -1, (LPWSTR)g_Local.wcStubMatlab, MAX_PATH))
			return false;
		if(!MultiByteToWideChar( CP_ACP , 0, (LPCSTR)pThisFileVersion, -1, (LPWSTR)g_Local.wcThisFileVersion, MAX_PATH))
			return false;
		if(!MultiByteToWideChar( CP_ACP , 0, (LPCSTR)pThisProductVersion, -1, (LPWSTR)g_Local.wcThisProductVersion, MAX_PATH))
			return false;
		memset(g_paVS_FIXEDFILEINFO,0,sizeof(g_paVS_FIXEDFILEINFO));
		memset(g_pFileVersion,0,sizeof(g_pFileVersion));
		memset(g_pProductVersion,0,sizeof(g_pProductVersion));
		memset(g_hModule,0,sizeof(g_hModule));
		_wsplitpath_s(g_Local.wcStubMatlab,g_Local.drive,_MAX_DRIVE,g_Local.dir,_MAX_DIR,g_Local.fnameIn,MAX_PATH,g_Local.ext,_MAX_EXT);

		for(int iDll=0;iDll<DLL_COUNT;iDll++)
		{
			int iDll2=DLL_COUNT-1-iDll;
			switch(iDll)
			{
			case 0: wcscpy_s(g_Local.fname,_MAX_FNAME,L"msvcr120");break;
			case 1: wcscpy_s(g_Local.fname,_MAX_FNAME,L"msvcp120");break;
			case 2: wcscpy_s(g_Local.fname,_MAX_FNAME,L"okFrontPanel");break;
			case 3: wcscpy_s(g_Local.fname,_MAX_FNAME,L"win_stub");break;
			case 4: wcscpy_s(g_Local.fname,_MAX_FNAME,L"UTCom");break;
			case 5: wcscpy_s(g_Local.fname,_MAX_FNAME,L"UTKernelDriver");break;
			case 6: wcscpy_s(g_Local.fname,_MAX_FNAME,L"UTKernelDriverOEMPA");break;
			case 7: wcscpy_s(g_Local.fname,_MAX_FNAME,L"UTCustomizedDriverAPI");break;
			case 8: wcscpy_s(g_Local.fname,_MAX_FNAME,g_Local.fnameIn);break;
			}
			if(!myLoadLibrary(g_Local.fname,g_Local.wcThisFileVersion,g_Local.wcThisProductVersion,
								g_pFileVersion[iDll2],g_pProductVersion[iDll2],&g_paVS_FIXEDFILEINFO[iDll2],
								&g_hModule[iDll2]))
				return false;
			g_iLoadCount++;
		}
		if(g_iLoadCount==DLL_COUNT)
		{
			//DRIVER MANAGEMENT
			DefineProcAddress(g_hModule[0],mxDelete,iError)
			DefineProcAddress(g_hModule[0],mxNew,iError)
			DefineProcAddress(g_hModule[0],mxAutoDisconnect,iError)
			DefineProcAddress(g_hModule[0],mxNewDevice,iError)
			DefineProcAddress(g_hModule[0],mxDeleteDevice,iError)
			DefineProcAddress(g_hModule[0],mxFindDevice,iError)
			DefineProcAddress(g_hModule[0],mxGetCfgKernelFile,iError)
			DefineProcAddress(g_hModule[0],mxSetCfgKernelFile,iError)
			DefineProcAddress(g_hModule[0],mxSaveRegistryString,iError)
			DefineProcAddress(g_hModule[0],mxGetFolderExe,iError)
			DefineProcAddress(g_hModule[0],mxReadFileWriteHW,iError)
			DefineProcAddress(g_hModule[0],mxReadHWWriteFile,iError)
			DefineProcAddress(g_hModule[0],mxConnect,iError)
			DefineProcAddress(g_hModule[0],mxIsConnected,iError)
			DefineProcAddress(g_hModule[0],mxIsUsb3Connected,iError)
			DefineProcAddress(g_hModule[0],mxIsFifo,iError)
			DefineProcAddress(g_hModule[0],mxIsFullMatrixCaptureReadWrite,iError)
			DefineProcAddress(g_hModule[0],mxResetCounters,iError)
			DefineProcAddress(g_hModule[0],mxResetEncoder,iError)
			DefineProcAddress(g_hModule[0],mxGetSerialNumber,iError)
			DefineProcAddress(g_hModule[0],mxGetStatus,iError)
			DefineProcAddress(g_hModule[0],mxEnableShot,iError)
			DefineProcAddress(g_hModule[0],mxEnableUsb3,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionStep,iError)
			DefineProcAddress(g_hModule[0],mxGetSWCycleCount,iError)
			DefineProcAddress(g_hModule[0],mxGetHWCycleCount,iError)
			DefineProcAddress(g_hModule[0],mxGetFullMatrixCapture,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionAscanCount,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionAscanEncoder,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionAscanSize,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionAscanBitSize,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionAscanData,iError)
			DefineProcAddress(g_hModule[0],mxDumpAcquisitionAscanFifo,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionAscanFifoStatus,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionAscanFifoIndex,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionAscanLifoIndex,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionAscanFifoInfo,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionAscanFifoData,iError)
			DefineProcAddress(g_hModule[0],mxFlushAcquisitionAscanFifo,iError)
			DefineProcAddress(g_hModule[0],mxFlushAcquisitionCscanFifo,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionCscanFifoStatus,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionCscanFifoIndex,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionCscanLifoIndex,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionCscanFifoData,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionCscanCount,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionCscanEncoder,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionCscanEnable,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionCscanAmplitude,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionCscanTimeOfFlight,iError)
			DefineProcAddress(g_hModule[0],mxSetGainAnalog,iError)
			DefineProcAddress(g_hModule[0],mxSetGainDigital,iError)
			DefineProcAddress(g_hModule[0],mxSetAscanRange,iError)
			DefineProcAddress(g_hModule[0],mxSetAscanRangeWithFactor,iError)
			DefineProcAddress(g_hModule[0],mxSetAscanStart,iError)
			DefineProcAddress(g_hModule[0],mxSetFilterIndex,iError)
			DefineProcAddress(g_hModule[0],mxSetFilter,iError)
			DefineProcAddress(g_hModule[0],mxLockDevice,iError)
			DefineProcAddress(g_hModule[0],mxUnlockDevice,iError)
            DefineProcAddress(g_hModule[0],mxGetLostCountAscan,iError)
            DefineProcAddress(g_hModule[0],mxGetLostCountCscan,iError)
            DefineProcAddress(g_hModule[0],mxGetLostCountEncoder,iError)
            DefineProcAddress(g_hModule[0],mxGetLostCountUSB3,iError)
			DefineProcAddress(g_hModule[0],mxGetRecoveryTimes,iError)
            DefineProcAddress(g_hModule[0],mxGetFMCTimeLimitation,iError)
            DefineProcAddress(g_hModule[0],mxSetFMCSubTimeSlot,iError)
            DefineProcAddress(g_hModule[0],mxSetTimeSlot,iError)
			DefineProcAddress(g_hModule[0],mxExample1,iError)
			DefineProcAddress(g_hModule[0],mxExample2,iError)
				
			DefineProcAddress(g_hModule[0],mxGetTraceGlobalFileName,iError)
			DefineProcAddress(g_hModule[0],mxEnableTraceGlobal,iError)
			DefineProcAddress(g_hModule[0],mxIsTraceGlobalEnabled,iError)

			DefineProcAddress(g_hModule[0],mxTrace,iError)
			DefineProcAddress(g_hModule[0],mxAllocArrayDouble,iError)
			//dwConvert = (DWORD)WideCharToMultiByte(CP_ACP, 0, (*g_pmxGetFolderExe)(), -1, pAux, MAX_PATH, NULL, NULL);
			//mexPrintf("path => '%s'\n",pAux);
			//mexPrintf("Init OK\n");

			//256:256
			DefineProcAddress(g_hModule[0],mxReadFileWriteHW256,iError)
			DefineProcAddress(g_hModule[0],mxEnableShot256,iError)
			DefineProcAddress(g_hModule[0],mxGetAcquisitionAscanFifoStatus256,iError)
			DefineProcAddress(g_hModule[0],mxLockDevice256,iError)
			DefineProcAddress(g_hModule[0],mxUnlockDevice256,iError)
			DefineProcAddress(g_hModule[0],mxResetCounters256,iError)

			//TOOLBOX MANAGEMENT
			DefineProcAddress(g_hModule[0],mxToolboxRun,iError)
			DefineProcAddress(g_hModule[0],mxIsToolboxRunning,iError)
			DefineProcAddress(g_hModule[0],mxToolboxVisible,iError)
			DefineProcAddress(g_hModule[0],mxEnableEventUpdate,iError)
			DefineProcAddress(g_hModule[0],mxToolboxClose,iError)
			DefineProcAddress(g_hModule[0],mxToolboxEcho,iError)
			DefineProcAddress(g_hModule[0],mxToolboxDisplay,iError)
			DefineProcAddress(g_hModule[0],mxToolboxWizardNew,iError)
			DefineProcAddress(g_hModule[0],mxToolboxWizardDelete,iError)
			DefineProcAddress(g_hModule[0],mxToolboxWizardUpdateScan,iError)
			DefineProcAddress(g_hModule[0],mxToolboxIsWizardEnabled,iError)
			DefineProcAddress(g_hModule[0],mxGetWizardBeamCount,iError)
			DefineProcAddress(g_hModule[0],mxGetWizardBeam,iError)
			DefineProcAddress(g_hModule[0],mxGetWizardElementCount,iError)
			DefineProcAddress(g_hModule[0],mxGetWizardElement,iError)
			DefineProcAddress(g_hModule[0],mxToolboxWizardReset,iError)
			DefineProcAddress(g_hModule[0],mxGetToolboxParameter,iError)
			DefineProcAddress(g_hModule[0],mxSetToolboxParameter,iError)
			DefineProcAddress(g_hModule[0],mxToolboxSaveSetup,iError)
			DefineProcAddress(g_hModule[0],mxToolboxSaveSetupEx,iError)
			DefineProcAddress(g_hModule[0],mxToolboxLoadWizard,iError)
			DefineProcAddress(g_hModule[0],mxToolboxSaveWizard,iError)
			DefineProcAddress(g_hModule[0],mxToolboxLoadMatrix,iError)
			DefineProcAddress(g_hModule[0],mxToolboxSaveMatrix,iError)
			DefineProcAddress(g_hModule[0],mxGetToolboxDeviceId,iError)
			DefineProcAddress(g_hModule[0],mxGetToolboxObjectId,iError)
			DefineProcAddress(g_hModule[0],mxSetToolboxDeviceAddress,iError)
			DefineProcAddress(g_hModule[0],mxToolboxDeviceConnection,iError)
			DefineProcAddress(g_hModule[0],mxToolboxIsDeviceConnected,iError)
			DefineProcAddress(g_hModule[0],mxGetToolboxDeviceType,iError)

			DefineProcAddress(g_hModule[0],mxTraceMemory,iError)

			mxMain.g_pExitDll = (pmxExit)GetProcAddress(g_hModule[0],"ExitDll");
			if(!mxMain.g_pExitDll)
			{
				iError++;
				mxMain.g_pmexPrintf("error function \"g_pExitDll\" unavailabled!\n");
			}
			if(!iError)
			{
				g_pmxNewDevice = mxMain.g_pmxNewDevice;
				mxMain.g_pmxNewDevice = mxNewDevice;
				g_pmxDeleteDevice = mxMain.g_pmxDeleteDevice;
				mxMain.g_pmxDeleteDevice = mxDeleteDevice;
				mxMain.g_pmxStackTest = mxStackTest;
				mxMain.g_pmxCheckCompilation = mxCheckCompilation;
			}
		}else
			return false;
#ifdef _DEBUG
	CrtCheckMemory();
#endif _DEBUG
	}else{
		if(!g_iLoadCount)
			return false;
#ifdef _DEBUG
		if(g_pBuffer2)
			mxMain.mxDelete(g_pBuffer2,__LINE__,__FILE__);
		if(g_pBuffer3)
			mxMain.mxDelete(g_pBuffer3,__LINE__,__FILE__);
#else _DEBUG
		if(g_pBuffer2)
			mxMain.mxDelete(g_pBuffer2);
		if(g_pBuffer3)
			mxMain.mxDelete(g_pBuffer3);
#endif _DEBUG
		g_iBufferSize2 = 0;
		g_iBufferSize3 = 0;
		ExitDll();
#ifdef _DEBUG
		CrtCheckMemory();
#endif _DEBUG

		for(int iDll=0;iDll<g_iLoadCount;iDll++)
		{
			if(g_hModule[iDll])
				FreeLibrary(g_hModule[iDll]);
			g_hModule[iDll] = 0;
		}
		g_iLoadCount = 0;
	}
	return true;
}

void Init()
{
	memset(&mxMain, 0, sizeof(mxMain));
	mxMain.g_pmxStubLibrary = mxStubLibrary;
	mxMain.g_pmxEnter = mxEnter;
	mxMain.g_pmxExit = mxExit;
	mxMain.g_pmxGeneralProcess = mxGeneralProcess;
	mxMain.g_pmxDeviceProcess = mxDeviceProcess;
	mxMain.g_pmxDevice256Process = mxDevice256Process;
}