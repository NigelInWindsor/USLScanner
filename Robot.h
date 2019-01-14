// Robot.h: interface for the CRobot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROBOT_H__18DD89FF_7EAC_46AD_8AB3_387C2E1481DD__INCLUDED_)
#define AFX_ROBOT_H__18DD89FF_7EAC_46AD_8AB3_387C2E1481DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRobot  
{
public:
	void SetBaseOffsets(int nSide);
	void GetScanSize(float *fReach,CString *pBuff);
	void GetInverseWrist(D3DXVECTOR3 *pPos, D3DXQUATERNION *pQuat);
	void GetInverseJ6(D3DXVECTOR3 *pPos, D3DXQUATERNION *pQuat);
	void GetToolPos(CCoord* pCoord,int nSide);
	void GetJointPos(CCoord* pCoord,int nSide);
	bool Retrieve(CFile* pFile,int nWhich);
	int Save(CFile* pFile);
	//inverse Kinematics
	bool InverseKinematic(D3DXVECTOR3 *pPos, D3DXQUATERNION *pQuat, D3DXMATRIXA16 *pmatWrist=NULL);

	D3DXVECTOR3 m_vInvJ6Pos;
	D3DXVECTOR3 m_vInvWristPos;
	D3DXMATRIXA16	m_matInverseJoint[6];
	float	m_fInverseJointAngle[6];
	float	m_fLastJointAngle[6];


	//Forward Kinematics
	void GetToolTipPos(D3DXVECTOR3 *pPos,D3DXQUATERNION *pQuat);
	void GetRobotJ6(D3DXVECTOR3 *pPos,D3DXQUATERNION *pQuat);
	void GetRobotWrist(D3DXVECTOR3 *pPos,D3DXQUATERNION *pQuat);

	void InitializeMatrices();
	void InitializeJointMatrices();
	void InitializeLimbMatrices();

	D3DXVECTOR3* TransformVectorFromBase(D3DXVECTOR3 *pVec);
	D3DXVECTOR3* TransformVectorFromJ1(D3DXVECTOR3 *pVec);
	D3DXVECTOR3* TransformVectorFromJ2(D3DXVECTOR3 *pVec);
	D3DXVECTOR3* TransformVectorFromJ3(D3DXVECTOR3 *pVec);
	D3DXVECTOR3* TransformVectorFromJ4(D3DXVECTOR3 *pVec);
	D3DXVECTOR3* TransformVectorFromJ5(D3DXVECTOR3 *pVec);
	D3DXVECTOR3* TransformVectorFromJ6(D3DXVECTOR3 *pVec);
	D3DXVECTOR3* TransformVectorFromToolTip(D3DXVECTOR3 *pVec);

	D3DXQUATERNION	m_qTool;
	D3DXMATRIXA16	m_matLimb[7];
	D3DXMATRIXA16	m_matJoint[7];
	D3DXMATRIXA16	m_matBase;

	//Forward and Inverse Kinematics
	float		m_fBaseRotX;
	float		m_fBaseRotY;
	float		m_fBaseRotZ;
	D3DXVECTOR3 m_vBaseNorm;
	D3DXVECTOR3 m_vBase;
	D3DXVECTOR3 m_vBaseJ1;
	D3DXVECTOR3 m_vJ1J2;
	D3DXVECTOR3 m_vJ2J3;
	D3DXVECTOR3 m_vJ3J4;
	D3DXVECTOR3 m_vJ4J5;
	D3DXVECTOR3 m_vJ5J6;
	D3DXVECTOR3 m_vJ6ToolTip;
	D3DXVECTOR3 m_vAtToolTip;
	float		m_fStepSize[6];
	float		m_fJointAngle[6];
	float		m_fPhi;					// Angle between L35 And (L34x+L45x) 
	float		m_fAlpha;				// Angle between L2WristXY And L2Wrist 

	float		m_fJogDirSpeedX;
	float		m_fJogDirSpeedY;
	float		m_fJogDirSpeedZ;
	float		m_fJogDirSpeedR;
	float		m_fJogDirSpeedHTip;
	float		m_fJogDirSpeedVTip;
	float		m_fJogDirSpeedTTip;

	int			m_nSide;				//Needs to know which Wp to use
	int			m_nJointPos[6];

	CString		m_strName;
	CString		m_strSerialNumber;

	int			m_nEndEffectorType;
	CString		m_EndEffectorType;
	CString		m_EndEffectorSerialNumber;

	CRobot();
	virtual ~CRobot();

};

#endif // !defined(AFX_ROBOT_H__18DD89FF_7EAC_46AD_8AB3_387C2E1481DD__INCLUDED_)
