// Kinematics.cpp: implementation of the CKinematics class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "Kinematics.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	_FLL	0
#define	_FLR	1
#define	_FUL	2
#define	_FUR	3
#define	_BLL	4
#define	_BLR	5
#define	_BUL	6
#define	_BUR	7

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WCHAR KinematicIdentifier[]={L"Ultrasonic Sciences Ltd USLScanner Kinematics"};


CKinematics::CKinematics()
{
	m_dXtip[0]=500.0;
	m_dXtip[1]=500.0;
	m_dYtip[0]=500.0;
	m_dYtip[1]=500.0;

	m_fComponentThickness = 0.0;

	memset(&m_Rhomboid,0,sizeof m_Rhomboid);
	m_pFrontLowerLeft = (RhomboidStruct*)m_Rhomboid;

	memset(&m_Tip,0,sizeof m_Tip);
	m_dXTipMinMax = 60.0;
	m_dYTipMinMax = 60.0;
	m_dXTipStep = 20.0;
	m_dYTipStep = 20.0;

	m_bRhomboidCompensation = TRUE;
	m_bTipCompensation = FALSE;
	m_bDesiredWaterPathChanged = false;
}

CKinematics::~CKinematics()
{

}


void CKinematics::SaveManipulator(CString FilePath)
{
	CUSLFile file;
	CFileException e;
	CString Buff;

	if(!file.Open(FilePath,CFile::modeWrite|CFile::typeBinary|CFile::modeCreate,&e)) {
		file.DecodeFileException(&e,&Buff,FilePath);
		MessageBox(NULL,Buff,_T("Manipulator Error"),MB_ICONERROR);
	} else {
		SaveManipulator(&file);
		file.Close();
	};
}


void CKinematics::SaveManipulator(CUSLFile *pFile)
{
	char Array[1000];
	ZeroMemory(Array, sizeof Array);
	int nNotUsed[8];

	ULONGLONG nStartOffset = pFile->GetPosition();
	pFile->Write(Array, sizeof Array);
	pFile->Seek(nStartOffset,CFile::begin);

	pFile->Write(KinematicIdentifier,sizeof KinematicIdentifier);
	pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);

	ManipulatorFileName = pFile->GetFileName();
	ManipulatorFilePath = pFile->GetFilePath();

	pFile->Write(m_fXOffset,sizeof m_fXOffset);
	pFile->Write(m_fYOffset,sizeof m_fYOffset);
	pFile->Write(m_fZOffset,sizeof m_fZOffset);
	pFile->Write(m_fProbeLength,sizeof m_fProbeLength);
	pFile->Write(m_fArmLength,sizeof m_fArmLength);
	pFile->Write(m_fDesiredWaterPath,sizeof m_fDesiredWaterPath);
	pFile->Write(nNotUsed,sizeof nNotUsed);
	pFile->Write(m_fProbeMirror,sizeof m_fProbeMirror);

	for(int nAxis=0;nAxis<16;nAxis++) {
		m_LeadScrew[nAxis].Save(pFile);
	}

	pFile->Write(nNotUsed,sizeof nNotUsed);
	pFile->Write(&m_nXtipRotateAxis,sizeof m_nXtipRotateAxis);
	pFile->Write(&m_nYtipRotateAxis,sizeof m_nYtipRotateAxis);
	pFile->Write(&m_nInvertXtip,sizeof m_nInvertXtip);
	pFile->Write(&m_nInvertYtip,sizeof m_nInvertYtip);
	for(int ii=0;ii<256;ii++) {
		pFile->WriteStruct(&m_Probe[ii],m_Probe[ii].nStructSize = sizeof ProbeStruct);
	};
	pFile->Write(&m_fComponentThickness,sizeof m_fComponentThickness);
	pFile->Write(&nLimitSwitchOffsetLoadMask,sizeof nLimitSwitchOffsetLoadMask);
	pFile->Write(&nKinematicOffsetLoadMask,	sizeof nKinematicOffsetLoadMask);

	pFile->Write(m_vLimb0,	sizeof m_vLimb0);
	pFile->Write(m_vLimb1,	sizeof m_vLimb1);
	pFile->Write(&m_nAxesIncludedJoystick, sizeof m_nAxesIncludedJoystick);
	pFile->Write(&m_nAxesIncludedMoves, sizeof m_nAxesIncludedMoves);
	pFile->Write(m_fRobotWeightingX, sizeof m_fRobotWeightingX);
	pFile->Write(m_fRobotWeightingY, sizeof m_fRobotWeightingY);
	pFile->Write(m_fRobotWeightingZ, sizeof m_fRobotWeightingZ);

}

bool CKinematics::RetrieveManipulator(CString FilePath)
{
	CUSLFile file;
	CFileException e;
	CString Buff;
	bool bFlag = false;

	if(!file.Open(FilePath,CFile::typeBinary|CFile::modeRead,&e)) {
		file.DecodeFileException(&e,&Buff,FilePath);
		MessageBox(NULL,Buff,_T("Manipulator Error"),MB_ICONERROR);
	} else {
		bFlag = RetrieveManipulator(&file);
		file.Close();
	};

	return bFlag;
}


bool CKinematics::RetrieveManipulator(CUSLFile *pFile)
{
	int ii;
	WCHAR Identifier[0x100];
	float fVersion;
	int nNotUsed[8];

	ULONGLONG nStartOffset = pFile->GetPosition();

	pFile->Read(Identifier,sizeof KinematicIdentifier);
	pFile->Read(&fVersion,sizeof fVersion);

	if(fVersion >= theApp.m_fVersion && wcscmp(Identifier,KinematicIdentifier)==0 ) {

		ManipulatorFileName = pFile->GetFileName();
		ManipulatorFilePath = pFile->GetFilePath();

		pFile->Read(m_fXOffset,sizeof m_fXOffset);
		pFile->Read(m_fYOffset,sizeof m_fYOffset);
		pFile->Read(m_fZOffset,sizeof m_fZOffset);
		pFile->Read(m_fProbeLength,sizeof m_fProbeLength);
		pFile->Read(m_fArmLength,sizeof m_fArmLength);
		pFile->Read(m_fDesiredWaterPath,sizeof m_fDesiredWaterPath);
		pFile->Read(nNotUsed,sizeof nNotUsed);
		pFile->Read(m_fProbeMirror,sizeof m_fProbeMirror);

		for(int nAxis=0;nAxis<16;nAxis++) {
			m_LeadScrew[nAxis].Retrieve(pFile);
		}
		PreProcessAxesCompensation();

		pFile->Read(nNotUsed,sizeof nNotUsed);
		pFile->Read(&m_nXtipRotateAxis,sizeof m_nXtipRotateAxis);
		pFile->Read(&m_nYtipRotateAxis,sizeof m_nYtipRotateAxis);
		pFile->Read(&m_nInvertXtip,sizeof m_nInvertXtip);
		pFile->Read(&m_nInvertYtip,sizeof m_nInvertYtip);
		for(ii=0;ii<256;ii++) {
			pFile->ReadStruct(&m_Probe[ii], sizeof ProbeStruct);
		};
		pFile->Read(&m_fComponentThickness,sizeof m_fComponentThickness);
		if(fVersion >= 4.35f) {
			pFile->Read(&nLimitSwitchOffsetLoadMask,	sizeof nLimitSwitchOffsetLoadMask);
			pFile->Read(&nKinematicOffsetLoadMask,	sizeof nKinematicOffsetLoadMask);
		}
		if(fVersion >= 4.46f) {
			pFile->Read(m_vLimb0,				sizeof m_vLimb0);
			pFile->Read(m_vLimb1,				sizeof m_vLimb1);
		};
		if(fVersion >= 4.52f) {
			pFile->Read(&m_nAxesIncludedJoystick, sizeof m_nAxesIncludedJoystick);
			pFile->Read(&m_nAxesIncludedMoves, sizeof m_nAxesIncludedMoves);
		};
		if(fVersion >= 4.62f) {
			pFile->Read(m_fRobotWeightingX, sizeof m_fRobotWeightingX);
			pFile->Read(m_fRobotWeightingY, sizeof m_fRobotWeightingY);
			pFile->Read(m_fRobotWeightingZ, sizeof m_fRobotWeightingZ);
		}

		return true;

	}
	return false;
}

void CKinematics::Save(CString Name)
{
	CUSLFile File;
	CFileException ExceptionInstance;
	CString FilePath = theApp.m_LastSettingsPath + L"\\" + Name + L".kinematics";
	
	if (File.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &ExceptionInstance)==0) { 
	
		CString Buff;
		Buff = L"Cannot Save the Last Settings!: " + FilePath;
		MessageBox(NULL,Buff,_T("Warning"), MB_ICONERROR);
	} else {
		Save(&File);

		File.Close();
	}
}

void CKinematics::Save(CUSLFile* pFile)
{
	int nX,nY,nZ;
	char Array[1000];
	ZeroMemory(Array, sizeof Array);

	ULONGLONG nStartOffset = pFile->GetPosition();
	pFile->Write(Array, sizeof Array);
	pFile->Seek(nStartOffset,CFile::begin);

	pFile->Write(KinematicIdentifier,sizeof KinematicIdentifier);
	pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);

	pFile->Write(m_fXOffset,sizeof m_fXOffset);
	pFile->Write(m_fYOffset,sizeof m_fXOffset);
	pFile->Write(m_fZOffset,sizeof m_fXOffset);
	pFile->Write(m_fProbeLength,sizeof m_fProbeLength);
	pFile->Write(m_fArmLength,sizeof m_fArmLength);
	pFile->Write(m_fDesiredWaterPath,sizeof m_fDesiredWaterPath);
	pFile->Write(&m_dXStepSize,sizeof m_dXStepSize);
	pFile->Write(&m_dYStepSize,sizeof m_dYStepSize);
	pFile->Write(&m_dZStepSize,sizeof m_dZStepSize);
	pFile->Write(&m_dXMin,sizeof m_dXMin);
	pFile->Write(&m_dYMin,sizeof m_dYMin);
	pFile->Write(&m_dZMin,sizeof m_dZMin);
	pFile->Write(&m_dXMax,sizeof m_dXMax);
	pFile->Write(&m_dYMax,sizeof m_dYMax);
	pFile->Write(&m_dZMax,sizeof m_dZMax);
	for(nX=0;nX<XPTS;nX++) {
		for(nY=0;nY<YPTS;nY++) {
			for(nZ=0;nZ<ZPTS;nZ++) {
				pFile->WriteStruct(&m_Rhomboid[nX][nY][nZ],m_Rhomboid[nX][nY][nZ].nStructSize = sizeof RhomboidStruct);
			}
		}
	}
	pFile->Write(&m_dXTipMinMax,sizeof m_dXTipMinMax);
	pFile->Write(&m_dYTipMinMax,sizeof m_dYTipMinMax);
	pFile->Write(&m_dXTipStep,sizeof m_dXTipStep);
	pFile->Write(&m_dYTipStep,sizeof m_dYTipStep);
	for(nX=0;nX<XTPTS;nX++) {
		for(nY=0;nY<YTPTS;nY++) {
			pFile->WriteStruct(&m_Tip[nX][nY],m_Tip[nX][nY].nStructSize = sizeof TipStruct);
		}
	}

	pFile->Write(&m_bRhomboidCompensation,sizeof m_bRhomboidCompensation);
	pFile->Write(&m_bTipCompensation,sizeof m_bTipCompensation);
	pFile->Write(&m_nAxesIncludedJoystick, sizeof m_nAxesIncludedJoystick);
	pFile->Write(&m_nAxesIncludedMoves, sizeof m_nAxesIncludedMoves);
	pFile->Write(m_fRobotWeightingX, sizeof m_fRobotWeightingX);
	pFile->Write(m_fRobotWeightingY, sizeof m_fRobotWeightingY);
	pFile->Write(m_fRobotWeightingZ, sizeof m_fRobotWeightingZ);
}


bool CKinematics::Retrieve(CString Name)
{
	CUSLFile File;
	CFileException ExceptionInstance;
	CString FilePath = theApp.m_LastSettingsPath + L"\\" + Name + L".kinematics";
	bool bFlag = false;
	
	if (File.Open(FilePath, CFile::typeBinary|CFile::modeRead, &ExceptionInstance)==0) { 
	
		CString Buff;
		Buff = L"Cannot Retrieve the Last Settings!: " + FilePath;
		MessageBox(NULL,Buff,_T("Warning"), MB_ICONERROR);
	} else {
		bFlag = Retrieve(&File);
		File.Close();

	}
	return bFlag;

}

bool CKinematics::Retrieve(CUSLFile* pFile)
{
	int nX,nY,nZ;
	WCHAR Identifier[0x100];
	float fVersion;

	ULONGLONG nStartOffset = pFile->GetPosition();

	pFile->Read(Identifier,sizeof KinematicIdentifier);
	pFile->Read(&fVersion,sizeof fVersion);

	if(fVersion >= theApp.m_fVersion && wcscmp(Identifier,KinematicIdentifier)==0 ) {

		pFile->Read(m_fXOffset,sizeof m_fXOffset);
		pFile->Read(m_fYOffset,sizeof m_fXOffset);
		pFile->Read(m_fZOffset,sizeof m_fXOffset);
		pFile->Read(m_fProbeLength,sizeof m_fProbeLength);
		pFile->Read(m_fArmLength,sizeof m_fArmLength);
		pFile->Read(m_fDesiredWaterPath,sizeof m_fDesiredWaterPath);
		pFile->Read(&m_dXStepSize,sizeof m_dXStepSize);
		pFile->Read(&m_dYStepSize,sizeof m_dYStepSize);
		pFile->Read(&m_dZStepSize,sizeof m_dZStepSize);
		pFile->Read(&m_dXMin,sizeof m_dXMin);
		pFile->Read(&m_dYMin,sizeof m_dYMin);
		pFile->Read(&m_dZMin,sizeof m_dZMin);
		pFile->Read(&m_dXMax,sizeof m_dXMax);
		pFile->Read(&m_dYMax,sizeof m_dYMax);
		pFile->Read(&m_dZMax,sizeof m_dZMax);
		for(nX=0;nX<XPTS;nX++) {
			for(nY=0;nY<YPTS;nY++) {
				for(nZ=0;nZ<ZPTS;nZ++) {
					pFile->ReadStruct(&m_Rhomboid[nX][nY][nZ], sizeof RhomboidStruct);
				}
			}
		}
		pFile->Read(&m_dXTipMinMax,sizeof m_dXTipMinMax);
		pFile->Read(&m_dYTipMinMax,sizeof m_dYTipMinMax);
		pFile->Read(&m_dXTipStep,sizeof m_dXTipStep);
		pFile->Read(&m_dYTipStep,sizeof m_dYTipStep);
		for(nX=0;nX<XTPTS;nX++) {
			for(nY=0;nY<YTPTS;nY++) {
				pFile->ReadStruct(&m_Tip[nX][nY], sizeof TipStruct);
			}
		}

		pFile->Read(&m_bRhomboidCompensation,sizeof m_bRhomboidCompensation);
		pFile->Read(&m_bTipCompensation,sizeof m_bTipCompensation);
		pFile->Read(&m_nAxesIncludedJoystick, sizeof m_nAxesIncludedJoystick);
		pFile->Read(&m_nAxesIncludedMoves, sizeof m_nAxesIncludedMoves);
		pFile->Read(m_fRobotWeightingX, sizeof m_fRobotWeightingX);
		pFile->Read(m_fRobotWeightingY, sizeof m_fRobotWeightingY);
		pFile->Read(m_fRobotWeightingZ, sizeof m_fRobotWeightingZ);

		CalculateIntegerVariables();
		return true;
	}
	return false;
}

float CKinematics::getToolLength(int nSide)
{
	float fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nSide] + m_fArmLength[nSide];

	return fToolLength;
}


void CKinematics::SurfaceFromHead(CCoord *CpHead, CCoord *CpSurface, int nSide, bool bCalculateSinCos,bool bUseDesiredWp, int nFl)
{
	float	fAngle,fToolLength,fH2,fFingerLength;
	int nWhich = 0;
	D3DXMATRIXA16 matWorld,matTranslation,matRotate,matTheta,matBase,matArm1,matArm2,matTool,matSwivel,matGimbal, matJ4, matJ5;
	D3DXVECTOR3 vecP;
	float fYaw,fPitch,fRoll,fXtip,fYtip,fYOffset;
	float temp;

	InvalidateDesiredWaterPath(CpHead);

	switch(theApp.m_LastSettings.nManipulatorType[nSide]) {
	case HORIZONTAL_WRIST:
		if (bUseDesiredWp == TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}

		D3DXMatrixTranslation(&matTool, 0.0f, fToolLength, 0.0f);
		D3DXMatrixTranslation(&matArm1, m_fXOffset[nSide], m_fYOffset[nSide], m_fZOffset[nSide]);
		D3DXMatrixRotationZ(&matJ4, CpHead->Side[nSide].fXt * DEG_TO_RAD);
		D3DXMatrixRotationX(&matJ5, CpHead->Side[nSide].fYt * DEG_TO_RAD);

		D3DXMatrixMultiply(&matWorld, &matTool, &matJ5);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matArm1);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matJ4);

		CpSurface->Side[nSide].fX = matWorld._41 + CpHead->Side[nSide].fX;
		CpSurface->Side[nSide].fY = matWorld._42 + CpHead->Side[nSide].fY;
		CpSurface->Side[nSide].fZ = matWorld._43 + CpHead->Side[nSide].fZ;

		break;
	case SWIVEL_Z_OFFSET_GIMBAL:
		if (bUseDesiredWp == TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}

		D3DXMatrixTranslation(&matTool, 0.0f, 0.0f, fToolLength);
		D3DXMatrixTranslation(&matArm1, m_fXOffset[nSide], m_fYOffset[nSide], m_fZOffset[nSide]);
		D3DXMatrixRotationZ(&matJ4, CpHead->Side[nSide].fXt * DEG_TO_RAD);
		D3DXMatrixRotationX(&matJ5, CpHead->Side[nSide].fYt * DEG_TO_RAD);

		D3DXMatrixMultiply(&matWorld, &matTool, &matJ5);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matArm1);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matJ4);

		CpSurface->Side[nSide].fX = matWorld._41 + CpHead->Side[nSide].fX;
		CpSurface->Side[nSide].fY = matWorld._42 + CpHead->Side[nSide].fY;
		CpSurface->Side[nSide].fZ = matWorld._43 + CpHead->Side[nSide].fZ;

		break;
	case VERTICAL_WRIST_PPMAC:
		if (bUseDesiredWp == TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}

		D3DXMatrixTranslation(&matTool, 0.0f, 0.0f, fToolLength);
		D3DXMatrixTranslation(&matArm1, m_fXOffset[nSide], m_fYOffset[nSide], m_fZOffset[nSide]);
		D3DXMatrixRotationY(&matJ4, CpHead->Side[nSide].fXt * DEG_TO_RAD);
		D3DXMatrixRotationX(&matJ5, CpHead->Side[nSide].fYt * DEG_TO_RAD);

		D3DXMatrixMultiply(&matWorld, &matTool, &matJ5);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matArm1);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matJ4);

		CpSurface->Side[nSide].fX = matWorld._41 + CpHead->Side[nSide].fX;
		CpSurface->Side[nSide].fY = matWorld._42 + CpHead->Side[nSide].fY;
		CpSurface->Side[nSide].fZ = matWorld._43 + CpHead->Side[nSide].fZ;
		break;
		
	case VERTICAL_WRIST:
		if (bCalculateSinCos == TRUE) {
			if (m_dXtip[nSide] - (fAngle = CpHead->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide] = fAngle;
				m_dCosXtip[nSide] = (float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide] = (float)sin(m_dXtip[nSide]);
			};
			if (m_dYtip[nSide] - (fAngle = CpHead->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide] = fAngle;
				m_dCosYtip[nSide] = (float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide] = (float)sin(m_dYtip[nSide]);
			};
		};
		nWhich = nSide;
		if (bUseDesiredWp == TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if (nSide == PORTSIDE) {
			fH2 = fToolLength * m_dCosYtip[nSide];
			CpSurface->Side[nSide].fY = CpHead->Side[nSide].fY + (m_fYOffset[nWhich] * m_dCosYtip[nSide]) + (fToolLength*m_dSinYtip[nSide]);
		}
		else {
			fH2 = fToolLength * m_dCosYtip[nSide] * -1.0f;
			CpSurface->Side[nSide].fY = CpHead->Side[nSide].fY + (m_fYOffset[nWhich] * m_dCosYtip[nSide]) - (fToolLength*m_dSinYtip[nSide]);
		}

		temp = (m_fXOffset[nWhich] * m_dSinXtip[nSide]);
		if (theApp.m_nJobNumber == NLR) {
			CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ - ((m_fZOffset[nWhich] + fH2)*m_dCosXtip[nSide]) + (m_fXOffset[nWhich] * m_dSinXtip[nSide]) + (m_dCosXtip[nSide] * (m_fYOffset[nWhich] * m_dSinYtip[nSide]));
		}
		else {
			CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ /*-*/ + ((m_fZOffset[nWhich] + fH2)*m_dCosXtip[nSide]) /*+*/ - (m_fXOffset[nWhich] * m_dSinXtip[nSide]) + (m_dCosXtip[nSide] * (m_fYOffset[nWhich] * m_dSinYtip[nSide]));
		}
		CpSurface->Side[nSide].fX = CpHead->Side[nSide].fX + ((m_fZOffset[nWhich] + fH2)*m_dSinXtip[nSide]) + (m_fXOffset[nWhich] * m_dCosXtip[nSide]) - (m_dSinXtip[nSide])*(m_fYOffset[nWhich] * m_dSinYtip[nSide]);;
		break;
		/*
	case HORIZONTAL_WRIST:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpHead->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpHead->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide==PORTSIDE) {
			fH2=fToolLength * m_dCosYtip[nSide];
			CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ + (m_fZOffset[nWhich] * m_dCosYtip[nSide]) + (fToolLength*m_dSinYtip[nSide]);
		} else {
			fH2=fToolLength * m_dCosYtip[nSide] * -1.0f;
			CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ + (m_fZOffset[nWhich] * m_dCosYtip[nSide]) - (fToolLength*m_dSinYtip[nSide]);
		}

		CpSurface->Side[nSide].fY = CpHead->Side[nSide].fY + ((m_fYOffset[nWhich] + fH2)*m_dCosXtip[nSide]) - (m_fXOffset[nWhich]*m_dSinXtip[nSide]) - (m_fZOffset[nWhich] * m_dSinYtip[nSide]);
		CpSurface->Side[nSide].fX = CpHead->Side[nSide].fX + ((m_fYOffset[nWhich] + fH2)*m_dSinXtip[nSide]) + (m_fXOffset[nWhich]*m_dCosXtip[nSide]);
		break;
		*/
	case TWIN_TOWER_FINGER:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpHead->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpHead->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide]-m_fProbeMirror[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide]-m_fProbeMirror[nWhich];
		}

		fYOffset = m_fProbeLength[nWhich] + m_fArmLength[nWhich] + m_fYOffset[nWhich];

		if(nSide==PORTSIDE) {
			fH2=fToolLength * m_dCosYtip[nSide];
			CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ + m_fZOffset[nWhich] + (fToolLength*m_dSinYtip[nSide]);
		} else {
			fH2=fToolLength * m_dCosYtip[nSide] * -1.0f;
			CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ + m_fZOffset[nWhich] - (fToolLength*m_dSinYtip[nSide]);
		}

		CpSurface->Side[nSide].fY = CpHead->Side[nSide].fY + ((fYOffset + fH2)*m_dCosXtip[nSide]) - (m_fXOffset[nWhich]*m_dSinXtip[nSide]);
		CpSurface->Side[nSide].fX = CpHead->Side[nSide].fX + ((fYOffset + fH2)*m_dSinXtip[nSide]) + (m_fXOffset[nWhich]*m_dCosXtip[nSide]);
		break;
	case INDEX_FINGER:	
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpHead->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpHead->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fFingerLength = m_fDesiredWaterPath[nSide] - m_fProbeMirror[nWhich];
		} else {
			fFingerLength = m_fWaterPath[nSide] - m_fProbeMirror[nWhich];
		}
		if(nSide==PORTSIDE) {
			fH2=m_fYOffset[nWhich] + (fFingerLength * m_dCosYtip[nSide]);
			CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ  + (fFingerLength*m_dSinYtip[nSide]);
		} else {
			fH2=m_fYOffset[nWhich] - (fFingerLength * m_dCosYtip[nSide]);
			CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ  - (fFingerLength*m_dSinYtip[nSide]);
		}

		CpSurface->Side[nSide].fX = CpHead->Side[nSide].fX + ((m_fXOffset[nWhich]*m_dCosXtip[nSide]) + (fH2 * m_dSinXtip[nSide]));
		CpSurface->Side[nSide].fY = CpHead->Side[nSide].fY + ((-m_fXOffset[nWhich]*m_dSinXtip[nSide]) + (fH2 * m_dCosXtip[nSide]));
		break;
	case TWI_MANIP:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpHead->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpHead->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide==PORTSIDE) {
			fH2=(fToolLength * m_dCosYtip[nSide]);
			CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ  + (fToolLength*m_dSinYtip[nSide]);
		} else {
			fH2= - (fToolLength * m_dCosYtip[nSide]);
			CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ  - (fToolLength*m_dSinYtip[nSide]);
		}

		CpSurface->Side[nSide].fX = CpHead->Side[nSide].fX + ((m_fXOffset[nWhich]*m_dCosXtip[nSide]) + (fH2 * m_dSinXtip[nSide]));
		CpSurface->Side[nSide].fY = CpHead->Side[nSide].fY + ((m_fXOffset[nWhich]*m_dSinXtip[nSide]) - (fH2 * m_dCosXtip[nSide]));
		break;
	case SWIVEL_GIMBAL:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpHead->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpHead->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		//New If Statement
		if(nSide==PORTSIDE) {
			fH2= (fToolLength * m_dCosYtip[nSide]) - (m_fZOffset[nWhich] * m_dSinYtip[nSide]);
			CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ  + ((fToolLength*m_dSinYtip[nSide]) + (m_fZOffset[nWhich] * m_dCosYtip[nSide]));
		} else {
			fH2=-((fToolLength * m_dCosYtip[nSide]) - (m_fZOffset[nWhich] * m_dSinYtip[nSide]));
			CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ  - ((fToolLength*m_dSinYtip[nSide]) + (m_fZOffset[nWhich] * m_dCosYtip[nSide]));
		}

		CpSurface->Side[nSide].fX = CpHead->Side[nSide].fX + ((m_fYOffset[nWhich]*m_dSinXtip[nSide]) + (m_fXOffset[nWhich]*m_dCosXtip[nSide]) + (fH2 * m_dSinXtip[nSide]));
		CpSurface->Side[nSide].fY = CpHead->Side[nSide].fY + ((m_fYOffset[nWhich]*m_dCosXtip[nSide]) - (m_fXOffset[nWhich]*m_dSinXtip[nSide]) + (fH2 * m_dCosXtip[nSide]));
		break;
	case ARCADIA_2:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpHead->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpHead->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide==PORTSIDE) {
			fH2=m_fXOffset[nWhich] + (fToolLength * m_dCosYtip[nSide]);
			CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ  + (fToolLength*m_dSinYtip[nSide]);
		} else {
			fH2=m_fXOffset[nWhich] - (fToolLength * m_dCosYtip[nSide]);
			CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ  - (fToolLength*m_dSinYtip[nSide]);
		}

		CpSurface->Side[nSide].fX = CpHead->Side[nSide].fX + ((-m_fYOffset[nWhich]*m_dSinXtip[nSide]) + (fH2 * m_dCosXtip[nSide]));
		CpSurface->Side[nSide].fY = CpHead->Side[nSide].fY + ((m_fYOffset[nWhich]*m_dCosXtip[nSide]) + (fH2 * m_dSinXtip[nSide]));
		break;
	case GOODRICH_4:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpHead->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpHead->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide==PORTSIDE) {
			fH2=m_fXOffset[nWhich] + (fToolLength * m_dCosYtip[nSide]);
			CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ  + (fToolLength*m_dSinYtip[nSide]);
		} else {
			fH2=m_fXOffset[nWhich] - (fToolLength * m_dCosYtip[nSide]);
			CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ  - (fToolLength*m_dSinYtip[nSide]);
		}

		CpSurface->Side[nSide].fX = CpHead->Side[nSide].fX + ((-m_fYOffset[nWhich]*m_dSinXtip[nSide]) + (fH2 * m_dCosXtip[nSide]));
		CpSurface->Side[nSide].fY = CpHead->Side[nSide].fY - ((m_fYOffset[nWhich]*m_dCosXtip[nSide]) + (fH2 * m_dSinXtip[nSide]));
		break;
	case DOWTY_B_SCAN:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpHead->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpHead->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide==PORTSIDE) {
			fH2=fToolLength * m_dCosYtip[nSide];
			CpSurface->Side[nSide].fY = CpHead->Side[nSide].fY - (m_fYOffset[nWhich]*m_dCosYtip[nSide]) + (fToolLength*m_dSinYtip[nSide]*m_dCosXtip[nSide]);
		} 

		CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ + (fToolLength * m_dCosYtip[nSide]*m_dCosXtip[nSide]) + (m_fYOffset[nWhich]*m_dSinYtip[nSide]);
		CpSurface->Side[nSide].fX = CpHead->Side[nSide].fX + (fToolLength * m_dSinXtip[nSide]) + (m_fXOffset[nWhich]*m_dCosXtip[nSide]);
		CpSurface->Side[nSide].fXt = CpHead->Side[nSide].fXt;
		CpSurface->Side[nSide].fYt = CpHead->Side[nSide].fYt;
		CpSurface->Side[nSide].fR = CpHead->Side[nSide].fR;

		break;	
	case VERTICAL_WRIST_90:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpHead->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpHead->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide==PORTSIDE) {
			fH2=fToolLength * m_dCosXtip[nSide];
			CpSurface->Side[nSide].fX = CpHead->Side[nSide].fX + m_fXOffset[nWhich] + (fToolLength*m_dSinXtip[nSide]);
		} else {
			fH2=fToolLength * m_dCosXtip[nSide] * -1.0f;
			CpSurface->Side[nSide].fX = CpHead->Side[nSide].fX + m_fXOffset[nWhich] - (fToolLength*m_dSinXtip[nSide]);
		}

		CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ - ((m_fZOffset[nWhich] + fH2)*m_dCosYtip[nSide]) + (m_fYOffset[nWhich]*m_dSinYtip[nSide]);
		CpSurface->Side[nSide].fY = CpHead->Side[nSide].fY + ((m_fZOffset[nWhich] + fH2)*m_dSinYtip[nSide]) + (m_fYOffset[nWhich]*m_dCosYtip[nSide]);
		break;
	case FOREPLANE_VERTICAL_WRIST_1:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpHead->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide] * -1.0f);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide] * -1.0f);
			};
			if(m_dYtip[nSide]-(fAngle=CpHead->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide] * -1.0f);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide] * -1.0f);
			};
		};
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide==PORTSIDE) {
			fH2=fToolLength * m_dCosYtip[nSide];
			CpSurface->Side[nSide].fY = CpHead->Side[nSide].fY + m_fYOffset[nWhich] + (fToolLength*m_dSinYtip[nSide]);
		} else {
			fH2=fToolLength * m_dCosYtip[nSide] * -1.0f;
			CpSurface->Side[nSide].fY = CpHead->Side[nSide].fY + m_fYOffset[nWhich] - (fToolLength*m_dSinYtip[nSide]);
		}

		CpSurface->Side[nSide].fZ = CpHead->Side[nSide].fZ + ((m_fZOffset[nWhich] + fH2)*m_dCosXtip[nSide]) - (m_fXOffset[nWhich]*m_dSinXtip[nSide]);
		CpSurface->Side[nSide].fX = CpHead->Side[nSide].fX + ((m_fZOffset[nWhich] + fH2)*m_dSinXtip[nSide]) + (m_fXOffset[nWhich]*m_dCosXtip[nSide]);
		break;

	case FOREPLANE_VERTICAL_WRIST:
		if(bCalculateSinCos==TRUE) {
			fYaw = fPitch = fRoll = 0.0f;
			fXtip = CpHead->Side[nSide].fXt;
			fYtip = CpHead->Side[nSide].fYt;
			if(m_nInvertXtip) fXtip *= -1.0f;
			if(m_nInvertYtip) fYtip *= -1.0f;
			switch(m_nXtipRotateAxis) {
			case 0:	fYaw = fXtip;
				break;
			case 1:	fPitch = fXtip;
				break;
			case 2:	fRoll = fXtip;
				break;
			}
			switch(m_nYtipRotateAxis) {
			case 0: fYaw = fYtip;
				break;
			case 1: fPitch = fYtip;
				break;
			case 2: fRoll = fYtip;
				break;
			}
			D3DXMatrixRotationYawPitchRoll(&m_matNormal, fYaw * DEG_TO_RAD, fPitch * DEG_TO_RAD, fRoll * DEG_TO_RAD);
		}
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}

		D3DXMatrixTranslation(&matTranslation,CpHead->Side[nSide].fX,CpHead->Side[nSide].fZ,CpHead->Side[nSide].fY);
		D3DXMatrixMultiply(&matWorld,&m_matNormal,&matTranslation);

		vecP.x = m_fXOffset[nWhich];
		vecP.y = fToolLength;
		vecP.z = m_fYOffset[nWhich];
		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
		CpSurface->Side[nSide].fX = vecP.x;
		CpSurface->Side[nSide].fY = vecP.z;
		CpSurface->Side[nSide].fZ = vecP.y;

		break;
	case INDEX_FINGER_V2:
		if(bCalculateSinCos==TRUE) {
			fXtip = CpHead->Side[nSide].fXt;
			fYtip = CpHead->Side[nSide].fYt;
			if(m_nInvertXtip) fXtip *= -1.0f;
			if(m_nInvertYtip) fYtip *= -1.0f;
			D3DXMatrixRotationY(&m_matY, fXtip * DEG_TO_RAD);
			D3DXMatrixRotationX(&m_matX, fYtip * DEG_TO_RAD);
		}
		nWhich=nSide;
		D3DXMatrixTranslation(&matTranslation,CpHead->Side[nSide].fX,CpHead->Side[nSide].fZ,CpHead->Side[nSide].fY);
		D3DXMatrixMultiply(&matWorld,&m_matNormal,&matTranslation);
		break;
	case ROTATE_DIF_THETA:
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}

		if(bCalculateSinCos==TRUE) {
			D3DXMatrixRotationX(&matTheta,CpHead->Side[nSide].fYt * DEG_TO_RAD);
			D3DXMatrixRotationZ(&matRotate,CpHead->Side[nSide].fXt * -DEG_TO_RAD);
			D3DXMatrixMultiply(&m_matNormal,&matTheta,&matRotate);
		}
		D3DXMatrixTranslation(&matTranslation,CpHead->Side[nSide].fX,CpHead->Side[nSide].fY,CpHead->Side[nSide].fZ);
		D3DXMatrixMultiply(&matWorld,&m_matNormal,&matTranslation);

		vecP = D3DXVECTOR3(0.0f,0.0f,fToolLength * -1.0f);
		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
		CpSurface->Side[nSide].pt = vecP;
		break;
	case ROTATE_THETA_HORIZONTAL:
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide == 1) fToolLength *= -1.0f;

		if(bCalculateSinCos==TRUE) {
			D3DXMatrixRotationX(&matTheta,CpHead->Side[nSide].fYt * DEG_TO_RAD);
			D3DXMatrixRotationY(&matRotate,CpHead->Side[nSide].fXt * DEG_TO_RAD);
			D3DXMatrixMultiply(&m_matNormal,&matTheta,&matRotate);
		}
		D3DXMatrixTranslation(&matTranslation,CpHead->Side[nSide].fX,CpHead->Side[nSide].fY,CpHead->Side[nSide].fZ);
		D3DXMatrixMultiply(&matWorld,&m_matNormal,&matTranslation);

		vecP = D3DXVECTOR3(0.0f,fToolLength,0.0f);
		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
		CpSurface->Side[nSide].pt = vecP;
		break;
	case ROTATE_THETA_VERTICAL:
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide == 1) fToolLength *= -1.0f;

		if(bCalculateSinCos==TRUE) {
			D3DXMatrixRotationX(&matTheta,CpHead->Side[nSide].fYt * DEG_TO_RAD);
			D3DXMatrixRotationZ(&matRotate,CpHead->Side[nSide].fXt * DEG_TO_RAD);
			D3DXMatrixMultiply(&m_matNormal,&matTheta,&matRotate);
		}
		D3DXMatrixTranslation(&matTranslation,CpHead->Side[nSide].fX,CpHead->Side[nSide].fY,CpHead->Side[nSide].fZ);
		D3DXMatrixMultiply(&matWorld,&m_matNormal,&matTranslation);

		vecP = D3DXVECTOR3(0.0f,fToolLength,0.0f);
		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
		CpSurface->Side[nSide].pt = vecP;
		break;
	case ROTATE_SWIVEL_GIMBAL:
//Help 1
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide];
		} else {
			fToolLength = m_fWaterPath[nSide];
		}
		if(nSide == 1) fToolLength *= -1.0f;

		vecP = m_vLimb1[nSide] + theApp.m_PhasedArray[nSide].m_FL[nFl].vecPt;
		vecP.z += fToolLength;

		D3DXMatrixTranslation(&matBase,CpHead->Side[nSide].fX,CpHead->Side[nSide].fY,CpHead->Side[nSide].fZ);
		D3DXMatrixTranslation(&matArm1,m_vLimb0[nSide].x,m_vLimb0[nSide].y,m_vLimb0[nSide].z);
		D3DXMatrixTranslation(&matTool,vecP.x,vecP.y,vecP.z);
		D3DXMatrixRotationZ(&matRotate,CpHead->Side[nSide].fR * DEG_TO_RAD);
		D3DXMatrixRotationX(&matSwivel,-CpHead->Side[nSide].fXt * DEG_TO_RAD);
		D3DXMatrixRotationY(&matGimbal,CpHead->Side[nSide].fYt * DEG_TO_RAD);


		vecP = D3DXVECTOR3(0.0f,0.0f,0.0f);
		D3DXVec3TransformCoord(&vecP,&vecP,&matTool);
		D3DXVec3TransformCoord(&vecP,&vecP,&matGimbal);
		D3DXVec3TransformCoord(&vecP,&vecP,&matArm1);
		D3DXVec3TransformCoord(&vecP,&vecP,&matSwivel);
		D3DXVec3TransformCoord(&vecP,&vecP,&matRotate);
		D3DXVec3TransformCoord(&vecP,&vecP,&matBase);

		CpSurface->Side[nSide].pt = vecP;

		vecP = D3DXVECTOR3(0.0f,0.0f,-1.0f);
		D3DXVec3TransformCoord(&vecP,&vecP,&matGimbal);
		D3DXVec3TransformCoord(&vecP,&vecP,&matSwivel);
		D3DXVec3TransformCoord(&vecP,&vecP,&matRotate);
		CpSurface->Side[nSide].fI = vecP.x;
		CpSurface->Side[nSide].fJ = vecP.y;
		CpSurface->Side[nSide].fK = vecP.z;

		break;

	}

	CpSurface->Side[nSide].fXt = CpHead->Side[nSide].fXt;
	CpSurface->Side[nSide].fYt = CpHead->Side[nSide].fYt;
	CpSurface->Side[nSide].fR = CpHead->Side[nSide].fR;

}




void CKinematics::HeadFromSurface(CCoord *CpSurface, CCoord *CpHead, int nSide, bool bCalculateSinCos,bool bUseDesiredWp, CCoord *CpCurrent, int nFl)
{
	float	fAngle,fToolLength,fH2,fFingerLength;
	int nWhich = 0;
	D3DXMATRIXA16 matWorld,matTranslation,matInvWorld,matRotate,matTheta,matArm1,matTool,matSwivel,matGimbal, matJ4, matJ5;
	D3DXVECTOR3 vecP,vecGimbalJoint,vecNorm,vecSwivelJoint;
	float fYaw,fPitch,fRoll,fXtip,fYtip,fYOffset;
	float fXtAngle,fYtAngle;
	double TLcosYCosX, fOffSinY;
	D3DXMATRIXA16 matQuatRot;
	D3DXQUATERNION  Quat;
	CCoord Cp;
	
	InvalidateDesiredWaterPath(CpHead);

	switch(theApp.m_LastSettings.nManipulatorType[nSide]) {
	case SWIVEL_Z_OFFSET_GIMBAL:
		TipsFromNormals(CpSurface, nSide);
		if (bUseDesiredWp == TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}

		D3DXMatrixTranslation(&matTool, 0.0f, 0.0f, fToolLength);
		D3DXMatrixTranslation(&matArm1, m_fXOffset[nSide], 0.0f, 0.0f);
		D3DXMatrixRotationZ(&matJ4, CpHead->Side[nSide].fXt * DEG_TO_RAD);
		D3DXMatrixRotationX(&matJ5, CpHead->Side[nSide].fYt * DEG_TO_RAD);

		D3DXMatrixMultiply(&matWorld, &matTool, &matJ5);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matArm1);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matJ4);

		CpHead->Side[nSide].fX =  CpSurface->Side[nSide].fX - matWorld._41;
		CpHead->Side[nSide].fY =  CpSurface->Side[nSide].fY - matWorld._42;
		CpHead->Side[nSide].fZ =  CpSurface->Side[nSide].fZ - matWorld._43;
		break;
	case VERTICAL_WRIST:
		if (bCalculateSinCos == TRUE) {
			if (m_dXtip[nSide] - (fAngle = CpSurface->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide] = fAngle;
				m_dCosXtip[nSide] = (float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide] = (float)sin(m_dXtip[nSide]);
			};
			if (m_dYtip[nSide] - (fAngle = CpSurface->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide] = fAngle;
				m_dCosYtip[nSide] = (float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide] = (float)sin(m_dYtip[nSide]);
			};
		};
		nWhich = nSide;
		if (bUseDesiredWp == TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if (nSide == PORTSIDE) {
			fH2 = fToolLength * m_dCosYtip[nSide];
			CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY - (m_fYOffset[nWhich] * m_dCosYtip[nSide]) - (fToolLength*m_dSinYtip[nSide]);
		}
		else {
			fH2 = fToolLength * m_dCosYtip[nSide] * -1.0f;
			CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY - (m_fYOffset[nWhich] * m_dCosYtip[nSide]) + (fToolLength*m_dSinYtip[nSide]);
		}

		if (theApp.m_nJobNumber == NLR) {
			CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ + ((m_fZOffset[nWhich] + fH2)*m_dCosXtip[nSide]) - (m_fXOffset[nWhich] * m_dSinXtip[nSide]) - (m_dCosXtip[nSide] * (m_fYOffset[nWhich] * m_dSinYtip[nSide]));
		}
		else {
			CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ /*+*/ - ((m_fZOffset[nWhich] + fH2)*m_dCosXtip[nSide]) /*-*/ + (m_fXOffset[nWhich] * m_dSinXtip[nSide]) - (m_dCosXtip[nSide] * (m_fYOffset[nWhich] * m_dSinYtip[nSide]));
		}
		CpHead->Side[nSide].fX = CpSurface->Side[nSide].fX - ((m_fZOffset[nWhich] + fH2)*m_dSinXtip[nSide]) - (m_fXOffset[nWhich] * m_dCosXtip[nSide]) + (m_dSinXtip[nSide])*(m_fYOffset[nWhich] * m_dSinYtip[nSide]);;
		CpHead->Side[nSide].fXt = CpSurface->Side[nSide].fXt;
		CpHead->Side[nSide].fYt = CpSurface->Side[nSide].fYt;
		CpHead->Side[nSide].fR = CpSurface->Side[nSide].fR;
		break;
	case VERTICAL_WRIST_PPMAC:
		if (bUseDesiredWp == TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}

		D3DXMatrixTranslation(&matTool, 0.0f, 0.0f, fToolLength);
		D3DXMatrixTranslation(&matArm1, m_fXOffset[nSide], 0.0f, 0.0f);
		D3DXMatrixRotationY(&matJ4, CpSurface->Side[nSide].fXt * DEG_TO_RAD);
		D3DXMatrixRotationX(&matJ5, CpSurface->Side[nSide].fYt * DEG_TO_RAD);

		D3DXMatrixMultiply(&matWorld, &matTool, &matJ5);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matArm1);
		D3DXMatrixMultiply(&matWorld, &matWorld, &matJ4);

		CpHead->Side[nSide].fX = CpSurface->Side[nSide].fX - matWorld._41;
		CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY - matWorld._42;
		CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ - matWorld._43;
		CpHead->Side[nSide].fXt = CpSurface->Side[nSide].fXt;
		CpHead->Side[nSide].fYt = CpSurface->Side[nSide].fYt;
		CpHead->Side[nSide].fR = CpSurface->Side[nSide].fR;

		break;
	case HORIZONTAL_WRIST:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpSurface->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpSurface->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide==PORTSIDE) {
			fH2=fToolLength * m_dCosYtip[nSide];
			CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ - (m_fZOffset[nWhich] * m_dCosYtip[nSide]) - (fToolLength*m_dSinYtip[nSide]);
		} else {
			fH2=fToolLength * m_dCosYtip[nSide] * -1.0f;
			CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ - (m_fZOffset[nWhich] * m_dCosYtip[nSide]) + (fToolLength*m_dSinYtip[nSide]);
		}

		CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY - ((m_fYOffset[nWhich] + fH2)*m_dCosXtip[nSide]) + (m_fXOffset[nWhich]*m_dSinXtip[nSide]) + (m_fZOffset[nWhich] * m_dSinYtip[nSide]);
		CpHead->Side[nSide].fX = CpSurface->Side[nSide].fX - ((m_fYOffset[nWhich] + fH2)*m_dSinXtip[nSide]) - (m_fXOffset[nWhich]*m_dCosXtip[nSide]);
		CpHead->Side[nSide].fXt = CpSurface->Side[nSide].fXt;
		CpHead->Side[nSide].fYt = CpSurface->Side[nSide].fYt;
		CpHead->Side[nSide].fR = CpSurface->Side[nSide].fR;
		break;
	case TWIN_TOWER_FINGER:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpSurface->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpSurface->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide]- m_fProbeMirror[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide]- m_fProbeMirror[nWhich];
		}

		fYOffset = m_fProbeLength[nWhich] + m_fArmLength[nWhich] + m_fYOffset[nWhich];

		if(nSide==PORTSIDE) {
			fH2=fToolLength * m_dCosYtip[nSide];
			CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ - m_fZOffset[nWhich] - (fToolLength*m_dSinYtip[nSide]);
		} else {
			fH2=fToolLength * m_dCosYtip[nSide] * -1.0f;
			CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ - m_fZOffset[nWhich] + (fToolLength*m_dSinYtip[nSide]);
		}

		CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY - ((fYOffset + fH2)*m_dCosXtip[nSide]) + (m_fXOffset[nWhich]*m_dSinXtip[nSide]);
		CpHead->Side[nSide].fX = CpSurface->Side[nSide].fX - ((fYOffset + fH2)*m_dSinXtip[nSide]) - (m_fXOffset[nWhich]*m_dCosXtip[nSide]);
		CpHead->Side[nSide].fXt = CpSurface->Side[nSide].fXt;
		CpHead->Side[nSide].fYt = CpSurface->Side[nSide].fYt;
		CpHead->Side[nSide].fR = CpSurface->Side[nSide].fR;
		break;
	case INDEX_FINGER:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpSurface->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpSurface->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fFingerLength = m_fDesiredWaterPath[nSide] - m_fProbeMirror[nWhich];
		} else {
			fFingerLength = m_fWaterPath[nSide] - m_fProbeMirror[nWhich];
		}
		if(nSide==PORTSIDE) {
			fH2=m_fYOffset[nWhich] + (fFingerLength * m_dCosYtip[nSide]);
			CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ - (fFingerLength*m_dSinYtip[nSide]);
		} else {
			fH2=m_fYOffset[nWhich] - (fFingerLength * m_dCosYtip[nSide]);
			CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ + (fFingerLength*m_dSinYtip[nSide]);
		}

		CpHead->Side[nSide].fX = CpSurface->Side[nSide].fX - ((m_fXOffset[nWhich]*m_dCosXtip[nSide]) + (fH2 * m_dSinXtip[nSide]));
		CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY - (-(m_fXOffset[nWhich]*m_dSinXtip[nSide]) + (fH2 * m_dCosXtip[nSide]));
		CpHead->Side[nSide].fXt = CpSurface->Side[nSide].fXt;
		CpHead->Side[nSide].fYt = CpSurface->Side[nSide].fYt;
		CpHead->Side[nSide].fR = CpSurface->Side[nSide].fR;
		break;
	case ROTATE_SWIVEL_GIMBAL:
//Help 1
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide];
		} else {
			fToolLength = m_fWaterPath[nSide];
		}
		if(nSide == 1) fToolLength *= -1.0f;

		Cp.Side[nSide] = CpSurface->Side[nSide];
		Cp.InvertNorm(nSide);
		Cp.QuaternionFromNormal(nSide,&Quat);
		D3DXMatrixRotationQuaternion(&matQuatRot,&Quat);
	
		fXtAngle = atan2f(matQuatRot._32,matQuatRot._33);
		if((matQuatRot._32 == 0.0f) && (matQuatRot._33 == 0.0f)) fXtAngle = 0.0f;
		fYtAngle = (PIf/2.0f) + -acosf(matQuatRot._31);

		//Calculate pos of gimbal joint
		D3DXMatrixRotationY(&matGimbal,fYtAngle);
		D3DXMatrixRotationX(&matSwivel,-fXtAngle);

		vecP = (m_vLimb1[nSide] + theApp.m_PhasedArray[nSide].m_FL[nFl].vecPt) * -1;
		vecP.z -= fToolLength;
		D3DXVec3TransformCoord(&vecGimbalJoint,&vecP,&matGimbal);
		D3DXVec3TransformCoord(&vecGimbalJoint,&vecGimbalJoint,&matSwivel);

		//calculate pos of swivel joint
		vecP = m_vLimb0[nSide] * -1;
		D3DXVec3TransformCoord(&vecSwivelJoint,&vecP,&matSwivel);
		
		CpHead->Side[nSide].pt = CpSurface->Side[nSide].pt + vecSwivelJoint + vecGimbalJoint;
		CpHead->Side[nSide].norm = CpSurface->Side[nSide].norm;

		//Tips from normals
		CpHead->Side[nSide].fR=0.0f;
		CpHead->Side[nSide].fXt=fXtAngle * RAD_TO_DEG;
		CpHead->Side[nSide].fYt=fYtAngle * RAD_TO_DEG;

		break;
	case TWI_MANIP:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpSurface->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpSurface->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich = nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide==PORTSIDE) {
			fH2=fToolLength * m_dCosYtip[nSide];
			CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ - (fToolLength*m_dSinYtip[nSide]);
		} else {
			fH2= - (fToolLength * m_dCosYtip[nSide]);
			CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ + (fToolLength*m_dSinYtip[nSide]);
		}

		CpHead->Side[nSide].fX = CpSurface->Side[nSide].fX - ((m_fXOffset[nWhich]*m_dCosXtip[nSide]) + (fH2 * m_dSinXtip[nSide]));
		CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY - ((m_fXOffset[nWhich]*m_dSinXtip[nSide]) - (fH2 * m_dCosXtip[nSide]));
		CpHead->Side[nSide].fXt = CpSurface->Side[nSide].fXt;
		CpHead->Side[nSide].fYt = CpSurface->Side[nSide].fYt;
		CpHead->Side[nSide].fR = CpSurface->Side[nSide].fR;
		break;
	case SWIVEL_GIMBAL:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpSurface->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpSurface->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich = nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		//New if statement
		if(nSide==PORTSIDE) {
			fH2 = (fToolLength * m_dCosYtip[nSide]) - (m_fZOffset[nWhich] * m_dSinYtip[nSide]);
			CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ - ((fToolLength*m_dSinYtip[nSide]) + (m_fZOffset[nWhich] * m_dCosYtip[nSide]));
		} else {
			fH2 = -((fToolLength * m_dCosYtip[nSide]) - (m_fZOffset[nWhich] * m_dSinYtip[nSide]));
			CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ + ((fToolLength*m_dSinYtip[nSide]) + (m_fZOffset[nWhich] * m_dCosYtip[nSide]));
		}

		CpHead->Side[nSide].fX = CpSurface->Side[nSide].fX - ((m_fYOffset[nWhich]*m_dSinXtip[nSide]) + (m_fXOffset[nWhich]*m_dCosXtip[nSide]) + (fH2 * m_dSinXtip[nSide]));
		CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY - ((m_fYOffset[nWhich]*m_dCosXtip[nSide]) - (m_fXOffset[nWhich]*m_dSinXtip[nSide]) + (fH2 * m_dCosXtip[nSide]));
		CpHead->Side[nSide].fXt = CpSurface->Side[nSide].fXt;
		CpHead->Side[nSide].fYt = CpSurface->Side[nSide].fYt;
		CpHead->Side[nSide].fR = CpSurface->Side[nSide].fR;
		break;
	case ARCADIA_2:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpSurface->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpSurface->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich = nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide==PORTSIDE) {
			fH2=m_fXOffset[nWhich] + (fToolLength * m_dCosYtip[nSide]);
			CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ - (fToolLength*m_dSinYtip[nSide]);
		} else {
			fH2=m_fXOffset[nWhich] - (fToolLength * m_dCosYtip[nSide]);
			CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ + (fToolLength*m_dSinYtip[nSide]);
		}

		CpHead->Side[nSide].fX = CpSurface->Side[nSide].fX - (-(m_fYOffset[nWhich]*m_dSinXtip[nSide]) + (fH2 * m_dCosXtip[nSide]));
		CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY - ((m_fYOffset[nWhich]*m_dCosXtip[nSide]) + (fH2 * m_dSinXtip[nSide]));
		CpHead->Side[nSide].fXt = CpSurface->Side[nSide].fXt;
		CpHead->Side[nSide].fYt = CpSurface->Side[nSide].fYt;
		CpHead->Side[nSide].fR = CpSurface->Side[nSide].fR;
		break;
	case GOODRICH_4:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpSurface->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpSurface->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich = nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide==PORTSIDE) {
			fH2=m_fXOffset[nWhich] + (fToolLength * m_dCosYtip[nSide]);
			CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ - (fToolLength*m_dSinYtip[nSide]);
		} else {
			fH2=m_fXOffset[nWhich] - (fToolLength * m_dCosYtip[nSide]);
			CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ + (fToolLength*m_dSinYtip[nSide]);
		}

		CpHead->Side[nSide].fX = CpSurface->Side[nSide].fX - (-(m_fYOffset[nWhich]*m_dSinXtip[nSide]) + (fH2 * m_dCosXtip[nSide]));
		CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY + ((m_fYOffset[nWhich]*m_dCosXtip[nSide]) + (fH2 * m_dSinXtip[nSide]));
		CpHead->Side[nSide].fXt = CpSurface->Side[nSide].fXt;
		CpHead->Side[nSide].fYt = CpSurface->Side[nSide].fYt;
		CpHead->Side[nSide].fR = CpSurface->Side[nSide].fR;
		break;
	case DOWTY_B_SCAN:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpSurface->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpSurface->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
	
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide==PORTSIDE) {
			fH2=fToolLength * m_dCosYtip[nSide];
			CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY + (m_fYOffset[nWhich]*m_dCosYtip[nSide]) - (fToolLength*m_dSinYtip[nSide]*m_dCosXtip[nSide]);
		} 

		TLcosYCosX = fToolLength * m_dCosYtip[nSide]*m_dCosXtip[nSide];
		fOffSinY = m_fYOffset[nWhich]*m_dSinYtip[nSide];
	
		CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ - (float)((TLcosYCosX) + (fOffSinY));
		CpHead->Side[nSide].fX = CpSurface->Side[nSide].fX - (fH2*m_dSinXtip[nSide]) - (m_fXOffset[nWhich]*m_dCosXtip[nSide]);
		CpHead->Side[nSide].fXt = CpSurface->Side[nSide].fXt;
		CpHead->Side[nSide].fYt = CpSurface->Side[nSide].fYt;
		CpHead->Side[nSide].fR = CpSurface->Side[nSide].fR;
		break;
	case VERTICAL_WRIST_90:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpSurface->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide]);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide]);
			};
			if(m_dYtip[nSide]-(fAngle=CpSurface->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide]);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide]);
			};
		};
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide==PORTSIDE) {
			fH2=fToolLength * m_dCosXtip[nSide];
			CpHead->Side[nSide].fX = CpSurface->Side[nSide].fX - m_fXOffset[nWhich] - (fToolLength*m_dSinXtip[nSide]);
		} else {
			fH2=fToolLength * m_dCosXtip[nSide] * -1.0f;
			CpHead->Side[nSide].fX = CpSurface->Side[nSide].fX - m_fXOffset[nWhich] + (fToolLength*m_dSinXtip[nSide]);
		}

		CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ + ((m_fZOffset[nWhich] + fH2)*m_dCosYtip[nSide]) - (m_fYOffset[nWhich]*m_dSinYtip[nSide]);
		CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY - ((m_fZOffset[nWhich] + fH2)*m_dSinYtip[nSide]) - (m_fYOffset[nWhich]*m_dCosYtip[nSide]);
		CpHead->Side[nSide].fXt = CpSurface->Side[nSide].fXt;
		CpHead->Side[nSide].fYt = CpSurface->Side[nSide].fYt;
		CpHead->Side[nSide].fR = CpSurface->Side[nSide].fR;
		break;
	case FOREPLANE_VERTICAL_WRIST_1:
		if(bCalculateSinCos==TRUE) {
			if(m_dXtip[nSide]-(fAngle=CpSurface->Side[nSide].fXt*(float)DEG_TO_RAD)) {
				m_dXtip[nSide]=fAngle;
				m_dCosXtip[nSide]=(float)cos(m_dXtip[nSide] * -1.0f);
				m_dSinXtip[nSide]=(float)sin(m_dXtip[nSide] * -1.0f);
			};
			if(m_dYtip[nSide]-(fAngle=CpSurface->Side[nSide].fYt*(float)DEG_TO_RAD)) {
				m_dYtip[nSide]=fAngle;
				m_dCosYtip[nSide]=(float)cos(m_dYtip[nSide] * -1.0f);
				m_dSinYtip[nSide]=(float)sin(m_dYtip[nSide] * -1.0f);
			};
		};//????????????
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide==PORTSIDE) {
			fH2=fToolLength * m_dCosYtip[nSide];
			CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY - m_fYOffset[nWhich] - (fToolLength*m_dSinYtip[nSide]);
		} else {
			fH2=fToolLength * m_dCosYtip[nSide] * -1.0f;
			CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY - m_fYOffset[nWhich] + (fToolLength*m_dSinYtip[nSide]);
		}

		CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ - ((m_fZOffset[nWhich] + fH2)*m_dCosXtip[nSide]) + (m_fXOffset[nWhich]*m_dSinXtip[nSide]);
		CpHead->Side[nSide].fX = CpSurface->Side[nSide].fX - ((m_fZOffset[nWhich] + fH2)*m_dSinXtip[nSide]) - (m_fXOffset[nWhich]*m_dCosXtip[nSide]);
		CpHead->Side[nSide].fXt = CpSurface->Side[nSide].fXt;
		CpHead->Side[nSide].fYt = CpSurface->Side[nSide].fYt;
		CpHead->Side[nSide].fR = CpSurface->Side[nSide].fR;
		break;
	case FOREPLANE_VERTICAL_WRIST:
		if(bCalculateSinCos==TRUE) {
			fYaw = fPitch = fRoll = 0.0f;
			fXtip = CpSurface->Side[nSide].fXt;
			fYtip = CpSurface->Side[nSide].fYt;
			if(m_nInvertXtip) fXtip *= -1.0f;
			if(m_nInvertYtip) fYtip *= -1.0f;
			switch(m_nXtipRotateAxis) {
			case 0:	fYaw = fXtip;
				break;
			case 1:	fPitch = fXtip;
				break;
			case 2:	fRoll = fXtip;
				break;
			}
			switch(m_nYtipRotateAxis) {
			case 0: fYaw = fYtip;
				break;
			case 1: fPitch = fYtip;
				break;
			case 2: fRoll = fYtip;
				break;
			}
			D3DXMatrixRotationYawPitchRoll(&m_matNormal, fYaw * DEG_TO_RAD, fPitch * DEG_TO_RAD, fRoll * DEG_TO_RAD);
		}
		nWhich=nSide;
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}

		D3DXMatrixTranslation(&matTranslation,CpSurface->Side[nSide].fX,CpSurface->Side[nSide].fZ,CpSurface->Side[nSide].fY);
		D3DXMatrixMultiply(&matWorld,&m_matNormal,&matTranslation);
		D3DXMatrixInverse(&matInvWorld,NULL,&matWorld);

		vecP.x = -m_fXOffset[nWhich];
		vecP.y = -fToolLength;
		vecP.z = -m_fYOffset[nWhich];
		D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
		CpHead->Side[nSide].fX = vecP.x;
		CpHead->Side[nSide].fY = vecP.z;
		CpHead->Side[nSide].fZ = vecP.y;
		CpHead->Side[nSide].fXt = CpSurface->Side[nSide].fXt;
		CpHead->Side[nSide].fYt = CpSurface->Side[nSide].fYt;
		CpHead->Side[nSide].fR = CpSurface->Side[nSide].fR;

		break;

	case ROTATE_DIF_THETA:
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}

		CpHead->Side[nSide].pt = vecP;
		CpHead->Side[nSide].fX = CpSurface->Side[nSide].fX + CpSurface->Side[nSide].fI * fToolLength;
		CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY + CpSurface->Side[nSide].fJ * fToolLength;
		CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ + CpSurface->Side[nSide].fK * fToolLength;
		CpHead->Side[nSide].norm = CpSurface->Side[nSide].norm;

		if(CpCurrent != NULL) CpSurface->Side[nSide].fW = CpCurrent->Side[nSide].fW;

		CpHead->Side[nSide].fYt=atan2f(sqrtf((CpSurface->Side[nSide].fJ*CpSurface->Side[nSide].fJ) + CpSurface->Side[nSide].fI*CpSurface->Side[nSide].fI),CpSurface->Side[nSide].fK) * RAD_TO_DEG;
		if(CpSurface->Side[nSide].fW < 0.0f) CpHead->Side[nSide].fYt *= -1.0f;
		//That's the easy bit
		//Now lets make sure it's the shortest route
		if(CpCurrent != NULL) {
			if( ((Signf(CpSurface->Side[nSide].fI) != Signf(CpCurrent->Side[nSide].fI)) && (fabsf(CpSurface->Side[nSide].fJ) < 0.01)) ||
				((Signf(CpSurface->Side[nSide].fJ) != Signf(CpCurrent->Side[nSide].fJ)) && (fabsf(CpSurface->Side[nSide].fI) < 0.01))) {
				CpHead->Side[nSide].fYt *= -1.0f;
			}
		}
		CpHead->Side[nSide].fW = sinf(CpHead->Side[nSide].fYt * DEG_TO_RAD);


		if(CpHead->Side[nSide].fW >= 0.0f) {
			CpHead->Side[nSide].fXt=atan2f((CpSurface->Side[nSide].fI * -1.0f) , (CpSurface->Side[nSide].fJ*-1.0f)) * RAD_TO_DEG;
		} else {
			CpHead->Side[nSide].fXt=atan2f((CpSurface->Side[nSide].fI * 1.0f) , (CpSurface->Side[nSide].fJ*1.0f)) * RAD_TO_DEG;
		}
		//That's the easy bit
		//Now lets make sure it's the shortest route
		if(CpCurrent != NULL) {
			//Check to see if it goes between -180 and 180 
			if(Signf(CpHead->Side[nSide].fXt) != Signf(CpCurrent->Side[nSide].fXt)) {
				if((Quadrant(CpHead->Side[nSide].fXt) == 'S') && (Quadrant(CpCurrent->Side[nSide].fXt) == 'C')) {
					CpHead->Side[nSide].fXt -= 180.0f;
					CpHead->Side[nSide].fYt *= -1.0f;
					CpHead->Side[nSide].fW = sinf(CpHead->Side[nSide].fYt * DEG_TO_RAD);
					break;
				}
				if((Quadrant(CpHead->Side[nSide].fXt) == 'C') && (Quadrant(CpCurrent->Side[nSide].fXt) == 'S')) {
					CpHead->Side[nSide].fXt += 180.0f;
					CpHead->Side[nSide].fYt *= -1.0f;
					CpHead->Side[nSide].fW = sinf(CpHead->Side[nSide].fYt * DEG_TO_RAD);
					break;
				}
				if((Quadrant(CpHead->Side[nSide].fXt) == 'A') && (Quadrant(CpCurrent->Side[nSide].fXt) == 'T')) {
					CpHead->Side[nSide].fXt -= 180.0f;
					CpHead->Side[nSide].fYt *= -1.0f;
					CpHead->Side[nSide].fW = sinf(CpHead->Side[nSide].fYt * DEG_TO_RAD);
					break;
				}
				if((Quadrant(CpHead->Side[nSide].fXt) == 'T') && (Quadrant(CpCurrent->Side[nSide].fXt) == 'A')) {
					CpHead->Side[nSide].fXt += 180.0f;
					CpHead->Side[nSide].fYt *= -1.0f;
					CpHead->Side[nSide].fW = sinf(CpHead->Side[nSide].fYt * DEG_TO_RAD);
					break;
				}
				if(fabs(CpHead->Side[nSide].fXt - CpCurrent->Side[nSide].fXt) > 180.0f) {
					if(CpHead->Side[nSide].fXt>=0.0f) {
						CpHead->Side[nSide].fXt -= 360.0f;
					} else {
						CpHead->Side[nSide].fXt += 360.0f;
					}
					break;
				}
			}
			//check to see if it goes past 360
			if((Signf(CpHead->Side[nSide].fXt) & Signf(CpCurrent->Side[nSide].fXt)) == POSITIVE) {
				if((CpCurrent->Side[nSide].fXt - CpHead->Side[nSide].fXt) > 180.0f) {
					CpHead->Side[nSide].fXt += 360.0f;
					break;
				}
			}
			//check to see if it goes past -360
			if((Signf(CpHead->Side[nSide].fXt) & Signf(CpCurrent->Side[nSide].fXt)) == NEGATIVE) {
				if((CpHead->Side[nSide].fXt - CpCurrent->Side[nSide].fXt) > 180.0f) {
					CpHead->Side[nSide].fXt -= 360.0f;
					break;
				}
			}
		} else {
			if(fabsf(CpHead->Side[nSide].fXt) > 90.0f) {
				if(CpHead->Side[nSide].fXt > 90.0f) {
					CpHead->Side[nSide].fXt -= 180.0f;
				} else {
					CpHead->Side[nSide].fXt += 180.0f;
				}
				CpHead->Side[nSide].fYt *= -1.0f;
				CpHead->Side[nSide].fW = sinf(CpHead->Side[nSide].fYt * DEG_TO_RAD);
			}
		}
		break;
	case ROTATE_THETA_HORIZONTAL:
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide == 1) fToolLength *= -1.0f;

		CpHead->Side[nSide].fX = CpSurface->Side[nSide].fX + CpSurface->Side[nSide].fI * fToolLength;
		CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY + CpSurface->Side[nSide].fJ * fToolLength;
		CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ + CpSurface->Side[nSide].fK * fToolLength;
		CpHead->Side[nSide].norm = CpSurface->Side[nSide].norm;

		if(CpCurrent != NULL) {
			CpHead->Side[nSide].fW = CpCurrent->Side[nSide].fW;
		} else {
			CpHead->Side[nSide].fW = cosf(CpSurface->Side[nSide].fYt * DEG_TO_RAD);
		}

		TipsFromNormals(CpHead,nSide,0.0f);

		CpHead->Side[nSide].fW = sinf(CpHead->Side[nSide].fYt * DEG_TO_RAD);


		if(theApp.m_LastSettings.nRThetaUseW) {
			if(CpHead->Side[nSide].fW >= 0.0f) {
				CpHead->Side[nSide].fXt=atan2f((CpSurface->Side[nSide].fI * -1.0f) , (CpSurface->Side[nSide].fK*-1.0f)) * RAD_TO_DEG;
			} else {
				CpHead->Side[nSide].fXt=atan2f((CpSurface->Side[nSide].fI * 1.0f) , (CpSurface->Side[nSide].fK*1.0f)) * RAD_TO_DEG;
			}
		} else {
			CpHead->Side[nSide].fXt=atan2f((CpSurface->Side[nSide].fI * -1.0f) , (CpSurface->Side[nSide].fK*-1.0f)) * RAD_TO_DEG;
		}
		if(CpSurface->Side[nSide].fI == 0.0f && CpSurface->Side[nSide].fK == 0.0f) CpHead->Side[nSide].fXt = 0.0f;

		//Now lets make sure it's the shortest route
		if((CpCurrent != NULL) && (theApp.m_LastSettings.nRThetaLookBack==1)) {

			if((Signf(CpHead->Side[nSide].fXt) != Signf(CpCurrent->Side[nSide].fXt)) && (Signf(CpHead->Side[nSide].fYt) != Signf(CpCurrent->Side[nSide].fYt))) {
				if(CpHead->Side[nSide].fXt <= 0.0f) {
					CpHead->Side[nSide].fXt -= 180.0f;
				} else {
					CpHead->Side[nSide].fXt -= 180.0f;
				}
				CpHead->Side[nSide].fYt *= -1.0f;
				CpHead->Side[nSide].fW = sinf(CpHead->Side[nSide].fYt * DEG_TO_RAD);
				break;
			}

			if(fabs(CpHead->Side[nSide].fXt - CpCurrent->Side[nSide].fXt) >= 180.0f) {
				if(Signf(CpHead->Side[nSide].fXt)==POSITIVE) {
					CpHead->Side[nSide].fXt -= 360.0f;
				} else {
					CpHead->Side[nSide].fXt += 360.0f;
				}
				break;
			}
			
		} else {
			if(fabsf(CpHead->Side[nSide].fXt) > 90.0f) {
				if(CpHead->Side[nSide].fXt > 90.0f) {
					CpHead->Side[nSide].fXt -= 180.0f;
				} else {
					CpHead->Side[nSide].fXt += 180.0f;
				}
				CpHead->Side[nSide].fYt *= -1.0f;
				CpHead->Side[nSide].fW = sinf(CpHead->Side[nSide].fYt * DEG_TO_RAD);
			}
		}
		break;
	case ROTATE_THETA_VERTICAL:
		if(bUseDesiredWp==TRUE) {
			fToolLength = m_fDesiredWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		} else {
			fToolLength = m_fWaterPath[nSide] + m_fProbeLength[nWhich] + m_fArmLength[nWhich];
		}
		if(nSide == 1) fToolLength *= -1.0f;

		CpHead->Side[nSide].fX = CpSurface->Side[nSide].fX + CpSurface->Side[nSide].fI * fToolLength;
		CpHead->Side[nSide].fY = CpSurface->Side[nSide].fY + CpSurface->Side[nSide].fJ * fToolLength;
		CpHead->Side[nSide].fZ = CpSurface->Side[nSide].fZ + CpSurface->Side[nSide].fK * fToolLength;
		CpHead->Side[nSide].norm = CpSurface->Side[nSide].norm;

		if(CpCurrent != NULL) {
			CpHead->Side[nSide].fW = CpCurrent->Side[nSide].fW;
		} else {
			CpHead->Side[nSide].fW = cosf(CpSurface->Side[nSide].fYt * DEG_TO_RAD);
		}

		TipsFromNormals(CpHead,nSide,0.0f);

		CpHead->Side[nSide].fW = cosf(CpHead->Side[nSide].fYt * DEG_TO_RAD);

		//Now lets make sure it's the shortest route
		if(CpCurrent != NULL) {
/*
			if((Signf(CpHead->Side[nSide].fXt) != Signf(CpCurrent->Side[nSide].fXt)) && (Signf(CpHead->Side[nSide].fYt) != Signf(CpCurrent->Side[nSide].fYt))) {
				CpHead->Side[nSide].fXt -= 180.0f;
				CpHead->Side[nSide].fYt *= -1.0f;
				CpHead->Side[nSide].fW = sinf(CpHead->Side[nSide].fYt * DEG_TO_RAD);
				break;
			}

			if(fabs(CpHead->Side[nSide].fXt - CpCurrent->Side[nSide].fXt) >= 180.0f) {
				if(Signf(CpHead->Side[nSide].fXt)==POSITIVE) {
					CpHead->Side[nSide].fXt -= 360.0f;
				} else {
					CpHead->Side[nSide].fXt += 360.0f;
				}
				break;
			}
			*/
		} else {
			if(fabsf(CpHead->Side[nSide].fXt) > 90.0f) {
				if(CpHead->Side[nSide].fXt > 90.0f) {
					CpHead->Side[nSide].fXt -= 180.0f;
				} else {
					CpHead->Side[nSide].fXt += 180.0f;
				}
				CpHead->Side[nSide].fYt -= 180.0f;
				CpHead->Side[nSide].fW = cosf(CpHead->Side[nSide].fYt * DEG_TO_RAD);
			}
		}
		break;
	}
	if(CpCurrent != NULL) {
		CpCurrent->Side[nSide].tip = CpHead->Side[nSide].tip;
		CpCurrent->Side[nSide].norm = CpSurface->Side[nSide].norm;
		CpCurrent->Side[nSide].fW = CpHead->Side[nSide].fW;
	}
	return;
}

void CKinematics::TipsFromNormals(CCoord *pCp,int nSide,float fRotateThetaRevNumber)
{
	float fAngle,fXtAngle,fYtAngle;
	float fTemp;
	D3DXMATRIXA16 matQuatRot;
	D3DXQUATERNION  Quat;
	CCoord Cp;

	switch(theApp.m_LastSettings.nManipulatorType[nSide]) {
	case VERTICAL_WRIST_PPMAC:
		if (pCp->Side[nSide].fK <= 0.0f) {
			pCp->Side[nSide].fYt = (acosf(pCp->Side[nSide].fJ) * RAD_TO_DEG - 90.0f) * -1.0f;
			pCp->Side[nSide].fXt = atan2f(pCp->Side[nSide].fI*-1.0f, pCp->Side[nSide].fK*-1.0f) * RAD_TO_DEG;
			if ((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) pCp->Side[nSide].fXt = 0.0f;
		}
		else {
			if (pCp->Side[nSide].fW >= 0.0f) {													//This has been checked on byfarangi
				pCp->Side[nSide].fYt = acosf(-pCp->Side[nSide].fJ) * RAD_TO_DEG + 90.0f;
				fTemp = atan2f(pCp->Side[nSide].fI*-1.0f, pCp->Side[nSide].fK*-1.0f) * RAD_TO_DEG;
				if ((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) fTemp = 0.0f;
				if (pCp->Side[nSide].fI >= 0.0f) {
					pCp->Side[nSide].fXt = 180.0f + fTemp;
				}
				else {
					pCp->Side[nSide].fXt = fTemp - 180.0f;
				}
			}
			else {
				//This bit needs checking one day
				if (pCp->Side[nSide].fI >= 0.0f) {
					pCp->Side[nSide].fYt = (acosf(pCp->Side[nSide].fK) * RAD_TO_DEG) - 90.0f;
					pCp->Side[nSide].fXt = atan2f(pCp->Side[nSide].fI*-1.0f, pCp->Side[nSide].fJ*-1.0f) * RAD_TO_DEG;
					if ((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) pCp->Side[nSide].fXt = 0.0f;
				}
				else {
					pCp->Side[nSide].fYt = (acosf(pCp->Side[nSide].fK) * RAD_TO_DEG) - 90.0f;
					pCp->Side[nSide].fXt = atan2f(pCp->Side[nSide].fI*-1.0f, pCp->Side[nSide].fJ*-1.0f) * RAD_TO_DEG;
					if ((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) pCp->Side[nSide].fXt = 0.0f;
				}
			}

		}


		break;
	case ROTATE_SWIVEL_GIMBAL:

		Cp.Side[nSide] = pCp->Side[nSide];
		Cp.InvertNorm(nSide);
		Cp.QuaternionFromNormal(nSide,&Quat);
		D3DXMatrixRotationQuaternion(&matQuatRot,&Quat);
	
		fXtAngle = atan2f(matQuatRot._32,matQuatRot._33);
		if((matQuatRot._32 == 0.0f) && (matQuatRot._33 == 0.0f)) fXtAngle = 0.0f;
		fYtAngle = (PIf/2.0f) + -acosf(matQuatRot._31);

		pCp->Side[nSide].fR=0.0f;
		pCp->Side[nSide].fXt=fXtAngle * RAD_TO_DEG;
		pCp->Side[nSide].fYt=fYtAngle * RAD_TO_DEG;

		break;
	case SWIVEL_Z_OFFSET_GIMBAL:
		Cp.Side[nSide] = pCp->Side[nSide];
		Cp.InvertNorm(nSide);

		if (Cp.Side[nSide].fJ < 0.0f) {
			fXtAngle = atan2f(Cp.Side[nSide].fJ, Cp.Side[nSide].fI) + (PIf / 2.0f);
			fYtAngle = acosf(Cp.Side[nSide].fK);
		}
		else {
			fXtAngle = atan2f(Cp.Side[nSide].fJ, Cp.Side[nSide].fI) - (PIf / 2.0f);
			fYtAngle = -acosf(Cp.Side[nSide].fK);
		}
		if (Cp.Side[nSide].fJ == 0.0f && Cp.Side[nSide].fI == 0.0f) {
			fXtAngle = ((float)theApp.m_Axes[3].nPos * theApp.m_Axes[3].fStepSize) * DEG_TO_RAD;
		}

		pCp->Side[nSide].fR = 0.0f;
		pCp->Side[nSide].fXt = fXtAngle * RAD_TO_DEG;
		pCp->Side[nSide].fYt = fYtAngle * RAD_TO_DEG;
		break;
	case HORIZONTAL_WRIST:
		Cp.Side[nSide] = pCp->Side[nSide];
		Cp.InvertNorm(nSide);

		if (Cp.Side[nSide].fJ < 0.0f) {
			fXtAngle = atan2f(Cp.Side[nSide].fJ, Cp.Side[nSide].fI) + (PIf / 2.0f);
			fYtAngle = -asinf(Cp.Side[nSide].fK);
		}
		else {
			fXtAngle = atan2f(Cp.Side[nSide].fJ, Cp.Side[nSide].fI) - (PIf / 2.0f);
			fYtAngle = asinf(Cp.Side[nSide].fK);
		}
		if (Cp.Side[nSide].fJ == 0.0f && Cp.Side[nSide].fI == 0.0f) {
			fXtAngle = ((float)theApp.m_Axes[3].nPos * theApp.m_Axes[3].fStepSize) * DEG_TO_RAD;
		}

		pCp->Side[nSide].fR = 0.0f;
		pCp->Side[nSide].fXt = fXtAngle * RAD_TO_DEG;
		pCp->Side[nSide].fYt = fYtAngle * RAD_TO_DEG;
		break;
	case ARCADIA_2:
		pCp->Side[nSide].fI = 0.01f;
		pCp->Side[nSide].fJ = -0.94f;
		pCp->Side[nSide].fK = -0.1164f;

		if(pCp->Side[nSide].fI <= 0.0f) {
			pCp->Side[nSide].fYt=acosf(pCp->Side[nSide].fK*1.0f) * RAD_TO_DEG - 90.0f;
			pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fJ*-1.0f,pCp->Side[nSide].fI*-1.0f) * RAD_TO_DEG;
			if((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) pCp->Side[nSide].fXt=0.0f;
		} else {
			if(pCp->Side[nSide].fW >= 0.0f) {
				if(pCp->Side[nSide].fK >= 0.0f) {
					pCp->Side[nSide].fYt=acosf(pCp->Side[nSide].fK*1.0f) * RAD_TO_DEG - 90.0f;
					pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fJ*1.0f,pCp->Side[nSide].fI*-1.0f) * -RAD_TO_DEG;
				} else {
					pCp->Side[nSide].fYt=acosf(pCp->Side[nSide].fK*1.0f) * RAD_TO_DEG - 90.0f;
					pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fJ*1.0f,pCp->Side[nSide].fI*-1.0f) * -RAD_TO_DEG;
				}
			} else {
				if(pCp->Side[nSide].fJ >= 0.0f) {
					pCp->Side[nSide].fYt=(acosf(pCp->Side[nSide].fK) * RAD_TO_DEG) - 90.0f;
					pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fJ*-1.0f,pCp->Side[nSide].fI*-1.0f) * RAD_TO_DEG;
					if((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) pCp->Side[nSide].fXt=0.0f;
				} else {
					pCp->Side[nSide].fYt=(acosf(pCp->Side[nSide].fK) * RAD_TO_DEG) -90.0f;
					pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fJ*-1.0f,pCp->Side[nSide].fI*-1.0f) * RAD_TO_DEG;
					if((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) pCp->Side[nSide].fXt=0.0f;
				}
			}

		}

		break;
	case GOODRICH_4:

		if(pCp->Side[nSide].fI <= 0.0f) {
			pCp->Side[nSide].fYt=acosf(pCp->Side[nSide].fK*1.0f) * RAD_TO_DEG - 90.0f;
			pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fJ*1.0f,pCp->Side[nSide].fI*-1.0f) * RAD_TO_DEG;
		} else {
			if(pCp->Side[nSide].fW >= 0.0f) {
				if(pCp->Side[nSide].fK >= 0.0f) {
					pCp->Side[nSide].fYt=-90 - acosf(pCp->Side[nSide].fK) * RAD_TO_DEG;
					pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fJ*1.0f,pCp->Side[nSide].fI*1.0f) * RAD_TO_DEG;
				} else {
					pCp->Side[nSide].fYt=90.0f + (180.0f - acosf(pCp->Side[nSide].fK) * RAD_TO_DEG);
					pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fJ*1.0f,pCp->Side[nSide].fI*1.0f) * RAD_TO_DEG;
				}
			} else {
				if(pCp->Side[nSide].fJ >= 0.0f) {
					pCp->Side[nSide].fYt=(acosf(pCp->Side[nSide].fK) * RAD_TO_DEG) - 90.0f;
					pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fJ*-1.0f,pCp->Side[nSide].fI*-1.0f) * RAD_TO_DEG;
				} else {
					pCp->Side[nSide].fYt=(acosf(pCp->Side[nSide].fK) * RAD_TO_DEG) -90.0f;
					pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fJ*-1.0f,pCp->Side[nSide].fI*-1.0f) * RAD_TO_DEG;
					if((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) pCp->Side[nSide].fXt=0.0f;
				}
			}

		}

		break;
	case SWIVEL_GIMBAL:
	case TWIN_TOWER_FINGER:
	case INDEX_FINGER:
	case INDEX_FINGER_V2:
		if(pCp->Side[nSide].fJ <= 0.0f) {
			pCp->Side[nSide].fYt=acosf(pCp->Side[nSide].fK) * RAD_TO_DEG - 90.0f;
			pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fI*-1.0f,pCp->Side[nSide].fJ*-1.0f) * RAD_TO_DEG;
			if((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) pCp->Side[nSide].fXt=0.0f;
		} else {
			if(pCp->Side[nSide].fW >= 0.0f) {
				if(pCp->Side[nSide].fK >= 0.0f) {
					pCp->Side[nSide].fYt=-90 - acosf(pCp->Side[nSide].fK) * RAD_TO_DEG;
					pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fI*1.0f,pCp->Side[nSide].fJ*1.0f) * RAD_TO_DEG;
				} else {
					pCp->Side[nSide].fYt=90.0f + (180.0f - acosf(pCp->Side[nSide].fK) * RAD_TO_DEG);
					pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fI*1.0f,pCp->Side[nSide].fJ*1.0f) * RAD_TO_DEG;
				}
			} else {
				if(pCp->Side[nSide].fI >= 0.0f) {
					pCp->Side[nSide].fYt=(acosf(pCp->Side[nSide].fK) * RAD_TO_DEG) - 90.0f;
					pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fI*-1.0f,pCp->Side[nSide].fJ*-1.0f) * RAD_TO_DEG;
					if((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) pCp->Side[nSide].fXt=0.0f;
				} else {
					pCp->Side[nSide].fYt=(acosf(pCp->Side[nSide].fK) * RAD_TO_DEG) -90.0f;
					pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fI*-1.0f,pCp->Side[nSide].fJ*-1.0f) * RAD_TO_DEG;
					if((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) pCp->Side[nSide].fXt=0.0f;
				}
			}

		}

		break;
	case TWI_MANIP:
		if(pCp->Side[nSide].fJ >= 0.0f) {
			pCp->Side[nSide].fYt=acosf(pCp->Side[nSide].fK) * RAD_TO_DEG - 90.0f;
			pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fI,pCp->Side[nSide].fJ) * -RAD_TO_DEG;
			if((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) pCp->Side[nSide].fXt=0.0f;
		} else {
			if(pCp->Side[nSide].fW >= 0.0f) {
				if(pCp->Side[nSide].fK >= 0.0f) {
					pCp->Side[nSide].fYt=-90 - acosf(pCp->Side[nSide].fK) * RAD_TO_DEG;
					pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fI*1.0f,pCp->Side[nSide].fJ*1.0f) * RAD_TO_DEG;
				} else {
					pCp->Side[nSide].fYt=90.0f + (180.0f - acosf(pCp->Side[nSide].fK) * RAD_TO_DEG);
					pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fI*-1.0f,pCp->Side[nSide].fJ*-1.0f) * -RAD_TO_DEG;
					if((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) pCp->Side[nSide].fXt=0.0f;
				}
			} else {
				if(pCp->Side[nSide].fI >= 0.0f) {
					pCp->Side[nSide].fYt=(acosf(pCp->Side[nSide].fK) * RAD_TO_DEG) - 90.0f;
					pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fI*-1.0f,pCp->Side[nSide].fJ*-1.0f) * RAD_TO_DEG;
					if((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) pCp->Side[nSide].fXt=0.0f;
				} else {
					pCp->Side[nSide].fYt=(acosf(pCp->Side[nSide].fK) * RAD_TO_DEG) -90.0f;
					pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fI*-1.0f,pCp->Side[nSide].fJ*-1.0f) * RAD_TO_DEG;
					if((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) pCp->Side[nSide].fXt=0.0f;
				}
			}

		}

		break;
	case DOWTY_B_SCAN:
	case VERTICAL_WRIST:
	case VERTICAL_WRIST_90:
		if(pCp->Side[nSide].fK <= 0.0f) {
			pCp->Side[nSide].fYt=acosf(pCp->Side[nSide].fJ) * RAD_TO_DEG - 90.0f;
			pCp->Side[nSide].fXt=atan2f(pCp->Side[nSide].fI*-1.0f,pCp->Side[nSide].fK*-1.0f) * RAD_TO_DEG;
			if((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) pCp->Side[nSide].fXt=0.0f;
		} else {
			pCp->Side[nSide].fYt=90.0f - acosf(-pCp->Side[nSide].fJ) * RAD_TO_DEG;
			fTemp=atan2f(pCp->Side[nSide].fI,pCp->Side[nSide].fK) * RAD_TO_DEG;
			if((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fJ == 0.0f)) fTemp=0.0f;
			if(pCp->Side[nSide].fI >= 0.0f) {
				pCp->Side[nSide].fXt = -180.0f + fTemp;
			} else {
				pCp->Side[nSide].fXt = 180.0f + fTemp;
			}
		}
	break;
	case FOREPLANE_VERTICAL_WRIST:
		pCp->Side[nSide].fXt=(float)asin(pCp->Side[nSide].fI) * RAD_TO_DEG * -1.0f;
		if(pCp->Side[nSide].fK<0.0f) {
			pCp->Side[nSide].fYt =(float)acos(pCp->Side[nSide].fJ) * RAD_TO_DEG -90.0f;
		} else {
			fAngle = (float)asin(pCp->Side[nSide].fJ) * RAD_TO_DEG * -1.0f;
			if(pCp->Side[nSide].fJ<0.0f) {
				pCp->Side[nSide].fYt = fAngle;
			} else {
				pCp->Side[nSide].fYt = fAngle;
			}
		}
		if(m_nInvertXtip) pCp->Side[nSide].fXt *= -1.0f;
		if(m_nInvertYtip) pCp->Side[nSide].fYt *= -1.0f;
		break;
	case ROTATE_DIF_THETA:
		if(pCp->Side[nSide].fW >= 0.0f) {
			pCp->Side[nSide].fXt=atan2f((pCp->Side[nSide].fI * -1.0f) , (pCp->Side[nSide].fJ*-1.0f)) * RAD_TO_DEG;
		} else {
			pCp->Side[nSide].fXt=atan2f((pCp->Side[nSide].fI * 1.0f) , (pCp->Side[nSide].fJ*1.0f)) * RAD_TO_DEG;
		}

		if(fRotateThetaRevNumber >= 0.5f) pCp->Side[nSide].fXt += 360.0f;
		if(fRotateThetaRevNumber <= -0.5f) pCp->Side[nSide].fXt -= 360.0f;

		pCp->Side[nSide].fYt=atan2f(sqrtf((pCp->Side[nSide].fJ*pCp->Side[nSide].fJ) + pCp->Side[nSide].fI*pCp->Side[nSide].fI),pCp->Side[nSide].fK) * RAD_TO_DEG;
		if(pCp->Side[nSide].fW < 0) pCp->Side[nSide].fYt *= -1.0f;

		break;
	case ROTATE_THETA_HORIZONTAL:
		if(theApp.m_LastSettings.nRThetaUseW) {
			if(pCp->Side[nSide].fW >= 0.0f) {
				pCp->Side[nSide].fXt=atan2f((pCp->Side[nSide].fI * -1.0f) , (pCp->Side[nSide].fK*-1.0f)) * RAD_TO_DEG;
			} else {
				pCp->Side[nSide].fXt=atan2f((pCp->Side[nSide].fI * 1.0f) , (pCp->Side[nSide].fK*1.0f)) * RAD_TO_DEG;
			}
		} else {
			pCp->Side[nSide].fXt=atan2f((pCp->Side[nSide].fI * -1.0f) , (pCp->Side[nSide].fK*-1.0f)) * RAD_TO_DEG;
		}

		if((pCp->Side[nSide].fI == 0.0f) && (pCp->Side[nSide].fK == 0.0f)) pCp->Side[nSide].fXt=0.0f;


		if(fRotateThetaRevNumber >= 0.5f) pCp->Side[nSide].fXt += 360.0f;
		if(fRotateThetaRevNumber <= -0.5f) pCp->Side[nSide].fXt -= 360.0f;

		pCp->Side[nSide].fYt=atan2f(sqrtf((pCp->Side[nSide].fK*pCp->Side[nSide].fK) + pCp->Side[nSide].fI*pCp->Side[nSide].fI) , pCp->Side[nSide].fJ * -1.0f) * RAD_TO_DEG;
		if(pCp->Side[nSide].fW < 0) pCp->Side[nSide].fYt *= -1.0f;

		break;
	case ROTATE_THETA_VERTICAL:
		pCp->Side[nSide].fXt=atan2f((pCp->Side[nSide].fI * -1.0f) , (pCp->Side[nSide].fJ*-1.0f)) * -RAD_TO_DEG;
		if(pCp->Side[nSide].fW < 0.0f) {
			pCp->Side[nSide].fXt += 180.0f;
		}

		if(fRotateThetaRevNumber >= 0.5f) pCp->Side[nSide].fXt += 360.0f;
		if(fRotateThetaRevNumber <= -0.5f) pCp->Side[nSide].fXt -= 360.0f;

		if(pCp->Side[nSide].fW >= 0.0f) {
			pCp->Side[nSide].fYt=asinf(pCp->Side[nSide].fK) * -RAD_TO_DEG;
		} else {
			pCp->Side[nSide].fYt=180.0f - asinf(pCp->Side[nSide].fK) * -RAD_TO_DEG;
		}

		break;
	}
}


void CKinematics::NormalsFromTips(CCoord *pCp,int nSide)
{
	float fXtip = pCp->Side[nSide].fXt;
	float fYtip = pCp->Side[nSide].fYt;
	float fXtipRad = pCp->Side[nSide].fXt * DEG_TO_RAD;
	float fYtipRad = pCp->Side[nSide].fYt * DEG_TO_RAD;
	D3DXMATRIX matNormal,matGimbal,matSwivel;
	D3DXMATRIX matRotate;
	D3DXMATRIX matTheta;
	D3DXVECTOR3 vecNormal;
	D3DXVECTOR3 vecP;

	float fRadius;

	switch(theApp.m_LastSettings.nManipulatorType[nSide]) {
	case VERTICAL_WRIST_PPMAC:
		pCp->Side[nSide].fJ = cosf((pCp->Side[nSide].fYt + 90.0f) * DEG_TO_RAD) * -1.0f;
		fRadius = cosf(pCp->Side[nSide].fYt * DEG_TO_RAD);
		pCp->Side[nSide].fI = fRadius * cosf((pCp->Side[nSide].fXt + 90.0f) * DEG_TO_RAD);
		pCp->Side[nSide].fK = fRadius * sinf((pCp->Side[nSide].fXt + 90.0f) * DEG_TO_RAD) * -1.0f;
		pCp->Side[nSide].fW = cosf(fXtipRad);
		break;
	case SWIVEL_Z_OFFSET_GIMBAL:
		D3DXMatrixRotationZ(&matSwivel, pCp->Side[nSide].fXt * DEG_TO_RAD);
		D3DXMatrixRotationX(&matGimbal, pCp->Side[nSide].fYt * DEG_TO_RAD);
		D3DXMatrixMultiply(&matNormal, &matGimbal, &matSwivel);
		pCp->Side[nSide].fI = -matNormal._31;
		pCp->Side[nSide].fJ = -matNormal._32;
		pCp->Side[nSide].fK = -matNormal._33;
		pCp->Side[nSide].fW = 0.0f;
		break;
	case HORIZONTAL_WRIST:
		D3DXMatrixRotationZ(&matSwivel, pCp->Side[nSide].fXt * DEG_TO_RAD);
		D3DXMatrixRotationX(&matGimbal, pCp->Side[nSide].fYt * DEG_TO_RAD);
		D3DXMatrixMultiply(&matNormal, &matGimbal, &matSwivel);
		pCp->Side[nSide].fI = matNormal._21;
		pCp->Side[nSide].fJ = matNormal._22;
		pCp->Side[nSide].fK = matNormal._23;
		pCp->Side[nSide].fW = 0.0f;
		break;

	case ROTATE_SWIVEL_GIMBAL:
		D3DXMatrixRotationZ(&matRotate,pCp->Side[nSide].fR * DEG_TO_RAD);
		D3DXMatrixRotationX(&matSwivel,-pCp->Side[nSide].fXt * DEG_TO_RAD);
		D3DXMatrixRotationY(&matGimbal,pCp->Side[nSide].fYt * DEG_TO_RAD);

		vecP = D3DXVECTOR3(0.0f,0.0f,-1.0f);
		D3DXVec3TransformCoord(&vecP,&vecP,&matGimbal);
		D3DXVec3TransformCoord(&vecP,&vecP,&matSwivel);
		D3DXVec3TransformCoord(&vecP,&vecP,&matRotate);
		pCp->Side[nSide].fI = vecP.x;
		pCp->Side[nSide].fJ = vecP.y;
		pCp->Side[nSide].fK = vecP.z;
		break;
	case SWIVEL_GIMBAL:
	case TWIN_TOWER_FINGER:
	case INDEX_FINGER:
	case INDEX_FINGER_V2:

		pCp->Side[nSide].fK = cosf((pCp->Side[nSide].fYt+90.0f) * DEG_TO_RAD);
		fRadius = cosf(pCp->Side[nSide].fYt * DEG_TO_RAD);
		pCp->Side[nSide].fI = fRadius * cosf((pCp->Side[nSide].fXt+90.0f) * DEG_TO_RAD);
		pCp->Side[nSide].fJ = fRadius * sinf((pCp->Side[nSide].fXt+90.0f) * DEG_TO_RAD) * -1.0f;
		pCp->Side[nSide].fW = cosf(fXtipRad);
		break;
	case TWI_MANIP:

		pCp->Side[nSide].fK = cosf((pCp->Side[nSide].fYt+90.0f) * DEG_TO_RAD);
		fRadius = cosf(pCp->Side[nSide].fYt * DEG_TO_RAD);
		pCp->Side[nSide].fI = fRadius * cosf((pCp->Side[nSide].fXt+90.0f) * DEG_TO_RAD);
		pCp->Side[nSide].fJ = fRadius * sinf((pCp->Side[nSide].fXt+90.0f) * DEG_TO_RAD);
		pCp->Side[nSide].fW = cosf(fXtipRad);
		break;
	case VERTICAL_WRIST:
	case VERTICAL_WRIST_90:
	case DOWTY_B_SCAN:
		/*
		pCp->Side[nSide].fI = (float)sin(pCp->Side[nSide].fXt * DEG_TO_RAD) * -1.0f;
		pCp->Side[nSide].fJ = (float)cos((pCp->Side[nSide].fYt+90.0f) * DEG_TO_RAD);
		pCp->Side[nSide].fK = (float)sqrt(1.0-(pCp->Side[nSide].fI*pCp->Side[nSide].fI) - (pCp->Side[nSide].fJ*pCp->Side[nSide].fJ));
		if((pCp->Side[nSide].fYt < 90.0f) && (pCp->Side[nSide].fYt> -90.0f)) {
			pCp->Side[nSide].fK;
		} else {
			pCp->Side[nSide].fK = pCp->Side[nSide].fK - 1.0f;
		}
*/
		pCp->Side[nSide].fJ = cosf((pCp->Side[nSide].fYt+90.0f) * DEG_TO_RAD);
		fRadius = cosf(pCp->Side[nSide].fYt * DEG_TO_RAD);
		pCp->Side[nSide].fI = fRadius * cosf((pCp->Side[nSide].fXt+90.0f) * DEG_TO_RAD);
		pCp->Side[nSide].fK = fRadius * sinf((pCp->Side[nSide].fXt+90.0f) * DEG_TO_RAD) * -1.0f;
		pCp->Side[nSide].fW = 0.0f;
		break;
	case FOREPLANE_VERTICAL_WRIST:
		if(m_nInvertXtip) fXtip *= -1.0f;
		if(m_nInvertYtip) fYtip *= -1.0f;

		pCp->Side[nSide].fI = (float)sin(fXtip * DEG_TO_RAD) * -1.0f;
		pCp->Side[nSide].fJ = (float)cos((fYtip+90.0f) * DEG_TO_RAD);
		pCp->Side[nSide].fK = (float)sqrt(1.0-(pCp->Side[nSide].fI*pCp->Side[nSide].fI) - (pCp->Side[nSide].fJ*pCp->Side[nSide].fJ));
		if((fYtip < 90.0f) && (fYtip> -90.0f)) {
			pCp->Side[nSide].fK;
		} else {
			pCp->Side[nSide].fK = pCp->Side[nSide].fK - 1.0f;
		}
		break;
	case GOODRICH_4:
		pCp->Side[nSide].fK = cosf((pCp->Side[nSide].fYt+90.0f) * DEG_TO_RAD) * 1.0f;
		fRadius = cosf(pCp->Side[nSide].fYt * DEG_TO_RAD);
		pCp->Side[nSide].fI = fRadius * sinf((pCp->Side[nSide].fXt+90.0f) * DEG_TO_RAD) * -1.0f;
		pCp->Side[nSide].fJ = fRadius * cosf((pCp->Side[nSide].fXt+90.0f) * DEG_TO_RAD) * -1.0f;
		pCp->Side[nSide].fW = cosf(fXtipRad);
		break;
	case ARCADIA_2:
		pCp->Side[nSide].fK = cosf((pCp->Side[nSide].fYt+90.0f) * DEG_TO_RAD) * 1.0f;
		fRadius = cosf(pCp->Side[nSide].fYt * DEG_TO_RAD);
		pCp->Side[nSide].fI = fRadius * sinf((pCp->Side[nSide].fXt+90.0f) * DEG_TO_RAD) * -1.0f;
		pCp->Side[nSide].fJ = fRadius * cosf((pCp->Side[nSide].fXt+90.0f) * DEG_TO_RAD);
		pCp->Side[nSide].fW = cosf(fXtipRad);
		break;
	case ROTATE_DIF_THETA:
		D3DXMatrixRotationX(&matTheta,pCp->Side[nSide].fYt * DEG_TO_RAD);
		D3DXMatrixRotationZ(&matRotate,pCp->Side[nSide].fXt * -DEG_TO_RAD);
		D3DXMatrixMultiply(&matNormal,&matTheta,&matRotate);

		vecP = D3DXVECTOR3(0.0f,0.0f,1.0f);
		D3DXVec3TransformCoord(&vecP,&vecP,&matNormal);
		pCp->Side[nSide].norm.x = vecP.x;
		pCp->Side[nSide].norm.y = vecP.y;
		pCp->Side[nSide].norm.z = vecP.z;
		pCp->Side[nSide].fW = sinf(pCp->Side[nSide].fYt * DEG_TO_RAD);
//		pCp->Side[nSide].fW = theApp.m_fRotateThetaRevNumber[nSide];
		break;
	case ROTATE_THETA_HORIZONTAL:
		D3DXMatrixRotationX(&matTheta,pCp->Side[nSide].fYt * DEG_TO_RAD);
		D3DXMatrixRotationY(&matRotate,pCp->Side[nSide].fXt * DEG_TO_RAD);
		D3DXMatrixMultiply(&matNormal,&matTheta,&matRotate);

		vecP = D3DXVECTOR3(0.0f,-1.0f,0.0f);
		D3DXVec3TransformCoord(&vecP,&vecP,&matNormal);
		pCp->Side[nSide].norm.x = vecP.x;
		pCp->Side[nSide].norm.y = vecP.y;
		pCp->Side[nSide].norm.z = vecP.z;
		pCp->Side[nSide].fW = sinf(pCp->Side[nSide].fYt * DEG_TO_RAD);
//		pCp->Side[nSide].fW = theApp.m_fRotateThetaRevNumber[nSide];
		break;
	case ROTATE_THETA_VERTICAL:
		D3DXMatrixRotationX(&matTheta,pCp->Side[nSide].fYt * DEG_TO_RAD);
		D3DXMatrixRotationZ(&matRotate,pCp->Side[nSide].fXt * DEG_TO_RAD);
		D3DXMatrixMultiply(&matNormal,&matTheta,&matRotate);

		vecP = D3DXVECTOR3(0.0f,-1.0f,0.0f);
		D3DXVec3TransformCoord(&vecP,&vecP,&matNormal);
		pCp->Side[nSide].norm.x = vecP.x;
		pCp->Side[nSide].norm.y = vecP.y;
		pCp->Side[nSide].norm.z = vecP.z;
		pCp->Side[nSide].fW = cosf(pCp->Side[nSide].fYt * DEG_TO_RAD);
//		pCp->Side[nSide].fW = theApp.m_fRotateThetaRevNumber[nSide];
		break;
	}
}

void CKinematics::CalculateOppositeCoord(int nDestSide,CCoord *pCp,float fThickness)
{
	int	nSrcSide;

	if(nDestSide == PORTSIDE) {
		nSrcSide = STARBOARD;
		pCp->Side[nDestSide].fX=pCp->Side[nSrcSide].fX - (fThickness*pCp->Side[nSrcSide].fI); 
		pCp->Side[nDestSide].fY=pCp->Side[nSrcSide].fY - (fThickness*pCp->Side[nSrcSide].fJ); 
		pCp->Side[nDestSide].fZ=pCp->Side[nSrcSide].fZ - (fThickness*pCp->Side[nSrcSide].fK); 
	} else {
		nSrcSide = PORTSIDE;
		pCp->Side[nDestSide].fX=pCp->Side[nSrcSide].fX - (fThickness*pCp->Side[nSrcSide].fI); 
		pCp->Side[nDestSide].fY=pCp->Side[nSrcSide].fY - (fThickness*pCp->Side[nSrcSide].fJ); 
		pCp->Side[nDestSide].fZ=pCp->Side[nSrcSide].fZ - (fThickness*pCp->Side[nSrcSide].fK); 
	}

	pCp->Side[nDestSide].fI = -pCp->Side[nSrcSide].fI;
	pCp->Side[nDestSide].fJ = -pCp->Side[nSrcSide].fJ;
	pCp->Side[nDestSide].fK = -pCp->Side[nSrcSide].fK;
	pCp->Side[nDestSide].fW = -pCp->Side[nSrcSide].fW;

	pCp->Side[nDestSide].fXt=pCp->Side[nSrcSide].fXt;
	pCp->Side[nDestSide].fYt=pCp->Side[nSrcSide].fYt;
}

bool CKinematics::InitializeRhomboid()
{
	float dX,dY,dZ;
	int	nX,nY,nZ;

	if(CalculateIntegerVariables()==FALSE) return FALSE;

	for(nX=0,dX=m_dXMin;nX<=m_nXpts;nX++,dX+=m_dXStepSize) {
		for(nY=0,dY=m_dYMin;nY<=m_nYpts;nY++,dY+=m_dYStepSize) {
			for(nZ=0,dZ=m_dZMin;nZ<=m_nZpts;nZ++,dZ+=m_dZStepSize) {
				
				memset(&m_Rhomboid[nX][nY][nZ],0,sizeof RhomboidStruct);
				m_Rhomboid[nX][nY][nZ].nStructSize = sizeof RhomboidStruct;
				m_Rhomboid[nX][nY][nZ].dXp = dX;
				m_Rhomboid[nX][nY][nZ].dYp = dY;
				m_Rhomboid[nX][nY][nZ].dZp = dZ;
				m_Rhomboid[nX][nY][nZ].nXp = (int)(dX / theApp.m_Axes[X0AXIS].fStepSize);
				m_Rhomboid[nX][nY][nZ].nYp = (int)(dY / theApp.m_Axes[Y0AXIS].fStepSize);
				m_Rhomboid[nX][nY][nZ].nZp = (int)(dZ / theApp.m_Axes[Z0AXIS].fStepSize);
			}
		}
	}
	return TRUE;
}

bool CKinematics::CalculateIntegerVariables()
{
	int	nX,nY,nZ;

	m_nXStepSize = (int)(m_dXStepSize / theApp.m_Axes[X0AXIS].fStepSize);
	m_nYStepSize = (int)(m_dYStepSize / theApp.m_Axes[Y0AXIS].fStepSize);
	m_nZStepSize = (int)(m_dZStepSize / theApp.m_Axes[Z0AXIS].fStepSize);
	m_nXMin = (int)(m_dXMin /  theApp.m_Axes[X0AXIS].fStepSize);
	m_nYMin = (int)(m_dYMin /  theApp.m_Axes[Y0AXIS].fStepSize);
	m_nZMin = (int)(m_dZMin /  theApp.m_Axes[Z0AXIS].fStepSize);
	m_nXMax = (int)(m_dXMax /  theApp.m_Axes[X0AXIS].fStepSize);
	m_nYMax = (int)(m_dYMax /  theApp.m_Axes[Y0AXIS].fStepSize);
	m_nZMax = (int)(m_dZMax /  theApp.m_Axes[Z0AXIS].fStepSize);
	m_nXpts = (int)((m_dXMax - m_dXMin) / m_dXStepSize) + 1;
	m_nYpts = (int)((m_dYMax - m_dYMin) / m_dYStepSize) + 1;
	m_nZpts = (int)((m_dZMax - m_dZMin) / m_dZStepSize) + 1;

	if(m_nXpts>=XPTS) return FALSE;
	if(m_nYpts>=YPTS) return FALSE;
	if(m_nZpts>=ZPTS) return FALSE;

	for(nX=0;nX<=m_nXpts;nX++) {
		for(nY=0;nY<=m_nYpts;nY++) {
			for(nZ=0;nZ<=m_nZpts;nZ++) {
				m_Rhomboid[nX][nY][nZ].nXp = (int)(m_Rhomboid[nX][nY][nZ].dXp / theApp.m_Axes[X0AXIS].fStepSize);
				m_Rhomboid[nX][nY][nZ].nYp = (int)(m_Rhomboid[nX][nY][nZ].dYp / theApp.m_Axes[Y0AXIS].fStepSize);
				m_Rhomboid[nX][nY][nZ].nZp = (int)(m_Rhomboid[nX][nY][nZ].dZp / theApp.m_Axes[Z0AXIS].fStepSize);
				m_Rhomboid[nX][nY][nZ].nDeltaX = (int)(m_Rhomboid[nX][nY][nZ].dDeltaX / theApp.m_Axes[X1AXIS].fStepSize);
				m_Rhomboid[nX][nY][nZ].nDeltaY = (int)(m_Rhomboid[nX][nY][nZ].dDeltaY / theApp.m_Axes[Y1AXIS].fStepSize);
				m_Rhomboid[nX][nY][nZ].nDeltaZ = (int)(m_Rhomboid[nX][nY][nZ].dDeltaZ / theApp.m_Axes[Z1AXIS].fStepSize);
			}
		}
	}

	m_nXTipStep = (int)(m_dXTipStep / theApp.m_Axes[XT0AXIS].fStepSize);
	m_nYTipStep = (int)(m_dYTipStep / theApp.m_Axes[YT0AXIS].fStepSize);
	m_nXTipMinMax = (int)(m_dXTipMinMax /  theApp.m_Axes[XT0AXIS].fStepSize);
	m_nYTipMinMax = (int)(m_dYTipMinMax /  theApp.m_Axes[YT0AXIS].fStepSize);
	m_nXTipPts = (int)((2.0 * m_dXTipMinMax) / m_dXTipStep) + 1;
	m_nYTipPts = (int)((2.0 * m_dYTipMinMax) / m_dYTipStep) + 1;

	if(m_nXTipPts>=XTPTS) return FALSE;
	if(m_nYTipPts>=YTPTS) return FALSE;

	for(nX=0;nX<=m_nXTipPts;nX++) {
		for(nY=0;nY<=m_nYTipPts;nY++) {
			m_Tip[nX][nY].nXtp = (int)(m_Tip[nX][nY].dXtp / theApp.m_Axes[XT0AXIS].fStepSize);
			m_Tip[nX][nY].nYtp = (int)(m_Tip[nX][nY].dYtp / theApp.m_Axes[YT0AXIS].fStepSize);
			m_Tip[nX][nY].nDeltaXt = (int)(m_Tip[nX][nY].dDeltaXt / theApp.m_Axes[XT1AXIS].fStepSize);
			m_Tip[nX][nY].nDeltaYt = (int)(m_Tip[nX][nY].dDeltaYt / theApp.m_Axes[YT1AXIS].fStepSize);
		}
	}

	return TRUE;
}

void CKinematics::ClearAllActions()
{
	int	nX,nY,nZ;

	for(nX=0;nX<XPTS;nX++) {
		for(nY=0;nY<YPTS;nY++) {
			for(nZ=0;nZ<ZPTS;nZ++) {
				m_Rhomboid[nX][nY][nZ].nAction = 0;

			}
		}
	}

	for(nX=0;nX<XTPTS;nX++) {
		for(nY=0;nY<YTPTS;nY++) {
			m_Tip[nX][nY].nAction = 0;
		}
	}

}


void CKinematics::SetAllActions()
{
	int	nX,nY,nZ;

	ClearAllActions();

	for(nX=0;nX<m_nXpts;nX++) {
		for(nY=0;nY<m_nYpts;nY++) {
			for(nZ=0;nZ<m_nZpts;nZ++) {
				m_Rhomboid[nX][nY][nZ].nAction = 1;

			}
		}
	}

	for(nX=0;nX<XTPTS;nX++) {
		for(nY=0;nY<YTPTS;nY++) {
			m_Tip[nX][nY].nAction = 1;
		}
	}

}

void CKinematics::ModifyDeltas(int nX, int nY, int nZ, CCoord CpHead)
{
	m_Rhomboid[nX][nY][nZ].dDeltaX = m_Rhomboid[nX][nY][nZ].dXp-CpHead.Side1.fX;
	m_Rhomboid[nX][nY][nZ].dDeltaY = m_Rhomboid[nX][nY][nZ].dYp-CpHead.Side1.fY;
	m_Rhomboid[nX][nY][nZ].dDeltaZ = m_Rhomboid[nX][nY][nZ].dZp-CpHead.Side1.fZ;

	m_Rhomboid[nX][nY][nZ].nDeltaX = (int)(m_Rhomboid[nX][nY][nZ].dDeltaX / theApp.m_Axes[X1AXIS].fStepSize);
	m_Rhomboid[nX][nY][nZ].nDeltaY = (int)(m_Rhomboid[nX][nY][nZ].dDeltaY / theApp.m_Axes[Y1AXIS].fStepSize);
	m_Rhomboid[nX][nY][nZ].nDeltaZ = (int)(m_Rhomboid[nX][nY][nZ].dDeltaZ / theApp.m_Axes[Z1AXIS].fStepSize);

	m_Rhomboid[nX][nY][nZ].nAmplitude = (theApp.m_UtUser.m_TS[theApp.m_UtUser.m_Global.nTimeSlot].Gate.nAmplitude[0]*100)/127;
}



void CKinematics::OldCalculateRealCoord(CCoord *pCpHead)
{
	CCoord Cp = *pCpHead;
	int	nX0,nX1,nY0,nY1,nZ0,nZ1;
	int nXp = (int)(Cp.Side1.fX / theApp.m_Axes[X1AXIS].fStepSize);
	int nYp = (int)(Cp.Side1.fY / theApp.m_Axes[Y1AXIS].fStepSize);
	int nZp = (int)(Cp.Side1.fZ / theApp.m_Axes[Z1AXIS].fStepSize);

	nX0 = MulDiv(nXp-m_nXMin,m_nXpts-1,m_nXMax-m_nXMin);
	if(nX0<0) nX0=0;
	if(nX0>=(m_nXpts-1)) nX0 = m_nXpts-2;
	nX1 = nX0 + 1;

	nY0 = MulDiv(nYp-m_nYMin,m_nYpts-1,m_nYMax-m_nYMin);
	if(nY0<0) nY0=0;
	if(nY0>=(m_nYpts-1)) nY0 = m_nYpts-2;
	nY1 = nY0 + 1;

	nZ0 = MulDiv(nZp-m_nZMin,m_nZpts-1,m_nZMax-m_nZMin);
	if(nZ0<0) nZ0=0;
	if(nZ0>=(m_nZpts-1)) nZ0 = m_nZpts-2;
	nZ1 = nZ0 + 1;


	RhomboidStruct* pRhom[8];
	pRhom[_FLL] = (RhomboidStruct*) &m_Rhomboid[nX0][nY0][nZ0];
	pRhom[_FLR] = (RhomboidStruct*) &m_Rhomboid[nX1][nY0][nZ0];
	pRhom[_FUL] = (RhomboidStruct*) &m_Rhomboid[nX0][nY0][nZ1];
	pRhom[_FUR] = (RhomboidStruct*) &m_Rhomboid[nX1][nY0][nZ1];
	pRhom[_BLL] = (RhomboidStruct*) &m_Rhomboid[nX0][nY1][nZ0];
	pRhom[_BLR] = (RhomboidStruct*) &m_Rhomboid[nX1][nY1][nZ0];
	pRhom[_BUL] = (RhomboidStruct*) &m_Rhomboid[nX0][nY1][nZ1];
	pRhom[_BUR] = (RhomboidStruct*) &m_Rhomboid[nX1][nY1][nZ1];

	m_pFrontLowerLeft = pRhom[_FLL];

	//X error at bottom front of box
	int nXbf = MulDiv(nXp - pRhom[_FLL]->nXp, pRhom[_FLR]->nDeltaX-pRhom[_FLL]->nDeltaX, m_nXStepSize) + pRhom[_FLL]->nDeltaX;
	//X error at top front of box
	int nXtf = MulDiv(nXp - pRhom[_FUL]->nXp, pRhom[_FUR]->nDeltaX-pRhom[_FUL]->nDeltaX, m_nXStepSize) + pRhom[_FUL]->nDeltaX;
	//x error at front
	int	nXf = MulDiv(nZp - pRhom[_FLL]->nZp, nXtf-nXbf, m_nZStepSize) + nXbf;
	//X error at bottom back of box
	int nXbb = MulDiv(nXp - pRhom[_BLL]->nXp, pRhom[_BLR]->nDeltaX-pRhom[_BLL]->nDeltaX, m_nXStepSize) + pRhom[_BLL]->nDeltaX;
	//X error at top back of box
	int nXtb = MulDiv(nXp - pRhom[_BUL]->nXp, pRhom[_BUR]->nDeltaX-pRhom[_BUL]->nDeltaX, m_nXStepSize) + pRhom[_BUL]->nDeltaX;
	//x error at back
	int	nXb = MulDiv(nZp - pRhom[_BLL]->nZp, nXtb-nXbb, m_nZStepSize) + nXbb;
	//x error in y axis
	m_nXError = MulDiv(nYp - pRhom[_FLL]->nYp, nXb-nXf, m_nYStepSize) + nXf;
	m_dXError = (float)m_nXError * theApp.m_Axes[X1AXIS].fStepSize;

	//Y Error bottom Left
	int nYbl = MulDiv(nYp - pRhom[_FLL]->nYp, pRhom[_BLL]->nDeltaY-pRhom[_FLL]->nDeltaY, m_nYStepSize) + pRhom[_FLL]->nDeltaY;
	//Y Error bottom Right
	int nYbr = MulDiv(nYp - pRhom[_FLR]->nYp, pRhom[_BLR]->nDeltaY-pRhom[_FLR]->nDeltaY, m_nYStepSize) + pRhom[_FLR]->nDeltaY;
	//Y error at bottom of box
	int nYl = MulDiv(nXp - pRhom[_FLL]->nXp, nYbr-nYbl, m_nXStepSize) + nYbl;
	//Y Error upper Left
	int nYul = MulDiv(nYp - pRhom[_FUL]->nYp, pRhom[_BUL]->nDeltaY-pRhom[_FUL]->nDeltaY, m_nYStepSize) + pRhom[_FUL]->nDeltaY;
	//Y Error upper Right
	int nYur = MulDiv(nYp - pRhom[_FUR]->nYp, pRhom[_BUR]->nDeltaY-pRhom[_FUR]->nDeltaY, m_nYStepSize) + pRhom[_FUR]->nDeltaY;
	//Y error at upper of box
	int nYu = MulDiv(nXp - pRhom[_FUL]->nXp, nYur-nYul, m_nXStepSize) + nYul;
	//y Error in Z
	m_nYError = MulDiv(nZp - pRhom[_FLL]->nZp, nYu-nYl, m_nZStepSize) + nYl;
	m_dYError = (float)m_nYError * theApp.m_Axes[Y1AXIS].fStepSize;

	//Z error at front left of box
	int nZlf = MulDiv(nZp - pRhom[_FLL]->nZp, pRhom[_FUL]->nDeltaZ-pRhom[_FLL]->nDeltaZ, m_nZStepSize) + pRhom[_FLL]->nDeltaZ;
	//Z error at front right of box
	int nZrf = MulDiv(nZp - pRhom[_FLR]->nZp, pRhom[_FUR]->nDeltaZ-pRhom[_FLR]->nDeltaZ, m_nZStepSize) + pRhom[_FLR]->nDeltaZ;
	//Z error at front
	int	nZf = MulDiv(nXp - pRhom[_FLL]->nXp, nZrf-nZlf, m_nXStepSize) + nZlf;
	//Z error at back left of box
	int nZlb = MulDiv(nZp - pRhom[_BLL]->nZp, pRhom[_BUL]->nDeltaZ-pRhom[_BLL]->nDeltaZ, m_nZStepSize) + pRhom[_BLL]->nDeltaZ;
	//Z error at back right of box
	int nZrb = MulDiv(nZp - pRhom[_BLR]->nZp, pRhom[_BUR]->nDeltaZ-pRhom[_BLR]->nDeltaZ, m_nZStepSize) + pRhom[_BLR]->nDeltaZ;
	//Z error at back
	int	nZb = MulDiv(nXp - pRhom[_BLL]->nXp, nZrb-nZlb, m_nXStepSize) + nZlb;

	//Z error in y axis
	m_nZError = MulDiv(nYp - pRhom[_FLL]->nYp, nZb-nZf, m_nYStepSize) + nZf;
	m_dZError = (float)m_nZError * theApp.m_Axes[Z1AXIS].fStepSize;

	pCpHead->Side1.fX -= m_dXError;
	pCpHead->Side1.fY -= m_dYError;
	pCpHead->Side1.fZ -= m_dZError;

}

void CKinematics::CalculateRealCoord(CCoord *pCpHead)
{
	CCoord Cp = *pCpHead;
	int	nX0,nX1,nY0,nY1,nZ0,nZ1;
	int nXp = (int)(Cp.Side1.fX / theApp.m_Axes[X1AXIS].fStepSize);
	int nYp = (int)(Cp.Side1.fY / theApp.m_Axes[Y1AXIS].fStepSize);
	int nZp = (int)(Cp.Side1.fZ / theApp.m_Axes[Z1AXIS].fStepSize);

	nX0 = MulDiv(nXp-m_nXMin,m_nXpts-1,m_nXMax-m_nXMin);
	if(nX0<0) nX0=0;
	if(nX0>=(m_nXpts-1)) nX0 = m_nXpts-2;
	nX1 = nX0 + 1;

	nY0 = MulDiv(nYp-m_nYMin,m_nYpts-1,m_nYMax-m_nYMin);
	if(nY0<0) nY0=0;
	if(nY0>=(m_nYpts-1)) nY0 = m_nYpts-2;
	nY1 = nY0 + 1;

	nZ0 = MulDiv(nZp-m_nZMin,m_nZpts-1,m_nZMax-m_nZMin);
	if(nZ0<0) nZ0=0;
	if(nZ0>=(m_nZpts-1)) nZ0 = m_nZpts-2;
	nZ1 = nZ0 + 1;


	RhomboidStruct* pRhom[8];
	pRhom[_FLL] = (RhomboidStruct*) &m_Rhomboid[nX0][nY0][nZ0];
	pRhom[_FLR] = (RhomboidStruct*) &m_Rhomboid[nX1][nY0][nZ0];
	pRhom[_FUL] = (RhomboidStruct*) &m_Rhomboid[nX0][nY0][nZ1];
	pRhom[_FUR] = (RhomboidStruct*) &m_Rhomboid[nX1][nY0][nZ1];
	pRhom[_BLL] = (RhomboidStruct*) &m_Rhomboid[nX0][nY1][nZ0];
	pRhom[_BLR] = (RhomboidStruct*) &m_Rhomboid[nX1][nY1][nZ0];
	pRhom[_BUL] = (RhomboidStruct*) &m_Rhomboid[nX0][nY1][nZ1];
	pRhom[_BUR] = (RhomboidStruct*) &m_Rhomboid[nX1][nY1][nZ1];

	m_pFrontLowerLeft = pRhom[_FLL];

	int nXbf = MulDiv(nXp - pRhom[_FLL]->nXp, pRhom[_FLR]->nDeltaX-pRhom[_FLL]->nDeltaX, m_nXStepSize) + pRhom[_FLL]->nDeltaX;
	int nXtf = MulDiv(nXp - pRhom[_FUL]->nXp, pRhom[_FUR]->nDeltaX-pRhom[_FUL]->nDeltaX, m_nXStepSize) + pRhom[_FUL]->nDeltaX;
	int	nXf = MulDiv(nZp - pRhom[_FLL]->nZp, nXtf-nXbf, m_nZStepSize) + nXbf;
	int nXbb = MulDiv(nXp - pRhom[_BLL]->nXp, pRhom[_BLR]->nDeltaX-pRhom[_BLL]->nDeltaX, m_nXStepSize) + pRhom[_BLL]->nDeltaX;
	int nXtb = MulDiv(nXp - pRhom[_BUL]->nXp, pRhom[_BUR]->nDeltaX-pRhom[_BUL]->nDeltaX, m_nXStepSize) + pRhom[_BUL]->nDeltaX;
	int	nXb = MulDiv(nZp - pRhom[_BLL]->nZp, nXtb-nXbb, m_nZStepSize) + nXbb;
	m_nXError = MulDiv(nYp - pRhom[_FLL]->nYp, nXb-nXf, m_nYStepSize) + nXf;
	m_dXError = (float)m_nXError * theApp.m_Axes[X1AXIS].fStepSize;

	int nYbf = MulDiv(nXp - pRhom[_FLL]->nXp, pRhom[_FLR]->nDeltaY-pRhom[_FLL]->nDeltaY, m_nXStepSize) + pRhom[_FLL]->nDeltaY;
	int nYtf = MulDiv(nXp - pRhom[_FUL]->nXp, pRhom[_FUR]->nDeltaY-pRhom[_FUL]->nDeltaY, m_nXStepSize) + pRhom[_FUL]->nDeltaY;
	int	nYf = MulDiv(nZp - pRhom[_FLL]->nZp, nYtf-nYbf, m_nZStepSize) + nYbf;
	int nYbb = MulDiv(nXp - pRhom[_BLL]->nXp, pRhom[_BLR]->nDeltaY-pRhom[_BLL]->nDeltaY, m_nXStepSize) + pRhom[_BLL]->nDeltaY;
	int nYtb = MulDiv(nXp - pRhom[_BUL]->nXp, pRhom[_BUR]->nDeltaY-pRhom[_BUL]->nDeltaY, m_nXStepSize) + pRhom[_BUL]->nDeltaY;
	int	nYb = MulDiv(nZp - pRhom[_BLL]->nZp, nYtb-nYbb, m_nZStepSize) + nYbb;
	m_nYError = MulDiv(nYp - pRhom[_FLL]->nYp, nYb-nYf, m_nYStepSize) + nYf;
	m_dYError = (float)m_nYError * theApp.m_Axes[Y1AXIS].fStepSize;

	int nZbf = MulDiv(nXp - pRhom[_FLL]->nXp, pRhom[_FLR]->nDeltaZ-pRhom[_FLL]->nDeltaZ, m_nXStepSize) + pRhom[_FLL]->nDeltaZ;
	int nZtf = MulDiv(nXp - pRhom[_FUL]->nXp, pRhom[_FUR]->nDeltaZ-pRhom[_FUL]->nDeltaZ, m_nXStepSize) + pRhom[_FUL]->nDeltaZ;
	int	nZf = MulDiv(nZp - pRhom[_FLL]->nZp, nZtf-nZbf, m_nZStepSize) + nZbf;
	int nZbb = MulDiv(nXp - pRhom[_BLL]->nXp, pRhom[_BLR]->nDeltaZ-pRhom[_BLL]->nDeltaZ, m_nXStepSize) + pRhom[_BLL]->nDeltaZ;
	int nZtb = MulDiv(nXp - pRhom[_BUL]->nXp, pRhom[_BUR]->nDeltaZ-pRhom[_BUL]->nDeltaZ, m_nXStepSize) + pRhom[_BUL]->nDeltaZ;
	int	nZb = MulDiv(nZp - pRhom[_BLL]->nZp, nZtb-nZbb, m_nZStepSize) + nZbb;
	m_nZError = MulDiv(nYp - pRhom[_FLL]->nYp, nZb-nZf, m_nYStepSize) + nZf;
	m_dZError = (float)m_nZError * theApp.m_Axes[Z1AXIS].fStepSize;

	pCpHead->Side1.fX -= m_dXError;
	pCpHead->Side1.fY -= m_dYError;
	pCpHead->Side1.fZ -= m_dZError;

}

int CKinematics::MulDiv(int nV0, int nV1, int nV2)
{
	_int64 V0 = (_int64)nV0;
	_int64 V1 = (_int64)nV1;
	_int64 V2 = (_int64)nV2;

	int nResult = (int)((V0*V1)/V2);

	return nResult;
}

void CKinematics::Compensate(CCoord *pCp)
{
	if(m_bRhomboidCompensation==TRUE) CalculateRealCoord(pCp);
	if(m_bTipCompensation==TRUE) CalculateRealTipCoord(pCp);
}

bool CKinematics::InitializeTip()
{
	float dX,dY;
	int	nX,nY;

	if(CalculateIntegerVariables()==FALSE) return FALSE;

	for(nX=0,dX=-m_dXTipMinMax;dX<=m_dXTipMinMax;nX++,dX+=m_dXTipStep) {
		for(nY=0,dY=-m_dYTipMinMax;dY<=m_dYTipMinMax;nY++,dY+=m_dYTipStep) {
				
			memset(&m_Tip[nX][nY],0,sizeof TipStruct);
			m_Tip[nX][nY].nStructSize = sizeof TipStruct;
			m_Tip[nX][nY].dXtp = dX;
			m_Tip[nX][nY].dYtp = dY;
			m_Tip[nX][nY].nXtp = (int)(dX / theApp.m_Axes[XT0AXIS].fStepSize);
			m_Tip[nX][nY].nYtp = (int)(dY / theApp.m_Axes[YT0AXIS].fStepSize);
		}
	}
	return TRUE;
}

void CKinematics::CalculateRealTipCoord(CCoord *pCp)
{
	CCoord Cp = *pCp;
	int nXtp = (int)(Cp.Side1.fXt / theApp.m_Axes[XT1AXIS].fStepSize);
	int nYtp = (int)(Cp.Side1.fYt / theApp.m_Axes[YT1AXIS].fStepSize);
	int	nX0,nX1,nY0,nY1;

	nX0 = MulDiv(nXtp+m_nXTipMinMax,m_nXTipPts-1,2*m_nXTipMinMax);
	if(nX0<0) nX0=0;
	if(nX0>=(m_nXTipPts-1)) nX0 = m_nXTipPts-2;
	nX1 = nX0 + 1;

	nY0 = MulDiv(nYtp+m_nYTipMinMax,m_nYTipPts-1,2*m_nYTipMinMax);
	if(nY0<0) nY0=0;
	if(nY0>=(m_nYTipPts-1)) nY0 = m_nYTipPts-2;
	nY1 = nY0 + 1;


	int nXb = MulDiv(nXtp-m_Tip[nX0][nY0].nXtp, m_Tip[nX1][nY0].nDeltaXt-m_Tip[nX0][nY0].nDeltaXt, m_nXTipStep) + m_Tip[nX0][nY0].nDeltaXt;
	int nXt = MulDiv(nXtp-m_Tip[nX0][nY1].nXtp, m_Tip[nX1][nY1].nDeltaXt-m_Tip[nX0][nY1].nDeltaXt, m_nXTipStep) + m_Tip[nX0][nY1].nDeltaXt;
	m_nXtError = MulDiv(nYtp-m_Tip[nX0][nY0].nYtp, nXt-nXb, m_nYTipStep) + nXb;
	m_dXtError = (float)m_nXtError * theApp.m_Axes[XT1AXIS].fStepSize;

	int nYl = MulDiv(nYtp-m_Tip[nX0][nY0].nYtp, m_Tip[nX0][nY1].nDeltaYt-m_Tip[nX0][nY0].nDeltaYt, m_nYTipStep) + m_Tip[nX0][nY0].nDeltaYt;
	int nYr = MulDiv(nYtp-m_Tip[nX1][nY0].nYtp, m_Tip[nX1][nY1].nDeltaYt-m_Tip[nX1][nY0].nDeltaYt, m_nYTipStep) + m_Tip[nX1][nY0].nDeltaYt;
	m_nYtError = MulDiv(nXtp-m_Tip[nX0][nY0].nXtp, nYr-nYl, m_nXTipStep) + nYl;
	m_dYtError = (float)m_nYtError * theApp.m_Axes[YT1AXIS].fStepSize;

	pCp->Side1.fXt -= m_dXtError;
	pCp->Side1.fYt -= m_dYtError;

}

void CKinematics::ModifyTipDeltas(int nX, int nY, CCoord CpHead)
{
	m_Tip[nX][nY].dDeltaXt = m_Tip[nX][nY].dXtp-CpHead.Side1.fXt;
	m_Tip[nX][nY].dDeltaYt = m_Tip[nX][nY].dYtp-CpHead.Side1.fYt;

	m_Tip[nX][nY].nDeltaXt = (int)(m_Tip[nX][nY].dDeltaXt / theApp.m_Axes[XT1AXIS].fStepSize);
	m_Tip[nX][nY].nDeltaYt = (int)(m_Tip[nX][nY].dDeltaYt / theApp.m_Axes[YT1AXIS].fStepSize);
}

void CKinematics::SetComponentThickness(float fComponentThickness)
{

	m_fComponentThickness  = fComponentThickness;
}

void CKinematics::CalculateStarboardSurfacePos(CCoord *pCp)
{
	float dXtAngle,dCosXtip,dSinXtip;
	float dYtAngle,dCosYtip,dSinYtip;

	dXtAngle=pCp->Side0.fXt*DEG_TO_RAD;
	dCosXtip = (float)cos(dXtAngle);
	dSinXtip = (float)sin(dXtAngle);

	dYtAngle=pCp->Side0.fYt*DEG_TO_RAD;
	dCosYtip = (float)cos(dYtAngle);
	dSinYtip = (float)sin(dYtAngle);

	pCp->Side1.fX=pCp->Side0.fX + (m_fComponentThickness*dCosYtip*dSinXtip); 
	pCp->Side1.fY=pCp->Side0.fY + (m_fComponentThickness*dCosYtip*dCosXtip); 
	pCp->Side1.fZ=pCp->Side0.fZ + (m_fComponentThickness*dSinYtip*dSinXtip); 
	
}

void CKinematics::CalculateBallCenterFromHole(CCoord CpHole, CCoord *pCpBall)
{
	float fH = theApp.m_LastSettings.fBallHeight - (theApp.m_LastSettings.fBallDiameter / 2.0f);
	*pCpBall = CpHole;

	pCpBall->Side0.fX += (CpHole.Side0.fI * fH);
	pCpBall->Side0.fY += (CpHole.Side0.fJ * fH);
	pCpBall->Side0.fZ += (CpHole.Side0.fK * fH);
}

bool CKinematics::CheckValidWaterPath(int nSide,int nTimeSlot,float *fWp)
{
	m_fWaterPath[nSide]=theApp.m_UtUser.GetGateWaterPath(nTimeSlot);
	if(m_fWaterPath[nSide] > 0.0f) {
		if(fWp != NULL) {
			*fWp = m_fWaterPath[nSide];
		}
		return TRUE;
	}
	return false;
}

void CKinematics::CalculateBallCenterFromSurface(CCoord CpSurface, CCoord *pCpCenter)
{
	float	fH = theApp.m_LastSettings.fBallDiameter / 2.0f;

	*pCpCenter = CpSurface;

	pCpCenter->Side0.fX=CpSurface.Side0.fX - (fH*CpSurface.Side0.fI);
	pCpCenter->Side0.fY=CpSurface.Side0.fY - (fH*CpSurface.Side0.fJ);
	pCpCenter->Side0.fZ=CpSurface.Side0.fZ - (fH*CpSurface.Side0.fK);
}



void CKinematics::ClearDeltas(int nX, int nY, int nZ)
{
	m_Rhomboid[nX][nY][nZ].dDeltaX = 0.0;
	m_Rhomboid[nX][nY][nZ].dDeltaY = 0.0;
	m_Rhomboid[nX][nY][nZ].dDeltaZ = 0.0;

	m_Rhomboid[nX][nY][nZ].nDeltaX = 0;
	m_Rhomboid[nX][nY][nZ].nDeltaY = 0;
	m_Rhomboid[nX][nY][nZ].nDeltaZ = 0;

}

void CKinematics::CopyPlaneToPlane(int nYSrc, int nYDest)
{
	int	nX,nZ;

	if(nYDest<0) return;
	if(nYDest>m_nYpts) return;

	for(nX=0;nX<=m_nXpts;nX++) {
		for(nZ=0;nZ<=m_nZpts;nZ++) {
			m_Rhomboid[nX][nYDest][nZ].dDeltaX = m_Rhomboid[nX][nYSrc][nZ].dDeltaX;
			m_Rhomboid[nX][nYDest][nZ].dDeltaY = m_Rhomboid[nX][nYSrc][nZ].dDeltaY;
			m_Rhomboid[nX][nYDest][nZ].dDeltaZ = m_Rhomboid[nX][nYSrc][nZ].dDeltaZ;
			m_Rhomboid[nX][nYDest][nZ].nDeltaX = m_Rhomboid[nX][nYSrc][nZ].nDeltaX;
			m_Rhomboid[nX][nYDest][nZ].nDeltaY = m_Rhomboid[nX][nYSrc][nZ].nDeltaY;
			m_Rhomboid[nX][nYDest][nZ].nDeltaZ = m_Rhomboid[nX][nYSrc][nZ].nDeltaZ;
		}
	}

}

void CKinematics::CurrentCubeMarkAction(CCoord	*pCpHead)
{
	CCoord Cp = *pCpHead;
	int	nX0,nX1,nY0,nY1,nZ0,nZ1;
	int nXp = (int)(Cp.Side1.fX / theApp.m_Axes[X1AXIS].fStepSize);
	int nYp = (int)(Cp.Side1.fY / theApp.m_Axes[Y1AXIS].fStepSize);
	int nZp = (int)(Cp.Side1.fZ / theApp.m_Axes[Z1AXIS].fStepSize);

	nX0 = MulDiv(nXp-m_nXMin,m_nXpts-1,m_nXMax-m_nXMin);
	if(nX0<0) nX0=0;
	if(nX0>=(m_nXpts-1)) nX0 = m_nXpts-2;
	nX1 = nX0 + 1;

	nY0 = MulDiv(nYp-m_nYMin,m_nYpts-1,m_nYMax-m_nYMin);
	if(nY0<0) nY0=0;
	if(nY0>=(m_nYpts-1)) nY0 = m_nYpts-2;
	nY1 = nY0 + 1;

	nZ0 = MulDiv(nZp-m_nZMin,m_nZpts-1,m_nZMax-m_nZMin);
	if(nZ0<0) nZ0=0;
	if(nZ0>=(m_nZpts-1)) nZ0 = m_nZpts-2;
	nZ1 = nZ0 + 1;


	m_Rhomboid[nX0][nY0][nZ0].nAction=1;
	m_Rhomboid[nX1][nY0][nZ0].nAction=1;
	m_Rhomboid[nX0][nY0][nZ1].nAction=1;
	m_Rhomboid[nX1][nY0][nZ1].nAction=1;
	m_Rhomboid[nX0][nY1][nZ0].nAction=1;
	m_Rhomboid[nX1][nY1][nZ0].nAction=1;
	m_Rhomboid[nX0][nY1][nZ1].nAction=1;
	m_Rhomboid[nX1][nY1][nZ1].nAction=1;

	m_Rhomboid[nX0][nY0][nZ0].nAmplitude=0;
	m_Rhomboid[nX1][nY0][nZ0].nAmplitude=0;
	m_Rhomboid[nX0][nY0][nZ1].nAmplitude=0;
	m_Rhomboid[nX1][nY0][nZ1].nAmplitude=0;
	m_Rhomboid[nX0][nY1][nZ0].nAmplitude=0;
	m_Rhomboid[nX1][nY1][nZ0].nAmplitude=0;
	m_Rhomboid[nX0][nY1][nZ1].nAmplitude=0;
	m_Rhomboid[nX1][nY1][nZ1].nAmplitude=0;

}

void CKinematics::MarkScanVolumeAction()
{
/*
	CCoord CpSurface,CpHead;
	int nLine,nS;

	for(nLine=0;nLine<PROFILE->m_nPolyLineL;nLine++) {
		for(nS=0;nS<PROFILE->m_PolyLine[nLine].m_nCoordL;nS++) {
			CpSurface = PROFILE->m_PolyLine[nLine].m_Coord[nS];
			HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
			CurrentCubeMarkAction(&CpHead);
		}
	}
*/
}

void CKinematics::MarkTestPieceAction()
{
/*
	float fXMin,fXMax,fX;
	float fZMin,fZMax,fZ;
	float fTemp;
	CCoord	CpSurface,CpHead;

	fXMin = PROFILE->m_CpTestPiece[0].Side0.fX;
	fXMax = PROFILE->m_CpTestPiece[1].Side0.fX;
	fZMin = PROFILE->m_CpTestPiece[0].Side0.fZ;
	fZMax = PROFILE->m_CpTestPiece[1].Side0.fZ;

	if((fTemp=fXMin) > fXMax) {
		fXMin = fXMax;
		fXMax = fTemp;
	}
	if((fTemp=fZMin) > fZMax) {
		fZMin = fZMax;
		fZMax = fTemp;
	}
	CpSurface = PROFILE->m_CpTestPiece[0];
	CpSurface.Side1.fY=CpSurface.Side0.fY;
	CpSurface.Side1.fXt=0.0;
	CpSurface.Side1.fYt=0.0;

	for(fZ=fZMin;fZ<=fZMax;fZ+=50.0) {
		for(fX=fXMin;fX<=fXMax;fX+=50.0) {
			CpSurface.Side1.fX=fX;
			CpSurface.Side1.fZ=fZ;
			HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE);
			CurrentCubeMarkAction(&CpHead);
		}
	}
*/
}


void CKinematics::Zero()
{

	memset(&m_Rhomboid,0,sizeof m_Rhomboid);
	memset(&m_Tip,0,sizeof m_Tip);
}

bool CKinematics::SaveCSV(CString FilePath)
{
	CUSLFile file;
	CFileException ExceptionInstance;
	int nX,nY,nZ;
	CString Buff,Temp;
	
	if (file.Open(FilePath, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite, &ExceptionInstance)==0) { 
	
		CString Buff;
		Buff = L"Cannot Save the Last Settings!: " + FilePath;
		MessageBox(NULL,Buff,_T("Warning"), MB_ICONERROR);
	} else {
		CArchive archFile(&file,CArchive::store);
		Buff = L"X,Y,Z,dX,dY,dZ\r\n";
		archFile.WriteString(Buff);
		for(nX=0;nX<m_nXpts;nX++) {
			for(nY=0;nY<m_nYpts;nY++) {
				for(nZ=0;nZ<m_nZpts;nZ++) {
					Buff.Format(_T("%.0f,%.0f,%.0f,%.02f,%.02f,%.02f\r\n"),
						m_Rhomboid[nX][nY][nZ].dXp,
						m_Rhomboid[nX][nY][nZ].dYp,
						m_Rhomboid[nX][nY][nZ].dZp,
						m_Rhomboid[nX][nY][nZ].dDeltaX,
						m_Rhomboid[nX][nY][nZ].dDeltaY,
						m_Rhomboid[nX][nY][nZ].dDeltaZ);
					archFile.WriteString(Buff);
				}
			}
		}

		Buff = L"\r\nXt,Yt,dXt,dYt\r\n";
		archFile.WriteString(Buff);
		for(nX=0;nX<m_nXTipPts;nX++) {
			for(nY=0;nY<m_nYTipPts;nY++) {
				Buff.Format(_T("%.0f,%.0f,%.02f,%.02f\r\n"),
					m_Tip[nX][nY].dXtp,
					m_Tip[nX][nY].dYtp,
					m_Tip[nX][nY].dDeltaXt,
					m_Tip[nX][nY].dDeltaYt);
				archFile.WriteString(Buff);
			}
		}

		archFile.Close();
		file.Close();
	}
	return TRUE;
}

void CKinematics::CalculateCoordAlongNorm(CCoord CpSurface,CCoord* pCp,int nSide,float fDistance)
{
	CopyMemory(&pCp->Side[nSide],&CpSurface.Side[nSide],sizeof SUBCOORD);

	pCp->Side[nSide].fX -= (pCp->Side[nSide].fI * fDistance);
	pCp->Side[nSide].fY -= (pCp->Side[nSide].fJ * fDistance);
	pCp->Side[nSide].fZ -= (pCp->Side[nSide].fK * fDistance);
}

void CKinematics::CalculateApproachPoint(CCoord CpSurface,CCoord* pCp,int nSide)
{
	CopyMemory(&pCp->Side[nSide],&CpSurface.Side[nSide],sizeof SUBCOORD);

	pCp->Side[nSide].fX += (pCp->Side[nSide].fI * theApp.m_LastSettings.fRelearnApproachDistance);
	pCp->Side[nSide].fY += (pCp->Side[nSide].fJ * theApp.m_LastSettings.fRelearnApproachDistance);
	pCp->Side[nSide].fZ += (pCp->Side[nSide].fK * theApp.m_LastSettings.fRelearnApproachDistance);
}

void CKinematics::CalculateOverShootPoint(CCoord CpSurface,CCoord* pCp,int nSide)
{
	CopyMemory(&pCp->Side[nSide],&CpSurface.Side[nSide],sizeof SUBCOORD);

	pCp->Side[nSide].fX -= (pCp->Side[nSide].fI * theApp.m_LastSettings.fRelearnOverShootDistance);
	pCp->Side[nSide].fY -= (pCp->Side[nSide].fJ * theApp.m_LastSettings.fRelearnOverShootDistance);
	pCp->Side[nSide].fZ -= (pCp->Side[nSide].fK * theApp.m_LastSettings.fRelearnOverShootDistance);
}


void CKinematics::PreProcessAxesCompensation()
{

	for(int nAxis=0;nAxis<16;nAxis++) {

	}

}


void CKinematics::GetDesiredWaterPath(CCoord *pCp, int nSide)
{
	pCp->fWp[nSide] = m_fDesiredWaterPath[nSide];
}

float CKinematics::GetDesiredWaterPath(int nSide)
{
	return m_fDesiredWaterPath[nSide];
}


void CKinematics::GetComponentThickness(CCoord *pCp)
{
	pCp->fThickness = m_fComponentThickness;
}

void CKinematics::InvalidateDesiredWaterPath(CCoord *pCp)
{
	if((pCp->fWp[PORTSIDE] > 2.0f) && (PROFILE->m_nHeader[H_WP0] == 1)) {
		m_fDesiredWaterPath[PORTSIDE] = pCp->fWp[PORTSIDE];
		m_bDesiredWaterPathChanged = true;
	}
	if((pCp->fWp[STARBOARD] > 2.0f) && (PROFILE->m_nHeader[H_WP1] == 1)) {
		m_fDesiredWaterPath[STARBOARD] = pCp->fWp[STARBOARD];
		m_bDesiredWaterPathChanged = true;
	}

}

void CKinematics::CalculateProbeSampleOffsets(CData *pData)
{
	for(int ii=0;ii<8;ii++) {
		m_Probe[ii].nSampleOffset = (int)(m_Probe[ii].fX / pData->m_fFastIncrement);
	}
}


//calculate rotate matrix so that normal will be 0,-1,0
void CKinematics::D3DXRotateMatrixFromJNormal(D3DXMATRIXA16 &mat, D3DXVECTOR4 vec)
{
	D3DXMATRIXA16 matRotZ,matRotY,matRotX;
	D3DXMATRIXA16 matTemp;

	float fLength = sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
	if(fLength == 0.0f) {
		D3DXMatrixIdentity( &mat);
 		return;
	}


	float fYAngle = atan2f(vec.x*-1.0f,vec.z*-1.0f);
	if(vec.x==0.0f && vec.z==0.0f) fYAngle = 0.0f;

	D3DXMatrixRotationY(&matRotY,fYAngle);
	fYAngle *= RAD_TO_DEG;

	float fXAngle = atan2f(sqrtf(vec.x*vec.x + vec.z*vec.z),-vec.y);
	D3DXMatrixRotationX(&matRotX,fXAngle);
	fXAngle *= RAD_TO_DEG;

	D3DXMatrixMultiply(&mat,&matRotX,&matRotY);

}



//calculate rotate matrix so that normal will be 0,0,1
void CKinematics::D3DXRotateMatrixFromKNormal(D3DXMATRIXA16 &mat, D3DXVECTOR4 vec)
{
	D3DXMATRIXA16 matRotZ,matRotY,matRotX;
	D3DXMATRIXA16 matTemp;

	float fLength = sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
	if(fLength == 0.0f) {
		D3DXMatrixIdentity( &mat);
 		return;
	}

	float fZAngle = atan2f(vec.x,vec.y);
	if((fabsf(vec.x) < 0.01f) && (fabsf(vec.y) < 0.01f)) fZAngle = 0.0f;
	D3DXMatrixRotationZ(&matRotZ,-fZAngle);

	float fXAngle = atan2f(sqrtf(vec.x*vec.x + vec.y*vec.y),vec.z);
	D3DXMatrixRotationX(&matRotX,-fXAngle);

	D3DXMatrixMultiply(&mat,&matRotX,&matRotZ);


//	D3DXMatrixRotationYawPitchRoll(&mat,vec.x * PIf / 2.0f,vec.z * PIf / 2.0f,0.0f);

//	D3DXVECTOR3 vecTest(0.0f,0.1f,0.9f);
//	D3DXVec3TransformCoord(&vecTest,&vecTest,&mat);

}


void CKinematics::CalculateCompensationPos(CCoord *pCp)
{
	CCoord Cp[3];
/*
	for(int nAxis=0;nAxis<3;nAxis++) {
		m_LeadScrew[nAxis].m_nStyle = 1;
		m_LeadScrew[nAxis].CalculateCoord(pCp->fPos[nAxis+10],Cp[nAxis]);
	}
	if(theApp.m_LastSettings.bUseAxesCompensation) {

	}
	*/
}


void CKinematics::LeadScrewCompensate(int nAxis, float fCurrentPos, float fDesirePos, int *nPos)
{
	if((fDesirePos - fCurrentPos) >= 0.0f) {
		*nPos = m_LeadScrew[nAxis].ActualPositiveToMachine((int)(fDesirePos / theApp.m_Axes[nAxis].fStepSize));
	} else {
		*nPos = m_LeadScrew[nAxis].ActualNegativeToMachine((int)(fDesirePos / theApp.m_Axes[nAxis].fStepSize));
	}


}


void CKinematics::SurfaceFromLaser(CCoord *CpLaser, CCoord *CpSurface)
{
	CCoord *ThreeCoordsFrom = (CCoord*)theApp.m_LastSettings.m_CpLaserHole ;
	CCoord *ThreeCoordsTo = (CCoord*)theApp.m_LastSettings.m_CpSquirterHole;

	Convert(ThreeCoordsTo, ThreeCoordsFrom, CpSurface, CpLaser);
}

void CKinematics::LaserFromSurface(CCoord *CpSurface, CCoord *CpLaser)
{
	CCoord *ThreeCoordsTo = (CCoord*)theApp.m_LastSettings.m_CpLaserHole ;
	CCoord *ThreeCoordsFrom = (CCoord*)theApp.m_LastSettings.m_CpSquirterHole;

	Convert(ThreeCoordsTo, ThreeCoordsFrom, CpLaser, CpSurface);

}

void CKinematics::Convert(CCoord CpThreeCoordsTo[], CCoord CpThreeCoordsFrom[], CCoord *CpTo, CCoord *CpFrom)
{
	float	fAngleHole0,fAngleHole1,fAngleHole2;
	float	fAngleSurface0,fAngleSurface1,fAngleSurface2;
	CCoord CpComponent[3];
	CCoord CpMeasured[3];
	
	*CpComponent=*CpThreeCoordsFrom;
	*CpMeasured=*CpThreeCoordsTo;

	//Accept Transform
	CalculateMatrix();
	TransformCoordinate(*CpFrom,CpTo);

	RotatePlaneToZ0(CpComponent[0],CpComponent[1],CpComponent[2],&fAngleHole0,&fAngleHole1,&fAngleHole2);
	RotatePlaneToZ0(CpMeasured[0],CpMeasured[1],CpMeasured[2],&fAngleSurface0,&fAngleSurface1,&fAngleSurface2);


	//translate to x = 0 y = 0 z = 0
	m_fTranslateX = -CpComponent[0].Side0.fX;
	m_fTranslateY = -CpComponent[0].Side0.fY;
	m_fTranslateZ = -CpComponent[0].Side0.fZ;
	m_fRotateX = 0.0;
	m_fRotateY = 0.0;
	m_fRotateZ = 0.0;
	CalculateMatrix();
	TransformCoordinate(*CpFrom,CpTo);
	CpFrom = CpTo;



	//Translate it
	m_fTranslateX = CpMeasured[0].Side0.fX;
	m_fTranslateY = CpMeasured[0].Side0.fY;
	m_fTranslateZ = CpMeasured[0].Side0.fZ;
	m_fRotateX = 0.0;
	m_fRotateY = 0.0;
	m_fRotateZ = 0.0;
	//Accept Transform
	CalculateMatrix();
	TransformCoordinate(*CpFrom,CpTo);
	CpFrom = CpTo;


}

void CKinematics::CalculateMatrix()
{
	float fResult[3][3];
	memset(fResult,0,sizeof fResult);

	m_fIden[0][0] = 1.0;
	m_fIden[0][1] = 0.0;
	m_fIden[0][2] = 0.0;
	m_fIden[1][0] = 0.0;
	m_fIden[1][1] = 1.0;
	m_fIden[1][2] = 0.0;
	m_fIden[2][0] = 0.0;
	m_fIden[2][1] = 0.0;
	m_fIden[2][2] = 1.0;

	m_fRotX[0][0] = 1.0;
	m_fRotX[0][1] = 0.0;
	m_fRotX[0][2] = 0.0;
	m_fRotX[1][0] = 0.0;
	m_fRotX[1][1] = (float)cos((double)m_fRotateX * DEG_TO_RAD);
	m_fRotX[1][2] = (float)sin((double)m_fRotateX * DEG_TO_RAD);
	m_fRotX[2][0] = 0.0;
	m_fRotX[2][1] = (float)sin((double)m_fRotateX * DEG_TO_RAD) * -1;
	m_fRotX[2][2] = (float)cos((double)m_fRotateX * DEG_TO_RAD);

	m_fRotY[0][0] = (float)cos((double)m_fRotateY * DEG_TO_RAD);
	m_fRotY[0][1] = (float)sin((double)m_fRotateY * DEG_TO_RAD);
	m_fRotY[0][2] = 0.0;
	m_fRotY[1][0] = (float)sin((double)m_fRotateY * DEG_TO_RAD) * -1;
	m_fRotY[1][1] = (float)cos((double)m_fRotateY * DEG_TO_RAD);
	m_fRotY[1][2] = 0.0;
	m_fRotY[2][0] = 0.0;
	m_fRotY[2][1] = 0.0;
	m_fRotY[2][2] = 1.0;

	m_fRotZ[0][0] = (float)cos((double)m_fRotateZ * DEG_TO_RAD);
	m_fRotZ[0][1] = 0.0;
	m_fRotZ[0][2] = (float)sin((double)m_fRotateZ * DEG_TO_RAD) * -1;
	m_fRotZ[1][0] = 0.0;
	m_fRotZ[1][1] = 1.0;
	m_fRotZ[1][2] = 0.0;
	m_fRotZ[2][0] = (float)sin((double)m_fRotateZ * DEG_TO_RAD);
	m_fRotZ[2][1] = 0.0;
	m_fRotZ[2][2] = (float)cos((double)m_fRotateZ * DEG_TO_RAD);

	fResult[0][0] = (m_fIden[0][0] * m_fRotX[0][0] + m_fIden[0][1] * m_fRotX[1][0] + m_fIden[0][2] * m_fRotX[2][0]);
	fResult[0][1] = (m_fIden[0][0] * m_fRotX[0][1] + m_fIden[0][1] * m_fRotX[1][1] + m_fIden[0][2] * m_fRotX[2][1]);
	fResult[0][2] = (m_fIden[0][0] * m_fRotX[0][2] + m_fIden[0][1] * m_fRotX[1][2] + m_fIden[0][2] * m_fRotX[2][2]);
	fResult[1][0] = (m_fIden[1][0] * m_fRotX[0][0] + m_fIden[1][1] * m_fRotX[1][0] + m_fIden[1][2] * m_fRotX[2][0]);
	fResult[1][1] = (m_fIden[1][0] * m_fRotX[0][1] + m_fIden[1][1] * m_fRotX[1][1] + m_fIden[1][2] * m_fRotX[2][1]);
	fResult[1][2] = (m_fIden[1][0] * m_fRotX[0][2] + m_fIden[1][1] * m_fRotX[1][2] + m_fIden[1][2] * m_fRotX[2][2]);
	fResult[2][0] = (m_fIden[2][0] * m_fRotX[0][0] + m_fIden[2][1] * m_fRotX[1][0] + m_fIden[2][2] * m_fRotX[2][0]);
	fResult[2][1] = (m_fIden[2][0] * m_fRotX[0][1] + m_fIden[2][1] * m_fRotX[1][1] + m_fIden[2][2] * m_fRotX[2][1]);
	fResult[2][2] = (m_fIden[2][0] * m_fRotX[0][2] + m_fIden[2][1] * m_fRotX[1][2] + m_fIden[2][2] * m_fRotX[2][2]);
	memcpy(m_fIden,fResult,sizeof fResult);

	fResult[0][0] = (m_fIden[0][0] * m_fRotY[0][0] + m_fIden[0][1] * m_fRotY[1][0] + m_fIden[0][2] * m_fRotY[2][0]);
	fResult[0][1] = (m_fIden[0][0] * m_fRotY[0][1] + m_fIden[0][1] * m_fRotY[1][1] + m_fIden[0][2] * m_fRotY[2][1]);
	fResult[0][2] = (m_fIden[0][0] * m_fRotY[0][2] + m_fIden[0][1] * m_fRotY[1][2] + m_fIden[0][2] * m_fRotY[2][2]);
	fResult[1][0] = (m_fIden[1][0] * m_fRotY[0][0] + m_fIden[1][1] * m_fRotY[1][0] + m_fIden[1][2] * m_fRotY[2][0]);
	fResult[1][1] = (m_fIden[1][0] * m_fRotY[0][1] + m_fIden[1][1] * m_fRotY[1][1] + m_fIden[1][2] * m_fRotY[2][1]);
	fResult[1][2] = (m_fIden[1][0] * m_fRotY[0][2] + m_fIden[1][1] * m_fRotY[1][2] + m_fIden[1][2] * m_fRotY[2][2]);
	fResult[2][0] = (m_fIden[2][0] * m_fRotY[0][0] + m_fIden[2][1] * m_fRotY[1][0] + m_fIden[2][2] * m_fRotY[2][0]);
	fResult[2][1] = (m_fIden[2][0] * m_fRotY[0][1] + m_fIden[2][1] * m_fRotY[1][1] + m_fIden[2][2] * m_fRotY[2][1]);
	fResult[2][2] = (m_fIden[2][0] * m_fRotY[0][2] + m_fIden[2][1] * m_fRotY[1][2] + m_fIden[2][2] * m_fRotY[2][2]);
	memcpy(m_fIden,fResult,sizeof fResult);

	fResult[0][0] = (m_fIden[0][0] * m_fRotZ[0][0] + m_fIden[0][1] * m_fRotZ[1][0] + m_fIden[0][2] * m_fRotZ[2][0]);
	fResult[0][1] = (m_fIden[0][0] * m_fRotZ[0][1] + m_fIden[0][1] * m_fRotZ[1][1] + m_fIden[0][2] * m_fRotZ[2][1]);
	fResult[0][2] = (m_fIden[0][0] * m_fRotZ[0][2] + m_fIden[0][1] * m_fRotZ[1][2] + m_fIden[0][2] * m_fRotZ[2][2]);
	fResult[1][0] = (m_fIden[1][0] * m_fRotZ[0][0] + m_fIden[1][1] * m_fRotZ[1][0] + m_fIden[1][2] * m_fRotZ[2][0]);
	fResult[1][1] = (m_fIden[1][0] * m_fRotZ[0][1] + m_fIden[1][1] * m_fRotZ[1][1] + m_fIden[1][2] * m_fRotZ[2][1]);
	fResult[1][2] = (m_fIden[1][0] * m_fRotZ[0][2] + m_fIden[1][1] * m_fRotZ[1][2] + m_fIden[1][2] * m_fRotZ[2][2]);
	fResult[2][0] = (m_fIden[2][0] * m_fRotZ[0][0] + m_fIden[2][1] * m_fRotZ[1][0] + m_fIden[2][2] * m_fRotZ[2][0]);
	fResult[2][1] = (m_fIden[2][0] * m_fRotZ[0][1] + m_fIden[2][1] * m_fRotZ[1][1] + m_fIden[2][2] * m_fRotZ[2][1]);
	fResult[2][2] = (m_fIden[2][0] * m_fRotZ[0][2] + m_fIden[2][1] * m_fRotZ[1][2] + m_fIden[2][2] * m_fRotZ[2][2]);
	memcpy(m_fIden,fResult,sizeof fResult);

}

void CKinematics::TransformCoordinate(COORDDATA Cp, COORDDATA *pCp)
{
	float dXp,dYp,dZp,dIp,dJp,dKp;

	*pCp = Cp;
	for(int nSide=0;nSide<2;nSide++) {
		pCp->Side[nSide].fX += m_fTranslateX;
		pCp->Side[nSide].fY += m_fTranslateY;
		pCp->Side[nSide].fZ += m_fTranslateZ;
		dXp = (pCp->Side[nSide].fX * m_fIden[0][0] + pCp->Side[nSide].fY * m_fIden[0][1] + pCp->Side[nSide].fZ * m_fIden[0][2]);
		dYp = (pCp->Side[nSide].fX * m_fIden[1][0] + pCp->Side[nSide].fY * m_fIden[1][1] + pCp->Side[nSide].fZ * m_fIden[1][2]);
		dZp = (pCp->Side[nSide].fX * m_fIden[2][0] + pCp->Side[nSide].fY * m_fIden[2][1] + pCp->Side[nSide].fZ * m_fIden[2][2]);
		pCp->Side[nSide].fX = dXp;
		pCp->Side[nSide].fY = dYp;
		pCp->Side[nSide].fZ = dZp;

		dIp = (pCp->Side[nSide].fI * m_fIden[0][0] + pCp->Side[nSide].fJ * m_fIden[0][1] + pCp->Side[nSide].fK * m_fIden[0][2]);
		dJp = (pCp->Side[nSide].fI * m_fIden[1][0] + pCp->Side[nSide].fJ * m_fIden[1][1] + pCp->Side[nSide].fK * m_fIden[1][2]);
		dKp = (pCp->Side[nSide].fI * m_fIden[2][0] + pCp->Side[nSide].fJ * m_fIden[2][1] + pCp->Side[nSide].fK * m_fIden[2][2]);
		pCp->Side[nSide].fI = dIp;
		pCp->Side[nSide].fJ = dJp;
		pCp->Side[nSide].fK = dKp;

	}

}

void CKinematics::RotatePlaneToZ0(CCoord O,CCoord A, CCoord B, float *fAngle0, float *fAngle1, float *fAngle2)
{
	CCoord	Cp,Z,ON;
	VectorStruct	a,b;
	float fVectorLength;
	CString	Buff;

	*fAngle0=0.0f;
	*fAngle1=0.0f;
	*fAngle2=0.0f;

	//translate to x = 0 y = 0 z = 0
	m_fTranslateX = -O.Side0.fX;
	m_fTranslateY = -O.Side0.fY;
	m_fTranslateZ = -O.Side0.fZ;
	m_fRotateX = 0.0f;
	m_fRotateY = 0.0f;
	m_fRotateZ = 0.0f;
	CalculateMatrix();

	TransformCoordinate(O,&Cp);
	O = Cp;
	TransformCoordinate(A,&Cp);
	A = Cp;
	TransformCoordinate(B,&Cp);
	B = Cp;
	Buff.Format(_T("A %.1f %.1f %.1f B %.1f %.1f %.1f"),A.Side0.fX,A.Side0.fY,A.Side0.fZ,B.Side0.fX,B.Side0.fY,B.Side0.fZ);

	//Rotate About X Axis to get one vector at z = 0
	ON=A;
	Z=A;
	ON.Side0.fY=0.0f;
	ON.Side0.fZ=0.0f;
	Z.Side0.fZ=0.0f;

	a.fI = A.Side0.fX-ON.Side0.fX;
	a.fJ = A.Side0.fY-ON.Side0.fY;
	a.fK = A.Side0.fZ-ON.Side0.fZ;
	b.fI = Z.Side0.fX-ON.Side0.fX;
	b.fJ = Z.Side0.fY-ON.Side0.fY;
	b.fK = Z.Side0.fZ-ON.Side0.fZ;

	fVectorLength = VectorLength(a);								//add

	m_fTranslateX = 0.0f;
	m_fTranslateY = 0.0f;
	m_fTranslateZ = 0.0f;
	m_fRotateX = 0.0f;
	if(fVectorLength) m_fRotateX =(float)acos(a.fJ/fVectorLength) * 1.0f * RAD_TO_DEG;
	m_fRotateY = 0.0f;
	m_fRotateZ = 0.0f;
	if(a.fK<0) m_fRotateX *= -1.0f;
	*fAngle0 = m_fRotateX;

	CalculateMatrix();

	TransformCoordinate(A,&Cp);
	A = Cp;
	TransformCoordinate(B,&Cp);
	B = Cp;
	Buff.Format(_T("A %.1f %.1f %.1f B %.1f %.1f %.1f"),A.Side0.fX,A.Side0.fY,A.Side0.fZ,B.Side0.fX,B.Side0.fY,B.Side0.fZ);

	//Rotate about z axis to get first vector to be along the zero axis
	Z=A;
	Z.Side0.fY=0.0f;

	a.fI = A.Side0.fX-O.Side0.fX;
	a.fJ = A.Side0.fY-O.Side0.fY;
	a.fK = A.Side0.fZ-O.Side0.fZ;
	b.fI = Z.Side0.fX-O.Side0.fX;
	b.fJ = Z.Side0.fY-O.Side0.fY;
	b.fK = Z.Side0.fZ-O.Side0.fZ;

	fVectorLength = VectorLength(a);

	m_fTranslateX = 0.0f;
	m_fTranslateY = 0.0f;
	m_fTranslateZ = 0.0f;
	m_fRotateX = 0.0f;
	m_fRotateY = (float)acos(a.fI/fVectorLength) * 1.0f * RAD_TO_DEG;
	m_fRotateZ = 0.0f;
	*fAngle1 = m_fRotateY;

	CalculateMatrix();

	TransformCoordinate(A,&Cp);
	A = Cp;
	TransformCoordinate(B,&Cp);
	B = Cp;
	Buff.Format(_T("A %.1f %.1f %.1f B %.1f %.1f %.1f"),A.Side0.fX,A.Side0.fY,A.Side0.fZ,B.Side0.fX,B.Side0.fY,B.Side0.fZ);


	//Rotate About X Axis to get second vector at z = 0
	ON=B;
	Z=B;
	ON.Side0.fY=0.0f;
	ON.Side0.fZ=0.0f;
	Z.Side0.fZ=0.0f;

	a.fI = B.Side0.fX-ON.Side0.fX;
	a.fJ = B.Side0.fY-ON.Side0.fY;
	a.fK = B.Side0.fZ-ON.Side0.fZ;
	b.fI = Z.Side0.fX-ON.Side0.fX;
	b.fJ = Z.Side0.fY-ON.Side0.fZ;
	b.fK = Z.Side0.fZ-ON.Side0.fZ;

	fVectorLength = VectorLength(a);								//add

	m_fTranslateX = 0.0f;
	m_fTranslateY = 0.0f;
	m_fTranslateZ = 0.0f;
	m_fRotateX =(float)acos(b.fJ/fVectorLength) * 1.0f * RAD_TO_DEG;
	m_fRotateY = 0.0f;
	m_fRotateZ = 0.0f;
	if(a.fK<0) m_fRotateX *= -1.0f;
	*fAngle2 = m_fRotateX;

	CalculateMatrix();

	TransformCoordinate(A,&Cp);
	A = Cp;
	TransformCoordinate(B,&Cp);
	B = Cp;
	Buff.Format(_T("A %.1f %.1f %.1f B %.1f %.1f %.1f"),A.Side0.fX,A.Side0.fY,A.Side0.fZ,B.Side0.fX,B.Side0.fY,B.Side0.fZ);


}

float CKinematics::VectorLength(VectorStruct Vp)
{
	float fResult = (float)sqrt((double)(Vp.fI*Vp.fI + Vp.fJ*Vp.fJ + Vp.fK*Vp.fK));

	return fResult;
}

void CKinematics::CalculateRandP(CCoord *pCp, int nSide, float fRefAngle)
{
	CCoord Cp;
	float fRadius,fAngle;
	D3DXVECTOR3 pt;
	D3DXVECTOR3 vecP;
	D3DXMATRIXA16 matRotate;

//	theApp.m_Tank.vTTNorm = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//	theApp.m_Tank.vTTZeroDegrees = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	pt = pCp->Side[nSide].pt;

	if(theApp.m_LastSettings.vecTurnTableNormal.z == 1.0f) {
		pt.z = 0.0f;

		fRadius = D3DXVec3Length(&pt);
		D3DXVec3Normalize(&pt,&pt);

		fAngle = acosf(D3DXVec3Dot(&pt, &theApp.m_Tank.vTTZeroDegrees)) * 57.29577951f;

		if((theApp.m_Tank.vTTZeroDegrees.x < 0.0f) && (theApp.m_Tank.vTTZeroDegrees.y == 0.0f)) {
			if(pCp->Side[nSide].fY >=0.0f) {
				pCp->Side[nSide].fR = pCp->Side[nSide].fR - fAngle;
				fAngle = pCp->Side[nSide].fR + fAngle;
			} else {
				pCp->Side[nSide].fR = pCp->Side[nSide].fR + fAngle;
				fAngle = pCp->Side[nSide].fR - fAngle;
			}
		}

		D3DXMatrixRotationZ(&matRotate,(fAngle - fRefAngle) * DEG_TO_RAD);

		D3DXVec3TransformCoord(&pCp->Side[nSide].pt,&pCp->Side[nSide].pt,&matRotate);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&pCp->Side[nSide].norm,(D3DXVECTOR3*)&pCp->Side[nSide].norm,&matRotate);
		D3DXVec3Normalize((D3DXVECTOR3*)&pCp->Side[nSide].norm,(D3DXVECTOR3*)&pCp->Side[nSide].norm);
		pCp->fRadius[nSide] = fRadius;
	}

	if(theApp.m_LastSettings.vecTurnTableNormal.y == 1.0f) {
		pt.y = 0.0f;

		fRadius = D3DXVec3Length(&pt);
		D3DXVec3Normalize(&pt,&pt);

		D3DXMatrixRotationY(&matRotate,(pCp->Side[nSide].fR - fRefAngle) * DEG_TO_RAD);

		D3DXVec3TransformCoord(&pCp->Side[nSide].pt,&pCp->Side[nSide].pt,&matRotate);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&pCp->Side[nSide].norm,(D3DXVECTOR3*)&pCp->Side[nSide].norm,&matRotate);
		D3DXVec3Normalize((D3DXVECTOR3*)&pCp->Side[nSide].norm,(D3DXVECTOR3*)&pCp->Side[nSide].norm);

		pCp->Side[nSide].fR = fRefAngle;
		pCp->fRadius[nSide] = fRadius;
	}
}

void CKinematics::MoveInToCorrectQuadrant(CPolyCoord *pLine, int nSide)
{
	int ii;
	bool bChangeToPositiveQuadrant = false;
	bool bChangeToNegativeQuadrant = false;

	switch(theApp.m_LastSettings.nManipulatorType[nSide]) {
	case ROTATE_THETA_HORIZONTAL:
		if(theApp.m_LastSettings.nRTheta180Check == 1) {
			for(ii=0;ii<pLine->m_nCoordL;ii++) {
				if(pLine->m_pCp[ii].Side[nSide].fXt <= -180.0f) bChangeToPositiveQuadrant = true;
				if(pLine->m_pCp[ii].Side[nSide].fXt >= 180.0f) bChangeToNegativeQuadrant = true;
			}
			if(bChangeToPositiveQuadrant == true) {
				for(ii=0;ii<pLine->m_nCoordL;ii++) {
					pLine->m_pCp[ii].Side[nSide].fXt += 180.0f;
					pLine->m_pCp[ii].Side[nSide].fYt *= -1.0f;
					pLine->m_pCp[ii].Side[nSide].fW = sinf(	pLine->m_pCp[ii].Side[nSide].fYt * DEG_TO_RAD);
				}
			}
			if(bChangeToNegativeQuadrant == true) {
				for(ii=0;ii<pLine->m_nCoordL;ii++) {
					pLine->m_pCp[ii].Side[nSide].fXt -= 180.0f;
					pLine->m_pCp[ii].Side[nSide].fYt *= -1.0f;
					pLine->m_pCp[ii].Side[nSide].fW = sinf(	pLine->m_pCp[ii].Side[nSide].fYt * DEG_TO_RAD);
				}
			}
		}

		if(theApp.m_LastSettings.nRTheta90Check == 1) {
			//Check to see if all the values are greater than 90 degrees
			bChangeToNegativeQuadrant = true;
			for(ii=0;ii<pLine->m_nCoordL;ii++) {
				if(pLine->m_pCp[ii].Side[nSide].fXt < 90.0f) bChangeToNegativeQuadrant = false;
			}
			if(bChangeToNegativeQuadrant == true) {
				for(ii=0;ii<pLine->m_nCoordL;ii++) {
					pLine->m_pCp[ii].Side[nSide].fXt -= 180.0f;
					pLine->m_pCp[ii].Side[nSide].fYt *= -1.0f;
					pLine->m_pCp[ii].Side[nSide].fW = sinf(	pLine->m_pCp[ii].Side[nSide].fYt * DEG_TO_RAD);
				}
			}

			//Check to see if all the values are less than -90 degrees
			bChangeToPositiveQuadrant = true;
			for(ii=0;ii<pLine->m_nCoordL;ii++) {
				if(pLine->m_pCp[ii].Side[nSide].fXt > -90.0f) bChangeToPositiveQuadrant = false;
			}
			if(bChangeToPositiveQuadrant == true) {
				for(ii=0;ii<pLine->m_nCoordL;ii++) {
					pLine->m_pCp[ii].Side[nSide].fXt += 180.0f;
					pLine->m_pCp[ii].Side[nSide].fYt *= -1.0f;
					pLine->m_pCp[ii].Side[nSide].fW = sinf(	pLine->m_pCp[ii].Side[nSide].fYt * DEG_TO_RAD);
				}
			}
		}

		break;
	}
}


inline char Quadrant(CCoord* pCp,int nSide)
{
	float fXt,fYt;

	fXt = fmodf(pCp->Side[nSide].fXt, 360.0f);
	fYt = fmodf(pCp->Side[nSide].fYt, 360.0f);

	if(fXt >= 0.0f) {
		if(fYt<90.0f) return 'A';
		if(fYt<180.0f) return 'S';
		if(fYt<270.0f) return 'T';
		if(fYt<360.0f) return 'C';
	} else {
		if(fYt>-90.0f) return 'C';
		if(fYt>-180.0f) return 'T';
		if(fYt>-270.0f) return 'S';
		if(fYt>-360.0f) return 'A';
	}
	return 'A';
}

inline char Quadrant(COORDDATA* pCp,int nSide)
{
	float fXt,fYt;

	fXt = fmodf(pCp->Side[nSide].fXt, 360.0f);
	fYt = fmodf(pCp->Side[nSide].fYt, 360.0f);

	if(fXt >= 0.0f) {
		if(fYt<90.0f) return 'A';
		if(fYt<180.0f) return 'S';
		if(fYt<270.0f) return 'T';
		if(fYt<360.0f) return 'C';
	} else {
		if(fYt>-90.0f) return 'C';
		if(fYt>-180.0f) return 'T';
		if(fYt>-270.0f) return 'S';
		if(fYt>-360.0f) return 'A';
	}
	return 'A';
}


void CKinematics::RThetaShortestRoutes(CPolyCoord *pLine, int nDir, CCoord *pCpCurrent)
{
	int nSide = 0;
	int ii;
	float fXt[2],fYt[2],fLength[2];
	if(theApp.m_LastSettings.nManipulatorType[nSide] != ROTATE_THETA_HORIZONTAL) return;


	if(nDir & 1) pLine->ReverseAllCoords();

	for(ii=0;ii<(pLine->m_nCoordL - 2);ii++) {
		if(Quadrant(&pLine->m_pCp[ii],nSide) != Quadrant(pCpCurrent, nSide)) {
			fXt[0] = pLine->m_pCp[ii].Side[nSide].fXt;
			fYt[0] = pLine->m_pCp[ii].Side[nSide].fYt;
			if(fXt[0] >= 0) {
				fXt[1] = fXt[0] - 180.0f;
			} else {
				fXt[1] = fXt[0] + 180.0f;
			}
			fYt[1] = fYt[0] * -1.0f;
			fLength[0] = fabsf(fXt[0] - pCpCurrent->Side[nSide].fXt);
			fLength[1] = fabsf(fXt[1] - pCpCurrent->Side[nSide].fXt);
			if(fLength[0] < fLength[1]) {
				pLine->m_pCp[ii].Side[nSide].fXt = fXt[0];
				pLine->m_pCp[ii].Side[nSide].fYt = fYt[0];
			} else {
				pLine->m_pCp[ii].Side[nSide].fXt = fXt[1];
				pLine->m_pCp[ii].Side[nSide].fYt = fYt[1];
			}
		}
	}


	if(nDir & 1) pLine->ReverseAllCoords();

}

void CKinematics::CalculateRollerKinematics(float fComponentDiameter, bool bSetStepsPerRev, bool bTakeShortestTurntableRoute)
{
	if( fComponentDiameter<=0.0f ) fComponentDiameter = 10.0f;
	if( theApp.m_LastSettings.fRollerDiameter <= 0.0f ) theApp.m_LastSettings.fRollerDiameter = 10.0f;
	if (theApp.m_LastSettings.fRollerStepSize <= 0.0f) theApp.m_LastSettings.fRollerStepSize = 0.001f;

	theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize = (theApp.m_LastSettings.fRollerDiameter / fComponentDiameter) * theApp.m_LastSettings.fRollerStepSize;
	if (bSetStepsPerRev == true) {
		theApp.m_Axes[theApp.m_Tank.nRLeft].nStepsPerRev = (int)(360.0f / theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize);
		theApp.m_Axes[theApp.m_Tank.nRLeft].dStepsPerRev = (double)(360.0 / (double)theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize);
	}
	else {
		theApp.m_Axes[theApp.m_Tank.nRLeft].nStepsPerRev = 0;
		theApp.m_Axes[theApp.m_Tank.nRLeft].dStepsPerRev = 0.0;
	}
	theApp.m_PmacUser.m_bTakeShortestTurntableRoute = bTakeShortestTurntableRoute;
}


bool CKinematics::SaveManipulator(CFile *pFile,int nSide)
{
	int nKinematicOffset[32],nLimitSwitchOffset[32];
	int nNotUsed[8];

	for(int ii=0;ii<32;ii++) {
		nKinematicOffset[ii] = theApp.m_Axes[ii].nOffset;
		nLimitSwitchOffset[ii] = theApp.m_Axes[ii].nLimitSwitchOffset;
	}


	pFile->Write(&theApp.m_fVersion, sizeof theApp.m_fVersion);
	pFile->Write(&nSide, sizeof nSide);
	pFile->Write(nKinematicOffset,sizeof nKinematicOffset);
	pFile->Write(nLimitSwitchOffset,sizeof nLimitSwitchOffset);
	pFile->Write(&theApp.m_LastSettings.nManipulatorType[nSide],	sizeof theApp.m_LastSettings.nManipulatorType[0]);
	pFile->Write(&nLimitSwitchOffsetLoadMask,	sizeof nLimitSwitchOffsetLoadMask);
	pFile->Write(&nKinematicOffsetLoadMask,		sizeof nKinematicOffsetLoadMask);

	pFile->Write(&m_fXOffset[nSide],			sizeof m_fXOffset[nSide]);
	pFile->Write(&m_fYOffset[nSide],			sizeof m_fYOffset[nSide]);
	pFile->Write(&m_fZOffset[nSide],			sizeof m_fZOffset[nSide]);
	pFile->Write(&m_fProbeLength[nSide],		sizeof m_fProbeLength[nSide]);
	pFile->Write(&m_fArmLength[nSide],			sizeof m_fArmLength[nSide]);
	pFile->Write(&nNotUsed[nSide],				sizeof nNotUsed[nSide]);
	pFile->Write(&m_fProbeMirror[nSide],		sizeof m_fProbeMirror[nSide]);

	pFile->Write(&m_vLimb0[nSide],				sizeof m_vLimb0[nSide]);
	pFile->Write(&m_vLimb1[nSide],				sizeof m_vLimb1[nSide]);

	return true;
}

bool CKinematics::RetrieveManipulator(CFile *pFile, int nSide)
{
	int nMask;
	int nKinematicOffset[32],nLimitSwitchOffset[32];
	float fVersion;
	int	nSavedSide;
	int nNotUsed[8];

	pFile->Read(&fVersion, sizeof fVersion);
	pFile->Read(&nSavedSide, sizeof nSavedSide);
	if(nSavedSide - nSide) {

		if(MessageBox(NULL,L"This manipulator is for the opposite side\r\nDo you wish to open it anyway?",L"Warning",MB_ICONWARNING|MB_YESNO)!=IDYES) {
			return false;
		}
	}

	pFile->Read(nKinematicOffset,sizeof nKinematicOffset);
	pFile->Read(nLimitSwitchOffset,sizeof nLimitSwitchOffset);
	pFile->Read(&theApp.m_LastSettings.nManipulatorType[nSide],	sizeof theApp.m_LastSettings.nManipulatorType[0]);
	pFile->Read(&nLimitSwitchOffsetLoadMask,	sizeof nLimitSwitchOffsetLoadMask);
	pFile->Read(&nKinematicOffsetLoadMask,		sizeof nKinematicOffsetLoadMask);

	pFile->Read(&m_fXOffset[nSide],			sizeof m_fXOffset[nSide]);
	pFile->Read(&m_fYOffset[nSide],			sizeof m_fYOffset[nSide]);
	pFile->Read(&m_fZOffset[nSide],			sizeof m_fZOffset[nSide]);
	pFile->Read(&m_fProbeLength[nSide],		sizeof m_fProbeLength[nSide]);
	pFile->Read(&m_fArmLength[nSide],		sizeof m_fArmLength[nSide]);
	pFile->Read(&nNotUsed[nSide],			sizeof nNotUsed[nSide]);
	pFile->Read(&m_fProbeMirror[nSide],		sizeof m_fProbeMirror[nSide]);

	pFile->Read(&m_vLimb0[nSide],			sizeof m_vLimb0[nSide]);
	pFile->Read(&m_vLimb1[nSide],			sizeof m_vLimb1[nSide]);

	if(nSide==PORTSIDE) {
		theApp.m_Axes[theApp.m_Tank.nXLeft].nLimitSwitchOffset = nLimitSwitchOffset[theApp.m_Tank.nXLeft];
		theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset = nKinematicOffset[theApp.m_Tank.nXLeft];
		theApp.m_Axes[theApp.m_Tank.nYLeft].nLimitSwitchOffset = nLimitSwitchOffset[theApp.m_Tank.nYLeft];
		theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset = nKinematicOffset[theApp.m_Tank.nYLeft];
		theApp.m_Axes[theApp.m_Tank.nZLeft].nLimitSwitchOffset = nLimitSwitchOffset[theApp.m_Tank.nZLeft];
		theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset = nKinematicOffset[theApp.m_Tank.nZLeft];

		nMask = 1;
		nMask<<=theApp.m_Tank.nXtLeft;
		if(nLimitSwitchOffsetLoadMask & nMask) 
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nLimitSwitchOffset = nLimitSwitchOffset[theApp.m_Tank.nXtLeft];
		if(nKinematicOffsetLoadMask & nMask) 
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset = nKinematicOffset[theApp.m_Tank.nXtLeft];
		nMask = 1;
		nMask<<=theApp.m_Tank.nYtLeft;
		if(nLimitSwitchOffsetLoadMask & nMask) 
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nLimitSwitchOffset = nLimitSwitchOffset[theApp.m_Tank.nYtLeft];
		if(nKinematicOffsetLoadMask & nMask) 
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset = nKinematicOffset[theApp.m_Tank.nYtLeft];

	} else {
		nMask = 1;
		nMask<<=theApp.m_Tank.nXtRight;
		if(nLimitSwitchOffsetLoadMask & nMask) 
			theApp.m_Axes[theApp.m_Tank.nXtRight].nLimitSwitchOffset = nLimitSwitchOffset[theApp.m_Tank.nXtRight];
		if(nKinematicOffsetLoadMask & nMask) 
			theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset = nKinematicOffset[theApp.m_Tank.nXtRight];
		nMask = 1;
		nMask<<=theApp.m_Tank.nYtRight;
		if(nLimitSwitchOffsetLoadMask & nMask) 
			theApp.m_Axes[theApp.m_Tank.nYtRight].nLimitSwitchOffset = nLimitSwitchOffset[theApp.m_Tank.nYtRight];
		if(nKinematicOffsetLoadMask & nMask) 
			theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset = nKinematicOffset[theApp.m_Tank.nYtRight];
	}


	theApp.m_PmacUser.DownloadLimitSwitchOffsets();

	return true;
}

void CKinematics::D3DXRotateMatrixFromNormal(D3DXMATRIXA16 &mat, D3DXVECTOR4 vec)
{
	D3DXMATRIXA16 matRotZ,matRotY,matRotX;
	D3DXMATRIXA16 matTemp;

	float fLength = sqrtf(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
	if(fLength == 0.0f) {
		D3DXMatrixIdentity( &mat);
 		return;
	}

	float fZAngle = atan2f(vec.x,vec.y);
	D3DXMatrixRotationZ(&matRotZ,-fZAngle);

	float fXAngle = atan2f(sqrtf(vec.x*vec.x + vec.y*vec.y),vec.z);
	D3DXMatrixRotationX(&matRotX,-fXAngle);

	D3DXMatrixMultiply(&mat,&matRotX,&matRotZ);


//	D3DXMatrixRotationYawPitchRoll(&mat,vec.x * PIf / 2.0f,vec.z * PIf / 2.0f,0.0f);

//	D3DXVECTOR3 vecTest(0.0f,0.1f,0.9f);
//	D3DXVec3TransformCoord(&vecTest,&vecTest,&mat);

}

void CKinematics::InvalidateToolchangerKinematics()
{
	int nSide;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		break;
	case FB_TWIN_TOWER:
		for(nSide=0;nSide<2;nSide++) {
			switch(theApp.m_FBCtrl.m_nToolID[nSide]) {
			default: m_fProbeLength[nSide] = 0.0f;
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
				m_fProbeLength[nSide] = theApp.m_ToolChangers.getClampToolTipLength(theApp.m_FBCtrl.m_nToolID[nSide]);
				break;
			}

			switch(theApp.m_ToolChangers.getTypeFromID(theApp.m_FBCtrl.m_nToolID[0])) {
			case NOT_KNOWN:
			case SINGLE_PROBE:
			case COAXIAL_PROBES:
			case ADJACENT_PROBES:
			case PHASED_ARRAY_FLAT:
			case PHASED_ARRAY_CURVED:	theApp.m_LastSettings.nManipulatorType[0] = HORIZONTAL_WRIST;
				m_fXOffset[0] = 0.0f;
				break;
			case FINGER_MANIPULATOR:	theApp.m_LastSettings.nManipulatorType[0] = TWIN_TOWER_FINGER;
				m_fXOffset[0] = -46.0f;
				break;
			}
		}
		break;
	}
}

void CKinematics::ZeroFrame(int nSide)
{
	CString Buff, Msg;
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&CpSurface);

	Buff.LoadString(IDS_Is_the_ball_bearing_in_place);
	Msg.LoadString(IDS_Question);
	if (theApp.m_LastSettings.fZeroBallDiameter > 0.0f && MessageBox(NULL, Buff, Msg, MB_YESNO) == IDYES) {
		theApp.m_Tank.vFrameZero[nSide] -= CpSurface.Side[nSide].pt;
		CpSurface.Side[nSide].norm = (theApp.m_LastSettings.fZeroBallDiameter / -2.0f) * CpSurface.Side[nSide].norm;
		theApp.m_Tank.vFrameZero[nSide] -= (D3DXVECTOR3)CpSurface.Side[nSide].norm;
	}
	else {
		theApp.m_Tank.vFrameZero[nSide] -= CpSurface.Side[nSide].pt;
	}

}