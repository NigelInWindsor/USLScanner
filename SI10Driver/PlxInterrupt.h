#ifndef __PLX_INTERRUPT_H
#define __PLX_INTERRUPT_H

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
 *      PlxInterrupt.h
 *
 * Description:
 *
 *      Driver Interrupt functions
 *
 * Revision History:
 *
 *      02-01-07 : PLX SDK v5.00
 *
 ******************************************************************************/


#include "PlxTypes.h"




/**********************************************
*               Definitions
**********************************************/
#define INTR_TYPE_NONE                  0              // Interrupt identifiers
#define INTR_TYPE_LOCAL_1               (1 << 0)
#define INTR_TYPE_LOCAL_2               (1 << 1)
#define INTR_TYPE_PCI_ABORT             (1 << 2)
#define INTR_TYPE_DOORBELL              (1 << 3)
#define INTR_TYPE_OUTBOUND_POST         (1 << 4)
#define INTR_TYPE_DMA_0                 (1 << 5)
#define INTR_TYPE_DMA_1                 (1 << 6)
#define INTR_TYPE_SOFTWARE              (1 << 7)




/**********************************************
*               Functions
**********************************************/
BOOLEAN
OnInterrupt(
    PKINTERRUPT pInterrupt,
    PVOID       ServiceContext
    );

VOID
DpcForIsr(
    PKDPC pDpc,
    PVOID pContext,
    PVOID pArg1,
    PVOID pArg2
    );



#endif
