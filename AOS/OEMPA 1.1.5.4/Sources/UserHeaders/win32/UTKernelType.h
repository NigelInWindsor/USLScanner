
#pragma once
#include "UTKernelExport.h"

typedef struct UTKERNEL_API_ALIGN structLayer
{
	//structLayer(){memset(this,0,sizeof(this));};
	void operator=(const structLayer &a);
	bool operator!=(const structLayer &a);
	bool operator==(const structLayer &a);

	enumClassID eClassID;
	int iIndexItem;
	WORD wIndex1D;
	WORD wIndex2D;
}structLayer;

class CUTBaseClass;
class CUTKernelRoot;
struct structCallbackParameter;
struct structCallbackDispatch;

typedef LRESULT (WINAPI* pCallbackFunction)(const structCallbackParameter &param,structCallbackDispatch &callDispatch);

typedef struct UTKERNEL_API structMaskStatus
{
	void Reset();
	structMaskStatus();
	void operator=(const structMaskStatus &a);
	bool operator!=(const structMaskStatus &a);
	bool operator==(const structMaskStatus &a);
	bool bMaskStatus;
	bool bNotMaskStatus;
	int iMaskStatus;
	int iNotMaskStatus;
}structMaskStatus;

typedef struct UTKERNEL_API structCallbackParameter
{
	structCallbackParameter();
//inputs
	bool bExternalProcess;					//bExternalProcess : true if the call comes from another application.
	bool bExternalDisabled;			//Reserved (bExternalDisabled : true to disable external call).
	DWORD dwSrcProcessId;				//processId of the root object.
	DWORD dwDstProcessId;
	enumEvent eEvent;
	enumRootType eRootType;				//root type.
	enumClassID eClassID;				//item type.
	int iRootSystemID;					//for system event ("eEventNew","eEventDelete","eEventReName","eEventTrace")
										//this is the iRootID of the object (to notify new object for example).
	int iRootObjectID;					//iRootObjectID : root id of the class from which the updated item belong.
										//			for a system event ("eEventNew","eEventDelete","eEventReName","eEventTrace") this value is 0.
	int iRootObjectPathID;				//iRootObjectPathID : root id of the callback function.
										//			for a system event ("eEventNew","eEventDelete","eEventReName","eEventTrace") this value is "iRootObjectID" or "iApplicationID"
	void *pInputParameter;				//void *pInputParameter : fixed input parameter (it could be for example the pointer on an object).
										//			but if you don't need it, it is better to use 0 (to avoid too many callback function in the list).
	LARGE_INTEGER _llInputParameter;	//LARGE_INTEGER llInputParameter : fixed input parameter (it could NOT be for example the pointer on an object).
										//			but if you don't need it, it is better to use 0 (to avoid too many callback function in the list).
	bool bStatus;						//true to notify a modification of the status (not the value, that is enable and visible).
	bool bLimit;
	bool bRelative;						//bRelative : (only for eEventUpdate event) true if the updated item could be different than the one used to register the callback.
	bool bKernelReserved;
	unsigned int uiStatus;				//status information.
	unsigned int uiLimitStatus;			//limit status information.
	int iLimitOffset;
	int iLimitSize;
	structMaskStatus layerStatusMask;
	int iLayerSize;
	structLayer *pLayerIndex;
	const wchar_t *pPathDefaultLanguage;		//pPath : for eEventUpdate event, this is the path of the updated item,
										//			for user event it is available to transmit an input parameter (available also between two different application).
										//This is the default language (english).
	const wchar_t *pPathCurrentLanguage;		//The same translated in the current language.
	int iPathCurrentLanguage;				//current language (-1 is not autorized value).
	LARGE_INTEGER llCallbackID;//llCallbackID : callback function registration id.
	int iExtendParameterSize;//iParameterSize,pParameter : this is the in/out parameter (size and pointer, it is available also between two different application).
	void *pExtendParameterPtr;
	int iExtendParameterRootId;
	DWORD dwExtendParameterId;//unique identifier for extended parameters.
	CUTKernelRoot *pKernelRoot;//address of the root object.
//outputs
	pCallbackFunction pFunc;
	DWORD dwOffsetSharedCallback;
	bool bKernel;//if kernel this is not UserInterface!
	ATOM hCallbackComment;
} structCallbackParameter;

typedef struct UTKERNEL_API structCallbackDispatch
{
	structCallbackDispatch();
//outputs
	enumCallback eCallback;
	bool bPostMessage;
	bool bSendMessage;
	bool bViaThread;
	bool bWaitEnd;
	DWORD dwCallID;//each call have a unique identifier.
	DWORD dwWaitEndID;//used when the callback ask for Windows message.
	HWND hWnd;//used when the callback ask for Windows message.
	UINT msg;//used when the callback ask for Windows message.
	WPARAM wParam;//used when the callback ask for Windows message.
	LPARAM lParam;//used when the callback ask for Windows message.
	LRESULT lResult;
} structCallbackDispatch;
