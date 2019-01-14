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
 *      Driver.c
 *
 * Description:
 *
 *      Initializes the driver and claims system resources for the device
 *
 * Revision History:
 *
 *      06-01-08 : PLX SDK v6.00
 *
 *****************************************************************************/


#include <stdio.h>
#include "ApiFunc.h"
#include "Dispatch.h"
#include "Driver.h"
#include "GlobalVars.h"
#include "PciFunc.h"
#include "PlugPlay.h"
#include "PlxChipFn.h"
#include "PlxInterrupt.h"
#include "Power.h"
#include "SuppFunc.h"
#include "DebugPrint.h"
#include "Si10Ioctl.h"
#include "..\Si6Driver\sys\Si6IOctl.h"
#include "..\USLDriver\sys\USLIoctl.h"



#if defined(ALLOC_PRAGMA) && !defined(PLX_DEBUG)
    #pragma alloc_text(INIT, DriverEntry)
#endif


/*******************************************************************************
 *
 * Function   :  DriverEntry
 *
 * Description:  Entry point for the driver
 *
 ******************************************************************************/
NTSTATUS
DriverEntry(
    DRIVER_OBJECT  *pDriverObject,
    UNICODE_STRING *pRegistryPath
    )
{
    PLX_REGISTRY_INFO RegistryInfo;

	DebugPrintInit("SI10");


    DebugPrint("<========================================================>");
    DebugPrint(
        "PLX driver v%d.%d%d (%d-bit) - built on %s %s",
        PLX_SDK_VERSION_MAJOR, PLX_SDK_VERSION_MINOR,
        PLX_SDK_VERSION_REVISION, (U32)(sizeof(PLX_UINT_PTR) * 8),
        __DATE__, __TIME__
        );

    DebugPrint(
        "Driver supports WDM v%x.%x",
        WDM_MAJORVERSION, WDM_MINORVERSION
        );

    // Default OS to Server 2003 or lower
    Gbl_WindowsVistaOrHigher = FALSE;

    // Check which OS version is running
    if (IoIsWdmVersionAvailable(6, 0))
    {
        DebugPrint("OS is Windows Vista (WDM v6.0) or higher");

        // Flag Vista or higher OS
        Gbl_WindowsVistaOrHigher = TRUE;
    }
    else if (IoIsWdmVersionAvailable(1, 0x30))
    {
        DebugPrint("OS is Windows Server 2003 (WDM v1.30)");
    }
    else if (IoIsWdmVersionAvailable(1, 0x20))
    {
        DebugPrint("OS is Windows XP (WDM v1.20)");
    }
    else if (IoIsWdmVersionAvailable(1, 0x10))
    {
        DebugPrint("OS is Windows 2000 (WDM v1.10)");
    }
    else
    {
        DebugPrint("OS is <Unknown> (WDM v?)");
    }

    // Fill in the appropriate dispatch handlers
    pDriverObject->DriverUnload                         = DriverUnload;
    pDriverObject->MajorFunction[IRP_MJ_CREATE]         = Dispatch_Create;
    pDriverObject->MajorFunction[IRP_MJ_CLOSE]          = Dispatch_Close;
    pDriverObject->MajorFunction[IRP_MJ_CLEANUP]        = Dispatch_Cleanup;
    pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = Dispatch_IoControl;
    pDriverObject->MajorFunction[IRP_MJ_PNP]            = Dispatch_Pnp;
    pDriverObject->MajorFunction[IRP_MJ_POWER]          = Dispatch_Power;
//    pDriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = Dispatch_SystemControl;
    pDriverObject->DriverExtension->AddDevice           = AddDevice;

    DebugPrint(
        "Driver Registry path = \"%ws\"",
        pRegistryPath->Buffer
        );

    // Get configuration information from registry
    PlxRegistryInformationGet(
        pRegistryPath,
        &RegistryInfo
        );

    // Initialize device count
    Gbl_DeviceCount = 0;

    // Initialize Common buffer pointer
    pGbl_CommonBuffer = NULL;

    // Set common buffer requested size
    Gbl_CommonBufferSize = RegistryInfo.Size_CommonBuffer;


    DebugPrint("...driver loaded 25/09 14:39");

    return STATUS_SUCCESS;
}




/*******************************************************************************
 *
 * Function   :  DriverUnload
 *
 * Description:  Unload the driver
 *
 ******************************************************************************/
VOID
DriverUnload(
    DRIVER_OBJECT *pDriverObject
    )
{
    DebugPrintf_NoInfo(("\n"));
    DebugPrint("Unloading driver...\n");

    DebugPrint("...driver unloaded\n");
}




/*******************************************************************************
 *
 * Function   :  AddDevice
 *
 * Description:  Add a new device object to the driver
 *
 ******************************************************************************/
NTSTATUS
AddDevice(
    DRIVER_OBJECT *pDriverObject,
    DEVICE_OBJECT *pdo
    )
{
    U8                i;
    WCHAR             DeviceName[PLX_MAX_NAME_LENGTH];
    WCHAR             DeviceLinkName[PLX_MAX_NAME_LENGTH];
    NTSTATUS          status;
    POWER_STATE       PowerState;
    UNICODE_STRING    DeviceName_Unicode;
    UNICODE_STRING    DeviceLinkName_Unicode;
    DEVICE_OBJECT    *fdo;
    DEVICE_EXTENSION *pdx;


    // Build a device name and attempt to create it
    for (i=0; i < 64; i++)
    {
        swprintf(
            DeviceName,
            L"\\Device\\" PLX_DRIVER_NAME_UNICODE L"_v%d%d%d-%d",
            PLX_SDK_VERSION_MAJOR,
            PLX_SDK_VERSION_MINOR,
            PLX_SDK_VERSION_REVISION,
            i
            );

        RtlInitUnicodeString(
            &DeviceName_Unicode,
            DeviceName
            );

        // Create the device object
        status =
            IoCreateDevice(
                pDriverObject,              // Driver object
                sizeof(DEVICE_EXTENSION),   // Device extension size
                &DeviceName_Unicode,        // Device name
                FILE_DEVICE_UNKNOWN,        // Device type
                FILE_DEVICE_SECURE_OPEN,    // Device characteristics
                FALSE,                      // Exclusive? (must be FALSE)
                &fdo                        // Returned device object
                );

        // IoCreateDevice() will fail if the same object already exists
        if (NT_SUCCESS(status))
        {
            break;
        }
    }

    // Check if the creation succeeded
    if ( !NT_SUCCESS(status) )
    {
        DebugPrint("ERROR - Unable to create Device\n");
        return status;
    }

    DebugPrintf_NoInfo(("\n"));
    DebugPrint(
        "Created Device (%ws)...\n",
        DeviceName
        );

    // Link a Win32 name for user applications
    swprintf(
        DeviceLinkName,
        L"\\DosDevices\\" PLX_DRIVER_NAME_UNICODE L"_v%d%d%d-%d",
        PLX_SDK_VERSION_MAJOR,
        PLX_SDK_VERSION_MINOR,
        PLX_SDK_VERSION_REVISION,
        i
        );

    RtlInitUnicodeString(
        &DeviceLinkName_Unicode,
        DeviceLinkName
        );




    DebugPrint(
        "Creating Win32 symbolic link (%ws)...\n",
        DeviceLinkName
        );

    status =
        IoCreateSymbolicLink(
            &DeviceLinkName_Unicode,
            &DeviceName_Unicode
            );

    if ( !NT_SUCCESS(status) )
    {
        DebugPrint("WARNING - Unable to create symbolic link for Win32 Apps\n");
        swprintf(DeviceLinkName, L"");
    }

    //
    // Initialize the device extension
    //

    pdx = fdo->DeviceExtension;


    // Clear device extension
    RtlZeroMemory(
        pdx,
        sizeof(DEVICE_EXTENSION)
        );

    // Store parent device object
    pdx->pDeviceObject         = fdo;
    pdx->pPhysicalDeviceObject = pdo;
    pdx->Key.DeviceNumber      = i;             // Store device number of device

    wcscpy(
        pdx->LinkName,
        DeviceLinkName
        );

    // Initialize hardware spinlock
    KeInitializeSpinLock(
        &(pdx->Lock_HwAccess)
        );

    // Initialize interrupt wait list
    InitializeListHead(
        &(pdx->List_WaitObjects)
        );

    KeInitializeSpinLock(
        &(pdx->Lock_WaitObjectsList)
        );

    // Initialize physical memories list
    InitializeListHead(
        &(pdx->List_PhysicalMem)
        );

    KeInitializeSpinLock(
        &(pdx->Lock_PhysicalMemList)
        );

    // Initialize BAR mappings list
    InitializeListHead(
        &(pdx->List_BarMappings)
        );

    KeInitializeSpinLock(
        &(pdx->Lock_BarMappingsList)
        );

    // Initialize the interrupt DPC
    KeInitializeDpc(
        &(pdx->DpcForIsr),
        DpcForIsr,
        pdx
        );

#if defined(PLX_DMA_SUPPORT)
    // Initialize DMA spinlock
    KeInitializeSpinLock(
        &(pdx->Lock_DmaChannel)
        );
#endif  // PLX_DMA_SUPPORT

    // Attach to device stack and store lower object for passing down IRPs later
    pdx->pLowerDeviceObject =
        IoAttachDeviceToDeviceStack(
            fdo,
            pdo
            );

    DebugPrint(
        "Attached device object to stack\n"
        "            Functional: 0x%p\n"
        "            Physical  : 0x%p\n"
        "            Lower     : 0x%p\n",
        fdo, pdo, pdx->pLowerDeviceObject
        );

    // Notify the power manager of the initial power state
    pdx->PowerState        = PowerDeviceD0;      // Start device in full power state
    PowerState.DeviceState = PowerDeviceD0;
    PoSetPowerState(
        fdo,
        DevicePowerState,
        PowerState
        );


    // Indicate the I/O Manager buffer management method
    fdo->Flags |= DO_BUFFERED_IO;
	fdo->Flags |= DO_DIRECT_IO;

    // Manually clear the Device Initialzing flag
    fdo->Flags &= ~DO_DEVICE_INITIALIZING;

	pdx->bUserShared = FALSE;
	pdx->bTimeSlotShared = FALSE;
	pdx->bUTShared = FALSE;
	DeRegisterEvent( pdx);

    return STATUS_SUCCESS;
}




/*******************************************************************************
 *
 * Function   :  RemoveDevice
 *
 * Description:  Remove a functional device object
 *
 ******************************************************************************/
VOID
RemoveDevice(
    DEVICE_OBJECT *fdo
    )
{
    NTSTATUS           status;
    UNICODE_STRING     DeviceLinkName_Unicode;
    DEVICE_EXTENSION  *pdx;


    pdx = fdo->DeviceExtension;

    // Remove Win32 link name
    if (wcslen(pdx->LinkName) != 0)
    {
        DebugPrint(
            "Removing Win32 link (%ws)\n",
            pdx->LinkName
            );

        RtlInitUnicodeString(
            &DeviceLinkName_Unicode,
            pdx->LinkName
            );

        status =
            IoDeleteSymbolicLink(
                &DeviceLinkName_Unicode
                );

        if ( !NT_SUCCESS(status) )
            DebugPrint("WARNING - Unable to remove Win32 link\n");
    }

    // Detach device from the device object stack
    if (pdx->pLowerDeviceObject)
    {
        IoDetachDevice(
            pdx->pLowerDeviceObject
            );
    }

    DebugPrint("Deleting device object...\n");

    // Delete the device object
    IoDeleteDevice(
        fdo
        );
}




/*******************************************************************************
 *
 * Function   :  StartDevice
 *
 * Description:  Start a device
 *
 ******************************************************************************/
NTSTATUS
StartDevice(
    DEVICE_OBJECT            *fdo,
    CM_PARTIAL_RESOURCE_LIST *ResourceListRaw,
    CM_PARTIAL_RESOURCE_LIST *ResourceList
    )
{
    U8                              i;
    U8                              BarIndex;
    U32                             vector;
    U32                             BarSize;
    U32                             RegValue;
    U32                             PciBars[PCI_NUM_BARS_TYPE_00];
    U64                             BarBusAddr;
    U64                             BarCpuAddr;
    KIRQL                           IrqL;
    BOOLEAN                         bIntPresent;
    NTSTATUS                        status;
    KAFFINITY                       affinity;
    KINTERRUPT_MODE                 mode;
    PLX_PHYSICAL_MEM                PciMem;
    DEVICE_EXTENSION               *pdx;
    CM_PARTIAL_RESOURCE_DESCRIPTOR *ResourceRaw;
    CM_PARTIAL_RESOURCE_DESCRIPTOR *Resource;
	int								ii;


    pdx         = fdo->DeviceExtension;
    bIntPresent = FALSE;
    ResourceRaw = ResourceListRaw->PartialDescriptors;
    Resource    = ResourceList->PartialDescriptors;

    // Read the PCI base addresses from the device
    PciRegisterRead_UsingPnP(
        fdo,
        0x10,       // Start at BAR 0
        &PciBars,
        PCI_NUM_BARS_TYPE_00 * sizeof(U32)
        );

    // Get bus and slot numbers of the device
    status =
        GetBusSlotNumber(
            pdx->pPhysicalDeviceObject,
            pdx,
            PciBars[0]
            );

    if (!NT_SUCCESS(status))
    {
        DebugPrint("WARNING - Unable to get bus and slot number\n");
    }

    DebugPrint(
        "Resource list contains %d descriptors\n",
        ResourceListRaw->Count
        );

    for (i = 0; i < ResourceListRaw->Count; ++i, ++Resource, ++ResourceRaw)
    {
        DebugPrintf_NoInfo(("            Resource %02d\n", i));

        switch (ResourceRaw->Type)
        {
            case CmResourceTypeInterrupt:
                bIntPresent = TRUE;
                IrqL        = (KIRQL) Resource->u.Interrupt.Level;
                vector      = Resource->u.Interrupt.Vector;
                affinity    = Resource->u.Interrupt.Affinity;

                DebugPrint("      Type     : Interrupt");
                DebugPrint("      Vector   : 0x%x        (Translated = 0x%x)",ResourceRaw->u.Interrupt.Vector, vector);
				DebugPrint("      IRQL     : 0x%x        (Translated = 0x%x)", ResourceRaw->u.Interrupt.Level, IrqL);
                DebugPrint("      Affinity : 0x%x", ResourceRaw->u.Interrupt.Affinity);
           
                if (ResourceRaw->Flags == CM_RESOURCE_INTERRUPT_LATCHED)
                {
                    mode = Latched;
                    DebugPrint("              Mode     : Latched");
                }
                else
                {
                    mode = LevelSensitive;
                    DebugPrint("              Mode     : Level Sensitive");
                }
                break;

            case CmResourceTypePort:
            case CmResourceTypeMemory:
                DebugPrintf_NoInfo((
                    "              Type     : %s\n",
                    (Resource->Type == CmResourceTypePort) ? "I/O Port" : "Memory Space"
                    ));

                // Get BAR address & size
                if (Resource->Type == CmResourceTypePort)
                {
                    BarBusAddr = ResourceRaw->u.Port.Start.QuadPart;
                    BarCpuAddr = Resource->u.Port.Start.QuadPart;
                    BarSize    = Resource->u.Port.Length;
                }
                else
                {
                    BarBusAddr = ResourceRaw->u.Memory.Start.QuadPart;
                    BarCpuAddr = Resource->u.Memory.Start.QuadPart;
                    BarSize    = Resource->u.Memory.Length;
                }

                BarIndex =
                    GetBarIndex(
                        pdx,
                        (U32)BarBusAddr,
                        PciBars
                        );

                if (BarIndex == (U8)-1)
                {
                    // Unable to find the BAR index
                    DebugPrintf_NoInfo((
                        "              PCI BAR ?: ERROR - unable to detect index for BAR (%08lX)\n",
                        (PLX_UINT_PTR)BarBusAddr
                        ));
                }
                else
                {
                    // Record resources
                    pdx->PciBar[BarIndex].Properties.BarValue = (U32)BarBusAddr;
                    pdx->PciBar[BarIndex].Properties.Physical = BarCpuAddr;
                    pdx->PciBar[BarIndex].Properties.Size     = BarSize;

                    if (Resource->Type == CmResourceTypePort)
                        pdx->PciBar[BarIndex].Properties.bIoSpace = TRUE;

                    DebugPrintf_NoInfo((
                        "              PCI BAR %d: %08lX\n",
                        BarIndex, (PLX_UINT_PTR)BarBusAddr
                        ));
                }

                DebugPrintf_NoInfo((
                    "              Phys Addr: %08lX\n"
                    "              Size     : %08X  ",
                    (PLX_UINT_PTR)BarCpuAddr,
                    BarSize
                    ));

                if (BarSize >= (1 << 10))
                {
                    DebugPrintf_NoInfo((
                        "(%d Kb)\n",
                        BarSize >> 10
                        ));
                }
                else
                {
                    DebugPrintf_NoInfo((
                        "(%d Bytes)\n",
                        BarSize
                        ));
                }

                // For memory spaces, map to kernel space
                if ((Resource->Type == CmResourceTypeMemory) && (BarIndex != (U8)-1))
                {
                    // Set prefetch flag
                    if (Resource->Flags & CM_RESOURCE_MEMORY_PREFETCHABLE)
                    {
                        pdx->PciBar[BarIndex].Properties.bPrefetchable = TRUE;
                    }

                    DebugPrintf_NoInfo((
                        "              Prefetch?: %s\n",
                        (pdx->PciBar[BarIndex].Properties.bPrefetchable) ? "Yes" : "No"
                        ));

                    // Map to kernel space
                    DebugPrintf_NoInfo(("              Kernel VA: "));

                    status =
                        PlxPciBarResourceMap(
                            pdx,
                            BarIndex
                            );

                    if ( NT_SUCCESS(status) )
                    {
                        DebugPrintf_NoInfo((
                            "%p\n",
                            pdx->PciBar[BarIndex].pVa
                            ));
                    }
                    else
                    {
                        DebugPrintf_NoInfo((
                            "ERROR - Unable to map %08lX ==> Kernel VA\n",
                            (PLX_UINT_PTR)BarCpuAddr
                            ));
                    }
                }
                break;

            case CmResourceTypeNull:
                DebugPrintf_NoInfo(("              Type: Null (unsupported)\n"));
                break;

            case CmResourceTypeDma:
                DebugPrintf_NoInfo(("              Type: DMA (unsupported)\n"));
                break;

            case CmResourceTypeDeviceSpecific:
                DebugPrintf_NoInfo(("              Type: Device Specific (unsupported)\n"));
                break;

            case CmResourceTypeBusNumber:
                DebugPrintf_NoInfo(("              Type: Bus Number (unsupported)\n"));
                break;

            case CmResourceTypeDevicePrivate:
                DebugPrintf_NoInfo(("              Type: Device Private Data (ignoring - reserved for system use)\n"));
                break;

            case CmResourceTypePcCardConfig:
                DebugPrintf_NoInfo(("              Type: PC Card Configuration (unsupported)\n"));
                break;

            case CmResourceTypeMfCardConfig:
                DebugPrintf_NoInfo(("              Type: Multi-function Card Configuration (unsupported)\n"));
                break;

            default:
                DebugPrintf_NoInfo(("              Type: ?Unknown Resource Type? (%d)\n", Resource->Type));
                break;
        }
    }

    // Make sure BAR 0 exists or the device can't be started
    if (pdx->PciBar[0].pVa == NULL)
    {
        ErrorPrintf(("ERROR - BAR 0 mapping is required for register access\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    // Fill in remaining device key information
    PlxUpdateDeviceKey(
        pdx
        );

    // Determine & store the PLX chip type
    PlxChipTypeGet(
        pdx,
        &pdx->Key,
        &pdx->PlxChipType,
        &pdx->PlxRevision
        );

    // Disable all interrupts
    PlxChipInterruptsDisable(
        pdx
        );

    // Install the ISR if available
    if (bIntPresent)
    {
        status =
            IoConnectInterrupt(
                &pdx->pInterruptObject,
                OnInterrupt,
                pdx,
                NULL,
                vector,
                IrqL,
                IrqL,
                mode,
                TRUE,
                affinity,
                FALSE
                );

        if ( !NT_SUCCESS(status) )
        {
            ErrorPrintf(("ERROR - Unable to install ISR\n"));
        }
        else
        {
            DebugPrint("Installed ISR for interrupt\n");

            // Re-enable interrupts
            KeSynchronizeExecution(
              pdx->pInterruptObject,
              PlxChipInterruptsEnable,
              pdx
              );
        }
    }
    else
    {
        DebugPrint("Device is not using a PCI interrupt resource\n");
    }

    // Allocate a DMA adapter object for physical memory allocations
    PlxDmaAdapterAllocate(
        pdx
        );

    // If this is the first device, allocate buffers
    if (Gbl_DeviceCount == 0)
    {
        if (Gbl_CommonBufferSize != 0)
        {
            DebugPrint(
                "Allocating common buffer...\n"
                );

            // Set requested size
            PciMem.Size = Gbl_CommonBufferSize;

            // Allocate common buffer
            PlxPciPhysicalMemoryAllocate(
                pdx,
                &PciMem,
                TRUE,                   // Smaller buffer is ok
                pdx                     // Assign owner as the device
                );
        }

        /********************************************************
         * Since this is the first device, make sure to invalidate
         * its removal relations.  This forces the PnP Manager
         * to eventually query for removal relations before
         * attempting to remove the device.  Devices added after
         * this one rely on it for the Common DMA and SGL buffers;
         * therefore, it can only be removed last.
         *******************************************************/
        IoInvalidateDeviceRelations(
            pdx->pPhysicalDeviceObject,
            RemovalRelations
            );
    }

    // Increment our device count
    InterlockedIncrement(
        &Gbl_DeviceCount
        );

	//USL INITIALIZATION
	nTable[0]=0xffff;
	for(ii=1;ii<=10000;ii++) {
		nTable[ii]=1000000/ii;
		if(nTable[ii]>0xffff) nTable[ii]=0xffff;
	};

    return STATUS_SUCCESS;
}




/*******************************************************************************
 *
 * Function   :  StopDevice
 *
 * Description:  Stop a device
 *
 ******************************************************************************/
VOID
StopDevice(
    DEVICE_OBJECT *fdo
    )
{
    PLX_PHYSICAL_MEM  PciMem;
    DEVICE_EXTENSION *pdx;


    pdx = fdo->DeviceExtension;

    // Free all interrupt resources
    if (pdx->pInterruptObject != NULL)
    {
        // Disable all interrupts
        KeSynchronizeExecution(
            pdx->pInterruptObject,
            (PKSYNCHRONIZE_ROUTINE)PlxChipInterruptsDisable,
            (PVOID)pdx
            );

        // Remove the ISR
        IoDisconnectInterrupt(
            pdx->pInterruptObject
            );

        pdx->pInterruptObject = NULL;
    }

    // Release the common buffer if this device owns it
    if (pGbl_CommonBuffer != NULL)
    {
        if (pGbl_CommonBuffer->pOwner == pdx)
        {
            // Release common buffer
            DebugPrint(
                "De-allocating Common Buffer...\n"
                );

            // Prepare buffer properties for parameter
            PciMem.PhysicalAddr = pGbl_CommonBuffer->BusPhysical;
            PciMem.Size         = pGbl_CommonBuffer->Size;

            // Release the buffer
            PlxPciPhysicalMemoryFree(
                pdx,
                &PciMem
                );

            // Mark buffer as released
            pGbl_CommonBuffer = NULL;
        }
    }

    // Release the DMA adapter object
    PlxDmaAdapterFree(
        pdx
        );

    // Unmap I/O regions from kernel space (No local register access after this)
    PlxPciBarResourcesUnmap(
        pdx
        );

    // Decrement our device count
    InterlockedDecrement(
        &Gbl_DeviceCount
        );
}




/*******************************************************************************
 *
 * Function   :  PlxDmaAdapterAllocate
 *
 * Description:  Allocate a DMA adapter object which will provide support for
 *               later allocation of physically contiguous page-locked memory.
 *
 ******************************************************************************/
NTSTATUS
PlxDmaAdapterAllocate(
    DEVICE_EXTENSION *pdx
    )
{
    ULONG               NumMapRegisters;
    DEVICE_DESCRIPTION  DeviceDescription;


    DebugPrint(
        "Allocating DMA Adapter object...\n"
        );

    // Verify object not already created
    if (pdx->pDmaAdapter != NULL)
    {
        DebugPrint("ERROR - DMA Adapter object already exist, unable to allocate\n");
        return STATUS_OBJECT_NAME_EXISTS;
    }

    // Clear device description
    RtlZeroMemory(
        &DeviceDescription,
        sizeof(DEVICE_DESCRIPTION)
        );

    // Set device DMA properties
    DeviceDescription.Version            = DEVICE_DESCRIPTION_VERSION;
    DeviceDescription.Master             = TRUE;                    // Device is bus master
    DeviceDescription.ScatterGather      = TRUE;                    // Device supports SGL
    DeviceDescription.Dma32BitAddresses  = TRUE;                    // Device supports 32-bit addressing
    DeviceDescription.Dma64BitAddresses  = FALSE;                   // Don't use 64-bit addressing
    DeviceDescription.InterfaceType      = PCIBus;                  // Device is PCI
    DeviceDescription.MaximumLength      = (8 << 20) - 1;           // Max bytes per DMA xfer (8MB-1)

    // OS will assign map register count
    NumMapRegisters = 0;

    // Allocate a DMA adapter object
    pdx->pDmaAdapter =
        IoGetDmaAdapter(
            pdx->pPhysicalDeviceObject,
            &DeviceDescription,
            &NumMapRegisters
            );

    if (pdx->pDmaAdapter == NULL)
    {
        DebugPrint("ERROR - DMA Adapter allocation failed\n");
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    return STATUS_SUCCESS;
}




/*******************************************************************************
 *
 * Function   :  PlxDmaAdapterFree
 *
 * Description:  Frees the device DMA adapter object
 *
 ******************************************************************************/
NTSTATUS
PlxDmaAdapterFree(
    DEVICE_EXTENSION *pdx
    )
{
    DebugPrint(
        "Releasing DMA Adapter object...\n"
        );

    // Verify DMA Adapter was created
    if (pdx->pDmaAdapter == NULL)
    {
        DebugPrint("ERROR - DMA Adapter object doesn't exist, unable to free object\n");
        return STATUS_RESOURCE_TYPE_NOT_FOUND;
    }

    // Delete the DMA Adapter
    pdx->pDmaAdapter->DmaOperations->PutDmaAdapter(
        pdx->pDmaAdapter
        );

    // DMA adapter object deleted, no longer available
    pdx->pDmaAdapter = NULL;

    return STATUS_SUCCESS;
}
