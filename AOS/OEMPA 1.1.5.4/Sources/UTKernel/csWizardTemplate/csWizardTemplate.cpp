#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <gcroot.h>
#include <math.h>

#pragma managed(push, off)
#include "UTKernelDriver.h"
#include "UTKernelDriverOEMPA.h"
#include "CustomizedDriverAPI.h"
#include "CustomizedWizardAPI.h"
#include "UTKernelAPI.h"
__declspec(dllimport) void UTKernel_EnableEventUpdate(bool bEnable);
__declspec(dllimport) bool WINAPI OEMPA_WriteHWWizard(	CHWDeviceOEMPA *pHWDeviceOEMPA,
									int iWizardSystemId,int iWizardChannelId);
#pragma managed(push, on)

using System::String;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;

using namespace csDriverOEMPA;

namespace WizardTemplate
{

#pragma region enumerations
	public enum class csSpecimen {csPlane=ePlane,
							csDisk=eDisk};
	public enum class csWave {csLongitudinal=eLongitudinal,
							csTransverse=eTransverse};
	public enum class csRectification{
							csSigned=eSigned,			//Rectification: RF --> not rectified, signed value,
							csUnsigned=eUnsigned,			//FW --> rectified, unsigned
							csUnsignedPositive=eUnsignedPositive,	//HWP --> only positive, unsigned,
							csUnsignedNegative=eUnsignedNegative	//HWN --> only negative
							};
	public enum class csGateModeAmp{csAmpAbsolute=eAmpAbsolute,
							csAmpMaximum=eAmpMaximum,
							csAmpMinimum=eAmpMinimum,
							csAmpPeakToPeak=eAmpPeakToPeak};
	public enum class csGateModeTof{csTofAmplitudeDetection=eTofAmplitudeDetection,//"AMP's TOF" : where the AMP result has been found, for Peak-Peak--> where Max has been found
								csTofThresholdCross=eTofThresholdCross,//"TH cross": first cross of the THRESHOLD  
								csTofZeroFirstAfterThresholdCross=eTofZeroFirstAfterThresholdCross,//"ZrA": first time crossed 0 after crossing THRESHOLD
								csTofZeroLastBeforeThresholdCross=eTofZeroLastBeforeThresholdCross//"ZrB": last time crossed 0 before crossing THRESHOLD
								};
	public enum class csEnumDepthMode{csTrueDepth=eStandardDepth,csSoundPath=eSoundPath,csTrueDepthBigBar=eDepthBigBar};
#pragma endregion enumerations
///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//# TEMPLATE - SINGLE-CHANNEL - LINEAR
	public ref class csWizardSpecimen
    {
	private:
		csSpecimen m_eSpecimen;
		double m_dVelocity;
		csWave m_eWave;
		double m_dRadius;

	public:
		csWizardSpecimen();
		~csWizardSpecimen();

		property double Velocity{
			double get()
			{
				return m_dVelocity;
			} 
			void set(double value)
			{
				m_dVelocity = value;
			}
		}

		property csWave Wave{
			csWave get()
			{
				return m_eWave;
			} 
			void set(csWave eWave)
			{
				m_eWave = eWave;
			}
		}

		property double Radius{
			double get()
			{
				if(m_eSpecimen==csSpecimen::csDisk)
					return m_dRadius;
				else
					return 0.0;
			} 
			void set(double value)
			{
				if(!value)
					m_eSpecimen = csSpecimen::csPlane;
				else
					m_eSpecimen = csSpecimen::csDisk;
				m_dRadius = value;
			}
		}

		property csSpecimen Specimen{
			csSpecimen get()
			{
				return m_eSpecimen;
			} 
			void set(csSpecimen value)
			{
				m_eSpecimen = value;
				if(m_eSpecimen == csSpecimen::csPlane)
					m_dRadius = 0.0;
			}
		}
		
		static property array<String^>^ Waves
		{
			array<String^>^ get()
			{
				array<String^>^ values = gcnew array<String^> {"Longitudinal", "Transverse"};
				return values;
			}
		}
		static property array<String^>^ Specimens
		{
			array<String^>^ get()
			{
				array<String^>^ values = gcnew array<String^> {"Plane", "Disk"};
				return values;
			}
		}
	protected:
		!csWizardSpecimen();
		void Free();
	};

	public ref class csWizardWedge
    {
	private:
		bool m_bEnable;
		double m_dVelocity;
		double m_dHeight;
		double m_dAngle;

	public:
		csWizardWedge();
		~csWizardWedge();

		property double Velocity{
			double get()
			{
				return m_dVelocity;
			} 
			void set(double value)
			{
				m_dVelocity = value;
			}
		}

		property bool Enable{
			bool get()
			{
				return m_bEnable;
			} 
			void set(bool bEnable)
			{
				m_bEnable = bEnable;
			}
		}

		property double Height{
			double get()
			{
				return m_dHeight;
			} 
			void set(double value)
			{
				m_dHeight = value;
			}
		}

		property double Angle{
			double get()
			{
				return m_dAngle;
			} 
			void set(double value)
			{
				m_dAngle = value;
			}
		}

	protected:
		!csWizardWedge();
		void Free();
	};

	public ref class csWizardProbe
    {
	private:
		int m_iElementCount;
		double m_dPitch;
		double m_dFrequency;
		double m_dRadius;

	public:
		csWizardProbe();
		~csWizardProbe();

		property int ElementCount{
			int get()
			{
				return m_iElementCount;
			} 
			void set(int value)
			{
				m_iElementCount = value;
			}
		}

		property double Pitch{
			double get()
			{
				return m_dPitch;
			} 
			void set(double value)
			{
				m_dPitch = value;
			}
		}

		property double Frequency{
			double get()
			{
				return m_dFrequency;
			} 
			void set(double value)
			{
				m_dFrequency = value;
			}
		}

		property double Radius{
			double get()
			{
				return m_dRadius;
			} 
			void set(double value)
			{
				m_dRadius = value;
			}
		}
	protected:
		!csWizardProbe();
		void Free();
	};

	public ref class csWizardScan
    {
	private:
		int m_iElementCount;
		double m_dDepthEmission;
		double m_dAngleStart;
		double m_dAngleStop;
		double m_dAngleStep;
		int m_iElementStart;
		int m_iElementStop;
		int m_iElementStep;
		bool m_bLinear;
		csEnumDepthMode m_DepthMode;

	public:
		csWizardScan();
		~csWizardScan();

		property bool Linear{
			bool get()
			{
				return m_bLinear;
			} 
			void set(bool value)
			{
				m_bLinear = value;
			}
		}

		property int ElementCount{
			int get()
			{
				return m_iElementCount;
			} 
			void set(int value)
			{
				m_iElementCount = value;
			}
		}

		property double DepthEmission{
			double get()
			{
				return m_dDepthEmission;
			} 
			void set(double value)
			{
				m_dDepthEmission = value;
			}
		}

		array<double>^ DepthReception;

		property double AngleStart{
			double get()
			{
				return m_dAngleStart;
			} 
			void set(double value)
			{
				m_dAngleStart = value;
			}
		}

		property double AngleStop{
			double get()
			{
				if(!m_bLinear)
					return m_dAngleStop;
				else
					return m_dAngleStart;
			} 
			void set(double value)
			{
				if(!m_bLinear)
				m_dAngleStop = value;
			}
		}

		property double AngleStep{
			double get()
			{
				if(!m_bLinear)
					return m_dAngleStep;
				else
					return M_PI / 180.0;
			}
			void set(double value)
			{
				if(!m_bLinear)
					m_dAngleStep = value;
			}
		}

		property int ElementStart{
			int get()
			{
				return m_iElementStart;
			} 
			void set(int value)
			{
				m_iElementStart = value;
			}
		}

		property int ElementStop{
			int get()
			{
				if(m_bLinear)
					return m_iElementStop;
				else
					return m_iElementStart;
			} 
			void set(int value)
			{
				if(m_bLinear)
					m_iElementStop = value;
			}
		}

		property int ElementStep{
			int get()
			{
				if(m_bLinear)
					return m_iElementStep;
				else
					return 1;
			} 
			void set(int value)
			{
				if(m_bLinear)
				m_iElementStep = value;
			}
		}

		property csEnumDepthMode DepthMode{
			csEnumDepthMode get()
			{
				return m_DepthMode;
			} 
			void set(csEnumDepthMode depthMode)
			{
				m_DepthMode = depthMode;
			}
		}

	protected:
		!csWizardScan();
		void Free();
	};

	public ref class csWizardGateAscan
    {
	private:
		double m_dStart;
		double m_dStop;
		double m_dTimeSlot;

	public:
		csWizardGateAscan();
		~csWizardGateAscan();

		property double Start{
			double get()
			{
				return m_dStart;
			} 
			void set(double value)
			{
				m_dStart = value;
			}
		}

		property double Stop{
			double get()
			{
				return m_dStop;
			} 
			void set(double value)
			{
				m_dStop = value;
			}
		}

		property double Range{
			double get()
			{
				return m_dStop-m_dStart;
			} 
			void set(double value)
			{
				m_dStop = m_dStart+value;
			}
		}

		property double TimeSlot{
			double get()
			{
				return m_dTimeSlot;
			} 
			void set(double value)
			{
				m_dTimeSlot = value;
			}
		}
	protected:
		!csWizardGateAscan();
		void Free();
	};

	public ref class csWizardGateCscan
    {
	private:
		bool m_bEnable;
		double m_dStart;
		double m_dStop;
		double m_dThreshold;
		csRectification m_eRectification;
		csGateModeAmp m_eGateModeAmp;
		csGateModeTof m_eGateModeTof;

	public:
		csWizardGateCscan();
		~csWizardGateCscan();

		void Init();

		property bool Enable{
			bool get()
			{
				return m_bEnable;
			} 
			void set(bool value)
			{
				m_bEnable = value;
			}
		}

		property double Start{
			double get()
			{
				return m_dStart;
			} 
			void set(double value)
			{
				m_dStart = value;
			}
		}

		property double Stop{
			double get()
			{
				return m_dStop;
			} 
			void set(double value)
			{
				m_dStop = value;
			}
		}

		property double Range{
			double get()
			{
				return m_dStop-m_dStart;
			} 
		}

		property double Threshold{
			double get()
			{
				return m_dThreshold;
			} 
			void set(double value)
			{
				m_dThreshold = value;
			}
		}

		property csRectification Rectification{
			csRectification get()
			{
				return m_eRectification;
			} 
			void set(csRectification value)
			{
				m_eRectification = value;
			}
		}

		property csGateModeAmp ModeAmplitude{
			csGateModeAmp get()
			{
				return m_eGateModeAmp;
			} 
			void set(csGateModeAmp value)
			{
				m_eGateModeAmp = value;
			}
		}

		property csGateModeTof ModeTimeOfFlight{
			csGateModeTof get()
			{
				return m_eGateModeTof;
			} 
			void set(csGateModeTof value)
			{
				m_eGateModeTof = value;
			}
		}

		static property array<String^>^ Rectifications
		{
			array<String^>^ get()
			{
				array<String^>^ values = gcnew array<String^> {"Signed", "Unsigned", "UnsignedPositive", "UnsignedNegative"};
				return values;
			}
		}

		static property array<String^>^ GateModeAmps
		{
			array<String^>^ get()
			{
				array<String^>^ values = gcnew array<String^> {"Absolute", "Maximum", "Minimum", "PeakToPeak"};
				return values;
			}
		}

		static property array<String^>^ GateModeTofs
		{
			array<String^>^ get()
			{
				array<String^>^ values = gcnew array<String^> {"AmplitudeDetection", "ThresholdCross", "ZeroFirstAfterThresholdCross", "ZeroLastBeforeThresholdCross"};
				return values;
			}
		}
	protected:
		!csWizardGateCscan();
		void Free();
	};

	public ref class csWizardTemplate
    {
	private:
		CUTWizardSystem *m_pWizardSystem;
		CUTChannels *m_pChannel;
		BOOL m_bCheckWizardEnable;
		int m_iDeviceId;

	public:
		csWizardTemplate();
		~csWizardTemplate();

		csWizardSpecimen^ Specimen;
		csWizardWedge^ Wedge;
		csWizardProbe^ Probe;
		csWizardScan^ Scan;
		array<csWizardScan^>^ aScan;//multiple scan management.
		csWizardGateAscan^ GateAscan;
		array<csWizardGateCscan^>^ aGateCscan;

		bool SetScanCount(int iScanCount);
		void TemplateEdit(String^ filename,bool bCloseWaiting);
		bool TemplateToWizard();
		bool WizardUpdateScan(int *piErrorChannelProbe,int *piErrorChannelScan);
		bool WizardToFile(csHWDeviceOEMPA^ hwDeviceOEMPA,String^ file);
		bool WizardToHw(csHWDeviceOEMPA^ hwDeviceOEMPA);
		bool ReadWizard(csHWDeviceOEMPA^ hwDeviceOEMPA,[Out] csRoot^ %root,[Out] array<csCycle^>^ %cycle,[Out] array<csFocalLaw^>^ %emission,[Out] array<csFocalLaw^>^ %reception);
		bool WriteFile(csHWDeviceOEMPA^ hwDeviceOEMPA,csRoot^ %root,array<csCycle^>^ %cycle,array<csFocalLaw^>^ %emission,array<csFocalLaw^>^ %reception,String^ file);

		static int GateCsanCountMax();
		void ReallocGateCscan();
		void DesallocGateCscan();
		void EditFile(String^ filename,bool bCloseWaiting);
		static bool GetWizardFolder(String^ %filename);

		static void EnableEventUpdate(bool bEnable)
		{
			UTKernel_EnableEventUpdate(bEnable);
		}
	protected:
		!csWizardTemplate();
		void Free();
		bool WizardDelete();
		bool WizardNew();
		static int g_iCount = 0;
	};

//////////////////////////////////////////////////////////////////////////

};

namespace WizardTemplate
{
////////////////////////////////////////////////////////
#pragma region csWizardTemplateImplement
	csWizardSpecimen::csWizardSpecimen()
	{
		double dVelocity=6300.0;
		double dRadius=0.0;//default if Plane 100.0e-3;

		m_dVelocity = dVelocity;
		m_eWave = csWave::csLongitudinal;
		m_dRadius = dRadius;
	}
	csWizardSpecimen::~csWizardSpecimen()
	{
		this->!csWizardSpecimen();
	}
	csWizardSpecimen::!csWizardSpecimen()
	{
		Free();
	}
	void csWizardSpecimen::Free()
	{
	}

	csWizardWedge::csWizardWedge()
	{
		double dVelocity=1480.0;
		double dHeight=20.0e-3;
		double dAngle=0.0;

		m_bEnable = false;
		m_dVelocity = dVelocity;
		m_dHeight = dHeight;
		m_dAngle = dAngle;
	}
	csWizardWedge::~csWizardWedge()
	{
		this->!csWizardWedge();
	}
	csWizardWedge::!csWizardWedge()
	{
		Free();
	}
	void csWizardWedge::Free()
	{
	}

	csWizardProbe::csWizardProbe()
	{
		double dPitch=0.6e-3;
		double dFrequency=5e6;
		double dRadius=150.0e-3;

		m_iElementCount = 128;
		m_dPitch = dPitch;
		m_dFrequency = dFrequency;
		m_dRadius = dRadius;
	}
	csWizardProbe::~csWizardProbe()
	{
		this->!csWizardProbe();
	}
	csWizardProbe::!csWizardProbe()
	{
		Free();
	}
	void csWizardProbe::Free()
	{
	}

	csWizardScan::csWizardScan()
	{
		double dDepthEmission=100.0e-3;
		m_iElementCount = 8;
		m_dDepthEmission = dDepthEmission;
		DepthReception = gcnew array<double>{100.0e-3};
		m_dAngleStart = 0.0;
		m_dAngleStop = 0.0;
		m_dAngleStep = 0.0;
		m_iElementStart = 1;
		m_iElementStop = 1;
		m_iElementStep = 1;
		m_bLinear = true;
		m_DepthMode = csEnumDepthMode::csTrueDepth;
		//DepthReception = gcnew array<double>(iSize);
	}
	csWizardScan::~csWizardScan()
	{
		this->!csWizardScan();
	}
	csWizardScan::!csWizardScan()
	{
		Free();
	}
	void csWizardScan::Free()
	{
	}

	csWizardGateAscan::csWizardGateAscan()
	{
		double dStop=1.0e-6;
		double dTimeSlot=2000.0e-6;

		m_dStart = 0.0;
		m_dStop = dStop;
		m_dTimeSlot = dTimeSlot;
	}
	csWizardGateAscan::~csWizardGateAscan()
	{
		this->!csWizardGateAscan();
	}
	csWizardGateAscan::!csWizardGateAscan()
	{
		Free();
	}
	void csWizardGateAscan::Free()
	{
	}

	csWizardGateCscan::csWizardGateCscan()
	{
		Init();
	}
	void csWizardGateCscan::Init()
	{
		double dStop=1.0e-6;
		double dThreshold=50.0;

		m_dStart = 0.0;
		m_dStop = dStop;
		m_dThreshold = dThreshold;
		m_eRectification = csRectification::csSigned;
		m_eGateModeAmp = csGateModeAmp::csAmpAbsolute;
		m_eGateModeTof = csGateModeTof::csTofAmplitudeDetection;
	}
	csWizardGateCscan::~csWizardGateCscan()
	{
		this->!csWizardGateCscan();
	}
	csWizardGateCscan::!csWizardGateCscan()
	{
		Free();
	}
	void csWizardGateCscan::Free()
	{
	}

	bool csWizardTemplate::SetScanCount(int iScanCount)
	{
		if(iScanCount<=0)
		{
			if(aScan!=nullptr)
			{
				for(int iScanIndex=0;iScanIndex<aScan->GetLength(0);iScanIndex++)
				{
					if(aScan[iScanIndex]!=nullptr)
						delete aScan[iScanIndex];
					aScan[iScanIndex] = nullptr;
				}
				delete aScan;
			}
			aScan = nullptr;
			Scan = nullptr;
			return true;
		}
		aScan = gcnew array<csWizardScan^>(iScanCount);
		for(int iScanIndex=0;iScanIndex<iScanCount;iScanIndex++)
		{
			aScan[iScanIndex] = gcnew csWizardScan();
		}
		Scan = aScan[0];
		return true;
	}
	csWizardTemplate::csWizardTemplate()
	{
		g_iCount++;
		if(g_iCount>1)
			UTKernel_SystemMessageBox(L"\"csWizardTemplate\" could be instantiated only one time!");
		Specimen = gcnew csWizardSpecimen();
		Wedge = gcnew csWizardWedge();
		Probe = gcnew csWizardProbe();
		aScan = nullptr;
		Scan = nullptr;
		GateAscan = gcnew csWizardGateAscan();
		aGateCscan = nullptr;
		ReallocGateCscan();
		SetScanCount(1);
		m_pWizardSystem = NULL;
		m_pChannel = NULL;
		m_bCheckWizardEnable = FALSE;
		m_iDeviceId = 0;
		WizardNew();
	}
	csWizardTemplate::~csWizardTemplate()
	{
		g_iCount--;
		this->!csWizardTemplate();
	}
	csWizardTemplate::!csWizardTemplate()
	{
		delete Specimen;
		delete Wedge;
		delete Probe;
		delete GateAscan;
		delete aGateCscan;
		Specimen = nullptr;
		Wedge = nullptr;
		Probe = nullptr;
		Scan = nullptr;
		GateAscan = nullptr;
		aGateCscan = nullptr;
		if(aScan!=nullptr)
		{
			for(int iIndex=0;iIndex<aScan->Length;iIndex++)
			{
				if(aScan[iIndex]!=nullptr)
					delete aScan[iIndex];
				aScan[iIndex] = nullptr;
			}
			delete aScan;
		}
		aScan = nullptr;
		Free();
	}
	void csWizardTemplate::Free()
	{
		WizardDelete();
	}
	int csWizardTemplate::GateCsanCountMax()
	{
		return g_iOEMPAGateCountMax;
	}
	void csWizardTemplate::ReallocGateCscan()
	{
		bool bRet=true;
		csWizardGateCscan ^gate0=nullptr;
		csWizardGateCscan ^gate1=nullptr,^gate2=nullptr,^gate3=nullptr;

		if(aGateCscan && (0<aGateCscan->Length))
			gate0 = aGateCscan[0];
		if(aGateCscan && (1<aGateCscan->Length))
			gate1 = aGateCscan[1];
		if(aGateCscan && (0<aGateCscan->Length))
			gate2 = aGateCscan[2];
		if(aGateCscan && (0<aGateCscan->Length))
			gate3 = aGateCscan[3];
		if(!aGateCscan)
			aGateCscan = gcnew array<csWizardGateCscan^>(g_iOEMPAGateCountMax);
		if(!aGateCscan)
			return;
		if(gate0==nullptr)
			aGateCscan[0] = gcnew csWizardGateCscan();
		else
			aGateCscan[0] = gate0;
		if(gate1==nullptr)
			aGateCscan[1] = gcnew csWizardGateCscan();
		else
			aGateCscan[1] = gate1;
		if(gate2==nullptr)
			aGateCscan[2] = gcnew csWizardGateCscan();
		else
			aGateCscan[2] = gate2;
		if(gate3==nullptr)
			aGateCscan[3] = gcnew csWizardGateCscan();
		else
			aGateCscan[3] = gate3;
	}
	void csWizardTemplate::DesallocGateCscan()
	{
		if(aGateCscan)
		{
			for(int iIndex=0;iIndex<aGateCscan->Length;iIndex++)
			{
				delete aGateCscan[iIndex];
				aGateCscan[iIndex] = nullptr;
			}
			delete aGateCscan;
			aGateCscan = nullptr;
		}
	}
	bool csWizardTemplate::WizardDelete()
	{
		bool bRet=true;

		if(!m_pWizardSystem)
			return false;
		if(!m_pChannel)
			return false;
		if(!CUTKernelFile::DeleteObject(m_pChannel,eRootTypeChannels))
			bRet = false;
		else
			m_pChannel = NULL;
		if(!CUTKernelFile::DeleteObject(m_pWizardSystem,eRootTypeWizardSystem))
			bRet = false;
		else
			m_pWizardSystem = NULL;
		m_bCheckWizardEnable = FALSE;
		return bRet;
	}

	bool csWizardTemplate::WizardNew()
	{
		wchar_t pAux[MAX_PATH];
		CUTKernelRoot *ptr;
		bool bRet=true;
		int iWizardProbeIndex=0;
		bool bCreate;

		if(m_bCheckWizardEnable)
		{
			WizardDelete();
			m_bCheckWizardEnable = FALSE;
		}
		swprintf(pAux,MAX_PATH,L"OEMPA_system");
		ptr = CUTKernelRoot::SafeNewObject(eRootTypeWizardSystem,pAux,bCreate);
		m_pWizardSystem = dynamic_cast<CUTWizardSystem*>(ptr);
		if(!m_pWizardSystem)
		{
			WizardDelete();
			return false;
		}
		swprintf(pAux,g_iMaxNameLength+1,L"OEMPA_channel_%d",m_iDeviceId);
		ptr = CUTKernelRoot::SafeNewObject(eRootTypeChannels,pAux,bCreate);
		m_pChannel = dynamic_cast<CUTChannels*>(ptr);
		if(!m_pChannel)
		{
			WizardDelete();
			return false;
		}
		if(m_pChannel->LinkWizardProbe(0,m_pWizardSystem,iWizardProbeIndex))
			m_bCheckWizardEnable = TRUE;
		else
			WizardDelete();
		return bRet;
	}

	bool csWizardTemplate::TemplateToWizard()
	{
		double dSpecimenRadius=Specimen->Radius;
		double adCscanStart[g_iOEMPAGateCountMax],adCscanRange[g_iOEMPAGateCountMax],adCscanThreshold[g_iOEMPAGateCountMax];
		enumRectification aeRectification[g_iOEMPAGateCountMax];
		enumGateModeAmp aeModeAmp[g_iOEMPAGateCountMax];
		enumGateModeTof aeModeTof[g_iOEMPAGateCountMax];
		bool bRet;
		int iCscanCount;
		enumDepthMode eDepthMode;

		if(!m_bCheckWizardEnable)
			return false;
		iCscanCount = aGateCscan->Length;
		if(aGateCscan->Length>g_iOEMPAGateCountMax)
			return false;
		if((!dSpecimenRadius && m_pWizardSystem->Specimen().SetSpecimen(ePlane)) ||
			((dSpecimenRadius>0.0) && m_pWizardSystem->Specimen().SetSpecimen(eDisk)))
			return false;
		if((dSpecimenRadius>0.0) && m_pWizardSystem->Specimen().SetRadius(dSpecimenRadius))
			return false;
		if(m_pWizardSystem->Probe(0).SetRadius(Probe->Radius))
			return false;
		if(aScan->GetLength(0)<=0)
			return false;
		if(m_pChannel->Probe(0).SetScanCount(aScan->GetLength(0)))
			return false;
		for(int iIndex=0;iIndex<aGateCscan->Length;iIndex++)
		{
			if(!aGateCscan[iIndex]->Enable)
			{
				iCscanCount = iIndex;
				break;
			}
			adCscanStart[iIndex] = aGateCscan[iIndex]->Start;
			adCscanRange[iIndex] = aGateCscan[iIndex]->Range;
			adCscanThreshold[iIndex] = aGateCscan[iIndex]->Threshold;
			aeRectification[iIndex] = (enumRectification)aGateCscan[iIndex]->Rectification;
			aeModeAmp[iIndex] = (enumGateModeAmp)aGateCscan[iIndex]->ModeAmplitude;
			aeModeTof[iIndex] = (enumGateModeTof)aGateCscan[iIndex]->ModeTimeOfFlight;
		}
		//pin_ptr<double> adScanDepthReception = &Scan->DepthReception[0];
		for(int iScanIndex=0;iScanIndex<aScan->GetLength(0);iScanIndex++)
		{
			double* adScanDepthReception = new double[aScan[iScanIndex]->DepthReception->Length];
			if(!adScanDepthReception)
				return false;
			for(int iIndex=0;iIndex<aScan[iScanIndex]->DepthReception->Length;iIndex++)
				adScanDepthReception[iIndex] = aScan[iScanIndex]->DepthReception[iIndex];
			eDepthMode = (enumDepthMode)aScan[iScanIndex]->DepthMode;
			if(aScan[iScanIndex]->Linear)
				bRet = OEMPA_WriteWizardMultipleChannel(0,iScanIndex,
							m_pWizardSystem,m_pChannel,
							Specimen->Wave==csWave::csLongitudinal,Specimen->Velocity,
							Probe->ElementCount,Probe->Pitch,Probe->Frequency,
							Wedge->Enable,
							Wedge->Height,Wedge->Angle,Wedge->Velocity,
							aScan[iScanIndex]->DepthEmission,adScanDepthReception,aScan[iScanIndex]->DepthReception->Length,
							aScan[iScanIndex]->ElementCount,
							true,aScan[iScanIndex]->ElementStart,aScan[iScanIndex]->ElementStop,aScan[iScanIndex]->ElementStep,
							false,aScan[iScanIndex]->AngleStart,aScan[iScanIndex]->AngleStart,0.01,
							GateAscan->Start,GateAscan->Range,GateAscan->TimeSlot,
							iCscanCount/*aGateCscan->Length*/,adCscanStart,adCscanRange,adCscanThreshold,
							aeRectification,aeModeAmp,aeModeTof,
							eDepthMode);
			else
				bRet = OEMPA_WriteWizardMultipleChannel(0,iScanIndex,
							m_pWizardSystem,m_pChannel,
							Specimen->Wave==csWave::csLongitudinal,Specimen->Velocity,
							Probe->ElementCount,Probe->Pitch,Probe->Frequency,
							Wedge->Enable,
							Wedge->Height,Wedge->Angle,Wedge->Velocity,
							aScan[iScanIndex]->DepthEmission,adScanDepthReception,aScan[iScanIndex]->DepthReception->Length,
							aScan[iScanIndex]->ElementCount,
							false,aScan[iScanIndex]->ElementStart,aScan[iScanIndex]->ElementStart,1,
							true,aScan[iScanIndex]->AngleStart,aScan[iScanIndex]->AngleStop,aScan[iScanIndex]->AngleStep,
							GateAscan->Start,GateAscan->Range,GateAscan->TimeSlot,
							iCscanCount/*aGateCscan->Length*/,adCscanStart,adCscanRange,adCscanThreshold,
							aeRectification,aeModeAmp,aeModeTof,
							eDepthMode);
			delete adScanDepthReception;
			adScanDepthReception = NULL;
		}
		return bRet;
	}

	bool csWizardTemplate::WizardUpdateScan(int *piErrorChannelProbe,int *piErrorChannelScan)
	{
		CUTProbe *pProbe;
		CUTScan *pScan;
		int iProbeCount,iScanCount;
		bool bRet=true;

		if(piErrorChannelProbe)
			*piErrorChannelProbe = -1;
		if(piErrorChannelScan)
			*piErrorChannelScan = -1;
		if(!m_pChannel)
			return false;
//int aiObjectList[2];
//enumRootType aeRootType[2];
//if(m_pWizardSystem->GetID(aiObjectList[0]) && m_pChannel->GetID(aiObjectList[1]))
//{
//	aeRootType[0] = eRootTypeWizardSystem;
//	aeRootType[1] = eRootTypeChannels;
//	UTKernel_FileSave(false,2,aiObjectList,aeRootType,L"C:\\Users\\Public\\Wizard.txt");
//}
		if(!CUTChannels::IsDefaultMultiChannels())
		{
			if(!(*m_pChannel).Probe().Scan().SetScanStatusOutOfDate())
				return false;
			return (*m_pChannel).Probe().Scan().UpdateScan();//UpdateScan
		}else{
			for(int iStep=0;iStep<2;iStep++)
			{
				iProbeCount = m_pChannel->GetProbeCount();
				for(int iProbeIndex=0;iProbeIndex<iProbeCount;iProbeIndex++)
				{
					pProbe = &m_pChannel->Probe(iProbeIndex);
					iScanCount = pProbe->GetScanCount();
					for(int iScanIndex=0;iScanIndex<iScanCount;iScanIndex++)
					{
						pScan = &pProbe->Scan(iScanIndex);
						if((iStep==0) && !pScan->SetScanStatusOutOfDate())
							bRet = false;
						if((iStep==1) && !pScan->UpdateScan())
						{
							bRet = false;
							if(piErrorChannelProbe && (*piErrorChannelProbe==-1))
								*piErrorChannelProbe = iProbeIndex;
							if(piErrorChannelScan && (*piErrorChannelScan==-1))
								*piErrorChannelScan = iScanIndex;
						}
					}
				}
			}
		}
		return bRet;
	}

	bool csWizardTemplate::WizardToFile(csHWDeviceOEMPA^ hwDeviceOEMPA,String^ file)
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA;
		wchar_t *pFile;
		bool bRet;

		if(!m_bCheckWizardEnable)
			return false;
		if(hwDeviceOEMPA==nullptr)
			return false;
		pHWDeviceOEMPA = (CHWDeviceOEMPA*)hwDeviceOEMPA->GetHWDeviceOEMPA();
		if(!pHWDeviceOEMPA)
			return false;
		pFile = (wchar_t*)(void*)Marshal::StringToHGlobalUni(file);
		bRet = OEMPA_ReadWizardWriteFile(pHWDeviceOEMPA,m_pWizardSystem,m_pChannel,pFile);
		Marshal::FreeHGlobal((IntPtr)pFile);
		return bRet;
	}

	bool csWizardTemplate::WizardToHw(csHWDeviceOEMPA^ hwDeviceOEMPA)
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA;
		int iWizardSystemId,iWizardChannelId;

		if(!m_bCheckWizardEnable)
			return false;
		if(hwDeviceOEMPA==nullptr)
			return false;
		if(!m_pWizardSystem->GetID(iWizardSystemId))
			return false;
		if(!m_pChannel->GetID(iWizardChannelId))
			return false;
		pHWDeviceOEMPA = (CHWDeviceOEMPA*)hwDeviceOEMPA->GetHWDeviceOEMPA();
		if(!pHWDeviceOEMPA)
			return false;
		return OEMPA_WriteHWWizard(pHWDeviceOEMPA,iWizardSystemId,iWizardChannelId);
	}

	bool csWizardTemplate::ReadWizard(csHWDeviceOEMPA^ hwDeviceOEMPA,[Out] csRoot^ %root,[Out] array<csCycle^>^ %cycle,[Out] array<csFocalLaw^>^ %emission,[Out] array<csFocalLaw^>^ %reception)
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA;
		bool bRet=true;
		structRoot Root;
		structCycle *pCycle=NULL;
		CFocalLaw *pEmission=NULL,*pReception=NULL;

		if(!m_bCheckWizardEnable)
			return false;
		if(hwDeviceOEMPA==nullptr)
			return false;
		pHWDeviceOEMPA = (CHWDeviceOEMPA*)hwDeviceOEMPA->GetHWDeviceOEMPA();
		if(!pHWDeviceOEMPA)
			return false;
		bRet = OEMPA_ReadWizard(pHWDeviceOEMPA,m_pWizardSystem,m_pChannel,Root,pCycle,pEmission,pReception);
		if(bRet)
		{
			root = gcnew csRoot;
			if(!root || !root->vCopyFrom(&Root))
				bRet = false;
			if(Root.iCycleCount>0)
			{
				cycle = gcnew array<csCycle^>(Root.iCycleCount);
				if(cycle)
				{
					for(int iCycle=0;iCycle<Root.iCycleCount;iCycle++)
					{
						cycle[iCycle] = gcnew csCycle;
						if(!cycle[iCycle]->vCopyFrom(&pCycle[iCycle]))
							bRet = false;
					}
				}
				emission = gcnew array<csFocalLaw^>(Root.iCycleCount);
				if(emission)
				{
					for(int iCycle=0;iCycle<Root.iCycleCount;iCycle++)
					{
						emission[iCycle] = gcnew csFocalLaw;
						if(!emission[iCycle]->vCopyFrom(&pEmission[iCycle]))
							bRet = false;
					}
				}
				reception = gcnew array<csFocalLaw^>(Root.iCycleCount);
				if(reception)
				{
					for(int iCycle=0;iCycle<Root.iCycleCount;iCycle++)
					{
						reception[iCycle] = gcnew csFocalLaw;
						if(!reception[iCycle]->vCopyFrom(&pReception[iCycle]))
							bRet = false;
					}
				}
			}
		}
		return bRet;
	}
	bool csWizardTemplate::WriteFile(csHWDeviceOEMPA^ hwDeviceOEMPA,csRoot^ %root,array<csCycle^>^ %cycle,array<csFocalLaw^>^ %emission,array<csFocalLaw^>^ %reception,String^ file)
	{
		CHWDeviceOEMPA *pHWDeviceOEMPA;
		wchar_t *pFile;
		bool bRet=true;
		structRoot Root;
		structCycle *pCycle=NULL;
		CFocalLaw *pEmission=NULL,*pReception=NULL;

		if(!m_bCheckWizardEnable)
			return false;
		if(hwDeviceOEMPA==nullptr)
			return false;
		pHWDeviceOEMPA = (CHWDeviceOEMPA*)hwDeviceOEMPA->GetHWDeviceOEMPA();
		if(!pHWDeviceOEMPA)
			return false;

		if(!root->vCopyTo(&Root))
			bRet = false;

		if(Root.iCycleCount>0)
		{
			pCycle = OEMPA_AllocCycle(Root.iCycleCount);
			pEmission = OEMPA_AllocFocalLaw(Root.iCycleCount);
			pReception = OEMPA_AllocFocalLaw(Root.iCycleCount);
			if(!pCycle || !pEmission || !pReception)
				return false;
			OEMPA_ResetArrayFocalLaw(Root.iCycleCount,pEmission);
			OEMPA_ResetArrayFocalLaw(Root.iCycleCount,pReception);

			if(pCycle)
			{
				for(int iCycle=0;iCycle<Root.iCycleCount;iCycle++)
				{
					if(!cycle[iCycle]->vCopyTo(&pCycle[iCycle]))
						bRet = false;
				}
			}
			if(pEmission)
			{
				for(int iCycle=0;iCycle<Root.iCycleCount;iCycle++)
				{
					if(!emission[iCycle]->vCopyTo(&pEmission[iCycle]))
						bRet = false;
				}
			}
			if(pReception)
			{
				for(int iCycle=0;iCycle<Root.iCycleCount;iCycle++)
				{
					if(!reception[iCycle]->vCopyTo(&pReception[iCycle]))
						bRet = false;
				}
			}
		}

		if(bRet)
			pFile = (wchar_t*)(void*)Marshal::StringToHGlobalUni(file);
		if(!OEMPA_WriteFileText(	pFile,&Root,pCycle,pEmission,pReception))
			bRet = false;
		Marshal::FreeHGlobal((IntPtr)pFile);

		if(Root.iCycleCount>0)
		{
			OEMPA_DesallocCycle(pCycle);
			OEMPA_DesallocFocalLaw(pEmission);
			OEMPA_DesallocFocalLaw(pReception);
		}

		return bRet;
	}

	bool csWizardTemplate::GetWizardFolder(String^ %filename)
	{
		filename = gcnew String(UTKernel_GetFolderData());
		return true;
	}

	void csWizardTemplate::TemplateEdit(String ^file,bool bCloseWaiting)
	{
		EditFile(file,bCloseWaiting);
	}

	void csWizardTemplate::EditFile(String^ file,bool bCloseWaiting)
	{
		SHELLEXECUTEINFO execinfo;
		wchar_t *pFile;

		pFile = (wchar_t*)(void*)Marshal::StringToHGlobalUni(file);
		memset(&execinfo, 0, sizeof(execinfo));
		execinfo.lpFile = pFile;
		execinfo.cbSize = sizeof(execinfo);
		execinfo.lpVerb = L"open";
		execinfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		execinfo.nShow = SW_SHOWDEFAULT;
		execinfo.lpParameters = 0;
		ShellExecuteEx(&execinfo);
		if(bCloseWaiting)
			WaitForSingleObject(execinfo.hProcess, INFINITE);
		Marshal::FreeHGlobal((IntPtr)pFile);
	}

	//void CDlgSetupFiles::OnBnClickedButtonToolbox()
	//{
	//	g_RunToolbox.Run(GetSafeHwnd(),CDlgSetupFiles::CallbackThreadToolbox);
	//}

#pragma endregion csWizardTemplateImplement
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

}

//DWORD WINAPI CallbackThreadToolbox(HWND hWnd,CRunToolbox *pRunToolbox)
//{
//	int iRootId;
//	CWaitCursor wait;
//
//	if(!UTKernel_IsToolboxRunning())
//	{
//		pRunToolbox->ResetPostFeedback();
//		UTKernel_ToolboxRun(false,hWnd,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
//		if(!pRunToolbox->WaitPostFeedback(5000,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_ERROR))
//			return 1;
//		pRunToolbox->ResetPostFeedback();
//		UTKernel_ToolboxDisplayCloseAll(hWnd,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
//		if(!pRunToolbox->WaitPostFeedback(5000,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_ERROR))
//			return 2;
//	}
//	if(m_bCheckWizardEnable)
//	{
//		if(m_pWizardSystem && m_pWizardSystem->GetID(iRootId))
//		{
//			pRunToolbox->ResetPostFeedback();
//			UTKernel_ToolboxDisplay(iRootId,hWnd,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
//			if(!pRunToolbox->WaitPostFeedback(5000,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_ERROR))
//				return 3;
//		}
//		if(m_pChannel && m_pChannel->GetID(iRootId))
//		{
//			pRunToolbox->ResetPostFeedback();
//			UTKernel_ToolboxDisplay(iRootId,hWnd,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_OK);
//			if(!pRunToolbox->WaitPostFeedback(5000,g_uiUTEventMessage,WPARAM_RUN_TOOLBOX,LPARAM_RUN_TOOLBOX_ERROR))
//				return 4;
//		}
//	}
//	return 0;//no error
//}
//
