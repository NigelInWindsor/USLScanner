#ifndef __PLUG_PLAY_H
#define __PLUG_PLAY_H

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
 *      PlugPlay.h
 *
 * Description:
 *
 *      Plug 'n' Play handler functions
 *
 * Revision History:
 *
 *      02-01-07 : PLX SDK v5.00
 *
 ******************************************************************************/


#include "PlxTypes.h"



/**********************************************
*               Functions
**********************************************/
NTSTATUS
Dispatch_Pnp(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    );

NTSTATUS
DefaultPnpHandler(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    );

NTSTATUS
HandleQueryRemoveDevice(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    );

NTSTATUS
HandleCancelRemoveDevice(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    );

NTSTATUS
HandleRemoveDevice(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    );

NTSTATUS
HandleStartDevice(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    );

NTSTATUS
HandleStopDevice(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    );

NTSTATUS
HandleQueryDeviceRelations(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    );

NTSTATUS
OnRequestComplete(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp,
    PKEVENT        pKEvent
    );

NTSTATUS
ForwardAndWait(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    );



#endif
