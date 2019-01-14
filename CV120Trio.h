#pragma once
#ifdef NPTRACKINGTOOLS

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "NPTrackingTools.h"
#include "CRigidBody.h"
#include "CoordTracker.h"

class CV120Trio 
{
public:
	CV120Trio();
	~CV120Trio();

	bool Initialize();
	bool Shutdown();
	void Update();
	bool Update(CRigidBody* rb);
	int CameraCount();
	int RigidBodyCount();
	int MarkerCount();
	void Initialize(const char* sProjectFile);
	const float m_kRadToDeg = 0.0174532925f;

	

private:
	bool CheckResult(NPRESULT result);
	const char* m_sProjectFile;
	bool m_bInitialized;
	float m_to_mm = 1000;
};

#endif //#ifdef NPTRACKINGTOOLS