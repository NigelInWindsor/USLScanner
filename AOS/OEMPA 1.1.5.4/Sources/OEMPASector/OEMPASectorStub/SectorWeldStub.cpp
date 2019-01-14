// testDemoSetting.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Resource.h"
#include "HWDlg.h"
#include "CustomizedDriverAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern wchar_t g_wcFolderData[];
extern wchar_t g_wcFolderDataCfg[];

pTypeCallbackAscan g_pCallbackAscan=NULL;
pTypeCallbackCscan g_pCallbackCscan=NULL;
pAllocDisplayCallback g_pAllocDisplayCallback=NULL;
CWnd *g_pMainFrame=NULL;

bool FileReadInteger(wchar_t *pSection,wchar_t *pKey,wchar_t *pFileName,int &iData);
bool FileReadDouble(wchar_t *pSection,wchar_t *pKey,wchar_t *pFileName,double dUnitFactor,double &dData,bool bDisplayMessage=true);

////////////////////////////////////////////////
__declspec(dllexport) bool DemoInit()
{
	return true;
}
__declspec(dllexport) bool DemoInit(int iSystemCount)
{
	return true;
}
__declspec(dllexport) bool DemoExit()
{
	return true;
}
__declspec(dllexport) bool DemoLoad(CString strFileBin)
{
	return true;
}
__declspec(dllexport) bool DemoLoad(int iSystem, CString strFileBin, CString strFileTxt)
{
	return true;
}
__declspec(dllexport) bool DemoReset()
{
	return true;
}
__declspec(dllexport) bool DemoRun(bool bStart, double dFactor, HWND hWndEnd)
{
	return true;
}
__declspec(dllexport) bool DemoDestroy()
{
	CHWDlg::Destroy();
	return true;
}

__declspec(dllexport) void SetDemoCallback(pTypeCallbackAscanOld pCallback)
{
}
__declspec(dllexport) void SetDemoCallback(pTypeCallbackAscan pCallbackAscan,pTypeCallbackCscan pCallbackCscan)
{
	g_pCallbackAscan = pCallbackAscan;
	g_pCallbackCscan = pCallbackCscan;
}
__declspec(dllexport) void SetAllocDisplay(pAllocDisplayCallback pCallback,CWnd* pMainFrame)
{
	g_pAllocDisplayCallback = pCallback;
	g_pMainFrame = pMainFrame;
}

bool IsSettingId(DWORD dwSettingId);
//extern int g_iEncoderResolution;
double g_dEncoder=0.0;
extern bool g_bExit;
__declspec(dllimport) long GetEncoderResolution();
bool g_bRawEncoder=false;
DWORD g_dwAscanSequenceLost=0;
extern double g_dEncoderStep,g_dEncoderLength;

double g_dEncoderLocalAscan=0.0;
UINT WINAPI CHWDlg::AcquisitionAscan1_0x00010103(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat)
{
//typedef bool (WINAPI* pTypeCallbackAscan)(int iSystem, int iCycle, double dEncoder, const char* pData, int iSize);
	long lResolution;
	int iCycle,iDataSize,iDataCount;

	if(g_bExit)
		return 0;
	if(g_pCallbackAscan && pBufferMax)
	{
#ifdef REJECT_SETTINGID
		//this test can be used to reject data that are not part of the curent setting id (EmuMon will not work with this test).
		if(!IsSettingId(pStreamHeader->settingId))
			return 0;
#endif REJECT_SETTINGID
		//use SW encoder values "acqInfo.dEncoder" (encoder resolution is already taken into account)...
		if(CHWDlg::m_bCheckInternal)
		{
			if(!pAscanHeader->cycle)
				g_dEncoderLocalAscan += g_dEncoderStep;
			if(g_dEncoderLocalAscan>g_dEncoderLength)
				g_dEncoderLocalAscan = 0.0;
			g_dEncoder = g_dEncoderLocalAscan;
		}else{
			if(!g_bRawEncoder)
			{
				g_dEncoder = acqInfo.dEncoder[0];
			}else{
				//but it is also possible to use raw encoder value as below... ("g_bRawEncoder=true;")
				lResolution = GetEncoderResolution();
				if(g_bRawEncoder && lResolution)
				{
					g_dEncoder = acqInfo.lEncoder[0];
					g_dEncoder /= lResolution;
					g_dEncoder *= 1.0e-3;
				}
			}
		}
		EnterCriticalSection(&m_CriticalSection);
		iCycle = pAscanHeader->cycle;
		if(iCycle<m_iAcquisitionCycleCount)
		{
			iDataCount = pAscanHeader->dataCount;
			iDataSize = pAscanHeader->dataSize;
			if((m_iAcquisitionDataCount[iCycle]!=iDataCount) || (m_iAcquisitionDataSize[iCycle]!=iDataSize) || !m_pAcquisitionDataMax[iCycle])
			{
				if(m_pAcquisitionDataMax[iCycle])
					delete m_pAcquisitionDataMax[iCycle];
				m_pAcquisitionDataMax[iCycle] = (WORD*) new char[iDataCount*iDataSize];
			}
			m_iAcquisitionDataCount[iCycle] = pAscanHeader->dataCount;
			m_iAcquisitionDataSize[iCycle] = pAscanHeader->dataSize;
			m_eAcquisitionDataBitSize[iCycle] = (enumBitSize)pAscanHeader->bitSize;
			m_bAcquisitionDataSigned[iCycle] = pAscanHeader->sign;
			if(m_pAcquisitionDataMax[iCycle])
				memcpy(m_pAcquisitionDataMax[iCycle],pBufferMax,iDataCount*iDataSize);
		}
		LeaveCriticalSection(&m_CriticalSection);
		//if(pAscanHeader->seqLost)
		//	g_dwAscanSequenceLost = GetTickCount();
		g_pCallbackAscan(0,(int)pAscanHeader->cycle,g_dEncoder,(const char*)pBufferMax,(int)pAscanHeader->dataCount);
	}
	return 0;
}

double g_dEncoderLocalCscan=0.0;
UINT WINAPI AcquisitionCscan1_0x00010X02(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamCscan_0x0X02 *pCscanHeader,const structCscanAmp_0x0102 *pBufferAmp,const structCscanAmpTof_0x0202 *pBufferAmpTof)
{
//typedef bool (WINAPI* pTypeCallbackAscan)(int iSystem, int iCycle, double dEncoder, const char* pData, int iSize);
	long lResolution;

	if(g_bExit)
		return 0;
	if(g_pCallbackCscan && (pBufferAmp || pBufferAmpTof))
	{
#ifdef REJECT_SETTINGID
		//this test can be used to reject data that are not part of the curent setting id (EmuMon will not work with this test).
		if(!IsSettingId(pStreamHeader->settingId))
			return 0;
#endif REJECT_SETTINGID
		//use SW encoder values "acqInfo.dEncoder" (encoder resolution is already taken into account)...
		if(CHWDlg::m_bCheckInternal)
		{
			if(!pCscanHeader->cycle)
				g_dEncoderLocalCscan += g_dEncoderStep;
			if(g_dEncoderLocalCscan>g_dEncoderLength)
				g_dEncoderLocalCscan = 0.0;
			g_dEncoder = g_dEncoderLocalCscan;
		}else{
			if(!g_bRawEncoder)
			{
				g_dEncoder = acqInfo.dEncoder[0];
			}else{
				//but it is also possible to use raw encoder value as below... ("g_bRawEncoder=true;")
				lResolution = GetEncoderResolution();
				if(g_bRawEncoder && lResolution)
				{
					g_dEncoder = acqInfo.lEncoder[0];
					g_dEncoder /= lResolution;
					g_dEncoder *= 1.0e-3;
				}
			}
		}
//#ifdef OLD_CSCAN_FORMAT
		//if(pBufferAmp)
			g_pCallbackCscan(0,(int)pCscanHeader->cycle,g_dEncoder,pBufferAmp,pBufferAmpTof,(int)pCscanHeader->count);
		//else if(pBufferAmpTof)
		//	g_pCallbackCscan(0,(int)pCscanHeader->cycle,g_dEncoder,pBufferAmp,pBufferAmpTof,(int)pCscanHeader->count/2);
//#endif OLD_CSCAN_FORMAT
	}
	return 0;
}

UINT WINAPI AcquisitionIO1_0x00010101(void *pAcquisitionParameter,const CStream_0x0001 *pStreamHeader,const CSubStreamIO_0x0101 *pIOHeader)
{
	if(pIOHeader->edges)
	{
		//digital inputs has been updated! => pIOHeader->inputs
		return 0;
	}
	return 1;
}

UINT WINAPI CHWDlg::AcquisitionAscan2_0x00010103(void *pAcquisitionParameter,structAcqInfoEx &acqInfo,const CStream_0x0001 *pStreamHeader,const CSubStreamAscan_0x0103 *pAscanHeader,const void *pBufferMax,const void *pBufferMin,const void *pBufferSat)
{
//typedef bool (WINAPI* pTypeCallbackAscan)(int iSystem, int iCycle, double dEncoder, const char* pData, int iSize);
	if(g_pCallbackAscan && pBufferMax)
	{
		g_pCallbackAscan(1,(int)pAscanHeader->cycle,g_dEncoder,(const char*)pBufferMax,(int)pAscanHeader->dataCount);
	}
	return 0;
}

void CHWDlg::DumpFileData(CStdioFile &file)
{
	CString string;
	char cValue;
	short sValue;
	int iAcquisitionDataCount;
	char *pAcquisitionData;
	short *psAcquisitionData;
	BYTE byValue;
	WORD wValue;

	for(int iCycle=0;iCycle<m_iAcquisitionCycleCount;iCycle++)
	{
		iAcquisitionDataCount = m_iAcquisitionDataCount[iCycle];
		string.Format(L"\n\nCycle %d Max (%d points)\n",iCycle,iAcquisitionDataCount);
		file.WriteString(string);
		pAcquisitionData = (char*)m_pAcquisitionDataMax[iCycle];
		psAcquisitionData = (short*)m_pAcquisitionDataMax[iCycle];
		for(int z=0;z<iAcquisitionDataCount;z++)
		{
			if(m_iAcquisitionDataSize[iCycle]==sizeof(BYTE))
			{
				cValue = (char)pAcquisitionData[z];
				if(m_bAcquisitionDataSigned[iCycle])
					string.Format(L"%d\n",(int)cValue);
				else{
					byValue = (BYTE)cValue;
					string.Format(L"%u\n",(unsigned int)byValue);
				}
			}else
			if(m_iAcquisitionDataSize[iCycle]==sizeof(WORD))
			{
				sValue = (short)psAcquisitionData[z];
				if(m_bAcquisitionDataSigned[iCycle])
					string.Format(L"%d\n",(int)sValue);
				else{
					wValue = (WORD)sValue;
					string.Format(L"%u\n",(unsigned int)wValue);
				}
			}
			file.WriteString(string);
		}
	}
}

int g_iEncoderResolution=1;
int g_iDisplayLastButOne=1;
int g_iAscanSequenceLostTimerMs=0;
extern UINT g_uiUTEventMessage;
double *g_pdAcqLength=NULL;
__declspec(dllexport) void DemoHWDialog(CWnd *pWnd,UINT &uiUTEventMessage,double &dPieceThickness,double &dAcqLength,bool &bDisplayLastButOne)
{
	wchar_t pFileName[MAX_PATH];

#ifdef _EVALUATION_
	CUTPreference *pPref=CUTPreference::GetCurrentPreference();
	wchar_t wcComp[MAX_PATH];
	if(pPref && pPref->GetCompilationOption(MAX_PATH,wcComp) && !wcsstr(wcComp,L"Evaluation"))
	{
		PostQuitMessage( 0 );
		return;
	}
#endif _EVALUATION_
	g_pdAcqLength = &dAcqLength;
	wcscpy_s(pFileName,MAX_PATH,g_wcFolderDataCfg);
	wcscat_s(pFileName,MAX_PATH,L"CfgOEMPASector.txt");
	dAcqLength = 100.0e-3;
	if(!FileReadDouble(L"Acquisition",L"Length",pFileName,1.0e-3,dAcqLength))
		AfxMessageBox(L"bad value for section \"Acquisition\" key \"Length\"!");
	dPieceThickness = 10.0e-3;
	if(!FileReadDouble(L"Acquisition",L"PieceThickness",pFileName,1.0e-3,dPieceThickness))
		AfxMessageBox(L"bad value for section \"Acquisition\" key \"PieceThickness\"!");
	CHWDlg::m_dEditThickness = dPieceThickness*1.0e3;
	CHWDlg::m_dEditLength = dAcqLength*1.0e3;
	CHWDlg::Display(pWnd);
	if(!FileReadInteger(L"Acquisition",L"DisplayLastButOne",pFileName,g_iDisplayLastButOne))
		AfxMessageBox(L"bad value for section \"Acquisition\" key \"DisplayLastButOne\"!");
	if(!FileReadInteger(L"Acquisition",L"AscanSequenceLostTimerMs",pFileName,g_iAscanSequenceLostTimerMs))
		AfxMessageBox(L"bad value for section \"Acquisition\" key \"AscanSequenceLostTimerMs\"!");
	if(g_iDisplayLastButOne)
		bDisplayLastButOne = true;
	else
		bDisplayLastButOne = false;
	uiUTEventMessage = g_uiUTEventMessage;
}

void WriteCfgSector()
{
	wchar_t pFileName[MAX_PATH];
	CString string;
	double dValue;

	wcscpy_s(pFileName,MAX_PATH,g_wcFolderDataCfg);
	wcscat_s(pFileName,MAX_PATH,L"CfgOEMPASector.txt");
	dValue = CHWDlg::m_dEditLength;
	string.Format(L"%f",dValue);
	WritePrivateProfileString(L"Acquisition",L"Length",string,pFileName);
	dValue = CHWDlg::m_dEditThickness;
	string.Format(L"%f",dValue);
	WritePrivateProfileString(L"Acquisition",L"PieceThickness",string,pFileName);
}
