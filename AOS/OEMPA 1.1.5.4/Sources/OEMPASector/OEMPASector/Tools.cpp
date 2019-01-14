
#include "stdafx.h"
#include "Tools.h"
#include <math.h>
#include "ExternalDefinition.h"

unsigned int GetOneSliceSize(int Probe)
{
	unsigned int Size = 0;

	if (Probe == 0)
		Size = InfoProbe1.iCycleCount * InfoProbe1.PointQuantity;
	else if (Probe == 1)
		Size = InfoProbe2.iCycleCount * InfoProbe2.PointQuantity;

	return Size;
}

unsigned int GetCycleCount(int Probe)
{
	unsigned int Size = 0;

	if (Probe == 0)
		Size = InfoProbe1.iCycleCount;
	else if (Probe == 1)
		Size = InfoProbe2.iCycleCount;

	return Size;
}

unsigned int GetEncoderPositionQuantity()
{
	return static_cast<unsigned int>(g_dEncoderAxisLength / g_dEncoderAxisStep);
}

bool AllocateAscanMemory(int Probe)
{
	if ((Probe == 0) && (AscanMemory1))
		DeleteAscanMemory(0);
	else if ((Probe == 1) && (AscanMemory2))
		DeleteAscanMemory(1);

	unsigned int EncoderPositionQty = GetEncoderPositionQuantity();
	unsigned int AscanSliceSize = GetOneSliceSize( Probe);

	if ((EncoderPositionQty == 0) ||
		(AscanSliceSize == 0))
		return false;
	
	if (Probe == 0)
	{
		EnterCriticalSection(&CRT_MEMORY_1);
		AscanMemory1 = new char*[EncoderPositionQty];
		for (unsigned int i = 0 ; i < EncoderPositionQty ; i++)
		{
			AscanMemory1[i] = new char[AscanSliceSize];
			memset(AscanMemory1[i], 0, AscanSliceSize);
		}

		LeaveCriticalSection(&CRT_MEMORY_1);
	}
	else if (Probe == 1)
	{
		EnterCriticalSection(&CRT_MEMORY_2);
		AscanMemory2 = new char*[EncoderPositionQty];

		for (unsigned int i = 0 ; i < EncoderPositionQty ; i++)
		{
			AscanMemory2[i] = new char[AscanSliceSize];
			memset(AscanMemory2[i], 0, AscanSliceSize);
		}

		LeaveCriticalSection(&CRT_MEMORY_2);
	}
	else
		return false;
	
	return true;
}

bool DeleteAscanMemory(int Probe)
{
	unsigned int EncoderPositionQty = GetEncoderPositionQuantity();

	if (EncoderPositionQty == 0)
		return false;

	if ((Probe == 0) && (AscanMemory1 == NULL))
		return false;
	else if ((Probe == 1) && (AscanMemory2 == NULL))
		return false;

	if (Probe == 0)
	{
		EnterCriticalSection(&CRT_MEMORY_1);

		for (unsigned int i = 0 ; i < EncoderPositionQty ; i++)
		{
		}
		for (unsigned int i = 0 ; i < EncoderPositionQty ; i++)
		{
			delete[] AscanMemory1[i];
		}
	
		delete[] AscanMemory1;
		AscanMemory1 = NULL;

		LeaveCriticalSection(&CRT_MEMORY_1);
	}
	else if (Probe == 1)
	{
		EnterCriticalSection(&CRT_MEMORY_2);

		for (unsigned int i = 0 ; i < EncoderPositionQty ; i++)
			delete[] AscanMemory2[i];
	
		delete[] AscanMemory2;
		AscanMemory2 = NULL;

		LeaveCriticalSection(&CRT_MEMORY_2);
	}
	else
		return false;

	return true;
}

bool AllocateCscanMemory(int Probe)
{
	if ((Probe == 0) && (CscanAmpMemory1[0] || CscanTofMemory1[0]))
		DeleteCscanMemory(0);
	else if ((Probe == 1) && (CscanAmpMemory2[0] || CscanTofMemory2[0]))
		DeleteCscanMemory(1);

	unsigned int EncoderPositionQty = GetEncoderPositionQuantity();
	unsigned int CycleCount = GetCycleCount(Probe);

	if ((EncoderPositionQty == 0) ||
		(CycleCount == 0))
		return false;
	
	if (Probe == 0)
	{
		EnterCriticalSection(&CRT_MEMORY_1);
		for(int iGate=0;iGate<4;iGate++)
		{
			g_iCscanAmpMemorySize1 = EncoderPositionQty*CycleCount;
			CscanAmpMemory1[iGate] = new BYTE [g_iCscanAmpMemorySize1];
			if(CscanAmpMemory1[iGate])
				memset(CscanAmpMemory1[iGate],0,sizeof(BYTE)*g_iCscanAmpMemorySize1);
			CscanTofMemory1[iGate] = new short [g_iCscanAmpMemorySize1];
			if(CscanTofMemory1[iGate])
				memset(CscanTofMemory1[iGate],0,sizeof(WORD)*g_iCscanAmpMemorySize1);
		}
		LeaveCriticalSection(&CRT_MEMORY_1);
	}
	else if (Probe == 1)
	{
		EnterCriticalSection(&CRT_MEMORY_2);
		for(int iGate=0;iGate<4;iGate++)
		{
			CscanAmpMemory2[iGate] = new BYTE [EncoderPositionQty*CycleCount];
			if(CscanAmpMemory2[iGate])
				memset(CscanAmpMemory2[iGate],0,sizeof(BYTE)*EncoderPositionQty*CycleCount);
			CscanTofMemory2[iGate] = new short [EncoderPositionQty*CycleCount];
			if(CscanTofMemory2[iGate])
				memset(CscanTofMemory2[iGate],0,sizeof(WORD)*EncoderPositionQty*CycleCount);
		}
		LeaveCriticalSection(&CRT_MEMORY_2);
	}
	else
		return false;
	
	return true;
}

bool DeleteCscanMemory(int Probe)
{
	unsigned int EncoderPositionQty = GetEncoderPositionQuantity();

	if (EncoderPositionQty == 0)
		return false;

	if ((Probe == 0) && (CscanAmpMemory1[0] == NULL) && (CscanTofMemory1[0] == NULL))
		return false;
	else if ((Probe == 1) && (CscanAmpMemory2[0] == NULL) && (CscanTofMemory2[0] == NULL))
		return false;

	if (Probe == 0)
	{
		EnterCriticalSection(&CRT_MEMORY_1);

		for(int iGate=0;iGate<4;iGate++)
		{
			if(CscanAmpMemory1[iGate])
				delete[] CscanAmpMemory1[iGate];
			CscanAmpMemory1[iGate] = NULL;

			if(CscanTofMemory1[iGate])
				delete[] CscanTofMemory1[iGate];
			CscanTofMemory1[iGate] = NULL;
		}

		LeaveCriticalSection(&CRT_MEMORY_1);
	}
	else if (Probe == 1)
	{
		EnterCriticalSection(&CRT_MEMORY_2);

		for(int iGate=0;iGate<4;iGate++)
		{
			if(CscanAmpMemory2[iGate])
				delete[] CscanAmpMemory2[iGate];
			CscanAmpMemory2[iGate] = NULL;

			if(CscanTofMemory2[iGate])
				delete[] CscanTofMemory2[iGate];
			CscanTofMemory2[iGate] = NULL;
		}

		LeaveCriticalSection(&CRT_MEMORY_2);
	}
	else
		return false;

	return true;
}

double GetZAxisWidth()
{
	double Width = 0.0;

	if (MANAGE_REBOUND == 0)
	{
		double MinAngle;
		double Step = GetPointStep();
		unsigned int PointQty;

		if (g_uiProbe == 0)		// Only probe 1
		{
			MinAngle = GetMinimumAngle(0, true);
			PointQty = g_uiPointQuantity[0];
		}
		else if (g_uiProbe == 1)	// Only probe 2
		{
			MinAngle = GetMinimumAngle(1, true);
			PointQty = g_uiPointQuantity[1];
		}
		else if (g_uiProbe == 2)	// Probes 1 & 2
		{
			MinAngle = min(GetMinimumAngle(0, true), GetMinimumAngle(1, true));
			PointQty = max(g_uiPointQuantity[0], g_uiPointQuantity[1]);
		}
		else
			return Width;

		Width = cos(MinAngle) * (PointQty - 1) * Step;
	}
	else if (MANAGE_REBOUND == 1)
		Width = g_dPieceThickness;

	return Width;
}

double GetXAxisMin()
{
	double XMin = 0.0;

	if (g_uiProbe == 0)		// Only probe 1
	{
		double MinAngle = GetMinimumAngle(0, false);

		if (MinAngle < 0.0)
			XMin = GetMinimumEmergence(0) + sin(MinAngle) * (g_uiPointQuantity[0] - 1) * GetPointStep();
		else
			XMin = GetMinimumEmergence(0);
	}
	else if (g_uiProbe == 1)	// Only probe 2
	{
		double MinAngle = GetMinimumAngle(1, false);

		if (MinAngle < 0.0)
			XMin = GetMinimumEmergence(1) + sin(MinAngle) * (g_uiPointQuantity[1] - 1) * GetPointStep();
		else
			XMin = GetMinimumEmergence(0);
	}
	else if (g_uiProbe == 2)	// Probes 1 & 2
	{
		double MinAngle1 = GetMinimumAngle(0, false);
		double MinAngle2 = GetMinimumAngle(1, false);
		double XMin1, XMin2;

		if (MinAngle1 < 0.0)
			XMin1 = GetMinimumEmergence(0) + sin(MinAngle1) * (g_uiPointQuantity[0] - 1) * GetPointStep();
		else
			XMin1 = GetMinimumEmergence(0);

		if (MinAngle2 < 0.0)
			XMin2 = GetMinimumEmergence(0) + sin(MinAngle2) * (g_uiPointQuantity[0] - 1) * GetPointStep();
		else
			XMin2 = GetMinimumEmergence(0);

		XMin = min(XMin1, XMin2);
	}

	return XMin;
}

double GetXAxisMax()
{
	double XMax = 0.0;

	if (g_uiProbe == 0)		// Only probe 1
	{
		XMax = GetMaximumEmergence(0) + sin(GetMaximumAngle(0)) * (g_uiPointQuantity[0] - 1) * GetPointStep();
	}
	else if (g_uiProbe == 1)	// Only probe 2
	{
		XMax = GetMaximumEmergence(1);
	}
	else if (g_uiProbe == 2)	// Probes 1 & 2
	{
		XMax = GetMaximumEmergence(1);
	}

	return XMax;
}

double GetYAxisMin()
{
	return 0.0;
}

double GetYAxisMax()
{
	return g_dEncoderAxisLength;
}

double GetPointStep()
{
	// D = V * T / 2 (for half path)
	return System.dVelocity * System.dDigitizingPeriod / 2.0;
}

double GetMinimumAngle(int Probe, bool Abs)
{
	double Angle = 90.0;

	if (Probe == 0)
	{
		for (int i = 0 ; i < InfoProbe1.iCycleCount ; i++)
		{
			if (Abs == false)
				Angle = min(Angle, InfoProbe1.pdAngle[i]);
			else
				Angle = min(Angle, abs(InfoProbe1.pdAngle[i]));
		}
	}
	else if (Probe == 1)
	{
		for (int i = 0 ; i < InfoProbe2.iCycleCount ; i++)
		{
			if (Abs == false)
				Angle = min(Angle, InfoProbe2.pdAngle[i]);
			else
				Angle = min(Angle, abs(InfoProbe2.pdAngle[i]));
		}
	}

	return Angle;
}

double GetMaximumAngle(int Probe)
{
	double Angle = -90.0;

	if (Probe == 0)
	{
		for (int i = 0 ; i < InfoProbe1.iCycleCount ; i++)
			Angle = max(Angle, InfoProbe1.pdAngle[i]);
	}
	else if (Probe == 1)
	{
		for (int i = 0 ; i < InfoProbe2.iCycleCount ; i++)
			Angle = max(Angle, InfoProbe2.pdAngle[i]);
	}

	return Angle;
}

double GetMinimumEmergence(int Probe)
{
	double Position = 100.0;
	
	if (Probe == 0)
	{
		for (int i = 0 ; i < InfoProbe1.iCycleCount ; i++)
			Position = min(Position, InfoProbe1.pdX[i]);
	}
	else if (Probe == 1)
	{
		for (int i = 0 ; i < InfoProbe2.iCycleCount ; i++)
			Position = min(Position, InfoProbe2.pdX[i]);
	}

	return Position;
}

double GetMaximumEmergence(int Probe)
{
	double Position = -100.0;
	
	if (Probe == 0)
	{
		for (int i = 0 ; i < InfoProbe1.iCycleCount ; i++)
			Position = max(Position, InfoProbe1.pdX[i]);
	}
	else if (Probe == 1)
	{
		for (int i = 0 ; i < InfoProbe2.iCycleCount ; i++)
			Position = max(Position, InfoProbe2.pdX[i]);
	}

	return Position;
}

bool AllocateAscanAnalysisMemory(int Probe, int EncoderSize, int SliceSize)
{
	if ((Probe == 0) && (AscanAnalysisMemory1))
		DeleteAscanAnalysisMemory(0, EncoderSize);
	else if ((Probe == 1) && (AscanAnalysisMemory2))
		DeleteAscanAnalysisMemory(1, EncoderSize);

	if ((EncoderSize == 0) ||
		(SliceSize == 0))
		return false;
	
	if (Probe == 0)
	{
		EnterCriticalSection(&CRT_ANALYSIS_MEMORY_1);
		AscanAnalysisMemory1 = new char*[EncoderSize];

		for (unsigned int i = 0 ; i < (unsigned int)EncoderSize ; i++)
		{
			AscanAnalysisMemory1[i] = new char[SliceSize];
			memset(AscanAnalysisMemory1[i], 0, SliceSize);
		}

		LeaveCriticalSection(&CRT_ANALYSIS_MEMORY_1);
	}
	else if (Probe == 1)
	{
		EnterCriticalSection(&CRT_ANALYSIS_MEMORY_2);
		AscanAnalysisMemory2 = new char*[EncoderSize];

		for (unsigned int i = 0 ; i < (unsigned int)EncoderSize ; i++)
		{
			AscanAnalysisMemory2[i] = new char[SliceSize];
			memset(AscanAnalysisMemory2[i], 0, SliceSize);
		}

		LeaveCriticalSection(&CRT_ANALYSIS_MEMORY_2);
	}
	else
		return false;
	
	return true;
}

bool DeleteAscanAnalysisMemory(int Probe, int EncoderSize)
{
	unsigned int EncoderPositionQty = GetAnalysisEncoderPositionQuantity();

	if (EncoderPositionQty == 0)
		return false;

	if ((Probe == 0) && (AscanAnalysisMemory1 == NULL))
		return false;
	else if ((Probe == 1) && (AscanAnalysisMemory2 == NULL))
		return false;

	if (Probe == 0)
	{
		EnterCriticalSection(&CRT_ANALYSIS_MEMORY_1);

		for (unsigned int i = 0 ; i < EncoderPositionQty ; i++)
			delete[] AscanAnalysisMemory1[i];
	
		delete[] AscanAnalysisMemory1;
		AscanAnalysisMemory1 = NULL;

		LeaveCriticalSection(&CRT_ANALYSIS_MEMORY_1);
	}
	else if (Probe == 1)
	{
		EnterCriticalSection(&CRT_ANALYSIS_MEMORY_2);

		for (unsigned int i = 0 ; i < EncoderPositionQty ; i++)
			delete[] AscanAnalysisMemory2[i];
	
		delete[] AscanAnalysisMemory2;
		AscanAnalysisMemory2 = NULL;

		LeaveCriticalSection(&CRT_ANALYSIS_MEMORY_2);
	}
	else
		return false;

	return true;
}

unsigned int GetAnalysisEncoderPositionQuantity()
{
	return static_cast<unsigned int>(g_dEncoderAxisLengthAnalysis / SystemAnalysis.dEncoderStep);
}

double GetPointStepAnalysis()
{
	// D = V * T / 2 (for half path)
	return SystemAnalysis.dVelocity * SystemAnalysis.dDigitizingPeriod / 2.0;
}

double GetAnalysisMinimumAngle(int Probe, bool Abs)
{
	double Angle = 90.0;

	if (Probe == 0)
	{
		for (int i = 0 ; i < InfoProbe1Analysis.iCycleCount ; i++)
		{
			if (Abs == false)
				Angle = min(Angle, InfoProbe1Analysis.pdAngle[i]);
			else
				Angle = min(Angle, abs(InfoProbe1Analysis.pdAngle[i]));
		}
	}
	else if (Probe == 1)
	{
		for (int i = 0 ; i < InfoProbe2Analysis.iCycleCount ; i++)
		{
			if (Abs == false)
				Angle = min(Angle, InfoProbe2Analysis.pdAngle[i]);
			else
				Angle = min(Angle, abs(InfoProbe2Analysis.pdAngle[i]));
		}
	}

	return Angle;
}

double GetAnalysisMaximumAngle(int Probe)
{
	double Angle = -90.0;

	if (Probe == 0)
	{
		for (int i = 0 ; i < InfoProbe1Analysis.iCycleCount ; i++)
			Angle = max(Angle, InfoProbe1Analysis.pdAngle[i]);
	}
	else if (Probe == 1)
	{
		for (int i = 0 ; i < InfoProbe2Analysis.iCycleCount ; i++)
			Angle = max(Angle, InfoProbe2Analysis.pdAngle[i]);
	}

	return Angle;
}

double GetAnalysisMinimumEmergence(int Probe)
{
	double Position = 100.0;
	
	if (Probe == 0)
	{
		for (int i = 0 ; i < InfoProbe1Analysis.iCycleCount ; i++)
			Position = min(Position, InfoProbe1Analysis.pdX[i]);
	}
	else if (Probe == 1)
	{
		for (int i = 0 ; i < InfoProbe2Analysis.iCycleCount ; i++)
			Position = min(Position, InfoProbe2Analysis.pdX[i]);
	}

	return Position;
}

double GetAnalysisMaximumEmergence(int Probe)
{
	double Position = -100.0;
	
	if (Probe == 0)
	{
		for (int i = 0 ; i < InfoProbe1Analysis.iCycleCount ; i++)
			Position = max(Position, InfoProbe1Analysis.pdX[i]);
	}
	else if (Probe == 1)
	{
		for (int i = 0 ; i < InfoProbe2Analysis.iCycleCount ; i++)
			Position = max(Position, InfoProbe2Analysis.pdX[i]);
	}

	return Position;
}

double GetAnalysisXAxisMin()
{
	double XMin = 0.0;

	if (g_uiProbeAnalysis == 0)		// Only probe 1
	{
		double MinAngle = GetAnalysisMinimumAngle(0, false);

		if (MinAngle < 0.0)
			XMin = GetAnalysisMinimumEmergence(0) + sin(MinAngle) * (InfoProbe1Analysis.PointQuantity - 1) * GetPointStepAnalysis();
		else
			XMin = GetAnalysisMinimumEmergence(0);
	}
	else if (g_uiProbeAnalysis == 1)	// Only probe 2
	{
		double MinAngle = GetAnalysisMinimumAngle(1, false);

		if (MinAngle < 0.0)
			XMin = GetAnalysisMinimumEmergence(1) + sin(MinAngle) * (InfoProbe2Analysis.PointQuantity - 1) * GetPointStepAnalysis();
		else
			XMin = GetAnalysisMinimumEmergence(0);
	}
	else if (g_uiProbeAnalysis == 2)	// Probes 1 & 2
	{
		double MinAngle1 = GetAnalysisMinimumAngle(0, false);
		double MinAngle2 = GetAnalysisMinimumAngle(1, false);
		double XMin1, XMin2;

		if (MinAngle1 < 0.0)
			XMin1 = GetAnalysisMinimumEmergence(0) + sin(MinAngle1) * (InfoProbe1Analysis.PointQuantity - 1) * GetPointStepAnalysis();
		else
			XMin1 = GetAnalysisMinimumEmergence(0);

		if (MinAngle2 < 0.0)
			XMin2 = GetAnalysisMinimumEmergence(0) + sin(MinAngle2) * (InfoProbe2Analysis.PointQuantity - 1) * GetPointStepAnalysis();
		else
			XMin2 = GetAnalysisMinimumEmergence(0);

		XMin = min(XMin1, XMin2);
	}

	return XMin;
}

double GetAnalysisXAxisMax()
{
	double XMax = 0.0;

	if (g_uiProbeAnalysis == 0)			// Only probe 1
		XMax = GetAnalysisMaximumEmergence(0) + sin(GetAnalysisMaximumAngle(0)) * (InfoProbe1Analysis.PointQuantity - 1) * GetPointStepAnalysis();
	else if (g_uiProbeAnalysis == 1)	// Only probe 2
		XMax = GetAnalysisMaximumEmergence(1);
	else if (g_uiProbeAnalysis == 2)	// Probes 1 & 2
		XMax = GetAnalysisMaximumEmergence(1);

	return XMax;
}

double GetAnalysisYAxisMin()
{
	return 0.0;
}

double GetAnalysisYAxisMax()
{
	return g_dEncoderAxisLengthAnalysis;
}

double GetAnalysisYAxisMax(double dAngle)
{
	return g_dEncoderAxisLengthAnalysis + sin(dAngle) * g_dPieceThicknessAnalysis;
}

double GetAnalysisZAxisMin()
{
	return 0.0;
}

double GetAnalysisZAxisMax()
{
	return g_dPieceThicknessAnalysis;
}

void UpdateAxisLimitation(int Mode)
{
	if (Mode == 0)
	{
		Analysis_X_Axis_MIN = GetXAxisMin();
		Analysis_X_Axis_MAX = GetXAxisMax();
		Analysis_Y_Axis_MIN = GetYAxisMin();
		Analysis_Y_Axis_MAX = GetYAxisMax();
		Analysis_Z_Axis_MIN = 0.0;
		Analysis_Z_Axis_MAX = g_dPieceThickness;//integration g_dEncoderAxisLength;

		Analysis_YPOS = 0.0;
		Analysis_XMIN = Analysis_X_Axis_MIN;
		Analysis_XMAX = Analysis_X_Axis_MAX;
		Analysis_YMIN = Analysis_Y_Axis_MIN;
		Analysis_YMAX = Analysis_Y_Axis_MAX;
		Analysis_ZMIN = Analysis_Z_Axis_MIN;
		Analysis_ZMAX = Analysis_Z_Axis_MAX;
	}
	else
	{
		Analysis_X_Axis_MIN = GetAnalysisXAxisMin();
		Analysis_X_Axis_MAX = GetAnalysisXAxisMax();
		Analysis_Y_Axis_MIN = GetAnalysisYAxisMin();
		Analysis_Y_Axis_MAX = GetAnalysisYAxisMax();
		Analysis_Z_Axis_MIN = GetAnalysisZAxisMin();
		Analysis_Z_Axis_MAX = GetAnalysisZAxisMax();

		Analysis_YPOS = 0.0;
		Analysis_XMIN = Analysis_X_Axis_MIN;
		Analysis_XMAX = Analysis_X_Axis_MAX;
		Analysis_YMIN = Analysis_Y_Axis_MIN;
		Analysis_YMAX = Analysis_Y_Axis_MAX;
		Analysis_ZMIN = Analysis_Z_Axis_MIN;
		Analysis_ZMAX = Analysis_Z_Axis_MAX;
	}
}