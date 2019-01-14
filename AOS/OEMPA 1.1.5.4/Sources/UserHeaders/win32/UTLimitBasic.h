
#pragma once

const int g_iEnumStringSizeMax=32;//max size of one enumeration string.

class CUTBaseClass;

//Callback options are available for each callback function.
typedef struct UTKERNEL_API structCallbackOption
{
public:
	structCallbackOption();

	void Reset();
	void operator=(const structCallbackOption &a);
	bool operator!=(const structCallbackOption &a);
	bool operator==(const structCallbackOption &a);
	void *pInputParameter;//input parameter to use when the callback function is called.
	LARGE_INTEGER _llInputParameter;//input parameter to use when the callback function is called.
	unsigned bWaitEnd:1;//when the function is called from another application, the caller have to wait the end of the function process.
						//in case "bPostMessage" is true, if "bWaitEnd" is true then "SendMessage" is used, otherwise "PostMessage" is used.
	unsigned bExternalProcess:1;//the function can be called from another application.
	unsigned bRelative:1;//in case the callback is attached to an item of a class, if true the callback is called for any item below the hierarchical tree.
	unsigned bStatus:1;//specify if we want to receive Status notification.
	unsigned bLimit:1;//specify if we want to receive Limit notification.
	unsigned bThisRootIdOnly:1;//if true function is registered for the current one instance RootId.
	short sPriority;//special value SHRT_MAX to disable the callback (the minimum value is the highest priority callback function).
	int iIndexItem;//for array this is the particular index item ? -1 if not (all items).
	bool bPostMessage;//An intermediate Window message is used to notify update (if true bWaitEnd is not used).
private:

	bool bKernel;//if kernel this is not UserInterface!
}structCallbackOption;

typedef enum enumLimitStatus{	eLimitStatusHWEnable=(1<<0),//the driver can disable the item.
								eLimitStatusHWResolution=(1<<1),//the driver can set the resolution (in this case the user cannot change it).
								eLimitStatusHWApplyToAll=(1<<2),//the driver can state the ApplyToAll.
								eLimitStatusApplyToAllRigth=(1<<3),//ability to use ApplyToAll for the item.
								eLimitStatusTargetLinkId=(1<<4),
								eLimitStatusArraySingleItem=(1<<5),
								eLimitStatusKernelItem=(1<<6),eLimitStatusKernelEnum=(1<<7),
								eLimitStatusKernelItem1D=(1<<8),eLimitStatusKernelItem2D=(1<<9),
								eLimitStatusKernelItemClass=(1<<10),eLimitStatusKernelItemList=(1<<11),
								eLimitStatusKernelArray=(1<<12),eLimitStatusKernelArrayClass=(1<<13),
								eLimitStatusKernelArrayList=(1<<14),
								eLimitStatusKernelEditEnable=(1<<16),

								eLimitStatusInFile=(1<<(16+0)),
								eLimitStatusPathWithListId=(1<<(16+1)),
								eLimitStatusApplyToAllUser=(1<<(16+2)),//the user have the ability to set ApplyToAll.
								eLimitStatusCallbackTimerUpdate=(1<<(16+3)),
								eLimitStatusInSharedMemory=(1<<(16+4))
							}enumLimitStatus;
typedef struct UTKERNEL_API_ALIGN structLimitCUTBaseClass
{
	enumClassID m_eClassID;
	enumUnit unit;
	double factor;//if the unit is used then this factor can be taken into account in memory for special unit. For example encoder is integer but the unit is millimeter, so the factor is 0.001.
					//it can be taken into account also for binary format.
	int iGrowBy;

	//constant member that canno't be modified during runtime.
	unsigned int bHWEnable:1;//false if the parameter is not supported by the HW.
	unsigned int bHWResolution:1;//resolution of the parameter.
	unsigned int bHWApplyToAll:1;
	unsigned int bApplyToAllRight:1;//enable the use of ApplyToAll.
	unsigned int bTargetLinkId:1;//this item is a link to another object!
	unsigned int bArraySingleItem:1;
	unsigned int bKernelItem:1;//inherit of CUTX
	unsigned int bKernelItemEnum:1;//inherit of CUTEnumX

	unsigned int bKernelItem1D:1;//element of CUTArrayX 1D
	unsigned int bKernelItem2D:1;//element of CUTArrayX 2D
	unsigned int bKernelItemClass:1;//element of CUTArrayCX
	unsigned int bKernelItemList:1;//element of CUTArrayCX (list)
	unsigned int bKernelArrayX:1;//inherit of CUTArrayX
	unsigned int bKernelArrayCXClass:1;//inherit of CUTArrayCX
	unsigned int bKernelArrayCXList:1;//inherit of CUTArrayCX (list)
	unsigned int bKernelEditEnable:1;

	//member that can be modified during runtime.
	unsigned int bInFile:1;//some item could be disabled during running time (example data buffer of acq buffer). BE CAREFULL : in such case those item should be at the last cases of "::vf_GetChildConst" function !!!!
	unsigned int bPathWithListId:1;
	unsigned int bApplyToAllUser:1;//"apply to all" set by the user.
	unsigned int bCallbackUpdateTimer:1;
	unsigned int bInMemory:1;//bInSharedMemory could be a better name ? it should be better to put it in static members !!
	unsigned int bMultiChannel:1;//simplification of "CUTChannels". it should be better to put it in static members !!
	unsigned int bUTParameters:1;//simplification of "CUTChannels". it should be better to put it in static members !!
	unsigned int bSetDefault:1;//useful for "_CUTWizardScan::m_aScanPriority"

	unsigned int bKernelMonoChannelEnable:1;//simplification of "CUTChannels". it should be better to put it in static members !!
	unsigned int bHWVisible:1;//useful for enumeration which only one item is enabled by the driver.
	unsigned int bExpand:1;//automatic tree expand
	unsigned int iLimitStatusMarge2:5;

}structLimitCUTBaseClass;

class UTKERNEL_API_ALIGN _limitCUTBaseClass
{
private:
	ULONGLONG m_ullPrivateMemory[6];
protected:

};

class UTKERNEL_API_ALIGN limitCUTBaseClass :
	public structLimitCUTBaseClass,
	public _limitCUTBaseClass
{
public:
	limitCUTBaseClass();

private:
	virtual int limitCUTBaseClass::vf_GetReservedData();
};


class UTKERNEL_API_ALIGN _limitCUTKernelRoot
{
private:
	ULONGLONG m_ullPrivateMemory[34];
public:

};

class UTKERNEL_API_ALIGN limitCUTKernelRoot :
	public limitCUTBaseClass,
	public _limitCUTKernelRoot
{
public:

};
