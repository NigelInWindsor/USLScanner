#include "stdafx.h"
#include <iostream>
#include "CRead_Write_ParametersList.h"
using namespace std;

wchar_t g_pDefaultString2[]=L"&#@default@#&";
int g_iTextFileMaxSingleLineDataCountForLong=8*2;
int g_iTextFileMaxSingleLineDataCountForFloat=16;
int g_iTextFileLineDataCountForLong=8;
int g_iTextFileLineDataCountForFloat=16;

	myCKey::myCKey()
	{
		m_wcName[0] = 0;
		m_wcValue = NULL;
		m_iValueSize = 0;
	}
	myCKey::myCKey(const myCKey &a)
	{
		m_wcName[0] = 0;
		m_wcValue = NULL;
		m_iValueSize = 0;
		*this = a;
	}

	myCKey::~myCKey()
	{
		Desalloc();//ljr
	}

	void myCKey::Desalloc()
	{
		if(m_wcValue)
			delete m_wcValue;
		m_wcValue = NULL;
		m_iValueSize = 0;
	}
	void myCKey::Reset()
	{
		m_wcValue = NULL;
		m_iValueSize = 0;
	}
	bool myCKey::operator<(const myCKey& rhs)
	{
		int b = wmemcmp ( m_wcName, rhs.m_wcName, g_iKeySize );
		if (b<0)
			return true;
		else
			return false;
	}
	myCKey& myCKey::operator=(const myCKey& rhs)
	{
		wcscpy_s(m_wcName, g_iKeySize, rhs.m_wcName);
		m_wcValue = rhs.m_wcValue;
		m_iValueSize = rhs.m_iValueSize;
		return *this;
	}
	myCKey& myCKey::Copy(const myCKey& rhs)
	{
		Desalloc();
		wcscpy_s(m_wcName, g_iKeySize, rhs.m_wcName);
		if(rhs.m_iValueSize>0)
		{
			m_wcValue = new wchar_t[rhs.m_iValueSize];
			wcscpy_s(m_wcValue, m_iValueSize, rhs.m_wcValue);
		}
		return *this;
	}
	bool myCKey::SetLine(std::wstring &line)
	{
		wchar_t* wCharValue = const_cast<wchar_t*>( line.c_str() );
 		return SetRString(wCharValue);
	}
	bool myCKey::SetRString(wchar_t *wcValue)
	{
		int iIndex, iLength;

		if(wcsstr(wcValue,L"="))
		{
			iIndex = 0;
			while(wcValue[iIndex]!=L'=')
				m_wcName[iIndex++] = wcValue[iIndex];
			m_wcName[iIndex] = 0;
			iLength = (int)wcslen(&wcValue[iIndex+1])+1;
			if(m_iValueSize && (m_iValueSize<iLength))
				Desalloc();
			if(!m_wcValue)
			{
				m_iValueSize = iLength;
				m_wcValue = new wchar_t[m_iValueSize];
			}
			wcscpy_s(m_wcValue,m_iValueSize,&wcValue[iIndex+1]);
			return true;
		}
		return false;
	};

	bool myCKey::SetWString(wchar_t *wcName,const wchar_t *wcValue)
	{
		int iLength;

		if(wcslen(wcName)>=g_iKeySize)
		{
			UTKernel_SystemMessageBoxList(L"ERROR: myCKey::SetWString");
			return false;
		}
		wcscpy_s(m_wcName,g_iKeySize,wcName);
		iLength = (int)wcslen(wcValue)+1;
		if(m_iValueSize && (m_iValueSize<iLength))
			Desalloc();
		if(!m_wcValue)
		{
			m_iValueSize = iLength;
			m_wcValue = new wchar_t[m_iValueSize];
		}
		wcscpy_s(m_wcValue,m_iValueSize,wcValue);

		return true;
	}

	myCSection::myCSection()
	{
		m_wcName[0] = 0;
	}
	myCSection::myCSection(const myCSection& a)
	{
		m_wcName[0] = 0;
		*this = a;
	}
	myCSection::~myCSection()
	{
		Desalloc();
	}
	void myCSection::Desalloc()
	{
		for(std::list<myCKey>::iterator itKey=m_ListKey.begin();itKey!=m_ListKey.end();itKey++)
		{
			itKey->Desalloc();
		}
		m_ListKey.clear();
	}
	void myCSection::Reset()
	{
		for(std::list<myCKey>::iterator itKey=m_ListKey.begin();itKey!=m_ListKey.end();itKey++)
		{
			itKey->Reset();
		}
	}
	bool myCSection::operator<(const myCSection& rhs)
	{
		int b = wmemcmp (m_wcName, rhs.m_wcName, g_iSectionSize);
		if (b<0)
			return true;
		else
			return false;
	}
	myCSection& myCSection::operator=(const myCSection& rhs)
	{
		m_ListKey.clear();
		wcscpy_s(m_wcName, g_iSectionSize, rhs.m_wcName);
		for(std::list<myCKey>::const_iterator itKey=rhs.m_ListKey.begin();itKey!=rhs.m_ListKey.end();itKey++)
		{
			m_ListKey.push_back(*itKey);
		}
		return *this;
	}
	myCSection& myCSection::Copy(myCSection& rhs)
	{
		myCKey key;

		m_ListKey.clear();
		wcscpy_s(m_wcName, g_iSectionSize, rhs.m_wcName);
		for(std::list<myCKey>::iterator itKey=rhs.m_ListKey.begin();itKey!=rhs.m_ListKey.end();itKey++)
		{
			key.Copy(*itKey);
			m_ListKey.push_back(key);
		}
		return *this;
	}

	bool myCSection::SetLine(std::wstring &line)
	{
		wchar_t* wCharValue = const_cast<wchar_t*>( line.c_str() );
		return SetRString(wCharValue);
	}
	bool myCSection::SetRString(wchar_t *wcValue)
	{
		int iIndex=0;

		if(wcslen(wcValue)>=g_iSectionSize)
		{
			UTKernel_SystemMessageBoxList(L"ERROR: myCSection::SetRString");
			return false;
		}
		if(wcsstr(wcValue,L"[") && wcsstr(wcValue,L"]"))
		{
			wcscpy_s(m_wcName,g_iSectionSize,wcValue+1);
			m_wcName[wcslen(m_wcName)-1] = 0;
			return true;
		}
		return false;
	}

	bool myCSection::SetWString(wchar_t *wcValue)
	{
		if(wcslen(wcValue)>=g_iSectionSize)
		{
			UTKernel_SystemMessageBoxList(L"ERROR: myCSection::SetWString");
			return false;
		}
		wcscpy_s(m_wcName,g_iSectionSize,wcValue);
		return true;
	}

///////////////////////////////////////////////////////////////////////

CRead_Write_ParametersList::CRead_Write_ParametersList() 
{
	int fastReading2;
	wchar_t pFileName[MAX_PATH];

	memset(m_wcWriteFile,0,MAX_PATH*sizeof(wchar_t));
	wcscpy_s(pFileName,MAX_PATH,UTKernel_GetFolderDataCfg());
	wcscat_s(pFileName,MAX_PATH,L"CfgKernel.txt");
	wcscpy_s(pFileName,MAX_PATH,UTKernel_GetConfigurationFile());
	//std::wifstream stream(L"C:\\ProgramData\\AOS\\OEMPA 1.1.5.2\\Cfg\\CfgKernel.txt");
	//wstring line;
	//myCKey myLine;
	//fastReading=-1;
	//fastWriting=-1;

	//while(std::getline(stream,line) && fastReading!=0 && fastWriting!=0)
	//{
	//	int foundFastRead = (int)line.find(L"FastReading");
	//	int foundFastWrite = (int)line.find(L"FastWriting");
	//	if(foundFastRead>=0)
	//	{
	//		myLine.SetLine(line);
	//		fastReading=_wtoi(myLine.m_wcValue);
	//	}

	//	if(foundFastWrite>=0)
	//	{
	//		myLine.SetLine(line);
	//		fastWriting=_wtoi(myLine.m_wcValue);
	//	}
	//}
	int fastReadingDefault=0;
	int fastWritingDefault=0;
	fastReading = 0;//required to read the cfg kernel with low speed feature.
	fastWriting = 0;//required to write the cfg kernel with low speed feature.
	if(!FileReadInteger(L"CustomizedAPI",L"FastReading",pFileName,fastReading2,false,0,1,fastReadingDefault))
		FileWriteInteger(L"CustomizedAPI",L"FastReading",pFileName,fastReadingDefault,L"");
	if(!FileReadInteger(L"CustomizedAPI",L"FastWriting",pFileName,fastWriting,false,0,1,fastWritingDefault))
		FileWriteInteger(L"CustomizedAPI",L"FastWriting",pFileName,fastWritingDefault,L"");
	fastReading = fastReading2;
}

bool CRead_Write_ParametersList::IsComment(std::wstring &line)
{
	if(!line.find(L"#"))
		return true;
	if(!line.find(L";"))
		return true;
	if(!line.find(L"//"))
		return true;
	return false;
}
void CRead_Write_ParametersList::ReadTxtCreateList(const wchar_t* lpFileName)
{
	myCKey myCurrentKey;
	myCSection myCurrentSection;
	std::wifstream stream(lpFileName);
	std::wstring line;
	std::list<myCSection> *listSection=&m_ListSection;
	int iSectionCount=0;
	bool bNewSection;

	while(std::getline(stream,line))
	{
		if(IsComment(line))
			continue;
		if(!line.find(L"["))
			bNewSection = true;
		else
			bNewSection = false;
		if(bNewSection)
		{
			if (iSectionCount>0)
			{//means storage of listLine in listSection
				myCurrentSection.m_ListKey.sort();
				listSection->push_back(myCurrentSection);
				myCurrentSection.Reset();//ljr because the pointer has been moved during "push_back(myCurrentSection)".
				myCurrentSection.m_ListKey.clear();
			}
			myCurrentSection.SetLine(line);
			iSectionCount++;
		}else
		if(line.find(L"=")!=-1)
		{//new key
			myCurrentKey.SetLine(line);
			myCurrentSection.m_ListKey.push_back(myCurrentKey);
			myCurrentKey.Reset();//ljr because the pointer has been moved during "push_back(myCurrentKey)".
		}
	}
	stream.close();

	myCurrentSection.m_ListKey.sort();
	listSection->push_back(myCurrentSection);
	myCurrentSection.Reset();//ljr because the pointer has been moved during "push_back(myCurrentSection)".
	listSection->sort();

	for(list<myCSection>::iterator p2 = listSection->begin();p2 != listSection->end();p2++)
	{
		m_ListSectionName.push_back(&*p2);
	}

}

//////////////////////////////////////////////////////////////////////////////

bool CRead_Write_ParametersList::safeFileReadString(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *pString,int iSize)
{
	wchar_t data[MAX_PATH];
	bool bRet=true;
	if(fastReading==0){
		::GetPrivateProfileString(pSection,pKey,g_pDefaultString2,data,MAX_PATH,pFileName);
	}else{
		GetPrivateProfileString(pSection,pKey,g_pDefaultString2,data,MAX_PATH,pFileName);
	}
	if(!wcscmp(data,g_pDefaultString2))
	{
		NotifyErrorFile(pSection,pKey);
		return false;
	}
	if(iSize>MAX_PATH)
	{
		if(wcslen(data)>=MAX_PATH-1)
			bRet = false;
	}else{
		if((int)wcslen(data)>=iSize)
		{
			data[iSize] = 0;
			bRet = false;
		}
	}
	wcscpy_s(pString,iSize,data);
	return true;
}

bool CRead_Write_ParametersList::FileReadInteger(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName, int &iData,bool bDisplayMsg,int iMin,int iMax,int iDefault)
{
	if(bDisplayMsg && ((iDefault<iMin) || (iMax<iDefault)))
	{
		wchar_t msg[MAX_PATH];
		swprintf(msg,MAX_PATH,L"Error default value is out of range (section '%s' key '%s')!",pSection,pKey);
		OempaApiMessageBox(NULL,msg,L"File Error",MB_OK);
		return false;
	}
	if(fastReading==0)
	{
		iData = ::GetPrivateProfileInt(pSection,pKey,-1,pFileName);
		if(iData==-1)
		{
			iData = ::GetPrivateProfileInt(pSection,pKey,0,pFileName);
			if(iData==0)
			{
				iData = iDefault;
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
			return false;
		}
		return true;
	}else{
		//ljr iData = ::GetPrivateProfileInt(pSection,pKey,-1,pFileName);
		iData = GetPrivateProfileInt(pSection,pKey,-1,pFileName);
		if(iData==-1)
		{
			iData = GetPrivateProfileInt(pSection,pKey,0,pFileName);
			if(iData==0)
			{
				iData = iDefault;
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
			return false;
		}
		return true;
	}
}

bool CRead_Write_ParametersList::FileReadLong(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,long &lData,bool bDisplayMsg)
{
	if(fastReading==0)
	{
		lData = (long)::GetPrivateProfileInt(pSection,pKey,-1,pFileName);
		if(lData==-1)
		{
			lData = (long)::GetPrivateProfileInt(pSection,pKey,0,pFileName);
			if(lData==0)
			{
				if(bDisplayMsg)
					NotifyErrorFile(pSection,pKey);
				return false;
			}
		}
		return true;
	}else{
		lData = (long) GetPrivateProfileInt(pSection,pKey,-1,pFileName);
		if(lData==-1)
		{
			lData = (long) GetPrivateProfileInt(pSection,pKey,0,pFileName);
			if(lData==0)
			{
				if(bDisplayMsg)
					NotifyErrorFile(pSection,pKey);
				return false;
			}
		}
		return true;
	}
}

bool CRead_Write_ParametersList::FileReadULong(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,unsigned long &dwData,bool bDisplayMsg)
{
	if(fastReading==0)
	{
		dwData = (long)::GetPrivateProfileInt(pSection,pKey,-1,pFileName);
		if(dwData==-1)
		{
			dwData = (long)::GetPrivateProfileInt(pSection,pKey,0,pFileName);
			if(dwData==0)
			{
				if(bDisplayMsg)
					NotifyErrorFile(pSection,pKey);
				return false;
			}
		}
		return true;

	}else{
		dwData = (long) GetPrivateProfileInt(pSection,pKey,-1,pFileName);
		if(dwData==-1)
		{
			dwData = (long) GetPrivateProfileInt(pSection,pKey,0,pFileName);
			if(dwData==0)
			{
				if(bDisplayMsg)
					NotifyErrorFile(pSection,pKey);
				return false;
			}
		}
		return true;
	}
}

bool CRead_Write_ParametersList::FileReadBoolean(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,bool &bData,bool bDisplayMsg)
{
	int iData;
	if(fastReading==0)
	{
		iData = ::GetPrivateProfileInt(pSection,pKey,-1,pFileName);
	}else{
		iData = GetPrivateProfileInt(pSection,pKey,-1,pFileName);
	}
	if(iData==-1)
	{
		if(bDisplayMsg)
			NotifyErrorFile(pSection,pKey);
		return false;
	}
	if(iData)
		bData = true;
	else
		bData = false;
	return true;
}

bool  CRead_Write_ParametersList::FileReadDouble(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,double dUnitFactor,double &dData,bool bDisplayMsg)
{
	wchar_t data[MAX_PATH];

	if(fastReading==0){
		::GetPrivateProfileString(pSection,pKey,g_pDefaultString2,data,MAX_PATH,pFileName);
	}else{
		GetPrivateProfileString(pSection,pKey,g_pDefaultString2,data,MAX_PATH,pFileName);
	}

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

bool CRead_Write_ParametersList::FileReadFloat(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,float fUnitFactor,float &fData,bool bDisplayMsg)
{
	wchar_t data[MAX_PATH];
	if(fastReading==0)
	{
		::GetPrivateProfileString(pSection,pKey,g_pDefaultString2,data,MAX_PATH,pFileName);
	}else{
		GetPrivateProfileString(pSection,pKey,g_pDefaultString2,data,MAX_PATH,pFileName);
	}
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

bool CRead_Write_ParametersList::FileReadString(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *wcData,int iSize,bool bDisplayMsg)
{
	wchar_t data[MAX_PATH];
	if (fastReading==0)
	{
		::GetPrivateProfileString(pSection,pKey,g_pDefaultString2,data,MAX_PATH,pFileName);
	}else{
		GetPrivateProfileString(pSection,pKey,g_pDefaultString2,data,MAX_PATH,pFileName);
	}
	if(!wcscmp(data,g_pDefaultString2))
		return false;
	wcscpy_s(wcData,iSize,data);
	return true;
}

bool CRead_Write_ParametersList::FileReadWord(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,WORD &wData,bool bDisplayMsg)
{
	wchar_t wcData[MAX_PATH/8],*pData;
	int tmp,iIndex;

	if(!FileReadString(pSection,pKey,pFileName,wcData,MAX_PATH/8))
		return false;
	pData = wcsstr(wcData,L"0x");
	if(!pData)
		return false;
	pData += 2;
	iIndex = 0;
	wData = 0;
	while(GetHex(pData[iIndex],tmp))
	{
		wData = wData<<4;
		wData += tmp;
		iIndex++;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////
wchar_t* CRead_Write_ParametersList::GetEnumNext(wchar_t *pEnum)
{
	wchar_t *pValue;
	int iLength;

	pValue = pEnum;
	iLength = (int)wcslen(pValue);
	if(!pValue[iLength+1])
		return NULL;
	return &pValue[iLength+1];
}

bool CRead_Write_ParametersList::GetHex(wchar_t c,int &tmp)
{
	tmp = 0;
	if ( (c >= L'0') && (c <= L'9'))
		tmp = c - L'0';
	else if ( (c >= L'A') && (c <= L'F'))
		tmp = c - L'A' + 10;
	else if ( (c >= L'a') && (c <= L'f'))
		tmp = c - L'a' + 10;
	else
		return false;
	return true;
}

bool CRead_Write_ParametersList::FileReadEnum(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *pEnumList,int &iEnum, bool bDisplayErrorMsg)
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

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

wchar_t* CRead_Write_ParametersList::ReadArrayLongLong(bool bDecimal,wchar_t *pValue,long long &llValue)
{
	//wchar_t separator=L';';
	int iIndex;
	bool bFound=false;
	wchar_t *pStop=pValue;
	wchar_t c;
	long long powerFactor = 1;
	long long tmp;
	bool bSign=false;
	
	pStop = wcsstr(pValue,L"0x");
	if(!pStop)
	{
		if(!bDecimal)
			return NULL;
		pStop = pValue;
		if(*pStop==L'-')
		{
			bSign = true;
			pStop++;
		}
	}else{
		bDecimal = false;
		pStop += 2;
	}
	iIndex = 0;
	llValue = 0;
	while(1)
	{
		c = *pStop;//pValue[i];
		if(!c)
			break;
		if ( (c >= L'0') && (c <= L'9'))
		{
			tmp = c - L'0';
			pStop++;
		}else if ( bDecimal && (c >= L'A') && (c <= L'F'))
		{
			tmp = c - L'A' + 10;
			pStop++;
		}else if ( bDecimal && (c >= L'a') && (c <= L'f'))
		{
			tmp = c - L'a' + 10;
			pStop++;
		}else{
			if(c==L';')
				break;
			return NULL;
		}
		if(!bDecimal)
			llValue = llValue<<4;
		else
			llValue *= 10;
		llValue += tmp;
	}
	if(bSign)
		llValue = -llValue;
	return pStop;
}

wchar_t* CRead_Write_ParametersList::ReadArrayFloat(wchar_t *pValue,float &fValue)
{
	double dValue;
	wchar_t *pStop;

	dValue = _wtof(pValue);
	fValue = (float)dValue;
	pStop = wcsstr(pValue,L";");
	if(pStop)
		pStop++;
	return pStop;
}

wchar_t* CRead_Write_ParametersList::ReadArrayDouble(wchar_t *pValue,double &dValue)
{
	wchar_t *pStop;

	dValue = _wtof(pValue);
	pStop = wcsstr(pValue,L";");
	if(pStop)
		pStop++;
	return pStop;
}

bool CRead_Write_ParametersList::FileReadArrayShort(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iCountMax,short *pwData,int &iCountExpected)
{
	wchar_t pKeyBigData[MAX_PATH];
	wchar_t line[MAX_PATH];
	short wData;
	int iCount;
	bool bKeyBigData=false;
	wchar_t *pValue;
	long long llValue;
	DWORD dwCount;

	iCount = 0;
	swprintf(pKeyBigData,MAX_PATH,L"%s_%d",pKey,iCount);
	while(true)
	{
		if(fastReading==0)
		{
			dwCount = ::GetPrivateProfileString(pSection,pKeyBigData,g_pDefaultString2,line,MAX_PATH,pFileName);
		}else{
			dwCount = GetPrivateProfileString(pSection,pKeyBigData,g_pDefaultString2,line,MAX_PATH,pFileName);
		}
		if(dwCount==MAX_PATH-1)
		{
			swprintf(pKeyBigData,MAX_PATH,L"Section \"%s\" key \" line overflow (maximum line size is %d)!",MAX_PATH-1);
			OempaApiMessageBox(NULL,pKeyBigData,L"File Error",MB_OK);
			return false;
		}
		if(!wcscmp(line,g_pDefaultString2))
		{
			if(!iCount)
			{
				if(!wcscmp(line,pKey))
				{
					NotifyErrorFile(pSection,pKey);
					iCountExpected = iCount;
					return false;
				}
				if(!wcscmp(pKeyBigData,pKey))
				{
					iCountExpected = iCount;
					return false;
				}
				wcscpy_s(pKeyBigData,MAX_PATH,pKey);
				continue;
			}
			if(iCountExpected && (iCountExpected!=iCount))
			{
				swprintf(line,MAX_PATH,L"Section \"%s\" key \"%s\" count should be %d not %d.",pSection,pKey,iCountExpected,iCount);
				OempaApiMessageBox(NULL,line,L"File Error",MB_OK);
				iCountExpected = iCount;
				return false;
			}
			if(!iCountExpected)
				iCountExpected = iCount;
			if(iCount)
				return true;
			return false;
		}
		pValue = ReadArrayLongLong(true,line,llValue);
		wData = (short)llValue;
		if(*pValue)
			pValue++;
		//while(AddArrayShort(wData,iCountMax,pwData,iCountExpected,iCount) && *pValue)
		while(true)
		{
			AddArrayShort(wData,iCountMax,pwData,iCountExpected,iCount);
			if(!pValue || !*pValue)
				break;
			pValue = ReadArrayLongLong(true,pValue,llValue);
			if(*pValue)
				pValue++;
			wData = (int)llValue;
		}
		if(!wcscmp(pKeyBigData,pKey))
		{
			if(iCountExpected && (iCountExpected!=iCount))
			{
				swprintf(line,MAX_PATH,L"Section \"%s\" key \"%s\" count should be %d not %d.",pSection,pKey,iCountExpected,iCount);
				OempaApiMessageBox(NULL,line,L"File Error",MB_OK);
				iCountExpected = iCount;
				return false;
			}
			return true;
		}
		swprintf(pKeyBigData,MAX_PATH,L"%s_%d",pKey,iCount);
	}
	return false;
}

bool CRead_Write_ParametersList::FileReadArrayInteger(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iCountMax,int *piData,int &iCountExpected)
{
	wchar_t pKeyBigData[MAX_PATH];
	wchar_t line[MAX_PATH];
	int iData,iCount;
	bool bKeyBigData=false;
	wchar_t *pValue;
	long long llValue;
	DWORD dwCount;

	iCount = 0;
	swprintf(pKeyBigData,MAX_PATH,L"%s_%d",pKey,iCount);
	while(true)
	{
		if(fastReading==0)
		{
			dwCount = ::GetPrivateProfileString(pSection,pKeyBigData,g_pDefaultString2,line,MAX_PATH,pFileName);
		}else{
			dwCount = GetPrivateProfileString(pSection,pKeyBigData,g_pDefaultString2,line,MAX_PATH,pFileName);
		}
		if(dwCount==MAX_PATH-1)
		{
			swprintf(pKeyBigData,MAX_PATH,L"Section \"%s\" key \"%s\" line overflow (maximum line size is %d)!",pSection,pKey,MAX_PATH-1);
			OempaApiMessageBox(NULL,pKeyBigData,L"File Error",MB_OK);
			return false;
		}
		if(!wcscmp(line,g_pDefaultString2))
		{
			if(!iCount)
			{
				if(!wcscmp(line,pKey))
				{
					NotifyErrorFile(pSection,pKey);
					iCountExpected = iCount;
					return false;
				}
				if(!wcscmp(pKeyBigData,pKey))
				{
					iCountExpected = iCount;
					return false;
				}
				wcscpy_s(pKeyBigData,MAX_PATH,pKey);
				continue;
			}
			if(iCountExpected && (iCountExpected!=iCount))
			{
				swprintf(line,MAX_PATH,L"Section \"%s\" key \"%s\" count should be %d not %d.",pSection,pKey,iCountExpected,iCount);
				OempaApiMessageBox(NULL,line,L"File Error",MB_OK);
				iCountExpected = iCount;
				return false;
			}
			if(!iCountExpected)
				iCountExpected = iCount;
			if(iCount)
				return true;
			return false;
		}
		pValue = ReadArrayLongLong(true,line,llValue);
		iData = (int)llValue;
		if(*pValue)
			pValue++;
		while(true)
		{
			AddArrayInteger(iData,iCountMax,piData,iCountExpected,iCount);
			if(!pValue || !*pValue)
				break;
			pValue = ReadArrayLongLong(true,pValue,llValue);
			if(*pValue)
				pValue++;
			iData = (int)llValue;
		}
		if(!wcscmp(pKeyBigData,pKey))
		{
			if(iCountExpected && (iCountExpected!=iCount))
			{
				swprintf(line,MAX_PATH,L"Section \"%s\" key \"%s\" count should be %d not %d.",pSection,pKey,iCountExpected,iCount);
				OempaApiMessageBox(NULL,line,L"File Error",MB_OK);
				iCountExpected = iCount;
				return false;
			}
			return true;
		}
		swprintf(pKeyBigData,MAX_PATH,L"%s_%d",pKey,iCount);
	}
	return false;
}

bool CRead_Write_ParametersList::FileReadArrayFloat(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iCountMax,float fUnitFactor,float *pfData,int &iCountExpected)
{
	wchar_t pKeyBigData[MAX_PATH];
	wchar_t line[MAX_PATH];
	int iCount;
	bool bKeyBigData=false;
	wchar_t *pValue;
	float fData;
	DWORD dwCount;

	iCount = 0;
	swprintf(pKeyBigData,MAX_PATH,L"%s_%d",pKey,iCount);
	while(true)
	{
		if(fastReading==0)
		{
			dwCount = ::GetPrivateProfileString(pSection,pKeyBigData,g_pDefaultString2,line,MAX_PATH,pFileName);
		}else{
			dwCount = GetPrivateProfileString(pSection,pKeyBigData,g_pDefaultString2,line,MAX_PATH,pFileName);
		}
		if(dwCount==MAX_PATH-1)
		{
			swprintf(pKeyBigData,MAX_PATH,L"Section \"%s\" key \"%s\" line overflow (maximum line size is %d)!",pSection,pKey,MAX_PATH-1);
			OempaApiMessageBox(NULL,pKeyBigData,L"File Error",MB_OK);
			return false;
		}
		if(!wcscmp(line,g_pDefaultString2))
		{
			if(!iCount)
			{
				if(!wcscmp(line,pKey))
				{
					NotifyErrorFile(pSection,pKey);
					iCountExpected = iCount;
					return false;
				}
				if(!wcscmp(pKeyBigData,pKey))
				{
					iCountExpected = iCount;
					return false;
				}
				wcscpy_s(pKeyBigData,MAX_PATH,pKey);
				continue;
			}
			if(iCountExpected && (iCountExpected!=iCount))
			{
				swprintf(line,MAX_PATH,L"Section \"%s\" key \"%s\" count should be %d not %d.",pSection,pKey,iCountExpected,iCount);
				OempaApiMessageBox(NULL,line,L"File Error",MB_OK);
				iCountExpected = iCount;
				return false;
			}
			if(!iCountExpected)
				iCountExpected = iCount;
			if(iCount)
				return true;
			return false;
		}
		pValue = ReadArrayFloat(line,fData);
		fData *= fUnitFactor;
		//while(AddArrayFloat(fData,iCountMax,pfData,iCountExpected,iCount) && pValue)
		while(true)
		{
			AddArrayFloat(fData,iCountMax,pfData,iCountExpected,iCount);
			if(!pValue || !*pValue)
				break;
			pValue = ReadArrayFloat(pValue,fData);
			fData *= fUnitFactor;
		}
		if(!wcscmp(pKeyBigData,pKey))
		{
			if(iCountExpected && (iCountExpected!=iCount))
			{
				swprintf(line,MAX_PATH,L"Section \"%s\" key \"%s\" count should be %d not %d.",pSection,pKey,iCountExpected,iCount);
				OempaApiMessageBox(NULL,line,L"File Error",MB_OK);
				iCountExpected = iCount;
				return false;
			}
			return true;
		}
		swprintf(pKeyBigData,MAX_PATH,L"%s_%d",pKey,iCount);
	}
	return false;
}

bool CRead_Write_ParametersList::FileReadArrayDouble(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iCountMax,double dUnitFactor,double *pdData,int &iCountExpected)
{
	wchar_t pKeyBigData[MAX_PATH];
	wchar_t line[MAX_PATH];
	int iCount;
	bool bKeyBigData=false;
	wchar_t *pValue;
	double dData;
	DWORD dwCount;

	iCount = 0;
	swprintf(pKeyBigData,MAX_PATH,L"%s_%d",pKey,iCount);
	while(true)
	{
		if(fastReading==0)
		{
			dwCount = ::GetPrivateProfileString(pSection,pKeyBigData,g_pDefaultString2,line,MAX_PATH,pFileName);
		}else{
			dwCount =  GetPrivateProfileString(pSection,pKeyBigData,g_pDefaultString2,line,MAX_PATH,pFileName);
		}
		if(dwCount==MAX_PATH-1)
		{
			swprintf(pKeyBigData,MAX_PATH,L"Section \"%s\" key \"%s\" line overflow (maximum line size is %d)!",pSection,pKey,MAX_PATH-1);
			OempaApiMessageBox(NULL,pKeyBigData,L"File Error",MB_OK);
			return false;
		}
		if(!wcscmp(line,g_pDefaultString2))
		{
			if(!iCount)
			{
				if(!wcscmp(line,pKey))
				{
					NotifyErrorFile(pSection,pKey);
					iCountExpected = iCount;
					return false;
				}
				if(!wcscmp(pKeyBigData,pKey))
				{
					iCountExpected = iCount;
					return false;
				}
				wcscpy_s(pKeyBigData,MAX_PATH,pKey);
				continue;
			}
			if(iCountExpected && (iCountExpected!=iCount))
			{
				swprintf(line,MAX_PATH,L"Section \"%s\" key \"%s\" count should be %d not %d.",pSection,pKey,iCountExpected,iCount);
				OempaApiMessageBox(NULL,line,L"File Error",MB_OK);
				iCountExpected = iCount;
				return false;
			}
			if(!iCountExpected)
				iCountExpected = iCount;
			if(iCount)
				return true;
			return false;
		}
		pValue = ReadArrayDouble(line,dData);
		dData *= dUnitFactor;
		//while(AddArrayDouble(dData,iCountMax,pdData,iCountExpected,iCount) && pValue)
		while(true)
		{
			AddArrayDouble(dData,iCountMax,pdData,iCountExpected,iCount);
			if(!pValue || !*pValue)
				break;
			pValue = ReadArrayDouble(pValue,dData);
			dData *= dUnitFactor;
		}
		if(!wcscmp(pKeyBigData,pKey))
		{
			if(iCountExpected && (iCountExpected!=iCount))
			{
				swprintf(line,MAX_PATH,L"Section \"%s\" key \"%s\" count should be %d not %d.",pSection,pKey,iCountExpected,iCount);
				OempaApiMessageBox(NULL,line,L"File Error",MB_OK);
				iCountExpected = iCount;
				return false;
			}
			return true;
		}
		swprintf(pKeyBigData,MAX_PATH,L"%s_%d",pKey,iCount);
	}
	return false;
}

bool CRead_Write_ParametersList::FileReadArrayFloat2D(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iCountElement,int iCountFocal,float fUnitFactor,CDDF &afData,int &iCountExpected)
{
	wchar_t pKeyBigData[MAX_PATH];
	wchar_t line[MAX_PATH];
	int iIndex1,iIndex2,iCount;
	bool bKeyBigData=false;
	wchar_t *pValue;
	float fData;
	DWORD dwCount;

	iIndex1=iIndex2=0;
	iCount = 0;
	swprintf(pKeyBigData,MAX_PATH,L"%s_%d",pKey,iCount);
	while(true)
	{
		if(fastReading==0)
		{
			dwCount = ::GetPrivateProfileString(pSection,pKeyBigData,g_pDefaultString2,line,MAX_PATH,pFileName);
		}else{
			dwCount =  GetPrivateProfileString(pSection,pKeyBigData,g_pDefaultString2,line,MAX_PATH,pFileName);
		}
		if(dwCount==MAX_PATH-1)
		{
			swprintf(pKeyBigData,MAX_PATH,L"Section \"%s\" key \"%s\" line overflow (maximum line size is %d)!",pSection,pKey,MAX_PATH-1);
			OempaApiMessageBox(NULL,pKeyBigData,L"File Error",MB_OK);
			return false;
		}
		if(!wcscmp(line,g_pDefaultString2))
		{
			if(!iCount)
			{
				if(!wcscmp(line,pKey))
				{
					NotifyErrorFile(pSection,pKey);
					iCountExpected = iCount;
					return false;
				}
				if(!wcscmp(pKeyBigData,pKey))
				{
					iCountExpected = iCount;
					return false;
				}
				wcscpy_s(pKeyBigData,MAX_PATH,pKey);
				continue;
			}
			if(iCountExpected && (iCountExpected!=iCount))
			{
				swprintf(line,MAX_PATH,L"Section \"%s\" key \"%s\" count should be %d not %d.",pSection,pKey,iCountExpected,iCount);
				OempaApiMessageBox(NULL,line,L"File Error",MB_OK);
				iCountExpected = iCount;
				return false;
			}
			if(!iCountExpected)
				iCountExpected = iCount;
			if(iCount)
				return true;
			return false;
		}
		pValue = ReadArrayFloat(line,fData);
		fData *= fUnitFactor;
		//while(AddArrayFloat2D(fData,iCountElement,iCountFocal,afData,iCountExpected,iIndex1,iIndex2,iCount) && pValue)
		while(true)
		{
			AddArrayFloat2D(fData,iCountElement,iCountFocal,afData,iCountExpected,iIndex1,iIndex2,iCount);
			if(!pValue || !*pValue)
				break;
			pValue = ReadArrayFloat(pValue,fData);
			fData *= fUnitFactor;
		}
		if(!wcscmp(pKeyBigData,pKey))
		{
			if(iCountExpected && (iCountExpected!=iCount))
			{
				swprintf(line,MAX_PATH,L"Section \"%s\" key \"%s\" count should be %d not %d.",pSection,pKey,iCountExpected,iCount);
				OempaApiMessageBox(NULL,line,L"File Error",MB_OK);
				iCountExpected = iCount;
				return false;
			}
			return true;
		}
		swprintf(pKeyBigData,MAX_PATH,L"%s_%d",pKey,iCount);
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

bool CRead_Write_ParametersList::FileWriteInteger(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iData,wchar_t *pUnit)
{
	wchar_t wcAux[MAX_PATH];

	if(!CheckWriteFile(pFileName))
		return false;
	if(!pUnit || (pUnit && !wcslen(pUnit)))
	{
		swprintf(wcAux,MAX_PATH,L"%d",iData);
		if(fastWriting==0)
		{
			if(::WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}else{
			if(WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}
	}else{
		swprintf(wcAux,MAX_PATH,L"%d %s",iData,pUnit);
		if(fastWriting==0)
		{
			if(::WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}else{
			if(WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}
	}
	return false;
}

bool CRead_Write_ParametersList::FileWriteLong(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,long lData,wchar_t *pUnit)
{
	wchar_t wcAux[MAX_PATH];

	if(!CheckWriteFile(pFileName))
		return false;
	if(!pUnit || (pUnit && !wcslen(pUnit)))
	{
		swprintf(wcAux,MAX_PATH,L"%ld",lData);
		if(fastWriting==0)
		{
			if(::WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;

		}else{
			if(WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}
	}else{
		swprintf(wcAux,MAX_PATH,L"%ld %s",lData,pUnit);
		if(fastWriting==0)
		{
			if(::WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;

		}else{
			if(WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}
	}
	return false;
}

bool CRead_Write_ParametersList::FileWriteBoolean(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,bool bData)
{
	wchar_t wcAux[MAX_PATH];

	if(!CheckWriteFile(pFileName))
		return false;
	swprintf(wcAux,MAX_PATH,L"%d",bData?1:0);
	if(fastWriting==0){
		if(::WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
			return true;
		return false;
	}else{
		if(WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
			return true;
		return false;
	}
}

bool CRead_Write_ParametersList::FileWriteDouble(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,double dUnitFactor,double dData,wchar_t *pUnit)
{
	wchar_t wcAux[MAX_PATH];

	if(!CheckWriteFile(pFileName))
		return false;
	dData *= dUnitFactor;
	if(!pUnit || (pUnit && !wcslen(pUnit)))
	{
		swprintf(wcAux,MAX_PATH,L"%lf",dData);
		if(fastWriting==0)
		{
			if(::WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}else{
			if(WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}
	}else{
		swprintf(wcAux,MAX_PATH,L"%lf %s",dData,pUnit);
		if(fastWriting==0)
		{
			if(::WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}else{
			if(WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}
	}
	return false;
}

bool CRead_Write_ParametersList::FileWriteFloat(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,float fUnitFactor,float fData,wchar_t *pUnit)
{
	wchar_t wcAux[MAX_PATH];

	if(!CheckWriteFile(pFileName))
		return false;
	fData *= fUnitFactor;
	if(!pUnit || (pUnit && !wcslen(pUnit)))
	{
		swprintf(wcAux,MAX_PATH,L"%f",fData);
		if(fastWriting==0)
		{
			if(::WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}else{
			if(WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}
	}else{
		swprintf(wcAux,MAX_PATH,L"%f %s",fData,pUnit);
		if(fastWriting==0)
		{
			if(::WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}else{
			if(WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}
	}
	return false;
}

bool CRead_Write_ParametersList::FileWriteString(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,const wchar_t *wcData)
{
	if(!CheckWriteFile(pFileName))
		return false;
	if(fastWriting==0){
		if(::WritePrivateProfileString(pSection,pKey,wcData,pFileName))
			return true;
		return false;
	}else{
		if(WritePrivateProfileString(pSection,pKey,wcData,pFileName))
			return true;
		return false;
	}
}

bool CRead_Write_ParametersList::FileWriteEnum(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *pEnumList,int iEnum)
{
	wchar_t *pValue;
	bool bFound=false;

	if(!CheckWriteFile(pFileName))
		return false;
	pValue = pEnumList;
	while(iEnum && pValue)
	{
		iEnum--;
		pValue = GetEnumNext(pValue);
	}
	if(fastWriting==0)
	{
		if(::WritePrivateProfileString(pSection,pKey,pValue,pFileName))
			return true;
		return false;
	}else{
		if(WritePrivateProfileString(pSection,pKey,pValue,pFileName))
			return true;
		return false;
	}
}

bool CRead_Write_ParametersList::FileWriteWord(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,WORD wData,wchar_t *pUnit)
{
	wchar_t wcAux[MAX_PATH];

	if(!CheckWriteFile(pFileName))
		return false;
	if(!pUnit || (pUnit && !wcslen(pUnit)))
	{
		swprintf(wcAux,MAX_PATH,L"0x%.4lX",wData);
		if(fastWriting==0)
		{
			if(::WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}else{
			if(WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}
	}else{
		swprintf(wcAux,MAX_PATH,L"0x%.4lX %s",wData,pUnit);
		if(fastWriting==0)
		{
			if(::WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}else{
			if(WritePrivateProfileString(pSection,pKey,wcAux,pFileName))
				return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CRead_Write_ParametersList::FileWriteArrayShort(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,short *pwData,int iCount,wchar_t *pUnit)
{
	wchar_t wcAux[MAX_PATH];
	wchar_t wcLine[MAX_PATH];
	wchar_t wcKey[MAX_PATH];
	int iLineCount,iLoopCount,iDataCount;
	int iLength;

	if(!CheckWriteFile(pFileName))
		return false;
	if(iCount<=g_iTextFileMaxSingleLineDataCountForLong)
	{
		//single line
		iLength = 0;
		wcLine[0] = 0;
		for(int iIndex=0;iIndex<iCount;iIndex++)
		{
			if(iIndex==iCount-1)
			{
				if(!pUnit || (pUnit && !wcslen(pUnit)))
					swprintf(wcAux,MAX_PATH,L"%d",pwData[iIndex]);
				else
					swprintf(wcAux,MAX_PATH,L"%d %s",pwData[iIndex],pUnit);
			}else
				swprintf(wcAux,MAX_PATH,L"%d;",pwData[iIndex]);
			iLength += (int)wcslen(wcAux);
			if(iLength>=MAX_PATH)
			{
				wcLine[MAX_PATH-2] = L'X';
				wcLine[MAX_PATH-1] = 0;
				if(fastWriting==0){
					::WritePrivateProfileString(pSection,pKey,wcLine,pFileName);
					return false;
				}else{
					WritePrivateProfileString(pSection,pKey,wcLine,pFileName);
					return false;
				}
			}
			wcscat_s(wcLine,MAX_PATH,wcAux);
		}
		if(fastWriting==0){
			if(!::WritePrivateProfileString(pSection,pKey,wcLine,pFileName))
				return false;
		}else{
			if(!WritePrivateProfileString(pSection,pKey,wcLine,pFileName))
				return false;
		}
	}else{
		//multiline
		iLineCount = iCount/g_iTextFileLineDataCountForLong;
		if(iCount%g_iTextFileLineDataCountForLong)
			iLineCount++;
		iDataCount = 0;
		for(int iLineIndex=0;iLineIndex<iLineCount;iLineIndex++)
		{
			if(iLineIndex<iLineCount-1)
				iLoopCount = (int)g_iTextFileLineDataCountForLong;
			else if(iCount%g_iTextFileLineDataCountForLong)
				iLoopCount = iCount%g_iTextFileLineDataCountForLong;
			else
				iLoopCount = (int)g_iTextFileLineDataCountForLong;
			iLength = 0;
			wcLine[0] = 0;
			for(int iIndex=0;iIndex<iLoopCount;iIndex++)
			{
				if(iIndex==iLoopCount-1)
				{
					if(!pUnit || (pUnit && !wcslen(pUnit)))
						swprintf(wcAux,MAX_PATH,L"%d",pwData[iIndex+iDataCount]);
					else
						swprintf(wcAux,MAX_PATH,L"%d %s",pwData[iIndex+iDataCount],pUnit);
				}else
					swprintf(wcAux,MAX_PATH,L"%d;",pwData[iIndex+iDataCount]);
				iLength += (int)wcslen(wcAux);
				if(iLength>=MAX_PATH)
				{
					wcLine[MAX_PATH-2] = L'X';
					wcLine[MAX_PATH-1] = 0;
					swprintf(wcKey,MAX_PATH,L"%s_%d",pKey,iDataCount);
					if(fastWriting==0)
					{
						::WritePrivateProfileString(pSection,wcKey,wcLine,pFileName);
						return false;
					}else{
						WritePrivateProfileString(pSection,wcKey,wcLine,pFileName);
						return false;
					}
				}
				wcscat_s(wcLine,MAX_PATH,wcAux);
			}
			swprintf(wcKey,MAX_PATH,L"%s_%d",pKey,iDataCount);
			if(fastWriting==0)
			{
				if(!::WritePrivateProfileString(pSection,wcKey,wcLine,pFileName))
					return false;
			}else{
				if(!WritePrivateProfileString(pSection,wcKey,wcLine,pFileName))
					return false;
			}
			iDataCount += iLoopCount;
		}
	}
	return true;
}

bool CRead_Write_ParametersList::FileWriteArrayInteger(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int *piData,int iCount,wchar_t *pUnit)
{
	wchar_t wcAux[MAX_PATH];
	wchar_t wcLine[MAX_PATH];
	wchar_t wcKey[MAX_PATH];
	int iLineCount,iLoopCount,iDataCount;
	int iLength;

	if(!CheckWriteFile(pFileName))
		return false;
	if(iCount<=g_iTextFileMaxSingleLineDataCountForLong)
	{
		//single line
		iLength = 0;
		wcLine[0] = 0;
		for(int iIndex=0;iIndex<iCount;iIndex++)
		{
			if(iIndex==iCount-1)
			{
				if(!pUnit || (pUnit && !wcslen(pUnit)))
					swprintf(wcAux,MAX_PATH,L"%d",piData[iIndex]);
				else
					swprintf(wcAux,MAX_PATH,L"%d %s",piData[iIndex],pUnit);
			}else
				swprintf(wcAux,MAX_PATH,L"%d;",piData[iIndex]);
			iLength += (int)wcslen(wcAux);
			if(iLength>=MAX_PATH)
			{
				wcLine[MAX_PATH-2] = L'X';
				wcLine[MAX_PATH-1] = 0;
				if(fastWriting==0)
				{
					::WritePrivateProfileString(pSection,pKey,wcLine,pFileName);
					return false;
				}else{
					WritePrivateProfileString(pSection,pKey,wcLine,pFileName);
					return false;
				}
			}
			wcscat_s(wcLine,MAX_PATH,wcAux);
		}
		if(fastWriting==0)
		{
			if(!::WritePrivateProfileString(pSection,pKey,wcLine,pFileName))
				return false;
		}else{
			if(!WritePrivateProfileString(pSection,pKey,wcLine,pFileName))
				return false;
		}
	}else{
		//multiline
		iLineCount = iCount/g_iTextFileLineDataCountForLong;
		if(iCount%g_iTextFileLineDataCountForLong)
			iLineCount++;
		iDataCount = 0;
		for(int iLineIndex=0;iLineIndex<iLineCount;iLineIndex++)
		{
			if(iLineIndex<iLineCount-1)
				iLoopCount = (int)g_iTextFileLineDataCountForLong;
			else if(iCount%g_iTextFileLineDataCountForLong)
				iLoopCount = iCount%g_iTextFileLineDataCountForLong;
			else
				iLoopCount = (int)g_iTextFileLineDataCountForLong;
			iLength = 0;
			wcLine[0] = 0;
			for(int iIndex=0;iIndex<iLoopCount;iIndex++)
			{
				if(iIndex==iLoopCount-1)
				{
					if(!pUnit || (pUnit && !wcslen(pUnit)))
						swprintf(wcAux,MAX_PATH,L"%d",piData[iIndex+iDataCount]);
					else
						swprintf(wcAux,MAX_PATH,L"%d %s",piData[iIndex+iDataCount],pUnit);
				}else
					swprintf(wcAux,MAX_PATH,L"%d;",piData[iIndex+iDataCount]);
				iLength += (int)wcslen(wcAux);
				if(iLength>=MAX_PATH)
				{
					wcLine[MAX_PATH-2] = L'X';
					wcLine[MAX_PATH-1] = 0;
					swprintf(wcKey,MAX_PATH,L"%s_%d",pKey,iDataCount);
					if(fastWriting==0){
						::WritePrivateProfileString(pSection,wcKey,wcLine,pFileName);
						return false;
					}else{
						WritePrivateProfileString(pSection,wcKey,wcLine,pFileName);
						return false;
					}
				}
				wcscat_s(wcLine,MAX_PATH,wcAux);
			}
			swprintf(wcKey,MAX_PATH,L"%s_%d",pKey,iDataCount);
			if(fastWriting==0)
			{
				if(!::WritePrivateProfileString(pSection,wcKey,wcLine,pFileName))
					return false;
			}else{
				if(!WritePrivateProfileString(pSection,wcKey,wcLine,pFileName))
					return false;
			}
			iDataCount += iLoopCount;
		}
	}
	return true;
}

bool CRead_Write_ParametersList::FileWriteArrayFloat(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,float fUnitFactor,float *pfData,int iCount,wchar_t *pUnit)
{
	wchar_t wcAux[MAX_PATH];
	wchar_t wcLine[MAX_PATH];
	wchar_t wcKey[MAX_PATH];
	int iLineCount,iLoopCount,iDataCount;
	int iLength;

	if(!CheckWriteFile(pFileName))
		return false;
	if(iCount<=g_iTextFileMaxSingleLineDataCountForLong)
	{
		//single line
		iLength = 0;
		wcLine[0] = 0;
		for(int iIndex=0;iIndex<iCount;iIndex++)
		{
			if(iIndex==iCount-1)
			{
				if(!pUnit || (pUnit && !wcslen(pUnit)))
					swprintf(wcAux,MAX_PATH,L"%f",pfData[iIndex]*fUnitFactor);
				else
					swprintf(wcAux,MAX_PATH,L"%f %s",pfData[iIndex]*fUnitFactor,pUnit);
			}else
				swprintf(wcAux,MAX_PATH,L"%f;",pfData[iIndex]*fUnitFactor);
			iLength += (int)wcslen(wcAux);
			if(iLength>=MAX_PATH)
			{
				wcLine[MAX_PATH-2] = L'X';
				wcLine[MAX_PATH-1] = 0;
				if(fastWriting==0)
				{
					::WritePrivateProfileString(pSection,pKey,wcLine,pFileName);
					return false;
				}else{
					WritePrivateProfileString(pSection,pKey,wcLine,pFileName);
					return false;
				}
			}
			wcscat_s(wcLine,MAX_PATH,wcAux);
		}
		if(fastWriting==0){
			if(!::WritePrivateProfileString(pSection,pKey,wcLine,pFileName))
				return false;
		}else{
			if(!WritePrivateProfileString(pSection,pKey,wcLine,pFileName))
				return false;
		}
	}else{
		//multiline
		iLineCount = iCount/g_iTextFileLineDataCountForLong;
		if(iCount%g_iTextFileLineDataCountForLong)
			iLineCount++;
		iDataCount = 0;
		for(int iLineIndex=0;iLineIndex<iLineCount;iLineIndex++)
		{
			if(iLineIndex<iLineCount-1)
				iLoopCount = (int)g_iTextFileLineDataCountForLong;
			else if(iCount%g_iTextFileLineDataCountForLong)
				iLoopCount = iCount%g_iTextFileLineDataCountForLong;
			else
				iLoopCount = (int)g_iTextFileLineDataCountForLong;
			iLength = 0;
			wcLine[0] = 0;
			for(int iIndex=0;iIndex<iLoopCount;iIndex++)
			{
				if(iIndex==iLoopCount-1)
				{
					if(!pUnit || (pUnit && !wcslen(pUnit)))
						swprintf(wcAux,MAX_PATH,L"%f",pfData[iIndex+iDataCount]*fUnitFactor);
					else
						swprintf(wcAux,MAX_PATH,L"%f %s",pfData[iIndex+iDataCount]*fUnitFactor,pUnit);
				}else
					swprintf(wcAux,MAX_PATH,L"%f;",pfData[iIndex+iDataCount]*fUnitFactor);
				iLength += (int)wcslen(wcAux);
				if(iLength>=MAX_PATH)
				{
					wcLine[MAX_PATH-2] = L'X';
					wcLine[MAX_PATH-1] = 0;
					swprintf(wcKey,MAX_PATH,L"%s_%d",pKey,iDataCount);
					if(fastWriting==0)
					{
						::WritePrivateProfileString(pSection,pKey,wcLine,pFileName);
						return false;
					}else{
						WritePrivateProfileString(pSection,pKey,wcLine,pFileName);
						return false;
					}
				}
				wcscat_s(wcLine,MAX_PATH,wcAux);
			}
			swprintf(wcKey,MAX_PATH,L"%s_%d",pKey,iDataCount);
			if(fastWriting==0){
				if(!::WritePrivateProfileString(pSection,wcKey,wcLine,pFileName))
					return false;
			}else{
				if(!WritePrivateProfileString(pSection,wcKey,wcLine,pFileName))
					return false;
			}
			iDataCount += iLoopCount;
		}
	}
	return true;
}

bool CRead_Write_ParametersList::FileWriteArrayFloat(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,float fUnitFactor,float *pfData,int iCount,wchar_t *pPrecision,wchar_t *pPrecision2)
{
	wchar_t wcAux[MAX_PATH];
	wchar_t wcLine[MAX_PATH];
	wchar_t wcKey[MAX_PATH];
	int iLineCount,iLoopCount,iDataCount;
	int iLength;

	if(!CheckWriteFile(pFileName))
		return false;
	if(iCount<=g_iTextFileMaxSingleLineDataCountForLong)
	{
		//single line
		iLength = 0;
		wcLine[0] = 0;
		for(int iIndex=0;iIndex<iCount;iIndex++)
		{
			if(iIndex==iCount-1)
				swprintf(wcAux,MAX_PATH,pPrecision,pfData[iIndex]*fUnitFactor);
			else
				swprintf(wcAux,MAX_PATH,pPrecision2,pfData[iIndex]*fUnitFactor);
			iLength += (int)wcslen(wcAux);
			if(iLength>MAX_PATH)
				return false;
			iLength += (int)wcslen(wcAux);
			if(iLength>=MAX_PATH)
			{
				wcLine[MAX_PATH-2] = L'X';
				wcLine[MAX_PATH-1] = 0;
				if(fastWriting==0)
				{
					::WritePrivateProfileString(pSection,pKey,wcLine,pFileName);
					return false;
				}else{
					WritePrivateProfileString(pSection,pKey,wcLine,pFileName);
					return false;
				}
			}
			wcscat_s(wcLine,MAX_PATH,wcAux);
		}
		if(fastWriting==0)
		{
			if(!::WritePrivateProfileString(pSection,pKey,wcLine,pFileName))
				return false;
		}else{
			if(!WritePrivateProfileString(pSection,pKey,wcLine,pFileName))
				return false;
		}
	}else{
		//multiline
		iLineCount = iCount/g_iTextFileLineDataCountForLong;
		if(iCount%g_iTextFileLineDataCountForLong)
			iLineCount++;
		iDataCount = 0;
		for(int iLineIndex=0;iLineIndex<iLineCount;iLineIndex++)
		{
			if(iLineIndex<iLineCount-1)
				iLoopCount = (int)g_iTextFileLineDataCountForLong;
			else if(iCount%g_iTextFileLineDataCountForLong)
				iLoopCount = iCount%g_iTextFileLineDataCountForLong;
			else
				iLoopCount = (int)g_iTextFileLineDataCountForLong;
			iLength = 0;
			wcLine[0] = 0;
			for(int iIndex=0;iIndex<iLoopCount;iIndex++)
			{
				if(iIndex==iLoopCount-1)
					swprintf(wcAux,MAX_PATH,L"%f",pfData[iIndex+iDataCount]*fUnitFactor);
				else
					swprintf(wcAux,MAX_PATH,L"%f;",pfData[iIndex+iDataCount]*fUnitFactor);
				iLength += (int)wcslen(wcAux);
				if(iLength>=MAX_PATH)
				{
					wcLine[MAX_PATH-2] = L'X';
					wcLine[MAX_PATH-1] = 0;
					swprintf(wcKey,MAX_PATH,L"%s_%d",pKey,iDataCount);
					if(fastWriting==0)
					{
						::WritePrivateProfileString(pSection,pKey,wcLine,pFileName);
						return false;
					}else{
						WritePrivateProfileString(pSection,pKey,wcLine,pFileName);
						return false;
					}
				}
				wcscat_s(wcLine,MAX_PATH,wcAux);
			}
			swprintf(wcKey,MAX_PATH,L"%s_%d",pKey,iDataCount);
			if(fastWriting==0)
			{
				if(!::WritePrivateProfileString(pSection,wcKey,wcLine,pFileName))
					return false;
			}else{
				if(!WritePrivateProfileString(pSection,wcKey,wcLine,pFileName))
					return false;
			}
			iDataCount += iLoopCount;
		}
	}
	return true;
}

bool CRead_Write_ParametersList::FileWriteArrayDouble(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,double dUnitFactor,double *pdData,int iCount,wchar_t *pUnit)
{
	wchar_t wcAux[MAX_PATH];
	wchar_t wcLine[MAX_PATH];
	wchar_t wcKey[MAX_PATH];
	int iLineCount,iLoopCount,iDataCount;
	int iLength;

	if(!CheckWriteFile(pFileName))
		return false;
	if(iCount<=g_iTextFileMaxSingleLineDataCountForLong)
	{
		//single line
		iLength = 0;
		wcLine[0] = 0;
		for(int iIndex=0;iIndex<iCount;iIndex++)
		{
			if(iIndex==iCount-1)
			{
				if(!pUnit || (pUnit && !wcslen(pUnit)))
					swprintf(wcAux,MAX_PATH,L"%f",pdData[iIndex]*dUnitFactor);
				else
					swprintf(wcAux,MAX_PATH,L"%f %s",pdData[iIndex]*dUnitFactor,pUnit);
			}else
				swprintf(wcAux,MAX_PATH,L"%f;",pdData[iIndex]*dUnitFactor);
			iLength += (int)wcslen(wcAux);
			if(iLength>=MAX_PATH)
			{
				wcLine[MAX_PATH-2] = L'X';
				wcLine[MAX_PATH-1] = 0;
				if(fastWriting==0)
				{
					::WritePrivateProfileString(pSection,pKey,wcLine,pFileName);
					return false;
				}else{
					WritePrivateProfileString(pSection,pKey,wcLine,pFileName);
					return false;
				}
			}
			wcscat_s(wcLine,MAX_PATH,wcAux);
		}
		if(fastWriting==0)
		{
			if(!::WritePrivateProfileString(pSection,pKey,wcLine,pFileName))
				return false;
		}else{
			if(!WritePrivateProfileString(pSection,pKey,wcLine,pFileName))
				return false;
		}
	}else{
		//multiline
		iLineCount = iCount/g_iTextFileLineDataCountForLong;
		if(iCount%g_iTextFileLineDataCountForLong)
			iLineCount++;
		iDataCount = 0;
		for(int iLineIndex=0;iLineIndex<iLineCount;iLineIndex++)
		{
			if(iLineIndex<iLineCount-1)
				iLoopCount = (int)g_iTextFileLineDataCountForLong;
			else if(iCount%g_iTextFileLineDataCountForLong)
				iLoopCount = iCount%g_iTextFileLineDataCountForLong;
			else
				iLoopCount = (int)g_iTextFileLineDataCountForLong;
			iLength = 0;
			wcLine[0] = 0;
			for(int iIndex=0;iIndex<iLoopCount;iIndex++)
			{
				if(iIndex==iLoopCount-1)
				{
					if(!pUnit || (pUnit && !wcslen(pUnit)))
						swprintf(wcAux,MAX_PATH,L"%f",pdData[iIndex+iDataCount]*dUnitFactor);
					else
						swprintf(wcAux,MAX_PATH,L"%f %s",pdData[iIndex+iDataCount]*dUnitFactor,pUnit);
				}else
					swprintf(wcAux,MAX_PATH,L"%f;",pdData[iIndex+iDataCount]*dUnitFactor);
				iLength += (int)wcslen(wcAux);
				if(iLength>=MAX_PATH)
				{
					wcLine[MAX_PATH-2] = L'X';
					wcLine[MAX_PATH-1] = 0;
					swprintf(wcKey,MAX_PATH,L"%s_%d",pKey,iDataCount);
					if(fastWriting==0)
					{
						::WritePrivateProfileString(pSection,wcKey,wcLine,pFileName);
						return false;
					}else{
						WritePrivateProfileString(pSection,wcKey,wcLine,pFileName);
						return false;
					}
				}
				wcscat_s(wcLine,MAX_PATH,wcAux);
			}
			swprintf(wcKey,MAX_PATH,L"%s_%d",pKey,iDataCount);
			if(fastWriting==0)
			{
				if(!::WritePrivateProfileString(pSection,wcKey,wcLine,pFileName))
					return false;
			}else{
				if(!WritePrivateProfileString(pSection,wcKey,wcLine,pFileName))
					return false;
			}
			iDataCount += iLoopCount;
		}
	}
	return true;
}

bool CRead_Write_ParametersList::FileWriteArrayFloat2D(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iElementCount,int iFocalCount,float fUnitFactor,CDDF &afData,wchar_t *pUnit)
{
	wchar_t wcAux[MAX_PATH];
	wchar_t wcLine[MAX_PATH];
	wchar_t wcKey[MAX_PATH];
	int iLineCount,iLoopCount,iDataCount;
	int iIndex1,iIndex2;
	float fData;
	int iCount;
	int iLength;

	if(!CheckWriteFile(pFileName))
		return false;
	iCount = iElementCount*iFocalCount;
	if(iCount<=g_iTextFileMaxSingleLineDataCountForFloat)
	{
		//single line
		iLength = 0;
		wcLine[0] = 0;
		iIndex1=iIndex2=0;
		for(int iIndex=0;iIndex<iCount;iIndex++)
		{
			if(!GetArrayFloat2D(afData,iElementCount,iFocalCount,iIndex1,iIndex2,fData))
				break;
			if(iIndex==iCount-1)
			{
				if(!pUnit || (pUnit && !wcslen(pUnit)))
					swprintf(wcAux,MAX_PATH,L"%f",fData*fUnitFactor);
				else
					swprintf(wcAux,MAX_PATH,L"%f %s",fData*fUnitFactor,pUnit);
			}else
				swprintf(wcAux,MAX_PATH,L"%f;",fData*fUnitFactor);
			iLength += (int)wcslen(wcAux);
			if(iLength>=MAX_PATH)
			{
				wcLine[MAX_PATH-2] = L'X';
				wcLine[MAX_PATH-1] = 0;
				if(fastWriting==0){
					::WritePrivateProfileString(pSection,pKey,wcLine,pFileName);
					return false;
				}else{
					WritePrivateProfileString(pSection,pKey,wcLine,pFileName);
					return false;
				}
			}
			wcscat_s(wcLine,MAX_PATH,wcAux);
		}
		if(fastWriting==0)
		{
			if(!::WritePrivateProfileString(pSection,pKey,wcLine,pFileName))
				return false;
		}else{
			if(!WritePrivateProfileString(pSection,pKey,wcLine,pFileName))
				return false;
		}
	}else{
		//multiline
		iLineCount = iCount/g_iTextFileLineDataCountForFloat;
		if(iCount%g_iTextFileLineDataCountForFloat)
			iLineCount++;
		iIndex1=iIndex2=0;
		iDataCount = 0;
		for(int iLineIndex=0;iLineIndex<iLineCount;iLineIndex++)
		{
			if(iLineIndex<iLineCount-1)
				iLoopCount = (int)g_iTextFileLineDataCountForFloat;
			else if(iCount%g_iTextFileLineDataCountForFloat)
				iLoopCount = iCount%g_iTextFileLineDataCountForFloat;
			else
				iLoopCount = (int)g_iTextFileLineDataCountForFloat;
			iLength = 0;
			wcLine[0] = 0;
			for(int iIndex=0;iIndex<iLoopCount;iIndex++)
			{
				if(!GetArrayFloat2D(afData,iElementCount,iFocalCount,iIndex1,iIndex2,fData))
					break;
				if(iIndex==iLoopCount-1)
				{
					if(!pUnit || (pUnit && !wcslen(pUnit)))
						swprintf(wcAux,MAX_PATH,L"%f",fData*fUnitFactor);
					else
						swprintf(wcAux,MAX_PATH,L"%f %s",fData*fUnitFactor,pUnit);
				}else
					swprintf(wcAux,MAX_PATH,L"%f;",fData*fUnitFactor);
				iLength += (int)wcslen(wcAux);
				if(iLength>=MAX_PATH)
				{
					wcLine[MAX_PATH-2] = L'X';
					wcLine[MAX_PATH-1] = 0;
					swprintf(wcKey,MAX_PATH,L"%s_%d",pKey,iDataCount);
					if(fastWriting==0){
						::WritePrivateProfileString(pSection,wcKey,wcLine,pFileName);
						return false;
					}else{
						WritePrivateProfileString(pSection,wcKey,wcLine,pFileName);
						return false;
					}
				}
				wcscat_s(wcLine,MAX_PATH,wcAux);
			}
			swprintf(wcKey,MAX_PATH,L"%s_%d",pKey,iDataCount);
			if(fastWriting==0)
			{
				if(!::WritePrivateProfileString(pSection,wcKey,wcLine,pFileName))
					return false;
			}else{
				if(!WritePrivateProfileString(pSection,wcKey,wcLine,pFileName))
					return false;
			}

			iDataCount += iLoopCount;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////


bool CRead_Write_ParametersList::AddArrayShort(short wData,int iCountMax,short *pwData,int iCountExpected,int &iIndex)
{
	if(iIndex>=iCountMax)
	{
		iIndex++;
		return false;
	}
	if(iCountExpected && (iIndex>=iCountExpected))
	{
		iIndex++;
		return false;
	}
	pwData[iIndex++] = wData;
	return true;
}

bool CRead_Write_ParametersList::AddArrayInteger(int iData,int iCountMax,int *piData,int iCountExpected,int &iIndex)
{
	if(iIndex>=iCountMax)
	{
		iIndex++;
		return false;
	}
	if(iCountExpected && (iIndex>=iCountExpected))
	{
		iIndex++;
		return false;
	}
	piData[iIndex++] = iData;
	return true;
}

bool CRead_Write_ParametersList::AddArrayFloat(float fData,int iCountMax,float *pfData,int iCountExpected,int &iIndex)
{
	if(iIndex>=iCountMax)
	{
		iIndex++;
		return false;
	}
	if(iCountExpected && (iIndex>=iCountExpected))
	{
		iIndex++;
		return false;
	}
	pfData[iIndex++] = fData;
	return true;
}

bool CRead_Write_ParametersList::AddArrayDouble(double dData,int iCountMax,double *pdData,int iCountExpected,int &iIndex)
{
	if(iIndex>=iCountMax)
	{
		iIndex++;
		return false;
	}
	if(iCountExpected && (iIndex>=iCountExpected))
	{
		iIndex++;
		return false;
	}
	pdData[iIndex++] = dData;
	return true;
}

bool  CRead_Write_ParametersList::AddArrayFloat2D(float fData,int iCountElement,int iCountFocal,CDDF &afData,int iCountExpected,int &iIndex1,int &iIndex2,int &iCount)
{
	if(iIndex2>=iCountFocal)
	{
		iCount++;
		return false;
	}
	if(iIndex1>=iCountElement)
	{
		iCount++;
		return false;
	}
	if(iCountExpected && (iIndex1+iCountElement*iIndex2>=iCountExpected))
	{
		iCount++;
		return false;
	}
	afData[iIndex2][iIndex1] = fData;
	iIndex1++;
	if(iIndex1>=iCountElement)
	{
		iIndex1 = 0;
		iIndex2++;
	}
	iCount++;
	return true;
}

bool CRead_Write_ParametersList::GetArrayFloat2D(CDDF &afData,int iElementCount,int iFocalCount,int &iIndex1,int &iIndex2,float &fData)
{
	if(iIndex2>=iFocalCount)
		return false;
	if(iIndex1>=iElementCount)
		return false;
	fData = afData[iIndex2][iIndex1];
	iIndex1++;
	if(iIndex1>=iElementCount)
	{
		iIndex1 = 0;
		iIndex2++;
	}
	return true;
}

void CRead_Write_ParametersList::NotifyErrorFile(wchar_t *pSection,wchar_t *pKey)
{
	wchar_t msg[MAX_PATH];

	swprintf(msg,MAX_PATH,L"Error in section '%s' for key '%s'!",pSection,pKey);
	OempaApiMessageBox(NULL,msg,L"File Error",MB_OK);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

int CRead_Write_ParametersList::GetPrivateProfileInt(const wchar_t* pSection, const wchar_t* pKey, int defaultValue, const wchar_t* pFileName)
{
	wchar_t data[MAX_PATH];
	int valueInt;

	GetPrivateProfileString(pSection,pKey,L"taratata", data, MAX_PATH,pFileName);
	if(!wcscmp(L"taratata",data))
		return defaultValue;
	valueInt = _wtoi(data);
	return valueInt;
}

unsigned long CRead_Write_ParametersList::GetPrivateProfileString(const wchar_t* pSection,const wchar_t* pKey,const wchar_t* g_pDefaultString2, wchar_t* data, unsigned long TotalSize,const wchar_t* pFileName)
{
	//ljr std::vector<myCSection*> *pointerStruct = getVector();
	size_t size1, size2;

	if(!m_ListSectionName.size())
	{
		wcscpy_s(data,TotalSize,g_pDefaultString2);
		return (unsigned long)wcslen(g_pDefaultString2);
	}
	
	// Initialisation
	int foundKey=-1;
	int foundSection=-1;
	wstring variable;
	wstring valueString;
	vector<myCSection*>::iterator pBeginListSectionName = m_ListSectionName.begin();
	vector<myCSection*>::iterator pEndListSectionName = m_ListSectionName.end()-1;
	vector<myCSection*>::iterator pMidListSectionName = pEndListSectionName;
	
	// binary search in ListSection
	size2 = wcslen(pSection);
	size1 = wcslen(pEndListSectionName[0]->m_wcName);
	foundSection = wmemcmp(pSection,pEndListSectionName[0]->m_wcName,max(size1,size2));
	if(foundSection!=0)
	{
		size1 = wcslen(pBeginListSectionName[0]->m_wcName);
		foundSection = wmemcmp(pSection,pBeginListSectionName[0]->m_wcName,max(size1,size2));
		pMidListSectionName = pBeginListSectionName;
	}

	while ((pEndListSectionName-pBeginListSectionName>1) && foundSection!=0)
	{//ljr comment: dichotomic search.
		pMidListSectionName = pBeginListSectionName+(pEndListSectionName-pBeginListSectionName)/2;
		size1 = wcslen(pMidListSectionName[0]->m_wcName);
		foundSection = wmemcmp(pSection,pMidListSectionName[0]->m_wcName,max(size1,size2));
		if(foundSection < 0)
			pEndListSectionName = pMidListSectionName;
		else if(foundSection > 0)
			pBeginListSectionName = pMidListSectionName;
	}

	list<myCKey>::iterator pSearchKey = pMidListSectionName[0]->m_ListKey.begin();

	if(foundSection==0)
	{
		size2 = wcslen(pKey);
		//ljr comment: NO dichotomic search (probably because it is not faster in this case).
		while(pSearchKey != pMidListSectionName[0]->m_ListKey.end() && foundKey!=0)
		{
			size1 = wcslen(pSearchKey->m_wcName);
			if(size1!=size2)
			{
				pSearchKey++;
				continue;
			}
			foundKey = wmemcmp(pKey,pSearchKey->m_wcName,size1);
			if(foundKey!=0)
				pSearchKey++;
		}
	}
	if(foundKey==0)
		wcscpy_s(data,TotalSize,pSearchKey->m_wcValue);
	else
		wcscpy_s(data,TotalSize,g_pDefaultString2);

	return (unsigned long)valueString.size();
}


bool CRead_Write_ParametersList::CheckWriteFile(const wchar_t *pFileName)
{
	if(!wcslen(m_wcWriteFile))
	{
		wcscpy_s(m_wcWriteFile,MAX_PATH,pFileName);
		return true;
	}
	if(wcscmp(m_wcWriteFile,pFileName))
		return false;
	return true;
}

bool CRead_Write_ParametersList::WritePrivateProfileString(wchar_t* pSection,wchar_t* pKey,const wchar_t* data,const wchar_t* pFileName)
{
	myCKey myLine;
	myCSection *mySection = &CurrentSection;
	std::list<myCSection> *listSection=&m_ListSection;

	if((wmemcmp(mySection->m_wcName,pSection,max(wcslen(mySection->m_wcName),wcslen(pSection)))!=0) && (wcslen(mySection->m_wcName)>0))
	{
		listSection->push_back(*mySection);
		mySection->SetWString(pSection);
		mySection->m_ListKey.clear();
	}
	else if((wmemcmp(mySection->m_wcName,pSection,max(wcslen(mySection->m_wcName),wcslen(pSection)))!=0) && (wcslen(mySection->m_wcName)<=0))
	{
		mySection->SetWString(pSection);
	}
	if(wmemcmp(mySection->m_ListKey.back().m_wcName,pKey,max(wcslen(mySection->m_ListKey.back().m_wcName),wcslen(pKey)))==0)
	{
		myLine.SetWString(pKey,data);
		mySection->m_ListKey.back() = myLine;
	}else{
		myLine.SetWString(pKey,data);
		mySection->m_ListKey.push_back(myLine);
	}
	return true;
}

bool CRead_Write_ParametersList::ReadListWriteFile(const wchar_t* pFileName)
{
	if(!fastWriting || !wcslen(m_wcWriteFile))
		return false;

	std::wofstream fout;
	fout.open(pFileName);
	myCSection *mySection = &CurrentSection;
	std::list<myCSection> *listSection=&m_ListSection;

	listSection->push_back(*mySection);

	
	for(list<myCSection>::iterator pointerSection = listSection->begin();pointerSection != listSection->end();pointerSection++)
	{
		fout << L"[" << pointerSection->m_wcName << L"]" << L"\n";

		for(list<myCKey>::iterator pointerKey = pointerSection->m_ListKey.begin();pointerKey != pointerSection->m_ListKey.end();pointerKey++)
		{
			fout << pointerKey->m_wcName << L"=" << pointerKey->m_wcValue << L"\n";
		}

	}
	fout.close();

	m_wcWriteFile[0] = 0;//file has been written
	return true;
}

CRead_Write_ParametersList::~CRead_Write_ParametersList()
{
	if(fastWriting && (wcslen(m_wcWriteFile)>0))
		ReadListWriteFile(m_wcWriteFile);//fastWriting is not so fast (same speed than old way), but it is a little bit more complex to use, so it is better to not use it.

	//ljr
	//list<myCSection>::iterator itSection = m_ListSection.begin();
	//for(itSection=m_ListSection.begin();itSection!=m_ListSection.end();itSection++)
	//{
	//	list<myCKey>::iterator itKey = itSection->m_ListKey.begin();
	//	for(itKey=itSection->m_ListKey.begin();itKey!=itSection->m_ListKey.end();itKey++)
	//	{
	//		if(itKey->m_wcValue && iswalpha(itKey->m_wcValue[0]))
	//			delete itKey->m_wcValue;
	//		itKey->m_wcValue = NULL;
	//	}
	//	itSection->m_ListKey.clear();
	//}
	//m_ListSection.clear();
}
