/*******************************************************************************
 * Copyright (c) PLX Technology, Inc.
 *
 * PLX Technology Inc. licenses this source file under the GNU Lesser General Public
 * License (LGPL) version 2.  This source file may be modified or redistributed
 * under the terms of the LGPL and without express permission from PLX Technology.
 *
 * PLX Technology, Inc. provides this software AS IS, WITHOUT ANY WARRANTY,
 * EXPRESS OR IMPLIED, INCLUDING, WITHOUT LIMITATION, ANY WARRANTY OF
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  PLX makes no guarantee
 * or representations regarding the use of, or the results of the use of,
 * the software and documentation in terms of correctness, accuracy,
 * reliability, currentness, or otherwise; and you rely on the software,
 * documentation and results solely at your own risk.
 *
 * IN NO EVENT SHALL PLX BE LIABLE FOR ANY LOSS OF USE, LOSS OF BUSINESS,
 * LOSS OF PROFITS, INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES
 * OF ANY KIND.
 *
 ******************************************************************************/

/******************************************************************************
 *
 * File Name:
 *
 *      PlxInterrupt.c
 *
 * Description:
 *
 *      This file handles interrupts for the PLX device
 *
 * Revision History:
 *
 *      12-01-07 : PLX SDK v5.20
 *
 ******************************************************************************/


#include "PlxChipFn.h"
#include "PlxInterrupt.h"
#include "SuppFunc.h"
#include "Si10Ioctl.h"
#include "DebugPrint.h"
#include "..\Si6Driver\sys\Si6IOctl.h"
#include "..\USLDriver\sys\USLIoctl.h"

#define USLMulDiv(a, b, c) ((a*b)/c)

unsigned long ReadDWord( DEVICE_EXTENSION *pdx, unsigned long offset);
unsigned long g_ClkSrc;
unsigned long g_CounterValue[10];
int g_nAccelLoop[10];


BOOLEAN Si10Isr( DEVICE_EXTENSION *pdx);
void Si10Moveto( DEVICE_EXTENSION *dx,int nWhich);
void Si10Joystick( DEVICE_EXTENSION *dx,int nWhich);
void Si10ContinuousRotate( DEVICE_EXTENSION *dx,int nWhich);
void Si10JoystickSerial( DEVICE_EXTENSION *dx,int nWhich);
void Si10JoystickADC( DEVICE_EXTENSION *dx,int nWhich);
void Si10Hometo(DEVICE_EXTENSION *dx,int nWhich);
void Si10PulseMotorUpdateEncoder(DEVICE_EXTENSION *dx,struct AxisData *pAxis);
void Si10CounterStart(DEVICE_EXTENSION *pdx,int number, unsigned long value);
BOOLEAN Si10CollectData( DEVICE_EXTENSION *pdx);
int  CheckLimitSwitches( DEVICE_EXTENSION *pdx);



/******************************************************************************
 *
 * Function   :  OnInterrupt
 *
 * Description:  The Interrupt Service Routine for the PLX device
 *
 ******************************************************************************/
BOOLEAN
OnInterrupt(
    PKINTERRUPT pInterrupt,
    PVOID       ServiceContext
    )
{
    U32               RegPciInt;
    U32               InterruptSource;
	U32				  offset;
	U32				  result;
    DEVICE_EXTENSION *pdx;
	U8				 Result[4];
	unsigned char OldIrql;


	KeRaiseIrql(31,&OldIrql);


    // Get the device extension
    pdx = (DEVICE_EXTENSION *)ServiceContext;


    // Read interrupt status register
    RegPciInt =
        PLX_9000_REG_READ(
            pdx,
            PCI9030_INT_CTRL_STAT
            );

    /****************************************************
     * If the chip is in a low power state, then local
     * register reads are disabled and will always return
     * 0xFFFFFFFF.  If the PLX chip's interrupt is shared
     * with another PCI device, the PXL ISR may continue
     * to be called.  This case is handled to avoid
     * erroneous reporting of an active interrupt.
     ***************************************************/
    if (RegPciInt == 0xFFFFFFFF)
        return FALSE;

    // Check for master PCI interrupt enable
    if ((RegPciInt & (1 << 6)) == 0)
        return FALSE;

    // Verify that an interrupt is truly active

    // Clear the interrupt type flag
    InterruptSource = INTR_TYPE_NONE;

    // Check if Local Interrupt 1 is active and not masked
    if ((RegPciInt & (1 << 2)) && (RegPciInt & (1 << 0)))
    {
        InterruptSource |= INTR_TYPE_LOCAL_1;
    }

    // Check if Local Interrupt 2 is active and not masked
    if ((RegPciInt & (1 << 5)) && (RegPciInt & (1 << 3)))
    {
        InterruptSource |= INTR_TYPE_LOCAL_2;
    }

    // Software Interrupt
    if (RegPciInt & (1 << 7))
    {
        InterruptSource |= INTR_TYPE_SOFTWARE;
    }

    // Return if no interrupts are active
    if (InterruptSource == INTR_TYPE_NONE)
        return FALSE;


	if (pdx->bUserShared == TRUE)
	{
		unsigned char OldIrql;
		KeRaiseIrql(31,&OldIrql);
		Si10Isr(pdx);
		KeLowerIrql(OldIrql);
	}


    // Mask the PCI Interrupt
    PLX_9000_REG_WRITE(
        pdx,
        PCI9030_INT_CTRL_STAT,
        RegPciInt & ~(1 << 6)
        );

    //
    // Schedule deferred procedure (DPC) to complete interrupt processing
    //

    KeInsertQueueDpc(
        &(pdx->DpcForIsr),
        (VOID *)(PLX_UINT_PTR)InterruptSource,
        NULL
        );


	KeLowerIrql(OldIrql);

    return TRUE;
}




/******************************************************************************
 *
 * Function   :  DpcForIsr
 *
 * Description:  This routine will be triggered by the ISR to service an interrupt
 *
 ******************************************************************************/
VOID
DpcForIsr(
    PKDPC pDpc,
    PVOID pContext,
    PVOID pArg1,
    PVOID pArg2
    )
{
    U32                 RegValue;
    PLX_REG_DATA        RegData;
    DEVICE_EXTENSION   *pdx;
    PLX_INTERRUPT_DATA  IntData;
//	PHANDLE_OBJECT		hop;


    // Get the device extension
    pdx = (DEVICE_EXTENSION *)pContext;


    // Get interrupt source
    IntData.Source_Ints     = PtrToUlong(pArg1);
    IntData.Source_Doorbell = 0;

    KeAcquireSpinLockAtDpcLevel(
        &(pdx->Lock_HwAccess)
        );


    // Local Interrupt 1
    if (IntData.Source_Ints & INTR_TYPE_LOCAL_1)
    {
        // Setup to synchronize access to Interrupt Control/Status Register
        RegData.pdx         = pdx;
        RegData.offset      = PCI9030_INT_CTRL_STAT;
        RegData.BitsToSet   = 0;
        RegData.BitsToClear = 0;

        // Check if this is an edge-triggered interrupt
        RegValue =
            PLX_9000_REG_READ(
                pdx,
                PCI9030_INT_CTRL_STAT
                );

        if ((RegValue & (1 << 1)) && (RegValue & (1 << 8)))
        {
            // Clear edge-triggered interrupt
            RegData.BitsToSet = (1 << 10);

            KeSynchronizeExecution(
                pdx->pInterruptObject,
                PlxSynchronizedRegisterModify,
                (VOID *)&RegData
                );
        }
        else
        {
            // Mask Local Interrupt 1
            //RegData.BitsToClear = (1 << 0);

            //KeSynchronizeExecution(
            //   pdx->pInterruptObject,
            //    PlxSynchronizedRegisterModify,
            //    (VOID *)&RegData
            //    );
        }
    }

    // Local Interrupt 2
    if (IntData.Source_Ints & INTR_TYPE_LOCAL_2)
    {
        // Setup to synchronize access to Interrupt Control/Status Register
        RegData.pdx         = pdx;
        RegData.offset      = PCI9030_INT_CTRL_STAT;
        RegData.BitsToSet   = 0;
        RegData.BitsToClear = 0;

        // Check if this is an edge-triggered interrupt
        RegValue =
            PLX_9000_REG_READ(
                pdx,
                PCI9030_INT_CTRL_STAT
                );

        if ((RegValue & (1 << 4)) && (RegValue & (1 << 9)))
        {
            // Clear edge-triggered interrupt
            RegData.BitsToSet = (1 << 11);

            KeSynchronizeExecution(
                pdx->pInterruptObject,
                PlxSynchronizedRegisterModify,
                (VOID *)&RegData
                );
        }
        else
        {
            // Mask Local Interrupt 2
            RegData.BitsToClear = (1 << 3);

            KeSynchronizeExecution(
                pdx->pInterruptObject,
                PlxSynchronizedRegisterModify,
                (VOID *)&RegData
                );
        }
    }

    // Software Interrupt
    if (IntData.Source_Ints & INTR_TYPE_SOFTWARE)
    {
        // Setup to synchronize access to Interrupt Control/Status Register
        RegData.pdx       = pdx;
        RegData.offset    = PCI9030_INT_CTRL_STAT;
        RegData.BitsToSet = 0;

        // Clear the software interrupt
        RegData.BitsToClear = (1 << 7);

        KeSynchronizeExecution(
            pdx->pInterruptObject,
            PlxSynchronizedRegisterModify,
            (VOID *)&RegData
            );
    }


    KeReleaseSpinLockFromDpcLevel(
        &(pdx->Lock_HwAccess)
        );

    // Signal any objects waiting for notification
    PlxSignalNotifications(
        pdx,
        &IntData
        );

	if(pdx->bUserShared == TRUE){
		if ((pdx->pIntEvent) && ( pdx->pSi10User->bGenerateEvent == TRUE)) {
			KeSetEvent(pdx->pIntEvent, 0, FALSE);
		}
		pdx->pSi10User->bGenerateEvent = FALSE;
	}


    // Re-enable interrupts
    KeSynchronizeExecution(
        pdx->pInterruptObject,
		PlxChipInterruptsEnable,
		pdx
		);

}




unsigned long ReadDWord( DEVICE_EXTENSION *pdx, unsigned long offset)
{

	U8					BarIndex = 2;
	U32					result;
    U32					*pBuffer = &result;
    U32					ByteCount = 4;
    PLX_ACCESS_TYPE		AccessType = (PLX_ACCESS_TYPE) BitSize32;
    BOOLEAN				bRemap = TRUE;
    BOOLEAN				bReadOperation = TRUE;

    U16          Offset_RegRemap;
    U32          RegValue;
    U32          SpaceRange;
    U32          SpaceOffset;
    U32          RemapOriginal;
    U32          BytesToTransfer;
    PLX_UINT_PTR SpaceVa;



    // Added to prevent compiler warnings
    RemapOriginal = 0;


    // Get offset of remap register
    PlxChipGetRemapOffset(
        pdx,
        BarIndex,
        &Offset_RegRemap
        );

    if (Offset_RegRemap == (U16)-1)
    {
        return -1;
    }

    // Only memory spaces are supported by this function
    if (pdx->PciBar[BarIndex].Properties.bIoSpace)
    {

        return -1;
    }

    // Get kernel virtual address for the space
    SpaceVa = (PLX_UINT_PTR)pdx->PciBar[BarIndex].pVa;

    if (SpaceVa == 0)
    {
        return -1;
    }

    // Save the remap register
    if (bRemap)
    {
        RemapOriginal =
            PLX_9000_REG_READ(
                pdx,
                Offset_RegRemap
                );
    }
    else
    {
        // Make sure requested area doesn't exceed our local space window boundary
        if ((offset + ByteCount) > (U32)pdx->PciBar[BarIndex].Properties.Size)
        {

            return -1;
        }
    }

    // Get the range of the space
    SpaceRange = ~((U32)pdx->PciBar[BarIndex].Properties.Size - 1);

    // Transfer data in blocks
    while (ByteCount != 0)
    {
        // Adjust remap if necessary
        if (bRemap)
        {
            // Clear upper bits of remap
            RegValue = RemapOriginal & ~SpaceRange;

            // Adjust window to local address
            RegValue |= offset & SpaceRange;

            PLX_9000_REG_WRITE(
                pdx,
                Offset_RegRemap,
                RegValue
                );
        }

        // Get current offset into space
        SpaceOffset = offset & (~SpaceRange);

        // Calculate bytes to transfer for next block
        if (ByteCount <= (((~SpaceRange) + 1) - SpaceOffset))
        {
            BytesToTransfer = ByteCount;
        }
        else
        {
            BytesToTransfer = ((~SpaceRange) + 1) - SpaceOffset;
        }

        if (bReadOperation)
        {

			  DEV_MEM_TO_USER_32(
				pBuffer,
				(SpaceVa + SpaceOffset),
				BytesToTransfer
				);

        }
        else
        {

			   USER_TO_DEV_MEM_32(
                (SpaceVa + SpaceOffset),
                pBuffer,
                BytesToTransfer
                );

        }

        // Adjust for next block access
        pBuffer   += BytesToTransfer;
        offset    += BytesToTransfer;
        ByteCount -= BytesToTransfer;
    }

    // Restore the remap register
    if (bRemap)
    {
        PLX_9000_REG_WRITE(
            pdx,
            Offset_RegRemap,
            RemapOriginal
            );
    }

    return result;

}



BOOLEAN Si10Isr( DEVICE_EXTENSION *pdx)
{
	int nIrq;
	int counter;
	BOOLEAN flag;
	int nIrqReg;
	int nMask = 1;
	int	nWhichAxis;

	pdx->pSi10User->nIrqCount++;


	nIrqReg = ReadDWord(pdx,0x01AC); //Clear IRQ line (1,2,4,8,16,32....
	pdx->pSi10User->nGeneralInput=ReadDWord(pdx,SI10_GENERAL_INPUT);


	for(nIrq=0,nMask=1;nIrq<11;nIrq++,nMask<<=1) {

		for(nWhichAxis=0;nWhichAxis<10;nWhichAxis++) {
			if(pdx->pAxis[nWhichAxis] != NULL) {
				if((pdx->pAxis[nWhichAxis]->nIrqMask == (nMask & nIrqReg)) && (pdx->pAxis[nWhichAxis]->bAvailable==TRUE)) {

					if((nIrq)<10) {

//						if(pdx->pAxis[nWhichAxis]) pdx->pAxis[nWhichAxis]->nIrqCount++;

						switch(pdx->pSi10User->nIrqAction[nWhichAxis]) {
						case MOVETO: Si10Moveto(pdx,nWhichAxis);
							break;
						case HOMETO: Si10Hometo(pdx,nWhichAxis);
							break;
						case JOYSTICK: Si10Joystick(pdx,nWhichAxis);
							break;
						case CONTINUOUS_ROTATE: Si10ContinuousRotate(pdx,nWhichAxis);
							break;
						};
						flag=FALSE;
					};
				}
			}
		}

		if((nIrq==10) && (nMask & nIrqReg)) { 
			pdx->pSi10User->nIrqPRFCount++;	
		}
	}

	if(nIrqReg & 0x1fe000) {
		pdx->pSi10User->nGPIOIrqMask = nIrqReg >> 13;
		pdx->pSi10User->nGPIOIrqCount++;
		pdx->pSi10User->bGPIOEvent = TRUE;
		pdx->pSi10User->bGenerateEvent = TRUE;
	}


	return flag;

}

void Si10Moveto(DEVICE_EXTENSION *dx,int nWhich)
{


	struct	AxisData *pAxis = dx->pAxis[nWhich];
	int	nAcceleration;
	__int64 nTemp;
	static int nLoopCounter=1;

	if(dx->pAxis[nWhich]==NULL)
		return;


	(pAxis->bDirectionReverse)?(dx->pSi10User->nDirEor|=pAxis->nBit):(dx->pSi10User->nDirEor&=(~pAxis->nBit));
	WriteDWord(dx,SI10_DIR_PORT,dx->pSi10User->nDirReg ^ dx->pSi10User->nDirEor);


	Si10PulseMotorUpdateEncoder(dx,pAxis);


	if((pAxis->nStepsPerRev) && (dx->pSi10User->nIrqAction[pAxis->nDriverAxisNumber]==MOVETO)){
		if(pAxis->nRevCounter==pAxis->nDesireRevCount){
			nTemp = (__int64)pAxis->nPos - (__int64)pAxis->nDesirePos;
		}else if(pAxis->nDesireRevCount>pAxis->nRevCounter){
			nTemp = (__int64)pAxis->nPos - (pAxis->nStepsPerRev*2); //get us going around until on correct rev
		}else{
			nTemp = (__int64)pAxis->nPos + (pAxis->nStepsPerRev*2); //get us going around until on correct rev
		}
	}else{
		nTemp = (__int64)pAxis->nPos - (__int64)pAxis->nDesirePos;
	}

	if(abs(nTemp)<=pAxis->nPositionError){
		dx->pSi10User->nProcessState &= (~pAxis->nIrqMask);
		goto finish_interrupt;
	};

	dx->pSi10User->nProcessState |= pAxis->nIrqMask;

	if((nTemp > 0) && !(dx->pSi10User->nDirReg & pAxis->nBit)) {
		pAxis->nCurrentSpeed = pAxis->nMinSpeed;
		dx->pSi10User->nDirReg |= pAxis->nBit;
		goto direction_set;
	};
	if((nTemp < 0) && (dx->pSi10User->nDirReg & pAxis->nBit)) {
		pAxis->nCurrentSpeed = pAxis->nMinSpeed;
		dx->pSi10User->nDirReg &= (~pAxis->nBit);
	};
	direction_set:

	nTemp = abs(nTemp);
	if(pAxis->nDeceleration==0) pAxis->nDeceleration=1;

	if(nTemp > ((pAxis->nCurrentSpeed-pAxis->nMinSpeed)/pAxis->nDeceleration)) {
		nLoopCounter--;
		nAcceleration=0;
		if((pAxis->nCurrentSpeed>5000) && (nLoopCounter<=0)) {
			nAcceleration = 1;
			nLoopCounter=(pAxis->nCurrentSpeed-5000)/50;
		} else {
			nAcceleration = pAxis->nAcceleration;
			nLoopCounter=1;
		}
		pAxis->nCurrentSpeed += nAcceleration;

		if( (pAxis->nReqSpeed>0)&&(pAxis->nReqSpeed<pAxis->nMaxSpeed) ){
			if(pAxis->nCurrentSpeed>pAxis->nReqSpeed)
				pAxis->nCurrentSpeed=pAxis->nReqSpeed;
		}else if(pAxis->nCurrentSpeed > pAxis->nMaxSpeed) {
			pAxis->nCurrentSpeed = pAxis->nMaxSpeed;
		};

	} else {
		pAxis->nCurrentSpeed -= pAxis->nDeceleration;
		if(pAxis->nCurrentSpeed < pAxis->nMinSpeed) {
			pAxis->nCurrentSpeed = pAxis->nMinSpeed;
		};
	};

	finish_interrupt:


//	if((nWhich == dx->pSi10User->nWhichAxis) && (dx->pSi10User->bCollectData == TRUE)) {
//		dx->pSi10User->bGenerateEvent = TRUE;
//	}
	if(dx->pSi10User->bCollectData == TRUE) {
		dx->pSi10User->bGenerateEvent = TRUE;
	}


//	if(dx->bUTShared == TRUE) {
//		if( (dx->pUtUser->bCollectData) )
//			Si10CollectData(dx);
//	}

	if(pAxis->nCurrentSpeed>9999) pAxis->nCurrentSpeed = 9999;
	Si10CounterStart(dx,pAxis->nCounter,nTable[pAxis->nCurrentSpeed]);
}

void Si10Joystick( DEVICE_EXTENSION *dx,int nWhich)
{

	switch(dx->pSi10User->nJoystickSource) {
	default: Si10JoystickADC(dx,nWhich);
		break;
	case WIRED_SERIAL:
	case WIRELESS_SERIAL: Si10JoystickSerial(dx,nWhich);
		break;
	}
}

void Si10JoystickSerial( DEVICE_EXTENSION *dx,int nWhich)
{

	struct	AxisData *pAxis = dx->pAxis[nWhich];

	int	nSideTemp,nTemp,nMinTemp,nMaxTemp,nLimitSwitch;
	unsigned long nJoyValues;

	if(dx->pAxis[nWhich]==NULL)
		return;
	dx->pAxis[nWhich]->nIrqCount++;

	pAxis->nJoystickNumber %= 5;

	(pAxis->bDirectionReverse) ? (dx->pSi10User->nDirEor|=pAxis->nBit) : (dx->pSi10User->nDirEor&=(~pAxis->nBit));
	WriteDWord(dx,SI10_DIR_PORT,dx->pSi10User->nDirReg ^ dx->pSi10User->nDirEor);

	Si10PulseMotorUpdateEncoder(dx,pAxis);

	if(!(dx->pSi10User->nJoystickSwitchValue & dx->pSi10User->nJoystickScannerFixtureBit) ||
		(!(dx->pSi10User->nJoystickSwitchValue & dx->pSi10User->nJoystickDeadManBit0) && !(dx->pSi10User->nJoystickSwitchValue & dx->pSi10User->nJoystickDeadManBit1))) {

		pAxis->nJoystickZero=dx->pSi10User->nJoystickValue[pAxis->nJoystickNumber];

	};

	nMinTemp=USLMulDiv(pAxis->nJoystickMinSpeed,dx->pSi10User->nJoystickPercent,100);
	nMaxTemp=USLMulDiv(pAxis->nJoystickMaxSpeed,dx->pSi10User->nJoystickPercent,100);

	pAxis->nJoystickValueRead=dx->pSi10User->nJoystickValue[pAxis->nJoystickNumber];

	nTemp=pAxis->nJoystickValueRead-pAxis->nJoystickZero;
	if(nTemp>=0) {
		if(pAxis->nJoystickMaxValue-pAxis->nJoystickZero)
			nTemp=USLMulDiv(100,nTemp,(pAxis->nJoystickMaxValue-pAxis->nJoystickZero));
	} else {
		if(pAxis->nJoystickZero - pAxis->nJoystickMinValue)
			nTemp=USLMulDiv(100,nTemp,(pAxis->nJoystickZero-pAxis->nJoystickMinValue));
	}


	switch(dx->pSi10User->nJoystickSource) {
	case WIRED_MSC:
	case WIRED_SI6:
	case WIRED_SERIAL:
	case WIRED_SI10:
		nSideTemp=( dx->pSi10User->nJoystickValue[4] );
		switch(pAxis->nJoystickSwitchPos) {
		case 0: if(nSideTemp>30) nTemp=0;
			break;
		case 1: if((nSideTemp < 30) || (nSideTemp > 226)) nTemp = 0;
			break;
		case 2: if(nSideTemp < 226) nTemp=0;
			break;
		case 3:
			break;
		}
		break;
	case WIRELESS_SERIAL:
		switch(pAxis->nJoystickSwitchPos) {
		case 0: if(dx->pSi10User->nJoystickSwitchValue & dx->pSi10User->nJoystickLeftSideBit) nTemp=0;
			break;
		case 1: if((dx->pSi10User->nJoystickSwitchValue & dx->pSi10User->nJoystickLeftSideBit) || (dx->pSi10User->nJoystickSwitchValue & dx->pSi10User->nJoystickRightSideBit)) nTemp = 0;
			break;
		case 2: if(dx->pSi10User->nJoystickSwitchValue & dx->pSi10User->nJoystickRightSideBit) nTemp=0;
			break;
		case 3:
			break;
		}
		break;
	}


	if(!(dx->pSi10User->nJoystickSwitchValue &dx->pSi10User->nJoystickScannerFixtureBit)) nTemp=0;
	if(pAxis->bJoystickAvailable==0) nTemp=0;
	if(pAxis->bJoystickReverse) nTemp *= -1;

	if(abs(nTemp) > 150) {
		pAxis->nCurrentSpeed=nTemp;
		dx->pSi10User->nProcessState &= (~pAxis->nIrqMask);
		goto finish_interrupt;
	};


	if((abs(nTemp)<dx->pSi10User->nJoystickDeadBand)&&(pAxis->nCurrentSpeed<=nMinTemp)) {
		pAxis->nCurrentSpeed = nMinTemp;
		dx->pSi10User->nProcessState &= (~pAxis->nIrqMask);
		goto finish_interrupt;
	};
	if((nTemp>0) && (pAxis->nCurrentSpeed <= nMinTemp)) {
		dx->pSi10User->nDirReg |= pAxis->nBit;
	};
	if((nTemp<0) && (pAxis->nCurrentSpeed <= nMinTemp)) {
		dx->pSi10User->nDirReg &= (~pAxis->nBit);
	};

	if(((nTemp>0) && !(dx->pSi10User->nDirReg & pAxis->nBit)) || ((nTemp<0) && (dx->pSi10User->nDirReg & pAxis->nBit))) {
		pAxis->nReqSpeed = nMinTemp;
	} else {
		nTemp = abs(nTemp)-dx->pSi10User->nJoystickDeadBand;
		pAxis->nReqSpeed = USLMulDiv((nMaxTemp-nMinTemp),nTemp,100)+nMinTemp;
		if(pAxis->nReqSpeed>nMaxTemp)
			pAxis->nReqSpeed = nMaxTemp;
	};
	pAxis->nReqSpeed=USLMulDiv(pAxis->nReqSpeed,dx->pSi10User->nJoystickPercent,100);
	if(g_nAccelLoop[pAxis->nCounter]<=0) {
		if(pAxis->nCurrentSpeed < pAxis->nReqSpeed) {
			pAxis->nCurrentSpeed += pAxis->nAcceleration;
			if(pAxis->nCurrentSpeed > pAxis->nReqSpeed) pAxis->nCurrentSpeed = pAxis->nReqSpeed;
		};
		g_nAccelLoop[pAxis->nCounter] = pAxis->nCurrentSpeed / 400;
	} else {
		g_nAccelLoop[pAxis->nCounter]--;
	}
	if(g_nAccelLoop[pAxis->nCounter] < 0) g_nAccelLoop[pAxis->nCounter] = 0;
	if(g_nAccelLoop[pAxis->nCounter] > 30) g_nAccelLoop[pAxis->nCounter] = 30;

	if(pAxis->nCurrentSpeed > pAxis->nReqSpeed) {
		pAxis->nCurrentSpeed -= pAxis->nDeceleration;
		if(pAxis->nCurrentSpeed < pAxis->nReqSpeed) pAxis->nCurrentSpeed = pAxis->nReqSpeed;
	};

	dx->pSi10User->nProcessState |= pAxis->nIrqMask;

	//CHECK LIMIT SWITCHES
	nLimitSwitch=CheckLimitSwitches(dx);
	if((nLimitSwitch&pAxis->nMaxSwitch)&&!(dx->pSi10User->nDirReg&pAxis->nBit)){
		dx->pSi10User->nProcessState &= (~pAxis->nIrqMask);
	};
	if((nLimitSwitch&pAxis->nMinSwitch)&&(dx->pSi10User->nDirReg&pAxis->nBit)){
		dx->pSi10User->nProcessState &= (~pAxis->nIrqMask);
	};

	//CHECK FOR OVER TRAVEL
/*	if((pAxis->nMinTravel!=pAxis->nMaxTravel)&&(dx->pSi6User->nProcessState&pAxis->nIrqMask)) {
		if((pAxis->nPos>=pAxis->nMaxTravel)&&!(dx->pSi6User->nDirReg&pAxis->nBit))
															dx->pSi6User->nProcessState &=(~pAxis->nIrqMask);
		if((pAxis->nPos<=pAxis->nMinTravel)&&(dx->pSi6User->nDirReg&pAxis->nBit))
														dx->pSi6User->nProcessState &=(~pAxis->nIrqMask);
		(dx->pSi6User->nDirReg&pAxis->nBit)?(nTemp=pAxis->nPos-pAxis->nMinTravel):
										(nTemp=pAxis->nMaxTravel-pAxis->nPos);
		if(pAxis->nDeceleration==0) pAxis->nDeceleration=1;

		if(nTemp>5000l) nTemp=5000l;
		if(nTemp<((pAxis->nCurrentSpeed-pAxis->nMinSpeed)/pAxis->nDeceleration)) {
			nTemp = (nTemp*pAxis->nDeceleration)+pAxis->nMinSpeed;
			if(nTemp<pAxis->nCurrentSpeed) pAxis->nCurrentSpeed = nTemp;
			if(pAxis->nCurrentSpeed<nMinTemp) pAxis->nCurrentSpeed=nMinTemp;
			if(pAxis->nCurrentSpeed>nMaxTemp) pAxis->nCurrentSpeed=nMaxTemp;
		};
	};
*/ //TODO

finish_interrupt:
	if(pAxis->nCurrentSpeed<0) pAxis->nCurrentSpeed=0;
	if(pAxis->nCurrentSpeed>9999) pAxis->nCurrentSpeed=9999;

	Si10CounterStart(dx,pAxis->nCounter,nTable[pAxis->nCurrentSpeed]);

}


void Si10JoystickADC( DEVICE_EXTENSION *dx,int nWhich)
{

	struct	AxisData *pAxis = dx->pAxis[nWhich];

	int	nSideTemp,nTemp,nMinTemp,nMaxTemp,nLimitSwitch;
	unsigned long nJoyValues;

	if(dx->pAxis[nWhich]==NULL)
		return;
	dx->pAxis[nWhich]->nIrqCount++;


	if((pAxis->nJoystickMaxValue-pAxis->nJoystickZero)==0) pAxis->nJoystickMaxValue=pAxis->nJoystickZero+35;
	if((pAxis->nJoystickMinValue-pAxis->nJoystickZero)==0) pAxis->nJoystickMinValue=pAxis->nJoystickZero-35;
	if(dx->pSi10User->nJoystickPercent<=0) dx->pSi10User->nJoystickPercent=100;

	if(nWhich==0)
		dx->pSi10User->nJoystickPercent=100;

	(pAxis->bDirectionReverse)?
	(dx->pSi10User->nDirEor|=pAxis->nBit):(dx->pSi10User->nDirEor&=(~pAxis->nBit));
	WriteDWord(dx,SI10_DIR_PORT,dx->pSi10User->nDirReg ^ dx->pSi10User->nDirEor);


	Si10PulseMotorUpdateEncoder(dx,pAxis);

	nMinTemp=USLMulDiv(pAxis->nJoystickMinSpeed,dx->pSi10User->nJoystickPercent,100);
	nMaxTemp=USLMulDiv(pAxis->nJoystickMaxSpeed,dx->pSi10User->nJoystickPercent,100);

	nJoyValues = ReadDWord( dx, SI10_JOYSTICK);
	nTemp = ( (nJoyValues>>(pAxis->nJoystickNumber*8)) & 0x000000FF );
	pAxis->nJoystickValueRead=nTemp;

	nSideTemp=( (nJoyValues>>24) & 0x000000FF );

	nTemp=pAxis->nJoystickValueRead-pAxis->nJoystickZero;

	if(nTemp>=0) {
		nTemp=USLMulDiv(100,nTemp,(pAxis->nJoystickMaxValue-pAxis->nJoystickZero));
	} else {
		nTemp=USLMulDiv(100,nTemp,(pAxis->nJoystickZero-pAxis->nJoystickMinValue));
	}

	switch(pAxis->nJoystickSwitchPos) {
	case 0: if(nSideTemp>30) nTemp=0;
		break;
	case 1: if((nSideTemp < 30) || (nSideTemp > 226)) nTemp = 0;
		break;
	case 2: if(nSideTemp < 226) nTemp=0;
		break;
	case 3:
		break;
	}


	if(pAxis->bJoystickAvailable==0) nTemp=0;
	if(pAxis->bJoystickReverse) nTemp *= -1;

	if(abs(nTemp) > 150) {
		pAxis->nCurrentSpeed=nTemp;
		dx->pSi10User->nProcessState &= (~pAxis->nIrqMask);
		goto finish_interrupt;
	};


	if((abs(nTemp)<dx->pSi10User->nJoystickDeadBand)&&(pAxis->nCurrentSpeed<=nMinTemp)) {
		pAxis->nCurrentSpeed = nMinTemp;
		dx->pSi10User->nProcessState &= (~pAxis->nIrqMask);
		goto finish_interrupt;
	};
	if((nTemp>0) && (pAxis->nCurrentSpeed <= nMinTemp)) {
		dx->pSi10User->nDirReg |= pAxis->nBit;
	};
	if((nTemp<0) && (pAxis->nCurrentSpeed <= nMinTemp)) {
		dx->pSi10User->nDirReg &= (~pAxis->nBit);
	};

	if(((nTemp>0) && !(dx->pSi10User->nDirReg & pAxis->nBit)) || ((nTemp<0) && (dx->pSi10User->nDirReg & pAxis->nBit))) {
		pAxis->nReqSpeed = nMinTemp;
	} else {
		nTemp = abs(nTemp)-dx->pSi10User->nJoystickDeadBand;
		pAxis->nReqSpeed = USLMulDiv((nMaxTemp-nMinTemp),nTemp,100)+nMinTemp;
		if(pAxis->nReqSpeed>nMaxTemp)
			pAxis->nReqSpeed = nMaxTemp;
	};
	pAxis->nReqSpeed=USLMulDiv(pAxis->nReqSpeed,dx->pSi10User->nJoystickPercent,100);
	if(pAxis->nCurrentSpeed < pAxis->nReqSpeed) {
		pAxis->nCurrentSpeed += pAxis->nAcceleration;
		if(pAxis->nCurrentSpeed > pAxis->nReqSpeed) pAxis->nCurrentSpeed = pAxis->nReqSpeed;
	};
	if(pAxis->nCurrentSpeed > pAxis->nReqSpeed) {
		pAxis->nCurrentSpeed -= pAxis->nDeceleration;
		if(pAxis->nCurrentSpeed < pAxis->nReqSpeed) pAxis->nCurrentSpeed = pAxis->nReqSpeed;
	};

	dx->pSi10User->nProcessState |= pAxis->nIrqMask;

	//CHECK LIMIT SWITCHES
	nLimitSwitch=CheckLimitSwitches(dx);
	if((nLimitSwitch&pAxis->nMaxSwitch)&&!(dx->pSi10User->nDirReg&pAxis->nBit)){
		dx->pSi10User->nProcessState &= (~pAxis->nIrqMask);
	};
	if((nLimitSwitch&pAxis->nMinSwitch)&&(dx->pSi10User->nDirReg&pAxis->nBit)){
		dx->pSi10User->nProcessState &= (~pAxis->nIrqMask);
	};

	//CHECK FOR OVER TRAVEL
/*	if((pAxis->nMinTravel!=pAxis->nMaxTravel)&&(dx->pSi6User->nProcessState&pAxis->nIrqMask)) {
		if((pAxis->nPos>=pAxis->nMaxTravel)&&!(dx->pSi6User->nDirReg&pAxis->nBit))
															dx->pSi6User->nProcessState &=(~pAxis->nIrqMask);
		if((pAxis->nPos<=pAxis->nMinTravel)&&(dx->pSi6User->nDirReg&pAxis->nBit))
														dx->pSi6User->nProcessState &=(~pAxis->nIrqMask);
		(dx->pSi6User->nDirReg&pAxis->nBit)?(nTemp=pAxis->nPos-pAxis->nMinTravel):
										(nTemp=pAxis->nMaxTravel-pAxis->nPos);
		if(pAxis->nDeceleration==0) pAxis->nDeceleration=1;

		if(nTemp>5000l) nTemp=5000l;
		if(nTemp<((pAxis->nCurrentSpeed-pAxis->nMinSpeed)/pAxis->nDeceleration)) {
			nTemp = (nTemp*pAxis->nDeceleration)+pAxis->nMinSpeed;
			if(nTemp<pAxis->nCurrentSpeed) pAxis->nCurrentSpeed = nTemp;
			if(pAxis->nCurrentSpeed<nMinTemp) pAxis->nCurrentSpeed=nMinTemp;
			if(pAxis->nCurrentSpeed>nMaxTemp) pAxis->nCurrentSpeed=nMaxTemp;
		};
	};
*/ //TODO

finish_interrupt:
if(pAxis->nCurrentSpeed>9999) pAxis->nCurrentSpeed = 9999;
Si10CounterStart(dx,pAxis->nCounter,nTable[pAxis->nCurrentSpeed]);

}



void Si10PulseMotorUpdateEncoder(DEVICE_EXTENSION *dx,struct AxisData *pAxis)
{
	int nPos,nLimitSwitch;
	static int nLastRevPos[10];

	if(pAxis==NULL)
		return;

	//static int nLastSwitchState,nLastRevPos;
	//int	nLimitSwitch;


	nLimitSwitch=CheckLimitSwitches(dx);
	nLimitSwitch & pAxis->nMinSwitch ?	(pAxis->nFlagMinMax |= MIN_SWITCH) : (pAxis->nFlagMinMax &= ~MIN_SWITCH);
	nLimitSwitch & pAxis->nMaxSwitch ?	(pAxis->nFlagMinMax |= MAX_SWITCH) : (pAxis->nFlagMinMax &= ~MAX_SWITCH);
	nLimitSwitch & pAxis->nHomeSwitch ?	(pAxis->nFlagMinMax |= HOME_SWITCH) : (pAxis->nFlagMinMax &= ~HOME_SWITCH);

	nPos = pAxis->nPos + pAxis->nOffset;
	if(dx->pSi10User->nProcessState & pAxis->nIrqMask) {
//		WriteDWord(dx,SI10_CLK_PORT,(unsigned long)pAxis->nBit);
		if(!(g_ClkSrc & (unsigned long)pAxis->nBit)) {
			g_ClkSrc |= (unsigned long)pAxis->nBit;
			WriteDWord(dx,SI10_CLK_SRC,g_ClkSrc);
		}

		if(pAxis->nEncoderMode==0) {
			(dx->pSi10User->nDirReg & pAxis->nBit)?(pAxis->nPos--):(pAxis->nPos++);
			if(pAxis->nStepsPerRev) {
				pAxis->nPos%=pAxis->nStepsPerRev;
				if(pAxis->nPos<0) pAxis->nPos+=pAxis->nStepsPerRev;
			}

		};
		(dx->pSi10User->nDirReg & pAxis->nBit)?(pAxis->nCount--):(pAxis->nCount++);
	} else {
		g_ClkSrc &= ~(unsigned long)pAxis->nBit;
		WriteDWord(dx,SI10_CLK_SRC,g_ClkSrc);
	}


	if(pAxis->nEncoderMode) {
		pAxis->nEncoderValue = ReadDWord(dx,SI10_ENCODER_0 + pAxis->nEncoderNumber * 4);
		if(pAxis->bEncoderReverse==TRUE) pAxis->nEncoderValue *= -1;
		nPos=pAxis->nEncoderValue;

		if(pAxis->nEncMotorRatioDenominator!=0) {
			nPos*=pAxis->nEncMotorRatioNumerator;
			nPos/=pAxis->nEncMotorRatioDenominator;
		}

		//pAxis->nPos = nPos;
		nPos -= pAxis->nOffset;
		pAxis->nPos = (int)nPos;
	}

	if(pAxis->nStepsPerRev) {
		pAxis->nCount%=pAxis->nStepsPerRev;
		pAxis->nPos%=pAxis->nStepsPerRev;
		if(pAxis->nPos<0) pAxis->nPos+=pAxis->nStepsPerRev;
		if(pAxis->nCount<0) pAxis->nCount+=pAxis->nStepsPerRev;

		if(abs(nLastRevPos[pAxis->nCounter]-pAxis->nPos)>(pAxis->nStepsPerRev/2)) {
			(dx->pSi10User->nDirReg & pAxis->nBit)?(pAxis->nRevCounter--):(pAxis->nRevCounter++);
		}
	}

	nLastRevPos[pAxis->nCounter]=pAxis->nPos;

}


void Si10Hometo(DEVICE_EXTENSION *dx,int nWhich)
{
	struct	AxisData *pAxis = dx->pAxis[nWhich];
	int	nLimitSwitch;


	switch(pAxis->nHomeAction) {
	case 0:
		(pAxis->bDirectionReverse)?(dx->pSi10User->nDirEor|=pAxis->nBit):(dx->pSi10User->nDirEor&=(~pAxis->nBit));
		WriteDWord(dx,dx->pSi10User->nAddrs+SI10_DIR_PORT,dx->pSi10User->nDirReg ^ dx->pSi10User->nDirEor);

		Si10PulseMotorUpdateEncoder(dx,pAxis);

		nLimitSwitch=CheckLimitSwitches(dx);

		switch(pAxis->nHomeMode) {
		case 0:
			if(nLimitSwitch&pAxis->nMinSwitch) {
				pAxis->nHomeFoundAt=pAxis->nPos;
				dx->pSi10User->nProcessState &= (~pAxis->nIrqMask);
				pAxis->nHomeAction = 10;
				goto finish_interrupt;
			};
			break;
		case 1:
			if(nLimitSwitch&pAxis->nMaxSwitch) {
				pAxis->nHomeFoundAt=pAxis->nPos;
				dx->pSi10User->nProcessState &= (~pAxis->nIrqMask);
				pAxis->nHomeAction = 10;
				goto finish_interrupt;
			};
			break;
		case 2:
			if(nLimitSwitch&pAxis->nHomeSwitch) {
				pAxis->nHomeFoundAt=pAxis->nPos;
				dx->pSi10User->nProcessState &= (~pAxis->nIrqMask);
				pAxis->nHomeAction = 10;
				goto finish_interrupt;
			};
			break;
		case 3:
			if(pAxis->bHomeFlag==FALSE) {
				if(abs(pAxis->nCount)>=((abs(pAxis->nLimitSwitchOffset)*3)/2)) {
					pAxis->nHomeFoundAt=pAxis->nPos;
					pAxis->nCurrentSpeed = pAxis->nMinSpeed;
					dx->pSi10User->nProcessState &= (~pAxis->nIrqMask);
					pAxis->nHomeAction = 10;
					goto finish_interrupt;
				};
			};
			break;
		case 4:
			if(pAxis->bHomeFlag==FALSE) {
				if(abs(pAxis->nCount)>=((abs(pAxis->nStepsPerRev)*3)/2)) {
					pAxis->nHomeFoundAt=pAxis->nPos;
					pAxis->nCurrentSpeed = pAxis->nMinSpeed;
					dx->pSi10User->nProcessState &= (~pAxis->nIrqMask);
					pAxis->nHomeAction = 10;
					goto finish_interrupt;
				};
			};
			break;
		};
		if(pAxis->nCurrentSpeed < abs(pAxis->nHomeSpeed)) {
			pAxis->nCurrentSpeed += pAxis->nAcceleration;
		};
		break;
	case 10:
		if(pAxis->bUseFiducial){
			//read latch into buffer
			pAxis->nLatchContents = ReadDWord(dx,SI10_COUNTER_LATCH_0 + pAxis->nEncoderNumber * 4);
			if(pAxis->nHomeSpeed>=0)
				pAxis->nDesirePos = pAxis->nPos - 10000;
			else
				pAxis->nDesirePos = pAxis->nPos + 10000;
			pAxis->nHomeAction = 20;
		}else{
			pAxis->nDesirePos = pAxis->nHomeFoundAt + pAxis->nLimitSwitchOffset;
			pAxis->nHomeAction = 30;
		}
		break;
	case 20:
		//Start moving in search of Fiducial
		Si10Moveto(dx,nWhich);
		//Read latch if not same as buffer, we have hit fiducial
		if(pAxis->nLatchContents != ReadDWord(dx,SI10_COUNTER_LATCH_0 + pAxis->nEncoderNumber * 4))
		{
			//populate nHomeFoundAt with contents of latch
			pAxis->nHomeFoundAt = ReadDWord(dx,SI10_COUNTER_LATCH_0 + pAxis->nEncoderNumber * 4);

			if(pAxis->bEncoderReverse==TRUE) pAxis->nHomeFoundAt *= -1;

			if(pAxis->nEncMotorRatioDenominator!=0) {
				pAxis->nHomeFoundAt*=pAxis->nEncMotorRatioNumerator;
				pAxis->nHomeFoundAt/=pAxis->nEncMotorRatioDenominator;
			}

			pAxis->nHomeFoundAt-=pAxis->nOffset;

			pAxis->nDesirePos = pAxis->nHomeFoundAt + pAxis->nLimitSwitchOffset;
			pAxis->nHomeAction = 30;
		} else if (!(dx->pSi10User->nProcessState & pAxis->nIrqMask)) {
			//can't find fiducial use limit switch
			if(pAxis->nHomeSpeed>=0)
				pAxis->nDesirePos = pAxis->nPos - 10000;
			else
				pAxis->nDesirePos = pAxis->nPos + 10000;
		}
		break;
	case 30: //Moveto Offset
		Si10Moveto(dx,nWhich);
		if(!(dx->pSi10User->nProcessState & pAxis->nIrqMask)) { //in Pos
			if(pAxis->nEncoderMode) {
				WriteDWord(dx,SI10_ENCODER_0 + pAxis->nEncoderNumber * 4,0);
			}
			pAxis->nPos = 0;
			pAxis->nCount = 0;
			pAxis->nEncoderValue = 0;

			pAxis->nCurrentSpeed = pAxis->nMinSpeed;
			pAxis->bHomeFlag=TRUE;
			pAxis->nHomeAction = 999;
			g_ClkSrc &= ~(unsigned long)pAxis->nBit;
			WriteDWord(dx,SI10_CLK_SRC,g_ClkSrc);
		}
		break;
	case 999:
		break;
	}

	finish_interrupt:
	if(pAxis->nCurrentSpeed>9999) pAxis->nCurrentSpeed = 9999;
	Si10CounterStart(dx,pAxis->nCounter,nTable[pAxis->nCurrentSpeed]);

}


void Si10CounterStart(DEVICE_EXTENSION *pdx,int number, unsigned long value)
{

	if(g_CounterValue[number] - value) {

		switch(number) {
		case 0:
			WriteDWord(pdx,SI10_COUNT_0,value&0xffff);
			break;
		case 1:
			WriteDWord(pdx,SI10_COUNT_1,value&0xffff);
			break;
		case 2:
			WriteDWord(pdx,SI10_COUNT_2,value&0xffff);
			break;
		case 3:
			WriteDWord(pdx,SI10_COUNT_3,value&0xffff);
			break;
		case 4:
			WriteDWord(pdx,SI10_COUNT_4,value&0xffff);
			break;
		case 5:
			WriteDWord(pdx,SI10_COUNT_5,value&0xffff);
			break;
		case 6:
			WriteDWord(pdx,SI10_COUNT_6,value&0xffff);
			break;
		case 7:
			WriteDWord(pdx,SI10_COUNT_7,value&0xffff);
			break;
		case 8:
			WriteDWord(pdx,SI10_COUNT_8,value&0xffff);
			break;
		case 9:
			WriteDWord(pdx,SI10_COUNT_9,value&0xffff);
			break;
		};

		g_CounterValue[number] = value;
	}
}


BOOLEAN Si10CollectData( DEVICE_EXTENSION *pdx)
{
	//Collect Data Rountine here.
	int nn,gg,tt,pp,bb,nTS,nPtr0,nPtr1;
	int	nGStop,nGStart,nPath,nTrigPos;
	int nPos, nSample,nDir;
	int nSamplesLine, nNumberLines, nScanLine, nSize;

	nSamplesLine =  pdx->pUtUser->nSamplesLine;
	nNumberLines = pdx->pUtUser->nNumberLines;
	nScanLine = pdx->pUtUser->nScanLine;


	//Get Pos

	nPos = pdx->pAxis[0]->nPos *  pdx->pUtUser->nStepSize; //Makes assumtion 0 Axis is scan axis nPos * StepSize * 1000 (to convert to um)

	//Work out which sample in line
	nSample = ((nPos-pdx->pUtUser->nStartPos) * (nSamplesLine-1) / (pdx->pUtUser->nFinishPos - pdx->pUtUser->nStartPos));
	if(nSample<0) nSample = 0;
	if(nSample>=nSamplesLine) nSample = nSamplesLine-1;


	pdx->pSi10User->nNumberAmpGates = nSample;

	//Work out where pointer is in databuffer
	nSize = nSamplesLine * nNumberLines;
	nPtr0 = (nSamplesLine * nScanLine) + nSample;
	if(nPtr0<0) nPtr0 = 0;
	if(nPtr0>nSize) nPtr0 = nSize-1;

	nDir = 0;

	//Found out where next pointer points to +1 or -1
	if(nScanLine % 2)
		nDir = 1; //if odd line

	if(nDir==0) {
		nPtr1 = nPtr0+1;
		if(nPtr1>=nSize) nPtr1=nSize-1;
	} else {
		nPtr1 = nPtr0-1;
		if(nPtr1<0) nPtr1=0;
	}


	//Load up Amp and TimeBuffers
	gg=tt=pp=bb=0;
	for(nTS=0;nTS<1;nTS++) { //1 TS at mo
		for(nn=0;nn<8;nn++) {
			if(pdx->pTimeSlotUser[nTS].Gate.nCollectAmp[nn]>0) {
				pdx->pAmpBuffer[gg][nPtr0] = pdx->pAmpBuffer[gg][nPtr1] = pdx->pTimeSlotUser[nTS].Gate.nAmplitude[nn]+128;
				gg++;
			}
			if(pdx->pTimeSlotUser[nTS].Gate.nCollectTime[nn]>0) {
				nGStop = pdx->pTimeSlotUser[nTS].Gate.nTOFStopGate[nn];
				nGStart = pdx->pTimeSlotUser[nTS].Gate.nTOFStartGate[nn];

				nPath = (int)( (pdx->pTimeSlotUser[nTS].Gate.nTimeSample[nGStop] - pdx->pTimeSlotUser[nTS].Gate.nTimeSample[nGStart-1]) * pdx->pUtUser->nSamplePeriod );
				pdx->pTimeBuffer[tt][nPtr0] = pdx->pTimeBuffer[tt][nPtr1] = nPath;
				tt++;
			}
		}
	}

	return TRUE;
}

int  CheckLimitSwitches( DEVICE_EXTENSION *pdx)
{
	int	nLimitSwitch;

	nLimitSwitch=pdx->pSi10User->nLimitSwitch=ReadDWord(pdx,SI10_LIMIT_SWITCHES)^pdx->pSi10User->nSwitchEor;

	return nLimitSwitch;

}

void Si10ContinuousRotate( DEVICE_EXTENSION *dx,int nWhich)
{
	struct	AxisData *pAxis = dx->pAxis[nWhich];
	int i, nRotationAxis=-1, nPos;
	int intComponent, fComponent=0, fComponentDenom;
	int nScanLines;


	if((pAxis->nDesirePos == 0x7fffffff) || (pAxis->nDesirePos == 0x80000000)) {
		//True continuous roate axis. Keep on rolling
		Si10Moveto(dx,nWhich);
		return;
	}

	for(i=0;i<9;i++){
		if(dx->pAxis[i]->nDesirePos == 0x7fffffff){
			nRotationAxis=i;
			break;
		}
	}

	if(nRotationAxis ==-1)
		return; //no rotation axis


	intComponent = dx->pAxis[nRotationAxis]->nRevCounter * pAxis->nSlowIncrement; //*1000 too big

	if(dx->pAxis[nRotationAxis]->nPos>0) {
		fComponentDenom = (dx->pAxis[nRotationAxis]->nStepsPerRev*1000)/dx->pAxis[nRotationAxis]->nPos;
		if(fComponentDenom>0)
			fComponent = (pAxis->nSlowIncrement)/fComponentDenom;
	}

	nPos = (intComponent/1000) + fComponent + pAxis->nStartPos;

	if(nPos>dx->pAxis[nWhich]->nFinishPos)
		return;

	if(dx->pSi10User->bCollectData == TRUE) {
		dx->pSi10User->bGenerateEvent = TRUE;
	}

	dx->pSi10User->nProcessState |= pAxis->nIrqMask;
	dx->pSi10User->nDirReg &= (~pAxis->nBit);


	if(nPos>pAxis->nPos)
		Si10PulseMotorUpdateEncoder(dx,pAxis);

	Si10CounterStart(dx,pAxis->nCounter,nTable[pAxis->nMaxSpeed]); //get a counter going
}
