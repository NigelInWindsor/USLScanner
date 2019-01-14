//
// Crandun Technologies CTI-HSIF-PCI software library Motor control example
// Copyright (c) 2001, 2006 Crandun Technologies Inc.
//
// Demonstrates using the high-speed interface's motor control and encoder
//
#include <iostream>
#include <windows.h>	// for Sleep() function

#include "CTI_HSIF_PCI.h"	// required header

using namespace std;
using namespace Crandun;	// all CTI library symbols are in this namespace

// A helper macro for checking return codes
#define CHK_RETURN_VALUE(retCode, fnName) \
if (retCode < 0) { \
		cerr << "ERROR: " #fnName " returned error code: " << rc << endl; \
		{	char errMsg[300]; if (my_Sensor.getIsError()) { \
				my_Sensor.getErrorMessage(errMsg, sizeof(errMsg)); \
				cerr << "Library error msg is: " << errMsg << endl; \
			} \
		}\
		my_Sensor.setCommClosed(); \
		cerr << "Please enter any character to exit: "; \
		cin >> c; \
		return (-1); \
} 


int main()
{
	char c;
	long rc;
	const int maxSamples = 5000;
	long numRead;
	HSIF_DATA_PT rangeData[maxSamples];
	
	CTI_HSIF_PCI my_Sensor;

	cout << "Crandun Technologies CTI-HSIF-PCI Library Motor control example." << endl;
 
	// Setup driver 
	rc = my_Sensor.setDriverOpen();
	if (rc != CTI_SUCCESS) {
		cerr << "ERROR: setDriverOpen returned error: " << rc << endl;
		cerr << "Enter any character to exit: ";
		cin >> c;
		return -1;
	}

	// Set calibration data file to use. MUST match the particular HSIF card 
	rc = my_Sensor.setCalibrationFile("C:\\Acuity\\Calibration\\lookuphs");
	if (rc != CTI_SUCCESS) {
		cerr << "ERROR: setCalibrationFile returned error: " << rc << endl;
		cerr << "Enter any character to exit: ";
		cin >> c;
		return -1;
	}

	// Open communications to the serial port and high-speed interface 
	cout << "Opening connection to sensor." << endl;
	rc = my_Sensor.setCommOpen("COM1", 9600);
	if (rc != CTI_SUCCESS) {
		cerr << "ERROR: setCommOpen returned error: " << rc << endl;
		cerr << "Enter any character to exit: ";
		cin >> c;
		return -1;
	}
	cout << "Successfully opened serial connection." << endl;

	// Tell library what the motor speed is at the max motor power setting
	rc = my_Sensor.setMotorMaxRPM(1, 2600);
	CHK_RETURN_VALUE(rc, setMotorMaxRPM);

	// Tell library that encoder has 2000 counts per revolution
	rc = my_Sensor.setEncoderCountsPerRev(1, 2000);
	CHK_RETURN_VALUE(rc, setEncoderCountsPerRev);

	rc = my_Sensor.setMotorRPM(1, 1000, 1);	// set 1000 revolutions per minute, direction=1
	CHK_RETURN_VALUE(rc, setMotorRPM);

	cout << "Sleeping to let motor speed up." << endl;
	Sleep(5000);	// sleep to let motor speed up

	rc = my_Sensor.setSamplesPerSec(30000);
	CHK_RETURN_VALUE(rc, setSamplesPerSec);

	// Get samples from the sensor
	cout << "Reading samples from sensor ..." << endl;
	my_Sensor.setClearBuffer();	// clear buffer - always returns CTI_SUCCESS

	// Read samples from sensor
	numRead = my_Sensor.getSamples(rangeData, maxSamples, 200, 10000);
	CHK_RETURN_VALUE(numRead, getSamples);

	my_Sensor.setCommClosed();

	cout << "Read " << numRead << " range samples from the sensor." << endl;
	for (int i = 0; i < numRead; i++) {
		cout << "Range " << i << " is " << rangeData[i].R_X;
		cout << " RawR=" << rangeData[i].rawRange;
		cout << " Raw Enc=" << rangeData[i].rawE1;
		cout << " inputs=" << rangeData[i].inputs;
		cout << " Angle is  " << rangeData[i].A1_Y << endl;
	} 

	cout << "Enter any character to exit: ";
	cin >> c;
	return 0;
}

