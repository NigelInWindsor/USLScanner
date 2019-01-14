// Laser.h: interface for the CLaser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LASER_H__49F66A13_8DBC_471B_AC7C_98194E96B37B__INCLUDED_)
#define AFX_LASER_H__49F66A13_8DBC_471B_AC7C_98194E96B37B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Crandun Technologies\CTI-HSIF-PCI\SOURCE\C_CPP\CTI_HSIF_PCI.h"
#include "Ethernet.h"
#include <map>

using namespace std;
using namespace Crandun;

#define	MAX_SAMPLES 4096
#define	NSR 4096
#define MAX_DATASETS 100

struct LASER_ETHERNET_MSG {
	union {
		char AllData[PACKET_SIZE];
		struct {
			int Instruction;
			int nDataSize;
			char Data[PACKET_SIZE-8];
		};
	};
};

class CLaser  
{
public:
	void FilterRange(int nLastCount);
	int LaserOn();
	int LaserOff();

	float	m_fRange[NSR];
	int		m_nTime[NSR];


	CLaser();
	virtual ~CLaser();

	bool	FindDevice();
	void	InitializeLaser();
	void	CheckLazerSensorReturnValue(long nResult);
	void	UpdateAllControls();
	void	CalculateData();
	void	TakeTaughtPoints(int nLine=-1, float fMaxZ=0, float fMinZ=0);
	
	int		setAngleOffset(long EncoderNumber, float fOffset);
	int		setMinValidAngle(long EncoderNumber, float fMinAngle);
	int		setMaxValidAngle(long EncoderNumber, float fMaxAngle);
	int		setMinValidRange(float fMinRange); 
	int		setMaxValidRange(float fMaxRange);
	int		setSamplesPerSec(long nSamplesPerSec);
	int		setMotorRPM(long nMotor, long nRPM, long nDir);
	int		setDiscardInvalidOff();
	int		setDiscardInvalidOn();
	int		setCallbackThreshold(int nPercent);
	int		setMinValidAmplitude(long amp);
	int		setSingleShotData(bool bSingleShot);
	bool	IsThereAReadingAtMarker();
	void	ResetDataSets();

	bool				m_bSendData;
	CTI_HSIF_PCI		m_LaserScanner;
	HSIF_DATA_PT		m_DataSet[MAX_DATASETS][MAX_SAMPLES];
	bool				m_bDevicePresent;
	CEthernet			m_Ethernet;
	int					m_nDataSet;
	DWORD				m_Timer;
	float				m_fDataSamplePerSec;
	CCoord				m_CpRect[2];
	bool				m_bNewData;
	int					m_nActualAverageLines;
	float				m_fMarkerAngle;
	float				m_fEdgeAngle[2];
	int					m_DataCount;
	bool				m_bDataCollected;
	float				m_fSingleShotData;

	std::map<float,float> m_nRangeDataMap;
	std::map<float,float>::iterator m_nRangeDataMapIter;

	CCriticalSection	m_CriticalSection;

};

#endif // !defined(AFX_LASER_H__49F66A13_8DBC_471B_AC7C_98194E96B37B__INCLUDED_)
