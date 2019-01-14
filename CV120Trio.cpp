
#include "stdafx.h"
#include "USLScanner.h"
#include "CV120Trio.h"

#ifdef NPTRACKINGTOOLS
CV120Trio::CV120Trio() 
{
	//Default project file.
	//"Optitrack/project.ttp"
	m_sProjectFile = "Optitrack/4RigidBodies.ttp";
	m_bInitialized = false;
}


CV120Trio::~CV120Trio()
{
	Shutdown();
}

bool CV120Trio::Initialize()
{
	TT_Initialize();
	TT_Update();
	TT_ClearRigidBodyList();
	int result=-1;
	result = TT_LoadProject(m_sProjectFile);
	m_bInitialized = CheckResult(result);
	return m_bInitialized;
}

bool CV120Trio::Shutdown()
{
	if (!m_bInitialized) return true;
	int result = TT_Shutdown();
	if (CheckResult(result)) {
		m_bInitialized = false;
		return true; //Returns true if shutdown was successful.
	}
	return false;
}

void CV120Trio::Update() {
	if (!m_bInitialized) return; 		
	TT_Update();
}

bool CV120Trio::Update(CRigidBody * rb)
{
	if (!m_bInitialized) return false;
	Update();
	float   yaw, pitch, roll;
	float   x, y, z;
	float   qx, qy, qz, qw;
	int ident = rb->GetIdent();
	TT_RigidBodyLocation(ident, &x, &y, &z, &qx, &qy, &qz, &qw, &yaw, &pitch, &roll);
	unsigned long  smResult = 0;
	if (TT_IsRigidBodyTracked(ident)) {
		rb->Set(x*m_to_mm, y*m_to_mm, z*m_to_mm, qx, qy, qz, qw, yaw, pitch, roll);
		rb->SetTracked(true);
		return true;
	}
	rb->SetTracked(false);
	return false;
}

int CV120Trio::CameraCount() {
	if (!m_bInitialized) return -1;
	return TT_CameraCount();
}

int CV120Trio::RigidBodyCount() {
	if (!m_bInitialized) return -1;
	return TT_RigidBodyCount();
}

int CV120Trio::MarkerCount() {
	if (!m_bInitialized) return -1;
	return TT_FrameMarkerCount();
}

void CV120Trio::Initialize(const char * sProjectFile)
{
	m_sProjectFile = sProjectFile;
	Initialize();
}

bool CV120Trio::CheckResult(NPRESULT result)
{
	if (result != NPRESULT_SUCCESS)
	{
		// Treat all errors as failure conditions.
	//	printf("Error: %s\n\n(Press any key to continue)\n", TT_GetResultString(result));

	//	Sleep(20);
	//	exit(1);
		return false;
	}
	return true;
}
#endif
