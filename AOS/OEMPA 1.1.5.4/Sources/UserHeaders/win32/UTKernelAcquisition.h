
#pragma once
#pragma warning( disable : 4584 )


class CUTDisplayCursor;
class CUTDisplayZoom;
class CUTAxis;

class UTKERNEL_API_ALIGN _CUTEncoder
{
private:
	ULONGLONG m_ullPrivateMemory[313];
protected:

};

class UTKERNEL_API_ALIGN CUTEncoder :
	public limitCUTEncoder, 
	public CUTKernelRoot,
	public _CUTEncoder
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTEncoder();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTEncoder();
	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	CUTKernelRoot* cast_pCUTKernelRoot();
	int operator=(const CUTEncoder &a);
	bool operator==(const CUTEncoder &a);
	bool operator!=(const CUTEncoder &a);

	bool SetID(int iRootID);//to connect with a preexisting data in kernel database.
	bool SetName(const wchar_t *pName,int iRootID=-1);//to register this new data in the kernel database.
	bool FreeID();//to disconnect the data with the kernel database.


	int SetResolution(DWORD dwResolution);//value could be negative if encoder need to be inverted.
	DWORD GetResolution() const;

	int SetUnit(enumUnit eUnit);
	enumUnit GetUnit() const;

	int SetType(enumEncoderType eEncoderType);
	enumEncoderType GetType() const;

	int SetInternal(enumInternalMode eInternal);
	enumInternalMode GetInternal() const;

	int SetInvert(bool bInvert);
	bool GetInvert() const;

	int SetDigitalInput(enumIndexDigitalInput eEncoderInput,enumDigitalInput eDigitalInput);
	enumDigitalInput GetDigitalInput(enumIndexDigitalInput eEncoderInput) const;

	int SetSpeedDistance(double dValue);
	bool GetSpeedDistance(double &dValue) const;

	//Slow functions with event notification
	int SetInspectionHWValue(int iValue);
	int GetInspectionHWValue() const;
	int SetInspectionSWValue(double dValue);
	double GetInspectionSWValue() const;

	int SetInspectionSpeed(double dValue);
	double GetInspectionSpeed() const;

	int SetInspectionLength(double dValue);
	double GetInspectionLength() const;

	int SetInspectionCount(int iValue);
	int GetInspectionCount() const;

	bool SetEncoderId(int iID);
	bool SetEncoderId(wchar_t *pName);
	bool GetEncoderId(int iSize,wchar_t *pName) const;
	int GetEncoderId() const;

	//Quick functions with no event notification (used by the acquisition process).
	//usefull functions for user who want to manage HW encoder process.
	bool GetEncoderIndex(int &iEncoderIndex) const;
	static bool SetInspectionHWValue(int iEncoderIndex,int iValue);
	static bool GetInspectionHWValue(int iEncoderIndex,int &iValue);
	static bool SetInspectionSWValue(int iEncoderIndex,double dValue);
	static bool GetInspectionSWValue(int iEncoderIndex,double &dValue);

	bool UpdateInspectionRange();

	enumRootType GetRootType() const;

private:
	virtual int CUTEncoder::vf_GetReservedData();
};

class UTKERNEL_API_ALIGN _CUTAxis
{
private:
	ULONGLONG m_ullPrivateMemory[220];
protected:

};


class UTKERNEL_API_ALIGN CUTAxis :
	public limitCUTAxis, 
	public CUTKernelRoot,
	public _CUTAxis
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTAxis();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTAxis();
	CUTKernelRoot* cast_pCUTKernelRoot();
	CUTBaseClass* cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	int operator=(const CUTAxis &a);
	bool operator==(const CUTAxis &a);
	bool operator!=(const CUTAxis &a);

	bool SetID(int iRootID);//to connect with a preexisting data in kernel database.
	bool SetName(const wchar_t *pName,int iRootID=-1);//to register this new data in the kernel database.
	bool FreeID();//to disconnect the data with the kernel database.


	bool SetEncoderId(int iEncoderId);
	bool SetEncoderId(wchar_t *pName);
	bool GetEncoderId(int iSize,wchar_t *pName) const;
	int GetEncoderId() const;

	int SetAxisType(enumAxis eValue);
	enumAxis GetAxisType() const;

	int SetStart(double dValue);
	double GetStart() const;

	int SetStep(double dValue);
	double GetStep() const;
	void UpdateStepIncrement(int iEncoderIndex);
	bool CheckStep(double &dValue);//the step should be compatible with the encoder resolution.

	int SetCount(__int64 iCount);
	__int64 GetCount() const;

	int GetBitsize() const;
	static int GetBitsize(__int64 iCount);

//axis management for cursor and zoom
	int SetDisplayUnit(enumUnit eUnit);
	enumUnit GetDisplayUnit() const;

	int SetDisplayVelocity(double dValue);//normally only the axis Z required this data, so no need to share it for all axis.
	double GetDisplayVelocity() const;

	bool SetDisplayHalfPath(bool bEnable);//default : half path ON (see "m_CursorHalfPath.def").
	bool IsDisplayHalfPath() const;

	int SetDataType(enumDataType eDataType);//usefull only for amplitude axis.
	enumDataType GetDataType() const;
	bool IsSigned() const;
	bool IsBitwise() const;

	bool GetValueByUnit(double dValueUnit,__int64 &iValue) const;
	bool GetValueByUnit(__int64 iValue,double &dValueUnit) const;
	bool GetValueStringByUnit(__int64 iValue,int iDecimal,int iSize,wchar_t *pDisplay) const;
	bool GetValueByUnit(double dValueUnit,int &iValue) const;
	bool GetValueByUnit(int iValue,double &dValueUnit) const;
	bool GetValueStringByUnit(int iValue,int iDecimal,int iSize,wchar_t *pDisplay) const;

	bool SetDisplayVelocityId(wchar_t *pValue);
	bool SetDisplayVelocityId(int iAxisId);
	int GetDisplayVelocityId() const;
	bool GetDisplayVelocityId(int iSize,wchar_t *pName) const;

	bool UpdateInspectionIndex();//return false if out of range.
	void SetInspectionIndex(bool bOutOfRange,int iIndex);//return false if out of range.
	bool GetInspectionIndex(enumAcquisitionMode eAcqMode,float fOffset,short &iIndex);//return false if out of range.


	enumRootType GetRootType() const;

private:
	virtual int CUTAxis::vf_GetReservedData();
};


class CUTData;
class UTKERNEL_API_ALIGN _CUTData
{
private:
	ULONGLONG m_ullPrivateMemory[36];
protected:

};

class UTKERNEL_API_ALIGN CUTData :
	public CUTDataGeneric,
	public _CUTData
{

public:

	CUTData();
	CUTData(CUTKernelRoot *pRoot,enumRootType eRootType,limitCUTData *pLimit);
	~CUTData();
	CUTDataGeneric* cast_pCUTDataGeneric();
	int operator=(const CUTData &a);
	bool operator==(const CUTData &a);
	bool operator!=(const CUTData &a);

	bool Alloc();
	bool Free();
	bool AllocThisBuffer();
	bool FreeThisBuffer();
	bool IsAllocated() const;

	bool IsThisAllocated() const;
	bool AllocThisBuffer(int iHeaderBufferSize,int iDataBufferSize,bool &bCreateSharedData);//to open this acquisition buffer.
	bool FreeThisBuffer(bool bReopenInTheFuture);//to close this acquisition buffer.
	bool SetServer(bool bEnable);//by default this is a client.
	bool IsServer();

//to manage all application objects that have the same root ID.
	bool CallbackBufferReqFree();
	bool CallbackBufferReqAlloc();
	int GetChildAllocatedCount(int &iRootCount);

//FOR SERVER
	void EventRefresh(int iCode=0);//to dispatch (notify) a refresh event.
	bool Reset();


private:
	virtual int CUTData::vf_GetReservedData();
};

class UTKERNEL_API_ALIGN _CUTAcquisitionData
{
private:
	ULONGLONG m_ullPrivateMemory[277];
protected:

};

class UTKERNEL_API_ALIGN CUTAcquisitionData :
	public limitCUTAcquisitionData, 
	public CUTKernelRoot,
	public CUTData,
	public _CUTAcquisitionData
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTAcquisitionData();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTAcquisitionData();
	CUTData* cast_pCUTData();
	CUTKernelRoot* cast_pCUTKernelRoot();
	int operator=(const CUTAcquisitionData &a);
	bool operator==(const CUTAcquisitionData &a);
	bool operator!=(const CUTAcquisitionData &a);

//root ID management.
	bool SetID(int iRootID);//to connect with a preexisting data in kernel database.
	bool FreeID();//to disconnect the data with the kernel database.
	bool SetName(const wchar_t *pName,int iRootID=-1);//to register this new data in the kernel database.

//*****************SETTING PART*****************
	//Before open the acquisition buffer to store and/or get data, you need to specify :
	//- server/client feature.
	//- axis definition, you need first to define those axis (see CUTAxis).
	//- acquisition data type : ascan, cscan etc... see "enumDataType".
	//- the fill buffer : it is used to know the historic to store where the last acquisition data are available in the acquisition buffer.
	bool Alloc();//to open the acquisition buffer.
	bool IsAllocated() const;
	bool Free();
	bool AllocThisBuffer();
	bool SetServer(bool bEnable);//by default this is a client.


	//in the future... overwrite, maximum or minimum.
	int SetGenericAccess(enumGenericAccess eGenericAccess);
	enumGenericAccess GetGenericAccess() const;

	int SetAxisCount(int iAxisCount);
		//first axis is amplitude, axis 2 is X, axis 3 is Y and axis 4 is Z.
		//example :
		//stripchart : axis amplitude + X => count = 2.
		//Cscan : axis amplitude + X + Y => count = 3.
		//Ascan : axis amplitude + X + Y + Z => count = 4.
		//Bscan Dscan : count = 4.
	int GetAxisCount() const;
	__int64 GetAxisCount(enumAxis eAxis) const;
	bool SetAxisId(enumAxis eAxis,int iID);
	bool SetAxisId(enumAxis eAxis,wchar_t *pName);
	int GetAxisId(enumAxis eAxis) const;
	bool IsSigned();

	int SetHistoricSize(unsigned short usSize);//for on line application a size of 1 is enough.
	unsigned short GetHistoricSize() const;
	int GetHistoricDimEnc(enumAxis &eAxis1D) const;//1 (1D) or 2 (2D) (and 0 if error), 1 if no encoder (eStaticScan).
	int GetHistoricDimAcq(enumAxis &eAxis1D) const;//same but the NotNull is taken into accound.

	int GetHistoricNotNullCount(enumAxis eAxis) const;

	enumHistoricIndex GetHistoricIndex() const;

	void EnableDataBuffer(bool bEnable);
	bool IsDataBufferEnabled();


	bool TestPattern(unsigned short usValue);

	static bool GetAcquisitionDataList(int iSizeIn,int *aiListId,int &iSizeOut);//find in the kernel the list of all available acquisition data.

	enumRootType GetRootType() const;

private:
	virtual int CUTAcquisitionData::vf_GetReservedData();
};

class UTKERNEL_API_ALIGN _CUTDisplayCursor
{
private:
	ULONGLONG m_ullPrivateMemory[87];
protected:

};

class UTKERNEL_API_ALIGN CUTDisplayCursor :
	public limitCUTDisplayCursor,
	public CUTKernelRoot,
	public _CUTDisplayCursor
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTDisplayCursor();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTDisplayCursor();
	CUTKernelRoot* cast_pCUTKernelRoot();
	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	int operator=(const CUTDisplayCursor &a);
	bool operator==(const CUTDisplayCursor &a);
	bool operator!=(const CUTDisplayCursor &a);

	bool SetAxisId(wchar_t *pName);
	bool GetAxisId(int iSize,wchar_t *pName) const;
	bool SetAxisId(int iId);
	int GetAxisId() const;

	void SetEnable(bool bValue);
	bool IsEnabled() const;

	int SetValue(__int64 iValue);//unit is given by m_Unit;
	bool GetValue(__int64 &iValue) const;
	int SetValue(int iValue);//unit is given by m_Unit;
	bool GetValue(int &iValue) const;
	int SetValueByUnit(double dValueUnit);//the value is using unit from function "SetUnit" of CUTDisplayCursor (this is not international system unit).
	bool GetValueByUnit(double &dValueUnit) const;//the value is using unit from function "SetUnit" of CUTDisplayCursor (this is not international system unit).
	bool GetValueStringByUnit(int iDecimal,int iSize,wchar_t *pDisplay) const;

	int SetColor(COLORREF color);
	COLORREF GetColor() const;

	int SetOption(DWORD dwValue);
	DWORD GetOption() const;


	enumRootType GetRootType() const;

private:
	virtual int CUTDisplayCursor::vf_GetReservedData();
};

class UTKERNEL_API_ALIGN _CUTDisplayZoom
{
private:
	ULONGLONG m_ullPrivateMemory[102];
protected:

};

class UTKERNEL_API_ALIGN CUTDisplayZoom :
	public limitCUTDisplayZoom,
	public CUTKernelRoot,
	public _CUTDisplayZoom
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTDisplayZoom();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTDisplayZoom();
	CUTKernelRoot* cast_pCUTKernelRoot();
	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	int operator=(const CUTDisplayZoom &a);
	bool operator==(const CUTDisplayZoom &a);
	bool operator!=(const CUTDisplayZoom &a);

	bool SetAxisId(wchar_t *pName);
	bool GetAxisId(int iSize,wchar_t *pName) const;
	bool SetAxisId(int iId);
	int GetAxisId() const;

	void SetEnable(bool bValue);
	bool IsEnabled() const;

	//iIndex=0 for the left cursor of the zoom,
	//iIndex=1 for the right cursor of the zoom.
	int SetValue(int iIndex,__int64 iValue);//unit is given by m_Unit;
	bool GetValue(int iIndex,__int64 &iValue) const;
	bool SetValues(__int64 iValueLeft,__int64 iValueRight);
	bool GetValues(__int64 &iValueLeft,__int64 &iValueRight) const;
	int SetValue(int iIndex,int iValue);//unit is given by m_Unit;
	bool GetValue(int iIndex,int &iValue) const;
	bool GetValues(int &iValueLeft,int &iValueRight) const;
	int SetValueByUnit(int iIndex,double dValueUnit);//the value is using unit from function "SetUnit" of CUTDisplayZoom (this is not international system unit).
	bool GetValueByUnit(int iIndex,double &dValueUnit) const;//the value is using unit from function "SetUnit" of CUTDisplayZoom (this is not international system unit).
	bool GetValueStringByUnit(int iIndex,int iDecimal,int iSize,wchar_t *pDisplay) const;

	int SetColor(COLORREF color);
	COLORREF GetColor() const;

	int SetOption(DWORD dwValue);
	DWORD GetOption() const;


	enumRootType GetRootType() const;

private:
	virtual int CUTDisplayZoom::vf_GetReservedData();
};

class CUTDisplayScreen;
class UTKERNEL_API_ALIGN _CUTDisplayScreen
{
private:
	ULONGLONG m_ullPrivateMemory[178];
protected:

};

class UTKERNEL_API_ALIGN CUTDisplayScreen :
	public CUTBaseClass,
	public _CUTDisplayScreen
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTDisplayScreen();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTDisplayScreen();
	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	int operator=(const CUTDisplayScreen &a);
	bool operator==(const CUTDisplayScreen &a);
	bool operator!=(const CUTDisplayScreen &a);

	int SetPlacement(WINDOWPLACEMENT &placement);
	int GetPlacement(WINDOWPLACEMENT &placement) const;

	int SetLeft(int iValue);
	int GetLeft() const;

	int SetRight(int iValue);
	int GetRight() const;

	int SetTop(int iValue);
	int GetTop() const;

	int SetBottom(int iValue);
	int GetBottom() const;

	int SetShowCmd(int iValue);
	int GetShowCmd() const;

	int SetFlags(int iValue);
	int GetFlags() const;

	int SetPositionMinX(int iValue);
	int GetPositionMinX() const;

	int SetPositionMinY(int iValue);
	int GetPositionMinY() const;

	int SetPositionMaxX(int iValue);
	int GetPositionMaxX() const;

	int SetPositionMaxY(int iValue);
	int GetPositionMaxY() const;

	int SetPaletteCount(int iValue);
	int GetPaletteCount() const;

	int SetPalette(int iIndex,DWORD dwValue);
	DWORD GetPalette(int iIndex) const;

	int SetOptionCount(int iValue);
	int GetOptionCount() const;

	int SetOption(int iIndex,DWORD dwValue);
	DWORD GetOption(int iIndex) const;


private:
	virtual int CUTDisplayScreen::vf_GetReservedData();
};

class CUTDisplayView;
class UTKERNEL_API_ALIGN _CUTDisplayView
{
private:
	ULONGLONG m_ullPrivateMemory[506];
protected:

};

class UTKERNEL_API_ALIGN CUTDisplayView :
	public CUTBaseClass,
	public CUTDisplayScreen,
	public _CUTDisplayView
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTDisplayView();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTDisplayView();
	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	CUTDisplayScreen* cast_pCUTDisplayScreen();
	int operator=(const CUTDisplayView &a);
	bool operator==(const CUTDisplayView &a);
	bool operator!=(const CUTDisplayView &a);
	
	bool SetDataId(int iID);//Id of kernel object.
	bool SetDataId(wchar_t *pName);
	int GetDataId() const;
	bool GetDataId(int iSize,wchar_t *pName) const;

	int SetDisplayType(enumDisplayType eValue);//unit is given by m_Unit;
	enumDisplayType GetDisplayType() const;

	int SetTitle(wchar_t *pName);
	bool GetTitle(int iSize,wchar_t *pName) const;

	int VisibleTitle(bool bValue);
	bool IsVisibledTitle() const;

	int SetTitleColor(COLORREF color);
	COLORREF GetTitleColor() const;

	//to display an Ascan you need to specify cursor X and cursor Y
	//by default this is the real time display (no cursor is required).
	//axe X : it is possible to use either cursor either direct value but not both.
	int SetDataValueX(int iId);
	int SetDataValueX(wchar_t *pValue);
	int GetDataValueX() const;
	bool SetDataCursorXId(int iId);
	bool SetDataCursorXId(wchar_t *pName);
	int GetDataCursorXId() const;
	bool GetDataCursorXId(int iSize,wchar_t *pName) const;
	//axe Y : it is possible to use either cursor either direct value but not both.
	int SetDataValueY(int iId);
	int SetDataValueY(wchar_t *pValue);
	int GetDataValueY() const;
	bool SetDataCursorYId(int iId);
	bool SetDataCursorYId(wchar_t *pName);
	int GetDataCursorYId() const;
	bool GetDataCursorYId(int iSize,wchar_t *pName) const;

	//for each pane you can display cursor, 'H'orizontal and 'V'ertical.
	//Horizontal
	bool SetCursorHId(int iIndex,int iId);
	bool SetCursorHId(int iIndex,wchar_t *pName);
	int GetCursorHId(int iIndex) const;
	bool GetCursorHId(int iIndex,int iSize,wchar_t *pName) const;
	//Vertical
	bool SetCursorVId(int iIndex,int iId);
	bool SetCursorVId(int iIndex,wchar_t *pName);
	int GetCursorVId(int iIndex) const;
	bool GetCursorVId(int iIndex,int iSize,wchar_t *pName) const;

	int VisibleCursorH(int iIndex,bool bValue);
	bool IsVisibledCursorH(int iIndex) const;
	int VisibleCursorV(int iIndex,bool bValue);
	bool IsVisibledCursorV(int iIndex) const;

	//for each pane you can display zoom, 'H'orizontal and 'V'ertical.
	//Horizontal
	bool SetZoomHId(int iId);
	bool SetZoomHId(wchar_t *pName);
	int GetZoomHId() const;
	bool GetZoomHId(int iSize,wchar_t *pName) const;
	//Vertical
	bool SetZoomVId(int iId);
	bool SetZoomVId(wchar_t *pName);
	int GetZoomVId() const;
	bool GetZoomVId(int iSize,wchar_t *pName) const;
	//Amplitude
	bool SetZoomAId(int iId);
	bool SetZoomAId(wchar_t *pName);
	int GetZoomAId() const;
	bool GetZoomAId(int iSize,wchar_t *pName) const;
	bool SetZoomId(int iIndex,int iId);//index : 0=H, 1=V, 2=A
	bool SetZoomId(int iIndex,wchar_t *pName);//index : 0=H, 1=V, 2=A
	int GetZoomId(int iIndex) const;//index : 0=H, 1=V, 2=A
	bool GetZoomId(int iIndex,int iSize,wchar_t *pName) const;//index : 0=H, 1=V, 2=A

	int VisibleZoomH(bool bValue);
	bool IsVisibledZoomH() const;
	int VisibleZoomV(bool bValue);
	bool IsVisibledZoomV() const;
	int VisibleZoomA(bool bValue);
	bool IsVisibledZoomA() const;
	int VisibleZoom(int iIndex,bool bValue);//index : 0=H, 1=V, 2=A
	bool IsVisibledZoom(int iIndex) const;//index : 0=H, 1=V, 2=A

	//Splitter management
	int SetSplitterCount(int iValue);//minimum value is 1 ( the splitter row and col of a single view is (0,0) ).
	int GetSplitterCount();
	int SetSplitterRow(int iSplitterIndex,int iRow);//iSplitterIndex=0 is the highest level splitter.
	int GetSplitterRow(int iSplitterIndex) const;
	int SetSplitterColumn(int iSplitterIndex,int iColumn);
	int GetSplitterColumn(int iSplitterIndex) const;

	//void SetListId(int iValue);//unique Id of the view in the list (this is not an array of view but a list of view, the management of the index is different).
	int GetListId() const;


private:
	virtual int CUTDisplayView::vf_GetReservedData();
};



class UTKERNEL_API_ALIGN _CUTDisplayWindow
{
private:
	ULONGLONG m_ullPrivateMemory[37];
protected:

};

class UTKERNEL_API_ALIGN CUTDisplayWindow :
	public CUTBaseClass,
	public CUTDisplayScreen,
	public _CUTDisplayWindow
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTDisplayWindow();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTDisplayWindow();
	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	CUTDisplayScreen* cast_pCUTDisplayScreen();
	int operator=(const CUTDisplayWindow &a);
	bool operator==(const CUTDisplayWindow &a);
	bool operator!=(const CUTDisplayWindow &a);
	CUTDisplayView& operator[](int iIndex);
	const CUTDisplayView& cView(int iIndex) const;
	
	CUTDisplayView* GetView(int iListId);
	CUTDisplayView* AddView();
	bool RemoveView(int iListId);
	int GetCount() const;
	void GetSplitterSize(int iSizeIn,DWORD *pdwRowIn,DWORD *pdwColIn,int &iRowOut,int &iColOut);//get the size of a splitter which coordinates are inputs parameters of this function.

	int SetName(wchar_t *pTitle);
	int GetName(int iSize,wchar_t *pTitle) const;

	//void SetListId(int iValue);//unique Id of the view in the list (this is not an array of view but a list of view, the management of the index is different).
	int GetListId() const;


private:
	virtual int CUTDisplayWindow::vf_GetReservedData();
};



class UTKERNEL_API_ALIGN _CUTDisplayLayout
{
private:
	ULONGLONG m_ullPrivateMemory[213];
protected:

};

class UTKERNEL_API_ALIGN CUTDisplayLayout :
	public limitCUTDisplayLayout,
	public CUTKernelRoot,
	public _CUTDisplayLayout
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTDisplayLayout();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTDisplayLayout();
	CUTKernelRoot* cast_pCUTKernelRoot();
	int operator=(const CUTDisplayLayout &a);
	bool operator==(const CUTDisplayLayout &a);
	bool operator!=(const CUTDisplayLayout &a);
	CUTDisplayWindow& operator[](int iIndex);
	const CUTDisplayWindow& cWindow(int iIndex) const;
	CUTDisplayScreen* GetDisplayScreen();
	static CUTDisplayLayout* GetDisplayLayout();//to get the single layout of the running process (if any).

	CUTDisplayWindow* GetWindow(int iListId);
	CUTDisplayWindow* AddWindow();
	bool RemoveWindow(int iListId);
	int GetCount() const;

	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;


	enumRootType GetRootType() const;

private:
	virtual int CUTDisplayLayout::vf_GetReservedData();
};

#pragma warning( default : 4584 )
