
#pragma once
#include <math.h>
#pragma warning( disable : 4584 )



class CUTProbe;

/////////////////////////////////////////////////////////////////////////////////////////////////
class CUTPoint;

class UTKERNEL_API_ALIGN _CUTCoordinate

{
private:
	ULONGLONG m_ullPrivateMemory[5];
protected:

};

class UTKERNEL_API_ALIGN CUTCoordinate :
	public _CUTCoordinate
{
public:
	CUTCoordinate(bool bPoint=true);
	CUTCoordinate(const CUTCoordinate &a);
	CUTCoordinate(bool bPoint,double x,double y,double z);

	~CUTCoordinate();
	const CUTCoordinate& operator=(const CUTCoordinate &a);

	const CUTCoordinate& operator=(const CUTPoint &a);
	const CUTCoordinate& operator+=(const CUTCoordinate &a);
	const CUTCoordinate& operator-=(const CUTCoordinate &a);
	const CUTCoordinate& operator*=(const double a);
	const CUTCoordinate& operator/=(const double a);
	const CUTCoordinate operator+(const CUTCoordinate &a);
	const CUTCoordinate operator-(const CUTCoordinate &a);
	const CUTCoordinate operator^(const CUTCoordinate &a);//Cross()
	const CUTCoordinate operator*(double a);
	const CUTCoordinate operator/(double a);
	bool operator==(const CUTCoordinate &a);
	bool operator!=(const CUTCoordinate &a);
	double angle();//polar system, result is between -PI and PI.
	double norm() const;//polar system.
	double norm2() const;
	void reset();


	void Set(enumAxis eAxis,const double dValue);
	double Get(enumAxis eAxis) const;

	void SetPrecision(double dValue);
	double GetPrecision() const;

	void EnablePoint();
	bool IsPoint() const;
	void EnableVector();
	bool IsVector() const;


};
UTKERNEL_API double GetPrecision();
UTKERNEL_API void SetPrecision(double dPrecision);


/////////////////////////////////////////////////////////////////////////////////////////////////

class UTKERNEL_API_ALIGN _CUTPoint
{
private:
	ULONGLONG m_ullPrivateMemory[45];
protected:

};

class UTKERNEL_API_ALIGN CUTPoint :
	public CUTBaseClass,
	public _CUTPoint
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTPoint();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTPoint();
	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	int operator=(const CUTPoint &a);
	bool operator==(const CUTPoint &a);
	bool operator!=(const CUTPoint &a);
	const CUTPoint& operator=(const CUTCoordinate &a);

	double GetX() const;
	double GetY() const;
	double GetZ() const;
	int SetX(double dValue);
	int SetY(double dValue);
	int SetZ(double dValue);

private:
	virtual int CUTPoint::vf_GetReservedData();
};


class CUTWizardSystem;
class CUTWizardAperture;
class CUTWizardVector;
class CUTWizardScan;
class CUTWizardFocalLaw;
class CUTReceiver;
class CUTAcquisitionInfo;
class CUTScan;
class CUTWizardProbe;

class UTKERNEL_API_ALIGN _CUTWizardWedge
{
private:
	ULONGLONG m_ullPrivateMemory[206];
protected:

};

class UTKERNEL_API_ALIGN CUTWizardWedge :
	public CUTBaseClass,
	public _CUTWizardWedge
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTWizardWedge();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTWizardWedge();
	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	int operator=(const CUTWizardWedge &a);
	bool operator==(const CUTWizardWedge &a);
	bool operator!=(const CUTWizardWedge &a);

	bool ReferentialChangePoint(enumReferential inReferential,CUTPoint &in,enumReferential outReferential,CUTPoint &out) const;

	bool SetEncoderId(enumAxis eAxis,wchar_t *pValue);
	bool SetEncoderId(enumAxis eAxis,int iId);
	int GetEncoderId(enumAxis eAxis) const;
	bool GetEncoderId(enumAxis eAxis,int iSize,wchar_t *pValue) const;

	//If the wedge is disabled then the probe is in contact with the part.
	int SetEnable(bool bValue);
	bool IsEnabled() const;

	int SetVelocity(double dValue);
	double GetVelocity() const;

//LOW LEVEL FUNCTION TO ACCESS PROBE POSITION (absolute position of the probe in the space).
//CUTCoordinate is used instead of CUTPoint because of polar coordinates,
//polar coordinates is not managed by CUTPoint.
	int SetProbeAxis(enumAxis eAxis,enumReferential eReferential,CUTCoordinate &vector);//if you use this function then 
	bool GetProbeAxis(enumAxis eAxis,enumReferential eReferential,CUTCoordinate &vector) const;
	bool GetWedgeAxis(enumAxis eAxis,enumReferential eReferential,CUTCoordinate &vector) const;//interface plane.

	//offset of the center of the probe.
	//for plane part and cylinder part you can use Cartesian coordinates
	int SetOffset(enumAxis eAxis,double dValue);//eAxis : eX, eY, eZ. eZ is for the height over the interface.
	bool GetOffset(enumAxis eAxis,double &dValue) const;//eAxis : eX, eY, eZ. eZ is for the height over the interface.
	bool GetProbeOffset(CUTPoint &point) const;
	bool GetWedgeOffset(CUTPoint &point) const;//same than probe offset except that the heigth is null.
	//for cylinder part you can use change the way to define the offset, use "CUTWizardSpecimen::SetOffsetType".
	enumOffsetType GetOffsetType() const;//see "CUTWizardSpecimen::SetOffsetType".
	int SetOffsetArcLength(double dValue);//in case "eArcLength", instead to use OffsetY the arc length with Z axis is used.
	bool GetOffsetArcLength(double &dValue) const;
	int SetOffsetAngle(double dValue);//in case "eAngle", instead to use OffsetY the angle with Z axis is used.
	bool GetOffsetAngle(double &dValue) const;

	bool GetProbePoint(enumReferential eReferential,int iIndex,CUTPoint& x) const;

//HIGH LEVEL FUNCTION TO ACCESS PROBE POSITION WITH A MECHANICAL WEDGE SPECIFICATION.
	//3 rotations of the probe are used :
	//	- to specify the wedge plane (=tangent plane of the probe on the center point).
	//		- first angle to rotate the wedge over the interface = skew of the wedge.
	//		- second angle to lift the main axis of the probe (center of the element 1) = wedge angle.
	//		- third to rotate the probe on its second axis = wedge tilt.
	int SetAngles(double dWedgeSkew,double dWedgeAngle,double dWedgeTilt);//rotation of the element axis of the probe.
	bool GetAngles(double &dWedgeSkew,double &dWedgeAngle,double &dWedgeTilt) const;


private:
	virtual int CUTWizardWedge::vf_GetReservedData();
};

class UTKERNEL_API_ALIGN _CUTWizardProbe
{
private:
	ULONGLONG m_ullPrivateMemory[344];
protected:

};

class UTKERNEL_API_ALIGN CUTWizardProbe :
	public CUTBaseClass,
	public CUTWizardWedge,
	public _CUTWizardProbe
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTWizardProbe();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTWizardProbe();
	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	CUTWizardWedge *cast_pCUTWizardWedge() const;
	const CUTWizardWedge *const_cast_pCUTWizardWedge() const;
	int operator=(const CUTWizardProbe &a);
	bool operator==(const CUTWizardProbe &a);
	bool operator!=(const CUTWizardProbe &a);
	CUTPoint& operator[](int iIndex);
	const CUTPoint& GetPoint(int iIndex) const;

///////////////////////////////////////////////////////////////////////
// SETTING
	int SetName(wchar_t *pName);
	int GetName(int iSize,wchar_t *pName) const;

	int SetWizardProbe(enumWizardProbe eWizardProbe);
	enumWizardProbe GetWizardProbe() const;

	int SetFrequency(double dValue);
	double GetFrequency() const;

	int GetCount() const;//total element count

	int SetRadius(double dValue);
	bool GetRadius(double &dValue) const;

	int SetGain(int iIndex,float fValue);
	bool GetGain(int iIndex,float &fValue) const;

	bool GetPoint(int iIndex,CUTPoint &x) const;
	bool SavePoint(const wchar_t *pFile) const;	//save points in a text file (same format than "LoadCustomPoint").

//Linear1D and Custom
	int SetCount(int iValue);

//Linear1D and Linear2D
	int SetPitch(double dValue);
	bool GetPitch(double &dValue) const;

//Linear2D only
	int SetCountLinear2D(int iCount1,int iCount2);
	bool GetCountLinear2D(int &iCount1,int &iCount2) const;

	int SetPitchLinear2D(double dValue);
	bool GetPitchLinear2D(double &dValue) const;

	bool GetPointLinear2D(int iIndex1,int iIndex2,CUTPoint &x) const;
	int GetIndexLinear2D(int iIndex1,int iIndex2) const;
	bool GetIndexLinear2D(int iIndex,int &iIndex1,int &iIndex2) const;

	bool SetXOrderInvert(bool bEnable);
	bool GetXOrderInvert() const;

//matrix (linear2D and custom) only
	bool SetSkew(double dValue);
	double GetSkew() const;

//custom only
	bool SetCustomPoint(int iIndex,CUTPoint &x);
	bool LoadCustomPoint(const wchar_t *pFile);	//load a text file in which points are saved.
												//format#1 is the following (instead to use the separator ";" it is also possible to use ","):
												//	[WizardProbe]
												//	Point.count=128
												//	[WizardProbe\Point:0]
												//	X=0.0
												//	Y=0.0
												//	Z=0.0
												//	[WizardProbe\Point:1]
												//	etc...
												//format#2 is the following (instead to use the separator ";" it is also possible to use ","):
												//	[Array]
												//	ElementNumber=128
												//	[Position]
												//	Element1=0.00;0.00;0.00
												//	Element2=0.00;0.00;0.00
												//	etc...

	int SetCoordinate(enumCoordinate eCoordinate);
	enumCoordinate GetCoordinate() const;
	int SetTRLReference(double dX,double dY,double dZ);
	bool GetTRLReference(double &dX,double &dY,double &dZ);

///////////////////////////////////////////////////////////////////////
//PROCESSING
	bool GetApertureCenter(CUTWizardAperture &aperture,CUTCoordinate &point) const;
	bool GetApertureCenter(int iSize,int *aiIndex,CUTCoordinate &point) const;//callback will be possible in the future.


private:
	virtual int CUTWizardProbe::vf_GetReservedData();
};

class CUTWizardSpecimen;
class UTKERNEL_API_ALIGN _CUTWizardSpecimen
{
private:
	ULONGLONG m_ullPrivateMemory[94];
protected:

};

class UTKERNEL_API_ALIGN CUTWizardSpecimen :
	public CUTBaseClass,
	public _CUTWizardSpecimen
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTWizardSpecimen();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTWizardSpecimen();
	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	int operator=(const CUTWizardSpecimen &a);
	bool operator==(const CUTWizardSpecimen &a);
	bool operator!=(const CUTWizardSpecimen &a);

	int SetSpecimen(enumSpecimen eSpecimen);
	enumSpecimen GetSpecimen() const;

	//for ePlane to specify focalisation refered to the back (and eTube in the future).
	int SetThickness(double dValue);
	bool GetThickness(double &dValue) const;

	//for eBar (and eTube in the future)
	int SetRadius(double dValue);
	bool GetRadius(double &dValue) const;

	int SetVelocity(enumWave eWave,double dValue);
	double GetVelocity(enumWave eWave) const;

	int SetOffsetType(enumOffsetType eValue);
	enumOffsetType GetOffsetType() const;

private:
	virtual int CUTWizardSpecimen::vf_GetReservedData();
};



class UTKERNEL_API_ALIGN _CUTWizardSystem
{
private:
	ULONGLONG m_ullPrivateMemory[130];
protected:

};

class UTKERNEL_API_ALIGN CUTWizardSystem :
	public limitCUTWizardSystem,
	public CUTKernelRoot,
	public _CUTWizardSystem
{

public:
	CUTWizardSystem();
	static bool IsDefaultUTParameters();//default value in configuration file.
	CUTKernelRoot* cast_pCUTKernelRoot();
	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	int operator=(const CUTWizardSystem &a);
	bool operator==(const CUTWizardSystem &a);
	bool operator!=(const CUTWizardSystem &a);
	CUTWizardProbe& operator[](int iIndex);
	CUTWizardProbe& Probe(int iIndex);//same than operator[]
	const CUTWizardProbe& cProbe(int iIndex) const;
	CUTWizardWedge& Wedge(int iIndex);
	const CUTWizardWedge& cWedge(int iIndex) const;
	CUTWizardSpecimen& Specimen();
	const CUTWizardSpecimen& cSpecimen() const;

	bool SetID(int iRootID);//to connect with a preexisting data in kernel database.
	bool FreeID();//to disconnect the data with the kernel database.
	bool SetName(const wchar_t *pName,int iRootID=-1);//to register this new data in the kernel database.


	int SetCount(int iValue);
	int GetCount() const;

	void EventRefresh();//to dispatch (signal) a refresh event.

	enumRootType GetRootType() const;

private:
	virtual int CUTWizardSystem::vf_GetReservedData();
};

class UTKERNEL_API_ALIGN _CUTWizardAperture
{
private:
	ULONGLONG m_ullPrivateMemory[77];
protected:

};

class UTKERNEL_API_ALIGN CUTWizardAperture :
	public CUTBaseClass,
	public _CUTWizardAperture
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTWizardAperture();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTWizardAperture();
	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	int operator=(const CUTWizardAperture &a);
	bool operator==(const CUTWizardAperture &a);
	bool operator!=(const CUTWizardAperture &a);

	int SetCount(int iValue);
	int GetCount() const;
	//for custom probe, you can also use "SetCustom(int iCount,int *aiIndexElement)" to set the count of element.

	bool GetProbeElementIndex(int iApertureElementIndex,int &iProbeElementIndex) const;
	bool GetProbeElementIndex(int iSize,int *aiElementIndex) const;

//Linear1D only
	int SetFirst(int iValue);
	bool GetFirst(int &iValue) const;

//Linear2D only
	int SetCountLinear2D(int iValue1,int iValue2);
	bool GetCountLinear2D(int &iValue1,int &iValue2) const;

	int SetFirstLinear2D(int iValue1,int iValue2);
	bool GetFirstLinear2D(int &iValue1,int &iValue2) const;

	int GetIndexLinear2D(int iIndex1,int iIndex2) const;
	bool GetIndexLinear2D(int iIndex,int &iIndex1,int &iIndex2) const;

//only for eCustom probe.
	int SetCustom(int iIndex,int iIndexElement);
	bool GetCustom(int iIndex,int &iIndexElement) const;
	int SetCustom(int iCount,int *aiIndexElement);
	bool GetCustom(int iCount,int *aiIndexElement) const;

//the following parameter is only for information, normally you don't need it.
	void SetMode(enumProbeMode eProbeMode);
	enumProbeMode GetMode() const;

	//you don't need this function.
	int SetWizardProbe(enumWizardProbe eWizardProbe);
	enumWizardProbe GetWizardProbe() const;


private:
	virtual int CUTWizardAperture::vf_GetReservedData();
};

class UTKERNEL_API_ALIGN _CUTWizardVector
{
private:
	ULONGLONG m_ullPrivateMemory[186];
protected:

};

class UTKERNEL_API_ALIGN CUTWizardVector :
	public CUTBaseClass,
	public CUTWizardAperture,
	public _CUTWizardVector
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTWizardVector();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTWizardVector();
	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	CUTWizardAperture& cast_CUTWizardAperture(bool bPulse);//Aperture(bool bPulse);
	const CUTWizardAperture& const_cast_CUTWizardAperture(bool bPulse) const;//const_cast_CUTWizardAperture(bool bPulse) const;
	int operator=(const CUTWizardVector &a);
	bool operator==(const CUTWizardVector &a);
	bool operator!=(const CUTWizardVector &a);

	int SetFocalLawCount(int iValue);//reception depth count (dynamic depth focusing)
	int GetFocalLawCount() const;

	int SetDepthEmission(double dValue);//single emission depth
	double GetDepthEmission() const;

	int SetDepth(double dValue);
	double GetDepth() const;

	int SetDepth(int iIndex,double dValue);
	double GetDepth(int iIndex) const;

	int SetAngle(double dValue);
	double GetAngle() const;

	int SetSkew(double dValue);
	double GetSkew() const;

	int SetGeometricOffset1(double dValue);//only for eCustom probe. What is the reference : the probe, the part, the wedge ? maybe the part (scan axis).
	double GetGeometricOffset1() const;

	int SetGeometricOffset2(double dValue);//only for eCustom probe. What is the reference : the probe, the part, the wedge ? maybe the part (scan axis).
	double GetGeometricOffset2() const;


private:
	virtual int CUTWizardVector::vf_GetReservedData();
};

class UTKERNEL_API_ALIGN _CUTWizardFocalLaw
{
private:
	ULONGLONG m_ullPrivateMemory[348];
protected:

};

class UTKERNEL_API_ALIGN CUTWizardFocalLaw :
	public CUTBaseClass,
	public _CUTWizardFocalLaw
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTWizardFocalLaw();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTWizardFocalLaw();
	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	int operator=(CUTWizardFocalLaw &a);
	bool operator==(CUTWizardFocalLaw &a);
	bool operator!=(CUTWizardFocalLaw &a);

	CUTWizardScan *cast_pWizardScan() const;
	const CUTWizardSystem *cast_cpWizardSystem();
	CUTWizardProbe *cast_pWizardProbe();

	int SetCount(int iElementCount);
	int GetCount() const;
	bool SetElementIndex(int iIndex,int iElementIndex);
	bool GetElementIndex(int iIndex,int &iElementIndex) const;

	int SetFocalLawCount(int iElementCount,int iFocalLawCount);
	bool GetFocalLawCount(int &iElementCount,int &iFocalLawCount) const;

//Points are given in the referential of the wedge (default referential),
//you can change the referential of result points with the function "SetResultReferential".
	//Functions available for single focal point (emission, no dff) or for the deeper focal point.
	bool SetCenter(enumReferential eTarget,CUTPoint &point);
	bool GetCenter(enumReferential eTarget,CUTPoint &point);
	//Functions available with FocalLaw.
	bool SetCenter(enumReferential eTarget,int iFocalLaw,CUTPoint &point);
	bool GetCenter(enumReferential eTarget,int iFocalLaw,CUTPoint &point);

	//emergence point
	bool SetInterfaceCenter(CUTPoint &point);//last focal law.
	bool GetInterfaceCenter(CUTPoint &point) const;
		//BE CAREFUL : before calling "GetInterfaceCenter", please call "UpdateScan" one time previously for safe call.
	bool SetInterfaceCenter(int iFocalLaw,CUTPoint &point);
	bool GetInterfaceCenter(int iFocalLaw,CUTPoint &point) const;
		//BE CAREFUL : before calling "GetInterfaceCenter", please call "UpdateScan" one time previously for safe call.
	//Functions available for single focal point (emission, no dff) or for the deeper focal point.
	bool SetInterfacePoint(int iElementIndex,CUTPoint &point);
	bool GetInterfacePoint(int iElementIndex,CUTPoint &point) const;
		//BE CAREFUL : before calling "GetInterfacePoint", please call "UpdateScan" one time previously for safe call.
	//Functions available with FocalLaw.
	bool SetInterfacePoint(int iElementIndex,int iFocalLaw,CUTPoint &point);
	bool GetInterfacePoint(int iElementIndex,int iFocalLaw,CUTPoint &point) const;
		//BE CAREFUL : before calling "GetInterfacePoint", please call "UpdateScan" one time previously for safe call.

	void GetFocal(int iFocalLawIndex,CUTPoint &focal) const;

	void SetPrecision(double dPrecision);//in meter
	double GetPrecision();//in meter

//You can change the default referential with following functions.
	void SetResultReferential(enumReferential eReferential);
	enumReferential GetResultReferential();

//Results function
	double GetTimeOfFlight(int iFocalLawIndex) const;
	double GetCenterDelay(int iFocalLawIndex) const;
	double GetWedgeDelay() const;
	double GetSkew() const;
	double GetAngle() const;
	bool GetDelay(int iSize,float *pfDelay,float &fCenterDelay,float &fTofMax) const;//for single focal law or if FocalLaw for the last focal point.
	bool GetDelay(int iFocalLawIndex,int iSize,float *pfDelay,float &fCenterDelay,float &fTofMax) const;
	bool GetVelocity(bool &bWedgeEnable,double &dVelocityWedge,double &dVelocitySpecimen) const;

	bool SetTRLWedgeDelaySize(int iSize);
	int GetTRLWedgeDelaySize() const;
	bool SetTRLWedgeDelayRef(int iIndex,double dTime);
	bool GetTRLWedgeDelayRef(int iIndex,double &dTime) const;
	bool SetTRLWedgeDelayReal(int iIndex,double dTime);
	bool GetTRLWedgeDelayReal(int iIndex,double &dTime) const;
	bool EnableTRL(bool bEnable);
	bool IsTRLEnabled() const;
	bool SetInterfaceCenterTRL(double dX,double dY,double dZ);
	bool GetInterfaceCenterTRL(double &dX,double &dY,double &dZ) const;


	bool UpdateScan();
};



class CUTWizardAcquisition;
class UTKERNEL_API_ALIGN _CUTWizardAcquisition
{
private:
	ULONGLONG m_ullPrivateMemory[146];
protected:

};

class UTKERNEL_API_ALIGN CUTWizardAcquisition :
	public CUTBaseClass,
	public _CUTWizardAcquisition
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTWizardAcquisition();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTWizardAcquisition();
	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	int operator=(const CUTWizardAcquisition &a);
	bool operator==(const CUTWizardAcquisition &a);
	bool operator!=(const CUTWizardAcquisition &a);

	int SetLinkType(enumAcquisitionWizardMode eLinkType);
	enumAcquisitionWizardMode GetLinkType() const;

	int SetAcqName(wchar_t *pValue);
	bool GetAcqName(int iSize,wchar_t *pValue) const;

	bool SetAcqId(wchar_t *pValue);
	bool SetAcqId(int iAxisId);
	int GetAcqId() const;
	bool GetAcqId(int iSize,wchar_t *pName) const;

	int SetType(enumAcquisitionDataType eType);
	enumAcquisitionDataType GetType() const;

	int SetData(enumAcquisitionDataMode eData);
	enumAcquisitionDataMode GetData() const;

	int SetMode(enumAxis eAxis,enumAcquisitionEncoderMode eMode);
	bool GetMode(enumAxis eAxis,enumAcquisitionEncoderMode &eMode) const;

	int SetGenericAccess(enumGenericAccess eGenericAccess);
	enumGenericAccess GetGenericAccess() const;

	bool CreateAxis(enumDataType eDataType,int iCountA,int iLawCount,int iPointCount,enumWave eWave);
	int CreateAcquisitionData(int iSeparateIndex,int iSize,wchar_t *pName,CUTScan *pScan,float &fStepX,float &fStepY);
	bool SetAcquisitionInfo(int iReplay,int iAcqId,int iIndex,float fOffsetX,float fOffsetY,float fStepX,float fStepY,CUTAcquisitionInfo *pAcquisitionInfo) const;


private:
	virtual int CUTWizardAcquisition::vf_GetReservedData();
};



class CUTWizardScan;
class UTKERNEL_API_ALIGN _CUTWizardScan
{
private:
	ULONGLONG m_ullPrivateMemory[1059];

public:

};

class UTKERNEL_API_ALIGN CUTWizardScan :
	public CUTBaseClass,
	public _CUTWizardScan
{

public:
#ifdef CONSTRUCTOR_NO_PARAMETER
	CUTWizardScan();
#endif CONSTRUCTOR_NO_PARAMETER
	~CUTWizardScan();
	CUTBaseClass *cast_pCUTBaseClass();
	const CUTBaseClass& const_cast_CUTBaseClass() const;
	int operator=(const CUTWizardScan &a);
	bool operator==(const CUTWizardScan &a);
	bool operator!=(const CUTWizardScan &a);
	CUTWizardAcquisition& WizardAcquisition(int iIndex);
	const CUTWizardFocalLaw& cWizardFocalLaw(bool bPulse,int iIndex);
	CUTWizardFocalLaw& WizardFocalLaw(bool bPulse,int iIndex);
	CUTProbe* Probe();
	CUTWizardVector& VectorStart();
	CUTWizardVector& VectorStop();
	CUTWizardVector& VectorStep();

	int SetWizardAcqCount(int iCount);
	int GetWizardAcqCount() const;

	int SetWave(enumWave eValue);
	enumWave GetWave() const;

	int SetScanPriority(enumScan eScan,int iOrder);//if iOrder=0 then scan is disabled. iOrder=1 is the scan of the greater priority.
	int GetScanPriority(enumScan eScan) const;

	int SetDepthMode(enumDepthMode eFocalMode);
	enumDepthMode GetDepthMode() const;

	int GetVectorCount(bool bPulse) const;
	bool GetVector(bool bPulse,int iIndex,CUTWizardVector &x) const;

	int GetCount();//Focal law count for pulse (emitter) or echo (reception).

	bool SetScanStatusOutOfDate();
	bool IsScanStatusUpToDate();
	bool UpdateScan(int iErrorSize=0,wchar_t *wcError=NULL);//to dispatch (signal) a refresh event.


private:
	virtual int CUTWizardScan::vf_GetReservedData();
};

/////////////////////////////////////////////////////////////////////////////////////////////////
#pragma warning( default : 4584 )
