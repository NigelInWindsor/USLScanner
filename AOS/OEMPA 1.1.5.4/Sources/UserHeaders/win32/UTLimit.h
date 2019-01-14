
#pragma once

#include <limits.h>
#include <float.h>
#pragma warning( disable : 4584 )


const COLORREF colorCursorDefault=RGB(0xff,0,0);
const COLORREF colorZoomDefault=RGB(0,0,0xff);
const COLORREF colorTitleDefault=RGB(0,0xff,0);
class UTKERNEL_API_ALIGN _limitCUTData
{
private:
	ULONGLONG m_ullPrivateMemory[24];
public:

};

class UTKERNEL_API_ALIGN limitCUTData :
	public limitCUTBaseClass,
	public _limitCUTData
{
public:

};

///////////////////////////////////////////////////////////////////////////////////////
// LIMIT CLASSES
///////////////////////////////////////////////////////////////////////////////////////

class UTKERNEL_API_ALIGN _limitCUTFocalLaw
{
private:
	ULONGLONG m_ullPrivateMemory[653];
public:

};

class UTKERNEL_API_ALIGN limitCUTFocalLaw :
	public limitCUTBaseClass,
	public _limitCUTFocalLaw
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTPulser
{
private:
	ULONGLONG m_ullPrivateMemory[82];
public:

};

class UTKERNEL_API_ALIGN limitCUTPulser :
	public limitCUTBaseClass,
	public limitCUTFocalLaw,
	public _limitCUTPulser
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTAcquisitionInfo
{
private:
	ULONGLONG m_ullPrivateMemory[172];
public:

};

class UTKERNEL_API_ALIGN limitCUTAcquisitionInfo :
	public limitCUTBaseClass,
	public _limitCUTAcquisitionInfo
{

};

class UTKERNEL_API_ALIGN _limitCUTGate
{
private:
	ULONGLONG m_ullPrivateMemory[697];
public:

};

class UTKERNEL_API_ALIGN limitCUTGate :
	public limitCUTBaseClass,
	public _limitCUTGate
{
public:

};



class UTKERNEL_API_ALIGN _limitCUTReceiver
{
private:
	ULONGLONG m_ullPrivateMemory[763];
public:

};

class UTKERNEL_API_ALIGN limitCUTReceiver :
	public limitCUTBaseClass,
	public limitCUTFocalLaw,
	public limitCUTGate,
	public _limitCUTReceiver
{
public:

};



class UTKERNEL_API_ALIGN _limitCUTPulserReceiver
{
private:
	ULONGLONG m_ullPrivateMemory[2302];
public:

};

class UTKERNEL_API_ALIGN limitCUTPulserReceiver :
	public limitCUTBaseClass,
	public limitCUTPulser,
	public _limitCUTPulserReceiver
{
public:

};



class UTKERNEL_API_ALIGN _limitCUTScan
{
private:
	ULONGLONG m_ullPrivateMemory[3165];
public:

};

class UTKERNEL_API_ALIGN limitCUTScan :
	public limitCUTBaseClass,
	public limitCUTWizardScan,//for pulse/echo
	public _limitCUTScan
{
public:

};



class UTKERNEL_API_ALIGN _limitCUTProbe
{
private:
	ULONGLONG m_ullPrivateMemory[6469];
public:

};

class UTKERNEL_API_ALIGN limitCUTProbe :
	public limitCUTBaseClass,
	public _limitCUTProbe
{
public:

};



class UTKERNEL_API_ALIGN _limitCUTChannelsSimulator
{
private:
	ULONGLONG m_ullPrivateMemory[50];
public:

};

class UTKERNEL_API_ALIGN limitCUTChannelsSimulator :
	public limitCUTBaseClass,
	public _limitCUTChannelsSimulator
{
public:

};

///////////////////////////////////////////////////////////////////

class UTKERNEL_API_ALIGN _limitCUTFilterOEMPA
{
private:
	ULONGLONG m_ullPrivateMemory[94];
public:

};

class UTKERNEL_API_ALIGN limitCUTFilterOEMPA :
	public limitCUTBaseClass,
	public _limitCUTFilterOEMPA
{
public:

};



class UTKERNEL_API_ALIGN _limitCUTDeviceOEMPA
{
private:
	ULONGLONG m_ullPrivateMemory[741];
public:

};

class UTKERNEL_API_ALIGN limitCUTDeviceOEMPA :
	public limitCUTBaseClass,
	public _limitCUTDeviceOEMPA
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTDeviceMonitor
{
private:
	ULONGLONG m_ullPrivateMemory[126];
public:

};

class UTKERNEL_API_ALIGN limitCUTDeviceMonitor :
	public limitCUTBaseClass,
	public _limitCUTDeviceMonitor
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTDevice
{
private:
	ULONGLONG m_ullPrivateMemory[1218];
public:

};

class UTKERNEL_API_ALIGN limitCUTDevice :
	public limitCUTKernelRoot,
	public limitCUTData,
	public limitCUTDeviceMonitor,
	public _limitCUTDevice
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTChannels
{
private:
	ULONGLONG m_ullPrivateMemory[6744];
public:

};

class UTKERNEL_API_ALIGN limitCUTChannels :
	public limitCUTKernelRoot,
	public limitCUTData,
	public _limitCUTChannels
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTModule
{
private:
	ULONGLONG m_ullPrivateMemory[77];
public:

};

class UTKERNEL_API_ALIGN limitCUTModule :
	public limitCUTBaseClass,
	public _limitCUTModule
{
public:

};



class UTKERNEL_API_ALIGN _limitCUTPreference
{
private:
	ULONGLONG m_ullPrivateMemory[311];
public:

};

class UTKERNEL_API_ALIGN limitCUTPreference :
	public limitCUTKernelRoot,
	public _limitCUTPreference
{
public:

};



/////////////////////////////////////////////////////////////////////////////////////

class UTKERNEL_API_ALIGN _limitCUTEncoder
{
private:
	ULONGLONG m_ullPrivateMemory[350];
public:

};

class UTKERNEL_API_ALIGN limitCUTEncoder :
	public limitCUTKernelRoot,
	public _limitCUTEncoder
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTAxis
{
private:
	ULONGLONG m_ullPrivateMemory[248];
public:

};

class UTKERNEL_API_ALIGN limitCUTAxis :
	public limitCUTKernelRoot,
	public _limitCUTAxis
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTAcquisitionData
{
private:
	ULONGLONG m_ullPrivateMemory[324];
public:

};

class UTKERNEL_API_ALIGN limitCUTAcquisitionData :
	public limitCUTKernelRoot,
	public limitCUTData,
	public _limitCUTAcquisitionData
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTDisplayCursor
{
private:
	ULONGLONG m_ullPrivateMemory[88];
public:

};

class UTKERNEL_API_ALIGN limitCUTDisplayCursor :
	public limitCUTKernelRoot,
	public _limitCUTDisplayCursor
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTDisplayZoom
{
private:
	ULONGLONG m_ullPrivateMemory[104];
public:

};

class UTKERNEL_API_ALIGN limitCUTDisplayZoom :
	public limitCUTKernelRoot,
	public _limitCUTDisplayZoom
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTDisplayScreen
{
private:
	ULONGLONG m_ullPrivateMemory[232];
public:

};

class UTKERNEL_API_ALIGN limitCUTDisplayScreen :
	public limitCUTBaseClass,
	public _limitCUTDisplayScreen
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTDisplayView
{
private:
	ULONGLONG m_ullPrivateMemory[607];
public:

};

class UTKERNEL_API_ALIGN limitCUTDisplayView :
	public limitCUTBaseClass,
	public limitCUTDisplayScreen,
	public _limitCUTDisplayView
{
public:

};



class UTKERNEL_API_ALIGN _limitCUTDisplayWindow
{
private:
	ULONGLONG m_ullPrivateMemory[907];
public:

};

class UTKERNEL_API_ALIGN limitCUTDisplayWindow :
	public limitCUTBaseClass,
	public limitCUTDisplayScreen,
	public _limitCUTDisplayWindow
{
public:

};



class UTKERNEL_API_ALIGN _limitCUTDisplayLayout
{
private:
	ULONGLONG m_ullPrivateMemory[1436];
public:

};

class UTKERNEL_API_ALIGN limitCUTDisplayLayout :
	public limitCUTKernelRoot,
	public _limitCUTDisplayLayout
{
public:

};
