// Robot.cpp: implementation of the CRobot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "Robot.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CRobot::CRobot()
{
	

	m_strName = "Staubli RX160";

	m_fBaseRotX = 0.0f;
	m_fBaseRotY = 0.0f;
	m_fBaseRotZ = 0.0f;
	m_vBaseNorm = D3DXVECTOR3(0.0f,		0.0f,	1.0f); 
	m_vBase = D3DXVECTOR3(0.0f,		0.0f,	0.0f);
	m_vBaseJ1 = D3DXVECTOR3(0.0f,		0.0f,	300.0f);
	m_vJ1J2 = D3DXVECTOR3(150.0f,		0.0f,	250.0f);
	m_vJ2J3 = D3DXVECTOR3(0.0f,		0.0f,	825.0f);
	m_vJ3J4 = D3DXVECTOR3(0.0f,		0.0f,	0.0f);
	m_vJ4J5 = D3DXVECTOR3(0.0f,		0.0f,	625.0f);
	m_vJ5J6 = D3DXVECTOR3(0.0f,		0.0f,	110.0f);
	m_fJogDirSpeedX = 1.0f;
	m_fJogDirSpeedY = 1.0f;
	m_fJogDirSpeedZ = 1.0f;
	m_fJogDirSpeedR = 1.0f;
	m_fJogDirSpeedHTip = 1.0f;
	m_fJogDirSpeedVTip = 1.0f;
	m_fJogDirSpeedTTip = 1.0f;

	switch(theApp.m_Robot[0].m_nEndEffectorType = 0) {
	default:
		m_vJ6ToolTip = D3DXVECTOR3(0.0f,0.0f,	170.0f);
		m_vAtToolTip = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		break;
	case 1:
		m_vJ6ToolTip = D3DXVECTOR3(-127.0f,0.0f,	141.0f);
		m_vAtToolTip = D3DXVECTOR3(-0.707f, 0.0f, 0.707f);
		break;
	}

	m_qTool = D3DXQUATERNION(.0f,0.0f,1.0f,1.0f);

	ZeroMemory(m_fJointAngle, sizeof m_fJointAngle);
	ZeroMemory(m_fLastJointAngle, sizeof m_fLastJointAngle);

	m_nSide = PORTSIDE;

	InitializeMatrices();
}

CRobot::~CRobot()
{

}

int CRobot::Save(CFile *pFile)
{
	
	float fTemp;

	ULONGLONG nFileOffset = pFile->GetPosition();

	pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);

	CArchive ar(pFile,CArchive::store);
	ar.WriteString(m_strName+"\r\n");
	ar.WriteString(m_strSerialNumber+"\r\n");
	ar.WriteString(m_EndEffectorType+"\r\n");
	ar.WriteString(m_EndEffectorSerialNumber+"\r\n");
	ar.Close();

	pFile->Write(&m_nEndEffectorType,sizeof m_nEndEffectorType);

	pFile->Write(&fTemp,sizeof fTemp);
	pFile->Write(&m_vBaseNorm,sizeof m_vBaseNorm);
	pFile->Write(&m_vBase,sizeof m_vBase);
	pFile->Write(&m_vBaseJ1,sizeof m_vBaseJ1);
	pFile->Write(&m_vJ1J2,sizeof m_vJ1J2);
	pFile->Write(&m_vJ2J3,sizeof m_vJ2J3);
	pFile->Write(&m_vJ3J4,sizeof m_vJ3J4);
	pFile->Write(&m_vJ4J5,sizeof m_vJ4J5);
	pFile->Write(&m_vJ5J6,sizeof m_vJ5J6);
	pFile->Write(&m_vJ6ToolTip,sizeof m_vJ6ToolTip);
	pFile->Write(&m_vAtToolTip,sizeof m_vAtToolTip);
	pFile->Write(&m_nSide,sizeof m_nSide);
	pFile->Write(&m_fBaseRotX,sizeof m_fBaseRotX);
	pFile->Write(&m_fBaseRotY,sizeof m_fBaseRotY);
	pFile->Write(&m_fBaseRotZ,sizeof m_fBaseRotZ);
	pFile->Write(m_fStepSize,sizeof m_fStepSize);
	pFile->Write(&m_fJogDirSpeedX,sizeof m_fJogDirSpeedX);
	pFile->Write(&m_fJogDirSpeedY,sizeof m_fJogDirSpeedX);
	pFile->Write(&m_fJogDirSpeedZ,sizeof m_fJogDirSpeedX);
	pFile->Write(&m_fJogDirSpeedHTip,sizeof m_fJogDirSpeedX);
	pFile->Write(&m_fJogDirSpeedVTip,sizeof m_fJogDirSpeedX);
	pFile->Write(&m_fJogDirSpeedTTip,sizeof m_fJogDirSpeedTTip);
	pFile->Write(&m_fJogDirSpeedR, sizeof m_fJogDirSpeedR);


	return (int)nFileOffset;
}

bool CRobot::Retrieve(CFile *pFile,int nWhich)
{
	float fVersion;
	float fTemp;

	pFile->Read(&fVersion,sizeof fVersion);
	if(fVersion>=4.31f) {
		CArchive ar(pFile,CArchive::load);
		ar.ReadString(m_strName);
		ar.ReadString(m_strSerialNumber);
		ar.ReadString(m_EndEffectorType);
		ar.ReadString(m_EndEffectorSerialNumber);
		ar.Close();

		pFile->Read(&m_nEndEffectorType,sizeof m_nEndEffectorType);

		pFile->Read(&fTemp,sizeof fTemp);
		pFile->Read(&m_vBaseNorm,sizeof m_vBaseNorm);
		pFile->Read(&m_vBase,sizeof m_vBase);
		pFile->Read(&m_vBaseJ1,sizeof m_vBaseJ1);
		pFile->Read(&m_vJ1J2,sizeof m_vJ1J2);
		pFile->Read(&m_vJ2J3,sizeof m_vJ2J3);
		pFile->Read(&m_vJ3J4,sizeof m_vJ3J4);
		pFile->Read(&m_vJ4J5,sizeof m_vJ4J5);
		pFile->Read(&m_vJ5J6,sizeof m_vJ5J6);
		pFile->Read(&m_vJ6ToolTip,sizeof m_vJ6ToolTip);
		pFile->Read(&m_vAtToolTip,sizeof m_vAtToolTip);
		pFile->Read(&m_nSide,sizeof m_nSide);
		pFile->Read(&m_fBaseRotX,sizeof m_fBaseRotX);
		pFile->Read(&m_fBaseRotY,sizeof m_fBaseRotY);
		pFile->Read(&m_fBaseRotZ,sizeof m_fBaseRotZ);
		pFile->Read(m_fStepSize,sizeof m_fStepSize);
		pFile->Read(&m_fJogDirSpeedX,sizeof m_fJogDirSpeedX);
		pFile->Read(&m_fJogDirSpeedY,sizeof m_fJogDirSpeedX);
		pFile->Read(&m_fJogDirSpeedZ,sizeof m_fJogDirSpeedX);
		pFile->Read(&m_fJogDirSpeedHTip,sizeof m_fJogDirSpeedX);
		pFile->Read(&m_fJogDirSpeedVTip,sizeof m_fJogDirSpeedX);
		pFile->Read(&m_fJogDirSpeedTTip,sizeof m_fJogDirSpeedTTip);
		pFile->Read(&m_fJogDirSpeedR, sizeof m_fJogDirSpeedR);

		m_nSide = nWhich;
		InitializeMatrices();

		return true;
	}
	return false;


}

D3DXVECTOR3* CRobot::TransformVectorFromBase(D3DXVECTOR3 *pVec)
{

	*pVec = *pVec + m_vBase;
	return pVec;

}

D3DXVECTOR3* CRobot::TransformVectorFromJ1(D3DXVECTOR3 *pVec)
{
	D3DXVECTOR3 vec;
	D3DXMATRIXA16 matWorld;

	D3DXMatrixMultiply(&matWorld,&m_matJoint[0],&m_matLimb[0]);

	D3DXVec3TransformCoord(&vec,pVec,&matWorld);

	*pVec = vec;
	return pVec;

}

D3DXVECTOR3* CRobot::TransformVectorFromJ2(D3DXVECTOR3 *pVec)
{
	D3DXVECTOR3 vec;
	D3DXMATRIXA16 matWorld;

	D3DXMatrixMultiply(&matWorld,&m_matJoint[1],&m_matLimb[1]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[0]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[0]);

	D3DXVec3TransformCoord(&vec,pVec,&matWorld);

	*pVec = vec;
	return pVec;

}

D3DXVECTOR3* CRobot::TransformVectorFromJ3(D3DXVECTOR3 *pVec)
{
	D3DXVECTOR3 vec;
	D3DXMATRIXA16 matWorld;

	D3DXMatrixMultiply(&matWorld,&m_matJoint[2],&m_matLimb[2]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[1]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[1]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[0]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[0]);

	D3DXVec3TransformCoord(&vec,pVec,&matWorld);

	*pVec = vec;
	return pVec;

}

D3DXVECTOR3* CRobot::TransformVectorFromJ4(D3DXVECTOR3 *pVec)
{
	D3DXVECTOR3 vec;
	D3DXMATRIXA16 matWorld;

	D3DXMatrixMultiply(&matWorld,&m_matJoint[3],&m_matLimb[3]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[2]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[2]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[1]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[1]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[0]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[0]);

	D3DXVec3TransformCoord(&vec,pVec,&matWorld);

	*pVec = vec;
	return pVec;

}

D3DXVECTOR3* CRobot::TransformVectorFromJ5(D3DXVECTOR3 *pVec)
{
	D3DXVECTOR3 vec;
	D3DXMATRIXA16 matWorld;

	D3DXMatrixMultiply(&matWorld,&m_matJoint[4],&m_matLimb[4]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[3]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[3]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[2]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[2]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[1]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[1]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[0]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[0]);

	D3DXVec3TransformCoord(&vec,pVec,&matWorld);

	*pVec = vec;
	return pVec;

}

D3DXVECTOR3* CRobot::TransformVectorFromJ6(D3DXVECTOR3 *pVec)
{
	D3DXVECTOR3 vec;
	D3DXMATRIXA16 matWorld;

	D3DXMatrixMultiply(&matWorld,&m_matJoint[5],&m_matLimb[5]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[4]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[4]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[3]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[3]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[2]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[2]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[1]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[1]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[0]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[0]);

	D3DXVec3TransformCoord(&vec,pVec,&matWorld);

	*pVec = vec;
	return pVec;

}

D3DXVECTOR3* CRobot::TransformVectorFromToolTip(D3DXVECTOR3 *pVec)
{
	D3DXVECTOR3 vec;
	D3DXMATRIXA16 matWorld;

	D3DXMatrixMultiply(&matWorld,&m_matJoint[6],&m_matLimb[6]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[5]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[5]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[4]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[4]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[3]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[3]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[2]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[2]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[1]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[1]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[0]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matLimb[0]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matBase);

	D3DXVec3TransformCoord(&vec,pVec,&matWorld);

	*pVec = vec;
	return pVec;

}

void CRobot::InitializeMatrices()
{

	InitializeJointMatrices();
	InitializeLimbMatrices();

}

void CRobot::InitializeJointMatrices()
{
	float fAngle;


	D3DXMatrixRotationX(&m_matBase, m_fBaseRotX * DEG_TO_RAD);
	D3DXMatrixRotationZ(&m_matJoint[0],m_fJointAngle[0] + m_fBaseRotZ * DEG_TO_RAD);
	D3DXMatrixRotationY(&m_matJoint[1],m_fJointAngle[1]);
	D3DXMatrixRotationY(&m_matJoint[2],m_fJointAngle[2]);
	D3DXMatrixRotationZ(&m_matJoint[3],m_fJointAngle[3]);
	D3DXMatrixRotationY(&m_matJoint[4],m_fJointAngle[4]);
	D3DXMatrixRotationZ(&m_matJoint[5],m_fJointAngle[5]);

	//needs expansion for differnt end effectors
	fAngle = atan2f(m_vAtToolTip.x,m_vAtToolTip.z);
//	fAngle = atan2f(sqrtf(m_vAtToolTip.y * m_vAtToolTip.y + m_vAtToolTip.x * m_vAtToolTip.x),m_vAtToolTip.z);
	D3DXMatrixRotationY(&m_matJoint[6],fAngle);

}

void CRobot::InitializeLimbMatrices()
{
	

	float fWp = theApp.m_Kinematics.GetDesiredWaterPath(m_nSide);

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		D3DXMatrixTranslation(&m_matLimb[0],m_vBaseJ1.x+m_vBase.x,m_vBaseJ1.y+m_vBase.y,m_vBaseJ1.z+m_vBase.z);
		break;
	case DUAL_ROBOT_9_PLUS_9:
		D3DXMatrixTranslation(&m_matLimb[0],m_vBaseJ1.x-m_vBase.x,m_vBaseJ1.y-m_vBase.y,m_vBaseJ1.z-m_vBase.z);
		break;
	};

	D3DXMatrixTranslation(&m_matLimb[1],m_vJ1J2.x,m_vJ1J2.y,m_vJ1J2.z);
	D3DXMatrixTranslation(&m_matLimb[2],m_vJ2J3.x,m_vJ2J3.y,m_vJ2J3.z);
	D3DXMatrixTranslation(&m_matLimb[3],m_vJ3J4.x,m_vJ3J4.y,m_vJ3J4.z);
	D3DXMatrixTranslation(&m_matLimb[4],m_vJ4J5.x,m_vJ4J5.y,m_vJ4J5.z);
	D3DXMatrixTranslation(&m_matLimb[5],m_vJ5J6.x,m_vJ5J6.y,m_vJ5J6.z);
	D3DXMatrixTranslation(&m_matLimb[6],m_vJ6ToolTip.x + fWp * m_vAtToolTip.x,m_vJ6ToolTip.y + fWp * m_vAtToolTip.y,m_vJ6ToolTip.z + fWp * m_vAtToolTip.z + (float)theApp.m_LastSettings.nExtraArmLength);

}

void CRobot::GetRobotJ6(D3DXVECTOR3 *pPos, D3DXQUATERNION *pQuat)
{
	D3DXMATRIXA16 matWorld;
	D3DXVECTOR3 vFace = D3DXVECTOR3(0.0f, 0.0f,0.0f);

	TransformVectorFromJ6(&vFace);

	D3DXMatrixMultiply(&matWorld,&m_matJoint[5],&m_matJoint[4]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[3]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[2]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[1]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[0]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matBase);

	D3DXQuaternionRotationMatrix(&m_qTool,&matWorld);
	*pPos = vFace;
	*pQuat = m_qTool;

}


void CRobot::GetToolTipPos(D3DXVECTOR3 *pPos, D3DXQUATERNION *pQuat)
{
	D3DXMATRIXA16 matWorld;
	D3DXVECTOR3 vToolTip = D3DXVECTOR3(0.0f, 0.0f,0.0f);
	D3DXQUATERNION qToolTip;

	TransformVectorFromToolTip(&vToolTip);

	D3DXMatrixMultiply(&matWorld,&m_matJoint[6],&m_matJoint[5]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[4]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[3]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[2]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[1]);
	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matJoint[0]);
//	D3DXMatrixMultiply(&matWorld,&matWorld,&m_matBase);

	D3DXQuaternionRotationMatrix(&qToolTip,&matWorld);
	D3DXQuaternionNormalize(&qToolTip,&qToolTip);
	if(qToolTip.w < 0.0f) {
		qToolTip.x *= -1.0f;
		qToolTip.y *= -1.0f;
		qToolTip.z *= -1.0f;
		qToolTip.w *= -1.0f;
	}

	*pPos = vToolTip;
	*pQuat = qToolTip;
}

void CRobot::GetInverseWrist(D3DXVECTOR3 *pPos, D3DXQUATERNION *pQuat)
{

	D3DXQuaternionIdentity(pQuat);
	*pPos = m_vInvWristPos;
}

void CRobot::GetInverseJ6(D3DXVECTOR3 *pPos, D3DXQUATERNION *pQuat)
{

	D3DXQuaternionIdentity(pQuat);
	*pPos = m_vInvJ6Pos;
}

bool CRobot::InverseKinematic(D3DXVECTOR3 *pPos, D3DXQUATERNION *pQuat,D3DXMATRIXA16* pmatWrist)
{
	

	D3DXQUATERNION Quat = *pQuat;
	D3DXVECTOR3 Pos = *pPos;
	D3DXMATRIXA16 matInv;
	D3DXVECTOR3 vInv;

	D3DXMATRIXA16 QuatRot,matWorld;
	D3DXVECTOR3 vAxis;

	D3DXMATRIXA16 matBase;

	D3DXQuaternionNormalize(&Quat,&Quat);
	D3DXMatrixRotationQuaternion(&QuatRot,&Quat);

	//Take out the rotation and translation of the base
	D3DXMatrixRotationZ(&matBase,-(m_fBaseRotZ * DEG_TO_RAD));
	D3DXMatrixMultiply(&QuatRot,&QuatRot,&matBase);

	Pos -= m_vBase;
	D3DXVec3TransformCoord(&Pos,&Pos,&matBase);

	//Joint 6 pos
	D3DXMatrixTranspose(&matInv,&m_matJoint[6]);
	D3DXMatrixMultiply(&QuatRot,&matInv,&QuatRot);
	float fWp = theApp.m_Kinematics.GetDesiredWaterPath(m_nSide);
	vInv.x = (m_vJ6ToolTip.x + fWp * m_vAtToolTip.x) * -1.0f;
	vInv.y = (m_vJ6ToolTip.y + fWp * m_vAtToolTip.y) * -1.0f;
	vInv.z = (m_vJ6ToolTip.z + fWp * m_vAtToolTip.z + (float)theApp.m_LastSettings.nExtraArmLength) * -1.0f;
	D3DXVec3TransformCoord(&vInv,&vInv,&QuatRot);
	m_vInvJ6Pos = Pos + vInv;


	//Wrist pos
	vInv.x = m_vJ5J6.x * -1.0f;
	vInv.y = m_vJ5J6.y * -1.0f;
	vInv.z = m_vJ5J6.z * -1.0f;
	D3DXVec3TransformCoord(&vInv,&vInv,&QuatRot);
	m_vInvWristPos = m_vInvJ6Pos + vInv;



	float fLength2WristXY = sqrt(powf(m_vInvWristPos.x,2.0f)+powf(m_vInvWristPos.y,2.0f)) - m_vJ1J2.x;
	float fLength2WristZ  = m_vInvWristPos.z - m_vJ1J2.z - m_vBaseJ1.z;
	float fLength2Wrist = sqrtf(powf(fLength2WristXY,2.0f) + powf(fLength2WristZ,2.0f));

	// Constraints
	float fInvTemp1 = sqrtf(powf(m_vInvWristPos.x,2.0f)+powf(m_vInvWristPos.y,2.0f));
	float fInvTemp2 = sqrtf(powf(fLength2WristXY,2.0f)+powf(fLength2WristZ,2.0f));

	float fLengthJ2J3 = D3DXVec3Length(&m_vJ2J3);
	float fLengthJ3J4 = D3DXVec3Length(&m_vJ3J4);
	float fLengthJ4J5 = D3DXVec3Length(&m_vJ4J5);
	float fLengthJ3J5 = fLengthJ3J4 + fLengthJ4J5;
	float fLengthJ2J5 = fLengthJ2J3 + fLengthJ3J4 + fLengthJ4J5;

	if (fInvTemp1>m_vJ1J2.x && fInvTemp2<fLengthJ2J5) {

		// Compute Motor 1
		if(m_vInvWristPos.y != 0.0f && m_vInvWristPos.x != 0.0f) {
			m_fInverseJointAngle[0]=atan2f(m_vInvWristPos.y,m_vInvWristPos.x);
		} else {
			m_fInverseJointAngle[0] = 0.0f;
		}

		// Compute Motor 3
		float fInvTemp3=powf(fLength2Wrist,2.0f)-powf(fLengthJ2J3,2.0f)-pow(fLengthJ3J5,2.0f);
		fInvTemp3=fInvTemp3/(-2.0f*fLengthJ2J3*fLengthJ3J5);
		m_fInverseJointAngle[2]=PIf - acosf(fInvTemp3);

		// Compute Motor 2
		m_fAlpha=asinf(fLength2WristZ/fLength2Wrist);
		float fInvTemp4=powf(fLengthJ2J3,2.0f) + powf(fLength2Wrist,2.0f) - pow(fLengthJ3J5,2.0f);
		float fInvTemp5=fInvTemp4/(2.0f*fLengthJ2J3*fLength2Wrist);

		if(fabs(m_fInverseJointAngle[0]) <= (PIf / 2.0f)) { //This needs completion for |J1| > 90.0
			if(m_vInvWristPos.x >= 0.0f) {		
				m_fInverseJointAngle[1]= (PIf/2.0f) - (m_fAlpha+acosf(fInvTemp5));
			} else {
				m_fInverseJointAngle[1]= (PIf/-2.0f) + (m_fAlpha-acosf(fInvTemp5));		//This needs checking if wrist is not on center line
			}
		} else {
			if(m_vInvWristPos.x >= 0.0f) {
				m_fInverseJointAngle[1]= (PIf/-2.0f) + (m_fAlpha-acosf(fInvTemp5));		//This needs checking if wrist is not on center line
			} else {
				m_fInverseJointAngle[1]= (PIf/2.0f) - (m_fAlpha+acosf(fInvTemp5));
			}
		}

		// Compute Motors #4,5,6
		D3DXMatrixRotationZ(&m_matInverseJoint[0],m_fInverseJointAngle[0]);
		D3DXMatrixRotationY(&m_matInverseJoint[1],m_fInverseJointAngle[1]);
		D3DXMatrixRotationY(&m_matInverseJoint[2],m_fInverseJointAngle[2]);

		D3DXMatrixMultiply(&matWorld,&m_matInverseJoint[2],&m_matInverseJoint[1]);
		D3DXMatrixMultiply(&matWorld,&matWorld,&m_matInverseJoint[0]);
		D3DXMatrixTranspose(&matWorld,&matWorld);
		D3DXMatrixMultiply(&matWorld,&QuatRot,&matWorld);

		float fKinPosM5DPlus;
		float fKinPosM5DMinus;
		float fKinPosM4DPlus;
		float fKinPosM4DMinus;

		fKinPosM5DPlus = acosf(matWorld._33);
		fKinPosM5DMinus=-fKinPosM5DPlus;
		fKinPosM4DPlus=atan2f(matWorld._32,matWorld._31);
		fKinPosM4DMinus=atan2f(-matWorld._32,-matWorld._31);

		float fSolution1M4D=fabs(fKinPosM4DPlus-m_fLastJointAngle[3]);
		float fSolution2M4D=fabs(fKinPosM4DMinus-m_fLastJointAngle[3]);
		if (fSolution1M4D <= fSolution2M4D)						// Shorter travel distance
		{
			m_fInverseJointAngle[3]=fKinPosM4DPlus;
			m_fInverseJointAngle[4]=fKinPosM5DPlus;
			m_fInverseJointAngle[5]=-atan2f(-matWorld._23,-matWorld._13);
		}
		else
		{
			m_fInverseJointAngle[3]=fKinPosM4DMinus;
			m_fInverseJointAngle[4]=fKinPosM5DMinus;
			m_fInverseJointAngle[5]=-atan2f(matWorld._23,matWorld._13);
		}

		for(int jj=0;jj<6;jj++) {
			m_fLastJointAngle[jj]=m_fInverseJointAngle[jj];
		};



		*pPos = m_vInvWristPos;
		if(pmatWrist != NULL) *pmatWrist = matWorld;
		return true;
	} else {
		return false;
	}
}



void CRobot::GetJointPos(CCoord *pCoord, int nSide)
{


	for(int ii=0;ii<6;ii++) {
		pCoord->Side[nSide].fPos[ii] = m_fJointAngle[ii];
	}

}

void CRobot::GetToolPos(CCoord *pCoord, int nSide)
{
	D3DXVECTOR3 vToolTipPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXQUATERNION qToolTip;
	D3DXMATRIXA16 QuatRot;

	InitializeMatrices();
	GetToolTipPos(&vToolTipPos, &qToolTip);

	D3DXMatrixRotationQuaternion(&QuatRot,&qToolTip);

	//Quaternion to Normal
	pCoord->Side[nSide].pt = vToolTipPos;
	pCoord->Side[nSide].fI = QuatRot._31;
	pCoord->Side[nSide].fJ = QuatRot._32;
	pCoord->Side[nSide].fK = QuatRot._33;
	pCoord->Side[nSide].fW = -atan2f(-QuatRot._23, -QuatRot._13) * RAD_TO_DEG;
}


void CRobot::GetScanSize(float *fReach, CString *pBuff)
{
	CString strY, strZ;
	CCoord Cp;
	float fHi = 10000.0f;
	float fLo = 0.0f;
	float fY[2],fZ[2];
	float fGuess;

	D3DXQUATERNION quatNormal;
	D3DXVECTOR3 vPos = D3DXVECTOR3(*fReach, 0.0f, m_vBase.z + m_vBaseJ1.z);

	Cp.Side0.norm = D3DXVECTOR4(1.0f, 0.0f, 0.0f,0.0f);
	Cp.QuaternionFromNormal(0,&quatNormal);

	fHi = 10000.0f;
	fLo = 0.0f;
	while (fabsf(fHi-fLo) > 0.5f) {
		vPos.x = *fReach;
		fGuess = vPos.y =(fHi + fLo) / 2.0f;
		vPos.z = m_vBase.z + m_vBaseJ1.z;
		if(InverseKinematic(&vPos,&quatNormal,NULL) == true) {
			fLo = fGuess;
		} else {
			fHi = fGuess;
		}
	}
	fY[1] = fGuess;
	fHi = 0.0f;
	fLo = -10000.0f;
	while (fabsf(fHi-fLo) > 0.5f) {
		vPos.x = *fReach;
		fGuess = vPos.y =(fHi + fLo) / 2.0f;
		vPos.z = m_vBase.z + m_vBaseJ1.z;
		if(InverseKinematic(&vPos,&quatNormal,NULL) == false) {
			fLo = fGuess;
		} else {
			fHi = fGuess;
		}
	}
	fY[0] = fGuess;
	strY.Format(L"Y %.0f <=> %.0f",fY[1],fY[0]);

	///////////////////////// Z //////////////////////
	fHi = 10000.0f;
	fLo = 0.0f;
	while (fabsf(fHi-fLo) > 0.5f) {
		vPos.x = *fReach;
		vPos.y = 0.0f;
		fGuess = vPos.z =(fHi + fLo) / 2.0f;
		if(InverseKinematic(&vPos,&quatNormal,NULL) == true) {
			fLo = fGuess;
		} else {
			fHi = fGuess;
		}
	}
	fZ[1] = fGuess;
	fHi = 0.0f;
	fLo = -10000.0f;
	while (fabsf(fHi-fLo) > 0.5f) {
		vPos.x = *fReach;
		vPos.y = 0.0f;
		fGuess = vPos.z =(fHi + fLo) / 2.0f;
		if(InverseKinematic(&vPos,&quatNormal,NULL) == false) {
			fLo = fGuess;
		} else {
			fHi = fGuess;
		}
	}
	fZ[0] = fGuess;
	strZ.Format(L"Z %.0f <=> %.0f",fZ[1],fZ[0]);

	pBuff->Format(L"%s\r\n%s",strY,strZ);
}


void CRobot::SetBaseOffsets(int nSide)
{
	
	CCoord CpSurface;
	theApp.m_Motors.GetSurfacePos(&CpSurface);

	if(nSide == 0) {
		theApp.m_Robot[nSide].m_vBase.x -= (CpSurface.Side[1].fX - CpSurface.Side[0].fX);
		theApp.m_Robot[nSide].m_vBase.y -= (CpSurface.Side[1].fY - CpSurface.Side[0].fY);
		theApp.m_Robot[nSide].m_vBase.z -= (CpSurface.Side[1].fZ - CpSurface.Side[0].fZ);
	} else {
		theApp.m_Robot[nSide].m_vBase.x -= (CpSurface.Side[1].fX - CpSurface.Side[0].fX);
		theApp.m_Robot[nSide].m_vBase.y -= (CpSurface.Side[1].fY - CpSurface.Side[0].fY);
		theApp.m_Robot[nSide].m_vBase.z -= (CpSurface.Side[1].fZ - CpSurface.Side[0].fZ);
	};
}