#ifndef __PLX_CHIP_API_H
#define __PLX_CHIP_API_H

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
 *      PlxChipApi.h
 *
 * Description:
 *
 *      Header for PLX chip-specific API functions
 *
 * Revision History:
 *
 *      12-01-07 : PLX SDK v5.20
 *
 ******************************************************************************/


#include "DrvDefs.h"




/**********************************************
 *                Functions
 *********************************************/
PLX_STATUS
PlxChip_BoardReset(
    DEVICE_EXTENSION *pdx
    );

PLX_STATUS
PlxChip_EepromReadByOffset(
    DEVICE_EXTENSION *pdx,
    U16               offset,
    U32              *pValue
    );

PLX_STATUS
PlxChip_EepromWriteByOffset(
    DEVICE_EXTENSION *pdx,
    U16               offset,
    U32               value
    );

PLX_STATUS
PlxChip_InterruptEnable(
    DEVICE_EXTENSION *pdx,
    PLX_INTERRUPT    *pPlxIntr
    );

PLX_STATUS
PlxChip_InterruptDisable(
    DEVICE_EXTENSION *pdx,
    PLX_INTERRUPT    *pPlxIntr
    );

PLX_STATUS
PlxChip_DmaControl(
    DEVICE_EXTENSION *pdx,
    U8                channel,
    PLX_DMA_COMMAND   command
    );

PLX_STATUS
PlxChip_DmaStatus(
    DEVICE_EXTENSION *pdx,
    U8                channel
    );

PLX_STATUS
PlxChip_DmaChannelOpen(
    DEVICE_EXTENSION *pdx,
    U8                channel,
    PLX_DMA_PROP     *pProp,
    VOID             *pOwner
    );

PLX_STATUS
PlxChip_DmaTransferBlock(
    DEVICE_EXTENSION *pdx,
    U8                channel,
    PLX_DMA_PARAMS   *pParams
    );

PLX_STATUS
PlxChip_DmaTransferUserBuffer(
    DEVICE_EXTENSION *pdx,
    U8                channel,
    PLX_DMA_PARAMS   *pParams
    );

PLX_STATUS
PlxChip_DmaChannelClose(
    DEVICE_EXTENSION *pdx,
    U8                channel,
    BOOLEAN           bCheckInProgress
    );




#endif
