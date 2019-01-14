// PropFile->cpp: implementation of the CProfile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "USLScanner.h"
#include "Profile.h"
#include "ProgressDlg.h"
#include "MainFrm.h"
#include "WayLinesEditDlg.h"
#include "CoordV3.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

UINT CalculateWayPointsThread(LPVOID pParam);

//#define	PERSPECTIVE_X_Z	0
//#define	PERSPECTIVE_X_Y	1
//#define	PERSPECTIVE_Y_Z	2

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProfile::CProfile()
{
	m_nFastAxis = theApp.m_LastSettings.nFastAxis;
	m_nSlowAxis = theApp.m_LastSettings.nSlowAxis;

	m_nTaughtLineL = 0;
	m_TaughtLine = NULL;

	m_nScanLineL = 0;
	m_ScanLine = NULL;

	m_nBrainLineL = 0;
	m_BrainLine = NULL;

	m_nEnvelopeLineL = 0;
	m_EnvelopeLine = NULL;

	m_nWayPointsL = 0;
	m_WayPoints = NULL;

	m_nSurfaceLineL = 0;
	m_SurfaceLine = NULL;

	for(int nn=0;nn<MAX_LINE_TYPES;nn++) {
		m_LineBlade[nn] = NULL;
		m_nLineBladeL[nn] = 0;
	};

	m_fFastIncrement = theApp.m_LastSettings.fFastIncrement;
	m_fSlowIncrement = theApp.m_LastSettings.fSlowIncrement;

	switch(theApp.m_nJobNumber) {
	default:
		m_fFastScanSize = theApp.m_LastSettings.fFastScanSize;
		m_fSlowScanSize = theApp.m_LastSettings.fSlowScanSize;
		m_nScanSide = theApp.m_LastSettings.nScanSide;
		m_fScanSpeed = theApp.m_LastSettings.fScanSpeed;
		m_nLineIncrement = 1;
		m_nLineIncrementMode = FIXED_INC;
		m_nRectilinearScanMode = theApp.m_LastSettings.m_nRectilinearScanMode;
		break;
	case GOODRICH_MACHINE_4:
		m_nLineIncrement = 1;
		m_nLineIncrementMode = FIXED_INC;
		m_fScanSpeed = 24.0f;
		m_nScanSide = 2;
		m_fComponentDiameter[1] = m_fComponentDiameter[0] = 3000.0f;
		break;
	}

	CopyGlobalUtToLocal();

	m_nWayPointsWithLiftOff = 0;
	m_nMeasuredRefPtsLocked = 0;
	m_bModified = FALSE;
	m_bCalculateWayPointsThreadEnabled = false;

}

CProfile::~CProfile()
{

	SAFE_DELETE_ARRAY( m_EnvelopeLine );
	SAFE_DELETE_ARRAY( m_TaughtLine );
	SAFE_DELETE_ARRAY( m_ScanLine );
	SAFE_DELETE_ARRAY( m_BrainLine );
	SAFE_DELETE_ARRAY( m_WayPoints );
	SAFE_DELETE_ARRAY( m_SurfaceLine );
	for(int nn=0;nn<MAX_LINE_TYPES;nn++) {
		SAFE_DELETE_ARRAY( m_LineBlade[nn] );
	};



}

void CProfile::Zero()
{
	m_bNeedsRendering = false;

	SAFE_DELETE_ARRAY(m_EnvelopeLine);
	SAFE_DELETE_ARRAY(m_TaughtLine);
	SAFE_DELETE_ARRAY(m_ScanLine);
	SAFE_DELETE_ARRAY(m_BrainLine);
	SAFE_DELETE_ARRAY(m_WayPoints);
	SAFE_DELETE_ARRAY( m_SurfaceLine );
	for(int nn=0;nn<MAX_LINE_TYPES;nn++) {
		SAFE_DELETE_ARRAY( m_LineBlade[nn] );
		m_nLineBladeL[nn] = 0;
	};
	m_nTaughtLineL = 0;
	m_nScanLineL = 0;
	m_nBrainLineL = 0;
	m_nEnvelopeLineL = 0;
	m_nWayPointsL=0;
	m_nSurfaceLineL = 0;
	m_nFlattenMode = 0;
	m_nWayPointNumber = theApp.m_LastSettings.nNumberOfWayPoints;
	m_nWaypointMode = theApp.m_LastSettings.nWaypointMode;
	m_fWaypointXYZChange = theApp.m_LastSettings.fWaypointXYZChange;
	m_fWaypointIJKChange = theApp.m_LastSettings.fWaypointIJKChange;
	m_nRUsed = 0;
	m_nBraneStyle = 0;
	m_nWayPointsWithLiftOff = 0;
	m_nBiUniDirectionalScan = 0;
	m_nScanResumeLine = 0;

	m_nComponentDescription = RECTILINEAR;

	m_bModified = FALSE;

	m_fRotateX = 0.0f;
	m_fRotateY = 0.0f;
	m_fRotateZ = 0.0f;
	m_fRotateR = 0.0f;
	m_fTranslateX = 0.0f;
	m_fTranslateY = 0.0f;
	m_fTranslateZ = 0.0f;
	D3DXMatrixIdentity(&m_matCadToScan);
	for(int nn=0;nn<4;nn++) D3DXMatrixIdentity(&m_matBladeLine[nn]);

	ZeroMemory(m_CpCadHole,sizeof m_CpCadHole);

	m_nFastAxis = theApp.m_LastSettings.nFastAxis;
	m_nSlowAxis = theApp.m_LastSettings.nSlowAxis;
	m_nNumberProbesPerImage = 1;
	m_nFirstProbe = 0;

	m_fFastIncrement = theApp.m_LastSettings.fFastIncrement;
	m_fSlowIncrement = theApp.m_LastSettings.fSlowIncrement;
	m_nNumberProbesPerImage = theApp.m_LastSettings.m_nNumberProbesPerImage;
	m_nFirstProbe = 0;

	switch(theApp.m_nJobNumber) {
	default:
		m_fFastScanSize = theApp.m_LastSettings.fFastScanSize;
		m_fSlowScanSize = theApp.m_LastSettings.fSlowScanSize;
		m_nScanSide = theApp.m_LastSettings.nScanSide;
		m_fScanSpeed = theApp.m_LastSettings.fScanSpeed;
		m_nRectilinearScanMode = theApp.m_LastSettings.m_nRectilinearScanMode;
		m_nLineIncrement = 1;
		m_nLineIncrementMode = FIXED_INC;
		break;
	case GOODRICH_MACHINE_4:
		m_nLineIncrement = 1;
		m_nLineIncrementMode = FIXED_INC;
		m_fScanSpeed = 30;
		m_nScanSide = 2;
		break;
	}

	m_vCloud.Zero();
	m_Edge.Zero();
	ZeroMemory(&m_BoreScanHeader, sizeof m_BoreScanHeader);

	CopyGlobalUtToLocal();

	ZeroMemory(m_vecBladeOffset, sizeof m_vecBladeOffset);

	m_vecToDangerPlane[0] = D3DXVECTOR3(-1.0f,0.0f,0.0f);
	m_vecToDangerPlane[1] = D3DXVECTOR3(1.0f,0.0f,0.0f);

	ZeroMemory(m_bExclusionZones, sizeof m_bExclusionZones);

	m_UltrasonicPathFileName.Empty();

	for (int nSide = 0; nSide < 2; nSide++) {
		switch (theApp.m_LastSettings.nDangerPlaneDir[nSide]) {
		case 0: m_vecToDangerPlane[nSide] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			break;
		case 1: m_vecToDangerPlane[nSide] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			break;
		case 2: m_vecToDangerPlane[nSide] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			break;
		}
	}

	m_vecAdditionalAngle[0] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	m_vecAdditionalAngle[1] = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	m_bUseAdditionalAngleDuringScanning[0] = m_bUseAdditionalAngleDuringScanning[1] = false;

}

void CProfile::operator = (CProfile* pProfile)
{
	int nLine,nn,gg;

	m_FileName = pProfile->m_FileName;

	m_fFastIncrement = pProfile->m_fFastIncrement;
	m_fSlowIncrement = pProfile->m_fSlowIncrement;
	CopyMemory(&m_nManipulatorType, pProfile->m_nManipulatorType, sizeof m_nManipulatorType);
	m_nComputeAlgorithm = pProfile->m_nComputeAlgorithm;
	m_nFastAxis = pProfile->m_nFastAxis;
	m_nPerpendicularStyle = pProfile->m_nPerpendicularStyle;
	m_fScanSpeed = pProfile->m_fScanSpeed;
	m_nSlowAxis = pProfile->m_nSlowAxis;
	m_fFastScanSize = pProfile->m_fFastScanSize;
	m_fSlowScanSize = pProfile->m_fSlowScanSize;
	m_nRectilinearScanMode = pProfile->m_nRectilinearScanMode;
	m_nRotaryScanMode = pProfile->m_nRotaryScanMode;
	m_nWaypointMode = pProfile->m_nWaypointMode;
	m_fWaypointXYZChange = pProfile->m_fWaypointXYZChange;
	m_fWaypointIJKChange = pProfile->m_fWaypointIJKChange;
	m_nWayPointNumber = pProfile->m_nWayPointNumber;
	CopyMemory(&m_nOriginOffsets, pProfile->m_nOriginOffsets, sizeof m_nOriginOffsets);
	CopyMemory(&m_nHeader, pProfile->m_nHeader, sizeof m_nHeader);
	m_nLineIncrement = pProfile->m_nLineIncrement;
	m_nLineIncrementMode = pProfile->m_nLineIncrementMode;
	m_nNumberProbesPerImage = pProfile->m_nNumberProbesPerImage;
	m_nFirstProbe = pProfile->m_nFirstProbe;
	CopyMemory(m_fComponentDiameter, pProfile->m_fComponentDiameter, sizeof m_fComponentDiameter);
	m_nCollectScanLines = pProfile->m_nCollectScanLines;
	m_nCollectSurface = pProfile->m_nCollectSurface;
	m_nFlattenMode = pProfile->m_nFlattenMode;
	m_matCadToScan = pProfile->m_matCadToScan;
	m_nRUsed = pProfile->m_nRUsed;
	m_nBraneStyle = pProfile->m_nBraneStyle;
	m_nComponentDescription = pProfile->m_nComponentDescription;
	m_nWayPointsWithLiftOff = pProfile->m_nWayPointsWithLiftOff;
	m_nBiUniDirectionalScan = pProfile->m_nBiUniDirectionalScan;
	m_nScanStartLine = pProfile->m_nScanStartLine;
	m_nScanFinishLine = pProfile->m_nScanFinishLine;
	m_nCollectAxis = pProfile->m_nCollectAxis;
	CopyMemory(m_vecBladeOffset, pProfile->m_vecBladeOffset, sizeof m_vecBladeOffset);
	CopyMemory(m_bExclusionZones, pProfile->m_bExclusionZones, sizeof m_bExclusionZones);

	m_strDescription = pProfile->m_strDescription;
	m_UltrasonicPathFileName = pProfile->m_UltrasonicPathFileName;


	m_nScanLineL = pProfile->m_nScanLineL;
	m_nTaughtLineL = pProfile->m_nTaughtLineL;
	m_nBrainLineL = pProfile->m_nBrainLineL;
	m_nEnvelopeLineL = pProfile->m_nEnvelopeLineL;

	if(m_nTaughtLineL) m_TaughtLine = new CPolyCoord[m_nTaughtLineL];
	for(nLine = 0;nLine < m_nTaughtLineL; nLine++) m_TaughtLine[nLine] = &pProfile->m_TaughtLine[nLine];

	if(m_nScanLineL) m_ScanLine = new CPolyCoord[m_nScanLineL];
	for(nLine = 0;nLine < m_nScanLineL; nLine++) m_ScanLine[nLine] = &pProfile->m_ScanLine[nLine];

	if(m_nBrainLineL) m_BrainLine = new CPolyCoord[m_nBrainLineL];
	for(nLine = 0;nLine < m_nBrainLineL; nLine++) m_BrainLine[nLine] = &pProfile->m_BrainLine[nLine];

	if(m_nEnvelopeLineL) m_EnvelopeLine = new CPolyCoord[m_nEnvelopeLineL];
	for(nLine = 0;nLine < m_nEnvelopeLineL; nLine++) m_EnvelopeLine[nLine] = &pProfile->m_EnvelopeLine[nLine];

	if(m_nSurfaceLineL) m_SurfaceLine = new CPolyCoord[m_nSurfaceLineL];
	for(nLine = 0;nLine < m_nSurfaceLineL; nLine++) m_SurfaceLine[nLine] = &pProfile->m_SurfaceLine[nLine];

	for(nn=0;nn<MAX_LINE_TYPES;nn++) {
		if(m_nLineBladeL[nn]) m_LineBlade[nn] = new CPolyCoord[m_nLineBladeL[nn]];
		for(nLine=0;nLine<m_nLineBladeL[nn];nLine++) m_LineBlade[nn][nLine] = &pProfile->m_LineBlade[nn][nLine];
	}

	for(nn=0;nn<3;nn++)	m_CpCadHole[nn] = pProfile->m_CpCadHole[nn];
	for(nn=0;nn<3;nn++)	m_CpMeasuredHole[nn] = pProfile->m_CpMeasuredHole[nn];

	m_CpHolding = pProfile->m_CpHolding;

	for(nn=0;nn<2;nn++)	m_CpRect[nn] = m_CpRect[nn];

	for(nn=0;nn<3;nn++) {
		for(gg=0;gg<2;gg++) {
			m_CpExclusion[nn][gg] = pProfile->m_CpExclusion[nn][gg];
		}
	}

	CopyMemory(&m_Global, &pProfile->m_Global, sizeof m_Global);
	for(int nTS=0;nTS<256;nTS++) {
		CopyMemory(&m_TS[nTS], &pProfile->m_TS[nTS], sizeof m_TS[0]);
	};
}

ULONGLONG CProfile::Save(CUSLFile *pFile)
{
	ULONGLONG nThumbNailOffset,nProfileDataOffset,nTaughtLineOffset,nScanLineOffset,nBrainLineOffset,nEnvelopeLineOffset,nStringsOffset;
	ULONGLONG	nHolesOffset,nHoldingFixtureOffset,nGlobalOffset,nTimeSlotOffset,nRectOffset,nExclusionOffset,nSurfaceLineOffset,nLineBladeOffset,nBoreScanOffset,nArmPositionOffset;
	ULONGLONG	nCloudOffset,nEdgeOffset;
	int	nLine,nn,gg;
	WCHAR Identifier[]={_T("Ultrasonic Sciences Ltd USLScanner Profile")};
	char cOffsetArray[2048];
	ZeroMemory(cOffsetArray,2048);
	CArchive* pArchFile;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;

	for(int nAxis = 0;nAxis<24;nAxis++) {
		m_nOriginOffsets[nAxis] = theApp.m_Axes[nAxis].nOffset;
	}

	m_FileName = pFile->GetFileName();

	ULONGLONG nStartOffset = pFile->GetPosition();

	pFile->Write(cOffsetArray,sizeof cOffsetArray);
	pFile->Seek(nStartOffset,CFile::begin);

	//Write the identifier
	pFile->Write(Identifier,sizeof Identifier);
	pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);

	//Write the thumbnail
	if(pFrame->m_p3DViewEditDlg) {
		pFile->Seek(nThumbNailOffset=pFile->GetPosition()+0x200,CFile::begin);
		pFrame->m_p3DViewEditDlg->SaveThumbNail(pFile);
	} else {
		nThumbNailOffset=0;
	}

	//write the structure
	pFile->Seek(nProfileDataOffset=pFile->GetPosition()+0x200,CFile::begin);
	pFile->Write(&m_fFastIncrement,sizeof m_fFastIncrement);
	pFile->Write(&m_fSlowIncrement,sizeof m_fSlowIncrement);
	pFile->Write(&m_nManipulatorType,sizeof m_nManipulatorType);
	pFile->Write(&m_nComputeAlgorithm,sizeof m_nComputeAlgorithm);
	pFile->Write(&m_nFastAxis,sizeof m_nFastAxis);
	pFile->Write(&m_nPerpendicularStyle,sizeof m_nPerpendicularStyle);
	pFile->Write(&m_fScanSpeed,sizeof m_fScanSpeed);
	pFile->Write(&m_nSlowAxis,sizeof m_nSlowAxis);
	pFile->Write(&m_fFastScanSize,sizeof m_fFastScanSize);
	pFile->Write(&m_fSlowScanSize,sizeof m_fSlowScanSize);
	pFile->Write(&m_nRectilinearScanMode,sizeof m_nRectilinearScanMode);
	pFile->Write(&m_nComponentDescription,sizeof m_nComponentDescription);
	pFile->Write(&m_nRotaryScanMode,sizeof m_nRotaryScanMode);
	pFile->Write(&theApp.m_Kinematics.m_fDesiredWaterPath,sizeof theApp.m_Kinematics.m_fDesiredWaterPath);
	pFile->Write(&m_nWaypointMode,sizeof m_nWaypointMode);
	pFile->Write(&m_fWaypointXYZChange,sizeof m_fWaypointXYZChange);
	pFile->Write(&m_fWaypointIJKChange,sizeof m_fWaypointIJKChange);
	pFile->Write(&m_nWayPointNumber,sizeof m_nWayPointNumber);
	pFile->Write(&m_nOriginOffsets,sizeof m_nOriginOffsets);
	pFile->Write(&m_nHeader,sizeof m_nHeader);
	pFile->Write(&m_nLineIncrement,sizeof m_nLineIncrement);
	pFile->Write(&m_nLineIncrementMode,sizeof m_nLineIncrementMode);
	pFile->Write(&m_nNumberProbesPerImage,sizeof m_nNumberProbesPerImage);
	pFile->Write(m_fComponentDiameter,sizeof m_fComponentDiameter);
	pFile->Write(&m_nCollectScanLines,sizeof m_nCollectScanLines);
	pFile->Write(&m_nCollectSurface,sizeof m_nCollectSurface);
	pFile->Write(&m_nFlattenMode,sizeof m_nFlattenMode);
	pFile->Write(&m_nRUsed,sizeof m_nRUsed);
	pFile->Write(&m_matCadToScan,sizeof m_matCadToScan);
	pFile->Write(&m_nBraneStyle, sizeof m_nBraneStyle);
	pFile->Write(&m_nFirstProbe, sizeof m_nFirstProbe);
	pFile->Write(&m_nWayPointsWithLiftOff, sizeof m_nWayPointsWithLiftOff);
	pFile->Write(m_vecToDangerPlane, sizeof m_vecToDangerPlane);
	pFile->Write(&m_nBiUniDirectionalScan, sizeof m_nBiUniDirectionalScan);
	pFile->Write(m_bExclusionZones, sizeof m_bExclusionZones);
	pFile->Write(&theApp.m_LastSettings.bUseBrainCompensation,sizeof theApp.m_LastSettings.bUseBrainCompensation);
	pFile->Write(&m_nScanStartLine,sizeof m_nScanStartLine);
	pFile->Write(&theApp.m_LastSettings.fBallDiameter,sizeof theApp.m_LastSettings.fBallDiameter);
	pFile->Write(&theApp.m_LastSettings.fBallHeight,sizeof theApp.m_LastSettings.fBallHeight);
	pFile->Write(theApp.m_Kinematics.m_fRobotWeightingX, sizeof theApp.m_Kinematics.m_fRobotWeightingX);
	pFile->Write(theApp.m_Kinematics.m_fRobotWeightingY, sizeof theApp.m_Kinematics.m_fRobotWeightingY);
	pFile->Write(theApp.m_Kinematics.m_fRobotWeightingZ, sizeof theApp.m_Kinematics.m_fRobotWeightingZ);
	pFile->Write(&m_bDiametersDervivedFromCoords, sizeof m_bDiametersDervivedFromCoords);
	pFile->Write(m_vecAdditionalAngle, sizeof m_vecAdditionalAngle);
	pFile->Write(m_bUseAdditionalAngleDuringScanning, sizeof m_bUseAdditionalAngleDuringScanning);

	//write strings
	pFile->Seek(nStringsOffset=pFile->GetPosition()+0x200,CFile::begin);
	pArchFile = new CArchive(pFile,CArchive::store);
	pArchFile->WriteString(m_strDescription+"\r\n");
	pArchFile->WriteString(m_UltrasonicPathFileName+"\r\n");
	
	pArchFile->Close();
	delete pArchFile;

	//Write the taughtlines
	pFile->Seek(nTaughtLineOffset=pFile->GetPosition()+0x200,CFile::begin);
	pFile->Write(&m_nTaughtLineL,sizeof m_nTaughtLineL);
	for(nLine = 0;nLine < m_nTaughtLineL; nLine++) m_TaughtLine[nLine].Save(pFile);

	//Write the scanlines
	pFile->Seek(nScanLineOffset=pFile->GetPosition()+0x200,CFile::begin);
	pFile->Write(&m_nScanLineL,sizeof m_nScanLineL);
	for(nLine = 0;nLine < m_nScanLineL; nLine++) m_ScanLine[nLine].Save(pFile);

	//Write the Brainlines
	pFile->Seek(nBrainLineOffset=pFile->GetPosition()+0x200,CFile::begin);
	pFile->Write(&m_nBrainLineL,sizeof m_nBrainLineL);
	for(nLine = 0;nLine < m_nBrainLineL; nLine++) m_BrainLine[nLine].Save(pFile);

	//Write the Envelopelines
	pFile->Seek(nEnvelopeLineOffset=pFile->GetPosition()+0x200,CFile::begin);
	pFile->Write(&m_nEnvelopeLineL,sizeof m_nEnvelopeLineL);
	for(nLine = 0;nLine < m_nEnvelopeLineL; nLine++) m_EnvelopeLine[nLine].Save(pFile);

	//Write the holes;
	int	nCadHoles=3;
	int	nMeasuredHoles=3;
	pFile->Seek(nHolesOffset=pFile->GetPosition()+0x200,CFile::begin);
	pFile->Write(&nCadHoles,sizeof nCadHoles);
	for (nn = 0; nn < nCadHoles; nn++)
		m_CpCadHole[nn].Save(pFile);

	pFile->Write(&nMeasuredHoles,sizeof nMeasuredHoles);
	for (nn = 0; nn < nMeasuredHoles; nn++)
		m_CpMeasuredHole[nn].Save(pFile);

	//Write Holding fixture
	pFile->Seek(nHoldingFixtureOffset=pFile->GetPosition()+0x200,CFile::begin);
	pFile->WriteStruct(&m_CpHolding,m_CpHolding.nStructSize = sizeof m_CpHolding);

	//Ultrasonics
	pFile->Seek(nGlobalOffset=pFile->GetPosition()+0x200,CFile::begin);
	pFile->WriteStruct(&theApp.m_UtUser.m_Global, theApp.m_UtUser.m_Global.nStructSize=sizeof theApp.m_UtUser.m_Global);

	pFile->Seek(nTimeSlotOffset=pFile->GetPosition()+0x200,CFile::begin);
	for(int nTS=0;nTS<256;nTS++) {
		pFile->WriteStruct(&theApp.m_UtUser.m_TS[nTS], theApp.m_UtUser.m_TS[nTS].nStructSize=sizeof theApp.m_UtUser.m_TS[0]);
	}

	pFile->Seek(nRectOffset=pFile->GetPosition()+0x200,CFile::begin);
	for(nn=0;nn<2;nn++)
		m_CpRect[nn].Save(pFile);

	pFile->Seek(nExclusionOffset=pFile->GetPosition()+0x200,CFile::begin);
	for(nn=0;nn<3;nn++) {
		for(gg=0;gg<2;gg++) {
			m_CpExclusion[nn][gg].Save(pFile);
		}
	}

	//Write the taughtlines
	pFile->Seek(nSurfaceLineOffset=pFile->GetPosition()+0x200,CFile::begin);
	pFile->Write(&m_nSurfaceLineL,sizeof m_nSurfaceLineL);
	for(nLine = 0;nLine < m_nSurfaceLineL; nLine++) m_SurfaceLine[nLine].Save(pFile);

	pFile->Seek(nCloudOffset=pFile->GetPosition()+0x200,CFile::begin);
	m_vCloud.Save(pFile);

	pFile->Seek(nEdgeOffset=pFile->GetPosition()+0x200,CFile::begin);
	m_Edge.Save(pFile);

	pFile->Seek(nLineBladeOffset=pFile->GetPosition()+0x200,CFile::begin);
	for(nn=0;nn<MAX_LINE_TYPES;nn++) {
		pFile->Write(&m_nLineBladeL[nn],sizeof m_nLineBladeL[nn]);
		for(nLine = 0;nLine < m_nLineBladeL[nn]; nLine++) m_LineBlade[nn][nLine].Save(pFile);
	}

	pFile->Seek(nBoreScanOffset=pFile->GetPosition()+0x200,CFile::begin);
	pFile->WriteStruct(&m_BoreScanHeader,m_BoreScanHeader.nStructSize = sizeof m_BoreScanHeader);

	pFile->Seek(nArmPositionOffset = (int)pFile->GetPosition() + 0x200, CFile::begin);
	pFile->Write(&theApp.m_nArmCount[1], sizeof theApp.m_nArmCount[1]);
	pFile->Write(theApp.m_Arm[1], sizeof theApp.m_Arm[0]);


	ULONGLONG nFinishOffset = pFile->GetPosition();

	//Write the offsets
	pFile->Seek(0x100 + nStartOffset,CFile::begin);
	pFile->Write(&nThumbNailOffset,sizeof nThumbNailOffset);
	pFile->Write(&nProfileDataOffset,sizeof nProfileDataOffset);
	pFile->Write(&nStringsOffset,sizeof nStringsOffset);
	pFile->Write(&nTaughtLineOffset,sizeof nTaughtLineOffset);
	pFile->Write(&nScanLineOffset,sizeof nScanLineOffset);
	pFile->Write(&nBrainLineOffset,sizeof nBrainLineOffset);
	pFile->Write(&nEnvelopeLineOffset,sizeof nEnvelopeLineOffset);
	pFile->Write(&nHolesOffset,sizeof nHolesOffset);
	pFile->Write(&nHoldingFixtureOffset,sizeof nHoldingFixtureOffset);
	pFile->Write(&nGlobalOffset,sizeof nGlobalOffset);
	pFile->Write(&nTimeSlotOffset,sizeof nTimeSlotOffset);
	pFile->Write(&nRectOffset,sizeof nRectOffset);
	pFile->Write(&nExclusionOffset,sizeof nExclusionOffset);
	pFile->Write(&nSurfaceLineOffset,sizeof nSurfaceLineOffset);
	pFile->Write(&nCloudOffset,sizeof nCloudOffset);
	pFile->Write(&nEdgeOffset,sizeof nEdgeOffset);
	pFile->Write(&nLineBladeOffset, sizeof nLineBladeOffset);
	pFile->Write(&nBoreScanOffset, sizeof nBoreScanOffset);
	pFile->Write(&nArmPositionOffset, sizeof nArmPositionOffset);

	pFile->Seek(nFinishOffset, CFile::begin);

	m_bModified = FALSE;
	return nStartOffset;
}


bool CProfile::Retrieve(CUSLFile *pFile)
{
	char aStr[70];
	ULONGLONG nThumbNailOffset,nProfileDataOffset,nTaughtLineOffset,nScanLineOffset,nBrainLineOffset,nEnvelopeLineOffset,nStringsOffset;
	ULONGLONG nHolesOffset,nHoldingFixtureOffset,nGlobalOffset,nTimeSlotOffset,nRectOffset,nExclusionOffset,nSurfaceLineOffset,nLineBladeOffset,nBoreScanOffset,nArmPositionOffset;
	ULONGLONG	nEdgeOffset,nCloudOffset;
	int	nLine,nn,gg;
	char aIdentifier[70]={"Ultrasonic Sciences Ltd USLScanner Profile"};
	char aDowtyIdentifier[70]={"Ultrasonic Sciences Ltd Dowty Propellers Profile"};
	WCHAR wIdentifier[]={_T("Ultrasonic Sciences Ltd USLScanner Profile")};
	WCHAR wStr[60];
	float fVersion,fWaterPath[8];
	CArchive* pArchFile;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	bool bOldCoordType = false;
	CCoord Cp;
	float fRobotWeightingX[2], fRobotWeightingY[2], fRobotWeightingZ[2];

	nArmPositionOffset = 0;

	ULONGLONG nStartOffset = pFile->GetPosition();

	ZeroMemory(wStr,sizeof wStr);
	pFile->Seek(nStartOffset,CFile::begin);
		
	pFile->Read(wStr,sizeof wIdentifier);
	pFile->Read(&fVersion,sizeof fVersion);
	pFile->m_fVersion = fVersion;
	if(wcscmp(wStr,wIdentifier)==0) {
		goto read_file;
	}

	ZeroMemory(aStr,sizeof aStr);
	pFile->Seek(nStartOffset,CFile::begin);
	pFile->Read(aStr,70);
	pFile->Read(&fVersion,sizeof fVersion);

	// look for legacy profiles

	if(strcmp(aDowtyIdentifier,aStr)==0) {
		ImportDowtyProfile(pFile);
		return false;
	}

	if(strcmp(aStr,aIdentifier)) {
		MessageBox(NULL,_T("Incompatible Profile type"),_T("Error"),MB_ICONERROR);
		return FALSE;
	}


read_file:

	Zero();

	m_FileName = pFile->GetFileName();

	pFile->Seek(nStartOffset + 0x100,CFile::begin);
	if (fVersion < 5.00f) {
		int nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nThumbNailOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nProfileDataOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nStringsOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nTaughtLineOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nScanLineOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nBrainLineOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nEnvelopeLineOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nHolesOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nHoldingFixtureOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nGlobalOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nTimeSlotOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nRectOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nExclusionOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nSurfaceLineOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nCloudOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nEdgeOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nLineBladeOffset = nOffset;
		pFile->Read(&nOffset, sizeof nOffset);	nBoreScanOffset = nOffset;

	}
	else {
		pFile->Read(&nThumbNailOffset, sizeof nThumbNailOffset);
		pFile->Read(&nProfileDataOffset, sizeof nProfileDataOffset);
		pFile->Read(&nStringsOffset, sizeof nStringsOffset);
		pFile->Read(&nTaughtLineOffset, sizeof nTaughtLineOffset);
		pFile->Read(&nScanLineOffset, sizeof nScanLineOffset);
		pFile->Read(&nBrainLineOffset, sizeof nBrainLineOffset);
		pFile->Read(&nEnvelopeLineOffset, sizeof nEnvelopeLineOffset);
		pFile->Read(&nHolesOffset, sizeof nHolesOffset);
		pFile->Read(&nHoldingFixtureOffset, sizeof nHoldingFixtureOffset);
		pFile->Read(&nGlobalOffset, sizeof nGlobalOffset);
		pFile->Read(&nTimeSlotOffset, sizeof nTimeSlotOffset);
		pFile->Read(&nRectOffset, sizeof nRectOffset);
		pFile->Read(&nExclusionOffset, sizeof nExclusionOffset);
		pFile->Read(&nSurfaceLineOffset, sizeof nSurfaceLineOffset);
		pFile->Read(&nCloudOffset, sizeof nCloudOffset);
		pFile->Read(&nEdgeOffset, sizeof nEdgeOffset);
		pFile->Read(&nLineBladeOffset, sizeof nLineBladeOffset);
		pFile->Read(&nBoreScanOffset, sizeof nBoreScanOffset);
		pFile->Read(&nArmPositionOffset, sizeof nArmPositionOffset);
	}

	if(nProfileDataOffset) {
		pFile->Seek(nProfileDataOffset,CFile::begin);
		pFile->Read(&m_fFastIncrement,sizeof m_fFastIncrement);
		pFile->Read(&m_fSlowIncrement,sizeof m_fSlowIncrement);
		pFile->Read(&m_nManipulatorType,sizeof m_nManipulatorType);
		pFile->Read(&m_nComputeAlgorithm,sizeof m_nComputeAlgorithm);
		pFile->Read(&m_nFastAxis,sizeof m_nFastAxis);
		pFile->Read(&m_nPerpendicularStyle,sizeof m_nPerpendicularStyle);
		pFile->Read(&m_fScanSpeed,sizeof m_fScanSpeed);
		pFile->Read(&m_nSlowAxis,sizeof m_nSlowAxis);
		pFile->Read(&m_fFastScanSize,sizeof m_fFastScanSize);
		pFile->Read(&m_fSlowScanSize,sizeof m_fSlowScanSize);
		pFile->Read(&m_nRectilinearScanMode,sizeof m_nRectilinearScanMode);
		pFile->Read(&m_nComponentDescription,sizeof m_nComponentDescription);
		pFile->Read(&m_nRotaryScanMode,sizeof m_nRotaryScanMode);
		pFile->Read(&fWaterPath,sizeof fWaterPath);
		pFile->Read(&m_nWaypointMode,sizeof m_nWaypointMode);
		pFile->Read(&m_fWaypointXYZChange,sizeof m_fWaypointXYZChange);
		pFile->Read(&m_fWaypointIJKChange,sizeof m_fWaypointIJKChange);
		pFile->Read(&m_nWayPointNumber,sizeof m_nWayPointNumber);
		pFile->Read(&m_nOriginOffsets,sizeof m_nOriginOffsets);
		pFile->Read(&m_nHeader,sizeof m_nHeader);
		pFile->Read(&m_nLineIncrement,sizeof m_nLineIncrement);
		pFile->Read(&m_nLineIncrementMode,sizeof m_nLineIncrementMode);
		pFile->Read(&m_nNumberProbesPerImage,sizeof m_nNumberProbesPerImage);
		pFile->Read(m_fComponentDiameter,sizeof m_fComponentDiameter);
		pFile->Read(&m_nCollectScanLines,sizeof m_nCollectScanLines);
		pFile->Read(&m_nCollectSurface,sizeof m_nCollectSurface);
		pFile->Read(&m_nFlattenMode,sizeof m_nFlattenMode);
		pFile->Read(&m_nRUsed,sizeof m_nRUsed);
		pFile->Read(&m_matCadToScan,sizeof m_matCadToScan);
		pFile->Read(&m_nBraneStyle, sizeof m_nBraneStyle);
		pFile->Read(&m_nFirstProbe, sizeof m_nFirstProbe);
		pFile->Read(&m_nWayPointsWithLiftOff, sizeof m_nWayPointsWithLiftOff);
		pFile->Read(m_vecToDangerPlane, sizeof m_vecToDangerPlane);
		pFile->Read(&m_nBiUniDirectionalScan, sizeof m_nBiUniDirectionalScan);
		pFile->Read(m_bExclusionZones, sizeof m_bExclusionZones);
		pFile->Read(&theApp.m_LastSettings.bUseBrainCompensation,sizeof theApp.m_LastSettings.bUseBrainCompensation);
		pFile->Read(&m_nScanStartLine,sizeof m_nScanStartLine);
		pFile->Read(&theApp.m_LastSettings.fBallDiameter,sizeof theApp.m_LastSettings.fBallDiameter);
		pFile->Read(&theApp.m_LastSettings.fBallHeight,sizeof theApp.m_LastSettings.fBallHeight);
		pFile->Read(fRobotWeightingX, sizeof fRobotWeightingX);
		pFile->Read(fRobotWeightingY, sizeof fRobotWeightingY);
		pFile->Read(fRobotWeightingZ, sizeof fRobotWeightingZ);
		pFile->Read(&m_bDiametersDervivedFromCoords, sizeof m_bDiametersDervivedFromCoords);
		pFile->Read(m_vecAdditionalAngle, sizeof m_vecAdditionalAngle);
		pFile->Read(m_bUseAdditionalAngleDuringScanning, sizeof m_bUseAdditionalAngleDuringScanning);

		//validate
		if(m_nRUsed!=1)
			m_nRUsed=0;

		if(theApp.m_LastSettings.nLoadWaterPathsFromProfile==TRUE)
			CopyMemory(theApp.m_Kinematics.m_fDesiredWaterPath,fWaterPath,sizeof fWaterPath);
		if(theApp.m_LastSettings.nScanMask & _LOAD_ROBOT_WEIGHTINGS_FROM_PROFILE) {
			CopyMemory(theApp.m_Kinematics.m_fRobotWeightingX, fRobotWeightingX, sizeof fRobotWeightingX);
			CopyMemory(theApp.m_Kinematics.m_fRobotWeightingY, fRobotWeightingY, sizeof fRobotWeightingY);
			CopyMemory(theApp.m_Kinematics.m_fRobotWeightingZ, fRobotWeightingZ, sizeof fRobotWeightingZ);

			for(int nSide =0; nSide < 2; nSide++) {
				theApp.m_LastSettings.vRobotWeighting[nSide].x = theApp.m_Kinematics.m_fRobotWeightingX[nSide];
				theApp.m_LastSettings.vRobotWeighting[nSide].y = theApp.m_Kinematics.m_fRobotWeightingY[nSide];
				theApp.m_LastSettings.vRobotWeighting[nSide].z = theApp.m_Kinematics.m_fRobotWeightingZ[nSide];
			}
		}
	}

	if(nStringsOffset) {
		pFile->Seek(nStringsOffset,CFile::begin);
		pArchFile = new CArchive(pFile,CArchive::load);
		pArchFile->ReadString(m_strDescription);
		pArchFile->ReadString(m_UltrasonicPathFileName);
		pArchFile->Close();
		delete pArchFile;
	}

	//Read the taughtlines
	if (fVersion < 5.00f) bOldCoordType = true;
	pFile->Seek(nTaughtLineOffset,CFile::begin);
	pFile->Read(&m_nTaughtLineL,sizeof m_nTaughtLineL);
	if(m_nTaughtLineL) m_TaughtLine = new CPolyCoord[m_nTaughtLineL];
	for(nLine = 0;nLine < m_nTaughtLineL; nLine++) m_TaughtLine[nLine].Retrieve(pFile, bOldCoordType);

	//Read the scanlines
	pFile->Seek(nScanLineOffset,CFile::begin);
	pFile->Read(&m_nScanLineL,sizeof m_nScanLineL);
	if(m_nScanLineL) m_ScanLine = new CPolyCoord[m_nScanLineL];
	for(nLine = 0;nLine < m_nScanLineL; nLine++) m_ScanLine[nLine].Retrieve(pFile, bOldCoordType);

	//Read the Brainlines
	pFile->Seek(nBrainLineOffset,CFile::begin);
	pFile->Read(&m_nBrainLineL,sizeof m_nBrainLineL);
	if(m_nBrainLineL) m_BrainLine = new CPolyCoord[m_nBrainLineL];
	for(nLine = 0;nLine < m_nBrainLineL; nLine++) m_BrainLine[nLine].Retrieve(pFile, bOldCoordType);

	//Read the envelopelines
	pFile->Seek(nEnvelopeLineOffset,CFile::begin);
	pFile->Read(&m_nEnvelopeLineL,sizeof m_nEnvelopeLineL);
	if(m_nEnvelopeLineL) m_EnvelopeLine = new CPolyCoord[m_nEnvelopeLineL];
	for(nLine = 0;nLine < m_nEnvelopeLineL; nLine++) m_EnvelopeLine[nLine].Retrieve(pFile, bOldCoordType);

	//Read the holes;
	int	nCadHoles;
	int	nMeasuredHoles;
	if (fVersion < 5.00f) {
		char Array[236];
		const COORDDATAV3 *pCpOld = (COORDDATAV3*)Array;
		pFile->Seek(nHolesOffset = pFile->GetPosition() + 0x200, CFile::begin);
		pFile->Read(&nCadHoles, sizeof nCadHoles);
		for (nn = 0; nn < nCadHoles; nn++) {
			pFile->Read(Array, 236);
			CopyMemory(&m_CpCadHole[nn].Side0, (PVOID)&pCpOld->Side0, sizeof m_CpCadHole[nn].Side0);
			CopyMemory(&m_CpCadHole[nn].Side1, (PVOID)&pCpOld->Side1, sizeof m_CpCadHole[nn].Side0);
		};

		pFile->Read(&nMeasuredHoles, sizeof nMeasuredHoles);
		for (nn = 0; nn < nMeasuredHoles; nn++) {
			if (m_nMeasuredRefPtsLocked == 1) {
				pFile->ReadStruct(&Cp, sizeof Cp);
			}
			else {
				pFile->Read(Array, 236);
				CopyMemory(&m_CpMeasuredHole[nn].Side0, (PVOID)&pCpOld->Side0, sizeof m_CpCadHole[nn].Side0);
				CopyMemory(&m_CpMeasuredHole[nn].Side1, (PVOID)&pCpOld->Side1, sizeof m_CpCadHole[nn].Side0);
			}
		}
	}
	else {
		pFile->Seek(nHolesOffset = pFile->GetPosition() + 0x200, CFile::begin);
		pFile->Read(&nCadHoles, sizeof nCadHoles);
		for (nn = 0; nn < nCadHoles; nn++)
			m_CpCadHole[nn].Retrieve(pFile);

		pFile->Read(&nMeasuredHoles, sizeof nMeasuredHoles);
		for (nn = 0; nn < nMeasuredHoles; nn++) {
			if (m_nMeasuredRefPtsLocked == 1) {
				Cp.Retrieve(pFile);
			}
			else {
				m_CpMeasuredHole[nn].Retrieve(pFile);
			}
		}
	}

	if (nHoldingFixtureOffset) {
		pFile->Seek(nHoldingFixtureOffset, CFile::begin);
		pFile->ReadStruct(&m_CpHolding, sizeof m_CpHolding);
	}

	if(nRectOffset) {
		pFile->Seek(nRectOffset,CFile::begin);
		for(nn=0;nn<2;nn++)
			m_CpRect[nn].Retrieve(pFile);
	}

	if(nExclusionOffset) {
		pFile->Seek(nExclusionOffset,CFile::begin);
		if (fVersion < 5.00f) {
			for (nn = 0; nn < 3; nn++) {
				for (gg = 0; gg < 2; gg++) {
					pFile->ReadStruct(&m_CpExclusion[nn][gg], sizeof m_CpExclusion[0][0]);
				}
			}
		}
		else {
			for (nn = 0; nn < 3; nn++) {
				for (gg = 0; gg < 2; gg++) {
					m_CpExclusion[nn][gg].Retrieve(pFile);
				}
			}
		}
	}
	//Read the surfacelines
	if(nSurfaceLineOffset) {
		pFile->Seek(nSurfaceLineOffset,CFile::begin);
		pFile->Read(&m_nSurfaceLineL,sizeof m_nSurfaceLineL);
		if(m_nSurfaceLineL) m_SurfaceLine = new CPolyCoord[m_nSurfaceLineL];
		for(nLine = 0;nLine < m_nSurfaceLineL; nLine++) m_SurfaceLine[nLine].Retrieve(pFile, bOldCoordType);
	}

	if(nCloudOffset>0) {
		pFile->Seek(nCloudOffset,CFile::begin);
		m_vCloud.Retrieve(pFile);
	}

	if(nEdgeOffset>0) {
		pFile->Seek(nEdgeOffset,CFile::begin);
		m_Edge.Retrieve(pFile);
	}

	if(nLineBladeOffset>0) {
		pFile->Seek(nLineBladeOffset, CFile::begin);
		for(int nn=0;nn<MAX_LINE_TYPES;nn++) {
			pFile->Read(&m_nLineBladeL[nn],sizeof m_nLineBladeL[nn]);
			if(m_nLineBladeL[nn]>0) {
				m_LineBlade[nn] = new CPolyCoord[m_nLineBladeL[nn]];
				for(nLine = 0;nLine < m_nLineBladeL[nn]; nLine++) m_LineBlade[nn][nLine].Retrieve(pFile, bOldCoordType);
			}
		}
	}
	if(nBoreScanOffset>0) {
		pFile->Seek(nBoreScanOffset, CFile::begin);
		pFile->ReadStruct(&m_BoreScanHeader,sizeof m_BoreScanHeader);
	};


	//Ultrasonics
	if((nGlobalOffset) && (nTimeSlotOffset)) {
		int nOldTriggerSource = theApp.m_UtUser.m_Global.nTriggerSource;
		pFile->Seek(nGlobalOffset,CFile::begin);
		pFile->ReadStruct(&m_Global, sizeof m_Global);
		if(fVersion<4.22f) {
			m_Global.LSA.nOldElementsInArray = 64;
			m_Global.LSA.fOldArrayPitch = 0.5f;
			m_Global.LSA.nOldStartElement = 0;
			m_Global.LSA.bOldReverseArrayOrder = false;
			ZeroMemory(&m_Global.Ext, sizeof m_Global.Ext);
		}

		pFile->Seek(nTimeSlotOffset,CFile::begin);
		int nMaxTS = 256;
		if(fVersion<4.22f) nMaxTS = 8;
		for(int nTS=0;nTS<nMaxTS;nTS++) {
			pFile->ReadStruct(&m_TS[nTS], sizeof m_TS[0]);
			if(fVersion < 4.22f) {
				m_TS[nTS].Pr30.nDACDeltaDelay = 0;
				m_TS[nTS].Pr30.nI2CUmbilicalSelect = 0;
				m_TS[nTS].Pr30.nRxRfUmbilicalSelect = 0;
				ZeroMemory(m_TS[nTS].Gate.nTimeStatus, sizeof m_TS[nTS].Gate.nTimeStatus);
				m_TS[nTS].Gate.nNsPeakWidth = 0;
			}
		}
		theApp.m_UtUser.m_Global.nTriggerSource = nOldTriggerSource;
	}

	if (nArmPositionOffset > 0 && nArmPositionOffset < pFile->GetLength() && fVersion >= 5.00f) {
		pFile->Seek(nArmPositionOffset, CFile::begin);
		pFile->Read(&theApp.m_nArmCount[1], sizeof theApp.m_nArmCount[0]);
		if (theApp.m_nArmCount[1] > 0 && theApp.m_nArmCount[1] < MAX_ARM_ENTRIES) {
			pFile->Read(theApp.m_Arm[1], sizeof theApp.m_Arm[0]);
		}
		else {
			ZeroMemory(theApp.m_Arm[1], sizeof theApp.m_Arm[0]);
		}
	}


	if(theApp.m_LastSettings.nLoadOriginFromProfile == TRUE) {
		if(MessageBox(NULL,_T("Would you like the origin modified to the one stored in the profile. You must\ncheck the XYZ Ball zero positions if you do"),_T("Origin prompt"),MB_YESNO) == IDYES) {

			theApp.m_Thread.StopCurrentThreadAction();
			theApp.m_Motors.SuspendMotors();

			theApp.m_Axes[theApp.m_Tank.nXBridge].nOffset = m_nOriginOffsets[theApp.m_Tank.nXBridge];
			theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset = m_nOriginOffsets[theApp.m_Tank.nXLeft];
			theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset = m_nOriginOffsets[theApp.m_Tank.nYLeft];
			theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset = m_nOriginOffsets[theApp.m_Tank.nZLeft];
			theApp.m_Axes[theApp.m_Tank.nXRight].nOffset = m_nOriginOffsets[theApp.m_Tank.nXRight];
			theApp.m_Axes[theApp.m_Tank.nYRight].nOffset = m_nOriginOffsets[theApp.m_Tank.nYRight];
			theApp.m_Axes[theApp.m_Tank.nZRight].nOffset = m_nOriginOffsets[theApp.m_Tank.nZRight];

			theApp.SaveTank();
			theApp.m_Thread.m_nThreadAction = JOYSTICK_START;
		}
	}

	ProfileDataValidationCheck();

	CalculateDangerZone();

	m_nScanFinishLine = m_nScanLineL-1;
	if(m_nLineIncrement<=0) m_nLineIncrement = 1;

	if (fVersion < 5.00f) {
		for (int nSide = 0; nSide < 2; nSide++) {
			if (m_vecToDangerPlane[nSide].x == 0.0f && m_vecToDangerPlane[nSide].y == 0.0f && m_vecToDangerPlane[nSide].z == 0.0f) {
				switch (theApp.m_LastSettings.nDangerPlaneDir[nSide]) {
				case 0: m_vecToDangerPlane[nSide] = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
					break;
				case 1: m_vecToDangerPlane[nSide] = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					break;
				case 2: m_vecToDangerPlane[nSide] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
					break;
				}
			}
		}
		for (int nTS = 0; nTS < 8; nTS++) {
			ZeroMemory(m_TS[nTS].Gate.nCollectPhase, sizeof theApp.m_UtUser.m_TS[nTS].Gate.nCollectPhase);
			m_TS[nTS].Adc.nAverages = 0;
			if (m_TS[nTS].Pr30.nMuxTx - m_TS[nTS].Pr30.nMuxRx) {
				m_TS[nTS].Adc.nAverages = 2;
			}
		}
		m_nWayPointNumber = theApp.m_LastSettings.nNumberOfWayPoints;
		m_nWaypointMode = theApp.m_LastSettings.nWaypointMode;
		m_fWaypointXYZChange = theApp.m_LastSettings.fWaypointXYZChange;
		m_fWaypointIJKChange = theApp.m_LastSettings.fWaypointIJKChange;

		if (PROFILE->m_nBrainLineL > 0) 
			theApp.m_LastSettings.bUseBrainCompensation = 1;

	}

	setNeedsRendering(true);
	return TRUE;

}


void CProfile::ImportAptSourceSimpleRaster(CString FileName)
{
	
	CUSLFile file;
	CArchive* pArchFile;
	CString	Buff;
	int	nn,ii,jj,nLength,nPtr;
	char	str[30];
	char	cV;
	int	nCoordL=0;
	COORDDATA Cp;

	if(!file.Open(FileName,CFile::modeRead|CFile::typeBinary)) {

	} else {
		ULONGLONG nFileLength = file.GetLength();
		pArchFile = new CArchive(&file,CArchive::load);
		Zero();
		while(pArchFile->ReadString(Buff)) {
			if(Buff.Find(_T("GOTO"),0) != -1) nCoordL++;
		}
		pArchFile->Close();
		delete pArchFile;

		m_TaughtLine = new CPolyCoord[m_nTaughtLineL = 1];

		m_TaughtLine[0].AllocateCoords(nCoordL);

//		CProgressDlg dlg(NULL);
//		dlg.Create(CProgressDlg::IDD, NULL);
//		dlg.SetProgressRange(0,nCoordL);

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);
		nPtr=0;
		while(pArchFile->ReadString(Buff)) {
			if(((nn=Buff.Find(_T("GOTO"),0)) != -1) && (nPtr<nCoordL)){
				nLength=Buff.GetLength();
//				dlg.SetProgressPos(nCoordL);
				Cp.Zero();

				for(ii=7,jj=0;(cV=(char)Buff.GetAt(ii))!=',';ii++,jj++) str[jj]=cV;
				str[jj]=0;
				Cp.Side0.fX=(float)atof(str);

				for(++ii,jj=0;(cV= (char)Buff.GetAt(ii))!=',';ii++,jj++) str[jj]=cV;
				str[jj]=0;
				Cp.Side0.fY=(float)atof(str);

				for(++ii,jj=0;(cV= (char)Buff.GetAt(ii))!=',';ii++,jj++) str[jj]=cV;
				str[jj]=0;
				Cp.Side0.fZ=(float)atof(str);

				for(++ii,jj=0;(cV= (char)Buff.GetAt(ii))!=',';ii++,jj++) str[jj]=cV;
				str[jj]=0;
				Cp.Side0.fI=(float)atof(str);

				for(++ii,jj=0;(cV= (char)Buff.GetAt(ii))!=',';ii++,jj++) str[jj]=cV;
				str[jj]=0;
				Cp.Side0.fJ=(float)atof(str);

				for(++ii,jj=0;(ii<nLength) && (cV= (char)Buff.GetAt(ii));ii++,jj++) str[jj]=cV;
				str[jj]=0;
				Cp.Side0.fK=(float)atof(str);

				if(theApp.m_LastSettings.nImportAptUnits==1) {
					Cp.Side0.fX *= MMPERINCH;
					Cp.Side0.fY *= MMPERINCH;
					Cp.Side0.fZ *= MMPERINCH;
				}

				Cp.CopySides(1,0);
				Cp.InvertNorm(STARBOARD);

				m_TaughtLine[0].m_pCp[nPtr++] = Cp;

				nCoordL++;
			}
		}


		pArchFile->Close();
		file.Close();
		delete pArchFile;
		m_bModified = TRUE;

	}
}

void CProfile::ImportAptSourceBAESamlesburyFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType)
{
	
	CUSLFile file;
	CArchive* pArchFile;
	CString	Buff;
	ULONGLONG	nTotalBytesRead;
	int nCoord;
	int nBytesRead;
	COORDDATA	Cp;
	Cp.Zero();
	int nPos,nOldPos=0;

	if(!file.Open(FileName,CFile::modeRead|CFile::typeBinary)) {

	} else {
		ULONGLONG nFileLength = file.GetLength();
		pArchFile = new CArchive(&file,CArchive::load);
		Zero();


		//Find number of scan Lines
		pbuttonHowManyLines->SetCheck(1);
		m_nScanLineL=0;
		nBytesRead=pArchFile->ReadString(Buff);
		nTotalBytesRead=Buff.GetLength();
		while(nBytesRead) {
			if(Buff.Find(_T("RAPID"),0)==0) {
				nBytesRead=pArchFile->ReadString(Buff);
				nTotalBytesRead+=Buff.GetLength();;
				if(Buff.Find(_T("RAPID"),0)==0) {
					m_nScanLineL++;
				}
			}
			nBytesRead=pArchFile->ReadString(Buff);
			nTotalBytesRead+=Buff.GetLength();

			nPos = (int)MulDiv(nTotalBytesRead,(ULONGLONG)100,nFileLength);
			if(nPos-nOldPos)
				pProgress->SetPos(nOldPos = nPos);
		}

		pArchFile->Close();
		delete pArchFile;

		if(m_nScanLineL<=0) {
			file.Close();
			return;
		}

		delete [] m_ScanLine;
		m_ScanLine = new CPolyCoord[m_nScanLineL];

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);

		//read in coords and make lines
		pbuttonReadCoordinates->SetCheck(1);
		int nLine=0;
		nBytesRead=pArchFile->ReadString(Buff);
		while(nBytesRead) {
			if(Buff.Find(_T("RAPID"),0)==0) {
				nBytesRead=pArchFile->ReadString(Buff);
				if(Buff.Find(_T("RAPID"),0)==0) {
					nCoord=0;

					nPos = MulDiv(nLine,100,m_nScanLineL);
					if(nPos-nOldPos)
						pProgress->SetPos(nOldPos = nPos);

					nBytesRead=pArchFile->ReadString(Buff);
					while(Buff.Find(_T("GOTO"),0)==0) {

						DecodeGotoLine(Buff,&Cp);

						m_ScanLine[nLine] + (Cp);
						nBytesRead=pArchFile->ReadString(Buff);
					}
					nLine++;
				}
			} else {
				nBytesRead=pArchFile->ReadString(Buff);
			}
		}

		pArchFile->Close();
		delete pArchFile;

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);

		//read in hole coordinates
		pbuttonReadingHoles->SetCheck(1);
		pProgress->SetPos(0);

		int	nHole = 0;
		nBytesRead=pArchFile->ReadString(Buff);
		switch(nHoleType) {
		case 0:
			while(nBytesRead>0) {
				if(Buff.Find(_T("CYCLE/DRILL"),0)==0) {
					nBytesRead=pArchFile->ReadString(Buff);
					while(Buff.Find(_T("GOTO"),0)==0) {
						pProgress->SetPos(MulDiv(nHole+1,100,3));

						ZeroMemory(&Cp,sizeof Cp);

						DecodeGotoLine(Buff,&Cp);

						if(nHole<3) {
							m_CpCadHole[nHole] = Cp;
							nHole++;
						}
						nBytesRead=pArchFile->ReadString(Buff);
					}

				} else {
					nBytesRead=pArchFile->ReadString(Buff);
				}
			}
			break;
		case 1:
			while((nBytesRead>0) && (Buff.Find(_T("$$  Start generation of : Referentie1"),0)!=0)){
				nBytesRead=pArchFile->ReadString(Buff);
			}
			while(nBytesRead>0) {
				nBytesRead=pArchFile->ReadString(Buff);
				if(Buff.Find(_T("GOTO"),0)==0) {
					pProgress->SetPos(MulDiv(nHole+1,100,3));

					ZeroMemory(&Cp,sizeof Cp);

					DecodeGotoLine(Buff,&Cp);

					if(nHole<3) {
						m_CpCadHole[nHole] = Cp;
						nHole++;
					}
				}
			}
			break;
		}

		pArchFile->Close();
		delete pArchFile;

		file.Close();

		if(theApp.m_LastSettings.nImportAptDirection == 1) {
			ReverseLines(SCANLINES,2);
		}

		if(theApp.m_LastSettings.nReverseLineOrder == 1) {
			ReverseScanLineOrder();
		}

		m_bModified = TRUE;
		if(theApp.m_LastSettings.nPlaceComponentInScanner) {
			if(nHole>0) {
				GetTheProfileInTheScanner(0);
			} else {
				m_ScanLine[0].GetCoord(0,&Cp);
				GetTheProfileInTheScanner(1,&Cp);
			}
		}

	}
}

void CProfile::ImportAptSourceFokkerDoubleRapidFormat(CString FileName)
{
	
	CUSLFile file;
	CArchive* pArchFile;
	CString	Buff;
	ULONGLONG	nTotalBytesRead;
	int nPos;
	int nBytesRead;
	COORDDATA	Cp;
	Cp.Zero();

	if(!file.Open(FileName,CFile::modeRead|CFile::typeBinary)) {

	} else {
		m_FileName = file.GetFileTitle();
		if((nPos = m_FileName.ReverseFind('.')) >0) {
			m_FileName.GetBufferSetLength(nPos);
		}
		m_strDescription = m_FileName;

		ULONGLONG nFileLength = file.GetLength();
		pArchFile = new CArchive(&file,CArchive::load);
		Zero();

		CProgressDlg dlg(NULL);
		dlg.Create(CProgressDlg::IDD, NULL);
		dlg.SetProgressRange(0,100);
		dlg.SetProgressText(L"Finding number of Lines");

		//Find number of scan Lines
		m_nScanLineL=0;
		nBytesRead=pArchFile->ReadString(Buff);
		nTotalBytesRead=Buff.GetLength();
		while(nBytesRead) {
			if(Buff.Find(_T("RAPID"),0)==0) {
				m_nScanLineL++;
			}
			nBytesRead=pArchFile->ReadString(Buff);
			nTotalBytesRead+=Buff.GetLength();

			nPos = (int)MulDiv(nTotalBytesRead,(ULONGLONG)100,nFileLength);
			dlg.SetProgressPos(nPos);
		}

		pArchFile->Close();
		delete pArchFile;

		m_nScanLineL/=2;
		if(m_nScanLineL<=0) {
			file.Close();
			return;
		}

		delete [] m_ScanLine;
		m_ScanLine = new CPolyCoord[m_nScanLineL];

		dlg.SetProgressRange(0,m_nScanLineL);
		dlg.SetProgressText(L"Generating scan lines");

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);

		//read in coords and make lines
		int nLine=0;
		nBytesRead=pArchFile->ReadString(Buff);
		while(nBytesRead) {
find_next_line:
			if(Buff.Find(_T("RAPID"),0)==0) {
				dlg.SetProgressPos(nLine);

				while(nBytesRead=pArchFile->ReadString(Buff)) {
					if(Buff.Find(_T("RAPID"),0)==0) {
						nBytesRead=pArchFile->ReadString(Buff);
						nLine++;
						goto find_next_line;
					}
					if(Buff.Find(_T("GOTO"),0)==0) {
						DecodeGotoLine(Buff,&Cp);
						m_ScanLine[nLine] + (Cp);
					}
				}
			}
			nBytesRead=pArchFile->ReadString(Buff);
		}

		pArchFile->Close();
		delete pArchFile;

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);

		//read in hole coordinates
		dlg.SetProgressRange(0,3);
		dlg.SetProgressText(L"Finding Holes & Frame");
		dlg.SetProgressPos(0);

		int	nHole = 0;
		m_CpCadHole[0]=m_ScanLine[0].m_pCp[0];
		m_CpCadHole[1]=m_ScanLine[0].m_pCp[0];
		m_CpCadHole[2]=m_ScanLine[0].m_pCp[0];

		nBytesRead=pArchFile->ReadString(Buff);
		while(nBytesRead>0) {
			if(Buff.Find(_T("CYCLE/DRILL"),0)==0) {
				nBytesRead=pArchFile->ReadString(Buff);
				while(Buff.Find(_T("GOTO"),0)==0) {
					dlg.SetProgressPos(nHole+1);

					ZeroMemory(&Cp,sizeof Cp);

					DecodeGotoLine(Buff,&Cp);

					if(nHole<3) {
						m_CpCadHole[nHole] = Cp;
						nHole++;
					}
					nBytesRead=pArchFile->ReadString(Buff);
				}
			} else {
				nBytesRead=pArchFile->ReadString(Buff);
			}
		}

		pArchFile->Close();
		delete pArchFile;

		file.Close();

		if(theApp.m_LastSettings.nImportAptDirection == 1) {
			ReverseLines(SCANLINES,2);
		}


		GetTheProfileInTheScanner();
		m_bModified = TRUE;

	}
}

void CProfile::ImportAptSourceFokkerSingleRapidFormat(CString FileName)
{
	
	CUSLFile file;
	CArchive* pArchFile;
	CString	Buff;
	ULONGLONG	nTotalBytesRead;
	int nPos;
	int nBytesRead;
	int nStartFound=0;
	COORDDATA	Cp;
	Cp.Zero();

	if(!file.Open(FileName,CFile::modeRead|CFile::typeBinary)) {

	} else {
		Zero();

		m_FileName = file.GetFileTitle();
		if((nPos = m_FileName.ReverseFind('.')) >0) {
			m_FileName.GetBufferSetLength(nPos);
		}
		m_strDescription = m_FileName;

		ULONGLONG nFileLength = file.GetLength();
		pArchFile = new CArchive(&file,CArchive::load);

		CProgressDlg dlg(NULL);
		dlg.Create(CProgressDlg::IDD, NULL);
		dlg.SetProgressRange(0,100);
		dlg.SetProgressText(L"Finding number of Lines");

		//Find number of scan Lines
		m_nScanLineL=0;
		nBytesRead=pArchFile->ReadString(Buff);
		nTotalBytesRead=Buff.GetLength();
		while(nBytesRead) {
			if(Buff.Find(_T("FEDRAT"),0)==0) {
				nStartFound=1;
			}
			if((Buff.Find(_T("RAPID"),0)==0) && (nStartFound==1)) {
				m_nScanLineL++;
			}
			if((Buff.Find(_T("Drilling"),0)>0) && (nStartFound==1)) {
				m_nScanLineL--;
				goto finished;
			}
			nBytesRead=pArchFile->ReadString(Buff);
			nTotalBytesRead+=Buff.GetLength();

			nPos = (int)MulDiv(nTotalBytesRead,(ULONGLONG)100,nFileLength);
			dlg.SetProgressPos(nPos);
		}
finished:

		pArchFile->Close();
		delete pArchFile;

		if(m_nScanLineL<=0) {
			file.Close();
			return;
		}

		delete [] m_ScanLine;
		m_ScanLine = new CPolyCoord[m_nScanLineL];

		dlg.SetProgressRange(0,m_nScanLineL);
		dlg.SetProgressText(L"Generating scan lines");

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);

		//read in coords and make lines
		int nLine=0;
		nStartFound=0;
		nBytesRead=pArchFile->ReadString(Buff);
		while(nBytesRead) {
			dlg.SetProgressPos(nLine);

			while(nBytesRead=pArchFile->ReadString(Buff)) {
				if(Buff.Find(_T("FEDRAT"),0)==0) {
					nStartFound++;
				}
				if(nStartFound==1) {
					if(Buff.Find(_T("RAPID"),0)==0) {
						nBytesRead=pArchFile->ReadString(Buff);
						nLine++;
					}
					if((Buff.Find(_T("GOTO"),0)==0) && (nLine<m_nScanLineL)) {
						DecodeGotoLine(Buff,&Cp);
						m_ScanLine[nLine] + (Cp);
					}
				}
				if(nStartFound>1) {
					if(Buff.Find(_T("FEDRAT"),0)==0) {
						nBytesRead=pArchFile->ReadString(Buff);
						DecodeGotoLine(Buff,&Cp);

						switch(nStartFound) {
						case 2:	m_CpCadHole[0] = Cp;
							break;
						case 3:	m_CpCadHole[1] = Cp;
							break;
						case 4:	m_CpCadHole[2] = Cp;
							break;
						}
					}
				}
			}
		}

		pArchFile->Close();
		delete pArchFile;


		file.Close();

		if(theApp.m_LastSettings.nImportAptDirection == 1) {
			ReverseLines(SCANLINES,2);
		}


		GetTheProfileInTheScanner();
		m_bModified = TRUE;

	}
}

///////////////////////////////////////////////////////////
// Hafei Format
// Single RAPID
// Holes start with the word HOLES
//////////////////////////////////////////////////////////
void CProfile::ImportAptSourceHafeiFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType)
{
	
	CUSLFile file;
	CArchive* pArchFile;
	CString	Buff;
	ULONGLONG	nTotalBytesRead;
	int nCoord;
	int nBytesRead;
	COORDDATA	Cp;
	Cp.Zero();
	int nPos,nOldPos=0;

	if(!file.Open(FileName,CFile::modeRead|CFile::typeBinary)) {

	} else {
		ULONGLONG nFileLength = file.GetLength();
		pArchFile = new CArchive(&file,CArchive::load);
		Zero();


		//Find number of scan Lines
		pbuttonHowManyLines->SetCheck(1);
		m_nScanLineL=0;
		nBytesRead=pArchFile->ReadString(Buff);
		nTotalBytesRead=Buff.GetLength();
		while(nBytesRead) {
			if(Buff.Find(_T("RAPID"),0)==0) {
				m_nScanLineL++;
			}
			nBytesRead=pArchFile->ReadString(Buff);
			nTotalBytesRead+=Buff.GetLength();

			nPos = (int)MulDiv(nTotalBytesRead,(ULONGLONG)100,nFileLength);
			if(nPos-nOldPos)
				pProgress->SetPos(nOldPos = nPos);
		}
		m_nScanLineL--;  //this is here because there is an extra RAPID

		pArchFile->Close();
		delete pArchFile;

		if(m_nScanLineL<=0) {
			file.Close();
			return;
		}

		delete [] m_ScanLine;
		m_ScanLine = new CPolyCoord[m_nScanLineL];

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);

		//read in coords and make lines
		pbuttonReadCoordinates->SetCheck(1);
		int nLine=0;
		nBytesRead=pArchFile->ReadString(Buff);
		while(nBytesRead) {
			if(Buff.Find(_T("RAPID"),0)==0) {
				nCoord=0;

				nPos = MulDiv(nLine,100,m_nScanLineL);
				if(nPos-nOldPos)
					pProgress->SetPos(nOldPos = nPos);

				nBytesRead=pArchFile->ReadString(Buff);
				while(Buff.Find(_T("GOTO"),0)==0) {

					DecodeGotoLine(Buff,&Cp);

					m_ScanLine[nLine] + (Cp);
					nBytesRead=pArchFile->ReadString(Buff);
				}
				nLine++;
			} else {
				nBytesRead=pArchFile->ReadString(Buff);
			}
		}

		pArchFile->Close();
		delete pArchFile;

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);

		//read in hole coordinates
		pbuttonReadingHoles->SetCheck(1);
		pProgress->SetPos(0);

		int	nHole = 0;
		nBytesRead=pArchFile->ReadString(Buff);

		while((nBytesRead>0) && (Buff.Find(_T("HOLES"),0)!=0)){
			nBytesRead=pArchFile->ReadString(Buff);
		}
		while(nBytesRead>0) {
			nBytesRead=pArchFile->ReadString(Buff);
			if(Buff.Find(_T("GOTO"),0)==0) {
				pProgress->SetPos(MulDiv(nHole+1,100,3));
				ZeroMemory(&Cp,sizeof Cp);

				DecodeGotoLine(Buff,&Cp);

				if(nHole<3) {
					m_CpCadHole[nHole] = Cp;
					nHole++;
				}
			}
		}

		pArchFile->Close();
		delete pArchFile;

		file.Close();

		if(theApp.m_LastSettings.nImportAptDirection == 1) {
			ReverseLines(SCANLINES,2);
		}

		if(theApp.m_LastSettings.nReverseLineOrder == 1) {
			ReverseScanLineOrder();
		}

		m_bModified = TRUE;
		if(theApp.m_LastSettings.nPlaceComponentInScanner) {
			if(nHole>0) {
				GetTheProfileInTheScanner(0);
			} else {
				m_ScanLine[0].GetCoord(0,&Cp);
				GetTheProfileInTheScanner(1,&Cp);
			}
		}

	}
}

void CProfile::AddScanLine(CPolyCoord *pPolyLine)
{

	CPolyCoord* pLine = NULL;
	int	nLine;

	if((m_ScanLine!=NULL) && (m_nScanLineL>0)) {
		pLine = new CPolyCoord[m_nScanLineL];
		for(nLine=0;nLine<m_nScanLineL;nLine++) {
			pLine[nLine] = &m_ScanLine[nLine];
		}
		delete [] m_ScanLine;
		m_nScanLineL++;
		m_ScanLine = new CPolyCoord[m_nScanLineL];
		for(nLine=0;nLine<(m_nScanLineL-1);nLine++) {
			m_ScanLine[nLine] = &pLine[nLine];
		}
		m_ScanLine[nLine] = pPolyLine;
		delete [] pLine;
	} else {
		m_nScanLineL=1;
		m_ScanLine = new CPolyCoord[m_nScanLineL];
		m_ScanLine[0] = pPolyLine;
	}

	m_bModified=TRUE;

}

void CProfile::AddTaughtLine(CPolyCoord *pPolyLine)
{
	CPolyCoord* pLine = NULL;
	int	nLine;

	if((m_TaughtLine!=NULL) && (m_nTaughtLineL>0)) {
		pLine = new CPolyCoord[m_nTaughtLineL];
		for(nLine=0;nLine<m_nTaughtLineL;nLine++) {
			pLine[nLine] = &m_TaughtLine[nLine];
		}
		delete [] m_TaughtLine;
		m_nTaughtLineL++;
		m_TaughtLine = new CPolyCoord[m_nTaughtLineL];
		for(nLine=0;nLine<(m_nTaughtLineL-1);nLine++) {
			m_TaughtLine[nLine] = &pLine[nLine];
		}
		m_TaughtLine[nLine] = pPolyLine;
		delete [] pLine;
	} else {
		m_nTaughtLineL=1;
		m_TaughtLine = new CPolyCoord[m_nTaughtLineL];
		m_TaughtLine[0] = pPolyLine;
	}

	m_bModified=TRUE;

}

void CProfile::AddBraneLine(CPolyCoord *pPolyLine)
{
	CPolyCoord* pLine = NULL;
	int	nLine;

	if((m_BrainLine!=NULL) && (m_nBrainLineL>0)) {
		pLine = new CPolyCoord[m_nBrainLineL];
		for(nLine=0;nLine<m_nBrainLineL;nLine++) {
			pLine[nLine] = &m_BrainLine[nLine];
		}
		delete [] m_BrainLine;
		m_nBrainLineL++;
		m_BrainLine = new CPolyCoord[m_nBrainLineL];
		for(nLine=0;nLine<(m_nBrainLineL-1);nLine++) {
			m_BrainLine[nLine] = &pLine[nLine];
		}
		m_BrainLine[nLine] = pPolyLine;
		delete [] pLine;
	} else {
		m_nBrainLineL=1;
		m_BrainLine = new CPolyCoord[m_nBrainLineL];
		m_BrainLine[0] = pPolyLine;
	}

	m_bModified=TRUE;

}

int CProfile::GetScanLineCoordL(int nLine)
{

	if(nLine<m_nScanLineL) {
		return m_ScanLine[nLine].m_nCoordL;
	}
	return 0;
}

void CProfile::GetScanLineCoord(int nLine, int nIndex, COORDDATA *pCp)
{
	if(nLine<m_nScanLineL) {
		if(nIndex<m_ScanLine[nLine].m_nCoordL) {
			CopyMemory(pCp,&m_ScanLine[nLine].m_pCp[nIndex],sizeof COORDDATA);
		}
	}
}


void CProfile::FindLineLimits(CPolyCoord* pLine,int nNumberLines,COORDDATA *pCp0, COORDDATA *pCp1)
{
	if((pLine) && (nNumberLines>0)) {
		CopyMemory(pCp0, &pLine[0].m_pCp[0],sizeof COORDDATA);
		CopyMemory(pCp1, &pLine[0].m_pCp[0],sizeof COORDDATA);
		for(int nLine = 0;nLine<nNumberLines;nLine++) {
			pLine[nLine].FindLimits(pCp0,pCp1,FALSE);
		}
	}
}

void CProfile::FindAllLineLimits(COORDDATA *pCp0, COORDDATA *pCp1)
{
	bool bInitialized = FALSE;

	if((m_TaughtLine) && (m_nTaughtLineL>0)) {
		if(bInitialized==FALSE) {
			CopyMemory(pCp0, &m_TaughtLine[0].m_pCp[0],sizeof COORDDATA);
			CopyMemory(pCp1, &m_TaughtLine[0].m_pCp[0],sizeof COORDDATA);
		}
		bInitialized=TRUE;
		for(int nLine = 0;nLine<m_nTaughtLineL;nLine++) {
			m_TaughtLine[nLine].FindLimits(pCp0,pCp1,FALSE);
		}
	}
	if((m_ScanLine) && (m_nScanLineL>0)) {
		if(bInitialized==FALSE) {
			CopyMemory(pCp0, &m_ScanLine[0].m_pCp[0],sizeof COORDDATA);
			CopyMemory(pCp1, &m_ScanLine[0].m_pCp[0],sizeof COORDDATA);
		}
		bInitialized=TRUE;
		for(int nLine = 0;nLine<m_nScanLineL;nLine++) {
			m_ScanLine[nLine].FindLimits(pCp0,pCp1,FALSE);
		}
	}
	if((m_BrainLine) && (m_nBrainLineL>0)) {
		if(bInitialized==FALSE) {
			CopyMemory(pCp0, &m_BrainLine[0].m_pCp[0],sizeof COORDDATA);
			CopyMemory(pCp1, &m_BrainLine[0].m_pCp[0],sizeof COORDDATA);
		}
		bInitialized=TRUE;
		for(int nLine = 0;nLine<m_nBrainLineL;nLine++) {
			m_BrainLine[nLine].FindLimits(pCp0,pCp1,FALSE);
		}
	}
	if((m_EnvelopeLine) && (m_nEnvelopeLineL>0)) {
		if(bInitialized==FALSE) {
			CopyMemory(pCp0, &m_EnvelopeLine[0].m_pCp[0],sizeof COORDDATA);
			CopyMemory(pCp1, &m_EnvelopeLine[0].m_pCp[0],sizeof COORDDATA);
		}
		bInitialized=TRUE;
		for(int nLine = 0;nLine<m_nEnvelopeLineL;nLine++) {
			m_EnvelopeLine[nLine].FindLimits(pCp0,pCp1,FALSE);
		}
	}
	for(int nSide=0;nSide<2;nSide++) {
		if((m_LineBlade[nSide]) && (m_nLineBladeL[nSide]>0)) {
			if(bInitialized==FALSE) {
				CopyMemory(pCp0, &m_LineBlade[nSide][0].m_pCp[0],sizeof COORDDATA);
				CopyMemory(pCp1, &m_LineBlade[nSide][0].m_pCp[0],sizeof COORDDATA);
			}
			bInitialized=TRUE;
			for(int nLine = 0;nLine<m_nLineBladeL[nSide];nLine++) {
				m_LineBlade[nSide][nLine].FindLimits(pCp0,pCp1,FALSE);
			}
		}
	}
}

void CProfile::FindAllHoleLimits(COORDDATA *pCp0, COORDDATA *pCp1)
{
	bool bInitialized = FALSE;


	for(int gg=0;gg<3;gg++) {
		if(m_CpCadHole[gg].Side[PORTSIDE].fX < pCp0->Side[PORTSIDE].fX) {
			pCp0->Side[PORTSIDE].fX = m_CpCadHole[gg].Side[PORTSIDE].fX;
		}
		if(m_CpCadHole[gg].Side[PORTSIDE].fY < pCp0->Side[PORTSIDE].fY) {
			pCp0->Side[PORTSIDE].fY = m_CpCadHole[gg].Side[PORTSIDE].fY;
		}
		if(m_CpCadHole[gg].Side[PORTSIDE].fZ < pCp0->Side[PORTSIDE].fZ) {
			pCp0->Side[PORTSIDE].fZ = m_CpCadHole[gg].Side[PORTSIDE].fZ;
		}

		if(m_CpCadHole[gg].Side[PORTSIDE].fX > pCp1->Side[PORTSIDE].fX) {
			pCp1->Side[PORTSIDE].fX = m_CpCadHole[gg].Side[PORTSIDE].fX;
		}
		if(m_CpCadHole[gg].Side[PORTSIDE].fY > pCp1->Side[PORTSIDE].fY) {
			pCp1->Side[PORTSIDE].fY = m_CpCadHole[gg].Side[PORTSIDE].fY;
		}
		if(m_CpCadHole[gg].Side[PORTSIDE].fZ > pCp1->Side[PORTSIDE].fZ) {
			pCp1->Side[PORTSIDE].fZ = m_CpCadHole[gg].Side[PORTSIDE].fZ;
		}
	}
}

void CProfile::ComponentMoveLine(D3DXMATRIXA16 & matWorld, int nLineType, int nLine)
{
	if(nLine<0)
		return;

	D3DXMATRIXA16 matRotate = matWorld;
	matRotate._41 = 0.0f;
	matRotate._42 = 0.0f;
	matRotate._43 = 0.0f;
	matRotate._44 = 1.0f;

	switch(nLineType) {
	case TAUGHTLINES:
		if(nLine>=m_nTaughtLineL)
			return;
		m_TaughtLine[nLine].TransformLine(matWorld);
		break;
	case SCANLINES:
		if(nLine>=m_nScanLineL)
			return;
		m_ScanLine[nLine].TransformLine(matWorld);
		break;
	case OUTSIDELINES:
		if(nLine>=m_nLineBladeL[OUTSIDE])
			return;
		m_LineBlade[OUTSIDE][nLine].TransformLine(matWorld);
		break;
	case INSIDELINES:
		if(nLine>=m_nLineBladeL[INSIDE])
			return;
		m_LineBlade[INSIDE][nLine].TransformLine(matWorld);
		break;
	case BRANEOUTSIDELINES:
		if(nLine>=m_nLineBladeL[OUTSIDE+2])
			return;
		m_LineBlade[OUTSIDE+2][nLine].TransformLine(matWorld);
		break;
	case BRANEINSIDELINES:
		if(nLine>=m_nLineBladeL[INSIDE+2])
			return;
		m_LineBlade[INSIDE+2][nLine].TransformLine(matWorld);
		break;
	}

}

void CProfile::ComponentMove(D3DXMATRIXA16 &matWorld)
{
	int nLine,ii,gg;

	D3DXMATRIXA16 matRotate = matWorld;
	matRotate._41 = 0.0f;
	matRotate._42 = 0.0f;
	matRotate._43 = 0.0f;
	matRotate._44 = 1.0f;

	for(nLine=0;nLine<m_nTaughtLineL;nLine++) {
		m_TaughtLine[nLine].TransformLine(matWorld);
	}
	for(nLine=0;nLine<m_nScanLineL;nLine++) {
		m_ScanLine[nLine].TransformLine(matWorld);
	}
	for(nLine=0;nLine<m_nBrainLineL;nLine++) {
		m_BrainLine[nLine].TransformLine(matWorld);
	}
	for(nLine=0;nLine<m_nEnvelopeLineL;nLine++) {
		m_EnvelopeLine[nLine].TransformLine(matWorld);
	}


	for(ii=0;ii<3;ii++) {
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_CpCadHole[ii].Side0.pt,(D3DXVECTOR3*)&m_CpCadHole[ii].Side0.pt,&matWorld);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_CpCadHole[ii].Side0.norm,(D3DXVECTOR3*)&m_CpCadHole[ii].Side0.norm,&matRotate);
		theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_CpCadHole[ii],PORTSIDE);
		theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_CpCadHole[ii],STARBOARD);
		for(gg=0;gg<2;gg++) {
			D3DXVec3TransformCoord((D3DXVECTOR3*)&m_CpExclusion[ii][gg].Side0.pt,(D3DXVECTOR3*)&m_CpExclusion[ii][gg].Side0.pt,&matWorld);
			D3DXVec3TransformCoord((D3DXVECTOR3*)&m_CpExclusion[ii][gg].Side0.norm,(D3DXVECTOR3*)&m_CpExclusion[ii][gg].Side0.norm,&matRotate);
			theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_CpExclusion[ii][gg],PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_CpExclusion[ii][gg],STARBOARD);
			theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_CpExclusion[ii][gg],PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_CpExclusion[ii][gg],STARBOARD);
		}
	}

	m_matCadToScan *= matWorld;

	CalculateDangerZone();
	m_bModified = TRUE;

}

void CProfile::AcceptTransform()
{
	
	int	nLine,ii,gg;
	COORDDATA	Cp;

	CalculateMatrix();

	for(nLine=0;nLine<m_nTaughtLineL;nLine++) {
		for(ii=0;ii<m_TaughtLine[nLine].m_nCoordL;ii++) {
			TransformCoordinate(m_TaughtLine[nLine].m_pCp[ii],&Cp);
			theApp.m_Kinematics.TipsFromNormals((CCoord*)&Cp,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals((CCoord*)&Cp,STARBOARD);
			m_TaughtLine[nLine].m_pCp[ii]=Cp;
		}
	}

	for(nLine=0;nLine<m_nScanLineL;nLine++) {
		for(ii=0;ii<m_ScanLine[nLine].m_nCoordL;ii++) {
			TransformCoordinate(m_ScanLine[nLine].m_pCp[ii],&Cp);
			theApp.m_Kinematics.TipsFromNormals((CCoord*)&Cp,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals((CCoord*)&Cp,STARBOARD);
			m_ScanLine[nLine].m_pCp[ii]=Cp;
		}
	}

	for(nLine=0;nLine<m_nBrainLineL;nLine++) {
		for(ii=0;ii<m_BrainLine[nLine].m_nCoordL;ii++) {
			TransformCoordinate(m_BrainLine[nLine].m_pCp[ii],&Cp);
			theApp.m_Kinematics.TipsFromNormals((CCoord*)&Cp,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals((CCoord*)&Cp,STARBOARD);
			m_BrainLine[nLine].m_pCp[ii]=Cp;
		}
	}

	for(nLine=0;nLine<m_nEnvelopeLineL;nLine++) {
		for(ii=0;ii<m_EnvelopeLine[nLine].m_nCoordL;ii++) {
			TransformCoordinate(m_EnvelopeLine[nLine].m_pCp[ii],&Cp);
			theApp.m_Kinematics.TipsFromNormals((CCoord*)&Cp,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals((CCoord*)&Cp,STARBOARD);
			m_EnvelopeLine[nLine].m_pCp[ii]=Cp;
		}
	}

	for(ii=0;ii<3;ii++) {
		TransformCoordinate(m_CpCadHole[ii],&Cp);
		theApp.m_Kinematics.TipsFromNormals((CCoord*)&Cp,PORTSIDE);
		theApp.m_Kinematics.TipsFromNormals((CCoord*)&Cp,STARBOARD);
		m_CpCadHole[ii] = Cp;
		for(gg=0;gg<2;gg++) {
			TransformCoordinate(m_CpExclusion[ii][gg],&Cp);
			theApp.m_Kinematics.TipsFromNormals((CCoord*)&Cp,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals((CCoord*)&Cp,STARBOARD);
			m_CpExclusion[ii][gg] = Cp;
		}
	}


	m_fRotateX=0.0;
	m_fRotateY=0.0;
	m_fRotateZ=0.0;
	m_fRotateR=0.0;
	m_fTranslateX=0.0;
	m_fTranslateY=0.0;
	m_fTranslateZ=0.0;

	CalculateMatrix();

	CalculateDangerZone();
	m_bModified = TRUE;
}


void CProfile::CalculateMatrix()
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

void CProfile::TransformCoordinate(COORDDATA Cp, COORDDATA *pCp)
{
	float dXp,dYp,dZp,dIp,dJp,dKp;

	//Rotation followed by translation
	/*
	*pCp = Cp;
	for(int nSide=0;nSide<2;nSide++) {
		dXp = (pCp->Side[nSide].fX * m_fIden[0][0] + pCp->Side[nSide].fY * m_fIden[0][1] + pCp->Side[nSide].fZ * m_fIden[0][2]);
		dYp = (pCp->Side[nSide].fX * m_fIden[1][0] + pCp->Side[nSide].fY * m_fIden[1][1] + pCp->Side[nSide].fZ * m_fIden[1][2]);
		dZp = (pCp->Side[nSide].fX * m_fIden[2][0] + pCp->Side[nSide].fY * m_fIden[2][1] + pCp->Side[nSide].fZ * m_fIden[2][2]);
		pCp->Side[nSide].fX = dXp + m_dTranslateX;
		pCp->Side[nSide].fY = dYp + m_dTranslateY;
		pCp->Side[nSide].fZ = dZp + m_dTranslateZ;

		pCp->Side[nSide].fI = (pCp->Side[nSide].fI * m_fIden[0][0] + pCp->Side[nSide].fJ * m_fIden[0][1] + pCp->Side[nSide].fK * m_fIden[0][2]);
		pCp->Side[nSide].fJ = (pCp->Side[nSide].fI * m_fIden[1][0] + pCp->Side[nSide].fJ * m_fIden[1][1] + pCp->Side[nSide].fK * m_fIden[1][2]);
		pCp->Side[nSide].fK = (pCp->Side[nSide].fI * m_fIden[2][0] + pCp->Side[nSide].fJ * m_fIden[2][1] + pCp->Side[nSide].fK * m_fIden[2][2]);
	}
	*/
	//Translation followed by rotation
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


void CProfile::ReverseAllLines()
{

	for(int nLine=0;nLine<m_nScanLineL;nLine++) {
		m_ScanLine[nLine].ReverseAllCoords();
	}
	m_bModified = TRUE;

}

void CProfile::CopyAllLinesToGlobal(int nSrc)
{
	CPolyCoord *pSrcLine;
	int nSrcLineL;

	SAFE_DELETE_ARRAY(theApp.m_pCopyManyLine);
	theApp.m_nManyLineL = 0;

	switch(nSrc) {
	case TAUGHTLINES:
		nSrcLineL = m_nTaughtLineL;
		pSrcLine = m_TaughtLine;
		break;
	case SCANLINES:
		nSrcLineL = m_nScanLineL;
		pSrcLine = m_ScanLine;
		break;
	case BRAINLINES:
		nSrcLineL = m_nBrainLineL;
		pSrcLine = m_BrainLine;
		break;
	default:
		MessageBox(NULL,L"Not supported", L"Errror", MB_ICONERROR);
		return;
		break;
	}

	if(nSrcLineL > 0 && pSrcLine != NULL) {
		theApp.m_pCopyManyLine = new CPolyCoord[nSrcLineL];
		for(int nLine=0;nLine<nSrcLineL;nLine++) {
			theApp.m_pCopyManyLine[nLine] = &pSrcLine[nLine];
		}
		theApp.m_nManyLineL = nSrcLineL;
	} else {
		MessageBox(NULL,L"There are no lines to copy", L"Errror", MB_ICONERROR);
		return;
	}


}

void CProfile::PasteAllLinesFromGlobal(int nDest, int nInsertAt)
{
	CPolyCoord *pDestLine;

	if(theApp.m_nManyLineL > 0 && theApp.m_pCopyManyLine != NULL) {

		switch(nDest) {
		case TAUGHTLINES:
			SAFE_DELETE_ARRAY(m_TaughtLine);
			m_TaughtLine = new CPolyCoord[ m_nTaughtLineL = theApp.m_nManyLineL ];
			pDestLine = m_TaughtLine;
			break;
		case SCANLINES:
			SAFE_DELETE_ARRAY(m_ScanLine);
			m_ScanLine = new CPolyCoord[ m_nScanLineL = theApp.m_nManyLineL ];
			pDestLine = m_ScanLine;
			break;
		case BRAINLINES:
			SAFE_DELETE_ARRAY(m_BrainLine);
			m_BrainLine = new CPolyCoord[ m_nBrainLineL = theApp.m_nManyLineL ];
			pDestLine = m_BrainLine;
			break;
		default:
			MessageBox(NULL,L"Not supported", L"Errror", MB_ICONERROR);
			return;
			break;
		}

		for(int nLine=0;nLine<theApp.m_nManyLineL;nLine++) {
			pDestLine[nLine] = &theApp.m_pCopyManyLine[nLine];
		}
	} else {
		MessageBox(NULL,L"There are no lines to paste", L"Errror", MB_ICONERROR);
		return;
	}

}



void CProfile::CopyAllLines(int nDest, int nSrc, bool bDeleteSrc)
{
	int nLine;
	if(nDest == nSrc) return;

	DeleteAllLines( nDest );

	switch(nDest) {
	case TAUGHTLINES:
		switch(nSrc) {
		case SURFACELINES:
			if(m_nSurfaceLineL>0) {
				m_TaughtLine = new CPolyCoord[m_nTaughtLineL = m_nSurfaceLineL];
				for(nLine=0;nLine<m_nTaughtLineL; nLine++) {
					m_TaughtLine[nLine]=&m_SurfaceLine[nLine];
				}
			}
			break;
		case SCANLINES:
			if(m_nScanLineL>0) {
				m_TaughtLine = new CPolyCoord[m_nTaughtLineL = m_nScanLineL];
				for(nLine=0;nLine<m_nTaughtLineL; nLine++) {
					m_TaughtLine[nLine]=&m_ScanLine[nLine];
				}
			}
			break;
		}
		break;
	case SURFACELINES:
		switch(nSrc) {
		case TAUGHTLINES:
			if(m_nTaughtLineL>0) {
				m_SurfaceLine = new CPolyCoord[m_nSurfaceLineL = m_nTaughtLineL];
				for(nLine=0;nLine<m_nSurfaceLineL; nLine++) {
					m_SurfaceLine[nLine]=&m_TaughtLine[nLine];
				}
			}
			break;
		}
		break;
	case OUTSIDELINES:
		switch(nSrc) {
		case SCANLINES:
			if(m_nScanLineL>0) {
				m_LineBlade[OUTSIDE] = new CPolyCoord[m_nLineBladeL[OUTSIDE] = m_nScanLineL];
				for(nLine=0;nLine<m_nLineBladeL[OUTSIDE]; nLine++) {
					m_LineBlade[OUTSIDE][nLine]=&m_ScanLine[nLine];
				}
			}
			break;
		}
		break;
	case INSIDELINES:
		switch(nSrc) {
		case SCANLINES:
			if(m_nScanLineL>0) {
				m_LineBlade[INSIDE] = new CPolyCoord[m_nLineBladeL[INSIDE] = m_nScanLineL];
				for(nLine=0;nLine<m_nLineBladeL[INSIDE]; nLine++) {
					m_LineBlade[INSIDE][nLine]=&m_ScanLine[nLine];
				}
			}
			break;
		}
		break;
	case SCANLINES:
		switch(nSrc) {
		case OUTSIDELINES:
			if(m_nLineBladeL[OUTSIDE]>0) {
				m_ScanLine = new CPolyCoord[m_nScanLineL = m_nLineBladeL[OUTSIDE]];
				for(nLine=0;nLine<m_nScanLineL; nLine++) {
					m_ScanLine[nLine]=&m_LineBlade[OUTSIDE][nLine];
				}
			}
			break;
		case INSIDELINES:
			if(m_nLineBladeL[INSIDE]>0) {
				m_ScanLine = new CPolyCoord[m_nScanLineL = m_nLineBladeL[INSIDE]];
				for(nLine=0;nLine<m_nScanLineL; nLine++) {
					m_ScanLine[nLine]=&m_LineBlade[INSIDE][nLine];
				}
			}
			break;
		}
		break;
	case BRANEOUTSIDELINES:
		switch(nSrc) {
		case BRAINLINES:
			if(m_nBrainLineL>0) {
				m_LineBlade[OUTSIDE + 2] = new CPolyCoord[m_nLineBladeL[OUTSIDE + 2] = m_nBrainLineL];
				for(nLine=0;nLine<m_nBrainLineL; nLine++) {
					m_LineBlade[OUTSIDE + 2][nLine]=&m_BrainLine[nLine];
				}
			}
			break;
		}
		break;
	case BRANEINSIDELINES:
		switch(nSrc) {
		case BRAINLINES:
			if(m_nBrainLineL>0) {
				m_LineBlade[INSIDE + 2] = new CPolyCoord[m_nLineBladeL[INSIDE + 2] = m_nBrainLineL];
				for(nLine=0;nLine<m_nBrainLineL; nLine++) {
					m_LineBlade[INSIDE + 2][nLine]=&m_BrainLine[nLine];
				}
			}
			break;
		}
		break;
	case BRAINLINES:
		switch(nSrc) {
		case BRANEOUTSIDELINES:
			if(m_nLineBladeL[OUTSIDE + 2]>0) {
				m_BrainLine = new CPolyCoord[m_nBrainLineL = m_nLineBladeL[OUTSIDE + 2]];
				for(nLine=0;nLine<m_nBrainLineL; nLine++) {
					m_BrainLine[nLine] = &m_LineBlade[OUTSIDE + 2][nLine];
				}
			}
			break;
		case BRANEINSIDELINES:
			if(m_nLineBladeL[INSIDE + 2]>0) {
				m_BrainLine = new CPolyCoord[m_nBrainLineL = m_nLineBladeL[INSIDE + 2]];
				for(nLine=0;nLine<m_nBrainLineL; nLine++) {
					m_BrainLine[nLine] = &m_LineBlade[INSIDE + 2][nLine];
				}
			}
			break;
		};
		break;
	}

	if(bDeleteSrc == true) {	//Delete source lines
		DeleteAllLines( nSrc );
	}

	m_bModified = TRUE;


}

void CProfile::DisplayThumbNail(CString csFilePath, CDC *pDC, CRect rr)
{
	CUSLFile File;
	CFileException e;
	CFileStatus rStatus;
	CString Buff;
	int nThumbNailOffset,nProfileDataOffset,nTaughtLineOffset,nScanLineOffset,nBrainLineOffset,nEnvelopeLineOffset,nStringsOffset;
	char aStr[60];
	char aIdentifier[60]={"Ultrasonic Sciences Ltd USLScanner Profile"};
	WCHAR wStr[60];
	WCHAR wIdentifier[]={_T("Ultrasonic Sciences Ltd USLScanner Profile")};
	float fVersion;
	CBitmap bmp;
	CDC  dcMem;
	char* pArray;
	BITMAPFILEHEADER BmpFileHeader;
	BITMAPINFOHEADER BmpHeader;

	if(CFile::GetStatus(csFilePath,rStatus) == FALSE) return;
	if(rStatus.m_size<=0) return;

	if(!File.Open(csFilePath,CFile::modeReadWrite|CFile::typeBinary,&e)) {
//		File.DecodeFileException(&e,&Buff,csFilePath);
//		MessageBox(NULL,Buff,"Error",MB_ICONERROR);
	} else {

		ZeroMemory(wStr,sizeof wStr);
		File.Seek(0,CFile::begin);
		File.Read(wStr,sizeof wIdentifier);
		File.Read(&fVersion,sizeof fVersion);
		if(wcscmp(wStr,wIdentifier)==0) {
			goto read_file;
		}

		ZeroMemory(aStr,sizeof aStr);
		File.Seek(0,CFile::begin);
		File.Read(aStr,strlen(aIdentifier));
		File.Read(&fVersion,sizeof fVersion);
		if(strcmp(aStr,aIdentifier)==0) {
			goto read_file;
		}
		File.Close();
		return;

read_file:
		File.Seek(0x100,CFile::begin);
		File.Read(&nThumbNailOffset,sizeof nThumbNailOffset);
		File.Read(&nProfileDataOffset,sizeof nProfileDataOffset);
		File.Read(&nStringsOffset,sizeof nStringsOffset);
		File.Read(&nTaughtLineOffset,sizeof nTaughtLineOffset);
		File.Read(&nScanLineOffset,sizeof nScanLineOffset);
		File.Read(&nBrainLineOffset,sizeof nBrainLineOffset);
		File.Read(&nEnvelopeLineOffset,sizeof nEnvelopeLineOffset);

		if(nThumbNailOffset) {

			File.Seek(nThumbNailOffset,CFile::begin);
			File.Read(&BmpFileHeader, sizeof(BITMAPFILEHEADER));
			File.Read(&BmpHeader, sizeof(BITMAPINFOHEADER));

			pArray = new char[BmpHeader.biWidth * BmpHeader.biHeight * 4];
			File.Read(pArray, BmpHeader.biWidth * BmpHeader.biHeight * (sizeof COLORREF));

			bmp.CreateCompatibleBitmap(pDC,BmpHeader.biWidth,BmpHeader.biHeight);
			bmp.SetBitmapBits((DWORD)(BmpHeader.biWidth*BmpHeader.biHeight*4),pArray);
			dcMem.CreateCompatibleDC(pDC);
			CBitmap *pOldBitmap = dcMem.SelectObject(&bmp);

			int nHeight = MulDiv(rr.Height(),BmpHeader.biHeight , BmpHeader.biWidth);
			int nTop = (rr.Height() - nHeight) / 2 + rr.top;
			pDC->StretchBlt(rr.left,nTop,rr.Width(),nHeight,&dcMem,0,0,BmpHeader.biWidth,BmpHeader.biHeight,SRCCOPY);

			delete pArray;

		}
		File.Close();
	}
}

bool CProfile::FindNearestCoordinate(int nWhichLine, COORDDATA Cp, int *pnLine, int *pnIndex,char cIgnoreAxis)
{
	
	CPolyCoord* pLine = NULL;
	int nLineL;
	float fHypot = 30000.0f;
	float fTemp;
	int	nLine,nn;
	int nStartLine,nFinishLine;

	switch(nWhichLine) {
	case TAUGHTLINES: pLine = m_TaughtLine;
		nLineL = m_nTaughtLineL;
		break;
	case SCANLINES: pLine = m_ScanLine;
		nLineL = m_nScanLineL;
		break;
	case BRAINLINES: pLine = m_BrainLine;
		nLineL = m_nBrainLineL;
		break;
	case ENVELOPELINES: pLine = m_EnvelopeLine;
		nLineL = m_nEnvelopeLineL;
		break;
	case OUTSIDELINES: pLine = m_LineBlade[OUTSIDE];
		nLineL = m_nLineBladeL[OUTSIDE];
		break;
	case INSIDELINES: pLine = m_LineBlade[INSIDE];
		nLineL = m_nLineBladeL[INSIDE];
		break;
	}
	if(pLine==NULL) return FALSE;
	if(nLineL<=0) return FALSE;
	if(theApp.m_LastSettings.nDisplayEditWhichLines==0) {
		nStartLine=0;
		nFinishLine=nLineL;
	} else {
		nStartLine = theApp.m_nLine;
		nFinishLine = theApp.m_nLine+1;
		if(nStartLine>=nLineL) nStartLine = nLineL-1;
		if(nFinishLine>nLineL) nFinishLine = nLineL-1;
	}

	for(nLine=nStartLine;nLine<nFinishLine;nLine++) {
		for(nn=0;nn<pLine[nLine].m_nCoordL;nn++) {
			switch(cIgnoreAxis) {
			case 'X': Cp.Side0.fX = pLine[nLine].m_pCp[nn].Side0.fX;
				break;
			case 'Y': Cp.Side0.fY = pLine[nLine].m_pCp[nn].Side0.fY;
				break;
			case 'Z': Cp.Side0.fZ = pLine[nLine].m_pCp[nn].Side0.fZ;
				break;
			}
			fTemp=CCoord::Length(pLine[nLine].m_pCp[nn],Cp);
			if(fTemp<fHypot) {
				*pnLine = nLine;
				*pnIndex = nn;
				fHypot=fTemp;
			}
		}
	}



	return TRUE;
}

bool CProfile::FindNearestAngleCoordinate(int nWhichLine, COORDDATA Cp, int *pnLine, int *pnIndex)
{
	
	CPolyCoord* pLine = NULL;
	int nLineL;
	float fHypot = 30000.0f;
	float fTemp,fR,fZ;
	int	nLine,nn;
	int nStartLine,nFinishLine;

	switch(nWhichLine) {
	case TAUGHTLINES: pLine = m_TaughtLine;
		nLineL = m_nTaughtLineL;
		break;
	case SCANLINES: pLine = m_ScanLine;
		nLineL = m_nScanLineL;
		break;
	case BRAINLINES: pLine = m_BrainLine;
		nLineL = m_nBrainLineL;
		break;
	case ENVELOPELINES: pLine = m_EnvelopeLine;
		nLineL = m_nEnvelopeLineL;
		break;
	}
	if(pLine==NULL) return FALSE;
	if(nLineL<=0) return FALSE;
	if(theApp.m_LastSettings.nDisplayEditWhichLines==0) {
		nStartLine=0;
		nFinishLine=nLineL;
	} else {
		nStartLine = theApp.m_nLine;
		nFinishLine = theApp.m_nLine+1;
		if(nStartLine>=nLineL) nStartLine = nLineL-1;
		if(nFinishLine>nLineL) nFinishLine = nLineL-1;
	}

	for(nLine=nStartLine;nLine<nFinishLine;nLine++) {
		for(nn=0;nn<pLine[nLine].m_nCoordL;nn++) {
			fR = Cp.Side0.fR - pLine[nLine].m_pCp[nn].Side0.fR;
			fZ = Cp.Side0.fZ - pLine[nLine].m_pCp[nn].Side0.fZ;
			fTemp=sqrtf(fZ * fZ + fR * fR);
			if(fTemp<fHypot) {
				*pnLine = nLine;
				*pnIndex = nn;
				fHypot=fTemp;
			}
		}
	}



	return TRUE;
}

void CProfile::AutomaticEnvelope(int nSrcLines,float fTop,float fLeft,float fBottom,float fRight)
{
	CCoord CpMin,CpMax,CpCenter;
	COORDDATA Cp;
	int	nLine,nMaxLine,nn;
	CPolyCoord* pLine = NULL;

	SAFE_DELETE_ARRAY( m_EnvelopeLine );
	m_nEnvelopeLineL = 0;

	FindBoundingCube(nSrcLines,&CpMin,&CpMax);
	CpCenter.Side0.fX=(CpMax.Side0.fX+CpMin.Side0.fX) / 2.0f;
	CpCenter.Side0.fY=(CpMax.Side0.fY+CpMin.Side0.fY) / 2.0f;
	CpCenter.Side0.fZ=(CpMax.Side0.fZ+CpMin.Side0.fZ) / 2.0f;


	switch(nSrcLines) {
	case TAUGHTLINES: if((m_TaughtLine==NULL) || (m_nTaughtLineL<=0)) return;
		pLine = m_TaughtLine;
		nMaxLine = m_nTaughtLineL-1;
		break;
	case SCANLINES: if((m_ScanLine==NULL) || (m_nScanLineL<=0)) return;
		pLine = m_ScanLine;
		nMaxLine = m_nScanLineL-1;
		break;
	case BRAINLINES: if((m_BrainLine==NULL) || (m_nBrainLineL<=0)) return;
		pLine = m_BrainLine;
		nMaxLine = m_nBrainLineL-1;
		break;
	default: return;
		break;
	}
	/*
	fBottom = fBottom / 100.0f + 1.0f;
	fTop = fTop / 100.0f + 1.0f;
	fLeft = fLeft / 100.0f + 1.0f;
	fRight = fRight / 100.0f + 1.0f;
	*/
	m_EnvelopeLine = new CPolyCoord[m_nEnvelopeLineL=1];

	for(nn=0;nn<pLine[0].m_nCoordL;nn++) {
		Cp=pLine[0].m_pCp[nn];
		if(nn==0) Cp.Side0.fPos[m_nFastAxis] = fLeft + (Cp.Side0.fPos[m_nFastAxis] - CpCenter.Side0.fPos[m_nFastAxis]) + CpCenter.Side0.fPos[m_nFastAxis];
		if(nn==(pLine[0].m_nCoordL-1)) Cp.Side0.fPos[m_nFastAxis] = fRight + (Cp.Side0.fPos[m_nFastAxis] - CpCenter.Side0.fPos[m_nFastAxis]) + CpCenter.Side0.fPos[m_nFastAxis];
		Cp.Side0.fPos[m_nSlowAxis] = fBottom + (Cp.Side0.fPos[m_nSlowAxis] - CpCenter.Side0.fPos[m_nSlowAxis]) + CpCenter.Side0.fPos[m_nSlowAxis];
		m_EnvelopeLine[0] + (Cp);
	}

	for(nLine=1;nLine<nMaxLine;nLine++) {
		Cp=pLine[nLine].m_pCp[pLine[nLine].m_nCoordL-1];
		Cp.Side0.fPos[m_nFastAxis] = fRight + (Cp.Side0.fPos[m_nFastAxis] - CpCenter.Side0.fPos[m_nFastAxis]) + CpCenter.Side0.fPos[m_nFastAxis];
		m_EnvelopeLine[0] +(Cp);
	}

	for(nn=pLine[nMaxLine].m_nCoordL-1;nn>=0;nn--) {
		Cp=pLine[nMaxLine].m_pCp[nn];
		if(nn==0) Cp.Side0.fPos[m_nFastAxis] = fLeft + (Cp.Side0.fPos[m_nFastAxis] - CpCenter.Side0.fPos[m_nFastAxis]) + CpCenter.Side0.fPos[m_nFastAxis];
		if(nn==(pLine[0].m_nCoordL-1)) Cp.Side0.fPos[m_nFastAxis] = fRight + (Cp.Side0.fPos[m_nFastAxis] - CpCenter.Side0.fPos[m_nFastAxis]) + CpCenter.Side0.fPos[m_nFastAxis];
		Cp.Side0.fPos[m_nSlowAxis] = fTop + (Cp.Side0.fPos[m_nSlowAxis] - CpCenter.Side0.fPos[m_nSlowAxis]) + CpCenter.Side0.fPos[m_nSlowAxis];

		m_EnvelopeLine[0] +(Cp);
	}

	for(nLine=nMaxLine-1;nLine>0;nLine--) {
		Cp=pLine[nLine].m_pCp[0];
		Cp.Side0.fPos[m_nFastAxis] = fLeft + (Cp.Side0.fPos[m_nFastAxis] - CpCenter.Side0.fPos[m_nFastAxis]) + CpCenter.Side0.fPos[m_nFastAxis];
		m_EnvelopeLine[0] +(Cp);
	}
	m_bModified = TRUE;


}

void CProfile::AutomaticRectangleEnvelope(int nSrcLines,float fTop,float fLeft,float fBottom,float fRight)
{
	CCoord CpMin,CpMax,CpCenter;
	COORDDATA Cp;

	SAFE_DELETE_ARRAY( m_EnvelopeLine );
	m_nEnvelopeLineL = 0;

	FindBoundingCube(nSrcLines,&CpMin,&CpMax);

	m_EnvelopeLine = new CPolyCoord[m_nEnvelopeLineL=1];
	Cp = CpMin;
	Cp.Side0.norm.x = 0;
	Cp.Side0.norm.y = 0;
	Cp.Side0.norm.z = 0;
	switch(m_nFastAxis) {
	case 0: Cp.Side0.fX = CpMin.Side0.fX + fLeft;
		break;
	case 1: Cp.Side0.fY = CpMin.Side0.fY + fLeft;
		break;
	case 2: Cp.Side0.fZ = CpMin.Side0.fZ + fLeft;
		break;
	}
	switch(m_nSlowAxis) {
	case 0: Cp.Side0.fX = CpMin.Side0.fX + fBottom;
		break;
	case 1: Cp.Side0.fY = CpMin.Side0.fY + fBottom;
		break;
	case 2: Cp.Side0.fZ = CpMin.Side0.fZ + fBottom;
		break;
	}
	Cp.Side[1] = Cp.Side0;
	m_EnvelopeLine[0] + (Cp);

	switch (m_nFastAxis) {
	case 0: Cp.Side0.fX = CpMax.Side0.fX + fRight;
		break;
	case 1: Cp.Side0.fY = CpMax.Side0.fY + fRight;
		break;
	case 2: Cp.Side0.fZ = CpMax.Side0.fZ + fRight;
		break;
	}
	Cp.Side[1] = Cp.Side0;
	m_EnvelopeLine[0] + (Cp);
	switch(m_nSlowAxis) {
	case 0: Cp.Side0.fX = CpMax.Side0.fX + fTop;
		break;
	case 1: Cp.Side0.fY = CpMax.Side0.fY + fTop;
		break;
	case 2: Cp.Side0.fZ = CpMax.Side0.fZ + fTop;
		break;
	}
	Cp.Side[1] = Cp.Side0;
	m_EnvelopeLine[0] + (Cp);
	switch(m_nFastAxis) {
	case 0: Cp.Side0.fX = CpMin.Side0.fX + fLeft;
		break;
	case 1: Cp.Side0.fY = CpMin.Side0.fY + fLeft;
		break;
	case 2: Cp.Side0.fZ = CpMin.Side0.fZ + fLeft;
		break;
	}
	Cp.Side[1] = Cp.Side0;
	m_EnvelopeLine[0] + (Cp);
	m_bModified = TRUE;

}

void CProfile::FindBoundingCube(int nWhichLines, CCoord *pCpMin, CCoord *pCpMax)
{
	pCpMin->Side0.fX=1e7;
	pCpMin->Side0.fY=1e7;
	pCpMin->Side0.fZ=1e7;
	pCpMax->Side0.fX=-1e7;
	pCpMax->Side0.fY=-1e7;
	pCpMax->Side0.fZ=-1e7;

	CPolyCoord* pLine = NULL;
	int	nLineL,nn,nLine;
	COORDDATA Cp;

	switch(nWhichLines) {
	case TAUGHTLINES: pLine = m_TaughtLine;
		nLineL=m_nTaughtLineL;
		break;
	case SCANLINES: pLine = m_ScanLine;
		nLineL=m_nScanLineL;
		break;
	case BRAINLINES: pLine = m_BrainLine;
		nLineL=m_nBrainLineL;
		break;
	}
	if((pLine) && (nLineL>0)) {

		for(nLine=0;nLine<nLineL;nLine++) {
			for(nn=0;nn<pLine[nLine].m_nCoordL;nn++) {
				Cp = pLine[nLine].m_pCp[nn];
				if(Cp.Side0.fX < pCpMin->Side0.fX) pCpMin->Side0.fX = Cp.Side0.fX;
				if(Cp.Side0.fY < pCpMin->Side0.fY) pCpMin->Side0.fY = Cp.Side0.fY;
				if(Cp.Side0.fZ < pCpMin->Side0.fZ) pCpMin->Side0.fZ = Cp.Side0.fZ;
				if(Cp.Side0.fX > pCpMax->Side0.fX) pCpMax->Side0.fX = Cp.Side0.fX;
				if(Cp.Side0.fY > pCpMax->Side0.fY) pCpMax->Side0.fY = Cp.Side0.fY;
				if(Cp.Side0.fZ > pCpMax->Side0.fZ) pCpMax->Side0.fZ = Cp.Side0.fZ;
			}
		}
	}

}


void CProfile::DeleteLine(int nSrcLine, int nLine)
{
	CPolyCoord* pLine = NULL;
	int	nn;

	switch(nSrcLine) {
	case TAUGHTLINES: if((m_TaughtLine==NULL) || (m_nTaughtLineL<=0)) return;
		pLine = new CPolyCoord[m_nTaughtLineL];
		for(nn=0;nn<nLine;nn++) {
			pLine[nn] = &m_TaughtLine[nn];
		}
		for(nn; nn<(m_nTaughtLineL-1) ; nn++) {
			pLine[nn] = &m_TaughtLine[nn+1];
		}
		m_nTaughtLineL--;
		delete [] m_TaughtLine;
		m_TaughtLine = NULL;
		if(m_nTaughtLineL) {
			m_TaughtLine = new CPolyCoord[m_nTaughtLineL];
			for(nn=0;nn<m_nTaughtLineL;nn++) {
				m_TaughtLine[nn] = &pLine[nn];
			}
		}
		break;
	case SCANLINES: if((m_ScanLine==NULL) && (m_nScanLineL<=0)) return;
		pLine = new CPolyCoord[m_nScanLineL];
		for(nn=0;nn<nLine;nn++) {
			pLine[nn] = &m_ScanLine[nn];
		}
		for(nn; nn<(m_nScanLineL-1) ; nn++) {
			pLine[nn] = &m_ScanLine[nn+1];
		}
		m_nScanLineL--;
		delete [] m_ScanLine;
		m_ScanLine = NULL;
		if(m_nScanLineL) {
			m_ScanLine = new CPolyCoord[m_nScanLineL];
			for(nn=0;nn<m_nScanLineL;nn++) {
				m_ScanLine[nn] = &pLine[nn];
			}
		}
		break;
	case BRAINLINES: if((m_BrainLine==NULL) && (m_nBrainLineL<=0)) return;
		pLine = new CPolyCoord[m_nBrainLineL];
		for(nn=0;nn<nLine;nn++) {
			pLine[nn] = &m_BrainLine[nn];
		}
		for(nn; nn<(m_nBrainLineL-1) ; nn++) {
			pLine[nn] = &m_BrainLine[nn+1];
		}
		m_nBrainLineL--;
		delete [] m_BrainLine;
		m_BrainLine = NULL;
		if(m_nBrainLineL) {
			m_BrainLine = new CPolyCoord[m_nBrainLineL];
			for(nn=0;nn<m_nBrainLineL;nn++) {
				m_BrainLine[nn] = &pLine[nn];
			}
		}
		break;
	case ENVELOPELINES: if((m_EnvelopeLine==NULL) && (m_nEnvelopeLineL<=0)) return;
		pLine = new CPolyCoord[m_nEnvelopeLineL];
		for(nn=0;nn<nLine;nn++) {
			pLine[nn] = &m_EnvelopeLine[nn];
		}
		for(nn; nn<(m_nEnvelopeLineL-1) ; nn++) {
			pLine[nn] = &m_EnvelopeLine[nn+1];
		}
		m_nEnvelopeLineL--;
		delete [] m_EnvelopeLine;
		m_EnvelopeLine = NULL;
		if(m_nEnvelopeLineL) {
			m_EnvelopeLine = new CPolyCoord[m_nEnvelopeLineL];
			for(nn=0;nn<m_nEnvelopeLineL;nn++) {
				m_EnvelopeLine[nn] = &pLine[nn];
			}
		}
		break;
	}

	delete [] pLine;
	m_bModified = TRUE;


}

void CProfile::DeleteLines(int nLineType, int nStartIndex, int nFinishIndex)
{
	CPolyCoord* pLine = NULL;
	int	nn,nNewTotal;

	switch(nLineType) {
	case SCANLINES: if((m_ScanLine==NULL) && (m_nScanLineL<=0)) return;
		pLine = new CPolyCoord[m_nScanLineL];
		for(nn=0,nNewTotal=0;nn<nStartIndex;nn++,nNewTotal++) {
			pLine[nNewTotal] = &m_ScanLine[nn];
		}
		for(nn = nFinishIndex+1; nn<m_nScanLineL ; nn++, nNewTotal++) {
			pLine[nNewTotal] = &m_ScanLine[nn];
		}
		m_nScanLineL=nNewTotal;
		SAFE_DELETE_ARRAY( m_ScanLine );
		if(m_nScanLineL) {
			m_ScanLine = new CPolyCoord[m_nScanLineL];
			for(nn=0;nn<m_nScanLineL;nn++) {
				m_ScanLine[nn] = &pLine[nn];
			}
		}
		break;
	}

}


/* The idea behind this function is that we search through all the lines created,
	see if our position matches an existing line that has less than the max variance within average positon less than the max deviation.
*/
int CProfile::FindLineNumber(int nWhichLines,int nAxis, float fPosition, float fMaxDeviation, float fMaxVariance) 
{
	CPolyCoord* pLine = NULL;
	int ii,jj;
	float fMin,fMax;
	float fCentre;

	switch(nWhichLines) {
	case TAUGHTLINES:
		if((m_TaughtLine!=NULL) && (m_nTaughtLineL>0)) {
			for(ii=0;ii<m_nTaughtLineL;ii++) {
				switch(nAxis) {
				case 0: //X (line should maintain constant X)
					fMin=20000;
					fMax=-20000;
					for(jj=0;jj<m_TaughtLine[ii].m_nCoordL;jj++) {
						if(m_TaughtLine[ii].m_pCp[jj].Side0.fX<fMin)
							fMin = m_TaughtLine[ii].m_pCp[jj].Side0.fX;
						if(m_TaughtLine[ii].m_pCp[jj].Side0.fX>fMax)
							fMax = m_TaughtLine[ii].m_pCp[jj].Side0.fX;
					}
					break;
				}//switch axis
				fCentre = ((fMax-fMin)/2)+fMin;
				if( abs(fCentre-fPosition) < fMaxDeviation ) {
					if ( abs(fMax-fMin) < fMaxVariance ) {
						return ii;
					}
				}
				
			}//for line
		}else{
			return -1;
		}
		break;
	}

	return -1;


}

int CProfile::AddLine(int nLineType, CPolyCoord *pInsertLine,int nSortAxis)
{
	CPolyCoord* pLine = NULL;
	int	nLine,nSrcLine,nDestLine;
	bool bInserted = FALSE;
	int	nInsertLine=0;

	switch(nLineType) {
	case TAUGHTLINES:
		if((m_TaughtLine!=NULL) && (m_nTaughtLineL>0)) {
			pLine = new CPolyCoord[m_nTaughtLineL];
			for(nLine=0;nLine<m_nTaughtLineL;nLine++) {
				pLine[nLine] = &m_TaughtLine[nLine];
			}
			delete [] m_TaughtLine;
			m_nTaughtLineL++;
			m_TaughtLine = new CPolyCoord[m_nTaughtLineL];
			for(nDestLine=nSrcLine=0;(nDestLine<m_nTaughtLineL)&&(nSrcLine<(m_nTaughtLineL-1)); ) {
				if((pLine[nSrcLine].m_pCp[0].fPos[nSortAxis] < pInsertLine->m_pCp[0].fPos[nSortAxis]) || (bInserted==TRUE)) {
					m_TaughtLine[nDestLine++] = &pLine[nSrcLine++];
				} else {
					nInsertLine=nDestLine;
					m_TaughtLine[nDestLine++] = pInsertLine;
					bInserted = TRUE;
				}
			}
			if(bInserted==FALSE) {
				nInsertLine=nDestLine;
				m_TaughtLine[nDestLine++] = pInsertLine;
			}
		} else {
			m_nTaughtLineL++;
			m_TaughtLine = new CPolyCoord[m_nTaughtLineL];
			m_TaughtLine[0] = pInsertLine;
		}
		break;
	case SCANLINES:
		if((m_ScanLine!=NULL) && (m_nScanLineL>0)) {
			pLine = new CPolyCoord[m_nScanLineL];
			for(nLine=0;nLine<m_nScanLineL;nLine++) {
				pLine[nLine] = &m_ScanLine[nLine];
			}
			delete [] m_ScanLine;
			m_nScanLineL++;
			m_ScanLine = new CPolyCoord[m_nScanLineL];
			for(nDestLine=nSrcLine=0;(nDestLine<m_nScanLineL)&&(nSrcLine<(m_nScanLineL-1)); ) {
				if((pLine[nSrcLine].m_pCp[0].fPos[nSortAxis] < pInsertLine->m_pCp[0].fPos[nSortAxis]) || (bInserted==TRUE)) {
					m_ScanLine[nDestLine++] = &pLine[nSrcLine++];
				} else {
					nInsertLine=nDestLine;
					m_ScanLine[nDestLine++] = pInsertLine;
					bInserted = TRUE;
				}
			}
			if(bInserted==FALSE) {
				nInsertLine=nDestLine;
				m_ScanLine[nDestLine++] = pInsertLine;
			}
		} else {
			m_nScanLineL++;
			m_ScanLine = new CPolyCoord[m_nScanLineL];
			m_ScanLine[0] = pInsertLine;
		}
		break;
	case BRAINLINES:
		if((m_BrainLine!=NULL) && (m_nBrainLineL>0)) {
			pLine = new CPolyCoord[m_nBrainLineL];
			for(nLine=0;nLine<m_nBrainLineL;nLine++) {
				pLine[nLine] = &m_BrainLine[nLine];
			}
			delete [] m_BrainLine;
			m_nBrainLineL++;
			m_BrainLine = new CPolyCoord[m_nBrainLineL];
			for(nDestLine=nSrcLine=0;(nDestLine<m_nBrainLineL) && (nSrcLine<(m_nBrainLineL-1)); ) {
				if((pLine[nSrcLine].m_pCp[0].fPos[nSortAxis] < pInsertLine->m_pCp[0].fPos[nSortAxis]) || (bInserted==TRUE)) {
					m_BrainLine[nDestLine++] = &pLine[nSrcLine++];
				} else {
					nInsertLine=nDestLine;
					m_BrainLine[nDestLine++] = pInsertLine;
					bInserted = TRUE;
				}
			}
			if(bInserted==FALSE) {
				nInsertLine=nDestLine;
				m_BrainLine[nDestLine++] = pInsertLine;
			}
		} else {
			m_nBrainLineL++;
			m_BrainLine = new CPolyCoord[m_nBrainLineL];
			m_BrainLine[0] = pInsertLine;
		}
		break;
	case ENVELOPELINES:
		if((m_EnvelopeLine!=NULL) && (m_nEnvelopeLineL>0)) {
			pLine = new CPolyCoord[m_nEnvelopeLineL];
			for(nLine=0;nLine<m_nEnvelopeLineL;nLine++) {
				pLine[nLine] = &m_EnvelopeLine[nLine];
			}
			delete [] m_EnvelopeLine;
			m_nEnvelopeLineL++;
			m_EnvelopeLine = new CPolyCoord[m_nEnvelopeLineL];
			for(nDestLine=nSrcLine=0;(nDestLine<m_nEnvelopeLineL) && (nSrcLine<(m_nEnvelopeLineL-1)); ) {
				/* Dont insert just append*/
				m_EnvelopeLine[nDestLine++] = &pLine[nSrcLine++];
			}
			if(bInserted==FALSE) {
				nInsertLine=nDestLine;
				m_EnvelopeLine[nDestLine++] = pInsertLine;
			}
		} else {
			m_nEnvelopeLineL++;
			m_EnvelopeLine = new CPolyCoord[m_nEnvelopeLineL];
			m_EnvelopeLine[0] = pInsertLine;
		}
		break;
	}

	delete [] pLine;
	m_bModified = TRUE;

	return 	nInsertLine;

}


void CProfile::AddLine(int nLineType)
{
	CPolyCoord *pLine = NULL;
	int nLine;

	switch(nLineType) {
	case TAUGHTLINES:
		if(m_nTaughtLineL>0) {
			pLine = new CPolyCoord[m_nTaughtLineL];
			for(nLine=0;nLine<m_nTaughtLineL;nLine++) pLine[nLine] = &m_TaughtLine[nLine];
			SAFE_DELETE_ARRAY(m_TaughtLine);
			m_TaughtLine = new CPolyCoord[m_nTaughtLineL + 1];
			for(nLine=0;nLine<m_nTaughtLineL;nLine++) m_TaughtLine[nLine] = &pLine[nLine];
			m_nTaughtLineL++;
		} else {
			m_TaughtLine = new CPolyCoord[m_nTaughtLineL=1];
		}
		m_TaughtLine[m_nTaughtLineL-1].Zero();
		break;
	case SCANLINES:
		if(m_nScanLineL>0) {
			pLine = new CPolyCoord[m_nScanLineL];
			for(nLine=0;nLine<m_nScanLineL;nLine++) pLine[nLine] = &m_ScanLine[nLine];
			SAFE_DELETE_ARRAY(m_ScanLine);
			m_ScanLine = new CPolyCoord[m_nScanLineL + 1];
			for(nLine=0;nLine<m_nScanLineL;nLine++) m_ScanLine[nLine] = &pLine[nLine];
			m_nScanLineL++;
		} else {
			m_ScanLine = new CPolyCoord[m_nScanLineL=1];
		}
		m_ScanLine[m_nScanLineL-1].Zero();
		break;
	}

	SAFE_DELETE_ARRAY( pLine );
	m_bModified = TRUE;
}

void CProfile::DecodeGotoLine(CString Buff, COORDDATA *pCp)
{
	
	int	ii,jj;
	char str[50];
	char cV;

	int nLength=Buff.GetLength();

	for(ii=7,jj=0;(cV=(char)Buff.GetAt(ii))!=',';ii++,jj++) str[jj]=cV;
	str[jj]=0;
	pCp->Side0.fX=(float)atof(str);

	for(++ii,jj=0;(cV= (char)Buff.GetAt(ii))!=',';ii++,jj++) str[jj]=cV;
	str[jj]=0;
	pCp->Side0.fY=(float)atof(str);

	for(++ii,jj=0;(cV= (char)Buff.GetAt(ii))!=',';ii++,jj++) str[jj]=cV;
	str[jj]=0;
	pCp->Side0.fZ=(float)atof(str);

	for(++ii,jj=0;(cV= (char)Buff.GetAt(ii))!=',';ii++,jj++) str[jj]=cV;
	str[jj]=0;
	pCp->Side0.fI=(float)atof(str);

	for(++ii,jj=0;(cV= (char)Buff.GetAt(ii))!=',';ii++,jj++) str[jj]=cV;
	str[jj]=0;
	pCp->Side0.fJ=(float)atof(str);

	for(++ii,jj=0;(ii<nLength) && ((cV= (char)Buff.GetAt(ii))!='P');ii++,jj++) str[jj]=cV;
	str[jj]=0;
	pCp->Side0.fK=(float)atof(str);

	if(theApp.m_LastSettings.nImportAptUnits==1) {
		pCp->Side0.fX *= MMPERINCH;
		pCp->Side0.fY *= MMPERINCH;
		pCp->Side0.fZ *= MMPERINCH;
	}

	pCp->CopySides(1,0);
	pCp->InvertNorm(STARBOARD);

	theApp.m_Kinematics.TipsFromNormals((CCoord*)pCp, PORTSIDE);
	theApp.m_Kinematics.TipsFromNormals((CCoord*)pCp, STARBOARD);
}

void CProfile::GetTheProfileInTheScanner(int nMode,COORDDATA *pCp)
{

	switch(nMode) {
	case 0:
		m_fTranslateX = m_CpCadHole[0].Side0.fX * -1.0f;
		m_fTranslateY = m_CpCadHole[0].Side0.fY * -1.0f;
		m_fTranslateZ = m_CpCadHole[0].Side0.fZ * -1.0f;
		break;
	case 1:
		m_fTranslateX = pCp->Side0.fX * -1.0f;
		m_fTranslateY = pCp->Side0.fY * -1.0f;
		m_fTranslateZ = pCp->Side0.fZ * -1.0f;
		break;
	}

	AcceptTransform();

}

void CProfile::ReverseLines(int nSrcLine, int nStep,int nStartLine)
{
	int nLine;

	switch(nSrcLine) {
	case TAUGHTLINES: if((m_TaughtLine==NULL) || (m_nTaughtLineL<=0)) return;
		for(nLine = nStartLine; nLine<m_nTaughtLineL ; nLine+=nStep) {
			m_TaughtLine[nLine].ReverseAllCoords();
		}
		break;
	case SCANLINES: if((m_ScanLine==NULL) && (m_nScanLineL<=0)) return;
		for(nLine = nStartLine; nLine<m_nScanLineL ; nLine+=nStep) {
			m_ScanLine[nLine].ReverseAllCoords();
		}
		break;
	case BRAINLINES: if((m_BrainLine==NULL) && (m_nBrainLineL<=0)) return;
		for(nLine = nStartLine; nLine<m_nBrainLineL ; nLine+=nStep) {
			m_BrainLine[nLine].ReverseAllCoords();
		}
		break;
	case ENVELOPELINES: if((m_EnvelopeLine==NULL) && (m_nEnvelopeLineL<=0)) return;
		for(nLine = nStartLine; nLine<m_nEnvelopeLineL ; nLine+=nStep) {
			m_EnvelopeLine[nLine].ReverseAllCoords();
		}
		break;
	}
	m_bModified = TRUE;
}


void CProfile::New()
{
	int ii;

	Zero();
	m_strDescription.Empty();
	m_FileName.Empty();
	for(ii=0;ii<3;ii++) {
		m_CpCadHole[ii].Zero();
		if(m_nMeasuredRefPtsLocked != 1) {
			m_CpMeasuredHole[ii].Zero();
		}
	}

	m_CpHolding.Zero();


}

void CProfile::SortEachLine(int nWhichLines, int nSortAxis)
{
	int	nLine;

	switch(nWhichLines) {
	case TAUGHTLINES: if((m_TaughtLine==NULL) || (m_nTaughtLineL<=0)) return;
		for(nLine = 0; nLine<m_nTaughtLineL ; nLine++) {
			m_TaughtLine[nLine].Sort(nSortAxis);
		}
		break;
	case SCANLINES: if((m_ScanLine==NULL) || (m_nScanLineL<=0)) return;
		for(nLine = 0; nLine<m_nScanLineL ; nLine++) {
			m_ScanLine[nLine].Sort(nSortAxis);
		}
		break;
	case BRAINLINES: if((m_BrainLine==NULL) || (m_nBrainLineL<=0)) return;
		for(nLine = 0; nLine<m_nBrainLineL ; nLine++) {
			m_BrainLine[nLine].Sort(nSortAxis);
		}
		break;
	case ENVELOPELINES: if((m_EnvelopeLine==NULL) || (m_nEnvelopeLineL<=0)) return;
		for(nLine = 0; nLine<m_nEnvelopeLineL ; nLine++) {
			m_EnvelopeLine[nLine].Sort(nSortAxis);
		}
		break;
	case OUTSIDELINES: if((m_LineBlade[OUTSIDE]==NULL) || (m_nLineBladeL[OUTSIDE]<=0)) return;
		for(nLine = 0; nLine<m_nLineBladeL[OUTSIDE] ; nLine++) {
			m_LineBlade[OUTSIDE][nLine].Sort(nSortAxis);
		}
		break;
	case INSIDELINES: if((m_LineBlade[INSIDE]==NULL) || (m_nLineBladeL[INSIDE]<=0)) return;
		for(nLine = 0; nLine<m_nLineBladeL[INSIDE] ; nLine++) {
			m_LineBlade[INSIDE][nLine].Sort(nSortAxis);
		}
		break;
	}
	m_bModified = TRUE;

}

void CProfile::SortLine(int nLineType, int nLine, int nSortAxis)
{

	switch(nLineType) {
	case TAUGHTLINES: if((m_TaughtLine==NULL) || (m_nTaughtLineL<=0)) return;
		m_TaughtLine[nLine].Sort(nSortAxis);
		break;
	case SCANLINES: if((m_ScanLine==NULL) || (m_nScanLineL<=0)) return;
		m_ScanLine[nLine].Sort(nSortAxis);
		break;
	case BRAINLINES: if((m_BrainLine==NULL) || (m_nBrainLineL<=0)) return;
		m_BrainLine[nLine].Sort(nSortAxis);
		break;
	case ENVELOPELINES: if((m_EnvelopeLine==NULL) || (m_nEnvelopeLineL<=0)) return;
		m_EnvelopeLine[nLine].Sort(nSortAxis);
		break;
	case OUTSIDELINES: if((m_LineBlade[OUTSIDE]==NULL) || (m_nLineBladeL[OUTSIDE]<=0)) return;
		m_LineBlade[OUTSIDE][nLine].Sort(nSortAxis);
		break;
	case INSIDELINES: if((m_LineBlade[INSIDE]==NULL) || (m_nLineBladeL[INSIDE]<=0)) return;
		m_LineBlade[INSIDE][nLine].Sort(nSortAxis);
		break;
	}
	m_bModified = TRUE;

}

void CProfile::GenerateScanLines()
{
	


	if((m_TaughtLine==NULL) && (m_nComputeAlgorithm<100)) {
		MessageBox(NULL,_T("There are no taught lines defined"),_T("Error"),MB_OK);
		return;
	}
	switch(m_nComputeAlgorithm) {
	case 0:
	case 1:
	case 2:
	case 5:
		if(m_nTaughtLineL<2) {
			MessageBox(NULL,_T("There are not enough taught lines"),_T("Error"),MB_OK);
			return;
		}
		if(m_EnvelopeLine==NULL) {
			MessageBox(NULL,_T("Envelope has not been defined"),_T("Error"),MB_OK);
			return;
		}
		if(m_EnvelopeLine[0].m_nCoordL<2) {
			MessageBox(NULL,_T("Envelope has not been defined"),_T("Error"),MB_OK);
			return;
		}
		break;
	case 3:
	case 7:
	case 12:
		if(m_nTaughtLineL<1) {
			MessageBox(NULL,_T("There are not enough taught lines"),_T("Error"),MB_OK);
			return;
		}
		break;
	case 8:
	case 9:
		if(m_nTaughtLineL!=3) {
			MessageBox(NULL,_T("There must be exactly 3 lines"),_T("Error"),MB_OK);
			return;
		}
		break;

	}

	if(m_fSlowIncrement<=0.0f) {
		MessageBox(NULL,_T("The slow axis increment has not been set"),_T("Error"),MB_OK);
		return;
	}
	if(m_fFastIncrement<=0.0f) {
		MessageBox(NULL,_T("The fast axis increment has not been set"),_T("Error"),MB_OK);
		return;
	}
	if(m_nFastAxis==m_nSlowAxis){
		MessageBox(NULL,_T("The fast axis is the same as the slow axis"),_T("Error"),MB_OK);
		return;
	}



	SAFE_DELETE_ARRAY( m_ScanLine );
	m_nScanLineL=0;

	switch(m_nComputeAlgorithm) {
	case 0: GenerateScanLinesScanForJNose();
		break;
	case 1: GenerateScanLinesScanParallelToTaughtUsingFastAxis();
		break;
	case 2: GenerateScanLinesScanPerpendicularlToTaught();
		break;
	case 3: GenerateRotoSymetricScanLineV1();
		break;
	case 4: GenerateScanLinesScanForAsymetricDNose();
		break;
	case 5: GenerateScanLinesScanParallelToTaughtUsingAxis30();
		break;
	case 6: GenerateDNoseScanLinesWithStraightLE();
		break;
	case 7:	GenerateInterpolatedScanLines();
		break;
	case 8:	GenerateArcScanLines(0);
		break;
	case 9:	GenerateArcScanLines(1);
		break;
	case 10: GenerateRRArcScanLines();
		break;
	case 11: GenerateInterpolatedLines();
		break;
	case 12: GenerateSingleLineBScan(0);
		break;
	case 13: GenerateRotoSymetricScanLineV2();
		break;
	case 100: GenerateTubeScan(0);
		break;
	case 101: GenerateTubeScan(1);
		break;
	}


	m_bModified = TRUE;

}

void CProfile::GenerateScanLinesScanForJNose()
{
	
	float fLength;
	CCoord CpStart,CpFinish;
	CPolyCoord Line,ToolLine;
	int	nLine,nn;
	float fMaxLength,fVerticalPos,fLinePos;
	CCoord Cp,Cp0,Cp1;
	int	nSlowAxis=2; //Slow axis hardcoded to Z???
	int nGuessNumberLines;

	FindLineLimits(&m_EnvelopeLine[0],m_nEnvelopeLineL,&CpStart,&CpFinish);

	fMaxLength = 0.0f;
	for(nn=0;nn<m_nTaughtLineL;nn++) {
		m_TaughtLine[nn].SetPrimaryAxis(30);
		fLength = m_TaughtLine[nn].GetLength();
		if(fMaxLength<fLength) {
			fMaxLength=fLength;
			nLine = nn;
		}
	}

	CProgressDlg dlg(NULL);
	dlg.Create(CProgressDlg::IDD, NULL);
	dlg.SetProgressText(L"Calculating scan path");
	int nNumberLines = (int)(fMaxLength/m_fSlowIncrement);
	dlg.SetProgressRange(0,nNumberLines);


	nGuessNumberLines = nNumberLines * 2;
	m_ScanLine = new CPolyCoord[nGuessNumberLines];

	for(fVerticalPos=0.0,m_nScanLineL=0;fVerticalPos<fMaxLength;fVerticalPos+=m_fSlowIncrement) {
		dlg.SetProgressPos(m_nScanLineL);
		Line.Zero();
		for(nn=0;nn<m_nTaughtLineL;nn++) {
			m_TaughtLine[nn].CalculateCoord(fVerticalPos,Cp);
			Line.Add(Cp);
		}
		FindScanLineLimitsFromEnvelope(m_nFastAxis,nSlowAxis,Cp.Side0.fZ,&Cp0,&Cp1); //assumes slow axis is z

		ToolLine.Zero();
		for(nn=0;nn<10;nn++) {
			fLinePos = (((float)nn * (CpFinish.fPos[m_nFastAxis] - CpStart.fPos[m_nFastAxis])) / 9.0f) + CpStart.fPos[m_nFastAxis];
			Line.CalculateCoord(fLinePos,Cp);
			ToolLine.Add(Cp);
		}
		if(m_nScanLineL<nGuessNumberLines) {
			m_ScanLine[m_nScanLineL] = &ToolLine;
			m_nScanLineL++;
		} else {
			AddLine(SCANLINES, &ToolLine,nSlowAxis);
		}
	}

	if(m_nScanLineL<nGuessNumberLines) {
		CPolyCoord *pLine = new CPolyCoord[m_nScanLineL];
		for(nn=0;nn<m_nScanLineL;nn++) {
			pLine[nn] = &m_ScanLine[nn];
		}
		delete [] m_ScanLine;
		m_ScanLine = NULL;
		if(m_nScanLineL) {
			m_ScanLine = new CPolyCoord[m_nScanLineL];
			for(nn=0;nn<m_nScanLineL;nn++) {
				m_ScanLine[nn] = &pLine[nn];
			}
		}
		delete [] pLine;
	}

	CalculateDangerZone();
	m_bModified = TRUE;

}


void CProfile::GenerateScanLinesScanParallelToTaughtUsingFastAxis()
{
	

	if(m_nSlowAxis==m_nFastAxis) 
		m_nSlowAxis=0;


	CCoord CpStart,CpFinish,Cp0,Cp1,Cp;
	float fSlow,fFast;
	CPoint pt0,pt1;
	int	nNumberLines,nn,nGuessNumberLines;
	CProgressDlg dlg(NULL);
	CPolyCoord Line;

	FindLineLimits(&m_EnvelopeLine[0],m_nEnvelopeLineL,&CpStart,&CpFinish);

	dlg.Create(CProgressDlg::IDD, NULL);
	dlg.SetProgressText(L"Calculating scan path");

	nNumberLines = (int)((CpFinish.fPos[m_nSlowAxis]-CpStart.fPos[m_nSlowAxis])/m_fSlowIncrement);
	dlg.SetProgressRange(0,nNumberLines);

	//guess the and allocate a lof of scan lines
	nGuessNumberLines = nNumberLines * 3;
	m_ScanLine = new CPolyCoord[nGuessNumberLines];

	int nNumberCoords = 200;

	for(fSlow=CpStart.fPos[m_nSlowAxis],m_nScanLineL=0;fSlow<CpFinish.fPos[m_nSlowAxis];fSlow+=m_fSlowIncrement) {
		dlg.SetProgressPos(m_nScanLineL);

		FindScanLineLimitsFromEnvelope(m_nFastAxis,m_nSlowAxis,fSlow,&Cp0,&Cp1);

		Line.Zero();

		if((Cp1.fPos[m_nFastAxis] - Cp0.fPos[m_nFastAxis]) > 2.0f) {
			for(nn=0;nn<nNumberCoords;nn++) {
				fFast = (((float)nn * (Cp1.fPos[m_nFastAxis] - Cp0.fPos[m_nFastAxis])) / (float)(nNumberCoords-1)) + Cp0.fPos[m_nFastAxis];
				CalculateCoord(m_nFastAxis,m_nSlowAxis,fFast,fSlow,Cp);

				//Populate R??
				Line.Add(Cp);
			}
			SetLineIncrement(&Line);
			if(m_nScanLineL<nGuessNumberLines) {
				m_ScanLine[m_nScanLineL] = &Line;
				m_nScanLineL++;
			} else {
				AddLine(SCANLINES, &Line,m_nSlowAxis);
			}
		}
	}

	if(m_nScanLineL<nGuessNumberLines) {
		CPolyCoord *pLine = new CPolyCoord[m_nScanLineL];
		for(nn=0;nn<m_nScanLineL;nn++) {
			pLine[nn] = &m_ScanLine[nn];
		}
		delete [] m_ScanLine;
		m_ScanLine = NULL;
		if(m_nScanLineL) {
			m_ScanLine = new CPolyCoord[m_nScanLineL];
			for(nn=0;nn<m_nScanLineL;nn++) {
				m_ScanLine[nn] = &pLine[nn];
			}
		}
		delete [] pLine;
	}

	CalculateDangerZone();
	m_bModified = TRUE;

}

void CProfile::GenerateScanLinesScanParallelToTaughtUsingAxis30(float fStepSize)
{
	

	CCoord CpStart,CpFinish,Cp0,Cp1,Cp;
	float fSlow,fPos,fLength;
	CPoint pt0,pt1;
	int	nNumberLines,nn,nGuessNumberLines;
	CProgressDlg dlg(NULL);
	CPolyCoord Line;

	FindLineLimits(&m_EnvelopeLine[0],m_nEnvelopeLineL,&CpStart,&CpFinish);

	dlg.Create(CProgressDlg::IDD, NULL);
	dlg.SetProgressText(L"Calculating scan path");

	nNumberLines = (int)((CpFinish.fPos[m_nSlowAxis]-CpStart.fPos[m_nSlowAxis])/m_fSlowIncrement);
	dlg.SetProgressRange(0,nNumberLines);

	//guess the and allocate a lof of scan lines
	nGuessNumberLines = nNumberLines * 3;
	m_ScanLine = new CPolyCoord[nGuessNumberLines];


	for(fSlow=CpStart.fPos[m_nSlowAxis],m_nScanLineL=0;fSlow<CpFinish.fPos[m_nSlowAxis];fSlow+=m_fSlowIncrement) {
		dlg.SetProgressPos(m_nScanLineL);

//		FindScanLineLimitsFromEnvelope(m_nFastAxis,m_nSlowAxis,fSlow,&Cp0,&Cp1);
		Cp.fPos[m_nSlowAxis] = fSlow;
		FindLineLengthFromTaughtLines(Cp,&fLength);

		Line.Zero();

		if(fLength > 20.0f) {
			
			for(fPos=0.0f;(fPos<=fLength);fPos+=fStepSize) {

				CalculateCoord(30,m_nSlowAxis,fPos,fSlow,Cp);

				//Populate R??
				Line.Add(Cp);
			}
			if( (fPos-fLength) < 49.0f) {
				CalculateCoord(30,m_nSlowAxis,fLength,fSlow,Cp);
				Line.Add(Cp);
			}

			SetLineIncrement(&Line);
			if(m_nScanLineL<nGuessNumberLines) {
				m_ScanLine[m_nScanLineL] = &Line;
				m_nScanLineL++;
			} else {
				AddLine(SCANLINES, &Line,m_nSlowAxis);
			}
		}
	}

	if(m_nScanLineL<nGuessNumberLines) {
		CPolyCoord *pLine = new CPolyCoord[m_nScanLineL];
		for(nn=0;nn<m_nScanLineL;nn++) {
			pLine[nn] = &m_ScanLine[nn];
		}
		delete [] m_ScanLine;
		m_ScanLine = NULL;
		if(m_nScanLineL) {
			m_ScanLine = new CPolyCoord[m_nScanLineL];
			for(nn=0;nn<m_nScanLineL;nn++) {
				m_ScanLine[nn] = &pLine[nn];
			}
		}
		delete [] pLine;
	}

	CalculateDangerZone();
	m_bModified = TRUE;

}

void CProfile::FindLineLengthFromTaughtLines(CCoord Cp, float *fLength)
{
	CCoord Cp0,Cp1;
	float fLength0,fLength1;
	int kLo=0;
	int kHi=1;

	for(int nLine=0;nLine<(m_nTaughtLineL-1);nLine++) {
		m_TaughtLine[nLine].GetCoord(0,&Cp0);
		m_TaughtLine[nLine+1].GetCoord(0,&Cp1);
		if((Cp0.fPos[m_nSlowAxis] <= Cp.fPos[m_nSlowAxis]) && (Cp.fPos[m_nSlowAxis] <= Cp1.fPos[m_nSlowAxis])) {
			kLo=nLine;
			kHi=nLine+1;
		}
	}
	if(Cp.fPos[m_nSlowAxis] > Cp1.fPos[m_nSlowAxis]) {
		kLo=m_nTaughtLineL-2;
		kHi=kLo+1;
	}
	m_TaughtLine[kLo].SetPrimaryAxis(30);
	m_TaughtLine[kHi].SetPrimaryAxis(30);
	fLength0 = m_TaughtLine[kLo].GetLength();
	fLength1 = m_TaughtLine[kHi].GetLength();

	m_TaughtLine[kLo].GetCoord(0,&Cp0);
	m_TaughtLine[kHi].GetCoord(0,&Cp1);


	*fLength = ((fLength1-fLength0) * (Cp.fPos[m_nSlowAxis] - Cp0.fPos[m_nSlowAxis]) / (Cp1.fPos[m_nSlowAxis] - Cp0.fPos[m_nSlowAxis]))  + fLength0;
}

void CProfile::GenerateScanLinesScanPerpendicularlToTaught()
{
	

	CCoord CpStart,CpFinish,Cp0,Cp1,Cp;
	float fSlow,fFast;
	CPoint pt0,pt1;
	int	nNumberLines,nn,nGuessNumberLines;
	CProgressDlg dlg(NULL);
	CPolyCoord Line;

	FindLineLimits(&m_EnvelopeLine[0],m_nEnvelopeLineL,&CpStart,&CpFinish);

	dlg.Create(CProgressDlg::IDD, NULL);
	dlg.SetProgressText(L"Calculating scan path");

	nNumberLines = (int)((CpFinish.fPos[m_nSlowAxis]-CpStart.fPos[m_nSlowAxis])/m_fSlowIncrement);
	dlg.SetProgressRange(0,nNumberLines);

	//guess the and allocate a lof of scan lines
	nGuessNumberLines = nNumberLines * 3;
	m_ScanLine = new CPolyCoord[nGuessNumberLines];

	int nNumberCoords = 200;

	for(fSlow=CpStart.fPos[m_nSlowAxis],m_nScanLineL=0;fSlow<CpFinish.fPos[m_nSlowAxis];fSlow+=m_fSlowIncrement) {
		dlg.SetProgressPos(m_nScanLineL);

		FindScanLineLimitsFromEnvelope(m_nFastAxis,m_nSlowAxis,fSlow,&Cp0,&Cp1);

		Line.Zero();

		if((Cp1.fPos[m_nFastAxis] - Cp0.fPos[m_nFastAxis]) > 2.0f) {
			for(nn=0;nn<nNumberCoords;nn++) {
				fFast = (((float)nn * (Cp1.fPos[m_nFastAxis] - Cp0.fPos[m_nFastAxis])) / (float)(nNumberCoords-1)) + Cp0.fPos[m_nFastAxis];
				
				CalculateCoord(m_nSlowAxis,m_nFastAxis,fSlow,fFast,Cp);
				//Populate R??
				Line.Add(Cp);
			}
			SetLineIncrement(&Line);
			if(m_nScanLineL<nGuessNumberLines) {
				m_ScanLine[m_nScanLineL] = &Line;
				m_nScanLineL++;
			} else {
				AddLine(SCANLINES, &Line,m_nSlowAxis);
			}
		}
	}

	if(m_nScanLineL<nGuessNumberLines) {
		CPolyCoord *pLine = new CPolyCoord[m_nScanLineL];
		for(nn=0;nn<m_nScanLineL;nn++) {
			pLine[nn] = &m_ScanLine[nn];
		}
		delete [] m_ScanLine;
		m_ScanLine = NULL;
		if(m_nScanLineL) {
			m_ScanLine = new CPolyCoord[m_nScanLineL];
			for(nn=0;nn<m_nScanLineL;nn++) {
				m_ScanLine[nn] = &pLine[nn];
			}
		}
		delete [] pLine;
	}

	CalculateDangerZone();
	m_bModified = TRUE;


}

bool CProfile::FindScanLineLimitsFromEnvelope(int nFastAxis,int nSlowAxis,float fPosp,CCoord *CpStart,CCoord *CpFinish)
{
	float	fX0,fX1,fZ0,fZ1,fTemp,fResult;
	int	ii,jj;


	CpStart->fPos[nFastAxis]=10000.0;
	CpFinish->fPos[nFastAxis]=-10000.0;
	CpStart->fPos[nSlowAxis]=fPosp;
	CpFinish->fPos[nSlowAxis]=fPosp;

	if(m_EnvelopeLine[0].m_nCoordL>2) {

		for(ii=0,jj=1;ii<m_EnvelopeLine[0].m_nCoordL;ii++,jj++) {
			jj%=m_EnvelopeLine[0].m_nCoordL;

			fX0=m_EnvelopeLine[0].m_pCp[ii].fPos[nFastAxis];
			fX1=m_EnvelopeLine[0].m_pCp[jj].fPos[nFastAxis];
			fZ0=m_EnvelopeLine[0].m_pCp[ii].fPos[nSlowAxis];
			fZ1=m_EnvelopeLine[0].m_pCp[jj].fPos[nSlowAxis];
			if(fZ0>fZ1) {
				fTemp=fZ0;
				fZ0=fZ1;
				fZ1=fTemp;
			};
			if((fPosp>=fZ0) && (fPosp<=fZ1)) {
				fResult=(fPosp-m_EnvelopeLine[0].m_pCp[ii].fPos[nSlowAxis])*(m_EnvelopeLine[0].m_pCp[jj].fPos[nFastAxis]-m_EnvelopeLine[0].m_pCp[ii].fPos[nFastAxis]);
				fResult/=(m_EnvelopeLine[0].m_pCp[jj].fPos[nSlowAxis]-m_EnvelopeLine[0].m_pCp[ii].fPos[nSlowAxis]);
				fResult += m_EnvelopeLine[0].m_pCp[ii].fPos[nFastAxis];
				if(fResult<=CpStart->fPos[nFastAxis]) CpStart->fPos[nFastAxis]=fResult;
				if(fResult>=CpFinish->fPos[nFastAxis]) CpFinish->fPos[nFastAxis]=fResult;
			};
		};
		if((CpStart->fPos[nFastAxis]<CpFinish->fPos[nFastAxis]) && (CpStart->fPos[nSlowAxis]<=CpFinish->fPos[nSlowAxis])) return TRUE;
		return FALSE;
	} else {
		CpStart->fPos[nFastAxis]=0.0;
		CpStart->fPos[nSlowAxis]=0.0;
		CpFinish->fPos[nFastAxis]=fPosp;
		CpFinish->fPos[nSlowAxis]=fPosp;
		return FALSE;
	}
}

bool CProfile::CalculateCoord(int nFastAxis,int nSlowAxis,float fFastPos, float fSlowPos, CCoord &pCp)
{
	int	nLine;
	CPolyCoord Line;
	CCoord Cp;

	for(nLine=0;nLine<m_nTaughtLineL;nLine++) { //TM create a line in slow axis using taught points that goes through our point
		m_TaughtLine[nLine].SetPrimaryAxis(nFastAxis); //nFastAxis
		m_TaughtLine[nLine].CalculateCoord(fFastPos,Cp);
		Line.Add(Cp);
	}

	if(m_nPerpendicularStyle==0) {
		Line.SetPrimaryAxis(nSlowAxis);
	} else {
		Line.m_nStyle=m_nPerpendicularStyle;
		Line.SetPrimaryAxis(nSlowAxis);
	}
	Line.CalculateCoord(fSlowPos,pCp); //TM calculate vertex at slow position i scan line position

	return FALSE;
}

void CProfile::CalculateTurntableDangerZone()
{
	
	int	nLine,nn;
	float fX, fY;
	CPolyCoord* pLine;
	int	nNumberLines,nLineType;

	float fMult = -1.0f;
	if(theApp.m_Tank.nScannerDescription == SPX_ROBOT) {
		fMult = 1.0f;
	}

	m_CpDanger[DANGER_MINS].Zero();
	m_CpDanger[DANGER_MAXS].Zero();

	m_CpDanger[DANGER_MINS].Side0.pt = D3DXVECTOR3(20000.0f,20000.0f,20000.0f);
	m_CpDanger[DANGER_MINS].Side1.pt = D3DXVECTOR3(20000.0f,20000.0f,20000.0f);
	m_CpDanger[DANGER_MAXS].Side0.pt = D3DXVECTOR3(-20000.0f,-20000.0f,-20000.0f);
	m_CpDanger[DANGER_MAXS].Side1.pt = D3DXVECTOR3(-20000.0f,-20000.0f,-20000.0f);
	m_CpDanger[DANGER_MINS].fRadius[PORTSIDE] = 20000.0f;
	m_CpDanger[DANGER_MINS].fRadius[STARBOARD] = 20000.0f;

	for(nLineType=0;nLineType<2;nLineType++) {
		switch(nLineType) {
		default: pLine = m_TaughtLine;
			nNumberLines = m_nTaughtLineL;
			break;
		case 1: pLine = m_ScanLine;
			nNumberLines = m_nScanLineL;
			break;
		}
		for(nLine=0; nLine<nNumberLines; nLine++) {
			for(nn=0;nn<pLine[nLine].m_nCoordL;nn++) {
				if(theApp.m_Tank.vTTZeroDegrees.z == 0.0f) {
					fX = pLine[nLine].m_pCp[nn].Side0.fX - theApp.m_Tank.vTTCentre.x;
					fY = pLine[nLine].m_pCp[nn].Side0.fY - theApp.m_Tank.vTTCentre.y;
					pLine[nLine].m_pCp[nn].fRadius[PORTSIDE] = hypot(fX, fY);

					fX = pLine[nLine].m_pCp[nn].Side1.fX - theApp.m_Tank.vTTCentre.x;
					fY = pLine[nLine].m_pCp[nn].Side1.fY - theApp.m_Tank.vTTCentre.y;
					pLine[nLine].m_pCp[nn].fRadius[STARBOARD] = hypot(fX, fY);
				};

				float fXSide0 = (theApp.m_Tank.vTTCentre.x - pLine[nLine].m_pCp[nn].fRadius[PORTSIDE]) * fMult;
				float fYSide0 = (theApp.m_Tank.vTTCentre.y - pLine[nLine].m_pCp[nn].fRadius[PORTSIDE]) * fMult;
				float fXSide1 = pLine[nLine].m_pCp[nn].fRadius[STARBOARD] + theApp.m_Tank.vTTCentre.x * fMult;
				float fYSide1 = pLine[nLine].m_pCp[nn].fRadius[STARBOARD] + theApp.m_Tank.vTTCentre.y * fMult;

				if(m_CpDanger[DANGER_MINS].Side0.fX > fXSide0)	m_CpDanger[DANGER_MINS].Side0.fX = fXSide0;
				if(m_CpDanger[DANGER_MINS].Side0.fY > fYSide0)	m_CpDanger[DANGER_MINS].Side0.fY = fYSide0;
				if(m_CpDanger[DANGER_MINS].Side0.fZ > pLine[nLine].m_pCp[nn].Side0.fZ)					m_CpDanger[DANGER_MINS].Side0.fZ = pLine[nLine].m_pCp[nn].Side0.fZ;

				if(m_CpDanger[DANGER_MAXS].Side0.fX < fXSide0)	m_CpDanger[DANGER_MAXS].Side0.fX = fXSide0;
				if(m_CpDanger[DANGER_MAXS].Side0.fY < fYSide0)	m_CpDanger[DANGER_MAXS].Side0.fY = fYSide0;
				if(m_CpDanger[DANGER_MAXS].Side0.fZ < pLine[nLine].m_pCp[nn].Side0.fZ)			m_CpDanger[DANGER_MAXS].Side0.fZ = pLine[nLine].m_pCp[nn].Side0.fZ;

				if(m_CpDanger[DANGER_MINS].Side1.fX > fXSide1)	m_CpDanger[DANGER_MINS].Side1.fX = fXSide1;
				if(m_CpDanger[DANGER_MINS].Side1.fY > fYSide1)	m_CpDanger[DANGER_MINS].Side1.fY = fYSide1;
				if(m_CpDanger[DANGER_MINS].Side1.fZ > pLine[nLine].m_pCp[nn].Side1.fZ)						m_CpDanger[DANGER_MINS].Side1.fZ = pLine[nLine].m_pCp[nn].Side1.fZ;

				if(m_CpDanger[DANGER_MAXS].Side1.fX < fXSide1)	m_CpDanger[DANGER_MAXS].Side1.fX = fXSide1;
				if(m_CpDanger[DANGER_MAXS].Side1.fY < fYSide1)	m_CpDanger[DANGER_MAXS].Side1.fY = fYSide1;
				if(m_CpDanger[DANGER_MAXS].Side1.fZ < pLine[nLine].m_pCp[nn].Side1.fZ)				m_CpDanger[DANGER_MAXS].Side1.fZ = pLine[nLine].m_pCp[nn].Side1.fZ;

				if(m_CpDanger[DANGER_MINS].fRadius[PORTSIDE] > pLine[nLine].m_pCp[nn].fRadius[PORTSIDE])	m_CpDanger[DANGER_MINS].fRadius[PORTSIDE] = pLine[nLine].m_pCp[nn].fRadius[PORTSIDE];
				if(m_CpDanger[DANGER_MINS].fRadius[STARBOARD] > pLine[nLine].m_pCp[nn].fRadius[STARBOARD])	m_CpDanger[DANGER_MINS].fRadius[STARBOARD] = pLine[nLine].m_pCp[nn].fRadius[STARBOARD];
				if(m_CpDanger[DANGER_MAXS].fRadius[PORTSIDE] < pLine[nLine].m_pCp[nn].fRadius[PORTSIDE])	m_CpDanger[DANGER_MAXS].fRadius[PORTSIDE] = pLine[nLine].m_pCp[nn].fRadius[PORTSIDE];
				if(m_CpDanger[DANGER_MAXS].fRadius[STARBOARD] < pLine[nLine].m_pCp[nn].fRadius[STARBOARD])	m_CpDanger[DANGER_MAXS].fRadius[STARBOARD] = pLine[nLine].m_pCp[nn].fRadius[STARBOARD];
			}
		}
		if(theApp.m_Tank.vTTZeroDegrees.y == 0.0f) {
			m_CpDanger[0].Side0.fX -= theApp.m_LastSettings.fDangerMargin[0];
			m_CpDanger[0].Side1.fX += theApp.m_LastSettings.fDangerMargin[0];
			m_CpDanger[1].Side0.fX -= theApp.m_LastSettings.fDangerMargin[0];
			m_CpDanger[1].Side1.fX += theApp.m_LastSettings.fDangerMargin[0];
			m_CpDanger[0].Side0.fI = -1.0f;
			m_CpDanger[0].Side1.fI = 1.0f;
		}
		if(theApp.m_Tank.vTTZeroDegrees.x == 0.0f) {
			m_CpDanger[0].Side0.fY -= theApp.m_LastSettings.fDangerMargin[0];
			m_CpDanger[0].Side1.fY += theApp.m_LastSettings.fDangerMargin[0];
			m_CpDanger[0].Side0.fJ = -1.0f;
			m_CpDanger[0].Side1.fJ = 1.0f;
		}
		m_CpDanger[1].fRadius[STARBOARD] += theApp.m_LastSettings.fDangerMargin[0];
		m_CpDanger[0].fRadius[STARBOARD] -= theApp.m_LastSettings.fDangerMargin[0];
	}


}


void CProfile::CalculateDangerZone()
{
	
	int	nLine,nn;
	CPolyCoord* pLine;
	int	nNumberLines,nLineType;
	CCoord CpMin,CpMax;


	if(m_nComponentDescription != RECTILINEAR) {
		CalculateTurntableDangerZone();
		return;
	}

	m_CpDanger[DANGER_MINS].Zero();
	m_CpDanger[DANGER_MAXS].Zero();

	if(theApp.m_LastSettings.nManipulatorType[0] == SWIVEL_Z_OFFSET_GIMBAL) {

		if((m_nFastAxis == 0) && (m_nSlowAxis == 1)) { // X - Y
			m_CpDanger[DANGER_MAXS].Side0.norm =m_CpDanger[DANGER_MINS].Side0.norm = D3DXVECTOR4(0.0f,0.0f,1.0f,0.0f);
		}
		if((m_nFastAxis == 0) && (m_nSlowAxis == 2)) { // X - Z
			m_CpDanger[DANGER_MAXS].Side0.norm = m_CpDanger[DANGER_MINS].Side0.norm = D3DXVECTOR4(0.0f,-1.0f,0.0f,0.0f);
		}
		if((m_nFastAxis == 1) && (m_nSlowAxis == 2)) { // Y - Z
			m_CpDanger[DANGER_MAXS].Side0.norm = m_CpDanger[DANGER_MINS].Side0.norm = D3DXVECTOR4(1.0f,0.0f,0.0f,0.0f);
		}
	} else {

		if((m_nFastAxis == 0) && (m_nSlowAxis == 1)) { // X - Y
			m_CpDanger[DANGER_MAXS].Side0.norm =m_CpDanger[DANGER_MINS].Side0.norm = D3DXVECTOR4(0.0f,0.0f,-1.0f,0.0f);
		}
		if((m_nFastAxis == 0) && (m_nSlowAxis == 2)) { // X - Z
			m_CpDanger[DANGER_MAXS].Side0.norm = m_CpDanger[DANGER_MINS].Side0.norm = D3DXVECTOR4(0.0f,-1.0f,0.0f,0.0f);
		}
		if((m_nFastAxis == 1) && (m_nSlowAxis == 2)) { // Y - Z
			m_CpDanger[DANGER_MAXS].Side0.norm = m_CpDanger[DANGER_MINS].Side0.norm = D3DXVECTOR4(-1.0f,0.0f,0.0f,0.0f);
		}

	}

	theApp.m_Kinematics.TipsFromNormals(&m_CpDanger[DANGER_MINS],PORTSIDE);
	theApp.m_Kinematics.TipsFromNormals(&m_CpDanger[DANGER_MAXS],PORTSIDE);

	m_CpDanger[DANGER_MINS].Side0.fX = m_CpDanger[DANGER_MINS].Side0.fY = m_CpDanger[DANGER_MINS].Side0.fZ = 20000.0f; 
	m_CpDanger[DANGER_MAXS].Side0.fX = m_CpDanger[DANGER_MAXS].Side0.fY = m_CpDanger[DANGER_MAXS].Side0.fZ = -20000.0f;

	m_CpDanger[DANGER_MINS].fRadius[PORTSIDE] = 100000.0f;
	m_CpDanger[DANGER_MINS].fRadius[STARBOARD] = 100000.0f;

	for(nLineType=0;nLineType<4;nLineType++) {
		switch(nLineType) {
		default: pLine = m_TaughtLine;
			nNumberLines = m_nTaughtLineL;
			break;
		case 1: pLine = m_ScanLine;
			nNumberLines = m_nScanLineL;
			break;
		case 2: pLine = m_LineBlade[OUTSIDE];
			nNumberLines = m_nLineBladeL[OUTSIDE];
			break;
		case 3: pLine = m_LineBlade[INSIDE];
			nNumberLines = m_nLineBladeL[INSIDE];
			break;
		}

		for(nLine=0; nLine<nNumberLines; nLine++) {
			for(nn=0;nn<pLine[nLine].m_nCoordL;nn++) {
				if(m_CpDanger[DANGER_MINS].Side0.fX > pLine[nLine].m_pCp[nn].Side0.fX) m_CpDanger[DANGER_MINS].Side0.fX = pLine[nLine].m_pCp[nn].Side0.fX;
				if(m_CpDanger[DANGER_MINS].Side0.fY > pLine[nLine].m_pCp[nn].Side0.fY) m_CpDanger[DANGER_MINS].Side0.fY = pLine[nLine].m_pCp[nn].Side0.fY;
				if(m_CpDanger[DANGER_MINS].Side0.fZ > pLine[nLine].m_pCp[nn].Side0.fZ) m_CpDanger[DANGER_MINS].Side0.fZ = pLine[nLine].m_pCp[nn].Side0.fZ;

				if(m_CpDanger[DANGER_MAXS].Side0.fX < pLine[nLine].m_pCp[nn].Side0.fX) m_CpDanger[DANGER_MAXS].Side0.fX = pLine[nLine].m_pCp[nn].Side0.fX;
				if(m_CpDanger[DANGER_MAXS].Side0.fY < pLine[nLine].m_pCp[nn].Side0.fY) m_CpDanger[DANGER_MAXS].Side0.fY = pLine[nLine].m_pCp[nn].Side0.fY;
				if(m_CpDanger[DANGER_MAXS].Side0.fZ < pLine[nLine].m_pCp[nn].Side0.fZ) m_CpDanger[DANGER_MAXS].Side0.fZ = pLine[nLine].m_pCp[nn].Side0.fZ;

				if(m_CpDanger[DANGER_MINS].fRadius[PORTSIDE] > pLine[nLine].m_pCp[nn].fRadius[PORTSIDE]) m_CpDanger[DANGER_MINS].fRadius[PORTSIDE] = pLine[nLine].m_pCp[nn].fRadius[PORTSIDE];
				if(m_CpDanger[DANGER_MINS].fRadius[STARBOARD] > pLine[nLine].m_pCp[nn].fRadius[STARBOARD]) m_CpDanger[DANGER_MINS].fRadius[STARBOARD] = pLine[nLine].m_pCp[nn].fRadius[STARBOARD];
				if(m_CpDanger[DANGER_MAXS].fRadius[PORTSIDE] < pLine[nLine].m_pCp[nn].fRadius[PORTSIDE]) m_CpDanger[DANGER_MAXS].fRadius[PORTSIDE] = pLine[nLine].m_pCp[nn].fRadius[PORTSIDE];
				if(m_CpDanger[DANGER_MAXS].fRadius[STARBOARD] < pLine[nLine].m_pCp[nn].fRadius[STARBOARD]) m_CpDanger[DANGER_MAXS].fRadius[STARBOARD] = pLine[nLine].m_pCp[nn].fRadius[STARBOARD];


				//If using both sides we should consider the other sides position
				if(theApp.m_LastSettings.bEnableSide[0] == theApp.m_LastSettings.bEnableSide[1] == 1) {
					if(m_CpDanger[DANGER_MINS].Side0.fX > pLine[nLine].m_pCp[nn].Side1.fX) 
						m_CpDanger[DANGER_MINS].Side0.fX = pLine[nLine].m_pCp[nn].Side1.fX;
					if(m_CpDanger[DANGER_MINS].Side0.fY > pLine[nLine].m_pCp[nn].Side1.fY) 
						m_CpDanger[DANGER_MINS].Side0.fY = pLine[nLine].m_pCp[nn].Side1.fY;
					if(m_CpDanger[DANGER_MINS].Side0.fZ > pLine[nLine].m_pCp[nn].Side1.fZ) 
						m_CpDanger[DANGER_MINS].Side0.fZ = pLine[nLine].m_pCp[nn].Side1.fZ;

					if(m_CpDanger[DANGER_MAXS].Side0.fX < pLine[nLine].m_pCp[nn].Side1.fX) 
						m_CpDanger[DANGER_MAXS].Side0.fX = pLine[nLine].m_pCp[nn].Side1.fX;
					if(m_CpDanger[DANGER_MAXS].Side0.fY < pLine[nLine].m_pCp[nn].Side1.fY) 
						m_CpDanger[DANGER_MAXS].Side0.fY = pLine[nLine].m_pCp[nn].Side1.fY;
					if(m_CpDanger[DANGER_MAXS].Side0.fZ < pLine[nLine].m_pCp[nn].Side1.fZ) 
						m_CpDanger[DANGER_MAXS].Side0.fZ = pLine[nLine].m_pCp[nn].Side1.fZ;
				}

			}
		}
	}


	m_CpDanger[DANGER_MINS].Side1 = m_CpDanger[DANGER_MINS].Side0;
	m_CpDanger[DANGER_MAXS].Side1 = m_CpDanger[DANGER_MAXS].Side0;
	CpMin = m_CpDanger[DANGER_MINS];
	CpMax = m_CpDanger[DANGER_MAXS];

	if(theApp.m_LastSettings.nManipulatorType[0] == SWIVEL_Z_OFFSET_GIMBAL) {
		m_CpDanger[DANGER_MAXS].Side0.fX = CpMin.Side1.fX;
		m_CpDanger[DANGER_MINS].Side0.fX = CpMax.Side1.fX;
		m_CpDanger[DANGER_MAXS].Side1.fX = CpMin.Side0.fX;
		m_CpDanger[DANGER_MINS].Side1.fX = CpMax.Side0.fX;

	}
	D3DXVECTOR3 vect;
	vect = m_vecToDangerPlane[0] * theApp.m_LastSettings.fDangerMargin[0];
	m_CpDanger[DANGER_MINS].Side0.pt += vect;
	m_CpDanger[DANGER_MAXS].Side0.pt += vect;

	vect = m_vecToDangerPlane[1] * theApp.m_LastSettings.fDangerMargin[1];
	m_CpDanger[DANGER_MINS].Side1.pt += vect;
	m_CpDanger[DANGER_MAXS].Side1.pt += vect;

	m_CpDanger[DANGER_MINS].fRadius[PORTSIDE] += theApp.m_LastSettings.fDangerMargin[0];
	m_CpDanger[DANGER_MINS].fRadius[STARBOARD] -= theApp.m_LastSettings.fDangerMargin[1];

	m_CpDanger[DANGER_MAXS].fRadius[PORTSIDE] += theApp.m_LastSettings.fDangerMargin[0];
	m_CpDanger[DANGER_MAXS].fRadius[STARBOARD] -= theApp.m_LastSettings.fDangerMargin[1];

}




int CProfile::FindMaximumSamplesLine(int *nSamplesLine)
{
	float fMaximumLineLength = 0.0;
	float fLineLength,fStartLength;
	int	nLine;
	CCoord CpMin,CpMax,Cp;

	FindBoundingCube(SCANLINES,&CpMin,&CpMax);

	if(m_ScanLine==NULL) return 0;
	switch(m_nScanType) {
	default:
		if(m_nScanLineL<=0) return 0;
		if(m_nScanFinishLine >= m_nScanLineL) m_nScanFinishLine = m_nScanLineL-1;
		for(nLine = m_nScanStartLine;nLine <= m_nScanFinishLine; nLine++) {
			m_ScanLine[nLine].SetPrimaryAxis(30);
			fLineLength = m_ScanLine[nLine].GetLength();
			m_ScanLine[nLine].GetCoord(0,&Cp);
			switch(m_nSlowAxis) {
			case 0: CpMin.Side0.fX = Cp.Side0.fX;
				break;
			case 1: CpMin.Side0.fY = Cp.Side0.fY;
				break;
			case 2: CpMin.Side0.fZ = Cp.Side0.fZ;
				break;
			}
			fStartLength = CpMin.Length(Cp);
			switch(m_nFastAxis) {
			case 0: fStartLength = fabs(Cp.Side0.fX - CpMin.Side0.fX);
				break;
			case 1: fStartLength = fabs(Cp.Side0.fY - CpMin.Side0.fY);
				break;
			case 2: fStartLength = fabs(Cp.Side0.fZ - CpMin.Side0.fZ);
				break;
			}
			fLineLength += fStartLength;
			if(fLineLength > fMaximumLineLength) fMaximumLineLength = fLineLength;
		}
		*nSamplesLine = (int)(fMaximumLineLength / m_fFastIncrement) + 1;
		break;
	case TURNTABLE_SCAN:
		if(m_nScanLineL<=0) return 0;
		if(m_nScanFinishLine >= m_ScanLine[0].m_nCoordL) m_nScanFinishLine = m_ScanLine[0].m_nCoordL-1;
		*nSamplesLine = (int)(360.0 / m_fFastIncrement) + 1;
		break;
	case CONTINUOUS_TURNTABLE_SCAN:
		if(m_nScanLineL<=0) return 0;
		if(m_nScanFinishLine >= m_ScanLine[0].m_nCoordL) m_nScanFinishLine = m_ScanLine[0].m_nCoordL-1;
		*nSamplesLine = (int)(360.0 / m_fFastIncrement) + 1;
		break;
	case SEGMENT_TURNTABLE_SCAN:
		if(m_nScanLineL<=0) return 0;
		if(m_nScanFinishLine >= m_ScanLine[0].m_nCoordL) m_nScanFinishLine = m_ScanLine[0].m_nCoordL-1;
		*nSamplesLine = (int)((m_CpRect[1].Side0.fR - m_CpRect[0].Side0.fR) / m_fFastIncrement) + 1;
		break;

	}


	return *nSamplesLine;
}

int CProfile::FindNumberScanLines(int *pnNumberScanLines)
{
	

	if(m_nLineIncrement<=0) m_nLineIncrement=1;
	*pnNumberScanLines = (m_nScanFinishLine - m_nScanStartLine + 1) / m_nLineIncrement;

	return *pnNumberScanLines;

}

void CProfile::FindScanSizes(float *pfFastScanSize, float *pfSlowScanSize,CCoord *CpStart,CCoord * CpFinish)
{
	CCoord Cp;
	CCoord Cp1;
	int	nLine;
	float	fHTotal,fH,fX,fY,fZ,fMaxLength=1e9f;

	CpStart->Zero();
	CpFinish->Zero();
	if (m_ScanLine) {
		CpStart->Side0.fX = 20000.0f;
		CpStart->Side0.fY = 20000.0f;
		CpStart->Side0.fZ = 20000.0f;
		CpFinish->Side0.fX = -20000.0f;
		CpFinish->Side0.fY = -20000.0f;
		CpFinish->Side0.fZ = -20000.0f;

		switch (m_nScanType) {
		default:
			for (nLine = m_nScanStartLine; nLine <= m_nScanFinishLine; nLine++) {
				m_ScanLine[nLine].PreProcessLine();
				if (fMaxLength < m_ScanLine[nLine].GetLength()) fMaxLength = m_ScanLine[nLine].GetLength();
				Cp = m_ScanLine[nLine].m_pCp[0];
				if (CpStart->Side0.fX > Cp.Side0.fX) CpStart->Side0.fX = Cp.Side0.fX;
				if (CpStart->Side0.fY > Cp.Side0.fY) CpStart->Side0.fY = Cp.Side0.fY;
				if (CpStart->Side0.fZ > Cp.Side0.fZ) CpStart->Side0.fZ = Cp.Side0.fZ;
				if (CpFinish->Side0.fX < Cp.Side0.fX) CpFinish->Side0.fX = Cp.Side0.fX;
				if (CpFinish->Side0.fY < Cp.Side0.fY) CpFinish->Side0.fY = Cp.Side0.fY;
				if (CpFinish->Side0.fZ < Cp.Side0.fZ) CpFinish->Side0.fZ = Cp.Side0.fZ;
				Cp = m_ScanLine[nLine].m_pCp[m_ScanLine[nLine].m_nCoordL - 1];
				if (CpStart->Side0.fX > Cp.Side0.fX) CpStart->Side0.fX = Cp.Side0.fX;
				if (CpStart->Side0.fY > Cp.Side0.fY) CpStart->Side0.fY = Cp.Side0.fY;
				if (CpStart->Side0.fZ > Cp.Side0.fZ) CpStart->Side0.fZ = Cp.Side0.fZ;
				if (CpFinish->Side0.fX < Cp.Side0.fX) CpFinish->Side0.fX = Cp.Side0.fX;
				if (CpFinish->Side0.fY < Cp.Side0.fY) CpFinish->Side0.fY = Cp.Side0.fY;
				if (CpFinish->Side0.fZ < Cp.Side0.fZ) CpFinish->Side0.fZ = Cp.Side0.fZ;
			}
			//	*pfFastScanSize = fMaxLength; // This was the origional one but it doesn't compensate for parallelograms
			*pfFastScanSize = CpFinish->Side0.fPos[m_nFastAxis] - CpStart->Side0.fPos[m_nFastAxis];

			fHTotal = 0.0f;
			for (nLine = m_nScanStartLine; nLine < (m_nScanFinishLine - 1); nLine++) {
				Cp = m_ScanLine[nLine].m_pCp[m_ScanLine[nLine].m_nCoordL / 2];
				Cp1 = m_ScanLine[nLine + 1].m_pCp[m_ScanLine[nLine + 1].m_nCoordL / 2];
				switch (m_nFastAxis) {
				case 0:
					fY = Cp.Side0.fY - Cp1.Side0.fY;
					fZ = Cp.Side0.fZ - Cp1.Side0.fZ;
					break;
				case 1:
					fY = Cp.Side0.fX - Cp1.Side0.fX;
					fZ = Cp.Side0.fZ - Cp1.Side0.fZ;
					break;
				case 2:
					fY = Cp.Side0.fX - Cp1.Side0.fX;
					fZ = Cp.Side0.fY - Cp1.Side0.fY;
					break;
				}
				fH = (float)sqrt((double)(fY*fY + fZ * fZ));
				fHTotal += fH;
			}
			*pfSlowScanSize = fHTotal;
			break;
		case TURNTABLE_SCAN:
		case CONTINUOUS_TURNTABLE_SCAN:
			CpStart->Side1.fR = CpStart->Side0.fR = 0.0f;
			CpFinish->Side1.fR = CpFinish->Side0.fR = 360.0f;
			*pfFastScanSize = CpFinish->Side0.fR - CpStart->Side0.fR;
			fHTotal = 0.0f;
			for (nLine = m_nScanStartLine; nLine < (m_nScanFinishLine - 1); nLine++) {
				Cp = m_ScanLine[0].m_pCp[nLine];
				if (CpStart->Side0.fX > Cp.Side0.fX) CpStart->Side0.fX = Cp.Side0.fX;
				if (CpStart->Side0.fY > Cp.Side0.fY) CpStart->Side0.fY = Cp.Side0.fY;
				if (CpStart->Side0.fZ > Cp.Side0.fZ) CpStart->Side0.fZ = Cp.Side0.fZ;
				if (CpFinish->Side0.fX < Cp.Side0.fX) CpFinish->Side0.fX = Cp.Side0.fX;
				if (CpFinish->Side0.fY < Cp.Side0.fY) CpFinish->Side0.fY = Cp.Side0.fY;
				if (CpFinish->Side0.fZ < Cp.Side0.fZ) CpFinish->Side0.fZ = Cp.Side0.fZ;

				Cp1 = m_ScanLine[0].m_pCp[nLine + 1];
				fX = Cp.Side0.fX - Cp1.Side0.fX;
				fY = Cp.Side0.fY - Cp1.Side0.fY;
				fZ = Cp.Side0.fZ - Cp1.Side0.fZ;
				fH = (float)sqrt((double)(fX*fX + fY * fY + fZ * fZ));
				fHTotal += fH;
			}
			*pfSlowScanSize = fHTotal;
			break;
		case SEGMENT_TURNTABLE_SCAN:
			CpStart->Side1.fR = CpStart->Side0.fR = m_CpRect[0].Side0.fR;
			CpFinish->Side1.fR = CpFinish->Side0.fR = m_CpRect[1].Side0.fR;
			*pfFastScanSize = CpFinish->Side0.fR - CpStart->Side0.fR;
			fHTotal = 0.0f;
			for (nLine = m_nScanStartLine; nLine < (m_nScanFinishLine - 1); nLine++) {
				Cp = m_ScanLine[0].m_pCp[nLine];
				if (CpStart->Side0.fX > Cp.Side0.fX) CpStart->Side0.fX = Cp.Side0.fX;
				if (CpStart->Side0.fY > Cp.Side0.fY) CpStart->Side0.fY = Cp.Side0.fY;
				if (CpStart->Side0.fZ > Cp.Side0.fZ) CpStart->Side0.fZ = Cp.Side0.fZ;
				if (CpFinish->Side0.fX < Cp.Side0.fX) CpFinish->Side0.fX = Cp.Side0.fX;
				if (CpFinish->Side0.fY < Cp.Side0.fY) CpFinish->Side0.fY = Cp.Side0.fY;
				if (CpFinish->Side0.fZ < Cp.Side0.fZ) CpFinish->Side0.fZ = Cp.Side0.fZ;

				Cp1 = m_ScanLine[0].m_pCp[nLine + 1];
				fX = Cp.Side0.fX - Cp1.Side0.fX;
				fY = Cp.Side0.fY - Cp1.Side0.fY;
				fZ = Cp.Side0.fZ - Cp1.Side0.fZ;
				fH = (float)sqrt((double)(fY*fY + fZ * fZ));
				fHTotal += fH;
			}
			*pfSlowScanSize = fHTotal;
			break;
		}
	}
}

bool CProfile::GetTaughtCoord(int nLine, int nIndex, CCoord *pCoord)
{

	if(nLine < m_nTaughtLineL) {
		if(nIndex < m_TaughtLine[nLine].m_nCoordL) {
			CopyMemory(&pCoord->nStructSize, &m_TaughtLine[nLine].m_pCp[nIndex].nStructSize, sizeof COORDDATA);
			return TRUE;
		}
	}
	return FALSE;
}

int CProfile::GetTaughtLineCoordL(int nLine)
{
	if(nLine < m_nTaughtLineL) {
		return m_TaughtLine[nLine].m_nCoordL;
	}
	return -1;

}

void CProfile::ModifyTaughtLine(int nLine,CPolyCoord *pLine)
{

	if(m_TaughtLine) {
		if(nLine <m_nTaughtLineL) {
			m_TaughtLine[nLine].Zero();
			m_TaughtLine[nLine] = pLine;
		}
	}
	m_bModified = TRUE;
}

void CProfile::CalculateOppositeTaughtSide(int nSide)
{
	
	int	nn;
	CCoord Cp;

	if(m_TaughtLine==NULL) return;
	for(int nLine=0;nLine<m_nTaughtLineL;nLine++) {
		for(nn=0;nn<m_TaughtLine[nLine].m_nCoordL;nn++) {
			Cp = m_TaughtLine[nLine].m_pCp[nn];
			theApp.m_Kinematics.GetComponentThickness(&Cp);
			theApp.m_Kinematics.CalculateOppositeCoord(nSide,&Cp,Cp.fThickness);			
			m_TaughtLine[nLine].m_pCp[nn] = Cp;
		}
	}

}

bool CProfile::GetSurfaceCoordinate(CCoord &pCp, CPoint ptSample, float fPos,int nFastAxis,bool bGenerateVirtualXY)
{
	
	bool bFlag=TRUE;
	float fRadius;
	pCp.Zero();


	if(m_ScanLine == NULL) return FALSE;
	if(m_nScanLineL <= 0) return FALSE;

	switch(nFastAxis) {
	default:
		if(ptSample.y<0) return FALSE;
		if(ptSample.y>=m_nScanLineL) return FALSE;
		m_ScanLine[ptSample.y].m_nEol[0]=EOL_NO_EXTRAPOLATION;
		m_ScanLine[ptSample.y].m_nEol[1]=EOL_NO_EXTRAPOLATION;
//		m_ScanLine[ptSample.y].m_nEol[0]=EOL_EXTRAPOLATION;
//		m_ScanLine[ptSample.y].m_nEol[1]=EOL_EXTRAPOLATION;

		m_ScanLine[ptSample.y].m_nPrimaryAxis = m_nFastAxis;
		bFlag = m_ScanLine[ptSample.y].CalculateCoord(fPos,pCp);
		break;
	case 5:
		if(ptSample.y<0) return FALSE;
		if(ptSample.y>=m_ScanLine[0].m_nCoordL) return FALSE;

		pCp = m_ScanLine[0].m_pCp[ptSample.y];
		pCp.Side0.fR = fPos;
		if(bGenerateVirtualXY == true) {
			fRadius = sqrtf(pCp.Side0.fX * pCp.Side0.fX + pCp.Side0.fY * pCp.Side0.fY);
			pCp.Side0.fX = -fRadius * cosf(pCp.Side0.fR * DEG_TO_RAD);
			pCp.Side0.fY = -fRadius * sinf(pCp.Side0.fR * DEG_TO_RAD);
		}
		break;
	case 6:
		if(ptSample.y<0) return FALSE;
		if(ptSample.y>=m_ScanLine[0].m_nCoordL) return FALSE;

		pCp = m_ScanLine[0].m_pCp[ptSample.y];
		pCp.Side1.fR = fPos;
		if(bGenerateVirtualXY == true) {
			fRadius = sqrtf(pCp.Side0.fX * pCp.Side0.fX + pCp.Side0.fY * pCp.Side0.fY);
			pCp.Side0.fX = -fRadius * cosf(pCp.Side1.fR * DEG_TO_RAD);
			pCp.Side0.fY = -fRadius * sinf(pCp.Side1.fR * DEG_TO_RAD);
		}
		break;
	}
	return bFlag;

}


void CProfile::CalculateMathematicalNormals()
{
	CCoord Cp0,CpA,CpB,Cp;
	int	nLine,ii=0;
	int	nS;
	D3DXVECTOR3 OA, OB, vecNorm;
	D3DXVECTOR4 vec;
	int nSegment;

	if(m_nScanLineL <= 0) return;
	if(m_ScanLine == NULL) return;

	vec.w = 0.0f;

	for(nLine=0;nLine<(m_nScanLineL-1);nLine++) {

		for(nS=0;nS<(m_ScanLine[nLine].m_nCoordL-1);nS++) {

			m_ScanLine[nLine].GetCoordComeWhatMay(nS,&Cp0);
			m_ScanLine[nLine].GetCoordComeWhatMay(nS+1,&CpA);
			m_ScanLine[nLine+1].GetCoordComeWhatMay(nS,&CpB);

			OA = Cp0.Side0.pt - CpA.Side0.pt;
			OB = Cp0.Side0.pt - CpB.Side0.pt;

			D3DXVec3Cross(&vecNorm, &OB, &OA);
			D3DXVec3Normalize( &vecNorm, &vecNorm );

			vec.x = vecNorm.x;
			vec.y = vecNorm.y;
			vec.z = vecNorm.z;

			vec.x *= -1.0f;
			vec.y *= -1.0f;
			vec.z *= -1.0f;

			m_ScanLine[nLine].ModifyBothNorms(nS,vec);
			m_ScanLine[nLine].ComputeBothTipsFromNormals(nS);


		}
	}

	//Trailing edge
	for(nLine=0;nLine<(m_nScanLineL-1);nLine++) {

		nS = m_ScanLine[nLine].m_nCoordL-1;

		m_ScanLine[nLine].GetCoordComeWhatMay(nS,&Cp0);
		m_ScanLine[nLine].GetCoordComeWhatMay(nS-1,&CpB);
		m_ScanLine[nLine+1].GetCoordComeWhatMay(nS,&CpA);

		OA = Cp0.Side0.pt - CpA.Side0.pt;
		OB = Cp0.Side0.pt - CpB.Side0.pt;

		D3DXVec3Cross(&vecNorm, &OB, &OA);
		D3DXVec3Normalize( &vecNorm, &vecNorm );

		vec.x = vecNorm.x;
		vec.y = vecNorm.y;
		vec.z = vecNorm.z;

		vec.x *= -1.0f;
		vec.y *= -1.0f;
		vec.z *= -1.0f;

		m_ScanLine[nLine].ModifyBothNorms(nS,vec);
		m_ScanLine[nLine].ComputeBothTipsFromNormals(nS);
	}

	//Top edge
	int nTopLine = m_nScanLineL-1;
	for(nS=0;nS<m_ScanLine[nTopLine].m_nCoordL;nS++) {
		m_ScanLine[nTopLine].GetCoord(nS,&Cp0);

		m_ScanLine[nTopLine-1].FindNearestSegment(Cp0,0,&nSegment);
		m_ScanLine[nTopLine-1].GetCoord(nSegment,&CpA);
		m_ScanLine[nTopLine].ModifyBothNorms(nS,CpA.Side0.norm);
		m_ScanLine[nLine].ComputeBothTipsFromNormals(nS);
	}


}

float CProfile::VectorLength(VectorStruct Vp)
{
	float fResult = (float)sqrt((double)(Vp.fI*Vp.fI + Vp.fJ*Vp.fJ + Vp.fK*Vp.fK));

	return fResult;
}


void CProfile::CrossProduct(CCoord A, CCoord B, CCoord *pP)
{

	pP->Side0.fI = (A.Side0.fJ*B.Side0.fK) - (A.Side0.fK*B.Side0.fJ);
	pP->Side0.fJ = (A.Side0.fK*B.Side0.fI) - (A.Side0.fI*B.Side0.fK);
	pP->Side0.fK = (A.Side0.fI*B.Side0.fJ) - (A.Side0.fJ*B.Side0.fI);

}


float CProfile::DotProduct(CCoord A, CCoord B)
{
	float fResult = A.Side0.fI*B.Side0.fI + A.Side0.fJ*B.Side0.fJ + A.Side0.fK*B.Side0.fK;

	return fResult;
}

void CProfile::MountAtSurfaceD3DX()
{
	
	float fAngle;
	int nSide;

	CCoord	CpComponent[3];
	CCoord	CpMeasured[3];

	CpComponent[0]=m_CpCadHole[0];
	CpComponent[1]=m_CpCadHole[1];
	CpComponent[2]=m_CpCadHole[2];

	for(int gg=0;gg<3;gg++) {
		switch(m_CpCadHole[gg].nType) {
		case 0:	CpMeasured[gg]=m_CpMeasuredHole[gg];
			break;
		case 1:
			theApp.m_Kinematics.CalculateBallCenterFromHole(m_CpCadHole[gg],&CpComponent[gg]);
			theApp.m_Kinematics.CalculateBallCenterFromSurface(m_CpMeasuredHole[gg],&CpMeasured[gg]);
			break;
		}
	}
	D3DXMATRIXA16 matWorldSrc,matWorldDst,matTran,matDstTranslation,matRot;
	D3DXMATRIXA16 matWorld;

	D3DXVECTOR3 vecSrc0(CpComponent[0].Side0.pt);
	D3DXVECTOR3 vecSrc1(CpComponent[1].Side0.pt);
	D3DXVECTOR3 vecSrc2(CpComponent[2].Side0.pt);

	D3DXVECTOR3 vecDst0(CpMeasured[0].Side0.pt);
	D3DXVECTOR3 vecDst1(CpMeasured[1].Side0.pt);
	D3DXVECTOR3 vecDst2(CpMeasured[2].Side0.pt);

	/////////////////////////////  Source plane move  /////////////////////////////////

	D3DXMatrixTranslation(&matTran,-vecSrc0.x,-vecSrc0.y,-vecSrc0.z);
	matWorldSrc = matTran;
	D3DXVec3TransformCoord(&vecSrc0,&vecSrc0,&matTran);
	D3DXVec3TransformCoord(&vecSrc1,&vecSrc1,&matTran);
	D3DXVec3TransformCoord(&vecSrc2,&vecSrc2,&matTran);

	fAngle = (float)atan2(vecSrc1.z,vecSrc1.x);			//-ve +ve
	D3DXMatrixRotationY(&matRot,fAngle);
	D3DXMatrixMultiply(&matWorldSrc,&matWorldSrc,&matRot);
	D3DXVec3TransformCoord(&vecSrc0,&vecSrc0,&matRot);
	D3DXVec3TransformCoord(&vecSrc1,&vecSrc1,&matRot);
	D3DXVec3TransformCoord(&vecSrc2,&vecSrc2,&matRot);

	fAngle = (float)atan2(vecSrc1.y,vecSrc1.x) * -1.0f;	//-ve +ve
	D3DXMatrixRotationZ(&matRot,fAngle);
	D3DXMatrixMultiply(&matWorldSrc,&matWorldSrc,&matRot);
	D3DXVec3TransformCoord(&vecSrc0,&vecSrc0,&matRot);
	D3DXVec3TransformCoord(&vecSrc1,&vecSrc1,&matRot);
	D3DXVec3TransformCoord(&vecSrc2,&vecSrc2,&matRot);

	fAngle = (float)atan2(vecSrc2.z,vecSrc2.y) * -1.0f;  //-ve +ve
	D3DXMatrixRotationX(&matRot,fAngle);
	D3DXMatrixMultiply(&matWorldSrc,&matWorldSrc,&matRot);
	D3DXVec3TransformCoord(&vecSrc0,&vecSrc0,&matRot);
	D3DXVec3TransformCoord(&vecSrc1,&vecSrc1,&matRot);
	D3DXVec3TransformCoord(&vecSrc2,&vecSrc2,&matRot);


/////////////////////////////  Destination plane move  /////////////////////////////////

	D3DXMatrixTranslation(&matTran,-vecDst0.x,-vecDst0.y,-vecDst0.z);
	matWorldDst = matTran;
	D3DXVec3TransformCoord(&vecDst0,&vecDst0,&matTran);
	D3DXVec3TransformCoord(&vecDst1,&vecDst1,&matTran);
	D3DXVec3TransformCoord(&vecDst2,&vecDst2,&matTran);

	fAngle = (float)atan2(vecDst1.z,vecDst1.x);		//-ve -ve
	D3DXMatrixRotationY(&matRot,fAngle);
	D3DXMatrixMultiply(&matWorldDst,&matWorldDst,&matRot);
	D3DXVec3TransformCoord(&vecDst0,&vecDst0,&matRot);
	D3DXVec3TransformCoord(&vecDst1,&vecDst1,&matRot);
	D3DXVec3TransformCoord(&vecDst2,&vecDst2,&matRot);

	fAngle = (float)atan2(vecDst1.y,vecDst1.x) * -1.0f;  //-ve +ve
	D3DXMatrixRotationZ(&matRot,fAngle);
	D3DXMatrixMultiply(&matWorldDst,&matWorldDst,&matRot);
	D3DXVec3TransformCoord(&vecDst0,&vecDst0,&matRot);
	D3DXVec3TransformCoord(&vecDst1,&vecDst1,&matRot);
	D3DXVec3TransformCoord(&vecDst2,&vecDst2,&matRot);

	fAngle = (float)atan2(vecDst2.z,vecDst2.y) * -1.0f;	//+ve -ve
	D3DXMatrixRotationX(&matRot,fAngle);
	D3DXMatrixMultiply(&matWorldDst,&matWorldDst,&matRot);
	D3DXVec3TransformCoord(&vecDst0,&vecDst0,&matRot);
	D3DXVec3TransformCoord(&vecDst1,&vecDst1,&matRot);
	D3DXVec3TransformCoord(&vecDst2,&vecDst2,&matRot);

	bool bTest = false;
	if(bTest == true) {
		// This will leave the two planes at z == 0 vector 1 along the positive X axis
		m_CpCadHole[0].Side0.fX = vecSrc0.x;
		m_CpCadHole[0].Side0.fY = vecSrc0.y;
		m_CpCadHole[0].Side0.fZ = vecSrc0.z;

		m_CpCadHole[1].Side0.fX = vecSrc1.x;
		m_CpCadHole[1].Side0.fY = vecSrc1.y;
		m_CpCadHole[1].Side0.fZ = vecSrc1.z;

		m_CpCadHole[2].Side0.fX = vecSrc2.x;
		m_CpCadHole[2].Side0.fY = vecSrc2.y;
		m_CpCadHole[2].Side0.fZ = vecSrc2.z;

		m_CpMeasuredHole[0].Side0.fX = vecDst0.x;
		m_CpMeasuredHole[0].Side0.fY = vecDst0.y;
		m_CpMeasuredHole[0].Side0.fZ = vecDst0.z;

		m_CpMeasuredHole[1].Side0.fX = vecDst1.x;
		m_CpMeasuredHole[1].Side0.fY = vecDst1.y;
		m_CpMeasuredHole[1].Side0.fZ = vecDst1.z;

		m_CpMeasuredHole[2].Side0.fX = vecDst2.x;
		m_CpMeasuredHole[2].Side0.fY = vecDst2.y;
		m_CpMeasuredHole[2].Side0.fZ = vecDst2.z;
		return;
	}


//////////////////////////////  complete matrix    //////////////////////////////////
	matWorld = matWorldSrc;
	D3DXMatrixInverse(&matWorldDst, NULL, &matWorldDst);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matWorldDst);

	int nLine,ii;
	CCoord Cp,CpHead;
	theApp.m_Motors.GetHeadPos(&CpHead);

	for(nLine=0;nLine<m_nTaughtLineL;nLine++) {
		for(ii=0;ii<m_TaughtLine[nLine].m_nCoordL;ii++) {
				m_TaughtLine[nLine].m_pCp[ii].TransformCoord(matWorld);
				theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_TaughtLine[nLine].m_pCp[ii],PORTSIDE);
				theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_TaughtLine[nLine].m_pCp[ii],STARBOARD);
		}
	}

	for(nLine=0;nLine<m_nScanLineL;nLine++) {
		for(ii=0;ii<m_ScanLine[nLine].m_nCoordL;ii++) {
				m_ScanLine[nLine].m_pCp[ii].TransformCoord(matWorld);
				theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_ScanLine[nLine].m_pCp[ii],PORTSIDE);
				theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_ScanLine[nLine].m_pCp[ii],STARBOARD);
		}
	}

	for(nLine=0;nLine<m_nBrainLineL;nLine++) {
		for(ii=0;ii<m_BrainLine[nLine].m_nCoordL;ii++) {
				m_BrainLine[nLine].m_pCp[ii].TransformCoord(matWorld);
				theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_BrainLine[nLine].m_pCp[ii],PORTSIDE);
				theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_BrainLine[nLine].m_pCp[ii],STARBOARD);
		}
	}

	for(nLine=0;nLine<m_nEnvelopeLineL;nLine++) {
		for(ii=0;ii<m_EnvelopeLine[nLine].m_nCoordL;ii++) {
				m_EnvelopeLine[nLine].m_pCp[ii].TransformCoord(matWorld);
				theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_EnvelopeLine[nLine].m_pCp[ii],PORTSIDE);
				theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_EnvelopeLine[nLine].m_pCp[ii],STARBOARD);
		}
	}


	for(ii=0;ii<3;ii++) {
		m_CpCadHole[ii].TransformCoord(matWorld);
		theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_CpCadHole[ii],PORTSIDE);
		theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_CpCadHole[ii],STARBOARD);
		for(int gg=0;gg<2;gg++) {
			m_CpExclusion[ii][gg].TransformCoord(matWorld);
			theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_CpExclusion[ii][gg],PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_CpExclusion[ii][gg],STARBOARD);
		}
	}

	for(nSide=0;nSide<2;nSide++) {
		for(nLine=0;nLine<m_nLineBladeL[nSide];nLine++) {
			for(ii=0;ii<m_LineBlade[nSide][nLine].m_nCoordL;ii++) {
				m_LineBlade[nSide][nLine].m_pCp[ii].TransformCoord(matWorld);
				theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_LineBlade[nSide][nLine].m_pCp[ii],PORTSIDE);
				theApp.m_Kinematics.TipsFromNormals((CCoord*)&m_LineBlade[nSide][nLine].m_pCp[ii],STARBOARD);
			}
		}
	}

	m_matCadToScan = matWorld;

}

void CProfile::MountAtSurface()
{
	float	fAngleHole0,fAngleHole1,fAngleHole2;
	float	fAngleSurface0,fAngleSurface1,fAngleSurface2;
	VectorStruct	a;

	CCoord	CpComponent[3];
	CCoord	CpMeasured[3];
	CCoord	CpBallCentre[3];

	CpComponent[0]=m_CpCadHole[0];
	CpComponent[1]=m_CpCadHole[1];
	CpComponent[2]=m_CpCadHole[2];


	if((theApp.m_LastSettings.fBallHeight==0.0) && (theApp.m_LastSettings.fBallDiameter==0.0)) {
		m_CpCadHole[0].nType=0;
		m_CpCadHole[1].nType=0;
		m_CpCadHole[2].nType=0;
	}


	for(int gg=0;gg<3;gg++) {
		switch(m_CpCadHole[gg].nType) {
		case 0:	CpMeasured[gg]=m_CpMeasuredHole[gg];
			break;
		case 1:
			theApp.m_Kinematics.CalculateBallCenterFromHole(m_CpCadHole[gg],&CpComponent[gg]);
			theApp.m_Kinematics.CalculateBallCenterFromSurface(m_CpMeasuredHole[gg],&CpMeasured[gg]);

			break;
		}
	}

	//check vector lengths for no other reason but its fun
	a.fI = CpComponent[1].Side0.fX-CpComponent[0].Side0.fX;
	a.fJ = CpComponent[1].Side0.fY-CpComponent[0].Side0.fY;
	a.fK = CpComponent[1].Side0.fZ-CpComponent[0].Side0.fZ;
	float fVLengthComponentA = VectorLength(a);
	a.fI = CpComponent[2].Side0.fX-CpComponent[0].Side0.fX;
	a.fJ = CpComponent[2].Side0.fY-CpComponent[0].Side0.fY;
	a.fK = CpComponent[2].Side0.fZ-CpComponent[0].Side0.fZ;
	float fVLengthComponentB = VectorLength(a);
	a.fI = CpMeasured[1].Side0.fX-CpMeasured[0].Side0.fX;
	a.fJ = CpMeasured[1].Side0.fY-CpMeasured[0].Side0.fY;
	a.fK = CpMeasured[1].Side0.fZ-CpMeasured[0].Side0.fZ;
	float fVLengthMeasuredA = VectorLength(a);
	a.fI = CpMeasured[2].Side0.fX-CpMeasured[0].Side0.fX;
	a.fJ = CpMeasured[2].Side0.fY-CpMeasured[0].Side0.fY;
	a.fK = CpMeasured[2].Side0.fZ-CpMeasured[0].Side0.fZ;
	float fVLengthMeasuredB = VectorLength(a);



//????????
	AcceptTransform();		//This gets the current orientation pre processed

	RotatePlaneToZ0(CpComponent[0],CpComponent[1],CpComponent[2],&fAngleHole0,&fAngleHole1,&fAngleHole2);
	RotatePlaneToZ0(CpMeasured[0],CpMeasured[1],CpMeasured[2],&fAngleSurface0,&fAngleSurface1,&fAngleSurface2);


	//translate to x = 0 y = 0 z = 0
	m_fTranslateX = -CpComponent[0].Side0.fX;
	m_fTranslateY = -CpComponent[0].Side0.fY;
	m_fTranslateZ = -CpComponent[0].Side0.fZ;
//	m_dTranslateX = -CpMeasured[0].dXp[0];
//	m_dTranslateY = -CpMeasured[0].dYp[0];
//	m_dTranslateZ = -CpMeasured[0].dZp[0];
	m_fRotateX = 0.0;
	m_fRotateY = 0.0;
	m_fRotateZ = 0.0;
	AcceptTransform();

	//Rotate About X Axis to get one vector at z = 0
	m_fTranslateX = 0.0;
	m_fTranslateY = 0.0;
	m_fTranslateZ = 0.0;
	m_fRotateX = fAngleHole0;//fAngleHole0;
	m_fRotateY = 0.0;
	m_fRotateZ = 0.0;
	AcceptTransform();

	//Rotate about z axis to get first vector to be along the zero axis
	m_fRotateX = 0.0;
	m_fRotateY = fAngleHole1;//fAngleHole1;
	m_fRotateZ = 0.0;
	AcceptTransform();


	//rotate about x axis to get other vector z = 0
	m_fRotateX = fAngleHole2;//fAngleHole2;
	m_fRotateY = 0.0;
	m_fRotateZ = 0.0;
	AcceptTransform();


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Rotate the Z plane on to surface
	m_fRotateX = fAngleSurface2 * -1.0f;
	m_fRotateY = 0.0f;
	m_fRotateZ = 0.0f;
	AcceptTransform();

	m_fRotateX = 0.0;
	m_fRotateY = fAngleSurface1 * -1.0f;
	m_fRotateZ = 0.0f;
	AcceptTransform();

	m_fRotateX = fAngleSurface0 * -1.0f;
	m_fRotateY = 0.0f;
	m_fRotateZ = 0.0f;
	AcceptTransform();


	//Translate it
	m_fTranslateX = CpMeasured[0].Side0.fX;
	m_fTranslateY = CpMeasured[0].Side0.fY;
	m_fTranslateZ = CpMeasured[0].Side0.fZ;
	m_fRotateX = 0.0;
	m_fRotateY = 0.0;
	m_fRotateZ = 0.0;
	AcceptTransform();



}


void CProfile::RotatePlaneToZ0(CCoord O,CCoord A, CCoord B, float *fAngle0, float *fAngle1, float *fAngle2)
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


void CProfile::GenerateMesh()
{
	switch(m_nBraneStyle & 1) {
	case 0: GenerateHorizontalMesh();
		break;
	case 1: GenerateVerticalMesh();
		break;
	}

}

void CProfile::GenerateHorizontalMesh()
{
	float fMeshBottom = theApp.m_LastSettings.fMeshBottomMargin;
	float fMeshTop = theApp.m_LastSettings.fMeshTopMargin;
	float fMeshLeft = theApp.m_LastSettings.fMeshLeftMargin;
	float fMeshRight = theApp.m_LastSettings.fMeshRightMargin;

	if((theApp.m_LastSettings.nOrientation3DEditImage  & 2) == 2) {
		fMeshLeft = theApp.m_LastSettings.fMeshRightMargin;
		fMeshRight = theApp.m_LastSettings.fMeshLeftMargin;
	}
	if((theApp.m_LastSettings.nOrientation3DEditImage  & 1) == 1) {
		fMeshBottom = theApp.m_LastSettings.fMeshTopMargin;
		fMeshTop = theApp.m_LastSettings.fMeshBottomMargin;
	}

	if(m_nScanLineL<=0) {
		MessageBox(NULL,_T("There are no scan lines to generate a mesh"),_T("Error"),MB_ICONERROR);
		return;
	}
	if(m_fSlowIncrement<=0.0f) {
		MessageBox(NULL,_T("The slow increment is incorrectly set"),_T("Error"),MB_ICONERROR);
		return;
	}


	SAFE_DELETE_ARRAY( m_BrainLine );
	m_nBrainLineL = 0;

	int	nLineIncrement = (int)(theApp.m_LastSettings.fMeshSlowSize  / m_fSlowIncrement);
	int nLine,nStartLine,nFinishLine,nRow,nColumn;
	CPolyCoord Line;
	CCoord Cp;
	float fPos,fStart,fFinish;

	SortEachLine(SCANLINES,m_nFastAxis);

	if(theApp.m_LastSettings.nBrainMeshMode == 0) {
		nStartLine=(int)((float)fabs((double)fMeshBottom) / m_fSlowIncrement);
		nFinishLine = m_nScanLineL - 1 - (int)((float)fabs((double)fMeshTop) / m_fSlowIncrement);

		for(nLine=nStartLine;nLine<nFinishLine;nLine+=nLineIncrement) {
			fStart=m_ScanLine[nLine].m_pCp[0].fPos[m_nFastAxis] + (float)fabs((double)fMeshLeft);
			fFinish = m_ScanLine[nLine].m_pCp[m_ScanLine[nLine].m_nCoordL-1].fPos[m_nFastAxis] - (float)fabs((double)fMeshRight);

			Line.Zero();
			for(fPos=fStart;fPos<fFinish;fPos += theApp.m_LastSettings.fMeshFastSize) {
				m_ScanLine[nLine].m_nPrimaryAxis = m_nFastAxis;
				m_ScanLine[nLine].CalculateCoord(fPos,Cp);
				Cp.Zero(2);
				Line.Add(Cp);
			}
			m_ScanLine[nLine].m_nPrimaryAxis = m_nFastAxis;
			m_ScanLine[nLine].CalculateCoord(fFinish,Cp);
			Cp.Zero(2);
			Line.Add(Cp);
			AddLine(BRAINLINES, &Line ,m_nSlowAxis);
		}
		nLine = nFinishLine;

		fStart=m_ScanLine[nLine].m_pCp[0].fPos[m_nFastAxis] + (float)fabs((double)fMeshLeft);
		fFinish = m_ScanLine[nLine].m_pCp[m_ScanLine[nLine].m_nCoordL-1].fPos[m_nFastAxis] - (float)fabs((double)fMeshRight);
		Line.Zero();
		for(fPos=fStart;fPos<fFinish;fPos += theApp.m_LastSettings.fMeshFastSize) {
			m_ScanLine[nLine].m_nPrimaryAxis = m_nFastAxis;
			m_ScanLine[nLine].CalculateCoord(fPos,Cp);
			Cp.Zero(2);
			Line.Add(Cp);
		}
		m_ScanLine[nLine].m_nPrimaryAxis = m_nFastAxis;
		m_ScanLine[nLine].CalculateCoord(fFinish,Cp);
		Cp.Zero(2);
		Line.Add(Cp);
		AddLine(BRAINLINES, &Line ,m_nSlowAxis);
	} else {
		nStartLine=(int)((float)fabs((double)fMeshBottom) / m_fSlowIncrement);
		nFinishLine = m_nScanLineL - 1 - (int)((float)fabs((double)fMeshTop) / m_fSlowIncrement);

		for(nRow=0;nRow<theApp.m_LastSettings.fMeshSlowSize;nRow++) {
			nLine = MulDiv(nRow,nFinishLine-nStartLine,(int)(theApp.m_LastSettings.fMeshSlowSize-1.0f))+nStartLine;
			fStart=m_ScanLine[nLine].m_pCp[0].fPos[m_nFastAxis] + (float)fabs((double)fMeshLeft); // m_nFastAxis from 0
			fFinish = m_ScanLine[nLine].m_pCp[m_ScanLine[nLine].m_nCoordL-1].fPos[m_nFastAxis] - (float)fabs((double)fMeshRight);
					
			if(fStart>fFinish)
			continue;

			Line.Zero();
			for(nColumn=0;nColumn<theApp.m_LastSettings.fMeshFastSize;nColumn++) {
				fPos = ((float)nColumn * (fFinish - fStart) / (float)(theApp.m_LastSettings.fMeshFastSize-1)) + fStart;
				m_ScanLine[nLine].m_nPrimaryAxis = m_nFastAxis;
				m_ScanLine[nLine].CalculateCoord(fPos,Cp);
				Cp.Zero(2);
				Line.Add(Cp);
			}
			AddLine(BRAINLINES, &Line ,m_nSlowAxis);
		}
	}


	m_bModified = TRUE;

}


void CProfile::GenerateVerticalMesh()
{
	float fMeshBottom = theApp.m_LastSettings.fMeshBottomMargin;
	float fMeshTop = theApp.m_LastSettings.fMeshTopMargin;
	float fMeshLeft = theApp.m_LastSettings.fMeshLeftMargin;
	float fMeshRight = theApp.m_LastSettings.fMeshRightMargin;

	if((theApp.m_LastSettings.nOrientation3DEditImage  & 2) == 2) {
		fMeshLeft = theApp.m_LastSettings.fMeshRightMargin;
		fMeshRight = theApp.m_LastSettings.fMeshLeftMargin;
	}
	if((theApp.m_LastSettings.nOrientation3DEditImage  & 1) == 1) {
		fMeshBottom = theApp.m_LastSettings.fMeshTopMargin;
		fMeshTop = theApp.m_LastSettings.fMeshBottomMargin;
	}

	if(m_nScanLineL<=0) {
		MessageBox(NULL,_T("There are no scan lines to generate a mesh"),_T("Error"),MB_ICONERROR);
		return;
	}
	if(m_fSlowIncrement<=0.0f) {
		MessageBox(NULL,_T("The slow increment is incorrectly set"),_T("Error"),MB_ICONERROR);
		return;
	}


	SAFE_DELETE_ARRAY( m_BrainLine );
	m_nBrainLineL = 0;

	int	nLineIncrement = (int)(theApp.m_LastSettings.fMeshSlowSize  / m_fSlowIncrement);
	int nLine,nStartLine,nFinishLine,nRow,nColumn;
	CPolyCoord Line;
	CCoord Cp;
	float fPos,fStart,fFinish;

	SortEachLine(SCANLINES,m_nFastAxis);
	nLine = 0;

	if(theApp.m_LastSettings.nBrainMeshMode == 0) {

		nStartLine=(int)((float)fabs((double)fMeshBottom) / m_fSlowIncrement);
		nFinishLine = m_nScanLineL - 1 - (int)((float)fabs((double)fMeshTop) / m_fSlowIncrement);

		fStart=m_ScanLine[nLine].m_pCp[0].fPos[m_nFastAxis] + (float)fabs((double)fMeshLeft);
		fFinish = m_ScanLine[nLine].m_pCp[m_ScanLine[nLine].m_nCoordL-1].fPos[m_nFastAxis] - (float)fabs((double)fMeshRight);

		for(fPos=fStart;fPos<fFinish;fPos += theApp.m_LastSettings.fMeshFastSize) {

			Line.Zero();


			nLine = (int)(fMeshBottom / m_fSlowIncrement);
			for(nLine=nStartLine;nLine<nFinishLine;nLine+=nLineIncrement) {
				m_ScanLine[nLine].m_nPrimaryAxis = m_nFastAxis;
				m_ScanLine[nLine].CalculateCoord(fPos,Cp);
				Cp.Zero(2);
				Line.Add(Cp);
			}
//			m_ScanLine[nLine].m_nPrimaryAxis = m_nFastAxis;
//			m_ScanLine[nLine].CalculateCoord(fFinish,Cp);
//			Cp.Zero(2);
//			Line.Add(Cp);
			AddLine(BRAINLINES, &Line ,m_nSlowAxis);
		}
/*
		nLine = nFinishLine;

		fStart=m_ScanLine[nLine].m_pCp[0].fPos[m_nFastAxis] + (float)fabs((double)fMeshLeft);
		fFinish = m_ScanLine[nLine].m_pCp[m_ScanLine[nLine].m_nCoordL-1].fPos[m_nFastAxis] - (float)fabs((double)fMeshRight);
		Line.Zero();
		for(fPos=fStart;fPos<fFinish;fPos += theApp.m_LastSettings.fMeshFastSize) {
			m_ScanLine[nLine].m_nPrimaryAxis = m_nFastAxis;
			m_ScanLine[nLine].CalculateCoord(fPos,Cp);
			Cp.Zero(2);
			Line.Add(Cp);
		}
		m_ScanLine[nLine].m_nPrimaryAxis = m_nFastAxis;
		m_ScanLine[nLine].CalculateCoord(fFinish,Cp);
		Cp.Zero(2);
		Line.Add(Cp);
		AddLine(BRAINLINES, &Line ,m_nSlowAxis);
		*/
	} else {
		nStartLine=(int)((float)fabs((double)fMeshBottom) / m_fSlowIncrement);
		nFinishLine = m_nScanLineL - 1 - (int)((float)fabs((double)fMeshTop) / m_fSlowIncrement);

		for(nRow=0;nRow<theApp.m_LastSettings.fMeshSlowSize;nRow++) {
			nLine = MulDiv(nRow,nFinishLine-nStartLine,(int)(theApp.m_LastSettings.fMeshSlowSize-1.0f))+nStartLine;
			fStart=m_ScanLine[nLine].m_pCp[0].fPos[m_nFastAxis] + (float)fabs((double)fMeshLeft); // m_nFastAxis from 0
			fFinish = m_ScanLine[nLine].m_pCp[m_ScanLine[nLine].m_nCoordL-1].fPos[m_nFastAxis] - (float)fabs((double)fMeshRight);
					
			if(fStart>fFinish)
			continue;

			Line.Zero();
			for(nColumn=0;nColumn<theApp.m_LastSettings.fMeshFastSize;nColumn++) {
				fPos = ((float)nColumn * (fFinish - fStart) / (float)(theApp.m_LastSettings.fMeshFastSize-1)) + fStart;
				m_ScanLine[nLine].m_nPrimaryAxis = m_nFastAxis;
				m_ScanLine[nLine].CalculateCoord(fPos,Cp);
				Cp.Zero(2);
				Line.Add(Cp);
			}
			AddLine(BRAINLINES, &Line ,m_nSlowAxis);
		}
	}


	m_bModified = TRUE;

}

bool CProfile::CalculateBrainCoord(int nFastAxis, int nSlowAxis, CCoord &pCp)
{
	int	nLine;
	CPolyCoord Line;
	CCoord Cp;

	if(m_nBrainLineL <= 0) return false;
	if(m_BrainLine == NULL) return false;

	for(nLine=0;nLine<m_nBrainLineL;nLine++) {
		if(m_BrainLine[nLine].m_nCoordL>1){
			m_BrainLine[nLine].m_nStyle = 0;
			m_BrainLine[nLine].m_bModified = TRUE;
			m_BrainLine[nLine].SetPrimaryAxis(nFastAxis);
			m_BrainLine[nLine].CalculateCoord(pCp.fPos[nFastAxis],Cp);
			Line.Add(Cp);
			m_BrainLine[nLine].m_nStyle = 0;
		}else{
			Line.Add(m_BrainLine[nLine].m_pCp[0]);
		}

	}

	Line.m_nStyle=0;
	Line.SetPrimaryAxis(nSlowAxis);
	Line.CalculateCoord(pCp.fPos[nSlowAxis],Cp);

	pCp.Side0.fX += Cp.Delta.fX0;
	pCp.Side0.fY += Cp.Delta.fY0;
	pCp.Side0.fZ += Cp.Delta.fZ0;
	pCp.Side1.fX += Cp.Delta.fX1;
	pCp.Side1.fY += Cp.Delta.fY1;
	pCp.Side1.fZ += Cp.Delta.fZ1;
	pCp.Side0.fI += Cp.Delta.fI;
	pCp.Side0.fJ += Cp.Delta.fJ;
	pCp.Side0.fK += Cp.Delta.fK;
	pCp.Side1.fI += Cp.Delta.fI;
	pCp.Side1.fJ += Cp.Delta.fJ;
	pCp.Side1.fK += Cp.Delta.fK;

	pCp.LimitIJK();

	return FALSE;

}

void CProfile::ModifyBrainDeltas(int nLine, int nIndex, bool bSingleSided)
{
	
	CCoord CpSurface,CpHead;

	if(nLine>=m_nBrainLineL) return;
	if(nIndex<0) return;
	if(nIndex>=m_BrainLine[nLine].m_nCoordL) return;

	theApp.m_Motors.GetSurfacePos(&CpSurface);

	m_BrainLine[nLine].m_pCp[nIndex].Delta.fX0 = CpSurface.Side0.fX - m_BrainLine[nLine].m_pCp[nIndex].Side0.fX;
	m_BrainLine[nLine].m_pCp[nIndex].Delta.fY0 = CpSurface.Side0.fY - m_BrainLine[nLine].m_pCp[nIndex].Side0.fY;
	m_BrainLine[nLine].m_pCp[nIndex].Delta.fZ0 = CpSurface.Side0.fZ - m_BrainLine[nLine].m_pCp[nIndex].Side0.fZ;
	m_BrainLine[nLine].m_pCp[nIndex].Delta.fI = CpSurface.Side0.fI - m_BrainLine[nLine].m_pCp[nIndex].Side0.fI;
	m_BrainLine[nLine].m_pCp[nIndex].Delta.fJ = CpSurface.Side0.fJ - m_BrainLine[nLine].m_pCp[nIndex].Side0.fJ;
	m_BrainLine[nLine].m_pCp[nIndex].Delta.fK = CpSurface.Side0.fK - m_BrainLine[nLine].m_pCp[nIndex].Side0.fK;

	if(bSingleSided == false) {
		m_BrainLine[nLine].m_pCp[nIndex].Delta.fX1 = CpSurface.Side1.fX - m_BrainLine[nLine].m_pCp[nIndex].Side1.fX;
		m_BrainLine[nLine].m_pCp[nIndex].Delta.fY1 = CpSurface.Side1.fY - m_BrainLine[nLine].m_pCp[nIndex].Side1.fY;
		m_BrainLine[nLine].m_pCp[nIndex].Delta.fZ1 = CpSurface.Side1.fZ - m_BrainLine[nLine].m_pCp[nIndex].Side1.fZ;
	} else {
		m_BrainLine[nLine].m_pCp[nIndex].Delta.fX1 = 0.0f;
		m_BrainLine[nLine].m_pCp[nIndex].Delta.fY1 = 0.0f;
		m_BrainLine[nLine].m_pCp[nIndex].Delta.fZ1 = 0.0f;
	}
	m_bModified = m_BrainLine[nLine].m_bModified = TRUE;

}

void CProfile::ModifyTaughtCoordinate(int nLine, int nIndex)
{
	CCoord CpSurface,CpHead;

	if(nLine>=m_nTaughtLineL) return;
	if(nIndex<0) return;
	if(nIndex>=m_TaughtLine[nLine].m_nCoordL) return;

	theApp.m_Motors.GetHeadPos(&CpHead);
	switch(theApp.m_LastSettings.nPositionDisplayMode) {
	default:
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,TRUE, CURRENT_FL);
		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,TRUE, CURRENT_FL);
		break;
//	case 3:
//		theApp.m_Kinematics.CheckValidWaterPath(PORTSIDE,theApp.m_LastSettings.nWaterPathTS[PORTSIDE]);
//		theApp.m_Kinematics.CheckValidWaterPath(STARBOARD,theApp.m_LastSettings.nWaterPathTS[STARBOARD]);
//		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,PORTSIDE,TRUE,FALSE);
//		theApp.m_Kinematics.SurfaceFromHead(&CpHead,&CpSurface,STARBOARD,TRUE,FALSE);
//		break;
	case 999:	//Shut compiler up
		break;
	}
	theApp.m_Kinematics.NormalsFromTips(&CpSurface,PORTSIDE);
	theApp.m_Kinematics.NormalsFromTips(&CpSurface,STARBOARD);

	m_TaughtLine[nLine].m_pCp[nIndex] = CpSurface;

	m_bModified = m_TaughtLine[nLine].m_bModified = TRUE;

}

bool CProfile::CheckNewProfileAllowed()
{

	if(m_bModified == TRUE) {
		if(MessageBox(NULL,_T("There have been changes to the current profile that have not been saved.\nDo you wish to proceen without saving modifications?"),_T("Warning"),MB_YESNO) != IDYES) return FALSE;
	}
	return TRUE;

}




void CProfile::DeleteCoordInArea(int nLineType, CCoord *pCp0, CCoord *pCp1,int nIgnoreAxis)
{
	CPolyCoord* pLine = NULL;
	int nLineL;
	int	nLine;

	switch(nLineType) {
	case TAUGHTLINES: pLine = m_TaughtLine;
		nLineL = m_nTaughtLineL;
		break;
	case SCANLINES: pLine = m_ScanLine;
		nLineL = m_nScanLineL;
		break;
	case BRAINLINES: pLine = m_BrainLine;
		nLineL = m_nBrainLineL;
		break;
	case ENVELOPELINES: pLine = m_EnvelopeLine;
		nLineL = m_nEnvelopeLineL;
		break;
	}
	if(pLine==NULL) return;
	if(nLineL<=0) return;

	for(nLine=0;nLine<nLineL;nLine++) {
		pLine[nLine].DeleteCoordInArea(pCp0,pCp1,nIgnoreAxis);
	}

	for(nLine=nLine-1;nLine>=0;nLine--) {
		if(pLine[nLine].m_nCoordL<=0) {
			DeleteLine(nLineType,nLine);
			switch (nLineType) {
			case TAUGHTLINES: pLine = m_TaughtLine;
				nLineL = m_nTaughtLineL;
				break;
			case SCANLINES: pLine = m_ScanLine;
				nLineL = m_nScanLineL;
				break;
			case BRAINLINES: pLine = m_BrainLine;
				nLineL = m_nBrainLineL;
				break;
			case ENVELOPELINES: pLine = m_EnvelopeLine;
				nLineL = m_nEnvelopeLineL;
				break;
			}
			if (pLine == NULL) return;
			if (nLineL <= 0) return;
		}
	}
}

void CProfile::DeleteAllLines(int nLineType)
{

	switch(nLineType) {
	case TAUGHTLINES:
		SAFE_DELETE_ARRAY( m_TaughtLine );
		m_nTaughtLineL=0;
		break;
	case SCANLINES:
		SAFE_DELETE_ARRAY( m_ScanLine );
		m_nScanLineL=0;
		break;
	case BRAINLINES:
		SAFE_DELETE_ARRAY( m_BrainLine );
		m_nBrainLineL=0;
		break;
	case ENVELOPELINES:
		SAFE_DELETE_ARRAY( m_EnvelopeLine );
		m_nEnvelopeLineL=0;
		break;
	case SURFACELINES:
		SAFE_DELETE_ARRAY( m_SurfaceLine );
		m_nSurfaceLineL=0;
		break;
	case OUTSIDELINES:
		SAFE_DELETE_ARRAY( m_LineBlade[OUTSIDE] );
		m_nLineBladeL[OUTSIDE]=0;
		break;
	case INSIDELINES:
		SAFE_DELETE_ARRAY( m_LineBlade[INSIDE] );
		m_nLineBladeL[INSIDE]=0;
		break;
	case BRANEOUTSIDELINES:
		SAFE_DELETE_ARRAY( m_LineBlade[OUTSIDE + 2] );
		m_nLineBladeL[OUTSIDE + 2]=0;
		break;
	case BRANEINSIDELINES:
		SAFE_DELETE_ARRAY( m_LineBlade[INSIDE + 2] );
		m_nLineBladeL[INSIDE + 2]=0;
		break;
	}

}

bool CProfile::CalculateEnvelopeFromRect()
{
	
	COORDDATA CpMin,CpMax,Cp;
	float fTemp;
	int nAxis;

	delete [] m_EnvelopeLine;
	m_EnvelopeLine = NULL;
	m_nEnvelopeLineL = 0;

	m_EnvelopeLine = new CPolyCoord[m_nEnvelopeLineL=1];

	CpMin=m_CpRect[0];
	CpMax=m_CpRect[1];
	for(nAxis=0;nAxis<3;nAxis++) {
		if(CpMin.Side[theApp.m_nPrimarySide].fPos[nAxis] > CpMax.Side[theApp.m_nPrimarySide].fPos[nAxis]) {
			fTemp = CpMin.Side[theApp.m_nPrimarySide].fPos[nAxis];
			CpMin.Side[theApp.m_nPrimarySide].fPos[nAxis] = CpMax.Side[theApp.m_nPrimarySide].fPos[nAxis];
			CpMax.Side[theApp.m_nPrimarySide].fPos[nAxis] = fTemp;
		}
	}

	Cp=CpMin;
	Cp.Side[theApp.m_nSecondarySide] = Cp.Side[theApp.m_nPrimarySide];
	m_EnvelopeLine[0] + (Cp);

	Cp.Side[theApp.m_nPrimarySide].fX = CpMax.Side[theApp.m_nPrimarySide].fX;
	Cp.Side[theApp.m_nSecondarySide] = Cp.Side[theApp.m_nPrimarySide];
	m_EnvelopeLine[0] + (Cp);

	Cp.Side[theApp.m_nPrimarySide].fZ = CpMax.Side[theApp.m_nPrimarySide].fZ;
	Cp.Side[theApp.m_nSecondarySide] = Cp.Side[theApp.m_nPrimarySide];
	m_EnvelopeLine[0] + (Cp);

	Cp.Side[theApp.m_nPrimarySide].fX = CpMin.Side[theApp.m_nPrimarySide].fX;
	Cp.Side[theApp.m_nSecondarySide] = Cp.Side[theApp.m_nPrimarySide];
	m_EnvelopeLine[0] + (Cp);


	m_bModified = TRUE;
	if(((CpMax.Side[theApp.m_nPrimarySide].fX - CpMin.Side[theApp.m_nPrimarySide].fX) > 10.0) && ((CpMax.Side[theApp.m_nPrimarySide].fZ - CpMin.Side[theApp.m_nPrimarySide].fZ) > 10.0)) {
		return TRUE;
	}
	return FALSE;
}


void CProfile::CropCoordInArea(int nLineType, CCoord *pCp0, CCoord *pCp1,int nIgnoreAxis)
{
	
	int nLineL;
	int	nLine;
	CCoord	Cp;
	CProgressDlg dlg(NULL);
	int DeleteLinesL=0, ii, jj=0;

	switch(nLineType) {
	case TAUGHTLINES:
		nLineL = m_nTaughtLineL;
		break;
	case SCANLINES:
		nLineL = m_nScanLineL;
		break;
	case BRAINLINES:
		nLineL = m_nBrainLineL;
		break;
	case ENVELOPELINES:
		nLineL = m_nEnvelopeLineL;
		break;
	}
	if(nLineL<=0) return;



	for(nLine=0;nLine<nLineL;nLine++) {
		if(m_ScanLine[nLine].DoesXPlainCutLine(pCp0->Side[theApp.m_nPrimarySide].fX,pCp0,pCp1,Cp) == TRUE) {
			m_ScanLine[nLine].Add(Cp);
			m_ScanLine[nLine].Sort(0);
		}
		if(m_ScanLine[nLine].DoesXPlainCutLine(pCp1->Side[theApp.m_nPrimarySide].fX,pCp0,pCp1,Cp) == TRUE) {
			m_ScanLine[nLine].Add(Cp);
			m_ScanLine[nLine].Sort(0);
		}
	}

	for(nLine=0;nLine<nLineL;nLine++) {
		m_ScanLine[nLine].DeleteCoordInArea(pCp0,pCp1,nIgnoreAxis);
	}


	for(ii=0;ii<nLineL;ii++){
		if(m_ScanLine[ii].m_nCoordL<=0) {
			DeleteLinesL++;
		}
	}
	
	if(DeleteLinesL>0){
		dlg.Create(CProgressDlg::IDD, NULL);
		dlg.SetProgressText(L"Deleting Scan Lines");
		dlg.SetProgressRange(0,DeleteLinesL);
	
		for(nLine=nLine-1;nLine>=0;nLine--) {
			if(m_ScanLine[nLine].m_nCoordL<=0) {
				DeleteLine(nLineType,nLine);
				dlg.SetProgressPos(jj);
				jj++;
			}
		}
	}

}


void CProfile::ReverseScanLineOrder()
{
	int nn;

	CPolyCoord *pLine = new CPolyCoord[m_nScanLineL];
	for(nn=0;nn<m_nScanLineL;nn++) {
		pLine[nn] = &m_ScanLine[nn];
	}
	delete [] m_ScanLine;
	m_ScanLine = NULL;
	if(m_nScanLineL) {
		m_ScanLine = new CPolyCoord[m_nScanLineL];
		for(nn=0;nn<m_nScanLineL;nn++) {
			m_ScanLine[m_nScanLineL-nn-1] = &pLine[nn];
		}
	}
	delete [] pLine;
}



void CProfile::ZeroAllBrainDeltas(int nAction)
{

	if(m_nBrainLineL==0) return;
	if(m_BrainLine==NULL) return;


	for(int nLine=0;nLine<m_nBrainLineL;nLine++) {
		m_BrainLine[nLine].ZeroAllDeltas(nAction);
	}

}


//qsort this will help you find this usefull algorithm in the future
void CProfile::OrderLines(int nWhich, int nOrderAxis)
{
	int	jj,ii;
	CPolyCoord Line;
	float fPosSrc,fPosDest;

	int nLineL=0;
	CPolyCoord* pLine=NULL;

	switch(nWhich) {
	case TAUGHTLINES: pLine = m_TaughtLine;
		nLineL = m_nTaughtLineL;
		break;
	case SCANLINES: pLine = m_ScanLine;
		nLineL = m_nScanLineL;
		break;
	case BRAINLINES: pLine = m_BrainLine;
		nLineL = m_nBrainLineL;
		break;
	case ENVELOPELINES: pLine = m_EnvelopeLine;
		nLineL = m_nEnvelopeLineL;
		break;
	}

	if((pLine==NULL) || (nLineL<=0)) return;
	for(jj=1;jj<nLineL;jj++) {
		fPosSrc = pLine[jj].m_pCp[0].fPos[nOrderAxis];
		Line.Zero();
		Line=&pLine[jj];

		for(ii=jj-1;ii>=0;ii--) {
			fPosDest = pLine[ii].m_pCp[0].fPos[nOrderAxis];
			if(fPosDest < fPosSrc) goto label_10;
			pLine[ii+1].Zero();
			pLine[ii+1] = &pLine[ii];
		}
		ii=-1;
label_10:
		pLine[ii+1].Zero();
		pLine[ii+1] = &Line;
	}
}


//Pre set m_CpRect[0] with the start coordinate
HRESULT CProfile::GenerateRectilinearScanFromSizes(int nSide)
{
	
	HRESULT hr = S_OK;
	CCoord Cp0,Cp1,Cp;
	int nLine;
	int nSlowAxis = m_nSlowAxis;

	nSide &= 1;

	if((m_fFastScanSize<=0.0) || (m_fSlowScanSize<=0.0)) {
		MessageBox(NULL,_T("Scan sizes are incorrect"),_T("Error"),MB_ICONERROR);
		hr = S_FALSE;
		goto LCleanReturn;
	}
	if((m_fFastIncrement<=0.0) || (m_fSlowIncrement<=0.0)) {
		MessageBox(NULL,_T("Scan increments are incorrect"),_T("Error"),MB_ICONERROR);
		hr = S_FALSE;
		goto LCleanReturn;
	}

	
	SAFE_DELETE_ARRAY(m_ScanLine);
	m_nScanLineL = 0;

	if(m_nFastAxis==5) {
//		return GenerateContinousRotateRectilinearScanFromCoordinates(nSide,false);
	}

	m_nScanLineL = (int)(m_fSlowScanSize / m_fSlowIncrement);

	if(m_nScanLineL) {
		m_ScanLine = new CPolyCoord[m_nScanLineL];
	}

	theApp.m_Kinematics.NormalsFromTips(&m_CpRect[0],PORTSIDE);
	theApp.m_Kinematics.NormalsFromTips(&m_CpRect[0],STARBOARD);
	Cp0 = m_CpRect[0];
	Cp1 = Cp0;
	Cp1.Side0.fPos[m_nFastAxis] += m_fFastScanSize;
	Cp1.Side1.fPos[m_nFastAxis] += m_fFastScanSize;


	theApp.m_Motors.GetHeadPos(&Cp);
	if(theApp.m_nUseTurntableOrRollers == 0) {
		m_CpRect[0].Side[1].fR = m_CpRect[0].Side[0].fR = Cp.Side0.fR;
	} else {
		m_CpRect[0].Side[1].fR = m_CpRect[0].Side[0].fR = Cp.Side1.fR;
	};

	for(nLine = 0;nLine<m_nScanLineL;nLine++) {
		Cp0.Side0.fPos[nSlowAxis] = m_fSlowIncrement * (float)nLine + m_CpRect[0].Side0.fPos[nSlowAxis];
		Cp1.Side0.fPos[nSlowAxis] = Cp0.Side0.fPos[nSlowAxis];
		Cp0.Side1.fPos[nSlowAxis] = m_fSlowIncrement * (float)nLine + m_CpRect[0].Side1.fPos[nSlowAxis];
		Cp1.Side1.fPos[nSlowAxis] = Cp0.Side1.fPos[nSlowAxis];
		m_ScanLine[nLine].SetPrimaryAxis(m_nFastAxis);
		m_ScanLine[nLine].Add(Cp0);
		m_ScanLine[nLine].Add(Cp1);
	}

	if(m_nSlowAxis==theApp.m_Tank.nRLeft) {
		theApp.m_Thread.m_nRevCounter = theApp.m_Thread.m_nScanLine = m_nScanStartLine = 0;
		theApp.m_Motors.SetRevCount(theApp.m_Thread.m_nRevCounter);
	}

	m_nWayPointNumber=2;

	CalculateDangerZone();

LCleanReturn:
    return hr;

}

HRESULT CProfile::GenerateContinousRotateRectilinearScanFromCoordinates(int nSide, bool bCoordinates)
{
	//CONTINOUS TURTABLE scan routine works by having 1 scanline with all the points along it, we are going to create that here

	//bCoordinates if not doing from sizes

	
	CCoord Cp0,Cp1;
	int nLine,nAxis,nPoints;
	float	fX,fY,fZ,fR,fTemp;
	int nFastAxis = m_nFastAxis;

	if(bCoordinates) {
		
		fX = fabs(m_CpRect[1].Side[nSide].fX - m_CpRect[0].Side[nSide].fX);
		fY = fabs(m_CpRect[1].Side[nSide].fY - m_CpRect[0].Side[nSide].fY);
		fZ = fabs(m_CpRect[1].Side[nSide].fZ - m_CpRect[0].Side[nSide].fZ);
		fR = fabs(m_CpRect[1].Side[nSide].fR - m_CpRect[0].Side[nSide].fR);


		m_fSlowScanSize = (float)sqrt((double)(fX*fX + fY*fY + fZ*fZ));
		m_fFastScanSize = fabs(m_CpRect[1].Side[nSide].fPos[5] - m_CpRect[0].Side[nSide].fPos[5]); //fR == 5
	}

	m_nScanLineL=1;

	m_ScanLine = new CPolyCoord[m_nScanLineL];

	nPoints = (int)(m_fSlowScanSize / m_fSlowIncrement);


	if(bCoordinates) {
		theApp.m_Kinematics.NormalsFromTips(&m_CpRect[0],PORTSIDE);
		theApp.m_Kinematics.NormalsFromTips(&m_CpRect[0],STARBOARD);
		theApp.m_Kinematics.NormalsFromTips(&m_CpRect[1],PORTSIDE);
		theApp.m_Kinematics.NormalsFromTips(&m_CpRect[1],STARBOARD);


		if(m_CpRect[0].Side[nSide].fX > m_CpRect[1].Side[nSide].fX) {
			fTemp = m_CpRect[0].Side[0].fX;
			m_CpRect[0].Side[0].fX = m_CpRect[1].Side[0].fX;
			m_CpRect[1].Side[0].fX = fTemp;

			fTemp = m_CpRect[0].Side[1].fX;
			m_CpRect[0].Side[1].fX = m_CpRect[1].Side[1].fX;
			m_CpRect[1].Side[1].fX = fTemp;
		}
		if(m_CpRect[0].Side[nSide].fY > m_CpRect[1].Side[nSide].fY) {
			fTemp = m_CpRect[0].Side[0].fY;
			m_CpRect[0].Side[0].fY = m_CpRect[1].Side[0].fY;
			m_CpRect[1].Side[0].fY = fTemp;

			fTemp = m_CpRect[0].Side[1].fY;
			m_CpRect[0].Side[1].fY = m_CpRect[1].Side[1].fY;
			m_CpRect[1].Side[1].fY = fTemp;
		}
		if(m_CpRect[0].Side[nSide].fZ > m_CpRect[1].Side[nSide].fZ) {
			fTemp = m_CpRect[0].Side[0].fZ;
			m_CpRect[0].Side[0].fZ = m_CpRect[1].Side[0].fZ;
			m_CpRect[1].Side[0].fZ = fTemp;

			fTemp = m_CpRect[0].Side[1].fZ;
			m_CpRect[0].Side[1].fZ = m_CpRect[1].Side[1].fZ;
			m_CpRect[1].Side[1].fZ = fTemp;
		}


		Cp0 = m_CpRect[0];
		Cp1 = m_CpRect[1];
	}else{
		theApp.m_Kinematics.NormalsFromTips(&m_CpRect[0],PORTSIDE);
		theApp.m_Kinematics.NormalsFromTips(&m_CpRect[0],STARBOARD);
		Cp0 = m_CpRect[0];
		Cp1 = Cp0;
		Cp1.Side0.fPos[m_nFastAxis] += m_fFastScanSize;
		Cp1.Side1.fPos[m_nFastAxis] += m_fFastScanSize;
	}

	if(bCoordinates) {
		for(nLine = 0;nLine<nPoints;nLine++) {
			for(nAxis=0;nAxis<30;nAxis++) {
				if((nAxis != m_nFastAxis) && (nAxis != (m_nFastAxis + 10))){
					Cp0.fPos[nAxis] = ((float)nLine * (m_CpRect[1].fPos[nAxis] - m_CpRect[0].fPos[nAxis]) / (float)nPoints) + m_CpRect[0].fPos[nAxis];
				}
			}
			m_ScanLine[0].SetPrimaryAxis(m_nFastAxis);
			m_ScanLine[0].Add(Cp0);
			m_ScanLine[0].m_pCp[nLine].nType = CONTINUOUS_TURNTABLE;
		}
	}else{
		for(nLine = 0;nLine<nPoints;nLine++) {
			Cp0.Side0.fPos[m_nSlowAxis] = m_fSlowIncrement * (float)nLine + m_CpRect[0].Side0.fPos[m_nSlowAxis];
			m_ScanLine[0].SetPrimaryAxis(m_nFastAxis);
			m_ScanLine[0].Add(Cp0);
			m_ScanLine[0].m_pCp[nLine].nType = CONTINUOUS_TURNTABLE;
		}
	}



	m_nWayPointNumber=2;


	CalculateDangerZone();

    return HRESULT();

}



void CProfile::GetDiameters(float* fDiameters)
{
	
	float fX,fY;
	CCoord Cp;

	if(m_bDiametersDervivedFromCoords == true) {
		if(m_ScanLine && m_ScanLine[0].m_nCoordL >= 1) {
			m_ScanLine[0].GetCoord(0,&Cp);
			fX = Cp.Side[0].fX - theApp.m_Tank.vTTCentre.x;
			fY = Cp.Side[0].fY - theApp.m_Tank.vTTCentre.y;
			fDiameters[0] = (float)sqrt((double)(fX*fX + fY*fY)) * 2.0f;

			m_ScanLine[0].GetCoord(m_ScanLine[0].m_nCoordL-1,&Cp);
			fX = Cp.Side[0].fX - theApp.m_Tank.vTTCentre.x;
			fY = Cp.Side[0].fY - theApp.m_Tank.vTTCentre.y;
			fDiameters[1] = (float)sqrt((double)(fX*fX + fY*fY)) * 2.0f;

		} else {
			fDiameters[0] = 1000.0f;
			fDiameters[1] = 1000.0f;
		}
		return;
	} else {
		fDiameters[1] = fDiameters[0] = m_fComponentDiameter[0];
	}

	fDiameters[1] = fDiameters[0] = m_fComponentDiameter[0];
	
}

void CProfile::CalculateToolPath(int nLine, int nStoreHeadSurface, int nDir)
{
	CalculateToolPath(nLine, m_ToolPath, nStoreHeadSurface, nDir);

}

void CProfile::CalculateToolPath(int nLine, CPolyCoord &ToolPath, int nStorrageDestination,int nDir)
{
	
	float	fXMin,fXMax,fX,fLength,fLastLength,fPos;
	float	fXtLast,fYtLast;
	COORDDATA CpMin,CpMax;
	int ii,jj;
	bool bUseCoord;
	CPolyCoord TempPath;
	bool bStore;
	float fCosAngle,fCosDev;
	D3DXVECTOR4 vecOA,vecOB;
	D3DXMATRIXA16 matRotate;
	int nLastii;
	float fLastStorredPos;
	float fStep;

	CCoord	CpSurface,CpHead,CpNext,CpCurrent,Cp0,Cp1,Cp;

	if(nStorrageDestination==0) {
		ToolPath.Zero();
	} else {
		m_WayPoints[nLine].Zero();
	}

	m_ScanLine[nLine].Sort(m_nFastAxis);

	CpMin = m_ScanLine[nLine].m_pCp[0];
	CpMax = m_ScanLine[nLine].m_pCp[m_ScanLine[nLine].m_nCoordL-1];

	fXMin = m_ScanLine[nLine].m_pCp[0].Side[theApp.m_nPrimarySide].fPos[m_nFastAxis];
	fXMax = m_ScanLine[nLine].m_pCp[m_ScanLine[nLine].m_nCoordL-1].Side[theApp.m_nPrimarySide].fPos[m_nFastAxis];
	m_ScanLine[nLine].SetPrimaryAxis(m_nFastAxis);

	if(m_bExclusionZones[0]) {
		if((m_CpExclusion[0][0].Side[theApp.m_nPrimarySide].fZ>=CpMin.Side[theApp.m_nPrimarySide].fZ) && (CpMin.Side[theApp.m_nPrimarySide].fZ >= m_CpExclusion[0][1].Side[theApp.m_nPrimarySide].fZ)) {
			fXMin = m_CpExclusion[0][1].Side[theApp.m_nPrimarySide].fX;
		}
	}
	if(m_bExclusionZones[1]) {
		if((m_CpExclusion[1][0].Side[theApp.m_nPrimarySide].fZ>=CpMax.Side[theApp.m_nPrimarySide].fZ) && (CpMax.Side[theApp.m_nPrimarySide].fZ >= m_CpExclusion[1][1].Side[theApp.m_nPrimarySide].fZ)) {
			fXMax = m_CpExclusion[1][0].Side[theApp.m_nPrimarySide].fX;
		}
	}


	if(theApp.m_LastSettings.fProbeSeparation < 0.0f) theApp.m_LastSettings.fProbeSeparation = 0.0f;
	if(m_nFirstProbe<0) m_nFirstProbe = 0;

	theApp.m_Motors.GetSurfacePos(&CpCurrent);

	switch(m_nWaypointMode) {
	case 0:
		MinMax(&m_nWayPointNumber, 2, 100);

		for(ii=0;ii<m_nWayPointNumber;ii++) {
			if(nDir == 0) {
				fX = ((float)ii * (fXMax-fXMin)) / (float)(m_nWayPointNumber - 1) + fXMin;
			} else {
				fX = fXMax - ((float)ii * (fXMax-fXMin)) / (float)(m_nWayPointNumber - 1);
			}
			m_ScanLine[nLine].CalculateCoord(fX,CpSurface);
			CpSurface.Side0.fY -= ((float)m_nFirstProbe * theApp.m_LastSettings.fProbeSeparation);	//For multiple probe flat bed
//			CpSurface.Side0.pt += m_vecBladeOffset[theApp.m_Thread.m_nBladeSide];

			////////
			D3DXMatrixRotationZ(&matRotate,theApp.m_fRotateAboutZDuringScan*DEG_TO_RAD);
			D3DXVec3TransformCoord((D3DXVECTOR3*)&CpSurface.Side[PORTSIDE].norm,(D3DXVECTOR3*)&CpSurface.Side[PORTSIDE].norm,&matRotate);
			D3DXVec3Normalize((D3DXVECTOR3*)&CpSurface.Side[PORTSIDE].norm,(D3DXVECTOR3*)&CpSurface.Side[PORTSIDE].norm);
			D3DXVec3TransformCoord((D3DXVECTOR3*)&CpSurface.Side[STARBOARD].norm,(D3DXVECTOR3*)&CpSurface.Side[STARBOARD].norm,&matRotate);
			D3DXVec3Normalize((D3DXVECTOR3*)&CpSurface.Side[STARBOARD].norm,(D3DXVECTOR3*)&CpSurface.Side[STARBOARD].norm);


			if((theApp.m_LastSettings.bUseBrainCompensation == TRUE) && (m_nBrainLineL>0)){
				CalculateBrainCoord(m_nFastAxis,m_nSlowAxis,CpSurface);
			}
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

			switch(nStorrageDestination) {
			case 0:
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
//				ToolPath.Add(CpNext);
				TempPath.Add(CpSurface);
				break;
			case 1:
				m_WayPoints[nLine].Add(CpSurface);
				break;
			case 2:
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
				m_WayPoints[nLine].Add(CpNext);
				break;
			}
		}
		if(nStorrageDestination == 0) {
			TempPath.Sort(m_nFastAxis);
			for(ii=0;ii<m_nWayPointNumber;ii++) {
				theApp.m_Kinematics.HeadFromSurface((CCoord*)&TempPath.m_pCp[ii],&CpNext,PORTSIDE,TRUE,TRUE,NULL,0);
				theApp.m_Kinematics.HeadFromSurface((CCoord*)&TempPath.m_pCp[ii],&CpNext,STARBOARD,TRUE,TRUE,NULL,0);
				ToolPath.Add(CpNext);
			}
		}
		break;
	case 1:
		fLastLength = 0.0f;
		fXtLast = CpMin.Side[0].fXt;
		fYtLast = CpMin.Side[0].fYt;
		for(ii=0;ii<200;ii++) {
			fX = ((float)ii * (fXMax-fXMin)) / (float)(200 - 1) + fXMin;
			m_ScanLine[nLine].CalculateCoord(fX,CpSurface);

			if(theApp.m_LastSettings.bUseBrainCompensation == TRUE) {
				CalculateBrainCoord(m_nFastAxis,m_nSlowAxis,CpSurface);
			}
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

			bUseCoord = FALSE;
			if(m_fWaypointXYZChange>0.0) {
				fLength = CpSurface.DistanceToLine(CpMin,CpMax);
				if(fabs(fLength-fLastLength) >= m_fWaypointXYZChange) bUseCoord = TRUE;
			}
			if(m_fWaypointIJKChange>0.0) {
				if(fabs(CpSurface.Side[0].fXt - fXtLast) > m_fWaypointIJKChange) bUseCoord = TRUE;
				if(fabs(CpSurface.Side[0].fYt - fYtLast) > m_fWaypointIJKChange) bUseCoord = TRUE;
			}

			if((ii==0) || (ii==199) || (bUseCoord==TRUE)) {
				fLastLength = fLength;
				fXtLast = CpSurface.Side[0].fXt;
				fYtLast = CpSurface.Side[0].fYt;

				if(nStorrageDestination == 0) {
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE, NULL, 0);
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE, NULL, 0);
					ToolPath.Add(CpHead);
				} else {
					m_WayPoints[nLine].Add(CpSurface);
				}
			}
		}
		break;
	case 2:
		fCosAngle = cosf(m_fWaypointIJKChange * DEG_TO_RAD);
		m_ScanLine[nLine].SetPrimaryAxis(30);
		m_ScanLine[nLine].SetStyle(0);
		fLength = m_ScanLine[nLine].GetLength();
		fStep = fLength / 200.0f;
		if(fStep<2.0f) fStep = 2.0f;

		for(fPos=0;fPos<=fLength;fPos+=fStep) {
			bStore = false;
			m_ScanLine[nLine].CalculateCoord(fPos,CpSurface);

			if(theApp.m_LastSettings.bUseBrainCompensation == TRUE) {
				CalculateBrainCoord(m_nFastAxis,m_nSlowAxis,CpSurface);
			}
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

			if(fPos==0.0f) {
				Cp0 = CpSurface;
				CpCurrent = CpSurface;
				fLastStorredPos = fPos;
				bStore = true;
			} else {
				vecOA = Cp0.Side0.norm;
				vecOB = CpSurface.Side0.norm;
				D3DXVec4Normalize(&vecOA,&vecOA);
				D3DXVec4Normalize(&vecOB,&vecOB);
				fCosDev =  D3DXVec4Dot(&vecOA,&vecOB);
				if((fabs(fCosDev) <= fCosAngle) || ((fPos - fLastStorredPos) > m_fWaypointXYZChange)) {
					Cp0 = CpSurface;
					fLastStorredPos = fPos;
					bStore = true;
				}
			}
			if(bStore == true) {
				switch(nStorrageDestination) {
				case 0:
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
					ToolPath.Add(CpNext);
//					TempPath.Add(CpSurface);
					break;
				case 1:
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
//					m_WayPoints[nLine].Add(CpNext);
					m_WayPoints[nLine].Add(CpSurface);
					break;
				case 2:
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
					m_WayPoints[nLine].Add(CpNext);
					break;
				}
			}
		}
		m_ScanLine[nLine].CalculateCoord(fLength,CpSurface);
		if(theApp.m_LastSettings.bUseBrainCompensation == TRUE) {
			CalculateBrainCoord(m_nFastAxis,m_nSlowAxis,CpSurface);
		}
		theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
		theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

		switch(nStorrageDestination) {
		case 0:
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
			ToolPath.Add(CpNext);
//			TempPath.Add(CpSurface);
			break;
		case 1:
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
//			m_WayPoints[nLine].Add(CpNext);
			m_WayPoints[nLine].Add(CpSurface);
			break;
		case 2:
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
			m_WayPoints[nLine].Add(CpNext);
			break;
		}
		break;
	case 3:
		for(ii=0,jj=m_ScanLine[nLine].m_nCoordL-1;ii<m_ScanLine[nLine].m_nCoordL;ii++,jj--) {

//			if(nDir==0) {
				CpSurface = m_ScanLine[nLine].m_pCp[ii];
//			} else {
//				CpSurface = m_ScanLine[nLine].m_pCp[jj];
//			}
			if(theApp.m_LastSettings.bUseBrainCompensation == TRUE) {
				CalculateBrainCoord(m_nFastAxis,m_nSlowAxis,CpSurface);
			}
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

			if(nStorrageDestination == 0) {
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
				ToolPath.Add(CpNext);
			} else {
				m_WayPoints[nLine].Add(CpSurface);
			}
		}
		break;
	case 4:	//Robot facets
		m_ScanLine[nLine].GetFirstCoord(&CpSurface);
		CpCurrent = CpSurface;
		m_WayPoints[nLine].Add(CpSurface);
		nLastii = 0;
		for(ii=1; ii<m_ScanLine[nLine].m_nCoordL; ii++) {
			m_ScanLine[nLine].GetCoord(ii,&CpSurface);
			if(
				(fabsf(CpSurface.Side0.fI - CpCurrent.Side0.fI) > 0.001) ||
				(fabsf(CpSurface.Side0.fJ - CpCurrent.Side0.fJ) > 0.001) ||
				(fabsf(CpSurface.Side0.fK - CpCurrent.Side0.fK) > 0.001)
				) {
				if((ii - nLastii) > 1) {
					m_WayPoints[nLine].Add(CpCurrent);
				}
				m_WayPoints[nLine].Add(CpSurface);
				nLastii = ii;
			} else {
				if((ii - nLastii) > 6) {
					m_WayPoints[nLine].Add(CpSurface);
					nLastii = ii;
				}
			}
			CpCurrent = CpSurface;
		}
		if(nLastii != (m_ScanLine[nLine].m_nCoordL -1)) {
			m_WayPoints[nLine].Add(CpSurface);
		};
		return;
		break;
	case 5:
		fCosAngle = cosf(m_fWaypointIJKChange * DEG_TO_RAD);
		m_ScanLine[nLine].SetPrimaryAxis(30);
		m_ScanLine[nLine].SetStyle(0);
		fLength = m_ScanLine[nLine].GetLength();

		for(fPos=0;fPos<fLength;fPos+=2.0) {
			bStore = false;
			m_ScanLine[nLine].CalculateCoord(fPos,CpSurface);

			if(theApp.m_LastSettings.bUseBrainCompensation == TRUE) {
				CalculateBrainCoord(m_nFastAxis,m_nSlowAxis,CpSurface);
			}
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

			if(fPos==0.0f) {
				Cp0 = CpSurface;
				CpCurrent = CpSurface;
				bStore = true;
			} else {
				vecOA = Cp0.Side0.norm;
				vecOB = CpSurface.Side0.norm;
				D3DXVec4Normalize(&vecOA,&vecOA);
				D3DXVec4Normalize(&vecOB,&vecOB);
				fCosDev =  D3DXVec4Dot(&vecOA,&vecOB);
				if(fabs(fCosDev) <= fCosAngle) {
					Cp0 = CpSurface;
					bStore = true;
				}
			}
			if(bStore == true) {
				switch(nStorrageDestination) {
				case 0:
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
					ToolPath.Add(CpNext);
//					TempPath.Add(CpSurface);
					break;
				case 1:
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
//					m_WayPoints[nLine].Add(CpNext);
					m_WayPoints[nLine].Add(CpSurface);
					break;
				case 2:
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
					m_WayPoints[nLine].Add(CpNext);
					break;
				}
			}
		}
		for(fPos=fLength;fPos>0;fPos-=2.0) {
			bStore = false;
			m_ScanLine[nLine].CalculateCoord(fPos,CpSurface);

			if(theApp.m_LastSettings.bUseBrainCompensation == TRUE) {
				CalculateBrainCoord(m_nFastAxis,m_nSlowAxis,CpSurface);
			}
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
			theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

			if(fPos==fLength) {
				Cp0 = CpSurface;
				CpCurrent = CpSurface;
				bStore = true;
			} else {
				vecOA = Cp0.Side0.norm;
				vecOB = CpSurface.Side0.norm;
				D3DXVec4Normalize(&vecOA,&vecOA);
				D3DXVec4Normalize(&vecOB,&vecOB);
				fCosDev =  D3DXVec4Dot(&vecOA,&vecOB);
				if(fabs(fCosDev) <= fCosAngle) {
					Cp0 = CpSurface;
					bStore = true;
				}
			}
			if(bStore == true) {
				switch(nStorrageDestination) {
				case 0:
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
					ToolPath.Add(CpNext);
//					TempPath.Add(CpSurface);
					break;
				case 1:
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
//					m_WayPoints[nLine].Add(CpNext);
					m_WayPoints[nLine].Add(CpSurface);
					break;
				case 2:
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
					m_WayPoints[nLine].Add(CpNext);
					break;
				}
			}
		}
		ToolPath.Sort(m_nFastAxis);
		m_WayPoints[nLine].Sort(m_nFastAxis);
		break;
	case 6:
		fCosAngle = cosf(m_fWaypointIJKChange * DEG_TO_RAD);
		m_ScanLine[nLine].GetFirstCoord(&CpSurface);
		CpCurrent = CpSurface;
		switch(nStorrageDestination) {
		case 0:
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
			ToolPath.Add(CpNext);
			break;
		case 1:
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
			m_WayPoints[nLine].Add(CpSurface);
			break;
		case 2:
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
			theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
			m_WayPoints[nLine].Add(CpNext);
			break;
		}
		vecOA = CpSurface.Side0.norm;
		D3DXVec4Normalize(&vecOA,&vecOA);
		nLastii = 0;
		for(ii=1; ii<m_ScanLine[nLine].m_nCoordL; ii++) {
			m_ScanLine[nLine].GetCoord(ii,&CpSurface);
			vecOB = CpSurface.Side0.norm;
			D3DXVec4Normalize(&vecOB,&vecOB);
			fCosDev =  D3DXVec4Dot(&vecOA,&vecOB);
			if(fabs(fCosDev) <= fCosAngle) {
				if((ii - nLastii) > 1) {
					switch(nStorrageDestination) {
					case 0:
						ToolPath.Add(CpCurrent);
						break;
					case 1:
						m_WayPoints[nLine].Add(CpCurrent);
						break;
					case 2:
						m_WayPoints[nLine].Add(CpCurrent);
						break;
					}
				}
				switch(nStorrageDestination) {
				case 0:
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
					ToolPath.Add(CpNext);
					break;
				case 1:
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
					m_WayPoints[nLine].Add(CpSurface);
					break;
				case 2:
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
					theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
					m_WayPoints[nLine].Add(CpNext);
					break;
				}
				vecOA = vecOB;
				nLastii = ii;
			} else {
				if((ii - nLastii) > 6) {
					m_WayPoints[nLine].Add(CpSurface);
					vecOA = CpSurface.Side0.norm;
					D3DXVec4Normalize(&vecOA,&vecOA);
					nLastii = ii;
				}
			}
			CpCurrent = CpSurface;
		}
		if(nLastii != (m_ScanLine[nLine].m_nCoordL -1)) {
			m_ScanLine[nLine].GetLastCoord(&CpSurface);
			switch(nStorrageDestination) {
			case 0:
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
				ToolPath.Add(CpNext);
				break;
			case 1:
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
				m_WayPoints[nLine].Add(CpSurface);
				break;
			case 2:
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);
				m_WayPoints[nLine].Add(CpNext);
				break;
			}

		};
		break;
	}

	int nTooLLast = ToolPath.m_nCoordL-1;
	int nScanLineLast = m_ScanLine[nLine].m_nCoordL-1;
	switch(nStorrageDestination) {
	case 0:
		theApp.m_Motors.GetHeadPos(&CpCurrent);
		switch(theApp.m_PmacUser.m_nScanSide) {
		case 0: 
			ToolPath.SetAllCoordsButNotR(CpCurrent,1);
			break;
		case 1: ToolPath.SetAllCoords(CpCurrent,0);
			break;
		case 2:
			break;
		}
		theApp.m_Kinematics.MoveInToCorrectQuadrant(&ToolPath,PORTSIDE);
		theApp.m_Kinematics.MoveInToCorrectQuadrant(&ToolPath,STARBOARD);
		ToolPath.m_pCp[0].nType = m_ScanLine[nLine].m_pCp[0].nType;
		ToolPath.m_pCp[nTooLLast].nType = m_ScanLine[nLine].m_pCp[nScanLineLast].nType;
		break;
	case 1:
	case 2: theApp.m_Kinematics.MoveInToCorrectQuadrant(&m_WayPoints[nLine],PORTSIDE);
			theApp.m_Kinematics.MoveInToCorrectQuadrant(&m_WayPoints[nLine],STARBOARD);
		break;
	}
	
}



void CProfile::CalculateWayPoints(PVOID pVoid)
{

	if(m_bCalculateWayPointsThreadEnabled != FALSE) {
		m_bCalculateWayPointsThreadEnabled=FALSE;
		WaitForSingleObject(m_pBackGroundThread,INFINITE);
	} else {
		m_bCalculateWayPointsThreadEnabled = true;
		m_pBackGroundThread = AfxBeginThread(&CalculateWayPointsThread, pVoid, THREAD_PRIORITY_NORMAL, 0, NULL);
	}
}

void CProfile::SuspendCalculateWayPointsThread()
{

	if(m_bCalculateWayPointsThreadEnabled==TRUE) {
		m_bCalculateWayPointsThreadEnabled=FALSE;
		WaitForSingleObject(m_pBackGroundThread,INFINITE);
	};
}

UINT CalculateWayPointsThread(LPVOID pParam)
{
	int nLine;
	int nLastProgressPoint;
	CCoord CpCurrent;
	CWayLinesEditDlg* pWnd = (CWayLinesEditDlg*)pParam;


	if(pWnd != NULL) {
		pWnd->m_progressBar.SetRange(0,100);
		pWnd->m_progressBar.SetPos(0);
		nLastProgressPoint = 0;
	}

	SAFE_DELETE_ARRAY(PROFILE->m_WayPoints);
	if(PROFILE->m_nScanLineL>0) {
		PROFILE->m_nWayPointsL = PROFILE->m_nScanLineL;
		PROFILE->m_WayPoints = new CPolyCoord[PROFILE->m_nWayPointsL];

		for(nLine = 0;nLine < PROFILE->m_nWayPointsL && PROFILE->m_bCalculateWayPointsThreadEnabled == true; nLine++) {
			if(pWnd != NULL) {
				if(nLastProgressPoint != MulDiv(nLine,100,PROFILE->m_nWayPointsL-1)) {
					pWnd->m_progressBar.SetPos(nLastProgressPoint = MulDiv(nLine,100,PROFILE->m_nWayPointsL-1));
				}
			}

			PROFILE->CalculateToolPath(nLine,1);
		}

		CpCurrent = PROFILE->m_WayPoints[0].m_pCp[0];
		for(nLine = 0;nLine < PROFILE->m_nWayPointsL && PROFILE->m_bCalculateWayPointsThreadEnabled == true; nLine++) {
			theApp.m_Kinematics.RThetaShortestRoutes(&PROFILE->m_WayPoints[nLine],nLine & 1,&CpCurrent);
		}
		if(pWnd != NULL) {
			pWnd->m_progressBar.SetPos(0);
		}
	}

	if(pWnd != NULL && PROFILE->m_bCalculateWayPointsThreadEnabled == true) {  //only send message if thread finished correctly
		PROFILE->m_bCalculateWayPointsThreadEnabled = false;
		pWnd->SendMessage(UI_CALCULATE_WAYPOINTS_FINISHED);
	}
	PROFILE->m_bCalculateWayPointsThreadEnabled = false;

	return 0;
}

int CProfile::GetMaxWayPoints(CString *pBuff)
{
	int nMax=0;
	for(int nLine=0;nLine<m_nWayPointsL;nLine++) {
		if(nMax < m_WayPoints[nLine].m_nCoordL) {
			nMax = m_WayPoints[nLine].m_nCoordL;
		}
	}
	if(pBuff != NULL) {
		pBuff->Format(_T("%d"),nMax);
	}

	return nMax;
}


#define _GOTO	1
#define	_$$$	2

void CProfile::ImportAptSourceGKNFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType)
{
	
	CUSLFile file;
	CArchive* pArchFile;
	CString	Buff;
	ULONGLONG	nTotalBytesRead;
	int nCoord;
	int nBytesRead;
	COORDDATA	Cp;
	Cp.Zero();
	int nPos,nOldPos=0,nLastLine,nThisLine;

	if(!file.Open(FileName,CFile::modeRead|CFile::typeBinary)) {

	} else {
		ULONGLONG nFileLength = file.GetLength();
		pArchFile = new CArchive(&file,CArchive::load);
		Zero();


		//Find number of Taught Lines
		pbuttonHowManyLines->SetCheck(1);
		m_nTaughtLineL=0;
		nBytesRead=pArchFile->ReadString(Buff);
		nTotalBytesRead=Buff.GetLength();
		nLastLine = 0;
		nThisLine = 0;
		while(nBytesRead) {
			nThisLine=0;
			if(Buff.Find(_T("GOTO"),0)==0) nThisLine = _GOTO;
			if(Buff.Find(_T("$$$"),0)==0) nThisLine = _$$$;
			if(Buff.Find(_T("Drilling"),0) > 5) goto line_count_finished;

			if((nLastLine == _GOTO) && (nThisLine != _GOTO)) {
				m_nTaughtLineL++;
			}
			nBytesRead=pArchFile->ReadString(Buff);
			nTotalBytesRead+=Buff.GetLength();

			nPos = (int)MulDiv(nTotalBytesRead,(ULONGLONG)100,nFileLength);
			if(nPos-nOldPos)
				pProgress->SetPos(nOldPos = nPos);

			nLastLine = nThisLine;
		}

line_count_finished:

		pArchFile->Close();
		delete pArchFile;

		if(m_nTaughtLineL<=0) {
			file.Close();
			return;
		}

		delete [] m_TaughtLine;
		m_TaughtLine = new CPolyCoord[m_nTaughtLineL];

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);

		//read in coords and make lines
		pbuttonReadCoordinates->SetCheck(1);
		int nLine=0;
		nBytesRead=pArchFile->ReadString(Buff);
		while(nBytesRead) {
			if(Buff.Find(_T("GOTO"),0)==0) {
				nCoord=0;

				nPos = MulDiv(nLine,100,m_nTaughtLineL);
				if(nPos-nOldPos)
					pProgress->SetPos(nOldPos = nPos);

				while(Buff.Find(_T("GOTO"),0)==0) {

					DecodeGotoLine(Buff,&Cp);

					m_TaughtLine[nLine] + (Cp);
					nBytesRead=pArchFile->ReadString(Buff);
				}
				nLine++;
			} else {
				if(Buff.Find(_T("Drilling"),0) > 5) {
					goto import_drilling;
				}
				nBytesRead=pArchFile->ReadString(Buff);
			}
		}

import_drilling:





		//read in hole coordinates
		pbuttonReadingHoles->SetCheck(1);
		pProgress->SetPos(0);

		int	nHole = 0;
		int nGoto = 0;
		while(nBytesRead) {





			if(Buff.Find(_T("GOTO"),0)==0) {
				pProgress->SetPos(MulDiv(nHole+1,100,3));
				ZeroMemory(&Cp,sizeof Cp);

				DecodeGotoLine(Buff,&Cp);

				switch(nGoto) {
				case 0:
				case 3:
				case 6:
					m_CpCadHole[nHole] = Cp;
					nHole++;
					break;
				}

				nGoto++;
			}
			nBytesRead=pArchFile->ReadString(Buff);
		}

		pArchFile->Close();
		delete pArchFile;

		file.Close();

		if(theApp.m_LastSettings.nImportAptDirection == 1) {
			ReverseLines(SCANLINES,2);
		}

		if(theApp.m_LastSettings.nReverseLineOrder == 1) {
			ReverseScanLineOrder();
		}

		m_bModified = TRUE;
		if(theApp.m_LastSettings.nPlaceComponentInScanner) {
			if(nHole>0) {
				GetTheProfileInTheScanner(0);
			} else {
				m_ScanLine[0].GetCoord(0,&Cp);
				GetTheProfileInTheScanner(1,&Cp);
			}
		}

	}

}

void CProfile::Untwist()
{
	
	CCoord CpStart,CpFinish,Cp0,Cp1,Cp;
	float fFastScanSize,fSlowScanSize,fAngle;
	CCoord CpSurface;
	int nLine=0,ii,nSide;
	D3DXMATRIXA16 matRotate;

	m_nScanStartLine=0;
	m_nScanFinishLine = m_nScanLineL-1;
	theApp.m_Motors.GetSurfacePos(&CpSurface);

	FindScanSizes(&fFastScanSize,&fSlowScanSize,&CpStart,&CpFinish);

	switch(theApp.m_LastSettings.nUnTwistMode) {
	case 0:

		for(nLine = 0;nLine<m_nScanLineL;nLine++) {
			Cp0 = m_ScanLine[nLine].m_pCp[0];
			Cp1 = m_ScanLine[nLine].m_pCp[m_ScanLine[nLine].m_nCoordL-1];
			//fAngle = (float)(atan2((Cp1.Side0.fZ - Cp0.Side0.fZ) , (Cp0.Side0.fY - Cp1.Side0.fY)) * RAD_TO_DEG);
			fAngle = (float)(atan2((Cp1.Side0.fZ - Cp0.Side0.fZ) , (Cp0.Side0.fX - Cp1.Side0.fX)) * RAD_TO_DEG);

			//m_fRotateX = 90.0f - fAngle; //rotate to 90 - fAngle. -ve as twist is opposite to axis direction
			m_fRotateZ = 180.0f + fAngle; //rotate to 90 - fAngle. -ve as twist is opposite to axis direction
			CalculateMatrix();

			for(ii=0;ii<m_ScanLine[nLine].m_nCoordL;ii++) {
				TransformCoordinate(m_ScanLine[nLine].m_pCp[ii],&Cp);
				theApp.m_Kinematics.TipsFromNormals((CCoord*)&Cp,PORTSIDE);
				theApp.m_Kinematics.TipsFromNormals((CCoord*)&Cp,STARBOARD);
				//Cp.Side0.fR = 90.0f + m_fRotateX; //the number is the angle the blade is in the fixture may not be quite 90!
				Cp.Side0.fR = 180.0f - m_fRotateZ;
				m_ScanLine[nLine].m_pCp[ii]=Cp;
			}

		}
		break;
	case 1:
		for(nLine = 0;nLine<m_nScanLineL;nLine++) {
			Cp0 = m_ScanLine[nLine].m_pCp[0];
			Cp1 = m_ScanLine[nLine].m_pCp[m_ScanLine[nLine].m_nCoordL-1];
			fAngle = (float)(atan2((Cp1.Side0.fY - Cp0.Side0.fY) , (Cp1.Side0.fZ - Cp0.Side0.fZ)) * RAD_TO_DEG) * -1.0f;
			D3DXMatrixRotationX(&matRotate,-fAngle*DEG_TO_RAD);
			PROFILE->ComponentMoveLine(matRotate,SCANLINES,nLine);
			if(theApp.m_LastSettings.nUntwistDirection != 0) fAngle *= -1.0f;
			for(ii=0;ii<m_ScanLine[nLine].m_nCoordL;ii++) {
				m_ScanLine[nLine].m_pCp[ii].Side0.fR += fAngle;
			}
		}
		break;

	case 2:
		for(nLine = 0;nLine<m_nTaughtLineL;nLine++) {
			Cp0 = m_TaughtLine[nLine].m_pCp[0];
			Cp1 = m_TaughtLine[nLine].m_pCp[m_TaughtLine[nLine].m_nCoordL-1];
			fAngle = (float)(atan2((Cp1.Side0.fZ - Cp0.Side0.fZ) , (Cp1.Side0.fX - Cp0.Side0.fX)) * RAD_TO_DEG) * -1.0f;
			D3DXMatrixRotationY(&matRotate,-fAngle*DEG_TO_RAD);
			PROFILE->ComponentMoveLine(matRotate,TAUGHTLINES,nLine);
			if(theApp.m_LastSettings.nUntwistDirection != 0) fAngle *= -1.0f;
			for(ii=0;ii<m_TaughtLine[nLine].m_nCoordL;ii++) 
				m_TaughtLine[nLine].m_pCp[ii].Side0.fR += fAngle;
		}
		for(nLine = 0;nLine<m_nScanLineL;nLine++) {
			Cp0 = m_ScanLine[nLine].m_pCp[0];
			Cp1 = m_ScanLine[nLine].m_pCp[m_ScanLine[nLine].m_nCoordL-1];
			fAngle = (float)(atan2((Cp1.Side0.fZ - Cp0.Side0.fZ) , (Cp1.Side0.fX - Cp0.Side0.fX)) * RAD_TO_DEG) * -1.0f;
			D3DXMatrixRotationY(&matRotate,-fAngle*DEG_TO_RAD);
			PROFILE->ComponentMoveLine(matRotate,SCANLINES,nLine);
			if(theApp.m_LastSettings.nUntwistDirection != 0) fAngle *= -1.0f;
			for(ii=0;ii<m_ScanLine[nLine].m_nCoordL;ii++) {
				m_ScanLine[nLine].m_pCp[ii].Side0.fR += fAngle;
			}
		}
		for(nSide=0;nSide<2;nSide++) {
			for(nLine = 0;nLine<m_nLineBladeL[nSide];nLine++) {
				Cp0 = m_LineBlade[nSide][nLine].m_pCp[0];
				Cp1 = m_LineBlade[nSide][nLine].m_pCp[m_LineBlade[nSide][nLine].m_nCoordL-1];
				fAngle = (float)(atan2((Cp1.Side0.fZ - Cp0.Side0.fZ) , (Cp1.Side0.fX - Cp0.Side0.fX)) * RAD_TO_DEG) * -1.0f;
				D3DXMatrixRotationY(&matRotate,-fAngle*DEG_TO_RAD);
				switch(nSide) {
				case 0: ComponentMoveLine(matRotate,OUTSIDELINES,nLine);
					break;
				case 1: ComponentMoveLine(matRotate,INSIDELINES,nLine);
					break;
				}
				if(theApp.m_LastSettings.nUntwistDirection != 0) fAngle *= -1.0f;
				for(ii=0;ii<m_LineBlade[nSide][nLine].m_nCoordL;ii++) 
					m_LineBlade[nSide][nLine].m_pCp[ii].Side0.fR += fAngle;
			}
		}
		break;
	}

	m_nRUsed = 1;


	CalculateDangerZone();

}

#define _GOTO	1
#define	_$$$	2
#define	_RAPID	3

void CProfile::ImportAptSourceTermaFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType)
{
	
	CUSLFile file;
	CArchive* pArchFile;
	CString	Buff, NextBuff;
	ULONGLONG	nTotalBytesRead;
	int nCoord;
	int nBytesRead;
	COORDDATA	Cp;
	int nPointCount=0;
	int nRapidCount=0;
	Cp.Zero();
	int nPos,nOldPos=0,nLastLine,nThisLine;

	if(!file.Open(FileName,CFile::modeRead|CFile::typeBinary)) {

	} else {
		ULONGLONG nFileLength = file.GetLength();
		pArchFile = new CArchive(&file,CArchive::load);
		Zero();


		//Find number of Scan Lines
		pbuttonHowManyLines->SetCheck(1);
		m_nScanLineL=0;
		nBytesRead=pArchFile->ReadString(Buff);
		nTotalBytesRead=Buff.GetLength();
		nLastLine = 0;
		nThisLine = 0;
		while(nBytesRead) {
			nThisLine=0;
			if(Buff.Find(_T("GOTO"),0)==0) {
				nThisLine = _GOTO;
				nPointCount++;
			}
			if(Buff.Find(_T("$$$"),0)==0) nThisLine = _$$$;
			if(Buff.Find(_T("Drill"),0) > 5) goto line_count_finished;

			if( (nLastLine == _GOTO) && (nThisLine != _GOTO) )  {
					m_nScanLineL++;
			}

			nBytesRead=pArchFile->ReadString(Buff);
			nTotalBytesRead+=Buff.GetLength();

			nPos = (int)MulDiv(nTotalBytesRead,(ULONGLONG)100,nFileLength);
			if(nPos-nOldPos)
				pProgress->SetPos(nOldPos = nPos);

			nLastLine = nThisLine;
		}

line_count_finished:
		
		pArchFile->Close();
		delete pArchFile;

		if(m_nScanLineL<=0) {
			file.Close();
			return;
		}

		SAFE_DELETE_ARRAY( m_ScanLine );
		m_ScanLine = new CPolyCoord[m_nScanLineL];

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);

		//read in coords and make lines
		pbuttonReadCoordinates->SetCheck(1);
		int nLine=0;
		nBytesRead=pArchFile->ReadString(Buff);
		while(nBytesRead) {
			if(Buff.Find(_T("GOTO"),0)==0) {
				nCoord=0;

				nPos = MulDiv(nLine,100,m_nScanLineL);
				if(nPos-nOldPos)
					pProgress->SetPos(nOldPos = nPos);

				while(Buff.Find(_T("GOTO"),0)==0){

					DecodeGotoLine(Buff,&Cp);

					m_ScanLine[nLine] + (Cp);
					nBytesRead=pArchFile->ReadString(Buff);

					while(Buff.Find(_T("FEDRAT"),0)==0) {
						nBytesRead=pArchFile->ReadString(Buff);
					}

				}

				nLine++;
			} else {
				if(Buff.Find(_T("Drill"),0) > 5) {
					goto import_drilling;
				}
				nBytesRead=pArchFile->ReadString(Buff);
			}
		}

import_drilling:





		//read in hole coordinates
		pbuttonReadingHoles->SetCheck(1);
		pProgress->SetPos(0);

		int	nHole = 0;
		int nGoto = 0;
		while(nBytesRead) {

			if(Buff.Find(_T("GOTO"),0)==0) {
				pProgress->SetPos(MulDiv(nHole+1,100,3));
				ZeroMemory(&Cp,sizeof Cp);

				DecodeGotoLine(Buff,&Cp);

				switch(nGoto) {
				case 0:
				case 1:
				case 2:
					m_CpCadHole[nHole] = Cp;
					nHole++;
					break;
				}

				nGoto++;
			}
			nBytesRead=pArchFile->ReadString(Buff);
		}

		pArchFile->Close();
		delete pArchFile;

		file.Close();

		if(theApp.m_LastSettings.nImportAptDirection == 1) {
			ReverseLines(SCANLINES,2,1);
		}

		if(theApp.m_LastSettings.nReverseLineOrder == 1) {
			ReverseScanLineOrder();
		}

		m_bModified = TRUE;
		if(theApp.m_LastSettings.nPlaceComponentInScanner) {
			if(nHole>0) {
				GetTheProfileInTheScanner(0);
			} else {
				m_ScanLine[0].GetCoord(0,&Cp);
				GetTheProfileInTheScanner(1,&Cp);
			}
		}



	}

}
/*
void CProfile::ImportAptSourceGknJsfFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType)
{
	
	CUSLFile file;
	CArchive* pArchFile;
	CString	Buff, NextBuff;
	int	nTotalBytesRead,nCoord;
	int nBytesRead;
	COORDDATA	Cp;
	int nPointCount=0;
	int nRapidCount=0;
	Cp.Zero();
	int nPos,nOldPos,nLastLine,nThisLine;

	if(!file.Open(FileName,CFile::modeRead|CFile::typeBinary)) {

	} else {
		int nFileLength = file.GetLength();
		pArchFile = new CArchive(&file,CArchive::load);
		Zero();


		//Find number of Taught Lines
		pbuttonHowManyLines->SetCheck(1);
		m_nScanLineL=0;
		nBytesRead=pArchFile->ReadString(Buff);
		nTotalBytesRead=Buff.GetLength();
		nLastLine = 0;
		nThisLine = 0;
		while(nBytesRead) {
			nThisLine=0;
			if(Buff.Find(_T("GOTO"),0)==0) {
				nThisLine = _GOTO;
				nPointCount++;
			}
			if(Buff.Find(_T("RAPID"),0)==0) nThisLine = _RAPID;
			if(Buff.Find(_T("FEDRAT"),0)==0) goto line_count_finished;

			if( (nLastLine == _RAPID) && (nThisLine == _GOTO) )  {
					m_nScanLineL++;
			}

			nBytesRead=pArchFile->ReadString(Buff);
			nTotalBytesRead+=Buff.GetLength();

			nPos = MulDiv(nTotalBytesRead,100,nFileLength);
			if(nPos-nOldPos)
				pProgress->SetPos(nOldPos = nPos);

			nLastLine = nThisLine;
		}

line_count_finished:
		
		pArchFile->Close();
		delete pArchFile;

		if(m_nScanLineL<=0) {
			file.Close();
			return;
		}

		delete [] m_ScanLine;
		m_ScanLine = new CPolyCoord[m_nScanLineL];

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);

		//read in coords and make lines
		pbuttonReadCoordinates->SetCheck(1);
		int nLine=0;
		nBytesRead=pArchFile->ReadString(Buff);
		while(nBytesRead) {
			if(Buff.Find(_T("GOTO"),0)==0) {
				nCoord=0;

				nPos = MulDiv(nLine,100,m_nScanLineL);
				if(nPos-nOldPos)
					pProgress->SetPos(nOldPos = nPos);

				while(Buff.Find(_T("GOTO"),0)==0){

					DecodeGotoLine(Buff,&Cp);

					if(nLine < m_nScanLineL) m_ScanLine[nLine] + (Cp);
					nBytesRead=pArchFile->ReadString(Buff);

					while(Buff.Find(_T("FEDRAT"),0)==0) {
						nBytesRead=pArchFile->ReadString(Buff);
						goto import_drilling;
					}

				}

				nLine++;
			} else {
				if(Buff.Find(_T("FEDRAT"),0)==0) {
					goto import_drilling;
				}
				nBytesRead=pArchFile->ReadString(Buff);
			}
		}

import_drilling:





		//read in hole coordinates
		pbuttonReadingHoles->SetCheck(1);
		pProgress->SetPos(0);

		int	nHole = 0;
		int nGoto = 0;
		while(nBytesRead) {

			if(Buff.Find(_T("GOTO"),0)==0) {
				pProgress->SetPos(MulDiv(nHole+1,100,3));
				ZeroMemory(&Cp,sizeof Cp);

				DecodeGotoLine(Buff,&Cp);

				switch(nGoto) {
				case 0:
				case 1:
				case 2:
					m_CpCadHole[nHole] = Cp;
					nHole++;
					break;
				}

				nGoto++;
			}
			nBytesRead=pArchFile->ReadString(Buff);
		}

		pArchFile->Close();
		delete pArchFile;

		file.Close();

		if(theApp.m_LastSettings.nImportAptDirection == 1) {
			ReverseLines(SCANLINES,2);
		}

		if(theApp.m_LastSettings.nReverseLineOrder == 1) {
			ReverseScanLineOrder();
		}

		m_bModified = TRUE;
		if(theApp.m_LastSettings.nPlaceComponentInScanner) {
			if(nHole>0) {
				GetTheProfileInTheScanner(0);
			} else {
				m_ScanLine[0].GetCoord(0,&Cp);
				GetTheProfileInTheScanner(1,&Cp);
			}
		}



	}

}
*/


void CProfile::ImportAptSourceGknJsfFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType)
{
	
	CUSLFile file;
	CArchive* pArchFile;
	CString	Buff, NextBuff;
	ULONGLONG	nTotalBytesRead;
	int nCoord;
	int nBytesRead;
	COORDDATA	Cp;
	int nPointCount=0;
	int nRapidCount=0;
	int nLine=0;
	Cp.Zero();
	int nPos,nOldPos,nLastLine,nThisLine;

	if(!file.Open(FileName,CFile::modeRead|CFile::typeBinary)) {

	} else {
		ULONGLONG nFileLength = file.GetLength();
		pArchFile = new CArchive(&file,CArchive::load);
		Zero();

import_drilling:

		//read in hole coordinates
		pbuttonReadingHoles->SetCheck(1);
		pProgress->SetPos(0);

		int	nHole = 0;
		int nGoto = 0;
		nBytesRead=pArchFile->ReadString(Buff);

		while(nBytesRead) {

			if(Buff.Find(_T("GOTO"),0)==0) {
				pProgress->SetPos(MulDiv(nHole+1,100,3));
				ZeroMemory(&Cp,sizeof Cp);

				DecodeGotoLine(Buff,&Cp);

				switch(nGoto) {
				case 0:
				case 1:
				case 2:
					m_CpCadHole[nHole] = Cp;
					nHole++;
					break;
				}

				nGoto++;
			}

			if((Buff.Find(_T("FEDRAT"),0)==0) && (nHole>=3)) goto line_count_start;

			nBytesRead=pArchFile->ReadString(Buff);
		}

line_count_start:

		//Find number of Taught Lines
		pbuttonHowManyLines->SetCheck(1);
		m_nScanLineL=0;
		nBytesRead=pArchFile->ReadString(Buff);
		nTotalBytesRead=Buff.GetLength();
		nLastLine = 0;
		nThisLine = 0;
		while(nBytesRead) {
			nThisLine=0;
			if(Buff.Find(_T("GOTO"),0)==0) {
				nThisLine = _GOTO;
				nPointCount++;
			}
			if(Buff.Find(_T("RAPID"),0)==0) nThisLine = _RAPID;
		//	if(Buff.Find(_T("FEDRAT"),0)==0) goto line_count_finished;

			if( (nLastLine == _RAPID) && (nThisLine == _GOTO) )  {
					m_nScanLineL++;
			}

			nBytesRead=pArchFile->ReadString(Buff);
			nTotalBytesRead+=Buff.GetLength();

			nPos = (int)MulDiv(nTotalBytesRead,(ULONGLONG)100,nFileLength);
			if(nPos-nOldPos)
				pProgress->SetPos(nOldPos = nPos);

			nLastLine = nThisLine;
		}


		pArchFile->Close();
		delete pArchFile;

		if(m_nScanLineL<=0) {
			file.Close();
			return;
		}

		delete [] m_ScanLine;
		m_ScanLine = new CPolyCoord[m_nScanLineL];

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);

		//read in coords and make lines
		pbuttonReadCoordinates->SetCheck(1);
		nLine=0;
		int nFEDRAT=0;
		nBytesRead=pArchFile->ReadString(Buff);
		while(nBytesRead) {
			if((Buff.Find(_T("GOTO"),0)==0) && (nFEDRAT>1)) {
				nCoord=0;

				nPos = MulDiv(nLine,100,m_nScanLineL);
				if(nPos-nOldPos)
					pProgress->SetPos(nOldPos = nPos);

				while(Buff.Find(_T("GOTO"),0)==0){

					DecodeGotoLine(Buff,&Cp);

					if(nLine < m_nScanLineL) m_ScanLine[nLine] + (Cp);
					nBytesRead=pArchFile->ReadString(Buff);

					while(Buff.Find(_T("FEDRAT"),0)==0) {
						nBytesRead=pArchFile->ReadString(Buff);
						goto import_drilling;
					}

				}

				nLine++;
			} else {
				if(Buff.Find(_T("FEDRAT"),0)==0) {
					nFEDRAT++;
				}
				nBytesRead=pArchFile->ReadString(Buff);
			}
		}


		pArchFile->Close();
		delete pArchFile;

		file.Close();

		if(theApp.m_LastSettings.nImportAptDirection == 1) {
			ReverseLines(SCANLINES,2);
		}

		if(theApp.m_LastSettings.nReverseLineOrder == 1) {
			ReverseScanLineOrder();
		}

		m_bModified = TRUE;
		if(theApp.m_LastSettings.nPlaceComponentInScanner) {
			if(nHole>0) {
				GetTheProfileInTheScanner(0);
			} else {
				m_ScanLine[0].GetCoord(0,&Cp);
				GetTheProfileInTheScanner(1,&Cp);
			}
		}



	}
}

///////////////////////////////////////////////////////////
// CTRM Format
// Tripple RAPID
// Scan Starts with line "PPRINT  Start generation of : Multi-Axis Sweeping"
// Holes start with the word Drilling
//////////////////////////////////////////////////////////
void CProfile::ImportAptSourceCTRMFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType)
{
	
	CUSLFile file;
	CArchive* pArchFile;
	CString	Buff;
	ULONGLONG	nTotalBytesRead;
	int nBytesRead;
	COORDDATA	Cp;
	Cp.Zero();
	int nPos,nOldPos=0,nLine;

	if(!file.Open(FileName,CFile::modeRead|CFile::typeBinary)) {

	} else {
		ULONGLONG nFileLength = file.GetLength();
		pArchFile = new CArchive(&file,CArchive::load);
		Zero();
		//Find start of scan path
		m_nScanLineL=0;
		nBytesRead=pArchFile->ReadString(Buff);
		nTotalBytesRead=Buff.GetLength();
		while(nBytesRead) {
			if(Buff.Find(_T("PPRINT  Start generation of : Multi-Axis Sweeping"),0)==0) {


				//Find number of scan Lines
				pbuttonHowManyLines->SetCheck(1);
				m_nScanLineL=0;
				nBytesRead=pArchFile->ReadString(Buff);
				nTotalBytesRead=Buff.GetLength();
				while(nBytesRead) {
					if(Buff.Find(_T("RAPID"),0)==0) {
						m_nScanLineL++;
					}
					nBytesRead=pArchFile->ReadString(Buff);
					nTotalBytesRead+=Buff.GetLength();

					nPos = (int)MulDiv(nTotalBytesRead,(ULONGLONG)100,nFileLength);
					if(nPos-nOldPos)
						pProgress->SetPos(nOldPos = nPos);
				}
				m_nScanLineL/=3;	//because this is a double rapid
			} else {
				nBytesRead=pArchFile->ReadString(Buff);
				nTotalBytesRead+=Buff.GetLength();
			}
		}

		pArchFile->Close();
		delete pArchFile;

		if(m_nScanLineL<=0) {
			file.Close();
			return;
		}


		SAFE_DELETE_ARRAY( m_ScanLine );
		m_ScanLine = new CPolyCoord[m_nScanLineL];

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);

		//Find start of scan path
		nLine = 0;
		nBytesRead=pArchFile->ReadString(Buff);
		nTotalBytesRead=Buff.GetLength();
		while(nBytesRead) {
			if(Buff.Find(_T("PPRINT  Start generation of : Multi-Axis Sweeping"),0)==0) {
				while(nBytesRead) {
find_next_line:
					if(Buff.Find(_T("RAPID"),0)==0) {
						nPos = MulDiv(nLine,100,m_nScanLineL);
						if(nPos-nOldPos)
							pProgress->SetPos(nOldPos = nPos);

						while(nBytesRead=pArchFile->ReadString(Buff)) {
							if(Buff.Find(_T("RAPID"),0)==0) {
								while(nBytesRead=pArchFile->ReadString(Buff)) {
									if(Buff.Find(_T("RAPID"),0)==0) {
										while(nBytesRead=pArchFile->ReadString(Buff)) {
											if(Buff.Find(_T("RAPID"),0)==0) {
												nLine++;
												goto find_next_line;
											}
										}
									}
								}
							}
							if(Buff.Find(_T("GOTO"),0)==0) {
								DecodeGotoLine(Buff,&Cp);
								if(nLine<m_nScanLineL)
									m_ScanLine[nLine] + (Cp);
							}
						}
					}
					nBytesRead=pArchFile->ReadString(Buff);
				}
			} else {
				nBytesRead=pArchFile->ReadString(Buff);
				nTotalBytesRead+=Buff.GetLength();
			}
		}

		pArchFile->Close();
		delete pArchFile;

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);

		//read in hole coordinates
		pbuttonReadingHoles->SetCheck(1);
		pProgress->SetPos(0);

		int	nHole = 0;
		nBytesRead=pArchFile->ReadString(Buff);

		while((nBytesRead>0) && (Buff.Find(_T("PPRINT  Start generation of : Drilling.1"),0)!=0)){
			nBytesRead=pArchFile->ReadString(Buff);
		}
		while((nBytesRead>0) && (nHole<3)) {
			nBytesRead=pArchFile->ReadString(Buff);
			if(Buff.Find(_T("GOTO"),0)==0) {
				pProgress->SetPos(MulDiv(nHole+1,100,3));
				ZeroMemory(&Cp,sizeof Cp);

				DecodeGotoLine(Buff,&Cp);

				if(nHole<3) {
					m_CpCadHole[nHole] = Cp;
					nHole++;
				}
			}
		}

		pArchFile->Close();
		delete pArchFile;

		file.Close();

		if(theApp.m_LastSettings.nImportAptDirection == 1) {
			ReverseLines(SCANLINES,2);
		}

		if(theApp.m_LastSettings.nReverseLineOrder == 1) {
			ReverseScanLineOrder();
		}

		m_bModified = TRUE;
		if(theApp.m_LastSettings.nPlaceComponentInScanner) {
			if(nHole>0) {
				GetTheProfileInTheScanner(0);
			} else {
				m_ScanLine[0].GetCoord(0,&Cp);
				GetTheProfileInTheScanner(1,&Cp);
			}
		}

	}
}

void CProfile::ModifyCoordWp(int nLineType, int nLine0, int nLine1,int nLeftDo, float fWpLeft0, float fWpLeft1, int nRightDo, float fWpRight0, float fWpRight1)
{
	int nLine;
	float fWp;

	switch(nLineType) {
	case SCANLINES:
		if(nLine0<0) return;
		if(nLine1>=m_nScanLineL) return;
		if(m_ScanLine==NULL) return;

		for(nLine=nLine0;nLine<=nLine1;nLine++) {
			if(nLeftDo == 1) {
				fWp = ((fWpLeft1 - fWpLeft0) * (float)(nLine-nLine0)) / (float)(nLine1-nLine0) + fWpLeft0;
				m_ScanLine[nLine].SetAllWp(PORTSIDE,fWp);
			}
			if(nRightDo == 1) {
				fWp = ((fWpRight1 - fWpRight0) * (float)(nLine-nLine0)) / (float)(nLine1-nLine0) + fWpRight0;
				m_ScanLine[nLine].SetAllWp(STARBOARD,fWp);
			}
		}
		break;
	}
}

void CProfile::InvertAllNormals()
{
	int nLine;

	if((m_TaughtLine!=NULL) || (m_nTaughtLineL>0)) {
		for(nLine = 0; nLine<m_nTaughtLineL ; nLine++) {
			m_TaughtLine[nLine].InvertAllNormals();
		}
	}
	if((m_ScanLine!=NULL) || (m_nScanLineL>0)) {
		for(nLine = 0; nLine<m_nScanLineL ; nLine++) {
			m_ScanLine[nLine].InvertAllNormals();
		}
	}
	if((m_BrainLine!=NULL) || (m_nBrainLineL>0)) {
		for(nLine = 0; nLine<m_nBrainLineL ; nLine++) {
			m_BrainLine[nLine].InvertAllNormals();
		}
	}
	m_bModified = TRUE;
}

void CProfile::InvertNormals(bool bTaught, bool bScan, bool bBrain, bool bCAD, bool bI, bool bJ, bool bK)
{
	int nLine;

	if(bTaught) {
		if((m_TaughtLine!=NULL) || (m_nTaughtLineL>0)) {
			for(nLine = 0; nLine<m_nTaughtLineL ; nLine++) {
				if(bI) m_TaughtLine[nLine].InvertINormal();
				if(bJ) m_TaughtLine[nLine].InvertJNormal();
				if(bK) m_TaughtLine[nLine].InvertKNormal();
			}
		}
	}


	if(bScan) {
		if((m_ScanLine!=NULL) || (m_nScanLineL>0)) {
			for(nLine = 0; nLine<m_nScanLineL ; nLine++) {
				if(bI) m_ScanLine[nLine].InvertINormal();
				if(bJ) m_ScanLine[nLine].InvertJNormal();
				if(bK) m_ScanLine[nLine].InvertKNormal();
			}
		}
	}

	if(bBrain) {
		if((m_BrainLine!=NULL) || (m_nBrainLineL>0)) {
			for(nLine = 0; nLine<m_nBrainLineL ; nLine++) {
				if(bI) m_BrainLine[nLine].InvertINormal();
				if(bJ) m_BrainLine[nLine].InvertJNormal();
				if(bK) m_BrainLine[nLine].InvertKNormal();
			}
		}
	}

	if(bCAD) {
		for(int nn=0;nn<3;nn++) {
			if(bI) {
				m_CpCadHole[nn].Side0.fI *= -1;
				m_CpCadHole[nn].Side1.fI *= -1;
			}
			if(bJ) {
				m_CpCadHole[nn].Side0.fJ *= -1;
				m_CpCadHole[nn].Side1.fJ *= -1;
			}
			if(bK) {
				m_CpCadHole[nn].Side0.fJ *= -1;
				m_CpCadHole[nn].Side1.fJ *= -1;
			}
		}
	
	}
	m_bModified = TRUE;
}

void CProfile::ChangeSignOfAnElement(char cWhich)
{
	int nLine;

	if((m_TaughtLine!=NULL) || (m_nTaughtLineL>0)) {
		for(nLine = 0; nLine<m_nTaughtLineL ; nLine++) {
			m_TaughtLine[nLine].ChangeSignOfAnElement(cWhich);
		}
	}
	if((m_ScanLine!=NULL) || (m_nScanLineL>0)) {
		for(nLine = 0; nLine<m_nScanLineL ; nLine++) {
			m_ScanLine[nLine].ChangeSignOfAnElement(cWhich);
		}
	}
	if((m_BrainLine!=NULL) || (m_nBrainLineL>0)) {
		for(nLine = 0; nLine<m_nBrainLineL ; nLine++) {
			m_BrainLine[nLine].ChangeSignOfAnElement(cWhich);
		}
	}

	for(int ii=0;ii<3;ii++)  {
		switch(cWhich) {
		case 'i': m_CpCadHole[ii].Side0.fI *= -1;
			break;
		case 'j': m_CpCadHole[ii].Side0.fJ *= -1;
			break;
		case 'k': m_CpCadHole[ii].Side0.fK *= -1;
			break;
		}
	}




	m_bModified = TRUE;
}

void CProfile::SetLineIncrement(CPolyCoord *pLine)
{
	CCoord Cp,Cpk;

	pLine->GetCoord(0,&Cp);

	int klo=0;
	int khi=m_nTaughtLineL-1;
	int k;

	while((khi-klo)>1) {
		k=(khi+klo)/2;
		m_TaughtLine[k].GetCoord(0,&Cpk);
		(Cpk.fPos[m_nSlowAxis]>Cp.fPos[m_nSlowAxis])?(khi=k):(klo=k);
	};
	pLine->m_nLineIncrement = m_TaughtLine[klo].m_nLineIncrement;



}

int CProfile::FindNearestScanLine(CCoord Cp)
{
	CCoord Cpk;
	int klo=0;
	int khi=m_nScanLineL-1;
	int k;

	while((khi-klo)>1) {
		k=(khi+klo)/2;
		m_ScanLine[k].GetCoord(0,&Cpk);
		(Cpk.fPos[m_nSlowAxis]>Cp.fPos[m_nSlowAxis])?(khi=k):(klo=k);
	};
	return klo;

}


void CProfile::ImportAptSourceGoodrichFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType)
{
	
	CUSLFile file;
	CArchive* pArchFile;
	CString	Buff;
	ULONGLONG	nTotalBytesRead;
	COORDDATA	Cp;
	Cp.Zero();
	int nPos,nOldPos=0;

	if(!file.Open(FileName,CFile::modeRead|CFile::typeBinary)) {

	} else {
		ULONGLONG nFileLength = file.GetLength();
		pArchFile = new CArchive(&file,CArchive::load);
		Zero();


		nTotalBytesRead=0;
		//Find number of Taught Lines
		pbuttonHowManyLines->SetCheck(1);
		m_nTaughtLineL=1;
		m_TaughtLine = new CPolyCoord[m_nTaughtLineL];
		m_nScanLineL=1;
		m_ScanLine = new CPolyCoord[m_nScanLineL];

		while(pArchFile->ReadString(Buff)) {
			if(Buff.Find(_T("GOTO"),0)==0) {
				DecodeGotoLine(Buff,&Cp);
				m_TaughtLine[0] + (Cp);
				m_ScanLine[0] + (Cp);
			}
//			if(Buff.Find(_T("$$$"),0)==0) nThisLine = _$$$;
//			if(Buff.Find(_T("Drilling"),0) > 5) goto line_count_finished;

			nTotalBytesRead+=Buff.GetLength();
			nPos = (int)MulDiv(nTotalBytesRead,(ULONGLONG)100,nFileLength);
			if(nPos-nOldPos)
				pProgress->SetPos(nOldPos = nPos);

		}

		pArchFile->Close();
		delete pArchFile;
		file.Close();
	}
}

void CProfile::SwapLines(int nLine0, int nLine1, int nType)
{
	CPolyCoord Line;

	Line.Zero();

	Line = &m_TaughtLine[nLine0];
	m_TaughtLine[nLine0].Zero();
	m_TaughtLine[nLine0] = &m_TaughtLine[nLine1];
	m_TaughtLine[nLine1].Zero();
	m_TaughtLine[nLine1] = &Line;
}

void CProfile::AvoidExclusionZones(CCoord *pCp)
{
	

	if(m_bExclusionZones[0]) {
		if((m_CpExclusion[0][0].Side[theApp.m_nPrimarySide].fZ>=pCp->Side[theApp.m_nPrimarySide].fZ) && (pCp->Side[theApp.m_nPrimarySide].fZ >= m_CpExclusion[0][1].Side[theApp.m_nPrimarySide].fZ)) {
			if(pCp->Side0.fX < m_CpExclusion[0][1].Side[theApp.m_nPrimarySide].fX) {
				pCp->Side0.fX = pCp->Side1.fX = m_CpExclusion[0][1].Side[theApp.m_nPrimarySide].fX;
			}
		}
	}
	if(m_bExclusionZones[1]) {
		if((m_CpExclusion[1][0].Side[theApp.m_nPrimarySide].fZ>=pCp->Side[theApp.m_nPrimarySide].fZ) && (pCp->Side[theApp.m_nPrimarySide].fZ >= m_CpExclusion[1][1].Side[theApp.m_nPrimarySide].fZ)) {
			if(pCp->Side0.fX > m_CpExclusion[1][0].Side[theApp.m_nPrimarySide].fX) {
				pCp->Side0.fX = pCp->Side1.fX = m_CpExclusion[1][0].Side[theApp.m_nPrimarySide].fX;
			}
		}
	}

}

void CProfile::ImportAptSourceGoodrichMachine4(CString FileName)
{
	
	CUSLFile file;
	CArchive* pArchFile;
	CString	Buff;
	int	nn,ii,jj,nLength,nPtr;
	char	str[30];
	char	cV;
	int	nCoordL=0;
	COORDDATA Cp;

	if(!file.Open(FileName,CFile::modeRead|CFile::typeBinary)) {

	} else {
		ULONGLONG nFileLength = file.GetLength();
		pArchFile = new CArchive(&file,CArchive::load);
		Zero();
		while(pArchFile->ReadString(Buff)) {
			if(Buff.Find(_T("GOTO"),0) != -1) nCoordL++;
		}
		pArchFile->Close();
		delete pArchFile;

		m_TaughtLine = new CPolyCoord[m_nTaughtLineL = 1];

		m_TaughtLine[0].AllocateCoords(nCoordL);


		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);
		nPtr=0;
		while(pArchFile->ReadString(Buff)) {
			if(((nn=Buff.Find(_T("GOTO"),0)) != -1) && (nPtr<nCoordL)){
				nLength=Buff.GetLength();
				Cp.Zero();

				for(ii=7,jj=0;(cV=(char)Buff.GetAt(ii))!=',';ii++,jj++) str[jj]=cV;
				str[jj]=0;
				Cp.Side0.fX=(float)atof(str);

				for(++ii,jj=0;(cV=(char)Buff.GetAt(ii))!=',';ii++,jj++) str[jj]=cV;
				str[jj]=0;
				Cp.Side0.fY=(float)atof(str);

				for(++ii,jj=0;(cV=(char)Buff.GetAt(ii))!=',';ii++,jj++) str[jj]=cV;
				str[jj]=0;
				Cp.Side0.fZ=(float)atof(str);

				for(++ii,jj=0;(cV=(char)Buff.GetAt(ii))!=',';ii++,jj++) str[jj]=cV;
				str[jj]=0;
				Cp.Side0.fI=(float)atof(str);

				for(++ii,jj=0;(cV=(char)Buff.GetAt(ii))!=',';ii++,jj++) str[jj]=cV;
				str[jj]=0;
				Cp.Side0.fJ=(float)atof(str);

				for(++ii,jj=0;(ii<nLength) && (cV=(char)Buff.GetAt(ii));ii++,jj++) str[jj]=cV;
				str[jj]=0;
				Cp.Side0.fK=(float)atof(str);

				if(theApp.m_LastSettings.nImportAptUnits==1) {
					Cp.Side0.fX *= MMPERINCH;
					Cp.Side0.fY *= MMPERINCH;
					Cp.Side0.fZ *= MMPERINCH;
				}

				Cp.CopySides(1,0);
				Cp.InvertNorm(STARBOARD);

				m_TaughtLine[0].m_pCp[nPtr++] = Cp;

				nCoordL++;
			}
		}


		pArchFile->Close();
		file.Close();
		delete pArchFile;

		nCoordL = m_TaughtLine[0].m_nCoordL;

		//check to ensure correct order of coords. Cant use sort because some Z values may be identical
/*
		if(m_TaughtLine[0].m_pCp[0].Side0.fZ > m_TaughtLine[0].m_pCp[nCoordL-1].Side0.fZ) {

			for(ii=0,jj=nCoordL-1;ii<(nCoordL/2);ii++,jj--) {
				Cp=m_TaughtLine[0].m_pCp[ii];
				m_TaughtLine[0].m_pCp[ii] = m_TaughtLine[0].m_pCp[jj];
				m_TaughtLine[0].m_pCp[jj] = Cp;
			}
		}
*/
		m_bModified = TRUE;

	}
}




void CProfile::FindScanLineSampleSlowVersion(CCoord Cp, int *pnSample, int *pnLine)
{
	int kHi,kLo,k;
	int nSegment;
	int nSide = 0;

	kHi=m_nScanLineL-1;
	kLo=0;

	float fLengthLo = m_ScanLine[kLo].FindNearestSegment(Cp,nSide,&nSegment);
	float fLengthHi = m_ScanLine[kHi].FindNearestSegment(Cp,nSide,&nSegment);
	while((kHi-kLo)>1) {
		k=(kHi+kLo)/2;
		if(fLengthHi > fLengthLo) {
			if((kHi-k) > 2) {
				kHi = (kHi+k)/2;
			} else {
				kHi=k;
			}
			*pnLine=kHi;
			fLengthHi = m_ScanLine[kHi].FindNearestSegment(Cp,nSide,&nSegment);
		} else {
			if((k-kLo) > 2) {
				kLo = (k+kLo)/2;
			} else {
				kLo=k;
			}
			*pnLine=kLo;
			fLengthLo = m_ScanLine[kLo].FindNearestSegment(Cp,nSide,&nSegment);
		}
	}
	fLengthLo > fLengthHi ? *pnLine = kLo : *pnLine = kHi; 

}

void CProfile::FindScanLineSample(CCoord Cp, int *pnSample, int *pnLine)
{
	//cant use klo khi because of nature of slopped edges
	int nSide = 0;
	int	nSegment;
	float fShortestDistance=1000000.0f;
	float fLength;
	CCoord CpFirst;
	float fAngle;
	float fShortestAngle=720.0f;
	*pnLine=0;

	int nStep = 1;

	if((m_nSlowAxis != 5) && (IsRUsed() == false)) {
		for(int nLine =0;nLine<m_nScanLineL;nLine+=nStep) {
			fLength = m_ScanLine[nLine].FindNearestSegment(Cp,nSide,&nSegment);
			if(fLength<fShortestDistance) {
				fShortestDistance=fLength;
				*pnLine = nLine;
//				nStep = (int)(fLength / m_fSlowIncrement);  // Look for this in the future
//				if(nStep<1) nStep = 1;
			} else {
				return;
			}
		}
	} else {
		for(int nLine =0;nLine<m_nScanLineL;nLine++) {
			m_ScanLine[nLine].GetFirstCoord(&CpFirst);
			fAngle = fabsf(Cp.Side[0].fR - CpFirst.Side0.fR);
			if(fShortestAngle > fAngle) {
				fShortestAngle = fAngle;
				*pnLine = nLine;
			};
		}
		if(m_ScanLine) {
			fLength = m_ScanLine[*pnLine].FindNearestSegment(Cp,nSide,&nSegment);
		}
	}
}


///////////////////////////////////////////////////////////
// Hafei Format
// Single RAPID
// Holes start with the word HOLES
//////////////////////////////////////////////////////////
void CProfile::ImportAptSourceXifeiFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType)
{
	
	CUSLFile file;
	CArchive* pArchFile;
	CString	Buff;
	ULONGLONG	nTotalBytesRead;
	int nCoord;
	int nBytesRead;
	COORDDATA	Cp;
	Cp.Zero();
	int nPos,nOldPos=0;

	if(!file.Open(FileName,CFile::modeRead|CFile::typeBinary)) {

	} else {
		ULONGLONG nFileLength = file.GetLength();
		pArchFile = new CArchive(&file,CArchive::load);
		Zero();


		//Find number of scan Lines
		pbuttonHowManyLines->SetCheck(1);
		m_nScanLineL=0;
		nBytesRead=pArchFile->ReadString(Buff);
		nTotalBytesRead=Buff.GetLength();
		while(nBytesRead) {
			if(Buff.Find(_T("RAPID"),0)==0) {
				m_nScanLineL++;
			}
			nBytesRead=pArchFile->ReadString(Buff);
			nTotalBytesRead+=Buff.GetLength();

			nPos = (int)MulDiv(nTotalBytesRead,(ULONGLONG)100,nFileLength);
			if(nPos-nOldPos)
				pProgress->SetPos(nOldPos = nPos);
		}
		m_nScanLineL--;  //this is here because there is an extra RAPID

		pArchFile->Close();
		delete pArchFile;

		if(m_nScanLineL<=0) {
			file.Close();
			return;
		}

		delete [] m_ScanLine;
		m_ScanLine = new CPolyCoord[m_nScanLineL];

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);

		//read in coords and make lines
		pbuttonReadCoordinates->SetCheck(1);
		int nLine=0;
		nBytesRead=pArchFile->ReadString(Buff);
		while(nBytesRead) {
			if(Buff.Find(_T("RAPID"),0)==0) {
				nCoord=0;

				nPos = MulDiv(nLine,100,m_nScanLineL);
				if(nPos-nOldPos)
					pProgress->SetPos(nOldPos = nPos);

				nBytesRead=pArchFile->ReadString(Buff);
				while(Buff.Find(_T("GOTO"),0)==0) {

					DecodeGotoLine(Buff,&Cp);

					m_ScanLine[nLine] + (Cp);
					nBytesRead=pArchFile->ReadString(Buff);
				}
				nLine++;
			} else {
				nBytesRead=pArchFile->ReadString(Buff);
			}
		}

		pArchFile->Close();
		delete pArchFile;

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);

		//read in hole coordinates
		pbuttonReadingHoles->SetCheck(1);
		pProgress->SetPos(0);

		int	nHole = 0;
		nBytesRead=pArchFile->ReadString(Buff);

		while((nBytesRead>0) && (Buff.Find(_T("HOLES"),0)!=0)){
			nBytesRead=pArchFile->ReadString(Buff);
		}
		while(nBytesRead>0) {
			nBytesRead=pArchFile->ReadString(Buff);
			if(Buff.Find(_T("GOTO"),0)==0) {
				pProgress->SetPos(MulDiv(nHole+1,100,3));
				ZeroMemory(&Cp,sizeof Cp);

				DecodeGotoLine(Buff,&Cp);

				if(nHole<3) {
					m_CpCadHole[nHole] = Cp;
					nHole++;
				}
			}
		}

		pArchFile->Close();
		delete pArchFile;

		file.Close();

		if(theApp.m_LastSettings.nImportAptDirection == 1) {
			ReverseLines(SCANLINES,2);
		}

		if(theApp.m_LastSettings.nReverseLineOrder == 1) {
			ReverseScanLineOrder();
		}

		m_bModified = TRUE;
		if(theApp.m_LastSettings.nPlaceComponentInScanner) {
			if(nHole>0) {
				GetTheProfileInTheScanner(0);
			} else {
				m_ScanLine[0].GetCoord(0,&Cp);
				GetTheProfileInTheScanner(1,&Cp);
			}
		}

	}
}




void CProfile::CopyGenericToCCoord(CCoord &CpSurface, CoordStructGeneric Cp)
{
	CpSurface.Zero();

	CpSurface.Side0.fX = (float)Cp.dX0p;
	CpSurface.Side0.fY = (float)Cp.dY0p;
	CpSurface.Side0.fZ = (float)Cp.dZ0p;
	CpSurface.Side0.fXt = (float)Cp.dXt0p;
	CpSurface.Side0.fYt = (float)Cp.dYt0p;
	CpSurface.Side0.fR = (float)Cp.dR0p;

	CpSurface.Side1.fX = (float)Cp.dX1p;
	CpSurface.Side1.fY = (float)Cp.dY1p;
	CpSurface.Side1.fZ = (float)Cp.dZ1p;
	CpSurface.Side1.fXt = (float)Cp.dXt1p;
	CpSurface.Side1.fYt = (float)Cp.dYt1p;

}

void CProfile::ImportAptSourceDowtyFormat(CString FileName,CProgressCtrl* pProgress,CButton* pbuttonHowManyLines,CButton* pbuttonReadCoordinates,CButton* pbuttonReadingHoles,int nHoleType)
{
	
	CUSLFile file;
	CArchive* pArchFile;
	CString	Buff;
	ULONGLONG	nTotalBytesRead;
	int nCoord;
	int nBytesRead;
	COORDDATA	Cp;
	Cp.Zero();
	int nPos,nOldPos=0;

	if(!file.Open(FileName,CFile::modeRead|CFile::typeBinary)) {

	} else {
		ULONGLONG nFileLength = file.GetLength();
		pArchFile = new CArchive(&file,CArchive::load);
		Zero();


		//Find number of scan Lines
		pbuttonHowManyLines->SetCheck(1);
		m_nScanLineL=0;
		nBytesRead=pArchFile->ReadString(Buff);
		nTotalBytesRead=Buff.GetLength();
		while(nBytesRead) {
			nBytesRead=pArchFile->ReadString(Buff);
			nTotalBytesRead+=Buff.GetLength();;
			
			if(Buff.Find(_T("RAPID"),0)==0) {
				m_nScanLineL++;
			}
			
			nPos = (int)MulDiv(nTotalBytesRead,(ULONGLONG)100,nFileLength);
			if(nPos-nOldPos)
				pProgress->SetPos(nOldPos = nPos);
		}

		pArchFile->Close();
		delete pArchFile;

		if(m_nScanLineL<=0) {
			file.Close();
			return;
		}

		delete [] m_ScanLine;
		m_ScanLine = new CPolyCoord[m_nScanLineL];

		file.Seek(0,CFile::begin);
		pArchFile = new CArchive(&file,CArchive::load);

		//read in coords and make lines
		pbuttonReadCoordinates->SetCheck(1);
		int nLine=0;
		nBytesRead=pArchFile->ReadString(Buff);
		while(nBytesRead) {
			if(Buff.Find(_T("RAPID"),0)==0) {
				nCoord=0;

				nPos = MulDiv(nLine,100,m_nScanLineL);
				if(nPos-nOldPos)
					pProgress->SetPos(nOldPos = nPos);

				nBytesRead=pArchFile->ReadString(Buff);
				while(Buff.Find(_T("Goto"),0)==0) {

					DecodeGotoLine(Buff,&Cp);

					m_ScanLine[nLine] + (Cp);
					nBytesRead=pArchFile->ReadString(Buff);
				}
				nLine++;
			} else {
				nBytesRead=pArchFile->ReadString(Buff);
			}
		}

		pArchFile->Close();
		delete pArchFile;

		if(theApp.m_LastSettings.nImportAptDirection == 1) {
			ReverseLines(SCANLINES,2);
		}

		if(theApp.m_LastSettings.nReverseLineOrder == 1) {
			ReverseScanLineOrder();
		}

		m_bModified = TRUE;
	
	}
}

void CProfile::SetToolPathFromLine(CPolyCoord *pLine)
{
	CCoord CpSurface,CpNext,CpCurrent;

	m_ToolPath.Zero();

	theApp.m_Motors.GetSurfacePos(&CpCurrent);

	for(int ii=0;ii<pLine->m_nCoordL;ii++) {

		pLine->GetCoord(ii, &CpSurface);

		theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
		theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);

		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,PORTSIDE,TRUE,TRUE,&CpCurrent, 0);
		theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpNext,STARBOARD,TRUE,TRUE,&CpCurrent, 0);

		m_ToolPath.Add(CpNext);
	}
	m_ToolPath.Sort(m_nFastAxis);

}

void CProfile::GenerateFlatPackMesh()
{
	
	int nNumberLines = (int)theApp.m_LastSettings.fMeshSlowSize;
	int nSamplesLine = (int)theApp.m_LastSettings.fMeshFastSize;

	CCoord Cp,Cp0,Cp1,CpStart0,CpStart1,CpFinish0,CpFinish1;
	int nSlowAxis = 1;
	int	nFastAxis = 0;
	int nLine,nSample;

	if(m_nTaughtLineL<=0) return;

	CPolyCoord *pLine = new CPolyCoord[nNumberLines];

	pLine[0] = &m_TaughtLine[0];
	pLine[nNumberLines-1] = &m_TaughtLine[m_nTaughtLineL-1];

	m_TaughtLine[0].GetCoord(0,&CpStart0);
	m_TaughtLine[m_nTaughtLineL-1].GetCoord(0,&CpStart1);
	m_TaughtLine[0].GetLastCoord(&CpFinish0);
	m_TaughtLine[m_nTaughtLineL-1].GetLastCoord(&CpFinish1);

	for(nLine = 1;nLine<(nNumberLines-1);nLine++) {

		for(nSample=0;nSample<nSamplesLine;nSample++) {
			Cp0.fPos[nFastAxis] = ((float)nSample * (CpFinish0.fPos[nFastAxis] - CpStart0.fPos[nFastAxis]) / (float)(nSamplesLine-1)) + CpStart0.fPos[nFastAxis];
			Cp1.fPos[nFastAxis] = ((float)nSample * (CpFinish1.fPos[nFastAxis] - CpStart1.fPos[nFastAxis]) / (float)(nSamplesLine-1)) + CpStart1.fPos[nFastAxis];
			Cp.fPos[nFastAxis] = ((float)nLine * (Cp1.fPos[nFastAxis] - Cp0.fPos[nFastAxis]) / (float)(nNumberLines-1)) + Cp0.fPos[nFastAxis];

			Cp0.fPos[nSlowAxis] = ((float)nSample * (CpFinish0.fPos[nSlowAxis] - CpStart0.fPos[nSlowAxis]) / (float)(nSamplesLine-1)) + CpStart0.fPos[nSlowAxis];
			Cp1.fPos[nSlowAxis] = ((float)nSample * (CpFinish1.fPos[nSlowAxis] - CpStart1.fPos[nSlowAxis]) / (float)(nSamplesLine-1)) + CpStart1.fPos[nSlowAxis];
			Cp.fPos[nSlowAxis] = ((float)nLine * (Cp1.fPos[nSlowAxis] - Cp0.fPos[nSlowAxis]) / (float)(nNumberLines-1)) + Cp0.fPos[nSlowAxis];

			CalculateCoord(nFastAxis,nSlowAxis,Cp.fPos[nFastAxis],Cp.fPos[nSlowAxis],Cp);

			pLine[nLine].Add(Cp);
		}
	}

	SAFE_DELETE_ARRAY( m_TaughtLine );
	m_TaughtLine = new CPolyCoord[m_nTaughtLineL = nNumberLines];
	for(nLine = 0;nLine < m_nTaughtLineL;nLine++) {
		m_TaughtLine[nLine] = &pLine[nLine];
	}


	SAFE_DELETE_ARRAY( pLine );
}


bool CProfile::ImportDowtyProfile(CUSLFile *pFile)
{
	return true;
}

void CProfile::SetAllNormals(int nLineType, D3DXVECTOR4 norm)
{
	int	nLine;

	switch(nLineType) {
	case TAUGHTLINES:
		for(nLine = 0;nLine<m_nTaughtLineL;nLine++) {
			m_TaughtLine[nLine].SetAllNorms(PORTSIDE,norm);
		}
		break;
	case SCANLINES:
		for(nLine = 0;nLine<m_nScanLineL;nLine++) {
			m_ScanLine[nLine].SetAllNorms(PORTSIDE,norm);
		}
		break;
	}


}



void CProfile::ProfileDataValidationCheck()
{
	
	int nLine=0;

	//Check all lines to see if any have no points in, if found delete
	for(nLine=0;nLine<m_nTaughtLineL;nLine++) {
		if(m_TaughtLine[nLine].m_nCoordL<=0) {
			DeleteLine(TAUGHTLINES,nLine);
			nLine--;
		}
	}

	for(nLine=0;nLine<m_nScanLineL;nLine++) {
		if(m_ScanLine[nLine].m_nCoordL<=0) {
			DeleteLine(SCANLINES,nLine);
			nLine--;
		}
	}

	for(nLine=0;nLine<m_nBrainLineL;nLine++) {
		if(m_BrainLine[nLine].m_nCoordL<=0) {
			DeleteLine(BRAINLINES,nLine);
			nLine--;
		}
	}

	for(nLine=0;nLine<m_nEnvelopeLineL;nLine++) {
		if(m_EnvelopeLine[nLine].m_nCoordL<=0) {
			DeleteLine(ENVELOPELINES,nLine);
			nLine--;
		}
	}

	if((!theApp.m_Tank.bTurnTable) && ((m_nComponentDescription!=CLOSED_ROTARY) || (m_nComponentDescription!=OPEN_ROTARY) || (m_nComponentDescription!=CIRCULAR))){
		m_nComponentDescription=RECTILINEAR;
	}

	//Sort all lines
//	SortEachLine(TAUGHTLINES,m_nFastAxis);
//	SortEachLine(SCANLINES,m_nFastAxis);
//	SortEachLine(BRAINLINES,m_nFastAxis);
}

void CProfile::DivideScanLineInTwo(int nLine, int nIndex)
{
	CPolyCoord MasterLine;

	MasterLine = &m_ScanLine[nLine];
	m_ScanLine[nLine].DeleteCoords(nIndex+1,m_ScanLine[nLine].m_nCoordL-1);


	MasterLine.DeleteCoords(0,nIndex);
	AddLine(SCANLINES,&MasterLine,m_nSlowAxis);

}

void CProfile::setRUsed(bool bState)
{
	switch(bState) {
	case false: m_nRUsed = 0;
		break;
	case true: m_nRUsed = 1;
		break;
	}
}


bool CProfile::IsRUsed()
{
	
	bool bRetval = false;

	if( (m_nSlowAxis==5)||(m_nSlowAxis==theApp.m_Tank.nRLeft) ) {
		if(theApp.m_Axes[theApp.m_Tank.nRLeft].bAvailable) {
			bRetval = true;
		}
	}

	if( (m_nSlowAxis==theApp.m_Tank.nRRight)||(m_nSlowAxis==theApp.m_Tank.nRRight) ) {
		if(theApp.m_Axes[theApp.m_Tank.nRRight].bAvailable) {
			bRetval = true;
		}
	}

	if( m_nRUsed == 1 )
		return true;

	return bRetval;


}

bool CProfile::AddCoordToLine(int nLineType,int nLine, CCoord Cp)
{
	switch(nLineType) {
	case TAUGHTLINES:
		if(m_TaughtLine == NULL) {
			m_TaughtLine = new CPolyCoord[m_nTaughtLineL=1];
			m_TaughtLine[nLine=0].Add(Cp);
		} else {
			if(nLine<m_nTaughtLineL) {
				m_TaughtLine[nLine].Add(Cp);
			}
		}
		return true;
		break;
	case SCANLINES:
		if(m_ScanLine == NULL) {
			m_ScanLine = new CPolyCoord[m_nScanLineL=1];
			m_ScanLine[nLine=0].Add(Cp);
		} else {
			if(nLine<m_nScanLineL) {
				m_ScanLine[nLine].Add(Cp);
			}
		}
		return true;
		break;
	case BRAINLINES:
		if(m_BrainLine == NULL) {
			m_BrainLine = new CPolyCoord[m_nBrainLineL=1];
			m_BrainLine[nLine=0].Add(Cp);
		} else {
			if(nLine<m_nBrainLineL) {
				m_BrainLine[nLine].Add(Cp);
			}
		}
		return true;
		break;
	case ENVELOPELINES:
		if(m_EnvelopeLine == NULL) {
			m_EnvelopeLine = new CPolyCoord[m_nEnvelopeLineL=1];
			m_EnvelopeLine[nLine=0].Add(Cp);
		} else {
			if(nLine<m_nEnvelopeLineL) {
				m_EnvelopeLine[nLine].Add(Cp);
			}
		}
		return true;
		break;
	}
	return false;

}

bool CProfile::DeleteCoordFromLine(int nLineType,int nLine, int nIndex)
{
	switch(nLineType) {
	case TAUGHTLINES:
		if(m_TaughtLine == NULL) return false;
		if(nLine>=m_nTaughtLineL) return false;
		if(m_TaughtLine[nLine].m_nCoordL == 1) {
			if(MessageBox(NULL,_T("This is the last vertex on this line. Deleteing this point will remove the entire line\r\nDo you wish to continue ?"),_T("Warning"),MB_YESNO|MB_ICONEXCLAMATION)!=IDYES) return false;
			DeleteLine(nLineType,nLine);
		} else {
			m_TaughtLine[nLine].DeleteCoords(nIndex,nIndex);
		}
		break;
	case SCANLINES:
		if(m_ScanLine == NULL) return false;
		if(nLine>=m_nScanLineL) return false;
		if(m_ScanLine[nLine].m_nCoordL == 1) {
			if(MessageBox(NULL,_T("This is the last vertex on this line. Deleteing this point will remove the entire line\r\nDo you wish to continue ?"),_T("Warning"),MB_YESNO|MB_ICONEXCLAMATION)!=IDYES) return false;
			DeleteLine(nLineType,nLine);
		} else {
			m_ScanLine[nLine].DeleteCoords(nIndex,nIndex);
		}
		break;
	case BRAINLINES:
		if(m_BrainLine == NULL) return false;
		if(nLine>=m_nBrainLineL) return false;
		if(m_BrainLine[nLine].m_nCoordL == 1) {
			if(MessageBox(NULL,_T("This is the last vertex on this line. Deleteing this point will remove the entire line\r\nDo you wish to continue ?"),_T("Warning"),MB_YESNO|MB_ICONEXCLAMATION)!=IDYES) return false;
			DeleteLine(nLineType,nLine);
		} else {
			m_BrainLine[nLine].DeleteCoords(nIndex,nIndex);
		}
		break;
	case ENVELOPELINES:
		if(m_EnvelopeLine == NULL) return false;
		if(nLine>=m_nEnvelopeLineL) return false;
		if(m_EnvelopeLine[nLine].m_nCoordL == 1) {
			if(MessageBox(NULL,_T("This is the last vertex on this line. Deleteing this point will remove the entire line\r\nDo you wish to continue ?"),_T("Warning"),MB_YESNO|MB_ICONEXCLAMATION)!=IDYES) return false;
			DeleteLine(nLineType,nLine);
		} else {
			m_EnvelopeLine[nLine].DeleteCoords(nIndex,nIndex);
		}
		break;
	}
	return true;
}

void CProfile::CopyGlobalUtToLocal()
{
	CopyMemory(&m_Global, &theApp.m_UtUser.m_Global, sizeof m_Global);
	CopyMemory(m_TS, theApp.m_UtUser.m_TS, sizeof m_TS);

}

void CProfile::CopyLocalUtToGlobal()
{
	CMainFrame *pFrame =   (CMainFrame*)AfxGetApp()->m_pMainWnd;

	CopyMemory(&theApp.m_UtUser.m_Global, &m_Global, sizeof m_Global);
	CopyMemory(theApp.m_UtUser.m_TS, m_TS, sizeof m_TS);

	theApp.m_UtUser.ResetMaxTimeSlots();
	theApp.m_UtUser.Initialize(theApp.m_nJobNumber);

	if (pFrame->m_pUltrasonicsSheet != NULL) {
		pFrame->m_pUltrasonicsSheet->SendMessage(UPDATE_TIMESLOTS);
	}

}


void CProfile::ExtrapolateScanLines(int nEnd, float fVariable)
{
	int	nLine;

	switch(theApp.m_LastSettings.nExtrapolateMode) {
	case 0:
		for(nLine = 0;nLine<m_nScanLineL;nLine++) {
			m_ScanLine[nLine].Extrapolate(nEnd,fVariable,0);
		}
		break;
	case 1:
		for(nLine = 0;nLine<m_nScanLineL;nLine++) {
			m_ScanLine[nLine].ProjectToYPlane(nEnd,fVariable);
		}
		break;
	case 2:
		for(nLine = 0;nLine<m_nScanLineL;nLine++) {
			m_ScanLine[nLine].Extrapolate(nEnd,fVariable,1);
		}
		break;
	}
}

void CProfile::AllocateSurfaceLine(int nSamplesLine)
{
	SAFE_DELETE_ARRAY( m_SurfaceLine );
	m_nSurfaceLineL = 0;

/*	if(m_nScanLineL>0) {
		m_nSurfaceLineL = m_nScanLineL;
		m_SurfaceLine = new CPolyCoord[m_nSurfaceLineL];

		for(int nLine = 0;nLine<m_nSurfaceLineL; nLine++) {
			m_SurfaceLine[nLine].AllocateCoords( nSamplesLine );
			m_SurfaceLine[nLine].ClearMemory();
		}

  }
*/
	m_nSurfaceLineL = 0;
	m_nSurfaceSample = 0;
	m_SurfaceLine = new CPolyCoord[1];
}

void CProfile::ModifySurfaceLine(CCoord Cp,int nSample,int nLine)
{
	if(m_SurfaceLine == NULL) return;

/*	if(nLine < m_nSurfaceLineL) {

		if((nSample < m_SurfaceLine[nLine].m_nCoordL) && (nSample>=0)) {

			m_SurfaceLine[nLine].ModifyCoord(nSample,Cp);

		}
	}
	*/

	CPolyCoord *pLine = NULL;
	
	if(m_nSurfaceSample==nSample)
		return;

	m_nSurfaceSample = nSample;

	if(m_nSurfaceLineL==0) {
		m_SurfaceLine = new CPolyCoord[m_nSurfaceLineL=1];
		m_SurfaceLine[nLine=0].Add(Cp);
	} else {
		if(nLine<m_nSurfaceLineL) {
			m_SurfaceLine[nLine].Add(Cp);
		}else{
			pLine = new CPolyCoord[m_nSurfaceLineL];
			
			for(nLine=0;nLine<m_nSurfaceLineL;nLine++) {
				pLine[nLine] = &m_SurfaceLine[nLine];
			}
						
			SAFE_DELETE_ARRAY(m_SurfaceLine);

			m_SurfaceLine = new CPolyCoord[m_nSurfaceLineL + 1];

			for(nLine=0;nLine<m_nSurfaceLineL;nLine++) {
				m_SurfaceLine[nLine] = &pLine[nLine];
			}

			m_nSurfaceLineL++;
		}
	}


}


void CProfile::ComputePolarValues(int nWhichLines)
{
	CPolyCoord* pLine = NULL;
	int nLineL;
	int	nLine;

	switch(nWhichLines) {
	case TAUGHTLINES: pLine = m_TaughtLine;
		nLineL = m_nTaughtLineL;
		break;
	case SCANLINES: pLine = m_ScanLine;
		nLineL = m_nScanLineL;
		break;
	case BRAINLINES: pLine = m_BrainLine;
		nLineL = m_nBrainLineL;
		break;
	case ENVELOPELINES: pLine = m_EnvelopeLine;
		nLineL = m_nEnvelopeLineL;
		break;
	}
	if(pLine==NULL) return;
	if(nLineL<=0) return;


	for(nLine=0;nLine<nLineL;nLine++) {
		pLine[nLine].ComputePolarValues(theApp.m_Tank.vTTZeroDegrees);
	}

}

void CProfile::GenerateTurntableWayPoints()
{
	CCoord CpSurface,CpHead;
	D3DXMATRIXA16 matRotate;
	CPolyCoord Line;
	int	nn;
	float fPos;

	SAFE_DELETE_ARRAY( m_WayPoints );
	m_nWayPointsL = 0;

	if(m_nScanLineL>0) {

		m_nWayPointsL = m_nScanLineL;
		m_WayPoints = new CPolyCoord[m_nWayPointsL];

		m_nWayPointNumber = 720;

		for(int nLine = 0;nLine<m_nWayPointsL; nLine++) {
			m_WayPoints[nLine].AllocateCoords( m_nWayPointNumber );
			m_WayPoints[nLine].ClearMemory();
			m_WayPoints[nLine].SetPrimaryAxis(5);
			m_WayPoints[nLine].SetStyle(0);

			Line.AllocateCoords(m_ScanLine[nLine].m_nCoordL);

			for(nn=0;nn<m_ScanLine[nLine].m_nCoordL;nn++) {
				m_ScanLine[nLine].GetCoord(nn,&CpSurface);

				D3DXMatrixRotationZ(&matRotate,CpSurface.Side0.fR * -DEG_TO_RAD);

				D3DXVec3TransformCoord(&CpSurface.Side[PORTSIDE].pt,&CpSurface.Side[PORTSIDE].pt,&matRotate);
				D3DXVec3TransformCoord((D3DXVECTOR3*)&CpSurface.Side[PORTSIDE].norm,(D3DXVECTOR3*)&CpSurface.Side[PORTSIDE].norm,&matRotate);
				D3DXVec3Normalize((D3DXVECTOR3*)&CpSurface.Side[PORTSIDE].norm,(D3DXVECTOR3*)&CpSurface.Side[PORTSIDE].norm);

				D3DXVec3TransformCoord(&CpSurface.Side[STARBOARD].pt,&CpSurface.Side[STARBOARD].pt,&matRotate);
				D3DXVec3TransformCoord((D3DXVECTOR3*)&CpSurface.Side[STARBOARD].norm,(D3DXVECTOR3*)&CpSurface.Side[STARBOARD].norm,&matRotate);
				D3DXVec3Normalize((D3DXVECTOR3*)&CpSurface.Side[STARBOARD].norm,(D3DXVECTOR3*)&CpSurface.Side[STARBOARD].norm);

				theApp.m_Kinematics.TipsFromNormals(&CpSurface,PORTSIDE);
				theApp.m_Kinematics.TipsFromNormals(&CpSurface,STARBOARD);
				if(CpSurface.Side0.fR < 0.0f) CpSurface.Side0.fR += 360.0f;
				CpHead = CpSurface;		//ensures fR gets copied accross
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,PORTSIDE,TRUE,TRUE, NULL, 0);
				theApp.m_Kinematics.HeadFromSurface(&CpSurface,&CpHead,STARBOARD,TRUE,TRUE, NULL, 0);

				Line.ModifyCoord(nn,CpSurface);
			}

			Line.SetPrimaryAxis(5);
			Line.Sort(5);
			for(nn=0;nn<m_nWayPointNumber;nn++) {
				fPos = 360.0f * (float)nn / (float)m_nWayPointNumber;
				Line.CalculateCoord(fPos,CpHead);
				m_WayPoints[nLine].ModifyCoord(nn,CpHead);
			}
		}
	}


}

void CProfile::CalculateTessellationEdges()
{
	
	D3DXVECTOR3 v[2];
	EDGE Edge;
	int ii,jj;

	m_Edge.Zero();

	for(ii=0;ii<m_vCloud.GetSize();ii++) {
		m_vCloud.GetPt(ii, &v[0]);

		for(jj=0;jj<m_vCloud.GetSize(); jj++) {
			if(ii-jj) {
				m_vCloud.GetPt(jj, &v[1]);
				Edge.nNode[0]=ii;
				Edge.nNode[1]=jj;
				Edge.v[0].pt = v[0];
				Edge.v[1].pt = v[1];
				Edge.SetLength();
				m_Edge.AddIfShortest(&Edge,0);
			}
		}
	}

	for(int nLoop = 0;nLoop<5;nLoop++) {
		for(ii=0;ii<m_vCloud.GetSize();ii++) {

			int nStartLength = m_Edge.GetSize();

			for(jj=0;jj<m_vCloud.GetSize();jj++) {
				Edge.nNode[0]=ii;
				Edge.nNode[1]=jj;
				if(m_Edge.DoesEdgeExist(&Edge) == false) {

					m_vCloud.GetPt(ii, &v[0]);
					m_vCloud.GetPt(jj, &v[1]);
					Edge.nNode[0]=ii;
					Edge.nNode[1]=jj;
					Edge.v[0].pt = v[0];
					Edge.v[1].pt = v[1];
					Edge.SetLength();
					m_Edge.AddIfShortest(&Edge,nStartLength);

				}
			}
		}
	}

	m_Edge.RemoveCrossingEdges();
	m_Edge.Sort(0);

}

void CProfile::CropScanLines(int nLessThanGreaterThan, float fPos, int nAxis)
{
	for(int nLine=0;nLine<m_nScanLineL;nLine++) {
		m_ScanLine[nLine].Crop(nLessThanGreaterThan, fPos, nAxis);
	}

}

void CProfile::DeleteScanLinesThatAreCompletelyOneSideOfThePlane(int nLessThanGreaterThan, float fPos, int nPlane)
{
	int nLine,nn;
	bool bDelete;

	for(nLine=0;nLine<m_nScanLineL;nLine++) {

		bDelete = true;
		for(nn=0;(nn<m_ScanLine[nLine].GetSize()) && (bDelete == true);nn++) {

			switch(nLessThanGreaterThan) {
			case 0:
				if(m_ScanLine[nLine].m_pCp[nn].fPos[nPlane]>fPos) {
					bDelete = false;
				}
				break;
			case 1:
				if(m_ScanLine[nLine].m_pCp[nn].fPos[nPlane]<fPos) {
					bDelete = false;
				}
				break;
			}
		}
		if(bDelete == true) {
			DeleteLine(SCANLINES,nLine);
			nLine--;
		}

	}

}

void CProfile::ConvertLeftHandedToRightHanded(int nLineType)
{
	int	nLine;

	switch(nLineType) {
	case SCANLINES:
		for(nLine=0;nLine<m_nScanLineL;nLine++) {
			m_ScanLine[nLine].ConvertLeftHandedToRightHanded();
		}
		break;
	case TAUGHTLINES:
		for(nLine=0;nLine<m_nTaughtLineL;nLine++) {
			m_TaughtLine[nLine].ConvertLeftHandedToRightHanded();
		}
		break;
	}

}

void CProfile::SplitScanLinesDueToXDirChange()
{
	int	nLine,ii,nStart,jj;
	int nMaxLines = 10000;
	CPolyCoord *pLine = new CPolyCoord[nMaxLines];
	int nNewLine = 0;

	for(nLine = 0;nLine<m_nScanLineL; nLine++) {

		for(ii=1,nStart=1;ii<m_ScanLine[nLine].GetSize()-2;ii++) {

			if(m_ScanLine[nLine].IsChangeInDir(ii,0) ) {
				pLine[nNewLine].Zero();
				pLine[nNewLine].AllocateCoords((ii-nStart)+1);
				for(jj=0;nStart<=ii;nStart++,jj++) {
					pLine[nNewLine].m_pCp[jj] = m_ScanLine[nLine].m_pCp[nStart];
				}
				nNewLine++;
				ii = nStart;
			}
		}
	}

	SAFE_DELETE_ARRAY( m_ScanLine );
	m_nScanLineL = nNewLine;
	m_ScanLine = new CPolyCoord[ m_nScanLineL ];

	for(nLine =0; nLine<m_nScanLineL;nLine++) {
		m_ScanLine[nLine] = &pLine[nLine];
	}

	SAFE_DELETE_ARRAY( pLine );

}

void CProfile::SplitScanLinesDueToValueChange(int nAxis)
{

	int	nLine,nIndex;
	int nLineL = 0;
	CPolyCoord *pLine = new CPolyCoord[10000];
	int nNewLine = 0;

	for(nLine = 0;nLine<m_nScanLineL; nLine++) {

		for(nIndex=0;nIndex<m_ScanLine[nLine].GetSize();nIndex++) {

			for(nNewLine=0;nNewLine<nLineL;nNewLine++) {
				if(fabsf(pLine[nNewLine].m_pCp[0].fPos[nAxis] - m_ScanLine[nLine].m_pCp[nIndex].fPos[nAxis]) < 0.2f) {
					pLine[nNewLine].Add(m_ScanLine[nLine].m_pCp[nIndex]);
					break;
				}
			}
			if(nNewLine == nLineL) {
				nLineL++;
				pLine[nNewLine].Zero();
				pLine[nNewLine].Add(m_ScanLine[nLine].m_pCp[nIndex]);
			}
		}
	}

	SAFE_DELETE_ARRAY( m_ScanLine );
	m_nScanLineL = nLineL;
	m_ScanLine = new CPolyCoord[ m_nScanLineL ];

	for(nLine =0; nLine<m_nScanLineL;nLine++) {
		m_ScanLine[nLine] = &pLine[nLine];
	}

	SAFE_DELETE_ARRAY( pLine );
}


void CProfile::AddRRotation(float fDelta)
{
	int	nLine;

	if(fDelta == 0.0f) return;

	for(nLine=0;nLine<m_nTaughtLineL;nLine++) {
		m_TaughtLine[nLine].Add(5,fDelta);
		m_TaughtLine[nLine].Add(15,fDelta);
	}
	for(nLine=0;nLine<m_nScanLineL;nLine++) {
		m_ScanLine[nLine].Add(5,fDelta);
		m_ScanLine[nLine].Add(15,fDelta);
	}
	for(nLine=0;nLine<m_nBrainLineL;nLine++) {
		m_BrainLine[nLine].Add(5,fDelta);
		m_BrainLine[nLine].Add(15,fDelta);
	}
	for(nLine=0;nLine<m_nEnvelopeLineL;nLine++) {
		m_EnvelopeLine[nLine].Add(5,fDelta);
		m_EnvelopeLine[nLine].Add(15,fDelta);
	}

}

void CProfile::RotateAllNormalsToIEqualsZero()
{
	int nLine;

	for(nLine=0;nLine<m_nScanLineL;nLine++) {
		m_ScanLine[nLine].RotateAllNormalsToIEqualsZero();
	}
	for(nLine=0;nLine<m_nTaughtLineL;nLine++) {
		m_TaughtLine[nLine].RotateAllNormalsToIEqualsZero();
	}
	for(nLine=0;nLine<m_nBrainLineL;nLine++) {
		m_BrainLine[nLine].RotateAllNormalsToIEqualsZero();
	}
}

void CProfile::SetScanLineWaterPaths(int nStartLine, int nFinishLine, float fWaterPath)
{
	int nLine;
	for(nLine=nStartLine; nLine<=nFinishLine && nLine<m_nScanLineL;nLine++) {
		m_ScanLine[nLine].SetAllWp(PORTSIDE,fWaterPath);
		m_ScanLine[nLine].SetAllWp(STARBOARD,fWaterPath);
	}
}

void CProfile::SetScanLineRAngle(int nStartLine, int nFinishLine, float fAngle)
{
	int nLine;
	for(nLine=nStartLine; nLine<=nFinishLine && nLine<m_nScanLineL;nLine++) {
		m_ScanLine[nLine].SetAllRPos(PORTSIDE,fAngle);
		m_ScanLine[nLine].SetAllRPos(STARBOARD,fAngle);
	}
}

void CProfile::AddScanLineRAngle(int nStartLine, int nFinishLine, float fAngle)
{
	int nLine;
	for(nLine=nStartLine; nLine<=nFinishLine && nLine<m_nScanLineL;nLine++) {
		m_ScanLine[nLine].AddAllRPos(PORTSIDE,fAngle);
		m_ScanLine[nLine].AddAllRPos(STARBOARD,fAngle);
	}
}

void CProfile::GenerateScanLinesScanForAsymetricDNose()
{
	
	float fLength;
	CCoord CpStart,CpFinish;
	CPolyCoord Line,ToolLine;
	int	nLine,nn;
	float fMaxLength,fPos,fLinePos;
	CCoord Cp,Cp0,Cp1;
	int	nSlowAxis=2; //Slow axis hardcoded to Z???

	FindLineLimits(&m_EnvelopeLine[0],m_nEnvelopeLineL,&CpStart,&CpFinish);

	fMaxLength = 0.0f;
	for(nn=0;nn<m_nTaughtLineL;nn++) {
		m_TaughtLine[nn].SetPrimaryAxis(30);
		fLength = m_TaughtLine[nn].GetLength();
		if(fMaxLength<fLength) {
			fMaxLength=fLength;
			nLine = nn;
		}
	}

	CProgressDlg dlg(NULL);
	dlg.Create(CProgressDlg::IDD, NULL);
	dlg.SetProgressText(L"Calculating scan path");
	int nNumberLines = (int)(fMaxLength/m_fSlowIncrement);
	dlg.SetProgressRange(0,nNumberLines);

	SAFE_DELETE_ARRAY( m_ScanLine );
	m_nScanLineL=0;
	m_ScanLine = new CPolyCoord[nNumberLines];

	for(m_nScanLineL=0;m_nScanLineL<nNumberLines;m_nScanLineL++) {
		dlg.SetProgressPos(m_nScanLineL);
		Line.Zero();
		for(nn=0;nn<m_nTaughtLineL;nn++) {
			fPos = (m_TaughtLine[nn].GetLength() * (float)m_nScanLineL) / (float)(nNumberLines-1);
			m_TaughtLine[nn].CalculateCoord(fPos,Cp);
			Line.Add(Cp);
		}
		FindScanLineLimitsFromEnvelope(m_nFastAxis,nSlowAxis,Cp.Side0.fZ,&Cp0,&Cp1); //assumes slow axis is z

		ToolLine.Zero();
		for(nn=0;nn<10;nn++) {
			fLinePos = (((float)nn * (CpFinish.fPos[m_nFastAxis] - CpStart.fPos[m_nFastAxis])) / 9.0f) + CpStart.fPos[m_nFastAxis];
			Line.CalculateCoord(fLinePos,Cp);
			ToolLine.Add(Cp);
		}
		if(m_nScanLineL<nNumberLines) {
			m_ScanLine[m_nScanLineL] = &ToolLine;
		} else {
			AddLine(SCANLINES, &ToolLine,nSlowAxis);
		}
	}

	CalculateDangerZone();
	m_bModified = TRUE;

}

void CProfile::GenerateDNoseScanLinesWithStraightLE()
{
	
	float fLength;
	CCoord CpStart,CpFinish;
	CPolyCoord Line,ToolLine;
	int	nLine,nn,nLineFinished,nLoop;
	float fMaxLength,fLinePos;
	CCoord Cp,Cp0,Cp1,Cp2;

	int	nSlowAxis=2; //Slow axis hardcoded to Z???
	FindLineLimits(&m_EnvelopeLine[0],m_nEnvelopeLineL,&CpStart,&CpFinish);

	CCoord *pCpLE = new CCoord[m_nTaughtLineL];
	float *fCurrentPos = new float[m_nTaughtLineL];
	fMaxLength = 0.0f;
	for(nn=0;nn<m_nTaughtLineL;nn++) {
		m_TaughtLine[nn].SetPrimaryAxis(30);
		fLength = m_TaughtLine[nn].GetLength();
		if(fMaxLength<fLength) {
			fMaxLength=fLength;
			nLine = nn;
		}

		m_TaughtLine[nn].FindMinimumYCoord(Cp);
		pCpLE[nn] = Cp;
	}

	CProgressDlg dlg(NULL);
	dlg.Create(CProgressDlg::IDD, NULL);
	dlg.SetProgressText(L"Calculating bottom half scan path");
	int nNumberLines = (int)(fMaxLength/m_fSlowIncrement);
	dlg.SetProgressRange(0,100);

	SAFE_DELETE_ARRAY( m_ScanLine );
	m_nScanLineL=0;
	m_ScanLine = new CPolyCoord[nNumberLines];

	//Bottom half
	nLoop = 0;
	nLineFinished = 0;
	while(nLineFinished<m_nTaughtLineL) {
		nLineFinished=0;
		Line.Zero();
		for(nn=0;nn<m_nTaughtLineL;nn++) {
			fCurrentPos[nn] = pCpLE[nn].fLength - ((float)nLoop * m_fSlowIncrement);
			m_TaughtLine[nn].CalculateCoord(fCurrentPos[nn],Cp);
			Line.Add(Cp);
			if(fCurrentPos[nn]<=0.0f) {
				nLineFinished++;
			}
		}
		FindScanLineLimitsFromEnvelope(m_nFastAxis,nSlowAxis,Cp.Side0.fZ,&Cp0,&Cp1); //assumes slow axis is z

		ToolLine.Zero();
		for(nn=0;nn<40;nn++) {
			fLinePos = (((float)nn * (CpFinish.fPos[m_nFastAxis] - CpStart.fPos[m_nFastAxis])) / 39.0f) + CpStart.fPos[m_nFastAxis];
			Line.CalculateCoord(fLinePos,Cp);
			ToolLine.Add(Cp);
		}
		AddScanLine(&ToolLine);
		nLoop++;
	}
	ReverseScanLineOrder();

	//Top half
	dlg.SetProgressPos(25);
	dlg.SetProgressText(L"Calculating top half scan path");
	nLoop = 0;
	nLineFinished = 0;
	while(nLineFinished<m_nTaughtLineL) {
		nLineFinished=0;
		Line.Zero();
		for(nn=0;nn<m_nTaughtLineL;nn++) {
			fCurrentPos[nn] = pCpLE[nn].fLength + ((float)nLoop * m_fSlowIncrement);
			m_TaughtLine[nn].CalculateCoord(fCurrentPos[nn],Cp);
			Line.Add(Cp);
			if(fCurrentPos[nn]>=fMaxLength) {
				nLineFinished++;
			}
		}
		FindScanLineLimitsFromEnvelope(m_nFastAxis,nSlowAxis,Cp.Side0.fZ,&Cp0,&Cp1); //assumes slow axis is z

		ToolLine.Zero();
		for(nn=0;nn<40;nn++) {
			fLinePos = (((float)nn * (CpFinish.fPos[m_nFastAxis] - CpStart.fPos[m_nFastAxis])) / 39.0f) + CpStart.fPos[m_nFastAxis];
			Line.CalculateCoord(fLinePos,Cp);
			ToolLine.Add(Cp);
		}
		AddScanLine(&ToolLine);
		nLoop++;
	}

	dlg.SetProgressPos(50);
	dlg.SetProgressText(L"Trim edges");

//Trim edges
	if(m_nTaughtLineL <= 2) {
		m_TaughtLine[0].GetLastCoord(&Cp0);
		m_TaughtLine[1].GetLastCoord(&Cp1);
		TrimEdge(Cp0,Cp1);

		m_TaughtLine[0].GetCoord(0,&Cp0);
		m_TaughtLine[1].GetCoord(0,&Cp1);
		TrimEdge(Cp0,Cp1);
	} else {
		//Top edge;
		m_TaughtLine[0].GetLastCoord(&Cp0);
		m_TaughtLine[m_nTaughtLineL-1].GetLastCoord(&Cp2);

		Cp1 = Cp0;
		for(nn=1;nn<(m_nTaughtLineL-1);nn++) {
			m_TaughtLine[nn].GetLastCoord(&Cp);
			if(Cp1.Side0.fY < Cp.Side0.fY) {
				Cp1 = Cp;
			}
		}
		if(Cp1.Side0.fY >= Cp0.Side0.fY) {
			TrimEdge(Cp0,Cp1);
			TrimEdge(Cp1,Cp2);
		} else {
			TrimEdge(Cp0,Cp2);
		}

		//Bottom edge;
		m_TaughtLine[0].GetCoord(0,&Cp0);
		m_TaughtLine[m_nTaughtLineL-1].GetCoord(0,&Cp2);

		Cp1 = Cp0;
		for(nn=1;nn<(m_nTaughtLineL-1);nn++) {
			m_TaughtLine[nn].GetCoord(0,&Cp);
			if(Cp1.Side0.fY < Cp.Side0.fY) {
				Cp1 = Cp;
			}
		}
		if(Cp1.Side0.fY >= Cp0.Side0.fY) {
			TrimEdge(Cp0,Cp1);
			TrimEdge(Cp1,Cp2);
		} else {
			TrimEdge(Cp0,Cp2);
		}

	}

	dlg.SetProgressPos(75);
	dlg.SetProgressText(L"Remove empty lines");

//remove empty lines
	for(nn=0;nn<m_nScanLineL;nn++) {
		if(m_ScanLine[nn].GetSize()<=1) {
			DeleteLine(SCANLINES,nn);
			nn=0;
		}
	}


	CalculateDangerZone();
	m_bModified = TRUE;

	SAFE_DELETE_ARRAY(pCpLE);
	SAFE_DELETE(fCurrentPos);

}

void CProfile::TrimEdge(CCoord &Cp0, CCoord &Cp1)
{
	int nn;
	float fAngle;
	D3DXMATRIXA16 matComponent,matTran,matZRot,matYRot;

	D3DXMatrixTranslation(&matTran,-Cp0.Side0.fX,-Cp0.Side0.fY,-Cp0.Side0.fZ);

	fAngle = (float)atan2(Cp1.Side0.fY-Cp0.Side0.fY,Cp1.Side0.fX-Cp0.Side0.fX);
	D3DXMatrixRotationZ(&matZRot,-fAngle);
	fAngle = (float)atan2(Cp1.Side0.fZ-Cp0.Side0.fZ,Cp1.Side0.fX-Cp0.Side0.fX);
	D3DXMatrixRotationY(&matYRot,fAngle);

	D3DXMatrixMultiply(&matComponent,&matTran,&matZRot);
	D3DXMatrixMultiply(&matComponent,&matComponent,&matYRot);


	for(nn=0;nn<m_nTaughtLineL;nn++) {
		m_TaughtLine[nn].TransformLine(matComponent);
	}
	for(nn=0;nn<m_nScanLineL;nn++) {
		m_ScanLine[nn].TransformLine(matComponent);
	}


	for(nn=0;nn<m_nScanLineL;nn++) {
		m_ScanLine[nn].DeleteAnyCoordWithYgreaterThan(0.0f,-40.0f,40.0f);
	}

	D3DXMatrixInverse(&matComponent, NULL, &matComponent);
	for(nn=0;nn<m_nTaughtLineL;nn++) {
		m_TaughtLine[nn].TransformLine(matComponent);
	}
	for(nn=0;nn<m_nScanLineL;nn++) {
		m_ScanLine[nn].TransformLine(matComponent);
	}

}

void CProfile::DeleteScanCoordWith(int nWhichSign, char cNorm, int nStart, int nFinish)
{

	for(int nLine=nStart;nLine<=nFinish;nLine++) {
		m_ScanLine[nLine].DeleteCoordsWith(nWhichSign, cNorm);

	}
}

void CProfile::RotateComponent(int nRotateAxis, float fAngle)
{
	D3DXMATRIXA16 matRotate;

	switch(nRotateAxis) {
	case 0: D3DXMatrixRotationX(&matRotate,fAngle*DEG_TO_RAD);
		break;
	case 1: D3DXMatrixRotationY(&matRotate,fAngle*DEG_TO_RAD);
		break;
	case 2: D3DXMatrixRotationZ(&matRotate,fAngle*DEG_TO_RAD);
		break;
	}

	ComponentMove(matRotate);

}


void CProfile::GenerateInterpolatedScanLines()
{
	CCoord Cp[2],Cpnn[2],CpAA[2],CpBB[2],Cpjj[2],Cpll;
	float fLength,fNumerator,fDenominator;
	int nLine,nn,nIndex,ii;
	float fPos,fFastPos;

	SAFE_DELETE_ARRAY( m_ScanLine );
	m_nScanLineL = 0;

	m_TaughtLine[0].GetCoord(0,&Cp[0]);
	m_TaughtLine[m_nTaughtLineL-1].GetCoord(0,&Cp[1]);

	fLength = fabsf(Cp[1].Side0.fPos[m_nSlowAxis] - Cp[0].Side0.fPos[m_nSlowAxis]);
	m_nScanLineL = (int)(fLength / m_fSlowIncrement);

	if(m_nScanLineL > 0) {

		m_ScanLine = new CPolyCoord[m_nScanLineL];

		for(nLine = 0;nLine < m_nScanLineL; nLine++) {
			fPos = ((float)nLine * (Cp[1].Side0.fPos[m_nSlowAxis] - Cp[0].Side0.fPos[m_nSlowAxis]) / (float)(m_nScanLineL - 1)) + Cp[0].Side0.fPos[m_nSlowAxis];

			for(nn=0;nn<(m_nTaughtLineL-1);nn++) {
				m_TaughtLine[nn].GetCoord(0,&Cpnn[0]);
				m_TaughtLine[nn+1].GetCoord(0,&Cpnn[1]);
				if((fPos>=Cpnn[0].Side0.fPos[m_nSlowAxis]) && (fPos<=Cpnn[1].Side0.fPos[m_nSlowAxis])) {

					m_ScanLine[nLine].AllocateCoords(200);

					m_TaughtLine[nn].GetCoord(0,&CpAA[0]);
					m_TaughtLine[nn].GetLastCoord(&CpAA[1]);
					m_TaughtLine[nn+1].GetCoord(0,&CpBB[0]);
					m_TaughtLine[nn+1].GetLastCoord(&CpBB[1]);

					for(nIndex=0;nIndex<200;nIndex++) {
						fFastPos = ((float)nIndex * (CpAA[1].Side0.fPos[m_nFastAxis] - CpAA[0].Side0.fPos[m_nFastAxis]) / 199.0f) + CpAA[0].Side0.fPos[m_nFastAxis];
						m_TaughtLine[nn].CalculateCoord(fFastPos,Cpjj[0]);

						fFastPos = ((float)nIndex * (CpBB[1].Side0.fPos[m_nFastAxis] - CpBB[0].Side0.fPos[m_nFastAxis]) / 199.0f) + CpBB[0].Side0.fPos[m_nFastAxis];
						m_TaughtLine[nn+1].CalculateCoord(fFastPos,Cpjj[1]);

						fNumerator = fPos - Cpjj[0].Side0.fPos[m_nSlowAxis];
						fDenominator = Cpjj[1].Side0.fPos[m_nSlowAxis] - Cpjj[0].Side0.fPos[m_nSlowAxis];
						Cpll = Cpjj[0];
						for(ii=0;ii<30;ii++) {
							Cpll.fPos[ii] = (fNumerator * (Cpjj[1].fPos[ii] - Cpjj[0].fPos[ii]) / fDenominator) + Cpjj[0].fPos[ii];
							m_ScanLine[nLine].ModifyCoord(nIndex,Cpll);
						}
					}

					break;
				}
			}
		}
	}
		
}

void CProfile::DeleteAnythingGreater45()
{

	for(int nLine = 0 ;nLine<m_nScanLineL;nLine++) {
		m_ScanLine[nLine].DeleteAnyCoordXtYtGreater(37.5f);
	}
}

void CProfile::BladeEdgeClip()
{
	for(int nLine = 0 ;nLine<m_nScanLineL;nLine++) {
		m_ScanLine[nLine].BladeEdgeClip();
	}

}


void CProfile::GenerateArcScanLines(int nCentreMode)
{
	D3DXMATRIXA16 matWorld,matTranslation,matNormal;
	D3DXVECTOR3 vecN,vecP,vecA,vecB,vecT;
	int nn,nLine,aa;
	int nSide = 0;
	float fLength,fRadius,fAngle0,fAngle1,fArcAngle,fStepAngle,fStartAngle;
	CCoord Cp0,Cp1;
	CPolyCoord Arc;
	CVertexArray vertArc;
	D3DXVECTOR3 vCentre,vNorm;


	switch(nCentreMode) {
	case 0:		//TT Centre
		break;
	case 1:		//Circumcentre
		break;
	};

	SAFE_DELETE_ARRAY(m_ScanLine);
	m_nScanLineL=0;


	Arc.AllocateCoords(m_nTaughtLineL);
	vertArc.SetSize(m_nTaughtLineL);

	if(m_TaughtLine[0].m_nStyle == 0) {
		for(nLine=0;nLine<m_nTaughtLineL;nLine++) {
			m_TaughtLine[nLine].SetPrimaryAxis(30);
		}
		fLength=m_TaughtLine[0].GetLength();
		m_nScanLineL = (int)(fLength / m_fSlowIncrement);
		m_ScanLine = new CPolyCoord[m_nScanLineL];

		Cp0=m_TaughtLine[0].m_pCp[0];
		fStartAngle = atan2f(Cp0.Side0.fY,Cp0.Side0.fX);
		for(nn=0;nn<m_nScanLineL;nn++) {

			for(aa=0;aa<m_nTaughtLineL;aa++) {
				m_TaughtLine[aa].CalculateCoord(m_fSlowIncrement * (float)nn,Cp1);
				Arc.ModifyCoord(aa,Cp1);
				vertArc.SetPt(aa,&Cp1.Side0.pt);
			}
			vertArc.CalculateCircumCenter(&fRadius,&vCentre,&fAngle0,&fAngle1,&vNorm);
			fArcAngle = fAngle0 + fAngle1;

			float fYaw = atan2f(vNorm.x, vNorm.z);
			float fPitch = atan2f(-vecN.y , sqrtf(vNorm.x * vNorm.x + vNorm.z * vNorm.z));

			D3DXMatrixRotationYawPitchRoll( &matNormal, fYaw, fPitch, 0 );
			D3DXMatrixTranslation(&matTranslation,vCentre.x,vCentre.y,vCentre.z);
			D3DXMatrixMultiply(&matNormal,&matNormal,&matTranslation);

			m_ScanLine[nn].AllocateCoords(200);

			for(int nNode=0;nNode<200;nNode++) {
				fStepAngle = fStartAngle - (float)nNode * fArcAngle / 199.0f;
				D3DXMatrixRotationZ(&matWorld,fStepAngle);
//				D3DXMatrixMultiply(&matWorld,&matWorld,&matNormal);

				vecP.x = fRadius;
				vecP.y = 0.0f;
				vecP.z = 0.0f;
				D3DXVec3TransformCoord(&vecP,&vecP,&matWorld);
				D3DXVec3TransformCoord(&vecP,&vecP,&matTranslation);

				vecN.x = 1.0f;
				vecN.y = 0.0f;
				vecN.z = 0.0f;
				D3DXVec3TransformCoord(&vecN,&vecN,&matWorld);

				m_ScanLine[nn].m_pCp[nNode].Side0.pt = vecP;
				m_ScanLine[nn].m_pCp[nNode].Side0.fI = vecN.x;
				m_ScanLine[nn].m_pCp[nNode].Side0.fJ = vecN.y;
				m_ScanLine[nn].m_pCp[nNode].Side0.fK = vecN.z;
				m_ScanLine[nn].m_pCp[nNode].Side0.fW = 0.0f;

			}


		}

	} else {
		fLength=m_TaughtLine[0].GetSplineLength(200);
		m_nScanLineL = (int)(fLength / m_fSlowIncrement);
		m_ScanLine = new CPolyCoord[m_nScanLineL];

		Cp0=m_TaughtLine[0].m_pCp[0];
		m_ScanLine[0].Add(Cp0);
		for(nn=0;nn<m_nTaughtLineL;nn++) {
			m_TaughtLine[0].CalculateCoord(Cp0,m_fSlowIncrement,&Cp1);
			m_ScanLine[0].Add(Cp1);
			Cp0=Cp1;
		}

	}
	m_bModified = TRUE;


}

void CProfile::RotateBlade(int nAxis, float fAngle, int nOutsideInside)
{
	int nLine;

	D3DXMATRIXA16 matRotate;

	switch(nAxis) {
	case 0: D3DXMatrixRotationX(&matRotate,fAngle*DEG_TO_RAD);
		break;
	case 1: D3DXMatrixRotationY(&matRotate,fAngle*DEG_TO_RAD);
		break;
	case 2: D3DXMatrixRotationZ(&matRotate,fAngle*DEG_TO_RAD);
		break;
	}


	for(nLine=0;nLine<m_nLineBladeL[nOutsideInside];nLine++) {
		m_LineBlade[nOutsideInside][nLine].TransformLine(matRotate);
	}

//	m_matCadToScan *= matWorld;

	CalculateDangerZone();
	m_bModified = TRUE;
}

void CProfile::AddRAngleToBlade(float fAngle, int nOutsideInside)
{
	int	nLine;

	if(fAngle == 0.0f) return;

	for(nLine=0;nLine<m_nLineBladeL[nOutsideInside];nLine++) {
		m_LineBlade[nOutsideInside][nLine].Add(5,fAngle);
	}

}

void CProfile::DecimateBladeLines(int nWhich)
{

	for(int nLine=0;nLine<m_nLineBladeL[nWhich];nLine++) {
		m_LineBlade[nWhich][nLine].DecimateEitherSideOfZero(m_fFastIncrement);
	}


}

void CProfile::AddBladeBranePt(int nLine)
{
	int nn;
	CCoord Cp,CpSurface;
	CPolyCoord Line;

	if(m_BrainLine == NULL) {
		m_BrainLine = new CPolyCoord[m_nBrainLineL = 2];
		m_ScanLine[0].GetFirstCoord(&Cp);		m_BrainLine[0].Add(Cp);
		m_ScanLine[0].GetLastCoord(&Cp);		m_BrainLine[0].Add(Cp);
		m_BrainLine[0].ZeroAllDeltas(0);
		nn = m_nScanLineL - 1;
		m_ScanLine[nn].GetFirstCoord(&Cp);		m_BrainLine[1].Add(Cp);
		m_ScanLine[nn].GetLastCoord(&Cp);		m_BrainLine[1].Add(Cp);
		m_BrainLine[1].ZeroAllDeltas(0);
	}

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	CpSurface.Delta.Zero();

findBrainLine:
	for(nn=0;nn<m_nBrainLineL;nn++) {
		m_BrainLine[nn].GetFirstCoord(&Cp);
		if(fabs(CpSurface.Side0.fY - Cp.Side0.fY) < 8.0f) {
			goto BrainLineFound;
		}
	}

	Line.Zero();
	m_ScanLine[nLine].GetFirstCoord(&Cp);		Cp.Delta.Zero();	Line.Add(Cp);
	m_ScanLine[nLine].GetLastCoord(&Cp);		Cp.Delta.Zero();	Line.Add(Cp);
	AddLine(BRAINLINES,&Line,1);
	goto findBrainLine;

BrainLineFound:
	m_ScanLine[nLine].CalculateCoord(CpSurface.Side0.fX,Cp);
	Cp.Delta.Zero();
	CpSurface.Delta.fI = CpSurface.Side0.fI - Cp.Side0.fI;
	CpSurface.Delta.fJ = CpSurface.Side0.fJ - Cp.Side0.fJ;
	CpSurface.Delta.fK = CpSurface.Side0.fK - Cp.Side0.fK;

	int nNearest = m_BrainLine[nn].FindNearestCoord(CpSurface.Side0.fX,m_BrainLine[nn].GetSize() / 2);
	m_BrainLine[nn].GetCoord(nNearest,&Cp);
	float fLength = Cp.Length(CpSurface,0);
	if(fLength<10.0f) {
		m_BrainLine[nn].DeleteCoord(nNearest);
	}

	m_BrainLine[nn].Add(CpSurface);
	m_BrainLine[nn].Sort(0,0);

}

void CProfile::CopyBraneLinesToBladeBraneLines(int nSide)
{
/*
	if(m_BrainLines && (m_nBrainLineL>0)) {
		SAFE_DELETE_ARRAY( m_LineBlade[nSide + 2] );
		m_LineBlade[nSide + 2] = new CPolyCoord[ m_nLineBladeL[nSide + 2] = m_nBrainLineL ];
		for(int nLine = 0; nLine < m_nBrainLineL; nLine++) {
			m_LineBlade[nSide + 2][nLine] = &m_BrainLine[nLine];
		};
	}
*/
}

HRESULT CProfile::GenerateRectilinearScanFromCoordinates(int nSide)
{
	
	HRESULT hr = NULL;
	CCoord CpBL,CpBR,CpTL,CpTR,Cp,Cp0,Cp1;
	int nLine,nAxis;
	float	fX,fY,fZ,fR;
	int nFastAxis = m_nFastAxis;

	if((m_fFastIncrement<=0.0) || (m_fSlowIncrement<=0.0)) {
		MessageBox(NULL,_T("Scan increments are incorrect"),_T("Error"),MB_ICONERROR);
		hr = S_FALSE;
		goto LCleanReturn;
	}

	if(m_nFastAxis==5) {
		return GenerateContinousRotateRectilinearScanFromCoordinates(nSide,true);
	}

	nSide &= 1;

	SAFE_DELETE_ARRAY(m_ScanLine);
	m_nScanLineL = 0;

	fX = fabs(m_CpRect[1].Side[nSide].fX - m_CpRect[0].Side[nSide].fX);
	fY = fabs(m_CpRect[1].Side[nSide].fY - m_CpRect[0].Side[nSide].fY);
	fZ = fabs(m_CpRect[1].Side[nSide].fZ - m_CpRect[0].Side[nSide].fZ);
	fR = fabs(m_CpRect[1].Side[nSide].fR - m_CpRect[0].Side[nSide].fR);
	if(m_nSlowAxis == 15) {
		fR = fabs(m_CpRect[1].Side[1].fR - m_CpRect[0].Side[1].fR);
	};

	m_fFastScanSize = fabs(m_CpRect[1].Side[nSide].fPos[nFastAxis] - m_CpRect[0].Side[nSide].fPos[nFastAxis]);

	switch(m_nFastAxis) {
	case 0: m_fSlowScanSize = sqrtf(fY*fY + fZ*fZ + fR*fR);
		break;
	case 1: m_fSlowScanSize = sqrtf(fX*fX + fZ*fZ + fR*fR);
		break;
	case 2: m_fSlowScanSize = sqrtf(fX*fX + fY*fY);
		break;
	}


	m_nScanLineL = (int)(m_fSlowScanSize / m_fSlowIncrement);

	if(m_nScanLineL) {
		m_ScanLine = new CPolyCoord[m_nScanLineL];
	}

	theApp.m_Kinematics.NormalsFromTips(&m_CpRect[0],PORTSIDE);
	theApp.m_Kinematics.NormalsFromTips(&m_CpRect[0],STARBOARD);
	theApp.m_Kinematics.NormalsFromTips(&m_CpRect[1],PORTSIDE);
	theApp.m_Kinematics.NormalsFromTips(&m_CpRect[1],STARBOARD);

	CpBL = m_CpRect[0];
	CpTR = m_CpRect[1];


	switch(m_nFastAxis) {
	case 0:
		if(CpBL.Side[nSide].fX > CpTR.Side[nSide].fX) {
			Cp = CpBL;
			CpBL = CpTR;
			CpTR = Cp;
		}
		CpBR = CpBL;
		CpTL = CpTR;
		CpBR.Side0.fX = CpTR.Side0.fX;
		CpBR.Side1.fX = CpTR.Side1.fX;
		CpTL.Side0.fX = CpBL.Side0.fX;
		CpTL.Side1.fX = CpBL.Side1.fX;
		break;
	case 1:
		if(CpBL.Side[nSide].fY > CpTR.Side[nSide].fY) {
			Cp = CpBL;
			CpBL = CpTR;
			CpTR = Cp;
		}
		CpBR = CpBL;
		CpTL = CpTR;
		CpBR.Side0.fY = CpTR.Side0.fY;
		CpBR.Side1.fY = CpTR.Side1.fY;
		CpTL.Side0.fY = CpBL.Side0.fY;
		CpTL.Side1.fY = CpBL.Side1.fY;
		break;
	case 2:
		if(CpBL.Side[nSide].fZ > CpTR.Side[nSide].fZ) {
			Cp = CpBL;
			CpBL = CpTR;
			CpTR = Cp;
		}
		CpBR = CpBL;
		CpTL = CpTR;
		CpBR.Side0.fZ = CpTR.Side0.fZ;
		CpBR.Side1.fZ = CpTR.Side1.fZ;
		CpTL.Side0.fZ = CpBL.Side0.fZ;
		CpTL.Side1.fZ = CpBL.Side1.fZ;
		break;
	}

	Cp0 = CpBL;
	Cp1 = CpBR;
	for(nLine = 0;nLine<m_nScanLineL;nLine++) {
		for(nAxis=0;nAxis<30;nAxis++) {
			Cp0.fPos[nAxis] = ((float)nLine * (CpTL.fPos[nAxis] - CpBL.fPos[nAxis]) / (float)m_nScanLineL) + CpBL.fPos[nAxis];
			Cp1.fPos[nAxis] = ((float)nLine * (CpTR.fPos[nAxis] - CpBR.fPos[nAxis]) / (float)m_nScanLineL) + CpBR.fPos[nAxis];
		}
		m_ScanLine[nLine].SetPrimaryAxis(m_nFastAxis);
		m_ScanLine[nLine].Add(Cp0);
		m_ScanLine[nLine].Add(Cp1);
	}

	if((m_nSlowAxis==5) ||(m_nSlowAxis==15)) {
		theApp.m_Thread.m_nRevCounter = theApp.m_Thread.m_nScanLine = m_nScanStartLine = 0;
		theApp.m_Motors.SetRevCount(theApp.m_Thread.m_nRevCounter);
	}

	m_nWayPointNumber=2;

	CalculateDangerZone();

LCleanReturn:
    return hr;

}


void CProfile::BisectAndAddLine(int nLineType, int nLine)
{
	CPolyCoord Line;
	CCoord Cp0,Cp1,Cp;
	int nPtr,nn;
	float fPos;

	for(nPtr = 0; nPtr < m_TaughtLine[nLine].GetSize(); nPtr++) {
		
		m_TaughtLine[nLine].GetCoord(nPtr,&Cp0);

		fPos = Cp0.Side0.fPos[m_nFastAxis];

		if((nPtr==0) || (nPtr == (m_TaughtLine[nLine].GetSize()-1))) {
			m_TaughtLine[nLine+1].GetCoord(nPtr,&Cp1);
		} else {
			m_TaughtLine[nLine+1].CalculateCoord(fPos, Cp1);
		}

		Cp = Cp0;
		for(nn=0;nn<30;nn++) {
			Cp.fPos[nn] = (Cp1.fPos[nn] + Cp0.fPos[nn]) / 2.0f;
		}
		Line.Add(Cp);
	}

	AddLine(nLineType,&Line,m_nSlowAxis);

}

void CProfile::CalculateLiftOffPointsForWayPoints(int nLine0,int nNumberLines, int nDir, CPolyCoord *pLine)
{
	float fX0,fX1,fZ0,fZ1,fTemp,fZ,fResult;
	int nPtr,ii,jj,nLoop,nLine1,nDest;
	CCoord CpStart,CpFinish;
	bool bOffOn[3];

	if(m_nWayPointsWithLiftOff != 1) return;
	if(m_EnvelopeLine[0].GetSize()<3) return;
	if(pLine == NULL) return;
	
	nLine1 = nNumberLines+nLine0;
	MinMax(&nLine1,0,m_nWayPointsL-1);

	if(nDir == 0) {

		for(nPtr=0;nPtr<(pLine->GetSize()-1);nPtr++) {
			bOffOn[0] = bOffOn[1] = bOffOn[2] = true;

			for(nLoop = 0; nLoop<2; nLoop++) {
				switch(nLoop) {
				case 0:
					m_WayPoints[nLine0].GetCoord(nPtr,  &CpStart);
					m_WayPoints[nLine0].GetCoord(nPtr+1,&CpFinish);
					break;
				case 1:
					m_WayPoints[nLine1].GetCoord(nPtr,  &CpStart);
					m_WayPoints[nLine1].GetCoord(nPtr+1,&CpFinish);
					break;
				}

				fZ = CpStart.fPos[m_nSlowAxis];
				for(ii=0,jj=1;ii<m_EnvelopeLine[0].GetSize();ii++,jj++) {
					jj%=m_EnvelopeLine[0].m_nCoordL;

					fX0=m_EnvelopeLine[0].m_pCp[ii].fPos[m_nFastAxis];
					fX1=m_EnvelopeLine[0].m_pCp[jj].fPos[m_nFastAxis];
					fZ0=m_EnvelopeLine[0].m_pCp[ii].fPos[m_nSlowAxis];
					fZ1=m_EnvelopeLine[0].m_pCp[jj].fPos[m_nSlowAxis];
					if(fZ0>fZ1) {
						fTemp=fZ0;
						fZ0=fZ1;
						fZ1=fTemp;
					};
					if((fZ>=fZ0) && (fZ<=fZ1)) {
						fResult=(fZ-m_EnvelopeLine[0].m_pCp[ii].fPos[m_nSlowAxis])*(m_EnvelopeLine[0].m_pCp[jj].fPos[m_nFastAxis]-m_EnvelopeLine[0].m_pCp[ii].fPos[m_nFastAxis]);
						fResult/=(m_EnvelopeLine[0].m_pCp[jj].fPos[m_nSlowAxis]-m_EnvelopeLine[0].m_pCp[ii].fPos[m_nSlowAxis]);
						fResult += m_EnvelopeLine[0].m_pCp[ii].fPos[m_nFastAxis];
						if((fResult>=CpStart.fPos[m_nFastAxis]) && (fResult<=CpFinish.fPos[m_nFastAxis])) {
							bOffOn[nLoop] == true ? bOffOn[nLoop] = false : bOffOn[nLoop] = true;
						};
					};
				};

			}
			if((bOffOn[0] == false) && (bOffOn[1] == false)) {
				nDest = pLine->FindNearestCoord(CpFinish.fPos[m_nFastAxis],nPtr);
				pLine->m_pCp[nDest].Side0.pt.x += (theApp.m_LastSettings.fDangerMargin[0] * pLine->m_pCp[nDest].Side0.norm.x);
				pLine->m_pCp[nDest].Side0.pt.y += (theApp.m_LastSettings.fDangerMargin[0] * pLine->m_pCp[nDest].Side0.norm.y);
				pLine->m_pCp[nDest].Side0.pt.z += (theApp.m_LastSettings.fDangerMargin[0] * pLine->m_pCp[nDest].Side0.norm.z);
			}
		}

	} else {


	}
}

void CProfile::GenerateRRArcScanLines()
{
	CCoord Cp;
	CPolyCoord *OldTaughtLine = new CPolyCoord[m_nTaughtLineL];
	int	nOldTaughtLineL = m_nTaughtLineL;
	int	nLine;

	for(nLine=0;nLine<m_nTaughtLineL;nLine++) {
		OldTaughtLine[nLine] = &m_TaughtLine[nLine];
	}
	SAFE_DELETE_ARRAY( m_TaughtLine );

	m_TaughtLine = new CPolyCoord[m_nTaughtLineL = 3];


	m_TaughtLine[0] = &OldTaughtLine[theApp.m_nLine];
	m_TaughtLine[1] = &OldTaughtLine[theApp.m_nLine];
	m_TaughtLine[2] = &OldTaughtLine[theApp.m_nLine];

	OldTaughtLine[theApp.m_nLine].GetFirstCoord(&Cp);
	Cp.Side0.fZ -= 100.0f;
	m_TaughtLine[0].SetAllfPos(&Cp,2);
	Cp.Side0.fZ += 200.0f;
	m_TaughtLine[2].SetAllfPos(&Cp,2);

	GenerateScanLinesScanParallelToTaughtUsingAxis30(2.0f);
//	GenerateScanLinesScanParallelToTaughtUsingFastAxis();

	SAFE_DELETE_ARRAY( m_TaughtLine );
	m_TaughtLine = new CPolyCoord[m_nTaughtLineL = nOldTaughtLineL];
	for(nLine=0;nLine<m_nTaughtLineL;nLine++) {
		m_TaughtLine[nLine] = &OldTaughtLine[nLine];
	};

	SAFE_DELETE_ARRAY(OldTaughtLine);
}

void CProfile::ImportCADRefPts(CUSLFile *pFile)
{
	CString Buff;
	char Array[256];
	int	ii,jj;
	char *pText = NULL;
	int nCoordL = 0;

	ULONGLONG nFileLength = pFile->GetLength();

	if(nFileLength > 0) {
		char*	pText = new char[(unsigned int)nFileLength+1];
		pFile->Read(pText,(unsigned int)nFileLength);

		for(ii=0,jj=0;ii<nFileLength;ii++,jj++) {

			Array[jj]=pText[ii];
			if((pText[ii]==0x0a) || (pText[ii]==0x0d) || (ii==nFileLength-1)) {
				Array[jj]=0;
				if(strlen(Array)>1) {
					Buff.Format(_T("%s"), (CString) Array);
					Buff.TrimRight();
					swscanf_s(Buff.GetBuffer(200),L"%f %f %f",&m_CpCadHole[nCoordL].Side0.fX,&m_CpCadHole[nCoordL].Side0.fY,&m_CpCadHole[nCoordL].Side0.fZ);
					nCoordL++;
				}
				jj=-1;
			}
		}



	}
	SAFE_DELETE( pText );
}

void CProfile::GuessCADNorms()
{
	int nCoord;
	int nLineType;
	int nLine,nIndex;
	CCoord Cp;

	if(m_nScanLineL>1) {
		nLineType = SCANLINES;
	} else {
		if(m_nTaughtLineL>1) {
			nLineType = TAUGHTLINES;
		} else {
			return;
		}
	}

	for(nCoord = 0;nCoord<3;nCoord++) {

		FindNearestCoordinate(nLineType, m_CpCadHole[nCoord], &nLine, &nIndex);

		switch(nLineType) {
		case TAUGHTLINES: m_TaughtLine[nLine].GetCoord(nIndex,&Cp);
			break;
		case SCANLINES: m_ScanLine[nLine].GetCoord(nIndex,&Cp);
			break;
		}
		m_CpCadHole[nCoord].Side0.norm = Cp.Side0.norm;
	}



}

void CProfile::GenerateRaftWayPoints(CPolyCoord *pLine,int nScanLine, int nNumberScanLines)
{
	int nIndex,jj;
	int nLine0 = nScanLine;
	int nLine1 = nScanLine + nNumberScanLines;
	CCoord Cp0,Cp1,Cp2,Cp;
	CPolyCoord AllPoints;
	float fCosAngle;
	bool	bPresentlyInside[2];


	MinMax(&nLine0,0,m_nScanLineL-1);
	MinMax(&nLine1,0,m_nScanLineL-1);

	pLine->Zero();
	AllPoints.Zero();
	AllPoints.AllocateCoords(m_ScanLine[nLine0].GetSize());

	bPresentlyInside[0] = bPresentlyInside[1] = false;
	for(nIndex=0;nIndex<m_ScanLine[nLine0].GetSize();nIndex++) {
		m_ScanLine[nLine0].GetCoordComeWhatMay(nIndex,&Cp0);
		m_ScanLine[nLine1].GetCoordComeWhatMay(nIndex,&Cp1);
/*
		if(IsCoordinateInsideEnvelope(nLine0,nIndex,&bPresentlyInside[0]) && IsCoordinateInsideEnvelope(nLine1,nIndex,&bPresentlyInside[1])) {

		} else {
			Cp0.Side0.fX += (theApp.m_LastSettings.fDangerMargin[0] * Cp0.Side0.fI);
			Cp0.Side0.fY += (theApp.m_LastSettings.fDangerMargin[0] * Cp0.Side0.fJ);
			Cp0.Side0.fZ += (theApp.m_LastSettings.fDangerMargin[0] * Cp0.Side0.fK);
		}
		*/
		AllPoints.ModifyCoord(nIndex,Cp0);
	}


	//Decimate pts
	AllPoints.GetFirstCoord(&Cp0);
	pLine->Add(Cp0);
	for(nIndex=1;nIndex<AllPoints.GetSize();nIndex++) {
		AllPoints.GetCoord(nIndex,&Cp1);

		fCosAngle = fabsf( GetCosAngleDeviation(Cp0,Cp1));
		if(fCosAngle <= 0.9999f) {
			pLine->Add(Cp1);
			Cp0 = Cp1;
		} else {

			for(++nIndex,jj=0; nIndex<AllPoints.GetSize();nIndex++,jj++) {
				AllPoints.GetCoord(nIndex,&Cp2);

				fCosAngle = fabsf( GetCosAngleDeviation(Cp1,Cp2));
				if(jj>20) {
					pLine->Add(Cp2);
					Cp0 = Cp2;
					break;
				}
				if(fCosAngle < 0.9999f) {
					pLine->Add(Cp1);
					pLine->Add(Cp2);
					Cp0 = Cp2;
					break;
				} else {
					Cp1 = Cp2;
				}
			}
		}
	}
	AllPoints.GetLastCoord(&Cp0);
	pLine->Add(Cp0);
	pLine->Sort(m_nFastAxis,0);
}	


float CProfile::GetCosAngleDeviation(CCoord Cp0,CCoord Cp1)
{
	D3DXVECTOR4 vecOA,vecOB;
	float fCosDev;

	vecOA = Cp0.Side0.norm;
	vecOB = Cp1.Side0.norm;
	D3DXVec4Normalize(&vecOA,&vecOA);
	D3DXVec4Normalize(&vecOB,&vecOB);
	fCosDev =  D3DXVec4Dot(&vecOA,&vecOB);

	return fCosDev;
}

bool CProfile::IsCoordinateInsideEnvelope(int nLine,int nIndex,bool *pbPresentlyInside)
{
	float	fX0,fX1,fZ0,fZ1,fTemp,fResult;
	int	ii,jj;
	CCoord Cp0,Cp1;
	float fPosZ;


	if(m_EnvelopeLine[0].m_nCoordL>2) {
		if(nIndex>0) {
			m_ScanLine[nLine].GetCoord(nIndex-1,&Cp0);
		} else {
			Cp0.fPos[m_nSlowAxis] = -10000.0f;
			Cp0.fPos[m_nFastAxis] = -10000.0f;
		}
		m_ScanLine[nLine].GetCoord(nIndex,&Cp1);
		fPosZ = Cp1.Side0.fZ;

		for(ii=0,jj=1;ii<m_EnvelopeLine[0].m_nCoordL;ii++,jj++) {
			jj%=m_EnvelopeLine[0].m_nCoordL;

			fX0=m_EnvelopeLine[0].m_pCp[ii].fPos[m_nFastAxis];
			fX1=m_EnvelopeLine[0].m_pCp[jj].fPos[m_nFastAxis];
			fZ0=m_EnvelopeLine[0].m_pCp[ii].fPos[m_nSlowAxis];
			fZ1=m_EnvelopeLine[0].m_pCp[jj].fPos[m_nSlowAxis];
			if(fZ0>fZ1) {
				fTemp=fZ0;
				fZ0=fZ1;
				fZ1=fTemp;
			};
			if(fX0>fX1) {
				fTemp=fX0;
				fX0=fX1;
				fX1=fTemp;
			};
			if((fPosZ>=fZ0) && (fPosZ<=fZ1)) {
				fResult=(fPosZ-m_EnvelopeLine[0].m_pCp[ii].fPos[m_nSlowAxis])*(m_EnvelopeLine[0].m_pCp[jj].fPos[m_nFastAxis]-m_EnvelopeLine[0].m_pCp[ii].fPos[m_nFastAxis]);
				fResult/=(m_EnvelopeLine[0].m_pCp[jj].fPos[m_nSlowAxis]-m_EnvelopeLine[0].m_pCp[ii].fPos[m_nSlowAxis]);
				fResult += m_EnvelopeLine[0].m_pCp[ii].fPos[m_nFastAxis];
				if((fX0 <= fResult) && (fResult<= fX1)) {
					*pbPresentlyInside == true ? *pbPresentlyInside = false : *pbPresentlyInside = true;
					break;
				}

			};
		};
	}
	return *pbPresentlyInside;
}


void CProfile::InsertExtraScanLines(int nNumberExtraLines)
{
	CPolyCoord *pLine = NULL;
	int nLine;
	CCoord Cp0,Cp1;

	m_ScanLine[0].GetFirstCoord(&Cp0);
	m_ScanLine[1].GetFirstCoord(&Cp1);
	m_fSlowIncrement = Cp1.Side0.fY - Cp0.Side0.fY;

	pLine = new CPolyCoord[m_nScanLineL];
	for(nLine = 0;nLine < m_nScanLineL; nLine++) pLine[nLine] = &m_ScanLine[nLine];
	SAFE_DELETE_ARRAY( m_ScanLine );

	m_nScanLineL *= 2;
	m_fSlowIncrement /= 2.0f;
	
	m_ScanLine = new CPolyCoord[m_nScanLineL];
	for(nLine=0;nLine<m_nScanLineL;nLine++) {
		m_ScanLine[nLine] = &pLine[nLine/2];

		if((nLine % 2) == 1) {
			m_ScanLine[nLine].Add(1,m_fSlowIncrement);
		}
	}


	SAFE_DELETE_ARRAY(pLine);

}

void CProfile::GenerateInterpolatedLines()
{
	COORDDATA Cp0,Cp1;
	CCoord Sp0,Sp1,Sp,CpFirst0,CpLast0,CpFirst1,CpLast1;
	float fVertical,fPos, fFraction;
	int nLine,nDestLine,nIndex,nAxis;
	
	SAFE_DELETE(m_ScanLine);

	FindAllLineLimits(&Cp0, &Cp1);

	float fSlowScanSize = Cp1.fPos[m_nSlowAxis] - Cp0.fPos[m_nSlowAxis];

	if(fSlowScanSize <= 0) {
		MessageBox(NULL,L"Please reorder lines",_T("Error"),MB_ICONERROR);
		return;
	}

	m_nScanLineL = (int)(fSlowScanSize / m_fSlowIncrement) * 2;
	m_ScanLine = new CPolyCoord[m_nScanLineL];


	for(nLine=0;nLine<m_nTaughtLineL;nLine++) {
		m_TaughtLine[nLine].SetPrimaryAxis(m_nFastAxis);
		m_TaughtLine[nLine].SetExtrapolation(EOL_NO_EXTRAPOLATION);
	}

	Sp.Zero();
	nDestLine = 0;
	for(fVertical = Cp0.fPos[m_nSlowAxis]; fVertical <= Cp1.fPos[m_nSlowAxis]; fVertical+=m_fSlowIncrement) {

		for(nLine=0;nLine<(m_nTaughtLineL - 1);nLine++) {
			m_TaughtLine[nLine].GetFirstCoord(&Sp0);
			m_TaughtLine[nLine+1].GetFirstCoord(&Sp1);

			if(fVertical>=Sp0.fPos[m_nSlowAxis] && fVertical <= Sp1.fPos[m_nSlowAxis]) {

				fFraction = (fVertical-Sp0.fPos[m_nSlowAxis]) / (Sp1.fPos[m_nSlowAxis] - Sp0.fPos[m_nSlowAxis]);

				m_TaughtLine[nLine].GetFirstCoord(&CpFirst0);
				m_TaughtLine[nLine].GetLastCoord(&CpLast0);
				m_TaughtLine[nLine+1].GetFirstCoord(&CpFirst1);
				m_TaughtLine[nLine+1].GetLastCoord(&CpLast1);

				if(CpFirst1.fPos[m_nFastAxis] > CpFirst0.fPos[m_nFastAxis]) CpFirst0.fPos[m_nFastAxis] = CpFirst1.fPos[m_nFastAxis];
				if(CpLast1.fPos[m_nFastAxis] > CpLast0.fPos[m_nFastAxis]) CpLast0.fPos[m_nFastAxis] = CpLast1.fPos[m_nFastAxis];

				for(nIndex=0;nIndex<200;nIndex++ ) {

					fPos = ((CpLast0.fPos[m_nFastAxis] - CpFirst0.fPos[m_nFastAxis]) * (float)nIndex / 199.0f) + CpFirst0.fPos[m_nFastAxis];
					m_TaughtLine[nLine].CalculateCoord(fPos,Sp0);
					if(m_TaughtLine[nLine+1].CalculateCoord(fPos,Sp1) == true) {

						for(nAxis=0;nAxis<10;nAxis++) {
							Sp.fPos[nAxis] = ( (Sp1.fPos[nAxis] - Sp0.fPos[nAxis]) * fFraction) + Sp0.fPos[nAxis];
						}
						Sp.Side0.Normalize();
						Sp.Side1 = Sp.Side0;
						m_ScanLine[nDestLine].Add(Sp);
					}
				}
				break;
			}
		}
		if(m_ScanLine[nDestLine].GetSize() > 0) nDestLine++;
	}
	m_nScanLineL = nDestLine;

}

bool CProfile::LoadDesiredUtFile()
{
	CString Buff;
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CUSLFile File;
	CFileException e;

	if(!File.Open(m_UltrasonicPathFileName,CFile::modeReadWrite|CFile::typeBinary,&e)) {
		File.DecodeFileException(&e,&Buff,m_UltrasonicPathFileName);
		MessageBox(NULL,Buff,_T("Error"),MB_ICONERROR);
		return false;
	} else {
		theApp.m_UtUser.Retrieve(&File);
		File.Close();
		theApp.m_UtUser.ResetMaxTimeSlots();
		theApp.m_UtUser.Initialize(theApp.m_nJobNumber);
		pFrame->RedrawUltrasonicsSheet();
	}

	return true;
}

void CProfile::GenerateRotoSymetricScanLineV1()
{
	////???????
	CCoord Cp0,Cp1,Cp;
	float fLength = m_TaughtLine[0].GetLength();
	float fX,fY,fZ,fMin,fMax;
	int nn,nNumberLines;

	SAFE_DELETE_ARRAY(m_ScanLine);
	m_nScanLineL=0;
	int nSide = 0;

	int nPrimaryAxis = 0;
	fX=(float)fabs(m_TaughtLine[0].m_pCp[0].Side[nSide].fX - m_TaughtLine[0].m_pCp[m_TaughtLine[0].m_nCoordL-1].Side[nSide].fX);
	fY=(float)fabs(m_TaughtLine[0].m_pCp[0].Side[nSide].fY - m_TaughtLine[0].m_pCp[m_TaughtLine[0].m_nCoordL-1].Side[nSide].fY);
	fZ=(float)fabs(m_TaughtLine[0].m_pCp[0].Side[nSide].fZ - m_TaughtLine[0].m_pCp[m_TaughtLine[0].m_nCoordL-1].Side[nSide].fZ);
	fMin=m_TaughtLine[0].m_pCp[0].Side[nSide].fX;
	fMax=m_TaughtLine[0].m_pCp[m_TaughtLine[0].m_nCoordL-1].Side[nSide].fX;
	if(fY>fX) {
		nSide==0 ? nPrimaryAxis = 1 : nPrimaryAxis = 11;
		fMin=m_TaughtLine[0].m_pCp[0].Side[nSide].fY;
		fMax=m_TaughtLine[0].m_pCp[m_TaughtLine[0].m_nCoordL-1].Side[nSide].fY;
		if(fZ>fY) {
			nSide==0 ? nPrimaryAxis = 2 : nPrimaryAxis = 12;
			fMin=m_TaughtLine[0].m_pCp[0].Side[nSide].fZ;
			fMax=m_TaughtLine[0].m_pCp[m_TaughtLine[0].m_nCoordL-1].Side[nSide].fZ;
		}
	} else {
		if(fZ>fX) {
			nSide==0 ? nPrimaryAxis = 2 : nPrimaryAxis = 12;
			fMin=m_TaughtLine[0].m_pCp[0].Side[nSide].fZ;
			fMax=m_TaughtLine[0].m_pCp[m_TaughtLine[0].m_nCoordL-1].Side[nSide].fZ;
		}
	}
	m_TaughtLine[0].SetPrimaryAxis(nPrimaryAxis);

	if(m_TaughtLine[0].m_nStyle == 0) {
		m_TaughtLine[0].SetPrimaryAxis(30);
		fLength=m_TaughtLine[0].GetLength();
		nNumberLines = (int)(fLength / m_fSlowIncrement) + 1;
		m_ScanLine = new CPolyCoord[m_nScanLineL=1];
		Cp0=m_TaughtLine[0].m_pCp[0];
		Cp0.Side0.fR = 0.0f;
		m_ScanLine[0].Add(Cp0);
		for(nn=1;nn<nNumberLines;nn++) {
			m_TaughtLine[0].CalculateCoord(m_fSlowIncrement * (float)nn,Cp1);
			Cp1.Side0.fR = 0.0f;
			m_ScanLine[0].Add(Cp1);
			Cp0=Cp1;
		}

	} else {
		fLength=m_TaughtLine[0].GetSplineLength(200);
		nNumberLines = (int)(fLength / m_fSlowIncrement) + 1;
		m_ScanLine = new CPolyCoord[m_nScanLineL=1];
		Cp0=m_TaughtLine[0].m_pCp[0];
		Cp0.Side0.fR = 0.0f;
		m_ScanLine[0].Add(Cp0);
		for(nn=1;nn<nNumberLines;nn++) {
			m_TaughtLine[0].CalculateCoord(Cp0,m_fSlowIncrement,&Cp1);
			Cp1.Side0.fR = 0.0f;
			m_ScanLine[0].Add(Cp1);
			Cp0=Cp1;
		}

	}

	if(m_nComputeAlgorithm == 1) {		//remove vertices at ump
		for(nn=0;nn<m_ScanLine[0].GetSize();nn++) {
			m_ScanLine[0].GetCoord(nn,&Cp);
			if(Cp.nType == 2) {
				m_ScanLine[0].DeleteCoord(nn);
				nn=-1;
			}
		}
	}

	m_nComponentDescription = CIRCULAR;

	m_bModified = TRUE;

}

void CProfile::GenerateRotoSymetricScanLineV2()
{
	int nSide = 0;
	int nIndex = 0;
	CCoord Cp0,Cp1;
	CPolyCoord Line;
	float fX = 0.0f;
	float fLength = 0.0f;

	SAFE_DELETE_ARRAY(m_ScanLine);
	m_ScanLine = new CPolyCoord[m_nScanLineL=1];


	for(nIndex=0;nIndex<m_TaughtLine[0].m_nCoordL;) {
		m_TaughtLine[0].GetCoord(nIndex, &Cp0);
		Line.Zero();

		for(nIndex;nIndex<m_TaughtLine[0].m_nCoordL;nIndex++) {
			m_TaughtLine[0].GetCoord(nIndex, &Cp1);
			Cp1.Side0.fR = 0.0f;
			Line.Add(Cp1);
			if(Cp1.nType - Cp0.nType) break;
		}
		switch(Cp0.nType & TYPE_SPLINE) {
		case 0:
			Line.SetStyle(0);
			Line.SetPrimaryAxis(30);
			break;
		case TYPE_SPLINE:
			Line.SetStyle(1);
			Line.SetPrimaryAxis(30);
			break;
		}
		fLength = Line.GetLength();

		for(fX ; fX< fLength; fX+=m_fSlowIncrement) {
			Line.CalculateCoord(fX,Cp1);
			m_ScanLine[0].Add(Cp1);
		}
		fX-=fLength;
	}

}

void CProfile::SetScanStartTime()
{

	m_StartTime = CTime::GetCurrentTime();

}

float CProfile::CalculateLineScanTime(int nLine)
{
	CCoord Cp0,Cp1;
	float fS0,fS3,fS;
	float fT3;

	m_ScanLine[nLine].GetCoord(0,&Cp0);
	m_ScanLine[nLine].GetLastCoord(&Cp1);
	fS0 = Cp0.fPos[m_nFastAxis];
	fS3 = Cp1.fPos[m_nFastAxis];
	fS = fabsf(fS3 - fS0);

	switch(theApp.m_Thread.m_nThreadAction) {
	case ROBOT_CONTOUR_SCAN:
	case ROBOT_ROLLER_PROBE_SCAN:
		fT3 = (fS / m_fScanSpeed) + 1.5f;		//1.25f to add a bit for acceleration deceleration
		break;
	default:
		float fAcceleration = theApp.m_Axes[m_nFastAxis].fAcceleration * 1e6f * theApp.m_Axes[m_nFastAxis].fStepSize;
		float fVelocity = m_fScanSpeed;

		float fAccelTime = fVelocity / fAcceleration;

		float fS1 = 0.5f * fAcceleration * (fAccelTime * fAccelTime);
		if((fS1 * 2) > fS) {
			fS1 = fS / 2.0f;
			fAccelTime = sqrtf((2.0f * fS1) / fAcceleration);
			fT3= fAccelTime * 2;
		} else {
			float fDS = (fS - (fS1 * 2.0f));
			float fDT = fDS / fVelocity;
			fT3 = fAccelTime * 2.0f + fDT;
		};
		break;
	}

	if (isnan(fT3)) fT3 = 10.0f;

	return fT3;
}


bool CProfile::IsCorrectOneLoaded(CString *pDateProfileName)
{
	if(m_FileName == *pDateProfileName) {
		return true;
	}
	return false;
}

int CProfile::GetNumberLines(int nLineType)
{
	switch (nLineType) {
	case taught:	return m_nTaughtLineL;
		break;
	case scan:	return m_nScanLineL;
		break;
	case brane: return m_nBrainLineL;
		break;
	case envelope:	return m_nEnvelopeLineL;
		break;
	case reference:	return 0;
		break;
	}
	return 0;
}

CPolyCoord* CProfile::GetLineElement(LineTypes nLineType, int nLine)
{
	int nType;
	if (nLine < 0) return NULL;

	if (nLineType >= 0) {
		nType = nLineType;
	}
	else {
		nType = theApp.m_nEditLineType;
	}

	switch (nType) {
	case taught: if (m_TaughtLine && (nLine < m_nTaughtLineL)) return &m_TaughtLine[nLine];
		break;
	case scan: if (m_ScanLine && (nLine < m_nScanLineL)) return &m_ScanLine[nLine];
		break;
	case brane: if (m_BrainLine && (nLine < m_nBrainLineL)) return &m_BrainLine[nLine];
		break;
	case envelope: if (m_EnvelopeLine && (nLine < m_nEnvelopeLineL)) return &m_EnvelopeLine[nLine];
		break;
	}
	return NULL;
}

CCoord& CProfile::GetRefCoord(int nIndex)
{
	switch (nIndex) {
	case 0:
	case 1:
	case 2: return m_CpMeasuredHole[nIndex];
		break;
	case 3:
	case 4:
	case 5: return m_CpCadHole[nIndex - 3];
		break;
	}
	return m_CpMeasuredHole[0];
}

void CProfile::SetRefCoord(int nIndex, CCoord Cp)
{
	switch (nIndex) {
	case 0:
	case 1:
	case 2: m_CpMeasuredHole[nIndex] = Cp;
		break;
	case 3:
	case 4:
	case 5: m_CpCadHole[nIndex - 3] = Cp;
		break;
	}
}

XMFLOAT3& CProfile::getDangerPlaneGlassSize(int nSide)
{
	static XMFLOAT3 vSize;

	FindScanSizes(&m_fFastScanSize, &m_fSlowScanSize, &m_CpMin, &m_CpMax);

	if (nSide == 0) {
		vSize = XMFLOAT3(m_CpMax.Side0.fX - m_CpMin.Side0.fX, m_CpMax.Side0.fY - m_CpMin.Side0.fY, m_CpMax.Side0.fZ - m_CpMin.Side0.fZ);
	}
	else {
		vSize = XMFLOAT3(m_CpMax.Side1.fX - m_CpMin.Side1.fX, m_CpMax.Side1.fY - m_CpMin.Side1.fY, m_CpMax.Side1.fZ - m_CpMin.Side1.fZ);
	}

	switch (m_eDangerPlaneType[nSide]) {
	case x_plane_p:
	case x_plane_n: vSize.x = 10.0f;
		break;
	case y_plane_n:
	case y_plane_p:	vSize.y = 10.0f;
		break;
	case z_plane_n:
	case z_plane_p: vSize.z = 10.0f;
		break;
	}

	return vSize;
}

XMFLOAT3& CProfile::getOverallSize()
{
	static XMFLOAT3 vSize;

	vSize = XMFLOAT3(m_CpMax.Side0.fX - m_CpMin.Side0.fX, m_CpMax.Side0.fY - m_CpMin.Side0.fY, m_CpMax.Side0.fZ - m_CpMin.Side0.fZ);
	return vSize;
}

bool CProfile::getNeedsRendering()
{
	return m_bNeedsRendering;
}
bool CProfile::setNeedsRendering(bool bFlag)
{
	return m_bNeedsRendering = bFlag;
}

bool CProfile::GenerateSingleLineBScan(int nLineNumber)
{
	bool bRet = true;
	CCoord Cp0, Cp1, Cp;
	CPolyCoord Line;

	if (theApp.m_LSA.m_nActualLawsL <= 0 || theApp.m_LSA.m_nActualLawsL >= 256) {
		AfxMessageBox(L"Incorrect number of focallaws");
		return  false;
	}

	SAFE_DELETE_ARRAY(m_ScanLine);
	m_nNumberProbesPerImage = theApp.m_LastSettings.m_nNumberProbesPerImage = theApp.m_LSA.m_nActualLawsL;
	m_nScanLineL = m_nNumberProbesPerImage;

	m_ScanLine = new CPolyCoord[m_nScanLineL];

	int nNumberCoords = 200;
	m_TaughtLine[nLineNumber].GetFirstCoord(&Cp0);
	m_TaughtLine[nLineNumber].GetLastCoord(&Cp1);


	for (int nLine = 0; nLine<m_nScanLineL; nLine++) {
		Line.Zero();
		if ((Cp1.fPos[m_nFastAxis] - Cp0.fPos[m_nFastAxis]) > 2.0f) {
			for (int nn = 0; nn < nNumberCoords; nn++) {
				float fFast = (((float)nn * (Cp1.fPos[m_nFastAxis] - Cp0.fPos[m_nFastAxis])) / (float)(nNumberCoords - 1)) + Cp0.fPos[m_nFastAxis];
				m_TaughtLine[nLineNumber].CalculateCoord(fFast, Cp);
				Cp.Side0.pt += theApp.m_PhasedArray[PORTSIDE].m_FL[nLine].vecPt;
				Cp.Side1.pt += theApp.m_PhasedArray[STARBOARD].m_FL[nLine].vecPt;
				Line.Add(Cp);
			}
		}
		m_ScanLine[nLine] = &Line;
	}


	return bRet;
}

void CProfile::GenerateTubeScan(int nPrimaryAxis)
{
	CCoord Cp[2];
	float fR,fRStart,fRFinish;
	int nLine, nAxis, nNumberLines;
	float fLength = 0.0f;
	int gg;

	if ((m_fFastIncrement <= 0.0) || (m_fSlowIncrement <= 0.0)) {
		MessageBox(NULL, _T("Scan increments are incorrect"), _T("Error"), MB_ICONERROR);
		return;
	}

	SAFE_DELETE_ARRAY(m_ScanLine);
	m_nScanLineL = 0;
	m_nComponentDescription = CIRCULAR;

	switch (nPrimaryAxis) {
	case 0:	//Cartesian step rotate
		m_fFastScanSize = m_CpRect[0].Length(m_CpRect[1], 0);
		m_fSlowScanSize = fabsf(m_CpRect[0].Side0.fR - m_CpRect[1].Side0.fR);
		fRStart = m_CpRect[0].Side0.fR;
		fRFinish = m_CpRect[1].Side0.fR;
		break;
	case 1: //Rotate continuous step cartesian
		fRStart = 0.0f;
		fRFinish = 360.0f;
		m_fFastScanSize = 360.0f;
		m_fSlowScanSize = m_CpRect[0].Length(m_CpRect[1], 0);
		m_nFastAxis = 5;
		if (fabs(m_CpRect[0].Side0.fX - m_CpRect[1].Side0.fX) > fLength) {
			fLength = fabs(m_CpRect[0].Side0.fX - m_CpRect[1].Side0.fX);
			m_nSlowAxis = 0;
		}
		if (fabs(m_CpRect[0].Side0.fY - m_CpRect[1].Side0.fY) > fLength) {
			fLength = fabs(m_CpRect[0].Side0.fY - m_CpRect[1].Side0.fY);
			m_nSlowAxis = 1;
		}
		if (fabs(m_CpRect[0].Side0.fZ - m_CpRect[1].Side0.fZ) > fLength) {
			fLength = fabs(m_CpRect[0].Side0.fZ - m_CpRect[1].Side0.fZ);
			m_nSlowAxis = 2;
		}
		break;
	};

	Cp[0] = m_CpRect[0];
	Cp[1] = m_CpRect[1];

	MinMax(&m_fSlowIncrement, 0.01f, 10.0f);
	nNumberLines = (int)(m_fSlowScanSize / m_fSlowIncrement);
	if (nNumberLines) {

		switch (nPrimaryAxis) {
		case 0:
			m_ScanLine = new CPolyCoord[m_nScanLineL = nNumberLines];
			for (nLine = 0; nLine < m_nScanLineL; nLine++) {

				fR = ((fRFinish-fRStart) * (float)nLine / (float)(m_nScanLineL - 1)) + fRStart;
				Cp[0].Side0.fR = Cp[1].Side0.fR = fR;

				m_ScanLine[nLine].SetPrimaryAxis(m_nFastAxis = 0);
				m_ScanLine[nLine].Add(Cp[0]);
				m_ScanLine[nLine].Add(Cp[1]);
			}
			break;
		case 1:
			m_ScanLine = new CPolyCoord[m_nScanLineL = 1];
			m_ScanLine[0].SetPrimaryAxis(m_nSlowAxis);
			for (nLine = 0; nLine < nNumberLines; nLine++) {
				for (nAxis = 0; nAxis < 30; nAxis++) {
					Cp[0].fPos[nAxis] = ((float)nLine * (m_CpRect[1].fPos[nAxis] - m_CpRect[0].fPos[nAxis]) / (float)nNumberLines) + m_CpRect[0].fPos[nAxis];
				}
				Cp[0].Side0.fR = fRStart;
				for (gg = 0; gg<14; gg++) {
					Cp[0].nElec[gg] = (int)((float)nLine * (float)(m_CpRect[1].nElec[gg] - m_CpRect[0].nElec[gg]) / (float)nNumberLines) + m_CpRect[0].nElec[gg];
				}
				m_ScanLine[0].Add(Cp[0]);
			}
			break;
		};


	}
	else {
		return;
	}
}

void CProfile::ApplyAdditionalAngle(CCoord *pCp)
{
	D3DXMATRIXA16 matRotate;

	D3DXMatrixRotationX(&matRotate, PROFILE->m_vecAdditionalAngle[0].x*DEG_TO_RAD);
	D3DXVec3TransformCoord((D3DXVECTOR3*)&pCp->Side[PORTSIDE].norm, (D3DXVECTOR3*)&pCp->Side[PORTSIDE].norm, &matRotate);

}
