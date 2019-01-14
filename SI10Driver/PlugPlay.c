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
 *      PlugPlay.c
 *
 * Description:
 *
 *      Plug 'n' Play handler functions
 *
 * Revision History:
 *
 *      12-01-07 : PLX SDK v5.20
 *
 ******************************************************************************/


#include "Dispatch.h"
#include "Driver.h"
#include "GlobalVars.h"
#include "PciFunc.h"
#include "PlugPlay.h"
#include "SuppFunc.h"


/**************************************************************
 * The following IRP minor code is erroneously not included
 * in the Windows DDK "wdm.h" file.  It is, however, included
 * in the "ntddk.h" file.  Until this is fixed by Microsoft,
 * the definition is provided here so the driver will build.
 *************************************************************/
#if !defined(IRP_MN_QUERY_LEGACY_BUS_INFORMATION)
    #define IRP_MN_QUERY_LEGACY_BUS_INFORMATION        0x18
#endif




/******************************************************************************
 *
 * Function   :  Dispatch_Pnp
 *
 * Description:  Handle PnP requests
 *
 ******************************************************************************/
NTSTATUS
Dispatch_Pnp(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
    NTSTATUS           status;
    PIO_STACK_LOCATION stack;


    DebugPrintf_NoInfo(("\n"));
    DebugPrintf((
        "Received PNP Message (IRP=0x%p) ==> ",
        pIrp
        ));

    stack =
        IoGetCurrentIrpStackLocation(
            pIrp
            );

    // Check minor code
    switch (stack->MinorFunction)
    {
        case IRP_MN_START_DEVICE:
            DebugPrintf_NoInfo(("IRP_MN_START_DEVICE\n"));
            status =
                HandleStartDevice(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_STOP_DEVICE:
            DebugPrintf_NoInfo(("IRP_MN_STOP_DEVICE\n"));
            status =
                HandleStopDevice(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_REMOVE_DEVICE:
            DebugPrintf_NoInfo(("IRP_MN_REMOVE_DEVICE\n"));
            status =
                HandleRemoveDevice(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_QUERY_REMOVE_DEVICE:
            DebugPrintf_NoInfo(("IRP_MN_QUERY_REMOVE_DEVICE\n"));
            status =
                HandleQueryRemoveDevice(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_CANCEL_REMOVE_DEVICE:
            DebugPrintf_NoInfo(("IRP_MN_CANCEL_REMOVE_DEVICE\n"));
            status =
                HandleCancelRemoveDevice(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_QUERY_STOP_DEVICE:
            DebugPrintf_NoInfo(("IRP_MN_QUERY_STOP_DEVICE\n"));
            status =
                HandleQueryRemoveDevice(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_CANCEL_STOP_DEVICE:
            DebugPrintf_NoInfo(("IRP_MN_CANCEL_STOP_DEVICE\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_QUERY_DEVICE_RELATIONS:
            DebugPrintf_NoInfo(("IRP_MN_QUERY_DEVICE_RELATIONS\n"));
            status =
                HandleQueryDeviceRelations(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_QUERY_INTERFACE:
            DebugPrintf_NoInfo(("IRP_MN_QUERY_INTERFACE\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_QUERY_CAPABILITIES:
            DebugPrintf_NoInfo(("IRP_MN_QUERY_CAPABILITIES\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_QUERY_RESOURCES:
            DebugPrintf_NoInfo(("IRP_MN_QUERY_RESOURCES\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_QUERY_RESOURCE_REQUIREMENTS:
            DebugPrintf_NoInfo(("IRP_MN_QUERY_RESOURCE_REQUIREMENTS\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_QUERY_DEVICE_TEXT:
            DebugPrintf_NoInfo(("IRP_MN_QUERY_DEVICE_TEXT\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_FILTER_RESOURCE_REQUIREMENTS:
            DebugPrintf_NoInfo(("IRP_MN_FILTER_RESOURCE_REQUIREMENTS\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_READ_CONFIG:
            DebugPrintf_NoInfo(("IRP_MN_READ_CONFIG\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_WRITE_CONFIG:
            DebugPrintf_NoInfo(("IRP_MN_WRITE_CONFIG\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_EJECT:
            DebugPrintf_NoInfo(("IRP_MN_EJECT\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_SET_LOCK:
            DebugPrintf_NoInfo(("IRP_MN_SET_LOCK\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_QUERY_ID:
            DebugPrintf_NoInfo(("IRP_MN_QUERY_ID\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_QUERY_PNP_DEVICE_STATE:
            DebugPrintf_NoInfo(("IRP_MN_QUERY_PNP_DEVICE_STATE\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_QUERY_BUS_INFORMATION:
            DebugPrintf_NoInfo(("IRP_MN_QUERY_BUS_INFORMATION\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_DEVICE_USAGE_NOTIFICATION:
            DebugPrintf_NoInfo(("IRP_MN_DEVICE_USAGE_NOTIFICATION\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_SURPRISE_REMOVAL:
            DebugPrintf_NoInfo(("IRP_MN_SURPRISE_REMOVAL\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        case IRP_MN_QUERY_LEGACY_BUS_INFORMATION:
            DebugPrintf_NoInfo(("IRP_MN_QUERY_LEGACY_BUS_INFORMATION\n"));
            status =
                DefaultPnpHandler(
                    fdo,
                    pIrp
                    );
            break;

        default:
            DebugPrintf_NoInfo(("Unsupported IRP_MN_Xxx (0x%08x)\n", stack->MinorFunction));
            status =
                DefaultPnpHandler(
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
 * Function   :  DefaultPnpHandler
 *
 * Description:  Handle standard PnP requests
 *
 ******************************************************************************/
NTSTATUS
DefaultPnpHandler(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
    DebugPrintf(("Forwarded IRP to next lower driver\n"));

    IoSkipCurrentIrpStackLocation(
        pIrp
        );

    return IoCallDriver(
               ((DEVICE_EXTENSION *)fdo->DeviceExtension)->pLowerDeviceObject,
               pIrp
               );
}




/******************************************************************************
 *
 * Function   :  HandleQueryRemoveDevice
 *
 * Description:  Handle the IRP_MN_QUERY_REMOVE_DEVICE PnP request
 *
 ******************************************************************************/
NTSTATUS
HandleQueryRemoveDevice(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
    DEVICE_EXTENSION *pdx;


    /********************************************************
     * Since the first device the driver owns is responsible
     * for the global common buffer, it can only be removed
     * if it is the last remaining device.  The device is
     * identified by the serial number.  If other devices
     * exist, the QUERY_REMOVE_DEVICE must fail.
     *******************************************************/

    pdx = fdo->DeviceExtension;

    // Check if more than one device is owned
    if (Gbl_DeviceCount > 1)
    {
        // This is not the only device left, check device name
        if (_wcsicmp(
                L"\\DosDevices\\" PLX_DRIVER_NAME_UNICODE L"-0",
                pdx->LinkName
                ) == 0)
        {
            DebugPrintf((
                "ERROR - Device owns shared resources in use, cannot remove at this time\n"
                ));

            // Device is first one, cannot be removed at this time
            return PlxCompleteIrpWithInformation(
                       pIrp,
                       STATUS_DEVICE_BUSY,
                       0
                       );
        }
    }

    // Succeed the IRP and pass to next driver
    pIrp->IoStatus.Status = STATUS_SUCCESS;

    // Pass to next driver in stack
    return DefaultPnpHandler(
               fdo,
               pIrp
               );
}




/******************************************************************************
 *
 * Function   :  HandleCancelRemoveDevice
 *
 * Description:  Handle the IRP_MN_CANCEL_REMOVE_DEVICE PnP request
 *
 ******************************************************************************/
NTSTATUS
HandleCancelRemoveDevice(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
    // We must succeed the IRP and pass it to the parent
    pIrp->IoStatus.Status = STATUS_SUCCESS;

    // Forward to parent driver
    return DefaultPnpHandler(
               fdo,
               pIrp
               );
}




/******************************************************************************
 *
 * Function   :  HandleRemoveDevice
 *
 * Description:  Handle the IRP_MN_REMOVE_DEVICE PnP request
 *
 ******************************************************************************/
NTSTATUS
HandleRemoveDevice(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
    NTSTATUS status;


    // Halt the device and release its resources
    StopDevice(
        fdo
        );

    // Succeed the IRP and pass to next driver
    pIrp->IoStatus.Status = STATUS_SUCCESS;

    // Pass to next driver in stack
    status =
        DefaultPnpHandler(
            fdo,
            pIrp
            );

    // Remove the device object
    RemoveDevice(
        fdo
        );

    return status;
}




/******************************************************************************
 *
 * Function   :  HandleStartDevice
 *
 * Description:  Handle the IRP_MN_START_DEVICE PnP request
 *
 ******************************************************************************/
NTSTATUS
HandleStartDevice(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
    NTSTATUS           status;
    PIO_STACK_LOCATION stack;


    /********************************************************
     * First let all lower-level drivers handle this request,
     * which is typically just the physical device object
     * created by the bus driver.
     *******************************************************/

    status =
        ForwardAndWait(
            fdo,
            pIrp
            );

    if (!NT_SUCCESS(status))
    {
        return PlxCompleteIrp(
                   pIrp,
                   status
                   );
    }

    stack =
        IoGetCurrentIrpStackLocation(
            pIrp
            );

    // Assign resources to the devices
    status =
        StartDevice(
            fdo,
            &stack->Parameters.StartDevice.AllocatedResources->List[0].PartialResourceList,
            &stack->Parameters.StartDevice.AllocatedResourcesTranslated->List[0].PartialResourceList
            );

    return PlxCompleteIrpWithInformation(
               pIrp,
               status,
               0
               );
}




/******************************************************************************
 *
 * Function   :  HandleStopDevice
 *
 * Description:  Handle the IRP_MN_STOP_DEVICE PnP request
 *
 ******************************************************************************/
NTSTATUS
HandleStopDevice(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
    NTSTATUS status;


    status =
        DefaultPnpHandler(
            fdo,
            pIrp
            );

    StopDevice(
        fdo
        );

    return status;
}




/******************************************************************************
 *
 * Function   :  HandleQueryDeviceRelations
 *
 * Description:  Handle the IRP_MN_QUERY_DEVICE_RELATIONS PnP request
 *
 ******************************************************************************/
NTSTATUS
HandleQueryDeviceRelations(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
    U32                 MemNeeded;
    PDEVICE_OBJECT      pdo;
    PDEVICE_OBJECT      pDevice;
    DEVICE_EXTENSION   *pdx;
    DEVICE_RELATIONS   *pDeviceRelations;
    PIO_STACK_LOCATION  stack;


    /********************************************************
     * When a query for device relations is received and the
     * request is for "RemovalRelations", the driver may
     * report to the PnP manager a list of device relations.
     * 
     * Since the first device loaded by the driver (PciXxx-0)
     * is responsible for the common buffer, all subsequently
     * owned devices are related to it.  An attempt to remove
     * the primary device requires the driver to report that
     * all other devices must be removed first.  The PnP
     * manager determines this by the device relations
     * provided by this driver.
     *******************************************************/

    pdx = fdo->DeviceExtension;

    // Get I/O Stack
    stack =
        IoGetCurrentIrpStackLocation(
            pIrp
            );

    // Check if this is for removal and more than one device is owned
    if ((stack->Parameters.QueryDeviceRelations.Type == RemovalRelations) &&
        (Gbl_DeviceCount > 1))
    {
        DebugPrintf((
            "Query for: RemovalRelations\n"
            ));

        // This is not the only device left, check if it's the primary device
        if (_wcsicmp(
                L"\\DosDevices\\" PLX_DRIVER_NAME_UNICODE L"-0",
                pdx->LinkName
                ) == 0)
        {
            /********************************************************
             * Calculate memory needed for device relations
             *
             * The default DEVICE_RELATIONS structure includes space
             * for one device object.  Therefore, the memory needed
             * is the total # of devices minus 2.  One since the
             * primary device is not reported and one for memory
             * already provided in the structure.
             *******************************************************/
            MemNeeded = sizeof(DEVICE_RELATIONS) +
                        ((Gbl_DeviceCount - 2) * sizeof(PDEVICE_OBJECT));

            // Allocate memory for device relations structure
            pDeviceRelations =
                ExAllocatePoolWithTag(
                    PagedPool,
                    MemNeeded,
                    (ULONG)' XLP'       // "PLX " in reverse order
                    );

            // Report other devices
            DebugPrintf((
                "Following device(s) must be removed first:\n"
                ));

            // Get first device object
            pDevice = fdo->DriverObject->DeviceObject;

            // Reset device count
            pDeviceRelations->Count = 0;

            // Traverse driver owned devices to build relations
            while (pDevice != NULL)
            {
                if (pDevice->DeviceExtension != pdx)
                {
                    // Get the physical device object
                    pdo = ((DEVICE_EXTENSION*)(pDevice->DeviceExtension))->pPhysicalDeviceObject;

                    DebugPrintf_NoInfo((
                        "           * %ws (pdo=0x%p)\n",
                        ((DEVICE_EXTENSION*)(pDevice->DeviceExtension))->LinkName,
                        pdo
                        ));

                    // Increment the reference count to the PDO
                    ObReferenceObject(
                        pdo
                        );

                    // Add PDO to device relations
                    pDeviceRelations->Objects[pDeviceRelations->Count] = pdo;

                    // Increment number of objects
                    pDeviceRelations->Count++;
                }

                // Jump to next device
                pDevice = pDevice->NextDevice;
            }

            DebugPrintf_NoInfo((
                "\n"
                ));

            // Return address of device relations structure
            return PlxCompleteIrpWithInformation(
                       pIrp,
                       STATUS_SUCCESS,
                       (PLX_UINT_PTR)pDeviceRelations
                       );
        }
    }

    // Just pass to parent
    return DefaultPnpHandler(
               fdo,
               pIrp
               );
}




/******************************************************************************
 *
 * Function   :  OnRequestComplete
 *
 * Description:  Set an event when a lower driver complete an IRP
 *
 ******************************************************************************/
NTSTATUS
OnRequestComplete(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp,
    PKEVENT        pKEvent
    )
{
    KeSetEvent(
       pKEvent,
       0,
       FALSE
       );

    return STATUS_MORE_PROCESSING_REQUIRED;
}




/******************************************************************************
 *
 * Function   :  ForwardAndWait
 *
 * Description:  Forward request to lower level and await completion, used
 *               in PnP's IRP_MN_START_DEVICE
 *
 ******************************************************************************/
NTSTATUS
ForwardAndWait(
    PDEVICE_OBJECT fdo,
    PIRP           pIrp
    )
{
    KEVENT   event;
    NTSTATUS status;


    /* 
       Initialize a kernel event object to use in waiting for the lower-level
       driver to finish processing the object. It's a little known fact that the
       kernel stack *can* be paged, but only while someone is waiting in user 
       mode for an event to finish. Since neither we nor a completion routine 
       can be in the forbidden state, it's okay to put the event object on the 
       stack.
    */
    KeInitializeEvent(
        &event,
        NotificationEvent,
        FALSE
        );

    IoCopyCurrentIrpStackLocationToNext(
        pIrp
        );

    IoSetCompletionRoutine(
        pIrp,
        (PIO_COMPLETION_ROUTINE) OnRequestComplete,
        (PVOID) &event,
        TRUE,
        TRUE,
        TRUE
        );

    status =
        IoCallDriver(
            ((DEVICE_EXTENSION *) fdo->DeviceExtension)->pLowerDeviceObject,
            pIrp
            );

    if (status == STATUS_PENDING)
    {
        // Wait for completion
        KeWaitForSingleObject(
            &event,
            Executive,
            KernelMode,
            FALSE,
            NULL
            );

        return pIrp->IoStatus.Status;
    }

    return status;
}
