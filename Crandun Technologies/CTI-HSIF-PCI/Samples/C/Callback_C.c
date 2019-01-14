/*
 Crandun Technologies CTI-HSIF-PCI Software Library Callback Example
 Copyright (c) 2006, Crandun Technologies Inc.

 This sample program demonstrates how to use a callback function to
 retrieve sample data from the library, and write that data to a disk file

 Note that this program must be compiled and linked with the
 Visual C++ Multi-Threaded libraries, since the callback is called
 in a different thread from the main program.
*/

#include "CTI_HSIF_PCI.h"	/* required header */
#include <stdio.h>
#include <windows.h>	/* needed for Sleep function */


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


/* File handle is global, so that both the callback and main have access. */
FILE * my_DataFile;

/* Declare the callback function that will be called by the library */
long myCallback(const HSIF_DATA_PT * pD1, const long N1,
				const HSIF_DATA_PT * pD2, const long N2);

int main()
{
	const char * outFileName = "C:\\CallbackTest.out";
	char c;
	long rc;
	long sensorHandle;

	printf("Crandun Technologies CTI-HSIF-PCI Library Callback example.\n");

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

	printf("Opening output data file %s\n", outFileName);
	my_DataFile = fopen(outFileName, "w");
	if (my_DataFile == NULL) {
		printf("Could not open the output data file %s Aborting!\n", outFileName);
		setCommClosed(sensorHandle);
		printf("Please enter any character to exit: ");
		scanf("%c\n", &c);
		return -1;
	}

	/* Set sample rate */
	rc = setSamplesPerSec(sensorHandle, 500);
	CHK_RETURN_VALUE(rc, setSamplePerSec);

	printf("Testing callback function.\n");

	/* set the function "myCallback" as the callback function */
	rc = setCallbackFunction(sensorHandle, myCallback);
	CHK_RETURN_VALUE(rc, setCallbackFunction);

	/* Tell library to call the callback when 250 samples are available
	   Sensor is at 500 samples/sec so this should result in the callback
	   being called approx 6 times during the 3 second period that the main 
	   thread is sleeping */
	rc = setCallbackThreshold(sensorHandle, 250);
	CHK_RETURN_VALUE(rc, setCallbackThreshold);

	setClearBuffer(sensorHandle);	 /* purge anything currently in buffer (always returns success) */

	/* sleep .*/
	printf("Main program is sleeping for 3 seconds...\n");
	Sleep(3000);

	printf("Main program finished sleeping - closing the serial port.\n");

	/* Close the sensor serial port. This also ensures that callback is done  */
	rc = setCommClosed(sensorHandle);
	CHK_RETURN_VALUE(rc, setCommClosed);

	rc = setReleaseHandle(sensorHandle);
	CHK_RETURN_VALUE(rc, setReleaseHandle);

	fflush(my_DataFile);	/* ensure that all data is written */
	fclose(my_DataFile);	/* close the data file */

	printf("Please enter any character to exit: ");
	scanf("%c", &c);

	return 0;
}



/* This is the callback function that will be called by the library */
long myCallback(const HSIF_DATA_PT * pD1, const long N1,
				const HSIF_DATA_PT * pD2, const long N2)
{
	long i,j;

	printf("In callback, reading %d samples. Writing to file\n", N1+N2);

	for (i = 0, j = 0; i < N1; i++, j++, pD1++) {
		fprintf(my_DataFile, "%8.3f\n", pD1->R_X );
	}

	/* read the samples from the second data pointer, if any */
	for (i = 0; i < N2; i++, j++, pD2++) {
		fprintf(my_DataFile, "%8.3f\n", pD2->R_X );
	}

	/* return non-zero to tell lib to remove samples from its buffer */
	return 1;
}


