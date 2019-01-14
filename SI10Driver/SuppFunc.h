#ifndef __SUPPORT_FUNC_H
#define __SUPPORT_FUNC_H

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
 *      SuppFunc.h
 *
 * Description:
 *
 *      Header for additional support functions
 *
 * Revision History:
 *
 *      06-01-08 : PLX SDK v6.00
 *
 ******************************************************************************/


#include "DrvDefs.h"



void FreeUserStruct( DEVICE_EXTENSION *dx);
void FreeDataArray( DEVICE_EXTENSION *dx);
void IrqCallbackRoutine( IN PVOID Context);
void FreeTimeSlot( DEVICE_EXTENSION *dx);
void FreeUtUserStruct( DEVICE_EXTENSION *dx);
void DeRegisterEvent( DEVICE_EXTENSION *dx);
void WriteDWord( DEVICE_EXTENSION *pdx, unsigned long offset, unsigned long dWord);

/**********************************************
 *               Functions
 *********************************************/
VOID
PlxLogPrintf(
    const char *format,
    ...
    );

VOID
Plx_sleep(
    U32 delay
    );

BOOLEAN
PlxSynchronizedRegisterModify(
    PLX_REG_DATA *pRegData
    );

VOID
PlxSignalNotifications(
    DEVICE_EXTENSION   *pdx,
    PLX_INTERRUPT_DATA *pIntData
    );

VOID
PlxRegistryInformationGet(
    UNICODE_STRING    *pRegistryPath,
    PLX_REGISTRY_INFO *pRegistryInfo
    );

U8
GetBarIndex(
    DEVICE_EXTENSION *pdx,
    U32               BarValue,
    U32              *pPciBars
    );

NTSTATUS
PlxCompleteIrp(
    PIRP     pIrp,
    NTSTATUS status
    );

NTSTATUS
PlxCompleteIrpWithInformation(
    PIRP         pIrp,
    NTSTATUS     status,
    PLX_UINT_PTR Info
    );

U16
PlxGetExtendedCapabilityOffset(
    DEVICE_EXTENSION *pdx,
    U16               CapabilityId
    );

VOID
PlxUpdateDeviceKey(
    DEVICE_EXTENSION *pdx
    );

BOOLEAN
PlxPowerStateIsSupported(
    DEVICE_EXTENSION   *pdx,
    DEVICE_POWER_STATE  PowerState
    );

PLX_STATUS
PlxPowerStateSet(
    DEVICE_EXTENSION   *pdx,
    DEVICE_POWER_STATE  PowerState
    );

NTSTATUS
PlxPciBarResourceMap(
    DEVICE_EXTENSION *pdx,
    U8                BarIndex
    );

VOID
PlxPciBarResourcesUnmap(
    DEVICE_EXTENSION *pdx
    );

VOID
PlxPciBarSpaceUnmapAll_ByOwner(
    DEVICE_EXTENSION *pdx,
    VOID             *pOwner
    );

VOID
PlxPciPhysicalMemoryFreeAll_ByOwner(
    DEVICE_EXTENSION *pdx,
    VOID             *pOwner
    );

VOID
PlxPciPhysicalMemoryUnmapAll_ByOwner(
    DEVICE_EXTENSION    *pdx,
    PLX_PHYS_MEM_OBJECT *pMemObject,
    VOID                *pOwner
    );

VOID*
Plx_dma_buffer_alloc(
    DEVICE_EXTENSION    *pdx,
    PLX_PHYS_MEM_OBJECT *pMemObject
    );

VOID
Plx_dma_buffer_free(
    DEVICE_EXTENSION    *pdx,
    PLX_PHYS_MEM_OBJECT *pMemObject
    );

VOID
PlxDmaChannelCleanup(
    DEVICE_EXTENSION *pdx,
    VOID             *pOwner
    );

VOID
PlxSglDmaTransferComplete(
    DEVICE_EXTENSION *pdx,
    U8                channel
    );

PLX_STATUS
PlxLockBufferAndBuildSgl(
    DEVICE_EXTENSION *pdx,
    U8                channel,
    PLX_DMA_PARAMS   *pDma,
    U32              *pSglAddress,
    BOOLEAN          *pbBits64
    );


#endif
