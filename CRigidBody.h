#pragma once
#include "CoordTracker.h"
#include "MovingAverage.h"
#include <vector>

#ifdef NPTRACKINGTOOLS

class CRigidBody : public CCoordTracker
{
public:
	CRigidBody();
	~CRigidBody();

	bool GetTracked() { return m_bTracked; }
	void SetTracked(bool val) { m_bTracked = val; }
	bool Set(float x, float y, float z, float qx, float qy, float qz, float qw, float yaw, float pitch, float roll);

	bool ZeroOrientation();

	int GetIdent() { return m_ident; }
	void SetIdent(int ident) { m_ident = ident; }

	bool SetOrigin();

	CMovingAverage* GetMAx()     { return &m_MAx; }
	CMovingAverage* GetMAy()     { return &m_MAy; }
	CMovingAverage* GetMAz()     { return &m_MAz; }
	CMovingAverage* GetMAyaw()   { return &m_MAyaw; }
	CMovingAverage* GetMApitch() { return &m_MApitch; }
	CMovingAverage* GetMAroll()  { return &m_MAroll; }
	CMovingAverage* GetMADist()  { return &m_MAdist; }

	virtual bool GetSurfacePos(CCoord * pCpSurface);
	virtual bool GetScanLine(int* val);

	void ResetStats();
	void SetStatsLength(int val);
	bool GetPositionData(D3DXVECTOR3 * pos, D3DXVECTOR3 * angle, D3DXQUATERNION * quat, float * dist);
	int GetStatsLength() { return m_MAx.GetNSamples(); }

	HANDLE m_positionUpdateSemaphore;

private:
	CMovingAverage m_MAyaw, m_MApitch, m_MAroll, m_MAx, m_MAy, m_MAz, m_MAqx, m_MAqy, m_MAqz, m_MAqw, m_MAdist; //Moving averages
	std::vector<CMovingAverage*> m_vpMA;
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_origin = { 0,0,0 };
	D3DXVECTOR3 m_angle;
	D3DXQUATERNION m_quaternion;
	float m_dist; //Distance from the origin.
	bool m_bTracked; //Is being tracked?
	int m_ident=0; //Rigid body identifier
	CCriticalSection m_Crit;

};

#endif //#ifdef NPTRACKINGTOOLS