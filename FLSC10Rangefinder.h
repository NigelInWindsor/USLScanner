// FLSC10Rangefinder.h: interface for the CFLSC10Rangefinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLSC10RANGEFINDER_H__ADF97B8A_5A03_43BF_B7EF_AD91B51B00D6__INCLUDED_)
#define AFX_FLSC10RANGEFINDER_H__ADF97B8A_5A03_43BF_B7EF_AD91B51B00D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LaserRangefinder.h"

class CFLSC10Rangefinder : public CLaserRangefinder
{
public:
	CFLSC10Rangefinder();
	virtual ~CFLSC10Rangefinder();

	virtual void Measure();
	virtual void MeasureCont();
	virtual void StopClear();

private:
	virtual void HandleMessage(CString buff);
	virtual void OnMeasurementReceived(CString meas);
	virtual void OnErrorReceived(CString sErr);

};

#endif // !defined(AFX_FLSC10RANGEFINDER_H__ADF97B8A_5A03_43BF_B7EF_AD91B51B00D6__INCLUDED_)
