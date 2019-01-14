
#include "stdafx.h"
#include "DataFile.h"
#include "ExternalDefinition.h"

CDataFile::CDataFile()
{
	FileName = _T("");

	EncoderSize = 0;
	SliceSize[0] = 0;
	SliceSize[1] = 0;
	CycleQuantity[0] = 0;
	CycleQuantity[1] = 0;
	PointQuantity[0] = 0;
	PointQuantity[1] = 0;

	Angle[0] = NULL;
	Angle[1] = NULL;
	Emergence[0] = NULL;
	Emergence[1] = NULL;

	EncoderAxisLength = 0.0;
	EncoderAxisStep = 0;
	SamplingTime = 0.0;
	Velocity = 0.0;
	PieceThickness = 0.0;

	OpenedRead = FALSE;
	OpenedWrite = FALSE;

	AscanMemory1 = NULL;
	AscanMemory2 = NULL;
	File = NULL;
}

CDataFile::~CDataFile()
{
	if (Angle[0])
		delete[] Angle[0];

	if (Angle[1])
		delete[] Angle[1];

	if (Emergence[0])
		delete[] Emergence[0];

	if (Emergence[1])
		delete[] Emergence[1];

	DeleteMemory();

	if (File)
	{
		File->Close();
		delete File;
		File = NULL;
	}
}

BOOL CDataFile::Open(bool Write)
{
	if ((OpenedRead) || (OpenedWrite))
		return FALSE;

	File = new CFile();

	if (Write == TRUE)
		OpenedWrite = File->Open(FileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	else
		OpenedRead = File->Open(FileName, CFile::modeRead | CFile::typeBinary);

	return (OpenedRead) || (OpenedWrite);
}

void CDataFile::Close()
{
	OpenedRead = FALSE;
	OpenedWrite = FALSE;
	
	if (File)
	{
		File->Close();
		delete File;
		File = NULL;
	}
}

BOOL CDataFile::Read()
{
	if (OpenedRead == FALSE)
		return FALSE;

	//There are Sscan on each point of the encoder axis.
	//On ecah point of the encoder axis, there is a slice of data.
	//below slice means S-scan.
	//Even if the second probe is saved in the file, this is not useful because there only one probe.
	File->Read(&EncoderSize, sizeof(unsigned int));//EncoderSize = acquisition point count on encoder axis.
	File->Read(&SliceSize[0], sizeof(unsigned int));//SliceSize[0] = data size of each S-scan (1 slice), this is for the first probe ([0]).
	File->Read(&SliceSize[1], sizeof(unsigned int));//SliceSize[1] = data size of each S-scan (1 slice), this is for the second probe ([1]).
	File->Read(&CycleQuantity[0], sizeof(unsigned int));//focal law count for the first probe ([0]).
	File->Read(&CycleQuantity[1], sizeof(unsigned int));//focal law count for the second probe ([1]).
	File->Read(&PointQuantity[0], sizeof(unsigned int));//ascan point count for the first probe ([0]).
	File->Read(&PointQuantity[1], sizeof(unsigned int));//ascan point count for the second probe ([1]).
	File->Read(&EncoderAxisLength, sizeof(double));//EncoderAxisLength = acquisition length (encoder axis), unit is meter.
	File->Read(&EncoderAxisStep, sizeof(double));//EncoderAxisStep = acquisition step (encoder axis), unit is meter.
	File->Read(&SamplingTime, sizeof(double));//digitizing period (ascan data), unit is second.
	File->Read(&Velocity, sizeof(double));//Velocity = specimen elocity, unit is meter/second.
	File->Read(&PieceThickness, sizeof(double));//PieceThickness = specimen thickness, unit is meter.

	if (Angle[0])//Angle[0] = angle array for each focal law (first probe), unit is radian.
				//Angle[0][0] = angle of the first focal law (first probe), unit is radian.
				//Angle[0][1] = angle of the second focal law (first probe), unit is radian.
		delete[] Angle[0];

	if (Angle[1])//Angle[1] = angle array for each focal law (second probe), unit is radian.
		delete[] Angle[1];

	if (Emergence[0])//Emergence[0] = emergence array for each focal law (first probe), unit is meter.
					//emergence is the position of the emergence point of the beam on the specimen surface.
		delete[] Emergence[0];

	if (Emergence[1])
		delete[] Emergence[1];

	Angle[0] = new double[CycleQuantity[0]];
	Angle[1] = new double[CycleQuantity[1]];
	Emergence[0] = new double[CycleQuantity[0]];
	Emergence[1] = new double[CycleQuantity[1]];

	for (unsigned int i = 0 ; i < 2 ; i++)
	{
		for (unsigned int j = 0 ; j < CycleQuantity[i] ; j++)
		{
			File->Read(&Angle[i][j], sizeof(double));
			File->Read(&Emergence[i][j], sizeof(double));
		}
	}

	AllocateMemory();
	
	for (int i = 0 ; i < (int)EncoderSize ; i++)
		File->Read(AscanMemory1[i], SliceSize[0] * sizeof(char));
	
	for (int i = 0 ; i < (int)EncoderSize ; i++)
		File->Read(AscanMemory2[i], SliceSize[1] * sizeof(char));

	return TRUE;
}

BOOL CDataFile::Write()
{
	if ((Angle[0] == NULL) ||
		(Angle[1] == NULL) ||
		(Emergence[0] == NULL) ||
		(Emergence[1] == NULL) ||
		(AscanMemory1 == NULL) ||
		(AscanMemory2 == NULL) ||
		(OpenedWrite == FALSE) ||
		(File == NULL))
		return FALSE;

	File->Write(&EncoderSize, sizeof(unsigned int));
	File->Write(&SliceSize[0], sizeof(unsigned int));
	File->Write(&SliceSize[1], sizeof(unsigned int));
	File->Write(&CycleQuantity[0], sizeof(unsigned int));
	File->Write(&CycleQuantity[1], sizeof(unsigned int));
	File->Write(&PointQuantity[0], sizeof(unsigned int));
	File->Write(&PointQuantity[1], sizeof(unsigned int));
	File->Write(&EncoderAxisLength, sizeof(double));
	File->Write(&EncoderAxisStep, sizeof(double));
	File->Write(&SamplingTime, sizeof(double));
	File->Write(&Velocity, sizeof(double));
	File->Write(&PieceThickness, sizeof(double));

	for (unsigned int i = 0 ; i < 2 ; i++)
	{
		for (unsigned int j = 0 ; j < CycleQuantity[i] ; j++)
		{
			File->Write(&Angle[i][j], sizeof(double));
			File->Write(&Emergence[i][j], sizeof(double));
		}
	}

	for (int i = 0 ; i < (int)EncoderSize ; i++)
		File->Write(AscanMemory1[i], SliceSize[0] * sizeof(char));
	
	for (int i = 0 ; i < (int)EncoderSize ; i ++)
		File->Write(AscanMemory2[i], SliceSize[1] * sizeof(char));

	return TRUE;
}

void CDataFile::AllocateMemory()
{
	if ((AscanMemory1) || (AscanMemory2))
		DeleteMemory();

	AscanMemory1 = new char*[EncoderSize];

	for (unsigned int i = 0 ; i < EncoderSize ; i++)
	{
		AscanMemory1[i] = new char[SliceSize[0]];
		memset(AscanMemory1[i], 0, SliceSize[0]);
	}

	AscanMemory2 = new char*[EncoderSize];

	for (unsigned int i = 0 ; i < EncoderSize ; i++)
	{
		AscanMemory2[i] = new char[SliceSize[1]];
		memset(AscanMemory2[i], 0, SliceSize[1]);
	}
}

void CDataFile::DeleteMemory()
{
	if (AscanMemory1)
	{
		for (unsigned int i = 0 ; i < EncoderSize ; i++)
			delete[] AscanMemory1[i];
	
		delete[] AscanMemory1;
		AscanMemory1 = NULL;
	}

	if (AscanMemory2)
	{
		for (unsigned int i = 0 ; i < EncoderSize ; i++)
			delete[] AscanMemory2[i];
	
		delete[] AscanMemory2;
		AscanMemory2 = NULL;
	}
}