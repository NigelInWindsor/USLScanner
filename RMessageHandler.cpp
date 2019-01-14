// RMessageHandler.cpp: implementation of the CRMessageHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "uslscanner.h"
#include "RMessageHandler.h"
#include "Remote.h"
#include "MainFrm.h"
#include "Coord.h"
#include <ctype.h>
#include "3DViewEditDlg.h"
#include "ProfileSheet.h"
#include "TimeSlotData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CRMessageHandler::CRMessageHandler(){
	m_dest=L"";
	m_comm=L"";
	m_message=NULL;
	m_destIndex=0;
	m_RFTraceSampledLength=0;
}

CRMessageHandler::~CRMessageHandler()
{
}


std::list<CRMessage*> CRMessageHandler::HandleMessage(CRMessage* mess){
	m_message = mess;
	m_dest = m_message->GetDestination();
	m_comm = m_message->GetCommand();
	return HandleMessage();
}


//Handling remote messages
std::list<CRMessage*> CRMessageHandler::HandleMessage(){
	std::list<CRMessage*> mlist;
	//SP0 to SPN for the N scopes.
	//Check that the destination begins with "SP".
	if(FindDestination(L"SP")){
		//Find the time slot
		m_destIndex = GetDestIndex();
		if(m_destIndex<0){
		//	CString errMess = "RMessageHandler::HandleMessage() - error - incorrect destination format: ";
		//	errMess += m_dest;
		//	AfxMessageBox(errMess);
		//	AfxThrowUserException();
		//	return mlist; //Return the empty message list.
			return HandleUnknownMessage();
		}
		return HandleScopeMessage();
	}else if(CompareDestination(L"REMT")){
		return HandleMessageForRemote();
	}else if(CompareDestination(L"NORM")){
		return HandleNormalizeMessage();
	}else if(CompareDestination(L"TSLT")){
		return HandleTimeslotMessage();
	}else if(CompareDestination(L"PROF")){
		return HandleProfileMessage();
	}else{
	//	AfxMessageBox(CString(L"Unknown remote message destination."));
		return HandleUnknownMessage();
	}

	return mlist; //Return the empty message list.
}

//============================================================================
//scope functions
//----------------------------------------------------------------------------

//Actions to perform on receipt of an RMessage
std::list<CRMessage*> CRMessageHandler::HandleScopeMessage(){
	std::list<CRMessage*> mlist;
	//Check that the requested time slot is available.
	int reqTimeSlot = m_destIndex;
	
	int availTimeSlots = theApp.m_UtUser.GetActiveTimeSlots()->GetSize();
	if(reqTimeSlot >= availTimeSlots){ 
		m_destIndex=0; //Requested time slot not available. Send to the first time slot.
		m_dest=L"SP00";
		return TimeslotWarningMessage();
	}

	if(CompareCommand(L"GDAT")) { return GetScopeData();}
	if(CompareCommand(L"GTRC")) { return GetTrace();}
	if(CompareCommand(L"GWDH")) { return GetWidth();}
	if(CompareCommand(L"SWDH")) { return SetWidth();}
	if(CompareCommand(L"GDLY")) { return GetDelay();}
	if(CompareCommand(L"SDLY")) { return SetDelay();}
	if(CompareCommand(L"SGAN")) { return SetGain();}
	if(CompareCommand(L"GGDT")) { return GetGatesData();}
	if(CompareCommand(L"GRFT")) { return GetRFType(); }
	if(CompareCommand(L"PING")) { return ReplyToPing(); }
//	AfxMessageBox(CString("CRemote: Unknown remote message scope command.")); For debugging only
	return HandleUnknownMessage();
	return mlist; //Return the empty message list.
}

CRMessage* CRMessageHandler::WarningMessage(CString humanReadableMessage){
	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"WARN");
	sendMess->SetData(humanReadableMessage);	
	std::list<CRMessage*> mlist;
	return sendMess;
}


std::list<CRMessage*> CRMessageHandler::TimeslotWarningMessage(){
	std::list<CRMessage*> mlist;
	CRMessage* sendMess = WarningMessage(CString("Warning: requested time slot not available. Changing scope display to first time slot."));
	sendMess->SetCommand(L"UTLT"); //Update time slot.
	mlist.push_back(sendMess);
	return mlist;
}


std::list<CRMessage*> CRMessageHandler::GetScopeData(){
	int timeSlot = m_destIndex;
	
	ADC200Data* pAdc=&theApp.m_UtUser.m_TS[timeSlot].Adc;
	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"UDAT");


	//Width
	sendMess->SetData(theApp.m_UtUser.m_TS[m_destIndex].Adc.nWidth);
	//Maximum width
	sendMess->AppendData(CString(","));
	sendMess->AppendData(theApp.m_Scope.m_fMaxRange);
	//Delay
	sendMess->AppendData(CString(","));
	sendMess->AppendData(theApp.m_UtUser.m_TS[m_destIndex].Adc.nDelay);
	//RFType
	sendMess->AppendData(CString(","));
	sendMess->AppendData(pAdc->nRfType);
	sendMess->AppendData(CString(","));
	sendMess->AppendData(theApp.m_LastSettings.nLogRange);
	//Gain
	sendMess->AppendData(CString(","));
	
	CTimeSlotData* pTimeSlot = theApp.m_UtUser.GetActiveTimeSlots()->Get(m_destIndex);
	if(pTimeSlot != NULL){ //Ensure the time slot is available.
		sendMess->AppendData(pTimeSlot->GetGain());
	} else {
		sendMess->AppendData(-1); //Signal not to update the gain on the remote device as we do not have the time slot yet.
	}
	
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;
}

std::list<CRMessage*> CRMessageHandler::GetTrace(){
	int timeSlot = m_destIndex;
	
	ADC200Data* pAdc=&theApp.m_UtUser.m_TS[timeSlot].Adc;
	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"UTRC");
	if(SampleTraceData()){
		sendMess->SetData(m_RFTraceSampled,m_RFTraceSampledLength);
	}else{
		sendMess->SetData(theApp.m_Scope.m_RFTrace[timeSlot],pAdc->nAcqLength);
	}
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;
}

bool CRMessageHandler::SampleTraceData(){
	int timeSlot = m_destIndex;
	
	ADC200Data* pAdc=&theApp.m_UtUser.m_TS[timeSlot].Adc;
	float frameWidthPixels=748; //The frame width in pixels of the scope on the android device
	float rawTraceLength = (float)pAdc->nAcqLength;
	if(rawTraceLength <= frameWidthPixels) return false;
	float xx;
	int nS0, nS1, nn; 
	float deltaXPixels = rawTraceLength/frameWidthPixels;
	int xxlast = (int)(frameWidthPixels-1.0f);
	char amplitude;
	char nMin;
	char nMax;
	for(xx=0, nn=0; xx<xxlast; xx++){
		nS0 = (int) (xx*deltaXPixels);
		nS1 = (int) ((xx+1) * deltaXPixels);
		nMin = 127;
		nMax = -127;

		for (int nPtr = nS0; nPtr < nS1; nPtr++){
			amplitude = theApp.m_Scope.m_RFTrace[timeSlot][nPtr];
			if (amplitude>=nMax){
				nMax = amplitude;
				
			
			}
			if (amplitude<=nMin){
				nMin = amplitude;
			}
		}
		m_RFTraceSampled[nn++] = nMax;
		m_RFTraceSampled[nn++] = nMin;
	}


	m_RFTraceSampledLength=nn;
	return true;
}

std::list<CRMessage*> CRMessageHandler::GetRFType(){
	int timeSlot = m_destIndex;
	
	ADC200Data* pAdc=&theApp.m_UtUser.m_TS[timeSlot].Adc;
	CRMessage* sendMess = new CRMessage();
	sendMess->SetData(pAdc->nRfType);
	sendMess->AppendData(CString(","));
	sendMess->AppendData(theApp.m_LastSettings.nLogRange);
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"URFT");
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;

}

std::list<CRMessage*> CRMessageHandler::GetWidth(){
	
	CRMessage* sendMess = new CRMessage();
	sendMess->SetData(theApp.m_UtUser.m_TS[m_destIndex].Adc.nWidth);
	sendMess->SetDestination(m_dest);
	//Update the width 
	sendMess->SetCommand(L"UWDH");
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;
}

std::list<CRMessage*> CRMessageHandler::SetWidth(){
	
	int width = -1;
	m_message->GetData(&width);	
	int maxTime = (int)(theApp.m_Scope.m_fMaxRange/1e-9f);
	int delay = theApp.m_UtUser.m_TS[m_destIndex].Adc.nDelay;
	//Put limits on the width.
	if(width<5) width=5;
	if((delay + width) > maxTime){
		width = maxTime - delay; 
	}

	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"STMS");
//	sendMess->SetData(CString("Delay updated."));
	sendMess->Pack();
	if(width>=5){
		theApp.m_UtUser.m_TS[m_destIndex].Adc.nWidth = width;
	}
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;
}

std::list<CRMessage*> CRMessageHandler::GetDelay(){
	
	CRMessage* sendMess = new CRMessage();
	sendMess->SetData(theApp.m_UtUser.m_TS[m_destIndex].Adc.nDelay);
	sendMess->SetDestination(m_dest);
	//Update the width 
	sendMess->SetCommand(L"UDLY");
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;
}

std::list<CRMessage*> CRMessageHandler::SetDelay(){
	
	int delay = -1;
	m_message->GetData(&delay);	

	int maxTime = (int)(theApp.m_Scope.m_fMaxRange/1e-9f);
	int width = theApp.m_UtUser.m_TS[m_destIndex].Adc.nWidth;
	//Put limits on the delay.
	if(delay<0) delay=0;
	if((delay + width) > maxTime){
		delay = maxTime - width; 
	}

	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"STMS");
//	sendMess->SetData(CString("Delay updated."));

	if(delay>=0){
		theApp.m_UtUser.m_TS[m_destIndex].Adc.nDelay = delay;
	}
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;
}

std::list<CRMessage*> CRMessageHandler::SetGain(){
	int timeSlot = m_destIndex;
	
	int gain = 0;
	m_message->GetData(&gain);	

	//Set the gain of the appropriate timeslot

//	CTimeSlotData* pDat = theApp.m_UtUser.GetActiveTimeSlots()->Get(timeSlot);
//	if(pDat){
//		pDat->SetGain(gain);
//	}

 theApp.m_UtUser.SelectWriteTimeslot();
	Pr30Data* m_pPr30 = &theApp.m_UtUser.m_TS[timeSlot].Pr30;
	m_pPr30->nPreampGain = gain;
	theApp.m_UtUser.Pr30PreampGain(m_pPr30);

	//Tell the ultrasonics dialogs to update all their controls
	CMainFrame* pFrame = (CMainFrame*)theApp.m_pMainWnd;
	pFrame->SendMessage(REMOTE_HANDLER_ULTRASONICS,REMOTE_UPDATE_ALL_CONTROLS);

	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"STMS");

	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;
}

std::list<CRMessage*> CRMessageHandler::GetGatesData(){
	//m_destIndex  is the timeslot number
	CRMessage* sendMess = new CRMessage();
	sendMess->SetGatesData(m_destIndex);
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"UGDT");
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;
}

//-------------------------------------------------------------------------



//=========================================================================
// Methods for the "Remote" object
//-------------------------------------------------------------------------


std::list<CRMessage*> CRMessageHandler::HandleMessageForRemote(){
	std::list<CRMessage*> mlist;
	//Close the remote connection sockets
	if(CompareCommand(L"PING")) { return ReplyToPing(); }
	return HandleUnknownMessage();
	return mlist; //Return the empty message list.
}


std::list<CRMessage*> CRMessageHandler::ReplyToPing(){
	
	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(m_dest);
	//Update the width 
	sendMess->SetCommand(L"PONG");
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;
}

bool CRMessageHandler::CompareCommand(const wchar_t * command)
{
	return (m_comm.Compare(CString(command)) == 0);
}

bool CRMessageHandler::CompareDestination(const wchar_t * dest)
{
	return (m_dest.Compare(CString(dest)) == 0);
}

bool CRMessageHandler::FindDestination(const wchar_t * dest)
{
	return (m_dest.Find(CString(dest)) == 0);
}

int CRMessageHandler::GetDestIndex(){
	//The third and fourth characters are integers.
	CString iStr = m_dest.Mid(2,2);
	if(isdigit(iStr.GetAt(0))){
		if(isdigit(iStr.GetAt(1))){
			return _wtoi(iStr);
		}else {
			return -1;
		}
	} else {
		return -1;
	}
}

//===============================================================
//Messgaes for NormalizePage
//---------------------------------------------------------------

std::list<CRMessage*> CRMessageHandler::HandleNormalizeMessage(){
	std::list<CRMessage*> mlist;
	
	if(!IsNormalizePageOpen()) {//Try to get the normalize page and return if unsuccessful;
			CRMessage* sendMess = new CRMessage();
			sendMess->SetDestination(m_dest);
			sendMess->SetCommand(L"WARN");
			sendMess->SetData(CString(L"Warning from USLScanner: \"optimise\" page not open. Action will have no effect."));

			mlist.push_back(sendMess);
			return mlist;
		}
		
	if(CompareCommand(L"NMLZ")) { return Normalize(); }
	if(CompareCommand(L"PKOL")) { return PeakOrthogonal();}
	if(CompareCommand(L"WPLT")) { return WaterPathLeft(); }
	if(CompareCommand(L"WPRT")) { return WaterPathRight(); }
	if(CompareCommand(L"LTOR")) { return LeftToRight(); }
	if(CompareCommand(L"RTOL")) { return RightToLeft(); }
	if(CompareCommand(L"GNDT")) { return GetNormalizeData(); }
	if(CompareCommand(L"GNST")) { return GetNormalizeSettings(); }
	if(CompareCommand(L"GPST")) { return GetPeakOrthogonalSettings(); }
	if(CompareCommand(L"GWLS")) { return GetWaterPathLeftSettings(); }
	if(CompareCommand(L"GWRS")) { return GetWaterPathRightSettings(); }
	return HandleUnknownMessage();
	return mlist;
}

std::list<CRMessage*> CRMessageHandler::Normalize(){
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SendMessage(REMOTE_HANDLER,REMOTE_NORMALIZE);
	std::list<CRMessage*> mlist;
	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"STMS");
	sendMess->SetData(CString(L"USLScanner: normalizing..."));

	mlist.push_back(sendMess);
//	mlist.splice(mlist.end(),GetNormalizeData());
	return mlist;
}

std::list<CRMessage*> CRMessageHandler::PeakOrthogonal(){
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SendMessage(REMOTE_HANDLER,REMOTE_PEAKOPPOSITE);
	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"STMS");
	sendMess->SetData(CString("USLScanner: peak orthogonal..."));
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);

//	mlist.splice(mlist.end(), GetNormalizeData());

	return mlist;
}

std::list<CRMessage*> CRMessageHandler::WaterPathLeft(){
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SendMessage(REMOTE_HANDLER,REMOTE_WATERPATHLEFT);
	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"STMS");
	sendMess->SetData(CString(L"USLScanner: water path left..."));
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;
}

std::list<CRMessage*> CRMessageHandler::WaterPathRight(){
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SendMessage(REMOTE_HANDLER,REMOTE_WATERPATHRIGHT);
	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"STMS");
	sendMess->SetData(CString(L"USLScanner: water path right..."));
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;
}

std::list<CRMessage*> CRMessageHandler::LeftToRight(){
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SendMessage(REMOTE_HANDLER,REMOTE_MOVETORIGHT);
	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"STMS");
	sendMess->SetData(CString(L"USLScanner: moving left probe to right..."));
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;
}

std::list<CRMessage*> CRMessageHandler::RightToLeft(){
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SendMessage(REMOTE_HANDLER,REMOTE_MOVETOLEFT);
	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"STMS");
	sendMess->SetData(CString(L"USLScanner: moving right probe to left..."));
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;
}

CPolyCoord CRMessageHandler::GenTestPolyCoord(){
	CPolyCoord pc;
	float fCoords=100;
	pc.AllocateCoords((int)fCoords);
	float fHalfwidth =(fCoords /2.0f);
	float fGradient = 127/fHalfwidth;
	for(float i=0; i<fCoords; i++){
		CCoord cord;
		cord.Zero();
		if(i<fHalfwidth){
			cord.nAmp = (int)(i*fGradient);
		} else {
			cord.nAmp = 127 - (int)(fGradient *(i-fHalfwidth));
		}
		pc.Add(cord);
	}
	return pc;
}

std::list<CRMessage*> CRMessageHandler::GetNormalizeData(){
	std::list<CRMessage*> mlist;
	

	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SendMessage(REMOTE_HANDLER,REMOTE_GETNORMALIZEDATA);
	
	return mlist;
}

std::list<CRMessage*> CRMessageHandler::GetSettings(UINT val){
	std::list<CRMessage*> mlist;
	

	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;	
	pFrame->PostMessage(REMOTE_HANDLER,val);
	return mlist;
}

std::list<CRMessage*> CRMessageHandler::GetNormalizeSettings(){
	return GetSettings(REMOTE_GETNORMALIZESETTINGS);
}

std::list<CRMessage*> CRMessageHandler::GetPeakOrthogonalSettings(){
	return GetSettings(REMOTE_GETPEAKORTHOGONALSETTINGS);
}

std::list<CRMessage*> CRMessageHandler::GetWaterPathLeftSettings(){
	return GetSettings(REMOTE_GETWATERPATHLEFTSETTINGS);
}

std::list<CRMessage*> CRMessageHandler::GetWaterPathRightSettings(){
	return GetSettings(REMOTE_GETWATERPATHRIGHTSETTINGS);
}

bool CRMessageHandler::IsNormalizePageOpen(){
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(pFrame){
		if(pFrame->m_pMotionToolsSheet)
			if(pFrame->m_pMotionToolsSheet->m_pNormalizePage) {
				if(pFrame->m_pMotionToolsSheet->m_pNormalizePage->GetSafeHwnd()){
					return true;
			}
		}
	}
	return false;
}


bool CRMessageHandler::Is3DViewEditDlgOpen(){
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(pFrame){
		if(pFrame->m_pProfileSheet){
			if(pFrame->m_pProfileSheet->m_p3DViewEditPage) {
			//	if(pFrame->m_pProfileSheet->m_p3DViewEditPage->GetSafeHwnd()){
			if(pFrame->m_pProfileSheet->m_p3DViewEditPage != NULL){
					return true;
				}
			}
		}
	}
	return false;
}

/*
bool CRMessageHandler::Is3DViewEditDlgOpen(){
	CMainFrame*  pFrame =  (CMainFrame*)AfxGetApp()->m_pMainWnd;
	if(pFrame){
		if(pFrame->m_pProfileSheet){
			if(pFrame->m_pProfileSheet->m_p3DViewEditPage) {
			//	if(pFrame->m_pProfileSheet->m_p3DViewEditPage->GetSafeHwnd()){
			if(pFrame->m_pProfileSheet->m_p3DViewEditPage != NULL){
					return true;
				}
			}
		}
	}
	return false;
}
*/



//-------------------------------------------------------------------------



//=========================================================================
// Timeslot messages
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

std::list<CRMessage*> CRMessageHandler::HandleTimeslotMessage(){
	if(CompareCommand(L"GNMS")) { return GetNames(); } //Return the names of the timeslots.
	return HandleUnknownMessage();
	std::list<CRMessage*> mlist;//Empty list
	return  mlist;
}

std::list<CRMessage*> CRMessageHandler::GetNames(){
	std::list<CRMessage*> mlist;//Empty list
	CRMessage* msg = new CRMessage();
	msg->SetCommand(L"UNMS");
	msg->SetDestination(m_dest);
	
	int nSlots = theApp.m_UtUser.m_Global.Mux.nMaxTimeslots;
	if(nSlots>0) msg->SetData(CString(theApp.m_UtUser.m_TS[0].cName));
	for(int nTS=1;nTS<nSlots;nTS++) {
		msg->AppendData(CString(L","));
		msg->AppendData(CString(theApp.m_UtUser.m_TS[nTS].cName));
	}
	mlist.push_back(msg);	
	return  mlist;
}

std::list<CRMessage*> CRMessageHandler::HandleUnknownMessage(){
	std::list<CRMessage*> mlist;//Empty list
	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(L"REMT");
	//For debug purposes,
	//sendMess->SetCommand(L"WARN");  //Instruct the remote app to close the connection.
	//sendMess->SetData(CString(L"Warning: server received unknown message from remote app."));
	sendMess->SetCommand(L"NULL");  //Instruct the remote app to close the connection.
	mlist.push_back(sendMess);
	return  mlist;
}


//======================================================================
// Profile messages
//----------------------------------------------------------------------


std::list<CRMessage*> CRMessageHandler::HandleProfileMessage(){
	std::list<CRMessage*> mlist;//Empty list
	if(!Is3DViewEditDlgOpen()) {//Try to get the page and return if unsuccessful;
			CRMessage* sendMess = new CRMessage();
			sendMess->SetDestination(m_dest);
			sendMess->SetCommand(L"WARN");
			sendMess->SetData(CString(L"Warning from USLScanner: \"Profiles->Edit->Cartesian\" tab not open. Action will have no effect."));
			mlist.push_back(sendMess);
			return mlist;
		}
		
	if(CompareCommand(L"ACOR")) { return AddCoord(); } //Return the names of the timeslots.
	if(CompareCommand(L"DCOR")) { return DeleteCoord(); } //Return the names of the timeslots.
	if(CompareCommand(L"NEWL")) { return NewLine(); } //Return the names of the timeslots.
	return HandleUnknownMessage();
	return  mlist;
}

std::list<CRMessage*> CRMessageHandler::AddCoord(){
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SendMessage(REMOTE_HANDLER_PROFILE,REMOTE_ADD_COORD);
	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"STMS");
	sendMess->SetData(CString(L"USLScanner: added coordinate."));
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;
}



std::list<CRMessage*> CRMessageHandler::DeleteCoord(){
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SendMessage(REMOTE_HANDLER_PROFILE,REMOTE_DELETE_COORD);
	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"STMS");
	sendMess->SetData(CString(L"USLScanner: deleted coordinate."));
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;
}

std::list<CRMessage*> CRMessageHandler::NewLine(){
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SendMessage(REMOTE_HANDLER_PROFILE,REMOTE_NEW_LINE);
	CRMessage* sendMess = new CRMessage();
	sendMess->SetDestination(m_dest);
	sendMess->SetCommand(L"STMS");
	sendMess->SetData(CString(L"USLScanner: new line."));
	std::list<CRMessage*> mlist;
	mlist.push_back(sendMess);
	return mlist;
}
