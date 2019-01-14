#ifndef __PLX_STATUS_H
#define __PLX_STATUS_H

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
 *      PlxStat.h
 *
 * Description:
 *
 *      This file defines all the status codes for PLX SDK
 *
 * Revision:
 *
 *      04-01-08 : PLX SDK v6.00
 *
 ******************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif




/******************************************
*             Definitions
******************************************/
#define PLX_STATUS_START               0x200   // Starting status code


// API Return Code Values
typedef enum _PLX_STATUS
{
    PLXApiSuccess = PLX_STATUS_START,
    PLXApiFailed,
    PLXApiNullParam,
    PLXApiUnsupportedFunction,
    PLXApiNoActiveDriver,
    PLXApiConfigAccessFailed,
    PLXApiInvalidDeviceInfo,
    PLXApiInvalidDriverVersion,
    PLXApiInvalidOffset,
    PLXApiInvalidData,
    PLXApiInvalidSize,
    PLXApiInvalidAddress,
    PLXApiInvalidAccessType,
    PLXApiInvalidIndex,
    PLXApiInvalidPowerState,
    PLXApiInvalidIopSpace,
    PLXApiInvalidHandle,
    PLXApiInvalidPciSpace,
    PLXApiInvalidBusIndex,
    PLXApiInsufficientResources,
    PLXApiWaitTimeout,
    PLXApiWaitCanceled,
    PLXApiDmaChannelUnavailable,
    PLXApiDmaChannelInvalid,
    PLXApiDmaDone,
    PLXApiDmaPaused,
    PLXApiDmaInProgress,
    PLXApiDmaCommandInvalid,
    PLXApiDmaInvalidChannelPriority,
    PLXApiDmaSglPagesGetError,
    PLXApiDmaSglPagesLockError,
    PLXApiMuFifoEmpty,
    PLXApiMuFifoFull,
    PLXApiPowerDown,
    PLXApiHSNotSupported,
    PLXApiVPDNotSupported,
    PLXApiDeviceInUse,
    PLXApiLastError               // Do not add API errors below this line
} PLX_STATUS;


// DBG - Added to support legacy code -- will be removed in future release
//#if !defined(PLX_DOS)
//   typedef PLX_STATUS        RETURN_CODE;
//#endif



#ifdef __cplusplus
}
#endif

#endif
