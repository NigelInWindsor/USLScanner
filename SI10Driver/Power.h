#ifndef __POWER_H
#define __POWER_H

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
 *      Power.h
 *
 * Description:
 *
 *      Power Management functions
 *
 * Revision History:
 *
 *      06-01-08 : PLX SDK v6.00
 *
 ******************************************************************************/


#include "DrvDefs.h"



/**********************************************
*               Functions
**********************************************/
NTSTATUS
Dispatch_Power(
    PDEVICE_OBJECT fdo,
    PIRP           Irp
    );

NTSTATUS 
Power_ForwardIrpToNextDriver(
    PDEVICE_OBJECT fdo,
    PIRP           Irp
    );

NTSTATUS 
Power_HandleIrpQueryPower(
    PDEVICE_OBJECT fdo,
    PIRP           Irp
    );

NTSTATUS 
Power_HandleIrpSetPower(
    PDEVICE_OBJECT fdo,
    PIRP           Irp
    );

NTSTATUS
Power_CompleteSystemSetPowerIrp(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp,
    PVOID          Context
    );

VOID
Power_PowerChangeComplete(
    PDEVICE_OBJECT   pDeviceObject,
    UCHAR            MinorFunction,
    POWER_STATE      PowerState,
    PVOID            Context,
    PIO_STATUS_BLOCK pIoStatus
    );

NTSTATUS
Power_CompleteDevicePowerUp(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp,
    PVOID          Context
    );



#endif
