// RMessageHandler.h: interface for the CRMessageHandler class.
//
//A class for sending, receving and acting on remote messages.
//Use together with CRMessage amd CRemote.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMESSAGEHANDLER_H__51973AA4_EDA7_4449_B27B_1441618C2144__INCLUDED_)
#define AFX_RMESSAGEHANDLER_H__51973AA4_EDA7_4449_B27B_1441618C2144__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RMessage.h"
#include "RSendMessage.h"
#include "RReceiveMessage.h"
#include "NormalizePage.h"
#include "RMessageQueue.h"
#include <list>


class CRMessageHandler  
{
public:
	CRMessageHandler();
	virtual ~CRMessageHandler();
	std::list<CRMessage*> HandleMessage(CRMessage* mess);
	static HANDLE m_normalizeCompleteSemaphore;
	static HANDLE m_peakOrthogonalCompleteSemaphore;

private:
	CRMessage* m_message;
	CString m_dest;
	CString m_comm;
	bool CompareCommand(const wchar_t* command);
	bool CompareDestination(const wchar_t* dest);
	bool FindDestination(const wchar_t* dest);
	int GetDestIndex();
	std::list<CRMessage*> HandleMessage();
	std::list<CRMessage*> HandleScopeMessage();
	std::list<CRMessage*> HandleMessageForRemote();
	std::list<CRMessage*> HandleNormalizeMessage();
	std::list<CRMessage*> HandleTimeslotMessage();
	std::list<CRMessage*> HandleProfileMessage();
	std::list<CRMessage*> HandleUnknownMessage();
	int m_destIndex;

	//Scope functions
	std::list<CRMessage*> GetScopeData();
	std::list<CRMessage*> GetTrace();
	std::list<CRMessage*> GetWidth();
	std::list<CRMessage*> SetWidth();
	std::list<CRMessage*> GetDelay();
	std::list<CRMessage*> SetDelay();
	std::list<CRMessage*> SetGain();
	std::list<CRMessage*> GetGatesData(); 
	std::list<CRMessage*> GetRFType();
	std::list<CRMessage*> TimeslotWarningMessage();
	CRMessage* WarningMessage(CString humanReadableMessage);

	//Remote functions
	std::list<CRMessage*> ReplyToPing();

	//Normalize functions
	std::list<CRMessage*> Normalize();
	std::list<CRMessage*> PeakOrthogonal();
	std::list<CRMessage*> WaterPathLeft();
	std::list<CRMessage*> WaterPathRight();
	std::list<CRMessage*> RightToLeft();
	std::list<CRMessage*> LeftToRight();
	std::list<CRMessage*> GetNormalizeData();
	std::list<CRMessage*> GetSettings(UINT val);
	std::list<CRMessage*> GetNormalizeSettings();
	std::list<CRMessage*> GetPeakOrthogonalSettings();
	std::list<CRMessage*> GetWaterPathLeftSettings();
	std::list<CRMessage*> GetWaterPathRightSettings();

	//Timeslot functions
	std::list<CRMessage*> GetNames(); //Returns the names of the timeslots.

	//Profile
	std::list<CRMessage*> AddCoord();
	std::list<CRMessage*> DeleteCoord();
	std::list<CRMessage*> NewLine();

	//Pages etc.
	CNormalizePage* m_pNormalizePage;
	bool IsNormalizePageOpen();
	bool Is3DViewEditDlgOpen();
	bool Does3DViewEditDlgExist();

	CPolyCoord GenTestPolyCoord();

	//The downsampled RF trace.
    char m_RFTraceSampled[0x8000];
	bool SampleTraceData();
	int m_RFTraceSampledLength;

	std::list<CRMessage*> TimerActionOK();

	void Open3DViewEditDlg();
};
#endif // !defined(AFX_RMESSAGEHANDLER_H__51973AA4_EDA7_4449_B27B_1441618C2144__INCLUDED_)
