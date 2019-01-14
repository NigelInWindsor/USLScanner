//Blocking.h
#ifndef _BLOCKING_H
#define _BLOCKING_H

#include <afxtempl.h>
class CommandDone;

class CBlocking
{
public:
	void Connect(WCHAR* ipAdd);
	void Disconnect();
	
	void ExecuteCommandSequence();
	void SetCommandDone(CommandDone* pDone);

	void StartSearch();

	void AbortCommand();

	void OnAvailableApplications(CList<char*, char*> &lstApps);
	void OnStopApplication();
	void OnRunApplication(WCHAR* appName);

	void OnStartMeasurements(CList<MeasurePointData*, MeasurePointData*> &DataList);
	void OnStartForegndMeasurements(CList<MeasurePointData*, MeasurePointData*> &DataList);
	void OnStartBackgndMeasurements(CList<MeasurePointData*, MeasurePointData*> &DataList);

private:
	CommandDone* m_pCmdDone;
};

#endif
