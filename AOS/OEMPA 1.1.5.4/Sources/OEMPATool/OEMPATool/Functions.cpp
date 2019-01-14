#include "stdafx.h"

wchar_t g_pDefaultString2[]=L"&#@default@#&";

bool IsBinaryFile(const wchar_t *pFileName,int iSize,wchar_t *pDiffFileName)
{
	bool bTextFile;
	wchar_t drive[_MAX_DRIVE],dir[_MAX_DIR],fname[MAX_PATH],ext[_MAX_EXT];

	_wsplitpath_s(pFileName,drive,_MAX_DRIVE,dir,_MAX_DIR,fname,MAX_PATH,ext,_MAX_EXT);
	if(!wcscmp(ext,L".bin"))
	{
		bTextFile = false;
		swprintf(pDiffFileName,iSize,L"%s%s%s_diff.txt",drive,dir,fname);
		return true;
	}
	if(!wcscmp(ext,L".txt"))
	{
		bTextFile = true;
		return false;
	}
	return false;
}

void NotifyErrorFile(wchar_t *pSection,wchar_t *pKey)
{
	wchar_t msg[MAX_PATH];

	swprintf(msg,MAX_PATH,L"Error in section '%s' for key '%s'!",pSection,pKey);
	OempaApiMessageBox(NULL,msg,L"File Error",MB_OK);
}

bool FileReadInteger(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int &iData,bool bDisplayMsg=true,int iMin=INT_MIN,int iMax=INT_MAX);
bool FileReadInteger(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int &iData,bool bDisplayMsg,int iMin,int iMax)
{
	iData = GetPrivateProfileInt(pSection,pKey,-1,pFileName);
	if(iData==-1)
	{
		iData = GetPrivateProfileInt(pSection,pKey,0,pFileName);
		if(iData==0)
		{
			if(bDisplayMsg)
				NotifyErrorFile(pSection,pKey);
			return false;
		}
	}
	if((iData<iMin) || (iMax<iData))
	{
		if(bDisplayMsg)
		{
			wchar_t msg[MAX_PATH];
			swprintf(msg,MAX_PATH,L"Error in section '%s' for key '%s' out of range!",pSection,pKey);
			OempaApiMessageBox(NULL,msg,L"File Error",MB_OK);
		}
	}
	return true;
}

bool FileReadFloat(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,float fUnitFactor,float &fData,bool bDisplayMsg=true);
bool FileReadFloat(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,float fUnitFactor,float &fData,bool bDisplayMsg)
{
	wchar_t data[MAX_PATH];

	GetPrivateProfileString(pSection,pKey,g_pDefaultString2,data,MAX_PATH,pFileName);
	if(!wcscmp(data,g_pDefaultString2))
	{
		if(bDisplayMsg)
			NotifyErrorFile(pSection,pKey);
		return false;
	}
	if(!swscanf_s(data,L"%f",&fData,sizeof(fData)))
	{
		if(bDisplayMsg)
			NotifyErrorFile(pSection,pKey);
		return false;
	}
	fData *= fUnitFactor;
	return true;
}

bool AddArrayDouble(double dData,int iCountMax,double *pdData,int iCountExpected,int &iIndex)
{
	if(iIndex>=iCountMax)
		return false;
	if(iCountExpected && (iIndex>=iCountExpected))
		return false;
	pdData[iIndex++] = dData;
	return true;
}

wchar_t *ReadArrayDouble(wchar_t *pValue,double &dValue)
{
	wchar_t *pStop;

	dValue = _wtof(pValue);
	pStop = wcsstr(pValue,L";");
	if(pStop)
		pStop++;
	return pStop;
}

wchar_t* GetEnumNext(wchar_t *pEnum)
{
	wchar_t *pValue;
	int iLength;

	pValue = pEnum;
	iLength = (int)wcslen(pValue);
	if(!pValue[iLength+1])
		return NULL;
	return &pValue[iLength+1];
}

bool FileReadDouble(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,double dUnitFactor,double &dData,bool bDisplayMsg=true);
bool FileReadDouble(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,double dUnitFactor,double &dData,bool bDisplayMsg)
{
	wchar_t data[MAX_PATH];

	GetPrivateProfileString(pSection,pKey,g_pDefaultString2,data,MAX_PATH,pFileName);
	if(!wcscmp(data,g_pDefaultString2))
	{
		if(bDisplayMsg)
			NotifyErrorFile(pSection,pKey);
		return false;
	}
	dData = _wtof(data);
	dData *= dUnitFactor;
	return true;
}

bool FileReadString(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *wcData,int iSize,bool bDisplayMsg=true);
bool FileReadString(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *wcData,int iSize,bool bDisplayMsg)
{
	wchar_t data[MAX_PATH];

	GetPrivateProfileString(pSection,pKey,g_pDefaultString2,data,MAX_PATH,pFileName);
	if(!wcscmp(data,g_pDefaultString2))
		return false;
	wcscpy_s(wcData,iSize,data);
	return true;
}

bool FileReadEnum(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *pEnumList,int &iEnum,bool bDisplayErrorMsg=true);
bool FileReadEnum(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *pEnumList,int &iEnum,bool bDisplayErrorMsg)
{
	wchar_t *pValue;
	bool bFound=false;
	wchar_t wcAux[MAX_PATH];

	if(!FileReadString(pSection,pKey,pFileName,wcAux,MAX_PATH))
		return false;
	pValue = pEnumList;
	iEnum = 0;
	while(pValue)
	{
		if(!wcscmp(wcAux,pValue))
		{
			bFound = true;
			break;
		}
		iEnum++;
		pValue = GetEnumNext(pValue);
	}
	if(!bFound)
	{
		NotifyErrorFile(pSection,pKey);
		wcscpy_s(wcAux,MAX_PATH,L"");
		iEnum = 0;
		pValue = pEnumList;
		do{
			wcscat_s(wcAux,MAX_PATH,L"\'");
			wcscat_s(wcAux,MAX_PATH,pValue);
			wcscat_s(wcAux,MAX_PATH,L"\' ");
			wcslen(pValue);
			pValue += wcslen(pValue)+1;
			iEnum++;
		}while(pValue[0]);
		if(iEnum && bDisplayErrorMsg)
			OempaApiMessageBox(NULL,wcAux,L"Available values",MB_OK);
		return false;
	}
	return true;
}
