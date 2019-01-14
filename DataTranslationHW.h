// DataTranslationHW.h: interface for the CDataTranslationHW class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATATRANSLATIONHW_H__76E64365_31B5_4046_8E4F_4ADBA899D574__INCLUDED_)
#define AFX_DATATRANSLATIONHW_H__76E64365_31B5_4046_8E4F_4ADBA899D574__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include ".\DataTranslation\Include\olerrors.h"
#include ".\DataTranslation\Include\oldaapi.h"
#include ".\DataTranslation\Include\oldadefs.h"

#define ARRAY_SIZE 100
#define NUM_ADCS	4

class CDataTranslationHW  
{
public:
	void ReadAllADC();
	CHRTimer	m_HRTimer;
	int	GetLastContinuousValue(int *pnValue,int nLength);
	void StartADCContinuousMode();
	void SetADCSingleValueMode();
	void StopADCConversion();
	int GetADCValue(int nChannel, int nGain);
	bool IsPresent();
	void CheckError(ECODE status);
	bool m_bBoardEnabled;
	HBUF  m_hBuffer;
	HDEV m_hDrvr; /* driver handle */
	HDASS m_hDass; /* subsystem handle */
	ECODE m_status; /* board error status */
	char m_name[MAX_BOARD_NAME_LENGTH]; /* string for board name */
	char m_entry[MAX_BOARD_NAME_LENGTH]; /* string for board name */
	int	 m_nExtADCArray[8];

	void Close();
	bool Start();
	CDataTranslationHW();
	virtual ~CDataTranslationHW();

};

#endif // !defined(AFX_DATATRANSLATIONHW_H__76E64365_31B5_4046_8E4F_4ADBA899D574__INCLUDED_)
