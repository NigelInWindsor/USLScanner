
#pragma once
#define _WIN_STUB_API_
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __WARN__ __FILE__ "("__STR1__(__LINE__)") : Warning user: "
void _UTKernel_TraceMemory(wchar_t *);
void _UTKernel_TraceError(wchar_t *);
void _UTKernel_TraceError(wchar_t *,int);
void _UTKernel_TraceError(char *,int);
const int g_iSharedRootMax=2048;//maximum count of root instances and UTPath item.



class CWSKernelData;

class _WIN_STUB_API_ _CWSMutex
{
protected:

};

class _WIN_STUB_API_ CWSMutex :
	public _CWSMutex
{
public:

	bool Lock();
	bool Unlock();
	bool TryLock();
	bool TryLock(DWORD dwTimeOut,bool &bLockedByOtherProcess);
	bool IsLockedPossible();
	bool Lock(DWORD dwTimeOut,DWORD *pdwWaitResult,bool *pbLockedByOtherProcess);
private:
	bool CreateMutex();//first alloc only.
	bool OpenMutex();//second alloc.
};

typedef struct structWSDataSharedHeader
{
	DWORD m_dwAllocatedSize;
	DWORD dwMutexCommonThreadId;
	LONG lMutexCommonCount;
	DWORD dwMutex2CommonThreadId;
	LONG lMutex2CommonCount;
} structWSDataSharedHeader;

typedef struct structWSDataMainHeader
{
	int m_iHeaderSize;
	int m_iDataSize;
	int m_iID;
	int m_iDataSizeCurrent;
	LONGLONG m_llUpdateDataCount;
	LONGLONG m_llUpdateHeaderCount;

	//no mutual exclusion required for this members:
	LONG m_lOwnerCount;
	DWORD m_dwOwnerThreadId;
	DWORD m_dwOwnerProcessId;
} structWSDataMainHeader;

typedef struct structWSDataHeader
{
	structWSDataMainHeader main;
	structWSDataSharedHeader shared;
} structWSDataHeader;

class _WIN_STUB_API_ _CWSKernelDataHeader
{
protected:

};

class _WIN_STUB_API_ CWSKernelDataHeader :
	protected structWSDataHeader,
	public _CWSKernelDataHeader
{
public:

private:
	virtual int CWSKernelDataHeader::vf_GetReservedData();
};

class CWSKernelData;
class _WIN_STUB_API_ _CWSKernelData
{
public:

};

class _WIN_STUB_API_ CWSKernelData :
	public CWSKernelDataHeader,
	public CWSMutex,
	public _CWSKernelData
{

public:

	bool IsAllocated() const;

	bool _GetID(int &iID) const;

	bool Lock(DWORD dwTimeOut=INFINITE,DWORD *pdwWaitResult=NULL);
	bool Unlock();

	LPVOID GetHeaderBuffer();
	LPVOID GetDataBuffer();

//overwrite : no need to specify size and sign.
	//array slow access : index check and lock is used.
	bool SetData(void *pBuffer,int iSize,int iByteOffset);
	bool GetData(void *pBuffer,int iSize,int iByteOffset);

	//array quick access : no index check and lock is not used.
	void SetQuickData(void *pBuffer,int iSize,int iByteOffset);
	void GetQuickData(void *pBuffer,int iSize,int iByteOffset);


public:
	structWSDataHeader *GetDataHeader();
private:
	virtual int CWSKernelData::vf_GetReservedData();
};

