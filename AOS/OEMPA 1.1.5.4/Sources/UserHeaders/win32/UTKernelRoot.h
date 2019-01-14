
#pragma once


const int g_iLayerMax=20;//maximum layer deep of any class.


//Callback function can be registered on the root of a instance
//if the callback is attached to a sub-item in this hierarchical data tree,
//you need to specify the path in this tree.
//2 different way can be used to specify the path : index list or string path.
//If the index list is null then string path is used.
typedef struct UTKERNEL_API structCallbackRegistrationPath{
	structCallbackRegistrationPath();
	structMaskStatus maskStatus;
	int iLayerSize;		//layer size, usefull when the callback is attached to an item of a hierarchical tree.
	structLayer *pLayerIndex;	//layer path, usefull when the callback is attached to an item of a hierarchical tree.
	const wchar_t *pPath;		//in case the callback is attached to an item of a hierarchical tree, this path is the one of the item.
						//in other case it is a string to identify the callback. In this last case this name is used to registered the callback and also to find it.
	int iKernelLanguageIndex;//language of the path. -1 => current language of UTKernelAPI.dll.
}structCallbackRegistrationPath;


typedef struct structDefaultSentence{

public:
	structDefaultSentence(){pDefaultEnglishValue=NULL;pLanguageCurrent=NULL;iLanguageCurrent=-1;iLanguageRequest=-1;}
	int iLanguageRequest;
private:
	const wchar_t *pDefaultEnglishValue;
	const wchar_t *pLanguageCurrent;
	int iLanguageCurrent;
} structDefaultSentence;

class UTKERNEL_API_ALIGN _CUTStringTable
{
private:
	ULONGLONG m_ullPrivateMemory[534];
public:

};

class UTKERNEL_API_ALIGN CUTStringTable :
	public _CUTStringTable
{
private:
public:

	CUTStringTable(HMODULE hModule,pCallbackFunction pCallback,wchar_t *pCallbackComment);
	~CUTStringTable();

	static CUTStringTable* GetStringTable(wchar_t *pModulePath);//if NULL then this is the main UTKernelAPI module.
	static bool CheckAllFolder();
	static bool GetConfigFolder(const wchar_t *pModuleName,int iSize,wchar_t *pFolderPath);
	static bool SetConfigFolder(const wchar_t *pModuleName,const wchar_t *pFolderPath);
	static bool GetConfigLanguage(const wchar_t *pModuleName,int iSize,wchar_t *pLanguage);
	static bool SetConfigLanguage(const wchar_t *pModuleName,const wchar_t *pLanguage);
	static bool SetStringTableExe(CUTStringTable *pStringTable);

	bool SetModule(HMODULE hModule);
	bool SetModule(wchar_t *pModuleName);
	bool GetModule(int iSize,wchar_t *pModuleName);
	bool GetModuleExt(int iSize,wchar_t *pModuleName);
	bool SetFolder(wchar_t *pFilePath);
	bool GetFolder(int iSize,wchar_t *pFilePath);
	bool Alloc();
	bool Free();

	int GetLanguageCount();
	bool GetLanguageIndex(wchar_t *pName,int &iLanguageIndex);
	bool SetLanguageCurrent(const wchar_t *pName);
	const wchar_t *GetLanguageName(int iLanguageIndex);
	bool GetLanguageCurrent(int iSize,wchar_t *pName);
	int GetLanguageCurrent();
	static int sGetLanguageCurrent();//one application have only one CUTStringTable.
	bool GetLanguage(int iLanguageIndex,int iSize,wchar_t *pName);

	int GetStringCount();
	bool GetString(int iStringIndex,int iSize,wchar_t *pValue,int *piRequiredSize=NULL);//current language
	bool GetString(int iLanguageIndex,int iStringIndex,int iSize,wchar_t *pValue,int *piRequiredSize);
	const wchar_t *GetStringTable(int iLanguageIndex,int iStringIndex);

	bool GetResourcesLanguage(int &iMemorySize);

	const wchar_t* _Language(int iStringIndex,const wchar_t *pDefaultEnglishValue);
	const wchar_t* _LanguaXe(int iStringIndex,int iThisLanguage,const wchar_t *pDefaultEnglishValue);
	const wchar_t* _Languag2(int iStringIndex,const wchar_t *pDefaultEnglishValue);
	const wchar_t* _KeyWord_(int iStringIndex,const wchar_t *pDefaultEnglishValue,const wchar_t *pKeyWordDefinition);
	const wchar_t* _KeyWord2(int iStringIndex,const wchar_t *pDefaultEnglishValue,const wchar_t *pKeyWordDefinition);
	const wchar_t* _Sentence(int iStringIndex,structDefaultSentence &defSentence,const wchar_t *pDefaultEnglishNoKeyWord,const wchar_t *pDefaultEnglishKeyWord);
	const wchar_t* _Sentenc2(int iStringIndex,structDefaultSentence &defSentence,const wchar_t *pDefaultEnglishNoKeyWord,const wchar_t *pDefaultEnglishKeyWord);

	int GetAllSize();
	int GetDataSize();


};


class CUTPreference;
class CUTEncoder;
class CUTAxis;
class CUTAcquisitionData;
class CUTDevice;
class CUTChannels;
class CUTWizardSystem;
class CUTReservedForFuture;
class CUTDisplayCursor;
class CUTDisplayZoom;
class CUTDisplayLayout;
class UTKERNEL_API_ALIGN _CUTKernelRoot
{
private:
	ULONGLONG m_ullPrivateMemory[303];
protected:

};

class UTKERNEL_API_ALIGN CUTKernelRoot :
	public CUTBaseClass,
	public _CUTKernelRoot
{

public:
	static CUTPreference* cast_pCUTPreference(CUTKernelRoot* pPtr);
	static CUTEncoder* cast_pCUTEncoder(CUTKernelRoot* pPtr);
	static CUTAxis* cast_pCUTAxis(CUTKernelRoot* pPtr);
	static CUTAcquisitionData* cast_pCUTAcquisitionData(CUTKernelRoot* pPtr);
	static CUTDevice* cast_pCUTDevice(CUTKernelRoot* pPtr);
	static CUTChannels* cast_pCUTParameters(CUTKernelRoot* pPtr);
	static CUTWizardSystem* cast_pCUTWizardSystem(CUTKernelRoot* pPtr);
	//static CUTReservedForFuture* cast_pCUTReservedForFuture(CUTKernelRoot* pPtr);
	static CUTDisplayCursor* cast_pCUTDisplayCursor(CUTKernelRoot* pPtr);
	static CUTDisplayZoom* cast_pCUTDisplayZoom(CUTKernelRoot* pPtr);
	static CUTDisplayLayout* cast_pCUTDisplayLayout(CUTKernelRoot* pPtr);
	static CUTKernelData& cast_CUTKernelData(CUTKernelRoot& a);
	int operator=(const CUTKernelRoot &a);

	//static functions to get information in object database of the kernel.
	static int GetRootIdFirst();//to get the first identification number in the kernel database.
	static int GetRootIdNext(int iRootId);//to get the next identification number in the kernel database.
				//example: loop to get identificatino number on all kernel objects
				//int iRootId=CUTKernelRoot::GetRootIdFirst();
				//while(iRootId>=0)
				//	iRootId = =CUTKernelRoot::GetRootIdNext(iRootId);
	static bool findRootName(const wchar_t *pName,enumRootType &eRootType,int &iRootId);
	static CUTKernelRoot *CUTKernelRoot::FindRootName(const wchar_t *pName,enumRootType eRootID);//to find an object in the current kernel database.
					//CURRENT means if the object is external the returned value will be NULL.
					//Returned value : pointer on the object.
					//pName : input parameter, name of the object.
					//eRootID : input parameter, type of the object.
	static bool FindRootName(const wchar_t *pName,enumRootType eRootID,int &iRootID);//to find an object in the kernel database.
					//Returned value : true if no error, false otherwise.
					//pName : input parameter, name of the object.
					//eRootID : input parameter, type of the object.
					//iRootID : output parameter, identification number of the object in the kernel database.
	static bool FindRootId(int iRootID,enumRootType &eRootID);//to find an object in the kernel database.
					//Returned value : true if no error, false otherwise.
					//iRootID : input parameter, identification number of the object in the kernel database.
	static bool FindRootId(int iRootID,int iSizeName,wchar_t *pName,int &iSizeNameOut,enumRootType &eRootID,DWORD &dwProcessId);//to find an object in the kernel database.
					//Returned value : true if no error, false otherwise.
					//iRootID : input parameter, identification number of the object in the kernel database.
					//iSizeName : input parameter, size of the buffer pName.
					//pName : input/output string to receive the name of the object.
					//iSizeNameOut : output parameter, size of the name of the object.
					//eRootID : output parameter, type of the object.
					//dwProcessId : output parameter, Id of the first process that have registered this object.
	static bool FindRootId(int iRootID,int iSizeName,wchar_t *pName,int iSizeNameOld,wchar_t *pNameOld,int &iSizeNameOut,int &iSizeNameOldOut,enumRootType &eRootID,DWORD &dwProcessId);
					//Returned value : true if no error, false otherwise.
					//iRootID : input parameter, identification number of the object in the kernel database.
	static bool FindRootID(enumRootType eRootType,const wchar_t *pName,int &iRootID);//to find an object in the kernel database.
					//Returned value : true if no error, false otherwise.
					//eRootType : input parameter, type of the object.
					//pName : input string, the name of the object.
					//iRootID : output parameter, identification number of the object in the kernel database.
	static bool FindRootID(int iRootID,int iSize,wchar_t *pName,enumRootType &eRootType);//to find an object in the kernel database.
					//Returned value : true if no error, false otherwise.
					//iRootID : input parameter, identification number of the object in the kernel database.
					//iSize : input parameter, size of the buffer pName.
					//pName : input/output string to receive the name of the object.
					//eRootType : output parameter, type of the object.
	static bool GetRootCounters(int iRootID,int &iRootCount,int &iRootDataCount);

	//others static functions to access the kernel
	static CUTKernelRoot *New(enumRootType eRootID);//allocate a new object.
					//Returned value : pointer on the new object.
					//eRootID : input parameter, type of the object.
	static bool Delete(enumRootType eRootID,CUTKernelRoot *ptr);
					//Returned value : true if no error, false otherwise.
					//eRootID : input parameter, type of the object.
					//ptr : input parameter, pointer on the object.
	static int DeleteAll();
	static CUTKernelRoot *SafeNewObject(enumRootType eRootType,wchar_t *pName,bool &bCreate);
	static CUTKernelRoot *NewObject(enumRootType eRootType,wchar_t *pName);
	static CUTKernelRoot *NewIfRequired(enumRootType eRootID,int iRootID,bool &bCreate);//allocate a new object if it doesn't exist in the current process.
					//Return value : pointer on the object.
					//	If the kernel object has been already registered in the current process (via 
					//	"::SetID()" or "::SetName()") then
					//	the returned pointer is a pointer on this data,
					//	otherwise a new object is dynamically allocated in memory (with operator new).
					//	It is quicker to use this function than to allocate
					//	local object and connect it to the kernel with "::SetID()".
					//	But if the global main object is deleted, then the pointer will
					//	be not valid. So be carefull...
					//You have to call "DeleteIfRequired" at the end.
					//eRootID : input parameter, type of the object.
					//iRootID : input parameter, identification number of the object.
					//bCreate : output parameter, false if the object has been already created in the current process,
					//			true if the object has been dynamically allocated (no object has been previously created in this current process).
					//			This value is useful later to call "DeleteIfRequired"..
	static CUTKernelRoot *NewIfRequired(enumRootType eRootID,wchar_t *pName,bool &bCreate);//allocate a new class if it doesn't exist.
					//same than the previous function but the name of the object is the input parameter instead the identification number.
	static bool DeleteIfRequired(enumRootType eRootID,bool bCreate,CUTKernelRoot *pRoot);//desallocate a new class if previously allocated by "NewIfRequired".
					//Should be called after each call to "NewIfRequired".
					//eRootID : input parameter, type of the object.
					//bCreate : input parameter, this is the value that has been returned of function "NewIfRequired".
					//pRoot : input parameter, returned value of function "NewIfRequired".
	static CUTKernelRoot* Find(enumRootType eRootID,int iRootID);//to find an object in the kernel database.
					//Return value : pointer on the object or NULL if object is not found.
					//eRootID : input parameter, type of the object.
					//iRootID : input parameter, identification number of the object.
	static CUTKernelRoot* Find(enumRootType eRootID,wchar_t *pName);//to find an object in the kernel database.
					//Return value : pointer on the object or NULL if object is not found.
					//eRootID : input parameter, type of the object.
					//pName : input parameter, name of the object.
	static enumRootType GetRootType(enumClassID eClassID);//to get the "enumRootType" of a "eClassID".
	static bool GetName(int iRootID,int iSize,wchar_t *pName);//to get the name of an object in the kernel database.
					//Returned value : true if no error, false otherwise.
					//iRootID : input parameter, identification number of the object.
					//iSize : input parameter, size of the buffer pName.
					//pName : input/output string to receive the name of the object.

	static int GetDefaultSettingDataSize(enumRootType eRootID);
	void TraceErrorDefaultSettingDataSize(enumRootType eRootID,int iDataSize,int iSizeRequired);
	static int GetAcquisitionBufferParent(int iRootID,enumRootType &eParentRootType);

	//function to register the object in the kernel database
	bool SetID(int iRootID);//to connect with a preexisting data in kernel database.
					//Returned value : true if no error, false otherwise.
	bool GetID(int &iRootID) const;//to get the identification number of the object.
					//Returned value : true if no error, false otherwise.
					//return false if this object has not been registered in the kernel database
					//otherwise return true.
					//iRootID : output parameters, -1 if error otherwise identification number.
	bool SetName(const wchar_t *pName,int iRootID=-1);//to register this new data in the kernel database.
					//Returned value : true if no error, false otherwise.
	bool GetName(int iSize,wchar_t *pName) const;//to get the name of the registered object.
					//return false if this object has not been registered in the kernel database
					//otherwise return true.
	bool FreeID();//to unregister the kernel object from the kernel database.
	bool FindID(wchar_t *pName,int &iRootID);//to find the name in the kernel database.
					//Returned value : true if no error, false otherwise.
					//pName : input parameter, name of the object.
					//iRootID : output parameter, identification number of the object.
	int CUTKernelRoot::EventSystem(enumEvent eEvent,int iRootID,enumRootType eRoot,const wchar_t *pName,wchar_t *pNameOld);

	int GetChildAllocatedCount() const;
	void SetAutomaticFree(bool bAutoFree);//to desalloc the object automatically at the end by the kernel.
	bool IsAutomaticFree();

	enumRootType GetRootType() const;

	//file exchange member functions
	bool SaveBinary(bool bAcquisitionBuffer,int iSizeBuffer,char *pBuffer,int &iSizeOut);
			//to save one by one object in binary file, inputs : iSizeBuffer, pBuffer => input buffer where to store data.
	bool SaveText(bool bFirst,bool bAcquisitionBuffer,const wchar_t *pFileName);
			//to save one by one object from text file.
	static bool StaticLoadText(const wchar_t *pFileName,int iSize,int *aiRootID,enumRootType *aeRootID,int &iSizeOut,int &iKernelLanguageIndex,int &iFileRootNameIndex);
			//to get object list inside a text file.
	bool LoadText(bool bLastObject,const wchar_t *pFileName,wchar_t *pVersionExtension,int iKernelLanguageIndex,int iFileRootNameIndex,const wchar_t *pName,bool bEnableCheck,bool bRootID,int &iRootID,bool &bVersion);
			//to load one by one object from text file, "pVersionExtension" is only used to rename configuration object.
			//bEnableCheck : true to check any data of the file.
			//				false to disable check and be able to load disabled data.
	static bool StaticLoadBinary(int iSizeBuffer,char *pBuffer,int iBufferOffset,wchar_t *pVersionExtension,int iNameSize,wchar_t *pName,enumRootType &eRootID,int &iSizeof);
			//to get object list inside a binary file.
	bool LoadBinary(int iSizeBuffer,char *pBuffer,wchar_t *pVersionExtension,const wchar_t *pName,int &iSizeOut,int &iRootID,bool &bVersion);
			//to load one by one object from binary file, "pVersionExtension" is only used to rename configuration object.

	static void GetClassName(enumRootType eRootID,int iSize,wchar_t *pName);//current language
	static void GetClassName(enumRootType eRootID,int iKernelLanguageIndex,int iSize,wchar_t *pName);
	static bool SetClassName(enumRootType eRootID,const wchar_t *pName,int iStringTableId);//current language.


	//user callback
	bool RegisterCallbackUser(	pCallbackFunction pCallback,wchar_t *pCallbackComment,
							enumEvent eEvent,structCallbackRegistrationPath callbackPath,
							void *pInputParameter,
							LARGE_INTEGER &llInputParameter,
							LARGE_INTEGER &llCallbackID);
	bool UnregisterCallback(pCallbackFunction pCallback,
							enumEvent eEvent,structCallbackRegistrationPath callbackPath
							);
	LRESULT Callback(enumEvent eEvent,bool bThisRootIdOnly,bool bAllDaisyChainCallback,bool bRelative,int iLayerSize,structLayer *pLayerIndex,const wchar_t *pPath,LARGE_INTEGER llCallbackID,int iExtendParameterSize,void *pExtendParameterPtr);//easy way to call daisy chain functions.
	bool GetRegisterCallback(	enumEvent eEvent,structCallbackRegistrationPath callbackPath,
								LARGE_INTEGER &llCallbackID);
	
	//callback

	bool RegisterCallback(	pCallbackFunction pCallback,wchar_t *pCallbackComment,
							enumEvent eEvent,structCallbackRegistrationPath callbackPath,
							structCallbackOption &callbackOption,
							LARGE_INTEGER &llCallbackID);
	bool RegisterCallbackUser(pCallbackFunction pCallback,wchar_t *pCallbackComment,enumEvent eEvent,void *pInputParameter,LARGE_INTEGER &llInputParameter,LARGE_INTEGER &llCallbackID);
	bool UnregisterCallback(pCallbackFunction pCallback,
							enumEvent eEvent,structCallbackRegistrationPath callbackRegistration1,structCallbackOption callbackRegistration2
							);
	bool UnregisterCallback(LARGE_INTEGER llCallbackID);

	bool IsVersion() const;

	int GetChildCount(CUTBaseClass *pObject,const structLayer &layerIndex,int &iCount1,int &iCount2);

	bool IsItem(CUTBaseClass *pObject,const structLayer &layerIndex);
	bool GetComment(CUTBaseClass *pObject,const structLayer &layerIndex,int iSize,wchar_t *pValue,int &iSizeRequired);
	bool GetPath(CUTBaseClass *pObject,const structLayer &layerIndex,int iSize,wchar_t *pValue,int &iSizeRequired);
	bool GetBaseLimit(CUTBaseClass *pObject,const structLayer &layerIndex,limitCUTBaseClass *pLimit);

	bool CheckLinkId(CUTBaseClass *pObject,const structLayer &layerIndex,wchar_t *pTargetPath,enumRootType eTargetRootClassID);
	bool SetLinkId(CUTBaseClass *pObject,const structLayer &layerIndex,wchar_t *pTargetPath,enumRootType eTargetRootClassID);
	bool GetMemberStringValue(CUTBaseClass *pObject,const structLayer &layerIndex,int iKernelLanguageIndex,int iSize,wchar_t *pValue,int &iStatus,limitCUTBaseClass *pLimit);
	bool GetStatus(CUTBaseClass *pObject,const structLayer &layerIndex,int &iStatus,int &iLimitStatus);
	bool SetValueString(int iLayerSize,structLayer *pLayerIndex,wchar_t *pValue);
	bool GetValueString(int iLayerSize,structLayer *pLayerIndex,int iSize,wchar_t *pValue);
	bool GetCallbackPathCurrentLanguage(const structCallbackParameter &param,int iSize,wchar_t *pPathCurrentLanguage);

	bool GetListLinkId(enumRootType eRootType,int iSizeIn,int *aiListId,int &iSizeOut);//find in the kernel the list of all available objects that point on this one.

private:
	virtual int CUTKernelRoot::vf_GetReservedData();
};

