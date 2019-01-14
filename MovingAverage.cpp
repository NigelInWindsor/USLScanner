#include "StdAfx.h"
#include "MovingAverage.h"

CMovingAverage::CMovingAverage()
{
	Reset();
}

CMovingAverage::~CMovingAverage()
{
}

void CMovingAverage::Reset()
{
	CStats::Reset();
	m_stats.Reset();
	m_upToDate = false;
	m_nRecorded = 0;
	m_dataIt = m_data.begin();
}

void CMovingAverage::Add(float val)
{
	CStats::Add(val); 
	if (m_nSamples == 0) return; //Add to infinite average data (CStats).
	if (m_dataIt == m_data.end()) { //Add to moving average data. Loop back to beginning if at end.
		m_dataIt = m_data.begin(); 
	}
	(*m_dataIt) = val; 
	m_dataIt++; 
	m_upToDate = false; //Need to recompute the stats as the data has been changed.
	if (m_nRecorded < m_nSamples) m_nRecorded++;
}

float CMovingAverage::Last()
{
	if (!ComputeStats()) return CStats::Last();
	return m_stats.Last();
}

float CMovingAverage::Mean()
{
	if (!ComputeStats()) return CStats::Mean();
	return m_stats.Mean();
}

float CMovingAverage::STDev()
{
	if (!ComputeStats()) return CStats::STDev();
	return m_stats.STDev();
}

float CMovingAverage::STDErr()
{
	if (!ComputeStats()) return CStats::STDErr();
	return m_stats.STDErr();
}

float CMovingAverage::Min()
{
	if (!ComputeStats()) return CStats::Min();
	return m_stats.Min();
}

float CMovingAverage::Max()
{
	if (!ComputeStats()) return CStats::Max();
	return m_stats.Max();
}

float CMovingAverage::Count()
{
	if (!m_bMoving) return CStats::Count();
	return (float)m_nRecorded;
}

void CMovingAverage::SetNSamples(int val) { 
	if (val == m_nSamples) return; //No change.
	if (val < 2) {     //Need at least 2 samples.
		m_nSamples = 0; //Infinite average.
		m_bMoving = false;
	}
	else {
		m_nSamples = val;
		m_data.resize(m_nSamples);
		m_bMoving = true;
	}
	Reset();
	m_upToDate = false;
}

bool CMovingAverage::ComputeStats()
{
	if (!m_bMoving || m_nRecorded < m_nSamples) return false; //Unable to compute moving average stats.
	if (!m_upToDate) {
		m_stats.Reset();
		for(const auto &data : m_data) {
			m_stats.Add(data);
		}
		m_upToDate = true;
	}
	return true;
}
