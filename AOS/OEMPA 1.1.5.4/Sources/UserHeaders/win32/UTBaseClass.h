
#pragma once


typedef enum enumBaseStatus{eStatusReadEnable=(1<<0),eStatusWriteEnable=(1<<1),
						eStatusEditEnable=(1<<2),
						eStatusVisible=(1<<3),
						eStatusEventUpdateEnable=(1<<4),
						eStatusKernel=(1<<5),
						eStatusUpdate=(1<<6),eStatusPathRootID=(1<<7)
					}enumBaseStatus;


class UTKERNEL_API_ALIGN _CUTBaseClass
{
private:
	ULONGLONG m_ullPrivateMemory[12];
public:
	_CUTBaseClass();

};

class UTKERNEL_API_ALIGN CUTBaseClass :
	public _CUTBaseClass
{

public:

	CUTBaseClass(const CUTBaseClass &a);
	~CUTBaseClass();
	virtual void ProcessPreSignalDestructor();
	void PreSignalDestructor();

	int GetListId() const;

	CUTBaseClass *GetObject(int iKernelLanguageIndex,const wchar_t *pPath,structLayer &layerIndex);//current language.
	CUTBaseClass *GetUserObject(int iSizeIndex,structLayer *pLayerIndex,structLayer &layerIndex);
	static bool sGetValueString(const wchar_t *pPath,int iSize, wchar_t *pValue);
	static bool sSetValueString(const wchar_t *pPathWithValue);

	int GetPathSize(int iKernelLanguageIndex) const;
	bool GetPath(int iKernelLanguageIndex,int iSize,wchar_t *pPath) const;
	bool GetPath(int iKernelLanguageIndex,int iSize,wchar_t *pPath,int &iSizeRequired) const;

	void GetMemberStringValue(const structLayer &layerIndex,int iKernelLanguageIndex,int iSizeName,wchar_t *pMemberValue,CUTBaseClass *pBaseClass);//current language.

	bool GetValueString(int iKernelLanguageIndex,int iSize,wchar_t *pValue) const;//current language.
	int SetValueString(wchar_t *pValue);

	int GetChildCount(enumClassID eClassID,int &iCount1,int &iCount2) const;

	bool GetUserLayerPath(int iKernelLanguageIndex,const wchar_t *pMember,int iSizeIndex,structLayer *pLayerIndex,int &iLayer) const;//user function.

	bool GetName(int iKernelLanguageIndex,int iSize,wchar_t *pName) const;
	const wchar_t *GetName(int iKernelLanguageIndex) const;
	const wchar_t *GetDefaultName() const;

	bool IsDisabled(bool bRead,CUTBaseClass *pObject,const structLayer &layerIndex);
	bool IsReadDisabled(CUTBaseClass *pObject,const structLayer &layerIndex);
	bool IsWriteDisabled(CUTBaseClass *pObject,const structLayer &layerIndex);
	void Enable(bool bValue);//both read and write
	void EnableWrite(bool bValue);
	void EnableWrite(bool bValue,bool bEventUpdate);
	void EnableRead(bool bValue);
	void Visible(bool bValue);

	bool IsReadDisabled(bool bThisItemOnly=false) const;
	bool IsWriteDisabled(bool bThisItemOnly=false) const;
	bool IsReadEnabled(bool bThisItemOnly=false) const;
	bool IsWriteEnabled(bool bThisItemOnly=false) const;
	bool IsEnabled(bool bThisItemOnly=false) const;
	bool IsVisibled(bool bThisItemOnly=false) const;
	bool IsExpand() const;

	bool IsStatusEnabled() const;
	unsigned int GetStatus() const;
	unsigned int GetLimitStatus() const;

	bool IsStatusVisibled() const;
	bool IsStatusKernel() const;
	bool IsStatusKernelItem() const;
	bool IsStatusKernelItem1D() const;
	bool IsStatusKernelItem2D() const;
	bool IsStatusKernelItemClass() const;
	bool IsStatusKernelItemList() const;
	bool IsStatusKernelArray() const;
	bool IsStatusKernelArrayClass() const;
	bool IsStatusKernelArrayList() const;
	bool IsApplyToAll() const;
	void Kernel(bool bValue);
	bool ApplyToAll(bool bValue);

	bool GetEnumCount(int &iCount) const;
	bool GetEnumItem(int iItem,int iSize,wchar_t *pEnum,bool &bEnabled) const;

private:
	virtual int CUTBaseClass::vf_GetReservedData();
};
