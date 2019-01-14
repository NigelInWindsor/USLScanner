#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <sstream> 
#include <list>
#include <algorithm>
#include <locale>
#include <codecvt>
#include "windows.h"

class CDDF;

using namespace std;

#define g_iSectionSize  50
#define g_iKeySize  50

class myCKey
{
public:
	myCKey();
	myCKey(const myCKey &a);
	~myCKey();
	void Desalloc();
	void Reset();

	bool operator < (const myCKey& rhs);
	myCKey& operator = (const myCKey& rhs);//ljr
	myCKey& Copy(const myCKey& rhs);//ljr
	bool SetLine(std::wstring &line);
	bool SetRString(wchar_t *wcValue);

	bool SetWString(wchar_t *wcName,const wchar_t *wcValue);

	wchar_t m_wcName[g_iKeySize];
	wchar_t* m_wcValue;
	int m_iValueSize;//LJR
};
 
class myCSection
{
public:
	myCSection();
	myCSection(const myCSection& rhs);
	~myCSection();
	void Desalloc();
	void Reset();
	
	bool operator < (const myCSection& rhs);
	myCSection& operator = (const myCSection& rhs);
	myCSection& Copy(myCSection& rhs);

	bool SetLine(std::wstring &line);
	bool SetRString(wchar_t *wcValue);

	bool SetWString(wchar_t *wcValue);

	wchar_t m_wcName[g_iSectionSize];
#pragma warning( disable : 4251 )//those members aren't/can't be used by clients (class not exported) => switch off the warning is enough.
	std::list<myCKey> m_ListKey;
#pragma warning( default : 4251 )
};

class _CUST_DRV_DECL_ CRead_Write_ParametersList
{
public:

	// Constructor
	CRead_Write_ParametersList();
 
	//ljr void setVector(vector<myCSection*> ListSectionName);

	bool safeFileReadString(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *pString,int iSize);
	bool FileReadInteger(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,  int &iData,bool bDisplayMsg=true,int iMin=INT_MIN,int iMax=INT_MAX, int iDefault=0);
	bool FileReadLong(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName, long &lData,bool bDisplayMsg=true);
	bool FileReadULong(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName, unsigned long &dwData,bool bDisplayMsg=true);
	bool FileReadBoolean(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName, bool &bData,bool bDisplayMsg=true);
	bool FileReadDouble(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,double dUnitFactor,double &dData,bool bDisplayMsg=true);
	bool FileReadFloat(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,float fUnitFactor,float &fData,bool bDisplayMsg=true);
	bool FileReadString(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *wcData,int iSize,bool bDisplayMsg=true);
	bool FileReadWord(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName, WORD &wData, bool bDisplayMsg=true);
	wchar_t* GetEnumNext(wchar_t *pEnum);
	bool GetHex(wchar_t c,int &tmp);
	bool FileReadEnum(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *pEnumList,int &iEnum, bool bDisplayErrorMsg=true);
	wchar_t *ReadArrayLongLong(bool bDecimal,wchar_t *pValue,long long &llValue);
	wchar_t *ReadArrayFloat(wchar_t *pValue,float &fValue);
	wchar_t *ReadArrayDouble(wchar_t *pValue,double &dValue);
	bool FileReadArrayShort(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iCountMax,short *pwData,int &iCountExpected);
	bool FileReadArrayInteger(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iCountMax,int *piData,int &iCountExpected);
	bool FileReadArrayFloat(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iCountMax,float fUnitFactor,float *pfData,int &iCountExpected);
	bool FileReadArrayDouble(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iCountMax,double dUnitFactor,double *pdData,int &iCountExpected);
	bool FileReadArrayFloat2D(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iCountElement,int iCountFocal,float fUnitFactor,CDDF &afData,int &iCountExpected);
	
	bool FileWriteInteger(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iData,wchar_t *pUnit);
	bool FileWriteLong(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,long lData,wchar_t *pUnit);
	bool FileWriteBoolean(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,bool bData);
	bool FileWriteDouble(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,double dUnitFactor,double dData,wchar_t *pUnit);
	bool FileWriteFloat(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,float fUnitFactor,float fData,wchar_t *pUnit);
	bool FileWriteString(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,const wchar_t *wcData);
	bool FileWriteEnum(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,wchar_t *pEnumList,int iEnum);
	bool FileWriteWord(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,WORD wData,wchar_t *pUnit);
	bool FileWriteArrayShort(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,short *pwData,int iCount,wchar_t *pUnit);
	bool FileWriteArrayInteger(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int *piData,int iCount,wchar_t *pUnit);
	bool FileWriteArrayFloat(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,float fUnitFactor,float *pfData,int iCount,wchar_t *pUnit);
	bool FileWriteArrayFloat(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,float fUnitFactor,float *pfData,int iCount,wchar_t *pPrecision,wchar_t *pPrecision2);
	bool FileWriteArrayDouble(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,double dUnitFactor,double *pdData,int iCount,wchar_t *pUnit);
	bool FileWriteArrayFloat2D(wchar_t *pSection,wchar_t *pKey,const wchar_t *pFileName,int iElementCount,int iFocalCount,float fUnitFactor,CDDF &afData,wchar_t *pUnit);
	
	bool AddArrayShort(short wData,int iCountMax,short *pwData,int iCountExpected,int &iIndex);
	bool AddArrayInteger(int iData,int iCountMax,int *piData,int iCountExpected,int &iIndex);
	bool AddArrayFloat(float fData,int iCountMax,float *pfData,int iCountExpected,int &iIndex);
	bool AddArrayDouble(double dData,int iCountMax,double *pdData,int iCountExpected,int &iIndex);
	bool AddArrayFloat2D(float fData,int iCountElement,int iCountFocal,CDDF &afData,int iCountExpected,int &iIndex1,int &iIndex2,int &iCount);
	bool GetArrayFloat2D(CDDF &afData,int iElementCount,int iFocalCount,int &iIndex1,int &iIndex2,float &fData);
	void NotifyErrorFile(wchar_t *pSection,wchar_t *pKey);

	int GetPrivateProfileInt(const wchar_t* lpAppName, const wchar_t* lpKeyName, int nDefault, const wchar_t* lpFileName);
	unsigned long GetPrivateProfileString(const wchar_t* lpAppName,const wchar_t* lpKeyName,const wchar_t* lpDefault, wchar_t* lpReturnedString, unsigned long nSize,const wchar_t* lpFileName);
	bool WritePrivateProfileString(wchar_t* lpAppName,wchar_t* lpKeyName,const wchar_t* wcdata,const wchar_t* lpFileName);
	void ReadTxtCreateList(const wchar_t* lpFileName);
	bool ReadListWriteFile(const wchar_t* lpFileName);//fastWriting is not so fast (same speed than old way), but it is a little bit more complex to use, so it is better to not use it.
	void Dump(char* pFileName);

	//ljr list<myCSection> *getListSection() {return &m_ListSection;};
	//ljr vector<myCSection*> *getVector() {return &ListSectionName;} ;
	bool IsComment(std::wstring &line);//ljr

  
	// Destructor
	~CRead_Write_ParametersList();

private :
	bool CheckWriteFile(const wchar_t *pFileName);

#pragma warning( disable : 4251 )//those members aren't/can't be used by clients => switch off the warning is enough.
	list<myCSection> m_ListSection;
	vector<myCSection*> m_ListSectionName;
	myCSection CurrentSection;
#pragma warning( default : 4251 )
	wchar_t m_wcWriteFile[MAX_PATH];

public:
	int fastReading;
	int fastWriting;
 
};
 