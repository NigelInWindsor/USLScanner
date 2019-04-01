// FieldBusController.cpp: implementation of the CFieldBusController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "FieldBusController.h"
#include "MainFrm.h"
#include "HRTimer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	_TERMINAL		0
#define	_BACKGROUND		1

#define DONT_FORCE	0
#define	FORCE		1

extern UINT FieldBusStartUpThread(LPVOID pParam);
extern UINT CommsThread(LPVOID pParam);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
bool nPositionMsgReceived = 0;


long FieldBusTerminalCallback(CString *pBuff)
{
	CString Temp;

	if(pBuff->GetLength()) {
		theApp.m_FBCtrl.AddRxStr(pBuff);
	}

	return 0;

}

long FieldBusBackGroundCallback(CString *pBuff)
{
	int nJointPos[14];
	static __int64 n64Status[14];
	int ii,nTotal,nStrLength;

	if(pBuff->GetLength()) {
		if(pBuff->GetAt(0) == '-' || isdigit(pBuff->GetAt(0))) {
			if(swscanf_s(pBuff->GetBuffer(256),L"%d %d %d %d %d %d %d %d %d %d %d %d",&nJointPos[0],&nJointPos[1],&nJointPos[2],&nJointPos[3],&nJointPos[4],&nJointPos[5],
																						&nJointPos[6],&nJointPos[7],&nJointPos[8],&nJointPos[9],&nJointPos[10],&nJointPos[11]) == 12) {
				if(theApp.m_LastSettings.nVirtualActualRobot) {
					theApp.m_Robot[0].m_nJointPos[0] = theApp.m_Axes[0].nPos = nJointPos[0];
					theApp.m_Robot[0].m_nJointPos[1] = theApp.m_Axes[1].nPos = nJointPos[1];
					theApp.m_Robot[0].m_nJointPos[2] = theApp.m_Axes[2].nPos = nJointPos[2];
					theApp.m_Robot[0].m_nJointPos[3] = theApp.m_Axes[3].nPos = nJointPos[3];
					theApp.m_Robot[0].m_nJointPos[4] = theApp.m_Axes[4].nPos = nJointPos[4];
					theApp.m_Robot[0].m_nJointPos[5] = theApp.m_Axes[5].nPos = nJointPos[5];
					theApp.m_Robot[1].m_nJointPos[0] = theApp.m_Axes[6].nPos = nJointPos[6];
					theApp.m_Robot[1].m_nJointPos[1] = theApp.m_Axes[7].nPos = nJointPos[7];
					theApp.m_Robot[1].m_nJointPos[2] = theApp.m_Axes[8].nPos = nJointPos[8];
					theApp.m_Robot[1].m_nJointPos[3] = theApp.m_Axes[9].nPos = nJointPos[9];
					theApp.m_Robot[1].m_nJointPos[4] = theApp.m_Axes[10].nPos = nJointPos[10];
					theApp.m_Robot[1].m_nJointPos[5] = theApp.m_Axes[11].nPos = nJointPos[11];

					for(ii=0;ii<6;ii++) {
						theApp.m_Robot[0].m_fJointAngle[ii] = (float)theApp.m_Robot[0].m_nJointPos[ii] * theApp.m_Axes[ii].fStepSize * DEG_TO_RAD;
						theApp.m_Robot[1].m_fJointAngle[ii] = (float)theApp.m_Robot[1].m_nJointPos[ii] * theApp.m_Axes[ii+6].fStepSize * DEG_TO_RAD;
					}
				}
			}
			nPositionMsgReceived = 1;
		}
		nStrLength = pBuff->GetLength();
		if((pBuff->GetAt(0) == '$') && (pBuff->Find('\r') > 1) && (nStrLength == 55)){
			pBuff->Replace(L"$",L"0x");
			nTotal = swscanf_s(pBuff->GetBuffer(512),L"%I64x %I64x %I64x %I64x %I64x %I64x %I64x %I64x %I64x %I64x %I64x %I64x %I64x %I64x",&n64Status[0],&n64Status[1],&n64Status[2],&n64Status[3],&n64Status[4],&n64Status[5],&n64Status[6],
																			&n64Status[7],&n64Status[8],&n64Status[9],&n64Status[10],&n64Status[11],&n64Status[12],&n64Status[13]);
			switch(nTotal) {
			case 14:
				for(ii=0;ii<14;ii++) {
					theApp.m_FBCtrl.m_n64Status[ii] = n64Status[ii];
				}
				nPositionMsgReceived = 0;
				break;
			case 3:
				for(ii=0;ii<3;ii++) {
					theApp.m_FBCtrl.m_n64CoordStatus[ii] = n64Status[ii];
				}
				nPositionMsgReceived = 0;
				break;
			}
			theApp.m_FBCtrl.m_nBkRxStr++;
		}

		theApp.m_FBCtrl.DecodeAllStrings(pBuff);

	}

	return 0;

}





CFieldBusController::CFieldBusController()
{

	m_bPeekCoordinateFound = false;

	Zero();

	m_bStartUpThreadEnabled = FALSE;
	m_bStartUpThreadFinished = FALSE;
	m_pStartUpThread = NULL;

	m_bCommsThreadEnabled = FALSE;
	m_bCommsThreadFinished = FALSE;
	m_pCommsThread = NULL;

	ZeroMemory(m_ArrayFlag, sizeof m_ArrayFlag);

	InitializeCriticalSection(&m_CriticalSection);

}

CFieldBusController::~CFieldBusController()
{

	SuspendStartUpThread();
	SuspendCommsThread();

//	if(m_FileDebug.m_hFile != CFile::hFileNull) {
//		m_FileDebug.Close();
//	}
}


int	CFieldBusController::getRxArraySize()
{
	return m_RxArray.GetSize();
}

CString &CFieldBusController::getRxArrayAt(int nIndex)
{
	static CString Buff;

	Buff.Empty();
	EnterCriticalSection(&m_CriticalSection);
	if (nIndex < m_RxArray.GetSize()) {
		Buff = m_RxArray.GetAt(nIndex);
	}
	LeaveCriticalSection(&m_CriticalSection);
	return Buff;
}

int CFieldBusController::GetRxFlagAt(int nIndex)
{
	return m_ArrayFlag[nIndex];
}

void CFieldBusController::AddRxStr(CString *pBuff)
{
	CString Buff;

	Buff = *pBuff;
	Buff.Remove(0x0d);
	Buff.Remove(0x0a);

	EnterCriticalSection(&m_CriticalSection);

	if(Buff.GetLength() > 0) {
		if(Buff.GetAt(0) != 6) {

			m_RxArray.Add(Buff);
			if (m_LastSendStr == Buff) {
				m_ArrayFlag[m_RxArray.GetSize() - 1] = TX_STR;
			}
			if (Buff.Find(L"error") >= 0) {
				m_ArrayFlag[m_RxArray.GetSize() - 1] = ERROR_STR;
			}
			if (m_RxArray.GetSize() > (MAX_NUMBER_STRINGS - 1000)) {
				m_RxArray.RemoveAt(0, 1000);
				CopyMemory(m_ArrayFlag, &m_ArrayFlag[1000], MAX_NUMBER_STRINGS - 1000);
			}
		}
	}
	LeaveCriticalSection(&m_CriticalSection);


}

void CFieldBusController::SendStr(CString Buff, int nWhichSocket)
{
	if(Buff.Find(L"#0j/") >= 0) {
		ASSERT(0);
	};

	switch (nWhichSocket) {
	default:
		if (m_TerminalSocket.IsConnected()) {
			m_TerminalSocket.SendStr(Buff);
			m_LastSendStr = Buff;
			m_LastSendStr.Remove(0x0d);
			m_LastSendStr.Remove(0x0a);
		}
		break;
	case 1:
		if (m_BackGroundSocket.IsConnected()) {
			m_BackGroundSocket.SendStr(Buff);
		}
		break;
	}
}

int CFieldBusController::DecodeAllStrings(CString *pBuff)
{
	int nErrorCode = 0;
	int ii, nMotor, nValuePos, nAxis;
	float fValue;
	CString Temp;

	if(pBuff->GetLength() <= 0) return -1;

	if((nValuePos = pBuff->Find(L"="))>0) {

		Temp.Empty();
		for(ii=nValuePos;ii<pBuff->GetLength();ii++) Temp += pBuff->GetAt(ii);

		if(swscanf_s(Temp.GetBuffer(100),L"=%f",&fValue) == 1) {
/*
			switch(m_nBkRxStr) {
			case 0: m_nBkRxStr++;
				break;
			case 1: m_ECAT.nDCClockDiff = (int)fValue;
				m_nBkRxStr++;
				break;
			case 2: m_Sys.nServoCount = (int)fValue;
				m_nBkRxStr++;
				break;
			case 3:
			case 4:
			case 5:	m_nTemperature[m_nBkRxStr - 3] = fValue;
				m_nBkRxStr++;
				break;
			case 6:
			case 7:	m_nOilerStatus[m_nBkRxStr - 6] = fValue;
				m_nBkRxStr++;
				break;
			case 8:
			case 9:	m_nToolHolderStatus[m_nBkRxStr - 9] = fValue;
				m_nBkRxStr++;
				break;
			case 10:	
			case 11: m_nToolID[m_nBkRxStr - 10] = fValue;
				m_nBkRxStr++;
				break;
			case 12:	
			case 13: m_nAirKnifeStatus[m_nBkRxStr - 12] = fValue;
				m_nBkRxStr++;
				break;
			default:
				m_nWatchRxStr = m_nBkRxStr - 14;
				if(m_nWatchRxStr>=0 && m_nWatchRxStr<m_strWatchArray.GetSize()) {
					m_fWatchArray[m_nWatchRxStr] = fValue;
				} else {
					m_nBkRxStr = 0;
				}
				break;
			}
*/

			//ECAT
			if(pBuff->Find(L"ECAT[0]") == 0) {
				if(pBuff->Find(L"DCClockDiff") > 0) {
					m_ECAT.nDCClockDiff = (int)fValue;
					return nErrorCode;
				}
			}

			//SYS
			if(pBuff->Find(L"Sys.") == 0) {
				if(pBuff->Find(L"ServoPeriod") > 0) {
					m_Sys.fServoPeriod = fValue;
					return nErrorCode;
				}
				if(pBuff->Find(L"ServoCount") > 0) {
					m_Sys.nServoCount = (int)fValue;
					return nErrorCode;
				}
			}




			//Motor
			if(swscanf_s(pBuff->GetBuffer(100),L"Motor[%d",&nMotor) == 1) {

				for(ii=0;ii<16;ii++) {
					if(theApp.m_Axes[ii].nPhysicalAxis==nMotor) {
						nAxis = ii;

						if(pBuff->Find(L"MaxSpeed") > 0) {
							theApp.m_Axes[nAxis].fMaxMoveSpeed = fValue;
							return nErrorCode;
						}
						if(pBuff->Find(L"HomeVel") > 0) {
							theApp.m_Axes[nAxis].fHomeSpeed = fValue;
							return nErrorCode;
						}
						if(pBuff->Find(L"Homeoffset") > 0) {
							theApp.m_Axes[nAxis].nLimitSwitchOffset = (int)fValue;
							return nErrorCode;
						}
						if(pBuff->Find(L"InPosBand") > 0) {
							theApp.m_Axes[nAxis].nPositionError = (int)fValue;
							return nErrorCode;
						}
						if(pBuff->Find(L"InvAmax") > 0) {
							theApp.m_Axes[nAxis].fAcceleration = 1.0f / fValue;
							return nErrorCode;
						}
						if(pBuff->Find(L"JogTa") > 0) {
							theApp.m_Axes[nAxis].fJoystickAcceleration = -1.0f / fValue;
							return nErrorCode;
						}
						if(pBuff->Find(L"HomePos") > 0) {
							theApp.m_Axes[nAxis].nHomePos = (int)fValue;
							return nErrorCode;
						}
						break;
					}
				}
			}
		}
	}

	return nErrorCode;
}


UINT CommsThread(LPVOID pParam)
{
	CFieldBusController* pParent = (CFieldBusController *)pParam;
	pParent->m_bCommsThreadFinished=FALSE;
	int nLastBkRxStr = -1;

	pParent->m_nLastMsgCounter=1;
	pParent->m_bTxNextMsg = true;
	pParent->m_nBkRxStr = 0;
	pParent->m_nWatchRxStr = -1;

	while(pParent->m_bCommsThreadEnabled==TRUE) {
		switch(pParent->m_nBkRxStr) {
		case 0://	pParent->SendStr(L"&1..3?\r\n", _BACKGROUND);
			break;
		case 1:	pParent->SendStr(L"ECAT[0].DCClockDiff\r\n", _BACKGROUND);
			break;
		case 2:	pParent->SendStr(L"Sys.ServoCount\r\n", _BACKGROUND);
			break;
		case 3:	pParent->SendStr(L"Temperature0\r\n", _BACKGROUND);
			break;
		case 4:	pParent->SendStr(L"Temperature1\r\n", _BACKGROUND);
			break;
		case 5:	pParent->SendStr(L"Temperature2\r\n", _BACKGROUND);
			break;
		case 6:	pParent->SendStr(L"OilerStatus0\r\n", _BACKGROUND);
			break;
		case 7:	pParent->SendStr(L"OilerStatus1\r\n", _BACKGROUND);
			break;
		case 8:	pParent->SendStr(L"ToolHolderClampStatus0\r\n", _BACKGROUND);
			break;
		case 9:	pParent->SendStr(L"ToolHolderClampStatus1\r\n", _BACKGROUND);
			break;
		case 10:	pParent->SendStr(L"ToolID0\r\n", _BACKGROUND);
			break;
		case 11:	pParent->SendStr(L"ToolID1\r\n", _BACKGROUND);
			break;
		case 12:	pParent->SendStr(L"AirKnifeStatus0\r\n", _BACKGROUND);
			break;
		case 13:	pParent->SendStr(L"AirKnifeStatus1\r\n", _BACKGROUND);
			break;
		default:
			pParent->m_nWatchRxStr = pParent->m_nBkRxStr - 14;
			if(pParent->m_nWatchRxStr<pParent->m_strWatchArray.GetSize()) {
				pParent->SendStr(pParent->m_strWatchArray.GetAt(pParent->m_nWatchRxStr) + L"\r\n", _BACKGROUND);
			} else {
				pParent->m_nBkRxStr = 0;
				pParent->m_nWatchRxStr = -1;
				Sleep(100);
			}
			break;
		}
		Sleep(1);
	};
	pParent->m_bCommsThreadFinished=TRUE;
	return 0;
}


void CFieldBusController::Initialize()
{

	if (m_bEnableAtStartUp == true) {
		SocketConnect();
		StartCommsThread();
		if (m_bStartUpThreadEnabled == FALSE) {
			m_bStartUpThreadEnabled = true;
			m_pStartUpThread = AfxBeginThread(&FieldBusStartUpThread, this, THREAD_PRIORITY_NORMAL, 0, NULL);
		}
	}
}

void CFieldBusController::SuspendStartUpThread()
{

	if(m_bStartUpThreadEnabled==TRUE) {
		m_bStartUpThreadEnabled=FALSE;
		WaitForSingleObject(m_pStartUpThread,INFINITE);
	};
}

UINT FieldBusStartUpThread(LPVOID pParam)
{
	CMainFrame*  pFrame;
	CFieldBusController* pParent = (CFieldBusController *)pParam;
	pParent->m_bStartUpThreadFinished = FALSE;
	int nLoop = 20;

	while (pParent->m_bStartUpThreadEnabled == TRUE && nLoop>0) {
		if (pParent->IsConnected(_BACKGROUND)) {

			if (theApp.m_Ethercat.IsConnected() == true && theApp.m_Ethercat.IsFieldBusConnected() == false) {
				pParent->SendStr(L"Ecat[0].DCClockDiff\r\n", _BACKGROUND);
				Sleep(10);
				if (pParent->m_ECAT.nDCClockDiff != 0) {
					theApp.m_Ethercat.StartFieldBus();
				}
				else {
					pParent->SendStr(L"Enable PLC 1\r\n", _TERMINAL);
				}
			}
			else {
				pParent->m_bStartUpThreadEnabled = FALSE;
			}
		}
		pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
//		if (pFrame)	pFrame->SendMessage(UI_INVALIDATE_RESOURCE_PANE);
		nLoop--;
		Sleep(500);
	}
	if (pParent->m_bStartUpThreadEnabled == TRUE) {
		//You shouldn't ever get here, but if you do
		//Start the field bus anyway. Clearly something went wrong with the previous bit
		pParent->SendStr(L"Enable PLC 1\r\n", _TERMINAL);
		Sleep(1000);
		theApp.m_Ethercat.StartFieldBus();		
	}

	switch (theApp.m_Tank.nScannerDescription) {
	default:
		for (int i = 0; i < 20; i++) {
			if (theApp.m_Axes[i].bAvailable == TRUE)
				pParent->DownloadAxisParameters(i);
		}
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case DUAL_ROBOT:
	case SINGLE_ROBOT:
		break;
	}
	pParent->m_bStartUpThreadFinished = TRUE;

	return 0;
}


void CFieldBusController::Zero()
{

	m_bEnableAtStartUp=false;
	m_nIPAddrs = 0;
	m_nPort=0;
	m_nProtocol=0;
	ZeroMemory(m_wUser, sizeof m_wUser);
	ZeroMemory(m_wPassword, sizeof m_wPassword);

	ZeroMemory(m_Motor, sizeof m_Motor);
	ZeroMemory(&m_Sys, sizeof m_Sys);
	ZeroMemory(m_Coord, sizeof m_Coord);

}

int CFieldBusController::Save(CFile *pFile)
{
	ULONGLONG nFileOffset = pFile->GetPosition();

	pFile->Write(&theApp.m_fVersion,sizeof theApp.m_fVersion);
	pFile->Write(&m_bEnableAtStartUp, sizeof m_bEnableAtStartUp);
	pFile->Write(&m_nIPAddrs, sizeof m_nIPAddrs);
	pFile->Write(&m_nPort, sizeof m_nPort);
	pFile->Write(&m_nProtocol, sizeof m_nProtocol);
	pFile->Write(&m_wUser, sizeof m_wUser);
	pFile->Write(&m_wPassword, sizeof m_wPassword);
	pFile->Write(&m_nDCRefBand, sizeof m_nDCRefBand);
	pFile->Write(&m_nDCRefPlus, sizeof m_nDCRefPlus);
	pFile->Write(&m_nDCRefMinus, sizeof m_nDCRefMinus);

	int nWatchSize = m_strWatchArray.GetSize();
	CArchive* pAr = new CArchive(pFile,CArchive::store);
	for(int ii=0;ii<nWatchSize;ii++) {
		pAr->WriteString(m_strWatchArray.GetAt(ii) + L"\r\n");
	}
	pAr->Close();
	SAFE_DELETE(pAr);

	return (int)nFileOffset;

}


bool CFieldBusController::Retrieve(CFile *pFile)
{
	float fVersion;
	CString Buff;

	pFile->Read(&fVersion,sizeof fVersion);
	if(fVersion>=4.30f) {
		pFile->Read(&m_bEnableAtStartUp, sizeof m_bEnableAtStartUp);
		pFile->Read(&m_nIPAddrs, sizeof m_nIPAddrs);
		pFile->Read(&m_nPort, sizeof m_nPort);
		pFile->Read(&m_nProtocol, sizeof m_nProtocol);
		pFile->Read(&m_wUser, sizeof m_wUser);
		pFile->Read(&m_wPassword, sizeof m_wPassword);
		pFile->Read(&m_nDCRefBand, sizeof m_nDCRefBand);
		pFile->Read(&m_nDCRefPlus, sizeof m_nDCRefPlus);
		pFile->Read(&m_nDCRefMinus, sizeof m_nDCRefMinus);

		CArchive* pAr = new CArchive(pFile,CArchive::load);
		while(pAr->ReadString(Buff)) {
			if(Buff.GetLength()>0) {
				m_strWatchArray.Add(Buff);
			} else {
				break;
			};
		}
		pAr->Close();
		SAFE_DELETE(pAr);

		return true;
	}
	return false;

}


bool CFieldBusController::SocketConnect()
{

	if(m_TerminalSocket.IsConnected() == false) {
		m_TerminalSocket.ConnectFromParent(FieldBusTerminalCallback, m_nIPAddrs, 23, L"root", L"deltatau");
	}
	if(m_BackGroundSocket.IsConnected() == false) {
		m_BackGroundSocket.ConnectFromParent(FieldBusBackGroundCallback, m_nIPAddrs, 23, L"root", L"deltatau");
	}

	return true;
}


bool CFieldBusController::IsConnected(int nWhichSocket)
{
	switch (nWhichSocket) {
	default: return m_TerminalSocket.IsConnected();
		break;
	case _BACKGROUND: return m_BackGroundSocket.IsConnected();
		break;
	}
}

void CFieldBusController::SetHomePos(int nAxis, int nHomePos)
{
	CString Buff;

	theApp.m_Axes[nAxis].nHomePos = nHomePos;
	Buff.Format(L"Motor[%d].HomePos=%d", theApp.m_Axes[nAxis].nPhysicalAxis, theApp.m_Axes[nAxis].nHomePos);
	SendStr(Buff, _TERMINAL);
	if (theApp.m_pMainWnd) {
		theApp.m_pMainWnd->SendMessage(UI_UPDATE_AXES, NULL, NULL);
	}
}

//Set the home position according to the current encoder position.
void CFieldBusController::SetHomePosToEncoderPos(int nAxis)
{
	theApp.m_Axes[nAxis].nHomePos += theApp.m_Ethercat.EncoderPos(theApp.m_Axes[nAxis].nPhysicalAxis - 1);
	SetHomePos(nAxis);
}

void CFieldBusController::SetHomePos(int nAxis)
{
	SetHomePos(nAxis, theApp.m_Axes[nAxis].nHomePos);
}

void CFieldBusController::SetHomePosAll()
{
	for (int iAxis = 0; iAxis < m_nMaxNumberAxes; iAxis++) {
		SetHomePos(iAxis);
	}
}

void CFieldBusController::StartCommsThread()
{
	if(m_bCommsThreadEnabled==FALSE) {
		m_nThreadAction=0;
		m_bCommsThreadEnabled=TRUE;
		m_pCommsThread=AfxBeginThread(&CommsThread,this,THREAD_PRIORITY_NORMAL ,0,NULL);
	};
}

void CFieldBusController::SuspendCommsThread()
{

	if(m_bCommsThreadEnabled==TRUE) {
		m_bCommsThreadEnabled=FALSE;
		WaitForSingleObject(m_pCommsThread,INFINITE);
	};
}


void CFieldBusController::DownloadPolyLine(CPolyCoord *pLine, int nFirstCoordinate)
{
	CCoord CpSurface;
	CString Buff;

	for (int nNumberCoords = 0; nNumberCoords<pLine->GetSize(); nNumberCoords++) {
		pLine->GetCoord(nNumberCoords, &CpSurface);

		switch (theApp.m_Tank.nScannerDescription) {
		case DUAL_ROBOT_9_PLUS_9:
		case DUAL_ROBOT:
			theApp.InvertNormScannerSpecific(&CpSurface, PORTSIDE);
			theApp.InvertNormScannerSpecific(&CpSurface, STARBOARD);
			CpSurface.TransformCoordinate(PORTSIDE, &theApp.m_Tank.vFrameZero[0], 0.0f, 0.0f, 0.0f);
			CpSurface.TransformCoordinate(STARBOARD, &theApp.m_Tank.vFrameZero[1], 0.0f, 0.0f, 180.0f);
			CpSurface.FormatQuaternion(&Buff, 1, (nNumberCoords * 14) + nFirstCoordinate);
			break;
		case RAILWAY_AXLE: 
		case TANK_5AXIS:
		case TANK_2AXIS:
		case TANK_6AXIS:
		case TWIN_TOWER_KINEMATIC:
			theApp.InvertNormScannerSpecific(&CpSurface, PORTSIDE);
			theApp.InvertNormScannerSpecific(&CpSurface, STARBOARD);
			CpSurface.TransformCoordinate(PORTSIDE, &theApp.m_Tank.vFrameZero[0], 0.0f, 0.0f, 0.0f);
			CpSurface.TransformCoordinate(STARBOARD, &theApp.m_Tank.vFrameZero[1], 0.0f, 0.0f, 180.0f);
			CpSurface.FormatNormal(&Buff, 1, (nNumberCoords * 20) + nFirstCoordinate);
			break;
		}

		SendStr(Buff, _TERMINAL);

		Sleep(5);
		if (((nNumberCoords % 10) == 0) && (nNumberCoords != 0)) {
			Sleep(50);
		};
	}

}


void CFieldBusController::DoPolyLine(CPolyCoord *pLine, float fMoveSpeed, float fInitialSpeed)
{
	CCoord CpSurface;
	CString Buff;
	int nFirstCoordinate = 0;

	KillAllMotionPrograms();

	theApp.m_LastSettings.fFeedRate = 100.0f;

	Buff.Format(L"InitialFeedRate=%.02f", fInitialSpeed * theApp.m_LastSettings.fFeedRate / 100.0f);	SendStr(Buff, _TERMINAL);
	Buff.Format(L"FeedRate=%.02f", fMoveSpeed * theApp.m_LastSettings.fFeedRate / 100.0f);				SendStr(Buff, _TERMINAL);

	DownloadPolyLine(pLine, nFirstCoordinate);

	Buff.Format(L"FirstCoordinate=%d", nFirstCoordinate);	SendStr(Buff, _TERMINAL);
	Buff.Format(L"NumberCoords=%d", pLine->GetSize());		SendStr(Buff, _TERMINAL);


	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_LastSettings.nCoordMask) {
		case 1:
			SendStr(L"&1", _TERMINAL);
			SendStr(L"#1->I", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"&1 define lookahead 5000", _TERMINAL);
			SendStr(L"Coord[1].LHDistance=500", _TERMINAL);
			SendStr(L"Coord[1].SegLinToPvt=0", _TERMINAL);
			SendStr(L"Coord[1].TA=20", _TERMINAL);
			SendStr(L"Coord[1].Td=20", _TERMINAL);
			SendStr(L"Coord[1].TS=20", _TERMINAL);
			SendStr(L"Coord[1].TM=5", _TERMINAL);
			SendStr(L"B4R", _TERMINAL);
			break;
		case 2:
			SendStr(L"&2", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"#8->I", _TERMINAL);
			SendStr(L"#9->I", _TERMINAL);
			SendStr(L"#10->I", _TERMINAL);
			SendStr(L"#11->I", _TERMINAL);
			SendStr(L"#12->I", _TERMINAL);
			SendStr(L"&2 define lookahead 5000", _TERMINAL);
			SendStr(L"Coord[2].LHDistance=500", _TERMINAL);
			SendStr(L"Coord[2].SegLinToPvt=1", _TERMINAL);
			SendStr(L"Coord[2].TA=20", _TERMINAL);
			SendStr(L"Coord[2].Td=20", _TERMINAL);
			SendStr(L"Coord[2].TS=20", _TERMINAL);
			SendStr(L"Coord[2].TM=5", _TERMINAL);
			SendStr(L"B5R", _TERMINAL);
			break;
		case 3:
			SendStr(L"&3", _TERMINAL);
			SendStr(L"#1->I", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"#8->I", _TERMINAL);
			SendStr(L"#9->I", _TERMINAL);
			SendStr(L"#10->I", _TERMINAL);
			SendStr(L"#11->I", _TERMINAL);
			SendStr(L"#12->I", _TERMINAL);
			SendStr(L"&3 define lookahead 5000", _TERMINAL);
			SendStr(L"Coord[3].LHDistance=500", _TERMINAL);
			SendStr(L"Coord[3].SegLinToPvt=1", _TERMINAL);
			SendStr(L"Coord[3].TA=20", _TERMINAL);
			SendStr(L"Coord[3].Td=20", _TERMINAL);
			SendStr(L"Coord[3].TS=50", _TERMINAL);
			SendStr(L"Coord[3].TM=10", _TERMINAL);
			SendStr(L"B6R", _TERMINAL);
			break;
		}
		break;
	case DUAL_ROBOT_9_PLUS_9:
		SendRobotWeightings(0,true);
		switch(theApp.m_LastSettings.nCoordMask) {
		case 1:
//			SendStr(L"#1->I", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"#8->I", _TERMINAL);
			SendStr(L"#9->I", _TERMINAL);
			SendStr(L"B4R", _TERMINAL);
			break;
		case 2:
			SendStr(L"&2", _TERMINAL);
//			SendStr(L"#11->I", _TERMINAL);
			SendStr(L"#12->I", _TERMINAL);
			SendStr(L"#13->I", _TERMINAL);
			SendStr(L"#14->I", _TERMINAL);
			SendStr(L"#15->I", _TERMINAL);
			SendStr(L"#16->I", _TERMINAL);
			SendStr(L"#17->I", _TERMINAL);
			SendStr(L"#18->I", _TERMINAL);
			SendStr(L"#19->I", _TERMINAL);
			SendStr(L"B5R", _TERMINAL);
			break;
		case 3:
			SendStr(L"&3", _TERMINAL);
//			SendStr(L"#1->I", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"#8->I", _TERMINAL);
			SendStr(L"#9->I", _TERMINAL);
//			SendStr(L"#11->I", _TERMINAL);
			SendStr(L"#12->I", _TERMINAL);
			SendStr(L"#13->I", _TERMINAL);
			SendStr(L"#14->I", _TERMINAL);
			SendStr(L"#15->I", _TERMINAL);
			SendStr(L"#16->I", _TERMINAL);
			SendStr(L"#17->I", _TERMINAL);
			SendStr(L"#18->I", _TERMINAL);
			SendStr(L"#19->I", _TERMINAL);
			SendStr(L"&3 define lookahead 5000", _TERMINAL);
			SendStr(L"Coord[3].LHDistance=500", _TERMINAL);
			SendStr(L"Coord[3].SegLinToPvt=1", _TERMINAL);
			SendStr(L"Coord[3].TA=20", _TERMINAL);
			SendStr(L"Coord[3].Td=20", _TERMINAL);
			SendStr(L"Coord[3].TS=20", _TERMINAL);
			SendStr(L"Coord[3].TM=2", _TERMINAL);
			SendStr(L"B6R", _TERMINAL);
			break;
		}
		break;
	case SPX_ROBOT:
		SendRobotWeightings(0,true);
		switch(theApp.m_LastSettings.nCoordMask) {
		case 1:
			SendStr(L"&1", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"#8->I", _TERMINAL);
			SendStr(L"#9->I", _TERMINAL);
			SendStr(L"B4R", _TERMINAL);
			break;
		case 2:
			SendStr(L"&2", _TERMINAL);
			SendStr(L"#13->I", _TERMINAL);
			SendStr(L"#14->I", _TERMINAL);
			SendStr(L"#15->I", _TERMINAL);
			SendStr(L"#16->I", _TERMINAL);
			SendStr(L"#17->I", _TERMINAL);
			SendStr(L"#18->I", _TERMINAL);
			SendStr(L"#19->I", _TERMINAL);
			SendStr(L"B5R", _TERMINAL);
			break;
		case 3:
			SendStr(L"&3", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"#8->I", _TERMINAL);
			SendStr(L"#9->I", _TERMINAL);
			SendStr(L"#13->I", _TERMINAL);
			SendStr(L"#14->I", _TERMINAL);
			SendStr(L"#15->I", _TERMINAL);
			SendStr(L"#16->I", _TERMINAL);
			SendStr(L"#17->I", _TERMINAL);
			SendStr(L"#18->I", _TERMINAL);
			SendStr(L"#19->I", _TERMINAL);
			SendStr(L"&3 define lookahead 5000", _TERMINAL);
			SendStr(L"Coord[3].LHDistance=500", _TERMINAL);
			SendStr(L"Coord[3].SegLinToPvt=1", _TERMINAL);
			SendStr(L"Coord[3].TA=20", _TERMINAL);
			SendStr(L"Coord[3].Td=20", _TERMINAL);
			SendStr(L"Coord[3].TS=20", _TERMINAL);
			SendStr(L"Coord[3].TM=2", _TERMINAL);
			if(pLine->GetSize()<5) {
				SendStr(L"Coord[3].FeedTime=1000", _TERMINAL);
			} else {
				SendStr(L"Coord[3].FeedTime=706", _TERMINAL);
			}
			SendStr(L"B6R", _TERMINAL);
			break;
		}
		break;
	case RAILWAY_AXLE:
		switch (theApp.m_LastSettings.nCoordMask) {
		case 1:
			SendStr(L"&1", _TERMINAL);
			SendStr(L"#1->I", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"B6R", _TERMINAL);
			break;
		case 2:
			SendStr(L"&2", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"B6R", _TERMINAL);
			break;
		case 3:
			SendStr(L"&3", _TERMINAL);
			SendStr(L"#1->I", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"&3 define lookahead 5000", _TERMINAL);
			SendStr(L"Coord[3].LHDistance=500", _TERMINAL);
			SendStr(L"Coord[3].SegLinToPvt=1", _TERMINAL);
			SendStr(L"Coord[3].TA=20", _TERMINAL);
			SendStr(L"Coord[3].Td=20", _TERMINAL);
			SendStr(L"Coord[3].TS=20", _TERMINAL);
			SendStr(L"Coord[3].TM=2", _TERMINAL);
			SendStr(L"Coord[3].FeedTime=706", _TERMINAL);
			SendStr(L"B6R", _TERMINAL);
			break;
		}
		break;
	case TANK_5AXIS:
	case TWIN_TOWER_KINEMATIC:
		switch (theApp.m_LastSettings.nCoordMask) {
		case 1:
			SendStr(L"&1", _TERMINAL);
			SendStr(L"#1->I", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"&1 define lookahead 5000", _TERMINAL);
			SendStr(L"Coord[1].LHDistance=500", _TERMINAL);
			SendStr(L"Coord[1].SegLinToPvt=1", _TERMINAL);
			SendStr(L"Coord[1].SegMoveTime=15", _TERMINAL);
			SendStr(L"Coord[1].TA=20", _TERMINAL);
			SendStr(L"Coord[1].Td=20", _TERMINAL);
			SendStr(L"Coord[1].TS=20", _TERMINAL);
			SendStr(L"Coord[1].TM=2", _TERMINAL);
			SendStr(L"B6R", _TERMINAL);
			break;
		case 2:
			SendStr(L"&2", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"#8->I", _TERMINAL);
			SendStr(L"#9->I", _TERMINAL);
			SendStr(L"#10->I", _TERMINAL);
			SendStr(L"&2 define lookahead 5000", _TERMINAL);
			SendStr(L"Coord[2].LHDistance=500", _TERMINAL);
			SendStr(L"Coord[2].SegLinToPvt=1", _TERMINAL);
			SendStr(L"Coord[2].SegMoveTime=15", _TERMINAL);
			SendStr(L"Coord[2].TA=20", _TERMINAL);
			SendStr(L"Coord[2].Td=20", _TERMINAL);
			SendStr(L"Coord[2].TS=20", _TERMINAL);
			SendStr(L"Coord[2].TM=2", _TERMINAL);
			SendStr(L"B6R", _TERMINAL);
			break;
		case 3:
			SendStr(L"&3", _TERMINAL);
			SendStr(L"#1->I", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"#8->I", _TERMINAL);
			SendStr(L"#9->I", _TERMINAL);
			SendStr(L"#10->I", _TERMINAL);
			SendStr(L"&3 define lookahead 5000", _TERMINAL);
			SendStr(L"Coord[3].LHDistance=500", _TERMINAL);
			SendStr(L"Coord[3].SegLinToPvt=1", _TERMINAL);
			SendStr(L"Coord[3].SegMoveTime=15", _TERMINAL);
			SendStr(L"Coord[3].TA=5", _TERMINAL);
			SendStr(L"Coord[3].Td=5", _TERMINAL);
			SendStr(L"Coord[3].TS=4", _TERMINAL);
			SendStr(L"Coord[3].TM=2", _TERMINAL);
			SendStr(L"Coord[3].FeedTime=406", _TERMINAL);
			SendStr(L"B6R", _TERMINAL);
			break;
		}
		break;
	case TANK_2AXIS:
		SendStr(L"&1", _TERMINAL);
		SendStr(L"#1->I", _TERMINAL);
		SendStr(L"#2->I", _TERMINAL);
		SendStr(L"&1 define lookahead 5000", _TERMINAL);
		SendStr(L"Coord[1].LHDistance=500", _TERMINAL);
		SendStr(L"Coord[1].SegLinToPvt=1", _TERMINAL);
		SendStr(L"Coord[1].SegMoveTime=15", _TERMINAL);
		SendStr(L"Coord[1].TA=500", _TERMINAL);
		SendStr(L"Coord[1].Td=500", _TERMINAL);
		SendStr(L"Coord[1].TS=500", _TERMINAL);
		SendStr(L"Coord[1].TM=2", _TERMINAL);
		Sleep(100);
		SendStr(L"B6R", _TERMINAL);
		Sleep(500);
		break;
	}

}

void CFieldBusController::DownloadRobotKinematics(int nRobot)
{
	CString Buff;
	CString Robot;

	SendAxesIncluded(MOVETO_INCLUDE, true);

	SendStr(L"undefine all", _TERMINAL);

	switch(nRobot) {
	case 0: Robot.Empty();
		break;
	case 1: Robot = "B_";
		break;
	}

/*
	These are NEVER NEVER SENT
	Buff.Format(L"%sBaseXPos=%f",Robot,theApp.m_Robot[nRobot].m_vBase.x);
	SendStr(Buff, _TERMINAL);
	Buff.Format(L"%sBaseYPos=%f",Robot,theApp.m_Robot[nRobot].m_vBase.y);
	SendStr(Buff, _TERMINAL);
	Buff.Format(L"%sBaseZPos=%f",Robot,theApp.m_Robot[nRobot].m_vBase.z);
	SendStr(Buff, _TERMINAL);
	Buff.Format(L"%sBaseRotX=%f",Robot,theApp.m_Robot[nRobot].m_fBaseRotX);
	SendStr(Buff, _TERMINAL);
	Buff.Format(L"%sBaseRotY=%f",Robot,theApp.m_Robot[nRobot].m_fBaseRotY);
	SendStr(Buff, _TERMINAL);
	Buff.Format(L"%sBaseRotZ=%f",Robot,theApp.m_Robot[nRobot].m_fBaseRotZ);
	SendStr(Buff, _TERMINAL);
*/

	Buff.Format(L"%sLength12X=%f",Robot,theApp.m_Robot[nRobot].m_vJ1J2.x);
	SendStr(Buff, _TERMINAL);
	Buff.Format(L"%sLength12Y=%f",Robot,theApp.m_Robot[nRobot].m_vJ1J2.y);
	SendStr(Buff, _TERMINAL);
	Buff.Format(L"%sLength12Z=%f",Robot,theApp.m_Robot[nRobot].m_vJ1J2.z);
	SendStr(Buff, _TERMINAL);

	Buff.Format(L"%sLength23X=%f",Robot,theApp.m_Robot[nRobot].m_vJ2J3.x);
	SendStr(Buff, _TERMINAL);
	Buff.Format(L"%sLength23Y=%f",Robot,theApp.m_Robot[nRobot].m_vJ2J3.y);
	SendStr(Buff, _TERMINAL);
	Buff.Format(L"%sLength23Z=%f",Robot,theApp.m_Robot[nRobot].m_vJ2J3.z);
	SendStr(Buff, _TERMINAL);

	Buff.Format(L"%sLength34X=%f",Robot,theApp.m_Robot[nRobot].m_vJ3J4.x);
	SendStr(Buff, _TERMINAL);
	Buff.Format(L"%sLength34Y=%f",Robot,theApp.m_Robot[nRobot].m_vJ3J4.y);
	SendStr(Buff, _TERMINAL);
	Buff.Format(L"%sLength34Z=%f",Robot,theApp.m_Robot[nRobot].m_vJ3J4.z);
	SendStr(Buff, _TERMINAL);

	Buff.Format(L"%sLength45X=%f",Robot,theApp.m_Robot[nRobot].m_vJ4J5.x);
	SendStr(Buff, _TERMINAL);
	Buff.Format(L"%sLength45Y=%f",Robot,theApp.m_Robot[nRobot].m_vJ4J5.y);
	SendStr(Buff, _TERMINAL);
	Buff.Format(L"%sLength45Z=%f",Robot,theApp.m_Robot[nRobot].m_vJ4J5.z);
	SendStr(Buff, _TERMINAL);

}

void CFieldBusController::DownloadRobotJoints(int nRobot)
{
	CString Buff;
	int nAxisStart = 0;
	int	nAxisFinish = 5;
	float fStepSize;

	SendStr(L"undefine all", _TERMINAL);

	switch(nRobot) {
	case 0:
		nAxisStart = 0;
		nAxisFinish = 5;
		break;
	case 1:
		nAxisStart = 6;
		nAxisFinish = 11;
		break;
	}

	for(int nAxis=nAxisStart;nAxis<=nAxisFinish;nAxis++) {

		fStepSize = theApp.m_Axes[nAxis - nAxisStart].fStepSize;

		Buff.Format(L"Motor[%d].MinPos=%d",nAxis+1, theApp.m_Axes[nAxis - nAxisStart].nMinTravel);
		SendStr(Buff, _TERMINAL);
		Buff.Format(L"Motor[%d].MaxPos=%d",nAxis+1, theApp.m_Axes[nAxis - nAxisStart].nMaxTravel);
		SendStr(Buff, _TERMINAL);
/*
		Buff.Format(L"Motor[%d].MaxSpeed=%.01f",nAxis+1, (theApp.m_Axes[nAxis - nAxisStart].fMaxMoveSpeed / fStepSize) / 1000.0f);
		SendStr(Buff, _TERMINAL);
		Buff.Format(L"Motor[%d].JogSpeed=%.01f",nAxis+1, (theApp.m_Axes[nAxis - nAxisStart].fMaxMoveSpeed / fStepSize) / 1000.0f);
		SendStr(Buff, _TERMINAL);
		Buff.Format(L"Motor[%d].InvAmax=%.07f",nAxis+1, 1.0f / theApp.m_Axes[nAxis - nAxisStart].fAcceleration);
		SendStr(Buff, _TERMINAL);
		Buff.Format(L"Motor[%d].InvDmax=%.07f",nAxis+1, 1.0f / theApp.m_Axes[nAxis - nAxisStart].fAcceleration);
		SendStr(Buff, _TERMINAL);
*/
	}

}



bool CFieldBusController::WaitForFreeCoordinateSystems()
{
	int nLoop = 1000;

	while(1) {
		if(m_nProgActive[0] == 0 && m_nProgActive[1] == 0 && m_nProgActive[2] == 0) {
			return true;
		}
		Sleep(1);
		nLoop--;
		if(nLoop<=0) return false;
	};
	return TRUE;

}

bool CFieldBusController::IsMotionFinished()
{
	switch(theApp.m_LastSettings.nCoordMask) {
	case 1: if(m_nProgActive[0] == 0) return true;
		break;
	case 2:  if(m_nProgActive[1] == 0) return true;
		break;
	case 3:  if(m_nProgActive[2] == 0) return true;
		break;
	}
	return false;
}

bool CFieldBusController::IsMotionStarted()
{
	switch(theApp.m_LastSettings.nCoordMask) {
	case 1: if(m_nProgActive[0] == 1) return true;
		break;
	case 2:  if(m_nProgActive[1] == 1) return true;
		break;
	case 3:  if(m_nProgActive[2] == 1) return true;
		break;
	}
	return false;
}

void CFieldBusController::DownloadEndEffectorWithWaterPath(int nSide,bool bImmediately)
{
	CString Buff,TX,TY,TZ,rotY;
	float fAngle,fWp;

	switch (theApp.m_Tank.nScannerDescription) {
	default:
		if (bImmediately == true) {
			SendStr(L"&1qa", _TERMINAL);
			SendStr(L"&2qa", _TERMINAL);
			SendStr(L"&3qa", _TERMINAL);
			SendStr(L"undefine all", _TERMINAL);
		}

		switch (nSide) {
		case 0:
			TX = L"PreAcceptLengthTX";
			TY = L"PreAcceptLengthTY";
			TZ = L"PreAcceptLengthTZ";
			rotY = L"PreAcceptToolRY";
			break;
		case 1:
			TX = L"B_PreAcceptLengthTX";
			TY = L"B_PreAcceptLengthTY";
			TZ = L"B_PreAcceptLengthTZ";
			rotY = L"B_PreAcceptToolRY";
			break;
		}

		MinMax(&theApp.m_LastSettings.nExtraArmLength, 0, 50);

		fWp = theApp.m_Kinematics.GetDesiredWaterPath(theApp.m_Robot[nSide].m_nSide);

		Buff.Format(L"%s=%f", TX, theApp.m_Robot[nSide].m_vJ6ToolTip.x + fWp * theApp.m_Robot[nSide].m_vAtToolTip.x);
		SendStr(Buff, _TERMINAL);
		Buff.Format(L"%s=%f", TY, theApp.m_Robot[nSide].m_vJ6ToolTip.y + fWp * theApp.m_Robot[nSide].m_vAtToolTip.y);
		SendStr(Buff, _TERMINAL);
		Buff.Format(L"%s=%f", TZ, theApp.m_Robot[nSide].m_vJ6ToolTip.z + fWp * theApp.m_Robot[nSide].m_vAtToolTip.z + (float)theApp.m_LastSettings.nExtraArmLength);
		SendStr(Buff, _TERMINAL);

		fAngle = atan2f(theApp.m_Robot[nSide].m_vAtToolTip.x, theApp.m_Robot[nSide].m_vAtToolTip.z) * RAD_TO_DEG;
		Buff.Format(L"%s=%f", rotY, fAngle);
		SendStr(Buff, _TERMINAL);

		SendStr(L"enable plc 0", _TERMINAL);
		break;
	case RAILWAY_AXLE:
		switch (nSide) {
		case 0:
			Buff.Format(L"LengthTZ=%f", theApp.m_Kinematics.getToolLength(0));
			SendStr(Buff, _TERMINAL);
			break;
		case 1:
			Buff.Format(L"B_LengthTZ=%f", theApp.m_Kinematics.getToolLength(1));
			SendStr(Buff, _TERMINAL);
			break;
		}
		break;
	case TANK_5AXIS:
	case TANK_2AXIS:
		Buff.Format(L"Length01X=%f", theApp.m_Kinematics.m_fXOffset[0]);
		SendStr(Buff, _TERMINAL);
		Buff.Format(L"LengthTZ=%f", theApp.m_Kinematics.getToolLength(0));
		SendStr(Buff, _TERMINAL);
		break;
	case TWIN_TOWER_KINEMATIC:
		Buff.Format(L"Length01X=%f", theApp.m_Kinematics.m_fXOffset[0]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"Length01Y=%f", theApp.m_Kinematics.m_fYOffset[0]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"Length01Z=%f", theApp.m_Kinematics.m_fZOffset[0]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"B_Length01X=%f", theApp.m_Kinematics.m_fXOffset[1]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"B_Length01Y=%f", theApp.m_Kinematics.m_fYOffset[1]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"B_Length01Z=%f", theApp.m_Kinematics.m_fZOffset[1]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"LengthTY=%f", theApp.m_Kinematics.getToolLength(0));		SendStr(Buff, _TERMINAL);
		Buff.Format(L"B_LengthTY=%f", theApp.m_Kinematics.getToolLength(1));	SendStr(Buff, _TERMINAL);
		break;
	case TANK_6AXIS:
		Buff.Format(L"Length01X=%f", theApp.m_Kinematics.m_fXOffset[0]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"Length01Y=%f", theApp.m_Kinematics.m_fYOffset[0]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"Length01Z=%f", theApp.m_Kinematics.m_fZOffset[0]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"LengthTX=0");												SendStr(Buff, _TERMINAL);
		Buff.Format(L"LengthTY=0");												SendStr(Buff, _TERMINAL);
		Buff.Format(L"LengthTZ=%f", theApp.m_Kinematics.getToolLength(0));		SendStr(Buff, _TERMINAL);
		break;
	}
}

void CFieldBusController::EnableJoystick()
{
	ZeroMemory(m_fLastJogCommand,sizeof m_fLastJogCommand);

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		KillAllMotionPrograms();
		EnableMotors();
		SendStr(L"undefine all", _TERMINAL);
		SendStr(L"delete all lookahead", _TERMINAL);

		SendStr(L"XJogMove=0", _TERMINAL);
		SendStr(L"YJogMove=0", _TERMINAL);
		SendStr(L"ZJogMove=0", _TERMINAL);
		SendStr(L"AJogMove=0", _TERMINAL);
		SendStr(L"BJogMove=0", _TERMINAL);
		SendStr(L"CJogMove=0", _TERMINAL);
		SendStr(L"UJogMove=0", _TERMINAL);
		SendStr(L"RXJogMove=0", _TERMINAL);
		SendStr(L"RYJogMove=0", _TERMINAL);
		SendStr(L"RZJogMove=0", _TERMINAL);

		SendStr(L"B_XJogMove=0", _TERMINAL);
		SendStr(L"B_YJogMove=0", _TERMINAL);
		SendStr(L"B_ZJogMove=0", _TERMINAL);
		SendStr(L"B_AJogMove=0", _TERMINAL);
		SendStr(L"B_BJogMove=0", _TERMINAL);
		SendStr(L"B_CJogMove=0", _TERMINAL);
		SendStr(L"B_UJogMove=0", _TERMINAL);
		SendStr(L"B_RXJogMove=0", _TERMINAL);
		SendStr(L"B_RYJogMove=0", _TERMINAL);
		SendStr(L"B_RZJogMove=0", _TERMINAL);
		SendFrameToolJoyMode(true);
		break;
	case FB_11_AXIS:
	case FB_TWIN_TOWER:
		KillAllMotionPrograms();
		break;
	};


	if (WaitAxesEnabled(theApp.m_LastSettings.nCoordMask) == false) {
		MessageBox(NULL, L"Failed to enable all motors", L"Error", MB_ICONERROR);
		return;
	}


	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_LastSettings.nCoordMask) {
		case 1:
			SendStr(L"&1", _TERMINAL);
			SendStr(L"#1->I#2->I#3->I#4->I#5->I#6->I", _TERMINAL);
			SendStr(L"Coord[1].SegLinToPvt=0", _TERMINAL);
			SendStr(L"Coord[1].TA=100", _TERMINAL);
			SendStr(L"Coord[1].Td=100", _TERMINAL);
			SendStr(L"Coord[1].TS=20", _TERMINAL);
			SendStr(L"Coord[1].TM=5", _TERMINAL);
			SendStr(L"B7R", _TERMINAL);
			break;
		case 2:
			SendStr(L"&2", _TERMINAL);
			SendStr(L"#7->I#8->I#9->I#10->I#11->I#12->I", _TERMINAL);
			SendStr(L"Coord[2].SegLinToPvt=0", _TERMINAL);
			SendStr(L"Coord[2].TA=100", _TERMINAL);
			SendStr(L"Coord[2].Td=100", _TERMINAL);
			SendStr(L"Coord[2].TS=20", _TERMINAL);
			SendStr(L"Coord[2].TM=5", _TERMINAL);
			SendStr(L"B7R", _TERMINAL);
			break;
		case 3:
			SendStr(L"&3", _TERMINAL);
			SendStr(L"#1->I#2->I#3->I#4->I#5->I#6->I#7->I#8->I#9->I#10->I#11->I#12->I", _TERMINAL);
			SendStr(L"Coord[3].SegLinToPvt=0", _TERMINAL);
			SendStr(L"Coord[3].TA=100", _TERMINAL);
			SendStr(L"Coord[3].Td=100", _TERMINAL);
			SendStr(L"Coord[3].TS=20", _TERMINAL);
			SendStr(L"Coord[3].TM=5", _TERMINAL);
			SendStr(L"B7R", _TERMINAL);
			break;
		}
		break;
	case TWIN_TOWER_KINEMATIC:
		switch (theApp.m_LastSettings.nCoordMask) {
		case 1:
			SendStr(L"&1", _TERMINAL);
			SendStr(L"#1->I#2->I#3->I#4->I#5->I", _TERMINAL);
			SendStr(L"Coord[1].SegLinToPvt=0", _TERMINAL);
			SendStr(L"Coord[1].TA=100", _TERMINAL);
			SendStr(L"Coord[1].Td=100", _TERMINAL);
			SendStr(L"Coord[1].TS=20", _TERMINAL);
			SendStr(L"Coord[1].TM=5", _TERMINAL);
			SendStr(L"B7R", _TERMINAL);
			break;
		case 2:
			SendStr(L"&2", _TERMINAL);
			SendStr(L"#6->I#7->I#8->I#9->I#10->I", _TERMINAL);
			SendStr(L"Coord[2].SegLinToPvt=0", _TERMINAL);
			SendStr(L"Coord[2].TA=100", _TERMINAL);
			SendStr(L"Coord[2].Td=100", _TERMINAL);
			SendStr(L"Coord[2].TS=20", _TERMINAL);
			SendStr(L"Coord[2].TM=5", _TERMINAL);
			SendStr(L"B7R", _TERMINAL);
			break;
		case 3:
			SendStr(L"&3", _TERMINAL);
			SendStr(L"#1->I#2->I#3->I#4->I#5->I#6->I#7->I#8->I#9->I#10->I", _TERMINAL);
			SendStr(L"Coord[3].SegLinToPvt=0", _TERMINAL);
			SendStr(L"Coord[3].TA=300", _TERMINAL);
			SendStr(L"Coord[3].Td=300", _TERMINAL);
			SendStr(L"Coord[3].TS=100", _TERMINAL);
			SendStr(L"Coord[3].TM=5", _TERMINAL);
			SendStr(L"B7R", _TERMINAL);
			break;
		}
		break;
	case DUAL_ROBOT_9_PLUS_9:
		SendRobotWeightings(0,true);
		switch(theApp.m_LastSettings.nCoordMask) {
		case 1:
			SendStr(L"&1", _TERMINAL);
			SendStr(L"#2->I#3->I#4->I#5->I#6->I#7->I#8->I#9->I", _TERMINAL);
			SendStr(L"Coord[1].SegLinToPvt=0", _TERMINAL);
			SendStr(L"Coord[1].TA=100", _TERMINAL);
			SendStr(L"Coord[1].Td=100", _TERMINAL);
			SendStr(L"Coord[1].TS=200", _TERMINAL);
			SendStr(L"Coord[1].TM=5", _TERMINAL);
			SendStr(L"B7R", _TERMINAL);
			break;
		case 2:
			SendStr(L"&2", _TERMINAL);
			SendStr(L"#12->I#13->I#14->I#15->I#16->I#17->I#18->I#19->I", _TERMINAL);
			SendStr(L"Coord[2].SegLinToPvt=0", _TERMINAL);
			SendStr(L"Coord[2].TA=100", _TERMINAL);
			SendStr(L"Coord[2].Td=100", _TERMINAL);
			SendStr(L"Coord[2].TS=200", _TERMINAL);
			SendStr(L"Coord[2].TM=5", _TERMINAL);
			SendStr(L"B7R", _TERMINAL);
			break;
		case 3:
			SendStr(L"&3", _TERMINAL);
			SendStr(L"#2->I#3->I#4->I#5->I#6->I#7->I#8->I#9->I#12->I#13->I#14->I#15->I#16->I#17->I#18->I#19->I", _TERMINAL);
			SendStr(L"Coord[3].SegLinToPvt=0", _TERMINAL);
			SendStr(L"Coord[3].TA=100", _TERMINAL);
			SendStr(L"Coord[3].Td=100", _TERMINAL);
			SendStr(L"Coord[3].TS=200", _TERMINAL);
			SendStr(L"Coord[3].TM=5", _TERMINAL);
			SendStr(L"B7R", _TERMINAL);
			Sleep(1000);
			break;
		}
		break;
	case SPX_ROBOT:
		SendRobotWeightings(0,true);
		switch(theApp.m_LastSettings.nCoordMask) {
		case 1:
			SendStr(L"&1", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"#8->I", _TERMINAL);
			SendStr(L"#9->I", _TERMINAL);
			SendStr(L"Coord[1].SegLinToPvt=0", _TERMINAL);
			SendStr(L"Coord[1].TA=100", _TERMINAL);
			SendStr(L"Coord[1].Td=100", _TERMINAL);
			SendStr(L"Coord[1].TS=20", _TERMINAL);
			SendStr(L"Coord[1].TM=5", _TERMINAL);
			SendStr(L"B7R", _TERMINAL);
			break;
		case 2:
			SendStr(L"&2", _TERMINAL);
			SendStr(L"#13->I", _TERMINAL);
			SendStr(L"#14->I", _TERMINAL);
			SendStr(L"#15->I", _TERMINAL);
			SendStr(L"#16->I", _TERMINAL);
			SendStr(L"#17->I", _TERMINAL);
			SendStr(L"#18->I", _TERMINAL);
			SendStr(L"#19->I", _TERMINAL);
			SendStr(L"Coord[2].SegLinToPvt=0", _TERMINAL);
			SendStr(L"Coord[2].TA=100", _TERMINAL);
			SendStr(L"Coord[2].Td=100", _TERMINAL);
			SendStr(L"Coord[2].TS=20", _TERMINAL);
			SendStr(L"Coord[2].TM=5", _TERMINAL);
			SendStr(L"B7R", _TERMINAL);
			break;
		case 3://?????
			SendStr(L"&3", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"#8->I", _TERMINAL);
			SendStr(L"#9->I", _TERMINAL);
			SendStr(L"#13->I", _TERMINAL);
			SendStr(L"#14->I", _TERMINAL);
			SendStr(L"#15->I", _TERMINAL);
			SendStr(L"#16->I", _TERMINAL);
			SendStr(L"#17->I", _TERMINAL);
			SendStr(L"#18->I", _TERMINAL);
			SendStr(L"#19->I", _TERMINAL);
			SendStr(L"Coord[3].SegLinToPvt=0", _TERMINAL);
			SendStr(L"Coord[3].TA=100", _TERMINAL);
			SendStr(L"Coord[3].Td=100", _TERMINAL);
			SendStr(L"Coord[3].TS=200", _TERMINAL);
			SendStr(L"Coord[3].TM=5", _TERMINAL);
			SendStr(L"B7R", _TERMINAL);
			break;
		}
		Sleep(1000);
		break;
	case FB_11_AXIS:
		SendStr(L"#1j/", _TERMINAL);
		SendStr(L"#2j/", _TERMINAL);
		SendStr(L"#3j/", _TERMINAL);
		SendStr(L"#4j/", _TERMINAL);
		SendStr(L"#5j/", _TERMINAL);
		SendStr(L"#6j/", _TERMINAL);
		SendStr(L"#7j/", _TERMINAL);
		SendStr(L"#8j/", _TERMINAL);
		SendStr(L"#9j/", _TERMINAL);
		SendStr(L"#10j/", _TERMINAL);
		SendStr(L"#11j/", _TERMINAL);
		SendStr(L"enable plc 2", _TERMINAL);
		break;
	case FB_TWIN_TOWER:
		SendStr(L"#1j/", _TERMINAL);
		SendStr(L"#2j/", _TERMINAL);
		SendStr(L"#3j/", _TERMINAL);
		SendStr(L"#4j/", _TERMINAL);
		SendStr(L"#5j/", _TERMINAL);
		if(theApp.m_LastSettings.nManipulatorType[0] == TWIN_TOWER_FINGER)	SendStr(L"#6j/", _TERMINAL);
		SendStr(L"#7j/", _TERMINAL);
		SendStr(L"#8j/", _TERMINAL);
		SendStr(L"#9j/", _TERMINAL);
		SendStr(L"#10j/", _TERMINAL);
		SendStr(L"#11j/", _TERMINAL);
		SendStr(L"enable plc 2", _TERMINAL);
		break;
	case RAILWAY_AXLE:
		switch (theApp.m_LastSettings.nCoordMask) {
		case 1:
			SendStr(L"&1", _TERMINAL);
			SendStr(L"#1->I", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"Coord[1].SegLinToPvt=0", _TERMINAL);
			SendStr(L"Coord[1].TA=100", _TERMINAL);
			SendStr(L"Coord[1].Td=100", _TERMINAL);
			SendStr(L"Coord[1].TS=50", _TERMINAL);
			SendStr(L"Coord[1].TM=5", _TERMINAL);
			SendStr(L"B7R", _TERMINAL);
			break;
		case 2:
			SendStr(L"&2", _TERMINAL);
			SendStr(L"#1->I", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"Coord[2].SegLinToPvt=0", _TERMINAL);
			SendStr(L"Coord[2].TA=100", _TERMINAL);
			SendStr(L"Coord[2].Td=100", _TERMINAL);
			SendStr(L"Coord[2].TS=50", _TERMINAL);
			SendStr(L"Coord[2].TM=5", _TERMINAL);
			SendStr(L"B7R", _TERMINAL);
			break;
		case 3:
			SendStr(L"&3", _TERMINAL);
			SendStr(L"#1->I", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"Coord[3].SegLinToPvt=0", _TERMINAL);
			SendStr(L"Coord[3].TA=100", _TERMINAL);
			SendStr(L"Coord[3].Td=100", _TERMINAL);
			SendStr(L"Coord[3].TS=50", _TERMINAL);
			SendStr(L"Coord[3].TM=5", _TERMINAL);
			SendStr(L"B7R", _TERMINAL);
			break;
		}
		break;
	case TANK_5AXIS:
		SendStr(L"&1", _TERMINAL);
		SendStr(L"#1->I", _TERMINAL);
		SendStr(L"#2->I", _TERMINAL);
		SendStr(L"#3->I", _TERMINAL);
		SendStr(L"#4->I", _TERMINAL);
		SendStr(L"#5->I", _TERMINAL);
		SendStr(L"Coord[1].SegLinToPvt=0", _TERMINAL);
		SendStr(L"Coord[1].TA=100", _TERMINAL);
		SendStr(L"Coord[1].Td=100", _TERMINAL);
		SendStr(L"Coord[1].TS=50", _TERMINAL);
		SendStr(L"Coord[1].TM=5", _TERMINAL);
		Sleep(100);
		SendStr(L"B7R", _TERMINAL);
		break;
	case TANK_2AXIS:
		SendStr(L"&1", _TERMINAL);
		SendStr(L"#1->I", _TERMINAL);
		SendStr(L"#2->I", _TERMINAL);
		SendStr(L"Coord[1].SegLinToPvt=0", _TERMINAL);
		SendStr(L"Coord[1].TA=50", _TERMINAL);
		SendStr(L"Coord[1].Td=50", _TERMINAL);
		SendStr(L"Coord[1].TS=50", _TERMINAL);
		SendStr(L"Coord[1].TM=5", _TERMINAL);
		Sleep(100);
		SendStr(L"B7R", _TERMINAL);
		break;
	}
}

void CFieldBusController::SendFrameToolJoyMode(bool bForce)
{
	if ((bForce == true) || ((theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nFrameToolJoyMode) != m_nOldFrameToolJoyMode)) {
		if ((m_nOldFrameToolJoyMode = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nFrameToolJoyMode) > 0) {
			SendStr(L"FrameToolJoyMode=1", _TERMINAL);
		}
		else {
			SendStr(L"FrameToolJoyMode=0", _TERMINAL);
		}
	}
}


void CFieldBusController::Joystick(int nNotSafe)
{

	switch(theApp.m_Tank.nScannerDescription) {
	default:	JoystickWithKinematics();
		break;
	case FB_11_AXIS:
		JoystickWithoutKinematics();
		break;
	case FB_TWIN_TOWER:
		JoystickTwinTower();
		break;
	}
}

void CFieldBusController::JoystickTwinTower(int nNotSafe)
{
	int	nXTemp,nYTemp,nZTemp,nRTemp,nXtTemp,nYtTemp,nTemp,nSign;
	int	nJoystickAction=0,nSide;
	int	nJoystickGain,nMaxDeviation;
	bool bDeadManSZ = false;
	bool bDeadManXY = false;
	AxisData *pGAxis=NULL;
	AxisData *pHAxis=NULL;
	float fXBridge;
	float fReqPos;

	#define	LEFT_JOYSTICK	1
	#define	RIGHT_JOYSTICK	2
	#define	BOTH_JOYSTICK	3
	#define	MAIN_AXES	0
	#define	TIP_AXES	1


	theApp.m_Motors.EnsureJoystickValuesAreCurrent();

	nXTemp=theApp.m_Motors.m_nJoyValue[0];
	nYTemp=theApp.m_Motors.m_nJoyValue[1];
	nZTemp=theApp.m_Motors.m_nJoyValue[2];
	nXtTemp=theApp.m_Motors.m_nJoyValue[0];
	nYtTemp=theApp.m_Motors.m_nJoyValue[1];
	nRTemp=theApp.m_Motors.m_nJoyValue[3];
	switch(theApp.m_Tank.nJoystickSource) {
	case WIRED_SERIAL:								nSide = BOTH_JOYSTICK;
		if(theApp.m_Motors.m_nJoyValue[4]<15)		nSide = RIGHT_JOYSTICK;
		if(theApp.m_Motors.m_nJoyValue[4] > 220)	nSide = LEFT_JOYSTICK;
		nMaxDeviation = 50;
		bDeadManXY = ~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit1;
		bDeadManSZ = ~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit0;
		if(~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nMainManipBit) nJoystickAction = TIP_AXES;
		break;
	case WIRELESS_SERIAL:													nSide = BOTH_JOYSTICK;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nLeftSideBit)	nSide = LEFT_JOYSTICK;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nRightSideBit) nSide = RIGHT_JOYSTICK;
		nMaxDeviation = 138;
		bDeadManXY = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit1;
		bDeadManSZ = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit0;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nMainManipBit) nJoystickAction = TIP_AXES;
		break;
	}

	if(m_nGeneralInput & theApp.m_Tank.nDockedBit) {
		bDeadManXY = true;
		bDeadManSZ = true;
	}

	if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nScannerFixtureBit) {
		theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Axes[theApp.m_Tank.nYLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
		theApp.m_Axes[theApp.m_Tank.nZLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[2];
		theApp.m_Axes[theApp.m_Tank.nXtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
		theApp.m_Axes[theApp.m_Tank.nFingerLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
		theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Axes[theApp.m_Tank.nYRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
		theApp.m_Axes[theApp.m_Tank.nZRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[2];
		theApp.m_Axes[theApp.m_Tank.nXtRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Axes[theApp.m_Tank.nYtRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
	} else {
		if(bDeadManXY == false) {
			theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
			theApp.m_Axes[theApp.m_Tank.nYLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
			theApp.m_Axes[theApp.m_Tank.nFingerLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
			theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
			theApp.m_Axes[theApp.m_Tank.nYRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
			theApp.m_Axes[theApp.m_Tank.nXtRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
			theApp.m_Axes[theApp.m_Tank.nYtRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];

			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nFingerLeft].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis-1,0.0f);
		
		}
		if(bDeadManSZ == false) {
			theApp.m_Axes[theApp.m_Tank.nZLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[2];
			theApp.m_Axes[theApp.m_Tank.nZRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[2];
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1,0.0f);
		}
	}

	for(int nWhichAxis=0; nWhichAxis<4; nWhichAxis++) {
		pGAxis=NULL;
		pHAxis=NULL;
		switch(nWhichAxis) {
		case 0:	nTemp=nXTemp;
			break;
		case 1:	nTemp=nYTemp;
			break;
		case 2:	nTemp=nZTemp;
			break;
		case 3:	nTemp=nRTemp;
			break;
		};

		switch(nSide) {
		case LEFT_JOYSTICK:
			if(nJoystickAction==MAIN_AXES) {
				switch(nWhichAxis) {
				case 0:	pGAxis=&theApp.m_Axes[theApp.m_Tank.nXLeft];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYLeft];
					break;
				case 2:pGAxis=&theApp.m_Axes[theApp.m_Tank.nZLeft];
					break;
				case 3:pGAxis=NULL;
					break;
				};
			} else {
				switch(nWhichAxis) {
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXtLeft];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYtLeft];
					break;
				case 2:
				case 3:pGAxis=NULL;
					break;
				};
			};
			break;
		case RIGHT_JOYSTICK:
			if(nJoystickAction==MAIN_AXES) {
				switch(nWhichAxis) {
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXRight];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYRight];
					break;
				case 2:pGAxis=&theApp.m_Axes[theApp.m_Tank.nZRight];
					break;
				case 3:pGAxis=NULL;
					break;
				};
			} else {
				switch(nWhichAxis) {
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXtRight];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYtRight];
					break;
				case 2:
				case 3:pGAxis=NULL;
					break;
				};
			};
			break;
		case BOTH_JOYSTICK:
			if(nJoystickAction==MAIN_AXES) {
				switch(nWhichAxis) {
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXLeft];
					if(theApp.m_LastSettings.bEnableSide[1]) pHAxis=&theApp.m_Axes[theApp.m_Tank.nXRight];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYLeft];
					if(theApp.m_LastSettings.bEnableSide[1]) pHAxis=&theApp.m_Axes[theApp.m_Tank.nYRight];
					break;
				case 2:pGAxis=&theApp.m_Axes[theApp.m_Tank.nZLeft];
					if(theApp.m_LastSettings.bEnableSide[1]) pHAxis=&theApp.m_Axes[theApp.m_Tank.nZRight];
					break;
				case 3:pGAxis=NULL;
					break;
				};
			} else {
				switch(nWhichAxis) {
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXtLeft];
					if(theApp.m_LastSettings.bEnableSide[1]) pHAxis=&theApp.m_Axes[theApp.m_Tank.nXtRight];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYtLeft];
					if(theApp.m_LastSettings.bEnableSide[1]) pHAxis=&theApp.m_Axes[theApp.m_Tank.nYtRight];
					break;
				case 2:
				case 3:pGAxis=NULL;
					break;
				};
			};
			break;
		}

		if(pGAxis==NULL) return;


		fReqPos=0;
		nTemp -= pGAxis->nJoystickZero;
		(pGAxis->bJoystickReverse==TRUE)?(nTemp *= -1):(nTemp);
		if(pGAxis->nJoystickGain==0)	pGAxis->nJoystickGain = 100;

		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nSpeedBit) {
			nJoystickGain = pGAxis->nJoystickGain / 10;
		} else {
			nJoystickGain = pGAxis->nJoystickGain;
		}

		if((abs(nTemp) > nMaxDeviation) || (abs(nTemp)<=theApp.m_Tank.nJoystickDeadBand) || (nNotSafe == 1)) {
			nTemp = 0;
			if(abs(nXTemp-theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero)<=theApp.m_Tank.nJoystickDeadBand) {
				if(abs(nYTemp-theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero)<=theApp.m_Tank.nJoystickDeadBand) {
					if(abs(nZTemp-theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero)<=theApp.m_Tank.nJoystickDeadBand) {
						theApp.m_Motors.GetHeadPos(&m_CpJoystickHead);		//I don't know what this is here for
					};
				};
			};
		} else {
			nSign = nTemp;
			fReqPos=(float)((abs(nTemp)-theApp.m_Tank.nJoystickDeadBand)*nJoystickGain);
			if(nSign<0) fReqPos*= -1.0f;
			fReqPos /= 100.0f;

		};


		if(nSide != BOTH_JOYSTICK) {
			theApp.m_Axes[theApp.m_Tank.nXLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nXRight].nPos - theApp.m_Axes[theApp.m_Tank.nXLeft].nPos;
			theApp.m_Axes[theApp.m_Tank.nYLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nYRight].nPos - theApp.m_Axes[theApp.m_Tank.nYLeft].nPos;
			theApp.m_Axes[theApp.m_Tank.nZLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nZRight].nPos - theApp.m_Axes[theApp.m_Tank.nZLeft].nPos;
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nXtRight].nPos - theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos;
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nYtRight].nPos - theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos;
		} else {
			if(pHAxis != NULL) {
				WriteJogSpeed(pHAxis->nPhysicalAxis-1,fReqPos);
			}
		}
		WriteJogSpeed(pGAxis->nPhysicalAxis-1,fReqPos);

		if(nJoystickAction==MAIN_AXES) {
//			WriteDpram(PMAC_JOYSTICK_TIP_MODE, 0);
			theApp.m_Motors.GetHeadPos(&m_CpHead);
			theApp.m_Kinematics.SurfaceFromHead(&m_CpHead, &m_CpSurface,PORTSIDE);
			theApp.m_Kinematics.SurfaceFromHead(&m_CpHead, &m_CpSurface,STARBOARD);
		} else {
			if(theApp.m_LastSettings.nJoystickTipMode==1) {
				theApp.m_Motors.GetHeadPos(&m_CpHead);
				m_CpSurface.Side0.fXt=m_CpHead.Side0.fXt;
				m_CpSurface.Side0.fYt=m_CpHead.Side0.fYt;
				m_CpSurface.Side1.fXt=m_CpHead.Side1.fXt;
				m_CpSurface.Side1.fYt=m_CpHead.Side1.fYt;
				theApp.m_Kinematics.HeadFromSurface(&m_CpSurface, &m_CpHead, PORTSIDE);
				theApp.m_Kinematics.HeadFromSurface(&m_CpSurface, &m_CpHead, STARBOARD);

				if(theApp.m_LastSettings.bEnableSide[0] == theApp.m_LastSettings.bEnableSide[1] == 1) {
					fXBridge = ((m_CpHead.Side0.fX + m_CpHead.Side1.fX) / 2.0f) + theApp.m_Kinematics.m_fXOffset[PORTSIDE];
					theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)(fXBridge/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
					theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)((m_CpHead.Side0.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
					theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos=(int)((m_CpHead.Side1.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
				} else {
					if(theApp.m_LastSettings.bEnableSide[0] == 1) {
						theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)(m_CpHead.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
						theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)(0.0/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
					}
					if(theApp.m_LastSettings.bEnableSide[1] == 1) {
						theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)(m_CpHead.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
						theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)(0.0/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
					}
				}
				theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos = (int)(m_CpHead.Side0.fY / theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos = (int)(m_CpHead.Side0.fZ / theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos = (int)(m_CpHead.Side1.fY / theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos = (int)(m_CpHead.Side1.fZ / theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
				WriteDesirePos(theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis-1,theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset);
				WriteDesirePos(theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis-1,theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset);
				WriteDesirePos(theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis-1,theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset);
				WriteDesirePos(theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1,theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset);
				WriteDesirePos(theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1,theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset);
				WriteDesirePos(theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1,theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset);
//				WriteDpram(PMAC_JOYSTICK_TIP_MODE, 1);
			}
		}
	}
}

void CFieldBusController::JoystickWithoutKinematics(int nNotSafe)
{
	int	nXTemp,nYTemp,nZTemp,nRTemp,nXtTemp,nYtTemp,nTemp,nSign;
	int	nJoystickAction=0,nSide;
	int	nJoystickGain,nMaxDeviation;
	bool bDeadManSZ = false;
	bool bDeadManXY = false;
	AxisData *pGAxis=NULL;
	AxisData *pHAxis=NULL;
	float fXBridge;
	float fReqPos;

	#define	LEFT_JOYSTICK	1
	#define	RIGHT_JOYSTICK	2
	#define	BOTH_JOYSTICK	3
	#define	MAIN_AXES	0
	#define	TIP_AXES	1


	theApp.m_Motors.EnsureJoystickValuesAreCurrent();

	nXTemp=theApp.m_Motors.m_nJoyValue[0];
	nYTemp=theApp.m_Motors.m_nJoyValue[1];
	nZTemp=theApp.m_Motors.m_nJoyValue[2];
	nXtTemp=theApp.m_Motors.m_nJoyValue[0];
	nYtTemp=theApp.m_Motors.m_nJoyValue[1];
	nRTemp=theApp.m_Motors.m_nJoyValue[3];
	switch(theApp.m_Tank.nJoystickSource) {
	case WIRED_SERIAL:							nSide = BOTH_JOYSTICK;
		if(theApp.m_Motors.m_nJoyValue[4]<15)		nSide = RIGHT_JOYSTICK;
		if(theApp.m_Motors.m_nJoyValue[4] > 220)	nSide = LEFT_JOYSTICK;
		nMaxDeviation = 50;
		bDeadManXY = ~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit1;
		bDeadManSZ = ~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit0;
		if(~theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nMainManipBit) nJoystickAction = TIP_AXES;
		break;
	case WIRELESS_SERIAL:												nSide = BOTH_JOYSTICK;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nLeftSideBit)	nSide = LEFT_JOYSTICK;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nRightSideBit) nSide = RIGHT_JOYSTICK;
		nMaxDeviation = 138;
		bDeadManXY = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit1;
		bDeadManSZ = theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit0;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nMainManipBit) nJoystickAction = TIP_AXES;
		break;
	}

	if(m_nGeneralInput & theApp.m_Tank.nDockedBit) {
		bDeadManXY = true;
		bDeadManSZ = true;
	}

	if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nScannerFixtureBit) {
		theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Axes[theApp.m_Tank.nYLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
		theApp.m_Axes[theApp.m_Tank.nZLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[2];
		theApp.m_Axes[theApp.m_Tank.nXtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Axes[theApp.m_Tank.nYtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
		theApp.m_Axes[theApp.m_Tank.nFingerLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
		theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Axes[theApp.m_Tank.nYRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
		theApp.m_Axes[theApp.m_Tank.nZRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[2];
		theApp.m_Axes[theApp.m_Tank.nXtRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
		theApp.m_Axes[theApp.m_Tank.nYtRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
	} else {
		if(bDeadManXY == false) {
			theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
			theApp.m_Axes[theApp.m_Tank.nYLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
			theApp.m_Axes[theApp.m_Tank.nXRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
			theApp.m_Axes[theApp.m_Tank.nYRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
			theApp.m_Axes[theApp.m_Tank.nXtRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[0];
			theApp.m_Axes[theApp.m_Tank.nYtRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];
			theApp.m_Axes[theApp.m_Tank.nFingerLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[1];

			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nFingerLeft].nPhysicalAxis-1,0.0f);
		
		}
		if(bDeadManSZ == false) {
			theApp.m_Axes[theApp.m_Tank.nZLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[2];
			theApp.m_Axes[theApp.m_Tank.nZRight].nJoystickZero=theApp.m_Motors.m_nJoyValue[2];
			theApp.m_Axes[theApp.m_Tank.nRLeft].nJoystickZero=theApp.m_Motors.m_nJoyValue[3];
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1,0.0f);
			WriteJogSpeed(theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis-1,0.0f);
		}
	}

	for(int nWhichAxis=0; nWhichAxis<4; nWhichAxis++) {
		pGAxis=NULL;
		pHAxis=NULL;
		switch(nWhichAxis) {
		case 0:	nTemp=nXTemp;
			break;
		case 1:	nTemp=nYTemp;
			break;
		case 2:	nTemp=nZTemp;
			break;
		case 3:	nTemp=nRTemp;
			break;
		};

		switch(nSide) {
		case LEFT_JOYSTICK:
			if(nJoystickAction==MAIN_AXES) {
				switch(nWhichAxis) {
				case 0:	pGAxis=&theApp.m_Axes[theApp.m_Tank.nXLeft];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYLeft];
					break;
				case 2:pGAxis=&theApp.m_Axes[theApp.m_Tank.nZLeft];
					break;
				case 3:pGAxis=NULL;
					break;
				};
			} else {
				switch(nWhichAxis) {
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXtLeft];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYtLeft];
					break;
				case 2:
				case 3:pGAxis=NULL;
					break;
				};
			};
			break;
		case RIGHT_JOYSTICK:
			if(nJoystickAction==MAIN_AXES) {
				switch(nWhichAxis) {
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXRight];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYRight];
					break;
				case 2:pGAxis=&theApp.m_Axes[theApp.m_Tank.nZRight];
					break;
				case 3:pGAxis=NULL;
					break;
				};
			} else {
				switch(nWhichAxis) {
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXtRight];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYtRight];
					break;
				case 2:
				case 3:pGAxis=NULL;
					break;
				};
			};
			break;
		case BOTH_JOYSTICK:
			if(nJoystickAction==MAIN_AXES) {
				switch(nWhichAxis) {
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXLeft];
					if(theApp.m_LastSettings.bEnableSide[1]) pHAxis=&theApp.m_Axes[theApp.m_Tank.nXRight];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYLeft];
					if(theApp.m_LastSettings.bEnableSide[1]) pHAxis=&theApp.m_Axes[theApp.m_Tank.nYRight];
					break;
				case 2:pGAxis=&theApp.m_Axes[theApp.m_Tank.nZLeft];
					if(theApp.m_LastSettings.bEnableSide[1]) pHAxis=&theApp.m_Axes[theApp.m_Tank.nZRight];
					break;
				case 3:pGAxis=&theApp.m_Axes[theApp.m_Tank.nRLeft];
					break;
				};
			} else {
				switch(nWhichAxis) {
				case 0:pGAxis=&theApp.m_Axes[theApp.m_Tank.nXtLeft];
					if(theApp.m_LastSettings.bEnableSide[1]) pHAxis=&theApp.m_Axes[theApp.m_Tank.nXtRight];
					break;
				case 1:pGAxis=&theApp.m_Axes[theApp.m_Tank.nYtLeft];
					if(theApp.m_LastSettings.bEnableSide[1]) pHAxis=&theApp.m_Axes[theApp.m_Tank.nYtRight];
					break;
				case 2:
				case 3:pGAxis=NULL;
					break;
				};
			};
			break;
		}

		if(pGAxis==NULL) return;


		fReqPos=0;
		nTemp -= pGAxis->nJoystickZero;
		(pGAxis->bJoystickReverse==TRUE)?(nTemp *= -1):(nTemp);
		if(pGAxis->nJoystickGain==0)	pGAxis->nJoystickGain = 100;

		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nSpeedBit) {
			nJoystickGain = pGAxis->nJoystickGain / 10;
		} else {
			nJoystickGain = pGAxis->nJoystickGain;
		}

		if((abs(nTemp) > nMaxDeviation) || (abs(nTemp)<=theApp.m_Tank.nJoystickDeadBand) || (nNotSafe == 1)) {
			nTemp = 0;
			if(abs(nXTemp-theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero)<=theApp.m_Tank.nJoystickDeadBand) {
				if(abs(nYTemp-theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero)<=theApp.m_Tank.nJoystickDeadBand) {
					if(abs(nZTemp-theApp.m_Axes[theApp.m_Tank.nXLeft].nJoystickZero)<=theApp.m_Tank.nJoystickDeadBand) {
						theApp.m_Motors.GetHeadPos(&m_CpJoystickHead);		//I don't know what this is here for
					};
				};
			};
		} else {
			nSign = nTemp;
			fReqPos=(float)((abs(nTemp)-theApp.m_Tank.nJoystickDeadBand)*nJoystickGain);
			if(nSign<0) fReqPos*= -1.0f;
			fReqPos /= 100.0f;

		};


		if(nSide != BOTH_JOYSTICK) {
			theApp.m_Axes[theApp.m_Tank.nXLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nXRight].nPos - theApp.m_Axes[theApp.m_Tank.nXLeft].nPos;
			theApp.m_Axes[theApp.m_Tank.nYLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nYRight].nPos - theApp.m_Axes[theApp.m_Tank.nYLeft].nPos;
			theApp.m_Axes[theApp.m_Tank.nZLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nZRight].nPos - theApp.m_Axes[theApp.m_Tank.nZLeft].nPos;
			theApp.m_Axes[theApp.m_Tank.nXtLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nXtRight].nPos - theApp.m_Axes[theApp.m_Tank.nXtLeft].nPos;
			theApp.m_Axes[theApp.m_Tank.nYtLeft].nRLdiff = theApp.m_Axes[theApp.m_Tank.nYtRight].nPos - theApp.m_Axes[theApp.m_Tank.nYtLeft].nPos;
		} else {
			if(pHAxis != NULL) {
				WriteJogSpeed(pHAxis->nPhysicalAxis-1,fReqPos);
			}
		}
		WriteJogSpeed(pGAxis->nPhysicalAxis-1,fReqPos);

		if(nJoystickAction==MAIN_AXES) {
//			WriteDpram(PMAC_JOYSTICK_TIP_MODE, 0);
			theApp.m_Motors.GetHeadPos(&m_CpHead);
			theApp.m_Kinematics.SurfaceFromHead(&m_CpHead, &m_CpSurface,PORTSIDE);
			theApp.m_Kinematics.SurfaceFromHead(&m_CpHead, &m_CpSurface,STARBOARD);
		} else {
			if(theApp.m_LastSettings.nJoystickTipMode==1) {
				theApp.m_Motors.GetHeadPos(&m_CpHead);
				m_CpSurface.Side0.fXt=m_CpHead.Side0.fXt;
				m_CpSurface.Side0.fYt=m_CpHead.Side0.fYt;
				m_CpSurface.Side1.fXt=m_CpHead.Side1.fXt;
				m_CpSurface.Side1.fYt=m_CpHead.Side1.fYt;
				theApp.m_Kinematics.HeadFromSurface(&m_CpSurface, &m_CpHead, PORTSIDE);
				theApp.m_Kinematics.HeadFromSurface(&m_CpSurface, &m_CpHead, STARBOARD);

				if(theApp.m_LastSettings.bEnableSide[0] == theApp.m_LastSettings.bEnableSide[1] == 1) {
					fXBridge = ((m_CpHead.Side0.fX + m_CpHead.Side1.fX) / 2.0f) + theApp.m_Kinematics.m_fXOffset[PORTSIDE];
					theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)(fXBridge/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
					theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)((m_CpHead.Side0.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
					theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos=(int)((m_CpHead.Side1.fX-fXBridge)/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
				} else {
					if(theApp.m_LastSettings.bEnableSide[0] == 1) {
						theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)(m_CpHead.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
						theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)(0.0/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
					}
					if(theApp.m_LastSettings.bEnableSide[1] == 1) {
						theApp.m_Axes[theApp.m_Tank.nXBridge].nDesirePos=(int)(m_CpHead.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXBridge].fStepSize);
						theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos=(int)(0.0/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
					}
				}
				theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos = (int)(m_CpHead.Side0.fY / theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos = (int)(m_CpHead.Side0.fZ / theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos = (int)(m_CpHead.Side1.fY / theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
				theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos = (int)(m_CpHead.Side1.fZ / theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
				WriteDesirePos(theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis-1,theApp.m_Axes[theApp.m_Tank.nXLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset);
				WriteDesirePos(theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis-1,theApp.m_Axes[theApp.m_Tank.nYLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset);
				WriteDesirePos(theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis-1,theApp.m_Axes[theApp.m_Tank.nZLeft].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset);
				WriteDesirePos(theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis-1,theApp.m_Axes[theApp.m_Tank.nXRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset);
				WriteDesirePos(theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis-1,theApp.m_Axes[theApp.m_Tank.nYRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset);
				WriteDesirePos(theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis-1,theApp.m_Axes[theApp.m_Tank.nZRight].nDesirePos+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset);
//				WriteDpram(PMAC_JOYSTICK_TIP_MODE, 1);
			}
		}
	}
}

void CFieldBusController::WriteJogSpeed(int nAxis,float fSpeed)
{
	CString Buff;

	if(fLastJogSpeed[nAxis] - fSpeed) {
		switch(theApp.m_Tank.nScannerDescription) {
		default:

			switch(nAxis) {
			case 0: Buff.Format(L"X1CommandJog=%.02f",fSpeed);
				break;
			case 1: Buff.Format(L"Y1CommandJog=%.02f",fSpeed);
				break;
			case 2: Buff.Format(L"Z1CommandJog=%.02f",fSpeed);
				break;
			case 3: Buff.Format(L"Xt1CommandJog=%.02f",fSpeed);
				break;
			case 4: Buff.Format(L"Yt1CommandJog=%.02f",fSpeed);
				break;
			case 5: Buff.Format(L"X2CommandJog=%.02f",fSpeed);
				break;
			case 6: Buff.Format(L"Y2CommandJog=%.02f",fSpeed);
				break;
			case 7: Buff.Format(L"Z2CommandJog=%.02f",fSpeed);
				break;
			case 8: Buff.Format(L"Xt2CommandJog=%.02f",fSpeed);
				break;
			case 9: Buff.Format(L"Yt2CommandJog=%.02f",fSpeed);
				break;
			case 10: Buff.Format(L"RCommandJog=%.02f",fSpeed);
				break;
			}
			break;
		case FB_TWIN_TOWER:
			switch(nAxis) {
			case 0: Buff.Format(L"X1CommandJog=%.02f",fSpeed);
				break;
			case 1: Buff.Format(L"Y1CommandJog=%.02f",fSpeed);
				break;
			case 2: Buff.Format(L"Z1CommandJog=%.02f",fSpeed);
				break;
			case 3: Buff.Format(L"Xt1CommandJog=%.02f",fSpeed);
				break;
			case 4: Buff.Format(L"Yt1CommandJog=%.02f",fSpeed);
				break;
			case 5:	Buff.Empty();
				break;
			case 6: Buff.Format(L"X2CommandJog=%.02f",fSpeed);
				break;
			case 7: Buff.Format(L"Y2CommandJog=%.02f",fSpeed);
				break;
			case 8: Buff.Format(L"Z2CommandJog=%.02f",fSpeed);
				break;
			case 9: Buff.Format(L"Xt2CommandJog=%.02f",fSpeed);
				break;
			case 10: Buff.Format(L"Yt2CommandJog=%.02f",fSpeed);
				break;
			}
			break;
		}

		SendStr(Buff, _TERMINAL);
		fLastJogSpeed[nAxis] = fSpeed;
	}
}


void CFieldBusController::WriteDesirePos(int nAxis,int nDesirePos,int nForce)
{
	CString Buff;

	if((nLastDesirePos[nAxis] - nDesirePos) || (nForce == FORCE)){
		switch(theApp.m_Tank.nScannerDescription) {
		default:
			switch(nAxis) {
			case 0: Buff.Format(L"X1DesirePos=%d",nDesirePos);
				break;
			case 1: Buff.Format(L"Y1DesirePos=%d",nDesirePos);
				break;
			case 2: Buff.Format(L"Z1DesirePos=%d",nDesirePos);
				break;
			case 3: Buff.Format(L"Xt1DesirePos=%d",nDesirePos);
				break;
			case 4: Buff.Format(L"Yt1DesirePos=%d",nDesirePos);
				break;
			case 5: Buff.Format(L"X2DesirePos=%d",nDesirePos);
				break;
			case 6: Buff.Format(L"Y2DesirePos=%d",nDesirePos);
				break;
			case 7: Buff.Format(L"Z2DesirePos=%d",nDesirePos);
				break;
			case 8: Buff.Format(L"Xt2DesirePos=%d",nDesirePos);
				break;
			case 9: Buff.Format(L"Yt2DesirePos=%d",nDesirePos);
				break;
			case 10: Buff.Format(L"RDesirePos=%d",nDesirePos);
				break;
			}
			break;
		case FB_TWIN_TOWER:
			switch(nAxis) {
			case 0: Buff.Format(L"X1DesirePos=%d",nDesirePos);
				break;
			case 1: Buff.Format(L"Y1DesirePos=%d",nDesirePos);
				break;
			case 2: Buff.Format(L"Z1DesirePos=%d",nDesirePos);
				break;
			case 3: Buff.Format(L"Xt1DesirePos=%d",nDesirePos);
				break;
			case 4: Buff.Format(L"Yt1DesirePos=%d",nDesirePos);
				break;
			case 5: Buff.Format(L"F1DesirePos=%d",nDesirePos);
				break;
			case 6: Buff.Format(L"X2DesirePos=%d",nDesirePos);
				break;
			case 7: Buff.Format(L"Y2DesirePos=%d",nDesirePos);
				break;
			case 8: Buff.Format(L"Z2DesirePos=%d",nDesirePos);
				break;
			case 9: Buff.Format(L"Xt2DesirePos=%d",nDesirePos);
				break;
			case 10: Buff.Format(L"Yt2DesirePos=%d",nDesirePos);
				break;
			case 11: Buff.Format(L"RDesirePos=%d",nDesirePos);
				break;
			}
			break;
		}
		if(Buff.GetLength() > 0) SendStr(Buff, _TERMINAL);
		nLastDesirePos[nAxis] = nDesirePos;
	}
}


void CFieldBusController::JoystickWithKinematics(int nNotSafe)
{
	int	nJoystickAction=0,nSide;
	float	fJoystickGain,fReqPos;
	int		ii,nAxis,nMaxDeviation,nTemp;
	bool bDeadManSZ = false;
	bool bDeadManXY = false;
	bool bDeadMan[4];
	static bool bLastDeadMan[4] = { false, false, false, false };
	float fJogCommand[5];
	CString Buff;
	CCoord Cp;
	D3DXVECTOR3 vec;
	D3DXMATRIXA16 matWorld,matRotY,matRotZ;
	D3DXQUATERNION qNormal;

	#define	LEFT_JOYSTICK	1
	#define	RIGHT_JOYSTICK	2
	#define	BOTH_JOYSTICK	3
	#define	MAIN_AXES	0
	#define	TIP_AXES	1

	ZeroMemory(fJogCommand, sizeof fJogCommand);
	ZeroMemory(bDeadMan, sizeof bDeadMan);

	if(theApp.m_Tank.nJoystickDeadBand < 1) theApp.m_Tank.nJoystickDeadBand = 1;

	theApp.m_Motors.EnsureJoystickValuesAreCurrent();

	switch(theApp.m_Tank.nJoystickSource) {
	case WIRED_SI10:
	case WIRED_SERIAL:
		break;
	case WIRELESS_SERIAL:												nSide = BOTH_JOYSTICK;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nLeftSideBit)	nSide = LEFT_JOYSTICK;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nRightSideBit) nSide = RIGHT_JOYSTICK;
		nMaxDeviation = 138;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit1) {
			bDeadMan[0]=true;
			bDeadMan[1]=true;
		}
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nDeadManBit0) {
			bDeadMan[2]=true;
			bDeadMan[3]=true;
		}
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nMainManipBit) nJoystickAction = TIP_AXES;
		SendFrameToolJoyMode(false);

		break;
	}
	if (bDeadMan[0] - bLastDeadMan[0] || bDeadMan[1] - bLastDeadMan[1]) {
		Buff.Format(L"XJogMove=0.0");	SendStr(Buff, _TERMINAL);
		Buff.Format(L"YJogMove=0.0");	SendStr(Buff, _TERMINAL);
		Buff.Format(L"ZJogMove=0.0");	SendStr(Buff, _TERMINAL);
		Buff.Format(L"RJogMove=0.0");	SendStr(Buff, _TERMINAL);
		Buff.Format(L"RXJogMove=0.0");	SendStr(Buff, _TERMINAL);
		Buff.Format(L"RYJogMove=0.0");	SendStr(Buff, _TERMINAL);
		Buff.Format(L"RZJogMove=0.0");	SendStr(Buff, _TERMINAL);
		Buff.Format(L"B_XJogMove=0.0");	SendStr(Buff, _TERMINAL);
		Buff.Format(L"B_YJogMove=0.0");	SendStr(Buff, _TERMINAL);
		Buff.Format(L"B_ZJogMove=0.0");	SendStr(Buff, _TERMINAL);
		Buff.Format(L"B_RJogMove=0.0");	SendStr(Buff, _TERMINAL);
		Buff.Format(L"B_RXJogMove=0.0");	SendStr(Buff, _TERMINAL);
		Buff.Format(L"B_RYJogMove=0.0");	SendStr(Buff, _TERMINAL);
		Buff.Format(L"B_RZJogMove=0.0");	SendStr(Buff, _TERMINAL);
	}
	CopyMemory(bLastDeadMan, bDeadMan, sizeof bLastDeadMan);

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nScannerFixtureBit) {
			for(ii=0;ii<5;ii++) {
				m_nJoyZeroValue[ii] = theApp.m_Motors.m_nJoyValue[ii];
			}
		} else {
			for(nAxis=0;nAxis<4;nAxis++) {
				if(bDeadMan[nAxis] == false) {
					m_nJoyZeroValue[nAxis] = theApp.m_Motors.m_nJoyValue[nAxis];
				}
			}
		}
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nScannerFixtureBit) {
			SendRobotWeightings(1, false);
		} else {
			SendRobotWeightings(0, false);
		}

		for(nAxis=0;nAxis<4;nAxis++) {
			if(bDeadMan[nAxis] == false) {
				m_nJoyZeroValue[nAxis] = theApp.m_Motors.m_nJoyValue[nAxis];
			}
		}
		break;

	}

	for(nAxis=0; nAxis<4; nAxis++) {

		fJoystickGain = 1.0f;
		if(theApp.m_Motors.m_nSwitchJoyValue & theApp.m_Tank.nSpeedBit) fJoystickGain /= 10.0f;

		nTemp =  theApp.m_Motors.m_nJoyValue[nAxis] - m_nJoyZeroValue[nAxis];

		if((abs(nTemp) > nMaxDeviation) || (abs(nTemp)<=theApp.m_Tank.nJoystickDeadBand) || (nNotSafe == 1) || (bDeadMan[nAxis]==false)) {
			fJogCommand[nAxis]=0.0f;
		} else {
			fReqPos=(float)((abs(nTemp)-theApp.m_Tank.nJoystickDeadBand))*fJoystickGain;
			if(nTemp<0) fReqPos*= -1.0f;
			fJogCommand[nAxis]=fReqPos;
		};
	}

	if(nJoystickAction == 0) {

		for(nAxis=0;nAxis<4;nAxis++) {
			if(m_fLastJogCommand[nAxis] - fJogCommand[nAxis]) {
				m_fLastJogCommand[nAxis] = fJogCommand[nAxis];

				if((nSide == 1) || (nSide == 3)) {
					switch(nAxis) {
					case 0: Buff.Format(L"XJogMove=%.02f",fJogCommand[nAxis] * theApp.m_Robot[0].m_fJogDirSpeedX);
						SendStr(Buff, _TERMINAL);
						break;
					case 1: Buff.Format(L"YJogMove=%.02f",fJogCommand[nAxis] * theApp.m_Robot[0].m_fJogDirSpeedY);
						SendStr(Buff, _TERMINAL);
						break;
					case 2: Buff.Format(L"ZJogMove=%.02f",fJogCommand[nAxis] * theApp.m_Robot[0].m_fJogDirSpeedZ);
						SendStr(Buff, _TERMINAL);
						break;
					case 3: Buff.Format(L"RJogMove=%.02f",fJogCommand[nAxis] * theApp.m_Robot[0].m_fJogDirSpeedR);
						SendStr(Buff, _TERMINAL);
						break;
					}
				}
				if((nSide == 2) || (nSide == 3)) {
					switch(nAxis) {
					case 0: Buff.Format(L"B_XJogMove=%.02f",fJogCommand[nAxis] * theApp.m_Robot[1].m_fJogDirSpeedX);
						SendStr(Buff, _TERMINAL);
						break;
					case 1: Buff.Format(L"B_YJogMove=%.02f",fJogCommand[nAxis] * theApp.m_Robot[1].m_fJogDirSpeedY);
						SendStr(Buff, _TERMINAL);
						break;
					case 2: Buff.Format(L"B_ZJogMove=%.02f",fJogCommand[nAxis] * theApp.m_Robot[1].m_fJogDirSpeedZ);
						SendStr(Buff, _TERMINAL);
						break;
					case 3: Buff.Format(L"B_RJogMove=%.02f",fJogCommand[nAxis] * theApp.m_Robot[1].m_fJogDirSpeedR);
						SendStr(Buff, _TERMINAL);
						break;
					}
				}
			}
		}
	} else {
		//Manipulator

		for(nAxis=0;nAxis<3;nAxis++) {
			if(m_fLastJogCommand[nAxis] - fJogCommand[nAxis]) {
				m_fLastJogCommand[nAxis] = fJogCommand[nAxis];

				if((nSide == 1) || (nSide == 3)) {
					switch(nAxis) {
					case 0: Buff.Format(L"RXJogMove=%.02f",fJogCommand[nAxis]  * theApp.m_Robot[0].m_fJogDirSpeedHTip);
						SendStr(Buff, _TERMINAL);
						break;
					case 1: Buff.Format(L"RYJogMove=%.02f", fJogCommand[nAxis] * theApp.m_Robot[0].m_fJogDirSpeedVTip);
						SendStr(Buff, _TERMINAL);
						break;
					case 2: Buff.Format(L"RZJogMove=%.02f", fJogCommand[nAxis] * theApp.m_Robot[0].m_fJogDirSpeedTTip);
						SendStr(Buff, _TERMINAL);
						break;
					}
				}
				if((nSide == 2) || (nSide == 3)) {
					switch(nAxis) {
					case 0: Buff.Format(L"B_RXJogMove=%.02f",fJogCommand[nAxis]  * theApp.m_Robot[1].m_fJogDirSpeedHTip);
						SendStr(Buff, _TERMINAL);
						break;
					case 1: Buff.Format(L"B_RYJogMove=%.02f",fJogCommand[nAxis]  * theApp.m_Robot[1].m_fJogDirSpeedVTip);
						SendStr(Buff, _TERMINAL);
						break;
					case 2: Buff.Format(L"B_RZJogMove=%.02f", fJogCommand[nAxis] * theApp.m_Robot[1].m_fJogDirSpeedTTip);
						SendStr(Buff, _TERMINAL);
						break;
					}
				}
			}
		}
	}


}


void CFieldBusController::RequestCoordinates()
{

	m_bPeekCoordinateFound = false;

}


bool CFieldBusController::IsPeekCoordinateFound()
{

	return m_bPeekCoordinateFound;
}

void CFieldBusController::GetPeekCoordinate(CCoord *pCp)
{
	*pCp = m_CpPeek;
}


void CFieldBusController::EnableMotors()
{
	int	ii;
	CString Buff;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		for(ii=0;ii<2;ii++) {
//			DownloadRobotKinematics(ii);
			DownloadPPMACKinematics(ii);
			DownloadEndEffectorWithWaterPath(ii,true);
		}		
		Buff.Format(L"AmplifierEnable = 1 CtrlSide=%d Enable plc 3",theApp.m_LastSettings.nCoordMask);	//Plc 3 #1j/#2j/.....#nj/
		SendStr(Buff, _TERMINAL);
		break;
	case DUAL_ROBOT_9_PLUS_9:
		for(ii=0;ii<2;ii++) {
			DownloadRobotKinematics(ii);
			DownloadEndEffectorWithWaterPath(ii,true);
		}
		SendAxesIncluded(MOVETO_INCLUDE, true);
		Buff.Format(L"AmplifierEnable = 1 CtrlSide=%d Enable plc 3", theApp.m_LastSettings.nCoordMask);	//Plc 3 #1j/#2j/.....#nj/
		SendStr(Buff, _TERMINAL);
		break;
	case SPX_ROBOT:
		for (ii = 0; ii<2; ii++) {
			DownloadRobotKinematics(ii);
			DownloadEndEffectorWithWaterPath(ii, true);
		}
		if (theApp.m_LastSettings.nCoordMask == 1) {
			SendStr(L"#2j/#3j/#4j/#5j/#6j/#7j/#8j/#9j/", _TERMINAL);
		}
		if (theApp.m_LastSettings.nCoordMask == 2) {
			SendStr(L"#13j/#14j/#15j/#16j/#17j/#18j/#19j/", _TERMINAL);
		}
		if (theApp.m_LastSettings.nCoordMask == 3) {
			SendStr(L"#2j/#3j/#4j/#5j/#6j/#7j/#8j/#9j/", _TERMINAL);
			SendStr(L"#13j/#14j/#15j/#16j/#17j/#18j/#19j/", _TERMINAL);
		}
		break;
	case FB_11_AXIS:
		SendStr(L"#1j/#2j/#3j/#4j/#5j/", _TERMINAL);
		SendStr(L"#6j/#7j/#8j/#9j/#10j/", _TERMINAL);
		break;
	case FB_TWIN_TOWER:
		if(theApp.m_LastSettings.nCoordMask == 1) {
			SendStr(L"#1j/#2j/#3j/#4j/#5j/", _TERMINAL);
		}
		if(theApp.m_LastSettings.nCoordMask == 2) {
			SendStr(L"#7j/#8j/#9j/#10j/#11j/", _TERMINAL);
		}
		if(theApp.m_LastSettings.nCoordMask == 3) {
			SendStr(L"#1j/#2j/#3j/#4j/#5j/", _TERMINAL);
			SendStr(L"#7j/#8j/#9j/#10j/#11j/", _TERMINAL);
		}
		break;
	};

}

void CFieldBusController::KillMotors()
{
	CString Buff;

	KillAllMotionPrograms();

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		Buff.Format(L"AmplifierEnable = 0 CtrlSide=%d Enable plc 3",theApp.m_LastSettings.nCoordMask);
		SendStr(Buff, _TERMINAL);
		break;
	case SPX_ROBOT:
		if (theApp.m_LastSettings.nCoordMask & 1) {
			SendStr(L"#1k#2k#3k#4k#5k#6k#7k#8k#9k", _TERMINAL);
		}
		if (theApp.m_LastSettings.nCoordMask & 2) {
			SendStr(L"#11k#12k#13k#14k#15k#16k#17k#18k#19k#21k", _TERMINAL);
		}
		break;
	case FB_11_AXIS:
		SendStr(L"#1k#2k#3k#4k#5k", _TERMINAL);
		SendStr(L"#6k#7k#8k#9k#10k#11k", _TERMINAL);
		break;
	case FB_TWIN_TOWER:
		SendStr(L"#1k#2k#3k#4k#5k#6k", _TERMINAL);
		SendStr(L"#7k#8k#9k#10k#11k", _TERMINAL);
		break;
	};
}


void CFieldBusController::StartEthercat()
{

	SendStr(L"enable plc 1", _TERMINAL);
}

void CFieldBusController::SuspendEthercat()
{
	SendStr(L"Motor[0].PhaseCtrl=0", _TERMINAL);
	SendStr(L"Motor[0].ServoCtrl=0", _TERMINAL);
	Sleep(100);
	SendStr(L"Ecat[0].Enable=0", _TERMINAL);
	Sleep(100);
}

void CFieldBusController::ToggleRRWaterSpray()
{
	CString Buff;

	switch(theApp.m_Ethercat.ReportWaterSpray()) {
	case 0:	m_nGeneralOutput |= 0x000c;
		break;
	case 1:	m_nGeneralOutput &= 0xff3;
		break;
	}
	Buff.Format(L"ACC65E[0].DataReg[3]=$%02x",m_nGeneralOutput);
	SendStr(Buff, _TERMINAL);
}

void CFieldBusController::RRWaterSpray(int nOffOn)
{
	CString	Buff;

	switch(nOffOn) {
	case 0:		m_nGeneralOutput &= 0xff3;
		break;
	case 1:		m_nGeneralOutput |= 0x000c;
		break;
	}
	Buff.Format(L"ACC65E[0].DataReg[3]=$%02x",m_nGeneralOutput);
	SendStr(Buff, _TERMINAL);
}

bool CFieldBusController::ReportAirStatus(int nMask, int nSide)
{
	bool bFlag;

	switch(theApp.m_Tank.nScannerDescription) {
	default:	(m_nGeneralOutput & nMask) ? bFlag = TRUE : bFlag = FALSE;
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case FB_TWIN_TOWER:
	case DUAL_ROBOT:
	case SPX_ROBOT:
	case RAILWAY_AXLE:
	case TANK_5AXIS:
	case TANK_2AXIS:
		m_nAirKnifeStatus[nSide] & 1 ? bFlag = TRUE : bFlag = FALSE;
		break;
	case TWIN_TOWER_KINEMATIC:
		break;
	}

	return bFlag;

}

void CFieldBusController::AirStart(int nMask, int nSide)
{
	CString Buff;

	m_nGeneralOutput |= nMask;
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		Buff.Format(L"ACC65E[0].DataReg[3]=$%02x",m_nGeneralOutput);
		SendStr(Buff, _TERMINAL);
		break;
	case FB_11_AXIS:	
		Buff.Format(L"ACC65E[0].DataReg[3]=$%02x",m_nGeneralOutput);
		SendStr(Buff, _TERMINAL);
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case FB_TWIN_TOWER:
	case DUAL_ROBOT:
	case SPX_ROBOT:
	case RAILWAY_AXLE:
	case TANK_5AXIS:
	case TANK_2AXIS:
		switch(nSide) {
		case 0:	SendStr(L"AirKnifeDemand0 = 1", _TERMINAL);
			break;
		case 1:	SendStr(L"AirKnifeDemand1 = 1", _TERMINAL);
			break;
		}
		break;
	case TWIN_TOWER_KINEMATIC:
		break;
	};
}

void CFieldBusController::AirStop(int nMask, int nSide)
{
	CString Buff;

	m_nGeneralOutput &= ~nMask;
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		Buff.Format(L"ACC65E[0].DataReg[3]=$%02x",m_nGeneralOutput);
		SendStr(Buff, _TERMINAL);
		break;
	case FB_11_AXIS:	
		Buff.Format(L"ACC65E[0].DataReg[3]=$%02x",m_nGeneralOutput);
		SendStr(Buff, _TERMINAL);
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case FB_TWIN_TOWER:
	case DUAL_ROBOT:
	case SPX_ROBOT:
	case RAILWAY_AXLE:
	case TANK_5AXIS:
	case TANK_2AXIS:
		switch(nSide) {
		case 0:	SendStr(L"AirKnifeDemand0 = 0", _TERMINAL);
			break;
		case 1:	SendStr(L"AirKnifeDemand1 = 0", _TERMINAL);
			break;
		}
		break;
	case TWIN_TOWER_KINEMATIC:
		break;
	};
}

void CFieldBusController::EnableLights(int nFlag)
{
	CString Buff;
	int nn = m_nGeneralOutput;

	int nMask = ~(theApp.m_Tank.nAndonOkay | theApp.m_Tank.nAndonCycleRunning | theApp.m_Tank.nAndonWarning | theApp.m_Tank.nAndonFault);
	nn&=nMask;
	nn|= nFlag;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		nn&=nMask;
		nn|= nFlag;

	//	if(nn != m_nGeneralOutput) {
		m_nGeneralOutput = nn;
		Buff.Format(L"ACC65E[0].DataReg[3]=$%02x",m_nGeneralOutput);
		SendStr(Buff, _TERMINAL);
	//	}
		break;
	case FB_11_AXIS:	
		Buff.Format(L"ACC65E[0].DataReg[3]=$%02x",m_nGeneralOutput);
		SendStr(Buff, _TERMINAL);
		break;
	case FB_TWIN_TOWER:
	case SPX_ROBOT:
 	case DUAL_ROBOT_9_PLUS_9:
	case RAILWAY_AXLE:
	case TANK_5AXIS:
	case TANK_2AXIS:
	case TWIN_TOWER_KINEMATIC:
		break;
	};

}
void CFieldBusController::ResetKarlMorganOperationMode()
{

	SendStr(L"enable plc 10", _TERMINAL);
}


void CFieldBusController::Origin(int nWhichAxis)
{
	CString Buff;

	KillAllMotionPrograms();

	switch(theApp.m_Tank.nScannerDescription) {
	case TWIN_TOWER_KINEMATIC:
		switch (nWhichAxis) {
		case ORIGIN_X_LEFT:	SendStr(L"enable plc 16", _TERMINAL);
			break;
		case ORIGIN_Y_LEFT:	SendStr(L"enable plc 17", _TERMINAL);
			break;
		case ORIGIN_Z_LEFT:	SendStr(L"enable plc 18", _TERMINAL);
			break;
		case ORIGIN_XT_LEFT:	SendStr(L"enable plc 19", _TERMINAL);
			break;
		case ORIGIN_YT_LEFT:	SendStr(L"enable plc 20", _TERMINAL);
			break;
		case ORIGIN_X_RIGHT:	SendStr(L"enable plc 11", _TERMINAL);
			break;
		case ORIGIN_Y_RIGHT:	SendStr(L"enable plc 12", _TERMINAL);
			break;
		case ORIGIN_Z_RIGHT:	SendStr(L"enable plc 13", _TERMINAL);
			break;
		case ORIGIN_XT_RIGHT:	SendStr(L"enable plc 14", _TERMINAL);
			break;
		case ORIGIN_YT_RIGHT:	SendStr(L"enable plc 15", _TERMINAL);
			break;
		case ORIGIN_ALL:
			SendStr(L"enable plc 11", _TERMINAL);
			SendStr(L"enable plc 12", _TERMINAL);
			SendStr(L"enable plc 13", _TERMINAL);
			SendStr(L"enable plc 14", _TERMINAL);
			SendStr(L"enable plc 15", _TERMINAL);
			SendStr(L"enable plc 16", _TERMINAL);
			SendStr(L"enable plc 17", _TERMINAL);
			SendStr(L"enable plc 18", _TERMINAL);
			SendStr(L"enable plc 19", _TERMINAL);
			SendStr(L"enable plc 20", _TERMINAL);
			break;
		case ORIGIN_ALL_LEFT:
			SendStr(L"enable plc 16", _TERMINAL);
			SendStr(L"enable plc 17", _TERMINAL);
			SendStr(L"enable plc 18", _TERMINAL);
			SendStr(L"enable plc 19", _TERMINAL);
			SendStr(L"enable plc 20", _TERMINAL);
			break;
		case ORIGIN_ALL_RIGHT:
			SendStr(L"enable plc 11", _TERMINAL);
			SendStr(L"enable plc 12", _TERMINAL);
			SendStr(L"enable plc 13", _TERMINAL);
			SendStr(L"enable plc 14", _TERMINAL);
			SendStr(L"enable plc 15", _TERMINAL);
			break;
		}

		break;
	case FB_11_AXIS:
		switch(nWhichAxis) {
		case ORIGIN_X_LEFT:	SendStr(L"enable plc 11", _TERMINAL);
			break;
		case ORIGIN_Y_LEFT:	SendStr(L"enable plc 12", _TERMINAL);
			break;
		case ORIGIN_Z_LEFT:	SendStr(L"enable plc 13", _TERMINAL);
			break;
		case ORIGIN_X_RIGHT:	SendStr(L"enable plc 14", _TERMINAL);
			break;
		case ORIGIN_Y_RIGHT:	SendStr(L"enable plc 15", _TERMINAL);
			break;
		case ORIGIN_Z_RIGHT:	SendStr(L"enable plc 16", _TERMINAL);
			break;
		case ORIGIN_ALL:
			SendStr(L"enable plc 11", _TERMINAL);
			SendStr(L"enable plc 12", _TERMINAL);
			SendStr(L"enable plc 13", _TERMINAL);
			SendStr(L"enable plc 14", _TERMINAL);
			SendStr(L"enable plc 15", _TERMINAL);
			SendStr(L"enable plc 16", _TERMINAL);
			break;
		case ORIGIN_ALL_LEFT:
			SendStr(L"enable plc 11", _TERMINAL);
			SendStr(L"enable plc 12", _TERMINAL);
			SendStr(L"enable plc 13", _TERMINAL);
			break;
		case ORIGIN_ALL_RIGHT:
			SendStr(L"enable plc 14", _TERMINAL);
			SendStr(L"enable plc 15", _TERMINAL);
			SendStr(L"enable plc 16", _TERMINAL);
			break;
		case ORIGIN_R_LEFT:	SendStr(L"enable plc 17", _TERMINAL);
			break;
		}

		break;
	case SPX_ROBOT:
		switch(nWhichAxis) {
		case ORIGIN_X_LEFT:
			break;
		case ORIGIN_Y_LEFT:
			break;
		case ORIGIN_Z_LEFT:
			break;
		case ORIGIN_X_RIGHT:
			break;
		case ORIGIN_Y_RIGHT:
			break;
		case ORIGIN_Z_RIGHT:
			break;
		case ORIGIN_ALL:
			break;
		case ORIGIN_ALL_LEFT:
			break;
		case ORIGIN_ALL_RIGHT:
			break;
		case ORIGIN_R_LEFT:	SendStr(L"enable plc 19", _TERMINAL);
			break;
		}
		break;
	case RAILWAY_AXLE:
		Buff.Format(L"HomeAxesMask = %d", nWhichAxis);
		SendStr(Buff, _TERMINAL);
		SendStr(L"enable plc 11", _TERMINAL);
		break;
	case TANK_5AXIS:
		Buff.Format(L"HomeAxesMask = %d", nWhichAxis);
		SendStr(Buff, _TERMINAL);
		SendStr(L"enable plc 11", _TERMINAL);
		break;
	case TANK_2AXIS:
		break;
	}
}

void CFieldBusController::KillAllMotionPrograms()
{
	int nAxis;

	switch(theApp.m_Tank.nScannerDescription) {
	case FB_TWIN_TOWER:
		SendStr(L"DISABLE PLC 2", _TERMINAL);
		SendStr(L"&1 Q", _TERMINAL);
		SendStr(L"&1 A", _TERMINAL);
		SendStr(L"&2 Q", _TERMINAL);
		SendStr(L"&2 A", _TERMINAL);
		SendStr(L"&3 Q", _TERMINAL);
		SendStr(L"&3 A", _TERMINAL);

		SendStr(L"UNDEFINE ALL", _TERMINAL);
		WaitForFreeCoordinateSystems();
		for(nAxis=0;nAxis<32;nAxis++) {
			if(theApp.m_Axes[nAxis].nMotorType == SERVO) {
				theApp.m_Axes[nAxis].bEnabled=FALSE;
			}
		}
		SendStr(L"delete all lookahead", _TERMINAL);
		break;
	case FB_11_AXIS:
		SendStr(L"DISABLE PLC 2..31", _TERMINAL);

		SendStr(L"&1 Q", _TERMINAL);
		SendStr(L"&1 A", _TERMINAL);
		SendStr(L"&2 Q", _TERMINAL);
		SendStr(L"&2 A", _TERMINAL);
		SendStr(L"&3 Q", _TERMINAL);
		SendStr(L"&3 A", _TERMINAL);

		SendStr(L"UNDEFINE ALL", _TERMINAL);
		WaitForFreeCoordinateSystems();
		for(nAxis=0;nAxis<32;nAxis++) {
			if(theApp.m_Axes[nAxis].nMotorType == SERVO) {
				theApp.m_Axes[nAxis].bEnabled=FALSE;
			}
		}
		SendStr(L"delete all lookahead", _TERMINAL);
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case DUAL_ROBOT:
	case RAILWAY_AXLE:
	case TANK_5AXIS:
	case TANK_6AXIS:
	case TANK_2AXIS:
	case TWIN_TOWER_KINEMATIC:

		SendStr(L"DISABLE PLC 10..31", _TERMINAL);

		SendStr(L"ProgramEnable=0", _TERMINAL);

		SendStr(L"&1 Q", _TERMINAL);
		SendStr(L"&1 A", _TERMINAL);
		SendStr(L"&2 Q", _TERMINAL);
		SendStr(L"&2 A", _TERMINAL);
		SendStr(L"&3 Q", _TERMINAL);
		SendStr(L"&3 A", _TERMINAL);

		SendStr(L"UNDEFINE ALL", _TERMINAL);
		SendStr(L"delete all lookahead", _TERMINAL);
		break;
	}

}

void CFieldBusController::EnableAxes(int nAction)
{
	CString Buff;

	switch (theApp.m_Tank.nScannerDescription) {
	case TANK_5AXIS:
	case RAILWAY_AXLE: 	
	case TANK_2AXIS:
	case TWIN_TOWER_KINEMATIC:
		EnableMotors();
		return;
		break;
	}

	switch(nAction) {
	case BOTH_MOVETO:
	case BOTH_POLY_MOVE:
	case BOTH_MOVETO_WITHOUT_R:
		SendStr(L"&1", _TERMINAL);
		EnableAxis(theApp.m_Tank.nXLeft,0,L"X");
		EnableAxis(theApp.m_Tank.nYLeft,0,L"Y");
		EnableAxis(theApp.m_Tank.nZLeft,0,L"Z");
		EnableAxis(theApp.m_Tank.nXtLeft,0,L"A");
		EnableAxis(theApp.m_Tank.nYtLeft,0,L"B");
		if(theApp.m_LastSettings.nManipulatorType[0] == TWIN_TOWER_FINGER)	EnableAxis(theApp.m_Tank.nFingerLeft,0,L"FF");
		EnableAxis(theApp.m_Tank.nXRight,0,L"XX");
		EnableAxis(theApp.m_Tank.nYRight,0,L"YY");
		EnableAxis(theApp.m_Tank.nZRight,0,L"ZZ");
		EnableAxis(theApp.m_Tank.nXtRight,0,L"AA");
		EnableAxis(theApp.m_Tank.nYtRight,0,L"BB");
		break;
	case BOTH_MOVETO_WITH_R:
		SendStr(L"&1", _TERMINAL);
		EnableAxis(theApp.m_Tank.nXLeft,0,L"X");
		EnableAxis(theApp.m_Tank.nYLeft,0,L"Y");
		EnableAxis(theApp.m_Tank.nZLeft,0,L"Z");
		EnableAxis(theApp.m_Tank.nXtLeft,0,L"A");
		EnableAxis(theApp.m_Tank.nYtLeft,0,L"B");
		if(theApp.m_LastSettings.nManipulatorType[0] == TWIN_TOWER_FINGER)	EnableAxis(theApp.m_Tank.nFingerLeft,0,L"FF");
		EnableAxis(theApp.m_Tank.nXRight,0,L"XX");
		EnableAxis(theApp.m_Tank.nYRight,0,L"YY");
		EnableAxis(theApp.m_Tank.nZRight,0,L"ZZ");
		EnableAxis(theApp.m_Tank.nXtRight,0,L"AA");
		EnableAxis(theApp.m_Tank.nYtRight,0,L"BB");
		EnableAxis(theApp.m_Tank.nRLeft,0,L"C");
		break;
	case LEFT_MOVETO:
	case LEFT_POLY_MOVE:
		SendStr(L"&1", _TERMINAL);
		EnableAxis(theApp.m_Tank.nXLeft,0,L"X");
		EnableAxis(theApp.m_Tank.nYLeft,0,L"Y");
		EnableAxis(theApp.m_Tank.nZLeft,0,L"Z");
		EnableAxis(theApp.m_Tank.nXtLeft,0,L"A");
		EnableAxis(theApp.m_Tank.nYtLeft,0,L"B");
		if(theApp.m_LastSettings.nManipulatorType[0] == TWIN_TOWER_FINGER)	EnableAxis(theApp.m_Tank.nFingerLeft,0,L"FF");
		break;
	case RIGHT_MOVETO:
	case RIGHT_POLY_MOVE:
		SendStr(L"&2", _TERMINAL);
		EnableAxis(theApp.m_Tank.nXRight,0,L"X");
		EnableAxis(theApp.m_Tank.nYRight,0,L"Y");
		EnableAxis(theApp.m_Tank.nZRight,0,L"Z");
		EnableAxis(theApp.m_Tank.nXtRight,0,L"A");
		EnableAxis(theApp.m_Tank.nYtRight,0,L"B");
		break;
	case ENABLE_JOYSTICK:
		EnableAxis(theApp.m_Tank.nXLeft);
		EnableAxis(theApp.m_Tank.nYLeft);
		EnableAxis(theApp.m_Tank.nZLeft);
		EnableAxis(theApp.m_Tank.nXtLeft);
		EnableAxis(theApp.m_Tank.nYtLeft);
		EnableAxis(theApp.m_Tank.nXRight);
		EnableAxis(theApp.m_Tank.nYRight);
		EnableAxis(theApp.m_Tank.nZRight);
		EnableAxis(theApp.m_Tank.nXtRight);
		EnableAxis(theApp.m_Tank.nYtRight);
//		SetFeedRate(theApp.m_LastSettings.fFeedRate);
//		DownloadSoftwareOverTravelLimits();
		break;

	}
	if(theApp.m_LastSettings.nBrakeScannerMode==1) theApp.m_Motors.ZBrakesEnable(FALSE,0);

}

void CFieldBusController::EnableAxis(int nAxisStruct,int nPhysicalAxisOffset, CString strCoordAxis)
{
	CString Buff;

	if(theApp.m_Axes[nAxisStruct].bAvailable==TRUE) {
		switch(theApp.m_Axes[nAxisStruct].eAmplifierType) {
		default:
			Buff.Format(_T("#%d->%s#%dj/"),theApp.m_Axes[nAxisStruct].nPhysicalAxis + nPhysicalAxisOffset,strCoordAxis ,theApp.m_Axes[nAxisStruct].nPhysicalAxis + nPhysicalAxisOffset);
			break;
		case GeoBrick:
			Buff.Format(_T("#%d->%s#%d$"),theApp.m_Axes[nAxisStruct].nPhysicalAxis + nPhysicalAxisOffset,strCoordAxis ,theApp.m_Axes[nAxisStruct].nPhysicalAxis + nPhysicalAxisOffset);
			break;
		}
		SendStr(Buff, _TERMINAL);
		theApp.m_Axes[nAxisStruct].bEnabled=theApp.m_Axes[nAxisStruct].bAvailable;
	}
}

void CFieldBusController::EnableAxis(int nAxisStruct)
{
	CString Buff;

	if(theApp.m_Axes[nAxisStruct].bAvailable==TRUE) {
		switch(theApp.m_Axes[nAxisStruct].eAmplifierType) {
		default: theApp.m_Axes[nAxisStruct].eAmplifierType = (AmplifierType)0;
			Buff.Format(_T("#%dj/"),theApp.m_Axes[nAxisStruct].nPhysicalAxis);
			break;
		case GeoBrick:
			Buff.Format(_T("#%d$"),theApp.m_Axes[nAxisStruct].nPhysicalAxis);
			break;
		}
		SendStr(Buff, _TERMINAL);
		theApp.m_Axes[nAxisStruct].bEnabled=theApp.m_Axes[nAxisStruct].bAvailable;
	}
}




void CFieldBusController::DisableLookAhead()
{

}

bool CFieldBusController::MoveTo(int nAction,int nFrame)
{
	int	ii,nMax,nTemp;
	int nNumberAxes = 11;
	int nPosArray[16];
	int	nDesirePos;
	int nPhysicalAxis;
	CString Buff;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		break;
	case FB_11_AXIS:
		SendStr(L"Motor[1].MaxSpeed = 174", _TERMINAL);
		SendStr(L"Motor[2].MaxSpeed = 30", _TERMINAL);
		SendStr(L"Motor[3].MaxSpeed = 30", _TERMINAL);
		SendStr(L"Motor[6].MaxSpeed = 174", _TERMINAL);
		SendStr(L"Motor[7].MaxSpeed = 30", _TERMINAL);
		SendStr(L"Motor[8].MaxSpeed = 30", _TERMINAL);
		SendStr(L"Motor[11].MaxSpeed = 150", _TERMINAL);

		SendStr(L"Coord[1].SegLinToPvt=0", _TERMINAL);
		break;
	case TANK_5AXIS:
		nNumberAxes = 5;
		break;
	case TANK_2AXIS:
		nNumberAxes = 2;
		break;
	case TWIN_TOWER_KINEMATIC:
		nNumberAxes = 10;
		break;
	}

	m_fTAMove[0] = m_fTDMove[0] = m_fTSMove[0] = m_fTMMove[0] = 0.0f;

	nMax=0;
	for(ii=0;ii<nNumberAxes;ii++) {
		if((theApp.m_Axes[ii].bAvailable) && (theApp.m_Axes[ii].nMotorType==SERVO)) {
			nPhysicalAxis = theApp.m_Axes[ii].nPhysicalAxis-1;

			if(theApp.m_Axes[ii].dStepsPerRev==0.0f) {
				switch(nFrame) {
				case _SURFACE:
				case _HEAD:
					nPosArray[nPhysicalAxis]=theApp.m_Kinematics.m_LeadScrew[ nPhysicalAxis ].ActualToMachine(theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset,theApp.m_PmacUser.m_nMachinePosition[nPhysicalAxis]);
					nTemp=CalcMoveTime(&theApp.m_Axes[ii],labs(theApp.m_Axes[ii].nDesirePos-theApp.m_Axes[ii].nPos));
					break;
				case _MACHINE:
					nPosArray[nPhysicalAxis]=theApp.m_Kinematics.m_LeadScrew[ nPhysicalAxis ].ActualToMachine(theApp.m_Axes[ii].nDesirePos,theApp.m_PmacUser.m_nMachinePosition[nPhysicalAxis]);
					nTemp=CalcMoveTime(&theApp.m_Axes[ii],labs((theApp.m_Axes[ii].nDesirePos-theApp.m_Axes[ii].nOffset)-theApp.m_Axes[ii].nPos));
					break;
				};
				if((ii==theApp.m_Tank.nYtLeft) && (theApp.m_LastSettings.nManipulatorType[PORTSIDE]==ROTATE_DIF_THETA)) {
					nPosArray[nPhysicalAxis]+=theApp.m_Axes[theApp.m_Tank.nXtLeft].nDesirePos;
				}
				if((ii==theApp.m_Tank.nYtRight) && (theApp.m_LastSettings.nManipulatorType[STARBOARD]==ROTATE_DIF_THETA)) {
					nPosArray[nPhysicalAxis]+=theApp.m_Axes[theApp.m_Tank.nXtRight].nDesirePos;
				}

			} else {
				if(abs(theApp.m_Axes[ii].nDesirePos - theApp.m_Axes[ii].nPos) < (int)(theApp.m_Axes[ii].dStepsPerRev / 2.0f)) {
					nDesirePos = theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset+(int)((float)theApp.m_Axes[ii].nRevCounter * theApp.m_Axes[ii].dStepsPerRev);
				} else {

					if(theApp.m_Axes[ii].nDesirePos < theApp.m_Axes[ii].nPos) {
						nDesirePos = theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset+(int)(((float)theApp.m_Axes[ii].nRevCounter+1) * theApp.m_Axes[ii].dStepsPerRev);
					} else {
						nDesirePos = theApp.m_Axes[ii].nDesirePos+theApp.m_Axes[ii].nOffset+(int)(((float)theApp.m_Axes[ii].nRevCounter-1) * theApp.m_Axes[ii].dStepsPerRev);
					}

				}
				nPosArray[nPhysicalAxis] = nDesirePos;
				nTemp=CalcMoveTime(&theApp.m_Axes[ii],labs(theApp.m_Axes[ii].nDesirePos-theApp.m_Axes[ii].nPos));
			}
 			if(nTemp>nMax) nMax=nTemp;
		};
		WriteDesirePos(nPhysicalAxis,nPosArray[nPhysicalAxis],FORCE);
	};

	Buff.Format(L"Coord[1].TA=%.01f\r\n", m_fTAMove[0] * 100.0f / theApp.m_LastSettings.fFeedRate); SendStr(Buff, _TERMINAL);
	Buff.Format(L"Coord[1].TD=%.01f\r\n", m_fTAMove[0] * 100.0f / theApp.m_LastSettings.fFeedRate); SendStr(Buff, _TERMINAL);
	Buff.Format(L"Coord[1].TS=%.01f\r\n", m_fTSMove[0] * 100.0f / theApp.m_LastSettings.fFeedRate); SendStr(Buff, _TERMINAL);
	Buff.Format(L"Coord[1].TM=%.01f\r\n", m_fTMMove[0] * 100.0f / theApp.m_LastSettings.fFeedRate); SendStr(Buff, _TERMINAL);

	SendStr(L"B6R", _TERMINAL);

	if(nAction==WAIT) {
		Sleep(200);
		while(!IsMotionFinished()) {
			Sleep(1);
		};
	}

	return TRUE;
}

int CFieldBusController::CalcMoveTime(struct AxisData *pAxis,int	nTotalS)
{
	float	vel,accel;
	float	s0,s1,s4,ds;
	float	t0,t1,t4,dt;
	float	fts,fta,ftm;
	int	nTemp;


	s4=(float)nTotalS * pAxis->fStepSize;

	vel=pAxis->fMaxMoveSpeed*pAxis->fStepSize*1000.0f;

	accel=(1.0f/pAxis->fAcceleration)*pAxis->fStepSize;
	accel=(pAxis->fAcceleration*pAxis->fStepSize*1000.0f);

	if(accel==0.0) accel=1.0;

	t0=s0=0;
	t1=vel/accel;
	s1=0.5f * accel * (t1 * t1) /1000.0f;
	if((s1 * 2) > s4) {
		s1 = s4 / 2;
		t1=sqrtf((2.0f * s1) / accel * 1000.0f);
		t4= t1 * 2;
	} else {
		ds = (s4 - (s1 * 2));
		dt = (ds / vel) * 1000.0f;
		t4 = t1 * 2 + dt;
	};

	fts = t1 *0.1f;
	fta = t1 - 2.0f * fts;
	ftm = t4;

	if(m_fTSMove[0] < fts) m_fTSMove[0] = fts;
	if(m_fTAMove[0] < fta) m_fTAMove[0] = fta;
	if(m_fTMMove[0] < ftm) m_fTMMove[0] = ftm;

	nTemp=(int)t4;
	return nTemp;
}

int CFieldBusController::DownLoadPolyMoveCoordinates(CPolyCoord *ToolPath, int nDir)
{
	int	ii,jj;
	int	nX0,nY0,nZ0,nXt0,nYt0,nR0;
	int	nOffset;
	CCoord	Cp;
	int nArray[12];
	ZeroMemory(nArray,sizeof nArray);
	CString Buff;
	int nYtLeftAxis;

	switch(theApp.m_Tank.nScannerDescription) {
	default: nYtLeftAxis = theApp.m_Tank.nYtLeft;
		break;
	case FB_TWIN_TOWER:
		switch(theApp.m_LastSettings.nManipulatorType[0]) {
		default:  nYtLeftAxis = theApp.m_Tank.nYtLeft;
			break;
		case TWIN_TOWER_FINGER: nYtLeftAxis = theApp.m_Tank.nFingerLeft;
			break;
		}
		break;
	}




	nOffset=PMAC_DPRAM_DATA_START;

	if(nDir==0) {
		for(ii=0;ii<ToolPath->m_nCoordL;ii++) {
			Cp=ToolPath->m_pCp[ii];
			nX0=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
			nY0=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
			nZ0=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
			nXt0=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
			nYt0=(int)(Cp.Side0.fYt/theApp.m_Axes[nYtLeftAxis].fStepSize);
			nArray[0]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis - 1 ].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
			nArray[1]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis - 1 ].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[1]);
			nArray[2]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis - 1 ].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[2]);
			nArray[3]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis - 1 ].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[3]);
			nArray[4]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[nYtLeftAxis].nPhysicalAxis - 1 ].ActualToMachine(nYt0+theApp.m_Axes[nYtLeftAxis].nOffset,theApp.m_PmacUser.m_nMachinePosition[4]);
			nX0=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
			nY0=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
			nZ0=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
			nXt0=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
			nYt0=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
			nArray[5]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis - 1 ].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[5]);
			nArray[6]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis - 1 ].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[6]);
			nArray[7]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis - 1 ].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[7]);
			nArray[8]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis - 1 ].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[8]);
			nArray[9]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis - 1 ].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[9]);

			nR0=(int)(Cp.Side0.fR/theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize);
			nArray[10]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis - 1 ].ActualToMachine(nR0+theApp.m_Axes[theApp.m_Tank.nRLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[10]);

			Buff.Format(L"M%d=%d",100+ii*12,nArray[0]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",101+ii*12,nArray[1]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",102+ii*12,nArray[2]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",103+ii*12,nArray[3]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",104+ii*12,nArray[4]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",105+ii*12,nArray[5]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",106+ii*12,nArray[6]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",107+ii*12,nArray[7]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",108+ii*12,nArray[8]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",109+ii*12,nArray[9]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",110+ii*12,nArray[10]);	SendStr(Buff, _TERMINAL);
			if(((ii % 20) == 0) && (ii != 0)) {
				Sleep(100);
			};
		};
	} else {
		for(ii=0,jj=ToolPath->m_nCoordL-1;ii<ToolPath->m_nCoordL;ii++,jj--) {
			Cp=ToolPath->m_pCp[jj];
			nX0=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
			nY0=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
			nZ0=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
			nXt0=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
			nYt0=(int)(Cp.Side0.fYt/theApp.m_Axes[nYtLeftAxis].fStepSize);
			nArray[0]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis - 1 ].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
			nArray[1]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis - 1 ].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[1]);
			nArray[2]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis - 1 ].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[2]);
			nArray[3]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis - 1 ].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[3]);
			nArray[4]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[nYtLeftAxis].nPhysicalAxis - 1 ].ActualToMachine(nYt0+theApp.m_Axes[nYtLeftAxis].nOffset,theApp.m_PmacUser.m_nMachinePosition[4]);
			nX0=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
			nY0=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
			nZ0=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
			nXt0=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
			nYt0=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
			nArray[5]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis - 1 ].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[5]);
			nArray[6]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis - 1 ].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[6]);
			nArray[7]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis - 1 ].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[7]);
			nArray[8]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis - 1 ].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[8]);
			nArray[9]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis - 1 ].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[9]);

			nR0=(int)(Cp.Side0.fR/theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize);
			nArray[10]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis - 1 ].ActualToMachine(nR0+theApp.m_Axes[theApp.m_Tank.nRLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[10]);

			Buff.Format(L"M%d=%d",100+ii*12,nArray[0]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",101+ii*12,nArray[1]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",102+ii*12,nArray[2]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",103+ii*12,nArray[3]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",104+ii*12,nArray[4]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",105+ii*12,nArray[5]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",106+ii*12,nArray[6]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",107+ii*12,nArray[7]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",108+ii*12,nArray[8]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",109+ii*12,nArray[9]);		SendStr(Buff, _TERMINAL);
			Buff.Format(L"M%d=%d",110+ii*12,nArray[10]);	SendStr(Buff, _TERMINAL);
			if(((ii % 20) == 0) && (ii != 0)) {
				Sleep(100);
			};
		};
	};

	SendStr(L"CoordPtr=0", _TERMINAL);
	Buff.Format(L"NumberCoords=%d",ToolPath->m_nCoordL);	SendStr(Buff, _TERMINAL);

	return ii;

}

void CFieldBusController::SetupLookAhead()
{
	SendStr(L"&1 define lookahead 4000", _TERMINAL);
	SendStr(L"Coord[1].LHDistance=4024", _TERMINAL);
	SendStr(L"Coord[1].SegLinToPvt=0", _TERMINAL);
	SendStr(L"Coord[1].TA=10", _TERMINAL);
	SendStr(L"Coord[1].Td=10", _TERMINAL);
	SendStr(L"Coord[1].TS=0", _TERMINAL);
	SendStr(L"Coord[1].TM=20", _TERMINAL);
	SendStr(L"Coord[1].MaxFeedrate = 1000", _TERMINAL);

}

void CFieldBusController::StartPolyMoveWithLookAhead(int nn)
{

//	SetFeedRate(theApp.m_LastSettings.fFeedRate);
	SendStr(L"&1 B7 R", _TERMINAL);

}


////////////////////////////////////////////////////
// Pump
///////////////////////////////////////////////////

void CFieldBusController::ChangePumpSpeedDueToHeight()
{
	CCoord Cp;

	return;

	theApp.m_Motors.GetSurfacePos(&Cp);
	switch(theApp.m_nPrimarySide) {
	case 0:
		if(fabsf(Cp.Side0.fZ - m_CpSurfacePump.Side0.fZ) >= 10.0f) {
			SetPumpSpeed(theApp.m_Tank.nLeftPump, 0);
			m_CpSurfacePump.Side0.fZ = Cp.Side0.fZ;
		}
		if(fabsf(Cp.Side1.fZ - m_CpSurfacePump.Side1.fZ) >= 10.0f) {
			SetPumpSpeed(theApp.m_Tank.nRightPump, 0);
			m_CpSurfacePump.Side1.fZ = Cp.Side1.fZ;
		}
		break;
	case 1:
		if(fabsf(Cp.Side1.fZ - m_CpSurfacePump.Side1.fZ) >= 10.0f) {
			SetPumpSpeed(theApp.m_Tank.nLeftPump, 1);
			m_CpSurfacePump.Side1.fZ = Cp.Side1.fZ;
		}
		if(fabsf(Cp.Side0.fZ - m_CpSurfacePump.Side0.fZ) >= 10.0f) {
			SetPumpSpeed(theApp.m_Tank.nRightPump, 0);
			m_CpSurfacePump.Side0.fZ = Cp.Side0.fZ;
		}
		break;
	};

}


bool CFieldBusController::ReportPumpStatus(int nAxis, int nSide)
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_Axes[nAxis].nPhysicalAxis) {
		case 13: return theApp.m_bPumpOffOnStatus[0];
			break;
		case 14: return theApp.m_bPumpOffOnStatus[1];
			break;
		};
		break;
	case FB_11_AXIS:
		switch(theApp.m_Axes[nAxis].nPhysicalAxis) {
		case 12: return theApp.m_bPumpOffOnStatus[0];
			break;
		case 13: return theApp.m_bPumpOffOnStatus[1];
			break;
		};
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case SPX_ROBOT:
	case RAILWAY_AXLE:
	case TANK_5AXIS:
	case TANK_2AXIS:
	case TWIN_TOWER_KINEMATIC:
		return theApp.m_Ethercat.ReportPumpStatus(nAxis, nSide);
		break;
	}

	return true;
}

void CFieldBusController::SetPumpSpeed(int nWhichAxis, int nSide)
{
	CString Buff;
	float fScaleFactor = 1;
	CCoord Cp;


	switch(theApp.m_Tank.nScannerDescription) {
	default:

		switch(nWhichAxis) {
		case 9:
		case 12: fScaleFactor = theApp.m_Tank.fPumpScaleFactor[0];
			if(theApp.m_Tank.fPumpVerticalMultiplier[0]) {
				theApp.m_Motors.GetSurfacePos(&Cp);
				fScaleFactor = (float)((int)(fScaleFactor * (1.0f + (theApp.m_Tank.fPumpVerticalMultiplier[0] * Cp.Side[nSide].fZ / 1000.0f))));	//This is meant to be a float. Leave it alone
			}
			break;
		case 19:
		case 13: fScaleFactor = theApp.m_Tank.fPumpScaleFactor[1];
			if(theApp.m_Tank.fPumpVerticalMultiplier[0]) {
				theApp.m_Motors.GetSurfacePos(&Cp);
				fScaleFactor = (float)((int)(fScaleFactor * (1.0f + (theApp.m_Tank.fPumpVerticalMultiplier[1] * Cp.Side[nSide].fZ / 1000.0f))));	//This is meant to be a float. Leave it alone
			}
			break;
		}
		if(fScaleFactor<=0.0f) fScaleFactor = 0.01f;
		Buff.Format(_T("Motor[%d].JogSpeed=%.03f"),theApp.m_Axes[nWhichAxis].nPhysicalAxis,(double)theApp.m_Axes[nWhichAxis].nMaxSpeed/1000.0*fScaleFactor);
		SendStr(Buff, _TERMINAL);
		if(ReportPumpStatus(nWhichAxis, nSide) == TRUE) {
			Buff.Format(_T("#%dj+"),theApp.m_Axes[nWhichAxis].nPhysicalAxis);
			SendStr(Buff, _TERMINAL);
		}
		break;
	case FB_TWIN_TOWER:
	case DUAL_ROBOT:
	case SPX_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case RAILWAY_AXLE:
	case TANK_5AXIS:
	case TANK_2AXIS:
	case TWIN_TOWER_KINEMATIC:

		switch(nSide) {
		case 0:
			Buff.Format(L"PumpScale0=%.02f", theApp.m_Tank.fPumpScaleFactor[0]);
			SendStr(Buff, _TERMINAL);
			Buff.Format(L"HeightScale0=%.03f",theApp.m_Tank.fPumpVerticalMultiplier[0]);
			SendStr(Buff, _TERMINAL);
			Buff.Format(L"PumpSpeed0=%d",theApp.m_Axes[theApp.m_Tank.nLeftPump].nMaxSpeed);
			SendStr(Buff, _TERMINAL);
			break;
		case 1:
			Buff.Format(L"PumpScale0=%.02f", theApp.m_Tank.fPumpScaleFactor[1]);
			SendStr(Buff, _TERMINAL);
			Buff.Format(L"HeightScale0=%.03f",theApp.m_Tank.fPumpVerticalMultiplier[1]);
			SendStr(Buff, _TERMINAL);
			Buff.Format(L"PumpSpeed1=%d",theApp.m_Axes[theApp.m_Tank.nRightPump].nMaxSpeed);
			SendStr(Buff, _TERMINAL);
			break;
		};
		break;
	}

}

void CFieldBusController::PumpStart(int nAxis, int nSide)
{
	CString Buff;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_Axes[nAxis].nPhysicalAxis) {
		case 13: theApp.m_bPumpOffOnStatus[0] = true;
			break;
		case 14: theApp.m_bPumpOffOnStatus[1] = true;
			break;
		};
		break;
	case FB_11_AXIS:
		Buff.Format(_T("#%dJ+"),theApp.m_Axes[nAxis].nPhysicalAxis);
		SendStr(Buff, _TERMINAL);
		switch(theApp.m_Axes[nAxis].nPhysicalAxis) {
		case 12: theApp.m_bPumpOffOnStatus[0] = true;
			break;
		case 13: theApp.m_bPumpOffOnStatus[1] = true;
			break;
		};
		break;
	case FB_TWIN_TOWER:
	case DUAL_ROBOT:
	case SPX_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
	case RAILWAY_AXLE:
	case TANK_5AXIS:
	case TANK_2AXIS:
	case TWIN_TOWER_KINEMATIC:

		switch(nSide) {
		case 0:
			Buff.Format(L"PumpScale0=%.02f", theApp.m_Tank.fPumpScaleFactor[0]);
			SendStr(Buff, _TERMINAL);
			Buff.Format(L"HeightScale0=%.03f",theApp.m_Tank.fPumpVerticalMultiplier[0]);
			SendStr(Buff, _TERMINAL);
			Buff.Format(L"PumpMode0=%d", theApp.m_Tank.nPumpMode[0]);
			SendStr(Buff, _TERMINAL);
			SendStr(L"PumpDemand0=1", _TERMINAL);
			break;
		case 1:
			Buff.Format(L"PumpScale1=%.02f", theApp.m_Tank.fPumpScaleFactor[1]);
			SendStr(Buff, _TERMINAL);
			Buff.Format(L"HeightScale1=%.03f",theApp.m_Tank.fPumpVerticalMultiplier[1]);
			SendStr(Buff, _TERMINAL);
			Buff.Format(L"PumpMode1=%d", theApp.m_Tank.nPumpMode[1]);
			SendStr(Buff, _TERMINAL);
			SendStr(L"PumpDemand1=1", _TERMINAL);
			break;
		};
		break;
	}
}

void CFieldBusController::PumpStop(int nAxis, int nSide)
{
	CString Buff;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		switch(theApp.m_Axes[nAxis].nPhysicalAxis) {
		case 13: theApp.m_bPumpOffOnStatus[0] = false;
			break;
		case 14: theApp.m_bPumpOffOnStatus[1] = false;
			break;
		};
		break;
	case FB_11_AXIS:
		Buff.Format(_T("#%dk"),theApp.m_Axes[nAxis].nPhysicalAxis);
		SendStr(Buff, _TERMINAL);
		Sleep(2000);
		switch(theApp.m_Axes[nAxis].nPhysicalAxis) {
		case 12: theApp.m_bPumpOffOnStatus[0] = false; 
			SendStr(L"Enable plc 18", _TERMINAL);
			break;
		case 13: theApp.m_bPumpOffOnStatus[1] = false; 
			SendStr(L"Enable plc 19", _TERMINAL);
			break;
		};
		break;
	case DUAL_ROBOT_9_PLUS_9:
	case FB_TWIN_TOWER:
	case DUAL_ROBOT:
	case SPX_ROBOT:
	case RAILWAY_AXLE:
	case TANK_5AXIS:
	case TANK_2AXIS:
	case TWIN_TOWER_KINEMATIC:
		switch(nSide) {
		case 0:	SendStr(L"PumpDemand0=0", _TERMINAL);
			break;
		case 1:	SendStr(L"PumpDemand1=0", _TERMINAL);
			break;
		};
		break;
	}
}

void CFieldBusController::HomeAxisWithoutMoving(int nPhysicalAxis, int nOffset)
{
	CString Buff;

	Buff.Format(_T("#%dHMZ"), nPhysicalAxis);
	SendStr(Buff, _TERMINAL);
	if (nOffset != 0) {
		Sleep(500);
		Buff.Format(_T("Motor[%d].HomePos=Motor[%d].HomePos - %d"), nPhysicalAxis, nPhysicalAxis, nOffset);
		SendStr(Buff, _TERMINAL);
	}

}

bool CFieldBusController::IsItAtDesirePos()
{
	int	ii;
	int nPos;

	switch(theApp.m_Tank.nScannerDescription) {
	default:

		for(ii=0;ii<10;ii++) {
			nPos = theApp.m_Axes[ii].nPos+theApp.m_Axes[ii].nOffset;
			if(	abs(nPos - nLastDesirePos[ii]) > 1000) {
				return false;
			}
		}
		break;
	case FB_TWIN_TOWER:
		for(ii=0;ii<11;ii++) {
			nPos = theApp.m_Axes[ii].nPos+theApp.m_Axes[ii].nOffset;
			nPos = theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[ii].nPhysicalAxis-1 ].ActualToMachine(nPos,0);
			switch(ii) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
				if(	abs(nPos - nLastDesirePos[ii]) > 100) {
					return false;
				}
				break;
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				if(	abs(nPos - nLastDesirePos[ii+1]) > 100) {
					return false;
				}
				break;
			case 10:
				break;
			};
		};
		break;
	};

	return true;
}

void CFieldBusController::SetScanSpeed(float fReqSpeed)
{
	CString Buff;

	float fRate;
	AxisData *pnAxis[6] = { &theApp.m_Axes[theApp.m_Tank.nXLeft], &theApp.m_Axes[theApp.m_Tank.nYLeft], &theApp.m_Axes[theApp.m_Tank.nZLeft],
							  &theApp.m_Axes[theApp.m_Tank.nXRight], &theApp.m_Axes[theApp.m_Tank.nYRight], &theApp.m_Axes[theApp.m_Tank.nZRight] };

	for(int nAxis=0;nAxis<6;nAxis++) {
//		if(fReqSpeed < ((pnAxis[nAxis]->fMaxMoveSpeed * 1000.0f) * pnAxis[nAxis]->fStepSize)) {
			fRate = (fReqSpeed / pnAxis[nAxis]->fStepSize) / 1000.0f;
//		} else {
//			fRate = pnAxis[nAxis]->fMaxMoveSpeed;
//		}

		if(pnAxis[nAxis]->bAvailable==TRUE) {
			Buff.Format(L"Motor[%d].MaxSpeed=%.01f",pnAxis[nAxis]->nPhysicalAxis, fRate);	SendStr(Buff, _TERMINAL);
		}
	}

}

void CFieldBusController::RequestAllAccelerationsAndVelocities()
{
	CString Buff;
	
	for(int ii=0;ii<12;ii++) {
		Buff.Format(L"Motor[%d].MaxSpeed",theApp.m_Axes[ii].nPhysicalAxis);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"Motor[%d].InvAmax",theApp.m_Axes[ii].nPhysicalAxis);		SendStr(Buff, _TERMINAL);
	}

}

void CFieldBusController::ResetAllMaxSpeeds()
{
	CString Buff;
	
	for(int ii=0;ii<12;ii++) {
		Buff.Format(L"Motor[%d].InvAmax=%f",theApp.m_Axes[ii].nPhysicalAxis,1.0f / theApp.m_Axes[ii].fAcceleration);
		SendStr(Buff, _TERMINAL);

		Buff.Format(L"Motor[%d].InvDmax=%f",theApp.m_Axes[ii].nPhysicalAxis,1.0f / theApp.m_Axes[ii].fAcceleration);
		SendStr(Buff, _TERMINAL);

		Buff.Format(L"Motor[%d].MaxSpeed=%f",theApp.m_Axes[ii].nPhysicalAxis,theApp.m_Axes[ii].fMaxMoveSpeed);
		SendStr(Buff, _TERMINAL);
	}

}

void CFieldBusController::StartTurntableCartesianScanningAxes(CCoord Cp)
{
	int	nTm=100;
	int	nTs=5;
	int	nTa=1;
	CString Buff;

	UpdateTurntableCartesianScanningAxes(Cp);

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		SendStr(L"Coord[1].TA=100", _TERMINAL);
		SendStr(L"Coord[1].Td=100", _TERMINAL);
		SendStr(L"Coord[1].TS=100", _TERMINAL);
		SendStr(L"Coord[1].TM=100", _TERMINAL);


		switch(theApp.m_PmacUser.m_nScanSide) {
		case 0:
			SendStr(L"&1 B8 R", _TERMINAL);
			break;
		case 1:
			SendStr(L"&1 B8 R", _TERMINAL);
			break;
		case 2:
			SendStr(L"&1 B8 R", _TERMINAL);
			break;
		}
		break;
	case SPX_ROBOT:
		KillAllMotionPrograms();
		Buff.Format(L"FeedRate=%.01f",theApp.m_LastSettings.fMoveSpeed * theApp.m_LastSettings.fFeedRate / 100.0f);
		SendStr(Buff, _TERMINAL);
		SendRobotWeightings(0,true);
		switch(theApp.m_LastSettings.nCoordMask) {
		case 1:
			SendStr(L"&1", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"#8->I", _TERMINAL);
			SendStr(L"#9->I", _TERMINAL);
			SendStr(L"B8R", _TERMINAL);
			break;
		case 2:
			SendStr(L"&2", _TERMINAL);
			SendStr(L"#13->I", _TERMINAL);
			SendStr(L"#14->I", _TERMINAL);
			SendStr(L"#15->I", _TERMINAL);
			SendStr(L"#16->I", _TERMINAL);
			SendStr(L"#17->I", _TERMINAL);
			SendStr(L"#18->I", _TERMINAL);
			SendStr(L"#19->I", _TERMINAL);
			SendStr(L"B8R", _TERMINAL);
			break;
		case 3:
			SendStr(L"&3", _TERMINAL);
			SendStr(L"#2->I", _TERMINAL);
			SendStr(L"#3->I", _TERMINAL);
			SendStr(L"#4->I", _TERMINAL);
			SendStr(L"#5->I", _TERMINAL);
			SendStr(L"#6->I", _TERMINAL);
			SendStr(L"#7->I", _TERMINAL);
			SendStr(L"#8->I", _TERMINAL);
			SendStr(L"#9->I", _TERMINAL);
			SendStr(L"#13->I", _TERMINAL);
			SendStr(L"#14->I", _TERMINAL);
			SendStr(L"#15->I", _TERMINAL);
			SendStr(L"#16->I", _TERMINAL);
			SendStr(L"#17->I", _TERMINAL);
			SendStr(L"#18->I", _TERMINAL);
			SendStr(L"#19->I", _TERMINAL);
			SendStr(L"&3 define lookahead 5000", _TERMINAL);
			SendStr(L"Coord[3].LHDistance=500", _TERMINAL);
			SendStr(L"Coord[3].SegLinToPvt=1", _TERMINAL);
			SendStr(L"Coord[3].TA=20", _TERMINAL);
			SendStr(L"Coord[3].Td=20", _TERMINAL);
			SendStr(L"Coord[3].TS=20", _TERMINAL);
			SendStr(L"Coord[3].TM=2", _TERMINAL);
			SendStr(L"Coord[3].FeedTime=500", _TERMINAL);
			SendStr(L"B8R", _TERMINAL);
			break;
		}
		break;
	case TANK_2AXIS:
	case TANK_5AXIS:
	case TANK_6AXIS:
		KillAllMotionPrograms();
		Buff.Format(L"FeedRate=%.01f", theApp.m_LastSettings.fMoveSpeed * theApp.m_LastSettings.fFeedRate / 100.0f);
		SendStr(Buff, _TERMINAL);
		SendRobotWeightings(0, true);
		switch (theApp.m_LastSettings.nCoordMask) {
		case 1:
			SendStr(L"&1", _TERMINAL);
			SendStr(L"#1->I#2->I#3->I#4->I#5->I", _TERMINAL);
			SendStr(L"&1 define lookahead 500", _TERMINAL);
			SendStr(L"Coord[1].LHDistance=50", _TERMINAL);
			SendStr(L"Coord[1].SegLinToPvt=1", _TERMINAL);
			SendStr(L"Coord[1].TA=20", _TERMINAL);
			SendStr(L"Coord[1].Td=20", _TERMINAL);
			SendStr(L"Coord[1].TS=20", _TERMINAL);
			SendStr(L"Coord[1].TM=2", _TERMINAL);
			SendStr(L"Coord[1].FeedTime=500", _TERMINAL);
			SendStr(L"B8R", _TERMINAL);
			break;
		case 2:
		case 3:
			break;
		}
		break;
	case RAILWAY_AXLE:
		break;
	case TWIN_TOWER_KINEMATIC:
		break;
	}

}

void CFieldBusController::UpdateTurntableCartesianScanningAxes(CCoord Cp)
{
	int nX0,nY0,nZ0,nXt0,nYt0;
	int	 nArray[16];
	CString Buff;
	int nFirstCoordinate;

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		nX0=(int)(Cp.Side0.fX/theApp.m_Axes[theApp.m_Tank.nXLeft].fStepSize);
		nY0=(int)(Cp.Side0.fY/theApp.m_Axes[theApp.m_Tank.nYLeft].fStepSize);
		nZ0=(int)(Cp.Side0.fZ/theApp.m_Axes[theApp.m_Tank.nZLeft].fStepSize);
		nXt0=(int)(Cp.Side0.fXt/theApp.m_Axes[theApp.m_Tank.nXtLeft].fStepSize);
		nYt0=(int)(Cp.Side0.fYt/theApp.m_Axes[theApp.m_Tank.nYtLeft].fStepSize);
		nArray[0]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nXLeft].nPhysicalAxis - 1 ].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[0]);
		nArray[1]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nYLeft].nPhysicalAxis - 1 ].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[1]);
		nArray[2]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nZLeft].nPhysicalAxis - 1 ].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[2]);
		nArray[3]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nXtLeft].nPhysicalAxis - 1 ].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[3]);
		nArray[4]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nYtLeft].nPhysicalAxis - 1 ].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtLeft].nOffset,theApp.m_PmacUser.m_nMachinePosition[4]);
		nX0=(int)(Cp.Side1.fX/theApp.m_Axes[theApp.m_Tank.nXRight].fStepSize);
		nY0=(int)(Cp.Side1.fY/theApp.m_Axes[theApp.m_Tank.nYRight].fStepSize);
		nZ0=(int)(Cp.Side1.fZ/theApp.m_Axes[theApp.m_Tank.nZRight].fStepSize);
		nXt0=(int)(Cp.Side1.fXt/theApp.m_Axes[theApp.m_Tank.nXtRight].fStepSize);
		nYt0=(int)(Cp.Side1.fYt/theApp.m_Axes[theApp.m_Tank.nYtRight].fStepSize);
		nArray[5]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nXRight].nPhysicalAxis - 1 ].ActualToMachine(nX0+theApp.m_Axes[theApp.m_Tank.nXRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[5]);
		nArray[6]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nYRight].nPhysicalAxis - 1 ].ActualToMachine(nY0+theApp.m_Axes[theApp.m_Tank.nYRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[6]);
		nArray[7]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nZRight].nPhysicalAxis - 1 ].ActualToMachine(nZ0+theApp.m_Axes[theApp.m_Tank.nZRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[7]);
		nArray[8]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nXtRight].nPhysicalAxis - 1 ].ActualToMachine(nXt0+theApp.m_Axes[theApp.m_Tank.nXtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[8]);
		nArray[9]=theApp.m_Kinematics.m_LeadScrew[ theApp.m_Axes[theApp.m_Tank.nYtRight].nPhysicalAxis - 1 ].ActualToMachine(nYt0+theApp.m_Axes[theApp.m_Tank.nYtRight].nOffset,theApp.m_PmacUser.m_nMachinePosition[9]);

		Buff.Format(L"M%d=%d",100,nArray[0]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"M%d=%d",101,nArray[1]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"M%d=%d",102,nArray[2]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"M%d=%d",103,nArray[3]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"M%d=%d",104,nArray[4]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"M%d=%d",105,nArray[5]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"M%d=%d",106,nArray[6]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"M%d=%d",107,nArray[7]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"M%d=%d",108,nArray[8]);		SendStr(Buff, _TERMINAL);
		Buff.Format(L"M%d=%d",109,nArray[9]);		SendStr(Buff, _TERMINAL);
		break;
	case SPX_ROBOT:
		Buff.Format(L"FirstCoordinate=%d", nFirstCoordinate = 100);
		SendStr(Buff, _TERMINAL);
		theApp.InvertNormScannerSpecific(&Cp, PORTSIDE);
		Cp.TransformCoordinate(PORTSIDE,&theApp.m_Robot[0].m_vBase,theApp.m_Robot[0].m_fBaseRotX,0.0f,theApp.m_Robot[0].m_fBaseRotZ);
		Cp.TransformCoordinate(STARBOARD,&theApp.m_Robot[1].m_vBase,theApp.m_Robot[1].m_fBaseRotX,0.0f,theApp.m_Robot[1].m_fBaseRotZ);
		Cp.FormatQuaternion(&Buff,1,100);
		SendStr(Buff, _TERMINAL);
		break;
	case RAILWAY_AXLE:
	case TWIN_TOWER_KINEMATIC:
		break;
	case TANK_2AXIS:
	case TANK_5AXIS:
	case TANK_6AXIS:
		Buff.Format(L"FirstCoordinate=%d", nFirstCoordinate = 0);
		SendStr(Buff, _TERMINAL);
		theApp.InvertNormScannerSpecific(&Cp, PORTSIDE);
		theApp.InvertNormScannerSpecific(&Cp, STARBOARD);
		Cp.TransformCoordinate(PORTSIDE, &theApp.m_Tank.vFrameZero[0], 0.0f, 0.0f, 0.0f);
		Cp.FormatNormal(&Buff, 1, nFirstCoordinate);
		SendStr(Buff, _TERMINAL);
		break;
	}
}

void CFieldBusController::StartContinuousRotate(int nAxis,float fRotateSpeed, int nCW_CCW)
{
	CString Buff;
	float fSpeed;

	fSpeed = fRotateSpeed / 1000.0f / theApp.m_Axes[nAxis].fStepSize;

	Buff.Format(_T("Motor[%d].JogSpeed=%.02f"),theApp.m_Axes[nAxis].nPhysicalAxis,fSpeed);
	SendStr(Buff, _TERMINAL);

	if(nCW_CCW == 0) {
		Buff.Format(L"#%dj+",theApp.m_Axes[nAxis].nPhysicalAxis);
	} else {
		Buff.Format(L"#%dj-",theApp.m_Axes[nAxis].nPhysicalAxis);
	}
	SendStr(Buff, _TERMINAL);

}

void CFieldBusController::StopContinuousRotate(int nAxis, bool bControlledStop)
{
	CString Buff;

	switch (theApp.m_Tank.nScannerDescription) {
	default:
		Buff.Format(_T("#%dj/"),theApp.m_Axes[nAxis].nPhysicalAxis);
		break;
	case DUAL_ROBOT:
	case DUAL_ROBOT_9_PLUS_9:
		break;
	case SPX_ROBOT:
		if(bControlledStop == true) {
			Buff.Format(_T("#%dj/"),theApp.m_Axes[nAxis].nPhysicalAxis);
		} else {
			Buff.Format(_T("#%dk"),theApp.m_Axes[nAxis].nPhysicalAxis);
		}
		break;
	case RAILWAY_AXLE:
		break;
	case TANK_5AXIS:
	case TANK_2AXIS:
	case TWIN_TOWER_KINEMATIC:
		break;
	}

	SendStr(Buff, _TERMINAL);

}

bool CFieldBusController::IsAxesInPos(int nAxisMask)
{
	int nMask = 1;
	int ii;

	for(ii=0;ii<20;ii++, nMask<<=1) {
		if(nMask & nAxisMask) {
			if((theApp.m_Axes[ii].nStatus & PMAC_IN_POS) == 0) return false;
		}
	}
	return true;
}

bool CFieldBusController::IsAxisInMachinePosition(int nAxis, int nPos)
{
	if((theApp.m_Axes[nAxis].nStatus & PMAC_IN_POS) == 0) return false;
	if(abs((theApp.m_Axes[nAxis].nPos+theApp.m_Axes[nAxis].nOffset) - nPos) > 10) return false;

	return true;
}

bool CFieldBusController::IsJogMoveFinished(int nSide, int nWhat)
{
	int	ii;

	if(nWhat & _JOG_ROBOT) {
		if(nSide & 1) {
			for(ii=3;ii<9;ii++) {
				if((theApp.m_Axes[ii].nStatus & PMAC_IN_POS) == 0) return false;
			}
		}
		if(nSide & 2) {
			for(ii=13;ii<19;ii++) {
				if((theApp.m_Axes[ii].nStatus & PMAC_IN_POS) == 0) return false;
			}
		}
	}

	if(nWhat & _JOG_TOWER) {
		if(nSide & 1) {
			if((theApp.m_Axes[1].nStatus & PMAC_IN_POS) == 0) return false;
			if((theApp.m_Axes[2].nStatus & PMAC_IN_POS) == 0) return false;
		}
		if(nSide & 2) {
			if((theApp.m_Axes[12].nStatus & PMAC_IN_POS) == 0) return false;
		}
	}

	return true;
}


bool CFieldBusController::IsRAxisInPosition(float fPos)
{
	CCoord CpSurface;

	theApp.m_Motors.GetSurfacePos(&CpSurface);
	if(abs(CpSurface.Side0.fR - fPos) < 0.01f) {
		return true;
	}
	return false;
}




bool CFieldBusController::IsRotateRunning()
{
	bool bFlag = false;

	return theApp.m_Ethercat.IsRotateRunning();
}

void CFieldBusController::RelativeMove(AxisData *pAxis, int nJogDistance)
{
	CString Buff;

	Buff.Format(L"Motor[%d].JogSpeed=%.02f\r\n",pAxis->nPhysicalAxis, pAxis->fMaxMoveSpeed * 0.9f);
	SendStr(Buff, _TERMINAL);

	Buff.Format(_T("#%dj/"),pAxis->nPhysicalAxis);
	SendStr(Buff, _TERMINAL);
	Sleep(100);
	Buff.Format(_T("#%dj:%d"),pAxis->nPhysicalAxis,nJogDistance);
	SendStr(Buff, _TERMINAL);

}

void CFieldBusController::AbsoluteMove(AxisData* pAxis,int nJogposition)
{
	CString Buff;

	Buff.Format(L"Motor[%d].JogSpeed=%.02f",pAxis->nPhysicalAxis, pAxis->fMaxMoveSpeed);
	SendStr(Buff, _TERMINAL);

	Buff.Format(_T("#%dj/"),pAxis->nPhysicalAxis);
	SendStr(Buff, _TERMINAL);
	Sleep(100);
	Buff.Format(_T("#%dj=%d"),pAxis->nPhysicalAxis,nJogposition);
	SendStr(Buff, _TERMINAL);

}


void CFieldBusController::JogJustR(float fAngle, float fRotateSpeed)
{
	CString Buff;
	float fSpeed;

	fSpeed = fRotateSpeed / 1000.0f / theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize;

	Buff.Format(_T("Motor[%d].JogSpeed=%.02f"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis,fSpeed);
	SendStr(Buff, _TERMINAL);

	int nPos = (int)(fAngle / theApp.m_Axes[theApp.m_Tank.nRLeft].fStepSize);
	nPos += (int)((double)theApp.m_Axes[theApp.m_Tank.nRLeft].nRevCounter * theApp.m_Axes[theApp.m_Tank.nRLeft].dStepsPerRev);
	Buff.Format(_T("#%dj=%d"),theApp.m_Axes[theApp.m_Tank.nRLeft].nPhysicalAxis,nPos);
	SendStr(Buff, _TERMINAL);
}


void CFieldBusController::DisasterRecovery()
{

	KillMotors();
	Sleep(1000);
	EnableMotors();
	Sleep(3000);
	SendStr(L"Enable plc 3", _TERMINAL);
}

void CFieldBusController::SendAxesIncluded(int nWhichSet, bool bForce) 
{
	static int nOldAxesIncluded = -1;

	return;

	switch(nWhichSet) {
	case MOVETO_INCLUDE:
		if(nOldAxesIncluded != theApp.m_Kinematics.m_nAxesIncludedJoystick || bForce == true) {
			if(theApp.m_Kinematics.m_nAxesIncludedMoves & 1) {
				SendStr(L"IncludeXAxis=1", _TERMINAL);
			} else {
				SendStr(L"IncludeXAxis=0", _TERMINAL);
			}
			if(theApp.m_Kinematics.m_nAxesIncludedMoves & 2) {
				SendStr(L"IncludeYAxis=1", _TERMINAL);
			} else {
				SendStr(L"IncludeYAxis=0", _TERMINAL);
			}
			if(theApp.m_Kinematics.m_nAxesIncludedMoves & 4) {
				SendStr(L"IncludeZAxis=1", _TERMINAL);
			} else {
				SendStr(L"IncludeZAxis=0", _TERMINAL);
			}
		}
		nOldAxesIncluded = theApp.m_Kinematics.m_nAxesIncludedMoves;
		break;
	case JOYSTICK_INCLUDE:
		if(nOldAxesIncluded != theApp.m_Kinematics.m_nAxesIncludedJoystick || bForce == true) {
			if(theApp.m_Kinematics.m_nAxesIncludedJoystick & 1) {
				SendStr(L"IncludeXAxis=1", _TERMINAL);
			} else {
				SendStr(L"IncludeXAxis=0", _TERMINAL);
			}
			if(theApp.m_Kinematics.m_nAxesIncludedJoystick & 2) {
				SendStr(L"IncludeYAxis=1", _TERMINAL);
			} else {
				SendStr(L"IncludeYAxis=0", _TERMINAL);
			}
			if(theApp.m_Kinematics.m_nAxesIncludedJoystick & 4) {
				SendStr(L"IncludeZAxis=1", _TERMINAL);
			} else {
				SendStr(L"IncludeZAxis=0", _TERMINAL);
			}
		}
		nOldAxesIncluded = theApp.m_Kinematics.m_nAxesIncludedJoystick;
		break;
	}

}


void CFieldBusController::SendRobotWeightings(int nMode, bool bForce)
{
	CString Buff;
	static float fOldRobotWeightingX[2] = { -1.0, -1.0 };
	static float fOldRobotWeightingY[2] = { -1.0, -1.0 };
	static float fOldRobotWeightingZ[2] = { -1.0, -1.0 };
	float fWeightingX[2], fWeightingY[2], fWeightingZ[2];
	int nSide;

	switch(nMode) {
	case 0:		//Weighting
		for(nSide=0;nSide<2;nSide++) {
			fWeightingX[nSide] = theApp.m_Kinematics.m_fRobotWeightingX[nSide];
			fWeightingY[nSide] = theApp.m_Kinematics.m_fRobotWeightingY[nSide];
			fWeightingZ[nSide] = theApp.m_Kinematics.m_fRobotWeightingZ[nSide];
		}
		break;
	case 1: //All Robot
		for(nSide=0;nSide<2;nSide++) {
			fWeightingX[nSide] = 100.0f;
			fWeightingY[nSide] = 100.0f;
			fWeightingZ[nSide] = 100.0f;
		}
		break;
	case 2: //All cartesian
		for(nSide=0;nSide<2;nSide++) {
			fWeightingX[nSide] = 0.0f;
			fWeightingY[nSide] = 0.0f;
			fWeightingZ[nSide] = 0.0f;
		}
		break;
	}

	if(fOldRobotWeightingX[0] - fWeightingX[0] || bForce == true) {
		Buff.Format(L"RobotWeightingX=%.03f", fWeightingX[0] / 100.0f);
		SendStr(Buff,_TERMINAL);
	}
	if(fOldRobotWeightingY[0] - fWeightingY[0] || bForce == true) {
		Buff.Format(L"RobotWeightingY=%.03f", fWeightingY[0] / 100.0f);
		SendStr(Buff,_TERMINAL);
	}
	if(fOldRobotWeightingZ[0] - fWeightingZ[0] || bForce == true) {
		Buff.Format(L"RobotWeightingZ=%.03f", fWeightingZ[0] / 100.0f);
		SendStr(Buff,_TERMINAL);
	}
	if(fOldRobotWeightingX[1] - fWeightingX[1] || bForce == true) {
		Buff.Format(L"B_RobotWeightingX=%.03f", fWeightingX[1] / 100.0f);
		SendStr(Buff,_TERMINAL);
	}
	if(fOldRobotWeightingY[1] - fWeightingY[1] || bForce == true) {
		Buff.Format(L"B_RobotWeightingY=%.03f", fWeightingY[1] / 100.0f);
		SendStr(Buff,_TERMINAL);
	}
	if(fOldRobotWeightingZ[1] - fWeightingZ[1] || bForce == true) {
		Buff.Format(L"B_RobotWeightingZ=%.03f", fWeightingZ[1] / 100.0f);
		SendStr(Buff,_TERMINAL);
	}

	for(nSide=0;nSide<2;nSide++) {
		fOldRobotWeightingX[nSide] = fWeightingX[nSide];
		fOldRobotWeightingY[nSide] = fWeightingY[nSide];
		fOldRobotWeightingZ[nSide] = fWeightingZ[nSide];
	}


}


int CFieldBusController::RequestTemperature(int nWhich)
{
	CString Buff;

	Buff.Format(L"Temperature%d",nWhich);
	SendStr(Buff, _BACKGROUND);

	return m_nTemperature[nWhich];
}

int CFieldBusController::RequestOiler(int nWhich)
{
	CString Buff;

	Buff.Format(L"Oiler%d",nWhich);
	SendStr(Buff, _BACKGROUND);

	return m_nOilerStatus[nWhich];
}

int CFieldBusController::RequestToolID(int nWhich)
{
	CString Buff;

	Buff.Format(L"ToolID%d",nWhich);
	SendStr(Buff, _BACKGROUND);

	return m_nToolID[nWhich];
}

int CFieldBusController::RequestToolHolderStatus(int nWhich)
{
	CString Buff;

	Buff.Format(L"ToolHolderStatus%d",nWhich);
	SendStr(Buff, _BACKGROUND);

	return m_nToolHolderStatus[nWhich];
}


void CFieldBusController::LockTool(int nSide)
{
	switch(theApp.m_Tank.nScannerDescription) {
	default:
		break;
	case FB_TWIN_TOWER:
		switch(nSide) {
		case 0:	SendStr(L"ToolClampDemand0=1", _TERMINAL);
			break;
		case 1:	SendStr(L"ToolClampDemand1=1", _TERMINAL);
			break;
		};
		break;
	}
}

void CFieldBusController::UnlockTool(int nSide)
{

	switch(theApp.m_Tank.nScannerDescription) {
	default:
		break;
	case FB_TWIN_TOWER:
		switch(nSide) {
		case 0:	SendStr(L"ToolClampDemand0=2", _TERMINAL);
			break;
		case 1:	SendStr(L"ToolClampDemand1=2", _TERMINAL);
			break;
		};
		break;
	}
}

bool CFieldBusController::IsToolLocked(int nSide)
{
	if((m_nToolHolderStatus[nSide] & 1) == 1) return true;

	return false;
}

bool CFieldBusController::IsDoorClosed(int nSide)
{
	if((m_nDoorStatus[nSide] & 1) == 0) return true;

	return false;
}

void CFieldBusController::AndonEvent(int nCode)
{
	CString Buff;

	Buff.Format(L"AndonEventCode=%d",nCode);
//	SendStr(Buff, _TERMINAL);
}

bool CFieldBusController::WaitForAmplifiersEnabled(int nMotorMask,int nMaxWaitTime_ms)
{
	bool bFlag = true;
	CHRTimer	HRTimer;

	HRTimer.StartTimer();

	do {
		int nMask = 1;
		for(int nMotor=0;nMotor<32;nMotor++, nMask<<=1) {
			if(nMask & nMotorMask) {
				if((theApp.m_Ethercat.getMotorStatus(nMotor) & 0x01) == 0) bFlag = false;
			}
		}
	}
	while((bFlag == false) && (HRTimer.GetTime()) < (double)nMaxWaitTime_ms);
	double dElapsedTime = (HRTimer.GetTime());


	HRTimer.StopTimer();
	return bFlag;
}

void CFieldBusController::OriginFingerManipulator()
{

	SendStr(L"Enable PLC 16",_TERMINAL);
}


void CFieldBusController::EnableCrashDetector(bool bFlag, int nSide)
{
	switch(nSide) {
	case 0:
		if(bFlag == false) {
			SendStr(L"CrashDetectEnable0 = 0",_TERMINAL);
		} else {
			SendStr(L"CrashDetectEnable0 = 1",_TERMINAL);
		}
		break;
	case 1:
		if(bFlag == false) {
			SendStr(L"CrashDetectEnable1 = 0",_TERMINAL);
		} else {
			SendStr(L"CrashDetectEnable1 = 1",_TERMINAL);
		}
		break;
	}
}

int CFieldBusController::RequestEthercatGeneralInput()
{
	return m_nGeneralEthercatInput;
}


void CFieldBusController::DownloadAxisParameters(int nAxis) 
{
	CString Buff;

	Buff.Format(L"Motor[%d].InvAmax=%f", theApp.m_Axes[nAxis].nPhysicalAxis, 1.0f / theApp.m_Axes[nAxis].fAcceleration);
	SendStr(Buff, _TERMINAL);

	Buff.Format(L"Motor[%d].InvDmax=%f", theApp.m_Axes[nAxis].nPhysicalAxis, 1.0f / theApp.m_Axes[nAxis].fAcceleration);
	SendStr(Buff, _TERMINAL);

	Buff.Format(L"Motor[%d].JogTa=%f", theApp.m_Axes[nAxis].nPhysicalAxis, -1.0f / theApp.m_Axes[nAxis].fJoystickAcceleration);
	SendStr(Buff, _TERMINAL);

	Buff.Format(L"Motor[%d].JogTs=%f", theApp.m_Axes[nAxis].nPhysicalAxis, -5.0f / theApp.m_Axes[nAxis].fJoystickAcceleration);
	SendStr(Buff, _TERMINAL);

	Buff.Format(L"Motor[%d].JogSpeed=%f", theApp.m_Axes[nAxis].nPhysicalAxis, theApp.m_Axes[nAxis].fMaxMoveSpeed);
	SendStr(Buff, _TERMINAL);

	Buff.Format(L"Motor[%d].MaxSpeed=%f", theApp.m_Axes[nAxis].nPhysicalAxis, theApp.m_Axes[nAxis].fMaxMoveSpeed);
	SendStr(Buff, _TERMINAL);

	Buff.Format(L"Motor[%d].InPosBand=%d", theApp.m_Axes[nAxis].nPhysicalAxis, theApp.m_Axes[nAxis].nPositionError);
	SendStr(Buff, _TERMINAL);

	Buff.Format(L"Mtr%dSF=%f", theApp.m_Axes[nAxis].nPhysicalAxis, 1.0f/theApp.m_Axes[nAxis].fStepSize);
	SendStr(Buff, _TERMINAL);

	Buff.Format(L"Motor[%d].FatalFeLimit=%d", theApp.m_Axes[nAxis].nPhysicalAxis, (int)(theApp.m_Axes[nAxis].fFatalFollowingError / theApp.m_Axes[nAxis].fStepSize));
	SendStr(Buff, _TERMINAL);

	Buff.Format(L"Motor[%d].MinPos=%d", theApp.m_Axes[nAxis].nPhysicalAxis, (int)(theApp.m_Axes[nAxis].fMinTravel / theApp.m_Axes[nAxis].fStepSize));
	SendStr(Buff, _TERMINAL);

	Buff.Format(L"Motor[%d].MaxPos=%d", theApp.m_Axes[nAxis].nPhysicalAxis, (int)(theApp.m_Axes[nAxis].fMaxTravel / theApp.m_Axes[nAxis].fStepSize));
	SendStr(Buff, _TERMINAL);

	Buff.Format(L"KinematicMin(%d)=%.02f", theApp.m_Axes[nAxis].nPhysicalAxis, theApp.m_Axes[nAxis].fMinKin);
	SendStr(Buff, _TERMINAL);

	Buff.Format(L"KinematicMax(%d)=%.02f", theApp.m_Axes[nAxis].nPhysicalAxis, theApp.m_Axes[nAxis].fMaxKin);
	SendStr(Buff, _TERMINAL);

	SetHomePos(nAxis);
}

void CFieldBusController::DownloadAllAxisParameters() {
	for (int iAxis = 0; iAxis < m_nMaxNumberAxes; iAxis++) {
		DownloadAxisParameters(iAxis);
	}
}

void CFieldBusController::DownloadPPMACKinematics(int nSide) 
{
	CString Buff;

	switch (nSide) {
	case 0:
		Buff.Format(L"Length01X=%f", theApp.m_Kinematics.m_fXOffset[nSide]);	SendStr(Buff, _TERMINAL);
		Buff.Format(L"Length01Y=%f", theApp.m_Kinematics.m_fYOffset[nSide]);	SendStr(Buff, _TERMINAL);
		Buff.Format(L"Length01Z=%f", theApp.m_Kinematics.m_fZOffset[nSide]);	SendStr(Buff, _TERMINAL);
		break;
	case 1:
		Buff.Format(L"B_Length01X=%f", theApp.m_Kinematics.m_fXOffset[nSide]);	SendStr(Buff, _TERMINAL);
		Buff.Format(L"B_Length01Y=%f", theApp.m_Kinematics.m_fYOffset[nSide]);	SendStr(Buff, _TERMINAL);
		Buff.Format(L"B_Length01Z=%f", theApp.m_Kinematics.m_fZOffset[nSide]);	SendStr(Buff, _TERMINAL);
		break;
	}
}

bool CFieldBusController::WaitAxesEnabled(int nCoordMask)
{
	bool bFlag = false;
	int nLoop;

	for (nLoop = 0; nLoop < 500; nLoop++) {
		if (IsMotorsEnabled(nCoordMask) == true) {
			return true;
		}
		Sleep(2);
	}

	return false;
}

bool CFieldBusController::IsMotorsEnabled(int nCoordMask)
{
	int nMask = 1;

	theApp.m_Ethercat.InvalidateRobotJoints();

	for (int ii = 0; ii < 32; ii++, nMask<<=1) {
		if ((nCoordMask & 1) && (theApp.m_Tank.nAxisEnableMask[1] & nMask)) {
			if (!(theApp.m_Ethercat.getAxisStatus(ii) & PMAC_AMPLIFIER_ENABLED)) return false;
		}
		if ((nCoordMask & 2) && (theApp.m_Tank.nAxisEnableMask[2] & nMask)) {
			if (!(theApp.m_Ethercat.getAxisStatus(ii) & PMAC_AMPLIFIER_ENABLED)) return false;
		}
	}
	return true;
}
