// EgismosRangefinder.h: interface for the CEgismosRangefinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EGISMOSRANGEFINDER_H__E1BCE553_CC43_4DE2_9D5A_7DB02CFCDC3E__INCLUDED_)
#define AFX_EGISMOSRANGEFINDER_H__E1BCE553_CC43_4DE2_9D5A_7DB02CFCDC3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SerialEgismos.h"
#include "EgismosBluetoothClient.h"
#include "LaserRangefinder.h"

class CEgismosRangefinder  : public CLaserRangefinder
{
public:
	CEgismosRangefinder();
	virtual ~CEgismosRangefinder();

	void SetSlaveAddress();
	void ReadSoftwareVersion();
	void ReadDeviceType();
	void ReadSlaveAddress();
	void LaserOn();
	void LaserOff();
	void BuzzerOn();
	void BuzzerOff();
	void ReadDeviceStatus();

	virtual void Measure();
	virtual void MeasureCont();
	virtual void StopClear();


private:
	virtual void HandleMessage(CString mess);
	virtual void OnMeasurementReceived(CString meas);
	virtual void OnErrorReceived(CString sErr);

	virtual void TestEndOfLineByte(BYTE val);
	virtual void ResetEndOfLineByte(BYTE val);

	virtual bool IStringReceived();

	bool m_bEnd;
};



#endif // !defined(AFX_EGISMOSRANGEFINDER_H__E1BCE553_CC43_4DE2_9D5A_7DB02CFCDC3E__INCLUDED_)
