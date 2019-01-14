#pragma once

#include "ExternalDefinition.h"

unsigned int GetEncoderPositionQuantity();
unsigned int GetOneSliceSize(int Probe);
unsigned int GetCycleCount(int Probe);

bool AllocateAscanMemory(int Probe);
bool DeleteAscanMemory(int Probe);
bool AllocateCscanMemory(int Probe);
bool DeleteCscanMemory(int Probe);

double GetZAxisWidth();
double GetXAxisMin();
double GetXAxisMax();
double GetYAxisMin();
double GetYAxisMax();
double GetPointStep();

double GetMinimumAngle(int Probe, bool Abs);
double GetMaximumAngle(int Probe);
double GetMinimumEmergence(int Probe);
double GetMaximumEmergence(int Probe);

bool AllocateAscanAnalysisMemory(int Probe, int EncoderSize, int SliceSize);
bool DeleteAscanAnalysisMemory(int Probe, int EncoderSize);

unsigned int GetAnalysisEncoderPositionQuantity();
double GetPointStepAnalysis();
double GetAnalysisMinimumAngle(int Probe, bool Abs);
double GetAnalysisMaximumAngle(int Probe);
double GetAnalysisMinimumEmergence(int Probe);
double GetAnalysisMaximumEmergence(int Probe);

double GetAnalysisXAxisMin();
double GetAnalysisXAxisMax();
double GetAnalysisYAxisMin();
double GetAnalysisYAxisMax();
double GetAnalysisZAxisMin();
double GetAnalysisZAxisMax();
int GetAnalysisCycle();

void UpdateAxisLimitation(int mode);