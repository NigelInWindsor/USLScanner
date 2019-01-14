//
// "Hello World" example for the Crandun Technologies CTI-HSIF-PCI software library.
// Copyright (c) 2006, Crandun Technologies Inc.
//
#include <iostream>
#include "CTI_HSIF_PCI.h"	// required header file

using namespace std;
using namespace Crandun;	// all CTI library symbols are in this namespace

int main()
{
	char c;
	char errMsg[300];
	long rc;
	const int maxSamples = 20;
	HSIF_DATA_PT rangeData[maxSamples];
	
	CTI_HSIF_PCI my_Sensor;

	cout << "Crandun Technologies CTI-HSIF-PCI Library 'Hello World' example." << endl;
 
	// Open driver
	rc = my_Sensor.setDriverOpen();
	if (rc != CTI_SUCCESS) {
		cerr << "ERROR: setDriverOpen returned error: " << rc << endl;
		cerr << "Enter any character to exit: ";
		cin >> c;
		return -1;
	}


	// Set calibration data file to use. MUST match the particular HSIF card 
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
		my_Sensor.setCommClosed();
		cerr << "ERROR: setCommOpen returned error: " << rc << endl;
		if (my_Sensor.getIsError()) { 
			my_Sensor.getErrorMessage(errMsg, sizeof(errMsg)); 
			cerr << "Library error msg is: " << errMsg << endl;
		} 
		cerr << "Enter any character to exit: ";
		cin >> c;
		return -1;
	}

	// Get samples from the sensor
	cout << "Reading samples from sensor ..." << endl;
	rc = my_Sensor.getSamples(rangeData, maxSamples, 10, 1000);
	if (rc < 0) {
		cerr << "ERROR: getSamples returned error: " << rc << endl;
		if (my_Sensor.getIsError()) { 
			my_Sensor.getErrorMessage(errMsg, sizeof(errMsg)); 
			cerr << "Library error msg is: " << errMsg << endl;
		} 
		my_Sensor.setCommClosed();
		cerr << "Enter any character to exit: ";
		cin >> c;
		return -1;
	}
	my_Sensor.setCommClosed();


	cout << "Read " << rc << " range samples from the sensor." << endl;
	for (int i= 0; i < rc; i++)
		cout << "Range " << i << " is " << rangeData[i].R_X << endl;

	cout << "Enter any character to exit: ";
	cin >> c;
	return 0;
}

