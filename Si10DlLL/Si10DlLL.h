
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SI10DLLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SI10DLLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef SI10DLLL_EXPORTS
#define SI10DLLL_API __declspec(dllexport)
#else
#define SI10DLLL_API __declspec(dllimport)
#endif

#include ".\.\include\PLXAPI.h"
#include ".\.\include\Reg9030.h"
#include ".\.\Include\PlxIoctl.h"
//#include <afxmt.h>

#define PCI_DEVICE_ID 0x9030; // Values for PCI9030
#define PCI_VENDOR_ID 0x10B5; //   "     "     "

//Used in LoadFPGAConfigurationData()
#define FPGACfgFileNameVerChar "A"
#define FPGACfgFileNameRevChar "1"
#define FPGARevisionByte1 'A'
#define FPGARevisionByte2 '1'
#define FPGARevisionByte3 '0'
#define FPGAVarientByte   ' '



#define SI10ApiSuccess 512


// This class is exported from the Si10DlLL.dll
class SI10DLLL_API CSi10DlLL {
public:
	bool	m_bDevicePresent;
	PLX_DEVICE_OBJECT	m_Device;

	void Close();
	bool Open();
	CSi10DlLL(void);
	// TODO: add your methods here.
};

extern SI10DLLL_API int nSi10DlLL;

SI10DLLL_API int fnSi10DlLL(void);

