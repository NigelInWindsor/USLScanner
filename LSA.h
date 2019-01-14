// LSA.h: interface for the CLSA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LSA_H__B1D6139C_1ED2_473D_A20E_87C18AFA5394__INCLUDED_)
#define AFX_LSA_H__B1D6139C_1ED2_473D_A20E_87C18AFA5394__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <bitset>
#include <vector>
#include "Si10.h"

#undef MAX_ASCAN_LENGTH
#define	MAX_ASCAN_LENGTH	8192

struct LSAFocalLaw {
	int		nStructSize;
	int		nTXStartElement;
	int		nTXFinishElement;
	int		nRXStartElement;
	int		nRXFinishElement;
	bool	bOutput[4];  //!!!PM30 Number!!!
	float	fCollectiveGain[4]; // !!!! ACM Number!!!!!
	float	fGainTrim[4]; // !!!! ACM Number!!!!!
	int		nOutputFocalLawNumber[4]; //Each Beam is linked to a different FocalLaw Number Conatined here !!!!PM30 Number!!!
};



class CLSA  
{
public:
//functions
	CLSA();
	virtual ~CLSA();
	bool FindDevice(); //Tries to find LSA equipment based on SIF FPGA Register
	bool IsConnected();
	bool ApplyFocalLaws(int &nFocalLaws, int nElements, int nStartElement, bool bFocused, int nFocusDepth=0, int nScanPitch=1, float fArrayPitchmm=1.0);  //nScanPitch in multiples of ArrayPitch
	bool OldApplyFocalLaws(int &nFocalLaws, int nElements, int nStartElement, bool bFocused, int nFocusDepth=0, int nScanPitch=1, float fArrayPitchmm=1.0);  //nScanPitch in multiples of ArrayPitch
	void StoreAScan(int nFocalLaw, unsigned char *pArray, int nAScanLength);
	void StoreAScan(int nFocalLaw, char *pArray, int nAScanLength);
	bool OldCalculateDelays(int nDepthmm, int nElements, std::vector<int> *Delaysns, float fElementPitch);
	bool Old1CalculateDelays(int nDepthmm, int nElements, int *pDelaysns, float fElementPitch);
	bool UpdateAllControls();
	bool MaintainUniformTSs(int nTimeSlot);
	float GetCollectiveGain(int nViewLaw);
	float GetGainTrim(int nViewLaw);

	
	//Global Commands
	bool CmdReset(/*??*/);
	bool CmdSetHTVoltage(int nVoltage);
	bool CmdSetHTOnOff(bool bOn);
	bool CmdSetWriteFL(int nWriteLaw);
	bool CmdSetActiveFL(int nActiveFL);
	bool CmdSetTXPulseWidth(int nWidthNS); 
	bool CmdSetPPGain(int nPPGainDb);
	bool CmdSetFilters(float fLowPassMHz, float fHighPassMhz); //LowPass 0xFF=Wideband, HighPass 0=WideBand

	//FocalLaw Commands	
	bool CmdSetTXBeamPos(int nWriteLaw, int nStartElement, int nFinishElement);
	bool CmdSetTXDelays(int nWriteLaw, int nElement, int nDelayNS); //TODO
	bool CmdSetRXBeamPos(int nWriteLaw, int nStartElement, int nFinishElement);
	bool CmdSetFLCollectiveGain(int nWriteLaw, float fGainDb);
	bool CmdSetGainTrim(int nWriteLaw, float fGaindB);

	bool GetActualLawAndOutputFromFocalLawNumber(int nFocalLawNumber, int &nActualLaw, int &nOutput);
	int GetFocalLawNumberFromActualLawAndOutput(int nActualLaw, int nOutput);
	bool GetActualLawAndACMFromFocalLawNumber(int nFocalLawNumber, int &nActualLaw, int &nACM);
	int GetLowestFLStartElement();
	int GetHightesFLStartElement();
	float GetFLGain(int nViewLaw);
	float GetCurrentFLDistanceFromElementOne(int nFL=-1);
	bool IsOutput(int nLaw, int nOutput);
	bool GetSetFL(int nFL, struct LSAFocalLaw *FL, bool bGet);
	bool GetFL(int nFL, struct LSAFocalLaw *FL);
	bool SetFL(int nFL, struct LSAFocalLaw *FL);
	

//variables
	CSi10				*m_Si10User;
	bool				m_bDevicePresent;
	int					m_nCurrentFocalLaw;
	int					nFocalLawsL;		//0-256
	int					m_nActualLawsL;

	int					m_nScopeViewLaw;		//0-128
	
	signed char			m_AscanData[MAX_ASCAN_LENGTH * 130]; //128*8192
	int					m_nAscanLength;



	short				PCB_Revision;
	short				FPGA_Revision;
	short				FPGA_Version;
	short				FPGA_Type;
	

private:
//functions
	bool CommandTransmit(int nCode, int nData); //equivalent to CUSLut::MailboxTx. Private only used by LSA member functions.
	bool ReadMessageRegister(int &nData);

	

//variables
	int m_nLSAMailboxAddrs;   //0x100
	bool m_bFailedNotice;
	CCriticalSection	m_CriticalSection;
	struct	LSAFocalLaw m_FL[257]; //0-special, 1-256-normal




};

#endif // !defined(AFX_LSA_H__B1D6139C_1ED2_473D_A20E_87C18AFA5394__INCLUDED_)
