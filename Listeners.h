//Listeners.h
#ifndef _LISTENERS_H
#define _LISTENERS_H

class CommandDone;

BOOL CALLBACK SEARCHDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

class TrackerConnectListener : public ConnectListener
{
	public:		
		void connectionChanged( ConnectEvent* event );
};

class TrackerCmdCompleteListener : public CommandCompleteListener
{
	public:		
		void commandComplete( CommandCompleteEvent* event );
};

class TrackerStatuslistener : public StatusListener
{
	public:
		void statusChanged(StatusEvent* event);
};

class TrackerChangelistener : public ChangeListener
{
	public:		
		void changeOccurred( ChangeEvent* event);
};

class SearchCmdCompleteListener : public CommandCompleteListener
{
	public:			
		void commandComplete( CommandCompleteEvent* event );
};

class TrackerApplicationListener : public ApplicationListener
{
	public:		
		void stateChanged(ApplicationEvent* event);
};

class ForegndCmdCompleteListener : public CommandCompleteListener
{
	public:		
		void commandComplete( CommandCompleteEvent* event );
};

class TrackerMeasureDataListener : public MeasureDataListener
{
	public:		
		void dataAvailable( MeasureDataEvent* event );
};

class TrackerBkndMeasureDataListener: public BkndMeasureDataListener
{
	public:		
		void dataAvailable( BkndMeasureDataEvent* event );
};

class CListeners
{
public:
	void Connect(WCHAR* ipAdd);
	void Disconnect();
	
	void ExecuteCommandSequence();
	void SetCommandDone(CommandDone* pDone);

	void StartAsyncMsgs();
	void StopAsyncMsgs();
	void GetAsyncCommandStatus(TRACKERCOMMANDS cmd, bool & state);
	WCHAR* OnGetTargetAsyncStatus();
	void ExecuteAsyncCommand(TRACKERCOMMANDS Cmd, bool state);	
	void ChangeTarget(WCHAR* type);

	void OnAvailableApplications(CList<char*, char*> &lstApps);
	void OnStopApplication();
	void OnRunApplication(WCHAR* appName);
	void AddListenersforApps();
	void RemoveListenersofApps();

	void OnStartMeasurements();
	void OnStartForegndMeasurements();
	void OnStopForegndMeasurements();
	void OnRemoveForegndCmdCompleteListener();
	void OnStartBackgndMeasurements();
	void OnStopBackgndMeasurements();
	void OnStopMeasurements();

	void AddListeners();
	void RemoveListeners();		

	inline void SetAppRunning(bool run){m_bAppRunning = run;}
	inline bool GetAppRunning()const {return m_bAppRunning;}

private:	
	TrackerConnectListener* m_pConnectListener;
	TrackerCmdCompleteListener* m_pTrackerCmdCompleteListener;
	TrackerStatuslistener* m_pTrackerStatuslistener;
	TrackerChangelistener* m_pTrackerChangelistener;
	SearchCmdCompleteListener* m_pSearchCmdCompleteListener;
	TrackerApplicationListener* m_pTrackerApplicationListener;	
	ForegndCmdCompleteListener* m_pForegndCmdCompleteListener;
	TrackerMeasureDataListener* m_pMeasureDataListener;
	TrackerBkndMeasureDataListener* m_pBkndMeasureDataListener;	

	void ExecuteCommand(TRACKERCOMMANDS Cmd, bool& state);

	CommandDone* m_pCmdDone;
	
	bool m_bAppRunning;
};

#endif
