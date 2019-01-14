//
// Filtering example for the Crandun Technologies CTI-HSIF
// software library.
// Copyright (c) 2006, Crandun Technologies Inc.
//
// This example demonstrates how to use the library's filtering
// functions to exclude samples that are not of interest.
// In this particular example, filtering is done based on the sample's
// range (distance).  However, samples may also be filtered based
// on numerous other criteria. (Please see the Programmers Guide.)
//
#include "CTI_HSIF_PCI.h"	// required header
#include <iostream>		// needed for cout, cerr, cin
#include <windows.h>	// needed for Sleep function

using namespace std;		// we use cout, cerr and cin
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
	const int MAXSAMPLES = 5000;
	HSIF_DATA_PT rangeData[MAXSAMPLES];

	CTI_HSIF_PCI my_Sensor;

	cout << "Crandun Technologies CTI-HSIF-PCI Library data filtering example." << endl << endl;


	// Open Library driver
	rc = my_Sensor.setDriverOpen();
	if (rc != CTI_SUCCESS) {
		cerr << "ERROR: setDriverOpen returned error: " << rc << endl;
		cerr << "Enter any character to exit: ";
		cin >> c;
		return -1;
	}

	// Set calibration data file to use. MUST match the actual sensor used
	rc = my_Sensor.setCalibrationFile("C:\\lookuphs");
	if (rc != CTI_SUCCESS) {
		cerr << "ERROR: setCalibrationFile returned error: " << rc << endl;
		cerr << "Enter any character to exit: ";
		cin >> c;
		return -1;
	}

	// Open communications to the serial port and high-speed interface
	cout << "Opening the serial port..." << endl;
	rc = my_Sensor.setCommOpen("COM1", 9600);
	if (rc != CTI_SUCCESS) {
		cerr << "ERROR: setCommOpen returned error: " << rc << endl;
		cerr << "Enter any character to exit: ";
		cin >> c;
		return -1;
	}
	cout << "Successfully opened the serial port!" << endl;

	cout << "Setting maximum valid range to 4 feet." << endl;
	rc = my_Sensor.setMaxValidRange(48.0);
	CHK_RETURN_VALUE(rc, setMaxValidRange);

	cout << "Setting minimum valid range to 2 feet." << endl;
	rc = my_Sensor.setMinValidRange(24.0);
	CHK_RETURN_VALUE(rc, setMinValidRange);

	cout << "Telling library to discard invalid samples." << endl << endl;
	my_Sensor.setDiscardInvalidOn();	// always returns CTI_SUCCESS, so don't need to check return code


	cout << "Setting sensor sample rate." << endl << endl;
	rc = my_Sensor.setSamplesPerSec(500);
	CHK_RETURN_VALUE(rc, setSamplesPerSec);


	cout << "Approximately 10 seconds of data will now be captured." << endl;
	cout << "Please move the sensor slowly, so that it reflects off a wide range of" << endl;
	cout << "targets, from less than 2 feet in distance to more than 4 feet in distance." << endl;
	cout << endl;
	cout << "Please enter any character to continue: ";
	cin >> c;

	my_Sensor.setClearBuffer();		// purge anything currently in buffer (always returns success)

	cout << "Sleeping for 10 seconds - please slowly move the sensor." << endl;
	Sleep(10000);

	// Sensor sample rate is at 500 samples/second, so the
	// number of possible samples over 10 seconds is about 5000.
	// However, some should be filtered by the library, so we
	// should get somewhat less than 5000 returned.

	rc = my_Sensor.getSamples(rangeData, MAXSAMPLES, 1, 12000);
	CHK_RETURN_VALUE(rc, getSamples);

	my_Sensor.setCommClosed();

	cout << "Read " << rc << " range samples from the sensor." << endl;

	cout << "Please enter any character to exit: ";
	cin >> c;
	return 0;
}