//Blocking.cpp
#include "stdafx.h"
#include "uslscanner.h"
#include "CommandDone.h"
#include "Globals.h"
#include "Constants.h"
#include "Blocking.h"

/*************************************************************************************************
* Function: Connect(char* ip)
* Comments: Establishes a connection to the tracker. To connect to the Tracker ipAddress,
*			Username and Password must be supplied. Also the Tracker object must be intialized
*			before connecting to the tracker.
/*************************************************************************************************/
void CBlocking::Connect(WCHAR* ip)
{
	APP App=(APP) AfxGetApp();
	if(!App->m_pFaroTracker)
		return;

	try
	{
		App->m_pFaroTracker->setBlocking(true);
		App->m_pFaroTracker->connect(ip, L"user", L"");
		App->m_pFaroTracker->setBlocking(false);
		App->m_bFaroTrackerConnected = TRUE;
//		STATIC_DOWNCAST(App->m_bFaroTrackerConnected = TRUE);
	}
	catch(TrackerException* e)
	{
		//Messsages are not modified in this demo. If you would like messages can be modified
		//if an exception is thrown
		WCHAR* msg;
		msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		//if Tracker connection failes delete the memory of Tracker object
//		STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd())->OnConnectFailed(0,0);
		delete e;
	}
}

/*************************************************************************************************
* Function: Disconnect()
* Comments: Disconnects from the Tracker
*************************************************************************************************/
void CBlocking::Disconnect()
{
	APP App=(APP) AfxGetApp();
	if(!App->m_pFaroTracker)
		return;

	try
	{
		App->m_pFaroTracker->setBlocking(true);
		App->m_pFaroTracker->disconnect();
		App->m_pFaroTracker->setBlocking(false);
		App->m_bFaroTrackerConnected = false;
//		STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd())->SetConnected(FALSE);
	}
	catch(TrackerException* e)
	{
		WCHAR* msg;
		msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		delete e;
	}
}

/*************************************************************************************************
* Function: ExecuteCommandSequence()
* Comments: This method shows how Tracker commands can be executed one after another in blocking
*			mode.To execute the Tracker commands in Blocking mode is one of the easiest 
*			way. But, if there are too many commands then there is a chance for UI to get blocked
*************************************************************************************************/
void CBlocking::ExecuteCommandSequence()
{
	APP App=(APP) AfxGetApp();
	if(!App->m_pFaroTracker)
		return;	
	
	try
	{
		App->m_pFaroTracker->setBlocking(true);		

		m_pCmdDone->TrackerCommandDone("Checking if Tracker is initialized");		
		bool state = App->m_pFaroTracker->initialized();
		if(!state)
		{
			m_pCmdDone->TrackerCommandDone("Tracker not initialized, Initializing");
			App->m_pFaroTracker->initialize();
		}

		m_pCmdDone->TrackerCommandDone("Checking motor state");
		state = App->m_pFaroTracker->motorsOn();
		if(!state)
		{
			m_pCmdDone->TrackerCommandDone("Motors turned off, turning on");
			App->m_pFaroTracker->changeMotorState(true);
			m_pCmdDone->TrackerCommandDone("Turned on motors");
		}

		m_pCmdDone->TrackerCommandDone("Checking tracking state");
		state = App->m_pFaroTracker->trackingOn();
		if(!state)
		{
			m_pCmdDone->TrackerCommandDone("Tracking turned off, turning on");
			App->m_pFaroTracker->changeTrackingState(true);
			m_pCmdDone->TrackerCommandDone("Turned on tracking");
		}

		m_pCmdDone->TrackerCommandDone("Checking if Distance is valid");
		state = App->m_pFaroTracker->targetLocationValid();
		if(!state)
		{
			m_pCmdDone->TrackerCommandDone("Distance Invalid");
		}

		m_pCmdDone->TrackerCommandDone("Checking if Target present");
		state = App->m_pFaroTracker->targetPresent();
		if(!state)
		{
			m_pCmdDone->TrackerCommandDone("Target not present");
		}

		m_pCmdDone->TrackerCommandDone("Homing Tracker");
		App->m_pFaroTracker->home(true);
		m_pCmdDone->TrackerCommandDone("Home done");
		
		App->m_pFaroTracker->setBlocking(false);		
	}
	catch(TrackerException* e)
	{
		WCHAR* msg;
		msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		delete e;
	}
}

/*************************************************************************************************
* Function: SetCommandDone(CommandDone* pDone)
* Comments: 
*************************************************************************************************/
void CBlocking::SetCommandDone(CommandDone* pDone)
{
	m_pCmdDone = pDone;
}

/*************************************************************************************************
* Function: StartSearch()
* Comments: Searches for the Target at the specified radius.
*************************************************************************************************/
void CBlocking::StartSearch()
{
	APP App=(APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;

	try
	{
		App->m_pFaroTracker->setBlocking(true);
		App->m_pFaroTracker->search(3);
		App->m_pFaroTracker->setBlocking(false);	
	}
	catch(TrackerException* e)
	{
		WCHAR* msg;
		msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		delete e;
	}
}

/*************************************************************************************************
* Function: AbortCommand()
* Comments: Aborts the currently executing Tracker command.
*************************************************************************************************/
void CBlocking::AbortCommand()
{
	APP App=(APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;

	try
	{
		App->m_pFaroTracker->setBlocking(true);
		App->m_pFaroTracker->abort();
		App->m_pFaroTracker->setBlocking(false);	
	}
	catch(TrackerException* e)
	{
		WCHAR* msg;
		msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		delete e;
	}
}

/*************************************************************************************************
* Function: OnAvailableApplications(CList<char*, char*> &lstApps)
* Comments: Returns all the available java applications.
*************************************************************************************************/
void CBlocking::OnAvailableApplications(CList<char*, char*> &lstApps)
{
	APP App=(APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;

	SMXObjectArray* pAppsArray = NULL;
	try
	{
		App->m_pFaroTracker->setBlocking(true);

		pAppsArray = App->m_pFaroTracker->availableApplications();
		if(pAppsArray)
		{
			int numApps = pAppsArray->getNumElements();
			for(int i = 0; i < numApps; i++)
			{					
				char* name = (char*)pAppsArray->getElement(i);				
				char* temp = new char[strlen(name) + 1];
				strcpy(temp,name);
				lstApps.AddTail(temp);
			}
		}	
		App->m_pFaroTracker->setBlocking(false);	
	}
	catch(TrackerException* e)
	{
		WCHAR* msg;
		msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		delete e;
	}
	delete pAppsArray;
}

/*************************************************************************************************
* Function: OnStopApplication()
* Comments: Stops any java application that is currently running.
*************************************************************************************************/
void CBlocking::OnStopApplication()
{
	APP App=(APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;

	SMXObjectArray* pAppsArray = NULL;
	try
	{	
		App->m_pFaroTracker->setBlocking(true);
		//This method returns the aplications that are currently running.
		pAppsArray = App->m_pFaroTracker->startedApplications();

		if( pAppsArray)
		{
			int numApps = pAppsArray->getNumElements();
			for(int i = 0; i < numApps; i++)
			{				
				WCHAR* pAppRunning = (WCHAR*)pAppsArray->getElement(i);
				App->m_pFaroTracker->stopApplicationFrame(pAppRunning);				
			}
		}
		App->m_pFaroTracker->setBlocking(false);
	}
	catch(TrackerException* e)
	{
		WCHAR* msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		delete e;
	}
	delete pAppsArray;
}

/*************************************************************************************************
* Function: OnRunApplication(char* appName)
* Comments: Start an application
*************************************************************************************************/
void CBlocking::OnRunApplication(WCHAR* appName)
{
	APP App=(APP) AfxGetApp();
	try
	{
		App->m_pFaroTracker->setBlocking(true);
		App->m_pFaroTracker->startApplicationFrame(appName, L"");
		App->m_pFaroTracker->setBlocking(false);	
	}
	catch(TrackerException* e)
	{
		WCHAR* msg;
		msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		delete e;
	}
}

/*************************************************************************************************
* Function: OnStartMeasurements(CList<MEASUREDDATA, MEASUREDDATA> &DataList)
* Comments: 
*************************************************************************************************/
void CBlocking::OnStartMeasurements(CList<MeasurePointData*, MeasurePointData*> &DataList)
{
//	if(STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd())->GetMeasurementType())	
//		OnStartForegndMeasurements(DataList);
//	else
//		OnStartBackgndMeasurements(DataList);
}

/*************************************************************************************************
* Function: OnForegndMeasurements(CList<MEASUREDDATA, MEASUREDDATA> &DataList)
* Comments: Here measurement is started, data is read and measurement is stopped.
*************************************************************************************************/
void CBlocking::OnStartForegndMeasurements(CList<MeasurePointData*, MeasurePointData*> &DataList)
{
	APP App=(APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;	
	
	//With the UI thread in blocking mode both the foreground and background measurements 
	//cannot be run at the same time. So that option is ignored here.

	SMXObjectArray* pDataArray = NULL;
	MeasurePointData* pData = NULL;

	try
	{
		//Before starting the measurement a measurement configuration must be specified
		//for the Tracker to take observations.
		//In the measurement configuration no of samples per observation, type of filter,
		//type of start trigger, type of continue trigger has to be specified.
		//In the measurement configuration we can also specify time trigger,
		//distance trigger, digital trigger etc. By default here we are using
		//null and continue trigger. Refer to the documentation how they can be specified.

		NullStartTrigger		startTrigger;
		IntervalTrigger			continueTrigger(0.1);
		AverageFilter			filter;
		MeasureCfg	cfg(2000, &filter,&startTrigger, &continueTrigger);

		App->m_pFaroTracker->setBlocking(true);

		//Tells the Tracker how many records to be read.
		int nEventRate;// = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd())->GetEventRate();
		App->m_pFaroTracker->setMeasureEventRate(nEventRate);

		App->m_pFaroTracker->startMeasurePoint(&cfg);
		
		if(nEventRate == 1)
		{			
			//if the user selects only to read a single data then we can read
			//the data as below.
			pData = App->m_pFaroTracker->readMeasurePointData();			
			ASSERT(pData);
			DataList.AddTail(pData);
		}
		else if(nEventRate == 10)
		{
			//if the user selects to read data of 10 points then we can use an SMXObjectArray
			//to read the data.
			//All the data from SMXObjectArray must be copied into a local buffer because
			//if it is deleted all the data also gets deleted.
			pDataArray = NULL;
			pDataArray = App->m_pFaroTracker->readMeasurePointData(nEventRate);
			ASSERT(pDataArray);
			int numElements = pDataArray->getNumElements();
			ASSERT(nEventRate == numElements);
			for(int i = 0; i < numElements; i++)
			{
				pData = (MeasurePointData*)pDataArray->getElement(i);
				ASSERT(pData);
				DataList.AddTail(pData);				
			}
		}
		App->m_pFaroTracker->stopMeasurePoint();

		App->m_pFaroTracker->setBlocking(false);
	}
		
	catch( TrackerException* e )
	{
		WCHAR* msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		delete e;
	}
	catch( SMXException* e )
	{	
		WCHAR msg[50];
		e->getMsg(msg, 50);
		AfxMessageBox(msg);
		delete e;
	}
}

/*************************************************************************************************
* Function: OnBackgndMeasurements(CList<MEASUREDDATA, MEASUREDDATA> &DataList)
* Comments: Similiar to foreground measurements
*************************************************************************************************/
void CBlocking::OnStartBackgndMeasurements(CList<MeasurePointData*, MeasurePointData*> &DataList)
{
	APP App=(APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;	

	//With the UI thread in blocking mode both the foreground and background measuremnts 
	//cannot be run at the same time. So that option is ignored here.
	SMXObjectArray* pDataArray = NULL;
	MeasurePointData* pData = NULL;

	try
	{
		//Specifying measurement configuration.
		NullStartTrigger		startTrigger;
		IntervalTrigger			continueTrigger(0.25);
		AverageFilter			filter;
		MeasureCfg	cfg(20, &filter,&startTrigger, &continueTrigger);		

		App->m_pFaroTracker->setBlocking(true);

		//This blocking is just for readBkndmeasurePointData
		App->m_pFaroTracker->setBkndMeasureBlocking(true);

		int nEventRate;// = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd())->GetEventRate();
		App->m_pFaroTracker->setBkndMeasureEventRate(nEventRate);	

		App->m_pFaroTracker->startBkndMeasurePoint(&cfg);		

		if(nEventRate == 1)
		{			
			pData = App->m_pFaroTracker->readBkndMeasurePointData();
			ASSERT(pData);
			DataList.AddTail(pData);
		}
		else if(nEventRate == 10)
		{
			pDataArray = NULL;
			pDataArray = App->m_pFaroTracker->readBkndMeasurePointData(nEventRate);
			ASSERT(pDataArray);
			int numElements = pDataArray->getNumElements();			
			ASSERT(nEventRate == numElements);
			for(int i = 0; i < numElements; i++)
			{
				pData = (MeasurePointData*)pDataArray->getElement(i);
				ASSERT(pData);
				DataList.AddTail(pData);				
			}
		}		
	}
	catch( TrackerException* e )
	{
		WCHAR* msg;
		msg = e->getText();
		AfxMessageBox(msg );
		trackerDeleteString(msg);
		delete e;
	}
	catch( SMXException* e )
	{	
		WCHAR msg[50];
		e->getMsg(msg, 50);
		AfxMessageBox(msg);
		delete e;
	}
	try
	{
		App->m_pFaroTracker->stopBkndMeasurePoint();
		App->m_pFaroTracker->setBkndMeasureBlocking(false);
		App->m_pFaroTracker->setBlocking(false);
	}
	catch( TrackerException* e )
	{
		WCHAR* msg;
		msg = e->getText();
		AfxMessageBox(msg );
		trackerDeleteString(msg);
		delete e;
	}
}
