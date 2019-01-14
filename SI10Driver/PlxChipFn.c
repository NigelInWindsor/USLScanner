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
 *      PlxChipFn.c
 *
 * Description:
 *
 *      Contains PLX chip-specific support functions
 *
 * Revision History:
 *
 *      06-01-08 : PLX SDK v6.00
 *
 ******************************************************************************/


#include "ApiFunc.h"
#include "PlxChipFn.h"
#include "PlxInterrupt.h"
#include "SuppFunc.h"




/******************************************************************************
 *
 * Function   :  PlxChipInterruptsEnable
 *
 * Description:  Globally enables PLX chip interrupts
 *
 *****************************************************************************/
BOOLEAN
PlxChipInterruptsEnable(
    DEVICE_EXTENSION *pdx
    )
{
    U32 RegInterrupt;


    // Enable PCI interrupt
    RegInterrupt =
        PLX_9000_REG_READ(
            pdx,
            PCI9030_INT_CTRL_STAT
            );

    PLX_9000_REG_WRITE(
        pdx,
        PCI9030_INT_CTRL_STAT,
        RegInterrupt | (1 << 6)
        );

    return TRUE;
}




/******************************************************************************
 *
 * Function   :  PlxChipInterruptsDisable
 *
 * Description:  Globally disables PLX chip interrupts
 *
 *****************************************************************************/
BOOLEAN
PlxChipInterruptsDisable(
    DEVICE_EXTENSION *pdx
    )
{
    U32 RegInterrupt;


    // Disable PCI interrupt
    RegInterrupt =
        PLX_9000_REG_READ(
            pdx,
            PCI9030_INT_CTRL_STAT
            );

    PLX_9000_REG_WRITE(
        pdx,
        PCI9030_INT_CTRL_STAT,
        RegInterrupt & ~(1 << 6)
        );

    return TRUE;
}




/******************************************************************************
 *
 * Function   :  PlxChipSetInterruptNotifyFlags
 *
 * Description:  Sets the interrupt notification flags of a wait object
 *
 ******************************************************************************/
VOID
PlxChipSetInterruptNotifyFlags(
    PLX_INTERRUPT   *pPlxIntr,
    PLX_WAIT_OBJECT *pWaitObject
    )
{
    // Clear notify events
    pWaitObject->Notify_Flags    = INTR_TYPE_NONE;
    pWaitObject->Notify_Doorbell = 0;

    if (pPlxIntr->LocalToPci_1)
        pWaitObject->Notify_Flags |= INTR_TYPE_LOCAL_1;

    if (pPlxIntr->LocalToPci_2)
        pWaitObject->Notify_Flags |= INTR_TYPE_LOCAL_2;

    if (pPlxIntr->SwInterrupt)
        pWaitObject->Notify_Flags |= INTR_TYPE_SOFTWARE;
}




/******************************************************************************
 *
 * Function   :  PlxChipSetInterruptStatusFlags
 *
 * Description:  Sets the interrupts that triggered notification
 *
 ******************************************************************************/
VOID
PlxChipSetInterruptStatusFlags(
    PLX_INTERRUPT_DATA *pIntData,
    PLX_INTERRUPT      *pPlxIntr
    )
{
    // Clear all interrupt flags
    RtlZeroMemory(
        pPlxIntr,
        sizeof(PLX_INTERRUPT)
        );

    if (pIntData->Source_Ints & INTR_TYPE_LOCAL_1)
        pPlxIntr->LocalToPci_1 = 1;

    if (pIntData->Source_Ints & INTR_TYPE_LOCAL_2)
        pPlxIntr->LocalToPci_2 = 1;

    if (pIntData->Source_Ints & INTR_TYPE_SOFTWARE)
        pPlxIntr->SwInterrupt = 1;
}




/******************************************************************************
 *
 * Function   :  PlxChipGetRemapOffset
 *
 * Description:  Returns the remap register offset for a PCI BAR space
 *
 ******************************************************************************/
VOID
PlxChipGetRemapOffset(
    DEVICE_EXTENSION *pdx,
    U8                BarIndex,
    U16              *pOffset_RegRemap
    )
{
    switch (BarIndex)
    {
        case 2:
            *pOffset_RegRemap = PCI9030_REMAP_SPACE0;
            return;

        case 3:
            *pOffset_RegRemap = PCI9030_REMAP_SPACE1;
            return;

        case 4:
            *pOffset_RegRemap = PCI9030_REMAP_SPACE2;
            return;

        case 5:
            *pOffset_RegRemap = PCI9030_REMAP_SPACE3;
            return;
    }

    DebugPrintf(("ERROR - Invalid Space\n"));

    // BAR not supported
    *pOffset_RegRemap = (U16)-1;
}




/******************************************************************************
 *
 * Function   :  PlxChipPowerContextSave
 *
 * Description:  Save device-specific information when the device goes to low power state
 *
 *****************************************************************************/
BOOLEAN
PlxChipPowerContextSave(
    DEVICE_EXTENSION *pdx
    )
{
    U32 i;


    if (pdx->pSavedContext != NULL)
    {
        DebugPrintf(("ERROR - Context save buffer already exists\n"));
        return FALSE;
    }

    // Allocate buffer to store registers
    pdx->pSavedContext =
        ExAllocatePoolWithTag(
            NonPagedPool,
            PLX_CHIP_CONTEXT_TOTAL_REGS * sizeof(U32),
            (ULONG)' XLP'       // "PLX " in reverse order
            );

    if (pdx->pSavedContext == NULL)
    {
        DebugPrintf(("ERROR - Unable to allocate context buffer\n"));
        return FALSE;
    }

    // Save device context information
    for (i=0; i < PLX_CHIP_CONTEXT_TOTAL_REGS; i++)
    {
        pdx->pSavedContext[i] =
            PLX_9000_REG_READ(
                pdx,
                i * sizeof(U32)
                );
    }

    return TRUE;
}




/******************************************************************************
 *
 * Function   :  PlxChipPowerContextRestore
 *
 * Description:  Restore device-specific information when the device goes to full power state
 *
 *****************************************************************************/
BOOLEAN
PlxChipPowerContextRestore(
    DEVICE_EXTENSION *pdx
    )
{
    U32 i;


    if (pdx->pSavedContext == NULL)
    {
        DebugPrintf(("ERROR - Context information has not been saved\n"));
        return FALSE;
    }

    // A return to D0 state should cause a local reset. A manual reset is forced
    // to ensure the EEPROM gets re-loaded.
    PlxPciDeviceReset(
        pdx,
        &pdx->Key
        );

    // Restore device context information
    for (i=0; i < PLX_CHIP_CONTEXT_TOTAL_REGS; i++)
    {
        PLX_9000_REG_WRITE(
            pdx,
            i * sizeof(U32),
            pdx->pSavedContext[i]
            );
    }

    // Release context save buffer
    ExFreePool(
        pdx->pSavedContext
        );

    // Mark buffer as released
    pdx->pSavedContext = NULL;

    return TRUE;
}
