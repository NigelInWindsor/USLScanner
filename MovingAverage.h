#pragma once
#include "Stats.h"
#include <vector>

class CMovingAverage : public CStats
{
public:
	CMovingAverage();
	~CMovingAverage();

	virtual void Reset();
	virtual void Add(float val);
	virtual float Last();
	virtual float Mean();
	virtual float STDev();
	virtual float STDErr();
	virtual float Min();
	virtual float Max();
	virtual float Count();

	void SetNSamples(int val);
	int GetNSamples() { return m_nSamples; }



protected:
	int m_nSamples = 0;
	bool m_bMoving = false;
	int m_nRecorded;
	std::vector<float> m_data;
	std::vector<float>::iterator m_dataIt;
	float m_fSum;
	CStats m_stats;
	bool m_upToDate = false;
private:
	bool ComputeStats();
};

