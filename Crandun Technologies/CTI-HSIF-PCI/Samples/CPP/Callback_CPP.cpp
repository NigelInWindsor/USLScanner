//
// Callback example for the Crandun Technologies CTI-HSIF
// software library.
// Copyright (c) 2006, Crandun Technologies Inc.
//
// This sample program demonstrates how to use a callback function
// to retrieve sample data from the library, and write that data
// to a disk file
//
// Note that this program must be compiled and linked with the
// Visual C++ Multi-Threaded libraries, since the callback is called
// in a different thread from the main program.
//

#include "CTI_HSIF_PCI.h"	// required header
#include <iostream>		// needed for cout, cerr, cin
#include <fstream>		// needed for data file functions
#include <windows.h>	// needed for Sleep function

using namespace std;		// we use cout and cerr
using namespace Crandun;	// all library symbols are in this namespace

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


// The data file and the Sensor instance must be global,
// so that both the callback and main have access.
ofstream my_DataFile;
CTI_HSIF_PCI my_Sensor;

// This is the callback function that will be called by the library
long myCallback(const HSIF_DATA_PT * pD1, const long N1, const HSIF_DATA_PT * pD2, const long N2)
{
	long i,j;

	cout << "In callback, reading " << (N1+N2) << " samples. Writing to file." << endl;

	// read the samples from the first data pointer
	for (i = 0, j = 0; i < N1; i++, j++, pD1++) {
		my_DataFile << pD1->R_X << endl;
	}

	// read the samples from the second data pointer, if any
	for (i = 0; i < N2; i++, j++, pD2++) {
		my_DataFile << pD2->R_X << endl;
	}

	// return non-zero to tell lib to remove samples from its buffer
	return 1;
}


int main()
{
	const char * outFileName = "C:\\CallbackTest.out";
	char c;
	long rc;

	cout << "Crandun Technologies CTI-HSIF-PCI Library Callback example." << endl;


	// Open library driver
	rc = my_Sensor.setDriverOpen();
	if (rc != CTI_SUCCESS) {
		cerr << "ERROR: setDriverOpen returned error: " << rc << endl;
		cerr << "Enter any character to exit: ";
		cin >> c;
		return -1;
	}

	// Set calibration data file to use. MUST match the actual sensor used
	rc = my_Sensor.setCalibrationFile("C:\\Acuity\\calibration\\lookuphs");
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

	cout << "Opening output data file " << outFileName << endl;
	my_DataFile.open(outFileName);
	if (!my_DataFile.is_open()) {
		cerr << "Could not open the output data file " << outFileName << ". Aborting!" << endl;
		my_Sensor.setCommClosed();
		cerr << "Please enter any character to exit: ";
		cin >> c;
		return -1;
	}

	cout << "Testing callback function." << endl;

	// set sample rate of 500 samples/second
	cout << "Setting sample rate." << endl;
	rc = my_Sensor.setSamplesPerSec(5000);
	CHK_RETURN_VALUE(rc, setSamplesPerSec);

	// set the function "myCallback" as the callback function
	cout << "Setting callback function." << endl;
	rc = my_Sensor.setCallbackFunction(myCallback);
	CHK_RETURN_VALUE(rc, setCallbackFunction);

	// Tell library to call the callback when 250 samples are available
	// Sensor is at 500 samples/sec so this should result in the callback
	// being called approx 6 times during the 3 second period that the main
	// thread is sleeping

	rc = my_Sensor.setCallbackThreshold(250);
	CHK_RETURN_VALUE(rc, setCallbackThreshold);

	my_Sensor.setClearBuffer();		// purge anything currently in buffer (always returns success)

	// sleep
	cout << "Main program is sleeping for 3 seconds..." << endl;
	Sleep(30000);

	cout << "Main program finished sleeping - closing the serial port." << endl;
	// close the sensor serial port - this also ensures that the callback is done
	rc = my_Sensor.setCommClosed();
	CHK_RETURN_VALUE(rc, setCommClosed);

	my_DataFile.flush();	// ensure that all data is written
	my_DataFile.close();	// close the data file

	cout << "Please enter any character to exit: ";
	cin >> c;

	return 0;
}