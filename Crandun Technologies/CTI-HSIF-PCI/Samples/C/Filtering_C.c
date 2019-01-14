/*
   C-Language example for the
   Crandun Technologies CTI-HSIF-PCI software library.
   Copyright (c) 2006, Crandun Technologies Inc.

	This example demonstrates how to use the library's filtering
	functions to exclude samples that are not of interest.
	In this particular example, filtering is done based on the sample's
	range (distance).  However, samples may also be filtered based
	on numerous other criteria. (Please see the Programmers Guide.)
*/

#include "CTI_HSIF_PCI.h"	/* required header */
#include <stdio.h>
#include <windows.h>	/* for Sleep function */


#define MAXSAMPLES 5000

/* A helper macro for checking return codes */
#define CHK_RETURN_VALUE(retCode, fnName) \
if (retCode < 0) { \
		printf("ERROR: " #fnName " returned error code: %ld\n", rc); \
		{	char errMsg[300]; if (getIsError(sensorHandle)) { \
				getErrorMessage(sensorHandle, errMsg, sizeof(errMsg)); \
				printf("Library err msg is: %s\n", errMsg); \
			} \
		} \
		setCommClosed(sensorHandle); \
		printf("Please enter any character to exit: "); \
		scanf("%c", &c); \
		return (-1); \
}


int main()
{
	char c;
	long rc;
	HSIF_DATA_PT rangeData[MAXSAMPLES];

	long sensorHandle;

	printf("Crandun Technologies CTI-HSIF-PCI Library data filtering example.\n");
	sensorHandle = -1;
	sensorHandle = getNewCTIHSIF_PCI();

	if (sensorHandle < 0) {
		printf("ERROR: getNewCTIHSIF_PCI returned error code %ld\n", sensorHandle);
		printf("Please enter any character to exit: ");
		scanf("%c\n", &c);
		return -1;
	}

	/* Open library driver */
	rc = setDriverOpen(sensorHandle);
	if (rc != CTI_SUCCESS) {
		printf("ERROR: setDriverOpen returned error %d\n", rc);
		printf("Please enter any character to exit: ");
		scanf("%c\n", &c);
		return -1;
	}

	/* Set calibration data file to use. MUST match the actual sensor used */
	rc = setCalibrationFile(sensorHandle, "C:\\lookuphs");
	if (rc != CTI_SUCCESS) {
		printf("ERROR: setCalibrationFile returned error %d\n", rc);
		printf("Please enter any character to exit: ");
		scanf("%c\n", &c);
		return -1;
	}

	/* Open communications to the serial port and high-speed interface */
	printf("Opening the serial port...\n");
	rc = setCommOpen(sensorHandle, "COM1", 9600);
	if (rc != CTI_SUCCESS) {
		printf("ERROR: setCommOpen returned error %d\n", rc);
		printf("Please enter any character to exit: ");
		scanf("%c\n", &c);
		return -1;
	}
	printf("Successfully opened the serial port!\n");


	printf("Setting maximum valid range to 4 feet.\n");
	rc = setMaxValidRange(sensorHandle, 48.0);
	CHK_RETURN_VALUE(rc, setMaxValidRange);

	printf("Setting minimum valid range to 2 feet.\n");
	rc = setMinValidRange(sensorHandle, 24.0);
	CHK_RETURN_VALUE(rc, setMinValidRange);

	printf("Telling library to discard invalid samples.\n");
	setDiscardInvalidOn(sensorHandle);	// always returns CTI_SUCCESS, so don't need to check return code

	printf("Setting sensor sample rate.\n\n");
	rc = setSamplesPerSec(sensorHandle, 500);
	CHK_RETURN_VALUE(rc, setSamplesPerSec);

	printf("Approximately 10 seconds of data will now be captured.\n");
	printf("Please move the sensor slowly, so that it reflects off a wide range of\n");
	printf("targets, from less than 2 feet in distance to more than 4 feet in distance.\n\n");

	printf("Please enter any character to continue: ");
	scanf("%c\n", &c);

	setClearBuffer(sensorHandle);	 // always returns success

	printf("Sleeping for 10 seconds - please slowly move the sensor.\n");
	Sleep(10000);

	/* Sensor sample rate is 500 samples/second, so the
	   number of possible samples over 10 seconds is about 5000.
	   However, some should be filtered by the library, so we
	   should get somewhat less than 5000 returned. */

	rc = getSamples(sensorHandle, rangeData, MAXSAMPLES, 1, 12000);
	CHK_RETURN_VALUE(rc, getSamples);
	setCommClosed(sensorHandle);

	printf("Read %d range samples from the sensor.\n", rc);

	printf("Please enter any character to exit: ");
	scanf("%c\n", &c);
	return 0;
}
