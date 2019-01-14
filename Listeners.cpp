//Listeners.cpp
#include "stdafx.h"
#include "uslscanner.h"
#include "Globals.h"
#include "Constants.h"
#include "CommandDone.h"
#include "Listeners.h"

HANDLE hCmdCompleteEvent;

/*************************************************************************************************
* Function: AddListeners()
* Comments: When the user selects the mode as Listeners mode then all these listeners are intialized.
*************************************************************************************************/
void CListeners::AddListeners()
{
	APP App = (APP) AfxGetApp();
	m_pConnectListener				= new TrackerConnectListener();
	m_pSearchCmdCompleteListener	= new SearchCmdCompleteListener();	
	m_pMeasureDataListener			= new TrackerMeasureDataListener();
	m_pForegndCmdCompleteListener	= new ForegndCmdCompleteListener();
	m_pBkndMeasureDataListener		= new TrackerBkndMeasureDataListener();	
	m_pTrackerCmdCompleteListener	= new TrackerCmdCompleteListener();		
	
	ASSERT(m_pConnectListener);		
	ASSERT(m_pSearchCmdCompleteListener);	
	ASSERT(m_pForegndCmdCompleteListener);
	ASSERT(m_pMeasureDataListener);
	ASSERT(m_pBkndMeasureDataListener);	
	ASSERT(m_pTrackerCmdCompleteListener);

	App->m_pFaroTracker->addConnectListener(m_pConnectListener);
	App->m_pFaroTracker->addCommandCompleteListener(m_pTrackerCmdCompleteListener);
}

/*************************************************************************************************
* Function: RemoveListeners()
* Comments: When the user disconnects from the Tracker all these listeners are removed
*************************************************************************************************/
void CListeners::RemoveListeners()
{
	APP App = (APP) AfxGetApp();
	App->m_pFaroTracker->removeConnectListener(m_pConnectListener);
	App->m_pFaroTracker->removeCommandCompleteListener(m_pTrackerCmdCompleteListener);

	if(m_pConnectListener)
		delete m_pConnectListener;	
	if(m_pSearchCmdCompleteListener)
		delete m_pSearchCmdCompleteListener;	
	if(m_pForegndCmdCompleteListener)
		delete m_pForegndCmdCompleteListener;
	if(m_pMeasureDataListener)
		delete m_pMeasureDataListener;
	if(m_pBkndMeasureDataListener)
		delete m_pBkndMeasureDataListener;
	if(m_pTrackerCmdCompleteListener)
		delete m_pTrackerCmdCompleteListener;

	m_pConnectListener				= NULL;	
	m_pSearchCmdCompleteListener	= NULL;	
	m_pForegndCmdCompleteListener	= NULL;
	m_pMeasureDataListener			= NULL;
	m_pBkndMeasureDataListener		= NULL;
	m_pTrackerCmdCompleteListener	= NULL;
}

//-------------------------------------------------------------------------------------------

/*DEMONSTRATING CONNECT LISTENER*/

/*************************************************************************************************
* Function: Connect(char* ip)
* Listener: Fires Connect Listener.
* Comments: Once the connection is established with the Tracker the listener fires
*************************************************************************************************/
void CListeners::Connect(WCHAR* ipAdd)
{
	APP App = (APP) AfxGetApp();
	if(!App->m_pFaroTracker)
		return;		
	AfxGetApp()->RestoreWaitCursor();
	App->m_pFaroTracker->connect(ipAdd, L"user", L"");
}

/*************************************************************************************************
* Function: Disconnect()
* Listener: Fires Connect Listener.
* Comments: Once the Tracker is disconnected the listener fires
*************************************************************************************************/
void CListeners::Disconnect()
{
	APP App = (APP) AfxGetApp();
	if(!App->m_pFaroTracker)
		return;		
	AfxGetApp()->RestoreWaitCursor();
	App->m_pFaroTracker->disconnect();
}

/*************************************************************************************************
* Function: connectionChanged( ConnectEvent *event )
* Fires: This listener fires when ever the tracker is connected or disconnected.
* Event: The event tells whether the Tracker is connected or disconnected
*************************************************************************************************/
void TrackerConnectListener::connectionChanged( ConnectEvent *event )
{
	bool connected = event->isConnected();
	LPARAM lParam = (LPARAM)connected;
	AfxGetMainWnd()->PostMessage(UWM_CONNECT_LISTENER_FIRED, lParam, 0 );
//	if(!connected)
//		STATIC_DOWNCAST(CMainFrame,AfxGetMainWnd())->SetDisconnectEvent();
}

//-------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------

/*DEMONSTRATING COMMAND COMPLETE LISTENER*/

/*************************************************************************************************
* Function: ExecuteCommandSequence()
* Listener: Command Complete Listener
* Comments: This functions uses the Command complete listener. 
*			Here the listener is added when the user selects to execute command sequence.
*			Also, an event is being created and we will wait on that event untill the
*			listener fires and resets the event.
*			PostMessage can also be used as per the requirement.
*************************************************************************************************/
void CListeners::ExecuteCommandSequence()
{
	APP App = (APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;		

	AfxGetMainWnd()->BeginWaitCursor();

	bool state;
	hCmdCompleteEvent = CreateEvent(NULL,TRUE,FALSE,NULL);	

	ExecuteCommand(IS_INITIALIZED, state);
	if(!state)
		ExecuteCommand(INITIALIZE, state);

	ExecuteCommand(MOTORS_ON, state);
	if(!state)
		ExecuteCommand(MOTORS_ON, state);	

	ExecuteCommand(TRACKING_ON, state);
	if(!state)
		ExecuteCommand(TRACKING_ON, state);	

	ExecuteCommand(TARGET_PRESENT, state);	

	ExecuteCommand(DISTANCE_VALID, state);

	ExecuteCommand(HOME, state);

	AfxGetMainWnd()->EndWaitCursor();
}

/*************************************************************************************************
* Function: ExecuteCommandSequence()
* Comments: This is just a Utility function
*************************************************************************************************/
void CListeners::ExecuteCommand(TRACKERCOMMANDS Cmd, bool& state)
{
	APP App = (APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;

	switch(Cmd)
	{
	case IS_INITIALIZED:
		{
			m_pCmdDone->TrackerCommandDone("Checking if Tracker is initialized");
			state = App->m_pFaroTracker->initialized();
			if(state)
				m_pCmdDone->TrackerCommandDone("Tracker is initialized");
		}
		break;
	case INITIALIZE:
		{
			m_pCmdDone->TrackerCommandDone("Tracker not initialized, Initializing");

			ResetEvent(hCmdCompleteEvent);
			App->m_pFaroTracker->initialize();
			VERIFY(WaitForSingleObject(hCmdCompleteEvent, INFINITE) == WAIT_OBJECT_0);
		}
		break;
	case MOTORS_ON:
		{
			m_pCmdDone->TrackerCommandDone("Checking motor state");			
			state = App->m_pFaroTracker->motorsOn();
			if(state)
				m_pCmdDone->TrackerCommandDone("Motors are on");
		}
		break;
	case TURN_MOTORS_ON:
		{
			m_pCmdDone->TrackerCommandDone("Motors turned off, turning on");
			ResetEvent(hCmdCompleteEvent);
			App->m_pFaroTracker->changeMotorState(true);
			VERIFY(WaitForSingleObject(hCmdCompleteEvent, INFINITE) == WAIT_OBJECT_0);
			m_pCmdDone->TrackerCommandDone("Turned on motors");
		}
		break;
	case TRACKING_ON:
		{
			m_pCmdDone->TrackerCommandDone("Checking tracking state");			
			state = App->m_pFaroTracker->trackingOn();
			if(state)
				m_pCmdDone->TrackerCommandDone("Tracking is on");
		}
		break;
	case TURN_TRACKING_ON:
		{
			m_pCmdDone->TrackerCommandDone("Tracking turned off, turning on");
			ResetEvent(hCmdCompleteEvent);
			App->m_pFaroTracker->changeTrackingState(true);
			VERIFY(WaitForSingleObject(hCmdCompleteEvent, INFINITE) == WAIT_OBJECT_0);
			m_pCmdDone->TrackerCommandDone("Turned on tracking");
		}
		break;
	case TARGET_PRESENT:
		{
			m_pCmdDone->TrackerCommandDone("Checking if Target present");
			state = App->m_pFaroTracker->targetPresent();
			if(!state)			
				m_pCmdDone->TrackerCommandDone("Target not present");
			else
				m_pCmdDone->TrackerCommandDone("Target present");
			
		}
		break;
	case DISTANCE_VALID:
		{
			m_pCmdDone->TrackerCommandDone("Checking if Distance is valid");
			state = App->m_pFaroTracker->targetLocationValid();
			if(!state)
				m_pCmdDone->TrackerCommandDone("Distance Invalid");
			else
				m_pCmdDone->TrackerCommandDone("Distance valid");
		}
		break;
	case HOME:
		{
			m_pCmdDone->TrackerCommandDone("Homing Tracker");
			ResetEvent(hCmdCompleteEvent);
			App->m_pFaroTracker->home(true);			
			VERIFY(WaitForSingleObject(hCmdCompleteEvent, INFINITE) == WAIT_OBJECT_0);
			m_pCmdDone->TrackerCommandDone("Home done");
		}
		break;	
	default:
		break;
	}	
}

/*************************************************************************************************
* Function: commandComplete( CommandCompleteEvent* event )
* Fires: This listener fires when ever the tracker a command submitted to the tracker is completed.
* Event: The event tells whether the command is completed or not.
* Comments: We are using this listener for Asynchronous messages also.
*************************************************************************************************/
void TrackerCmdCompleteListener::commandComplete( CommandCompleteEvent* event )
{	
	TrackerException* e  = event->getRelatedException();
	if(e)
	{
		try
		{
			// rethrow this exception as a derived class to get the appropriate catch block branching		
			e->throwThisException();
		}	
		//Connect exception is caught here specifically so that if Tracker connection failes
		//we would like to delete the memory of Tracker object and Listeners.
		catch( ConnectFailedException* e1 )
		{
			WCHAR*text = e1->getText();
			LPARAM lParam = (LPARAM)text;			
			AfxGetMainWnd()->PostMessage(UWM_CONNECT_FAILED, lParam, 0);
			trackerDeleteString(text);
			delete e1;		
		}
		catch( TrackerException* e1 )		
		{
			//If it is not an aborted exception it could be some other exception.		
			WCHAR*text = e1->getText();
			LPARAM lParam = (LPARAM)text;
			AfxGetMainWnd()->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
			trackerDeleteString(text);
			delete e1;
		}	
	}
	if(hCmdCompleteEvent)
		SetEvent(hCmdCompleteEvent);
	delete e;
}

/*************************************************************************************************
* Function: SetCommandDone(CommandDone* pDone)
* Comments: 
*************************************************************************************************/
void CListeners::SetCommandDone(CommandDone* pDone)
{
	if(pDone)
		m_pCmdDone = pDone;
}

//-----------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------

/*DEMONSTRATING ASYNCHRONOUS MESSAGES*/
/*DEMONSTRATING STATUS LISTENER AND CHANGE LISTENER*/

/*************************************************************************************************
* Function: StartAsyncMsgs()
* Comments: Starts the asynchronous messages.
*			Asynchronous messages have to be started and both the status listener and
*			change listener	should be added for the messages to fire.
*************************************************************************************************/
void CListeners::StartAsyncMsgs()
{
	APP App = (APP) AfxGetApp();
	try
	{
		//Adding the listeners.
		m_pTrackerStatuslistener = new TrackerStatuslistener();
		m_pTrackerChangelistener = new TrackerChangelistener();

		App->m_pFaroTracker->addStatusListener(m_pTrackerStatuslistener);
		App->m_pFaroTracker->addChangeListener(m_pTrackerChangelistener);

		App->m_pFaroTracker->setBlocking(true);
		//We are adding a command complete listener since in async msg dialog there
		//is an option to turn the motors ON and OFF. If motors are turned OFF and 
		//sight mode is toggled it should throw an exception and let the user
		//know that motors are turned off. For this purpose this listener
		//is added here.
		App->m_pFaroTracker->startAsync();
		App->m_pFaroTracker->setBlocking(false);
	}
	catch(TrackerException* e)
	{
		WCHAR* msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		delete e;
	}
}

/*************************************************************************************************
* Function: StopAsyncMsgs()
* Comments: Stop the Asynchronous messages. Before disconnecting from the Tracker
*			Asynchronous messages have to be stopped.
*************************************************************************************************/
void CListeners::StopAsyncMsgs()
{
	APP App = (APP) AfxGetApp();
	try
	{
		if(m_pTrackerStatuslistener)
		{
			App->m_pFaroTracker->removeStatusListener(m_pTrackerStatuslistener);
			delete m_pTrackerStatuslistener;
			m_pTrackerStatuslistener = NULL;
		}

		if(m_pTrackerChangelistener)
		{
			App->m_pFaroTracker->removeChangeListener(m_pTrackerChangelistener);
			delete m_pTrackerChangelistener;
			m_pTrackerChangelistener = NULL;
		}

		App->m_pFaroTracker->setBlocking(true);		
		App->m_pFaroTracker->stopAsync();
		App->m_pFaroTracker->setBlocking(false);
	}
	catch(TrackerException* e)
	{
		WCHAR* msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		delete e;
	}	
}

/*************************************************************************************************
* Function: GetAsyncCommandStatus(TRACKERCOMMANDS cmd, bool & state)
* Comments: 
*************************************************************************************************/
void CListeners::GetAsyncCommandStatus(TRACKERCOMMANDS cmd, bool & state)
{	
	APP App = (APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;

	try
	{
		switch(cmd)
		{		
		case MOTORS_ON:
				state = App->m_pFaroTracker->motorsOn();
			break;	
		case DISTANCE_VALID:
				state = App->m_pFaroTracker->targetLocationValid();
			break;
		case SIGHT_MODE:
			state = App->m_pFaroTracker->backsightOrientation();
			break;		
		default:
			break;
		}
	}
	catch(TrackerException* e)
	{
		WCHAR* msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		delete e;
	}
}

/*************************************************************************************************
* Function: OnGetTargetAsyncStatus()
* Comments: To know the type of Target
*************************************************************************************************/
WCHAR* CListeners::OnGetTargetAsyncStatus()
{
	APP App = (APP) AfxGetApp();
	//Type of target is returned and it has to be type casted to determine whether
	//it is a SMR type of target or Mirror type of target.
	TargetType *target = App->m_pFaroTracker->targetType();
	
	if(target)
	{	
		SMRTargetType* SMRTarget = dynamic_cast<SMRTargetType*>(target);
		if(SMRTarget)
		{
			//if it is an SMR type of Target diameter has to be checked
			//to detemine if it is an half inch or One and half inch target etc..
			double diameter = SMRTarget->getDiameter();
			for (int i = 0; i < sizeof(targets)/sizeof(TARGET_INFO); i++)
			{
				if (targets[i].diameter == diameter)
					return targets[i].keyword;
			}			
		}
		else
		{
			MirrorTargetType* mirrorTarget = dynamic_cast<MirrorTargetType*>(target);
				return L"SMM";
		}		
	}
	delete target;
	//default
	return targets[2].keyword;
}

/*************************************************************************************************
* Function: ExecuteAsyncCommand(TRACKERCOMMANDS Cmd, bool state)
* Comments: In this method we are trying to show some of changes in status listener
*************************************************************************************************/
void CListeners::ExecuteAsyncCommand(TRACKERCOMMANDS Cmd, bool state)
{
	APP App = (APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;
	try
	{
		switch(Cmd)
		{		
			//Whenever motors are turned On or Off status listener fires.
		case TURN_MOTORS_ON:
				App->m_pFaroTracker->changeMotorState(state);
			break;	
		case DISTANCE_VALID:
			{
				//Whenever the distance is not valid also the status listener fires.				
				if(state)
				{
					//Homing the Tracker in front sight
					App->m_pFaroTracker->home(false);					
				}
				else
					//To make the distance invalid the laser beam is pointed to a location
					//where there might not be a target so the Tracker thinks that beam is
					//broken and distance is invalid.
					App->m_pFaroTracker->move(90, 90, 12, false);
			}
			break;
		case SIGHT_MODE:		
			//When ever the sight mode is changed change listener fires.
			//If the sight is back it would toggle to front and 
			//if it is back it would toggle front.
			App->m_pFaroTracker->toggleSightOrientation();
			break;		
		default:
			break;
		}
	}
	catch(TrackerException* e)
	{
		WCHAR* msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		delete e;
	}
	//*Note: For the list of asynchronous status messages for which the status listener
	//and change listener fires refer to documentation
}

/*************************************************************************************************
* Function: ExecuteAsyncCommand(TRACKERCOMMANDS Cmd, bool state)
* Comments: This method shows how to change the target type of the Tracker.
*************************************************************************************************/

void CListeners::ChangeTarget(WCHAR* type)
{
	APP App= (APP) AfxGetApp();
	try
	{
		if((wcscmp(type,L"SMR 0.500 in") == 0) ||
		(wcscmp(type, L"SMR 0.875 in") == 0) ||
		(wcscmp(type, L"SMR 1.500 in") == 0) )
		{
			//if the target type is of SMR will have to set the diameter first before
			//changing the target type.			
			for (int i = 0; i < sizeof(targets)/sizeof(TARGET_INFO); i++)
			{
				if (wcscmp(targets[i].keyword, type) == 0 )
				{
					SMRTargetType SMRTarget(targets[i].diameter);
					App->m_pFaroTracker->changeTargetType(&SMRTarget);
					break;
				}
			}		
		}
		else
		{
			MirrorTargetType mirrorTarget;
			App->m_pFaroTracker->changeTargetType(&mirrorTarget);
		}
	}
	catch(TrackerException* e)
	{
		WCHAR* msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		delete e;
	}
}

/*************************************************************************************************
* Function: statusChanged(StatusEvent* event)
* Fires: Whenever a status event occurs this listener fires.
* Event: The event tells which type of status message is generated.
* Comments:	Whenever the listener fires it can be checked for which status message it fired
*			correspondingly update the status on the status bar or notify the user about it etc.
*************************************************************************************************/
void TrackerStatuslistener::statusChanged(StatusEvent* event)
{
	Status* status = event->getStatus();

	LPARAM lParam = (LPARAM)status;
	AfxGetMainWnd()->PostMessage(UWM_STATUS_CHANGED, lParam, 0);
}

/*************************************************************************************************
* Function: changeOccurred( ChangeEvent* event)
* Fires: Whenever a change event occurs this listeners fires.
* Event: change event
* Comments: Same as Status listener
*************************************************************************************************/
void TrackerChangelistener::changeOccurred( ChangeEvent* event)
{
	Change* change = event->getChange();
	LPARAM lParam = (LPARAM)change;
	AfxGetMainWnd()->PostMessage(UWM_CHANGE_OCCURED, lParam, 0);
}

//-------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------

/*DEMONSTRATING HOW TO HANDLE EXCEPTIONS AND HOW TO ABORT A TRACKER COMMAND*/

/*************************************************************************************************
* Function: SEARCHDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
* Comments: A call back function is being used for Search and Aborting Tracker command.
*			A call back function is being used here just to provide an example of how
*			a callback  function can used to execute Tracker commands 
*************************************************************************************************/
HWND hWND;
BOOL CALLBACK SEARCHDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	APP App = (APP) AfxGetApp();
	static SearchCmdCompleteListener* pSearchCmdCompleteListener = NULL;	

	hWND = hDlg;
	static BOOL abort;

	switch (message)
	{
		case WM_INITDIALOG:
			 abort = (BOOL)lParam;
			if(abort)
			{
				SetWindowText(hDlg,L"Abort Commmand");				
				ShowWindow(GetDlgItem(hDlg, IDCANCEL),SW_SHOW);
			}
			else
			{
				SetWindowText(hDlg,L"Generate Exception");
				ShowWindow(GetDlgItem(hDlg, IDCANCEL),SW_HIDE);
			}
			EnableWindow(GetDlgItem(hDlg, IDCANCEL),FALSE);
			pSearchCmdCompleteListener = new SearchCmdCompleteListener();
			
			return TRUE;			

		case WM_USER+1:
			App->m_pFaroTracker->addCommandCompleteListener(pSearchCmdCompleteListener);
			
			// search
			App->m_pFaroTracker->search(3);
			return TRUE;

		case WM_CLOSE:
			EndDialog(hDlg, IDOK);
			App->m_pFaroTracker->removeCommandCompleteListener(pSearchCmdCompleteListener);
			delete pSearchCmdCompleteListener;
			pSearchCmdCompleteListener = NULL;
			break;		

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				/*
			case IDC_BUTTON_SEARCH:
				switch (HIWORD(wParam)) 
				{
					case BN_CLICKED:
					App->m_pFaroTracker->addCommandCompleteListener(pSearchCmdCompleteListener);
					
					EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SEARCH), FALSE);
					if(abort)
						EnableWindow(GetDlgItem(hDlg, IDCANCEL), TRUE);
			
					// search
					App->m_pFaroTracker->search(3);
					break;					
				}
				return TRUE;
			*/
			case IDCANCEL:
				App->m_pFaroTracker->abort();
				return TRUE;
					
			case IDOK:				
//				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SEARCH), TRUE);
				if(abort)
						EnableWindow(GetDlgItem(hDlg, IDCANCEL), TRUE);

				return TRUE;			
			}
					
		}	
	return FALSE;
}

/*************************************************************************************************
* Function: commandComplete( CommandCompleteEvent* event )
* Fires: This listener fires when ever the tracker has completed executing Tracker command.
* Event: The event tells if the Tracker has thrown an exception or if a command has 
*		 executed sucessfully.
*************************************************************************************************/
void SearchCmdCompleteListener::commandComplete( CommandCompleteEvent* event )
{
	//Whenever Tracker throws an exception the exception has to be rethrown to determine
	//what type of exception Tracker has thrown.
	//An example is shown at the end of this file of how different exceptions can be caught		
	WCHAR* text;
	TrackerException* e  = event->getRelatedException();	
	try
	{
		// rethrow this exception as a derived class to get the appropriate catch block branching		
		e->throwThisException();
	}
	//This exception is thrown whenever the Tracker command has been successfully
	//aborted.
	catch( AbortedException* e1 )
	{		
		text = e1->getText();		
		delete e1;
		
	}
	catch( TrackerException* e1 )
	{
		//If it is not an aborted exception it could be some other exception.
		text = e1->getText();
		delete e1;
	}		
	//If exception is not thrown it means that the command is successful.	
	LPARAM lParam = (LPARAM)text;
	AfxGetMainWnd()->PostMessage(UWM_SEARCH_CMD_FIRED, lParam, 0);	
	PostMessage(hWND, WM_COMMAND, IDOK, 0L);
	delete e;

	//**Note: For the list of exceptions Tracker throws, refer the documentation.
}

//-------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------

/*DEMONSTRATING APPLICATION LISTENER AND BUSY LISTENER*/

/*************************************************************************************************
* Function: OnAvailableApplications(CList<char*, char*> &lstApps)
* Comments: Returns all the available applications
*************************************************************************************************/
void CListeners::OnAvailableApplications(CList<char*, char*> &lstApps)
{
	APP App= (APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;
	//SMXObjectArray is used to obtain an array of data. 

	SMXObjectArray* pAppsArray = NULL;		
	//Whenever SMXObjectArray is used to obtain any data, the data should be retrieved
	//and stored in a local buffer since once the SMXObjectArray object is deleted all
	//the data is deleted.

	try
	{
		//This method returns the list of available applications that can be started
		//through the driver.
		pAppsArray = App->m_pFaroTracker->availableApplications();
		
		if(pAppsArray)
		{			
			int numApps = pAppsArray->getNumElements();
			for(int i = 0; i < numApps; i++)
			{					
				//After getting the available applications we are retrieving the name
				//of each available applications and adding them to a list.

				//The pointer that is returned by get element must be cast to the correct object.
				char* name = (char*)pAppsArray->getElement(i);				
				char* temp = new char[strlen(name) + 1];				
				strcpy(temp,name);
				lstApps.AddTail(temp);
			}
		}
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
* Function: OnRunApplication(char * appName)
* Comments: starts the application
*************************************************************************************************/
void CListeners::OnRunApplication(WCHAR* appName)
{
	APP App = (APP) AfxGetApp();
	try
	{
		//To start the application the name of the application and then if any parameters are
		//required can be passed. Since we do not need to pass any parameters we are passing null.
		//For eg: if you would like to remove the buttons of home and backsight on the Trackerpad
		//then the  parameter to be passed can be like
		//CString paramStr = "-bHome = off -bBacksight = off"and call the function as
		//App->m_pFaroTracker->startApplicationFrame(appName, paramStr);

		//If you would like specify units for length and Angle
		//CString paramStr = "-uLength = millimeters, 1000.0 -uAngular = degrees, 57.29577951"
		//and call the function as	App->m_pFaroTracker->startApplicationFrame(appName, paramStr)
		//Please refer to the documentation(Interface User's Guide.rtf) for more information on this

//		App->m_pFaroTracker->startApplicationFrame(appName, "");
	}
	catch(TrackerException* e)
	{
		WCHAR* msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		delete e;
	}
}

/*************************************************************************************************
* Function: OnStopApplication()
* Comments: stops the application
*************************************************************************************************/
void CListeners::OnStopApplication()
{
	APP App = (APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;

	SMXObjectArray* pAppsArray = NULL;
	
	try
	{
		//returns all the applications that are currently running.
		pAppsArray = App->m_pFaroTracker->startedApplications();

		if( pAppsArray)
		{
			int numApps = pAppsArray->getNumElements();
			for(int i = 0; i < numApps; i++)
			{				
				WCHAR* pAppRunning = (WCHAR*)pAppsArray->getElement(i);
				try
				{
					App->m_pFaroTracker->setBlocking(true);
					//to stop the application have to specify the application name to be stopped.
					App->m_pFaroTracker->stopApplicationFrame(pAppRunning);
					App->m_pFaroTracker->setBlocking(false);
				}
				catch(TrackerException* e)
				{
					WCHAR* msg = e->getText();
					AfxMessageBox(msg);
					trackerDeleteString(msg);
					delete e;
				}
			}
		}
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
* Function: AddListenersforApps()
* Comments: Adding the busy listener and Application listener
*************************************************************************************************/
void CListeners::AddListenersforApps()
{
	APP App = (APP) AfxGetApp();

	m_pTrackerApplicationListener = new TrackerApplicationListener;
	if(m_pTrackerApplicationListener)
		App->m_pFaroTracker->addApplicationListener(m_pTrackerApplicationListener);
}

/*************************************************************************************************
* Function: stateChanged(ApplicationEvent* event)
* Fires: This listener fires when ever an application is started are stopped.
* Event: The event tells which application has started and whether it has started or stopped
*************************************************************************************************/
void TrackerApplicationListener::stateChanged(ApplicationEvent* event)
{
	WCHAR* name = event->getAppName();
	bool run = event->isAppRunning();
//	gListeners.SetAppRunning(run);
	LPARAM lParam = (LPARAM)run;
	WPARAM wParam = (WPARAM)name;
	AfxGetMainWnd()->PostMessage(UWM_APPLICATION_STATE_CHANGED, lParam, wParam);
}

/*************************************************************************************************
* Function: RemoveListenersofApps()
* Comments: Removing the listeners when all the applications are stopped.
*************************************************************************************************/
void CListeners::RemoveListenersofApps()
{
	APP App = (APP) AfxGetApp();
	App->m_pFaroTracker->removeApplicationListener(m_pTrackerApplicationListener);
	if(m_pTrackerApplicationListener)
	{
		delete m_pTrackerApplicationListener;
		m_pTrackerApplicationListener = NULL;
	}
} 

//--------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------

/*DEMONSTARTING MEASUREMENTS*/

/*************************************************************************************************
* Function: OnStartMeasurement()
* Comments: 
*************************************************************************************************/
void CListeners::OnStartMeasurements()
{
//	if(STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd())->GetMeasurementType())	
//		OnStartForegndMeasurements();
//	else
//		OnStartBackgndMeasurements();
}

/*************************************************************************************************
* Function: OnStopMeasurement()
* Comments: 
*************************************************************************************************/
void CListeners::OnStopMeasurements()
{
//	if(STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd())->GetMeasurementType())	
//		OnStopForegndMeasurements();
//	else
//		OnStopBackgndMeasurements();
}

/*************************************************************************************************
* Function: OnStartForegndMeasurements()
* Listener: Uses MeasureData Listener.
* Comments: Once the Measurements are started the data available listener fires when the
*			data is available. 
*************************************************************************************************/
void CListeners::OnStartForegndMeasurements()
{
	APP App = (APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;

	try
	{
		NullStartTrigger		startTrigger;
		IntervalTrigger			continueTrigger(0.1);
		AverageFilter			filter;
		MeasureCfg	cfg(2000, &filter,&startTrigger, &continueTrigger);

		//Before starting the measurements listener has to be added
		App->m_pFaroTracker->addMeasureDataListener(m_pMeasureDataListener);

		int rate;// = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd())->GetEventRate();
		App->m_pFaroTracker->setMeasureEventRate(rate);

		App->m_pFaroTracker->startMeasurePoint(&cfg);		
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
		WCHAR msg[64];
		e->getMsg(msg, 64);
		AfxMessageBox(msg);
		delete e;
	}
}

/*************************************************************************************************
* Function: OnStopForegndMeasurements()
* Comments: Stop the measurements.
*************************************************************************************************/
void CListeners::OnStopForegndMeasurements()
{
	APP App = (APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;	

	try
	{
		//remove the listener before stopping the measurements.
		App->m_pFaroTracker->removeMeasureDataListener(m_pMeasureDataListener);
		App->m_pFaroTracker->addCommandCompleteListener(m_pForegndCmdCompleteListener);
		App->m_pFaroTracker->stopMeasurePoint();
	}
	catch( TrackerException* e )
	{
		WCHAR* msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		delete e;		
	}
}

/*************************************************************************************************
* Function: dataAvailable( MeasureDataEvent* event )
* Listener: MeasureData Listener fires when the data is available after the measurements are started
* Event:	Event returns the measured data.
* Comments: The listener fires when all the no of records set by the measure event rate are available.
			Once the listener fires all the data can be retrieved.
			If the measure event rate is set for more than one observation then the data is returned
			in the form of SMXObjectArray. An example of this is also shown.
*************************************************************************************************/
void TrackerMeasureDataListener::dataAvailable( MeasureDataEvent* event )
{
	APP App = (APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;
	//Here we are using SMXObjectArray for all the cases like
	//If the user wants to read a single point of data or 10 points of data or just
	//keep on collecting data untill the user selects stop.
	//This is just being done to provide an example that SMXObjectArray can be used in all cases.
	//Where as in blocking mode and in worker thread mode for single point data
	//we are using readmeasurepointdata to provide an example that if single point data
	//is required it can be done in that way and for multiple data this way.
 	
	SMXObjectArray* pDataArray = NULL;
	MeasurePointData* pData = NULL;
	try
	{		
		int rate = App->m_pFaroTracker->getMeasureEventRate();
		pDataArray = App->m_pFaroTracker->readMeasurePointData(rate);
		ASSERT(pDataArray);
		int numElements = pDataArray->getNumElements();
		ASSERT(numElements == rate);
		for(int i = 0; i < numElements; i++)
		{
			pData = (MeasurePointData*)pDataArray->getElement(i);
			ASSERT(pData);
			if(pData)
			{
				LPARAM lParam = (LPARAM)pData;
				AfxGetMainWnd()->PostMessage(UWM_FOREGND_MEASUREMENT_AVAILABLE, lParam, 0);
				Sleep(200);
			}
		}
	}
	catch( TrackerException* e )
	{
		WCHAR* msg = e->getText();
		LPARAM lParam = (LPARAM)msg;
		AfxGetMainWnd()->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
		delete e;
	}
}

/*************************************************************************************************
* Function: commandComplete( CommandCompleteEvent* event )
* Listener: Command Complete Listener
* Comments: When the measurement has been completely stopped this listener fires
*************************************************************************************************/
void ForegndCmdCompleteListener::commandComplete( CommandCompleteEvent* event )
{
	TrackerException* e = event->getRelatedException();
	if(e)	
	{		
		WCHAR* text = e->getText();
		LPARAM lParam = (LPARAM)text;
		AfxGetMainWnd()->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
	}
	else
	{	
		AfxGetMainWnd()->PostMessage(UWM_FOREGND_MEASUREMENT_STOPPED);
	}
	delete e;
}

/*************************************************************************************************
* Function: OnRemoveForegndCmdCompleteListener
* Comments: Remove the listener once the measurements are stopped
*************************************************************************************************/
void CListeners::OnRemoveForegndCmdCompleteListener()
{
	APP App = (APP) AfxGetApp();
	App->m_pFaroTracker->removeCommandCompleteListener(m_pForegndCmdCompleteListener);
}

/*************************************************************************************************
* Function: OnStartBackgndMeasurements()
* Listener: Uses Background MeasureData Listener.
* Comments: Start the Back ground measurements
*************************************************************************************************/
void CListeners::OnStartBackgndMeasurements()
{
	APP App = (APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;

	//The best option to do measurements is before starting the measurements add the
	//listeners and start the measurements. The listener fires whenever data is available
	//collect the data, remove the listeners and stop the measurements using blocking.

	try
	{
		NullStartTrigger		startTrigger;
		IntervalTrigger			continueTrigger(0.25);
		AverageFilter			filter;
		MeasureCfg	cfg(20, &filter,&startTrigger, &continueTrigger);

		App->m_pFaroTracker->addBkndMeasureDataListener(m_pBkndMeasureDataListener);

		int rate;// = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd())->GetEventRate();
		App->m_pFaroTracker->setBkndMeasureEventRate(rate);

		App->m_pFaroTracker->startBkndMeasurePoint(&cfg);		
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
		WCHAR msg[64];
		e->getMsg(msg, 64);
		LPARAM lParam = (LPARAM)msg;
		STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd())->PostMessage(UWM_ON_SMXEXCEPTION, lParam, 0);
		delete e;
	}
}

/*************************************************************************************************
* Function: OnStopBackgndMeasurements()
* Comments: In this method it can be seen that Setblocking is used to stop the background
*			measurements. This is being done to show how setblocking can be used in combination 
*			with listeners.
*			For ex: Starting and stopping the measurements can be done with setblocking
*			and the data can be read whenever the listener fires when the data is available
*			(Make sure that Listeners are added before the measurements are started for this
*			combination to work). The advantage of this is that it doesn't block the UI.
*************************************************************************************************/
void CListeners::OnStopBackgndMeasurements()
{
	APP App = (APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;	

	try
	{	
		//for backgnd measurements stop the measurements and then remove the listener.
		//if first the listener is removed without stopping the measurements it would hang
		//its actually a bug in the driver which has to be fixed.
		App->m_pFaroTracker->setBlocking(true);
		App->m_pFaroTracker->stopBkndMeasurePoint();
		App->m_pFaroTracker->setBlocking(false);
		App->m_pFaroTracker->removeBkndMeasureDataListener(m_pBkndMeasureDataListener);
	}
	catch( TrackerException* e )
	{
		WCHAR* msg = e->getText();
		AfxMessageBox(msg);
		trackerDeleteString(msg);
		delete e;
	}
}

/*************************************************************************************************
* Function: dataAvailable( BkndMeasureDataEvent* event )
* Listener: Uses Background measure data listener
* Event:	Returns the data
* Comments: This listener fires when ever the data is available once the measurements are started.
*************************************************************************************************/
void TrackerBkndMeasureDataListener::dataAvailable( BkndMeasureDataEvent* event )
{
	APP App = (APP) AfxGetApp();
	if(!App->m_pFaroTracker)
	return;

	SMXObjectArray* pDataArray = NULL;
	MeasurePointData* pData = NULL;
	try
	{	
		int nEventRate = App->m_pFaroTracker->getBkndMeasureEventRate();
		pDataArray = App->m_pFaroTracker->readBkndMeasurePointData(nEventRate);
		ASSERT(pDataArray);
		int numElements = pDataArray->getNumElements();
		ASSERT(numElements == nEventRate);
		for(int i = 0; i < numElements; i++)
		{
			pData = (MeasurePointData*)pDataArray->getElement(i);
			ASSERT(pData);
			if(pData)
			{
				LPARAM lParam = (LPARAM)pData;
				AfxGetMainWnd()->PostMessage(UWM_BKGND_MEASUREMENT_AVAILABLE,lParam, 0);
				Sleep(200);
			}
		}
	}
	catch( TrackerException* e )
	{
		WCHAR* msg = e->getText();
		LPARAM lParam = (LPARAM)msg;
		AfxGetMainWnd()->PostMessage(UWM_COMMAND_FAILED, lParam, 0);
		delete e;
	}	
}


/* An example on how to typecast exceptions.

  try
  {

  }
 catch(TrackerException* e)
 {		
	try
	{
		// rethrow this exception as a derived class to get the appropriate catch block branching
		// note that although the "e" variables have different scope, they point to the same object
		e->throwThisException();
	}

	// generic tracker command failures are handled here ...
	catch( AbortedException* e )
	{		
		delete e;
	}
	catch( CommunicationException* e )
	{		
		delete e;
	}
	catch( DistanceSetException* e )
	{
		delete e;
	}		
	catch( InternalDeviceFailureException* e )
	{		
		delete e;
	}
	catch( LaserStateException* e )
	{		
		delete e;
	}
	catch( MotorStateException* e )
	{			
		delete e;
	}
	catch( NoResponseException* e )
	{		
		delete e;
	}
	catch( ShutterStateException* e )
	{
		delete e;
	}	
	catch( ConnectFailedException* e )
	{
		delete e;
	}
	catch( NotConnectedException* e )
	{
		delete e;
	}
	catch( DisconnectFailedException* e )
	{
		delete e;
	}	
	catch( NoTargetException* e )
	{
		delete e;
	}	
	catch( DeviceLockedException* e )
	{
		delete e;
	}
	catch( InterfaceBusyException* e )
	{
		delete e;
	}
	catch( AlreadyConnectedException* e )
	{
		delete e;
	}
	catch( ApplicationFailedException* e )
	{
		delete e;
	}
	catch( AsyncAlreadyStartedException* e )
	{
		delete e;
	}
	catch( ClassLoadException* e )
	{
		delete e;
	}
	catch( DataFormatException* e )
	{
		delete e;
	}	
	catch( BadCommandException* e )
	{
		delete e;
	}	
	catch( MeasureTypeException* e )
	{
		delete e;
	}	
	catch( NoDataAvailableException* e )
	{
		delete e;
	}	
	catch( PermissionException* e )
	{
		delete e;
	}	
	catch( TrackingStateException* e )
	{
		delete e;
	}
	catch( UnsupportedFeatureException* e )
	{
		delete e;
	}
	catch( UserEntryException* e )
	{
		delete e;
	}
	catch( UserPromptException* e )
	{
		delete e;
	}
	catch( TrackerException* e)
	{
		delete e;
	}
}
*/
