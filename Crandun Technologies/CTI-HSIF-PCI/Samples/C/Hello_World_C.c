/*
  "Hello World" example for the Crandun Technologies CTI-HSIF-PCI software library.
  Copyright (c) 2006, Crandun Technologies Inc.
*/

#include "CTI_HSIF_PCI.h"	/* required header */
#include <stdio.h>
#define maxSamples 20

int main()
{
	char c;
	char errMsg[300];
	long i, rc;
	HSIF_DATA_PT rangeData[maxSamples];
	long sensorHandle=-1;

	printf("Crandun Technologies CTI-HSIF-PCI Library 'Hello World' example.\n");

	sensorHandle = getNewCTIHSIF_PCI();
	if (sensorHandle < 0) {
		printf("ERROR: getNewCTIHSIF_PCI returned error code %ld\n", sensorHandle);
		printf("Please enter any character to exit: ");
		scanf("%c", &c);
		return -1;
	}

	/* Open library */
	rc = setDriverOpen(sensorHandle);
	if (rc != CTI_SUCCESS) {
		printf("ERROR: setDriverOpen returned error %d\n", rc);
		printf("Please enter any character to exit: ");
		scanf("%c", &c);
		return -1;
	}

	/* Set calibration data file to use. MUST match the actual sensor used */
	rc = setCalibrationFile(sensorHandle, "C:\\lookuphs");
	if (rc != CTI_SUCCESS) {
		printf("ERROR: setCalibrationFile returned error %d\n", rc);
		printf("Please enter any character to exit: ");
		scanf("%c", &c);
		return -1;
	}

	/* Open communications to the serial port and high-speed interface */
	printf("Opening the serial port...\n");
	rc = setCommOpen(sensorHandle, "COM1", 9600);
	if (rc != CTI_SUCCESS) {
		printf("ERROR: setCommOpen returned error %d\n", rc);
		if (getIsError(sensorHandle)) { 
			getErrorMessage(sensorHandle, errMsg, sizeof(errMsg)); 
			printf("Library err msg is: %s\n", errMsg); 
		}
		printf("Please enter any character to exit: ");
		scanf("%c", &c);
		return -1;
	}

	/* Get samples from the sensor */
	printf("Reading samples from sensor ...\n");
	rc = getSamples(sensorHandle, rangeData, maxSamples, 10, 1000);
	if (rc < 0) {
		printf("ERROR: getSamples returned error %d\n", rc);
		if (getIsError(sensorHandle)) { 
			getErrorMessage(sensorHandle, errMsg, sizeof(errMsg)); 
			printf("Library err msg is: %s\n", errMsg); 
		}
		setCommClosed(sensorHandle);
		printf("Please enter any character to exit: ");
		scanf("%c", &c);
		return -1;
	}

	setCommClosed(sensorHandle);
	setReleaseHandle(sensorHandle);

	printf("Read %d range samples from the sensor.\n", rc);
	for (i = 0; i < rc; i++)
		printf("Range %d is %8.3f inches\n", i, rangeData[i].R_X);

	printf("Please enter any character to exit: ");
	scanf("%c", &c);
	return 0;
}

