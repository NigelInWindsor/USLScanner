//WorkerThread.cpp

#include "stdafx.h"
#include "uslscanner.h"
#include "CommandDone.h"
#include "Globals.h"
#include "WorkerThread.h"

UINT StartBkgndMeasurements(LPVOID pParam);

/*************************************************************************************************
* Function: CreateThread()
* Comments: 
*************************************************************************************************/
void CWorkerThread::CreateThread()
{	
	m_bCommandEventSet = FALSE;
	m_bConnected = FALSE;
	m_bCollectingBackGround = FALSE;
	m_dLevelRX = 0.0;
	m_dLevelRY = 0.0;
	m_dLevelRZ = 0.0;

	m_pMyMainWnd = AfxGetMainWnd();

	//After executing the command specified this event will wait untill it is set 
	//Once it is set the thread will resume and executes the next specified command
	//This event is a MANUAL event
	m_hSuspendResumeEvent = CreateEvent(NULL, TRUE, FALSE, L"SuspendResumeEvent");

	//This event is used only in cases where the UI thread has to wait for the worker thread
	//to complete its operation.
	//This event is a MANUAL event
	m_hCommandDoneEvent = CreateEvent(NULL, TRUE, FALSE, L"CommandCompleteEvent");
	
	//Creating the Thread which starts immediately.
	m_pWorkerThread = AfxBeginThread(StartThread, this,
									THREAD_PRIORITY_NORMAL, 0 ,CREATE_SUSPENDED);
	VERIFY(m_pWorkerThread);	
	return;
}

bool CWorkerThread::IsConnected()
{
	return m_bConnected;
}

bool CWorkerThread::IsCollectingData()
{
	return m_bCollectingBackGround;
}

/*************************************************************************************************
* Function: StartThread( LPVOID pParam )
* Comments: 
*************************************************************************************************/
UINT CWorkerThread:: StartThread( LPVOID pParam )
{	
	CWorkerThread* pWorkerThread = (CWorkerThread*)pParam;	
	pWorkerThread->RunThread();
	return 0;
}

/*************************************************************************************************
* Function: RunThread()
* Comments: 
*************************************************************************************************/
void CWorkerThread::RunThread()
{
	
	BOOL bBlock = TRUE;
	LevelData* pLD = NULL;
	theApp.m_pFaroTracker->setBlocking(true);

	while(tCommand != EXIT)
	{		
		switch(tCommand)
		{			
			case CONNECT:
			{
				PostMsgToUpdateStatus(L"Connecting...");
				try
				{
					theApp.m_pFaroTracker->connect((FAROLPTSTR)m_pIpAdd, (FAROLPTSTR)L"user", (FAROLPTSTR)L"");
				}
				catch( TrackerException* e )
				{
					WCHAR* msg = L"Tracker connection failed.";
					LPARAM lParam = (LPARAM)msg;
					//if Tracker connection failes delete the memory of Tracker object					
					m_pMyMainWnd->PostMessage(UWM_CONNECT_FAILED, lParam, 0);
					PostMsgToUpdateStatus(msg);
					delete e;
				}
				try {
					if (m_bConnected = theApp.m_pFaroTracker->connected()) {
						if (theApp.m_pFaroTracker->initialized()) {
							m_nInitializationStatus = 0x3f;
							PostMsgToUpdateStatus(L"Tracker initialized");
						}
						else {
							m_nInitializationStatus = 0;
							PostMsgToUpdateStatus(L"Tracker not initialized");
						}
					}
				}
				catch (TrackerException* e)
				{
					WCHAR* msg = (WCHAR*)e->getText();
					LPARAM lParam = (LPARAM)msg;
					//if Tracker connection failes delete the memory of Tracker object					
					m_pMyMainWnd->PostMessage(UWM_CONNECT_FAILED, lParam, 0);
					PostMsgToUpdateStatus(msg);
					delete e;
				}
			}
			break;
			case DISCONNECT:
			{
				try
				{
					theApp.m_pFaroTracker->disconnect();
				}
				catch( TrackerException* e )
				{
					WCHAR* msg = (WCHAR*)e->getText();
					LPARAM lParam = (LPARAM)msg;
					m_pMyMainWnd->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
					delete e;
				}
				m_bConnected = theApp.m_pFaroTracker->connected();
			}
			break;
			case CMD_SEQUENCE:
			{		
				try
				{
					PostMsgToUpdateStatus(L"Checking if Tracker is initialized");
					bool state = theApp.m_pFaroTracker->initialized();
					m_nInitializationStatus = 0;
					if(!state)
					{
						PostMsgToUpdateStatus(L"Tracker not initialized, Initializing");			
						theApp.m_pFaroTracker->initialize();
					}
					m_nInitializationStatus |= 1;

					PostMsgToUpdateStatus(L"Checking motor state");
					state = theApp.m_pFaroTracker->motorsOn();
					if(!state)
					{
						PostMsgToUpdateStatus(L"Motors turned off, turning on");
						theApp.m_pFaroTracker->changeMotorState(true);
						PostMsgToUpdateStatus(L"Turned on motors");
					}
					m_nInitializationStatus |= 2;

					PostMsgToUpdateStatus(L"Checking tracking state");
					state = theApp.m_pFaroTracker->trackingOn();
					if(!state)
					{
						PostMsgToUpdateStatus(L"Tracking turned off, turning on");
						theApp.m_pFaroTracker->changeTrackingState(true);
						PostMsgToUpdateStatus(L"Turned on tracking");
					}
					m_nInitializationStatus |= 4;

					PostMsgToUpdateStatus(L"Checking if Distance is valid");
					state = theApp.m_pFaroTracker->targetLocationValid();
					if(!state)
					{
						PostMsgToUpdateStatus(L"Distance Invalid");
					}
					m_nInitializationStatus |= 8;

					PostMsgToUpdateStatus(L"Checking if Target present");
					state = theApp.m_pFaroTracker->targetPresent();
					if(!state)
					{
						PostMsgToUpdateStatus(L"Target not present");
					}
					m_nInitializationStatus |= 0x10;

					PostMsgToUpdateStatus(L"Homing Tracker");
					theApp.m_pFaroTracker->home(true);
					PostMsgToUpdateStatus(L"Tracker initialized");
					m_nInitializationStatus |= 0x20;
				}
				catch( TrackerException* e )
				{
					WCHAR* msg = (WCHAR*)e->getText();
					LPARAM lParam = (LPARAM)msg;
					m_pMyMainWnd->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
					delete e;
				}
			}
			break;
			case SEARCH:
			{
				try
				{
					theApp.m_pFaroTracker->search(3);
				}
				catch( TrackerException* e )
				{
					WCHAR* msg = (WCHAR*)e->getText();
					LPARAM lParam = (LPARAM)msg;
					m_pMyMainWnd->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
					delete e;
				}
			}
			break;
			case LEVEL_SEQUENCE:
			{		
				try
				{
					PostMsgToUpdateStatus(L"Running level routine");
					pLD = theApp.m_pFaroTracker->measureLevel();
				}
				catch( TrackerException* e )
				{
					WCHAR* msg = (WCHAR*)e->getText();
					LPARAM lParam = (LPARAM)msg;
					m_pMyMainWnd->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
					delete e;
					PostMsgToUpdateStatus(L"Level failed");
				}
				PostMsgToUpdateStatus(L"Tracker levelled");
				if (pLD) {
					theApp.m_FaroWorkerThread.m_dLevelRX = pLD->getRX();
					theApp.m_FaroWorkerThread.m_dLevelRY = pLD->getRY();
					theApp.m_FaroWorkerThread.m_dLevelRZ = pLD->getRZ();
				}
				theApp.m_pFaroTracker->home(false);
			}
			break;
			case AVAILABLE_APPS:
			{
				SMXObjectArray* pAppsArray = NULL;
				try
				{
					pAppsArray = theApp.m_pFaroTracker->availableApplications();
					if(pAppsArray)
					{
						int numApps = pAppsArray->getNumElements();
						for(int i = 0; i < numApps; i++)
						{					
							WCHAR* name = (WCHAR*)pAppsArray->getElement(i);		
							WCHAR* temp = new WCHAR[wcslen(name) + 1];
							wcscpy_s(temp, wcslen(name) + 1,name);
							m_Appslist.AddTail(temp);
						}
					}
				}
				catch(TrackerException* e)
				{
					WCHAR* msg = (WCHAR*)e->getText();
					LPARAM lParam = (LPARAM)msg;
					m_pMyMainWnd->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
					delete e;
				}
				delete pAppsArray;				
			}
			break;
			case RUN_APP:
			{
				try
				{
				theApp.m_pFaroTracker->startApplicationFrame((FAROLPTSTR)m_AppName, (FAROLPTSTR)L"");			
				}
				catch(TrackerException* e)
				{
					WCHAR* msg = L"Unable to start tracker application.";
					LPARAM lParam = (LPARAM)msg;
					m_pMyMainWnd->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
					delete e;
				}
			}
			break;
			case STOP_APPS:
			{
				SMXObjectArray* pAppsArray = NULL;
				try
				{
					pAppsArray = theApp.m_pFaroTracker->startedApplications();

					if( pAppsArray)
					{
						int numApps = pAppsArray->getNumElements();
						for(int i = 0; i < numApps; i++)
						{				
							WCHAR* pAppRunning = (WCHAR*)pAppsArray->getElement(i);
							theApp.m_pFaroTracker->stopApplicationFrame((FAROLPTSTR)pAppRunning);					
						}
					}		
				}
				catch(TrackerException *e)
				{
					WCHAR* msg = (WCHAR*)e->getText();
					LPARAM lParam = (LPARAM)msg;
					m_pMyMainWnd->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
					delete e;
				}
				delete pAppsArray;
			}
			break;
			case FOREGND_MEASURE:
			{
				SMXObjectArray* pDataArray = NULL;
				MeasurePointData* pData = NULL;

				try
				{
					NullStartTrigger		startTrigger;
					IntervalTrigger			continueTrigger(0.1);
					AverageFilter			filter;
					MeasureCfg	cfg(2000, &filter,&startTrigger, &continueTrigger);				

					theApp.m_pFaroTracker->setMeasureEventRate(nEventRate);

					theApp.m_pFaroTracker->startMeasurePoint(&cfg);
					if(nEventRate == 1 && !bMulti)
					{			
						pData = theApp.m_pFaroTracker->readMeasurePointData();
						
						ASSERT(pData);

						LPARAM lParam = (LPARAM)pData;
						AfxGetMainWnd()->PostMessage(UWM_FOREGND_MEASUREMENT_AVAILABLE,lParam, 0);	
					}
					else if(nEventRate == 10 && !bMulti)
					{
						pDataArray = NULL;
						pDataArray = theApp.m_pFaroTracker->readMeasurePointData(nEventRate);
						ASSERT(pDataArray);
						int numElements = pDataArray->getNumElements();
						ASSERT(numElements == nEventRate);
						for(int i = 0; i < numElements; i++)
						{
							pData = (MeasurePointData*)pDataArray->getElement(i);
							ASSERT(pData);
							LPARAM lParam = (LPARAM)pData;				
							AfxGetMainWnd()->PostMessage(UWM_FOREGND_MEASUREMENT_AVAILABLE,lParam, 0);
						}
					}
					else if(nEventRate == 1 && bMulti)
					{
						while(!bStopBkgnd)
						{
							pData = theApp.m_pFaroTracker->readMeasurePointData();
						
						ASSERT(pData);

						LPARAM lParam = (LPARAM)pData;
						AfxGetMainWnd()->PostMessage(UWM_FOREGND_MEASUREMENT_AVAILABLE,lParam, 0);						
						}
					}					
				}
				catch( TrackerException* e )
				{
					WCHAR* msg = (WCHAR*)e->getText();
					LPARAM lParam = (LPARAM)msg;
					m_pMyMainWnd->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
					delete e;
				}
				catch( SMXException* e )
				{	
					WCHAR msg[64];
					e->getMsg(msg, 64);
					LPARAM lParam = (LPARAM)lParam;
					m_pMyMainWnd->PostMessage(UWM_ON_SMXEXCEPTION, lParam, 0);
					delete e;
				}
				try
				{
					theApp.m_pFaroTracker->stopMeasurePoint();
				}
				catch( TrackerException* e )
				{
					WCHAR* msg = (WCHAR*)e->getText();
					LPARAM lParam = (LPARAM)msg;
					m_pMyMainWnd->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
					delete e;
				}
			}
			break;			
			default:
				break;
		}
		try
		{
			if(bBlock)
			{
				//If the setblocking is set to False while the Tracker is busy like say
				//if the healthchecks is running then it would throw a busy exception.
				//To avoid this it can be checked whether the Tracker is busy or not.
				theApp.m_pFaroTracker->setBlocking(false);
				bBlock = FALSE;
			}
		}
		catch(TrackerException* e)
		{
			WCHAR* msg = (WCHAR*)e->getText();
			LPARAM lParam = (LPARAM)msg;
			m_pMyMainWnd->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
			delete e;
		}

		ResetEvent(m_hSuspendResumeEvent);
		if(m_bCommandEventSet)
			SetEvent(m_hCommandDoneEvent);

		if(tCommand != EXIT)
		{
			VERIFY(WaitForSingleObject(m_hSuspendResumeEvent, INFINITE) == WAIT_OBJECT_0);
			try
			{
				if(!theApp.m_pFaroTracker->getBlocking())
				{
					theApp.m_pFaroTracker->setBlocking(true);
					bBlock = TRUE;
				}
			}
			catch(TrackerException* e)
			{
				WCHAR* msg = (WCHAR*)e->getText();
				LPARAM lParam = (LPARAM)msg;
				m_pMyMainWnd->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
				delete e;
			}
		}
	}
	return;
}

/*************************************************************************************************
* Function: OnConnect(WCHAR* ip)
* Comments: 
*************************************************************************************************/
void CWorkerThread::OnConnect(WCHAR* IpAddress)
{	
	m_pIpAdd = IpAddress;
	tCommand = CONNECT;
	m_pWorkerThread->ResumeThread();
	//SetEvent(m_hSuspendResumeEvent);
}

/*************************************************************************************************
* Function: OnDisconnect()
* Comments: 
*************************************************************************************************/
void CWorkerThread::OnDisconnect()
{
	if(IsConnected() != true) return;

	tCommand = DISCONNECT;
	m_bCommandEventSet = TRUE;
	SetEvent(m_hSuspendResumeEvent);

	//Here the UI thread will wait untill the worker thread completely Disconnects
	ResetEvent(m_hCommandDoneEvent);	
	VERIFY(WaitForSingleObject(m_hCommandDoneEvent, INFINITE) == WAIT_OBJECT_0);
	m_bCommandEventSet = FALSE;

	//After disconnecting the thread is asked to kill itself.
	tCommand = EXIT;
	SetEvent(m_hSuspendResumeEvent);

	DWORD dwExitCode;
	//After asking the thread to kill itself we will wait untill the worker thread kills itself
	if (m_pWorkerThread)
	{
		if(GetExitCodeThread(m_pWorkerThread->m_hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE)
		{			
			VERIFY(WaitForSingleObject(m_pWorkerThread->m_hThread, INFINITE) == WAIT_OBJECT_0);
		}			
	}
	if(m_AppName)
		delete m_AppName;
}

/*************************************************************************************************
* Function: ExecuteCommandSequence()
* Comments: 
*************************************************************************************************/
void CWorkerThread::ExecuteCommandSequence()
{
	tCommand = CMD_SEQUENCE;
	SetEvent(m_hSuspendResumeEvent);
}

/*************************************************************************************************
* Function: PostMsgToUpdateStatus(WCHAR* txt)
* Comments: 
*************************************************************************************************/
void CWorkerThread::PostMsgToUpdateStatus(WCHAR* txt)
{
	LPARAM lParam = (LPARAM)txt;
	m_pMyMainWnd->PostMessage(UWM_UPDATE_CMDSEQ_STATUS, lParam, 0);
	m_strMessage.Format(L"%s",txt);
}

/*************************************************************************************************
* Function: StartSearch()
* Comments: 
*************************************************************************************************/
void CWorkerThread::StartSearch()
{
	tCommand = SEARCH;
	SetEvent(m_hSuspendResumeEvent);
}

void CWorkerThread::StartLevelMeasure()
{
	tCommand = LEVEL_SEQUENCE;
	SetEvent(m_hSuspendResumeEvent);
}

/*************************************************************************************************
* Function: Abort()
* Comments: 
*************************************************************************************************/
void CWorkerThread::Abort()
{
	
	//Since worker thread is busy executing the Search command, Tracker command
	//is aborted from the UI thread.	
	try
	{
	//	theApp.m_pFaroTracker->setBlocking(true);
		theApp.m_pFaroTracker->abort();
	//	theApp.m_pFaroTracker->setBlocking(false)
	}	
	catch( TrackerException* e )
	{	
		WCHAR* msg = (WCHAR*)e->getText();
		LPARAM lParam = (LPARAM)msg;
		m_pMyMainWnd->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
		delete e;
	}
}

/*************************************************************************************************
* Function: OnAvailableApplications(CList<WCHAR*, WCHAR*> &AppsList)
* Comments: 
*************************************************************************************************/
void CWorkerThread::OnAvailableApplications(CList<WCHAR*, WCHAR*> &AppsList)
{
	tCommand = AVAILABLE_APPS;

	m_bCommandEventSet = TRUE;
	ResetEvent(m_hCommandDoneEvent);
	//Here also the UI thread waits for the worker thread to complete its operation
	//so that the UI thread can then access the data in m_AppsList filled by the worker thread	
	SetEvent(m_hSuspendResumeEvent);
	VERIFY(WaitForSingleObject(m_hCommandDoneEvent, INFINITE) == WAIT_OBJECT_0);
	m_bCommandEventSet = FALSE;

	for( POSITION pos = m_Appslist.GetHeadPosition(); pos; m_Appslist.GetNext(pos))
	{
		WCHAR* pAppName =  m_Appslist.GetAt(pos);
		AppsList.AddTail(pAppName);		
	}	
}

/*************************************************************************************************
* Function: OnRunApplication(WCHAR* appName)
* Comments: 
*************************************************************************************************/
void CWorkerThread::OnRunApplication(WCHAR* appName)
{
	tCommand = RUN_APP;
	m_AppName = new WCHAR[wcslen(appName) + 1];
	wcscpy_s(m_AppName, wcslen(appName) + 1, appName);
	m_CharAppName = new CHAR[wcslen(appName) + 1];
	CStringA sConverted = CStringA(appName);
	m_CharAppName = (const CHAR*)sConverted;
	SetEvent(m_hSuspendResumeEvent);
}

/*************************************************************************************************
* Function: OnStopApplication()
* Comments: 
*************************************************************************************************/
void CWorkerThread::OnStopApplication()
{
	tCommand = STOP_APPS;
	SetEvent(m_hSuspendResumeEvent);
}

/*************************************************************************************************
* Function: OnStartMeasurements()
* Comments: 
*************************************************************************************************/
void CWorkerThread::OnStartMeasurements()
{
//	if(STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd())->GetMeasurementType())	
//		OnStartForegndMeasurements();
//	else
//		OnStartBackgndMeasurements();
}

/*************************************************************************************************
* Function: OnStopMeasurements()
* Comments: 
*************************************************************************************************/
void CWorkerThread::OnStopMeasurements()
{
//	if(STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd())->GetMeasurementType())	
//		OnStopForegndMeasurements();
//	else
//		OnStopBackgndMeasurements();
}

/*************************************************************************************************
* Function: OnStartForegndMeasurements()
* Comments: 
*************************************************************************************************/
void CWorkerThread::OnStartForegndMeasurements()
{
	//Before the foregnd measurements are started we would like to supply the information
	//whether the measurements have to run untill user selects stop and even rate.
	tCommand = FOREGND_MEASURE;
//	bMulti = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd())->GetIfMultiMeasurements();
	bStopBkgnd = FALSE;
//	nEventRate = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd())->GetEventRate();
	SetEvent(m_hSuspendResumeEvent);
}

/*************************************************************************************************
* Function: OnStopForegndMeasurements()
* Comments: 
*************************************************************************************************/
void CWorkerThread::OnStopForegndMeasurements()
{
	bStopBkgnd = TRUE;
}

/*************************************************************************************************
* Function: OnStartBackgndMeasurements()
* Comments: 
*************************************************************************************************/
void CWorkerThread::OnStartBackgndMeasurements()
{
	//For backgnd measurements we are creating a new worker thread
	//For forgnd measurements the worker thread that is already existing is used.
	backGndInfo.bMulti = true;
	backGndInfo.pMainWnd = AfxGetMainWnd();
	backGndInfo.bStopBkgnd = FALSE;
	backGndInfo.nEventRate = 1;
	
	m_pBkgndThread = AfxBeginThread(StartBkgndMeasurements, (LPVOID)&backGndInfo,
									THREAD_PRIORITY_NORMAL, 0 ,0);
	m_bCollectingBackGround = true;
}

/*************************************************************************************************
* Function: StartBkgndMeasurements(LPVOID pParam)
* Comments: 
*************************************************************************************************/
UINT StartBkgndMeasurements(LPVOID pParam)
{
	
	if(!theApp.m_pFaroTracker)
	return 0;

	BACKGNDINFO* tBackGndInfo = (BACKGNDINFO*)pParam;
		
	MeasurePointData* pData = NULL;	
	SMXObjectArray* pDataArray = NULL;

	try
	{
		NullStartTrigger		startTrigger;
		IntervalTrigger			continueTrigger(0.25);
		AverageFilter			filter;
		MeasureCfg	cfg(100, &filter,&startTrigger, &continueTrigger);		//100 ms

		theApp.m_FaroWorkerThread.m_nCount=0;
		theApp.m_pFaroTracker->setBkndMeasureEventRate(tBackGndInfo->nEventRate);

		theApp.m_pFaroTracker->setBlocking(true);
		theApp.m_pFaroTracker->setBkndMeasureBlocking(true);
			
		theApp.m_pFaroTracker->startBkndMeasurePoint(&cfg);		

		if(tBackGndInfo->nEventRate == 1 && !tBackGndInfo->bMulti)
		{			
			pData = theApp.m_pFaroTracker->readBkndMeasurePointData();
			
			ASSERT(pData);

			LPARAM lParam = (LPARAM)pData;
			tBackGndInfo->pMainWnd->PostMessage(UWM_BKGND_MEASUREMENT_AVAILABLE,lParam, 0);	
		}
		else if(tBackGndInfo->nEventRate == 10 && !tBackGndInfo->bMulti)
		{
			pDataArray = NULL;
			pDataArray = theApp.m_pFaroTracker->readBkndMeasurePointData(tBackGndInfo->nEventRate);
			int numElements = pDataArray->getNumElements();
			for(int i = 0; i < numElements; i++)
			{
				pData = (MeasurePointData*)pDataArray->getElement(i);
				ASSERT(pData);
				LPARAM lParam = (LPARAM)pData;
				tBackGndInfo->pMainWnd->PostMessage(UWM_BKGND_MEASUREMENT_AVAILABLE,lParam, 0);
			}
		}
		else if(tBackGndInfo->nEventRate == 1 && tBackGndInfo->bMulti)
		{
			while(!tBackGndInfo->bStopBkgnd)
			{
				pData = theApp.m_pFaroTracker->readBkndMeasurePointData();
			
				if(pData)
				{
					theApp.m_FaroWorkerThread.m_dAzimuth = pData->azimuth();
					theApp.m_FaroWorkerThread.m_dZenith =  pData->zenith();
					theApp.m_FaroWorkerThread.m_dDistance = pData->distance();
					theApp.m_FaroWorkerThread.m_nStatus = pData->status();
					theApp.m_FaroWorkerThread.m_bNewValueAvailable = true;
					theApp.m_FaroWorkerThread.m_nCount++;

				}

			}
		}		
	}
	catch( TrackerException* e )
	{
		WCHAR* msg = (WCHAR*)e->getText();
		LPARAM lParam = (LPARAM)msg;
		tBackGndInfo->pMainWnd->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
		delete e;	
	}
	catch( SMXException* e )
	{	
		WCHAR msg[64];
		e->getMsg(msg, 64);
		LPARAM lParam = (LPARAM)msg;
		tBackGndInfo->pMainWnd->PostMessage(UWM_ON_SMXEXCEPTION, lParam, 0);
		delete e;
	}
	try
	{
		theApp.m_pFaroTracker->stopBkndMeasurePoint();
		theApp.m_pFaroTracker->setBkndMeasureBlocking(false);
		theApp.m_pFaroTracker->setBlocking(false);
	}
	catch( TrackerException* e )
	{
		WCHAR* msg = (WCHAR*)e->getText();
		LPARAM lParam = (LPARAM)msg;
		tBackGndInfo->pMainWnd->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
		delete e;	
	}
	return 0;
}

/*************************************************************************************************
* Function: OnStopBackgndMeasurements()
* Comments: 
*************************************************************************************************/
void CWorkerThread::OnStopBackgndMeasurements()
{
	backGndInfo.bStopBkgnd = TRUE;
	m_bCollectingBackGround = FALSE;
}

int  CWorkerThread::GetRawXYZCoordinate(D3DXVECTOR3 *pvRaw)
{
	D3DXMATRIXA16 matWorld,matRotX,matRotY,matRotZ;
	D3DXVECTOR3 vec;

	D3DXMatrixRotationZ(&matRotZ,(float)m_dAzimuth);
	D3DXMatrixRotationY(&matRotY,(float)m_dZenith);
	D3DXMatrixMultiply(&matWorld,&matRotY,&matRotZ);

	vec = D3DXVECTOR3(0.0f,0.0f,(float)m_dDistance*1000.0f);
	D3DXVec3TransformCoord(pvRaw,&vec,&matWorld);

	//Compensate for Level
	D3DXMatrixRotationX(&matRotX,(float)-m_dLevelRX);
	D3DXMatrixRotationY(&matRotY,(float)-m_dLevelRY);
	D3DXMatrixRotationZ(&matRotZ,(float)-m_dLevelRZ);
	D3DXMatrixMultiply(&matWorld,&matRotX,&matRotY);
	D3DXMatrixMultiply(&matWorld,&matWorld,&matRotZ);
	D3DXVec3TransformCoord(pvRaw,pvRaw,&matWorld);

	return m_nStatus = 1;
}


int CWorkerThread::GetRawXYZCoordinate(float *pfX,float *pfY,float *pfZ)
{
	D3DXVECTOR3 Vec;

	GetRawXYZCoordinate(&Vec);

	*pfX = Vec.x;
	*pfY = Vec.y;
	*pfZ = Vec.z;

	return m_nStatus;
}

int CWorkerThread::GetAZDCoordinate(D3DXVECTOR3 *pvRaw)
{

	pvRaw->x = (float)m_dAzimuth;
	pvRaw->y = (float)m_dZenith;
	pvRaw->z = (float)m_dDistance;
	return m_nStatus;
}

int CWorkerThread::GetAZDCoordinate(float *pfAzimuth,float *pfZenith,float *pfDistance)
{

	*pfAzimuth = (float)m_dAzimuth;
	*pfZenith = (float)m_dZenith;
	*pfDistance = (float)m_dDistance;
	return m_nStatus;
}

BOOL CWorkerThread::AddCoordinate()
{
	
	D3DXVECTOR3 Vec;

	if(GetRawXYZCoordinate(&Vec) == 1) {

		PROFILE->m_vCloud.Add(&Vec);
		return TRUE;
	}
	return FALSE;
}

void CWorkerThread::InitializeFrameMatrix()
{
	D3DXMATRIXA16 matRotZero,matRotY,matRotZ;
	float fYaw = atan2f(m_vFrameNorm.y, m_vFrameNorm.x);
	float fPitch = acosf(m_vFrameNorm.z);

	D3DXMatrixRotationY(&matRotY,fPitch);
	D3DXMatrixRotationZ(&matRotZ,fYaw);
	D3DXMatrixRotationZ(&matRotZero,m_vFrameRotate.z);
	D3DXMatrixMultiply(&m_matFrame,&matRotY,&matRotZ);
	D3DXMatrixTranspose(&m_matFrame,&m_matFrame);
	D3DXMatrixMultiply(&m_matFrame,&m_matFrame,&matRotZero);
/*
	float fYaw = atan2f(m_vFrameNorm.x, m_vFrameNorm.z);
	float fPitch = atan2f(-m_vFrameNorm.y , sqrtf(m_vFrameNorm.x * m_vFrameNorm.x + m_vFrameNorm.z * m_vFrameNorm.z));

	D3DXMatrixRotationYawPitchRoll( &m_matFrame, fYaw, fPitch, 0 );
	D3DXMatrixTranspose(&m_matFrame,&m_matFrame);
	D3DXMatrixMultiply(&m_matFrame,&m_matFrame,&matRotZ);
	*/
}

/*
FrameCoordinateFromWorld
D3DXVECTOR3 *pvFrame
D3DXVECTOR3 *pvRaw
*/
void CWorkerThread::FrameCoordinateFromWorld(D3DXVECTOR3 *pvFrame, D3DXVECTOR3 *pvRaw)
{

	InitializeFrameMatrix();

	*pvFrame = *pvRaw;
	pvFrame->x -= m_vFrameOrigin.x;
	pvFrame->y -= m_vFrameOrigin.y;
	pvFrame->z -= m_vFrameOrigin.z;

	D3DXVec3TransformCoord(pvFrame,pvFrame,&m_matFrame);

}

void CWorkerThread::GetFrameCoordinate(CUSLVertex &pvFrame, CUSLVertex &pvRaw)
{

	InitializeFrameMatrix();
	D3DXVECTOR3 Vt;

	pvFrame = pvRaw;

	Vt.x = pvRaw.fX;
	Vt.y = pvRaw.fY;
	Vt.z = pvRaw.fZ;

	Vt.x -= m_vFrameOrigin.x;
	Vt.y -= m_vFrameOrigin.y;
	Vt.z -= m_vFrameOrigin.z;

	D3DXVec3TransformCoord(&Vt,&Vt,&m_matFrame);

	pvFrame.fX = Vt.x;
	pvFrame.fY = Vt.y;
	pvFrame.fZ = Vt.z;
	
}

void CWorkerThread::RotateToFrameNorm(D3DXVECTOR3 *vNormOut,D3DXVECTOR3 *vNormIn)
{
	D3DXMATRIXA16 matRotZ;


	InitializeFrameMatrix();
	D3DXVec3TransformCoord(vNormOut,vNormIn,&m_matFrame);

//	vNormOut->x *=-1.0f;
//	vNormOut->y *=-1.0f;
//	vNormOut->z *=-1.0f;
}

void CWorkerThread::GetFrameRotateAngles(D3DXVECTOR3 *pvFrameRotate, D3DXVECTOR3 *pvRaw)
{
	D3DXVECTOR3 vFrame;

	FrameCoordinateFromWorld(&vFrame, pvRaw);

	pvFrameRotate->x = atan2f(vFrame.z, sqrtf(vFrame.y*vFrame.y + vFrame.x*vFrame.x));
	pvFrameRotate->y = atan2f(vFrame.z, sqrtf(vFrame.x*vFrame.x + vFrame.y*vFrame.y));
	pvFrameRotate->z = atan2f(vFrame.y, vFrame.x);
}

void CWorkerThread::GetToolCoordinate(D3DXVECTOR3 *pvTool, D3DXVECTOR3 *pvRaw)
{
	FrameCoordinateFromWorld(pvTool, pvRaw);

	pvTool->x -= m_vToolOrigin.x;
	pvTool->y -= m_vToolOrigin.y;
	pvTool->z -= m_vToolOrigin.z;

}

void CWorkerThread::GetToolCoordinate(CUSLVertex &pvTool, CUSLVertex &pvRaw)
{
	GetFrameCoordinate(pvTool, pvRaw);

	pvTool.fX -= m_vToolOrigin.x;
	pvTool.fY -= m_vToolOrigin.y;
	pvTool.fZ -= m_vToolOrigin.z;

}

void CWorkerThread::GetToolRotateAngles(D3DXVECTOR3 *pvToolRotate, D3DXVECTOR3 *pvRaw)
{
	D3DXVECTOR3 vTool;

	GetToolCoordinate(&vTool, pvRaw);

	pvToolRotate->x = atan2f(vTool.z, sqrtf(vTool.y*vTool.y + vTool.x*vTool.x));
	pvToolRotate->y = atan2f(vTool.z, sqrtf(vTool.x*vTool.x + vTool.y*vTool.y));
	pvToolRotate->z = atan2f(vTool.y, vTool.x);
}

void CWorkerThread::DeleteElements(int nStartIndex, int nFinishIndex)
{
	int	nn,nLength;
	nLength = nFinishIndex-nStartIndex+1;
	if(nStartIndex<0) return;

	for(nn=nStartIndex;nn<m_nElementL-nLength;nn++ ) {
		m_Element[nn]=&m_Element[nn+nLength];
	}
	m_nElementL -= nLength;

}


bool CWorkerThread::AddCloudCoordinate()
{
	

	D3DXVECTOR3 Vec,vPt;

	if(GetRawXYZCoordinate(&Vec) == 0) {

		for(int ii=0;ii<PROFILE->m_vCloud.GetSize();ii++) {
			PROFILE->m_vCloud.GetPt(ii,&vPt);
			vPt.x = Vec.x - vPt.x;
			vPt.y = Vec.y - vPt.y;
			vPt.z = Vec.z - vPt.z;
			if(D3DXVec3Length(&vPt) <= theApp.m_LastSettings.fFaroCloudLength) {
				return false;
			}
		}
		PROFILE->m_vCloud.Add(&Vec);
		return TRUE;
	}
	return FALSE;

}
