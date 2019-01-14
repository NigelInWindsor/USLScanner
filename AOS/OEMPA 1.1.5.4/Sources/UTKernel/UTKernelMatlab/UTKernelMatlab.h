// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the UTKERNELMATLAB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// UTKERNELMATLAB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef UTKERNELMATLAB_EXPORTS
#define UTKERNELMATLAB_API __declspec(dllexport)
#else
#define UTKERNELMATLAB_API __declspec(dllimport)
#endif

// This class is exported from the UTKernelMatlab.dll
class UTKERNELMATLAB_API CUTKernelMatlab {
public:
	CUTKernelMatlab(void);
	// TODO: add your methods here.
};

extern UTKERNELMATLAB_API int nUTKernelMatlab;

UTKERNELMATLAB_API int fnUTKernelMatlab(void);
