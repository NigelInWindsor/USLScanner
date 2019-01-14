
#pragma once

class CDataFile : public CObject
{

public:
	CDataFile();
	virtual~ CDataFile();
	CString FileName;

	BOOL Open(bool Write);
	BOOL OpenedRead;
	BOOL OpenedWrite;
	void Close();

	BOOL Write();
	BOOL Read();

	void AllocateMemory();
	void DeleteMemory();

	unsigned int EncoderSize;
	unsigned int SliceSize[2];
	unsigned int CycleQuantity[2];
	unsigned int PointQuantity[2];

	double* Angle[2];
	double* Emergence[2];
	double EncoderAxisLength;
	double EncoderAxisStep;
	double SamplingTime;
	double Velocity;
	double PieceThickness;

	char** AscanMemory1;
	char** AscanMemory2;
	
protected:
	CFile* File;
};