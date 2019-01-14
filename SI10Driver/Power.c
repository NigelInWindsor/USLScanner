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
 *      Power.c
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


#include "GlobalVars.h"
#include "PciFunc.h"
#include "PlugPlay.h"
#include "PlxChipFn.h"
#include "Power.h"
#include "SuppFunc.h"




/******************************************************************************
 *
 * Function   :  Dispatch_Power
 *
 * Description:  Handles power requests
 *
 ******************************************************************************/
NTSTATUS
Dispatch_Power(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
    NTSTATUS           status;
    PIO_STACK_LOCATION pIrpStack;


    DebugPrintf_NoInfo(("\n"));
    DebugPrintf(("Received POWER Message (IRP=%p) ==> ", pIrp));

    // Get current I/O stack
    pIrpStack = IoGetCurrentIrpStackLocation( pIrp );

    // Dispatch power IRP
    switch (pIrpStack->MinorFunction)
    {
        case IRP_MN_WAIT_WAKE:
            DebugPrintf_NoInfo(("IRP_MN_WAIT_WAKE\n"));
            status =
                Power_ForwardIrpToNextDriver(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_POWER_SEQUENCE:
            DebugPrintf_NoInfo(("IRP_MN_POWER_SEQUENCE\n"));
            status =
                Power_ForwardIrpToNextDriver(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_QUERY_POWER:
            DebugPrintf_NoInfo(("IRP_MN_QUERY_POWER\n"));

            status =
                Power_HandleIrpQueryPower(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_SET_POWER:
            DebugPrintf_NoInfo(("IRP_MN_SET_POWER\n"));

            status =
                Power_HandleIrpSetPower(
                    fdo,
                    pIrp
                    );
            break;

        default:
            DebugPrintf_NoInfo((
                "Unsupported IRP_MN_Xxx (%08X)\n",
                pIrpStack->MinorFunction
                ));

            status =
                Power_ForwardIrpToNextDriver(
                    fdo,
                    pIrp
                    );
            break;
    }

    DebugPrintf(("...Completed message\n"));

    return status;
}




/******************************************************************************
 *
 * Function   :  Power_ForwardIrpToNextDriver
 *
 * Description:  Forwards the IRP to the next-lower driver
 *
 ******************************************************************************/
NTSTATUS
Power_ForwardIrpToNextDriver(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
    NTSTATUS status;


    DebugPrintf(("Forwarding IRP to next-lower driver\n"));

    // Notify Power Manager that driver is ready to accept another power IRP.
    // Starting with Vista, drivers do not need to call PoStartNextPowerIrp().
    if (Gbl_WindowsVistaOrHigher == FALSE)
        PoStartNextPowerIrp( pIrp );

    // Provide lower driver the same I/O stack this driver received
    IoSkipCurrentIrpStackLocation( pIrp );

    // Pass IRP to next-lower driver
    // Starting with Vista, drivers should call IoCallDriver(), instead of
    //  PoCallDriver(), to pass power IRPs to lower driver
    if (Gbl_WindowsVistaOrHigher == FALSE)
    {
        status = 
            PoCallDriver(
                ((DEVICE_EXTENSION *)fdo->DeviceExtension)->pLowerDeviceObject,
                pIrp
                );
    }
    else
    {
        status = 
            IoCallDriver(
                ((DEVICE_EXTENSION *)fdo->DeviceExtension)->pLowerDeviceObject,
                pIrp
                );
    }

    return status;
}




/******************************************************************************
 *
 * Function   :  Power_HandleIrpQueryPower
 *
 * Description:  Handle the IRP_MN_QUERY_POWER power IRP
 *
 ******************************************************************************/
NTSTATUS
Power_HandleIrpQueryPower(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
    PIO_STACK_LOCATION pIrpStack;


    // Get current I/O stack
    pIrpStack = IoGetCurrentIrpStackLocation( pIrp );

    if (pIrpStack->Parameters.Power.Type == SystemPowerState)
    {
        DebugPrintf(("Query to put system into power state ==> "));

        if (pIrpStack->Parameters.Power.State.SystemState == PowerSystemWorking)
            DebugPrintf_NoInfo(("S0 (Working)\n"));
        else if (pIrpStack->Parameters.Power.State.SystemState == PowerSystemSleeping1)
            DebugPrintf_NoInfo(("S1 (Sleep 1)\n"));
        else if (pIrpStack->Parameters.Power.State.SystemState == PowerSystemSleeping2)
            DebugPrintf_NoInfo(("S1 (Sleep 2)\n"));
        else if (pIrpStack->Parameters.Power.State.SystemState == PowerSystemSleeping3)
            DebugPrintf_NoInfo(("S3 (Sleep 3)\n"));
        else if (pIrpStack->Parameters.Power.State.SystemState == PowerSystemHibernate)
            DebugPrintf_NoInfo(("S4 (Hibernate)\n"));
        else if (pIrpStack->Parameters.Power.State.SystemState == PowerSystemShutdown)
        {
            DebugPrintf_NoInfo(("S5 (Shutdown"));

            if (pIrpStack->Parameters.Power.ShutdownType == PowerActionSleep)
                DebugPrintf_NoInfo((" & Sleep)\n"));
            else if (pIrpStack->Parameters.Power.ShutdownType == PowerActionHibernate)
                DebugPrintf_NoInfo((" & Hibernate)\n"));
            else if (pIrpStack->Parameters.Power.ShutdownType == PowerActionShutdownReset)
                DebugPrintf_NoInfo((" & Reset)\n"));
            else // PowerActionShutdown, PowerActionShutdownOff
                DebugPrintf_NoInfo((")\n"));
        }
    }
    else    // DevicePowerState
    {
        DebugPrintf(("Query to put device into power state ==> "));

        if (pIrpStack->Parameters.Power.State.DeviceState == PowerDeviceD0)
            DebugPrintf_NoInfo(("D0\n"));
        else if (pIrpStack->Parameters.Power.State.DeviceState == PowerDeviceD1)
            DebugPrintf_NoInfo(("D1\n"));
        else if (pIrpStack->Parameters.Power.State.DeviceState == PowerDeviceD2)
            DebugPrintf_NoInfo(("D2\n"));
        else if (pIrpStack->Parameters.Power.State.DeviceState == PowerDeviceD3)
            DebugPrintf_NoInfo(("D3\n"));
    }

    // Flag the device supports power state
    pIrp->IoStatus.Status = STATUS_SUCCESS;

    // Pass IRP to next-lower driver
    return Power_ForwardIrpToNextDriver(
               fdo,
               pIrp
               );
}




/******************************************************************************
 *
 * Function   :  Power_HandleIrpSetPower
 *
 * Description:  Handles IRP_MN_SET_POWER IRP
 *
 ******************************************************************************/
NTSTATUS
Power_HandleIrpSetPower(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
    NTSTATUS            status;
    POWER_STATE         NewPowerState;
    DEVICE_EXTENSION   *pdx;
    PIO_STACK_LOCATION  pIrpStack;


    pdx = fdo->DeviceExtension;

    // Get current I/O stack
    pIrpStack = IoGetCurrentIrpStackLocation( pIrp );

    if (pIrpStack->Parameters.Power.Type == SystemPowerState)
    {
        //
        // Handle SET_POWER IRP for system state
        //

        DebugPrintf(("System is being put into power state ==> "));

        if (pIrpStack->Parameters.Power.State.SystemState == PowerSystemWorking)
            DebugPrintf_NoInfo(("S0 (Full Power)\n"));
        else if (pIrpStack->Parameters.Power.State.SystemState == PowerSystemSleeping1)
            DebugPrintf_NoInfo(("S1 (Sleep 1)\n"));
        else if (pIrpStack->Parameters.Power.State.SystemState == PowerSystemSleeping2)
            DebugPrintf_NoInfo(("S1 (Sleep 2)\n"));
        else if (pIrpStack->Parameters.Power.State.SystemState == PowerSystemSleeping3)
            DebugPrintf_NoInfo(("S3 (Sleep 3)\n"));
        else if (pIrpStack->Parameters.Power.State.SystemState == PowerSystemHibernate)
            DebugPrintf_NoInfo(("S4 (Hibernate)\n"));
        else if (pIrpStack->Parameters.Power.State.SystemState == PowerSystemShutdown)
        {
            DebugPrintf_NoInfo(("S5 (Shutdown"));

            if (pIrpStack->Parameters.Power.ShutdownType == PowerActionSleep)
                DebugPrintf_NoInfo((" & Sleep)\n"));
            else if (pIrpStack->Parameters.Power.ShutdownType == PowerActionHibernate)
                DebugPrintf_NoInfo((" & Hibernate)\n"));
            else if (pIrpStack->Parameters.Power.ShutdownType == PowerActionShutdownReset)
                DebugPrintf_NoInfo((" & Reset)\n"));
            else // PowerActionShutdown, PowerActionShutdownOff
                DebugPrintf_NoInfo((")\n"));
        }

        // Only save context if not shutting down
        if (pIrpStack->Parameters.Power.State.SystemState == PowerSystemShutdown)
            pdx->bSaveContext = FALSE;
        else
            pdx->bSaveContext = TRUE;

        DebugPrintf(("Forwarding IRP to next-lower driver\n"));

        // Copy IRP stack so completion routine runs with correct stack
        IoCopyCurrentIrpStackLocationToNext( pIrp );

        // Set IRP competion routine
        IoSetCompletionRoutine(
            pIrp,                               // IRP
            Power_CompleteSystemSetPowerIrp,    // Completion routine
            NULL,                               // Context parameter
            TRUE,                               // Invoke if IRP successful?
            TRUE,                               // Invoke if IRP error?
            TRUE                                // Invoke if IRP cancelled?
            );

        // Mark the IRP as pending
        IoMarkIrpPending( pIrp );

        // Pass IRP to next-lower driver
        if (Gbl_WindowsVistaOrHigher == FALSE)
        {
            PoCallDriver( pdx->pLowerDeviceObject, pIrp );
        }
        else
        {
            IoCallDriver( pdx->pLowerDeviceObject, pIrp );
        }
    }
    else if (pIrpStack->Parameters.Power.Type == DevicePowerState)
    {
        //
        // Handle SET_POWER IRP for device state
        //

        // Get requested device power state
        NewPowerState.DeviceState = pIrpStack->Parameters.Power.State.DeviceState;

        DebugPrintf(("Request to put device into power state ==> "));

        if (NewPowerState.DeviceState == PowerDeviceD0)
            DebugPrintf_NoInfo(("D0 (Full Power)\n"));
        else if (NewPowerState.DeviceState == PowerDeviceD1)
            DebugPrintf_NoInfo(("D1 (Low Power 1)\n"));
        else if (NewPowerState.DeviceState == PowerDeviceD2)
            DebugPrintf_NoInfo(("D2 (Low Power 2)\n"));
        else if (NewPowerState.DeviceState == PowerDeviceD3)
            DebugPrintf_NoInfo(("D3 (Powered Down)\n"));
        else
            DebugPrintf_NoInfo(("?? (Unspecified state)\n"));

        // Default IRP to successful status
        pIrp->IoStatus.Status = STATUS_SUCCESS;

        if (pdx->PowerState == NewPowerState.DeviceState)
        {
            //
            // No device power state change needed
            //
            DebugPrintf(("Device already in requested power state\n"));

            // Pass IRP to next-lower driver
            Power_ForwardIrpToNextDriver(
                fdo,
                pIrp
                );
        }
        else if (pdx->PowerState > NewPowerState.DeviceState)
        {
            //
            // Powering up device
            //
            DebugPrintf(("Powering up device\n"));

            DebugPrintf(("Forwarding IRP to next-lower driver\n"));

            // Mark the IRP as pending
            IoMarkIrpPending( pIrp );

            // Copy IRP stack so completion routine runs with correct stack
            IoCopyCurrentIrpStackLocationToNext( pIrp );

            // Set IRP competion routine
            IoSetCompletionRoutine(
                pIrp,                           // IRP
                Power_CompleteDevicePowerUp,    // Completion routine
                NULL,                           // Context parameter
                TRUE,                           // Invoke if IRP successful?
                TRUE,                           // Invoke if IRP error?
                TRUE                            // Invoke if IRP cancelled?
                );

            // Pass IRP to next-lower driver
            if (Gbl_WindowsVistaOrHigher == FALSE)
            {
                PoCallDriver( pdx->pLowerDeviceObject, pIrp );
            }
            else
            {
                IoCallDriver( pdx->pLowerDeviceObject, pIrp );
            }
        }
        else
        {
            //
            // Powering down device
            //
            DebugPrintf(("Powering down device\n"));

            // Disable device interrupts
            PlxChipInterruptsDisable( pdx );

            // Save device context information if requested
            if (pdx->bSaveContext)
            { 
                DebugPrintf(("Saving device context\n"));

                PlxChipPowerContextSave( pdx );

                // Flag context is saved
                pdx->bSaveContext = FALSE;
            }

            // Report new device state to power manager before leaving D0 state
            PoSetPowerState(
                fdo,
                DevicePowerState,
                NewPowerState
                );

            // Set the new power state
            PlxPowerStateSet(
                pdx,
                NewPowerState.DeviceState
                );

            // Record the new power state
            pdx->PowerState = NewPowerState.DeviceState;

            // Pass IRP to next-lower driver
            Power_ForwardIrpToNextDriver(
                fdo,
                pIrp
                );
        }
    }

    return STATUS_PENDING;
}




/******************************************************************************
 *
 * Function   :  Power_CompleteSystemSetPowerIrp
 *
 * Description:  Completion routine for system SET_POWER IRP
 *
 ******************************************************************************/
NTSTATUS
Power_CompleteSystemSetPowerIrp(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp,
    PVOID          Context
    )
{
    NTSTATUS            status;
    POWER_STATE         NewPowerState;
    DEVICE_EXTENSION   *pdx;
    PIO_STACK_LOCATION  pIrpStack;


    // Get current IRP status
    status = pIrp->IoStatus.Status;

    // If lower driver failed IRP, just return status
    if (!NT_SUCCESS(status))
    {
        DebugPrintf(("ERROR - IRP failed by lower driver (status=%Xh)\n", status));

        // Notify Power Manager that driver is ready to accept another power IRP.
        if (Gbl_WindowsVistaOrHigher == FALSE)
            PoStartNextPowerIrp( pIrp );

        return status;
    }

    // Get device extension
    pdx = fdo->DeviceExtension;

    // Get current I/O stack
    pIrpStack = IoGetCurrentIrpStackLocation( pIrp );

    // Set corresponding device state based on system state
    if (pIrpStack->Parameters.Power.State.SystemState <= PowerSystemWorking)
        NewPowerState.DeviceState = PowerDeviceD0;
    else
        NewPowerState.DeviceState = PowerDeviceD3;

    DebugPrintf(("Requesting SET_POWER IRP to set device power state\n"));

    // Request a SET_POWER IRP to set the device state
    status =
        PoRequestPowerIrp(
            pdx->pPhysicalDeviceObject,     // Physical Device Object
            IRP_MN_SET_POWER,               // Minor function
            NewPowerState,                  // New power state
            Power_PowerChangeComplete,      // IRP completion/callback function
            pIrp,                           // Context - should contain original system SET_POWER IRP
            NULL                            // Optional - used only for IRP_MN_WAIT_WAKE
            );

    if (status != STATUS_PENDING)
    {
        DebugPrintf(("ERROR - Unable to request device SET_POWER IRP (status=%Xh)\n", status));

        // Notify Power Manager that driver is ready to accept another power IRP.
        if (Gbl_WindowsVistaOrHigher == FALSE)
            PoStartNextPowerIrp( pIrp );

        // Complete system IRP
        return PlxCompleteIrp(
                   pIrp,
                   status
                   );
    }

    return STATUS_MORE_PROCESSING_REQUIRED;
}




/******************************************************************************
 *
 * Function   :  Power_PowerChangeComplete
 *
 * Description:  Callback function called after all SET_POWER IRPs have completed
 *
 ******************************************************************************/
VOID
Power_PowerChangeComplete(
    PDEVICE_OBJECT   pDeviceObject,
    UCHAR            MinorFunction,
    POWER_STATE      PowerState,
    PVOID            Context,
    PIO_STATUS_BLOCK pIoStatus
    )
{
    PIRP pIrp;


    // Get system SET_POWER IRP
    pIrp = (PIRP)Context;

    DebugPrintf((
        "Device power change complete - completing system SET_POWER IRP (%p)\n",
        pIrp
        ));

    // Notify Power Manager that driver is ready to accept another power IRP.
    if (Gbl_WindowsVistaOrHigher == FALSE)
        PoStartNextPowerIrp( pIrp );

    // Complete system IRP
    PlxCompleteIrp(
        pIrp,
        pIrp->IoStatus.Status
        );
}




/******************************************************************************
 *
 * Function   :  Power_CompleteDevicePowerUp
 *
 * Description:  Completion routine for device SET_POWER when device powers up to D0
 *
 ******************************************************************************/
NTSTATUS
Power_CompleteDevicePowerUp(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp,
    PVOID          Context
    )
{
    POWER_STATE       NewPowerState;
    DEVICE_EXTENSION *pdx;


    pdx = fdo->DeviceExtension;

    // Set new power state
    PlxPowerStateSet(
        pdx,
        PowerDeviceD0
        );

    // Record the new power state
    pdx->PowerState = PowerDeviceD0;

    DebugPrintf(("Restoring device context\n"));

    // Restore device context
    PlxChipPowerContextRestore( pdx );

    // Re-enable device interrupts
    PlxChipInterruptsEnable( pdx );

    // Report new device state to power manager after entering D0 state
    NewPowerState.DeviceState = PowerDeviceD0;

    PoSetPowerState(
        fdo,
        DevicePowerState,
        NewPowerState
        );

    // Notify Power Manager that driver is ready to accept another power IRP.
    if (Gbl_WindowsVistaOrHigher == FALSE)
        PoStartNextPowerIrp( pIrp );

    return STATUS_SUCCESS;
}
