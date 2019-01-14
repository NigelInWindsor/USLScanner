/*
	 Crandun Technologies CTI-HSIF-PCI software library Motor control example
	 Copyright (c) 2006, Crandun Technologies Inc.

	 Demonstrates using the high-speed interface's motor control and encoder
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
	int i;
	
	HSIF_DATA_PT rangeData[MAXSAMPLES];
	
	long sensorHandle=0;

	printf("Crandun Technologies CTI-HSIF-PCI Library Motor control example.\n");
 
	sensorHandle = getNewCTIHSIF_PCI();
	if (sensorHandle < 0) {
		printf("ERROR: getNewCTIHSIF_PCI returned error code %ld\n", sensorHandle);
		printf("Please enter any character to exit: ");
		scanf("%c\n", &c);
		return -1;
	}

	/* Open library */
	rc = setDriverOpen(sensorHandle);
	if (rc != CTI_SUCCESS) {
		printf("ERROR: setDriverOpen returned error: %d.\n", rc);
		printf("Enter any character to exit: ");
		scanf("%c\n", &c);
		return -1;
	}

	/* Set calibration data file to use. MUST match the particular HSIF card  */
	rc = setCalibrationFile(sensorHandle, "C:\\lookuphs");
	if (rc != CTI_SUCCESS) {
		printf("ERROR: setCalibrationFile returned error: %d.\n", rc);
		printf("Enter any character to exit: ");
		scanf("%c\n", &c);
		return -1;
	}

	/* Open communications to the serial port and high-speed interface  */
	printf("Opening serial connection to sensor.\n");
	rc = setCommOpen(sensorHandle, "COM1", 9600);
	if (rc != CTI_SUCCESS) {
		printf("ERROR: setCommOpen returned error: %d.\n", rc);
		printf("Enter any character to exit: ");
		scanf("%c\n", &c);
		return -1;
	}
	printf("Successfully opened serial connection.\n");

	/* Tell library what the motor speed is at the max motor power setting */
	rc = setMotorMaxRPM(sensorHandle, 1, 2600);
	CHK_RETURN_VALUE(rc, setMotorMaxRPM);

	/* Tell library that encoder has 2000 counts per revolution */
	rc = setEncoderCountsPerRev(sensorHandle, 1, 2000);
	CHK_RETURN_VALUE(rc, setEncoderCountsPerRev);

	rc = setSamplesPerSec(sensorHandle, 500);
	CHK_RETURN_VALUE(rc, setSamplesPerSec);

	rc = setMotorRPM(sensorHandle, 1, 1000, 1);	/* set 1000 revolutions per minute, direction=1 */
	CHK_RETURN_VALUE(rc, setMotorRPM);

	printf("Sleeping to let motor speed up.\n");
	Sleep(5000);	/* sleep to let motor speed up */

	/* Get samples from the sensor */
	printf("Reading samples from sensor ...\n");
	setClearBuffer(sensorHandle);	/* clear buffer - always returns CTI_SUCCESS */

	/* Read 100 samples from sensor */
	rc = getSamples(sensorHandle, rangeData, MAXSAMPLES, 100, 10000);
	CHK_RETURN_VALUE(rc, getSamples);

	printf("Read %d range samples from the sensor.\n", rc);
	for (i = 0; i < rc; i++) {
		printf("Range %d is %8.3f inches", i, rangeData[i].R_X);
		printf(" RawR=%d",   rangeData[i].rawRange);
		printf(" RawEnc=%d", rangeData[i].rawE1);
		printf(" inputs=%d", rangeData[i].inputs);
		printf(" Angle is %8.3f\n", rangeData[i].A1_Y);
	} 

	setCommClosed(sensorHandle);
	setReleaseHandle(sensorHandle);
	printf("Enter any character to exit: ");
	scanf("%c\n", &c);
	return 0;
}

