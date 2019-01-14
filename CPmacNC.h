// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CPmacNC wrapper class
#include "PCommServer Pro2 Library\Include\PmacServer.H"    // Accsee from Additional directory include of Project setting.

class CPmacNC : public COleDispatchDriver
{
public:
	CPmacNC(){} // Calls COleDispatchDriver default constructor
	CPmacNC(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CPmacNC(const CPmacNC& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IPmacNC methods
public:
	void Open(long Device, long Control, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PBOOL ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Device, Control, pbSuccess);
	}
	void Close(long Control)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control);
	}
	void OpenFile(long Control, long dwCoord, LPCTSTR szFilePath, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR VTS_PBOOL ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, dwCoord, szFilePath, pbSuccess);
	}
	void CloseFile(long Control, long dwCoord, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PBOOL ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, dwCoord, pbSuccess);
	}
	CString get_Title(long Control)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms, Control);
		return result;
	}
	void put_Title(long Control, LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	long get_DeviceNumber(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	BOOL get_IsOpen(long Control)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Control);
		return result;
	}
	void SetMachineMode(long Control, long mode)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, mode);
	}
	void GetMachineMode(long Control, long * pMode)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, pMode);
	}
	long get_mode(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	void put_mode(long Control, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	void GetRegistryBool(long Control, LPCTSTR section, LPCTSTR name, BOOL def, BOOL * pVal, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR VTS_BOOL VTS_PBOOL VTS_PBOOL ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, section, name, def, pVal, pbSuccess);
	}
	void SetRegistryBool(long Control, LPCTSTR section, LPCTSTR name, BOOL bVal, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR VTS_BOOL VTS_PBOOL ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, section, name, bVal, pbSuccess);
	}
	void GetRegistryLong(long Control, LPCTSTR section, LPCTSTR name, long def, long * pVal, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_PI4 VTS_PBOOL ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, section, name, def, pVal, pbSuccess);
	}
	void SetRegistryLong(long Control, LPCTSTR section, LPCTSTR name, long lVal, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR VTS_I4 VTS_PBOOL ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, section, name, lVal, pbSuccess);
	}
	void GetRegistryString(long Control, LPCTSTR section, LPCTSTR name, LPCTSTR def, BSTR * pVal, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_PBSTR VTS_PBOOL ;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, section, name, def, pVal, pbSuccess);
	}
	void SetRegistryString(long Control, LPCTSTR section, LPCTSTR name, LPCTSTR Val, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_PBOOL ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, section, name, Val, pbSuccess);
	}
	void GetRegistryDouble(long Control, LPCTSTR section, LPCTSTR name, double def, double * pVal, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR VTS_R8 VTS_PR8 VTS_PBOOL ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, section, name, def, pVal, pbSuccess);
	}
	void SetRegistryDouble(long Control, LPCTSTR section, LPCTSTR name, double Val, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR VTS_BSTR VTS_R8 VTS_PBOOL ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, section, name, Val, pbSuccess);
	}
	long get_CoordSystem(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	void put_CoordSystem(long Control, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	long get_NumCoordSystems(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	BOOL get_SingleBlock(long Control)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Control);
		return result;
	}
	void put_SingleBlock(long Control, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	BOOL get_BlockDelete(long Control)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Control);
		return result;
	}
	void put_BlockDelete(long Control, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	BOOL get_OptionalStop(long Control)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Control);
		return result;
	}
	void put_OptionalStop(long Control, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	BOOL get_MachineLock(long Control)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Control);
		return result;
	}
	void put_MachineLock(long Control, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	BOOL get_DryRun(long Control, long coord)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Control, coord);
		return result;
	}
	void put_DryRun(long Control, long coord, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, coord, newValue);
	}
	long get_MachineType(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	long get_JogSelect(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	void put_JogSelect(long Control, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	long get_SpeedSelect(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	void put_SpeedSelect(long Control, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	BOOL get_ProgramLoaded(long Control, long coord)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Control, coord);
		return result;
	}
	void GetAxisEnabled(long Control, long coord, short axis, BOOL * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I2 VTS_PBOOL ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, axis, pVal);
	}
	void GetProgramName(long Control, long coord, BOOL bMain, BSTR * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL VTS_PBSTR ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, bMain, pVal);
	}
	void GetProgramPath(long Control, long coord, BOOL bMain, BSTR * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL VTS_PBSTR ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, bMain, pVal);
	}
	long get_Semaphore(long Control, long coord)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x22, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control, coord);
		return result;
	}
	void put_Semaphore(long Control, long coord, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x22, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, coord, newValue);
	}
	void GetProgramStatus(long Control, long coord, long * pProgID, long * pNumLines, long * pLine, long * pParseLine, long * pRepeat, long * pCount, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PBOOL ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pProgID, pNumLines, pLine, pParseLine, pRepeat, pCount, pbSuccess);
	}
	void GetProgramNumber(long Control, long coord, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pVal);
	}
	void GetCurrentLabel(long Control, long coord, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pVal);
	}
	long get_DistanceSelect(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x26, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	void put_DistanceSelect(long Control, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x26, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	void GetJogStep(long Control, long coord, double * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PR8 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pVal);
	}
	void GetHandleStep(long Control, long coord, double * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PR8 ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pVal);
	}
	void SetOrigin(long Control, long coord, BOOL bOrginAll, long adr, BOOL bClear, double Val, BOOL metric)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL VTS_I4 VTS_BOOL VTS_R8 VTS_BOOL ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, bOrginAll, adr, bClear, Val, metric);
	}
	void ZeroShift(long Control, long coord, BOOL bShiftAll, long adr, BOOL bClear, double Val, BOOL bMetric)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL VTS_I4 VTS_BOOL VTS_R8 VTS_BOOL ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, bShiftAll, adr, bClear, Val, bMetric);
	}
	void GetVectorVelocity(long Control, long coord, double * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PR8 ;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pVal);
	}
	void ReadMdiBuffer(long Control, long coord, long * hDlg, long idControl)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, hDlg, idControl);
	}
	void LoadMdiBuffer(long Control, long coord, long * hDlg, long idControl, long nRepeat)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x2f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, hDlg, idControl, nRepeat);
	}
	void ClearTextBuffer(long Control, long coord)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x30, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord);
	}
	void GetCurrentGValue(long Control, long coord, long group, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x31, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, group, pVal);
	}
	long get_AxisSelect(long Control, long coord)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x32, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control, coord);
		return result;
	}
	void put_AxisSelect(long Control, long coord, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x32, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, coord, newValue);
	}
	void GetInposition(long Control, long coord, BOOL * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PBOOL ;
		InvokeHelper(0x33, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pVal);
	}
	long get_BufferMode(long Control, long coord)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x34, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control, coord);
		return result;
	}
	void put_BufferMode(long Control, long coord, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x34, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, coord, newValue);
	}
	void GetProgramMode(long Control, long coord, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x35, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pVal);
	}
	void GetSelectedBufferMode(long Control, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x36, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, pVal);
	}
	void GetSelectedAxisMotionMode(long Control, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x37, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, pVal);
	}
	void GetProgramMotionMode(long Control, long coord, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x38, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pVal);
	}
	void GetBufferOpen(long Control, BOOL * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_PBOOL ;
		InvokeHelper(0x39, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, pVal);
	}
	void GetBufferRemaining(long Control, long coord, long * pRemain, long * pParsed)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x3a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pRemain, pParsed);
	}
	long get_SpindleSelect(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	void put_SpindleSelect(long Control, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x3b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	long get_SpindleRPM(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	void put_SpindleRPM(long Control, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x3c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	double get_SpindleCSS(long Control)
	{
		double result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3d, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms, Control);
		return result;
	}
	void put_SpindleCSS(long Control, double newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_R8 ;
		InvokeHelper(0x3d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	BOOL get_SpindleCSSMode(long Control)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Control);
		return result;
	}
	void put_SpindleCSSMode(long Control, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x3e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	double get_SpindleCSSUnits(long Control)
	{
		double result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3f, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms, Control);
		return result;
	}
	void put_SpindleCSSUnits(long Control, double newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_R8 ;
		InvokeHelper(0x3f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	void GetSpindleActRPM(long Control, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x40, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, pVal);
	}
	long get_SpindleMaxRPM(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x41, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	void put_SpindleMaxRPM(long Control, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x41, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	long get_SpindleOverride(long Control, long coord)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x42, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control, coord);
		return result;
	}
	void put_SpindleOverride(long Control, long coord, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x42, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, coord, newValue);
	}
	double get_FeedrateOverride(long Control, long coord)
	{
		double result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x45, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms, Control, coord);
		return result;
	}
	void put_FeedrateOverride(long Control, long coord, double newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_R8 ;
		InvokeHelper(0x45, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, coord, newValue);
	}
	double get_RapidOverride(long Control, long coord)
	{
		double result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x46, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms, Control, coord);
		return result;
	}
	void put_RapidOverride(long Control, long coord, double newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_R8 ;
		InvokeHelper(0x46, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, coord, newValue);
	}
	long get_CoolantSelect(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x48, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	void put_CoolantSelect(long Control, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x48, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	long get_TimebaseMode(long Control, long coord)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x49, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control, coord);
		return result;
	}
	void put_TimebaseMode(long Control, long coord, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x49, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, coord, newValue);
	}
	void GetFeedrate(long Control, long coord, long * pMode, double * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 VTS_PR8 ;
		InvokeHelper(0x4a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pMode, pVal);
	}
	void SetFeedrate(long Control, long coord, long mode, double newVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_R8 ;
		InvokeHelper(0x4b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, mode, newVal);
	}
	double get_ThreadLead(long Control, long coord)
	{
		double result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x4c, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms, Control, coord);
		return result;
	}
	void put_ThreadLead(long Control, long coord, double newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_R8 ;
		InvokeHelper(0x4c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, coord, newValue);
	}
	void GetNumOfTools(long Control, long coord, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x4d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pVal);
	}
	long get_CurrentToolNumber(long Control, long coord)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x4e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control, coord);
		return result;
	}
	void put_CurrentToolNumber(long Control, long coord, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x4e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, coord, newValue);
	}
	long get_NextToolNumber(long Control, long coord)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x4f, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control, coord);
		return result;
	}
	void put_NextToolNumber(long Control, long coord, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x4f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, coord, newValue);
	}
	long get_CurrentToolHolderNumber(long Control, long coord)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x50, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control, coord);
		return result;
	}
	void put_CurrentToolHolderNumber(long Control, long coord, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x50, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, coord, newValue);
	}
	void SetToolHolderNumber(long Control, long coord, long tool, long holder)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x51, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, tool, holder);
	}
	void GetToolHolderNumber(long Control, long coord, long tool, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x52, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, tool, pVal);
	}
	void ZeroAllToolOffsets(long Control, long coord)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x53, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord);
	}
	void GetTypeToolOffset(long Control, long coord, long tool, LPCTSTR chAxis, long type, BOOL bMetric, double * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL VTS_PR8 ;
		InvokeHelper(0x54, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, tool, chAxis, type, bMetric, pVal);
	}
	void SetTypeToolOffset(long Control, long coord, long tool, LPCTSTR chAxis, long type, BOOL bMetric, BOOL bConfirm, double newVal, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL VTS_BOOL VTS_R8 VTS_PBOOL ;
		InvokeHelper(0x55, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, tool, chAxis, type, bMetric, bConfirm, newVal, pbSuccess);
	}
	long get_ToolInSpindle(long Control, long coord)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x56, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control, coord);
		return result;
	}
	void put_ToolInSpindle(long Control, long coord, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x56, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, coord, newValue);
	}
	long get_ToolDirection(long Control, long coord, long tool)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x57, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control, coord, tool);
		return result;
	}
	void put_ToolDirection(long Control, long coord, long tool, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x57, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, coord, tool, newValue);
	}
	double get_SpindleCountsPerRev(long Control)
	{
		double result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x58, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms, Control);
		return result;
	}
	void put_SpindleCountsPerRev(long Control, double newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_R8 ;
		InvokeHelper(0x58, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	double get_SpindleGearRatio(long Control)
	{
		double result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x59, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, parms, Control);
		return result;
	}
	void put_SpindleGearRatio(long Control, double newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_R8 ;
		InvokeHelper(0x59, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	BOOL get_SpindleDetect(long Control)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5a, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Control);
		return result;
	}
	void put_SpindleDetect(long Control, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x5a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	BOOL get_SpindleAtSpeed(long Control)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Control);
		return result;
	}
	void put_SpindleAtSpeed(long Control, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x5b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	BOOL get_SpindleAtZero(long Control)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Control);
		return result;
	}
	void put_SpindleAtZero(long Control, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x5c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	BOOL get_SpindleFPR(long Control)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Control);
		return result;
	}
	void put_SpindleFPR(long Control, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x5d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	BOOL get_HomeInit(long Control)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Control);
		return result;
	}
	void put_HomeInit(long Control, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x5e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	long get_HomeMotorMask(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x60, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	void put_HomeMotorMask(long Control, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x60, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	void GetActiveGCode(long Control, long coord, long group, double * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PR8 ;
		InvokeHelper(0x61, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, group, pVal);
	}
	void GetActiveGCodeStr(long Control, long coord, long group, BSTR * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PBSTR ;
		InvokeHelper(0x62, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, group, pVal);
	}
	void GetToolOffset(long Control, long coord, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x63, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pVal);
	}
	void GetCompOffset(long Control, long coord, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x64, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pVal);
	}
	void UpdateAllMotors(long Control)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x65, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control);
	}
	void GetOperatingTime(long Control, long * pDays, long * pHours, long * pMinutes, long * pSeconds)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x66, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, pDays, pHours, pMinutes, pSeconds);
	}
	void GetTotalCuttingTime(long Control, long * pDays, long * pHours, long * pMinutes, long * pSeconds)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x67, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, pDays, pHours, pMinutes, pSeconds);
	}
	void GetRunningTime(long Control, long * pDays, long * pHours, long * pMinutes, long * pSeconds)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x68, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, pDays, pHours, pMinutes, pSeconds);
	}
	void GetPartTime(long Control, long * pHours, long * pMinutes, long * pSeconds)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x69, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, pHours, pMinutes, pSeconds);
	}
	void GetCycleCuttingTime(long Control, long * pHours, long * pMinutes, long * pSeconds)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x6a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, pHours, pMinutes, pSeconds);
	}
	void GetCycleTime(long Control, long * pHours, long * pMinutes, long * Seconds)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x6b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, pHours, pMinutes, Seconds);
	}
	long get_PartsRequired(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	void put_PartsRequired(long Control, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x6c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	long get_PartsCount(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	void put_PartsCount(long Control, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x6d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	long get_PartsTotal(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x6e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	void put_PartsTotal(long Control, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x6e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	void GetNumDisplayedAxis(long Control, long coord, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x6f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pVal);
	}
	void GetNumDisplayAxisTotal(long Control, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x70, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, pVal);
	}
	void GetMotorPosStr(long Control, long coord, long motor, long t, BOOL bCombined, BSTR * pAdr, BSTR * pPos, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_PBSTR VTS_PBSTR VTS_PBOOL ;
		InvokeHelper(0x71, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, motor, t, bCombined, pAdr, pPos, pbSuccess);
	}
	BOOL get_IsSpindle(long Control, long coord, long motor)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x72, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Control, coord, motor);
		return result;
	}
	void put_IsSpindle(long Control, long coord, long motor, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x72, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, coord, motor, newValue);
	}
	BOOL get_IsDisplayed(long Control, long coord, long motor)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0x73, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Control, coord, motor);
		return result;
	}
	void put_IsDisplayed(long Control, long coord, long motor, BOOL newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_BOOL ;
		InvokeHelper(0x73, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, coord, motor, newValue);
	}
	void GetMotorUnitsStr(long Control, long coord, long motor, BSTR * pVal, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PBSTR VTS_PBOOL ;
		InvokeHelper(0x74, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, motor, pVal, pbSuccess);
	}
	void GetProgramLines(long Control, long coord, long start, long count, VARIANT * pVal, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT VTS_PBOOL ;
		InvokeHelper(0x75, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, start, count, pVal, pbSuccess);
	}
	void GetProgramLine(long Control, long coord, long lineNumber, BSTR * pVal, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PBSTR VTS_PBOOL ;
		InvokeHelper(0x76, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, lineNumber, pVal, pbSuccess);
	}
	void GetInputLong(long Control, long num, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x77, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, num, pVal);
	}
	void GetOutputLong(long Control, long num, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x78, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, num, pVal);
	}
	void GetCommandLong(long Control, long num, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x79, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, num, pVal);
	}
	void GetStatusLong(long Control, long num, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 ;
		InvokeHelper(0x7a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, num, pVal);
	}
	BOOL get_HomeInProgress(long Control)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7b, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, parms, Control);
		return result;
	}
	long get_SpindleCmdRPM(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	void command(long Control, long id, long iv, double fv, LPCTSTR bStr, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_R8 VTS_BSTR VTS_PBOOL ;
		InvokeHelper(0x7d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, id, iv, fv, bStr, pbSuccess);
	}
	void AddErrorRecord(long Control, long errnum, long em, long et, long Line, short ax, LPCTSTR Msg)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I2 VTS_BSTR ;
		InvokeHelper(0x7e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, errnum, em, et, Line, ax, Msg);
	}
	void GetParametricDouble(long Control, long coord, LPCTSTR region, long index, double * pVal, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_PR8 VTS_PBOOL ;
		InvokeHelper(0x7f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, region, index, pVal, pbSuccess);
	}
	void SetParametricDouble(long Control, long coord, LPCTSTR region, long index, double newVal, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR VTS_I4 VTS_R8 VTS_PBOOL ;
		InvokeHelper(0x80, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, region, index, newVal, pbSuccess);
	}
	void GetSelectedAxisChar(long Control, short * pAxChar)
	{
		static BYTE parms[] = VTS_I4 VTS_PI2 ;
		InvokeHelper(0x81, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, pAxChar);
	}
	long get_CurrentErrorLevel(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x82, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	void put_CurrentErrorLevel(long Control, long newValue)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x82, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, Control, newValue);
	}
	void GetAxisFormat(long Control, long coord, short ax, BOOL bMetric, long * pLen, long * pDecPlaces)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I2 VTS_BOOL VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x83, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, ax, bMetric, pLen, pDecPlaces);
	}
	void GetAxisFormatStr(long Control, long coord, short ax, BOOL bMetric, BSTR * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I2 VTS_BOOL VTS_PBSTR ;
		InvokeHelper(0x84, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, ax, bMetric, pVal);
	}
	void SetAxisFormat(long Control, long coord, short ax, BOOL bMetric, long len, long decPlaces)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I2 VTS_BOOL VTS_I4 VTS_I4 ;
		InvokeHelper(0x85, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, ax, bMetric, len, decPlaces);
	}
	void GetFirstError(long Control, BSTR * pHeader, BSTR * pMessage, BSTR * pCode, BSTR * pFilename, long * pCount, long * pChanged)
	{
		static BYTE parms[] = VTS_I4 VTS_PBSTR VTS_PBSTR VTS_PBSTR VTS_PBSTR VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x87, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, pHeader, pMessage, pCode, pFilename, pCount, pChanged);
	}
	void GetNextError(long Control, BSTR * pHeader, BSTR * pMessage, BSTR * pCode, BSTR * pFilename, BOOL * pbMore)
	{
		static BYTE parms[] = VTS_I4 VTS_PBSTR VTS_PBSTR VTS_PBSTR VTS_PBSTR VTS_PBOOL ;
		InvokeHelper(0x88, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, pHeader, pMessage, pCode, pFilename, pbMore);
	}
	void OffsetsUpdateClear(long Control, long clearMask)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x89, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, clearMask);
	}
	void OffsetsUpdate(long Control, long * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x8a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, pVal);
	}
	void ReadErrorMsgBoxText(long Control, long index, long * pCount, BSTR * pText, BSTR * pTitle)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 VTS_PBSTR VTS_PBSTR ;
		InvokeHelper(0x8b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, index, pCount, pText, pTitle);
	}
	void ResetMsgBoxError(long Control, long errRetVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x8c, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, errRetVal);
	}
	void SaveParametricVars(long Control)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control);
	}
	void LoadParametricVars(long Control)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control);
	}
	void ResetCycleTime(long Control)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x8f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control);
	}
	void GetToolGuageOffset(long Control, long coord, long tool, BOOL bMulti, short ax, BOOL bMetric, double * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_I2 VTS_BOOL VTS_PR8 ;
		InvokeHelper(0x90, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, tool, bMulti, ax, bMetric, pVal);
	}
	void SetToolGuageOffset(long Control, long coord, long tool, BOOL bMulti, short ax, BOOL bMetric, double newVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_I2 VTS_BOOL VTS_R8 ;
		InvokeHelper(0x91, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, tool, bMulti, ax, bMetric, newVal);
	}
	void AutoSetToolOffset(long Control, long coord, long tool, short axis)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I2 ;
		InvokeHelper(0x92, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, tool, axis);
	}
	void GetOffsetValue(long Control, long coord, short axis, long t, BOOL bMetric, double * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I2 VTS_I4 VTS_BOOL VTS_PR8 ;
		InvokeHelper(0x93, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, axis, t, bMetric, pVal);
	}
	void SetOffsetValue(long Control, long coord, short axis, long t, BOOL bMetric, BOOL bConfirm, double newVal, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I2 VTS_I4 VTS_BOOL VTS_BOOL VTS_R8 VTS_PBOOL ;
		InvokeHelper(0x94, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, axis, t, bMetric, bConfirm, newVal, pbSuccess);
	}
	void AutoSetWorkOffset(long Control, long coord, short axis, long t, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I2 VTS_I4 VTS_PBOOL ;
		InvokeHelper(0x95, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, axis, t, pbSuccess);
	}
	void SearchText(long Control, long coord, LPCTSTR text, BOOL bMatchCase, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR VTS_BOOL VTS_PBOOL ;
		InvokeHelper(0x96, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, text, bMatchCase, pbSuccess);
	}
	void SearchTextNext(long Control, long coord, LPCTSTR text, BOOL bMatchCase, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BSTR VTS_BOOL VTS_PBOOL ;
		InvokeHelper(0x97, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, text, bMatchCase, pbSuccess);
	}
	void GetBufferText(long Control, long coord, BOOL bMDI, BSTR * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL VTS_PBSTR ;
		InvokeHelper(0x98, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, bMDI, pVal);
	}
	void SetBufferText(long Control, long coord, long repeat, BOOL bMDI, LPCTSTR newVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_BSTR ;
		InvokeHelper(0x99, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, repeat, bMDI, newVal);
	}
	void GetProcessorClocks(double * pIO, double * pLA, double * pParse, double * pFormat, double * pWrite)
	{
		static BYTE parms[] = VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 VTS_PR8 ;
		InvokeHelper(0x9a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, pIO, pLA, pParse, pFormat, pWrite);
	}
	long get_CurrentErrorCount(long Control)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x9b, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control);
		return result;
	}
	long get_BlocksPerSecond(long Control, long coord)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x9c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms, Control, coord);
		return result;
	}
	void GetMetricInput(long Control, long coord, BOOL * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PBOOL ;
		InvokeHelper(0x9d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pVal);
	}
	void GetToolInfo(long Control, long coord, long * pCurrentTool, long * pNextTool, long * pHolder, long * pHolderTool)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 VTS_PI4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0x9e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pCurrentTool, pNextTool, pHolder, pHolderTool);
	}
	void GetToolData(long Control, long coord, long tool, VARIANT * pVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT ;
		InvokeHelper(0x9f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, tool, pVal);
	}
	void ZeroAllTypeToolOffsets(long Control, long coord, long type)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 ;
		InvokeHelper(0xa0, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, type);
	}
	void SearchLine(long Control, long coord, long Line, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PBOOL ;
		InvokeHelper(0xa1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, Line, pbSuccess);
	}
	void GetAxisMotor(long Control, long coord, short axis, long * pMotor, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I2 VTS_PI4 VTS_PBOOL ;
		InvokeHelper(0xa2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, axis, pMotor, pbSuccess);
	}
	void GetMotorAxis(long Control, long coord, long motor, short * pAxis, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PI2 VTS_PBOOL ;
		InvokeHelper(0xa3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, motor, pAxis, pbSuccess);
	}
	void GetMotorPos(long Control, long coord, long motor, long t, BSTR * pAdr, double * pPos, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_PBSTR VTS_PR8 VTS_PBOOL ;
		InvokeHelper(0xa4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, motor, t, pAdr, pPos, pbSuccess);
	}
	void GetAllAxisPos(long Control, long coord, long t, VARIANT * pvAdrArray, VARIANT * pvPosArray, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PVARIANT VTS_PVARIANT VTS_PBOOL ;
		InvokeHelper(0xa5, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, t, pvAdrArray, pvPosArray, pbSuccess);
	}
	void GetAllAxisPosStr(long Control, long coord, long t, BOOL bCoord, VARIANT * pvArray, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_PVARIANT VTS_PBOOL ;
		InvokeHelper(0xa6, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, t, bCoord, pvArray, pbSuccess);
	}
	void GetMaxFeedrates(long Control, long coord, double * pMaxPerRev, double * pMaxPerMinute)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PR8 VTS_PR8 ;
		InvokeHelper(0xa7, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pMaxPerRev, pMaxPerMinute);
	}
	void SetMaxFeedrates(long Control, long coord, double MaxPerRev, double MaxPerMinute)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_R8 VTS_R8 ;
		InvokeHelper(0xa8, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, MaxPerRev, MaxPerMinute);
	}
	void ClearOffsetValue(long Control, long coord, short axis, long t, BOOL bMetric, double newVal, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I2 VTS_I4 VTS_BOOL VTS_R8 VTS_PBOOL ;
		InvokeHelper(0xa9, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, axis, t, bMetric, newVal, pbSuccess);
	}
	void GetToolDataStruct(long Control, long coord, long tool, NCTOOL * pStruct)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_UNKNOWN ;
		InvokeHelper(0xaa, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, tool, pStruct);
	}
	void StartDataCollection(long Control, long resolution, long period, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_I4 VTS_PBOOL ;
		InvokeHelper(0xab, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, resolution, period, pbSuccess);
	}
	void StopDataCollection(long Control)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xac, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control);
	}
	void DataCollect(long Control, long coord, NCDATA * pData, BOOL * pbSuccess)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_UNKNOWN VTS_PBOOL ;
		InvokeHelper(0xad, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, pData, pbSuccess);
	}
	void ResetPartsCount(long Control)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xae, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control);
	}
	void ResetPartsTotal(long Control)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xaf, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control);
	}
	void GetWorkOffset(long Control, long coord, long * t, long * pExtendedVal)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PI4 VTS_PI4 ;
		InvokeHelper(0xb0, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Control, coord, t, pExtendedVal);
	}

	// IPmacNC properties
public:

};
