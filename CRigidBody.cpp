
#include "stdafx.h"
#include "USLScanner.h"


#ifdef NPTRACKINGTOOLS
#define RB_TIMEOUT_MS 10


CRigidBody::CRigidBody() : CCoordTracker()
{

	m_vpMA.push_back(&m_MAyaw);
	m_vpMA.push_back(&m_MApitch);
	m_vpMA.push_back(&m_MAroll);
	m_vpMA.push_back(&m_MAx);
	m_vpMA.push_back(&m_MAy);
	m_vpMA.push_back(&m_MAz);
	m_vpMA.push_back(&m_MAqx);
	m_vpMA.push_back(&m_MAqy);
	m_vpMA.push_back(&m_MAqz);
	m_vpMA.push_back(&m_MAqw);
	m_vpMA.push_back(&m_MAdist);
	

	m_positionUpdateSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
}


CRigidBody::~CRigidBody()
{
	WaitForSingleObject(m_positionUpdateSemaphore, 1000);
	m_vpMA.clear();
}

bool CRigidBody::Set(float x, float y, float z, float qx, float qy, float qz, float qw, float yaw, float pitch, float roll)
{
	if (WaitForSingleObject(m_positionUpdateSemaphore, RB_TIMEOUT_MS) != WAIT_OBJECT_0) return false;
	
	m_angle.x = yaw;
	m_angle.y = pitch;
	m_angle.z = roll;

	m_position.x = x;
	m_position.y = y;
	m_position.z = z;

	m_position -= m_origin;

	m_dist = D3DXVec3Length(&m_position);

	m_quaternion.x = qx;
	m_quaternion.y = qy;
	m_quaternion.z = qz;
	m_quaternion.w = qw;

	//Update the stats
	m_MAyaw.Add(m_angle.x);
	m_MApitch.Add(m_angle.y);
	m_MAroll.Add(m_angle.z);

	m_MAx.Add(m_position.x);
	m_MAy.Add(m_position.y);
	m_MAz.Add(m_position.z);

	m_MAqx.Add(m_quaternion.x);
	m_MAqy.Add(m_quaternion.y);
	m_MAqz.Add(m_quaternion.z);
	m_MAqw.Add(m_quaternion.w);

	m_MAdist.Add(m_dist);

	ReleaseSemaphore(m_positionUpdateSemaphore, 1, NULL);
	return true;
}

bool CRigidBody::ZeroOrientation()
{
	if (WaitForSingleObject(m_positionUpdateSemaphore, RB_TIMEOUT_MS) != WAIT_OBJECT_0) return false;
	TT_RigidBodyResetOrientation(m_ident);
	ReleaseSemaphore(m_positionUpdateSemaphore, 1, NULL);
	return true;
}

bool CRigidBody::SetOrigin()
{
	if (WaitForSingleObject(m_positionUpdateSemaphore, RB_TIMEOUT_MS) != WAIT_OBJECT_0) return false;
	 m_origin += m_position; 
	 ReleaseSemaphore(m_positionUpdateSemaphore, 1, NULL);
}


bool CRigidBody::GetSurfacePos(CCoord *pCoord)
{
	
	theApp.m_pV120Trio->Update(this);//Update the coordinates.

	if (WaitForSingleObject(m_positionUpdateSemaphore, RB_TIMEOUT_MS) != WAIT_OBJECT_0) return false;

	int nSide = 0;

	pCoord->Side[nSide].fX = m_position.x;
	pCoord->Side[nSide].fY = m_position.y;
	pCoord->Side[nSide].fZ = m_position.z;
	pCoord->Side[nSide].fI = m_angle.x;
	pCoord->Side[nSide].fJ = m_angle.y;
	pCoord->Side[nSide].fK = m_angle.z;


	if (theApp.m_LastSettings.nDesiredDevice == PHASED_ARRAY) {
		pCoord->Side0.pt += theApp.m_PhasedArray[0].m_FL[CURRENT_FL].vecPt;
	}
	ReleaseSemaphore(m_positionUpdateSemaphore, 1, NULL);
	return true;
}

bool CRigidBody::GetScanLine(int* val)
{
	return false;
}

void CRigidBody::ResetStats()
{
	for (const auto &ma : m_vpMA) {
		ma->Reset();
	}
}

void CRigidBody::SetStatsLength(int val)
{
	for (const auto &ma: m_vpMA) {
		ma->SetNSamples(val);
	}
}

bool CRigidBody::GetPositionData(D3DXVECTOR3* pos, D3DXVECTOR3* angle, D3DXQUATERNION* quat, float* dist) {
	D3DXVECTOR3 val;
	if (WaitForSingleObject(m_positionUpdateSemaphore, RB_TIMEOUT_MS) != WAIT_OBJECT_0) return false;
	*pos = m_position;
	*quat = m_quaternion;
	*angle = m_angle;
	*dist = m_dist;
	ReleaseSemaphore(m_positionUpdateSemaphore, 1, NULL);
	return true;
}
#endif //#ifdef NPTRACKINGTOOLS
