
#include "stdafx.h"
#include "ExternalDefinition.h"
#include "Tools.h"
#include "DynamicCscopeView.h"
#include "HardwareCscopeView.h"
#include "SectorApp.h"
#include "UTKernelStream.h"

char** AscanMemory1 = NULL;
char** AscanMemory2 = NULL;
BYTE* CscanAmpMemory1[4] = {NULL,NULL,NULL,NULL};
BYTE* CscanAmpMemory2[4] = {NULL,NULL,NULL,NULL};
short* CscanTofMemory1[4] = {NULL,NULL,NULL,NULL};
short* CscanTofMemory2[4] = {NULL,NULL,NULL,NULL};
int g_iCscanAmpMemorySize1=0;

CRITICAL_SECTION CRT_MEMORY_1;
CRITICAL_SECTION CRT_MEMORY_2;
CRITICAL_SECTION CRT_UPDATE_SETTING;

#ifdef _TEST_DATA
	unsigned int g_uiCycleQuantity[2] = {0, 0};
	unsigned int g_uiPointQuantity[2] = {0, 0};
	unsigned int g_uiProbe = 0;
	double g_dMinimumAngle[2] = {0.0, 0.0};
	double g_dStepAngle[2] = {0.0, 0.0};
	double g_dEncoderAxisLength = 0.01;
	double g_dEncoderAxisStep = 0.001;
	double g_dPieceThickness = 0.100;
	double g_dVelocity = 5900.0;
	double g_dSamplingPeriod = 0.000000020;
	bool g_bSettingBeingReset = true;
#else
	unsigned int g_uiCycleQuantity[2] = {50, 50};
	unsigned int g_uiPointQuantity[2] = {1500, 1500};
	unsigned int g_uiProbe = 0;
	double g_dMinimumAngle[2] = {20.0 * PI / 180.0, -70.0 * PI / 180.0};
	double g_dStepAngle[2] = {1.0 * PI / 180.0, 1.0 * PI / 180.0};
	double g_dEncoderAxisLength = 0.2;
	double g_dEncoderAxisStep = 0.001;
	double g_dPieceThickness = 0.100;
	double g_dVelocity = 5900.0;
	double g_dSamplingPeriod = 0.000000020;
	bool g_bSettingBeingReset = true;
#endif

structInfo InfoProbe1;
structInfo InfoProbe2;
structSystem System;
unsigned int g_LastAscanEncoderPosition = 0;
unsigned int g_LastCscanEncoderPosition = 0;
int g_iAscanLastEncoderPosition = 0;//integration
int g_iCscanLastEncoderPosition[4+4] = {0,0,0,0,0,0,0,0};//integration
int g_iCscanInspectionCount=0;//integration
int g_aiCscanInspectionCount[4+4] = {0,0,0,0,0,0,0,0};//integration
bool g_bAscanDisplayLastButOne=true;//integration
bool g_bCscanDisplayLastButOne=true;//integration
int g_iAscanPreviousEncoder=-1;//integration
int g_iCscanPreviousEncoder=-1;//integration
int g_iAscanPreviousCycle=-1;//integration
int g_iCscanPreviousCycle=-1;//integration
double g_dIncAscanPreviousEncoder=-1.0e10;//integration
double g_dIncCscanPreviousEncoder=-1.0e10;//integration
bool g_bMechanicalDirectionForward=true;//integration
int g_bMechanicalOutOfRange=0;//integration

void AllocDisplay2(bool bAlloc)//integration
{
	if(!bAlloc)
		g_bMechanicalOutOfRange++;

	g_iAscanPreviousEncoder = -1;
	g_iAscanPreviousCycle = -1;
	g_dIncAscanPreviousEncoder = -1.0e10;

	g_iCscanPreviousEncoder = -1;
	g_iCscanPreviousCycle = -1;
	g_dIncCscanPreviousEncoder = -1.0e10;

	g_iCscanInspectionCount = 0;
	for(int iGate=0;iGate<4+4;iGate++)
		g_aiCscanInspectionCount[iGate] = 0;
}

char** AscanAnalysisMemory1 = NULL;
char** AscanAnalysisMemory2 = NULL;

CRITICAL_SECTION CRT_ANALYSIS_MEMORY_1;
CRITICAL_SECTION CRT_ANALYSIS_MEMORY_2;

structInfo InfoProbe1Analysis;
structInfo InfoProbe2Analysis;
structSystem SystemAnalysis;

unsigned int g_uiProbeAnalysis = 0;
unsigned int g_uiMode = 0;
double g_dEncoderAxisLengthAnalysis = 0.01;
double g_dPieceThicknessAnalysis = 0.100;

double Analysis_YPOS = 0.0;
double Analysis_XMIN = 0.0;
double Analysis_XMAX = 0.0;
double Analysis_YMIN = 0.0;
double Analysis_YMAX = 0.0;
double Analysis_ZMIN = 0.0;
double Analysis_ZMAX = 0.0;

double Analysis_X_Axis_MIN = 0.0;
double Analysis_X_Axis_MAX = 0.0;
double Analysis_Y_Axis_MIN = 0.0;
double Analysis_Y_Axis_MAX = 0.0;
double Analysis_Z_Axis_MIN = 0.0;
double Analysis_Z_Axis_MAX = 0.0;

void AllocDisplay(bool bAlloc);//integration
int iround(double x)
{
	int nx = (int)x;
	if(x>0)
	{
		if((x-nx)>0.5)
			return nx+1;
		else
			return nx;
	}else{
		if((x-nx)<-0.5)
			return nx-1;
		else
			return nx;
	}
}
bool acq_data_ascan(int Probe, int Cycle, double Encoder, int DataCount, const char* Data)
{
	EnterCriticalSection(&CRT_UPDATE_SETTING);

	//integration start
	if(!IsMechanicalManagement(true))
	{
		if(g_dIncAscanPreviousEncoder<-1.0e10/2)
			g_dIncAscanPreviousEncoder = Encoder;
		if(g_dIncAscanPreviousEncoder<Encoder)//integration
		{
			g_dIncAscanPreviousEncoder = Encoder;
			if(!g_bMechanicalDirectionForward)
			{
				g_bMechanicalDirectionForward = true;
				if (Encoder <= -10*System.dEncoderStep)
				{
					//reset display
					AllocDisplay(false);
					AllocDisplay(true);
					//AutoResetAcq(Encoder);it does not work
				}
			}
		}else if(g_dIncAscanPreviousEncoder>Encoder)//integration
		{
			g_dIncAscanPreviousEncoder = Encoder;
			if(g_bMechanicalDirectionForward)
			{
				g_bMechanicalDirectionForward = false;
				if (Encoder >= g_dEncoderAxisLength+10*System.dEncoderStep)
				{
					//reset display
					AllocDisplay(false);
					AllocDisplay(true);
				}
			}
		}
	}
	//integration stop
	if ((Encoder >= 0.0) && (Encoder <= g_dEncoderAxisLength))
	{
		int EncoderPosition = (int)iround(Encoder / System.dEncoderStep);
		int max = GetEncoderPositionQuantity();
		//TRACE(_T("Encoder = %.12f\r\n"), Encoder);

		if (EncoderPosition < max)
		{
			if ((Probe == 0) && (AscanMemory1))
			{
				EnterCriticalSection(&CRT_MEMORY_1);
				memcpy(AscanMemory1[EncoderPosition] + Cycle * DataCount, Data, DataCount * sizeof(char));
				LeaveCriticalSection(&CRT_MEMORY_1);
if(g_bAscanDisplayLastButOne)
{
	if((g_iAscanPreviousCycle>=0) && Cycle<g_iAscanPreviousCycle)//integration (reloop)
	{//previous data are completed, ready to be displayed.
		g_LastAscanEncoderPosition = g_iAscanPreviousEncoder;
	}
	g_iAscanPreviousEncoder=EncoderPosition;//integration
	g_iAscanPreviousCycle=Cycle;//integration
}else
				g_LastAscanEncoderPosition = EncoderPosition;
				LeaveCriticalSection(&CRT_UPDATE_SETTING);
				return true;
			}
			else if ((Probe == 1) && (AscanMemory2))
			{
				EnterCriticalSection(&CRT_MEMORY_2);
				memcpy(AscanMemory2[EncoderPosition] + Cycle * DataCount, Data, DataCount * sizeof(char));
				LeaveCriticalSection(&CRT_MEMORY_2);
				g_LastAscanEncoderPosition = EncoderPosition;

				LeaveCriticalSection(&CRT_UPDATE_SETTING);
				return true;
			}
		}
	}

	LeaveCriticalSection(&CRT_UPDATE_SETTING);
	return false;
}

bool acq_data_cscan(int Probe, int Cycle, double Encoder, int DataCount, const structCscanAmp_0x0102 *pAmp, const structCscanAmpTof_0x0202 *pAmpTof)
{
	int iGate,iPos;
	int iCycleCount=InfoProbe1.iCycleCount;
	char cAmp;
//#ifndef OLD_CSCAN_FORMAT
//	if(Cycle==iCycleCount)
//		Cycle = iCycleCount-1;
//#endif OLD_CSCAN_FORMAT

	EnterCriticalSection(&CRT_UPDATE_SETTING);

	//integration start
	if(!IsMechanicalManagement(false))
	{
		if(!g_pDynamicCscope)
		if(g_dIncCscanPreviousEncoder<-1.0e10/2)
			g_dIncCscanPreviousEncoder = Encoder;
		if(g_dIncCscanPreviousEncoder<Encoder)//integration
		{
			g_dIncCscanPreviousEncoder = Encoder;
			if(!g_bMechanicalDirectionForward)
			{
				g_bMechanicalDirectionForward = true;
				if (Encoder <= -10*System.dEncoderStep)
				{
					//reset display
					AllocDisplay(false);
					AllocDisplay(true);
					//AutoResetAcq(Encoder);it does not work
				}
			}
		}else if(g_dIncCscanPreviousEncoder>Encoder)//integration
		{
			g_dIncCscanPreviousEncoder = Encoder;
			if(g_bMechanicalDirectionForward)
			{
				g_bMechanicalDirectionForward = false;
				if (Encoder >= g_dEncoderAxisLength+10*System.dEncoderStep)
				{
					//reset display
					AllocDisplay(false);
					AllocDisplay(true);
				}
			}
		}
	}
	//integration stop
	if ((Encoder >= 0.0) && (Encoder <= g_dEncoderAxisLength))
	{
		int EncoderPosition = (int)iround(Encoder / System.dEncoderStep);
		int max = GetEncoderPositionQuantity();
		//TRACE(_T("Encoder = %.12f\r\n"), Encoder);

		if (EncoderPosition < max)
		{
			if (Probe == 0)
			{
				EnterCriticalSection(&CRT_MEMORY_1);
				if (CscanAmpMemory1[0] && CscanAmpMemory1[3] && pAmp && !pAmpTof)
				{
					for(int iIndex=0;iIndex<DataCount;iIndex++)
					{
						iGate = pAmp[iIndex].gateId;
						iPos = EncoderPosition*iCycleCount+Cycle;
						if((0<=iPos) && (iPos<g_iCscanAmpMemorySize1))
						if((0<=iGate) && (iGate<4))
						{
							//CscanAmpMemory1[iGate][iPos] = (BYTE)pAmp[iIndex].byAmp;
							//CscanAmpMemory1[iGate][iPos] = (BYTE)(0x10*(iGate+1)+0x50*Cycle);//pAmp[iIndex].byAmp;
							//CscanAmpMemory1[iGate][iPos] = (BYTE)(0x10*(iGate+1)+EncoderPosition);//pAmp[iIndex].byAmp;
							cAmp= (char)pAmp[iIndex].byAmp;
							if(pAmp[iIndex].sign)
							{
								//-128 (0x80) => 254; 127 => 254.
								if(cAmp<0)
									cAmp = -cAmp-1;
								CscanAmpMemory1[iGate][iPos] = (BYTE)(cAmp*2);
							}else
								CscanAmpMemory1[iGate][iPos] = (BYTE)cAmp;
						}
					}
				}
				if (CscanAmpMemory1[0] && CscanTofMemory1[0] && CscanAmpMemory1[3] && CscanTofMemory1[3] && pAmpTof)
				{
					for(int iIndex=0;iIndex<DataCount;iIndex++)
					{
						iGate = pAmpTof[iIndex].gateId;
						iPos = EncoderPosition*iCycleCount+Cycle;
						if((0<=iPos) && (iPos<g_iCscanAmpMemorySize1))
						if((0<=iGate) && (iGate<4))
						{
							cAmp= (char)pAmpTof[iIndex].byAmp;
							if(pAmpTof[iIndex].sign)
							{
								//-128 (0x80) => 254; 127 => 254.
								if(cAmp<0)
									cAmp = -cAmp-1;
								CscanAmpMemory1[iGate][iPos] = (BYTE)(cAmp*2);
							}else
								CscanAmpMemory1[iGate][iPos] = (BYTE)cAmp;
							CscanTofMemory1[iGate][iPos] = pAmpTof[iIndex].wTof;
							//CscanAmpMemory1[iGate][iPos] = (BYTE)(0x10*(iGate+1)+0x50*Cycle);
							//CscanTofMemory1[iGate][iPos] = (short)(0x10*(iGate+1)+0x50*Cycle);
						}
					}
				}
				LeaveCriticalSection(&CRT_MEMORY_1);
if(g_bCscanDisplayLastButOne)
{
	if(!EncoderPosition && !Cycle && (g_iCscanPreviousEncoder>=0))
		g_iCscanInspectionCount++;//new inspection (usefull for internal synchro)
	if((g_iCscanPreviousCycle>=0) && Cycle<g_iCscanPreviousCycle)//integration (reloop)
	{//previous data are completed, ready to be displayed.
		g_LastCscanEncoderPosition = g_iCscanPreviousEncoder;
	}
	g_iCscanPreviousEncoder=EncoderPosition;//integration
	g_iCscanPreviousCycle=Cycle;//integration
}else
				g_LastCscanEncoderPosition = EncoderPosition;
				LeaveCriticalSection(&CRT_UPDATE_SETTING);
				return true;
			}
			else if (Probe == 1)
			{
				EnterCriticalSection(&CRT_MEMORY_2);
				if (CscanAmpMemory2[0] && CscanAmpMemory2[3] && !CscanTofMemory2)
				{
					for(int iIndex=0;iIndex<DataCount;iIndex++)
					{
						iGate = pAmp[iIndex].gateId;
						CscanAmpMemory2[iGate][EncoderPosition*iCycleCount+iIndex] = pAmp[iIndex].byAmp;
					}
				}
				if (CscanAmpMemory2[0] && CscanTofMemory2[0] && CscanAmpMemory2[3] && CscanTofMemory2[3])
				{
					for(int iIndex=0;iIndex<DataCount;iIndex++)
					{
						iGate = pAmp[iIndex].gateId;
						CscanAmpMemory2[iGate][EncoderPosition*iCycleCount+iIndex] = pAmpTof[iIndex].byAmp;
						CscanTofMemory2[iGate][EncoderPosition*iCycleCount+iIndex] = pAmpTof[iIndex].wTof;
					}
				}
				LeaveCriticalSection(&CRT_MEMORY_2);
				g_LastCscanEncoderPosition = EncoderPosition;

				LeaveCriticalSection(&CRT_UPDATE_SETTING);
				return true;
			}
		}
	}

	LeaveCriticalSection(&CRT_UPDATE_SETTING);
	return false;
}

void InitProgram()
{
	::InitializeCriticalSection(&CRT_MEMORY_1);
	::InitializeCriticalSection(&CRT_MEMORY_2);
	::InitializeCriticalSection(&CRT_UPDATE_SETTING);

	::InitializeCriticalSection(&CRT_ANALYSIS_MEMORY_1);
	::InitializeCriticalSection(&CRT_ANALYSIS_MEMORY_2);

#ifndef _TEST_DATA
	
	InfoProbe1.iCycleCount = g_uiCycleQuantity[0];
	InfoProbe1.PointQuantity = g_uiPointQuantity[0];
	InfoProbe1.pdAngle = new double[CYCLE_QUANTITY_1_MAX];
	InfoProbe1.pdX = new double[CYCLE_QUANTITY_1_MAX];

	InfoProbe2.iCycleCount = g_uiCycleQuantity[1];
	InfoProbe2.PointQuantity = g_uiPointQuantity[1];
	InfoProbe2.pdAngle = new double[CYCLE_QUANTITY_2_MAX];
	InfoProbe2.pdX = new double[CYCLE_QUANTITY_2_MAX];

	System.dDigitizingPeriod = g_dSamplingPeriod;
	System.dEncoderStep = g_dEncoderAxisStep;
	System.dVelocity = g_dVelocity;

	InfoProbe1Analysis.pdAngle = new double[CYCLE_QUANTITY_1_MAX];
	InfoProbe1Analysis.pdX = new double[CYCLE_QUANTITY_2_MAX];

	InfoProbe2Analysis.pdAngle = new double[CYCLE_QUANTITY_1_MAX];
	InfoProbe2Analysis.pdX = new double[CYCLE_QUANTITY_2_MAX];
#endif
}

void CloseProgram()
{
	DeleteAscanMemory(0);
	DeleteAscanMemory(1);

	::DeleteCriticalSection(&CRT_MEMORY_1);
	::DeleteCriticalSection(&CRT_MEMORY_2);
	::DeleteCriticalSection(&CRT_UPDATE_SETTING);

	delete[] InfoProbe1.pdAngle;
	delete[] InfoProbe1.pdX;
	delete[] InfoProbe2.pdAngle;
	delete[] InfoProbe2.pdX;

	DeleteAscanAnalysisMemory(0, (int)(g_dEncoderAxisLengthAnalysis / SystemAnalysis.dEncoderStep));
	DeleteAscanAnalysisMemory(1, (int)(g_dEncoderAxisLengthAnalysis / SystemAnalysis.dEncoderStep));

	::DeleteCriticalSection(&CRT_ANALYSIS_MEMORY_1);
	::DeleteCriticalSection(&CRT_ANALYSIS_MEMORY_2);

	delete[] InfoProbe1Analysis.pdAngle;
	delete[] InfoProbe1Analysis.pdX;
	delete[] InfoProbe2Analysis.pdAngle;
	delete[] InfoProbe2Analysis.pdX;
}