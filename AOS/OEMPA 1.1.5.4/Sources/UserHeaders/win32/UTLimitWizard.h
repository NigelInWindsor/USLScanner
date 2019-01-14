
#pragma once
#pragma warning( disable : 4584 )
#include <math.h>

/////////////////////////////////////////////////////////////////////////////////////////////////
class UTKERNEL_API_ALIGN _limitCUTPoint
{
private:
	ULONGLONG m_ullPrivateMemory[49];
public:

};

class UTKERNEL_API_ALIGN limitCUTPoint :
	public limitCUTBaseClass,
	public _limitCUTPoint
{
public:

};

/////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////

class UTKERNEL_API_ALIGN _limitCUTWizardSpecimen
{
private:
	ULONGLONG m_ullPrivateMemory[106];
public:

};

class UTKERNEL_API_ALIGN limitCUTWizardSpecimen :
	public limitCUTBaseClass,
	public _limitCUTWizardSpecimen
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTWizardWedge
{
private:
	ULONGLONG m_ullPrivateMemory[224];
public:

};

class UTKERNEL_API_ALIGN limitCUTWizardWedge :
	public limitCUTBaseClass,
	public _limitCUTWizardWedge
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTWizardProbe
{
private:
	ULONGLONG m_ullPrivateMemory[504];
public:

};

class UTKERNEL_API_ALIGN limitCUTWizardProbe :
	public limitCUTBaseClass,
	public limitCUTWizardWedge,
	public _limitCUTWizardProbe
{
public:

};



class UTKERNEL_API_ALIGN _limitCUTWizardSystem
{
private:
	ULONGLONG m_ullPrivateMemory[888];
public:

};

class UTKERNEL_API_ALIGN limitCUTWizardSystem :
	public limitCUTKernelRoot,
	public _limitCUTWizardSystem
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTWizardAperture
{
private:
	ULONGLONG m_ullPrivateMemory[99];
public:

};

class UTKERNEL_API_ALIGN limitCUTWizardAperture :
	public limitCUTBaseClass,
	public _limitCUTWizardAperture
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTWizardVector
{
private:
	ULONGLONG m_ullPrivateMemory[243];
public:

};

class UTKERNEL_API_ALIGN limitCUTWizardVector :
	public limitCUTBaseClass,
	public limitCUTWizardAperture,
	public _limitCUTWizardVector
{
public:

};

class UTKERNEL_API_ALIGN limitCUTWizardVectorStep : public limitCUTWizardVector
{
public:

};

class UTKERNEL_API_ALIGN _limitCUTWizardFocalLaw
{
private:
	ULONGLONG m_ullPrivateMemory[764];
public:

};

class UTKERNEL_API_ALIGN limitCUTWizardFocalLaw :
	public limitCUTBaseClass,
	public _limitCUTWizardFocalLaw
{
public:

};



class UTKERNEL_API_ALIGN _limitCUTWizardAcquisition
{
private:
	ULONGLONG m_ullPrivateMemory[171];
public:

};

class UTKERNEL_API_ALIGN limitCUTWizardAcquisition :
	public limitCUTBaseClass,
	public _limitCUTWizardAcquisition
{
public:

};



class UTKERNEL_API_ALIGN _limitCUTWizardScan
{
private:
	ULONGLONG m_ullPrivateMemory[3086];
public:

};

class UTKERNEL_API_ALIGN limitCUTWizardScan :
	public limitCUTBaseClass,
	public _limitCUTWizardScan
{
public:

};

/////////////////////////////////////////////////////////////////////////////////////////////////
#pragma warning( default : 4584 )
