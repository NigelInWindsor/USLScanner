// UTKernelMatlab.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "UTKernelMatlab.h"


// This is an example of an exported variable
UTKERNELMATLAB_API int nUTKernelMatlab=0;

// This is an example of an exported function.
UTKERNELMATLAB_API int fnUTKernelMatlab(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see UTKernelMatlab.h for the class definition
CUTKernelMatlab::CUTKernelMatlab()
{
	return;
}
